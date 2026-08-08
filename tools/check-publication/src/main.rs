//! check-publication --- port of `tools/check/check_publication.ts`.
//!
//!   check-publication --staged      # gate the index (pre-commit)
//!   check-publication --pre-push    # gate outgoing history (reads stdin)
//!   check-publication --self-test
//!
//! Exit codes: 0 clean, 1 any rejection, git failure, bad usage, or a scan that
//! looked at nothing.
//!
//! PORT NOTE: on failure bun prints an uncaught-error stack trace around the
//! message; this binary prints `error: <message>` on stderr and nothing else.
//! The message text itself is identical.

use std::io::Read;
use std::path::Path;
use std::process::ExitCode;

use check_publication::{
    check_push, check_staged, commit_message_reason, conflict_marker_reason,
    publication_content_reason, publication_entry_reason, publication_path_reason, ACCEPTED_PATHS,
    REJECTED_PATHS,
};

const USAGE: &str = "usage: check_publication.ts {--staged|--pre-push|--self-test}";

fn root() -> &'static Path {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
}

fn fail(message: &str) -> ExitCode {
    eprintln!("error: {message}");
    ExitCode::from(1)
}

/// The gate's own load-bearing invariants, kept runnable outside `cargo test`
/// so a deployed binary can prove itself. `cargo test` covers far more.
fn self_test() -> ExitCode {
    // A gate that checked nothing would print `self-test=ok` and mean nothing.
    if REJECTED_PATHS.is_empty() || ACCEPTED_PATHS.is_empty() {
        return fail("self-test scanned nothing");
    }
    for path in REJECTED_PATHS {
        if publication_path_reason(path).is_none() {
            return fail(&format!("private path accepted: {path}"));
        }
    }
    for path in ACCEPTED_PATHS {
        if let Some(reason) = publication_path_reason(path) {
            return fail(&format!("source path rejected: {path}: {reason}"));
        }
    }
    let mut rom = vec![0u8; 0x8000];
    rom[0xb2] = 0x96;
    let mut sum: u8 = 0;
    for byte in &rom[0xa0..=0xbc] {
        sum = sum.wrapping_add(*byte);
    }
    rom[0xbd] = 0u8.wrapping_sub(sum).wrapping_sub(0x19);

    let checks: Vec<(&str, bool)> = vec![
        (
            "disguised GBA ROM was accepted",
            publication_content_reason(&rom) == Some("GBA ROM image"),
        ),
        (
            "disguised ELF was accepted",
            publication_content_reason(&[0x7f, 0x45, 0x4c, 0x46]) == Some("ELF build product"),
        ),
        (
            "disguised archive was accepted",
            publication_content_reason(b"!<arch>\n") == Some("archive or object library"),
        ),
        (
            "ordinary source content was rejected",
            publication_content_reason(b"canonical source").is_none(),
        ),
        (
            "committed incbin payload was accepted",
            publication_entry_reason("asm/08000000.s", b".incbin \"rom.gba\"\n").as_deref()
                == Some("committed incbin payload"),
        ),
        (
            "an opening conflict marker was accepted",
            conflict_marker_reason("HANDOVER.md", b"a\n<<<<<<< HEAD\nb\n").is_some(),
        ),
        (
            "a closing conflict marker was accepted",
            conflict_marker_reason("HANDOVER.md", b"a\n>>>>>>> topic\n").is_some(),
        ),
        (
            "the history-facing gate flagged a conflict marker",
            publication_entry_reason("HANDOVER.md", b"x\n<<<<<<< HEAD\n").is_none(),
        ),
        (
            "a Markdown heading underline was rejected as a conflict marker",
            conflict_marker_reason("HANDOVER.md", b"Title\n=======\n\nbody\n").is_none(),
        ),
        (
            "a marker-like string without the separator was rejected",
            conflict_marker_reason("HANDOVER.md", b"see <<<<<<<HEAD in the output\n").is_none(),
        ),
        (
            "a binary extension was scanned for conflict markers",
            conflict_marker_reason("assets/readme/x.png", b"<<<<<<< HEAD\n").is_none(),
        ),
        (
            "a byte dump in a commit message was accepted",
            commit_message_reason("fixed the header\n\n00 11 22 33 44 55 66 77\n").is_some(),
        ),
    ];
    for (message, ok) in checks {
        if !ok {
            return fail(message);
        }
    }
    // Everything below is prose a real commit message contains, and blocking
    // any of it would push authors around the gate instead of through it.
    for accepted in [
        "Close 12 owners the sweep left open\n",
        "reverts 3d36cfb0aa11bb22cc33dd44ee55ff6677889900\n",
        "resource_39b:e6c span 0x02000e6c..0x02000e78 is not audited\n",
        "the prologue pushes r7 where the reference does not\n",
        "the low halfword ff 00 stayed wrong\n",
    ] {
        if commit_message_reason(accepted).is_some() {
            return fail(&format!(
                "a legitimate commit message was rejected: {}",
                accepted.trim()
            ));
        }
    }
    println!("self-test=ok");
    ExitCode::SUCCESS
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.len() != 1 {
        return fail(USAGE);
    }
    match args[0].as_str() {
        "--staged" => match check_staged(root()) {
            Ok(()) => ExitCode::SUCCESS,
            Err(error) => fail(&error),
        },
        "--pre-push" => {
            let mut updates = String::new();
            if let Err(error) = std::io::stdin().read_to_string(&mut updates) {
                return fail(&format!("pre-push stdin failed: {error}"));
            }
            match check_push(root(), &updates) {
                Ok(()) => ExitCode::SUCCESS,
                Err(failure) => {
                    for line in &failure.message_failures {
                        eprintln!("{line}");
                    }
                    fail(&failure.error)
                }
            }
        }
        "--self-test" => self_test(),
        _ => fail(USAGE),
    }
}
