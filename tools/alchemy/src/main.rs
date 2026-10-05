use std::process::ExitCode;

mod adopt;
mod adopt_into;
mod assembly {
    pub mod addresses;
}
mod bootstrap;
mod build;
mod build_rom;
mod build_text;
mod c_items;
mod cache;
mod check;
mod compiler;
mod coverage;
mod decode;
mod deps;
mod deps_command;
mod diff;
mod edition;
mod elf;
mod gate;
mod integrate;
mod keep;
mod label;
mod ld_map;
mod merge;
mod objects;
mod process;
mod queues;
mod show;
mod similar;
mod similar_command;
mod sizes;
mod targets;
mod thumb;
mod verify;
mod worst;
mod worst_command;

const USAGE: &str = r#"usage: alchemy <command> [args]  (both games, all twelve editions)
Build and verify:
  bootstrap   install or validate the persistent compiler toolchain
  build       build compilers, maintained source, assembly and ROMs
  verify      the gates of make verify, make land and the commit hooks
  check       run repository contract checks
Matching:
  diff        compare a C draft with the listing it would replace
  adopt       take an adopted function out of every edition's scaffolding
  label       label English-only scaffold data in the other editions
  queues      split not-yet-C work into queues that cannot collide
  integrate   merge worker branches, reconcile, build and name what differs
  keep        keep only the changed source that builds byte-identical
  merge-driver  git merge drivers for scaffolds and linker scripts, and
              links, which reconciles linker scripts after a merge
Reports for people (nothing else reads them):
  deps        dependency map of not-yet-C code: what unblocks the most
  similar     each not-yet-C function's nearest C twins in either game
  sizes       a draft's struct sizes, and the types that fill a frame gap
  worst       maintained C ranked by how mechanical it reads
  figure      PROGRESS.png and PROGRESS_CHART.png from the verified count
  objects     linked objects and functions that differ from the reference ROM
  show        a built function beside the reference, instruction by instruction
Run any command with --help for its options."#;

fn main() -> ExitCode {
    compiler::routing::prefer_installed_binutils();
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "bootstrap" => result(bootstrap::run(rest)),
        "build" => build::entry(rest),
        "verify" => verify::entry(rest),
        "check" => check::entry(rest),
        "deps" => deps_command::entry(rest),
        "adopt" => result(adopt::run(rest)),
        "label" => result(label::run(rest)),
        "queues" => result(queues::run(rest)),
        "integrate" => result(integrate::run(rest)),
        "keep" => result(keep::run(rest)),
        "diff" => result(diff::run(rest)),
        "merge-driver" => result(merge::run(rest)),
        "similar" => similar_command::entry(rest),
        "sizes" => result(sizes::run(rest)),
        "worst" => result(worst_command::run(rest)),
        "figure" => result(coverage::figures::run(rest)),
        "objects" => result(objects::run(rest)),
        "show" => result(show::run(rest)),
        "-h" | "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        _ => {
            eprintln!("unknown alchemy command: {command}\n{USAGE}");
            ExitCode::from(2)
        }
    }
}

fn result(value: Result<(), String>) -> ExitCode {
    match value {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
