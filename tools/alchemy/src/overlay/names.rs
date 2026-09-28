//! Names for one overlay owner's calls, literal pool and unit symbols.
//!
//! An overlay reaches the main image through import veneers and its own
//! functions directly. A call is named, in order, by what the repository
//! already knows: a name another unit of the same overlay binds at that
//! runtime address, the name most units of any overlay bind to a veneer of the
//! same main function (the `Engine_*` services of `FIELD_EVENT.H`), the owner
//! register's name (object or plain source-path form), and finally
//! `Main_<main address>` or `Local_<listing address>`. A unit binds each
//! name at its runtime address, the listing address plus `0x8000`.
//!
//! Registered names, `Func_`, `Data_` and `Value_` spellings bind in the
//! link without a unit symbol, so a unit only declares the rest.

use crate::compiler::overlay::RUNTIME_BASE;
use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use crate::overlay::assembly::OVERLAY_BASE;
use serde_json::Value;
use std::collections::{BTreeMap, BTreeSet};

/// The distance from an overlay's listing address to its runtime address.
pub const RUNTIME_DISPLACEMENT: u32 = RUNTIME_BASE - OVERLAY_BASE as u32;

/// A name a unit may bind: its runtime address and whether it is data.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct Bound {
    pub address: u64,
    pub data: bool,
}

/// One absolute symbol some unit declares, in the image it links into.
#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Declared {
    pub image: String,
    pub name: String,
    pub bound: Bound,
}

/// Names that never vote for a veneer's service: they spell addresses.
fn address_spelling(name: &str) -> bool {
    ["Func_", "Main_", "Local_", "Dispatch_", "Data_", "Value_"]
        .iter()
        .any(|prefix| name.starts_with(prefix))
}

/// Every absolute symbol the translation-unit register declares, per image:
/// a unit's own and each instance's.
pub fn declared_symbols(units: &Value) -> Vec<Declared> {
    let mut found = Vec::new();
    let mut add = |image: &str, symbols: &Value| {
        for (name, symbol) in symbols.as_object().into_iter().flatten() {
            let Some(address) = symbol["address"]
                .as_str()
                .and_then(|text| u64::from_str_radix(text.trim_start_matches("0x"), 16).ok())
            else {
                continue;
            };
            found.push(Declared {
                image: image.to_string(),
                name: name.clone(),
                bound: Bound {
                    address,
                    data: symbol["kind"].as_str() == Some("data"),
                },
            });
        }
    };
    for unit in units["units"].as_array().into_iter().flatten() {
        if let Some(image) = unit["overlay"].as_str() {
            add(image, &unit["absolute_symbols"]);
        }
        for (image, instance) in unit["instances"].as_object().into_iter().flatten() {
            add(image, &instance["absolute_symbols"]);
        }
    }
    found
}

/// What the repository knows about names in one overlay.
#[derive(Debug, Default)]
pub struct Dictionary {
    /// Name to binding: this overlay's names, every data name, each of this
    /// overlay's veneers under its service and `Main_` names.
    symbols: BTreeMap<String, Bound>,
    /// This overlay's runtime addresses under the first name bound there,
    /// functions and data alike.
    local: BTreeMap<u64, String>,
    /// This overlay's veneers: runtime address to the main function.
    veneers: BTreeMap<u64, u32>,
    /// The service name most units give each main function.
    services: BTreeMap<u32, String>,
    /// The overlay image's length, bounding `Local_` spellings.
    length: usize,
}

impl Dictionary {
    /// `veneers` maps each image's import veneers (runtime address) to the
    /// main function they reach; `overlay` is the owner's image.
    pub fn new(
        overlay: &str,
        length: usize,
        declared: &[Declared],
        veneers: &BTreeMap<String, BTreeMap<u64, u32>>,
    ) -> Self {
        let mut dictionary = Dictionary {
            length,
            ..Dictionary::default()
        };
        let mut votes = BTreeMap::<u32, BTreeMap<&str, usize>>::new();
        for symbol in declared {
            let own = symbol.image == overlay;
            if symbol.bound.data {
                if !address_spelling(&symbol.name) {
                    dictionary
                        .symbols
                        .entry(symbol.name.clone())
                        .or_insert(symbol.bound);
                    if own {
                        dictionary
                            .local
                            .entry(symbol.bound.address)
                            .or_insert_with(|| symbol.name.clone());
                    }
                }
                continue;
            }
            if own && !address_spelling(&symbol.name) {
                dictionary
                    .symbols
                    .entry(symbol.name.clone())
                    .or_insert(symbol.bound);
                dictionary
                    .local
                    .entry(symbol.bound.address)
                    .or_insert_with(|| symbol.name.clone());
            }
            let main = veneers
                .get(&symbol.image)
                .and_then(|table| table.get(&symbol.bound.address));
            if let (Some(main), false) = (main, address_spelling(&symbol.name)) {
                *votes
                    .entry(*main)
                    .or_default()
                    .entry(&symbol.name)
                    .or_default() += 1;
            }
        }
        for (main, names) in votes {
            // Most votes, then the first name in order: deterministic.
            let best = names
                .iter()
                .max_by(|(left, a), (right, b)| a.cmp(b).then(right.cmp(left)))
                .map(|(name, _)| name.to_string());
            if let Some(best) = best {
                dictionary.services.insert(main, best);
            }
        }
        if let Some(own) = veneers.get(overlay) {
            dictionary.veneers = own.clone();
            for (runtime, main) in own {
                let bound = Bound {
                    address: *runtime,
                    data: false,
                };
                if let Some(service) = dictionary.services.get(main) {
                    dictionary.symbols.entry(service.clone()).or_insert(bound);
                }
                dictionary
                    .symbols
                    .entry(format!("Main_{main:08x}"))
                    .or_insert(bound);
            }
        }
        dictionary
    }

    /// The name of a call through this overlay's veneer at `runtime`.
    pub fn veneer_name(&self, runtime: u64, main: u32, registered: Option<&str>) -> String {
        if let Some(name) = self.local.get(&runtime) {
            return name.clone();
        }
        self.services
            .get(&main)
            .cloned()
            .or_else(|| registered.map(str::to_string))
            .unwrap_or_else(|| format!("Main_{main:08x}"))
    }

    /// The name of a direct call to this overlay's function at `listing`.
    pub fn local_name(&self, listing: u32, registered: Option<&str>) -> String {
        registered
            .map(str::to_string)
            .or_else(|| {
                self.local
                    .get(&u64::from(listing + RUNTIME_DISPLACEMENT))
                    .cloned()
            })
            .unwrap_or_else(|| format!("Local_{listing:08x}"))
    }

    /// The binding a unit needs for `name`, if the dictionary knows one.
    pub fn lookup(&self, name: &str) -> Option<Bound> {
        if let Some(hex) = name.strip_prefix("Local_") {
            let listing = u32::from_str_radix(hex, 16)
                .ok()
                .filter(|_| hex.len() == 8)?;
            let offset = listing.checked_sub(OVERLAY_BASE as u32)? as usize;
            return (offset < self.length).then_some(Bound {
                address: u64::from(listing + RUNTIME_DISPLACEMENT),
                data: false,
            });
        }
        self.symbols.get(name).copied()
    }

    /// A name for a literal pool word, when the dictionary knows the value.
    pub fn describe(&self, word: u32) -> Option<String> {
        let runtime = u64::from(word & !1);
        if (RUNTIME_BASE..RUNTIME_BASE + self.length as u32).contains(&word) {
            if let Some(name) = self.local.get(&runtime) {
                return Some(name.clone());
            }
            if let Some(main) = self.veneers.get(&runtime) {
                return Some(self.veneer_name(runtime, *main, None));
            }
            let listing = (word & !1) - RUNTIME_DISPLACEMENT;
            return Some(if word & 1 == 1 {
                format!("Local_{listing:08x} (overlay function)")
            } else {
                format!("overlay data at 0x{listing:08x}")
            });
        }
        if word >= 0x0200_0000 {
            let data = self
                .symbols
                .iter()
                .find(|(_, bound)| bound.data && bound.address == u64::from(word));
            if let Some((name, _)) = data {
                return Some(name.clone());
            }
        }
        (word < 0x1_0000).then(|| format!("constant; Value_{word:08x} if the pool must hold it"))
    }
}

/// Whether the link binds `name` itself: address spellings and every name
/// the owner register gives this overlay's owners or a main function.
pub fn link_binds(paths: &SourcePaths, overlay: &str, name: &str) -> bool {
    ["Func_", "Data_", "Value_", "_call_via_"]
        .iter()
        .any(|prefix| name.starts_with(prefix))
        || !paths.overlay_owners_named(overlay, name).is_empty()
        || paths.main_symbol(name).ok().flatten().is_some()
}

/// The owner register's name for a main function, from either record form.
pub fn registered_main(paths: &SourcePaths, main: u32) -> Option<String> {
    SourceOwner::parse(&format!("main:{main:08x}"))
        .ok()
        .and_then(|owner| paths.registered_name(owner).map(str::to_string))
}

/// Comments removed, so identifiers in them never bind.
fn code_only(source: &str) -> String {
    let block = regex::Regex::new(r"(?s)/\*.*?\*/").unwrap();
    let line = regex::Regex::new(r"//[^\n]*").unwrap();
    line.replace_all(&block.replace_all(source, " "), "")
        .into_owned()
}

/// The functions a source defines: `name(...)` followed by a body.
pub fn defined_functions(source: &str) -> Vec<String> {
    let definition = regex::Regex::new(
        r"(?m)^[A-Za-z_][\w \t*]*?\b([A-Za-z_]\w*)\s*\((?:[^;{()]|\([^;{()]*\))*\)\s*\{",
    )
    .unwrap();
    definition
        .captures_iter(&code_only(source))
        .map(|captures| captures[1].to_string())
        .collect()
}

/// The absolute symbols a unit compiling `sources` needs: every identifier
/// the sources use that `kept` (bindings an earlier unit declared) or the
/// dictionary binds, except what the sources define. A kept binding wins;
/// otherwise names the link binds itself are left to it.
pub fn bind<'a>(
    sources: impl IntoIterator<Item = &'a str>,
    kept: &BTreeMap<String, Bound>,
    dictionary: &Dictionary,
    link_binds: impl Fn(&str) -> bool,
) -> BTreeMap<String, Bound> {
    let identifier = regex::Regex::new(r"\b[A-Za-z_]\w*\b").unwrap();
    let mut bound = BTreeMap::new();
    for source in sources {
        let code = code_only(source);
        let defined = defined_functions(source)
            .into_iter()
            .collect::<BTreeSet<_>>();
        for name in identifier.find_iter(&code).map(|found| found.as_str()) {
            if bound.contains_key(name) || defined.contains(name) {
                continue;
            }
            if let Some(symbol) = kept.get(name) {
                bound.insert(name.to_string(), *symbol);
            } else if link_binds(name) {
                continue;
            } else if let Some(symbol) = dictionary.lookup(name) {
                bound.insert(name.to_string(), symbol);
            }
        }
    }
    bound
}

/// Unit JSON for bound symbols, in name order.
pub fn symbols_json(bound: &BTreeMap<String, Bound>) -> Value {
    Value::Object(
        bound
            .iter()
            .map(|(name, symbol)| {
                (
                    name.clone(),
                    serde_json::json!({
                        "address": format!("0x{:08x}", symbol.address),
                        "kind": if symbol.data { "data" } else { "thumb" },
                    }),
                )
            })
            .collect(),
    )
}

/// `psynergy decompile` output with each call renamed and the function
/// named: `calls` maps the pseudo symbol the decompiler spells for a call
/// site (`Func_<address>`) to its name. Declarations of one name merge,
/// keeping a non-void return type, and the `Function` alias goes: the build
/// binds the owner's registered name.
pub fn convert(decompiled: &str, calls: &BTreeMap<String, String>, name: &str) -> String {
    let symbol = regex::Regex::new(r"\bFunc_[0-9a-f]{8}\b").unwrap();
    let declaration = regex::Regex::new(r"^(\w[\w ]*?\**)\s*\b(\w+)\(\);$").unwrap();
    let function = regex::Regex::new(r"\bFunction\b").unwrap();
    let renamed = symbol.replace_all(decompiled, |found: &regex::Captures| {
        calls
            .get(&found[0])
            .cloned()
            .unwrap_or_else(|| found[0].to_string())
    });
    let mut types = BTreeMap::<String, String>::new();
    for line in renamed.lines() {
        if let Some(captures) = declaration.captures(line) {
            let entry = types.entry(captures[2].to_string()).or_default();
            if entry.is_empty() || entry == "void" {
                *entry = captures[1].trim().to_string();
            }
        }
    }
    let mut written = BTreeSet::new();
    let mut out = Vec::new();
    for line in renamed.lines() {
        if line.starts_with("#define Function ") {
            continue;
        }
        if let Some(captures) = declaration.captures(line) {
            let callee = captures[2].to_string();
            if written.insert(callee.clone()) {
                out.push(format!("{} {callee}();", types[&callee]));
            }
            continue;
        }
        out.push(function.replace_all(line, name).into_owned());
    }
    let mut text = out.join("\n");
    while text.contains("\n\n\n") {
        text = text.replace("\n\n\n", "\n\n");
    }
    text + "\n"
}

/// The owner's literal pool words, `(listing address, word)`, in address
/// order: the words its `ldr rN, [pc, #...]` instructions read.
pub fn pool_words(image: &[u8], entry: u32, span: u32) -> Vec<(u32, u32)> {
    let base = OVERLAY_BASE as u32;
    let mut words = BTreeMap::new();
    for ins in psynergy::decode::decode_window_at(image, base, entry, span) {
        if !matches!(ins.kind, psynergy::decode::Kind::LdrPool { .. }) {
            continue;
        }
        let at = (ins.addr - base) as usize;
        let Some(halfword) = image.get(at..at + 2) else {
            continue;
        };
        let immediate = u32::from(halfword[0]) * 4;
        let literal = ((ins.addr + 4) & !3) + immediate;
        let offset = (literal - base) as usize;
        if let Some(word) = image.get(offset..offset + 4) {
            words.insert(literal, u32::from_le_bytes(word.try_into().unwrap()));
        }
    }
    words.into_iter().collect()
}

/// A comment listing the literal pool with what each word names.
pub fn pool_comment(words: &[(u32, u32)], dictionary: &Dictionary) -> String {
    if words.is_empty() {
        return String::new();
    }
    let mut text = String::from("/* Literal pool, read from the ROM:\n");
    for (address, word) in words {
        let note = dictionary
            .describe(*word)
            .map(|note| format!("  {note}"))
            .unwrap_or_default();
        text.push_str(&format!(" *   0x{address:08x}: 0x{word:08x}{note}\n"));
    }
    text.push_str(" */\n");
    text
}

/// The last function a source defines that is not static: the owner.
pub fn exported_function(source: &str) -> Option<String> {
    defined_functions(source).into_iter().rfind(|candidate| {
        let pattern = format!(r"(?m)^static\b[^\n]*\b{candidate}\s*\(");
        !regex::Regex::new(&pattern).unwrap().is_match(source)
    })
}

/// The source with its last non-static function definition renamed,
/// every use included, and any `#define NAME Func_<entry>` alias of the
/// owner dropped: the build binds the registered name. `None` when it
/// defines no function.
pub fn rename_function(source: &str, entry: u32, name: &str) -> Option<String> {
    let alias = regex::Regex::new(&format!(r"(?m)^#define\s+\w+\s+Func_{entry:08x}\s*\n")).unwrap();
    let source = &alias.replace_all(source, "").into_owned();
    let old = exported_function(source)?;
    let word = regex::Regex::new(&format!(r"\b{}\b", regex::escape(&old))).unwrap();
    Some(word.replace_all(source, name).into_owned())
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;

    fn declared(image: &str, name: &str, address: u64, data: bool) -> Declared {
        Declared {
            image: image.into(),
            name: name.into(),
            bound: Bound { address, data },
        }
    }

    fn dictionary() -> Dictionary {
        // resource_3ca calls main 0x08000118 through 0x0200933c; two other
        // overlays name that main function Engine_TaskWait, one Engine_Other.
        let veneers = BTreeMap::from([
            (
                "resource_3ca".to_string(),
                BTreeMap::from([(0x0200_933c, 0x0800_0118), (0x0200_9344, 0x0800_0200)]),
            ),
            (
                "resource_3cb".to_string(),
                BTreeMap::from([(0x0200_9714, 0x0800_0118)]),
            ),
            (
                "resource_3cc".to_string(),
                BTreeMap::from([(0x0200_9000, 0x0800_0118)]),
            ),
            (
                "resource_3cd".to_string(),
                BTreeMap::from([(0x0200_9100, 0x0800_0118)]),
            ),
        ]);
        let symbols = [
            declared("resource_3cb", "Engine_TaskWait", 0x0200_9714, false),
            declared("resource_3cc", "Engine_TaskWait", 0x0200_9000, false),
            declared("resource_3cd", "Engine_Other", 0x0200_9100, false),
            declared("resource_3cd", "Main_08000118", 0x0200_9100, false),
            declared("resource_3cb", "gEventWork", 0x0300_1ebc, true),
            declared("resource_3ca", "Scene_Helper", 0x0200_8200, false),
        ];
        Dictionary::new("resource_3ca", 0x2000, &symbols, &veneers)
    }

    #[test]
    fn calls_take_same_overlay_then_majority_service_then_register_then_address() {
        let dictionary = dictionary();
        assert_eq!(
            dictionary.veneer_name(0x0200_933c, 0x0800_0118, Some("Registered")),
            "Engine_TaskWait"
        );
        assert_eq!(
            dictionary.veneer_name(0x0200_9344, 0x0800_0200, Some("Registered")),
            "Registered"
        );
        assert_eq!(
            dictionary.veneer_name(0x0200_9344, 0x0800_0200, None),
            "Main_08000200"
        );
        assert_eq!(dictionary.local_name(0x0200_0200, None), "Scene_Helper");
        assert_eq!(dictionary.local_name(0x0200_0200, Some("Named")), "Named");
        assert_eq!(dictionary.local_name(0x0200_0400, None), "Local_02000400");
    }

    #[test]
    fn binds_what_the_source_uses_and_the_link_does_not() {
        let dictionary = dictionary();
        let source = "/* Engine_Other in a comment */\nvoid Mine(void)\n{\n    Engine_TaskWait(1);\n    Main_08000200();\n    Local_02000400();\n    Local_02004000();\n    Registered();\n    gEventWork[0] = 1;\n    Func_02000100();\n}\n";
        let kept = BTreeMap::from([(
            "Func_02000100".to_string(),
            Bound {
                address: 0x0200_8100,
                data: false,
            },
        )]);
        let bound = bind([source], &kept, &dictionary, |name| {
            name == "Registered" || name.starts_with("Func_")
        });
        assert_eq!(
            bound.keys().map(String::as_str).collect::<Vec<_>>(),
            [
                "Engine_TaskWait",
                "Func_02000100",
                "Local_02000400",
                "Main_08000200",
                "gEventWork"
            ]
        );
        assert_eq!(
            bound["Engine_TaskWait"],
            Bound {
                address: 0x0200_933c,
                data: false
            }
        );
        assert_eq!(bound["Local_02000400"].address, 0x0200_8400);
        assert!(bound["gEventWork"].data);
        assert_eq!(
            symbols_json(&bound)["gEventWork"],
            json!({"address": "0x03001ebc", "kind": "data"})
        );
    }

    #[test]
    fn declared_symbols_read_units_and_instances() {
        let units = json!({"units": [{"overlay": "resource_3ba", "absolute_symbols": {"A": {"address": "0x02009000", "kind": "thumb"}},
            "instances": {"resource_3bb": {"absolute_symbols": {"B": {"address": "0x03000000", "kind": "data"}}}}}]});
        assert_eq!(
            declared_symbols(&units),
            [
                declared("resource_3ba", "A", 0x0200_9000, false),
                declared("resource_3bb", "B", 0x0300_0000, true)
            ]
        );
    }

    #[test]
    fn registered_names_resolve_from_both_record_forms() {
        let root = tempfile::tempdir().unwrap();
        let register = json!({"format": 3, "owners": {
            "main:080773d8": "GAME/CHARACTER/GET_RECORD.C",
            "main:080974d8": {"name": "Camera_WorldToScreen", "source": "GRAPHICS/RENDER/WORLD_TO_SCREEN.C"},
            "main:08000170": {"name": "Heap_Alloc"}
        }});
        let paths = SourcePaths::parse(root.path(), &register.to_string()).unwrap();
        assert_eq!(
            registered_main(&paths, 0x0807_73d8).as_deref(),
            Some("GET_RECORD")
        );
        assert_eq!(
            registered_main(&paths, 0x0809_74d8).as_deref(),
            Some("Camera_WorldToScreen")
        );
        assert_eq!(
            registered_main(&paths, 0x0800_0170).as_deref(),
            Some("Heap_Alloc")
        );
        assert_eq!(registered_main(&paths, 0x0800_0180), None);
        assert!(link_binds(&paths, "resource_3ca", "GET_RECORD"));
        assert!(link_binds(&paths, "resource_3ca", "Heap_Alloc"));
        assert!(!link_binds(&paths, "resource_3ca", "Engine_TaskWait"));
    }

    #[test]
    fn conversion_renames_calls_merges_declarations_and_names_the_function() {
        let decompiled = "#include \"TYPES.H\"\n\n#define Function Func_02000194\n\nextern u8 Data_00000000[];\nvoid Func_020014f6();\nvoid Func_0200152e();\ns32 Func_02001616();\n\nvoid Function(void)\n{\n    Func_020014f6(1);\n    Func_0200152e(2);\n    Func_02001616();\n}\n";
        let calls = BTreeMap::from([
            ("Func_020014f6".to_string(), "Engine_TaskWait".to_string()),
            ("Func_0200152e".to_string(), "Engine_TaskWait".to_string()),
            ("Func_02001616".to_string(), "Main_08000170".to_string()),
        ]);
        assert_eq!(
            convert(decompiled, &calls, "BabiFune_Func02000194"),
            "#include \"TYPES.H\"\n\nextern u8 Data_00000000[];\nvoid Engine_TaskWait();\ns32 Main_08000170();\n\nvoid BabiFune_Func02000194(void)\n{\n    Engine_TaskWait(1);\n    Engine_TaskWait(2);\n    Main_08000170();\n}\n"
        );
    }

    #[test]
    fn pool_words_follow_pc_relative_loads() {
        // At 0x02000000: ldr r0, [pc, #4]; bx lr; nop; nop; .word 0x03001ebc
        let image = [
            0x01, 0x48, 0x70, 0x47, 0xc0, 0x46, 0xc0, 0x46, 0xbc, 0x1e, 0x00, 0x03,
        ];
        assert_eq!(
            pool_words(&image, 0x0200_0000, 12),
            [(0x0200_0008, 0x0300_1ebc)]
        );
        let comment = pool_comment(
            &[
                (0x0200_0008, 0x0300_1ebc),
                (0x0200_000c, 0x0200_8401),
                (0x0200_0010, 7),
            ],
            &dictionary(),
        );
        assert_eq!(
            comment,
            "/* Literal pool, read from the ROM:\n *   0x02000008: 0x03001ebc  gEventWork\n *   0x0200000c: 0x02008401  Local_02000400 (overlay function)\n *   0x02000010: 0x00000007  constant; Value_00000007 if the pool must hold it\n */\n"
        );
    }

    #[test]
    fn renames_the_last_exported_function() {
        let draft = "/* NONMATCHING: x */\n#define LinkLobby_Send Func_02000580\n#define Other Func_02000600\nstatic void helper(void)\n{\n}\n\nvoid Local_02000580(void)\n{\n    helper();\n}\n";
        assert_eq!(
            rename_function(draft, 0x0200_0580, "LinkLobby_Send").unwrap(),
            "/* NONMATCHING: x */\n#define Other Func_02000600\nstatic void helper(void)\n{\n}\n\nvoid LinkLobby_Send(void)\n{\n    helper();\n}\n"
        );
        assert_eq!(rename_function("int x;\n", 0x0200_0580, "A"), None);
        assert_eq!(
            defined_functions(
                "static __inline__ s32 Value0(s32 (*f)())\n{\n}\ns32 *Get(void)\n{\n}\n"
            ),
            ["Value0", "Get"]
        );
    }
}
