//! Scaffold a declared translation unit over a contiguous main-image range.
//!
//!   unit-scaffold <game> <unit-id> <start-hex> <end-hex> [--apply]
//!
//! Reads the owner inventory for the range and emits
//! games/<game>/recon/en/units/<unit-id>.c: an address-ordered include
//! composite of the range's production sources and candidate drafts. Owners
//! with no C anywhere are holes: retained assembly, listed in the manifest
//! only. Owners whose name is registered while production stays assembly fit
//! neither manifest state and are skipped entirely. With --apply the manifest
//! entry is appended to games/<game>/recon/translation-units.json; without
//! it the entry is printed. The composite is a starting point: resolving
//! declaration collisions between the included files is the recovering
//! agent's work, scored with diff --unit until every previously
//! exact owner is exact again.

use serde_json::{json, Value};
use std::collections::BTreeSet;
use std::fs;
use std::path::Path;
use std::process::ExitCode;

struct Row {
    address: u32,
    include: Option<String>,
    state: &'static str,
    extent: u64,
}

pub fn entry(args: &[String]) -> ExitCode {
    match scaffold(&compiler_core::routing::root(), args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

/// Every input and output is resolved against `root`, so the operation does
/// not depend on the process working directory.
fn scaffold(root: &Path, args: &[String]) -> Result<(), String> {
    const USAGE: &str =
        "usage: alchemy unit scaffold <game> <unit-id> <start-hex> <end-hex> [--apply]";
    if args == ["--help"] || args == ["-h"] {
        println!("{USAGE}");
        return Ok(());
    }
    let apply = args.iter().any(|a| a == "--apply");
    let positional: Vec<&String> = args.iter().filter(|a| *a != "--apply").collect();
    let [game, unit_id, start_hex, end_hex] = positional[..] else {
        return Err(USAGE.into());
    };
    let start = u32::from_str_radix(start_hex.trim_start_matches("0x"), 16)
        .map_err(|_| "start is not hex")?;
    let end =
        u32::from_str_radix(end_hex.trim_start_matches("0x"), 16).map_err(|_| "end is not hex")?;
    let inventory: Value = compiler_core::build_io::read_json(
        &root.join(format!("out/{game}-en/full/rebuilt.owner-inventory.json")),
    )?;
    let manifest_relative = format!("games/{game}/recon/translation-units.json");
    let manifest_path = root.join(&manifest_relative);
    let manifest_text = fs::read_to_string(&manifest_path)
        .map_err(|e| format!("{}: {e}", manifest_path.display()))?;
    let manifest: Value = serde_json::from_str(&manifest_text)
        .map_err(|e| format!("{}: {e}", manifest_path.display()))?;
    // Owners already claimed by a declared unit are excluded: overlapping
    // declarations can never coexist, and the fix is extending that unit.
    let mut declared: BTreeSet<u32> = BTreeSet::new();
    for unit in manifest["units"].as_array().ok_or("manifest lacks units")? {
        for owner in unit["owners"].as_array().unwrap_or(&Vec::new()) {
            if let Some(address) = hex_of(&owner["address"]) {
                declared.insert(address);
            }
        }
    }
    let mut rows: Vec<Row> = Vec::new();
    let mut skipped: Vec<String> = Vec::new();
    for owner in inventory["owners"]
        .as_array()
        .ok_or("inventory lacks owners")?
    {
        if owner["container"]["kind"] != "main-rom" {
            continue;
        }
        let Some(address) = hex_of(&owner["address"]) else {
            continue;
        };
        if address < start || address >= end || declared.contains(&address) {
            continue;
        }
        let hex = format!("{address:08x}");
        let production = &owner["production"];
        let registered = !owner["registration"]["source_path"].is_null();
        let exact = production["state"] == "exact-c";
        if registered && !exact {
            // Named while still assembly: fits neither manifest state.
            skipped.push(hex);
            continue;
        }
        let source = production["source"].as_str().unwrap_or("");
        let include = if exact && source.ends_with(".c") {
            Some(format!(
                "../../../{}",
                source.replace(&format!("games/{game}/"), "")
            ))
        } else if root
            .join(format!("games/{game}/recon/en/main/{hex}.c"))
            .exists()
        {
            Some(format!("../main/{hex}.c"))
        } else {
            None
        };
        rows.push(Row {
            address,
            include,
            state: if exact {
                "exact-c"
            } else {
                "retained-assembly"
            },
            extent: production["extent_bytes"].as_u64().unwrap_or(0),
        });
    }
    rows.sort_by_key(|r| r.address);
    if rows.is_empty() {
        return Err("no owners in range".into());
    }
    // Every refusal comes before the first write, so neither the composite nor
    // the printed entry can preview a unit that cannot be declared.
    if let Some(refusal) = hole_refusal(unit_id, &rows) {
        return Err(refusal);
    }
    if manifest["units"]
        .as_array()
        .ok_or("manifest lacks units")?
        .iter()
        .any(|declared| declared["id"] == *unit_id.as_str())
    {
        return Err(format!("unit {unit_id} already declared"));
    }
    let mut lines = vec![
        format!("/* Translation unit {unit_id}: 0x{start_hex} .. 0x{end_hex}."),
        " * Address-ordered composite; holes stay retained assembly and are listed".into(),
        " * in the manifest. Generated by alchemy unit scaffold, then maintained by".into(),
        " * hand: resolve declaration collisions here or in shared headers. */".into(),
        String::new(),
    ];
    for r in &rows {
        match &r.include {
            Some(include) => lines.push(format!("#include \"{include}\"")),
            None => lines.push(format!(
                "/* hole: 0x{:08x} stays retained assembly (no C anywhere yet) */",
                r.address
            )),
        }
    }
    // The manifest records the composite by its repository-relative path.
    let out = format!("games/{game}/recon/en/units/{unit_id}.c");
    let entry = json!({
        "id": unit_id,
        "game": game,
        "source": out,
        "compiler_route": "canonical-gcc296",
        "overlay": null,
        "absolute_symbols": {},
        "local_symbols": [],
        // Every row is declared: a range holding a hole was already refused,
        // because the manifest has no state for one.
        "owners": rows.iter().map(|r| json!({
            "address": format!("0x{:08x}", r.address),
            "extent": r.extent,
            "state": r.state,
        })).collect::<Vec<_>>(),
    });
    // The declaration is prepared, checked and only then written, so a
    // manifest this scaffold cannot append to leaves no composite behind.
    // Everything is prepared and checked before the first write, so a range
    // this scaffold cannot declare leaves nothing on disk at all.
    let declaration = match apply {
        false => None,
        true => Some(append_unit(&manifest_text, &entry)?),
    };
    let summary = format!(
        "{} owners ({} exact, {} candidates, {} holes{})",
        rows.len(),
        rows.iter().filter(|r| r.state == "exact-c").count(),
        rows.iter()
            .filter(|r| r.include.is_some() && r.state != "exact-c")
            .count(),
        rows.iter().filter(|r| r.include.is_none()).count(),
        if skipped.is_empty() {
            String::new()
        } else {
            format!("; skipped named-assembly: {}", skipped.join(" "))
        }
    );
    let Some(updated) = declaration else {
        // Without --apply the composition is reported, never written: a
        // composite no manifest entry refers to is litter.
        println!("{unit_id}: {summary}");
        println!(
            "{}",
            serde_json::to_string_pretty(&entry).map_err(|e| e.to_string())?
        );
        return Ok(());
    };
    let composite = root.join(&out);
    if let Some(parent) = composite.parent() {
        fs::create_dir_all(parent).map_err(|e| format!("{}: {e}", parent.display()))?;
    }
    fs::write(&composite, format!("{}\n", lines.join("\n")))
        .map_err(|e| format!("{}: {e}", composite.display()))?;
    if let Err(error) = fs::write(&manifest_path, updated) {
        // The composite exists only for the entry that names it.
        let _ = fs::remove_file(&composite);
        return Err(format!("{}: {error}", manifest_path.display()));
    }
    println!("wrote {out}: {summary}");
    println!("declared {unit_id} in {manifest_relative}");
    Ok(())
}

/// An owner with no C anywhere cannot be declared: the manifest's only
/// retained state requires the member's draft to be included from `../main/`,
/// and dropping the hole instead leaves a unit that is wholly exact and
/// ungrouped, which the strict translation-unit check refuses in turn.
fn hole_refusal(unit_id: &str, rows: &[Row]) -> Option<String> {
    let holes: Vec<String> = rows
        .iter()
        .filter(|row| row.include.is_none())
        .map(|row| format!("{:08x}", row.address))
        .collect();
    (!holes.is_empty()).then(|| {
        format!(
            "{unit_id} has no C for {}: a declared retained member's body must be included from ../main/<address>.c, so draft the hole with alchemy decompile before declaring the unit",
            holes.join(" ")
        )
    })
}

/// The byte offset just past the last entry of the `units` array, with the
/// indentation that entry is written at. Found by walking the array's own
/// brackets, so no indentation or line shape is assumed of the file.
fn last_unit(text: &str) -> Result<(usize, String), String> {
    const MISSING: &str = "translation-units.json: no units array to append to";
    let key = text.find("\"units\"").ok_or(MISSING)?;
    let open = key + text[key..].find('[').ok_or(MISSING)?;
    let (mut depth, mut in_string, mut escaped) = (0i32, false, false);
    let (mut first, mut last) = (None, None);
    for (index, byte) in text.bytes().enumerate().skip(open) {
        if in_string {
            match (escaped, byte) {
                (true, _) => escaped = false,
                (_, b'\\') => escaped = true,
                (_, b'"') => in_string = false,
                _ => {}
            }
            continue;
        }
        match byte {
            b'"' => in_string = true,
            b'[' | b'{' => {
                depth += 1;
                if depth == 2 && byte == b'{' {
                    first = Some(index);
                }
            }
            b']' | b'}' => {
                depth -= 1;
                if depth == 1 && byte == b'}' {
                    last = Some(index + 1);
                }
                if depth == 0 {
                    let (first, last) = first
                        .zip(last)
                        .ok_or("translation-units.json: the units array is empty")?;
                    let indent = text[..first].rsplit('\n').next().unwrap_or_default();
                    return Ok((last, indent.to_string()));
                }
            }
            _ => {}
        }
    }
    Err("translation-units.json: unterminated units array".into())
}

/// Append one unit in place. The manifest still mixes compact and expanded
/// entries from its history, so a whole-document rewrite would re-canonicalise
/// every entry that is still compact and bury the new one in the diff.
fn append_unit(text: &str, entry: &Value) -> Result<String, String> {
    let (last, indent) = last_unit(text)?;
    let rendered = serde_json::to_string_pretty(entry).map_err(|e| e.to_string())?;
    let updated = format!(
        "{},\n{indent}{}{}",
        &text[..last],
        rendered.replace('\n', &format!("\n{indent}")),
        &text[last..]
    );
    // A wrong insertion point can still leave parseable JSON, so the result is
    // read back and compared with the document it has to be: the original with
    // this one entry appended and nothing else moved.
    let mut expected: Value =
        serde_json::from_str(text).map_err(|e| format!("translation-units.json: {e}"))?;
    expected["units"]
        .as_array_mut()
        .ok_or("translation-units.json: units must be an array")?
        .push(entry.clone());
    let parsed: Value =
        serde_json::from_str(&updated).map_err(|e| format!("translation-units.json: {e}"))?;
    if parsed != expected {
        return Err("translation-units.json: the splice changed the entries around it".into());
    }
    Ok(updated)
}

#[cfg(test)]
mod tests {
    use super::*;

    const MANIFEST: &str = concat!(
        "{\n",
        "  \"format\": \"translation-units\",\n",
        "  \"units\": [\n",
        "    {\"id\":\"compact\",\"game\":\"gs1\",\"owners\":[]},\n",
        "    {\n      \"id\": \"expanded\",\n      \"game\": \"gs1\",\n      \"owners\": []\n    }\n",
        "  ]\n",
        "}\n"
    );

    /// A scaffolding tree: one exact owner in production, plus a second owner
    /// that is either drafted or a true hole.
    fn tree(drafted: bool) -> tempfile::TempDir {
        let tree = tempfile::tempdir().unwrap();
        let root = tree.path();
        let write = |relative: &str, text: &str| {
            let path = root.join(relative);
            std::fs::create_dir_all(path.parent().expect("fixture parent")).unwrap();
            std::fs::write(path, text).unwrap();
        };
        let owner = |address: &str, exact: bool| {
            json!({
                "container": {"kind": "main-rom"},
                "address": address,
                "registration": {"source_path": if exact { json!("runtime/exact.c") } else { Value::Null }},
                "production": {
                    "state": if exact { "exact-c" } else { "retained-assembly" },
                    "source": if exact { json!("games/gs1/src/runtime/exact.c") } else { Value::Null },
                    "extent_bytes": 64,
                },
            })
        };
        // The third owner sits past the scaffolded range, so a second unit can
        // be asked for without reusing an owner the first one claimed.
        write(
            "out/gs1-en/full/rebuilt.owner-inventory.json",
            &json!({"owners": [
                owner("0x08094400", true),
                owner("0x08094500", false),
                owner("0x08094600", true),
            ]})
            .to_string(),
        );
        write("games/gs1/recon/translation-units.json", MANIFEST);
        write("games/gs1/src/runtime/exact.c", "void Exact(void) {}\n");
        if drafted {
            write(
                "games/gs1/recon/en/main/08094500.c",
                "void Draft(void) {}\n",
            );
        }
        tree
    }

    fn argv(args: &[&str]) -> Vec<String> {
        args.iter().copied().map(str::to_owned).collect()
    }

    /// A range holding an owner with no C is refused in both modes, and the
    /// refusal comes before the composite or the manifest entry is written.
    #[test]
    fn a_hole_stops_the_scaffold_before_it_writes() {
        let tree = tree(false);
        let root = tree.path();
        let manifest = root.join("games/gs1/recon/translation-units.json");
        for extra in [vec![], vec!["--apply"]] {
            let mut args = argv(&["gs1", "unit-probe", "0x08094400", "0x08094600"]);
            args.extend(extra.into_iter().map(str::to_owned));
            let refusal = scaffold(root, &args).unwrap_err();
            assert!(refusal.contains("has no C for 08094500"), "{refusal}");
            assert_eq!(std::fs::read_to_string(&manifest).unwrap(), MANIFEST);
            assert!(!root.join("games/gs1/recon/en/units/unit-probe.c").exists());
        }
    }

    /// Declaring a unit adds its entry and leaves every other manifest byte
    /// alone, including the entry still held in the compact form.
    #[test]
    fn declaring_a_unit_rewrites_nothing_around_it() {
        let tree = tree(true);
        let root = tree.path();
        let manifest = root.join("games/gs1/recon/translation-units.json");
        let args = argv(&["gs1", "unit-probe", "0x08094400", "0x08094600", "--apply"]);
        scaffold(root, &args).unwrap();
        let updated = std::fs::read_to_string(&manifest).unwrap();
        let close = MANIFEST.rfind("\n  ]").unwrap();
        assert_eq!(updated[..close], MANIFEST[..close]);
        assert!(updated.ends_with(&MANIFEST[close..]));
        assert!(updated.contains("{\"id\":\"compact\",\"game\":\"gs1\",\"owners\":[]},"));
        // The composite is the unit's actual content, so it is read too.
        let composite = root.join("games/gs1/recon/en/units/unit-probe.c");
        let text = std::fs::read_to_string(&composite).unwrap();
        let includes: Vec<&str> = text
            .lines()
            .filter(|line| line.starts_with("#include"))
            .collect();
        assert_eq!(
            includes,
            [
                "#include \"../../../src/runtime/exact.c\"",
                "#include \"../main/08094500.c\""
            ]
        );
        let units = serde_json::from_str::<Value>(&updated).unwrap()["units"].clone();
        let ids: Vec<&str> = units
            .as_array()
            .unwrap()
            .iter()
            .filter_map(|unit| unit["id"].as_str())
            .collect();
        assert_eq!(ids, ["compact", "expanded", "unit-probe"]);
        // The declared unit keeps both owners, the exact one and the drafted,
        // in address order and with the states the composite reflects.
        assert_eq!(
            units[2]["owners"],
            json!([
                {"address": "0x08094400", "extent": 64, "state": "exact-c"},
                {"address": "0x08094500", "extent": 64, "state": "retained-assembly"},
            ])
        );
        assert_eq!(units[2]["source"], "games/gs1/recon/en/units/unit-probe.c");
        // A whole-document rewrite would have expanded the compact entry.
        assert_ne!(
            updated,
            format!(
                "{}\n",
                serde_json::to_string_pretty(&serde_json::from_str::<Value>(&updated).unwrap())
                    .unwrap()
            )
        );

        // Reusing the id for another range is refused in both modes, before
        // the composite or the manifest entry is written.
        let composite = root.join("games/gs1/recon/en/units/unit-probe.c");
        std::fs::remove_file(&composite).unwrap();
        for extra in [vec![], vec!["--apply"]] {
            let mut again = argv(&["gs1", "unit-probe", "0x08094600", "0x08094700"]);
            again.extend(extra.into_iter().map(str::to_owned));
            assert_eq!(
                scaffold(root, &again).unwrap_err(),
                "unit unit-probe already declared"
            );
            assert_eq!(std::fs::read_to_string(&manifest).unwrap(), updated);
            assert!(!composite.exists());
        }
    }

    /// A manifest the scaffold cannot append to leaves no composite behind:
    /// the declaration is prepared and checked before the first write.
    #[test]
    fn a_manifest_that_cannot_be_appended_to_leaves_no_composite() {
        let tree = tree(true);
        let root = tree.path();
        let manifest = root.join("games/gs1/recon/translation-units.json");
        let empty = "{\n  \"units\": [\n  ]\n}\n";
        std::fs::write(&manifest, empty).unwrap();
        let args = argv(&["gs1", "unit-probe", "0x08094400", "0x08094600", "--apply"]);
        assert_eq!(
            scaffold(root, &args).unwrap_err(),
            "translation-units.json: the units array is empty"
        );
        assert!(!root.join("games/gs1/recon/en/units/unit-probe.c").exists());
        assert_eq!(std::fs::read_to_string(&manifest).unwrap(), empty);
    }

    /// Without --apply nothing reaches disk: a composite no manifest entry
    /// refers to is litter, and the dry run used to leave one behind.
    #[test]
    fn a_dry_run_writes_nothing() {
        let tree = tree(true);
        let root = tree.path();
        let manifest = root.join("games/gs1/recon/translation-units.json");
        let args = argv(&["gs1", "unit-probe", "0x08094400", "0x08094600"]);
        scaffold(root, &args).unwrap();
        assert_eq!(std::fs::read_to_string(&manifest).unwrap(), MANIFEST);
        assert!(!root.join("games/gs1/recon/en/units").exists());
    }

    /// The insertion point is the last entry's own closing brace, found by
    /// walking the array's brackets: a nested object inside that entry, or an
    /// inline array, must not be mistaken for the end of the units array.
    #[test]
    fn the_splice_follows_the_last_entry_not_the_indentation() {
        let nested = concat!(
            "{\n",
            "  \"units\": [\n",
            "    {\"id\":\"compact\",\"owners\":[]},\n",
            "    {\n",
            "      \"id\": \"expanded\",\n",
            "      \"absolute_symbols\": {\n",
            "        \"Func_1\": {\"address\": \"0x1\", \"kind\": \"thumb\"}\n",
            "      },\n",
            "      \"owners\": [{\"address\": \"0x2\", \"note\": \"] not an end\"}]\n",
            "    }\n",
            "  ]\n",
            "}\n"
        );
        let entry = json!({"id": "added", "owners": []});
        let updated = append_unit(nested, &entry).unwrap();
        let (last, indent) = last_unit(nested).unwrap();
        assert_eq!(indent, "    ");
        assert_eq!(&nested[last - 1..last], "}");
        assert_eq!(updated[..last], nested[..last]);
        assert!(updated.ends_with(&nested[last..]));
        let units = serde_json::from_str::<Value>(&updated).unwrap()["units"].clone();
        let ids: Vec<&str> = units
            .as_array()
            .unwrap()
            .iter()
            .filter_map(|unit| unit["id"].as_str())
            .collect();
        assert_eq!(ids, ["compact", "expanded", "added"]);
        assert_eq!(units[1]["absolute_symbols"]["Func_1"]["kind"], "thumb");
        assert_eq!(units[1]["owners"][0]["note"], "] not an end");
    }

    /// The splice separates the new entry from the one before it, so an empty
    /// array says so rather than emitting a leading comma.
    #[test]
    fn an_empty_units_array_is_refused_rather_than_spliced() {
        let empty = "{\n  \"units\": [\n  ]\n}\n";
        assert_eq!(
            append_unit(empty, &json!({"id": "first"})).unwrap_err(),
            "translation-units.json: the units array is empty"
        );
    }

    /// A range with an owner that has no C anywhere cannot be declared:
    /// dropping the hole instead leaves a wholly exact ungrouped unit, and
    /// `diff --unit --first` then fails on an unrelated message.
    #[test]
    fn a_range_with_no_c_for_an_owner_is_refused_before_it_is_declared() {
        let row = |address, include: Option<&str>, state| Row {
            address,
            include: include.map(str::to_string),
            state,
            extent: 32,
        };
        let drafted = [
            row(
                0x0809445c,
                Some("../../../src/runtime/disarm_hblank_dma.c"),
                "exact-c",
            ),
            row(0x080944ec, Some("../main/080944ec.c"), "retained-assembly"),
        ];
        assert!(hole_refusal("unit-0809445c", &drafted).is_none());
        let refusal = hole_refusal(
            "unit-0809445c",
            &[
                row(
                    0x0809445c,
                    Some("../../../src/runtime/disarm_hblank_dma.c"),
                    "exact-c",
                ),
                row(0x080944ec, None, "retained-assembly"),
            ],
        )
        .expect("a hole refuses");
        assert!(refusal.contains("has no C for 080944ec"), "{refusal}");
        assert!(refusal.contains("../main/<address>.c"), "{refusal}");
    }

    /// The maintained manifest still mixes compact and expanded entries, so a
    /// whole-document rewrite reformats entries this scaffold never touched.
    /// Appending in place has to leave every other byte alone.
    #[test]
    fn appending_a_unit_leaves_the_entries_around_it_untouched() {
        let path = compiler_core::routing::root().join("games/gs1/recon/translation-units.json");
        let text = std::fs::read_to_string(&path).unwrap();
        let entry = json!({"id": "unit-append-check", "game": "gs1", "owners": []});
        let updated = append_unit(&text, &entry).unwrap();
        let close = text.rfind("\n  ]").unwrap();
        assert_eq!(updated[..close], text[..close]);
        assert!(updated.ends_with(&text[close..]));
        let mut expected: Value = serde_json::from_str(&text).unwrap();
        expected["units"].as_array_mut().unwrap().push(entry);
        assert_eq!(
            serde_json::from_str::<Value>(&updated).unwrap(),
            expected,
            "the append changed the manifest beyond the new unit"
        );
        // A whole-document rewrite would not: the file is not canonical.
        let canonical = format!("{}\n", serde_json::to_string_pretty(&expected).unwrap());
        assert_ne!(canonical, updated);
    }
}

fn hex_of(value: &Value) -> Option<u32> {
    u32::from_str_radix(value.as_str()?.trim_start_matches("0x"), 16).ok()
}
