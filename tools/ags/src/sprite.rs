//! Field sprite banks: the table of sprite records the game reads by id,
//! each sprite's animation list and frame list, the animation scripts, and
//! the frames, built from a record table `STEM.TSV`, one `SPRITE.PNG` per
//! sprite with frames (one frame wide, its frames stacked) and one
//! `SPRITE.TSV` per sprite with animations.
//!
//! The record table's columns are `sprite width height scale draw adjust_x
//! adjust_y box_x box_y codec frames animations`; `frames` and `animations`
//! name the sprite whose list a record uses, or `-` for none. A sprite's
//! own TSV holds `anim` lines, each an animation script as hex bytes, and
//! `same SPRITE N` lines, which reuse the Nth `anim` of a sprite; a `list`
//! line gives the frame list as frame numbers when it is not each frame
//! once in order.
//!
//! The bank lays out every record, then every animation list, every frame
//! list, every script and every frame, each in record order. Frames are
//! coded by the record's codec: 0 zero-skip, 1 the packer's tagged LZ of the
//! pixels, 3 arena streams whose copies read the sprite's earlier frames
//! (an all-transparent frame is an empty stream).
use crate::asm::Data;
use crate::graphics::indices;
use crate::lz::compress_tagged;
use crate::resource::PACKER;
use psynergy::assets::compression::encode_zero_skip;
use psynergy::assets::image::indexed_bitmap_png;
use psynergy::assets::lz::compress_arena;
use std::collections::HashMap;

struct Record {
    name: String,
    bytes: [u8; 12],
    frames: Option<String>,
    animations: Option<String>,
}

enum Script {
    Own(Vec<u8>),
    Same(String, usize),
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
            Ok(Record {
                name: fields[0].to_owned(),
                bytes,
                frames: reference(fields[10]),
                animations: reference(fields[11]),
            })
        })
        .collect()
}

/// A sprite's own TSV: its animation scripts, its frame list when that is
/// not each frame once, and the blank frames stored as empty streams.
#[derive(Default)]
struct Sprite {
    scripts: Vec<Script>,
    list: Option<Vec<usize>>,
    empty: Vec<usize>,
}

fn numbers(built: &str, text: &str) -> Result<Vec<usize>, String> {
    text.split(' ')
        .map(|n| {
            n.parse()
                .map_err(|_| format!("{built}: {n:?} is not a frame"))
        })
        .collect()
}

fn sprite_text(built: &str, text: &str) -> Result<Sprite, String> {
    let mut sprite = Sprite::default();
    for line in text.lines().filter(|line| !line.trim().is_empty()) {
        let fields: Vec<&str> = line.split('\t').collect();
        match fields.as_slice() {
            ["anim", bytes] => sprite.scripts.push(Script::Own(
                bytes
                    .split(' ')
                    .map(|byte| {
                        u8::from_str_radix(byte, 16)
                            .map_err(|_| format!("{built}: {byte:?} is not a hex byte"))
                    })
                    .collect::<Result<_, _>>()?,
            )),
            ["same", owner, index] => sprite.scripts.push(Script::Same(
                (*owner).to_owned(),
                index
                    .parse()
                    .map_err(|_| format!("{built}: {index:?} is not an animation number"))?,
            )),
            ["list", list] => sprite.list = Some(numbers(built, list)?),
            ["empty", list] => sprite.empty = numbers(built, list)?,
            _ => {
                return Err(format!(
                    "{built}: {line:?} is not an anim, same, list or empty line"
                ))
            }
        }
    }
    Ok(sprite)
}

/// One sprite's frames, coded by `codec`, each after the ones before it.
/// The frames `empty` names must be blank and are stored as empty streams.
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
/// The label of a sprite's own item.
fn label(sprite: &str, item: &str) -> String {
    format!("Sprite_{sprite}_{item}")
}

/// A field sprite bank from its record table and each sprite's files.
pub fn bank(
    built: &str,
    table: &[u8],
    sibling: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<Data, String> {
    let text = std::str::from_utf8(table).map_err(|_| format!("{built}: table is not text"))?;
    let records = records(built, text)?;
    let mut sprites: HashMap<String, Sprite> = HashMap::new();
    for record in &records {
        let owns = |list: &Option<String>| list.as_deref() == Some(record.name.as_str());
        if owns(&record.animations)
            || (owns(&record.frames) && sibling(&format!("{}.TSV", record.name)).is_ok())
        {
            let text = sibling(&format!("{}.TSV", record.name))?;
            let text = String::from_utf8(text)
                .map_err(|_| format!("{built}: {}.TSV is not text", record.name))?;
            sprites.insert(record.name.clone(), sprite_text(built, &text)?);
        }
    }
    let mut data = Data::default();
    for record in &records {
        data.bytes.extend(&record.bytes[..5]);
        let count = match &record.animations {
            Some(owner) => sprites
                .get(owner)
                .ok_or_else(|| format!("{built}: {owner} has no animations"))?
                .scripts
                .len(),
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
    // Animation lists, naming each script by its owner and number.
    for record in records
        .iter()
        .filter(|r| r.animations.as_deref() == Some(&r.name))
    {
        data.label(&label(&record.name, "Animations"), false);
        let mut own = 0;
        for script in &sprites[&record.name].scripts {
            match script {
                Script::Own(_) => {
                    data.pointer(&label(&record.name, &format!("Animation{own}")), 0);
                    own += 1;
                }
                Script::Same(owner, index) => {
                    data.pointer(&label(owner, &format!("Animation{index}")), 0)
                }
            }
        }
    }
    // Frame lists, and each sprite's frames.
    let mut frame_data = Data::default();
    for record in records
        .iter()
        .filter(|r| r.frames.as_deref() == Some(&r.name))
    {
        let png = sibling(&format!("{}.PNG", record.name))?;
        let image = indexed_bitmap_png(&png)
            .map_err(|error| format!("{built}: {}: {}", record.name, error.0))?;
        let (width, height) = (record.bytes[0] as usize, record.bytes[1] as usize);
        if image.width as usize != width || height == 0 || image.height as usize % height != 0 {
            return Err(format!(
                "{built}: {} is not {width}x{height} frames stacked",
                record.name
            ));
        }
        let pixels = indices(&image);
        let frames: Vec<&[u8]> = pixels.chunks(width * height).collect();
        let list = sprites
            .get(&record.name)
            .and_then(|sprite| sprite.list.clone())
            .unwrap_or_else(|| (0..frames.len()).collect());
        data.label(&label(&record.name, "Frames"), false);
        for number in list {
            if number >= frames.len() {
                return Err(format!("{built}: {} has no frame {number}", record.name));
            }
            data.pointer(&label(&record.name, &format!("Frame{number}")), 0);
        }
        let empty = sprites
            .get(&record.name)
            .map(|sprite| sprite.empty.clone())
            .unwrap_or_default();
        for (number, stream) in frame_streams(built, record.bytes[10], &frames, &empty)?
            .into_iter()
            .enumerate()
        {
            frame_data.label(&label(&record.name, &format!("Frame{number}")), false);
            frame_data.bytes.extend(stream);
        }
    }
    for record in records
        .iter()
        .filter(|r| r.animations.as_deref() == Some(&r.name))
    {
        let mut own = 0;
        for script in &sprites[&record.name].scripts {
            if let Script::Own(bytes) = script {
                data.label(&label(&record.name, &format!("Animation{own}")), false);
                data.bytes.extend(bytes);
                own += 1;
            }
        }
    }
    data.append(frame_data)?;
    Ok(data)
}

#[cfg(test)]
mod tests {
    use super::*;
    use psynergy::assets::image::png_from_bitmap;
    use psynergy::assets::lz::decode_arena;

    #[test]
    fn bank_lays_out_records_lists_scripts_and_frames() {
        let palette: Vec<u8> = (0..16u16).flat_map(|c| c.to_le_bytes()).collect();
        // Two 2x2 frames, the second repeating the first.
        let png = png_from_bitmap(&[1, 2, 3, 4, 1, 2, 3, 4], &palette, 2).unwrap();
        let table = "sprite\twidth\theight\tscale\tdraw\tadjust_x\tadjust_y\tbox_x\tbox_y\tcodec\tframes\tanimations\n\
            A\t2\t2\t0x100\t5\t0\t-2\t20\t16\t0\tA\tA\n\
            B\t2\t2\t0x100\t5\t0\t0\t20\t16\t2\t-\tB\n";
        let sibling = |name: &str| -> Result<Vec<u8>, String> {
            Ok(match name {
                "A.PNG" => png.clone(),
                "A.TSV" => b"list\t0 1 1\nanim\t00 05 f1 00\n".to_vec(),
                "B.TSV" => b"same\tA\t0\nanim\t01 02\n".to_vec(),
                other => return Err(format!("no {other}")),
            })
        };
        let data = bank("S.sprites", table.as_bytes(), &sibling).unwrap();
        // Records: A has one animation, B two; B has no frames.
        assert_eq!(
            &data.bytes[..12],
            &[2, 2, 0, 1, 5, 1, 0, 0xfe, 20, 16, 0, 0]
        );
        assert_eq!(&data.bytes[20..32], &[2, 2, 0, 1, 5, 2, 0, 0, 20, 16, 2, 0]);
        assert_eq!(&data.bytes[32..36], &[0; 4]);
        let names: Vec<&str> = data
            .pointers
            .iter()
            .map(|(_, p)| p.symbol.as_str())
            .collect();
        assert_eq!(
            names,
            [
                "Sprite_A_Frames",
                "Sprite_A_Animations",
                "Sprite_B_Animations",
                "Sprite_A_Animation0",
                "Sprite_A_Animation0",
                "Sprite_B_Animation0",
                "Sprite_A_Frame0",
                "Sprite_A_Frame1",
                "Sprite_A_Frame1"
            ]
        );
        // Scripts, then the zero-skip frames.
        let tail = &data.bytes[data.bytes.len() - 16..];
        assert_eq!(tail, &[0, 5, 0xf1, 0, 1, 2, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0]);
    }

    #[test]
    fn arena_frames_read_their_sprite_and_store_blank_frames_empty() {
        let first: Vec<u8> = (1..=40).collect();
        let blank = vec![0u8; 40];
        let streams = frame_streams("S", 3, &[&first, &blank, &first], &[1]).unwrap();
        assert_eq!(streams[1], [0, 0]);
        // The third frame copies the first, which lies before it in the sprite.
        let mut container = streams.concat();
        container.extend([0; 4]);
        let at = streams[0].len() + 2;
        let (decoded, _, _) = decode_arena(&container, at).unwrap();
        assert_eq!(decoded, encode_zero_skip(&first).unwrap());
        assert!(streams[2].len() < streams[0].len());
    }
}
