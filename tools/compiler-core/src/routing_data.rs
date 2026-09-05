// Compiler-family membership. These tables say which compiler and library
// produced a source's bytes; they never carry a per-file flag. Every member
// of a family compiles with that family's single flag set (routing.rs), and a
// source that is not exact under its family's flags is not exact.

// The soft-float support leaves: prebuilt toolchain library code with the
// stock ABI (r4 callee-saved) and no interworking, linked into the two
// overlays that carry a copy. Members are named by their overlay routing
// path: a bare address would also claim the unrelated function another
// overlay links at the same address.
pub static SOFT_FLOAT_LIBRARY_OVERLAY_SOURCES: &[&str] = &[
    "games/gs1/src/resource_3a7_c_0200142c.c",
    "games/gs1/src/resource_3a7_c_0200143c.c",
    "games/gs1/src/resource_3a7_c_0200144c.c",
    "games/gs1/src/resource_3a7_c_02001564.c",
    "games/gs1/src/resource_3a7_c_02001750.c",
    "games/gs1/src/resource_3a7_c_02001760.c",
    "games/gs1/src/resource_3bf_c_02005ac0.c",
    "games/gs1/src/resource_3bf_c_02005ad0.c",
    "games/gs1/src/resource_3bf_c_02005bd8.c",
    "games/gs1/src/resource_3bf_c_02005be8.c",
    "games/gs1/src/resource_3bf_c_02005bf8.c",
    "games/gs1/src/resource_3bf_c_02005dd4.c",
    "games/gs1/src/resource_3bf_c_02005de4.c",
    "games/gs1/src/resource_3bf_c_02005df4.c",
    "games/gs1/src/resource_3a7_c_02001770.c",
    "games/gs1/src/resource_3bf_c_02005e04.c",
    "games/gs1/src/resource_3a7_c_020015a4.c",
    "games/gs1/src/resource_3bf_c_02005c38.c",
    "games/gs1/src/resource_3a7_c_020013ac.c",
    "games/gs1/src/resource_3a7_c_020013e4.c",
    "games/gs1/src/resource_3a7_c_02001554.c",
    "games/gs1/src/resource_3a7_c_02001740.c",
    "games/gs1/src/resource_3bf_c_02005ae0.c",
    "games/gs1/src/resource_3a7_c_02001544.c",
    "games/gs1/src/resource_3bf_c_02005a40.c",
    "games/gs1/src/resource_3bf_c_02005a78.c",
    "games/gs1/src/resource_3bf_c_02005c08.c",
    "games/gs1/src/resource_3bf_c_02005af0.c",
    "games/gs1/src/resource_3bf_c_02005b6c.c",
    "games/gs1/src/resource_3a7_c_020014d8.c",
    "games/gs1/src/resource_3a7_c_0200145c.c",
    "games/gs1/src/resource_3a7_c_02001574.c",
    "games/gs1/src/resource_3a7_c_02001158.c",
    "games/gs1/src/resource_3bf_c_020057ec.c",
];
// Library code built with agbcc.
pub static AGBCC_SOURCES: &[&str] = &[
    "080fb73c", "080fb728", "080fb714", "080fb75c", "080fb750", "08006878", "080069a4", "08006a00",
    "08006a78", "08006af8", "08006ba8", "08006c24", "08006c68", "08006cdc", "08006d50", "08006dec",
    "08006e24", "08006f48", "08006f84", "08007028", "08007098", "0800711c", "080071a8", "08007220",
    "080f9a30", "080f9a50", "080fada0", "080fadf0", "080fa1fc", "080fa264", "080fa2a0", "080fa324",
    "080fa350", "080fa39c", "080fa3f0", "080fa424", "080fa458", "080fa490", "080fa514", "080fa55c",
    "080fa6a0", "080fa83c", "080fa8d4", "080fa928", "080fa9a4", "080fa9e0", "080fab3c", "080fab7c",
    "080fac44", "080facf8", "080fb2cc", "080fb334", "080fb3a8", "080fb430", "080fb4a4", "080fb670",
    "080fb6a4", "08006910", "080fa280", "080fa4cc", "080fa798", "080faa58", "080fae58", "080fb2a4",
    "080fb410", "080fb518",
];
pub static GS2_AGBCC_SOURCES: &[&str] = &[
    "081c2168", "081c2640", "081c28e0", "081c2a3c", "081c2a8c", "081c2f68", "081c2fd0", "081c3044",
    "081c3340",
];
