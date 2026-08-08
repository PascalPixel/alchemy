//! Differential harness driver: reads a case file on stdin and prints one
//! result line per case, for byte-comparison against the TypeScript original
//! driven over the identical case file.
//!
//! Case lines are tab-separated:
//!
//! ```text
//! S<TAB>name<TAB>base        externalSymbol(name, base)
//! A<TAB>name<TAB>base        externalSymbolAssembly(name, base)
//! O<TAB>overlay<TAB>source   overlayCallViaBase(overlay, source), "-" for undefined
//! ```
//!
//! Results are rendered so that every distinguishable outcome -- including the
//! difference between `null` and a throw -- is a distinct line.
//!
//! RETIREMENT: this binary only exists to compare against the TypeScript.
//! Delete it, and the `[[bin]]` entry, when `tools/lib/alchemy_gcc.ts` goes.

use std::io::{self, BufWriter, Read, Write};

use alchemy_symbols::{external_symbol, external_symbol_assembly, overlay_call_via_base};

/// The case file is line- and tab-separated, so a name containing a newline
/// arrives spelled as a backslash-u escape. Both drivers decode it, which keeps
/// the "JS `$` does not allow a trailing newline" case inside the harness
/// rather than only inside the unit tests.
fn unescape_field(text: &str) -> String {
    text.replace("\\u000a", "\n")
}

fn escape(text: &str) -> String {
    text.replace('\\', "\\\\").replace('\n', "\\n").replace('\t', "\\t")
}

fn main() {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input).expect("read stdin");
    let stdout = io::stdout();
    let mut out = BufWriter::new(stdout.lock());
    for line in input.lines() {
        if line.is_empty() {
            continue;
        }
        let mut fields = line.split('\t');
        let kind = fields.next().expect("kind");
        let first = unescape_field(fields.next().expect("first argument"));
        let first = first.as_str();
        let second = fields.next().expect("second argument");
        let rendered = match kind {
            "S" => {
                let base: u64 = second.parse().expect("base");
                match external_symbol(first, base) {
                    None => "null".to_string(),
                    Some(symbol) => format!("{},{}", symbol.address, symbol.thumb),
                }
            }
            "A" => {
                let base: u64 = second.parse().expect("base");
                match external_symbol_assembly(first, base) {
                    Ok(text) => escape(&text),
                    Err(message) => format!("throw:{}", escape(&message)),
                }
            }
            "O" => {
                let source = if second == "-" { None } else { Some(second) };
                overlay_call_via_base(first, source).to_string()
            }
            other => panic!("unknown case kind {other:?}"),
        };
        writeln!(out, "{rendered}").expect("write");
    }
}
