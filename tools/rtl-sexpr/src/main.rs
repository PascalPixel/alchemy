// CLI for the RTL S-expression parser.
//
// `--self-test` mirrors the TypeScript original's `--self-test` flag exactly,
// including the `self-test=ok tool=rtl-sexpr` line it prints.
//
// PORT NOTE: the TS entrypoint has no mode other than `--self-test`. This
// binary additionally accepts file paths and prints one rendered top-level
// form per line, which is what the TS/Rust parity comparison consumes. It is
// additive; no existing behaviour changes.

use std::path::Path;
use std::process::ExitCode;

use rtl_sexpr::{first_atom_deep, head, parse_all, parse_tag, render, Tag};

fn repository_root() -> &'static Path {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
}

fn self_test() {
    let parsed = parse_all(concat!(
        ";; a trace comment, not content\n",
        "(insn 15 10 17 (set (reg/v:SI 3 r3)\n",
        "        (plus:SI (reg:SI 3 r3)\n",
        "            (const_int 3 [0x3]))) 5 {*thumb_addsi3} (nil))\n",
    ));
    assert!(
        parsed.len() == 1 && parsed[0].is_list(),
        "expected one top-level list"
    );
    assert!(
        head(&parsed[0]) == Some("insn"),
        "expected head \"insn\", got {:?}",
        head(&parsed[0])
    );
    let pattern = &parsed[0].items()[4];
    assert!(
        head(pattern) == Some("set"),
        "expected pattern head \"set\", got {:?}",
        head(pattern)
    );
    let dest_head = head(&pattern.items()[1]);
    assert!(
        dest_head == Some("reg/v:SI"),
        "expected dest head \"reg/v:SI\", got {dest_head:?}"
    );
    let Tag { tag, flags, mode } = parse_tag(dest_head.unwrap());
    assert!(
        tag == "reg" && flags.join(",") == "v" && mode.as_deref() == Some("SI"),
        "parse_tag mis-split: tag={tag} flags={flags:?} mode={mode:?}"
    );

    let symbol_wrapped = &parse_all("(symbol_ref:SI (\"Func_02001508\"))")[0];
    let symbol_argument = &symbol_wrapped.items()[1];
    assert!(
        first_atom_deep(symbol_argument) == Some("\"Func_02001508\""),
        "first_atom_deep did not descend through the extra wrapping paren"
    );

    let truncated = parse_all("(insn 1 0 2 (set (reg:SI 3");
    assert!(
        truncated.len() == 1,
        "truncated input should still yield the partial form"
    );

    println!("self-test=ok tool=rtl-sexpr");
}

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.iter().any(|argument| argument == "--self-test") {
        self_test();
        return ExitCode::SUCCESS;
    }
    let paths: Vec<&String> = arguments
        .iter()
        .filter(|argument| !argument.starts_with("--"))
        .collect();
    if paths.is_empty() {
        eprintln!(
            "usage: rtl-sexpr --self-test | rtl-sexpr <dump>... (paths relative to {})",
            repository_root().display()
        );
        return ExitCode::from(2);
    }
    for path in paths {
        let candidate = Path::new(path);
        let resolved = if candidate.is_absolute() {
            candidate.to_path_buf()
        } else {
            repository_root().join(candidate)
        };
        let text = match std::fs::read_to_string(&resolved) {
            Ok(text) => text,
            Err(error) => {
                eprintln!("rtl-sexpr: {}: {error}", resolved.display());
                return ExitCode::FAILURE;
            }
        };
        for form in parse_all(&text) {
            println!("{}", render(&form));
        }
    }
    ExitCode::SUCCESS
}
