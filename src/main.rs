mod utils;

use crossterm::event::{self, Event, KeyCode, KeyEventKind};
use log::error;
use ratatui::layout::{Constraint, Layout};
use ratatui::style::{Color, Stylize};
use ratatui::widgets::{Block, Borders, Paragraph};
use std::error::Error;
use std::process::ExitCode;
use utils::list_interfaces;

struct Interface {
    name: String,
    //curr_delay: u32,
}

struct ProgramContext {
    interf_vec: Vec<Interface>,
}

fn run() -> Result<(), Box<dyn Error>> {
    let mut ctx: ProgramContext = ProgramContext {
        interf_vec: list_interfaces()?
            .into_iter()
            .map(|name| Interface { name })
            .collect(),
    };

    ctx.interf_vec.push(Interface {
        name: "aaa".to_string(),
    });

    ratatui::run(|terminal| {
        loop {
            terminal.draw(|frame| render(frame, &ctx))?;

            if handle_events()? {
                break Ok::<(), Box<dyn Error>>(());
            }
        }
    })?;

    Ok(())
}

fn render(frame: &mut ratatui::Frame, ctx: &ProgramContext) {
    let general = Layout::default()
        .direction(ratatui::layout::Direction::Vertical)
        .margin(1)
        .constraints(vec![Constraint::Length(3), 
            Constraint::Length(1), // Padding
            Constraint::Percentage(100)])
        .split(frame.area());

    frame.render_widget(
        Paragraph::new("Commands: [q]uit").block(Block::new().borders(Borders::ALL)),
        general[0],
    );

    let aval_interf = Layout::default()
        .direction(ratatui::layout::Direction::Vertical)
        .constraints(vec![Constraint::Length(3); ctx.interf_vec.len()])
        .split(general[2]);

    for (interf, area) in ctx.interf_vec.iter().zip(aval_interf.iter()) {
        frame.render_widget(
            Paragraph::new(interf.name.as_str()).block(Block::new().borders(Borders::ALL)),
            *area,
        );
    }
}

fn handle_events() -> std::io::Result<bool> {
    match event::read()? {
        Event::Key(key) if key.kind == KeyEventKind::Press => match key.code {
            KeyCode::Char('q') => return Ok(true),
            _ => {}
        },
        _ => {}
    }
    Ok(false)
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
