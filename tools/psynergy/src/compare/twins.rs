//! Relocation-masked twins: one compiled function linked at other addresses
//! or into other images. Linking changes the offsets of calls that leave a
//! body and the literal words it loads, so those are masked; instructions,
//! calls within the body and jump tables relative to the body compare
//! exactly. Callers supply loaded images and complete function extents; no
//! owner, layout or default image belongs here.
use crate::thumb::{relocation_info, stub_target};
use std::ops::Range;

/// Shorter bodies recur too widely to be evidence of one function.
pub const MINIMUM_BODY: usize = 12;
/// Near twins are searched for bodies of at least this length.
pub const NEAR_MINIMUM_BODY: usize = 48;
/// A near twin has the same length and differs in 1 to this many units: a
/// halfword, a `bl` pair or a data word.
pub const NEAR_UNITS: usize = 8;

const CODE: u16 = 0;
const LOCAL: u16 = 1 << 8;
const CALL: u16 = 2 << 8;
const LITERAL: u16 = 3 << 8;
const TABLE: u16 = 4 << 8;

/// An image as it executes from `base`, with PC-relative `bl` pairs.
pub struct Image<'a> {
    pub bytes: &'a [u8],
    pub base: u32,
    /// The image mapped beneath this one while it runs, such as the main
    /// program under a loaded overlay. Addresses that leave an image land
    /// there.
    pub beneath: Option<usize>,
}
impl Image<'_> {
    pub fn contains(&self, address: u32) -> bool {
        address
            .checked_sub(self.base)
            .is_some_and(|offset| (offset as usize) < self.bytes.len())
    }
}

/// Where a body sits: its image and first address.
pub type Site = (usize, u32);

/// A body's relocation-masked form; equal signatures are twins.
#[derive(Clone, Debug, PartialEq, Eq, Hash)]
pub struct Signature(Vec<u16>);
impl Signature {
    pub fn len(&self) -> usize {
        self.0.len()
    }
    pub fn is_empty(&self) -> bool {
        self.0.is_empty()
    }
    fn plain(&self, range: Range<usize>, roles: &[u16]) -> bool {
        self.0[range]
            .iter()
            .all(|unit| roles.contains(&(unit & 0xff00)))
    }
    fn word(&self, at: usize) -> u32 {
        u32::from_le_bytes([0, 1, 2, 3].map(|index| self.0[at + index] as u8))
    }
    /// Unit boundaries: relocated words and `bl` pairs are one unit each,
    /// everything else a halfword.
    fn units(&self) -> Vec<Range<usize>> {
        let mut units = Vec::new();
        let mut at = 0;
        while at + 1 < self.0.len() {
            let width = if self.0[at] & 0xff00 == CODE { 2 } else { 4 };
            let end = (at + width).min(self.0.len());
            units.push(at..end);
            at = end;
        }
        units
    }
    /// Differing units, measured on this signature's unit boundaries.
    pub fn differing_units(&self, other: &Self) -> Option<usize> {
        (self.0.len() == other.0.len()).then(|| {
            self.units()
                .into_iter()
                .filter(|unit| self.0[unit.clone()] != other.0[unit.clone()])
                .count()
        })
    }
}

/// The signature of exactly `length` bytes at `start`, and each masked
/// reference by offset: a leaving call's target or a literal word's value.
fn read(image: &Image, start: u32, length: usize) -> Option<(Signature, Vec<(usize, bool, u32)>)> {
    let offset = start.checked_sub(image.base)? as usize;
    let bytes = image.bytes.get(offset..offset.checked_add(length)?)?;
    let end = u64::from(start) + length as u64;
    let inside = |address: u32| (u64::from(start)..end).contains(&u64::from(address));
    let mut units = bytes
        .iter()
        .map(|byte| u16::from(*byte))
        .collect::<Vec<_>>();
    let (_, sites) = relocation_info(bytes, u64::from(start));
    let mut references = Vec::new();
    for site in sites.iter().filter(|site| site.0 == b'B') {
        let pair = site.1..site.1 + 4;
        if inside(site.3) {
            units[pair].iter_mut().for_each(|unit| *unit |= LOCAL);
        } else {
            // The ten opcode bits stay; the 22 offset bits are masked.
            for (at, keep) in pair.zip([0, 0xf8, 0, 0xf8]) {
                units[at] = CALL | (units[at] & keep);
            }
            references.push((site.1, true, site.3));
        }
    }
    for at in (0..length.saturating_sub(1)).step_by(2) {
        if units[at] & 0xff00 != CODE
            || u16::from_le_bytes([bytes[at], bytes[at + 1]]) & 0xffc7 != 0x4687
        {
            continue;
        }
        // `mov pc, rN` jumps through the word-aligned table that follows it.
        let mut word = (at + 2 + ((start as usize + at + 2) & 2)).min(length);
        while word + 4 <= length {
            let target = u32::from_le_bytes(bytes[word..word + 4].try_into().unwrap());
            if !inside(target & !1)
                || units[word..word + 4]
                    .iter()
                    .any(|unit| unit & 0xff00 != CODE)
            {
                break;
            }
            let relative = target.wrapping_sub(start).to_le_bytes();
            for (index, byte) in relative.into_iter().enumerate() {
                units[word + index] = TABLE | u16::from(byte);
            }
            word += 4;
        }
    }
    for site in sites.iter().filter(|site| site.0 == b'L') {
        // A load decoded from data already claimed as a word is not code.
        if units[site.1..site.1 + 2]
            .iter()
            .any(|unit| matches!(unit & 0xff00, LITERAL | TABLE))
        {
            continue;
        }
        units[site.2..site.2 + 4].fill(LITERAL);
        references.push((site.2, false, site.3));
    }
    references.sort_unstable_by_key(|reference| (reference.0, reference.1));
    references.dedup_by_key(|reference| reference.0);
    references.retain(|reference| !reference.1 || units[reference.0] == CALL);
    Some((Signature(units), references))
}

/// A complete function extent read as a twin signature.
#[derive(Clone, Debug)]
pub struct Body {
    pub image: usize,
    pub start: u32,
    pub signature: Signature,
}
impl Body {
    /// A complete extent without its trailing zero alignment halfword; `None`
    /// when it leaves the image or is shorter than `MINIMUM_BODY`.
    pub fn read(images: &[Image], image: usize, start: u32, extent: usize) -> Option<Self> {
        let (mut signature, _) = read(images.get(image)?, start, extent)?;
        let aligned = (u64::from(start) + extent as u64).is_multiple_of(4);
        if aligned && extent >= 2 && signature.0[extent - 2..] == [CODE, CODE] {
            signature.0.truncate(extent - 2);
        }
        (signature.len() >= MINIMUM_BODY).then_some(Self {
            image,
            start,
            signature,
        })
    }
    /// Exactly `length` bytes at `site`, with no alignment removed.
    pub fn at(images: &[Image], site: Site, length: usize) -> Option<Self> {
        let (signature, _) = read(images.get(site.0)?, site.1, length)?;
        Some(Self {
            image: site.0,
            start: site.1,
            signature,
        })
    }
    pub fn len(&self) -> usize {
        self.signature.len()
    }
    pub fn is_empty(&self) -> bool {
        self.signature.is_empty()
    }
}

/// What a masked reference binds: a leaving call's final target, with the
/// image it lands in, or a literal word.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Reference {
    Call(Option<usize>, u32),
    Literal(u32),
}

/// The image an address lands in from `image`: itself, then the images
/// beneath it.
pub fn locate(images: &[Image], image: usize, address: u32) -> Option<usize> {
    let mut current = Some(image);
    for _ in 0..images.len() {
        let found = current?;
        if images[found].contains(address) {
            return Some(found);
        }
        current = images[found].beneath;
    }
    None
}

/// Where a call to `address` from `image` lands after long-branch stubs
/// (`ldr rN, [pc]; bx rN; .word`), such as an overlay import veneer and then
/// the main program's far-call veneer.
pub fn call_target(images: &[Image], image: usize, address: u32) -> Reference {
    let (mut from, mut address) = (image, address);
    for _ in 0..16 {
        let Some(found) = locate(images, from, address) else {
            break;
        };
        let offset = (address - images[found].base) as usize;
        match stub_target(&images[found].bytes[offset..]).map(|(_, target)| target) {
            Some(next) if offset.is_multiple_of(4) && next & 1 != 0 => {
                (from, address) = (found, next & !1);
            }
            Some(next) if offset.is_multiple_of(4) => {
                return Reference::Call(locate(images, found, next), next)
            }
            _ => return Reference::Call(Some(found), address),
        }
    }
    Reference::Call(locate(images, from, address), address)
}

/// A body's references in order: final call targets and literal values.
pub fn references(images: &[Image], site: Site, length: usize) -> Vec<Reference> {
    let Some((_, found)) = images
        .get(site.0)
        .and_then(|image| read(image, site.1, length))
    else {
        return Vec::new();
    };
    found
        .into_iter()
        .map(|(_, call, value)| match call {
            true => call_target(images, site.0, value),
            false => Reference::Literal(value),
        })
        .collect()
}

/// Whether two twins bind alike: every call lands on the same function, or
/// both land in their own images on functions `twins` accepts, and every
/// literal is the same value or points into its own image, whose data may
/// differ.
pub fn equivalent(
    images: &[Image],
    left: (usize, &[Reference]),
    right: (usize, &[Reference]),
    twins: impl Fn(Site, Site) -> bool,
) -> bool {
    left.1.len() == right.1.len()
        && left.1.iter().zip(right.1).all(|pair| match pair {
            (Reference::Call(a, x), Reference::Call(b, y)) => {
                (a, x) == (b, y)
                    || *a == Some(left.0)
                        && *b == Some(right.0)
                        && twins((left.0, *x), (right.0, *y))
            }
            (Reference::Literal(x), Reference::Literal(y)) => {
                x == y || images[left.0].contains(*x) && images[right.0].contains(*y)
            }
            _ => false,
        })
}

/// Every even-offset word of a set of images, sorted for anchor lookup.
pub struct Index {
    keys: Vec<u64>,
    starts: Vec<usize>,
}
impl Index {
    pub fn new(images: &[Image]) -> Self {
        let mut starts = Vec::with_capacity(images.len());
        let mut keys = Vec::new();
        let mut position = 0;
        for image in images {
            starts.push(position);
            for at in (0..image.bytes.len().saturating_sub(3)).step_by(2) {
                let word = u32::from_le_bytes(image.bytes[at..at + 4].try_into().unwrap());
                keys.push(u64::from(word) << 32 | (position + at) as u64);
            }
            position += image.bytes.len();
        }
        keys.sort_unstable();
        Self { keys, starts }
    }
    fn range(&self, word: u32) -> Range<usize> {
        let first = self
            .keys
            .partition_point(|key| (key >> 32) < u64::from(word));
        let last = self
            .keys
            .partition_point(|key| (key >> 32) <= u64::from(word));
        first..last
    }
    /// Starts of bodies whose anchor at `at` holds `word`.
    fn starts(&self, images: &[Image], word: u32, at: usize, length: usize) -> Vec<Site> {
        self.keys[self.range(word)]
            .iter()
            .filter_map(|key| {
                let position = (*key & u64::from(u32::MAX)) as usize;
                let image = self.starts.partition_point(|start| *start <= position) - 1;
                let offset = (position - self.starts[image]).checked_sub(at)?;
                (offset + length <= images[image].bytes.len())
                    .then(|| (image, images[image].base + offset as u32))
            })
            .collect()
    }
    /// Every site whose signature equals `body`'s, including its own.
    pub fn sites(&self, images: &[Image], body: &Body) -> Vec<Site> {
        let signature = &body.signature;
        let length = signature.len();
        let anchor = (0..length.saturating_sub(3))
            .step_by(2)
            .filter(|at| signature.plain(*at..*at + 4, &[CODE, LOCAL]))
            .min_by_key(|at| self.range(signature.word(*at)).len());
        let candidates = match anchor {
            Some(at) => self.starts(images, signature.word(at), at, length),
            None => scan(images, signature),
        };
        verified(images, signature, candidates)
    }
    /// Sites of the same length that differ from `body` in 1 to `NEAR_UNITS`
    /// units, with that count, found through disjoint exact anchors. The flag
    /// is true when the anchors guarantee every such site was found.
    pub fn near(&self, images: &[Image], body: &Body) -> (Vec<(Site, usize)>, bool) {
        let signature = &body.signature;
        let length = signature.len();
        if length < NEAR_MINIMUM_BODY {
            return (Vec::new(), false);
        }
        let mut windows = (0..length.saturating_sub(3))
            .step_by(2)
            .filter(|at| signature.plain(*at..*at + 4, &[CODE]))
            .map(|at| (self.range(signature.word(at)).len(), at))
            .collect::<Vec<_>>();
        windows.sort_unstable();
        let mut anchors: Vec<usize> = Vec::new();
        for (_, at) in windows {
            if anchors.len() <= NEAR_UNITS && anchors.iter().all(|other| at.abs_diff(*other) >= 4) {
                anchors.push(at);
            }
        }
        let mut candidates = anchors
            .iter()
            .flat_map(|at| self.starts(images, signature.word(*at), *at, length))
            .collect::<Vec<_>>();
        candidates.sort_unstable();
        candidates.dedup();
        let sites = candidates
            .into_iter()
            .filter_map(|site| {
                let (found, _) = read(&images[site.0], site.1, length)?;
                let differing = signature.differing_units(&found)?;
                (1..=NEAR_UNITS)
                    .contains(&differing)
                    .then_some((site, differing))
            })
            .collect();
        (sites, anchors.len() > NEAR_UNITS)
    }
}

/// Every site whose signature equals `body`'s, by scanning each image: one
/// query needs no index.
pub fn find(images: &[Image], body: &Body) -> Vec<Site> {
    verified(images, &body.signature, scan(images, &body.signature))
}

fn verified(images: &[Image], signature: &Signature, candidates: Vec<Site>) -> Vec<Site> {
    let mut sites = candidates
        .into_iter()
        .filter(|site| {
            read(&images[site.0], site.1, signature.len())
                .is_some_and(|found| found.0 == *signature)
        })
        .collect::<Vec<_>>();
    sites.sort_unstable();
    sites.dedup();
    sites
}

/// Candidates by linear scan: every position matching the body's first exact
/// word, else its first exact halfword, else every even position.
fn scan(images: &[Image], signature: &Signature) -> Vec<Site> {
    let length = signature.len();
    let exact = |width: usize| {
        (0..length.saturating_sub(width - 1))
            .step_by(2)
            .find(|at| signature.plain(*at..*at + width, &[CODE, LOCAL]))
            .map(|at| at..at + width)
    };
    let anchor = exact(4).or_else(|| exact(2));
    images
        .iter()
        .enumerate()
        .flat_map(|(index, image)| {
            let anchor = anchor.clone();
            (0..(image.bytes.len() + 1).saturating_sub(length))
                .step_by(2)
                .filter(move |offset| {
                    anchor.clone().is_none_or(|range| {
                        range
                            .clone()
                            .all(|at| image.bytes[offset + at] == signature.0[at] as u8)
                    })
                })
                .map(move |offset| (index, image.base + offset as u32))
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    const PUSH: [u8; 2] = [0x30, 0xb5]; // push {r4, r5, lr}
    const POP: [u8; 2] = [0x30, 0xbd]; // pop {r4, r5, pc}
    const MOVS: [u8; 2] = [0x05, 0x1c]; // adds r5, r0, #0
    const CMP: [u8; 2] = [0x07, 0x28]; // cmp r0, #7

    /// A `bl` pair at `site` reaching `target`.
    fn bl(site: u32, target: u32) -> [u8; 4] {
        let value = target.wrapping_sub(site + 4) as i32 >> 1;
        let high = 0xf000 | ((value >> 11) & 0x7ff) as u16;
        let low = 0xf800 | (value & 0x7ff) as u16;
        let [a, b] = high.to_le_bytes();
        let [c, d] = low.to_le_bytes();
        [a, b, c, d]
    }
    /// A function at `base`: prologue, a leaving call, a literal load, a
    /// return and its word-aligned pool holding `literal`.
    fn function(base: u32, callee: u32, literal: u32) -> Vec<u8> {
        let mut bytes = PUSH.to_vec();
        bytes.extend(bl(base + 2, callee));
        bytes.extend([0x02, 0x48]); // ldr r0, [pc, #8]
        bytes.extend(MOVS);
        bytes.extend(CMP);
        bytes.extend(POP);
        bytes.extend([0xc0, 0x46]); // nop to the pool
        bytes.extend(literal.to_le_bytes());
        bytes
    }
    fn image(bytes: &[u8], base: u32) -> Image<'_> {
        Image {
            bytes,
            base,
            beneath: None,
        }
    }

    #[test]
    fn twins_mask_external_bl_offsets_and_literal_words_but_not_instructions() {
        let one = function(0x0800_0000, 0x0800_4000, 0x0300_1000);
        let two = function(0x0900_0000, 0x0900_8000, 0x0300_2000);
        let images = [image(&one, 0x0800_0000), image(&two, 0x0900_0000)];
        let left = Body::read(&images, 0, 0x0800_0000, one.len()).unwrap();
        let right = Body::read(&images, 1, 0x0900_0000, two.len()).unwrap();
        assert_eq!(left.signature, right.signature);
        assert_eq!(
            references(&images, (0, 0x0800_0000), left.len()),
            [
                Reference::Call(None, 0x0800_4000),
                Reference::Literal(0x0300_1000)
            ]
        );
        // One changed instruction, or a load of another pool word, is not a twin.
        let mut changed = two.clone();
        changed[8] = 0x08;
        let images = [image(&one, 0x0800_0000), image(&changed, 0x0900_0000)];
        let changed = Body::read(&images, 1, 0x0900_0000, one.len()).unwrap();
        assert_ne!(left.signature, changed.signature);
        assert_eq!(left.signature.differing_units(&changed.signature), Some(1));
        // A call's opcode bits are compared: a pair that is not a call differs.
        let mut unpaired = one.clone();
        unpaired[5] = 0x46;
        let images = [image(&one, 0x0800_0000), image(&unpaired, 0x0900_0000)];
        let unpaired = Body::read(&images, 1, 0x0900_0000, one.len()).unwrap();
        assert_ne!(left.signature, unpaired.signature);
    }

    #[test]
    fn in_body_bl_and_jump_tables_compare_exactly() {
        // push; cmp; mov pc, r0; alignment; two-word table into the body;
        // a bl to the body's own start; pop.
        let body = |base: u32, first: u32| {
            let mut bytes = PUSH.to_vec();
            bytes.extend(CMP);
            bytes.extend([0x87, 0x46, 0x00, 0x00]);
            bytes.extend((base + first).to_le_bytes());
            bytes.extend((base + 0x14).to_le_bytes());
            bytes.extend(bl(base + 0x10, base));
            bytes.extend(POP);
            bytes.extend([0xc0, 0x46]);
            bytes
        };
        let one = body(0x0200_8000, 0x10);
        let two = body(0x0200_9000, 0x10);
        let other = body(0x0200_a000, 0x14);
        let images = [
            image(&one, 0x0200_8000),
            image(&two, 0x0200_9000),
            image(&other, 0x0200_a000),
        ];
        let read = |index: usize, base| Body::read(&images, index, base, one.len()).unwrap();
        assert_eq!(
            read(0, 0x0200_8000).signature,
            read(1, 0x0200_9000).signature
        );
        assert!(references(&images, (0, 0x0200_8000), one.len()).is_empty());
        assert_ne!(
            read(0, 0x0200_8000).signature,
            read(2, 0x0200_a000).signature
        );
        // The same bl bytes at another length leave the body and are masked.
        let mut far = one.clone();
        far[16..20].copy_from_slice(&bl(0x0200_9010, 0x0200_7000));
        let images = [image(&one, 0x0200_8000), image(&far, 0x0200_9000)];
        let far = Body::read(&images, 1, 0x0200_9000, one.len()).unwrap();
        assert_ne!(read(0, 0x0200_8000).signature, far.signature);
    }

    #[test]
    fn trailing_alignment_halfword_is_ignored() {
        let mut padded = function(0x0800_0000, 0x0800_4000, 0x1234_0000);
        padded.truncate(12);
        padded.extend([0, 0, 0, 0]);
        let mut plain = padded.clone();
        plain.truncate(14);
        let images = [image(&padded, 0x0800_0000), image(&plain, 0x0900_0000)];
        let long = Body::read(&images, 0, 0x0800_0000, 16).unwrap();
        assert_eq!(long.len(), 14);
        // A body ending before a word boundary keeps its last halfword.
        assert_eq!(Body::read(&images, 1, 0x0900_0000, 14).unwrap().len(), 14);
        assert_eq!(Body::at(&images, (0, 0x0800_0000), 16).unwrap().len(), 16);
        // A zero halfword that ends a literal word is data, not alignment.
        let pooled = function(0x0800_0000, 0x0800_4000, 0x0000_1234);
        let images = [image(&pooled, 0x0800_0000)];
        assert_eq!(
            Body::read(&images, 0, 0x0800_0000, pooled.len())
                .unwrap()
                .len(),
            20
        );
    }

    #[test]
    fn near_twins_found_by_disjoint_anchors() {
        let base = 0x0800_0000;
        let mut original = Vec::new();
        for index in 0..32u16 {
            original.extend((0x2000 | index).to_le_bytes()); // movs r0, #index
        }
        original.extend([0x70, 0x47, 0xc0, 0x46]); // bx lr; nop
        let mut image_bytes = original.clone();
        let mut copy = original.clone();
        for at in [0, 10, 20, 30, 40, 50, 60, 62] {
            copy[at] ^= 0x80;
        }
        image_bytes.extend(&copy);
        let mut distant = original.clone();
        for at in (0..17).map(|index| index * 4) {
            distant[at] ^= 0x80;
        }
        image_bytes.extend(&distant);
        let images = [image(&image_bytes, base)];
        let index = Index::new(&images);
        let body = Body::read(&images, 0, base, original.len()).unwrap();
        assert_eq!(index.sites(&images, &body), [(0, base)]);
        assert_eq!(find(&images, &body), [(0, base)]);
        let changed = Body::read(&images, 0, base + 68, 68).unwrap();
        assert_eq!(index.sites(&images, &changed), find(&images, &changed));
        let (near, complete) = index.near(&images, &body);
        assert!(complete);
        assert_eq!(near, [((0, base + original.len() as u32), 8)]);
        // Too short for the near tier.
        let short = Body::read(&images, 0, base, 16).unwrap();
        assert_eq!(index.near(&images, &short), (Vec::new(), false));
    }

    #[test]
    fn veneer_chain_resolves_overlay_import_and_main_far_call_veneer() {
        const VENEER: [u8; 4] = [0x00, 0x4c, 0x20, 0x47];
        let mut main = vec![0u8; 0x100];
        main[0x40..0x44].copy_from_slice(&VENEER);
        main[0x44..0x48].copy_from_slice(&0x0300_0081u32.to_le_bytes());
        main[0x80..0x84].copy_from_slice(&[0x00, 0x4b, 0x18, 0x47]); // through r3
        main[0x84..0x88].copy_from_slice(&0x0300_0080u32.to_le_bytes());
        let mut overlay = function(0x0200_8000, 0x0200_8020, 0x0200_8000);
        overlay.resize(0x20, 0);
        overlay.extend(VENEER);
        overlay.extend(0x0800_0041u32.to_le_bytes());
        let images = [
            Image {
                bytes: &main,
                base: 0x0800_0000,
                beneath: None,
            },
            Image {
                bytes: &overlay,
                base: 0x0200_8000,
                beneath: Some(0),
            },
        ];
        assert_eq!(
            call_target(&images, 1, 0x0200_8020),
            Reference::Call(None, 0x0300_0080)
        );
        // A Thumb veneer target is followed; an ARM one ends the chain.
        assert_eq!(
            call_target(&images, 0, 0x0800_0080),
            Reference::Call(None, 0x0300_0080)
        );
        assert_eq!(
            call_target(&images, 1, 0x0800_0010),
            Reference::Call(Some(0), 0x0800_0010)
        );
        assert_eq!(
            call_target(&images, 1, 0x0200_8002),
            Reference::Call(Some(1), 0x0200_8002)
        );
        let found = references(&images, (1, 0x0200_8000), 20);
        assert_eq!(
            found,
            [
                Reference::Call(None, 0x0300_0080),
                Reference::Literal(0x0200_8000)
            ]
        );
        // A literal into its own image and a call reaching twin functions bind alike.
        let other = [
            Reference::Call(Some(1), 0x0200_8010),
            Reference::Literal(0x0200_8004),
        ];
        let own = [
            Reference::Call(Some(1), 0x0200_8014),
            Reference::Literal(0x0200_8008),
        ];
        assert!(equivalent(&images, (1, &own), (1, &other), |_, _| true));
        assert!(!equivalent(&images, (1, &own), (1, &other), |_, _| false));
        let global = [
            Reference::Call(Some(1), 0x0200_8014),
            Reference::Literal(0x0300_0000),
        ];
        assert!(!equivalent(&images, (1, &own), (1, &global), |_, _| true));
    }

    #[test]
    fn bodies_below_minimum_are_ignored() {
        let bytes = function(0x0800_0000, 0x0800_4000, 7);
        let images = [image(&bytes, 0x0800_0000)];
        assert!(Body::read(&images, 0, 0x0800_0000, MINIMUM_BODY - 2).is_none());
        assert!(Body::read(&images, 0, 0x0800_0000, MINIMUM_BODY).is_some());
        // Twelve bytes padded to a word boundary are ten bytes of body.
        let mut padded = bytes[..10].to_vec();
        padded.extend([0, 0]);
        let images = [image(&padded, 0x0800_0000)];
        assert!(Body::read(&images, 0, 0x0800_0000, 12).is_none());
        assert!(Body::read(&images, 0, 0x0800_0000, 64).is_none());
    }
}
