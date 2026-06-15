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

fn main() -> ExitCode {
    env_logger::init();

    match Gui::run() {
        Ok(()) => ExitCode::SUCCESS,
        Err(e) => {
            error!("{e}");
            ExitCode::FAILURE
        }
    }
}
