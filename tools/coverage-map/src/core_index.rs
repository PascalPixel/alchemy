//! Exhaustive, non-overlapping contributor target index.
//!
//! The coverage map already partitions every audited executable byte into
//! source-owner tiles.  This module turns the unfinished tiles into one stable
//! ranking instead of maintaining another hand-counted target ledger.  A row
//! is deliberately no larger than its witnessed owner scope (or one contiguous
//! unresolved run): broader campaign cuts are useful planning views, but they
//! overlap and therefore cannot be the primary exhaustive index.

use crate::json::Value;
use crate::model::{Area, Tile};
use crate::pipeline::CoverageMap;
use std::collections::BTreeSet;

#[derive(Clone, Debug, PartialEq, Eq)]
struct Target {
    namespace: String,
    address: Option<i64>,
    label: String,
    scope_bytes: i64,
    target_bytes: i64,
    semantic_c_bytes: i64,
    assembly_bytes: i64,
    exact_c_bytes: i64,
    permanent_assembly_bytes: i64,
}

fn category(tile: &Tile, name: &str) -> i64 {
    tile.categories.get(&name.to_string()).copied().unwrap_or(0)
}

fn namespace(area: &Area, tile: &Tile) -> String {
    if area.id == "main" {
        return "main".to_string();
    }
    tile.group
        .as_deref()
        .map(|group| format!("resource_{group}"))
        .unwrap_or_else(|| area.id.clone())
}

fn target(area: &Area, tile: &Tile) -> Option<Target> {
    let semantic_c_bytes = category(tile, "semantic_c");
    let assembly_bytes = category(tile, "assembly");
    let target_bytes = semantic_c_bytes + assembly_bytes;
    if target_bytes == 0 {
        return None;
    }
    Some(Target {
        namespace: namespace(area, tile),
        address: tile.address,
        label: tile.label.clone(),
        scope_bytes: tile.bytes,
        target_bytes,
        semantic_c_bytes,
        assembly_bytes,
        exact_c_bytes: category(tile, "exact_c"),
        permanent_assembly_bytes: category(tile, "retained_asm"),
    })
}

fn number(document: &Value, path: &[&str]) -> Result<i64, String> {
    let mut node = document;
    for key in path {
        node = node
            .get(key)
            .ok_or_else(|| format!("coverage map has no {}", path.join(".")))?;
    }
    node.as_f64()
        .map(|value| value as i64)
        .ok_or_else(|| format!("coverage map has non-numeric {}", path.join(".")))
}

fn address(value: Option<i64>) -> Value {
    value
        .map(|address| Value::Str(format!("0x{address:08x}")))
        .unwrap_or(Value::Null)
}

fn target_value(rank: usize, row: &Target) -> Value {
    Value::Obj(vec![
        ("rank".into(), Value::Num(rank as f64)),
        ("namespace".into(), Value::Str(row.namespace.clone())),
        ("address".into(), address(row.address)),
        ("label".into(), Value::Str(row.label.clone())),
        ("scope_bytes".into(), Value::Num(row.scope_bytes as f64)),
        ("target_bytes".into(), Value::Num(row.target_bytes as f64)),
        (
            "semantic_c_bytes".into(),
            Value::Num(row.semantic_c_bytes as f64),
        ),
        (
            "assembly_bytes".into(),
            Value::Num(row.assembly_bytes as f64),
        ),
        (
            "exact_c_leverage_bytes".into(),
            Value::Num(row.exact_c_bytes as f64),
        ),
        (
            "permanent_assembly_bytes".into(),
            Value::Num(row.permanent_assembly_bytes as f64),
        ),
    ])
}

fn value_number(value: &Value, key: &str) -> i64 {
    value.get(key).and_then(Value::as_f64).unwrap_or(0.0) as i64
}

/// Human-sized front page for the complete machine-readable index.
pub fn markdown(document: &Value) -> Result<String, String> {
    let accounting = document
        .get("accounting")
        .ok_or_else(|| "core index has no accounting".to_string())?;
    let targets = document
        .get("targets")
        .and_then(Value::as_array)
        .ok_or_else(|| "core index has no targets".to_string())?;
    let target_count = value_number(accounting, "target_count");
    let audited_spaces = value_number(accounting, "audited_address_spaces");
    let target_spaces = value_number(accounting, "address_spaces_with_targets");
    let target_bytes = value_number(accounting, "target_bytes");
    let resolved_only = value_number(accounting, "resolved_only_bytes");
    let executable = value_number(accounting, "executable_bytes");
    let visible: Vec<&Value> = targets
        .iter()
        .filter(|row| value_number(row, "scope_bytes") >= 1_000)
        .collect();

    let mut text = format!(
        "This section is generated. It is the primary contributor target list:\n\
non-overlapping audited source-owner scopes (or contiguous unresolved\n\
executable runs), sorted largest to smallest. Broader multi-owner campaign cuts\n\
belong in [Status](#status); they may overlap and therefore are not used for\n\
byte accounting. Regenerate with `make coverage` -- do not edit by hand.\n\n\
- **Unfinished scopes:** {}\n\
- **Address spaces scanned:** {} ({} still contain targets)\n\
- **Target bytes:** {} semantic-C or unresolved-assembly bytes\n\
- **Resolved-only bytes:** {} Exact C or audited permanent assembly bytes\n\
- **Executable bytes accounted for:** {}\n\n\
### Main target list\n\n\
This table contains every scope of at least 1,000 bytes ({} rows). The complete\n\
{}-row index, including the smallest audited owners, is\n\
[`metrics/gs1-en-core-targets.json`](metrics/gs1-en-core-targets.json).\n\n\
| Rank | Scope | Target | Namespace / owner |\n\
|---:|---:|---:|---|\n",
        crate::js::commas(target_count),
        crate::js::commas(audited_spaces),
        crate::js::commas(target_spaces),
        crate::js::commas(target_bytes),
        crate::js::commas(resolved_only),
        crate::js::commas(executable),
        visible.len(),
        crate::js::commas(target_count),
    );
    for row in visible {
        let rank = value_number(row, "rank");
        let scope = value_number(row, "scope_bytes");
        let target = value_number(row, "target_bytes");
        let namespace = row
            .get("namespace")
            .and_then(Value::as_str)
            .unwrap_or("unknown");
        let address = row
            .get("address")
            .and_then(Value::as_str)
            .unwrap_or("unknown");
        let owner = format!("`{namespace}:{address}`");
        text.push_str(&format!(
            "| {rank} | {} | {} | {owner} |\n",
            crate::js::commas(scope),
            crate::js::commas(target),
        ));
    }
    Ok(text)
}

/// Build the tracked exhaustive target index from the same byte partition the
/// dashboard renders.
pub fn build(map: &CoverageMap, target_name: &str) -> Result<Value, String> {
    let namespaces: BTreeSet<String> = map
        .executable_areas
        .iter()
        .flat_map(|area| area.tiles.iter().map(|tile| namespace(area, tile)))
        .collect();
    let mut targets: Vec<Target> = map
        .executable_areas
        .iter()
        .flat_map(|area| area.tiles.iter().filter_map(|tile| target(area, tile)))
        .collect();
    let target_namespaces: BTreeSet<String> =
        targets.iter().map(|row| row.namespace.clone()).collect();
    targets.sort_by(|left, right| {
        right
            .scope_bytes
            .cmp(&left.scope_bytes)
            .then_with(|| right.target_bytes.cmp(&left.target_bytes))
            .then_with(|| left.namespace.cmp(&right.namespace))
            .then_with(|| left.address.cmp(&right.address))
            .then_with(|| left.label.cmp(&right.label))
    });

    let executable_bytes: i64 = map.executable_areas.iter().map(|area| area.bytes).sum();
    let map_executable = number(&map.document, &["executable_bytes"])?;
    if executable_bytes != map_executable {
        return Err(format!(
            "core index sees {executable_bytes} of {map_executable} executable bytes"
        ));
    }

    let target_scope_bytes: i64 = targets.iter().map(|row| row.scope_bytes).sum();
    let target_bytes: i64 = targets.iter().map(|row| row.target_bytes).sum();
    let semantic_c_bytes: i64 = targets.iter().map(|row| row.semantic_c_bytes).sum();
    let assembly_bytes: i64 = targets.iter().map(|row| row.assembly_bytes).sum();
    let exact_c_leverage_bytes: i64 = targets.iter().map(|row| row.exact_c_bytes).sum();
    let permanent_assembly_leverage_bytes: i64 =
        targets.iter().map(|row| row.permanent_assembly_bytes).sum();
    let map_semantic = number(&map.document, &["categories", "semantic_c", "bytes"])?;
    let map_assembly = number(&map.document, &["categories", "assembly", "bytes"])?;
    if semantic_c_bytes != map_semantic || assembly_bytes != map_assembly {
        return Err(format!(
            "core targets account for semantic/assembly {semantic_c_bytes}/{assembly_bytes}, expected {map_semantic}/{map_assembly}"
        ));
    }

    let exact_c_bytes = number(&map.document, &["categories", "exact_c", "bytes"])?;
    let permanent_assembly_bytes = number(&map.document, &["categories", "retained_asm", "bytes"])?;
    let resolved_only_bytes = executable_bytes - target_scope_bytes;
    let expected_resolved = (exact_c_bytes - exact_c_leverage_bytes)
        + (permanent_assembly_bytes - permanent_assembly_leverage_bytes);
    if resolved_only_bytes != expected_resolved {
        return Err(format!(
            "core index resolved remainder is {resolved_only_bytes}, expected {expected_resolved}"
        ));
    }

    Ok(Value::Obj(vec![
        ("format".into(), Value::Num(1.0)),
        (
            "kind".into(),
            Value::Str("golden-sun-core-target-index".into()),
        ),
        ("target".into(), Value::Str(target_name.to_string())),
        (
            "derivation".into(),
            Value::Str("coverage-owner-scopes-v1".into()),
        ),
        (
            "policy".into(),
            Value::Obj(vec![
                (
                    "unit".into(),
                    Value::Str(
                        "audited source-owner scope or contiguous unresolved executable run"
                            .into(),
                    ),
                ),
                (
                    "ordering".into(),
                    Value::Str(
                        "scope_bytes descending, then target_bytes descending, then namespace and address"
                            .into(),
                    ),
                ),
                (
                    "target_categories".into(),
                    Value::Arr(vec![
                        Value::Str("semantic_c".into()),
                        Value::Str("assembly".into()),
                    ]),
                ),
                (
                    "overlap".into(),
                    Value::Str("none; broader campaign cuts are intentionally excluded".into()),
                ),
            ]),
        ),
        (
            "accounting".into(),
            Value::Obj(vec![
                (
                    "executable_bytes".into(),
                    Value::Num(executable_bytes as f64),
                ),
                (
                    "audited_address_spaces".into(),
                    Value::Num(namespaces.len() as f64),
                ),
                (
                    "address_spaces_with_targets".into(),
                    Value::Num(target_namespaces.len() as f64),
                ),
                ("target_count".into(), Value::Num(targets.len() as f64)),
                (
                    "target_scope_bytes".into(),
                    Value::Num(target_scope_bytes as f64),
                ),
                ("target_bytes".into(), Value::Num(target_bytes as f64)),
                (
                    "semantic_c_bytes".into(),
                    Value::Num(semantic_c_bytes as f64),
                ),
                (
                    "assembly_bytes".into(),
                    Value::Num(assembly_bytes as f64),
                ),
                (
                    "exact_c_leverage_bytes".into(),
                    Value::Num(exact_c_leverage_bytes as f64),
                ),
                (
                    "permanent_assembly_leverage_bytes".into(),
                    Value::Num(permanent_assembly_leverage_bytes as f64),
                ),
                (
                    "resolved_only_bytes".into(),
                    Value::Num(resolved_only_bytes as f64),
                ),
            ]),
        ),
        (
            "targets".into(),
            Value::Arr(
                targets
                    .iter()
                    .enumerate()
                    .map(|(index, row)| target_value(index + 1, row))
                    .collect(),
            ),
        ),
    ]))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::model::{area, Tile};
    use crate::ordered::OrderedMap;

    fn tile(label: &str, bytes: i64, categories: &[(&str, i64)]) -> Tile {
        let mut values = OrderedMap::new();
        for (name, value) in categories {
            values.insert((*name).to_string(), *value);
        }
        Tile {
            label: label.into(),
            bytes,
            categories: values,
            ..Tile::default()
        }
    }

    #[test]
    fn unfinished_scopes_are_sorted_and_every_category_is_conserved() {
        let map = CoverageMap {
            document: Value::Obj(vec![
                ("executable_bytes".into(), Value::Num(20.0)),
                (
                    "categories".into(),
                    Value::Obj(vec![
                        (
                            "exact_c".into(),
                            Value::Obj(vec![("bytes".into(), Value::Num(6.0))]),
                        ),
                        (
                            "semantic_c".into(),
                            Value::Obj(vec![("bytes".into(), Value::Num(10.0))]),
                        ),
                        (
                            "assembly".into(),
                            Value::Obj(vec![("bytes".into(), Value::Num(2.0))]),
                        ),
                        (
                            "retained_asm".into(),
                            Value::Obj(vec![("bytes".into(), Value::Num(2.0))]),
                        ),
                    ]),
                ),
            ]),
            rom_areas: Vec::new(),
            executable_areas: vec![area(
                "main",
                "Main",
                vec![
                    tile("small", 4, &[("semantic_c", 4)]),
                    tile("done", 4, &[("exact_c", 4)]),
                    tile("large", 8, &[("semantic_c", 6), ("assembly", 2)]),
                    tile("mixed", 2, &[("exact_c", 2)]),
                    tile("permanent", 2, &[("retained_asm", 2)]),
                ],
            )],
        };
        let built = build(&map, "fixture").expect("index");
        let rows = built
            .get("targets")
            .and_then(Value::as_array)
            .expect("targets");
        assert_eq!(rows.len(), 2);
        assert_eq!(rows[0].get("label").and_then(Value::as_str), Some("large"));
        assert_eq!(
            built
                .get("accounting")
                .and_then(|value| value.get("target_bytes"))
                .and_then(Value::as_f64),
            Some(12.0)
        );
    }
}
