//! Legacy main-image call-via parameter and ARM register spelling aliases.
//! Overlay bank positions are derived from the explicitly selected image.

/// `CALL_VIA_BASE` -- the main image's `bx rN` bank, which is what a `src/`
/// translation unit links against.
pub const CALL_VIA_BASE: u64 = 0x0800_72e4;

/// `CALL_VIA_REGISTERS` -- the four register aliases, in declaration order.
pub static CALL_VIA_REGISTERS: &[(&str, u64)] = &[("sl", 10), ("fp", 11), ("ip", 12), ("sp", 13)];
