use overlay_showcase::{
    canonical_c_source, fail, is_showcase_id, owner_names, parse_manifest, report,
    uses_named_interface, Failure, Showcase,
};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};

fn repository_root() -> PathBuf {
    // The crate lives at tools/overlay-showcase, so the repository root is
    // two levels up from CARGO_MANIFEST_DIR regardless of the invocation cwd.
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

fn cargo_command(root: &Path, crate_name: &str) -> Command {
    let mut command = Command::new("cargo");
    command
        .args([
            "run",
            "--offline",
            "--quiet",
            "--release",
            "--manifest-path",
        ])
        .arg(root.join("tools").join(crate_name).join("Cargo.toml"))
        .arg("--")
        .current_dir(root);
    command
}

/// Run one native sub-check through its Cargo manifest, failing with its combined output.
fn checked_native(root: &Path, crate_name: &str, arguments: &[&str]) -> Result<(), Failure> {
    let result = cargo_command(root, crate_name).args(arguments).output();
    let output = match result {
        Ok(output) => output,
        Err(error) => {
            return fail(format!(
                "tools/{crate_name} {} failed:\n{error}",
                arguments.join(" ")
            ))
        }
    };
    if output.status.success() {
        return Ok(());
    }
    fail(format!(
        "tools/{crate_name} {} failed:\n{}{}",
        arguments.join(" "),
        String::from_utf8_lossy(&output.stderr),
        String::from_utf8_lossy(&output.stdout),
    ))
}

/// Byte length and sha256 of the assembled overlay image.
fn assembled_image(root: &Path, id: &str) -> Result<(u64, String), Failure> {
    let source = root.join("assets/code").join(format!("{id}_overlay.s"));
    let image = overlay_disasm::assemble_overlay(
        &overlay_disasm::OverlaySource::path(&source),
        overlay_disasm::OVERLAY_BASE,
    )
    .map_err(Failure)?;
    Ok((image.len() as u64, alchemy_bundle::sha256::hex(&image)))
}

fn manifest(root: &Path) -> Result<Vec<Showcase>, Failure> {
    let path = root.join("metrics/overlay-showcases.json");
    match fs::read_to_string(&path) {
        Ok(text) => parse_manifest(&text),
        Err(error) => fail(format!("{}: {error}", path.display())),
    }
}

fn main_for(root: &Path, id: &str) -> Result<(), Failure> {
    if !is_showcase_id(id) {
        return fail("usage: overlay-showcase resource_NNN");
    }
    let overlays = manifest(root)?;
    let showcase = match overlays.iter().find(|row| row.id == id) {
        Some(showcase) => showcase,
        None => return fail(format!("{id} is not a registered showcase overlay")),
    };

    let semantic = root.join("semantic");
    let semantic_owners = if semantic.exists() {
        match owner_names(&semantic, id) {
            Ok(names) => names,
            Err(error) => return fail(format!("{}: {error}", semantic.display())),
        }
    } else {
        Vec::new()
    };
    if !semantic_owners.is_empty() {
        return fail(format!(
            "{id} still has semantic owners: {}",
            semantic_owners.join(", ")
        ));
    }

    let code = root.join("exact");
    let exact_owners = match owner_names(&code, id) {
        Ok(names) => names,
        Err(error) => return fail(format!("{}: {error}", code.display())),
    };
    if exact_owners.len() != showcase.exact_c_owners {
        return fail(format!(
            "{id} exact owner count is {}, expected {}",
            exact_owners.len(),
            showcase.exact_c_owners,
        ));
    }
    for name in &exact_owners {
        let path = code.join(name);
        let source = match fs::read_to_string(&path) {
            Ok(source) => source,
            Err(error) => return fail(format!("{}: {error}", path.display())),
        };
        if !canonical_c_source(&source) {
            return fail(format!(
                "{name} uses inline assembly or register-pinned fakematch and is not canonical Exact C"
            ));
        }
        if !uses_named_interface(&source, id) {
            return fail(format!(
                "{name} does not use the showcase overlay's named interface"
            ));
        }
    }

    let (length, digest) = assembled_image(root, id)?;
    if length != showcase.decoded_bytes || digest != showcase.sha256 {
        return fail(format!(
            "{id} decoded image changed: bytes={length}/{} sha256={digest}/{}",
            showcase.decoded_bytes, showcase.sha256,
        ));
    }

    checked_native(root, "overlay-published", &[id])?;
    checked_native(root, "overlay-gaps", &[id])?;
    checked_native(root, "overlay-certify", &[id])?;
    checked_native(root, "overlay-driver", &[id])?;

    println!(
        "{}",
        report(
            id,
            exact_owners.len(),
            length,
            &digest,
            &showcase.retained_assembly
        )
    );
    Ok(())
}

const USAGE: &str = "usage: overlay-showcase <resource_NNN>\n       overlay-showcase --all\n       overlay-showcase --self-test";

#[derive(Debug, PartialEq, Eq)]
enum CommandLine {
    Help,
    SelfTest,
    All,
    One(String),
}

fn parse_arguments(arguments: &[String]) -> Result<CommandLine, Failure> {
    match arguments {
        [argument] if argument == "-h" || argument == "--help" => Ok(CommandLine::Help),
        [argument] if argument == "--self-test" => Ok(CommandLine::SelfTest),
        [argument] if argument == "--all" => Ok(CommandLine::All),
        [argument] if !argument.starts_with('-') && is_showcase_id(argument) => {
            Ok(CommandLine::One(argument.clone()))
        }
        [] => fail(USAGE.to_string()),
        _ => fail(format!("unknown or misplaced option\n{USAGE}")),
    }
}

fn run() -> Result<(), Failure> {
    let root = repository_root();
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    match parse_arguments(&arguments)? {
        CommandLine::Help => {
            println!("{USAGE}");
            Ok(())
        }
        CommandLine::SelfTest => {
            let overlays = manifest(&root)?;
            let showcase = overlays
                .first()
                .ok_or_else(|| Failure("overlay-showcase self-test found no showcase".into()))?;
            main_for(&root, &showcase.id)?;
            println!("self-test=ok overlay={}", showcase.id);
            Ok(())
        }
        CommandLine::All => {
            for overlay in manifest(&root)? {
                main_for(&root, &overlay.id)?;
            }
            Ok(())
        }
        CommandLine::One(requested) => main_for(&root, &requested),
    }
}

fn main() -> ExitCode {
    match run() {
        Ok(()) => ExitCode::SUCCESS,
        Err(failure) => {
            eprintln!("{failure}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn cli_contract_is_strict_and_help_is_side_effect_free() {
        assert_eq!(
            parse_arguments(&["--help".into()]).unwrap(),
            CommandLine::Help
        );
        assert_eq!(
            parse_arguments(&["--all".into()]).unwrap(),
            CommandLine::All
        );
        assert_eq!(
            parse_arguments(&["resource_37c".into()]).unwrap(),
            CommandLine::One("resource_37c".into())
        );
        assert!(parse_arguments(&["--unknown".into()]).is_err());
        assert!(parse_arguments(&["--all".into(), "resource_37c".into()]).is_err());
    }

    #[test]
    fn child_checks_are_cargo_authoritative() {
        let command = cargo_command(Path::new("/repo"), "overlay-driver");
        assert_eq!(command.get_program(), "cargo");
        let args: Vec<_> = command
            .get_args()
            .map(|arg| arg.to_string_lossy().into_owned())
            .collect();
        assert_eq!(
            args[0..6],
            [
                "run",
                "--offline",
                "--quiet",
                "--release",
                "--manifest-path",
                "/repo/tools/overlay-driver/Cargo.toml",
            ]
        );
        assert_eq!(args[6], "--");
    }
}
