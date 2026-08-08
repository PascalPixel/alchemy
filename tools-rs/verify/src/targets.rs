//! PORT NOTE: inlined from `tools/lib/decomp_targets.ts` -- `DEFAULT_TARGET`,
//! `TARGETS`, `parseDecompTarget`, `decompTarget`, and its `selfTest` (which
//! became the `#[cfg(test)]` assertions below). Delete this module when that
//! file gets a crate of its own.

use crate::routing::CompilerTarget;
use crate::Result;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct DecompTarget {
    pub id: &'static str,
    pub rom: &'static str,
    pub rom_size: usize,
    pub compiler: CompilerTarget,
    pub source_dir: &'static str,
    pub asm_dir: &'static str,
    pub asset_manifest: &'static str,
    pub output_dir: &'static str,
}

pub const DEFAULT_TARGET: &str = "gs1-en";

pub static TARGETS: &[DecompTarget] = &[
    DecompTarget {
        id: "gs1-en",
        rom: "roms/gs1-en.gba",
        rom_size: 0x0080_0000,
        compiler: CompilerTarget::Gs1,
        source_dir: "exact",
        asm_dir: "asm",
        asset_manifest: "assets/manifest.json",
        output_dir: "out",
    },
    DecompTarget {
        id: "gs2-en",
        rom: "roms/gs2-en.gba",
        rom_size: 0x0100_0000,
        compiler: CompilerTarget::Gs2,
        source_dir: "games/gs2/src",
        asm_dir: "games/gs2/asm",
        asset_manifest: "games/gs2/assets/manifest.json",
        output_dir: "out/gs2-en",
    },
];

/// `parseDecompTarget`. The error text matches the TypeScript, including the
/// JSON-quoted value.
pub fn parse_decomp_target(value: &str) -> Result<&'static str> {
    match TARGETS.iter().find(|target| target.id == value) {
        Some(target) => Ok(target.id),
        None => {
            let ids: Vec<&str> = TARGETS.iter().map(|target| target.id).collect();
            Err(format!(
                "unsupported decomp target {}; expected {}",
                json_quote(value),
                ids.join(" or ")
            ))
        }
    }
}

/// `JSON.stringify` of a string, for the values this tool can see (command line
/// arguments). Escapes the characters JSON requires.
fn json_quote(value: &str) -> String {
    let mut out = String::with_capacity(value.len() + 2);
    out.push('"');
    for character in value.chars() {
        match character {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            '\u{08}' => out.push_str("\\b"),
            '\u{0c}' => out.push_str("\\f"),
            control if (control as u32) < 0x20 => {
                out.push_str(&format!("\\u{:04x}", control as u32));
            }
            other => out.push(other),
        }
    }
    out.push('"');
    out
}

/// `decompTarget`.
pub fn decomp_target(id: &str) -> Result<DecompTarget> {
    let id = parse_decomp_target(id)?;
    Ok(*TARGETS
        .iter()
        .find(|target| target.id == id)
        .expect("parse_decomp_target only returns known ids"))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn relative(path: &str) -> bool {
        !path.is_empty()
            && !path.starts_with('/')
            && !path.starts_with('\\')
            && !path.split(['/', '\\']).any(|part| part == "..")
    }

    #[test]
    fn registry_matches_the_typescript() {
        let gs1 = decomp_target(DEFAULT_TARGET).unwrap();
        let gs2 = decomp_target("gs2-en").unwrap();
        assert_eq!(gs1.id, DEFAULT_TARGET);
        assert_eq!(gs1.rom, "roms/gs1-en.gba");
        assert_eq!(gs1.rom_size, 0x0080_0000);
        assert_eq!(gs1.compiler, CompilerTarget::Gs1);
        assert_eq!(gs2.rom, "roms/gs2-en.gba");
        assert_eq!(gs2.rom_size, 0x0100_0000);
        assert_eq!(gs2.compiler, CompilerTarget::Gs2);
        assert_eq!(TARGETS.len(), 2);
    }

    #[test]
    fn every_path_is_relative_and_target_isolated() {
        for target in TARGETS {
            for path in [
                target.rom,
                target.source_dir,
                target.asm_dir,
                target.asset_manifest,
                target.output_dir,
            ] {
                assert!(relative(path), "{} has a non-relative path {path}", target.id);
            }
        }
        let gs1 = TARGETS[0];
        let gs2 = TARGETS[1];
        assert_ne!(gs1.source_dir, gs2.source_dir);
        assert_ne!(gs1.asm_dir, gs2.asm_dir);
        assert_ne!(gs1.asset_manifest, gs2.asset_manifest);
        assert_ne!(gs1.output_dir, gs2.output_dir);
    }

    #[test]
    fn invalid_ids_are_rejected() {
        for invalid in ["", "gs1", "GS1-en", "gs1-en ", "gs2-ja"] {
            let error = parse_decomp_target(invalid).unwrap_err();
            assert_eq!(
                error,
                format!(
                    "unsupported decomp target {}; expected gs1-en or gs2-en",
                    json_quote(invalid)
                )
            );
        }
    }

    #[test]
    fn json_quoting_matches_json_stringify() {
        assert_eq!(json_quote("gs1"), "\"gs1\"");
        assert_eq!(json_quote(""), "\"\"");
        assert_eq!(json_quote("a\"b\\c"), "\"a\\\"b\\\\c\"");
        assert_eq!(json_quote("a\nb"), "\"a\\nb\"");
        assert_eq!(json_quote("a\u{1}b"), "\"a\\u0001b\"");
    }
}
