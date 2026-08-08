//! `main()` -- select drafts, try each `M2C_UNK` substitution, score the
//! result, and write the canonical report.

use std::path::{Path, PathBuf};

use alchemy_routing::routing::{root, CompilerTarget};
use decomp_targets::{target_for, DecompCompilerTarget, DecompTarget, DecompTargetId, DEFAULT_TARGET};

use crate::cli::Options;
use crate::json::{canonical_json, Json};
use crate::jsnum::{is_strictly_better, math_abs, math_max, parse_hex};
use crate::jsstring::utf16_cmp;
use crate::verify::{
    source_stem, verify_candidate, CandidateCompilerConfiguration, M2C_PREAMBLE, ROM_BASE,
};

/// The four markers that disqualify a draft outright.
///
/// A `Vec`, not a `Set`: it is iterated with `.some`, order is the short-circuit
/// order, and a `Set` would silently collapse a duplicate if one were ever
/// added. Four routing tables in this repository were recently found carrying
/// exactly that.
pub const REJECT: [&str; 4] = ["M2C_ERROR", "M2C_BITFIELD", "M2C_MEMSET", "M2C_MEMCPY"];

/// The `M2C_UNK` substitutions, in the order the TypeScript tries them. The
/// order is the tiebreak: `compareTuple(...) < 0` is strict, so the FIRST
/// strategy to reach a given score keeps it.
pub const UNKNOWN_TYPE_STRATEGIES: [&str; 8] =
    ["void", "s32", "u32", "void *", "s16", "u16", "s8", "u8"];

/// `spans`, a `Map<number, number>`.
///
/// NOT a `HashMap`. `Map` keys use SameValueZero, so `-0` and `0` are the same
/// key and `NaN` is a key equal to itself -- neither of which `f64` as a Rust
/// hash key gives you for free. Lookups are linear over a few hundred rows,
/// which is nothing against five process spawns per candidate.
#[derive(Debug, Default)]
pub struct SpanMap {
    entries: Vec<(f64, f64)>,
}

impl SpanMap {
    fn same_value_zero(left: f64, right: f64) -> bool {
        (left.is_nan() && right.is_nan()) || left == right
    }

    pub fn set(&mut self, key: f64, value: f64) {
        match self
            .entries
            .iter_mut()
            .find(|(existing, _)| Self::same_value_zero(*existing, key))
        {
            Some(slot) => slot.1 = value,
            None => self.entries.push((key, value)),
        }
    }

    /// `spans.get(entry) ?? 0`.
    ///
    /// PORT NOTE -- `??` only substitutes for `undefined`/`null`, so a stored
    /// `NaN` is returned AS `NaN` rather than becoming 0. That is how a
    /// `report.json` row with a missing or non-numeric `size` poisons the score.
    pub fn get_or_zero(&self, key: f64) -> f64 {
        self.entries
            .iter()
            .find(|(existing, _)| Self::same_value_zero(*existing, key))
            .map(|(_, value)| *value)
            .unwrap_or(0.0)
    }

    pub fn len(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }
}

/// Load `<drafts>/report.json` into a [`SpanMap`], or an empty map when the
/// file does not exist.
pub fn load_spans(draft_report: &Path) -> Result<SpanMap, String> {
    let mut spans = SpanMap::default();
    if !draft_report.exists() {
        return Ok(spans);
    }
    let text = read_utf8(draft_report)?;
    let rows = crate::json::parse(&text)?;
    let Json::Array(rows) = rows else {
        // `for (const row of rows)` on a non-iterable throws in JavaScript.
        return Err("rows is not iterable".to_string());
    };
    for row in &rows {
        spans.set(
            Json::to_js_number(row.get("entry")),
            Json::to_js_number(row.get("size")),
        );
    }
    Ok(spans)
}

/// `readFileSync(path, "utf8")`, which is lossy on invalid UTF-8 exactly as
/// `String::from_utf8_lossy` is.
pub fn read_utf8(path: &Path) -> Result<String, String> {
    let bytes = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    Ok(String::from_utf8_lossy(&bytes).into_owned())
}

fn read_dir_names(path: &Path) -> Result<Vec<String>, String> {
    let mut names = Vec::new();
    for entry in std::fs::read_dir(path).map_err(|e| format!("{}: {e}", path.display()))? {
        let entry = entry.map_err(|e| format!("{}: {e}", path.display()))?;
        names.push(entry.file_name().to_string_lossy().into_owned());
    }
    // `readdirSync` order is filesystem order and is not relied on: `tracked`
    // is a membership set and the draft list is sorted below. Sorting here
    // makes the port deterministic without changing any observable.
    names.sort_by(|a, b| utf16_cmp(a, b));
    Ok(names)
}

/// The per-draft outcome, in the key order the TypeScript object literals use.
#[derive(Debug, Clone)]
pub enum DraftOutcome {
    Matched {
        entry: f64,
        size: f64,
        source: String,
        unknown_type: Option<String>,
    },
    NoCandidate {
        entry: f64,
    },
    Best {
        entry: f64,
        size: f64,
        mismatched_bytes: f64,
        common_prefix: f64,
        unknown_type: Option<String>,
    },
}

impl DraftOutcome {
    pub fn matched(&self) -> bool {
        matches!(self, DraftOutcome::Matched { .. })
    }

    pub fn to_json(&self) -> Json {
        let unknown = |value: &Option<String>| match value {
            Some(text) => Json::String(text.clone()),
            None => Json::Null,
        };
        match self {
            // KEY ORDER IS THE OUTPUT FORMAT. These match the object literals
            // in `main()` exactly.
            DraftOutcome::Matched { entry, size, source, unknown_type } => Json::Object(vec![
                ("entry".into(), Json::Number(*entry)),
                ("matched".into(), Json::Bool(true)),
                ("size".into(), Json::Number(*size)),
                ("source".into(), Json::String(source.clone())),
                ("unknown_type".into(), unknown(unknown_type)),
            ]),
            DraftOutcome::NoCandidate { entry } => Json::Object(vec![
                ("entry".into(), Json::Number(*entry)),
                ("matched".into(), Json::Bool(false)),
            ]),
            DraftOutcome::Best {
                entry,
                size,
                mismatched_bytes,
                common_prefix,
                unknown_type,
            } => Json::Object(vec![
                ("entry".into(), Json::Number(*entry)),
                ("matched".into(), Json::Bool(false)),
                ("size".into(), Json::Number(*size)),
                ("mismatched_bytes".into(), Json::Number(*mismatched_bytes)),
                ("common_prefix".into(), Json::Number(*common_prefix)),
                ("unknown_type".into(), unknown(unknown_type)),
            ]),
        }
    }
}

/// Everything `main()` computes before the per-draft work starts.
pub struct Plan {
    pub target: DecompTarget,
    pub candidate_directory: PathBuf,
    pub output_directory: PathBuf,
    pub report_path: PathBuf,
    pub drafts: Vec<String>,
    pub spans: SpanMap,
    pub rom: Vec<u8>,
}

fn compiler_target(target: DecompCompilerTarget) -> CompilerTarget {
    match target {
        DecompCompilerTarget::Gs1 => CompilerTarget::Gs1,
        DecompCompilerTarget::Gs2 => CompilerTarget::Gs2,
    }
}

pub fn candidate_directory(root: &Path, id: DecompTargetId) -> PathBuf {
    if id == DEFAULT_TARGET {
        root.join("work/matches/m2c")
    } else {
        root.join("work/matches").join(id.as_str()).join("m2c")
    }
}

pub fn output_directory(root: &Path, target: &DecompTarget) -> PathBuf {
    if target.id == DEFAULT_TARGET {
        root.join("out/m2c")
    } else {
        root.join(target.output_dir).join("m2c")
    }
}

pub fn report_path(root: &Path, id: DecompTargetId) -> PathBuf {
    if id == DEFAULT_TARGET {
        root.join("work/matches/m2c.json")
    } else {
        root.join("work/matches").join(format!("{}-m2c.json", id.as_str()))
    }
}

/// Draft selection: `.c` files, sorted, minus anything already tracked, minus
/// anything containing a reject marker.
///
/// PORT NOTE -- the `.sort()` is a bare `Array#sort`, which orders by UTF-16
/// code unit. It runs BEFORE the `tracked` filter and before the reject filter,
/// so the surviving order is the sorted order regardless.
pub fn select_drafts(
    drafts_directory: &Path,
    tracked: &[String],
) -> Result<Vec<String>, String> {
    let mut names: Vec<String> = read_dir_names(drafts_directory)?
        .into_iter()
        .filter(|name| name.ends_with(".c"))
        .collect();
    names.sort_by(|a, b| utf16_cmp(a, b));
    let mut selected = Vec::new();
    for name in names {
        // `tracked.has(name)`. A `Vec` + `contains` rather than a `HashSet`:
        // the membership answer is the same, and the insertion order stays
        // inspectable if this ever needs to be printed.
        if tracked.contains(&name) {
            continue;
        }
        let path = drafts_directory.join(&name);
        let source = read_utf8(&path)?;
        if REJECT.iter().any(|marker| source.contains(marker)) {
            continue;
        }
        selected.push(path.to_string_lossy().into_owned());
    }
    Ok(selected)
}

pub fn prepare(options: &Options) -> Result<Plan, String> {
    let repository_root = root();
    let target = target_for(options.target);
    let rom = std::fs::read(&options.rom).map_err(|e| format!("{}: {e}", options.rom))?;
    if rom.len() as u64 != target.rom_size {
        return Err(format!(
            "{} ROM must contain exactly {} bytes",
            target.id.as_str(),
            target.rom_size
        ));
    }
    let candidate_directory = candidate_directory(repository_root, target.id);
    let output_directory = output_directory(repository_root, &target);
    std::fs::create_dir_all(&candidate_directory).map_err(|e| e.to_string())?;
    std::fs::create_dir_all(&output_directory).map_err(|e| e.to_string())?;
    let tracked: Vec<String> = read_dir_names(&repository_root.join(target.source_dir))?
        .into_iter()
        .filter(|name| name.ends_with(".c"))
        .collect();
    let spans = load_spans(&Path::new(&options.drafts).join("report.json"))?;
    let drafts = select_drafts(Path::new(&options.drafts), &tracked)?;
    Ok(Plan {
        target,
        candidate_directory,
        output_directory,
        report_path: report_path(repository_root, target.id),
        drafts,
        spans,
        rom,
    })
}

/// One draft: the body of the `parallelMap` callback.
pub fn process_draft(plan: &Plan, draft: &str) -> Result<DraftOutcome, String> {
    let name = alchemy_plan::nodepath::basename(draft).to_string();
    let stem = source_stem(draft);
    let entry = parse_hex(&stem)?;
    let candidate = plan
        .candidate_directory
        .join(&name)
        .to_string_lossy()
        .into_owned();
    let function_output = plan.output_directory.join(&stem);
    std::fs::create_dir_all(&function_output).map_err(|e| e.to_string())?;
    let function_output = function_output.to_string_lossy().into_owned();
    let source = read_utf8(Path::new(draft))?;

    // `source.includes("M2C_UNK")`. Note this also matches `M2C_UNKNOWN` and
    // any other identifier with that prefix; the substitution below is a plain
    // `replaceAll` on the same substring, so the two agree.
    let strategies: Vec<Option<&str>> = if source.contains("M2C_UNK") {
        UNKNOWN_TYPE_STRATEGIES.iter().map(|s| Some(*s)).collect()
    } else {
        vec![None]
    };

    struct Best<'a> {
        score: [f64; 3],
        replacement: Option<&'a str>,
        body: String,
        size: f64,
        prefix: f64,
    }
    let mut best: Option<Best> = None;

    for replacement in strategies {
        let body = match replacement {
            None => source.clone(),
            Some(text) => source.replace("M2C_UNK", text),
        };
        crate::verify::write(&candidate, format!("{M2C_PREAMBLE}{body}").as_bytes())?;
        let verification = match verify_candidate(
            &candidate,
            &plan.rom,
            &function_output,
            &[],
            ROM_BASE,
            compiler_target(plan.target.compiler),
            &CandidateCompilerConfiguration::default(),
        ) {
            Ok(verification) => verification,
            // `catch { continue }` -- EVERY failure is swallowed, including a
            // missing toolchain binary. Reproduced; the parity harness fails
            // hard on an empty corpus so that a silently non-compiling
            // environment cannot read as a clean sweep.
            Err(_) => continue,
        };
        let actual = &verification.actual;
        let expected = &verification.expected;
        let size = verification.size;

        let mut mismatch = 0.0f64;
        let common = std::cmp::min(actual.len(), expected.len());
        for index in 0..common {
            if actual[index] != expected[index] {
                mismatch += 1.0;
            }
        }
        let span = plan.spans.get_or_zero(entry);
        // `Math.max`, not `f64::max`: a NaN span must propagate.
        mismatch += math_max(0.0, span - actual.len() as f64);
        let mut prefix = 0usize;
        while prefix < common && actual[prefix] == expected[prefix] {
            prefix += 1;
        }
        let prefix = prefix as f64;
        let score = [mismatch, -prefix, math_abs(size - span)];
        // `best === null || compareTuple(score, best.score) < 0`. A poisoned
        // (NaN) score never displaces the incumbent, because `NaN < 0` is false.
        let replaces = match &best {
            None => true,
            Some(current) => is_strictly_better(score, current.score),
        };
        if replaces {
            best = Some(Best { score, replacement, body: body.clone(), size, prefix });
        }
        if actual == expected {
            return Ok(DraftOutcome::Matched {
                entry,
                size,
                source: candidate,
                unknown_type: replacement.map(str::to_string),
            });
        }
    }

    let Some(best) = best else {
        return Ok(DraftOutcome::NoCandidate { entry });
    };
    crate::verify::write(&candidate, format!("{M2C_PREAMBLE}{}", best.body).as_bytes())?;
    Ok(DraftOutcome::Best {
        entry,
        size: best.size,
        mismatched_bytes: best.score[0],
        common_prefix: best.prefix,
        unknown_type: best.replacement.map(str::to_string),
    })
}

/// `parallelMap` + the report write + the summary line.
pub fn run_pipeline(options: &Options) -> Result<String, String> {
    let plan = prepare(options)?;
    let results = parallel_map(&plan, options.jobs)?;
    let document = Json::Array(results.iter().map(DraftOutcome::to_json).collect());
    let text = format!("{}\n", canonical_json(&document)?);
    std::fs::write(&plan.report_path, text)
        .map_err(|e| format!("{}: {e}", plan.report_path.display()))?;
    let matches = results.iter().filter(|item| item.matched()).count();
    Ok(format!(
        "candidates={} matches={matches} failures={}",
        results.len(),
        results.len() - matches
    ))
}

/// `parallelMap(items, jobs, operation)`: results indexed by input position, so
/// the report order is the draft order regardless of completion order.
pub fn parallel_map(plan: &Plan, jobs: usize) -> Result<Vec<DraftOutcome>, String> {
    let workers = std::cmp::min(jobs, plan.drafts.len());
    if workers == 0 {
        return Ok(Vec::new());
    }
    let next = std::sync::atomic::AtomicUsize::new(0);
    let slots: Vec<std::sync::Mutex<Option<Result<DraftOutcome, String>>>> =
        (0..plan.drafts.len()).map(|_| std::sync::Mutex::new(None)).collect();
    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| loop {
                let index = next.fetch_add(1, std::sync::atomic::Ordering::SeqCst);
                if index >= plan.drafts.len() {
                    return;
                }
                let outcome = process_draft(plan, &plan.drafts[index]);
                *slots[index].lock().expect("slot poisoned") = Some(outcome);
            });
        }
    });
    let mut results = Vec::with_capacity(slots.len());
    for slot in slots {
        match slot.into_inner().expect("slot poisoned") {
            Some(Ok(outcome)) => results.push(outcome),
            Some(Err(error)) => return Err(error),
            None => return Err("worker did not fill its slot".to_string()),
        }
    }
    Ok(results)
}
