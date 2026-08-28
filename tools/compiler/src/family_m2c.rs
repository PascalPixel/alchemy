//! Exact-family m2c seed generation for `compiler families transplant`.

use candidate_compiler::verify::{
    compile_to_assembly, run, CandidateCompilerConfiguration, CandidateCompilerFamily,
};
use compiler_core::{
    plan::direct_preprocessor_command,
    routing::{root, uses_agbcc_compiler, CompilerTarget},
    source_paths::SourceOwner,
};
use regex::Regex;
use std::{
    collections::BTreeMap,
    env, fs,
    path::{Path, PathBuf},
};

pub(crate) struct Seed {
    pub source: PathBuf,
    pub context: Option<PathBuf>,
    pub context_kind: &'static str,
}

pub(crate) fn generate(
    target_owner: SourceOwner,
    target_assembly: &Path,
    target_symbol: &str,
    template: Option<(SourceOwner, &Path, &str)>,
    output: &Path,
) -> Result<Seed, String> {
    fs::create_dir_all(output).map_err(|error| format!("{}: {error}", output.display()))?;
    let context = if let Some((template_owner, template_source, template_symbol)) = template {
        let route = template_owner.routing_path();
        let assembly = compile_to_assembly(
            text_path(template_source)?,
            text_path(&route)?,
            text_path(&output.join("template-build"))?,
            &[],
            CompilerTarget::Gs1,
            &CandidateCompilerConfiguration {
                family: Some(CandidateCompilerFamily::Routed),
                ..Default::default()
            },
        )?;
        let generated = Path::new(&assembly).with_extension("i");
        if !uses_agbcc_compiler(CompilerTarget::Gs1, text_path(&route)?) {
            run(
                &direct_preprocessor_command(text_path(template_source)?, text_path(&generated)?)?,
                root(),
            )?;
        }
        let context_text = read(&generated)?;
        if !context_text.contains(template_symbol) {
            return Err(format!(
                "{}: preprocessed exact template does not define {template_symbol}",
                template_source.display()
            ));
        }
        let path = output.join("family-template.i");
        write(
            &path,
            context_text
                .replace(template_symbol, target_symbol)
                .as_bytes(),
        )?;
        Some(path)
    } else {
        None
    };
    let target_text = read(target_assembly)?;
    let mut canonical_text = String::new();
    let mut previous_is_symbol = false;
    for line in target_text.lines() {
        let trimmed = line.trim();
        let alias = previous_is_symbol
            && (trimmed.starts_with("Region_") || trimmed.starts_with("Func_"))
            && trimmed.ends_with(':');
        canonical_text.push_str(if alias {
            "@ canonical same-address owner alias omitted"
        } else {
            line
        });
        canonical_text.push('\n');
        previous_is_symbol = trimmed == format!("{target_symbol}:");
    }
    let canonical = output.join("target.canonical.s");
    let listing = output.join("target.lst");
    let object = output.join("target.o");
    write(&canonical, canonical_text.as_bytes())?;
    run(
        &[
            "arm-none-eabi-as".into(),
            "-mcpu=arm7tdmi".into(),
            "-mthumb-interwork".into(),
            format!("-alhnd={}", listing.display()),
            "-o".into(),
            object.to_string_lossy().into_owned(),
            canonical.to_string_lossy().into_owned(),
        ],
        root(),
    )?;
    let symbolized = output.join("symbolized.s");
    write(
        &symbolized,
        symbolize(target_assembly, &listing, target_symbol)?.as_bytes(),
    )?;

    let m2c = locate_m2c()?;
    let source = output.join(format!("{}.c", target_owner.address_stem()));
    let arguments = m2c_arguments(&m2c, target_symbol, &symbolized, context.as_deref());
    let translated = run(&arguments, root())?;
    let mut body = b"#include \"types.h\"\n\n".to_vec();
    body.extend(
        fs::read(m2c.parent().unwrap_or(Path::new(".")).join("m2c_macros.h"))
            .map_err(|error| format!("m2c_macros.h: {error}"))?,
    );
    body.push(b'\n');
    body.extend(translated.as_bytes());
    write(&source, &body)?;
    Ok(Seed {
        source,
        context,
        context_kind: if template.is_some() {
            "family_template"
        } else {
            "none"
        },
    })
}

fn m2c_arguments(m2c: &Path, symbol: &str, assembly: &Path, context: Option<&Path>) -> Vec<String> {
    let mut arguments = vec![
        "python3".into(),
        text_path(m2c).unwrap().into(),
        "-t".into(),
        "gba-gcc-c".into(),
        "-f".into(),
        symbol.into(),
        "--valid-syntax".into(),
        "--deterministic-vars".into(),
        "--globals".into(),
        "none".into(),
        "--no-cache".into(),
    ];
    if let Some(context) = context {
        arguments.extend(["--context".into(), text_path(context).unwrap().into()]);
    }
    arguments.push(text_path(assembly).unwrap().into());
    arguments
}

fn locate_m2c() -> Result<PathBuf, String> {
    env::var_os("M2C")
        .map(PathBuf::from)
        .into_iter()
        .chain(std::iter::once(root().join("m2c/m2c.py")))
        .chain(
            env::var_os("PATH")
                .into_iter()
                .flat_map(|paths| env::split_paths(&paths).collect::<Vec<_>>())
                .map(|path| path.join("m2c.py")),
        )
        .find(|path| path.is_file())
        .and_then(|path| path.canonicalize().ok())
        .ok_or_else(|| "m2c.py not found; clone upstream m2c into ignored m2c/ or set M2C".into())
}

/// Turn numeric PC-relative operands into the local labels m2c requires. The
/// assembler listing is the single source of truth for source-line offsets.
fn symbolize(source: &Path, listing: &Path, symbol: &str) -> Result<String, String> {
    let original_text = read(source)?;
    let original = original_text.lines().collect::<Vec<_>>();
    let base_regex = Regex::new(r"(?i)([0-9a-f]{8})").unwrap();
    let base = source
        .file_stem()
        .and_then(|stem| stem.to_str())
        .and_then(|stem| base_regex.captures(stem))
        .and_then(|capture| u64::from_str_radix(&capture[1], 16).ok())
        .ok_or_else(|| format!("cannot derive image address from {}", source.display()))?;
    let listing_row = Regex::new(r"^\s*(\d+)\s+([0-9A-Fa-f]{4,8})\s+").unwrap();
    let mut offsets = vec![None; original.len()];
    let mut offset_lines = BTreeMap::new();
    let listing_text =
        fs::read(listing).map_err(|error| format!("{}: {error}", listing.display()))?;
    for capture in String::from_utf8_lossy(&listing_text)
        .lines()
        .filter_map(|line| listing_row.captures(line))
    {
        let line = capture[1]
            .parse::<usize>()
            .map_err(|error| error.to_string())?;
        let offset = u64::from_str_radix(&capture[2], 16).map_err(|error| error.to_string())?;
        if let Some(slot) = line.checked_sub(1).and_then(|line| offsets.get_mut(line)) {
            *slot = Some(offset);
            offset_lines.entry(offset).or_insert(line - 1);
        }
    }

    let mov_pc = Regex::new(r"\bmov\s+pc,\s*r\d+").unwrap();
    let word = Regex::new(r"\.4byte\s+(0x[0-9A-Fa-f]+)").unwrap();
    let pc_load = Regex::new(r"\[pc,\s*#([0-9]+)\]").unwrap();
    let absolute =
        Regex::new(r"^(\s*)\.set\s+((?:sub|Func)_[0-9A-Fa-f]{8}),\s*0x[0-9A-Fa-f]+\s*$").unwrap();
    let mut lines = original
        .iter()
        .map(|line| (*line).to_owned())
        .collect::<Vec<_>>();
    let mut labels = vec![Vec::<String>::new(); lines.len()];
    let mut changed = vec![false; lines.len()];
    let mut external_targets = BTreeMap::new();
    let mut table = 0;
    let mut at = 0;
    while at < original.len() {
        if !mov_pc.is_match(original[at]) {
            at += 1;
            continue;
        }
        let start = (at + 1..original.len())
            .find(|line| word.is_match(original[*line]))
            .ok_or_else(|| format!("no jump table after source line {}", at + 1))?;
        let end = (start..original.len())
            .find(|line| !word.is_match(original[*line]))
            .unwrap_or(original.len());
        let loader = (at.saturating_sub(24)..at)
            .rev()
            .find(|line| pc_load.is_match(original[*line]))
            .ok_or_else(|| format!("no PC-relative jump-table load before line {}", at + 1))?;
        table += 1;
        let table_label = format!(".Lm2c_jtbl_{table}");
        let pointer_label = format!(".Lm2c_jtbl_ptr_{table}");
        labels[start].push(format!("{table_label}:"));
        lines[loader] = pc_load
            .replace(original[loader], pointer_label.as_str())
            .into_owned();
        changed[loader] = true;
        for line in start..end {
            let capture = word.captures(original[line]).unwrap();
            let target = u64::from_str_radix(capture[1].trim_start_matches("0x"), 16)
                .map_err(|error| error.to_string())?;
            let target_line = offset_lines.get(&target.saturating_sub(base)).copied();
            let target_label = target_line.map_or_else(
                || format!("sub_{target:08x}"),
                |_| format!(".Lm2c_{target:08x}"),
            );
            lines[line] = word
                .replace(original[line], format!(".4byte {target_label}"))
                .into_owned();
            changed[line] = true;
            if let Some(target_line) = target_line {
                labels[target_line].push(format!("{target_label}:"));
            } else {
                external_targets.insert(target_label, ());
            }
        }
        let address = base
            + offsets[start].ok_or_else(|| format!("listing omits source line {}", start + 1))?;
        let pointer = format!("0x{address:08x}");
        for line in 0..original.len() {
            if !(start..end).contains(&line) && original[line].contains(&pointer) {
                lines[line] = original[line].replace(&pointer, &table_label);
                changed[line] = true;
                labels[line].push(format!("{pointer_label}:"));
            }
        }
        at = end;
    }

    let image = fs::read(root().join("roms/gs1-en.gba")).map_err(|error| error.to_string())?;
    let mut external_pools = BTreeMap::new();
    for line in 0..original.len() {
        let Some(capture) = (!changed[line])
            .then(|| pc_load.captures(original[line]))
            .flatten()
        else {
            continue;
        };
        let instruction = offsets[line]
            .ok_or_else(|| format!("listing omits PC load on source line {}", line + 1))?;
        let displacement = capture[1]
            .parse::<u64>()
            .map_err(|error| error.to_string())?;
        let pool = ((instruction + 4) & !3) + displacement;
        let pool_label = format!(".Lm2c_pool_{pool:04x}");
        if let Some(pool_line) = offset_lines.get(&pool).copied() {
            labels[pool_line].push(format!("{pool_label}:"));
        } else {
            let address = base + pool;
            let offset = address
                .checked_sub(0x0800_0000)
                .and_then(|value| usize::try_from(value).ok())
                .ok_or_else(|| format!("literal pool address 0x{address:08x} is outside ROM"))?;
            let bytes: [u8; 4] = image
                .get(offset..offset + 4)
                .ok_or_else(|| format!("literal pool address 0x{address:08x} is outside ROM"))?
                .try_into()
                .unwrap();
            external_pools.insert(pool, u32::from_le_bytes(bytes));
        }
        lines[line] = pc_load
            .replace(original[line], pool_label.as_str())
            .into_owned();
    }

    let mut output = String::new();
    for target in external_targets.keys() {
        output.push_str(&format!("\t.global {target}\n"));
    }
    for line in 0..lines.len() {
        for (index, label) in labels[line].iter().enumerate() {
            if labels[line][..index].contains(label) {
                continue;
            }
            output.push_str(label);
            output.push('\n');
        }
        let trimmed = original[line].trim();
        if line > 0
            && original[line - 1].trim() == format!("{symbol}:")
            && (trimmed.starts_with("Region_") || trimmed.starts_with("Func_"))
            && trimmed.ends_with(':')
        {
            continue;
        }
        if let Some(capture) = absolute.captures(&lines[line]) {
            output.push_str(&format!("{}\t.global {}", &capture[1], &capture[2]));
        } else {
            output.push_str(&lines[line]);
        }
        output.push('\n');
    }
    for (address, value) in external_pools {
        output.push_str(&format!(
            ".Lm2c_pool_{address:04x}:\n.4byte 0x{value:08x}\n"
        ));
    }
    Ok(output)
}

fn text_path(path: &Path) -> Result<&str, String> {
    path.to_str()
        .ok_or_else(|| format!("{}: non-UTF-8 path", path.display()))
}
fn read(path: &Path) -> Result<String, String> {
    fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))
}
fn write(path: &Path, bytes: &[u8]) -> Result<(), String> {
    fs::write(path, bytes).map_err(|error| format!("{}: {error}", path.display()))
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn symbolizes_jump_tables_external_pools_and_aliases() {
        let directory = env::temp_dir().join(format!("alchemy-family-m2c-{}", std::process::id()));
        let _ = fs::remove_dir_all(&directory);
        fs::create_dir_all(&directory).unwrap();
        let source = directory.join("08000000.s");
        let listing = directory.join("target.lst");
        write(&source, b".thumb\nldr r2, [pc, #8]\nmov pc, r2\n.4byte 0x08000008\nnop\n.align 2\n.4byte 0x08000004\n").unwrap();
        write(&listing, b"1 0000 asm\n2 0000 0000 asm\n3 0002 0000 asm\n4 0004 00000000 asm\n5 0008 0000 asm\n6 000a 00 asm\n7 000c 00000000 asm\n").unwrap();
        let output = symbolize(&source, &listing, "Func_08000000").unwrap();
        assert!(output.contains("ldr r2, .Lm2c_jtbl_ptr_1"));
        assert!(output.contains(".4byte .Lm2c_08000008"));
        assert!(output.contains(".Lm2c_jtbl_ptr_1:"));
        write(&source, b".thumb\n.set sub_08000100, 0x08000100\nHumanOwner:\nRegion_08000000:\nldr r0, [pc, #4]\nbx lr\n").unwrap();
        write(
            &listing,
            b"1 0000 asm\n2 0000 asm\n3 0000 asm\n4 0000 asm\n5 0000 0000 asm\n6 0002 0000 asm\n",
        )
        .unwrap();
        let output = symbolize(&source, &listing, "HumanOwner").unwrap();
        assert!(!output.contains("Region_08000000:"));
        assert!(output.contains(".global sub_08000100"));
        assert!(output.contains("ldr r0, .Lm2c_pool_0008"));
        assert!(output.contains(".Lm2c_pool_0008:\n.4byte 0x"));
        fs::remove_dir_all(directory).unwrap();
    }
    #[test]
    fn m2c_context_is_optional() {
        let plain = m2c_arguments(Path::new("m2c.py"), "Target", Path::new("target.s"), None);
        let contextual = m2c_arguments(
            Path::new("m2c.py"),
            "Target",
            Path::new("target.s"),
            Some(Path::new("exact.i")),
        );
        assert!(!plain.contains(&"--context".into()));
        assert!(contextual
            .windows(2)
            .any(|pair| pair == ["--context", "exact.i"]));
    }
}
