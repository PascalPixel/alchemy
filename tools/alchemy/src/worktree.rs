use crate::compiler::routing::root;
use crate::targets::{target_for, TARGET_IDS};
use std::path::{Path, PathBuf};
use std::process::Command;

const USAGE: &str = "usage: alchemy worktree MAIN_CHECKOUT\n\
Prepares a new git worktree of this repository before its first build:\n\
initialises the submodules, links roms/ and the installed toolchain\n\
(tools/out/binutils, compiler-runtime, compilers) to MAIN_CHECKOUT's, and\n\
clone-copies MAIN_CHECKOUT's built tools and every edition's build cache\n\
so the first build only redoes what differs. make worktree runs it with\n\
the main checkout's own alchemy.";

const LINKED: [&str; 4] = ["roms", "tools/out/binutils", "tools/out/compiler-runtime", "tools/out/compilers"];

/// The directories copied from the main checkout: the built tools, then each
/// edition's build output.
fn caches() -> Vec<String> {
    let mut caches = vec!["tools/out/cargo-target/release".to_string()];
    caches.extend(TARGET_IDS.iter().map(|id| target_for(*id).output_dir.to_string()));
    caches
}

/// Points `link` at `target`, replacing whatever link or file was there.
fn relink(target: &Path, link: &Path) -> Result<(), String> {
    if link.symlink_metadata().is_ok() {
        std::fs::remove_file(link).map_err(|e| format!("{}: {e}", link.display()))?;
    }
    std::os::unix::fs::symlink(target, link).map_err(|e| format!("{}: {e}", link.display()))
}

fn run_in(dir: &Path, program: &str, args: &[&str]) -> Result<(), String> {
    let status = Command::new(program)
        .args(args)
        .current_dir(dir)
        .status()
        .map_err(|e| format!("{program}: {e}"))?;
    if status.success() {
        Ok(())
    } else {
        Err(format!("{program} {}: {status}", args.join(" ")))
    }
}

fn prepare(here: &Path, main: &Path) -> Result<(), String> {
    run_in(here, "git", &["submodule", "update", "--init", "--recursive", "--quiet"])?;
    for dir in ["tools/out", "out"] {
        std::fs::create_dir_all(here.join(dir)).map_err(|e| format!("{dir}: {e}"))?;
    }
    for path in LINKED {
        relink(&main.join(path), &here.join(path))?;
    }
    for cache in caches() {
        let source = main.join(&cache);
        if !source.is_dir() {
            continue;
        }
        let parent = here.join(&cache).parent().map(Path::to_path_buf).unwrap_or_else(|| here.to_path_buf());
        std::fs::create_dir_all(&parent).map_err(|e| format!("{}: {e}", parent.display()))?;
        // cp -c clones on APFS, so a copy costs no space until it changes.
        let parent_arg = format!("{}/", parent.display());
        run_in(here, "cp", &["-c", "-R", &source.to_string_lossy(), &parent_arg])?;
        drop_maps(&here.join(&cache))?;
    }
    Ok(())
}

/// Removes the copied linker maps under `dir`: they name the main checkout's
/// objects by absolute path, and the link cache would otherwise keep them, so
/// the count would read the main checkout's objects. Without them the first
/// build relinks here.
fn drop_maps(dir: &Path) -> Result<(), String> {
    let Ok(entries) = std::fs::read_dir(dir) else {
        return Ok(());
    };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.is_dir() {
            if path.file_name().is_some_and(|name| name == "overlays") {
                drop_maps(&path)?;
            }
        } else if path.extension().is_some_and(|ext| ext == "map") {
            std::fs::remove_file(&path).map_err(|e| format!("{}: {e}", path.display()))?;
        }
    }
    Ok(())
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let main = match arguments {
        [flag] if flag == "-h" || flag == "--help" => return Err(USAGE.into()),
        [main] => PathBuf::from(main),
        _ => return Err(USAGE.into()),
    };
    let main = std::fs::canonicalize(&main).map_err(|e| format!("{}: {e}", main.display()))?;
    let here = root();
    if here == main {
        return Err("alchemy worktree prepares another worktree; this is the main checkout".into());
    }
    prepare(here, &main)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn caches_are_the_built_tools_and_all_twelve_editions() {
        let caches = caches();
        assert_eq!(caches[0], "tools/out/cargo-target/release");
        assert_eq!(caches.len(), 13);
        for edition in ["tbs-ja", "tbs-en", "tbs-it", "tla-ja", "tla-en", "tla-it"] {
            assert!(caches.contains(&format!("out/{edition}")), "{edition}");
        }
    }

    #[test]
    fn copied_maps_are_dropped_so_the_first_build_relinks() {
        let dir = tempfile::tempdir().unwrap();
        let edition = dir.path().join("out/tbs-en");
        std::fs::create_dir_all(edition.join("overlays")).unwrap();
        for file in ["tbs-en.map", "tbs-en.gba", "overlays/resource_370.map", "overlays/resource_370.lz"] {
            std::fs::write(edition.join(file), "x").unwrap();
        }
        drop_maps(&edition).unwrap();
        assert!(!edition.join("tbs-en.map").exists());
        assert!(!edition.join("overlays/resource_370.map").exists());
        assert!(edition.join("tbs-en.gba").exists());
        assert!(edition.join("overlays/resource_370.lz").exists());
    }

    #[test]
    fn relinking_replaces_an_old_link_or_file() {
        let dir = tempfile::tempdir().unwrap();
        let (a, b, link) = (dir.path().join("a"), dir.path().join("b"), dir.path().join("link"));
        std::fs::create_dir(&a).unwrap();
        std::fs::create_dir(&b).unwrap();
        std::fs::write(&link, "stale").unwrap();
        relink(&a, &link).unwrap();
        assert_eq!(std::fs::read_link(&link).unwrap(), a);
        relink(&b, &link).unwrap();
        assert_eq!(std::fs::read_link(&link).unwrap(), b);
    }
}
