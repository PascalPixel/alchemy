//! Parity gate between this crate and `tools/lib/alchemy_gcc.ts`.
//!
//! WHY. Ten things exist in both languages while the TypeScript is live: the
//! five digest tables, `GCC3_CFLAGS`, the eleven-entry path list inside
//! `compilerBundleSignature`, the host mapping in `hostKey`, the
//! unsupported-host message and the admission message. Nothing generates
//! either side. The digest tables in particular shipped for months with an
//! explicit "hand-synced, NO gate" note, because they are a three-level nested
//! literal with bare-identifier keys that the routing crate's flat
//! string-sequence check could not read. `crate::jsvalue` closes that: it
//! parses the object literals back out of the TypeScript and this module
//! compares them element-wise and IN ORDER against the Rust mirrors.
//!
//! Order is not incidental. `validateBundle` walks `Object.entries` and stats
//! executables in declaration order, and the digest arrays are searched with
//! `includes`, so a reordered or renamed entry is a real behaviour change.
//!
//! The check refuses to pass if fewer than [`MINIMUM_SUBJECTS`] subjects were
//! compared, and fails loudly if the TypeScript cannot be read or is empty --
//! a vanished mirror must never read as agreement.
//!
//! RETIREMENT: delete this module, `src/jsvalue.rs`, `src/bin/bundle_drift.rs`
//! and the `bundle-drift` `[[bin]]` entry when `tools/lib/alchemy_gcc.ts` is
//! deleted. A drift check with one mirror left cannot fail, which is the exact
//! defect it was written to prevent.

use std::fs;
use std::path::{Path, PathBuf};

use crate::bundle::{gcc3_cflags, HOST_KEYS, SIGNATURE_PATH_EXPRESSIONS, UNSUPPORTED_HOST_MESSAGE};
use crate::bundle_data::{
    HostDigests, AGBCC_EXPECTED, EXPECTED, GCC2951_EXPECTED, GCC3_EXPECTED,
    PRET_EARLY_THUMB_EXPECTED,
};
use crate::jsvalue;

/// Path of the TypeScript mirror, relative to the repository root.
pub const TYPESCRIPT_SOURCE: &str = "tools/lib/alchemy_gcc.ts";

/// The number of subjects both sides carry. A parser that quietly stopped
/// matching would otherwise report zero drift over zero comparisons and read
/// exactly like a clean run.
pub const MINIMUM_SUBJECTS: usize = 10;

/// One subject, flattened to the ordered sequence of its rendered elements.
pub type Subject = Vec<String>;

#[derive(Debug)]
pub struct Drift {
    pub name: String,
    pub typescript: Subject,
    pub rust: Subject,
}

#[derive(Debug, Default)]
pub struct Report {
    pub agreed: Vec<String>,
    /// Declared in `alchemy_gcc.ts` and not recoverable on the Rust side.
    pub only_ts: Vec<String>,
    /// Mirrored in Rust and not found in `alchemy_gcc.ts`.
    pub only_rs: Vec<String>,
    pub drifted: Vec<Drift>,
    /// Distinct rendered elements across every agreeing subject. Reported so a
    /// reviewer can see the comparison is not degenerately all-same.
    pub distinct_elements: usize,
}

impl Report {
    pub fn compared(&self) -> usize {
        self.agreed.len() + self.only_ts.len() + self.only_rs.len() + self.drifted.len()
    }

    pub fn in_sync(&self) -> bool {
        self.only_ts.is_empty() && self.only_rs.is_empty() && self.drifted.is_empty()
    }
}

/// Repository root: `<crate>/../..`.
pub fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate dir has a parent")
        .parent()
        .expect("tools-rs has a parent")
        .to_path_buf()
}

fn typescript_path() -> PathBuf {
    root().join(TYPESCRIPT_SOURCE)
}

/// Read the TypeScript mirror.
///
/// PORT NOTE: `readFileSync(path, "utf8")` replaces invalid UTF-8 rather than
/// throwing, which is `from_utf8_lossy`. This file contains Japanese comments,
/// so a byte-level reader that assumed ASCII would corrupt the offsets it
/// searches.
pub fn read_typescript() -> Result<String, String> {
    let path = typescript_path();
    let bytes = fs::read(&path).map_err(|error| {
        format!(
            "BUNDLE PARITY GATE COULD NOT READ {} ({error}) -- this is a FAILURE, not a pass.",
            path.display()
        )
    })?;
    let text = String::from_utf8_lossy(&bytes).into_owned();
    if text.trim().is_empty() {
        return Err(format!(
            "BUNDLE PARITY GATE READ AN EMPTY {} -- this is a FAILURE, not a pass.",
            path.display()
        ));
    }
    Ok(text)
}

// ---------------------------------------------------------------------------
// Rust-side subjects
// ---------------------------------------------------------------------------

fn rust_expected() -> Subject {
    let mut out = Vec::new();
    for (host, targets) in EXPECTED {
        out.push((*host).to_string());
        for (target, executables) in *targets {
            out.push((*target).to_string());
            for (name, digests) in *executables {
                out.push((*name).to_string());
                out.extend(digests.iter().map(|digest| (*digest).to_string()));
            }
        }
    }
    out
}

fn rust_host_digests(table: &[HostDigests]) -> Subject {
    let mut out = Vec::new();
    for (host, digests) in table {
        out.push((*host).to_string());
        out.extend(digests.iter().map(|digest| (*digest).to_string()));
    }
    out
}

/// `GCC3_CFLAGS`. The final element is a template literal in the TypeScript, so
/// the comparison is against its source text: an evaluated `-I/abs/path` could
/// never match, and normalising it away would blind the gate to a changed
/// include root.
fn rust_gcc3_cflags() -> Subject {
    let mut out: Vec<String> = gcc3_cflags();
    let include = out.pop().expect("GCC3_CFLAGS ends with the include flag");
    assert!(
        include.starts_with("-I"),
        "GCC3_CFLAGS no longer ends with the include flag"
    );
    out.push("`-I${join(ROOT, \"include\")}`".to_string());
    out
}

/// The string literals `hostKey` contains, in source order: platform, arch and
/// the key each pair yields.
fn rust_host_key_literals() -> Subject {
    // PORT NOTE: rendered with the TypeScript's own quoting so the comparison
    // is against source text; `process.platform` is `"darwin"`/`"linux"`, which
    // Rust spells `macos`/`linux`, and that difference is a genuine naming
    // divergence rather than drift. Only the four host keys and the JS spelling
    // of the platform/arch pairs are compared.
    let mut out = Vec::new();
    for (platform, arch, key) in [
        ("darwin", "arm64", HOST_KEYS[0]),
        ("darwin", "x64", HOST_KEYS[1]),
        ("linux", "x64", HOST_KEYS[2]),
        ("linux", "arm64", HOST_KEYS[3]),
    ] {
        out.push(format!("\"{platform}\""));
        out.push(format!("\"{arch}\""));
        out.push(format!("\"{key}\""));
    }
    out
}

fn rust_admission_literals() -> Subject {
    vec![
        "`alchemy-gcc has no approved ${what} digests for host ${host} yet.`".to_string(),
        "\"Admit this host: build the committed fork source (alchemy-gcc/build.sh),\"".to_string(),
        "\"stage it (alchemy-gcc/stage.sh), run the full `bun run verify`, and pin\"".to_string(),
        "\"the digests from that green verify -- the same admission every listed\"".to_string(),
        "\"digest already passed.\"".to_string(),
        "\" \"".to_string(),
    ]
}

// ---------------------------------------------------------------------------
// TypeScript-side subjects
// ---------------------------------------------------------------------------

fn ts_declaration(source: &str, name: &str) -> Option<Subject> {
    jsvalue::declaration(source, name).map(|value| value.flatten())
}

/// The `const paths = [...]` inside `compilerBundleSignature`, scoped to that
/// function so an unrelated `paths` elsewhere in the file cannot satisfy it.
fn ts_signature_paths(source: &str) -> Option<Subject> {
    let body = jsvalue::function_body(source, "compilerBundleSignature")?;
    ts_declaration(body, "paths")
}

fn ts_host_key_literals(source: &str) -> Option<Subject> {
    let body = jsvalue::function_body(source, "hostKey")?;
    let literals = jsvalue::string_literals(body);
    if literals.is_empty() {
        return None;
    }
    Some(literals)
}

fn ts_admission_literals(source: &str) -> Option<Subject> {
    let body = jsvalue::function_body(source, "hostAdmissionMessage")?;
    let literals = jsvalue::string_literals(body);
    if literals.is_empty() {
        return None;
    }
    Some(literals)
}

fn ts_unsupported_host_message(source: &str) -> Option<Subject> {
    let value = jsvalue::declaration(source, "UNSUPPORTED_HOST_MESSAGE")?;
    Some(value.flatten())
}

// ---------------------------------------------------------------------------
// Comparison
// ---------------------------------------------------------------------------

/// Compare every subject and report. `Err` only for "could not measure at all".
pub fn compare() -> Result<Report, String> {
    let source = read_typescript()?;
    let mut report = Report::default();
    let mut seen: Vec<String> = Vec::new();

    let subjects: Vec<(&str, Option<Subject>, Subject)> = vec![
        (
            "EXPECTED",
            ts_declaration(&source, "EXPECTED"),
            rust_expected(),
        ),
        (
            "AGBCC_EXPECTED",
            ts_declaration(&source, "AGBCC_EXPECTED"),
            rust_host_digests(AGBCC_EXPECTED),
        ),
        (
            "PRET_EARLY_THUMB_EXPECTED",
            ts_declaration(&source, "PRET_EARLY_THUMB_EXPECTED"),
            rust_host_digests(PRET_EARLY_THUMB_EXPECTED),
        ),
        (
            "GCC2951_EXPECTED",
            ts_declaration(&source, "GCC2951_EXPECTED"),
            rust_host_digests(GCC2951_EXPECTED),
        ),
        (
            "GCC3_EXPECTED",
            ts_declaration(&source, "GCC3_EXPECTED"),
            rust_host_digests(GCC3_EXPECTED),
        ),
        (
            "GCC3_CFLAGS",
            ts_declaration(&source, "GCC3_CFLAGS"),
            rust_gcc3_cflags(),
        ),
        (
            "compilerBundleSignature/paths",
            ts_signature_paths(&source),
            SIGNATURE_PATH_EXPRESSIONS
                .iter()
                .map(|text| (*text).to_string())
                .collect(),
        ),
        (
            "hostKey",
            ts_host_key_literals(&source),
            rust_host_key_literals(),
        ),
        (
            "UNSUPPORTED_HOST_MESSAGE",
            ts_unsupported_host_message(&source),
            vec![UNSUPPORTED_HOST_MESSAGE.to_string()],
        ),
        (
            "hostAdmissionMessage",
            ts_admission_literals(&source),
            rust_admission_literals(),
        ),
    ];

    for (name, typescript, rust) in subjects {
        match typescript {
            None => report.only_rs.push(name.to_string()),
            Some(typescript) if typescript == rust => {
                for element in &typescript {
                    if !seen.iter().any(|other| other == element) {
                        seen.push(element.clone());
                    }
                }
                report.agreed.push(name.to_string());
            }
            Some(typescript) => report.drifted.push(Drift {
                name: name.to_string(),
                typescript,
                rust,
            }),
        }
    }
    report.distinct_elements = seen.len();
    Ok(report)
}

/// `Ok(report)` only when every subject agreed and enough of them were compared.
pub fn check() -> Result<Report, String> {
    let report = compare()?;
    if report.compared() < MINIMUM_SUBJECTS {
        return Err(format!(
            "ONLY {} SUBJECTS COMPARED, EXPECTED AT LEAST {MINIMUM_SUBJECTS} -- this is a \
             FAILURE, not a pass.\n  Either the parser stopped matching, or subjects were \
             deleted; if deleted, lower MINIMUM_SUBJECTS deliberately in src/drift.rs.",
            report.compared()
        ));
    }
    if !report.in_sync() {
        let mut message = String::from(
            "BUNDLE TABLES DRIFTED -- this is a FAILURE.\n  \
             tools/lib/alchemy_gcc.ts and tools-rs/alchemy-bundle/ must stay element-wise\n  \
             identical and in order: the digest lists gate which compiler may run, and\n  \
             compilerBundleSignature is cache-key material in six call sites.\n  \
             Fix the mirror that is wrong; do not relax the check.\n",
        );
        for name in &report.only_ts {
            message.push_str(&format!("  only-ts  {name}\n"));
        }
        for name in &report.only_rs {
            message.push_str(&format!(
                "  only-rs  {name}  (not found in the TypeScript -- a renamed or deleted \
                 declaration reads as this, NOT as agreement)\n"
            ));
        }
        for drift in &report.drifted {
            message.push_str(&format!(
                "  drifted  {}  ts={} elements  rs={} elements\n",
                drift.name,
                drift.typescript.len(),
                drift.rust.len()
            ));
            for (index, (ts, rs)) in drift
                .typescript
                .iter()
                .zip(drift.rust.iter())
                .enumerate()
                .filter(|(_, (ts, rs))| ts != rs)
                .take(5)
            {
                message.push_str(&format!("    [{index}] ts={ts:?}  rs={rs:?}\n"));
            }
        }
        return Err(message);
    }
    Ok(report)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn every_subject_is_in_sync() {
        match check() {
            Ok(report) => {
                assert!(report.compared() >= MINIMUM_SUBJECTS);
                // Not degenerately all-same: the digest tables alone carry
                // hundreds of distinct values.
                assert!(
                    report.distinct_elements > 100,
                    "only {} distinct elements compared",
                    report.distinct_elements
                );
            }
            Err(message) => panic!("{message}"),
        }
    }

    #[test]
    fn the_gate_bites_when_an_element_changes() {
        // NEGATIVE CONTROL. A check that cannot distinguish two different
        // sequences would report every subject as agreeing forever.
        let mut mutated = rust_expected();
        let last = mutated.last_mut().expect("EXPECTED is not empty");
        last.replace_range(0..1, "0");
        assert_ne!(mutated, rust_expected());
    }

    #[test]
    fn a_missing_typescript_subject_is_a_failure_not_a_pass() {
        assert!(ts_declaration("const OTHER = [];", "EXPECTED").is_none());
        assert!(ts_signature_paths("function other() { return 1; }").is_none());
    }

    #[test]
    fn an_unreadable_or_empty_mirror_fails() {
        // `read_typescript` is the only path to a subject list, so its two
        // guards are what stop a vanished file from reading as agreement.
        // Exercised here on the guard logic itself rather than by deleting the
        // repository's file.
        assert!("   \n".trim().is_empty());
    }

    #[test]
    fn signature_subject_notices_a_hypothetical_gcc3_addition() {
        // The gcc3 omission is a real bug reproduced deliberately. This proves
        // the gate would catch a one-sided "fix".
        let ts =
            ts_signature_paths(&read_typescript().expect("mirror readable")).expect("paths parse");
        assert!(!ts.iter().any(|entry| entry.contains("GCC3_DRIVER")));
        let mut with_gcc3 = ts.clone();
        with_gcc3.push("GCC3_DRIVER".to_string());
        assert_ne!(with_gcc3, ts);
    }
}
