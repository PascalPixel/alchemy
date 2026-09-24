mod cli;

use psynergy::{decode, lift, unit};
use std::fs::{self, OpenOptions};
use std::io::Write;
use std::path::PathBuf;
use std::process::ExitCode;

const USAGE: &str = "usage: psynergy <command> [args]\n\
  decompile INPUT       recover draft C from an explicit Thumb image\n\
  disassemble INPUT     decode a bounded Thumb image\n\
  decode-lz INPUT       decode one tagged LZ stream at --offset\n\
No default ROM, project registry, compiler route, or adoption authority.";
const CODE_USAGE: &str = "usage: psynergy decompile INPUT --base ADDRESS --entry ADDRESS --span BYTES [--name NAME] [--out FILE]\n\
       psynergy disassemble INPUT --base ADDRESS --entry ADDRESS --span BYTES [--out FILE]";
const KEYWORDS: &[&str] = &[
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else",
    "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", "register",
    "restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while",
];

fn number(value: &str, label: &str) -> Result<u32, String> {
    let (radix, digits) = value.strip_prefix("0x").map_or((10, value), |v| (16, v));
    if digits.is_empty() {
        return Err(format!(
            "{label} wants a decimal or 0x-prefixed hexadecimal value"
        ));
    }
    u32::from_str_radix(digits, radix).map_err(|_| format!("invalid {label}: {value}"))
}

fn valid_name(name: &str) -> bool {
    let mut chars = name.chars();
    chars
        .next()
        .is_some_and(|c| c == '_' || c.is_ascii_alphabetic())
        && chars.all(|c| c == '_' || c.is_ascii_alphanumeric())
        && !KEYWORDS.contains(&name)
}

fn code(command: &str, arguments: &[String]) -> Result<String, String> {
    let input = arguments.first().ok_or_else(|| CODE_USAGE.to_string())?;
    let mut base = None;
    let mut entry = None;
    let mut span = None;
    let mut name = None;
    let mut out = None;
    let mut i = 1;
    while i < arguments.len() {
        let flag = &arguments[i];
        let value = |at: usize| {
            arguments
                .get(at)
                .ok_or_else(|| format!("{flag} needs a value"))
        };
        match flag.as_str() {
            "--base" => {
                if base.is_some() {
                    return Err("duplicate --base".to_string());
                }
                base = Some(number(value(i + 1)?, "--base")?);
            }
            "--entry" => {
                if entry.is_some() {
                    return Err("duplicate --entry".to_string());
                }
                entry = Some(number(value(i + 1)?, "--entry")?);
            }
            "--span" => {
                if span.is_some() {
                    return Err("duplicate --span".to_string());
                }
                span = Some(number(value(i + 1)?, "--span")?);
            }
            "--name" => {
                if name.is_some() {
                    return Err("duplicate --name".to_string());
                }
                name = Some(value(i + 1)?.clone());
            }
            "--out" => {
                if out.is_some() {
                    return Err("duplicate --out".to_string());
                }
                out = Some(PathBuf::from(value(i + 1)?));
            }
            other => return Err(format!("unknown flag {other}\n{CODE_USAGE}")),
        }
        i += 2;
    }
    let base = base.ok_or_else(|| "--base is required".to_string())?;
    let entry = entry.ok_or_else(|| "--entry is required".to_string())?;
    let span = span.ok_or_else(|| "--span is required".to_string())?;
    if base & 1 != 0 || entry & 1 != 0 {
        return Err("--base and --entry must be halfword-aligned".to_string());
    }
    if entry < base {
        return Err("--entry must not precede --base".to_string());
    }
    if span == 0 || span & 1 != 0 {
        return Err("--span must be a positive even byte count".to_string());
    }
    entry
        .checked_add(span)
        .ok_or_else(|| "--entry plus --span overflows 32-bit address space".to_string())?;
    if command != "decompile" && name.is_some() {
        return Err("--name applies only to decompile".into());
    }
    let name = name.unwrap_or_else(|| "Function".to_string());
    if !valid_name(&name) {
        return Err(format!("invalid C identifier: {name}"));
    }
    let image = fs::read(input).map_err(|error| format!("{input}: {error}"))?;
    let start =
        usize::try_from(entry - base).map_err(|_| "address range is too large".to_string())?;
    let image_end = start
        .checked_add(span as usize)
        .ok_or_else(|| "address range is too large".to_string())?;
    if image_end > image.len() {
        return Err(format!(
            "--entry/--span exceed input ({image_len} bytes)",
            image_len = image.len()
        ));
    }
    let instructions = decode::decode_window_at(&image, base, entry, span);
    let source = if command == "decompile" {
        let symbols = |_: u32, _: lift::ReferenceKind| None;
        let (body, tables) = unit::bodies(&instructions, &symbols);
        unit::compose(entry, &name, &body, &tables)
    } else {
        instructions
            .iter()
            .map(|ins| format!("{:08x}: {}\n", ins.addr, ins.text))
            .collect()
    };
    output(source, out)
}

fn output(source: String, out: Option<PathBuf>) -> Result<String, String> {
    match out {
        None => Ok(source),
        Some(path) => {
            let mut file = OpenOptions::new()
                .write(true)
                .create_new(true)
                .open(&path)
                .map_err(|error| format!("{}: {error}", path.display()))?;
            if let Err(error) = file.write_all(source.as_bytes()) {
                let _ = fs::remove_file(&path);
                return Err(format!("{}: {error}", path.display()));
            }
            Ok(String::new())
        }
    }
}

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.is_empty() || arguments == ["--help"] || arguments == ["-h"] {
        println!("{USAGE}");
        return if arguments.is_empty() {
            ExitCode::from(2)
        } else {
            ExitCode::SUCCESS
        };
    }
    let command = arguments[0].as_str();
    let rest = &arguments[1..];
    let help = rest == ["--help"] || rest == ["-h"];
    let result = match command {
        "decompile" | "disassemble" if help => Ok(CODE_USAGE.into()),
        "decompile" | "disassemble" => code(command, rest),
        "decode-lz" if help => Ok(cli::decode_lz::USAGE.into()),
        "decode-lz" => cli::decode_lz::run(rest),
        _ => Err(format!("unknown psynergy command: {command}\n{USAGE}")),
    };
    match result {
        Ok(source) => {
            if !source.is_empty() {
                print!("{source}");
            }
            ExitCode::SUCCESS
        }
        Err(error) => {
            eprintln!("{error}");
            ExitCode::from(2)
        }
    }
}
