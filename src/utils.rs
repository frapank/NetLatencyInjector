use std::process::Command;
use thiserror::Error;

#[derive(Debug, Error)]
pub enum NetemError {
    #[error("tc returned non-zero exit status: {0}")]
    TcExitError(String),

    #[error("io error: {0}")]
    Io(#[from] std::io::Error),
}

pub fn list_interfaces() -> std::io::Result<Vec<String>> {
    let mut ifaces: Vec<String> = std::fs::read_dir("/sys/class/net/")?
        .filter_map(std::io::Result::ok)
        .map(|e| e.file_name().to_string_lossy().into_owned())
        .filter(|name| name != "lo")
        .collect();
    ifaces.sort();
    Ok(ifaces)
}

pub fn get_delay(iface: &str) -> Result<Option<u32>, NetemError> {
    let output = Command::new("tc")
        .args(["qdisc", "show", "dev", iface])
        .output()?;

    if !output.status.success() {
        return Err(NetemError::TcExitError(
            String::from_utf8_lossy(&output.stderr).into_owned(),
        ));
    }

    let stdout = String::from_utf8_lossy(&output.stdout);

    for line in stdout.lines() {
        if let Some(pos) = line.find("delay ") {
            let part = &line[pos + 6..];

            if let Some(end) = part.find("ms") {
                let num = &part[..end];

                if let Ok(val) = num.trim().parse::<f32>() {
                    return Ok(Some(val.round() as u32));
                }
            }
        }
    }

    Ok(None)
}

pub fn set_delay(iface: &str, ms: u32) -> Result<(), NetemError> {
    let delay = format!("{ms}ms");

    let output = Command::new("tc")
        .args([
            "qdisc",
            "replace",
            "dev",
            iface,
            "root",
            "netem",
            "delay",
            delay.as_str(),
        ])
        .output()?;

    if !output.status.success() {
        return Err(NetemError::TcExitError(
            String::from_utf8_lossy(&output.stderr).into_owned(),
        ));
    }

    Ok(())
}

pub fn clear_delay(iface: &str) -> Result<(), NetemError> {
    let output = Command::new("tc")
        .args(["qdisc", "del", "dev", iface, "root"])
        .output()?;

    if !output.status.success() {
        return Err(NetemError::TcExitError(
            String::from_utf8_lossy(&output.stderr).into_owned(),
        ));
    }

    Ok(())
}
