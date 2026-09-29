//! A minimal little-endian ELF32 reader: loaded section bytes by address and
//! the symbol table, which is all a linked image needs to be disassembled.

pub struct Section {
    pub name: String,
    pub address: u32,
    pub bytes: Vec<u8>,
}

pub struct Symbol {
    pub name: String,
    pub value: u32,
    pub size: u32,
    /// STT_* in the low nibble of st_info.
    pub kind: u8,
    pub section: u16,
}

pub struct Elf {
    pub sections: Vec<Section>,
    pub symbols: Vec<Symbol>,
}

fn half(data: &[u8], at: usize) -> Result<u16, String> {
    data.get(at..at + 2)
        .map(|b| u16::from_le_bytes([b[0], b[1]]))
        .ok_or_else(|| format!("ELF truncated at {at:#x}"))
}

fn word(data: &[u8], at: usize) -> Result<u32, String> {
    data.get(at..at + 4)
        .map(|b| u32::from_le_bytes([b[0], b[1], b[2], b[3]]))
        .ok_or_else(|| format!("ELF truncated at {at:#x}"))
}

fn string(table: &[u8], at: usize) -> String {
    let tail = table.get(at..).unwrap_or(&[]);
    let end = tail.iter().position(|&b| b == 0).unwrap_or(tail.len());
    String::from_utf8_lossy(&tail[..end]).into_owned()
}

const SHT_PROGBITS: u32 = 1;
const SHT_SYMTAB: u32 = 2;
const SHF_ALLOC: u32 = 2;

impl Elf {
    pub fn parse(data: &[u8]) -> Result<Elf, String> {
        if data.get(..4) != Some(b"\x7fELF") || data.get(4) != Some(&1) || data.get(5) != Some(&1) {
            return Err("not a little-endian ELF32 file".into());
        }
        let shoff = word(data, 0x20)? as usize;
        let shentsize = half(data, 0x2e)? as usize;
        let shnum = half(data, 0x30)? as usize;
        let shstrndx = half(data, 0x32)? as usize;
        struct Header {
            name: u32,
            kind: u32,
            flags: u32,
            address: u32,
            offset: usize,
            size: usize,
            link: u32,
        }
        let mut headers = Vec::with_capacity(shnum);
        for i in 0..shnum {
            let at = shoff + i * shentsize;
            headers.push(Header {
                name: word(data, at)?,
                kind: word(data, at + 4)?,
                flags: word(data, at + 8)?,
                address: word(data, at + 12)?,
                offset: word(data, at + 16)? as usize,
                size: word(data, at + 20)? as usize,
                link: word(data, at + 24)?,
            });
        }
        let bytes_of = |h: &Header| -> Result<&[u8], String> {
            data.get(h.offset..h.offset + h.size)
                .ok_or_else(|| "ELF section outside the file".to_string())
        };
        let names = match headers.get(shstrndx) {
            Some(h) => bytes_of(h)?,
            None => &[],
        };
        let mut sections = Vec::new();
        let mut symbols = Vec::new();
        for h in &headers {
            if h.kind == SHT_PROGBITS && h.flags & SHF_ALLOC != 0 && h.size > 0 {
                sections.push(Section {
                    name: string(names, h.name as usize),
                    address: h.address,
                    bytes: bytes_of(h)?.to_vec(),
                });
            }
            if h.kind == SHT_SYMTAB {
                let table = bytes_of(h)?;
                let strings = match headers.get(h.link as usize) {
                    Some(s) => bytes_of(s)?,
                    None => &[],
                };
                for entry in table.chunks_exact(16) {
                    let info = entry[12];
                    symbols.push(Symbol {
                        name: string(strings, word(entry, 0)? as usize),
                        value: word(entry, 4)?,
                        size: word(entry, 8)?,
                        kind: info & 0xf,
                        section: half(entry, 14)?,
                    });
                }
            }
        }
        Ok(Elf { sections, symbols })
    }

    /// The loaded section holding `address`, and the offset into it.
    pub fn section_at(&self, address: u32) -> Option<(&Section, usize)> {
        self.sections.iter().find_map(|s| {
            (address >= s.address && address - s.address < s.bytes.len() as u32)
                .then(|| (s, (address - s.address) as usize))
        })
    }
}
