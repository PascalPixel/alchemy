use crate::candidate::{
    compile_to_assembly, source_symbol_bindings, verify_candidate_owned_routed_with_object,
    CandidateCompilerConfiguration, ROM_BASE,
};
use crate::compiler::bundle::compiler_bundle_signature_checked;
use crate::compiler::routing::CompilerTarget;
use crate::compiler::source_inputs::source_tree_signature;
use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use crate::diff::{
    cli::Options,
    disasm::disassemble,
    patch::apply_unified_diff_in_tree,
    triage::{classify, classify_with_topology},
};
use crate::overlay::assembly::OVERLAY_BASE;
use psynergy::compare::{
    differing_offsets,
    insns::{align_streams, gas_function_insns},
    topology::{self, Comparison},
};
use serde_json::Value;
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
    pub allocator: Option<crate::diff::allocator::Report>,
    pub residual: crate::diff::triage::ResidualReport,
}

struct SourceIdentity {
    owner: SourceOwner,
    routing: PathBuf,
}

impl SourceIdentity {
    fn resolve(
        root: &Path,
        source: &str,
        target: CompilerTarget,
        selected: Option<u32>,
        overlay: Option<&str>,
    ) -> Result<Self, String> {
        let path = Path::new(source);
        let (owner, registered) = if let Some(address) = selected {
            (
                match overlay {
                    Some(overlay) => SourceOwner::parse(&format!("{overlay}:{address:08x}"))?,
                    None => SourceOwner::Main(address),
                },
                true,
            )
        } else {
            let paths = SourcePaths::load_for_game(root, target.as_str())?;
            let mut owner = paths.owner_for_path(path)?;
            if owner.is_none() && !path.is_absolute() {
                owner = paths.owner_for_path(&root.join(path))?;
            }
            let registered = owner.is_some();
            let owner = owner
                .or_else(|| {
                    path.file_stem()
                        .and_then(|stem| stem.to_str())
                        .and_then(SourceOwner::from_legacy_stem)
                })
                .ok_or_else(|| {
                    format!(
                        "no {} source owner registered for {source}",
                        target.as_str().to_ascii_uppercase()
                    )
                })?;
            (owner, registered)
        };
        let routing = match owner {
            SourceOwner::Main(_) if target == CompilerTarget::Gs2 => {
                Path::new("games/gs2/src").join(owner.legacy_relative_path())
            }
            SourceOwner::Main(_) if registered => owner.routing_path(),
            SourceOwner::Main(_) => path.to_path_buf(),
            SourceOwner::Overlay { .. } => owner.routing_path_for_game(target.as_str()),
        };
        Ok(Self { owner, routing })
    }

    fn stem(&self) -> String {
        self.owner.address_stem()
    }
}

fn staged_source(
    root: &Path,
    options: &Options,
    work: &Path,
    patch: Option<&str>,
) -> Result<String, String> {
    let Some(patch) = patch else {
        return Ok(options.source.clone());
    };
    apply_unified_diff_in_tree(root, &options.source, patch, &work.join("try"))
        .map(|path| path.to_string_lossy().into_owned())
}

struct RenderedScore {
    output: RenderOutput,
    candidate: Vec<String>,
    reference: Vec<String>,
}
pub fn region_size(root: &Path, address: u32) -> Option<usize> {
    for manifest in [
        "out/gs1-en/full/claimed/manifest.json",
        "out/gs1-en/claimed/manifest.json",
        "out/gs1-en/full/asm/manifest.json",
        "out/gs1-en/asm/manifest.json",
    ] {
        let Ok(document) = crate::compiler::build_io::read_json::<Value>(root.join(manifest))
        else {
            continue;
        };
        let Some(regions) = document["regions"].as_array() else {
            continue;
        };
        let size = regions.iter().find_map(|region| {
            (region["address"].as_u64() == Some(u64::from(address)))
                .then(|| usize::try_from(region["size"].as_u64()?).ok())
                .flatten()
        });
        if size.is_some() {
            return size;
        }
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
    let identity = SourceIdentity::resolve(
        root,
        &options.source,
        options.target,
        options.owner,
        options.overlay.as_deref(),
    )?;
    // With an explicit owner symbol the compare address (--owner) names a
    // different edition's location; compilation must still follow the source
    // file's own registered route, or the flags change and the codegen with
    // them.
    let identity = if options.configuration.owner_symbol.is_some() {
        let routed = SourceIdentity::resolve(
            root,
            &options.source,
            options.target,
            None,
            options.overlay.as_deref(),
        )?;
        SourceIdentity {
            owner: identity.owner,
            routing: routed.routing,
        }
    } else {
        identity
    };
    if options.asm {
        return render_asm(root, options, &work, &identity);
    }
    let rom_path = options
        .rom
        .as_deref()
        .ok_or("The \"path\" argument must be of type string. Received undefined")?;
    let rom = std::fs::read(rom_path).map_err(|error| format!("{rom_path}: {error}"))?;
    let patch_text = read_patch(options.patch.as_deref())?;
    let image_base = if identity.owner.is_main() {
        ROM_BASE as u32
    } else {
        OVERLAY_BASE as u32
    };
    let stem = identity.stem();
    let key = source_cache_key(
        &options.source,
        &identity.routing.to_string_lossy(),
        options.target,
        &stem,
        &options.configuration,
        rom_path,
        &rom,
        options.size,
        patch_text.as_deref(),
    )?;
    let cache = psynergy::cache::SqliteCache::open(&work.join("cache.sqlite3"))?;
    let cached = (!options.allocator_order)
        .then(|| cached_bins(&cache, &key))
        .flatten();
    let source = if options.allocator_order || cached.is_none() {
        staged_source(root, options, &work, patch_text.as_deref())?
    } else {
        options.source.clone()
    };
    let (actual, expected, candidate_gas, compile) = if let Some(pair) = cached {
        pair
    } else {
        let verification = verify_candidate_owned_routed_with_object(
            &source,
            &identity.routing.to_string_lossy(),
            &stem,
            &rom,
            work.to_string_lossy().as_ref(),
            &[],
            f64::from(image_base),
            options.target,
            &options.configuration,
            options.precompiled_object.as_deref(),
        )?;
        let size = options.size.or_else(|| region_size(root, identity.owner.address())).ok_or_else(|| {
                format!(
                    "no owner-size entry for {stem} in the claimed or asm build manifests -- pass `--size BYTES` for an independently established owner boundary, or run `make build-claimed` (or `make build-full`) before scoring against the ROM. Falling back to the candidate's own linked length would compare the source against itself."
                )
            })?;
        let offset = identity
            .owner
            .address()
            .checked_sub(image_base)
            .ok_or_else(|| format!("{stem} precedes its image base"))?
            as usize;
        let end = offset.saturating_add(size).min(rom.len());
        let (actual, expected) = if options.configuration.overlay_extent.is_some() {
            let runtime = crate::compiler::overlay::load(&rom, 0)?;
            (
                crate::compiler::overlay::load(&verification.actual, offset)?,
                runtime[offset.min(end)..end].to_vec(),
            )
        } else {
            (verification.actual, rom[offset.min(end)..end].to_vec())
        };
        let candidate_gas = read_candidate_gas(&work, &stem, options.precompiled_object.as_deref());
        if !options.allocator_order {
            let mut entries = vec![
                ("candidate", actual.as_slice()),
                ("reference", expected.as_slice()),
            ];
            if let Some(assembly) = candidate_gas.as_deref() {
                entries.push(("candidate-gas", assembly));
            }
            cache
                .put(&key, &entries)
                .map_err(|error| format!("cache: {error}"))?;
        }
        (
            actual,
            expected,
            candidate_gas,
            if options.precompiled_object.is_some() {
                "shared-object"
            } else {
                "fresh"
            },
        )
    };
    let topology = if actual == expected {
        Comparison::Equal
    } else {
        topology_for_owner(
            root,
            options,
            identity.owner.is_main(),
            &stem,
            candidate_gas.as_deref(),
            &expected,
        )
    };
    let mut score = render_bytes(actual, expected, compile, topology, options, &work)?;
    let allocator = if options.allocator_order {
        let report = crate::diff::allocator::decode(
            root,
            options,
            &identity.routing,
            &stem,
            &source,
            &work,
            &score.candidate,
            &score.reference,
        )?;
        Some(crate::diff::structure::augment(report, &work))
    } else {
        None
    };
    if let Some(report) = &allocator {
        score.output.residual = score
            .output
            .residual
            .with_decoder_coverage(report.repair.is_some());
    }
    let mut rendered = score.output;
    if let Some(report) = allocator {
        rendered.stdout.push_str(&report.text);
        rendered.stdout.push_str(&format!(
            "triage_final={} playbook={}\nnext={}\n",
            rendered.residual.class.label(),
            rendered.residual.class.playbook().unwrap_or("smart-queue"),
            rendered.residual.class.next_command()
        ));
        rendered.allocator = Some(report);
    }
    rendered.stdout = rendered
        .stdout
        .replace("{owner}", &identity.owner.id())
        .replace("{source}", &options.source);
    Ok(rendered)
}

fn read_candidate_gas(
    work: &Path,
    stem: &str,
    precompiled_object: Option<&str>,
) -> Option<Vec<u8>> {
    let path = precompiled_object
        .map(|object| Path::new(object).with_extension("s"))
        .unwrap_or_else(|| work.join(format!("{stem}.s")));
    std::fs::read(path).ok()
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

fn topology_for_owner(
    root: &Path,
    options: &Options,
    main: bool,
    stem: &str,
    candidate: Option<&[u8]>,
    expected: &[u8],
) -> Comparison {
    let Some(candidate) = candidate.and_then(|source| std::str::from_utf8(source).ok()) else {
        return Comparison::Uncovered("candidate-gas-unavailable".into());
    };
    let reference = root
        .join("games")
        .join(options.target.as_str())
        .join("asm")
        .join(format!("{stem}.s"));
    let candidate_symbol = format!("Func_{stem}");
    if !main || !reference.is_file() {
        let Some(address) = u32::from_str_radix(stem, 16).ok() else {
            return Comparison::Uncovered("invalid-owner-address".into());
        };
        let base = address
            + if main {
                0
            } else {
                crate::compiler::overlay::RUNTIME_BASE - crate::compiler::overlay::RESOURCE_BASE
            };
        return match crate::overlay::assembly::build_region_source(expected, i64::from(base)) {
            Ok(reference) => topology::compare_symbols_at(
                candidate,
                &candidate_symbol,
                &reference,
                &format!("Overlay_{base:08x}"),
                base,
            ),
            Err(error) => Comparison::Uncovered(format!("reference-decode:{error}")),
        };
    }
    let Ok(reference) = std::fs::read_to_string(&reference) else {
        return Comparison::Uncovered("reference-gas-unavailable".into());
    };
    let reference_symbol = u32::from_str_radix(stem, 16)
        .ok()
        .and_then(|address| {
            SourcePaths::load_for_game(root, options.target.as_str())
                .ok()
                .map(|paths| (paths, address))
        })
        .and_then(|(paths, address)| {
            paths
                .registered_name(SourceOwner::Main(address))
                .map(str::to_owned)
        })
        .unwrap_or_else(|| candidate_symbol.clone());
    topology::compare_symbols(candidate, &candidate_symbol, &reference, &reference_symbol)
}

fn render_bytes(
    actual: Vec<u8>,
    expected: Vec<u8>,
    compile: &str,
    topology: Comparison,
    options: &Options,
    work: &Path,
) -> Result<RenderedScore, String> {
    let candidate_path = work.join("candidate.bin");
    let reference_path = work.join("reference.bin");
    for (path, bytes) in [(&candidate_path, &actual), (&reference_path, &expected)] {
        std::fs::write(path, bytes).map_err(|error| format!("{}: {error}", path.display()))?;
    }
    let candidate_rows = disassemble(&candidate_path.to_string_lossy(), 0)?;
    let reference_rows = disassemble(&reference_path.to_string_lossy(), 0)?;
    let candidate: Vec<_> = candidate_rows.values().cloned().collect();
    let reference: Vec<_> = reference_rows.values().cloned().collect();
    let differing = differing_offsets(&actual, &expected, 2);
    let residual = classify_with_topology(
        &candidate,
        &reference,
        actual.len(),
        expected.len(),
        differing.len(),
        &topology,
    );
    let mut offsets: Vec<_> = candidate_rows.keys().chain(reference_rows.keys()).collect();
    offsets.sort_unstable();
    offsets.dedup();
    let wrong = residual.wrong_instructions;
    let class = residual.class.label();
    let playbook = residual.class.playbook().unwrap_or("smart-queue");
    let next = residual.class.next_command();
    let mut out = format!("candidate={} reference={} differing_halfwords={}\ncompile={compile}\ntopology={}\nclass={class} wrong_instructions={wrong}\ntriage={class} playbook={playbook}\nnext={next}\n", actual.len(), expected.len(), differing.len(), topology.summary());
    for hint in &residual.facts.repair_hints {
        out.push_str(&format!(
            "repair_hint={} playbook={} detail={}\n",
            hint.signal, hint.playbook, hint.detail
        ));
    }
    if options.align {
        let pairs = align_streams(&candidate, &reference);
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
    } else {
        if actual.len() != expected.len() {
            out.push_str("  note: the two sides are different lengths, so the offset view below is\n             phase-shifted and every later row will read as a difference.\n             Re-run with --align to see the insertion or deletion itself.\n");
        }
        out.push_str("      offset  candidate                      reference\n");
        for offset in offsets {
            let mark = if differing.contains(&(*offset as usize)) {
                "!"
            } else {
                " "
            };
            out.push_str(&format!(
                "  {mark} {:04x}  {:<30.30} {}\n",
                offset,
                candidate_rows.get(offset).map(String::as_str).unwrap_or(""),
                reference_rows.get(offset).map(String::as_str).unwrap_or("")
            ));
        }
    }
    Ok(RenderedScore {
        output: RenderOutput {
            stdout: out,
            candidate_length: actual.len(),
            reference_length: expected.len(),
            differing_halfwords: differing.len(),
            allocator: None,
            residual,
        },
        candidate,
        reference,
    })
}
fn render_asm(
    root: &Path,
    options: &Options,
    work: &Path,
    identity: &SourceIdentity,
) -> Result<RenderOutput, String> {
    let started = Instant::now();
    let stem = identity.stem();
    let reference = root
        .join("games")
        .join(options.target.as_str())
        .join("asm")
        .join(format!("{stem}.s"));
    let patch = read_patch(options.patch.as_deref())?;
    let source = staged_source(root, options, work, patch.as_deref())?;
    let assembly = compile_to_assembly(
        &source,
        &identity.routing.to_string_lossy(),
        &work.to_string_lossy(),
        &[],
        options.target,
    )?;
    let candidate_gas =
        std::fs::read_to_string(&assembly).map_err(|error| format!("{assembly}: {error}"))?;
    if !reference.is_file() && !identity.owner.is_main() {
        return Ok(RenderOutput {
            stdout: candidate_gas,
            candidate_length: 0,
            reference_length: 0,
            differing_halfwords: 0,
            allocator: None,
            residual: classify(&[], &[], 0, 0, 0),
        });
    }
    if !reference.is_file() {
        return Err(format!(
            "--asm expects a main-image owner with {}",
            reference.display()
        ));
    }
    let symbol = format!("Func_{stem}");
    let reference_gas = std::fs::read_to_string(&reference)
        .map_err(|error| format!("{}: {error}", reference.display()))?;
    let topology = topology::compare(&candidate_gas, &reference_gas, &symbol);
    let candidate = gas_function_insns(&candidate_gas, &symbol);
    let expected = gas_function_insns(&reference_gas, &symbol);
    let dir = work;
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
        "elapsed_ms={:.0} compile=s-only\ntopology={}\ncandidate_insns={} reference_insns={}\nvs reference:\n{}",
        started.elapsed().as_secs_f64() * 1000.0,
        topology.summary(),
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
        residual: classify_with_topology(
            &candidate,
            &expected,
            candidate.len(),
            expected.len(),
            usize::from(candidate != expected),
            &topology,
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
fn source_cache_key(
    source: &str,
    routing_source: &str,
    compiler: CompilerTarget,
    owner_stem: &str,
    configuration: &CandidateCompilerConfiguration,
    rom_path: &str,
    rom: &[u8],
    size: Option<usize>,
    patch: Option<&str>,
) -> Result<String, String> {
    let executable = crate::compiler::bundle::executable_signature()?;
    let bundle = compiler_bundle_signature_checked()?;
    source_cache_key_with_environment(
        source,
        routing_source,
        compiler,
        owner_stem,
        configuration,
        rom_path,
        rom,
        size,
        patch,
        executable.as_bytes(),
        bundle.as_bytes(),
    )
}
fn source_cache_key_with_environment(
    source: &str,
    routing_source: &str,
    compiler: CompilerTarget,
    owner_stem: &str,
    configuration: &CandidateCompilerConfiguration,
    rom_path: &str,
    rom: &[u8],
    size: Option<usize>,
    patch: Option<&str>,
    executable: &[u8],
    compiler_bundle: &[u8],
) -> Result<String, String> {
    let symbols: Vec<_> = configuration
        .absolute_symbols
        .iter()
        .map(|(name, symbol)| (name, symbol.address, symbol.kind as u8))
        .collect();
    // JSON frames every field, including None versus zero and symbol names;
    // delimiter bytes and a second hand-written binary schema are unnecessary.
    let identity = serde_json::json!({
        "version": "diff-cache-v9",
        "source": source_input_signature(crate::compiler::routing::root(), source, routing_source, compiler)?,
        "route": routing_source,
        "owner": owner_stem,
        "implementation": executable,
        "compiler_bundle": compiler_bundle,
        "reference_symbols": configuration.reference_symbols,
        "absolute_symbols": symbols,
        "call_via_base": configuration.call_via_base,
        "overlay_extent": configuration.overlay_extent,
        "owner_symbol": configuration.owner_symbol,
        "rom_path": rom_path,
        "rom": crate::compiler::sha256::hex(rom),
        "size": size,
        "patch": patch,
    });
    Ok(crate::compiler::sha256::hex(
        &serde_json::to_vec(&identity).map_err(|error| error.to_string())?,
    ))
}
fn source_input_signature(
    root: &Path,
    source: &str,
    routing_source: &str,
    compiler: CompilerTarget,
) -> Result<Vec<u8>, String> {
    let source = root.join(source);
    let include_dirs = routing_source
        .strip_prefix("games/")
        .and_then(|path| path.split('/').next())
        .map(|game| root.join("games").join(game).join("include"))
        .into_iter()
        .collect::<Vec<_>>();
    let mut signature = source_tree_signature(&source, &include_dirs)?;
    signature.extend(source_symbol_bindings(root, routing_source, compiler)?.as_bytes());
    Ok(signature)
}
#[cfg(test)]
mod cache_key_tests {
    use super::*;
    #[test]
    fn compiler_route_bindings_host_and_bundle_are_cache_identity() {
        let directory = tempfile::tempdir().unwrap();
        let source = directory.path().join("candidate.c");
        std::fs::write(&source, "void Func_08000000(void) {}\n").unwrap();
        let source = source.to_str().unwrap();
        let routed = CandidateCompilerConfiguration::default();
        let reference_symbols = CandidateCompilerConfiguration {
            reference_symbols: true,
            ..Default::default()
        };
        let key = |route, owner, configuration, host: &[u8], bundle: &[u8]| {
            source_cache_key_with_environment(
                source,
                route,
                CompilerTarget::Gs1,
                owner,
                configuration,
                "roms/gs1-en.gba",
                b"reference-rom",
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
            key(
                route,
                "08000000",
                &reference_symbols,
                b"host-a",
                b"bundle-a",
            ),
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
    }

    #[test]
    fn changing_rom_contents_at_the_same_path_does_not_reuse_cached_reference() {
        let directory = tempfile::tempdir().unwrap();
        let source = directory.path().join("candidate.c");
        let rom = directory.path().join("reference.gba");
        std::fs::write(&source, "void Func_08000000(void) {}\n").unwrap();
        std::fs::write(&rom, b"first-rom").unwrap();
        let source = source.to_str().unwrap();
        let rom_path = rom.to_str().unwrap();
        let configuration = CandidateCompilerConfiguration::default();
        let key = |contents: &[u8]| {
            source_cache_key_with_environment(
                source,
                "games/gs1/src/08000000.c",
                CompilerTarget::Gs1,
                "08000000",
                &configuration,
                rom_path,
                contents,
                None,
                None,
                b"diff",
                b"compiler-bundle",
            )
            .unwrap()
        };
        let first_key = key(&std::fs::read(&rom).unwrap());
        let cache =
            psynergy::cache::SqliteCache::open(&directory.path().join("cache.sqlite3")).unwrap();
        cache
            .put(
                &first_key,
                &[("candidate", b"candidate"), ("reference", b"stale")],
            )
            .unwrap();
        assert_eq!(cached_bins(&cache, &first_key).unwrap().1, b"stale");

        std::fs::write(&rom, b"second-rom").unwrap();
        let second_key = key(&std::fs::read(&rom).unwrap());
        assert_ne!(first_key, second_key);
        assert!(cached_bins(&cache, &second_key).is_none());
    }
}
fn cached_bins(
    cache: &psynergy::cache::SqliteCache,
    key: &str,
) -> Option<(Vec<u8>, Vec<u8>, Option<Vec<u8>>, &'static str)> {
    let entries = cache.get(key).ok().flatten()?;
    let find = |kind: &str| {
        entries
            .iter()
            .find(|(entry_kind, _)| entry_kind == kind)
            .map(|(_, value)| value.clone())
    };
    let actual = find("candidate")?;
    let expected = find("reference")?;
    let candidate_gas = find("candidate-gas");
    (!actual.is_empty() && !expected.is_empty()).then_some((
        actual,
        expected,
        candidate_gas,
        "cache",
    ))
}
#[cfg(test)]
mod region_size_tests {
    use super::*;
    use std::fs;

    fn scratch_root() -> tempfile::TempDir {
        let dir = tempfile::tempdir().unwrap();
        fs::create_dir_all(dir.path().join("out/gs1-en/asm")).unwrap();
        dir
    }

    #[test]
    fn shared_object_reads_translation_unit_assembly() {
        let dir = scratch_root();
        let root = dir.path();
        let object = root.join("first.o");
        fs::write(root.join("first.s"), b"translation-unit").unwrap();
        fs::write(root.join("later.s"), b"stale-member").unwrap();
        assert_eq!(
            read_candidate_gas(root, "later", object.to_str()).unwrap(),
            b"translation-unit"
        );
    }

    #[test]
    fn owner_size_requires_a_matching_generated_manifest_region() {
        let directory = scratch_root();
        let root = directory.path();
        assert_eq!(region_size(root, 0x080a_b5e4), None);
        fs::write(
            root.join("out/gs1-en/asm/manifest.json"),
            r#"{"regions":[{"address":134986508,"size":6332}]}"#,
        )
        .unwrap();
        assert_eq!(region_size(root, 0x080a_b5e4), None);
        fs::create_dir_all(root.join("out/gs1-en/claimed")).unwrap();
        fs::write(
            root.join("out/gs1-en/claimed/manifest.json"),
            r#"{"regions":[{"address":134942628,"size":296}]}"#,
        )
        .unwrap();
        assert_eq!(region_size(root, 0x080b_0fa4), Some(296));
    }
}

#[cfg(test)]
mod source_identity_tests {
    use super::*;
    use std::fs;

    fn scratch_root() -> tempfile::TempDir {
        let root = tempfile::tempdir().unwrap();
        fs::create_dir_all(root.path().join("games/gs1")).unwrap();
        root
    }

    #[test]
    fn source_identity_uses_manifest_and_stable_routes() {
        let directory = scratch_root();
        let root = directory.path();
        fs::write(
            root.join("games/gs1/source-paths.json"),
            r#"{"format":3,"owners":{"main:080b0fa4":"battle/inventory/draw_paged_item_list.c"}}"#,
        )
        .unwrap();
        let identity = SourceIdentity::resolve(
            root,
            "games/gs1/src/battle/inventory/draw_paged_item_list.c",
            CompilerTarget::Gs1,
            None,
            None,
        )
        .unwrap();
        assert_eq!(identity.owner, SourceOwner::Main(0x080b0fa4));
        assert_eq!(identity.routing, PathBuf::from("games/gs1/src/080b0fa4.c"));
        let source = "games/gs1/recon/en/main/080ab5e4.c";
        let identity =
            SourceIdentity::resolve(root, source, CompilerTarget::Gs1, None, None).unwrap();
        assert_eq!(identity.owner, SourceOwner::Main(0x080ab5e4));
        assert_eq!(identity.routing, PathBuf::from(source));
        let identity = SourceIdentity::resolve(
            root,
            "candidate.c",
            CompilerTarget::Gs1,
            Some(0x080a8904),
            None,
        )
        .unwrap();
        assert_eq!(identity.routing, PathBuf::from("games/gs1/src/080a8904.c"));
    }
    #[test]
    fn register_binding_changes_invalidate_candidate_source_identity() {
        let directory = scratch_root();
        let root = directory.path();
        fs::write(root.join("candidate.c"), "void Scene_Run(void) {}\n").unwrap();
        let signature = |route| {
            source_input_signature(root, "candidate.c", route, CompilerTarget::Gs1).unwrap()
        };
        let register = root.join("games/gs1/source-paths.json");
        fs::write(&register, r#"{"format":3,"owners":{"main:08001234":{"name":"Scene_Run"},"resource_380:02000100":{"name":"Scene_Run"}}}"#).unwrap();
        let main = signature("games/gs1/src/08001234.c");
        let overlay = signature("games/gs1/src/resource_380_c_02000100.c");
        assert_ne!(main, overlay);
        fs::write(&register, r#"{"format":3,"owners":{"main:08001234":{"name":"Scene_Start"},"resource_380:02000100":{"name":"Scene_Run"}}}"#).unwrap();
        assert_ne!(main, signature("games/gs1/src/08001234.c"));
        assert_eq!(
            overlay,
            signature("games/gs1/src/resource_380_c_02000100.c")
        );
    }
}
