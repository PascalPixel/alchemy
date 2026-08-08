//! Routing-table parity gate: `routing_data.rs` against `tools/lib/alchemy_gcc.ts`.
//!
//! RETIREMENT: delete this binary, `src/drift.rs` and the `[[bin]]` entry when
//! `tools/lib/alchemy_gcc.ts` is deleted. There is nothing to compare once one
//! mirror is gone, and a check with only one side left cannot fail.

use std::process::ExitCode;

use alchemy_routing::drift::compare;

fn main() -> ExitCode {
    let report = match compare() {
        Ok(report) => report,
        Err(error) => {
            eprintln!("{error}");
            eprintln!(
                "ROUTING PARITY GATE COULD NOT READ BOTH MIRRORS — this is a FAILURE, not a pass."
            );
            return ExitCode::FAILURE;
        }
    };

    for name in &report.only_ts {
        println!("only-ts  {name}");
    }
    for name in &report.only_rs {
        println!("only-rs  {name}");
    }
    for drift in &report.drifted {
        println!(
            "drifted  {}  ts={} elements  rs={} elements",
            drift.name,
            drift.typescript.len(),
            drift.rust.len()
        );
    }
    println!(
        "tables={} (sets={} maps={}) elements={} distinct={} \
         agreed={} only-ts={} only-rs={} drifted={}",
        report.compared(),
        report.set_tables,
        report.map_tables,
        report.total_elements,
        report.distinct_elements,
        report.agreed.len(),
        report.only_ts.len(),
        report.only_rs.len(),
        report.drifted.len()
    );

    // COMPARING NOTHING IS NOT PASSING. `drifted=0` over an empty table list
    // reads exactly like a clean run, and a check that can pass without looking
    // is the defect it polices. The floors are per category so a parser branch
    // that stops matching names itself instead of hiding behind the total.
    let failures = report.floor_failures();
    if !failures.is_empty() {
        for failure in &failures {
            println!("below floor  {failure}");
        }
        println!(
            "THE SCAN FOUND TOO LITTLE TO BELIEVE — this is a FAILURE, not a pass.\n  \
             Either the parsers stopped matching, or data was deleted; if deleted, \
             lower the floor deliberately in src/drift.rs."
        );
        return ExitCode::FAILURE;
    }
    if !report.in_sync() {
        println!(
            "ROUTING TABLES DRIFTED — this is a FAILURE.\n\
             \x20 tools/lib/alchemy_gcc.ts and tools-rs/alchemy-routing/src/routing_data.rs\n\
             \x20 route the same translation units and must stay element-wise identical,\n\
             \x20 in order: cflags_for_source emits in table order and gcc is later-flag-wins.\n\
             \x20 Fix the mirror that is wrong; do not relax the check."
        );
        return ExitCode::FAILURE;
    }
    ExitCode::SUCCESS
}
