//! Golden Sun owner integration and source reconstruction, outside Psynergy.
mod adopt;
pub mod cli;
mod imports;
mod owners;
use psynergy::lift::contains_call;
use psynergy::{decode, lift, unit};
use std::collections::BTreeMap;
use std::path::Path;

/// Lifts an owner from the canonical ROM. `span` falls back to the register.
pub fn lift_owner(
    root: &Path,
    owner: &str,
    span: Option<u32>,
    name: Option<&str>,
) -> Result<(String, u32), String> {
    let (image, base, entry, span) = owners::image_window(root, owner, span)?;
    let name = name.map(str::to_string).unwrap_or_else(|| {
        let prefix = if base == decode::MAIN_BASE {
            "Func"
        } else {
            "Lifted"
        };
        format!("{prefix}_{entry:08x}")
    });
    let main = base == decode::MAIN_BASE;
    let ins = decode::decode_window_at(&image, base, entry, span);
    let text = source(&ins, entry, &name, main);
    Ok((text, span))
}

fn source(ins: &[decode::Ins], entry: u32, name: &str, main: bool) -> String {
    let mut tables = BTreeMap::new();
    let body = unit::split_functions(ins)
        .iter()
        .map(|(entry, function)| {
            let mut draft = lift::lift(function, &mut tables, main, &[0x03001ebc]);
            if !main {
                rewrite_scene_work(&mut draft.lines);
            }
            let source = unit::function_source(*entry, &draft);
            if main {
                source
            } else {
                source
                    .lines()
                    .map(scene_spelling)
                    .collect::<Vec<_>>()
                    .join("\n")
                    + "\n"
            }
        })
        .collect::<Vec<_>>()
        .join("\n");
    let body = body.replace("0x03001ebc", "Data_03001ebc");
    let helper = if main {
        String::new()
    } else {
        "/* The scene step counter at 0x1d8 of the shared scene work record. */\n\
static __inline__ void bump_step(s32 amount)\n{\n    u8 *work = *(u8 **)Data_03001ebc;\n\n    \
*(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);\n}\n\n"
            .to_string()
    };
    let symbols_note = if main {
        "/* Main-image symbols: every pool word inside the ROM or the work RAM. */\n"
    } else {
        "/* Loader-relocated overlay calls: each symbol names the pre-relocation call\n\
\x20* word the image holds. */\n"
    };

    let source = unit::compose(entry, name, &format!("{helper}{body}"), &tables);
    source.replacen(
        &format!("#define {name} Func_{entry:08x}\n\n"),
        &format!("#define {name} Func_{entry:08x}\n\n{symbols_note}"),
        1,
    )
}
#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn scene_reconstruction_is_owned_by_alchemy() {
        let image = [0x08, 0x60, 0x70, 0x47];
        let ins = decode::decode_window_at(&image, 0x02000000, 0x02000000, 4);
        let (body, tables) = unit::bodies(&ins, false);
        let portable = unit::compose(0x02000000, "Store", &body, &tables);
        assert!(!portable.contains("bump_step"));
        assert!(!portable.contains("Data_03001ebc"));
        let overlay = source(&ins, 0x02000000, "Store", false);
        assert!(overlay.contains("static __inline__ void bump_step"));
        assert!(overlay.contains("extern u8 Data_03001ebc[];"));
        assert!(!source(&ins, 0x02000000, "Store", true).contains("bump_step"));
    }

    #[test]
    fn scene_step_and_volatile_rules_survive_the_move() {
        let mut lines = vec!["    *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;".into()];
        rewrite_scene_work(&mut lines);
        assert_eq!(lines, ["    bump_step(1);"]);
        assert_eq!(
            scene_spelling("*(s32 *)(slot0 + 4) = 1;"),
            "*(s32 *)(slot0 + 4) = 1;"
        );
        assert_eq!(
            scene_spelling("*(u16 *)(work + 4) = 1;"),
            "*(u16 *)(work + 4) = 1;"
        );
        assert_eq!(
            scene_spelling("*(u16 *)(record + 4) = 1;"),
            "*(volatile u16 *)(record + 4) = 1;"
        );
    }
}

fn scene_spelling(line: &str) -> String {
    let mut spelled = volatile_spelling(line);
    for width in ["s32", "u32", "u16", "u8"] {
        spelled = spelled
            .replace(
                &format!("*(volatile {width} *)(slot"),
                &format!("*({width} *)(slot"),
            )
            .replace(
                &format!("*(volatile {width} *)slot"),
                &format!("*({width} *)slot"),
            );
    }
    spelled
}
/// Word and halfword accesses through record and work pointers are volatile
/// in the original: the scheduler orders a volatile store before a later
/// volatile load or store, which the reference schedules show. Byte
/// accesses already alias everything and stay as they are.
pub fn volatile_spelling(line: &str) -> String {
    let spelled = line
        .replace("*(u16 *)", "*(volatile u16 *)")
        .replace("*(s32 *)", "*(volatile s32 *)")
        .replace("*(u32 *)", "*(volatile u32 *)")
        .replace("*(u8 **)", "*(u8 *volatile *)");
    // The scene work pointer is a volatile global, but the record it points
    // at is plain: its field stores fold their offsets into the address.
    let mut plain = spelled;
    for width in ["u16", "s32", "u32"] {
        for base in [
            "(work + ",
            "(*(u8 *volatile *)0x03001ebc + ",
            "((*(u8 *volatile *)0x03001ebc + ",
        ] {
            plain = plain.replace(
                &format!("*(volatile {width} *){base}"),
                &format!("*({width} *){base}"),
            );
        }
    }
    plain
}

/// The scene work record at 0x03001ebc: its step counter bump becomes the
/// `bump_step` helper and its other fields read through a byte pointer.
fn rewrite_scene_work(lines: &mut Vec<String>) {
    const STEP: &str = "*(u16 *)((*(s32 *)0x03001ebc + 0x1d8))";
    for line in lines.iter_mut() {
        let trimmed = line.trim_start();
        let lead = line.len() - trimmed.len();
        if let Some(rest) = trimmed.strip_prefix(STEP) {
            if let Some(rest) = rest.strip_prefix(" = (") {
                if let Some(rest) = rest.strip_prefix(STEP) {
                    if let Some(amount) =
                        rest.strip_prefix(" + ").and_then(|r| r.strip_suffix(");"))
                    {
                        if amount.bytes().all(|b| b.is_ascii_digit()) {
                            *line = format!("{}bump_step({amount});", &line[..lead]);
                            continue;
                        }
                    }
                }
            }
        }
        if let Some(rest) = trimmed.strip_prefix(STEP) {
            if let Some(amount) = rest.strip_prefix(" += ").and_then(|r| r.strip_suffix(';')) {
                if amount.bytes().all(|b| b.is_ascii_digit()) {
                    *line = format!("{}bump_step({amount});", &line[..lead]);
                    continue;
                }
            }
        }
        *line = line.replace("(*(s32 *)0x03001ebc + 0x", "(*(u8 **)0x03001ebc + 0x");
    }
    share_scene_work(lines);
}

/// Consecutive statements through the scene work pointer read it once: the
/// reference loads it into one register for the run, and a volatile read
/// cannot be merged by the compiler, so the run names it.
fn share_scene_work(lines: &mut Vec<String>) {
    const WORK: &str = "(*(u8 **)0x03001ebc + 0x";
    let uses_work = |line: &str| {
        line.contains(WORK) && !contains_call(line) && !line.trim_start().starts_with("bump_step")
    };
    let mut at = 0;
    while at < lines.len() {
        if !uses_work(&lines[at]) {
            at += 1;
            continue;
        }
        let lead = lines[at].len() - lines[at].trim_start().len();
        let mut end = at + 1;
        while end < lines.len()
            && uses_work(&lines[end])
            && lines[end].len() - lines[end].trim_start().len() == lead
        {
            end += 1;
        }
        if end - at >= 2 {
            for line in &mut lines[at..end] {
                *line = line.replace(WORK, "(work + 0x");
            }
            lines.insert(
                at,
                format!("{}work = *(u8 **)0x03001ebc;", " ".repeat(lead)),
            );
            end += 1;
        }
        at = end;
    }
}
