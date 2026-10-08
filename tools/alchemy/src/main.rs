use std::process::ExitCode;

mod adopt;
mod adopt_data;
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
mod camelotize;
mod check;
mod compiler;
mod coverage;
mod dead;
mod devices;
mod decode;
mod deps;
mod deps_command;
mod diff;
mod edition;
mod editions;
mod elf;
mod fakematch;
mod gate;
mod gbafix;
mod integrate;
mod keep;
mod label;
mod ld_map;
mod merge;
mod modern;
mod objects;
mod permute;
mod process;
mod queues;
mod show;
mod sweep;
mod similar;
mod similar_command;
mod sizes;
mod targets;
mod thumb;
mod units;
mod verify;
mod worst;
mod worst_command;
mod worktree;

const USAGE: &str = r#"usage: alchemy <command> [args]   (both games, all twelve editions)
Run any command with --help for its options. The binary acts on the
checkout it runs in.

Set up and build
  bootstrap     install or validate the pinned compiler toolchain
  build         compilers, maintained source, assembly and ROMs
  verify        every gate of make verify, make land and the commit hooks
  check         one repository contract check
  worktree      prepare a new git worktree before its first build
  gbafix        fix a ROM's cartridge header as pret's gbafix: Nintendo logo and
                complement check; the build runs it on every ROM it links

Choose what to match
  deps          what not-yet-C code calls and blocks: what unblocks the most
  similar       each not-yet-C function's nearest C twins in either game

Match one function
  diff          a C draft against the listing it would replace
  sizes         the structs a draft sees, and the locals that fill a frame gap
  permute       search rewrites and PERM macro choices of a draft toward differ 0
  show          a built function beside the reference ROM
  adopt         the C into its module (--into) and out of all scaffolding
  label         name English-only scaffold data in the other editions

Adopt data
  adopt-data    a data block's object into all six linker scripts and its
                bytes out of every edition's scaffold, splitting sections

Check the whole build
  objects       what differs from the reference ROM, object or placement
  dead          what no edition links, includes or selects: unlinked files,
                never-selected #if arms, unused header declarations
  progress      DONE in every edition from the verified linker maps

Camelotize
  worst         maintained C ranked by how mechanical it reads
  fakematch     every FAKEMATCH tag by device and family, largest first
  camelotize    mechanical rewrites: actors, events, registers,
                declarations, barriers
  keep          keep only the edits that leave all twelve editions identical
  sweep         drop FAKEMATCH pins, barriers and volatiles the bytes no longer need
  units         propose Camelot's translation units from the linked images

Work in parallel
  queues        split not-yet-C work into queues that cannot collide
  integrate     merge branches, reconcile linker scripts, build all twelve
  merge-driver  git merge drivers for scaffolds and linker scripts

Report for people (nothing else reads it)
  figure        PROGRESS.png and PROGRESS_CHART.png from the verified count"#;

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
        "gbafix" => result(gbafix::run(rest)),
        "verify" => verify::entry(rest),
        "check" => check::entry(rest),
        "progress" => {
            let mut args = vec!["progress".to_string()];
            args.extend(rest.iter().cloned());
            check::entry(&args)
        }
        "deps" => deps_command::entry(rest),
        "adopt" => result(adopt::run(rest)),
        "label" => result(label::run(rest)),
        "adopt-data" => result(adopt_data::run(rest)),
        "queues" => result(queues::run(rest)),
        "integrate" => result(integrate::run(rest)),
        "keep" => result(keep::run(rest)),
        "diff" => result(diff::run(rest)),
        "permute" => result(permute::run(rest)),
        "merge-driver" => result(merge::run(rest)),
        "similar" => similar_command::entry(rest),
        "sizes" => result(sizes::run(rest)),
        "worst" => result(worst_command::run(rest)),
        "fakematch" => result(fakematch::run(rest)),
        "figure" => result(coverage::figures::run(rest)),
        "camelotize" => result(camelotize::run(rest)),
        "objects" => result(objects::run(rest)),
        "dead" => result(dead::run(rest)),
        "show" => result(show::run(rest)),
        "sweep" => result(sweep::run(rest)),
        "units" => result(units::run(rest)),
        "worktree" => result(worktree::run(rest)),
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
