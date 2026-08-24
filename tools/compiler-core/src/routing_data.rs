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

pub static FIXED_R3_SOURCES: &[&str] = &["080fb6ec", "080fb700", "080fb768", "080fb77c"];
pub static OPTIMIZE_O1_SOURCES: &[&str] = &["080049e8", "08021e28"];
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
// Soft-float library leaves that keep r4 CALLEE-SAVED, i.e. the stock ARM ABI
// without `-fcall-used-r4`.
//
// The base flag set hands r4 to the allocator because Camelot's own code was
// built that way. These entries are compiler soft-float support routines and
// keep the stock ABI. For the original leaf set, the entire residual was the
// r4 bit in otherwise identical `push` and `pop` register lists; the larger
// add-parts owner independently confirms the same library boundary.
//
// Same shape of routing as `NO_INTERWORK_OVERLAY_SOURCES` above, which
// subtracts `-mthumb-interwork` from this same family, and the same reasoning
// pret uses when it routes `old_agbcc` at `m4a.o` and `libc.o`: a library
// object is not game code and does not take the game's ABI. Stock option,
// subtracted per file, recorded as routing debt.
// The four members already byte-exact WITHOUT this subtraction -- 3a7:1544,
// 3a7:1554, 3a7:1740 and 3bf:5ae0 -- are deliberately absent. They reproduce
// on the base set, so routing them here would change flags under a settled
// owner to no purpose.
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
// Tried and rejected: 3ad:11b8, 3ae:02dc, 3ca:004c, 3ca:00b0 and 3cb:0128 also
// show r4 in the reference prologue, but subtracting the flag does not close
// any of them and moves 3ad:11b8 from two differing halfwords to fifteen. A
// saved r4 alone does not make an owner part of this family.
// Main-image counterparts of the two overlay tables below/above, keyed by bare
// stem the way every other main-image table is.
//
// 08006b84 is the same soft-library ABI case as the overlay r4 family: its
// reference prologue saves r4 and ours does not.
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
    "080981b0", "08098c08", "080a45cc", "080b2720", "080b3284", "080d40ec", "08004144",
];
pub static NO_EXPENSIVE_SOURCES: &[&str] = &["08092878", "08004144"];
pub static NO_STRENGTH_REDUCE_SOURCES: &[&str] = &["080a9d3c", "02004058", "02005d68"];
pub static NO_REGMOVE_SOURCES: &[&str] = &["08006088", "080a3d9c", "080ba918"];
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
    // The rematerialisation shape: the reference rebuilds a two-instruction
    // constant at each use, while the rerun of cse after loop commons the two
    // into one callee-saved register and pays for it with an extra register in
    // the prologue and a copy at each use. The tell is a `push` that saves one
    // more register than the reference's.
    //
    // DO NOT strip this table on the strength of bef5cad7c's "closed to flag
    // sweeps". That commit is right that the flag cannot be applied BROADLY --
    // measured again here, 12 adopted resource_3bf owners that are NOT in this
    // table go 2 worse and 0 better under it, which is the 7-against-6 it
    // reports. But routing is per FILE, and every entry below is an owner the
    // flag closes outright; the owners it would regress are simply not listed.
    // `overlay audit --all` is the standing proof: 0 findings with all of these
    // routed and adopted. Blanket application is refused; per-owner is what a
    // routing table is for.
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
    "080f9a50", "080fada0", "080fadf0", "080fa1fc", "080fa264", "080fa2a0", "080fa324", "080fa350",
    "080fa39c", "080fa3f0", "080fa424", "080fa458", "080fa490", "080fa514", "080fa55c", "080fa6a0",
    "080fa83c", "080fa8d4", "080fa928", "080fa9a4", "080fa9e0", "080fab3c", "080fab7c", "080fac44",
    "080facf8", "080fb2cc", "080fb334", "080fb3a8", "080fb430", "080fb4a4", "080fb670", "080fb6a4",
    "08006910", "080fa280", "080fa4cc", "080fa798", "080faa58", "080fae58", "080fb2a4", "080fb410",
    "080fb518",
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

pub static NO_RERUN_CSE_AFTER_LOOP_SOURCES: &[&str] = &["08006088", "0808c30c", "080ba918"];

pub static OPTIMIZE_OS_SOURCES: &[&str] = &["08019d2c"];

pub static SCHED2_OFF_THUMB_SOURCES: &[&str] = &[
    "08029274", "08011590", "080060e8", "080043e0", "0800439c", "08004358", "0800430c", "080042c8",
    "08004198", "0800307c", "08002f10", "080f9a30",
];
