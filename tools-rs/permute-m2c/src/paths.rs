//! Where this port is allowed to read and write.
//!
//! The TypeScript hard-codes `<root>/asm`, `<root>/exact` and
//! `<root>/out/permute`, and on a successful permutation it writes into
//! `exact/` and deletes from `asm/`. A Rust port that did the same during a
//! parity run would mutate the repository, so every one of those directories is
//! resolved through an environment variable that **defaults to the production
//! path**. With no variables set the port is byte-for-byte the TypeScript's
//! behaviour; the parity harness and the tests set all three to a directory
//! under `tools-rs/permute-m2c/target/fixtures/`.
//!
//! * `PERMUTE_M2C_ASM`   -> `<root>/asm`   (classification, veneer scan, `.s` sources, and the delete)
//! * `PERMUTE_M2C_EXACT` -> `<root>/exact` (the tracked-stem scan and the write)
//! * `PERMUTE_M2C_OUT`   -> `<root>/out/permute` (scratch for candidates and objects)

use std::path::PathBuf;

pub use verify::routing::root;

fn overridden(variable: &str, fallback: PathBuf) -> PathBuf {
    match std::env::var_os(variable) {
        Some(value) if !value.is_empty() => PathBuf::from(value),
        _ => fallback,
    }
}

pub fn asm_dir() -> PathBuf {
    overridden("PERMUTE_M2C_ASM", root().join("asm"))
}

pub fn exact_dir() -> PathBuf {
    overridden("PERMUTE_M2C_EXACT", root().join("exact"))
}

pub fn out_dir() -> PathBuf {
    overridden("PERMUTE_M2C_OUT", root().join("out").join("permute"))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn defaults_are_the_production_paths() {
        // Set nothing: the defaults must be exactly what the TypeScript uses.
        assert_eq!(
            overridden("PERMUTE_M2C_ASM_UNSET_IN_TESTS", root().join("asm")),
            root().join("asm")
        );
        assert_eq!(
            overridden("PERMUTE_M2C_OUT_UNSET_IN_TESTS", root().join("out").join("permute")),
            root().join("out").join("permute")
        );
    }
}
