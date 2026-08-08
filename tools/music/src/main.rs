use std::io::Write;
use std::process::ExitCode;

use music::{
    build_sound_table, canonical_json, cli_integer, extract_sound_table, option, self_test, Result,
    SoundTableEntry, SoundTableSource, SymbolValue,
};

fn source(path: &str) -> Result<SoundTableSource> {
    let bytes = std::fs::read(path).map_err(|error| format!("{path}: {error}"))?;
    let value: serde_json::Value =
        serde_json::from_slice(&bytes).map_err(|error| format!("{path}: {error}"))?;
    let object = value
        .as_object()
        .ok_or_else(|| "unsupported sound-table source".to_string())?;
    let fields = object
        .get("fields")
        .and_then(serde_json::Value::as_array)
        .ok_or_else(|| "unsupported sound-table source".to_string())?
        .iter()
        .map(|item| {
            item.as_str()
                .map(str::to_string)
                .ok_or_else(|| "unsupported sound-table source".to_string())
        })
        .collect::<Result<Vec<_>>>()?;
    let symbols = object
        .get("symbols")
        .and_then(serde_json::Value::as_object)
        .ok_or_else(|| "unsupported sound-table source".to_string())?
        .iter()
        .map(|(name, item)| {
            let value = if let Some(text) = item.as_str() {
                SymbolValue::Text(text.to_string())
            } else if let Some(number) = item.as_f64() {
                SymbolValue::Number(number)
            } else {
                return Err("unsupported sound-table source".to_string());
            };
            Ok((name.clone(), value))
        })
        .collect::<Result<Vec<_>>>()?;
    let entries = object
        .get("entries")
        .and_then(serde_json::Value::as_array)
        .ok_or_else(|| "unsupported sound-table source".to_string())?
        .iter()
        .map(|item| {
            let pair = item
                .as_array()
                .ok_or_else(|| "unsupported sound-table source".to_string())?;
            if pair.len() != 2 {
                return Err("unsupported sound-table source".to_string());
            }
            Ok(SoundTableEntry {
                header: pair[0]
                    .as_str()
                    .ok_or_else(|| "unsupported sound-table source".to_string())?
                    .to_string(),
                player: pair[1]
                    .as_i64()
                    .ok_or_else(|| "unsupported sound-table source".to_string())?,
            })
        })
        .collect::<Result<Vec<_>>>()?;
    Ok(SoundTableSource {
        format: object
            .get("format")
            .and_then(serde_json::Value::as_i64)
            .ok_or_else(|| "unsupported sound-table source".to_string())?,
        fields,
        auxiliary: object
            .get("auxiliary")
            .and_then(serde_json::Value::as_str)
            .ok_or_else(|| "unsupported sound-table source".to_string())?
            .to_string(),
        symbols,
        entries,
    })
}

fn run(args: &[String]) -> Result<()> {
    if args.iter().any(|a| a == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        return Ok(());
    }
    if let [command, input] = args {
        if command == "build-stdout" {
            let (built, report) = build_sound_table(&source(input)?)?;
            let mut players = serde_json::Map::new();
            let mut counts = report.players.clone();
            counts.sort_by_key(|(key, _)| key.parse::<u64>().unwrap_or(u64::MAX));
            for (key, count) in counts {
                players.insert(key, serde_json::json!(count));
            }
            eprintln!(
                "{}",
                serde_json::json!({
                    "entries": report.entries,
                    "unique_headers": report.unique_headers,
                    "players": players,
                    "mirrored_auxiliary": report.mirrored_auxiliary,
                })
            );
            std::io::stdout()
                .write_all(&built)
                .map_err(|error| error.to_string())?;
            return Ok(());
        }
    }
    if args.first().map(String::as_str) != Some("extract-sound-table")
        || args.get(1).is_none_or(String::is_empty)
    {
        println!(
            "usage: music extract-sound-table ROM --address ADDRESS --count COUNT -o OUTPUT"
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
