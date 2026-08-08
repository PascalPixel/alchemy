// Prove that a named showcase overlay is closed, descriptively named, and
// still reconstructs the reviewed decoded image byte for byte.
//
// Ported from tools/overlay/overlay_showcase.ts.
//
// PORT NOTE — what was inlined, and from where.
//
//   * `canonical_c_source` is `canonicalCSource` from
//     full-c-progress (four rejection patterns, hand-rolled here
//     because the crate takes no regex dependency). JavaScript `\b` is the
//     ASCII word boundary over [0-9A-Za-z_], which `is_word_byte` reproduces.
//
//   * `assembleOverlay` from tools/lib/overlay_disasm.ts is NOT reimplemented.
//     It splices every adopted exact-C owner's compiled bytes into the overlay
//     image, which drags in the whole alchemy-gcc compilation plan: the routed
//     flag tables, the bundle digest validation, the source-keyed content cache,
//     the label-word bias and the external-symbol rules — several thousand lines
//     of policy whose parity cannot be argued, only re-derived. This port shells
//     out to `bun` for that one value (see `assemble_command` in main.rs), the
//     same way the tool already shells out to `bun` for its four sub-checks. The
//     Rust side receives the image's byte length and sha256 and does every
//     comparison itself.
//
// PORT NOTE — deliberate behaviour differences, all of them narrow:
//
//   * Exact-C owner filenames are sorted before they are checked. The TypeScript
//     walks `readdirSync` order, so which of several bad owners is reported first
//     is filesystem-dependent; the count check and the pass/fail verdict are
//     unaffected.
//
//   * A failure is written to stderr as the bare message and exits 1, rather
//     than as bun's uncaught-exception stack. The message text itself is
//     identical, including "usage: overlay_showcase.ts resource_NNN", which
//     keeps naming the TypeScript entry point on purpose: both spellings of the
//     tool must tell a reader the same thing.
//
//   * The manifest's `overlays` rows must carry every field this tool reads. The
//     TypeScript casts the parsed JSON and would compare `undefined` — silently
//     failing the byte check rather than saying the manifest is malformed.

use std::fmt;
use std::fs;
use std::io;
use std::path::Path;

/// A registered showcase overlay, as spelled in metrics/overlay-showcases.json.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Showcase {
    pub id: String,
    pub decoded_bytes: u64,
    pub exact_c_owners: usize,
    pub sha256: String,
    pub retained_assembly: String,
}

#[derive(Debug)]
pub struct Failure(pub String);

impl fmt::Display for Failure {
    fn fmt(&self, formatter: &mut fmt::Formatter<'_>) -> fmt::Result {
        formatter.write_str(&self.0)
    }
}

impl std::error::Error for Failure {}

pub fn fail<T>(message: impl Into<String>) -> Result<T, Failure> {
    Err(Failure(message.into()))
}

fn is_word_byte(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || byte == b'_'
}

/// JavaScript `\b` immediately before `index`, i.e. a word/non-word transition.
fn boundary_at(bytes: &[u8], index: usize) -> bool {
    let before = index > 0 && is_word_byte(bytes[index - 1]);
    let after = index < bytes.len() && is_word_byte(bytes[index]);
    before != after
}

/// `\b<word>\b` anchored at `index`.
fn word_at(bytes: &[u8], index: usize, word: &[u8]) -> bool {
    if !bytes[index..].starts_with(word) {
        return false;
    }
    boundary_at(bytes, index) && boundary_at(bytes, index + word.len())
}

fn has_word(text: &str, word: &str) -> bool {
    let bytes = text.as_bytes();
    (0..bytes.len()).any(|index| word_at(bytes, index, word.as_bytes()))
}

/// `/\bregister\b[^;\n]*\basm\s*\(/`
fn register_asm_call(bytes: &[u8]) -> bool {
    for start in 0..bytes.len() {
        if !word_at(bytes, start, b"register") {
            continue;
        }
        // `[^;\n]*` cannot cross a statement end or a line end, so the window in
        // which `asm` may begin stops there. `\s*` after `asm` may cross a
        // newline, which is why only the start of `asm` is windowed.
        let mut window = start + "register".len();
        while window < bytes.len() && bytes[window] != b';' && bytes[window] != b'\n' {
            window += 1;
        }
        for site in start + "register".len()..=window.min(bytes.len().saturating_sub(1)) {
            if !word_at(bytes, site, b"asm") {
                continue;
            }
            let mut after = site + 3;
            while after < bytes.len() && (bytes[after] as char).is_whitespace() {
                after += 1;
            }
            if bytes.get(after) == Some(&b'(') {
                return true;
            }
        }
    }
    false
}

/// `/\basm\s+volatile\b/`
fn asm_volatile(bytes: &[u8]) -> bool {
    for start in 0..bytes.len() {
        if !bytes[start..].starts_with(b"asm") || !boundary_at(bytes, start) {
            continue;
        }
        // No `\b` after `asm` in the pattern: `\s+` supplies the separation.
        let mut after = start + 3;
        let spaced = {
            let begin = after;
            while after < bytes.len() && (bytes[after] as char).is_whitespace() {
                after += 1;
            }
            after > begin
        };
        if spaced && word_at(bytes, after, b"volatile") {
            return true;
        }
    }
    false
}

/// `/\.incbin\b/`
fn incbin(bytes: &[u8]) -> bool {
    (0..bytes.len()).any(|start| {
        bytes[start..].starts_with(b".incbin") && boundary_at(bytes, start + ".incbin".len())
    })
}

/// Canonical Exact C: no inline assembly, no register-pinned fakematch, no
/// incbin escape hatch, no m2c error marker.
pub fn canonical_c_source(source: &str) -> bool {
    let bytes = source.as_bytes();
    !(register_asm_call(bytes)
        || has_word(source, "__asm__")
        || asm_volatile(bytes)
        || incbin(bytes)
        || has_word(source, "M2C_ERROR"))
}

/// `/^resource_[0-9a-f]+$/`
pub fn is_showcase_id(id: &str) -> bool {
    match id.strip_prefix("resource_") {
        Some(rest) => {
            !rest.is_empty() && rest.bytes().all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))
        }
        None => false,
    }
}

/// `/^#include\s+"<id>\.h"/m` — the showcase overlay's named interface.
///
/// The TypeScript escapes regex metacharacters in `id` before building this
/// pattern; `is_showcase_id` has already proved there are none, so the escape
/// is a no-op and is not reproduced.
pub fn uses_named_interface(source: &str, id: &str) -> bool {
    let bytes = source.as_bytes();
    let needle = format!("\"{id}.h\"");
    for start in 0..=bytes.len() {
        // `^` under the `m` flag: string start, or just past a line terminator.
        let anchored = start == 0 || bytes[start - 1] == b'\n' || bytes[start - 1] == b'\r';
        if !anchored || !bytes[start..].starts_with(b"#include") {
            continue;
        }
        let mut after = start + "#include".len();
        let begin = after;
        while after < bytes.len() && (bytes[after] as char).is_whitespace() {
            after += 1;
        }
        if after > begin && bytes[after..].starts_with(needle.as_bytes()) {
            return true;
        }
    }
    false
}

fn field<'a>(row: &'a serde_json::Value, name: &str, index: usize) -> Result<&'a serde_json::Value, Failure> {
    match row.get(name) {
        Some(value) => Ok(value),
        None => fail(format!("overlays[{index}] has no {name}")),
    }
}

fn string(row: &serde_json::Value, name: &str, index: usize) -> Result<String, Failure> {
    match field(row, name, index)?.as_str() {
        Some(value) => Ok(value.to_string()),
        None => fail(format!("overlays[{index}].{name} is not a string")),
    }
}

fn count(row: &serde_json::Value, name: &str, index: usize) -> Result<u64, Failure> {
    match field(row, name, index)?.as_u64() {
        Some(value) => Ok(value),
        None => fail(format!("overlays[{index}].{name} is not a non-negative integer")),
    }
}

/// Parse metrics/overlay-showcases.json into its registered overlays.
pub fn parse_manifest(text: &str) -> Result<Vec<Showcase>, Failure> {
    let parsed: serde_json::Value = match serde_json::from_str(text) {
        Ok(value) => value,
        Err(error) => return fail(format!("overlay showcase manifest is not JSON: {error}")),
    };
    let rows = match parsed.get("overlays").and_then(|value| value.as_array()) {
        Some(rows) => rows,
        None => return fail("overlay showcase manifest has no overlays array"),
    };
    let mut overlays = Vec::with_capacity(rows.len());
    for (index, row) in rows.iter().enumerate() {
        overlays.push(Showcase {
            id: string(row, "id", index)?,
            decoded_bytes: count(row, "decoded_bytes", index)?,
            exact_c_owners: count(row, "exact_c_owners", index)? as usize,
            sha256: string(row, "sha256", index)?,
            retained_assembly: string(row, "retained_assembly", index)?,
        });
    }
    Ok(overlays)
}

/// Every `<id>_c_*.c` entry of `directory`, sorted. A missing directory is an
/// error here; callers that treat absence as "no owners" check first, exactly
/// as the TypeScript does for `semantic/`.
pub fn owner_names(directory: &Path, id: &str) -> io::Result<Vec<String>> {
    let prefix = format!("{id}_c_");
    let mut names = Vec::new();
    for entry in fs::read_dir(directory)? {
        let name = entry?.file_name().to_string_lossy().into_owned();
        if name.starts_with(&prefix) && name.ends_with(".c") {
            names.push(name);
        }
    }
    names.sort();
    Ok(names)
}

/// The one line a passing showcase prints.
pub fn report(id: &str, exact_c_owners: usize, decoded_bytes: u64, sha256: &str, retained_assembly: &str) -> String {
    format!(
        "showcase=complete overlay={id} exact_c_owners={exact_c_owners} \
         decoded_bytes={decoded_bytes} sha256={sha256} retained_assembly={retained_assembly}"
    )
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn plain_c_is_canonical() {
        assert!(canonical_c_source("#include \"resource_37c.h\"\nvoid Func_02000000(void) { int x = 1; }\n"));
    }

    #[test]
    fn register_pinned_fakematch_is_rejected() {
        assert!(!canonical_c_source("register int value asm(\"r4\");\n"));
        assert!(!canonical_c_source("  register void *pointer asm (\"r7\") = 0;\n"));
    }

    #[test]
    fn register_and_asm_must_share_a_statement_and_a_line() {
        // A `;` between them ends `[^;\n]*`.
        assert!(canonical_c_source("register int value; other = wasm(3);\n"));
        // So does a newline.
        assert!(canonical_c_source("register int value\nint asm_like = 1;\n"));
        // `\s*` after `asm` may itself cross a newline, though.
        assert!(!canonical_c_source("register int value asm\n(\"r4\");\n"));
    }

    #[test]
    fn asm_must_be_a_whole_word() {
        assert!(canonical_c_source("register int value = wasm(1);\n"));
        assert!(canonical_c_source("register int value = asmx(1);\n"));
        assert!(canonical_c_source("int reregister_asm(void);\n"));
    }

    #[test]
    fn inline_assembly_spellings_are_rejected() {
        assert!(!canonical_c_source("void f(void) { __asm__(\"nop\"); }\n"));
        assert!(!canonical_c_source("void f(void) { asm volatile (\"nop\"); }\n"));
        assert!(!canonical_c_source("void f(void) { asm\tvolatile(\"nop\"); }\n"));
        // `\s+` is required: `asmvolatile` is one identifier.
        assert!(canonical_c_source("int asmvolatile = 1;\n"));
        // And `volatile` must end at a boundary.
        assert!(canonical_c_source("int x = asm volatileish;\n"));
        // `__asm` alone is not one of these four patterns.
        assert!(canonical_c_source("int __asm_helper = 1;\n"));
    }

    #[test]
    fn incbin_and_m2c_error_are_rejected() {
        assert!(!canonical_c_source("__attribute__((section(\".incbin\")));\n"));
        assert!(!canonical_c_source("    M2C_ERROR(\"unhandled\");\n"));
        assert!(canonical_c_source("// mentions incbinary and M2C_ERRORS only\n"));
    }

    #[test]
    fn showcase_ids_are_lowercase_hex_resources() {
        assert!(is_showcase_id("resource_37c"));
        assert!(is_showcase_id("resource_0"));
        assert!(!is_showcase_id("resource_"));
        assert!(!is_showcase_id("resource_37C"));
        assert!(!is_showcase_id("resource_37g"));
        assert!(!is_showcase_id("xresource_37c"));
        assert!(!is_showcase_id("resource_37c\n"));
        assert!(!is_showcase_id(""));
    }

    #[test]
    fn named_interface_must_be_the_showcase_header() {
        assert!(uses_named_interface("#include \"resource_37c.h\"\n", "resource_37c"));
        assert!(uses_named_interface("/* head */\n#include   \"resource_37c.h\"\nvoid f(void);\n", "resource_37c"));
        // Wrong overlay's header.
        assert!(!uses_named_interface("#include \"resource_37d.h\"\n", "resource_37c"));
        // Not at a line start.
        assert!(!uses_named_interface("  #include \"resource_37c.h\"\n", "resource_37c"));
        // Whitespace after `#include` is mandatory.
        assert!(!uses_named_interface("#include\"resource_37c.h\"\n", "resource_37c"));
        // A system include of the same stem is not the named interface.
        assert!(!uses_named_interface("#include <resource_37c.h>\n", "resource_37c"));
        // `\s+` under `m` may swallow the line break, as in JavaScript.
        assert!(uses_named_interface("#include\n\"resource_37c.h\"\n", "resource_37c"));
    }

    #[test]
    fn manifest_rows_parse() {
        let text = r#"{"format":1,"overlays":[
            {"id":"resource_37c","decoded_bytes":344,"exact_c_owners":6,
             "sha256":"d77f","retained_assembly":"veneers only"}]}"#;
        let overlays = parse_manifest(text).unwrap();
        assert_eq!(overlays.len(), 1);
        assert_eq!(overlays[0], Showcase {
            id: "resource_37c".into(),
            decoded_bytes: 344,
            exact_c_owners: 6,
            sha256: "d77f".into(),
            retained_assembly: "veneers only".into(),
        });
    }

    #[test]
    fn malformed_manifests_are_named_not_guessed() {
        assert!(parse_manifest("{").is_err());
        assert!(parse_manifest(r#"{"format":1}"#).is_err());
        let missing = r#"{"overlays":[{"id":"resource_37c","decoded_bytes":344,"exact_c_owners":6}]}"#;
        let error = parse_manifest(missing).unwrap_err().to_string();
        assert_eq!(error, "overlays[0] has no sha256");
        let negative = r#"{"overlays":[{"id":"a","decoded_bytes":-1,"exact_c_owners":0,
            "sha256":"x","retained_assembly":"y"}]}"#;
        assert_eq!(
            parse_manifest(negative).unwrap_err().to_string(),
            "overlays[0].decoded_bytes is not a non-negative integer"
        );
    }

    #[test]
    fn the_real_manifest_parses_and_registers_every_showcase() {
        let root = Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap();
        let text = fs::read_to_string(root.join("metrics/overlay-showcases.json")).unwrap();
        let overlays = parse_manifest(&text).unwrap();
        assert!(overlays.len() >= 11);
        for overlay in &overlays {
            assert!(is_showcase_id(&overlay.id), "{} is not a showcase id", overlay.id);
            assert_eq!(overlay.sha256.len(), 64);
            assert!(overlay.decoded_bytes > 0 && overlay.decoded_bytes % 2 == 0);
            assert!(overlay.exact_c_owners > 0);
        }
    }

    #[test]
    fn every_registered_owner_in_the_tree_is_canonical() {
        let root = Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap();
        let text = fs::read_to_string(root.join("metrics/overlay-showcases.json")).unwrap();
        let overlays = parse_manifest(&text).unwrap();
        let mut checked = 0;
        for overlay in &overlays {
            let owners = owner_names(&root.join("exact"), &overlay.id).unwrap();
            assert_eq!(owners.len(), overlay.exact_c_owners, "{} owner count", overlay.id);
            for name in owners {
                let source = fs::read_to_string(root.join("exact").join(&name)).unwrap();
                assert!(canonical_c_source(&source), "{name} is not canonical Exact C");
                assert!(uses_named_interface(&source, &overlay.id), "{name} lacks the named interface");
                checked += 1;
            }
        }
        assert!(checked > 100, "only {checked} owners were checked");
    }

    #[test]
    fn owners_are_matched_by_prefix_and_extension_and_sorted() {
        let root = Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap();
        let owners = owner_names(&root.join("exact"), "resource_37c").unwrap();
        let mut sorted = owners.clone();
        sorted.sort();
        assert_eq!(owners, sorted);
        for name in &owners {
            assert!(name.starts_with("resource_37c_c_") && name.ends_with(".c"));
        }
        // A neighbouring overlay's owners must not be picked up by prefix.
        assert!(owner_names(&root.join("exact"), "resource_37").unwrap().is_empty());
        assert!(owner_names(&root.join("does-not-exist"), "resource_37c").is_err());
    }

    #[test]
    fn the_report_line_is_the_documented_shape() {
        assert_eq!(
            report("resource_37c", 6, 344, "d77f", "veneers only"),
            "showcase=complete overlay=resource_37c exact_c_owners=6 decoded_bytes=344 \
             sha256=d77f retained_assembly=veneers only"
        );
    }
}
