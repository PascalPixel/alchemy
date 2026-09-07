//! Asset commands share one declaration for their help and executable route.
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy assets <command> [args]\n       assets --list";

macro_rules! asset_commands {
    ($($name:literal: $summary:literal => $run:expr),* $(,)?) => {
        const COMMANDS: &[(&str, &str)] = &[$(($name, $summary)),*];
        fn dispatch(command: &str, args: &[String]) -> Option<ExitCode> {
            Some(match command {
                $($name => ($run)(args),)*
                _ => return None,
            })
        }
    };
}

asset_commands! {
    "wordstream": "extract wordstream data from the ROM" =>
        wordstream::cli::entry,
    "pairtable": "extract pairtable data from the ROM" =>
        pairtable::cli::entry,
    "indexed-still": "extract indexed still data from the ROM" =>
        indexed_still::cli::entry,
    "encounter-data": "extract encounter data data from the ROM" =>
        encounter_data::cli::entry,
    "character-catalog": "extract character catalog data from the ROM" =>
        character_catalog::cli::entry,
    "staff-roll": "extract staff roll data from the ROM" =>
        staff_roll::cli::entry,
    "audio-wave": "extract audio wave data from the ROM" =>
        audio_wave::cli::entry,
    "audio-engine-data": "build or extract Golden Sun audio-engine data" =>
        |args: &[String]| report_message(audio_engine_data::run(args.to_vec())),
    "localization-tables": "build the localization tables" =>
        |args| { localization_tables::cli::entry(args); ExitCode::SUCCESS },
    "byte-value-regions": "build byte-value regions" =>
        |args| { byte_value_regions::entrypoint::entry(args); ExitCode::SUCCESS },
    "executable-gap-sources": "build executable-gap sources" =>
        executable_gap_sources::cli::entry,
    "music-residuals": "build music residuals" =>
        music_residuals::cli::entry,
    "f0-archive": "extract f0 archive data from the ROM" =>
        f0_archive::cli::entry,
    "simple-resources": "extract simple resources data from the ROM" =>
        simple_resources::cli::entry,
    "kind2-resources": "extract kind2 resources data from the ROM" =>
        kind2_resources::cli::entry,
    "skip-sprite-archive": "extract skip sprite archive data from the ROM" =>
        skip_sprite_archive::cli::entry,
    "static-sprite-series": "extract static sprite series data from the ROM" =>
        static_sprite_series::cli::entry,
    "localization-font": "extract localization font data from the ROM" =>
        localization_font::cli::entry,
    "map-container-components": "extract map container components data from the ROM" =>
        map_container_components::cli::entry,
    "byte-henkan": "extract byte henkan data from the ROM" =>
        byte_henkan::cli::entry,
    "namae-nyuuryoku": "extract namae nyuuryoku data from the ROM" =>
        namae_nyuuryoku::cli::entry,
    "music": "extract music data from the ROM" =>
        music::cli::entry,
    "kind1-map-grid": "extract kind1 map grid data from the ROM" =>
        kind1_map_grid::cli::entry,
    "resource-directory": "extract resource directory data from the ROM" =>
        resource_directory::cli::entry,
    "late-runtime-residual": "extract late runtime residual data from the ROM" =>
        late_runtime_residual::cli::entry,
    "early-runtime-data": "early runtime data" =>
        early_runtime_data::cli::entry,
    "message-archive": "message archive" =>
        |args| { message_archive::cli::entry(args); ExitCode::SUCCESS },
    "5": "decode resource 5" =>
        |args: &[String]| report(resource_5::run(args.to_vec())),
    "title": "decode title-screen resources" =>
        |args: &[String]| report(title_resources::run(args.to_vec())),
    "sentou": "decode battle (sentou) resources" =>
        |args| report(sentou_resources::run(args)),
    "map-tokushu": "decode special (tokushu) map resources" =>
        |args| { map_resources::entry_tokushu::entry(args); ExitCode::SUCCESS },
    "map-chiiki": "decode regional (chiiki) map resources" =>
        |args| { map_resources::entry_chiiki::entry(args); ExitCode::SUCCESS },
    "battle-effect": "build the battle-effect data package" =>
        |args: &[String]| report(battle_effect_data::run(args.to_vec())),
    "runtime-support": "build the runtime support data package" =>
        |args: &[String]| report(runtime_support_data::run(args.to_vec())),
    "battle-runtime": "build and verify the battle-effect runtime (sentou kouka)" =>
        |args: &[String]| report(sentou_kouka_runtime::run(args.to_vec())),
    "battle-menu": "build the battle menu data (sentou menu)" =>
        sentou_menu_data::cli::entry,
    "battle-screen": "build the battle screen data (sentou gamen)" =>
        sentou_gamen_data::cli::entry,
    "battle-display": "build the battle display data (sentou hyouji)" =>
        sentou_hyouji::cli::entry,
}

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
    report(result.map(|message| {
        if let Some(message) = message {
            println!("{message}");
        }
    }))
}

pub fn entry(args: &[String]) -> ExitCode {
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
    dispatch(command, &args[1..]).unwrap_or_else(|| {
        eprintln!("unknown assets command: {command}\n\n{USAGE}");
        list();
        ExitCode::from(2)
    })
}

fn list() {
    for (name, summary) in COMMANDS {
        println!("  {name:<22} {summary}");
    }
}

#[test]
fn listed_asset_commands_are_unique_and_unknown_commands_are_rejected() {
    let names: std::collections::BTreeSet<_> = COMMANDS.iter().map(|(name, _)| *name).collect();
    assert_eq!(names.len(), COMMANDS.len());
    assert!(dispatch("absent", &[]).is_none());
}
