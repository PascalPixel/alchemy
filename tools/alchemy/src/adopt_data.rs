use crate::compiler::routing::root;
use std::path::{Path, PathBuf};

const USAGE: &str = r#"usage: alchemy adopt-data OBJECT PIECE... [--sections ".rodata .data"]
       alchemy adopt-data OBJECT --tail SIZE
                         [--game tbs|tla] [--only LANG,...] [--dry-run]
After a data block's source is in its module: takes the block out of every
edition's scaffold (recon/<game>/unidentified.s for English,
recon/<game>/<lang>/rom.s for the others) and links OBJECT in its place in
that edition's linker script (games/<GAME>/MAIN.LD for Japanese,
MAIN_<LANG>.LD beside it for ☀️, recon/tla/<lang>/MAIN.LD for ⚓️).

OBJECT is the module's object or source, games/<GAME>/SRC/.../X.o (or .S
or .C); --sections lists its input sections, .rodata by default. A PIECE
names scaffold bytes by their labels:
  LABEL         from LABEL to the next label or the end of its section
  FIRST..LAST   from FIRST through LAST's block
  LABEL+SIZE    SIZE bytes from LABEL (decimal or 0x hexadecimal)
Several pieces must follow each other without a gap in one section; they
become one block. A section holding more than the block is split: the bytes
before it keep the section, the bytes after it become a new section named
after their first byte, each with its labels, and the linker script links
before, OBJECT, after where the section was. Labels inside the block (and
any other label on its first byte) leave the scaffold: OBJECT must define
them. The tool reads no ROM; it works only from the scaffolds' labels.

An edition whose scaffold lacks the labels, or whose block differs in size
from English's, keeps its scaffold (S4) and is reported; German, Spanish,
French and Italian adopt only with English, whose script theirs derive
from. --only limits the editions (ja,en,de,es,fr,it), and an edition named
there must adopt. Nothing is written unless every adopting edition can be
cut and relinked; --dry-run writes nothing. Then build all twelve.

--tail SIZE: OBJECT, already linked, now builds the SIZE bytes that follow
it (its alignment fill, or the packer's leftover bytes after a compressed
stream). In each edition whose script links a scaffold section right after
OBJECT, the first SIZE bytes of that section leave the scaffold; no label
may lie inside them. An edition where none follows, or whose section is
shorter or labelled inside the tail, keeps its scaffold and is reported."#;

#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) enum Piece {
    Label(String),
    Through(String, String),
    Sized(String, u32),
}

impl Piece {
    pub(crate) fn parse(text: &str) -> Result<Piece, String> {
        let named = |name: &str| {
            (!name.is_empty()
                && name.bytes().all(|b| b.is_ascii_alphanumeric() || b == b'_' || b == b'.')
                && !name.as_bytes()[0].is_ascii_digit())
            .then(|| name.to_string())
            .ok_or_else(|| format!("{name:?} is not a label"))
        };
        if let Some((first, last)) = text.split_once("..") {
            return Ok(Piece::Through(named(first)?, named(last)?));
        }
        if let Some((label, size)) = text.split_once('+') {
            let size = match size.strip_prefix("0x") {
                Some(hex) => u32::from_str_radix(hex, 16),
                None => size.parse(),
            }
            .map_err(|_| format!("{size:?} is not a size"))?;
            if size == 0 {
                return Err(format!("{text}: a block has bytes"));
            }
            return Ok(Piece::Sized(named(label)?, size));
        }
        Ok(Piece::Label(named(text)?))
    }

    fn first(&self) -> &str {
        match self {
            Piece::Label(label) | Piece::Through(label, _) | Piece::Sized(label, _) => label,
        }
    }
}

/// One line of a scaffold section's body, at the byte offset where it sits.
#[derive(Debug, Clone)]
enum Line {
    Bytes { file: u32, size: u32 },
    Declaration { text: String, label: Option<String> },
}

#[derive(Debug, Clone)]
struct Section {
    /// The header up to the name's key, ".rom." or ".unidentified.".
    prefix: String,
    name: String,
    flags: String,
    /// Each line with the offset it starts at.
    lines: Vec<(u32, Line)>,
    size: u32,
}

impl Section {
    fn labels(&self) -> impl Iterator<Item = (u32, &str)> {
        self.lines.iter().filter_map(|(offset, line)| match line {
            Line::Declaration { label: Some(label), .. } => Some((*offset, label.as_str())),
            _ => None,
        })
    }

    fn label(&self, name: &str) -> Option<u32> {
        self.labels().find(|(_, label)| *label == name).map(|(offset, _)| offset)
    }

    fn next_label(&self, offset: u32) -> u32 {
        self.labels()
            .map(|(at, _)| at)
            .filter(|&at| at > offset)
            .min()
            .unwrap_or(self.size)
    }

    fn header(&self, name: &str) -> String {
        format!("{}{name}{}", self.prefix, self.flags)
    }
}

fn incbin(line: &str) -> Option<(u32, u32)> {
    let (offset, size) = line
        .strip_prefix("\t.incbin \"baserom.gba\", 0x")?
        .split_once(", 0x")?;
    Some((u32::from_str_radix(offset, 16).ok()?, u32::from_str_radix(size.trim(), 16).ok()?))
}

fn label_of(line: &str) -> Option<&str> {
    let name = line.strip_suffix(':')?;
    (!name.is_empty() && !line.starts_with(['\t', ' ', '@', '.'])).then_some(name)
}

fn section_header(line: &str) -> Option<(String, String, String)> {
    let rest = line.strip_prefix("\t.section ")?;
    let (prefix, tail) = [".rom.", ".unidentified."]
        .iter()
        .find_map(|prefix| rest.strip_prefix(prefix).map(|tail| (*prefix, tail)))?;
    let end = tail.find(|c: char| !c.is_ascii_hexdigit()).unwrap_or(tail.len());
    (end == 8).then(|| {
        (format!("\t.section {prefix}"), tail[..end].to_string(), tail[end..].to_string())
    })
}

/// A scaffold file: whatever comes before its first section, then sections.
fn sections(text: &str) -> Result<(Vec<String>, Vec<Section>), String> {
    let mut head = Vec::new();
    let mut found: Vec<Section> = Vec::new();
    let mut pending: Vec<String> = Vec::new();
    for line in text.lines() {
        if let Some((prefix, name, flags)) = section_header(line) {
            if let Some(last) = found.last_mut() {
                for text in pending.drain(..) {
                    last.lines.push((last.size, Line::Declaration { text, label: None }));
                }
            }
            found.push(Section { prefix, name, flags, lines: Vec::new(), size: 0 });
            continue;
        }
        let Some(section) = found.last_mut() else {
            head.push(line.to_string());
            continue;
        };
        if let Some((file, size)) = incbin(line) {
            for text in pending.drain(..) {
                section.lines.push((section.size, Line::Declaration { text, label: None }));
            }
            section.lines.push((section.size, Line::Bytes { file, size }));
            section.size += size;
        } else if line.trim_start().starts_with(".incbin") {
            return Err(format!(
                "section {} holds {line:?}, which is not a base-ROM range",
                section.name
            ));
        } else if let Some(label) = label_of(line) {
            // A label's .global, .type and .thumb_func lines go with it.
            for text in pending.drain(..) {
                section.lines.push((
                    section.size,
                    Line::Declaration { text, label: None },
                ));
            }
            section.lines.push((
                section.size,
                Line::Declaration { text: line.to_string(), label: Some(label.to_string()) },
            ));
        } else {
            pending.push(line.to_string());
        }
    }
    if let Some(last) = found.last_mut() {
        for text in pending.drain(..) {
            last.lines.push((last.size, Line::Declaration { text, label: None }));
        }
    } else {
        head.extend(pending);
    }
    Ok((head, found))
}

/// The declaration lines that belong to the label declared at `index`:
/// the run of unlabelled declarations right before it at the same offset.
fn owned_by_next_label(lines: &[(u32, Line)]) -> Vec<Option<usize>> {
    let mut owner = vec![None; lines.len()];
    let mut run: Vec<usize> = Vec::new();
    for (index, (_, line)) in lines.iter().enumerate() {
        match line {
            Line::Declaration { label: None, .. } => run.push(index),
            Line::Declaration { label: Some(_), .. } => {
                for &member in &run {
                    owner[member] = Some(index);
                }
                owner[index] = Some(index);
                run.clear();
            }
            Line::Bytes { .. } => run.clear(),
        }
    }
    owner
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct Cut {
    pub text: String,
    pub section: String,
    pub before: bool,
    pub after: Option<String>,
    pub size: u32,
    pub labels: Vec<String>,
}

/// Takes the block the pieces name out of one edition's scaffold.
pub(crate) fn cut(text: &str, pieces: &[Piece]) -> Result<Cut, String> {
    let (head, all) = sections(text)?;
    let mut ranges = Vec::new();
    let mut holder = None;
    for piece in pieces {
        let first = piece.first();
        let found: Vec<usize> = all
            .iter()
            .enumerate()
            .filter(|(_, section)| section.label(first).is_some())
            .map(|(index, _)| index)
            .collect();
        let [index] = found[..] else {
            return Err(if found.is_empty() {
                format!("no section labels {first}")
            } else {
                format!("{first} is labelled in {} sections", found.len())
            });
        };
        if holder.is_some_and(|holder| holder != index) {
            return Err("the pieces lie in more than one section; adopt each section's block on its own".into());
        }
        holder = Some(index);
        let section = &all[index];
        let start = section.label(first).expect("found above");
        let end = match piece {
            Piece::Label(_) => section.next_label(start),
            Piece::Through(_, last) => {
                let at = section
                    .label(last)
                    .ok_or_else(|| format!("{last} is not in {first}'s section"))?;
                if at < start {
                    return Err(format!("{last} comes before {first}"));
                }
                section.next_label(at)
            }
            Piece::Sized(_, size) => start
                .checked_add(*size)
                .filter(|&end| end <= section.size)
                .ok_or_else(|| format!("{first}+{size:#x} runs past its section's end"))?,
        };
        if end == start {
            return Err(format!("{first} labels no bytes"));
        }
        ranges.push((start, end));
    }
    let index = holder.ok_or("name at least one piece")?;
    ranges.sort();
    for pair in ranges.windows(2) {
        if pair[0].1 != pair[1].0 {
            return Err(format!(
                "the pieces leave a gap or overlap at {:#x} in their section",
                pair[0].1
            ));
        }
    }
    let (start, end) = (ranges[0].0, ranges[ranges.len() - 1].1);
    cut_range(head, all, index, start, end)
}

/// Takes the first `size` bytes of the scaffold section `name`: the tail an
/// object linked right before it now builds itself (its alignment fill or
/// the packer's leftover bytes). No label may sit inside them.
pub(crate) fn cut_head(text: &str, name: &str, size: u32) -> Result<Cut, String> {
    let (head, all) = sections(text)?;
    let index = all
        .iter()
        .position(|section| section.name == name)
        .ok_or_else(|| format!("no section {name}"))?;
    let section = &all[index];
    if size == 0 || size > section.size {
        return Err(format!("section {name} holds {:#x} bytes, not a {size:#x}-byte tail", section.size));
    }
    if let Some((_, label)) = section.labels().find(|(offset, _)| *offset < size) {
        return Err(format!("{label} lies inside the tail"));
    }
    cut_range(head, all, index, 0, size)
}

fn cut_range(head: Vec<String>, mut all: Vec<Section>, index: usize, start: u32, end: u32) -> Result<Cut, String> {
    let section = all[index].clone();
    let owner = owned_by_next_label(&section.lines);
    let has_after = end < section.size;
    let place = |at: usize| -> u32 { section.lines[at].0 };
    let mut before = Vec::new();
    let mut after = Vec::new();
    let mut labels = Vec::new();
    for (at, (offset, line)) in section.lines.iter().enumerate() {
        match line {
            Line::Bytes { file, size } => {
                let (low, high) = (*offset, offset + size);
                if low < start {
                    before.push(range_line(*file, start.min(high) - low));
                }
                if high > end {
                    let from = end.max(low);
                    after.push(range_line(file + (from - low), high - from));
                }
            }
            Line::Declaration { text, label } => {
                let at_offset = owner[at].map_or(*offset, place);
                let inside = at_offset >= start && at_offset < end;
                if inside {
                    if let Some(label) = label {
                        labels.push(label.clone());
                    }
                } else if at_offset < start || !has_after {
                    before.push(text.clone());
                } else {
                    after.push(text.clone());
                }
            }
        }
    }
    let first_byte = section.lines.iter().find_map(|(offset, line)| match line {
        Line::Bytes { file, size } if offset + size > end => Some(file + (end.max(*offset) - offset)),
        _ => None,
    });
    let base = if section.prefix.ends_with(".unidentified.") { 0x0800_0000 } else { 0 };
    let after_name = has_after.then(|| {
        format!("{:08x}", base + first_byte.expect("bytes follow the block"))
    });
    let keep_before = start > 0;
    if let Some(name) = &after_name {
        let clash = all
            .iter()
            .any(|other| other.name == *name && !(other.name == section.name && !keep_before));
        if clash {
            return Err(format!("the bytes after the block would be section {name}, which exists"));
        }
    }
    let mut out = head;
    for (at, other) in all.drain(..).enumerate() {
        if at != index {
            out.push(other.header(&other.name));
            out.extend(render(&other));
            continue;
        }
        if keep_before {
            out.push(section.header(&section.name));
            out.extend(before.iter().cloned());
        }
        if let Some(name) = &after_name {
            out.push(section.header(name));
            out.extend(after.iter().cloned());
        }
    }
    Ok(Cut {
        text: out.join("\n") + "\n",
        section: section.name,
        before: keep_before,
        after: after_name,
        size: end - start,
        labels,
    })
}

fn range_line(file: u32, size: u32) -> String {
    format!("\t.incbin \"baserom.gba\", 0x{file:08x}, 0x{size:08x}")
}

fn render(section: &Section) -> Vec<String> {
    section
        .lines
        .iter()
        .map(|(_, line)| match line {
            Line::Bytes { file, size } => range_line(*file, *size),
            Line::Declaration { text, .. } => text.clone(),
        })
        .collect()
}

/// Links `object` where the cut section was: the bytes before it, the
/// object, then the bytes after it.
pub(crate) fn relink(script: &str, scaffold: &str, prefix: &str, cut: &Cut, object: &str) -> Result<String, String> {
    let link = |name: &str| format!("\"*/{scaffold}\"({prefix}{name})");
    let old = link(&cut.section);
    let lines: Vec<&str> = script.lines().collect();
    let at: Vec<usize> = lines
        .iter()
        .enumerate()
        .filter(|(_, line)| line.trim() == old)
        .map(|(at, _)| at)
        .collect();
    let [at] = at[..] else {
        return Err(format!("{} lines link {old}", at.len()));
    };
    let path = object.split('(').next().unwrap_or(object);
    if lines.iter().any(|line| line.contains(path)) {
        return Err(format!("{path} is already linked"));
    }
    let indent = &lines[at][..lines[at].len() - lines[at].trim_start().len()];
    let mut out: Vec<String> = lines[..at].iter().map(|line| line.to_string()).collect();
    if cut.before {
        out.push(format!("{indent}{old}"));
    }
    out.push(format!("{indent}{object}"));
    if let Some(name) = &cut.after {
        out.push(format!("{indent}{}", link(name)));
    }
    out.extend(lines[at + 1..].iter().map(|line| line.to_string()));
    Ok(out.join("\n") + "\n")
}

/// The scaffold section a script links right after `object`.
pub(crate) fn following_section(script: &str, scaffold: &str, prefix: &str, object: &str) -> Result<String, String> {
    let path = object.split('(').next().unwrap_or(object);
    let lines: Vec<&str> = script.lines().collect();
    let at: Vec<usize> = lines
        .iter()
        .enumerate()
        .filter(|(_, line)| line.trim().starts_with(path))
        .map(|(at, _)| at)
        .collect();
    let [at] = at[..] else {
        return Err(match at.len() {
            0 => "its script does not link the object".into(),
            count => format!("{count} lines link the object"),
        });
    };
    let link = format!("\"*/{scaffold}\"({prefix}");
    lines
        .get(at + 1)
        .and_then(|line| line.trim().strip_prefix(&link))
        .and_then(|rest| rest.strip_suffix(')'))
        .map(str::to_string)
        .ok_or_else(|| "no scaffold section follows the object".to_string())
}

/// Links what is left of a section whose head the object before it took.
pub(crate) fn relink_tail(script: &str, scaffold: &str, prefix: &str, cut: &Cut) -> Result<String, String> {
    let link = |name: &str| format!("\"*/{scaffold}\"({prefix}{name})");
    let old = link(&cut.section);
    let mut found = 0;
    let mut out = Vec::new();
    for line in script.lines() {
        if line.trim() != old {
            out.push(line.to_string());
            continue;
        }
        found += 1;
        if let Some(name) = &cut.after {
            let indent = &line[..line.len() - line.trim_start().len()];
            out.push(format!("{indent}{}", link(name)));
        }
    }
    if found != 1 {
        return Err(format!("{found} lines link {old}"));
    }
    Ok(out.join("\n") + "\n")
}

/// Decides every edition for a tail: the first `size` bytes of the scaffold
/// section each script links right after the object.
pub(crate) fn plan_tail(
    editions: &[(&str, &str, &str, &str, &str)],
    object: &str,
    size: u32,
    required: &[&str],
) -> Result<Vec<(String, Outcome)>, String> {
    let mut outcomes = Vec::new();
    for &(lang, scaffold_path, prefix, scaffold, script) in editions {
        let outcome = following_section(script, scaffold_path, prefix, object)
            .and_then(|name| cut_head(scaffold, &name, size))
            .and_then(|cut| relink_tail(script, scaffold_path, prefix, &cut).map(|text| (cut, text)));
        let outcome = match outcome {
            Ok((cut, text)) => Outcome::Adopted(cut, text),
            Err(reason) => Outcome::Kept(reason),
        };
        if let Outcome::Kept(reason) = &outcome {
            if required.contains(&lang) {
                return Err(format!("{lang}: {reason}"));
            }
        }
        outcomes.push((lang.to_string(), outcome));
    }
    if !outcomes.iter().any(|(_, outcome)| matches!(outcome, Outcome::Adopted(..))) {
        let reasons: Vec<String> = outcomes
            .iter()
            .map(|(lang, outcome)| match outcome {
                Outcome::Kept(reason) => format!("{lang}: {reason}"),
                Outcome::Adopted(..) => unreachable!(),
            })
            .collect();
        return Err(format!("no edition can take the tail\n{}", reasons.join("\n")));
    }
    outcomes.sort_by_key(|(lang, _)| LANGUAGES.iter().position(|l| l == lang));
    Ok(outcomes)
}

const LANGUAGES: [&str; 6] = ["ja", "en", "de", "es", "fr", "it"];

#[derive(Debug, PartialEq, Eq)]
pub(crate) enum Outcome {
    Adopted(Cut, String),
    Kept(String),
}

/// Decides every edition from its scaffold and script texts.
pub(crate) fn plan(
    editions: &[(&str, &str, &str, &str, &str)],
    pieces: &[Piece],
    object: &str,
    required: &[&str],
    english_links: bool,
) -> Result<Vec<(String, Outcome)>, String> {
    let mut outcomes: Vec<(String, Outcome)> = Vec::new();
    let mut english = None;
    let order = ["en", "ja", "de", "es", "fr", "it"];
    for lang in order {
        let Some(&(_, scaffold_path, prefix, scaffold, script)) =
            editions.iter().find(|edition| edition.0 == lang)
        else {
            continue;
        };
        let international = !matches!(lang, "ja" | "en");
        let outcome = match cut(scaffold, pieces) {
            Err(error) => Outcome::Kept(error),
            Ok(_) if international && english.is_none() && !english_links => Outcome::Kept(
                "English does not link the object, and this edition's script derives from English's".into(),
            ),
            Ok(cut) if english.is_some_and(|size| size != cut.size) => Outcome::Kept(format!(
                "its block is {:#x} bytes, English's {:#x}",
                cut.size,
                english.unwrap()
            )),
            Ok(cut) => match relink(script, scaffold_path, prefix, &cut, object) {
                Ok(text) => Outcome::Adopted(cut, text),
                Err(error) => Outcome::Kept(error),
            },
        };
        if lang == "en" {
            if let Outcome::Adopted(cut, _) = &outcome {
                english = Some(cut.size);
            }
        }
        outcomes.push((lang.to_string(), outcome));
    }
    for (lang, outcome) in &outcomes {
        if let Outcome::Kept(reason) = outcome {
            if required.contains(&lang.as_str()) {
                return Err(format!("{lang}: {reason}"));
            }
        }
    }
    if !outcomes.iter().any(|(_, outcome)| matches!(outcome, Outcome::Adopted(..))) {
        let reasons: Vec<String> = outcomes
            .iter()
            .map(|(lang, outcome)| match outcome {
                Outcome::Kept(reason) => format!("{lang}: {reason}"),
                Outcome::Adopted(..) => unreachable!(),
            })
            .collect();
        return Err(format!("no edition can adopt the block\n{}", reasons.join("\n")));
    }
    outcomes.sort_by_key(|(lang, _)| LANGUAGES.iter().position(|l| l == lang));
    Ok(outcomes)
}

fn scaffold_of(game: &str, lang: &str) -> (PathBuf, &'static str) {
    if lang == "en" {
        ([ "recon", game, "unidentified.s"].iter().collect(), ".unidentified.")
    } else {
        (["recon", game, lang, "rom.s"].iter().collect(), ".rom.")
    }
}

fn object_line(root: &Path, object: &str, sections: &str) -> Result<String, String> {
    let path = Path::new(object);
    let stem = path.with_extension("");
    if !object.starts_with("games/") || object.contains("..") || object.contains('"') {
        return Err(format!("{object} is not a games/ object"));
    }
    if !["S", "s", "C", "c"]
        .iter()
        .any(|extension| root.join(stem.with_extension(extension)).is_file())
    {
        return Err(format!("{object} has no source (.S or .C) to build it from"));
    }
    let sections = sections.split_whitespace().collect::<Vec<_>>().join(" ");
    if sections.is_empty() || !sections.split(' ').all(|section| section.starts_with('.')) {
        return Err(format!("{sections:?} lists no input sections"));
    }
    Ok(format!("\"*/{}.o\"({sections})", stem.display()))
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let (mut game, mut sections, mut only, mut dry) = ("tbs", ".rodata".to_string(), None, false);
    let mut tail = None;
    let mut positional = Vec::new();
    let mut i = 0;
    while i < arguments.len() {
        match arguments[i].as_str() {
            "--game" => {
                i += 1;
                game = match arguments.get(i).map(String::as_str) {
                    Some("tbs") => "tbs",
                    Some("tla") => "tla",
                    _ => return Err(USAGE.into()),
                };
            }
            "--sections" => {
                i += 1;
                sections = arguments.get(i).ok_or_else(|| USAGE.to_string())?.clone();
            }
            "--only" => {
                i += 1;
                let list = arguments.get(i).ok_or_else(|| USAGE.to_string())?;
                let langs: Vec<String> = list.split(',').map(str::to_string).collect();
                if langs.iter().any(|lang| !LANGUAGES.contains(&lang.as_str())) {
                    return Err(USAGE.into());
                }
                only = Some(langs);
            }
            "--dry-run" => dry = true,
            "--tail" => {
                i += 1;
                let size = arguments.get(i).ok_or_else(|| USAGE.to_string())?;
                tail = Some(
                    match size.strip_prefix("0x") {
                        Some(hex) => u32::from_str_radix(hex, 16),
                        None => size.parse(),
                    }
                    .ok()
                    .filter(|&size| size > 0)
                    .ok_or_else(|| format!("{size:?} is not a size"))?,
                );
            }
            "-h" | "--help" => return Err(USAGE.into()),
            value => positional.push(value.to_string()),
        }
        i += 1;
    }
    let [object, pieces @ ..] = positional.as_slice() else {
        return Err(USAGE.into());
    };
    if pieces.is_empty() == tail.is_none() {
        return Err(USAGE.into());
    }
    let pieces = pieces.iter().map(|piece| Piece::parse(piece)).collect::<Result<Vec<_>, _>>()?;
    let root = root();
    let line = object_line(root, object, &sections)?;
    let langs: Vec<&str> = match &only {
        Some(langs) => LANGUAGES.iter().copied().filter(|lang| langs.iter().any(|l| l == lang)).collect(),
        None => LANGUAGES.to_vec(),
    };
    let mut texts = Vec::new();
    for lang in &langs {
        let (scaffold, prefix) = scaffold_of(game, lang);
        let script = crate::targets::edition_script_path(game, lang);
        let read = |path: &Path| {
            std::fs::read_to_string(root.join(path)).map_err(|e| format!("{}: {e}", path.display()))
        };
        texts.push((*lang, scaffold.clone(), prefix, read(&scaffold)?, script.clone(), read(&script)?));
    }
    let scaffold_objects: Vec<String> = texts
        .iter()
        .map(|t| t.1.with_extension("o").to_string_lossy().into_owned())
        .collect();
    let editions: Vec<(&str, &str, &str, &str, &str)> = texts
        .iter()
        .zip(&scaffold_objects)
        .map(|(t, object)| (t.0, object.as_str(), t.2, t.3.as_str(), t.5.as_str()))
        .collect();
    let required: Vec<&str> = only.iter().flatten().map(String::as_str).collect();
    let english_script = crate::targets::edition_script_path(game, "en");
    let linked = line.split('(').next().unwrap_or(&line);
    let english_links = std::fs::read_to_string(root.join(&english_script))
        .map_err(|e| format!("{}: {e}", english_script.display()))?
        .contains(linked);
    let outcomes = match tail {
        Some(size) => plan_tail(&editions, &line, size, &required)?,
        None => plan(&editions, &pieces, &line, &required, english_links)?,
    };
    let mut writes = Vec::new();
    let mut must_define = std::collections::BTreeSet::new();
    for (lang, outcome) in &outcomes {
        match outcome {
            Outcome::Adopted(cut, script) => {
                let t = texts.iter().find(|t| t.0 == lang).expect("planned editions were read");
                println!(
                    "{game}-{lang}: adopted {:#x} bytes of {}{}{}",
                    cut.size,
                    t.2,
                    cut.section,
                    cut.after.as_ref().map_or(String::new(), |name| format!(", the rest now {}{name}", t.2))
                );
                must_define.extend(cut.labels.iter().cloned());
                writes.push((t.1.clone(), cut.text.clone()));
                writes.push((t.4.clone(), script.clone()));
            }
            Outcome::Kept(reason) => println!("{game}-{lang}: kept on its scaffold: {reason}"),
        }
    }
    if !must_define.is_empty() {
        println!(
            "{object} must define: {}",
            must_define.into_iter().collect::<Vec<_>>().join(" ")
        );
    }
    if dry {
        println!("dry run: nothing written");
        return Ok(());
    }
    for (path, text) in writes {
        std::fs::write(root.join(&path), text).map_err(|e| format!("{}: {e}", path.display()))?;
    }
    println!("build all twelve editions to confirm (make compare-editions)");
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    const ENGLISH: &str = "\t.section .unidentified.08000100,\"a\"\n\t.global A\nA:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n\t.global B\n\t.type B, %object\nB:\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000008\n\t.incbin \"baserom.gba\", 0x00000118, 0x00000008\n\t.global C\nC:\n\t.incbin \"baserom.gba\", 0x00000120, 0x00000020\n\t.section .unidentified.08000200,\"a\"\n\t.global D\nD:\n\t.incbin \"baserom.gba\", 0x00000200, 0x00000004\n";
    const SCRIPT: &str = ".rom_main :\n{\n        \"*/games/G/X.o\"(.text)\n        \"*/recon/tbs/unidentified.o\"(.unidentified.08000100)\n        \"*/games/G/Y.o\"(.text)\n        \"*/recon/tbs/unidentified.o\"(.unidentified.08000200)\n}\n";
    const OBJECT: &str = "\"*/games/G/SRC/BATTLE/B.o\"(.rodata)";

    fn piece(text: &str) -> Piece {
        Piece::parse(text).unwrap()
    }

    #[test]
    fn a_block_in_the_middle_splits_its_section() {
        let cut = cut(ENGLISH, &[piece("B")]).unwrap();
        assert_eq!(cut.size, 0x10);
        assert_eq!(cut.labels, ["B"]);
        assert_eq!(cut.section, "08000100");
        assert_eq!(cut.after.as_deref(), Some("08000120"));
        assert_eq!(
            cut.text,
            "\t.section .unidentified.08000100,\"a\"\n\t.global A\nA:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n\t.section .unidentified.08000120,\"a\"\n\t.global C\nC:\n\t.incbin \"baserom.gba\", 0x00000120, 0x00000020\n\t.section .unidentified.08000200,\"a\"\n\t.global D\nD:\n\t.incbin \"baserom.gba\", 0x00000200, 0x00000004\n"
        );
        let script = relink(SCRIPT, "recon/tbs/unidentified.o", ".unidentified.", &cut, OBJECT).unwrap();
        assert_eq!(
            script,
            ".rom_main :\n{\n        \"*/games/G/X.o\"(.text)\n        \"*/recon/tbs/unidentified.o\"(.unidentified.08000100)\n        \"*/games/G/SRC/BATTLE/B.o\"(.rodata)\n        \"*/recon/tbs/unidentified.o\"(.unidentified.08000120)\n        \"*/games/G/Y.o\"(.text)\n        \"*/recon/tbs/unidentified.o\"(.unidentified.08000200)\n}\n"
        );
        assert!(relink(&script, "recon/tbs/unidentified.o", ".unidentified.", &cut, OBJECT).is_err());
    }

    #[test]
    fn a_sized_block_may_end_inside_a_range() {
        let cut = cut(ENGLISH, &[piece("A+0xc")]).unwrap();
        assert!(!cut.before);
        assert_eq!(cut.labels, ["A"]);
        assert_eq!(cut.after.as_deref(), Some("0800010c"));
        assert!(cut.text.starts_with("\t.section .unidentified.0800010c,\"a\"\n\t.incbin \"baserom.gba\", 0x0000010c, 0x00000004\n\t.global B\n\t.type B, %object\nB:\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000008\n"), "{}", cut.text);
        let script = relink(SCRIPT, "recon/tbs/unidentified.o", ".unidentified.", &cut, OBJECT).unwrap();
        assert!(script.contains("        \"*/games/G/SRC/BATTLE/B.o\"(.rodata)\n        \"*/recon/tbs/unidentified.o\"(.unidentified.0800010c)\n        \"*/games/G/Y.o\""), "{script}");
        assert!(!script.contains("(.unidentified.08000100)"));
        let across = super::cut(ENGLISH, &[piece("A+0x14")]).unwrap();
        assert_eq!(across.labels, ["A", "B"]);
        assert!(across.text.starts_with("\t.section .unidentified.08000114,\"a\"\n\t.incbin \"baserom.gba\", 0x00000114, 0x00000004\n\t.incbin \"baserom.gba\", 0x00000118, 0x00000008\n\t.global C\n"), "{}", across.text);
    }

    #[test]
    fn ranges_and_listed_pieces_join_into_one_block() {
        let through = cut(ENGLISH, &[piece("A..B")]).unwrap();
        let listed = cut(ENGLISH, &[piece("B"), piece("A")]).unwrap();
        assert_eq!(through, listed);
        assert_eq!(through.size, 0x20);
        let whole = cut(ENGLISH, &[piece("A..C")]).unwrap();
        assert!(!whole.before && whole.after.is_none());
        assert_eq!(whole.labels, ["A", "B", "C"]);
        assert!(!whole.text.contains("08000100"));
        assert!(cut(ENGLISH, &[piece("A"), piece("C")]).unwrap_err().contains("gap"));
        assert!(cut(ENGLISH, &[piece("A"), piece("D")]).unwrap_err().contains("more than one section"));
        assert!(cut(ENGLISH, &[piece("C..A")]).is_err());
        assert!(cut(ENGLISH, &[piece("C+0x21")]).is_err());
        assert!(cut(ENGLISH, &[piece("Missing")]).is_err());
    }

    #[test]
    fn an_object_takes_the_tail_that_follows_it() {
        let object = "\"*/games/G/X.o\"(.text)";
        let pad = "\t.section .unidentified.08000100,\"a\"\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000002\n\t.section .unidentified.08000200,\"a\"\n\t.global D\nD:\n\t.incbin \"baserom.gba\", 0x00000200, 0x00000004\n";
        let scaffold = "recon/tbs/unidentified.o";
        let name = following_section(SCRIPT, scaffold, ".unidentified.", object).unwrap();
        assert_eq!(name, "08000100");
        let whole = cut_head(pad, &name, 2).unwrap();
        assert!(!whole.text.contains("08000100") && whole.after.is_none());
        let script = relink_tail(SCRIPT, scaffold, ".unidentified.", &whole).unwrap();
        assert_eq!(script, SCRIPT.replace("        \"*/recon/tbs/unidentified.o\"(.unidentified.08000100)\n", ""));
        // A German tail that heads a longer section: the rest keeps its labels.
        let german = "\t.section .unidentified.08000100,\"a\"\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n\t.global B\nB:\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000008\n";
        let head = cut_head(german, "08000100", 4).unwrap();
        assert_eq!(head.after.as_deref(), Some("08000104"));
        assert_eq!(head.text, "\t.section .unidentified.08000104,\"a\"\n\t.incbin \"baserom.gba\", 0x00000104, 0x0000000c\n\t.global B\nB:\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000008\n");
        let relinked = relink_tail(SCRIPT, scaffold, ".unidentified.", &head).unwrap();
        assert!(relinked.contains("(.text)\n        \"*/recon/tbs/unidentified.o\"(.unidentified.08000104)\n        \"*/games/G/Y.o\""), "{relinked}");
        assert!(cut_head(german, "08000100", 0x19).is_err());
        assert!(cut_head(ENGLISH, "08000100", 2).unwrap_err().contains("A lies"));
        let editions = [
            ("en", scaffold, ".unidentified.", pad, SCRIPT),
            ("ja", scaffold, ".unidentified.", pad, ".rom_main :\n{\n        \"*/games/G/X.o\"(.text)\n        \"*/games/G/Y.o\"(.text)\n}\n"),
        ];
        let outcomes = plan_tail(&editions, object, 2, &[]).unwrap();
        assert_eq!(outcomes[0].0, "ja");
        assert!(matches!(&outcomes[0].1, Outcome::Kept(reason) if reason.contains("no scaffold section")));
        assert!(matches!(outcomes[1].1, Outcome::Adopted(..)));
        assert!(plan_tail(&editions, object, 2, &["ja"]).is_err());
        assert!(plan_tail(&editions, "\"*/games/G/Z.o\"(.text)", 2, &[]).is_err());
    }

    #[test]
    fn pieces_parse_by_form() {
        assert_eq!(piece("A"), Piece::Label("A".into()));
        assert_eq!(piece("A..B"), Piece::Through("A".into(), "B".into()));
        assert_eq!(piece("A+16"), Piece::Sized("A".into(), 16));
        assert_eq!(piece("A+0x10"), Piece::Sized("A".into(), 16));
        assert!(Piece::parse("A+0").is_err());
        assert!(Piece::parse("0x08000100").is_err());
        assert!(Piece::parse("A+x").is_err());
    }

    #[test]
    fn an_edition_whose_block_differs_keeps_its_scaffold() {
        let german = "\t.section .rom.00000100, \"ax\"\n\t.global A\nA:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n\t.global B\nB:\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000014\n\t.global C\nC:\n\t.incbin \"baserom.gba\", 0x00000124, 0x00000020\n";
        let french = german.replace("0x00000014", "0x00000010").replace("0x00000124", "0x00000120");
        let script = |lang: &str| SCRIPT.replace("recon/tbs/unidentified.o\"(.unidentified.08000100)", &format!("recon/tbs/{lang}/rom.o\"(.rom.00000100)"));
        let (de, fr) = (script("de"), script("fr"));
        let editions = [
            ("en", "recon/tbs/unidentified.o", ".unidentified.", ENGLISH, SCRIPT),
            ("de", "recon/tbs/de/rom.o", ".rom.", german, de.as_str()),
            ("fr", "recon/tbs/fr/rom.o", ".rom.", french.as_str(), fr.as_str()),
        ];
        let outcomes = plan(&editions, &[piece("B")], OBJECT, &[], false).unwrap();
        let langs: Vec<&str> = outcomes.iter().map(|(lang, _)| lang.as_str()).collect();
        assert_eq!(langs, ["en", "de", "fr"]);
        assert!(matches!(&outcomes[1].1, Outcome::Kept(reason) if reason.contains("0x14")));
        let Outcome::Adopted(cut, text) = &outcomes[2].1 else { panic!("{:?}", outcomes[2]) };
        assert_eq!(cut.after.as_deref(), Some("00000120"));
        assert!(text.contains("(.rom.00000100)\n        \"*/games/G/SRC/BATTLE/B.o\"(.rodata)\n        \"*/recon/tbs/fr/rom.o\"(.rom.00000120)"), "{text}");
        assert!(plan(&editions, &[piece("B")], OBJECT, &["de"], false).is_err());
        let without_english = [editions[0], editions[2]];
        let missing = [("en", "recon/tbs/unidentified.o", ".unidentified.", "\t.section .unidentified.08000100,\"a\"\n", SCRIPT), editions[2]];
        let outcomes = plan(&missing, &[piece("B")], OBJECT, &[], false);
        assert!(outcomes.is_err(), "{outcomes:?}");
        assert!(plan(&without_english, &[piece("B")], OBJECT, &[], false).is_ok());
        let french_only = [editions[2]];
        assert!(plan(&french_only, &[piece("B")], OBJECT, &[], false).is_err());
        let later = plan(&french_only, &[piece("B")], OBJECT, &["fr"], true).unwrap();
        assert!(matches!(later[0].1, Outcome::Adopted(..)));
    }
}
