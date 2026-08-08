use std::process::ExitCode;

use music::{
    build_sound_table, canonical_json, cli_integer, extract_sound_table, option, self_test, Result,
};

fn run(args: &[String]) -> Result<()> {
    if args.iter().any(|a| a == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        return Ok(());
    }
    if args.first().map(String::as_str) != Some("extract-sound-table")
        || args.get(1).is_none_or(String::is_empty)
    {
        println!(
            "usage: music.ts extract-sound-table ROM --address ADDRESS --count COUNT -o OUTPUT"
        );
        println!("       music.ts --self-test");
        return Ok(());
    }
    let rom = std::fs::read(&args[1]).map_err(|e| format!("{}: {e}", args[1]))?;
    let source = extract_sound_table(
        &rom,
        cli_integer(&option(args, "--address")?, "--address")?,
        cli_integer(&option(args, "--count")?, "--count")?,
    )?;
    let output = option(args, "-o")?;
    std::fs::write(&output, canonical_json(&source.to_json()) + "\n")
        .map_err(|e| format!("{output}: {e}"))?;
    let (built, report) = build_sound_table(&source)?;
    println!(
        "entries={} unique_headers={} bytes={}",
        report.entries,
        report.unique_headers,
        built.len()
    );
    Ok(())
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match run(&args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
