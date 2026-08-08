//! JavaScript operators that Rust spells differently.

use std::cmp::Ordering;

/// JavaScript `a - b || <tiebreak>` inside an `Array#sort` comparator.
///
/// PORT NOTE -- THIS IS NOT `partial_cmp` AND NOT `total_cmp`. Two things make
/// the naive forms wrong:
///
///  * `Infinity - Infinity` is `NaN`, and JavaScript's `||` treats `NaN` as
///    falsy, so the comparator *falls through to its tiebreak* rather than
///    reporting a difference. `partial_cmp` returns `None` (usually unwrapped
///    into a panic or an arbitrary order) and `total_cmp` reports `Equal` only
///    by accident of the bit pattern, ordering `NaN` after `Infinity`.
///  * A comparator that returns `NaN` is treated by the sort spec as `0`, which
///    is the same fall-through.
///
/// `mode_cohort.ts` chains four of these per comparator. Returning `Equal` for
/// both `0` and `NaN` is exactly the falsy set.
pub fn js_diff(left: f64, right: f64) -> Ordering {
    let difference = left - right;
    if difference.is_nan() || difference == 0.0 {
        Ordering::Equal
    } else if difference < 0.0 {
        Ordering::Less
    } else {
        Ordering::Greater
    }
}

/// `Math.min(seed, ...rest)`.
///
/// PORT NOTE -- THIS IS NOT `f64::min` AND NOT `fold(f64::min)`. `Math.min`
/// propagates `NaN`; `f64::min` deliberately *discards* it and returns the
/// other operand. `mode_cohort.ts` feeds this the scores of every compiled
/// proper subset, so one non-numeric score has to poison the bound rather than
/// vanish from it.
pub fn js_min(seed: f64, rest: &[f64]) -> f64 {
    let mut best = seed;
    for &value in rest {
        if value.is_nan() || best.is_nan() {
            return f64::NAN;
        }
        if value < best {
            best = value;
        }
    }
    best
}

/// An insertion-ordered string-keyed map.
///
/// PORT NOTE -- NEVER `HashMap`. `mode_cohort.ts` iterates `configs.values()`
/// and `grouped.values()` directly into the report, and a JavaScript `Map`
/// iterates in insertion order. A `HashMap` would emit a different report on
/// every run, and even `BTreeMap` would emit the wrong one.
#[derive(Debug, Clone, Default)]
pub struct OrderedMap<V> {
    entries: Vec<(String, V)>,
}

impl<V> OrderedMap<V> {
    pub fn new() -> Self {
        Self {
            entries: Vec::new(),
        }
    }

    pub fn get(&self, key: &str) -> Option<&V> {
        self.entries.iter().find(|(k, _)| k == key).map(|(_, v)| v)
    }

    pub fn len(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }

    /// `map.set(key, value)`: replaces in place, keeping the original position.
    pub fn set(&mut self, key: &str, value: V) {
        match self.entries.iter_mut().find(|(k, _)| k == key) {
            Some(slot) => slot.1 = value,
            None => self.entries.push((key.to_string(), value)),
        }
    }

    /// `map.get(key) ?? make()` followed by `map.set(key, row)`.
    pub fn entry_or_insert_with(&mut self, key: &str, make: impl FnOnce() -> V) -> &mut V {
        if let Some(position) = self.entries.iter().position(|(k, _)| k == key) {
            return &mut self.entries[position].1;
        }
        self.entries.push((key.to_string(), make()));
        let last = self.entries.len() - 1;
        &mut self.entries[last].1
    }

    pub fn values(&self) -> impl Iterator<Item = &V> {
        self.entries.iter().map(|(_, v)| v)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn infinity_minus_infinity_falls_through_instead_of_ordering() {
        assert_eq!(js_diff(f64::INFINITY, f64::INFINITY), Ordering::Equal);
        // What the naive ports would say instead.
        assert_eq!(
            f64::INFINITY.partial_cmp(&f64::INFINITY),
            Some(Ordering::Equal)
        );
        assert_eq!(js_diff(f64::NAN, 1.0), Ordering::Equal);
        assert_eq!(f64::NAN.total_cmp(&1.0), Ordering::Greater);
    }

    #[test]
    fn js_diff_orders_ordinary_numbers() {
        assert_eq!(js_diff(1.0, 2.0), Ordering::Less);
        assert_eq!(js_diff(2.0, 1.0), Ordering::Greater);
        assert_eq!(js_diff(2.0, 2.0), Ordering::Equal);
        assert_eq!(js_diff(-0.0, 0.0), Ordering::Equal);
    }

    #[test]
    fn math_min_propagates_nan_where_f64_min_swallows_it() {
        assert!(js_min(3.0, &[f64::NAN, 1.0]).is_nan());
        assert_eq!(f64::min(3.0, f64::NAN), 3.0);
        assert_eq!(js_min(3.0, &[]), 3.0);
        assert_eq!(js_min(3.0, &[5.0, 1.0]), 1.0);
    }

    #[test]
    fn ordered_map_keeps_insertion_order_under_overwrite() {
        let mut map: OrderedMap<u32> = OrderedMap::new();
        map.set("zeta", 1);
        map.set("alpha", 2);
        map.set("zeta", 3);
        assert_eq!(map.values().copied().collect::<Vec<_>>(), vec![3, 2]);
        assert_eq!(map.len(), 2);
        assert!(!map.is_empty());
    }
}
