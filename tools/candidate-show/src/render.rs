use crate::{
    cli::Options,
    disasm::{disassemble, Rows},
    insns::gas_function_insns,
    jsparse::{js_parse_int_radix, pad_end, pad_start_zero, slice_utf16},
    patch::apply_unified_diff,
};
use candidate_compiler::{
    jsnum::to_js_number_string,
    verify::{compile_to_assembly, js_subarray, verify_candidate, ROM_BASE},
};
use compiler_core::routing::CompilerTarget;
use regex::Regex;
use serde_json::Value;
use sha2::{Digest, Sha256};
use std::{
    cmp::Ordering,
    collections::BTreeMap,
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
    let base = path
        .trim_end_matches('/')
        .rsplit('/')
        .next()
        .unwrap_or(path);
    base.strip_suffix(extension)
        .filter(|value| !value.is_empty())
        .unwrap_or(base)
}
fn region_size(root: &Path, stem: &str) -> Option<f64> {
    let path = ["out/full/asm/manifest.json", "out/asm/manifest.json"]
        .iter()
        .map(|path| root.join(path))
        .find(|path| path.exists())?;
    let document: Value = serde_json::from_str(&std::fs::read_to_string(path).ok()?).ok()?;
    document["regions"].as_array()?.iter().find_map(|region| {
        (basename_without(region["source"].as_str()?, ".s") == stem)
            .then(|| region["size"].as_f64())
            .flatten()
    })
}
pub fn render(root: &Path, options: &Options) -> Result<RenderOutput, String> {
    let work = options
        .work
        .as_deref()
        .ok_or("The \"path\" argument must be of type string. Received undefined")?;
    std::fs::create_dir_all(work).map_err(|error| format!("{work}: {error}"))?;
    if options.asm {
        return render_asm(root, options, work);
    }
    let rom_path = options
        .rom
        .as_deref()
        .ok_or("The \"path\" argument must be of type string. Received undefined")?;
    let patch_text = read_patch(options.patch.as_deref())?;
    let stem = basename_without(&options.source, ".c").to_string();
    let key = source_cache_key(&options.source, &options.flags, patch_text.as_deref())?;
    let work = Path::new(work);
    let key_path = work.join(format!("{stem}.key"));
    let candidate_path = work.join("candidate.bin");
    let reference_path = work.join("reference.bin");
    let first_path = work.join("first.txt");
    if options.first {
        if let Some(stdout) = cached_first(&key_path, &key, &first_path) {
            return Ok(RenderOutput {
                stdout,
                candidate_length: 0,
                reference_length: 0,
                differing_halfwords: 0,
                rows: 1,
            });
        }
    }
    let (actual, expected, compile) =
        if let Some(pair) = cached_bins(&key, &key_path, &candidate_path, &reference_path) {
            pair
        } else {
            let source = match patch_text.as_deref() {
                Some(patch) => {
                    let dest = work.join("try").join(format!("{stem}.c"));
                    apply_unified_diff(&options.source, patch, &dest)?;
                    dest.to_string_lossy().into_owned()
                }
                None => options.source.clone(),
            };
            let rom = std::fs::read(rom_path).map_err(|error| format!("{rom_path}: {error}"))?;
            let verification = verify_candidate(
                &source,
                &rom,
                work.to_string_lossy().as_ref(),
                &options.flags,
                ROM_BASE,
                CompilerTarget::Gs1,
                &options.configuration,
            )?;
            let address = js_parse_int_radix(&stem, 16);
            let linked_path = work.join(format!("{stem}.bin"));
            let linked = std::fs::read(&linked_path)
                .map_err(|error| format!("{}: {error}", linked_path.display()))?;
            let extent = nm_extent(
                work.join(format!("{stem}.elf")),
                &format!("Func_{stem}"),
                address,
                linked.len() as f64,
            )?
            .unwrap_or(verification.actual.len() as f64);
            let size = region_size(root, &stem).unwrap_or(linked.len() as f64);
            let actual = js_subarray(&linked, 0.0, extent);
            let expected = js_subarray(&rom, address - ROM_BASE, address - ROM_BASE + size);
            std::fs::write(&candidate_path, &actual)
                .map_err(|error| format!("{}: {error}", candidate_path.display()))?;
            std::fs::write(&reference_path, &expected)
                .map_err(|error| format!("{}: {error}", reference_path.display()))?;
            std::fs::write(&key_path, key.as_bytes())
                .map_err(|error| format!("{}: {error}", key_path.display()))?;
            let _ = std::fs::remove_file(&first_path);
            (actual, expected, "fresh")
        };
    render_bytes(
        actual,
        expected,
        compile,
        options,
        &candidate_path,
        &reference_path,
        &first_path,
    )
}
fn read_patch(path: Option<&str>) -> Result<Option<String>, String> {
    path.map(|path| {
        if path == "-" {
            let mut text = String::new();
            std::io::Read::read_to_string(&mut std::io::stdin(), &mut text)
                .map(|_| text)
                .map_err(|error| format!("stdin: {error}"))
        } else {
            std::fs::read_to_string(path).map_err(|error| format!("{path}: {error}"))
        }
    })
    .transpose()
}
fn nm_extent(
    path: PathBuf,
    target: &str,
    address: f64,
    length: f64,
) -> Result<Option<f64>, String> {
    let output = Command::new("arm-none-eabi-nm")
        .args(["-S", "--defined-only"])
        .arg(&path)
        .output()
        .map_err(|error| format!("arm-none-eabi-nm failed: {error}"))?;
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
            if hex.len() != 8
                || !hex
                    .bytes()
                    .all(|b| b.is_ascii_digit() || matches!(b, b'a'..=b'f'))
            {
                return None;
            }
            Some((
                js_parse_int_radix(fields[0], 16),
                js_parse_int_radix(fields[1], 16),
                name,
            ))
        })
        .filter(|(entry, _, _)| *entry >= address && *entry < address + length)
        .collect();
    if !entries
        .iter()
        .any(|(entry, _, name)| *entry == address && *name == target)
        || entries.iter().any(|(entry, size, _)| {
            !entry.is_finite()
                || !size.is_finite()
                || entry.fract() != 0.0
                || size.fract() != 0.0
                || *entry > 9_007_199_254_740_991.0
                || *size > 9_007_199_254_740_991.0
                || *size <= 0.0
        })
    {
        return Err("compiled function symbols differ".into());
    }
    let end = entries
        .iter()
        .map(|(entry, size, _)| entry + size)
        .fold(f64::NEG_INFINITY, f64::max);
    if end <= address || end - address > length {
        return Err("compiled function extent differs".into());
    }
    Ok(Some(end - address))
}
fn render_bytes(
    actual: Vec<u8>,
    expected: Vec<u8>,
    compile: &str,
    options: &Options,
    candidate_path: &Path,
    reference_path: &Path,
    first_path: &Path,
) -> Result<RenderOutput, String> {
    let left = disassemble(&candidate_path.to_string_lossy(), 0.0)?;
    let right = disassemble(&reference_path.to_string_lossy(), 0.0)?;
    let differing = crate::diff::differing_offsets(&actual, &expected);
    let mut offsets: Vec<_> = left
        .keys()
        .chain(right.keys())
        .fold(Vec::new(), |mut keys, key| {
            if !keys.contains(&key) {
                keys.push(key);
            }
            keys
        });
    offsets.sort_by(js_cmp);
    let ordered = |rows: &Rows| {
        let mut keys: Vec<_> = rows.keys().collect();
        keys.sort_by(js_cmp);
        keys.into_iter()
            .map(|key| rows.get(key).unwrap_or("").to_string())
            .collect::<Vec<_>>()
    };
    let (left_lines, right_lines) = (ordered(&left), ordered(&right));
    let (class, wrong) = residual_class(&left_lines, &right_lines);
    let mut out = format!("candidate={} reference={} differing_halfwords={}\ncompile={compile}\nclass={class} wrong_instructions={wrong}\n", n(actual.len())?, n(expected.len())?, n(differing.len())?);
    if options.align {
        let pairs = align_streams(&left_lines, &right_lines);
        let matched = first_residual_index(&pairs);
        out.push_str(&format!("matched_prefix={}\n", n(matched)?));
        let start = if options.first { matched } else { 0 };
        let end = if options.first {
            pairs.len().min(matched + 48)
        } else {
            pairs.len()
        };
        if options.first {
            out.push_str(&format!(
                "showing={} omitted={}\n",
                n(end - start)?,
                n(pairs.len() - end)?
            ));
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
            out.push_str(&format!(
                "  {mark} {} {}\n",
                slice_utf16(&pad_end(candidate.as_deref().unwrap_or(""), 30), 30),
                reference.as_deref().unwrap_or("")
            ));
        }
        if options.first {
            std::fs::write(first_path, &out)
                .map_err(|error| format!("{}: {error}", first_path.display()))?;
        }
    } else {
        if actual.len() != expected.len() {
            out.push_str("  note: the two sides are different lengths, so the offset view below is\n             phase-shifted and every later row will read as a difference.\n             Re-run with --align to see the insertion or deletion itself.\n");
        }
        out.push_str("      offset  candidate                      reference\n");
        for offset in offsets {
            let mark = if differing.contains(&(offset as usize)) {
                "!"
            } else {
                " "
            };
            out.push_str(&format!(
                "  {mark} {}  {} {}\n",
                pad_start_zero(&format!("{:x}", offset as u64), 4),
                slice_utf16(&pad_end(left.get(offset).unwrap_or(""), 30), 30),
                right.get(offset).unwrap_or("")
            ));
        }
    }
    Ok(RenderOutput {
        stdout: out,
        candidate_length: actual.len(),
        reference_length: expected.len(),
        differing_halfwords: differing.len(),
        rows: left.len().max(right.len()),
    })
}
fn render_asm(root: &Path, options: &Options, work: &str) -> Result<RenderOutput, String> {
    let started = Instant::now();
    let stem = basename_without(&options.source, ".c").to_string();
    let reference = root.join("asm").join(format!("{stem}.s"));
    if !reference.is_file() {
        return Err(format!(
            "--asm expects a main-image owner with {}",
            reference.display()
        ));
    }
    let source = match read_patch(options.patch.as_deref())? {
        Some(patch) => {
            let dest = Path::new(work).join("try").join(format!("{stem}.c"));
            apply_unified_diff(&options.source, &patch, &dest)?;
            dest.to_string_lossy().into_owned()
        }
        None => options.source.clone(),
    };
    let assembly = compile_to_assembly(
        &source,
        &options.source,
        work,
        &options.flags,
        CompilerTarget::Gs1,
        &options.configuration,
    )?;
    let symbol = format!("Func_{stem}");
    let candidate = gas_function_insns(
        &std::fs::read_to_string(&assembly).map_err(|error| format!("{assembly}: {error}"))?,
        &symbol,
    );
    let expected = gas_function_insns(
        &std::fs::read_to_string(&reference)
            .map_err(|error| format!("{}: {error}", reference.display()))?,
        &symbol,
    );
    let dir = Path::new(work);
    let candidate_path = dir.join("candidate.insns");
    let previous = dir.join("previous.insns");
    let had_previous = candidate_path.is_file();
    if had_previous {
        let _ = std::fs::rename(&candidate_path, &previous);
    }
    std::fs::write(&candidate_path, candidate.join("\n") + "\n")
        .map_err(|error| format!("{}: {error}", candidate_path.display()))?;
    let reference_path = dir.join("reference.insns");
    std::fs::write(&reference_path, expected.join("\n") + "\n")
        .map_err(|error| format!("{}: {error}", reference_path.display()))?;
    let mut out = format!(
        "elapsed_ms={:.0} compile=s-only\ncandidate_insns={} reference_insns={}\nvs reference:\n{}",
        started.elapsed().as_secs_f64() * 1000.0,
        candidate.len(),
        expected.len(),
        git_diff_stat(&reference_path, &candidate_path)?
    );
    if had_previous {
        out.push_str(&format!(
            "vs previous candidate:\n{}",
            git_diff_stat(&previous, &candidate_path)?
        ));
    }
    Ok(RenderOutput {
        stdout: out,
        candidate_length: candidate.len(),
        reference_length: expected.len(),
        differing_halfwords: 0,
        rows: 1,
    })
}
fn git_diff_stat(old: &Path, new: &Path) -> Result<String, String> {
    let output = Command::new("git")
        .args([
            "diff",
            "--no-index",
            "--stat",
            "--stat-width=80",
            &old.to_string_lossy(),
            &new.to_string_lossy(),
        ])
        .output()
        .map_err(|error| format!("git diff: {error}"))?;
    Ok(if output.stdout.is_empty() {
        "  identical\n".into()
    } else {
        String::from_utf8_lossy(&output.stdout).into_owned()
    })
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
        let end = out[start..]
            .find(']')
            .map_or(out.len() - start, |end| end + 1);
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
        return if left == right {
            ("exact", 0)
        } else {
            ("ordering", 0)
        };
    }
    if right.iter().any(|line| multiple(line)) && !left.iter().any(|line| multiple(line)) {
        return ("unemittable", wrong);
    }
    if count(|line| without_register(&without_pc_offset(line)))
        .values()
        .all(|count| *count == 0)
    {
        ("allocation", wrong)
    } else {
        ("wrong", wrong)
    }
}
fn multiple(line: &str) -> bool {
    line.split(|c: char| !c.is_ascii_alphanumeric())
        .any(|word| matches!(word, "stmia" | "ldmia" | "stmdb" | "ldmdb" | "stm" | "ldm"))
}
pub fn without_register(instruction: &str) -> String {
    static REG: std::sync::OnceLock<Regex> = std::sync::OnceLock::new();
    REG.get_or_init(|| Regex::new(r"(?i)\b(?:r(?:1[0-2]|[0-9])|fp|ip|sl)\b").unwrap())
        .replace_all(instruction, "R")
        .into_owned()
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
    let (a, b): (Vec<_>, Vec<_>) = (
        left.iter().map(|line| alignment_key(line)).collect(),
        right.iter().map(|line| alignment_key(line)).collect(),
    );
    let mut table = vec![vec![0; b.len() + 1]; a.len() + 1];
    for i in (0..a.len()).rev() {
        for j in (0..b.len()).rev() {
            table[i][j] = if a[i] == b[j] {
                table[i + 1][j + 1] + 1
            } else {
                table[i + 1][j].max(table[i][j + 1])
            };
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
fn source_cache_key(source: &str, flags: &[String], patch: Option<&str>) -> Result<String, String> {
    let mut hasher = Sha256::new();
    hasher.update(std::fs::read(source).map_err(|error| format!("{source}: {error}"))?);
    for flag in flags {
        hasher.update([0]);
        hasher.update(flag.as_bytes());
    }
    if let Some(patch) = patch {
        hasher.update([1]);
        hasher.update(patch.as_bytes());
    }
    Ok(hasher
        .finalize()
        .iter()
        .map(|byte| format!("{byte:02x}"))
        .collect())
}
fn cached_first(key_path: &Path, key: &str, report: &Path) -> Option<String> {
    (std::fs::read_to_string(key_path).ok()?.trim() == key)
        .then(|| std::fs::read_to_string(report).ok())
        .flatten()
        .filter(|text| !text.is_empty())
        .map(|text| text.replacen("compile=fresh\n", "compile=cache\n", 1))
}
fn cached_bins(
    key: &str,
    key_path: &Path,
    candidate: &Path,
    reference: &Path,
) -> Option<(Vec<u8>, Vec<u8>, &'static str)> {
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
