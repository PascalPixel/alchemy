// A Map that remembers insertion order.
//
// WHY: the coverage map's output order is load-bearing. Owners, overlays and
// asset ranges are collected into JavaScript `Map`s and then iterated to emit
// tiles, and tile order decides treemap placement, which decides the bytes of
// four tracked SVGs. A `HashMap` would scramble that and a `BTreeMap` would
// silently re-sort it; neither is the JavaScript semantics.
//
// PORT NOTE: `Map#set` on an existing key overwrites the value *in place*,
// keeping the key at its original position. `insert` below does the same.

#[derive(Clone, Debug)]
pub struct OrderedMap<K, V> {
    entries: Vec<(K, V)>,
}

impl<K: PartialEq, V> Default for OrderedMap<K, V> {
    fn default() -> Self {
        Self::new()
    }
}

impl<K: PartialEq, V> OrderedMap<K, V> {
    pub fn new() -> Self {
        Self { entries: Vec::new() }
    }

    pub fn get(&self, key: &K) -> Option<&V> {
        self.entries.iter().find(|(k, _)| k == key).map(|(_, v)| v)
    }

    pub fn contains_key(&self, key: &K) -> bool {
        self.entries.iter().any(|(k, _)| k == key)
    }

    /// `map.set(key, value)`: overwrite in place, preserving key position.
    pub fn insert(&mut self, key: K, value: V) {
        match self.entries.iter_mut().find(|(k, _)| *k == key) {
            Some(slot) => slot.1 = value,
            None => self.entries.push((key, value)),
        }
    }

    pub fn len(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }

    pub fn iter(&self) -> std::slice::Iter<'_, (K, V)> {
        self.entries.iter()
    }

    pub fn keys(&self) -> impl Iterator<Item = &K> {
        self.entries.iter().map(|(k, _)| k)
    }

    pub fn values(&self) -> impl Iterator<Item = &V> {
        self.entries.iter().map(|(_, v)| v)
    }
}

impl<'a, K: PartialEq, V> IntoIterator for &'a OrderedMap<K, V> {
    type Item = &'a (K, V);
    type IntoIter = std::slice::Iter<'a, (K, V)>;
    fn into_iter(self) -> Self::IntoIter {
        self.entries.iter()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn insertion_order_survives_an_overwrite() {
        let mut map: OrderedMap<&str, i32> = OrderedMap::new();
        map.insert("b", 1);
        map.insert("a", 2);
        map.insert("b", 3);
        let keys: Vec<&&str> = map.keys().collect();
        assert_eq!(keys, vec![&"b", &"a"], "an overwrite must not move the key");
        assert_eq!(map.get(&"b"), Some(&3));
        assert_eq!(map.len(), 2);
        assert!(map.contains_key(&"a"));
        assert!(!map.contains_key(&"c"));
    }
}
