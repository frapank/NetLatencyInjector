mod gui;
mod utils;

use std::process::ExitCode;

use log::error;

use crate::gui::gui::Gui;
use utils::list_interfaces;

struct Interface {
    name: String,
}

struct ProgramContext {
    interf_vec: Vec<Interface>,
    interf_sel: usize,
}

fn run_program() -> Result<(), Box<dyn std::error::Error>> {
    let ctx = ProgramContext {
        interf_vec: list_interfaces()?
            .into_iter()
            .map(|name| Interface { name })
            .collect(),
            interf_sel: 0,
    };

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
