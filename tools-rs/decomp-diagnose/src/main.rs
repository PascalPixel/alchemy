// CLI for the exact-C candidate diagnostics, ported from
// tools/decomp/decomp_diagnose.ts.
//
// WHY THIS FILE EXISTS
// Someone staring at "41 bytes differ" needs the mismatch broken into classes
// before deciding whether to sweep compiler flags or rewrite the C. This binary
// compiles the candidate, disassembles both sides, and prints that breakdown as
// the same JSON document the TypeScript emits.
//
// PORT NOTE: on failure Bun prints `error: <message>` plus a JS stack trace and
// a version banner. This binary prints `error: <message>` and exits 1; the
// exit code and the message line match, the stack trace does not exist.

use std::path::{Path, PathBuf};

use candidate_compiler::{
    verify_candidate as compile_candidate, CandidateCompilerConfiguration, CompilerTarget,
};

use decomp_diagnose::{
    assembly_sizes, basename, diagnose_instructions, disassemble, lookup, repo_root,
    CandidateDiagnosis, DecodedInstruction, ROM_BASE,
};

fn main() {
    match run() {
        Ok(Some(text)) => println!("{text}"),
        Ok(None) => {}
        Err(message) => {
            eprintln!("error: {message}");
            std::process::exit(1);
        }
    }
}

fn run() -> Result<Option<String>, String> {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.len() == 1 && arguments[0] == "--self-test" {
        self_test()?;
        println!("decomp diagnosis self-test passed");
        return Ok(None);
    }
    let root = repo_root();
    let source = arguments
        .iter()
        .find(|item| item.ends_with(".c"))
        .ok_or("usage: decomp_diagnose.ts --self-test | CANDIDATE.c [ROM]")?;
    let rom_path: PathBuf = arguments
        .iter()
        .find(|item| item.ends_with(".gba"))
        .map(PathBuf::from)
        .unwrap_or_else(|| root.join("roms").join("gs1-en.gba"));
    let scratch = root
        .join("out")
        .join("decomp")
        .join("diagnose")
        .join(basename(source, ".c"));
    let rom = std::fs::read(&rom_path)
        .map_err(|error| format!("ENOENT: {}: {error}", rom_path.display()))?;
    let diagnosis = diagnose_candidate(source, &rom, &scratch, &root)?;
    Ok(Some(diagnosis.to_json()))
}

/// `expectedSize(stem, fallback)` — lazily loaded manifest lookup.
fn expected_size(root: &Path, stem: &str, fallback: i64) -> i64 {
    lookup(&assembly_sizes(root), stem).unwrap_or(fallback)
}

/// `Buffer#subarray` index clamping, including the negative-index rewrite.
fn js_subarray(length: usize, start: i64, end: i64) -> (usize, usize) {
    let resolve = |index: i64| -> usize {
        if index < 0 {
            (length as i64 + index).max(0) as usize
        } else {
            (index as usize).min(length)
        }
    };
    let begin = resolve(start);
    let finish = resolve(end);
    (begin, finish.max(begin))
}

fn diagnose_candidate(
    source: &str,
    rom: &[u8],
    scratch: &Path,
    root: &Path,
) -> Result<CandidateDiagnosis, String> {
    std::fs::create_dir_all(scratch).map_err(|error| error.to_string())?;
    let stem = basename(source, ".c");
    let verification = verify_candidate(source, rom, scratch)?;
    let address = i64::from_str_radix(&stem, 16)
        .map_err(|_| format!("invalid hexadecimal value: {stem}"))?;
    let target_size = expected_size(root, &stem, verification.expected_length);
    let (begin, finish) = js_subarray(
        rom.len(),
        address - ROM_BASE,
        address - ROM_BASE + target_size,
    );
    let target = &rom[begin..finish];
    let actual = disassemble(
        &verification.actual,
        address,
        &scratch.join(format!("{stem}.actual.bin")),
    )?;
    let expected = disassemble(target, address, &scratch.join(format!("{stem}.expected.bin")))?;
    let mut bytes = (verification.actual.len() as i64 - target.len() as i64).abs();
    bytes += verification
        .actual
        .iter()
        .zip(target.iter())
        .filter(|(left, right)| left != right)
        .count() as i64;
    Ok(diagnose_instructions(
        &stem,
        &actual,
        &expected,
        verification.actual.len() as i64,
        target.len() as i64,
        bytes,
    ))
}

struct Verification {
    actual: Vec<u8>,
    expected_length: i64,
}

fn verify_candidate(
    source: &str,
    rom: &[u8],
    scratch: &Path,
) -> Result<Verification, String> {
    let compiled = compile_candidate(
        source,
        rom,
        &scratch.to_string_lossy(),
        &[],
        candidate_compiler::ROM_BASE,
        CompilerTarget::Gs1,
        &CandidateCompilerConfiguration::default(),
    )?;
    Ok(Verification {
        actual: compiled.actual,
        expected_length: compiled.expected.len() as i64,
    })
}

fn self_test() -> Result<(), String> {
    let insn = |mnemonic: &str, operands: &str, address: i64| DecodedInstruction {
        address,
        size: 2,
        mnemonic: mnemonic.to_string(),
        operands: operands.to_string(),
    };
    let expected = vec![
        insn("ldr", "r3, [r0]", 0),
        insn("str", "r2, [r1]", 2),
        insn("bx", "lr", 4),
    ];
    let registers = vec![
        insn("ldr", "r2, [r0]", 0),
        insn("str", "r3, [r1]", 2),
        insn("bx", "lr", 4),
    ];
    let first = diagnose_instructions("test", &registers, &expected, 0, 0, 0);
    if first.count(decomp_diagnose::MismatchKind::RegisterOnly) != 2
        || first.dominant != "register_only"
    {
        return Err("register diagnosis self-test failed".to_string());
    }
    let mut inserted = vec![insn("mov", "r3, r2", 0)];
    for item in &expected {
        let mut shifted = item.clone();
        shifted.address += 2;
        inserted.push(shifted);
    }
    let second = diagnose_instructions("test", &inserted, &expected, 0, 0, 0);
    if second.count(decomp_diagnose::MismatchKind::ExtraInstruction) != 1 {
        return Err("alignment diagnosis self-test failed".to_string());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_self_test_gate_actually_asserts() {
        assert!(self_test().is_ok());
    }

    /// A gate that cannot fail is not a gate. `--self-test` passing proves
    /// nothing on its own, so this drives the exact predicates it checks with
    /// inputs that must break them, proving each one discriminates.
    #[test]
    fn the_self_test_predicates_can_fail() {
        let insn = |mnemonic: &str, operands: &str, address: i64| DecodedInstruction {
            address,
            size: 2,
            mnemonic: mnemonic.to_string(),
            operands: operands.to_string(),
        };
        let expected = vec![
            insn("ldr", "r3, [r0]", 0),
            insn("str", "r2, [r1]", 2),
            insn("bx", "lr", 4),
        ];
        // Predicate one: `register_only == 2 && dominant == "register_only"`.
        // Identical listings classify as exact, so both halves must fail.
        let exact = diagnose_instructions("test", &expected, &expected, 0, 0, 0);
        assert_ne!(
            exact.count(decomp_diagnose::MismatchKind::RegisterOnly),
            2,
            "the register predicate would accept an unperturbed listing"
        );
        assert_ne!(exact.dominant, "register_only");
        // Predicate two: `extra_instruction == 1`. Without the extra leading
        // instruction there is nothing extra to charge.
        assert_eq!(
            exact.count(decomp_diagnose::MismatchKind::ExtraInstruction),
            0,
            "the alignment predicate would accept a listing with no insertion"
        );
    }

    #[test]
    fn subarray_indices_clamp_and_accept_negatives_like_javascript() {
        assert_eq!(js_subarray(10, 2, 5), (2, 5));
        assert_eq!(js_subarray(10, 8, 40), (8, 10));
        assert_eq!(js_subarray(10, -3, 40), (7, 10));
        assert_eq!(js_subarray(10, -30, 2), (0, 2));
        assert_eq!(js_subarray(10, 6, 3), (6, 6));
    }

    #[test]
    fn expected_size_falls_back_when_no_manifest_names_the_stem() {
        let root = repo_root();
        assert_eq!(expected_size(&root, "not-a-real-stem", 99), 99);
    }
}
