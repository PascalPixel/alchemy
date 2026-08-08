//! CLI for `semantic-owner-scope`, mirroring
//! `tools/semantic/semantic_owner_scope.ts`.
//!
//! See `src/lib.rs` for the PORT NOTES. The only deliberate difference at this
//! layer: the TypeScript lets `throw new Error(msg)` escape `main`, so Bun
//! prints a stack trace and exits 1; this binary prints `error: <msg>` on
//! stderr and exits 1. Stdout and exit codes are identical.

use std::path::{Path, PathBuf};
use std::process::ExitCode;

use semantic_owner_scope::{
    census_declared_closed, group_owners, has_epilogue, is_stem_argument, open_owners,
    overlaps, owners_to_json, render_report, row_facts_from_assembly, AddressRange, Region,
};

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

fn region(address: i64, size: i64, retention: &str) -> Region {
    Region {
        address,
        size,
        source: String::new(),
        kind: "k".into(),
        retention: retention.into(),
    }
}

fn self_test() -> Result<(), String> {
    let code = row_facts_from_assembly(
        "\t.thumb\nRegion_08000000:\n\tpush {r4, lr}\n\tbl sub_08001234\n\tpop {r4, pc}\n",
        &region(0x0800_0000, 16, "split_first"),
    );
    if !code.has_prologue || !code.has_epilogue || code.is_data || code.calls != 1 {
        return Err("code row misread".into());
    }
    if !has_epilogue("\tpop {r1}\n\tbx r1\n") {
        return Err("interworking return missed".into());
    }
    if has_epilogue("\tldr r3, [r4, #0]\n\tbx r3\n") {
        return Err("dispatcher must not close an owner".into());
    }
    let pool = row_facts_from_assembly(
        "\t.thumb\nFragment_08000010:\n\t.inst.n 0x1eec\n\t.inst.n 0x0300\n",
        &region(0x0800_0010, 4, "merge_with_owner"),
    );
    if !pool.is_data || pool.has_prologue {
        return Err("pool row misread".into());
    }
    let decoded_pool = row_facts_from_assembly(
        "\t.thumb\n.L_0800:\n\tlsrs r0, r0, #1\n\tmovs r0, #0\n",
        &region(0x0800_0030, 4, "merge_with_owner"),
    );
    if decoded_pool.is_data {
        return Err("decoded pool must not be isData".into());
    }
    if !decoded_pool.suspected_pool {
        return Err("decoded pool must be suspected".into());
    }
    let real_code = row_facts_from_assembly(
        "\t.thumb\n.L_0800:\n\tldr r0, [r1, #0]\n\tbl sub_08001234\n",
        &region(0x0800_0040, 8, "split_first"),
    );
    if real_code.suspected_pool {
        return Err("real code must not be suspected".into());
    }
    let ldr_pool = row_facts_from_assembly(
        "\t.thumb\n.L_0800:\n\tldr r0, [r4, #4]\n\tmovs r0, r0\n",
        &region(0x0800_0050, 4, "merge_with_owner"),
    );
    if !ldr_pool.suspected_pool {
        return Err("ldr-shaped pool must be suspected".into());
    }
    let mut first = code.clone();
    first.address = 0x0800_0000;
    first.stem = "08000000".into();
    first.has_epilogue = false;
    let mut second = pool.clone();
    second.address = 0x0800_0010;
    second.stem = "08000010".into();
    let mut third = code.clone();
    third.address = 0x0800_0020;
    third.stem = "08000020".into();
    third.has_prologue = false;
    let owners = group_owners(&[first, second, third]);
    if owners.len() != 1 {
        return Err(format!("expected 1 owner, got {}", owners.len()));
    }
    if owners[0].excluded_bytes != 4 {
        return Err("pool not excluded".into());
    }
    if !owners[0].closed {
        return Err("owner should be closed by its epilogue".into());
    }
    if !overlaps(
        0x0800_0000,
        8,
        &AddressRange {
            start: Some(0x0800_0004),
            end: Some(0x0800_000c),
        },
    ) {
        return Err("range overlap missed".into());
    }
    if overlaps(
        0x0800_0000,
        4,
        &AddressRange {
            start: Some(0x0800_0004),
            end: Some(0x0800_0008),
        },
    ) {
        return Err("touching ranges must not overlap".into());
    }
    println!("self-test=ok");
    Ok(())
}

fn run(args: &[String]) -> Result<(), String> {
    if args.iter().any(|a| a == "--self-test") {
        return self_test();
    }
    let root = root();
    let owners = open_owners(&root)?;
    if args.iter().any(|a| a == "--check") {
        if !census_declared_closed(&root)? {
            return Err("main-image semantic census has no reviewed closed declaration".into());
        }
        if !owners.is_empty() {
            let bytes: i64 = owners.iter().map(|owner| owner.executable_bytes).sum();
            return Err(format!(
                "main-image semantic census is open: {} owners, {bytes} bytes",
                owners.len()
            ));
        }
        println!("core_semantic_census=closed owners=0 executable_bytes=0");
        return Ok(());
    }
    if args.iter().any(|a| a == "--json") {
        println!("{}", owners_to_json(&owners));
        return Ok(());
    }
    let wanted = args.iter().find(|a| is_stem_argument(a));
    print!("{}", render_report(&owners, wanted.map(String::as_str)));
    Ok(())
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match run(&args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
