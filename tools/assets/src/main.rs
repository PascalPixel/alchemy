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
    ("documented", "extract documented data from the ROM"),
    ("remaining-survey", "extract remaining survey data from the ROM"),
    ("exact-reading-list", "extract exact reading list data from the ROM"),
    ("f0-archive", "extract f0 archive data from the ROM"),
    ("simple-resources", "extract simple resources data from the ROM"),
    ("kind2-resources", "extract kind2 resources data from the ROM"),
    ("resource-01c", "extract resource 01c data from the ROM"),
    ("resource-byte-canvases", "extract resource byte canvases data from the ROM"),
    ("archive-asset", "extract archive asset data from the ROM"),
    ("skip-sprite-archive", "extract skip sprite archive data from the ROM"),
    ("static-sprite-series", "extract static sprite series data from the ROM"),
    ("localization-font", "extract localization font data from the ROM"),
    ("map-container-components", "extract map container components data from the ROM"),
    ("byte-henkan", "extract byte henkan data from the ROM"),
    ("namae-nyuuryoku", "extract namae nyuuryoku data from the ROM"),
    ("music", "extract music data from the ROM"),
    ("kind1-map-grid", "extract kind1 map grid data from the ROM"),
    ("resource-directory", "extract resource directory data from the ROM"),
    ("late-runtime-residual", "extract late runtime residual data from the ROM"),
    ("gba-header", "gba header"),
    ("early-runtime-data", "early runtime data"),
    ("export-asset", "export asset"),
    ("import-asset", "import asset"),
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
        "gba-header" => gba_header::cli::entry(&rest),
        "early-runtime-data" => early_runtime_data::cli::entry(&rest),
        "export-asset" => export_asset::cli::entry(&rest),
        "import-asset" => import_asset::cli::entry(&rest),
        "documented" => documented::cli::entry(&rest),
        "remaining-survey" => remaining_survey::cli::entry(&rest),
        "exact-reading-list" => exact_reading_list::cli::entry(&rest),
        "f0-archive" => f0_archive::cli::entry(&rest),
        "simple-resources" => simple_resources::cli::entry(&rest),
        "kind2-resources" => kind2_resources::cli::entry(&rest),
        "resource-01c" => resource_01c::cli::entry(&rest),
        "resource-byte-canvases" => resource_byte_canvases::cli::entry(&rest),
        "archive-asset" => archive_asset::cli::entry(&rest),
        "skip-sprite-archive" => skip_sprite_archive::cli::entry(&rest),
        "static-sprite-series" => static_sprite_series::cli::entry(&rest),
        "localization-font" => localization_font::cli::entry(&rest),
        "map-container-components" => map_container_components::cli::entry(&rest),
        "byte-henkan" => byte_henkan::cli::entry(&rest),
        "namae-nyuuryoku" => namae_nyuuryoku::cli::entry(&rest),
        "music" => music::cli::entry(&rest),
        "kind1-map-grid" => kind1_map_grid::cli::entry(&rest),
        "resource-directory" => resource_directory::cli::entry(&rest),
        "late-runtime-residual" => late_runtime_residual::cli::entry(&rest),
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
