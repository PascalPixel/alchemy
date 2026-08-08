// Interval algebra and the commit-subject grammar of the Full-C Byte Share
// metric.
//
// WHY: this is the arithmetic core of the Full-C progress metric -- the
// audited union that forms the denominator, the byte-identical C union that
// forms the numerator, and the kilobyte subject marker both are rendered
// into. It is separated from the overlay decoding so the traps can be pinned
// by tests that assemble intervals directly.
//
// PORT NOTE: `start` and `end` are `f64`, not integers. In JavaScript there is
// only one number type, so `validateIntervals` accepting `1.0` and rejecting
// `1.5` is a runtime check, not a type. A listing row whose encoding column
// holds an odd number of hex digits yields `width = digits / 2 = 1.5` and the
// interval built from it must be REJECTED by that check rather than silently
// truncated. Modelling the field as `i64` would quietly change that outcome.

use crate::json;

pub const OVERLAY_BASE: i64 = 0x0200_0000;

const MAX_SAFE_INTEGER: f64 = 9_007_199_254_740_991.0;

#[derive(Debug, Clone, PartialEq)]
pub struct Interval {
    pub start: f64,
    pub end: f64,
    pub kind: String,
    pub evidence: String,
}

impl Interval {
    pub fn new(start: i64, end: i64, kind: &str, evidence: &str) -> Self {
        Interval {
            start: start as f64,
            end: end as f64,
            kind: kind.to_string(),
            evidence: evidence.to_string(),
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq)]
pub struct Span {
    pub start: f64,
    pub end: f64,
}

/// `Number.isSafeInteger(value) && value >= 0`, with the original message.
pub fn integer(value: f64, label: &str) -> Result<f64, String> {
    if !is_safe_integer(value) || value < 0.0 {
        return Err(format!("{label} must be a non-negative safe integer"));
    }
    Ok(value)
}

pub fn is_safe_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0 && value.abs() <= MAX_SAFE_INTEGER
}

/// `a.start - b.start || a.end - b.end`, the comparator used throughout.
///
/// PORT NOTE: `Array.prototype.sort` has been required to be stable since
/// ES2019, and `slice::sort_by` is stable too, so equal keys keep source order
/// on both sides. That matters: `mergeClassified` relies on it to pick a
/// deterministic representative evidence string.
fn by_start_then_end(left: (f64, f64), right: (f64, f64)) -> std::cmp::Ordering {
    left.0
        .partial_cmp(&right.0)
        .unwrap_or(std::cmp::Ordering::Equal)
        .then(
            left.1
                .partial_cmp(&right.1)
                .unwrap_or(std::cmp::Ordering::Equal),
        )
}

pub fn validate_intervals(intervals: &[Interval], label: &str) -> Result<(), String> {
    for (index, interval) in intervals.iter().enumerate() {
        integer(interval.start, &format!("{label}[{index}].start"))?;
        integer(interval.end, &format!("{label}[{index}].end"))?;
        if interval.end <= interval.start {
            return Err(format!("{label}[{index}] is empty or reversed"));
        }
        // PORT NOTE: the TypeScript tests `!interval.kind || !interval.evidence`,
        // which is JS falsiness -- for these two string fields that is exactly
        // "is the empty string".
        if interval.kind.is_empty() || interval.evidence.is_empty() {
            return Err(format!("{label}[{index}] lacks classification evidence"));
        }
    }
    Ok(())
}

/// `Number.prototype.toString(16)` for the non-negative safe integers that
/// reach this point: lowercase, no prefix.
fn hex(value: f64) -> String {
    format!("{:x}", value as i64)
}

pub fn overlap_diagnostics(intervals: &[Interval]) -> Result<Vec<String>, String> {
    validate_intervals(intervals, "intervals")?;
    let mut sorted: Vec<&Interval> = intervals.iter().collect();
    sorted.sort_by(|a, b| by_start_then_end((a.start, a.end), (b.start, b.end)));
    let mut diagnostics = Vec::new();
    let mut active: Option<&Interval> = None;
    for interval in sorted {
        if let Some(current) = active {
            let identical = interval.start == current.start
                && interval.end == current.end
                && interval.kind == current.kind
                && interval.evidence == current.evidence;
            if interval.start < current.end && !identical {
                diagnostics.push(format!(
                    "0x{}..0x{} overlaps {} and {}",
                    hex(interval.start),
                    hex(current.end.min(interval.end)),
                    current.kind,
                    interval.kind,
                ));
            }
        }
        if active.is_none_or(|current| interval.end > current.end) {
            active = Some(interval);
        }
    }
    Ok(diagnostics)
}

pub fn union_intervals(intervals: &[Interval]) -> Result<Vec<Span>, String> {
    validate_intervals(intervals, "intervals")?;
    let mut sorted: Vec<&Interval> = intervals.iter().collect();
    sorted.sort_by(|a, b| by_start_then_end((a.start, a.end), (b.start, b.end)));
    let mut result: Vec<Span> = Vec::new();
    for interval in sorted {
        match result.last_mut() {
            Some(previous) if interval.start <= previous.end => {
                if interval.end > previous.end {
                    previous.end = interval.end;
                }
            }
            _ => result.push(Span {
                start: interval.start,
                end: interval.end,
            }),
        }
    }
    Ok(result)
}

pub fn interval_bytes(intervals: &[Interval]) -> Result<f64, String> {
    Ok(union_intervals(intervals)?
        .iter()
        .map(|span| span.end - span.start)
        .sum())
}

/// `${value}`.replace(/\B(?=(\d{3})+(?!\d))/g, ",") after the `integer` guard.
pub fn commas(value: f64) -> Result<String, String> {
    integer(value, "count")?;
    Ok(crate::js::commas(value as i64))
}

fn canonical_count(value: &str, label: &str) -> Result<f64, String> {
    // PORT NOTE: `Number("")` is 0 and `Number("0x10")` is 16 under the
    // ECMAScript StringNumericLiteral grammar, neither of which `f64::from_str`
    // would produce. The caller only ever passes a `[0-9,]` run captured by
    // SUBJECT, so after removing commas the text is a plain decimal digit run
    // -- but an empty run must still read as 0 rather than a parse failure,
    // and the `commas(parsed) !== value` round trip is what rejects it.
    let digits = value.replace(',', "");
    let parsed = if digits.is_empty() {
        0.0
    } else {
        digits.parse::<f64>().unwrap_or(f64::NAN)
    };
    let canonical =
        is_safe_integer(parsed) && parsed >= 0.0 && crate::js::commas(parsed as i64) == value;
    if !canonical {
        return Err(format!("{label} is not canonically comma-separated"));
    }
    Ok(parsed)
}

/// Whole kilobytes, floored, so the subject reads `[ ☀️ 316 / 1,345 ]`.
pub fn kilobytes(value: f64) -> Result<f64, String> {
    Ok((integer(value, "byte count")? / 1000.0).floor())
}

pub fn format_subject(full_c_bytes: f64, executable_bytes: f64) -> Result<String, String> {
    if full_c_bytes > executable_bytes {
        return Err("Full-C numerator exceeds executable denominator".to_string());
    }
    Ok(format!(
        "[ \u{2600}\u{fe0f} {} / {} ]",
        commas(kilobytes(full_c_bytes)?)?,
        commas(kilobytes(executable_bytes)?)?
    ))
}

pub struct Subject {
    pub full_c_kilobytes: f64,
    pub executable_kilobytes: f64,
}

/// `undefined` when the marker is absent; `Err` when it is present but not
/// canonical, which is how the TypeScript distinguishes "no marker" from
/// "broken marker".
pub fn parse_subject(subject: &str) -> Result<Option<Subject>, String> {
    let Some((numerator, denominator)) = crate::js::subject_match(subject) else {
        return Ok(None);
    };
    let full_c_kilobytes = canonical_count(numerator, "Full-C numerator")?;
    let executable_kilobytes = canonical_count(denominator, "executable denominator")?;
    if full_c_kilobytes > executable_kilobytes {
        return Err("Full-C numerator exceeds executable denominator".to_string());
    }
    Ok(Some(Subject {
        full_c_kilobytes,
        executable_kilobytes,
    }))
}

pub fn round_half_up_percent(numerator: f64, denominator: f64) -> Result<f64, String> {
    if denominator <= 0.0 {
        return Err("executable denominator must be positive".to_string());
    }
    Ok(((numerator * 10000.0 + denominator / 2.0) / denominator).floor() / 100.0)
}

/// `Number.prototype.toFixed(2)`.
///
/// PORT NOTE: `toFixed` breaks ties toward +∞ while Rust's `{:.2}` breaks them
/// to even. The only values reaching here come from `roundHalfUpPercent`, which
/// returns `floor(...)/100`; the f64 nearest a two-decimal value is never
/// exactly halfway at the third decimal, so no tie can occur and the two agree.
pub fn to_fixed_2(value: f64) -> String {
    format!("{value:.2}")
}

/// Dedupe by `start:end:kind`, sort, then coalesce adjacent same-kind spans.
pub fn merge_classified(intervals: &[Interval]) -> Result<Vec<Interval>, String> {
    // PORT NOTE: `unique` is a JS `Map`, and `[...unique.values()]` yields
    // INSERTION order. A HashMap would randomise the pre-sort order and, since
    // the sort is stable, change which duplicate's `evidence` survives.
    let mut order: Vec<&Interval> = Vec::new();
    let mut seen: std::collections::HashSet<String> = std::collections::HashSet::new();
    for interval in intervals {
        let key = format!(
            "{}:{}:{}",
            json::number(interval.start),
            json::number(interval.end),
            interval.kind
        );
        if seen.insert(key) {
            order.push(interval);
        }
    }
    // PORT NOTE: the third sort key is `a.kind.localeCompare(b.kind)`, which is
    // ICU collation, not code-unit order. The kind vocabulary is closed --
    // arm, executable_alignment, literal_pool, structural, thumb, veneer -- and
    // those six are lowercase ASCII with distinct first letters, so ICU order,
    // UTF-16 order and byte order all coincide. Byte order is used here.
    order.sort_by(|a, b| {
        by_start_then_end((a.start, a.end), (b.start, b.end)).then_with(|| a.kind.cmp(&b.kind))
    });

    let mut result: Vec<Interval> = Vec::new();
    for interval in order {
        match result.last_mut() {
            Some(previous) if previous.end == interval.start && previous.kind == interval.kind => {
                previous.end = interval.end;
                if previous.evidence != interval.evidence {
                    previous.evidence = format!(
                        "adjacent audited {} spans; see namespace evidence",
                        interval.kind
                    );
                }
            }
            _ => result.push(interval.clone()),
        }
    }
    let diagnostics = overlap_diagnostics(&result)?;
    if !diagnostics.is_empty() {
        return Err(diagnostics.join("\n"));
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn item(start: i64, end: i64) -> Interval {
        Interval::new(start, end, "thumb", "test")
    }

    fn kinded(start: i64, end: i64, kind: &str) -> Interval {
        Interval::new(start, end, kind, "test")
    }

    #[test]
    fn union_matches_the_self_test_cases() {
        assert_eq!(interval_bytes(&[]).unwrap(), 0.0);
        assert_eq!(interval_bytes(&[item(0, 2), item(2, 4)]).unwrap(), 4.0);
        assert_eq!(interval_bytes(&[item(0, 8), item(2, 4)]).unwrap(), 8.0);
        assert_eq!(interval_bytes(&[item(0, 4), item(2, 6)]).unwrap(), 6.0);
    }

    #[test]
    fn overlap_diagnostics_ignore_identical_aliases() {
        assert_eq!(
            overlap_diagnostics(&[item(0, 4), item(2, 6)])
                .unwrap()
                .len(),
            1
        );
        assert_eq!(
            overlap_diagnostics(&[item(0, 4), item(0, 4)])
                .unwrap()
                .len(),
            0
        );
        // Same span, different evidence, is a real conflict.
        let mut other = item(0, 4);
        other.evidence = "elsewhere".to_string();
        assert_eq!(overlap_diagnostics(&[item(0, 4), other]).unwrap().len(), 1);
        assert_eq!(
            overlap_diagnostics(&[item(0x2000000, 0x2000008), item(0x2000004, 0x2000010)]).unwrap()
                [0],
            "0x2000004..0x2000008 overlaps thumb and thumb"
        );
    }

    #[test]
    fn fractional_bounds_are_rejected_not_truncated() {
        // A listing row with an odd hex-digit encoding column produces
        // `width = 1.5`; JS accepts the arithmetic and the validator rejects it.
        let fractional = Interval {
            start: 0.0,
            end: 1.5,
            kind: "thumb".to_string(),
            evidence: "test".to_string(),
        };
        assert_eq!(
            validate_intervals(&[fractional], "intervals").unwrap_err(),
            "intervals[0].end must be a non-negative safe integer"
        );
        // But `1.0` is a safe integer in JS, and must pass.
        assert!(validate_intervals(&[item(0, 1)], "intervals").is_ok());
    }

    #[test]
    fn empty_and_reversed_and_unclassified_are_rejected() {
        assert!(validate_intervals(&[item(4, 4)], "intervals").is_err());
        assert!(validate_intervals(&[item(6, 4)], "intervals").is_err());
        let blank = Interval {
            start: 0.0,
            end: 4.0,
            kind: String::new(),
            evidence: "x".into(),
        };
        assert_eq!(
            validate_intervals(&[blank], "x").unwrap_err(),
            "x[0] lacks classification evidence"
        );
    }

    #[test]
    fn subject_round_trips_and_rejects_non_canonical_markers() {
        assert_eq!(
            format_subject(123456.0, 1234567.0).unwrap(),
            "[ \u{2600}\u{fe0f} 123 / 1,234 ]"
        );
        let parsed = parse_subject("decomp: x [ \u{2600}\u{fe0f} 123 / 1,234 ]")
            .unwrap()
            .unwrap();
        assert_eq!(parsed.full_c_kilobytes, 123.0);
        assert_eq!(parsed.executable_kilobytes, 1234.0);
        for invalid in [
            "x [ \u{2600}\u{fe0f} 1234 / 1,234 ]",
            "x [ \u{2600}\u{fe0f} 123/1,234 ]",
            "x [ \u{2600}\u{fe0f} 123 / 1,234 bytes]",
            "x [C 123,456/1,234,567 bytes]",
            "x [ \u{2600}\u{fe0f} 2 / 1 ]",
            "x [123 of 456]",
        ] {
            let outcome = parse_subject(invalid);
            assert!(
                matches!(outcome, Ok(None) | Err(_)),
                "{invalid} should not parse to a subject"
            );
        }
        assert!(format_subject(2000.0, 1000.0).is_err());
    }

    #[test]
    fn kilobytes_floor_and_percent_round_half_up() {
        assert_eq!(kilobytes(1999.0).unwrap(), 1.0);
        assert_eq!(kilobytes(999.0).unwrap(), 0.0);
        assert_eq!(round_half_up_percent(1.0, 8.0).unwrap(), 12.5);
        assert!(round_half_up_percent(1.0, 0.0).is_err());
        assert_eq!(
            to_fixed_2(round_half_up_percent(1.0, 8.0).unwrap()),
            "12.50"
        );
        assert_eq!(to_fixed_2(23.56), "23.56");
    }

    #[test]
    fn merge_coalesces_same_kind_and_generalises_evidence() {
        let merged = merge_classified(&[
            Interval::new(0, 2, "thumb", "a"),
            Interval::new(2, 4, "thumb", "b"),
            Interval::new(4, 8, "literal_pool", "c"),
        ])
        .unwrap();
        assert_eq!(merged.len(), 2);
        assert_eq!((merged[0].start, merged[0].end), (0.0, 4.0));
        assert_eq!(
            merged[0].evidence,
            "adjacent audited thumb spans; see namespace evidence"
        );
        assert_eq!(merged[1].kind, "literal_pool");
    }

    #[test]
    fn merge_dedupes_on_start_end_kind_keeping_the_first_evidence() {
        let merged = merge_classified(&[
            Interval::new(0, 4, "thumb", "first"),
            Interval::new(0, 4, "thumb", "second"),
        ])
        .unwrap();
        assert_eq!(merged.len(), 1);
        assert_eq!(merged[0].evidence, "first");
    }

    #[test]
    fn merge_orders_equal_spans_by_kind() {
        let merged = merge_classified(&[kinded(0, 4, "thumb"), kinded(0, 4, "arm")]).unwrap_err();
        // Two different kinds over the same span is a real overlap, reported
        // with the byte-ordered kind first.
        assert_eq!(merged, "0x0..0x4 overlaps arm and thumb");
    }
}
