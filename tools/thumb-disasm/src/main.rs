//! CLI entrypoint for the Thumb disassembly-line parser.
//!
//! PORT NOTE: the TypeScript's only entrypoint is `--self-test`, which is kept
//! here for parity (the real assertions now live in `cargo test`). The extra
//! `--canonical` mode is what the differential harness drives: it reads one
//! objdump line per stdin line and writes one canonical rendering per line, so
//! the Rust and the TypeScript can be diffed byte-for-byte.
//!
//! Input lines are `\t`- and `\\`-escaped so a single physical line can carry a
//! tab (objdump separates mnemonic from operands with one).

use std::io::{self, BufWriter, Read, Write};

use thumb_disasm::{classify, parse_instruction};

fn unescape(line: &str) -> String {
    let mut out = String::with_capacity(line.len());
    let mut chars = line.chars();
    while let Some(c) = chars.next() {
        if c != '\\' {
            out.push(c);
            continue;
        }
        match chars.next() {
            Some('t') => out.push('\t'),
            Some('n') => out.push('\n'),
            Some('r') => out.push('\r'),
            Some('\\') => out.push('\\'),
            Some(other) => {
                out.push('\\');
                out.push(other);
            }
            None => out.push('\\'),
        }
    }
    out
}

fn self_test() {
    let add = parse_instruction(0x6, "adds\tr3, r3, r2");
    assert_eq!(add.mnemonic, "adds");
    assert_eq!(add.operands.len(), 3);
    println!("self-test=ok tool=thumb-disasm");
}

fn canonical_stream() -> io::Result<()> {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input)?;
    let stdout = io::stdout();
    let mut out = BufWriter::new(stdout.lock());
    for line in input.lines() {
        let raw = unescape(line);
        let insn = parse_instruction(0, &raw);
        writeln!(out, "{}\u{2}{}", insn.canonical(), classify(&insn.mnemonic))?;
    }
    out.flush()
}

fn main() -> io::Result<()> {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.iter().any(|a| a == "--self-test") {
        self_test();
        return Ok(());
    }
    if args.iter().any(|a| a == "--canonical") {
        return canonical_stream();
    }
    // Default: parse each argument as an objdump line and print its canonical
    // form, so the tool is usable without a pipe.
    if args.is_empty() {
        eprintln!("usage: thumb-disasm [--self-test | --canonical | <objdump-line>...]");
        std::process::exit(2);
    }
    let stdout = io::stdout();
    let mut out = BufWriter::new(stdout.lock());
    for (i, arg) in args.iter().enumerate() {
        let insn = parse_instruction(i as i64, &unescape(arg));
        writeln!(out, "{}\u{2}{}", insn.canonical(), classify(&insn.mnemonic))?;
    }
    out.flush()
}
