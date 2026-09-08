//! Read GCC's pass dumps: pseudo creation, costs, conflicts, allocation and reloads.

use compiler_core::{
    routing::{cflags_for_target_source, CompilerTarget},
    source_paths::SourceOwner,
};
use std::collections::BTreeMap;
use std::fs;
use std::path::Path;
use std::process::{Command, ExitCode};

const REGS: [&str; 16] = [
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "sl", "fp", "ip", "sp", "lr", "pc",
];

#[cfg(test)]
mod tests {
    #[test]
    fn invalid_owners_and_route_overrides_are_rejected_before_compilation() {
        for args in [
            vec!["../../elsewhere"],
            vec!["resource_3ba:02002910"],
            vec!["080bbb0c", "-fno-cse-follow-jumps"],
            vec!["080bbb0c", "source.c", "extra"],
        ] {
            let args = args.into_iter().map(str::to_string).collect::<Vec<_>>();
            assert!(super::run(&args).is_err());
        }
        assert!(super::run(&["--help".into()]).is_ok());
    }
}

pub fn entry(args: &[String]) -> ExitCode {
    match run(args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

fn run(args: &[String]) -> Result<(), String> {
    const USAGE: &str = "usage: alchemy inspect allocator MAIN_OWNER [candidate.c]";
    if args == ["--help"] || args == ["-h"] {
        println!("{USAGE}");
        return Ok(());
    }
    if args.is_empty() || args.len() > 2 || args.iter().any(|a| a.starts_with('-')) {
        return Err(USAGE.into());
    }
    let parsed = SourceOwner::parse_argument(&args[0])?;
    if !parsed.is_main() {
        return Err("allocator inspection currently requires a main-ROM owner".into());
    }
    let owner = format!("{:08x}", parsed.address());
    let source = args
        .get(1)
        .cloned()
        .unwrap_or_else(|| format!("games/gs1/recon/en/main/{owner}.c"));
    let repo = compiler_core::routing::root();
    let directory = repo.join("out/allocator");
    fs::create_dir_all(&directory).map_err(|e| e.to_string())?;
    let work = tempfile::Builder::new()
        .prefix(&format!("{owner}-"))
        .tempdir_in(&directory)
        .map_err(|e| e.to_string())?
        .keep();
    let src = repo.join(&source);
    let bundle = compiler_core::routing::bundle();
    let mut cpp = compiler_core::plan::direct_preprocessor_command(
        &src.to_string_lossy(),
        &work.join("in.i").to_string_lossy(),
    )
    .map_err(|e| e.to_string())?;
    let bindings = work.join("bindings.h");
    fs::write(
        &bindings,
        crate::candidate::source_symbol_bindings(&repo, &source, CompilerTarget::Gs1)?,
    )
    .map_err(|e| e.to_string())?;
    cpp.splice(
        1..1,
        ["-include".into(), bindings.to_string_lossy().into_owned()],
    );
    checked(Path::new(&cpp[0]), &cpp[1..], &repo)?;
    let mut cc1 = cflags_for_target_source(CompilerTarget::Gs1, &source);
    cc1.retain(|flag| flag != "-nostdinc" && !flag.starts_with("-I"));
    cc1.extend([
        "-quiet".into(),
        "-da".into(),
        "-o".into(),
        work.join("out.s").to_string_lossy().into_owned(),
        work.join("in.i").to_string_lossy().into_owned(),
    ]);
    checked(&bundle.join("cc1"), &cc1, &work)?;

    let read = |pass: &str| -> String {
        fs::read_dir(&work)
            .ok()
            .and_then(|entries| {
                entries
                    .filter_map(Result::ok)
                    .map(|e| e.path())
                    .find(|p| p.to_string_lossy().ends_with(&format!(".{pass}")))
            })
            .and_then(|p| fs::read_to_string(p).ok())
            .unwrap_or_default()
    };

    // Pseudo -> source variable, from RTL `(reg/v:SI NN [ name ])` annotations.
    let rtl = read("rtl");
    let mut names: BTreeMap<u32, String> = BTreeMap::new();
    let mut creation: Vec<u32> = Vec::new();
    let mut rest = rtl.as_str();
    while let Some(at) = rest.find("(reg") {
        rest = &rest[at + 4..];
        let Some(space) = rest.find(' ') else { break };
        let after = &rest[space + 1..];
        let digits: String = after.chars().take_while(|c| c.is_ascii_digit()).collect();
        let Ok(pseudo) = digits.parse::<u32>() else {
            continue;
        };
        if pseudo < 32 {
            continue;
        }
        if !creation.contains(&pseudo) {
            creation.push(pseudo);
        }
        let tail = &after[digits.len()..];
        if let Some(name) = tail.strip_prefix(" [ ").and_then(|t| t.split(" ]").next()) {
            names
                .entry(pseudo)
                .or_insert_with(|| name.trim().to_string());
        }
    }

    // Local alloc: class costs and preferences.
    let lreg = read("lreg");
    let mut costs: BTreeMap<u32, String> = BTreeMap::new();
    let mut prefs: BTreeMap<u32, String> = BTreeMap::new();
    for line in lreg.lines() {
        let Some(rest) = line.strip_prefix("Register ") else {
            continue;
        };
        let digits: String = rest.chars().take_while(|c| c.is_ascii_digit()).collect();
        let Ok(pseudo) = digits.parse::<u32>() else {
            continue;
        };
        if let Some(list) = rest.split("costs: ").nth(1) {
            let interesting: Vec<&str> = list
                .split(' ')
                .filter(|c| {
                    ["LO_REGS:", "HI_REGS:", "MEM:", "GENERAL_REGS:"]
                        .iter()
                        .any(|p| c.starts_with(p))
                })
                .collect();
            costs.insert(pseudo, interesting.join(" "));
        }
        if let Some(pref) = rest.split("pref ").nth(1) {
            prefs.insert(pseudo, pref.trim_end_matches('.').trim().to_string());
        }
    }

    // Global alloc: ordering, conflicts, dispositions, spills, reloads.
    let greg = read("greg");
    let mut order: Vec<u32> = Vec::new();
    let mut conflicts: BTreeMap<u32, String> = BTreeMap::new();
    let mut assigned: BTreeMap<u32, u32> = BTreeMap::new();
    let mut spills: Vec<String> = Vec::new();
    let mut reloads: Vec<String> = Vec::new();
    let mut dispositions = false;
    for text in [&lreg, &greg] {
        for line in text.lines() {
            if let Some(list) = line.split("regs to allocate: ").nth(1) {
                order = list
                    .split_whitespace()
                    .filter_map(|n| n.parse().ok())
                    .collect();
            }
            if let Some(rest) = line.strip_prefix(";; ") {
                if let Some((pseudo, list)) = rest.split_once(" conflicts: ") {
                    if let Ok(p) = pseudo.parse::<u32>() {
                        conflicts.insert(p, list.trim().to_string());
                    }
                }
                if let Some(rest) = rest.strip_prefix("Register ") {
                    if let Some((p, h)) = rest.split_once(" in ") {
                        if let (Ok(p), Ok(h)) = (
                            p.trim().parse::<u32>(),
                            h.trim().trim_end_matches('.').parse::<u32>(),
                        ) {
                            assigned.insert(p, h);
                        }
                    }
                }
            }
            if line.contains("Register dispositions:") {
                dispositions = true;
                continue;
            }
            if dispositions {
                if line.trim().is_empty() || line.starts_with(";;") || line.starts_with('(') {
                    dispositions = false;
                } else {
                    let words: Vec<&str> = line.split_whitespace().collect();
                    let mut k = 0;
                    while k + 2 < words.len() {
                        if words[k + 1] == "in" {
                            if let (Ok(p), Ok(h)) =
                                (words[k].parse::<u32>(), words[k + 2].parse::<u32>())
                            {
                                assigned.insert(p, h);
                            }
                            k += 3;
                        } else {
                            k += 1;
                        }
                    }
                }
            }
            if let Some(rest) = line.strip_prefix("Spilling for insn ") {
                spills.push(rest.trim_end_matches('.').to_string());
            }
            if let Some(rest) = line.strip_prefix("Using reg ") {
                if let Some((reg, reload)) = rest.split_once(" for reload ") {
                    reloads.push(format!("r{reg} for reload {}", reload.trim()));
                }
            }
        }
    }

    println!(
        "{owner}: {} pseudos, global order: {}",
        creation.len(),
        order
            .iter()
            .map(|n| n.to_string())
            .collect::<Vec<_>>()
            .join(" ")
    );
    println!("pseudo  hard  var                     pref      order  costs");
    for p in &creation {
        let hard = assigned
            .get(p)
            .map(|h| {
                REGS.get(*h as usize)
                    .map(|r| (*r).to_string())
                    .unwrap_or_else(|| format!("h{h}"))
            })
            .unwrap_or_else(|| "-".into());
        let pos = order
            .iter()
            .position(|o| o == p)
            .map(|i| i.to_string())
            .unwrap_or_else(|| "-".into());
        println!(
            "{:<7} {:<5} {:<23} {:<9} {:<6} {}{}",
            p,
            hard,
            names.get(p).cloned().unwrap_or_default(),
            prefs.get(p).cloned().unwrap_or_default(),
            pos,
            costs.get(p).cloned().unwrap_or_default(),
            conflicts
                .get(p)
                .map(|c| format!("  conflicts: {c}"))
                .unwrap_or_default()
        );
    }
    if !spills.is_empty() {
        println!("spill insns: {}", spills.join(" "));
    }
    if !reloads.is_empty() {
        println!("reloads: {}", reloads.join("; "));
    }
    println!(
        "dumps kept in {} (cse=03, cse2=09, combine=13, lreg=17, greg=18, sched2=23)",
        work.display()
    );
    Ok(())
}

fn checked(program: &Path, args: &[String], cwd: &Path) -> Result<(), String> {
    let output = Command::new(program)
        .args(args)
        .current_dir(cwd)
        .output()
        .map_err(|e| format!("{}: {e}", program.display()))?;
    if output.status.success() {
        Ok(())
    } else {
        Err(format!(
            "{} failed:\n{}",
            program.display(),
            String::from_utf8_lossy(&output.stderr)
        ))
    }
}
