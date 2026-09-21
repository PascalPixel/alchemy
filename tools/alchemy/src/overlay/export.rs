//! `alchemy overlay export`: one code overlay from a target's ROM directory
//! into `resource_XXX_overlay.s`. Nothing is written unless the listing
//! reassembles exactly and automatic compression reproduces the ROM stream.

use crate::overlay::assembly::{export_overlay_source, OVERLAY_BASE, ROM_BASE};
use crate::overlay::compile::assemble_overlay_raw;
use crate::overlay::rom::{leading_entry_veneers, resource_id, CanonicalRom, Stream};
use crate::overlay::source::OverlaySource;
use crate::targets::{decomp_target, DecompTarget};
use serde_json::json;
use std::io::Write;
use std::path::{Path, PathBuf};

pub const USAGE: &str = "usage: alchemy overlay export RESOURCE... [--target GAME-EDITION] [--output DIR]\n       alchemy overlay export --list [--target GAME-EDITION]\nWrites resource_XXX_overlay.s without a compression sidecar (default DIR: the target's raw/overlays)\nand prints each recon/assets.json thumb-overlay series tuple [id, address, size, decoded_size].\n--list prints every resource whose decoded image has the target's entry-veneer shape.";

struct Options {
    target: DecompTarget,
    output: Option<PathBuf>,
    list: bool,
    resources: Vec<String>,
}

fn parse(argv: &[String]) -> Result<Options, String> {
    let mut options = Options {
        target: decomp_target(None)?,
        output: None,
        list: false,
        resources: Vec::new(),
    };
    let mut arguments = argv.iter();
    while let Some(argument) = arguments.next() {
        let mut value = |flag: &str| {
            arguments
                .next()
                .cloned()
                .ok_or_else(|| format!("{flag} needs a value"))
        };
        match argument.as_str() {
            "--target" => options.target = decomp_target(Some(&value("--target")?))?,
            "--output" => options.output = Some(PathBuf::from(value("--output")?)),
            "--list" => options.list = true,
            other if other.starts_with("--") => return Err(format!("unknown flag {other}")),
            other => options.resources.push(other.to_string()),
        }
    }
    if options.list != options.resources.is_empty() {
        return Err(USAGE.into());
    }
    Ok(options)
}

pub fn run(root: &Path, argv: &[String]) -> Result<i32, String> {
    if argv == ["--help"] || argv == ["-h"] {
        println!("{USAGE}");
        return Ok(0);
    }
    let options = parse(argv)?;
    let rom = CanonicalRom::load_target(root, options.target)?;
    if options.list {
        for resource in rom.overlay_resources(options.target.overlay_entry_veneers) {
            let stream = rom.stream(resource)?;
            println!(
                "resource_{resource:03x}\ttag={}\t{}",
                stream.tag(),
                series_tuple(&stream)
            );
        }
        return Ok(0);
    }
    let directory = match &options.output {
        Some(path) if path.is_absolute() => path.clone(),
        Some(path) => root.join(path),
        None => root.join(options.target.overlay_dir()),
    };
    for name in &options.resources {
        let exported = export(root, &rom, options.target, name)?;
        std::fs::create_dir_all(&directory)
            .map_err(|error| format!("{}: {error}", directory.display()))?;
        let overlay = format!("resource_{:03x}", exported.stream.resource);
        write_new(
            &directory.join(format!("{overlay}_overlay.s")),
            exported.source.as_bytes(),
        )?;
        if let Some(note) = exported.note {
            eprintln!("{overlay}: {note}");
        }
        println!(
            "{overlay}\ttarget={}\ttag={}\tlookahead={}\t{}",
            options.target.id,
            exported.stream.tag(),
            exported.lookahead,
            series_tuple(&exported.stream)
        );
    }
    Ok(0)
}

struct Exported {
    stream: Stream,
    source: String,
    lookahead: usize,
    note: Option<String>,
}

fn export(
    root: &Path,
    rom: &CanonicalRom,
    target: DecompTarget,
    name: &str,
) -> Result<Exported, String> {
    let stream = rom.stream(resource_id(name)?)?;
    let veneers = leading_entry_veneers(&stream.decoded);
    if veneers != target.overlay_entry_veneers {
        return Err(format!(
            "{name}: decoded image opens with {veneers} entry veneers; a {} code overlay has {}",
            target.id, target.overlay_entry_veneers
        ));
    }
    let compressed = &rom.bytes()[stream.start..stream.end];
    verify_compression(&stream.decoded, compressed)?;
    let lookahead = compressed.len() - stream.encoded()?.len();

    let veneer_macro = target.overlay_macro();
    let source = export_overlay_source(
        &stream.decoded,
        OVERLAY_BASE,
        &veneer_macro,
        target.overlay_entry_veneers,
    )
    .map_err(|error| format!("{name}: {error}"))?;
    // The macro is one fixed Thumb veneer in both games. A game that has not
    // yet committed its own copy is verified against the default game's.
    let (verified, note) = if root.join(&veneer_macro).is_file() {
        (source.clone(), None)
    } else {
        let fallback = crate::targets::target_for(crate::targets::DEFAULT_TARGET).overlay_macro();
        if !root.join(&fallback).is_file() {
            return Err(format!("{veneer_macro} and {fallback} are both missing"));
        }
        (
            source.replacen(&veneer_macro, &fallback, 1),
            Some(format!(
                "{veneer_macro} does not exist yet; reassembly verified against {fallback}"
            )),
        )
    };
    let assembled = assemble_overlay_raw(&OverlaySource::text(verified), OVERLAY_BASE)?;
    if assembled != stream.decoded {
        let first = assembled
            .iter()
            .zip(&stream.decoded)
            .position(|(left, right)| left != right)
            .unwrap_or(assembled.len().min(stream.decoded.len()));
        return Err(format!(
            "{name}: listing does not reassemble byte-identically (first difference at +0x{first:x}, {} vs {} bytes)",
            assembled.len(),
            stream.decoded.len()
        ));
    }
    Ok(Exported {
        stream,
        source,
        lookahead,
        note,
    })
}

fn verify_compression(decoded: &[u8], expected: &[u8]) -> Result<(), String> {
    if crate::build_assets::encode_overlay_stream(decoded)? != expected {
        return Err("automatic overlay compression or packing differs; export refuses a saved-answer exception".into());
    }
    Ok(())
}

/// `[id, address, size, decoded_size]`, exactly as a recon/assets.json
/// `golden-sun-thumb-overlay-series` lists one resource.
fn series_tuple(stream: &Stream) -> String {
    json!([
        format!("{:03x}", stream.resource),
        format!("0x{:08x}", stream.start as i64 + ROM_BASE),
        format!("0x{:x}", stream.end - stream.start),
        format!("0x{:x}", stream.decoded.len()),
    ])
    .to_string()
}

fn write_new(path: &Path, bytes: &[u8]) -> Result<(), String> {
    std::fs::OpenOptions::new()
        .write(true)
        .create_new(true)
        .open(path)
        .and_then(|mut file| file.write_all(bytes))
        .map_err(|error| format!("{}: {error}", path.display()))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn automatic_export_refuses_padding_and_changed_bytes() {
        let decoded = b"ABABABAB";
        let compressed = crate::build_assets::encode_overlay_stream(decoded).unwrap();
        verify_compression(decoded, &compressed).unwrap();
        let mut padded = compressed.clone();
        padded.push(0);
        assert!(verify_compression(decoded, &padded).is_err());
        let mut changed = compressed;
        changed[0] ^= 1;
        assert!(verify_compression(decoded, &changed).is_err());
    }

    #[test]
    fn export_arguments_require_resources_or_list() {
        let arguments = |items: &[&str]| items.iter().map(|s| s.to_string()).collect::<Vec<_>>();
        assert!(parse(&arguments(&[])).is_err());
        assert!(parse(&arguments(&["--list", "resource_649"])).is_err());
        let options = parse(&arguments(&["resource_649", "--target", "tla-en"])).unwrap();
        assert_eq!(options.target.overlay_entry_veneers, 7);
        assert_eq!(options.resources, ["resource_649"]);
    }
}
