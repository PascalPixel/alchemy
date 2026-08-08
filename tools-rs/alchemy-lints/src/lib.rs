//! The three toolchain-free lints that used to live at the bottom of
//! `tools/lib/alchemy_gcc.ts`, REBUILT rather than ported.
//!
//! WHY A REBUILD. All three shipped green while looking at less than they
//! claimed, which is the same defect class `cache-key-lint` and
//! `source-citations` were written to close. A faithful port would have carried
//! the blindness across the language boundary and given it a fresh coat of
//! paint. The measured gaps, each reproducible against the tree:
//!
//!   * `callbackArityLint` resolved its scan directory with three `dirname`
//!     calls from `tools/lib/alchemy_gcc.ts`, which lands on the REPOSITORY
//!     ROOT, then read only that one directory non-recursively and kept the
//!     `.ts` files. The repository root contains zero `.ts` files, so the loop
//!     body never ran. The variable was even named `toolsDirectory`; the name
//!     was the whole belief. Seven tools had the bug it claims to police and
//!     the scan that was added to catch them scanned nothing.
//!
//!   * `overlayStemCollisionLint` matched `const [A-Z0-9_]+ = new Set\(\[`.
//!     That spelling misses every type-annotated declaration and every `Map`,
//!     so `SCHED_CALL_DEST_DESCENDING_OVERLAY_SOURCES` (alchemy_gcc.ts:1688),
//!     `SCHED_IMMEDIATE_BEFORE_POOL_OVERLAY_SOURCES` (:1880),
//!     `THUMB_LOW_REG_ORDER_SOURCES` (:595) and
//!     `THUMB_CALLEE_REG_ORDER_SOURCES` (:606) were invisible to a lint whose
//!     stated job is "every stem-keyed set". It also matched against the raw
//!     file, so a stem named inside a comment counted as a routing entry.
//!
//!   * `flagCapabilityLint` probes the staged compilers with the flags
//!     `evidencedRoutingFlags` returns, and `evidencedRoutingFlags` builds that
//!     list from a HAND-WRITTEN spread of routing containers. Six live
//!     containers are missing from it, so any flag reachable only through them
//!     is never offered to the binary the lint claims to have proven. The probe
//!     also never checks that the spawn produced output, never requires the
//!     flag list to be non-empty, and recognises exactly two stderr spellings —
//!     three separate ways for "the compiler was never asked" to read as "the
//!     compiler accepted everything".
//!
//! THE SHARED RULE. Scanning nothing is not passing. Every lint here counts the
//! checks it executed and the surface it looked at, and fails when either falls
//! under a floor. The floors are guards against a broken scanner, not facts
//! about today's tree: they sit far below the real numbers so ordinary edits
//! never trip them and a scanner that goes blind always does.

pub mod callback_arity;
pub mod flag_capability;
pub mod stem_collision;

use std::fs;
use std::io;
use std::path::{Path, PathBuf};

/// A single thing a lint objected to.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Finding {
    pub file: String,
    pub line: usize,
    pub message: String,
}

/// What one lint did, so the caller can prove it did anything at all.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Report {
    pub lint: &'static str,
    /// Named counters, in execution order. `Vec` and not a map because the
    /// print order is part of the output contract.
    pub counts: Vec<(&'static str, usize)>,
    pub findings: Vec<Finding>,
    /// Reasons the lint could not vouch for itself: an empty corpus, a counter
    /// under its floor, a structure it expected to find and did not. Distinct
    /// from `findings`, which are defects in the code being linted.
    pub blind: Vec<String>,
}

impl Report {
    pub fn new(lint: &'static str) -> Self {
        Report { lint, counts: Vec::new(), findings: Vec::new(), blind: Vec::new() }
    }

    pub fn count(&mut self, name: &'static str, value: usize) {
        self.counts.push((name, value));
    }

    /// Record a counter and demand it clear a floor in one move, so a counter
    /// can never be added without someone deciding what "too few" means.
    pub fn count_with_floor(&mut self, name: &'static str, value: usize, floor: usize) {
        self.count(name, value);
        if value < floor {
            self.blind.push(format!(
                "{}: {name}={value} is under the floor of {floor} -- the scanner is broken, \
                 not the tree",
                self.lint
            ));
        }
    }

    pub fn ok(&self) -> bool {
        self.findings.is_empty() && self.blind.is_empty()
    }
}

/// The repository root. The crate sits two levels below it.
pub fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("crate lives two levels below the repository root")
        .to_path_buf()
}

/// Blank out TypeScript comments, replacing each byte with a space and keeping
/// newlines, so byte offsets and line numbers in the result are byte offsets and
/// line numbers in the input.
///
/// String and template literals are PRESERVED, unlike `no-asm-c`'s equivalent:
/// two of these lints exist to read the string literals inside routing tables.
/// Literals are still tracked while scanning, because a `//` inside a string is
/// not a comment and a `*/` inside a string does not close one.
///
/// The regex-based original scanned raw text and had no idea about either.
pub fn blank_comments(text: &str) -> String {
    #[derive(Clone, Copy, PartialEq)]
    enum State {
        Code,
        Line,
        Block,
        Double,
        Single,
        Template,
        Regex,
    }
    let bytes = text.as_bytes();
    let mut out = String::with_capacity(text.len());
    let mut state = State::Code;
    let mut index = 0;
    // `/` opens a regex literal only where a value may begin. After an
    // identifier, a `)` or a `]` it is division. Without this a line like
    // `a / b; // note` would swallow the rest of the file as a regex.
    let mut value_position = true;
    while index < bytes.len() {
        let byte = bytes[index];
        let next = bytes.get(index + 1).copied();
        match state {
            State::Code => match (byte, next) {
                (b'/', Some(b'/')) => {
                    out.push_str("  ");
                    index += 2;
                    state = State::Line;
                    continue;
                }
                (b'/', Some(b'*')) => {
                    out.push_str("  ");
                    index += 2;
                    state = State::Block;
                    continue;
                }
                (b'/', _) if value_position => {
                    out.push('/');
                    state = State::Regex;
                }
                _ => {
                    out.push(byte as char);
                    state = match byte {
                        b'"' => State::Double,
                        b'\'' => State::Single,
                        b'`' => State::Template,
                        _ => State::Code,
                    };
                    if state == State::Code && !(byte as char).is_whitespace() {
                        value_position = !matches!(byte, b')' | b']' | b'}')
                            && !is_word_byte(byte);
                    }
                }
            },
            State::Line => {
                if byte == b'\n' {
                    out.push('\n');
                    state = State::Code;
                    value_position = true;
                } else {
                    out.push(' ');
                }
            }
            State::Block => {
                if byte == b'*' && next == Some(b'/') {
                    out.push_str("  ");
                    index += 2;
                    state = State::Code;
                    continue;
                }
                out.push(if byte == b'\n' { '\n' } else { ' ' });
            }
            State::Double | State::Single | State::Template | State::Regex => {
                out.push(byte as char);
                if byte == b'\\' && next.is_some() {
                    out.push(next.unwrap_or(b' ') as char);
                    index += 2;
                    continue;
                }
                let closes = match state {
                    State::Double => byte == b'"',
                    State::Single => byte == b'\'',
                    State::Template => byte == b'`',
                    State::Regex => byte == b'/' || byte == b'\n',
                    _ => false,
                };
                if closes {
                    state = State::Code;
                    value_position = false;
                }
            }
        }
        index += 1;
    }
    out
}

/// `blank_comments`, and then the CONTENTS of every string and template
/// literal blanked too, quotes and newlines and byte offsets preserved.
///
/// The routing lints need literals; the arity lint must not see them, or a
/// `.map(emit)` written inside an error message becomes a finding. Two
/// functions rather than a flag because the two lints want genuinely opposite
/// things from the same text.
pub fn blank_literals(text: &str) -> String {
    let blanked = blank_comments(text);
    let mut out = blanked.clone().into_bytes();
    for (start, _) in string_literals(&blanked) {
        let mark = out[start];
        let mut index = start + 1;
        while index < out.len() {
            if out[index] == b'\\' {
                out[index] = b' ';
                if index + 1 < out.len() && out[index + 1] != b'\n' {
                    out[index + 1] = b' ';
                }
                index += 2;
                continue;
            }
            if out[index] == mark {
                break;
            }
            if out[index] != b'\n' {
                out[index] = b' ';
            }
            index += 1;
        }
    }
    String::from_utf8(out).unwrap_or(blanked)
}

pub fn is_word_byte(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || byte == b'_' || byte == b'$'
}

/// 1-based line number of a byte offset. Used everywhere a finding needs to
/// name a place a human can open.
pub fn line_of(text: &str, offset: usize) -> usize {
    text.as_bytes()[..offset.min(text.len())].iter().filter(|byte| **byte == b'\n').count() + 1
}

/// Every file under `directory` whose name ends in `suffix`, RECURSIVELY,
/// relative-pathed and sorted.
///
/// Recursion is the point. `callbackArityLint` read one directory flat; even
/// had it been pointed at `tools/` it would have missed `tools/lib/` and
/// `tools/check/`, which is where the code lives.
pub fn files_with_suffix(directory: &Path, suffix: &str) -> io::Result<Vec<String>> {
    fn walk(at: &Path, prefix: &str, suffix: &str, into: &mut Vec<String>) -> io::Result<()> {
        let mut entries: Vec<PathBuf> =
            fs::read_dir(at)?.filter_map(Result::ok).map(|entry| entry.path()).collect();
        entries.sort();
        for path in entries {
            let Some(name) = path.file_name().and_then(|name| name.to_str()) else { continue };
            let relative =
                if prefix.is_empty() { name.to_string() } else { format!("{prefix}/{name}") };
            if path.is_dir() {
                if matches!(name, "target" | "node_modules" | ".git") {
                    continue;
                }
                walk(&path, &relative, suffix, into)?;
            } else if name.ends_with(suffix) {
                into.push(relative);
            }
        }
        Ok(())
    }
    let mut files = Vec::new();
    if directory.is_dir() {
        walk(directory, "", suffix, &mut files)?;
    }
    files.sort();
    Ok(files)
}

/// A `const NAME = new Set(...)` / `new Map(...)` declaration, however it is
/// spelled: with or without a type argument, upper or mixed case name.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Declaration {
    pub name: String,
    /// `Set` or `Map`.
    pub kind: String,
    pub line: usize,
    /// The argument list, comments already blanked, brackets balanced.
    pub body: String,
    /// Byte offset of the body within the (comment-blanked) source.
    pub body_offset: usize,
}

/// Find every `Set`/`Map` construction bound to a `const`, working on
/// comment-blanked text.
///
/// The original's `/const\s+([A-Z0-9_]+)\s*=\s*new Set\(\[([\s\S]*?)\]\)/g` made
/// three separate assumptions that each silently shrank the corpus: the name is
/// screaming snake case, there is no type argument, and the first `])` in the
/// body is the end of it. This makes none of them — the body is found by
/// balancing brackets while skipping string literals.
pub fn declarations(blanked: &str) -> Vec<Declaration> {
    let bytes = blanked.as_bytes();
    let mut found = Vec::new();
    let mut index = 0;
    while let Some(hit) = blanked[index..].find("new ") {
        let start = index + hit;
        index = start + 4;
        let mut at = index;
        while at < bytes.len() && bytes[at].is_ascii_whitespace() {
            at += 1;
        }
        let kind = if blanked[at..].starts_with("Set") {
            "Set"
        } else if blanked[at..].starts_with("Map") {
            "Map"
        } else {
            continue;
        };
        let mut after = at + 3;
        // An optional type argument: `new Set<string>(`, `new Map<string, X>(`.
        if bytes.get(after) == Some(&b'<') {
            let mut depth = 0usize;
            while after < bytes.len() {
                match bytes[after] {
                    b'<' => depth += 1,
                    b'>' => {
                        depth -= 1;
                        if depth == 0 {
                            after += 1;
                            break;
                        }
                    }
                    // A `>` can never be this far from its `<` in a real type
                    // argument; bail rather than run to the end of the file.
                    b'\n' | b';' => break,
                    _ => {}
                }
                after += 1;
            }
        }
        while after < bytes.len() && bytes[after].is_ascii_whitespace() {
            after += 1;
        }
        if bytes.get(after) != Some(&b'(') {
            continue;
        }
        let Some(end) = balanced_end(blanked, after) else { continue };
        let Some(name) = binding_name(blanked, start) else { continue };
        found.push(Declaration {
            name,
            kind: kind.to_string(),
            line: line_of(blanked, start),
            body: blanked[after + 1..end].to_string(),
            body_offset: after + 1,
        });
    }
    found
}

/// Walk back from `new` to the `const NAME =` that binds it, if any.
fn binding_name(text: &str, new_at: usize) -> Option<String> {
    let bytes = text.as_bytes();
    let mut at = new_at;
    while at > 0 && bytes[at - 1].is_ascii_whitespace() {
        at -= 1;
    }
    if at == 0 || bytes[at - 1] != b'=' {
        return None;
    }
    at -= 1;
    while at > 0 && bytes[at - 1].is_ascii_whitespace() {
        at -= 1;
    }
    let end = at;
    while at > 0 && is_word_byte(bytes[at - 1]) {
        at -= 1;
    }
    if at == end {
        return None;
    }
    let name = &text[at..end];
    // `const`, `let` or a type annotation may precede; only the binder matters.
    let mut before = at;
    while before > 0 && bytes[before - 1].is_ascii_whitespace() {
        before -= 1;
    }
    let keyword_end = before;
    while before > 0 && is_word_byte(bytes[before - 1]) {
        before -= 1;
    }
    match &text[before..keyword_end] {
        "const" | "let" | "var" => Some(name.to_string()),
        _ => None,
    }
}

/// Offset of the bracket matching the one at `open`, skipping string literals.
pub fn balanced_end(text: &str, open: usize) -> Option<usize> {
    let bytes = text.as_bytes();
    let mut depth = 0usize;
    let mut index = open;
    let mut quote: Option<u8> = None;
    while index < bytes.len() {
        let byte = bytes[index];
        if let Some(mark) = quote {
            if byte == b'\\' {
                index += 2;
                continue;
            }
            if byte == mark {
                quote = None;
            }
            index += 1;
            continue;
        }
        match byte {
            b'"' | b'\'' | b'`' => quote = Some(byte),
            b'(' | b'[' | b'{' => depth += 1,
            b')' | b']' | b'}' => {
                depth -= 1;
                if depth == 0 {
                    return Some(index);
                }
            }
            _ => {}
        }
        index += 1;
    }
    None
}

/// Every string literal in `text`, as (offset of the opening quote, contents).
/// Escapes are kept verbatim: routing entries are plain paths and hex stems, and
/// unescaping would only invent a way for the two to disagree.
pub fn string_literals(text: &str) -> Vec<(usize, String)> {
    let bytes = text.as_bytes();
    let mut found = Vec::new();
    let mut index = 0;
    while index < bytes.len() {
        let byte = bytes[index];
        if matches!(byte, b'"' | b'\'' | b'`') {
            let start = index;
            let mut at = index + 1;
            let mut value = String::new();
            while at < bytes.len() {
                if bytes[at] == b'\\' {
                    at += 2;
                    continue;
                }
                if bytes[at] == byte {
                    break;
                }
                value.push(bytes[at] as char);
                at += 1;
            }
            found.push((start, value));
            index = at + 1;
            continue;
        }
        index += 1;
    }
    found
}

/// A `Map`'s keys: the first string literal of each top-level `[...]` element.
///
/// Needed because a Map's VALUES are data, not routing keys, and some of them
/// are all-hex by coincidence -- `THUMB_LOW_REG_ORDER_SOURCES` maps
/// `"080f9a30"` to `"01231230"`, a register order that is eight hex-legal
/// characters. Treating values as stems would invent collisions.
pub fn map_keys(body: &str) -> Vec<(usize, String)> {
    let bytes = body.as_bytes();
    let mut keys = Vec::new();
    let mut index = 0;
    // Elements sit inside the outer array; find each inner `[` at depth 1.
    let Some(outer) = body.find('[') else { return keys };
    index = index.max(outer + 1);
    let mut depth = 1usize;
    let mut quote: Option<u8> = None;
    while index < bytes.len() && depth > 0 {
        let byte = bytes[index];
        if let Some(mark) = quote {
            if byte == b'\\' {
                index += 2;
                continue;
            }
            if byte == mark {
                quote = None;
            }
            index += 1;
            continue;
        }
        match byte {
            b'"' | b'\'' | b'`' => quote = Some(byte),
            b'[' => {
                if let Some(end) = balanced_end(body, index) {
                    if let Some((offset, value)) = string_literals(&body[index..end]).first() {
                        keys.push((index + offset, value.clone()));
                    }
                    index = end;
                }
            }
            b']' => depth -= 1,
            _ => {}
        }
        index += 1;
    }
    keys
}

/// The routing keys of a declaration: a Set's elements, a Map's keys.
pub fn routing_keys(declaration: &Declaration) -> Vec<(usize, String)> {
    if declaration.kind == "Map" {
        map_keys(&declaration.body)
            .into_iter()
            .map(|(offset, value)| (declaration.body_offset + offset, value))
            .collect()
    } else {
        string_literals(&declaration.body)
            .into_iter()
            .map(|(offset, value)| (declaration.body_offset + offset, value))
            .collect()
    }
}

/// Eight lowercase hex digits and nothing else: the shape of a routing stem.
///
/// Hand-rolled rather than `/^[0-9a-f]{8}$/` because JS `$` also accepts a
/// trailing newline, which would have let `"08004000\n"` pass as a stem.
pub fn is_stem(value: &str) -> bool {
    value.len() == 8 && value.bytes().all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn comments_are_blanked_and_strings_are_kept() {
        let text = "const A = new Set([\"08001234\"]); // \"08005678\"\n";
        let blanked = blank_comments(text);
        assert!(blanked.contains("08001234"), "a routing entry must survive");
        assert!(!blanked.contains("08005678"), "a stem named in a comment must not");
        assert_eq!(blanked.len(), text.len(), "offsets must be preserved");
        assert_eq!(blanked.matches('\n').count(), 1, "newlines must be preserved");
    }

    #[test]
    fn a_comment_marker_inside_a_string_does_not_open_a_comment() {
        let text = "const A = new Set([\"http://x\", \"08001234\"]);\n";
        assert!(blank_comments(text).contains("08001234"));
    }

    #[test]
    fn division_does_not_open_a_regex_literal() {
        // The failure mode this guards: `/` read as a regex opener swallows the
        // rest of the file, and every later declaration disappears.
        let text = "const n = a / b; const A = new Set([\"08001234\"]);\n";
        assert!(blank_comments(text).contains("08001234"));
    }

    #[test]
    fn typed_and_untyped_declarations_are_both_found() {
        // The exact spelling gap that made the TypeScript lint blind.
        let text = "const A = new Set([\"08001234\"]);\n\
                    const B = new Set<string>([\"08005678\"]);\n\
                    const C = new Map([[\"0800abcd\", \"0123\"]]);\n\
                    const D = new Map<string, string>([[\"0800ffff\", \"0123\"]]);\n";
        let found = declarations(&blank_comments(text));
        let names: Vec<&str> = found.iter().map(|one| one.name.as_str()).collect();
        assert_eq!(names, vec!["A", "B", "C", "D"]);
    }

    #[test]
    fn map_values_are_not_mistaken_for_stems() {
        let text = "const A = new Map([[\"080f9a30\", \"01231230\"]]);\n";
        let found = declarations(&blank_comments(text));
        let keys: Vec<String> =
            routing_keys(&found[0]).into_iter().map(|(_, value)| value).collect();
        assert_eq!(keys, vec!["080f9a30"], "the register order is data, not a stem");
    }

    #[test]
    fn a_nested_closing_bracket_does_not_end_the_body_early() {
        // `[\s\S]*?\]\)` in the original stops at the first `])` it sees.
        let text = "const A = new Set([\"08001111\", ...(x[0]), \"08002222\"]);\n";
        let found = declarations(&blank_comments(text));
        let keys: Vec<String> =
            routing_keys(&found[0]).into_iter().map(|(_, value)| value).collect();
        assert_eq!(keys, vec!["08001111", "08002222"]);
    }

    #[test]
    fn a_bare_construction_without_a_binding_is_not_a_declaration() {
        assert!(declarations(&blank_comments("return new Set([\"08001234\"]);\n")).is_empty());
    }

    #[test]
    fn a_stem_must_be_exactly_eight_hex_digits() {
        assert!(is_stem("0800abcd"));
        assert!(!is_stem("0800ABCD"), "uppercase is not the tree's spelling");
        assert!(!is_stem("0800abcd\n"), "JS `$` would have accepted this");
        assert!(!is_stem("0800abc"));
        assert!(!is_stem("exact/resource_3bb_c_020039fc.c"));
    }

    #[test]
    fn the_floor_helper_reports_blindness_rather_than_a_finding() {
        let mut report = Report::new("t");
        report.count_with_floor("files", 0, 1);
        assert!(report.findings.is_empty());
        assert_eq!(report.blind.len(), 1);
        assert!(!report.ok());
    }

    #[test]
    fn walking_is_recursive_and_skips_build_output() {
        let dir = std::env::temp_dir().join(format!("alchemy-lints-walk-{}", std::process::id()));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(dir.join("lib")).unwrap();
        fs::create_dir_all(dir.join("target")).unwrap();
        fs::write(dir.join("a.ts"), b"").unwrap();
        fs::write(dir.join("lib").join("b.ts"), b"").unwrap();
        fs::write(dir.join("target").join("c.ts"), b"").unwrap();
        assert_eq!(files_with_suffix(&dir, ".ts").unwrap(), vec!["a.ts", "lib/b.ts"]);
        fs::remove_dir_all(&dir).unwrap();
    }
}
