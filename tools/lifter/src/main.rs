use lifter::owners::{self, first_error, score, Module};
use lifter::{lift_owner, tune};
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: lifter <command> [args]\n\
  draft <overlay>:<addressHex> [--span BYTES] [--name NAME] [--out FILE]\n\
      lift the owner and print (or write) the candidate unit\n\
  score <overlay>:<addressHex> [--span BYTES] [--name NAME] [--diff ROWS]\n\
      lift, compile, and compare against the ROM bytes\n\
  tune  <overlay>:<addressHex> [--span BYTES] [--name NAME]\n\
      score, then try call respellings line by line and keep improvements\n\
  batch [--kind WORD] [--limit N] [--all]\n\
      score every unregistered retained module (scene and script kinds)\n\
  list  [--kind WORD] [--all]\n\
      print the retained modules the batch would visit\n\
Candidates are written under out/lifter/.";

struct Options {
    positional: Vec<String>,
    span: Option<u32>,
    name: Option<String>,
    out: Option<PathBuf>,
    diff: usize,
    kind: Option<String>,
    limit: Option<usize>,
    all: bool,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options {
        positional: Vec::new(),
        span: None,
        name: None,
        out: None,
        diff: 40,
        kind: None,
        limit: None,
        all: false,
    };
    let mut iter = arguments.iter();
    while let Some(argument) = iter.next() {
        let mut value = |flag: &str| {
            iter.next()
                .cloned()
                .ok_or_else(|| format!("{flag} needs a value"))
        };
        match argument.as_str() {
            "--span" => {
                options.span = Some(
                    value("--span")?
                        .parse()
                        .map_err(|_| "--span wants a byte count")?,
                )
            }
            "--name" => options.name = Some(value("--name")?),
            "--out" => options.out = Some(PathBuf::from(value("--out")?)),
            "--diff" => {
                options.diff = value("--diff")?
                    .parse()
                    .map_err(|_| "--diff wants a row count")?
            }
            "--kind" => options.kind = Some(value("--kind")?),
            "--limit" => {
                options.limit = Some(
                    value("--limit")?
                        .parse()
                        .map_err(|_| "--limit wants a count")?,
                )
            }
            "--all" => options.all = true,
            other if other.starts_with("--") => return Err(format!("unknown flag {other}")),
            other => options.positional.push(other.to_string()),
        }
    }
    Ok(options)
}

fn scratch_path(root: &Path, owner: &str) -> Result<PathBuf, String> {
    let directory = root.join("out/lifter");
    std::fs::create_dir_all(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?;
    Ok(directory.join(format!("{}.c", owner.replace(':', "_"))))
}

fn owner_argument(options: &Options) -> Result<&str, String> {
    options
        .positional
        .first()
        .map(String::as_str)
        .ok_or_else(|| "an <overlay>:<addressHex> owner is required".to_string())
}

fn draft(root: &Path, options: &Options) -> Result<(), String> {
    let owner = owner_argument(options)?;
    let (unit, _) = lift_owner(root, owner, options.span, options.name.as_deref())?;
    match &options.out {
        Some(path) => {
            std::fs::write(path, unit).map_err(|error| format!("{}: {error}", path.display()))
        }
        None => {
            print!("{unit}");
            Ok(())
        }
    }
}

fn print_diff(report: &str, rows: usize) {
    let mut printed = 0;
    let mut context: Vec<&str> = Vec::new();
    let mut in_hunk = false;
    for line in report.lines().skip(9) {
        let differs = line.starts_with("  -") || line.starts_with("  +") || line.starts_with("  !");
        if differs {
            if !in_hunk {
                for c in context.drain(..) {
                    println!("{c}");
                }
                in_hunk = true;
            }
            println!("{line}");
            printed += 1;
            if printed >= rows {
                break;
            }
        } else if in_hunk {
            println!("{line}");
            in_hunk = false;
        } else {
            context.push(line);
            if context.len() > 3 {
                context.remove(0);
            }
        }
    }
}

fn score_owner(root: &Path, options: &Options) -> Result<i32, String> {
    let owner = owner_argument(options)?;
    let (unit, span) = lift_owner(root, owner, options.span, options.name.as_deref())?;
    let path = match &options.out {
        Some(path) => path.clone(),
        None => scratch_path(root, owner)?,
    };
    std::fs::write(&path, &unit).map_err(|error| format!("{}: {error}", path.display()))?;
    let result = score(root, &path, owner, span)?;
    println!(
        "candidate={} reference={} differing_halfwords={} source={}",
        result.candidate,
        result.reference,
        result.differing,
        path.display()
    );
    if result.differing > 0 {
        print_diff(&result.report, options.diff);
    }
    Ok(i32::from(result.differing != 0))
}

fn tune_owner(root: &Path, options: &Options) -> Result<i32, String> {
    let owner = owner_argument(options)?;
    let (unit, span) = lift_owner(root, owner, options.span, options.name.as_deref())?;
    let path = scratch_path(root, owner)?;
    let (_, best) = if options.all {
        tune::tune(root, &path, owner, span, &unit, |line| println!("{line}"))?
    } else {
        tune::tune_targeted(root, &path, owner, span, &unit, |line| println!("{line}"))?
    };
    println!(
        "candidate={} reference={} differing_halfwords={} source={}",
        best.candidate,
        best.reference,
        best.differing,
        path.display()
    );
    Ok(i32::from(best.differing != 0))
}

fn selected(root: &Path, options: &Options) -> Result<Vec<Module>, String> {
    let mut modules: Vec<Module> = owners::modules(root)?
        .into_iter()
        .filter(|m| match &options.kind {
            Some(kind) => m.kind.contains(kind.as_str()),
            None => m.kind.contains("scene") || m.kind.contains("script"),
        })
        .filter(|m| options.all || !m.registered)
        .collect();
    modules.sort_by(|a, b| b.span.cmp(&a.span).then(a.key().cmp(&b.key())));
    if let Some(limit) = options.limit {
        modules.truncate(limit);
    }
    Ok(modules)
}

fn list(root: &Path, options: &Options) -> Result<(), String> {
    for m in selected(root, options)? {
        println!("{} {} {}", m.key(), m.span, m.kind);
    }
    Ok(())
}

fn batch(root: &Path, options: &Options) -> Result<(), String> {
    let modules = selected(root, options)?;
    let (mut exact, mut differing, mut failed) = (0, 0, 0);
    let mut near: Vec<(u32, String)> = Vec::new();
    for m in &modules {
        let owner = m.key();
        let path = scratch_path(root, &owner)?;
        let outcome = lift_owner(root, &owner, Some(m.span), None).and_then(|(unit, _)| {
            std::fs::write(&path, &unit).map_err(|error| format!("{}: {error}", path.display()))?;
            score(root, &path, &owner, m.span)
        });
        match outcome {
            Ok(result) if result.differing == 0 => {
                exact += 1;
                println!("{owner} {} EXACT", m.span);
            }
            Ok(result) => {
                let mut differing_halfwords = result.differing;
                if options.all && differing_halfwords <= 60 {
                    let unit = std::fs::read_to_string(&path).unwrap_or_default();
                    if let Ok((_, tuned)) =
                        tune::tune_targeted(root, &path, &owner, m.span, &unit, |_| {})
                    {
                        differing_halfwords = tuned.differing;
                    }
                }
                if differing_halfwords == 0 {
                    exact += 1;
                    println!("{owner} {} EXACT tuned from {}", m.span, result.differing);
                    continue;
                }
                differing += 1;
                if differing_halfwords <= 40 {
                    near.push((differing_halfwords, owner.clone()));
                }
                println!("{owner} {} diff {differing_halfwords}", m.span);
            }
            Err(error) => {
                failed += 1;
                let detail = first_error(&error).unwrap_or(error);
                println!("{owner} {} failed {}", m.span, detail.trim());
            }
        }
    }
    near.sort();
    println!(
        "modules={} exact={exact} differing={differing} failed={failed}",
        modules.len()
    );
    if !near.is_empty() {
        println!(
            "near: {}",
            near.iter()
                .map(|(d, owner)| format!("{owner}:{d}"))
                .collect::<Vec<_>>()
                .join(" ")
        );
    }
    Ok(())
}

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let root = owners::root();
    let result = parse(&arguments[1..]).and_then(|options| match command {
        "draft" => draft(&root, &options).map(|_| 0),
        "score" => score_owner(&root, &options),
        "tune" => tune_owner(&root, &options),
        "batch" => batch(&root, &options).map(|_| 0),
        "list" => list(&root, &options).map(|_| 0),
        "study" => study(&root, &options).map(|_| 0),
        "bench" => bench(&root, &options).map(|_| 0),
        "disasm" => disasm(&root, &options).map(|_| 0),
        "-h" | "--help" => {
            println!("{USAGE}");
            Ok(0)
        }
        other => Err(format!("unknown lifter command: {other}\n{USAGE}")),
    });
    match result {
        Ok(0) => ExitCode::SUCCESS,
        Ok(_) => ExitCode::FAILURE,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

/// Study: for every exact owner, pair each `bl` with the adopted source's
/// spelling of that call and print the argument-register load order.
fn study(root: &Path, options: &Options) -> Result<(), String> {
    use lifter::decode::{decode_window, Kind};
    let sources = compiler_core::source_paths::SourcePaths::load(root)?;
    let regions: serde_json::Value = serde_json::from_slice(
        &std::fs::read(root.join("games/gs1/semantic/regions.json")).map_err(|e| e.to_string())?,
    )
    .map_err(|e| e.to_string())?;
    let mut owners: Vec<(String, u32, u32, PathBuf)> = Vec::new();
    for region in regions["manual_regions"]
        .as_array()
        .ok_or("manual_regions")?
    {
        let overlay = region["overlay"].as_str().unwrap_or("");
        let entry = u32::from_str_radix(
            region["entry"]
                .as_str()
                .unwrap_or("")
                .trim_start_matches("0x"),
            16,
        )
        .unwrap_or(0);
        let span = region["span_bytes"].as_u64().unwrap_or(0) as u32;
        let owner =
            compiler_core::source_paths::SourceOwner::parse(&format!("{overlay}:{entry:08x}"))?;
        if let Some(path) = sources.mapped_source_path(owner) {
            owners.push((overlay.to_string(), entry, span, path));
        }
    }
    owners.sort_by_key(|owner| std::cmp::Reverse(owner.2));
    owners.truncate(options.limit.unwrap_or(100));
    let mut images: std::collections::BTreeMap<String, Vec<u8>> = std::collections::BTreeMap::new();
    for (overlay, entry, span, path) in &owners {
        let image = match images.entry(overlay.clone()) {
            std::collections::btree_map::Entry::Occupied(e) => e.into_mut(),
            std::collections::btree_map::Entry::Vacant(e) => {
                e.insert(owners::overlay_image(root, overlay)?)
            }
        };
        let ins = decode_window(image, *entry, *span);
        let text = std::fs::read_to_string(path).map_err(|e| e.to_string())?;
        // Adopted call statements in order: (callee, spelling, argument count).
        let mut spelled: Vec<(u32, String, usize, String, usize)> = Vec::new();
        let mut rest = text.as_str();
        while let Some(at) = rest.find("Func_") {
            let tail = &rest[at + 5..];
            if tail.len() > 9 && tail[..8].bytes().all(|b| b.is_ascii_hexdigit()) {
                let callee = u32::from_str_radix(&tail[..8], 16).unwrap();
                let after = &tail[8..];
                let before = &rest[..at];
                let spelling = if before.ends_with('(') && before.len() >= 6 {
                    let head = before[..before.len() - 1]
                        .rsplit(|c: char| !c.is_ascii_alphanumeric())
                        .next()
                        .unwrap_or("");
                    if head.starts_with("Call") {
                        "Call"
                    } else if head.starts_with("Value") {
                        "Value"
                    } else {
                        ""
                    }
                } else {
                    ""
                };
                let args = if !spelling.is_empty() {
                    after
                        .strip_prefix(", ")
                        .or(after.strip_prefix(")"))
                        .map(|a| a.split(')').next().unwrap_or(""))
                        .unwrap_or("")
                } else if let Some(a) = after.strip_prefix('(') {
                    a.split(')').next().unwrap_or("")
                } else {
                    ""
                };
                let is_call = !spelling.is_empty() || after.starts_with('(');
                if is_call
                    && !before.ends_with("define ")
                    && !before.trim_end().ends_with("s32")
                    && !before.trim_end().ends_with("void")
                    && !before.trim_end().ends_with('*')
                {
                    let n = if args.trim().is_empty() {
                        0
                    } else {
                        args.split(", ").count()
                    };
                    let form = if spelling.is_empty() {
                        "direct".to_string()
                    } else {
                        spelling.to_string()
                    };
                    spelled.push((
                        callee,
                        form,
                        n,
                        args.to_string(),
                        text.len() - rest.len() + at,
                    ));
                }
            }
            rest = &rest[at + 5..];
        }
        let mut cursor = 0;
        let mut loads: Vec<u8> = Vec::new();
        for x in &ins {
            match x.kind {
                Kind::Bl { target } => {
                    if let Some(k) = spelled[cursor.min(spelled.len())..]
                        .iter()
                        .position(|s| s.0 == target)
                    {
                        let s = &spelled[cursor + k];
                        cursor += k + 1;
                        let order: Vec<String> = loads.iter().map(|r| format!("r{r}")).collect();
                        let symbol = format!("Func_{target:08x}(");
                        let declared = if text.contains(&format!("s32 {symbol}")) {
                            "s32"
                        } else if text.contains(&format!("u8 *{symbol}")) {
                            "ptr"
                        } else if text.contains(&format!("void {symbol}")) {
                            "void"
                        } else {
                            "none"
                        };
                        let line_no = text[..s.4].matches('\n').count();
                        let lines: Vec<&str> = text.lines().collect();
                        let prev = lines[..line_no]
                            .iter()
                            .rev()
                            .find(|l| !l.trim().is_empty())
                            .map(|l| l.trim())
                            .unwrap_or("");
                        let next = lines.get(line_no + 1).map(|l| l.trim()).unwrap_or("");
                        println!("{overlay}:{entry:08x} {:08x} {} n={} order={} declared={} args={} |prev={} |next={}", target, s.1, s.2, order.join(""), declared, s.3, prev, next);
                    }
                    loads.clear();
                }
                Kind::MovImm { rd, .. }
                | Kind::LdrPool { rd, .. }
                | Kind::Movs { rd, .. }
                | Kind::AddImm3 { rd, .. }
                | Kind::AddImm8 { rd, .. }
                | Kind::ShiftImm { rd, .. }
                | Kind::AddReg { rd, .. }
                | Kind::Load { rd, .. }
                | Kind::LdrSp { rd, .. }
                | Kind::MovHi { rd, .. }
                | Kind::SubImm3 { rd, .. }
                | Kind::SubImm8 { rd, .. } => {
                    if rd <= 3 {
                        loads.retain(|r| *r != rd);
                        loads.push(rd);
                    }
                }
                Kind::Bcond { .. } | Kind::B { .. } | Kind::Bx(_) | Kind::Pop { .. } => {
                    loads.clear()
                }
                _ => {}
            }
        }
    }
    Ok(())
}

/// Bench: lift the largest exact owners, optionally tune, and report how
/// far the lifter is from each adopted source's bytes.
fn bench(root: &Path, options: &Options) -> Result<(), String> {
    let sources = compiler_core::source_paths::SourcePaths::load(root)?;
    let regions: serde_json::Value = serde_json::from_slice(
        &std::fs::read(root.join("games/gs1/semantic/regions.json")).map_err(|e| e.to_string())?,
    )
    .map_err(|e| e.to_string())?;
    let mut owners: Vec<(String, u32, String)> = Vec::new();
    for region in regions["manual_regions"]
        .as_array()
        .ok_or("manual_regions")?
    {
        let overlay = region["overlay"].as_str().unwrap_or("");
        let entry = u32::from_str_radix(
            region["entry"]
                .as_str()
                .unwrap_or("")
                .trim_start_matches("0x"),
            16,
        )
        .unwrap_or(0);
        let span = region["span_bytes"].as_u64().unwrap_or(0) as u32;
        let owner =
            compiler_core::source_paths::SourceOwner::parse(&format!("{overlay}:{entry:08x}"))?;
        if sources.mapped_relative_path(owner).is_some() {
            let name = sources
                .registered_name(owner)
                .unwrap_or("Lifted")
                .to_string();
            owners.push((format!("{overlay}:{entry:08x}"), span, name));
        }
    }
    owners.sort_by_key(|owner| std::cmp::Reverse(owner.1));
    owners.truncate(options.limit.unwrap_or(100));
    let (mut exact, mut total_base, mut total_final) = (0, 0u64, 0u64);
    for (owner, span, name) in &owners {
        let path = scratch_path(root, owner)?;
        let outcome = lift_owner(root, owner, Some(*span), Some(name)).and_then(|(unit, _)| {
            std::fs::write(&path, &unit).map_err(|error| format!("{}: {error}", path.display()))?;
            let base = score(root, &path, owner, *span)?;
            if !options.all || base.differing == 0 {
                return Ok((base.differing, base.differing));
            }
            let (_, tuned) = tune::tune_targeted(root, &path, owner, *span, &unit, |_| {})?;
            Ok((base.differing, tuned.differing))
        });
        match outcome {
            Ok((base, final_)) => {
                total_base += u64::from(base);
                total_final += u64::from(final_);
                if final_ == 0 {
                    exact += 1;
                }
                println!("{owner} {span} {base} -> {final_}");
            }
            Err(error) => println!(
                "{owner} {span} failed {}",
                first_error(&error).unwrap_or(error).trim()
            ),
        }
    }
    println!(
        "owners={} exact={exact} base={total_base} final={total_final}",
        owners.len()
    );
    Ok(())
}

/// Disasm: the decoded window with instruction indices, for reading the
/// scheduler's evidence next to a diff.
fn disasm(root: &Path, options: &Options) -> Result<(), String> {
    let owner = owner_argument(options)?;
    let (overlay, entry) = owners::parse_owner(owner)?;
    let span = match options.span {
        Some(span) => span,
        None => owners::span_for(root, &overlay, entry)?,
    };
    let image = owners::overlay_image(root, &overlay)?;
    let ins = lifter::decode::decode_window(&image, entry, span);
    let values = lifter::sched::value_calls(&ins);
    for (index, x) in ins.iter().enumerate() {
        let mark = if values.contains(&index) {
            " ; value"
        } else {
            ""
        };
        println!("{index:5} {:08x} {}{mark}", x.addr, x.text);
    }
    Ok(())
}
