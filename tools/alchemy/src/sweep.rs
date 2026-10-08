//! alchemy sweep: drop FAKEMATCH devices the compiler no longer needs.
//!
//! For each tagged register pin, empty asm barrier or volatile in a C file,
//! the device and its tag come out, the file compiles for every edition that
//! builds it, and the removal stays only when every object is byte-identical
//! to the one the last build linked: same object, same ROM.

use crate::targets::{target_for, DecompTarget, TARGET_IDS};
use std::fs;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy sweep FILE.C...\n\
For each FAKEMATCH-tagged device in each file (a register pin, an empty asm\n\
barrier or a volatile qualifier), remove the device and its tag, compile the\n\
file for every edition whose last build compiled it, and keep the removal\n\
when every object is byte-identical to that build's. Other devices (one-pass\n\
blocks, wrappers, dead stores) are listed and left alone. Run make\n\
compare-editions first, and again after.";

pub fn run(args: &[String]) -> Result<(), String> {
    if args.is_empty() || args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return if args.is_empty() { Err("no file given".into()) } else { Ok(()) };
    }
    let root = crate::compiler::routing::root();
    let (mut kept, mut tried, mut skipped) = (0, 0, 0);
    for file in args {
        let source = relative(root, file)?;
        let report = sweep(root, &source)?;
        kept += report.kept;
        tried += report.tried;
        skipped += report.other;
        println!("{source}: removed {} of {} devices tried; {} other devices left", report.kept, report.tried, report.other);
    }
    println!("swept {} file(s): removed {kept} of {tried}; {skipped} not mechanical", args.len());
    Ok(())
}

fn relative(root: &Path, file: &str) -> Result<String, String> {
    let path = fs::canonicalize(file).map_err(|error| format!("{file}: {error}"))?;
    let root = fs::canonicalize(root).map_err(|error| error.to_string())?;
    path.strip_prefix(&root)
        .map(|path| path.to_string_lossy().into_owned())
        .map_err(|_| format!("{file} is outside the checkout"))
}

struct Report {
    kept: usize,
    tried: usize,
    other: usize,
}

/// The editions whose last build compiled this source, with its object.
fn built(root: &Path, source: &str) -> Vec<(DecompTarget, PathBuf)> {
    TARGET_IDS
        .iter()
        .map(|id| target_for(*id))
        .filter_map(|target| {
            let object = root.join(target.output_dir).join("obj").join(source).with_extension("o");
            object.is_file().then_some((target, object))
        })
        .collect()
}

/// Whether the file as it stands compiles to the built objects.
fn identical(root: &Path, source: &str, editions: &[(DecompTarget, PathBuf)]) -> Result<bool, String> {
    let results: Vec<Result<bool, String>> = std::thread::scope(|scope| {
        let handles: Vec<_> = editions
            .iter()
            .map(|(target, object)| {
                scope.spawn(move || {
                    let trial = object.with_extension("sweep.o");
                    let steps = crate::build_rom::c_steps(*target, Path::new(source), &trial)?;
                    let ran = steps.iter().try_for_each(|step| crate::process::run(step, root).map(drop));
                    let same = ran.is_ok()
                        && fs::read(&trial).ok() == Some(fs::read(object).map_err(|e| e.to_string())?);
                    for scratch in [trial.clone(), trial.with_extension("s"), trial.with_extension("i")] {
                        let _ = fs::remove_file(scratch);
                    }
                    Ok(same)
                })
            })
            .collect();
        handles.into_iter().map(|handle| handle.join().unwrap()).collect()
    });
    let mut all = true;
    for result in results {
        all &= result?;
    }
    Ok(all)
}

fn sweep(root: &Path, source: &str) -> Result<Report, String> {
    let path = root.join(source);
    let original = fs::read_to_string(&path).map_err(|error| format!("{source}: {error}"))?;
    let editions = built(root, source);
    if editions.is_empty() {
        return Err(format!("{source}: no edition's build compiled it; run make compare-editions"));
    }
    if !identical(root, source, &editions)? {
        return Err(format!("{source}: does not rebuild its objects as it stands; run make compare-editions"));
    }
    let mut text = original.clone();
    let mut report = Report { kept: 0, tried: 0, other: 0 };
    // Last tag first, so that a kept removal moves no tag still to try.
    for tag in tags(&original).into_iter().rev() {
        let Some(edited) = remove(&text, &tag) else {
            report.other += 1;
            continue;
        };
        report.tried += 1;
        fs::write(&path, &edited).map_err(|error| format!("{source}: {error}"))?;
        let same = identical(root, source, &editions);
        match same {
            Ok(true) => {
                report.kept += 1;
                text = edited;
            }
            Ok(false) => {}
            Err(error) => {
                fs::write(&path, &text).map_err(|error| format!("{source}: {error}"))?;
                return Err(error);
            }
        }
        fs::write(&path, &text).map_err(|error| format!("{source}: {error}"))?;
    }
    Ok(report)
}

/// A FAKEMATCH tag: the byte range of its comment.
#[derive(Clone, Debug, PartialEq)]
struct Tag {
    start: usize,
    end: usize,
}

fn tags(text: &str) -> Vec<Tag> {
    let mut found = Vec::new();
    let mut at = 0;
    while let Some(offset) = text[at..].find("/*") {
        let start = at + offset;
        let Some(length) = text[start..].find("*/") else { break };
        let end = start + length + 2;
        if text[start + 2..end - 2].trim_start().starts_with("FAKEMATCH") {
            found.push(Tag { start, end });
        }
        at = end;
    }
    found
}

/// The text without the tag and the device it marks, or None when the
/// device is not one sweep can take out mechanically. The device is the
/// first pin, empty asm or volatile from the tag to the end of the next
/// statement.
fn remove(text: &str, tag: &Tag) -> Option<String> {
    let after = &text[tag.end..];
    let statement = after.find(';').map(|end| end + 1)?;
    let span = &after[..statement];
    let pin = regex::Regex::new(r#"\bregister\s+([^;]*?)\s*(?:asm|__asm__)\s*\(\s*"(?:r\d+|ip|sp|lr|fp|sl)"\s*\)"#)
        .expect("static pattern");
    let barrier = regex::Regex::new(r#"^\s*(?:asm|__asm__)\s*(?:volatile|__volatile__)?\s*\(\s*""[^;]*\)\s*;"#)
        .expect("static pattern");
    let volatile = regex::Regex::new(r"\bvolatile\s+").expect("static pattern");
    let device = if let Some(found) = barrier.find(span) {
        let mut out = String::new();
        out.push_str(&span[..found.start()]);
        out.push_str(&span[found.end()..]);
        out
    } else if let Some(capture) = pin.captures(span) {
        let whole = capture.get(0).unwrap();
        format!("{}{}{}", &span[..whole.start()], &capture[1], &span[whole.end()..])
    } else if !span.contains("asm") && volatile.is_match(span) {
        volatile.replace(span, "").into_owned()
    } else {
        return None;
    };
    // The tag goes with its line when it stands alone on it.
    let line_start = text[..tag.start].rfind('\n').map_or(0, |at| at + 1);
    let tag_line_rest = &after[..after.find('\n').unwrap_or(after.len())];
    let alone = text[line_start..tag.start].trim().is_empty() && tag_line_rest.trim().is_empty();
    let (prefix, mut device) = if alone {
        let skip = device.find('\n').map_or(device.len(), |at| at + 1);
        (&text[..line_start], device[skip..].to_string())
    } else {
        (text[..tag.start].trim_end_matches([' ', '\t']), device)
    };
    let mut rest = &after[statement..];
    // A removed barrier leaves its line empty: the line goes too.
    if device.trim().is_empty() && rest.starts_with('\n') {
        device.clear();
        rest = &rest[1..];
    }
    Some(format!("{prefix}{device}{rest}"))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn removed(text: &str) -> Option<String> {
        remove(text, &tags(text)[0])
    }

    #[test]
    fn a_pin_becomes_an_ordinary_local() {
        let text = "{\n    /* FAKEMATCH: r9 */\n    register struct W *w asm(\"r9\");\n    f();\n}\n";
        assert_eq!(removed(text).unwrap(), "{\n    struct W *w;\n    f();\n}\n");
    }

    #[test]
    fn a_barrier_goes_with_its_line() {
        let text = "{\n    x = 1;\n    /* FAKEMATCH: order */\n    __asm__ volatile (\"\" : \"+r\" (t));\n    g(t);\n}\n";
        assert_eq!(removed(text).unwrap(), "{\n    x = 1;\n    g(t);\n}\n");
    }

    #[test]
    fn a_volatile_loses_its_qualifier() {
        let text = "{\n    /* FAKEMATCH:\n       spill */\n    volatile s32 v = 0;\n}\n";
        assert_eq!(removed(text).unwrap(), "{\n    s32 v = 0;\n}\n");
    }

    #[test]
    fn other_devices_stay() {
        let text = "{\n    /* FAKEMATCH: one pass */\n    do {\n        f();\n    } while (0);\n}\n";
        assert!(removed(text).is_none());
        let text = "{\n    /* FAKEMATCH: dead store */\n    result = -1;\n}\n";
        assert!(removed(text).is_none());
    }

    #[test]
    fn a_trailing_tag_goes_with_its_device() {
        let text = "    register s32 a asm(\"r4\") = 0; /* FAKEMATCH: r4 */\n";
        // The tag follows its device: nothing after it to take.
        assert!(removed(text).is_none());
    }
}
