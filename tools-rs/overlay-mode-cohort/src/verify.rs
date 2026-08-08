//! `run` and `verifyOverlayCandidate`: the binutils boundary.
//!
//! `match_m2c`'s verifier intentionally makes the candidate filename double as
//! its routing identity. Overlay candidates cannot do that: their established
//! source name contains both overlay and address. This local adapter keeps the
//! address-stemmed scratch input for linking while asking the compiler policy
//! to route from the original candidate path. It is otherwise the same public
//! compile/link boundary used by `match_m2c`.

use crate::jsops::{js_parse_int, js_slice_to, js_split_lines, js_split_whitespace, js_subarray_to, js_trim};
use crate::paths::root;
use crate::plan::{address_from_source, Candidate};
use alchemy_plan::plan::{
    source_to_assembly_plan, CompilerFamily, CompilerFlagMutations, SourceToAssemblyPlanOptions,
};
use alchemy_routing::routing::CompilerTarget;
use alchemy_symbols::symbols::{external_symbol, external_symbol_assembly, overlay_call_via_base};
use mode_sweep::Config;
use overlay_disasm::compile::bias_in_image_label_words;
use overlay_disasm::disasm::OVERLAY_BASE;
use search_compiler_modes::relative_path;
use std::path::Path;

/// `run(command)`.
///
/// PORT NOTE -- FOUR DETAILS, EACH OBSERVABLE:
///
///  * `cwd` is `ROOT`, not the caller's directory. The plan's argv contains
///    repository-relative include paths, so running elsewhere changes the
///    preprocessor's view of the tree.
///  * The failure message names `basename(command[0])`, so `arm-none-eabi-as`
///    rather than its absolute path.
///  * `(stderr || stdout)` is `||`, not `??`: an EMPTY stderr falls through to
///    stdout. A tool that fails silently on stderr but explains itself on
///    stdout still produces a useful message.
///  * `.trim().slice(0, 500)` trims FIRST and truncates SECOND, so a message
///    with 600 leading spaces is not truncated to whitespace.
pub fn run(command: &[String]) -> Result<String, String> {
    let program = &command[0];
    let output = std::process::Command::new(program)
        .args(&command[1..])
        .current_dir(root())
        .output()
        // `Bun.spawn` throws on a missing executable. The text differs between
        // runtimes by design; the harness asserts same-failure, not same-prose.
        .map_err(|error| format!("{}: {error}", basename(program)))?;
    let stdout = String::from_utf8_lossy(&output.stdout).into_owned();
    if !output.status.success() {
        let stderr = String::from_utf8_lossy(&output.stderr).into_owned();
        let chosen = if stderr.is_empty() { &stdout } else { &stderr };
        return Err(format!(
            "{} failed: {}",
            basename(program),
            js_slice_to(js_trim(chosen), 500)
        ));
    }
    Ok(stdout)
}

fn basename(path: &str) -> String {
    Path::new(path)
        .file_name()
        .map(|name| name.to_string_lossy().into_owned())
        .unwrap_or_else(|| path.to_string())
}

/// `verifyOverlayCandidate(input, candidate, image, work, config)`.
pub fn verify_overlay_candidate(
    input: &str,
    candidate: &Candidate,
    image: &[u8],
    work: &Path,
    config: &Config,
) -> Result<Vec<u8>, String> {
    let address = address_from_source(candidate)?;
    // `relative(ROOT, source).split(sep).join("/")`: the routing tables are
    // keyed by forward-slash paths regardless of platform separator.
    let routing_source = relative_path(&root(), Path::new(&candidate.source));
    let call_via_base = overlay_call_via_base(&candidate.overlay, Some(&routing_source));
    let stem = format!("{:08x}", address as i64);
    let path_in = |suffix: &str| work.join(format!("{stem}{suffix}")).to_string_lossy().into_owned();
    let assembly = path_in(".s");
    let object = path_in(".o");
    let symbols_source = path_in(".symbols.s");
    let symbols_object = path_in(".symbols.o");
    let elf = path_in(".elf");
    let binary = path_in(".bin");

    let family = CompilerFamily::parse(&config.compiler_family)
        .ok_or_else(|| format!("unknown compiler family: {}", config.compiler_family))?;
    let plan = source_to_assembly_plan(&SourceToAssemblyPlanOptions {
        target: CompilerTarget::Gs1,
        routing_source: routing_source.clone(),
        input: input.to_string(),
        output: assembly.clone(),
        family: Some(family),
        flags: Some(CompilerFlagMutations {
            add_flags: config.flags.clone(),
            remove_flags: config.remove_flags.clone(),
        }),
        preprocessed_output: Some(path_in(".i")),
        dumpbase: None,
    })?;
    for step in &plan.steps {
        run(&step.command)?;
    }
    // Overlay code is linked 0x8000 above where the assembler thinks it sits,
    // so in-image label words need the bias applied before assembly.
    let text = String::from_utf8_lossy(
        &std::fs::read(&assembly).map_err(|error| format!("{assembly}: {error}"))?,
    )
    .into_owned();
    std::fs::write(&assembly, bias_in_image_label_words(&text).text)
        .map_err(|error| format!("{assembly}: {error}"))?;

    run(&argv(&["arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", &object, &assembly]))?;

    // PORT NOTE -- THIS LIST IS A `[]`, NOT A `Set`. `nm -u` can name the same
    // undefined symbol twice, and the TypeScript emits a stub for each. A
    // `HashSet` "cleanup" would emit one, and the assembler would then see a
    // different stub block. It happens to assemble either way today, which is
    // exactly why the divergence would go unnoticed.
    let mut names: Vec<String> = Vec::new();
    let undefined = run(&argv(&["arm-none-eabi-nm", "-u", &object]))?;
    for line in js_split_lines(&undefined) {
        // `.filter(Boolean)` drops empty lines, including the trailing one.
        if line.is_empty() {
            continue;
        }
        // `.trim().split(/\s+/).at(-1)!` -- the LAST field, because `nm -u`
        // prints `         U name` with leading blanks.
        let fields = js_split_whitespace(js_trim(line));
        let name = (*fields.last().unwrap_or(&"")).to_string();
        if external_symbol(&name, call_via_base).is_none() {
            return Err(format!("unsupported external symbol: {name}"));
        }
        names.push(name);
    }
    let mut stub = String::from(".syntax unified\n.thumb\n");
    for name in &names {
        stub.push_str(&external_symbol_assembly(name, call_via_base)?);
    }
    std::fs::write(&symbols_source, stub).map_err(|error| format!("{symbols_source}: {error}"))?;
    run(&argv(&[
        "arm-none-eabi-as", "-mcpu=arm7tdmi", "-mthumb-interwork", "-o", &symbols_object, &symbols_source,
    ]))?;
    run(&argv(&[
        "arm-none-eabi-ld",
        &format!("-Ttext=0x{stem}"),
        "-e",
        &format!("Func_{stem}"),
        "-o",
        &elf,
        &object,
        &symbols_object,
    ]))?;
    run(&argv(&["arm-none-eabi-objcopy", "-O", "binary", "-j", ".text", &elf, &binary]))?;

    let listing = run(&argv(&["arm-none-eabi-nm", "-S", &elf]))?;
    let rows = js_split_lines(&listing);
    let suffix = format!(" Func_{stem}");
    let row = match rows.iter().find(|line| line.ends_with(&suffix)) {
        Some(row) => *row,
        None => return Err(format!("missing linked function Func_{stem}")),
    };
    let fields = js_split_whitespace(js_trim(row));
    let data = std::fs::read(&binary).map_err(|error| format!("{binary}: {error}"))?;
    // `nm -S` prints `addr size type name` when a size is recorded and
    // `addr type name` when it is not. Fewer than four fields means the linker
    // gave no size, and the whole `.text` image is the answer.
    let size = if fields.len() >= 4 {
        js_parse_int(fields[1], 16)
    } else {
        data.len() as f64
    };
    // `image` is deliberately an argument: keeping it in this adapter makes it
    // impossible to accidentally score a main-image reference against an
    // overlay.
    let relative_address = address - OVERLAY_BASE as f64;
    if relative_address < 0.0 || relative_address >= image.len() as f64 {
        return Err("overlay address outside image".into());
    }
    Ok(js_subarray_to(&data, size).to_vec())
}

fn argv(parts: &[&str]) -> Vec<String> {
    parts.iter().map(|part| part.to_string()).collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_failure_names_the_basename_and_prefers_stderr() {
        let error = run(&argv(&["/usr/bin/false"])).unwrap_err();
        assert!(error.starts_with("false failed: "), "{error}");
    }

    #[test]
    fn an_empty_stderr_falls_through_to_stdout() {
        // `||`, not `??`: `sh -c 'echo boom; exit 1'` writes nothing to stderr.
        let error = run(&argv(&["/bin/sh", "-c", "echo boom; exit 1"])).unwrap_err();
        assert_eq!(error, "sh failed: boom");
    }

    #[test]
    fn the_message_is_trimmed_before_it_is_truncated() {
        let error = run(&argv(&[
            "/bin/sh",
            "-c",
            "printf '%600s' '' >&2; printf 'tail' >&2; exit 1",
        ]))
        .unwrap_err();
        assert_eq!(error, "sh failed: tail");
    }

    #[test]
    fn a_missing_executable_is_an_error_not_a_panic() {
        let error = run(&argv(&["arm-none-eabi-definitely-not-a-tool"])).unwrap_err();
        assert!(error.contains("arm-none-eabi-definitely-not-a-tool"), "{error}");
    }

    #[test]
    fn the_binutils_this_port_shells_out_to_are_present() {
        // SCANNING NOTHING IS NOT PASSING. If the toolchain is absent the
        // subprocess half of this port is untested, and that must be loud.
        for tool in [
            "arm-none-eabi-as",
            "arm-none-eabi-ld",
            "arm-none-eabi-nm",
            "arm-none-eabi-objcopy",
        ] {
            let found = std::env::var_os("PATH")
                .map(|path| {
                    std::env::split_paths(&path).any(|directory| directory.join(tool).exists())
                })
                .unwrap_or(false);
            assert!(found, "{tool} is not on PATH; the verifier cannot be exercised");
        }
    }
}
