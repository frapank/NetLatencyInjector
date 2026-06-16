use ratatui::layout::{Alignment, Constraint, Layout};
use ratatui::style::{Color, Style};
use ratatui::text::Line;
use ratatui::widgets::{Block, BorderType, Borders, List, ListItem, Paragraph};

use crossterm::event::{self, Event, KeyCode, KeyEventKind};
use std::error::Error;
use std::ops::ControlFlow;

use crate::app::ProgramContext;

const TITLE_HEIGHT: u16 = 3;

pub struct Gui {
    ctx: ProgramContext,
}

impl Gui {
    pub fn run(ctx: ProgramContext) -> Result<(), Box<dyn Error>> {
        let mut gui = Self::new(ctx);

        ratatui::run(|terminal| {
            loop {
                terminal.draw(|frame| gui.render(frame))?;

                if gui.handle_events()?.is_break() {
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
            .constraints(vec![Constraint::Length(TITLE_HEIGHT), Constraint::Min(0)])
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
                    ListItem::new(Line::from(format!(
                        " > {} [{}ms]",
                        interf.name, interf.delay
                    )))
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

    pub fn handle_events(&mut self) -> std::io::Result<ControlFlow<()>> {
        match event::read()? {
            Event::Key(key) if key.kind == KeyEventKind::Press => match key.code {
                KeyCode::Char('q') => return Ok(ControlFlow::Break(())),
                KeyCode::Char('j') if !self.ctx.interf_vec.is_empty() => {
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
        Ok(ControlFlow::Continue(()))
    }

    fn new(ctx: ProgramContext) -> Self {
        Self { ctx }
    }
}
