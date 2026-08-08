// CLI for the main-image retained-assembly guard.
//
//   core-retained-audit --check
//   core-retained-audit --check --json
//   core-retained-audit --self-test

use core_retained_audit::json::stringify_pretty;
use core_retained_audit::load::{
    canonical_spans, executable_is_nonempty, main_inventory, manifests,
};
use core_retained_audit::{
    audit_core_retained, normalized, AsmRegion, AuditInput, ClaimedRegion, InputPaths,
    InventoryInterval, NonCRange, Span, ROM_BASE,
};
use std::path::{Path, PathBuf};
use std::process::ExitCode;

fn repository_root() -> PathBuf {
    // The binary lives in tools/core-retained-audit/target/<profile>/, but
    // CARGO_MANIFEST_DIR is fixed at compile time and does not depend on the
    // working directory the gate is invoked from.
    Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap().to_path_buf()
}

fn main() -> ExitCode {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    match run(&argv) {
        Ok(code) => code,
        Err(message) => {
            // Bun prints `error: <message>` plus a stack trace for a thrown
            // Error; the first line is the part that is compared.
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}

fn run(argv: &[String]) -> Result<ExitCode, String> {
    if argv.len() == 1 && argv[0] == "--self-test" {
        return self_test().map(|()| ExitCode::SUCCESS);
    }
    if argv.iter().any(|argument| argument != "--check" && argument != "--json")
        || !argv.iter().any(|argument| argument == "--check")
    {
        return Err("usage: core_retained_audit.ts --check [--json] | --self-test".into());
    }
    let root = repository_root();
    let display = |relative: &str| root.join(relative).to_string_lossy().into_owned();
    let paths = InputPaths {
        inventory: display("metrics/gs1-en-executable.json"),
        semantic: display("semantic/main-regions.json"),
        asm_manifest: display("out/full/asm/manifest.json"),
        claimed_manifest: display("out/full/claimed/manifest.json"),
    };
    let inventory = main_inventory(&root.join("metrics/gs1-en-executable.json"))?;
    let executable = normalized(
        &inventory.iter().map(|item| item.span).collect::<Vec<_>>(),
        "executable inventory",
    )?;

    // Scanning nothing is not passing: an audit over an empty executable
    // inventory reconciles zero bytes and prints `status=ok`, which reads
    // exactly like a clean run. A gate that can pass without looking is the
    // defect it polices.
    //
    // PORT NOTE: this guard is an ADDITION. The TypeScript has no such check
    // and would happily report `status=ok executable=0`. It fires only on
    // inputs where the TypeScript's answer is vacuous, so parity on the real
    // tree is unaffected (verified in the side-by-side run).
    if inventory.is_empty() || !executable_is_nonempty(&executable) {
        return Err(
            "core retained audit reconciled an empty executable inventory — this is a FAILURE, not a pass"
                .into(),
        );
    }
    let spans = canonical_spans(&root, &executable)?;
    let loaded = manifests(&root.join("out/full/asm/manifest.json"), &root.join("out/full/claimed/manifest.json"))?;
    if loaded.asm.is_empty() || loaded.claimed.is_empty() {
        return Err(
            "core retained audit read an empty asm or claimed manifest — this is a FAILURE, not a pass"
                .into(),
        );
    }

    let result = audit_core_retained(&AuditInput {
        inventory,
        exact: spans.exact,
        semantic: spans.semantic,
        non_c: spans.non_c,
        asm: loaded.asm,
        claimed: loaded.claimed,
        noncanonical_semantic_claims: spans.noncanonical_semantic_claims,
        paths: Some(paths),
    })?;

    if argv.iter().any(|argument| argument == "--json") {
        println!("{}", stringify_pretty(&result.to_json()));
    } else {
        println!(
            "status={} executable={} exact_c={} semantic_c={} retained={}",
            result.status(),
            result.executable_bytes,
            result.exact_c_bytes,
            result.semantic_c_bytes,
            result.retained_bytes
        );
        for row in &result.retained_by_kind_confidence {
            println!("{}\t{}\tregions={}\tbytes={}", row.kind, row.confidence, row.regions, row.bytes);
        }
        for failure in &result.failures {
            eprintln!("FAIL {failure}");
        }
    }
    Ok(if result.status_ok { ExitCode::SUCCESS } else { ExitCode::FAILURE })
}

// ---------------------------------------------------------------------------
// Self-test
// ---------------------------------------------------------------------------

fn interval(start: i64, end: i64, evidence: &str) -> InventoryInterval {
    InventoryInterval {
        span: Span { start, end },
        kind: "thumb".into(),
        evidence: evidence.into(),
    }
}

fn baseline_asm() -> AsmRegion {
    AsmRegion {
        span: Span { start: ROM_BASE, end: ROM_BASE + 12 },
        source: "asm/a.s".into(),
        kind: "retained".into(),
        retention: "keep_asm".into(),
        confidence: "proven".into(),
        evidence: "proof".into(),
    }
}

fn input(
    exact: Vec<Span>,
    semantic: Vec<Span>,
    non_c: Vec<NonCRange>,
    asm: Vec<AsmRegion>,
    claimed: Vec<ClaimedRegion>,
    claims: Vec<ClaimedRegion>,
) -> AuditInput {
    AuditInput {
        inventory: vec![
            interval(ROM_BASE, ROM_BASE + 12, "asm/a.s:ordinary"),
            interval(ROM_BASE + 12, ROM_BASE + 16, "src/a.c:byte-identical-claimed-C"),
        ],
        exact,
        semantic,
        non_c,
        asm,
        claimed,
        noncanonical_semantic_claims: claims,
        paths: None,
    }
}

fn claim(start: i64, end: i64, source: &str) -> ClaimedRegion {
    ClaimedRegion { span: Span { start, end }, source: source.into() }
}

/// PORT NOTE: this mirrors `selfTest()` in the TypeScript and prints the same
/// single line. The added `executed` counter is the gate-hardening rule: a
/// self-test whose assertions get dropped by a future edit would otherwise
/// still print `self-test=ok`.
fn self_test() -> Result<(), String> {
    let mut executed = 0usize;
    let mut check = |condition: bool, message: &str| -> Result<(), String> {
        executed += 1;
        if condition {
            Ok(())
        } else {
            Err(message.to_string())
        }
    };

    let exact = vec![Span { start: ROM_BASE + 12, end: ROM_BASE + 16 }];
    let claimed = vec![claim(ROM_BASE + 12, ROM_BASE + 16, "src/a.c")];

    let ok = audit_core_retained(&input(
        exact.clone(),
        vec![],
        vec![],
        vec![baseline_asm()],
        claimed.clone(),
        vec![],
    ))?;
    check(
        ok.status_ok
            && ok.retained_bytes == 12
            && ok.retained_by_kind_confidence.first().map(|row| row.bytes) == Some(12),
        "core retained audit self-test baseline failed",
    )?;

    let candidate = AsmRegion {
        retention: "c_candidate".into(),
        kind: "compiler_output".into(),
        ..baseline_asm()
    };
    let bad = audit_core_retained(&input(
        exact.clone(),
        vec![],
        vec![],
        vec![candidate.clone()],
        claimed.clone(),
        vec![],
    ))?;
    check(
        !bad.status_ok
            && bad.failures.iter().any(|failure| failure.contains("ordinary c_candidate")),
        "core retained audit self-test candidate rejection failed",
    )?;

    let unsupported = audit_core_retained(&input(
        exact.clone(),
        vec![],
        vec![],
        vec![baseline_asm()],
        vec![claim(ROM_BASE + 8, ROM_BASE + 16, "src/a.c")],
        vec![],
    ))?;
    check(
        !unsupported.status_ok
            && unsupported.failures.iter().any(|failure| failure.contains("unsupported claimed-C")),
        "core retained audit self-test claimed rejection failed",
    )?;

    let replacement = audit_core_retained(&input(
        exact.clone(),
        vec![Span { start: ROM_BASE, end: ROM_BASE + 12 }],
        vec![],
        vec![],
        vec![
            claim(ROM_BASE, ROM_BASE + 12, "src/noncanonical.c"),
            claim(ROM_BASE + 12, ROM_BASE + 16, "src/exact.c"),
        ],
        vec![claim(ROM_BASE, ROM_BASE + 12, "src/noncanonical.c")],
    ))?;
    check(
        replacement.status_ok,
        "core retained audit self-test noncanonical semantic replacement failed",
    )?;

    for kind in ["literal_pool", "alignment_padding", "lookup_table"] {
        let permitted = audit_core_retained(&input(
            exact.clone(),
            vec![],
            vec![NonCRange {
                span: Span { start: ROM_BASE, end: ROM_BASE + 12 },
                kind: kind.into(),
                evidence: "explicit directive".into(),
            }],
            vec![candidate.clone()],
            claimed.clone(),
            vec![],
        ))?;
        check(
            permitted.status_ok,
            &format!("core retained audit self-test {kind} exemption failed"),
        )?;
    }

    // Scanning nothing is not passing, applied to the self-test itself.
    const EXPECTED_CHECKS: usize = 7;
    if executed != EXPECTED_CHECKS {
        return Err(format!(
            "core retained audit self-test executed {executed} checks, expected {EXPECTED_CHECKS} — a check was dropped"
        ));
    }
    println!("self-test=ok");
    Ok(())
}
