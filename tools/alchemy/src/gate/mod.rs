//! Gates `build rom` runs on what it links, as Pascal set them on 2026-09-29.
//! Each reads the build's own objects, linked images and maps, and fails the
//! build with a message naming the rule and where it broke. They write
//! nothing the build caches.
pub(crate) mod ids;
pub(crate) mod overlay;

/// A minimal little-endian ARM ELF32 for the gates' tests: one allocated
/// section, its REL relocations and a symbol table.
#[cfg(test)]
pub(crate) mod elf {
    /// Where a test symbol is defined.
    pub(crate) const UNDEFINED: u16 = 0;
    pub(crate) const SECTION: u16 = 1;
    pub(crate) const ABSOLUTE: u16 = 0xfff1;

    /// A symbol: its name, value, section and whether it is global. Local
    /// symbols come first, as ELF requires.
    pub(crate) struct Symbol<'a>(pub &'a str, pub u32, pub u16, pub bool);

    fn string(table: &mut Vec<u8>, name: &str) -> u32 {
        let offset = table.len() as u32;
        table.extend_from_slice(name.as_bytes());
        table.push(0);
        offset
    }

    fn align(file: &mut Vec<u8>) -> u32 {
        while file.len() % 4 != 0 {
            file.push(0);
        }
        file.len() as u32
    }

    /// `section` at `address` holding `bytes`, with `relocations` as
    /// `(offset, symbol number from 1, type)` against `symbols`.
    pub(crate) fn build(
        section: &str,
        address: u32,
        bytes: &[u8],
        symbols: &[Symbol],
        relocations: &[(u32, u32, u32)],
    ) -> Vec<u8> {
        let mut file = vec![0u8; 52];
        let data = align(&mut file);
        file.extend_from_slice(bytes);
        let rel = align(&mut file);
        for (offset, symbol, kind) in relocations {
            file.extend_from_slice(&offset.to_le_bytes());
            file.extend_from_slice(&(symbol << 8 | kind).to_le_bytes());
        }
        let mut strings = vec![0u8];
        let symtab = align(&mut file);
        file.extend_from_slice(&[0; 16]);
        for Symbol(name, value, index, global) in symbols {
            let name = string(&mut strings, name);
            file.extend_from_slice(&name.to_le_bytes());
            file.extend_from_slice(&value.to_le_bytes());
            file.extend_from_slice(&0u32.to_le_bytes());
            file.push(if *global { 0x10 } else { 0 });
            file.push(0);
            file.extend_from_slice(&index.to_le_bytes());
        }
        let strtab = file.len() as u32;
        file.extend_from_slice(&strings);
        let mut names = vec![0u8];
        let headers = [
            (
                section.to_owned(),
                1u32,
                6u32,
                address,
                data,
                bytes.len() as u32,
                0,
                0,
                0,
            ),
            (
                format!(".rel{section}"),
                9,
                0x40,
                0,
                rel,
                relocations.len() as u32 * 8,
                3,
                1,
                8,
            ),
            (
                ".symtab".to_owned(),
                2,
                0,
                0,
                symtab,
                (symbols.len() as u32 + 1) * 16,
                4,
                1 + symbols.iter().filter(|symbol| !symbol.3).count() as u32,
                16,
            ),
            (
                ".strtab".to_owned(),
                3,
                0,
                0,
                strtab,
                strings.len() as u32,
                0,
                0,
                0,
            ),
        ];
        let named: Vec<u32> = headers
            .iter()
            .map(|header| string(&mut names, &header.0))
            .collect();
        let shstrtab_name = string(&mut names, ".shstrtab");
        let shstrtab = file.len() as u32;
        file.extend_from_slice(&names);
        let shoff = align(&mut file);
        file.extend_from_slice(&[0; 40]);
        let mut header = |values: [u32; 10]| {
            for value in values {
                file.extend_from_slice(&value.to_le_bytes());
            }
        };
        for (index, (_, kind, flags, address, offset, size, link, info, entsize)) in
            headers.iter().enumerate()
        {
            header([
                named[index],
                *kind,
                *flags,
                *address,
                *offset,
                *size,
                *link,
                *info,
                4,
                *entsize,
            ]);
        }
        header([
            shstrtab_name,
            3,
            0,
            0,
            shstrtab,
            names.len() as u32,
            0,
            0,
            1,
            0,
        ]);
        file[..16].copy_from_slice(b"\x7fELF\x01\x01\x01\0\0\0\0\0\0\0\0\0");
        let mut at = 16;
        let mut put = |value: u32, width: usize| {
            file[at..at + width].copy_from_slice(&value.to_le_bytes()[..width]);
            at += width;
        };
        put(1, 2); // ET_REL
        put(40, 2); // EM_ARM
        put(1, 4);
        put(0, 4);
        put(0, 4);
        put(shoff, 4);
        put(0x0500_0000, 4);
        put(52, 2);
        put(0, 2);
        put(0, 2);
        put(40, 2);
        put(6, 2);
        put(5, 2);
        file
    }
}
