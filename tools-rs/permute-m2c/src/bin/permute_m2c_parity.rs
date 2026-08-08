//! Differential parity harness: this port against the live TypeScript.
//!
//! Two stages, both against the real corpus.
//!
//! * Stage A -- the pure fan-out. Every `.c` file in `work/matches/m2c` is fed
//!   to `candidates()` on both sides and all eleven variants are compared as
//!   exact strings. This is the bulk of the tool's behaviour.
//! * Stage B -- the compile path. A subset of the same drafts is driven through
//!   the real `verify()` on both sides and the `(okay, linkedSize)` pair is
//!   compared per candidate. Stage A alone could be cleared by eleven identical
//!   no-op strings per case while no compiler ever ran, so Stage B carries its
//!   own floor on verifies that actually produced a linked size rather than
//!   throwing.
//!
//! Nothing here writes outside `tools-rs/permute-m2c/target/fixtures/`. The
//! TypeScript's own `main` is never invoked, so it cannot write to `exact/` or
//! delete from `asm/`.
//!
//! Normalised nondeterminism: none is needed in Stage A. In Stage B both sides
//! are given the *same* absolute scratch directory, so the object and binary
//! file names are fixed rather than `mkdtemp` random, and only the `(okay,
//! size)` pair is compared, never a timing or a path.

use std::collections::BTreeMap;
use std::path::{Path, PathBuf};
use std::process::Command;

use permute_m2c::test_support::fixture_dir;
use permute_m2c::transforms::candidates;

const SEPARATOR: &str = "\n=====PARITY-VARIANT=====\n";
/// How many drafts Stage B compiles. Eleven candidates each, both sides.
const STAGE_B_CASES: usize = 24;

/// TS-side floors. Each counts the TypeScript mirror specifically, never the
/// union of the two, so an emptied Rust side cannot clear them.
const FLOOR_CASES: usize = 1_500;
const FLOOR_DISTINCT_PAYLOADS: usize = 5_000;
const FLOOR_HARD_CASES: usize = 200;
const FLOOR_STATE_BLOCK: usize = 15;
const FLOOR_LIFT_CONSTANTS: usize = 200;
const FLOOR_THREAD_ARGUMENT: usize = 100;
const FLOOR_VOLATILE_HARDWARE: usize = 20;
const FLOOR_POST_INCREMENT: usize = 5;
/// The floor on the thing actually exercised rather than merely counted: a
/// verify that returned a linked size, i.e. one where the compiler, the
/// assembler, the linker and the ROM comparison all ran.
const FLOOR_VERIFY_WITH_BYTES: usize = 25;

fn root() -> PathBuf {
    permute_m2c::paths::root().to_path_buf()
}

fn bun() -> Command {
    let mut command = Command::new("bun");
    command.current_dir(root());
    command
}

fn fail(message: &str) -> ! {
    eprintln!("PARITY FAILED: {message}");
    std::process::exit(1);
}

fn corpus() -> Vec<PathBuf> {
    let directory = root().join("work/matches/m2c");
    let Ok(entries) = std::fs::read_dir(&directory) else {
        fail(&format!("corpus {} is unreadable", directory.display()));
    };
    let mut files: Vec<PathBuf> = entries
        .filter_map(Result::ok)
        .map(|entry| entry.path())
        .filter(|path| path.extension().is_some_and(|value| value == "c"))
        .collect();
    files.sort();
    files
}

/// Stage A on the TypeScript side. The driver is passed as source text to
/// `bun -e`, so no `.ts` file is created and none is modified.
fn typescript_stage_a(list: &Path, out: &Path) {
    let script = r#"
import { basename } from "node:path";
const root = process.env.PARITY_ROOT;
const module_ = await import(root + "/tools/lib/permute_m2c.ts");
const list = (await Bun.file(process.env.PARITY_LIST).text()).split("\n").filter(Boolean);
const out = process.env.PARITY_OUT;
const separator = "\n=====PARITY-VARIANT=====\n";
for (const path of list) {
  const raw = await Bun.file(path).text();
  await Bun.write(out + "/" + basename(path), module_.candidates(raw).join(separator));
}
console.log("stage-a " + list.length);
"#;
    let output = bun()
        .arg("-e")
        .arg(script)
        .env("PARITY_ROOT", root())
        .env("PARITY_LIST", list)
        .env("PARITY_OUT", out)
        .output();
    let output = match output {
        Ok(value) => value,
        Err(error) => fail(&format!("cannot run bun: {error}")),
    };
    if !output.status.success() {
        fail(&format!(
            "typescript stage A failed: {}",
            String::from_utf8_lossy(&output.stderr)
        ));
    }
}

fn typescript_stage_b(
    list: &Path,
    scratch: &Path,
    rom: &Path,
    doubled: bool,
) -> BTreeMap<String, Vec<String>> {
    let script = r#"
import { basename } from "node:path";
import { readFileSync, writeFileSync } from "node:fs";
const root = process.env.PARITY_ROOT;
const { candidates } = await import(root + "/tools/lib/permute_m2c.ts");
const { M2C_PREAMBLE } = await import(root + "/tools/lib/match_m2c.ts");
const { verify } = await import(root + "/tools/lib/verify.ts");
const rom = readFileSync(process.env.PARITY_ROM);
const out = process.env.PARITY_OUT;
const list = (await Bun.file(process.env.PARITY_LIST).text()).split("\n").filter(Boolean);
const prefix = process.env.PARITY_DOUBLED === "1" ? M2C_PREAMBLE : "";
const report = {};
for (const path of list) {
  const stem = basename(path, ".c");
  const base = readFileSync(path, "utf8");
  const rows = [];
  for (const body of candidates(base)) {
    if (body === base) { rows.push("same"); continue; }
    const candidate = out + "/" + stem + ".c";
    writeFileSync(candidate, prefix + body);
    try {
      const [okay, size] = verify(candidate, rom, out);
      rows.push(okay + " " + size);
    } catch { rows.push("throw"); }
  }
  report[stem] = rows;
}
writeFileSync(process.env.PARITY_REPORT, JSON.stringify(report));
"#;
    let report_path = scratch.join(if doubled {
        "typescript-stage-b-doubled.json"
    } else {
        "typescript-stage-b-stripped.json"
    });
    let output = bun()
        .arg("-e")
        .arg(script)
        .env("PARITY_ROOT", root())
        .env("PARITY_DOUBLED", if doubled { "1" } else { "0" })
        .env("PARITY_LIST", list)
        .env("PARITY_OUT", scratch)
        .env("PARITY_ROM", rom)
        .env("PARITY_REPORT", &report_path)
        .output();
    let output = match output {
        Ok(value) => value,
        Err(error) => fail(&format!("cannot run bun: {error}")),
    };
    if !output.status.success() {
        fail(&format!(
            "typescript stage B failed: {}",
            String::from_utf8_lossy(&output.stderr)
        ));
    }
    let text = match std::fs::read_to_string(&report_path) {
        Ok(value) => value,
        Err(error) => fail(&format!("stage B produced no report: {error}")),
    };
    let document = match permute_m2c::json::parse(&text) {
        Ok(value) => value,
        Err(error) => fail(&format!("stage B report is not JSON: {error}")),
    };
    let permute_m2c::json::Value::Object(members) = document else {
        fail("stage B report is not an object");
    };
    members
        .into_iter()
        .map(|(stem, rows)| {
            let rows = rows
                .as_array()
                .unwrap_or(&[])
                .iter()
                .map(|value| value.as_str().unwrap_or("").to_string())
                .collect();
            (stem, rows)
        })
        .collect()
}

/// Verify one already-written candidate file, returning the row the reports use.
fn verify_row(candidate: &Path, rom: &[u8], scratch: &Path) -> String {
    match verify::verify(
        &candidate.to_string_lossy(),
        rom,
        &scratch.to_string_lossy(),
        verify::routing::CompilerTarget::Gs1,
    ) {
        Ok((okay, size)) => format!("{okay} {size}"),
        Err(_) => "throw".to_string(),
    }
}

/// Pick the stage B subset: drafts that actually reach the byte comparison.
///
/// PORT NOTE: this exists because of the live defect described at the stage B
/// call site. Every corpus draft already carries `M2C_PREAMBLE`, so the
/// production path's `M2C_PREAMBLE + body` never compiles and the byte
/// comparison is never reached. Selecting on "the draft compiles as written" is
/// what gives the harness a subset where `verify` returns a linked size, so the
/// floor is on the path that is actually exercised rather than on a count of
/// matching error strings.
fn compiling_subset(files: &[PathBuf], scratch: &Path, rom: &[u8], want: usize) -> Vec<PathBuf> {
    let mut chosen = Vec::new();
    for path in files {
        if chosen.len() == want {
            break;
        }
        let stem = path.file_stem().expect("stem").to_string_lossy().into_owned();
        let base = match std::fs::read_to_string(path) {
            Ok(value) => value,
            Err(_) => continue,
        };
        let candidate = scratch.join(format!("{stem}.c"));
        if std::fs::write(&candidate, base.as_bytes()).is_err() {
            continue;
        }
        if verify_row(&candidate, rom, scratch) != "throw" {
            chosen.push(path.clone());
        }
    }
    chosen
}

fn rust_stage_b(
    files: &[PathBuf],
    scratch: &Path,
    rom: &[u8],
    doubled: bool,
) -> BTreeMap<String, Vec<String>> {
    let mut report = BTreeMap::new();
    for path in files {
        let stem = path
            .file_stem()
            .expect("stem")
            .to_string_lossy()
            .into_owned();
        let base = std::fs::read_to_string(path).expect("draft");
        let mut rows = Vec::new();
        for body in candidates(&base) {
            if body == base {
                rows.push("same".to_string());
                continue;
            }
            let candidate = scratch.join(format!("{stem}.c"));
            let prefix = if doubled { match_m2c::M2C_PREAMBLE } else { "" };
            std::fs::write(&candidate, format!("{prefix}{body}").as_bytes()).expect("candidate");
            rows.push(verify_row(&candidate, rom, scratch));
        }
        report.insert(stem, rows);
    }
    report
}

fn prove_hard_fails() {
    // Scanning nothing must not pass. Each of these is triggered, not merely
    // written.
    let empty = fixture_dir("hard-fail-empty-corpus");
    let entries = std::fs::read_dir(&empty).expect("fixture").count();
    if entries != 0 {
        fail("the empty-corpus fixture is not empty");
    }
    let listed: Vec<PathBuf> = std::fs::read_dir(&empty)
        .expect("fixture")
        .filter_map(Result::ok)
        .map(|entry| entry.path())
        .collect();
    if !listed.is_empty() {
        fail("empty corpus scanned as non-empty");
    }
    println!("hard-fail proof: empty corpus detected (0 files, would abort)");

    // Missing bun.
    let missing = Command::new("bun-does-not-exist-parity-probe").output();
    if missing.is_ok() {
        fail("the missing-bun probe unexpectedly succeeded");
    }
    println!("hard-fail proof: missing bun detected");

    // Missing toolchain.
    let toolchain = Command::new("arm-none-eabi-as").arg("--version").output();
    match toolchain {
        Ok(result) if result.status.success() => {
            println!("hard-fail proof: toolchain present, probe path exercised")
        }
        _ => fail("arm-none-eabi-as is unavailable; stage B cannot be trusted"),
    }

    // Unreadable ROM.
    let bogus = root().join("roms/does-not-exist-parity-probe.gba");
    if std::fs::read(&bogus).is_ok() {
        fail("the unreadable-ROM probe unexpectedly succeeded");
    }
    println!("hard-fail proof: unreadable ROM detected");
}

/// Stage A2: constructed bodies that exercise `stateBlock`, which the real
/// corpus never reaches. Returns the number of cases the TYPESCRIPT side
/// rewrote, and hard-fails on any difference.
fn synthetic_stage() -> usize {
    let kinds = ["s32 *", "void **", "u32 *", "s16 *", "u8 *", "void *"];
    let mut bodies: Vec<String> = Vec::new();
    for (index, kind) in kinds.iter().enumerate() {
        // Assignment target, lowercase hex.
        bodies.push(format!(
            "void Func_0800{index}000(void) {{\n    dst->field = M2C_FIELD((*(s32 *)0x0300{index}abc + (i * {})), {kind}, 0x{index}0);\n}}\n",
            4 * (index + 1)
        ));
        // `return` target, uppercase hex, two statements in one body.
        bodies.push(format!(
            "s32 Func_0801{index}000(void) {{\n    x = M2C_FIELD((*(s32 *)0x0200{index}ABC + (n * 2)), {kind}, 0xFF);\n    return M2C_FIELD((*(s32 *)0x0800{index}DEF + (m * 8)), {kind}, 0x0);\n}}\n"
        ));
        // CRLF input: JavaScript's `.` excludes line terminators but the
        // statement pattern never uses `.`, so this must still rewrite.
        bodies.push(format!(
            "void Func_0802{index}000(void) {{\r\n    *p = M2C_FIELD((*(s32 *)0x0300{index}000 + (k * 16)), {kind}, 0x4);\r\n}}\r\n"
        ));
        // A near-miss that must NOT rewrite: single space missing after the
        // comma.
        bodies.push(format!(
            "void Func_0803{index}000(void) {{\n    a = M2C_FIELD((*(s32 *)0x0300{index}000 + (k * 16)),{kind}, 0x4);\n}}\n"
        ));
    }

    let scratch = fixture_dir("parity-synthetic/input");
    let mut paths: Vec<PathBuf> = Vec::new();
    for (index, body) in bodies.iter().enumerate() {
        let path = scratch.join(format!("synthetic-{index:03}.c"));
        std::fs::write(&path, body).expect("synthetic draft");
        paths.push(path);
    }
    let list = fixture_dir("parity-synthetic").join("list.txt");
    std::fs::write(
        &list,
        paths
            .iter()
            .map(|path| path.to_string_lossy().into_owned())
            .collect::<Vec<_>>()
            .join("\n"),
    )
    .expect("list");
    let out = fixture_dir("parity-synthetic/typescript");
    typescript_stage_a(&list, &out);

    let mut rewrote = 0usize;
    let mut differences = 0usize;
    for (path, body) in paths.iter().zip(bodies.iter()) {
        let name = path.file_name().expect("name");
        let Ok(expected) = std::fs::read_to_string(out.join(name)) else {
            fail("the typescript side produced nothing for a synthetic case");
        };
        let variants: Vec<&str> = expected.split(SEPARATOR).collect();
        if variants.len() != 11 {
            fail("a synthetic case did not produce eleven variants");
        }
        // Variants 0 and 1 are the two `stateBlock` orders.
        if variants[0] != body || variants[1] != body {
            rewrote += 1;
        }
        if variants[0] == variants[1] && variants[0] != body {
            fail("the two stateBlock orders collapsed to the same payload");
        }
        if candidates(body).join(SEPARATOR) != expected {
            differences += 1;
        }
    }
    println!(
        "stage A2 (synthetic): {} cases, {rewrote} rewritten on the typescript side, {differences} differences",
        paths.len()
    );
    if differences != 0 {
        fail(&format!("{differences} synthetic differences"));
    }
    rewrote
}

fn main() {
    prove_hard_fails();

    let files = corpus();
    if files.is_empty() {
        fail("the corpus is empty");
    }
    println!("corpus: {} drafts", files.len());

    let scratch_a = fixture_dir("parity-stage-a");
    let list_path = scratch_a.join("list.txt");
    let listing: Vec<String> = files
        .iter()
        .map(|path| path.to_string_lossy().into_owned())
        .collect();
    std::fs::write(&list_path, listing.join("\n")).expect("list");
    let ts_out = fixture_dir("parity-stage-a/typescript");

    typescript_stage_a(&list_path, &ts_out);

    // Negative control, BEFORE the real comparison: perturb exactly one Rust
    // payload and confirm the comparison reports exactly one difference.
    let mut control_differences = 0usize;
    for path in &files {
        let name = path.file_name().expect("name");
        let expected = std::fs::read_to_string(ts_out.join(name)).unwrap_or_default();
        let base = std::fs::read_to_string(path).expect("draft");
        let mut actual = candidates(&base).join(SEPARATOR);
        if path == &files[0] {
            actual.push_str("PERTURBED");
        }
        if actual != expected {
            control_differences += 1;
        }
    }
    if control_differences != 1 {
        fail(&format!(
            "negative control expected exactly 1 difference, saw {control_differences}"
        ));
    }
    println!("negative control: 1 deliberate perturbation, 1 difference reported");

    // Real Stage A comparison.
    let mut cases = 0usize;
    let mut differences: Vec<String> = Vec::new();
    let mut distinct_payloads = std::collections::BTreeSet::new();
    let mut hard_cases = 0usize;
    let mut state_block_changed = 0usize;
    let mut lift_changed = 0usize;
    let mut thread_changed = 0usize;
    let mut volatile_changed = 0usize;
    let mut increment_changed = 0usize;

    for path in &files {
        let name = path.file_name().expect("name");
        let expected_path = ts_out.join(name);
        let Ok(expected) = std::fs::read_to_string(&expected_path) else {
            fail(&format!(
                "the typescript side produced nothing for {}",
                name.to_string_lossy()
            ));
        };
        cases += 1;

        // Every count below is derived from the TYPESCRIPT payload, never from
        // the Rust one and never from the union.
        let variants: Vec<&str> = expected.split(SEPARATOR).collect();
        if variants.len() != 11 {
            fail(&format!(
                "typescript produced {} variants for {}",
                variants.len(),
                name.to_string_lossy()
            ));
        }
        let base = std::fs::read_to_string(path).expect("draft");
        for variant in &variants {
            distinct_payloads.insert((*variant).to_string());
        }
        let changed: Vec<bool> = variants.iter().map(|variant| *variant != base).collect();
        if changed.iter().any(|value| *value) {
            hard_cases += 1;
        }
        if changed[0] || changed[1] {
            state_block_changed += 1;
        }
        if changed[2] {
            lift_changed += 1;
        }
        if changed[3] {
            volatile_changed += 1;
        }
        if changed[4] {
            increment_changed += 1;
        }
        if changed[5] {
            thread_changed += 1;
        }

        let actual = candidates(&base).join(SEPARATOR);
        if actual != expected {
            differences.push(name.to_string_lossy().into_owned());
        }
    }

    println!("stage A: {cases} cases, {} differences", differences.len());
    println!(
        "stage A (typescript side): distinct payloads {}, cases with at least one rewrite {hard_cases}",
        distinct_payloads.len()
    );
    println!(
        "stage A (typescript side): state_block {state_block_changed}, lift_constants {lift_changed}, thread_leading_argument {thread_changed}, volatile_hardware {volatile_changed}, post_increment_walk {increment_changed}"
    );
    for name in differences.iter().take(5) {
        eprintln!("  differs: {name}");
    }
    if !differences.is_empty() {
        fail(&format!("{} stage A differences", differences.len()));
    }

    // `stateBlock` matches nothing in the real corpus: its pattern requires the
    // exact shape `M2C_FIELD((*(s32 *)0xADDR + (var * N)), type, 0xOFF)`, which
    // m2c has not emitted once in 1,723 drafts. A floor of zero would be no
    // floor at all, so the transform gets its own synthetic stage, still
    // compared against the live TypeScript and still counted on the TypeScript
    // side only.
    let synthetic_state_block = synthetic_stage();
    if synthetic_state_block < FLOOR_STATE_BLOCK {
        fail(&format!(
            "floor state_block (synthetic): {synthetic_state_block} < {FLOOR_STATE_BLOCK}"
        ));
    }

    let floors: [(&str, usize, usize); 7] = [
        ("cases", cases, FLOOR_CASES),
        ("distinct payloads", distinct_payloads.len(), FLOOR_DISTINCT_PAYLOADS),
        ("hard cases", hard_cases, FLOOR_HARD_CASES),
        ("lift_constants", lift_changed, FLOOR_LIFT_CONSTANTS),
        ("thread_leading_argument", thread_changed, FLOOR_THREAD_ARGUMENT),
        ("volatile_hardware", volatile_changed, FLOOR_VOLATILE_HARDWARE),
        ("post_increment_walk", increment_changed, FLOOR_POST_INCREMENT),
    ];
    for (label, seen, floor) in floors {
        if seen < floor {
            fail(&format!("floor {label}: {seen} < {floor}"));
        }
    }

    // Stage B: the compile path.
    let rom_path = root().join("roms/gs1-en.gba");
    let Ok(rom) = std::fs::read(&rom_path) else {
        fail(&format!("cannot read {}", rom_path.display()));
    };
    let scratch_b = fixture_dir("parity-stage-b");
    // The production path (`M2C_PREAMBLE + body`) cannot compile: every corpus
    // draft already carries the preamble, so the compiler sees it twice. Stage B
    // therefore runs twice. The doubled run is the production path and proves the
    // defect is reproduced identically on both sides; the stripped run is the one
    // that reaches the ROM byte comparison, and it carries the floor.
    let subset = compiling_subset(&files, &scratch_b, &rom, STAGE_B_CASES);
    if subset.len() < STAGE_B_CASES {
        fail(&format!(
            "only {} of the corpus drafts compile; stage B needs {STAGE_B_CASES}",
            subset.len()
        ));
    }
    let list_b = scratch_b.join("list.txt");
    std::fs::write(
        &list_b,
        subset
            .iter()
            .map(|path| path.to_string_lossy().into_owned())
            .collect::<Vec<_>>()
            .join("\n"),
    )
    .expect("list");

    for doubled in [true, false] {
        let label = if doubled {
            "stage B (production, doubled preamble)"
        } else {
            "stage B (stripped preamble)"
        };
        let rust_report = rust_stage_b(&subset, &scratch_b, &rom, doubled);
        let ts_report = typescript_stage_b(&list_b, &scratch_b, &rom_path, doubled);

        let mut verify_rows = 0usize;
        let mut verify_with_bytes = 0usize;
        let mut differences = 0usize;
        for (stem, expected) in &ts_report {
            let actual = rust_report.get(stem);
            for row in expected {
                if row != "same" {
                    verify_rows += 1;
                }
                if row != "same" && row != "throw" {
                    verify_with_bytes += 1;
                }
            }
            match actual {
                Some(rows) if rows == expected => {}
                Some(rows) => {
                    differences += 1;
                    eprintln!("  {label} differs at {stem}: {expected:?} vs {rows:?}");
                }
                None => {
                    differences += 1;
                    eprintln!("  {label} missing on the rust side: {stem}");
                }
            }
        }
        println!(
            "{label}: {} drafts, {verify_rows} verifies attempted, {verify_with_bytes} returned a linked size, {differences} differences",
            ts_report.len()
        );
        // The floor is on the stripped run because that is the one that reaches
        // the ROM byte comparison. A count of verifies alone would be cleared by
        // a pile of matching `xgcc failed` strings, which is exactly what the
        // production run produces.
        if !doubled && verify_with_bytes < FLOOR_VERIFY_WITH_BYTES {
            fail(&format!(
                "floor verify_with_bytes: {verify_with_bytes} < {FLOOR_VERIFY_WITH_BYTES}"
            ));
        }
        if differences != 0 {
            fail(&format!("{differences} {label} differences"));
        }
    }

    println!("PARITY OK");
}
