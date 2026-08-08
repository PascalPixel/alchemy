// CLI for the assembly-constraint inference: given function stems, read
// asm/<stem>.s, work out which permutation operators are worth trying on that
// function, and write the record to out/decomp/constraints/<stem>.json.
//
// Native compiler-aware source-constraint analyzer.

use std::fs;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

use decomp_constraints::{canonical_json, infer_assembly_constraints, regex::Regex};

// The TypeScript derives ROOT from import.meta.url, three directories up from
// tools/search/. CARGO_MANIFEST_DIR is tools/decomp-constraints at compile
// time, so two up lands on the same repository root.
fn repo_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate lives under tools")
        .parent()
        .expect("tools lives at the repository root")
        .to_path_buf()
}

const SAMPLE: &str = "Func_08000000:\n\tmov r3, r1\n\tldrsh r2, [r0, r3]\n\tcmp r2, #0\n\tblt .L0\n\tbl Func_08000100\n.L0:\n\tbx lr\n";

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();

    // PORT NOTE: cargo test is the real suite, but --self-test survives so the
    // repository's own gate can invoke the Rust binary exactly where it used
    // to invoke the deployed binary's self-test.
    if arguments.iter().any(|argument| argument == "--self-test") {
        let result = infer_assembly_constraints("08000000", SAMPLE);
        if result.inferred_arguments != 2
            || result.memory.signed != 1
            || result.conditions.signed != 1
            || !result
                .suggested_operators
                .iter()
                .any(|name| name == "fieldtype")
        {
            eprintln!("assembly constraint self-test failed");
            return ExitCode::FAILURE;
        }
        println!("decomp constraint self-test passed");
        return ExitCode::SUCCESS;
    }

    let stem_pattern = Regex::new(r"^[0-9a-f]{8}$", "");
    let stems: Vec<&String> = arguments
        .iter()
        .filter(|argument| stem_pattern.is_match(argument))
        .collect();

    // A run that inspected nothing is a failure, not a silent success: this is
    // the guard that stops the gate passing when every argument was junk.
    if stems.is_empty() {
        // PORT NOTE: the TypeScript throws, so bun prints an Error stack trace
        // and exits 1. Only the message text and the exit code are contractual;
        // the stack frames are not reproduced.
        eprintln!("usage: decomp-constraints STEM [STEM ...]");
        return ExitCode::FAILURE;
    }

    let root = repo_root();
    let output = root.join("out").join("decomp").join("constraints");
    if let Err(error) = fs::create_dir_all(&output) {
        eprintln!("cannot create {}: {error}", output.display());
        return ExitCode::FAILURE;
    }

    for stem in stems {
        let path = root.join("asm").join(format!("{stem}.s"));
        // PORT NOTE: readFileSync(path, "utf8") replaces invalid sequences with
        // U+FFFD rather than failing, which from_utf8_lossy matches.
        let bytes = match fs::read(&path) {
            Ok(bytes) => bytes,
            Err(error) => {
                eprintln!("cannot read {}: {error}", path.display());
                return ExitCode::FAILURE;
            }
        };
        let source = String::from_utf8_lossy(&bytes);
        let result = infer_assembly_constraints(stem, &source);
        let text = format!("{}\n", canonical_json(&result.to_json()));
        if let Err(error) = fs::write(output.join(format!("{stem}.json")), text) {
            eprintln!("cannot write {stem}.json: {error}");
            return ExitCode::FAILURE;
        }
        println!(
            "{stem}: insns={} args={} calls={} suggestions={}",
            result.instructions,
            result.inferred_arguments,
            result.calls,
            result.suggested_operators.join(",")
        );
    }
    ExitCode::SUCCESS
}
