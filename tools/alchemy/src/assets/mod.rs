//! Alchemy's asset encoders, its gbagfx, mid2agb and preproc: each turns one
//! editable input (an indexed PNG, a tilemap, a JSON table or sequence, a
//! MIDI, a WAV, a PO catalog) into the bytes or assembler source the build
//! links, and compresses with the resource packer's rules. None reads a ROM
//! and none takes an address: data that points at other data is emitted as
//! `asm::Data` whose address words name labels the linker resolves.
//!
//! `build rom` compresses code overlays with `lz` today; the graphics, table,
//! sound and text encoders wait for the build rules that link their inputs,
//! and until then only their tests reach them.
#![allow(dead_code)]
pub(crate) mod asm;
pub(crate) mod graphics;
pub(crate) mod lz;
pub(crate) mod sound;
pub(crate) mod table;
pub(crate) mod text;
