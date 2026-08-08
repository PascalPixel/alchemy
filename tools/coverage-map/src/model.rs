// The drawable vocabulary: categories, tiles, areas.
//
// WHY: every later stage -- tiling, treemap, SVG, JSON -- agrees on these three
// shapes, and the category *order* is load-bearing twice over. It is the
// stacking order inside a tile (exact C at the bottom) and the iteration order
// when category totals are written out, so it is a fixed list rather than a
// map that some future edit could re-sort.

use crate::ordered::OrderedMap;

pub const CATEGORY_ORDER: [&str; 5] = [
    "exact_c",
    "semantic_c",
    "assembly",
    "retained_asm",
    "asset_data",
];

pub const ASSET_TIERS: [&str; 4] = ["asset_bytes", "asset_bw", "asset_color", "asset_objects"];

/// One drawable leaf: a byte run of the image with its category composition.
#[derive(Clone, Debug, Default)]
pub struct Tile {
    pub label: String,
    pub bytes: i64,
    /// PORT NOTE: this is a JS object literal, so member order is insertion
    /// order and reaches the emitted JSON verbatim. An `OrderedMap` is the only
    /// faithful backing; a `BTreeMap` would silently alphabetise the keys.
    pub categories: OrderedMap<String, i64>,
    pub group: Option<String>,
    pub subgroup: Option<String>,
    pub address: Option<i64>,
}

impl Tile {
    pub fn category(&self, name: &str) -> i64 {
        self.categories.get(&name.to_string()).copied().unwrap_or(0)
    }

    pub fn set_category(&mut self, name: &str, bytes: i64) {
        self.categories.insert(name.to_string(), bytes);
    }

    pub fn add_category(&mut self, name: &str, bytes: i64) {
        let total = self.category(name) + bytes;
        self.categories.insert(name.to_string(), total);
    }
}

#[derive(Clone, Debug, Default)]
pub struct Area {
    pub id: String,
    pub label: String,
    pub bytes: i64,
    pub categories: OrderedMap<String, i64>,
    pub tiles: Vec<Tile>,
}

pub fn category_total(tiles: &[Tile], category: &str) -> i64 {
    tiles.iter().map(|tile| tile.category(category)).sum()
}

/// `area(id, label, tiles)`.
///
/// PORT NOTE: a zero total is skipped (`if (bytes)`), so an area that touches a
/// category only with zero bytes omits the key rather than writing `0`. Empty
/// tiles are dropped from the drawn list but still counted in `bytes`, which is
/// summed before the filter -- reproduced in that order below.
pub fn area(id: &str, label: &str, tiles: Vec<Tile>) -> Area {
    let mut categories = OrderedMap::new();
    for category in CATEGORY_ORDER {
        let bytes = category_total(&tiles, category);
        if bytes != 0 {
            categories.insert(category.to_string(), bytes);
        }
    }
    let bytes = tiles.iter().map(|tile| tile.bytes).sum();
    Area {
        id: id.to_string(),
        label: label.to_string(),
        bytes,
        categories,
        tiles: tiles.into_iter().filter(|tile| tile.bytes > 0).collect(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn area_omits_zero_categories_and_drops_empty_tiles() {
        let mut solid = Tile {
            label: "a".into(),
            bytes: 10,
            ..Tile::default()
        };
        solid.set_category("exact_c", 10);
        solid.set_category("assembly", 0);
        let empty = Tile {
            label: "b".into(),
            bytes: 0,
            ..Tile::default()
        };
        let built = area("main", "Main image", vec![solid, empty]);
        assert_eq!(built.bytes, 10);
        assert_eq!(built.tiles.len(), 1, "a zero-byte tile is not drawn");
        let keys: Vec<&String> = built.categories.keys().collect();
        assert_eq!(keys, vec!["exact_c"], "a zero total omits the key entirely");
    }

    #[test]
    fn categories_keep_the_stacking_order_not_alphabetical_order() {
        let mut tile = Tile::default();
        tile.set_category("retained_asm", 1);
        tile.set_category("exact_c", 1);
        let keys: Vec<String> = tile.categories.keys().cloned().collect();
        assert_eq!(
            keys,
            vec!["retained_asm", "exact_c"],
            "insertion order, not sorted"
        );
    }
}
