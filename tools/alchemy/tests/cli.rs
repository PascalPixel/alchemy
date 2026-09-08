use std::process::Command;

fn command() -> Command {
    Command::new(env!("CARGO_BIN_EXE_alchemy"))
}

#[test]
fn every_advertised_command_has_help_and_documented_ownership() {
    let output = command().arg("--help").output().unwrap();
    assert!(output.status.success());
    let help = String::from_utf8(output.stdout).unwrap();
    let contributing = include_str!("../../../CONTRIBUTING.md");
    for line in help.lines().filter(|line| line.starts_with("  ")) {
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
        assert!(
            contributing.contains(&format!("| `alchemy {name}` |")),
            "{name} missing from inventory"
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
fn project_build_stages_remain_discoverable() {
    for stage in [
        "compilers",
        "asm",
        "claimed",
        "full",
        "rom",
        "assets",
        "allocator",
    ] {
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
fn overlay_help_never_treats_flags_as_resource_names() {
    for operation in ["adopt", "park", "audit"] {
        for flag in ["--help", "-h"] {
            let output = command()
                .args(["overlay", operation, flag])
                .output()
                .unwrap();
            assert!(
                output.status.success(),
                "{operation}: {}",
                String::from_utf8_lossy(&output.stderr)
            );
            assert!(String::from_utf8_lossy(&output.stdout).contains("usage: alchemy overlay"));
        }
    }
}
