//! CLI mirror of `tools/make/audio_wave.ts`.
//!
//! `--self-test` anywhere in the arguments runs the self-test and prints
//! `self-test=ok`; anything else prints the usage line. Both exit 0. A failing
//! self-test writes the thrown message to stderr and exits 1, matching Bun's
//! behaviour for an uncaught `Error` (Bun also prints a stack trace, which the
//! Rust port omits).

use std::io::Write;
use std::process::ExitCode;

use audio_wave::{ExactWaveHeaderSource, ExactWavePaddingSource, Scalar, WaveRecordSource};

const USAGE: &str =
    "usage: audio-wave build-record-stdout SOURCE WAV | --self-test\n       audio-wave [-h|--help]";

fn scalar(value: &serde_json::Value, label: &str) -> Result<Scalar, String> {
    if let Some(text) = value.as_str() {
        Ok(Scalar::Str(text.to_string()))
    } else if let Some(number) = value.as_f64() {
        Ok(Scalar::Num(number))
    } else {
        Err(format!("{label} must be a number or string"))
    }
}

fn source(text: &str) -> Result<WaveRecordSource, String> {
    let value: serde_json::Value = serde_json::from_str(text).map_err(|error| error.to_string())?;
    let object = value
        .as_object()
        .ok_or_else(|| "wave source must be an object".to_string())?;
    let header = object
        .get("header")
        .map(|value| -> Result<ExactWaveHeaderSource, String> {
            let item = value
                .as_object()
                .ok_or_else(|| "wave header must be an object".to_string())?;
            Ok(ExactWaveHeaderSource {
                control: scalar(
                    item.get("control").unwrap_or(&serde_json::Value::Null),
                    "wave control",
                )?,
                frequency: scalar(
                    item.get("frequency").unwrap_or(&serde_json::Value::Null),
                    "wave frequency",
                )?,
                loop_start: scalar(
                    item.get("loop_start").unwrap_or(&serde_json::Value::Null),
                    "wave loop start",
                )?,
                sample_count: scalar(
                    item.get("sample_count").unwrap_or(&serde_json::Value::Null),
                    "wave sample count",
                )?,
            })
        })
        .transpose()?;
    let padding = object
        .get("padding")
        .map(|value| -> Result<ExactWavePaddingSource, String> {
            let item = value
                .as_object()
                .ok_or_else(|| "wave padding must be an object".to_string())?;
            Ok(ExactWavePaddingSource {
                size: scalar(
                    item.get("size").unwrap_or(&serde_json::Value::Null),
                    "wave padding size",
                )?,
                fill: scalar(
                    item.get("fill").unwrap_or(&serde_json::Value::Null),
                    "wave padding fill",
                )?,
            })
        })
        .transpose()?;
    let loop_start = match object.get("loop_start") {
        Some(serde_json::Value::Null) | None => None,
        Some(value) => Some(
            value
                .as_f64()
                .ok_or_else(|| "wave loop start must be numeric or null".to_string())?,
        ),
    };
    Ok(WaveRecordSource {
        frequency: scalar(
            object.get("frequency").unwrap_or(&serde_json::Value::Null),
            "wave catalog frequency",
        )?,
        loop_start,
        size: scalar(
            object.get("size").unwrap_or(&serde_json::Value::Null),
            "wave record size",
        )?,
        header,
        padding,
    })
}

fn validate_options(args: &[String]) -> Result<(), String> {
    for argument in args {
        if matches!(argument.as_str(), "-h" | "--help" | "--self-test") {
            continue;
        }
        if argument.starts_with('-') {
            return Err(format!("unknown option: {argument}"));
        }
    }
    Ok(())
}

fn run(args: &[String]) -> Result<ExitCode, String> {
    validate_options(args)?;
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return Ok(ExitCode::SUCCESS);
    }
    if let [command, catalog, wav] = args {
        if command == "build-record-stdout" {
            let outcome = (|| {
                let wav = std::fs::read(wav).map_err(|error| error.to_string())?;
                let (built, report) = audio_wave::build_wave_record(&source(catalog)?, &wav)?;
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
            })();
            return outcome
                .map(|()| ExitCode::SUCCESS)
                .map_err(|message| message.to_string());
        }
    }
    if args.iter().any(|arg| arg == "--self-test") {
        audio_wave::self_test().map_err(|message| message.to_string())?;
        Ok(ExitCode::SUCCESS)
    } else {
        println!("{USAGE}");
        Ok(ExitCode::SUCCESS)
    }
}

fn main() -> ExitCode {
    match run(&std::env::args().skip(1).collect::<Vec<_>>()) {
        Ok(code) => code,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn args(values: &[&str]) -> Vec<String> {
        values.iter().map(|value| (*value).to_string()).collect()
    }

    #[test]
    fn help_is_successful_without_reading_inputs() {
        assert_eq!(run(&args(&["-h"])).unwrap(), ExitCode::SUCCESS);
        assert_eq!(run(&args(&["--help"])).unwrap(), ExitCode::SUCCESS);
    }

    #[test]
    fn unknown_options_are_rejected_before_work() {
        assert_eq!(
            run(&args(&[
                "build-record-stdout",
                "missing.json",
                "missing.wav",
                "--bogus"
            ]))
            .unwrap_err(),
            "unknown option: --bogus"
        );
    }
}
