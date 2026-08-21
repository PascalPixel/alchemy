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
            if let Some(span) = (a.start.max(b.start) < a.end.min(b.end))
                .then(|| Span::new(a.start.max(b.start), a.end.min(b.end)))
            {
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

pub fn contains(outer: &[Span], inner: Span) -> bool {
    normalize(outer)
        .iter()
        .any(|span| span.start <= inner.start && inner.end <= span.end)
}

pub const CATEGORIES: [&str; 5] = [
    "exact_c",
    "tracked_c",
    "assembly",
    "retained_asm",
    "asset_data",
];

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
    Area {
        id: id.into(),
        label: label.into(),
        bytes,
        categories,
        tiles: tiles.into_iter().filter(|t| t.bytes > 0).collect(),
    }
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

/// Deterministic slice-and-dice treemap. Both tools use this same geometry so
/// byte accounting and rendered ownership have one model.
pub fn treemap<T, F: Fn(&T) -> i64>(items: &[T], weight: F, frame: Rect) -> Vec<Placed> {
    let total: i64 = items.iter().map(&weight).sum();
    if total <= 0 {
        return Vec::new();
    }
    let horizontal = frame.width >= frame.height;
    let mut cursor = if horizontal { frame.x } else { frame.y };
    let mut out = Vec::with_capacity(items.len());
    for (index, item) in items.iter().enumerate() {
        let share = weight(item).max(0) as f64 / total as f64;
        let rect = if horizontal {
            let width = if index + 1 == items.len() {
                frame.x + frame.width - cursor
            } else {
                frame.width * share
            };
            let rect = Rect {
                x: cursor,
                y: frame.y,
                width,
                height: frame.height,
            };
            cursor += width;
            rect
        } else {
            let height = if index + 1 == items.len() {
                frame.y + frame.height - cursor
            } else {
                frame.height * share
            };
            let rect = Rect {
                x: frame.x,
                y: cursor,
                width: frame.width,
                height,
            };
            cursor += height;
            rect
        };
        out.push(Placed { index, rect });
    }
    out
}
