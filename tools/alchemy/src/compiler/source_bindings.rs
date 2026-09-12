//! Per-source address bindings recovered from the owner register's companion
//! manifest. Production C spells semantic names; this file is never included
//! from `games/gs1/src` or `games/gs1/include`. The compile plan expands it
//! into a generated header under `out/`.
use serde::Deserialize;
use std::collections::{BTreeMap, HashSet};
use std::path::Path;
use std::sync::OnceLock;

pub const SOURCE_BINDINGS_MANIFEST: &str = "games/gs1/recon/source-bindings.json";

#[derive(Debug, Deserialize)]
struct Manifest {
    format: u32,
    kind: String,
    #[serde(default)]
    common: String,
    #[serde(default)]
    files: BTreeMap<String, String>,
}

static GS1: OnceLock<Result<Manifest, String>> = OnceLock::new();

fn load_gs1() -> Result<&'static Manifest, String> {
    match GS1.get_or_init(|| read_manifest(&crate::compiler::routing::root())) {
        Ok(manifest) => Ok(manifest),
        Err(error) => Err(error.clone()),
    }
}

fn read_manifest(root: &Path) -> Result<Manifest, String> {
    let path = root.join(SOURCE_BINDINGS_MANIFEST);
    if !path.exists() {
        return Ok(Manifest {
            format: 1,
            kind: "gs1-source-bindings".into(),
            common: String::new(),
            files: BTreeMap::new(),
        });
    }
    let manifest: Manifest = crate::compiler::build_io::read_json(&path)?;
    if manifest.format != 1 || manifest.kind != "gs1-source-bindings" {
        return Err(format!(
            "{}: expected gs1-source-bindings format 1",
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
    let src_root = root.join("games/gs1/src");
    let relative = source
        .strip_prefix(&src_root)
        .ok()
        .or_else(|| source.strip_prefix("games/gs1/src").ok())
        .or_else(|| {
            let nested = source.extension().and_then(|ext| ext.to_str()) == Some("c")
                && source.components().count() >= 2
                && !source.is_absolute();
            nested.then_some(source)
        })?;
    Some(relative.to_string_lossy().replace('\\', "/"))
}

/// Register names plus the per-source / common address map for one compile.
pub fn production_bindings(
    root: &Path,
    register_text: &str,
    source: Option<&Path>,
) -> Result<String, String> {
    let reserved = define_names(register_text);
    let mut text = String::new();
    let manifest = load_gs1()?;
    if !manifest.common.is_empty() {
        text.push_str(&filter_reserved_defines(
            &expand_binding_text(&manifest.common),
            &reserved,
        ));
        if !text.ends_with('\n') {
            text.push('\n');
        }
    }
    if let Some(source) = source {
        if let Some(key) = source_key(root, source) {
            if let Some(file) = manifest.files.get(&key) {
                text.push_str(&filter_reserved_defines(
                    &expand_binding_text(file),
                    &reserved,
                ));
                if !text.ends_with('\n') {
                    text.push('\n');
                }
            }
        }
    }
    let header = define_only_bindings(&text);
    let declared = source
        .and_then(|path| std::fs::read_to_string(path).ok())
        .map(|src| declared_function_names(&src))
        .unwrap_or_default();
    Ok(with_missing_forwards(&header, &declared))
}

/// Function-pointer casts of recovered aliases need a declaration. Skip
/// names the translation unit already prototypes so `void Name();` cannot
/// fight `s32 Name(...)`.
fn with_missing_forwards(header: &str, declared: &HashSet<String>) -> String {
    let mut forwards = String::new();
    let mut seen = HashSet::new();
    for line in header.lines() {
        let trimmed = line.trim_start();
        let Some(rest) = trimmed.strip_prefix("#define ") else {
            continue;
        };
        let Some(name) = rest.split_whitespace().next() else {
            continue;
        };
        let name = name.split('(').next().unwrap_or(name);
        if declared.contains(name) || !seen.insert(name.to_string()) {
            continue;
        }
        if rest.contains("Func_") || rest.contains("Call") || rest.contains("Value") {
            forwards.push_str("void ");
            forwards.push_str(name);
            forwards.push_str("();\n");
        }
    }
    if forwards.is_empty() {
        return header.to_string();
    }
    let mut out = String::with_capacity(header.len() + forwards.len() + 1);
    out.push_str(header);
    if !header.ends_with('\n') {
        out.push('\n');
    }
    out.push_str(&forwards);
    out
}

fn declared_function_names(src: &str) -> HashSet<String> {
    let mut names = HashSet::new();
    for line in src.lines() {
        let trimmed = line.trim_start();
        if trimmed.starts_with("#") || trimmed.starts_with("/*") || trimmed.starts_with("//") {
            continue;
        }
        if let Some(paren) = trimmed.find('(') {
            let prefix = &trimmed[..paren];
            if let Some(name) = prefix.split_whitespace().last() {
                if name.chars().all(|c| c.is_ascii_alphanumeric() || c == '_')
                    && name.starts_with(|c: char| c.is_ascii_alphabetic() || c == '_')
                {
                    names.insert(name.to_string());
                }
            }
        }
    }
    names
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

fn filter_reserved_defines(text: &str, reserved: &HashSet<String>) -> String {
    if reserved.is_empty() {
        return text.to_string();
    }
    let mut out = String::with_capacity(text.len());
    for line in text.lines() {
        let trimmed = line.trim_start();
        if let Some(rest) = trimmed.strip_prefix("#define ") {
            if let Some(name) = rest.split_whitespace().next() {
                let name = name.split('(').next().unwrap_or(name);
                if reserved.contains(name) {
                    continue;
                }
            }
        }
        out.push_str(line);
        out.push('\n');
    }
    out
}

/// Prototypes in the manifest mention `u8` / structs before `types.h` is
/// visible to `-include`. Keep preprocessor names, comments, and `extern`
/// data objects (rewritten to C89 builtins so `ADDR_*` macros still resolve).
fn define_only_bindings(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
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
    use super::{define_only_bindings, expand_binding_text};

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
            "/* Shared names:\n   callers use these aliases. */\n#define PackedTable_AdjustMarkedOffsets Func_08002f4c\n"
        );
    }

    #[test]
    fn keeps_extern_data_and_drops_typed_prototypes() {
        let text = "extern u8 Data_03001e70_a[];\nextern s32 Func_080072f0(s32 mode, u8 *destination);\n#define ADDR_03001E70 ((u32)Data_03001e70_a)\n";
        assert_eq!(
            define_only_bindings(text),
            "extern unsigned char Data_03001e70_a[];\n#define ADDR_03001E70 ((u32)Data_03001e70_a)\n"
        );
    }
}
