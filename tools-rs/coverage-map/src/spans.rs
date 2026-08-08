// Half-open byte-range algebra for the coverage map.
//
// This file exists because every layer of the map -- exact C, semantic C,
// retained assembly, asset data -- is expressed as a set of ROM byte ranges,
// and the map is only honest if those sets compose without double counting.
// `normalize` is the single canonical form (sorted, merged, non-empty) that
// `intersect` and `subtract` both consume and produce.
//
// PORT NOTE: the TypeScript carries these coordinates as JS numbers. Every
// value that reaches a `Span` here is a non-negative safe integer (ROM
// addresses and byte sizes, validated at each parse boundary before a span is
// constructed), so `i64` is exact for the whole algebra and avoids inheriting
// float comparison hazards that cannot actually occur. Callers that read
// untrusted JSON must run the value through `crate::jsnum::safe_integer`
// first, exactly where the TypeScript runs `Number.isSafeInteger`.

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Span {
    pub start: i64,
    pub end: i64,
}

impl Span {
    pub fn new(start: i64, end: i64) -> Self {
        Self { start, end }
    }

    pub fn bytes(&self) -> i64 {
        self.end - self.start
    }
}

/// `spans.reduce((sum, span) => sum + span.end - span.start, 0)`
pub fn span_bytes(spans: &[Span]) -> i64 {
    spans.iter().map(|span| span.end - span.start).sum()
}

/// Sorted, merged, empty-run-free form.
///
/// PORT NOTE: the TypeScript merges when `span.start <= previous.end`, so
/// abutting runs (`[0,4)` and `[4,8)`) collapse into one. That is deliberate --
/// contiguous ownership must not be counted as two tiles -- and is preserved.
pub fn normalize(spans: &[Span]) -> Vec<Span> {
    let mut sorted: Vec<Span> = spans.iter().copied().filter(|span| span.end > span.start).collect();
    // `Array.prototype.sort` is stable, and so is `sort_by`; the comparator is
    // `left.start - right.start || left.end - right.end`.
    sorted.sort_by(|left, right| left.start.cmp(&right.start).then(left.end.cmp(&right.end)));
    let mut result: Vec<Span> = Vec::new();
    for span in sorted {
        match result.last_mut() {
            Some(previous) if span.start <= previous.end => {
                previous.end = previous.end.max(span.end);
            }
            _ => result.push(span),
        }
    }
    result
}

/// The part of `spans` that lies inside `mask`.
///
/// PORT NOTE: the two-pointer walk keeps the TypeScript's backtracking loop
/// (`while (index > 0 && right[index - 1].end > span.start) index--`). It is
/// not dead code: `left` is sorted by start but successive spans can still
/// begin before the cursor's current mask entry ends when a long span was
/// followed by a short one starting inside it, and dropping the backtrack
/// silently loses coverage.
pub fn intersect(spans: &[Span], mask: &[Span]) -> Vec<Span> {
    let mut result: Vec<Span> = Vec::new();
    let left = normalize(spans);
    let right = normalize(mask);
    let mut index: usize = 0;
    for span in left {
        while index > 0 && right[index - 1].end > span.start {
            index -= 1;
        }
        while index < right.len() && right[index].end <= span.start {
            index += 1;
        }
        let mut cursor = index;
        while cursor < right.len() && right[cursor].start < span.end {
            let start = span.start.max(right[cursor].start);
            let end = span.end.min(right[cursor].end);
            if end > start {
                result.push(Span { start, end });
            }
            cursor += 1;
        }
    }
    normalize(&result)
}

/// `spans` minus `removed`.
pub fn subtract(spans: &[Span], removed: &[Span]) -> Vec<Span> {
    let mut result: Vec<Span> = Vec::new();
    let cuts = normalize(removed);
    for span in normalize(spans) {
        let mut start = span.start;
        for cut in &cuts {
            if cut.end <= start {
                continue;
            }
            if cut.start >= span.end {
                break;
            }
            if cut.start > start {
                result.push(Span { start, end: cut.start.min(span.end) });
            }
            start = start.max(cut.end);
        }
        if start < span.end {
            result.push(Span { start, end: span.end });
        }
    }
    normalize(&result)
}

/// Audited main-image region starts bracket every owned address; this is the
/// TypeScript's `regionSpan` binary search, `(low + high) >> 1` included.
pub fn region_span(address: i64, boundaries: &[i64], limit: i64) -> Span {
    let mut low = 0usize;
    let mut high = boundaries.len();
    while low < high {
        let middle = (low + high) >> 1;
        if boundaries[middle] <= address {
            low = middle + 1;
        } else {
            high = middle;
        }
    }
    Span { start: address, end: if low < boundaries.len() { boundaries[low] } else { limit } }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn s(start: i64, end: i64) -> Span {
        Span::new(start, end)
    }

    #[test]
    fn normalize_merges_overlapping_and_abutting_runs() {
        assert_eq!(normalize(&[s(4, 8), s(0, 4)]), vec![s(0, 8)]);
        assert_eq!(normalize(&[s(0, 8), s(2, 4)]), vec![s(0, 8)]);
        assert_eq!(normalize(&[s(0, 4), s(6, 8)]), vec![s(0, 4), s(6, 8)]);
    }

    #[test]
    fn normalize_drops_empty_and_reversed_runs() {
        assert_eq!(normalize(&[s(4, 4), s(8, 2)]), Vec::<Span>::new());
    }

    #[test]
    fn normalize_does_not_mutate_its_input_aliases() {
        // The TypeScript copies with `{ ...span }` before mutating `previous.end`;
        // a port that pushed a reference would corrupt the caller's array.
        let input = vec![s(0, 4), s(2, 9)];
        let merged = normalize(&input);
        assert_eq!(merged, vec![s(0, 9)]);
        assert_eq!(input, vec![s(0, 4), s(2, 9)]);
    }

    #[test]
    fn intersect_clips_to_the_mask() {
        assert_eq!(intersect(&[s(0, 16)], &[s(4, 8), s(12, 20)]), vec![s(4, 8), s(12, 16)]);
        assert_eq!(intersect(&[s(0, 4)], &[s(8, 12)]), Vec::<Span>::new());
    }

    #[test]
    fn intersect_backtracks_for_a_span_that_starts_inside_an_earlier_one() {
        // Exercises the `index--` loop: the long span advances the cursor past
        // mask entries the following short span still overlaps.
        let left = [s(0, 40), s(4, 8)];
        assert_eq!(intersect(&left, &[s(2, 6), s(30, 36)]), vec![s(2, 6), s(30, 36)]);
    }

    #[test]
    fn subtract_splits_and_clamps() {
        assert_eq!(subtract(&[s(0, 16)], &[s(4, 8)]), vec![s(0, 4), s(8, 16)]);
        assert_eq!(subtract(&[s(0, 16)], &[s(0, 16)]), Vec::<Span>::new());
        assert_eq!(subtract(&[s(0, 16)], &[s(12, 40)]), vec![s(0, 12)]);
        assert_eq!(subtract(&[s(0, 16)], &[]), vec![s(0, 16)]);
    }

    #[test]
    fn span_bytes_sums_lengths() {
        assert_eq!(span_bytes(&[s(0, 4), s(10, 16)]), 10);
        assert_eq!(span_bytes(&[]), 0);
    }

    #[test]
    fn region_span_brackets_by_the_next_boundary() {
        let boundaries = [0x100, 0x140, 0x200];
        assert_eq!(region_span(0x100, &boundaries, 0x400), s(0x100, 0x140));
        assert_eq!(region_span(0x120, &boundaries, 0x400), s(0x120, 0x140));
        assert_eq!(region_span(0x200, &boundaries, 0x400), s(0x200, 0x400));
        assert_eq!(region_span(0x000, &boundaries, 0x400), s(0x000, 0x100));
        assert_eq!(region_span(0x100, &[], 0x400), s(0x100, 0x400));
    }
}
