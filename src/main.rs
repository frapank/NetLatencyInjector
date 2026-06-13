mod utils;

use utils::list_interfaces;
use crossterm::event;
use log::error;
use std::error::Error;
use std::process::ExitCode;

fn run() -> Result<(), Box<dyn Error>> {

    ratatui::run(|terminal| {
        let list = list_interfaces().unwrap();
        for l in list {
            terminal.draw(|frame| frame.render_widget(l, frame.area()))?;
        }
        loop {
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
