// Repository paths used by the dashboard server.

use std::path::{Path, PathBuf};

/// The repository root containing the dashboard's data and assets.
///
/// A Rust binary has no source-module URL, so this uses `CARGO_MANIFEST_DIR`,
/// which sits two levels below the repository root. The override is test-only
/// state: it lets the path and watcher tests use an isolated fixture tree.
pub fn root() -> PathBuf {
    if let Ok(override_root) = std::env::var("ALCHEMY_DASHBOARD_ROOT") {
        if !override_root.is_empty() {
            return PathBuf::from(override_root);
        }
    }
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("crate lives two levels below ROOT")
        .to_path_buf()
}

pub fn font() -> PathBuf {
    root().join("assets").join("fonts").join("weyard.otf")
}

/// The Rust server entry point.
pub fn source() -> PathBuf {
    root()
        .join("tools")
        .join("dashboard-server")
        .join("src")
        .join("main.rs")
}

/// Native source roots whose Rust files require a server restart when edited.
pub fn native_source_directories_at(repository_root: &Path) -> [PathBuf; 2] {
    [
        repository_root
            .join("tools")
            .join("dashboard-server")
            .join("src"),
        repository_root
            .join("tools")
            .join("coverage-map")
            .join("src"),
    ]
}

fn rust_source_files(directory: &Path, files: &mut Vec<PathBuf>) {
    let Ok(entries) = std::fs::read_dir(directory) else {
        return;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.is_dir() {
            rust_source_files(&path, files);
        } else if path.extension().and_then(|extension| extension.to_str()) == Some("rs") {
            files.push(path);
        }
    }
}

/// Every native dashboard and coverage source file, in stable order.
pub fn restart_files_at(repository_root: &Path) -> Vec<PathBuf> {
    let mut files = Vec::new();
    for directory in native_source_directories_at(repository_root) {
        rust_source_files(&directory, &mut files);
    }
    files.sort();
    files
}

pub fn restart_files() -> Vec<PathBuf> {
    restart_files_at(&root())
}

pub const COVERAGE_DIRECTORIES: [&str; 5] = ["asm", "assets", "metrics", "semantic", "exact"];

pub fn coverage_build_files() -> Vec<PathBuf> {
    coverage_build_files_at(&root())
}

pub fn coverage_build_files_at(repository_root: &Path) -> Vec<PathBuf> {
    vec![
        repository_root
            .join("out")
            .join("full")
            .join("asm")
            .join("manifest.json"),
        repository_root
            .join("out")
            .join("full")
            .join("assets")
            .join("manifest.json"),
    ]
}

/// There are no external page assets; both browser sources are embedded in the
/// native server.
pub fn page_files() -> Vec<PathBuf> {
    page_files_at(&root())
}

pub fn page_files_at(_repository_root: &Path) -> Vec<PathBuf> {
    Vec::new()
}

/// Parse `ALCHEMY_DASHBOARD_PORT`, defaulting to 4649 when it is unset.
///
/// An empty or invalid value binds an ephemeral port. This keeps a malformed
/// development setting from preventing the local dashboard from starting.
pub fn port() -> Option<u16> {
    let raw = match std::env::var("ALCHEMY_DASHBOARD_PORT") {
        Ok(value) => value,
        Err(_) => return Some(4649),
    };
    let number = js_to_number(&raw);
    if number.is_nan() {
        return None;
    }
    if !(0.0..=65535.0).contains(&number) {
        return None;
    }
    Some(number as u16)
}

/// `Number(string)`: trims JS whitespace, an empty result is 0, anything the
/// grammar rejects is NaN.
///
/// PORT NOTE: `str::trim` uses Unicode `White_Space`, which INCLUDES U+0085
/// and EXCLUDES U+FEFF. JS `Number` trims the reverse set. `is_js_space` from
/// the coverage-map crate already models the JS set, so it is used rather than
/// `trim`.
fn js_to_number(raw: &str) -> f64 {
    let text: &str = {
        let start = raw
            .find(|c| !coverage_map::js::is_js_space(c))
            .unwrap_or(raw.len());
        let end = raw
            .rfind(|c| !coverage_map::js::is_js_space(c))
            .map_or(start, |i| {
                i + raw[i..].chars().next().map_or(1, char::len_utf8)
            });
        &raw[start..end]
    };
    if text.is_empty() {
        return 0.0;
    }
    // The dashboard port is an integer in practice; anything else falls to
    // NaN and binds an ephemeral port. Keep the parser small and predictable.
    if text.eq_ignore_ascii_case("nan") || text.eq_ignore_ascii_case("inf") {
        return f64::NAN;
    }
    text.parse::<f64>().unwrap_or(f64::NAN)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn root_reaches_the_repository_and_finds_the_rust_server() {
        assert!(
            source().exists(),
            "the Rust server must be reachable from root()"
        );
        assert!(restart_files()
                .iter()
                .all(|path| path.extension().and_then(|e| e.to_str()) == Some("rs")),
            "restart paths must contain native Rust sources only"
        );
        assert!(
            page_files().is_empty(),
            "browser assets must be embedded in Rust"
        );
    }

    #[test]
    fn number_of_empty_is_zero_not_the_default_port() {
        // `??` is nullish, so "" is kept and `Number("")` is 0. A port that
        // "simplifies" this to `unwrap_or(4649)` fails here.
        assert_eq!(js_to_number(""), 0.0);
        assert_eq!(js_to_number("  "), 0.0);
    }

    #[test]
    fn unparsable_ports_are_nan_and_never_a_number() {
        assert!(js_to_number("abc").is_nan());
        assert!(js_to_number("nan").is_nan());
        // `x.is_nan()` and not `x != x` written as a comparison chain: clippy
        // would rewrite a `!(x > 0.0)` here into `x <= 0.0`, which is FALSE
        // for NaN. Named predicate instead.
    }

    #[test]
    fn js_whitespace_trimming_differs_from_rust_trimming() {
        // U+00A0 is JS whitespace, so `Number("\u{a0}5")` is 5.
        assert_eq!(js_to_number("\u{a0}5"), 5.0);
        // U+0085 is Rust `White_Space` but NOT JS whitespace, so this is NaN.
        assert!(js_to_number("\u{85}5").is_nan());
    }
}
