use super::asm::{identifier, Data, Pointer};
use crate::assets::midi::{midi_events, EventBody, MidiEvent};
use std::collections::{BTreeSet, HashMap};
use std::fmt;

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Arg {
    Int(i64),
    Sym(String),
}
impl fmt::Display for Arg {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Arg::Int(value) => write!(f, "{value}"),
            Arg::Sym(name) => f.write_str(name),
        }
    }
}
impl From<i64> for Arg {
    fn from(value: i64) -> Self {
        Arg::Int(value)
    }
}
impl From<&str> for Arg {
    fn from(name: &str) -> Self {
        Arg::Sym(name.to_string())
    }
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Event {
    pub kind: String,
    pub args: Vec<Arg>,
}
impl Event {
    pub fn new(kind: &str, args: Vec<Arg>) -> Self {
        Event {
            kind: kind.to_string(),
            args,
        }
    }
    pub fn parse(text: &str) -> Result<Self, String> {
        let mut words = text.split(' ');
        let kind = words.next().filter(|kind| identifier(kind));
        let kind = kind.ok_or_else(|| format!("sequence event {text:?} has no kind"))?;
        let args = words
            .map(|word| {
                if identifier(word) {
                    Ok(Arg::Sym(word.to_string()))
                } else {
                    word.parse::<i64>()
                        .map(Arg::Int)
                        .map_err(|_| format!("sequence event {text:?}: {word:?} is neither"))
                }
            })
            .collect::<Result<_, _>>()?;
        Ok(Event::new(kind, args))
    }
}
impl fmt::Display for Event {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        f.write_str(&self.kind)?;
        for arg in &self.args {
            write!(f, " {arg}")?;
        }
        Ok(())
    }
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Segment {
    Stream { label: String, events: Vec<Event> },
    Align { boundary: i64, fill: i64 },
    Header {
        label: String,
        block_count: i64,
        priority: i64,
        reverb: i64,
        tone_bank: String,
        tracks: Vec<String>,
    },
}

const SKELETON_ENGINE: &str = "smsh-sequence 1";

pub(crate) fn parse_skeleton(text: &str) -> Result<Vec<Segment>, String> {
    let mut lines = text.lines();
    if lines.next() != Some(SKELETON_ENGINE) {
        return Err(format!("sequence skeleton must begin {SKELETON_ENGINE:?}"));
    }
    let integer = |word: &str, label: &str| {
        word.parse::<i64>()
            .map_err(|_| format!("{label} must be an integer"))
    };
    let mut segments = Vec::new();
    for line in lines {
        let words: Vec<&str> = line.split(' ').collect();
        segments.push(match words.as_slice() {
            ["stream", label] => Segment::Stream {
                label: symbol(label, "stream label")?,
                events: Vec::new(),
            },
            ["align", boundary, fill] => Segment::Align {
                boundary: integer(boundary, "alignment boundary")?,
                fill: integer(fill, "alignment fill")?,
            },
            ["header", label, fields @ ..] => {
                let mut values = HashMap::new();
                for field in fields {
                    let (key, value) = field
                        .split_once('=')
                        .ok_or_else(|| format!("header field {field:?} has no value"))?;
                    if !matches!(
                        key,
                        "block_count" | "priority" | "reverb" | "tone_bank" | "tracks"
                    ) {
                        return Err(format!(
                            "sequence header records {key}; the build supplies placement and symbols"
                        ));
                    }
                    if values.insert(key, value).is_some() {
                        return Err(format!("header repeats {key}"));
                    }
                }
                let get = |key: &str| {
                    values
                        .get(key)
                        .copied()
                        .ok_or_else(|| format!("header {key} is missing"))
                };
                Segment::Header {
                    label: symbol(label, "header label")?,
                    block_count: integer(get("block_count")?, "header block_count")?,
                    priority: integer(get("priority")?, "header priority")?,
                    reverb: integer(get("reverb")?, "header reverb")?,
                    tone_bank: symbol(get("tone_bank")?, "tone bank")?,
                    tracks: get("tracks")?
                        .split(',')
                        .filter(|track| !track.is_empty())
                        .map(|track| symbol(track, "track symbol"))
                        .collect::<Result<_, _>>()?,
                }
            }
            _ => return Err(format!("unsupported skeleton line {line:?}")),
        });
    }
    Ok(segments)
}

fn symbol(word: &str, label: &str) -> Result<String, String> {
    if identifier(word) {
        Ok(word.to_string())
    } else {
        Err(format!("{label} is invalid"))
    }
}
fn int(arg: Option<&Arg>, label: &str) -> Result<i64, String> {
    match arg {
        Some(Arg::Int(value)) => Ok(*value),
        Some(Arg::Sym(_)) => Err(format!("{label} must be an integer")),
        None => Err(format!("{label} is missing")),
    }
}
fn number(arg: Option<&Arg>, label: &str) -> Result<usize, String> {
    usize::try_from(int(arg, label)?).map_err(|_| format!("{label} must be an integer"))
}
fn sequence_symbol(arg: Option<&Arg>, label: &str) -> Result<String, String> {
    match arg {
        Some(Arg::Sym(name)) => symbol(name, label),
        Some(Arg::Int(_)) => Err(format!("{label} must be a label")),
        None => Err(format!("{label} is missing")),
    }
}
const SEQUENCE_DURATIONS: [usize; 49] = [
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 28,
    30, 32, 36, 40, 42, 44, 48, 52, 54, 56, 60, 64, 66, 68, 72, 76, 78, 80, 84, 88, 90, 92, 96,
];
fn sequence_control_opcode(name: &str) -> Option<u8> {
    Some(match name {
        "priority" => 0xba,
        "tempo" => 0xbb,
        "key_shift" => 0xbc,
        "voice" => 0xbd,
        "volume" => 0xbe,
        "pan" => 0xbf,
        "pitch_bend" => 0xc0,
        "pitch_bend_range" => 0xc1,
        "lfo_speed" => 0xc2,
        "lfo_delay" => 0xc3,
        "modulation_depth" => 0xc4,
        "modulation_type" => 0xc5,
        "tuning" => 0xc8,
        _ => return None,
    })
}
fn sequence_sets_running_status(opcode: u8) -> bool {
    opcode != 0xbb
}
fn sequence_duration_index(arg: Option<&Arg>, label: &str) -> Result<u8, String> {
    let ticks = number(arg, label)?;
    SEQUENCE_DURATIONS
        .iter()
        .position(|candidate| *candidate == ticks)
        .map(|index| index as u8)
        .ok_or_else(|| format!("{label} is not representable by the engine duration table"))
}
fn sequence_parameter(arg: Option<&Arg>, name: &str) -> Result<u8, String> {
    let number = int(arg, name)?;
    let signed = matches!(name, "key_shift" | "pan" | "pitch_bend" | "tuning");
    if signed {
        if !(-128..=127).contains(&number) {
            return Err(format!("{name} does not fit s8"));
        }
        Ok((number as i8) as u8)
    } else if (0..=0xff).contains(&number) {
        Ok(number as u8)
    } else {
        Err(format!("{name} does not fit u8"))
    }
}
fn sequence_note_parameters(args: &[Arg], label: &str) -> Result<Vec<u8>, String> {
    if args.len() > 3 {
        return Err(format!("{label} has more than three parameters"));
    }
    args.iter()
        .enumerate()
        .map(|(index, arg)| {
            let number = number(Some(arg), &format!("{label} parameter {index}"))?;
            if number >= 0x80 {
                return Err(format!("{label} parameter {index} must be below 0x80"));
            }
            Ok(number as u8)
        })
        .collect()
}
struct EncodedSequenceStream {
    data: Vec<u8>,
    labels: Vec<(String, usize)>,
    pointers: Vec<(usize, String)>,
}
fn encode_sequence_stream(events: &[Event]) -> Result<EncodedSequenceStream, String> {
    let mut data = Vec::new();
    let mut local_labels = Vec::new();
    let mut pointers = Vec::new();
    let mut running: Option<u8> = None;
    for event in events {
        let (kind, args) = (event.kind.as_str(), event.args.as_slice());
        if kind == "label" {
            local_labels.push((sequence_symbol(args.first(), "event label")?, data.len()));
            continue;
        }
        let mut encoded = Vec::new();
        match kind {
            "wait" => encoded.push(0x80 + sequence_duration_index(args.first(), "wait duration")?),
            "fine" => {
                if !args.is_empty() {
                    return Err("fine takes no parameters".to_string());
                }
                encoded.push(0xb1);
            }
            "goto" | "pattern" => {
                if args.len() != 1 {
                    return Err(format!("{kind} takes one target"));
                }
                let target = sequence_symbol(args.first(), "sequence target")?;
                encoded.push(if kind == "goto" { 0xb2 } else { 0xb3 });
                pointers.push((data.len() + 1, target));
                encoded.extend_from_slice(&[0; 4]);
            }
            "pattern_end" => {
                if !args.is_empty() {
                    return Err("pattern_end takes no parameters".to_string());
                }
                encoded.push(0xb4);
            }
            "repeat" => {
                if args.len() != 2 {
                    return Err("repeat requires a count and target".to_string());
                }
                let count = number(args.first(), "repeat count")?;
                if count > 0xff {
                    return Err("repeat count does not fit u8".to_string());
                }
                let target = sequence_symbol(args.get(1), "repeat target")?;
                encoded.push(0xb5);
                encoded.push(count as u8);
                pointers.push((data.len() + 2, target));
                encoded.extend_from_slice(&[0; 4]);
            }
            "note" => {
                let opcode = 0xcf + sequence_duration_index(args.first(), "note duration")?;
                encoded.push(opcode);
                encoded.extend(sequence_note_parameters(&args[1..], "note")?);
                running = Some(opcode);
            }
            "note_running" => {
                let opcode = 0xcf + sequence_duration_index(args.first(), "running note duration")?;
                let values = sequence_note_parameters(&args[1..], "running note")?;
                if values.is_empty() {
                    return Err("running note emits no bytes".to_string());
                }
                if running.is_some_and(|active| active != opcode) {
                    return Err("running note status differs from active status".to_string());
                }
                encoded = values;
                running = Some(opcode);
            }
            "control_running" => {
                if args.len() != 2 {
                    return Err("control_running requires a name and value".to_string());
                }
                let name = sequence_symbol(args.first(), "running control name")?;
                let opcode = sequence_control_opcode(&name).ok_or("unknown running control")?;
                if !sequence_sets_running_status(opcode) {
                    return Err(format!("{name} cannot use running status"));
                }
                let value = sequence_parameter(args.get(1), &name)?;
                if value >= 0x80 {
                    return Err(
                        "running control parameter would be parsed as a command".to_string()
                    );
                }
                if running.is_some_and(|active| active != opcode) {
                    return Err("running control status differs from active status".to_string());
                }
                encoded.push(value);
                running = Some(opcode);
            }
            "note_end_running" => {
                if args.len() != 1 {
                    return Err("running note_end requires a value".to_string());
                }
                let value = sequence_note_parameters(args, "running note_end")?[0];
                if running.is_some_and(|active| active != 0xce) {
                    return Err("running note_end status differs from active status".to_string());
                }
                encoded.push(value);
                running = Some(0xce);
            }
            "note_end" => {
                if args.len() > 1 {
                    return Err("note_end has too many parameters".to_string());
                }
                encoded.push(0xce);
                encoded.extend(sequence_note_parameters(args, "note_end")?);
                running = Some(0xce);
            }
            _ => {
                let opcode = sequence_control_opcode(kind)
                    .ok_or_else(|| format!("unsupported sequence event: {kind}"))?;
                if args.len() != 1 {
                    return Err(format!("{kind} requires one parameter"));
                }
                encoded.push(opcode);
                encoded.push(sequence_parameter(args.first(), kind)?);
                if sequence_sets_running_status(opcode) {
                    running = Some(opcode);
                }
            }
        }
        data.extend(encoded);
    }
    Ok(EncodedSequenceStream {
        data,
        labels: local_labels,
        pointers,
    })
}
pub(crate) fn build_sequence(layout: &[Segment]) -> Result<Data, String> {
    let mut data = Data::default();
    let mut tone_banks = BTreeSet::new();
    for segment in layout {
        match segment {
            Segment::Stream { label, events } => {
                let offset = data.bytes.len();
                data.label(&symbol(label, "stream label")?, false);
                let encoded = encode_sequence_stream(events)?;
                data.labels
                    .extend(
                        encoded
                            .labels
                            .into_iter()
                            .map(|(name, inner)| super::asm::Label {
                                name,
                                offset: offset + inner,
                                global: false,
                            }),
                    );
                data.pointers.extend(
                    encoded
                        .pointers
                        .into_iter()
                        .map(|(site, symbol)| (offset + site, Pointer { symbol, addend: 0 })),
                );
                data.bytes.extend(encoded.data);
            }
            Segment::Align { boundary, fill } => {
                let boundary = usize::try_from(*boundary).unwrap_or(0);
                if !(2..=0x100).contains(&boundary) || !boundary.is_power_of_two() {
                    return Err(
                        "alignment boundary must be a power of two from 2 through 256".into(),
                    );
                }
                let fill = u8::try_from(*fill).map_err(|_| "alignment fill does not fit u8")?;
                data.align_to(boundary, fill)?;
            }
            Segment::Header {
                label,
                block_count,
                priority,
                reverb,
                tone_bank,
                tracks,
            } => {
                data.label(&symbol(label, "header label")?, true);
                if tracks.is_empty() || tracks.len() > 16 {
                    return Err("header track list is invalid".into());
                }
                if *block_count != 0 {
                    return Err("nonzero sequence block_count is not supported".into());
                }
                let priority =
                    u8::try_from(*priority).map_err(|_| "header value does not fit u8")?;
                let reverb = u8::try_from(*reverb).map_err(|_| "header value does not fit u8")?;
                let tone_bank = symbol(tone_bank, "tone bank")?;
                data.bytes
                    .extend_from_slice(&[tracks.len() as u8, 0, priority, reverb]);
                data.pointer(&tone_bank, 0);
                tone_banks.insert(tone_bank);
                for track in tracks {
                    data.pointer(&symbol(track, "track symbol")?, 0);
                }
            }
        }
    }
    let defined: BTreeSet<&str> = data.labels.iter().map(|l| l.name.as_str()).collect();
    if defined.len() != data.labels.len() {
        return Err("duplicate local label".into());
    }
    for (_, pointer) in &data.pointers {
        let symbol = pointer.symbol.as_str();
        if !defined.contains(symbol) && !tone_banks.contains(symbol) {
            return Err(format!("unknown sequence symbol: {symbol}"));
        }
    }
    Ok(data)
}

const MIDI_BUILD_DIRECTIVE: &[u8] = b"alchemy-mid2agb\0";
fn sequence_meter(conductor: &[MidiEvent], division: u16) -> Result<Vec<(i64, i64)>, String> {
    let whole = i64::from(division) * 4;
    let mut meter = Vec::<(i64, i64)>::new();
    for event in conductor {
        let EventBody::Meta { meta: 0x58, data } = &event.body else {
            continue;
        };
        let [numerator, power, ..] = data.as_slice() else {
            return Err("MIDI time signature is truncated".to_string());
        };
        let span = i64::from(*numerator) * whole;
        if *numerator == 0 || *power > 7 || span % (1i64 << power) != 0 {
            return Err(format!(
                "MIDI time signature {numerator}/2^{power} does not fit the tick grid"
            ));
        }
        if meter.last().is_some_and(|(tick, _)| *tick >= event.tick) {
            return Err("MIDI time signatures share or reverse a tick".to_string());
        }
        meter.push((event.tick, span >> power));
    }
    if meter.first().is_none_or(|(tick, _)| *tick != 0) {
        meter.insert(0, (0, whole));
    }
    Ok(meter)
}
fn rest_waits(start: i64, length: i64, meter: &[(i64, i64)]) -> Result<Vec<usize>, String> {
    if start < 0 || length < 0 {
        return Err("MIDI rest lies before the stream start".to_string());
    }
    let mut waits = Vec::new();
    let (mut at, end) = (start, start + length);
    while at < end {
        let index = meter.partition_point(|(tick, _)| *tick <= at) - 1;
        let (origin, bar) = meter[index];
        let mut line = origin + ((at - origin) / bar + 1) * bar;
        if let Some((next, _)) = meter.get(index + 1) {
            line = line.min(*next);
        }
        let stop = line.min(end);
        let mut gap = (stop - at) as usize;
        while gap > 0 {
            let wait = SEQUENCE_DURATIONS
                .iter()
                .rev()
                .copied()
                .find(|duration| *duration != 0 && *duration <= gap)
                .ok_or("MIDI wait cannot be tokenized")?;
            waits.push(wait);
            gap -= wait;
        }
        at = stop;
    }
    Ok(waits)
}
#[derive(Clone)]
struct MidiNode {
    compact_tick: i64,
    raw_tick: i64,
    order: usize,
    event: Event,
}

const MIDI_CONTROLS: [&str; 8] = [
    "tempo",
    "voice",
    "volume",
    "pan",
    "modulation_depth",
    "pitch_bend",
    "pitch_bend_range",
    "tuning",
];

fn midi_tempo(micros: u32) -> Result<i64, String> {
    let numerator = 7_200_000_000u128 * 280_896;
    let denominator = 16_777_216u128 * u128::from(micros);
    if micros == 0 {
        return Err("MIDI tempo is zero".to_string());
    }
    Ok(((2 * numerator + denominator) / (2 * denominator)) as i64)
}

fn midi_control(
    status: u8,
    data: &[u8],
    parameter: &mut (Option<u8>, Option<u8>),
    preamble: bool,
) -> Result<Option<Event>, String> {
    let value = |index: usize| {
        data.get(index)
            .map(|byte| i64::from(*byte))
            .ok_or_else(|| "MIDI channel event is truncated".to_string())
    };
    let control = |kind: &str, value: i64| Ok(Some(Event::new(kind, vec![value.into()])));
    match status & 0xf0 {
        0xc0 => control("voice", value(0)?),
        0xe0 => control("pitch_bend", value(1)?),
        0xb0 => match value(0)? {
            7 => control("volume", value(1)?),
            10 => control("pan", value(1)?),
            1 => control("modulation_depth", value(1)?),
            101 => {
                parameter.0 = Some(value(1)? as u8);
                Ok(None)
            }
            100 => {
                parameter.1 = Some(value(1)? as u8);
                Ok(None)
            }
            6 => match *parameter {
                (Some(0), Some(0)) if preamble => Ok(None),
                (Some(0), Some(0)) => control("pitch_bend_range", value(1)?),
                (Some(0), Some(1)) => control("tuning", value(1)?),
                _ => Err("MIDI data entry names no registered parameter".to_string()),
            },
            0 | 38 | 91 => Ok(None),
            other => Err(format!(
                "MIDI control change {other} is no sequence command"
            )),
        },
        _ => Ok(None),
    }
}

fn reconstruct_midi_stream(
    events: &[MidiEvent],
    meter: &[(i64, i64)],
) -> Result<Vec<Event>, String> {
    let mut nodes = Vec::<MidiNode>::new();
    let mut grid = Vec::<usize>::new();
    let mut pending = HashMap::<u8, Vec<usize>>::new();
    let mut depth = 0i32;
    let mut bracket_start = 0i64;
    let mut removed = 0i64;
    let mut parameter = (None, None);
    let mut preamble = true;
    let mut sorted = events.to_vec();
    sorted.sort_by_key(|event| (event.tick, event.order));
    for event in sorted {
        let derived = match &event.body {
            EventBody::Meta { meta: 0x51, data } => {
                let micros = match data.as_slice() {
                    [a, b, c] => u32::from_be_bytes([0, *a, *b, *c]),
                    _ => return Err("MIDI tempo is not three bytes".to_string()),
                };
                Some(Event::new("tempo", vec![midi_tempo(micros)?.into()]))
            }
            EventBody::Channel { status, data } if !matches!(status & 0xf0, 0x80 | 0x90) => {
                midi_control(*status, data, &mut parameter, preamble)?
            }
            _ => None,
        };
        if matches!(event.body, EventBody::Meta { .. })
            || matches!(&event.body, EventBody::Channel { status, .. } if status & 0xf0 == 0x90)
        {
            preamble = false;
        }
        if let Some(control) = derived {
            if depth == 0 {
                grid.push(nodes.len());
                nodes.push(MidiNode {
                    compact_tick: event.tick - removed,
                    raw_tick: event.tick,
                    order: event.order,
                    event: control,
                });
            }
            continue;
        }
        match &event.body {
            EventBody::Meta { meta: 0x2f, .. } | EventBody::Meta { meta: 0x51, .. } => continue,
            EventBody::Meta { meta: 0x07, data } => {
                let text =
                    std::str::from_utf8(data).map_err(|_| "MIDI cue is not UTF-8".to_string())?;
                match text {
                    "pattern+" => {
                        if depth == 0 {
                            bracket_start = event.tick;
                        }
                        depth += 1;
                    }
                    "pattern-" => {
                        depth -= 1;
                        if depth < 0 {
                            return Err("MIDI pattern bracket underflow".to_string());
                        }
                        if depth == 0 {
                            removed += event.tick - bracket_start;
                        }
                    }
                    _ => {}
                }
            }
            EventBody::Meta { meta: 0x06, data } => {
                let text = std::str::from_utf8(data)
                    .map_err(|_| "MIDI event marker is not UTF-8".to_string())?;
                let marked = Event::parse(text).map_err(|e| format!("MIDI event marker: {e}"))?;
                if MIDI_CONTROLS.contains(&marked.kind.as_str()) {
                    return Err(format!(
                        "MIDI marker {text:?} spells a control the MIDI's own event carries"
                    ));
                }
                if depth > 0 {
                    continue;
                }
                let index = nodes.len();
                nodes.push(MidiNode {
                    compact_tick: event.tick - removed,
                    raw_tick: event.tick,
                    order: event.order,
                    event: marked,
                });
                grid.push(index);
            }
            EventBody::Channel { status, data }
                if status & 0xf0 == 0x90 || status & 0xf0 == 0x80 =>
            {
                if data.len() < 2 {
                    return Err("MIDI note event is truncated".to_string());
                }
                let key = data[0];
                let note_on = status & 0xf0 == 0x90 && data[1] != 0;
                if note_on {
                    let index = nodes.len();
                    nodes.push(MidiNode {
                        compact_tick: event.tick - removed,
                        raw_tick: event.tick,
                        order: event.order,
                        event: Event::new(
                            "note",
                            vec![0.into(), i64::from(key).into(), i64::from(data[1]).into()],
                        ),
                    });
                    if depth == 0 {
                        grid.push(index);
                    }
                    pending.entry(key).or_default().push(index);
                } else {
                    let queue = pending
                        .get_mut(&key)
                        .ok_or("MIDI note-off has no note-on")?;
                    let index = queue
                        .first()
                        .copied()
                        .ok_or("MIDI note-off has no note-on")?;
                    queue.remove(0);
                    nodes[index].event.args[0] = Arg::Int(event.tick - nodes[index].raw_tick);
                }
            }
            _ => {}
        }
    }
    if pending.values().any(|queue| !queue.is_empty()) {
        return Err("MIDI has an unclosed note-on".to_string());
    }
    if depth != 0 {
        return Err("MIDI pattern bracket is not closed".to_string());
    }
    grid.sort_by_key(|index| (nodes[*index].compact_tick, nodes[*index].order));
    let mut output = Vec::new();
    let mut cursor = 0i64;
    for index in grid {
        let node = &nodes[index];
        if node.compact_tick < cursor {
            return Err("MIDI event order moves backwards".to_string());
        }
        let gap = node.compact_tick - cursor;
        for wait in rest_waits(node.raw_tick - gap, gap, meter)? {
            output.push(Event::new("wait", vec![(wait as i64).into()]));
        }
        cursor = node.compact_tick;
        output.push(node.event.clone());
    }
    Ok(output)
}
fn default_sequence(events: &[Event]) -> Result<Vec<Event>, String> {
    let mut output = Vec::new();
    let mut running: Option<u8> = None;
    let (mut key, mut velocity) = (None::<usize>, None::<usize>);
    let mut after_rest = false;
    for event in events {
        let (kind, args) = (event.kind.as_str(), event.args.as_slice());
        match kind {
            "note" | "note_running" => {
                let duration = number(args.first(), "note duration")?;
                let note_key = number(args.get(1), "note key")?;
                let note_velocity = number(args.get(2), "note velocity")?;
                let opcode = SEQUENCE_DURATIONS
                    .iter()
                    .position(|candidate| *candidate == duration)
                    .map(|index| 0xcf + index as u8);
                let params = if velocity != Some(note_velocity) {
                    vec![note_key, note_velocity]
                } else if key != Some(note_key) {
                    vec![note_key]
                } else {
                    Vec::new()
                };
                let continues =
                    after_rest && opcode.is_some() && running == opcode && !params.is_empty();
                let mut rebuilt = vec![Arg::Int(duration as i64)];
                rebuilt.extend(params.into_iter().map(|value| Arg::Int(value as i64)));
                output.push(Event::new(
                    if continues { "note_running" } else { "note" },
                    rebuilt,
                ));
                running = opcode;
                key = Some(note_key);
                velocity = Some(note_velocity);
            }
            "wait" => output.push(event.clone()),
            "label" | "pattern" | "repeat" | "goto" => {
                output.push(event.clone());
                running = None;
                key = None;
                velocity = None;
            }
            "pattern_end" => {
                output.push(event.clone());
                continue;
            }
            "control_running" | "note_end_running" => {
                return Err(format!(
                    "{kind} spells running status the converter derives"
                ));
            }
            "note_end" => {
                let continues = running == Some(0xce) && args.len() == 1;
                output.push(if continues {
                    Event::new("note_end_running", args.to_vec())
                } else {
                    event.clone()
                });
                running = Some(0xce);
            }
            _ => match sequence_control_opcode(kind)
                .filter(|opcode| sequence_sets_running_status(*opcode))
            {
                Some(opcode) => {
                    let continues = after_rest
                        && running == Some(opcode)
                        && args.len() == 1
                        && sequence_parameter(args.first(), kind)? < 0x80;
                    output.push(if continues {
                        let mut running_args = vec![Arg::Sym(kind.to_string())];
                        running_args.extend(args.iter().cloned());
                        Event::new("control_running", running_args)
                    } else {
                        event.clone()
                    });
                    running = Some(opcode);
                }
                None => output.push(event.clone()),
            },
        }
        after_rest = kind == "wait";
    }
    Ok(output)
}
struct SequenceMidi {
    tracks: HashMap<usize, Vec<MidiEvent>>,
    skeleton: Option<Vec<Segment>>,
    meter: Vec<(i64, i64)>,
}
fn midi_tracks(midi: &[u8]) -> Result<(HashMap<usize, Vec<MidiEvent>>, u16), String> {
    let report = midi_events(midi).map_err(|error| error.to_string())?;
    let mut tracks = HashMap::<usize, Vec<MidiEvent>>::new();
    for event in report.events {
        tracks.entry(event.track).or_default().push(event);
    }
    Ok((tracks, report.ticks_per_quarter))
}
fn read_sequence_midi(midi: &[u8]) -> Result<SequenceMidi, String> {
    let (tracks, division) = midi_tracks(midi)?;
    let conductor = tracks.get(&0).map(Vec::as_slice).unwrap_or(&[]);
    let mut skeleton = None;
    for event in conductor {
        match &event.body {
            EventBody::Meta { meta: 0x7f, data } if data.starts_with(MIDI_BUILD_DIRECTIVE) => {
                return Err("MIDI carries a retired sequence sidecar directive".to_string())
            }
            EventBody::Meta { meta: 0x01, data } if skeleton.is_none() => {
                let text = std::str::from_utf8(data)
                    .map_err(|_| "MIDI conductor skeleton is not UTF-8".to_string())?;
                skeleton = Some(
                    parse_skeleton(text).map_err(|e| format!("MIDI conductor skeleton: {e}"))?,
                );
            }
            _ => {}
        }
    }
    let meter = sequence_meter(conductor, division)?;
    Ok(SequenceMidi {
        tracks,
        skeleton,
        meter,
    })
}
fn read_midi_stream(
    midi: &SequenceMidi,
    track: usize,
    meter: &[(i64, i64)],
) -> Result<Vec<Event>, String> {
    default_sequence(&reconstruct_midi_stream(
        midi.tracks.get(&track).map(Vec::as_slice).unwrap_or(&[]),
        meter,
    )?)
}
pub(crate) fn build_midi_sequence(midi: &[u8]) -> Result<Data, String> {
    let midi = read_sequence_midi(midi)?;
    let skeleton = midi
        .skeleton
        .as_ref()
        .ok_or("MIDI conductor skeleton is missing")?;
    let mut stream_index = 0usize;
    let mut layout = Vec::new();
    for segment in skeleton {
        let Segment::Stream { label, .. } = segment else {
            layout.push(segment.clone());
            continue;
        };
        stream_index += 1;
        layout.push(Segment::Stream {
            label: label.clone(),
            events: read_midi_stream(&midi, stream_index, &midi.meter)?,
        });
    }
    build_sequence(&layout)
}
/// The song header labels a sequence MIDI defines, in its skeleton's order.
pub fn sequence_labels(midi: &[u8]) -> Result<Vec<String>, String> {
    let midi = read_sequence_midi(midi)?;
    let skeleton = midi
        .skeleton
        .as_ref()
        .ok_or("MIDI conductor skeleton is missing")?;
    Ok(skeleton
        .iter()
        .filter_map(|segment| match segment {
            Segment::Header { label, .. } => Some(label.clone()),
            _ => None,
        })
        .collect())
}
pub fn sequence_assembly(midi: &[u8]) -> Result<String, String> {
    Ok(format!(
        "\t.section .rodata\n{}",
        build_midi_sequence(midi)?.source()?
    ))
}
/// How near a half the scaled pitch may come before it is refused. The pitch
/// goes through the host's `pow`, which C libraries round differently in the
/// last place: about 1e-9 at these magnitudes, far inside this margin.
const PITCH_MARGIN: f64 = 1e-6;

/// The native record's pitch: the playback rate in 1/1024 Hz, rounded. A
/// value within PITCH_MARGIN of a half is refused, so that no host can round
/// a sample's pitch the other way and build a different ROM.
fn native_pitch(scaled: f64) -> Result<u32, String> {
    let pitch = scaled.round();
    if !pitch.is_finite() || !(1.0..=f64::from(u32::MAX)).contains(&pitch) {
        return Err("WAV playback pitch exceeds the native wave format".into());
    }
    if ((scaled - scaled.floor()) - 0.5).abs() < PITCH_MARGIN {
        return Err(format!(
            "WAV playback pitch {scaled} lies within {PITCH_MARGIN} of a half, where hosts round it differently; change the sample's pitch fraction"
        ));
    }
    Ok(pitch as u32)
}

pub(crate) fn build_pcm_record(wav: &[u8]) -> Result<Vec<u8>, String> {
    let wave = crate::assets::wav::read_pcm8(wav).map_err(|e| e.to_string())?;
    let last_sample = u32::try_from(
        wave.samples
            .len()
            .checked_sub(1)
            .ok_or("WAV has no samples")?,
    )
    .map_err(|_| "wave sample count exceeds u32")?;
    let frequency = native_pitch(wave.playback_rate(60) * 1024.0)?;
    let (control, loop_start) = if let Some((start, end)) = wave.loop_range {
        if end != last_sample {
            return Err("native PCM requires its forward loop to end at the last sample".into());
        }
        (0x4000_0000u32, start)
    } else {
        (0, 0)
    };
    let size = wave
        .samples
        .len()
        .checked_add(19)
        .ok_or("wave alignment overflows")?
        & !3;
    let mut bytes = Vec::with_capacity(size);
    for value in [control, frequency, loop_start, last_sample] {
        bytes.extend(value.to_le_bytes());
    }
    bytes.extend(&wave.samples);
    bytes.resize(size, 0);
    Ok(bytes)
}

pub fn build_sound_file(name: &str, input: &[u8]) -> Result<Vec<u8>, String> {
    let extension = name.rsplit_once('.').map_or("", |(_, suffix)| suffix);
    if extension.eq_ignore_ascii_case("wav") {
        build_pcm_record(input)
    } else if extension.eq_ignore_ascii_case("pcm4") {
        if input.len() != 16 {
            return Err("a CGB wave RAM pattern is 16 bytes".into());
        }
        Ok(input.to_vec())
    } else {
        Err(format!("{name} is not a sound input"))
    }
}

#[cfg(test)]
mod pitch_tests {
    use super::native_pitch;

    #[test]
    fn a_pitch_is_rounded_unless_a_host_could_round_it_the_other_way() {
        assert_eq!(native_pitch(13_379.0 * 1024.0), Ok(13_700_096));
        assert_eq!(native_pitch(22_050.25), Ok(22_050));
        assert_eq!(native_pitch(22_050.75), Ok(22_051));
        assert_eq!(native_pitch(22_050.499_99), Ok(22_050));
        assert!(native_pitch(22_050.5).unwrap_err().contains("half"));
        assert!(native_pitch(22_050.500_000_1).unwrap_err().contains("half"));
        assert!(native_pitch(22_050.499_999_9).is_err());
        assert!(native_pitch(0.2).unwrap_err().contains("exceeds"));
        assert!(native_pitch(f64::NAN).is_err());
        assert!(native_pitch(5e9).is_err());
    }
}
