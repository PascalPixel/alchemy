//! `--network`: the field maps one world map exit leads into, drawn as one
//! image with an arrow from every exit trigger to the entrance it reaches.
//!
//! A family starts at a world map exit and follows scene exits until they
//! lead back to the world map. Maps are placed one at a time: a map starts
//! with its arrival entrance on the trigger that leads to it and slides the
//! way the party faces on arrival until it overlaps nothing, so a door to the
//! north puts its room above and an east edge puts its neighbour to the right.
use super::*;

const WORLD_MAP: usize = 2;
const OVERLAY_BASE: u32 = 0x0200_8000;
const CELLS: usize = 128;
const PANEL_GAP: i64 = 256;
const LINK: [u8; 4] = [200, 60, 255, 255];
const WORLD_MAP_MARK: [u8; 4] = [64, 224, 96, 255];
const MARK: [u8; 4] = [255, 48, 48, 255];
const MAX_EDGE: usize = 16384;

/// Where the scene script keeps the loaded scene id.
fn scene_address(target: &DecompTarget) -> u32 {
    match target.compiler {
        CompilerTarget::Tbs => 0x0200_0400,
        CompilerTarget::Tla => 0x0200_0420,
    }
}

/// A decompressed scene overlay and the table getters its entry veneers name.
struct Overlay {
    bytes: Vec<u8>,
    scene_address: u32,
}
impl Overlay {
    fn read(
        rom: &[u8],
        directory: &Directory,
        id: usize,
        scene_address: u32,
    ) -> Result<Self, String> {
        let (offset, span) = directory.resource(id)?;
        let bytes = decode_tagged(rom, offset, offset + span)
            .map_err(|e| format!("overlay {id:03x}: {e}"))?;
        Ok(Self {
            bytes,
            scene_address,
        })
    }
    fn u16(&self, address: u32) -> Option<u16> {
        let at = address.checked_sub(OVERLAY_BASE)? as usize;
        self.bytes
            .get(at..at + 2)
            .map(|b| u16::from_le_bytes([b[0], b[1]]))
    }
    fn u32(&self, address: u32) -> Option<u32> {
        let at = address.checked_sub(OVERLAY_BASE)? as usize;
        self.bytes
            .get(at..at + 4)
            .map(|b| u32::from_le_bytes([b[0], b[1], b[2], b[3]]))
    }
    /// Entry veneer `slot`: `ldr r4, [pc, #0]; bx r4; .4byte target`.
    fn entry(&self, slot: u32) -> Result<u32, String> {
        let at = OVERLAY_BASE + slot * 8;
        match (self.u16(at), self.u16(at + 2), self.u32(at + 4)) {
            (Some(0x4c00), Some(0x4720), Some(target)) => Ok(target),
            _ => Err(format!("overlay entry veneer {slot} is malformed")),
        }
    }
    fn read16(&self, address: u32, scene: u16) -> Result<u16, String> {
        if address == self.scene_address {
            return Ok(scene);
        }
        self.u16(address)
            .ok_or_else(|| format!("getter reads 0x{address:08x}"))
    }
    /// Run a table getter for `scene` and return what it leaves in r0. Getters
    /// load a literal, sometimes after comparing the loaded scene id.
    fn table(&self, slot: u32, scene: usize) -> Result<u32, String> {
        let entry = self.entry(slot)?;
        let scene = scene as u16;
        // The getter returns when it branches to the link value it was given.
        const RETURN: u32 = 0xffff_fffe;
        let mut r = [0u32; 16];
        r[14] = RETURN;
        let mut stack = Vec::new();
        let (mut n, mut z, mut c, mut v) = (false, false, false, false);
        let mut pc = entry & !1;
        for _ in 0..512 {
            let op = self
                .u16(pc)
                .ok_or_else(|| format!("getter 0x{entry:08x} leaves the overlay"))?
                as u32;
            let (rd, rn, rm) = (
                (op & 7) as usize,
                (op >> 3 & 7) as usize,
                (op >> 6 & 7) as usize,
            );
            let rt = (op >> 8 & 7) as usize;
            let (high_rd, high_rm) = ((op & 7 | op >> 4 & 8) as usize, (op >> 3 & 15) as usize);
            let mut subtract = |a: u32, b: u32| {
                let result = a.wrapping_sub(b);
                (n, z, c, v) = (
                    result >> 31 == 1,
                    result == 0,
                    a >= b,
                    ((a ^ b) & (a ^ result)) >> 31 == 1,
                );
                result
            };
            match op {
                _ if op & 0xfe00 == 0xb400 => {
                    if op & 0x100 != 0 {
                        stack.push(r[14]);
                    }
                    for register in (0..8).rev().filter(|i| op >> i & 1 == 1) {
                        stack.push(r[register]);
                    }
                }
                _ if op & 0xfe00 == 0xbc00 => {
                    for register in (0..8).filter(|i| op >> i & 1 == 1) {
                        r[register] = stack.pop().ok_or("getter pops an empty stack")?;
                    }
                    if op & 0x100 != 0 {
                        match stack.pop().ok_or("getter pops an empty stack")? {
                            RETURN => return Ok(r[0]),
                            address => {
                                pc = address & !1;
                                continue;
                            }
                        }
                    }
                }
                _ if op & 0xf800 == 0xf000 => {
                    let low = self.u16(pc + 2).ok_or("call runs past the overlay")? as u32;
                    let offset = ((((op & 0x7ff) << 21) as i32) >> 9) as u32 | ((low & 0x7ff) << 1);
                    let callee = (pc + 4).wrapping_add(offset);
                    if self.u16(callee).is_some() && stack.len() < 64 {
                        r[14] = (pc + 4) | 1;
                        pc = callee;
                    } else {
                        // A main-image service (a flag test, say) is taken to answer zero.
                        r[0] = 0;
                        pc += 4;
                    }
                    continue;
                }
                _ if op & 0xff87 == 0x4700 => {
                    let target = r[(op >> 3 & 15) as usize];
                    if target == RETURN {
                        return Ok(r[0]);
                    }
                    if self.u16(target & !1).is_some() {
                        pc = target & !1;
                        continue;
                    }
                    // A tail call into the main image answers zero to its caller.
                    r[0] = 0;
                    if r[14] == RETURN {
                        return Ok(0);
                    }
                    pc = r[14] & !1;
                    continue;
                }
                // Stores do not change which table a getter returns.
                _ if matches!(op >> 11, 0xc | 0xe | 0x10)
                    || op & 0xf000 == 0x5000 && op & 0x0800 == 0 => {}
                _ if op & 0xff00 == 0x4600 => r[high_rd] = r[high_rm],
                _ if op & 0xffc0 == 0x4200 => {
                    let result = r[rd] & r[rn];
                    (n, z) = (result >> 31 == 1, result == 0);
                }
                _ if op & 0xffc0 == 0x4000 => {
                    r[rd] &= r[rn];
                    (n, z) = (r[rd] >> 31 == 1, r[rd] == 0);
                }
                _ if op & 0xf800 == 0x6800 => {
                    let address = r[rn].wrapping_add((op >> 6 & 31) << 2);
                    r[rd] = self
                        .u32(address)
                        .ok_or_else(|| format!("getter reads 0x{address:08x}"))?;
                }
                _ if op & 0xf800 == 0x0800 => {
                    let shift = op >> 6 & 31;
                    r[rd] = if shift == 0 { 0 } else { r[rn] >> shift };
                    (n, z) = (r[rd] >> 31 == 1, r[rd] == 0);
                }
                _ if op & 0xf800 == 0x4800 => {
                    let literal = ((pc + 4) & !3) + ((op & 0xff) << 2);
                    r[rt] = self.u32(literal).ok_or_else(|| {
                        format!("literal 0x{literal:08x} lies outside the overlay")
                    })?;
                }
                _ if op & 0xf800 == 0x2000 => {
                    r[rt] = op & 0xff;
                    (n, z) = (false, r[rt] == 0);
                }
                _ if op & 0xf800 == 0x2800 => {
                    subtract(r[rt], op & 0xff);
                }
                _ if op & 0xffc0 == 0x4280 => {
                    subtract(r[rd], r[rn]);
                }
                _ if op & 0xf800 == 0x0000 => {
                    r[rd] = r[rn] << (op >> 6 & 31);
                    (n, z) = (r[rd] >> 31 == 1, r[rd] == 0);
                }
                _ if op & 0xfe00 == 0x1c00 => r[rd] = r[rn].wrapping_add(op >> 6 & 7),
                _ if op & 0xfe00 == 0x1800 => r[rd] = r[rn].wrapping_add(r[rm]),
                _ if op & 0xf800 == 0x3000 => r[rt] = r[rt].wrapping_add(op & 0xff),
                _ if op & 0xf800 == 0x3800 => r[rt] = subtract(r[rt], op & 0xff),
                _ if op & 0xfe00 == 0x5e00 => {
                    r[rd] = self.read16(r[rn].wrapping_add(r[rm]), scene)? as i16 as i32 as u32
                }
                _ if op & 0xfe00 == 0x5a00 => {
                    r[rd] = self.read16(r[rn].wrapping_add(r[rm]), scene)? as u32
                }
                _ if op & 0xf800 == 0x8800 => {
                    r[rd] = self.read16(r[rn].wrapping_add((op >> 6 & 31) << 1), scene)? as u32
                }
                _ if op & 0xf000 == 0xd000 && op >> 8 & 15 < 14 => {
                    let taken = match op >> 8 & 15 {
                        0 => z,
                        1 => !z,
                        2 => c,
                        3 => !c,
                        4 => n,
                        5 => !n,
                        6 => v,
                        7 => !v,
                        8 => c && !z,
                        9 => !c || z,
                        10 => n == v,
                        11 => n != v,
                        12 => !z && n == v,
                        _ => z || n != v,
                    };
                    if taken {
                        pc = (pc + 4).wrapping_add(((op & 0xff) as i8 as i32 * 2) as u32);
                        continue;
                    }
                }
                _ if op & 0xf800 == 0xe000 => {
                    pc = (pc + 4).wrapping_add((((op & 0x7ff) << 21) as i32 >> 20) as u32);
                    continue;
                }
                _ => {
                    return Err(format!(
                        "getter 0x{entry:08x}: instruction 0x{op:04x} at 0x{pc:08x} is not modelled"
                    ))
                }
            }
            pc += 2;
        }
        Err(format!("getter 0x{entry:08x} does not return"))
    }
}

#[derive(Clone)]
struct Exit {
    scene: usize,
    flag: Option<u32>,
    exit: u32,
    to_scene: usize,
    entrance: i16,
}
/// The overlay's exit stream: groups open with their scene, an exit word packs
/// exit, entrance and destination, and a conditional exit carries a flag word.
fn exits(overlay: &Overlay, scene: usize) -> Result<Vec<Exit>, String> {
    let mut at = overlay.table(2, scene)?;
    let (mut group, mut exits) = (None, Vec::new());
    for _ in 0..4096 {
        let word = overlay
            .u32(at)
            .ok_or_else(|| format!("exit stream runs past the overlay at 0x{at:08x}"))?;
        at += 4;
        match word {
            0x1ff => return Ok(exits),
            _ if word < 0x200 => group = Some(word as usize),
            _ => {
                let flag = (word & 0x1000_0000 != 0).then(|| overlay.u32(at)).flatten();
                if word & 0x1000_0000 != 0 {
                    at += 4;
                }
                if group == Some(scene) {
                    exits.push(Exit {
                        scene,
                        flag,
                        exit: word >> 20 & 0xff,
                        to_scene: (word & 0xfff) as usize,
                        entrance: (word >> 12 & 0xff) as i16,
                    });
                }
            }
        }
    }
    Err("exit stream has no end".into())
}

/// Entrance id to map position (x, z) in pixels and arrival facing.
fn entrances(overlay: &Overlay, scene: usize) -> Result<BTreeMap<i16, (i64, i64, u16)>, String> {
    let at = overlay.table(1, scene)?;
    let mut found = BTreeMap::new();
    for record in 0..256 {
        let field = |offset: u32| {
            overlay
                .u16(at + record * 24 + offset)
                .ok_or_else(|| "entrance table runs past the overlay".to_string())
        };
        let entrance = field(0)? as i16;
        if entrance == -1 {
            break;
        }
        found.entry(entrance).or_insert((
            field(4)? as i16 as i64,
            field(8)? as i16 as i64,
            field(10)?,
        ));
    }
    Ok(found)
}

/// Trigger code to (exit id, event control), from the scene's exit events.
fn exit_triggers(overlay: &Overlay, scene: usize) -> Result<BTreeMap<u8, (u32, u32)>, String> {
    let at = overlay.table(4, scene)?;
    let mut triggers = BTreeMap::new();
    for record in 0..512 {
        let base = at + record * 12;
        let control = overlay
            .u32(base)
            .ok_or("event table runs past the overlay")?;
        if control == u32::MAX {
            break;
        }
        let trigger = overlay
            .u16(base + 4)
            .ok_or("event table runs past the overlay")?;
        let value = overlay
            .u32(base + 8)
            .ok_or("event table runs past the overlay")?;
        if control & 15 == 1 && trigger < 255 {
            triggers.entry(trigger as u8).or_insert((value, control));
        }
    }
    Ok(triggers)
}

/// Unit travel direction for a facing angle, rounded to a quarter turn.
fn direction(facing: u16) -> (i64, i64) {
    match (facing as u32 + 0x2000) / 0x4000 % 4 {
        0 => (1, 0),
        1 => (0, 1),
        2 => (-1, 0),
        _ => (0, -1),
    }
}

struct Image {
    width: usize,
    height: usize,
    rgba: Vec<u8>,
}
impl Image {
    fn new(width: usize, height: usize, color: [u8; 4]) -> Self {
        Self {
            width,
            height,
            rgba: color.repeat(width * height),
        }
    }
    fn put(&mut self, x: i64, y: i64, color: [u8; 4]) {
        if x >= 0 && y >= 0 && (x as usize) < self.width && (y as usize) < self.height {
            let at = (y as usize * self.width + x as usize) * 4;
            self.rgba[at..at + 4].copy_from_slice(&color);
        }
    }
    fn disc(&mut self, x: i64, y: i64, radius: i64, color: [u8; 4]) {
        for dy in -radius..=radius {
            for dx in -radius..=radius {
                if dx * dx + dy * dy <= radius * radius {
                    self.put(x + dx, y + dy, color);
                }
            }
        }
    }
    fn line(&mut self, from: (i64, i64), to: (i64, i64), radius: i64, color: [u8; 4]) {
        let steps = (to.0 - from.0).abs().max((to.1 - from.1).abs()).max(1);
        for step in 0..=steps {
            self.disc(
                from.0 + (to.0 - from.0) * step / steps,
                from.1 + (to.1 - from.1) * step / steps,
                radius,
                color,
            );
        }
    }
    /// A dashed line through right-angle corners, outlined in black, with a
    /// head at its last point when `head` is set.
    fn dashed(&mut self, points: &[(i64, i64)], color: [u8; 4], head: bool) {
        for (radius, shade) in [(3, [0, 0, 0, 255]), (2, color)] {
            let mut travelled = 0;
            for pair in points.windows(2) {
                let (a, b) = (pair[0], pair[1]);
                let length = (b.0 - a.0).abs().max((b.1 - a.1).abs());
                for step in 0..length {
                    if (travelled + step) % 16 < 10 {
                        self.disc(
                            a.0 + (b.0 - a.0) * step / length.max(1),
                            a.1 + (b.1 - a.1) * step / length.max(1),
                            radius,
                            shade,
                        );
                    }
                }
                travelled += length;
            }
            if head && points.len() >= 2 {
                let (last, to) = (points[points.len() - 2], points[points.len() - 1]);
                let (ux, uy) = ((to.0 - last.0).signum(), (to.1 - last.1).signum());
                for side in [-1, 1] {
                    let tip = (
                        to.0 - ux * 12 + uy * side * 8,
                        to.1 - uy * 12 + ux * side * 8,
                    );
                    self.line(tip, to, radius, shade);
                }
            }
        }
    }
    /// A circle of 4-pixel blocks, 8 pixels thick.
    fn pixel_circle(&mut self, centre: (i64, i64), radius: i64, color: [u8; 4]) {
        let block = 4;
        for by in (centre.1 - radius - block)..=(centre.1 + radius + block) {
            if by.rem_euclid(block) != 0 {
                continue;
            }
            for bx in (centre.0 - radius - block)..=(centre.0 + radius + block) {
                if bx.rem_euclid(block) != 0 {
                    continue;
                }
                let (cx, cy) = (bx + block / 2 - centre.0, by + block / 2 - centre.1);
                let distance = ((cx * cx + cy * cy) as f64).sqrt();
                if distance <= radius as f64 && distance > (radius - 2 * block) as f64 {
                    for py in 0..block {
                        for px in 0..block {
                            self.put(bx + px, by + py, color);
                        }
                    }
                }
            }
        }
    }
    fn blit(&mut self, source: &Image, x: i64, y: i64) {
        for sy in 0..source.height {
            for sx in 0..source.width {
                let at = (sy * source.width + sx) * 4;
                if source.rgba[at + 3] != 0 {
                    let mut pixel = [0; 4];
                    pixel.copy_from_slice(&source.rgba[at..at + 4]);
                    self.put(x + sx as i64, y + sy as i64, pixel);
                }
            }
        }
    }
    /// Scene numbers and level names drawn with a 3x5 font.
    fn label(&mut self, x: i64, y: i64, text: &str) {
        const DIGITS: [u16; 10] = [
            0x7b6f, 0x2c97, 0x73e7, 0x72cf, 0x5bc9, 0x79cf, 0x79ef, 0x7249, 0x7bef, 0x7bcf,
        ];
        let glyph = |ch: char| match ch {
            'L' => Some(0x4927),
            '-' => Some(0x01c0),
            _ => ch.to_digit(10).map(|d| DIGITS[d as usize]),
        };
        let scale = 4;
        let width = text.len() as i64 * 4 * scale + scale;
        for dy in 0..7 * scale {
            for dx in 0..width {
                self.put(x + dx, y + dy, [0, 0, 0, 220]);
            }
        }
        for (index, ch) in text.chars().enumerate() {
            let Some(bits) = glyph(ch) else { continue };
            for row in 0..5 {
                for column in 0..3 {
                    if bits >> (14 - row * 3 - column) & 1 == 1 {
                        for py in 0..scale {
                            for px in 0..scale {
                                self.put(
                                    x + scale + (index as i64 * 4 + column) * scale + px
                                        - scale / 2,
                                    y + scale + row * scale + py,
                                    [255, 255, 255, 255],
                                );
                            }
                        }
                    }
                }
            }
        }
    }
    fn halve(&self) -> Image {
        let (width, height) = (self.width / 2, self.height / 2);
        let mut rgba = vec![0u8; width * height * 4];
        for y in 0..height {
            for x in 0..width {
                for channel in 0..4 {
                    let sum: u32 = [(0, 0), (1, 0), (0, 1), (1, 1)]
                        .iter()
                        .map(|(ox, oy)| {
                            self.rgba[((y * 2 + oy) * self.width + x * 2 + ox) * 4 + channel] as u32
                        })
                        .sum();
                    rgba[(y * width + x) * 4 + channel] = (sum / 4) as u8;
                }
            }
        }
        Image {
            width,
            height,
            rgba,
        }
    }
    fn png(&self) -> Result<Vec<u8>, String> {
        let mut bytes = Vec::new();
        let mut encoder = png::Encoder::new(&mut bytes, self.width as u32, self.height as u32);
        encoder.set_color(png::ColorType::Rgba);
        encoder.set_depth(png::BitDepth::Eight);
        encoder
            .write_header()
            .map_err(|e| e.to_string())?
            .write_image_data(&self.rgba)
            .map_err(|e| e.to_string())?;
        Ok(bytes)
    }
}

/// Decompress a tagged stream: general, palette or MTF4 LZ by its first byte.
fn decode_tagged(rom: &[u8], start: usize, end: usize) -> Result<Vec<u8>, String> {
    let decoded = match rom.get(start) {
        Some(0) => psynergy::assets::lz::decode_general(rom, start, end, DECODED_LIMIT),
        Some(1) => psynergy::assets::lz::decode_palette(rom, start + 1, end, DECODED_LIMIT),
        _ => psynergy::assets::lz::decode_mtf4_lz(rom, start, end, DECODED_LIMIT),
    };
    decoded
        .map(|(bytes, _)| bytes)
        .map_err(|e| format!("stream at 0x{:08x}: {e}", start + ROM_BASE))
}

/// A map's colour effect: its BLDCNT word and the first BLDALPHA word of the
/// container's blend animation (The Broken Seal slot 4, The Lost Age slot 5,
/// after the extra layer).
#[derive(Clone, Copy)]
struct Blend {
    control: u16,
    alpha: u16,
}
/// The Lost Age's unnamed map plane after the grid (container slot 3), one
/// byte per cell of the 128x128 grid. Its nonzero cells outline each room's
/// walls and obstacles; zero cells inside a room are taken as floor.
fn floor_plane(deriver: &Deriver, container: usize) -> Option<Vec<u8>> {
    if component_slots(&deriver.target) != 7 {
        return None;
    }
    let (base, size) = deriver.directory.resource(container).ok()?;
    let offsets = (0..7)
        .map(|k| u32_at(deriver.rom, base + 0x24 + 4 * k).map(|o| o as usize))
        .collect::<Result<Vec<_>, _>>()
        .ok()?;
    let start = offsets[3];
    if start == 0 {
        return None;
    }
    let end = offsets
        .iter()
        .copied()
        .filter(|o| *o > start)
        .min()
        .unwrap_or(size);
    let plane = decode_tagged(deriver.rom, base + start, base + end).ok()?;
    (plane.len() == CELLS * CELLS).then_some(plane)
}

fn blend(deriver: &Deriver, container: usize) -> Option<Blend> {
    let (base, size) = deriver.directory.resource(container).ok()?;
    let slots = component_slots(&deriver.target);
    let offsets = (0..slots)
        .map(|k| u32_at(deriver.rom, base + 0x24 + 4 * k).map(|o| o as usize))
        .collect::<Result<Vec<_>, _>>()
        .ok()?;
    let start = offsets[if slots == 7 { 5 } else { 4 }];
    if start == 0 {
        return None;
    }
    let end = offsets
        .iter()
        .copied()
        .filter(|o| *o > start)
        .min()
        .unwrap_or(size);
    let words = decode_tagged(deriver.rom, base + start, base + end).ok()?;
    let word = |i: usize| {
        words
            .get(i * 2..i * 2 + 2)
            .map(|b| u16::from_le_bytes([b[0], b[1]]))
    };
    let control = word(0)?;
    (control < 0xfe00 && control >> 6 & 3 == 1).then(|| Blend {
        control,
        alpha: word(1).filter(|w| *w < 0xfe00).unwrap_or(0x0808),
    })
}

/// Layers back to front, as the GBA shows them: the front pixel, alpha
/// blended with the pixel behind it when the map's effect names both layers.
/// A layer opaque over the whole map is a backdrop the scene selects by scroll
/// or window; one in front of drawn layers is kept only when it blends.
fn picture(
    deriver: &Deriver,
    preview: &Preview,
    blend: Option<Blend>,
) -> Result<(Image, Vec<[usize; 3]>), String> {
    let staged = &deriver.staged;
    let binary = &staged.binaries[&preview.map];
    let unpacked = preview.tiles[..3]
        .iter()
        .map(|(source, offset)| {
            staged.tiles[source][offset * 32..offset * 32 + TILE_BANK]
                .iter()
                .flat_map(|byte| [byte & 15, byte >> 4])
                .collect::<Vec<u8>>()
        })
        .collect::<Vec<_>>();
    let field = render_field(&FieldMap {
        parameters: &preview.parameters,
        origins: preview.origins,
        grid: &binary[preview.grid_offset..preview.grid_offset + 65536],
        metatiles: &binary
            [preview.metatile_offset..preview.metatile_offset + preview.metatile_length],
        charblocks: [&unpacked[0], &unpacked[1], &unpacked[2]],
        palettes: preview.banks.len(),
    })?;
    let colors = preview
        .banks
        .iter()
        .map(|bank| staged.banks[*bank].clone())
        .collect::<Vec<_>>();
    let first = |bg: usize| blend.is_some_and(|b| b.control >> bg & 1 == 1);
    let second = |bg: usize| blend.is_some_and(|b| b.control >> (8 + bg) & 1 == 1);
    let mut order = (0..field.layers.len()).collect::<Vec<_>>();
    order.sort_by_key(|&i| {
        (
            std::cmp::Reverse(field.layers[i].priority),
            std::cmp::Reverse(field.layers[i].bg),
        )
    });
    let layers = order
        .iter()
        .map(|&i| {
            [
                field.layers[i].bg,
                field.layers[i].priority as usize,
                field.layers[i].opaque as usize,
            ]
        })
        .collect();
    let backdrop = order.iter().position(|&i| field.layers[i].opaque);
    let order = order
        .iter()
        .enumerate()
        .filter(|(rank, &i)| {
            let layer = &field.layers[i];
            !layer.opaque || Some(*rank) == backdrop || first(layer.bg)
        })
        .map(|(_, &i)| i)
        .collect::<Vec<_>>();
    let (eva, evb) = blend.map_or((16, 0), |b| {
        (
            (b.alpha & 31).min(16) as u32,
            (b.alpha >> 8 & 31).min(16) as u32,
        )
    });
    let mut image = Image::new(field.width, field.height, [0, 0, 0, 0]);
    for at in 0..field.width * field.height {
        // The two front-most drawn pixels and their BG numbers.
        let (mut front, mut behind): (Option<(u16, usize)>, Option<(u16, usize)>) = (None, None);
        for &layer in &order {
            let value = field.layers[layer].pixels[at];
            if value != 0 {
                let color = colors
                    .get(usize::from(value >> 4))
                    .and_then(|bank| bank.get(usize::from(value & 15)))
                    .copied()
                    .unwrap_or(0);
                behind = front;
                front = Some((color, field.layers[layer].bg));
            }
        }
        let Some((color, bg)) = front else { continue };
        let mut rgba = rgba_color(color);
        if first(bg) {
            let (under, alpha) = match behind {
                Some((under, under_bg)) if second(under_bg) => (under, 255),
                Some(_) => (color, 255),
                // Over nothing the light fades toward the map's transparency.
                None => (0, (eva * 255 / 16) as u8),
            };
            let under = rgba_color(under);
            for channel in 0..3 {
                let mixed = (rgba[channel] as u32 * eva + under[channel] as u32 * evb) / 16;
                rgba[channel] = mixed.min(248) as u8;
            }
            rgba[3] = alpha;
        }
        image.rgba[at * 4..at * 4 + 4].copy_from_slice(&rgba);
    }
    Ok((image, layers))
}

struct Scene {
    overlay: usize,
    container: usize,
}
fn scene_record(deriver: &Deriver, index: usize) -> Result<Scene, String> {
    let tables = field_tables(&deriver.target)?;
    let record = tables.scenes - ROM_BASE + index * 8;
    let overlay = u16_at(deriver.rom, record)? as usize;
    let load = u16_at(deriver.rom, record + 4)? as usize;
    let container =
        u16_at(deriver.rom, tables.loads - ROM_BASE + load * 12)? as usize + tables.bias;
    if overlay == 0 {
        return Err(format!("scene {index} has no overlay"));
    }
    Ok(Scene { overlay, container })
}

/// What one scene's overlay says about getting in and out.
struct SceneTables {
    container: usize,
    entrances: BTreeMap<i16, (i64, i64, u16)>,
    /// Exit id to (trigger code, event control).
    triggers: BTreeMap<u32, (u8, u32)>,
    exits: Vec<Exit>,
}

/// One map container: the scenes that load it, its picture cut into rooms.
struct Map {
    container: usize,
    scenes: Vec<usize>,
    image: Image,
    columns: usize,
    /// Room per cell, row by row, for cells that draw something.
    room_of: Vec<Option<usize>>,
    codes: BTreeMap<u8, Vec<(usize, usize)>>,
    blend: Option<Blend>,
    layers: Vec<[usize; 3]>,
    error: Option<String>,
}

/// A room: one connected drawn area of a map, separated from the others by
/// cells that draw nothing but black.
struct Room {
    map: usize,
    /// Cell bounds: left, top, right and bottom, exclusive.
    cells: (usize, usize, usize, usize),
    area: usize,
    image: Image,
    /// The room's own cells within its bounds, and the same grown by a cell
    /// (two cells wider and taller).
    mask: Vec<bool>,
    halo: Vec<bool>,
    /// Own cells that are floor rather than wall; all own cells when the map
    /// has no plane to tell them apart.
    floor: Vec<bool>,
    level: i64,
    position: Option<(i64, i64)>,
}
impl Room {
    fn span(&self) -> (i64, i64) {
        (
            (self.cells.2 - self.cells.0) as i64,
            (self.cells.3 - self.cells.1) as i64,
        )
    }
    /// Whether this room at `at` comes within a cell of `other` at `other_at`.
    /// Whether this room at `at` has floor on floor of `other` at `other_at`;
    /// walls may stand over the room behind them.
    fn overlaps(&self, at: (i64, i64), other: &Room, other_at: (i64, i64)) -> bool {
        let (ax, ay) = (at.0.div_euclid(16), at.1.div_euclid(16));
        let (bx, by) = (other_at.0.div_euclid(16), other_at.1.div_euclid(16));
        let (aw, ah) = self.span();
        let (bw, bh) = other.span();
        let (x0, x1) = (ax.max(bx), (ax + aw).min(bx + bw));
        let (y0, y1) = (ay.max(by), (ay + ah).min(by + bh));
        (y0..y1).any(|y| {
            (x0..x1).any(|x| {
                self.floor[((y - ay) * aw + x - ax) as usize]
                    && other.floor[((y - by) * bw + x - bx) as usize]
            })
        })
    }
    fn touches(&self, at: (i64, i64), other: &Room, other_at: (i64, i64)) -> bool {
        let (ax, ay) = (at.0.div_euclid(16), at.1.div_euclid(16));
        let (bx, by) = (other_at.0.div_euclid(16) - 1, other_at.1.div_euclid(16) - 1);
        let (aw, ah) = self.span();
        let (bw, bh) = (other.span().0 + 2, other.span().1 + 2);
        let (x0, x1) = (ax.max(bx), (ax + aw).min(bx + bw));
        let (y0, y1) = (ay.max(by), (ay + ah).min(by + bh));
        (y0..y1).any(|y| {
            (x0..x1).any(|x| {
                self.mask[((y - ay) * aw + x - ax) as usize]
                    && other.halo[((y - by) * bw + x - bx) as usize]
            })
        })
    }
    fn origin(&self) -> (i64, i64) {
        ((self.cells.0 * 16) as i64, (self.cells.1 * 16) as i64)
    }
    fn at(&self, point: (i64, i64)) -> (i64, i64) {
        let (x, y) = self.position.unwrap_or((0, 0));
        let origin = self.origin();
        (x + point.0 - origin.0, y + point.1 - origin.1)
    }
    fn bounds(&self, at: (i64, i64)) -> (i64, i64, i64, i64) {
        (
            at.0,
            at.1,
            at.0 + self.image.width as i64,
            at.1 + self.image.height as i64,
        )
    }
    /// The way into the room from a map point: toward drawn ground.
    fn inward(&self, point: (i64, i64)) -> (i64, i64) {
        let origin = self.origin();
        let local = (point.0 - origin.0, point.1 - origin.1);
        let ground = |x: i64, y: i64| {
            x >= 0
                && y >= 0
                && (x as usize) < self.image.width
                && (y as usize) < self.image.height
                && self.image.rgba[(y as usize * self.image.width + x as usize) * 4 + 3] != 0
        };
        let reach = |(dx, dy): (i64, i64)| {
            (1..=10)
                .filter(|step| ground(local.0 + dx * step * 16, local.1 + dy * step * 16))
                .count() as i64
        };
        [(-1, 0), (0, 1), (1, 0), (0, -1)]
            .into_iter()
            .max_by_key(|&(dx, dy)| reach((dx, dy)) - reach((-dx, -dy)))
            .unwrap()
    }
}

/// Split a map picture into rooms and cut each one out on a clear ground. A
/// dark doorway or trigger cell between drawn cells still joins them.
fn cut_rooms(
    map_index: usize,
    image: &Image,
    codes: &BTreeMap<u8, Vec<(usize, usize)>>,
    ground: Option<&[u8]>,
) -> (usize, Vec<Option<usize>>, Vec<Room>) {
    let (columns, rows) = (image.width / 16, image.height / 16);
    let mut drawn = (0..columns * rows)
        .map(|cell| {
            let (cx, cy) = (cell % columns, cell / columns);
            (0..256).any(|p| {
                let at = ((cy * 16 + p / 16) * image.width + cx * 16 + p % 16) * 4;
                image.rgba[at + 3] != 0 && image.rgba[at..at + 3] != [0, 0, 0]
            })
        })
        .collect::<Vec<_>>();
    for cells in codes.values() {
        for &(x, y) in cells {
            if x < columns && y < rows {
                drawn[y * columns + x] = true;
            }
        }
    }
    let lit = drawn.clone();
    for cy in 0..rows {
        for cx in 0..columns {
            let at = |x: usize, y: usize| x < columns && y < rows && lit[y * columns + x];
            if at(cx.wrapping_sub(1), cy) && at(cx + 1, cy)
                || at(cx, cy.wrapping_sub(1)) && at(cx, cy + 1)
            {
                drawn[cy * columns + cx] = true;
            }
        }
    }
    let mut room_of = vec![None; columns * rows];
    let mut rooms = Vec::new();
    for start in 0..columns * rows {
        if room_of[start].is_some() || !drawn[start] {
            continue;
        }
        let id = rooms.len();
        room_of[start] = Some(id);
        let mut cells = vec![start];
        let mut next = 0;
        while next < cells.len() {
            let (cx, cy) = (cells[next] % columns, cells[next] / columns);
            next += 1;
            for (nx, ny) in [
                (cx.wrapping_sub(1), cy),
                (cx + 1, cy),
                (cx, cy.wrapping_sub(1)),
                (cx, cy + 1),
            ] {
                if nx < columns
                    && ny < rows
                    && room_of[ny * columns + nx].is_none()
                    && drawn[ny * columns + nx]
                {
                    room_of[ny * columns + nx] = Some(id);
                    cells.push(ny * columns + nx);
                }
            }
        }
        let left = cells.iter().map(|c| c % columns).min().unwrap();
        let right = cells.iter().map(|c| c % columns).max().unwrap() + 1;
        let top = cells.iter().map(|c| c / columns).min().unwrap();
        let bottom = cells.iter().map(|c| c / columns).max().unwrap() + 1;
        let mut cut = Image::new((right - left) * 16, (bottom - top) * 16, [0, 0, 0, 0]);
        for &cell in &cells {
            let (cx, cy) = (cell % columns, cell / columns);
            for p in 0..256 {
                let (x, y) = (cx * 16 + p % 16, cy * 16 + p / 16);
                let from = (y * image.width + x) * 4;
                let to = ((y - top * 16) * cut.width + x - left * 16) * 4;
                // Pure black is the void around rooms, left clear.
                if image.rgba[from..from + 3] != [0, 0, 0] {
                    cut.rgba[to..to + 4].copy_from_slice(&image.rgba[from..from + 4]);
                }
            }
        }
        // Dark pixels joined to that clear ground are the room's outline;
        // clear them too, so rooms meet wall to wall.
        let dark = |rgba: &[u8], at: usize| {
            rgba[at * 4 + 3] != 0 && rgba[at * 4..at * 4 + 3].iter().all(|&c| c <= 64)
        };
        let mut edge = (0..cut.width * cut.height)
            .filter(|&at| cut.rgba[at * 4 + 3] == 0)
            .collect::<Vec<_>>();
        while let Some(at) = edge.pop() {
            let (x, y) = (at % cut.width, at / cut.width);
            for (nx, ny) in [
                (x.wrapping_sub(1), y),
                (x + 1, y),
                (x, y.wrapping_sub(1)),
                (x, y + 1),
            ] {
                if nx < cut.width && ny < cut.height && dark(&cut.rgba, ny * cut.width + nx) {
                    let next = ny * cut.width + nx;
                    cut.rgba[next * 4 + 3] = 0;
                    edge.push(next);
                }
            }
        }
        let (width, height) = (right - left, bottom - top);
        let mut mask = vec![false; width * height];
        let mut halo = vec![false; (width + 2) * (height + 2)];
        let mut floor = vec![false; width * height];
        for &cell in &cells {
            let (x, y) = (cell % columns - left, cell / columns - top);
            mask[y * width + x] = true;
            // Door and trigger cells are thresholds, not floor, so two rooms can
            // share them.
            let threshold = codes
                .values()
                .any(|cells| cells.contains(&(cell % columns, cell / columns)));
            floor[y * width + x] = !threshold
                && ground.is_none_or(|g| g[cell / columns * CELLS + cell % columns] == 0);
            for hy in y..y + 3 {
                for hx in x..x + 3 {
                    halo[hy * (width + 2) + hx] = true;
                }
            }
        }
        rooms.push(Room {
            map: map_index,
            cells: (left, top, right, bottom),
            area: cells.len(),
            image: cut,
            mask,
            halo,
            floor,
            level: 0,
            position: None,
        });
    }
    (columns, room_of, rooms)
}

/// One exit between rooms, or out to the world map when `to` is `None`.
struct Link {
    exit: Exit,
    from: Option<usize>,
    start: (i64, i64),
    to: Option<usize>,
    end: (i64, i64),
    /// Stairs up (+1), stairs down (-1) or a door on the same level (0).
    rise: i64,
}

/// A grid of 16-pixel cells over the drawing: cells a room draws are walls,
/// and cells a line already follows cost more, so lines go around maps and
/// spread apart.
struct Router {
    width: usize,
    height: usize,
    blocked: Vec<bool>,
    used: Vec<u8>,
    /// Open cells joined to the edge of the drawing, not holes inside rooms.
    outer: Vec<bool>,
}
impl Router {
    fn open(&self, x: i64, y: i64) -> bool {
        x >= 0
            && y >= 0
            && (x as usize) < self.width
            && (y as usize) < self.height
            && self.outer[y as usize * self.width + x as usize]
    }
    /// The open cell nearest a door, preferring the side toward `toward`.
    fn outside(&self, door: (i64, i64), toward: (i64, i64)) -> Option<(i64, i64)> {
        (1..=12i64).find_map(|radius| {
            let ring = (-radius..=radius)
                .flat_map(|d| [(d, -radius), (d, radius)])
                .chain((1 - radius..radius).flat_map(|d| [(-radius, d), (radius, d)]));
            ring.map(|(dx, dy)| (door.0 + dx, door.1 + dy))
                .filter(|&(x, y)| self.open(x, y))
                .min_by_key(|&(x, y)| {
                    ((x - door.0).abs() + (y - door.1).abs()) * 64
                        + (x - toward.0).abs()
                        + (y - toward.1).abs()
                })
        })
    }
    /// The cheapest four-way path between open cells: a step costs 10, a turn
    /// 40 and a cell another line uses 30.
    fn path(&self, from: (i64, i64), to: (i64, i64)) -> Option<Vec<(i64, i64)>> {
        use std::cmp::Reverse;
        use std::collections::BinaryHeap;
        const STEPS: [(i64, i64); 4] = [(1, 0), (0, 1), (-1, 0), (0, -1)];
        let index = |(x, y): (i64, i64), d: usize| (y as usize * self.width + x as usize) * 4 + d;
        let mut best = vec![u32::MAX; self.width * self.height * 4];
        let mut previous = vec![usize::MAX; self.width * self.height * 4];
        let mut queue = BinaryHeap::new();
        let guess = |(x, y): (i64, i64)| ((x - to.0).abs() + (y - to.1).abs()) as u32 * 10;
        for d in 0..4 {
            best[index(from, d)] = 0;
            queue.push(Reverse((guess(from), 0u32, from.0, from.1, d)));
        }
        while let Some(Reverse((_, cost, x, y, d))) = queue.pop() {
            if cost > best[index((x, y), d)] {
                continue;
            }
            if (x, y) == to {
                let mut cells = vec![(x, y)];
                let mut at = index((x, y), d);
                while previous[at] != usize::MAX {
                    at = previous[at];
                    let cell = at / 4;
                    cells.push(((cell % self.width) as i64, (cell / self.width) as i64));
                }
                cells.reverse();
                return Some(cells);
            }
            for (next, (dx, dy)) in STEPS.iter().enumerate() {
                let (nx, ny) = (x + dx, y + dy);
                if !self.open(nx, ny) {
                    continue;
                }
                let crowd = self.used[ny as usize * self.width + nx as usize].min(3) as u32 * 30;
                let step = cost + 10 + if next == d { 0 } else { 40 } + crowd;
                let at = index((nx, ny), next);
                if step < best[at] {
                    best[at] = step;
                    previous[at] = index((x, y), d);
                    queue.push(Reverse((step + guess((nx, ny)), step, nx, ny, next)));
                }
            }
        }
        None
    }
    /// Pixel corners of a line from door `a` to door `b`: out of each room to
    /// its nearest open cell, then around every map on the grid.
    fn line(&mut self, a: (i64, i64), b: (i64, i64)) -> Vec<(i64, i64)> {
        let cell = |p: (i64, i64)| (p.0.div_euclid(16), p.1.div_euclid(16));
        let centre = |c: (i64, i64)| (c.0 * 16 + 8, c.1 * 16 + 8);
        let (Some(start), Some(end)) = (
            self.outside(cell(a), cell(b)),
            self.outside(cell(b), cell(a)),
        ) else {
            return vec![a, (b.0, a.1), b];
        };
        let cells = self.path(start, end).unwrap_or_else(|| vec![start, end]);
        for &(x, y) in &cells {
            let at = y as usize * self.width + x as usize;
            self.used[at] = self.used[at].saturating_add(1);
        }
        let mut points = vec![a];
        let first = centre(cells[0]);
        points.push((first.0, a.1));
        for &c in &cells {
            points.push(centre(c));
        }
        let last = centre(*cells.last().unwrap());
        points.push((last.0, b.1));
        points.push(b);
        // Keep only the corners.
        points.dedup();
        let mut corners = vec![points[0]];
        for window in points.windows(3) {
            let (p, q, r) = (window[0], window[1], window[2]);
            if !((p.0 == q.0 && q.0 == r.0) || (p.1 == q.1 && q.1 == r.1)) {
                corners.push(q);
            }
        }
        corners.push(*points.last().unwrap());
        corners
    }
    /// A stub out of a room to its nearest open cell and one cell further.
    fn stub(&mut self, a: (i64, i64)) -> Vec<(i64, i64)> {
        let cell = (a.0.div_euclid(16), a.1.div_euclid(16));
        let Some(open) = self.outside(cell, cell) else {
            return vec![a];
        };
        let (dx, dy) = ((open.0 - cell.0).signum(), (open.1 - cell.1).signum());
        let far = (open.0 + dx, open.1 + dy);
        let centre = |c: (i64, i64)| (c.0 * 16 + 8, c.1 * 16 + 8);
        let near = centre(open);
        vec![a, (near.0, a.1), near, centre(far)]
    }
}

/// Rooms moved together when packing: one level of a component, or one
/// unlinked room. Cells and halo are relative to the top-left of its rooms.
struct Block {
    level: i64,
    rooms: Vec<usize>,
    origin: (i64, i64),
    size: (i64, i64),
    area: usize,
    mask: Vec<bool>,
    halo: Vec<bool>,
    position: Option<(i64, i64)>,
}
impl Block {
    fn new(rooms: &[Room], level: i64, members: Vec<usize>) -> Self {
        let at = |r: usize| rooms[r].position.unwrap();
        let left = members.iter().map(|&r| at(r).0).min().unwrap();
        let top = members.iter().map(|&r| at(r).1).min().unwrap();
        let right = members
            .iter()
            .map(|&r| at(r).0 + rooms[r].span().0 * 16)
            .max()
            .unwrap();
        let bottom = members
            .iter()
            .map(|&r| at(r).1 + rooms[r].span().1 * 16)
            .max()
            .unwrap();
        let (w, h) = ((right - left) / 16, (bottom - top) / 16);
        let mut mask = vec![false; (w * h) as usize];
        let mut halo = vec![false; ((w + 2) * (h + 2)) as usize];
        for &r in &members {
            let (rx, ry) = ((at(r).0 - left) / 16, (at(r).1 - top) / 16);
            let (rw, rh) = rooms[r].span();
            for y in 0..rh {
                for x in 0..rw {
                    if rooms[r].mask[(y * rw + x) as usize] {
                        mask[((ry + y) * w + rx + x) as usize] = true;
                        for hy in ry + y..ry + y + 3 {
                            for hx in rx + x..rx + x + 3 {
                                halo[(hy * (w + 2) + hx) as usize] = true;
                            }
                        }
                    }
                }
            }
        }
        Self {
            level,
            area: members.iter().map(|&r| rooms[r].area).sum(),
            rooms: members,
            origin: (left, top),
            size: (w, h),
            mask,
            halo,
            position: None,
        }
    }
    /// Whether this block at `at` comes within a cell of `other` at `other_at`.
    fn touches(&self, at: (i64, i64), other: &Block, other_at: (i64, i64)) -> bool {
        let (ax, ay) = (at.0.div_euclid(16), at.1.div_euclid(16));
        let (bx, by) = (other_at.0.div_euclid(16) - 1, other_at.1.div_euclid(16) - 1);
        let (aw, ah) = self.size;
        let (bw, bh) = (other.size.0 + 2, other.size.1 + 2);
        let (x0, x1) = (ax.max(bx), (ax + aw).min(bx + bw));
        let (y0, y1) = (ay.max(by), (ay + ah).min(by + bh));
        (y0..y1).any(|y| {
            (x0..x1).any(|x| {
                self.mask[((y - ay) * aw + x - ax) as usize]
                    && other.halo[((y - by) * bw + x - bx) as usize]
            })
        })
    }
}

/// Links between blocks: block and block-local point at each end.
type Join = (usize, (i64, i64), usize, (i64, i64));

/// The stairs from `block` at `at` to placed blocks, plus how far the drawing
/// grows across and down, so blocks without stairs still pack in close.
fn pack_cost(
    blocks: &[Block],
    joins: &[Join],
    block: usize,
    at: (i64, i64),
    others: &[usize],
) -> i64 {
    let point = |p: (i64, i64), place: (i64, i64)| (place.0 + p.0, place.1 + p.1);
    let lines: i64 = joins
        .iter()
        .filter_map(|&(a, pa, b, pb)| {
            let (x, y) = if a == block && others.contains(&b) {
                (point(pa, at), point(pb, blocks[b].position.unwrap()))
            } else if b == block && others.contains(&a) {
                (point(pa, blocks[a].position.unwrap()), point(pb, at))
            } else {
                return None;
            };
            Some((x.0 - y.0).abs() + (x.1 - y.1).abs())
        })
        .sum();
    let spans = others
        .iter()
        .map(|&o| (blocks[o].position.unwrap(), blocks[o].size))
        .chain([(at, blocks[block].size)]);
    let (mut left, mut top, mut right, mut bottom) = (i64::MAX, i64::MAX, i64::MIN, i64::MIN);
    for (p, (w, h)) in spans {
        (left, top) = (left.min(p.0), top.min(p.1));
        (right, bottom) = (right.max(p.0 + w * 16), bottom.max(p.1 + h * 16));
    }
    lines + (right - left) + (bottom - top)
}

/// The free spot for `block` with the lowest pack cost, searched outward
/// from positions that join its stairs, or around the placed drawing.
fn pack_spot(blocks: &[Block], joins: &[Join], block: usize, others: &[usize]) -> (i64, i64) {
    let snap = |p: (i64, i64)| (p.0.div_euclid(16) * 16, p.1.div_euclid(16) * 16);
    let mut ideals = joins
        .iter()
        .filter_map(|&(a, pa, b, pb)| {
            if a == block && others.contains(&b) {
                let there = blocks[b].position.unwrap();
                Some(snap((there.0 + pb.0 - pa.0, there.1 + pb.1 - pa.1)))
            } else if b == block && others.contains(&a) {
                let there = blocks[a].position.unwrap();
                Some(snap((there.0 + pa.0 - pb.0, there.1 + pa.1 - pb.1)))
            } else {
                None
            }
        })
        .take(6)
        .collect::<Vec<_>>();
    if ideals.is_empty() {
        let (w, h) = blocks[block].size;
        for &o in others {
            let (p, (ow, oh)) = (blocks[o].position.unwrap(), blocks[o].size);
            ideals.push((p.0 + ow * 16 + 32, p.1));
            ideals.push((p.0, p.1 + oh * 16 + 32));
            ideals.push((p.0 - w * 16 - 32, p.1));
            ideals.push((p.0, p.1 - h * 16 - 32));
        }
        ideals.truncate(24);
    }
    if ideals.is_empty() {
        return (0, 0);
    }
    let free = |at: (i64, i64)| {
        others
            .iter()
            .all(|&o| !blocks[block].touches(at, &blocks[o], blocks[o].position.unwrap()))
    };
    let mut best: Option<(i64, (i64, i64))> = None;
    for ideal in &ideals {
        let mut first = None;
        for radius in 0..512i64 {
            if first.is_some_and(|f| radius > f + 4) {
                break;
            }
            let ring = (-radius..=radius)
                .flat_map(|d| [(d, -radius), (d, radius)])
                .chain((1 - radius..radius).flat_map(|d| [(-radius, d), (radius, d)]));
            for (dx, dy) in ring {
                let at = (ideal.0 + dx * 16, ideal.1 + dy * 16);
                if free(at) {
                    first.get_or_insert(radius);
                    let cost = pack_cost(blocks, joins, block, at, others);
                    if best.is_none_or(|(c, _)| cost < c) {
                        best = Some((cost, at));
                    }
                }
            }
        }
    }
    best.map_or(ideals[0], |(_, at)| at)
}

struct Family {
    world_exits: Vec<u32>,
    root: (usize, i16),
    /// Where the world map exit arrives: map point, room and inward direction.
    root_link: Option<((i64, i64), usize, (i64, i64))>,
    scenes: BTreeMap<usize, SceneTables>,
    maps: Vec<Map>,
    rooms: Vec<Room>,
    links: Vec<Link>,
    /// Rooms linked to the root room, through doors and stairs either way.
    members: BTreeSet<usize>,
    /// The main component from the root room, then islands, each with the
    /// room its layout starts from.
    components: Vec<(usize, BTreeSet<usize>)>,
    /// Level and top of each drawn band.
    bands: Vec<(i64, (i64, i64))>,
    /// A scene whose map is circled.
    mark: Option<usize>,
    /// Doors meet doors with no lines: levels are packed as layers.
    packed: bool,
    /// Scenes to draw on their own instead of a world map family.
    scope: Option<BTreeSet<usize>>,
    errors: Vec<String>,
}

impl Family {
    fn gather(
        deriver: &mut Deriver,
        root: (usize, i16),
        world_exits: Vec<u32>,
        scope: Option<BTreeSet<usize>>,
    ) -> Self {
        let scene_address = scene_address(&deriver.target);
        let mut family = Family {
            world_exits,
            root,
            root_link: None,
            scenes: BTreeMap::new(),
            maps: Vec::new(),
            rooms: Vec::new(),
            links: Vec::new(),
            members: BTreeSet::new(),
            components: Vec::new(),
            bands: Vec::new(),
            mark: None,
            packed: false,
            scope: scope.clone(),
            errors: Vec::new(),
        };
        let mut queue = match &scope {
            Some(scenes) => scenes.iter().copied().collect(),
            None => std::collections::VecDeque::from([root.0]),
        };
        while let Some(index) = queue.pop_front() {
            if index == WORLD_MAP
                || family.scenes.contains_key(&index)
                || family.scope.as_ref().is_some_and(|s| !s.contains(&index))
            {
                continue;
            }
            let loaded = (|| -> Result<SceneTables, String> {
                let scene = scene_record(deriver, index)?;
                let overlay = Overlay::read(
                    deriver.rom,
                    &deriver.directory,
                    scene.overlay,
                    scene_address,
                )?;
                Ok(SceneTables {
                    container: scene.container,
                    entrances: entrances(&overlay, index)?,
                    triggers: exit_triggers(&overlay, index)
                        .unwrap_or_default()
                        .into_iter()
                        .map(|(code, (exit, control))| (exit, (code, control)))
                        .collect(),
                    exits: exits(&overlay, index)?,
                })
            })();
            let tables = match loaded {
                Ok(tables) => tables,
                Err(error) => {
                    family.errors.push(format!("scene {index}: {error}"));
                    continue;
                }
            };
            // An exit whose destination depends on flags (a ship that docks in
            // several places) is followed only back into this family, or by
            // its default when none of its destinations is known yet.
            let mut groups: BTreeMap<u32, Vec<&Exit>> = BTreeMap::new();
            for exit in &tables.exits {
                groups.entry(exit.exit).or_default().push(exit);
            }
            for group in groups.values() {
                let destinations = group.iter().map(|e| e.to_scene).collect::<BTreeSet<_>>();
                let follow = if destinations.len() == 1 {
                    destinations.into_iter().collect::<Vec<_>>()
                } else {
                    let known = destinations
                        .iter()
                        .copied()
                        .filter(|d| *d == index || family.scenes.contains_key(d) || *d == root.0)
                        .collect::<Vec<_>>();
                    if known.is_empty() {
                        group
                            .iter()
                            .find(|e| e.flag.is_none_or(|f| f == u32::MAX))
                            .map(|e| vec![e.to_scene])
                            .unwrap_or_default()
                    } else {
                        known
                    }
                };
                queue.extend(follow);
            }
            if let Some(map) = family
                .maps
                .iter_mut()
                .find(|m| m.container == tables.container)
            {
                map.scenes.push(index);
            } else {
                let map = family.draw_map(deriver, index, tables.container);
                family.maps.push(map);
            }
            family.scenes.insert(index, tables);
        }
        family.link();
        family
    }

    fn draw_map(&mut self, deriver: &mut Deriver, index: usize, container: usize) -> Map {
        let mut map = Map {
            container,
            scenes: vec![index],
            image: Image::new(16, 16, [0, 0, 0, 0]),
            columns: 1,
            room_of: vec![None],
            codes: BTreeMap::new(),
            blend: blend(deriver, container),
            layers: Vec::new(),
            error: None,
        };
        let result = (|| -> Result<(), String> {
            let before = deriver.output.previews.len();
            deriver.scene(&SceneRequest { index, name: None })?;
            let preview = deriver
                .output
                .previews
                .iter()
                .skip(before)
                .chain(deriver.output.previews.iter())
                .find(|p| p.container == resource_name(container))
                .ok_or("the container was staged without a preview")?;
            let (image, layers) = picture(deriver, preview, map.blend)?;
            let binary = &deriver.staged.binaries[&preview.map];
            for (cell, &code) in binary[preview.grid_offset..preview.grid_offset + CELLS * CELLS]
                .iter()
                .enumerate()
            {
                if code != 0 && code != 255 {
                    map.codes
                        .entry(code)
                        .or_default()
                        .push((cell % CELLS, cell / CELLS));
                }
            }
            let ground = floor_plane(deriver, container);
            let (columns, room_of, rooms) =
                cut_rooms(self.maps.len(), &image, &map.codes, ground.as_deref());
            let offset = self.rooms.len();
            map.room_of = room_of.into_iter().map(|r| r.map(|r| r + offset)).collect();
            (map.image, map.columns, map.layers) = (image, columns, layers);
            self.rooms.extend(rooms);
            Ok(())
        })();
        if let Err(error) = result {
            self.errors.push(format!("scene {index}: {error}"));
            map.error = Some(error);
        }
        map
    }

    fn map_of(&self, scene: usize) -> Option<usize> {
        self.maps.iter().position(|m| m.scenes.contains(&scene))
    }
    /// The room drawing a map point, looking a few cells around void cells.
    fn room_at(&self, map: usize, point: (i64, i64)) -> Option<usize> {
        let map = &self.maps[map];
        if map.columns == 0 {
            return None;
        }
        let rows = map.room_of.len() / map.columns;
        let (cx, cy) = (point.0.div_euclid(16), point.1.div_euclid(16));
        (0..=3i64).find_map(|radius| {
            (-radius..=radius)
                .flat_map(|dy| (-radius..=radius).map(move |dx| (dx, dy)))
                .find_map(|(dx, dy)| {
                    let (x, y) = (cx + dx, cy + dy);
                    (x >= 0 && y >= 0 && (x as usize) < map.columns && (y as usize) < rows)
                        .then(|| map.room_of[y as usize * map.columns + x as usize])
                        .flatten()
                })
        })
    }
    /// The centre of a trigger code's cells in the room holding most of them.
    fn code_point(&self, map: usize, code: u8) -> Option<((i64, i64), Option<usize>)> {
        let cells = self.maps[map].codes.get(&code)?;
        let mut by_room: BTreeMap<Option<usize>, Vec<(usize, usize)>> = BTreeMap::new();
        for &(x, y) in cells {
            by_room
                .entry(self.room_at(map, ((x * 16 + 8) as i64, (y * 16 + 8) as i64)))
                .or_default()
                .push((x, y));
        }
        let (room, cells) = by_room
            .into_iter()
            .max_by_key(|(room, cells)| (room.is_some(), cells.len()))?;
        let n = cells.len() as i64;
        let x = cells.iter().map(|c| c.0 as i64 * 16 + 8).sum::<i64>() / n;
        let y = cells.iter().map(|c| c.1 as i64 * 16 + 8).sum::<i64>() / n;
        Some(((x, y), room))
    }
    /// Where an exit arrives, its room and the way the party walks in. A listed
    /// entrance keeps its position and facing; entrance k is otherwise the
    /// destination map's trigger k, the far side of the door.
    fn arrival(&self, scene: usize, entrance: i16) -> Option<((i64, i64), usize, (i64, i64))> {
        let map = self.map_of(scene)?;
        if let Some(&(x, z, facing)) = self
            .scenes
            .get(&scene)
            .and_then(|t| t.entrances.get(&entrance))
        {
            let room = self.room_at(map, (x, z))?;
            return Some(((x, z), room, direction(facing)));
        }
        let (point, room) = self.code_point(map, u8::try_from(entrance).ok()?)?;
        let room = room?;
        Some((point, room, self.rooms[room].inward(point)))
    }

    fn link(&mut self) {
        let mut links = Vec::new();
        for (&index, tables) in &self.scenes {
            let Some(map) = self.map_of(index) else {
                continue;
            };
            for exit in &tables.exits {
                let outside =
                    exit.to_scene != WORLD_MAP && !self.scenes.contains_key(&exit.to_scene);
                if outside && self.scope.is_none() {
                    continue;
                }
                let (code, control) = tables
                    .triggers
                    .get(&exit.exit)
                    .copied()
                    .unwrap_or((exit.exit as u8, 1));
                let (start, from) = match self.code_point(map, code) {
                    Some((point, room)) => (point, room),
                    None => ((0, 0), None),
                };
                let rise = match control & 0xf0 {
                    0x20 => 1,
                    0x30 => -1,
                    _ => 0,
                };
                let arrival = (exit.to_scene != WORLD_MAP && !outside)
                    .then(|| self.arrival(exit.to_scene, exit.entrance))
                    .flatten();
                if exit.to_scene != WORLD_MAP && !outside && arrival.is_none() {
                    self.errors.push(format!(
                        "scene {index} exit {} reaches scene {} entrance {} nowhere drawn",
                        exit.exit, exit.to_scene, exit.entrance
                    ));
                    continue;
                }
                let (end, to) = match arrival {
                    Some((point, room, _)) => (point, Some(room)),
                    None => (start, None),
                };
                links.push(Link {
                    exit: exit.clone(),
                    from,
                    start,
                    to,
                    end,
                    rise,
                });
            }
        }
        self.links = links;
        self.root_link = self.arrival(self.root.0, self.root.1);
        // Scenes share maps and maps hold unrelated rooms, so the family is the
        // rooms that links join to the room the world map exit arrives in, plus
        // islands: linked rooms that only a script reaches (a warp, say), that
        // share a map with the family and have no world map exit of their own.
        // Drawn on their own, scenes start from their most linked room and
        // keep every linked group.
        let root = match (self.root_link, &self.scope) {
            (Some((_, root, _)), None) => root,
            (_, Some(_)) => match self
                .links
                .iter()
                .flat_map(|l| [l.from, l.to])
                .flatten()
                .max_by_key(|&r| {
                    self.links
                        .iter()
                        .filter(|l| l.from == Some(r) || l.to == Some(r))
                        .count()
                }) {
                Some(room) => room,
                None => return,
            },
            _ => return,
        };
        let component = |start: usize| {
            let mut found = BTreeSet::from([start]);
            let mut queue = vec![start];
            while let Some(room) = queue.pop() {
                for link in &self.links {
                    let other = match (link.from, link.to) {
                        (Some(a), Some(b)) if a == room => b,
                        (Some(a), Some(b)) if b == room => a,
                        _ => continue,
                    };
                    if found.insert(other) {
                        queue.push(other);
                    }
                }
            }
            found
        };
        let main = component(root);
        let mut maps = main
            .iter()
            .map(|&r| self.rooms[r].map)
            .collect::<BTreeSet<_>>();
        let mut components = vec![(root, main.clone())];
        let mut seen = main;
        let linked = self
            .links
            .iter()
            .flat_map(|l| [l.from, l.to])
            .flatten()
            .collect::<BTreeSet<_>>();
        let mut candidates = Vec::new();
        for room in linked {
            if seen.contains(&room) {
                continue;
            }
            let island = component(room);
            seen.extend(island.iter().copied());
            let leaves = self
                .links
                .iter()
                .any(|l| l.to.is_none() && l.from.is_some_and(|r| island.contains(&r)));
            if !leaves || self.scope.is_some() {
                candidates.push(island);
            }
        }
        while let Some(at) = candidates.iter().position(|island| {
            self.scope.is_some() || island.iter().any(|&r| maps.contains(&self.rooms[r].map))
        }) {
            let island = candidates.remove(at);
            maps.extend(island.iter().map(|&r| self.rooms[r].map));
            let start = *island.iter().max_by_key(|&&r| self.rooms[r].area).unwrap();
            components.push((start, island));
        }
        self.members = components
            .iter()
            .flat_map(|(_, rooms)| rooms.iter().copied())
            .collect();
        self.components = components;
    }

    /// Total length of the links between `room`, placed at `at`, and the
    /// placed rooms in `others`: door to door on its level, and only across the
    /// page for stairs, whose levels stack.
    fn cost(&self, room: usize, at: (i64, i64), others: &[usize]) -> i64 {
        let origin = self.rooms[room].origin();
        let here = |p: (i64, i64)| (at.0 + p.0 - origin.0, at.1 + p.1 - origin.1);
        self.links
            .iter()
            .filter_map(|l| {
                let (a, b, other) = match (l.from, l.to) {
                    (Some(f), Some(t)) if f == room && t != room && others.contains(&t) => {
                        (here(l.start), self.rooms[t].at(l.end), t)
                    }
                    (Some(f), Some(t)) if t == room && f != room && others.contains(&f) => {
                        (self.rooms[f].at(l.start), here(l.end), f)
                    }
                    _ => return None,
                };
                let (dx, dy) = ((a.0 - b.0).abs(), (a.1 - b.1).abs());
                Some(
                    match (
                        self.packed,
                        self.rooms[other].level == self.rooms[room].level,
                    ) {
                        // Packed thresholds sit on the entrance; stairs on stairs.
                        (true, true) => dx + dy,
                        (true, false) => dx + dy,
                        (false, true) => dx + dy,
                        (false, false) => dx,
                    },
                )
            })
            .sum()
    }

    /// The free spot for `room` with the shortest links to `others`: search
    /// outward from each position that puts a door on the door it leads to,
    /// a few cells past the first free ring, keeping a cell between rooms.
    fn best_spot(&self, room: usize, others: &[usize]) -> (i64, i64) {
        let level = self.rooms[room].level;
        let origin = self.rooms[room].origin();
        let snap = |p: (i64, i64)| (p.0.div_euclid(16) * 16, p.1.div_euclid(16) * 16);
        let mut ideals = Vec::new();
        for l in &self.links {
            // A door lines up with the entrance it leads to.
            let (local, target) = match (l.from, l.to) {
                (Some(f), Some(t)) if f == room && others.contains(&t) => {
                    (l.start, self.rooms[t].at(l.end))
                }
                (Some(f), Some(t)) if t == room && others.contains(&f) => {
                    (l.end, self.rooms[f].at(l.start))
                }
                _ => continue,
            };
            let ideal = snap((
                target.0 - (local.0 - origin.0),
                target.1 - (local.1 - origin.1),
            ));
            if !ideals.contains(&ideal) && ideals.len() < 6 {
                ideals.push(ideal);
            }
        }
        let neighbours = others
            .iter()
            .copied()
            .filter(|&o| self.rooms[o].level == level)
            .collect::<Vec<_>>();
        let free = |at: (i64, i64)| {
            neighbours.iter().all(|&o| {
                let there = self.rooms[o].position.unwrap();
                if self.packed {
                    !self.rooms[room].overlaps(at, &self.rooms[o], there)
                } else {
                    !self.rooms[room].touches(at, &self.rooms[o], there)
                }
            })
        };
        let mut best: Option<(i64, (i64, i64))> = None;
        for ideal in &ideals {
            let mut first = None;
            for radius in 0..256i64 {
                if first.is_some_and(|f| radius > f + 6) {
                    break;
                }
                let ring = (-radius..=radius)
                    .flat_map(|d| [(d, -radius), (d, radius)])
                    .chain((1 - radius..radius).flat_map(|d| [(-radius, d), (radius, d)]));
                for (dx, dy) in ring {
                    let at = (ideal.0 + dx * 16, ideal.1 + dy * 16);
                    if free(at) {
                        first.get_or_insert(radius);
                        let cost = self.cost(room, at, others);
                        if best.is_none_or(|(c, _)| cost < c) {
                            best = Some((cost, at));
                        }
                    }
                }
            }
        }
        best.map(|(_, at)| at)
            .unwrap_or_else(|| ideals.first().copied().unwrap_or((0, 0)))
    }

    /// Place one component's rooms on levels: stairs change level and a door
    /// keeps it. Rooms join in order of how many links they share with placed
    /// rooms, each at the free spot with the shortest links, then every room is
    /// moved again while that shortens its links.
    fn layout(&mut self, start: usize, rooms: &BTreeSet<usize>) -> Vec<usize> {
        self.rooms[start].position = Some((0, 0));
        self.rooms[start].level = 0;
        let mut placed = vec![start];
        loop {
            let mut best: Option<(usize, usize, usize)> = None;
            for to in rooms
                .iter()
                .copied()
                .filter(|r| self.rooms[*r].position.is_none())
            {
                for &from in &placed {
                    let count = self
                        .links
                        .iter()
                        .filter(|l| {
                            (l.from == Some(from) && l.to == Some(to))
                                || (l.from == Some(to) && l.to == Some(from))
                        })
                        .count();
                    if count > 0 && best.is_none_or(|(_, _, most)| count > most) {
                        best = Some((to, from, count));
                    }
                }
            }
            let Some((to, from, _)) = best else { break };
            let rise = self
                .links
                .iter()
                .find_map(|l| match (l.from, l.to) {
                    (Some(f), Some(t)) if f == from && t == to => Some(l.rise),
                    (Some(f), Some(t)) if f == to && t == from => Some(-l.rise),
                    _ => None,
                })
                .unwrap_or(0);
            self.rooms[to].level = self.rooms[from].level + rise;
            let spot = self.best_spot(to, &placed);
            self.rooms[to].position = Some(spot);
            placed.push(to);
        }
        for _ in 0..4 {
            let mut moved = false;
            for index in 1..placed.len() {
                let room = placed[index];
                let others = placed
                    .iter()
                    .copied()
                    .filter(|&r| r != room)
                    .collect::<Vec<_>>();
                let current = self.rooms[room].position.unwrap();
                let before = self.cost(room, current, &others);
                let spot = self.best_spot(room, &others);
                if self.cost(room, spot, &others) < before {
                    self.rooms[room].position = Some(spot);
                    moved = true;
                }
            }
            if !moved {
                break;
            }
        }
        placed
    }

    /// Lay out each component's rooms level by level, then pack the levels,
    /// the islands and the unlinked rooms as rigid blocks: the stairs between
    /// blocks as short as they can be and the whole drawing compact.
    fn place(&mut self) {
        let mut blocks: Vec<Block> = Vec::new();
        let mut rank = Vec::new();
        for (component, (start, rooms)) in self.components.clone().into_iter().enumerate() {
            let placed = self.layout(start, &rooms);
            let mut levels: BTreeMap<i64, Vec<usize>> = BTreeMap::new();
            for room in placed {
                levels.entry(self.rooms[room].level).or_default().push(room);
            }
            for (level, members) in levels {
                rank.push((component, level.abs(), level));
                blocks.push(Block::new(&self.rooms, level, members));
            }
        }
        // Large rooms no exit touches, in the family's maps, may be unused.
        // Small unlinked pieces are tile stock.
        let maps = self
            .members
            .iter()
            .map(|&r| self.rooms[r].map)
            .collect::<BTreeSet<_>>();
        for index in 0..self.rooms.len() {
            let linked = self
                .links
                .iter()
                .any(|l| l.from == Some(index) || l.to == Some(index));
            let room = &mut self.rooms[index];
            if room.position.is_none() && !linked && room.area >= 64 && maps.contains(&room.map) {
                room.position = Some((0, 0));
                room.level = i64::MIN;
                rank.push((usize::MAX, 0, 0));
                blocks.push(Block::new(&self.rooms, i64::MIN, vec![index]));
            }
        }
        if self.packed {
            // Every level of every linked group is its own panel, the highest
            // on top, one under the next with a wide gap. A panel keeps its
            // place across the page, so stairs stay above the stairs they meet.
            let mut order = (0..blocks.len()).collect::<Vec<_>>();
            order.sort_by_key(|&b| (rank[b].0, std::cmp::Reverse(blocks[b].level)));
            let mut lefts: BTreeMap<usize, i64> = BTreeMap::new();
            for (b, block) in blocks.iter().enumerate() {
                let left = lefts.entry(rank[b].0).or_insert(i64::MAX);
                *left = (*left).min(block.origin.0);
            }
            let mut cursor = 0;
            for block in order {
                let left = lefts[&rank[block].0];
                blocks[block].position = Some((blocks[block].origin.0 - left, cursor));
                cursor += blocks[block].size.1 * 16 + PANEL_GAP;
            }
            for block in &blocks {
                let at = block.position.unwrap();
                for &room in &block.rooms {
                    let (x, y) = self.rooms[room].position.unwrap();
                    self.rooms[room].position =
                        Some((at.0 + x - block.origin.0, at.1 + y - block.origin.1));
                }
                if block.level != i64::MIN {
                    self.bands.push((block.level, at));
                }
            }
            return;
        }
        let block_of = blocks
            .iter()
            .enumerate()
            .flat_map(|(b, block)| block.rooms.iter().map(move |&r| (r, b)))
            .collect::<BTreeMap<_, _>>();
        // Links between blocks, as block-local pixel points.
        let local = |room: usize, point: (i64, i64)| {
            let block = &blocks[block_of[&room]];
            let at = self.rooms[room].position.unwrap();
            let origin = self.rooms[room].origin();
            (
                at.0 - block.origin.0 + point.0 - origin.0,
                at.1 - block.origin.1 + point.1 - origin.1,
            )
        };
        let joins = self
            .links
            .iter()
            .filter_map(|l| {
                let (from, to) = (l.from?, l.to?);
                let (a, b) = (*block_of.get(&from)?, *block_of.get(&to)?);
                (a != b).then(|| (a, local(from, l.start), b, local(to, l.end)))
            })
            .collect::<Vec<_>>();
        let order = {
            let mut order = vec![(0..blocks.len())
                .max_by_key(|&b| blocks[b].area)
                .unwrap_or(0)];
            while order.len() < blocks.len() {
                let next = (0..blocks.len())
                    .filter(|b| !order.contains(b))
                    .max_by_key(|&b| {
                        let count = joins
                            .iter()
                            .filter(|(a, _, c, _)| {
                                (*a == b && order.contains(c)) || (*c == b && order.contains(a))
                            })
                            .count();
                        (count, blocks[b].area)
                    })
                    .unwrap();
                order.push(next);
            }
            order
        };
        let mut placed: Vec<usize> = Vec::new();
        for &block in &order {
            let spot = pack_spot(&blocks, &joins, block, &placed);
            blocks[block].position = Some(spot);
            placed.push(block);
        }
        for _ in 0..4 {
            let mut moved = false;
            for &block in order.iter().skip(1) {
                let others = placed
                    .iter()
                    .copied()
                    .filter(|&b| b != block)
                    .collect::<Vec<_>>();
                let current = blocks[block].position.unwrap();
                let before = pack_cost(&blocks, &joins, block, current, &others);
                let spot = pack_spot(&blocks, &joins, block, &others);
                if pack_cost(&blocks, &joins, block, spot, &others) < before {
                    blocks[block].position = Some(spot);
                    moved = true;
                }
            }
            if !moved {
                break;
            }
        }
        for block in &blocks {
            let at = block.position.unwrap();
            for &room in &block.rooms {
                let (x, y) = self.rooms[room].position.unwrap();
                self.rooms[room].position =
                    Some((at.0 + x - block.origin.0, at.1 + y - block.origin.1));
            }
            if block.level != i64::MIN {
                self.bands.push((block.level, at));
            }
        }
    }

    fn draw(&self) -> Image {
        let shown = (0..self.rooms.len())
            .filter(|&r| self.rooms[r].position.is_some())
            .collect::<Vec<_>>();
        let boxes = shown
            .iter()
            .map(|&r| self.rooms[r].bounds(self.rooms[r].position.unwrap()))
            .collect::<Vec<_>>();
        let margin = 192;
        let left = boxes.iter().map(|b| b.0).min().unwrap_or(0) - margin;
        let top = boxes.iter().map(|b| b.1).min().unwrap_or(0) - margin;
        let right = boxes.iter().map(|b| b.2).max().unwrap_or(0) + margin;
        let bottom = boxes.iter().map(|b| b.3).max().unwrap_or(0) + margin;
        let ground = if self.packed {
            [0, 0, 0, 255]
        } else {
            [0, 0, 0, 0]
        };
        let mut canvas = Image::new((right - left) as usize, (bottom - top) as usize, ground);
        let shift = |p: (i64, i64)| (p.0 - left, p.1 - top);
        // Rooms further down the page are drawn later, so their walls stand
        // in front of the rooms behind them.
        let mut layered = shown.clone();
        layered.sort_by_key(|&r| {
            let (x, y) = self.rooms[r].position.unwrap();
            (y, x)
        });
        for &r in &layered {
            let at = shift(self.rooms[r].position.unwrap());
            canvas.blit(&self.rooms[r].image, at.0, at.1);
        }
        if self.packed {
            for &(level, at) in &self.bands {
                let at = shift(at);
                canvas.label(at.0, at.1 - 96, &format!("L{level}"));
            }
        }
        let visible =
            |room: Option<usize>| room.filter(|r| self.members.contains(r) && !self.packed);
        let mut router = Router {
            width: canvas.width / 16,
            height: canvas.height / 16,
            blocked: vec![false; (canvas.width / 16) * (canvas.height / 16)],
            used: vec![0; (canvas.width / 16) * (canvas.height / 16)],
            outer: vec![false; (canvas.width / 16) * (canvas.height / 16)],
        };
        for &r in &shown {
            let room = &self.rooms[r];
            let at = shift(room.position.unwrap());
            let (w, h) = room.span();
            for y in 0..h {
                for x in 0..w {
                    if room.mask[(y * w + x) as usize] {
                        let (cx, cy) = (at.0 / 16 + x, at.1 / 16 + y);
                        router.blocked[cy as usize * router.width + cx as usize] = true;
                    }
                }
            }
        }
        let mut edge = (0..router.width)
            .flat_map(|x| [(x, 0), (x, router.height - 1)])
            .chain((0..router.height).flat_map(|y| [(0, y), (router.width - 1, y)]))
            .collect::<Vec<_>>();
        while let Some((x, y)) = edge.pop() {
            let at = y * router.width + x;
            if router.blocked[at] || router.outer[at] {
                continue;
            }
            router.outer[at] = true;
            for (nx, ny) in [
                (x.wrapping_sub(1), y),
                (x + 1, y),
                (x, y.wrapping_sub(1)),
                (x, y + 1),
            ] {
                if nx < router.width && ny < router.height {
                    edge.push((nx, ny));
                }
            }
        }
        // A door and its way back are one line; a one-way exit ends in a head.
        let mut drawn = vec![false; self.links.len()];
        for (index, link) in self.links.iter().enumerate() {
            if drawn[index] {
                continue;
            }
            drawn[index] = true;
            let Some(from) = visible(link.from) else {
                continue;
            };
            let start = self.rooms[from].at(link.start);
            let Some(to) = visible(link.to) else {
                if link.to.is_none() {
                    let stub = router.stub(shift(start));
                    let out = *stub.last().unwrap();
                    canvas.dashed(&stub, LINK, false);
                    canvas.disc(out.0, out.1, 7, [0, 0, 0, 255]);
                    canvas.disc(out.0, out.1, 5, WORLD_MAP_MARK);
                }
                continue;
            };
            let end = self.rooms[to].at(link.end);
            let near = |a: (i64, i64), b: (i64, i64)| (a.0 - b.0).abs() + (a.1 - b.1).abs() <= 64;
            let back = self.links.iter().enumerate().position(|(other, l)| {
                !drawn[other]
                    && l.from == link.to
                    && l.to == link.from
                    && near(self.rooms[to].at(l.start), end)
                    && near(self.rooms[from].at(l.end), start)
            });
            if let Some(back) = back {
                drawn[back] = true;
            }
            let points = router.line(shift(start), shift(end));
            canvas.dashed(&points, LINK, back.is_none());
        }
        if let Some((point, room, (dx, dy))) = self.root_link.filter(|_| !self.packed) {
            let end = self.rooms[room].at(point);
            let covered = self.links.iter().any(|l| {
                l.to.is_none()
                    && l.from == Some(room)
                    && (self.rooms[room].at(l.start).0 - end.0).abs()
                        + (self.rooms[room].at(l.start).1 - end.1).abs()
                        <= 64
            });
            if !covered {
                let _ = (dx, dy);
                let mut stub = router.stub(shift(end));
                stub.reverse();
                let out = stub[0];
                canvas.dashed(&stub, LINK, true);
                canvas.disc(out.0, out.1, 7, [0, 0, 0, 255]);
                canvas.disc(out.0, out.1, 5, WORLD_MAP_MARK);
            }
        }
        // A marked scene gets a pixel circle around the rooms of its map.
        if let Some(scene) = self.mark {
            let marked = shown
                .iter()
                .copied()
                .filter(|&r| {
                    self.members.contains(&r)
                        && self.maps[self.rooms[r].map].scenes.contains(&scene)
                })
                .map(|r| self.rooms[r].bounds(self.rooms[r].position.unwrap()))
                .collect::<Vec<_>>();
            if !marked.is_empty() {
                let l = marked.iter().map(|b| b.0).min().unwrap();
                let t = marked.iter().map(|b| b.1).min().unwrap();
                let r = marked.iter().map(|b| b.2).max().unwrap();
                let b = marked.iter().map(|b| b.3).max().unwrap();
                let centre = shift(((l + r) / 2, (t + b) / 2));
                let radius = (((r - l).pow(2) + (b - t).pow(2)) as f64).sqrt() as i64 / 2 + 40;
                canvas.pixel_circle(centre, radius, MARK);
            }
        }
        // Scene numbers and level over the largest shown room of each map on
        // each level.
        let mut labelled: BTreeMap<(usize, i64), usize> = BTreeMap::new();
        for &r in &shown {
            let key = (self.rooms[r].map, self.rooms[r].level);
            let best = labelled.entry(key).or_insert(r);
            if self.rooms[r].area > self.rooms[*best].area {
                *best = r;
            }
        }
        for ((map, level), room) in labelled.into_iter().filter(|_| !self.packed) {
            let at = shift(self.rooms[room].position.unwrap());
            let mut text = self.maps[map]
                .scenes
                .iter()
                .map(usize::to_string)
                .collect::<Vec<_>>()
                .join(" ");
            if level != i64::MIN {
                text.push_str(&format!(" L{level}"));
            }
            canvas.label(at.0 + 4, at.1 - 28, &text);
        }
        while canvas.width.max(canvas.height) > MAX_EDGE {
            canvas = canvas.halve();
        }
        canvas
    }

    fn report(&self) -> Value {
        json!({
            "world_map_exits": self.world_exits,
            "root": {"scene": self.root.0, "entrance": self.root.1},
            "maps": self.maps.iter().map(|m| json!({
                "container": resource_name(m.container),
                "scenes": m.scenes,
                "blend": m.blend.map(|b| [format!("0x{:04x}", b.control), format!("0x{:04x}", b.alpha)]),
                "layers": m.layers,
                "codes": m.codes.keys().collect::<Vec<_>>(),
                "entrances": m.scenes.iter().map(|s| (s.to_string(), json!(self.scenes.get(s).map(|t| t.entrances.keys().collect::<Vec<_>>())))).collect::<serde_json::Map<_, _>>(),
                "error": m.error,
            })).collect::<Vec<_>>(),
            "rooms": self.rooms.iter().enumerate().filter(|(_, r)| r.position.is_some()).map(|(i, r)| json!({
                "room": i,
                "container": resource_name(self.maps[r.map].container),
                "cells": [r.cells.0, r.cells.1, r.cells.2, r.cells.3],
                "level": (r.level != i64::MIN).then_some(r.level),
                "position": r.position.map(|p| [p.0, p.1]),
            })).collect::<Vec<_>>(),
            "links": self.links.iter().map(|l| json!({
                "scene": l.exit.scene,
                "exit": l.exit.exit,
                "flag": l.exit.flag.map(|f| format!("0x{f:x}")),
                "to_scene": l.exit.to_scene,
                "entrance": l.exit.entrance,
                "from_room": l.from,
                "to_room": l.to,
                "rise": l.rise,
            })).collect::<Vec<_>>(),
            "errors": self.errors,
        })
    }
}

const USAGE: &str =
    "usage: alchemy build assets --network ROM --target TARGET -o DIR [--from WORLD_MAP_EXIT | --scenes LIST] [--mark SCENE] [--packed]";

pub(in crate::build_assets) fn run(root: &Path, arguments: &[String]) -> Result<(), String> {
    let (mut rom_path, mut target, mut output, mut from, mut mark, mut scenes) =
        (None, None, None, None, None, None);
    let mut packed = false;
    let mut rest = arguments.iter();
    while let Some(argument) = rest.next() {
        let slot = match argument.as_str() {
            "--packed" => {
                packed = true;
                continue;
            }
            "--target" => &mut target,
            "-o" | "--output" => &mut output,
            "--from" => &mut from,
            "--mark" => &mut mark,
            "--scenes" => &mut scenes,
            _ if !argument.starts_with('-') && rom_path.is_none() => {
                rom_path = Some(argument.clone());
                continue;
            }
            _ => return Err(USAGE.into()),
        };
        *slot = Some(rest.next().ok_or(USAGE)?.clone());
    }
    let (Some(rom_path), Some(target), Some(output)) = (rom_path, target, output) else {
        return Err(USAGE.into());
    };
    let output = ignored_output_path(root, Path::new(&output), "map networks")?;
    let from = from
        .map(|value| {
            value
                .parse::<u32>()
                .map_err(|_| format!("invalid world map exit {value:?}"))
        })
        .transpose()?;
    let mark = mark
        .map(|value| {
            value
                .parse::<usize>()
                .map_err(|_| format!("invalid scene {value:?}"))
        })
        .transpose()?;
    let target = decomp_target(Some(&target))?;
    let rom = fs::read(&rom_path).map_err(|e| format!("{rom_path}: {e}"))?;
    let mut deriver = Deriver {
        rom: &rom,
        directory: Directory::read(&rom)?,
        paths: NativePaths::of(&target),
        target,
        index: None,
        staged: Staged::default(),
        seen: BTreeSet::new(),
        chr_banks: BTreeMap::new(),
        output: Output {
            scenes: vec![],
            layouts: vec![],
            regions: vec![],
            bindings: vec![],
            private_inputs: vec![],
            previews: vec![],
        },
    };
    if let Some(list) = scenes {
        let mut scope = BTreeSet::new();
        for item in list.split(',') {
            let (first, last) = item.split_once('-').unwrap_or((item, item));
            let parse = |v: &str| {
                v.trim()
                    .parse::<usize>()
                    .map_err(|_| format!("invalid scene {v:?}"))
            };
            scope.extend(parse(first)?..=parse(last)?);
        }
        let first = *scope.first().ok_or("--scenes names no scene")?;
        let mut family = Family::gather(&mut deriver, (first, -1), Vec::new(), Some(scope));
        family.mark = mark;
        family.packed = packed;
        family.place();
        fs::create_dir_all(&output).map_err(|e| e.to_string())?;
        let path = output.join(format!("NETWORK_SCENES_{}.PNG", list.replace(',', "_")));
        fs::write(&path, family.draw().png()?).map_err(|e| e.to_string())?;
        eprintln!(
            "network={} rooms={} links={}",
            path.display(),
            family.members.len(),
            family.links.len()
        );
        let text = canonical_json(
            &json!({"format":"alchemy-map-network-v2","target":deriver.target.id.as_str(),"families":[family.report()]}),
        );
        fs::write(
            output.join("NETWORK.JSON"),
            format!(
                "{text}
"
            ),
        )
        .map_err(|e| e.to_string())?;
        return Ok(());
    }
    let world = scene_record(&deriver, WORLD_MAP)?;
    let overlay = Overlay::read(
        &rom,
        &deriver.directory,
        world.overlay,
        scene_address(&deriver.target),
    )?;
    // One family per destination; several world map exits may enter it.
    let mut roots: BTreeMap<usize, (i16, Vec<u32>)> = BTreeMap::new();
    for exit in exits(&overlay, WORLD_MAP)? {
        if exit.to_scene == WORLD_MAP || from.is_some_and(|from| from != exit.exit) {
            continue;
        }
        roots
            .entry(exit.to_scene)
            .or_insert((exit.entrance, Vec::new()))
            .1
            .push(exit.exit);
    }
    if roots.is_empty() {
        return Err("no world map exit matches".into());
    }
    fs::create_dir_all(&output).map_err(|e| e.to_string())?;
    let mut reports = Vec::new();
    let mut drawn: Vec<BTreeSet<(usize, (usize, usize, usize, usize))>> = Vec::new();
    for (scene, (entrance, world_exits)) in roots {
        let mut family = Family::gather(&mut deriver, (scene, entrance), world_exits, None);
        family.mark = mark;
        family.packed = packed;
        let rooms = family
            .members
            .iter()
            .map(|&r| {
                (
                    family.maps[family.rooms[r].map].container,
                    family.rooms[r].cells,
                )
            })
            .collect::<BTreeSet<_>>();
        if rooms.is_empty() || drawn.contains(&rooms) {
            continue;
        }
        drawn.push(rooms);
        family.place();
        let path = output.join(format!(
            "NETWORK_{:03}_SCENE_{scene:03}.PNG",
            family.world_exits[0]
        ));
        fs::write(&path, family.draw().png()?).map_err(|e| e.to_string())?;
        eprintln!(
            "network={} maps={} rooms={} links={}",
            path.display(),
            family.maps.len(),
            family.rooms.iter().filter(|r| r.position.is_some()).count(),
            family.links.len()
        );
        let mut report = family.report();
        report["image"] = json!(path.file_name().map(|n| n.to_string_lossy().to_string()));
        reports.push(report);
    }
    let text = canonical_json(
        &json!({"format":"alchemy-map-network-v2","target":deriver.target.id.as_str(),"families":reports}),
    );
    fs::write(output.join("NETWORK.JSON"), format!("{text}\n")).map_err(|e| e.to_string())?;
    Ok(())
}
