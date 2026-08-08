// CLI for the RTL instruction extractor.
//
// `--self-test` mirrors the TypeScript original's `--self-test` flag exactly,
// including the `self-test=ok tool=rtl-insn` line it prints.
//
// PORT NOTE: the TS entrypoint has no mode other than `--self-test`, and with
// no arguments it silently exits 0 having scanned nothing. This binary instead
// accepts dump paths and prints `JSON.stringify(parseInsns(text))` (one line
// per file), which is what the TS/Rust parity comparison consumes, and it
// FAILS with exit 2 when given nothing to do -- a check that can "pass"
// without reading a single byte is not a check. Both divergences are additive:
// no existing behaviour changes.

use std::path::{Path, PathBuf};
use std::process::ExitCode;

use rtl_insn::{callee_symbol, dest_register, json_insns, parse_insns, BinaryOp, Dependency, DependencyKind, InsnKind, RtlExpr};

fn repository_root() -> &'static Path {
    Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap()
}

const SELF_TEST_DUMP: &str = r#"
;; Ready list (t = 4): 19 28
;;		--> scheduling insn <<<28>>> on unit core

(insn 28 15 17 (set (reg:SI 3 r3)
        (plus:SI (reg/v:SI 3 r3)
            (reg/v:SI 2 r2))) 5 {*thumb_addsi3} (nil))

(insn 30 28 32 (set (mem:QI (plus:SI (reg/f:SI 0 r0)
                (const_int 9 [0x9])) [0 S1 A8])
        (reg:SI 3 r3)) 174 {*movqi_insn} (insn_list 28 (insn_list:REG_DEP_ANTI 17 (nil)))
    (expr_list:REG_DEAD (reg:SI 3 r3)
        (nil)))

(call_insn 67 58 71 (parallel [
            (set (reg:SI 0 r0)
                (call (mem:SI (symbol_ref:SI ("Func_02001508")) 0)
                    (const_int 0 [0x0])))
            (use (const_int 0 [0x0]))
            (clobber (reg:SI 14 lr))
        ] ) 245 {*call_value_insn} (nil))
"#;

fn self_test() {
    let insns = parse_insns(SELF_TEST_DUMP);
    assert!(insns.len() == 3, "expected 3 insns, got {}", insns.len());
    let (add, store, call) = (&insns[0], &insns[1], &insns[2]);

    assert!(add.uid == 28.0 && add.kind == InsnKind::Insn, "wrong first insn: {add:?}");
    assert!(
        dest_register(add) == Some(3.0),
        "expected add to write r3, got {:?}",
        dest_register(add)
    );
    assert!(
        matches!(add.set.as_ref().map(|set| &set.src), Some(RtlExpr::Binary { op: BinaryOp::Plus, .. })),
        "expected add src to be a plus"
    );

    assert!(store.uid == 30.0, "wrong second insn uid: {}", store.uid);
    assert!(
        dest_register(store).is_none(),
        "a store's dest is a MEM, dest_register must be None"
    );
    assert!(
        matches!(store.set.as_ref().map(|set| &set.dest), Some(RtlExpr::Mem { .. })),
        "expected store dest to be mem"
    );
    assert!(
        store.dependencies.len() == 2,
        "expected 2 LOG_LINKS, got {:?}",
        store.dependencies
    );
    assert!(
        store.dependencies[0] == Dependency { uid: 28.0, kind: DependencyKind::True },
        "expected an untagged (true) dependency on 28, got {:?}",
        store.dependencies[0]
    );
    assert!(
        store.dependencies[1] == Dependency { uid: 17.0, kind: DependencyKind::Anti },
        "expected a REG_DEP_ANTI dependency on 17, got {:?}",
        store.dependencies[1]
    );
    assert!(
        add.dependencies.is_empty(),
        "expected no dependencies on a (nil) LOG_LINKS, got {:?}",
        add.dependencies
    );

    assert!(call.uid == 67.0 && call.kind == InsnKind::CallInsn, "wrong third insn: {call:?}");
    // A CALL's operand is the memory location holding the callee, so the
    // symbol sits one MEM layer under callTarget.
    assert!(
        callee_symbol(call) == Some("Func_02001508"),
        "expected call target Func_02001508, got {:?}",
        callee_symbol(call)
    );

    println!("self-test=ok tool=rtl-insn");
}

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.iter().any(|argument| argument == "--self-test") {
        self_test();
        return ExitCode::SUCCESS;
    }
    let paths: Vec<&String> = arguments.iter().filter(|argument| !argument.starts_with("--")).collect();
    if paths.is_empty() {
        eprintln!(
            "usage: rtl-insn --self-test | rtl-insn <dump>... (paths relative to {})",
            repository_root().display()
        );
        return ExitCode::from(2);
    }
    for path in paths {
        let candidate = Path::new(path);
        let resolved: PathBuf = if candidate.is_absolute() {
            candidate.to_path_buf()
        } else {
            repository_root().join(candidate)
        };
        // PORT NOTE: lossy, matching Node/Bun's `readFileSync(..., "utf8")`,
        // which substitutes U+FFFD for invalid sequences rather than throwing.
        let bytes = match std::fs::read(&resolved) {
            Ok(bytes) => bytes,
            Err(error) => {
                eprintln!("rtl-insn: {}: {error}", resolved.display());
                return ExitCode::FAILURE;
            }
        };
        let text = String::from_utf8_lossy(&bytes);
        println!("{}", json_insns(&parse_insns(&text)));
    }
    ExitCode::SUCCESS
}
