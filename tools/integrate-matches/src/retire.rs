//! Retire exact main-image C back to retained assembly.
//!
//!   check retire <main:08xxxxxx | 08xxxxxx> [...] [--apply]
//!
//! The reverse of `check integrate`: the owner's pre-adoption
//! `games/gs1/asm/<addr>.s` comes back from git history, its C moves to the
//! EN reconstruction corpus (`games/gs1/recon/en/main/<addr>.c`) as a draft,
//! the register drops the source path (keeping a registered name), and a
//! declared translation unit that lists the owner marks it retained and
//! includes the draft instead of the production file. Without `--apply` the
//! plan is printed. A source shared with other owners is copied, not moved.

use compiler_core::source_paths::{SourceOwner, SourcePaths};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;

const USAGE: &str = "usage: check retire <main:08xxxxxx | 08xxxxxx> [...] [--apply]";

struct Plan {
    owner: SourceOwner,
    stem: String,
    source: PathBuf,
    shared: bool,
    draft: PathBuf,
    assembly: PathBuf,
    assembly_commit: String,
    unit: Option<String>,
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    match run(arguments) {
        Ok(lines) => {
            for line in lines {
                println!("{line}");
            }
            std::process::ExitCode::SUCCESS
        }
        Err(error) => {
            eprintln!("{error}");
            std::process::ExitCode::FAILURE
        }
    }
}

fn run(arguments: &[String]) -> Result<Vec<String>, String> {
    if arguments.is_empty() || arguments.iter().any(|a| a == "-h" || a == "--help") {
        return Ok(vec![USAGE.to_string()]);
    }
    let apply = arguments.iter().any(|a| a == "--apply");
    let root = compiler_core::routing::root();
    let paths = SourcePaths::load(root)?;
    // `--assembly-only`: rewrite an already-retired owner's retained
    // assembly from the ROM over its audited extent (the register and the
    // corpus are left alone).
    if arguments.iter().any(|a| a == "--assembly-only") {
        let mut lines = Vec::new();
        for argument in arguments.iter().filter(|a| !a.starts_with("--")) {
            let id = if argument.contains(':') {
                argument.clone()
            } else {
                format!("main:{argument}")
            };
            let owner = SourceOwner::parse(&id)?;
            let text = synthesize_assembly(root, owner)?;
            let assembly = root
                .join("games/gs1/asm")
                .join(format!("{}.s", owner.address_stem()));
            if apply {
                fs::write(&assembly, text.as_bytes())
                    .map_err(|error| format!("{}: {error}", assembly.display()))?;
                if let Some(name) = paths.registered_name(owner) {
                    label_assembly_with_name(&assembly, name)?;
                }
            }
            lines.push(format!(
                "assembly {} {} bytes of listing{}",
                owner.id(),
                text.len(),
                if apply { "" } else { " (dry run)" }
            ));
        }
        return Ok(lines);
    }
    let mut plans = Vec::new();
    let mut lines = Vec::new();
    for argument in arguments.iter().filter(|a| !a.starts_with("--")) {
        let id = if argument.contains(':') {
            argument.clone()
        } else {
            format!("main:{argument}")
        };
        let owner = SourceOwner::parse(&id)?;
        if !owner.is_main() {
            return Err(format!(
                "{id}: overlay owners retire through `overlay park <owner> --apply`"
            ));
        }
        match plan(root, &paths, owner) {
            Ok(plan) => plans.push(plan),
            Err(error) => lines.push(format!("skip {error}")),
        }
    }
    for plan in &plans {
        lines.push(format!(
            "retire {} source={} {} draft={} assembly={} from {}{}",
            plan.owner.id(),
            plan.source
                .strip_prefix(root)
                .unwrap_or(&plan.source)
                .display(),
            if plan.shared {
                "(shared: copy)"
            } else {
                "(move)"
            },
            plan.draft
                .strip_prefix(root)
                .unwrap_or(&plan.draft)
                .display(),
            plan.assembly
                .strip_prefix(root)
                .unwrap_or(&plan.assembly)
                .display(),
            plan.assembly_commit.chars().take(12).collect::<String>(),
            plan.unit
                .as_ref()
                .map(|unit| format!(" unit={unit}"))
                .unwrap_or_default()
        ));
    }
    if !apply {
        lines.push(format!(
            "{} owners planned (dry run; add --apply)",
            plans.len()
        ));
        return Ok(lines);
    }
    for plan in &plans {
        apply_plan(root, &paths, plan)?;
    }
    lines.push(format!("{} owners retired", plans.len()));
    Ok(lines)
}

/// The production source: the registered path, or for a declared unit
/// member the file the inventory says the unit composes for it.
fn production_source(
    root: &Path,
    paths: &SourcePaths,
    owner: SourceOwner,
) -> Result<PathBuf, String> {
    if let Some(path) = paths.mapped_source_path(owner) {
        return Ok(path);
    }
    let path = root.join("out/gs1-en/full/rebuilt.owner-inventory.json");
    let text = fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let value: serde_json::Value =
        serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))?;
    value["owners"]
        .as_array()
        .into_iter()
        .flatten()
        .find(|entry| entry["id"].as_str() == Some(&owner.id()))
        .and_then(|entry| entry["production"]["source"].as_str())
        .filter(|source| source.ends_with(".c"))
        .map(|source| root.join(source))
        .ok_or_else(|| format!("{}: no registered or composed source to retire", owner.id()))
}

fn plan(root: &Path, paths: &SourcePaths, owner: SourceOwner) -> Result<Plan, String> {
    let stem = owner.address_stem();
    let source = production_source(root, paths, owner)?;
    if !source.exists() {
        return Err(format!(
            "{}: registered source {} is absent",
            owner.id(),
            source.display()
        ));
    }
    let shared = paths
        .owners_for_path(&source)
        .into_iter()
        .any(|other| other != owner);
    let assembly = root.join("games/gs1/asm").join(format!("{stem}.s"));
    let unit = unit_of(root, owner)?;
    // A declared unit keeps its members' assembly beside the composite, so
    // a unit member only changes state; a standalone owner gets its
    // pre-adoption listing back from history when a commit held it, and
    // owners adopted before the assembly tree existed get a fresh
    // disassembly of their audited extent, verified to reassemble to the
    // ROM's bytes before anything is written.
    let assembly_commit = if assembly.exists() {
        if unit.is_none() {
            return Err(format!(
                "{}: {} already exists",
                owner.id(),
                assembly.display()
            ));
        }
        "present".to_string()
    } else {
        match deleting_commit(root, &format!("games/gs1/asm/{stem}.s")) {
            Ok(commit) => commit,
            Err(_) => {
                let text = synthesize_assembly(root, owner)?;
                format!("synthesized:{}", text.len())
            }
        }
    };
    let draft = root
        .join("games/gs1/recon/en/main")
        .join(format!("{stem}.c"));
    Ok(Plan {
        owner,
        stem,
        source,
        shared,
        draft,
        assembly,
        assembly_commit,
        unit,
    })
}

/// The owner's audited extent: everything up to the next owner or retained
/// region in the last full build's inventory. A linked function's symbol
/// size stops before its literal pool and alignment tail, which the owner
/// still owns, so the gap to the next start is the honest extent.
fn audited_extent(root: &Path, owner: SourceOwner) -> Result<usize, String> {
    let path = root.join("out/gs1-en/full/rebuilt.owner-inventory.json");
    let text = fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let value: serde_json::Value =
        serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))?;
    let parse = |entry: &serde_json::Value| {
        entry["address"]
            .as_str()
            .and_then(|a| u32::from_str_radix(a.trim_start_matches("0x"), 16).ok())
    };
    let mut starts: Vec<u32> = value["owners"]
        .as_array()
        .into_iter()
        .flatten()
        .filter(|entry| entry["container"]["kind"] == "main-rom")
        .filter_map(parse)
        .chain(
            value["auxiliary_main_assembly_regions"]
                .as_array()
                .into_iter()
                .flatten()
                .filter_map(parse),
        )
        .collect();
    starts.sort_unstable();
    let address = owner.address();
    starts
        .into_iter()
        .find(|start| *start > address)
        .map(|next| (next - address) as usize)
        .ok_or_else(|| format!("{}: no owner follows it in the inventory", owner.id()))
}

/// Retained assembly for the owner's extent, disassembled from the ROM in
/// the repository's main-image style (`Func_` symbols, `.L_` labels, pool
/// words as `.4byte`) and proved to reassemble byte-exactly.
fn synthesize_assembly(root: &Path, owner: SourceOwner) -> Result<String, String> {
    let extent = audited_extent(root, owner)?;
    let rom_path = root.join("roms/gs1-en.gba");
    let rom = fs::read(&rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    let address = owner.address() as usize;
    let start = address - 0x0800_0000;
    let bytes = rom
        .get(start..start + extent)
        .ok_or_else(|| format!("{}: extent runs past the ROM", owner.id()))?;
    let raw = overlay_disasm::build_region_source(bytes, address as i64)?;
    let set_line = regex::Regex::new(r"(?m)^\t\.set sub_[0-9a-f]{8}, 0x[0-9a-f]{8}\n").unwrap();
    let symbol = regex::Regex::new(r"\bsub_([0-9a-f]{8})\b").unwrap();
    let global = regex::Regex::new(r"(?m)^\t\.global Overlay_([0-9a-f]{8})$").unwrap();
    let label = regex::Regex::new(r"(?m)^Overlay_([0-9a-f]{8}):$").unwrap();
    let mut text = set_line.replace_all(&raw, "").into_owned();
    text = symbol.replace_all(&text, "Func_$1").into_owned();
    text = global
        .replace_all(&text, "\t.global Func_$1\n\t.thumb_func")
        .into_owned();
    text = label.replace_all(&text, "Func_$1:").into_owned();
    let header = format!(
        "@ Retired from C: the registered source did not reproduce these bytes\n@ under the family's flags. Disassembled from the audited extent at\n@ 0x{:08x} ({extent} bytes); the draft C stays in games/gs1/recon/en/main/.\n",
        owner.address()
    );
    let text = format!("{header}{text}");
    verify_assembly(root, &text, address, bytes)?;
    Ok(text)
}

/// Assemble the text with absolute stubs for every `Func_` it references and
/// compare the linked bytes with the ROM's.
fn verify_assembly(root: &Path, text: &str, address: usize, expected: &[u8]) -> Result<(), String> {
    let work = tempfile::tempdir().map_err(|error| error.to_string())?;
    let source = work.path().join("region.s");
    fs::write(&source, text).map_err(|error| error.to_string())?;
    let referenced = regex::Regex::new(r"\bFunc_([0-9a-f]{8})\b").unwrap();
    let mut stubs = String::new();
    let own = format!("{address:08x}");
    let mut seen = std::collections::BTreeSet::new();
    for capture in referenced.captures_iter(text) {
        let hex = capture[1].to_string();
        if hex == own || !seen.insert(hex.clone()) {
            continue;
        }
        stubs.push_str(&format!(
            "\t.global Func_{hex}\n\t.thumb_set Func_{hex}, 0x{hex}\n"
        ));
    }
    let stubs_path = work.path().join("stubs.s");
    fs::write(&stubs_path, stubs).map_err(|error| error.to_string())?;
    let run = |program: &str, args: &[&str]| -> Result<(), String> {
        let output = Command::new(program)
            .current_dir(work.path())
            .args(args)
            .output()
            .map_err(|error| format!("{program}: {error}"))?;
        if output.status.success() {
            Ok(())
        } else {
            Err(format!(
                "{program} failed: {}",
                String::from_utf8_lossy(&output.stderr).trim()
            ))
        }
    };
    run(
        "arm-none-eabi-as",
        &[
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "region.s",
            "-o",
            "region.o",
        ],
    )?;
    run(
        "arm-none-eabi-as",
        &[
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "stubs.s",
            "-o",
            "stubs.o",
        ],
    )?;
    run(
        "arm-none-eabi-ld",
        &[
            &format!("-Ttext=0x{address:08x}"),
            "-o",
            "region.elf",
            "region.o",
            "stubs.o",
        ],
    )?;
    run(
        "arm-none-eabi-objcopy",
        &["-O", "binary", "-j", ".text", "region.elf", "region.bin"],
    )?;
    let built = fs::read(work.path().join("region.bin")).map_err(|error| error.to_string())?;
    if built != expected {
        return Err(format!(
            "synthesized assembly for 0x{address:08x} reassembles to {} bytes that differ from the ROM's {}",
            built.len(),
            expected.len()
        ));
    }
    let _ = root;
    Ok(())
}

/// The newest commit at which the retained assembly still exists: the last
/// commit touching the path if it holds the file, else that commit's parent
/// (the adoption that deleted it).
fn deleting_commit(root: &Path, relative: &str) -> Result<String, String> {
    let output = Command::new("git")
        .current_dir(root)
        .args(["rev-list", "-n", "1", "--all", "--", relative])
        .output()
        .map_err(|error| format!("git rev-list: {error}"))?;
    let commit = String::from_utf8_lossy(&output.stdout).trim().to_string();
    if commit.is_empty() {
        return Err(format!(
            "{relative}: no commit ever held it, nothing to restore"
        ));
    }
    if git_show(root, &format!("{commit}:{relative}")).is_ok() {
        return Ok(commit);
    }
    let parent = format!("{commit}^");
    git_show(root, &format!("{parent}:{relative}"))
        .map(|_| parent)
        .map_err(|error| format!("{relative}: {error}"))
}

fn git_show(root: &Path, spec: &str) -> Result<Vec<u8>, String> {
    let output = Command::new("git")
        .current_dir(root)
        .args(["show", spec])
        .output()
        .map_err(|error| format!("git show: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "git show {spec}: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    Ok(output.stdout)
}

fn unit_of(root: &Path, owner: SourceOwner) -> Result<Option<String>, String> {
    let path = root.join("games/gs1/recon/translation-units.json");
    let text = fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let value: serde_json::Value =
        serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))?;
    for unit in value["units"].as_array().into_iter().flatten() {
        if unit["overlay"].is_string() {
            continue;
        }
        for member in unit["owners"].as_array().into_iter().flatten() {
            let address = member["address"]
                .as_str()
                .and_then(|a| u32::from_str_radix(a.trim_start_matches("0x"), 16).ok());
            if address == Some(owner.address()) {
                return Ok(unit["id"].as_str().map(str::to_string));
            }
        }
    }
    Ok(None)
}

fn apply_plan(root: &Path, paths: &SourcePaths, plan: &Plan) -> Result<(), String> {
    if plan.assembly_commit != "present" {
        let assembly = if plan.assembly_commit.starts_with("synthesized:") {
            synthesize_assembly(root, plan.owner)?.into_bytes()
        } else {
            git_show(
                root,
                &format!("{}:games/gs1/asm/{}.s", plan.assembly_commit, plan.stem),
            )?
        };
        fs::write(&plan.assembly, assembly)
            .map_err(|error| format!("{}: {error}", plan.assembly.display()))?;
    }
    if let Some(parent) = plan.draft.parent() {
        fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    if plan.shared {
        fs::copy(&plan.source, &plan.draft)
            .map_err(|error| format!("{}: {error}", plan.draft.display()))?;
    } else {
        fs::rename(&plan.source, &plan.draft)
            .map_err(|error| format!("{}: {error}", plan.draft.display()))?;
        remove_empty_parents(root.join("games/gs1/src"), plan.source.parent());
    }
    let explicit_name = paths.registered_name(plan.owner).map(str::to_string);
    paths.unregister_owner(plan.owner)?;
    match explicit_name {
        // A plain path entry has no name of its own: `unregister_owner`
        // would coin one from the file stem, which out of its directory says
        // nothing ("create", "update"). Retire it nameless instead.
        None => drop_register_entry(root, plan.owner)?,
        Some(name) => label_assembly_with_name(&plan.assembly, &name)?,
    }
    if let Some(unit) = &plan.unit {
        mark_unit_member_retained(root, unit, plan)?;
    }
    Ok(())
}

fn drop_register_entry(root: &Path, owner: SourceOwner) -> Result<(), String> {
    let manifest = root.join("games/gs1/source-paths.json");
    let text = fs::read_to_string(&manifest)
        .map_err(|error| format!("{}: {error}", manifest.display()))?;
    let mut value: serde_json::Value =
        serde_json::from_str(&text).map_err(|error| format!("{}: {error}", manifest.display()))?;
    if let Some(owners) = value
        .get_mut("owners")
        .and_then(serde_json::Value::as_object_mut)
    {
        owners.remove(&owner.id());
    }
    let mut rendered = serde_json::to_string_pretty(&value).map_err(|error| error.to_string())?;
    rendered.push('\n');
    fs::write(&manifest, rendered).map_err(|error| format!("{}: {error}", manifest.display()))
}

/// Named retained owners declare both labels, the name and the `Func_`
/// alias, on the same address; add the name when the listing lacks it.
fn label_assembly_with_name(assembly: &Path, name: &str) -> Result<(), String> {
    let text =
        fs::read_to_string(assembly).map_err(|error| format!("{}: {error}", assembly.display()))?;
    if text.contains(&format!("\n{name}:")) {
        return Ok(());
    }
    let global = regex::Regex::new(r"(?m)^\t\.global (Func_[0-9a-f]{8})$").unwrap();
    let label = regex::Regex::new(r"(?m)^(Func_[0-9a-f]{8}):$").unwrap();
    let updated = global.replace(&text, format!("\t.global {name}\n\t.global $1"));
    let updated = label.replace(&updated, format!("{name}:\n$1:"));
    fs::write(assembly, updated.as_ref())
        .map_err(|error| format!("{}: {error}", assembly.display()))
}

fn remove_empty_parents(stop: PathBuf, mut directory: Option<&Path>) {
    while let Some(dir) = directory {
        if dir == stop || !dir.starts_with(&stop) {
            break;
        }
        if fs::read_dir(dir)
            .map(|mut d| d.next().is_none())
            .unwrap_or(false)
        {
            let _ = fs::remove_dir(dir);
            directory = dir.parent();
        } else {
            break;
        }
    }
}

/// The unit keeps the owner as a retained member whose draft is included
/// from the reconstruction corpus, the composition `unit-scaffold` documents.
fn mark_unit_member_retained(root: &Path, unit_id: &str, plan: &Plan) -> Result<(), String> {
    let manifest = root.join("games/gs1/recon/translation-units.json");
    let text = fs::read_to_string(&manifest)
        .map_err(|error| format!("{}: {error}", manifest.display()))?;
    let mut value: serde_json::Value =
        serde_json::from_str(&text).map_err(|error| format!("{}: {error}", manifest.display()))?;
    let mut source_file = None;
    let mut exact_left = 0;
    for unit in value["units"].as_array_mut().into_iter().flatten() {
        if unit["id"].as_str() != Some(unit_id) {
            continue;
        }
        source_file = unit["source"].as_str().map(|s| root.join(s));
        for member in unit["owners"].as_array_mut().into_iter().flatten() {
            let address = member["address"]
                .as_str()
                .and_then(|a| u32::from_str_radix(a.trim_start_matches("0x"), 16).ok());
            if address == Some(plan.owner.address()) {
                member["state"] = serde_json::Value::String("retained-assembly".into());
            }
            if member["state"] == "exact-c" {
                exact_left += 1;
            }
        }
    }
    // A unit with no exact member left is not a translation unit any more:
    // its owners are plain retained assembly with drafts in the corpus.
    if exact_left == 0 {
        if let Some(units) = value["units"].as_array_mut() {
            units.retain(|unit| unit["id"].as_str() != Some(unit_id));
        }
        let mut rendered =
            serde_json::to_string_pretty(&value).map_err(|error| error.to_string())?;
        rendered.push('\n');
        fs::write(&manifest, rendered)
            .map_err(|error| format!("{}: {error}", manifest.display()))?;
        if let Some(composite) = source_file {
            if composite.exists() {
                let _ = fs::remove_file(composite);
            }
        }
        return Ok(());
    }
    let mut rendered = serde_json::to_string_pretty(&value).map_err(|error| error.to_string())?;
    rendered.push('\n');
    fs::write(&manifest, rendered).map_err(|error| format!("{}: {error}", manifest.display()))?;
    let Some(composite) = source_file else {
        return Ok(());
    };
    if !composite.exists() {
        return Ok(());
    }
    let text = fs::read_to_string(&composite)
        .map_err(|error| format!("{}: {error}", composite.display()))?;
    let relative = plan
        .source
        .strip_prefix(root.join("games/gs1"))
        .map(|p| p.to_string_lossy().into_owned())
        .unwrap_or_default();
    let updated: Vec<String> = text
        .lines()
        .map(|line| {
            let trimmed = line.trim();
            if trimmed.starts_with("#include \"")
                && !relative.is_empty()
                && trimmed.contains(&relative)
            {
                format!("#include \"../main/{}.c\"", plan.stem)
            } else {
                line.to_string()
            }
        })
        .collect();
    fs::write(&composite, format!("{}\n", updated.join("\n")))
        .map_err(|error| format!("{}: {error}", composite.display()))
}
