//! Psynergy: portable Thumb decoding, C recovery, comparison and bounded repairs.
//! Callers supply bytes and image addresses; repository ownership and game-specific
//! source rewrites belong to the integrating project.
pub mod cache;
pub mod compare;
pub mod decode;
pub mod lift;
pub mod process;
pub mod repair;
pub mod sched;
pub mod thumb;
pub mod unit;
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
        unit::compose(base, "ReadPointer", &body, &tables)
    }

    #[test]
    fn independently_composed_modes_do_not_change_each_other() {
        let main = synthetic_unit(true);
        let overlay = synthetic_unit(false);
        assert!(!main.contains("bump_step"));
        assert!(!overlay.contains("bump_step"));
        assert!(!overlay.contains("03001ebc"));
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
