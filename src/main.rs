#![warn(clippy::pedantic)]

mod app;
mod gui;
mod utils;

use std::process::ExitCode;

use log::error;

use app::ProgramContext;
use gui::tui::Gui;

fn run_program() -> Result<(), Box<dyn std::error::Error>> {
    let ctx = ProgramContext::new()?; 

    Gui::run(ctx)
}

fn main() -> ExitCode {
    env_logger::init();

    match run_program() {
        Ok(()) => ExitCode::SUCCESS,
        Err(e) => {
            error!("{e}");
            ExitCode::FAILURE
        }
    }
}
