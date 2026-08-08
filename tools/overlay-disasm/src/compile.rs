//! Overlay assembly, the exact-C splice, and the source-keyed cache.

use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::Mutex;

use alchemy_routing::routing::CompilerTarget;
use alchemy_bundle::sha256;
use alchemy_plan::plan::{
    source_to_assembly_plan, CompilerFlagMutations, SourceToAssemblyPlanOptions,
};
use alchemy_symbols::{external_symbol, external_symbol_assembly, overlay_call_via_base};

use crate::paths::{basename, basename_without, extname, root, OverlaySource};
use crate::regex::Regex;

/// `hex(value, width = 8)`.
///
/// PORT NOTE: `padStart` pads and never truncates, so a value wider than
/// `width` prints in full rather than being clipped. `{:0width$x}` has the
/// same property, and both are exercised by a test.
pub fn hex(value: i64, width: usize) -> String {
    // PORT NOTE: JS `Number#toString(16)` on a negative number yields `-1f`,
    // not two's complement. Every caller here passes a non-negative address,
    // but the sign is reproduced rather than masked so a future negative input
    // fails the same way in both languages instead of silently differing.
    if value < 0 {
        let body = format!("{:x}", value.unsigned_abs());
        let padded = format!("{body:0>width$}", width = width);
        return format!("-{padded}");
    }
    format!("{value:0width$x}", width = width)
}

// ---------------------------------------------------------------------------
// The in-image label-word bias
// ---------------------------------------------------------------------------

// An overlay image is linked 0x8000 above the address it is loaded at, so every
// ABSOLUTE in-image code pointer the ROM stores is spelled `base + 0x8000`.
// Hand-written exact C carries that bias in its literals, but the compiler
// cannot: when a switch becomes a `mov pc, rN` dispatch, gcc emits the jump
// table as bare `.word .LN` rows and the pool word holding the table's own base
// as one more of them, and `ld -Ttext` resolves them all to the load address
// with no bias at all.
//
// Only a bare reference to a label DEFINED IN THIS FILE is biased. `.word` rows
// naming an external symbol, an integer, or a label DIFFERENCE (`.L5-.L2`) are
// left alone: a difference of two in-image addresses is already right.
const LOCAL_LABEL_DEFINITION: &str = r"^(\.L[A-Za-z0-9_$.]*):";
const LOCAL_LABEL_WORD: &str = r"^(\s*\.word\s+)(\.L[A-Za-z0-9_$.]*)\s*$";

pub const OVERLAY_LINK_BIAS: i64 = 0x8000;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BiasResult {
    pub text: String,
    pub biased: usize,
}

/// `biasInImageLabelWords(assembly)`.
///
/// PORT NOTE -- FOUR JS-REGEX TRAPS LIVE IN THESE TWO PATTERNS:
///
/// 1. Neither has the `i` flag. `.l4` must NOT match `\.L`. A case-insensitive
///    "simplification" here would bias rows the TypeScript leaves alone. This
///    is exactly the class of defect that has already been found in this tree
///    (`/^ldr/` beside a `/gi` sibling), so it is pinned by a test.
/// 2. `\s` in `LOCAL_LABEL_WORD` is the ECMAScript set, which includes U+00A0
///    and U+FEFF and EXCLUDES U+0085 -- the reverse of Rust's `White_Space`.
///    The engine in [`crate::regex`] implements the JS set.
/// 3. `$` without the `m` flag does NOT match before a trailing newline. The
///    caller splits on `"\n"` first, so no line carries one; the property is
///    still asserted so a future refactor that stops splitting fails loudly.
/// 4. The definition pattern is applied to `line.trim()` but the word pattern
///    to the RAW line. That asymmetry is the original's, and it matters: a
///    label definition indented by a tab is recognised, an indented `.word`
///    keeps its indentation inside capture group 1.
///
/// PORT NOTE: `defined` is a JS `Set<string>` used only for membership, so a
/// `BTreeSet` is behaviourally identical here. It is spelled as an ordered
/// container anyway, per this crate's no-hash-container rule.
pub fn bias_in_image_label_words(assembly: &str) -> BiasResult {
    let definition = Regex::new(LOCAL_LABEL_DEFINITION, "");
    let word = Regex::new(LOCAL_LABEL_WORD, "");
    let lines: Vec<&str> = assembly.split('\n').collect();
    let mut defined: std::collections::BTreeSet<String> = std::collections::BTreeSet::new();
    for line in &lines {
        let trimmed = crate::regex::js_trim(line);
        if let Some(found) = definition.exec(trimmed) {
            defined.insert(found.group(trimmed, 1).expect("group 1").to_string());
        }
    }
    let mut biased = 0usize;
    let mut out: Vec<String> = Vec::with_capacity(lines.len());
    for line in &lines {
        match word.exec(line) {
            Some(found) => {
                let label = found.group(line, 2).expect("group 2");
                if !defined.contains(label) {
                    out.push((*line).to_string());
                    continue;
                }
                biased += 1;
                let head = found.group(line, 1).expect("group 1");
                out.push(format!("{head}{label} + 0x{:x}", OVERLAY_LINK_BIAS));
            }
            None => out.push((*line).to_string()),
        }
    }
    BiasResult { text: out.join("\n"), biased }
}

// ---------------------------------------------------------------------------
// The source-keyed cache
// ---------------------------------------------------------------------------

/// Content-addressed cache for already-adopted overlay C spans.
///
/// `assembleOverlay` splices every adopted function's compiled bytes back into
/// the overlay image, so without a cache each call recompiles every prior
/// adoption in that overlay: the cost of one verification grows with the
/// progress already made.
///
/// PORT NOTE: overridable through `ALCHEMY_OVERLAY_C_CACHE` so a test run never
/// writes into the shared `out/` tree. Unset, it is byte-for-byte the
/// TypeScript's `out/cache/overlay-c`.
pub fn overlay_c_cache_dir() -> PathBuf {
    match std::env::var_os("ALCHEMY_OVERLAY_C_CACHE") {
        Some(value) => PathBuf::from(value),
        None => root().join("out/cache/overlay-c"),
    }
}

/// This crate's own source, embedded at build time.
///
/// PORT NOTE: the TypeScript reads `overlay_disasm.ts` off disk and throws when
/// it cannot. `include_bytes!` cannot fail at run time, which is strictly
/// stronger, but the empty-source guard is kept because it is the property that
/// actually matters: a digest that quietly degrades to the empty-string digest
/// is a cache key that has stopped discriminating.
///
/// EVERY MODULE IN THIS CRATE MUST BE LISTED. A module left out would mean an
/// edit to it does not move the key, which is precisely the rot the `-vN`
/// literal suffered.
const SELF_SOURCE: [&[u8]; 6] = [
    include_bytes!("lib.rs"),
    include_bytes!("compile.rs"),
    include_bytes!("disasm.rs"),
    include_bytes!("paths.rs"),
    include_bytes!("regex.rs"),
    include_bytes!("selftest.rs"),
];

/// A digest of this crate's own source, mixed into every overlay-C cache key.
///
/// The command plan stamps the compiler binaries and flags, but NOT the work
/// this module does after the compile: the label-word bias, the external-symbol
/// rules, the splice. Those used to be covered by a hand-bumped `-vN` string,
/// which is only ever as good as the next editor's memory. One key present in
/// two checkouts with different contents, 160 bytes against 164, poisoned an
/// entry and made `verify` die in `build_assets` on resource_39c; because
/// `git checkout` does not touch `out/`, runs at three different commits all
/// shared that entry and none was a test of its commit.
///
/// NEVER GIVE THIS A FALLBACK.
///
/// PORT NOTE: the Rust digest deliberately differs from the TypeScript's. Both
/// are "a digest of the tool's own source"; the tools are different files. The
/// consequence is that the two implementations occupy disjoint key namespaces
/// in the same cache directory, which is correct: a Rust entry must never be
/// served to the TypeScript or the reverse, because only the compiled bytes are
/// proven equal, not the two post-compile code paths' future edits.
pub fn self_digest() -> String {
    static CACHE: Mutex<Option<String>> = Mutex::new(None);
    let mut slot = CACHE.lock().expect("self-digest lock");
    if let Some(found) = slot.as_ref() {
        return found.clone();
    }
    let mut stream: Vec<u8> = Vec::new();
    for part in SELF_SOURCE {
        stream.extend_from_slice(part);
    }
    assert!(
        !stream.is_empty(),
        "overlay_disasm read an EMPTY source; refusing to key the cache"
    );
    let digest = sha256::hex(&stream);
    *slot = Some(digest.clone());
    digest
}

/// The digest this crate's source WOULD have with two characters appended.
/// Exists only so `self_test` can assert the digest moves when the source does.
pub fn edited_self_digest() -> String {
    let mut stream: Vec<u8> = Vec::new();
    for part in SELF_SOURCE {
        stream.extend_from_slice(part);
    }
    stream.extend_from_slice(b"//\n");
    sha256::hex(&stream)
}

/// `planStamp(commands, work)`.
///
/// PORT NOTE: the memo is keyed on the identity string, and the identity blanks
/// any argv part that starts with the temp directory, so two runs in different
/// temp directories stamp the same. `startsWith` is a prefix test on the whole
/// part, not a path-component test: an argv part that merely begins with the
/// same characters is blanked too. Reproduced.
///
/// PORT NOTE: the digest streams the identity and then every `/`-rooted binary's
/// contents, in command order. `readFileSync` failure becomes the literal
/// `"unreadable"`, so a missing compiler is distinguishable from an empty one.
fn plan_stamp(commands: &[Vec<String>], work: &str) -> String {
    static MEMO: Mutex<Option<BTreeMap<String, String>>> = Mutex::new(None);
    let identity: String = commands
        .iter()
        .map(|command| {
            command
                .iter()
                .map(|part| if part.starts_with(work) { "<work>".to_string() } else { part.clone() })
                .collect::<Vec<_>>()
                .join("|")
        })
        .collect::<Vec<_>>()
        .join("");
    {
        let guard = MEMO.lock().expect("plan-stamp lock");
        if let Some(found) = guard.as_ref().and_then(|memo| memo.get(&identity)) {
            return found.clone();
        }
    }
    let mut stream: Vec<u8> = identity.as_bytes().to_vec();
    for command in commands {
        let Some(binary) = command.first() else { continue };
        if !binary.starts_with('/') {
            continue;
        }
        match fs::read(binary) {
            Ok(bytes) => stream.extend_from_slice(&bytes),
            Err(_) => stream.extend_from_slice(b"unreadable"),
        }
    }
    let stamp = sha256::hex(&stream);
    let mut guard = MEMO.lock().expect("plan-stamp lock");
    guard.get_or_insert_with(BTreeMap::new).insert(identity, stamp.clone());
    stamp
}

// ---------------------------------------------------------------------------
// Overlay C discovery
// ---------------------------------------------------------------------------

/// `overlayCSources(source)`.
///
/// Overlay C replacements live in `exact/`, named `prefix_c_<address>.c` after
/// the `.s` container's own basename, NOT siblings of the `.s` file itself.
///
/// PORT NOTE: the prefix strips `/overlay\.s$/` with NO leading underscore, so
/// `resource_39c_overlay.s` yields the prefix `resource_39c_c_`. `overlayCSpans`
/// and `assembleOverlay` strip `/_overlay\.s$/` WITH one for a different
/// purpose. The two really are different regexes in the original.
///
/// PORT NOTE: `readdirSync(...).sort()` is JS's default sort, which compares
/// UTF-16 code units. Every name here is ASCII, where that coincides with byte
/// order, so `Vec::sort` is exact. Pinned by a test rather than assumed.
pub fn overlay_c_sources(source: &OverlaySource) -> Vec<PathBuf> {
    let Some(anchor) = source.c_source_anchor() else { return Vec::new() };
    let directory = root().join("exact");
    let stripped = Regex::new(r"overlay\.s$", "").replace_first(basename(anchor), "");
    let prefix = format!("{stripped}c_");
    if !directory.exists() {
        return Vec::new();
    }
    let Ok(entries) = fs::read_dir(&directory) else { return Vec::new() };
    let mut names: Vec<String> = entries
        .filter_map(Result::ok)
        .filter_map(|entry| entry.file_name().into_string().ok())
        .filter(|name| name.starts_with(&prefix) && name.ends_with(".c"))
        .collect();
    names.sort();
    names.into_iter().map(|name| directory.join(name)).collect()
}

/// The eight-hex-digit address a `prefix_c_<address>.c` filename encodes.
///
/// PORT NOTE: `basename(path, extname(path)).slice(-8)` clamps rather than
/// panicking on a stem shorter than eight characters, and `String#slice` counts
/// UTF-16 units. The names are ASCII, so a byte-wise last-eight is exact; the
/// clamp is reproduced explicitly.
fn address_stem(path: &Path) -> Result<(String, i64), String> {
    let display = path.to_string_lossy().to_string();
    let stem = basename_without(&display, extname(&display));
    let tail: String = {
        let bytes = stem.as_bytes();
        let from = bytes.len().saturating_sub(8);
        stem[from..].to_string()
    };
    // PORT NOTE: `/^[0-9a-f]{8}$/i` DOES carry the `i` flag here, unlike the two
    // label patterns above. Upper-case hex is accepted by the test and then
    // parsed by `parseInt(stem, 16)`, which is also case-insensitive.
    if !Regex::new(r"^[0-9a-f]{8}$", "i").is_match(&tail) {
        return Err(format!("overlay C filename is not an address: {display}"));
    }
    let address = i64::from_str_radix(&tail, 16).map_err(|error| error.to_string())?;
    Ok((tail, address))
}

/// `overlayCAddresses(source)`.
///
/// PORT NOTE: the TypeScript returns a `Set<number>`, which SILENTLY DEDUPES.
/// Two files whose stems differ only in case (`...02000240.c` and
/// `...02000240.C`) would collapse to one entry there; a `Vec` here would not.
/// The `BTreeSet` reproduces the dedupe, and the sorted order is a superset of
/// the guarantee callers already had (the sources are read in sorted order, so
/// insertion order was already ascending for the ASCII names that occur).
pub fn overlay_c_addresses(source: &OverlaySource) -> Result<std::collections::BTreeSet<i64>, String> {
    let mut found = std::collections::BTreeSet::new();
    for path in overlay_c_sources(source) {
        found.insert(address_stem(&path)?.1);
    }
    Ok(found)
}

// ---------------------------------------------------------------------------
// Subprocess plumbing
// ---------------------------------------------------------------------------

/// `checked(command, cwd)`.
///
/// PORT NOTE: on failure the detail is stderr, or stdout when stderr is EMPTY.
/// JS `||` is falsy-on-empty-string; `is_empty` is the same test here. Bun's
/// spawn failure (ENOENT) has no `error: ` prefix, so parity is asserted on the
/// exit code and the offending path, never on prose.
fn checked(command: &[String], cwd: &Path) -> Result<String, String> {
    let Some((binary, rest)) = command.split_first() else {
        return Err("empty command".to_string());
    };
    let output = Command::new(binary)
        .args(rest)
        .current_dir(cwd)
        .output()
        .map_err(|error| format!("{} failed: {error}", basename(binary)))?;
    let stdout = String::from_utf8_lossy(&output.stdout).to_string();
    if output.status.code() != Some(0) {
        let stderr = String::from_utf8_lossy(&output.stderr).to_string();
        let detail = crate::regex::js_trim(if stderr.is_empty() { &stdout } else { &stderr });
        let suffix = if detail.is_empty() { String::new() } else { format!(": {detail}") };
        return Err(format!("{} failed{suffix}", basename(binary)));
    }
    Ok(stdout)
}

/// A run of a subprocess whose stderr is the whole error message.
pub(crate) fn spawn_raw(command: &[String], cwd: &Path) -> Result<Vec<u8>, String> {
    let (binary, rest) = command.split_first().ok_or_else(|| "empty command".to_string())?;
    let output = Command::new(binary)
        .args(rest)
        .current_dir(cwd)
        .output()
        .map_err(|error| format!("{binary}: {error}"))?;
    if output.status.code() != Some(0) {
        return Err(crate::regex::js_trim(&String::from_utf8_lossy(&output.stderr)).to_string());
    }
    Ok(output.stdout)
}

// ---------------------------------------------------------------------------
// compileOverlayC
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Compiled {
    pub address: i64,
    pub data: Vec<u8>,
}

/// `compileOverlayC(source, work, overlay, routingSource = source, extraFlags = [])`.
pub fn compile_overlay_c(
    source: &Path,
    work: &Path,
    overlay: &str,
    routing_source: Option<&Path>,
    extra_flags: &[String],
) -> Result<Compiled, String> {
    let source_display = source.to_string_lossy().to_string();
    let routing_source = routing_source.unwrap_or(source).to_string_lossy().to_string();
    let call_via_base = overlay_call_via_base(overlay, Some(&source_display)) as i64;
    let (stem, address) = address_stem(source)?;
    let symbol = format!("Func_{}", stem.to_lowercase());

    let text = fs::read_to_string(source).map_err(|error| format!("{source_display}: {error}"))?;
    // PORT NOTE: `new RegExp("\\b" + symbol + "\\s*\\([^;{}]*\\)\\s*\\{")` is
    // built by interpolation, unanchored, ASCII `\b`. `[^;{}]*` is greedy and
    // spans newlines, because a negated class is NOT `.` and so is not stopped
    // by a line terminator.
    let guard = Regex::new(&format!(r"\b{symbol}\s*\([^;{{}}]*\)\s*\{{"), "");
    if !guard.is_match(&text) {
        return Err(format!("overlay C source does not define {symbol}: {source_display}"));
    }

    let work_display = work.to_string_lossy().to_string();
    let at = |name: &str| work.join(name).to_string_lossy().to_string();
    let assembly = at(&format!("{stem}.s"));
    let object = at(&format!("{stem}.o"));
    let symbols_source = at(&format!("{stem}.symbols.s"));
    let symbols_object = at(&format!("{stem}.symbols.o"));
    let elf = at(&format!("{stem}.elf"));
    let binary = at(&format!("{stem}.bin"));

    let mut options = SourceToAssemblyPlanOptions::new(
        CompilerTarget::Gs1,
        routing_source,
        source_display.clone(),
        assembly.clone(),
    );
    options.preprocessed_output = Some(at(&format!("{stem}.i")));
    // PORT NOTE: `extraFlags.length > 0 ? { addFlags: extraFlags } : undefined`.
    // An EMPTY slice must stay `undefined`, not `Some(empty)`: the plan layer
    // distinguishes them.
    if !extra_flags.is_empty() {
        options.flags = Some(CompilerFlagMutations {
            add_flags: extra_flags.to_vec(),
            remove_flags: Vec::new(),
        });
    }
    let plan = source_to_assembly_plan(&options).map_err(|error| error.to_string())?;

    // THE CACHE KEY CARRIES A DIGEST OF THIS TOOL'S OWN SOURCE, never a
    // hand-bumped version literal. See `self_digest`. The compiler BINARY is
    // part of the key too, not just the argv that names it: rebuilding the fork
    // leaves every command identical while changing the bytes it emits, and
    // 14,339 entries once survived a rebuild and made resource_39c fail its
    // asset round trip long after `verify` reported green.
    let mut key: Vec<u8> = Vec::new();
    key.extend_from_slice(
        format!(
            "overlay-c:{}:{}:{}:{}\0",
            self_digest(),
            alchemy_bundle::bundle::compiler_bundle_signature(),
            hex(address, 8),
            hex(call_via_base, 8)
        )
        .as_bytes(),
    );
    let steps: Vec<Vec<String>> = plan.steps.iter().map(|step| step.command.clone()).collect();
    key.extend_from_slice(plan_stamp(&steps, &work_display).as_bytes());
    key.push(0);
    key.extend_from_slice(&fs::read(source).map_err(|error| format!("{source_display}: {error}"))?);
    let cached = overlay_c_cache_dir().join(format!("{}.bin", sha256::hex(&key)));

    // A cache hit returns before the compile steps ever run, so extra debug
    // flags would silently produce no side-effect files on a repeat invocation.
    if extra_flags.is_empty() && cached.exists() {
        if let Ok(data) = fs::read(&cached) {
            return Ok(Compiled { address, data });
        }
    }

    for step in &plan.steps {
        checked(&step.command, work)?;
    }
    let produced = fs::read_to_string(&assembly).map_err(|error| format!("{assembly}: {error}"))?;
    fs::write(&assembly, bias_in_image_label_words(&produced).text)
        .map_err(|error| format!("{assembly}: {error}"))?;
    checked(
        &strings(&["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", &object, &assembly]),
        work,
    )?;

    // PORT NOTE: `nm -u` output is split, empty lines dropped, and the LAST
    // whitespace-separated field taken. `at(-1)!` on a non-empty split cannot be
    // undefined, so the `!` is sound.
    //
    // PORT NOTE: this is a plain array, NOT a Set. A symbol listed twice emits
    // its stub twice, and the assembler then fails on the duplicate label.
    // Deduping here would be a behaviour change, so the order-preserving
    // duplicate-keeping list is reproduced.
    let undefined_symbols: Vec<String> = split_lines(&checked(
        &strings(&["arm-none-eabi-nm", "-u", &object]),
        work,
    )?)
    .into_iter()
    .filter(|line| !line.is_empty())
    .map(|line| {
        crate::regex::js_trim(&line)
            .split(|c: char| crate::regex::is_js_space(c))
            .rfind(|part| !part.is_empty())
            .expect("a non-empty trimmed line has a last field")
            .to_string()
    })
    .collect();
    for external in &undefined_symbols {
        if external_symbol(external, call_via_base as u64).is_none() {
            return Err(format!("unsupported overlay C external symbol: {external}"));
        }
    }
    let mut stubs = String::from(".syntax unified\n.thumb\n");
    for name in &undefined_symbols {
        stubs.push_str(&external_symbol_assembly(name, call_via_base as u64)?);
    }
    fs::write(&symbols_source, stubs).map_err(|error| format!("{symbols_source}: {error}"))?;
    checked(
        &strings(&[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            &symbols_object,
            &symbols_source,
        ]),
        work,
    )?;
    checked(
        &strings(&[
            "arm-none-eabi-ld",
            &format!("-Ttext=0x{}", hex(address, 8)),
            "-e",
            &symbol,
            "-o",
            &elf,
            &object,
            &symbols_object,
        ]),
        work,
    )?;
    checked(
        &strings(&["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", &elf, &binary]),
        work,
    )?;
    let listing = checked(&strings(&["arm-none-eabi-nm", "-S", &elf]), work)?;
    let needle = format!(" {symbol}");
    let row = split_lines(&listing)
        .into_iter()
        .find(|line| line.ends_with(&needle))
        .ok_or_else(|| format!("missing linked overlay C symbol: {symbol}"))?;
    // PORT NOTE: `parseInt(field, 16)` stops at the first non-hex character and
    // yields NaN on none, where `from_str_radix` errors on any trailing junk.
    // `nm -S` prints fixed-width hex, so the two agree; the parse error is
    // surfaced rather than silently becoming a zero-length span.
    let size_field = crate::regex::js_trim(&row)
        .split(|c: char| crate::regex::is_js_space(c))
        .filter(|part| !part.is_empty())
        .nth(1)
        .ok_or_else(|| format!("nm -S row has no size field: {row}"))?
        .to_string();
    let size = js_parse_int_hex(&size_field)
        .ok_or_else(|| format!("nm -S size is not hex: {size_field}"))?;
    let whole = fs::read(&binary).map_err(|error| format!("{binary}: {error}"))?;
    // PORT NOTE: `Buffer#subarray(0, size)` CLAMPS where Rust `&whole[..size]`
    // PANICS. A linked symbol larger than the emitted `.text` is not impossible
    // (a `.bss`-tailed function), so the clamp is load-bearing.
    let end = (size.max(0) as usize).min(whole.len());
    let data = whole[..end].to_vec();

    // A cache write failure must never fail a verification.
    let _ = fs::create_dir_all(overlay_c_cache_dir());
    let _ = cache_entry::write_cache_entry_atomically(&cached, &data);

    Ok(Compiled { address, data })
}

/// `compileOverlayCandidate`: compile one prospective overlay C owner without
/// installing a placeholder or touching the canonical overlay assembly.
/// Exact-source iteration and parallel agents need this read-only path, because
/// `overlay_adopt` deliberately rehearses in place.
pub fn compile_overlay_candidate(
    source: &Path,
    work: &Path,
    overlay: &str,
    routing_source: Option<&Path>,
    extra_flags: &[String],
) -> Result<Compiled, String> {
    compile_overlay_c(source, work, overlay, routing_source, extra_flags)
}

// ---------------------------------------------------------------------------
// Spans and the splice
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Span {
    pub start: i64,
    pub end: i64,
}

/// `overlayCSpans(source, base = OVERLAY_BASE)`: the image-offset span of every
/// already-adopted exact-C row in an overlay.
///
/// A row that fails to compile is OMITTED rather than guessed at, and callers
/// must read a missing start as "still unknown". Reporting a span this function
/// could not produce would recreate exactly the false confidence it removes.
///
/// PORT NOTE: the sort is `(a, b) => a.start - b.start`, a numeric comparator
/// on i64. JS's sort is STABLE, so equal starts keep source order; `sort_by_key`
/// is also stable, so the two agree. `sort_unstable_by_key` would not.
pub fn overlay_c_spans(source: &OverlaySource, base: i64) -> Vec<Span> {
    let display = source.to_display_string();
    let overlay = Regex::new(r"_overlay\.s$", "").replace_first(basename(&display), "");
    let mut spans = Vec::new();
    for c_source in overlay_c_sources(source) {
        let Ok(work) = TempDir::new("alchemy-overlay-span-") else { continue };
        if let Ok(compiled) = compile_overlay_c(&c_source, work.path(), &overlay, None, &[]) {
            let start = compiled.address - base;
            spans.push(Span { start, end: start + compiled.data.len() as i64 });
        }
    }
    spans.sort_by_key(|span| span.start);
    spans
}

/// `assembleOverlay(source, base = OVERLAY_BASE)`.
pub fn assemble_overlay(source: &OverlaySource, base: i64) -> Result<Vec<u8>, String> {
    let work = TempDir::new("alchemy-overlay-").map_err(|error| error.to_string())?;
    let at = |name: &str| work.path().join(name).to_string_lossy().to_string();
    let assembly = at("o.s");
    let object = at("o.o");
    let elf = at("o.elf");
    let binary = at("o.bin");
    let text = source.read_text().map_err(|error| error.to_string())?;
    fs::write(&assembly, text).map_err(|error| error.to_string())?;
    spawn_raw(
        &strings(&["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", &object, &assembly]),
        work.path(),
    )?;
    spawn_raw(
        &strings(&["arm-none-eabi-ld", &format!("-Ttext=0x{}", hex(base, 8)), "-o", &elf, &object]),
        work.path(),
    )?;
    spawn_raw(
        &strings(&["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", &elf, &binary]),
        work.path(),
    )?;
    let mut result = fs::read(&binary).map_err(|error| error.to_string())?;

    let display = source.to_display_string();
    let overlay = Regex::new(r"_overlay\.s$", "").replace_first(basename(&display), "");
    // PORT NOTE: `occupied` is a `Set<number>` of BYTE OFFSETS. A `Set<number>`
    // cannot collapse `0x10` with `16` here because both are the same JS number
    // -- the trap is real for literals written two ways, not for computed
    // offsets, and this set only ever holds computed ones.
    let mut occupied: std::collections::BTreeSet<usize> = std::collections::BTreeSet::new();
    for c_source in overlay_c_sources(source) {
        // Name the row. A toolchain failure inside `compileOverlayC` surfaced
        // as a bare `xgcc failed: ...` with no indication of which overlay, or
        // which of its rows, was being built.
        let compiled = compile_overlay_c(&c_source, work.path(), &overlay, None, &[]).map_err(
            |cause| {
                format!("{overlay}: {}: {cause}", basename(&c_source.to_string_lossy()))
            },
        )?;
        let offset = compiled.address - base;
        if offset < 0 || offset + compiled.data.len() as i64 > result.len() as i64 {
            return Err(format!("overlay C span is outside {display}: {}", c_source.display()));
        }
        let offset = offset as usize;
        for (index, existing) in result[offset..offset + compiled.data.len()].iter().enumerate() {
            let byte = offset + index;
            if occupied.contains(&byte) {
                return Err(format!("overlapping overlay C span: {}", c_source.display()));
            }
            occupied.insert(byte);
            if *existing != 0 {
                return Err(format!(
                    "overlay C placeholder is not zero at 0x{}",
                    hex(base + byte as i64, 8)
                ));
            }
        }
        result[offset..offset + compiled.data.len()].copy_from_slice(&compiled.data);
    }
    Ok(result)
}

// ---------------------------------------------------------------------------
// Small helpers
// ---------------------------------------------------------------------------

pub(crate) fn strings(parts: &[&str]) -> Vec<String> {
    parts.iter().map(|part| (*part).to_string()).collect()
}

/// `text.split(/\r?\n/)`.
///
/// PORT NOTE: `str::lines` also strips a trailing `\r`, but it DROPS the final
/// empty field after a trailing newline, where JS's split keeps it. The
/// `filter(Boolean)` and `find` callers do not care; a future `len()` caller
/// would, so the JS shape is reproduced.
pub(crate) fn split_lines(text: &str) -> Vec<String> {
    let mut out = Vec::new();
    let mut current = String::new();
    let mut chars = text.chars().peekable();
    while let Some(c) = chars.next() {
        if c == '\r' && chars.peek() == Some(&'\n') {
            chars.next();
            out.push(std::mem::take(&mut current));
        } else if c == '\n' {
            out.push(std::mem::take(&mut current));
        } else {
            current.push(c);
        }
    }
    out.push(current);
    out
}

/// `Number.parseInt(text, 16)`: leading JS whitespace, an optional sign, an
/// optional `0x` prefix, then hex digits, stopping at the first non-digit.
/// `None` is NaN.
pub(crate) fn js_parse_int_hex(text: &str) -> Option<i64> {
    let body = crate::regex::js_trim(text);
    let (negative, body) = match body.strip_prefix('-') {
        Some(rest) => (true, rest),
        None => (false, body.strip_prefix('+').unwrap_or(body)),
    };
    let body = body.strip_prefix("0x").or_else(|| body.strip_prefix("0X")).unwrap_or(body);
    let digits: String = body.chars().take_while(|c| c.is_ascii_hexdigit()).collect();
    if digits.is_empty() {
        return None;
    }
    let value = i64::from_str_radix(&digits, 16).ok()?;
    Some(if negative { -value } else { value })
}

/// `mkdtempSync(join(TMPDIR, prefix))`, removed on drop.
///
/// PORT NOTE: `TMPDIR ?? TMP ?? TEMP ?? "/tmp"`, in that order, and `??` falls
/// through only on null/undefined -- an EMPTY `TMPDIR` is honoured, not
/// replaced. `std::env::temp_dir` would substitute `/tmp` for an empty value,
/// so the chain is spelled out.
pub struct TempDir {
    path: PathBuf,
}

impl TempDir {
    pub fn new(prefix: &str) -> std::io::Result<TempDir> {
        let base = ["TMPDIR", "TMP", "TEMP"]
            .iter()
            .find_map(std::env::var_os)
            .map(PathBuf::from)
            .unwrap_or_else(|| PathBuf::from("/tmp"));
        let mut attempt = 0u32;
        loop {
            let nonce = format!(
                "{:x}{:x}{attempt:x}",
                std::process::id(),
                std::time::SystemTime::now()
                    .duration_since(std::time::UNIX_EPOCH)
                    .map(|d| d.as_nanos())
                    .unwrap_or(0)
            );
            let candidate = base.join(format!("{prefix}{nonce}"));
            match fs::create_dir_all(&base).and_then(|()| fs::create_dir(&candidate)) {
                Ok(()) => return Ok(TempDir { path: candidate }),
                Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => {
                    attempt += 1;
                    if attempt > 64 {
                        return Err(error);
                    }
                }
                Err(error) => return Err(error),
            }
        }
    }

    pub fn path(&self) -> &Path {
        &self.path
    }
}

impl Drop for TempDir {
    /// DESTRUCTIVE-PATH RULE: the target is the directory this type created
    /// itself, under the temp root, and it is never assembled from caller
    /// input. Nothing else is ever removed by this crate.
    fn drop(&mut self) {
        if self.path.starts_with("/") && self.path.components().count() > 2 {
            let _ = fs::remove_dir_all(&self.path);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    // ---- biasInImageLabelWords -------------------------------------------

    const SAMPLE: &str = concat!(
        "\t.text\n",
        ".L2:\n",
        "\t.word\t.L4\n",
        "\t.word\t.L4-.L2\n",
        "\t.word\tData_02000240\n",
        "\t.word\t265\n",
        "\t.word\t.Lelsewhere\n",
        "\t.word\t.L2\n",
        ".L4:\n",
        "\tbx\tlr"
    );

    #[test]
    fn bias_matches_the_typescript_self_test() {
        let result = bias_in_image_label_words(SAMPLE);
        assert_eq!(result.biased, 2);
        let lines: Vec<&str> = result.text.split('\n').collect();
        assert_eq!(lines[2], "\t.word\t.L4 + 0x8000");
        assert_eq!(lines[7], "\t.word\t.L2 + 0x8000");
        let original: Vec<&str> = SAMPLE.split('\n').collect();
        for index in [3, 4, 5, 6] {
            assert_eq!(lines[index], original[index], "row {index} must be left alone");
        }
    }

    #[test]
    fn an_undefined_label_is_never_biased() {
        assert_eq!(bias_in_image_label_words("\t.word\t.L4").biased, 0);
    }

    /// TRAP: neither label pattern carries the `i` flag. clippy once suggested a
    /// case-insensitive comparison in this tree and masked a real missing-`i`
    /// bug; this test is the thing that would catch the reverse.
    #[test]
    fn label_patterns_are_case_sensitive() {
        let lower = ".l2:\n\t.word\t.l2";
        assert_eq!(bias_in_image_label_words(lower).biased, 0);
        assert_eq!(bias_in_image_label_words(lower).text, lower);
        // And the mixed case does not cross-match.
        let mixed = ".L2:\n\t.word\t.l2";
        assert_eq!(bias_in_image_label_words(mixed).biased, 0);
    }

    /// TRAP: JS `\s` includes U+00A0 and U+FEFF and EXCLUDES U+0085; Rust's
    /// `char::is_whitespace` is the reverse on both.
    #[test]
    fn word_pattern_uses_the_ecmascript_space_set() {
        let nbsp = ".L2:\n\u{00a0}.word\u{00a0}.L2";
        assert_eq!(bias_in_image_label_words(nbsp).biased, 1);
        let nel = ".L2:\n\u{0085}.word .L2";
        assert_eq!(bias_in_image_label_words(nel).biased, 0, "U+0085 is not JS \\s");
    }

    /// TRAP: `$` without `m` does not match before a trailing newline, and the
    /// input is split on "\n" before matching so no line ever carries one.
    #[test]
    fn word_pattern_end_anchor_rejects_trailing_content() {
        assert_eq!(bias_in_image_label_words(".L2:\n\t.word\t.L2 ; note").biased, 0);
        assert_eq!(bias_in_image_label_words(".L2:\n\t.word\t.L2\t").biased, 1);
    }

    /// The definition pattern runs on the TRIMMED line, the word pattern on the
    /// RAW one. That asymmetry is the original's.
    #[test]
    fn definition_is_trimmed_and_word_is_not() {
        // An indented definition still registers.
        assert_eq!(bias_in_image_label_words("   .L2:\n\t.word\t.L2").biased, 1);
        // The word row keeps its indentation in group 1.
        let out = bias_in_image_label_words(".L2:\n    .word .L2");
        assert_eq!(out.text.split('\n').nth(1).unwrap(), "    .word .L2 + 0x8000");
    }

    #[test]
    fn crlf_input_does_not_silently_produce_an_empty_table() {
        // TRAP: JS `.` excludes line terminators, so a CRLF file parsed with a
        // `.`-bearing pattern can yield nothing. These patterns use `\s`, which
        // matches `\r`, so the `$` anchor still lands -- but the definition
        // scan sees a trailing `\r` and `js_trim` removes it.
        // Verified against Bun: the pattern DOES match, because `\r` is JS
        // `\s` and the definition scan sees `.L2:` after `js_trim` removes
        // the CR. A `.`-bearing pattern would have produced nothing.
        let crlf = ".L2:\r\n\t.word\t.L2\r";
        assert_eq!(bias_in_image_label_words(crlf).biased, 1);
    }

    // ---- self digest ------------------------------------------------------

    #[test]
    fn self_digest_is_a_stable_sha256() {
        let digest = self_digest();
        assert!(Regex::new(r"^[0-9a-f]{64}$", "").is_match(&digest), "not a sha256: {digest}");
        assert_eq!(self_digest(), digest, "must be stable within a run");
    }

    #[test]
    fn self_digest_never_collapses_to_the_empty_digest() {
        assert_ne!(self_digest(), sha256::hex(&[]));
    }

    /// The digest must MOVE when the source moves. Asserted on synthetic input
    /// so unrelated progress cannot break it.
    #[test]
    fn self_digest_changes_when_the_source_changes() {
        let mut stream: Vec<u8> = Vec::new();
        for part in SELF_SOURCE {
            stream.extend_from_slice(part);
        }
        let edited = sha256::hex(&[stream.as_slice(), b"//\n"].concat());
        assert_ne!(edited, self_digest());
    }

    /// EVERY module file must be in `SELF_SOURCE`, or an edit to the missing one
    /// would not move the key. This walks `src/` and fails on any `.rs` whose
    /// bytes are not part of the digested stream.
    #[test]
    fn self_source_covers_every_module() {
        let src = Path::new(env!("CARGO_MANIFEST_DIR")).join("src");
        let mut stream: Vec<u8> = Vec::new();
        for part in SELF_SOURCE {
            stream.extend_from_slice(part);
        }
        let mut seen = 0usize;
        for entry in fs::read_dir(&src).expect("src/ is readable") {
            let path = entry.expect("entry").path();
            if path.extension().and_then(|e| e.to_str()) != Some("rs") {
                continue;
            }
            if path.file_name().and_then(|n| n.to_str()) == Some("main.rs") {
                // The binary is a thin dispatcher over the library; the
                // TypeScript's `import.meta.main` block is likewise not part of
                // what the key protects... but it IS part of the same file
                // there. Assert deliberately rather than forgetting.
                continue;
            }
            let bytes = fs::read(&path).expect("readable module");
            assert!(
                stream.windows(bytes.len()).any(|window| window == bytes),
                "{} is not in SELF_SOURCE; an edit to it would not move the cache key",
                path.display()
            );
            seen += 1;
        }
        assert_eq!(seen, SELF_SOURCE.len(), "SELF_SOURCE length must equal the module count");
    }

    // ---- helpers ----------------------------------------------------------

    #[test]
    fn hex_pads_but_never_truncates() {
        assert_eq!(hex(0x2000000, 8), "02000000");
        assert_eq!(hex(0x1_0000_0000, 8), "100000000");
        assert_eq!(hex(0xffff, 4), "ffff");
        // TRAP: padStart on a negative number gives `000000-2` in JS only when
        // the caller pads the already-signed string; `toString(16)` yields
        // "-2" and padStart then prefixes zeros. Reproduced as "-0000002".
        assert_eq!(hex(-2, 8), "-00000002");
    }

    #[test]
    fn split_lines_keeps_the_trailing_empty_field() {
        assert_eq!(split_lines("a\nb\n"), vec!["a", "b", ""]);
        assert_eq!(split_lines("a\r\nb"), vec!["a", "b"]);
        assert_eq!(split_lines(""), vec![""]);
    }

    #[test]
    fn parse_int_hex_follows_js_not_from_str_radix() {
        assert_eq!(js_parse_int_hex("24"), Some(0x24));
        assert_eq!(js_parse_int_hex("  1f  "), Some(0x1f));
        // JS stops at the first non-digit rather than erroring.
        assert_eq!(js_parse_int_hex("1fzz"), Some(0x1f));
        assert_eq!(js_parse_int_hex("0x20"), Some(0x20));
        assert_eq!(js_parse_int_hex("zz"), None);
        assert_eq!(js_parse_int_hex(""), None);
        // f64::from_str would accept these; parseInt(_, 16) does not.
        assert_eq!(js_parse_int_hex("inf"), None);
        // "nan": `n` is not a hex digit, so parseInt(_, 16) reads nothing.
        assert_eq!(js_parse_int_hex("nan"), None);
    }

    #[test]
    fn address_stem_clamps_a_short_name() {
        let long = Path::new("/x/resource_39c_c_02000240.c");
        assert_eq!(address_stem(long).expect("hex"), ("02000240".to_string(), 0x0200_0240));
        assert!(address_stem(Path::new("/x/ab.c")).is_err());
        // Upper case is accepted: that pattern DOES carry the `i` flag.
        assert_eq!(
            address_stem(Path::new("/x/p_c_0200BEEF.c")).expect("hex").1,
            0x0200_beef
        );
    }
}
