//! Rebuild connected source; retain unfinished ROM material privately and uncredited.
use crate::compiler::{canonical_json::write_canonical, native};
use crate::coverage::proof;
use crate::targets::{decomp_target, BuildSupport, DecompTarget};
use std::path::Path;
use std::process::Command;

const ROM_BASE: u64 = 0x0800_0000;
const USAGE: &str = "usage: alchemy build full [--target tbs-en|tla-en]\nBuild the ordinary source/link rules, compare every linked byte, and retain the\nunwritten remainder from your verified local ROM. Private input grants no credit.";

pub fn run(args: &[String]) -> Result<(), String> {
    if args == ["--help"] || args == ["-h"] {
        println!("{USAGE}");
        return Ok(());
    }
    if args == ["--self-test"] {
        self_test()?;
        println!("self-test=ok");
        return Ok(());
    }
    let target = match args {
        [] => decomp_target(None)?,
        [flag, target] if flag == "--target" => decomp_target(Some(target))?,
        _ => return Err(USAGE.into()),
    };
    let root = crate::compiler::routing::root();
    println!("{}", build(root, target)?);
    Ok(())
}

fn compose(
    reference: &[u8],
    binary: &[u8],
    sections: &[native::Section],
) -> Result<(Vec<u8>, usize), String> {
    let first = sections
        .iter()
        .map(|section| section.load_address)
        .min()
        .ok_or("the source link produced no loaded sections")?;
    let mut rebuilt = reference.to_vec();
    let mut ranges = Vec::new();
    let mut source_bytes = 0usize;
    for section in sections {
        let start = section
            .load_address
            .checked_sub(ROM_BASE)
            .ok_or_else(|| format!("{} has no ROM load address", section.name))?
            as usize;
        let size = usize::try_from(section.size).map_err(|error| error.to_string())?;
        let end = start.checked_add(size).ok_or("section address overflow")?;
        if size == 0 || end > reference.len() {
            return Err(format!("{} lies outside the ROM", section.name));
        }
        if ranges
            .iter()
            .any(|&(left, right)| start < right && left < end)
        {
            return Err("linked sections overlap".into());
        }
        let offset =
            usize::try_from(section.load_address - first).map_err(|error| error.to_string())?;
        let bytes = binary
            .get(offset..offset.checked_add(size).ok_or("binary offset overflow")?)
            .ok_or("linked binary is shorter than its loaded sections")?;
        if reference[start..end] != *bytes {
            let difference = bytes
                .iter()
                .zip(&reference[start..end])
                .position(|(built, original)| built != original)
                .unwrap();
            return Err(format!(
                "{} differs from the reference at 0x{:08x}; no build proof was written",
                section.name,
                section.load_address + difference as u64
            ));
        }
        rebuilt[start..end].copy_from_slice(bytes);
        ranges.push((start, end));
        source_bytes += size;
    }
    Ok((rebuilt, source_bytes))
}

fn build(root: &Path, target: DecompTarget) -> Result<String, String> {
    if target.build_support != BuildSupport::Full {
        return Err(format!(
            "{} has no maintained full-ROM link layout",
            target.id
        ));
    }
    proof::withdraw_full_build(root, target)?;
    let receipt = root.join(format!("{}/reports/verified-code.json", target.output_dir));
    if receipt.exists() {
        std::fs::remove_file(receipt).map_err(|error| error.to_string())?;
    }
    let inputs = proof::identity(root, target.id.as_str())?;
    let reference =
        std::fs::read(root.join(target.rom)).map_err(|error| format!("{}: {error}", target.rom))?;
    crate::text_catalog::verify_reference(root, target.id.as_str(), &reference)?;
    if reference.len() as u64 != target.rom_size {
        return Err("reference ROM has the wrong size".into());
    }
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let status = Command::new("make")
        .current_dir(root)
        .args(["--no-print-directory", "native"])
        .arg(format!("TARGET={}", target.id))
        .arg(format!("ALCHEMY={}", executable.display()))
        .status()
        .map_err(|error| error.to_string())?;
    if !status.success() {
        return Err("maintained source failed to compile or link".into());
    }
    let native_path = root.join(format!("{}/native/native.json", target.output_dir));
    let native: native::Build =
        serde_json::from_slice(&std::fs::read(&native_path).map_err(|error| error.to_string())?)
            .map_err(|error| error.to_string())?;
    let binary = std::fs::read(&native.binary).map_err(|error| error.to_string())?;
    let (mut rebuilt, _) = compose(&reference, &binary, &native.sections)?;
    let options = crate::build_asm::Options {
        target: target.id,
        rom: target.rom.into(),
        output: format!("{}/full/asm", target.output_dir),
        source: None,
        source_only: false,
        asm_dir: target.asm_dir.into(),
    };
    let fallback = crate::build_asm::build(root, root, &options)?;
    let source_bytes = compose_fallback(root, target, &reference, &mut rebuilt, &native.sections)?;
    let output = root.join(proof::full_build_rom(target));
    std::fs::create_dir_all(output.parent().unwrap()).map_err(|error| error.to_string())?;
    std::fs::write(&output, &rebuilt).map_err(|error| error.to_string())?;
    let mut report = proof::record_source_build(root, target, &inputs, &rebuilt, source_bytes)?;
    report["sections"] =
        serde_json::to_value(&native.sections).map_err(|error| error.to_string())?;
    write_canonical(&root.join(proof::full_build_report(target)), &report)?;
    // Source output is compared here; complete extents and the denominator
    // require a fresh independent audit before any progress credit.
    proof::write(root, target.id.as_str(), &rebuilt, &inputs, Vec::new())?;
    Ok(format!("target={}\nbyte_identical=true\nsource_bytes={}\nfallback_regions={}\nprivate_unwritten_bytes={}\nDONE=pending\nrom={}",
        target.id, source_bytes, fallback.regions, reference.len() - source_bytes, output.display()))
}

fn compose_fallback(
    root: &Path,
    target: DecompTarget,
    reference: &[u8],
    rebuilt: &mut [u8],
    native: &[native::Section],
) -> Result<usize, String> {
    let output = root.join(format!("{}/full/asm", target.output_dir));
    let document: serde_json::Value = serde_json::from_slice(
        &std::fs::read(output.join("manifest.json")).map_err(|error| error.to_string())?,
    )
    .map_err(|error| error.to_string())?;
    if document["verification"] != "rom" {
        return Err("assembly fallback has no ROM comparison".into());
    }
    let mut covered = vec![false; reference.len()];
    for section in native {
        let start = (section.load_address - ROM_BASE) as usize;
        covered[start..start + section.size as usize].fill(true);
    }
    for region in document["regions"]
        .as_array()
        .ok_or("assembly build has no regions")?
    {
        let address = region["address"]
            .as_u64()
            .ok_or("assembly region has no address")?;
        let size = region["size"]
            .as_u64()
            .ok_or("assembly region has no size")? as usize;
        let start = address
            .checked_sub(ROM_BASE)
            .ok_or("assembly region precedes ROM")? as usize;
        let end = start.checked_add(size).ok_or("assembly region overflows")?;
        let source = output.join(format!("{address:08x}.bin"));
        let bytes = std::fs::read(&source).map_err(|error| error.to_string())?;
        if bytes.len() != size || reference.get(start..end) != Some(bytes.as_slice()) {
            return Err(format!("{} differs from the ROM", source.display()));
        }
        rebuilt[start..end].copy_from_slice(&bytes);
        covered[start..end].fill(true);
    }
    Ok(covered.iter().filter(|byte| **byte).count())
}

pub fn self_test() -> Result<(), String> {
    let section = native::Section {
        name: ".text".into(),
        size: 2,
        address: ROM_BASE + 2,
        load_address: ROM_BASE + 2,
    };
    if compose(&[0, 0, 1, 2], &[1, 3], &[section.clone()]).is_ok()
        || compose(&[0, 0, 1, 2], &[1, 2], &[section.clone(), section.clone()]).is_ok()
    {
        return Err("source comparison accepted a mismatch or overlap".into());
    }
    let (built, count) = compose(&[0, 0, 1, 2], &[1, 2], &[section])?;
    if built != [0, 0, 1, 2] || count != 2 {
        return Err("private and source composition disagrees".into());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    #[test]
    fn source_mismatch_and_overlap_cannot_leave_a_proof() {
        super::self_test().unwrap();
    }
}
