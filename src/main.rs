use crossterm::event;
use log::error;
use std::error::Error;
use std::path::Path;
use std::process::ExitCode;

fn run() -> Result<(), Box<dyn Error>> {
    ratatui::run(|terminal| {
        loop {
            terminal.draw(|frame| frame.render_widget("Hello World!", frame.area()))?;
            if event::read()?.is_key_press() {
                break Ok::<(), Box<dyn Error>>(());
            }
        }
    })?;

    Ok(())
}

fn main() -> ExitCode {
    env_logger::init();

    match run() {
        Ok(()) => ExitCode::SUCCESS,
        Err(e) => {
            error!("{e}");
            ExitCode::FAILURE
        }
    }
}
