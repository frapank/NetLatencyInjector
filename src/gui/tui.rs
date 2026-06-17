use crate::app::ProgramContext;
use crate::utils::{NetemError, set_delay};
use crossterm::event::{self, Event, KeyCode, KeyEventKind};
use ratatui::layout::{Alignment, Constraint, Layout, Rect};
use ratatui::style::{Color, Style};
use ratatui::text::Line;
use ratatui::widgets::{Block, BorderType, Borders, Clear, List, ListItem, Paragraph};
use std::error::Error;
use std::ops::ControlFlow;

const TITLE_HEIGHT: u16 = 3;

pub struct Gui {
    ctx: ProgramContext,
    popup_open: bool,
    popup_input: String,
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

        frame.render_widget(
            List::new(items).block(
                Block::default()
                    .borders(Borders::ALL)
                    .border_type(BorderType::Rounded)
                    .title(" Interfaces "),
            ),
            general[1],
        );

        if self.popup_open {
            let full = frame.area();

            let area = Self::centered_rect(40, 3, full);

            frame.render_widget(Clear, area);

            frame.render_widget(
                Paragraph::new(self.popup_input.as_str()).block(
                    Block::default()
                        .title(" Delay in ms — ESC close ")
                        .title_alignment(Alignment::Center)
                        .borders(Borders::ALL)
                        .border_type(BorderType::Rounded)
                        .border_style(Style::default().fg(Color::Yellow)),
                ),
                area,
            );
        }
    }

    fn centered_rect(percent_x: u16, height_lines: u16, r: Rect) -> Rect {
        let vertical = Layout::vertical([
            Constraint::Fill(1),
            Constraint::Length(height_lines),
            Constraint::Fill(1),
        ])
        .split(r);

        Layout::horizontal([
            Constraint::Percentage((100 - percent_x) / 2),
            Constraint::Percentage(percent_x),
            Constraint::Percentage((100 - percent_x) / 2),
        ])
        .split(vertical[1])[1]
    }

    pub fn handle_events(&mut self) -> Result<ControlFlow<()>, NetemError> {
        match event::read()? {
            Event::Key(key) if key.kind == KeyEventKind::Press => {
                if self.popup_open {
                    match key.code {
                        KeyCode::Enter => {
                            if let Ok(delay) = self.popup_input.trim().parse::<u32>() {
                                if let Some(iface) =
                                    self.ctx.interf_vec.get_mut(self.ctx.interf_sel)
                                {
                                    iface.delay = delay;
                                    if let Err(error) = set_delay(&iface.name, delay) {
                                        return Err(error);
                                    }
                                }
                            }
                            self.close_popup();
                        }
                        KeyCode::Esc => self.close_popup(),
                        KeyCode::Backspace => {
                            self.popup_input.pop();
                        }
                        KeyCode::Char(c) if c.is_ascii_digit() => {
                            self.popup_input.push(c);
                        }
                        _ => {}
                    }
                } else {
                    match key.code {
                        KeyCode::Char('q') => return Ok(ControlFlow::Break(())),
                        KeyCode::Enter => self.open_sel(),
                        KeyCode::Char('j') if !self.ctx.interf_vec.is_empty() => {
                            self.ctx.interf_sel =
                                (self.ctx.interf_sel + 1) % self.ctx.interf_vec.len();
                        }
                        KeyCode::Char('k') if !self.ctx.interf_vec.is_empty() => {
                            if self.ctx.interf_sel == 0 {
                                self.ctx.interf_sel = self.ctx.interf_vec.len() - 1;
                            } else {
                                self.ctx.interf_sel -= 1;
                            }
                        }
                        _ => {}
                    }
                }
            }
            _ => {}
        }
        Ok(ControlFlow::Continue(()))
    }

    fn open_sel(&mut self) {
        self.popup_input.clear();
        self.popup_open = true;
    }

    fn close_popup(&mut self) {
        self.popup_input.clear();
        self.popup_open = false;
    }

    fn new(ctx: ProgramContext) -> Self {
        Self {
            ctx,
            popup_open: false,
            popup_input: String::new(),
        }
    }
}
