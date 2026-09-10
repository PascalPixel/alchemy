use std::process::ExitCode;

mod allocator;
mod build;
mod build_asm;
mod build_assets;
mod build_claimed;
mod build_full;
mod candidate;
mod check;
mod compiler;
mod coverage;
mod cross_edition;
mod dashboard;
mod flatten;
mod generated_files;
mod http;
mod matching;
mod overlay;
mod recovery;
mod scaffold;
mod score;
mod targets;

const USAGE: &str = "usage: alchemy <command> [args]\n\
  extract OWNER         extract reference bytes for psynergy decompile\n\
  inspect OWNER         resolve calls and symbols; --asm shows annotated instructions\n\
  score SOURCE          compile through the approved route and compare the owner\n\
  adopt OWNER           verify and integrate candidate C\n\
  match SOURCE          search decoder-named, catalogued source repairs\n\
  unit                  scaffold or flatten translation units\n\
  cross-edition         compare historical editions\n\
  build                 build compilers, ROM stages, assets or allocator dumps\n\
  verify                verify the staged repository using the build contract\n\
  coverage              rebuild and report ROM coverage\n\
  dashboard             serve live coverage on localhost:4650\n\
  check                 run repository contract checks\n\
  overlay               legacy overlay operations during migration";

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "unit" if rest.first().map(String::as_str) == Some("scaffold") => {
            scaffold::entry(&rest[1..])
        }
        "unit" if rest.first().map(String::as_str) == Some("flatten") => flatten::entry(&rest[1..]),
        "unit" => {
            println!("usage: alchemy unit <scaffold|flatten> [args]");
            if rest == ["--help"] || rest == ["-h"] {
                ExitCode::SUCCESS
            } else {
                ExitCode::from(2)
            }
        }
        "dashboard" => result(dashboard::entry(rest)),
        "build" => build::entry(rest),
        "verify" | "coverage" => make_target(command, rest),
        "check" => check::entry(rest),
        "overlay" => overlay::entry(rest),
        "extract" | "adopt" | "inspect" => recovery_command(command, rest),
        "score" => match overlay_candidate(rest) {
            Ok(true) => overlay::code(overlay::score::run(crate::compiler::routing::root(), rest)),
            Ok(false) => {
                score::entry(rest);
                ExitCode::SUCCESS
            }
            Err(error) => result(Err(error)),
        },
        "cross-edition" => result(cross_edition::run(rest)),
        "match" => result(matching::run(rest)),
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

fn make_target(target: &str, arguments: &[String]) -> ExitCode {
    if arguments == ["--help"] || arguments == ["-h"] {
        println!("usage: alchemy {target}\nRuns the repository's make {target} contract.");
        return ExitCode::SUCCESS;
    }
    if !arguments.is_empty() {
        eprintln!("usage: alchemy {target}");
        return ExitCode::from(2);
    }
    match std::process::Command::new("make")
        .current_dir(crate::compiler::routing::root())
        .arg(target)
        .status()
    {
        Ok(status) if status.success() => ExitCode::SUCCESS,
        Ok(_) => ExitCode::FAILURE,
        Err(error) => result(Err(format!("cannot run make {target}: {error}"))),
    }
}

fn overlay_candidate(arguments: &[String]) -> Result<bool, String> {
    use crate::compiler::source_paths::{SourceOwner, SourcePaths};
    if arguments.iter().any(|arg| arg == "--unit") {
        return Ok(false);
    }
    if let Some(pair) = arguments.windows(2).find(|pair| pair[0] == "--owner") {
        return SourceOwner::parse_argument(&pair[1]).map(|owner| !owner.is_main());
    }
    let Some(first) = arguments.first() else {
        return Ok(false);
    };
    if first.contains(':') && !first.starts_with('-') {
        return SourceOwner::parse_argument(first).map(|owner| !owner.is_main());
    }
    let path = std::path::Path::new(first);
    if path.is_file() {
        let paths = SourcePaths::load(crate::compiler::routing::root())?;
        return Ok(paths
            .owner_for_path(path)?
            .is_some_and(|owner| !owner.is_main()));
    }
    Ok(false)
}

#[cfg(test)]
mod command_tests {
    use super::overlay_candidate;

    #[test]
    fn explicit_owner_selects_address_space() {
        for (owner, overlay) in [
            ("080bbb0c", false),
            ("main:080bbb0c", false),
            ("resource_3ba:02002910", true),
        ] {
            let args = vec!["draft.c".into(), "--owner".into(), owner.into()];
            assert_eq!(overlay_candidate(&args).unwrap(), overlay);
        }
        assert!(overlay_candidate(&["resource_3ba:02002910".into()]).unwrap());
        assert!(!overlay_candidate(
            &["--unit", "battle", "--owner", "resource_3ba:02002910"].map(str::to_owned)
        )
        .unwrap());
    }

    #[test]
    fn invalid_explicit_owner_fails_instead_of_guessing_main() {
        assert!(overlay_candidate(&[
            "draft.c".into(),
            "--owner".into(),
            "resource_bad:nope".into()
        ])
        .is_err());
        assert!(!overlay_candidate(&["--help".into()]).unwrap());
        assert!(!overlay_candidate(&["--unit".into(), "battle-action-resolution".into()]).unwrap());
    }
}

fn recovery_command(command: &str, arguments: &[String]) -> ExitCode {
    if arguments == ["--help"] || arguments == ["-h"] {
        let usage = match command {
            "extract" => "extract OWNER --out out/FILE [--span BYTES]",
            "adopt" => "adopt OWNER [--source FILE] [--span BYTES] [--name NAME] [--path PATH]",
            "inspect" => "inspect OWNER [--span BYTES] [--asm]",
            _ => return recovery::cli::entry(&["--help".to_string()]),
        };
        println!(
            "usage: alchemy {usage}\nOWNER is a main-ROM address or resource-qualified address."
        );
        return ExitCode::SUCCESS;
    }
    let mut args = vec![command.to_string()];
    args.extend_from_slice(arguments);
    recovery::cli::entry(&args)
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
