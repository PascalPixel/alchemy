// Native compiler routing tables.
//
// These are hand-maintained evidence ledgers. Their order and documented
// duplicate entries are intentional because the resulting command-line order
// is part of byte-exact output.
//
// PORT NOTE (reachability, measured 2026-08-07). A prior audit claimed EIGHT of
// these tables are unreachable by `evidencedRoutingFlags`, the flag-capability
// probe. Re-measured against the live probe, only THREE are:
//
//   OPTIMIZE_O3_OVERLAY_SOURCES               keyed by sourceKey; its one path
//                                             is in none of the sets the probe
//                                             enumerates, and `-O3` never shows
//                                             up in the probe's output.
//   NO_INTERWORK_SOURCES                      keyed by bare stem, but every
//                                             member is an 0200xxxx overlay
//                                             address while the probe's sweep
//                                             covers 08000000..080fffff only.
//                                             It also SUBTRACTS `-mthumb-interwork`
//                                             rather than adding a flag, so it
//                                             could never be evidenced anyway.
//   SCHED_CALL_DEST_DESCENDING_OVERLAY_SOURCES  keyed by sourceKey; its overlay
//                                             paths are not enumerated. Main
//                                             image stems use the separate,
//                                             reachable table below.
//
// The other five the audit named ARE reachable, and the audit was wrong about
// them: NO_CONSTANT_REUSE_OVERLAY_SOURCES, GROUP_CONTROL_LAST_OVERLAY_SOURCES,
// ORR_DEAD_INPUT_REUSE_OVERLAY_SOURCES and CALL_LITERAL_ARG1_FIRST_OVERLAY_SOURCES
// all have members that the probe's explicit overlay-set enumeration visits, and
// SCHED_LOW_DEST_FIRST_SOURCES contains "08097540", which the address sweep hits
// directly. Nothing here is fixed on that account -- reachability is a property
// of the probe, not of the data, and the tables are mirrored as they stand.

pub static FIXED_R3_SOURCES: &[&str] = &[
    "080fb6ec", "080fb700", "080fb768",
    "080fb77c",
];
pub static OPTIMIZE_O1_SOURCES: &[&str] = &["080049e8", "08021e28"];
pub static OPTIMIZE_O3_OVERLAY_SOURCES: &[&str] = &["exact/resource_3a7_c_02000944.c"];
pub static NO_INTERWORK_SOURCES: &[&str] = &[
    "0200142c", "0200143c", "0200144c", "02001564", "02001750", "02001760", "02005ac0", "02005ad0",
    "02005bd8", "02005be8", "02005bf8", "02005dd4", "02005de4", "02005df4",
];
pub static NO_INTERWORK_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3a7_c_02001770.c",
    "exact/resource_3bf_c_02005e04.c",
    "exact/resource_3a7_c_020015a4.c",
    "exact/resource_3bf_c_02005c38.c",
    "exact/resource_3a7_c_020013ac.c",
    "exact/resource_3a7_c_020013e4.c",
    "exact/resource_3a7_c_02001554.c",
    "exact/resource_3a7_c_02001740.c",
    "exact/resource_3bf_c_02005ae0.c",
    "exact/resource_3a7_c_02001544.c",
    "exact/resource_3bf_c_02005a40.c",
    "exact/resource_3bf_c_02005a78.c",
    "exact/resource_3bf_c_02005c08.c",
    "exact/resource_3bf_c_02005af0.c",
    "exact/resource_3bf_c_02005b6c.c",
    "exact/resource_3a7_c_020014d8.c",
    "exact/resource_3a7_c_0200145c.c",
    "exact/resource_3a7_c_02001574.c",
];
pub static UNSCHEDULED_SOURCES: &[&str] = &[
    "08006b84", "080fb768", "080fb77c",
];
pub static UNSCHEDULED_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_39f_c_02000ab0.c",
    "exact/resource_3b2_c_02000ab0.c",
    "exact/resource_3c4_c_02000ab0.c",
    "exact/resource_3c5_c_02000ab0.c",
    "exact/resource_3cb_c_02001050.c",
];
pub static NO_CSE_FOLLOW_SOURCES: &[&str] = &["0800f9f4", "08019d2c", "080bbb0c"];
pub static NO_GCSE_SOURCES: &[&str] = &[
    "080981b0", "08098c08", "080a45cc", "080b2720", "080b3284", "080d40ec", "08004144",
];
pub static NO_EXPENSIVE_SOURCES: &[&str] = &["08092878", "08004144"];
pub static NO_STRENGTH_REDUCE_SOURCES: &[&str] = &["080a9d3c", "02004058", "02005d68"];
pub static NO_REGMOVE_SOURCES: &[&str] = &["08006088", "080a3d9c", "080ba918"];
pub static NO_OPTIMIZE_SIBLING_CALLS_SOURCES: &[&str] = &["080b110c"];
pub static OPTIMIZE_O1_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3ab_c_020007f4.c",
];
pub static NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES: &[&str] = &["exact/resource_383_c_0200082c.c",
        ];
pub static NO_CSE_SKIP_BLOCKS_OVERLAY_SOURCES: &[&str] = &["exact/resource_3b1_c_02000cc8.c", "exact/resource_3b1_c_02000b84.c", 
    "exact/resource_372_c_02000278.c",
    "exact/resource_398_c_02000304.c",
];
pub static FIXED_R7_OVERLAY_SOURCES: &[&str] = &["exact/resource_372_c_02000ec4.c"];
pub static NO_STRICT_ALIASING_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_373_c_02000608.c",
    "exact/resource_389_c_02000608.c",
    "exact/resource_391_c_02000608.c",
    "exact/resource_392_c_02000608.c",
    "exact/resource_393_c_02000608.c",
    "exact/resource_380_c_02000104.c",
    "exact/resource_39c_c_02000104.c",
    "exact/resource_39e_c_02000104.c",
    "exact/resource_39b_c_02001c20.c",
    "exact/resource_39c_c_020003a8.c",
    "exact/resource_3c9_c_02000104.c",
    "exact/resource_3c9_c_0200215c.c",
    "exact/resource_3c9_c_020021ac.c",
    "exact/resource_3c9_c_02003600.c",
    "exact/resource_397_c_020002a0.c",
    "exact/resource_39e_c_02003e58.c",
];
pub static NO_THREAD_JUMPS_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3c4_c_02001aba.c",
];
pub static NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3ad_c_02000460.c",
    "exact/resource_3b3_c_02002384.c",
    "exact/resource_3b4_c_02001308.c",
    "exact/resource_3ad_c_020001b0.c",
    "exact/resource_38c_c_0200035c.c",
    "exact/resource_3a5_c_02000498.c",
    "exact/resource_37a_c_02000054.c",
    "exact/resource_37a_c_02000108.c",
    "exact/resource_37a_c_02000150.c",
    "exact/resource_37a_c_020001ec.c",
    "exact/resource_37a_c_02001a58.c",
    "exact/resource_373_c_02000ba8.c",
    "exact/resource_373_c_02005950.c",
    "exact/resource_373_c_02005a40.c",
    "exact/resource_399_c_02000abc.c",
    "exact/resource_3b8_c_0200049c.c",
    "exact/resource_3a4_c_020009ec.c",
    "exact/resource_3a4_c_02000a94.c",
    "exact/resource_3a4_c_02000b3c.c",
    "exact/resource_3a4_c_02000bd8.c",
    "exact/resource_39c_c_02001c9c.c",
    "exact/resource_39c_c_02001d3c.c",
    "exact/resource_3ba_c_02000540.c",
    "exact/resource_3a7_c_020003e0.c",
    "exact/resource_3cd_c_020000c0.c",
    "exact/resource_3b4_c_02001070.c",
    "exact/resource_37f_c_0200056c.c",
    "exact/resource_37f_c_020005ac.c",
    "exact/resource_37f_c_020005ec.c",
    "exact/resource_37f_c_02000634.c",
    "exact/resource_37f_c_0200067c.c",
    "exact/resource_37f_c_020006c4.c",
    "exact/resource_37f_c_0200070c.c",
    "exact/resource_37f_c_02000754.c",
    "exact/resource_38d_c_020001b4.c",
    "exact/resource_38d_c_0200028c.c",
    "exact/resource_3a7_c_02000368.c",
    "exact/resource_3a7_c_020004d0.c",
    "exact/resource_3a7_c_0200048c.c",
];
pub static NO_EXPENSIVE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_370_c_02000384.c",
    "exact/resource_3b2_c_020012b4.c",
    "exact/resource_398_c_02000214.c",
    "exact/resource_398_c_02000538.c",
    "exact/resource_3a2_c_02000924.c",
];
pub static NO_GCSE_OVERLAY_SOURCES: &[&str] = &[
    "semantic/resource_3c2_c_0200006c.c",
    "exact/resource_3c2_c_0200006c.c",
    "semantic/resource_3c7_c_020000c8.c",
    "exact/resource_3c7_c_020000c8.c",
    "exact/resource_375_c_02000be0.c",
    "exact/resource_38f_c_020008ec.c",
    "exact/resource_391_c_02000d3c.c",
    "exact/resource_3cd_c_020000c0.c",
    ];
pub static AGBCC_SOURCES: &[&str] = &["080fb73c", "080fb728", "080fb714", "080fb75c", "080fb750", 
    "08006878", "080069a4", "08006a00", "08006a78", "08006af8", "08006ba8", "08006c24", "08006c68",
    "08006cdc", "08006d50", "08006dec", "08006e24", "08006f48", "08006f84", "08007028", "08007098",
    "0800711c", "080071a8", "08007220", "080f9a50", "080fada0", "080fadf0", "080fa1fc", "080fa2a0",
    "080fa324", "080fa350", "080fa39c", "080fa3f0", "080fa424", "080fa458", "080fa490", "080fa514",
    "080fa55c", "080fa6a0", "080fa83c", "080fa8d4", "080fa928", "080fa9a4", "080fa9e0", "080fab3c",
    "080fab7c", "080fac44", "080facf8", "080fb2cc", "080fb334", "080fb3a8", "080fb430", "080fb4a4",
    "080fb670", "080fb6a4", "08006910", "080fa280", "080fa4cc", "080fa798", "080faa58", "080fae58",
    "080fb2a4", "080fb410", "080fb518",
];
pub static AGBCC_LITERAL_BEFORE_SHIFT_SOURCES: &[&str] = &["080fb670"];
pub static AGBCC_OPTIMIZE_O1_SOURCES: &[&str] = &[
    "08006878", "08006a00", "08006ba8", "08006c68", "08006cdc", "08006d50", "08006e24", "08006f84",
    "08007028", "08007098", "0800711c", "080071a8", "08007220", "080fa514",
];
pub static AGBCC_NO_EXPENSIVE_SOURCES: &[&str] = &["08006910", "08006a78"];
pub static AGBCC_NO_GCSE_SOURCES: &[&str] = &["080faa58"];
pub static AGBCC_NO_REGMOVE_SOURCES: &[&str] = &["08006910"];
pub static AGBCC_COMPARE_ONLY_AND_TST_SOURCES: &[&str] = &["080f9a50"];
pub static AGBCC_COMMUTATIVE_COPY_CONSTANT_SOURCES: &[&str] = &["080fa514"];
pub static AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES: &[&str] = &["080fb2cc", "080fb334", "080fb3a8"];
pub static AGBCC_TRACK_NARROW_VALUE_R1_SOURCES: &[&str] = &["080fa280", "080fa4cc"];

pub static NO_RERUN_CSE_AFTER_LOOP_SOURCES: &[&str] =
    &["08006088", "0808c30c", "080ba918"];

pub static OPTIMIZE_OS_SOURCES: &[&str] = &["08019d2c"];

pub static SCHED2_OFF_THUMB_SOURCES: &[&str] = &["08029274", "08011590", "080060e8", "080043e0", "0800439c", "08004358", "0800430c", "080042c8", "08004198", "0800307c", "08002f10", "080f9a30"];
