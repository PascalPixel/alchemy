use crate::compiler::routing::root;
use crate::process::run as command;
use std::path::Path;

const USAGE: &str = r#"usage: alchemy integrate BRANCH... [--no-build]
Merges each BRANCH that is not yet in the current branch, in order, and
stops at the first one that conflicts, leaving the merge for you to
resolve. After the merges it reconciles each edition's link order, its
linker script or its rows of the game's object list (alchemy
merge-driver links) and commits any change, then runs make
compare-editions; for each edition that differs it names the first
object whose bytes differ, from that edition's map and reference ROM."#;

pub(crate) fn first_difference(map: &str, reference: &[u8], built: &[u8]) -> Option<(u32, u32, String)> {
    let mut best: Option<(u32, u32, String)> = None;
    for line in map.lines() {
        let line = line.trim_start();
        let Some(rest) = [".text", ".rodata", ".data"].iter().find_map(|s| line.strip_prefix(s)) else { continue };
        let mut words = rest.split_whitespace();
        let (Some(address), Some(size), Some(path)) = (words.next(), words.next(), words.next()) else { continue };
        let (Ok(address), Ok(size)) = (
            u32::from_str_radix(address.trim_start_matches("0x"), 16),
            u32::from_str_radix(size.trim_start_matches("0x"), 16),
        ) else { continue };
        let Some(start) = address.checked_sub(0x0800_0000) else { continue };
        let (start, end) = (start as usize, (start + size) as usize);
        if size == 0 || end > reference.len() || end > built.len() {
            continue;
        }
        if let Some(k) = (start..end).find(|&k| reference[k] != built[k]) {
            if best.as_ref().is_none_or(|(a, _, _)| address < *a) {
                best = Some((address, (k - start) as u32, path.to_string()));
            }
        }
    }
    best
}

fn report_editions(root: &Path, output: &str) {
    for line in output.lines() {
        let Some(edition) = line.strip_suffix(".gba: FAILED").and_then(|l| l.rsplit('/').next()) else { continue };
        let read = |p: String| std::fs::read(root.join(p)).unwrap_or_default();
        let map = std::fs::read_to_string(root.join(format!("out/{edition}/{edition}.map"))).unwrap_or_default();
        match first_difference(&map, &crate::targets::decomp_target(Some(edition)).map(|target| read(target.rom.to_string())).unwrap_or_default(), &read(format!("out/{edition}/{edition}.gba"))) {
            Some((address, offset, path)) => println!("{edition}: first difference at {address:#010x}+{offset:#x} in {}", path.rsplit("/obj/").next().unwrap_or(&path)),
            None => println!("{edition}: differs outside linked objects (size or padding)"),
        }
    }
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let build = !arguments.iter().any(|a| a == "--no-build");
    let branches: Vec<&String> = arguments.iter().filter(|a| !a.starts_with("--")).collect();
    if branches.is_empty() || arguments.iter().any(|a| a == "-h" || a == "--help") {
        return Err(USAGE.into());
    }
    let root = root();
    if !command(&["git", "status", "--porcelain", "--", "games", "recon"], root)?.trim().is_empty() {
        return Err("games/ or recon/ has uncommitted changes; commit or stash them first".into());
    }
    for branch in branches {
        if command(&["git", "merge-base", "--is-ancestor", branch, "HEAD"], root).is_ok() {
            println!("{branch}: already in");
            continue;
        }
        match command(&["git", "merge", "--no-edit", branch], root) {
            Ok(_) => println!("{branch}: merged"),
            Err(_) => {
                let conflicts = command(&["git", "diff", "--name-only", "--diff-filter=U"], root).unwrap_or_default();
                return Err(format!("{branch}: conflicts to resolve, then commit and run again:\n{conflicts}"));
            }
        }
    }
    crate::merge::run(&["links".to_string()])?;
    if !command(&["git", "status", "--porcelain", "--", "games", "recon"], root)?.trim().is_empty() {
        command(&["git", "add", "-A", "games", "recon"], root)?;
        command(&["git", "commit", "-q", "-m", "Reconcile the editions' link order after the merges"], root)?;
        println!("committed the reconciled link order");
    }
    if build {
        let output = std::process::Command::new("make")
            .args(["compare-editions"])
            .current_dir(root)
            .output()
            .map_err(|e| e.to_string())?;
        let text = String::from_utf8_lossy(&output.stdout).into_owned() + &String::from_utf8_lossy(&output.stderr);
        let ok = text.lines().filter(|l| l.ends_with(".gba: OK")).count();
        println!("{ok} of 12 editions identical");
        report_editions(root, &text);
        let mut seen = std::collections::BTreeSet::new();
        for line in text.lines().filter(|l| !l.contains("warning") && (l.contains(": error") || l.contains("undefined reference") || l.contains("multiple definition") || l.contains("has no member") || l.contains("conflicting types") || l.contains("already defined"))) {
            if seen.insert(line.to_string()) && seen.len() <= 12 {
                println!("{line}");
            }
        }
        if ok != 12 {
            return Err("some editions differ; fix them before going on".into());
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn names_the_first_object_that_differs() {
        let map = " .text          0x0000000008000000        0x4 obj/A.o\n .text          0x0000000008000004        0x4 obj/B.o\n";
        let reference = [1, 2, 3, 4, 5, 6, 7, 8];
        let built = [1, 2, 3, 4, 5, 9, 7, 8];
        assert_eq!(first_difference(map, &reference, &built), Some((0x0800_0004, 1, "obj/B.o".to_string())));
        assert_eq!(first_difference(map, &reference, &reference), None);
    }
}
