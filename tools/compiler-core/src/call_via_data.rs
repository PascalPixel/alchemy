//! Ordered native `call_via` bank tables. Indirect calls branch to a `bx rN`
//! stub; each overlay has its own bank, so the main-image address is not valid.
//! Ordered slices preserve element-wise drift checks; these tiny lookups are cold.

/// `CALL_VIA_BASE` -- the main image's `bx rN` bank, which is what a `src/`
/// translation unit links against.
pub const CALL_VIA_BASE: u64 = 0x0800_72e4;

/// `CALL_VIA_REGISTERS` -- the four register aliases, in declaration order.
pub static CALL_VIA_REGISTERS: &[(&str, u64)] = &[("sl", 10), ("fp", 11), ("ip", 12), ("sp", 13)];

/// Overlay banks are recorded because their deriving `bl` may later become C.
pub static OVERLAY_CALL_VIA_BASE: &[(&str, u64)] = &[
    ("resource_373", 0x0200_61b4),
    ("resource_382", 0x0200_3138),
    ("resource_385", 0x0200_14fc),
    ("resource_389", 0x0200_1578),
    ("resource_391", 0x0200_2d8c),
    ("resource_392", 0x0200_0eec),
    ("resource_393", 0x0200_0f34),
    ("resource_39b", 0x0200_2960),
    ("resource_39c", 0x0200_5fb0),
    ("resource_39d", 0x0200_4108),
    ("resource_39f", 0x0200_2f1c),
    ("resource_3a0", 0x0200_18bc),
    ("resource_3a5", 0x0200_2c6e),
    ("resource_3a6", 0x0200_20a8),
    ("resource_3b2", 0x0200_3180),
    ("resource_3b3", 0x0200_2f00),
    ("resource_3b4", 0x0200_2668),
    ("resource_3b5", 0x0200_0edc),
    ("resource_3b7", 0x0200_28e0),
    ("resource_3ba", 0x0200_3f4e),
    ("resource_3bc", 0x0200_4d4e),
    ("resource_3bd", 0x0200_3ed8),
    ("resource_3be", 0x0200_1920),
    ("resource_3bf", 0x0200_5810),
    ("resource_3c4", 0x0200_3214),
    ("resource_3c5", 0x0200_2ff8),
    ("resource_3c8", 0x0200_5324),
    ("resource_3c9", 0x0200_6384),
    ("resource_3cb", 0x0200_1a96),
];
