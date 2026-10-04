//! `alchemy adopt`: once a function's C is in its module, take its bytes
//! out of every edition's scaffolding: the English raw listing and its
//! linker line, the draft, and in each other edition the scaffold section
//! that holds exactly that function and its linker line. Optionally drops a
//! two-byte padding section beside it, which the compiler's own alignment
//! supplies once the function is C, and renames the label everywhere.
use crate::compiler::routing::root;
use std::path::{Path, PathBuf};

const USAGE: &str = r#"usage: alchemy adopt ADDRESS LABEL [NEW_NAME] [--game tbs|tla] [--padding]
After the function's C is in its module: removes recon/<game>/raw/ADDRESS.s,
its line in recon/<game>/en/MAIN.LD and the draft recon/<game>/en/main/
ADDRESS.c; in each other edition removes the rom.s section that holds
exactly LABEL and its line in that edition's linker script (Japanese:
games/<GAME>/MAIN.LD). --padding also removes a two-byte section directly
before or after it. NEW_NAME renames LABEL across games/ and recon/. The
game defaults to tbs. Then build all twelve editions."#;

fn read(path: &Path) -> Result<String, String> {
    std::fs::read_to_string(path).map_err(|e| format!("{}: {e}", path.display()))
}

fn write(path: &Path, text: &str) -> Result<(), String> {
    std::fs::write(path, text).map_err(|e| format!("{}: {e}", path.display()))
}

fn section_name(line: &str) -> Option<&str> {
    line.strip_prefix("\t.section .rom.")?.strip_suffix(", \"ax\"")
}

fn is_label(line: &str) -> bool {
    line.ends_with(':') && !line.starts_with(['\t', ' ', '@', '.'])
}

/// The [start, end) line range of the scaffold section holding `label`.
fn section_of(lines: &[&str], label: &str) -> Option<(usize, usize)> {
    let at = lines.iter().position(|l| *l == format!("{label}:"))?;
    let start = (0..=at).rev().find(|&i| section_name(lines[i]).is_some())?;
    let end = (at + 1..lines.len()).find(|&i| section_name(lines[i]).is_some()).unwrap_or(lines.len());
    Some((start, end))
}

fn padding_section(lines: &[&str], start: usize, end: usize) -> bool {
    end - start == 2 && lines[start + 1].ends_with(", 0x00000002")
}

/// Remove `label`'s section (and, with `padding`, a two-byte neighbour)
/// from one scaffold; returns the new text and the removed section names.
pub(crate) fn remove_section(text: &str, label: &str, padding: bool) -> Result<(String, Vec<String>), String> {
    let lines: Vec<&str> = text.lines().collect();
    let (start, end) = section_of(&lines, label).ok_or_else(|| format!("no section holds {label}"))?;
    let block = &lines[start..end];
    let labels = block.iter().filter(|l| is_label(l)).count();
    let incbins = block.iter().filter(|l| l.contains(".incbin")).count();
    if labels != 1 || incbins != 1 {
        return Err(format!(
            "the section holding {label} holds {labels} labels and {incbins} byte ranges; split it by hand"
        ));
    }
    let mut cut = vec![(start, end)];
    if padding {
        let previous = (0..start).rev().find(|&i| section_name(lines[i]).is_some());
        if let Some(p) = previous.filter(|&p| padding_section(&lines, p, start)) {
            cut.push((p, start));
        }
        let next_end = (end + 1..lines.len()).find(|&i| section_name(lines[i]).is_some()).unwrap_or(lines.len());
        if end < lines.len() && padding_section(&lines, end, next_end) {
            cut.push((end, next_end));
        }
    }
    let names = cut.iter().filter_map(|&(s, _)| section_name(lines[s]).map(str::to_string)).collect();
    let kept: Vec<&str> = lines
        .iter()
        .enumerate()
        .filter(|(i, _)| !cut.iter().any(|&(s, e)| *i >= s && *i < e))
        .map(|(_, l)| *l)
        .collect();
    Ok((kept.join("\n") + "\n", names))
}

fn drop_lines(text: &str, keep: impl Fn(&str) -> bool) -> (String, usize) {
    let lines: Vec<&str> = text.lines().filter(|l| keep(l)).collect();
    let removed = text.lines().count() - lines.len();
    (lines.join("\n") + "\n", removed)
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let mut positional = Vec::new();
    let (mut game, mut padding) = ("tbs", false);
    let mut i = 0;
    while i < arguments.len() {
        match arguments[i].as_str() {
            "--padding" => padding = true,
            "--game" => {
                i += 1;
                game = match arguments.get(i).map(String::as_str) {
                    Some("tbs") => "tbs",
                    Some("tla") => "tla",
                    _ => return Err(USAGE.into()),
                };
            }
            "-h" | "--help" => return Err(USAGE.into()),
            value => positional.push(value.to_string()),
        }
        i += 1;
    }
    let (address, label, new_name) = match positional.as_slice() {
        [a, l] => (a.clone(), l.clone(), None),
        [a, l, n] => (a.clone(), l.clone(), Some(n.clone())),
        _ => return Err(USAGE.into()),
    };
    let root = root();
    let game_dir = if game == "tla" { "THE LOST AGE" } else { "THE BROKEN SEAL" };

    // English: the raw listing, its linker line and the draft.
    let english = root.join(format!("recon/{game}/en/MAIN.LD"));
    let object = format!("\"*/recon/{game}/raw/{address}.o\"");
    let (text, removed) = drop_lines(&read(&english)?, |l| !l.contains(&object));
    if removed != 1 {
        return Err(format!("{}: {removed} lines link {object}", english.display()));
    }
    write(&english, &text)?;
    for file in [format!("recon/{game}/raw/{address}.s"), format!("recon/{game}/en/main/{address}.c")] {
        if root.join(&file).exists() {
            crate::process::run(&["git", "rm", "-q", "-f", &file], root)?;
        }
    }

    // The other editions: the section holding exactly the function.
    for lang in ["ja", "de", "es", "fr", "it"] {
        let scaffold = root.join(format!("recon/{game}/{lang}/rom.s"));
        let script: PathBuf = if lang == "ja" {
            root.join(format!("games/{game_dir}/MAIN.LD"))
        } else {
            root.join(format!("recon/{game}/{lang}/MAIN.LD"))
        };
        let (text, sections) = remove_section(&read(&scaffold)?, &label, padding).map_err(|e| format!("{game}-{lang}: {e}"))?;
        write(&scaffold, &text)?;
        let (text, removed) = drop_lines(&read(&script)?, |l| !sections.iter().any(|s| l.contains(&format!("(.rom.{s})"))));
        if removed != sections.len() {
            return Err(format!("{}: expected {} section lines, removed {removed}", script.display(), sections.len()));
        }
        write(&script, &text)?;
    }

    if let Some(name) = new_name.filter(|n| *n != label) {
        let files = crate::process::run(&["git", "grep", "-l", "-w", &label, "--", "games", "recon"], root).unwrap_or_default();
        let mut count = 0;
        for file in files.lines().filter(|f| !f.is_empty()) {
            let path = root.join(file);
            let text = read(&path)?;
            let renamed = rename_word(&text, &label, &name);
            if renamed != text {
                write(&path, &renamed)?;
                count += 1;
            }
        }
        println!("renamed {label} to {name} in {count} files");
    }
    println!("adopted {address} in all six {game} editions; build all twelve to confirm");
    Ok(())
}

fn rename_word(text: &str, from: &str, to: &str) -> String {
    let is_word = |c: char| c.is_ascii_alphanumeric() || c == '_';
    let mut out = String::with_capacity(text.len());
    let mut rest = text;
    while let Some(at) = rest.find(from) {
        let before = rest[..at].chars().next_back();
        let after = rest[at + from.len()..].chars().next();
        out.push_str(&rest[..at]);
        if before.is_some_and(is_word) || after.is_some_and(is_word) {
            out.push_str(from);
        } else {
            out.push_str(to);
        }
        rest = &rest[at + from.len()..];
    }
    out.push_str(rest);
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    const SCAFFOLD: &str = "@ h\n\t.section .rom.00000100, \"ax\"\n\t.global A\nA:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n\t.section .rom.00000110, \"ax\"\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000002\n\t.section .rom.00000112, \"ax\"\n\t.global B\n\t.thumb_func\nB:\n\t.incbin \"baserom.gba\", 0x00000112, 0x00000020\n";

    #[test]
    fn removes_the_function_section_and_its_padding() {
        let (text, names) = remove_section(SCAFFOLD, "B", true).unwrap();
        assert_eq!(names, ["00000112", "00000110"]);
        assert_eq!(text, "@ h\n\t.section .rom.00000100, \"ax\"\n\t.global A\nA:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n");
        assert!(remove_section(SCAFFOLD, "Missing", false).is_err());
    }

    #[test]
    fn renames_whole_words_only() {
        assert_eq!(rename_word("Func_08 Func_080 xFunc_08", "Func_08", "Run"), "Run Func_080 xFunc_08");
    }
}
