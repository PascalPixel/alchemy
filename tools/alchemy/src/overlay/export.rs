//! `alchemy overlay export`: one code overlay from a target's ROM directory
//! into the retained pair the asset build consumes, `resource_XXX_overlay.s`
//! and `resource_XXX_stream.lz.json`. Nothing is written unless the listing
//! reassembles to the decoded image and the stream plan re-encodes to the
//! ROM's compressed bytes, both exactly.

use crate::overlay::assembly::{export_overlay_source, OVERLAY_BASE, ROM_BASE};
use crate::overlay::compile::assemble_overlay_raw;
use crate::overlay::rom::{leading_entry_veneers, resource_id, CanonicalRom, Stream, Trace};
use crate::overlay::source::OverlaySource;
use crate::targets::{decomp_target, DecompTarget};
use psynergy::assets::lz::{
    encode_general, encode_palette, GeneralToken, PaletteGroup, PaletteOperation,
};
use serde_json::{json, Value};
use std::io::Write;
use std::path::{Path, PathBuf};

pub const USAGE: &str = "usage: alchemy overlay export RESOURCE... [--target GAME-EDITION] [--output DIR]\n       alchemy overlay export --list [--target GAME-EDITION]\nWrites resource_XXX_overlay.s and resource_XXX_stream.lz.json (default DIR: the target's asm/overlays)\nand prints each RESOURCE.JSON thumb-overlay series tuple [id, address, size, decoded_size].\n--list prints every resource whose decoded image has the target's entry-veneer shape.";

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
        write_new(
            &directory.join(format!("{overlay}_stream.lz.json")),
            exported.plan.as_bytes(),
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
    plan: String,
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
    let plan = plan_document(&stream, compressed)?;
    let plan_text = format!(
        "{}\n",
        serde_json::to_string(&plan).map_err(|error| error.to_string())?
    );
    // The written document, read back, must rebuild every compressed byte.
    let reread: Value = serde_json::from_str(&plan_text).map_err(|error| error.to_string())?;
    if encode_document(&stream.decoded, &reread)? != compressed {
        return Err(format!(
            "{name}: stream plan does not re-encode to the ROM bytes"
        ));
    }
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
        plan: plan_text,
        lookahead,
        note,
    })
}

fn plan_document(stream: &Stream, compressed: &[u8]) -> Result<Value, String> {
    let body = stream.encoded()?;
    if !compressed.starts_with(&body) {
        return Err(format!(
            "resource {:03x}: the traced plan does not reproduce the compressed stream",
            stream.resource
        ));
    }
    let lookahead: String = compressed[body.len()..]
        .iter()
        .map(|byte| format!("{byte:02x}"))
        .collect();
    let decoded_size = format!("0x{:x}", stream.decoded.len());
    let mut document = match &stream.trace {
        Trace::General(tokens) => json!({
            "format": 1,
            "codec": "golden-sun-general-lz",
            "decoded_size": decoded_size,
            "lookahead": lookahead,
            "tokens": tokens.iter().map(general_token).collect::<Vec<_>>(),
        }),
        Trace::Palette(groups) => json!({
            "format": 1,
            "codec": "golden-sun-tagged-palette-lz",
            "decoded_size": decoded_size,
            "lookahead": lookahead,
            "tokens": groups.iter().map(palette_group).collect::<Vec<_>>(),
            "tag": 1,
        }),
    };
    // The directory's last stream ends where its decoder stops; no following
    // resource lends it trailing bytes, so its plan carries no lookahead.
    if !stream.bounded {
        if !lookahead.is_empty() {
            return Err(format!(
                "resource {:03x}: the last stream decodes past its encoding",
                stream.resource
            ));
        }
        document
            .as_object_mut()
            .expect("plan is an object")
            .remove("lookahead");
    }
    Ok(document)
}

fn general_token(token: &GeneralToken) -> Value {
    match *token {
        GeneralToken::Literal(count) => json!(["l", count]),
        GeneralToken::Copy { length, distance } => json!(["c", length, distance]),
    }
}

fn palette_group(group: &PaletteGroup) -> Value {
    match group {
        PaletteGroup::Zeros => json!(["z"]),
        PaletteGroup::Group(operations) => json!([
            "g",
            operations
                .iter()
                .map(|operation| match *operation {
                    PaletteOperation::Literal => json!(["l"]),
                    PaletteOperation::End => json!(["e"]),
                    PaletteOperation::Copy { length, distance } => {
                        json!(["c", length, distance])
                    }
                })
                .collect::<Vec<_>>()
        ]),
    }
}

/// Rebuilds the compressed bytes from a plan document alone.
fn encode_document(decoded: &[u8], plan: &Value) -> Result<Vec<u8>, String> {
    let malformed = || "stream plan is malformed".to_string();
    let number = |value: &Value| {
        value
            .as_u64()
            .and_then(|value| u32::try_from(value).ok())
            .ok_or_else(malformed)
    };
    let tokens = plan["tokens"].as_array().ok_or_else(malformed)?;
    if plan["decoded_size"] != format!("0x{:x}", decoded.len()) {
        return Err("stream plan decoded_size differs".into());
    }
    let mut built = match plan["codec"].as_str() {
        Some("golden-sun-general-lz") => {
            let tokens = tokens
                .iter()
                .map(|token| match token.as_array().map(Vec::as_slice) {
                    Some([kind, count]) if kind == "l" => Ok(GeneralToken::Literal(number(count)?)),
                    Some([kind, length, distance]) if kind == "c" => Ok(GeneralToken::Copy {
                        length: number(length)?,
                        distance: number(distance)?,
                    }),
                    _ => Err(malformed()),
                })
                .collect::<Result<Vec<_>, _>>()?;
            encode_general(decoded, &tokens).map_err(|error| error.0)?
        }
        Some("golden-sun-tagged-palette-lz") if plan["tag"] == 1 => {
            let groups = tokens
                .iter()
                .map(|group| match group.as_array().map(Vec::as_slice) {
                    Some([kind]) if kind == "z" => Ok(PaletteGroup::Zeros),
                    Some([kind, operations]) if kind == "g" => operations
                        .as_array()
                        .ok_or_else(malformed)?
                        .iter()
                        .map(|operation| match operation.as_array().map(Vec::as_slice) {
                            Some([kind]) if kind == "l" => Ok(PaletteOperation::Literal),
                            Some([kind]) if kind == "e" => Ok(PaletteOperation::End),
                            Some([kind, length, distance]) if kind == "c" => {
                                Ok(PaletteOperation::Copy {
                                    length: number(length)?,
                                    distance: number(distance)?,
                                })
                            }
                            _ => Err(malformed()),
                        })
                        .collect::<Result<Vec<_>, _>>()
                        .map(PaletteGroup::Group),
                    _ => Err(malformed()),
                })
                .collect::<Result<Vec<_>, _>>()?;
            let mut body = encode_palette(decoded, &groups).map_err(|error| error.0)?;
            body.insert(0, 1);
            body
        }
        _ => return Err(malformed()),
    };
    let lookahead = plan
        .get("lookahead")
        .map_or(Some(""), Value::as_str)
        .ok_or_else(malformed)?;
    if lookahead.len() % 2 != 0 {
        return Err(malformed());
    }
    for at in (0..lookahead.len()).step_by(2) {
        built.push(u8::from_str_radix(&lookahead[at..at + 2], 16).map_err(|_| malformed())?);
    }
    Ok(built)
}

/// `[id, address, size, decoded_size]`, exactly as a RESOURCE.JSON
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
    fn plan_documents_rebuild_both_encodings_with_lookahead() {
        let decoded = b"ABABABAB".to_vec();
        for trace in [
            Trace::General(vec![
                GeneralToken::Literal(2),
                GeneralToken::Copy {
                    length: 6,
                    distance: 2,
                },
            ]),
            Trace::Palette(vec![PaletteGroup::Group(vec![
                PaletteOperation::Literal,
                PaletteOperation::Literal,
                PaletteOperation::Copy {
                    length: 6,
                    distance: 2,
                },
                PaletteOperation::End,
            ])]),
        ] {
            let stream = Stream {
                resource: 0x649,
                start: 0x100,
                end: 0,
                bounded: true,
                decoded: decoded.clone(),
                trace,
            };
            let mut compressed = stream.encoded().unwrap();
            compressed.extend([0xaa, 0xbb]);
            let plan = plan_document(&stream, &compressed).unwrap();
            assert_eq!(plan["lookahead"], "aabb");
            assert_eq!(encode_document(&decoded, &plan).unwrap(), compressed);
            let mut damaged = plan.clone();
            damaged["decoded_size"] = json!("0x9");
            assert!(encode_document(&decoded, &damaged).is_err());
        }
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
