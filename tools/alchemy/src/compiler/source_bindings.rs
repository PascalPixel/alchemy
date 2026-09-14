//! Per-source address bindings recovered from the owner register's companion
//! manifest. Production C spells semantic names; this file is never included
//! from `games/tbs/SRC` or `games/tbs/INCLUDE`. The compile plan expands it
//! into a generated header under `out/`.
use serde::Deserialize;
use std::collections::{BTreeMap, HashSet};
use std::path::{Component, Path, PathBuf};
use std::sync::OnceLock;

pub const SOURCE_BINDINGS_MANIFEST: &str = "games/tbs/recon/source-bindings.json";

#[derive(Debug, Deserialize)]
struct Manifest {
    format: u32,
    kind: String,
    #[serde(default)]
    common: String,
    #[serde(default)]
    files: BTreeMap<String, String>,
}

static TBS: OnceLock<Result<Manifest, String>> = OnceLock::new();

fn load_tbs() -> Result<&'static Manifest, String> {
    match TBS.get_or_init(|| read_manifest(&crate::compiler::routing::root())) {
        Ok(manifest) => Ok(manifest),
        Err(error) => Err(error.clone()),
    }
}

fn read_manifest(root: &Path) -> Result<Manifest, String> {
    let path = root.join(SOURCE_BINDINGS_MANIFEST);
    if !path.exists() {
        return Ok(Manifest {
            format: 1,
            kind: "tbs-source-bindings".into(),
            common: String::new(),
            files: BTreeMap::new(),
        });
    }
    let manifest: Manifest = crate::compiler::build_io::read_json(&path)?;
    if manifest.format != 1 || manifest.kind != "tbs-source-bindings" {
        return Err(format!(
            "{}: expected tbs-source-bindings format 1",
            path.display()
        ));
    }
    Ok(manifest)
}

/// Expand the compact hex encoding used in the manifest. Address tokens are
/// stored without a `Func_` / `Data_` prefix so the repository source tree
/// does not publish those names as an API.
pub fn expand_binding_text(text: &str) -> String {
    let mut out = String::with_capacity(text.len() + text.len() / 8);
    let bytes = text.as_bytes();
    let mut i = 0;
    while i < bytes.len() {
        if bytes[i] == b'@' && i + 1 < bytes.len() {
            match bytes[i + 1] {
                b'F' => {
                    out.push_str("Func_");
                    i += 2;
                    continue;
                }
                b'D' => {
                    out.push_str("Data_");
                    i += 2;
                    continue;
                }
                _ => {}
            }
        }
        out.push(bytes[i] as char);
        i += 1;
    }
    out
}

fn source_key(root: &Path, source: &Path) -> Option<String> {
    let src_root = root.join("games/tbs/SRC");
    let relative = source
        .strip_prefix(&src_root)
        .ok()
        .or_else(|| source.strip_prefix("games/tbs/SRC").ok())
        .or_else(|| {
            let nested = is_c_source_path(source)
                && source.components().count() >= 2
                && !source.is_absolute();
            nested.then_some(source)
        })?;
    Some(relative.to_string_lossy().replace('\\', "/"))
}

fn is_c_source_path(path: &Path) -> bool {
    matches!(
        path.extension().and_then(|value| value.to_str()),
        Some("c" | "C")
    )
}

fn quoted_c_includes(text: &str) -> Vec<String> {
    let mut out = Vec::new();
    for raw in text.lines() {
        let line = raw.trim_start();
        let Some(rest) = line.strip_prefix("#include") else {
            continue;
        };
        let rest = rest.trim_start();
        let Some(inner) = rest.strip_prefix('"') else {
            continue;
        };
        let Some(end) = inner.find('"') else {
            continue;
        };
        out.push(inner[..end].to_string());
    }
    out
}

fn lexical_join(base: &Path, rel: &str) -> PathBuf {
    let mut out = base.to_path_buf();
    for component in Path::new(rel).components() {
        match component {
            Component::CurDir => {}
            Component::ParentDir => {
                out.pop();
            }
            Component::Normal(name) => out.push(name),
            Component::RootDir | Component::Prefix(_) => {
                out = PathBuf::from(rel);
                break;
            }
        }
    }
    out
}

/// Production src keys for this TU: the file itself, plus `#include`d `.c`/`.C`
/// files under `games/tbs/SRC`. Mixed leftover wrappers compile those src
/// files through a recon unit path that has no bindings key of its own.
fn included_source_keys(root: &Path, source: &Path) -> Vec<String> {
    let mut keys = Vec::new();
    let mut seen = HashSet::new();
    if let Some(key) = source_key(root, source) {
        seen.insert(key.clone());
        keys.push(key);
    }
    let Ok(text) = std::fs::read_to_string(source) else {
        return keys;
    };
    let Some(parent) = source.parent() else {
        return keys;
    };
    for include in quoted_c_includes(&text) {
        if !is_c_source_path(Path::new(&include)) {
            continue;
        }
        let resolved = lexical_join(parent, &include);
        if let Some(key) = source_key(root, &resolved) {
            if seen.insert(key.clone()) {
                keys.push(key);
            }
        }
    }
    keys
}

/// Register names plus the per-source / common address map for one compile.
pub fn production_bindings(
    root: &Path,
    register_text: &str,
    source: Option<&Path>,
) -> Result<String, String> {
    let mut reserved = define_names(register_text);
    if let Some(source) = source {
        if let Ok(source_text) = std::fs::read_to_string(source) {
            reserved.extend(type_tags(&source_text));
        }
    }
    let mut text = String::new();
    let manifest = load_tbs()?;
    if !manifest.common.is_empty() {
        text.push_str(&filter_reserved_defines(
            &expand_binding_text(&manifest.common),
            &mut reserved,
        ));
        if !text.ends_with('\n') {
            text.push('\n');
        }
    }
    if let Some(source) = source {
        for key in included_source_keys(root, source) {
            let src_path = root.join("games/tbs/SRC").join(&key);
            if let Ok(src_text) = std::fs::read_to_string(&src_path) {
                reserved.extend(type_tags(&src_text));
            }
            if let Some(file) = manifest.files.get(&key) {
                text.push_str(&filter_reserved_defines(
                    &expand_binding_text(file),
                    &mut reserved,
                ));
                if !text.ends_with('\n') {
                    text.push('\n');
                }
            }
        }
    }
    Ok(define_only_bindings(&text))
}

/// Overlay compile has one `-include` header: register names first, then
/// recovered aliases that do not override them.
pub fn with_register(register: &str, recovered: &str) -> String {
    let mut out = register.to_owned();
    if !out.is_empty() && !out.ends_with('\n') {
        out.push('\n');
    }
    out.push_str(recovered);
    out
}

/// Struct/union/enum/typedef tags in the C file must not be `#define`d away.
fn type_tags(text: &str) -> HashSet<String> {
    let mut names = HashSet::new();
    for raw in text.lines() {
        let line = raw.trim_start();
        if line.starts_with('#') || line.starts_with("//") || line.starts_with("/*") {
            continue;
        }
        for prefix in ["struct ", "union ", "enum "] {
            if let Some(rest) = line.strip_prefix(prefix) {
                if let Some(name) = rest
                    .split(|c: char| !c.is_ascii_alphanumeric() && c != '_')
                    .next()
                {
                    if !name.is_empty() {
                        names.insert(name.to_string());
                    }
                }
            }
        }
        if line.starts_with("typedef ") {
            if let Some(name) = line.trim_end_matches(';').split_whitespace().last() {
                let name = name.trim_start_matches('*').trim_end_matches(';');
                if !name.is_empty() && name != "{" && name != "}" {
                    names.insert(name.to_string());
                }
            }
        }
    }
    names
}

fn define_names(text: &str) -> HashSet<String> {
    let mut names = HashSet::new();
    for line in text.lines() {
        let trimmed = line.trim_start();
        if let Some(rest) = trimmed.strip_prefix("#define ") {
            if let Some(name) = rest.split_whitespace().next() {
                names.insert(name.split('(').next().unwrap_or(name).to_string());
            }
        }
    }
    names
}

fn filter_reserved_defines(text: &str, reserved: &mut HashSet<String>) -> String {
    let mut out = String::with_capacity(text.len());
    for line in text.lines() {
        let trimmed = line.trim_start();
        if let Some(rest) = trimmed.strip_prefix("#define ") {
            if let Some(name) = rest.split_whitespace().next() {
                let name = name.split('(').next().unwrap_or(name);
                if !reserved.insert(name.to_string()) {
                    continue;
                }
            }
        }
        out.push_str(line);
        out.push('\n');
    }
    out
}

/// Preserve recovered declarations as well as aliases. Dropping prototypes
/// loses callback declarations and changes C89 argument/return conversions.
fn define_only_bindings(text: &str) -> String {
    let mut out = String::from("#include \"TYPES.H\"\n");
    let mut in_block_comment = false;
    for line in text.lines() {
        let trimmed = line.trim_start();
        if in_block_comment {
            out.push_str(line);
            out.push('\n');
            if trimmed.contains("*/") {
                in_block_comment = false;
            }
            continue;
        }
        if trimmed.starts_with("/*") {
            out.push_str(line);
            out.push('\n');
            if !trimmed.contains("*/") {
                in_block_comment = true;
            }
            continue;
        }
        if trimmed.is_empty() || trimmed.starts_with('#') || trimmed.starts_with("//") {
            out.push_str(line);
            out.push('\n');
            continue;
        }
        if trimmed.starts_with("extern ") && !trimmed.contains('(') {
            out.push_str(&rewrite_extern_data_types(line));
            out.push('\n');
        } else {
            out.push_str(line);
            out.push('\n');
        }
    }
    out
}

fn rewrite_extern_data_types(line: &str) -> String {
    line.replace("u16", "unsigned short")
        .replace("s16", "short")
        .replace("u32", "unsigned int")
        .replace("s32", "int")
        .replace("u8", "unsigned char")
        .replace("s8", "signed char")
}

#[cfg(test)]
mod tests {
    use super::{
        define_only_bindings, expand_binding_text, filter_reserved_defines, included_source_keys,
        lexical_join, quoted_c_includes, source_key, type_tags,
    };
    use std::collections::HashSet;
    use std::path::Path;
    use tempfile::tempdir;

    #[test]
    fn expands_compact_func_and_data_tokens() {
        assert_eq!(
            expand_binding_text("#define Field_unk17 @F020038b0_a\n#define gWork @D03001ebc\n"),
            "#define Field_unk17 Func_020038b0_a\n#define gWork Data_03001ebc\n"
        );
    }

    #[test]
    fn keeps_multiline_comments_with_defines() {
        let text = "/* Shared names:\n   callers use these aliases. */\nextern s32 Func_08002f4c(u8 *p);\n#define PackedTable_AdjustMarkedOffsets Func_08002f4c\n";
        assert_eq!(
            define_only_bindings(text),
            "#include \"TYPES.H\"\n/* Shared names:\n   callers use these aliases. */\nextern s32 Func_08002f4c(u8 *p);\n#define PackedTable_AdjustMarkedOffsets Func_08002f4c\n"
        );
    }

    #[test]
    fn keeps_extern_data_and_typed_prototypes() {
        let text = "extern u8 Data_03001e70_a[];\nextern s32 Func_080072f0(s32 mode, u8 *destination);\n#define ADDR_03001E70 ((u32)Data_03001e70_a)\n";
        assert_eq!(
            define_only_bindings(text),
            "#include \"TYPES.H\"\nextern unsigned char Data_03001e70_a[];\nextern s32 Func_080072f0(s32 mode, u8 *destination);\n#define ADDR_03001E70 ((u32)Data_03001e70_a)\n"
        );
    }

    #[test]
    fn first_define_wins_for_recovered_aliases() {
        let mut reserved = HashSet::new();
        let text = "#define gRom Data_08028195\n#define gRom Data_0803740f\n#define gVal Data_0000001f\n#define gVal Data_00000024\n";
        assert_eq!(
            filter_reserved_defines(text, &mut reserved),
            "#define gRom Data_08028195\n#define gVal Data_0000001f\n"
        );
    }

    #[test]
    fn reserves_struct_and_typedef_tags() {
        let tags =
            type_tags("struct gRom {\n    u8 x;\n};\ntypedef struct { u8 bytes[4]; } gVal;\n");
        assert!(tags.contains("gRom"));
        assert!(tags.contains("gVal"));
    }

    #[test]
    fn mixed_unit_includes_resolve_to_src_keys() {
        let includes = quoted_c_includes(
            "#include \"../main/0808fe38.C\"\n#include \"../../../SRC/BATTLE/EFFECT/ENABLE_TWO_CALLBACKS.C\"\n",
        );
        assert_eq!(
            includes,
            [
                "../main/0808fe38.C",
                "../../../SRC/BATTLE/EFFECT/ENABLE_TWO_CALLBACKS.C"
            ]
        );
        let unit = Path::new("/workspace/games/tbs/recon/en/units/unit-0808fe38.c");
        let resolved = lexical_join(unit.parent().unwrap(), &includes[1]);
        assert_eq!(
            source_key(Path::new("/workspace"), &resolved).as_deref(),
            Some("BATTLE/EFFECT/ENABLE_TWO_CALLBACKS.C")
        );
        assert_eq!(source_key(Path::new("/workspace"), unit), None);
    }

    #[test]
    fn uppercase_c_sources_and_includes_resolve_to_src_keys() {
        let root = tempdir().unwrap();
        let source_root = root.path().join("games/tbs/SRC");
        let source = source_root.join("battle/main.C");
        let included = source_root.join("battle/helper.C");
        std::fs::create_dir_all(source.parent().unwrap()).unwrap();
        std::fs::write(&source, "#include \"helper.C\"\n").unwrap();
        std::fs::write(&included, "void helper(void) {}\n").unwrap();

        assert_eq!(
            source_key(root.path(), &source).as_deref(),
            Some("battle/main.C")
        );
        assert_eq!(
            included_source_keys(root.path(), &source),
            ["battle/main.C", "battle/helper.C"]
        );
    }
}
