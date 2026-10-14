//! Ids written as numbers where the code takes an id, named from a header.
//!
//! `alchemy camelotize constants --header H FILE...` reads the names H
//! defines (`#define NAME value`, or enum constants) and replaces an integer
//! literal with the one name of its value, only at a position known to hold
//! an id of the header's kind (POSITIONS): an argument of a listed call or
//! macro, a value stored to a listed member, a listed member of a struct
//! initializer's rows, an element of a listed table, a listed argument of an
//! assembler macro or a column of the data rows under a listed label. A
//! literal anywhere else is never touched, nor a value the position writes
//! for no id (an always-true condition, a placeholder). A per-file alias
//! table (file stem, value, alias) names a value in that file before the
//! header does. What the tool cannot name plainly is printed for naming by
//! hand: an id computed from other values or written as an expression of
//! literals, a value with no name or with two, an id packed beside other
//! bits, a literal no id of the kind can be (a table typed as one it is
//! not) and an assembler file the preprocessor does not read. `alchemy
//! check constants` counts the literal ids left at the same positions.
//!
//! A preprocessed assembler file (`.S`) reads only what the assembler
//! can: it takes a name only from a `#define` outside the header's C-only
//! blocks (`#ifndef __ASSEMBLER__`), and nothing from a header that holds
//! C outside them (an enum), which it cannot include; an alias only where
//! the file itself #defines it. The rest is listed for a hand.
//!
//! Preprocessor lines are not read, so a literal inside a macro body is no
//! site. A name replaces a literal of the same value, which the
//! preprocessor puts back, so the bytes cannot change; `alchemy keep`
//! proves it all the same.

use super::source::{closing, is_ident, mask, split_commas, text, trim};
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::ops::Range;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy camelotize [--dry-run] constants --header H [--kind flag|sound] [--aliases TSV] FILE...\n\
Names the integer literals at the id positions of H's kind with H's\n\
#define (or enum) names: flags.h and flag_ids.h name flags, songs.h,\n\
sounds.h and sound_ids.h sounds; --kind gives another header's kind. The\n\
TSV's rows are a file stem, a value and an alias that names that value in\n\
that file before H does. A .S file takes only names the assembler reads:\n\
H's #defines outside #ifndef __ASSEMBLER__, none from an H that holds C\n\
outside it (an enum), and aliases the file #defines. Prints each site to\n\
name by hand (computed, an expression of literals, no name, two names,\n\
packed beside other bits, no id of the kind, a name the assembler cannot\n\
read, or in a .s file the preprocessor does not read), then each changed\n\
file's count, and adds H's #include to a file that names an id from it.";

/// What an id names.
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub(crate) enum Kind {
    Flag,
    Sound,
}

impl Kind {
    pub(crate) const ALL: [Kind; 2] = [Kind::Flag, Kind::Sound];

    pub(crate) fn name(self) -> &'static str {
        match self {
            Kind::Flag => "flag",
            Kind::Sound => "sound",
        }
    }

    fn parse(name: &str) -> Option<Kind> {
        Kind::ALL.into_iter().find(|kind| kind.name() == name)
    }

    /// The values that can be ids of the kind: the flags fill 0x200 bytes
    /// in both games; songs and sounds differ by game.
    fn holds(self, value: i64) -> bool {
        match self {
            Kind::Flag => (0..=0xfff).contains(&value),
            Kind::Sound => true,
        }
    }
}

/// The kind of the ids a header names, by its file stem.
const HEADER_KINDS: [(&str, Kind); 5] = [
    ("flags", Kind::Flag),
    ("flag_ids", Kind::Flag),
    ("songs", Kind::Sound),
    ("sounds", Kind::Sound),
    ("sound_ids", Kind::Sound),
];

/// How the value written at a position holds its id.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) enum Form {
    /// The value is the id.
    Plain,
    /// A scene condition: the flag, which holds while the flag is clear, or
    /// the flag with this bit, which holds while it is set (IF_FLAG_SET).
    Condition(i64),
    /// The id with this marker bit; a value without it is no id.
    Marked(i64),
    /// The id in the bits of this mask, beside another field.
    Packed(i64),
}

/// Where an id is written.
#[derive(Clone, Copy, Debug)]
pub(crate) enum Place {
    /// An argument of a C call or macro, by index; a name ending in `*`
    /// stands for every name it begins. With `when`, only where another
    /// argument is that name or that number.
    Call { name: &'static str, argument: usize, when: Option<(usize, &'static str, i64)> },
    /// The value stored to a member or variable of this name.
    Store { member: &'static str },
    /// A member of each row of a `struct TAG` initializer, by its place in
    /// the row or its designated name.
    Member { tag: &'static str, index: usize, field: &'static str },
    /// Every element of a table's initializer.
    Table { name: &'static str },
    /// An argument of an assembler macro, by index.
    Macro { name: &'static str, argument: usize },
    /// A column of each data row under an assembler label.
    Rows { label: &'static str, column: usize },
}

/// One position that holds an id.
#[derive(Clone, Copy, Debug)]
pub(crate) struct Position {
    pub kind: Kind,
    pub place: Place,
    pub form: Form,
    /// Values written here that are no id: an always-true condition, a
    /// placeholder.
    pub not_ids: &'static [i64],
}

/// An always-true condition: -1, also written as the word 0xffffffff.
const ALWAYS: &[i64] = &[-1, 0xffff_ffff];
/// A placement condition of 0 is a view placeholder that
/// FieldScene_PrepareActors rewrites to 0x164 or 0x165 (Scene_AssignViewFlags).
const ALWAYS_OR_VIEW: &[i64] = &[-1, 0xffff_ffff, 0];
/// An exit's flag word of 0 is no condition: MapGroupTable_SelectEntry
/// takes the exit when the word is 0 or its flag is clear.
const ALWAYS_OR_NONE: &[i64] = &[-1, 0xffff_ffff, 0];
/// The unused rows and the -1 that end WorldMap_PlaceMarkers.
const EMPTY_ROW: &[i64] = &[0, -1];
/// BattleEventRuntime_BeginPhase queues its cue only when it is not 0.
const NO_CUE: &[i64] = &[0];
/// IF_FLAG_SET's bit: the condition holds while the flag is set.
const FLAG_SET: i64 = 0x1000;

const fn flag(name: &'static str) -> Position {
    Position { kind: Kind::Flag, place: Place::Call { name, argument: 0, when: None }, form: Form::Plain, not_ids: ALWAYS }
}

const fn sound(name: &'static str, argument: usize) -> Position {
    Position { kind: Kind::Sound, place: Place::Call { name, argument, when: None }, form: Form::Plain, not_ids: &[] }
}

const fn store(member: &'static str) -> Position {
    Position { kind: Kind::Sound, place: Place::Store { member }, form: Form::Plain, not_ids: &[] }
}

/// Every position known to hold a flag or sound id. The same lists will
/// take items, abilities, scenes and messages as their headers come.
pub(crate) const POSITIONS: &[Position] = &[
    flag("GameFlag_IsSet"),
    flag("GameFlag_Set"),
    flag("GameFlag_Clear"),
    flag("GameFlag_Test"),
    flag("GameFlag_TestFar"),
    flag("GameFlag_SetBit"),
    flag("GameFlag_SetBitFar"),
    flag("GameFlag_ClearBit"),
    flag("GameFlag_ClearBitFar"),
    flag("GameFlag_Toggle"),
    flag("GameFlag_SetByte"),
    flag("GameFlag_SetByteFar"),
    flag("GameFlag_GetByte"),
    flag("GameFlag_GetByteFar"),
    flag("GameFlag_IncrementByte"),
    flag("GameFlag_DecrementByte"),
    flag("GameFlag_SetNibble"),
    flag("GameFlag_GetNibble"),
    flag("Engine_GameFlagIsSet"),
    flag("Engine_GameFlagSet"),
    flag("Engine_GameFlagClear"),
    flag("FieldEvent_GameFlag*"),
    // engcall.h's forwards to GameFlag_GetByte and GameFlag_SetByte.
    flag("FieldGameFlag_GetByte"),
    flag("FieldGameFlag_SetByte"),
    // event.h's scene commands and objcmd.h's object script commands.
    flag("FLAG_CHK"),
    flag("FLAG_SET"),
    flag("FLAG_CLR"),
    flag("IF_FLAG_SET"),
    flag("SC_FLAG_CHK"),
    flag("SC_FLAG_ON"),
    flag("SC_FLAG_OFF"),
    flag("SC_FLAG_REV"),
    Position {
        kind: Kind::Flag,
        place: Place::Call { name: "GameFlag_IsConditionActive", argument: 0, when: None },
        form: Form::Condition(FLAG_SET),
        not_ids: ALWAYS,
    },
    // field_scene.h: an exit MapGroupTable_SelectEntry takes while its flag
    // is clear (`flag == 0 || GameFlag_TestFar(flag) == 0`).
    Position {
        kind: Kind::Flag,
        place: Place::Call { name: "SCENE_EXIT_IF", argument: 3, when: None },
        form: Form::Plain,
        not_ids: ALWAYS_OR_NONE,
    },
    Position {
        kind: Kind::Flag,
        place: Place::Member { tag: "SceneEvent", index: 2, field: "condition" },
        form: Form::Condition(FLAG_SET),
        not_ids: ALWAYS,
    },
    Position {
        kind: Kind::Flag,
        place: Place::Member { tag: "ScenePlacement", index: 1, field: "condition" },
        form: Form::Condition(FLAG_SET),
        not_ids: ALWAYS_OR_VIEW,
    },
    Position {
        kind: Kind::Flag,
        place: Place::Member { tag: "SceneInteractionEntry", index: 3, field: "condition" },
        form: Form::Plain,
        not_ids: ALWAYS,
    },
    // Scene_InteractionRuleTable's rows: scene or group, entrance, flag (-1
    // always), song; Location's: scene, entrance, flag, shift, zone.
    Position { kind: Kind::Flag, place: Place::Macro { name: "scene_cue", argument: 2 }, form: Form::Plain, not_ids: ALWAYS },
    Position { kind: Kind::Flag, place: Place::Macro { name: "group_cue", argument: 2 }, form: Form::Plain, not_ids: ALWAYS },
    Position { kind: Kind::Flag, place: Place::Macro { name: "location", argument: 2 }, form: Form::Plain, not_ids: ALWAYS },
    // {tile << 16 | flag, place}: a marker shows once its area's flag is set.
    Position {
        kind: Kind::Flag,
        place: Place::Rows { label: "WorldMap_PlaceMarkers", column: 0 },
        form: Form::Packed(0xffff),
        not_ids: EMPTY_ROW,
    },
    sound("Engine_AudioPlayCue", 0),
    sound("AudioCommand_Play", 0),
    sound("AudioCommand_PlayFar", 0),
    sound("Event_PlayCue", 0),
    sound("SE", 0),
    sound("SC_SE", 0),
    sound("Audio_PlaySound", 0),
    sound("Audio_PlayCueReturnOne", 0),
    sound("SetModeAndPlayCue", 2),
    Position {
        kind: Kind::Sound,
        place: Place::Call { name: "BattleEventRuntime_BeginPhase", argument: 0, when: None },
        form: Form::Plain,
        not_ids: NO_CUE,
    },
    Position {
        kind: Kind::Sound,
        place: Place::Call { name: "BattleEventRuntime_BeginPhaseFar", argument: 0, when: None },
        form: Form::Plain,
        not_ids: NO_CUE,
    },
    sound("BattleFx_SetQueuedSoundAndPlay", 0),
    sound("BattleFx_SetQueuedSoundAndPlayFar", 0),
    // BattleFx_SetQueuedSoundAndPlay's names in the scene imports.
    sound("QueueSceneSound", 0),
    sound("Resource37c_QueueSoundCue", 0),
    Position {
        kind: Kind::Sound,
        place: Place::Call { name: "BtlEvPush", argument: 1, when: Some((0, "BEV_SE", 14)) },
        form: Form::Plain,
        not_ids: &[],
    },
    store("pending_cue"),
    store("return_cue"),
    store("queued_sound"),
    Position {
        kind: Kind::Sound,
        place: Place::Member { tag: "SceneInteractionEntry", index: 4, field: "result" },
        form: Form::Plain,
        not_ids: &[],
    },
    Position { kind: Kind::Sound, place: Place::Macro { name: "scene_cue", argument: 3 }, form: Form::Plain, not_ids: &[] },
    Position { kind: Kind::Sound, place: Place::Macro { name: "group_cue", argument: 3 }, form: Form::Plain, not_ids: &[] },
    // 0x8000 | song, then the pairs that choose it, ending in 0.
    Position { kind: Kind::Sound, place: Place::Table { name: "gBattleCueTable" }, form: Form::Marked(0x8000), not_ids: &[] },
    // The Sanctum's party services.
    Position { kind: Kind::Sound, place: Place::Table { name: "Shop_PartyActionCues" }, form: Form::Plain, not_ids: &[] },
];

impl Place {
    /// How a printed site says where it is.
    pub(crate) fn label(&self) -> String {
        match *self {
            Place::Call { name, argument, .. } => format!("{}#{argument}", name.trim_end_matches('*')),
            Place::Store { member } => format!("{member} ="),
            Place::Member { tag, field, .. } => format!("{tag}.{field}"),
            Place::Table { name } => name.to_string(),
            Place::Macro { name, argument } => format!("{name}#{argument}"),
            Place::Rows { label, column } => format!("{label}#{column}"),
        }
    }
}

/// What a position holds, as written.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Held {
    /// A literal that is the id: the literal's span, its sign included.
    Id { literal: Range<usize>, value: i64 },
    /// A literal that holds the id with other bits.
    Encoded { value: i64, id: i64, form: Form },
    /// An expression of literals alone (`148 << 2`): an id the compiler
    /// folds, written as no number names it.
    Constant { value: i64, id: i64, form: Form },
    /// An expression that computes the id.
    Computed,
    /// A literal that cannot be an id of the kind: a table typed as one it
    /// is not.
    Outside { value: i64 },
}

/// An id position as a file writes it.
#[derive(Clone, Debug)]
pub(crate) struct Site {
    pub position: &'static Position,
    /// The written value's span.
    pub span: Range<usize>,
    pub line: usize,
    pub held: Held,
}

impl Site {
    /// A literal id, plain or encoded: what `alchemy check constants` counts.
    pub(crate) fn literal(&self) -> bool {
        matches!(self.held, Held::Id { .. } | Held::Encoded { .. } | Held::Constant { .. })
    }

    /// The id the literal holds.
    pub(crate) fn id(&self) -> Option<i64> {
        match self.held {
            Held::Id { value, .. } => Some(value),
            Held::Encoded { id, .. } | Held::Constant { id, .. } => Some(id),
            Held::Computed | Held::Outside { .. } => None,
        }
    }
}

/// How an expression is written.
enum Written {
    Literal { span: Range<usize>, value: i64 },
    Constant { value: i64 },
    Computed,
    Other,
}

/// An integer literal's value: decimal, hex or octal, with any suffix.
pub(crate) fn number(text: &str) -> Option<i64> {
    let digits = text.trim_end_matches(['u', 'U', 'l', 'L']);
    if !digits.as_bytes().first().is_some_and(u8::is_ascii_digit) {
        return None;
    }
    if let Some(hex) = digits.strip_prefix("0x").or_else(|| digits.strip_prefix("0X")) {
        return i64::from_str_radix(hex, 16).ok();
    }
    if digits.len() > 1 && digits.starts_with('0') {
        return i64::from_str_radix(&digits[1..], 8).ok();
    }
    digits.parse().ok()
}

/// Integer types a cast in front of an id may name.
const CAST_WORDS: &[&str] = &[
    "u8", "s8", "u16", "s16", "u32", "s32", "vu8", "vu16", "vu32", "vs8", "vs16", "vs32", "int", "unsigned", "signed", "char",
    "short", "long", "const", "volatile",
];

fn cast(inner: &str) -> bool {
    let words: Vec<&str> = inner.split_whitespace().collect();
    !words.is_empty() && words.iter().all(|word| CAST_WORDS.contains(word))
}

/// How the expression in `range` of a masked text is written: one integer
/// literal, perhaps signed, in parentheses or behind an integer cast; an
/// expression with an operator outside any bracket; or anything else (a
/// name, a variable, a call).
fn written(mask: &[u8], range: Range<usize>) -> Written {
    let mut range = trim(mask, range);
    while range.start < range.end && mask[range.start] == b'(' {
        let Some(close) = closing(mask, range.start) else {
            return Written::Other;
        };
        if close + 1 == range.end {
            range = trim(mask, range.start + 1..close);
        } else if cast(text(mask, range.start + 1..close)) {
            range = trim(mask, close + 1..range.end);
        } else {
            break;
        }
    }
    if range.is_empty() {
        return Written::Other;
    }
    let mut at = range.start;
    let negative = mask[at] == b'-';
    if matches!(mask[at], b'-' | b'+') {
        at = trim(mask, at + 1..range.end).start;
    }
    if at < range.end && mask[at].is_ascii_digit() && mask[at..range.end].iter().all(|&b| is_ident(b)) {
        if let Some(value) = number(text(mask, at..range.end)) {
            return Written::Literal { span: range.clone(), value: if negative { -value } else { value } };
        }
    }
    let mut depth = 0i32;
    for i in range.clone() {
        match mask[i] {
            b'(' | b'[' | b'{' => depth += 1,
            b')' | b']' | b'}' => depth -= 1,
            b'-' if mask.get(i + 1) == Some(&b'>') => {}
            b'>' if i > 0 && mask[i - 1] == b'-' => {}
            b'+' | b'-' | b'*' | b'/' | b'%' | b'|' | b'&' | b'^' | b'~' | b'<' | b'>' | b'?' if depth == 0 => {
                return match evaluate(text(mask, range), &HashMap::new()) {
                    Some(value) => Written::Constant { value },
                    None => Written::Computed,
                };
            }
            _ => {}
        }
    }
    Written::Other
}

/// What a position holds, given how its value is written; None where it
/// holds no id.
fn held(position: &Position, mask: &[u8], range: &Range<usize>) -> Option<Held> {
    let (literal, value) = match written(mask, range.clone()) {
        Written::Other => return None,
        Written::Computed => return Some(Held::Computed),
        Written::Literal { span, value } => (Some(span), value),
        Written::Constant { value } => (None, value),
    };
    if position.not_ids.contains(&value) {
        return None;
    }
    let id = match position.form {
        Form::Plain => value,
        Form::Condition(bit) => value & !bit,
        Form::Marked(bit) if value & bit == 0 => return None,
        Form::Marked(bit) => value & !bit,
        Form::Packed(bits) if position.not_ids.contains(&(value & bits)) => return None,
        Form::Packed(bits) => value & bits,
    };
    Some(match literal {
        _ if !position.kind.holds(id) => Held::Outside { value },
        None => Held::Constant { value, id, form: position.form },
        Some(literal) if id == value => Held::Id { literal, value },
        Some(_) => Held::Encoded { value, id, form: position.form },
    })
}

/// The start of each line, for line numbers.
fn line_starts(text: &str) -> Vec<usize> {
    std::iter::once(0).chain(text.match_indices('\n').map(|(i, _)| i + 1)).collect()
}

/// Whether a file is assembler: `.s`, or `.S`, which the preprocessor reads.
pub(crate) fn assembler(path: &Path) -> bool {
    matches!(path.extension().and_then(|e| e.to_str()), Some("s" | "S"))
}

/// Every id position of the given kinds in a file, in the order written.
pub(crate) fn sites(path: &Path, text: &str, kinds: &[Kind]) -> Vec<Site> {
    let positions: Vec<&'static Position> = POSITIONS.iter().filter(|p| kinds.contains(&p.kind)).collect();
    let found = if assembler(path) {
        assembler_sites(&assembler_mask(text), &positions)
    } else {
        c_sites(&mask(text), &positions)
    };
    let starts = line_starts(text);
    let mut seen = BTreeSet::new();
    let mut sites: Vec<Site> = found
        .into_iter()
        .filter(|(_, span, _)| seen.insert((span.start, span.end)))
        .map(|(position, span, held)| Site { position, line: starts.partition_point(|&s| s <= span.start), span, held })
        .collect();
    sites.sort_by_key(|site| (site.span.start, site.span.end));
    sites
}

type Found = (&'static Position, Range<usize>, Held);

fn c_sites(mask: &[u8], positions: &[&'static Position]) -> Vec<Found> {
    let mut calls: HashMap<&str, Vec<&'static Position>> = HashMap::new();
    let mut prefixes: Vec<(&str, &'static Position)> = Vec::new();
    let mut stores: HashMap<&str, Vec<&'static Position>> = HashMap::new();
    let mut tables: HashMap<&str, Vec<&'static Position>> = HashMap::new();
    let mut members: HashMap<&str, Vec<&'static Position>> = HashMap::new();
    for &position in positions {
        match position.place {
            Place::Call { name, .. } => match name.strip_suffix('*') {
                Some(prefix) => prefixes.push((prefix, position)),
                None => calls.entry(name).or_default().push(position),
            },
            Place::Store { member } => stores.entry(member).or_default().push(position),
            Place::Table { name } => tables.entry(name).or_default().push(position),
            Place::Member { tag, .. } => members.entry(tag).or_default().push(position),
            Place::Macro { .. } | Place::Rows { .. } => {}
        }
    }
    let mut found = Vec::new();
    let mut at = 0;
    while at < mask.len() {
        if !is_ident(mask[at]) {
            at += 1;
            continue;
        }
        let end = (at..mask.len()).find(|&i| !is_ident(mask[i])).unwrap_or(mask.len());
        let word = text(mask, at..end);
        if !mask[at].is_ascii_digit() {
            let mut called: Vec<&'static Position> = calls.get(word).cloned().unwrap_or_default();
            called.extend(prefixes.iter().filter(|(prefix, _)| word.starts_with(prefix)).map(|(_, p)| *p));
            if !called.is_empty() {
                found.extend(call_sites(mask, end, &called));
            }
            if let Some(stored) = stores.get(word) {
                found.extend(store_sites(mask, end, stored));
            }
            if let Some(listed) = tables.get(word) {
                found.extend(table_sites(mask, end, listed));
            }
            if word == "struct" {
                found.extend(member_sites(mask, end, &members));
            }
        }
        at = end;
    }
    found
}

fn call_sites(mask: &[u8], end: usize, positions: &[&'static Position]) -> Vec<Found> {
    let Some(open) = super::source::next(mask, end).filter(|&i| mask[i] == b'(') else {
        return Vec::new();
    };
    let Some(close) = closing(mask, open) else {
        return Vec::new();
    };
    let arguments = split_commas(mask, open + 1..close);
    let mut found = Vec::new();
    for &position in positions {
        let Place::Call { argument, when, .. } = position.place else {
            continue;
        };
        let Some(range) = arguments.get(argument) else {
            continue;
        };
        if let Some((index, name, value)) = when {
            let Some(other) = arguments.get(index) else {
                continue;
            };
            let other = text(mask, trim(mask, other.clone()));
            if other != name && number(other) != Some(value) {
                continue;
            }
        }
        if let Some(held) = held(position, mask, range) {
            found.push((position, trim(mask, range.clone()), held));
        }
    }
    found
}

fn store_sites(mask: &[u8], end: usize, positions: &[&'static Position]) -> Vec<Found> {
    if !super::source::store(mask, end) {
        return Vec::new();
    }
    let Some(equals) = super::source::next(mask, end) else {
        return Vec::new();
    };
    let mut depth = 0i32;
    let mut stop = mask.len();
    for (i, &byte) in mask.iter().enumerate().skip(equals + 1) {
        match byte {
            b'(' | b'[' | b'{' => depth += 1,
            b')' | b']' | b'}' if depth == 0 => {
                stop = i;
                break;
            }
            b')' | b']' | b'}' => depth -= 1,
            b';' | b',' if depth == 0 => {
                stop = i;
                break;
            }
            _ => {}
        }
    }
    let range = equals + 1..stop;
    positions
        .iter()
        .filter_map(|&position| held(position, mask, &range).map(|held| (position, trim(mask, range.clone()), held)))
        .collect()
}

/// The braced initializer of the definition whose declarator starts at
/// `from`: the text inside its braces, and whether it is an array. None
/// for a prototype, a declaration, a cast or a struct definition.
fn initializer(mask: &[u8], from: usize) -> Option<(Range<usize>, bool)> {
    let mut array = false;
    for i in from..mask.len() {
        match mask[i] {
            b'[' => array = true,
            b'=' if mask.get(i + 1) != Some(&b'=') => {
                let open = super::source::next(mask, i + 1).filter(|&o| mask[o] == b'{')?;
                let close = closing(mask, open)?;
                return Some((open + 1..close, array));
            }
            b';' | b'{' | b'(' | b')' | b'}' | b',' => return None,
            _ => {}
        }
    }
    None
}

fn elements(mask: &[u8], body: Range<usize>) -> Vec<Range<usize>> {
    split_commas(mask, body).into_iter().map(|range| trim(mask, range)).filter(|range| !range.is_empty()).collect()
}

fn table_sites(mask: &[u8], end: usize, positions: &[&'static Position]) -> Vec<Found> {
    let Some((body, _)) = initializer(mask, end) else {
        return Vec::new();
    };
    let mut found = Vec::new();
    for element in elements(mask, body) {
        for &position in positions {
            if let Some(held) = held(position, mask, &element) {
                found.push((position, element.clone(), held));
            }
        }
    }
    found
}

/// The value of a row's designated `.field = value`, if the row names it.
fn designated(mask: &[u8], values: &[Range<usize>], field: &str) -> Option<Range<usize>> {
    values.iter().find_map(|value| {
        if mask[value.start] != b'.' {
            return None;
        }
        let equals = (value.start..value.end).find(|&i| mask[i] == b'=')?;
        (text(mask, trim(mask, value.start + 1..equals)) == field).then_some(equals + 1..value.end)
    })
}

fn member_sites(mask: &[u8], end: usize, members: &HashMap<&str, Vec<&'static Position>>) -> Vec<Found> {
    let Some(tag_start) = super::source::next(mask, end) else {
        return Vec::new();
    };
    let tag_end = (tag_start..mask.len()).find(|&i| !is_ident(mask[i])).unwrap_or(mask.len());
    let Some(positions) = members.get(text(mask, tag_start..tag_end)) else {
        return Vec::new();
    };
    let Some((body, array)) = initializer(mask, tag_end) else {
        return Vec::new();
    };
    let rows: Vec<Range<usize>> = if array {
        elements(mask, body)
            .into_iter()
            .filter(|row| mask[row.start] == b'{' && closing(mask, row.start) == Some(row.end - 1))
            .map(|row| row.start + 1..row.end - 1)
            .collect()
    } else {
        vec![body]
    };
    let mut found = Vec::new();
    for row in rows {
        let values = elements(mask, row);
        let named = values.iter().any(|value| mask[value.start] == b'.');
        for &position in positions {
            let Place::Member { index, field, .. } = position.place else {
                continue;
            };
            let range = if named { designated(mask, &values, field) } else { values.get(index).cloned() };
            let Some(range) = range else {
                continue;
            };
            if let Some(held) = held(position, mask, &range) {
                found.push((position, trim(mask, range), held));
            }
        }
    }
    found
}

/// Assembler text with its comments (`@`, `/* */`, `//`), preprocessor and
/// `#` comment lines and string contents blanked, newlines kept.
pub(crate) fn assembler_mask(text: &str) -> Vec<u8> {
    let bytes = text.as_bytes();
    let mut out = bytes.to_vec();
    let blank = |out: &mut Vec<u8>, range: Range<usize>| {
        for byte in &mut out[range] {
            if *byte != b'\n' {
                *byte = b' ';
            }
        }
    };
    let (mut i, mut line_start) = (0, true);
    while i < bytes.len() {
        let byte = bytes[i];
        if byte == b'\n' {
            line_start = true;
            i += 1;
            continue;
        }
        if bytes[i..].starts_with(b"/*") {
            let end = text[i + 2..].find("*/").map_or(bytes.len(), |e| i + 2 + e + 2);
            blank(&mut out, i..end);
            i = end;
            continue;
        }
        if byte == b'@' || bytes[i..].starts_with(b"//") || (line_start && byte == b'#') {
            let end = text[i..].find('\n').map_or(bytes.len(), |e| i + e);
            blank(&mut out, i..end);
            i = end;
            continue;
        }
        if !byte.is_ascii_whitespace() {
            line_start = false;
        }
        if byte == b'"' {
            let mut end = i + 1;
            while end < bytes.len() && bytes[end] != b'"' && bytes[end] != b'\n' {
                end += if bytes[end] == b'\\' { 2 } else { 1 };
            }
            let end = end.min(bytes.len());
            blank(&mut out, i + 1..end);
            i = end + 1;
            continue;
        }
        if !byte.is_ascii() {
            out[i] = b' ';
        }
        i += 1;
    }
    out
}

/// The data directives whose rows a Rows position reads.
const DATA: &[&str] = &[".byte", ".2byte", ".hword", ".short", ".4byte", ".word", ".int", ".long"];

fn assembler_sites(mask: &[u8], positions: &[&'static Position]) -> Vec<Found> {
    let symbol = |at: usize, end: usize| (at..end).find(|&i| !(is_ident(mask[i]) || mask[i] == b'.')).unwrap_or(end);
    let mut found = Vec::new();
    let mut label: Option<&str> = None;
    let mut offset = 0;
    for line in mask.split_inclusive(|&b| b == b'\n') {
        let range = trim(mask, offset..offset + line.len());
        offset += line.len();
        let (mut at, end) = (range.start, range.end);
        // Labels, then the directive, instruction or macro.
        loop {
            let word = symbol(at, end);
            if word > at && mask.get(word) == Some(&b':') {
                label = Some(text(mask, at..word));
                at = trim(mask, word + 1..end).start;
                continue;
            }
            break;
        }
        let word_end = symbol(at, end);
        if word_end == at {
            continue;
        }
        let word = text(mask, at..word_end);
        let arguments = if word_end < end { split_commas(mask, word_end..end) } else { Vec::new() };
        for &position in positions {
            let (index, here) = match position.place {
                Place::Macro { name, argument } => (argument, word == name),
                Place::Rows { label: wanted, column } => (column, DATA.contains(&word) && label == Some(wanted)),
                _ => continue,
            };
            let Some(argument) = arguments.get(index).filter(|_| here) else {
                continue;
            };
            if let Some(held) = held(position, mask, argument) {
                found.push((position, trim(mask, argument.clone()), held));
            }
        }
    }
    found
}

/// The names a header defines, by value.
#[derive(Default, Debug)]
pub(crate) struct Names {
    by_value: BTreeMap<i64, Vec<String>>,
    /// The names a preprocessed assembler file reads: object-like
    /// #defines outside the header's C-only blocks, whose bodies need no
    /// enum constant and carry no C integer suffix.
    assembler: BTreeSet<String>,
    /// Whether the header holds C outside its C-only blocks (an enum, a
    /// declaration), which an assembler file cannot include.
    c: bool,
}

/// A name that counts a family's ids, which never names one of them.
fn count(name: &str) -> bool {
    name.ends_with("_COUNT") || name.starts_with("NUM_")
}

/// A name that bounds a family of ids: it names its value only where
/// nothing else does.
fn bound(name: &str) -> bool {
    ["_START", "_END", "_BASE", "_FIRST", "_LAST"].iter().any(|suffix| name.ends_with(suffix))
}

/// Whether each line of a header lies in a block only C reads: `#ifndef
/// __ASSEMBLER__`, `#if !defined(__ASSEMBLER__)`, or the `#else` of
/// `#ifdef __ASSEMBLER__`.
fn c_only_lines(text: &str) -> Vec<bool> {
    // Each open conditional: for a test of __ASSEMBLER__, whether its first
    // branch is C only and whether the current branch is.
    let mut blocks: Vec<Option<(bool, bool)>> = Vec::new();
    let mut lines = Vec::new();
    for line in text.lines() {
        if let Some(directive) = line.trim_start().strip_prefix('#') {
            let directive = directive.split("//").next().unwrap_or("").split("/*").next().unwrap_or("");
            let directive: String = directive.chars().filter(|c| !c.is_whitespace() && !matches!(c, '(' | ')')).collect();
            if directive.starts_with("if") {
                blocks.push(match directive.as_str() {
                    "ifndef__ASSEMBLER__" | "if!defined__ASSEMBLER__" => Some((true, true)),
                    "ifdef__ASSEMBLER__" | "ifdefined__ASSEMBLER__" => Some((false, false)),
                    _ => None,
                });
            } else if directive.starts_with("el") {
                if let Some(Some((first, now))) = blocks.last_mut() {
                    *now = !*first;
                }
            } else if directive.starts_with("endif") {
                blocks.pop();
            }
        }
        lines.push(blocks.iter().any(|block| matches!(block, Some((_, true)))));
    }
    lines
}

/// Whether a #define body reads the same to the assembler: no C integer
/// suffix (`0x10u`).
fn assembler_body(body: &str) -> bool {
    tokens(body).is_some_and(|tokens| {
        tokens.iter().all(|token| !(token.as_bytes()[0].is_ascii_digit() && token.ends_with(['u', 'U', 'l', 'L'])))
    })
}

/// Evaluates the pending #defines whose bodies are known, until none is
/// left that can be.
fn resolve(pending: &mut Vec<(String, String)>, known: &mut HashMap<String, i64>, order: &mut Vec<(String, i64)>) {
    loop {
        let before = pending.len();
        pending.retain(|(name, body)| match evaluate(body, known) {
            Some(value) => {
                known.insert(name.clone(), value);
                order.push((name.clone(), value));
                false
            }
            None => true,
        });
        if pending.len() == before {
            return;
        }
    }
}

/// What a value is called.
#[derive(Debug, PartialEq, Eq)]
pub(crate) enum Called<'a> {
    One(&'a str),
    Several(Vec<&'a str>),
    Nothing,
}

impl Names {
    pub(crate) fn parse(text: &str) -> Names {
        let c_only = c_only_lines(text);
        let defined = defines(text);
        let mut known: HashMap<String, i64> = HashMap::new();
        let mut order: Vec<(String, i64)> = Vec::new();
        let mut pending: Vec<(String, String)> = defined.iter().map(|(name, body, _)| (name.clone(), body.clone())).collect();
        // The defines, the enums, then defines that use an enum constant.
        resolve(&mut pending, &mut known, &mut order);
        for (name, value) in enums(text, &known) {
            known.insert(name.clone(), value);
            order.push((name, value));
        }
        resolve(&mut pending, &mut known, &mut order);
        // What the assembler sees: no enum, nothing in a C-only block.
        let (mut seen, mut read) = (HashMap::new(), Vec::new());
        let mut visible: Vec<(String, String)> = defined
            .iter()
            .filter(|(_, body, line)| !c_only.get(*line).copied().unwrap_or(false) && assembler_body(body))
            .map(|(name, body, _)| (name.clone(), body.clone()))
            .collect();
        resolve(&mut visible, &mut seen, &mut read);
        let masked = mask(text);
        let mut names = Names {
            assembler: read.into_iter().filter(|(name, value)| known.get(name) == Some(value)).map(|(name, _)| name).collect(),
            c: masked
                .split(|&b| b == b'\n')
                .zip(c_only.iter().copied().chain(std::iter::repeat(false)))
                .any(|(line, only)| !only && !line.iter().all(u8::is_ascii_whitespace)),
            ..Names::default()
        };
        for (name, value) in order {
            let entry = names.by_value.entry(value).or_default();
            if !entry.contains(&name) {
                entry.push(name);
            }
        }
        names
    }

    pub(crate) fn is_empty(&self) -> bool {
        self.by_value.is_empty()
    }

    /// Whether a preprocessed assembler file can include the header: it
    /// holds no C outside its C-only blocks.
    pub(crate) fn assembler_readable(&self) -> bool {
        !self.c
    }

    /// Whether a preprocessed assembler file reads the name.
    pub(crate) fn assembler_reads(&self, name: &str) -> bool {
        self.assembler.contains(name)
    }

    /// Adds another header's names.
    pub(crate) fn extend(&mut self, other: Names) {
        for (value, names) in other.by_value {
            let entry = self.by_value.entry(value).or_default();
            for name in names {
                if !entry.contains(&name) {
                    entry.push(name);
                }
            }
        }
        self.assembler.extend(other.assembler);
        self.c |= other.c;
    }

    /// The one name of a value: a count never names it, a family's bound
    /// gives way to a name of its own, and two names of one value name
    /// neither.
    pub(crate) fn called(&self, value: i64) -> Called<'_> {
        let all: Vec<&str> =
            self.by_value.get(&value).into_iter().flatten().map(String::as_str).filter(|name| !count(name)).collect();
        let own: Vec<&str> = all.iter().copied().filter(|name| !bound(name)).collect();
        match (own.as_slice(), all.as_slice()) {
            (_, []) => Called::Nothing,
            ([one], _) | ([], [one]) => Called::One(one),
            ([], several) | (several, _) => Called::Several(several.to_vec()),
        }
    }
}

/// Each object-like `#define NAME body`, continuation lines joined and
/// comments dropped, with the index of the line it starts on.
fn defines(text: &str) -> Vec<(String, String, usize)> {
    let mut found = Vec::new();
    let mut lines = text.lines();
    let mut index = 0;
    while let Some(line) = lines.next() {
        let start = index;
        index += 1;
        let mut joined = line.to_string();
        while joined.trim_end().ends_with('\\') {
            joined = joined.trim_end().trim_end_matches('\\').to_string();
            match lines.next() {
                Some(next) => {
                    index += 1;
                    joined.push_str(next);
                }
                None => break,
            }
        }
        let Some(rest) = joined.trim_start().strip_prefix('#').map(str::trim_start).and_then(|r| r.strip_prefix("define")) else {
            continue;
        };
        if !rest.starts_with([' ', '\t']) {
            continue;
        }
        let rest = rest.trim_start();
        let name_end = rest.bytes().position(|b| !is_ident(b)).unwrap_or(rest.len());
        if name_end == 0 || rest[name_end..].starts_with('(') {
            continue;
        }
        let mut body = rest[name_end..].to_string();
        while let Some(start) = body.find("/*") {
            let end = body[start..].find("*/").map_or(body.len(), |e| start + e + 2);
            body.replace_range(start..end, " ");
        }
        if let Some(start) = body.find("//") {
            body.truncate(start);
        }
        found.push((rest[..name_end].to_string(), body.trim().to_string(), start));
    }
    found
}

/// Each enum constant and its value.
fn enums(source: &str, known: &HashMap<String, i64>) -> Vec<(String, i64)> {
    let masked = mask(source);
    let mut known = known.clone();
    let mut found = Vec::new();
    for at in super::source::words(&masked, "enum", 0..masked.len()) {
        let Some(open) = (at + 4..masked.len()).find(|&i| !(is_ident(masked[i]) || masked[i].is_ascii_whitespace())) else {
            continue;
        };
        if masked[open] != b'{' {
            continue;
        }
        let Some(close) = closing(&masked, open) else {
            continue;
        };
        let mut next = 0i64;
        for entry in elements(&masked, open + 1..close) {
            let entry = text(&masked, entry);
            let (name, value) = match entry.split_once('=') {
                Some((name, value)) => (name.trim(), evaluate(value, &known)),
                None => (entry.trim(), Some(next)),
            };
            let Some(value) = value.filter(|_| !name.is_empty() && name.bytes().all(is_ident)) else {
                break;
            };
            known.insert(name.to_string(), value);
            found.push((name.to_string(), value));
            next = value + 1;
        }
    }
    found
}

/// The value of a constant expression of integers and known names, with
/// C's arithmetic, shift and bit operators; None for anything else.
pub(crate) fn evaluate(text: &str, known: &HashMap<String, i64>) -> Option<i64> {
    let tokens = tokens(text)?;
    let mut at = 0;
    let value = binary(&tokens, &mut at, 0, known)?;
    (at == tokens.len()).then_some(value)
}

fn tokens(text: &str) -> Option<Vec<&str>> {
    let bytes = text.as_bytes();
    let mut out = Vec::new();
    let mut i = 0;
    while i < bytes.len() {
        let byte = bytes[i];
        let end = if byte.is_ascii_whitespace() {
            i += 1;
            continue;
        } else if is_ident(byte) {
            (i..bytes.len()).find(|&k| !is_ident(bytes[k])).unwrap_or(bytes.len())
        } else if text[i..].starts_with("<<") || text[i..].starts_with(">>") {
            i + 2
        } else if b"()+-*/%|&^~".contains(&byte) {
            i + 1
        } else {
            return None;
        };
        out.push(&text[i..end]);
        i = end;
    }
    Some(out)
}

/// Binary operators from the loosest: | ^ & shifts, then + -; * / % bind
/// tighter still.
const LEVELS: [&[&str]; 6] = [&["|"], &["^"], &["&"], &["<<", ">>"], &["+", "-"], &["*", "/", "%"]];

fn binary(tokens: &[&str], at: &mut usize, level: usize, known: &HashMap<String, i64>) -> Option<i64> {
    if level == LEVELS.len() {
        return unary(tokens, at, known);
    }
    let mut value = binary(tokens, at, level + 1, known)?;
    while let Some(&op) = tokens.get(*at).filter(|t| LEVELS[level].contains(t)) {
        *at += 1;
        let right = binary(tokens, at, level + 1, known)?;
        value = match op {
            "|" => value | right,
            "^" => value ^ right,
            "&" => value & right,
            "<<" => value.checked_shl(u32::try_from(right).ok()?)?,
            ">>" => value.checked_shr(u32::try_from(right).ok()?)?,
            "+" => value.checked_add(right)?,
            "-" => value.checked_sub(right)?,
            "*" => value.checked_mul(right)?,
            "/" => value.checked_div(right)?,
            _ => value.checked_rem(right)?,
        };
    }
    Some(value)
}

fn unary(tokens: &[&str], at: &mut usize, known: &HashMap<String, i64>) -> Option<i64> {
    let token = *tokens.get(*at)?;
    *at += 1;
    match token {
        "-" => unary(tokens, at, known)?.checked_neg(),
        "+" => unary(tokens, at, known),
        "~" => Some(!unary(tokens, at, known)?),
        "(" => {
            let value = binary(tokens, at, 0, known)?;
            (tokens.get(*at) == Some(&")")).then(|| *at += 1)?;
            Some(value)
        }
        word if word.as_bytes()[0].is_ascii_digit() => number(word),
        word => known.get(word).copied(),
    }
}

/// Names for one file's values before the header's: file stem, value, alias.
#[derive(Default, Debug)]
pub(crate) struct Aliases {
    by_stem: HashMap<String, HashMap<i64, String>>,
}

impl Aliases {
    /// A tab-separated table; a first row whose value is no number is its
    /// heading, and a row beginning with `#` a comment.
    pub(crate) fn parse(text: &str, source: &str) -> Result<Aliases, String> {
        let mut aliases = Aliases::default();
        for (number, row) in text.lines().enumerate() {
            if row.trim().is_empty() || row.starts_with('#') {
                continue;
            }
            let fields: Vec<&str> = row.split('\t').map(str::trim).collect();
            let [stem, value, alias] = fields[..] else {
                return Err(format!("{source}:{}: a file stem, a value and an alias expected", number + 1));
            };
            let Some(value) = evaluate(value, &HashMap::new()) else {
                if number == 0 {
                    continue;
                }
                return Err(format!("{source}:{}: {value} is not a number", number + 1));
            };
            if !alias.as_bytes().first().is_some_and(|b| is_ident(*b) && !b.is_ascii_digit()) || !alias.bytes().all(is_ident) {
                return Err(format!("{source}:{}: {alias} is not a name", number + 1));
            }
            let previous = aliases.by_stem.entry(stem.to_string()).or_default().insert(value, alias.to_string());
            if previous.is_some_and(|previous| previous != alias) {
                return Err(format!("{source}:{}: {stem} names {value:#x} twice", number + 1));
            }
        }
        Ok(aliases)
    }

    fn get(&self, stem: &str, value: i64) -> Option<&str> {
        self.by_stem.get(stem)?.get(&value).map(String::as_str)
    }
}

/// A site to name by hand.
#[derive(Debug, PartialEq, Eq)]
pub(crate) struct Hand {
    pub line: usize,
    pub kind: Kind,
    pub place: String,
    pub written: String,
    pub why: String,
}

/// One file's renaming.
#[derive(Debug)]
pub(crate) struct Renamed {
    pub text: String,
    pub count: usize,
    /// Whether a header name was written (aliases are the file's own).
    pub header_used: bool,
    pub hand: Vec<Hand>,
}

/// A value as a hand note prints it: flags in hex, sounds in decimal.
fn shown(kind: Kind, value: i64) -> String {
    match kind {
        Kind::Flag if value >= 0 => format!("{value:#x}"),
        _ => value.to_string(),
    }
}

/// Names a file's literal ids of one kind: an alias for the file's stem
/// first, then the header's names. An assembler file takes only the names
/// the assembler reads.
pub(crate) fn rename(path: &Path, text: &str, kind: Kind, names: &Names, aliases: &Aliases) -> Renamed {
    let stem = path.file_stem().and_then(|s| s.to_str()).unwrap_or("");
    let preprocessed = path.extension().and_then(|e| e.to_str()) != Some("s");
    let assembly = assembler(path);
    let defined: BTreeSet<String> = defines(text).into_iter().map(|(name, ..)| name).collect();
    let name_of = |value: i64| -> Option<String> {
        aliases.get(stem, value).map(str::to_string).or_else(|| match names.called(value) {
            Called::One(name) => Some(name.to_string()),
            _ => None,
        })
    };
    let mut edits: Vec<(Range<usize>, String)> = Vec::new();
    let mut hand = Vec::new();
    let mut header_used = false;
    let mut aliases_used: BTreeSet<&str> = BTreeSet::new();
    for site in sites(path, text, &[kind]) {
        let written = text[site.span.clone()].split_whitespace().collect::<Vec<_>>().join(" ");
        let mut note = |why: String| {
            hand.push(Hand { line: site.line, kind, place: site.position.place.label(), written: written.clone(), why });
        };
        let (literal, value) = match site.held {
            Held::Computed => {
                note("computed".into());
                continue;
            }
            Held::Outside { .. } => {
                note(format!("no {} id: the table or call holds something else", kind.name()));
                continue;
            }
            Held::Encoded { value, id, form } | Held::Constant { value, id, form } => {
                let named = name_of(id).unwrap_or_else(|| shown(kind, id));
                let written = match form {
                    _ if value == id => named,
                    Form::Condition(_) => format!("IF_FLAG_SET({named})"),
                    _ => format!("{:#x} | {named}", value ^ id),
                };
                note(match (site.held, form) {
                    (Held::Constant { .. }, _) => format!("constant: {written}"),
                    (_, Form::Condition(_)) => format!("condition while set: {written}"),
                    _ => format!("packed: {written}"),
                });
                continue;
            }
            Held::Id { literal, value } => (literal, value),
        };
        if !preprocessed {
            note("the preprocessor does not read a .s file: name it NAME.S first (S3)".into());
            continue;
        }
        if let Some(alias) = aliases.get(stem, value) {
            if assembly && !defined.contains(alias) {
                note(format!("alias {alias}: the assembler reads it only from a #define in this file"));
                continue;
            }
            aliases_used.insert(alias);
            edits.push((literal, alias.to_string()));
            continue;
        }
        match names.called(value) {
            Called::One(_) if assembly && !names.assembler_readable() => {
                note("the header holds C outside #ifndef __ASSEMBLER__ (an enum): the assembler cannot include it".into());
            }
            Called::One(name) if assembly && !names.assembler_reads(name) => {
                note(format!("{name} is an enum constant or C only: the assembler cannot read it"));
            }
            Called::One(name) => {
                header_used = true;
                edits.push((literal, name.to_string()));
            }
            Called::Several(several) => note(format!("{} names: {}", several.len(), several.join(", "))),
            Called::Nothing => note("no name".into()),
        }
    }
    let masked = if assembly { assembler_mask(text) } else { mask(text) };
    for alias in aliases_used {
        if !defined.contains(alias) && super::source::words(&masked, alias, 0..masked.len()).is_empty() {
            hand.push(Hand {
                line: 1,
                kind,
                place: "alias".into(),
                written: alias.into(),
                why: "used here and defined nowhere in this file".into(),
            });
        }
    }
    let mut out = String::with_capacity(text.len());
    let mut last = 0;
    for (span, name) in &edits {
        out.push_str(&text[last..span.start]);
        out.push_str(name);
        last = span.end;
    }
    out.push_str(&text[last..]);
    Renamed { text: out, count: edits.len(), header_used, hand }
}

/// The include path of the game a file builds in: its game's include
/// folder, then games/common's.
fn include_path(root: &Path, file: &Path) -> Vec<PathBuf> {
    let game = super::games(file)[0].game_dir();
    let common = crate::compiler::routing::COMMON_INCLUDE_MODULES.iter().map(|module| root.join("games/common/include").join(module));
    std::iter::once(root.join(game).join("include")).chain(common).collect()
}

/// Whether `file` itself includes `header`.
fn includes_header(file: &Path, text: &str, header: &Path, search: &[PathBuf]) -> bool {
    let Ok(header) = header.canonicalize() else {
        return false;
    };
    let directory = file.parent().unwrap_or(Path::new("."));
    super::source::includes(text).into_iter().any(|(name, quoted)| {
        super::source::resolve(&name, quoted, directory, search).and_then(|path| path.canonicalize().ok()).as_deref()
            == Some(header.as_path())
    })
}

struct Options {
    header: PathBuf,
    kind: Kind,
    aliases: Aliases,
    files: Vec<String>,
}

fn options(arguments: &[String]) -> Result<Options, String> {
    let (mut header, mut kind, mut aliases, mut files) = (None, None, Aliases::default(), Vec::new());
    let mut rest = arguments.iter();
    while let Some(argument) = rest.next() {
        match argument.as_str() {
            "-h" | "--help" => return Err(USAGE.into()),
            "--header" => header = Some(PathBuf::from(rest.next().ok_or(USAGE)?)),
            "--kind" => {
                let name = rest.next().ok_or(USAGE)?;
                kind = Some(Kind::parse(name).ok_or_else(|| format!("unknown kind {name}\n{USAGE}"))?);
            }
            "--aliases" => {
                let path = rest.next().ok_or(USAGE)?;
                let text = std::fs::read_to_string(path).map_err(|e| format!("{path}: {e}"))?;
                aliases = Aliases::parse(&text, path)?;
            }
            option if option.starts_with("--") => return Err(format!("unknown option {option}\n{USAGE}")),
            file => files.push(file.to_string()),
        }
    }
    let header = header.ok_or(USAGE)?;
    let stem = header.file_stem().and_then(|s| s.to_str()).unwrap_or("");
    let kind = kind
        .or_else(|| HEADER_KINDS.iter().find(|(name, _)| *name == stem).map(|(_, kind)| *kind))
        .ok_or_else(|| format!("{}: which kind of id does it name? Pass --kind flag or --kind sound", header.display()))?;
    if files.is_empty() {
        return Err(USAGE.into());
    }
    Ok(Options { header, kind, aliases, files })
}

/// `alchemy camelotize constants`, once camelotize has taken `--dry-run`.
pub(crate) fn run(root: &Path, arguments: &[String], dry: bool) -> Result<(), String> {
    let options = options(arguments)?;
    let header = std::path::absolute(&options.header).map_err(|e| format!("{}: {e}", options.header.display()))?;
    let names = Names::parse(&std::fs::read_to_string(&header).map_err(|e| format!("{}: {e}", header.display()))?);
    if names.is_empty() {
        return Err(format!("{}: defines no integer names", header.display()));
    }
    let (mut total, mut by_hand) = (0, 0);
    for name in &options.files {
        let file = std::path::absolute(name).map_err(|e| format!("{name}: {e}"))?;
        if file == header {
            continue;
        }
        if super::library(root, &file) {
            eprintln!("{name}: skipped: Nintendo library code");
            continue;
        }
        let text = std::fs::read_to_string(&file).map_err(|e| format!("{name}: {e}"))?;
        let renamed = rename(&file, &text, options.kind, &names, &options.aliases);
        for hand in &renamed.hand {
            println!("hand\t{name}:{}\t{}\t{}\t{}\t{}", hand.line, hand.kind.name(), hand.place, hand.written, hand.why);
        }
        by_hand += renamed.hand.len();
        if renamed.count == 0 {
            continue;
        }
        let mut out = renamed.text;
        if renamed.header_used {
            let search = include_path(root, &file);
            if !includes_header(&file, &text, &header, &search) {
                let spelled = super::source::spelling(&file, &header, &search)
                    .ok_or_else(|| format!("{name}: cannot spell an include of {}", header.display()))?;
                // rename names nothing from a header an assembler file
                // cannot include, so header_used never asks for one.
                out = if assembler(&file) {
                    super::source::insert_include_masked(&out, &spelled, &assembler_mask(&out))
                } else {
                    super::source::insert_include(&out, &spelled)
                };
            }
        }
        if !dry {
            std::fs::write(&file, &out).map_err(|e| format!("{name}: {e}"))?;
        }
        println!("{}\t{name}", renamed.count);
        total += renamed.count;
    }
    let then = if dry {
        "nothing written (--dry-run)"
    } else {
        "run a bare `alchemy keep`, with no paths, to keep only byte-identical files"
    };
    println!("{total} names, {by_hand} sites to name by hand; {then}");
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    const FLAGS: &str = include_str!("../../tests/constants/flags.h");
    const SONGS: &str = include_str!("../../tests/constants/songs.h");
    const SCENE: &str = include_str!("../../tests/constants/scene.c");
    const SCENE_FLAGS: &str = include_str!("../../tests/constants/scene.flags.c");
    const SCENE_BOTH: &str = include_str!("../../tests/constants/scene.expected.c");
    const RULES: &str = include_str!("../../tests/constants/RULES.S");
    const RULES_EXPECTED: &str = include_str!("../../tests/constants/RULES.expected.S");
    const ALIASES: &str = include_str!("../../tests/constants/aliases.tsv");

    fn aliases() -> Aliases {
        Aliases::parse(ALIASES, "aliases.tsv").unwrap()
    }

    fn hands(renamed: &Renamed) -> Vec<(usize, &str, &str)> {
        renamed.hand.iter().map(|h| (h.line, h.written.as_str(), h.why.as_str())).collect()
    }

    #[test]
    fn a_header_names_each_value_once_and_a_familys_bound_gives_way() {
        let names = Names::parse(FLAGS);
        assert_eq!(names.called(0x909), Called::One("FLAG_ALTIN_FREED"));
        // A family's start names its first member when nothing else does.
        assert_eq!(names.called(0x180), Called::One("FLAG_VISITED_START"));
        // A name built from another.
        assert_eq!(names.called(0x182), Called::One("FLAG_VISITED_VALE"));
        // A start that shares its value with a member gives way to it.
        assert_eq!(names.called(0x400), Called::One("FLAG_SHOP_STOCKED_VALE"));
        assert_eq!(names.called(0x16e), Called::Several(vec!["FLAG_DJINN_TUTORIAL", "FLAG_DJINNI_MET"]));
        assert_eq!(names.called(0x123), Called::Nothing);
        // A count names no id, even alone.
        assert_eq!(names.called(0x20), Called::Nothing);
        assert_eq!(names.called(0x30), Called::Nothing);
        assert!(names.assembler_readable() && names.assembler_reads("FLAG_VISITED_VALE"));
        let songs = Names::parse(SONGS);
        assert_eq!(songs.called(74), Called::One("MUS_SANCTUM"));
        assert_eq!(songs.called(-1), Called::One("SONG_NONE"));
        // Enum constants count too, with their implicit values, but only C
        // reads them.
        assert_eq!(songs.called(113), Called::One("SE_MENU_CANCEL"));
        assert!(songs.assembler_readable() && songs.assembler_reads("MUS_SANCTUM") && !songs.assembler_reads("SE_MENU_CANCEL"));
        // A header with C outside #ifndef __ASSEMBLER__ is C's alone; a
        // define that needs an enum constant or a C suffix is too.
        let c = Names::parse("enum { SE_A = 3 };\n#define SE_B (SE_A + 1)\n#define SE_C 5u\n#define SE_D 6\n");
        assert!(!c.assembler_readable());
        assert_eq!((c.called(4), c.called(5)), (Called::One("SE_B"), Called::One("SE_C")));
        assert!(!c.assembler_reads("SE_B") && !c.assembler_reads("SE_C") && c.assembler_reads("SE_D"));
        let guarded = Names::parse("#ifdef __ASSEMBLER__\n#define SE_A 3\n#else\n#define SE_B 4\nextern int x;\n#endif\n");
        assert!(guarded.assembler_readable() && guarded.assembler_reads("SE_A") && !guarded.assembler_reads("SE_B"));
    }

    #[test]
    fn constant_expressions_evaluate_as_c_does() {
        let known = HashMap::from([("BASE".to_string(), 0x180)]);
        assert_eq!(evaluate("(BASE + 2)", &known), Some(0x182));
        assert_eq!(evaluate("1 << 4 | 0x3", &known), Some(0x13));
        assert_eq!(evaluate("-1", &known), Some(-1));
        assert_eq!(evaluate("0x10u", &known), Some(16));
        assert_eq!(evaluate("010", &known), Some(8));
        assert_eq!(evaluate("2 + 3 * 4", &known), Some(14));
        assert_eq!(evaluate("~0 & 0xff", &known), Some(0xff));
        assert_eq!(evaluate("UNKNOWN + 1", &known), None);
        assert_eq!(evaluate("(u16)3", &known), None);
        assert_eq!(evaluate("", &known), None);
    }

    #[test]
    fn flags_then_sounds_rewrite_exactly_the_id_positions() {
        let path = Path::new("scene.c");
        let flags = rename(path, SCENE, Kind::Flag, &Names::parse(FLAGS), &aliases());
        assert_eq!(flags.text, SCENE_FLAGS);
        assert_eq!(flags.count, 12);
        assert!(flags.header_used);
        let sounds = rename(path, &flags.text, Kind::Sound, &Names::parse(SONGS), &Aliases::default());
        assert_eq!(sounds.text, SCENE_BOTH);
        assert_eq!(sounds.count, 10);
        // Named once, a file has nothing left to name.
        assert_eq!(rename(path, &sounds.text, Kind::Flag, &Names::parse(FLAGS), &aliases()).count, 0);
        assert_eq!(rename(path, &sounds.text, Kind::Sound, &Names::parse(SONGS), &aliases()).count, 0);
    }

    #[test]
    fn what_cannot_be_named_plainly_is_listed_for_a_hand() {
        let path = Path::new("scene.c");
        let flags = rename(path, SCENE, Kind::Flag, &Names::parse(FLAGS), &aliases());
        assert_eq!(
            hands(&flags),
            [
                (29, "0x180 + group", "computed"),
                (30, "0x123", "no name"),
                (31, "0x16e", "2 names: FLAG_DJINN_TUTORIAL, FLAG_DJINNI_MET"),
                (32, "181 << 1", "constant: FLAG_AUTO_BATTLE"),
                (57, "0x1909", "condition while set: IF_FLAG_SET(FLAG_ALTIN_FREED)"),
                (60, "0x69c0", "no flag id: the table or call holds something else"),
            ]
        );
        let sounds = rename(path, SCENE, Kind::Sound, &Names::parse(SONGS), &Aliases::default());
        assert_eq!(
            hands(&sounds),
            [(37, "a + 90", "computed"), (73, "0x8036", "packed: 0x8000 | 54"), (74, "0x804a", "packed: 0x8000 | MUS_SANCTUM"), (81, "0x57", "no name")]
        );
    }

    #[test]
    fn assembler_macros_and_rows_name_their_flag_and_song_operands() {
        let path = Path::new("RULES.S");
        let flags = rename(path, RULES, Kind::Flag, &Names::parse(FLAGS), &aliases());
        let sounds = rename(path, &flags.text, Kind::Sound, &Names::parse(SONGS), &Aliases::default());
        assert_eq!(sounds.text, RULES_EXPECTED);
        assert_eq!((flags.count, sounds.count), (4, 3));
        assert_eq!(
            hands(&flags).iter().map(|(_, written, why)| (*written, *why)).collect::<Vec<_>>(),
            [("0x10182", "packed: 0x10000 | FLAG_VISITED_VALE"), ("0x20185", "packed: 0x20000 | 0x185")]
        );
        // SE_MENU_CANCEL is an enum constant: the assembler cannot read it.
        assert_eq!(
            hands(&sounds).iter().map(|(_, written, why)| (*written, *why)).collect::<Vec<_>>(),
            [("42", "no name"), ("113", "SE_MENU_CANCEL is an enum constant or C only: the assembler cannot read it")]
        );
        // A file the preprocessor does not read keeps its numbers.
        let plain = rename(Path::new("rules.s"), RULES, Kind::Flag, &Names::parse(FLAGS), &aliases());
        assert_eq!((plain.count, plain.text.as_str()), (0, RULES));
        assert_eq!(plain.hand.iter().filter(|h| h.why.contains("NAME.S")).count(), 4);
        // An alias names a value in an assembler file only where the file
        // #defines it.
        let alias = Aliases::parse("RULES\t0x909\tALTIN_GATE\n", "t").unwrap();
        let undefined = rename(path, RULES, Kind::Flag, &Names::default(), &alias);
        assert_eq!(undefined.text, RULES);
        assert!(undefined.hand.iter().any(|h| h.written == "0x909" && h.why.contains("ALTIN_GATE")));
        let text = RULES.replace("#define TOWN_GROUP 2\n", "#define TOWN_GROUP 2\n#define ALTIN_GATE 0x909\n");
        let defined = rename(path, &text, Kind::Flag, &Names::default(), &alias);
        assert_eq!((defined.count, defined.header_used), (1, false));
        assert!(defined.text.contains("group_cue 3, -1, ALTIN_GATE, 74"));
    }

    #[test]
    fn an_assembler_file_takes_no_enum_and_its_include_follows_its_comments_and_includes() {
        let directory = tempfile::tempdir().unwrap();
        let path = |name: &str| directory.path().join(name).to_string_lossy().into_owned();
        let root = crate::compiler::routing::root();
        let read = |name: &str| std::fs::read_to_string(path(name)).unwrap();
        // A header with an enum outside #ifndef __ASSEMBLER__ names nothing
        // in the file and is not included: each site is left for a hand.
        let unguarded = SONGS.replace("#ifndef __ASSEMBLER__\n", "").replacen("};\n#endif\n", "};\n", 1);
        std::fs::write(path("songs.h"), &unguarded).unwrap();
        std::fs::write(path("RULES.S"), RULES).unwrap();
        run(root, &["--header".into(), path("songs.h"), path("RULES.S")], false).unwrap();
        assert_eq!(read("RULES.S"), RULES);
        let left = rename(Path::new("RULES.S"), RULES, Kind::Sound, &Names::parse(&unguarded), &Aliases::default());
        assert_eq!(left.count, 0);
        assert_eq!(left.hand.iter().filter(|h| h.why.contains("cannot include")).count(), 4);
        // A header the assembler reads lands after the file's own include.
        std::fs::write(path("songs.h"), SONGS).unwrap();
        run(root, &["--header".into(), path("songs.h"), path("RULES.S")], false).unwrap();
        let songs = rename(Path::new("RULES.S"), RULES, Kind::Sound, &Names::parse(SONGS), &Aliases::default()).text;
        assert_eq!(read("RULES.S"), songs.replacen("#include \"flags.h\"\n", "#include \"flags.h\"\n#include \"songs.h\"\n", 1));
        // With no include, it lands below the leading @ comments.
        std::fs::write(path("flags.h"), FLAGS).unwrap();
        let bare = RULES.replacen("#include \"flags.h\"\n", "", 1);
        std::fs::write(path("RULES.S"), &bare).unwrap();
        run(root, &["--header".into(), path("flags.h"), path("RULES.S")], false).unwrap();
        let named = rename(Path::new("RULES.S"), &bare, Kind::Flag, &Names::parse(FLAGS), &Aliases::default()).text;
        assert_eq!(read("RULES.S"), named.replacen("#define TOWN_GROUP", "#include \"flags.h\"\n#define TOWN_GROUP", 1));
        assert!(read("RULES.S").starts_with("@ alchemy camelotize constants: assembler id positions, as scnrule.s,\n@ locrule.s"));
    }

    #[test]
    fn an_alias_names_its_files_values_first() {
        let text = "void F(void)\n{\n\tGameFlag_Set(0x240);\n}\n";
        let other = rename(Path::new("other.c"), text, Kind::Flag, &Names::parse(FLAGS), &aliases());
        assert_eq!(other.text, "void F(void)\n{\n\tGameFlag_Set(FLAG_SCENE_TEMP_00);\n}\n");
        let scene = rename(Path::new("scene.c"), text, Kind::Flag, &Names::parse(FLAGS), &aliases());
        assert_eq!(scene.text, "void F(void)\n{\n\tGameFlag_Set(GATE_OPENED);\n}\n");
        assert!(!scene.header_used);
        assert_eq!(hands(&scene), [(1, "GATE_OPENED", "used here and defined nowhere in this file")]);
        assert!(Aliases::parse("stem\tvalue\talias\nscene\t0x1\tX\n", "t").is_ok());
        assert!(Aliases::parse("scene\t0x1\tX\nscene\tfoo\tY\n", "t").is_err());
        assert!(Aliases::parse("scene\t0x1\tX\nscene\t1\tY\n", "t").is_err());
        assert!(Aliases::parse("scene\t0x1\t1X\n", "t").is_err());
    }

    #[test]
    fn a_run_writes_the_names_and_the_include_and_a_dry_run_writes_nothing() {
        let directory = tempfile::tempdir().unwrap();
        let path = |name: &str| directory.path().join(name).to_string_lossy().into_owned();
        std::fs::write(path("flags.h"), FLAGS).unwrap();
        std::fs::write(path("ids.h"), FLAGS).unwrap();
        std::fs::write(path("scene.c"), SCENE).unwrap();
        std::fs::write(path("aliases.tsv"), ALIASES).unwrap();
        let root = crate::compiler::routing::root();
        let arguments = ["--header".into(), path("flags.h"), "--aliases".into(), path("aliases.tsv"), path("scene.c")];
        run(root, &arguments, true).unwrap();
        assert_eq!(std::fs::read_to_string(path("scene.c")).unwrap(), SCENE);
        run(root, &arguments, false).unwrap();
        let written = std::fs::read_to_string(path("scene.c")).unwrap();
        assert_eq!(written, SCENE_FLAGS.replacen("#include \"event.h\"\n", "#include \"event.h\"\n#include \"flags.h\"\n", 1));
        // A header that already is included is not included again.
        run(root, &arguments, false).unwrap();
        assert_eq!(std::fs::read_to_string(path("scene.c")).unwrap(), written);
        // A header whose name does not say its kind needs --kind.
        let error = run(root, &["--header".into(), path("ids.h"), path("scene.c")], true).unwrap_err();
        assert!(error.contains("--kind"), "{error}");
        run(root, &["--header".into(), path("ids.h"), "--kind".into(), "flag".into(), path("scene.c")], true).unwrap();
    }

    #[test]
    fn a_game_file_includes_the_header_by_its_include_path() {
        let root = tempfile::tempdir().unwrap();
        let path = |name: &str| root.path().join(name);
        for folder in ["games/tbs/include", "games/tbs/src/scene", "games/tbs/src/battle"] {
            std::fs::create_dir_all(path(folder)).unwrap();
        }
        std::fs::write(path("games/tbs/include/flags.h"), FLAGS).unwrap();
        let scene = "#include \"event.h\"\n\nvoid F(void)\n{\n\tGameFlag_Set(0x909);\n}\n";
        let battle = "#include \"flags.h\"\n#include \"event.h\"\n\nvoid G(void)\n{\n\tGameFlag_Set(0x909);\n}\n";
        std::fs::write(path("games/tbs/src/scene/a.c"), scene).unwrap();
        std::fs::write(path("games/tbs/src/battle/b.c"), battle).unwrap();
        let arguments = ["games/tbs/include/flags.h", "games/tbs/src/scene/a.c", "games/tbs/src/battle/b.c"]
            .map(|name| path(name).to_string_lossy().into_owned());
        run(root.path(), &["--header".into(), arguments[0].clone(), arguments[1].clone(), arguments[2].clone()], false).unwrap();
        assert_eq!(
            std::fs::read_to_string(path("games/tbs/src/scene/a.c")).unwrap(),
            "#include \"event.h\"\n#include \"flags.h\"\n\nvoid F(void)\n{\n\tGameFlag_Set(FLAG_ALTIN_FREED);\n}\n"
        );
        assert_eq!(std::fs::read_to_string(path("games/tbs/src/battle/b.c")).unwrap(), battle.replace("(0x909)", "(FLAG_ALTIN_FREED)"));
    }

    #[test]
    fn a_literal_outside_an_id_position_is_never_a_site() {
        let text = "void F(void)\n{\n\tActor_SetPosition(8, 0x909, 0);\n\tx = 0x909;\n\tWAIT(74);\n\t// GameFlag_Set(0x909);\n\
            \tMes(\"GameFlag_Set(0x909)\");\n\tGameFlag_IsSetLater(0x909);\n\tBtlEvPush(BEV_NUM, 74);\n\tif (s->pending_cue == 74) {}\n\tBattleEventRuntime_BeginPhaseFar(0);\n}\n\
            s32 GameFlag_IsSet(s32 flag);\n#define CHECK() GameFlag_IsSet(0x909)\nextern const u16 gBattleCueTable[35];\n\
            u32 gSceneExits[] = {SCENE_EXIT_IF(1, 4, 2, 0), SCENE_EXIT_IF(1, 4, 2, 0xffffffff)};\n";
        // An exit's flag word of 0 is no condition, not flag 0.
        assert!(sites(Path::new("x.c"), text, &Kind::ALL).is_empty());
    }
}
