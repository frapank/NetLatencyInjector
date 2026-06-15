use ratatui::layout::{Alignment, Constraint, Layout};
use ratatui::style::{Color, Style};
use ratatui::text::Line;
use ratatui::widgets::{Block, BorderType, Borders, List, ListItem, Paragraph};

use crate::Interface;
use crate::ProgramContext;
use crate::list_interfaces;
use crossterm::event::{self, Event, KeyCode, KeyEventKind};

use std::error::Error;

pub struct Gui {
    ctx: ProgramContext,
}

impl Gui {
    pub fn run() -> Result<(), Box<dyn Error>> {
        let mut gui = Self::new()?;

        ratatui::run(|terminal| {
            loop {
                terminal.draw(|frame| gui.render(frame))?;

                if gui.handle_events()? {
                    break Ok::<(), Box<dyn Error>>(());
                }
            }
        })?;

        Ok(())
    }

    pub fn render(&self, frame: &mut ratatui::Frame) {
        let general = Layout::default()
            .direction(ratatui::layout::Direction::Vertical)
            .margin(1)
            .constraints(vec![Constraint::Length(3), Constraint::Min(0)])
            .split(frame.area());

        let rounded = Block::default()
            .borders(Borders::ALL)
            .border_type(BorderType::Rounded);

        frame.render_widget(
            Paragraph::new("NetLatencyInjector")
                .alignment(Alignment::Center)
                .block(rounded.clone()),
            general[0],
        );

        let items: Vec<ListItem> = self
            .ctx
            .interf_vec
            .iter()
            .enumerate()
            .map(|(idx, interf)| {
                if idx == self.ctx.interf_sel {
                    ListItem::new(Line::from(format!("> {}", interf.name)))
                        .style(Style::default().fg(Color::Black).bg(Color::LightBlue))
                } else {
                    ListItem::new(Line::from(format!("  {}", interf.name)))
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

    pub fn handle_events(&mut self) -> std::io::Result<bool> {
        match event::read()? {
            Event::Key(key) if key.kind == KeyEventKind::Press => match key.code {
                KeyCode::Char('q') => return Ok(true),
                KeyCode::Char('j') => {
                    self.ctx.interf_sel = (self.ctx.interf_sel + 1) % self.ctx.interf_vec.len();
                }
                KeyCode::Char('k') => {
                    if self.ctx.interf_sel == 0 {
                        self.ctx.interf_sel = self.ctx.interf_vec.len() - 1;
                    } else {
                        self.ctx.interf_sel -= 1;
                    }
                }
                _ => {}
            },
            _ => {}
        }
        Ok(false)
    }

    fn new() -> Result<Self, Box<dyn std::error::Error>> {
        let ctx = ProgramContext {
            interf_vec: list_interfaces()?
                .into_iter()
                .map(|name| Interface { name })
                .collect(),
            interf_sel: 0,
        };

        Ok(Self { ctx })
    }
}
