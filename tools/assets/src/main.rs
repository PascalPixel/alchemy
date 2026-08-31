//! `assets`: one executable for every ROM data and resource extractor.
//!
//! Absorbed the former `resource` and `data` hosts: three binaries for one job
//! was the same sprawl this consolidation exists to remove.
//!
//! These tools ran once, produced the 11,636 files committed under `games/gs1/assets/`,
//! and have not been needed since. They are NOT deletable: in a clean-room
//! decompilation the extractor is the provenance record proving an asset was
//! derived from the ROM by a documented transform. They simply do not each need
//! an entry point. See `CONTRIBUTING.md`'s tooling index.

use std::process::ExitCode;

const USAGE: &str = "usage: assets <command> [args]\n       assets --list";
const COMMANDS: &[(&str, &str)] = &[
    ("wordstream", "extract wordstream data from the ROM"),
    ("pairtable", "extract pairtable data from the ROM"),
    ("indexed-still", "extract indexed still data from the ROM"),
    ("encounter-data", "extract encounter data data from the ROM"),
    (
        "character-catalog",
        "extract character catalog data from the ROM",
    ),
    ("staff-roll", "extract staff roll data from the ROM"),
    ("audio-wave", "extract audio wave data from the ROM"),
    (
        "audio-engine-data",
        "build or extract Golden Sun audio-engine data",
    ),
    ("localization-tables", "build the localization tables"),
    ("byte-value-regions", "build byte-value regions"),
    ("executable-gap-sources", "build executable-gap sources"),
    ("music-residuals", "build music residuals"),
    ("f0-archive", "extract f0 archive data from the ROM"),
    (
        "simple-resources",
        "extract simple resources data from the ROM",
    ),
    (
        "kind2-resources",
        "extract kind2 resources data from the ROM",
    ),
    ("resource-01c", "extract resource 01c data from the ROM"),
    (
        "resource-byte-canvases",
        "extract resource byte canvases data from the ROM",
    ),
    (
        "skip-sprite-archive",
        "extract skip sprite archive data from the ROM",
    ),
    (
        "static-sprite-series",
        "extract static sprite series data from the ROM",
    ),
    (
        "localization-font",
        "extract localization font data from the ROM",
    ),
    (
        "map-container-components",
        "extract map container components data from the ROM",
    ),
    ("byte-henkan", "extract byte henkan data from the ROM"),
    (
        "namae-nyuuryoku",
        "extract namae nyuuryoku data from the ROM",
    ),
    ("music", "extract music data from the ROM"),
    ("kind1-map-grid", "extract kind1 map grid data from the ROM"),
    (
        "resource-directory",
        "extract resource directory data from the ROM",
    ),
    (
        "late-runtime-residual",
        "extract late runtime residual data from the ROM",
    ),
    ("early-runtime-data", "early runtime data"),
    ("message-archive", "message archive"),
    ("3ce", "decode resource 3ce"),
    ("5", "decode resource 5"),
    ("d1-d3", "decode resources d1 through d3"),
    ("title", "decode title-screen resources"),
    ("sentou", "decode battle (sentou) resources"),
    ("map-tokushu", "decode special (tokushu) map resources"),
    ("map-chiiki", "decode regional (chiiki) map resources"),
    ("battle-effect", "build the battle-effect data package"),
    ("runtime-support", "build the runtime support data package"),
    (
        "battle-runtime",
        "build and verify the battle-effect runtime (sentou kouka)",
    ),
    ("battle-menu", "build the battle menu data (sentou menu)"),
    (
        "battle-screen",
        "build the battle screen data (sentou gamen)",
    ),
    (
        "battle-display",
        "build the battle display data (sentou hyouji)",
    ),
];

fn report<E: std::fmt::Display>(result: Result<(), E>) -> ExitCode {
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

fn report_message<E: std::fmt::Display>(result: Result<Option<String>, E>) -> ExitCode {
    match result {
        Ok(Some(message)) => {
            println!("{message}");
            ExitCode::SUCCESS
        }
        Ok(None) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        list();
        return ExitCode::from(2);
    };
    if command == "--self-test" {
        println!("self-test=ok commands={}", COMMANDS.len());
        return ExitCode::SUCCESS;
    }
    if matches!(command, "-h" | "--help" | "--list") {
        println!("{USAGE}\n\ncommands:");
        list();
        return ExitCode::SUCCESS;
    }
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "map-tokushu" => {
            map_resources::entry_tokushu::entry(&rest);
            ExitCode::SUCCESS
        }
        "map-chiiki" => {
            map_resources::entry_chiiki::entry(&rest);
            ExitCode::SUCCESS
        }
        "3ce" => report(resource_3ce::run(rest)),
        "5" => report(resource_5::run(rest)),
        "d1-d3" => report(resource_d1_d3::run(rest)),
        "title" => report(title_resources::run(rest)),
        "sentou" => report(sentou_resources::run(&rest)),

        "battle-effect" => report(battle_effect_data::run(rest)),
        "runtime-support" => report(runtime_support_data::run(rest)),
        "battle-runtime" => report(sentou_kouka_runtime::run(rest)),
        "battle-menu" => sentou_menu_data::cli::entry(&rest),
        "battle-screen" => sentou_gamen_data::cli::entry(&rest),
        "battle-display" => sentou_hyouji::cli::entry(&rest),
        "message-archive" => {
            message_archive::cli::entry(&rest);
            ExitCode::SUCCESS
        }
        "early-runtime-data" => early_runtime_data::cli::entry(&rest),
        "f0-archive" => f0_archive::cli::entry(&rest),
        "simple-resources" => simple_resources::cli::entry(&rest),
        "kind2-resources" => kind2_resources::cli::entry(&rest),
        "resource-01c" => resource_01c::cli::entry(&rest),
        "resource-byte-canvases" => resource_byte_canvases::cli::entry(&rest),
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
        "wordstream" => wordstream::cli::entry(&rest),
        "pairtable" => pairtable::cli::entry(&rest),
        "indexed-still" => indexed_still::cli::entry(&rest),
        "encounter-data" => encounter_data::cli::entry(&rest),
        "character-catalog" => character_catalog::cli::entry(&rest),
        "staff-roll" => staff_roll::cli::entry(&rest),
        "audio-wave" => audio_wave::cli::entry(&rest),
        "audio-engine-data" => report_message(audio_engine_data::run(rest)),
        "localization-tables" => {
            localization_tables::cli::entry(&rest);
            ExitCode::SUCCESS
        }
        "byte-value-regions" => {
            byte_value_regions::entrypoint::entry(&rest);
            ExitCode::SUCCESS
        }
        "executable-gap-sources" => executable_gap_sources::cli::entry(&rest),
        "music-residuals" => music_residuals::cli::entry(&rest),
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
