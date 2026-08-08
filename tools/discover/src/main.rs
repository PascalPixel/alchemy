// CLI wrapper matching `bun tools/lib/discover.ts`: read a ROM image, run the
// discovery fixpoint, write the canonical-JSON report to -o, and print the
// one-line summary that callers grep.

use discover::{json::canonical_json, Discovery, ROM_BASE};
use std::process::ExitCode;

fn usage() -> ! {
    println!("usage: discover [-h] -o OUTPUT [--details] rom");
    println!("\npositional arguments:\n  rom");
    println!("\noptions:\n  -h, --help            show this help message and exit\n  -o OUTPUT, --output OUTPUT\n  --details");
    std::process::exit(0);
}

fn run(argv: &[String]) -> Result<String, String> {
    if argv.iter().any(|arg| arg == "-h" || arg == "--help") {
        usage();
    }
    let mut output: Option<String> = None;
    let mut details = false;
    let mut rom: Option<String> = None;
    let mut index = 0usize;
    while index < argv.len() {
        let arg = &argv[index];
        if arg == "-o" || arg == "--output" {
            // `argv[++index]` past the end yields undefined, which then fails
            // the required-arguments check below rather than erroring here.
            index += 1;
            output = argv.get(index).cloned();
        } else if arg == "--details" {
            details = true;
        } else if !arg.starts_with('-') {
            // A later positional overwrites an earlier one, as in the source.
            rom = Some(arg.clone());
        } else {
            return Err(format!("unrecognized arguments: {arg}"));
        }
        index += 1;
    }
    // PORT NOTE: the guard is `!rom || !output`, and the empty string is falsy
    // in JavaScript, so `-o ""` reports the arguments as missing rather than
    // trying to write to "". Reproduced by treating empty as absent.
    let (rom, output) = match (rom, output) {
        (Some(rom), Some(output)) if !rom.is_empty() && !output.is_empty() => (rom, output),
        _ => return Err("the following arguments are required: rom, -o/--output".into()),
    };
    let data = std::fs::read(&rom).map_err(|error| format!("{rom}: {error}"))?;
    let mut discovery = Discovery::new(&data, ROM_BASE);
    let entry = discovery.run();
    let report = discovery.report(entry, details);
    std::fs::write(&output, format!("{}\n", canonical_json(&report)))
        .map_err(|error| format!("{output}: {error}"))?;
    Ok(format!(
        "functions={} instructions={} calls={} external_calls={} unresolved={} jump_tables={} conflicts={}",
        discovery.function_count(),
        discovery.instructions.len(),
        discovery.call_count(),
        discovery.external_call_count(),
        discovery.unresolved.len(),
        discovery.jump_tables.len(),
        discovery.conflicts.len(),
    ))
}

fn main() -> ExitCode {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    match run(&argv) {
        Ok(summary) => {
            println!("{summary}");
            ExitCode::SUCCESS
        }
        // PORT NOTE: the TypeScript throws, so Bun prints a stack trace and
        // exits 1. The message text and the exit code match; the surrounding
        // stack frames necessarily do not.
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
