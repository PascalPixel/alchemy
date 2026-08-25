use crate::{
    cli::Options,
    disasm::{disassemble, Rows},
    insns::gas_function_insns,
    jsparse::{js_parse_int_radix, pad_end, pad_start_zero, slice_utf16},
    patch::apply_unified_diff,
};
use candidate_compiler::{
    jsnum::to_js_number_string,
    verify::{compile_to_assembly, js_subarray, verify_candidate_owned_routed_with_object, CandidateCompilerConfiguration, CandidateCompilerFamily, ROM_BASE},
};
use compiler_core::bundle::compiler_bundle_signature_checked;
use compiler_core::routing::CompilerTarget;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use overlay_disasm::OVERLAY_BASE;
use regex::Regex;
use serde_json::Value;
use sha2::{Digest, Sha256};
use std::{
    cmp::Ordering,
    collections::{BTreeMap, BTreeSet},
    path::{Path, PathBuf},
    process::Command,
    time::Instant,
};
pub struct RenderOutput {
    pub stdout: String,
    pub candidate_length: usize,
    pub reference_length: usize,
    pub differing_halfwords: usize,
    pub rows: usize,
}
fn basename_without<'a>(path: &'a str, extension: &str) -> &'a str {
    let base = path.trim_end_matches('/').rsplit('/').next().unwrap_or(path);
    base.strip_suffix(extension).filter(|value| !value.is_empty()).unwrap_or(base)
}

fn main_source_identity(root: &Path, source: &str, target: CompilerTarget, selected: Option<u32>, overlay: Option<&str>) -> Result<(SourceOwner, PathBuf), String> {
    let path = Path::new(source);
    if let Some(address) = selected {
        let owner = match overlay {
            Some(overlay) => SourceOwner::parse(&format!("{overlay}:{address:08x}"))?,
            None => SourceOwner::Main(address),
        };
        let route = match owner {
            SourceOwner::Main(_) if target == CompilerTarget::Gs2 => Path::new("games/gs2/src").join(owner.legacy_relative_path()),
            SourceOwner::Main(_) => owner.routing_path(),
            SourceOwner::Overlay { .. } => owner.routing_path_for_game(target.as_str()),
        };
        return Ok((owner, route));
    }
    let paths = SourcePaths::load_for_game(root, target.as_str())?;
    let mut owner = paths.owner_for_path(path)?;
    if owner.is_none() && !path.is_absolute() {
        owner = paths.owner_for_path(&root.join(path))?;
    }
    let routed_by_manifest = owner.is_some();
    let owner = owner.or_else(|| path.file_stem().and_then(|stem| stem.to_str()).and_then(SourceOwner::from_legacy_stem));
    let owner = owner.ok_or_else(|| format!("no {} source owner registered for {source}", target.as_str().to_ascii_uppercase()))?;
    match owner {
        SourceOwner::Main(_) => {
            let routing = if target == CompilerTarget::Gs2 {
                Path::new("games/gs2/src").join(owner.legacy_relative_path())
            } else if routed_by_manifest {
                owner.routing_path()
            } else {
                path.to_path_buf()
            };
            Ok((owner, routing))
        }
        SourceOwner::Overlay { .. } => Ok((owner, owner.routing_path_for_game(target.as_str()))),
    }
}
fn region_size(root: &Path, address: u32) -> Option<f64> {
    let manifests = ["out/gs1-en/full/claimed/manifest.json", "out/gs1-en/claimed/manifest.json", "out/gs1-en/full/asm/manifest.json", "out/gs1-en/asm/manifest.json"];
    for manifest in manifests {
        let Ok(text) = std::fs::read_to_string(root.join(manifest)) else {
            continue;
        };
        let Ok(document) = serde_json::from_str::<Value>(&text) else {
            continue;
        };
        let Some(regions) = document["regions"].as_array() else {
            continue;
        };
        let size = regions.iter().find_map(|region| (region["address"].as_u64() == Some(u64::from(address))).then(|| region["size"].as_f64()).flatten());
        if size.is_some() {
            return size;
        }
    }
    None
}
pub fn render(root: &Path, options: &Options) -> Result<RenderOutput, String> {
    let work = options.work.as_deref().ok_or("The \"path\" argument must be of type string. Received undefined")?;
    std::fs::create_dir_all(work).map_err(|error| format!("{work}: {error}"))?;
    if options.asm {
        return render_asm(root, options, work);
    }
    let rom_path = options.rom.as_deref().ok_or("The \"path\" argument must be of type string. Received undefined")?;
    let patch_text = read_patch(options.patch.as_deref())?;
    let source_label = basename_without(&options.source, ".c").to_string();
    let (owner, routing_source) = main_source_identity(root, &options.source, options.target, options.owner, options.overlay.as_deref())?;
    let image_base = if owner.is_main() { ROM_BASE } else { OVERLAY_BASE as f64 };
    let stem = owner.address_stem();
    let key = source_cache_key(&options.source, &routing_source.to_string_lossy(), &stem, &options.flags, &options.configuration, options.rom.as_deref(), options.size, patch_text.as_deref())?;
    let work = Path::new(work);
    let key_path = work.join(format!("{source_label}.key"));
    let candidate_path = work.join("candidate.bin");
    let reference_path = work.join("reference.bin");
    let first_path = work.join("first.txt");
    if options.first {
        if let Some(stdout) = cached_first(&key_path, &key, &first_path) {
            return Ok(RenderOutput { stdout, candidate_length: 0, reference_length: 0, differing_halfwords: 0, rows: 1 });
        }
    }
    let (actual, expected, compile) = if let Some(pair) = cached_bins(&key, &key_path, &candidate_path, &reference_path) {
        pair
    } else {
        let source = match patch_text.as_deref() {
            Some(patch) => {
                let dest = work.join("try").join(format!("{source_label}.c"));
                apply_unified_diff(&options.source, patch, &dest)?;
                dest.to_string_lossy().into_owned()
            }
            None => options.source.clone(),
        };
        let rom = std::fs::read(rom_path).map_err(|error| format!("{rom_path}: {error}"))?;
        let verification = verify_candidate_owned_routed_with_object(&source, &routing_source.to_string_lossy(), &stem, &rom, work.to_string_lossy().as_ref(), &options.flags, image_base, options.target, &options.configuration, options.precompiled_object.as_deref())?;
        let address = js_parse_int_radix(&stem, 16);
        let linked_path = work.join(format!("{stem}.bin"));
        let linked = std::fs::read(&linked_path).map_err(|error| format!("{}: {error}", linked_path.display()))?;
        let size = options.size.map(|size| size as f64).or_else(|| region_size(root, owner.address())).ok_or_else(|| {
                format!(
                    "no owner-size entry for {stem} in the claimed or asm build manifests -- pass `--size BYTES` for an independently established owner boundary, or run `make build-claimed` (or `make build-full`) before scoring against the ROM. Falling back to the candidate's own linked length would compare the source against itself."
                )
            })?;
        let elf_path = work.join(format!("{stem}.elf"));
        let extent = nm_extent(elf_path.clone(), &format!("Func_{stem}"), address, size)?.unwrap_or(verification.actual.len() as f64);
        let text_start = nm_text_start(&elf_path)?.unwrap_or(address);
        let owner_offset = address - text_start;
        let actual = js_subarray(&linked, owner_offset, owner_offset + extent);
        let expected = js_subarray(&rom, address - image_base, address - image_base + size);
        std::fs::write(&candidate_path, &actual).map_err(|error| format!("{}: {error}", candidate_path.display()))?;
        std::fs::write(&reference_path, &expected).map_err(|error| format!("{}: {error}", reference_path.display()))?;
        std::fs::write(&key_path, key.as_bytes()).map_err(|error| format!("{}: {error}", key_path.display()))?;
        let _ = std::fs::remove_file(&first_path);
        (actual, expected, if options.precompiled_object.is_some() { "shared-object" } else { "fresh" })
    };
    render_bytes(actual, expected, compile, options, &candidate_path, &reference_path, &first_path)
}
fn read_patch(path: Option<&str>) -> Result<Option<String>, String> {
    path.map(|path| {
        if path == "-" {
            let mut text = String::new();
            std::io::Read::read_to_string(&mut std::io::stdin(), &mut text).map(|_| text).map_err(|error| format!("stdin: {error}"))
        } else {
            std::fs::read_to_string(path).map_err(|error| format!("{path}: {error}"))
        }
    })
    .transpose()
}
fn nm_extent(path: PathBuf, target: &str, address: f64, length: f64) -> Result<Option<f64>, String> {
    let output = Command::new("arm-none-eabi-nm").args(["-S", "--defined-only"]).arg(&path).output().map_err(|error| format!("arm-none-eabi-nm failed: {error}"))?;
    if !output.status.success() {
        return Ok(None);
    }
    let symbols = String::from_utf8_lossy(&output.stdout);
    let entries: Vec<_> = symbols
        .lines()
        .filter_map(|line| {
            let fields: Vec<_> = line.split_whitespace().collect();
            if fields.len() < 4 || !matches!(fields[fields.len() - 2], "T" | "t") {
                return None;
            }
            let name = fields[fields.len() - 1];
            let hex = name.strip_prefix("Func_")?;
            if !(hex.len() == 7 || hex.len() == 8) || !hex.bytes().all(|b| b.is_ascii_digit() || matches!(b, b'a'..=b'f')) {
                return None;
            }
            Some((js_parse_int_radix(fields[0], 16), js_parse_int_radix(fields[1], 16), name))
        })
        .filter(|(entry, _, _)| *entry >= address && *entry < address + length)
        .collect();
    if entries.iter().any(|(entry, size, _)| !entry.is_finite() || !size.is_finite() || entry.fract() != 0.0 || size.fract() != 0.0 || *entry > 9_007_199_254_740_991.0 || *size > 9_007_199_254_740_991.0 || *size <= 0.0) {
        return Err("compiled function symbols differ".into());
    }
    entries.iter().find(|(entry, _, name)| *entry == address && (*name == target || target.strip_prefix("Func_0") == name.strip_prefix("Func_"))).map(|(_, size, _)| Some(*size)).ok_or_else(|| "compiled function symbols differ".into())
}
fn nm_text_start(path: &Path) -> Result<Option<f64>, String> {
    let output = Command::new("arm-none-eabi-nm").args(["-S", "--defined-only"]).arg(path).output().map_err(|error| format!("arm-none-eabi-nm failed: {error}"))?;
    if !output.status.success() {
        return Ok(None);
    }
    Ok(String::from_utf8_lossy(&output.stdout)
        .lines()
        .filter_map(|line| {
            let fields: Vec<_> = line.split_whitespace().collect();
            (fields.len() >= 4 && matches!(fields[fields.len() - 2], "T" | "t")).then(|| js_parse_int_radix(fields[0], 16))
        })
        .filter(|address| address.is_finite())
        .min_by(|left, right| left.partial_cmp(right).unwrap_or(Ordering::Equal)))
}
fn render_bytes(actual: Vec<u8>, expected: Vec<u8>, compile: &str, options: &Options, candidate_path: &Path, reference_path: &Path, first_path: &Path) -> Result<RenderOutput, String> {
    let left = disassemble(&candidate_path.to_string_lossy(), 0.0)?;
    let right = disassemble(&reference_path.to_string_lossy(), 0.0)?;
    let differing = crate::diff::differing_offsets(&actual, &expected);
    let mut offsets: Vec<_> = left.keys().chain(right.keys()).fold(Vec::new(), |mut keys, key| {
        if !keys.contains(&key) {
            keys.push(key);
        }
        keys
    });
    offsets.sort_by(js_cmp);
    let ordered = |rows: &Rows| {
        let mut keys: Vec<_> = rows.keys().collect();
        keys.sort_by(js_cmp);
        keys.into_iter().map(|key| rows.get(key).unwrap_or("").to_string()).collect::<Vec<_>>()
    };
    let (left_lines, right_lines) = (ordered(&left), ordered(&right));
    let (class, wrong) = residual_class(&left_lines, &right_lines);
    let mut out = format!("candidate={} reference={} differing_halfwords={}\ncompile={compile}\nclass={class} wrong_instructions={wrong}\n", n(actual.len())?, n(expected.len())?, n(differing.len())?);
    if options.align {
        let pairs = align_streams(&left_lines, &right_lines);
        let matched = first_residual_index(&pairs);
        out.push_str(&format!("matched_prefix={}\n", n(matched)?));
        let start = if options.first { matched } else { 0 };
        let end = if options.first { pairs.len().min(matched + 48) } else { pairs.len() };
        if options.first {
            out.push_str(&format!("showing={} omitted={}\n", n(end - start)?, n(pairs.len() - end)?));
        }
        out.push_str("      candidate                      reference\n");
        for (candidate, reference) in &pairs[start..end] {
            let mark = match (candidate, reference) {
                (Some(a), Some(b)) if a == b => " ",
                (Some(_), Some(_)) => "!",
                (Some(_), None) => "+",
                (None, Some(_)) => "-",
                _ => " ",
            };
            out.push_str(&format!("  {mark} {} {}\n", slice_utf16(&pad_end(candidate.as_deref().unwrap_or(""), 30), 30), reference.as_deref().unwrap_or("")));
        }
        if options.first {
            std::fs::write(first_path, &out).map_err(|error| format!("{}: {error}", first_path.display()))?;
        }
    } else {
        if actual.len() != expected.len() {
            out.push_str("  note: the two sides are different lengths, so the offset view below is\n             phase-shifted and every later row will read as a difference.\n             Re-run with --align to see the insertion or deletion itself.\n");
        }
        out.push_str("      offset  candidate                      reference\n");
        for offset in offsets {
            let mark = if differing.contains(&(offset as usize)) { "!" } else { " " };
            out.push_str(&format!("  {mark} {}  {} {}\n", pad_start_zero(&format!("{:x}", offset as u64), 4), slice_utf16(&pad_end(left.get(offset).unwrap_or(""), 30), 30), right.get(offset).unwrap_or("")));
        }
    }
    Ok(RenderOutput { stdout: out, candidate_length: actual.len(), reference_length: expected.len(), differing_halfwords: differing.len(), rows: left.len().max(right.len()) })
}
fn render_asm(root: &Path, options: &Options, work: &str) -> Result<RenderOutput, String> {
    let started = Instant::now();
    let source_label = basename_without(&options.source, ".c").to_string();
    let (owner, routing_source) = main_source_identity(root, &options.source, options.target, options.owner, options.overlay.as_deref())?;
    let stem = owner.address_stem();
    let reference = root.join("games").join(options.target.as_str()).join("asm").join(format!("{stem}.s"));
    if !reference.is_file() {
        return Err(format!("--asm expects a main-image owner with {}", reference.display()));
    }
    let source = match read_patch(options.patch.as_deref())? {
        Some(patch) => {
            let dest = Path::new(work).join("try").join(format!("{source_label}.c"));
            apply_unified_diff(&options.source, &patch, &dest)?;
            dest.to_string_lossy().into_owned()
        }
        None => options.source.clone(),
    };
    let assembly = compile_to_assembly(&source, &routing_source.to_string_lossy(), work, &options.flags, options.target, &options.configuration)?;
    let symbol = format!("Func_{stem}");
    let candidate = gas_function_insns(&std::fs::read_to_string(&assembly).map_err(|error| format!("{assembly}: {error}"))?, &symbol);
    let expected = gas_function_insns(&std::fs::read_to_string(&reference).map_err(|error| format!("{}: {error}", reference.display()))?, &symbol);
    let dir = Path::new(work);
    let candidate_path = dir.join("candidate.insns");
    let previous = dir.join("previous.insns");
    let had_previous = candidate_path.is_file();
    if had_previous {
        let _ = std::fs::rename(&candidate_path, &previous);
    }
    std::fs::write(&candidate_path, candidate.join("\n") + "\n").map_err(|error| format!("{}: {error}", candidate_path.display()))?;
    let reference_path = dir.join("reference.insns");
    std::fs::write(&reference_path, expected.join("\n") + "\n").map_err(|error| format!("{}: {error}", reference_path.display()))?;
    let mut out = format!("elapsed_ms={:.0} compile=s-only\ncandidate_insns={} reference_insns={}\nvs reference:\n{}", started.elapsed().as_secs_f64() * 1000.0, candidate.len(), expected.len(), git_diff_stat(&reference_path, &candidate_path)?);
    if had_previous {
        out.push_str(&format!("vs previous candidate:\n{}", git_diff_stat(&previous, &candidate_path)?));
    }
    Ok(RenderOutput { stdout: out, candidate_length: candidate.len(), reference_length: expected.len(), differing_halfwords: 0, rows: 1 })
}
fn git_diff_stat(old: &Path, new: &Path) -> Result<String, String> {
    let output = Command::new("git").args(["diff", "--no-index", "--stat", "--stat-width=80", &old.to_string_lossy(), &new.to_string_lossy()]).output().map_err(|error| format!("git diff: {error}"))?;
    Ok(if output.stdout.is_empty() { "  identical\n".into() } else { String::from_utf8_lossy(&output.stdout).into_owned() })
}
pub fn alignment_key(instruction: &str) -> String {
    let text = instruction.split('@').next().unwrap_or(instruction);
    let mut out = String::new();
    let mut chars = text.chars().peekable();
    while let Some(c) = chars.next() {
        if c == '0' && chars.peek() == Some(&'x') {
            chars.next();
            out.push_str("0xN");
            while chars.peek().is_some_and(|c| c.is_ascii_hexdigit()) {
                chars.next();
            }
        } else if c.is_ascii_digit() {
            out.push('N');
            while chars.peek().is_some_and(char::is_ascii_digit) {
                chars.next();
            }
        } else {
            out.push(if c == '\t' { ' ' } else { c });
        }
    }
    out.split_whitespace().collect::<Vec<_>>().join(" ")
}
pub fn without_pc_offset(instruction: &str) -> String {
    let mut out = instruction.to_string();
    while let Some(start) = out.find("[pc, #") {
        let end = out[start..].find(']').map_or(out.len() - start, |end| end + 1);
        out = format!("{}[pc]{}", &out[..start], &out[start + end..]);
    }
    out
}
pub fn residual_class(left: &[String], right: &[String]) -> (&'static str, i64) {
    let count = |key: fn(&str) -> String| {
        let mut map = BTreeMap::new();
        for line in left {
            *map.entry(key(line)).or_insert(0i64) += 1;
        }
        for line in right {
            *map.entry(key(line)).or_insert(0i64) -= 1;
        }
        map
    };
    let pool = count(without_pc_offset);
    let wrong = pool.values().map(|count| count.abs()).sum();
    if wrong == 0 {
        return if left == right { ("exact", 0) } else { ("ordering", 0) };
    }
    if right.iter().any(|line| multiple(line)) && !left.iter().any(|line| multiple(line)) {
        return ("unemittable", wrong);
    }
    if count(|line| without_register(&without_pc_offset(line))).values().all(|count| *count == 0) {
        ("allocation", wrong)
    } else {
        ("wrong", wrong)
    }
}
fn multiple(line: &str) -> bool {
    line.split(|c: char| !c.is_ascii_alphanumeric()).any(|word| matches!(word, "stmia" | "ldmia" | "stmdb" | "ldmdb" | "stm" | "ldm"))
}
pub fn without_register(instruction: &str) -> String {
    static REG: std::sync::OnceLock<Regex> = std::sync::OnceLock::new();
    REG.get_or_init(|| Regex::new(r"(?i)\b(?:r(?:1[0-2]|[0-9])|fp|ip|sl)\b").unwrap()).replace_all(instruction, "R").into_owned()
}
pub fn first_residual_index(pairs: &[(Option<String>, Option<String>)]) -> usize {
    pairs
        .iter()
        .position(|(left, right)| match (left, right) {
            (Some(a), Some(b)) => a != b,
            (None, None) => false,
            _ => true,
        })
        .unwrap_or(pairs.len())
}
pub fn align_streams(left: &[String], right: &[String]) -> Vec<(Option<String>, Option<String>)> {
    let (a, b): (Vec<_>, Vec<_>) = (left.iter().map(|line| alignment_key(line)).collect(), right.iter().map(|line| alignment_key(line)).collect());
    let mut table = vec![vec![0; b.len() + 1]; a.len() + 1];
    for i in (0..a.len()).rev() {
        for j in (0..b.len()).rev() {
            table[i][j] = if a[i] == b[j] { table[i + 1][j + 1] + 1 } else { table[i + 1][j].max(table[i][j + 1]) };
        }
    }
    let mut out = Vec::new();
    let (mut i, mut j) = (0, 0);
    while i < a.len() && j < b.len() {
        if a[i] == b[j] {
            out.push((Some(left[i].clone()), Some(right[j].clone())));
            i += 1;
            j += 1;
        } else if table[i + 1][j] >= table[i][j + 1] {
            out.push((Some(left[i].clone()), None));
            i += 1;
        } else {
            out.push((None, Some(right[j].clone())));
            j += 1;
        }
    }
    while i < a.len() {
        out.push((Some(left[i].clone()), None));
        i += 1;
    }
    while j < b.len() {
        out.push((None, Some(right[j].clone())));
        j += 1;
    }
    out
}
fn source_cache_key(source: &str, routing_source: &str, owner_stem: &str, flags: &[String], configuration: &CandidateCompilerConfiguration, rom: Option<&str>, size: Option<usize>, patch: Option<&str>) -> Result<String, String> {
    let executable = std::env::current_exe().map_err(|error| format!("cannot resolve the candidate-show executable: {error}"))?;
    let executable = std::fs::read(&executable).map_err(|error| format!("{}: {error}", executable.display()))?;
    let bundle = compiler_bundle_signature_checked()?;
    source_cache_key_with_environment(source, routing_source, owner_stem, flags, configuration, rom, size, patch, &executable, bundle.as_bytes())
}

fn source_cache_key_with_environment(source: &str, routing_source: &str, owner_stem: &str, flags: &[String], configuration: &CandidateCompilerConfiguration, rom: Option<&str>, size: Option<usize>, patch: Option<&str>, executable: &[u8], compiler_bundle: &[u8]) -> Result<String, String> {
    let mut hasher = Sha256::new();
    hasher.update(b"candidate-show-cache-v4");
    hasher.update(source_input_signature(compiler_core::routing::root(), source, routing_source, flags)?);
    hasher.update([7]);
    hasher.update(routing_source.as_bytes());
    hasher.update([8]);
    hasher.update(owner_stem.as_bytes());
    hasher.update([5]);
    hasher.update(executable);
    hasher.update([6]);
    hasher.update(compiler_bundle);
    for flag in flags {
        hasher.update([0]);
        hasher.update(flag.as_bytes());
    }
    hasher.update([1]);
    hasher.update(configuration.family.unwrap_or(CandidateCompilerFamily::Routed).as_str().as_bytes());
    for flag in &configuration.add_flags {
        hasher.update([2]);
        hasher.update(flag.as_bytes());
    }
    for flag in &configuration.remove_flags {
        hasher.update([3]);
        hasher.update(flag.as_bytes());
    }
    hasher.update([9, u8::from(configuration.reference_symbols)]);
    for (name, symbol) in &configuration.absolute_symbols {
        hasher.update([12]);
        hasher.update(name.as_bytes());
        hasher.update(symbol.address.to_le_bytes());
        hasher.update([symbol.kind as u8]);
    }
    hasher.update(configuration.call_via_base.unwrap_or_default().to_le_bytes());
    hasher.update(configuration.label_word_bias.unwrap_or_default().to_le_bytes());
    if let Some(rom) = rom {
        hasher.update([10]);
        hasher.update(rom.as_bytes());
    }
    if let Some(size) = size {
        hasher.update([11]);
        hasher.update(size.to_le_bytes());
    }
    if let Some(patch) = patch {
        hasher.update([4]);
        hasher.update(patch.as_bytes());
    }
    Ok(hasher.finalize().iter().map(|byte| format!("{byte:02x}")).collect())
}

fn source_input_signature(root: &Path, source: &str, routing_source: &str, flags: &[String]) -> Result<Vec<u8>, String> {
    let source = rooted_path(root, Path::new(source));
    let mut include_dirs = Vec::new();
    if let Some(game) = routing_source.strip_prefix("games/").and_then(|path| path.split('/').next()) {
        include_dirs.push(root.join("games").join(game).join("include"));
    }
    include_dirs.extend(flags.iter().filter_map(|flag| flag.strip_prefix("-I").filter(|path| !path.is_empty()).map(|path| rooted_path(root, Path::new(path)))));

    let mut seen = BTreeSet::new();
    let mut inputs = Vec::new();
    collect_source_inputs(&source, &include_dirs, &mut seen, &mut inputs)?;
    let mut hasher = Sha256::new();
    for (path, bytes) in inputs {
        hasher.update(path.to_string_lossy().as_bytes());
        hasher.update([0]);
        hasher.update(bytes);
        hasher.update([0xff]);
    }
    Ok(hasher.finalize().to_vec())
}

fn rooted_path(root: &Path, path: &Path) -> PathBuf {
    if path.is_absolute() {
        path.to_path_buf()
    } else {
        root.join(path)
    }
}

fn collect_source_inputs(path: &Path, include_dirs: &[PathBuf], seen: &mut BTreeSet<PathBuf>, inputs: &mut Vec<(PathBuf, Vec<u8>)>) -> Result<(), String> {
    let canonical = std::fs::canonicalize(path).map_err(|error| format!("{}: {error}", path.display()))?;
    if !seen.insert(canonical.clone()) {
        return Ok(());
    }
    let bytes = std::fs::read(&canonical).map_err(|error| format!("{}: {error}", canonical.display()))?;
    let text = String::from_utf8_lossy(&bytes);
    let mut includes = Vec::new();
    for line in text.lines() {
        let Some(rest) = line.trim_start().strip_prefix("#include") else {
            continue;
        };
        let rest = rest.trim_start();
        let Some(rest) = rest.strip_prefix('"') else {
            continue;
        };
        let Some((include, _)) = rest.split_once('"') else {
            continue;
        };
        let include = Path::new(include);
        let local = canonical.parent().unwrap_or(Path::new("")).join(include);
        let resolved = std::iter::once(local).chain(include_dirs.iter().map(|directory| directory.join(include))).find(|candidate| candidate.is_file());
        if let Some(resolved) = resolved {
            includes.push(resolved);
        }
    }
    inputs.push((canonical, bytes));
    for include in includes {
        collect_source_inputs(&include, include_dirs, seen, inputs)?;
    }
    Ok(())
}

#[cfg(test)]
mod cache_key_tests {
    use super::*;

    #[test]
    fn compiler_family_and_flag_mutations_are_part_of_the_cache_identity() {
        let source = std::env::temp_dir().join("candidate-show-cache-key.c");
        std::fs::write(&source, "void Func_08000000(void) {}\n").unwrap();
        let source = source.to_str().unwrap();
        let routed = CandidateCompilerConfiguration { family: Some(CandidateCompilerFamily::Routed), ..Default::default() };
        let gcc296 = CandidateCompilerConfiguration { family: Some(CandidateCompilerFamily::Gcc296), ..Default::default() };
        let removed = CandidateCompilerConfiguration { family: Some(CandidateCompilerFamily::Routed), remove_flags: vec!["-fgcse".into()], ..Default::default() };
        let route = "games/gs1/src/08000000.c";
        let base = source_cache_key(source, route, "08000000", &[], &routed, None, None, None).unwrap();
        assert_ne!(base, source_cache_key(source, route, "08000000", &[], &gcc296, None, None, None).unwrap());
        assert_ne!(base, source_cache_key(source, route, "08000000", &[], &removed, None, None, None).unwrap());
        assert_ne!(base, source_cache_key(source, "games/gs1/recon/en/main/08000000.c", "08000000", &[], &routed, None, None, None).unwrap());
        assert_ne!(base, source_cache_key(source, route, "08000004", &[], &routed, None, None, None).unwrap());
        let _ = std::fs::remove_file(source);
    }

    #[test]
    fn host_and_compiler_bundle_are_part_of_the_cache_identity() {
        let source = std::env::temp_dir().join("candidate-show-environment-cache-key.c");
        std::fs::write(&source, "void Func_08000000(void) {}\n").unwrap();
        let source = source.to_str().unwrap();
        let configuration = CandidateCompilerConfiguration::default();
        let base = source_cache_key_with_environment(source, "games/gs1/src/08000000.c", "08000000", &[], &configuration, None, None, None, b"host-a", b"bundle-a").unwrap();
        assert_ne!(base, source_cache_key_with_environment(source, "games/gs1/src/08000000.c", "08000000", &[], &configuration, None, None, None, b"host-b", b"bundle-a",).unwrap());
        assert_ne!(base, source_cache_key_with_environment(source, "games/gs1/src/08000000.c", "08000000", &[], &configuration, None, None, None, b"host-a", b"bundle-b",).unwrap());
        let _ = std::fs::remove_file(source);
    }

    #[test]
    fn included_source_changes_are_part_of_the_cache_identity() {
        let root = std::env::temp_dir().join("candidate-show-include-cache-key");
        let _ = std::fs::remove_dir_all(&root);
        std::fs::create_dir_all(root.join("games/gs1/include")).unwrap();
        std::fs::create_dir_all(root.join("games/gs1/recon/en/main")).unwrap();
        let source = root.join("games/gs1/recon/en/main/08000000.c");
        let body = root.join("games/gs1/recon/en/main/body.c");
        std::fs::write(&source, "#include \"body.c\"\n").unwrap();
        std::fs::write(&body, "void Func_08000000(void) {}\n").unwrap();
        let first = source_input_signature(&root, source.to_str().unwrap(), "games/gs1/src/08000000.c", &[]).unwrap();
        std::fs::write(&body, "void Func_08000000(void) { for (;;) {} }\n").unwrap();
        let second = source_input_signature(&root, source.to_str().unwrap(), "games/gs1/src/08000000.c", &[]).unwrap();
        assert_ne!(first, second);
        let _ = std::fs::remove_dir_all(&root);
    }
}
fn cached_first(key_path: &Path, key: &str, report: &Path) -> Option<String> {
    (std::fs::read_to_string(key_path).ok()?.trim() == key).then(|| std::fs::read_to_string(report).ok()).flatten().filter(|text| !text.is_empty()).map(|text| text.replacen("compile=fresh\n", "compile=cache\n", 1))
}
fn cached_bins(key: &str, key_path: &Path, candidate: &Path, reference: &Path) -> Option<(Vec<u8>, Vec<u8>, &'static str)> {
    if std::fs::read_to_string(key_path).ok()?.trim() != key {
        return None;
    }
    let actual = std::fs::read(candidate).ok()?;
    let expected = std::fs::read(reference).ok()?;
    (!actual.is_empty() && !expected.is_empty()).then_some((actual, expected, "cache"))
}
fn n(value: usize) -> Result<String, String> {
    to_js_number_string(value as f64)
}
fn js_cmp(a: &f64, b: &f64) -> Ordering {
    a.partial_cmp(b).unwrap_or(Ordering::Equal)
}
#[cfg(test)]
mod region_size_tests {
    use super::*;
    use std::fs;

    fn scratch_root(name: &str) -> std::path::PathBuf {
        let dir = std::env::temp_dir().join(format!("candidate-show-region-size-test-{name}"));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(dir.join("out/gs1-en/asm")).unwrap();
        dir
    }

    #[test]
    fn reads_the_owner_size_from_the_generated_manifest() {
        let root = scratch_root("present");
        fs::write(root.join("out/gs1-en/asm/manifest.json"), r#"{"regions":[{"address":134919652,"size":4888}]}"#).unwrap();
        assert_eq!(region_size(&root, 0x080a_b5e4), Some(4888.0));
        let _ = fs::remove_dir_all(&root);
    }

    #[test]
    fn reads_a_nested_source_size_by_owner_address() {
        let root = scratch_root("claimed-nested");
        fs::create_dir_all(root.join("out/gs1-en/claimed")).unwrap();
        fs::write(root.join("out/gs1-en/claimed/manifest.json"), r#"{"regions":[{"address":134942628,"size":296}]}"#).unwrap();
        assert_eq!(region_size(&root, 0x080b_0fa4), Some(296.0));
        let _ = fs::remove_dir_all(&root);
    }

    #[test]
    fn returns_none_without_falling_back_when_the_manifest_is_absent() {
        let root = scratch_root("absent");
        assert_eq!(region_size(&root, 0x080a_b5e4), None);
        let _ = fs::remove_dir_all(&root);
    }

    #[test]
    fn returns_none_when_the_manifest_lacks_this_owner() {
        let root = scratch_root("other-owner");
        fs::write(root.join("out/gs1-en/asm/manifest.json"), r#"{"regions":[{"address":134986508,"size":6332}]}"#).unwrap();
        assert_eq!(region_size(&root, 0x080a_b5e4), None);
        let _ = fs::remove_dir_all(&root);
    }
}

#[cfg(test)]
mod source_identity_tests {
    use super::*;
    use std::fs;

    fn scratch_root(name: &str) -> PathBuf {
        let root = std::env::temp_dir().join(format!("candidate-show-source-identity-{name}"));
        let _ = fs::remove_dir_all(&root);
        fs::create_dir_all(root.join("games/gs1")).unwrap();
        root
    }

    #[test]
    fn nested_source_uses_manifest_owner_and_stable_route() {
        let root = scratch_root("nested");
        fs::write(root.join("games/gs1/source-paths.json"), r#"{"format":3,"owners":{"main:080b0fa4":"battle/inventory/draw_paged_item_list.c"}}"#).unwrap();
        let (owner, route) = main_source_identity(&root, "games/gs1/src/battle/inventory/draw_paged_item_list.c", CompilerTarget::Gs1, None, None).unwrap();
        assert_eq!(owner, SourceOwner::Main(0x080b0fa4));
        assert_eq!(route, PathBuf::from("games/gs1/src/080b0fa4.c"));
        let _ = fs::remove_dir_all(&root);
    }

    #[test]
    fn address_named_recon_candidate_keeps_its_legacy_route() {
        let root = scratch_root("recon");
        let source = "games/gs1/recon/en/main/080ab5e4.c";
        let (owner, route) = main_source_identity(&root, source, CompilerTarget::Gs1, None, None).unwrap();
        assert_eq!(owner, SourceOwner::Main(0x080ab5e4));
        assert_eq!(route, PathBuf::from(source));
        let _ = fs::remove_dir_all(&root);
    }

    #[test]
    fn explicit_owner_uses_its_stable_route() {
        let root = scratch_root("selected");
        let (_, route) = main_source_identity(&root, "candidate.c", CompilerTarget::Gs1, Some(0x080a8904), None).unwrap();
        assert_eq!(route, PathBuf::from("games/gs1/src/080a8904.c"));
        let _ = fs::remove_dir_all(&root);
    }
}
