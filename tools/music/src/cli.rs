use crate::{
    build_sound_table, parse_music_catalog, Result, SoundTableEntry, SoundTableSource, SymbolValue,
};
use std::io::Write;
use std::process::ExitCode;

const USAGE: &str = "usage: music {build-stdout SOURCE|catalog SOURCE}";

fn source(path: &str) -> Result<SoundTableSource> {
    let value: serde_json::Value =
        serde_json::from_slice(&std::fs::read(path).map_err(|e| format!("{path}: {e}"))?)
            .map_err(|e| e.to_string())?;
    let object = value
        .as_object()
        .ok_or_else(|| "sound table must be an object".to_string())?;
    let fields = object
        .get("fields")
        .and_then(serde_json::Value::as_array)
        .ok_or_else(|| "sound table fields are missing".to_string())?
        .iter()
        .map(|item| {
            item.as_str()
                .map(str::to_owned)
                .ok_or_else(|| "sound table field is not a string".to_string())
        })
        .collect::<Result<Vec<_>>>()?;
    let symbols = object
        .get("symbols")
        .and_then(serde_json::Value::as_object)
        .ok_or_else(|| "sound table symbols are missing".to_string())?
        .iter()
        .map(|(name, item)| {
            let value = item
                .as_str()
                .map(|v| SymbolValue::Text(v.to_owned()))
                .or_else(|| item.as_f64().map(SymbolValue::Number))
                .ok_or_else(|| "sound table symbol is not scalar".to_string())?;
            Ok((name.clone(), value))
        })
        .collect::<Result<Vec<_>>>()?;
    let entries = object
        .get("entries")
        .and_then(serde_json::Value::as_array)
        .ok_or_else(|| "sound table entries are missing".to_string())?
        .iter()
        .map(|item| {
            let pair = item
                .as_array()
                .ok_or_else(|| "sound table entry is not a pair".to_string())?;
            if pair.len() != 2 {
                return Err("sound table entry is not a pair".into());
            }
            Ok(SoundTableEntry {
                header: pair[0]
                    .as_str()
                    .ok_or_else(|| "sound table header is not a string".to_string())?
                    .to_owned(),
                player: pair[1]
                    .as_i64()
                    .ok_or_else(|| "sound table player is not an integer".to_string())?,
            })
        })
        .collect::<Result<Vec<_>>>()?;
    Ok(SoundTableSource {
        format: object
            .get("format")
            .and_then(serde_json::Value::as_i64)
            .ok_or_else(|| "sound table format is missing".to_string())?,
        fields,
        auxiliary: object
            .get("auxiliary")
            .and_then(serde_json::Value::as_str)
            .ok_or_else(|| "sound table auxiliary is missing".to_string())?
            .to_owned(),
        symbols,
        entries,
    })
}

fn run(args: &[String]) -> Result<()> {
    if args.first().map(String::as_str) == Some("catalog") {
        let path = args.get(1).ok_or_else(|| USAGE.to_string())?;
        if args.len() != 2 {
            return Err(USAGE.into());
        }
        let source = std::fs::read_to_string(path).map_err(|error| format!("{path}: {error}"))?;
        let catalog = parse_music_catalog(&source)?;
        println!("game\tsound_id\tname\ten\tde\tes\tfr\tit\tja\tsource");
        for track in &catalog.tracks {
            println!(
                "{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}",
                track.game,
                track.sound_id,
                track.name,
                track.titles[0],
                track.titles[1],
                track.titles[2],
                track.titles[3],
                track.titles[4],
                track.titles[5],
                track.source
            );
        }
        eprintln!(
            "tracks={} shared={}",
            catalog.tracks.len(),
            catalog.shared.len()
        );
        return Ok(());
    }
    if args.first().map(String::as_str) != Some("build-stdout") {
        return Err(USAGE.into());
    }
    let path = args.get(1).ok_or_else(|| USAGE.to_string())?;
    let (bytes, report) = build_sound_table(&source(path)?)?;
    eprintln!(
        "{{\"entries\":{},\"unique_headers\":{}}}",
        report.entries, report.unique_headers
    );
    std::io::stdout()
        .write_all(&bytes)
        .map_err(|e| e.to_string())
}

pub fn entry(arguments: &[String]) -> ExitCode {
    match run(arguments) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
