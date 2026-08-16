//! `data`: one executable for the runtime-data builders.
//!
//! Third consolidation, after `overlay` and `resource`. See `tools/overlay` for
//! why: 115 loose executables that nothing could enumerate meant agents kept
//! rebuilding work that already existed.

use std::process::ExitCode;

const USAGE: &str = "usage: data <command> [args]\n       data --list";

const COMMANDS: &[(&str, &str)] = &[
    ("audio-engine", "build the audio engine data package"),
    ("battle-effect", "build the battle-effect data package"),
    ("runtime-support", "build the runtime support data package"),
    ("battle-runtime", "build and verify the battle-effect runtime (sentou kouka)"),
    ("battle-menu", "build the battle menu data (sentou menu)"),
    ("battle-screen", "build the battle screen data (sentou gamen)"),
    ("battle-display", "build the battle display data (sentou hyouji)"),
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
        "audio-engine" => match audio_engine_data::run(rest) {
            Ok(Some(message)) => {
                println!("{message}");
                ExitCode::SUCCESS
            }
            Ok(None) => ExitCode::SUCCESS,
            Err(error) => {
                eprintln!("error: {error}");
                ExitCode::FAILURE
            }
        },
        "battle-effect" => report(battle_effect_data::run(rest)),
        "runtime-support" => report(runtime_support_data::run(rest)),
        "battle-runtime" => report(sentou_kouka_runtime::run(rest)),
        "battle-menu" => sentou_menu_data::cli::entry(&rest),
        "battle-screen" => sentou_gamen_data::cli::entry(&rest),
        "battle-display" => sentou_hyouji::cli::entry(&rest),
        other => {
            eprintln!("unknown data command: {other}\n\n{USAGE}");
            list();
            ExitCode::from(2)
        }
    }
}

fn list() {
    for (name, summary) in COMMANDS {
        println!("  {name:<18} {summary}");
    }
}
