//! alchemy headers events: a scene's event functions take the name S3
//! gives them, `EV<id>_<WORD>`.
//!
//! An overlay's C file holds its scene's event tables, and a row names
//! the function it runs: `{ EVENT_TALK, 20, CONDITION_ALWAYS,
//! EVENT_SCRIPT(Function) }`. A function every row reaches under one id
//! is named by that id, in decimal, and one word: the subject it has
//! when it is an EV name already; else the kind of trigger its rows
//! spell (`EVENT_TOUCH` gives TOUCH, as does a macro that builds its
//! control from EVENT_TOUCH); else SCRIPT, the word the row itself gives
//! its function, where the kind is a bare number or the rows spell
//! several. No subject is made up: the tables give ids and kinds, and
//! what a function is about stays for a reader of its code to say.

use super::internal::Overlays;
use super::model::Model;
use super::objects::{self, Bench, Built};
use super::plan::identifiers;
use super::scan::What;
use crate::c_items::{declared, flat, Kind};
use crate::camelotize::source::{closing, mask, split_commas};
use crate::compiler::c_syntax::{lex, Tok};
use crate::compiler::preprocess::Expansion;
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

/// The word of a function whose rows spell no one kind of trigger: what
/// the row calls it, `EVENT_SCRIPT(...)`.
pub(crate) const SCRIPT: &str = "SCRIPT";

/// The type of an event table's rows, and the enum of their kinds.
const ROW: &str = "struct SceneEvent";
const KINDS: &str = "SceneEventKind";

/// A constant expression's value: numbers, enumerators, parentheses,
/// casts and integer operators.
struct Reader<'a> {
    tokens: &'a [Tok],
    at: usize,
    constants: &'a BTreeMap<String, i64>,
}

impl Reader<'_> {
    fn punct(&mut self, wanted: &[&'static str]) -> Option<&'static str> {
        match self.tokens.get(self.at) {
            Some(Tok::Punct(found)) if wanted.contains(found) => {
                self.at += 1;
                Some(found)
            }
            _ => None,
        }
    }

    /// A binary level: its operators, and the level below.
    fn level(&mut self, depth: usize) -> Option<i64> {
        const LEVELS: [&[&str]; 6] = [&["|"], &["^"], &["&"], &["<<", ">>"], &["+", "-"], &["*", "/", "%"]];
        let Some(operators) = LEVELS.get(depth) else {
            return self.unary();
        };
        let mut left = self.level(depth + 1)?;
        while let Some(operator) = self.punct(operators) {
            let right = self.level(depth + 1)?;
            left = match operator {
                "|" => left | right,
                "^" => left ^ right,
                "&" => left & right,
                "<<" => left.checked_shl(u32::try_from(right).ok()?)?,
                ">>" => left.checked_shr(u32::try_from(right).ok()?)?,
                "+" => left.checked_add(right)?,
                "-" => left.checked_sub(right)?,
                "*" => left.checked_mul(right)?,
                "/" => left.checked_div(right)?,
                _ => left.checked_rem(right)?,
            };
        }
        Some(left)
    }

    fn unary(&mut self) -> Option<i64> {
        match self.punct(&["-", "~", "+"]) {
            Some("-") => return self.unary()?.checked_neg(),
            Some("~") => return Some(!self.unary()?),
            Some(_) => return self.unary(),
            None => {}
        }
        match self.tokens.get(self.at)? {
            Tok::Number(text) => {
                self.at += 1;
                let digits = text.trim_end_matches(['u', 'U', 'l', 'L']);
                match digits.strip_prefix("0x").or_else(|| digits.strip_prefix("0X")) {
                    Some(hex) => i64::from_str_radix(hex, 16).ok(),
                    None if digits.len() > 1 && digits.starts_with('0') => i64::from_str_radix(&digits[1..], 8).ok(),
                    None => digits.parse().ok(),
                }
            }
            Tok::Ident(name) => {
                self.at += 1;
                self.constants.get(name).copied()
            }
            Tok::Punct("(") => {
                // A cast: type words and stars in parentheses before an
                // operand. An enumerator in parentheses is a value.
                let close = (self.at + 1..self.tokens.len()).find(|&i| !matches!(&self.tokens[i], Tok::Ident(_) | Tok::Punct("*")))?;
                let words = &self.tokens[self.at + 1..close];
                let cast = !words.is_empty() && self.tokens[close] == Tok::Punct(")") && !words.iter().any(|word| matches!(word, Tok::Ident(name) if self.constants.contains_key(name))) && self.tokens.get(close + 1).is_some();
                if cast {
                    self.at = close + 1;
                    return self.unary();
                }
                self.at += 1;
                let inner = self.level(0)?;
                self.punct(&[")"])?;
                Some(inner)
            }
            _ => None,
        }
    }
}

/// The value of a constant expression; none for what this does not read.
pub(crate) fn value(expression: &str, constants: &BTreeMap<String, i64>) -> Option<i64> {
    let tokens: Vec<Tok> = lex(expression).ok()?.into_iter().map(|token| token.tok).filter(|tok| !matches!(tok, Tok::Comment(_))).collect();
    let mut reader = Reader { tokens: &tokens, at: 0, constants };
    let found = reader.level(0)?;
    (reader.at == tokens.len()).then_some(found)
}

/// The enumerators a preprocessed text defines, with their values, and
/// those of the event kinds' enum.
pub(crate) fn constants(text: &str) -> (BTreeMap<String, i64>, BTreeSet<String>) {
    let mask = mask(text);
    let (mut found, mut kinds) = (BTreeMap::new(), BTreeSet::new());
    for at in crate::camelotize::source::words(&mask, "enum", 0..mask.len()) {
        let Some(open) = (at + 4..mask.len()).find(|&i| matches!(mask[i], b'{' | b';' | b'(' | b')' | b',' | b'*')).filter(|&i| mask[i] == b'{') else {
            continue;
        };
        let Some(close) = closing(&mask, open) else {
            continue;
        };
        let tag = text[at + 4..open].trim();
        let mut next = Some(0i64);
        for range in split_commas(&mask, open + 1..close) {
            let item = text[range].trim();
            if item.is_empty() {
                continue;
            }
            let (name, given) = match item.split_once('=') {
                Some((name, expression)) => (name.trim(), Some(value(expression, &found))),
                None => (item, None),
            };
            let now = given.unwrap_or(next);
            if let Some(now) = now {
                found.insert(name.to_string(), now);
            }
            if tag == KINDS {
                kinds.insert(name.to_string());
            }
            next = now.and_then(|now| now.checked_add(1));
        }
    }
    (found, kinds)
}

/// One row of an event table that names something by an identifier.
#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub(crate) struct Row {
    pub table: String,
    /// The kind its control spells, without `EVENT_`; none for a bare
    /// number or several kinds.
    pub kind: Option<String>,
    pub id: Option<i64>,
    /// The identifiers its value names.
    pub names: Vec<String>,
}

/// The rows of every event table a preprocessed text defines, in order:
/// each row whose value names a wanted identifier. And the functions the
/// text defines: what this edition compiles.
pub(crate) fn rows(text: &str, wanted: &dyn Fn(&str) -> bool) -> (Vec<Row>, BTreeSet<String>) {
    let (constants, kinds) = constants(text);
    let (mut found, mut defined) = (Vec::new(), BTreeSet::new());
    for piece in flat(text) {
        if let Kind::Function { .. } = piece.item.kind {
            defined.extend(declared(&piece.item).into_iter().next().map(|d| d.name));
        }
        if piece.item.kind != Kind::Declaration || !piece.item.text.contains(ROW) || !piece.item.text.contains('=') {
            continue;
        }
        let Some(table) = declared(&piece.item).into_iter().find(|d| d.initialized && !d.function && d.signature.contains(ROW) && d.signature.contains('[')) else {
            continue;
        };
        let item = piece.item.text;
        let mask = mask(item);
        let Some(open) = item.find('=').and_then(|at| (at..mask.len()).find(|&i| mask[i] == b'{')) else {
            continue;
        };
        let Some(close) = closing(&mask, open) else {
            continue;
        };
        for range in split_commas(&mask, open + 1..close) {
            let Some(start) = (range.start..range.end).find(|&i| !mask[i].is_ascii_whitespace()).filter(|&i| mask[i] == b'{') else {
                continue;
            };
            let Some(end) = closing(&mask, start) else {
                continue;
            };
            let fields: Vec<&str> = split_commas(&mask, start + 1..end).into_iter().map(|field| item[field].trim()).collect();
            let [control, id, _, script] = fields[..] else {
                continue;
            };
            let spelled: BTreeSet<&str> = identifiers(control).into_iter().filter(|word| kinds.contains(*word)).collect();
            let kind = match spelled.into_iter().collect::<Vec<_>>()[..] {
                [one] => one.strip_prefix("EVENT_").map(str::to_string),
                _ => None,
            };
            let names: Vec<String> = lex(script).unwrap_or_default().into_iter().filter_map(|token| match token.tok {
                Tok::Ident(name) if wanted(&name) => Some(name),
                _ => None,
            }).collect();
            if !names.is_empty() {
                found.push(Row { table: table.name.clone(), kind, id: value(id, &constants), names });
            }
        }
    }
    (found, defined)
}

/// What the pass says of one function its unit's tables reach.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Outcome {
    /// Its new name.
    Renamed(String),
    /// It has the name the tables give it.
    Named,
    /// It keeps its name, and why.
    Kept(String),
}

/// An EV name's id and subject: `EV08_KAGI` gives (8, KAGI).
pub(crate) fn event_name(name: &str) -> Option<(i64, &str)> {
    let rest = name.strip_prefix("EV")?;
    let digits = rest.find(|c: char| !c.is_ascii_digit()).filter(|&at| at > 0)?;
    let subject = rest[digits..].strip_prefix('_').filter(|subject| !subject.is_empty())?;
    Some((rest[..digits].parse().ok()?, subject))
}

/// The ordinal a table's name carries: the digits that end it after
/// `Events` or `Table`. Digits after any other word count something else
/// (a flag, an entrance, an address).
pub(crate) fn ordinal(table: &str) -> Option<&str> {
    let digits = table.len() - table.bytes().rev().take_while(u8::is_ascii_digit).count();
    let counted = ["Events", "Table"].iter().any(|word| table[..digits].ends_with(word));
    (digits < table.len() && counted).then(|| &table[digits..])
}

/// The name each function takes. `functions` are the unit's own static
/// functions in the file's order; `fixed` gives the cause a function the
/// rows name cannot be renamed for, if any; `taken` says whether a word
/// is a name the unit reads already.
pub(crate) fn names(rows: &[Row], functions: &[String], fixed: &dyn Fn(&str) -> Option<String>, taken: &dyn Fn(&str) -> bool) -> Vec<(String, Outcome)> {
    let mut tables: Vec<&str> = Vec::new();
    for row in rows {
        if !tables.contains(&row.table.as_str()) {
            tables.push(&row.table);
        }
    }
    // Each function the rows name, in the file's order, then the others.
    let mut reached: Vec<&str> = functions.iter().map(String::as_str).filter(|name| rows.iter().any(|row| row.names.iter().any(|named| named == name))).collect();
    for row in rows {
        for name in &row.names {
            if fixed(name).is_some() && !reached.contains(&name.as_str()) {
                reached.push(name);
            }
        }
    }
    let mut out: Vec<(String, Outcome)> = Vec::new();
    // The functions that take a name, by the name the tables give alone.
    let mut wanted: BTreeMap<String, Vec<(&str, Option<&str>)>> = BTreeMap::new();
    for name in &reached {
        if let Some(why) = fixed(name) {
            out.push((name.to_string(), Outcome::Kept(why)));
            continue;
        }
        let own: Vec<&Row> = rows.iter().filter(|row| row.names.iter().any(|named| named == name)).collect();
        let ids: BTreeSet<Option<i64>> = own.iter().map(|row| row.id).collect();
        let id = match ids.iter().collect::<Vec<_>>()[..] {
            [Some(id)] if *id >= 0 => *id,
            [_] => {
                out.push((name.to_string(), Outcome::Kept("its rows' id is not a number the pass reads".into())));
                continue;
            }
            _ => {
                let all: Vec<String> = ids.iter().map(|id| id.map_or("?".to_string(), |id| id.to_string())).collect();
                out.push((name.to_string(), Outcome::Kept(format!("it is reached under several ids: {}", all.join(", ")))));
                continue;
            }
        };
        let kinds: BTreeSet<Option<&str>> = own.iter().map(|row| row.kind.as_deref()).collect();
        let word = match (event_name(name), kinds.iter().collect::<Vec<_>>().as_slice()) {
            (Some((_, subject)), _) => subject,
            (None, [Some(kind)]) => kind,
            _ => SCRIPT,
        };
        let first = tables.iter().find(|table| own.iter().any(|row| row.table == **table)).copied();
        wanted.entry(format!("EV{id:02}_{word}")).or_default().push((name, first.and_then(ordinal)));
    }
    let mut given: Vec<(&str, String)> = Vec::new();
    for (base, takers) in &wanted {
        for (name, ordinal) in takers {
            // Two functions of one id and kind are told apart by the
            // ordinal their tables' names carry, and by nothing else.
            let alone = takers.len() == 1;
            let apart = takers.iter().filter(|(_, other)| other == ordinal).count() == 1;
            if !alone && !apart {
                let others: Vec<&str> = takers.iter().filter(|(other, _)| other != name).map(|(other, _)| *other).collect();
                out.push((name.to_string(), Outcome::Kept(format!("another function has its id and kind, and their tables' names do not tell them apart: {base} with {}", others.join(", ")))));
                continue;
            }
            let new = match ordinal.filter(|_| !alone) {
                Some(ordinal) => format!("{base}_{ordinal}"),
                None => base.clone(),
            };
            given.push((name, new));
        }
    }
    for (name, new) in &given {
        let twice = given.iter().filter(|(_, other)| other == new).count() > 1;
        out.push((name.to_string(), if new == name {
            Outcome::Named
        } else if twice || taken(new) {
            Outcome::Kept(format!("the unit has the name already: {new}"))
        } else {
            Outcome::Renamed(new.clone())
        }));
    }
    // In the file's order, as the functions were listed.
    out.sort_by_key(|(name, _)| reached.iter().position(|found| found == name));
    out
}

/// A text with each name written as its new one: in its code and its
/// comments, not in its string and character literals.
pub(crate) fn renamed(text: &str, names: &BTreeMap<String, String>) -> String {
    let bytes = text.as_bytes();
    let mut out = String::with_capacity(text.len() + 64);
    let (mut i, mut copied) = (0, 0);
    // Inside a comment a quote is a letter like any other.
    let mut comment: Option<&[u8]> = None;
    while i < bytes.len() {
        let byte = bytes[i];
        if let Some(end) = comment {
            if bytes[i..].starts_with(end) {
                comment = None;
                i += end.len();
                continue;
            }
        } else if bytes[i..].starts_with(b"/*") {
            comment = Some(b"*/");
            i += 2;
            continue;
        } else if bytes[i..].starts_with(b"//") {
            comment = Some(b"\n");
            i += 2;
            continue;
        } else if byte == b'"' || byte == b'\'' {
            i += 1;
            while i < bytes.len() && bytes[i] != byte && bytes[i] != b'\n' {
                i += if bytes[i] == b'\\' { 2 } else { 1 };
            }
            i = (i + 1).min(bytes.len());
            continue;
        }
        if byte.is_ascii_alphabetic() || byte == b'_' {
            let end = (i..bytes.len()).find(|&k| !(bytes[k].is_ascii_alphanumeric() || bytes[k] == b'_')).unwrap_or(bytes.len());
            let fresh = i == 0 || !(bytes[i - 1].is_ascii_alphanumeric() || bytes[i - 1] == b'_');
            if let Some(new) = names.get(&text[i..end]).filter(|_| fresh) {
                out.push_str(&text[copied..i]);
                out.push_str(new);
                copied = end;
            }
            i = end;
        } else {
            i += 1;
        }
    }
    out.push_str(&text[copied..]);
    out
}

/// A unit's objects with their symbols named anew: what the same code
/// gives under the new names.
pub(crate) fn renamed_parts(built: &Built, names: &BTreeMap<String, String>) -> Built {
    let name = |old: &str| names.get(old).map_or(old, String::as_str).to_string();
    built
        .iter()
        .map(|edition| {
            let parts = edition.as_ref().map_err(String::clone)?;
            Ok(parts
                .iter()
                .map(|(key, part)| {
                    let mut part = part.clone();
                    let mut lines: Vec<String> = part
                        .relocations
                        .lines()
                        .map(|line| {
                            // A symbol's row begins with its name; a
                            // relocation's ends with its target's.
                            let (first, last) = (line.split(' ').next().unwrap_or(""), line.rsplit(' ').next().unwrap_or(""));
                            if key == "symbols" && names.contains_key(first) {
                                format!("{}{}", name(first), &line[first.len()..])
                            } else if key != "symbols" && names.contains_key(last) {
                                format!("{}{}", &line[..line.len() - last.len()], name(last))
                            } else {
                                line.to_string()
                            }
                        })
                        .collect();
                    if key == "symbols" {
                        lines.sort();
                    }
                    part.relocations = lines.join("\n") + if part.relocations.ends_with('\n') { "\n" } else { "" };
                    (name(key), part)
                })
                .collect())
        })
        .collect()
}

/// The cause a function of a unit's rows cannot take a new name for.
/// `everywhere` are the functions all six editions compile.
fn fixed(model: &Model, overlays: &Overlays, unit: &str, others: &BTreeMap<&str, BTreeSet<&str>>, everywhere: &BTreeSet<String>, name: &str) -> Option<String> {
    let own = model.entries[unit].iter().filter(|entry| entry.name == name && matches!(entry.what, What::Definition { .. })).collect::<Vec<_>>();
    if own.is_empty() {
        let part = model.parts(unit).find(|part| model.entries[*part].iter().any(|entry| entry.name == name && matches!(entry.what, What::Definition { .. })));
        return Some(match part {
            Some(part) => format!("a shared file defines it: {part}, for {} units", model.users(part).len()),
            None => "no file of the unit defines it".to_string(),
        });
    }
    if own.iter().any(|entry| !entry.local) {
        let image = overlays.image.get(unit).into_iter().flatten().find(|image| overlays.named.get(*image).is_some_and(|named| named.contains(name)));
        return Some(match image {
            Some(image) => format!("it is not static: another object of overlay {image} names it"),
            None => "it is not static".to_string(),
        });
    }
    // The count (C2) knows a function an edition leaves out by its name:
    // no image of that edition has the symbol. A name every overlay may
    // hold would hide that.
    if !everywhere.contains(name) {
        return Some("an edition leaves it out, and the count knows that by its name".to_string());
    }
    let other = model.reach[unit].iter().find(|file| others.get(file.as_str()).is_some_and(|named| named.contains(name)));
    other.map(|file| format!("another file of the unit names it too: {file}"))
}

/// What the pass makes of one unit: what it says of each function its
/// tables reach, the names it gives, and the rows it read.
struct Planned<'a> {
    unit: &'a String,
    outcomes: Vec<(String, Outcome)>,
    names: BTreeMap<String, String>,
    rows: Vec<Row>,
}

/// A cause without the names it gives after its colon, for the count.
fn kind(why: &str) -> &str {
    why.split(": ").next().unwrap_or(why)
}

/// The overlay units the pass reads: those `only` names, or all of them.
fn chosen<'a>(units: Vec<&'a String>, only: &[String]) -> Result<Vec<&'a String>, String> {
    if let Some(unknown) = only.iter().find(|file| !units.contains(file)) {
        return Err(format!("{unknown} is not the C file of an overlay"));
    }
    Ok(units.into_iter().filter(|unit| only.is_empty() || only.contains(*unit)).collect())
}

pub(crate) fn run(root: &Path, dry: bool, only: &[String], report: Option<&str>) -> Result<(), String> {
    let model = Model::read(root)?;
    let overlays = Overlays::read(root, &model)?;
    let units: Vec<&String> = overlays.image.keys().filter(|unit| !model.arm.contains(*unit)).collect();
    let mut units = chosen(units, only)?;
    units.sort_by_key(|unit| std::cmp::Reverse(model.text[*unit].len()));
    // Each unit's rows, of all six editions.
    let state = || objects::editions().iter().map(|target| Expansion::new(root, *target)).collect::<Result<Vec<_>, _>>();
    let read = super::parallel(&units, state, |expansions, unit| -> Result<(Vec<Row>, BTreeSet<String>), String> {
        // A row's value is a function where a C file of the unit defines it.
        let function = |name: &str| model.parts(unit).any(|part| model.entries[part].iter().any(|entry| entry.name == name && matches!(entry.what, What::Definition { .. })));
        let mut all: Vec<Row> = Vec::new();
        let mut everywhere: Option<BTreeSet<String>> = None;
        for expansion in expansions {
            let (found, defined) = rows(&expansion.code(unit)?, &function);
            for row in found {
                if !all.contains(&row) {
                    all.push(row);
                }
            }
            everywhere = Some(match everywhere {
                Some(so_far) => so_far.intersection(&defined).cloned().collect(),
                None => defined,
            });
        }
        Ok((all, everywhere.unwrap_or_default()))
    })?;
    // What every other file names: a name one of them reads stays.
    let others: BTreeMap<&str, BTreeSet<&str>> = model.fragments.iter().chain(&model.headers).map(|file| (file.as_str(), identifiers(&model.text[file]))).collect();
    let mut plans: Vec<Planned> = Vec::new();
    for (unit, found) in units.iter().zip(read) {
        let (found, everywhere) = found?;
        let text = &model.text[*unit];
        let mut functions: Vec<String> = Vec::new();
        for entry in model.entries[*unit].iter().filter(|entry| matches!(entry.what, What::Definition { .. })) {
            if !functions.contains(&entry.name) {
                functions.push(entry.name.clone());
            }
        }
        let own = identifiers(text);
        let image: Vec<&BTreeSet<String>> = overlays.image[*unit].iter().filter_map(|image| overlays.named.get(image)).collect();
        let taken = |word: &str| own.contains(word) || image.iter().any(|named| named.contains(word)) || model.reach[*unit].iter().any(|file| others.get(file.as_str()).is_some_and(|named| named.contains(word)));
        let outcomes = names(&found, &functions, &|name| fixed(&model, &overlays, unit, &others, &everywhere, name), &taken);
        let map: BTreeMap<String, String> = outcomes.iter().filter_map(|(name, outcome)| match outcome {
            Outcome::Renamed(new) => Some((name.clone(), new.clone())),
            _ => None,
        }).collect();
        plans.push(Planned { unit, outcomes, names: map, rows: found });
    }
    // Each unit under its new names gives the objects it gave, named anew.
    let judged = super::parallel(&plans, || Bench::new(root), |bench, planned| {
        let (unit, map) = (planned.unit, &planned.names);
        if map.is_empty() {
            return None;
        }
        let text = &model.text[unit];
        let before = renamed_parts(&bench.build(root, unit, text), map);
        let now = bench.build(root, unit, &renamed(text, map));
        let parts = objects::differing(&before, &now);
        parts.iter().next().map(|first| match objects::difference(&before, &now, first) {
            Some(how) => format!("{first} {how}"),
            None => now.iter().find_map(|edition| edition.as_ref().err()).map_or_else(|| format!("{first} is not what it was"), |messages| format!("does not compile: {}", objects::first_error(messages))),
        })
    })?;
    let (mut renamed_count, mut named, mut kept, mut files) = (0, 0, 0, 0);
    let mut causes: BTreeMap<String, usize> = BTreeMap::new();
    let mut words: BTreeMap<String, usize> = BTreeMap::new();
    let mut rows_text = String::from("unit\tfunction\tname\tcause\tids\tkinds\ttables\n");
    for (planned, differs) in plans.iter().zip(&judged) {
        let Planned { unit, outcomes, names: map, rows: found } = planned;
        if let Some(how) = differs {
            println!("differs, left as it was: {unit}: {how}");
        }
        for (name, outcome) in outcomes {
            let own: Vec<&Row> = found.iter().filter(|row| row.names.contains(name)).collect();
            let ids: BTreeSet<String> = own.iter().map(|row| row.id.map_or("?".to_string(), |id| id.to_string())).collect();
            let kinds: BTreeSet<&str> = own.iter().map(|row| row.kind.as_deref().unwrap_or("number")).collect();
            let mut tables: Vec<&str> = own.iter().map(|row| row.table.as_str()).collect();
            tables.dedup();
            let (new, cause) = match outcome {
                Outcome::Renamed(_) if differs.is_some() => {
                    kept += 1;
                    *causes.entry("its unit's objects differ under the new names".to_string()).or_default() += 1;
                    (name.as_str(), "its unit's objects differ under the new names")
                }
                Outcome::Renamed(new) => {
                    renamed_count += 1;
                    *words.entry(event_name(new).map_or("", |(_, word)| word.trim_end_matches(|c: char| c.is_ascii_digit() || c == '_')).to_string()).or_default() += 1;
                    (new.as_str(), "")
                }
                Outcome::Named => {
                    named += 1;
                    (name.as_str(), "it has its name")
                }
                Outcome::Kept(why) => {
                    kept += 1;
                    *causes.entry(kind(why).to_string()).or_default() += 1;
                    (name.as_str(), why.as_str())
                }
            };
            rows_text.push_str(&format!("{unit}\t{name}\t{new}\t{cause}\t{}\t{}\t{}\n", ids.into_iter().collect::<Vec<_>>().join(","), kinds.into_iter().collect::<Vec<_>>().join(","), tables.join(",")));
        }
        if differs.is_none() && !map.is_empty() {
            files += 1;
            if !dry {
                super::write(root, unit, &renamed(&model.text[*unit], map))?;
            }
        }
    }
    if let Some(path) = report {
        std::fs::write(path, &rows_text).map_err(|e| format!("{path}: {e}"))?;
    }
    for (cause, count) in &causes {
        println!("kept: {count}: {cause}");
    }
    let by_word: Vec<String> = words.iter().map(|(word, count)| format!("{word} {count}")).collect();
    println!(
        "{renamed_count} event functions {} their name in {files} units ({}); {named} have it; {kept} keep theirs",
        if dry { "would take" } else { "took" },
        by_word.join(", ")
    );
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    const TEXT: &str = "# 1 \"games/tbs/src/o/a.c\"\nenum SceneEventKind\n{\n\tEVENT_TALK = 0,\n\tEVENT_EXIT = 1,\n\tEVENT_TOUCH = 2,\n\tEVENT_PSYNERGY = 5\n};\nenum { FACING_NORTH, FACING_EAST };\nenum Actor\n{\n\tACTOR_FIRST = 8,\n\tACTOR_GUARD,\n\tACTOR_LAST = ACTOR_FIRST + 4\n};\nstruct SceneEvent\n{\n\tu32 control;\n};\nextern struct SceneEvent gMuraEvents2[];\nstatic void Guard_Talk(void)\n{\n}\nstruct SceneEvent gMuraEvents[] = {\n\t{ EVENT_TALK, (ACTOR_GUARD), -1, (((u32)(Guard_Talk))) },\n\t{ (EVENT_TOUCH | 0x200 | 0x400 | (FACING_EAST)), 0x1f, -1, ((u32)(Gate_Open)) },\n\t{ 0x202, 31, 0x94a, ((u32)(Gate_Open)) },\n\t{ EVENT_EXIT, 3, -1, 0x10004 },\n\t{ (EVENT_PSYNERGY | 0x10 | (0x21) << 8), (s16)(ACTOR_LAST), -1, ((s32)&MsgSealed) },\n\t{ 0xffffffff },\n};\nstruct SceneEvent gMuraEvents2[] = {\n\t{ EVENT_TALK, unknown, -1, ((u32)(Guard_Talk)) },\n};\n";

    #[test]
    fn a_value_is_read_from_numbers_enumerators_and_operators() {
        let (constants, kinds) = constants(TEXT);
        assert_eq!(constants["ACTOR_GUARD"], 9);
        assert_eq!(constants["ACTOR_LAST"], 12);
        assert_eq!(constants["FACING_EAST"], 1);
        assert_eq!(kinds.iter().map(String::as_str).collect::<Vec<_>>(), ["EVENT_EXIT", "EVENT_PSYNERGY", "EVENT_TALK", "EVENT_TOUCH"]);
        assert_eq!(value("(EVENT_TOUCH | 0x200 | 0x400 | (FACING_EAST))", &constants), Some(0x603));
        assert_eq!(value("(s16)(ACTOR_LAST)", &constants), Some(12));
        assert_eq!(value("(ACTOR_GUARD)", &constants), Some(9));
        assert_eq!(value("-(2 + 3 * 4) << 1", &constants), Some(-28));
        assert_eq!(value("0x21 << 8 | 010", &constants), Some(0x2108));
        assert_eq!(value("unknown", &constants), None);
        assert_eq!(value("3 4", &constants), None);
    }

    #[test]
    fn a_tables_rows_give_each_function_its_kind_and_id() {
        let row = |table: &str, kind: Option<&str>, id: Option<i64>, name: &str| Row { table: table.to_string(), kind: kind.map(str::to_string), id, names: vec![name.to_string()] };
        let (found, defined) = rows(TEXT, &|name| ["Guard_Talk", "Gate_Open", "MsgSealed"].contains(&name));
        assert_eq!(defined.into_iter().collect::<Vec<_>>(), ["Guard_Talk"]);
        assert_eq!(
            found,
            [
                row("gMuraEvents", Some("TALK"), Some(9), "Guard_Talk"),
                row("gMuraEvents", Some("TOUCH"), Some(31), "Gate_Open"),
                row("gMuraEvents", None, Some(31), "Gate_Open"),
                row("gMuraEvents", Some("PSYNERGY"), Some(12), "MsgSealed"),
                row("gMuraEvents2", Some("TALK"), None, "Guard_Talk"),
            ]
        );
    }

    fn row(table: &str, kind: Option<&str>, id: i64, name: &str) -> Row {
        Row { table: table.to_string(), kind: kind.map(str::to_string), id: Some(id), names: vec![name.to_string()] }
    }

    #[test]
    fn a_function_takes_its_id_and_the_kind_its_rows_spell() {
        let rows = [
            row("gEvents1", Some("TALK"), 8, "Guard_Talk"),
            row("gEvents2", Some("TALK"), 8, "Guard_Talk"),
            row("gEvents1", Some("TOUCH"), 31, "Gate_Open"),
            row("gEvents1", None, 31, "Gate_Open"),
            row("gEvents1", None, 70, "Start"),
            row("gEvents1", Some("TALK"), 20, "EV20_MAIN"),
            row("gEvents1", Some("PSYNERGY"), 21, "EV20_END"),
            row("gEvents1", Some("TOUCH"), 40, "Many"),
            row("gEvents1", Some("TOUCH"), 41, "Many"),
            row("gEvents1", Some("SEARCH"), 99, "Shared"),
            row("gEvents1", Some("SEARCH"), 98, "Entry"),
            row("gEvents1", Some("TALK"), 120, "Taken"),
            row("gEvents1", Some("PSYNERGY"), 12, "MsgSealed"),
        ];
        let functions: Vec<String> = ["Start", "Guard_Talk", "Gate_Open", "EV20_MAIN", "EV20_END", "Many", "Entry", "Taken"].iter().map(|name| name.to_string()).collect();
        let fixed = |name: &str| match name {
            "Shared" => Some("a shared file defines it: games/tbs/src/o/f.c, for 3 units".to_string()),
            "Entry" => Some("it is not static".to_string()),
            "MsgSealed" => Some("no file of the unit defines it".to_string()),
            _ => None,
        };
        let found = names(&rows, &functions, &fixed, &|word| word == "EV120_TALK");
        let said: Vec<(&str, String)> = found.iter().map(|(name, outcome)| (name.as_str(), match outcome {
            Outcome::Renamed(new) => new.clone(),
            Outcome::Named => "named".to_string(),
            Outcome::Kept(why) => format!("kept: {why}"),
        })).collect();
        assert_eq!(
            said,
            [
                ("Start", "EV70_SCRIPT".to_string()),
                ("Guard_Talk", "EV08_TALK".to_string()),
                ("Gate_Open", "EV31_SCRIPT".to_string()),
                ("EV20_MAIN", "named".to_string()),
                ("EV20_END", "EV21_END".to_string()),
                ("Many", "kept: it is reached under several ids: 40, 41".to_string()),
                ("Entry", "kept: it is not static".to_string()),
                ("Taken", "kept: the unit has the name already: EV120_TALK".to_string()),
                ("Shared", "kept: a shared file defines it: games/tbs/src/o/f.c, for 3 units".to_string()),
                ("MsgSealed", "kept: no file of the unit defines it".to_string()),
            ]
        );
    }

    #[test]
    fn two_functions_of_one_id_and_kind_are_told_apart_by_their_tables_ordinal() {
        assert_eq!((ordinal("gHaidiaMuraEvents2"), ordinal("FuneHeya_SceneTable04"), ordinal("gRunpaJoEventsOther"), ordinal("9")), (Some("2"), Some("04"), None, None));
        assert_eq!((ordinal("gKuupuappuHeyaEventsFlag855"), ordinal("gKareiHeyaEvents1Entrance9"), ordinal("Data_02009098"), ordinal("gRariberoEvents9a7")), (None, None, None, None));
        assert_eq!((event_name("EV08_KAGI"), event_name("EV120_TALK_2"), event_name("EVENT_TALK"), event_name("EV_A"), event_name("EV08")), (Some((8, "KAGI")), Some((120, "TALK_2")), None, None, None));
        let rows = [
            row("gEvents", Some("TALK"), 10, "First"),
            row("gEvents2", Some("TALK"), 10, "First"),
            row("gEvents2", Some("TALK"), 11, "Second"),
            row("gEvents3", Some("TALK"), 10, "Third"),
            row("gEvents3", Some("TALK"), 11, "Fourth"),
            row("gEvents3", Some("TALK"), 11, "Fifth"),
            row("gEventsOther", Some("TOUCH"), 5, "Sixth"),
            row("gEventsLast", Some("TOUCH"), 5, "Seventh"),
        ];
        let functions: Vec<String> = ["First", "Second", "Third", "Fourth", "Fifth", "Sixth", "Seventh"].iter().map(|name| name.to_string()).collect();
        let found = names(&rows, &functions, &|_| None, &|_| false);
        let said: Vec<(&str, String)> = found.iter().map(|(name, outcome)| (name.as_str(), match outcome {
            Outcome::Renamed(new) => new.clone(),
            Outcome::Named => "named".to_string(),
            Outcome::Kept(why) => format!("kept: {why}"),
        })).collect();
        // First is reached from the table without an ordinal first, and
        // takes the plain name; Fourth and Fifth share id, kind and table.
        assert_eq!(
            said,
            [
                ("First", "EV10_TALK".to_string()),
                ("Second", "EV11_TALK_2".to_string()),
                ("Third", "EV10_TALK_3".to_string()),
                ("Fourth", "kept: another function has its id and kind, and their tables' names do not tell them apart: EV11_TALK with Second, Fifth".to_string()),
                ("Fifth", "kept: another function has its id and kind, and their tables' names do not tell them apart: EV11_TALK with Second, Fourth".to_string()),
                ("Sixth", "kept: another function has its id and kind, and their tables' names do not tell them apart: EV05_TOUCH with Seventh".to_string()),
                ("Seventh", "kept: another function has its id and kind, and their tables' names do not tell them apart: EV05_TOUCH with Sixth".to_string()),
            ]
        );
    }

    #[test]
    fn a_name_is_written_anew_in_code_and_comments_not_in_literals() {
        let names: BTreeMap<String, String> = [("Guard_Talk".to_string(), "EV08_TALK".to_string())].into();
        let text = "// Guard_Talk の前に don't\nstatic void Guard_Talk(void);\n/* FAKEMATCH: Guard_Talk's frame */\nconst char *n = \"Guard_Talk\"; char c = '\\''; u32 f = (u32)Guard_Talk + OldGuard_Talk + Guard_Talk2;\n";
        assert_eq!(
            renamed(text, &names),
            "// EV08_TALK の前に don't\nstatic void EV08_TALK(void);\n/* FAKEMATCH: EV08_TALK's frame */\nconst char *n = \"Guard_Talk\"; char c = '\\''; u32 f = (u32)EV08_TALK + OldGuard_Talk + Guard_Talk2;\n"
        );
    }

    #[test]
    fn objects_under_new_names_are_the_objects_named_anew() {
        use objects::Part;
        let built = |name: &str, other: &str| -> Built {
            let symbols = Part { relocations: format!("{name} .text 4 Text Compilation\n{other} .text 4 Text Compilation\nTable .data 8 Data Linkage"), ..Part::default() };
            let table = Part { bytes: vec![0; 8], relocations: format!("0x0 Elf {{ r_type: 2 }} 0 {name}\n0x4 Elf {{ r_type: 2 }} 0 {other}\n"), code: false };
            vec![Ok([(name.to_string(), Part { bytes: vec![1], ..Part::default() }), (other.to_string(), Part { bytes: vec![2], ..Part::default() }), ("Table".to_string(), table), ("symbols".to_string(), symbols)].into())]
        };
        let names: BTreeMap<String, String> = [("Guard_Talk".to_string(), "EV08_TALK".to_string()), ("Zeta".to_string(), "EV09_TALK".to_string())].into();
        let old = renamed_parts(&built("Guard_Talk", "Zeta"), &names);
        // The symbols' rows are in the order of their new names.
        assert!(objects::differing(&old, &built("EV08_TALK", "EV09_TALK")).is_empty());
        assert_eq!(objects::differing(&old, &built("EV08_TALK", "EV10_TALK")).into_iter().collect::<Vec<_>>(), ["EV09_TALK", "EV10_TALK", "Table", "symbols"]);
    }

    #[test]
    fn named_files_limit_the_pass_to_their_overlays() {
        let all: Vec<String> = ["games/tbs/src/scene/a.c", "games/tbs/src/scene/b.c", "games/tbs/src/menu/c.c"].iter().map(|unit| unit.to_string()).collect();
        let units = || all.iter().collect::<Vec<_>>();
        assert_eq!(chosen(units(), &[]).unwrap().len(), 3);
        let only = ["games/tbs/src/menu/c.c".to_string(), "games/tbs/src/scene/a.c".to_string()];
        assert_eq!(chosen(units(), &only).unwrap(), [&all[0], &all[2]]);
        let header = ["games/tbs/src/scene/a.h".to_string()];
        assert_eq!(chosen(units(), &header).unwrap_err(), "games/tbs/src/scene/a.h is not the C file of an overlay");
    }

    #[test]
    fn a_cause_is_counted_by_its_kind() {
        assert_eq!(kind("it is reached under several ids: 40, 41, 42"), "it is reached under several ids");
        assert_eq!(kind("it is not static: another object of overlay 3a0 names it"), "it is not static");
        assert_eq!(kind("its rows' id is not a number the pass reads"), "its rows' id is not a number the pass reads");
    }
}
