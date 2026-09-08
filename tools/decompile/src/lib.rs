//! Recover candidate C from bounded Thumb instruction windows. Owner lookup
//! supplies game context; lifting and composition share explicit unit tables.
//! Generated source still requires compilation and complete-byte comparison.

pub mod adopt;
pub mod decode;
pub mod imports;
pub mod lift;
pub mod owners;
pub mod sched;
pub mod unit;

use std::path::Path;

/// Lifts an owner from the canonical ROM. `span` falls back to the register.
pub fn lift_owner(
    root: &Path,
    owner: &str,
    span: Option<u32>,
    name: Option<&str>,
) -> Result<(String, u32), String> {
    let (image, base, entry, span) = owners::image_window(root, owner, span)?;
    let name = name.map(str::to_string).unwrap_or_else(|| {
        let prefix = if base == decode::MAIN_BASE {
            "Func"
        } else {
            "Lifted"
        };
        format!("{prefix}_{entry:08x}")
    });
    lift::set_main_mode(base == decode::MAIN_BASE);
    let ins = decode::decode_window_at(&image, base, entry, span);
    let (body, tables) = unit::bodies(&ins);
    let text = unit::compose(entry, &name, &body, &tables);
    lift::set_main_mode(false);
    Ok((text, span))
}
pub mod cli;
