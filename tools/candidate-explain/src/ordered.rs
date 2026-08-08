//! Insertion-ordered `Set` and `Map` replacements.
//!
//! PORT NOTE -- `HashSet` / `HashMap` are never used in this crate. JavaScript
//! `Set` and `Map` iterate in insertion order, and that order is load-bearing
//! across this tree; a hashed container would make output depend on the hash
//! seed. Where a JavaScript `Set` silently deduplicates, these do too, and the
//! deduplication is spelled out rather than inherited from a container whose
//! semantics a reader would have to guess at.

/// `new Set<f64>()` over integral keys.
#[derive(Debug, Default, Clone)]
pub struct OrderedSet {
    order: Vec<i64>,
    sorted: Vec<i64>,
}

impl OrderedSet {
    pub fn new() -> Self {
        Self::default()
    }

    /// `set.add(value)` -- a repeat insertion is dropped and does NOT move the
    /// existing entry to the end.
    pub fn add(&mut self, value: i64) {
        match self.sorted.binary_search(&value) {
            Ok(_) => {}
            Err(at) => {
                self.sorted.insert(at, value);
                self.order.push(value);
            }
        }
    }

    /// `set.has(value)`.
    pub fn has(&self, value: i64) -> bool {
        self.sorted.binary_search(&value).is_ok()
    }

    /// `set.size`.
    pub fn size(&self) -> usize {
        self.order.len()
    }

    /// Iteration in insertion order, as `for (const x of set)` gives.
    pub fn iter(&self) -> std::slice::Iter<'_, i64> {
        self.order.iter()
    }
}

/// `new Map<i64, V>()`.
#[derive(Debug, Default, Clone)]
pub struct OrderedMap<V> {
    entries: Vec<(i64, V)>,
}

impl<V> OrderedMap<V> {
    pub fn new() -> Self {
        Self {
            entries: Vec::new(),
        }
    }

    /// `map.set(key, value)` -- an existing key keeps its original position and
    /// takes the new value, which is what `new Map(pairs)` does when `pairs`
    /// carries a duplicate key.
    pub fn set(&mut self, key: i64, value: V) {
        if let Some(slot) = self.entries.iter_mut().find(|(k, _)| *k == key) {
            slot.1 = value;
            return;
        }
        self.entries.push((key, value));
    }

    /// `map.get(key)`.
    pub fn get(&self, key: i64) -> Option<&V> {
        self.entries.iter().find(|(k, _)| *k == key).map(|(_, v)| v)
    }

    pub fn len(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }
}
