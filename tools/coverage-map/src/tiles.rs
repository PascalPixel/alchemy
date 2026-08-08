// Turning span algebra into drawable leaves.
//
// WHY: the two views need different grains. The executable view wants one tile
// per audited owner boundary, because that is the unit a contributor can act
// on. The ROM view holds the whole cartridge, so the same regions are merged
// into display-sized bands -- otherwise the main image renders as a barcode.
// Both builders credit categories in the same precedence: exact C, then
// semantic C, then explicitly retained assembly, and only the remainder is
// unresolved assembly.

use crate::jsnum::hex8;
use crate::model::{Tile, CATEGORY_ORDER};
use crate::spans::{intersect, normalize, span_bytes, subtract, Span};

/// Merge executable runs into bands of about `target` bytes.
pub fn main_bands(
    executable: &[Span],
    exact: &[Span],
    semantic: &[Span],
    retained: &[Span],
    target: i64,
) -> Vec<Tile> {
    let mut tiles: Vec<Tile> = Vec::new();
    let mut current: Option<(i64, Vec<Span>)> = None;

    fn flush(
        current: &mut Option<(i64, Vec<Span>)>,
        tiles: &mut Vec<Tile>,
        exact: &[Span],
        semantic: &[Span],
        retained: &[Span],
    ) {
        let Some((start, spans)) = current.take() else {
            return;
        };
        if spans.is_empty() {
            return;
        }
        let bytes = span_bytes(&spans);
        let exact_bytes = span_bytes(&intersect(&spans, exact));
        let semantic_bytes = span_bytes(&intersect(&spans, semantic));
        // Retained spans may only claim bytes no C category already owns.
        let mut owned_input = exact.to_vec();
        owned_input.extend_from_slice(semantic);
        let owned = normalize(&owned_input);
        let retained_bytes = span_bytes(&intersect(&subtract(&spans, &owned), retained));
        let mut tile = Tile {
            label: hex8(start)[..6].to_string(),
            bytes,
            ..Tile::default()
        };
        tile.set_category("exact_c", exact_bytes);
        tile.set_category("semantic_c", semantic_bytes);
        tile.set_category(
            "assembly",
            (bytes - exact_bytes - semantic_bytes - retained_bytes).max(0),
        );
        tile.set_category("retained_asm", retained_bytes);
        tiles.push(tile);
    }

    for span in normalize(executable) {
        let mut start = span.start;
        while start < span.end {
            if current.is_none() {
                current = Some((start, Vec::new()));
            }
            let entry = current.as_mut().expect("just seeded");
            let room = target - span_bytes(&entry.1);
            // `max(room, 1)` guarantees forward progress even once the band is
            // already at or over target, so the loop cannot stall.
            let end = span.end.min(start + room.max(1));
            entry.1.push(Span::new(start, end));
            start = end;
            if span_bytes(&entry.1) >= target {
                flush(&mut current, &mut tiles, exact, semantic, retained);
            }
        }
    }
    flush(&mut current, &mut tiles, exact, semantic, retained);
    tiles
}

/// One tile per audited owner boundary, never a display-sized partition.
pub fn main_owner_tiles(
    executable: &[Span],
    boundaries: &[i64],
    exact: &[Span],
    semantic: &[Span],
    retained: &[Span],
) -> Vec<Tile> {
    let mut tiles: Vec<Tile> = Vec::new();
    let mut owned_input = exact.to_vec();
    owned_input.extend_from_slice(semantic);
    let owned = normalize(&owned_input);
    for executable_span in normalize(executable) {
        let mut bank_cuts: Vec<i64> = Vec::new();
        let mut address = (executable_span.start / 0x1_0000) * 0x1_0000 + 0x1_0000;
        while address < executable_span.end {
            bank_cuts.push(address);
            address += 0x1_0000;
        }
        // PORT NOTE: `[...new Set(...)]` keeps first-insertion order and then
        // the numeric sort decides. Deduping before the sort reproduces both.
        let mut cuts: Vec<i64> = vec![executable_span.start, executable_span.end];
        cuts.extend(bank_cuts);
        cuts.extend(
            boundaries
                .iter()
                .copied()
                .filter(|a| *a > executable_span.start && *a < executable_span.end),
        );
        let mut seen: Vec<i64> = Vec::new();
        for cut in cuts {
            if !seen.contains(&cut) {
                seen.push(cut);
            }
        }
        seen.sort_unstable();

        for window in seen.windows(2) {
            let span = Span::new(window[0], window[1]);
            let bytes = span.end - span.start;
            let one = [span];
            let exact_bytes = span_bytes(&intersect(&one, exact));
            let semantic_bytes = span_bytes(&intersect(&one, semantic));
            let retained_bytes = span_bytes(&intersect(&subtract(&one, &owned), retained));
            let mut tile = Tile {
                label: format!("0x{}\u{2013}0x{}", hex8(span.start), hex8(span.end)),
                bytes,
                group: Some(format!(
                    "{} · 64 KiB bank",
                    &hex8((span.start / 0x1_0000) * 0x1_0000)[..4]
                )),
                address: Some(span.start),
                ..Tile::default()
            };
            tile.set_category("exact_c", exact_bytes);
            tile.set_category("semantic_c", semantic_bytes);
            tile.set_category(
                "assembly",
                (bytes - exact_bytes - semantic_bytes - retained_bytes).max(0),
            );
            tile.set_category("retained_asm", retained_bytes);
            tiles.push(tile);
        }
    }
    tiles
}

fn short_overlay(overlay: &str) -> String {
    overlay
        .strip_prefix("resource_")
        .unwrap_or(overlay)
        .to_string()
}

/// One tile per source-backed overlay owner, plus one per contiguous run.
pub fn overlay_owner_tiles(
    overlay: &str,
    executable: &[Span],
    exact_owners: &[crate::ownership::OverlayOwner],
    semantic_owners: &[Span],
    retained: &[Span],
) -> Vec<Tile> {
    let extent = normalize(executable);
    let flattened: Vec<Span> = exact_owners
        .iter()
        .flat_map(|owner| owner.spans.iter().copied())
        .collect();
    let exact = normalize(&flattened);
    let name = short_overlay(overlay);
    let mut tiles: Vec<Tile> = Vec::new();

    for owner in exact_owners {
        let bytes = span_bytes(&intersect(&owner.spans, &extent));
        if bytes <= 0 {
            continue;
        }
        let mut tile = Tile {
            label: format!("{name} · {} · byte-exact C", owner.label),
            bytes,
            group: Some(name.clone()),
            address: Some(owner.entry),
            ..Tile::default()
        };
        tile.set_category("exact_c", bytes);
        tiles.push(tile);
    }

    let mut semantic_credited: Vec<Span> = Vec::new();
    for owner in semantic_owners {
        let credited = subtract(&intersect(&[*owner], &extent), &exact);
        let bytes = span_bytes(&credited);
        if bytes <= 0 {
            continue;
        }
        semantic_credited.extend(credited);
        let mut tile = Tile {
            label: format!("{name} · semantic owner 0x{}", hex8(owner.start)),
            bytes,
            group: Some(name.clone()),
            address: Some(owner.start),
            ..Tile::default()
        };
        tile.set_category("semantic_c", bytes);
        tiles.push(tile);
    }

    let mut owned_input = exact.clone();
    owned_input.extend(semantic_credited.iter().copied());
    let owned = normalize(&owned_input);
    let retained_credited = intersect(&subtract(&extent, &owned), retained);
    for span in &retained_credited {
        let bytes = span.end - span.start;
        let mut tile = Tile {
            label: format!(
                "{name} · retained structural assembly 0x{}\u{2013}0x{}",
                hex8(span.start),
                hex8(span.end)
            ),
            bytes,
            group: Some(name.clone()),
            address: Some(span.start),
            ..Tile::default()
        };
        tile.set_category("retained_asm", bytes);
        tiles.push(tile);
    }

    // Every remaining executable byte still comes from the checked-in overlay
    // assembly source, so it is a known byte-exact representation rather than
    // unknown data. It stays in the retained bucket until an owner lands.
    let mut consumed = owned.clone();
    consumed.extend(retained_credited.iter().copied());
    for span in subtract(&extent, &consumed) {
        let bytes = span.end - span.start;
        let mut tile = Tile {
            label: format!(
                "{name} · byte-exact assembly 0x{}\u{2013}0x{}",
                hex8(span.start),
                hex8(span.end)
            ),
            bytes,
            group: Some(name.clone()),
            address: Some(span.start),
            ..Tile::default()
        };
        tile.set_category("retained_asm", bytes);
        tiles.push(tile);
    }
    tiles
}

/// Merge neighbouring tiles until each carries at least `target` bytes.
pub fn group_tiles(tiles: &[Tile], target: i64) -> Vec<Tile> {
    let mut grouped: Vec<Tile> = Vec::new();
    let mut current: Option<Tile> = None;
    let mut first = String::new();
    let mut last = String::new();
    for tile in tiles {
        if current.is_none() {
            current = Some(Tile {
                label: tile.label.clone(),
                ..Tile::default()
            });
            first = tile.label.clone();
        }
        let entry = current.as_mut().expect("just seeded");
        entry.bytes += tile.bytes;
        last = tile.label.clone();
        for category in CATEGORY_ORDER {
            let bytes = tile.category(category);
            // A zero contribution never creates the key, so a merged run of
            // asset-free code tiles does not gain an empty `asset_data`.
            if bytes != 0 {
                entry.add_category(category, bytes);
            }
        }
        if entry.bytes >= target {
            entry.label = if first == last {
                first.clone()
            } else {
                format!("{first}\u{2013}{last}")
            };
            grouped.push(current.take().expect("just checked"));
        }
    }
    if let Some(mut entry) = current {
        entry.label = if first == last {
            first
        } else {
            format!("{first}\u{2013}{last}")
        };
        grouped.push(entry);
    }
    grouped
}

#[cfg(test)]
mod tests {
    use super::*;

    fn spans(runs: &[(i64, i64)]) -> Vec<Span> {
        runs.iter()
            .map(|(start, end)| Span::new(*start, *end))
            .collect()
    }

    #[test]
    fn main_bands_cuts_at_the_target_and_labels_by_prefix() {
        let executable = spans(&[(0x0800_0000, 0x0800_0300)]);
        let exact = spans(&[(0x0800_0000, 0x0800_0080)]);
        let bands = main_bands(&executable, &exact, &[], &[], 0x100);
        assert_eq!(bands.len(), 3, "0x300 bytes at 0x100 per band");
        assert_eq!(bands[0].label, "080000", "hex8 truncated to six characters");
        assert_eq!(bands[0].bytes, 0x100);
        assert_eq!(bands[0].category("exact_c"), 0x80);
        assert_eq!(bands[0].category("assembly"), 0x80);
        assert_eq!(
            bands[2].category("exact_c"),
            0,
            "later bands hold no exact C"
        );
        let total: i64 = bands.iter().map(|band| band.bytes).sum();
        assert_eq!(total, 0x300, "bands conserve every executable byte");
    }

    #[test]
    fn retained_never_outranks_owned_bytes() {
        let executable = spans(&[(0, 0x100)]);
        let exact = spans(&[(0, 0x80)]);
        let retained = spans(&[(0, 0x100)]);
        let bands = main_bands(&executable, &exact, &[], &retained, 0x1000);
        assert_eq!(bands.len(), 1);
        assert_eq!(bands[0].category("exact_c"), 0x80);
        assert_eq!(
            bands[0].category("retained_asm"),
            0x80,
            "only the unowned half"
        );
        assert_eq!(bands[0].category("assembly"), 0);
    }

    #[test]
    fn owner_tiles_cut_at_banks_and_boundaries() {
        let executable = spans(&[(0x0800_0000, 0x0802_0000)]);
        let tiles = main_owner_tiles(&executable, &[0x0800_8000], &[], &[], &[]);
        let starts: Vec<i64> = tiles
            .iter()
            .map(|tile| tile.address.expect("set"))
            .collect();
        assert_eq!(starts, vec![0x0800_0000, 0x0800_8000, 0x0801_0000]);
        assert_eq!(tiles[0].group.as_deref(), Some("0800 · 64 KiB bank"));
        assert_eq!(tiles[2].group.as_deref(), Some("0801 · 64 KiB bank"));
        assert_eq!(tiles[0].label, "0x08000000\u{2013}0x08008000");
        // A boundary outside the extent must not create a cut.
        let ignored = main_owner_tiles(&spans(&[(0, 0x10)]), &[0x40], &[], &[], &[]);
        assert_eq!(ignored.len(), 1);
    }

    #[test]
    fn overlay_tiles_credit_exact_then_semantic_then_retained_then_the_rest() {
        let owner = crate::ownership::OverlayOwner {
            label: "sub_1".to_string(),
            entry: 0x0300_0000,
            spans: spans(&[(0x0300_0000, 0x0300_0010)]),
        };
        let extent = spans(&[(0x0300_0000, 0x0300_0040)]);
        let semantic = spans(&[(0x0300_0000, 0x0300_0020)]);
        let retained = spans(&[(0x0300_0020, 0x0300_0030)]);
        let tiles = overlay_owner_tiles("resource_2a", &extent, &[owner], &semantic, &retained);
        let labels: Vec<&str> = tiles.iter().map(|tile| tile.label.as_str()).collect();
        assert_eq!(labels.len(), 4, "one tile per credited layer");
        assert!(
            labels[0].starts_with("2a · sub_1 · byte-exact C"),
            "prefix is stripped"
        );
        assert_eq!(tiles[0].category("exact_c"), 0x10);
        assert_eq!(
            tiles[1].category("semantic_c"),
            0x10,
            "the exact half is not re-credited"
        );
        assert_eq!(tiles[2].category("retained_asm"), 0x10);
        assert!(labels[3].contains("byte-exact assembly"));
        let total: i64 = tiles.iter().map(|tile| tile.bytes).sum();
        assert_eq!(
            total, 0x40,
            "the whole extent is accounted for exactly once"
        );
    }

    #[test]
    fn group_tiles_names_a_merged_run_by_its_ends() {
        let make = |label: &str, bytes: i64| {
            let mut tile = Tile {
                label: label.into(),
                bytes,
                ..Tile::default()
            };
            tile.set_category("exact_c", bytes);
            tile.set_category("assembly", 0);
            tile
        };
        let grouped = group_tiles(&[make("a", 10), make("b", 10), make("c", 5)], 20);
        assert_eq!(grouped.len(), 2);
        assert_eq!(grouped[0].label, "a\u{2013}b");
        assert_eq!(grouped[0].bytes, 20);
        assert_eq!(
            grouped[1].label, "c",
            "a single-tile tail keeps its own label"
        );
        let keys: Vec<String> = grouped[0].categories.keys().cloned().collect();
        assert_eq!(
            keys,
            vec!["exact_c"],
            "a zero contribution never creates the key"
        );
    }
}
