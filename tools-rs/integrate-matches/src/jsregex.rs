//! The six regular expressions in `tools/lib/integrate_matches.ts`, written by
//! hand.
//!
//! WHY BY HAND. Every pattern here is anchored, tiny, and carries at least one
//! JavaScript-specific detail that the `regex` crate spells differently:
//! `[0-9a-f]` WITHOUT an `i` flag, `.` that excludes four line terminators,
//! multiline `^` that also fires after a lone `\r`, and an ASCII-only `\b`.
//! Reaching for a regex engine would mean re-encoding all four rules in a
//! second syntax; six predicates is less surface than that.
//!
//! THE FLAG IS PART OF THE PATTERN. Two of these patterns are lowercase-only
//! `[0-9a-f]` classes with no `i` flag, which is a four-times-confirmed live
//! defect class in this repository. They are reproduced lowercase-only, on
//! purpose, and `tests/js_traps.rs` pins that uppercase input does NOT match.

/// ECMAScript `LineTerminator`. This is the set that `.` excludes, that
/// multiline `^` starts a line after, and that multiline `$` matches before.
///
/// PORT NOTE -- exactly four characters. It is NOT `char::is_whitespace`, and
/// it does NOT include U+0085 NEXT LINE, which Rust's `str::lines` reasoning
/// and several Unicode line-breaking definitions do.
pub fn is_line_terminator(c: char) -> bool {
    matches!(c, '\n' | '\r' | '\u{2028}' | '\u{2029}')
}

/// `/^08[0-9a-f]{6}$/` -- the guard on a scratch stem and on a source address.
///
/// PORT NOTE -- NO `i` flag in the TypeScript, so `08ABCDEF` is rejected there
/// and is rejected here. This is not an oversight being carried forward
/// blindly: `cleanupInstalledScratch` deletes files, and a stem that fails this
/// test throws before anything is removed. Widening it would widen a deletion
/// predicate.
///
/// PORT NOTE -- `$` with no `m` flag matches only at the very end of the
/// string, NOT before a trailing newline the way Rust's `$` in multi-line mode
/// or a naive `trim()`-then-compare would. `"08abcdef\n"` does not match.
pub fn is_source_address(stem: &str) -> bool {
    let bytes = stem.as_bytes();
    bytes.len() == 8
        && bytes[0] == b'0'
        && bytes[1] == b'8'
        && bytes[2..]
            .iter()
            .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(b))
}

/// `/^Func_[0-9a-f]{8}$/` -- the linked-symbol name filter in
/// [`crate::extent::linked_function_extent`].
///
/// PORT NOTE -- NO `i` flag. This is the exact pattern named in the standing
/// trap list: an uppercase-hex symbol silently fails to match, is filtered out
/// of the extent computation, and therefore does not contribute to the
/// `Math.max`. `arm-none-eabi-nm` emits the name the assembler was given and
/// every stem in this tree is lowercase, so the defect is unreachable today.
/// It is reproduced anyway, because "unreachable today" is what the four
/// previous instances were.
pub fn is_function_symbol(name: &str) -> bool {
    let Some(rest) = name.strip_prefix("Func_") else {
        return false;
    };
    let bytes = rest.as_bytes();
    bytes.len() == 8
        && bytes
            .iter()
            .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(b))
}

/// `/^[Tt]$/` -- the `nm` type column, text section, global or local.
pub fn is_text_type(field: &str) -> bool {
    field == "T" || field == "t"
}

/// `/^src_.*\.c$/` -- the candidate-filename filter.
///
/// PORT NOTE -- `.` excludes the four line terminators, so a filename
/// containing a newline between `src_` and `.c` does NOT match. A filename may
/// legally contain `\n` on both macOS and Linux, so this is reachable, and a
/// `(?s)`-equivalent port would accept a file the TypeScript skips.
pub fn is_candidate_name(name: &str) -> bool {
    if !name.starts_with("src_") || !name.ends_with(".c") || name.len() < 6 {
        return false;
    }
    // The span the `.*` has to cover: everything between the literal prefix and
    // the literal `.c` suffix.
    name[4..name.len() - 2]
        .chars()
        .all(|c| !is_line_terminator(c))
}

/// Every offset at which a multiline `^` matches.
///
/// PORT NOTE -- position 0, plus the position after EACH line terminator. A
/// lone `\r` starts a line in ECMAScript multiline mode; so does U+2028 and
/// U+2029. In a `\r\n` sequence both the offset after `\r` and the offset after
/// `\n` are line starts, which is why the `\r\n` case needs no special handling
/// here but does need a test.
fn line_starts(text: &str) -> Vec<usize> {
    let mut starts = vec![0usize];
    for (offset, c) in text.char_indices() {
        if is_line_terminator(c) {
            starts.push(offset + c.len_utf8());
        }
    }
    starts
}

/// ECMAScript `\b` measured at a byte offset, for ASCII word characters only.
///
/// PORT NOTE -- JavaScript's `\w` is `[A-Za-z0-9_]` and nothing else. It is NOT
/// `char::is_alphanumeric`: `static\u{e9}` has a `\b` after `static` in Rust's
/// intuition of "word" but NOT in JavaScript, where `\u{e9}` is a non-word
/// character and the boundary therefore DOES exist. Spelled out so the ASCII
/// restriction is a decision rather than an accident.
fn is_word_byte(b: u8) -> bool {
    b.is_ascii_alphanumeric() || b == b'_'
}

fn has_word_boundary_after(text: &str, offset: usize) -> bool {
    // The character before `offset` is always a word character at every call
    // site here (the last letter of `static` or `inline`), so a boundary exists
    // exactly when the character at `offset` is not one, end-of-string
    // included.
    match text.as_bytes().get(offset) {
        None => true,
        Some(b) => !is_word_byte(*b),
    }
}

/// `HELPER = /inline_fn|^(static|inline)\b/m` -- the m2c-helper rejection.
///
/// A candidate matching this is rejected with "carries an m2c helper" and never
/// reaches the toolchain. `LAWS.md` line 374 records this fingerprint, so it is
/// a documented gate, not an implementation detail.
///
/// PORT NOTE -- the alternation is unanchored on its FIRST branch and anchored
/// on its second. `inline_fn` matches anywhere on any line, including mid-word
/// and inside a comment; `static`/`inline` only at a line start. A port that
/// anchored both, or neither, would change which candidates install.
pub fn carries_helper(source: &str) -> bool {
    if source.contains("inline_fn") {
        return true;
    }
    line_starts(source).into_iter().any(|start| {
        let rest = &source[start..];
        for keyword in ["static", "inline"] {
            if rest.starts_with(keyword) && has_word_boundary_after(source, start + keyword.len()) {
                return true;
            }
        }
        false
    })
}

/// The extent of the first `/^State:.*$/m` match, as a byte range.
///
/// PORT NOTE -- `.` excludes the line terminators, so on CRLF input the match
/// ends BEFORE the `\r`, and multiline `$` is satisfied there because `\r` is a
/// line terminator. The surviving `\r` is then re-emitted after the
/// replacement. A port that consumed the `\r` would silently convert one CRLF
/// dossier line to LF; a port that refused to match before `\r` would fall
/// through to the insert branch and write a SECOND `State:` line.
fn state_line_range(text: &str) -> Option<(usize, usize)> {
    for start in line_starts(text) {
        if !text[start..].starts_with("State:") {
            continue;
        }
        let end = text[start..]
            .char_indices()
            .find(|(_, c)| is_line_terminator(*c))
            .map_or(text.len(), |(offset, _)| start + offset);
        return Some((start, end));
    }
    None
}

/// `/^State:.*$/m.test(source)`.
pub fn has_state_line(text: &str) -> bool {
    state_line_range(text).is_some()
}

/// `source.replace(/^State:.*$/m, state)`.
///
/// PORT NOTE -- `String.prototype.replace` with a non-global regular expression
/// replaces the FIRST match only. A dossier carrying two `State:` lines keeps
/// the second.
///
/// PORT NOTE -- `state` is used as a literal. The TypeScript passes a template
/// string that contains no `$`, so the `$&`/`$1`/`$$` substitution rules never
/// fire; if a `$` ever appeared in it, this call would have to grow those
/// rules. `tests/js_traps.rs` asserts the replacement text is `$`-free.
pub fn replace_state_line(source: &str, state: &str) -> String {
    match state_line_range(source) {
        Some((start, end)) => {
            let mut out = String::with_capacity(source.len() + state.len());
            out.push_str(&source[..start]);
            out.push_str(state);
            out.push_str(&source[end..]);
            out
        }
        None => source.to_string(),
    }
}

/// `source.replace(/^(#[^\n]*\n)/, `$1\n${state}\n`)`.
///
/// PORT NOTE -- NO `m` flag on this one, so `^` is the start of the STRING
/// only. A dossier that does not begin with `#` is returned unchanged and gains
/// no `State:` line at all, while `dossierClosed` is still reported as `true`.
/// That is the TypeScript's behaviour; see the bug list in the port report.
///
/// PORT NOTE -- `[^\n]*` excludes only `\n`. A `\r` is a perfectly good member
/// of the class, so on CRLF input the `\r` is part of `$1` and the inserted
/// blank line lands after it.
pub fn insert_state_after_heading(source: &str, state: &str) -> String {
    if !source.starts_with('#') {
        return source.to_string();
    }
    let Some(newline) = source.find('\n') else {
        return source.to_string();
    };
    let captured_end = newline + 1;
    let mut out = String::with_capacity(source.len() + state.len() + 2);
    out.push_str(&source[..captured_end]);
    out.push('\n');
    out.push_str(state);
    out.push('\n');
    out.push_str(&source[captured_end..]);
    out
}
