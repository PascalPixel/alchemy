use std::fs;
use std::process::Command;

const IMAGE: &[u8] = &[
    0x01, 0x49, 0x08, 0x60, 0x70, 0x47, 0xc0, 0x46, 0x00, 0x10, 0x00, 0x08,
];

fn command(dir: &std::path::Path) -> Command {
    let mut command = Command::new(env!("CARGO_BIN_EXE_psynergy"));
    command.current_dir(dir);
    command
}

fn write_image(dir: &std::path::Path) -> std::path::PathBuf {
    let path = dir.join("input.bin");
    fs::write(&path, IMAGE).unwrap();
    path
}

#[test]
fn decompile_writes_c_to_stdout_without_project_defaults() {
    let dir = tempfile::tempdir().unwrap();
    let input = write_image(dir.path());
    let output = command(dir.path())
        .args([
            "decompile",
            input.to_str().unwrap(),
            "--base",
            "0x08000000",
            "--entry",
            "0x08000000",
            "--span",
            "6",
            "--name",
            "ReadPointer",
        ])
        .output()
        .unwrap();
    assert!(
        output.status.success(),
        "{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let source = String::from_utf8(output.stdout).unwrap();
    assert!(source.contains("ReadPointer"));
    assert!(!source.contains("Golden Sun"));
}

#[test]
fn decompile_writes_file_and_refuses_overwrite() {
    let dir = tempfile::tempdir().unwrap();
    let input = write_image(dir.path());
    let output_path = dir.path().join("candidate.c");
    let args = [
        "decompile",
        input.to_str().unwrap(),
        "--base",
        "0x8000000",
        "--entry",
        "0x8000000",
        "--span",
        "6",
        "--out",
        output_path.to_str().unwrap(),
    ];
    let first = command(dir.path()).args(args).output().unwrap();
    assert!(first.status.success());
    let original = fs::read(&output_path).unwrap();
    let second = command(dir.path()).args(args).output().unwrap();
    assert!(!second.status.success());
    assert!(String::from_utf8_lossy(&second.stderr).contains("File exists"));
    assert_eq!(fs::read(output_path).unwrap(), original);
}

#[test]
fn decompile_rejects_invalid_bounds() {
    let dir = tempfile::tempdir().unwrap();
    let input = write_image(dir.path());
    let output = command(dir.path())
        .args([
            "decompile",
            input.to_str().unwrap(),
            "--base",
            "0x8000001",
            "--entry",
            "0x8000000",
            "--span",
            "6",
        ])
        .output()
        .unwrap();
    assert!(!output.status.success());
    assert!(String::from_utf8_lossy(&output.stderr).contains("halfword-aligned"));
}

#[test]
fn help_is_available_at_both_entry_points() {
    for args in [vec!["--help"], vec!["decompile", "--help"]] {
        let output = command(std::path::Path::new("."))
            .args(args)
            .output()
            .unwrap();
        assert!(output.status.success());
        assert!(String::from_utf8_lossy(&output.stdout).contains("psynergy decompile"));
    }
}

#[test]
fn decompile_rejects_invalid_options_without_creating_output() {
    let cases = [
        ("odd span", vec!["--span", "5"]),
        ("zero span", vec!["--span", "0"]),
        (
            "entry before base",
            vec!["--base", "0x8000002", "--entry", "0x8000000"],
        ),
        ("out of bounds", vec!["--span", "14"]),
        ("invalid name", vec!["--name", "return"]),
        ("duplicate flag", vec!["--span", "6", "--span", "6"]),
    ];
    for (label, change) in cases {
        let dir = tempfile::tempdir().unwrap();
        let input = write_image(dir.path());
        let output_path = dir.path().join("should-not-exist.c");
        let mut args = vec![
            "decompile".to_string(),
            input.to_str().unwrap().to_string(),
            "--base".to_string(),
            "0x8000000".to_string(),
            "--entry".to_string(),
            "0x8000000".to_string(),
            "--span".to_string(),
            "6".to_string(),
            "--out".to_string(),
            output_path.to_str().unwrap().to_string(),
        ];
        for pair in change.chunks(2) {
            if pair.len() == 2 {
                args.extend([pair[0].to_string(), pair[1].to_string()]);
            }
        }
        if label == "entry before base" {
            args[3] = "0x8000002".into();
            args[5] = "0x8000000".into();
        }
        let output = command(dir.path()).args(&args).output().unwrap();
        assert!(!output.status.success(), "{label}");
        assert!(output.stdout.is_empty(), "{label} wrote stdout");
        assert!(!output_path.exists(), "{label} created output");
    }
}

#[test]
fn decompile_rejects_address_overflow() {
    let dir = tempfile::tempdir().unwrap();
    let input = write_image(dir.path());
    let output = command(dir.path())
        .args([
            "decompile",
            input.to_str().unwrap(),
            "--base",
            "0xffffff00",
            "--entry",
            "0xfffffffe",
            "--span",
            "2",
        ])
        .output()
        .unwrap();
    assert!(!output.status.success());
    assert!(String::from_utf8_lossy(&output.stderr).contains("overflows"));
}
