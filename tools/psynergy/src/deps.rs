//! Dependency map: who calls whom, what each not-yet-C function refers to,
//! and which unresolved names block the most not-yet-C bytes.
//!
//! The index is rebuilt from linked images, listings and drafts on every run;
//! nothing is stored, and nothing in the build or the count reads it.

use crate::decode::{Ins, Kind};
use crate::similar::{Function, Origin};
use std::collections::{BTreeMap, BTreeSet, HashMap};

/// A symbol of one linked image.
#[derive(Clone, Debug)]
pub struct Symbol {
    pub name: String,
    pub value: u32,
    pub size: u32,
    /// Defined with an absolute value rather than in a section.
    pub absolute: bool,
}

/// One linked image: its functions with their instructions, and its symbols.
pub struct Image {
    pub build: String,
    pub name: String,
    /// The main cartridge image, which overlays call into.
    pub main: bool,
    pub functions: Vec<(Function, Vec<Ins>)>,
    pub symbols: Vec<Symbol>,
}

/// How a name that code refers to stands.
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Class {
    Named,
    /// `Func_`, `Data_`, `Value_`, `RomBytes_`, `Unnamed_` or `Region_` followed
    /// by its address.
    Placeholder,
    /// A message, scene or resource id.
    Id,
    /// An address no symbol names.
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

pub fn is_placeholder(name: &str) -> bool {
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

pub fn is_id(name: &str) -> bool {
    ["Msg", "Scene", "Resource"].iter().any(|p| {
        name.strip_prefix(p)
            .and_then(|rest| rest.chars().next())
            .is_some_and(|c| c.is_ascii_uppercase() || c == '_' || c.is_ascii_digit())
    })
}

/// I/O registers, palette, VRAM and OAM: hardware, never a blocker.
pub fn is_hardware(value: u32) -> bool {
    (0x0400_0000..0x0800_0000).contains(&value)
}

/// A value inside EWRAM, IWRAM or the cartridge: an address of code or data.
pub fn is_memory(value: u32) -> bool {
    (0x0200_0000..0x0204_0000).contains(&value)
        || (0x0300_0000..0x0300_8000).contains(&value)
        || (0x0800_0000..0x0a00_0000).contains(&value)
}

/// A function in the index: image index and function index.
pub type Key = (usize, usize);

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub enum Target {
    Function(Key),
    /// A call to an address where no function starts.
    Unnamed(u32),
}

#[derive(Clone, Debug)]
pub struct Call {
    pub caller: Key,
    pub callee: Target,
    /// `bl`, `veneer` (through an 8-byte far-call or overlay veneer) or
    /// `pointer` (a Thumb function address in a literal pool).
    pub kind: &'static str,
    pub via: Option<String>,
}

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub struct Reference {
    pub function: Key,
    pub class: Class,
    /// The symbol (with `+offset` inside an object) or the raw address.
    pub name: String,
    pub value: u32,
}

/// Something that must be named before a not-yet-C function can be C.
#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub struct Blocker {
    /// `placeholder`, `missing-label` or `unnamed-callee`; `named-callee`
    /// lists named not-yet-C callees for information only.
    pub kind: &'static str,
    /// Blank unless the item is local to one overlay.
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

/// How `value` is named in an image, if it is an address worth reporting.
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

/// The target of an 8-byte `ldr rN, [pc, #0]; bx rN; .4byte target` veneer.
pub fn veneer_target(f: &Function, ins: &[Ins]) -> Option<u32> {
    match (f.bytes, ins) {
        (8, [a, b]) => match (&a.kind, &b.kind) {
            (Kind::LdrPool { rd, word }, Kind::Bx(rm)) if rd == rm => Some(*word),
            _ => None,
        },
        _ => None,
    }
}

/// The target of a veneer at `address` that no symbol starts, found in the
/// instructions of the function around it.
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
    /// Resolves calls and references. `ids` are identifiers the listings use
    /// for each function, by (image, address); those an image defines as
    /// absolute message, scene or resource symbols become id references.
    pub fn build(images: &'a [Image], ids: &BTreeMap<(usize, u32), BTreeSet<String>>) -> Index<'a> {
        let lookups: Vec<Lookup> = images.iter().map(lookup).collect();
        let main = images.iter().position(|i| i.main);
        // A function starting at `address` as seen from image `from`.
        let resolve = |from: usize, address: u32| -> Option<Key> {
            if let Some(&i) = lookups[from].starts.get(&address) {
                return Some((from, i));
            }
            let m = main.filter(|&m| m != from && address >= 0x0800_0000)?;
            lookups[m].starts.get(&address).map(|&i| (m, i))
        };
        // Follows veneers (at most a few hops) to the code they reach.
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
                    // A veneer's own word is its call, recorded at its callers.
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
                                    // An unlabeled veneer inside a larger section
                                    // still says where it leads.
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
                            // IWRAM the overlay does not name itself is named by the main image.
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

    /// Every blocker with the not-yet-C functions it blocks, most bytes first.
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

    /// Not-yet-C functions with nothing left to name, largest first.
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

/// What a draft's notes say is left: compiler choices or missing names.
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

/// The remaining difference a draft header records, as its first
/// `N halfword edits`, `N differing`, `N edits` or `N lines` figure.
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

/// Comment text of a C draft (its header and notes).
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

/// Identifiers each function label of an assembly listing uses in data
/// words and calls: `(label, identifiers)` in listing order.
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

#[cfg(test)]
mod tests {
    use super::*;
    use crate::decode::Ins;

    fn ins(addr: u32, kind: Kind) -> Ins {
        Ins {
            addr,
            size: if matches!(kind, Kind::Bl { .. }) {
                4
            } else {
                2
            },
            text: String::new(),
            kind,
        }
    }

    fn function(name: &str, address: u32, bytes: u32, origin: Origin) -> Function {
        Function {
            build: "t".into(),
            name: name.into(),
            address,
            bytes,
            origin,
            source: String::new(),
            tokens: Vec::new(),
        }
    }

    fn symbol(name: &str, value: u32, size: u32) -> Symbol {
        Symbol {
            name: name.into(),
            value,
            size,
            absolute: false,
        }
    }

    fn fixture() -> Vec<Image> {
        let main = Image {
            build: "t".into(),
            name: "main".into(),
            main: true,
            functions: vec![
                // A C function.
                (
                    function("Done", 0x0800_0100, 4, Origin::C),
                    vec![ins(0x0800_0100, Kind::Bx(14))],
                ),
                // A far-call veneer to Done.
                (
                    function("DoneFar", 0x0800_0108, 8, Origin::Assembly),
                    vec![
                        ins(
                            0x0800_0108,
                            Kind::LdrPool {
                                rd: 4,
                                word: 0x0800_0101,
                            },
                        ),
                        ins(0x0800_010a, Kind::Bx(4)),
                    ],
                ),
                // Not yet C, but everything it needs is named.
                (
                    function("Ready", 0x0800_0200, 16, Origin::NotYetC),
                    vec![
                        ins(
                            0x0800_0200,
                            Kind::Bl {
                                target: 0x0800_0108,
                            },
                        ),
                        ins(
                            0x0800_0204,
                            Kind::LdrPool {
                                rd: 0,
                                word: 0x0200_0010,
                            },
                        ),
                        ins(
                            0x0800_0206,
                            Kind::LdrPool {
                                rd: 1,
                                word: 0x0400_0000,
                            },
                        ),
                        ins(
                            0x0800_0208,
                            Kind::LdrPool {
                                rd: 2,
                                word: 0x0800_0501,
                            },
                        ),
                    ],
                ),
                // Not yet C: a placeholder, a raw address and an unnamed callee.
                (
                    function("Func_08000300", 0x0800_0300, 32, Origin::NotYetC),
                    vec![
                        ins(
                            0x0800_0300,
                            Kind::LdrPool {
                                rd: 0,
                                word: 0x0200_0100,
                            },
                        ),
                        ins(
                            0x0800_0302,
                            Kind::LdrPool {
                                rd: 1,
                                word: 0x0300_0000,
                            },
                        ),
                        ins(
                            0x0800_0304,
                            Kind::Bl {
                                target: 0x0800_0400,
                            },
                        ),
                        ins(
                            0x0800_0308,
                            Kind::Bl {
                                target: 0x0800_0200,
                            },
                        ),
                    ],
                ),
                // Not yet C and calls the placeholder-named function.
                (
                    function("Waiting", 0x0800_0500, 8, Origin::NotYetC),
                    vec![
                        ins(
                            0x0800_0500,
                            Kind::Bl {
                                target: 0x0800_0300,
                            },
                        ),
                        ins(
                            0x0800_0504,
                            Kind::Bl {
                                target: 0x0800_0608,
                            },
                        ),
                    ],
                ),
                // Import veneers, the second without a label of its own.
                (
                    function("Imports", 0x0800_0600, 16, Origin::Assembly),
                    vec![
                        ins(
                            0x0800_0600,
                            Kind::LdrPool {
                                rd: 4,
                                word: 0x0800_0101,
                            },
                        ),
                        ins(0x0800_0602, Kind::Bx(4)),
                        ins(
                            0x0800_0608,
                            Kind::LdrPool {
                                rd: 4,
                                word: 0x0800_0109,
                            },
                        ),
                        ins(0x0800_060a, Kind::Bx(4)),
                    ],
                ),
            ],
            symbols: vec![
                symbol("Done", 0x0800_0101, 4),
                symbol("DoneFar", 0x0800_0109, 8),
                symbol("Ready", 0x0800_0201, 16),
                symbol("Func_08000300", 0x0800_0301, 32),
                symbol("Waiting", 0x0800_0501, 8),
                symbol("gTable", 0x0200_0000, 0x20),
                symbol("Data_02000100", 0x0200_0100, 4),
                Symbol {
                    name: "MsgHello".into(),
                    value: 7,
                    size: 0,
                    absolute: true,
                },
            ],
        };
        vec![main]
    }

    #[test]
    fn names_are_classified_by_form() {
        assert!(is_placeholder("Func_08001234"));
        assert!(is_placeholder("RomBytes_02003000"));
        assert!(!is_placeholder("Func_Main"));
        assert!(is_id("MsgAbilityName"));
        assert!(is_id("SceneKolima"));
        assert!(!is_id("Message"));
        assert!(is_hardware(0x0400_0200));
        assert!(!is_memory(0x1ff));
        assert!(!is_memory(0x021b_1a9b) && !is_memory(0x0309_21c0));
        assert!(is_placeholder("Unnamed_080cb7f8"));
    }

    #[test]
    fn calls_follow_veneers_and_function_pointers() {
        let images = fixture();
        let index = Index::build(&images, &BTreeMap::new());
        let edges: Vec<(String, String, &str, Option<String>)> = index
            .calls
            .iter()
            .map(|c| {
                let callee = match &c.callee {
                    Target::Function(k) => index.function(*k).name.clone(),
                    Target::Unnamed(a) => format!("{a:08x}"),
                };
                (
                    index.function(c.caller).name.clone(),
                    callee,
                    c.kind,
                    c.via.clone(),
                )
            })
            .collect();
        assert!(edges.contains(&(
            "Ready".into(),
            "Done".into(),
            "veneer",
            Some("DoneFar".into())
        )));
        assert!(edges.contains(&("Ready".into(), "Waiting".into(), "pointer", None)));
        assert!(edges.contains(&("Func_08000300".into(), "08000400".into(), "bl", None)));
        assert!(edges.contains(&("Waiting".into(), "Func_08000300".into(), "bl", None)));
        assert!(edges.contains(&(
            "Waiting".into(),
            "08000608".into(),
            "veneer",
            Some("to Done".into())
        )));
    }

    #[test]
    fn overlays_take_iwram_names_from_the_main_image() {
        let mut images = fixture();
        images[0].symbols.push(symbol("gSlots", 0x0300_0000, 0x10));
        images.push(Image {
            build: "t".into(),
            name: "overlay".into(),
            main: false,
            functions: vec![(
                function("Func_02000000", 0x0200_0000, 8, Origin::NotYetC),
                vec![
                    ins(
                        0x0200_0000,
                        Kind::LdrPool {
                            rd: 0,
                            word: 0x0300_0004,
                        },
                    ),
                    ins(
                        0x0200_0002,
                        Kind::LdrPool {
                            rd: 1,
                            word: 0x0200_0100,
                        },
                    ),
                ],
            )],
            symbols: vec![symbol("Func_02000000", 0x0200_0001, 8)],
        });
        let index = Index::build(&images, &BTreeMap::new());
        let seen: Vec<(&str, Class)> = index
            .references
            .iter()
            .filter(|r| r.function.0 == 1)
            .map(|r| (r.name.as_str(), r.class))
            .collect();
        assert!(seen.contains(&("gSlots+0x4", Class::Named)));
        assert!(seen.contains(&("02000100", Class::Raw)));
    }

    #[test]
    fn blockers_rank_by_bytes_and_the_frontier_is_what_they_leave() {
        let images = fixture();
        let mut ids = BTreeMap::new();
        ids.insert(
            (0, 0x0800_0300),
            BTreeSet::from(["MsgHello".to_string(), "gTable".to_string()]),
        );
        let index = Index::build(&images, &ids);
        let classes: Vec<(&str, Class)> = index
            .references
            .iter()
            .map(|r| (r.name.as_str(), r.class))
            .collect();
        assert!(classes.contains(&("gTable+0x10", Class::Named)));
        assert!(classes.contains(&("Data_02000100", Class::Placeholder)));
        assert!(classes.contains(&("03000000", Class::Raw)));
        assert!(classes.contains(&("MsgHello", Class::Id)));
        assert!(!classes.iter().any(|(n, _)| n.starts_with("0400")));
        let ranked = index.ranked_blockers();
        let top: Vec<(&str, &str, u64)> = ranked
            .iter()
            .map(|(b, _, bytes)| (b.kind, b.item.as_str(), *bytes))
            .collect();
        assert_eq!(top[0], ("missing-label", "03000000", 32));
        assert!(top.contains(&("placeholder", "Func_08000300", 8)));
        assert!(top.contains(&("unnamed-callee", "08000400", 32)));
        assert_eq!(top.last().unwrap().0, "named-callee");
        let frontier: Vec<&str> = index
            .frontier()
            .iter()
            .map(|k| index.function(*k).name.as_str())
            .collect();
        assert_eq!(frontier, vec!["Ready"]);
    }

    #[test]
    fn drafts_read_their_score_and_what_is_left() {
        let notes = comments("/* Nonmatching: 26 halfword edits. The register allocation\n swaps r5 and r7. */\nint x; /* cse */");
        assert_eq!(draft_score(&notes), Some(26));
        assert_eq!(draft_status(&notes), "register/scheduling");
        assert_eq!(
            draft_score("resource_372:020031ac; 2716 / 2716 bytes, 33 differing"),
            Some(33)
        );
        assert_eq!(
            draft_status("needs a real name for the placeholder Data_02001234"),
            "names/data"
        );
        assert_eq!(draft_status("nothing"), "unknown");
    }

    #[test]
    fn listings_give_each_label_its_identifiers() {
        let listing = "\t.thumb\nFoo:\n\tldr r0, [pc, #0]\n\tbl Bar\n\t.4byte MsgHello\n.L_1:\n\t.4byte 0x08000001\nBaz:\n\tldr r1, =SceneTown+4\n";
        let found = listing_identifiers(listing);
        assert_eq!(found.len(), 2);
        assert_eq!(found[0].0, "Foo");
        assert!(found[0].1.contains("Bar") && found[0].1.contains("MsgHello"));
        assert!(!found[0].1.iter().any(|w| w.starts_with("0x")));
        assert_eq!(found[1].1, BTreeSet::from(["SceneTown".to_string()]));
    }
}
