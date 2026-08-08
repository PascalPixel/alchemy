//! Parity check between `routing_data.rs` and `tools/lib/alchemy_gcc.ts`.
//!
//! The ~137 routing tables exist twice, once in each language, and until the
//! TypeScript is deleted both are live: `alchemy_gcc.ts` routes every Bun tool,
//! `routing_data.rs` routes `verify`. Nothing generates either file. Left to
//! hand discipline, an edit to one side and not the other silently produces a
//! different ROM depending on which tool built it.
//!
//! So this module measures the parity instead of promising it. It parses the
//! `const NAME = new Set([...])` / `new Map([...])` declarations back out of the
//! TypeScript source and the `pub static NAME: &[...] = &[...]` declarations out
//! of `routing_data.rs`, and compares them element-wise, in order. Order matters:
//! `cflags_for_source` emits flags in table order and gcc is later-flag-wins.
//!
//! RETIREMENT: this check exists only because the TypeScript does. When
//! `tools/lib/alchemy_gcc.ts` is deleted, delete `drift.rs`, `bin/routing_drift.rs`
//! and the `[[bin]]` entry with it -- a comparison with only one side left is
//! exactly the check-that-cannot-fail this module was written to prevent.

use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};

use crate::routing::root;

/// The number of routing tables both sides carry today. The check refuses to
/// pass below this: a parser that silently stops matching would otherwise
/// report `drifted=0` over two tables and read exactly like a clean run.
/// Raising it is correct when tables are added; lowering it means tables were
/// deleted and should be a deliberate, separate edit.
pub const MINIMUM_TABLES: usize = 137;

/// Floors on what the TypeScript scan must actually FIND, per category. A table
/// count alone does not prove the scan saw table *contents*: an extractor that
/// matched 137 declarations and read zero elements out of each would agree with
/// a Rust side it had also read as empty. These are the measured 2026-08-07
/// values; raise them when data is added, lower them only deliberately.
///
/// `MINIMUM_ELEMENTS` counts every literal in the TypeScript tables, keys and
/// values alike. `MINIMUM_DISTINCT_ELEMENTS` counts them once each, which is
/// what proves the corpus is not degenerate: 137 tables all holding the same
/// stem would clear an element floor and prove nothing.
pub const MINIMUM_ELEMENTS: usize = 1073;
/// See [`MINIMUM_ELEMENTS`].
pub const MINIMUM_DISTINCT_ELEMENTS: usize = 753;
/// `new Set` declarations the TypeScript scan must find. Split from the `Map`
/// floor because the two spellings are matched by different branches of the
/// parser, and a total-only floor lets one branch die unnoticed behind the
/// other.
pub const MINIMUM_SET_TABLES: usize = 135;
/// See [`MINIMUM_SET_TABLES`]. There are exactly two `new Map` tables
/// (`THUMB_LOW_REG_ORDER_SOURCES`, `THUMB_CALLEE_REG_ORDER_SOURCES`) and they
/// are the only ones exercising the pair-flattening path on both sides.
pub const MINIMUM_MAP_TABLES: usize = 2;

/// Path of the TypeScript mirror, relative to the repository root.
pub const TYPESCRIPT_SOURCE: &str = "tools/lib/alchemy_gcc.ts";
/// Path of the Rust mirror, relative to this crate.
pub const RUST_SOURCE: &str = "src/routing_data.rs";

/// One table, flattened to the ordered sequence of its string elements. A
/// `Map` contributes key and value alternately, which is exactly how the Rust
/// side stores its `&[(&str, &str)]` pairs.
pub type Table = Vec<String>;

/// Which JavaScript container a TypeScript table was declared as. Tracked only
/// so the per-category floors can tell the two parser branches apart.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Kind {
    Set,
    Map,
}

/// The outcome of one comparison.
#[derive(Debug, Default)]
pub struct Report {
    /// Literals read out of the TypeScript tables, keys and values alike.
    pub total_elements: usize,
    /// Those literals counted once each. Proves the corpus is not degenerate.
    pub distinct_elements: usize,
    /// TypeScript tables declared `new Set`.
    pub set_tables: usize,
    /// TypeScript tables declared `new Map`.
    pub map_tables: usize,
    /// Tables present on both sides and identical, element for element.
    pub agreed: Vec<String>,
    /// Declared in `alchemy_gcc.ts` and missing from `routing_data.rs`.
    pub only_ts: Vec<String>,
    /// Declared in `routing_data.rs` and missing from `alchemy_gcc.ts`.
    pub only_rs: Vec<String>,
    /// Present on both sides with differing contents, and how they differ.
    pub drifted: Vec<Drift>,
}

/// One table whose two copies disagree.
#[derive(Debug)]
pub struct Drift {
    pub name: String,
    pub typescript: Table,
    pub rust: Table,
}

impl Report {
    /// Tables compared in total: the union of both sides.
    pub fn compared(&self) -> usize {
        self.agreed.len() + self.only_ts.len() + self.only_rs.len() + self.drifted.len()
    }

    /// True when the two mirrors are element-wise identical.
    pub fn in_sync(&self) -> bool {
        self.only_ts.is_empty() && self.only_rs.is_empty() && self.drifted.is_empty()
    }

    /// Every floor this report falls below, phrased for a human. Empty means
    /// the scan found enough to be worth believing.
    ///
    /// SCANNING NOTHING IS NOT PASSING. An empty or unreadable `alchemy_gcc.ts`
    /// yields zero tables, zero elements and zero of each kind, which without
    /// these floors is indistinguishable from a clean sweep. Each floor is a
    /// separate assertion on purpose: they fail one parser branch at a time and
    /// so name which one stopped matching.
    pub fn floor_failures(&self) -> Vec<String> {
        let mut failures = Vec::new();
        let mut check = |what: &str, got: usize, floor: usize| {
            if got < floor {
                failures.push(format!("{what}: found {got}, floor is {floor}"));
            }
        };
        // Floored on the TypeScript side, NOT on `compared()`. `compared()` is
        // the union of both mirrors, so a TypeScript file that had vanished or
        // gone empty would still report 137 "compared" tables off the Rust side
        // alone -- the exact can-pass-without-looking failure this floor exists
        // to catch. The union count is still printed; it is just not the gate.
        check(
            "TypeScript tables",
            self.set_tables + self.map_tables,
            MINIMUM_TABLES,
        );
        check("elements scanned", self.total_elements, MINIMUM_ELEMENTS);
        check(
            "distinct elements",
            self.distinct_elements,
            MINIMUM_DISTINCT_ELEMENTS,
        );
        check("`new Set` tables", self.set_tables, MINIMUM_SET_TABLES);
        check("`new Map` tables", self.map_tables, MINIMUM_MAP_TABLES);
        failures
    }

    /// True when every floor in [`Report::floor_failures`] is cleared.
    pub fn floors_met(&self) -> bool {
        self.floor_failures().is_empty()
    }

    /// The whole gate: the scan found enough, and what it found agrees.
    pub fn passed(&self) -> bool {
        self.floors_met() && self.in_sync()
    }
}

fn typescript_path() -> PathBuf {
    root().join(TYPESCRIPT_SOURCE)
}

fn rust_path() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join(RUST_SOURCE)
}

/// Compare the two mirrors on the real tree.
///
/// Errors when either file is unreadable. A missing `alchemy_gcc.ts` is an
/// error rather than a pass: while this module exists, the TypeScript is
/// supposed to exist too, and "the other side vanished" must never look like
/// agreement. When the TypeScript is deleted for real, delete this module.
pub fn compare() -> Result<Report, String> {
    let (ts_text, rs_text) = read_sources()?;
    Ok(compare_text(&ts_text, &rs_text))
}

/// Read both mirrors off the real tree. Split out so the negative control can
/// perturb the real corpus rather than a toy one: a control run against a
/// two-table fixture proves the comparator works on fixtures, not that it works
/// on the 137 tables the gate actually has to police.
pub fn read_sources() -> Result<(String, String), String> {
    let ts_path = typescript_path();
    let rs_path = rust_path();
    let ts_text = fs::read_to_string(&ts_path)
        .map_err(|error| format!("cannot read {}: {error}", ts_path.display()))?;
    let rs_text = fs::read_to_string(&rs_path)
        .map_err(|error| format!("cannot read {}: {error}", rs_path.display()))?;
    Ok((ts_text, rs_text))
}

/// Compare two already-read sources. Split out so the parsers are testable
/// without touching the tree.
pub fn compare_text(ts_text: &str, rs_text: &str) -> Report {
    let kinded = parse_typescript_kinds(ts_text);
    let typescript: BTreeMap<String, Table> = kinded
        .iter()
        .map(|(name, (_, table))| (name.clone(), table.clone()))
        .collect();
    let rust = parse_rust(rs_text);
    let mut report = Report::default();
    let mut distinct = std::collections::BTreeSet::new();
    for (kind, table) in kinded.values() {
        match kind {
            Kind::Set => report.set_tables += 1,
            Kind::Map => report.map_tables += 1,
        }
        report.total_elements += table.len();
        distinct.extend(table.iter().cloned());
    }
    report.distinct_elements = distinct.len();
    for (name, ts_table) in &typescript {
        match rust.get(name) {
            None => report.only_ts.push(name.clone()),
            Some(rs_table) if rs_table == ts_table => report.agreed.push(name.clone()),
            Some(rs_table) => report.drifted.push(Drift {
                name: name.clone(),
                typescript: ts_table.clone(),
                rust: rs_table.clone(),
            }),
        }
    }
    for name in rust.keys() {
        if !typescript.contains_key(name) {
            report.only_rs.push(name.clone());
        }
    }
    report
}

// ---------------------------------------------------------------------------
// parsing
// ---------------------------------------------------------------------------

/// Read the double-quoted string literals of one bracketed initializer,
/// starting at `start` (the byte offset of its opening `(` or `[`), and return
/// them in source order together with the offset just past the initializer.
///
/// This is a scanner, not a parser: it tracks bracket depth so it knows where
/// the declaration ends, skips `//` and `/* */` comments so their prose cannot
/// contribute literals, and honours backslash escapes inside strings. Both
/// mirrors hold nothing but string literals, so collecting them in order and
/// comparing is a complete element-wise comparison. Nesting is deliberately
/// flattened: a `Map`'s `["k", "v"]` pairs land as `k, v, ...`, matching the
/// Rust `&[(&str, &str)]` layout.
fn scan_literals(text: &str, start: usize) -> (Vec<String>, usize) {
    let bytes = text.as_bytes();
    let mut literals = Vec::new();
    let mut depth = 0usize;
    let mut index = start;
    while index < bytes.len() {
        match bytes[index] {
            b'(' | b'[' | b'{' => {
                depth += 1;
                index += 1;
            }
            b')' | b']' | b'}' => {
                depth -= 1;
                index += 1;
                if depth == 0 {
                    break;
                }
            }
            b'"' | b'\'' => {
                let quote = bytes[index];
                let mut value = String::new();
                index += 1;
                while index < bytes.len() && bytes[index] != quote {
                    if bytes[index] == b'\\' && index + 1 < bytes.len() {
                        index += 1;
                    }
                    value.push(bytes[index] as char);
                    index += 1;
                }
                index += 1;
                literals.push(value);
            }
            b'/' if index + 1 < bytes.len() && bytes[index + 1] == b'/' => {
                while index < bytes.len() && bytes[index] != b'\n' {
                    index += 1;
                }
            }
            b'/' if index + 1 < bytes.len() && bytes[index + 1] == b'*' => {
                index += 2;
                while index + 1 < bytes.len() && !(bytes[index] == b'*' && bytes[index + 1] == b'/')
                {
                    index += 1;
                }
                index += 2;
            }
            _ => index += 1,
        }
    }
    (literals, index)
}

/// True for the characters a SCREAMING_SNAKE table name is made of.
fn is_name_char(byte: u8) -> bool {
    byte.is_ascii_uppercase() || byte.is_ascii_digit() || byte == b'_'
}

/// Extract every `const NAME = new Set(...)` / `new Map(...)` table from the
/// TypeScript. Only SCREAMING_SNAKE names are tables; the module's lowercase
/// `new Set` memo state (`validated`, `experimentalValidated`) is not one, and
/// has no Rust counterpart in this file.
pub fn parse_typescript(text: &str) -> BTreeMap<String, Table> {
    parse_typescript_kinds(text)
        .into_iter()
        .map(|(name, (_, table))| (name, table))
        .collect()
}

/// As [`parse_typescript`], but keeping which container each table was declared
/// as. The per-category floors need it; nothing else does.
pub fn parse_typescript_kinds(text: &str) -> BTreeMap<String, (Kind, Table)> {
    let mut tables = BTreeMap::new();
    let bytes = text.as_bytes();
    let mut index = 0usize;
    while let Some(offset) = text[index..].find("const ") {
        let mut cursor = index + offset + "const ".len();
        // Only a declaration at the start of a line is a table declaration.
        let at_line_start = index + offset == 0 || bytes[index + offset - 1] == b'\n';
        let name_start = cursor;
        while cursor < bytes.len() && is_name_char(bytes[cursor]) {
            cursor += 1;
        }
        let name = &text[name_start..cursor];
        index = name_start.max(index + offset + 1);
        if !at_line_start || name.is_empty() {
            continue;
        }
        let rest = &text[cursor..];
        let Some(kind_offset) = rest.strip_prefix(" = new ") else {
            continue;
        };
        let (kind, kind_offset) = if let Some(tail) = kind_offset.strip_prefix("Set") {
            (Kind::Set, tail)
        } else if let Some(tail) = kind_offset.strip_prefix("Map") {
            (Kind::Map, tail)
        } else {
            continue;
        };
        // An optional type argument: `new Set<string>([...])`.
        let after_type = match kind_offset.strip_prefix('<') {
            Some(tail) => match tail.find('>') {
                Some(close) => &tail[close + 1..],
                None => continue,
            },
            None => kind_offset,
        };
        if !after_type.starts_with('(') {
            continue;
        }
        let open = text.len() - after_type.len();
        let (literals, end) = scan_literals(text, open);
        tables.insert(name.to_string(), (kind, literals));
        index = end;
    }
    tables
}

/// Extract every `pub static NAME: ... = &[...];` table from `routing_data.rs`.
pub fn parse_rust(text: &str) -> BTreeMap<String, Table> {
    let mut tables = BTreeMap::new();
    let mut index = 0usize;
    while let Some(offset) = text[index..].find("pub static ") {
        let mut cursor = index + offset + "pub static ".len();
        let name_start = cursor;
        let bytes = text.as_bytes();
        while cursor < bytes.len() && is_name_char(bytes[cursor]) {
            cursor += 1;
        }
        let name = text[name_start..cursor].to_string();
        index = name_start.max(index + offset + 1);
        if name.is_empty() {
            continue;
        }
        let Some(equals) = text[cursor..].find('=') else {
            continue;
        };
        let Some(open) = text[cursor + equals..].find('[') else {
            continue;
        };
        let open = cursor + equals + open;
        let (literals, end) = scan_literals(text, open);
        tables.insert(name, literals);
        index = end;
    }
    tables
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn scanner_ignores_prose_in_comments() {
        let source = r#"const A_SOURCES = new Set([
  // a comment with "quoted" prose and a ] bracket
  "08000000",
  /* block "comment" */ "08000004",
]);"#;
        let tables = parse_typescript(source);
        assert_eq!(tables["A_SOURCES"], vec!["08000000", "08000004"]);
    }

    #[test]
    fn scanner_reads_typed_sets_maps_and_empties() {
        let source = concat!(
            "const B_SOURCES = new Set<string>([\"08000000\"]);\n",
            "const C_SOURCES = new Set<string>();\n",
            "const D_SOURCES = new Map([\n  [\"a\", \"b\"],\n  [\"c\", \"d\"],\n]);\n",
            "const validated = new Set<CompilerTarget>();\n",
        );
        let tables = parse_typescript(source);
        assert_eq!(tables["B_SOURCES"], vec!["08000000"]);
        assert!(tables["C_SOURCES"].is_empty());
        assert_eq!(tables["D_SOURCES"], vec!["a", "b", "c", "d"]);
        assert!(!tables.contains_key("validated"));
    }

    #[test]
    fn rust_parser_reads_slices_and_pair_slices() {
        let source = concat!(
            "pub static B_SOURCES: &[&str] = &[\"08000000\"];\n",
            "pub static C_SOURCES: &[&str] = &[];\n",
            "pub static D_SOURCES: &[(&str, &str)] = &[(\"a\", \"b\"), (\"c\", \"d\")];\n",
        );
        let tables = parse_rust(source);
        assert_eq!(tables["B_SOURCES"], vec!["08000000"]);
        assert!(tables["C_SOURCES"].is_empty());
        assert_eq!(tables["D_SOURCES"], vec!["a", "b", "c", "d"]);
    }

    #[test]
    fn comparison_names_each_kind_of_disagreement() {
        let report = compare_text(
            "const A_SOURCES = new Set([\"1\"]);\nconst B_SOURCES = new Set([\"2\"]);\n",
            "pub static A_SOURCES: &[&str] = &[\"1\"];\npub static C_SOURCES: &[&str] = &[\"3\"];\n",
        );
        assert_eq!(report.agreed, vec!["A_SOURCES"]);
        assert_eq!(report.only_ts, vec!["B_SOURCES"]);
        assert_eq!(report.only_rs, vec!["C_SOURCES"]);
        assert!(report.drifted.is_empty());
        assert!(!report.in_sync());
    }

    #[test]
    fn element_order_is_load_bearing() {
        let report = compare_text(
            "const A_SOURCES = new Set([\"1\", \"2\"]);\n",
            "pub static A_SOURCES: &[&str] = &[\"2\", \"1\"];\n",
        );
        assert_eq!(report.drifted.len(), 1);
        assert_eq!(report.drifted[0].name, "A_SOURCES");
    }

    /// An unreadable or empty TypeScript side must FAIL, not read as a clean
    /// sweep. `compare()` errors when the file is missing; this covers the
    /// nastier case where the file is present but says nothing.
    #[test]
    fn an_empty_typescript_source_hard_fails() {
        let (_, rs_text) = read_sources().expect("both mirrors must be readable");
        let report = compare_text("", &rs_text);
        assert!(report.drifted.is_empty(), "nothing to drift against");
        assert!(
            !report.floors_met(),
            "an empty TypeScript source cleared the floors, so the gate can pass \
             without reading anything"
        );
        assert!(!report.passed());
        // Every floor should fire, not just the table count.
        assert_eq!(
            report.floor_failures().len(),
            5,
            "{:?}",
            report.floor_failures()
        );
    }

    /// NEGATIVE CONTROL, on the real corpus, run before the real comparison
    /// below. Perturb exactly one element of one real table and prove the
    /// harness reports exactly one difference in exactly that table. Without
    /// this, a green run only proves the comparator returned; it does not prove
    /// the comparator can detect anything at all.
    #[test]
    fn negative_control_one_perturbed_element_yields_exactly_one_difference() {
        let (ts_text, rs_text) = read_sources().expect("both mirrors must be readable");

        // A single-element table with a stem that appears nowhere else, so the
        // substitution is provably confined to one element of one table.
        let original = r#"const OPTIMIZE_OS_SOURCES = new Set(["08019d2c"]);"#;
        let perturbed = r#"const OPTIMIZE_OS_SOURCES = new Set(["08019d2d"]);"#;
        assert_eq!(
            ts_text.matches(original).count(),
            1,
            "the control needs exactly one anchor to perturb; the declaration it \
             keys on changed shape"
        );
        let mutated = ts_text.replace(original, perturbed);
        assert_ne!(
            mutated, ts_text,
            "the control did not actually perturb anything"
        );

        let report = compare_text(&mutated, &rs_text);
        assert!(
            report.floors_met(),
            "the control must still scan the full corpus: {:?}",
            report.floor_failures()
        );
        assert!(report.only_ts.is_empty() && report.only_rs.is_empty());
        assert_eq!(
            report.drifted.len(),
            1,
            "expected exactly one difference, got {:?}",
            report.drifted.iter().map(|d| &d.name).collect::<Vec<_>>()
        );
        assert_eq!(report.drifted[0].name, "OPTIMIZE_OS_SOURCES");
        assert_eq!(report.drifted[0].typescript, vec!["08019d2d"]);
        assert_eq!(report.drifted[0].rust, vec!["08019d2c"]);
        assert!(!report.passed());
    }

    /// The floors are met by the real corpus, and by enough margin to be worth
    /// stating: this is the "things found" assertion the table count alone
    /// cannot make.
    #[test]
    fn the_real_corpus_clears_every_floor() {
        let report = compare().expect("both mirrors must be readable");
        assert!(report.floors_met(), "{:?}", report.floor_failures());
        assert!(report.map_tables >= MINIMUM_MAP_TABLES);
        assert!(report.set_tables >= MINIMUM_SET_TABLES);
        assert!(report.distinct_elements >= MINIMUM_DISTINCT_ELEMENTS);
        // Not degenerate: most elements are unique, so agreement is evidence.
        assert!(report.distinct_elements * 2 > report.total_elements);
    }

    /// The check itself, on the real tree. This is the assertion that keeps the
    /// two mirrors honest; everything above only proves the parsers work.
    #[test]
    fn the_two_mirrors_agree_on_the_real_tree() {
        let report = compare().expect("both mirrors must be readable");
        assert!(
            report.floors_met(),
            "{:?} — a check that can pass without looking is the defect it polices",
            report.floor_failures()
        );
        assert!(
            report.in_sync(),
            "routing tables drifted: only-ts={:?} only-rs={:?} drifted={:?}",
            report.only_ts,
            report.only_rs,
            report.drifted.iter().map(|d| &d.name).collect::<Vec<_>>()
        );
    }
}
