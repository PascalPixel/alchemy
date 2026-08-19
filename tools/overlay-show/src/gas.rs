//! Assemble one Thumb `.s` file to the objdump listing `reconstruct` consumes.
//!
//! WHY THIS EXISTS. `overlay reconstruct` already lifts a listing. Main-image
//! owners live as GAS in `asm/<addr>.s`, not as an overlay row, so the missing
//! step is turning that file into the same listing overlay-show already feeds
//! the lifter. This is that step. It is not the deleted `reverse-gcc296` crate:
//! that crate searched existing C, duplicated the permuter, and was folded
//! into `shape-sweep`. This takes assembly and produces a draft.
//!
//! HOW. Collect every `Func_<addr>` the file calls, give the linker an
//! absolute `.set` for each one, assemble at zero, link at the owner's
//! address, objcopy to a raw image, and objdump it the same way overlay-show
//! does. The resulting `bl 0x08004938` and `@ (0x...)` pool comments are
//! exactly what `draft` already reads.

use std::collections::BTreeSet;
use std::path::{Path, PathBuf};
use std::process::Command;

/// One assembled function, ready to lift.
pub struct Assembled {
    pub listing: Vec<String>,
    pub func: String,
    pub addr: i64,
}

/// First `Func_<hex>:` label, which is the owner this file is named for.
pub fn entry_of(source: &str) -> Result<(String, i64), String> {
    for line in source.lines() {
        let trimmed = line.trim();
        let Some(name) = trimmed.strip_suffix(':') else { continue };
        let Some(digits) = name.strip_prefix("Func_") else { continue };
        if digits.is_empty() || !digits.bytes().all(|b| b.is_ascii_hexdigit()) {
            continue;
        }
        let addr = i64::from_str_radix(digits, 16)
            .map_err(|_| format!("{name}: address is not hex"))?;
        return Ok((name.to_string(), addr));
    }
    Err("no Func_<addr>: label -- a reverse draft needs an owner entry".to_string())
}

/// `Func_<hex>` labels defined in this file.
pub fn defined_funcs(source: &str) -> BTreeSet<String> {
    let mut names = BTreeSet::new();
    for line in source.lines() {
        let trimmed = line.trim();
        let Some(name) = trimmed.strip_suffix(':') else { continue };
        if name.starts_with("Func_") {
            names.insert(name.to_string());
        }
    }
    names
}

/// Every `Func_<hex>` token, labels and operands both.
pub fn referenced_funcs(source: &str) -> BTreeSet<String> {
    let mut names = BTreeSet::new();
    let bytes = source.as_bytes();
    let mut i = 0usize;
    while i + 5 < bytes.len() {
        if &bytes[i..i + 5] == b"Func_" {
            let start = i;
            i += 5;
            let digits = bytes[i..].iter().take_while(|b| b.is_ascii_hexdigit()).count();
            if digits > 0 {
                i += digits;
                if let Ok(name) = std::str::from_utf8(&bytes[start..i]) {
                    names.insert(name.to_string());
                }
                continue;
            }
        }
        i += 1;
    }
    names
}

/// Callees the file names but does not define -- the linker needs a `.set`.
pub fn externals_of(source: &str) -> Vec<(String, i64)> {
    let defined = defined_funcs(source);
    let mut out = Vec::new();
    for name in referenced_funcs(source) {
        if defined.contains(&name) {
            continue;
        }
        let Some(digits) = name.strip_prefix("Func_") else { continue };
        if let Ok(addr) = i64::from_str_radix(digits, 16) {
            out.push((name, addr));
        }
    }
    out
}

/// Preamble so `bl Func_08004938` encodes against the real address.
pub fn preamble(source: &str) -> String {
    let mut lines = vec![
        ".syntax unified".to_string(),
        ".thumb".to_string(),
        ".text".to_string(),
        ".align 2".to_string(),
    ];
    for (name, addr) in externals_of(source) {
        lines.push(format!(".set {name}, {addr:#x}"));
    }
    lines.push(String::new());
    lines.join("\n")
}

fn find_tool(name: &str) -> Result<PathBuf, String> {
    let path = std::env::var_os("PATH").ok_or_else(|| format!("no PATH; cannot find {name}"))?;
    for dir in std::env::split_paths(&path) {
        let candidate = dir.join(name);
        if candidate.is_file() {
            return Ok(candidate);
        }
    }
    Err(format!("{name}: not on PATH"))
}

fn run_tool(bin: &Path, args: &[&str]) -> Result<Vec<u8>, String> {
    let output = Command::new(bin)
        .args(args)
        .output()
        .map_err(|e| format!("{}: {e}", bin.display()))?;
    if !output.status.success() {
        let stderr = String::from_utf8_lossy(&output.stderr);
        let stdout = String::from_utf8_lossy(&output.stdout);
        return Err(format!(
            "{} {} failed:\n{stderr}{stdout}",
            bin.display(),
            args.join(" ")
        ));
    }
    Ok(output.stdout)
}

fn is_listing_line(line: &str) -> bool {
    let trimmed = line.trim_start();
    if trimmed.len() == line.len() {
        return false;
    }
    match trimmed.find(':') {
        Some(colon) => {
            let digits = &trimmed[..colon];
            !digits.is_empty() && digits.bytes().all(|b| b.is_ascii_hexdigit())
        }
        None => false,
    }
}

/// Assemble `source` into an objdump listing, writing intermediates under `work`.
pub fn assemble_listing(source: &str, work: &Path) -> Result<Assembled, String> {
    let (func, addr) = entry_of(source)?;
    let assembler = find_tool("arm-none-eabi-as")?;
    let linker = find_tool("arm-none-eabi-ld")?;
    let objcopy = find_tool("arm-none-eabi-objcopy")?;
    let objdump = find_tool("arm-none-eabi-objdump")?;

    std::fs::create_dir_all(work).map_err(|e| format!("{}: {e}", work.display()))?;
    let combined = format!("{}{}\n", preamble(source), source);
    let gas = work.join("input.s");
    let object = work.join("input.o");
    let elf = work.join("input.elf");
    let bin = work.join("input.bin");
    std::fs::write(&gas, combined).map_err(|e| format!("{}: {e}", gas.display()))?;

    run_tool(
        &assembler,
        &[
            "-mthumb",
            "-mcpu=arm7tdmi",
            "-o",
            object.to_str().ok_or("object path is not utf-8")?,
            gas.to_str().ok_or("gas path is not utf-8")?,
        ],
    )?;
    let text = format!("{addr:#x}");
    run_tool(
        &linker,
        &[
            &format!("-Ttext={text}"),
            "--no-warn-mismatch",
            "-o",
            elf.to_str().ok_or("elf path is not utf-8")?,
            object.to_str().ok_or("object path is not utf-8")?,
        ],
    )?;
    run_tool(
        &objcopy,
        &[
            "-O",
            "binary",
            elf.to_str().ok_or("elf path is not utf-8")?,
            bin.to_str().ok_or("bin path is not utf-8")?,
        ],
    )?;
    let dump = run_tool(
        &objdump,
        &[
            "-D",
            "-b",
            "binary",
            "-m",
            "arm",
            "-M",
            "force-thumb",
            &format!("--adjust-vma={addr:#x}"),
            bin.to_str().ok_or("bin path is not utf-8")?,
        ],
    )?;
    let text = String::from_utf8_lossy(&dump);
    let listing = text
        .split('\n')
        .filter(|line| is_listing_line(line))
        .map(|line| line.trim_end().to_string())
        .filter(|line| !line.is_empty())
        .collect::<Vec<_>>();
    if listing.is_empty() {
        return Err("objdump produced no Thumb listing".to_string());
    }
    Ok(Assembled { listing, func, addr })
}

/// Read a `.s` file and assemble it.
pub fn assemble_path(path: &Path, work: &Path) -> Result<Assembled, String> {
    let source = std::fs::read_to_string(path).map_err(|e| format!("{}: {e}", path.display()))?;
    assemble_listing(&source, work)
}

#[cfg(test)]
mod tests {
    use super::*;

    const SAMPLE: &str = "\
.syntax unified
	.thumb
	.global Func_08000000
	.thumb_func
Func_08000000:
	movs	r0, #4
	movs	r1, #1
	bl	Func_08000100
	bx	lr
";

    #[test]
    fn entry_is_the_first_func_label() {
        let (name, addr) = entry_of(SAMPLE).unwrap();
        assert_eq!(name, "Func_08000000");
        assert_eq!(addr, 0x0800_0000);
    }

    #[test]
    fn a_file_with_no_func_label_is_rejected() {
        assert!(entry_of("	movs	r0, #0\n").is_err());
    }

    #[test]
    fn callees_are_externals_and_the_entry_is_not() {
        let ext = externals_of(SAMPLE);
        assert_eq!(ext, vec![("Func_08000100".to_string(), 0x0800_0100)]);
        assert!(defined_funcs(SAMPLE).contains("Func_08000000"));
        assert!(!defined_funcs(SAMPLE).contains("Func_08000100"));
    }

    #[test]
    fn preamble_sets_each_external_at_its_address() {
        let text = preamble(SAMPLE);
        assert!(text.contains(".set Func_08000100, 0x8000100"), "{text}");
        assert!(!text.contains(".set Func_08000000"), "{text}");
    }

    #[test]
    fn assemble_then_lift_keeps_the_call_and_its_args() {
        let Ok(as_bin) = find_tool("arm-none-eabi-as") else {
            return;
        };
        let _ = as_bin;
        let work = std::env::temp_dir().join(format!("alchemy-gas-self-test-{}", std::process::id()));
        let assembled = assemble_listing(SAMPLE, &work).expect("assemble sample");
        assert_eq!(assembled.func, "Func_08000000");
        assert_eq!(assembled.addr, 0x0800_0000);
        let draft = crate::reconstruct::draft(&assembled.listing, &assembled.func);
        assert_eq!(draft.calls, 1);
        assert!(
            draft.lines.iter().any(|l| l.contains("Func_08000100(4, 1);")),
            "{:?}",
            draft.lines
        );
    }
}
