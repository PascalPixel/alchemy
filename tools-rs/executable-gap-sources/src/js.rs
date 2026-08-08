// Hand-written equivalents of the JavaScript regular expressions, string
// helpers, and number conversions that `executable_gap_sources.ts` and the
// `armSource` helper it imports depend on.
//
// WHY: the originals are `bun` scripts whose behaviour is defined by JS regex
// and `Number()` semantics, and those differ from anything Rust gives by
// default. The differences that actually bite here are the character class
// `\s` (JS includes U+FEFF and excludes U+0085, the exact reverse of Rust's
// `char::is_whitespace`), `\b`/`\w` (ASCII-only in JS), leftmost-first ordered
// alternation with backtracking, and `.` excluding line terminators. Each
// matcher below is written against the one pattern it replaces, and that
// pattern is quoted directly above it.

/// JS `\s`: WhiteSpace u LineTerminator. U+FEFF is in and U+0085 is out, the
/// opposite of Rust's `White_Space` property.
pub fn is_js_space(character: char) -> bool {
    matches!(
        character,
        '\t' | '\n'
            | '\u{b}'
            | '\u{c}'
            | '\r'
            | ' '
            | '\u{a0}'
            | '\u{1680}'
            | '\u{2000}'..='\u{200a}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202f}'
            | '\u{205f}'
            | '\u{3000}'
            | '\u{feff}'
    )
}

/// JS LineTerminator, which is what `^`/`$` under the `m` flag split on and
/// what `.` refuses to match.
fn is_js_line_terminator(character: char) -> bool {
    matches!(character, '\n' | '\r' | '\u{2028}' | '\u{2029}')
}

/// JS `\w`: `[A-Za-z0-9_]`, ASCII only.
fn is_js_word(character: char) -> bool {
    character.is_ascii_alphanumeric() || character == '_'
}

/// `String.prototype.trim`, which strips JS `\s` (not Rust's white space).
pub fn js_trim(text: &str) -> &str {
    text.trim_matches(is_js_space)
}

/// `text.split(/\r?\n/)`.
///
/// PORT NOTE: this is NOT `str::lines` -- `lines` also drops a trailing `\r`
/// from a `\n`-only split and treats a lone `\r` as a terminator. The JS
/// pattern splits on `\n` and on `\r\n`, and leaves a lone `\r` inside a field.
pub fn split_crlf_lines(text: &str) -> Vec<&str> {
    let bytes = text.as_bytes();
    let mut parts = Vec::new();
    let mut start = 0usize;
    let mut index = 0usize;
    while index < bytes.len() {
        if bytes[index] == b'\n' {
            let end = if index > start && bytes[index - 1] == b'\r' {
                index - 1
            } else {
                index
            };
            parts.push(&text[start..end]);
            start = index + 1;
        }
        index += 1;
    }
    parts.push(&text[start..]);
    parts
}

/// `trimmed.split(/\s+/)` for a subject that has already been trimmed, which is
/// the only way the ported code uses it. (On an untrimmed subject JS would emit
/// a leading and/or trailing empty field; the caller trims first.)
pub fn split_js_whitespace(text: &str) -> Vec<&str> {
    text.split(is_js_space).filter(|part| !part.is_empty()).collect()
}

/// `/^0x[0-9a-f]{8}$/` -- lower-case hex only, exactly eight digits.
pub fn is_hex8_literal(text: &str) -> bool {
    is_hex_literal(text, 8)
}

/// `/^0x[0-9a-f]{4}$/`
pub fn is_hex4_literal(text: &str) -> bool {
    is_hex_literal(text, 4)
}

fn is_hex_literal(text: &str, digits: usize) -> bool {
    let Some(rest) = text.strip_prefix("0x") else {
        return false;
    };
    rest.len() == digits && rest.chars().all(|c| c.is_ascii_digit() || ('a'..='f').contains(&c))
}

/// `/^[a-z][a-z0-9_]*$/`
pub fn is_snake_identifier(text: &str) -> bool {
    let mut characters = text.chars();
    matches!(characters.next(), Some(first) if first.is_ascii_lowercase())
        && characters.all(|c| c.is_ascii_lowercase() || c.is_ascii_digit() || c == '_')
}

/// `/^[a-z][a-z0-9-]*$/`
pub fn is_kebab_identifier(text: &str) -> bool {
    let mut characters = text.chars();
    matches!(characters.next(), Some(first) if first.is_ascii_lowercase())
        && characters.all(|c| c.is_ascii_lowercase() || c.is_ascii_digit() || c == '-')
}

/// `/^[A-Za-z_][A-Za-z0-9_]*$/`
pub fn is_c_identifier(text: &str) -> bool {
    let mut characters = text.chars();
    matches!(characters.next(), Some(first) if first.is_ascii_alphabetic() || first == '_')
        && characters.all(is_js_word)
}

/// `/_([0-9a-f]{8})$/` -- the trailing address a section symbol carries.
pub fn trailing_symbol_address(symbol: &str) -> Option<&str> {
    if symbol.len() < 9 || !symbol.is_char_boundary(symbol.len() - 9) {
        return None;
    }
    let tail = &symbol[symbol.len() - 9..];
    if !tail.starts_with('_') {
        return None;
    }
    let digits = &tail[1..];
    digits
        .chars()
        .all(|c| c.is_ascii_digit() || ('a'..='f').contains(&c))
        .then_some(digits)
}

/// `Number.parseInt(text, 16)`: skip leading JS space, take an optional sign,
/// then the longest run of hex digits; `NaN` if there is none.
///
/// PORT NOTE: `parseInt` truncates at the first non-digit instead of failing,
/// which is why this returns the prefix value rather than erroring. Every call
/// site here has already checked the shape with a regex, but keeping the
/// permissive form means a future call site cannot silently gain strictness.
pub fn parse_int_16(text: &str) -> Option<i64> {
    let rest = text.trim_start_matches(is_js_space);
    let (negative, rest) = match rest.strip_prefix('-') {
        Some(tail) => (true, tail),
        None => (false, rest.strip_prefix('+').unwrap_or(rest)),
    };
    let rest = rest.strip_prefix("0x").or_else(|| rest.strip_prefix("0X")).unwrap_or(rest);
    let taken = rest.find(|c: char| !c.is_ascii_hexdigit()).unwrap_or(rest.len());
    if taken == 0 {
        return None;
    }
    let value = i64::from_str_radix(&rest[..taken], 16).ok()?;
    Some(if negative { -value } else { value })
}

/// `Number(text)` restricted to the `0x`-prefixed StringNumericLiteral form,
/// which is the only shape the ported code feeds it.
///
/// PORT NOTE: full `Number()` also accepts decimals, `Infinity`, `0b`/`0o`
/// prefixes, and the empty string (which is 0, not NaN). Those inputs cannot
/// reach this helper: every call site validates `/^0x[0-9a-f]{4,8}$/` first, and
/// `buildExecutableGapData`'s `Number(section.address)` reads a field this tool
/// itself wrote as `hexadecimal(...)`. `None` stands for `NaN`.
pub fn js_number_hex(text: &str) -> Option<i64> {
    let rest = js_trim(text);
    let body = rest.strip_prefix("0x").or_else(|| rest.strip_prefix("0X"))?;
    if body.is_empty() || !body.chars().all(|c| c.is_ascii_hexdigit()) {
        return None;
    }
    i64::from_str_radix(body, 16).ok()
}

// ---------------------------------------------------------------------------
// Multiline `^` support
// ---------------------------------------------------------------------------

/// Under the `m` flag, `^` matches at index 0 and immediately after any line
/// terminator. A pattern of the form `^\s*LITERAL` therefore matches at `at`
/// exactly when the maximal run of JS whitespace ending at `at` either starts
/// at index 0 or contains a line terminator: `\s*` may itself span newlines, so
/// the anchor only has to fall somewhere inside that run.
fn whitespace_run_reaches_a_line_start(text: &str, at: usize) -> bool {
    let bytes = text.as_bytes();
    let mut cursor = at;
    while cursor > 0 {
        let previous = text[..cursor].chars().next_back().unwrap();
        if !is_js_space(previous) {
            return false;
        }
        cursor -= previous.len_utf8();
        if is_js_line_terminator(previous) {
            return true;
        }
        let _ = bytes;
    }
    true
}

/// Every index at which `needle` occurs.
fn occurrences(text: &str, needle: &str) -> Vec<usize> {
    let mut hits = Vec::new();
    let mut from = 0usize;
    while let Some(offset) = text[from..].find(needle) {
        hits.push(from + offset);
        from += offset + 1;
    }
    hits
}

fn next_char(text: &str, at: usize) -> Option<char> {
    text[at..].chars().next()
}

/// `text.match(/^\s*\.4byte\b/gm)?.length ?? 0`
pub fn count_4byte_lines(text: &str) -> usize {
    occurrences(text, ".4byte")
        .into_iter()
        .filter(|&at| {
            whitespace_run_reaches_a_line_start(text, at)
                && next_char(text, at + 6).is_none_or(|c| !is_js_word(c))
        })
        .count()
}

/// `text.match(/^\s*\.4byte\s+0x[0-9a-f]{8}$/gm)?.length ?? 0`
pub fn count_4byte_literal_lines(text: &str) -> usize {
    occurrences(text, ".4byte")
        .into_iter()
        .filter(|&at| {
            if !whitespace_run_reaches_a_line_start(text, at) {
                return false;
            }
            // `\s+` is greedy and the run is followed by a non-space, so it can
            // only ever end where `0x` begins.
            let mut cursor = at + 6;
            let mut spaces = 0usize;
            while let Some(character) = next_char(text, cursor) {
                if !is_js_space(character) {
                    break;
                }
                cursor += character.len_utf8();
                spaces += 1;
            }
            if spaces == 0 {
                return false;
            }
            let rest = &text[cursor..];
            let Some(body) = rest.strip_prefix("0x") else {
                return false;
            };
            if body.len() < 8 || !body.is_char_boundary(8) {
                return false;
            }
            if !body[..8].chars().all(|c| c.is_ascii_digit() || ('a'..='f').contains(&c)) {
                return false;
            }
            // `$` under `m`: end of input, or immediately before a terminator.
            body[8..].chars().next().is_none_or(is_js_line_terminator)
        })
        .count()
}

/// `/\.incbin\b|\.(?:byte|2byte|hword|short)\b/`
pub fn has_opaque_byte_directive(text: &str) -> bool {
    for (needle, length) in [
        (".incbin", 7usize),
        (".byte", 5),
        (".2byte", 6),
        (".hword", 6),
        (".short", 6),
    ] {
        for at in occurrences(text, needle) {
            if next_char(text, at + length).is_none_or(|c| !is_js_word(c)) {
                return true;
            }
        }
    }
    false
}

/// `/(?:^|\s)(?:\/\*|\/\/|@|;)/m`
///
/// PORT NOTE: the `m` flag only changes `^`, and `(?:^|\s)` already accepts any
/// whitespace, so a line terminator before the marker is covered by the `\s`
/// branch either way. The condition reduces to: the marker is at index 0, or the
/// character before it is JS whitespace.
pub fn has_comment_marker(text: &str) -> bool {
    let bytes = text.as_bytes();
    for at in 0..bytes.len() {
        if !text.is_char_boundary(at) {
            continue;
        }
        let rest = &text[at..];
        let marker = rest.starts_with("/*")
            || rest.starts_with("//")
            || rest.starts_with('@')
            || rest.starts_with(';');
        if !marker {
            continue;
        }
        if at == 0 || text[..at].chars().next_back().is_some_and(is_js_space) {
            return true;
        }
    }
    false
}

// ---------------------------------------------------------------------------
// The branch pattern
// ---------------------------------------------------------------------------

/// The conditional suffixes, in the order the alternation lists them. Order is
/// load-bearing: JS alternation is leftmost-FIRST, not longest, so `bl` is
/// tried before `bls` and only fails over to it when the rest of the pattern
/// cannot match.
const CONDITIONS: [&str; 17] = [
    "l", "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le", "hs",
    "lo",
];

#[derive(Debug, PartialEq, Eq)]
pub struct BranchMatch {
    pub start: usize,
    pub end: usize,
    pub mnemonic: String,
    pub target: String,
}

/// `/\b(b(?:l|eq|...|lo)?)(?:\.[nw])?\s+0x([0-9a-f]+)\b/` when `sized` is true
/// (the Thumb form in `executable_gap_sources.ts`), and the same pattern
/// without the `(?:\.[nw])?` group when it is false (the ARM form in
/// `sentou_kouka_runtime.ts`).
///
/// PORT NOTE: the two files really do carry different patterns. Merging them
/// would let `b.n` reach the ARM renderer, which never sees one.
pub fn branch_match(text: &str, sized: bool) -> Option<BranchMatch> {
    let bytes = text.as_bytes();
    for start in 0..bytes.len() {
        if !text.is_char_boundary(start) || bytes[start] != b'b' {
            continue;
        }
        // `\b` before a word character: the previous character must be
        // non-word (ASCII-only), or there must be no previous character.
        if text[..start]
            .chars()
            .next_back()
            .is_some_and(is_js_word)
        {
            continue;
        }
        let mut suffixes: Vec<&str> = CONDITIONS.to_vec();
        suffixes.push("");
        for suffix in suffixes {
            let after_mnemonic = start + 1 + suffix.len();
            if after_mnemonic > text.len()
                || !text.is_char_boundary(after_mnemonic)
                || &text[start + 1..after_mnemonic] != suffix
            {
                continue;
            }
            // `(?:\.[nw])?` is greedy: present is tried before absent.
            let widths: &[usize] = if sized { &[2, 0] } else { &[0] };
            for &width in widths {
                let mut cursor = after_mnemonic;
                if width == 2 {
                    let rest = &text[cursor..];
                    if !(rest.starts_with(".n") || rest.starts_with(".w")) {
                        continue;
                    }
                    cursor += 2;
                }
                // `\s+` is greedy and cannot end on a non-space, so it always
                // consumes the whole run.
                let mut spaces = 0usize;
                while let Some(character) = next_char(text, cursor) {
                    if !is_js_space(character) {
                        break;
                    }
                    cursor += character.len_utf8();
                    spaces += 1;
                }
                if spaces == 0 {
                    continue;
                }
                let Some(body) = text[cursor..].strip_prefix("0x") else {
                    continue;
                };
                let digits = body
                    .find(|c: char| !(c.is_ascii_digit() || ('a'..='f').contains(&c)))
                    .unwrap_or(body.len());
                if digits == 0 {
                    continue;
                }
                // The trailing `\b` cannot be satisfied by giving digits back:
                // every hex digit is itself a word character, so the only way
                // out is for the character after the maximal run to be
                // non-word.
                let end = cursor + 2 + digits;
                if next_char(text, end).is_some_and(is_js_word) {
                    continue;
                }
                return Some(BranchMatch {
                    start,
                    end,
                    mnemonic: text[start..after_mnemonic].to_string(),
                    target: body[..digits].to_string(),
                });
            }
        }
    }
    None
}

/// `text.replace(BRANCH, fn)` with a NON-global regex: only the first match is
/// rewritten, and the replacement is computed from the whole match plus the two
/// captures.
pub fn replace_first_branch_with(
    text: &str,
    sized: bool,
    replace: impl FnOnce(&str, &str, &str) -> String,
) -> String {
    match branch_match(text, sized) {
        Some(found) => format!(
            "{}{}{}",
            &text[..found.start],
            replace(
                &text[found.start..found.end],
                &found.mnemonic,
                &found.target
            ),
            &text[found.end..]
        ),
        None => text.to_string(),
    }
}

/// `text.replace(new RegExp(BRANCH.source, "g"), fn)`: every match, left to
/// right, with the replacement computed from the captures.
pub fn replace_all_branches(
    text: &str,
    sized: bool,
    mut replace: impl FnMut(&str, &str, &str) -> String,
) -> String {
    let mut out = String::new();
    let mut rest = text;
    while let Some(found) = branch_match(rest, sized) {
        out.push_str(&rest[..found.start]);
        out.push_str(&replace(
            &rest[found.start..found.end],
            &found.mnemonic,
            &found.target,
        ));
        rest = &rest[found.end..];
    }
    out.push_str(rest);
    out
}

// ---------------------------------------------------------------------------
// objdump row shapes
// ---------------------------------------------------------------------------

fn take_while(text: &str, at: usize, mut allow: impl FnMut(char) -> bool) -> usize {
    let mut cursor = at;
    while let Some(character) = next_char(text, cursor) {
        if !allow(character) {
            break;
        }
        cursor += character.len_utf8();
    }
    cursor
}

fn is_lower_hex(character: char) -> bool {
    character.is_ascii_digit() || ('a'..='f').contains(&character)
}

/// `/^\s*([0-9a-f]+):\s+((?:[0-9a-f]{4}\s+)+)(.*)$/` -- the halfword listing
/// `executable_gap_sources.ts` parses. Returns (address text, halfword field,
/// trailing text).
pub fn gap_objdump_row(line: &str) -> Option<(&str, &str, &str)> {
    let mut cursor = take_while(line, 0, is_js_space);
    let address_start = cursor;
    cursor = take_while(line, cursor, is_lower_hex);
    if cursor == address_start {
        return None;
    }
    let address = &line[address_start..cursor];
    if !line[cursor..].starts_with(':') {
        return None;
    }
    cursor += 1;
    let after_colon = take_while(line, cursor, is_js_space);
    if after_colon == cursor {
        return None;
    }
    cursor = after_colon;
    let words_start = cursor;
    // `(?:[0-9a-f]{4}\s+)+` is greedy, and `(.*)$` after it always succeeds, so
    // the repetition simply runs as far as it can.
    let mut repeats = 0usize;
    loop {
        let quad_end = cursor + 4;
        if quad_end > line.len()
            || !line.is_char_boundary(quad_end)
            || !line[cursor..quad_end].chars().all(is_lower_hex)
        {
            break;
        }
        let space_end = take_while(line, quad_end, is_js_space);
        if space_end == quad_end {
            break;
        }
        cursor = space_end;
        repeats += 1;
    }
    if repeats == 0 {
        return None;
    }
    // `.` excludes line terminators; a caller that split on `/\r?\n/` can still
    // hand us a lone `\r`, and `(.*)$` would stop there.
    let tail_end = take_while(line, cursor, |c| !is_js_line_terminator(c));
    if tail_end != line.len() {
        return None;
    }
    Some((address, &line[words_start..cursor], &line[cursor..]))
}

/// `/^\s*([0-9a-f]+):\s+[0-9a-f]{8}\s+(.+)$/` together with the second pass
/// `/^\s*[0-9a-f]+:\s+([0-9a-f]{8})\s+/` that recovers the word. Returns
/// (address text, word text, trailing text).
pub fn arm_objdump_row(line: &str) -> Option<(&str, &str, &str)> {
    let mut cursor = take_while(line, 0, is_js_space);
    let address_start = cursor;
    cursor = take_while(line, cursor, is_lower_hex);
    if cursor == address_start {
        return None;
    }
    let address = &line[address_start..cursor];
    if !line[cursor..].starts_with(':') {
        return None;
    }
    cursor += 1;
    let after_colon = take_while(line, cursor, is_js_space);
    if after_colon == cursor {
        return None;
    }
    cursor = after_colon;
    let word_end = cursor + 8;
    if word_end > line.len()
        || !line.is_char_boundary(word_end)
        || !line[cursor..word_end].chars().all(is_lower_hex)
    {
        return None;
    }
    let word = &line[cursor..word_end];
    let after_word = take_while(line, word_end, is_js_space);
    if after_word == word_end {
        return None;
    }
    // `(.+)$` needs at least one non-terminator character.
    let tail_end = take_while(line, after_word, |c| !is_js_line_terminator(c));
    if tail_end == after_word || tail_end != line.len() {
        return None;
    }
    Some((address, word, &line[after_word..]))
}

/// `text.replace(/\s+[;@].*$/, "")` (the gap tool) and
/// `text.replace(/\s+@.*$/, "")` (the ARM renderer).
pub fn strip_trailing_comment(text: &str, semicolon_too: bool) -> &str {
    let bytes = text.as_bytes();
    let mut index = 0usize;
    while index < bytes.len() {
        let Some(character) = next_char(text, index) else {
            break;
        };
        if !is_js_space(character) {
            index += character.len_utf8();
            continue;
        }
        let run_start = index;
        let run_end = take_while(text, index, is_js_space);
        match next_char(text, run_end) {
            Some('@') => return &text[..run_start],
            Some(';') if semicolon_too => return &text[..run_start],
            _ => {}
        }
        index = run_end;
    }
    text
}

// ---------------------------------------------------------------------------
// ARM renderer row shapes
// ---------------------------------------------------------------------------

/// `/^ldr[a-z]*\s+(?:r\d+|sp|lr|ip),\s*\[pc,\s*#(-?\d+)\]/` -- returns the
/// signed byte displacement.
pub fn arm_pc_literal(text: &str) -> Option<i64> {
    let rest = text.strip_prefix("ldr")?;
    // `[a-z]*` is greedy and letters are never whitespace, so backtracking can
    // never uncover the `\s+` that follows.
    let letters = take_while(rest, 0, |c| c.is_ascii_lowercase());
    let mut cursor = letters;
    let spaces = take_while(rest, cursor, is_js_space);
    if spaces == cursor {
        return None;
    }
    cursor = spaces;
    let register = &rest[cursor..];
    cursor += if let Some(after) = register.strip_prefix('r') {
        let digits = take_while(after, 0, |c| c.is_ascii_digit());
        if digits == 0 {
            return None;
        }
        1 + digits
    } else if register.starts_with("sp") || register.starts_with("lr") || register.starts_with("ip")
    {
        2
    } else {
        return None;
    };
    let rest = &rest[cursor..];
    let rest = rest.strip_prefix(',')?;
    let rest = &rest[take_while(rest, 0, is_js_space)..];
    let rest = rest.strip_prefix("[pc,")?;
    let rest = &rest[take_while(rest, 0, is_js_space)..];
    let rest = rest.strip_prefix('#')?;
    let (negative, rest) = match rest.strip_prefix('-') {
        Some(tail) => (true, tail),
        None => (false, rest),
    };
    let digits = take_while(rest, 0, |c| c.is_ascii_digit());
    if digits == 0 || !rest[digits..].starts_with(']') {
        return None;
    }
    let value: i64 = rest[..digits].parse().ok()?;
    Some(if negative { -value } else { value })
}

/// `/^@ <UNDEFINED> instruction: (0x[0-9a-f]{8})$/` -- no `m` flag, so `$` is
/// the end of the whole subject.
pub fn undefined_instruction_word(text: &str) -> Option<&str> {
    let rest = text.strip_prefix("@ <UNDEFINED> instruction: ")?;
    is_hex8_literal(rest).then_some(rest)
}

/// `/^\.word\s+(0x[0-9a-f]{8})$/`
pub fn word_directive(text: &str) -> Option<&str> {
    let rest = text.strip_prefix(".word")?;
    let spaces = take_while(rest, 0, is_js_space);
    if spaces == 0 {
        return None;
    }
    let rest = &rest[spaces..];
    is_hex8_literal(rest).then_some(rest)
}

/// `/^ldr(?:h|sh|sb)[a-z]*\s+r\d+,\s*\[pc\],/`
pub fn is_reserved_pc_writeback_load(text: &str) -> bool {
    let Some(rest) = text.strip_prefix("ldr") else {
        return false;
    };
    // Ordered alternation: `h` first, then `sh`, then `sb`.
    let mut widths = Vec::new();
    if rest.starts_with('h') {
        widths.push(1);
    }
    if rest.starts_with("sh") || rest.starts_with("sb") {
        widths.push(2);
    }
    for width in widths {
        let rest = &rest[width..];
        let letters = take_while(rest, 0, |c| c.is_ascii_lowercase());
        let spaces = take_while(rest, letters, is_js_space);
        if spaces == letters {
            continue;
        }
        let rest = &rest[spaces..];
        let Some(after) = rest.strip_prefix('r') else {
            continue;
        };
        let digits = take_while(after, 0, |c| c.is_ascii_digit());
        if digits == 0 {
            continue;
        }
        let rest = &after[digits..];
        let Some(rest) = rest.strip_prefix(',') else {
            continue;
        };
        let rest = &rest[take_while(rest, 0, is_js_space)..];
        if rest.starts_with("[pc],") {
            return true;
        }
    }
    false
}

/// `/^tst[a-z]*\s+r\d+,\s+r\d+,\s+lsl r\d+$/`
pub fn is_reserved_tst_shift(text: &str) -> bool {
    let Some(rest) = text.strip_prefix("tst") else {
        return false;
    };
    let mut cursor = take_while(rest, 0, |c| c.is_ascii_lowercase());
    for index in 0..3 {
        let spaces = take_while(rest, cursor, is_js_space);
        if spaces == cursor {
            return false;
        }
        cursor = spaces;
        if index == 2 {
            let Some(after) = rest[cursor..].strip_prefix("lsl ") else {
                return false;
            };
            cursor = rest.len() - after.len();
        }
        let Some(after) = rest[cursor..].strip_prefix('r') else {
            return false;
        };
        let digits = take_while(after, 0, |c| c.is_ascii_digit());
        if digits == 0 {
            return false;
        }
        cursor = rest.len() - after.len() + digits;
        if index < 2 {
            if !rest[cursor..].starts_with(',') {
                return false;
            }
            cursor += 1;
        }
    }
    cursor == rest.len()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn js_whitespace_is_not_rust_whitespace() {
        assert!(is_js_space('\u{feff}'));
        assert!(!is_js_space('\u{85}'));
        assert!('\u{85}'.is_whitespace());
        assert!(!'\u{feff}'.is_whitespace());
        assert_eq!(js_trim("\u{feff} x \u{feff}"), "x");
        assert_eq!(js_trim("\u{85}x"), "\u{85}x");
    }

    #[test]
    fn crlf_splitting_keeps_a_lone_carriage_return() {
        assert_eq!(split_crlf_lines("a\r\nb\nc"), vec!["a", "b", "c"]);
        assert_eq!(split_crlf_lines("a\rb"), vec!["a\rb"]);
        assert_eq!(split_crlf_lines("a\n"), vec!["a", ""]);
    }

    #[test]
    fn hex_literal_shapes_are_lower_case_only() {
        assert!(is_hex8_literal("0x08000010"));
        assert!(!is_hex8_literal("0x0800001A"));
        assert!(!is_hex8_literal("0x0800001"));
        assert!(is_hex4_literal("0xabcd"));
        assert!(!is_hex4_literal("0xabcde"));
    }

    #[test]
    fn symbol_addresses_come_from_the_trailing_underscore_group() {
        assert_eq!(trailing_symbol_address("Func_08002500"), Some("08002500"));
        assert_eq!(trailing_symbol_address("Func_0800250"), None);
        assert_eq!(trailing_symbol_address("Func_0800250G"), None);
        assert!(is_c_identifier("Func_08002500"));
        assert!(!is_c_identifier("0Func"));
    }

    #[test]
    fn four_byte_counting_follows_the_multiline_anchor() {
        // Ordinary emitted assembly.
        let text = ".syntax unified\n.text\n\t.4byte 0x12345678\n\t.4byte 0xdeadbeef\n";
        assert_eq!(count_4byte_lines(text), 2);
        assert_eq!(count_4byte_literal_lines(text), 2);
        // `\s*` may span a newline, so a directive preceded only by whitespace
        // back to a line start still counts.
        assert_eq!(count_4byte_lines("\n \t.4byte 1\n"), 1);
        // A non-space before it on the same line does not.
        assert_eq!(count_4byte_lines("x .4byte 1\n"), 0);
        // The very first line counts without any preceding terminator.
        assert_eq!(count_4byte_lines("  .4byte 1"), 1);
        // `\b` rejects a longer directive name.
        assert_eq!(count_4byte_lines("\t.4bytes 1\n"), 0);
        // The literal form is stricter: it needs exactly eight lower-case
        // digits and nothing after them on the line.
        assert_eq!(count_4byte_literal_lines("\t.4byte 0x1234567\n"), 0);
        assert_eq!(count_4byte_literal_lines("\t.4byte 0x123456789\n"), 0);
        assert_eq!(count_4byte_literal_lines("\t.4byte 0x1234567A\n"), 0);
        assert_eq!(count_4byte_literal_lines("\t.4byte 0x12345678 x\n"), 0);
        assert_eq!(count_4byte_lines("\t.4byte 0x12345678 x\n"), 1);
    }

    #[test]
    fn opaque_directives_and_comments_are_detected_the_way_the_audit_does() {
        assert!(has_opaque_byte_directive(".byte 0\n"));
        assert!(has_opaque_byte_directive("\t.2byte 1\n"));
        assert!(has_opaque_byte_directive("\t.hword 1\n"));
        assert!(has_opaque_byte_directive("\t.short 1\n"));
        assert!(has_opaque_byte_directive(".incbin \"x\"\n"));
        // `\b` after the name: a longer identifier is not the directive.
        assert!(!has_opaque_byte_directive(".bytes 0\n"));
        assert!(!has_opaque_byte_directive(".incbinary\n"));
        // `.4byte` does not contain a literal `.byte`.
        assert!(!has_opaque_byte_directive("\t.4byte 0x1\n"));

        assert!(has_comment_marker("@ note\n"));
        assert!(has_comment_marker("\tmov r0, r1 ; note\n"));
        assert!(has_comment_marker("code\n// note\n"));
        assert!(has_comment_marker("code\n/* note */\n"));
        // Not preceded by whitespace or start of input.
        assert!(!has_comment_marker("a@b\n"));
        assert!(!has_comment_marker("\t.4byte 0x1\n"));
    }

    #[test]
    fn branch_alternation_is_leftmost_first_not_longest() {
        let found = branch_match("bls 0x8002500", true).unwrap();
        assert_eq!(found.mnemonic, "bls");
        assert_eq!(found.target, "8002500");
        let found = branch_match("blt 0x8002500", true).unwrap();
        assert_eq!(found.mnemonic, "blt");
        let found = branch_match("bl 0x8002500", true).unwrap();
        assert_eq!(found.mnemonic, "bl");
        let found = branch_match("b 0x8002500", true).unwrap();
        assert_eq!(found.mnemonic, "b");
        // The `.n`/`.w` width suffix is only in the Thumb pattern.
        let found = branch_match("b.n 0x8002500", true).unwrap();
        assert_eq!(found.mnemonic, "b");
        assert_eq!(found.target, "8002500");
        assert!(branch_match("b.n 0x8002500", false).is_none());
        // `\b` before the mnemonic.
        assert!(branch_match("sub 0x8002500", true).is_none());
        // `\b` after the digits.
        assert!(branch_match("b 0x8002500z", true).is_none());
        assert!(branch_match("b 0x8002500)", true).is_some());
        // Upper-case hex is not in the class, so the run stops early and the
        // trailing boundary fails on the remaining word character.
        assert!(branch_match("b 0x800250A", true).is_none());
    }

    #[test]
    fn branch_replacement_rewrites_only_the_first_match() {
        let text = "bl 0x8002500 bl 0x8002600";
        assert_eq!(
            replace_first_branch_with(text, true, |_, mnemonic, _| format!("{mnemonic} Target")),
            "bl Target bl 0x8002600"
        );
        let all = replace_all_branches(text, true, |_, mnemonic, target| {
            format!("{mnemonic} L{target}")
        });
        assert_eq!(all, "bl L8002500 bl L8002600");
    }

    #[test]
    fn objdump_rows_split_the_halfword_field() {
        let (address, words, text) =
            gap_objdump_row(" 8002000:\t2001      \tmovs\tr0, #1").unwrap();
        assert_eq!(address, "8002000");
        assert_eq!(split_js_whitespace(js_trim(words)).len(), 1);
        assert_eq!(js_trim(text), "movs\tr0, #1");
        let (_, words, _) = gap_objdump_row(" 8002004:\tf000 f800 \tbl\t0x8002400").unwrap();
        assert_eq!(split_js_whitespace(js_trim(words)).len(), 2);
        assert!(gap_objdump_row("Disassembly of section .data:").is_none());

        let (address, word, text) =
            arm_objdump_row(" 8002000:\te3a00001 \tmov\tr0, #1").unwrap();
        assert_eq!((address, word), ("8002000", "e3a00001"));
        assert_eq!(js_trim(text), "mov\tr0, #1");
        assert!(arm_objdump_row(" 8002000:\t2001 \tmovs").is_none());
    }

    #[test]
    fn trailing_comments_are_cut_at_the_whitespace_run() {
        assert_eq!(strip_trailing_comment("bl\t0x8002400 ; note", true), "bl\t0x8002400");
        assert_eq!(strip_trailing_comment("bl\t0x8002400 @ note", false), "bl\t0x8002400");
        // The semicolon form is only in the gap tool's pattern.
        assert_eq!(
            strip_trailing_comment("bl\t0x8002400 ; note", false),
            "bl\t0x8002400 ; note"
        );
        // No preceding whitespace means no match.
        assert_eq!(strip_trailing_comment("mov r0, r1@x", true), "mov r0, r1@x");
    }

    #[test]
    fn arm_row_special_forms() {
        assert_eq!(arm_pc_literal("ldr\tr0, [pc, #4]"), Some(4));
        assert_eq!(arm_pc_literal("ldrb   sp, [pc,   #-12]"), Some(-12));
        assert_eq!(arm_pc_literal("ldr r0, [pc, #4"), None);
        assert_eq!(arm_pc_literal("str r0, [pc, #4]"), None);
        assert_eq!(
            undefined_instruction_word("@ <UNDEFINED> instruction: 0xdeadbeef"),
            Some("0xdeadbeef")
        );
        assert_eq!(undefined_instruction_word("@ <UNDEFINED> instruction: 0xDEADBEEF"), None);
        assert_eq!(word_directive(".word  0x12345678"), Some("0x12345678"));
        assert_eq!(word_directive(".word 0x12345678 x"), None);
        assert!(is_reserved_pc_writeback_load("ldrh\tr0, [pc], #4"));
        assert!(is_reserved_pc_writeback_load("ldrsbne r10, [pc], #1"));
        assert!(!is_reserved_pc_writeback_load("ldr r0, [pc], #4"));
        assert!(is_reserved_tst_shift("tst\tr0, r1, lsl r2"));
        assert!(is_reserved_tst_shift("tstne r0, r1, lsl r2"));
        assert!(!is_reserved_tst_shift("tst r0, r1, lsl r2 "));
        assert!(!is_reserved_tst_shift("tst r0, r1, lsr r2"));
    }

    #[test]
    fn number_conversions_follow_the_js_grammar() {
        assert_eq!(js_number_hex("0x10"), Some(16));
        assert_eq!(js_number_hex("0x08000000"), Some(0x0800_0000));
        assert_eq!(js_number_hex("12abc"), None);
        assert_eq!(parse_int_16("8002500"), Some(0x800_2500));
        // `parseInt` truncates rather than failing.
        assert_eq!(parse_int_16("8002500z"), Some(0x800_2500));
        assert_eq!(parse_int_16("z"), None);
    }
}
