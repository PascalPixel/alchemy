//! The sound encoders, as pret's mid2agb and wav2agb: engine sequences from
//! sequence MIDI files, and PCM wave records from WAV files. A sequence's
//! jumps, pattern calls, track list and tone bank are address words naming
//! labels, so the linker places every sequence.
//!
//! A sequence MIDI's conductor track carries the sequence's skeleton as a
//! text event, one segment per line:
//!
//! ```text
//! smsh-sequence 1
//! stream track_1
//! align 4 0
//! header sound_000 block_count=0 priority=0 reverb=178 tone_bank=Voices tracks=track_1
//! ```
//!
//! and each stream track carries the events that are not notes as marker
//! events of one line each, such as `volume 100` or `goto loop_1`: the
//! event's kind, then its integers and labels.
use super::asm::{identifier, Data, Pointer};
use psynergy::assets::midi::{midi_events, EventBody, MidiEvent};
use std::collections::{BTreeSet, HashMap};
use std::fmt;

/// One argument of a sequence event: an integer or a label.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Arg {
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

/// One sequence event: its kind and arguments, written `kind arg arg`.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Event {
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
    /// Read an event from its one-line text.
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

/// One segment of a sequence's layout, in order.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Segment {
    /// A track's events under its label.
    Stream { label: String, events: Vec<Event> },
    /// Padding to a power-of-two boundary with a fill byte.
    Align { boundary: i64, fill: i64 },
    /// The song header: its exported label, settings, tone bank and tracks.
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

/// A skeleton's text: the engine line, then one line per segment. Streams
/// carry only their labels; their events live in their tracks.
pub fn skeleton_text(segments: &[Segment]) -> String {
    let mut text = format!("{SKELETON_ENGINE}\n");
    for segment in segments {
        text += &match segment {
            Segment::Stream { label, .. } => format!("stream {label}\n"),
            Segment::Align { boundary, fill } => format!("align {boundary} {fill}\n"),
            Segment::Header {
                label,
                block_count,
                priority,
                reverb,
                tone_bank,
                tracks,
            } => format!(
                "header {label} block_count={block_count} priority={priority} reverb={reverb} tone_bank={tone_bank} tracks={}\n",
                tracks.join(",")
            ),
        };
    }
    text
}

/// Read a skeleton's text; streams come back without events.
pub fn parse_skeleton(text: &str) -> Result<Vec<Segment>, String> {
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
/// An engine sequence from its layout: `header`, `stream` and `align`
/// segments in order. The header's label is exported; the tone bank it names
/// is linked from elsewhere, and every other symbol (tracks, jump, repeat and
/// pattern targets) must be a label the sequence defines.
pub fn build_sequence(layout: &[Segment]) -> Result<Data, String> {
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
#[cfg(test)]
fn events(lines: &[&str]) -> Vec<Event> {
    lines
        .iter()
        .map(|line| Event::parse(line).unwrap())
        .collect()
}
#[cfg(test)]
fn song(track_events: Vec<Event>) -> Vec<Segment> {
    vec![
        Segment::Header {
            label: "song".into(),
            block_count: 0,
            priority: 1,
            reverb: 0,
            tone_bank: "voicegroup".into(),
            tracks: vec!["track".into()],
        },
        Segment::Align {
            boundary: 16,
            fill: 0,
        },
        Segment::Stream {
            label: "track".into(),
            events: track_events,
        },
    ]
}
#[test]
fn sequences_link_their_tracks_jumps_and_tone_bank_by_label() {
    let track = events(&[
        "goto end",
        "label loop",
        "note 1 60 100",
        "wait 1",
        "repeat 2 loop",
        "label end",
        "pattern loop",
        "fine",
    ]);
    let data = build_sequence(&song(track.clone())).unwrap();
    assert_eq!(
        data.bytes,
        [
            1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xb2, 0, 0, 0, 0, 0xd0, 60, 100, 0x81,
            0xb5, 2, 0, 0, 0, 0, 0xb3, 0, 0, 0, 0, 0xb1,
        ]
    );
    let sites: Vec<(usize, &str)> = data
        .pointers
        .iter()
        .map(|(site, pointer)| (*site, pointer.symbol.as_str()))
        .collect();
    assert_eq!(
        sites,
        [
            (4, "voicegroup"),
            (8, "track"),
            (17, "end"),
            (27, "loop"),
            (32, "loop")
        ]
    );
    assert_eq!(data.align, 16);
    let text = data.source().unwrap();
    assert!(
        text.starts_with("\t.balign 16\n\t.global song\nsong:\n"),
        "{text}"
    );
    assert!(text.contains("\ntrack:\n") && !text.contains(".global track"));
    let mut missing = song(track.clone());
    if let Segment::Stream { events, .. } = &mut missing[2] {
        events[0] = Event::parse("goto missing").unwrap();
    }
    assert!(build_sequence(&missing)
        .unwrap_err()
        .contains("unknown sequence symbol"));
    let mut duplicate = song(track.clone());
    if let Segment::Stream { label, .. } = &mut duplicate[2] {
        *label = "song".into();
    }
    assert!(build_sequence(&duplicate)
        .unwrap_err()
        .contains("duplicate local label"));
    for (edit, error) in [
        ((1i64, 1i64, 0i64, 1usize), "nonzero"),
        ((0, 256, 0, 1), "header value"),
        ((0, 1, 256, 1), "header value"),
        ((0, 1, 0, 0), "track list"),
    ] {
        let mut invalid = song(track.clone());
        if let Segment::Header {
            block_count,
            priority,
            reverb,
            tracks,
            ..
        } = &mut invalid[0]
        {
            (*block_count, *priority, *reverb) = (edit.0, edit.1, edit.2);
            tracks.truncate(edit.3);
        }
        assert!(
            build_sequence(&invalid).unwrap_err().contains(error),
            "{error}"
        );
    }
    for (boundary, fill, error) in [(3, 0, "power of two"), (16, 256, "alignment fill")] {
        let mut invalid = song(track.clone());
        invalid[1] = Segment::Align { boundary, fill };
        assert!(
            build_sequence(&invalid).unwrap_err().contains(error),
            "{error}"
        );
    }
}
#[test]
fn skeletons_and_events_read_back_their_text() {
    let layout = song(Vec::new());
    let text = skeleton_text(&layout);
    assert_eq!(
        text,
        "smsh-sequence 1\nheader song block_count=0 priority=1 reverb=0 tone_bank=voicegroup tracks=track\nalign 16 0\nstream track\n"
    );
    assert_eq!(parse_skeleton(&text).unwrap(), layout);
    assert!(parse_skeleton("{\"format\":1}").is_err());
    assert!(
        parse_skeleton(&text.replace("reverb=0", "reverb=0 base=0x08000000"))
            .unwrap_err()
            .contains("records base")
    );
    for line in ["volume 100", "goto loop_1", "note_end", "key_shift -1"] {
        assert_eq!(Event::parse(line).unwrap().to_string(), line);
    }
    assert!(Event::parse("[\"fine\"]").is_err());
    assert!(Event::parse("volume 1.5").is_err());
}
#[test]
fn running_status_follows_the_engine_and_tempo_never_runs() {
    let encoded = encode_sequence_stream(&events(&[
        "priority 5",
        "control_running priority 6",
        "key_shift -1",
        "control_running key_shift 1",
        "tempo 30",
    ]))
    .unwrap();
    assert_eq!(encoded.data, [0xba, 5, 6, 0xbc, 0xff, 1, 0xbb, 30]);
    assert!(encode_sequence_stream(&events(&["tempo 30", "control_running tempo 31"])).is_err());
}
/// Prefix of the retired per-event sequence sidecars. The converter derives
/// every encoding choice, so a MIDI still carrying one is refused.
const MIDI_BUILD_DIRECTIVE: &[u8] = b"alchemy-mid2agb\0";
/// Bar lengths, in sequence ticks, tried in order when a playback MIDI is
/// adopted: four, three, two and one 24-tick beats, then one, seven, five and
/// three 12-tick half beats.
const ADOPTION_BARS: [i64; 8] = [96, 72, 48, 24, 12, 84, 60, 36];
/// Bar lines of a sequence MIDI: every time signature in the conductor track
/// starts bars of its length at its tick; before the first one, MIDI's 4/4.
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
/// The time signature whose bars last `bar` sequence ticks, counted in 24-tick
/// beats, or in 12-tick half beats when the bar is not whole beats.
fn time_signature(bar: i64, division: u16) -> Result<[u8; 4], String> {
    let whole = i64::from(division) * 4;
    for unit in [24, 12] {
        let note = whole / unit;
        if bar % unit == 0 && note * unit == whole && note.count_ones() == 1 && bar / unit <= 0xff {
            return Ok([(bar / unit) as u8, note.trailing_zeros() as u8, 24, 8]);
        }
    }
    Err(format!(
        "a {bar}-tick bar has no time signature at {division} ticks per quarter"
    ))
}
/// A rest as the converter writes it: cut at every bar line, then into the
/// longest wait commands within each bar.
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
    let mut sorted = events.to_vec();
    sorted.sort_by_key(|event| (event.tick, event.order));
    for event in sorted {
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
                if depth > 0 {
                    continue;
                }
                let text = std::str::from_utf8(data)
                    .map_err(|_| "MIDI event marker is not UTF-8".to_string())?;
                let index = nodes.len();
                nodes.push(MidiNode {
                    compact_tick: event.tick - removed,
                    raw_tick: event.tick,
                    order: event.order,
                    event: Event::parse(text).map_err(|e| format!("MIDI event marker: {e}"))?,
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
        // The rest ends at this event in played time; pattern calls before it
        // have already advanced the bar position.
        let gap = node.compact_tick - cursor;
        for wait in rest_waits(node.raw_tick - gap, gap, meter)? {
            output.push(Event::new("wait", vec![(wait as i64).into()]));
        }
        cursor = node.compact_tick;
        output.push(node.event.clone());
    }
    Ok(output)
}
/// The converter's encoding of a reconstructed stream. A note, or a control
/// that sets running status, continues the running command only as the first
/// event after a rest; chords and events after other commands repeat it. An
/// end of tie that names a key continues a running end of tie. Key and
/// velocity are written when they change. A label, pattern call, repeat or
/// jump forgets the running command, key and velocity, because another path
/// reaches the event after it; a pattern end changes nothing.
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
fn midi_variable(mut value: usize) -> Vec<u8> {
    let mut bytes = vec![(value & 0x7f) as u8];
    while {
        value >>= 7;
        value != 0
    } {
        bytes.push(((value & 0x7f) as u8) | 0x80);
    }
    bytes.reverse();
    bytes
}
fn encode_midi_track(events: &[MidiEvent]) -> Result<Vec<u8>, String> {
    let mut output = Vec::new();
    let mut tick = 0i64;
    for event in events {
        if matches!(event.body, EventBody::Meta { meta: 0x2f, .. }) {
            continue;
        }
        let delta = event
            .tick
            .checked_sub(tick)
            .filter(|delta| *delta >= 0)
            .ok_or("MIDI event order moves backwards")? as usize;
        output.extend(midi_variable(delta));
        tick = event.tick;
        match &event.body {
            EventBody::Meta { meta, data } => {
                output.extend([0xff, *meta]);
                output.extend(midi_variable(data.len()));
                output.extend(data);
            }
            EventBody::Sysex { status, data } => {
                output.push(*status);
                output.extend(midi_variable(data.len()));
                output.extend(data);
            }
            EventBody::Channel { status, data } => {
                output.push(*status);
                output.extend(data);
            }
        }
    }
    output.extend([0, 0xff, 0x2f, 0]);
    Ok(output)
}
fn repack_midi_tracks(midi: &[u8], native_tracks: usize) -> Result<Vec<u8>, String> {
    let report = midi_events(midi).map_err(|error| error.to_string())?;
    if report.format == 1 && usize::from(report.tracks) == native_tracks + 1 {
        return Ok(midi.to_vec());
    }
    if report.format != 0 || report.tracks != 1 {
        return Err("playback MIDI is neither canonical format 1 nor convertible format 0".into());
    }
    let mut tracks = vec![Vec::<MidiEvent>::new(); native_tracks + 1];
    for event in report.events {
        let destination = match &event.body {
            EventBody::Channel { status, .. } => usize::from(status & 0x0f) + 1,
            _ => 0,
        };
        if destination >= tracks.len() {
            return Err(format!(
                "playback MIDI channel {} exceeds native track count",
                destination
            ));
        }
        tracks[destination].push(event);
    }
    let count = u16::try_from(tracks.len()).map_err(|_| "too many MIDI tracks")?;
    let mut output = Vec::new();
    output.extend_from_slice(b"MThd");
    output.extend_from_slice(&6u32.to_be_bytes());
    output.extend_from_slice(&1u16.to_be_bytes());
    output.extend_from_slice(&count.to_be_bytes());
    output.extend_from_slice(&report.ticks_per_quarter.to_be_bytes());
    for (track_index, mut events) in tracks.into_iter().enumerate() {
        if track_index != 0 {
            let mut active = HashMap::<u8, usize>::new();
            let mut last_tick = 0i64;
            for event in &events {
                last_tick = last_tick.max(event.tick);
                if let EventBody::Channel { status, data } = &event.body {
                    let note = data.first().copied().unwrap_or(0);
                    if status & 0xf0 == 0x90 && data.get(1).copied().unwrap_or(0) != 0 {
                        *active.entry(note).or_default() += 1;
                    } else if status & 0xf0 == 0x80
                        || (status & 0xf0 == 0x90 && data.get(1) == Some(&0))
                    {
                        if let Some(count) = active.get_mut(&note).filter(|count| **count != 0) {
                            *count -= 1;
                        }
                    }
                }
            }
            let channel = u8::try_from(track_index - 1).map_err(|_| "too many MIDI channels")?;
            for (note, count) in active {
                for order in 0..count {
                    events.push(MidiEvent {
                        tick: last_tick,
                        track: track_index,
                        order: usize::MAX - order,
                        body: EventBody::Channel {
                            status: 0x80 | channel,
                            data: vec![note, 0],
                        },
                    });
                }
            }
        }
        events.sort_by_key(|event| (event.tick, event.order));
        let data = encode_midi_track(&events)?;
        output.extend_from_slice(b"MTrk");
        output.extend_from_slice(
            &u32::try_from(data.len())
                .map_err(|_| "MIDI track is too large")?
                .to_be_bytes(),
        );
        output.extend(data);
    }
    Ok(output)
}
/// A sequence MIDI's events by track, its stream skeleton and its bar lines.
struct SequenceMidi {
    tracks: HashMap<usize, Vec<MidiEvent>>,
    skeleton: Option<Vec<Segment>>,
    meter: Vec<(i64, i64)>,
}
/// A MIDI file's events by track and its ticks per quarter note.
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
/// The converter's reading of one MIDI stream track under the given bar lines.
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
/// The engine sequence a sequence MIDI encodes, as mid2agb converts one: its
/// conductor track carries the layout skeleton and bar lines, and track `n`
/// holds the events of the skeleton's `n`th stream.
pub fn build_midi_sequence(midi: &[u8]) -> Result<Data, String> {
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
/// The assembler source `build rom` assembles for a sequence MIDI, as pret's
/// mid2agb writes one song's: the sequence in `.rodata`, its header label
/// exported and every address word a label the linker resolves.
pub fn sequence_assembly(midi: &[u8]) -> Result<String, String> {
    Ok(format!(
        "\t.section .rodata\n{}",
        build_midi_sequence(midi)?.source()?
    ))
}
/// Turn a playback MIDI into the sequence MIDI of a sequence layout. Its
/// conductor becomes the layout's skeleton and the first time signature, in
/// `ADOPTION_BARS` order, under which the converter reads every stream of the
/// layout back exactly; without one, adoption refuses and names the first
/// difference. Nothing here reads a ROM: both inputs are editable.
pub fn adopt_midi(layout: &[Segment], midi: &[u8]) -> Result<Vec<u8>, String> {
    let mut streams = Vec::new();
    let mut skeleton = Vec::new();
    for segment in layout {
        if let Segment::Stream { label, events } = segment {
            streams.push((label.as_str(), events));
            skeleton.push(Segment::Stream {
                label: label.clone(),
                events: Vec::new(),
            });
        } else {
            skeleton.push(segment.clone());
        }
    }
    let midi = repack_midi_tracks(midi, streams.len())?;
    // The conductor is replaced, so nothing it carried is read.
    let (tracks, division) = midi_tracks(&midi)?;
    if tracks.keys().copied().max().unwrap_or(0) != streams.len() {
        return Err("MIDI and native sequence track counts differ".to_string());
    }
    let playback = SequenceMidi {
        tracks,
        skeleton: None,
        meter: Vec::new(),
    };
    let mut first_difference = None;
    let mut adopted = None;
    'bars: for bar in ADOPTION_BARS {
        for (index, (label, native)) in streams.iter().enumerate() {
            let read = read_midi_stream(&playback, index + 1, &[(0, bar)])?;
            if read != **native {
                if first_difference.is_none() {
                    let at = read
                        .iter()
                        .zip(native.iter())
                        .take_while(|(a, b)| a == b)
                        .count();
                    let show = |event: Option<&Event>| {
                        event.map_or("nothing".to_string(), Event::to_string)
                    };
                    first_difference = Some(format!(
                        "{label} event {at}: native {} but MIDI reads {}",
                        show(native.get(at)),
                        show(read.get(at))
                    ));
                }
                continue 'bars;
            }
        }
        adopted = Some(bar);
        break;
    }
    let bar = adopted.ok_or_else(|| {
        format!(
            "no time signature reproduces the native sequence; {}",
            first_difference.unwrap_or_default()
        )
    })?;
    let conductor = encode_midi_track(&[
        MidiEvent {
            tick: 0,
            track: 0,
            order: 0,
            body: EventBody::Meta {
                meta: 0x01,
                data: skeleton_text(&skeleton).into_bytes(),
            },
        },
        MidiEvent {
            tick: 0,
            track: 0,
            order: 1,
            body: EventBody::Meta {
                meta: 0x58,
                data: time_signature(bar, division)?.to_vec(),
            },
        },
    ])?;
    let header = 8 + u32::from_be_bytes(midi[4..8].try_into().map_err(|_| "MIDI header")?) as usize;
    let old_conductor = midi
        .get(header + 4..header + 8)
        .map(|size| u32::from_be_bytes(size.try_into().unwrap()) as usize)
        .ok_or("MIDI conductor track is missing")?;
    let mut output = midi[..header].to_vec();
    output.extend_from_slice(b"MTrk");
    output.extend_from_slice(
        &u32::try_from(conductor.len())
            .map_err(|_| "MIDI conductor is too large")?
            .to_be_bytes(),
    );
    output.extend(conductor);
    output.extend_from_slice(
        midi.get(header + 8 + old_conductor..)
            .ok_or("MIDI conductor track is truncated")?,
    );
    Ok(output)
}
#[cfg(test)]
fn marker(tick: i64, order: usize, text: &str) -> MidiEvent {
    MidiEvent {
        tick,
        track: 1,
        order,
        body: EventBody::Meta {
            meta: 0x06,
            data: text.as_bytes().to_vec(),
        },
    }
}
#[test]
fn sequence_midi_reading_follows_bar_lines_time_slots_and_jump_targets() {
    let note = |tick, order, status: u8, key| MidiEvent {
        tick,
        track: 1,
        order,
        body: EventBody::Channel {
            status,
            data: vec![key, 100],
        },
    };
    let stream = [
        note(0, 0, 0x90, 60),
        note(0, 1, 0x90, 64),
        note(5, 2, 0x80, 60),
        note(5, 3, 0x80, 64),
        note(12, 4, 0x90, 67),
        note(17, 5, 0x80, 67),
        marker(24, 6, "label loop"),
        note(24, 7, 0x90, 67),
        note(29, 8, 0x80, 67),
        marker(200, 9, "goto loop"),
    ];
    let read =
        |bar| default_sequence(&reconstruct_midi_stream(&stream, &[(0, bar)]).unwrap()).unwrap();
    let with_rest = |waits: &[i64]| {
        let mut expected = events(&[
            "note 5 60 100",
            "note 5 64",
            "wait 12",
            "note_running 5 67",
            "wait 12",
            "label loop",
            "note 5 67 100",
        ]);
        expected.extend(
            waits
                .iter()
                .map(|wait| Event::new("wait", vec![(*wait).into()])),
        );
        expected.push(Event::parse("goto loop").unwrap());
        expected
    };
    assert_eq!(read(96), with_rest(&[72, 96, 8]));
    assert_eq!(read(72), with_rest(&[48, 72, 56]));
    let signature = |numerator: u8, power: u8| MidiEvent {
        tick: 0,
        track: 0,
        order: 0,
        body: EventBody::Meta {
            meta: 0x58,
            data: vec![numerator, power, 24, 8],
        },
    };
    assert_eq!(sequence_meter(&[signature(3, 4)], 96).unwrap(), [(0, 72)]);
    assert_eq!(sequence_meter(&[], 96).unwrap(), [(0, 384)]);
    assert!(sequence_meter(&[signature(1, 5)], 100).is_err());
    assert_eq!(time_signature(96, 96).unwrap(), [4, 4, 24, 8]);
    assert_eq!(time_signature(84, 96).unwrap(), [7, 5, 24, 8]);
    assert_eq!(time_signature(12, 96).unwrap(), [1, 5, 24, 8]);
}
#[test]
fn sequence_midi_reading_derives_control_running_status() {
    let stream = [
        marker(0, 0, "volume 80"),
        marker(12, 1, "volume 90"),
        marker(12, 2, "pan 64"),
        marker(12, 3, "pan 60"),
        marker(24, 4, "note_end 60"),
        marker(24, 5, "note_end 64"),
        marker(36, 6, "note_end 62"),
        marker(36, 7, "note_end"),
        marker(48, 8, "fine"),
    ];
    let read = default_sequence(&reconstruct_midi_stream(&stream, &[(0, 96)]).unwrap()).unwrap();
    assert_eq!(
        read,
        events(&[
            "volume 80",
            "wait 12",
            "control_running volume 90",
            "pan 64",
            "pan 60",
            "wait 12",
            "note_end 60",
            "note_end_running 64",
            "wait 12",
            "note_end_running 62",
            "note_end",
            "wait 12",
            "fine",
        ])
    );
    let spelled = [marker(0, 0, "control_running volume 1")];
    assert!(
        default_sequence(&reconstruct_midi_stream(&spelled, &[(0, 96)]).unwrap())
            .unwrap_err()
            .contains("derives")
    );
    assert!(reconstruct_midi_stream(&[marker(0, 0, "[\"fine\"]")], &[(0, 96)]).is_err());
}
#[cfg(test)]
fn track_chunk(events: &[MidiEvent]) -> Vec<u8> {
    let data = encode_midi_track(events).unwrap();
    [
        b"MTrk".as_slice(),
        &(data.len() as u32).to_be_bytes(),
        &data,
    ]
    .concat()
}
#[test]
fn midi_adoption_records_the_meter_and_refuses_unreproducible_streams() {
    let at = |tick, order, body| MidiEvent {
        tick,
        track: 1,
        order,
        body,
    };
    let playback = [
        b"MThd\0\0\0\x06\0\x01\0\x02\0\x60".as_slice(),
        &track_chunk(&[]),
        &track_chunk(&[
            at(
                0,
                0,
                EventBody::Channel {
                    status: 0x90,
                    data: vec![60, 100],
                },
            ),
            at(
                5,
                1,
                EventBody::Channel {
                    status: 0x80,
                    data: vec![60, 64],
                },
            ),
            at(
                96,
                2,
                EventBody::Meta {
                    meta: 0x06,
                    data: b"fine".to_vec(),
                },
            ),
        ]),
    ]
    .concat();
    let layout = |waits: &[i64]| {
        let mut stream = events(&["note 5 60 100"]);
        stream.extend(
            waits
                .iter()
                .map(|wait| Event::new("wait", vec![(*wait).into()])),
        );
        stream.push(Event::parse("fine").unwrap());
        vec![Segment::Stream {
            label: "track_1".into(),
            events: stream,
        }]
    };
    let adopted_midi = adopt_midi(&layout(&[72, 24]), &playback).unwrap();
    // The adopted MIDI builds exactly the sequence its layout describes.
    assert_eq!(
        build_midi_sequence(&adopted_midi).unwrap(),
        build_sequence(&layout(&[72, 24])).unwrap()
    );
    let adopted = read_sequence_midi(&adopted_midi).unwrap();
    assert_eq!(adopted.meter, [(0, 72)]);
    assert_eq!(
        adopted.skeleton.unwrap(),
        [Segment::Stream {
            label: "track_1".into(),
            events: Vec::new()
        }]
    );
    let refusal = adopt_midi(&layout(&[50, 46]), &playback).unwrap_err();
    assert!(refusal.contains("no time signature"), "{refusal}");
    let directive = [MIDI_BUILD_DIRECTIVE, b"{}"].concat();
    let retired = [
        b"MThd\0\0\0\x06\0\x01\0\x01\0\x60".as_slice(),
        &track_chunk(&[MidiEvent {
            tick: 0,
            track: 0,
            order: 0,
            body: EventBody::Meta {
                meta: 0x7f,
                data: directive,
            },
        }]),
    ]
    .concat();
    assert!(read_sequence_midi(&retired).is_err());
}

pub fn build_pcm_record(wav: &[u8]) -> Result<Vec<u8>, String> {
    let wave = psynergy::assets::wav::read_pcm8(wav).map_err(|e| e.to_string())?;
    let last_sample = u32::try_from(
        wave.samples
            .len()
            .checked_sub(1)
            .ok_or("WAV has no samples")?,
    )
    .map_err(|_| "wave sample count exceeds u32")?;
    let pitch = (wave.playback_rate(60) * 1024.0).round();
    if !pitch.is_finite() || !(1.0..=f64::from(u32::MAX)).contains(&pitch) {
        return Err("WAV playback pitch exceeds the native wave format".into());
    }
    let frequency = pitch as u32;
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

/// The bytes a sound data source reads for one editable input, as pret's
/// wav2agb builds `.bin` files from `.wav`: a WAV's PCM wave record, or the
/// sixteen bytes of a `.PCM4` CGB wave RAM pattern (32 four-bit samples).
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

#[test]
fn sound_files_build_from_wav_and_pcm4_inputs_only() {
    use psynergy::assets::wav::{write_pcm8, Pcm8};
    let wave = write_pcm8(&Pcm8 {
        rate: 8_000,
        samples: vec![1, 2, 3],
        unity_note: 60,
        pitch_fraction: 0,
        loop_range: None,
    })
    .unwrap();
    assert_eq!(
        build_sound_file("SAMPLE/X.PCM8.WAV", &wave).unwrap(),
        build_pcm_record(&wave).unwrap()
    );
    assert_eq!(
        build_sound_file("SAMPLE/X.PCM4", &[0x5a; 16]).unwrap(),
        [0x5a; 16]
    );
    assert!(build_sound_file("SAMPLE/X.PCM4", &[0; 15]).is_err());
    assert!(build_sound_file("SAMPLE/X.BIN", &[0; 16]).is_err());
}

#[test]
fn sequence_assembly_places_the_song_in_rodata_by_label() {
    let source = vec![
        Segment::Stream {
            label: "track_1".into(),
            events: events(&["fine"]),
        },
        Segment::Align {
            boundary: 4,
            fill: 0,
        },
        Segment::Header {
            label: "sound_001".into(),
            block_count: 0,
            priority: 0,
            reverb: 0,
            tone_bank: "Voices".into(),
            tracks: vec!["track_1".into()],
        },
    ];
    let track = |events: &[MidiEvent]| {
        let data = encode_midi_track(events).unwrap();
        [
            b"MTrk".as_slice(),
            &(data.len() as u32).to_be_bytes(),
            &data,
        ]
        .concat()
    };
    let fine = MidiEvent {
        tick: 0,
        track: 1,
        order: 0,
        body: EventBody::Meta {
            meta: 0x06,
            data: b"fine".to_vec(),
        },
    };
    let playback = [
        b"MThd\0\0\0\x06\0\x01\0\x02\0\x60".as_slice(),
        &track(&[]),
        &track(&[fine]),
    ]
    .concat();
    let midi = adopt_midi(&source, &playback).unwrap();
    let text = sequence_assembly(&midi).unwrap();
    assert!(
        text.starts_with("\t.section .rodata\n\t.balign 4\n"),
        "{text}"
    );
    assert!(text.contains("\t.global sound_001\nsound_001:\n"), "{text}");
    assert!(
        text.contains("\t.4byte Voices\n\t.4byte track_1\n"),
        "{text}"
    );
}

#[test]
fn pcm_records_derive_header_pitch_and_alignment_from_the_wav() {
    use psynergy::assets::wav::{write_pcm8, Pcm8};
    let mut wave = Pcm8 {
        rate: 8_000,
        samples: vec![128, 0, 127],
        unity_note: 48,
        pitch_fraction: 0,
        loop_range: Some((1, 2)),
    };
    let built = build_pcm_record(&write_pcm8(&wave).unwrap()).unwrap();
    let mut expected = Vec::new();
    for value in [0x4000_0000u32, 16_384_000, 1, 2] {
        expected.extend(value.to_le_bytes());
    }
    expected.extend([128, 0, 127, 0]);
    assert_eq!(built, expected);
    wave.loop_range = Some((1, 1));
    assert!(build_pcm_record(&write_pcm8(&wave).unwrap()).is_err());
    wave.loop_range = None;
    let built = build_pcm_record(&write_pcm8(&wave).unwrap()).unwrap();
    assert_eq!(&built[..4], &[0; 4]);
    wave.samples.clear();
    assert!(build_pcm_record(&write_pcm8(&wave).unwrap()).is_err());
}
