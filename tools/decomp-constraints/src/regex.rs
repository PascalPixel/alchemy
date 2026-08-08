// A miniature backtracking regex engine with ECMAScript semantics.
//
// WHY this exists instead of pulling in the `regex` crate: the TypeScript
// constraint grammar leans on two features the
// `regex` crate deliberately does not have -- negative lookahead
// (`(?!volatile )`) and backreferences (`\b(\w+) = \1 \+ 1;`). A port that
// dropped them would silently change which source mutations the analyzer can
// generate. Rather than hand-unrolling twenty bespoke scanners (each its own
// chance to diverge), the patterns are transcribed character-for-character
// from the .ts file and interpreted here.
//
// The engine matches JS, not Rust, on the details that bite:
//   - `\s` is the ECMAScript WhiteSpace + LineTerminator set: it INCLUDES
//     U+FEFF and EXCLUDES U+0085, the exact reverse of Rust's White_Space.
//   - `\w` is ASCII [A-Za-z0-9_] only, and so is `\b`.
//   - `$` without the multiline flag matches only at end of input -- it does
//     NOT match before a trailing newline.
//   - alternation is ordered and leftmost-first, quantifiers are greedy with
//     backtracking, and `exec` scans forward one position at a time.
//
// PORT NOTE: offsets are BYTE offsets into a &str, where JS uses UTF-16 code
// units. Every offset produced here is fed straight back into Rust slicing of
// the same string, and every pattern in this module matches ASCII-only text,
// so the two indexings agree everywhere it matters. Character classes and `.`
// still consume whole UTF-8 characters, so a Japanese comment in a C body is
// never split mid-character.

#[derive(Debug, Clone)]
pub enum ClassItem {
    Single(char),
    Range(char, char),
    Digit(bool),
    Word(bool),
    Space(bool),
}

#[derive(Debug, Clone)]
pub enum Node {
    Char(char),
    Any,
    Class {
        negated: bool,
        items: Vec<ClassItem>,
    },
    Group {
        index: Option<usize>,
        alt: Box<Node>,
    },
    Look {
        negative: bool,
        alt: Box<Node>,
    },
    Alt(Vec<Vec<Node>>),
    Repeat {
        node: Box<Node>,
        min: u32,
        max: u32,
        greedy: bool,
    },
    WordBoundary(bool),
    Start,
    End,
    BackRef(usize),
}

pub fn is_word_byte(c: char) -> bool {
    c.is_ascii_alphanumeric() || c == '_'
}

/// ECMAScript `\s`: WhiteSpace ∪ LineTerminator. Note U+FEFF is in, U+0085 is out.
pub fn is_js_space(c: char) -> bool {
    matches!(c,
        '\u{9}' | '\u{A}' | '\u{B}' | '\u{C}' | '\u{D}' | '\u{20}' | '\u{A0}'
        | '\u{1680}' | '\u{2000}'..='\u{200A}' | '\u{2028}' | '\u{2029}'
        | '\u{202F}' | '\u{205F}' | '\u{3000}' | '\u{FEFF}')
}

fn is_line_terminator(c: char) -> bool {
    matches!(c, '\n' | '\r' | '\u{2028}' | '\u{2029}')
}

/// JS `String.prototype.trim`, which uses the same WhiteSpace set as `\s`.
pub fn js_trim(text: &str) -> &str {
    text.trim_matches(is_js_space)
}

struct Parser<'a> {
    src: Vec<char>,
    at: usize,
    groups: usize,
    _marker: std::marker::PhantomData<&'a ()>,
}

impl Parser<'_> {
    fn peek(&self) -> Option<char> {
        self.src.get(self.at).copied()
    }

    fn next(&mut self) -> Option<char> {
        let c = self.peek();
        if c.is_some() {
            self.at += 1;
        }
        c
    }

    fn parse_alt(&mut self) -> Node {
        let mut branches = vec![self.parse_seq()];
        while self.peek() == Some('|') {
            self.at += 1;
            branches.push(self.parse_seq());
        }
        Node::Alt(branches)
    }

    fn parse_seq(&mut self) -> Vec<Node> {
        let mut nodes = Vec::new();
        while let Some(c) = self.peek() {
            if c == '|' || c == ')' {
                break;
            }
            let atom = self.parse_atom();
            nodes.push(self.parse_quantifier(atom));
        }
        nodes
    }

    fn parse_quantifier(&mut self, atom: Node) -> Node {
        let (min, max) = match self.peek() {
            Some('*') => {
                self.at += 1;
                (0, u32::MAX)
            }
            Some('+') => {
                self.at += 1;
                (1, u32::MAX)
            }
            Some('?') => {
                self.at += 1;
                (0, 1)
            }
            Some('{') => {
                // Only the well-formed `{n}` / `{n,}` / `{n,m}` forms appear in
                // the ported patterns; anything else stays a literal brace.
                let save = self.at;
                self.at += 1;
                let mut low = String::new();
                while self.peek().is_some_and(|c| c.is_ascii_digit()) {
                    low.push(self.next().expect("peeked"));
                }
                if low.is_empty() {
                    self.at = save;
                    return atom;
                }
                let min: u32 = low.parse().expect("digits");
                let max = if self.peek() == Some(',') {
                    self.at += 1;
                    let mut high = String::new();
                    while self.peek().is_some_and(|c| c.is_ascii_digit()) {
                        high.push(self.next().expect("peeked"));
                    }
                    if high.is_empty() {
                        u32::MAX
                    } else {
                        high.parse().expect("digits")
                    }
                } else {
                    min
                };
                if self.peek() != Some('}') {
                    self.at = save;
                    return atom;
                }
                self.at += 1;
                (min, max)
            }
            _ => return atom,
        };
        let greedy = if self.peek() == Some('?') {
            self.at += 1;
            false
        } else {
            true
        };
        Node::Repeat {
            node: Box::new(atom),
            min,
            max,
            greedy,
        }
    }

    fn parse_atom(&mut self) -> Node {
        match self.next().expect("parse_atom called at end") {
            '.' => Node::Any,
            '^' => Node::Start,
            '$' => Node::End,
            '(' => {
                if self.peek() == Some('?') {
                    self.at += 1;
                    match self.next() {
                        Some(':') => {
                            let alt = self.parse_alt();
                            self.expect_close();
                            Node::Group {
                                index: None,
                                alt: Box::new(alt),
                            }
                        }
                        Some('=') | Some('!') => {
                            let negative = self.src[self.at - 1] == '!';
                            let alt = self.parse_alt();
                            self.expect_close();
                            Node::Look {
                                negative,
                                alt: Box::new(alt),
                            }
                        }
                        other => panic!("unsupported group prefix (?{other:?})"),
                    }
                } else {
                    self.groups += 1;
                    let index = self.groups;
                    let alt = self.parse_alt();
                    self.expect_close();
                    Node::Group {
                        index: Some(index),
                        alt: Box::new(alt),
                    }
                }
            }
            '[' => self.parse_class(),
            '\\' => self.parse_escape(),
            c => Node::Char(c),
        }
    }

    fn expect_close(&mut self) {
        assert_eq!(self.next(), Some(')'), "unbalanced group");
    }

    fn parse_escape(&mut self) -> Node {
        let c = self.next().expect("dangling backslash");
        match c {
            'd' => Node::Class {
                negated: false,
                items: vec![ClassItem::Digit(false)],
            },
            'D' => Node::Class {
                negated: false,
                items: vec![ClassItem::Digit(true)],
            },
            'w' => Node::Class {
                negated: false,
                items: vec![ClassItem::Word(false)],
            },
            'W' => Node::Class {
                negated: false,
                items: vec![ClassItem::Word(true)],
            },
            's' => Node::Class {
                negated: false,
                items: vec![ClassItem::Space(false)],
            },
            'S' => Node::Class {
                negated: false,
                items: vec![ClassItem::Space(true)],
            },
            'b' => Node::WordBoundary(true),
            'B' => Node::WordBoundary(false),
            'n' => Node::Char('\n'),
            'r' => Node::Char('\r'),
            't' => Node::Char('\t'),
            'f' => Node::Char('\u{c}'),
            'v' => Node::Char('\u{b}'),
            '0' => Node::Char('\0'),
            '1'..='9' => Node::BackRef(c.to_digit(10).expect("digit") as usize),
            other => Node::Char(other),
        }
    }

    fn parse_class(&mut self) -> Node {
        let negated = if self.peek() == Some('^') {
            self.at += 1;
            true
        } else {
            false
        };
        let mut items = Vec::new();
        loop {
            let c = self.next().expect("unterminated character class");
            if c == ']' {
                break;
            }
            let low = if c == '\\' {
                match self.next().expect("dangling backslash in class") {
                    'd' => {
                        items.push(ClassItem::Digit(false));
                        continue;
                    }
                    'D' => {
                        items.push(ClassItem::Digit(true));
                        continue;
                    }
                    'w' => {
                        items.push(ClassItem::Word(false));
                        continue;
                    }
                    'W' => {
                        items.push(ClassItem::Word(true));
                        continue;
                    }
                    's' => {
                        items.push(ClassItem::Space(false));
                        continue;
                    }
                    'S' => {
                        items.push(ClassItem::Space(true));
                        continue;
                    }
                    'n' => '\n',
                    'r' => '\r',
                    't' => '\t',
                    other => other,
                }
            } else {
                c
            };
            if self.peek() == Some('-') && self.src.get(self.at + 1).copied() != Some(']') {
                self.at += 1;
                let raw = self.next().expect("unterminated range");
                let high = if raw == '\\' {
                    self.next().expect("dangling backslash in range")
                } else {
                    raw
                };
                items.push(ClassItem::Range(low, high));
            } else {
                items.push(ClassItem::Single(low));
            }
        }
        Node::Class { negated, items }
    }
}

pub struct Regex {
    root: Node,
    group_count: usize,
    multiline: bool,
    ignore_case: bool,
}

pub struct Match {
    /// Byte offsets of each capture group; index 0 is the whole match.
    pub caps: Vec<Option<(usize, usize)>>,
}

impl Match {
    pub fn start(&self) -> usize {
        self.caps[0].expect("group 0 always participates").0
    }

    pub fn end(&self) -> usize {
        self.caps[0].expect("group 0 always participates").1
    }

    /// `match[n]` in JS: the captured text, or None when the group did not
    /// participate (JS `undefined`).
    pub fn group<'h>(&self, hay: &'h str, n: usize) -> Option<&'h str> {
        self.caps.get(n).copied().flatten().map(|(a, b)| &hay[a..b])
    }

    pub fn text<'h>(&self, hay: &'h str) -> &'h str {
        &hay[self.start()..self.end()]
    }
}

type Caps = Vec<Option<(usize, usize)>>;

impl Regex {
    /// `flags` accepts the subset used by the port: `m` (multiline) and `i`
    /// (ignore case). `g` is not a compile flag here -- use `find_iter`.
    pub fn new(pattern: &str, flags: &str) -> Regex {
        let mut parser = Parser {
            src: pattern.chars().collect(),
            at: 0,
            groups: 0,
            _marker: std::marker::PhantomData,
        };
        let root = parser.parse_alt();
        assert!(parser.at == parser.src.len(), "trailing ')' in {pattern}");
        Regex {
            root,
            group_count: parser.groups,
            multiline: flags.contains('m'),
            ignore_case: flags.contains('i'),
        }
    }

    fn char_at(hay: &str, pos: usize) -> Option<(char, usize)> {
        hay[pos..].chars().next().map(|c| (c, c.len_utf8()))
    }

    fn char_before(hay: &str, pos: usize) -> Option<char> {
        hay[..pos].chars().next_back()
    }

    fn class_matches(&self, negated: bool, items: &[ClassItem], c: char) -> bool {
        let hit = |probe: char| {
            items.iter().any(|item| match *item {
                ClassItem::Single(s) => s == probe,
                ClassItem::Range(a, b) => a <= probe && probe <= b,
                ClassItem::Digit(neg) => probe.is_ascii_digit() != neg,
                ClassItem::Word(neg) => is_word_byte(probe) != neg,
                ClassItem::Space(neg) => is_js_space(probe) != neg,
            })
        };
        let mut inside = hit(c);
        if !inside && self.ignore_case {
            inside = hit(c.to_ascii_lowercase()) || hit(c.to_ascii_uppercase());
        }
        inside != negated
    }

    fn chars_equal(&self, a: char, b: char) -> bool {
        a == b || (self.ignore_case && a.eq_ignore_ascii_case(&b))
    }

    fn seq(
        &self,
        hay: &str,
        nodes: &[Node],
        pos: usize,
        caps: &mut Caps,
        k: &mut dyn FnMut(usize, &mut Caps) -> bool,
    ) -> bool {
        match nodes.split_first() {
            None => k(pos, caps),
            Some((head, rest)) => self.node(hay, head, pos, caps, &mut |p, c| {
                self.seq(hay, rest, p, c, k)
            }),
        }
    }

    fn node(
        &self,
        hay: &str,
        node: &Node,
        pos: usize,
        caps: &mut Caps,
        k: &mut dyn FnMut(usize, &mut Caps) -> bool,
    ) -> bool {
        match node {
            Node::Char(want) => match Self::char_at(hay, pos) {
                Some((c, width)) if self.chars_equal(c, *want) => k(pos + width, caps),
                _ => false,
            },
            Node::Any => match Self::char_at(hay, pos) {
                Some((c, width)) if !is_line_terminator(c) => k(pos + width, caps),
                _ => false,
            },
            Node::Class { negated, items } => match Self::char_at(hay, pos) {
                Some((c, width)) if self.class_matches(*negated, items, c) => k(pos + width, caps),
                _ => false,
            },
            Node::Start => {
                let ok = pos == 0
                    || (self.multiline && Self::char_before(hay, pos).is_some_and(is_line_terminator));
                ok && k(pos, caps)
            }
            Node::End => {
                let ok = pos == hay.len()
                    || (self.multiline
                        && Self::char_at(hay, pos).is_some_and(|(c, _)| is_line_terminator(c)));
                ok && k(pos, caps)
            }
            Node::WordBoundary(want) => {
                let before = Self::char_before(hay, pos).is_some_and(is_word_byte);
                let after = Self::char_at(hay, pos).is_some_and(|(c, _)| is_word_byte(c));
                ((before != after) == *want) && k(pos, caps)
            }
            Node::BackRef(index) => {
                let Some(Some((a, b))) = caps.get(*index).copied() else {
                    // An unmatched group backreference matches the empty string.
                    return k(pos, caps);
                };
                let want = &hay[a..b];
                let end = pos + want.len();
                if end <= hay.len() && self.slice_equal(&hay[pos..end], want) {
                    k(end, caps)
                } else {
                    false
                }
            }
            Node::Look { negative, alt } => {
                let saved = caps.clone();
                let hit = self.node(hay, alt, pos, caps, &mut |_, _| true);
                if hit == *negative {
                    *caps = saved;
                    false
                } else {
                    if *negative {
                        *caps = saved;
                    }
                    k(pos, caps)
                }
            }
            Node::Alt(branches) => branches
                .iter()
                .any(|branch| self.seq(hay, branch, pos, caps, k)),
            Node::Group { index, alt } => {
                let start = pos;
                match index {
                    None => self.node(hay, alt, pos, caps, k),
                    Some(index) => {
                        let saved = caps[*index];
                        let ok = self.node(hay, alt, pos, caps, &mut |p, c| {
                            let previous = c[*index];
                            c[*index] = Some((start, p));
                            if k(p, c) {
                                true
                            } else {
                                c[*index] = previous;
                                false
                            }
                        });
                        if !ok {
                            caps[*index] = saved;
                        }
                        ok
                    }
                }
            }
            Node::Repeat {
                node,
                min,
                max,
                greedy,
            } => self.repeat(hay, node, *min, *max, *greedy, 0, pos, caps, k),
        }
    }

    fn slice_equal(&self, a: &str, b: &str) -> bool {
        if self.ignore_case {
            a.eq_ignore_ascii_case(b)
        } else {
            a == b
        }
    }

    #[allow(clippy::too_many_arguments)]
    fn repeat(
        &self,
        hay: &str,
        node: &Node,
        min: u32,
        max: u32,
        greedy: bool,
        done: u32,
        pos: usize,
        caps: &mut Caps,
        k: &mut dyn FnMut(usize, &mut Caps) -> bool,
    ) -> bool {
        let more = |slf: &Self, caps: &mut Caps, k: &mut dyn FnMut(usize, &mut Caps) -> bool| {
            if done >= max {
                return false;
            }
            slf.node(hay, node, pos, caps, &mut |p, c| {
                // Guard against an empty-body loop, exactly as the ECMAScript
                // RepeatMatcher does: a zero-width iteration cannot repeat.
                if p == pos && done >= min {
                    return false;
                }
                slf.repeat(hay, node, min, max, greedy, done + 1, p, c, k)
            })
        };
        // The two arms are the same two alternatives in the opposite order,
        // and the ORDER is the whole difference between greedy and lazy: `||`
        // short-circuits, so whichever is written first wins the match.
        // clippy reads them as identical blocks; they are not.
        #[allow(clippy::if_same_then_else)]
        if greedy {
            more(self, caps, k) || (done >= min && k(pos, caps))
        } else {
            (done >= min && k(pos, caps)) || more(self, caps, k)
        }
    }

    /// `regex.exec` starting the scan at byte offset `from`.
    pub fn exec_at(&self, hay: &str, from: usize) -> Option<Match> {
        let mut pos = from;
        loop {
            if pos > hay.len() {
                return None;
            }
            if hay.is_char_boundary(pos) {
                let mut caps: Caps = vec![None; self.group_count + 1];
                let mut end = None;
                let hit = self.node(hay, &self.root, pos, &mut caps, &mut |p, _| {
                    end = Some(p);
                    true
                });
                if hit {
                    caps[0] = Some((pos, end.expect("a hit records its end")));
                    return Some(Match { caps });
                }
            }
            pos += 1;
        }
    }

    pub fn exec(&self, hay: &str) -> Option<Match> {
        self.exec_at(hay, 0)
    }

    pub fn is_match(&self, hay: &str) -> bool {
        self.exec(hay).is_some()
    }

    /// `[...hay.matchAll(/…/g)]`: non-overlapping matches, advancing one
    /// character past a zero-width hit the way the `g` flag does.
    pub fn find_iter(&self, hay: &str) -> Vec<Match> {
        let mut out = Vec::new();
        let mut at = 0usize;
        while at <= hay.len() {
            let Some(found) = self.exec_at(hay, at) else {
                break;
            };
            let (start, end) = (found.start(), found.end());
            out.push(found);
            at = if end == start {
                let width = Self::char_at(hay, start).map_or(1, |(_, w)| w);
                start + width
            } else {
                end
            };
        }
        out
    }

    /// `hay.replace(/…/, replacement)` -- first match only, with `$1`..`$9`,
    /// `$&` and `$$` expansion in the replacement, exactly as JS does.
    pub fn replace_first(&self, hay: &str, replacement: &str) -> String {
        match self.exec(hay) {
            None => hay.to_string(),
            Some(found) => {
                let mut out = String::new();
                out.push_str(&hay[..found.start()]);
                out.push_str(&expand(replacement, hay, &found));
                out.push_str(&hay[found.end()..]);
                out
            }
        }
    }

    /// `hay.replace(/…/, (…) => …)` -- first match only, replacement computed
    /// from the match with NO `$` expansion (JS skips expansion for function
    /// replacers).
    pub fn replace_first_with(&self, hay: &str, build: impl Fn(&Match) -> String) -> String {
        match self.exec(hay) {
            None => hay.to_string(),
            Some(found) => {
                let mut out = String::new();
                out.push_str(&hay[..found.start()]);
                out.push_str(&build(&found));
                out.push_str(&hay[found.end()..]);
                out
            }
        }
    }
}

fn expand(replacement: &str, hay: &str, found: &Match) -> String {
    let bytes: Vec<char> = replacement.chars().collect();
    let mut out = String::new();
    let mut at = 0;
    while at < bytes.len() {
        if bytes[at] == '$' && at + 1 < bytes.len() {
            let next = bytes[at + 1];
            if next == '$' {
                out.push('$');
                at += 2;
                continue;
            }
            if next == '&' {
                out.push_str(found.text(hay));
                at += 2;
                continue;
            }
            if let Some(digit) = next.to_digit(10) {
                let index = digit as usize;
                if index >= 1 && index < found.caps.len() {
                    if let Some(text) = found.group(hay, index) {
                        out.push_str(text);
                    }
                    at += 2;
                    continue;
                }
            }
        }
        out.push(bytes[at]);
        at += 1;
    }
    out
}

/// `hay.replace("needle", replacement)`: JS replaces the FIRST literal
/// occurrence, and still expands `$` patterns in the replacement.
pub fn replace_literal_first(hay: &str, needle: &str, replacement: &str) -> String {
    match hay.find(needle) {
        None => hay.to_string(),
        Some(at) => {
            let found = Match {
                caps: vec![Some((at, at + needle.len()))],
            };
            let mut out = String::new();
            out.push_str(&hay[..at]);
            out.push_str(&expand(replacement, hay, &found));
            out.push_str(&hay[at + needle.len()..]);
            out
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn alternation_is_ordered_and_leftmost() {
        let re = Regex::new(r"\b(r(?:1[0-2]|[0-9])|sp|lr|pc|ip|fp|sl)\b", "");
        let found = re.find_iter("r10, r1, r13, sp");
        let names: Vec<&str> = found.iter().map(|m| m.text("r10, r1, r13, sp")).collect();
        // r13 has no alternative that ends on a word boundary, so it drops out
        // entirely -- the exact behaviour inferAssemblyConstraints relies on.
        assert_eq!(names, vec!["r10", "r1", "sp"]);
    }

    #[test]
    fn backreference_requires_the_same_text() {
        let re = Regex::new(r"\b(\w+) = \1 \+ 1;", "");
        assert!(re.is_match("    p = p + 1;"));
        assert!(!re.is_match("    p = q + 1;"));
        assert_eq!(re.exec("    p = p + 1;").unwrap().start(), 4);
    }

    #[test]
    fn negative_lookahead_blocks_already_volatile_casts() {
        let re = Regex::new(r"\*\((?!volatile )((?:s8|u8|s32|[A-Za-z_][A-Za-z0-9_]*)) \*\)", "");
        assert!(re.is_match("x = *(s32 *)p;"));
        assert!(!re.is_match("x = *(volatile s32 *)p;"));
    }

    #[test]
    fn dollar_without_multiline_does_not_match_before_a_newline() {
        let plain = Regex::new(r"abc$", "");
        assert!(!plain.is_match("abc\n"));
        let multi = Regex::new(r"abc$", "m");
        assert!(multi.is_match("abc\n"));
    }

    #[test]
    fn js_whitespace_includes_feff_and_excludes_next_line() {
        let re = Regex::new(r"^\s+$", "");
        assert!(re.is_match("\u{feff} \t"), "U+FEFF is JS whitespace");
        assert!(!re.is_match("\u{85}"), "U+0085 is not JS whitespace");
        assert_eq!(js_trim("\u{feff} hi \u{feff}"), "hi");
        assert_eq!(js_trim("\u{85}hi"), "\u{85}hi");
    }

    #[test]
    fn ignore_case_hex_offsets() {
        let re = Regex::new(r"#(0x[0-9a-f]+|\d+)", "i");
        let hay = "[r0, #0X1F]";
        assert_eq!(re.exec(hay).unwrap().group(hay, 1), Some("0X1F"));
        let decimal = "[sp, #12]";
        assert_eq!(re.exec(decimal).unwrap().group(decimal, 1), Some("12"));
    }

    #[test]
    fn replacement_expands_group_references() {
        let re = Regex::new(r"\b(\w+) \+= 1;", "");
        assert_eq!(re.replace_first("  p += 1;", "$1++;"), "  p++;");
        assert_eq!(replace_literal_first("aXa", "X", "$$"), "a$a");
    }

    #[test]
    fn multiline_caret_with_leading_whitespace_class() {
        // `^(\s+)` under /m: \s itself matches newlines, so the group can run
        // backwards over blank lines. Pin the behaviour rather than assume it.
        let re = Regex::new(r"^(\s+)(\w+);", "m");
        let hay = "int f() {\n\n    x;\n}";
        let found = re.exec(hay).unwrap();
        assert_eq!(found.group(hay, 1), Some("\n    "));
        assert_eq!(found.group(hay, 2), Some("x"));
        // And it does NOT reach across an intervening word: `s32 x;` has a
        // space the greedy \w+ cannot cross, so the whole pattern fails.
        assert!(re.exec("int f() {\n\n    s32 x;\n}").is_none());
    }

    #[test]
    fn non_ascii_is_never_split_mid_character() {
        let re = Regex::new(r"M2C_FIELD\(([^,()]+),", "");
        let hay = "/* 揮発 */ M2C_FIELD(日本語ptr, s32, 0x4)";
        let found = re.exec(hay).unwrap();
        assert_eq!(found.group(hay, 1), Some("日本語ptr"));
    }
}
