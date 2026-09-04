use lifter::owners::{self, first_error, score, score_extending, Module, RegisteredOwner};
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
  batch [--kind WORD] [--limit N] [--all] [--owner-spans] [--sweep]\n\
      score every unregistered retained module (scene and script kinds)\n\
  list  [--kind WORD] [--all] [--owner-spans]\n\
      print the retained modules the batch would visit\n\
Candidates are written under out/lifter/.\n\n\
  --owner-spans   also score (list: also list) every registered owner\n\
                  (regions.json manual_regions) whose start lies inside the\n\
                  module, at that owner's own span, in addition to the\n\
                  module-extent score. Default behaviour is unchanged when\n\
                  this is absent. batch only.\n\
  --sweep         with --owner-spans on batch: also retry each owner-span\n\
                  score with the span widened to the rest of the module, and\n\
                  report the better of the two (a short registered span can\n\
                  cut a function that really runs to the next owner or the\n\
                  module end).";

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
    owner_spans: bool,
    sweep: bool,
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
        owner_spans: false,
        sweep: false,
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
            "--owner-spans" => options.owner_spans = true,
            "--sweep" => options.sweep = true,
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

fn score_owner(root: &Path, options: &Options) -> Result<i32, String> {
    let owner = owner_argument(options)?;
    let (unit, span) = lift_owner(root, owner, options.span, options.name.as_deref())?;
    let path = match &options.out {
        Some(path) => path.clone(),
        None => scratch_path(root, owner)?,
    };
    std::fs::write(&path, &unit).map_err(|error| format!("{}: {error}", path.display()))?;
    let result = score_extending(root, &path, owner, span)?;
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

/// The registered owners (from `regions.json`) whose start lies inside `m`.
fn owners_inside<'a>(m: &Module, registered: &'a [RegisteredOwner]) -> Vec<&'a RegisteredOwner> {
    registered
        .iter()
        .filter(|o| o.overlay == m.overlay && o.entry >= m.entry && o.entry < m.entry + m.span)
        .collect()
}

fn list(root: &Path, options: &Options) -> Result<(), String> {
    let registered = if options.owner_spans {
        owners::registered_owners(root)?
    } else {
        Vec::new()
    };
    for m in selected(root, options)? {
        println!("{} {} {}", m.key(), m.span, m.kind);
        for owner in owners_inside(&m, &registered) {
            println!("  {} span={}", owner.key(), owner.span);
        }
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

/// True when the reference is exactly one halfword longer than the candidate
/// and the scorer reports no wrong instruction: every instruction matches and
/// the registered span merely includes the pad word after the final return
/// that ELF `.size` drops. The raw count stays visible in the row; `adopt`
/// still decides on the real bytes.
fn trailing_pad(result: &owners::Score) -> bool {
    result.differing == 1
        && result.reference == result.candidate + 2
        && result.report.contains("wrong_instructions=0")
}

/// One span's score, with the trailing-pad tolerance already applied.
struct Scored {
    span: u32,
    raw: u32,
    ranked: u32,
    pad: bool,
}

fn describe(span: u32, result: &owners::Score) -> Scored {
    let pad = trailing_pad(result);
    Scored {
        span,
        raw: result.differing,
        ranked: if pad { 0 } else { result.differing },
        pad,
    }
}

/// Renders one `Scored` as `{prefix}differing_halfwords=N[ {prefix}trailing_pad=1
/// {prefix}raw_differing_halfwords=N]`, `prefix` distinguishing an owner-span
/// score from its `--sweep` retry on the same line.
fn format_scored(prefix: &str, s: &Scored) -> String {
    let mut out = format!("{prefix}differing_halfwords={}", s.ranked);
    if s.pad {
        out.push_str(&format!(
            " {prefix}trailing_pad=1 {prefix}raw_differing_halfwords={}",
            s.raw
        ));
    }
    out
}

/// One registered-owner span's batch result, ranking already applying the
/// trailing-pad tolerance.
enum OwnerOutcome {
    Scored { ranked: u32 },
    Failed,
}

/// Scores one registered owner against a module's draft at the owner's own
/// span (`--owner-spans`), and optionally again at the rest of the module
/// (`--sweep`) when the owner-span score is not exact — a short registered
/// span can cut a function that really runs to the next owner or the module
/// end (see HANDOFF's "reverse case": a pool-island split registered as two
/// owners).
fn owner_span_one(
    root: &Path,
    options: &Options,
    m: &Module,
    path: &Path,
    owner: &RegisteredOwner,
) -> OwnerOutcome {
    let owner_key = owner.key();
    let result = match score(root, path, &owner_key, owner.span) {
        Ok(result) => result,
        Err(error) => {
            let detail = first_error(&error).unwrap_or(error);
            println!("{owner_key} span={} failed {}", owner.span, detail.trim());
            return OwnerOutcome::Failed;
        }
    };
    let scored = describe(owner.span, &result);
    let module_note = format!("(module {:08x} extent {})", m.entry, m.span);
    let swept_span = m.entry + m.span - owner.entry;
    let sweep = options.sweep && scored.ranked > 0 && swept_span != owner.span;
    if !sweep {
        println!(
            "{owner_key} span={} {} {module_note}",
            owner.span,
            format_scored("", &scored)
        );
        return OwnerOutcome::Scored {
            ranked: scored.ranked,
        };
    }
    match score(root, path, &owner_key, swept_span) {
        Ok(swept_result) => {
            let swept = describe(swept_span, &swept_result);
            let best = if swept.ranked < scored.ranked {
                "sweep"
            } else {
                "owner"
            };
            println!(
                "{owner_key} span={} {} {module_note} sweep_span={} {} best={best}",
                scored.span,
                format_scored("", &scored),
                swept.span,
                format_scored("sweep_", &swept)
            );
            OwnerOutcome::Scored {
                ranked: scored.ranked.min(swept.ranked),
            }
        }
        Err(error) => {
            let detail = first_error(&error).unwrap_or(error);
            println!(
                "{owner_key} span={} {} {module_note} sweep_span={swept_span} sweep_failed {}",
                scored.span,
                format_scored("", &scored),
                detail.trim()
            );
            OwnerOutcome::Scored {
                ranked: scored.ranked,
            }
        }
    }
}

/// Runs the `--owner-spans` pass over every registered owner inside an
/// already-scored (not failed) module, printing one row per owner and
/// returning a summary tally.
fn owner_spans_pass(root: &Path, options: &Options, modules: &[Module], outcomes: &[Outcome]) {
    let registered = match owners::registered_owners(root) {
        Ok(registered) => registered,
        Err(error) => {
            println!("owner-spans failed {error}");
            return;
        }
    };
    let mut jobs_list: Vec<(Module, RegisteredOwner)> = Vec::new();
    for (m, outcome) in modules.iter().zip(outcomes) {
        if matches!(outcome, Outcome::Failed) {
            continue;
        }
        for owner in owners_inside(m, &registered) {
            jobs_list.push((m.clone(), owner.clone()));
        }
    }
    if jobs_list.is_empty() {
        return;
    }
    let results = parallel(&jobs_list, jobs(options), |(m, owner)| {
        match scratch_path(root, &m.key()) {
            Ok(path) => owner_span_one(root, options, m, &path, owner),
            Err(error) => {
                println!("{} span={} failed {error}", owner.key(), owner.span);
                OwnerOutcome::Failed
            }
        }
    });
    let (mut owner_exact, mut owner_differing, mut owner_failed) = (0, 0, 0);
    for result in &results {
        match result {
            OwnerOutcome::Scored { ranked: 0 } => owner_exact += 1,
            OwnerOutcome::Scored { .. } => owner_differing += 1,
            OwnerOutcome::Failed => owner_failed += 1,
        }
    }
    println!(
        "owner_spans={} owner_exact={owner_exact} owner_differing={owner_differing} owner_failed={owner_failed}",
        jobs_list.len()
    );
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
    if options.owner_spans {
        owner_spans_pass(root, options, &modules, &outcomes);
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
