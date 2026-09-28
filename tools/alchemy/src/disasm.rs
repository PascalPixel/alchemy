use std::process::Command;
pub fn disassemble(
    binary: &str,
    base: u32,
) -> Result<std::collections::BTreeMap<u32, String>, String> {
    let output = Command::new("arm-none-eabi-objdump")
        .args([
            "-D",
            "-b",
            "binary",
            "-m",
            "arm",
            "-M",
            "force-thumb",
            &format!("--adjust-vma=0x{base:x}"),
            binary,
        ])
        .output()
        .map_err(|error| format!("objdump failed: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "objdump failed: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    Ok(psynergy::compare::insns::objdump_rows(
        &String::from_utf8_lossy(&output.stdout),
    ))
}

#[test]
fn objdump_invocation_keeps_thumb_offsets_and_instruction_text() {
    let directory = tempfile::tempdir().unwrap();
    let binary = directory.path().join("thumb.bin");
    std::fs::write(&binary, [0x01, 0x20, 0x70, 0x47]).unwrap();
    for base in [0, 0x08001234] {
        let rows = disassemble(binary.to_str().unwrap(), base).unwrap();
        assert_eq!(rows.keys().copied().collect::<Vec<_>>(), [base, base + 2]);
        assert!(rows[&base].starts_with("movs"));
        assert_eq!(rows[&(base + 2)], "bx\tlr");
    }
}
