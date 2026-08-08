// Why this exists: the decompilation loop needs a cheap, honest answer to "how
// close is this build to the reference ROM?" that survives code motion. A plain
// same-offset byte diff collapses to noise the moment a function changes size,
// so every downstream byte moves and the number stops tracking real progress.
// This tool reports both halves: the same-offset run census (what is still
// byte-identical in place) and a relocated-block census (what matched after
// shifting), plus an optional Thumb-aware normalization pass that neutralizes
// BL long-call displacements and ROM-pointer literals so relocated code stops
// reading as different merely because its addresses moved.
//
// Ported from tools/metrics/compare_roms.ts. The JSON report is tracked output
// consumed by other metrics tooling, so key order and number spelling are
// load-bearing and reproduced exactly; see the PORT NOTEs below.

use serde_json::{Map, Value};

pub const DEFAULT_BASE: u64 = 0x0800_0000;
pub const DEFAULT_MAXIMUM_OCCURRENCES: usize = 16;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct NormalizationRange {
    pub start: usize,
    pub end: usize,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct RelativeSpan {
    pub reference: usize,
    pub candidate: usize,
    pub size: usize,
    /// `undefined` in the TypeScript shape; every read site spells it
    /// `maximum_occurrences ?? 1`, so `None` and `Some(1)` are not
    /// interchangeable during sorting and are kept distinct here.
    pub maximum_occurrences: Option<usize>,
}

#[derive(Debug, Clone)]
pub struct RelocatedMatchAnalysis {
    pub spans: Vec<RelativeSpan>,
    pub raw_span_bytes: usize,
    pub unique_matched_bytes: usize,
    pub seed_count: usize,
    pub ambiguous_seed_count: usize,
    pub repetitive_seed_count: usize,
}

#[derive(Debug, Clone)]
pub struct ThumbNormalization {
    pub data: Vec<u8>,
    pub long_calls: usize,
    pub rom_pointer_literals: usize,
    pub scanned_bytes: usize,
}

#[derive(Debug, Clone)]
pub struct SameOffsetRuns {
    pub compared: usize,
    pub matching: usize,
    pub runs: Vec<RelativeSpan>,
}

#[derive(Debug, Clone, Default)]
pub struct ThumbRanges {
    pub reference: Option<Vec<NormalizationRange>>,
    pub candidate: Option<Vec<NormalizationRange>>,
}

// ---------------------------------------------------------------------------
// JavaScript numeric semantics
// ---------------------------------------------------------------------------

/// PORT NOTE: `String.prototype.trim` strips the ECMAScript WhiteSpace and
/// LineTerminator sets, which are not Rust's `char::is_whitespace`: JS includes
/// U+FEFF (ZWNBSP) and excludes U+0085 (NEL), while Rust's White_Space property
/// is the exact reverse. `Number(value)` trims with the JS set, so this does.
fn is_js_space(character: char) -> bool {
    matches!(
        character,
        '\u{0009}'
            | '\u{000b}'
            | '\u{000c}'
            | '\u{0020}'
            | '\u{00a0}'
            | '\u{feff}'
            | '\u{000a}'
            | '\u{000d}'
            | '\u{2028}'
            | '\u{2029}'
    ) || (character != '\u{0085}' && character.is_whitespace())
}

/// PORT NOTE: implements the ECMAScript `StringNumericLiteral` grammar used by
/// `Number(string)`, which is not `f64::from_str`. The differences that reach a
/// *successful* parse are the radix prefixes (`Number("0x10")` is 16, where
/// `f64::from_str` fails) and the empty/whitespace-only string (`Number("")` is
/// 0, where `f64::from_str` fails). Spellings Rust accepts and JS does not —
/// `"inf"`, `"NaN"`, `"infinity"` — are rejected here explicitly; they would
/// otherwise parse to a non-finite value which the caller rejects anyway, but
/// making it explicit keeps the divergence out of the failure message.
pub fn js_number(value: &str) -> f64 {
    let text = value.trim_matches(is_js_space);
    if text.is_empty() {
        return 0.0;
    }
    if let Some(digits) = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")) {
        return radix_literal(digits, 16);
    }
    if let Some(digits) = text.strip_prefix("0o").or_else(|| text.strip_prefix("0O")) {
        return radix_literal(digits, 8);
    }
    if let Some(digits) = text.strip_prefix("0b").or_else(|| text.strip_prefix("0B")) {
        return radix_literal(digits, 2);
    }
    let (sign, body) = match text.as_bytes()[0] {
        b'+' => (1.0, &text[1..]),
        b'-' => (-1.0, &text[1..]),
        _ => (1.0, text),
    };
    if body == "Infinity" {
        return sign * f64::INFINITY;
    }
    // Reject the Rust-only spellings before handing the rest to `from_str`.
    if body.is_empty()
        || body
            .bytes()
            .any(|byte| !matches!(byte, b'0'..=b'9' | b'.' | b'e' | b'E' | b'+' | b'-'))
    {
        return f64::NAN;
    }
    body.parse::<f64>().map(|parsed| sign * parsed).unwrap_or(f64::NAN)
}

fn radix_literal(digits: &str, radix: u32) -> f64 {
    if digits.is_empty() {
        return f64::NAN;
    }
    let mut result = 0.0f64;
    for character in digits.chars() {
        match character.to_digit(radix) {
            Some(digit) => result = result * f64::from(radix) + f64::from(digit),
            None => return f64::NAN,
        }
    }
    result
}

/// `Number.isSafeInteger`. PORT NOTE: JavaScript has one number type, so `1.0`
/// and `1` are the same value and both pass. Any parse that produced a
/// fractional part, a NaN, or a magnitude above 2^53-1 fails.
pub fn is_safe_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0 && value.abs() <= 9_007_199_254_740_991.0
}

/// PORT NOTE: ECMAScript `ToString(Number)` (spec 6.1.6.1.20), which is *not*
/// Rust's `Display` or serde_json's ryu output. Two divergences reach the
/// tracked report through the `ratio` field: JS writes an integral double as
/// `1` where Rust writes `1.0`, and JS switches to exponential notation below
/// 1e-6 (`5.960464477539063e-8`) where Rust never does. Both are reachable —
/// identical ROMs give ratio 1, and a single matching byte in a 32 MiB image
/// gives a ratio under 1e-6.
pub fn js_number_string(value: f64) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if value == 0.0 {
        return "0".to_string();
    }
    if value < 0.0 {
        return format!("-{}", js_number_string(-value));
    }
    if value.is_infinite() {
        return "Infinity".to_string();
    }
    // Rust's `{:e}` yields the shortest round-tripping digit string, which is
    // exactly the `s`/`k`/`n` triple the spec asks for.
    let exponential = format!("{value:e}");
    let (mantissa, exponent) = exponential.split_once('e').expect("{:e} always emits an exponent");
    let digits: String = mantissa.chars().filter(|character| *character != '.').collect();
    let digits = digits.trim_end_matches('0');
    let digits = if digits.is_empty() { "0" } else { digits };
    let k = digits.len() as i32;
    let n = exponent.parse::<i32>().expect("{:e} always emits an integer exponent") + 1;

    if k <= n && n <= 21 {
        return format!("{digits}{}", "0".repeat((n - k) as usize));
    }
    if 0 < n && n <= 21 {
        return format!("{}.{}", &digits[..n as usize], &digits[n as usize..]);
    }
    if -6 < n && n <= 0 {
        return format!("0.{}{digits}", "0".repeat((-n) as usize));
    }
    let tail = if n > 0 {
        format!("e+{}", n - 1)
    } else {
        format!("e-{}", 1 - n)
    };
    if k == 1 {
        return format!("{digits}{tail}");
    }
    format!("{}.{}{tail}", &digits[..1], &digits[1..])
}

// ---------------------------------------------------------------------------
// Primitive helpers
// ---------------------------------------------------------------------------

fn integer(value: &str, label: &str) -> Result<i64, String> {
    let result = js_number(value);
    if !is_safe_integer(result) {
        return Err(format!("{label} must be an integer"));
    }
    Ok(result as i64)
}

fn basename(path: &str) -> String {
    let normalized = path.replace('\\', "/");
    // PORT NOTE: `"".split("/").at(-1)` is `""`, not undefined, so the `?? path`
    // fallback in the original is unreachable and is not reproduced.
    normalized.rsplit('/').next().unwrap_or(&normalized).to_string()
}

fn hexadecimal(value: usize, base: u64) -> String {
    format!("0x{:08x}", value as u64 + base)
}

/// FNV-1a over a block. PORT NOTE: `Math.imul` is a u32 wrapping multiply and
/// `>>> 0` is a u32 coercion, so the whole accumulator is `u32` here; a plain
/// `i32` would overflow-panic in debug and saturate differently on cast.
fn hash_block(data: &[u8], start: usize, size: usize) -> u32 {
    let mut hash: u32 = 0x811c_9dc5;
    for byte in &data[start..start + size] {
        hash = (hash ^ u32::from(*byte)).wrapping_mul(0x0100_0193);
    }
    hash
}

fn read_u16(data: &[u8], offset: usize) -> u16 {
    u16::from(data[offset]) | (u16::from(data[offset + 1]) << 8)
}

fn read_u32(data: &[u8], offset: usize) -> u32 {
    u32::from(data[offset])
        | (u32::from(data[offset + 1]) << 8)
        | (u32::from(data[offset + 2]) << 16)
        | (u32::from(data[offset + 3]) << 24)
}

fn write_u16(data: &mut [u8], offset: usize, value: u16) {
    data[offset] = value as u8;
    data[offset + 1] = (value >> 8) as u8;
}

fn write_u32(data: &mut [u8], offset: usize, value: u32) {
    data[offset] = value as u8;
    data[offset + 1] = (value >> 8) as u8;
    data[offset + 2] = (value >> 16) as u8;
    data[offset + 3] = (value >> 24) as u8;
}

/// Decode a Thumb BL long-call pair into its absolute target, or `None`.
fn thumb_long_call_target(data: &[u8], offset: usize, base: u64) -> Option<i64> {
    if offset + 4 > data.len() || !offset.is_multiple_of(2) {
        return None;
    }
    let first = read_u16(data, offset);
    let second = read_u16(data, offset + 2);
    if (first & 0xf800) != 0xf000 || (second & 0xf800) != 0xf800 {
        return None;
    }
    let mut high = i64::from(first & 0x07ff);
    if (high & 0x0400) != 0 {
        high -= 0x0800;
    }
    Some(base as i64 + offset as i64 + 4 + high * 0x1000 + i64::from(second & 0x07ff) * 2)
}

fn rom_pointer(value: u32, base: u64, size: usize) -> bool {
    let address = u64::from(value - (value & 1));
    address >= base && address < base + size as u64
}

// ---------------------------------------------------------------------------
// Thumb relocation normalization
// ---------------------------------------------------------------------------

pub fn normalize_thumb_relocations(
    source: &[u8],
    base: u64,
    ranges: Option<&[NormalizationRange]>,
) -> Result<ThumbNormalization, String> {
    if base > 0xffff_ffff {
        return Err("base must be a u32 address".to_string());
    }
    let mut data = source.to_vec();
    // PORT NOTE: this is a `Set<number>` in the original and it *is* iterated,
    // so insertion order would normally be load-bearing. It is safe to use a
    // sorted set here because the write loop reads only the untouched `source`
    // and writes to disjoint 4-aligned slots, making it order-independent.
    let mut literal_offsets: std::collections::BTreeSet<usize> = std::collections::BTreeSet::new();
    let mut long_calls = 0usize;

    let default_range = [NormalizationRange { start: 0, end: source.len() }];
    let requested: &[NormalizationRange] = ranges.unwrap_or(&default_range);
    let mut selected: Vec<NormalizationRange> = Vec::with_capacity(requested.len());
    for (index, range) in requested.iter().enumerate() {
        if range.end <= range.start || range.end > source.len() {
            return Err(format!("normalization range {index} is invalid"));
        }
        selected.push(NormalizationRange { start: range.start & !1, end: range.end });
    }
    selected.sort_by(|left, right| {
        left.start.cmp(&right.start).then(left.end.cmp(&right.end))
    });

    let mut merged: Vec<NormalizationRange> = Vec::new();
    for range in selected {
        match merged.last_mut() {
            Some(previous) if range.start <= previous.end => {
                previous.end = previous.end.max(range.end);
            }
            _ => merged.push(range),
        }
    }

    for range in &merged {
        let mut offset = range.start;
        while offset + 4 <= range.end {
            if let Some(target) = thumb_long_call_target(source, offset, base) {
                if target >= base as i64
                    && target < base as i64 + source.len() as i64
                    && target % 2 == 0
                {
                    write_u16(&mut data, offset, 0xf000);
                    write_u16(&mut data, offset + 2, 0xf800);
                    long_calls += 1;
                }
            }
            let instruction = read_u16(source, offset);
            if (instruction & 0xf800) == 0x4800 {
                let literal = ((offset + 4) & !3) + usize::from(instruction & 0xff) * 4;
                if literal + 4 <= source.len()
                    && literal % 4 == 0
                    && rom_pointer(read_u32(source, literal), base, source.len())
                {
                    literal_offsets.insert(literal);
                }
            }
            offset += 2;
        }
    }

    for offset in &literal_offsets {
        let value = read_u32(source, *offset);
        write_u32(&mut data, *offset, (base as u32) | (value & 1));
    }

    Ok(ThumbNormalization {
        data,
        long_calls,
        rom_pointer_literals: literal_offsets.len(),
        scanned_bytes: merged.iter().map(|range| range.end - range.start).sum(),
    })
}

// ---------------------------------------------------------------------------
// Span consolidation
// ---------------------------------------------------------------------------

fn equal_at(left: &[u8], left_start: usize, right: &[u8], right_start: usize, size: usize) -> bool {
    left[left_start..left_start + size] == right[right_start..right_start + size]
}

fn overlaps(left_start: usize, left_size: usize, right_start: usize, right_size: usize) -> bool {
    left_start < right_start + right_size && right_start < left_start + left_size
}

fn extend_exact_span(reference: &[u8], candidate: &[u8], source: &RelativeSpan) -> RelativeSpan {
    let mut reference_start = source.reference;
    let mut candidate_start = source.candidate;
    let mut size = source.size;
    while reference_start > 0
        && candidate_start > 0
        && reference[reference_start - 1] == candidate[candidate_start - 1]
    {
        reference_start -= 1;
        candidate_start -= 1;
        size += 1;
    }
    while reference_start + size < reference.len()
        && candidate_start + size < candidate.len()
        && reference[reference_start + size] == candidate[candidate_start + size]
    {
        size += 1;
    }
    RelativeSpan {
        reference: reference_start,
        candidate: candidate_start,
        size,
        maximum_occurrences: source.maximum_occurrences,
    }
}

fn delta(span: &RelativeSpan) -> i64 {
    span.candidate as i64 - span.reference as i64
}

fn consolidate_spans(
    reference: &[u8],
    candidate: &[u8],
    source: &[RelativeSpan],
) -> Vec<RelativeSpan> {
    // PORT NOTE: `Array.prototype.sort` has been required to be stable since
    // ES2019 and every comparator below leaves ties, so `sort_by` (stable)
    // rather than `sort_unstable_by` is required for identical output.
    let mut extended: Vec<RelativeSpan> = source
        .iter()
        .map(|span| extend_exact_span(reference, candidate, span))
        .collect();
    extended.sort_by(|left, right| {
        delta(left)
            .cmp(&delta(right))
            .then(left.reference.cmp(&right.reference))
            .then(left.candidate.cmp(&right.candidate))
            .then(right.size.cmp(&left.size))
    });

    let mut merged: Vec<RelativeSpan> = Vec::new();
    for span in extended {
        let continues = match merged.last() {
            Some(previous) => {
                delta(previous) == delta(&span)
                    && span.reference <= previous.reference + previous.size
            }
            None => false,
        };
        if continues {
            let previous = merged.last_mut().expect("checked above");
            let end = (previous.reference + previous.size).max(span.reference + span.size);
            previous.size = end - previous.reference;
            previous.maximum_occurrences = Some(
                previous
                    .maximum_occurrences
                    .unwrap_or(1)
                    .max(span.maximum_occurrences.unwrap_or(1)),
            );
        } else {
            merged.push(span);
        }
    }

    let mut ranked = merged.clone();
    ranked.sort_by(|left, right| {
        right
            .size
            .cmp(&left.size)
            .then(
                left.maximum_occurrences
                    .unwrap_or(1)
                    .cmp(&right.maximum_occurrences.unwrap_or(1)),
            )
            .then(left.reference.cmp(&right.reference))
            .then(left.candidate.cmp(&right.candidate))
    });

    let mut accepted: Vec<RelativeSpan> = Vec::new();
    for span in ranked {
        let clashes = accepted.iter().any(|other| {
            overlaps(span.reference, span.size, other.reference, other.size)
                || overlaps(span.candidate, span.size, other.candidate, other.size)
        });
        if clashes {
            continue;
        }
        accepted.push(span);
    }
    accepted.sort_by(|left, right| {
        left.reference
            .cmp(&right.reference)
            .then(left.candidate.cmp(&right.candidate))
    });
    accepted
}

pub fn unique_reference_bytes(spans: &[RelativeSpan]) -> usize {
    let mut intervals: Vec<(usize, usize)> = spans
        .iter()
        .map(|span| (span.reference, span.reference + span.size))
        .collect();
    intervals.sort_by(|left, right| left.0.cmp(&right.0).then(left.1.cmp(&right.1)));
    let mut covered = 0usize;
    let mut end: i64 = -1;
    for (left, right) in intervals {
        if left as i64 > end {
            covered += right - left;
            end = right as i64;
        } else if right as i64 > end {
            covered += right - end as usize;
            end = right as i64;
        }
    }
    covered
}

pub fn same_offset_runs(
    reference: &[u8],
    candidate: &[u8],
    minimum: i64,
) -> Result<SameOffsetRuns, String> {
    if minimum <= 0 {
        return Err("minimum run must be positive".to_string());
    }
    let minimum = minimum as usize;
    let compared = reference.len().min(candidate.len());
    let mut runs: Vec<RelativeSpan> = Vec::new();
    let mut matching = 0usize;
    let mut start: Option<usize> = None;
    for index in 0..compared {
        if reference[index] == candidate[index] {
            matching += 1;
            if start.is_none() {
                start = Some(index);
            }
        } else if let Some(begin) = start.take() {
            let size = index - begin;
            if size >= minimum {
                runs.push(RelativeSpan {
                    reference: begin,
                    candidate: begin,
                    size,
                    maximum_occurrences: None,
                });
            }
        }
    }
    if let Some(begin) = start {
        if compared - begin >= minimum {
            runs.push(RelativeSpan {
                reference: begin,
                candidate: begin,
                size: compared - begin,
                maximum_occurrences: None,
            });
        }
    }
    Ok(SameOffsetRuns { compared, matching, runs })
}

pub fn analyze_relocated_matches(
    reference: &[u8],
    candidate: &[u8],
    block_size: i64,
    step: i64,
    maximum_occurrences: i64,
) -> Result<RelocatedMatchAnalysis, String> {
    if block_size <= 0 {
        return Err("block size must be positive".to_string());
    }
    if step <= 0 || step > block_size {
        return Err("step must fit block size".to_string());
    }
    if maximum_occurrences <= 0 {
        return Err("maximum occurrences must be positive".to_string());
    }
    let block_size = block_size as usize;
    let step = step as usize;
    let maximum_occurrences = maximum_occurrences as usize;

    // PORT NOTE: `Map<number, number[] | null>` in the original. It is never
    // iterated — only `get`/`set` by hash — so insertion order is not
    // observable and a HashMap is safe here. `null` is the poisoned marker for
    // a hash seen more than `maximum_occurrences` times.
    let mut index: std::collections::HashMap<u32, Option<Vec<usize>>> =
        std::collections::HashMap::new();
    let mut start = 0usize;
    while start + block_size <= candidate.len() {
        let hash = hash_block(candidate, start, block_size);
        match index.get_mut(&hash) {
            None => {
                index.insert(hash, Some(vec![start]));
            }
            Some(None) => {}
            Some(Some(positions)) => {
                if positions.len() >= maximum_occurrences {
                    index.insert(hash, None);
                } else {
                    positions.push(start);
                }
            }
        }
        start += step;
    }

    let mut spans: Vec<RelativeSpan> = Vec::new();
    let mut seed_count = 0usize;
    let mut ambiguous_seed_count = 0usize;
    let mut repetitive_seed_count = 0usize;

    let mut start = 0usize;
    while start + block_size <= reference.len() {
        let positions = index.get(&hash_block(reference, start, block_size));
        let positions = match positions {
            Some(None) => {
                repetitive_seed_count += 1;
                start += step;
                continue;
            }
            None => {
                start += step;
                continue;
            }
            Some(Some(positions)) => positions,
        };
        let verified: Vec<usize> = positions
            .iter()
            .copied()
            .filter(|position| equal_at(reference, start, candidate, *position, block_size))
            .collect();
        if verified.is_empty() {
            start += step;
            continue;
        }
        seed_count += 1;
        if verified.len() > 1 {
            ambiguous_seed_count += 1;
        }
        let continuation = spans.last().and_then(|previous| {
            if previous.reference + previous.size - block_size + step == start {
                Some(previous.candidate + previous.size - block_size + step)
            } else {
                None
            }
        });
        let chosen = match continuation {
            Some(position) if verified.contains(&position) => position,
            _ => verified[0],
        };
        if continuation == Some(chosen) {
            let previous = spans.last_mut().expect("continuation implies a previous span");
            previous.size += step;
            previous.maximum_occurrences =
                Some(previous.maximum_occurrences.unwrap_or(1).max(verified.len()));
        } else {
            spans.push(RelativeSpan {
                reference: start,
                candidate: chosen,
                size: block_size,
                maximum_occurrences: Some(verified.len()),
            });
        }
        start += step;
    }

    let raw_span_bytes: usize = spans.iter().map(|span| span.size).sum();
    let consolidated = consolidate_spans(reference, candidate, &spans);
    Ok(RelocatedMatchAnalysis {
        unique_matched_bytes: unique_reference_bytes(&consolidated),
        spans: consolidated,
        raw_span_bytes,
        seed_count,
        ambiguous_seed_count,
        repetitive_seed_count,
    })
}

pub fn relocated_matches(
    reference: &[u8],
    candidate: &[u8],
    block_size: i64,
    step: i64,
    maximum_occurrences: i64,
) -> Result<Vec<RelativeSpan>, String> {
    Ok(analyze_relocated_matches(reference, candidate, block_size, step, maximum_occurrences)?.spans)
}

// ---------------------------------------------------------------------------
// Report
// ---------------------------------------------------------------------------

/// Sentinel standing in for the `ratio` double while the report is assembled.
/// PORT NOTE: `serde_json` cannot hold a raw number token, and its ryu output
/// diverges from ECMAScript `ToString(Number)` for exactly the values `ratio`
/// can take (see `js_number_string`). The report is therefore serialized with
/// `ratio` as this string and the quoted token is replaced afterwards. The
/// sentinel uses control characters so no filesystem path can collide with it,
/// and `render_report` hard-fails unless it finds exactly one occurrence.
const RATIO_SENTINEL: &str = "\u{1}ratio\u{1}";

fn number(value: usize) -> Value {
    Value::from(value as u64)
}

fn span_entry(span: &RelativeSpan, base: u64) -> Value {
    let occurrences = span.maximum_occurrences.unwrap_or(1);
    let mut entry = Map::new();
    entry.insert(
        "reference_address".to_string(),
        Value::String(hexadecimal(span.reference, base)),
    );
    entry.insert(
        "candidate_address".to_string(),
        Value::String(hexadecimal(span.candidate, base)),
    );
    entry.insert("size".to_string(), number(span.size));
    entry.insert("delta".to_string(), Value::from(delta(span)));
    entry.insert("maximum_occurrences".to_string(), number(occurrences));
    entry.insert(
        "confidence".to_string(),
        Value::String(if occurrences == 1 { "unique" } else { "ambiguous" }.to_string()),
    );
    Value::Object(entry)
}

fn relocated_block(
    analysis: &RelocatedMatchAnalysis,
    longest: &[RelativeSpan],
    block_size: i64,
    step: i64,
    base: u64,
) -> Map<String, Value> {
    let mut block = Map::new();
    block.insert("block_size".to_string(), Value::from(block_size));
    block.insert("step".to_string(), Value::from(step));
    block.insert("span_count".to_string(), number(analysis.spans.len()));
    block.insert("matched_bytes".to_string(), number(analysis.unique_matched_bytes));
    block.insert("raw_span_bytes".to_string(), number(analysis.raw_span_bytes));
    block.insert(
        "unique_matched_bytes".to_string(),
        number(analysis.unique_matched_bytes),
    );
    block.insert("seed_count".to_string(), number(analysis.seed_count));
    block.insert(
        "ambiguous_seed_count".to_string(),
        number(analysis.ambiguous_seed_count),
    );
    block.insert(
        "repetitive_seed_count".to_string(),
        number(analysis.repetitive_seed_count),
    );
    block.insert(
        "longest_spans".to_string(),
        Value::Array(longest.iter().map(|span| span_entry(span, base)).collect()),
    );
    block
}

pub struct Comparison {
    pub report: Value,
    pub ratio: f64,
    pub matching_bytes: usize,
    pub compared_bytes: usize,
    pub run_count: usize,
    pub relocated_span_count: usize,
    pub relocated_matched_bytes: usize,
    pub relocated_ambiguous_seed_count: usize,
    pub thumb: Option<(usize, usize, i64, usize)>,
}

#[allow(clippy::too_many_arguments)]
pub fn compare(
    reference_name: &str,
    reference: &[u8],
    candidate_name: &str,
    candidate: &[u8],
    minimum_run: i64,
    block_size: i64,
    step: i64,
    limit: i64,
    base: u64,
    thumb_relocations: bool,
    thumb_ranges: &ThumbRanges,
) -> Result<Comparison, String> {
    if limit <= 0 {
        return Err("limit must be positive".to_string());
    }
    let limit = limit as usize;
    let same = same_offset_runs(reference, candidate, minimum_run)?;
    let relocated = analyze_relocated_matches(
        reference,
        candidate,
        block_size,
        step,
        DEFAULT_MAXIMUM_OCCURRENCES as i64,
    )?;

    let mut longest_same = same.runs.clone();
    longest_same.sort_by_key(|span| std::cmp::Reverse(span.size));
    longest_same.truncate(limit);
    let mut longest_relocated = relocated.spans.clone();
    longest_relocated.sort_by_key(|span| std::cmp::Reverse(span.size));
    longest_relocated.truncate(limit);

    let ratio = if same.compared == 0 {
        0.0
    } else {
        same.matching as f64 / same.compared as f64
    };

    let mut same_offset = Map::new();
    same_offset.insert("compared_bytes".to_string(), number(same.compared));
    same_offset.insert("matching_bytes".to_string(), number(same.matching));
    same_offset.insert("ratio".to_string(), Value::String(RATIO_SENTINEL.to_string()));
    same_offset.insert("run_count".to_string(), number(same.runs.len()));
    same_offset.insert(
        "longest_runs".to_string(),
        Value::Array(
            longest_same
                .iter()
                .map(|span| {
                    let mut entry = Map::new();
                    entry.insert(
                        "address".to_string(),
                        Value::String(hexadecimal(span.reference, base)),
                    );
                    entry.insert("size".to_string(), number(span.size));
                    Value::Object(entry)
                })
                .collect(),
        ),
    );

    let mut report = Map::new();
    report.insert("format".to_string(), Value::from(1));
    report.insert("reference".to_string(), Value::String(basename(reference_name)));
    report.insert("candidate".to_string(), Value::String(basename(candidate_name)));
    report.insert("reference_size".to_string(), number(reference.len()));
    report.insert("candidate_size".to_string(), number(candidate.len()));
    report.insert("same_offset".to_string(), Value::Object(same_offset));
    report.insert(
        "relocated".to_string(),
        Value::Object(relocated_block(
            &relocated,
            &longest_relocated,
            block_size,
            step,
            base,
        )),
    );

    let mut thumb = None;
    if thumb_relocations {
        let normalized_reference =
            normalize_thumb_relocations(reference, base, thumb_ranges.reference.as_deref())?;
        let normalized_candidate =
            normalize_thumb_relocations(candidate, base, thumb_ranges.candidate.as_deref())?;
        let normalized = analyze_relocated_matches(
            &normalized_reference.data,
            &normalized_candidate.data,
            block_size,
            step,
            DEFAULT_MAXIMUM_OCCURRENCES as i64,
        )?;
        let mut longest_normalized = normalized.spans.clone();
        longest_normalized.sort_by_key(|span| std::cmp::Reverse(span.size));
        longest_normalized.truncate(limit);

        let matched_bytes = normalized.unique_matched_bytes;
        let exact_matched_bytes = relocated.unique_matched_bytes;
        let improvement = matched_bytes as i64 - exact_matched_bytes as i64;

        let mut block = Map::new();
        block.insert("block_size".to_string(), Value::from(block_size));
        block.insert("step".to_string(), Value::from(step));
        block.insert("span_count".to_string(), number(normalized.spans.len()));
        block.insert("matched_bytes".to_string(), number(matched_bytes));
        block.insert("exact_matched_bytes".to_string(), number(exact_matched_bytes));
        block.insert("improvement_bytes".to_string(), Value::from(improvement));
        block.insert("raw_span_bytes".to_string(), number(normalized.raw_span_bytes));
        block.insert(
            "unique_matched_bytes".to_string(),
            number(normalized.unique_matched_bytes),
        );
        block.insert("seed_count".to_string(), number(normalized.seed_count));
        block.insert(
            "ambiguous_seed_count".to_string(),
            number(normalized.ambiguous_seed_count),
        );
        block.insert(
            "repetitive_seed_count".to_string(),
            number(normalized.repetitive_seed_count),
        );
        let mut normalization = Map::new();
        normalization.insert(
            "reference_long_calls".to_string(),
            number(normalized_reference.long_calls),
        );
        normalization.insert(
            "reference_rom_pointer_literals".to_string(),
            number(normalized_reference.rom_pointer_literals),
        );
        normalization.insert(
            "candidate_long_calls".to_string(),
            number(normalized_candidate.long_calls),
        );
        normalization.insert(
            "candidate_rom_pointer_literals".to_string(),
            number(normalized_candidate.rom_pointer_literals),
        );
        normalization.insert(
            "reference_scanned_bytes".to_string(),
            number(normalized_reference.scanned_bytes),
        );
        normalization.insert(
            "candidate_scanned_bytes".to_string(),
            number(normalized_candidate.scanned_bytes),
        );
        block.insert("normalization".to_string(), Value::Object(normalization));
        block.insert(
            "longest_spans".to_string(),
            Value::Array(
                longest_normalized
                    .iter()
                    .map(|span| span_entry(span, base))
                    .collect(),
            ),
        );
        thumb = Some((
            normalized.spans.len(),
            matched_bytes,
            improvement,
            normalized.ambiguous_seed_count,
        ));
        report.insert("thumb_relocated".to_string(), Value::Object(block));
    }

    Ok(Comparison {
        report: Value::Object(report),
        ratio,
        matching_bytes: same.matching,
        compared_bytes: same.compared,
        run_count: same.runs.len(),
        relocated_span_count: relocated.spans.len(),
        relocated_matched_bytes: relocated.unique_matched_bytes,
        relocated_ambiguous_seed_count: relocated.ambiguous_seed_count,
        thumb,
    })
}

/// Render the canonical JSON text, substituting the real `ratio` token.
pub fn render_report(comparison: &Comparison) -> Result<String, String> {
    let text = canonical_json::canonical_json(&comparison.report);
    let quoted = serde_json::to_string(&Value::String(RATIO_SENTINEL.to_string()))
        .expect("a string always serializes");
    if text.matches(&quoted).count() != 1 {
        return Err("ratio placeholder did not appear exactly once".to_string());
    }
    Ok(text.replace(&quoted, &js_number_string(comparison.ratio)))
}

pub fn summary_line(comparison: &Comparison) -> String {
    let mut line = format!(
        "same_offset={}/{} same_runs={} relocated_spans={} relocated_bytes={} ambiguous_seeds={}",
        comparison.matching_bytes,
        comparison.compared_bytes,
        comparison.run_count,
        comparison.relocated_span_count,
        comparison.relocated_matched_bytes,
        comparison.relocated_ambiguous_seed_count,
    );
    if let Some((spans, bytes, improvement, ambiguous)) = comparison.thumb {
        line.push_str(&format!(
            " thumb_relocated_spans={spans} thumb_relocated_bytes={bytes} \
             thumb_improvement={improvement} thumb_ambiguous_seeds={ambiguous}"
        ));
    }
    line
}

// ---------------------------------------------------------------------------
// Self-test
// ---------------------------------------------------------------------------

/// xorshift32 fixture generator. PORT NOTE: `data[index] = state` truncates to
/// the low byte on store into a `Uint8Array`, which `as u8` reproduces.
pub fn synthetic(seed: u32, size: usize) -> Vec<u8> {
    let mut data = vec![0u8; size];
    let mut state = seed;
    for slot in data.iter_mut() {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        *slot = state as u8;
    }
    data
}

fn encode_thumb_long_call(
    data: &mut [u8],
    offset: usize,
    target: i64,
    base: u64,
) -> Result<(), String> {
    let displacement = target - (base as i64 + offset as i64 + 4);
    if displacement % 2 != 0 || !(-0x400000..=0x3ffffe).contains(&displacement) {
        return Err("synthetic long call is outside Thumb range".to_string());
    }
    write_u16(data, offset, 0xf000 | ((displacement >> 12) & 0x07ff) as u16);
    write_u16(data, offset + 2, 0xf800 | ((displacement >> 1) & 0x07ff) as u16);
    Ok(())
}

/// Every branch here is a real assertion over an assembled fixture. The counter
/// exists so the gate cannot pass vacuously: if a future edit skips checks, the
/// final count mismatch is a hard failure rather than a silent green.
pub fn self_test() -> Result<usize, String> {
    let mut checks = 0usize;
    let base = DEFAULT_BASE;

    let reference = synthetic(1, 1024);
    let mut candidate = synthetic(2, 1280);
    candidate[400..400 + 512].copy_from_slice(&reference[128..640]);
    let moved = relocated_matches(&reference, &candidate, 64, 16, 16)?;
    if !moved
        .iter()
        .any(|span| span.reference == 128 && span.candidate == 400 && span.size == 512)
    {
        return Err("relocated comparison self-test failed".to_string());
    }
    checks += 1;

    let mut same_candidate = reference.clone();
    same_candidate[96..128].fill(0);
    let same = same_offset_runs(&reference, &same_candidate, 32)?;
    if same.matching != 992 || same.runs.len() != 2 || same.runs[0].size != 96 {
        return Err("same-offset comparison self-test failed".to_string());
    }
    checks += 1;

    let mut instructions = vec![0u8; 64];
    write_u16(&mut instructions, 0, 0x4803);
    encode_thumb_long_call(&mut instructions, 4, base as i64 + 48, base)?;
    write_u16(&mut instructions, 8, 0xe001);
    write_u32(&mut instructions, 16, base as u32 + 41);
    write_u32(&mut instructions, 20, base as u32 + 45);
    let normalized_instructions = normalize_thumb_relocations(&instructions, base, None)?;
    if normalized_instructions.long_calls != 1
        || normalized_instructions.rom_pointer_literals != 1
        || read_u16(&normalized_instructions.data, 4) != 0xf000
        || read_u16(&normalized_instructions.data, 6) != 0xf800
        || read_u16(&normalized_instructions.data, 8) != 0xe001
        || read_u32(&normalized_instructions.data, 16) != base as u32 + 1
        || read_u32(&normalized_instructions.data, 20) != base as u32 + 45
    {
        return Err("Thumb relocation normalization self-test failed".to_string());
    }
    checks += 1;

    let mut relocated_reference = synthetic(3, 1024);
    let mut relocated_candidate = synthetic(4, 1280);
    relocated_candidate[400..400 + 128].copy_from_slice(&relocated_reference[128..256]);
    write_u16(&mut relocated_reference, 128, 0x4807);
    write_u16(&mut relocated_candidate, 400, 0x4807);
    encode_thumb_long_call(&mut relocated_reference, 136, base as i64 + 800, base)?;
    encode_thumb_long_call(&mut relocated_candidate, 408, base as i64 + 1000, base)?;
    write_u32(&mut relocated_reference, 160, base as u32 + 701);
    write_u32(&mut relocated_candidate, 432, base as u32 + 901);
    let exact_moved = relocated_matches(&relocated_reference, &relocated_candidate, 64, 16, 16)?;
    let normalized_moved = relocated_matches(
        &normalize_thumb_relocations(&relocated_reference, base, None)?.data,
        &normalize_thumb_relocations(&relocated_candidate, base, None)?.data,
        64,
        16,
        16,
    )?;
    let hit = |spans: &[RelativeSpan]| {
        spans
            .iter()
            .any(|span| span.reference == 128 && span.candidate == 400 && span.size >= 128)
    };
    if hit(&exact_moved) || !hit(&normalized_moved) {
        return Err("Thumb-aware relocated comparison self-test failed".to_string());
    }
    checks += 1;

    let phased_reference = synthetic(5, 1024);
    let mut phased_candidate = synthetic(6, 1400);
    phased_candidate[393..393 + 608].copy_from_slice(&phased_reference[125..733]);
    let coarse_phase = relocated_matches(&phased_reference, &phased_candidate, 64, 16, 16)?;
    let fine_phase = relocated_matches(&phased_reference, &phased_candidate, 64, 4, 16)?;
    if coarse_phase
        .iter()
        .any(|span| span.reference <= 125 && span.reference + span.size >= 733)
        || !fine_phase
            .iter()
            .any(|span| span.reference == 125 && span.candidate == 393 && span.size == 608)
    {
        return Err("relocated phase and extension self-test failed".to_string());
    }
    checks += 1;

    if unique_reference_bytes(&[
        RelativeSpan { reference: 10, candidate: 20, size: 20, maximum_occurrences: None },
        RelativeSpan { reference: 20, candidate: 60, size: 20, maximum_occurrences: None },
    ]) != 30
    {
        return Err("unique relocated coverage self-test failed".to_string());
    }
    checks += 1;

    let ranged = normalize_thumb_relocations(
        &instructions,
        base,
        Some(&[NormalizationRange { start: 4, end: 12 }]),
    )?;
    if ranged.long_calls != 1
        || ranged.rom_pointer_literals != 0
        || ranged.scanned_bytes != 8
        || read_u32(&ranged.data, 16) != base as u32 + 41
    {
        return Err("ranged Thumb normalization self-test failed".to_string());
    }
    checks += 1;

    let repeated_reference = vec![0u8; 128];
    let repeated_candidate = vec![0u8; 256];
    let repeated =
        analyze_relocated_matches(&repeated_reference, &repeated_candidate, 32, 4, 2)?;
    if repeated.repetitive_seed_count == 0 || !repeated.spans.is_empty() {
        return Err("repetitive seed diagnosis self-test failed".to_string());
    }
    checks += 1;

    if checks != 8 {
        return Err(format!("self-test executed {checks} checks, expected 8"));
    }
    Ok(checks)
}

// ---------------------------------------------------------------------------
// CLI argument handling
// ---------------------------------------------------------------------------

pub const USAGE: &str = "usage: compare-roms REFERENCE CANDIDATE -o OUT [--min-run N] \
[--block-size N] [--step N] [--limit N] [--thumb-relocations] \
[--thumb-reference-range START:END] [--thumb-candidate-range START:END]";

pub fn option<'a>(args: &'a [String], name: &str, fallback: Option<&'a str>) -> Result<&'a str, String> {
    match args.iter().position(|argument| argument == name) {
        None => fallback.ok_or_else(|| format!("{name} is required")),
        Some(index) => {
            if index + 1 >= args.len() {
                return Err(format!("{name} requires a value"));
            }
            Ok(&args[index + 1])
        }
    }
}

pub fn range_options(
    args: &[String],
    name: &str,
    base: u64,
    size: usize,
) -> Result<Option<Vec<NormalizationRange>>, String> {
    let mut result: Vec<NormalizationRange> = Vec::new();
    for index in 0..args.len() {
        if args[index] != name {
            continue;
        }
        let value = args
            .get(index + 1)
            .ok_or_else(|| format!("{name} requires START:END"))?;
        let parts: Vec<&str> = value.split(':').collect();
        if parts.len() != 2 {
            return Err(format!("{name} requires START:END"));
        }
        let low = integer(parts[0], name)?;
        let high = integer(parts[1], name)?;
        let start = if low >= base as i64 { low - base as i64 } else { low };
        let end = if high >= base as i64 { high - base as i64 } else { high };
        if start < 0 || end <= start || end > size as i64 {
            return Err(format!("{name} is outside the ROM"));
        }
        result.push(NormalizationRange { start: start as usize, end: end as usize });
    }
    Ok(if result.is_empty() { None } else { Some(result) })
}

pub fn private_output(path: &str) -> bool {
    let normalized = path.replace('\\', "/");
    normalized.starts_with("out/")
        || normalized.contains("/out/")
        || normalized.starts_with("/tmp/")
        || normalized.starts_with("/private/tmp/")
}

pub fn positional(args: &[String]) -> Vec<String> {
    args.iter()
        .enumerate()
        .filter(|(index, argument)| {
            !argument.starts_with('-') && (*index == 0 || !args[index - 1].starts_with('-'))
        })
        .map(|(_, argument)| argument.clone())
        .collect()
}

/// Outcome of a CLI run, so `main` stays a thin shell around testable logic.
#[derive(Debug)]
pub enum Run {
    Printed(String),
    Wrote { path: String, contents: String, line: String },
}

pub fn run(args: &[String]) -> Result<Run, String> {
    if args.iter().any(|argument| argument == "--self-test") {
        self_test()?;
        return Ok(Run::Printed("self-test=ok".to_string()));
    }
    let positional = positional(args);
    if positional.len() < 2 {
        return Ok(Run::Printed(USAGE.to_string()));
    }
    let output = option(args, "-o", None)?.to_string();
    if !private_output(&output) {
        return Err("comparison reports must stay under out/ or a temporary directory".to_string());
    }
    let reference_path = &positional[0];
    let candidate_path = &positional[1];
    let reference = std::fs::read(reference_path)
        .map_err(|error| format!("{reference_path}: {error}"))?;
    let candidate = std::fs::read(candidate_path)
        .map_err(|error| format!("{candidate_path}: {error}"))?;
    let base = DEFAULT_BASE;
    let thumb_ranges = ThumbRanges {
        reference: range_options(args, "--thumb-reference-range", base, reference.len())?,
        candidate: range_options(args, "--thumb-candidate-range", base, candidate.len())?,
    };
    let comparison = compare(
        reference_path,
        &reference,
        candidate_path,
        &candidate,
        integer(option(args, "--min-run", Some("64"))?, "--min-run")?,
        integer(option(args, "--block-size", Some("64"))?, "--block-size")?,
        integer(option(args, "--step", Some("4"))?, "--step")?,
        integer(option(args, "--limit", Some("128"))?, "--limit")?,
        base,
        args.iter().any(|argument| argument == "--thumb-relocations"),
        &thumb_ranges,
    )?;
    Ok(Run::Wrote {
        path: output,
        contents: format!("{}\n", render_report(&comparison)?),
        line: summary_line(&comparison),
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    fn argv(items: &[&str]) -> Vec<String> {
        items.iter().map(|item| (*item).to_string()).collect()
    }

    #[test]
    fn ported_self_test_runs_every_check() {
        assert_eq!(self_test().unwrap(), 8);
    }

    #[test]
    fn js_number_string_matches_ecmascript_tostring() {
        // Integral doubles lose the `.0` that Rust's Display adds.
        assert_eq!(js_number_string(1.0), "1");
        assert_eq!(js_number_string(0.0), "0");
        assert_eq!(js_number_string(100.0), "100");
        // Ordinary fractions keep the shortest round-tripping digits.
        assert_eq!(js_number_string(0.5), "0.5");
        assert_eq!(js_number_string(1.0 / 3.0), "0.3333333333333333");
        assert_eq!(js_number_string(992.0 / 1024.0), "0.96875");
        // Below 1e-6 ECMAScript switches to exponential; Rust's Display does not.
        assert_eq!(js_number_string(1.0 / 16777216.0), "5.960464477539063e-8");
        assert_eq!(js_number_string(0.000001), "0.000001");
        assert_eq!(js_number_string(0.0000001), "1e-7");
        // Above 1e21 it switches the other way.
        assert_eq!(js_number_string(1e21), "1e+21");
        assert_eq!(js_number_string(1e20), "100000000000000000000");
    }

    #[test]
    fn js_number_follows_the_string_numeric_literal_grammar() {
        assert_eq!(js_number(""), 0.0);
        assert_eq!(js_number("   \n\t"), 0.0);
        assert_eq!(js_number("\u{feff}7"), 7.0);
        assert_eq!(js_number("0x10"), 16.0);
        assert_eq!(js_number("0b101"), 5.0);
        assert_eq!(js_number("0o17"), 15.0);
        assert_eq!(js_number(" 42 "), 42.0);
        assert_eq!(js_number("1e3"), 1000.0);
        assert_eq!(js_number("-5"), -5.0);
        assert!(js_number("12abc").is_nan());
        assert!(js_number("inf").is_nan());
        assert!(js_number("NaN").is_nan());
        assert!(js_number("1_0").is_nan());
        assert!(js_number("Infinity").is_infinite());
        // U+0085 is whitespace to Rust but not to JavaScript.
        assert!(js_number("\u{85}7").is_nan());
        // `1.5` parses fine but is not a safe integer, which is what `integer` gates on.
        assert!(!is_safe_integer(js_number("1.5")));
        assert!(is_safe_integer(js_number("1.0")));
    }

    #[test]
    fn integer_rejects_non_integers_with_the_original_message() {
        assert_eq!(integer("64", "--min-run").unwrap(), 64);
        assert_eq!(integer("0x40", "--min-run").unwrap(), 64);
        assert_eq!(
            integer("1.5", "--min-run").unwrap_err(),
            "--min-run must be an integer"
        );
    }

    #[test]
    fn hash_block_wraps_like_math_imul() {
        // FNV-1a of a single 0xff byte, computed with u32 wrapping.
        let expected = (0x811c_9dc5u32 ^ 0xff).wrapping_mul(0x0100_0193);
        assert_eq!(hash_block(&[0xff], 0, 1), expected);
        // An empty block is the offset basis.
        assert_eq!(hash_block(&[], 0, 0), 0x811c_9dc5);
    }

    #[test]
    fn synthetic_truncates_to_the_low_byte() {
        let data = synthetic(1, 4);
        assert_eq!(data.len(), 4);
        // xorshift32 from seed 1: first state is 0x0004_2021 -> low byte 0x21.
        let mut state: u32 = 1;
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        assert_eq!(data[0], state as u8);
    }

    #[test]
    fn same_offset_runs_reports_boundary_runs() {
        let reference = vec![1u8; 16];
        let mut candidate = reference.clone();
        candidate[8] = 2;
        let runs = same_offset_runs(&reference, &candidate, 4).unwrap();
        assert_eq!(runs.compared, 16);
        assert_eq!(runs.matching, 15);
        // One run before the difference, one trailing run closed by the tail case.
        assert_eq!(runs.runs.len(), 2);
        assert_eq!(runs.runs[0].size, 8);
        assert_eq!(runs.runs[1].size, 7);
        // Runs shorter than the minimum are dropped but still counted as matching.
        let short = same_offset_runs(&reference, &candidate, 8).unwrap();
        assert_eq!(short.matching, 15);
        assert_eq!(short.runs.len(), 1);
        assert_eq!(
            same_offset_runs(&reference, &candidate, 0).unwrap_err(),
            "minimum run must be positive"
        );
    }

    #[test]
    fn unique_reference_bytes_merges_overlaps() {
        assert_eq!(
            unique_reference_bytes(&[
                RelativeSpan { reference: 0, candidate: 0, size: 10, maximum_occurrences: None },
                RelativeSpan { reference: 5, candidate: 0, size: 10, maximum_occurrences: None },
                RelativeSpan { reference: 40, candidate: 0, size: 5, maximum_occurrences: None },
            ]),
            20
        );
        assert_eq!(unique_reference_bytes(&[]), 0);
    }

    #[test]
    fn analyze_rejects_bad_geometry() {
        let data = synthetic(9, 64);
        assert_eq!(
            analyze_relocated_matches(&data, &data, 0, 4, 16).unwrap_err(),
            "block size must be positive"
        );
        assert_eq!(
            analyze_relocated_matches(&data, &data, 16, 32, 16).unwrap_err(),
            "step must fit block size"
        );
        assert_eq!(
            analyze_relocated_matches(&data, &data, 16, 4, 0).unwrap_err(),
            "maximum occurrences must be positive"
        );
    }

    #[test]
    fn normalization_ranges_are_validated_and_merged() {
        let source = vec![0u8; 64];
        assert_eq!(
            normalize_thumb_relocations(
                &source,
                DEFAULT_BASE,
                Some(&[NormalizationRange { start: 8, end: 8 }])
            )
            .unwrap_err(),
            "normalization range 0 is invalid"
        );
        assert_eq!(
            normalize_thumb_relocations(
                &source,
                DEFAULT_BASE,
                Some(&[NormalizationRange { start: 0, end: 65 }])
            )
            .unwrap_err(),
            "normalization range 0 is invalid"
        );
        // Touching ranges merge, and an odd start is rounded down to a halfword.
        let merged = normalize_thumb_relocations(
            &source,
            DEFAULT_BASE,
            Some(&[
                NormalizationRange { start: 16, end: 32 },
                NormalizationRange { start: 5, end: 16 },
            ]),
        )
        .unwrap();
        assert_eq!(merged.scanned_bytes, 28);
    }

    #[test]
    fn report_key_order_and_ratio_spelling_survive_rendering() {
        let reference = synthetic(11, 256);
        let comparison = compare(
            "roms/reference.gba",
            &reference,
            "out/candidate.gba",
            &reference,
            16,
            64,
            4,
            8,
            DEFAULT_BASE,
            false,
            &ThumbRanges::default(),
        )
        .unwrap();
        let text = render_report(&comparison).unwrap();
        // Identical inputs give ratio 1, the value that would otherwise render "1.0".
        assert!(text.contains("\"ratio\": 1,"), "{text}");
        assert!(!text.contains("1.0"), "{text}");
        // Basenames, not full paths.
        assert!(text.contains("\"reference\": \"reference.gba\""));
        assert!(text.contains("\"candidate\": \"candidate.gba\""));
        // Key order is insertion order, not sorted.
        let format_at = text.find("\"format\"").unwrap();
        let reference_at = text.find("\"reference\"").unwrap();
        let same_at = text.find("\"same_offset\"").unwrap();
        let relocated_at = text.find("\"relocated\"").unwrap();
        assert!(format_at < reference_at && reference_at < same_at && same_at < relocated_at);
        assert!(!text.contains("thumb_relocated"));
    }

    #[test]
    fn ratio_zero_and_tiny_render_like_javascript() {
        let reference = vec![0u8; 4];
        let candidate = vec![1u8; 4];
        let comparison = compare(
            "a", &reference, "b", &candidate, 1, 4, 4, 8, DEFAULT_BASE, false,
            &ThumbRanges::default(),
        )
        .unwrap();
        assert!(render_report(&comparison).unwrap().contains("\"ratio\": 0,"));
    }

    #[test]
    fn thumb_block_appears_only_when_requested() {
        let mut reference = synthetic(13, 512);
        let mut candidate = synthetic(14, 640);
        candidate[128..128 + 256].copy_from_slice(&reference[64..320]);
        encode_thumb_long_call(&mut reference, 64, DEFAULT_BASE as i64 + 400, DEFAULT_BASE)
            .unwrap();
        encode_thumb_long_call(&mut candidate, 128, DEFAULT_BASE as i64 + 500, DEFAULT_BASE)
            .unwrap();
        let comparison = compare(
            "a", &reference, "b", &candidate, 16, 64, 4, 8, DEFAULT_BASE, true,
            &ThumbRanges::default(),
        )
        .unwrap();
        let text = render_report(&comparison).unwrap();
        assert!(text.contains("thumb_relocated"));
        assert!(text.contains("reference_scanned_bytes"));
        assert!(comparison.thumb.is_some());
        assert!(summary_line(&comparison).contains("thumb_improvement="));
    }

    #[test]
    fn positional_skips_flag_values() {
        assert_eq!(
            positional(&argv(&["a.gba", "b.gba", "-o", "out/x.json", "--limit", "4"])),
            vec!["a.gba".to_string(), "b.gba".to_string()]
        );
        // A value that follows a flag is never positional, even at index 0's neighbour.
        assert_eq!(
            positional(&argv(&["--limit", "4", "a.gba", "b.gba"])),
            vec!["a.gba".to_string(), "b.gba".to_string()]
        );
    }

    #[test]
    fn option_lookup_matches_index_of_semantics() {
        let args = argv(&["--step", "8", "--limit"]);
        assert_eq!(option(&args, "--step", None).unwrap(), "8");
        assert_eq!(option(&args, "--min-run", Some("64")).unwrap(), "64");
        assert_eq!(
            option(&args, "--limit", None).unwrap_err(),
            "--limit requires a value"
        );
        assert_eq!(option(&args, "-o", None).unwrap_err(), "-o is required");
    }

    #[test]
    fn range_options_accept_absolute_and_relative_bounds() {
        let args = argv(&[
            "--thumb-reference-range",
            "0:16",
            "--thumb-reference-range",
            "0x08000020:0x08000030",
        ]);
        let ranges = range_options(&args, "--thumb-reference-range", DEFAULT_BASE, 64)
            .unwrap()
            .unwrap();
        assert_eq!(ranges.len(), 2);
        assert_eq!(ranges[0], NormalizationRange { start: 0, end: 16 });
        assert_eq!(ranges[1], NormalizationRange { start: 32, end: 48 });
        assert!(range_options(&argv(&[]), "--thumb-reference-range", DEFAULT_BASE, 64)
            .unwrap()
            .is_none());
        assert_eq!(
            range_options(&argv(&["--thumb-reference-range", "0:99"]), "--thumb-reference-range", DEFAULT_BASE, 64)
                .unwrap_err(),
            "--thumb-reference-range is outside the ROM"
        );
        assert_eq!(
            range_options(&argv(&["--thumb-reference-range", "0"]), "--thumb-reference-range", DEFAULT_BASE, 64)
                .unwrap_err(),
            "--thumb-reference-range requires START:END"
        );
    }

    #[test]
    fn private_output_gate_matches_the_original() {
        assert!(private_output("out/report.json"));
        assert!(private_output("build/out/report.json"));
        assert!(private_output("/tmp/report.json"));
        assert!(private_output("/private/tmp/report.json"));
        assert!(private_output("build\\out\\report.json"));
        assert!(!private_output("report.json"));
        assert!(!private_output("outside/report.json"));
        assert!(!private_output("/var/report.json"));
    }

    #[test]
    fn run_dispatches_self_test_and_usage() {
        match run(&argv(&["--self-test"])).unwrap() {
            Run::Printed(text) => assert_eq!(text, "self-test=ok"),
            _ => panic!("expected printed output"),
        }
        match run(&argv(&["only.gba"])).unwrap() {
            Run::Printed(text) => assert!(text.starts_with("usage: compare-roms")),
            _ => panic!("expected usage"),
        }
        assert_eq!(
            run(&argv(&["a.gba", "b.gba", "-o", "report.json"])).unwrap_err(),
            "comparison reports must stay under out/ or a temporary directory"
        );
    }
}
