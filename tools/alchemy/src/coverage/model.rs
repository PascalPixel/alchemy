//! The treemap model the README's file map draws.
/// One tracked file, or a folder of them.
#[derive(Clone, Debug, Default)]
pub struct Tile {
    pub label: String,
    pub bytes: i64,
    /// A file's lowercase extension, `s-credited` for assembly its header
    /// credits; empty for a folder.
    pub extension: String,
    pub source: Option<String>,
    pub children: Vec<Tile>,
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
            out.push(Placed {
                index: weighted[0].0,
                rect: frame,
            });
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
            (
                Rect { width, ..frame },
                Rect {
                    x: frame.x + width,
                    width: frame.width - width,
                    ..frame
                },
            )
        } else {
            let height = frame.height * share;
            (
                Rect { height, ..frame },
                Rect {
                    y: frame.y + height,
                    height: frame.height - height,
                    ..frame
                },
            )
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
        let frame = Rect {
            x: 3.0,
            y: 7.0,
            width: 120.0,
            height: 80.0,
        };
        let placed = treemap(&[1, 1, 1, 1], |n| *n, frame);
        assert_eq!(placed.len(), 4);
        assert!(placed.iter().any(|p| p.rect.width < frame.width));
        assert!(placed.iter().any(|p| p.rect.height < frame.height));
        let area: f64 = placed.iter().map(|p| p.rect.width * p.rect.height).sum();
        assert!((area - frame.width * frame.height).abs() < 0.001);
    }
    #[test]
    fn treemap_ignores_non_positive_weights_and_keeps_indexes() {
        let placed = treemap(
            &[4, 0, -2, 6],
            |n| *n,
            Rect {
                x: 0.0,
                y: 0.0,
                width: 100.0,
                height: 50.0,
            },
        );
        assert_eq!(placed.iter().map(|p| p.index).collect::<Vec<_>>(), [0, 3]);
    }
}
