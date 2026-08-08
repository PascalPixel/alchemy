use overlay_showcase::{
    canonical_c_source, fail, is_showcase_id, owner_names, parse_manifest, report,
    uses_named_interface, Failure, Showcase,
};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};

fn repository_root() -> PathBuf {
    // The crate lives at tools-rs/overlay-showcase, so the repository root is
    // two levels up from CARGO_MANIFEST_DIR regardless of the invocation cwd.
    Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap().to_path_buf()
}

/// The interpreter that runs the TypeScript sub-checks. The TypeScript spawns
/// `process.execPath`, which is the running bun; from Rust the equivalent is
/// bun on PATH, overridable for a pinned install.
fn bun() -> String {
    std::env::var("BUN").unwrap_or_else(|_| "bun".to_string())
}

/// Run one TypeScript sub-check, failing with its combined output.
fn checked(root: &Path, arguments: &[&str]) -> Result<(), Failure> {
    let result = Command::new(bun()).args(arguments).current_dir(root).output();
    let output = match result {
        Ok(output) => output,
        Err(error) => return fail(format!("{} failed:\n{error}", arguments.join(" "))),
    };
    if output.status.success() {
        return Ok(());
    }
    fail(format!(
        "{} failed:\n{}{}",
        arguments.join(" "),
        String::from_utf8_lossy(&output.stderr),
        String::from_utf8_lossy(&output.stdout),
    ))
}

/// Run one native sub-check binary directly, failing with its combined output.
/// `overlay-driver` has no TypeScript file left to spawn through `bun`; its
/// `.ts` source was deleted once tools-rs/overlay-driver was verified
/// byte-for-byte and wired into the overlay dispatcher.
fn checked_native(root: &Path, binary: &str, arguments: &[&str]) -> Result<(), Failure> {
    let path = root.join(binary);
    let result = Command::new(&path).args(arguments).current_dir(root).output();
    let output = match result {
        Ok(output) => output,
        Err(error) => return fail(format!("{binary} {} failed:\n{error}", arguments.join(" "))),
    };
    if output.status.success() {
        return Ok(());
    }
    fail(format!(
        "{binary} {} failed:\n{}{}",
        arguments.join(" "),
        String::from_utf8_lossy(&output.stderr),
        String::from_utf8_lossy(&output.stdout),
    ))
}

fn escape(text: &str) -> String {
    text.replace('\\', "\\\\").replace('"', "\\\"")
}

/// Byte length and sha256 of the assembled overlay image.
///
/// See the PORT NOTE in lib.rs: `assembleOverlay` stays in TypeScript because
/// it carries the whole alchemy-gcc compilation plan behind it. Everything the
/// showcase check does with the result is done here.
fn assembled_image(root: &Path, id: &str) -> Result<(u64, String), Failure> {
    let disasm = root.join("tools/lib/overlay_disasm.ts");
    let source = root.join("assets/code").join(format!("{id}_overlay.s"));
    let script = format!(
        concat!(
            "import {{ assembleOverlay }} from \"{}\";\n",
            "try {{\n",
            "  const image = assembleOverlay(\"{}\");\n",
            "  const digest = new Bun.CryptoHasher(\"sha256\").update(image).digest(\"hex\");\n",
            "  console.log(`${{image.length}} ${{digest}}`);\n",
            "}} catch (error) {{\n",
            "  console.error(error instanceof Error ? error.message : String(error));\n",
            "  process.exit(1);\n",
            "}}\n",
        ),
        escape(&disasm.to_string_lossy()),
        escape(&source.to_string_lossy()),
    );
    let result = Command::new(bun()).args(["-e", &script]).current_dir(root).output();
    let output = match result {
        Ok(output) => output,
        Err(error) => return fail(format!("assembling {id} failed: {error}")),
    };
    if !output.status.success() {
        return fail(String::from_utf8_lossy(&output.stderr).trim_end().to_string());
    }
    let text = String::from_utf8_lossy(&output.stdout).trim().to_string();
    let mut parts = text.split(' ');
    match (parts.next().map(str::parse::<u64>), parts.next(), parts.next()) {
        (Some(Ok(length)), Some(digest), None) if digest.len() == 64 => {
            Ok((length, digest.to_string()))
        }
        _ => fail(format!("assembling {id} produced no image measurement: {text}")),
    }
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
        return fail("usage: overlay_showcase.ts resource_NNN");
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
        return fail(format!("{id} still has semantic owners: {}", semantic_owners.join(", ")));
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
            return fail(format!("{name} does not use the showcase overlay's named interface"));
        }
    }

    let (length, digest) = assembled_image(root, id)?;
    if length != showcase.decoded_bytes || digest != showcase.sha256 {
        return fail(format!(
            "{id} decoded image changed: bytes={length}/{} sha256={digest}/{}",
            showcase.decoded_bytes, showcase.sha256,
        ));
    }

    checked(root, &["tools/lib/overlay_published.ts", id])?;
    checked(root, &["tools/overlay/overlay_gaps.ts", id])?;
    checked(root, &["tools/overlay/overlay_certify.ts", id])?;
    checked_native(root, "tools-rs/overlay-driver/target/release/overlay-driver", &[id])?;

    println!(
        "{}",
        report(id, exact_owners.len(), length, &digest, &showcase.retained_assembly)
    );
    Ok(())
}

fn run() -> Result<(), Failure> {
    let root = repository_root();
    let requested = std::env::args().nth(1).unwrap_or_default();
    if requested == "--all" {
        for overlay in manifest(&root)? {
            main_for(&root, &overlay.id)?;
        }
        return Ok(());
    }
    main_for(&root, &requested)
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
