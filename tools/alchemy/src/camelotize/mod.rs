mod barriers;
mod casts;
mod commands;
mod declarations;
mod fields;
mod idioms;
mod numbers;
mod registers;
pub(crate) mod source;
mod stabs;
mod tagged;
pub(crate) mod trial;

use crate::compiler::routing::{family_for_source, root, CompilerFamily, COMMON_INCLUDE_MODULES};
use crate::targets::{target_for, DecompTarget, DecompTargetId};
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};

const USAGE: &str = r#"usage: alchemy camelotize [--dry-run] REWRITE FILE.c...
Applies Alchemy's mechanical source cleanup in place and prints how
many places changed in each file; --dry-run counts them and writes
nothing. REWRITE is one of:
  actors        byte offsets on field actors as struct FieldActor members
  events        byte offsets on the event work as struct EventWork members
  registers     hardware register addresses by their io_reg.h names
  declarations  a file's own declarations of functions a header declares,
                replaced by an include of that header
  barriers      FAKEMATCH-tagged empty asm barriers, with their tag
  layer         ☀️ overlay calls of a routine a layer header's inline forwards
                to, through that inline (field_event.h, field_service.h and
                scene/runpa_jo.h for scenes; engcall.h and iwram_call.h for
                every overlay): the one an event.h command expands to, else
                the outermost, else the first; an include it needs goes with
                the first includes
  commands      ☀️ scene calls an event.h command expands to, as the command,
                with fixed-point coordinates written in pixels where the
                argument is a whole-pixel literal, PIXELS(e) or e << 16;
                event.h goes with the first includes. layer and commands are
                proved like casts, one function's calls together first, in
                every C file that includes the file when it is a header, and
                leave asm and FAKEMATCH-tagged code as numbers, casts and
                idioms do
  numbers       hex literals below 0x1000 in decimal, only where the code
                shows a quantity: an argument of an event.h scene command, a
                loop bound compared with its counter, or plain arithmetic
                with no bit operation, register, fixed address or subscript
                in the statement or on its variables, beside operands every
                edition types as numbers; case labels, stores, subscripts,
                byte offsets, pointer offsets, character codes, angles,
                register words, flags, masks, packed pairs and everything
                from 0x1000 up stay hex, and one quantity keeps one base in
                a function
  casts         value casts and pointer casts the compiler does not need:
                each removal is compiled and assembled in every edition of
                the game, and kept only when the object's bytes,
                relocations and symbols are identical and no warning is new;
                a pointer cast of an address taken on the spot, of a
                function or in pointer arithmetic stays, and *(T *)&x folds
                to x only when x is declared T
  idioms        x = x + 1 as x++, x = x | n as x |= n, !(a == b) as
                a != b, parentheses around one name, number or call, an
                else after a block that returns, flattened, and empty
                statements; each proved the same way as casts
numbers, casts and idioms leave every statement holding asm and Nintendo's
library files untouched, and FAKEMATCH-tagged code: the whole block that
holds a tag, with the statement that owns it, or the whole function when the
tag is in its top-level block.
Struct layouts and types come from the build's own compiler for each game
the file builds in. Then run a bare
`alchemy keep`, with no paths: it keeps only the files whose edits leave all
twelve editions byte-identical (a path given to keep is held, not checked)."#;

fn games(file: &Path) -> Vec<DecompTarget> {
    let text = file.to_string_lossy();
    let mut found = Vec::new();
    if !text.contains("games/tla/") {
        found.push(target_for(DecompTargetId::TbsJa));
    }
    if !text.contains("games/tbs/") {
        found.push(target_for(DecompTargetId::TlaJa));
    }
    found
}

/// Whether the file is Nintendo's library code (K1's library routes): not
/// Camelot's, so never camelotized.
fn library(root: &Path, file: &Path) -> bool {
    let Ok(relative) = file.strip_prefix(root) else {
        return false;
    };
    let relative = relative.to_string_lossy();
    games(file).iter().any(|target| family_for_source(target.compiler, &relative) != CompilerFamily::Game)
}

fn search(root: &Path, target: DecompTarget) -> Vec<PathBuf> {
    let mut paths = vec![root.join("games").join(target.game_dir()).join("include")];
    paths.extend(
        COMMON_INCLUDE_MODULES
            .iter()
            .map(|module| root.join("games/common/include").join(module)),
    );
    paths
}

fn header_words(file: &Path, search: &[PathBuf]) -> (HashSet<String>, String) {
    let mut words = HashSet::new();
    let mut texts = String::new();
    for header in source::reachable(file, search) {
        if header == file {
            continue;
        }
        let Ok(text) = std::fs::read_to_string(&header) else {
            continue;
        };
        for word in text.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_')) {
            if !word.is_empty() {
                words.insert(word.to_string());
            }
        }
        texts.push_str(&text);
        texts.push('\n');
    }
    (words, texts)
}

fn unit(root: &Path, file: &Path, text: &str, spec: &fields::Spec) -> Result<Option<fields::Unit>, String> {
    let names: Vec<&str> = spec.getters.iter().chain(spec.globals).copied().collect();
    let probes: Vec<stabs::Probe> = names
        .iter()
        .enumerate()
        .map(|(i, name)| stabs::Probe {
            name: format!("alchemy_probe_{i}"),
            expression: if spec.getters.contains(name) {
                format!("{name}(0)")
            } else {
                (*name).to_string()
            },
        })
        .collect();
    let mut layouts = Vec::new();
    let mut agreed: Option<HashMap<String, stabs::Kind>> = None;
    let mut declared = HashSet::new();
    let mut header_text = String::new();
    for target in games(file) {
        let imports = crate::build_text::fresh_c_imports(root, target)?;
        let compiled = stabs::compile(target, imports.as_ref().map(|d| d.path()), file, &probes)?;
        let Some(members) = compiled.members(spec.tag) else {
            return Ok(None);
        };
        layouts.push(members);
        let types: HashMap<String, stabs::Kind> = names
            .iter()
            .enumerate()
            .filter_map(|(i, name)| Some((name.to_string(), compiled.typedef(&format!("alchemy_probe_{i}"))?)))
            .collect();
        agreed = Some(match agreed {
            None => types,
            Some(before) => before.into_iter().filter(|(k, v)| types.get(k) == Some(v)).collect(),
        });
        let (words, texts) = header_words(file, &search(root, target));
        declared.extend(words);
        header_text.push_str(&texts);
    }
    let macros = fields::macros(&format!("{header_text}\n{text}"));
    Ok(Some(fields::Unit { layouts, types: agreed.unwrap_or_default(), declared, macros }))
}

fn registers_in(root: &Path, file: &Path, text: &str) -> Result<(String, usize), String> {
    let mut all = Vec::new();
    for header in registers::HEADERS {
        let path = root.join(header);
        let body = std::fs::read_to_string(&path).map_err(|e| format!("{header}: {e}"))?;
        all.extend(registers::defines(&body, &path));
    }
    let table = registers::Table::new(all);
    let (mut out, count, headers) = registers::rewrite(text, &table);
    if count == 0 {
        return Ok((out, 0));
    }
    let target = games(file)[0];
    let paths = search(root, target);
    let reached = source::reachable(file, &paths);
    for header in headers {
        let header = header.canonicalize().unwrap_or(header);
        if reached.iter().any(|h| h.canonicalize().ok().as_deref() == Some(header.as_path())) {
            continue;
        }
        let name = source::spelling(file, &header, &paths)
            .ok_or_else(|| format!("{}: cannot spell an include of {}", file.display(), header.display()))?;
        out = source::insert_include(&out, &name);
    }
    Ok((out, count))
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let dry = arguments.iter().any(|a| a == "--dry-run");
    let arguments: Vec<String> = arguments.iter().filter(|a| *a != "--dry-run").cloned().collect();
    let Some((rewrite, files)) = arguments.split_first() else {
        return Err(USAGE.into());
    };
    if rewrite == "-h" || rewrite == "--help" || files.is_empty() {
        return Err(USAGE.into());
    }
    let root = root();
    let proved = matches!(rewrite.as_str(), "casts" | "idioms");
    // Proved rewrites compile every candidate set, so files run side by side.
    let workers = if proved {
        std::thread::available_parallelism().map_or(1, |n| (n.get() / 2).max(1)).min(files.len())
    } else {
        1
    };
    let next = std::sync::atomic::AtomicUsize::new(0);
    let results: Vec<std::sync::Mutex<Option<Result<Rewritten, String>>>> =
        files.iter().map(|_| std::sync::Mutex::new(None)).collect();
    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| loop {
                let k = next.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
                let Some(name) = files.get(k) else {
                    break;
                };
                let result = one(root, rewrite, name);
                *results[k].lock().expect("result") = Some(result);
            });
        }
    });
    let mut total = 0;
    for (name, result) in files.iter().zip(results) {
        let Some((file, text, out, count)) = result.into_inner().expect("result").expect("every file runs")? else {
            continue;
        };
        if count > 0 && out != text {
            if !dry {
                std::fs::write(&file, &out).map_err(|e| format!("{name}: {e}"))?;
            }
            println!("{count}\t{name}");
            total += count;
        }
    }
    if dry {
        println!("{total} changes; nothing written (--dry-run)");
    } else {
        println!("{total} changes; run a bare `alchemy keep`, with no paths, to keep only byte-identical files");
    }
    Ok(())
}

/// One file's rewrite: its path, text, rewritten text and count, or none
/// when it was skipped.
#[allow(clippy::type_complexity)]
fn one(root: &Path, rewrite: &str, name: &str) -> Result<Option<(PathBuf, String, String, usize)>, String> {
    let file = std::path::absolute(name).map_err(|e| format!("{name}: {e}"))?;
    let text = std::fs::read_to_string(&file).map_err(|e| format!("{name}: {e}"))?;
    let (out, count) = match rewrite {
        "actors" | "events" => {
            let spec = if rewrite == "actors" { &fields::ACTORS } else { &fields::EVENTS };
            match unit(root, &file, &text, spec) {
                Ok(Some(unit)) => fields::rewrite(&text, spec, &unit),
                Ok(None) => (text.clone(), 0),
                Err(error) => {
                    eprintln!("{name}: skipped: {}", error.lines().next().unwrap_or(""));
                    return Ok(None);
                }
            }
        }
        "registers" => registers_in(root, &file, &text)?,
        "declarations" => {
            let target = games(&file)[0];
            declarations::rewrite(&text, &declarations::index(root, target)?)
        }
        "barriers" => barriers::rewrite(&text),
        "layer" | "commands" => match commands::rewrite(root, &file, &text, rewrite, &games(&file)) {
            Ok(done) => done,
            Err(error) => {
                eprintln!("{name}: skipped: {}", error.lines().next().unwrap_or(""));
                return Ok(None);
            }
        },
        "numbers" | "casts" | "idioms" if library(root, &file) => {
            eprintln!("{name}: skipped: Nintendo library code");
            return Ok(None);
        }
        "numbers" | "casts" | "idioms" => match if rewrite == "numbers" {
            numbers::rewrite(root, &file, &text, &games(&file), &numbers::scene_commands(root))
        } else if rewrite == "casts" {
            casts::rewrite(root, &file, &text, &games(&file))
        } else {
            idioms::rewrite(root, &file, &text, &games(&file))
        } {
            Ok(done) => done,
            Err(error) => {
                eprintln!("{name}: skipped: {}", error.lines().next().unwrap_or(""));
                return Ok(None);
            }
        },
        other => return Err(format!("unknown rewrite {other}\n{USAGE}")),
    };
    Ok(Some((file, text, out, count)))
}

/// A rewritten file: its path, old text, new text and change count.
type Rewritten = Option<(PathBuf, String, String, usize)>;
