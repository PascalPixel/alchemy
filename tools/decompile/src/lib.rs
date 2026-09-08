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
    let main = base == decode::MAIN_BASE;
    let ins = decode::decode_window_at(&image, base, entry, span);
    let (body, tables) = unit::bodies(&ins, main);
    let text = unit::compose(entry, &name, &body, &tables, main);
    Ok((text, span))
}
pub mod cli;

#[cfg(test)]
mod tests {
    use super::*;

    fn synthetic_unit(main: bool) -> String {
        let base = if main {
            decode::MAIN_BASE
        } else {
            decode::OVERLAY_BASE
        };
        // ldr r1, [pc, #4]; str r0, [r1]; bx lr; alignment; pointer word.
        let image = [
            0x01, 0x49, 0x08, 0x60, 0x70, 0x47, 0xc0, 0x46, 0x00, 0x10, 0x00, 0x08,
        ];
        let ins = decode::decode_window_at(&image, base, base, image.len() as u32);
        let (body, tables) = unit::bodies(&ins, main);
        unit::compose(base, "ReadPointer", &body, &tables, main)
    }

    #[test]
    fn independently_composed_modes_do_not_change_each_other() {
        let main = synthetic_unit(true);
        let overlay = synthetic_unit(false);
        assert!(!main.contains("bump_step"));
        assert!(overlay.contains("bump_step"));
        assert!(main.contains("Data_08001000"), "{main}");
        assert!(!overlay.contains("Data_08001000"), "{overlay}");
        assert!(overlay.contains("0x08001000"), "{overlay}");
        for mode in [false, true, false, true] {
            assert_eq!(
                synthetic_unit(mode),
                if mode { &main } else { &overlay }.as_str()
            );
        }
        assert_eq!(
            std::thread::spawn(|| synthetic_unit(true)).join().unwrap(),
            main
        );
        assert_eq!(
            std::thread::spawn(|| synthetic_unit(false)).join().unwrap(),
            overlay
        );
    }
}
