use lifter::owners::{self, first_error, score, score_extending, Module};
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
    path: Option<String>,
    source: Option<PathBuf>,
    only: Option<PathBuf>,
    jobs: Option<usize>,
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
        path: None,
        source: None,
        only: None,
        jobs: None,
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
            "--path" => options.path = Some(value("--path")?),
            "--source" => options.source = Some(PathBuf::from(value("--source")?)),
            "--only" => options.only = Some(PathBuf::from(value("--only")?)),
            "--jobs" => {
                options.jobs = Some(
                    value("--jobs")?
                        .parse()
                        .map_err(|_| "--jobs wants a count")?,
                )
            }
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

/// Lists the loader veneer tables of an overlay image: word-aligned runs of
/// `ldr r4, [pc, #0]; bx r4; .4byte target` (halfwords 4c00 4720 then a
/// ROM or overlay address). Prints `<overlay>:<start> <end> <count>`.
fn veneers(root: &Path, options: &Options) -> Result<i32, String> {
    let overlay = options
        .positional
        .first()
        .ok_or_else(|| "an overlay id such as resource_36f is required".to_string())?;
    let image = owners::overlay_image(root, overlay)?;
    let half = |at: usize| u16::from_le_bytes([image[at], image[at + 1]]);
    let word =
        |at: usize| u32::from_le_bytes([image[at], image[at + 1], image[at + 2], image[at + 3]]);
    let is_veneer = |at: usize| {
        at + 8 <= image.len() && half(at) == 0x4c00 && half(at + 2) == 0x4720 && {
            let target = word(at + 4);
            (0x0800_0000..0x0a00_0000).contains(&target)
                || (0x0200_0000..0x0204_0000).contains(&target)
        }
    };
    let mut at = 0usize;
    while at + 8 <= image.len() {
        if !is_veneer(at) {
            at += 4;
            continue;
        }
        let start = at;
        while is_veneer(at) {
            at += 8;
        }
        println!(
            "{overlay}:{:08x} {:08x} {}",
            lifter::decode::OVERLAY_BASE + start as u32,
            lifter::decode::OVERLAY_BASE + at as u32,
            (at - start) / 8
        );
    }
    Ok(0)
}

/// Carves an unregistered overlay stretch into functions: one starts at the
/// entry and at every `push {.., lr}` that follows a return; each spans up
/// to the next start, pool and padding included. Prints `owner span`.
fn split_owner(root: &Path, options: &Options) -> Result<i32, String> {
    let owner = owner_argument(options)?;
    let (overlay, entry) = owners::parse_owner(owner)?;
    let span = options
        .span
        .ok_or_else(|| "split needs --span <bytes> for the stretch".to_string())?;
    let image = owners::overlay_image(root, &overlay)?;
    let end = entry + span;
    // A stretch often opens with a table left behind by the previous
    // function: the first function starts where decoding is clean, that is,
    // every instruction up to the first return decodes and a return exists.
    let clean = |at: u32| {
        let ins = lifter::decode::decode_window(&image, at, end - at);
        let mut returned = false;
        for x in &ins {
            if matches!(x.kind, lifter::decode::Kind::Unknown(_)) {
                return false;
            }
            if matches!(
                x.kind,
                lifter::decode::Kind::Bx(_) | lifter::decode::Kind::Pop { pc: true }
            ) {
                returned = true;
                break;
            }
        }
        returned && ins.first().is_some_and(|x| x.addr == at)
    };
    // Pool words (ROM or RAM addresses) and zero padding decode as harmless
    // shifts and moves: skip them before looking for the first clean start.
    let half = |at: u32| {
        let k = (at - lifter::decode::OVERLAY_BASE) as usize;
        image
            .get(k..k + 2)
            .map(|b| u16::from_le_bytes([b[0], b[1]]))
            .unwrap_or(0)
    };
    let word = |at: u32| u32::from(half(at)) | (u32::from(half(at + 2)) << 16);
    let mut first = entry;
    while first + 2 <= end {
        let w = word(first);
        let pool = first % 4 == 0
            && first + 4 <= end
            && ((0x0200_0000..0x0204_0000).contains(&w)
                || (0x0800_0000..0x0a00_0000).contains(&w)
                || (0x0300_0000..0x0300_8000).contains(&w)
                || w == 0);
        if pool {
            first += 4;
        } else if half(first) == 0 {
            first += 2;
        } else {
            break;
        }
    }
    // Table words that happen to decode as shifts still fool the clean rule:
    // a `push {.., lr}` within the next 64 bytes is the first function.
    let prologue = (first..end.min(first + 64))
        .step_by(2)
        .find(|&at| half(at) & 0xff00 == 0xb500 && clean(at));
    let Some(entry) = prologue.or_else(|| (first..end).step_by(2).find(|&at| clean(at))) else {
        return Ok(0);
    };
    let span = end - entry;
    let ins = lifter::decode::decode_window(&image, entry, span);
    let mut starts = vec![entry];
    let mut previous_return = false;
    for x in &ins {
        let prologue = matches!(x.kind, lifter::decode::Kind::Push { lr: true });
        if prologue && previous_return && x.addr != entry {
            starts.push(x.addr);
        }
        previous_return = matches!(
            x.kind,
            lifter::decode::Kind::Bx(_) | lifter::decode::Kind::Pop { pc: true }
        );
    }
    starts.sort_unstable();
    starts.dedup();
    for (k, start) in starts.iter().enumerate() {
        let end = starts.get(k + 1).copied().unwrap_or(entry + span);
        println!("{overlay}:{start:08x} {}", end - start);
    }
    Ok(0)
}

fn score_owner(root: &Path, options: &Options) -> Result<i32, String> {
    let owner = owner_argument(options)?;
    let (unit, span) = lift_owner(root, owner, options.span, options.name.as_deref())?;
    let path = match &options.out {
        Some(path) => path.clone(),
        None => scratch_path(root, owner)?,
    };
    std::fs::write(&path, &unit).map_err(|error| format!("{}: {error}", path.display()))?;
    let result = match owners::parse_main_owner(owner) {
        Some(address) => owners::score_main(root, &path, address, span)?,
        None => score_extending(root, &path, owner, span)?,
    };
    println!(
        "candidate={} reference={} differing_halfwords={} source={}{}",
        result.candidate,
        result.reference,
        result.differing,
        path.display(),
        match result.extended {
            Some(extended) => format!(" span={extended} (extended past the registered {span})"),
            None => String::new(),
        }
    );
    if result.differing > 0 {
        print_diff(&result.report, options.diff);
    }
    Ok(i32::from(result.differing != 0))
}

fn tune_owner(root: &Path, options: &Options) -> Result<i32, String> {
    let owner = owner_argument(options)?;
    let (mut unit, span) = lift_owner(root, owner, options.span, options.name.as_deref())?;
    // `--source <file>` tunes a hand-finished unit instead of the draft.
    if let Some(source) = &options.source {
        unit = std::fs::read_to_string(source)
            .map_err(|error| format!("{}: {error}", source.display()))?;
    }
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
    if let Some(only) = &options.only {
        let keys = std::fs::read_to_string(only).map_err(|e| format!("{}: {e}", only.display()))?;
        let wanted: Vec<String> = keys
            .lines()
            .filter_map(|l| l.split_whitespace().next())
            .map(str::to_string)
            .collect();
        modules.retain(|m| wanted.contains(&m.key()));
    }
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

/// One module's batch result: the distance after any tuning, or the failure.
enum Outcome {
    Scored { differing: u32 },
    Failed,
}

/// Runs `work` over `items` on `jobs` threads. Results keep item order; the
/// work itself prints each line as it lands, so progress shows during the run.
fn parallel<T: Sync, R: Send>(items: &[T], jobs: usize, work: impl Fn(&T) -> R + Sync) -> Vec<R> {
    let next = std::sync::atomic::AtomicUsize::new(0);
    let results: std::sync::Mutex<Vec<Option<R>>> =
        std::sync::Mutex::new((0..items.len()).map(|_| None).collect());
    std::thread::scope(|scope| {
        for _ in 0..jobs.max(1) {
            scope.spawn(|| loop {
                let index = next.fetch_add(1, std::sync::atomic::Ordering::SeqCst);
                if index >= items.len() {
                    break;
                }
                let result = work(&items[index]);
                results.lock().unwrap()[index] = Some(result);
            });
        }
    });
    results
        .into_inner()
        .unwrap()
        .into_iter()
        .map(|r| r.expect("every item was worked"))
        .collect()
}

fn jobs(options: &Options) -> usize {
    options.jobs.unwrap_or_else(|| {
        std::thread::available_parallelism()
            .map(|n| n.get())
            .unwrap_or(4)
    })
}

fn batch_one(root: &Path, options: &Options, m: &Module) -> Outcome {
    let owner = m.key();
    let path = match scratch_path(root, &owner) {
        Ok(path) => path,
        Err(error) => {
            println!("{owner} {} failed {error}", m.span);
            return Outcome::Failed;
        }
    };
    let outcome = lift_owner(root, &owner, Some(m.span), None).and_then(|(unit, _)| {
        std::fs::write(&path, &unit).map_err(|error| format!("{}: {error}", path.display()))?;
        score_extending(root, &path, &owner, m.span)
    });
    match outcome {
        Ok(result) => {
            let mut differing = result.differing;
            if options.all && differing > 0 && differing <= 60 {
                let unit = std::fs::read_to_string(&path).unwrap_or_default();
                if let Ok((_, tuned)) =
                    tune::tune_targeted(root, &path, &owner, m.span, &unit, |_| {})
                {
                    differing = tuned.differing;
                }
            }
            match (differing, result.differing) {
                (0, 0) if result.extended.is_some() => println!(
                    "{owner} {} EXACT span={}",
                    m.span,
                    result.extended.unwrap_or(m.span)
                ),
                (0, 0) => println!("{owner} {} EXACT", m.span),
                (0, base) => println!("{owner} {} EXACT tuned from {base}", m.span),
                (d, _) => println!("{owner} {} diff {d}", m.span),
            }
            Outcome::Scored { differing }
        }
        Err(error) => {
            let detail = first_error(&error).unwrap_or(error);
            println!("{owner} {} failed {}", m.span, detail.trim());
            Outcome::Failed
        }
    }
}

fn batch(root: &Path, options: &Options) -> Result<(), String> {
    let modules = selected(root, options)?;
    let outcomes = parallel(&modules, jobs(options), |m| batch_one(root, options, m));
    let (mut exact, mut differing, mut failed) = (0, 0, 0);
    let mut near: Vec<(u32, String)> = Vec::new();
    for (m, outcome) in modules.iter().zip(&outcomes) {
        match outcome {
            Outcome::Scored { differing: 0 } => exact += 1,
            Outcome::Scored { differing: d } => {
                differing += 1;
                if *d <= 40 {
                    near.push((*d, m.key()));
                }
            }
            Outcome::Failed => failed += 1,
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
        "split" => split_owner(&root, &options),
        "veneers" => veneers(&root, &options),
        "adopt" => adopt_owner(&root, &options).map(|_| 0),
        "tune" => tune_owner(&root, &options),
        "imports" => imports_owner(&root, &options),
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
    let outcomes = parallel(&owners, jobs(options), |(owner, span, name)| {
        let outcome = scratch_path(root, owner).and_then(|path| {
            let (unit, _) = lift_owner(root, owner, Some(*span), Some(name))?;
            std::fs::write(&path, &unit).map_err(|error| format!("{}: {error}", path.display()))?;
            let base = score(root, &path, owner, *span)?;
            if !options.all || base.differing == 0 {
                return Ok((base.differing, base.differing));
            }
            let (_, tuned) = tune::tune_targeted(root, &path, owner, *span, &unit, |_| {})?;
            Ok((base.differing, tuned.differing))
        });
        match &outcome {
            Ok((base, final_)) => println!("{owner} {span} {base} -> {final_}"),
            Err(error) => println!(
                "{owner} {span} failed {}",
                first_error(error)
                    .clone()
                    .unwrap_or_else(|| error.clone())
                    .trim()
            ),
        }
        outcome
    });
    let (mut exact, mut total_base, mut total_final) = (0, 0u64, 0u64);
    for (base, final_) in outcomes.iter().flatten() {
        total_base += u64::from(*base);
        total_final += u64::from(*final_);
        if *final_ == 0 {
            exact += 1;
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
        let mark = match (
            values.value.contains(&index),
            values.direct.contains(&index),
        ) {
            (true, true) => " ; value direct",
            (true, false) => " ; value",
            (false, true) => " ; direct",
            (false, false) => "",
        };
        println!("{index:5} {:08x} {}{mark}", x.addr, x.text);
    }
    Ok(())
}

fn adopt_owner(root: &Path, options: &Options) -> Result<(), String> {
    let owner = owner_argument(options)?;
    let request = lifter::adopt::Request {
        owner,
        span: options.span,
        name: options.name.as_deref(),
        path: options.path.as_deref(),
        source: options.source.as_deref(),
    };
    for line in lifter::adopt::adopt(root, &request)? {
        println!("{line}");
    }
    Ok(())
}

/// Prints every call site of an owner resolved to its real target, one JSON
/// object per line, for the humanizing passes that annotate the units.
fn imports_owner(root: &Path, options: &Options) -> Result<i32, String> {
    let owner = owner_argument(options)?;
    for import in lifter::imports::imports(root, owner, options.span)? {
        println!(
            "{}",
            serde_json::to_string(&import).map_err(|error| error.to_string())?
        );
    }
    Ok(0)
}
