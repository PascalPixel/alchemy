//! Golden Sun owner extraction and inspection. Portable C recovery belongs to
//! `psynergy decompile`.
pub mod cli;
mod imports;
mod owners;

#[cfg(test)]
mod tests {
    use psynergy::compare::topology::{compare, Comparison};
    #[test]
    fn live_arm_owner_fails_closed() {
        let source = include_str!(
            "../../../../games/THE BROKEN SEAL/SRC/SYSTEM/RESOURCE/PATCH_THUMB_BRANCH.S"
        );
        assert!(matches!(
            compare(source, source, "Func_08002d5c"),
            Comparison::Uncovered(reason) if reason == "candidate-arm-mode"
        ));
    }
}
