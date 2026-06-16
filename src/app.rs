use crate::utils::list_interfaces;
use crate::utils::get_delay;

pub struct Interface {
    pub name: String,
    pub delay: u32
}

pub struct ProgramContext {
    pub interf_vec: Vec<Interface>,
    pub interf_sel: usize,
}

impl ProgramContext {
    pub fn new() -> Result<Self, crate::utils::NetemError> {
        let mut interfaces = Vec::new();

        for name in list_interfaces()? {
            let delay = get_delay(&name)?.unwrap_or(0);

            interfaces.push(Interface { name, delay });
        }

        Ok(Self {
            interf_vec: interfaces,
            interf_sel: 0,
        })
    }
}
