//! alchemy headers: one declaration per function, in the header of the C
//! file that defines it.

mod expand;
mod generate;
mod model;
mod objects;
mod plan;
mod scan;
mod settle;

use crate::compiler::preprocess::Expansion;
use crate::compiler::routing::root;
use model::Model;
use plan::{Plan, View, Views};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Mutex;

const USAGE: &str = "usage: alchemy headers apply [--dry-run] [--report FILE]\n\
  apply  write games/tbs/include/unit/<unit>.h for every ☀️ C file: the\n\
         prototype of each function it defines for other code. Take those\n\
         functions' prototypes and their function-shape cast macros out of\n\
         ☀️'s C files and headers, and make each file include the unit\n\
         headers of what it calls. Put a shared header's prototype that\n\
         words a ☀️ definition differently under #if GAME_TLA. Then, unit\n\
         by unit, compile in the six editions: a call that saw another\n\
         declaration than the header's is cast to the one it saw, under a\n\
         FAKEMATCH tag, and every cast the unit's bytes do not need is\n\
         taken out again. --report writes one row per unit: the casts\n\
         kept and dropped, and each function that differs from before.";

fn write(root: &Path, file: &str, text: &str) -> Result<(), String> {
    let path = root.join(file);
    if let Some(parent) = path.parent() {
        std::fs::create_dir_all(parent).map_err(|e| format!("{file}: {e}"))?;
    }
    std::fs::write(&path, text).map_err(|e| format!("{file}: {e}"))
}

/// Each item's result, worked by as many threads as the machine has, each
/// with its own state; the largest items go first.
fn parallel<T: Sync, S, R: Send>(items: &[T], state: impl Fn() -> Result<S, String> + Sync, work: impl Fn(&S, &T) -> R + Sync) -> Result<Vec<R>, String> {
    let next = AtomicUsize::new(0);
    let found: Mutex<Vec<(usize, R)>> = Mutex::new(Vec::new());
    let failed: Mutex<Option<String>> = Mutex::new(None);
    let workers = std::thread::available_parallelism().map_or(1, |n| n.get()).min(items.len().max(1));
    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| {
                let state = match state() {
                    Ok(state) => state,
                    Err(error) => {
                        *failed.lock().expect("failed") = Some(error);
                        return;
                    }
                };
                loop {
                    let index = next.fetch_add(1, Ordering::Relaxed);
                    let Some(item) = items.get(index) else {
                        break;
                    };
                    let result = work(&state, item);
                    found.lock().expect("found").push((index, result));
                }
            });
        }
    });
    if let Some(error) = failed.into_inner().expect("failed") {
        return Err(error);
    }
    let mut found = found.into_inner().expect("found");
    found.sort_by_key(|(index, _)| *index);
    Ok(found.into_iter().map(|(_, result)| result).collect())
}

/// What every edition's expansion of every unit says of the wanted names.
fn views(root: &Path, units: &[String], sources: &BTreeMap<String, expand::Source>, wanted: &BTreeSet<&str>) -> Result<Views, String> {
    let prefix = root.to_string_lossy().into_owned();
    let wanted = |name: &str| wanted.contains(name);
    let state = || objects::editions().iter().map(|target| Expansion::new(root, *target)).collect::<Result<Vec<_>, _>>();
    let found = parallel(units, state, |expansions, unit| -> Result<Vec<View>, String> {
        expansions
            .iter()
            .map(|expansion| {
                let text = expansion.code(unit)?;
                let expanded = expand::read(&prefix, &text, unit, &wanted);
                let (seen, strays) = expand::pair(sources, &expanded);
                Ok(View { defined: expanded.defined, named: expanded.named, seen, strays })
            })
            .collect()
    })?;
    units.iter().zip(found).map(|(unit, views)| Ok((unit.clone(), views?))).collect()
}

fn apply(root: &Path, dry: bool, report: Option<&str>) -> Result<(), String> {
    let model = Model::read(root)?;
    let definitions = model.definitions();
    let wanted: BTreeSet<&str> = definitions.keys().copied().collect();
    let sources: BTreeMap<String, expand::Source> = model
        .units
        .iter()
        .chain(&model.fragments)
        .chain(&model.headers)
        .filter(|file| file.starts_with("games/tbs/"))
        .map(|file| (file.clone(), expand::Source::new(&model.text[file], &|name| wanted.contains(name))))
        .collect();
    // The largest units first: they take the longest.
    let mut units = model.units.clone();
    units.sort_by_key(|unit| std::cmp::Reverse(model.text[unit].len()));
    let views = views(root, &units, &sources, &wanted)?;
    let plan = Plan::new(&model, &sources, &views);
    for problem in &plan.problems {
        println!("problem: {problem}");
    }
    let casts: usize = plan.drafts.values().map(|draft| draft.casts.len()).sum();
    println!(
        "{} headers and {} files {}: {} declarations and macros out, {} calls cast to the declaration they saw, {} shared prototypes left to the other game",
        plan.headers.len(),
        plan.drafts.len(),
        if dry { "would change" } else { "change" },
        plan.removed,
        casts,
        plan.guarded
    );
    if dry {
        for (file, draft) in &plan.drafts {
            for cast in &draft.casts {
                println!("cast: {file}:{}: {} in {} as {} not {}", sources[file].line(cast.at), cast.name, cast.function, cast.old, cast.truth);
            }
        }
        return Ok(());
    }
    let before = parallel(&units, || objects::Bench::new(root), |bench, unit| bench.build(root, unit, &model.text[unit]))?;
    for (file, text) in &plan.headers {
        write(root, file, text)?;
    }
    let empty = plan::Draft::default();
    let all = |draft: &plan::Draft| -> BTreeMap<usize, String> { (0..draft.casts.len()).map(|index| (index, "this is not measured yet".to_string())).collect() };
    // A unit on the ARM route is written with every cast, like a shared
    // file: only the whole build compiles it.
    for (file, draft) in &plan.drafts {
        if !model.units.contains(file) || model.arm.contains(file) {
            write(root, file, &settle::render(&model.text[file], draft, &all(draft)))?;
        }
    }
    let jobs: Vec<(&String, &objects::Built)> = units.iter().zip(&before).collect();
    let outcomes = parallel(&jobs, || objects::Bench::new(root), |bench, (unit, before)| {
        if model.arm.contains(*unit) {
            let draft = plan.drafts.get(*unit).unwrap_or(&empty);
            return (settle::Outcome { kept: draft.casts.len(), ..settle::Outcome::default() }, Ok(()));
        }
        let outcome = settle::settle(root, bench, unit, &model.text[*unit], before, plan.drafts.get(*unit).unwrap_or(&empty));
        let written = if outcome.text == model.text[*unit] { Ok(()) } else { write(root, unit, &outcome.text) };
        (outcome, written)
    })?;
    // A shared file's casts answer for every unit that compiles it: all
    // of them are tried out at once, then one at a time.
    let bench = objects::Bench::new(root)?;
    let mut bad: BTreeMap<&str, BTreeSet<String>> = BTreeMap::new();
    let mut was: BTreeMap<&str, &objects::Built> = BTreeMap::new();
    for ((unit, before), (outcome, _)) in jobs.iter().zip(&outcomes) {
        bad.insert(unit, outcome.differing.iter().map(|(name, _)| name.clone()).collect());
        was.insert(unit, before);
    }
    for (file, draft) in &plan.drafts {
        if model.units.contains(file) || draft.casts.is_empty() {
            continue;
        }
        let users = model.users(file);
        // The part of some unit that a text of the file changes, if any.
        let changed = |kept: &BTreeMap<usize, String>| -> Result<Option<String>, String> {
            write(root, file, &settle::render(&model.text[file], draft, kept))?;
            for user in &users {
                let text = std::fs::read_to_string(root.join(user)).map_err(|e| format!("{user}: {e}"))?;
                let now = bench.build(root, user, &text);
                if let Some(part) = objects::differing(was[user], &now).difference(&bad[user]).next() {
                    return Ok(Some(match objects::difference(was[user], &now, part) {
                        Some(how) => format!("{part} {how}"),
                        None => format!("{user} does not compile"),
                    }));
                }
            }
            Ok(None)
        };
        let mut kept: BTreeMap<usize, String> = all(draft);
        if changed(&BTreeMap::new())?.is_none() {
            kept.clear();
        } else {
            for index in 0..draft.casts.len() {
                let mut without = kept.clone();
                without.remove(&index);
                match changed(&without)? {
                    None => kept = without,
                    Some(measured) => {
                        kept.insert(index, measured);
                    }
                }
            }
        }
        println!("{file}: {} of {} casts kept", kept.len(), draft.casts.len());
        write(root, file, &settle::render(&model.text[file], draft, &kept))?;
    }
    let mut rows = String::from("unit\tstate\tcasts kept\tcasts dropped\tbuilds\tdiffers\n");
    let (mut same, mut kept, mut dropped) = (0, 0, 0);
    for ((unit, before), (outcome, written)) in jobs.iter().zip(outcomes) {
        written?;
        let state = if model.arm.contains(*unit) || before.iter().all(|built| built.is_err()) {
            "unmeasured"
        } else if outcome.differing.is_empty() {
            same += 1;
            "same"
        } else if outcome.differing.iter().any(|(name, _)| name == "compile") {
            "broken"
        } else {
            "differs"
        };
        kept += outcome.kept;
        dropped += outcome.dropped;
        let differs: Vec<String> = outcome.differing.iter().map(|(name, how)| format!("{name} {how}")).collect();
        rows.push_str(&format!("{unit}\t{state}\t{}\t{}\t{}\t{}\n", outcome.kept, outcome.dropped, outcome.builds, differs.join("; ")));
    }
    match report {
        Some(path) => std::fs::write(path, &rows).map_err(|e| format!("{path}: {e}"))?,
        None => print!("{rows}"),
    }
    println!("{same} of {} units give the same objects; {kept} casts kept, {dropped} dropped", units.len());
    Ok(())
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    match arguments.first().map(String::as_str) {
        Some("apply") => {
            let report = arguments.iter().position(|a| a == "--report").and_then(|at| arguments.get(at + 1)).map(String::as_str);
            apply(root(), arguments.iter().any(|a| a == "--dry-run"), report)
        }
        // What one unit's expansions say of one name: for looking into a
        // call the pass judged otherwise than the compiler does.
        Some("sights") => {
            let (unit, name) = (arguments.get(1).ok_or(USAGE)?, arguments.get(2).ok_or(USAGE)?);
            let prefix = root().to_string_lossy().into_owned();
            for target in objects::editions() {
                let text = Expansion::new(root(), target)?.code(unit)?;
                if let Ok(path) = std::env::var("ALCHEMY_HEADERS_EXPANSION") {
                    std::fs::write(format!("{path}.{}", target.id), &text).map_err(|e| e.to_string())?;
                }
                let expanded = expand::read(&prefix, &text, unit, &|word| word == name);
                println!("{}: defined {:?}, static {}", target.id, expanded.defined.get(name), expanded.hidden.contains(name));
                for sight in &expanded.sights {
                    println!("  {}:{} in {} ({}:{}): {:?} sees {:?}", sight.file, sight.line, sight.function, sight.open.0, sight.open.1, sight.how, sight.seen);
                }
            }
            Ok(())
        }
        _ => Err(USAGE.into()),
    }
}
