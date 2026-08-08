// The three external toolchain calls this tool cannot do itself: a temporary
// directory, `arm-none-eabi-objdump`, and the as/ld/objcopy pipeline that
// `assembleOverlay` runs.
//
// WHY not reimplement them: the whole point of `linkedExact` is that the
// generated assembly is checked against the ROM by the SAME assembler the
// build uses. A hand-rolled encoder here would be checking our own arithmetic
// against itself, which is exactly the confidence this tool must not have.

use std::path::{Path, PathBuf};
use std::process::Command;

/// `mkdtempSync(join(TMPDIR, prefix))`.
///
/// PORT NOTE: Node fills the six `X` characters from its own CSPRNG; this uses
/// the process id and a monotonically increasing counter seeded from the clock.
/// The name is never observed -- it lives inside a temp directory that is
/// removed in the same function that creates it -- only its uniqueness matters.
pub fn mkdtemp(prefix: &str) -> Result<PathBuf, String> {
    use std::sync::atomic::{AtomicU64, Ordering};
    static COUNTER: AtomicU64 = AtomicU64::new(0);
    let root = std::env::var("TMPDIR")
        .or_else(|_| std::env::var("TMP"))
        .or_else(|_| std::env::var("TEMP"))
        .unwrap_or_else(|_| "/tmp".to_string());
    let nanos = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map(|delta| delta.subsec_nanos() as u64)
        .unwrap_or(0);
    for _ in 0..64 {
        let tag = COUNTER.fetch_add(1, Ordering::SeqCst)
            ^ nanos
            ^ (u64::from(std::process::id()) << 20);
        let path = Path::new(&root).join(format!("{prefix}{tag:012x}"));
        match std::fs::create_dir(&path) {
            Ok(()) => return Ok(path),
            Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => continue,
            Err(error) => return Err(format!("{}: {error}", path.display())),
        }
    }
    Err(format!("could not create a temporary directory under {root}"))
}

struct Scratch(PathBuf);

impl Drop for Scratch {
    fn drop(&mut self) {
        // `rmSync(work, { recursive: true, force: true })` in the TypeScript's
        // `finally`; a Drop guard runs it on the error paths too.
        let _ = std::fs::remove_dir_all(&self.0);
    }
}

/// Runs one toolchain program, returning stdout on success and the TRIMMED
/// stderr as the error message, exactly as the TypeScript throws it.
fn run(command: &str, arguments: &[&str]) -> Result<Vec<u8>, String> {
    let output = Command::new(command)
        .args(arguments)
        .output()
        .map_err(|error| format!("{command}: {error}"))?;
    if !output.status.success() {
        // PORT NOTE: `process.stderr.toString()` is UTF-8 with replacement
        // characters for invalid sequences, which is `from_utf8_lossy`. `.trim()`
        // in JS strips the WhiteSpace + LineTerminator set; the assembler's
        // diagnostics only ever end in `\n`, where `str::trim` agrees.
        return Err(String::from_utf8_lossy(&output.stderr).trim().to_string());
    }
    Ok(output.stdout)
}

/// `objdumpRows`' subprocess half: disassemble a raw binary at a given VMA and
/// hand back the tool's stdout.
pub fn objdump(data: &[u8], base: i64, thumb: bool, prefix: &str) -> Result<String, String> {
    let work = Scratch(mkdtemp(prefix)?);
    let binary = work.0.join("source.bin");
    std::fs::write(&binary, data).map_err(|error| format!("{}: {error}", binary.display()))?;
    let vma = format!("--adjust-vma=0x{base:08x}");
    let binary_text = binary.to_string_lossy().into_owned();
    let mut arguments: Vec<&str> = vec!["-D", "-b", "binary", "-marmv4t"];
    if thumb {
        arguments.push("-Mforce-thumb");
    }
    arguments.push(&vma);
    arguments.push(&binary_text);
    let stdout = run("arm-none-eabi-objdump", &arguments)?;
    // PORT NOTE: `process.stdout.toString()` is lossy UTF-8, matching this.
    Ok(String::from_utf8_lossy(&stdout).into_owned())
}

/// `assembleOverlay(source, base)`.
///
/// PORT NOTE: the TypeScript also patches compiled C rows into the image, but
/// only for a `source` that names a file on disk -- `overlayCSources` returns
/// `[]` for any string containing a newline, and every call site here passes
/// generated multi-line assembly. That branch is provably dead for this tool
/// and is not carried over.
pub fn assemble_overlay(source: &str, base: i64) -> Result<Vec<u8>, String> {
    let work = Scratch(mkdtemp("alchemy-overlay-")?);
    let assembly = work.0.join("o.s");
    let object = work.0.join("o.o");
    let elf = work.0.join("o.elf");
    let binary = work.0.join("o.bin");
    std::fs::write(&assembly, source).map_err(|error| format!("{}: {error}", assembly.display()))?;

    let (assembly, object) = (assembly.to_string_lossy(), object.to_string_lossy());
    let (elf, binary) = (elf.to_string_lossy(), binary.to_string_lossy());
    run(
        "arm-none-eabi-as",
        &[
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            &object,
            &assembly,
        ],
    )?;
    run(
        "arm-none-eabi-ld",
        &[&format!("-Ttext=0x{base:08x}"), "-o", &elf, &object],
    )?;
    run(
        "arm-none-eabi-objcopy",
        &["-O", "binary", "-j", ".text", &elf, &binary],
    )?;
    std::fs::read(binary.as_ref() as &str).map_err(|error| format!("{binary}: {error}"))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_temporary_directory_is_fresh_and_removable() {
        let first = mkdtemp("alchemy-gap-test-").unwrap();
        let second = mkdtemp("alchemy-gap-test-").unwrap();
        assert_ne!(first, second);
        assert!(first.is_dir() && second.is_dir());
        std::fs::remove_dir_all(&first).unwrap();
        std::fs::remove_dir_all(&second).unwrap();
    }

    #[test]
    fn the_scratch_guard_removes_its_directory() {
        let path = {
            let scratch = Scratch(mkdtemp("alchemy-gap-test-").unwrap());
            let path = scratch.0.clone();
            std::fs::write(path.join("a"), b"x").unwrap();
            assert!(path.is_dir());
            path
        };
        assert!(!path.exists());
    }

    #[test]
    fn assembling_thumb_reproduces_the_expected_encoding() {
        // `movs r0, #1` is 0x2001 and `bx lr` is 0x4770; the linker base is
        // irrelevant to both, so this pins the pipeline rather than the ROM.
        let built = assemble_overlay(
            ".syntax unified\n.text\n\t.thumb\n\t.global T\n\t.thumb_func\nT:\n\tmovs r0, #1\n\tbx lr\n",
            0x0800_2000,
        )
        .unwrap();
        assert_eq!(built, vec![0x01, 0x20, 0x70, 0x47]);
    }

    #[test]
    fn a_bad_source_reports_the_assembler_diagnostic() {
        let error = assemble_overlay(".syntax unified\n.text\n\tnot_an_instruction\n", 0x0800_0000)
            .unwrap_err();
        assert!(error.contains("Error:"), "unexpected message: {error}");
        assert!(!error.ends_with('\n'), "message was not trimmed: {error:?}");
    }

    #[test]
    fn objdump_disassembles_thumb_at_the_requested_vma() {
        let text = objdump(&[0x01, 0x20], 0x0800_2000, true, "alchemy-gap-test-").unwrap();
        assert!(text.contains("8002000:"), "unexpected listing: {text}");
        assert!(text.contains("movs"), "unexpected listing: {text}");
    }
}
