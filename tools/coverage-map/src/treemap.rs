// Squarified treemap placement (Bruls, Huizing, van Wijk).
//
// PORT NOTE: the TypeScript returns `{item, rect}` pairs. This returns
// `{index, rect}` into the caller's slice instead, which is the same
// information without forcing every element type to be `Clone`. Placement
// order is identical, and placement order is output-visible: it decides which
// leaf is drawn where in four tracked SVG files.
//
// PORT NOTE (comparator): the sort key is `value(right) - value(left) ||
// left.index - right.index`. A JS `||` falls through on `0`, `-0` and `NaN`
// alike. Every value here is a finite byte count, so only the `0` case can
// occur, and the explicit index tiebreak reproduces it exactly. `partial_cmp`
// is deliberately not used: it would return `None` on a NaN and callers
// commonly `unwrap`, which would panic where JS quietly treats the result as 0.
//
// PORT NOTE (aspect ratio): `thickness / side` and `side / thickness` divide.
// `scale`, `short` and every value are strictly positive by the guards above
// them, so neither quotient can become `Infinity` and the `Infinity - Infinity`
// NaN case cannot arise. `js_worse` still compares the JS way rather than via
// `f64::max` so that a future zero-area row degrades the same way JS would.

#[derive(Clone, Copy, Debug, PartialEq)]
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

/// `Math.max(worst, ...)`. Extracted rather than inlined so clippy cannot
/// suggest `f64::max`, which differs from JS `Math.max` on NaN.
fn js_max(left: f64, right: f64) -> f64 {
    if left.is_nan() || right.is_nan() {
        return f64::NAN;
    }
    if right > left {
        right
    } else {
        left
    }
}

/// Place every positive-valued item of `items` inside `rect`.
pub fn squarify<T>(
    items: &[T],
    value: impl Fn(&T) -> f64,
    rect: Rect,
) -> Result<Vec<Placed>, String> {
    let mut placed: Vec<Placed> = Vec::new();
    let mut queue: Vec<usize> = (0..items.len())
        .filter(|index| value(&items[*index]) > 0.0)
        .collect();
    // `Array#sort` is stable; `sort_by` is too, and the explicit index tiebreak
    // makes the result independent of that anyway.
    queue.sort_by(|left, right| {
        let difference = value(&items[*right]) - value(&items[*left]);
        if difference < 0.0 {
            std::cmp::Ordering::Less
        } else if difference > 0.0 {
            std::cmp::Ordering::Greater
        } else {
            left.cmp(right)
        }
    });
    let total_value: f64 = queue.iter().map(|index| value(&items[*index])).sum();
    if queue.is_empty() || total_value <= 0.0 || rect.width <= 0.0 || rect.height <= 0.0 {
        return Ok(placed);
    }
    let mut free = rect;
    let scale = (rect.width * rect.height) / total_value;
    let mut index = 0usize;
    while index < queue.len() {
        let short = if free.width < free.height {
            free.width
        } else {
            free.height
        };
        if short <= 0.0 {
            return Err("treemap exhausted its rectangle before placing every item".to_string());
        }
        let mut row: Vec<usize> = Vec::new();
        let mut row_value = 0.0f64;
        let mut best = f64::INFINITY;
        while index < queue.len() {
            let candidate = value(&items[queue[index]]) * scale;
            let next_value = row_value + candidate;
            let thickness = next_value / short;
            let mut worst_next = 0.0f64;
            for member in row.iter().chain(std::iter::once(&queue[index])) {
                let side = (value(&items[*member]) * scale) / thickness;
                worst_next = js_max(worst_next, js_max(thickness / side, side / thickness));
            }
            if !row.is_empty() && worst_next > best {
                break;
            }
            row.push(queue[index]);
            row_value = next_value;
            best = worst_next;
            index += 1;
        }
        let final_row = index == queue.len();
        let horizontal = free.width >= free.height;
        let thickness = if final_row {
            if horizontal {
                free.width
            } else {
                free.height
            }
        } else {
            row_value / short
        };
        let raw_row_value: f64 = row.iter().map(|member| value(&items[*member])).sum();
        let mut offset = 0.0f64;
        let last = row.len() - 1;
        for (row_index, member) in row.iter().enumerate() {
            // Shared final boundaries prevent accumulated floating-point drift
            // from producing hairline gaps. The final row consumes the exact
            // remainder.
            let side = if row_index == last {
                short - offset
            } else if final_row {
                short * value(&items[*member]) / raw_row_value
            } else {
                (value(&items[*member]) * scale) / thickness
            };
            placed.push(Placed {
                index: *member,
                rect: if horizontal {
                    Rect {
                        x: free.x,
                        y: free.y + offset,
                        width: thickness,
                        height: side,
                    }
                } else {
                    Rect {
                        x: free.x + offset,
                        y: free.y,
                        width: side,
                        height: thickness,
                    }
                },
            });
            offset += side;
        }
        free = if horizontal {
            Rect {
                x: free.x + thickness,
                y: free.y,
                width: free.width - thickness,
                height: free.height,
            }
        } else {
            Rect {
                x: free.x,
                y: free.y + thickness,
                width: free.width,
                height: free.height - thickness,
            }
        };
        if final_row {
            break;
        }
    }
    if placed.len() != queue.len() {
        return Err("treemap did not place every positive item".to_string());
    }
    Ok(placed)
}

#[cfg(test)]
mod tests {
    use super::{squarify, Rect};

    fn identity(value: &f64) -> f64 {
        *value
    }

    #[test]
    fn the_placement_fills_the_rectangle_and_stays_inside_it() {
        let items = vec![6.0, 6.0, 4.0, 3.0, 2.0, 2.0, 1.0];
        let frame = Rect {
            x: 4.0,
            y: 7.0,
            width: 120.0,
            height: 80.0,
        };
        let placed = squarify(&items, identity, frame).expect("placement");
        assert_eq!(placed.len(), items.len());
        let area: f64 = placed.iter().map(|p| p.rect.width * p.rect.height).sum();
        assert!((area - 120.0 * 80.0).abs() < 1e-6, "area was {area}");
        for entry in &placed {
            assert!(entry.rect.x >= frame.x - 1e-9);
            assert!(entry.rect.y >= frame.y - 1e-9);
            assert!(entry.rect.x + entry.rect.width <= frame.x + frame.width + 1e-9);
            assert!(entry.rect.y + entry.rect.height <= frame.y + frame.height + 1e-9);
        }
    }

    #[test]
    fn zero_and_negative_items_are_dropped_not_placed() {
        let items = vec![0.0, 5.0, -3.0, 5.0];
        let placed = squarify(
            &items,
            identity,
            Rect {
                x: 0.0,
                y: 0.0,
                width: 10.0,
                height: 10.0,
            },
        )
        .expect("placement");
        assert_eq!(placed.len(), 2);
        assert_eq!(placed[0].index, 1);
        assert_eq!(placed[1].index, 3);
    }

    #[test]
    fn equal_values_keep_their_original_index_order() {
        // The `|| left.index - right.index` tiebreak. Without it a different
        // sort could reorder equal leaves and move them in the tracked SVGs.
        let items = vec![1.0; 6];
        let placed = squarify(
            &items,
            identity,
            Rect {
                x: 0.0,
                y: 0.0,
                width: 60.0,
                height: 40.0,
            },
        )
        .expect("placement");
        let order: Vec<usize> = placed.iter().map(|p| p.index).collect();
        assert_eq!(order, vec![0, 1, 2, 3, 4, 5]);
    }

    #[test]
    fn an_empty_or_degenerate_rectangle_places_nothing() {
        let items = vec![1.0, 2.0];
        for frame in [
            Rect {
                x: 0.0,
                y: 0.0,
                width: 0.0,
                height: 10.0,
            },
            Rect {
                x: 0.0,
                y: 0.0,
                width: 10.0,
                height: 0.0,
            },
        ] {
            assert!(squarify(&items, identity, frame)
                .expect("placement")
                .is_empty());
        }
        assert!(squarify::<f64>(
            &[],
            identity,
            Rect {
                x: 0.0,
                y: 0.0,
                width: 4.0,
                height: 4.0
            }
        )
        .expect("placement")
        .is_empty());
    }

    #[test]
    fn a_sub_half_pixel_leaf_is_still_placed() {
        let items = vec![100000.0, 1.0];
        let placed = squarify(
            &items,
            identity,
            Rect {
                x: 0.0,
                y: 0.0,
                width: 40.0,
                height: 20.0,
            },
        )
        .expect("placement");
        assert_eq!(placed.len(), 2);
        assert!(placed[1].rect.width > 0.0 && placed[1].rect.height > 0.0);
    }
}
