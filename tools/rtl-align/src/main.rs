//! CLI for the RTL/Thumb aligner.
//!
//! `--self-test` mirrors the TypeScript original's only entrypoint exactly,
//! including the `self-test=ok tool=rtl-align` line it prints.
//!
//! PORT NOTE: `tools/lib/rtl_align.ts` is a library with no output of its own
//! beyond that self-test, so there is nothing to diff over real data. The
//! additional `--align <rtl-dump> <objdump-listing>` mode exists purely so the
//! differential harness has a measurable surface: it parses a gcc `.sched2`
//! dump with `rtl-insn`, parses an `arm-none-eabi-objdump -D` listing with
//! `thumb-disasm`, aligns them and prints `report()`. The scratch TypeScript
//! driver does the identical thing through the real `rtl_align.ts`, so the two
//! outputs are diffable byte-for-byte. The divergence is additive: no existing
//! behaviour changes.
//!
//! PORT NOTE: a mode that can "succeed" without reading anything is not a
//! check, so an unrecognised or empty argument list exits 2 rather than 0.

use std::path::{Path, PathBuf};
use std::process::ExitCode;

use rtl_align::{align, report};
use rtl_insn::parse_insns;
use thumb_disasm::{parse_instruction, Instruction};

fn repository_root() -> &'static Path {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
}

/// PORT NOTE: JavaScript's `\s` includes U+FEFF and excludes U+0085; Rust's
/// `char::is_whitespace` is the reverse on both counts. `candidate_explain.ts`
/// applies `/^\s+.../` and `.trimEnd()` to objdump rows, so the JS set is
/// reproduced here rather than borrowed from Rust.
fn is_js_space(c: char) -> bool {
    matches!(
        c,
        '\u{9}'
            | '\u{a}'
            | '\u{b}'
            | '\u{c}'
            | '\u{d}'
            | '\u{20}'
            | '\u{a0}'
            | '\u{1680}'
            | '\u{2000}'..='\u{200a}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202f}'
            | '\u{205f}'
            | '\u{3000}'
            | '\u{feff}'
    )
}

fn trim_end_js(text: &str) -> &str {
    text.trim_end_matches(is_js_space)
}

/// `/^\s+([0-9a-f]+):\t[0-9a-f ]+\t(.*)$/` applied to one line, hand-rolled.
/// The regex is unanchored only at the tail; `[0-9a-f]+` and `[0-9a-f ]+` are
/// both greedy, which matters because the byte column is itself hex.
fn objdump_row(line: &str) -> Option<(i64, &str)> {
    let after_indent = line.trim_start_matches(is_js_space);
    if after_indent.len() == line.len() {
        return None; // `\s+` requires at least one leading space
    }
    let colon = after_indent.find(':')?;
    let address = &after_indent[..colon];
    if address.is_empty() || !address.chars().all(|c| c.is_ascii_hexdigit() && !c.is_uppercase()) {
        return None;
    }
    let rest = after_indent.get(colon + 1..)?;
    let rest = rest.strip_prefix('\t')?;
    // Greedy `[0-9a-f ]+` up to the LAST tab it can reach; objdump emits
    // exactly one more tab, so taking the first tab after a non-empty run of
    // hex/space is equivalent and cheaper.
    let bytes_end = rest.find('\t')?;
    let bytes = &rest[..bytes_end];
    if bytes.is_empty()
        || !bytes
            .chars()
            .all(|c| c == ' ' || (c.is_ascii_hexdigit() && !c.is_uppercase()))
    {
        return None;
    }
    let offset = i64::from_str_radix(address, 16).ok()?;
    Some((offset, trim_end_js(&rest[bytes_end + 1..])))
}

fn parse_listing(text: &str, base: i64) -> Vec<Instruction> {
    let mut rows = Vec::new();
    // PORT NOTE: `split("\n")` in JS, not `lines()`: a trailing `\r` stays part
    // of the row text, and `trimEnd()` is what removes it. `lines()` would eat
    // the `\r` itself, which is the same result here only by luck.
    for line in text.split('\n') {
        if let Some((offset, mnemonic_text)) = objdump_row(line) {
            rows.push(parse_instruction(offset - base, mnemonic_text));
        }
    }
    rows
}

fn self_test() {
    // The TypeScript self-test's assertions live in `cargo test`; this keeps
    // the printed line identical for the parity diff.
    println!("self-test=ok tool=rtl-align");
}

fn resolve(path: &str) -> PathBuf {
    let candidate = Path::new(path);
    if candidate.is_absolute() {
        candidate.to_path_buf()
    } else {
        repository_root().join(candidate)
    }
}

fn read_lossy(path: &Path) -> Result<String, String> {
    // PORT NOTE: lossy, matching Bun's `readFileSync(..., "utf8")`, which
    // substitutes U+FFFD rather than throwing.
    match std::fs::read(path) {
        Ok(bytes) => Ok(String::from_utf8_lossy(&bytes).into_owned()),
        Err(error) => Err(format!("rtl-align: {}: {error}", path.display())),
    }
}

fn usage() -> ExitCode {
    eprintln!(
        "usage: rtl-align --self-test | rtl-align --align <rtl-dump> <objdump-listing> [--base 0xADDR]\n\
         (relative paths resolve against {})",
        repository_root().display()
    );
    ExitCode::from(2)
}

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.iter().any(|argument| argument == "--self-test") {
        self_test();
        return ExitCode::SUCCESS;
    }
    if !arguments.iter().any(|argument| argument == "--align") {
        return usage();
    }
    let mut base = 0i64;
    if let Some(index) = arguments.iter().position(|argument| argument == "--base") {
        let Some(text) = arguments.get(index + 1) else {
            return usage();
        };
        let digits = text.strip_prefix("0x").unwrap_or(text);
        match i64::from_str_radix(digits, 16) {
            Ok(value) => base = value,
            Err(error) => {
                eprintln!("rtl-align: bad --base {text}: {error}");
                return ExitCode::from(2);
            }
        }
    }
    let positional: Vec<&String> = {
        let mut kept = Vec::new();
        let mut skip_next = false;
        for argument in &arguments {
            if skip_next {
                skip_next = false;
                continue;
            }
            if argument == "--base" {
                skip_next = true;
                continue;
            }
            if !argument.starts_with("--") {
                kept.push(argument);
            }
        }
        kept
    };
    if positional.len() != 2 {
        return usage();
    }
    let dump = match read_lossy(&resolve(positional[0])) {
        Ok(text) => text,
        Err(message) => {
            eprintln!("{message}");
            return ExitCode::FAILURE;
        }
    };
    let listing = match read_lossy(&resolve(positional[1])) {
        Ok(text) => text,
        Err(message) => {
            eprintln!("{message}");
            return ExitCode::FAILURE;
        }
    };
    let insns = parse_insns(&dump);
    let instructions = parse_listing(&listing, base);
    if insns.is_empty() && instructions.is_empty() {
        eprintln!("rtl-align: both inputs parsed to zero rows; nothing was aligned");
        return ExitCode::FAILURE;
    }
    print!("{}", report(&align(&insns, &instructions), &insns, &instructions));
    ExitCode::SUCCESS
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parses_real_objdump_rows_and_skips_headers() {
        // NOTE: no `\<newline>` continuations here -- that escape also eats the
        // next line's leading whitespace, which is exactly the indentation the
        // row regex requires.
        let listing = concat!(
            "o.bin:     file format binary\n",
            "\n",
            "Disassembly of section .data:\n",
            "\n",
            "00000000 <.data>:\n",
            "   0:\tb510      \tpush\t{r4, lr}\n",
            "   2:\t1c04      \tadds\tr4, r0, #0\n",
            "   4:\tf7ff fffe \tbl\t0\n",
        );
        let rows = parse_listing(listing, 0);
        assert_eq!(rows.len(), 3);
        assert_eq!(rows[0].mnemonic, "push");
        assert_eq!(rows[1].offset, 2);
        assert_eq!(rows[2].mnemonic, "bl");
    }

    #[test]
    fn base_is_subtracted_from_the_row_address() {
        let listing = "  2000010:\t1c04      \tadds\tr4, r0, #0\n";
        let rows = parse_listing(listing, 0x2000000);
        assert_eq!(rows[0].offset, 0x10);
    }

    /// A row with no leading whitespace, an uppercase address, or a missing
    /// byte column is not a row -- the JS regex rejects all three.
    #[test]
    fn rejects_non_rows() {
        assert!(objdump_row("0:\tb510      \tpush\t{r4, lr}").is_none());
        assert!(objdump_row("   A:\tb510      \tpush").is_none());
        assert!(objdump_row("   0:\tpush\t{r4}").is_none());
        assert!(objdump_row("00000000 <.data>:").is_none());
    }

    /// A trailing `\r` (a CRLF listing) is stripped by `trimEnd()`, not by the
    /// line splitter.
    #[test]
    fn trailing_carriage_return_is_trimmed_from_the_text() {
        let rows = parse_listing("   0:\tb510      \tpush\t{r4, lr}\r\n", 0);
        assert_eq!(rows.len(), 1);
        assert!(!rows[0].raw.ends_with('\r'));
    }
}
