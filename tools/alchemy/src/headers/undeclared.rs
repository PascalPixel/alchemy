//! alchemy headers undeclared: the functions a file calls before it defines
//! them, with no prototype in scope.
//!
//! A call with no declaration in sight is C89's implicit `int name()`: the
//! compiler arranges it as a call whose result may be used. One prototype
//! per function in its file's header gives every caller the definition's
//! type, and a caller above the definition that the bytes show as the
//! implicit call then needs a cast to say so. The source had no prototype
//! there. This pass measures it: the function is left out of its file's
//! header, under a comment in the prototype's place that says so, and its
//! callers above the definition are written as plain calls. Where every
//! unit that compiles the file gives the same objects in the six editions,
//! that is how the source stays. A function stays declared unless the bytes
//! need otherwise, and one another file names cannot be left out: that file
//! would call it undeclared.

use super::expand;
use super::model::{unit_header, Model, SOURCE};
use super::objects::{self, Bench, Built};
use super::plan::{apply, removal};
use super::scan::{bodies, How, Use, What};
use crate::camelotize::source::{mask, next, opening, previous, word_before};
use crate::camelotize::trial::Edit;
use crate::compiler::preprocess::Expansion;
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

/// What the comment says after the function's name.
const SAYS: &str = " is called before its definition with no prototype in scope. */";

/// The comment that stands in a header where the function's prototype
/// would: the source's own record that the function is left out.
pub(crate) fn comment(name: &str) -> String {
    format!("/* {name}{SAYS}")
}

/// The other form of the comment, for a function another unit calls with
/// no prototype in scope, on one line: `/* NAME is called from FILE.c with
/// no prototype in scope`, then what that call shows, then ` */`. Two files
/// whose merge would put the definition above such a call keep it apart.
const FROM: &str = " is called from ";
const UNDECLARED: &str = " with no prototype in scope";

/// One comment that leaves a function out of its header: the function,
/// the other C file whose calls it records (none for a call above the
/// definition in the function's own file), and the comment's byte range.
pub(crate) struct Note<'a> {
    pub name: &'a str,
    pub from: Option<&'a str>,
    pub start: usize,
    pub end: usize,
}

fn identifier(name: &str) -> bool {
    !name.is_empty() && name.bytes().all(|b| b.is_ascii_alphanumeric() || b == b'_')
}

/// The comments of a header that leave functions out, in either form.
pub(crate) fn notes(header: &str) -> Vec<Note<'_>> {
    let mut found = Vec::new();
    let mut at = 0;
    for line in header.split_inclusive('\n') {
        let trimmed = line.trim();
        let start = at + line.find("/*").unwrap_or(0);
        let end = start + trimmed.len();
        at += line.len();
        let Some(rest) = trimmed.strip_prefix("/* ") else {
            continue;
        };
        if let Some(name) = rest.strip_suffix(SAYS).filter(|name| identifier(name)) {
            found.push(Note { name, from: None, start, end });
        } else if let Some((name, after)) = rest.split_once(FROM).filter(|(name, _)| identifier(name)) {
            let file = after.split_once(UNDECLARED).map(|(file, _)| file);
            if let Some(file) = file.filter(|file| file.ends_with(".c") && !file.contains(char::is_whitespace)) {
                if trimmed.ends_with(" */") && !rest.contains("*/ ") {
                    found.push(Note { name, from: Some(file), start, end });
                }
            }
        }
    }
    found
}

/// The functions a header's comments leave out: each name and its
/// comment's byte range.
pub(crate) fn left_out(header: &str) -> Vec<(&str, usize, usize)> {
    notes(header).into_iter().map(|note| (note.name, note.start, note.end)).collect()
}

/// The header with the function's prototype replaced by the comment; none
/// when the header does not declare it once, under no condition but the
/// header's own guard.
pub(crate) fn leave_out(header: &str, name: &str) -> Option<String> {
    let entries = super::scan::entries(header);
    let mut own = entries.iter().filter(|entry| entry.what == What::Prototype && entry.name == name);
    let (entry, None) = (own.next()?, own.next()) else {
        return None;
    };
    let guard = |condition: &String| condition.starts_with("!defined(") && condition.ends_with("_H)");
    if entry.declarators != 1 || !entry.conditions.iter().all(guard) {
        return None;
    }
    Some(format!("{}{}{}", &header[..entry.start], comment(name), &header[entry.end..]))
}

/// The header with the function's comment replaced by its prototype.
pub(crate) fn declare(header: &str, name: &str, prototype: &str) -> Option<String> {
    let (_, start, end) = left_out(header).into_iter().find(|(found, _, _)| *found == name)?;
    Some(format!("{}{prototype}{}", &header[..start], &header[end..]))
}

/// How the source writes a call.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Written {
    /// `Name(...)`.
    Plain,
    /// `((type)Name)(...)`: the outer parenthesis and the one that closes
    /// it after the name.
    Cast { open: usize, close: usize },
    /// `ValueN(Name, ...)` or `CallN(Name, ...)`, the call helpers: the
    /// helper's first byte and where the arguments after the name begin.
    Helper { start: usize, rest: usize },
}

/// One call of a function in a file's text.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Site {
    pub at: usize,
    pub name: String,
    pub written: Written,
}

/// Whether a word names one of call.h's helpers, which call a function
/// through a pointer of another shape.
fn helper(word: &str) -> bool {
    ["Value", "Call"].iter().any(|stem| word.strip_prefix(stem).is_some_and(|count| !count.is_empty() && count.bytes().all(|b| b.is_ascii_digit())))
}

fn written(text: &str, mask: &[u8], used: &Use) -> Option<Written> {
    let end = used.at + used.name.len();
    match &used.how {
        How::Call => Some(Written::Plain),
        How::Cast(_) => {
            let open = previous(mask, opening(mask, previous(mask, used.at)?)?)?;
            Some(Written::Cast { open, close: next(mask, end)? })
        }
        How::Value => {
            let open = previous(mask, used.at).filter(|&at| mask[at] == b'(')?;
            let word = word_before(mask, open).filter(|word| helper(&text[word.clone()]))?;
            let after = next(mask, end)?;
            let rest = match mask[after] {
                b',' => next(mask, after + 1)?,
                b')' => after,
                _ => return None,
            };
            Some(Written::Helper { start: word.start, rest })
        }
        How::View { .. } => None,
    }
}

/// Every call of a wanted name in a text's function bodies, as written.
pub(crate) fn sites(text: &str, wanted: &dyn Fn(&str) -> bool) -> Vec<Site> {
    let mask = mask(text);
    let mut found = Vec::new();
    for body in bodies(text, wanted) {
        for used in &body.uses {
            if let Some(written) = written(text, &mask, used) {
                found.push(Site { at: used.at, name: used.name.clone(), written });
            }
        }
    }
    found
}

/// The edits that write a call plainly.
pub(crate) fn plain(site: &Site) -> Vec<Edit> {
    let end = site.at + site.name.len();
    match site.written {
        Written::Plain => Vec::new(),
        Written::Cast { open, close } => vec![Edit { start: open, end: site.at, text: String::new() }, Edit { start: end, end: close + 1, text: String::new() }],
        Written::Helper { start, rest } => vec![Edit { start, end: rest, text: format!("{}(", site.name) }],
    }
}

/// Every comment of a text that holds a FAKEMATCH tag, as a byte range.
fn tags(text: &str) -> Vec<(usize, usize)> {
    let bytes = text.as_bytes();
    let mut found = Vec::new();
    let mut i = 0;
    while i < bytes.len() {
        let comment = if bytes[i..].starts_with(b"/*") {
            Some(text[i + 2..].find("*/").map_or(bytes.len(), |at| i + 2 + at + 2))
        } else if bytes[i..].starts_with(b"//") {
            Some(text[i..].find('\n').map_or(bytes.len(), |at| i + at))
        } else {
            None
        };
        if let Some(end) = comment {
            if text[i..end].contains("FAKEMATCH") {
                found.push((i, end));
            }
            i = end;
        } else if bytes[i] == b'"' || bytes[i] == b'\'' {
            let mut end = i + 1;
            while end < bytes.len() && bytes[end] != bytes[i] && bytes[end] != b'\n' {
                end += if bytes[end] == b'\\' { 2 } else { 1 };
            }
            i = (end + 1).min(bytes.len());
        } else {
            i += 1;
        }
    }
    found
}

/// The semicolon that ends the statement starting at `first`, or the
/// brace that opens its block.
fn statement_end(mask: &[u8], first: usize, limit: usize) -> usize {
    let mut depth = 0usize;
    for (at, &byte) in mask.iter().enumerate().take(limit).skip(first) {
        match byte {
            b'(' | b'[' => depth += 1,
            b')' | b']' => depth = depth.saturating_sub(1),
            b';' | b'{' | b'}' if depth == 0 => return at,
            _ => {}
        }
    }
    limit
}

/// Where the statement that ends with the semicolon at `last` starts.
fn statement_start(mask: &[u8], last: usize, limit: usize) -> usize {
    let mut depth = 0usize;
    for at in (limit + 1..last).rev() {
        match mask[at] {
            b')' | b']' => depth += 1,
            b'(' | b'[' => depth = depth.saturating_sub(1),
            b';' | b'{' | b'}' if depth == 0 => return at + 1,
            _ => {}
        }
    }
    limit + 1
}

/// Whether a use is a call under another shape: a cast of the function
/// that is called, or a call helper.
fn shaped(used: &Use) -> bool {
    matches!(used.how, How::Cast(_)) || (used.how == How::Call && helper(&used.name))
}

/// The tags that go with the calls now written plainly: a FAKEMATCH
/// comment directly above such a call's statement, or at its end on its
/// line, when no cast of a called function and no call helper is left
/// between the comment and the next tag of its function.
pub(crate) fn tag_edits(text: &str, sites: &[Site]) -> Vec<Edit> {
    if sites.is_empty() {
        return Vec::new();
    }
    let mask = mask(text);
    let mut gone: BTreeSet<usize> = sites.iter().map(|site| site.at).collect();
    gone.extend(sites.iter().filter_map(|site| match site.written {
        Written::Helper { start, .. } => Some(start),
        _ => None,
    }));
    let tags = tags(text);
    let line_start = |at: usize| text[..at].rfind('\n').map_or(0, |n| n + 1);
    let mut edits = Vec::new();
    for body in bodies(text, &|_| true) {
        let own: Vec<(usize, usize)> = tags.iter().copied().filter(|(start, _)| body.open < *start && *start < body.close).collect();
        for (index, &(start, end)) in own.iter().enumerate() {
            let from = line_start(start);
            let to = own.get(index + 1).map_or(body.close, |(next, _)| line_start(*next));
            let alone = text[from..start].trim().is_empty();
            // The statement the tag stands on: the one below it, or the
            // one that ends on its line, before it or right after it.
            let statement = if alone {
                next(&mask, end).map(|first| (first, statement_end(&mask, first, body.close)))
            } else {
                let before = previous(&mask, start).filter(|&last| mask[last] == b';' && last >= from);
                let last = before.or_else(|| next(&mask, end).filter(|&after| mask[after] == b';'));
                last.map(|last| (statement_start(&mask, last, body.open), last))
            };
            let Some((first, last)) = statement else {
                continue;
            };
            let left = body.uses.iter().any(|used| from <= used.at && used.at < to.max(from) && !gone.contains(&used.at) && shaped(used));
            if gone.range(first..=last.max(first)).next().is_none() || left {
                continue;
            }
            edits.push(if alone { removal(text, start, end) } else { Edit { start: text[..start].trim_end_matches([' ', '\t']).len(), end, text: String::new() } });
        }
    }
    edits
}

/// A function that may be left out of its file's header, or is.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) struct Candidate {
    /// The C file that defines the function, and that file's header.
    pub home: String,
    pub header: String,
    pub name: String,
    /// Its calls above the definition that are written under another
    /// shape, by the file that writes them.
    pub sites: BTreeMap<String, Vec<Site>>,
    /// How many plain calls stand above the definition.
    pub plain: usize,
    /// Whether the header leaves it out already.
    pub out: bool,
    /// Why it cannot be left out, whatever the bytes say.
    pub refused: Option<String>,
}

impl Candidate {
    fn count(&self) -> usize {
        self.sites.values().map(Vec::len).sum()
    }
}

/// What the measurement says of one candidate.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Verdict {
    /// The bytes agree with no prototype in scope: the function is left
    /// out, and its calls above the definition are plain.
    Undeclared,
    /// The calls match as plain calls with the prototype in scope: no
    /// cast was needed, and the function stays declared.
    Spare,
    /// The objects differ with the function left out: it stays as it is.
    Kept(String),
    /// A function already left out whose bytes still need that.
    Stays,
    /// A function already left out whose bytes agree with its prototype
    /// in scope: it is declared again.
    Declared,
    Refused(String),
}

impl Verdict {
    fn text(&self) -> String {
        match self {
            Verdict::Undeclared => "undeclared: the same objects with no prototype in scope".to_string(),
            Verdict::Spare => "spare: the same objects as plain calls with the prototype in scope".to_string(),
            Verdict::Kept(how) => format!("kept: with no prototype in scope {how}"),
            Verdict::Stays => "stays undeclared: the objects differ with the prototype in scope".to_string(),
            Verdict::Declared => "declared again: the same objects with the prototype in scope".to_string(),
            Verdict::Refused(why) => format!("refused: {why}"),
        }
    }
}

/// What one edition's expansion of a unit says of the names under trial.
#[derive(Default)]
pub(crate) struct Seen {
    pub named: BTreeSet<String>,
    /// Each use inside a function: the file and line that write it, the
    /// name, and whether the name's definition stands above it.
    pub uses: Vec<(String, usize, String, bool)>,
}

fn line(text: &str, offset: usize) -> usize {
    text[..offset].matches('\n').count() + 1
}

/// The candidates among the functions the C files define: each one with a
/// call above its definition under another shape, and each one a header
/// leaves out. `written` gives every file's calls, `seen` every unit's
/// six expansions.
pub(crate) fn candidates(model: &Model, headers: &BTreeMap<String, String>, written: &BTreeMap<String, Vec<Site>>, seen: &BTreeMap<String, Vec<Seen>>) -> Vec<Candidate> {
    let definitions = model.definitions();
    let mut found = Vec::new();
    for (name, homes) in &definitions {
        let mut done = BTreeSet::new();
        for (home, entry) in homes {
            if !matches!(entry.what, What::Definition { .. }) || !done.insert(*home) {
                continue;
            }
            let Some(header) = unit_header(home) else {
                continue;
            };
            let text = headers.get(&header);
            let out = text.is_some_and(|text| left_out(text).iter().any(|(found, _, _)| found == name));
            let users = model.users(home);
            // Where each unit's editions saw the name, and whether its
            // definition stood above.
            let mut above: BTreeMap<(&str, usize), Vec<bool>> = BTreeMap::new();
            for unit in &users {
                for edition in seen.get(*unit).into_iter().flatten() {
                    for (file, line, used, defined) in &edition.uses {
                        if used == name {
                            above.entry((file.as_str(), *line)).or_default().push(*defined);
                        }
                    }
                }
            }
            let mut candidate = Candidate { home: home.to_string(), header: header.clone(), name: name.to_string(), out, ..Candidate::default() };
            let parts: BTreeSet<&str> = users.iter().flat_map(|unit| model.parts(unit)).collect();
            for part in parts {
                for site in written.get(part).into_iter().flatten().filter(|site| site.name == *name) {
                    let early = above.get(&(part, line(&model.text[part], site.at))).is_some_and(|flags| flags.iter().all(|defined| !defined));
                    if !early {
                        continue;
                    }
                    if site.written == Written::Plain {
                        candidate.plain += 1;
                    } else {
                        candidate.sites.entry(part.to_string()).or_default().push(site.clone());
                    }
                }
            }
            if candidate.sites.is_empty() && !out {
                continue;
            }
            let other = model.units.iter().find(|unit| {
                !users.contains(&unit.as_str())
                    && seen.get(*unit).is_some_and(|editions| editions.iter().any(|edition| edition.named.contains(*name)))
                    && model.home(&definitions, unit, name).is_some_and(|(file, _)| file == *home)
            });
            let far = model.far.iter().find(|(_, far)| far.target == *name).filter(|_| model.in_main(home));
            let arm = reached(model, &candidate).into_iter().find(|unit| model.arm.contains(*unit));
            candidate.refused = if let Some(unit) = other {
                Some(format!("{unit} names it too"))
            } else if let Some((stub, far)) = far {
                Some(format!("the far-call entry {stub} of {} reaches it", far.file))
            } else if let Some(unit) = arm {
                Some(format!("{unit} is on the ARM route, which only the whole build compiles"))
            } else if text.is_none() {
                Some(format!("{header} is not there"))
            } else if !out && text.is_some_and(|text| leave_out(text, name).is_none()) {
                Some(format!("{header} does not declare it once, under no condition"))
            } else if out && homes.iter().filter(|(file, _)| file == home).count() > 1 {
                Some("it has several wordings".to_string())
            } else {
                None
            };
            found.push(candidate);
        }
    }
    found
}

/// The files a set of candidates changes, as their new texts: the calls
/// written plainly, each header with the comment or the prototype, and,
/// with `tidy`, the tags that went with the calls taken out, which the
/// second value counts.
pub(crate) fn changes(model: &Model, headers: &BTreeMap<String, String>, chosen: &[(&Candidate, &Verdict)], tidy: bool) -> (BTreeMap<String, String>, usize) {
    let mut plain_sites: BTreeMap<&str, Vec<Site>> = BTreeMap::new();
    let mut new: BTreeMap<String, String> = BTreeMap::new();
    let mut tags = 0;
    for (candidate, verdict) in chosen {
        if matches!(verdict, Verdict::Undeclared | Verdict::Spare) {
            for (file, sites) in &candidate.sites {
                plain_sites.entry(file).or_default().extend(sites.iter().cloned());
            }
        }
        let header = new.get(&candidate.header).or_else(|| headers.get(&candidate.header));
        let text = match verdict {
            Verdict::Undeclared => header.and_then(|text| leave_out(text, &candidate.name)),
            Verdict::Declared => {
                let prototype = model.entries[&candidate.home].iter().find_map(|entry| match &entry.what {
                    What::Definition { prototype } if entry.name == candidate.name && !entry.local => Some(prototype),
                    _ => None,
                });
                header.zip(prototype).and_then(|(text, prototype)| declare(text, &candidate.name, prototype))
            }
            _ => None,
        };
        if let Some(text) = text {
            new.insert(candidate.header.clone(), text);
        }
    }
    for (file, sites) in plain_sites {
        let text = &model.text[file];
        let mut edits: Vec<Edit> = sites.iter().flat_map(plain).collect();
        if tidy {
            let gone = tag_edits(text, &sites);
            tags += gone.len();
            edits.extend(gone);
        }
        new.insert(file.to_string(), apply(text, &edits));
    }
    (new, tags)
}

/// The units a candidate's changes reach: every unit that compiles its
/// file or a file whose calls change.
fn reached<'a>(model: &'a Model, candidate: &'a Candidate) -> BTreeSet<&'a str> {
    std::iter::once(candidate.home.as_str()).chain(candidate.sites.keys().map(String::as_str)).flat_map(|file| model.users(file)).collect()
}

/// A unit's objects with the changed files in place of the tree's.
fn build(root: &Path, bench: &Bench, model: &Model, unit: &str, new: &BTreeMap<String, String>) -> Built {
    let others: Vec<(String, String)> = new.iter().filter(|(file, _)| model.reach[unit].contains(*file)).map(|(file, text)| (file.clone(), text.clone())).collect();
    bench.build_among(root, unit, new.get(unit).unwrap_or(&model.text[unit]), &others)
}

/// How the first of the units differs from before with the changes in
/// place, if any does.
fn differs(root: &Path, bench: &Bench, model: &Model, before: &BTreeMap<&str, Built>, units: &BTreeSet<&str>, new: &BTreeMap<String, String>) -> Option<String> {
    for unit in units {
        let now = build(root, bench, model, unit, new);
        if let Some(Err(messages)) = now.iter().find(|built| built.is_err()) {
            return Some(format!("{unit} does not compile: {}", objects::first_error(messages).rsplit('/').next().unwrap_or("")));
        }
        if let Some(part) = objects::differing(&before[unit], &now).into_iter().next() {
            return Some(format!("{part} {}", objects::difference(&before[unit], &now, &part).unwrap_or_else(|| "differs".to_string())));
        }
    }
    None
}

/// What every edition's expansion of every unit says of the names.
fn expansions(root: &Path, model: &Model, names: &BTreeSet<String>) -> Result<BTreeMap<String, Vec<Seen>>, String> {
    let prefix = root.to_string_lossy().into_owned();
    let wanted = |name: &str| names.contains(name);
    let state = || objects::editions().iter().map(|target| Expansion::new(root, *target)).collect::<Result<Vec<_>, _>>();
    let found = super::parallel(&model.units, state, |expansions, unit| -> Result<Vec<Seen>, String> {
        expansions
            .iter()
            .map(|expansion| {
                let expanded = expand::read(&prefix, &expansion.code(unit)?, unit, &wanted);
                let uses = expanded.sights.into_iter().map(|sight| (sight.file, sight.line, sight.name, sight.defined)).collect();
                Ok(Seen { named: expanded.named, uses })
            })
            .collect()
    })?;
    model.units.iter().zip(found).map(|(unit, seen)| Ok((unit.clone(), seen?))).collect()
}

pub(crate) fn run(root: &Path, dry: bool, only: &[String], report: Option<&str>) -> Result<(), String> {
    let model = Model::read(root)?;
    let definitions = model.definitions();
    let functions: BTreeSet<&str> = definitions.iter().filter(|(_, homes)| homes.iter().any(|(_, entry)| matches!(entry.what, What::Definition { .. }))).map(|(name, _)| *name).collect();
    let files: Vec<&String> = model.units.iter().chain(&model.fragments).filter(|file| file.starts_with(SOURCE)).collect();
    if let Some(unknown) = only.iter().find(|file| !files.contains(file)) {
        return Err(format!("{unknown} is not a ☀️ C file the editions compile or include"));
    }
    let mut headers: BTreeMap<String, String> = BTreeMap::new();
    for header in files.iter().filter_map(|file| unit_header(file)) {
        if let Ok(text) = std::fs::read_to_string(root.join(&header)) {
            headers.insert(header, text);
        }
    }
    let written: BTreeMap<String, Vec<Site>> = files.iter().map(|file| (file.to_string(), sites(&model.text[*file], &|name| functions.contains(name)))).collect();
    let mut names: BTreeSet<String> = written.values().flatten().filter(|site| site.written != Written::Plain).map(|site| site.name.clone()).collect();
    names.extend(headers.values().flat_map(|text| left_out(text)).map(|(name, _, _)| name.to_string()));
    let seen = expansions(root, &model, &names)?;
    let mut found = candidates(&model, &headers, &written, &seen);
    found.retain(|candidate| only.is_empty() || only.contains(&candidate.home));
    // Every unit the trials build, as the tree has it.
    let units: Vec<&str> = found.iter().filter(|candidate| candidate.refused.is_none()).flat_map(|candidate| reached(&model, candidate)).collect::<BTreeSet<_>>().into_iter().collect();
    let built = super::parallel(&units, || Bench::new(root), |bench, unit| bench.build(root, unit, &model.text[*unit]))?;
    let before: BTreeMap<&str, Built> = units.iter().copied().zip(built).collect();
    let mut verdicts = super::parallel(&found, || Bench::new(root), |bench, candidate| {
        if let Some(why) = &candidate.refused {
            return Verdict::Refused(why.clone());
        }
        let units = reached(&model, candidate);
        if let Some(unit) = units.iter().find(|unit| before[*unit].iter().any(|built| built.is_err())) {
            return Verdict::Refused(format!("{unit} does not compile as it is"));
        }
        let trial = |verdict: Verdict| differs(root, bench, &model, &before, &units, &changes(&model, &headers, &[(candidate, &verdict)], false).0);
        if candidate.out {
            return if trial(Verdict::Declared).is_none() { Verdict::Declared } else { Verdict::Stays };
        }
        if trial(Verdict::Spare).is_none() {
            return Verdict::Spare;
        }
        match trial(Verdict::Undeclared) {
            None => Verdict::Undeclared,
            Some(how) => Verdict::Kept(how),
        }
    })?;
    // All that the trials accept, together: each unit once more.
    let accepted = |verdict: &Verdict| matches!(verdict, Verdict::Undeclared | Verdict::Declared);
    let (new, tags) = loop {
        let chosen: Vec<(&Candidate, &Verdict)> = found.iter().zip(&verdicts).filter(|(_, verdict)| accepted(verdict)).collect();
        let (new, tags) = changes(&model, &headers, &chosen, true);
        let units: Vec<&str> = chosen.iter().flat_map(|(candidate, _)| reached(&model, candidate)).collect::<BTreeSet<_>>().into_iter().collect();
        let bad = super::parallel(&units, || Bench::new(root), |bench, unit| differs(root, bench, &model, &before, &BTreeSet::from([*unit]), &new).map(|how| (*unit, how)))?;
        let bad: BTreeMap<&str, String> = bad.into_iter().flatten().collect();
        if bad.is_empty() {
            break (new, tags);
        }
        // A unit that differs with all of them: none of its candidates stays.
        let dropped: Vec<(usize, String)> = found
            .iter()
            .enumerate()
            .filter(|(index, _)| accepted(&verdicts[*index]))
            .filter_map(|(index, candidate)| reached(&model, candidate).iter().find_map(|unit| bad.get(unit)).map(|how| (index, how.clone())))
            .collect();
        for (index, how) in dropped {
            verdicts[index] = Verdict::Kept(format!("and the unit's other functions left out, {how}"));
        }
    };
    let mut rows = String::from("file\tfunction\tcalls under another shape above the definition\tplain calls above it\tverdict\n");
    let mut counts: BTreeMap<&str, (usize, usize)> = BTreeMap::new();
    for (candidate, verdict) in found.iter().zip(&verdicts) {
        rows.push_str(&format!("{}\t{}\t{}\t{}\t{}\n", candidate.home, candidate.name, candidate.count(), candidate.plain, verdict.text()));
        let kind = match verdict {
            Verdict::Undeclared => "left out of their headers",
            Verdict::Spare => "with casts no byte needs",
            Verdict::Kept(_) => "kept as they are",
            Verdict::Stays => "still left out",
            Verdict::Declared => "declared again",
            Verdict::Refused(_) => "refused",
        };
        let count = counts.entry(kind).or_default();
        count.0 += 1;
        count.1 += candidate.count();
    }
    match report {
        Some(path) => std::fs::write(path, &rows).map_err(|e| format!("{path}: {e}"))?,
        None => print!("{rows}"),
    }
    for (kind, (functions, calls)) in &counts {
        println!("{functions} functions {kind} ({calls} calls)");
    }
    if dry {
        println!("{} files would change, {tags} FAKEMATCH tags would go", new.len());
        return Ok(());
    }
    for (file, text) in &new {
        super::write(root, file, text)?;
    }
    // What was written, compiled from the tree itself: a unit that
    // differs now puts every file back.
    let units: Vec<&str> = found.iter().zip(&verdicts).filter(|(_, verdict)| accepted(verdict)).flat_map(|(candidate, _)| reached(&model, candidate)).collect::<BTreeSet<_>>().into_iter().collect();
    let bad = super::parallel(&units, || Bench::new(root), |bench, unit| {
        let now = bench.build(root, unit, new.get(*unit).unwrap_or(&model.text[*unit]));
        objects::differing(&before[unit], &now).into_iter().next().map(|part| format!("{unit}: {part} differs"))
    })?;
    if let Some(how) = bad.into_iter().flatten().next() {
        for file in new.keys() {
            super::write(root, file, model.text.get(file).or_else(|| headers.get(file)).map_or("", String::as_str))?;
        }
        return Err(format!("{how} once the files are written: they are put back"));
    }
    println!("{} files changed, {tags} FAKEMATCH tags gone", new.len());
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_comment_stands_in_the_prototypes_place_and_is_read_back() {
        let header = "#ifndef ALCHEMY_UNIT_A_H\n#define ALCHEMY_UNIT_A_H\n\n#include \"types.h\"\n\nvoid Run(void);\nvoid Late(s32 mode,\n\ts32 owner);\n#if EDITION_INTERNATIONAL\nvoid Arm(void);\n#endif\nvoid Twice(void);\nvoid Twice(void);\n\n#endif\n";
        let said = "/* Late is called before its definition with no prototype in scope. */";
        assert_eq!(comment("Late"), said);
        let out = leave_out(header, "Late").unwrap();
        assert_eq!(out, header.replace("void Late(s32 mode,\n\ts32 owner);", said));
        assert_eq!(left_out(&out).into_iter().map(|(name, start, end)| (name, &out[start..end])).collect::<Vec<_>>(), [("Late", said)]);
        assert_eq!(declare(&out, "Late", "void Late(s32 mode, s32 owner);").unwrap(), header.replace(",\n\ts32 owner", ", s32 owner"));
        // A prototype under a condition, one written twice and one that is
        // not there are not replaced.
        assert_eq!(leave_out(header, "Arm"), None);
        assert_eq!(leave_out(header, "Twice"), None);
        assert_eq!(leave_out(header, "Other"), None);
        assert_eq!(declare(header, "Late", "void Late(void);"), None);
        // Any other comment leaves nothing out.
        assert!(left_out("/* Late is called before its definition. */\n// Late is called before its definition with no prototype in scope. */\n/* a b is called before its definition with no prototype in scope. */\n").is_empty());
    }

    #[test]
    fn a_comment_may_name_the_other_file_whose_calls_leave_it_out() {
        let across = "/* Late is called from b.c with no prototype in scope: in one file the definition would come first and Run changes. */";
        let header = format!("void Run(void);\n{across}\n/* Early is called before its definition with no prototype in scope. */\n");
        let found: Vec<(&str, Option<&str>, &str)> = notes(&header).into_iter().map(|note| (note.name, note.from, &header[note.start..note.end])).collect();
        assert_eq!(
            found,
            [("Late", Some("b.c"), across), ("Early", None, "/* Early is called before its definition with no prototype in scope. */")]
        );
        assert_eq!(left_out(&header).into_iter().map(|(name, _, _)| name).collect::<Vec<_>>(), ["Late", "Early"]);
        // The file is one C file's name, and the comment one line.
        assert!(notes("/* Late is called from b.h with no prototype in scope. */\n/* Late is called from two files with no prototype in scope. */\n/* Late is called from b.c with a prototype. */\n/* Late is called from b.c with no prototype in scope.\n */\n").is_empty());
    }

    #[test]
    fn calls_are_read_as_written_and_written_plainly() {
        let text = "void Run(s32 a)\n{\n\t((s32 (*)(s32, s32))Late)(a, 1);\n\t((s32 (*)())Late)(\n\t\ta, 2);\n\tValue2(Late, a, 3);\n\tCall0(Late);\n\tLate(a, 4);\n\tOther(Late, a);\n\tx = Value2(Other, Late);\n}\n";
        let found = sites(text, &|name| name == "Late");
        let kinds: Vec<&str> = found
            .iter()
            .map(|site| match site.written {
                Written::Plain => "plain",
                Written::Cast { .. } => "cast",
                Written::Helper { .. } => "helper",
            })
            .collect();
        assert_eq!(kinds, ["cast", "cast", "helper", "helper", "plain"]);
        let edits: Vec<Edit> = found.iter().flat_map(plain).collect();
        assert_eq!(apply(text, &edits), "void Run(s32 a)\n{\n\tLate(a, 1);\n\tLate(\n\t\ta, 2);\n\tLate(a, 3);\n\tLate();\n\tLate(a, 4);\n\tOther(Late, a);\n\tx = Value2(Other, Late);\n}\n");
    }

    #[test]
    fn a_tag_goes_with_the_calls_it_stood_on() {
        let text = "void Run(s32 a)\n{\n\t/* FAKEMATCH: void function called as returning s32. */\n\t((s32 (*)())Late)(a, 1);\n\t((s32 (*)())Late)(a, 2);\n\tPlain(a);\n\t/* FAKEMATCH: the result sets r0 last. */\n\tValue2(Veneer, a, 0);\n\tif (a)\n\t{\n\t\t/* FAKEMATCH: void functions called as returning s32. */\n\t\t((s32 (*)())Late)(a, 3);\n\t}\n\telse\n\t{\n\t\t((s32 (*)())Before)(a, 4);\n\t}\n\t((s32 (*)())Late)(a, 5); /* FAKEMATCH: called with no prototype. */\n\t/* FAKEMATCH: the zero is a forced temporary. */\n\tzero = 0;\n\t((s32 (*)())Late)(zero, 6);\n\t((s32 (*)())Late)(a, 7) /* FAKEMATCH: an argument too many. */;\n}\n";
        let found = sites(text, &|name| name == "Late");
        assert_eq!(found.len(), 6);
        let mut edits: Vec<Edit> = found.iter().flat_map(plain).collect();
        edits.extend(tag_edits(text, &found));
        // The first tag and the two at a call's end go with their calls. The
        // helper's tag stands on another call, the tag in the branch still
        // has the cast of the other branch under it, and the last one
        // stands on a statement that is no call.
        assert_eq!(
            apply(text, &edits),
            "void Run(s32 a)\n{\n\tLate(a, 1);\n\tLate(a, 2);\n\tPlain(a);\n\t/* FAKEMATCH: the result sets r0 last. */\n\tValue2(Veneer, a, 0);\n\tif (a)\n\t{\n\t\t/* FAKEMATCH: void functions called as returning s32. */\n\t\tLate(a, 3);\n\t}\n\telse\n\t{\n\t\t((s32 (*)())Before)(a, 4);\n\t}\n\tLate(a, 5);\n\t/* FAKEMATCH: the zero is a forced temporary. */\n\tzero = 0;\n\tLate(zero, 6);\n\tLate(a, 7);\n}\n"
        );
        // With the other branch's call plain too, its tag goes.
        let both = text.replace("Before", "Late");
        let found = sites(&both, &|name| name == "Late");
        let mut edits: Vec<Edit> = found.iter().flat_map(plain).collect();
        edits.extend(tag_edits(&both, &found));
        assert!(!apply(&both, &edits).contains("void functions"));
        assert!(tag_edits(text, &[]).is_empty());
    }

    const UNIT: &str = "games/tbs/src/main/a.c";
    const PART: &str = "games/tbs/src/main/f.c";
    const OTHER: &str = "games/tbs/src/o/b.c";
    const HEADER: &str = "games/tbs/include/unit/main/a.h";
    const PART_HEADER: &str = "games/tbs/include/unit/main/f.h";

    struct Sample {
        model: Model,
        headers: BTreeMap<String, String>,
        written: BTreeMap<String, Vec<Site>>,
        seen: BTreeMap<String, Vec<Seen>>,
    }

    impl Sample {
        fn candidates(&self) -> Vec<Candidate> {
            candidates(&self.model, &self.headers, &self.written, &self.seen)
        }
    }

    fn sample() -> Sample {
        let unit = "#include \"unit/main/a.h\"\nvoid Run(s32 a)\n{\n\t/* FAKEMATCH: void function called as returning s32. */\n\t((s32 (*)())Late)(a, 1);\n\tLate(a, 2);\n\tPart(a);\n}\nvoid Late(s32 a, s32 b)\n{\n}\nvoid After(s32 a)\n{\n\t((s32 (*)())Late)(a, 3);\n\t((s32 (*)())Shared)(a);\n}\nvoid Shared(s32 a)\n{\n}\n#include \"f.c\"\n";
        let files = [
            (UNIT, unit),
            (PART, "void Part(s32 a)\n{\n}\n"),
            (OTHER, "void Scene(void)\n{\n\tShared(1);\n}\n"),
            (HEADER, "void Run(s32 a);\nvoid Late(s32 a, s32 b);\nvoid After(s32 a);\nvoid Shared(s32 a);\n"),
            (PART_HEADER, "/* Part is called before its definition with no prototype in scope. */\n"),
        ];
        let model = Model::of(&files, &[(UNIT, &[PART, HEADER, PART_HEADER]), (OTHER, &[])]);
        let headers = [HEADER, PART_HEADER].iter().map(|header| (header.to_string(), model.text[*header].clone())).collect();
        let names = ["Run", "Late", "After", "Shared", "Part", "Scene"];
        let written = [UNIT, PART, OTHER].iter().map(|file| (file.to_string(), sites(&model.text[*file], &|name| names.contains(&name)))).collect();
        let used = |file: &str, line: usize, name: &str, defined: bool| (file.to_string(), line, name.to_string(), defined);
        let own = Seen {
            named: ["Late", "Shared", "Part"].iter().map(|name| name.to_string()).collect(),
            uses: vec![used(UNIT, 5, "Late", false), used(UNIT, 6, "Late", false), used(UNIT, 7, "Part", false), used(UNIT, 14, "Late", true), used(UNIT, 15, "Shared", false)],
        };
        let other = Seen { named: BTreeSet::from(["Shared".to_string()]), uses: vec![used(OTHER, 3, "Shared", false)] };
        let seen = BTreeMap::from([(UNIT.to_string(), vec![own]), (OTHER.to_string(), vec![other])]);
        Sample { model, headers, written, seen }
    }

    #[test]
    fn a_candidate_is_called_under_another_shape_above_its_definition() {
        let Sample { model, .. } = &sample();
        let found = sample().candidates();
        let told: Vec<String> = found.iter().map(|c| format!("{} of {}: {} under another shape, {} plain, out {}, {:?}", c.name, c.home, c.count(), c.plain, c.out, c.refused)).collect();
        // Late's cast below its definition is no call above it; the other
        // unit means the main image's Shared; Part is left out already.
        assert_eq!(told, [
            format!("Late of {UNIT}: 1 under another shape, 1 plain, out false, None"),
            format!("Part of {PART}: 0 under another shape, 1 plain, out true, None"),
            format!("Shared of {UNIT}: 1 under another shape, 0 plain, out false, Some(\"{OTHER} names it too\")"),
        ]);
        assert_eq!(found[0].header, HEADER);
        assert_eq!(found[0].sites[UNIT][0].at, model.text[UNIT].find("Late").unwrap());
        assert_eq!(reached(model, &found[1]).into_iter().collect::<Vec<_>>(), [UNIT]);
    }

    #[test]
    fn the_changes_leave_a_function_out_or_declare_it_again() {
        let Sample { model, headers, .. } = &sample();
        let found = sample().candidates();
        let (new, tags) = changes(model, headers, &[(&found[0], &Verdict::Undeclared), (&found[1], &Verdict::Declared), (&found[2], &Verdict::Kept(String::new()))], true);
        assert_eq!(tags, 1);
        assert_eq!(new.keys().map(String::as_str).collect::<Vec<_>>(), [HEADER, PART_HEADER, UNIT]);
        assert_eq!(new[HEADER], "void Run(s32 a);\n/* Late is called before its definition with no prototype in scope. */\nvoid After(s32 a);\nvoid Shared(s32 a);\n");
        assert_eq!(new[PART_HEADER], "void Part(s32 a);\n");
        // The call above the definition is plain and its tag is gone; the
        // cast below the definition and the refused function's stay.
        assert_eq!(new[UNIT], model.text[UNIT].replace("\t/* FAKEMATCH: void function called as returning s32. */\n\t((s32 (*)())Late)(a, 1);", "\tLate(a, 1);"));
        // The trial of a spare cast takes the cast out and leaves the header.
        let (spare, _) = changes(model, headers, &[(&found[0], &Verdict::Spare)], false);
        assert_eq!(spare.keys().map(String::as_str).collect::<Vec<_>>(), [UNIT]);
        assert!(spare[UNIT].contains("FAKEMATCH: void function called as returning s32. */\n\tLate(a, 1);"));
    }
}
