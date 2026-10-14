//! alchemy headers: one declaration per function, in the header of the C
//! file that defines it; none for a function the bytes show was called
//! before its definition with no prototype in scope.

mod events;
pub(crate) mod expand;
pub(crate) mod generate;
mod internal;
pub(crate) mod lint;
mod narrow;
pub(crate) mod model;
pub(crate) mod objects;
pub(crate) mod plan;
pub(crate) mod scan;
mod settle;
pub(crate) mod undeclared;

use crate::compiler::preprocess::Expansion;
use crate::compiler::routing::root;
use model::Model;
use plan::{Plan, View, Views};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Mutex;

const USAGE: &str = "usage: alchemy headers apply [--dry-run] [--functions] [--leave FILE] [--report FILE]\n\
  apply  write games/tbs/include/unit/<unit>.h for every ☀️ C file: the\n\
         prototype of each function it defines for other code (a function\n\
         the header leaves out, under its comment that the function is\n\
         called before its definition with no prototype in scope, stays\n\
         out: see undeclared). Take those\n\
         functions' prototypes and their function-shape cast macros out of\n\
         ☀️'s C files and headers, and make each file include the unit\n\
         headers of what it calls. Put a shared header's prototype that\n\
         words a ☀️ definition differently under #if GAME_TLA. Then, unit\n\
         by unit, compile in the six editions: a call that saw another\n\
         declaration than the header's is cast to the one it saw, under a\n\
         FAKEMATCH tag, and every cast the unit's bytes do not need is\n\
         taken out again. --report writes one row per unit: the casts\n\
         kept and dropped, and each function that differs from before.\n\
         --functions leaves every object for a later run. --leave FILE\n\
         names objects, one a line, to leave declared as they are: a run\n\
         prints `leave: NAME` for each object a still differing unit views.\n\
       alchemy headers units save|same FILE\n\
  units  save writes every Thumb unit's objects in the six editions to\n\
         FILE, part by part; same builds them again and prints each part\n\
         that differs from FILE: the trial of an edit by hand.\n\
       alchemy headers undeclared [--dry-run] [--report FILE] [FILE.c]...\n\
  undeclared  a call with no prototype in scope is C89's implicit\n\
         `int name()`, and some callers match only as that: a function's\n\
         callers above its definition, written as a cast of the function\n\
         or through a ValueN or CallN helper, each a device. For\n\
         each such function no other file names, try what the source had:\n\
         the function left out of its file's header, a comment in the\n\
         prototype's place saying it is called before its definition with\n\
         no prototype in scope, and those callers plain calls. Where every\n\
         unit that compiles the file gives the same objects in the six\n\
         editions, write that, and take out each tag that stood on those\n\
         calls alone. A function already left out whose callers match with\n\
         its prototype in scope is declared again. Prints one row per\n\
         function: its file, its calls above the definition under another\n\
         shape, its plain calls there, and the verdict. FILE.c limits the\n\
         pass to the functions those files define; --dry-run writes\n\
         nothing (the trials read the tree and compile in a copy).\n\
       alchemy headers static [--dry-run] [--leave FILE] [--report FILE]\n\
  static  an overlay is one C file beside its stub tables, and a function\n\
         of it that no other object of the overlay names is `static`: the\n\
         entry table's functions and whatever an assembly file names stay\n\
         as they are, and so does a function called before its definition\n\
         with no prototype in scope, one a header beside its own names,\n\
         and one of a shared C file that some unit names above the line\n\
         that includes the file. The file's header loses the prototype;\n\
         where the file names the function above its definition, the\n\
         prototype stands once, `static`, in one block after the file's\n\
         includes, in definition order, with the struct tags the block and\n\
         the static heads point to and no header declares. A header left\n\
         with nothing goes, with the lines that include it. Each overlay\n\
         unit is compiled before and after in the six editions: `static`\n\
         makes a symbol local and changes nothing else, and a unit whose\n\
         objects differ otherwise is left as it was, with the first part\n\
         that differs printed. --leave FILE names C files, one a line, to\n\
         leave as they are; --report writes one row per function.\n\
       alchemy headers events [--dry-run] [--report FILE] [FILE.c]...\n\
  events  an overlay's C file holds its scene's event tables, and a row\n\
         names the function it runs under a trigger kind and an id. A\n\
         static function of the file that every row reaches under one id\n\
         is named EV<id>_<WORD> (S3): the id in decimal as the table gives\n\
         it, and the subject the function has when it is an EV name\n\
         already, else the kind its rows spell (EVENT_TOUCH, or a macro\n\
         that builds its control from it, gives TOUCH), else SCRIPT where\n\
         the kind is a bare number or the rows spell several. Two\n\
         functions of one id and kind are told apart by the ordinal the\n\
         name of the first table that reaches each carries (..Events2\n\
         gives _2), and keep their names where that tells nothing. A\n\
         function reached under several ids keeps its name, and so does\n\
         one that is not static, one a shared C file defines, and one\n\
         another file of the unit names. The rows are read from the six\n\
         editions' preprocessed text; the name is written anew wherever\n\
         the file's code and comments spell it, and each unit is compiled\n\
         before and after: its objects are the same under the new names,\n\
         or the unit is left as it was. --report writes one row per\n\
         function the tables reach. FILE.c limits the pass to those\n\
         overlays' C files.";

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
                Ok(View { defined: expanded.defined, declared: expanded.declared, named: expanded.named, seen, strays })
            })
            .collect()
    })?;
    units.iter().zip(found).map(|(unit, views)| Ok((unit.clone(), views?))).collect()
}

fn apply(root: &Path, dry: bool, functions_only: bool, leave: BTreeSet<String>, report: Option<&str>) -> Result<(), String> {
    let model = Model { functions_only, leave, ..Model::read(root)? };
    // The names the pass follows: what the C files define for other code,
    // the far-call stubs, and whatever a file or header declares.
    let definitions = model.definitions();
    let mut wanted: BTreeSet<&str> = definitions.keys().copied().collect();
    wanted.extend(model.far.keys().map(String::as_str));
    wanted.extend(model.imports.values().flat_map(|veneers| veneers.keys().map(String::as_str)));
    for file in model.units.iter().chain(&model.fragments).chain(&model.headers) {
        wanted.extend(model.entries[file].iter().filter(|e| matches!(e.what, scan::What::Prototype | scan::What::Extern | scan::What::ShapeMacro { .. } | scan::What::ViewMacro)).map(|e| e.name.as_str()));
    }
    let inner: Vec<String> = model.units.iter().chain(&model.fragments).flat_map(|file| scan::bodies(&model.text[file], &|_| false)).flat_map(|body| body.locals).map(|local| local.name).collect();
    wanted.extend(inner.iter().map(String::as_str));
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
        "{} headers and {} files {}: {} declarations and macros out, {} uses cast to the declaration they saw, {} far-call entries typed from their functions, {} shared declarations left to the other game",
        plan.headers.len(),
        plan.drafts.len(),
        if dry { "would change" } else { "change" },
        plan.removed,
        casts,
        plan.retyped,
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
    // of them are tried out at once, then by halves.
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
            let found = parallel(&users, || objects::Bench::new(root), |bench, user| -> Result<Option<String>, String> {
                let text = std::fs::read_to_string(root.join(user)).map_err(|e| format!("{user}: {e}"))?;
                let now = bench.build(root, user, &text);
                Ok(objects::differing(was[user], &now).difference(&bad[user]).next().map(|part| match objects::difference(was[user], &now, part) {
                    Some(how) => format!("{part} {how}"),
                    None => format!("{user} does not compile"),
                }))
            })?;
            Ok(found.into_iter().collect::<Result<Vec<_>, _>>()?.into_iter().flatten().next())
        };
        let every: Vec<usize> = (0..draft.casts.len()).collect();
        let without = |gone: &[usize]| -> BTreeMap<usize, String> { all(draft).into_iter().filter(|(index, _)| !gone.contains(index)).collect() };
        let (gone, needed) = crate::keep::bisect(&every, |gone: &[usize]| changed(&without(gone)).map(|found| found.is_none()))?;
        let mut kept: BTreeMap<usize, String> = BTreeMap::new();
        for index in needed {
            let mut trial = gone.clone();
            trial.push(index);
            kept.insert(index, changed(&without(&trial))?.unwrap_or_else(|| "this is not measured yet".to_string()));
        }
        println!("{file}: {} of {} casts kept", kept.len(), draft.casts.len());
        write(root, file, &settle::render(&model.text[file], draft, &kept))?;
    }
    // A unit that still differs: the objects its differing functions view,
    // and the ones its macros name, are the candidates to leave as they are.
    let mut blamed: BTreeSet<&str> = BTreeSet::new();
    for ((unit, _), (outcome, _)) in jobs.iter().zip(&outcomes) {
        if outcome.differing.is_empty() || model.arm.contains(*unit) {
            continue;
        }
        let differing: BTreeSet<&str> = outcome.differing.iter().map(|(name, _)| name.as_str()).collect();
        for part in model.parts(unit) {
            for cast in plan.drafts.get(part).map(|draft| draft.casts.as_slice()).unwrap_or(&[]) {
                if cast.form == plan::Form::View && differing.contains(cast.function.as_str()) {
                    blamed.insert(&cast.name);
                }
            }
        }
        blamed.extend(plan.strays.get(*unit).into_iter().flatten().map(String::as_str));
    }
    for name in blamed {
        println!("leave: {name}");
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

/// Every unit's objects as rows, built from the tree as it stands.
fn unit_rows(root: &Path) -> Result<String, String> {
    let model = Model::read(root)?;
    let units: Vec<&String> = model.units.iter().filter(|unit| !model.arm.contains(*unit)).collect();
    let built = parallel(&units, || objects::Bench::new(root), |bench, unit| objects::rows(unit, &bench.build(root, unit, &model.text[*unit])))?;
    Ok(built.into_iter().flatten().map(|row| row + "\n").collect())
}

/// `save` writes every unit's objects to a file; `same` builds them again
/// and prints each part that is no longer what the file says.
fn units(root: &Path, what: &str, file: &str) -> Result<(), String> {
    let now = unit_rows(root)?;
    match what {
        "save" => std::fs::write(file, now).map_err(|e| format!("{file}: {e}")),
        "same" => {
            let before = std::fs::read_to_string(file).map_err(|e| format!("{file}: {e}"))?;
            let changed = objects::changed(&before, &now);
            for row in &changed {
                println!("{row}");
            }
            if changed.is_empty() {
                println!("every unit gives the objects of {file}");
                Ok(())
            } else {
                Err(format!("{} parts differ from {file}", changed.len()))
            }
        }
        _ => Err(USAGE.into()),
    }
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    match arguments.first().map(String::as_str) {
        Some("apply") => {
            let report = arguments.iter().position(|a| a == "--report").and_then(|at| arguments.get(at + 1)).map(String::as_str);
            let leave = match arguments.iter().position(|a| a == "--leave").and_then(|at| arguments.get(at + 1)) {
                Some(path) => std::fs::read_to_string(path).map_err(|e| format!("{path}: {e}"))?.lines().map(|line| line.trim().to_string()).filter(|line| !line.is_empty()).collect(),
                None => BTreeSet::new(),
            };
            apply(root(), arguments.iter().any(|a| a == "--dry-run"), arguments.iter().any(|a| a == "--functions"), leave, report)
        }
        Some("units") => units(root(), arguments.get(1).ok_or(USAGE)?, arguments.get(2).ok_or(USAGE)?),
        Some("events") => {
            let report = arguments.iter().position(|a| a == "--report").and_then(|at| arguments.get(at + 1));
            let files: Vec<String> = arguments[1..].iter().filter(|a| !a.starts_with("--") && Some(*a) != report).cloned().collect();
            events::run(root(), arguments.iter().any(|a| a == "--dry-run"), &files, report.map(String::as_str))
        }
        Some("narrow") => narrow::run(root(), arguments.get(1).is_some_and(|flag| flag == "--dry-run")),
        Some("static") => {
            let after = |flag: &str| arguments.iter().position(|a| a == flag).and_then(|at| arguments.get(at + 1));
            let leave = match after("--leave") {
                Some(path) => std::fs::read_to_string(path).map_err(|e| format!("{path}: {e}"))?.lines().map(|line| line.trim().to_string()).filter(|line| !line.is_empty()).collect(),
                None => BTreeSet::new(),
            };
            internal::run(root(), arguments.iter().any(|a| a == "--dry-run"), leave, after("--report").map(String::as_str))
        }
        Some("undeclared") => {
            let report = arguments.iter().position(|a| a == "--report").and_then(|at| arguments.get(at + 1));
            let files: Vec<String> = arguments[1..].iter().filter(|a| !a.starts_with("--") && Some(*a) != report).cloned().collect();
            undeclared::run(root(), arguments.iter().any(|a| a == "--dry-run"), &files, report.map(String::as_str))
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
