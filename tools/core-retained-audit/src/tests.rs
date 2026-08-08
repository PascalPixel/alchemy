// Fixture-driven tests for the audit. Every test here assembles inputs and
// asserts an outcome that would change if the logic changed; none of them
// assert a value against the same expression that produced it.

use super::*;
use crate::load::{
    boundaries, canonical_spans, executable_is_nonempty, main_inventory, manifests, region_span,
    semantic_spans,
};

fn s(start: i64, end: i64) -> Span {
    Span { start, end }
}

// ---------------------------------------------------------------------------
// Interval algebra
// ---------------------------------------------------------------------------

#[test]
fn normalized_merges_touching_spans_and_rejects_out_of_rom() {
    let merged = normalized(
        &[s(ROM_BASE + 8, ROM_BASE + 12), s(ROM_BASE, ROM_BASE + 8), s(ROM_BASE + 20, ROM_BASE + 24)],
        "t",
    )
    .unwrap();
    // Touching (start == previous.end) MERGES; the guard is `>`, not `>=`.
    assert_eq!(merged, vec![s(ROM_BASE, ROM_BASE + 12), s(ROM_BASE + 20, ROM_BASE + 24)]);
    assert!(normalized(&[s(0, 4)], "t").is_err());
    assert!(normalized(&[s(ROM_BASE, ROM_BASE + ROM_SIZE as i64 + 1)], "t").is_err());
    assert_eq!(bytes(&[s(ROM_BASE, ROM_BASE + 8), s(ROM_BASE + 4, ROM_BASE + 12)]).unwrap(), 12);
}

#[test]
fn without_and_missing_coverage_are_complements() {
    let left = vec![s(ROM_BASE, ROM_BASE + 32)];
    let right = vec![s(ROM_BASE + 8, ROM_BASE + 16)];
    assert_eq!(without(&left, &right), vec![s(ROM_BASE, ROM_BASE + 8), s(ROM_BASE + 16, ROM_BASE + 32)]);
    let mut mask = vec![0u8; ROM_SIZE];
    mark(&mut mask, &right, 1);
    assert_eq!(
        missing_coverage(&mask, &left),
        vec![s(ROM_BASE, ROM_BASE + 8), s(ROM_BASE + 16, ROM_BASE + 32)]
    );
    assert!(missing_coverage(&mask, &right).is_empty());
    assert!(all_covered(&mask, &right));
    assert!(!all_covered(&mask, &left));
}

#[test]
fn mark_clamps_an_out_of_range_span_instead_of_panicking() {
    // A span below the ROM base produces a negative relative index, which JS
    // `fill` resolves from the END of the array. Rust slicing would panic.
    let mut mask = vec![0u8; ROM_SIZE];
    mark(&mut mask, &[s(ROM_BASE - 16, ROM_BASE - 8)], 1);
    assert_eq!(mask[ROM_SIZE - 16..ROM_SIZE - 8], [1u8; 8]);
    assert_eq!(mask[0], 0);
}

#[test]
fn hex_pads_to_eight_digits() {
    assert_eq!(hex(ROM_BASE), "0x08000000");
    assert_eq!(hex(0x080006fc), "0x080006fc");
    // The same value WITHOUT padding is what the known bug produces.
    assert_eq!(format!("0x{:x}", 0x080006fc_i64), "0x80006fc");
}

// ---------------------------------------------------------------------------
// Span parsing
// ---------------------------------------------------------------------------

#[test]
fn span_accepts_hex_strings_and_address_size_form() {
    let hex_form = json::parse(r#"{"address":"0x08002344","size":16}"#).unwrap();
    assert_eq!(span(&hex_form, "t").unwrap(), s(0x08002344, 0x08002354));
    // An explicit null `start` falls through to `address` via `??`.
    let nullish = json::parse(r#"{"start":null,"address":134226756,"end":134226760}"#).unwrap();
    assert_eq!(span(&nullish, "t").unwrap(), s(134226756, 134226760));
    // A whole-valued float is a safe integer in JavaScript.
    assert_eq!(
        span(&json::parse(r#"{"start":16.0,"end":32.0}"#).unwrap(), "t").unwrap(),
        s(16, 32)
    );
    assert!(span(&json::parse(r#"{"start":16,"end":16}"#).unwrap(), "t").is_err());
    assert!(span(&json::parse(r#"{"start":16.5,"end":32}"#).unwrap(), "t").is_err());
    assert!(span(&json::parse(r#"{"start":true,"end":32}"#).unwrap(), "t").is_err());
    assert!(span(&json::parse(r#"{"end":32}"#).unwrap(), "t").is_err());
}

// ---------------------------------------------------------------------------
// Source scanning
// ---------------------------------------------------------------------------

#[test]
fn canonical_c_rejects_exactly_the_four_disqualifiers() {
    assert!(canonical_c("int f(void) { return 1; }\n"));
    assert!(!canonical_c("/* M2C_ERROR(x) */\n"));
    assert!(!canonical_c("__asm__(\"nop\");\n"));
    assert!(!canonical_c("\t.incbin \"baserom.gba\"\n"));
    assert!(!canonical_c("asm   volatile (\"nop\");\n"));
    assert!(!canonical_c("register int r asm (\"r0\");\n"));
    assert!(!canonical_c("register int r asm(\"r0\");\n"));
    // `\b` is ASCII-only and the words must stand alone.
    assert!(canonical_c("int M2C_ERRORS = 0;\n"));
    assert!(canonical_c("int my__asm__thing = 0;\n"));
    assert!(canonical_c("asmvolatile();\n"));
    // `[^;\n]*` cannot cross a semicolon or a newline.
    assert!(canonical_c("register int r; asm(\"nop\");\n"));
    assert!(canonical_c("register int r;\nasm(\"nop\");\n"));
    // `asm\s+volatile` needs at least one space; JS `\s` includes U+FEFF.
    assert!(!canonical_c("asm\u{feff}volatile(\"nop\");\n"));
}

#[test]
fn address_from_name_requires_exactly_eight_hex_digits() {
    assert_eq!(address_from_name(Path::new("exact/080006fc.c"), ".c"), Some(0x080006fc));
    assert_eq!(address_from_name(Path::new("exact/80006fc.c"), ".c"), None);
    assert_eq!(address_from_name(Path::new("exact/080006fc.s"), ".c"), None);
    assert_eq!(address_from_name(Path::new("exact/0800xxfc.c"), ".c"), None);
}

#[test]
fn region_span_runs_to_the_next_boundary_or_the_limit() {
    let starts = [0x0800_0000, 0x0800_0010, 0x0800_0030];
    assert_eq!(region_span(0x0800_0000, &starts, 0x0800_1000), s(0x0800_0000, 0x0800_0010));
    assert_eq!(region_span(0x0800_0030, &starts, 0x0800_1000), s(0x0800_0030, 0x0800_1000));
}

// ---------------------------------------------------------------------------
// The audit
// ---------------------------------------------------------------------------

fn asm_region(start: i64, end: i64, retention: &str, kind: &str) -> AsmRegion {
    AsmRegion {
        span: s(start, end),
        source: "asm/a.s".into(),
        kind: kind.into(),
        retention: retention.into(),
        confidence: "proven".into(),
        evidence: "proof".into(),
    }
}

fn base_input() -> AuditInput {
    AuditInput {
        inventory: vec![InventoryInterval {
            span: s(ROM_BASE, ROM_BASE + 32),
            kind: "thumb".into(),
            evidence: "asm/a.s".into(),
        }],
        exact: vec![s(ROM_BASE, ROM_BASE + 8)],
        semantic: vec![s(ROM_BASE + 8, ROM_BASE + 16)],
        non_c: vec![],
        // Semantic C must still be covered by the asm manifest (it is compiled
        // from reviewed C but the bytes are accounted for on the asm side),
        // hence the second region, which overlaps no retained byte.
        asm: vec![
            asm_region(ROM_BASE + 8, ROM_BASE + 16, "semantic", "semantic_c"),
            asm_region(ROM_BASE + 16, ROM_BASE + 32, "keep_asm", "retained"),
        ],
        claimed: vec![ClaimedRegion { span: s(ROM_BASE, ROM_BASE + 8), source: "src/a.c".into() }],
        noncanonical_semantic_claims: vec![],
        paths: None,
    }
}

#[test]
fn the_complement_is_the_inventory_less_exact_and_semantic_c() {
    let result = audit_core_retained(&base_input()).unwrap();
    assert!(result.status_ok, "unexpected failures: {:?}", result.failures);
    assert_eq!(result.executable_bytes, 32);
    assert_eq!(result.exact_c_bytes, 8);
    assert_eq!(result.semantic_c_bytes, 8);
    assert_eq!(result.retained_bytes, 16);
    assert_eq!(
        result.retained_by_kind_confidence,
        vec![KindConfidenceRow {
            kind: "retained".into(),
            confidence: "proven".into(),
            regions: 1,
            bytes: 16
        }]
    );
}

#[test]
fn c_outside_the_inventory_and_uncovered_retained_bytes_both_fail() {
    let mut input = base_input();
    input.exact = vec![s(ROM_BASE + 64, ROM_BASE + 72)];
    input.claimed = vec![];
    let result = audit_core_retained(&input).unwrap();
    assert!(!result.status_ok);
    assert!(result
        .failures
        .iter()
        .any(|f| f == "exact C contains bytes outside the tracked executable inventory"));

    let mut uncovered = base_input();
    uncovered.asm = vec![];
    let result = audit_core_retained(&uncovered).unwrap();
    assert!(result
        .failures
        .iter()
        .any(|f| f.starts_with("retained complement absent from the current full asm manifest at")));
}

#[test]
fn a_c_candidate_in_the_complement_fails_unless_an_evidenced_directive_covers_it() {
    let mut input = base_input();
    input.asm[1] = asm_region(ROM_BASE + 16, ROM_BASE + 32, "c_candidate", "compiler_output");
    let result = audit_core_retained(&input).unwrap();
    assert!(!result.status_ok);
    assert_eq!(result.failures.len(), 1);
    assert!(result.failures[0].starts_with("ordinary c_candidate/compiler_output retained code at"));

    for kind in ["literal_pool", "alignment_padding", "lookup_table"] {
        let mut permitted = input.clone_with_non_c(kind);
        permitted.paths = None;
        assert!(audit_core_retained(&permitted).unwrap().status_ok, "kind {kind} was not exempted");
    }
}

impl AuditInput {
    fn clone_with_non_c(&self, kind: &str) -> AuditInput {
        AuditInput {
            inventory: self.inventory.clone(),
            exact: self.exact.clone(),
            semantic: self.semantic.clone(),
            non_c: vec![NonCRange {
                span: s(ROM_BASE + 16, ROM_BASE + 32),
                kind: kind.into(),
                evidence: "explicit .align directive".into(),
            }],
            asm: self.asm.clone(),
            claimed: self.claimed.clone(),
            noncanonical_semantic_claims: self.noncanonical_semantic_claims.clone(),
            paths: self.paths.clone(),
        }
    }
}

#[test]
fn a_claimed_fragment_needs_exact_c_backing_or_a_named_semantic_replacement() {
    let mut input = base_input();
    input.claimed = vec![ClaimedRegion { span: s(ROM_BASE, ROM_BASE + 16), source: "src/a.c".into() }];
    let result = audit_core_retained(&input).unwrap();
    assert!(result
        .failures
        .iter()
        .any(|f| f.starts_with("unsupported claimed-C fragment src/a.c at")));

    // Naming the same source as an approved noncanonical semantic replacement
    // clears it — but only for that source.
    let mut approved = input.clone_with_non_c("literal_pool");
    approved.non_c = vec![];
    approved.noncanonical_semantic_claims =
        vec![ClaimedRegion { span: s(ROM_BASE, ROM_BASE + 16), source: "src/a.c".into() }];
    let cleared = audit_core_retained(&approved).unwrap();
    assert!(
        !cleared.failures.iter().any(|f| f.starts_with("unsupported claimed-C fragment")),
        "{:?}",
        cleared.failures
    );

    approved.noncanonical_semantic_claims[0].source = "src/other.c".into();
    let wrong_owner = audit_core_retained(&approved).unwrap();
    assert!(wrong_owner
        .failures
        .iter()
        .any(|f| f.starts_with("unsupported claimed-C fragment src/a.c at")));
}

#[test]
fn failures_are_deduplicated_in_first_seen_order() {
    let mut input = base_input();
    // Two claimed entries for the same source and span produce the same string.
    input.claimed = vec![
        ClaimedRegion { span: s(ROM_BASE, ROM_BASE + 16), source: "src/a.c".into() },
        ClaimedRegion { span: s(ROM_BASE, ROM_BASE + 16), source: "src/a.c".into() },
    ];
    let result = audit_core_retained(&input).unwrap();
    let unsupported: Vec<_> = result
        .failures
        .iter()
        .filter(|f| f.starts_with("unsupported claimed-C fragment"))
        .collect();
    assert_eq!(unsupported.len(), 1);
}

#[test]
fn kind_confidence_rows_accumulate_per_pair_and_sort_by_locale_order() {
    let mut input = base_input();
    input.asm = vec![
        asm_region(ROM_BASE + 8, ROM_BASE + 16, "semantic", "semantic_c"),
        asm_region(ROM_BASE + 16, ROM_BASE + 24, "keep_asm", "retained"),
        asm_region(ROM_BASE + 24, ROM_BASE + 28, "keep_asm", "retained"),
        asm_region(ROM_BASE + 28, ROM_BASE + 32, "keep_asm", "bios_wrapper"),
    ];
    let result = audit_core_retained(&input).unwrap();
    assert_eq!(
        result.retained_by_kind_confidence,
        vec![
            KindConfidenceRow {
                kind: "bios_wrapper".into(),
                confidence: "proven".into(),
                regions: 1,
                bytes: 4
            },
            KindConfidenceRow {
                kind: "retained".into(),
                confidence: "proven".into(),
                regions: 2,
                bytes: 12
            },
        ]
    );
}

#[test]
fn the_json_report_carries_the_typescript_key_order_and_integral_numbers() {
    let mut input = base_input();
    input.paths = Some(InputPaths {
        inventory: "metrics/gs1-en-executable.json".into(),
        semantic: "semantic/main-regions.json".into(),
        asm_manifest: "out/full/asm/manifest.json".into(),
        claimed_manifest: "out/full/claimed/manifest.json".into(),
    });
    let text = json::stringify_pretty(&audit_core_retained(&input).unwrap().to_json());
    let keys: Vec<&str> = text
        .lines()
        .filter(|line| line.starts_with("  \""))
        .map(|line| line.trim().split('"').nth(1).unwrap())
        .collect();
    assert_eq!(
        keys,
        vec![
            "format",
            "kind",
            "status",
            "inputs",
            "bytes",
            "retained_by_kind_confidence",
            "failures"
        ]
    );
    // Every emitted number must be integral: `stringify_pretty` panics on a
    // non-integral value, so reaching here at all is the assertion, and this
    // pins that no `1.0` leaks out where JavaScript would print `1`.
    assert!(text.contains("\"format\": 1,"));
    assert!(!text.contains(".0"));
}

// ---------------------------------------------------------------------------
// The gate's own denominator
// ---------------------------------------------------------------------------

#[test]
fn an_empty_denominator_is_detected_rather_than_reported_as_ok() {
    assert!(!executable_is_nonempty(&[]));
    assert!(executable_is_nonempty(&[s(ROM_BASE, ROM_BASE + 4)]));
    // ...and the un-guarded audit really would call it ok, which is why the
    // binary refuses it.
    let vacuous = AuditInput {
        inventory: vec![],
        exact: vec![],
        semantic: vec![],
        non_c: vec![],
        asm: vec![],
        claimed: vec![],
        noncanonical_semantic_claims: vec![],
        paths: None,
    };
    let result = audit_core_retained(&vacuous).unwrap();
    assert!(result.status_ok && result.executable_bytes == 0);
}

// ---------------------------------------------------------------------------
// Loaders, against the real tracked inputs
// ---------------------------------------------------------------------------

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap().to_path_buf()
}

#[test]
fn the_real_tracked_inputs_load_and_reconcile() {
    let root = root();
    let inventory = main_inventory(&root.join("metrics/gs1-en-executable.json")).unwrap();
    assert!(inventory.len() > 100, "inventory looks empty: {}", inventory.len());
    let executable =
        normalized(&inventory.iter().map(|i| i.span).collect::<Vec<_>>(), "t").unwrap();
    assert!(executable_is_nonempty(&executable));

    let starts = boundaries(&root).unwrap();
    assert!(starts.len() > 1000, "boundaries look empty: {}", starts.len());
    assert!(starts.windows(2).all(|pair| pair[0] < pair[1]), "boundaries are not sorted");

    let spans = canonical_spans(&root, &executable).unwrap();
    assert!(!spans.exact.is_empty() && !spans.semantic.is_empty());

    let loaded = manifests(
        &root.join("out/full/asm/manifest.json"),
        &root.join("out/full/claimed/manifest.json"),
    )
    .unwrap();
    assert!(!loaded.asm.is_empty() && !loaded.claimed.is_empty());

    let result = audit_core_retained(&AuditInput {
        inventory,
        exact: spans.exact,
        semantic: spans.semantic,
        non_c: spans.non_c,
        asm: loaded.asm,
        claimed: loaded.claimed,
        noncanonical_semantic_claims: spans.noncanonical_semantic_claims,
        paths: None,
    })
    .unwrap();
    assert!(result.status_ok, "real tree failed the audit: {:?}", result.failures);
    assert_eq!(
        result.executable_bytes,
        result.exact_c_bytes + result.semantic_c_bytes + result.retained_bytes
    );
}

#[test]
fn the_known_padding_bug_is_reproduced_and_is_currently_latent() {
    // The mechanism: every main-image address renders as SEVEN hex digits, so
    // the unpadded entry loses a leading zero once `slice(2)` strips "0x".
    let address = 0x0800_06fc_i64;
    let buggy = format!("0x{address:x}");
    assert_eq!(&buggy[2..], "80006fc");
    assert_eq!(&hex(address)[2..], "080006fc");

    // Latency: no unregistered semantic source has an exact/ twin under either
    // spelling, so the buggy lookup and the correct one agree today. If this
    // test starts failing, the bug has gone live and must be fixed at the call
    // site in `load::semantic_spans`.
    let root = root();
    let mut checked = 0usize;
    for source in source_names(&root.join("semantic")) {
        let Some(address) = address_from_name(&source, ".c") else { continue };
        checked += 1;
        let correct = root.join("exact").join(format!("{}.c", js::to_string_16_pad_start(address, 8)));
        assert!(
            !correct.exists(),
            "the padding bug is now LIVE: {} would be missed",
            correct.display()
        );
    }
    assert!(checked > 100, "semantic tree looks empty: {checked}");
}

#[test]
fn the_semantic_loader_rejects_a_malformed_registry() {
    let root = root();
    let executable = vec![s(ROM_BASE, ROM_BASE + 0x100)];
    let starts = boundaries(&root).unwrap();
    let missing = root.join("semantic/does-not-exist.json");
    let error = semantic_spans(&root, &missing, &starts, ROM_BASE + 0x100, &executable).unwrap_err();
    assert!(error.starts_with("missing required input:"), "{error}");
}
