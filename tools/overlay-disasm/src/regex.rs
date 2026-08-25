pub struct Regex(regex::Regex);
pub struct Match {
    captures: Vec<Option<(usize, usize)>>,
}
impl Match {
    pub fn group<'a>(&self, text: &'a str, index: usize) -> Option<&'a str> {
        self.captures.get(index).copied().flatten().map(|(start, end)| &text[start..end])
    }
}
impl Regex {
    pub fn new(pattern: &str, flags: &str) -> Self {
        const SPACE: &str = r"[\x09-\x0d\x20\u{a0}\u{1680}\u{2000}-\u{200a}\u{2028}\u{2029}\u{202f}\u{205f}\u{3000}\u{feff}]";
        const NOT_SPACE: &str = r"[^\x09-\x0d\x20\u{a0}\u{1680}\u{2000}-\u{200a}\u{2028}\u{2029}\u{202f}\u{205f}\u{3000}\u{feff}]";
        let pattern = pattern.replace(r"\s", SPACE).replace(r"\S", NOT_SPACE).replace(r"\b", r"(?-u:\b)").replace(r"\B", r"(?-u:\B)");
        let mut builder = regex::RegexBuilder::new(&pattern);
        builder.case_insensitive(flags.contains('i')).multi_line(flags.contains('m'));
        Self(builder.build().unwrap_or_else(|error| panic!("invalid regex {pattern:?}: {error}")))
    }
    pub fn exec(&self, text: &str) -> Option<Match> {
        self.0.captures(text).map(|captures| Match { captures: captures.iter().map(|item| item.map(|found| (found.start(), found.end()))).collect() })
    }
    pub fn is_match(&self, text: &str) -> bool {
        self.0.is_match(text)
    }
    pub fn replace_first(&self, text: &str, replacement: &str) -> String {
        self.0.replacen(text, 1, replacement).into_owned()
    }
}
pub fn is_js_space(character: char) -> bool {
    character.is_whitespace() || character == '\u{feff}'
}
pub fn js_trim(text: &str) -> &str {
    text.trim_matches(is_js_space)
}
