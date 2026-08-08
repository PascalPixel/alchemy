//! Architecture checks for the native Rust tooling tree.
//!
//! The dispatcher is the registry of user-facing tools.  Cargo manifests are
//! the topology: each directory under `tools/` with a manifest is a crate,
//! its binary declarations are its entry points, and `path` dependencies are
//! its edges.  Keeping those three facts in the gate makes it useful after the
//! old source tree disappears.

use std::collections::{BTreeMap, BTreeSet, HashSet};
use std::fs;
use std::path::{Path, PathBuf};

use dispatch::{all_entries, Entry, Target};

/// Repository-level files whose commands and native paths are part of the
/// architecture contract.  Keep this list deliberately independent of any
/// package manager: the Makefile and installed native hooks are the workflow
/// entry points after the TypeScript tooling is gone.
pub const ROOT_SOURCE_FILES: &[&str] = &["Makefile"];
pub const HOOK_DIRECTORY: &str = ".hooks";

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct NativeCrate {
    pub directory: String,
    pub package: String,
    pub binaries: Vec<String>,
    pub path_dependencies: Vec<String>,
    pub invalid_paths: Vec<String>,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct NativeTarget {
    pub group: String,
    pub name: String,
    pub path: String,
}

fn quoted_value(line: &str, key: &str) -> Option<String> {
    let line = line.trim();
    let rest = line.strip_prefix(key)?.trim_start();
    let rest = rest.strip_prefix('=')?.trim_start();
    let rest = rest.strip_prefix('"')?;
    let end = rest.find('"')?;
    Some(rest[..end].to_string())
}

fn inline_quoted_value(line: &str, key: &str) -> Option<String> {
    let needle = format!("{key} = \"");
    let start = line.find(&needle)? + needle.len();
    let rest = &line[start..];
    let end = rest.find('"')?;
    Some(rest[..end].to_string())
}

fn section_name(line: &str) -> Option<&str> {
    let line = line.trim();
    line.strip_prefix('[')?.strip_suffix(']')
}

fn normalize(path: &Path) -> PathBuf {
    let mut result = PathBuf::new();
    for component in path.components() {
        match component {
            std::path::Component::CurDir => {}
            std::path::Component::ParentDir => {
                result.pop();
            }
            other => result.push(other.as_os_str()),
        }
    }
    result
}

fn binary_declarations(manifest: &str, package: &str) -> Vec<(String, String)> {
    let mut declarations = Vec::new();
    let mut in_bin = false;
    let mut name: Option<String> = None;
    let mut path: Option<String> = None;
    let finish = |declarations: &mut Vec<(String, String)>,
                  in_bin: bool,
                  name: &mut Option<String>,
                  path: &mut Option<String>| {
        if in_bin {
            declarations.push((
                name.take().unwrap_or_else(|| package.to_string()),
                path.take().unwrap_or_else(|| "src/main.rs".to_string()),
            ));
        }
    };
    for line in manifest.lines() {
        if line.trim() == "[[bin]]" {
            finish(&mut declarations, in_bin, &mut name, &mut path);
            in_bin = true;
            continue;
        }
        if line.trim_start().starts_with('[') {
            finish(&mut declarations, in_bin, &mut name, &mut path);
            in_bin = false;
            continue;
        }
        if !in_bin {
            continue;
        }
        if name.is_none() {
            name = quoted_value(line, "name");
        }
        if path.is_none() {
            path = quoted_value(line, "path");
        }
    }
    finish(&mut declarations, in_bin, &mut name, &mut path);
    declarations
}

fn package_name(manifest: &str) -> Option<String> {
    let mut in_package = false;
    for line in manifest.lines() {
        let section = section_name(line);
        if let Some(section) = section {
            in_package = section == "package";
            continue;
        }
        if in_package {
            if let Some(name) = quoted_value(line, "name") {
                return Some(name);
            }
        }
    }
    None
}

fn parse_crate(root: &Path, directory: &Path) -> Result<NativeCrate, String> {
    let manifest_path = directory.join("Cargo.toml");
    let manifest = fs::read_to_string(&manifest_path)
        .map_err(|error| format!("{}: {error}", manifest_path.display()))?;
    let package = package_name(&manifest)
        .ok_or_else(|| format!("{} has no [package] name", manifest_path.display()))?;
    let directory_name = directory
        .file_name()
        .and_then(|name| name.to_str())
        .ok_or_else(|| format!("{} has no valid directory name", directory.display()))?
        .to_string();

    let mut binaries = Vec::new();
    let mut invalid_paths = Vec::new();
    for (name, path) in binary_declarations(&manifest, &package) {
        if directory.join(&path).is_file() {
            binaries.push(name);
        } else {
            invalid_paths.push(format!("tools/{directory_name}/{path}"));
        }
    }
    if directory.join("src/main.rs").is_file() && !binaries.iter().any(|name| name == &package) {
        binaries.push(package.clone());
    }
    let bin_directory = directory.join("src/bin");
    if let Ok(entries) = fs::read_dir(&bin_directory) {
        for entry in entries.flatten() {
            let path = entry.path();
            if path.extension().and_then(|extension| extension.to_str()) == Some("rs") {
                if let Some(stem) = path.file_stem().and_then(|stem| stem.to_str()) {
                    let name = stem.replace('_', "-");
                    if !binaries.iter().any(|existing| existing == &name) {
                        binaries.push(name);
                    }
                }
            }
        }
    }

    let tools_rs = root.join("tools");
    let mut path_dependencies = Vec::new();
    for line in manifest.lines() {
        let Some(path) = inline_quoted_value(line, "path") else {
            continue;
        };
        if !path.starts_with('.') {
            continue;
        }
        let dependency = normalize(&directory.join(&path));
        if let Ok(relative) = dependency.strip_prefix(&tools_rs) {
            let relative = relative.to_string_lossy().replace('\\', "/");
            if relative.contains('/') || relative.is_empty() {
                invalid_paths.push(format!("tools/{directory_name}/{path}"));
            } else {
                path_dependencies.push(relative);
            }
        } else {
            invalid_paths.push(format!("tools/{directory_name}/{path}"));
        }
        if !dependency.join("Cargo.toml").is_file() && path.starts_with("../") {
            invalid_paths.push(format!("tools/{directory_name}/{path}"));
        }
    }
    binaries.sort();
    binaries.dedup();
    path_dependencies.sort();
    path_dependencies.dedup();
    invalid_paths.sort();
    invalid_paths.dedup();
    Ok(NativeCrate {
        directory: directory_name,
        package,
        binaries,
        path_dependencies,
        invalid_paths,
    })
}

/// Discover the immediate native crate topology under the repository root.
pub fn scan_crates(root: &Path) -> Result<Vec<NativeCrate>, String> {
    let tools_rs = root.join("tools");
    let entries = fs::read_dir(&tools_rs)
        .map_err(|error| format!("cannot scan {}: {error}", tools_rs.display()))?;
    let mut crates = Vec::new();
    for entry in entries.flatten() {
        let path = entry.path();
        if !path.is_dir() || path.file_name().and_then(|name| name.to_str()) == Some("target") {
            continue;
        }
        if path.join("Cargo.toml").is_file() {
            crates.push(parse_crate(root, &path)?);
        }
    }
    crates.sort_by(|left, right| left.directory.as_bytes().cmp(right.directory.as_bytes()));
    Ok(crates)
}

pub fn dispatch_targets() -> Vec<NativeTarget> {
    all_entries()
        .map(|(group, Entry { name, target })| {
            let Target::Binary(path) = target;
            NativeTarget {
                group: group.name().to_string(),
                name: name.to_string(),
                path: path.to_string(),
            }
        })
        .collect()
}

fn crate_map<'a>(crates: &'a [NativeCrate]) -> BTreeMap<&'a str, &'a NativeCrate> {
    crates
        .iter()
        .map(|native| (native.directory.as_str(), native))
        .collect()
}

/// Resolve a dispatcher or generated-workspace binary path to its crate.
pub fn target_crate(path: &str, crates: &[NativeCrate]) -> Result<String, String> {
    let path = path.replace('\\', "/");
    let parts: Vec<&str> = path.split('/').collect();
    if parts.len() < 4 || parts[0] != "tools" {
        return Err(format!("{path} is outside tools"));
    }
    let (crate_name, binary) = if parts.len() == 4 && parts[1] == "target" {
        (None, parts[3])
    } else if parts.len() == 5 && parts[2] == "target" && parts[3] == "release" {
        (Some(parts[1]), parts[4])
    } else {
        return Err(format!("{path} is not a native release binary path"));
    };
    let by_directory = crate_map(crates);
    let candidate = match crate_name {
        Some(name) => by_directory
            .get(name)
            .filter(|native| native.binaries.iter().any(|known| known == binary))
            .map(|native| native.directory.clone()),
        None => {
            let matches: Vec<String> = crates
                .iter()
                .filter(|native| native.binaries.iter().any(|known| known == binary))
                .map(|native| native.directory.clone())
                .collect();
            if matches.len() == 1 {
                matches.into_iter().next()
            } else {
                None
            }
        }
    };
    candidate.ok_or_else(|| format!("{path} does not name one native binary"))
}

/// Validate every dispatch entry and return its root crates.
pub fn valid_dispatch_targets(
    targets: &[NativeTarget],
    crates: &[NativeCrate],
) -> (BTreeSet<String>, Vec<String>) {
    let mut roots = BTreeSet::new();
    let mut problems = Vec::new();
    let mut names = HashSet::new();
    for target in targets {
        if !names.insert((target.group.clone(), target.name.clone())) {
            problems.push(format!(
                "dispatch registers {} twice in {}",
                target.name, target.group
            ));
        }
        match target_crate(&target.path, crates) {
            Ok(crate_name) => {
                roots.insert(crate_name);
            }
            Err(error) => problems.push(format!(
                "dispatch {} {}: {error}",
                target.group, target.name
            )),
        }
    }
    (roots, problems)
}

fn path_char(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || matches!(byte, b'_' | b'-' | b'/' | b'.')
}

fn legacy_source_path(path: &str) -> bool {
    matches!(
        Path::new(path)
            .extension()
            .and_then(|extension| extension.to_str()),
        Some("ts" | "tsx" | "js" | "mjs" | "cjs" | "py" | "pyi")
    )
}

/// Extract native paths from Markdown, manifests, and hook text.
pub fn native_paths(text: &str) -> Vec<String> {
    let bytes = text.as_bytes();
    let mut paths = Vec::new();
    let mut at = 0usize;
    while at + "tools/".len() <= bytes.len() {
        if &bytes[at..at + "tools/".len()] != b"tools/" {
            at += 1;
            continue;
        }
        let start = at;
        at += "tools/".len();
        while at < bytes.len() && path_char(bytes[at]) {
            at += 1;
        }
        let mut end = at;
        while end > start && matches!(bytes[end - 1], b'.' | b',' | b';' | b':') {
            end -= 1;
        }
        if end > start + "tools/".len() {
            let path = &text[start..end];
            if !legacy_source_path(path) {
                paths.push(path.to_string());
            }
        }
    }
    paths
}

/// Read the repository's authoritative command/path sources.
///
/// Markdown remains the documentation corpus.  The operational corpus is the
/// root Makefile plus every file in `.hooks`; `package.json` is intentionally
/// not consulted because it is neither required nor authoritative for the
/// native workspace.
pub fn authoritative_sources(root: &Path, markdown: &[(String, String)]) -> Vec<(String, String)> {
    let mut sources = markdown.to_vec();
    for name in ROOT_SOURCE_FILES {
        let path = root.join(name);
        if let Ok(text) = fs::read_to_string(&path) {
            let text = if *name == "Makefile" {
                // `TOOLS` is the Makefile's one repository-path variable.  It
                // expands to the native tree at execution time; expand it in
                // the scanner too so command targets participate in the same
                // path and reachability checks as literal hook paths.
                text.replace("$(TOOLS)", "tools")
            } else {
                text
            };
            sources.push(((*name).to_string(), text));
        }
    }
    let hooks = root.join(HOOK_DIRECTORY);
    let mut hook_paths = fs::read_dir(&hooks)
        .into_iter()
        .flatten()
        .flatten()
        .map(|entry| entry.path())
        .filter(|path| path.is_file())
        .collect::<Vec<_>>();
    hook_paths.sort_by(|left, right| left.as_os_str().cmp(right.as_os_str()));
    for path in hook_paths {
        if let Ok(text) = fs::read_to_string(&path) {
            let name = path
                .strip_prefix(root)
                .unwrap_or(&path)
                .to_string_lossy()
                .replace('\\', "/");
            sources.push((name, text));
        }
    }
    sources
}

fn rust_sources(directory: &Path, files: &mut Vec<PathBuf>) {
    let Ok(entries) = fs::read_dir(directory) else {
        return;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.file_name().and_then(|name| name.to_str()) == Some("target") {
            continue;
        }
        if path.is_dir() {
            rust_sources(&path, files);
        } else if path.extension().and_then(|extension| extension.to_str()) == Some("rs") {
            files.push(path);
        }
    }
}

fn command_name_after(source: &str, start: usize) -> Option<String> {
    let open = source[start..].find('(')? + start;
    let end = (open + 512).min(source.len());
    let call = &source[open..end];
    let quote = call.find('"')? + open + 1;
    let rest = &source[quote..end];
    let close = rest.find('"')?;
    let name = &source[quote..quote + close];
    (!name.is_empty()).then(|| name.to_string())
}

/// Find native subprocess edges that Cargo cannot express as dependencies.
///
/// Asset builders invoke small standalone crates by binary name. Those
/// binaries are real architecture roots even though the caller cannot depend
/// on them as Rust libraries. Only the native invocation helpers are scanned;
/// arbitrary strings and comments do not create reachability edges.
pub fn native_command_roots(root: &Path, crates: &[NativeCrate]) -> BTreeSet<String> {
    let mut files = Vec::new();
    rust_sources(&root.join("tools"), &mut files);
    let mut roots = BTreeSet::new();
    for file in files {
        let Ok(source) = fs::read_to_string(&file) else {
            continue;
        };
        for marker in ["run_tool(", "native_bytes(", "native_command("] {
            let mut at = 0;
            while let Some(found) = source[at..].find(marker) {
                let start = at + found;
                if let Some(name) = command_name_after(&source, start) {
                    if let Some(native) = crates.iter().find(|native| {
                        native.directory == name
                            || native.package == name
                            || native.directory.replace('-', "_") == name
                    }) {
                        roots.insert(native.directory.clone());
                    }
                }
                at = start + marker.len();
            }
        }
    }
    roots
}

/// Validate native paths appearing in repository documentation and workflow
/// sources.
pub fn valid_paths(root: &Path, texts: &[(String, String)], crates: &[NativeCrate]) -> Vec<String> {
    let by_directory = crate_map(crates);
    let mut problems = Vec::new();
    let mut seen = BTreeSet::new();
    for (source, text) in texts {
        for path in native_paths(text) {
            if !seen.insert((source.clone(), path.clone())) {
                continue;
            }
            let suffix = path.strip_prefix("tools/").unwrap_or_default();
            let parts: Vec<&str> = suffix.split('/').collect();
            let valid = if root.join(&path).exists() {
                true
            } else if suffix.starts_with("target/release/") {
                target_crate(&path, crates).is_ok()
            } else if suffix == "Cargo.toml" {
                root.join(&path).is_file()
            } else if parts
                .first()
                .is_some_and(|name| by_directory.contains_key(name))
            {
                if parts.len() == 1 {
                    true
                } else if parts.len() == 4 && parts[1] == "target" && parts[2] == "release" {
                    target_crate(&path, crates).is_ok()
                } else {
                    root.join(&path).exists()
                }
            } else {
                false
            };
            if !valid {
                problems.push(format!(
                    "{source} names {path}, which does not exist in the native tree"
                ));
            }
        }
    }
    problems
}

/// Read workspace member directory names so shared native crates are roots too.
pub fn workspace_members(manifest: &str) -> Vec<String> {
    let mut members = Vec::new();
    let mut in_members = false;
    for line in manifest.lines() {
        let line = line.trim();
        if line == "members = [" {
            in_members = true;
            continue;
        }
        if in_members && line == "]" {
            break;
        }
        if in_members {
            if let Some(member) = line
                .strip_prefix('"')
                .and_then(|line| line.strip_suffix(','))
            {
                if let Some(member) = member.strip_suffix('"') {
                    members.push(member.to_string());
                }
            }
        }
    }
    members
}

/// Walk path dependencies from registered native roots and report orphan crates.
pub fn unreachable_crates(crates: &[NativeCrate], roots: &BTreeSet<String>) -> Vec<String> {
    let by_directory = crate_map(crates);
    let mut seen = BTreeSet::new();
    let mut stack: Vec<String> = roots.iter().cloned().collect();
    while let Some(name) = stack.pop() {
        if !seen.insert(name.clone()) {
            continue;
        }
        if let Some(native) = by_directory.get(name.as_str()) {
            stack.extend(native.path_dependencies.iter().cloned());
        }
    }
    crates
        .iter()
        .filter(|native| !seen.contains(&native.directory))
        .map(|native| {
            format!(
                "tools/{} is reachable from no native entry point -- wire it up or remove it",
                native.directory
            )
        })
        .collect()
}

pub fn scanned_nothing(
    crate_count: usize,
    target_count: usize,
    document_count: usize,
) -> Option<String> {
    if crate_count == 0 {
        Some("architecture scanned no Cargo crates under tools -- nothing was checked".into())
    } else if target_count == 0 {
        Some("architecture found no native dispatch entries -- nothing was checked".into())
    } else if document_count == 0 {
        Some("architecture found no Markdown files -- nothing was compared".into())
    } else {
        None
    }
}

const REQUIRED_CHECKS: &[&str] = &[
    "native_registry_is_nonempty",
    "explicit_target_resolves",
    "workspace_target_resolves",
    "bad_target_is_rejected",
    "valid_documentation_target_is_accepted",
    "bad_documentation_path_is_rejected",
    "path_dependency_is_reachable",
    "orphan_crate_is_reported",
    "empty_scan_is_rejected",
];

pub fn self_test() -> Result<(), String> {
    let mut ran = BTreeSet::new();
    let mut check = |name: &'static str, ok: bool, why: &str| -> Result<(), String> {
        ran.insert(name);
        if ok {
            Ok(())
        } else {
            Err(format!("{name}: {why}"))
        }
    };
    let crates = vec![
        NativeCrate {
            directory: "alpha".into(),
            package: "alpha".into(),
            binaries: vec!["alpha".into()],
            path_dependencies: vec!["shared".into()],
            invalid_paths: Vec::new(),
        },
        NativeCrate {
            directory: "shared".into(),
            package: "shared".into(),
            binaries: Vec::new(),
            path_dependencies: Vec::new(),
            invalid_paths: Vec::new(),
        },
        NativeCrate {
            directory: "orphan".into(),
            package: "orphan".into(),
            binaries: vec!["orphan".into()],
            path_dependencies: Vec::new(),
            invalid_paths: Vec::new(),
        },
    ];
    check(
        "native_registry_is_nonempty",
        !dispatch_targets().is_empty(),
        "registry is empty",
    )?;
    check(
        "explicit_target_resolves",
        target_crate("tools/alpha/target/release/alpha", &crates)
            .ok()
            .as_deref()
            == Some("alpha"),
        "an explicit crate target must resolve",
    )?;
    check(
        "workspace_target_resolves",
        target_crate("tools/target/release/alpha", &crates)
            .ok()
            .as_deref()
            == Some("alpha"),
        "a workspace target must resolve by binary name",
    )?;
    check(
        "bad_target_is_rejected",
        target_crate("tools/alpha/debug/alpha", &crates).is_err(),
        "a non-release target must fail",
    )?;
    let text = vec![("AGENTS.md".into(), "tools/missing\n".into())];
    let valid_text = vec![(
        "AGENTS.md".into(),
        "tools/alpha/target/release/alpha\n".into(),
    )];
    check(
        "valid_documentation_target_is_accepted",
        valid_paths(Path::new("/repo"), &valid_text, &crates).is_empty(),
        "a valid crate target path must pass",
    )?;
    check(
        "bad_documentation_path_is_rejected",
        valid_paths(Path::new("/repo"), &text, &crates).len() == 1,
        "a dead native path must fail",
    )?;
    let mut roots = BTreeSet::from(["alpha".to_string()]);
    check(
        "path_dependency_is_reachable",
        unreachable_crates(&crates[..2], &roots).is_empty(),
        "path dependencies must be traversed",
    )?;
    check(
        "orphan_crate_is_reported",
        unreachable_crates(&crates, &roots)
            .iter()
            .any(|line| line.contains("orphan")),
        "an unrooted crate must fail",
    )?;
    roots.clear();
    check(
        "empty_scan_is_rejected",
        scanned_nothing(0, 1, 1).is_some()
            && scanned_nothing(1, 0, 1).is_some()
            && scanned_nothing(1, 1, 0).is_some(),
        "empty scans must fail",
    )?;
    let missing: Vec<_> = REQUIRED_CHECKS
        .iter()
        .copied()
        .filter(|name| !ran.contains(name))
        .collect();
    if missing.is_empty() {
        Ok(())
    } else {
        Err(format!("self-test is incomplete: {}", missing.join(", ")))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn native_architecture_self_test_passes() {
        self_test().expect("architecture self-test");
    }

    #[test]
    fn dispatch_registry_rejects_unregistered_entries() {
        let targets = dispatch_targets();
        let (_, problems) = valid_dispatch_targets(
            &targets,
            &[NativeCrate {
                directory: "architecture".into(),
                package: "architecture".into(),
                binaries: vec!["architecture".into()],
                path_dependencies: Vec::new(),
                invalid_paths: Vec::new(),
            }],
        );
        assert!(!targets.is_empty());
        assert!(problems
            .iter()
            .any(|problem| problem.contains("does not name one native binary")));
    }

    #[test]
    fn manifest_paths_are_extracted_without_legacy_tree_names() {
        assert_eq!(
            native_paths("run tools/dispatch/target/release/dispatch and tools/architecture"),
            vec![
                "tools/dispatch/target/release/dispatch".to_string(),
                "tools/architecture".to_string(),
            ]
        );
    }

    #[test]
    fn retired_language_paths_are_not_native_architecture_edges() {
        assert_eq!(
            native_paths("historical tools/overlay_driver.ts; live tools/overlay-driver"),
            vec!["tools/overlay-driver".to_string()]
        );
    }

    #[test]
    fn authoritative_sources_use_makefile_and_hooks_without_package_json() {
        let repository = Path::new(env!("CARGO_MANIFEST_DIR")).join("../..");
        let sources = authoritative_sources(&repository, &[]);
        let names: BTreeSet<&str> = sources.iter().map(|(name, _)| name.as_str()).collect();

        assert!(names.contains("Makefile"));
        assert!(names.contains(".hooks/pre-commit"));
        assert!(names.contains(".hooks/pre-push"));
        assert!(names.contains(".hooks/commit-msg"));
        assert!(!names.contains("package.json"));

        let makefile = sources
            .iter()
            .find(|(name, _)| name == "Makefile")
            .map(|(_, text)| text)
            .expect("root Makefile is authoritative");
        assert!(native_paths(makefile)
            .iter()
            .any(|path| path == "tools/alchemy-lints/Cargo.toml"));
    }

    #[test]
    fn native_subprocess_edges_root_the_kind2_worker() {
        let repository = Path::new(env!("CARGO_MANIFEST_DIR")).join("../..");
        let crates = scan_crates(&repository).expect("native crates");
        assert!(native_command_roots(&repository, &crates).contains("kind2-resources"));
    }
}
