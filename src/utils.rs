pub fn list_interfaces() -> std::io::Result<Vec<String>> {
    let mut ifaces: Vec<String> = std::fs::read_dir("/sys/class/net/")?
        .filter_map(|e| e.ok())
        .map(|e| e.file_name().to_string_lossy().into_owned())
        .filter(|name| name != "lo")
        .collect();
    ifaces.sort();
    Ok(ifaces)
}


