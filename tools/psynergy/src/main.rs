use psynergy::{compare, decode, lift, repair, unit};
use std::fs::{self, OpenOptions};
use std::io::Write;
use std::path::PathBuf;
use std::process::ExitCode;

const USAGE: &str = "usage: psynergy <command> [args]\n\
  decompile INPUT       recover draft C from an explicit Thumb image\n\
  disassemble INPUT     decode a bounded Thumb image\n\
  diff ACTUAL EXPECTED  compare bytes without compiling or resolving owners\n\
  repair SOURCE         enumerate or emit a named C repair\n\
  inspect allocator DIR read existing GCC allocation dumps\n\
  convert FORMAT        convert explicit files (convert --help lists formats)\n\
No default ROM, project registry, compiler route, or adoption authority.";
const CODE_USAGE: &str = "usage: psynergy decompile INPUT --base ADDRESS --entry ADDRESS --span BYTES [--name NAME] [--out FILE]\n\
       psynergy disassemble INPUT --base ADDRESS --entry ADDRESS --span BYTES [--out FILE]";
const DIFF_USAGE: &str = "usage: psynergy diff ACTUAL EXPECTED [--width 1|2|4]\nExit status: 0 identical, 1 different, 2 invalid input. No compilation or relocation.";
const REPAIR_USAGE: &str = "usage: psynergy repair SOURCE --repair OPERATION [OPERANDS] [--repair OPERATION [OPERANDS]] [--choice N] [--out FILE]\n\
Without --choice, report the finite search space. --choice 0 is the original source.\n\
Operations (one or two per plan):\n\
  swap-declarations LEFT RIGHT\n\
  split-lifetime NAME\n\
  merge-zero-carrier\n\
  reciprocal-role-swap NAME\n\
  preload-adjacent-halfwords DEST1 SRC1 DEST2 SRC2 CARRIER\n\
  materialize-message-and-merge-count VALUE MESSAGE COORDINATE COUNT\n\
  split-opposite-side-and-scaled-offset SIDE OPPOSITE\n\
  merge-carrier-phases EARLIER LATER\n\
  mirror-relational-guards\n\
Repairs require caller evidence; emitted C is not compiled, scored, or adopted.";
const INSPECT_USAGE: &str = "usage: psynergy inspect allocator DIR\nReads one .rtl, .lreg and .greg dump from DIR; does not run a compiler.";
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
    if command == "disassemble" && name.is_some() {
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

fn diff(arguments: &[String]) -> Result<(String, u8), String> {
    if arguments.len() != 2 && arguments.len() != 4 {
        return Err(DIFF_USAGE.into());
    }
    let width = if arguments.len() == 4 {
        if arguments[2] != "--width" {
            return Err(DIFF_USAGE.into());
        }
        match arguments[3].as_str() {
            "1" => 1,
            "2" => 2,
            "4" => 4,
            _ => return Err(DIFF_USAGE.into()),
        }
    } else {
        1
    };
    let read = |path: &str| fs::read(path).map_err(|error| format!("{path}: {error}"));
    let actual = read(&arguments[0])?;
    let expected = read(&arguments[1])?;
    let differences = compare::differing_offsets(&actual, &expected, width);
    let mut text = format!(
        "actual={} expected={} unit_width={width} differing_units={} identical={}\n",
        actual.len(),
        expected.len(),
        differences.len(),
        differences.is_empty()
    );
    for offset in differences.iter().take(32) {
        text.push_str(&format!("offset=0x{offset:x}\n"));
    }
    if differences.len() > 32 {
        text.push_str(&format!("omitted={}\n", differences.len() - 32));
    }
    Ok((text, u8::from(!differences.is_empty())))
}

fn repair_operation(args: &mut std::slice::Iter<'_, String>) -> Result<repair::Repair, String> {
    use repair::Repair;
    let name = args.next().ok_or("--repair needs an operation")?;
    let mut operand = || {
        args.next()
            .filter(|arg| !arg.starts_with("--"))
            .cloned()
            .ok_or_else(|| format!("missing operand for {name}"))
    };
    Ok(match name.as_str() {
        "swap-declarations" => Repair::SwapDeclarations {
            left: operand()?,
            right: operand()?,
        },
        "split-lifetime" => Repair::SplitLifetime { name: operand()? },
        "merge-zero-carrier" => Repair::MergeZeroCarrier,
        "reciprocal-role-swap" => Repair::ReciprocalRoleSwap { name: operand()? },
        "preload-adjacent-halfwords" => Repair::PreloadAdjacentHalfwords {
            first_destination: operand()?,
            first_source: operand()?,
            second_destination: operand()?,
            second_source: operand()?,
            carrier: operand()?,
        },
        "materialize-message-and-merge-count" => Repair::MaterializeMessageAndMergeCount {
            indexed_value: operand()?,
            message: operand()?,
            coordinate: operand()?,
            count: operand()?,
        },
        "split-opposite-side-and-scaled-offset" => Repair::SplitOppositeSideAndScaledOffset {
            side: operand()?,
            opposite: operand()?,
        },
        "merge-carrier-phases" => Repair::MergeCarrierPhases {
            earlier: operand()?,
            later: operand()?,
        },
        "mirror-relational-guards" => Repair::MirrorRelationalGuards,
        _ => return Err(format!("unknown repair {name}\n{REPAIR_USAGE}")),
    })
}

fn repair(arguments: &[String]) -> Result<String, String> {
    let path = arguments.first().ok_or(REPAIR_USAGE)?;
    let mut args = arguments[1..].iter();
    let mut repairs = Vec::new();
    let mut choice = None;
    let mut out = None;
    while let Some(flag) = args.next() {
        match flag.as_str() {
            "--repair" => repairs.push(repair_operation(&mut args)?),
            "--choice" if choice.is_none() => {
                choice = Some(
                    number(args.next().ok_or("--choice needs an index")?, "--choice")? as usize,
                )
            }
            "--out" if out.is_none() => {
                out = Some(PathBuf::from(args.next().ok_or("--out needs a path")?))
            }
            _ => return Err(format!("unknown or repeated option {flag}\n{REPAIR_USAGE}")),
        }
    }
    if out.is_some() && choice.is_none() {
        return Err("--out requires --choice".into());
    }
    let plan = repair::RepairPlan::try_from_repairs(repairs)?;
    let source = fs::read_to_string(path).map_err(|error| format!("{path}: {error}"))?;
    let permutation = repair::enumerate(&source, &plan)?;
    let report = format!(
        "repair={} raw_choices={} unique_choices={}\n",
        plan.label(),
        permutation.raw_count(),
        permutation.count()
    );
    match choice {
        None => Ok(report),
        Some(choice) => {
            let text = permutation.evaluate(choice)?;
            let result = output(text, out)?;
            eprint!("{report}");
            Ok(result)
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
        "decompile" | "disassemble" if help => Ok((CODE_USAGE.into(), 0)),
        "decompile" | "disassemble" => code(command, rest).map(|text| (text, 0)),
        "diff" if help => Ok((DIFF_USAGE.into(), 0)),
        "diff" => diff(rest),
        "repair" if help => Ok((REPAIR_USAGE.into(), 0)),
        "repair" => repair(rest).map(|text| (text, 0)),
        "inspect" if help || rest == ["allocator", "--help"] || rest == ["allocator", "-h"] => {
            Ok((INSPECT_USAGE.into(), 0))
        }
        "inspect" if rest.len() == 2 && rest[0] == "allocator" => {
            psynergy::allocator::inspect(std::path::Path::new(&rest[1])).map(|text| (text, 0))
        }
        "inspect" => Err(INSPECT_USAGE.into()),
        "convert" => psynergy::convert::run(rest).map(|_| (String::new(), 0)),
        _ => Err(format!("unknown psynergy command: {command}\n{USAGE}")),
    };
    match result {
        Ok((source, status)) => {
            if !source.is_empty() {
                print!("{source}");
            }
            ExitCode::from(status)
        }
        Err(error) => {
            eprintln!("{error}");
            ExitCode::from(2)
        }
    }
}
