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
    for args in [
        vec!["--help"],
        vec!["decompile", "--help"],
        vec!["disassemble", "--help"],
        vec!["diff", "--help"],
        vec!["repair", "--help"],
        vec!["inspect", "--help"],
        vec!["inspect", "allocator", "--help"],
        vec!["convert", "--help"],
    ] {
        let output = command(std::path::Path::new("."))
            .args(args)
            .output()
            .unwrap();
        assert!(output.status.success());
        assert!(String::from_utf8_lossy(&output.stdout).contains("usage: psynergy"));
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
        for pair in change.chunks_exact(2) {
            if label != "duplicate flag" {
                if let Some(index) = args.iter().position(|arg| arg == pair[0]) {
                    args[index + 1] = pair[1].into();
                    continue;
                }
            }
            args.extend([pair[0].to_string(), pair[1].to_string()]);
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

#[test]
fn disassembly_uses_explicit_addresses_and_relative_paths() {
    let dir = tempfile::tempdir().unwrap();
    write_image(dir.path());
    let output = command(dir.path())
        .args([
            "disassemble",
            "input.bin",
            "--base",
            "0x1000",
            "--entry",
            "0x1000",
            "--span",
            "6",
            "--out",
            "output.s",
        ])
        .output()
        .unwrap();
    assert!(
        output.status.success(),
        "{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let text = fs::read_to_string(dir.path().join("output.s")).unwrap();
    assert!(text.contains("00001000:") && text.contains("bx"), "{text}");
    assert!(!text.contains("Func_080"));
    assert_eq!(fs::read(dir.path().join("input.bin")).unwrap(), IMAGE);
}

#[test]
fn binary_diff_distinguishes_equality_difference_and_invalid_input() {
    let dir = tempfile::tempdir().unwrap();
    fs::write(dir.path().join("a.bin"), [1, 2, 3]).unwrap();
    fs::write(dir.path().join("b.bin"), [1, 2, 3]).unwrap();
    let equal = command(dir.path())
        .args(["diff", "a.bin", "b.bin"])
        .output()
        .unwrap();
    assert_eq!(equal.status.code(), Some(0));
    assert!(String::from_utf8_lossy(&equal.stdout).contains("identical=true"));
    fs::write(dir.path().join("b.bin"), [1, 2, 3, 4]).unwrap();
    let changed = command(dir.path())
        .args(["diff", "a.bin", "b.bin", "--width", "2"])
        .output()
        .unwrap();
    assert_eq!(changed.status.code(), Some(1));
    let report = String::from_utf8_lossy(&changed.stdout);
    assert!(
        report.contains("differing_units=1") && report.contains("offset=0x2"),
        "{report}"
    );
    let invalid = command(dir.path())
        .args(["diff", "a.bin", "b.bin", "--width", "0"])
        .output()
        .unwrap();
    assert_eq!(invalid.status.code(), Some(2));
    assert!(invalid.stdout.is_empty());
    assert_eq!(fs::read(dir.path().join("a.bin")).unwrap(), [1, 2, 3]);
}

#[test]
fn conversion_runs_without_a_project_and_refuses_overwrite() {
    let dir = tempfile::tempdir().unwrap();
    fs::write(dir.path().join("words.tsv"), "0x1234\n65535\n").unwrap();
    let args = ["convert", "words2bin", "words.tsv", "data.bin"];
    let first = command(dir.path()).args(args).output().unwrap();
    assert!(
        first.status.success(),
        "{}",
        String::from_utf8_lossy(&first.stderr)
    );
    assert_eq!(
        fs::read(dir.path().join("data.bin")).unwrap(),
        [0x34, 0x12, 0xff, 0xff]
    );
    assert!(!command(dir.path())
        .args(args)
        .output()
        .unwrap()
        .status
        .success());
}

#[test]
fn repairs_are_named_bounded_and_never_edit_the_input() {
    let dir = tempfile::tempdir().unwrap();
    let source = "int f(int x, int y) {\n    if (x < y)\n        return 1;\n    return 0;\n}\n";
    fs::write(dir.path().join("input.c"), source).unwrap();
    let args = ["repair", "input.c", "--repair", "mirror-relational-guards"];
    let listed = command(dir.path()).args(args).output().unwrap();
    assert!(
        listed.status.success(),
        "{}",
        String::from_utf8_lossy(&listed.stderr)
    );
    assert!(String::from_utf8_lossy(&listed.stdout).contains("unique_choices=2"));
    let emitted = command(dir.path())
        .args(args)
        .args(["--choice", "1", "--out", "candidate.c"])
        .output()
        .unwrap();
    assert!(
        emitted.status.success(),
        "{}",
        String::from_utf8_lossy(&emitted.stderr)
    );
    assert!(fs::read_to_string(dir.path().join("candidate.c"))
        .unwrap()
        .contains("y > x"));
    assert_eq!(
        fs::read_to_string(dir.path().join("input.c")).unwrap(),
        source
    );
    let invalid = command(dir.path())
        .args(args)
        .args(["--choice", "2", "--out", "invalid.c"])
        .output()
        .unwrap();
    assert!(!invalid.status.success());
    assert!(!dir.path().join("invalid.c").exists());
    let no_plan = command(dir.path())
        .args(["repair", "input.c"])
        .output()
        .unwrap();
    assert!(!no_plan.status.success());
}

#[test]
fn allocator_inspection_reads_only_supplied_dumps() {
    let dir = tempfile::tempdir().unwrap();
    fs::write(dir.path().join("unit.rtl"), "(reg/v:SI 32 [ count ])").unwrap();
    fs::write(
        dir.path().join("unit.lreg"),
        "Register 32 costs: LO_REGS:3 MEM:9\nRegister 32 pref LO_REGS.\n",
    )
    .unwrap();
    fs::write(
        dir.path().join("unit.greg"),
        "regs to allocate: 32\n;; Register 32 in 3.\n",
    )
    .unwrap();
    let result = command(dir.path())
        .args(["inspect", "allocator", "."])
        .output()
        .unwrap();
    assert!(
        result.status.success(),
        "{}",
        String::from_utf8_lossy(&result.stderr)
    );
    let report = String::from_utf8_lossy(&result.stdout);
    assert!(
        report.contains("count") && report.contains("r3") && report.contains("LO_REGS:3"),
        "{report}"
    );
    assert_eq!(fs::read_dir(dir.path()).unwrap().count(), 3);
}

#[test]
fn project_commands_are_not_portable_aliases() {
    let dir = tempfile::tempdir().unwrap();
    for name in [
        "score",
        "match",
        "extract",
        "adopt",
        "build",
        "verify",
        "coverage",
        "check",
        "unit",
        "overlay",
        "cross-edition",
        "font",
        "dashboard",
        "music-debug",
    ] {
        let result = command(dir.path()).args([name, "--help"]).output().unwrap();
        assert_eq!(result.status.code(), Some(2), "{name}");
        assert!(
            String::from_utf8_lossy(&result.stderr).contains("unknown psynergy command"),
            "{name}"
        );
    }
}
