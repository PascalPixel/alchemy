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
//   SCHED_CALL_DEST_DESCENDING_OVERLAY_SOURCES  keyed by sourceKey; its one path
//                                             is not enumerated, and
//                                             `-fsched-call-dest-descending` is
//                                             absent from the probe's output.
//
// The other five the audit named ARE reachable, and the audit was wrong about
// them: NO_CONSTANT_REUSE_OVERLAY_SOURCES, GROUP_CONTROL_LAST_OVERLAY_SOURCES,
// ORR_DEAD_INPUT_REUSE_OVERLAY_SOURCES and CALL_LITERAL_ARG1_FIRST_OVERLAY_SOURCES
// all have members that the probe's explicit overlay-set enumeration visits, and
// SCHED_LOW_DEST_FIRST_SOURCES contains "08097540", which the address sweep hits
// directly. Nothing here is fixed on that account -- reachability is a property
// of the probe, not of the data, and the tables are mirrored as they stand.

pub static FIXED_R3_SOURCES: &[&str] = &[
    "080fb6ec", "080fb700", "080fb714", "080fb728", "080fb73c", "080fb750", "080fb75c", "080fb768",
    "080fb77c",
];
pub static OPTIMIZE_O1_SOURCES: &[&str] = &["080049e8", "08021e28"];
pub static THUMB_0807A664_SOURCES: &[&str] = &["0807a664"];
pub static OPTIMIZE_OS_SOURCES: &[&str] = &["08019d2c"];
pub static OPTIMIZE_O3_OVERLAY_SOURCES: &[&str] = &["exact/resource_3a7_c_02000944.c"];
pub static NO_INTERWORK_SOURCES: &[&str] = &[
    "0200142c", "0200143c", "0200144c", "02001564", "02001750", "02001760", "02005ac0", "02005ad0",
    "02005bd8", "02005be8", "02005bf8", "02005dd4", "02005de4", "02005df4",
];
pub static NO_INTERWORK_OVERLAY_SOURCES: &[&str] = &[
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
    "semantic/resource_3bf_c_02005b6c.c",
    "exact/resource_3bf_c_02005b6c.c",
    "exact/resource_3a7_c_0200145c.c",
    "exact/resource_3a7_c_02001574.c",
];
pub static UNSCHEDULED_SOURCES: &[&str] = &[
    "08002f10", "0800307c", "08006b84", "080060e8", "08004198", "080042c8", "0800430c", "08004358",
    "0800439c", "080043e0", "08011590", "08029274", "080fb714", "080fb728", "080fb73c", "080fb750",
    "080fb75c", "080fb768", "080fb77c",
];
pub static UNSCHEDULED_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_39f_c_02000ab0.c",
    "exact/resource_3b2_c_02000ab0.c",
    "exact/resource_3c4_c_02000ab0.c",
    "exact/resource_3c5_c_02000ab0.c",
    "exact/resource_3cb_c_02001050.c",
    "semantic/resource_3cb_c_02001050.c",
];
pub static NO_CSE_FOLLOW_SOURCES: &[&str] = &["0800f9f4", "08019d2c"];
pub static NO_CSE_TWO_INSN_IMMEDIATE_SOURCES: &[&str] = &["080108c4"];
pub static GROUP_VALUE2_IN_PLACE_SOURCES: &[&str] = &["080b5ad4"];
pub static NO_RERUN_CSE_AFTER_LOOP_SOURCES: &[&str] =
    &["08006088", "0808c30c", "080ba918", "080044d0"];
pub static NO_RERUN_LOOP_OPT_SOURCES: &[&str] = &["0807a0f4"];
pub static NO_GCSE_SOURCES: &[&str] = &[
    "0801ed40", "080981b0", "08098c08", "080a45cc", "080b2720", "080b3284", "080d40ec",
];
pub static NO_EXPENSIVE_SOURCES: &[&str] = &["08092878"];
pub static NO_STRENGTH_REDUCE_SOURCES: &[&str] = &["080a9d3c"];
pub static NO_CONTIGUOUS_IMMEDIATE_SOURCES: &[&str] =
    &["080a1090", "08005a78", "0800d304", "08019bac"];
pub static SPLIT_GROUP_BASE_SOURCES: &[&str] = &["08005a78"];
pub static HOIST_PARAMETER_SAVE_SOURCES: &[&str] =
    &["08005340", "08005394", "080053e8", "08019bac"];
pub static MINIPOOL_TAIL_FIRST_SOURCES: &[&str] = &["080a9aec"];
pub static ENTRY_SAVES_DESCENDING_SOURCES: &[&str] = &["08093054"];
pub static GROUP_CONTROL_LAST_SOURCES: &[&str] = &[
    "080c08a8", "08005a78", "08005c68", "080907b0", "08090824", "080b010c", "0808fe38", "080b0744",
];
pub static GROUP_VALUE1_BEFORE_BASE_SOURCES: &[&str] = &["080907b0"];
pub static GROUP_POOLED_CONTROL_LAST_SOURCES: &[&str] = &["0801a4fc"];
pub static HIGH_MOVE_BEFORE_ALU_SOURCES: &[&str] = &["0808b868"];
pub static MOVE_BEFORE_IMMEDIATE_ALU_SOURCES: &[&str] = &["0801fd34"];
pub static LOOP_INVARIANT_BLOCK_HEAD_SOURCES: &[&str] = &["080a3354", "080b5d3c", "080a90bc"];
pub static CALL_ARG0_REG_SOURCE_SOURCES: &[&str] = &["080a90bc"];
pub static SINK_CONSTANT_PAST_CALL_SOURCES: &[&str] = &["080a90bc"];
pub static MOVE_BEFORE_UNARY_ALU_SOURCES: &[&str] = &["080a90bc"];
pub static GROUP_ZERO_ANY_REGISTER_SOURCES: &[&str] = &["0808fe38", "080c08a8"];
pub static ARG0_AFTER_SPLIT_SOURCES: &[&str] = &["0808fe38"];
pub static CALL_ARG0_POOL_LOAD_SOURCES: &[&str] = &["0808fe38"];
pub static RETURN_VALUE_BEFORE_STACK_ADJUST_SOURCES: &[&str] = &["0808fecc"];
pub static SINK_GROUP_POOL_LOADS_SOURCES: &[&str] = &["080c08a8"];
pub static SINK_STACK_ADJUST_SOURCES: &[&str] = &["080c08a8"];
pub static SINK_DEPENDENT_LOAD_SOURCES: &[&str] = &["080c08a8"];
pub static COLLAPSE_DEAD_SCRATCH_SOURCES: &[&str] = &["0800fec8"];
pub static SINK_BLOCK_CONSTANT_SOURCES: &[&str] = &["0800430c"];
pub static SINK_PAST_POOL_LOAD_SOURCES: &[&str] = &["0800430c"];
pub static SINK_STORE_PAST_STORE_SOURCES: &[&str] = &["08006408"];
pub static SINK_ADD_IMMEDIATE_SOURCES: &[&str] = &["080c0130"];
pub static HOIST_ADD_IMMEDIATE_SOURCES: &[&str] = &["08011568"];
pub static NO_CONSTANT_REUSE_SOURCES: &[&str] = &["080cd358"];
pub static NO_CONSTANT_REUSE_OVERLAY_SOURCES: &[&str] = &[
    "semantic/resource_373_c_02002f14.c",
    "exact/resource_373_c_02002f14.c",
];
pub static GROUP_CONTROL_LAST_OVERLAY_SOURCES: &[&str] = &["exact/resource_3bd_c_02000c98.c"];
pub static NO_SCHED_DEPEND_COUNT_SOURCES: &[&str] = &[
    "080b0744", "08002fb0", "08003e10", "08004760", "08005340", "08005394", "080053e8", "0800d304",
    "08019bac", "08021d88", "080903bc", "080907b0", "08094730",
];
pub static MOVE_BEFORE_ALU_SOURCES: &[&str] = &[
    "08002fb0", "08003e10", "08005340", "08005394", "080053e8", "0800d304", "08019bac",
];
pub static NO_REGMOVE_SOURCES: &[&str] = &["08006088", "080a3d9c", "080ba918"];
pub static NO_OPTIMIZE_SIBLING_CALLS_SOURCES: &[&str] = &["080b110c"];
// PORT NOTE: this table lists "080c08a8" twice. In the TypeScript the container
// is a `Set`, so the second occurrence is silently collapsed and the duplicate
// has no runtime effect; on this side the slice is searched with `contains`, so
// it has no effect here either. The duplicate is REPRODUCED, not fixed: `drift`
// compares the two declarations as ordered literal sequences, and deduplicating
// one side would fail the gate. Four other tables carry the same defect --
// NO_CSE_TWO_INSN_IMMEDIATE_OVERLAY_SOURCES, SCHED_LOW_DEST_FIRST_OVERLAY_SOURCES,
// NO_CSE_POOL_IMMEDIATE_OVERLAY_SOURCES -- each marked where it occurs. Fixing
// them is a deliberate edit to BOTH mirrors, not a cleanup to this one.
pub static GROUPED_DMA_STORE_SOURCES: &[&str] = &[
    "080b0744", "0808fe38", "080c08a8", "08005c68", "080060e8", "08002f10", "08004838", "08004858",
    "080049e8", "08004a28", "08004a44", "08004a5c", "08004a94", "08005340", "08005394", "080053e8",
    "0800bc48", "0800bdd4", "0800c0f4", "0800d304", "08011590", "080170c4", "08019bac", "0801d014",
    "0801d980", "080251d4", "080284dc", "080907b0", "08090824", "08094730", "08095160", "08095290",
    "080958a8", "08097540", "0809bb34", "080c0184", "080c08a8", "0808fecc", "08004760", "08005a78",
    "080037d4", "080b5ad4", "0800300c", "080f377c", "08091174", "08002fb0", "08003e10", "080a1090",
    "080b010c", "0801a4fc",
];
pub static ENTRY_LITERAL_FIRST_SOURCES: &[&str] = &[
    "0800383c", "0800387c", "080038bc", "080038fc", "0800393c", "0800397c", "080039bc", "080039fc",
    "08003a3c",
];
pub static HIGH_REGISTER_MOVE_FIRST_SOURCES: &[&str] =
    &["0808b8e8", "080b6e30", "08002fb0", "08003e10", "08019bac"];
pub static ORR_DEAD_INPUT_REUSE_SOURCES: &[&str] = &["08003adc"];
pub static ORR_DEAD_INPUT_REUSE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3a8_c_02003768.c",
    "semantic/resource_3a8_c_02003768.c",
    "exact/resource_3a6_c_02001938.c",
    "exact/resource_39a_c_02001004.c",
    "exact/resource_38f_c_020008ec.c",
    "semantic/resource_38f_c_020008ec.c",
];
pub static CALL_LITERAL_ARG1_FIRST_OVERLAY_SOURCES: &[&str] = &[
    "semantic/resource_3ae_c_020002dc.c",
    "exact/resource_3ae_c_020002dc.c",
    "semantic/resource_3ae_c_0200051c.c",
    "exact/resource_3ae_c_0200051c.c",
    "semantic/resource_377_c_020001e0.c",
    "exact/resource_377_c_020001e0.c",
    "semantic/resource_3a2_c_02000ac0.c",
    "exact/resource_3a2_c_02000ac0.c",
];
pub static CALL_ARG1_BEFORE_ARG0_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_38f_c_020008ec.c",
    "exact/resource_3c3_c_02000730.c",
    "exact/resource_3c0_c_02000adc.c",
    "semantic/resource_3c0_c_02000adc.c",
];
pub static FIXED_LR_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3ab_c_020007f4.c",
    "semantic/resource_3ab_c_020007f4.c",
    "exact/resource_385_c_02000a80.c",
    "semantic/resource_385_c_02000a80.c",
];
pub static OPTIMIZE_O1_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3ab_c_020007f4.c",
    "semantic/resource_3ab_c_020007f4.c",
];
pub static CALL_ARG0_BEFORE_STORE_SOURCES: &[&str] = &["08077f70"];
pub static POSTCALL_BYTE_INCREMENT_R2_SOURCES: &[&str] = &["08098b10"];
pub static GROUP_CONTROL_REMATERIALIZE_SOURCES: &[&str] = &["080f377c"];
pub static SCHED_POOL_LOAD_LATE_SOURCES: &[&str] = &["080f377c", "08091174"];
pub static THUMB_LEAF_NO_LR_SOURCES: &[&str] = &["080f9a30", "080fa1ac", "080fa264"];
pub static THUMB_NO_IF_CONVERT_SOURCES: &[&str] = &["080f9a30", "080fa1ac", "080fa264"];
pub static SCHED2_OFF_THUMB_SOURCES: &[&str] = &["080f9a30"];
pub static THUMB_LOW_REG_ORDER_SOURCES: &[(&str, &str)] =
    &[("080f9a30", "01231230"), ("080fa264", "30120123")];
pub static THUMB_CALLEE_REG_ORDER_SOURCES: &[(&str, &str)] =
    &[("08092f84", "0132"), ("0801faa8", "0132")];
pub static ENTRY_FRAME_CLUSTER_SOURCES: &[&str] = &["0801c34c"];
pub static LATE_FRAME_ALLOCATION_SOURCES: &[&str] = &["080b0744"];
pub static LITERAL_BEFORE_INDEX_SHIFT_SOURCES: &[&str] = &["0800307c"];
pub static LOW_CONSTANT_BEFORE_HIGH_MOVE_SOURCES: &[&str] = &["080a5614", "080ba918"];
pub static HIGH_MOVE_BEFORE_STACK_STORE_SOURCES: &[&str] = &["08095290"];
pub static EARLY_FRAME_ALLOCATION_SOURCES: &[&str] = &["0809802c", "08004760"];
pub static CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3bf_c_02001cf0.c",
    "exact/resource_3a0_c_02000048.c",
    "exact/resource_3a1_c_02000048.c",
    "exact/resource_3a5_c_02000048.c",
    "exact/resource_3a6_c_02000048.c",
    "exact/resource_3ab_c_02000048.c",
    "exact/resource_3b3_c_02000048.c",
    "exact/resource_3be_c_02000048.c",
    "exact/resource_3c0_c_02000048.c",
    "exact/resource_3c9_c_02000048.c",
    "exact/resource_380_c_020000a0.c",
    "exact/resource_382_c_020000a0.c",
    "exact/resource_385_c_020000a0.c",
    "exact/resource_387_c_020000a0.c",
    "exact/resource_38a_c_020000a0.c",
    "exact/resource_396_c_020000a0.c",
    "exact/resource_39b_c_020000a0.c",
    "exact/resource_39c_c_020000a0.c",
    "exact/resource_39d_c_020000a0.c",
    "exact/resource_39e_c_020000a0.c",
    "exact/resource_3a0_c_020000a0.c",
    "exact/resource_3a1_c_020000a0.c",
    "exact/resource_3a5_c_020000a0.c",
    "exact/resource_3a6_c_020000a0.c",
    "exact/resource_3ab_c_020000a0.c",
    "exact/resource_3b3_c_020000a0.c",
    "exact/resource_3be_c_020000a0.c",
    "exact/resource_3c0_c_020000a0.c",
    "exact/resource_3c9_c_020000a0.c",
];
pub static EARLY_LITERAL_POOL_OVERLAY_SOURCES: &[&str] = &["02000e3c", "02000dfc", "02003a44"];
pub static EARLY_LITERAL_POOL_OVERLAY_PATHS: &[&str] = &[
    "exact/resource_394_c_02000ee0.c",
    "exact/resource_3bd_c_02000ee0.c",
];
pub static NO_CANONICALIZE_COMPARISON_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3a9_c_0200007c.c",
    "exact/resource_3a9_c_020000e4.c",
];
pub static THUMB_IMMEDIATE_LATENCY_SOURCES: &[&str] = &["080babdc"];
pub static SCHED_HIGH_DEST_FIRST_SOURCES: &[&str] = &["08098954", "0809a294", "08097540"];
pub static SCHED_LOW_DEST_FIRST_SOURCES: &[&str] = &[
    "08097540", "02001958", "02000260", "02003dec", "02003e34", "02003d10", "020040e8", "020029dc",
];
pub static THUMB_IMMEDIATE_LATENCY_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3c6_c_02000078.c",
    "exact/resource_3b7_c_02000154.c",
    "exact/resource_3b7_c_02000178.c",
    "exact/resource_383_c_02000428.c",
    "exact/resource_3b1_c_02000670.c",
    "exact/resource_372_c_0200173c.c",
    "exact/resource_3bf_c_02004794.c",
    "exact/resource_37a_c_02001380.c",
    "exact/resource_37a_c_02001790.c",
    "exact/resource_37a_c_02002924.c",
    "exact/resource_372_c_020016cc.c",
    "exact/resource_399_c_020005dc.c",
    "exact/resource_399_c_02000a3c.c",
    "exact/resource_399_c_02000abc.c",
    "exact/resource_3c7_c_02000030.c",
    "exact/resource_3cd_c_0200004c.c",
    "exact/resource_3b8_c_0200049c.c",
    "exact/resource_3b9_c_02002668.c",
    "exact/resource_3b9_c_02002964.c",
];
pub static NO_SCHED_DEPEND_COUNT_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3cb_c_02000128.c",
    "exact/resource_38f_c_020026cc.c",
    "exact/resource_398_c_020004b4.c",
    "exact/resource_3c8_c_0200096c.c",
    "exact/resource_3c5_c_02000cf0.c",
    "exact/resource_3b2_c_02000da4.c",
    "exact/resource_37a_c_02001790.c",
    "exact/resource_399_c_0200021c.c",
    "exact/resource_3ca_c_020010d4.c",
    "exact/resource_399_c_02000254.c",
    "exact/resource_399_c_020005dc.c",
    "exact/resource_399_c_02000608.c",
    "exact/resource_399_c_02000668.c",
    "exact/resource_399_c_02000688.c",
    "exact/resource_399_c_02000a3c.c",
    "exact/resource_3cd_c_0200004c.c",
    "exact/resource_3ce_c_02000244.c",
    "exact/resource_3c4_c_02001068.c",
];
pub static THUMB_LOAD_LATENCY_ONE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3aa_c_02001494.c",
    "semantic/resource_3aa_c_02001494.c",
    "exact/resource_38f_c_020026cc.c",
    "exact/resource_3bc_c_020001c8.c",
];
pub static GROUPED_DMA_STORE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3bd_c_02000c98.c",
    "exact/resource_3ca_c_020010d4.c",
    "exact/resource_395_c_02001838.c",
    "semantic/resource_395_c_02001838.c",
    "exact/resource_395_c_02001858.c",
    "semantic/resource_395_c_02001858.c",
    "exact/resource_392_c_02000c0c.c",
    "exact/resource_393_c_02000ddc.c",
    "exact/resource_394_c_02000f34.c",
    "exact/resource_394_c_02000f54.c",
    "exact/resource_394_c_02000fb4.c",
    "exact/resource_393_c_02000d5c.c",
    "exact/resource_393_c_02000d7c.c",
    "exact/resource_392_c_02000b8c.c",
    "exact/resource_392_c_02000bac.c",
];
pub static NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_383_c_0200082c.c",
    "exact/resource_3b1_c_02000b84.c",
    "exact/resource_3b1_c_02000cc8.c",
];
pub static NO_CSE_SKIP_BLOCKS_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_372_c_02000278.c",
    "exact/resource_398_c_02000304.c",
    "semantic/resource_398_c_02000304.c",
];
// PORT NOTE: lists "exact/resource_3b9_c_02002904.c" twice; collapsed by the
// TypeScript `Set`, inert here. Reproduced deliberately -- see the note on
// GROUPED_DMA_STORE_SOURCES.
pub static NO_CSE_TWO_INSN_IMMEDIATE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3a2_c_02000924.c",
    "semantic/resource_3a2_c_02000924.c",
    "exact/resource_371_c_02001888.c",
    "semantic/resource_371_c_02001888.c",
    "exact/resource_371_c_02001938.c",
    "semantic/resource_371_c_02001938.c",
    "exact/resource_371_c_020019e8.c",
    "semantic/resource_371_c_020019e8.c",
    "exact/resource_39c_c_020015e0.c",
    "semantic/resource_39c_c_020015e0.c",
    "exact/resource_3b1_c_0200413c.c",
    "semantic/resource_3b1_c_0200413c.c",
    "exact/resource_38f_c_020008ec.c",
    "semantic/resource_38f_c_020008ec.c",
    "exact/resource_3b9_c_02002904.c",
    "semantic/resource_3b9_c_02002904.c",
    "exact/resource_382_c_020012c0.c",
    "semantic/resource_382_c_020012c0.c",
    "exact/resource_385_c_02000d84.c",
    "semantic/resource_385_c_02000d84.c",
    "exact/resource_382_c_02001238.c",
    "semantic/resource_382_c_02001238.c",
    "exact/resource_385_c_020009f8.c",
    "semantic/resource_385_c_020009f8.c",
    "exact/resource_3c4_c_02001fdc.c",
    "semantic/resource_3c4_c_02001fdc.c",
    "exact/resource_3c4_c_02002040.c",
    "semantic/resource_3c4_c_02002040.c",
    "exact/resource_3b9_c_020025f0.c",
    "semantic/resource_3b9_c_020025f0.c",
    "exact/resource_3b8_c_02002014.c",
    "exact/resource_3bd_c_02002c44.c",
    "exact/resource_379_c_02000074.c",
    "semantic/resource_379_c_02000074.c",
    "exact/resource_371_c_02001a98.c",
    "semantic/resource_371_c_02001a98.c",
    "exact/resource_3aa_c_02001494.c",
    "semantic/resource_3aa_c_02001494.c",
    "exact/resource_37b_c_02001624.c",
    "exact/resource_373_c_02003380.c",
    "exact/resource_373_c_020012bc.c",
    "exact/resource_3bb_c_02000b38.c",
    "exact/resource_3c8_c_020007d8.c",
    "exact/resource_373_c_02000cd0.c",
    "exact/resource_3b1_c_02005c48.c",
    "semantic/resource_3b1_c_02005c48.c",
    "exact/resource_38d_c_02001984.c",
    "exact/resource_3b4_c_020011d8.c",
    "exact/resource_39f_c_02002004.c",
    "exact/resource_3a2_c_020008a8.c",
    "exact/resource_3b5_c_0200028c.c",
    "exact/resource_372_c_02000f38.c",
    "exact/resource_3bf_c_02000bec.c",
    "exact/resource_3af_c_02001a98.c",
    "exact/resource_3af_c_02004218.c",
    "exact/resource_3c8_c_020009c8.c",
    "exact/resource_373_c_020031b4.c",
    "exact/resource_3c5_c_02001158.c",
    "exact/resource_3c5_c_02000eac.c",
    "exact/resource_3a8_c_0200158c.c",
    "exact/resource_3a8_c_020015b4.c",
    "exact/resource_374_c_02000780.c",
    "exact/resource_39c_c_020010c0.c",
    "exact/resource_39e_c_0200071c.c",
    "exact/resource_3af_c_02001f90.c",
    "exact/resource_380_c_02000390.c",
    "exact/resource_373_c_020032b0.c",
    "exact/resource_3bf_c_02004704.c",
    "exact/resource_3bf_c_020049a0.c",
    "exact/resource_3bf_c_0200169c.c",
    "exact/resource_3bf_c_0200269c.c",
    "exact/resource_3bf_c_02002718.c",
    "exact/resource_3bf_c_020025f8.c",
    "exact/resource_3bf_c_020021c4.c",
    "exact/resource_3b0_c_020004b0.c",
    "exact/resource_39e_c_02000414.c",
    "exact/resource_372_c_02003c48.c",
    "exact/resource_3b8_c_02003e40.c",
    "exact/resource_3a4_c_02000c9c.c",
    "exact/resource_3af_c_02001b58.c",
    "exact/resource_3af_c_020019c0.c",
    "exact/resource_3af_c_020012f0.c",
    "exact/resource_3af_c_02002b7c.c",
    "exact/resource_3ba_c_02000974.c",
    "exact/resource_3aa_c_02001450.c",
    "exact/resource_3b9_c_020004c8.c",
    "exact/resource_3b9_c_0200055c.c",
    "exact/resource_3b9_c_02002904.c",
    "exact/resource_3b9_c_02002668.c",
    "exact/resource_3b9_c_02002964.c",
];
// PORT NOTE: the worst of the five. Four entries appear twice --
// "exact/resource_3c8_c_02002f30.c", "semantic/resource_3c8_c_02002f30.c",
// "exact/resource_3c6_c_02000158.c", "exact/resource_394_c_020007e0.c" -- all
// collapsed by the TypeScript `Set` and inert here. Reproduced deliberately;
// see the note on GROUPED_DMA_STORE_SOURCES.
pub static SCHED_LOW_DEST_FIRST_OVERLAY_SOURCES: &[&str] = &[
    "semantic/resource_373_c_02002f14.c",
    "exact/resource_373_c_02002f14.c",
    "semantic/resource_3a2_c_02000924.c",
    "exact/resource_3a2_c_02000924.c",
    "semantic/resource_372_c_02003e48.c",
    "exact/resource_372_c_02003e48.c",
    "semantic/resource_3a3_c_02000d08.c",
    "exact/resource_3a3_c_02000d08.c",
    "semantic/resource_373_c_020010d8.c",
    "exact/resource_373_c_020010d8.c",
    "semantic/resource_3c8_c_02002f30.c",
    "exact/resource_3c8_c_02002f30.c",
    "semantic/resource_3c6_c_02000158.c",
    "exact/resource_3c6_c_02000158.c",
    "semantic/resource_39f_c_02000f94.c",
    "exact/resource_39f_c_02000f94.c",
    "semantic/resource_37f_c_020017c0.c",
    "exact/resource_37f_c_020017c0.c",
    "exact/resource_394_c_020007e0.c",
    "semantic/resource_394_c_020007e0.c",
    "exact/resource_3b1_c_0200413c.c",
    "semantic/resource_3b1_c_0200413c.c",
    "exact/resource_3b1_c_02000728.c",
    "semantic/resource_3b1_c_02000728.c",
    "exact/resource_3c8_c_02002f30.c",
    "semantic/resource_3c8_c_02002f30.c",
    "semantic/resource_377_c_020001e0.c",
    "exact/resource_377_c_020001e0.c",
    "semantic/resource_3a2_c_02000ac0.c",
    "exact/resource_3a2_c_02000ac0.c",
    "semantic/resource_3ae_c_02000144.c",
    "exact/resource_3ae_c_02000144.c",
    "semantic/resource_3ae_c_020002dc.c",
    "exact/resource_3ae_c_020002dc.c",
    "semantic/resource_399_c_020002b8.c",
    "semantic/resource_39f_c_020021b0.c",
    "exact/resource_39f_c_020021b0.c",
    "exact/resource_38b_c_020009cc.c",
    "semantic/resource_38b_c_020009cc.c",
    "exact/resource_38d_c_02000568.c",
    "semantic/resource_38d_c_02000568.c",
    "exact/resource_38d_c_020005f4.c",
    "semantic/resource_38d_c_020005f4.c",
    "exact/resource_37b_c_02000c8c.c",
    "semantic/resource_37b_c_02000c8c.c",
    "exact/resource_372_c_0200150c.c",
    "semantic/resource_372_c_0200150c.c",
    "exact/resource_37f_c_02000154.c",
    "semantic/resource_37f_c_02000154.c",
    "exact/resource_3b9_c_02002904.c",
    "semantic/resource_3b9_c_02002904.c",
    "exact/resource_382_c_020012c0.c",
    "semantic/resource_382_c_020012c0.c",
    "exact/resource_385_c_02000d84.c",
    "semantic/resource_385_c_02000d84.c",
    "exact/resource_3c0_c_02000adc.c",
    "semantic/resource_3c0_c_02000adc.c",
    "exact/resource_389_c_020009dc.c",
    "semantic/resource_389_c_020009dc.c",
    "exact/resource_3b9_c_020004c8.c",
    "semantic/resource_3b9_c_020004c8.c",
    "exact/resource_3bf_c_0200206c.c",
    "semantic/resource_3bf_c_0200206c.c",
    "exact/resource_385_c_02000a80.c",
    "semantic/resource_385_c_02000a80.c",
    "exact/resource_382_c_02001238.c",
    "semantic/resource_382_c_02001238.c",
    "exact/resource_385_c_020009f8.c",
    "semantic/resource_385_c_020009f8.c",
    "exact/resource_3c4_c_02001fdc.c",
    "semantic/resource_3c4_c_02001fdc.c",
    "exact/resource_3c4_c_02002040.c",
    "semantic/resource_3c4_c_02002040.c",
    "exact/resource_3b9_c_020025f0.c",
    "semantic/resource_3b9_c_020025f0.c",
    "exact/resource_3b4_c_02002334.c",
    "semantic/resource_3b4_c_02002334.c",
    "exact/resource_3b4_c_02000ad0.c",
    "semantic/resource_3b4_c_02000ad0.c",
    "exact/resource_3ab_c_02000c10.c",
    "semantic/resource_3ab_c_02000c10.c",
    "exact/resource_3ab_c_02000b0c.c",
    "semantic/resource_3ab_c_02000b0c.c",
    "exact/resource_3ab_c_020015bc.c",
    "semantic/resource_3ab_c_020015bc.c",
    "exact/resource_3c4_c_02001aba.c",
    "semantic/resource_3c4_c_02001aba.c",
    "exact/resource_3b2_c_02001214.c",
    "semantic/resource_3b2_c_02001214.c",
    "exact/resource_3c6_c_020011bc.c",
    "exact/resource_3c6_c_02000158.c",
    "exact/resource_3cb_c_020011bc.c",
    "exact/resource_38f_c_020008ec.c",
    "exact/resource_38f_c_02000304.c",
    "semantic/resource_38f_c_020008ec.c",
    "exact/resource_379_c_02000074.c",
    "exact/resource_3b8_c_02002014.c",
    "exact/resource_3bd_c_02002c44.c",
    "exact/resource_394_c_020007e0.c",
    "exact/resource_373_c_02003380.c",
    "exact/resource_373_c_020012bc.c",
    "exact/resource_3b5_c_02000644.c",
    "exact/resource_371_c_0200281c.c",
    "exact/resource_371_c_02002858.c",
    "exact/resource_3bb_c_02000b38.c",
    "exact/resource_3c8_c_020007d8.c",
    "exact/resource_372_c_02000278.c",
    "exact/resource_373_c_020011d8.c",
    "exact/resource_3b4_c_020011d8.c",
    "exact/resource_3aa_c_0200140c.c",
    "exact/resource_3aa_c_02001450.c",
    "exact/resource_3aa_c_02001494.c",
    "semantic/resource_3aa_c_02001494.c",
    "exact/resource_3aa_c_02000184.c",
    "exact/resource_38d_c_02001984.c",
    "exact/resource_3bf_c_02000c78.c",
    "exact/resource_39f_c_02002004.c",
    "exact/resource_38d_c_02000150.c",
    "exact/resource_39a_c_020017a8.c",
    "exact/resource_383_c_020019a4.c",
    "exact/resource_3bf_c_02000bac.c",
    "exact/resource_3b6_c_02000898.c",
    "exact/resource_3b5_c_02000568.c",
    "exact/resource_3b5_c_02000224.c",
    "exact/resource_3b5_c_02000528.c",
    "exact/resource_3a2_c_02000870.c",
    "exact/resource_3a2_c_020008a8.c",
    "exact/resource_3a2_c_02000180.c",
    "exact/resource_3c8_c_02001780.c",
    "exact/resource_3c8_c_02001150.c",
    "exact/resource_372_c_02000f38.c",
    "exact/resource_381_c_02000e30.c",
    "exact/resource_3c5_c_02001030.c",
    "exact/resource_38f_c_020002b4.c",
    "exact/resource_3a8_c_02001ed8.c",
    "exact/resource_3a8_c_02000504.c",
    "exact/resource_3a8_c_02000464.c",
    "exact/resource_374_c_02000750.c",
    "exact/resource_374_c_020007ec.c",
    "exact/resource_374_c_0200081c.c",
    "exact/resource_374_c_0200084c.c",
    "exact/resource_374_c_0200087c.c",
    "exact/resource_374_c_020008ac.c",
    "exact/resource_374_c_02000780.c",
    "exact/resource_3b2_c_02001494.c",
    "exact/resource_39e_c_0200071c.c",
    "exact/resource_3af_c_02001f90.c",
    "exact/resource_380_c_02000390.c",
    "exact/resource_373_c_020032b0.c",
    "exact/resource_3bf_c_02004704.c",
    "exact/resource_3bf_c_020049a0.c",
    "exact/resource_3bf_c_0200269c.c",
    "exact/resource_3bf_c_02002718.c",
    "exact/resource_3bf_c_020025f8.c",
    "exact/resource_3bf_c_020021c4.c",
    "exact/resource_3b0_c_02000468.c",
    "exact/resource_399_c_020019bc.c",
    "exact/resource_39c_c_02000ffc.c",
    "exact/resource_39c_c_02002e6c.c",
    "exact/resource_39e_c_02000658.c",
    "exact/resource_3af_c_02000c28.c",
    "exact/resource_372_c_02001600.c",
    "exact/resource_3b0_c_020004b0.c",
    "exact/resource_372_c_02000398.c",
    "exact/resource_372_c_020003cc.c",
    "exact/resource_372_c_02000400.c",
    "exact/resource_3b8_c_020003b0.c",
    "exact/resource_3b8_c_02003d40.c",
    "exact/resource_38c_c_020001a8.c",
    "exact/resource_38c_c_02000430.c",
    "exact/resource_38c_c_0200035c.c",
    "exact/resource_3b5_c_0200028c.c",
    "exact/resource_372_c_0200198c.c",
    "exact/resource_39f_c_02001818.c",
    "exact/resource_39f_c_02000d90.c",
    "exact/resource_38e_c_0200045c.c",
    "exact/resource_3a3_c_020006a4.c",
    "exact/resource_3c6_c_0200010c.c",
    "exact/resource_39a_c_02001ad0.c",
    "exact/resource_3c3_c_02000288.c",
    "exact/resource_386_c_02000204.c",
    "exact/resource_386_c_0200011c.c",
    "exact/resource_383_c_0200091c.c",
    "exact/resource_383_c_020019e4.c",
    "exact/resource_38c_c_02000124.c",
    "exact/resource_38c_c_020001e0.c",
    "exact/resource_38c_c_02000250.c",
    "exact/resource_3b8_c_02003df8.c",
    "exact/resource_3b8_c_02003e40.c",
    "exact/resource_373_c_02000dc0.c",
    "exact/resource_373_c_02000e54.c",
    "exact/resource_373_c_02000e84.c",
    "exact/resource_373_c_02000f5c.c",
    "exact/resource_373_c_02000f8c.c",
    "exact/resource_373_c_02000fbc.c",
    "exact/resource_373_c_02000fec.c",
    "exact/resource_373_c_02001490.c",
    "exact/resource_37b_c_020015d4.c",
    "exact/resource_37b_c_020015fc.c",
    "exact/resource_37b_c_0200101c.c",
    "exact/resource_37b_c_02001624.c",
    "exact/resource_37b_c_0200166c.c",
    "exact/resource_37b_c_020016a4.c",
    "exact/resource_37b_c_0200195c.c",
    "exact/resource_3a4_c_02000c9c.c",
    "exact/resource_394_c_020008b0.c",
    "exact/resource_3b8_c_02000264.c",
    "exact/resource_373_c_020031b4.c",
    "exact/resource_3af_c_02001b58.c",
    "exact/resource_3af_c_020019c0.c",
    "exact/resource_3af_c_020012f0.c",
    "exact/resource_3af_c_02002b7c.c",
    "exact/resource_3ba_c_02000974.c",
    "exact/resource_39e_c_02002484.c",
    "exact/resource_39e_c_0200268c.c",
    "exact/resource_39e_c_02002778.c",
    "exact/resource_3bf_c_02000ce0.c",
    "semantic/resource_3bf_c_02000ce0.c",
    "exact/resource_3bf_c_02000dcc.c",
    "semantic/resource_3bf_c_02000dcc.c",
    "exact/resource_3bf_c_02000e80.c",
    "semantic/resource_3bf_c_02000e80.c",
    "exact/resource_3bf_c_02000f30.c",
    "semantic/resource_3bf_c_02000f30.c",
    "exact/resource_382_c_02000ef4.c",
    "exact/resource_3c9_c_02001280.c",
];
pub static NO_CSE_SHIFT_IMMEDIATE_OVERLAY_SOURCES: &[&str] = &[
    "semantic/resource_3ae_c_02000144.c",
    "exact/resource_3ae_c_02000144.c",
    "exact/resource_3cb_c_020002d8.c",
    "semantic/resource_3cb_c_020002d8.c",
    "exact/resource_372_c_0200150c.c",
    "semantic/resource_372_c_0200150c.c",
    "semantic/resource_3bc_c_020002f8.c",
    "exact/resource_3bc_c_020002f8.c",
    "semantic/resource_3bc_c_020004a4.c",
    "exact/resource_3bc_c_020004a4.c",
    "semantic/resource_3bc_c_0200076c.c",
    "exact/resource_3bc_c_0200076c.c",
    "semantic/resource_3bc_c_02001474.c",
    "exact/resource_3bc_c_02001474.c",
    "semantic/resource_3bc_c_02001a0c.c",
    "exact/resource_3bc_c_02001a0c.c",
    "semantic/resource_3bc_c_02001c20.c",
    "exact/resource_3bc_c_02001c20.c",
    "semantic/resource_3bc_c_020029ac.c",
    "exact/resource_3bc_c_020029ac.c",
    "semantic/resource_3bc_c_02002bac.c",
    "exact/resource_3bc_c_02002bac.c",
    "semantic/resource_3bc_c_02002e54.c",
    "exact/resource_3bc_c_02002e54.c",
    "semantic/resource_3bc_c_02002ee8.c",
    "exact/resource_3bc_c_02002ee8.c",
    "semantic/resource_3bc_c_020033d8.c",
    "exact/resource_3bc_c_020033d8.c",
    "semantic/resource_3bc_c_02003bd0.c",
    "exact/resource_3bc_c_02003bd0.c",
    "semantic/resource_3bc_c_02003d88.c",
    "exact/resource_3bc_c_02003d88.c",
    "semantic/resource_3bc_c_02003ef0.c",
    "exact/resource_3bc_c_02003ef0.c",
    "semantic/resource_3bc_c_0200457c.c",
    "exact/resource_3bc_c_0200457c.c",
    "exact/resource_3bf_c_02000ce0.c",
    "semantic/resource_3bf_c_02000ce0.c",
    "exact/resource_3bf_c_02000dcc.c",
    "semantic/resource_3bf_c_02000dcc.c",
    "exact/resource_3bf_c_02000e80.c",
    "semantic/resource_3bf_c_02000e80.c",
    "exact/resource_3bf_c_02000f30.c",
    "semantic/resource_3bf_c_02000f30.c",
    "exact/resource_371_c_020017a4.c",
    "semantic/resource_371_c_020017a4.c",
    "exact/resource_3c8_c_02002f30.c",
    "semantic/resource_3c8_c_02002f30.c",
    "exact/resource_3b9_c_020004c8.c",
    "semantic/resource_3b9_c_020004c8.c",
    "exact/resource_3bf_c_0200206c.c",
    "semantic/resource_3bf_c_0200206c.c",
    "exact/resource_3b4_c_02002334.c",
    "semantic/resource_3b4_c_02002334.c",
    "exact/resource_3b4_c_02000ad0.c",
    "semantic/resource_3b4_c_02000ad0.c",
    "exact/resource_3ab_c_020015bc.c",
    "semantic/resource_3ab_c_020015bc.c",
    "exact/resource_3c4_c_02001aba.c",
    "semantic/resource_3c4_c_02001aba.c",
];
pub static FIXED_R7_OVERLAY_SOURCES: &[&str] = &["exact/resource_372_c_02000ec4.c"];
pub static SCHED_HIGH_DEST_FIRST_OVERLAY_SOURCES: &[&str] = &["exact/resource_372_c_02000ec4.c"];
pub static SCHED_CALL_DEST_DESCENDING_OVERLAY_SOURCES: &[&str] =
    &["exact/resource_3bb_c_020039fc.c"];
pub static NO_SCHED_ALIAS_OVERLAY_SOURCES: &[&str] = &[
    "exact/08078144.c",
    "exact/resource_3af_c_02002b7c.c",
    "exact/resource_3b0_c_02000030.c",
    "exact/resource_381_c_02002e0c.c",
    "exact/resource_381_c_02002e5c.c",
    "exact/resource_38f_c_020027ac.c",
    "exact/resource_391_c_02002974.c",
];
pub static NO_GCSE_INSERT_LOAD_OVERLAY_SOURCES: &[&str] =
    &["exact/0807a550.c", "exact/resource_37a_c_02000d9c.c"];
pub static SCHED_STORE_FIRST_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_373_c_020032b0.c",
    "exact/resource_3bd_c_02000a54.c",
];
// PORT NOTE: lists "exact/resource_394_c_020007e0.c" twice; collapsed by the
// TypeScript `Set`, inert here. Reproduced deliberately -- see the note on
// GROUPED_DMA_STORE_SOURCES.
pub static NO_CSE_POOL_IMMEDIATE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3a8_c_02003768.c",
    "semantic/resource_3a8_c_02003768.c",
    "semantic/resource_3ae_c_0200051c.c",
    "exact/resource_3ae_c_0200051c.c",
    "exact/resource_394_c_020007e0.c",
    "semantic/resource_394_c_020007e0.c",
    "exact/resource_3b1_c_02000728.c",
    "semantic/resource_3b1_c_02000728.c",
    "exact/resource_3b4_c_02000ad0.c",
    "semantic/resource_3b4_c_02000ad0.c",
    "exact/resource_3c8_c_020016a4.c",
    "semantic/resource_3c8_c_020016a4.c",
    "exact/resource_382_c_02001238.c",
    "semantic/resource_382_c_02001238.c",
    "exact/resource_385_c_020009f8.c",
    "semantic/resource_385_c_020009f8.c",
    "exact/resource_38f_c_020008ec.c",
    "semantic/resource_38f_c_020008ec.c",
    "exact/resource_3aa_c_02001494.c",
    "semantic/resource_3aa_c_02001494.c",
    "exact/resource_3b8_c_02002014.c",
    "exact/resource_3bd_c_02002c44.c",
    "exact/resource_379_c_02000074.c",
    "semantic/resource_379_c_02000074.c",
    "exact/resource_38b_c_02000240.c",
    "exact/resource_372_c_02000f38.c",
    "exact/resource_37b_c_02001b44.c",
    "exact/resource_3c5_c_020024d0.c",
    "exact/resource_3c5_c_02002548.c",
    "exact/resource_3a8_c_02001ed8.c",
    "exact/resource_374_c_02000634.c",
    "exact/resource_3af_c_02001f90.c",
    "exact/resource_380_c_02000390.c",
    "exact/resource_3bf_c_020049a0.c",
    "exact/resource_3bf_c_02002308.c",
    "exact/resource_3bf_c_0200238c.c",
    "exact/resource_372_c_02001600.c",
    "exact/resource_3b8_c_02003f84.c",
    "exact/resource_37b_c_0200195c.c",
    "exact/resource_37b_c_02002244.c",
    "exact/resource_37b_c_020022f4.c",
    "exact/resource_3a4_c_02000c9c.c",
    "exact/resource_394_c_020008b0.c",
    "exact/resource_394_c_020007e0.c",
    "exact/resource_3b8_c_02000264.c",
    "exact/resource_3bf_c_02004bfc.c",
    "exact/resource_39c_c_020014cc.c",
    "exact/resource_3bf_c_0200175c.c",
    "exact/resource_3bf_c_020017bc.c",
    "exact/resource_3b9_c_0200055c.c",
    "exact/resource_3b9_c_02002668.c",
    "exact/resource_3b9_c_02002964.c",
];
pub static NO_STRICT_ALIASING_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_380_c_02000104.c",
    "exact/resource_39c_c_02000104.c",
    "exact/resource_39e_c_02000104.c",
    "exact/resource_39b_c_02001c20.c",
    "semantic/resource_39b_c_02001c20.c",
    "exact/resource_39c_c_020003a8.c",
    "exact/resource_3c9_c_02000104.c",
    "exact/resource_3c9_c_0200215c.c",
    "exact/resource_3c9_c_020021ac.c",
    "exact/resource_3c9_c_02003600.c",
    "exact/resource_397_c_020002a0.c",
];
pub static SCHED_POOL_LOAD_LATE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3b3_c_02002384.c",
    "semantic/resource_3b3_c_02002384.c",
];
pub static SCHED_IMMEDIATE_BEFORE_POOL_OVERLAY_SOURCES: &[&str] = &[];
pub static THUMB_HI_IMMEDIATE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_377_c_020003f8.c",
    "semantic/resource_377_c_020003f8.c",
];
pub static CALL_POOL_ARG1_FIRST_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3a2_c_02000924.c",
    "semantic/resource_3a2_c_02000924.c",
    "exact/resource_372_c_02003e48.c",
    "semantic/resource_372_c_02003e48.c",
];
pub static ARG_BEFORE_FINAL_SHIFT_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3b1_c_0200366c.c",
    "semantic/resource_3b1_c_0200366c.c",
];
pub static CALL_ARG0_BEFORE_POOL_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_371_c_02001888.c",
    "semantic/resource_371_c_02001888.c",
    "exact/resource_371_c_02001938.c",
    "semantic/resource_371_c_02001938.c",
    "exact/resource_371_c_020019e8.c",
    "semantic/resource_371_c_020019e8.c",
];
pub static CALL_ARGREG_BEFORE_POOL_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3a7_c_02000b8c.c",
    "semantic/resource_3a7_c_02000b8c.c",
];
pub static SWAP_ADJACENT_SHIFTS_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3bc_c_02004494.c",
    "semantic/resource_3bc_c_02004494.c",
    "exact/resource_3a4_c_020002cc.c",
    "semantic/resource_3a4_c_020002cc.c",
];
pub static STACK_ARGS_BEFORE_STORES_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_382_c_02000fb4.c",
    "semantic/resource_382_c_02000fb4.c",
    "exact/resource_382_c_02001010.c",
    "semantic/resource_382_c_02001010.c",
    "exact/resource_382_c_0200113c.c",
    "semantic/resource_382_c_0200113c.c",
    "exact/resource_3b9_c_02001c6c.c",
    "semantic/resource_3b9_c_02001c6c.c",
    "exact/resource_382_c_02001090.c",
    "semantic/resource_382_c_02001090.c",
];
pub static LITERAL_ARG1_FIRST_AFTER_CALL_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3b9_c_020006bc.c",
    "semantic/resource_3b9_c_020006bc.c",
    "exact/resource_376_c_02000258.c",
    "semantic/resource_376_c_02000258.c",
    "exact/resource_376_c_02000190.c",
    "semantic/resource_376_c_02000190.c",
    "exact/resource_382_c_02000614.c",
    "semantic/resource_382_c_02000614.c",
];
pub static SMALL_SHIFT_BEFORE_IMMEDIATES_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_39f_c_020021b0.c",
    "semantic/resource_39f_c_020021b0.c",
];
pub static BLOCKMOVE_DEST_BEFORE_SOURCE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_39f_c_02000f94.c",
    "semantic/resource_39f_c_02000f94.c",
];
pub static LITERAL_ARG1_FIRST_CHAINED_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3ad_c_020011b8.c",
    "semantic/resource_3ad_c_020011b8.c",
];
pub static HIGH_MOVE_BEFORE_STORE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_382_c_02001090.c",
    "semantic/resource_382_c_02001090.c",
];
pub static POOL_LOAD_BEFORE_LOAD_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_371_c_020002f0.c",
    "semantic/resource_371_c_020002f0.c",
];
pub static SHIFT_BEFORE_STORE_IN_SPLIT_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_371_c_020002f0.c",
    "semantic/resource_371_c_020002f0.c",
];
pub static ARG_BEFORE_SHIFT_IN_SHEET_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3b1_c_02005ca4.c",
    "semantic/resource_3b1_c_02005ca4.c",
    "exact/resource_3a5_c_02001874.c",
    "semantic/resource_3a5_c_02001874.c",
    "exact/resource_382_c_02000614.c",
    "semantic/resource_382_c_02000614.c",
];
pub static SINK_LOAD_PAST_STORE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_382_c_02000104.c",
    "semantic/resource_382_c_02000104.c",
    "exact/resource_385_c_02000104.c",
    "semantic/resource_385_c_02000104.c",
    "exact/resource_387_c_02000104.c",
    "semantic/resource_387_c_02000104.c",
    "exact/resource_38a_c_02000104.c",
    "semantic/resource_38a_c_02000104.c",
    "exact/resource_396_c_02000104.c",
    "semantic/resource_396_c_02000104.c",
    "exact/resource_39b_c_02000104.c",
    "semantic/resource_39b_c_02000104.c",
    "exact/resource_3a0_c_02000104.c",
    "semantic/resource_3a0_c_02000104.c",
    "exact/resource_3a5_c_02000104.c",
    "semantic/resource_3a5_c_02000104.c",
    "exact/resource_3a6_c_02000104.c",
    "semantic/resource_3a6_c_02000104.c",
    "exact/resource_3ab_c_02000104.c",
    "semantic/resource_3ab_c_02000104.c",
    "exact/resource_3b3_c_02000104.c",
    "semantic/resource_3b3_c_02000104.c",
    "exact/resource_3be_c_02000104.c",
    "semantic/resource_3be_c_02000104.c",
    "exact/resource_3c0_c_02000104.c",
    "semantic/resource_3c0_c_02000104.c",
];
pub static CALL_ARG0_BETWEEN_POOL_PAIR_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_39b_c_02000f48.c",
    "semantic/resource_39b_c_02000f48.c",
];
pub static STORE_VALUE_BEFORE_BASE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3ca_c_0200004c.c",
    "semantic/resource_3ca_c_0200004c.c",
];
pub static SWAP_SHIFTS_ACROSS_INSN_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3c8_c_02002f30.c",
    "semantic/resource_3c8_c_02002f30.c",
];
pub static ORR_INTO_OLDER_INPUT_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3b3_c_02001fd4.c",
    "semantic/resource_3b3_c_02001fd4.c",
];
pub static CALL_ARG0_BEFORE_POOL_PAIR_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_371_c_02001a98.c",
    "semantic/resource_371_c_02001a98.c",
];
pub static SINK_POOL_LOAD_TO_USE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3c6_c_02000158.c",
    "semantic/resource_3c6_c_02000158.c",
];
pub static SINK_CONSTANT_PAST_CALL_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_39e_c_020026d8.c",
    "semantic/resource_39e_c_020026d8.c",
];
pub static NO_THREAD_JUMPS_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3c4_c_02001aba.c",
    "semantic/resource_3c4_c_02001aba.c",
];
pub static NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES: &[&str] = &[
    "semantic/resource_3ad_c_02000460.c",
    "exact/resource_3ad_c_02000460.c",
    "exact/resource_3b3_c_02002384.c",
    "semantic/resource_3b3_c_02002384.c",
    "exact/resource_3b4_c_02001308.c",
    "semantic/resource_3b4_c_02001308.c",
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
    "semantic/resource_398_c_02000214.c",
    "exact/resource_398_c_02000538.c",
    "semantic/resource_398_c_02000538.c",
    "exact/resource_3a2_c_02000924.c",
    "semantic/resource_3a2_c_02000924.c",
];
pub static NO_GCSE_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_38f_c_020008ec.c",
    "semantic/resource_38f_c_020008ec.c",
    "exact/resource_3cd_c_020000c0.c",
];
pub static DEFAULT_ABI_SOURCES: &[&str] = &[
    "08006a00", "08006b84", "08006ba8", "08006c24", "08006dec", "08007098", "080fada0", "080fadf0",
];
pub static DEFAULT_ABI_OVERLAY_SOURCES: &[&str] = &[
    "exact/resource_3a7_c_0200145c.c",
    "exact/resource_3a7_c_02001574.c",
    "semantic/resource_3bf_c_02005b6c.c",
    "exact/resource_3bf_c_02005b6c.c",
    "exact/resource_3bf_c_02005af0.c",
    "exact/resource_3bf_c_02005c08.c",
    "exact/resource_3bf_c_02005a40.c",
    "exact/resource_3bf_c_02005a78.c",
    "exact/resource_3a7_c_020013ac.c",
    "exact/resource_3a7_c_020013e4.c",
];
pub static AGBCC_SOURCES: &[&str] = &[
    "080069a4", "08006a00", "08006ba8", "08006c24", "08006c68", "08006cdc", "08006d50", "08006dec",
    "08006e24", "08006f48", "08006f84", "08007028", "08007098", "0800711c", "080071a8", "08007220",
    "080f9a50", "080fada0", "080fadf0", "080fa1fc", "080fa2a0", "080fa324", "080fa350", "080fa39c",
    "080fa3f0", "080fa424", "080fa458", "080fa490", "080fa514", "080fa55c", "080fa6a0", "080fa83c",
    "080fa8d4", "080fa928", "080fa9a4", "080fa9e0", "080fab3c", "080fab7c", "080fac44", "080facf8",
    "080fb2cc", "080fb334", "080fb3a8", "080fb430", "080fb4a4", "080fb670", "080fb6a4",
];
pub static AGBCC_LITERAL_BEFORE_SHIFT_SOURCES: &[&str] = &["080fb670"];
pub static AGBCC_OPTIMIZE_O1_SOURCES: &[&str] = &[
    "08006a00", "08006ba8", "08006c68", "08006cdc", "08006d50", "08006e24", "08006f84", "08007028",
    "08007098", "0800711c", "080071a8", "08007220", "080fa514",
];
pub static AGBCC_COMPARE_ONLY_AND_TST_SOURCES: &[&str] = &["080f9a50"];
pub static AGBCC_COMMUTATIVE_COPY_CONSTANT_SOURCES: &[&str] = &["080fa514"];
pub static AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES: &[&str] = &["080fb2cc", "080fb334", "080fb3a8"];
pub static FIXED_LR_SOURCES: &[&str] = &[];
