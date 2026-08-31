use std::io::Write;
use std::process::ExitCode;

use crate::{ExactWaveHeaderSource, ExactWavePaddingSource, Scalar, WaveRecordSource};

fn scalar(value: Option<&serde_json::Value>, label: &str) -> Result<Scalar, String> {
    match value {
        Some(value) if value.is_string() => Ok(Scalar::Str(value.as_str().unwrap().into())),
        Some(value) if value.is_number() => Ok(Scalar::Num(value.as_f64().unwrap())),
        _ => Err(format!("{label} must be a number or string")),
    }
}

fn source(text: &str) -> Result<WaveRecordSource, String> {
    let value: serde_json::Value = serde_json::from_str(text).map_err(|error| error.to_string())?;
    let object = value.as_object().ok_or("wave source must be an object")?;
    let header = object
        .get("header")
        .map(|value| -> Result<ExactWaveHeaderSource, String> {
            let item = value.as_object().ok_or("wave header must be an object")?;
            Ok(ExactWaveHeaderSource {
                control: scalar(item.get("control"), "wave control")?,
                frequency: scalar(item.get("frequency"), "wave frequency")?,
                loop_start: scalar(item.get("loop_start"), "wave loop start")?,
                sample_count: scalar(item.get("sample_count"), "wave sample count")?,
            })
        })
        .transpose()?;
    let padding = object
        .get("padding")
        .map(|value| -> Result<ExactWavePaddingSource, String> {
            let item = value.as_object().ok_or("wave padding must be an object")?;
            Ok(ExactWavePaddingSource {
                size: scalar(item.get("size"), "wave padding size")?,
                fill: scalar(item.get("fill"), "wave padding fill")?,
            })
        })
        .transpose()?;
    let loop_start = match object.get("loop_start") {
        None | Some(serde_json::Value::Null) => None,
        Some(value) => Some(
            value
                .as_f64()
                .ok_or("wave loop start must be numeric or null")?,
        ),
    };
    Ok(WaveRecordSource {
        frequency: scalar(object.get("frequency"), "wave catalog frequency")?,
        loop_start,
        size: scalar(object.get("size"), "wave record size")?,
        header,
        padding,
    })
}

pub fn entry(arguments: &[String]) -> ExitCode {
    let result = match arguments {
        [command, catalog, wav] if command == "build-record-stdout" => (|| {
            let source = source(catalog)?;
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
                    source(&serde_json::to_string(value).map_err(|e| e.to_string())?)?;
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
