mod utils;

use std::error::Error;
use std::process::ExitCode;

use crossterm::event::{self, Event, KeyCode, KeyEventKind};
use log::error;

use ratatui::layout::{Alignment, Constraint, Layout};
use ratatui::style::{Color, Style};
use ratatui::text::Line;
use ratatui::widgets::{Block, BorderType, Borders, List, ListItem, Paragraph};

use utils::list_interfaces;

struct Interface {
    name: String,
    //curr_delay: u32,
}

struct ProgramContext {
    interf_vec: Vec<Interface>,
    interf_sel: usize,
}

fn run() -> Result<(), Box<dyn Error>> {
    let mut ctx: ProgramContext = ProgramContext {
        interf_vec: list_interfaces()?
            .into_iter()
            .map(|name| Interface { name })
            .collect(),
            interf_sel: 0
    };

    ctx.interf_vec.push(Interface {
        name: "aaa".to_string(),
    });

    ratatui::run(|terminal| {
        loop {
            terminal.draw(|frame| render(frame, &ctx))?;

            if handle_events(&mut ctx)? {
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
        .constraints(vec![Constraint::Length(3), Constraint::Min(0)])
        .split(frame.area());

    let rounded = Block::default()
        .borders(Borders::ALL)
        .border_type(BorderType::Rounded);

    frame.render_widget(
        Paragraph::new("NetLatencyInjector").alignment(Alignment::Center).block(rounded.clone()),
        general[0],
    );

    let items: Vec<ListItem> = ctx
    .interf_vec
    .iter()
    .enumerate()
    .map(|(idx, interf)| {
        if idx == ctx.interf_sel {
            ListItem::new(
                Line::from(format!("> {}", interf.name))
            )
            .style(
                Style::default()
                    .fg(Color::Black)
                    .bg(Color::LightBlue)
            )
        } else {
            ListItem::new(
                Line::from(format!("  {}", interf.name))
            )
        }
    })
    .collect();

    let list = List::new(items).block(
        Block::default()
            .borders(Borders::ALL)
            .border_type(BorderType::Rounded)
            .title(" Interfaces "),
    );

    frame.render_widget(list, general[1]);
}

fn handle_events(ctx: &mut ProgramContext) -> std::io::Result<bool> {
    match event::read()? {
        Event::Key(key) if key.kind == KeyEventKind::Press => match key.code {
            KeyCode::Char('q') => return Ok(true),
            KeyCode::Char('j') => {
                ctx.interf_sel = (ctx.interf_sel + 1) % ctx.interf_vec.len();
            },
            KeyCode::Char('k') => {
                if ctx.interf_sel == 0 {
            ctx.interf_sel = ctx.interf_vec.len() - 1;
        } else {
            ctx.interf_sel -= 1;
        }
    },
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
