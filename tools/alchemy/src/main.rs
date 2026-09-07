use std::process::ExitCode;

mod allocator;
mod build;
mod build_assets;
mod check;
mod convert;
mod cross_edition;
mod families;
mod flatten;
mod font;
mod overlay;
mod scaffold;

const USAGE: &str = "usage: alchemy <command> [args]\n\
  decompile OWNER       recover candidate C\n\
  disassemble OWNER     display reference instructions\n\
  inspect OWNER         inspect calls and symbols; allocator OWNER reads GCC dumps\n\
  diff SOURCE           compile and compare a candidate\n\
  adopt OWNER           verify and integrate candidate C\n\
  match SOURCE          search decoder-named, catalogued source repairs\n\
  unit                  scaffold or flatten translation units\n\
  families              compare related functions and audit retained families\n\
  cross-edition         compare historical editions\n\
  dashboard             serve the local dashboard\n\
  build                 build compilers or ROM stages (asm, claimed, full, rom, assets)\n\
  verify                verify the staged repository using the build contract\n\
  coverage              rebuild and report ROM coverage\n\
  check                 run repository contract checks\n\
  convert               convert named file formats (see convert --help)\n\
  font                  rebuild the shared Golden Sun font\n\
  overlay               legacy overlay operations during migration";

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "inspect" if rest.first().map(String::as_str) == Some("allocator") => {
            allocator::entry(&rest[1..])
        }
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
        "font" => font::entry(rest),
        "build" if rest.first().map(String::as_str) == Some("assets") => {
            build_assets::entry(&rest[1..])
        }
        "build" => build::entry(rest),
        "verify" | "coverage" => make_target(command, rest),
        "check" => check::entry(rest),
        "convert" => result(convert::run(rest)),
        "overlay" => overlay::entry(rest),
        "decompile" => decompile_command("draft", rest),
        "adopt" => decompile_command("adopt", rest),
        "disassemble" => decompile_command("disasm", rest),
        "inspect" => decompile_command("imports", rest),
        "diff" => match overlay_candidate(rest) {
            Ok(true) => {
                let mut args = vec!["score".to_string()];
                args.extend_from_slice(rest);
                overlay::entry(&args)
            }
            Ok(false) => {
                diff::entrypoint::entry(rest);
                ExitCode::SUCCESS
            }
            Err(error) => result(Err(error)),
        },
        "cross-edition" => result(cross_edition::run(rest)),
        "families" => result(families::run(rest)),
        "match" => result(matching::run(rest.to_vec())),
        "dashboard" => {
            dashboard_server::cli::entry(rest);
            ExitCode::SUCCESS
        }
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
        .current_dir(compiler_core::routing::root())
        .arg(target)
        .status()
    {
        Ok(status) if status.success() => ExitCode::SUCCESS,
        Ok(_) => ExitCode::FAILURE,
        Err(error) => result(Err(format!("cannot run make {target}: {error}"))),
    }
}

fn overlay_candidate(arguments: &[String]) -> Result<bool, String> {
    use compiler_core::source_paths::{SourceOwner, SourcePaths};
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
        let paths = SourcePaths::load(compiler_core::routing::root())?;
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

fn decompile_command(command: &str, arguments: &[String]) -> ExitCode {
    if arguments == ["--help"] || arguments == ["-h"] {
        let usage = match command {
            "draft" => "decompile OWNER [--span BYTES] [--name NAME] [--out FILE]",
            "adopt" => "adopt OWNER [--source FILE] [--span BYTES] [--name NAME] [--path PATH]",
            "disasm" => "disassemble OWNER [--span BYTES]",
            "imports" => "inspect OWNER [--span BYTES]",
            _ => return decompile::cli::entry(&["--help".to_string()]),
        };
        println!(
            "usage: alchemy {usage}\nOWNER is a main-ROM address or resource-qualified address."
        );
        return ExitCode::SUCCESS;
    }
    let mut args = vec![command.to_string()];
    args.extend_from_slice(arguments);
    decompile::cli::entry(&args)
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
