//! Thumb-to-C lifting for retained overlay modules. The output is a
//! candidate translation unit tuned to recompile byte-exactly through the
//! GCC 2.96 overlay route rather than to read well.

pub mod decode;
pub mod lift;
pub mod owners;
pub mod sched;
pub mod tune;
pub mod unit;

use std::path::Path;

/// Lifts the window of an overlay image into a candidate unit.
pub fn lift_window(image: &[u8], entry: u32, span: u32, name: &str) -> String {
    let ins = decode::decode_window(image, entry, span);
    let body = unit::bodies(&ins);
    unit::compose(entry, name, &body)
}

/// Lifts an owner from the canonical ROM. `span` falls back to the register.
pub fn lift_owner(
    root: &Path,
    owner: &str,
    span: Option<u32>,
    name: Option<&str>,
) -> Result<(String, u32), String> {
    let (overlay, entry) = owners::parse_owner(owner)?;
    let span = match span {
        Some(span) => span,
        None => owners::span_for(root, &overlay, entry)?,
    };
    let image = owners::overlay_image(root, &overlay)?;
    let name = name
        .map(str::to_string)
        .unwrap_or_else(|| format!("Lifted_{entry:08x}"));
    Ok((lift_window(&image, entry, span, &name), span))
}
