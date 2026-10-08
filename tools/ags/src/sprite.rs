use crate::asm::Data;
use crate::graphics::indices;
use crate::lz::compress_tagged;
use crate::resource::PACKER;
use crate::assets::compression::encode_zero_skip;
use crate::assets::image::indexed_bitmap_png;
use crate::assets::lz::compress_arena;
use std::collections::HashMap;
use std::fmt::Write;

struct Record {
    name: String,
    files: String,
    bytes: [u8; 12],
    frames: Option<String>,
    animations: Option<String>,
    count: Option<usize>,
}

#[derive(Clone, Debug, PartialEq, Eq)]
enum Script {
    Own(usize),
    Other(String, usize),
}

#[derive(Default)]
struct Sprite {
    scripts: Vec<Vec<u8>>,
    animations: Option<Vec<Script>>,
    list: Option<Vec<Option<usize>>>,
    empty: Vec<usize>,
}

impl Sprite {
    fn animations(&self) -> Vec<Script> {
        self.animations
            .clone()
            .unwrap_or_else(|| (0..self.scripts.len()).map(Script::Own).collect())
    }
}

fn number(built: &str, field: &str) -> Result<i64, String> {
    let (negative, digits) = match field.strip_prefix('-') {
        Some(digits) => (true, digits),
        None => (false, field),
    };
    let value = match digits.strip_prefix("0x") {
        Some(hex) => i64::from_str_radix(hex, 16),
        None => digits.parse(),
    }
    .map_err(|_| format!("{built}: {field:?} is not a number"))?;
    Ok(if negative { -value } else { value })
}

fn records(built: &str, text: &str) -> Result<Vec<Record>, String> {
    let mut lines = text.lines().filter(|line| !line.trim().is_empty());
    let header = lines
        .next()
        .ok_or_else(|| format!("{built}: empty record table"))?;
    if header.split('\t').count() != 12 {
        return Err(format!("{built}: the record table has twelve columns"));
    }
    lines
        .map(|line| {
            let fields: Vec<&str> = line.split('\t').collect();
            if fields.len() != 12 {
                return Err(format!("{built}: record {line:?} needs twelve fields"));
            }
            let mut bytes = [0u8; 12];
            let byte = |field: &str, low: i64, high: i64| -> Result<u8, String> {
                let value = number(built, field)?;
                if value < low || value > high {
                    return Err(format!("{built}: {field} does not fit"));
                }
                Ok(value as u8)
            };
            bytes[0] = byte(fields[1], 0, 255)?;
            bytes[1] = byte(fields[2], 0, 255)?;
            let scale = number(built, fields[3])?;
            if !(0..=0xffff).contains(&scale) {
                return Err(format!("{built}: scale {scale} does not fit"));
            }
            bytes[2..4].copy_from_slice(&(scale as u16).to_le_bytes());
            bytes[4] = byte(fields[4], 0, 255)?;
            bytes[6] = byte(fields[5], -128, 127)?;
            bytes[7] = byte(fields[6], -128, 127)?;
            bytes[8] = byte(fields[7], 0, 255)?;
            bytes[9] = byte(fields[8], 0, 255)?;
            bytes[10] = byte(fields[9], 0, 255)?;
            let reference = |field: &str| (field != "-").then(|| field.to_owned());
            let (animations, count) = match fields[11].split_once(':') {
                Some((owner, count)) => (Some(owner.to_owned()), Some(frame_number(built, count)?)),
                None => (reference(fields[11]), None),
            };
            Ok(Record {
                name: fields[0]
                    .split_once('=')
                    .map_or(fields[0], |(name, _)| name)
                    .to_owned(),
                files: fields[0]
                    .split_once('=')
                    .map_or(fields[0], |(_, files)| files)
                    .to_owned(),
                bytes,
                frames: reference(fields[10]),
                animations,
                count,
            })
        })
        .collect()
}

fn frame_number(built: &str, text: &str) -> Result<usize, String> {
    text.parse()
        .map_err(|_| format!("{built}: {text:?} is not a number"))
}

fn sprite_text(built: &str, text: &str) -> Result<Sprite, String> {
    let mut sprite = Sprite::default();
    for line in text.lines().filter(|line| !line.trim().is_empty()) {
        let (key, values) = line
            .split_once('\t')
            .ok_or_else(|| format!("{built}: {line:?} needs a key and values"))?;
        let values = values.split(' ');
        match key {
            "anim" => sprite.scripts.push(
                values
                    .map(|byte| {
                        u8::from_str_radix(byte, 16)
                            .map_err(|_| format!("{built}: {byte:?} is not a hex byte"))
                    })
                    .collect::<Result<_, _>>()?,
            ),
            "animations" => {
                sprite.animations = Some(
                    values
                        .map(|value| match value.split_once(':') {
                            Some((owner, n)) => {
                                Ok(Script::Other(owner.to_owned(), frame_number(built, n)?))
                            }
                            None => Ok(Script::Own(frame_number(built, value)?)),
                        })
                        .collect::<Result<_, String>>()?,
                )
            }
            "list" => {
                sprite.list = Some(
                    values
                        .map(|value| match value {
                            "-" => Ok(None),
                            value => frame_number(built, value).map(Some),
                        })
                        .collect::<Result<_, _>>()?,
                )
            }
            "empty" => {
                sprite.empty = values
                    .map(|value| frame_number(built, value))
                    .collect::<Result<_, _>>()?
            }
            _ => return Err(format!("{built}: {key:?} is not a sprite key")),
        }
    }
    Ok(sprite)
}

fn frame_streams(
    built: &str,
    codec: u8,
    frames: &[&[u8]],
    empty: &[usize],
) -> Result<Vec<Vec<u8>>, String> {
    let zero_skip =
        |pixels: &[u8]| encode_zero_skip(pixels).map_err(|error| format!("{built}: {}", error.0));
    let mut streams: Vec<Vec<u8>> = Vec::new();
    let mut container = Vec::new();
    for (number, frame) in frames.iter().enumerate() {
        let blank = empty.contains(&number);
        if blank && (codec != 3 || frame.iter().any(|&pixel| pixel != 0)) {
            return Err(format!("{built}: only a blank arena frame can be empty"));
        }
        let stream = match codec {
            0 => zero_skip(frame)?,
            1 => compress_tagged(frame, &PACKER)?,
            3 => {
                let decoded = if blank { Vec::new() } else { zero_skip(frame)? };
                compress_arena(&decoded, &container)
                    .map_err(|error| format!("{built}: {}", error.0))?
            }
            other => return Err(format!("{built}: codec {other} has no frames")),
        };
        container.extend(&stream);
        streams.push(stream);
    }
    Ok(streams)
}

fn label(sprite: &str, item: &str) -> String {
    format!("Sprite_{sprite}_{item}")
}

struct Bank {
    records: Vec<Record>,
    sprites: HashMap<String, Sprite>,
    pictures: HashMap<String, String>,
}

fn shared_pictures(
    built: &str,
    sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<HashMap<String, String>, String> {
    let Ok(text) = sibling(PICTURES) else {
        return Ok(HashMap::new());
    };
    let text = String::from_utf8(text).map_err(|_| format!("{built}: {PICTURES} is not text"))?;
    let mut lines = text.lines().filter(|line| !line.trim().is_empty());
    if lines.next() != Some("sprite\tpicture") {
        return Err(format!(
            "{built}: {PICTURES} needs a sprite\\tpicture header"
        ));
    }
    let mut pictures = HashMap::new();
    for line in lines {
        let fields: Vec<&str> = line.split('\t').map(str::trim).collect();
        let [sprite, picture] = fields.as_slice() else {
            return Err(format!(
                "{built}: {PICTURES} line {line:?} needs two fields"
            ));
        };
        let plain = |name: &str| {
            !name.is_empty()
                && name
                    .bytes()
                    .all(|byte| byte.is_ascii_uppercase() || byte.is_ascii_digit() || byte == b'_')
        };
        if !plain(sprite) || !plain(picture) {
            return Err(format!(
                "{built}: {PICTURES} line {line:?} names no plain stems"
            ));
        }
        if pictures
            .insert((*sprite).to_owned(), (*picture).to_owned())
            .is_some()
        {
            return Err(format!("{built}: {PICTURES} names {sprite} twice"));
        }
    }
    Ok(pictures)
}

pub(crate) const PICTURES: &str = "pictures.tsv";

impl Bank {
    fn read(
        built: &str,
        table: &[u8],
        sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
    ) -> Result<Self, String> {
        let text = std::str::from_utf8(table).map_err(|_| format!("{built}: table is not text"))?;
        let records = records(built, text)?;
        let mut sprites = HashMap::new();
        for record in &records {
            let owns = |list: &Option<String>| list.as_deref() == Some(record.name.as_str());
            let name = format!("{}.tsv", record.files);
            if owns(&record.animations) || (owns(&record.frames) && sibling(&name).is_ok()) {
                let text = String::from_utf8(sibling(&name)?)
                    .map_err(|_| format!("{built}: {name} is not text"))?;
                sprites.insert(record.name.clone(), sprite_text(built, &text)?);
            }
        }
        let pictures = shared_pictures(built, sibling)?;
        Ok(Self {
            records,
            sprites,
            pictures,
        })
    }
    fn sprite(&self, built: &str, name: &str) -> Result<&Sprite, String> {
        self.sprites
            .get(name)
            .ok_or_else(|| format!("{built}: {name} has no TSV"))
    }
    fn owns_animations(&self, name: &str) -> bool {
        self.records
            .iter()
            .any(|r| r.name == name && r.animations.as_deref() == Some(name))
    }
    fn owns_frames(&self, name: &str) -> Option<&Record> {
        self.records
            .iter()
            .find(|r| r.name == name && r.frames.as_deref() == Some(name))
    }
    fn records(&self, built: &str, data: &mut Data) -> Result<(), String> {
        for record in &self.records {
            data.bytes.extend(&record.bytes[..5]);
            let count = match &record.animations {
                Some(owner) => {
                    let all = self.sprite(built, owner)?.animations().len();
                    match record.count {
                        Some(count) if count <= all => count,
                        Some(count) => {
                            return Err(format!("{built}: {owner} has no {count} animations"))
                        }
                        None => all,
                    }
                }
                None => 0,
            };
            data.bytes
                .push(u8::try_from(count).map_err(|_| format!("{built}: too many animations"))?);
            data.bytes.extend(&record.bytes[6..12]);
            for (list, item) in [
                (&record.frames, "Frames"),
                (&record.animations, "Animations"),
            ] {
                match list {
                    Some(owner) => data.pointer(&label(owner, item), 0),
                    None => data.bytes.extend([0; 4]),
                }
            }
        }
        Ok(())
    }
    fn animation_list(&self, built: &str, name: &str, data: &mut Data) -> Result<(), String> {
        data.label(&label(name, "Animations"), false);
        let sprite = self.sprite(built, name)?;
        for script in sprite.animations() {
            let (owner, number) = match &script {
                Script::Own(number) => (name, *number),
                Script::Other(owner, number) => (owner.as_str(), *number),
            };
            if self.sprite(built, owner)?.scripts.len() <= number {
                return Err(format!("{built}: {owner} has no animation {number}"));
            }
            data.pointer(&label(owner, &format!("Animation{number}")), 0);
        }
        Ok(())
    }
    fn scripts(&self, built: &str, name: &str, data: &mut Data) -> Result<(), String> {
        for (number, bytes) in self.sprite(built, name)?.scripts.iter().enumerate() {
            data.label(&label(name, &format!("Animation{number}")), false);
            data.bytes.extend(bytes);
        }
        Ok(())
    }
    fn frames(
        &self,
        built: &str,
        record: &Record,
        sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
    ) -> Result<(Data, Data), String> {
        let name = &record.name;
        let png = match self.pictures.get(&record.files) {
            Some(picture) => sibling(&format!("common/{picture}.png"))?,
            None => sibling(&format!("{}.png", record.files))?,
        };
        let image =
            indexed_bitmap_png(&png).map_err(|error| format!("{built}: {name}: {}", error.0))?;
        let (width, height) = (record.bytes[0] as usize, record.bytes[1] as usize);
        if image.width as usize != width || height == 0 || !(image.height as usize).is_multiple_of(height) {
            return Err(format!(
                "{built}: {name} is not {width}x{height} frames stacked"
            ));
        }
        let pixels = indices(&image);
        let frames: Vec<&[u8]> = pixels.chunks(width * height).collect();
        let sprite = self.sprites.get(name);
        let list = sprite
            .and_then(|sprite| sprite.list.clone())
            .unwrap_or_else(|| (0..frames.len()).map(Some).collect());
        let empty = sprite
            .map(|sprite| sprite.empty.clone())
            .unwrap_or_default();
        let mut list_data = Data::default();
        list_data.label(&label(name, "Frames"), false);
        for number in list {
            match number {
                Some(number) if number < frames.len() => {
                    list_data.pointer(&label(name, &format!("Frame{number}")), 0)
                }
                Some(number) => return Err(format!("{built}: {name} has no frame {number}")),
                None => list_data.bytes.extend([0; 4]),
            }
        }
        let mut frame_data = Data::default();
        let streams = frame_streams(built, record.bytes[10], &frames, &empty)?;
        for (number, stream) in streams.into_iter().enumerate() {
            frame_data.label(&label(name, &format!("Frame{number}")), false);
            frame_data.bytes.extend(stream);
        }
        Ok((list_data, frame_data))
    }
}

pub(crate) fn bank(
    built: &str,
    table: &[u8],
    sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<Data, String> {
    let bank = Bank::read(built, table, sibling)?;
    let mut data = Data::default();
    bank.records(built, &mut data)?;
    for record in &bank.records {
        if bank.owns_animations(&record.name) {
            bank.animation_list(built, &record.name, &mut data)?;
        }
    }
    let mut frame_data = Data::default();
    for record in &bank.records {
        if bank.owns_frames(&record.name).is_some() {
            let (list, frames) = bank.frames(built, record, sibling)?;
            data.append(list)?;
            frame_data.append(frames)?;
        }
    }
    for record in &bank.records {
        if bank.owns_animations(&record.name) {
            bank.scripts(built, &record.name, &mut data)?;
        }
    }
    data.append(frame_data)?;
    Ok(data)
}

pub(crate) fn blocks(
    built: &str,
    table: &[u8],
    sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<Vec<(Option<String>, Data)>, String> {
    let bank = Bank::read(built, table, sibling)?;
    let layout = String::from_utf8(sibling("layout.tsv")?)
        .map_err(|_| format!("{built}: layout.tsv is not text"))?;
    let mut parts = vec![(None, Data::default())];
    bank.records(built, &mut parts[0].1)?;
    let mut order = Vec::new();
    for line in layout.lines().filter(|line| !line.trim().is_empty()) {
        match line.split_once('\t') {
            Some(("section", name)) => parts.push((Some(name.to_owned()), Data::default())),
            None => {
                let data = &mut parts.last_mut().expect("a part").1;
                if bank.owns_animations(line) {
                    bank.scripts(built, line, data)?;
                    data.align_to(4, 0)?;
                    bank.animation_list(built, line, data)?;
                }
                order.push(line);
            }
            Some(_) => return Err(format!("{built}: layout line {line:?} is not a sprite")),
        }
    }
    let data = &mut parts.last_mut().expect("a part").1;
    for name in order {
        if let Some(record) = bank.owns_frames(name) {
            let (list, frames) = bank.frames(built, record, sibling)?;
            data.append(frames)?;
            data.align_to(4, 0)?;
            data.append(list)?;
            data.bytes.extend([0; 4]);
        }
    }
    Ok(parts)
}

pub(crate) fn blocks_source(
    built: &str,
    table: &[u8],
    sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<String, String> {
    let mut text = String::new();
    for (section, data) in blocks(built, table, sibling)? {
        if let Some(section) = section {
            writeln!(text, "\t.section .rodata.{section},\"a\"").unwrap();
        }
        text.push_str(&data.source()?);
    }
    Ok(text)
}
