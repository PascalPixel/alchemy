//! Bounded readers for data the maintained runtime actually consumes.
//!
//! Roots come from ordinary source declarations and getter mechanisms. These
//! readers classify data, never reconstructed-source credit or compiler output.
//! Failed reads leave holes; a ROM pointer never supplies an arbitrary extent.

use super::model::Span;
use crate::build_assets::{read_sprite_frame, SpriteCatalog};
use crate::compiler::routing::CompilerTarget;
use crate::overlay::rom::{resource_table, CanonicalRom};
use crate::targets::DecompTarget;
use regex::Regex;
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};

const BASE: usize = 0x0800_0000;
const DESCRIPTOR_BYTES: usize = 20;
const DIRECTORY_LIMIT: usize = 4096;
const SCRIPT_LIMIT: usize = 0x10000;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) enum Kind {
    SpriteDescriptors,
    AnimationDirectory,
    FrameDirectory,
    AnimationScript,
    SpriteFrame,
    Delta7Image,
    MapHeader,
    MapComponent,
    SongEntry,
    SequenceHeader,
    Voice,
    VoiceKeyMap,
    PcmWave,
    CgbWave,
}

#[derive(Debug)]
pub(crate) struct Range {
    pub span: Span,
    pub kind: Kind,
    pub source: PathBuf,
}

#[derive(Debug, Default)]
pub(crate) struct Read {
    pub ranges: Vec<Range>,
    pub issues: Vec<String>,
}

impl Read {
    fn add(&mut self, start: usize, size: usize, kind: Kind, source: &Path) -> Result<(), String> {
        let end = start
            .checked_add(size)
            .ok_or("typed data extent overflow")?;
        if size == 0 {
            return Ok(());
        }
        self.ranges.push(Range {
            span: Span::new(start as i64, end as i64),
            kind,
            source: source.to_owned(),
        });
        Ok(())
    }
}

pub(crate) fn read(root: &Path, target: DecompTarget, rom: &CanonicalRom) -> Read {
    let mut result = Read::default();
    if let Err(error) = sprites(root, target, rom.bytes(), &mut result) {
        result.issues.push(format!("sprite data: {error}"));
    }
    if let Err(error) = sound(root, target, rom.bytes(), &mut result) {
        result.issues.push(format!("sound data: {error}"));
    }
    if let Err(error) = delta7_images(root, target, rom, &mut result) {
        result.issues.push(format!("delta7 data: {error}"));
    }
    if let Err(error) = maps(root, target, rom, &mut result) {
        result.issues.push(format!("map data: {error}"));
    }
    result
}

fn source(root: &Path, path: &Path) -> Result<String, String> {
    let canonical_root = std::fs::canonicalize(root).map_err(|error| error.to_string())?;
    let canonical = std::fs::canonicalize(root.join(path))
        .map_err(|error| format!("{}: {error}", path.display()))?;
    if path.is_absolute()
        || !canonical.starts_with(&canonical_root)
        || canonical.starts_with(canonical_root.join("out"))
        || canonical.starts_with(canonical_root.join("tools/out"))
    {
        return Err("data reader needs an ordinary repository source".into());
    }
    std::fs::read_to_string(canonical).map_err(|error| error.to_string())
}

fn bytes(rom: &[u8], address: usize, size: usize) -> Result<&[u8], String> {
    let start = address
        .checked_sub(BASE)
        .ok_or("data pointer precedes ROM")?;
    let end = start.checked_add(size).ok_or("data pointer overflow")?;
    rom.get(start..end)
        .ok_or_else(|| format!("data at {address:#x}, {size} bytes, exceeds ROM"))
}

fn word(rom: &[u8], address: usize) -> Result<usize, String> {
    Ok(u32::from_le_bytes(bytes(rom, address, 4)?.try_into().unwrap()) as usize)
}

fn pointer(rom: &[u8], address: usize) -> Result<usize, String> {
    let pointer = word(rom, address)?;
    bytes(rom, pointer, 1)?;
    Ok(pointer)
}

fn capture(source: &str, pattern: &str) -> Result<String, String> {
    Regex::new(pattern)
        .unwrap()
        .captures(source)
        .and_then(|matched| matched.get(1))
        .map(|value| value.as_str().to_owned())
        .ok_or_else(|| "maintained getter/declaration shape is not established".into())
}

fn number(value: &str) -> Result<usize, String> {
    value
        .strip_prefix("0x")
        .map_or_else(|| value.parse(), |hex| usize::from_str_radix(hex, 16))
        .map_err(|_| "source number is invalid".into())
}

fn literal(rom: &[u8], address: usize) -> Result<usize, String> {
    let instruction = u16::from_le_bytes(bytes(rom, address, 2)?.try_into().unwrap());
    if instruction & 0xf800 != 0x4800 {
        return Err("getter does not load a PC-relative word".into());
    }
    let pool = address.checked_add(4).ok_or("getter PC overflow")? & !3;
    word(
        rom,
        pool.checked_add(usize::from(instruction & 255) * 4)
            .ok_or("getter literal overflow")?,
    )
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
struct Value {
    factor: usize,
    mask: usize,
    constant: usize,
}

impl Value {
    fn constant(value: usize) -> Self {
        Self {
            factor: 0,
            mask: usize::MAX,
            constant: value,
        }
    }
    fn add(self, other: Self) -> Result<Self, String> {
        let mask = match (self.factor != 0, other.factor != 0) {
            (true, true) if self.mask != other.mask => {
                return Err("getter adds differently masked indices".into())
            }
            (_, true) => other.mask,
            _ => self.mask,
        };
        Ok(Self {
            factor: self
                .factor
                .checked_add(other.factor)
                .ok_or("getter factor overflow")?,
            mask,
            constant: self
                .constant
                .checked_add(other.constant)
                .ok_or("getter constant overflow")?,
        })
    }
}

/// Symbolically read the small TBS descriptor indexer: literal loads, register
/// moves/adds, mask and shifts must establish exactly the maintained C index.
/// Its table pointer is read from the actual PC-relative load, not a saved root.
fn tbs_indexer(rom: &[u8], start: usize, mask: usize, stride: usize) -> Result<usize, String> {
    let mut registers = [None; 8];
    registers[0] = Some(Value {
        factor: 1,
        mask: usize::MAX,
        constant: 0,
    });
    for index in 0..32 {
        let pc = start
            .checked_add(index * 2)
            .ok_or("getter address overflow")?;
        let half = u16::from_le_bytes(bytes(rom, pc, 2)?.try_into().unwrap());
        let destination = usize::from(half & 7);
        let input = usize::from((half >> 3) & 7);
        let get = |register: usize| {
            registers[register].ok_or_else(|| "getter reads an unknown register".to_owned())
        };
        if half & 0xf800 == 0x4800 {
            registers[usize::from((half >> 8) & 7)] = Some(Value::constant(literal(rom, pc)?));
        } else if half & 0xf800 == 0 {
            let shift = usize::from((half >> 6) & 31);
            let value = get(input)?;
            registers[destination] = Some(Value {
                factor: value
                    .factor
                    .checked_shl(shift as u32)
                    .ok_or("getter scale overflow")?,
                mask: value.mask,
                constant: value
                    .constant
                    .checked_shl(shift as u32)
                    .ok_or("getter constant scale overflow")?,
            });
        } else if half & 0xfa00 == 0x1800 {
            let left = get(input)?;
            let right = if half & 0x0400 != 0 {
                Value::constant(usize::from((half >> 6) & 7))
            } else {
                get(usize::from((half >> 6) & 7))?
            };
            registers[destination] = Some(left.add(right)?);
        } else if half & 0xffc0 == 0x4000 {
            let left = get(destination)?;
            let right = get(input)?;
            if left.factor != 1 || left.constant != 0 || right.factor != 0 {
                return Err("getter mask is not a constant index mask".into());
            }
            registers[destination] = Some(Value {
                mask: left.mask & right.constant,
                ..left
            });
        } else if half == 0x4770 {
            let result = get(0)?;
            if result.factor != stride || result.mask != mask || result.constant < BASE {
                return Err("ROM getter differs from the maintained descriptor index".into());
            }
            bytes(rom, result.constant, DESCRIPTOR_BYTES)?;
            return Ok(result.constant);
        } else {
            return Err(format!(
                "descriptor getter has an unsupported instruction at {pc:#x}"
            ));
        }
    }
    Err("descriptor getter does not return within its bounded walk".into())
}

fn sprite_root(
    root: &Path,
    target: DecompTarget,
    rom: &[u8],
) -> Result<(usize, usize, PathBuf), String> {
    match target.compiler {
        CompilerTarget::Tbs => {
            let veneer = PathBuf::from(format!("{}/SYSTEM/FAR_CALL/RESOURCE.S", target.source_dir));
            let text = source(root, &veneer)?;
            let address =
                usize::from_str_radix(&capture(&text, r"(?m)^Func_([0-9a-fA-F]{8}):\s*$")?, 16)
                    .map_err(|_| "veneer source address is invalid")?;
            let getter = number(&capture(
                &text,
                r"(?m)^\s*\.irp\s+target,\s*(0x[0-9a-fA-F]+)\s*$",
            )?)?;
            if bytes(rom, address, 4)? != [0, 0x4c, 0x20, 0x47]
                || word(rom, address + 4)? != getter
                || getter & 1 == 0
            {
                return Err("maintained descriptor veneer differs from the ROM".into());
            }
            let input = PathBuf::from(format!(
                "{}/SYSTEM/RESOURCE/METADATA_GET_RECORD.C",
                target.source_dir
            ));
            let text = source(root, &input)?;
            let matched = Regex::new(r"Character_DescriptorTable\[\(arg0\s*&\s*(0x[0-9a-fA-F]+|[0-9]+)\)\s*\*\s*([0-9]+)\]").unwrap().captures(&text).ok_or("maintained descriptor C getter is unrecognized")?;
            let mask = number(&matched[1])?;
            let stride = number(&matched[2])?;
            if stride != DESCRIPTOR_BYTES || mask >= DIRECTORY_LIMIT {
                return Err("descriptor getter stride differs from its typed record".into());
            }
            let table = tbs_indexer(rom, getter & !1, mask, stride)?;
            let bound = BASE + resource_table(rom)?;
            let bounded = &rom[..bound - BASE];
            let mut first_script = bound;
            let mut count = 0usize;
            for index in 0..=mask {
                let at = table
                    .checked_add(
                        index
                            .checked_mul(stride)
                            .ok_or("descriptor table overflow")?,
                    )
                    .ok_or("descriptor table overflow")?;
                if at >= first_script {
                    break;
                }
                let catalog = SpriteCatalog {
                    game: target.compiler,
                    table: at,
                    count: 1,
                    palette: 0,
                };
                let descriptor = catalog.descriptors(bounded)?.remove(0);
                for item in 0..descriptor.animation_count {
                    let script = pointer(bounded, descriptor.animation_table + item * 4)?;
                    if script <= at || script >= bound {
                        return Err(
                            "descriptor animation pointer does not bound the typed bank".into()
                        );
                    }
                    first_script = first_script.min(script);
                }
                count += 1;
            }
            if count == 0 || table.checked_add(count * stride) != Some(first_script) {
                return Err("descriptor table has no exact first-script boundary".into());
            }
            Ok((table, count, input))
        }
        CompilerTarget::Tla => {
            let input = PathBuf::from(format!("{}/raw/08021918.s", target.recon_dir()));
            let text = source(root, &input)?;
            let start =
                usize::from_str_radix(&capture(&text, r"(?m)^Func_([0-9a-fA-F]{8}):\s*$")?, 16)
                    .map_err(|_| "getter source address is invalid")?;
            let matched = Regex::new(
                r"movs\s+r3,\s*#([0-9]+)\s*\n\s*lsls\s+r3,\s*r3,\s*#([0-9]+)\s*\n\s*cmp\s+r2,\s*r3",
            )
            .unwrap()
            .captures(&text)
            .ok_or("maintained descriptor clamp is unrecognized")?;
            let count = number(&matched[1])?
                .checked_shl(number(&matched[2])? as u32)
                .ok_or("descriptor clamp overflow")?;
            if count == 0 || count > DIRECTORY_LIMIT {
                return Err("descriptor clamp is outside the bounded format".into());
            }
            // Read the table from the actual instruction named by the
            // maintained raw getter, after its id-mask/clamp and x20 scale.
            let prefix = text
                .split("\tldr\tr3, [pc,")
                .next()
                .ok_or("getter has no table load")?;
            let instructions = prefix
                .lines()
                .filter(|line| {
                    let text = line.trim();
                    !text.is_empty()
                        && !text.starts_with('.')
                        && !text.ends_with(':')
                        && !text.starts_with('@')
                })
                .count();
            let pc = start
                .checked_add(instructions * 2)
                .ok_or("getter instruction address overflow")?;
            let table = literal(rom, pc)?;
            bytes(
                rom,
                table,
                count
                    .checked_mul(DESCRIPTOR_BYTES)
                    .ok_or("descriptor clamp size overflow")?,
            )?;
            Ok((table, count, input))
        }
    }
}

fn sprites(root: &Path, target: DecompTarget, rom: &[u8], result: &mut Read) -> Result<(), String> {
    let (table, count, input) = sprite_root(root, target, rom)?;
    let end = BASE + resource_table(rom)?;
    let bounded = &rom[..end - BASE];
    let catalog = SpriteCatalog {
        game: target.compiler,
        table,
        count,
        palette: 0,
    };
    let descriptors = catalog.descriptors(bounded)?;
    let directories = catalog.directories(bounded, &descriptors)?;
    result.add(
        table,
        count
            .checked_mul(DESCRIPTOR_BYTES)
            .ok_or("descriptor extent overflow")?,
        Kind::SpriteDescriptors,
        &input,
    )?;
    let mut scripts = BTreeSet::new();
    for descriptor in &descriptors {
        if descriptor.animation_count != 0 {
            let size = descriptor
                .animation_count
                .checked_mul(4)
                .ok_or("animation directory overflow")?;
            bytes(bounded, descriptor.animation_table, size)?;
            result.add(
                descriptor.animation_table,
                size,
                Kind::AnimationDirectory,
                &input,
            )?;
            for index in 0..descriptor.animation_count {
                scripts.insert(pointer(bounded, descriptor.animation_table + index * 4)?);
            }
        }
    }
    for (&address, slots) in &directories {
        let size = (slots.len() + usize::from(target.compiler == CompilerTarget::Tbs))
            .checked_mul(4)
            .ok_or("frame directory overflow")?;
        bytes(bounded, address, size)?;
        for &frame in slots {
            bytes(bounded, frame, 1)?;
        }
        result.add(address, size, Kind::FrameDirectory, &input)?;
    }
    let (commands, terminals) = if target.compiler == CompilerTarget::Tbs {
        let commands = PathBuf::from(format!(
            "{}/SYSTEM/RESOURCE/METADATA_SUM_COMMAND_LENGTHS.C",
            target.source_dir
        ));
        let text = source(root, &commands)?;
        let stops = capture(&text, r"(?s)if\s*\((op\s*==[^{}]+)\)\s*\{\s*break;")?;
        let terminals = Regex::new(r"op\s*==\s*([0-9]+)")
            .unwrap()
            .captures_iter(&stops)
            .map(|matched| {
                number(&matched[1]).and_then(|value| {
                    u8::try_from(value).map_err(|_| "script stop opcode overflow".into())
                })
            })
            .collect::<Result<BTreeSet<_>, _>>()?;
        if !text.contains("p += 2") || terminals.is_empty() {
            return Err("maintained animation command width/terminators are unrecognized".into());
        }
        (commands, terminals)
    } else {
        let commands = PathBuf::from(format!("{}/raw/08022d1c.s", target.recon_dir()));
        let text = source(root, &commands)?;
        if !text.contains("ldrb\tr3, [r0, #5]")
            || !text.contains("ldr\tr2, [r0, #16]")
            || !text.contains("ldrb\tr2, [r0, #0]")
            || !text.contains("ldrb\tr3, [r0, #1]")
            || !text.contains("adds\tr0, #2")
        {
            return Err("maintained raw animation reader fields are unrecognized".into());
        }
        let mut exits = BTreeMap::<String, BTreeSet<u8>>::new();
        for matched in
            Regex::new(r"(?m)^\s*cmp\s+r2,\s*#([0-9]+)\s*\n\s*beq(?:\.n)?\s+(\.L_[0-9a-fA-F]+)\s*$")
                .unwrap()
                .captures_iter(&text)
        {
            exits.entry(matched[2].to_owned()).or_default().insert(
                u8::try_from(number(&matched[1])?).map_err(|_| "animation stop opcode overflow")?,
            );
        }
        let mut exits = exits.into_values().filter(|values| values.len() == 4);
        let terminals = exits
            .next()
            .ok_or("maintained raw animation stops are unrecognized")?;
        if exits.next().is_some() {
            return Err("maintained raw animation exit is ambiguous".into());
        }
        (commands, terminals)
    };
    for script in scripts {
        match script_extent(bounded, script, &terminals) {
            Ok(size) => result.add(script, size, Kind::AnimationScript, &commands)?,
            Err(error) => result.issues.push(error),
        }
    }
    let mut seen = BTreeSet::new();
    for descriptor in &descriptors {
        let Some(slots) = directories.get(&descriptor.frame_directory) else {
            continue;
        };
        for &frame in slots {
            if !seen.insert((
                frame,
                descriptor.frame_codec,
                descriptor.width,
                descriptor.height,
            )) {
                continue;
            }
            match read_sprite_frame(
                bounded,
                target.compiler,
                descriptor.frame_codec,
                frame,
                descriptor.width,
                descriptor.height,
            ) {
                Ok((_, size)) => result.add(frame, size, Kind::SpriteFrame, &input)?,
                Err(error) => result.issues.push(error),
            }
        }
    }
    Ok(())
}

fn script_extent(rom: &[u8], start: usize, terminals: &BTreeSet<u8>) -> Result<usize, String> {
    for index in 0..SCRIPT_LIMIT / 2 {
        let at = start
            .checked_add(index * 2)
            .ok_or("animation script overflow")?;
        let command = bytes(rom, at, 2)?;
        if terminals.contains(&command[0]) {
            return Ok(index * 2 + 2);
        }
    }
    Err(format!(
        "animation script {start:#x} lacks a bounded source-defined terminator"
    ))
}

fn delta7_pixels(text: &str) -> Result<usize, String> {
    if !Regex::new(r"\bmov\s+fp,\s*#7\b").unwrap().is_match(text) {
        return Err("maintained image decoder does not establish seven-bit pixels".into());
    }
    let inner = capture(
        text,
        r"(?s)\bmov\s+r7,\s*#[0-9]+\s*\n[^\n]+:\s*\n(.*?)\bsubs\s+r7,\s*r7,\s*#1",
    )?;
    let calls = Regex::new(r"(?m)^\s*bl\s+(\w+)\s*$")
        .unwrap()
        .captures_iter(&inner)
        .map(|matched| matched[1].to_owned())
        .collect::<Vec<_>>();
    if calls.len() != 8 || calls.iter().any(|name| name != &calls[0]) {
        return Err("maintained image loop is not eight calls of one pixel reader".into());
    }
    let mut count = calls.len();
    for register in [9, 8, 7] {
        let matched = Regex::new(&format!(
            r"\bmov\s+r{register},\s*#([0-9]+)\s*\n(?:\s*ldr\s+sl,\s*\[pc,\s*#[0-9]+\]\s*\n)?\s*([.\w]+):"
        ))
        .unwrap()
        .captures(text)
        .ok_or("maintained image loop has no established entry")?;
        let loops = number(&matched[1])?;
        let back_edge = capture(
            text,
            &format!(r"\bsubs\s+r{register},\s*r{register},\s*#1\s*\n\s*bne\s+([.\w]+)"),
        )?;
        if back_edge != matched[2] {
            return Err("maintained image loop does not return to its established entry".into());
        }
        count = count
            .checked_mul(loops)
            .ok_or("image loop count overflow")?;
    }
    if count == 0 || count > 0x20000 {
        return Err("maintained image loop exceeds the bounded pixel reader".into());
    }
    Ok(count)
}

fn delta7_extent(package: &[u8], palette: usize, pixels: usize) -> Result<usize, String> {
    if pixels == 0 || pixels > 0x20000 {
        return Err("image exceeds the bounded pixel reader".into());
    }
    let colours = package.get(..palette).ok_or("image palette is truncated")?;
    if palette == 0 || palette % 2 != 0 || colours.chunks_exact(2).any(|word| word[1] & 128 != 0) {
        return Err("image palette is not complete BGR555 colours".into());
    }
    let stored = &package[palette..];
    let decoded = psynergy::assets::compression::decode_delta7(stored, pixels)
        .map_err(|error| error.to_string())?;
    let encoded = psynergy::assets::compression::encode_delta7(&decoded)
        .map_err(|error| error.to_string())?;
    if stored.get(..encoded.len()) != Some(encoded.as_slice())
        || stored.len().saturating_sub(encoded.len()) > 3
    {
        return Err("image does not have an exact bounded delta7 inverse".into());
    }
    palette
        .checked_add(encoded.len())
        .ok_or_else(|| "image extent overflow".into())
}

fn delta7_images(
    root: &Path,
    target: DecompTarget,
    rom: &CanonicalRom,
    result: &mut Read,
) -> Result<(), String> {
    let (loader, decoder, palette_pattern) = match target.compiler {
        CompilerTarget::Tbs => (
            PathBuf::from(format!("{}/raw/080c08ec.s", target.recon_dir())),
            PathBuf::from(format!("{}/GRAPHICS/TILE/DECODE.S", target.source_dir)),
            r"(?s)\bmovs\s+r0,\s*#([0-9]+)\s*\n\s*ldr\s+r2,\s*\[pc,\s*#[0-9]+\]\s*\n\s*lsls\s+r0,\s*r0,\s*#([0-9]+)\s*\n\s*ldr\s+r3,\s*\[r2,\s*#20\]\s*\n\s*ldr\s+r1,\s*\[pc,\s*#[0-9]+\]\s*\n\s*add\s+r0,\s*r8\s*\n\s*bl\s+\w+",
        ),
        CompilerTarget::Tla => (
            PathBuf::from(format!("{}/raw/081a04d0.s", target.recon_dir())),
            PathBuf::from(format!(
                "{}/BATTLE/EFFECT/SENTOU_KOUKA_TENKAI.S",
                target.source_dir
            )),
            r"\bmovs\s+r2,\s*#([0-9]+)\s*\n\s*lsls\s+r2,\s*r2,\s*#([0-9]+)\s*\n\s*adds\s+r0,\s*r7,\s*r2",
        ),
    };
    let text = source(root, &loader)?;
    let matched = Regex::new(palette_pattern)
        .unwrap()
        .captures(&text)
        .ok_or("maintained loader does not establish the palette-to-stream displacement")?;
    let shift =
        u32::try_from(number(&matched[2])?).map_err(|_| "image palette displacement overflow")?;
    let palette = number(&matched[1])?
        .checked_shl(shift)
        .ok_or("image palette displacement overflow")?;
    if palette == 0 || palette > 512 || palette % 2 != 0 {
        return Err("maintained loader palette displacement is invalid".into());
    }
    let count = delta7_pixels(&source(root, &decoder)?)?;
    for (start, end) in resource_bounds(rom)? {
        if let Ok(size) = delta7_extent(&rom.bytes()[start..end], palette, count) {
            result.add(BASE + start, size, Kind::Delta7Image, &loader)?;
        }
    }
    Ok(())
}

fn resource_bounds(rom: &CanonicalRom) -> Result<Vec<(usize, usize)>, String> {
    let starts = (0..rom.resource_count())
        .map(|resource| rom.resource_pointer(resource))
        .collect::<Result<Vec<_>, _>>()?;
    let mut boundaries = starts.iter().copied().collect::<BTreeSet<_>>();
    boundaries.insert(rom.bytes().len());
    starts
        .into_iter()
        .skip(2)
        .collect::<BTreeSet<_>>()
        .into_iter()
        .map(|start| {
            boundaries
                .range(start + 1..)
                .next()
                .copied()
                .map(|end| (start, end))
                .ok_or_else(|| "resource has no upper bound".into())
        })
        .collect()
}

#[derive(Debug, PartialEq, Eq)]
struct MapComponent {
    field: usize,
    decoded: bool,
    optional: bool,
}

fn map_components(text: &str, base: &str, getter: &str) -> Result<Vec<MapComponent>, String> {
    let body = capture(
        text,
        &format!(
            r"(?s)\bbl\s+{getter}\s*\n\s*adds\s+{base},\s*r0,\s*#0\s*\n(\s*ldr\s+r3,\s*\[{base},\s*#[0-9]+\].*?)\bldrb\s+r3,\s*\[{base},\s*#0\]"
        ),
    )?;
    let decoder = capture(
        &body,
        &format!(r"\badds\s+r0,\s*{base},\s*r3\s*\n\s*bl\s+(\w+)"),
    )?;
    let fields = Regex::new(&format!(r"\bldr\s+(r[0-9]+),\s*\[{base},\s*#([0-9]+)\]"))
        .unwrap()
        .captures_iter(&body)
        .map(|matched| {
            (
                matched.get(0).unwrap().start(),
                matched[1].to_owned(),
                number(&matched[2]),
            )
        })
        .collect::<Vec<_>>();
    let mut components = Vec::new();
    for (index, (at, register, field)) in fields.iter().enumerate() {
        let end = fields.get(index + 1).map_or(body.len(), |next| next.0);
        let part = &body[*at..end];
        let decoded = Regex::new(&format!(r"\bbl\s+{}\b", regex::escape(&decoder)))
            .unwrap()
            .is_match(part);
        if decoded
            && !Regex::new(&format!(r"\badds\s+r0,\s*{base},\s*{register}\b"))
                .unwrap()
                .is_match(part)
        {
            return Err("map component is not decoded relative to its container".into());
        }
        let optional = !decoded
            || Regex::new(&format!(r"\bcmp\s+{register},\s*#0\b"))
                .unwrap()
                .is_match(part);
        components.push(MapComponent {
            field: field.clone()?,
            decoded,
            optional,
        });
    }
    if components.len() > 16
        || components.iter().filter(|field| field.decoded).count() < 3
        || components
            .first()
            .is_none_or(|field| field.field == 0 || field.field % 4 != 0)
        || components
            .windows(2)
            .any(|pair| pair[0].field.checked_add(4) != Some(pair[1].field))
    {
        return Err("maintained map component directory is not established".into());
    }
    Ok(components)
}

fn map_ranges(
    package: &[u8],
    components: &[MapComponent],
) -> Result<Vec<(usize, usize, Kind)>, String> {
    let header = components
        .last()
        .and_then(|field| field.field.checked_add(4))
        .ok_or("map header extent overflow")?;
    let mut offsets = Vec::new();
    for component in components {
        let end = component
            .field
            .checked_add(4)
            .ok_or("map field extent overflow")?;
        let raw = package
            .get(component.field..end)
            .ok_or("map header is truncated")?;
        let offset = u32::from_le_bytes(raw.try_into().unwrap()) as usize;
        if (offset == 0 && !component.optional)
            || (offset != 0 && (offset < header || offset >= package.len()))
        {
            return Err("map component pointer is outside its container".into());
        }
        offsets.push(offset);
    }
    if offsets.first().copied() != Some(header) {
        return Err("map payload does not begin after its actual directory".into());
    }
    let boundaries = offsets
        .iter()
        .copied()
        .filter(|offset| *offset != 0)
        .chain([package.len()])
        .collect::<BTreeSet<_>>();
    let mut ranges = vec![(0, header, Kind::MapHeader)];
    for (component, offset) in components.iter().zip(offsets) {
        if !component.decoded || offset == 0 {
            continue;
        }
        let end = *boundaries
            .range(offset + 1..)
            .next()
            .ok_or("map component has no upper bound")?;
        let (_, size) = crate::build_assets::tagged_extent(package, offset, end)?;
        ranges.push((offset, size, Kind::MapComponent));
    }
    Ok(ranges)
}

fn maps(
    root: &Path,
    target: DecompTarget,
    rom: &CanonicalRom,
    result: &mut Read,
) -> Result<(), String> {
    let (file, register, getter) = match target.compiler {
        CompilerTarget::Tbs => ("0800fb38.s", "r5", "sub_08002f40"),
        CompilerTarget::Tla => ("08027e20.s", "r7", "sub_08013300"),
    };
    let input = PathBuf::from(format!("{}/raw/{file}", target.recon_dir()));
    let components = map_components(&source(root, &input)?, register, getter)?;
    for (start, end) in resource_bounds(rom)? {
        if let Ok(ranges) = map_ranges(&rom.bytes()[start..end], &components) {
            for (offset, size, kind) in ranges {
                result.add(BASE + start + offset, size, kind, &input)?;
            }
        }
    }
    Ok(())
}

fn song_table_getter(text: &str, rom: &[u8]) -> Result<usize, String> {
    let matched = Regex::new(
        r"(?m)^(\.L_[0-9a-fA-F]+):\s*\n\s*movs\s+r3,\s*#([0-9]+)\s*\n\s*lsls\s+r3,\s*r3,\s*#([0-9]+)\s*\n\s*cmp\s+r6,\s*r3\s*\n\s*bge(?:\.n)?\s+\.L_[0-9a-fA-F]+\s*\n\s*cmp\s+r6,\s*#99\s*\n\s*ble(?:\.n)?\s+\.L_[0-9a-fA-F]+\s*\n\s*ldr\s+r7,\s*\[pc,\s*#([0-9]+)\]\s*\n\s*lsls\s+r4,\s*r6,\s*#3\s*\n\s*adds\s+r3,\s*r4,\s*#4\s*\n\s*ldrh\s+r2,\s*\[r7,\s*r3\]",
    )
    .unwrap()
    .captures(text)
    .ok_or("maintained sound reader does not establish eight-byte song entries")?;
    if !text.contains("ldr\tr1, [r7, r4]") {
        return Err("maintained sound reader does not load the song header".into());
    }
    let start = usize::from_str_radix(matched[1].trim_start_matches(".L_"), 16)
        .map_err(|_| "sound reader label is invalid")?;
    let limit = number(&matched[2])?;
    let shift = number(&matched[3])?;
    let displacement = number(&matched[4])?;
    if limit > 255 || shift > 31 || displacement % 4 != 0 || displacement / 4 > 255 {
        return Err("sound reader instruction operand is invalid".into());
    }
    let instructions = [
        (0xffff, 0x2300 | limit as u16),
        (0xffff, ((shift as u16) << 6) | (3 << 3) | 3),
        (0xffff, 0x429e),
        (0xff00, 0xda00),
        (0xffff, 0x2e63),
        (0xff00, 0xdd00),
        (0xffff, 0x4f00 | (displacement / 4) as u16),
        (0xffff, 0x00f4),
        (0xffff, 0x1d23),
        (0xffff, 0x5afa),
    ];
    for (index, (mask, expected)) in instructions.into_iter().enumerate() {
        let actual = u16::from_le_bytes(bytes(rom, start + index * 2, 2)?.try_into().unwrap());
        if actual & mask != expected {
            return Err(
                "song reader's actual ROM instructions differ from maintained source".into(),
            );
        }
    }
    let table = literal(rom, start + 12)?;
    bytes(rom, table, 8)?;
    Ok(table)
}

fn sound_root(root: &Path, target: DecompTarget, rom: &[u8]) -> Result<(usize, PathBuf), String> {
    match target.compiler {
        CompilerTarget::Tbs => Err(
            "song table root needs ordinary linked source or a retained reader's actual literal"
                .into(),
        ),
        CompilerTarget::Tla => {
            let input = PathBuf::from(format!("{}/raw/081c0c1c.s", target.recon_dir()));
            let text = source(root, &input)?;
            Ok((song_table_getter(&text, rom)?, input))
        }
    }
}

fn sound(root: &Path, target: DecompTarget, rom: &[u8], result: &mut Read) -> Result<(), String> {
    let (table, input) = sound_root(root, target, rom)?;
    let definitions = Path::new("games/COMMON/INCLUDE/SOUND/AUDIO_ENGINE_TYPES.H");
    let text = source(root, definitions)?;
    if !text.contains("struct SongEntry")
        || !text.contains("struct SequenceHeader")
        || !text.contains("struct SoundVoice")
    {
        return Err("maintained sound record definitions are missing".into());
    }
    let mut banks = BTreeSet::new();
    let mut headers = BTreeMap::new();
    // The source's low-12-bit cue selects an eight-byte SongEntry. A malformed
    // row ends this typed prefix, and never classifies the following bytes.
    for index in 0..DIRECTORY_LIMIT {
        let at = table.checked_add(index * 8).ok_or("song table overflow")?;
        let Ok(row) = bytes(rom, at, 8) else { break };
        let Ok(header) = pointer(rom, at) else { break };
        let Ok(data) = bytes(rom, header, 8) else {
            break;
        };
        let tracks = usize::from(data[0]);
        if u16::from_le_bytes(row[4..6].try_into().unwrap()) >= 8 || tracks > 16 {
            break;
        }
        let size = 8 + tracks * 4;
        if bytes(rom, header, size).is_err()
            || (0..tracks).any(|track| pointer(rom, header + 8 + track * 4).is_err())
        {
            break;
        }
        result.add(at, 8, Kind::SongEntry, &input)?;
        headers.insert(header, size);
        if let Ok(bank) = pointer(rom, header + 4) {
            banks.insert(bank);
        }
    }
    for (header, size) in headers {
        result.add(header, size, Kind::SequenceHeader, definitions)?;
    }
    let handle = PathBuf::from(format!(
        "{}/SOUND/{}HANDLE_NOTE.S",
        target.source_dir,
        if target.compiler == CompilerTarget::Tbs {
            "DRIVER/"
        } else {
            ""
        }
    ));
    source(root, &handle)?;
    let mut pending = banks.into_iter().collect::<Vec<_>>();
    let mut visited = BTreeSet::new();
    while let Some(bank) = pending.pop() {
        if !visited.insert(bank) {
            continue;
        }
        if visited.len() > DIRECTORY_LIMIT {
            return Err("sound voice graph exceeds its bounded reader".into());
        }
        // Program and key bytes are seven-bit driver indices. Nested voices
        // use the same twelve-byte structure; cycles are traversed once.
        for voice in 0..128 {
            let size = psynergy::assets::midi::SOUND_VOICE_BYTES;
            let at = bank
                .checked_add(voice * size)
                .ok_or("voice table overflow")?;
            let Ok(record) = psynergy::assets::midi::sound_voice_record(
                rom,
                at.checked_sub(BASE).ok_or("voice table precedes ROM")?,
            ) else {
                continue;
            };
            let kind = record.kind;
            if !matches!(kind, 0 | 1 | 2 | 3 | 4 | 8 | 9 | 10 | 11 | 12 | 64 | 128) {
                continue;
            }
            let target = record.target as usize;
            if matches!(kind, 0 | 8 | 3 | 11 | 64 | 128) && bytes(rom, target, 1).is_err() {
                continue;
            }
            result.add(at, size, Kind::Voice, definitions)?;
            match kind {
                64 => {
                    pending.push(target);
                    let map = u32::from_le_bytes(record.envelope) as usize;
                    if bytes(rom, map, 128).is_ok() {
                        result.add(map, 128, Kind::VoiceKeyMap, &handle)?;
                    }
                }
                128 => pending.push(target),
                0 | 8 => match pcm_extent(rom, target) {
                    Ok(size) => result.add(target, size, Kind::PcmWave, &handle)?,
                    Err(error) => result.issues.push(error),
                },
                3 | 11 => {
                    if bytes(rom, target, 16).is_ok() {
                        result.add(target, 16, Kind::CgbWave, &handle)?;
                    }
                }
                _ => {}
            }
        }
    }
    Ok(())
}

fn pcm_extent(rom: &[u8], start: usize) -> Result<usize, String> {
    crate::build_assets::pcm_wave_extent(
        rom,
        start.checked_sub(BASE).ok_or("PCM pointer precedes ROM")?,
    )
    .map_err(|error| format!("PCM data {start:#x}: {error}"))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn put(rom: &mut [u8], at: usize, value: u32) {
        rom[at..at + 4].copy_from_slice(&value.to_le_bytes());
    }

    #[test]
    fn script_aliases_can_overlap_at_the_source_defined_stop() {
        let rom = [3, 1, 0xf1, 0, 9, 9];
        let stops = BTreeSet::from([0xf1]);
        assert_eq!(script_extent(&rom, BASE, &stops).unwrap(), 4);
        assert_eq!(script_extent(&rom, BASE + 2, &stops).unwrap(), 2);
        assert!(script_extent(&rom[..3], BASE, &stops).is_err());
        assert!(script_extent(&rom, BASE, &BTreeSet::from([0xff])).is_err());
    }

    #[test]
    fn wave_header_proves_its_exact_samples_and_refuses_truncation() {
        let mut rom = vec![0; 64];
        put(&mut rom, 4, 8192000);
        put(&mut rom, 12, 9);
        assert_eq!(pcm_extent(&rom, BASE).unwrap(), 26);
        assert!(pcm_extent(&rom[..25], BASE).is_err());
        put(&mut rom, 0, 0x40000000);
        put(&mut rom, 8, 10);
        assert!(pcm_extent(&rom, BASE).is_err());
        put(&mut rom, 8, 0);
        put(&mut rom, 0, 1);
        assert!(pcm_extent(&rom, BASE).is_err());
    }

    #[test]
    fn pointers_never_turn_overflow_or_non_rom_words_into_extents() {
        let rom = [0xff; 16];
        assert!(bytes(&rom, BASE - 1, 1).is_err());
        assert!(bytes(&rom, usize::MAX, usize::MAX).is_err());
        assert!(pointer(&rom, BASE).is_err());
        assert!(literal(&rom, BASE).is_err());
    }

    #[test]
    fn indexer_derives_its_loaded_table_and_checks_the_source_equation() {
        let mut rom = vec![0u8; 128];
        let instructions = [
            0x4a04u16, 0x1c03, 0x4013, 0x0098, 0x18c0, 0x4b03, 0x0080, 0x18c0, 0x4770, 0,
        ];
        for (index, instruction) in instructions.into_iter().enumerate() {
            rom[index * 2..index * 2 + 2].copy_from_slice(&instruction.to_le_bytes());
        }
        put(&mut rom, 20, 0xfff);
        put(&mut rom, 24, (BASE + 64) as u32);
        assert_eq!(tbs_indexer(&rom, BASE, 0xfff, 20).unwrap(), BASE + 64);
        assert!(tbs_indexer(&rom, BASE, 0xff, 20).is_err());
        assert!(tbs_indexer(&rom, BASE, 0xfff, 24).is_err());
        for end in 0..28 {
            assert!(tbs_indexer(&rom[..end], BASE, 0xfff, 20).is_err());
        }
        put(&mut rom, 24, u32::MAX);
        assert!(tbs_indexer(&rom, BASE, 0xfff, 20).is_err());
    }

    #[test]
    fn song_root_comes_from_the_reader_literal_and_checked_entry_access() {
        let source = ".L_08000000:\n\tmovs\tr3, #175\n\tlsls\tr3, r3, #2\n\tcmp\tr6, r3\n\tbge.n\t.L_08000040\n\tcmp\tr6, #99\n\tble.n\t.L_08000040\n\tldr\tr7, [pc, #16]\n\tlsls\tr4, r6, #3\n\tadds\tr3, r4, #4\n\tldrh\tr2, [r7, r3]\n\tldr\tr1, [r7, r4]\n";
        let mut rom = vec![0u8; 80];
        let instructions = [
            0x23afu16, 0x009b, 0x429e, 0xda00, 0x2e63, 0xdd00, 0x4f04, 0x00f4, 0x1d23, 0x5afa,
        ];
        for (index, instruction) in instructions.into_iter().enumerate() {
            rom[index * 2..index * 2 + 2].copy_from_slice(&instruction.to_le_bytes());
        }
        put(&mut rom, 32, (BASE + 64) as u32);
        assert_eq!(song_table_getter(source, &rom).unwrap(), BASE + 64);
        assert!(song_table_getter(&source.replace("#3\n", "#2\n"), &rom).is_err());
        assert!(song_table_getter(&source.replace("ldr\tr1", "ldr\tr2"), &rom).is_err());
        assert!(song_table_getter(source, &rom[..35]).is_err());
        rom[14] = 0;
        assert!(song_table_getter(source, &rom).is_err());
        put(&mut rom, 32, u32::MAX);
        assert!(song_table_getter(source, &rom).is_err());
    }

    #[test]
    fn image_pixels_come_from_the_current_nested_loops_and_their_back_edges() {
        let tbs = include_str!(concat!(
            env!("CARGO_MANIFEST_DIR"),
            "/../../games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/DECODE.S"
        ));
        let tla = include_str!(concat!(
            env!("CARGO_MANIFEST_DIR"),
            "/../../games/THE LOST AGE/SRC/BATTLE/EFFECT/SENTOU_KOUKA_TENKAI.S"
        ));
        assert_eq!(delta7_pixels(tbs).unwrap(), 15 * 8 * 32 * 8);
        assert_eq!(delta7_pixels(tla).unwrap(), 15 * 8 * 32 * 8);
        assert_eq!(
            delta7_pixels(&tbs.replace("r9, #15", "r9, #30")).unwrap(),
            30 * 8 * 32 * 8
        );
        assert!(delta7_pixels(&tbs.replace("fp, #7", "fp, #6")).is_err());
        assert!(delta7_pixels(&tbs.replace("bne .L_080b517c", "bne .L_080b5180")).is_err());
        assert!(delta7_pixels(&tbs.replacen("bl sub_080b520c", "bl other_reader", 1)).is_err());
    }

    #[test]
    fn image_extent_proves_the_full_inverse_and_keeps_trailing_bytes_unknown() {
        let pixels = [0, 1, 127, 0, 3, 11, 27, 101, 102, 117, 118, 125, 126, 127];
        let encoded = psynergy::assets::compression::encode_delta7(&pixels).unwrap();
        let mut package = vec![0, 0, 0xff, 0x7f];
        package.extend_from_slice(&encoded);
        let extent = package.len();
        assert_eq!(delta7_extent(&package, 4, pixels.len()).unwrap(), extent);
        assert!(delta7_extent(&package[..extent - 1], 4, pixels.len()).is_err());
        package.extend_from_slice(&[0x9a, 0xbc, 0xde]);
        assert_eq!(delta7_extent(&package, 4, pixels.len()).unwrap(), extent);
        package.push(0);
        assert!(delta7_extent(&package, 4, pixels.len()).is_err());
        package.truncate(extent);
        package[extent - 1] ^= 128;
        assert!(delta7_extent(&package, 4, pixels.len()).is_err());
        package[extent - 1] ^= 128;
        package[3] |= 128;
        assert!(delta7_extent(&package, 4, pixels.len()).is_err());
        assert!(delta7_extent(&package, 3, pixels.len()).is_err());
        assert!(delta7_extent(&package, 4, 0).is_err());
    }

    #[test]
    fn map_fields_and_codec_calls_come_from_the_maintained_loaders() {
        let tbs = include_str!(concat!(
            env!("CARGO_MANIFEST_DIR"),
            "/../../recon/tbs/raw/0800fb38.s"
        ));
        let tla = include_str!(concat!(
            env!("CARGO_MANIFEST_DIR"),
            "/../../recon/tla/raw/08027e20.s"
        ));
        let tbs_fields = map_components(tbs, "r5", "sub_08002f40").unwrap();
        let tla_fields = map_components(tla, "r7", "sub_08013300").unwrap();
        assert_eq!(
            tbs_fields
                .iter()
                .map(|field| field.field)
                .collect::<Vec<_>>(),
            [36, 40, 44, 48, 52, 56]
        );
        assert_eq!(
            tla_fields
                .iter()
                .map(|field| field.field)
                .collect::<Vec<_>>(),
            [36, 40, 44, 48, 52, 56, 60]
        );
        assert!(tbs_fields[..5].iter().all(|field| field.decoded));
        assert!(!tbs_fields[5].decoded);
        assert!(tla_fields.iter().all(|field| field.decoded));
        assert!(
            map_components(&tbs.replace("[r5, #44]", "[r5, #45]"), "r5", "sub_08002f40").is_err()
        );
        assert!(map_components(
            &tbs.replace("adds\tr0, r5, r0", "adds\tr0, r4, r0"),
            "r5",
            "sub_08002f40"
        )
        .is_err());
        assert!(map_components(tbs, "r5", "another_getter").is_err());
    }

    fn map_fixture() -> (Vec<u8>, Vec<MapComponent>, usize) {
        let stream = psynergy::assets::lz::encode_general(
            &[1, 2, 3],
            &[psynergy::assets::lz::GeneralToken::Literal(3)],
        )
        .unwrap();
        let header = 52;
        let second = header + stream.len() + 3;
        let raw = second + stream.len() + 1;
        let mut package = vec![0u8; raw + 4];
        put(&mut package, 36, header as u32);
        put(&mut package, 40, second as u32);
        put(&mut package, 44, header as u32);
        put(&mut package, 48, raw as u32);
        package[header..header + stream.len()].copy_from_slice(&stream);
        package[second..second + stream.len()].copy_from_slice(&stream);
        package[raw..].copy_from_slice(&[0x9a, 0xbc, 0xde, 0xf0]);
        let components = (36..52)
            .step_by(4)
            .map(|field| MapComponent {
                field,
                decoded: field != 48,
                optional: field >= 44,
            })
            .collect();
        (package, components, stream.len())
    }

    #[test]
    fn map_decode_classifies_exact_streams_and_keeps_raw_and_gap_bytes_unknown() {
        let (mut package, components, size) = map_fixture();
        let ranges = map_ranges(&package, &components).unwrap();
        assert_eq!(ranges[0], (0, 52, Kind::MapHeader));
        assert_eq!(ranges[1], (52, size, Kind::MapComponent));
        assert_eq!(ranges[2], (52 + size + 3, size, Kind::MapComponent));
        assert_eq!(ranges[3], ranges[1]);
        assert!(ranges
            .iter()
            .all(|(start, size, _)| start + size < package.len()));
        put(&mut package, 44, 0);
        assert_eq!(map_ranges(&package, &components).unwrap().len(), 3);
    }

    #[test]
    fn map_decode_refuses_truncated_and_outside_component_extents() {
        let (package, components, _) = map_fixture();
        assert!(map_ranges(&package[..51], &components).is_err());
        for (field, value) in [(36, 0), (36, 51), (36, 53), (40, u32::MAX), (40, 54)] {
            let mut malformed = package.clone();
            put(&mut malformed, field, value);
            assert!(
                map_ranges(&malformed, &components).is_err(),
                "field={field} value={value}"
            );
        }
        let malformed = [
            MapComponent {
                field: usize::MAX,
                decoded: true,
                optional: false,
            },
            MapComponent {
                field: 36,
                decoded: true,
                optional: false,
            },
        ];
        assert!(map_ranges(&package, &malformed).is_err());
        assert!(map_ranges(&package, &[]).is_err());
    }
}
