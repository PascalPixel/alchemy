//! Resource archive packing: the alignment bytes after each stream.
//!
//! The original packer ran on i386 GNU/Linux. For each resource file, in
//! archive order, it allocated a buffer the size of the file, opened and read
//! the file, wrote the buffer rounded up to four bytes, freed the buffer and
//! closed the file. The C library hands every buffer the same heap address, so
//! a stream's one to three alignment bytes are whatever that memory last held:
//! the tail of an earlier and longer file, the free-chunk links and footer of
//! the previous buffer, or the closed stdio `FILE` object that the previous
//! `fopen` placed directly after the previous buffer.
//!
//! Not every resource file is a bare stream. A map container is written by
//! its own tool: a header and several streams, each started on a four-byte
//! boundary with the gap zero-filled, the file itself ending aligned. The
//! packer adds nothing to such a file, and its streams' alignment is zero.
//! A file whose writer left no gap inside it, such as a palette followed by
//! its stream, ends where that writer stopped, and the packer pads it from
//! the heap like a bare stream.
//!
//! [`alignment`] replays the whole resource directory in physical order from
//! the built regions (never reference bytes). The allocator is GNU libc
//! `malloc` on i386 (4-byte size fields, 8-byte alignment, 16-byte minimum
//! chunk, requests of 128 KiB or more mapped outside the heap, a freed chunk
//! linked to its empty bin in `main_arena`). The closed `FILE` is the host C
//! library's `struct locked_FILE` ([`Libc`]) in a program without
//! libpthread, whose lock initialiser is therefore a no-op.
//!
//! The heap grows with `sbrk` in whole pages and shrinks again: when `fclose`
//! leaves a free top chunk of at least 128 KiB, `malloc` returns all but its
//! last partial page to the system (glibc 2.1 and 2.2's default trim
//! threshold and zero top pad). Memory the heap gets back later is fresh and
//! zero-filled. Where the break falls depends on the host's heap address:
//! the replay follows every address the recorded host facts allow and
//! derives a byte only where they all agree.
//!
//! The game's reference machine definition (`recon/<game>/machine.json`,
//! read as a [`Host`]) names each edition's libc. Some alignment bytes are
//! addresses on the machine that ran the packer, the file buffer's or
//! libc's: those follow from the definition's observed host facts, which hold
//! each address only to the bits observed. A byte those bits do not fix is
//! reported, never guessed, as is a byte of memory last written by a file
//! the build does not produce.
use serde_json::Value;
use std::collections::{BTreeMap, BTreeSet};

use super::{json_string, number};

const MMAP_THRESHOLD: usize = 128 * 1024;
/// glibc's `DEFAULT_TRIM_THRESHOLD`: a free top chunk this large is trimmed.
const TRIM_THRESHOLD: usize = 128 * 1024;
/// glibc's `DEFAULT_TOP_PAD`: `sbrk` asks for, and trimming keeps, no more.
const TOP_PAD: usize = 0;
/// `MINSIZE`: the smallest chunk, which a split must leave the top.
const MIN_CHUNK: usize = 16;
/// The i386 page, the unit in which the break moves.
const PAGE: usize = 4096;
/// `MALLOC_ALIGNMENT`: every chunk, and so the file buffer, is aligned so.
const CHUNK_ALIGNMENT: usize = 8;
/// glibc `malloc`'s bins below this chunk size hold one size each.
const MAX_SMALLBIN_SIZE: usize = 512;
/// The alignment of files in the archive and of streams in a container.
const ALIGNMENT: usize = 4;

/// An address on the packing machine that its heap came to hold.
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub(super) enum Base {
    /// The file buffer, as `malloc` returned it.
    HeapBuffer,
    /// The stream heading libc's list of open streams at each `fopen`.
    Chain,
    /// libc's `main_arena`, whose bins a freed chunk links to.
    MainArena,
    /// libc's `_IO_file_jumps`.
    FileJumps,
    /// libc's `_IO_wfile_jumps`.
    WideFileJumps,
}
impl Base {
    const ALL: [(Base, &'static str); 5] = [
        (Base::HeapBuffer, "heap_buffer"),
        (Base::Chain, "chain"),
        (Base::MainArena, "main_arena"),
        (Base::FileJumps, "file_jumps"),
        (Base::WideFileJumps, "wide_file_jumps"),
    ];
}

/// One byte of the replayed heap.
#[derive(Clone, Copy, Debug, PartialEq)]
enum Cell {
    Byte(u8),
    /// Byte `byte` of the host address `offset` past `base`.
    Address {
        what: &'static str,
        base: Base,
        offset: usize,
        byte: u8,
    },
    /// A value only the packer's host knew that is not an address.
    Host(&'static str),
    /// Memory last written by a file the build does not produce.
    Unbuilt,
    /// Nonzero memory the heap returned to the system at some of the host
    /// heap addresses the recorded facts allow, and kept at others.
    Trimmed,
}

/// A value `fopen`, reading or `fclose` leaves in a `FILE` field.
#[derive(Clone, Copy)]
enum Field {
    Byte(u8),
    Host(&'static str),
    /// A host address: `base` plus a constant.
    Address(&'static str, Base, usize),
    /// An address inside this `FILE` object, which lies in the heap.
    Inside(&'static str, usize),
}

/// What the packer host's C library leaves in the `FILE` object that
/// `fopen` allocates and `fclose` frees.
pub(super) struct Libc {
    /// `sizeof (struct locked_FILE)`: the `fopen` allocation.
    locked_file: usize,
    /// Bytes `fopen`, reading and `fclose` leave written, as `(start, end,
    /// value)`. Every other byte keeps the memory's earlier contents.
    fields: &'static [(usize, usize, Field)],
}

/// The fields libc 2.1 and 2.2 share: `struct _IO_FILE` up to `_offset`.
/// `_lock` points at the mutex after the 0x98-byte `_IO_FILE_plus`.
const FILE_FIELDS: [(usize, usize, Field); 7] = [
    // _flags (fclose clears it), stream pointers, _markers
    (0x00, 0x34, Field::Byte(0)),
    (
        0x34,
        0x38,
        Field::Address("the closed FILE's _chain", Base::Chain, 0),
    ),
    (0x38, 0x3c, Field::Byte(0xff)), // _fileno = -1
    (0x3c, 0x40, Field::Host("the closed FILE's _blksize")),
    (0x44, 0x47, Field::Byte(0)), // _cur_column, _vtable_offset
    (0x48, 0x4c, Field::Inside("the closed FILE's _lock", 0x98)),
    (0x4c, 0x54, Field::Byte(0xff)), // _offset = _IO_pos_BAD
];
const JUMP_TABLE: (usize, usize, Field) = (
    0x94,
    0x98,
    Field::Address("the closed FILE's jump table", Base::FileJumps, 0),
);

/// GNU libc 2.1: `_IO_FILE_plus` (0x98 bytes) and a 24-byte mutex. The
/// old offset, `_shortbuf`, `_unused2` and the lock keep earlier contents.
static GLIBC_2_1: Libc = Libc {
    locked_file: 0xb0,
    fields: &[
        FILE_FIELDS[0],
        FILE_FIELDS[1],
        FILE_FIELDS[2],
        FILE_FIELDS[3],
        FILE_FIELDS[4],
        FILE_FIELDS[5],
        FILE_FIELDS[6],
        JUMP_TABLE,
    ],
};

/// GNU libc 2.2 adds wide-character streams. `_IO_FILE` gains `_codecvt`,
/// `_wide_data` and `_mode` after `_offset`, and `fopen` allocates the
/// stream's `struct _IO_wide_data` after the mutex: eleven buffer pointers,
/// two `__mbstate_t`, a `struct _IO_codecvt` (eight functions and two
/// `_G_iconv_t` of 0x2c bytes), `_shortbuf` and `_wide_vtable`, 0xbc bytes in
/// all. `_IO_no_init` clears the wide pointers and sets `_wide_data` and
/// `_wide_vtable`; `_codecvt`, the conversion state and `_unused2` keep
/// earlier contents. `_mode` turns from 0 to -1 when `fread` underflows,
/// which the host file system's block size decides.
static GLIBC_2_2: Libc = Libc {
    locked_file: 0x16c,
    fields: &[
        FILE_FIELDS[0],
        FILE_FIELDS[1],
        FILE_FIELDS[2],
        FILE_FIELDS[3],
        FILE_FIELDS[4],
        FILE_FIELDS[5],
        FILE_FIELDS[6],
        (
            0x58,
            0x5c,
            Field::Inside("the closed FILE's _wide_data", 0xb0),
        ),
        (
            0x5c,
            0x60,
            Field::Host("the closed FILE's _mode, set by the host's block size"),
        ),
        JUMP_TABLE,
        (0xb0, 0xdc, Field::Byte(0)), // the wide buffer pointers
        (
            0x168,
            0x16c,
            Field::Address("the closed FILE's wide jump table", Base::WideFileJumps, 0),
        ),
    ],
};

/// The machine that packed one edition's archive, as its reference machine
/// definition names it: the C library layout, and the host addresses
/// observed in the edition's alignment bytes, each as the lowest and highest
/// value its observed bits allow. Those are recorded facts about the build
/// machine, not reconstructed source.
pub(super) struct Host {
    libc: &'static Libc,
    addresses: BTreeMap<Base, (usize, usize)>,
}
impl Host {
    /// `edition`'s machine in a machine definition: its libc (an edition's
    /// own, else the machine's) and the host facts observed for it.
    pub(super) fn of(definition: &Value, edition: &str) -> Result<Self, String> {
        if definition["format"] != 1 {
            return Err("unsupported machine definition".into());
        }
        let libc = definition["editions"][edition]
            .get("libc")
            .unwrap_or(&definition["machine"]["libc"]);
        let libc = match json_string(&libc["layout"], "machine libc layout")? {
            "glibc-2.1" => &GLIBC_2_1,
            "glibc-2.2" => &GLIBC_2_2,
            other => return Err(format!("unsupported machine libc layout {other}")),
        };
        let mut addresses = BTreeMap::new();
        for fact in definition["host_facts"]
            .as_array()
            .ok_or("machine host facts are not an array")?
        {
            let name = json_string(&fact["address"], "host fact address")?;
            let base = Base::ALL
                .iter()
                .find(|(_, known)| *known == name)
                .ok_or_else(|| format!("unknown host fact address {name}"))?
                .0;
            let editions = fact["editions"]
                .as_array()
                .ok_or("host fact editions are not an array")?;
            if !editions.iter().any(|known| known == edition) {
                continue;
            }
            let lowest = number(&fact["value"]["lowest"], "lowest host address")?;
            let highest = number(&fact["value"]["highest"], "highest host address")?;
            if lowest > highest
                || fact["confidence"] != "observed"
                || fact["credit"] != "none"
                || fact["observed_from"].as_array().is_none_or(Vec::is_empty)
                || addresses.insert(base, (lowest, highest)).is_some()
            {
                return Err(format!(
                    "host fact {name} must be one observed, uncredited range with its observations"
                ));
            }
        }
        Ok(Self { libc, addresses })
    }
    /// A host byte, when the recorded bits of its address fix it.
    fn resolve(&self, what: &str, base: Base, offset: usize, byte: u8) -> Result<u8, String> {
        let Some(&(lowest, highest)) = self.addresses.get(&base) else {
            return Err(format!("{what}, which only the packer's host knew"));
        };
        let shift = 8 * u32::from(byte);
        let (low, high) = ((lowest + offset) >> shift, (highest + offset) >> shift);
        if low != high {
            return Err(format!(
                "{what}, whose byte the recorded bits of the host's {} do not fix",
                Base::ALL
                    .iter()
                    .find(|(known, _)| *known == base)
                    .unwrap()
                    .1
            ));
        }
        Ok(low as u8)
    }
    /// Every distance from the file buffer's chunk up to a page boundary
    /// that the recorded bits of the host's heap buffer allow: the break
    /// always ends on a page, so the top's size leaves this over whole pages.
    fn break_residues(&self) -> BTreeSet<usize> {
        let (lowest, highest) = self
            .addresses
            .get(&Base::HeapBuffer)
            .copied()
            .unwrap_or((CHUNK_ALIGNMENT, CHUNK_ALIGNMENT + PAGE - 1));
        (lowest.next_multiple_of(CHUNK_ALIGNMENT)..=highest)
            .step_by(CHUNK_ALIGNMENT)
            .take(PAGE / CHUNK_ALIGNMENT)
            .map(|buffer| (PAGE - (buffer - CHUNK_ALIGNMENT) % PAGE) % PAGE)
            .collect()
    }
}

/// The heap as seen from the packer's file buffer's address.
struct Heap<'a> {
    host: &'a Host,
    cells: Vec<Cell>,
    /// Every size the free top chunk, which starts at the file buffer's
    /// chunk, can have between files: one for each host heap address the
    /// recorded facts allow.
    tops: BTreeSet<usize>,
}

/// The top a fresh or just trimmed heap keeps: from the file buffer's chunk
/// to the first page boundary past a minimal chunk and the pad, where the
/// break's distance from that chunk leaves `residue` over whole pages.
fn fresh_top(residue: usize) -> usize {
    let floor = MIN_CHUNK + TOP_PAD + 1;
    floor + (residue % PAGE + PAGE - floor % PAGE) % PAGE
}

impl<'a> Heap<'a> {
    /// Memory the heap has never used is fresh, zero-filled break space:
    /// before the first file the top is only what the break's rounding to a
    /// page left past the packer's earlier, smaller allocations.
    fn new(host: &'a Host) -> Self {
        Self {
            host,
            cells: Vec::new(),
            tops: host.break_residues().into_iter().map(fresh_top).collect(),
        }
    }
    /// `malloc` of a `request`-byte chunk after `used` bytes of the top:
    /// a top that would keep less than a minimal chunk first grows by the
    /// request, padded and rounded up to whole pages.
    fn grow(&mut self, used: usize, request: usize) {
        self.tops = self
            .tops
            .iter()
            .map(|&top| {
                if top < used + request + MIN_CHUNK {
                    top + (request + MIN_CHUNK + TOP_PAD).next_multiple_of(PAGE)
                } else {
                    top
                }
            })
            .collect();
    }
    /// The `free` that consolidates everything into the top: a top of the
    /// trim threshold or more shrinks to a fresh heap's, and the memory past
    /// it reads back as zeros. Memory only some possibilities release is
    /// unknown unless zero.
    fn trim(&mut self) {
        let kept = |top: usize| (top >= TRIM_THRESHOLD).then(|| fresh_top(top));
        let released = self
            .tops
            .iter()
            .map(|&top| kept(top).map(|kept| kept - CHUNK_ALIGNMENT))
            .collect::<Vec<_>>();
        let Some(&first) = released.iter().flatten().min() else {
            return;
        };
        let everywhere = match released.iter().copied().collect::<Option<Vec<_>>>() {
            Some(all) => all.into_iter().max().unwrap_or(first),
            None => usize::MAX,
        };
        for at in first..everywhere.min(self.cells.len()) {
            if self.cells[at] != Cell::Byte(0) {
                self.cells[at] = Cell::Trimmed;
            }
        }
        self.cells.truncate(everywhere.min(self.cells.len()));
        self.tops = self
            .tops
            .iter()
            .map(|&top| kept(top).unwrap_or(top))
            .collect();
    }
    fn set(&mut self, at: usize, value: Cell) {
        if at >= self.cells.len() {
            self.cells.resize(at + 1, Cell::Byte(0));
        }
        self.cells[at] = value;
    }
    fn fill(&mut self, start: usize, end: usize, value: Cell) {
        for at in start..end {
            self.set(at, value);
        }
    }
    fn word(&mut self, at: usize, value: usize) {
        for (index, byte) in (value as u32).to_le_bytes().into_iter().enumerate() {
            self.set(at + index, Cell::Byte(byte));
        }
    }
    /// A host address's four bytes at `at`.
    fn address(&mut self, at: usize, what: &'static str, base: Base, offset: usize) {
        for byte in 0..4 {
            self.set(
                at + byte,
                Cell::Address {
                    what,
                    base,
                    offset,
                    byte: byte as u8,
                },
            );
        }
    }
    fn file_chunk(&self) -> usize {
        chunk(self.host.libc.locked_file)
    }
    /// The bytes a write rounded up to `end` emits after a file of `start`
    /// bytes, or why the first underivable one is unknown.
    fn tail(&self, start: usize, end: usize) -> Result<Vec<u8>, String> {
        (start..end)
            .map(|at| {
                let unknown = |why: String| format!("alignment byte +0x{at:x} is {why}");
                match self.cells.get(at).copied().unwrap_or(Cell::Byte(0)) {
                    Cell::Byte(byte) => Ok(byte),
                    Cell::Address {
                        what,
                        base,
                        offset,
                        byte,
                    } => self.host.resolve(what, base, offset, byte).map_err(unknown),
                    Cell::Host(what) => Err(unknown(format!(
                        "{what}, which only the packer's host knew"
                    ))),
                    Cell::Unbuilt => Err(unknown(
                        "memory last written by a resource file the build does not produce".into(),
                    )),
                    Cell::Trimmed => Err(unknown(
                        "memory the heap kept or returned to the system depending on the host's heap address"
                            .into(),
                    )),
                }
            })
            .collect()
    }
    /// The closed `FILE` object whose user data starts at `at`, and the size
    /// field of the top chunk that `malloc` split off after it.
    fn file_object(&mut self, at: usize) {
        for &(start, end, value) in self.host.libc.fields {
            match value {
                Field::Byte(byte) => self.fill(at + start, at + end, Cell::Byte(byte)),
                Field::Host(what) => self.fill(at + start, at + end, Cell::Host(what)),
                Field::Address(what, base, offset) => self.address(at + start, what, base, offset),
                Field::Inside(what, offset) => {
                    self.address(at + start, what, Base::HeapBuffer, at + offset)
                }
            }
        }
        let top = at - 8 + self.file_chunk();
        self.fill(top + 4, top + 8, Cell::Host("the heap's top chunk size"));
    }
    /// The heap chunk a file of `extent` bytes takes before its `FILE`, and
    /// the heap's growth for both: none for a mapped buffer.
    fn allocate(&mut self, extent: usize) -> usize {
        let buffer = chunk(extent);
        let buffer = if buffer >= MMAP_THRESHOLD {
            0
        } else {
            self.grow(0, buffer);
            buffer
        };
        self.grow(buffer, self.file_chunk());
        buffer
    }
    /// Replay one file: `malloc`, `fopen`, `fread`, write, `free`, `fclose`.
    fn pack(&mut self, file: &[u8]) {
        let buffer = self.allocate(file.len());
        if buffer == 0 {
            // The mapped buffer leaves the heap; the FILE takes the heap top.
            self.file_object(0);
            self.trim();
            return;
        }
        for (at, byte) in file.iter().enumerate() {
            self.set(at, Cell::Byte(*byte));
        }
        self.file_object(buffer);
        // free(buf) while the FILE is open links the buffer to its empty bin
        // (both links hold the bin), writes its footer and clears the FILE
        // chunk's previous-in-use bit.
        let bin = 8 * bin_index(buffer);
        for link in [0, 4] {
            self.address(link, "the freed buffer's bin link", Base::MainArena, bin);
        }
        self.word(buffer - 8, buffer);
        self.word(buffer - 4, self.file_chunk());
        self.trim();
    }
    /// A file the build could not produce: everything up to the latest place
    /// of its `FILE` is unknown.
    fn pack_unbuilt(&mut self, extent: usize) {
        let end = self.allocate(extent) + self.file_chunk() + 8;
        self.fill(0, end, Cell::Unbuilt);
        self.trim();
    }
}

/// `request2size`: the chunk holding a request, header word included.
fn chunk(request: usize) -> usize {
    ((request + 4 + 7) & !7).max(16)
}

/// `bin_index`: the `main_arena` bin of a free chunk, whose header lies
/// eight bytes per index past the arena (`bin_at`).
fn bin_index(size: usize) -> usize {
    match size >> 9 {
        _ if size < MAX_SMALLBIN_SIZE => size >> 3,
        0..=4 => 56 + (size >> 6),
        5..=20 => 91 + (size >> 9),
        21..=84 => 110 + (size >> 12),
        85..=340 => 119 + (size >> 15),
        341..=1364 => 124 + (size >> 18),
        _ => 126,
    }
}

/// Resource starts after the directory table, in archive (address) order.
pub(super) fn resource_starts(directory: &Value) -> Result<Vec<usize>, String> {
    let table = number(&directory["address"], "resource directory address")?;
    let mut starts = directory["slots"]
        .as_array()
        .ok_or("resource directory slots are not an array")?
        .iter()
        .filter_map(|slot| slot.as_str().filter(|text| text.starts_with("0x")))
        .map(|slot| number(&Value::from(slot), "resource address"))
        .collect::<Result<Vec<_>, _>>()?
        .into_iter()
        .filter(|&address| address > table)
        .collect::<Vec<_>>();
    starts.sort_unstable();
    starts.dedup();
    Ok(starts)
}

/// A built region: its bytes, which a stream leaves short of its alignment,
/// and the size it occupies in the archive.
pub(super) struct Region<'a> {
    pub(super) bytes: &'a [u8],
    pub(super) size: usize,
}

/// The alignment bytes of every region that stops short of its size, keyed
/// by address, or why they cannot be derived.
pub(super) type Alignment = BTreeMap<usize, Result<Vec<u8>, String>>;

/// Replay the archive of resources starting at `starts` on `host` over the
/// built `regions` and return the alignment bytes of every region whose
/// bytes stop short of its size. A resource with an unbuilt span leaves its
/// span of the heap unknown.
pub(super) fn alignment(
    host: &Host,
    starts: &[usize],
    regions: &BTreeMap<usize, Region>,
) -> Alignment {
    let mut heap = Heap::new(host);
    let mut aligned = Alignment::new();
    for (index, &start) in starts.iter().enumerate() {
        let end = match starts.get(index + 1) {
            Some(&next) => next,
            None => archive_end(start, regions),
        };
        let members = regions.range(start..end).collect::<Vec<_>>();
        resource(&mut heap, &mut aligned, start, end, &members);
    }
    for (&address, region) in regions {
        if region.bytes.len() < region.size && !aligned.contains_key(&address) {
            aligned.insert(
                address,
                Err(format!(
                    "stream ends 0x{:x} bytes short of its region, which no resource of the directory holds",
                    region.size - region.bytes.len()
                )),
            );
        }
    }
    aligned
}

/// Where the last resource, and with it the archive, ends: no directory
/// pointer follows it. A last resource that opens with a stream stored short
/// of its region is that stream's file, which the packer wrote rounded up to
/// its region's end. Any other last resource ends with the last region it
/// holds. What the image places after the archive, such as its fill to the
/// cartridge size, is no resource's.
fn archive_end(start: usize, regions: &BTreeMap<usize, Region>) -> usize {
    let mut members = regions.range(start..);
    match members.next() {
        Some((&address, region)) if address == start && region.bytes.len() < region.size => {
            address + region.size
        }
        first => members
            .last()
            .or(first)
            .map_or(start, |(address, region)| address + region.size),
    }
}

/// Lay out one resource file from the regions it holds and replay it on the
/// heap. The file is its regions end to end. A writer that aligned streams
/// inside its file, zero-filling a gap after a region before the last, ended
/// the file aligned the same way, and the packer pads nothing. A file with
/// no such gap, a bare stream or a palette and its stream, ends where its
/// writer stopped: the packer pads its last region from the heap.
fn resource(
    heap: &mut Heap,
    aligned: &mut Alignment,
    start: usize,
    end: usize,
    members: &[(&usize, &Region)],
) {
    let writer_aligned = members
        .iter()
        .rev()
        .skip(1)
        .any(|(_, region)| region.bytes.len() < region.size);
    let mut file = Vec::new();
    let mut complete = true;
    let mut padded = false;
    for (index, (&address, region)) in members.iter().enumerate() {
        if start + file.len() != address {
            complete = false;
        }
        let built = region.bytes.len();
        if built > region.size || address + region.size > end {
            complete = false;
            continue;
        }
        if complete {
            file.extend_from_slice(region.bytes);
        }
        if built == region.size {
            continue;
        }
        let short = region.size - built;
        let padding = if short >= ALIGNMENT || (address + region.size - start) % ALIGNMENT != 0 {
            Err(format!(
                "stream ends 0x{short:x} bytes before its region's end, which is not its file's next four-byte boundary"
            ))
        } else if index + 1 == members.len() && address + region.size == end && !writer_aligned {
            padded = true;
            heap.tail(address + built - start, end - start)
        } else {
            Ok(vec![0; short])
        };
        match &padding {
            Ok(bytes) if complete && !padded => file.extend_from_slice(bytes),
            Err(_) if !padded => complete = false,
            _ => {}
        }
        aligned.insert(address, padding);
    }
    // The packer wrote only the file's own bytes; its padding is memory the
    // file never wrote. A file whose regions are all built therefore stands
    // on the heap as those bytes even when its padding is left unregistered.
    let unregistered_padding = !writer_aligned
        && !file.is_empty()
        && start + file.len().div_ceil(ALIGNMENT) * ALIGNMENT == end;
    if complete && (padded || start + file.len() == end || unregistered_padding) {
        heap.pack(&file);
    } else {
        heap.pack_unbuilt(end - start);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn host(libc: &'static Libc) -> Host {
        Host {
            libc,
            addresses: BTreeMap::new(),
        }
    }

    #[test]
    fn alignment_reads_the_reused_buffer_and_the_closed_file_object() {
        let host = host(&GLIBC_2_1);
        let mut heap = Heap::new(&host);
        assert_eq!(heap.tail(5, 8), Ok(vec![0, 0, 0]));
        heap.pack(&[7; 0x40]);
        // A shorter file sees the longer file's tail.
        assert_eq!(heap.tail(0x21, 0x24), Ok(vec![7, 7, 7]));
        // The FILE chunk follows the 0x48-byte buffer chunk: footer, size,
        // then the object's cleared flags and pointers.
        assert_eq!(
            heap.tail(0x40, 0x48),
            Ok(vec![0x48, 0, 0, 0, 0xb8, 0, 0, 0])
        );
        assert_eq!(heap.tail(0x48, 0x4c), Ok(vec![0; 4]));
        assert_eq!(heap.tail(0x48 + 0x38, 0x48 + 0x3c), Ok(vec![0xff; 4]));
        // _shortbuf keeps the earlier memory; _lock is a host address.
        assert_eq!(heap.tail(0x48 + 0x47, 0x48 + 0x48), Ok(vec![0]));
        assert!(heap
            .tail(0x48 + 0x48, 0x48 + 0x4c)
            .unwrap_err()
            .contains("+0x90 is the closed FILE's _lock, which only the packer's host knew"));
        // The freed buffer's bin links are libc addresses.
        assert!(heap.tail(4, 8).unwrap_err().contains("bin link"));
    }

    #[test]
    fn libc_2_2_streams_carry_their_wide_data() {
        let host = host(&GLIBC_2_2);
        let mut heap = Heap::new(&host);
        heap.pack(&[9; 0x200]);
        heap.pack(&[7; 0x40]);
        // The 0x16c-byte locked_FILE takes a 0x170-byte chunk.
        assert_eq!(
            heap.tail(0x40, 0x48),
            Ok(vec![0x48, 0, 0, 0, 0x70, 1, 0, 0])
        );
        assert!(heap
            .tail(0x48 + 0x58, 0x48 + 0x5c)
            .unwrap_err()
            .contains("_wide_data"));
        // _unused2 and the lock keep the earlier file's bytes; the wide
        // buffer pointers are cleared.
        assert_eq!(heap.tail(0x48 + 0x60, 0x48 + 0x64), Ok(vec![9; 4]));
        assert_eq!(heap.tail(0x48 + 0x98, 0x48 + 0xb0), Ok(vec![9; 0x18]));
        assert_eq!(heap.tail(0x48 + 0xb0, 0x48 + 0xdc), Ok(vec![0; 0x2c]));
        assert!(heap
            .tail(0x48 + 0x16c, 0x48 + 0x170)
            .unwrap_err()
            .contains("top chunk"));
    }

    #[test]
    fn recorded_host_addresses_fix_only_the_bytes_their_bits_decide() {
        let fact = |address: &str, lowest: &str, highest: &str| {
            serde_json::json!({"address": address, "editions": ["x"], "value": {"lowest": lowest, "highest": highest},
                "confidence": "observed", "observed_from": ["test"], "credit": "none"})
        };
        let document = serde_json::json!({"format": 1,
            "machine": {"libc": {"layout": "glibc-2.2"}},
            "editions": {"x": {"libc": {"layout": "glibc-2.1"}}},
            "host_facts": [
                fact("heap_buffer", "0x0804f210", "0x0804f218"),
                fact("main_arena", "0x40120000", "0x4012ff00"),
            ]
        });
        let host = Host::of(&document, "x").unwrap();
        assert!(std::ptr::eq(host.libc, &GLIBC_2_1));
        let mut heap = Heap::new(&host);
        heap.pack(&[7; 0x40]);
        // _lock is the heap buffer + 0x48 + 0x98: its upper three bytes are
        // fixed, its lowest is not.
        let lock = 0x48 + 0x48;
        assert_eq!(heap.tail(lock + 1, lock + 4), Ok(vec![0xf2, 0x04, 0x08]));
        assert!(heap
            .tail(lock, lock + 1)
            .unwrap_err()
            .contains("do not fix"));
        // Both bin links hold bin 9 (a 0x48-byte chunk): the arena + 0x48.
        assert_eq!(heap.tail(2, 4), Ok(vec![0x12, 0x40]));
        assert_eq!(heap.tail(6, 8), Ok(vec![0x12, 0x40]));
        assert!(heap.tail(0, 1).is_err());
        // An address the record lacks stays the host's.
        assert!(heap
            .tail(0x48 + 0x94, 0x48 + 0x98)
            .unwrap_err()
            .contains("only the packer's host knew"));
        // Another edition takes the machine's libc and none of the facts.
        let other = Host::of(&document, "y").unwrap();
        assert!(std::ptr::eq(other.libc, &GLIBC_2_2) && other.addresses.is_empty());
        // A fact must be an observed, uncredited range with its observations.
        for (key, value) in [
            ("observed_from", serde_json::json!([])),
            ("confidence", serde_json::json!("assumed")),
            ("credit", serde_json::json!("exact")),
        ] {
            let mut unlabelled = document.clone();
            unlabelled["host_facts"][1][key] = value;
            assert!(Host::of(&unlabelled, "x").is_err());
        }
    }

    #[test]
    fn a_trimmed_heap_reads_back_zeros_past_its_kept_page() {
        // The buffer's chunk lies 0x100 bytes below a page boundary.
        let mut exact = host(&GLIBC_2_2);
        exact
            .addresses
            .insert(Base::HeapBuffer, (0x0805_0f08, 0x0805_0f08));
        assert_eq!(exact.break_residues(), BTreeSet::from([0x100]));
        let unknown = host(&GLIBC_2_2);
        assert_eq!(unknown.break_residues().len(), PAGE / CHUNK_ALIGNMENT);
        for host in [&exact, &unknown] {
            let mut heap = Heap::new(host);
            heap.pack(&[7; 0x10000]);
            // The top grew to under 128 KiB and kept every page.
            assert!(heap.tops.iter().all(|&top| top < TRIM_THRESHOLD));
            assert_eq!(heap.tail(0x2000, 0x2004), Ok(vec![7; 4]));
            heap.pack(&[9; 0x1c000]);
            // Growing for the second file took the free top past 128 KiB:
            // its pages past the first boundary returned and read as zeros.
            assert!(heap.tops.iter().all(|&top| top <= PAGE + MIN_CHUNK));
            assert_eq!(heap.tail(0x2000, 0x2004), Ok(vec![0; 4]));
        }
        let mut heap = Heap::new(&exact);
        heap.pack(&[7; 0x10000]);
        heap.pack(&[9; 0x1c000]);
        assert_eq!(heap.tops, BTreeSet::from([0x100]));
        assert_eq!(heap.tail(0xf0, 0xf8), Ok(vec![9; 8]));
        assert_eq!(heap.tail(0xf8, 0x100), Ok(vec![0; 8]));
        // Without the address, where the kept page ends is unknown.
        let mut heap = Heap::new(&unknown);
        heap.pack(&[7; 0x10000]);
        heap.pack(&[9; 0x1c000]);
        assert!(heap
            .tail(0xf0, 0xf8)
            .unwrap_err()
            .contains("kept or returned to the system"));
        assert_eq!(heap.tail(PAGE + 8, PAGE + 12), Ok(vec![0; 4]));
    }

    #[test]
    fn free_chunks_link_to_their_size_bins() {
        assert_eq!(bin_index(0x10), 2);
        assert_eq!(bin_index(0x1f8), 63);
        assert_eq!(bin_index(0x200), 64);
        assert_eq!(bin_index(0xa00), 96);
        assert_eq!(bin_index(0x14c8), 101);
    }

    fn starts(starts: &[usize]) -> Vec<usize> {
        starts.to_vec()
    }

    #[test]
    fn stream_files_take_the_heap_and_containers_align_with_zeros() {
        let long = [7u8; 0x40];
        let short = [1u8; 0x21];
        let header = [9u8; 8];
        let inner = [2u8; 5];
        let last = [3u8; 6];
        let regions = BTreeMap::from([
            // A stream file longer than the next one.
            (
                0x200,
                Region {
                    bytes: &long,
                    size: 0x40,
                },
            ),
            // A stream file: its alignment is the earlier file's tail.
            (
                0x240,
                Region {
                    bytes: &short,
                    size: 0x24,
                },
            ),
            // A container: header, then streams aligned with zeros.
            (
                0x264,
                Region {
                    bytes: &header,
                    size: 8,
                },
            ),
            (
                0x26c,
                Region {
                    bytes: &inner,
                    size: 8,
                },
            ),
            (
                0x274,
                Region {
                    bytes: &last,
                    size: 8,
                },
            ),
        ]);
        let host = host(&GLIBC_2_1);
        let aligned = alignment(&host, &starts(&[0x200, 0x240, 0x264]), &regions);
        assert_eq!(aligned[&0x240], Ok(vec![7, 7, 7]));
        assert_eq!(aligned[&0x26c], Ok(vec![0; 3]));
        assert_eq!(aligned[&0x274], Ok(vec![0; 2]));
        assert!(!aligned.contains_key(&0x200));
    }

    #[test]
    fn files_without_inner_alignment_take_the_heap() {
        let long = [7u8; 0x40];
        let palette = [5u8; 4];
        let stream = [1u8; 0x1d];
        let regions = BTreeMap::from([
            (
                0x200,
                Region {
                    bytes: &long,
                    size: 0x40,
                },
            ),
            // A palette and its stream, written whole: nothing aligned the
            // palette, so the packer pads the stream from the heap.
            (
                0x240,
                Region {
                    bytes: &palette,
                    size: 4,
                },
            ),
            (
                0x244,
                Region {
                    bytes: &stream,
                    size: 0x20,
                },
            ),
        ]);
        let host = host(&GLIBC_2_1);
        let aligned = alignment(&host, &starts(&[0x200, 0x240, 0x264]), &regions);
        assert_eq!(aligned[&0x244], Ok(vec![7, 7, 7]));
        assert_eq!(aligned.len(), 1);
    }

    #[test]
    fn the_archive_ends_with_its_last_stream_file() {
        let long = [7u8; 0x40];
        let last = [1u8; 0x21];
        let fill = [0u8; 0x1c];
        let regions = BTreeMap::from([
            (
                0x200,
                Region {
                    bytes: &long,
                    size: 0x40,
                },
            ),
            // The last file: a stream the packer rounded up from the heap.
            (
                0x240,
                Region {
                    bytes: &last,
                    size: 0x24,
                },
            ),
            // The image's fill after the archive belongs to no resource.
            (
                0x264,
                Region {
                    bytes: &fill,
                    size: 0x1c,
                },
            ),
        ]);
        let host = host(&GLIBC_2_1);
        let aligned = alignment(&host, &starts(&[0x200, 0x240]), &regions);
        assert_eq!(aligned[&0x240], Ok(vec![7, 7, 7]));
        assert_eq!(aligned.len(), 1);
        // A last resource that does not open with a short stream ends with
        // the last region it holds.
        assert_eq!(archive_end(0x200, &regions), 0x280);
        assert_eq!(archive_end(0x240, &regions), 0x264);
        assert_eq!(archive_end(0x300, &regions), 0x300);
    }

    #[test]
    fn files_with_unregistered_padding_still_stand_on_the_heap() {
        let long = [7u8; 0x40];
        let file = [5u8; 0x1d];
        let short = [1u8; 0x11];
        // The file at 0x240 is built whole; its three padding bytes are not
        // registered, so they are not derived, but the next file's padding
        // still reads the bytes it wrote.
        let regions = BTreeMap::from([
            (
                0x200,
                Region {
                    bytes: &long,
                    size: 0x40,
                },
            ),
            (
                0x240,
                Region {
                    bytes: &file,
                    size: 0x1d,
                },
            ),
            (
                0x260,
                Region {
                    bytes: &short,
                    size: 0x14,
                },
            ),
        ]);
        let host = host(&GLIBC_2_1);
        let aligned = alignment(&host, &starts(&[0x200, 0x240, 0x260]), &regions);
        assert_eq!(aligned[&0x260], Ok(vec![5, 5, 5]));
        assert!(!aligned.contains_key(&0x240));
        // Bytes beyond the file's padding are part of the file: without them
        // the file is unbuilt.
        let gapped = BTreeMap::from([
            (
                0x240,
                Region {
                    bytes: &file[..0x19],
                    size: 0x19,
                },
            ),
            (
                0x260,
                Region {
                    bytes: &short,
                    size: 0x14,
                },
            ),
        ]);
        let aligned = alignment(&host, &starts(&[0x240, 0x260]), &gapped);
        assert!(aligned[&0x260].is_err());
    }

    #[test]
    fn unbuilt_files_leave_the_heap_unknown() {
        let first = [7u8; 0x21];
        let third = [1u8; 0x11];
        // The resource at 0x240 did not build: its buffer and FILE are unknown.
        let regions = BTreeMap::from([
            (
                0x200,
                Region {
                    bytes: &first,
                    size: 0x24,
                },
            ),
            (
                0x280,
                Region {
                    bytes: &third,
                    size: 0x14,
                },
            ),
        ]);
        let host = host(&GLIBC_2_1);
        let aligned = alignment(&host, &starts(&[0x200, 0x240, 0x280]), &regions);
        assert_eq!(aligned[&0x200], Ok(vec![0; 3]));
        assert!(aligned[&0x280].as_ref().unwrap_err().contains(
            "+0x11 is memory last written by a resource file the build does not produce"
        ));
        // A stream outside every resource has no packer.
        let outside = BTreeMap::from([(
            0x80,
            Region {
                bytes: &third,
                size: 0x14,
            },
        )]);
        let aligned = alignment(&host, &starts(&[0x200]), &outside);
        assert!(aligned[&0x80].as_ref().unwrap_err().contains("no resource"));
    }

    /// The evidence for each edition's recorded libc, where the local ROMs
    /// exist (a test oracle only): replaying each edition's archive from its
    /// own stored files, the recorded library's `FILE` never contradicts an
    /// alignment byte it derives, while the other library's does.
    #[test]
    fn recorded_host_libc_agrees_with_every_local_edition() {
        use crate::targets::{target_for, TARGET_IDS};
        let root = super::super::repository_root();
        for id in TARGET_IDS {
            let target = target_for(id);
            let Ok(rom) = std::fs::read(root.join(target.rom)) else {
                continue;
            };
            let read = |name: String| {
                serde_json::from_slice::<Value>(&std::fs::read(root.join(name)).unwrap()).unwrap()
            };
            let slots = read(format!(
                "{}/SYSTEM/RESOURCE/DIRECTORY.JSON",
                target.source_dir
            ))["slot_count"]
                .as_u64()
                .unwrap() as usize;
            let recorded = Host::of(
                &read(format!("{}/machine.json", target.recon_dir())),
                id.as_str(),
            )
            .unwrap();
            let other = host(if std::ptr::eq(recorded.libc, &GLIBC_2_1) {
                &GLIBC_2_2
            } else {
                &GLIBC_2_1
            });
            assert_eq!(contradictions(&rom, slots, &recorded), 0, "{id}");
            assert!(contradictions(&rom, slots, &other) > 0, "{id}");
        }
    }

    /// The file a stored resource holds when it is a 128-colour palette and
    /// a delta7 still of 120 rows of 256 or 512 pixels ending in its final
    /// word.
    fn still_file(stored: &[u8]) -> Option<usize> {
        use psynergy::assets::compression::{decode_delta7, encode_delta7};
        let stream = stored.get(0x100..)?;
        [256, 512].into_iter().find_map(|width| {
            let stream = encode_delta7(&decode_delta7(stream, width * 120).ok()?).ok()?;
            let file = 0x100 + stream.len();
            (file <= stored.len()
                && stored.len() - file < ALIGNMENT
                && stored[0x100..file] == stream[..])
                .then_some(file)
        })
    }

    /// Alignment bytes the replay derives differently from the ROM's, taking
    /// each resource's file to be its tagged stream or still when that ends
    /// in the resource's final word, and the whole resource otherwise.
    fn contradictions(rom: &[u8], slots: usize, host: &Host) -> usize {
        let word = |at: usize| u32::from_le_bytes(rom[at..at + 4].try_into().unwrap()) as usize;
        let table = (0..rom.len() - 8)
            .step_by(4)
            .find(|&at| word(at) == 0x0800_0000 && word(at + 4) == 0x0800_0000 + at)
            .expect("resource directory");
        let mut starts = (0..slots)
            .map(|slot| word(table + slot * 4))
            .filter(|&start| start > 0x0800_0000 + table && start < 0x0800_0000 + rom.len())
            .collect::<Vec<_>>();
        starts.sort_unstable();
        starts.dedup();
        let mut heap = Heap::new(host);
        let mut contradictions = 0;
        for pair in starts.windows(2) {
            let stored = &rom[pair[0] - 0x0800_0000..pair[1] - 0x0800_0000];
            let file = match psynergy::assets::lz::tagged_stream_length(
                stored,
                0,
                stored.len(),
                1 << 24,
            ) {
                Ok(length) if stored.len() - length < ALIGNMENT => length,
                _ => still_file(stored).unwrap_or(stored.len()),
            };
            if heap
                .tail(file, stored.len())
                .is_ok_and(|tail| tail != stored[file..])
            {
                contradictions += 1;
            }
            heap.pack(&stored[..file]);
        }
        contradictions
    }
}
