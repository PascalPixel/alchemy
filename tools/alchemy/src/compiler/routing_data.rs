// Compiler-family membership. These tables say which compiler and library
// produced a source's bytes; they never carry a per-file flag. Every member
// of a family compiles with that family's single flag set (routing.rs), and a
// source that is not exact under its family's flags is not exact.
//
// Members are canonical source-owner IDs (`main:ADDRESS` or
// `resource_XXX:ADDRESS`), the same keys as each game's source-paths.json.
// Routing belongs to the owner, not to whatever file currently holds it, so a
// member keeps its family when its source is renamed, moved, or grouped.

// Library code built with agbcc.
pub static AGBCC_SOURCES: &[&str] = &[
    "main:08006f48",
    "main:08006a78",
    "main:080fb73c",
    // Voice-field setters beside the decay, sustain and release setters.
    "main:080fb6ec",
    "main:080fb700",
    "main:080fb768",
    "main:080fb77c",
    "main:080fb728",
    "main:080fb714",
    "main:080fb75c",
    "main:080fb750",
    "main:080fada0",
    "main:080fadf0",
    "main:080fa1fc",
    "main:080fa264",
    "main:080fa2a0",
    "main:080fa324",
    "main:080fa350",
    "main:080fa39c",
    "main:080fa3f0",
    "main:080fa424",
    "main:080fa458",
    "main:080fa490",
    "main:080fa514",
    "main:080fa55c",
    // The two command-slot trampolines between the CGB and engine initializers
    // pick their call_via register as agbcc does (r1), not as agscc does (r3).
    "main:080fa678",
    "main:080fa68c",
    "main:080fa6a0",
    "main:080fa83c",
    "main:080fa8d4",
    "main:080fa928",
    "main:080fa9a4",
    "main:080fa9e0",
    "main:080fab3c",
    "main:080fab7c",
    "main:080fac44",
    "main:080facf8",
    "main:080fb2cc",
    "main:080fb334",
    "main:080fb3a8",
    "main:080fb430",
    "main:080fb4a4",
    "main:080fb670",
    "main:080fb6a4",
    "main:080fa280",
    "main:080fa4cc",
    "main:080fa798",
    "main:080faa58",
    // The CGB channel update of the MusicPlayer2000 C object, exact under
    // this family's -O2 set like its neighbours.
    "main:080fae58",
    "main:080fb2a4",
    "main:080fb410",
    "main:080fb518",
];
// TLA's copy of the agbcc-built audio engine library.
pub static TLA_AGBCC_SOURCES: &[&str] = &[
    "main:081c0428",
    "main:081c048c",
    "main:081c0550",
    "main:081c057c",
    "main:081c0f70",
    "main:081c0fac",
    "main:081c0fd0",
    "main:081c1e98",
    "main:081c1efc",
    "main:081c1f3c",
    "main:081c1fc0",
    "main:081c1fec",
    "main:081c2038",
    "main:081c208c",
    "main:081c20c0",
    "main:081c20f4",
    "main:081c2120",
    "main:081c212c",
    "main:081c2158",
    "main:081c2168",
    "main:081c21f8",
    "main:081c233c",
    "main:081c2434",
    "main:081c24d8",
    "main:081c2570",
    "main:081c25c4",
    "main:081c2640",
    "main:081c267c",
    "main:081c26f4",
    "main:081c27d8",
    "main:081c2818",
    "main:081c28e0",
    "main:081c2994",
    "main:081c2a3c",
    "main:081c2a8c",
    "main:081c2f68",
    "main:081c2fd0",
    "main:081c3044",
    "main:081c30ac",
    "main:081c30cc",
    "main:081c3140",
    "main:081c31b4",
    "main:081c330c",
    "main:081c3340",
    "main:081c33b0",
    "main:081c33c4",
    "main:081c33d8",
    "main:081c33ec",
    "main:081c33f8",
];

// The flash library, built with agbcc at -O rather than -O2: under -O2 its
// drafts differ in allocation and scheduling throughout, under -O the twelve
// drafted flash routines (erase, program, read, verify, the Atmel variants)
// reproduce their bytes exactly. IdentifyFlash (0x08006910) is exact under -O
// too once its wait-state update is one expression; under -O2 its loop
// allocates the table cursor and the loaded entry to swapped registers.
pub static AGBCC_FLASH_SOURCES: &[&str] = &[
    "main:08006878",
    "main:08006910",
    "main:080069a4",
    "main:08006a00",
    "main:08006af8",
    "main:08006b84",
    "main:08006ba8",
    "main:08006c24",
    "main:08006c68",
    "main:08006cdc",
    "main:08006d50",
    "main:08006dec",
    "main:08006e24",
    "main:08006f84",
    "main:08007028",
    "main:08007098",
    "main:0800711c",
    "main:080071a8",
    "main:08007220",
];
