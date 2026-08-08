// CLI entrypoint for the resource directory tool, mirroring `main()` in
// tools/make/resource_directory.ts.
//
//   resource-directory {export ROM|verify ROM SOURCE|build SOURCE}
//                      [-o FILE] [--address ADDRESS] [--slots COUNT]
//                      [--self-test] [-h|--help]
//
// PORT NOTE: the TypeScript lets errors escape `main`, so bun prints a source
// excerpt and a stack trace before exiting 1. This binary prints
// `error: <message>` on stderr and exits 1. Stdout, the written files and the
// exit codes are identical.

use std::fs;
use std::io::Write;
use std::process::ExitCode;

use resource_directory::{
    build_resource_directory, canonical_json, document, export_resource_directory, hex,
    integer_text, js_number_string, self_test, DIRECTORY_ADDRESS, DIRECTORY_SLOTS, ROM_BASE,
};
use serde_json::Value;

const USAGE: &str = "usage: resource-directory {export ROM|verify ROM SOURCE|build SOURCE} [-o FILE] [--address ADDRESS] [--slots COUNT]";

fn option(args: &[String], name: &str, fallback: Option<String>) -> Option<String> {
    match args.iter().position(|item| item == name) {
        Some(index) => args.get(index + 1).cloned(),
        None => fallback,
    }
}

fn positional(args: &[String]) -> Vec<String> {
    const VALUED: [&str; 4] = ["-o", "--output", "--address", "--slots"];
    args.iter()
        .enumerate()
        .filter(|(index, item)| {
            !item.starts_with('-') && !(*index > 0 && VALUED.contains(&args[index - 1].as_str()))
        })
        .map(|(_, item)| item.clone())
        .collect()
}

fn read_json(path: &str) -> Result<Value, String> {
    let bytes = fs::read(path).map_err(|error| format!("{path}: {error}"))?;
    // `Bun.file().json()` decodes as UTF-8 with replacement, then JSON.parses.
    let text = String::from_utf8_lossy(&bytes);
    serde_json::from_str(&text).map_err(|error| format!("{path}: {error}"))
}

fn integer_option(text: Option<String>, label: &str) -> Result<f64, String> {
    integer_text(text.as_deref(), label)
}

fn run(mut args: Vec<String>) -> Result<(), String> {
    if args.iter().any(|item| item == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        if args.len() == 1 {
            return Ok(());
        }
        args.retain(|item| item != "--self-test");
    }
    if let [command, source] = args.as_slice() {
        if command == "build-stdout" {
            let result = build_resource_directory(&read_json(source)?)?;
            std::io::stdout()
                .write_all(&result)
                .map_err(|error| error.to_string())?;
            return Ok(());
        }
    }
    let words = positional(&args);
    let command = words.first().cloned();
    let input = words.get(1).cloned();
    if command.is_none() || args.iter().any(|item| item == "-h" || item == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    let command = command.expect("checked");
    let output = option(&args, "--output", option(&args, "-o", None));

    if command == "export" {
        let (Some(input), Some(output)) = (input, output) else {
            return Err("export requires ROM and --output".to_string());
        };
        let rom = fs::read(&input).map_err(|error| format!("{input}: {error}"))?;
        let address = integer_option(
            option(&args, "--address", Some(hex(DIRECTORY_ADDRESS))),
            "resource directory address",
        )?;
        let slots = integer_option(
            option(&args, "--slots", Some(js_number_string(DIRECTORY_SLOTS))),
            "resource directory slot count",
        )?;
        let result = export_resource_directory(&rom, address, slots)?;
        let text = format!("{}\n", canonical_json(&result));
        fs::write(&output, text).map_err(|error| format!("{output}: {error}"))?;
        let count = result["slot_count"].as_f64().expect("number");
        println!(
            "slots={} bytes={}",
            js_number_string(count),
            js_number_string(count * 4.0)
        );
        return Ok(());
    }

    if command == "build" {
        let (Some(input), Some(output)) = (input, output) else {
            return Err("build requires SOURCE and --output".to_string());
        };
        let result = build_resource_directory(&read_json(&input)?)?;
        fs::write(&output, &result).map_err(|error| format!("{output}: {error}"))?;
        println!(
            "slots={} bytes={}",
            js_number_string(result.len() as f64 / 4.0),
            js_number_string(result.len() as f64)
        );
        return Ok(());
    }

    if command == "verify" {
        let source = words.get(2).cloned();
        let (Some(input), Some(source)) = (input, source) else {
            return Err("verify requires ROM and SOURCE".to_string());
        };
        let rom = fs::read(&input).map_err(|error| format!("{input}: {error}"))?;
        let value = read_json(&source)?;
        let (start, count) = document(&value)?;
        let built = build_resource_directory(&value)?;
        let start = start - ROM_BASE;
        if start < 0.0
            || start + built.len() as f64 > rom.len() as f64
            || built != rom[start as usize..start as usize + built.len()]
        {
            return Err("resource directory differs from ROM".to_string());
        }
        println!(
            "identical=true slots={} bytes={}",
            js_number_string(count),
            js_number_string(built.len() as f64)
        );
        return Ok(());
    }

    Err(format!("unknown command: {command}"))
}

fn main() -> ExitCode {
    match run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn strings(items: &[&str]) -> Vec<String> {
        items.iter().map(|s| (*s).to_string()).collect()
    }

    #[test]
    fn positional_skips_flag_values() {
        let args = strings(&["export", "rom.gba", "-o", "out.json", "--slots", "8"]);
        assert_eq!(positional(&args), strings(&["export", "rom.gba"]));
        let args = strings(&["verify", "rom.gba", "src.json", "--address", "0x1"]);
        assert_eq!(
            positional(&args),
            strings(&["verify", "rom.gba", "src.json"])
        );
    }

    #[test]
    fn positional_keeps_a_word_that_follows_a_non_valued_flag() {
        let args = strings(&["--self-test", "build"]);
        assert_eq!(positional(&args), strings(&["build"]));
    }

    #[test]
    fn option_returns_nothing_when_the_flag_is_last() {
        // JS: `index >= 0 ? args[index + 1] : fallback` -- a trailing flag
        // yields undefined, it does NOT fall back.
        let args = strings(&["export", "rom.gba", "--slots"]);
        assert_eq!(option(&args, "--slots", Some("1000".to_string())), None);
        assert_eq!(
            option(&args, "--address", Some("0x1".to_string())),
            Some("0x1".to_string())
        );
    }

    #[test]
    fn integer_option_reads_hex_defaults() {
        assert_eq!(
            integer_option(Some(hex(DIRECTORY_ADDRESS)), "x").expect("ok"),
            DIRECTORY_ADDRESS
        );
        assert_eq!(
            integer_option(Some("1000".to_string()), "x").expect("ok"),
            1000.0
        );
        assert_eq!(
            integer_option(None, "resource directory address").unwrap_err(),
            "resource directory address must be an integer"
        );
        assert_eq!(
            integer_option(Some("8.5".to_string()), "x").unwrap_err(),
            "x must be an integer"
        );
    }

    #[test]
    fn unknown_command_is_an_error() {
        assert_eq!(
            run(strings(&["bogus"])).unwrap_err(),
            "unknown command: bogus"
        );
    }

    #[test]
    fn missing_output_is_an_error() {
        assert_eq!(
            run(strings(&["export", "rom.gba"])).unwrap_err(),
            "export requires ROM and --output"
        );
        assert_eq!(
            run(strings(&["build", "src.json"])).unwrap_err(),
            "build requires SOURCE and --output"
        );
        assert_eq!(
            run(strings(&["verify", "rom.gba"])).unwrap_err(),
            "verify requires ROM and SOURCE"
        );
    }

    #[test]
    fn help_and_self_test_succeed() {
        run(strings(&["--help"])).expect("help");
        run(strings(&[])).expect("bare invocation prints usage");
        run(strings(&["--self-test"])).expect("self test");
    }
}
