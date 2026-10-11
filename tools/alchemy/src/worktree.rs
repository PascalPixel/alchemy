use crate::compiler::routing::root;
use crate::targets::{target_for, TARGET_IDS};
use std::path::{Path, PathBuf};
use std::process::Command;

const USAGE: &str = "usage: alchemy worktree MAIN_CHECKOUT\n\
Prepares a new git worktree of this repository before its first build:\n\
initialises the submodules, links roms/ and the installed toolchain\n\
(tools/out/binutils, compiler-runtime, compilers) to MAIN_CHECKOUT's, and\n\
copies MAIN_CHECKOUT's built tools and every edition's build cache, as\n\
copy-on-write clones where the file system makes them, so the first build\n\
only redoes what differs. The copied linker maps are moved to this\n\
checkout's path; the RTL dumps of alchemy diff --rtl are left behind.\n\
make worktree runs it with the main checkout's own alchemy.";

/// What a draft's `alchemy diff --rtl` leaves in an edition's output: read
/// once by whoever asked for it, and most of that output's size.
const DUMPS: &str = "obj/drafts/rtl";

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
    for (at, cache) in caches().iter().enumerate() {
        let (source, copy) = (main.join(cache), here.join(cache));
        if !source.is_dir() {
            continue;
        }
        let dumps = source.join(DUMPS);
        copy_tree(&source, &copy, &|path| path == dumps)?;
        // Every cache after the built tools is an edition's output.
        if at > 0 {
            crate::build_rom::rebase_maps(&copy, main, here)?;
        }
    }
    Ok(())
}

/// Copies the tree at `source` to `copy`, over whatever is there, leaving out
/// each directory `skipped` names. A file keeps its permissions and the time
/// it was written, which the caches compare; a link stays a link. The copy
/// is the standard library's, which clones on APFS and asks Linux for a
/// reflink where the file system has them, and copies the bytes otherwise.
fn copy_tree(source: &Path, copy: &Path, skipped: &dyn Fn(&Path) -> bool) -> Result<(), String> {
    std::fs::create_dir_all(copy).map_err(|e| format!("{}: {e}", copy.display()))?;
    let entries = std::fs::read_dir(source).map_err(|e| format!("{}: {e}", source.display()))?;
    for entry in entries {
        let entry = entry.map_err(|e| format!("{}: {e}", source.display()))?;
        let (from, to) = (entry.path(), copy.join(entry.file_name()));
        let shown = |e: std::io::Error| format!("{}: {e}", from.display());
        let kind = entry.file_type().map_err(shown)?;
        if kind.is_dir() {
            if !skipped(&from) {
                copy_tree(&from, &to, skipped)?;
            }
        } else if kind.is_symlink() {
            relink(&std::fs::read_link(&from).map_err(shown)?, &to)?;
        } else {
            // A clone needs the name free; a copy would write through a link.
            if to.symlink_metadata().is_ok() {
                std::fs::remove_file(&to).map_err(|e| format!("{}: {e}", to.display()))?;
            }
            std::fs::copy(&from, &to).map_err(shown)?;
            let written = entry.metadata().and_then(|metadata| metadata.modified()).map_err(shown)?;
            let kept = std::fs::metadata(&to).and_then(|metadata| metadata.modified());
            if kept.is_ok_and(|kept| kept != written) {
                keep_time(&to, written).map_err(|e| format!("{}: {e}", to.display()))?;
            }
        }
    }
    Ok(())
}

/// Sets when `path` was written, also on a file its owner may not write.
fn keep_time(path: &Path, written: std::time::SystemTime) -> std::io::Result<()> {
    let permissions = std::fs::metadata(path)?.permissions();
    if permissions.readonly() {
        use std::os::unix::fs::PermissionsExt;
        std::fs::set_permissions(path, std::fs::Permissions::from_mode(permissions.mode() | 0o200))?;
    }
    let set = std::fs::File::options().write(true).open(path).and_then(|file| file.set_modified(written));
    std::fs::set_permissions(path, permissions)?;
    set
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
    fn a_copied_tree_keeps_times_and_links_and_leaves_the_dumps_behind() {
        let dir = tempfile::tempdir().unwrap();
        let (main, here) = (dir.path().join("main/out/tbs-en"), dir.path().join("here/out/tbs-en"));
        std::fs::create_dir_all(main.join("obj/drafts/rtl")).unwrap();
        std::fs::create_dir_all(here.join("obj")).unwrap();
        std::fs::write(main.join("cache.tsv"), "rows").unwrap();
        std::fs::write(main.join("obj/a.o"), "object").unwrap();
        std::fs::write(main.join("obj/drafts/draft.o"), "draft").unwrap();
        std::fs::write(main.join("obj/drafts/rtl/draft.c.23.sched2"), "dump").unwrap();
        std::os::unix::fs::symlink("/elsewhere/roms/tla-en.gba", main.join("baserom.gba")).unwrap();
        let old = std::time::SystemTime::UNIX_EPOCH + std::time::Duration::from_secs(1_000_000_000);
        keep_time(&main.join("obj/a.o"), old).unwrap();
        // What an earlier run left: a stale object, and a link where a file goes.
        std::fs::write(here.join("obj/a.o"), "a stale and longer object").unwrap();
        std::os::unix::fs::symlink(main.join("cache.tsv"), here.join("cache.tsv")).unwrap();

        let dumps = main.join(DUMPS);
        copy_tree(&main, &here, &|path| path == dumps).unwrap();

        assert_eq!(std::fs::read_to_string(here.join("obj/a.o")).unwrap(), "object");
        assert_eq!(std::fs::metadata(here.join("obj/a.o")).unwrap().modified().unwrap(), old);
        assert_eq!(std::fs::read_to_string(here.join("obj/drafts/draft.o")).unwrap(), "draft");
        assert!(!here.join("obj/drafts/rtl").exists());
        assert!(here.join("cache.tsv").symlink_metadata().unwrap().is_file());
        assert_eq!(std::fs::read_to_string(main.join("cache.tsv")).unwrap(), "rows");
        assert_eq!(
            std::fs::read_link(here.join("baserom.gba")).unwrap(),
            Path::new("/elsewhere/roms/tla-en.gba")
        );
    }

    #[test]
    fn a_read_only_file_keeps_its_time_and_stays_read_only() {
        use std::os::unix::fs::PermissionsExt;
        let dir = tempfile::tempdir().unwrap();
        let file = dir.path().join("file");
        std::fs::write(&file, "x").unwrap();
        std::fs::set_permissions(&file, std::fs::Permissions::from_mode(0o444)).unwrap();
        let old = std::time::SystemTime::UNIX_EPOCH + std::time::Duration::from_secs(1_000_000_000);
        keep_time(&file, old).unwrap();
        let metadata = std::fs::metadata(&file).unwrap();
        assert_eq!(metadata.modified().unwrap(), old);
        assert_eq!(metadata.permissions().mode() & 0o777, 0o444);
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
