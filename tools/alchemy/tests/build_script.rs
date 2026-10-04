#[path = "../build.rs"]
mod build_script;

use std::{fs, path::Path, process::Command};

fn workspace(root: &Path) {
    for name in ["alchemy", "psynergy", "ags"] {
        let directory = root.join("tools").join(name);
        fs::create_dir_all(directory.join("src")).unwrap();
        fs::write(directory.join("src/lib.rs"), "pub fn producer() {}\n").unwrap();
        fs::write(directory.join("Cargo.toml"), format!("name = \"{name}\"\n")).unwrap();
    }
    fs::write(root.join("tools/Cargo.lock"), "version = 4\n").unwrap();
}

fn signature(root: &Path) -> String {
    let output = Command::new(std::env::current_exe().unwrap())
        .args([
            "--exact",
            "reused_build_script_reads_the_current_checkout",
            "--nocapture",
        ])
        .env("ALCHEMY_SIGNATURE_TEST_CHILD", "1")
        .env("CARGO_MANIFEST_DIR", root.join("tools/alchemy"))
        .output()
        .unwrap();
    assert!(
        output.status.success(),
        "{}",
        String::from_utf8_lossy(&output.stderr)
    );
    let output = String::from_utf8(output.stdout).unwrap();
    assert!(
        output.contains(&format!(
            "cargo:rerun-if-changed={}",
            root.join("tools/alchemy/src").display()
        )),
        "the reused script read its original checkout instead of Cargo's current one: {output}"
    );
    output
        .lines()
        .find_map(|line| line.strip_prefix("cargo:rustc-env=ALCHEMY_BUILD_IMPLEMENTATION="))
        .unwrap()
        .to_owned()
}

#[test]
fn reused_build_script_reads_the_current_checkout() {
    if std::env::var_os("ALCHEMY_SIGNATURE_TEST_CHILD").is_some() {
        build_script::main();
        return;
    }
    let original = tempfile::tempdir().unwrap();
    let moved = tempfile::tempdir().unwrap();
    workspace(original.path());
    workspace(moved.path());
    let expected = signature(original.path());
    original.close().unwrap();
    assert_eq!(signature(moved.path()), expected);
    fs::write(
        moved.path().join("tools/ags/src/lib.rs"),
        "pub fn changed() {}\n",
    )
    .unwrap();
    assert_ne!(signature(moved.path()), expected);
}
