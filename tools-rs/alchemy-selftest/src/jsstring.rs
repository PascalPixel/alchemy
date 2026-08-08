//! The two JavaScript string behaviours `selfTest` depends on.

use std::cmp::Ordering;

/// `Array.prototype.sort()` with no comparator compares UTF-16 code units, not
/// bytes and not Unicode scalar values.
///
/// PORT NOTE -- THIS IS NOT `str::cmp`. Rust orders by UTF-8 bytes, which agrees
/// with UTF-16 order for everything below U+E000 but disagrees above it: a
/// supplementary character (U+10000 and up) is a surrogate pair beginning at
/// U+D800, so JavaScript sorts it *below* U+E000..U+FFFF while UTF-8 bytes sort
/// it above. Every stem `selfTest` sorts today is ASCII, where the two agree --
/// but "the inputs happen to be ASCII" is not a reason to write the wrong
/// comparator, and the test suite pins the difference so nobody simplifies this
/// back to `sort()`.
pub fn js_string_cmp(left: &str, right: &str) -> Ordering {
    let mut left_units = left.encode_utf16();
    let mut right_units = right.encode_utf16();
    loop {
        match (left_units.next(), right_units.next()) {
            (None, None) => return Ordering::Equal,
            (None, Some(_)) => return Ordering::Less,
            (Some(_), None) => return Ordering::Greater,
            (Some(a), Some(b)) => {
                if a != b {
                    return a.cmp(&b);
                }
            }
        }
    }
}

/// `[...new Set(items)].sort()`.
///
/// PORT NOTE -- THE DEDUPLICATION IS LOAD-BEARING. The TypeScript declares these
/// allowlists as `new Set([...])` over literal arrays, and at least one of those
/// arrays lists the same stem twice (`080c08a8` appears twice in
/// `GROUPED_DMA_STORE_SOURCES`). `selfTest` compares the spread-and-sorted set
/// against a 48-element literal, so the port collapses the duplicate exactly as
/// the `Set` constructor does. A plain sort of the backing slice yields 49
/// entries and fails.
pub fn sorted_set<S: AsRef<str>>(items: &[S]) -> Vec<String> {
    let mut unique: Vec<String> = Vec::new();
    for item in items {
        let item = item.as_ref();
        if !unique.iter().any(|seen| seen == item) {
            unique.push(item.to_string());
        }
    }
    unique.sort_by(|left, right| js_string_cmp(left, right));
    unique
}

/// `(Number.parseInt(stem, 16) + delta).toString(16).padStart(8, "0")`.
///
/// PORT NOTE: `u64`, not `u32` or `i32`. JavaScript has one number type and
/// `parseInt` of an eight-digit hex stem plus four cannot overflow a double, so
/// there is no wraparound to reproduce; widening keeps it that way instead of
/// saturating (`as i32`) or wrapping (`| 0`) at a boundary the TypeScript never
/// reaches. `padStart` pads and never truncates, and `{:08x}` does the same.
pub fn neighbour_stem(stem: &str, delta: u64) -> String {
    let value = u64::from_str_radix(stem, 16).expect("self-test stems are hexadecimal");
    format!("{:08x}", value + delta)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn utf16_order_disagrees_with_byte_order_above_u_e000() {
        // U+FFFD encodes as three UTF-8 bytes starting 0xEF; U+10000 encodes as
        // four starting 0xF0, so `str::cmp` puts U+10000 second. JavaScript
        // sees the surrogate pair 0xD800 0xDC00 and puts it FIRST.
        let bmp = "\u{fffd}";
        let supplementary = "\u{10000}";
        assert_eq!(bmp.cmp(supplementary), Ordering::Less);
        assert_eq!(js_string_cmp(bmp, supplementary), Ordering::Greater);
    }

    #[test]
    fn utf16_order_agrees_with_byte_order_on_the_stems_we_actually_sort() {
        let stems = ["080fb670", "08006a00", "080f9a50"];
        let mut by_bytes = stems.to_vec();
        by_bytes.sort_unstable();
        assert_eq!(sorted_set(&stems), by_bytes);
    }

    #[test]
    fn the_set_constructor_collapses_the_duplicate_entry() {
        let with_duplicate = ["080c08a8", "08002f10", "080c08a8"];
        assert_eq!(sorted_set(&with_duplicate), ["08002f10", "080c08a8"]);
        // A plain sort would leave three, which is the failure mode this
        // function exists to prevent.
        assert_eq!(with_duplicate.len(), 3);
    }

    #[test]
    fn the_live_grouped_dma_table_really_does_contain_a_duplicate() {
        // Pins the reason `sorted_set` cannot be simplified to a sort: the
        // TypeScript literal lists `080c08a8` twice and `new Set` hides it.
        let table = alchemy_routing::routing_data::GROUPED_DMA_STORE_SOURCES;
        assert_eq!(table.len(), 50);
        assert_eq!(sorted_set(table).len(), 49);
    }

    #[test]
    fn neighbour_stem_pads_and_does_not_wrap() {
        assert_eq!(neighbour_stem("08095290", 4), "08095294");
        assert_eq!(neighbour_stem("0800bffc", 4), "0800c000");
        // Well past `i32::MAX`, where `as i32` would saturate and `| 0` wrap.
        assert_eq!(neighbour_stem("fffffffc", 4), "100000000");
    }
}
