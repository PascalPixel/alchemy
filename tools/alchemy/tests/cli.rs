use std::process::Command;

fn command() -> Command {
    Command::new(env!("CARGO_BIN_EXE_alchemy"))
}

#[test]
fn every_advertised_command_has_help() {
    let output = command().arg("--help").output().unwrap();
    assert!(output.status.success());
    let help = String::from_utf8(output.stdout).unwrap();
    for line in help.lines().skip(1).filter(|line| !line.trim().is_empty()) {
        let name = line.split_whitespace().next().unwrap();
        let output = command().args([name, "--help"]).output().unwrap();
        assert!(
            output.status.success(),
            "{name}: {}",
            String::from_utf8_lossy(&output.stderr)
        );
        assert!(
            String::from_utf8_lossy(&output.stdout).contains("usage:"),
            "{name}"
        );
    }
}

#[test]
fn unknown_command_is_refused() {
    let output = command()
        .args(["unknown-command", "--help"])
        .output()
        .unwrap();
    assert_eq!(output.status.code(), Some(2));
    assert!(String::from_utf8_lossy(&output.stderr).contains("unknown alchemy command"));
}

#[test]
fn project_build_stages_remain_discoverable() {
    for stage in ["compilers", "runtime", "rom"] {
        let output = command().args(["build", stage, "--help"]).output().unwrap();
        assert!(
            output.status.success(),
            "{stage}: {}",
            String::from_utf8_lossy(&output.stderr)
        );
        assert!(
            String::from_utf8_lossy(&output.stdout).contains("usage: alchemy build"),
            "{stage}"
        );
    }
}

#[cfg(target_os = "macos")]
fn worktree_git(root: &std::path::Path, args: &[&str]) {
    let output = Command::new("git")
        .arg("-C")
        .arg(root)
        .args(args)
        .env_remove("GIT_DIR")
        .env_remove("GIT_WORK_TREE")
        .env_remove("GIT_INDEX_FILE")
        .output()
        .unwrap();
    assert!(
        output.status.success(),
        "{}",
        String::from_utf8_lossy(&output.stderr)
    );
}

#[cfg(target_os = "macos")]
fn worktree_fixture() -> (tempfile::TempDir, std::path::PathBuf, std::path::PathBuf) {
    let fixture = tempfile::tempdir().unwrap();
    let primary = fixture.path().join("primary checkout");
    let worktree = fixture.path().join("field calls worktree");
    std::fs::create_dir(&primary).unwrap();
    std::fs::write(primary.join("Makefile"), include_str!("../../../Makefile")).unwrap();
    std::fs::create_dir_all(primary.join("tools/alchemy")).unwrap();
    std::fs::write(
        primary.join("tools/alchemy/worktree.ts"),
        include_str!("../worktree.ts"),
    )
    .unwrap();
    worktree_git(&primary, &["init", "--quiet"]);
    worktree_git(&primary, &["add", "Makefile", "tools/alchemy/worktree.ts"]);
    worktree_git(
        &primary,
        &[
            "-c",
            "user.name=Alchemy fixture",
            "-c",
            "user.email=fixture@example.invalid",
            "-c",
            "commit.gpgsign=false",
            "-c",
            "core.hooksPath=/dev/null",
            "commit",
            "--quiet",
            "-m",
            "synthetic worktree fixture",
        ],
    );
    worktree_git(
        &primary,
        &[
            "worktree",
            "add",
            "--quiet",
            "--detach",
            worktree.to_str().unwrap(),
        ],
    );
    for directory in [
        "roms",
        "tools/out/binutils",
        "tools/out/compiler-runtime",
        "tools/out/compilers",
    ] {
        std::fs::create_dir_all(primary.join(directory)).unwrap();
        std::fs::write(
            primary.join(directory).join("fixture"),
            b"synthetic tool or ROM fixture",
        )
        .unwrap();
    }
    (fixture, primary, worktree)
}

#[cfg(target_os = "macos")]
fn worktree_prepare(root: &std::path::Path) -> std::process::Output {
    Command::new("make")
        .arg("worktree")
        .current_dir(root)
        .env_remove("GIT_DIR")
        .env_remove("GIT_WORK_TREE")
        .env_remove("GIT_INDEX_FILE")
        .output()
        .unwrap()
}

#[cfg(target_os = "macos")]
#[test]
fn worktree_prepares_only_available_edition_caches() {
    let editions = [
        "tbs-ja", "tbs-en", "tbs-de", "tbs-es", "tbs-fr", "tbs-it", "tla-ja", "tla-en", "tla-de",
        "tla-es", "tla-fr", "tla-it",
    ];
    for (case, cached, cargo) in [
        ("full", &editions[..], true),
        ("partial", &editions[..2], false),
        ("empty", &editions[..0], false),
    ] {
        let (_fixture, primary, worktree) = worktree_fixture();
        for edition in cached {
            let object = primary
                .join("out")
                .join(edition)
                .join("obj/scene with spaces.o");
            std::fs::create_dir_all(object.parent().unwrap()).unwrap();
            std::fs::write(object, b"cached object").unwrap();
        }
        for omitted in [
            "reports",
            "verification",
            "recovery",
            "tbs-old",
            "unrelated analysis",
        ] {
            let directory = primary.join("out").join(omitted);
            std::fs::create_dir_all(&directory).unwrap();
            std::fs::write(directory.join("fixture"), b"private analysis").unwrap();
        }
        if cargo {
            let directory = primary.join("tools/out/cargo-target/release");
            std::fs::create_dir_all(&directory).unwrap();
            std::fs::write(directory.join("fixture"), b"cargo cache").unwrap();
        }
        let output = worktree_prepare(&worktree);
        assert!(
            output.status.success(),
            "{case}: {}",
            String::from_utf8_lossy(&output.stderr)
        );
        let mut actual: Vec<_> = std::fs::read_dir(worktree.join("out"))
            .unwrap()
            .map(|entry| entry.unwrap().file_name().into_string().unwrap())
            .collect();
        let mut expected: Vec<_> = cached.iter().map(|edition| edition.to_string()).collect();
        actual.sort();
        expected.sort();
        assert_eq!(actual, expected, "{case}");
        for edition in cached {
            let relative = std::path::Path::new("out")
                .join(edition)
                .join("obj/scene with spaces.o");
            let copied = worktree.join(&relative);
            assert!(std::fs::symlink_metadata(&copied)
                .unwrap()
                .file_type()
                .is_file());
            assert_eq!(std::fs::read(&copied).unwrap(), b"cached object");
            std::fs::write(copied, b"changed worktree object").unwrap();
            assert_eq!(
                std::fs::read(primary.join(relative)).unwrap(),
                b"cached object"
            );
        }
        for linked in [
            "roms",
            "tools/out/binutils",
            "tools/out/compiler-runtime",
            "tools/out/compilers",
        ] {
            let copy = worktree.join(linked);
            assert!(std::fs::symlink_metadata(&copy)
                .unwrap()
                .file_type()
                .is_symlink());
            assert_eq!(
                std::fs::canonicalize(copy).unwrap(),
                std::fs::canonicalize(primary.join(linked)).unwrap()
            );
        }
        let cargo_copy = worktree.join("tools/out/cargo-target");
        assert_eq!(cargo_copy.exists(), cargo);
        if cargo {
            assert!(!std::fs::symlink_metadata(&cargo_copy)
                .unwrap()
                .file_type()
                .is_symlink());
            let copied = cargo_copy.join("release/fixture");
            assert_eq!(std::fs::read(&copied).unwrap(), b"cargo cache");
            std::fs::write(copied, b"changed worktree cargo").unwrap();
            assert_eq!(
                std::fs::read(primary.join("tools/out/cargo-target/release/fixture")).unwrap(),
                b"cargo cache"
            );
        }
    }
}

#[cfg(target_os = "macos")]
#[test]
fn worktree_propagates_cache_copy_failures() {
    for collision in ["out/tbs-ja", "tools/out/cargo-target"] {
        let (_fixture, primary, worktree) = worktree_fixture();
        for cached in ["out/tbs-ja", "tools/out/cargo-target"] {
            std::fs::create_dir_all(primary.join(cached)).unwrap();
            std::fs::write(primary.join(cached).join("fixture"), b"synthetic cache").unwrap();
        }
        let destination = worktree.join(collision);
        std::fs::create_dir_all(destination.parent().unwrap()).unwrap();
        std::fs::write(destination, b"copy must fail here").unwrap();
        let output = worktree_prepare(&worktree);
        assert!(
            !output.status.success(),
            "silently ignored copy failure: {collision}"
        );
        assert_eq!(
            std::fs::read(worktree.join(collision)).unwrap(),
            b"copy must fail here"
        );
    }
}
