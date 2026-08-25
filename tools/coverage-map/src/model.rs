//! The one byte-range and treemap model shared by progress and coverage.

#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub struct Span {
    pub start: i64,
    pub end: i64,
}

impl Span {
    pub const fn new(start: i64, end: i64) -> Self {
        Self { start, end }
    }
    pub const fn bytes(self) -> i64 {
        self.end - self.start
    }
}

pub fn bytes(spans: &[Span]) -> i64 {
    spans.iter().map(|s| s.bytes()).sum()
}

pub fn normalize(input: &[Span]) -> Vec<Span> {
    let mut spans: Vec<_> = input.iter().copied().filter(|s| s.end > s.start).collect();
    spans.sort_by_key(|s| (s.start, s.end));
    let mut out: Vec<Span> = Vec::with_capacity(spans.len());
    for span in spans {
        match out.last_mut() {
            Some(last) if span.start <= last.end => last.end = last.end.max(span.end),
            _ => out.push(span),
        }
    }
    out
}

pub fn intersect(left: &[Span], right: &[Span]) -> Vec<Span> {
    let left = normalize(left);
    let right = normalize(right);
    let mut out = Vec::new();
    for a in left {
        for b in &right {
            if b.start >= a.end {
                break;
            }
            if let Some(span) = (a.start.max(b.start) < a.end.min(b.end)).then(|| Span::new(a.start.max(b.start), a.end.min(b.end))) {
                out.push(span);
            }
        }
    }
    normalize(&out)
}

pub fn subtract(input: &[Span], cuts: &[Span]) -> Vec<Span> {
    let cuts = normalize(cuts);
    let mut out = Vec::new();
    for span in normalize(input) {
        let mut cursor = span.start;
        for cut in &cuts {
            if cut.end <= cursor {
                continue;
            }
            if cut.start >= span.end {
                break;
            }
            if cut.start > cursor {
                out.push(Span::new(cursor, cut.start.min(span.end)));
            }
            cursor = cursor.max(cut.end);
        }
        if cursor < span.end {
            out.push(Span::new(cursor, span.end));
        }
    }
    out
}

pub const CATEGORIES: [&str; 5] = ["exact_c", "tracked_c", "assembly", "retained_asm", "asset_data"];

fn category_index(name: &str) -> Option<usize> {
    CATEGORIES.iter().position(|item| *item == name)
}

#[derive(Clone, Debug, Default)]
pub struct Tile {
    pub label: String,
    pub bytes: i64,
    pub categories: [i64; 5],
    pub group: Option<String>,
    pub subgroup: Option<String>,
    pub address: Option<i64>,
}

impl Tile {
    pub fn category(&self, name: &str) -> i64 {
        category_index(name).map_or(0, |i| self.categories[i])
    }
    pub fn set(&mut self, name: &str, value: i64) {
        if let Some(i) = category_index(name) {
            self.categories[i] = value;
        }
    }
    pub fn add(&mut self, name: &str, value: i64) {
        if let Some(i) = category_index(name) {
            self.categories[i] += value;
        }
    }
}

#[derive(Clone, Debug, Default)]
pub struct Area {
    pub id: String,
    pub label: String,
    pub bytes: i64,
    pub categories: [i64; 5],
    pub tiles: Vec<Tile>,
}

pub fn area(id: &str, label: &str, tiles: Vec<Tile>) -> Area {
    let mut categories = [0; 5];
    let bytes = tiles.iter().map(|tile| tile.bytes).sum();
    for tile in &tiles {
        for (slot, value) in categories.iter_mut().zip(tile.categories) {
            *slot += value;
        }
    }
    Area { id: id.into(), label: label.into(), bytes, categories, tiles: tiles.into_iter().filter(|t| t.bytes > 0).collect() }
}

#[derive(Clone, Copy, Debug, Default)]
pub struct Rect {
    pub x: f64,
    pub y: f64,
    pub width: f64,
    pub height: f64,
}

#[derive(Clone, Copy, Debug)]
pub struct Placed {
    pub index: usize,
    pub rect: Rect,
}

/// Deterministic, order-preserving binary treemap. Each partition is split
/// across its longest edge near half of its total weight, producing readable
/// two-dimensional blocks without changing proportional byte accounting.
pub fn treemap<T, F: Fn(&T) -> i64>(items: &[T], weight: F, frame: Rect) -> Vec<Placed> {
    fn place(weighted: &[(usize, i64)], frame: Rect, out: &mut Vec<Placed>) {
        if weighted.is_empty() {
            return;
        }
        if weighted.len() == 1 {
            out.push(Placed { index: weighted[0].0, rect: frame });
            return;
        }

        let total: i64 = weighted.iter().map(|(_, n)| *n).sum();
        let mut first = weighted[0].1;
        let mut split = 1;
        while split + 1 < weighted.len() && (first + weighted[split].1) * 2 <= total {
            first += weighted[split].1;
            split += 1;
        }
        let share = first as f64 / total as f64;
        let (a, b) = if frame.width >= frame.height {
            let width = frame.width * share;
            (Rect { width, ..frame }, Rect { x: frame.x + width, width: frame.width - width, ..frame })
        } else {
            let height = frame.height * share;
            (Rect { height, ..frame }, Rect { y: frame.y + height, height: frame.height - height, ..frame })
        };
        place(&weighted[..split], a, out);
        place(&weighted[split..], b, out);
    }

    let weighted: Vec<_> = items
        .iter()
        .enumerate()
        .filter_map(|(index, item)| {
            let n = weight(item);
            (n > 0).then_some((index, n))
        })
        .collect();
    let mut out = Vec::with_capacity(weighted.len());
    place(&weighted, frame, &mut out);
    out.sort_by_key(|placed| placed.index);
    out
}

#[cfg(test)]
mod tests {
    use super::{treemap, Rect};

    #[test]
    fn treemap_uses_both_dimensions_and_preserves_area() {
        let frame = Rect { x: 3.0, y: 7.0, width: 120.0, height: 80.0 };
        let placed = treemap(&[1, 1, 1, 1], |n| *n, frame);
        assert_eq!(placed.len(), 4);
        assert!(placed.iter().any(|p| p.rect.width < frame.width));
        assert!(placed.iter().any(|p| p.rect.height < frame.height));
        let area: f64 = placed.iter().map(|p| p.rect.width * p.rect.height).sum();
        assert!((area - frame.width * frame.height).abs() < 0.001);
    }

    #[test]
    fn treemap_ignores_non_positive_weights_and_keeps_indexes() {
        let placed = treemap(&[4, 0, -2, 6], |n| *n, Rect { x: 0.0, y: 0.0, width: 100.0, height: 50.0 });
        assert_eq!(placed.iter().map(|p| p.index).collect::<Vec<_>>(), [0, 3]);
    }
}
