//! GNU ld map files as the build writes them: each input section the linker
//! placed (object path, run-time address, size and where its bytes sit in
//! the image), the global symbols inside it and the alignment fill between
//! them. Maps print 64-bit addresses (0x0000000008xxxxxx); the Game Boy
//! Advance's fit in 32 bits. Code that runs from IWRAM is placed at its
//! run-time address and loaded from the ROM, which the output section's
//! `load address` gives; `load` is where its bytes are in the ROM. RAM the
//! game only reserves gets a load address too, but the next ROM section
//! starts there: it has no bytes in the image.

/// Where the cartridge ROM is mapped.
pub const ROM: u32 = 0x0800_0000;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Placed {
    /// The input section, `.text` or `.rom.00002e00`, or `*fill*`.
    pub name: String,
    /// The object as the map prints it; empty for fill.
    pub path: String,
    pub address: u32,
    pub size: u32,
    /// The address of its bytes in the image.
    pub load: u32,
    /// Whether the image holds its bytes; reserved RAM does not.
    pub loaded: bool,
    /// The symbols the map lists inside it, in map order.
    pub symbols: Vec<(u32, String)>,
}

impl Placed {
    pub fn is_fill(&self) -> bool {
        self.name == "*fill*"
    }

    pub fn end(&self) -> u32 {
        self.address + self.size
    }

    /// The object path from the build's `obj/` on, as people name it.
    pub fn object(&self) -> &str {
        self.path.rsplit_once("/obj/").map_or(&self.path, |(_, tail)| tail)
    }

    /// The object, with the section when it is not one of the object's
    /// ordinary sections: `recon/tbs/de/rom.o(.rom.00002e00)`.
    pub fn label(&self) -> String {
        if self.is_fill() {
            return format!("*fill* at {:#010x}", self.load);
        }
        match self.name.as_str() {
            ".text" | ".rodata" | ".data" | ".padding" => self.object().to_string(),
            name => format!("{}({name})", self.object()),
        }
    }

    /// The symbol whose range holds `address`, preferring a real name to a
    /// placeholder that spells an address, and the offset into it.
    pub fn symbol_at(&self, address: u32) -> Option<(&str, u32)> {
        let start = self.symbols.iter().map(|(a, _)| *a).filter(|&a| a <= address).max()?;
        Some((preferred(&self.symbols, start)?, address - start))
    }
}

/// Among the names at `address`, the first that is not a placeholder.
pub fn preferred(symbols: &[(u32, String)], address: u32) -> Option<&str> {
    let mut names = symbols.iter().filter(|(a, _)| *a == address).map(|(_, n)| n.as_str());
    let first = names.next()?;
    Some(std::iter::once(first).chain(names).find(|n| !is_placeholder(n)).unwrap_or(first))
}

/// `Func_080abcde`, `Data_03001ec8`: a name that only spells its address.
pub fn is_placeholder(name: &str) -> bool {
    name.rsplit_once('_')
        .is_some_and(|(_, tail)| tail.len() == 8 && tail.chars().all(|c| c.is_ascii_hexdigit()))
}

pub struct Map {
    /// In map order, which is image order within each output section.
    pub placed: Vec<Placed>,
}

/// An output section: run-time address, size and load address.
struct Output {
    address: u32,
    size: u32,
    load: u32,
}

fn hex(word: &str) -> Option<u32> {
    u64::from_str_radix(word.strip_prefix("0x")?, 16).ok().and_then(|v| u32::try_from(v).ok())
}

/// The first whitespace-delimited word of `text` and what follows it.
fn word(text: &str) -> (&str, &str) {
    let text = text.trim_start();
    let end = text.find(char::is_whitespace).unwrap_or(text.len());
    (&text[..end], &text[end..])
}

/// `ADDRESS SIZE [REST]` after a section name; REST may hold spaces.
fn placement(text: &str) -> Option<(u32, u32, &str)> {
    let (address, rest) = word(text);
    let (size, rest) = word(rest);
    Some((hex(address)?, hex(size)?, rest.trim()))
}

impl Map {
    pub fn parse(text: &str) -> Map {
        let mut placed: Vec<Placed> = Vec::new();
        let mut outputs: Vec<Output> = Vec::new();
        // The output section each placed section belongs to.
        let mut owner: Vec<Option<usize>> = Vec::new();
        let mut started = false;
        // A section name too long for its line: (is output section, name).
        let mut pending: Option<(bool, String)> = None;
        for line in text.lines() {
            if !started {
                started = line.starts_with("Linker script and memory map");
                continue;
            }
            if line.trim().is_empty() {
                pending = None;
                continue;
            }
            let indented = line.starts_with(' ');
            let column_one = indented && !line[1..].starts_with(' ');
            if let Some((output, name)) = pending.take() {
                if let Some((address, size, rest)) = placement(line).filter(|_| indented && !column_one) {
                    if output {
                        outputs.push(output_section(address, size, rest));
                    } else {
                        push(&mut placed, name, address, size, rest, outputs.last());
                        owner.push(outputs.len().checked_sub(1));
                    }
                    continue;
                }
            }
            if !indented {
                // An output section header, or LOAD and OUTPUT lines.
                let (name, rest) = word(line);
                if rest.trim().is_empty() {
                    pending = Some((true, name.to_string()));
                } else if let Some((address, size, rest)) = placement(rest) {
                    outputs.push(output_section(address, size, rest));
                }
                continue;
            }
            if column_one {
                let (name, rest) = word(line);
                // Input specifications (`*/games/...(.text)`) say what was
                // asked for; the placed sections follow them.
                if name.starts_with('*') && name != "*fill*" {
                    continue;
                }
                if rest.trim().is_empty() {
                    pending = Some((false, name.to_string()));
                } else if let Some((address, size, rest)) = placement(rest) {
                    push(&mut placed, name.to_string(), address, size, rest, outputs.last());
                    owner.push(outputs.len().checked_sub(1));
                }
                continue;
            }
            // A symbol: `ADDRESS NAME`; assignments carry `= expression`.
            let (address, rest) = word(line);
            let (name, tail) = word(rest);
            if let (Some(address), true, true) = (hex(address), tail.trim().is_empty(), is_identifier(name)) {
                if let Some(last) = placed.last_mut().filter(|p| !p.is_fill()) {
                    if address >= last.address && address <= last.end() {
                        last.symbols.push((address, name.to_string()));
                    }
                }
            }
        }
        // RAM that is only reserved: loaded from where a section that
        // takes ROM space begins.
        let reserved: Vec<bool> = outputs
            .iter()
            .map(|o| {
                o.load != o.address
                    && outputs.iter().any(|q| q.size > 0 && q.load >= o.load && q.load - o.load < o.size && !std::ptr::eq(q, o))
            })
            .collect();
        for (p, owner) in placed.iter_mut().zip(owner) {
            p.loaded = !is_unloaded(&p.name) && !owner.is_some_and(|o| reserved[o]);
        }
        Map { placed }
    }

    /// Sections whose bytes are in an image of `length` bytes, with any.
    pub fn in_image(&self, length: usize) -> impl Iterator<Item = &Placed> {
        self.placed.iter().filter(move |p| {
            p.size > 0 && p.loaded && p.load >= ROM && (p.load - ROM) as usize + p.size as usize <= length
        })
    }

    /// The section whose bytes hold image address `load`.
    pub fn at_load(&self, load: u32, length: usize) -> Option<&Placed> {
        self.in_image(length).find(|p| load >= p.load && load - p.load < p.size)
    }

    /// The section that lists `name`, and the symbol's address.
    pub fn symbol(&self, name: &str) -> Option<(&Placed, u32)> {
        self.placed.iter().find_map(|p| {
            p.symbols.iter().find(|(_, n)| n == name).map(|(a, _)| (p, *a))
        })
    }

    /// The section holding run-time address `address`, fill aside.
    pub fn at_address(&self, address: u32) -> Option<&Placed> {
        self.placed
            .iter()
            .find(|p| !p.is_fill() && p.size > 0 && address >= p.address && address < p.end())
    }
}

fn output_section(address: u32, size: u32, rest: &str) -> Output {
    let load = rest.split_once("load address").and_then(|(_, load)| hex(load.trim()));
    Output { address, size, load: load.unwrap_or(address) }
}

fn push(placed: &mut Vec<Placed>, name: String, address: u32, size: u32, path: &str, output: Option<&Output>) {
    let relocation = output.map_or(0, |o| i64::from(o.load) - i64::from(o.address));
    placed.push(Placed {
        name,
        path: path.to_string(),
        address,
        size,
        load: (i64::from(address) + relocation) as u32,
        loaded: true,
        symbols: Vec::new(),
    });
}

/// Sections that take room at run time but have no bytes in the image.
fn is_unloaded(name: &str) -> bool {
    name.starts_with(".bss") || name.starts_with(".sbss") || name == "COMMON"
}

fn is_identifier(name: &str) -> bool {
    let mut chars = name.chars();
    chars.next().is_some_and(|c| c.is_ascii_alphabetic() || c == '_' || c == '.')
        && chars.all(|c| c.is_ascii_alphanumeric() || c == '_' || c == '.')
}

#[cfg(test)]
pub(crate) mod tests {
    use super::*;

    /// A map in the build's own shape: an output section loaded from the
    /// ROM, reserved RAM, a name too long for its line, fill, assignments
    /// and an object path with spaces.
    pub(crate) const MAP: &str = "\
Discarded input sections

 .ARM.attributes
                0x0000000000000000       0x20 /x/out/tbs-de/obj/recon/tbs/unidentified.o

Linker script and memory map

                0x00000000000004ec                Resource_Decode2CodeSize = (Resource_Decode2End - Resource_Decode2)

.rom_start      0x0000000008000000       0x30
 */recon/tbs/de/rom.o(.rom.00000000)
 .rom.00000000  0x0000000008000000       0x10 /x/out/tbs-de/obj/recon/tbs/de/rom.o
                0x0000000008000000                Rom_Start
 */games/THE BROKEN SEAL/SRC/SYSTEM/IO.o(.text .rodata .data .padding)
 .text          0x0000000008000010        0xe /x/out/tbs-de/obj/games/THE BROKEN SEAL/SRC/SYSTEM/IO.o
                0x0000000008000010                Func_08000010
                0x0000000008000010                Input_Handle
                0x0000000008000018                Input_Poll
 */recon/tbs/de/rom.o(.rom.00000020)
 *fill*         0x000000000800001e        0x2
 .rom.00000020  0x0000000008000020       0x10 /x/out/tbs-de/obj/recon/tbs/de/rom.o
                0x0000000008000020                Func_08000020
                0x0000000008000030                IwramCodeEnd = .

.iwram_runtime  0x0000000003000000        0x8 load address 0x0000000008000030
 */games/THE BROKEN SEAL/SRC/SYSTEM/RUNTIME.o(.text)
 .text          0x0000000003000000        0x8 /x/out/tbs-de/obj/games/THE BROKEN SEAL/SRC/SYSTEM/RUNTIME.o
                0x0000000003000000                IwramIrqMain
 .bss           0x0000000003000008        0x4 /x/out/tbs-de/obj/games/THE BROKEN SEAL/SRC/SYSTEM/RUNTIME.o

iwram           0x0000000003000100       0x40 load address 0x0000000008000038
 */recon/tbs/sym_iwram.o(.sym_iwram)
 .sym_iwram     0x0000000003000100       0x40 /x/out/tbs-de/obj/recon/tbs/sym_iwram.o
                0x0000000003000100                gFrameTick

.rom_main       0x0000000008000038       0x20
 */recon/tbs/unidentified.o(.unidentified.08000030)
 .unidentified.08000030
                0x0000000008000038       0x10 /x/out/tbs-de/obj/recon/tbs/unidentified.o
                0x0000000008000038                Math_Table
 */recon/tbs/raw/08000048.o(.text)
 .text          0x0000000008000048       0x10 /x/out/tbs-de/obj/recon/tbs/raw/08000048.o
                0x0000000008000048                Func_08000048
LOAD /x/out/tbs-de/obj/recon/tbs/de/rom.o
";

    #[test]
    fn reads_placements_loads_and_symbols() {
        let map = Map::parse(MAP);
        let names: Vec<&str> = map.placed.iter().map(|p| p.name.as_str()).collect();
        assert_eq!(names, [".rom.00000000", ".text", "*fill*", ".rom.00000020", ".text", ".bss", ".sym_iwram", ".unidentified.08000030", ".text"]);
        let io = &map.placed[1];
        assert_eq!(io.object(), "games/THE BROKEN SEAL/SRC/SYSTEM/IO.o");
        assert_eq!((io.address, io.size, io.load), (0x0800_0010, 0xe, 0x0800_0010));
        assert_eq!(io.symbol_at(0x0800_0012), Some(("Input_Handle", 2)));
        assert_eq!(io.symbol_at(0x0800_001a), Some(("Input_Poll", 2)));
        // Assignments are not symbols of the section before them.
        assert_eq!(map.placed[3].symbols, [(0x0800_0020, "Func_08000020".to_string())]);
        let runtime = &map.placed[4];
        assert_eq!((runtime.address, runtime.load), (0x0300_0000, 0x0800_0030));
        assert_eq!(map.placed[7].label(), "recon/tbs/unidentified.o(.unidentified.08000030)");
        assert_eq!(map.placed[7].symbols.len(), 1);
        assert_eq!(map.placed[2].label(), "*fill* at 0x0800001e");
        assert_eq!(map.symbol("IwramIrqMain").map(|(p, a)| (p.load, a)), Some((0x0800_0030, 0x0300_0000)));
        // Neither .bss nor reserved IWRAM has bytes in the image, though
        // the reserved IWRAM's symbols still name its addresses.
        let held: Vec<&str> = map.in_image(0x58).map(|p| p.name.as_str()).collect();
        assert_eq!(held, [".rom.00000000", ".text", "*fill*", ".rom.00000020", ".text", ".unidentified.08000030", ".text"]);
        assert_eq!(map.symbol("gFrameTick").map(|(_, a)| a), Some(0x0300_0100));
        assert_eq!(map.at_load(0x0800_0034, 0x58).map(Placed::label), Some("games/THE BROKEN SEAL/SRC/SYSTEM/RUNTIME.o".to_string()));
        assert_eq!(map.at_load(0x0800_0038, 0x58).map(Placed::label), Some("recon/tbs/unidentified.o(.unidentified.08000030)".to_string()));
        assert!(is_placeholder("Func_08000010") && !is_placeholder("Input_Handle"));
    }
}
