#![allow(clippy::tabs_in_doc_comments)] // the example is a TSV file, tabs and all
//! Edits Camelot made to a stream after packing it (W3).
//!
//! A patch file sits beside its asset as `<STEM>_patch.tsv`:
//!
//! ```text
//! position	pack	ship	proof
//! 65472	0x02	0x01	literal: 254 of 255 stand-ins reproduce the stream
//! 65474	0x02	0x01	copy of 65472
//! ```
//!
//! Each row is one decoded byte whose value at pack time differed from the
//! asset: `pack` is the neutral stand-in the packer sees, `ship` the byte the
//! asset holds. The build packs the stand-ins, then writes the shipped bytes
//! into the stream's literals. Rows marked `copy of` are bytes the packer's own
//! copies carried from an edited literal; they need no edit of their own.

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Edit {
    pub position: usize,
    pub pack: u8,
    pub ship: u8,
    pub proof: String,
}

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct Patch {
    pub edits: Vec<Edit>,
}

/// The patch file's name beside the asset an input `name` builds.
pub fn patch_name(input: &str) -> String {
    let file = input.rsplit('/').next().unwrap_or(input);
    let stem = file.split('.').next().unwrap_or(file);
    format!("{stem}_patch.tsv")
}

fn byte(field: &str) -> Option<u8> {
    match field.strip_prefix("0x") {
        Some(hex) if hex.len() == 2 => u8::from_str_radix(hex, 16).ok(),
        _ => None,
    }
}

impl Patch {
    pub fn parse(name: &str, text: &[u8]) -> Result<Patch, String> {
        let text = std::str::from_utf8(text).map_err(|_| format!("{name}: patch is not text"))?;
        let mut lines = text.lines().filter(|line| !line.trim().is_empty());
        if lines.next().map(str::trim) != Some("position\tpack\tship\tproof") {
            return Err(format!(
                "{name}: header must be position, pack, ship, proof"
            ));
        }
        let mut edits: Vec<Edit> = Vec::new();
        for line in lines {
            let fields: Vec<&str> = line.splitn(4, '\t').collect();
            let [position, pack, ship, proof] = fields.as_slice() else {
                return Err(format!("{name}: {line:?} needs four fields"));
            };
            let position = position
                .trim()
                .parse::<usize>()
                .map_err(|_| format!("{name}: {position:?} is not a position"))?;
            let (Some(pack), Some(ship)) = (byte(pack.trim()), byte(ship.trim())) else {
                return Err(format!("{name}: {line:?} needs two 0xNN bytes"));
            };
            let proof = proof.trim().to_owned();
            if proof.is_empty() {
                return Err(format!("{name}: {position} carries no proof"));
            }
            if pack == ship {
                return Err(format!("{name}: {position} changes nothing"));
            }
            if edits.last().is_some_and(|last| last.position >= position) {
                return Err(format!("{name}: positions must rise"));
            }
            edits.push(Edit {
                position,
                pack,
                ship,
                proof,
            });
        }
        if edits.is_empty() {
            return Err(format!("{name}: patch lists no edit"));
        }
        Ok(Patch { edits })
    }

    /// The bytes the packer saw, from the shipped asset bytes.
    pub fn packed(&self, name: &str, shipped: &[u8]) -> Result<Vec<u8>, String> {
        let mut packed = shipped.to_vec();
        for edit in &self.edits {
            match packed.get_mut(edit.position) {
                Some(byte) if *byte == edit.ship => *byte = edit.pack,
                Some(byte) => {
                    return Err(format!(
                        "{name}: byte {} is 0x{byte:02x}, the patch ships 0x{:02x}",
                        edit.position, edit.ship
                    ))
                }
                None => return Err(format!("{name}: {} lies past the asset", edit.position)),
            }
        }
        Ok(packed)
    }

    /// Edited literals, not counting bytes the packer's copies carried.
    pub fn literals(&self) -> usize {
        self.edits
            .iter()
            .filter(|edit| !edit.proof.starts_with("copy of"))
            .count()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    const TEXT: &str = "position\tpack\tship\tproof\n\
        4\t0x02\t0x01\tliteral: 254 of 255 stand-ins reproduce the stream\n\
        6\t0x02\t0x01\tcopy of 4\n";

    #[test]
    fn a_patch_restores_the_bytes_the_packer_saw() {
        let patch = Patch::parse("grid_patch.tsv", TEXT.as_bytes()).unwrap();
        assert_eq!(patch.literals(), 1);
        let shipped = [0, 0, 0, 0, 1, 0, 1, 0];
        assert_eq!(
            patch.packed("GRID", &shipped).unwrap(),
            vec![0, 0, 0, 0, 2, 0, 2, 0]
        );
        assert!(patch.packed("GRID", &[0; 8]).is_err());
    }

    #[test]
    fn a_patch_needs_its_proof_and_rising_positions() {
        let unproven = "position\tpack\tship\tproof\n4\t0x02\t0x01\t\n";
        assert!(Patch::parse("X", unproven.as_bytes()).is_err());
        let falling = "position\tpack\tship\tproof\n6\t0x02\t0x01\ta\n4\t0x02\t0x01\tb\n";
        assert!(Patch::parse("X", falling.as_bytes()).is_err());
        assert_eq!(patch_name("map/map_181/grid.tsv"), "grid_patch.tsv");
    }
}
