//! Ratchet for the showcase standard in AGENTS.md: every registered
//! module folder, the headers it includes and its overlay listing must stay
//! free of the reconstruction scaffolding the standard retires.
use regex::Regex;
use serde::Deserialize;
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::process::ExitCode;
use std::sync::OnceLock;

const GAME: &str = "games/THE BROKEN SEAL";
const REGISTRY: &str = "games/THE BROKEN SEAL/recon/showcase.json";
const USAGE: &str = "usage: alchemy check showcase\n\nFail when a folder registered in games/THE BROKEN SEAL/recon/showcase.json contains address names, alias defines, *_BODY.INC includes, literal EWRAM/IWRAM/ROM addresses in C, register, pool or spill comments, per-file source bindings, or an overlay listing with code or data directives beyond its allowlist.";

#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
struct Registry {
    format: u32,
    kind: String,
    folders: Vec<Folder>,
}

#[derive(Deserialize)]
#[serde(deny_unknown_fields)]
struct Folder {
    source: String,
    overlay: String,
    #[serde(default)]
    allowed_data: Vec<String>,
}

pub fn entry(arguments: &[String]) -> ExitCode {
    match arguments {
        [] => match check(crate::compiler::routing::root()) {
            Ok((folders, files)) => {
                println!("showcase ok: {folders} folders, {files} sources and headers");
                ExitCode::SUCCESS
            }
            Err(findings) => {
                for finding in findings {
                    eprintln!("{finding}");
                }
                ExitCode::FAILURE
            }
        },
        [flag] if flag == "-h" || flag == "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        _ => {
            eprintln!("{USAGE}");
            ExitCode::from(2)
        }
    }
}

fn pattern(cell: &'static OnceLock<Regex>, source: &str) -> &'static Regex {
    cell.get_or_init(|| Regex::new(source).expect("valid showcase pattern"))
}

/// Identifiers spelled from an address: legacy prefixes, or any name that
/// carries an eight-digit EWRAM, IWRAM or ROM address.
fn address_name(text: &str) -> Option<String> {
    static PREFIXED: OnceLock<Regex> = OnceLock::new();
    static EMBEDDED: OnceLock<Regex> = OnceLock::new();
    let prefixed = pattern(
        &PREFIXED,
        r"\b(?:Func|Data|Value|ADDR|sub|loc)_[0-9A-Fa-f]{4,8}\w*",
    );
    let embedded = pattern(&EMBEDDED, r"\b[A-Za-z_]\w*?(?:02|03|08)[0-9A-Fa-f]{6}\w*");
    prefixed
        .find(text)
        .or_else(|| embedded.find(text))
        .map(|found| found.as_str().to_string())
}

/// `#define NAME OTHER` and `#define NAME(args) OTHER(args)`: a second spelling
/// of an existing identifier rather than a value or an expression.
fn alias_define(line: &str) -> bool {
    static OBJECT: OnceLock<Regex> = OnceLock::new();
    static FUNCTION: OnceLock<Regex> = OnceLock::new();
    let object = pattern(
        &OBJECT,
        r"^\s*#\s*define\s+\w+\s+\(?\s*[A-Za-z_]\w*\s*\)?\s*(?:/\*.*\*/)?\s*$",
    );
    let function = pattern(
        &FUNCTION,
        r"^\s*#\s*define\s+\w+\(([^)]*)\)\s+[A-Za-z_]\w*\s*\(([^)]*)\)\s*$",
    );
    if object.is_match(line) {
        return true;
    }
    function.captures(line).is_some_and(|captures| {
        let parameters = |index: usize| {
            captures[index]
                .split(',')
                .map(str::trim)
                .collect::<Vec<_>>()
        };
        parameters(1) == parameters(2)
    })
}

fn body_include(line: &str) -> bool {
    static BODY: OnceLock<Regex> = OnceLock::new();
    pattern(&BODY, r#"(?i)^\s*#\s*include\s*"[^"]*_BODY\.INC""#).is_match(line)
}

fn literal_address(text: &str) -> Option<String> {
    static LITERAL: OnceLock<Regex> = OnceLock::new();
    pattern(&LITERAL, r"\b0[xX]0[238][0-9A-Fa-f]{6}\b")
        .find(text)
        .map(|found| found.as_str().to_string())
}

/// Reverse-engineering vocabulary that belongs in dossiers, not in comments
/// describing game behavior.
fn machine_comment(comment: &str) -> Option<String> {
    static WORDS: OnceLock<Regex> = OnceLock::new();
    pattern(
        &WORDS,
        r"(?i)\b(?:pool|spill\w*|register\w*|compiler|r(?:[0-9]|1[0-2])|sp|lr|pc|ip|fp|sl)\b",
    )
    .find(comment)
    .map(|found| found.as_str().to_string())
}

fn comments(text: &str) -> Vec<&str> {
    static COMMENT: OnceLock<Regex> = OnceLock::new();
    pattern(&COMMENT, r"(?s)/\*.*?\*/|//[^\n]*")
        .find_iter(text)
        .map(|found| found.as_str())
        .collect()
}

fn quoted_includes(text: &str) -> Vec<String> {
    static INCLUDE: OnceLock<Regex> = OnceLock::new();
    pattern(&INCLUDE, r#"(?m)^\s*#\s*include\s*"([^"]+)""#)
        .captures_iter(text)
        .map(|captures| captures[1].to_string())
        .collect()
}

fn source_findings(path: &str, text: &str) -> Vec<String> {
    let mut findings = Vec::new();
    let mut code = text.to_string();
    for comment in comments(text) {
        if let Some(word) = machine_comment(comment) {
            findings.push(format!("{path}: machine-level comment ({word})"));
        }
        code = code.replacen(comment, "", 1);
    }
    for (index, line) in text.lines().enumerate() {
        let at = format!("{path}:{}", index + 1);
        if alias_define(line) {
            findings.push(format!("{at}: alias define"));
        }
        if body_include(line) {
            findings.push(format!("{at}: *_BODY.INC include"));
        }
    }
    if let Some(name) = address_name(text) {
        findings.push(format!("{path}: address name {name}"));
    }
    if let Some(literal) = literal_address(&code) {
        findings.push(format!("{path}: literal address {literal}"));
    }
    findings
}

const DATA_DIRECTIVES: &[&str] = &[
    ".byte", ".2byte", ".4byte", ".8byte", ".word", ".short", ".long", ".int", ".ascii", ".asciz",
    ".string", ".incbin", ".fill",
];

/// An overlay listing may keep its veneer includes, placeholders and alignment;
/// data directives need an allowlist entry and code or symbol glue is refused.
fn listing_findings(path: &str, text: &str, allowed: &[String]) -> Vec<String> {
    static LABEL: OnceLock<Regex> = OnceLock::new();
    static INCLUDE: OnceLock<Regex> = OnceLock::new();
    let label = pattern(&LABEL, r"^(?:Overlay|AlchemyC|AlchemyData)_[0-9a-f]{8}:$");
    let include = pattern(
        &INCLUDE,
        r#"^\.include\s+"[^"]*/(?:OVERLAY|ENTRY|IMPORT)\.INC"$"#,
    );
    let mut remaining = BTreeMap::<&str, usize>::new();
    for line in allowed {
        *remaining.entry(line.trim()).or_default() += 1;
    }
    let mut findings = Vec::new();
    for (index, raw) in text.lines().enumerate() {
        let line = raw.trim();
        let directive = line.split_whitespace().next().unwrap_or("");
        let structural = line.is_empty()
            || line == ".syntax unified"
            || line == ".thumb"
            || line.starts_with(".global Overlay_")
            || label.is_match(line)
            || include.is_match(line)
            || directive == ".space"
            || directive == ".balign"
            || directive == ".align";
        if structural {
            continue;
        }
        let at = format!("{path}:{}", index + 1);
        if DATA_DIRECTIVES.contains(&directive) {
            match remaining.get_mut(line) {
                Some(count) if *count > 0 => *count -= 1,
                _ => findings.push(format!(
                    "{at}: data directive outside the allowlist: {line}"
                )),
            }
        } else {
            findings.push(format!("{at}: retained code or symbol glue: {line}"));
        }
    }
    findings
}

fn read(root: &Path, relative: &str) -> Result<String, String> {
    std::fs::read_to_string(root.join(relative)).map_err(|error| format!("{relative}: {error}"))
}

fn json<T: serde::de::DeserializeOwned>(root: &Path, relative: &str) -> Result<T, String> {
    serde_json::from_str(&read(root, relative)?).map_err(|error| format!("{relative}: {error}"))
}

fn files_under(root: &Path, directory: &str) -> Result<Vec<String>, String> {
    let mut files = Vec::new();
    let mut pending = vec![PathBuf::from(directory)];
    while let Some(relative) = pending.pop() {
        let entries = std::fs::read_dir(root.join(&relative))
            .map_err(|error| format!("{}: {error}", relative.display()))?;
        for entry in entries.filter_map(Result::ok) {
            let child = relative.join(entry.file_name());
            if entry.path().is_dir() {
                pending.push(child);
            } else {
                files.push(child.to_string_lossy().replace('\\', "/"));
            }
        }
    }
    files.sort();
    Ok(files)
}

fn is_source(path: &str) -> bool {
    path.ends_with(".C") || path.ends_with(".H")
}

/// Every registered folder, its C sources, the quoted headers they reach and
/// its overlay listing, returning the folder and file counts or every finding.
pub(crate) fn check(root: &Path) -> Result<(usize, usize), Vec<String>> {
    let registry: Registry = json(root, REGISTRY).map_err(|error| vec![error])?;
    if registry.format != 1
        || registry.kind != "tbs-showcase-modules"
        || registry.folders.is_empty()
    {
        return Err(vec![format!(
            "{REGISTRY}: expected tbs-showcase-modules format 1"
        )]);
    }
    let register: serde_json::Value =
        json(root, &format!("{GAME}/source-paths.json")).map_err(|error| vec![error])?;
    let bindings: serde_json::Value =
        json(root, &format!("{GAME}/recon/source-bindings.json")).map_err(|error| vec![error])?;
    let mut findings = Vec::new();
    let mut scanned = BTreeSet::new();
    for folder in &registry.folders {
        let prefix = folder
            .source
            .strip_prefix("SRC/")
            .map(|relative| format!("{relative}/"));
        let Some(prefix) = prefix else {
            findings.push(format!("{REGISTRY}: {} is not under SRC", folder.source));
            continue;
        };
        let owners = register["owners"]
            .as_object()
            .into_iter()
            .flatten()
            .filter(|(_, record)| {
                record["source"]
                    .as_str()
                    .is_some_and(|source| source.starts_with(&prefix))
            })
            .map(|(owner, _)| owner.clone())
            .collect::<Vec<_>>();
        if owners.is_empty()
            || owners
                .iter()
                .any(|owner| !owner.starts_with(&format!("{}:", folder.overlay)))
        {
            findings.push(format!(
                "{}: registered owners must exist and all belong to {}",
                folder.source, folder.overlay
            ));
        }
        for key in bindings["files"]
            .as_object()
            .into_iter()
            .flatten()
            .map(|(key, _)| key)
        {
            if key.starts_with(&prefix) {
                findings.push(format!(
                    "{GAME}/SRC/{key}: per-file source bindings alias its names"
                ));
            }
        }
        let directory = format!("{GAME}/{}", folder.source);
        let mut pending = match files_under(root, &directory) {
            Ok(files) => files
                .into_iter()
                .filter(|path| is_source(path))
                .collect::<Vec<_>>(),
            Err(error) => {
                findings.push(error);
                continue;
            }
        };
        while let Some(path) = pending.pop() {
            if !scanned.insert(path.clone()) {
                continue;
            }
            let text = match read(root, &path) {
                Ok(text) => text,
                Err(error) => {
                    findings.push(error);
                    continue;
                }
            };
            findings.extend(source_findings(&path, &text));
            for include in quoted_includes(&text) {
                let beside = Path::new(&path).with_file_name(&include);
                let shared = format!("{GAME}/INCLUDE/{include}");
                let beside = beside.to_string_lossy().replace('\\', "/");
                if root.join(&beside).is_file() {
                    pending.push(beside);
                } else if root.join(&shared).is_file() {
                    pending.push(shared);
                } else {
                    findings.push(format!("{path}: unresolved include {include}"));
                }
            }
        }
        let listing = format!("{GAME}/raw/overlays/{}_overlay.s", folder.overlay);
        match read(root, &listing) {
            Ok(text) => findings.extend(listing_findings(&listing, &text, &folder.allowed_data)),
            Err(error) => findings.push(error),
        }
    }
    if findings.is_empty() {
        Ok((registry.folders.len(), scanned.len()))
    } else {
        Err(findings)
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tempfile::tempdir;

    const CLEAN_SOURCE: &str = "#include \"SCENE.H\"\n\n/* The keeper greets the party. */\nconst s16 gKeeperLines[] = { 1, 2 };\n\nvoid Keeper_Talk(void)\n{\n    Scene_Say(KEEPER_GREETING);\n}\n";
    const CLEAN_HEADER: &str = "#ifndef SCENE_H\n#define SCENE_H\n\nenum { KEEPER_GREETING = 0x1234 };\n#define TILES(count) ((count) << 4)\nvoid Scene_Say(s32 message);\n\n#endif\n";
    const CLEAN_LISTING: &str = ".syntax unified\n.include \"games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC\"\n\t.thumb\n\t.global Overlay_02000000\nOverlay_02000000:\n.include \"games/THE BROKEN SEAL/SRC/FIELD/KEEP/ENTRY.INC\"\nAlchemyC_02000030:\n\t.space 0x12\n\t.balign 4, 0\n.include \"games/THE BROKEN SEAL/SRC/FIELD/KEEP/IMPORT.INC\"\nAlchemyData_02000100:\n\t.space 0x20\n";

    struct Fixture {
        directory: tempfile::TempDir,
    }

    impl Fixture {
        fn new() -> Self {
            let fixture = Self {
                directory: tempdir().unwrap(),
            };
            fixture.write(
                "recon/showcase.json",
                r#"{"format": 1, "kind": "tbs-showcase-modules", "folders": [{"source": "SRC/FIELD/KEEP", "overlay": "resource_3ff"}]}"#,
            );
            fixture.write(
                "source-paths.json",
                r#"{"format": 3, "owners": {"resource_3ff:02000030": {"name": "Keeper_Talk", "source": "FIELD/KEEP/KEEP.C"}}}"#,
            );
            fixture.write(
                "recon/source-bindings.json",
                r#"{"format": 1, "kind": "tbs-source-bindings", "common": "", "files": {}}"#,
            );
            fixture.write("SRC/FIELD/KEEP/KEEP.C", CLEAN_SOURCE);
            fixture.write("INCLUDE/SCENE.H", CLEAN_HEADER);
            fixture.write("raw/overlays/resource_3ff_overlay.s", CLEAN_LISTING);
            fixture
        }

        fn write(&self, relative: &str, text: &str) {
            let path = self.directory.path().join(GAME).join(relative);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        }

        fn findings(&self) -> Vec<String> {
            check(self.directory.path()).err().unwrap_or_default()
        }
    }

    fn assert_finding(fixture: &Fixture, needle: &str) {
        let findings = fixture.findings();
        assert!(
            findings.iter().any(|finding| finding.contains(needle)),
            "expected {needle:?} in {findings:?}"
        );
    }

    #[test]
    fn clean_folder_passes() {
        let fixture = Fixture::new();
        assert_eq!(check(fixture.directory.path()), Ok((1, 2)));
    }

    #[test]
    fn address_names_fail() {
        for name in [
            "Func_02000308",
            "Data_03001ebc",
            "Value_00001be0",
            "SceneData_GetTable020084f8",
        ] {
            let fixture = Fixture::new();
            fixture.write(
                "SRC/FIELD/KEEP/KEEP.C",
                &CLEAN_SOURCE.replace("Scene_Say(KEEPER_GREETING)", &format!("{name}()")),
            );
            assert_finding(&fixture, &format!("address name {name}"));
        }
    }

    #[test]
    fn alias_defines_fail_but_value_macros_pass() {
        let fixture = Fixture::new();
        fixture.write(
            "INCLUDE/SCENE.H",
            &CLEAN_HEADER.replace("#endif", "#define Keeper_Say Scene_Say\n#endif"),
        );
        assert_finding(&fixture, "alias define");
        let fixture = Fixture::new();
        fixture.write(
            "INCLUDE/SCENE.H",
            &CLEAN_HEADER.replace("#endif", "#define Say(message) Scene_Say(message)\n#endif"),
        );
        assert_finding(&fixture, "alias define");
        assert!(!alias_define("#define TILES(count) ((count) << 4)"));
        assert!(!alias_define("#define TBS_EDITION_EN 1"));
    }

    #[test]
    fn body_includes_fail() {
        let fixture = Fixture::new();
        fixture.write("SRC/FIELD/KEEP/SPAWN_BODY.INC", "");
        fixture.write(
            "SRC/FIELD/KEEP/KEEP.C",
            &format!("#include \"SPAWN_BODY.INC\"\n{CLEAN_SOURCE}"),
        );
        assert_finding(&fixture, "*_BODY.INC include");
    }

    #[test]
    fn literal_addresses_fail_outside_comments_only() {
        let fixture = Fixture::new();
        fixture.write(
            "SRC/FIELD/KEEP/KEEP.C",
            &CLEAN_SOURCE.replace("{ 1, 2 }", "{ 1, 0x020084f8 }"),
        );
        assert_finding(&fixture, "literal address 0x020084f8");
        assert_eq!(literal_address("0x00100000 | 0x1be0"), None);
    }

    #[test]
    fn machine_comments_fail() {
        for comment in [
            "the pool word",
            "r4 holds the table",
            "keeps the register spill",
        ] {
            let fixture = Fixture::new();
            fixture.write(
                "SRC/FIELD/KEEP/KEEP.C",
                &CLEAN_SOURCE.replace("The keeper greets the party.", comment),
            );
            assert_finding(&fixture, "machine-level comment");
        }
    }

    #[test]
    fn listing_data_needs_an_allowlist_and_code_is_refused() {
        let fixture = Fixture::new();
        fixture.write(
            "raw/overlays/resource_3ff_overlay.s",
            &format!("{CLEAN_LISTING}\t.4byte 0x0000ffff\n"),
        );
        assert_finding(&fixture, "data directive outside the allowlist");
        fixture.write(
            "recon/showcase.json",
            r#"{"format": 1, "kind": "tbs-showcase-modules", "folders": [{"source": "SRC/FIELD/KEEP", "overlay": "resource_3ff", "allowed_data": [".4byte 0x0000ffff"]}]}"#,
        );
        assert_eq!(check(fixture.directory.path()), Ok((1, 2)));
        fixture.write(
            "raw/overlays/resource_3ff_overlay.s",
            &format!("{CLEAN_LISTING}\t.set sub_02000308, 0x02000308\n\tbx lr\n"),
        );
        assert_finding(&fixture, "retained code or symbol glue: .set");
        assert_finding(&fixture, "retained code or symbol glue: bx lr");
    }

    #[test]
    fn per_file_bindings_and_foreign_owners_fail() {
        let fixture = Fixture::new();
        fixture.write(
            "recon/source-bindings.json",
            r##"{"format": 1, "kind": "tbs-source-bindings", "common": "", "files": {"FIELD/KEEP/KEEP.C": "#define Keeper_Talk @F02000030\n"}}"##,
        );
        assert_finding(&fixture, "per-file source bindings");
        let fixture = Fixture::new();
        fixture.write(
            "source-paths.json",
            r#"{"format": 3, "owners": {"resource_3fe:02000030": {"name": "Keeper_Talk", "source": "FIELD/KEEP/KEEP.C"}}}"#,
        );
        assert_finding(&fixture, "all belong to resource_3ff");
    }
}
