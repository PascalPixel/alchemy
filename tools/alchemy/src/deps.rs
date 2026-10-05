
use crate::decode::{Ins, Kind};
use crate::similar::{Function, Origin};
use std::collections::{BTreeMap, BTreeSet, HashMap};

#[derive(Clone, Debug)]
pub struct Symbol {
    pub name: String,
    pub value: u32,
    pub size: u32,
    pub absolute: bool,
}

pub struct Image {
    pub name: String,
    pub main: bool,
    pub functions: Vec<(Function, Vec<Ins>)>,
    pub symbols: Vec<Symbol>,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Class {
    Named,
    Placeholder,
    Id,
    Raw,
}

impl Class {
    pub fn label(self) -> &'static str {
        match self {
            Class::Named => "named",
            Class::Placeholder => "placeholder",
            Class::Id => "id",
            Class::Raw => "raw-address",
        }
    }
}

pub(crate) fn is_placeholder(name: &str) -> bool {
    [
        "Func_",
        "Data_",
        "Value_",
        "RomBytes_",
        "Unnamed_",
        "Region_",
        "sub_",
    ]
    .iter()
    .find_map(|p| name.strip_prefix(p))
    .is_some_and(|rest| rest.len() == 8 && rest.bytes().all(|b| b.is_ascii_hexdigit()))
}

pub(crate) fn is_id(name: &str) -> bool {
    ["Msg", "Scene", "Resource"].iter().any(|p| {
        name.strip_prefix(p)
            .and_then(|rest| rest.chars().next())
            .is_some_and(|c| c.is_ascii_uppercase() || c == '_' || c.is_ascii_digit())
    })
}

pub(crate) fn is_hardware(value: u32) -> bool {
    (0x0400_0000..0x0800_0000).contains(&value)
}

pub(crate) fn is_memory(value: u32) -> bool {
    (0x0200_0000..0x0204_0000).contains(&value)
        || (0x0300_0000..0x0300_8000).contains(&value)
        || (0x0800_0000..0x0a00_0000).contains(&value)
}

pub type Key = (usize, usize);

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum Target {
    Function(Key),
    Unnamed(u32),
}

#[derive(Clone, Debug)]
pub struct Call {
    pub caller: Key,
    pub callee: Target,
    pub kind: &'static str,
    pub via: Option<String>,
}

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub struct Reference {
    pub function: Key,
    pub class: Class,
    pub name: String,
    pub value: u32,
}

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Blocker {
    pub kind: &'static str,
    pub scope: String,
    pub item: String,
}

impl Blocker {
    pub fn blocks(&self) -> bool {
        self.kind != "named-callee"
    }
}

pub struct Index<'a> {
    pub images: &'a [Image],
    pub calls: Vec<Call>,
    pub references: Vec<Reference>,
    pub blockers: BTreeMap<Key, BTreeSet<Blocker>>,
}

struct Lookup<'a> {
    starts: BTreeMap<u32, usize>,
    exact: HashMap<u32, Vec<&'a Symbol>>,
    sized: BTreeMap<u32, Vec<&'a Symbol>>,
    names: HashMap<&'a str, &'a Symbol>,
}

fn lookup(image: &Image) -> Lookup<'_> {
    let mut l = Lookup {
        starts: BTreeMap::new(),
        exact: HashMap::new(),
        sized: BTreeMap::new(),
        names: HashMap::new(),
    };
    for (i, (f, _)) in image.functions.iter().enumerate() {
        l.starts.entry(f.address).or_insert(i);
    }
    for s in &image.symbols {
        l.names.entry(&s.name).or_insert(s);
        if s.absolute {
            continue;
        }
        l.exact.entry(s.value).or_default().push(s);
        if s.value & 1 != 0 {
            l.exact.entry(s.value & !1).or_default().push(s);
        }
        if s.size > 0 {
            l.sized.entry(s.value & !1).or_default().push(s);
        }
    }
    l
}

fn prefer<'a>(symbols: &[&'a Symbol]) -> (&'a str, bool) {
    let named = symbols.iter().find(|s| !is_placeholder(&s.name));
    match named {
        Some(s) => (&s.name, false),
        None => (&symbols[0].name, true),
    }
}

fn classify_word(l: &Lookup, value: u32) -> Option<(Class, String)> {
    if is_hardware(value) || !is_memory(value) {
        return None;
    }
    if let Some(symbols) = l.exact.get(&value) {
        let (name, placeholder) = prefer(symbols);
        let class = if placeholder {
            Class::Placeholder
        } else {
            Class::Named
        };
        return Some((class, name.to_string()));
    }
    if let Some((start, symbols)) = l.sized.range(..=value).next_back() {
        let inside: Vec<&Symbol> = symbols
            .iter()
            .copied()
            .filter(|s| value < start + s.size)
            .collect();
        if !inside.is_empty() {
            let (name, placeholder) = prefer(&inside);
            let class = if placeholder {
                Class::Placeholder
            } else {
                Class::Named
            };
            return Some((class, format!("{name}+{:#x}", value - start)));
        }
    }
    Some((Class::Raw, format!("{value:08x}")))
}

pub(crate) fn veneer_target(f: &Function, ins: &[Ins]) -> Option<u32> {
    match (f.bytes, ins) {
        (8, [a, b]) => match (&a.kind, &b.kind) {
            (Kind::LdrPool { rd, word }, Kind::Bx(rm)) if rd == rm => Some(*word),
            _ => None,
        },
        _ => None,
    }
}

fn unlabeled_veneer(
    images: &[Image],
    lookups: &[Lookup],
    image: usize,
    address: u32,
) -> Option<u32> {
    let (_, &fi) = lookups[image].starts.range(..=address).next_back()?;
    let (_, ins) = &images[image].functions[fi];
    let at = ins.iter().position(|i| i.addr == address)?;
    match (&ins[at].kind, &ins.get(at + 1)?.kind) {
        (Kind::LdrPool { rd, word }, Kind::Bx(rm)) if rd == rm => Some(*word),
        _ => None,
    }
}

impl<'a> Index<'a> {
    pub fn build(images: &'a [Image], ids: &BTreeMap<(usize, u32), BTreeSet<String>>) -> Index<'a> {
        let lookups: Vec<Lookup> = images.iter().map(lookup).collect();
        let main = images.iter().position(|i| i.main);
        let resolve = |from: usize, address: u32| -> Option<Key> {
            if let Some(&i) = lookups[from].starts.get(&address) {
                return Some((from, i));
            }
            let m = main.filter(|&m| m != from && address >= 0x0800_0000)?;
            lookups[m].starts.get(&address).map(|&i| (m, i))
        };
        let through = |mut key: Key| -> (Key, Option<String>) {
            let mut via = None;
            for _ in 0..4 {
                let (f, ins) = &images[key.0].functions[key.1];
                let Some(next) = veneer_target(f, ins).and_then(|t| resolve(key.0, t & !1)) else {
                    break;
                };
                via.get_or_insert_with(|| f.name.clone());
                key = next;
            }
            (key, via)
        };
        let mut calls = Vec::new();
        let mut references = Vec::new();
        for (ii, image) in images.iter().enumerate() {
            for (fi, (f, ins)) in image.functions.iter().enumerate() {
                let caller = (ii, fi);
                if veneer_target(f, ins).is_some() {
                    continue;
                }
                let mut seen = BTreeSet::new();
                for i in ins {
                    match &i.kind {
                        Kind::Bl { target } => {
                            if (f.address..f.address + f.bytes).contains(target) {
                                continue;
                            }
                            let call = match resolve(ii, *target) {
                                Some(key) => {
                                    let (key, via) = through(key);
                                    let kind = if via.is_some() { "veneer" } else { "bl" };
                                    Call {
                                        caller,
                                        callee: Target::Function(key),
                                        kind,
                                        via,
                                    }
                                }
                                None => {
                                    let reached = unlabeled_veneer(images, &lookups, ii, *target)
                                        .and_then(|t| resolve(ii, t & !1))
                                        .map(|k| through(k).0);
                                    Call {
                                        caller,
                                        callee: Target::Unnamed(*target),
                                        kind: if reached.is_some() { "veneer" } else { "bl" },
                                        via: reached.map(|k| {
                                            format!("to {}", images[k.0].functions[k.1].0.name)
                                        }),
                                    }
                                }
                            };
                            if seen.insert((call.callee.clone(), call.kind)) {
                                calls.push(call);
                            }
                        }
                        Kind::LdrPool { word, .. } => {
                            let word = *word;
                            if word & 1 != 0 {
                                if let Some(key) = resolve(ii, word & !1) {
                                    let (key, via) = through(key);
                                    if seen.insert((Target::Function(key), "pointer")) {
                                        calls.push(Call {
                                            caller,
                                            callee: Target::Function(key),
                                            kind: "pointer",
                                            via,
                                        });
                                    }
                                    continue;
                                }
                            }
                            let own = (f.address..f.address + f.bytes).contains(&word);
                            let named = classify_word(&lookups[ii], word).map(|found| {
                                match (found.0, main) {
                                    (Class::Raw, Some(m)) if m != ii && word >= 0x0300_0000 => {
                                        classify_word(&lookups[m], word).unwrap_or(found)
                                    }
                                    _ => found,
                                }
                            });
                            if let Some((class, name)) = named.filter(|_| !own) {
                                references.push(Reference {
                                    function: caller,
                                    class,
                                    name,
                                    value: word,
                                });
                            }
                        }
                        _ => {}
                    }
                }
                if let Some(used) = ids.get(&(ii, f.address)) {
                    for name in used {
                        let Some(s) = lookups[ii].names.get(name.as_str()) else {
                            continue;
                        };
                        if s.absolute && is_id(name) {
                            references.push(Reference {
                                function: caller,
                                class: Class::Id,
                                name: name.clone(),
                                value: s.value,
                            });
                        }
                    }
                }
            }
        }
        references.sort();
        references.dedup();
        let mut index = Index {
            images,
            calls,
            references,
            blockers: BTreeMap::new(),
        };
        index.blockers = index.find_blockers();
        index
    }

    pub fn function(&self, key: Key) -> &Function {
        &self.images[key.0].functions[key.1].0
    }

    fn scope(&self, key: Key, value: u32) -> String {
        let image = &self.images[key.0];
        if image.main || value >= 0x0300_0000 {
            String::new()
        } else {
            image.name.clone()
        }
    }

    fn find_blockers(&self) -> BTreeMap<Key, BTreeSet<Blocker>> {
        let mut out: BTreeMap<Key, BTreeSet<Blocker>> = BTreeMap::new();
        for (ii, image) in self.images.iter().enumerate() {
            for (fi, (f, _)) in image.functions.iter().enumerate() {
                if f.origin == Origin::NotYetC {
                    out.insert((ii, fi), BTreeSet::new());
                }
            }
        }
        for call in &self.calls {
            let Some(set) = out.get_mut(&call.caller) else {
                continue;
            };
            let blocker = match &call.callee {
                Target::Unnamed(address) => Blocker {
                    kind: "unnamed-callee",
                    scope: self.scope(call.caller, *address),
                    item: format!("{address:08x}"),
                },
                Target::Function(key) => {
                    let callee = self.function(*key);
                    if is_placeholder(&callee.name) {
                        Blocker {
                            kind: "placeholder",
                            scope: self.scope(*key, callee.address),
                            item: callee.name.clone(),
                        }
                    } else if callee.origin == Origin::NotYetC {
                        Blocker {
                            kind: "named-callee",
                            scope: self.scope(*key, callee.address),
                            item: callee.name.clone(),
                        }
                    } else {
                        continue;
                    }
                }
            };
            set.insert(blocker);
        }
        for r in &self.references {
            let Some(set) = out.get_mut(&r.function) else {
                continue;
            };
            let kind = match r.class {
                Class::Placeholder => "placeholder",
                Class::Raw => "missing-label",
                Class::Named | Class::Id => continue,
            };
            let item = r.name.split('+').next().unwrap_or(&r.name).to_string();
            set.insert(Blocker {
                kind,
                scope: self.scope(r.function, r.value),
                item,
            });
        }
        out
    }

    pub fn ranked_blockers(&self) -> Vec<(&Blocker, Vec<Key>, u64)> {
        let mut by: BTreeMap<&Blocker, Vec<Key>> = BTreeMap::new();
        for (key, set) in &self.blockers {
            for b in set {
                by.entry(b).or_default().push(*key);
            }
        }
        let mut out: Vec<(&Blocker, Vec<Key>, u64)> = by
            .into_iter()
            .map(|(b, keys)| {
                let bytes = keys
                    .iter()
                    .map(|k| u64::from(self.function(*k).bytes))
                    .sum();
                (b, keys, bytes)
            })
            .collect();
        out.sort_by(|a, b| {
            b.0.blocks()
                .cmp(&a.0.blocks())
                .then(b.2.cmp(&a.2))
                .then(b.1.len().cmp(&a.1.len()))
                .then(a.0.cmp(b.0))
        });
        out
    }

    pub fn frontier(&self) -> Vec<Key> {
        let mut out: Vec<Key> = self
            .blockers
            .iter()
            .filter(|(_, set)| set.iter().all(|b| !b.blocks()))
            .map(|(k, _)| *k)
            .collect();
        out.sort_by(|a, b| {
            let (fa, fb) = (self.function(*a), self.function(*b));
            fb.bytes.cmp(&fa.bytes).then(a.cmp(b))
        });
        out
    }
}

pub fn draft_status(notes: &str) -> &'static str {
    let lower = notes.to_ascii_lowercase();
    let count = |words: &[&str]| {
        words
            .iter()
            .map(|w| lower.matches(w).count())
            .sum::<usize>()
    };
    let register = count(&[
        "register",
        "allocat",
        "schedul",
        "spill",
        "swap",
        "preheader",
        "cse",
        "hoist",
        "reload",
        "instruction order",
        "reorder",
        "coalesc",
    ]);
    let names = count(&[
        "placeholder",
        "unnamed",
        "missing name",
        "needs a name",
        "real name",
        "hardcoded",
        "symbol",
        "label",
        "func_",
        "data_",
        "value_",
        "rombytes_",
        "extern",
    ]);
    match (register, names) {
        (0, 0) => "unknown",
        (r, n) if r >= n => "register/scheduling",
        _ => "names/data",
    }
}

pub fn draft_score(notes: &str) -> Option<u32> {
    let words: Vec<&str> = notes.split_whitespace().chain(["", ""]).collect();
    for pair in words.windows(3) {
        let number = pair[0].trim_matches(|c: char| !c.is_ascii_digit());
        if number.is_empty()
            || !pair[0]
                .chars()
                .next()
                .is_some_and(|c| c.is_ascii_digit() || c == '(')
        {
            continue;
        }
        let Ok(n) = number.parse::<u32>() else {
            continue;
        };
        let next = pair[1].trim_end_matches(|c: char| !c.is_ascii_alphabetic());
        let after = pair[2].trim_end_matches(|c: char| !c.is_ascii_alphabetic());
        let unit = matches!(next, "differing" | "edits" | "lines")
            || (matches!(next, "halfword" | "aligned" | "differing")
                && matches!(after, "edits" | "halfwords" | "bytes"));
        if unit {
            return Some(n);
        }
    }
    None
}

pub fn comments(source: &str) -> String {
    let mut out = String::new();
    let mut rest = source;
    while let Some(start) = rest.find("/*") {
        let tail = &rest[start + 2..];
        let end = tail.find("*/").unwrap_or(tail.len());
        out.push_str(&tail[..end]);
        out.push('\n');
        rest = &tail[(end + 2).min(tail.len())..];
    }
    out
}

pub fn listing_identifiers(listing: &str) -> Vec<(String, BTreeSet<String>)> {
    let mut out: Vec<(String, BTreeSet<String>)> = Vec::new();
    for line in listing.lines() {
        let line = line.split('@').next().unwrap_or("");
        if !line.starts_with(|c: char| c.is_whitespace()) {
            if let Some(label) = line.trim_end().strip_suffix(':') {
                if !label.starts_with('.') && !label.is_empty() {
                    out.push((label.to_string(), BTreeSet::new()));
                }
            }
            continue;
        }
        let text = line.trim();
        let operand = if let Some(rest) = text
            .strip_prefix(".4byte")
            .or_else(|| text.strip_prefix(".word"))
        {
            rest
        } else if let Some(rest) = text
            .strip_prefix("bl")
            .filter(|r| r.starts_with(char::is_whitespace))
        {
            rest
        } else if let Some(at) = text.find("=") {
            &text[at + 1..]
        } else {
            continue;
        };
        let Some((_, set)) = out.last_mut() else {
            continue;
        };
        for word in operand.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_')) {
            if word.starts_with(|c: char| c.is_ascii_alphabetic() || c == '_') {
                set.insert(word.to_string());
            }
        }
    }
    out
}
