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
