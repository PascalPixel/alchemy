//! `assets`: one executable for the archival asset extractors.
//!
//! These tools ran once, produced the 11,636 files committed under `assets/`,
//! and have not been needed since. They are NOT deletable: in a clean-room
//! decompilation the extractor is the provenance record proving an asset was
//! derived from the ROM by a documented transform. They simply do not each need
//! an entry point. See docs/ENTRY-POINTS.md.

use std::process::ExitCode;

const USAGE: &str = "usage: assets <command> [args]\n       assets --list";
const COMMANDS: &[(&str, &str)] = &[
    ("tilemap", "extract tilemap data from the ROM"),
    ("wordstream", "extract wordstream data from the ROM"),
    ("pairtable", "extract pairtable data from the ROM"),
    ("indexed-still", "extract indexed still data from the ROM"),
    ("encounter-data", "extract encounter data data from the ROM"),
    ("character-catalog", "extract character catalog data from the ROM"),
    ("staff-roll", "extract staff roll data from the ROM"),
    ("audio-wave", "extract audio wave data from the ROM"),
];

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        list();
        return ExitCode::from(2);
    };
    if matches!(command, "-h" | "--help" | "--list") {
        println!("{USAGE}\n\ncommands:");
        list();
        return ExitCode::SUCCESS;
    }
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "tilemap" => tilemap::cli::entry(&rest),
        "wordstream" => wordstream::cli::entry(&rest),
        "pairtable" => pairtable::cli::entry(&rest),
        "indexed-still" => indexed_still::cli::entry(&rest),
        "encounter-data" => encounter_data::cli::entry(&rest),
        "character-catalog" => character_catalog::cli::entry(&rest),
        "staff-roll" => staff_roll::cli::entry(&rest),
        "audio-wave" => audio_wave::cli::entry(&rest),
        other => {
            eprintln!("unknown assets command: {other}\n\n{USAGE}");
            list();
            ExitCode::from(2)
        }
    }
}

fn list() {
    for (name, summary) in COMMANDS {
        println!("  {name:<22} {summary}");
    }
}
