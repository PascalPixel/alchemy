// Ordered evidence ledgers: flag order and intentional duplicates affect bytes.
// The 2026-08-07 reachability audit found only O3 overlays, bare-stem overlay
// no-interwork entries, and descending-call overlays absent from its probe.
// The constant-reuse, group-control, dead-input, literal-argument, and low-dest
// tables were reachable; probe reachability never licenses changing this data.

pub static FIXED_R3_SOURCES: &[&str] = &["080fb6ec", "080fb700", "080fb768", "080fb77c"];
// 080a8904's register-only delay loop is exact at -O1; -O2 removes it, while
// volatile C adds memory traffic and changes the delay.
pub static OPTIMIZE_O1_SOURCES: &[&str] = &["080049e8", "08021e28", "080a8904"];
pub static OPTIMIZE_O3_OVERLAY_SOURCES: &[&str] = &["games/gs1/src/resource_3a7_c_02000944.c"];
pub static NO_INTERWORK_SOURCES: &[&str] = &[
    "0200142c", "0200143c", "0200144c", "02001564", "02001750", "02001760", "02005ac0", "02005ad0",
    "02005bd8", "02005be8", "02005bf8", "02005dd4", "02005de4", "02005df4",
];
pub static NO_INTERWORK_OVERLAY_SOURCES: &[&str] = &[
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
// Soft-float library leaves keep stock-ABI r4 callee-saved. Their residual was
// the r4 push/pop bit; the larger add-parts owner confirms the boundary. The
// already-exact 3a7:1544/1554/1740 and 3bf:5ae0 deliberately stay on base flags.
pub static CALLEE_SAVED_R4_OVERLAY_SOURCES: &[&str] = &[
    "games/gs1/src/resource_3a7_c_0200145c.c",
    "games/gs1/src/resource_3a7_c_02001574.c",
    "games/gs1/src/resource_3a7_c_02001158.c",
    "games/gs1/src/resource_3bf_c_020057ec.c",
    "games/gs1/src/resource_3bf_c_02005af0.c",
    "games/gs1/src/resource_3bf_c_02005c08.c",
    "games/gs1/src/resource_3a7_c_020013ac.c",
    "games/gs1/src/resource_3a7_c_020013e4.c",
    "games/gs1/src/resource_3a7_c_020014d8.c",
    "games/gs1/src/resource_3a7_c_020015a4.c",
    "games/gs1/src/resource_3a7_c_02001770.c",
    "games/gs1/src/resource_3bf_c_02005a40.c",
    "games/gs1/src/resource_3bf_c_02005a78.c",
    "games/gs1/src/resource_3bf_c_02005b6c.c",
    "games/gs1/src/resource_3bf_c_02005c38.c",
    "games/gs1/src/resource_3bf_c_02005e04.c",
];
// Rejected r4 lookalikes: 3ad:11b8, 3ae:02dc, 3ca:004c/00b0, 3cb:0128; the flag
// closes none and worsens 3ad:11b8 from two differing halfwords to fifteen.
// 08006b84 is the corresponding main-image soft-library ABI case.
pub static CALLEE_SAVED_R4_SOURCES: &[&str] = &["08006b84"];
// 0801c34c reloads through a differently-typed pointer that strict aliasing
// lets the compiler keep live.
pub static NO_STRICT_ALIASING_SOURCES: &[&str] = &["0801c34c"];
pub static UNSCHEDULED_SOURCES: &[&str] = &["08006b84", "080fb768", "080fb77c"];
pub static UNSCHEDULED_OVERLAY_SOURCES: &[&str] = &[
    "games/gs1/src/resource_39f_c_02000ab0.c",
    "games/gs1/src/resource_3b2_c_02000ab0.c",
    "games/gs1/src/resource_3c4_c_02000ab0.c",
    "games/gs1/src/resource_3c5_c_02000ab0.c",
    "games/gs1/src/resource_3cb_c_02001050.c",
];
pub static NO_CSE_FOLLOW_SOURCES: &[&str] = &["0800f9f4", "08019d2c", "080b2b0c", "080bbb0c"];
pub static NO_GCSE_SOURCES: &[&str] = &[
    "080981b0", "08098c08", "080a45cc", "080b2720", "080b3284", "080d40ec",
];
pub static NO_EXPENSIVE_SOURCES: &[&str] = &["08092878"];
// 0800ebec: its four register-only delay loops survive only without strength
// reduction; -O1 scores worse (843) than -O2 -fno-strength-reduce (814).
pub static NO_STRENGTH_REDUCE_SOURCES: &[&str] = &["080a9d3c", "02004058", "02005d68", "0800ebec"];
// 0808b8e8 keeps the camera anchor live through both X bounds and reuses the
// dead visual pointer for its null slot store.  Stock regmove undoes those two
// source lifetimes; the shipped per-file disable preserves the exact schedule.
pub static NO_REGMOVE_SOURCES: &[&str] = &["08006088", "0808b8e8", "080a3d9c", "080ba918"];
pub static NO_OPTIMIZE_SIBLING_CALLS_SOURCES: &[&str] = &["080b110c"];
pub static OPTIMIZE_O1_OVERLAY_SOURCES: &[&str] = &["games/gs1/src/resource_3ab_c_020007f4.c"];
pub static NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES: &[&str] =
    &["games/gs1/src/resource_383_c_0200082c.c"];
pub static NO_CSE_SKIP_BLOCKS_OVERLAY_SOURCES: &[&str] = &[
    "games/gs1/src/resource_3b1_c_02000cc8.c",
    "games/gs1/src/resource_3b1_c_02000b84.c",
    "games/gs1/src/resource_372_c_02000278.c",
    "games/gs1/src/resource_398_c_02000304.c",
];
pub static FIXED_R7_OVERLAY_SOURCES: &[&str] = &["games/gs1/src/resource_372_c_02000ec4.c"];
pub static NO_STRICT_ALIASING_OVERLAY_SOURCES: &[&str] = &[
    // Reference reloads through a differently-typed pointer where strict
    // aliasing lets the rerun keep the first load live.
    "games/gs1/src/resource_381_c_02002e0c.c",
    "games/gs1/src/resource_381_c_02002e5c.c",
    "games/gs1/src/resource_3b0_c_02000030.c",
    "games/gs1/src/resource_373_c_02000608.c",
    "games/gs1/src/resource_389_c_02000608.c",
    "games/gs1/src/resource_39b_c_020008ec.c",
    "games/gs1/src/resource_39c_c_020008ec.c",
    "games/gs1/src/resource_3a6_c_020008ec.c",
    "games/gs1/src/resource_3b3_c_020008ec.c",
    "games/gs1/src/resource_3be_c_020008ec.c",
    "games/gs1/src/resource_39f_c_02000608.c",
    "games/gs1/src/resource_3b2_c_02000608.c",
    "games/gs1/src/resource_3b4_c_02000608.c",
    "games/gs1/src/resource_3bd_c_02000608.c",
    "games/gs1/src/resource_3bf_c_02000608.c",
    "games/gs1/src/resource_3c4_c_02000608.c",
    "games/gs1/src/resource_3c5_c_02000608.c",
    "games/gs1/src/resource_391_c_02000608.c",
    "games/gs1/src/resource_392_c_02000608.c",
    "games/gs1/src/resource_393_c_02000608.c",
    "games/gs1/src/resource_380_c_02000104.c",
    "games/gs1/src/resource_382_c_02000104.c",
    "games/gs1/src/resource_385_c_02000104.c",
    "games/gs1/src/resource_387_c_02000104.c",
    "games/gs1/src/resource_38a_c_02000104.c",
    "games/gs1/src/resource_396_c_02000104.c",
    "games/gs1/src/resource_39b_c_02000104.c",
    "games/gs1/src/resource_39c_c_02000104.c",
    "games/gs1/src/resource_39e_c_02000104.c",
    "games/gs1/src/resource_3a0_c_02000104.c",
    "games/gs1/src/resource_3a5_c_02000104.c",
    "games/gs1/src/resource_3a6_c_02000104.c",
    "games/gs1/src/resource_3ab_c_02000104.c",
    "games/gs1/src/resource_3b3_c_02000104.c",
    "games/gs1/src/resource_3be_c_02000104.c",
    "games/gs1/src/resource_3c0_c_02000104.c",
    "games/gs1/src/resource_39b_c_02001c20.c",
    "games/gs1/src/resource_39c_c_020003a8.c",
    "games/gs1/src/resource_3c9_c_02000104.c",
    "games/gs1/src/resource_3c9_c_0200215c.c",
    "games/gs1/src/resource_3c9_c_020021ac.c",
    "games/gs1/src/resource_3c9_c_02003600.c",
    "games/gs1/src/resource_397_c_020002a0.c",
    "games/gs1/src/resource_39e_c_02003e58.c",
];
pub static NO_THREAD_JUMPS_OVERLAY_SOURCES: &[&str] = &["games/gs1/src/resource_3c4_c_02001aba.c"];
pub static NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES: &[&str] = &[
    // Reference rematerializes a two-insn constant; rerun-CSE keeps a register.
    // This flag worsens 12 unlisted 3bf owners but closes every listed owner.
    // It is per-file only; `overlay audit --all` is the standing exactness proof.
    "games/gs1/src/resource_39c_c_02001568.c",
    "games/gs1/src/resource_3a8_c_020015b4.c",
    "games/gs1/src/resource_3a8_c_0200158c.c",
    "games/gs1/src/resource_3bf_c_0200169c.c",
    "games/gs1/src/resource_3cb_c_020002d8.c",
    "games/gs1/src/resource_374_c_02000634.c",
    "games/gs1/src/resource_3ad_c_0200033c.c",
    "games/gs1/src/resource_3b4_c_02001cf8.c",
    "games/gs1/src/resource_3b8_c_02003f84.c",
    "games/gs1/src/resource_3bf_c_0200175c.c",
    "games/gs1/src/resource_3bf_c_020017bc.c",
    "games/gs1/src/resource_3bf_c_02002308.c",
    "games/gs1/src/resource_3bf_c_0200238c.c",
    "games/gs1/src/resource_3bf_c_02004bfc.c",
    "games/gs1/src/resource_3ad_c_02000460.c",
    "games/gs1/src/resource_3b3_c_02002384.c",
    "games/gs1/src/resource_3b4_c_02001308.c",
    "games/gs1/src/resource_3ad_c_020001b0.c",
    "games/gs1/src/resource_38c_c_0200035c.c",
    "games/gs1/src/resource_3a5_c_02000498.c",
    "games/gs1/src/resource_37a_c_02000054.c",
    "games/gs1/src/resource_37a_c_02000108.c",
    "games/gs1/src/resource_37a_c_02000150.c",
    "games/gs1/src/resource_37a_c_020001ec.c",
    "games/gs1/src/resource_37a_c_02001a58.c",
    "games/gs1/src/resource_373_c_02000ba8.c",
    "games/gs1/src/resource_373_c_02005950.c",
    "games/gs1/src/resource_373_c_02005a40.c",
    "games/gs1/src/resource_399_c_02000abc.c",
    "games/gs1/src/resource_3b8_c_0200049c.c",
    "games/gs1/src/resource_3a4_c_020009ec.c",
    "games/gs1/src/resource_3a4_c_02000a94.c",
    "games/gs1/src/resource_3a4_c_02000b3c.c",
    "games/gs1/src/resource_3a4_c_02000bd8.c",
    "games/gs1/src/resource_39c_c_02001c9c.c",
    "games/gs1/src/resource_39c_c_02001d3c.c",
    "games/gs1/src/resource_3ba_c_02000540.c",
    "games/gs1/src/resource_3a7_c_020003e0.c",
    "games/gs1/src/resource_3cd_c_020000c0.c",
    "games/gs1/src/resource_3b4_c_02001070.c",
    "games/gs1/src/resource_37f_c_0200056c.c",
    "games/gs1/src/resource_37f_c_020005ac.c",
    "games/gs1/src/resource_37f_c_020005ec.c",
    "games/gs1/src/resource_37f_c_02000634.c",
    "games/gs1/src/resource_37f_c_0200067c.c",
    "games/gs1/src/resource_37f_c_020006c4.c",
    "games/gs1/src/resource_37f_c_0200070c.c",
    "games/gs1/src/resource_37f_c_02000754.c",
    "games/gs1/src/resource_38d_c_020001b4.c",
    "games/gs1/src/resource_38d_c_0200028c.c",
    "games/gs1/src/resource_3a7_c_02000368.c",
    "games/gs1/src/resource_3a7_c_020004d0.c",
    "games/gs1/src/resource_3a7_c_0200048c.c",
];
pub static NO_EXPENSIVE_OVERLAY_SOURCES: &[&str] = &[
    "games/gs1/src/resource_370_c_02000384.c",
    "games/gs1/src/resource_3b2_c_020012b4.c",
    "games/gs1/src/resource_398_c_02000214.c",
    "games/gs1/src/resource_398_c_02000538.c",
    "games/gs1/src/resource_3a2_c_02000924.c",
];
pub static NO_GCSE_OVERLAY_SOURCES: &[&str] = &[
    "games/gs1/recon/en/overlays/resource_3c2_c_0200006c.c",
    "games/gs1/src/resource_3c2_c_0200006c.c",
    "games/gs1/recon/en/overlays/resource_3c7_c_020000c8.c",
    "games/gs1/src/resource_3c7_c_020000c8.c",
    "games/gs1/src/resource_375_c_02000be0.c",
    "games/gs1/src/resource_38f_c_020008ec.c",
    "games/gs1/src/resource_391_c_02000d3c.c",
    "games/gs1/src/resource_3cd_c_020000c0.c",
];
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
pub static GS2_AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES: &[&str] =
    &["081c2f68", "081c2fd0", "081c3044"];
pub static AGBCC_TRACK_NARROW_VALUE_R1_SOURCES: &[&str] = &["080fa280", "080fa4cc"];
pub static GS2_AGBCC_TRACK_NARROW_VALUE_R1_SOURCES: &[&str] = &["081c2168"];

pub static NO_RERUN_CSE_AFTER_LOOP_SOURCES: &[&str] = &["08006088", "0808c30c", "080ba918"];

pub static OPTIMIZE_OS_SOURCES: &[&str] = &["08019d2c"];

pub static SCHED2_OFF_THUMB_SOURCES: &[&str] = &[
    "08029274", "08011590", "0800307c", "08002f10", "08078320", "080f9a30",
];
