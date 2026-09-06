use std::io::Write;
use std::process::ExitCode;

use crate::parse_wave_source;

pub fn entry(arguments: &[String]) -> ExitCode {
    let result = match arguments {
        [command, catalog, wav] if command == "build-record-stdout" => (|| {
            let source = parse_wave_source(catalog)?;
            let wav = std::fs::read(wav).map_err(|error| error.to_string())?;
            let (built, report) = crate::build_wave_record(&source, &wav)?;
            eprintln!(
                "{}",
                serde_json::json!({
                    "samples": report.samples as u64,
                    "rate": report.rate as u64,
                    "frequency": report.frequency as u64,
                    "control": report.control as u64,
                    "looped": report.looped,
                    "loop_start": report.loop_start.map(|value| value as u64),
                    "padding_bytes": report.padding_bytes as u64,
                    "padding_fill": report.padding_fill as u64,
                })
            );
            std::io::stdout()
                .write_all(&built)
                .map_err(|error| error.to_string())
        })(),
        [command, index, table] if command == "extract-catalog" => (|| {
            let index_path = std::path::Path::new(index);
            let document: serde_json::Value = serde_json::from_slice(
                &std::fs::read(index_path).map_err(|error| error.to_string())?,
            )
            .map_err(|error| error.to_string())?;
            let waves = document
                .get("waves")
                .and_then(serde_json::Value::as_array)
                .ok_or("wave index differs")?;
            let directory = index_path
                .parent()
                .unwrap_or_else(|| std::path::Path::new("."));
            let mut output = String::from(
                "# ROM-derived GS1 direct-sound samples.\nsample\taddress\tfrequency\tloop_start\tsample_count\tsource\n",
            );
            for (sample, value) in waves.iter().enumerate() {
                let wav_name = value
                    .get("source")
                    .and_then(serde_json::Value::as_str)
                    .ok_or("wave source differs")?;
                let wav =
                    std::fs::read(directory.join(wav_name)).map_err(|error| error.to_string())?;
                let wave_source =
                    parse_wave_source(&serde_json::to_string(value).map_err(|e| e.to_string())?)?;
                let (_, report) = crate::build_wave_record(&wave_source, &wav)?;
                output.push_str(&format!(
                    "{sample}\t{}\t{}\t{}\t{}\tdirect_sound_samples/{wav_name}\n",
                    value
                        .get("address")
                        .and_then(serde_json::Value::as_str)
                        .ok_or("wave address differs")?,
                    report.frequency as u64,
                    report
                        .loop_start
                        .map(|value| (value as u64).to_string())
                        .unwrap_or_default(),
                    report.samples as u64,
                ));
            }
            std::fs::write(table, output).map_err(|error| error.to_string())
        })(),
        [argument] if matches!(argument.as_str(), "-h" | "--help") => {
            println!(
                "usage: audio-wave {{build-record-stdout SOURCE WAV|extract-catalog JSON TSV}}"
            );
            Ok(())
        }
        _ => Err(
            "usage: audio-wave {build-record-stdout SOURCE WAV|extract-catalog JSON TSV}".into(),
        ),
    };
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
