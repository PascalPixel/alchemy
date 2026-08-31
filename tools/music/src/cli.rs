use crate::{
    add_midi_build_directive, add_smsh_midi_tempos, build_sound_table, normalized_midi_notes,
    parse_music_catalog, Result, SoundTableEntry, SoundTableSource, SymbolValue,
};
use std::io::Write;
use std::process::ExitCode;

const USAGE: &str =
    "usage: music {build-stdout SOURCE|catalog SOURCE|compare-midi-notes LEFT RIGHT|match-midi-notes LEFT_DIR RIGHT_DIR|extract-sequences JSON TSV|extract-sound-table JSON SEQUENCES RESIDUAL_INDEX TSV|extract-smsh-source ROM HEADER NAME|add-midi-directive MIDI SOURCE|add-smsh-tempo MIDI...}";

fn source(path: &str) -> Result<SoundTableSource> {
    if path.ends_with(".tsv") {
        let text = std::fs::read_to_string(path).map_err(|error| format!("{path}: {error}"))?;
        let mut rows = text.lines().filter(|line| !line.starts_with('#'));
        if rows.next() != Some("slot\theader\tplayer\tsource\tstatus") {
            return Err("sound table header differs".into());
        }
        let mut symbols = Vec::new();
        let mut by_address = std::collections::BTreeMap::new();
        let mut entries = Vec::new();
        for (slot, row) in rows.enumerate() {
            let fields = row.split('\t').collect::<Vec<_>>();
            if fields.len() != 5 || fields[0] != slot.to_string() {
                return Err(format!("sound table row {slot} identity differs"));
            }
            let address = u32::from_str_radix(fields[1].trim_start_matches("0x"), 16)
                .map_err(|_| format!("sound table row {slot} address differs"))?;
            let symbol = by_address
                .entry(address)
                .or_insert_with(|| {
                    let symbol = format!("header_{address:08x}");
                    symbols.push((
                        symbol.clone(),
                        SymbolValue::Text(format!("0x{address:08x}")),
                    ));
                    symbol
                })
                .clone();
            entries.push(SoundTableEntry {
                header: symbol,
                player: fields[2]
                    .parse::<i64>()
                    .map_err(|_| format!("sound table row {slot} player differs"))?,
            });
        }
        return Ok(SoundTableSource {
            format: 1,
            fields: vec!["header".into(), "player".into()],
            auxiliary: "copy-player".into(),
            symbols,
            entries,
        });
    }
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
    if args.first().map(String::as_str) == Some("match-midi-notes") {
        if args.len() != 3 {
            return Err(USAGE.into());
        }
        let read = |directory: &str| -> Result<Vec<(String, Vec<crate::MidiNoteEvent>)>> {
            let mut paths = std::fs::read_dir(directory)
                .map_err(|error| format!("{directory}: {error}"))?
                .filter_map(std::result::Result::ok)
                .map(|entry| entry.path())
                .filter(|path| path.extension().and_then(|value| value.to_str()) == Some("mid"))
                .collect::<Vec<_>>();
            paths.sort();
            paths
                .into_iter()
                .map(|path| {
                    let name = path
                        .file_name()
                        .and_then(|value| value.to_str())
                        .ok_or_else(|| format!("{}: filename differs", path.display()))?
                        .to_string();
                    let bytes = std::fs::read(&path)
                        .map_err(|error| format!("{}: {error}", path.display()))?;
                    Ok((name, normalized_midi_notes(&bytes)?))
                })
                .collect()
        };
        let left = read(&args[1])?;
        let right = read(&args[2])?;
        let shape = |notes: &[crate::MidiNoteEvent]| {
            let mut events = notes
                .iter()
                .map(|note| (note.tick, note.note, note.on))
                .collect::<Vec<_>>();
            events.sort();
            events
        };
        let mut matches = 0;
        for (left_name, left_notes) in &left {
            for (right_name, right_notes) in &right {
                if left_notes == right_notes {
                    println!("{left_name}\t{right_name}\t{}\texact", left_notes.len());
                    matches += 1;
                } else if shape(left_notes) == shape(right_notes) {
                    println!(
                        "{left_name}\t{right_name}\t{}\tnote-shape",
                        left_notes.len()
                    );
                    matches += 1;
                }
            }
        }
        eprintln!(
            "matches={matches} left={} right={}",
            left.len(),
            right.len()
        );
        return Ok(());
    }
    if args.first().map(String::as_str) == Some("compare-midi-notes") {
        if args.len() != 3 {
            return Err(USAGE.into());
        }
        let left = normalized_midi_notes(
            &std::fs::read(&args[1]).map_err(|error| format!("{}: {error}", args[1]))?,
        )?;
        let right = normalized_midi_notes(
            &std::fs::read(&args[2]).map_err(|error| format!("{}: {error}", args[2]))?,
        )?;
        if left != right {
            let first = left
                .iter()
                .zip(&right)
                .position(|(left, right)| left != right)
                .unwrap_or(left.len().min(right.len()));
            return Err(format!(
                "normalized MIDI notes differ at event {first} (left={} right={})",
                left.len(),
                right.len()
            ));
        }
        println!("identical=true note_events={}", left.len());
        return Ok(());
    }
    if args.first().map(String::as_str) == Some("extract-sequences") {
        if args.len() != 3 {
            return Err(USAGE.into());
        }
        let index: serde_json::Value = serde_json::from_slice(
            &std::fs::read(&args[1]).map_err(|error| format!("{}: {error}", args[1]))?,
        )
        .map_err(|error| error.to_string())?;
        let names = index
            .get("names")
            .and_then(serde_json::Value::as_object)
            .ok_or("song names differ")?;
        let directory = index
            .get("directory")
            .and_then(serde_json::Value::as_str)
            .ok_or("song directory differs")?;
        let mut output = String::from(
            "# Byte-exact GS1 SMSH sequence sources. Human titles live only in games/music.tsv.\nsound_id\tclass\taddress\tsize\tsource\n",
        );
        for sequence in index
            .get("sequences")
            .and_then(serde_json::Value::as_array)
            .ok_or("song sequence index differs")?
        {
            let fields = sequence.as_array().ok_or("song sequence row differs")?;
            if fields.len() != 4 {
                return Err("song sequence row width differs".into());
            }
            let id = fields[0].as_u64().ok_or("song sequence ID differs")?;
            let class = fields[1].as_str().ok_or("song sequence class differs")?;
            let address = fields[2].as_str().ok_or("song sequence address differs")?;
            let size = fields[3].as_str().ok_or("song sequence size differs")?;
            let name = names
                .get(&id.to_string())
                .and_then(serde_json::Value::as_str)
                .ok_or("song sequence name differs")?;
            output.push_str(&format!(
                "{id}\t{class}\t{address}\t{size}\tsongs/{directory}/{name}.mid\n"
            ));
        }
        std::fs::write(&args[2], output).map_err(|error| format!("{}: {error}", args[2]))?;
        return Ok(());
    }
    if args.first().map(String::as_str) == Some("extract-sound-table") {
        if args.len() != 5 {
            return Err(USAGE.into());
        }
        let table = source(&args[1])?;
        let sequences =
            std::fs::read_to_string(&args[2]).map_err(|error| format!("{}: {error}", args[2]))?;
        let mut rows = sequences.lines().filter(|line| !line.starts_with('#'));
        if rows.next() != Some("sound_id\tclass\taddress\tsize\tsource") {
            return Err("sequence table header differs".into());
        }
        let mut sources = std::collections::BTreeMap::new();
        for row in rows {
            let fields = row.split('\t').collect::<Vec<_>>();
            if fields.len() != 5 {
                return Err("sequence table row width differs".into());
            }
            let id = fields[0]
                .parse::<u64>()
                .map_err(|_| "sequence table ID differs")?;
            if sources.insert(id, fields[4].to_string()).is_some() {
                return Err("sequence table ID is duplicated".into());
            }
        }
        let residuals =
            std::fs::read_to_string(&args[3]).map_err(|error| format!("{}: {error}", args[3]))?;
        let mut residual_rows = residuals.lines().filter(|line| !line.starts_with('#'));
        if residual_rows.next()
            != Some("kind\tname\taddress\tend\tpriority\treverb\ttone_bank\tfill")
        {
            return Err("music residual table header differs".into());
        }
        let empty_headers = residual_rows
            .filter_map(|row| {
                let fields = row.split('\t').collect::<Vec<_>>();
                ((4..=8).contains(&fields.len()) && fields[0] == "empty_header").then(|| fields[2])
            })
            .collect::<std::collections::BTreeSet<_>>();
        let symbols = table
            .symbols
            .iter()
            .map(|(name, value)| {
                let address = match value {
                    SymbolValue::Text(value) => value.clone(),
                    SymbolValue::Number(value) => format!("0x{:08x}", *value as u32),
                };
                (name.as_str(), address)
            })
            .collect::<std::collections::BTreeMap<_, _>>();
        let mut output = String::from(
            "# ROM-derived GS1 sound-selection table. Human titles live only in games/music.tsv.\nslot\theader\tplayer\tsource\tstatus\n",
        );
        for (slot, entry) in table.entries.iter().enumerate() {
            let header = symbols
                .get(entry.header.as_str())
                .ok_or("sound-table symbol differs")?;
            let mut source = sources
                .get(&(slot as u64))
                .map(String::as_str)
                .unwrap_or("");
            let status = if empty_headers.contains(header.as_str()) {
                source = "residuals.tsv";
                "empty"
            } else if !source.is_empty() {
                "byte-exact"
            } else if header == "0x080fd044" {
                "empty"
            } else {
                "unavailable"
            };
            output.push_str(&format!(
                "{slot}\t{header}\t{}\t{source}\t{status}\n",
                entry.player
            ));
        }
        std::fs::write(&args[4], output).map_err(|error| format!("{}: {error}", args[4]))?;
        return Ok(());
    }
    if args.first().map(String::as_str) == Some("extract-smsh-source") {
        if args.len() != 4 {
            return Err(USAGE.into());
        }
        let header = u32::from_str_radix(args[2].trim_start_matches("0x"), 16)
            .map_err(|_| "sequence header must be hexadecimal".to_string())?;
        let rom = std::fs::read(&args[1]).map_err(|error| format!("{}: {error}", args[1]))?;
        let extracted = crate::smsh::extract_sequence(&rom, header, &args[3])?;
        println!(
            "{}",
            serde_json::to_string(&extracted.source).map_err(|error| error.to_string())?
        );
        eprintln!(
            "base=0x{:08x} end=0x{:08x} tracks={} events={}",
            extracted.base, extracted.end, extracted.tracks, extracted.events
        );
        return Ok(());
    }
    if args.first().map(String::as_str) == Some("add-smsh-tempo") {
        if args.len() < 2 {
            return Err(USAGE.into());
        }
        for path in &args[1..] {
            let midi = std::fs::read(path).map_err(|error| format!("{path}: {error}"))?;
            std::fs::write(path, add_smsh_midi_tempos(&midi)?)
                .map_err(|error| format!("{path}: {error}"))?;
        }
        return Ok(());
    }
    if args.first().map(String::as_str) == Some("add-midi-directive") {
        if args.len() != 3 {
            return Err(USAGE.into());
        }
        let midi = std::fs::read(&args[1]).map_err(|error| format!("{}: {error}", args[1]))?;
        let source: serde_json::Value = serde_json::from_slice(
            &std::fs::read(&args[2]).map_err(|error| format!("{}: {error}", args[2]))?,
        )
        .map_err(|error| error.to_string())?;
        if source.get("engine").and_then(serde_json::Value::as_str) != Some("smsh-sequence-sidecar")
        {
            return Err("source is not a sequence build directive".into());
        }
        let directive = serde_json::to_vec(&source).map_err(|error| error.to_string())?;
        std::fs::write(&args[1], add_midi_build_directive(&midi, &directive)?)
            .map_err(|error| format!("{}: {error}", args[1]))?;
        return Ok(());
    }
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
