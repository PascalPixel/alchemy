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
fn retired_portable_entry_points_are_not_aliases() {
    for name in ["decompile", "convert", "diff", "disassemble", "repair"] {
        let output = command().args([name, "--help"]).output().unwrap();
        assert_eq!(output.status.code(), Some(2), "{name}");
        assert!(String::from_utf8_lossy(&output.stderr).contains("unknown alchemy command"));
    }
    let output = command().args(["inspect", "allocator"]).output().unwrap();
    assert!(!output.status.success());
}

#[test]
fn compression_recipe_writers_are_retired() {
    let help = command()
        .args(["build", "assets", "--help"])
        .output()
        .unwrap();
    assert!(!help.status.success());
    assert!(String::from_utf8_lossy(&help.stderr).contains("removed generated catalogs"));
    for flag in ["--compact-plans", "--derive-plans"] {
        assert!(!String::from_utf8_lossy(&help.stdout).contains(flag));
        let output = command()
            .args(["build", "assets", flag, "missing-plan.tsv"])
            .output()
            .unwrap();
        assert!(
            !output.status.success(),
            "{flag} must not generate stored answers"
        );
    }
}

#[test]
fn project_build_stages_remain_discoverable() {
    for stage in ["compilers", "runtime", "native", "rom"] {
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

#[test]
fn retired_catalog_operations_explain_the_source_build_route() {
    for operation in [
        "adopt",
        "unit",
        "overlay",
        "land",
        "score",
        "targets",
        "cross-edition",
    ] {
        let output = command().args([operation, "--help"]).output().unwrap();
        assert!(!output.status.success(), "{operation}");
        let error = String::from_utf8_lossy(&output.stderr);
        assert!(
            error.contains("removed owner/translation-unit catalogs"),
            "{operation}: {error}"
        );
        assert!(error.contains("maintained source"), "{operation}: {error}");
    }
    for stage in ["claimed", "assets", "allocator"] {
        let output = command().args(["build", stage, "--help"]).output().unwrap();
        assert!(!output.status.success(), "{stage}");
        assert!(String::from_utf8_lossy(&output.stderr).contains("removed generated catalogs"));
    }
    for stage in ["full", "asm"] {
        let output = command().args(["build", stage, "--help"]).output().unwrap();
        assert!(!output.status.success(), "{stage}");
        assert!(String::from_utf8_lossy(&output.stderr).contains("build rom links"));
    }
    for check in ["source-build", "owners", "siblings"] {
        let output = command().args(["check", check]).output().unwrap();
        assert!(!output.status.success(), "{check}");
        assert!(String::from_utf8_lossy(&output.stderr).contains("make compare"));
    }
}
