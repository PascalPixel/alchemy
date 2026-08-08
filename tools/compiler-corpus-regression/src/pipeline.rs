//! `main()`, `parallelMap` and the per-member compile/verify/diff flow.

use crate::cli::Options;
use crate::config::family_name;
use crate::corpus::{corpus, deterministic_sample, hash, join, Member};
use crate::diff::byte_difference;
use crate::extent::linked_function_extent;
use crate::jsparse::utf16_slice_to;
use crate::jsvalue::{canonical_json, number, object, string, strings, Json};
use crate::result::{atomic_json, read_cache, Outcome};
use crate::{FORMAT, ROM_BASE};
use alchemy_plan::plan::CompilerFamily;
use alchemy_routing::routing::CompilerTarget;
use candidate_compiler::verify::verify_candidate;
use std::path::Path;
use std::sync::atomic::{AtomicUsize, Ordering as AtomicOrdering};
use std::sync::Mutex;

/// The four native sources whose bytes go into `compilerSignature`.
///
/// The signature deliberately names the native source files directly. This
/// avoids the stale path assumptions that the former TypeScript implementation
/// had after the compiler tables moved between `tools/` subfolders.
pub const SIGNATURE_SOURCES: [&str; 4] = [
    "tools/alchemy-routing/src/routing.rs",
    "tools/candidate-compiler/src/verify.rs",
    "tools/integrate-matches/src/pipeline.rs",
    "tools/compiler-corpus-regression/src/pipeline.rs",
];

/// `hash(compilerBundleSignature(), ...four source files)`.
///
/// PORT NOTE -- CACHE KEY PROVENANCE. The key is a pure function of its inputs:
/// the bundle signature (a digest of the compiler binaries themselves), the
/// bytes of the tool's own source and its three libraries, the source file, the
/// expected ROM bytes, and the canonical JSON of the requested configuration.
/// There is no hand-maintained `-vN` literal anywhere in it. `FORMAT` is a
/// literal but it is not load-bearing, because the source digest alongside it
/// already changes whenever the logic does.
///
/// PORT NOTE -- the signature root is `alchemy_routing::routing::root()` and
/// can be overridden with `ALCHEMY_CCR_SIGNATURE_ROOT`. That override exists
/// for the differential parity harness, which runs a CORRECTED copy of the
/// TypeScript out of a symlink farm: without it the two sides would digest
/// different bytes for `compiler_corpus_regression.ts` and every `cache_key`
/// in the report would differ for a reason that has nothing to do with the
/// port. It is not read by any production path.
///
/// PORT NOTE -- MEASURED PERFORMANCE TRAP. `compilerBundleSignature()` digests
/// roughly 20 MB of compiler binaries and NEITHER side memoizes it. Bun's
/// `CryptoHasher` is native; `alchemy_bundle`'s portable sha256 is about 7x
/// slower and that alone made the `overlay_disasm` port 5.9x slower overall.
/// It is called ONCE per process here, so the cost is a constant, and it is
/// reported as its own line in the benchmark rather than being hidden. It is
/// deliberately NOT memoized locally: that would win the benchmark by moving
/// the fix out of `alchemy-bundle`, where it belongs.
pub fn compiler_signature() -> Result<String, String> {
    let root = signature_root();
    let mut parts: Vec<Vec<u8>> = vec![alchemy_bundle::bundle::compiler_bundle_signature().into_bytes()];
    for name in SIGNATURE_SOURCES {
        let path = root.join(name);
        parts.push(
            std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?,
        );
    }
    let borrowed: Vec<&[u8]> = parts.iter().map(|part| part.as_slice()).collect();
    Ok(hash(&borrowed))
}

fn signature_root() -> std::path::PathBuf {
    match std::env::var("ALCHEMY_CCR_SIGNATURE_ROOT") {
        Ok(value) if !value.is_empty() => std::path::PathBuf::from(value),
        _ => alchemy_routing::routing::root().to_path_buf(),
    }
}

/// `rom.subarray(start, end)`.
///
/// PORT NOTE -- `Buffer#subarray` CLAMPS both ends and never throws, where
/// Rust's `rom[start..end]` PANICS. A member whose address falls outside the
/// ROM yields an EMPTY expected buffer on both sides, and the run then reports
/// a size regression rather than crashing. Negative starts clamp to 0. The
/// arithmetic is done in `f64` because `member.address` and `member.size` are
/// JavaScript numbers and either can be NaN, which clamps to 0 the same way
/// `Buffer#subarray` treats a NaN index.
pub fn rom_slice(rom: &[u8], address: f64, size: f64) -> Vec<u8> {
    let clamp = |value: f64| -> usize {
        if value.is_nan() || value <= 0.0 {
            0
        } else if value >= rom.len() as f64 {
            rom.len()
        } else {
            value as usize
        }
    };
    let start = clamp(address - ROM_BASE);
    let stop = clamp(address - ROM_BASE + size);
    if stop <= start {
        return Vec::new();
    }
    rom[start..stop].to_vec()
}

/// `member.source.slice(ROOT.length + 1)`.
///
/// PORT NOTE -- this is an UNCHECKED prefix strip: the TypeScript does not
/// verify that `member.source` actually starts with `ROOT`, it just cuts
/// `ROOT.length + 1` UTF-16 units off the front. For a source outside the
/// repository that produces a mangled relative path rather than an error.
/// Reproduced exactly, including the UTF-16 unit counting.
pub fn relative_source(source: &str, root: &Path) -> String {
    let root_units = root.to_string_lossy().chars().map(|c| c.len_utf16()).sum::<usize>();
    let units: Vec<u16> = source.encode_utf16().collect();
    let start = (root_units + 1).min(units.len());
    String::from_utf16_lossy(&units[start..])
}

/// One member's compile, verify, link-extent and byte comparison.
fn evaluate(
    member: &Member,
    rom: &[u8],
    options: &Options,
    signature: &str,
    root: &Path,
) -> Result<Outcome, String> {
    let expected = rom_slice(rom, member.address, member.size);
    let source_bytes = std::fs::read(&member.source)
        .map_err(|error| format!("{}: {error}", member.source))?;
    let configuration = object(vec![
        ("flags", strings(&options.flags)),
        ("compiler_config", compiler_config_json(&options.compiler_config)),
    ]);
    let key = hash(&[
        // `String(FORMAT)` -- ECMAScript number-to-string, so `3` and not `3.0`.
        candidate_compiler::jsnum::to_js_number_string(FORMAT)?.as_bytes(),
        &source_bytes,
        &expected,
        signature.as_bytes(),
        canonical_json(&configuration)?.as_bytes(),
    ]);
    let cache_path = join(Path::new(&options.cache), &format!("cache/{key}.json"));

    // PORT NOTE -- BUG REPRODUCED. This read is OUTSIDE the `try` in the
    // TypeScript, so a corrupt cache file aborts the whole run instead of
    // falling back to a recompile. The `?` below propagates identically.
    if let Some(accepted) = read_cache(&cache_path, &key)? {
        return Ok(Outcome { cached: true, ..accepted });
    }

    let scratch = join(Path::new(&options.cache), &format!("scratch/{key}"));
    std::fs::create_dir_all(&scratch).map_err(|error| format!("{scratch}: {error}"))?;

    // Everything from here to the end of `attempt` is inside the TypeScript's
    // `try`, and every failure becomes `compiled: false` with the message.
    let attempt = || -> Result<(usize, usize, crate::diff::ByteDifference), String> {
        let verification = verify_candidate(
            &member.source,
            rom,
            &scratch,
            &options.flags,
            ROM_BASE,
            CompilerTarget::Gs1,
            &options.compiler_config,
        )?;
        let linked_path = join(Path::new(&scratch), &format!("{}.bin", member.stem));
        let linked = std::fs::read(&linked_path).map_err(|error| format!("{linked_path}: {error}"))?;
        let elf = join(Path::new(&scratch), &format!("{}.elf", member.stem));
        let symbols = std::process::Command::new("arm-none-eabi-nm")
            .args(["-S", "--defined-only", &elf])
            .output()
            .map_err(|error| format!("arm-none-eabi-nm: {error}"))?;
        if !symbols.status.success() {
            // `symbols.stderr.toString().trim() || "nm failed"` -- `||` on the
            // EMPTY STRING, which is falsy, so empty stderr becomes the
            // fallback. `??` would keep the empty string; this is `||`.
            let text = candidate_compiler::jsstring::js_trim(&String::from_utf8_lossy(&symbols.stderr))
                .to_string();
            return Err(if text.is_empty() { "nm failed".to_string() } else { text });
        }
        // PORT NOTE -- `options.compilerConfig.family === "gcc2951"` is a
        // STRING comparison against one of the six family literals, and
        // `undefined === "gcc2951"` is false, so an absent family takes the
        // `linkedFunctionExtent` branch. Modelled as `Some(Gcc2951)`.
        let extent = if options.compiler_config.family == Some(CompilerFamily::Gcc2951) {
            verification.actual.len()
        } else {
            linked_function_extent(
                &String::from_utf8_lossy(&symbols.stdout),
                &format!("Func_{}", member.stem),
                member.address,
                linked.len(),
            )?
        };
        // `linked.subarray(0, extent)` CLAMPS; `&linked[..extent]` panics.
        let actual = &linked[..extent.min(linked.len())];
        Ok((actual.len(), expected.len(), byte_difference(actual, &expected)))
    };

    let source = relative_source(&member.source, root);
    let outcome = match attempt() {
        Ok((actual_size, expected_size, difference)) => Outcome {
            stem: member.stem.clone(),
            source,
            cache_key: key.clone(),
            cached: false,
            compiled: true,
            exact: difference.count == 0,
            expected_size,
            actual_size: Some(actual_size),
            differing_bytes: Some(difference.count),
            first_difference: difference.first,
            error: None,
        },
        Err(message) => Outcome {
            stem: member.stem.clone(),
            source,
            cache_key: key.clone(),
            cached: false,
            compiled: false,
            exact: false,
            expected_size: expected.len(),
            actual_size: None,
            differing_bytes: None,
            first_difference: None,
            // `error.message.slice(0, 600)` counts UTF-16 units, so a message
            // containing astral characters truncates at a different byte
            // offset than `&message[..600]` would.
            error: Some(utf16_slice_to(&message, 600)),
        },
    };
    atomic_json(&cache_path, &outcome.to_json())?;
    Ok(outcome)
}

/// `CandidateCompilerConfiguration` as the canonical JSON the cache key sees.
///
/// PORT NOTE -- KEY ORDER IS THE CACHE KEY. The TypeScript object is built by
/// `compilerConfigurationOf` in the order `family`, `addFlags`, `removeFlags`,
/// and `canonicalJson` preserves insertion order. Any other order silently
/// invalidates every cached entry. `addFlags`/`removeFlags` are OMITTED when
/// absent, not written as `null`.
pub fn compiler_config_json(config: &candidate_compiler::verify::CandidateCompilerConfiguration) -> Json {
    let mut entries: Vec<(&str, Json)> = Vec::new();
    if let Some(family) = config.family {
        entries.push(("family", string(family_name(family))));
    }
    entries.push(("addFlags", strings(&config.add_flags)));
    entries.push(("removeFlags", strings(&config.remove_flags)));
    object(entries)
}

/// `parallelMap(items, jobs, operation)`.
///
/// PORT NOTE -- the TypeScript spawns `Math.min(jobs, items.length)` workers
/// pulling from a shared cursor, and writes results BY INDEX, so the output
/// order is the input order regardless of completion order. The scoped threads
/// below do the same. Results must never be collected in completion order: the
/// report lists them and the order is part of the artifact.
fn parallel_map<F>(count: usize, jobs: usize, operation: F) -> Vec<Result<Outcome, String>>
where
    F: Fn(usize) -> Result<Outcome, String> + Sync,
{
    let slots: Vec<Mutex<Option<Result<Outcome, String>>>> =
        (0..count).map(|_| Mutex::new(None)).collect();
    let cursor = AtomicUsize::new(0);
    let workers = jobs.min(count);
    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| loop {
                let index = cursor.fetch_add(1, AtomicOrdering::SeqCst);
                if index >= count {
                    return;
                }
                let value = operation(index);
                *slots[index].lock().expect("slot poisoned") = Some(value);
            });
        }
    });
    slots
        .into_iter()
        .map(|slot| slot.into_inner().expect("slot poisoned").expect("slot unfilled"))
        .collect()
}

/// The whole run. Returns the process exit code and the printed lines.
///
/// PORT NOTE -- the TypeScript prints with `console.log` and sets
/// `process.exitCode = 1` when there are regressions, which does NOT abort;
/// the process still exits normally after `main` resolves. Modelled by
/// returning the code rather than calling `exit`.
pub struct Run {
    pub exit_code: i32,
    pub lines: Vec<String>,
    pub report: Json,
}

pub fn run(options: &Options) -> Result<Run, String> {
    let root = alchemy_routing::routing::root();
    let available = corpus(options)?;
    let selected = deterministic_sample(&available, options.sample, &options.seed);
    // SCANNING NOTHING IS NOT PASSING. An empty selection is a hard failure on
    // both sides, and it is the failure the broken `src/` filter produces
    // today on the real manifest.
    if selected.is_empty() {
        return Err("no exact-C sources matched the selection filters".to_string());
    }

    let rom = std::fs::read(&options.rom).map_err(|error| format!("{}: {error}", options.rom))?;
    let signature = compiler_signature()?;
    std::fs::create_dir_all(&options.cache)
        .map_err(|error| format!("{}: {error}", options.cache))?;

    let outcomes = parallel_map(selected.len(), options.jobs, |index| {
        evaluate(&selected[index], &rom, options, &signature, root)
    });
    let mut results: Vec<Outcome> = Vec::with_capacity(outcomes.len());
    for outcome in outcomes {
        results.push(outcome?);
    }

    let regressions: Vec<&Outcome> = results.iter().filter(|result| !result.exact).collect();
    let cached = results.iter().filter(|result| result.cached).count();
    let exact = results.len() - regressions.len();

    // `[...options.sources].sort()` -- the DEFAULT sort, UTF-16 code-unit
    // order, not `localeCompare` and not `str::cmp`. Stems are hex so all
    // three agree, but the default is what is written.
    let mut sources = options.sources.clone();
    sources.sort_by(|left, right| crate::jsparse::default_sort_cmp(left, right));

    let near = match options.near {
        None => Json::Null,
        // `` `0x${options.near.toString(16)}` `` -- Number#toString(16).
        Some(value) => string(&format!("0x{}", js_hex(value)?)),
    };
    let filters = object(vec![
        ("sample", Json::Number(options.sample as f64)),
        ("seed", string(&options.seed)),
        (
            "family",
            match &options.family {
                None => Json::Null,
                Some(value) => string(value),
            },
        ),
        ("near", near),
        ("radius", Json::Number(options.radius)),
        ("sources", strings(&sources)),
    ]);
    let report = object(vec![
        ("format", Json::Number(FORMAT)),
        ("flags", strings(&options.flags)),
        ("compiler_config", compiler_config_json(&options.compiler_config)),
        ("filters", filters),
        ("available", number(available.len())),
        ("selected", number(selected.len())),
        ("cached", number(cached)),
        ("exact", number(exact)),
        ("regressions", number(regressions.len())),
        (
            "results",
            Json::Array(results.iter().map(Outcome::to_json).collect()),
        ),
    ]);
    if let Some(path) = &options.report {
        atomic_json(path, &report)?;
    }

    let mut lines = Vec::new();
    let mut printed_flags: Vec<String> = options.compiler_config.add_flags.clone();
    printed_flags.extend(options.flags.iter().cloned());
    lines.push(format!(
        "family={} flags={} remove={} available={} selected={} cached={}",
        // `${options.compilerConfig.family}` on `undefined` prints
        // "undefined", not an empty string.
        options.compiler_config.family.map(family_name).unwrap_or("undefined"),
        printed_flags.join(","),
        options.compiler_config.remove_flags.join(","),
        available.len(),
        selected.len(),
        cached,
    ));
    lines.push(format!("exact={exact} regressions={}", regressions.len()));
    for result in &regressions {
        if !result.compiled {
            // `${result.error}` on an absent error prints "undefined".
            lines.push(format!(
                "REGRESSION {} compile_error={}",
                result.stem,
                result.error.as_deref().unwrap_or("undefined"),
            ));
        } else {
            // PORT NOTE -- BUG REPRODUCED. `result.first_difference!` is a
            // non-null assertion over a value that a hand-edited or truncated
            // cache row can leave absent, and `undefined.toString(16)` is a
            // TypeError that kills the run at the very last step, after every
            // compile has been paid for. Surfaced as an error rather than
            // silently printing "undefined", because the TypeScript throws.
            let Some(first) = result.first_difference else {
                return Err(format!(
                    "cached result for {} claims a difference but omits first_difference",
                    result.stem
                ));
            };
            lines.push(format!(
                "REGRESSION {} size={}/{} differing_bytes={} first=0x{}",
                result.stem,
                result.actual_size.map(|v| v.to_string()).unwrap_or_else(|| "undefined".into()),
                result.expected_size,
                result
                    .differing_bytes
                    .map(|v| v.to_string())
                    .unwrap_or_else(|| "undefined".into()),
                js_hex(first as f64)?,
            ));
        }
    }
    Ok(Run {
        exit_code: i32::from(!regressions.is_empty()),
        lines,
        report,
    })
}

/// `Number#toString(16)` for the non-negative integers this tool produces.
///
/// PORT NOTE -- JavaScript's `toString(16)` is LOWERCASE and, for a negative
/// number, emits a leading `-` rather than a two's-complement form. It also
/// renders a fractional part in base 16, which no value here can have. The
/// domain is asserted rather than assumed.
fn js_hex(value: f64) -> Result<String, String> {
    if !value.is_finite() || value.fract() != 0.0 || value < 0.0 {
        return Err(format!("cannot render {value} as hexadecimal"));
    }
    Ok(format!("{:x}", value as u64))
}
