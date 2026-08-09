// CLI wrapper matching `bun tools/lib/discover.ts`: read a ROM image, run the
// discovery fixpoint, write the canonical-JSON report to -o, and print the
// one-line summary that callers grep.

use discover::{json::canonical_json, Discovery, ROM_BASE};
use std::process::ExitCode;

fn usage() -> ! {
    println!("usage: discover [-h] [--self-test] -o OUTPUT [--details] rom");
    println!("\npositional arguments:\n  rom");
    println!("\noptions:\n  -h, --help            show this help message and exit\n  --self-test           exercise discovery against a synthetic image\n  -o OUTPUT, --output OUTPUT\n  --details");
    std::process::exit(0);
}

fn self_test() {
    let mut image = vec![0; 0x400];
    // Reset: b 0x08000200. The fixture has one real ARM return function and
    // is large enough to exercise the library's normal image bounds checks.
    image[..4].copy_from_slice(&0xea00_007e_u32.to_le_bytes());
    image[0x200..0x204].copy_from_slice(&0xe1a0_f00e_u32.to_le_bytes());

    let mut discovery = Discovery::new(&image, ROM_BASE);
    let entry = discovery.run();
    assert_eq!(entry, ROM_BASE + 0x200);
    assert_eq!(discovery.function_count(), 1);
    assert_eq!(discovery.instructions.len(), 1);

    let report = discovery.report(entry, true);
    let json = canonical_json(&report);
    assert!(json.contains("\"rom_size\": 1024"));
    assert!(json.contains("\"reset_entry\": 134218240"));
    println!("discover self-test ok");
}

fn run(argv: &[String]) -> Result<String, String> {
    let mut output: Option<String> = None;
    let mut details = false;
    let mut help = false;
    let mut rom: Option<String> = None;
    let mut index = 0usize;
    while index < argv.len() {
        let arg = &argv[index];
        if arg == "-h" || arg == "--help" {
            help = true;
        } else if arg == "-o" || arg == "--output" {
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
    if help {
        usage();
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
    if argv == ["--self-test"] {
        self_test();
        return ExitCode::SUCCESS;
    }
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
