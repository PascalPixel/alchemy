//! The Golden Sun asset encoders, as pret's gbagfx, mid2agb, wav2agb and
//! preproc are for Pokémon: each turns one editable input (an indexed PNG,
//! a tilemap or table, a MIDI, a WAV, a PO catalog) into
//! the bytes or assembler source the build links, and compresses with the
//! resource packer's rules. None reads a ROM and none takes an address: data
//! that points at other data is emitted as `asm::Data` whose address words
//! name labels the linker resolves. The binaries agsgfx, mid2ags, wav2ags
//! and po2ags put each encoder on the command line; Alchemy's build calls
//! the same functions.
#![allow(dead_code)]
pub mod asm;
pub mod graphics;
pub mod lz;
pub mod resource;
pub mod sound;
pub mod text;
