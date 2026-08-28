use crate::{
    cli::Options,
    disasm::{disassemble, Rows},
    insns::gas_function_insns,
    patch::apply_unified_diff_in_tree,
    triage::classify,
};
use candidate_compiler::verify::{
    compile_to_assembly, verify_candidate_owned_routed_with_object, CandidateCompilerConfiguration,
    CandidateCompilerFamily, ROM_BASE,
};
use compiler_core::bundle::compiler_bundle_signature_checked;
use compiler_core::routing::CompilerTarget;
use compiler_core::source_inputs::source_tree_signature;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use overlay_disasm::OVERLAY_BASE;
use regex::Regex;
use serde_json::Value;
use sha2::{Digest, Sha256};
use std::{
    path::{Path, PathBuf},
    process::Command,
    time::Instant,
};
pub struct RenderOutput {
    pub stdout: String,
    pub candidate_length: usize,
    pub reference_length: usize,
    pub differing_halfwords: usize,
    pub allocator: Option<crate::allocator::Report>,
    pub residual: crate::triage::ResidualReport,
}
fn main_source_identity(
    root: &Path,
    source: &str,
    target: CompilerTarget,
    selected: Option<u32>,
    overlay: Option<&str>,
) -> Result<(SourceOwner, PathBuf), String> {
    let path = Path::new(source);
    if let Some(address) = selected {
        let owner = match overlay {
            Some(overlay) => SourceOwner::parse(&format!("{overlay}:{address:08x}"))?,
            None => SourceOwner::Main(address),
        };
        let route = match owner {
            SourceOwner::Main(_) if target == CompilerTarget::Gs2 => {
                Path::new("games/gs2/src").join(owner.legacy_relative_path())
            }
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
    let owner = owner.or_else(|| {
        path.file_stem()
            .and_then(|stem| stem.to_str())
            .and_then(SourceOwner::from_legacy_stem)
    });
    let owner = owner.ok_or_else(|| {
        format!(
            "no {} source owner registered for {source}",
            target.as_str().to_ascii_uppercase()
        )
    })?;
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
fn region_size(root: &Path, address: u32) -> Option<usize> {
    for manifest in [
        "out/gs1-en/full/claimed/manifest.json",
        "out/gs1-en/claimed/manifest.json",
        "out/gs1-en/full/asm/manifest.json",
        "out/gs1-en/asm/manifest.json",
    ] {
        let size = std::fs::read_to_string(root.join(manifest))
            .ok()
            .and_then(|text| serde_json::from_str::<Value>(&text).ok())
            .and_then(|document| {
                document["regions"]
                    .as_array()?
                    .iter()
                    .find(|region| region["address"].as_u64() == Some(u64::from(address)))?["size"]
                    .as_u64()
                    .and_then(|size| usize::try_from(size).ok())
            });
        let Some(size) = size else {
            continue;
        };
        return Some(size);
    }
    None
}
pub fn render(root: &Path, options: &Options) -> Result<RenderOutput, String> {
    let work = root.join(
        options
            .work
            .as_deref()
            .ok_or("The \"path\" argument must be of type string. Received undefined")?,
    );
    std::fs::create_dir_all(&work).map_err(|error| format!("{}: {error}", work.display()))?;
    if options.asm {
        return render_asm(root, options, work.to_string_lossy().as_ref());
    }
    let rom_path = options
        .rom
        .as_deref()
        .ok_or("The \"path\" argument must be of type string. Received undefined")?;
    let patch_text = read_patch(options.patch.as_deref())?;
    let (owner, routing_source) = main_source_identity(
        root,
        &options.source,
        options.target,
        options.owner,
        options.overlay.as_deref(),
    )?;
    let image_base = if owner.is_main() {
        ROM_BASE as u32
    } else {
        OVERLAY_BASE as u32
    };
    let stem = owner.address_stem();
    let key = source_cache_key(
        &options.source,
        &routing_source.to_string_lossy(),
        &stem,
        &options.flags,
        &options.configuration,
        options.rom.as_deref(),
        options.size,
        patch_text.as_deref(),
    )?;
    let cache = cache_entry::sqlite::SqliteCache::open(&work.join("cache.sqlite3"))?;
    let candidate_path = work.join("candidate.bin");
    let reference_path = work.join("reference.bin");
    if options.first && !options.allocator_order {
        if let Some(stdout) = cached_first(&cache, &key) {
            return Ok(RenderOutput {
                stdout,
                candidate_length: 0,
                reference_length: 0,
                differing_halfwords: 0,
                allocator: None,
                residual: classify(&[], &[], 0, 0, 0),
            });
        }
    }
    let cached = (!options.allocator_order)
        .then(|| cached_bins(&cache, &key))
        .flatten();
    let source = if options.allocator_order || cached.is_none() {
        match patch_text.as_deref() {
            Some(patch) => {
                apply_unified_diff_in_tree(root, &options.source, patch, &work.join("try"))?
                    .to_string_lossy()
                    .into_owned()
            }
            None => options.source.clone(),
        }
    } else {
        options.source.clone()
    };
    let (actual, expected, compile) = if let Some(pair) = cached {
        pair
    } else {
        let rom = std::fs::read(rom_path).map_err(|error| format!("{rom_path}: {error}"))?;
        let verification = verify_candidate_owned_routed_with_object(
            &source,
            &routing_source.to_string_lossy(),
            &stem,
            &rom,
            work.to_string_lossy().as_ref(),
            &options.flags,
            f64::from(image_base),
            options.target,
            &options.configuration,
            options.precompiled_object.as_deref(),
        )?;
        let size = options.size.or_else(|| region_size(root, owner.address())).ok_or_else(|| {
                format!(
                    "no owner-size entry for {stem} in the claimed or asm build manifests -- pass `--size BYTES` for an independently established owner boundary, or run `make build-claimed` (or `make build-full`) before scoring against the ROM. Falling back to the candidate's own linked length would compare the source against itself."
                )
            })?;
        let offset = owner
            .address()
            .checked_sub(image_base)
            .ok_or_else(|| format!("{stem} precedes its image base"))?
            as usize;
        let end = offset.saturating_add(size).min(rom.len());
        let actual = verification.actual;
        let expected = rom[offset.min(end)..end].to_vec();
        if !options.allocator_order {
            cache
                .put(&key, &[("candidate", &actual), ("reference", &expected)])
                .map_err(|error| format!("cache: {error}"))?;
        }
        (
            actual,
            expected,
            if options.precompiled_object.is_some() {
                "shared-object"
            } else {
                "fresh"
            },
        )
    };
    std::fs::write(&candidate_path, &actual)
        .map_err(|error| format!("{}: {error}", candidate_path.display()))?;
    std::fs::write(&reference_path, &expected)
        .map_err(|error| format!("{}: {error}", reference_path.display()))?;
    let mut rendered = render_bytes(
        actual,
        expected,
        compile,
        options,
        &candidate_path,
        &reference_path,
        &cache,
        &key,
    )?;
    if options.allocator_order {
        let report =
            crate::allocator::decode(root, options, &routing_source, &stem, &source, &work)?;
        rendered.stdout.push_str(&report.text);
        rendered.residual = rendered
            .residual
            .with_decoder_coverage(report.repair.is_some());
        rendered.stdout.push_str(&format!(
            "triage_final={} playbook={}\n",
            rendered.residual.class.label(),
            rendered.residual.class.playbook().unwrap_or("smart-queue")
        ));
        rendered.allocator = Some(report);
    }
    Ok(rendered)
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
fn render_bytes(
    actual: Vec<u8>,
    expected: Vec<u8>,
    compile: &str,
    options: &Options,
    candidate_path: &Path,
    reference_path: &Path,
    cache: &cache_entry::sqlite::SqliteCache,
    key: &str,
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
    offsets.sort_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal));
    let (left_lines, right_lines) = (ordered_lines(&left), ordered_lines(&right));
    let residual = classify(
        &left_lines,
        &right_lines,
        actual.len(),
        expected.len(),
        differing.len(),
    );
    let wrong = residual.wrong_instructions;
    let playbook = residual.class.playbook().unwrap_or("smart-queue");
    let mut out = format!("candidate={} reference={} differing_halfwords={}\ncompile={compile}\nclass={} wrong_instructions={wrong}\ntriage={} playbook={playbook}\n", actual.len(), expected.len(), differing.len(), residual.class.label(), residual.class.label());
    if options.align {
        let pairs = align_streams(&left_lines, &right_lines);
        let matched = pairs
            .iter()
            .position(|(left, right)| left != right)
            .unwrap_or(pairs.len());
        out.push_str(&format!("matched_prefix={matched}\n"));
        let start = if options.first { matched } else { 0 };
        let end = if options.first {
            pairs.len().min(matched + 48)
        } else {
            pairs.len()
        };
        if options.first {
            out.push_str(&format!(
                "showing={} omitted={}\n",
                end - start,
                pairs.len() - end
            ));
        }
        out.push_str("      candidate                      reference\n");
        out.push_str(&side_by_side(&pairs[start..end]));
        if options.first {
            cache
                .upsert(key, "first", out.as_bytes())
                .map_err(|error| format!("cache: {error}"))?;
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
                "  {mark} {:04x}  {:<30.30} {}\n",
                offset as u64,
                left.get(offset).unwrap_or(""),
                right.get(offset).unwrap_or("")
            ));
        }
    }
    Ok(RenderOutput {
        stdout: out,
        candidate_length: actual.len(),
        reference_length: expected.len(),
        differing_halfwords: differing.len(),
        allocator: None,
        residual,
    })
}
fn render_asm(root: &Path, options: &Options, work: &str) -> Result<RenderOutput, String> {
    let started = Instant::now();
    let (owner, routing_source) = main_source_identity(
        root,
        &options.source,
        options.target,
        options.owner,
        options.overlay.as_deref(),
    )?;
    let stem = owner.address_stem();
    let reference = root
        .join("games")
        .join(options.target.as_str())
        .join("asm")
        .join(format!("{stem}.s"));
    if !reference.is_file() {
        return Err(format!(
            "--asm expects a main-image owner with {}",
            reference.display()
        ));
    }
    let source = match read_patch(options.patch.as_deref())? {
        Some(patch) => {
            let dest = apply_unified_diff_in_tree(
                root,
                &options.source,
                &patch,
                &Path::new(work).join("try"),
            )?;
            dest.to_string_lossy().into_owned()
        }
        None => options.source.clone(),
    };
    let assembly = compile_to_assembly(
        &source,
        &routing_source.to_string_lossy(),
        work,
        &options.flags,
        options.target,
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
        allocator: None,
        residual: classify(
            &candidate,
            &expected,
            candidate.len(),
            expected.len(),
            usize::from(candidate != expected),
        ),
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
    let registered = without_register(instruction);
    let text = registered.split('@').next().unwrap_or(&registered);
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
pub fn ordered_lines(rows: &Rows) -> Vec<String> {
    let mut keys: Vec<_> = rows.keys().collect();
    keys.sort_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal));
    keys.into_iter()
        .map(|key| rows.get(key).unwrap_or("").to_string())
        .collect()
}
pub fn side_by_side(pairs: &[(Option<String>, Option<String>)]) -> String {
    pairs
        .iter()
        .map(|(candidate, reference)| {
            let mark = if candidate == reference {
                " "
            } else if candidate.is_none() {
                "-"
            } else if reference.is_none() {
                "+"
            } else {
                "!"
            };
            let candidate = candidate.as_deref().unwrap_or("");
            let reference = reference.as_deref().unwrap_or("");
            format!("  {mark} {candidate:<30.30} {reference}\n")
        })
        .collect()
}
pub fn without_register(instruction: &str) -> String {
    static REG: std::sync::OnceLock<Regex> = std::sync::OnceLock::new();
    REG.get_or_init(|| Regex::new(r"(?i)\b(?:r(?:1[0-2]|[0-9])|fp|ip|sl)\b").unwrap())
        .replace_all(instruction, "R")
        .into_owned()
}
pub fn align_streams(left: &[String], right: &[String]) -> Vec<(Option<String>, Option<String>)> {
    crate::triage::alignment_indices(left, right, |left, right| {
        usize::from(alignment_key(left) == alignment_key(right))
    })
    .into_iter()
    .map(|(left_index, right_index)| {
        (
            left_index.map(|index| left[index].clone()),
            right_index.map(|index| right[index].clone()),
        )
    })
    .collect()
}
fn source_cache_key(
    source: &str,
    routing_source: &str,
    owner_stem: &str,
    flags: &[String],
    configuration: &CandidateCompilerConfiguration,
    rom: Option<&str>,
    size: Option<usize>,
    patch: Option<&str>,
) -> Result<String, String> {
    let executable = std::env::current_exe()
        .map_err(|error| format!("cannot resolve the candidate-show executable: {error}"))?;
    let executable =
        std::fs::read(&executable).map_err(|error| format!("{}: {error}", executable.display()))?;
    let bundle = compiler_bundle_signature_checked()?;
    source_cache_key_with_environment(
        source,
        routing_source,
        owner_stem,
        flags,
        configuration,
        rom,
        size,
        patch,
        &executable,
        bundle.as_bytes(),
    )
}
fn source_cache_key_with_environment(
    source: &str,
    routing_source: &str,
    owner_stem: &str,
    flags: &[String],
    configuration: &CandidateCompilerConfiguration,
    rom: Option<&str>,
    size: Option<usize>,
    patch: Option<&str>,
    executable: &[u8],
    compiler_bundle: &[u8],
) -> Result<String, String> {
    let mut hasher = Sha256::new();
    hasher.update(b"candidate-show-cache-v4");
    hasher.update(source_input_signature(
        compiler_core::routing::root(),
        source,
        routing_source,
        flags,
    )?);
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
    hasher.update(
        configuration
            .family
            .unwrap_or(CandidateCompilerFamily::Routed)
            .as_str()
            .as_bytes(),
    );
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
    hasher.update(
        configuration
            .call_via_base
            .unwrap_or_default()
            .to_le_bytes(),
    );
    hasher.update(
        configuration
            .label_word_bias
            .unwrap_or_default()
            .to_le_bytes(),
    );
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
    Ok(compiler_core::sha256::hex(&hasher.finalize()))
}
fn source_input_signature(
    root: &Path,
    source: &str,
    routing_source: &str,
    flags: &[String],
) -> Result<Vec<u8>, String> {
    let source = root.join(source);
    let include_dirs = routing_source
        .strip_prefix("games/")
        .and_then(|path| path.split('/').next())
        .map(|game| root.join("games").join(game).join("include"))
        .into_iter()
        .chain(flags.iter().filter_map(|flag| {
            let path = Path::new(flag.strip_prefix("-I").filter(|path| !path.is_empty())?);
            Some(
                path.is_absolute()
                    .then(|| path.into())
                    .unwrap_or_else(|| root.join(path)),
            )
        }))
        .collect::<Vec<_>>();
    source_tree_signature(&source, &include_dirs)
}
#[cfg(test)]
mod cache_key_tests {
    use super::*;
    #[test]
    fn compiler_route_flags_host_and_bundle_are_cache_identity() {
        let source = std::env::temp_dir().join("candidate-show-cache-key.c");
        std::fs::write(&source, "void Func_08000000(void) {}\n").unwrap();
        let source = source.to_str().unwrap();
        let routed = CandidateCompilerConfiguration {
            family: Some(CandidateCompilerFamily::Routed),
            ..Default::default()
        };
        let gcc296 = CandidateCompilerConfiguration {
            family: Some(CandidateCompilerFamily::Gcc296),
            ..Default::default()
        };
        let removed = CandidateCompilerConfiguration {
            family: Some(CandidateCompilerFamily::Routed),
            remove_flags: vec!["-fgcse".into()],
            ..Default::default()
        };
        let key = |route, owner, configuration, host: &[u8], bundle: &[u8]| {
            source_cache_key_with_environment(
                source,
                route,
                owner,
                &[],
                configuration,
                None,
                None,
                None,
                host,
                bundle,
            )
            .unwrap()
        };
        let route = "games/gs1/src/08000000.c";
        let base = key(route, "08000000", &routed, b"host-a", b"bundle-a");
        for changed in [
            key(route, "08000000", &gcc296, b"host-a", b"bundle-a"),
            key(route, "08000000", &removed, b"host-a", b"bundle-a"),
            key(
                "games/gs1/recon/en/main/08000000.c",
                "08000000",
                &routed,
                b"host-a",
                b"bundle-a",
            ),
            key(route, "08000004", &routed, b"host-a", b"bundle-a"),
            key(route, "08000000", &routed, b"host-b", b"bundle-a"),
            key(route, "08000000", &routed, b"host-a", b"bundle-b"),
        ] {
            assert_ne!(base, changed);
        }
        let _ = std::fs::remove_file(source);
    }
}
fn cached_first(cache: &cache_entry::sqlite::SqliteCache, key: &str) -> Option<String> {
    let entries = cache.get(key).ok().flatten()?;
    let text = String::from_utf8(entries.into_iter().find(|(kind, _)| kind == "first")?.1).ok()?;
    (!text.is_empty()).then(|| text.replacen("compile=fresh\n", "compile=cache\n", 1))
}
fn cached_bins(
    cache: &cache_entry::sqlite::SqliteCache,
    key: &str,
) -> Option<(Vec<u8>, Vec<u8>, &'static str)> {
    let entries = cache.get(key).ok().flatten()?;
    let find = |kind: &str| {
        entries
            .iter()
            .find(|(entry_kind, _)| entry_kind == kind)
            .map(|(_, value)| value.clone())
    };
    let actual = find("candidate")?;
    let expected = find("reference")?;
    (!actual.is_empty() && !expected.is_empty()).then_some((actual, expected, "cache"))
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
    fn owner_size_requires_a_matching_generated_manifest_region() {
        let root = scratch_root("lookup");
        assert_eq!(region_size(&root, 0x080a_b5e4), None);
        fs::write(
            root.join("out/gs1-en/asm/manifest.json"),
            r#"{"regions":[{"address":134986508,"size":6332}]}"#,
        )
        .unwrap();
        assert_eq!(region_size(&root, 0x080a_b5e4), None);
        fs::create_dir_all(root.join("out/gs1-en/claimed")).unwrap();
        fs::write(
            root.join("out/gs1-en/claimed/manifest.json"),
            r#"{"regions":[{"address":134942628,"size":296}]}"#,
        )
        .unwrap();
        assert_eq!(region_size(&root, 0x080b_0fa4), Some(296));
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
    fn source_identity_uses_manifest_and_stable_routes() {
        let root = scratch_root("routes");
        fs::write(
            root.join("games/gs1/source-paths.json"),
            r#"{"format":3,"owners":{"main:080b0fa4":"battle/inventory/draw_paged_item_list.c"}}"#,
        )
        .unwrap();
        let (owner, route) = main_source_identity(
            &root,
            "games/gs1/src/battle/inventory/draw_paged_item_list.c",
            CompilerTarget::Gs1,
            None,
            None,
        )
        .unwrap();
        assert_eq!(owner, SourceOwner::Main(0x080b0fa4));
        assert_eq!(route, PathBuf::from("games/gs1/src/080b0fa4.c"));
        let source = "games/gs1/recon/en/main/080ab5e4.c";
        let (owner, route) =
            main_source_identity(&root, source, CompilerTarget::Gs1, None, None).unwrap();
        assert_eq!(owner, SourceOwner::Main(0x080ab5e4));
        assert_eq!(route, PathBuf::from(source));
        let (_, route) = main_source_identity(
            &root,
            "candidate.c",
            CompilerTarget::Gs1,
            Some(0x080a8904),
            None,
        )
        .unwrap();
        assert_eq!(route, PathBuf::from("games/gs1/src/080a8904.c"));
        let _ = fs::remove_dir_all(&root);
    }
}
