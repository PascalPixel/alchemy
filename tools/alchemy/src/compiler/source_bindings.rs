//! Per-source address bindings recovered from the owner register's companion
//! manifest. Production C spells semantic names; this file is never included
//! from `games/gs1/src` or `games/gs1/include`. The compile plan expands it
//! into a generated header under `out/`.
use serde::Deserialize;
use std::collections::BTreeMap;
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
    let mut text = register_text.to_owned();
    if !text.is_empty() && !text.ends_with('\n') {
        text.push('\n');
    }
    let manifest = load_gs1()?;
    if !manifest.common.is_empty() {
        text.push_str(&expand_binding_text(&manifest.common));
        if !text.ends_with('\n') {
            text.push('\n');
        }
    }
    if let Some(source) = source {
        if let Some(key) = source_key(root, source) {
            if let Some(file) = manifest.files.get(&key) {
                text.push_str(&expand_binding_text(file));
                if !text.ends_with('\n') {
                    text.push('\n');
                }
            }
        }
    }
    Ok(define_only_bindings(&text))
}

/// Prototypes in the manifest mention `u8` / structs before `types.h` is
/// visible to `-include`. Keep preprocessor names and comments only.
fn define_only_bindings(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    for line in text.lines() {
        let trimmed = line.trim_start();
        if trimmed.is_empty()
            || trimmed.starts_with('#')
            || trimmed.starts_with("/*")
            || trimmed.starts_with('*')
            || trimmed.starts_with("//")
        {
            out.push_str(line);
            out.push('\n');
        }
    }
    out
}

#[cfg(test)]
mod tests {
    use super::expand_binding_text;

    #[test]
    fn expands_compact_func_and_data_tokens() {
        assert_eq!(
            expand_binding_text("#define Field_unk17 @F020038b0_a\n#define gWork @D03001ebc\n"),
            "#define Field_unk17 Func_020038b0_a\n#define gWork Data_03001ebc\n"
        );
    }
}
