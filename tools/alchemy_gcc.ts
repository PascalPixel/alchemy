#!/usr/bin/env bun
import { readFileSync, statSync } from "node:fs";
import { basename, dirname, extname, join, relative, resolve, sep } from "node:path";

export const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
// 承認済みコンパイラ束は姉妹リポジトリ PascalPixel/alchemy-gcc の dist/ に住む。
// ソースからの再構築手順は同リポジトリの build.sh、検証は下の validateBundle。
export const BUNDLE = join(ROOT, "..", "alchemy-gcc", "dist");
export const DRIVER = join(BUNDLE, "xgcc");
export const M2C = join(BUNDLE, "m2c-venv/bin/m2c");
export type CompilerTarget = "gs1" | "gs2";
export const GS2_BUNDLE = join(BUNDLE, "gs2");
export const GS2_DRIVER = join(GS2_BUNDLE, "xgcc");
export const AGBCC_BUNDLE = join(BUNDLE, "agbcc");
export const AGBCC_DRIVER = join(AGBCC_BUNDLE, "old_agbcc");
// -nostdinc keeps the host's headers out; -Iinclude puts this repository's own
// back in. Before it, the eight fixed-width typedefs were restated in 1,224 of
// the 1,249 sources, 7,751 lines of the same eight declarations.
export const CFLAGS = [
  "-O2", "-mthumb", "-mthumb-interwork", "-mcpu=arm7tdmi",
  "-fno-builtin", "-nostdinc", "-ffreestanding", "-fcall-used-r4",
  `-I${join(ROOT, "include")}`,
] as const;
export const GS2_CFLAGS = [...CFLAGS, "-ffixed-r7"] as const;
export const AGBCC_CFLAGS = [
  "-mthumb-interwork", "-O2", "-fno-builtin", "-ffreestanding",
] as const;

export function bundleForTarget(target: CompilerTarget): string {
  return target === "gs1" ? BUNDLE : GS2_BUNDLE;
}

export function driverForTarget(target: CompilerTarget): string {
  return target === "gs1" ? DRIVER : GS2_DRIVER;
}

export function cflagsForTarget(target: CompilerTarget): readonly string[] {
  return target === "gs1" ? CFLAGS : GS2_CFLAGS;
}

// These tiny command-stream handlers never use r3 in the reference code.  GCC
// reproduces their allocation exactly when the translation unit reserves r3,
// consistent with an original file-scope register assignment.  Keep this
// evidence as compiler configuration instead of expressing a register pin in C.
const FIXED_R3_SOURCES = new Set([
  "080fb6ec", "080fb700", "080fb714", "080fb728", "080fb73c",
  "080fb750", "080fb75c", "080fb768", "080fb77c",
]);
// This default-ABI palette family reserves lr while keeping long-lived loop
// constants in r8/r9; allowing lr as a general register changes the prologue
// and allocation before source shaping can affect either.
const FIXED_LR_SOURCES = new Set<string>();
// These compact hardware helpers match the reference load/store order at -O1;
// -O2 only swaps independent descriptor setup instructions.
const OPTIMIZE_O1_SOURCES = new Set(["080049e8", "08021e28"]);
// Only the second flag does anything. The pre-reload scheduler is inert in this
// fork: 40 converted sources, including the largest, compile byte-identically
// with -fschedule-insns and with -fno-schedule-insns (measured 2026-07-26,
// work/sched_probe.ts). Every member below is carried by -fno-schedule-insns2
// alone; the first flag is kept only because removing it would rewrite the
// routed command line for sixteen already-verified regions.
const UNSCHEDULED_SOURCES = new Set([
    "08006b84",
  "08002f10",
  "08004198", "080042c8", "0800430c", "08004358", "0800439c", "080043e0",
  "08029274",
  "080fb714", "080fb728", "080fb73c", "080fb750", "080fb75c",
  "080fb768", "080fb77c",
]);
// This decoder has mutually exclusive switch arms that reuse the same input
// base.  Following jumps during CSE rematerializes one arm's base in r3;
// disabling that pass preserves the reference's r6 lifetime and coalescing.
const NO_CSE_FOLLOW_SOURCES = new Set(["0800f9f4"]);
// This unrolled six-item display setup shares one resource-ID base and one
// signed sentinel.  Global CSE expands them back into independent constants;
// disabling it preserves the reference's r7/r8 lifetimes.
// 080b2720 is the same tell in a copy loop: the row base is addressed once for
// the emptiness test and again for the loop preheader, and global CSE keeps the
// shared address alive across the branch instead of recomputing it inside.
const NO_GCSE_SOURCES = new Set(["080a45cc", "080b2720"]);
// This bounded-angle convergence loop only retains the reference allocation
// when GCC does not perform its expensive-expression rewrite.  The rewrite
// rotates r0-r2 and folds each signed clamp into a shorter non-reference form.
const NO_EXPENSIVE_SOURCES = new Set([
  "08092878"]);
// This four-step signed index loop is emitted as an ascending loop in the
// reference translation unit. Strength reduction rewrites it to a descending
// counter and changes both the allocation and loop tail.
const NO_STRENGTH_REDUCE_SOURCES = new Set(["080200cc", "080a9d3c"]);
// arm_reorg pulls the two halves of a split constant back together when the
// scheduler put an independent insn between them. These references want the
// insn left where it is; see alchemy-gcc 1ec1044 and work/hand/080a1090.
const NO_CONTIGUOUS_IMMEDIATE_SOURCES = new Set(["080a1090", "08005a78", "0800d304", "08019bac"]);
// The grouped transfer restores its base register, so the DMA status poll that
// follows reuses it instead of loading the pool word again the way the
// reference does. Splitting the live range is the only way to spell two
// materialisations of one constant; see alchemy-gcc ff7c566.
const SPLIT_GROUP_BASE_SOURCES = new Set(["08005a78"]);
// The reference emits every parameter save before the body; ours leaves the
// second one after the pool load that follows it. See alchemy-gcc.
const HOIST_PARAMETER_SAVE_SOURCES = new Set(["08019bac"]);
// The reference saves the second parameter before the first; ours always follows
// parameter order. Two halfwords, and the whole difference in 08093054.
const ENTRY_SAVES_DESCENDING_SOURCES = new Set(["08093054"]);
// thumb_order_grouped_dma_store only normalises descriptor setup order when the
// three setup insns are adjacent. When the source word needs arithmetic the
// interleaved insns hide them, and the control load stays hoisted -- which also
// permutes the constant pool. See alchemy-gcc ff7c566.
const GROUP_CONTROL_LAST_SOURCES = new Set(["08005a78"]);
// The descriptor's base pool load wins a priority-68 ready-list tie on forward
// dependent count alone; these references break it by original order instead.
const NO_SCHED_DEPEND_COUNT_SOURCES = new Set(["08002fb0", "08003e10", "0800d304", "08019bac"]);
// The reference issues the destination copy ahead of the control word's `orrs`.
const MOVE_BEFORE_ALU_SOURCES = new Set(["08002fb0", "08003e10", "0800d304", "08019bac"]);
// This palette-row scan ANDs a loaded halfword against a hoisted 0xF800 mask.
// The AND is a two-address *thumb_andsi3_insn, so regmove's forward pass may
// overwrite either input; it rejects the mask operand at reg_is_remote_constant_p
// (CSE's REG_EQUAL note sits in the entry block, unreachable through LOG_LINKS)
// and instead retargets the loaded value, giving `ands r2,r3` where the
// reference has `ands r3,r2`. In the reference translation unit no regmove fix
// happens at all, because local-alloc's update_equiv_regs later sinks the
// constant next to its use and ties the two pseudos. regmove runs before
// local-alloc, so nothing the C source can say is present early enough to break
// the forward scan.
const NO_REGMOVE_SOURCES = new Set(["080a3d9c"]);
// This effect-dispatch wrapper's explicit entry and exit blocks are preserved
// in the reference.  Sibling-call optimization merges the final cases and
// rotates the call arguments even though no source tail call is present.
const NO_OPTIMIZE_SIBLING_CALLS_SOURCES = new Set(["080b110c"]);
// These functions construct a three-word DMA descriptor whose historical
// Thumb lowering uses one writeback STMIA and restores the descriptor base.
// This is a routing table, not a build manifest: cflagsForTargetSource feeds
// tools/decomp_diagnose.ts as well as the build, so a stem stays listed once
// the mode is evidenced even while its region is still assembly. 080170c4,
// 0800d304, 080c08a8 and 0808fecc are four such -- all pinned near-misses whose
// residual is scheduler-internal, and delisting them would make diagnose lie to
// the next agent that picks them up. 0808fecc's residual is the smallest of the
// four: the mode takes it from 26 mismatches to 4, and what remains is the
// return-value copy and the stack restore in the opposite order. 08005a78 is a
// fifth: 15 mismatches unrouted, 17 on the mode alone and 11 with sched2 also
// off, and at 11 every remaining pair is a transposition -- no wrong register
// and nothing semantic. It is listed on the mode only; -fno-schedule-insns2 is
// deliberately withheld, and the reason is narrower than it first looked. With
// sched2 off the residual falls to 10 bytes over five pairs, but those pairs are
// mutually contradictory, which is itself the proof sched2 was on. The pass does
// rewrite a correct schedule for the trailing descriptor trio -- that part holds
// -- but it is not what blocks the block-2 head: there rank_for_schedule simply
// prefers a higher-priority insn, and no source shape reorders the ready list.
// Closing 08005a78 needs a compiler change, not another routing flag. 080b5ad4
// is a sixth, and the clearest statement of the same limit: with the mode its
// descriptor store groups and its whole tail agrees, and what is left is one
// `adds r2, r4, #0`. Its control word is also the following call's third
// argument, so the pseudo has two uses and thumb_store_multiple3's hard-coded
// (reg:SI 2) makes arm_pre_reload insert a copy the reference does not have --
// value0 has a special case in that pass and value2 has none
// (work/hand/080b5ad4/NOTES.md).
const GROUPED_DMA_STORE_SOURCES = new Set([
  "08002f10", "08004838", "08004858", "080049e8", "08004a28", "08004a44",
  "08004a5c", "08004a94", "0800bc48", "0800bdd4", "0800c0f4", "0800d304", "080170c4", "08019bac",
  "0801d980",
  "080251d4", "080284dc", "080958a8", "0809bb34", "080c0184", "080c08a8",
  "0808fecc", "08004760", "08005a78", "080037d4", "080b5ad4", "0800300c", "080f377c",
  "08002fb0", "08003e10",
  "080a1090",
]);

// Nine sound-request entry wrappers: the entry pool load precedes the
// parameter copies only under the evidenced entry-literal-first mode with
// the second scheduling pass off (work/hand/compiler-cohort-integration.md).
const ENTRY_LITERAL_FIRST_SOURCES = new Set([
  "0800383c", "0800387c", "080038bc", "080038fc", "0800393c",
  "0800397c", "080039bc", "080039fc", "08003a3c",
]);
const HIGH_REGISTER_MOVE_FIRST_SOURCES = new Set(["0808b8e8", "080b6e30", "08002fb0", "08003e10", "08019bac"]);
// 08004760 is a still-assembly near-miss routed for the same reason as the
// grouped-DMA entries below: without this mode its `sub sp, #4` sinks under the
// first literal load, and with it the whole prologue and the entire tail agree.
// What is left is one register copy in the middle, so the mode is evidenced
// even though the region has not been converted yet.
const EARLY_FRAME_ALLOCATION_SOURCES = new Set(["0809802c", "08004760"]);
// These overlay-local object constructors share one exact compiler fingerprint:
// immediately before a call, the independent r0 register copy precedes the r1
// immediate. Their common filename is not unique, so routing must use the
// canonical resource path rather than the 020000a0 stem.
//
// The 02000048 block is a second constructor in the same nine objects, and it
// is the sharper evidence of the two: those nine are one duplicate-fingerprint
// family, byte-identical apart from their four `bl` encodings, and each one is
// exact with the mode and off by exactly the same four bytes without it --
// `movs r1, #14` / `adds r0, r5, #0` transposed immediately before the second
// call. Its two other calls are unaffected in both modes, so the mode moves
// that one pair and nothing else. Every 02000048 entry shares an object with
// the 020000a0 entry directly above it.
const CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES = new Set([
  "assets/code/resource_3a0_c_02000048.c",
  "assets/code/resource_3a1_c_02000048.c",
  "assets/code/resource_3a5_c_02000048.c",
  "assets/code/resource_3a6_c_02000048.c",
  "assets/code/resource_3ab_c_02000048.c",
  "assets/code/resource_3b3_c_02000048.c",
  "assets/code/resource_3be_c_02000048.c",
  "assets/code/resource_3c0_c_02000048.c",
  "assets/code/resource_3c9_c_02000048.c",
  "assets/code/resource_380_c_020000a0.c",
  "assets/code/resource_382_c_020000a0.c",
  "assets/code/resource_385_c_020000a0.c",
  "assets/code/resource_387_c_020000a0.c",
  "assets/code/resource_38a_c_020000a0.c",
  "assets/code/resource_396_c_020000a0.c",
  "assets/code/resource_39b_c_020000a0.c",
  "assets/code/resource_39c_c_020000a0.c",
  "assets/code/resource_39d_c_020000a0.c",
  "assets/code/resource_39e_c_020000a0.c",
  "assets/code/resource_3a0_c_020000a0.c",
  "assets/code/resource_3a1_c_020000a0.c",
  "assets/code/resource_3a5_c_020000a0.c",
  "assets/code/resource_3a6_c_020000a0.c",
  "assets/code/resource_3ab_c_020000a0.c",
  "assets/code/resource_3b3_c_020000a0.c",
  "assets/code/resource_3be_c_020000a0.c",
  "assets/code/resource_3c0_c_020000a0.c",
  "assets/code/resource_3c9_c_020000a0.c",
]);
// The fork's scheduler cost model gives a Thumb immediate move's result a
// ready-delay of 1, so a two-insn constant (`movs rN,#imm` / `lsls rN,rN,#k`)
// always issues back to back. These objects behave as if that result arrives a
// cycle later, which lets an independent argument setter fill the gap. Evidence
// is per-source and byte-exact: `resource_3c7:0030` is 4 mismatched bytes
// without the mode and 18/18 exact with it, moving only the `movs r0,#14` that
// the reference places between the two halves of the constant. See LAWS.md
// "Pre-call argument setters diverge on two independent `sched2` defects",
// defect (A).
// `fold` rewrites a signed `x < C` into `x <= C-1` (and `x >= C` into
// `x > C-1`) whenever C is positive, so the emitted comparison is `cmp #C-1`
// with `ble` where these objects' reference is `cmp #C` with `blt`. The
// rewrite happens at tree level, before any source shaping can reach it:
// `x < C`, `x >= C` with the arms swapped, `C <= x`, `!(x < C)` and `x > C-1`
// all fold to the same form, and the operand type does not matter.
// -fno-canonicalize-comparison suppresses it (fold-const.c, gated in the
// alchemy-gcc fork).
// Overlay streams whose constant pool sits between the entry block and the loop
// that reads it, rather than after the return (LAWS.md, "A mid-function literal
// pool is a compiler layout choice, not a source shape").
const EARLY_LITERAL_POOL_OVERLAY_SOURCES = new Set(["02000e3c", "02000dfc"]);
const NO_CANONICALIZE_COMPARISON_OVERLAY_SOURCES = new Set([
  "assets/code/resource_3a9_c_020000e4.c",
]);
const THUMB_IMMEDIATE_LATENCY_OVERLAY_SOURCES = new Set([
  "assets/code/resource_3c7_c_02000030.c",
  "assets/code/resource_3cd_c_0200004c.c",
]);
// `rank_for_schedule` breaks a tie towards the insn with more forward
// dependents; these objects fall through to the `INSN_LUID` (original order)
// tie-break instead. This is deliberately a narrow per-source list, NOT a model
// correction: gated off globally it breaks 260 of the 1239 functions that
// otherwise match, and one known function wants the rule on at one site and off
// at another. Only list a unit with an independent exact-byte proof.
// `resource_3cd:004c` is 8 mismatched bytes with `-mthumb-immediate-latency`
// alone and 36/36 exact with both; the mode moves only the two `movs r0,#13`
// argument setters the reference places ahead of their r1/r2 companions, which
// `-mcall-arg0-move-first` cannot reach because the r0 setter here is an
// immediate rather than a register move. LAWS.md, defect (B).
const NO_SCHED_DEPEND_COUNT_OVERLAY_SOURCES = new Set([
  "assets/code/resource_3cd_c_0200004c.c",
]);
// 既定ABI(標準のr4被呼出保存)で構築された収蔵ライブラリ翻訳単位。
// 証拠: r4を保存する序文は -fcall-used-r4 の下では出ない
// (割込保護記録08006a00、バイト複写08006b84、比較08006c24、
// フラッシュ書込列08006dec、LAWS.md「第四層」)。
// 同一cc1・既定フラグ。
const DEFAULT_ABI_SOURCES = new Set([
  "08006a00", "08006b84", "08006ba8", "08006c24", "08006dec", "08007098",
  "080fadf0",
]);
// The stock m4a object linked into GS1 was built with the public old_agbcc
// compiler rather than Camelot's gcc-2.96 fork. Keep adoption source-scoped:
// every listed unit must have an independent exact-byte proof.
// `08006c24` is the first non-m4a unit with that proof: it is byte-exact (66/66)
// under old_agbcc and impossible under the fork on three independently measured
// counts — REG_ALLOC_ORDER hands the block-local temps r3/r2 and leaves the
// long-lived pointers r1/r0/r4 where the reference wants r2/r3/r4; the
// loop-invariant 0xFFFF copy takes HI_REGS from *thumb_movsi_insn alternative
// 8's `*lh` constraint and lands in ip rather than r5; and arm_reorg dumps the
// minipool at the trailing post-epilogue barrier instead of the barrier after
// the unconditional branch, costing the two bytes of .align 2 padding. Its
// already-matched DEFAULT_ABI_SOURCES sibling 08006b84 also compiles exact
// under old_agbcc, so the whole default-ABI TU is likely old_agbcc; the rest
// stay on the fork until each has its own exact-byte proof.
const AGBCC_SOURCES = new Set([
  "08006a00", "08006ba8", "08006c24", "08006dec", "08007098", "080071a8",
  "080f9a50",
  "080fadf0",
  "080fa1fc", "080fa2a0", "080fa324", "080fa350", "080fa39c", "080fa3f0",
  "080fa424", "080fa458", "080fa490", "080fa514", "080fa83c", "080fa8d4", "080fa928", "080fa9a4",
  "080fa9e0", "080fab3c", "080facf8", "080fb2cc", "080fb334", "080fb3a8", "080fb430", "080fb4a4",
  "080fb670",
  "080fb6a4",
]);
// This command-table lookup is byte-identical only when the independent
// literal load precedes its adjacent index shift, matching the stock object.
const AGBCC_LITERAL_BEFORE_SHIFT_SOURCES = new Set(["080fb670"]);
// The flash reader copies its own callee's body onto the stack, so its loop
// test is duplicated above the loop at -O2 by jump.c:duplicate_loop_exit_test.
// The reference has no such guard: at optimize<2 that transform bails out
// while BLOCK notes are still present, and the value it would keep live is
// what pushes a fifth long-lived pseudo into r8 and changes the push list.
// The sector erase at 08007098 wants the same level for an unrelated reason:
// its head shift is a two-address ashlsi3 whose reference input reload
// ("adds r4, r0, #0") only survives because local-alloc declines to tie the
// shift's input and output pseudos, which it does tie at -O2.
// 08006a00 is a third distinct reason, and here -O1 is standing in for
// -fno-regmove (flag_regmove is set unconditionally at optimize>=2, and
// regmove.c returns early when it is clear). The front end narrows the
// interrupt-enable read-modify-write to a HImode BIT_IOR, so the ior's first
// operand is a subreg; regmove's forward pass skips non-REG sources, takes the
// commutative alternative instead, and renames the ior's destination into the
// shift result, transposing r0 and r1 across four instructions. No source shape
// avoids it, because the narrowing that creates the subreg is what the C
// semantics require.
const AGBCC_OPTIMIZE_O1_SOURCES = new Set(["08006a00", "08006ba8", "08007098", "080071a8", "080fa514"]);
const AGBCC_COMPARE_ONLY_AND_TST_SOURCES = new Set(["080f9a50"]);
const AGBCC_COMMUTATIVE_COPY_CONSTANT_SOURCES = new Set(["080fa514"]);
const AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES = new Set([
  "080fb2cc", "080fb334", "080fb3a8",
]);

function sourceStem(source: string): string {
  return basename(source, extname(source));
}

// An overlay candidate is verified from a work directory as <address>.c and
// installed as assets/code/<overlay>_c_<address>.c. Keying on the address makes
// the two spellings route to the same flags, so a candidate that verified exact
// still compiles the same way once adopted.
function overlayStem(source: string): string {
  const stem = sourceStem(source);
  if (/^[0-9a-f]{8}$/.test(stem)) return stem;
  return /_c_([0-9a-f]{8})$/.exec(stem)?.[1] ?? stem;
}

function sourceKey(source: string): string {
  return relative(ROOT, resolve(ROOT, source)).split(sep).join("/");
}

export function cflagsForSource(source: string): readonly string[] {
  const stem = overlayStem(source);
  const base = DEFAULT_ABI_SOURCES.has(stem)
    ? CFLAGS.filter((flag) => flag !== "-fcall-used-r4")
    : [...CFLAGS];
  return [
    ...base,
    ...(FIXED_R3_SOURCES.has(stem) ? ["-ffixed-r3"] : []),
    ...(FIXED_LR_SOURCES.has(stem) ? ["-ffixed-r14"] : []),
    ...(OPTIMIZE_O1_SOURCES.has(stem) ? ["-O1"] : []),
    ...(UNSCHEDULED_SOURCES.has(stem) ? ["-fno-schedule-insns", "-fno-schedule-insns2"] : []),
    ...(NO_CSE_FOLLOW_SOURCES.has(stem) ? ["-fno-cse-follow-jumps"] : []),
    ...(NO_GCSE_SOURCES.has(stem) ? ["-fno-gcse"] : []),
    ...(NO_EXPENSIVE_SOURCES.has(stem) ? ["-fno-expensive-optimizations"] : []),
    ...(NO_STRENGTH_REDUCE_SOURCES.has(stem) ? ["-fno-strength-reduce"] : []),
    ...(NO_CONTIGUOUS_IMMEDIATE_SOURCES.has(stem) ? ["-fno-thumb-contiguous-immediate"] : []),
    ...(NO_SCHED_DEPEND_COUNT_SOURCES.has(stem) ? ["-fno-sched-depend-count"] : []),
    ...(SPLIT_GROUP_BASE_SOURCES.has(stem) ? ["-fthumb-split-group-base"] : []),
    ...(HOIST_PARAMETER_SAVE_SOURCES.has(stem) ? ["-fthumb-hoist-parameter-save"] : []),
    ...(ENTRY_SAVES_DESCENDING_SOURCES.has(stem) ? ["-fthumb-entry-saves-descending"] : []),
    ...(GROUP_CONTROL_LAST_SOURCES.has(stem) ? ["-fthumb-group-control-last"] : []),
    ...(MOVE_BEFORE_ALU_SOURCES.has(stem) ? ["-fthumb-move-before-alu"] : []),
    ...(NO_REGMOVE_SOURCES.has(stem) ? ["-fno-regmove"] : []),
    ...(ENTRY_LITERAL_FIRST_SOURCES.has(stem)
      ? ["-fno-schedule-insns2", "-mthumb-entry-literal-first"] : []),
    ...(HIGH_REGISTER_MOVE_FIRST_SOURCES.has(stem) ? ["-mhigh-register-move-first"] : []),
    ...(EARLY_FRAME_ALLOCATION_SOURCES.has(stem) ? ["-mearly-frame-allocation"] : []),
    ...(NO_OPTIMIZE_SIBLING_CALLS_SOURCES.has(stem) ? ["-fno-optimize-sibling-calls"] : []),
    ...(GROUPED_DMA_STORE_SOURCES.has(stem) ? ["-mgrouped-dma-store"] : []),
    ...(CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-mcall-arg0-move-first"]
      : []),
    ...(THUMB_IMMEDIATE_LATENCY_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-mthumb-immediate-latency"]
      : []),
    ...(NO_CANONICALIZE_COMPARISON_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-canonicalize-comparison"]
      : []),
    ...(NO_SCHED_DEPEND_COUNT_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-sched-depend-count"]
      : []),
    ...(EARLY_LITERAL_POOL_OVERLAY_SOURCES.has(overlayStem(source))
      ? ["-mthumb-early-literal-pool"]
      : []),
  ];
}

export function cflagsForTargetSource(target: CompilerTarget, source: string): readonly string[] {
  if (target === "gs1" && AGBCC_SOURCES.has(sourceStem(source))) {
    return [
      ...AGBCC_CFLAGS,
      ...(AGBCC_OPTIMIZE_O1_SOURCES.has(sourceStem(source)) ? ["-O1"] : []),
      ...(AGBCC_LITERAL_BEFORE_SHIFT_SOURCES.has(sourceStem(source))
        ? ["-mliteral-before-shift"]
        : []),
      ...(AGBCC_COMMUTATIVE_COPY_CONSTANT_SOURCES.has(sourceStem(source))
        ? ["-mcommutative-copy-constant"]
        : []),
      ...(AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES.has(sourceStem(source))
        ? ["-mprologue-next-high-reg"]
        : []),
      ...(AGBCC_COMPARE_ONLY_AND_TST_SOURCES.has(sourceStem(source))
        ? ["-mcompare-only-and-tst"]
        : []),
    ];
  }
  return target === "gs1" ? cflagsForSource(source) : [...GS2_CFLAGS];
}

export function usesAgbccCompiler(target: CompilerTarget, source: string): boolean {
  return target === "gs1" && AGBCC_SOURCES.has(sourceStem(source));
}

// The optional trailing letter names a *second* external symbol whose value is
// the same address. The stock objects sometimes referenced one address through
// several symbols, and arm.c's minipool keeps one entry per distinct SYMBOL_REF,
// so a reconstruction that spells them all the same way collapses the duplicate
// pool words (and, with them, the blocks that load them).
const ADDRESS_SYMBOL = /^(Func|Data|Value)_([0-9a-f]{8})(?:_[a-z])?$/;
const CALL_VIA_SYMBOL = /^_call_via_r(1[0-3]|[0-9])$/;
const CALL_VIA_ALIAS = /^_call_via_(sl|fp|ip|sp)$/;
const CALL_VIA_BASE = 0x080072e4;
const CALL_VIA_REGISTERS: Record<string, number> = { sl: 10, fp: 11, ip: 12, sp: 13 };

export interface ExternalSymbol {
  address: number;
  thumb: boolean;
}

export function externalSymbol(name: string): ExternalSymbol | null {
  const addressed = name.match(ADDRESS_SYMBOL);
  if (addressed !== null) {
    return {
      address: Number.parseInt(addressed[2], 16),
      thumb: addressed[1] === "Func",
    };
  }
  const callVia = name.match(CALL_VIA_SYMBOL);
  if (callVia !== null) {
    return {
      address: CALL_VIA_BASE + Number.parseInt(callVia[1], 10) * 4,
      thumb: true,
    };
  }
  const callViaAlias = name.match(CALL_VIA_ALIAS);
  if (callViaAlias !== null) {
    return {
      address: CALL_VIA_BASE + CALL_VIA_REGISTERS[callViaAlias[1]] * 4,
      thumb: true,
    };
  }
  return null;
}

// `.thumb_func` only marks a symbol defined by a label; on a `.set` alias the
// branch type is lost, so a data reference to the stub linked to the plain even
// address. The ARM ELF ABI puts bit 0 of an ABS32 relocation against a Thumb
// function, which is what the stock ROM's own pool words carry (the flash
// reader at 08006ba8 loads 0x08006b85 and clears the tag at run time).
// `.thumb_set` is the alias form that keeps the branch type, so both the
// branch offset and the tagged data word come out right.
export function externalSymbolAssembly(name: string): string {
  const symbol = externalSymbol(name);
  if (symbol === null) throw new Error(`unsupported external symbol: ${name}`);
  const directive = symbol.thumb ? ".thumb_set" : ".set";
  return `.global ${name}\n${directive} ${name}, 0x${symbol.address.toString(16).padStart(8, "0")}\n`;
}

// The approved compiler bundle is host-specific: xgcc/cc1/cpp/tradcpp are
// native executables, not portable across host platform+arch, so each
// supported host keeps its own pinned digest set. darwin-arm64 is Pascal's
// native Apple Silicon build environment; linux-x64 is a
// from-source build of the same alchemy-gcc commit via its documented
// Ubuntu/WSL host path (alchemy-gcc/README.md `build.sh`/`stage.sh`).
// Codegen (the bytes xgcc/cc1 emit for a *target* arm7tdmi/Thumb program) is
// host-independent; a linux-x64 bundle is admitted only after the full
// existing source-only build reproduces gs1-en.gba byte-identically with it,
// exactly as any other verified assembler/toolchain substitution requires.
type HostKey = "darwin-arm64" | "linux-x64";

function hostKey(): HostKey | null {
  if (process.platform === "darwin" && process.arch === "arm64") return "darwin-arm64";
  if (process.platform === "linux" && process.arch === "x64") return "linux-x64";
  return null;
}

const EXPECTED: Record<HostKey, Record<CompilerTarget, Record<string, string>>> = {
  "darwin-arm64": {
    gs1: {
      xgcc: "9580bf21ee1828bf3ba6969ce894dfedb17569cb840ee2630199bdca7a5c59e5",
      cpp: "acf056df9321b1016afea640bac858c1cd4572f04002af356aced14e7509fae2",
      tradcpp: "086343042dd10f26c8d990b30fc9a17e17802eb0f72fed09daa979faac6cec99",
      cc1: "04fd122590413736744c54897e5f40d83604ac83f3a5ecd441c7e2aa6b85aa2a",
    },
    gs2: {
      xgcc: "128520f13ff01aee64a984b1279a6e3a682a3679de44c99296064f46fb1e8ec2",
      cpp0: "b4ac7f5ff7fd74f4eca40385832fd0360d13cb5d4f0b6c8b3ead4a67d2f3d5b0",
      tradcpp0: "7698319dfea3647dace68ffb5c3dbc0fd459f3a859699acb47c669d3eb8956a3",
      cc1: "91b2a67275a100e8b6695d85ef2d82d1fd144853cbcb361ddf1d8be31858230f",
    },
  },
  "linux-x64": {
    gs1: {
      xgcc: "2ed03493228a7873f020b16a63b89b3aadf4835be2d1a3a217cabca0fa244444",
      cpp: "06096beb427848574f610626bb53408b1a76f69b178ee2d7f0a05f6c2f6d3778",
      tradcpp: "f9b951486d4e1769e06b892a59980c91a45435559505d73039130b63d1156803",
      cc1: "0e5339f7286d409d4851a729b4c4352130aa24c21c27db214fddedc41c0716a8",
    },
    gs2: {
      xgcc: "d0b10d67bc7f9965d586eba766b77e6ca54cc791b5eb297b55a6b9b6d6d0ef3d",
      cpp0: "9d93c7762f60d13474764d2ca9e721b235ed4935ba7b69012aba054cace60d0d",
      tradcpp0: "010da8763b9ebf39cb52aab0412ca350e038ccf4c3aa5647440c2abc91dcad6c",
      cc1: "1b1c039eda51c0c2ee67d076f33e3284dd369789378cdf34671b66ba76cd6c75",
    },
  },
};

const LINUX_AGBCC_EXPECTED = "0c2d5ec04129f7b9d1ecf738f096167af152661bc2506f8fdb2749305fa3eb37";

const validated = new Set<CompilerTarget>();
let agbccValidated = false;
const AGBCC_EXPECTED = "4f7664872d10a737184fb2e0502c407c9d74505f0cff7313ba4e9083736c2207";

function outputText(value: Uint8Array): string {
  return Buffer.from(value).toString("utf8");
}

export function validateBundle(target: CompilerTarget = "gs1"): void {
  if (validated.has(target)) return;
  const host = hostKey();
  if (host === null) {
    throw new Error("alchemy-gcc requires native arm64 macOS or x64 Linux");
  }
  const bundle = bundleForTarget(target);
  for (const [name, expected] of Object.entries(EXPECTED[host][target])) {
    const path = join(bundle, name);
    let mode = 0;
    try {
      mode = statSync(path).mode;
    } catch {
      throw new Error(`alchemy-gcc ${target} bundle is missing executable ${name}`);
    }
    if ((mode & 0o111) === 0) {
      throw new Error(`alchemy-gcc ${target} bundle is missing executable ${name}`);
    }
    const actual = new Bun.CryptoHasher("sha256").update(readFileSync(path)).digest("hex");
    if (actual !== expected) {
      throw new Error(`alchemy-gcc ${target}/${name} has an unapproved digest`);
    }
  }
  // 起動確認。並列処理の前に移設済み補助実行体を一つずつ起動する。
  // 初回起動を同時に行うとmacOSの検証処理が競合する。
  const smoke = Bun.spawnSync(
    [driverForTarget(target), `-B${bundle}/`, "-S", "-x", "c", "-o", "/dev/null", "/dev/null"],
    { cwd: ROOT, stdout: "pipe", stderr: "pipe" },
  );
  if (smoke.exitCode !== 0) {
    const detail = (outputText(smoke.stderr) || outputText(smoke.stdout)).trim();
    throw new Error(`alchemy-gcc ${target} smoke compile failed: ${detail}`);
  }
  validated.add(target);
}

export function validateAgbccBundle(): void {
  if (agbccValidated) return;
  const host = hostKey();
  if (host === null) {
    throw new Error("alchemy-gcc requires native arm64 macOS or x64 Linux");
  }
  let mode = 0;
  try {
    mode = statSync(AGBCC_DRIVER).mode;
  } catch {
    throw new Error("alchemy-gcc agbcc bundle is missing executable old_agbcc");
  }
  if ((mode & 0o111) === 0) {
    throw new Error("alchemy-gcc agbcc bundle is missing executable old_agbcc");
  }
  const actual = new Bun.CryptoHasher("sha256").update(readFileSync(AGBCC_DRIVER)).digest("hex");
  const expectedAgbcc = host === "darwin-arm64" ? AGBCC_EXPECTED : LINUX_AGBCC_EXPECTED;
  if (actual !== expectedAgbcc) {
    throw new Error("alchemy-gcc agbcc/old_agbcc has an unapproved digest");
  }
  const smoke = Bun.spawnSync(
    [AGBCC_DRIVER, "/dev/null", "-mthumb-interwork", "-O2", "-o", "/dev/null"],
    { cwd: ROOT, stdout: "pipe", stderr: "pipe" },
  );
  if (smoke.exitCode !== 0) {
    const detail = (outputText(smoke.stderr) || outputText(smoke.stdout)).trim();
    throw new Error(`alchemy-gcc agbcc smoke compile failed: ${detail}`);
  }
  agbccValidated = true;
}

export function compilerCommand(...arguments_: Array<string | number>): string[] {
  return compilerCommandForTarget("gs1", ...arguments_);
}

export function compilerCommandForTarget(
  target: CompilerTarget,
  ...arguments_: Array<string | number>
): string[] {
  validateBundle(target);
  const bundle = bundleForTarget(target);
  return [driverForTarget(target), `-B${bundle}/`, ...arguments_.map(String)];
}

export function compilerCommandForTargetSource(
  target: CompilerTarget,
  source: string,
  ...arguments_: Array<string | number>
): string[] {
  if (!usesAgbccCompiler(target, source)) {
    return compilerCommandForTarget(target, ...arguments_);
  }
  validateAgbccBundle();
  return [
    AGBCC_DRIVER,
    ...arguments_.filter((argument) => argument !== "-S").map(String),
  ];
}

// Hot-search pipeline: invoke the approved preprocessor and cc1 directly, saving
// one driver process per candidate. These arguments are the exact subprocesses
// emitted by xgcc for CFLAGS; ordinary builds use the source-aware command API.
export function directPreprocessorCommand(input: string, output: string): string[] {
  validateBundle();
  return [
    join(BUNDLE, "cpp"), "-lang-c", "-nostdinc",
    "-D__GNUC__=2", "-D__GNUC_MINOR__=96", "-D__GNUC_PATCHLEVEL__=0",
    "-Acpu(arm)", "-Amachine(arm)", "-D__CHAR_UNSIGNED__", "-D__OPTIMIZE__",
    "-D__ARM_ARCH_4T__", "-D__APCS_32__", "-D__ARMEL__", "-D__THUMBEL__",
    "-Darm_elf", "-D__ELF__", "-Dthumb", "-D__thumb__", input, output,
  ];
}

export function directCompilerCommand(
  input: string,
  output: string,
  dumpbase: string,
  source = dumpbase,
): string[] {
  const stem = sourceStem(source);
  validateBundle();
  return [
    join(BUNDLE, "cc1"), input, "-quiet", "-dumpbase", dumpbase,
    "-mthumb", "-mthumb-interwork", "-mcpu=arm7tdmi", "-O2",
    "-fno-builtin", "-ffreestanding",
    ...(DEFAULT_ABI_SOURCES.has(stem) ? [] : ["-fcall-used-r4"]),
    ...(FIXED_R3_SOURCES.has(stem) ? ["-ffixed-r3"] : []),
    ...(FIXED_LR_SOURCES.has(stem) ? ["-ffixed-r14"] : []),
    ...(OPTIMIZE_O1_SOURCES.has(stem) ? ["-O1"] : []),
    ...(UNSCHEDULED_SOURCES.has(stem) ? ["-fno-schedule-insns", "-fno-schedule-insns2"] : []),
    ...(NO_CSE_FOLLOW_SOURCES.has(stem) ? ["-fno-cse-follow-jumps"] : []),
    ...(NO_GCSE_SOURCES.has(stem) ? ["-fno-gcse"] : []),
    ...(NO_EXPENSIVE_SOURCES.has(stem) ? ["-fno-expensive-optimizations"] : []),
    ...(NO_STRENGTH_REDUCE_SOURCES.has(stem) ? ["-fno-strength-reduce"] : []),
    ...(NO_CONTIGUOUS_IMMEDIATE_SOURCES.has(stem) ? ["-fno-thumb-contiguous-immediate"] : []),
    ...(NO_SCHED_DEPEND_COUNT_SOURCES.has(stem) ? ["-fno-sched-depend-count"] : []),
    ...(MOVE_BEFORE_ALU_SOURCES.has(stem) ? ["-fthumb-move-before-alu"] : []),
    ...(NO_REGMOVE_SOURCES.has(stem) ? ["-fno-regmove"] : []),
    ...(ENTRY_LITERAL_FIRST_SOURCES.has(stem)
      ? ["-fno-schedule-insns2", "-mthumb-entry-literal-first"] : []),
    ...(HIGH_REGISTER_MOVE_FIRST_SOURCES.has(stem) ? ["-mhigh-register-move-first"] : []),
    ...(EARLY_FRAME_ALLOCATION_SOURCES.has(stem) ? ["-mearly-frame-allocation"] : []),
    ...(NO_OPTIMIZE_SIBLING_CALLS_SOURCES.has(stem) ? ["-fno-optimize-sibling-calls"] : []),
    ...(CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-mcall-arg0-move-first"]
      : []),
    "-o", output,
  ];
}

export function directCompilerCommandForSource(
  source: string,
  input: string,
  output: string,
  dumpbase: string,
): string[] {
  if (!usesAgbccCompiler("gs1", source)) {
    return directCompilerCommand(input, output, dumpbase, source);
  }
  validateAgbccBundle();
  return [
    AGBCC_DRIVER, input, "-dumpbase", dumpbase,
    ...cflagsForTargetSource("gs1", source), "-o", output,
  ];
}

function selfTest(): void {
  const expected = [
    "08006a00", "08006ba8", "08006c24", "08006dec", "08007098", "080071a8",
    "080f9a50",
    "080fa1fc", "080fa2a0", "080fa324", "080fa350", "080fa39c", "080fa3f0",
    "080fa424", "080fa458", "080fa490", "080fa514", "080fa83c", "080fa8d4", "080fa928", "080fa9a4",
    "080fa9e0", "080fab3c", "080facf8", "080fadf0", "080fb2cc", "080fb334", "080fb3a8", "080fb430", "080fb4a4",
    "080fb670",
    "080fb6a4",
  ];
  if (JSON.stringify([...AGBCC_SOURCES].sort()) !== JSON.stringify(expected)) {
    throw new Error("old_agbcc source allowlist self-test failed");
  }
  for (const stem of expected) {
    const source = `/tmp/${stem}.c`;
    if (!usesAgbccCompiler("gs1", source) || usesAgbccCompiler("gs2", source)) {
      throw new Error(`old_agbcc target routing self-test failed for ${stem}`);
    }
    const expectedFlags = [
      ...AGBCC_CFLAGS,
      ...(["08006a00", "08006ba8", "08007098", "080071a8"].includes(stem) ? ["-O1"] : []),
      ...(stem === "080fa514" ? ["-O1", "-mcommutative-copy-constant"] : []),
      ...(stem === "080fb670" ? ["-mliteral-before-shift"] : []),
      ...(["080fb2cc", "080fb334", "080fb3a8"].includes(stem)
        ? ["-mprologue-next-high-reg"]
        : []),
      ...(stem === "080f9a50" ? ["-mcompare-only-and-tst"] : []),
    ];
    if (JSON.stringify(cflagsForTargetSource("gs1", source)) !== JSON.stringify(expectedFlags)) {
      throw new Error(`old_agbcc flags self-test failed for ${stem}`);
    }
  }
  if (usesAgbccCompiler("gs1", "/tmp/080000c0.c") ||
      JSON.stringify(cflagsForTargetSource("gs1", "/tmp/080000c0.c")) === JSON.stringify(AGBCC_CFLAGS)) {
    throw new Error("old_agbcc unrelated-source routing self-test failed");
  }
  const groupedDma = [...GROUPED_DMA_STORE_SOURCES].sort();
  if (JSON.stringify(groupedDma) !== JSON.stringify([
    "08002f10", "08002fb0", "0800300c", "080037d4", "08003e10", "08004760",
    "08004838", "08004858", "080049e8", "08004a28", "08004a44", "08004a5c",
    "08004a94", "08005a78", "0800bc48", "0800bdd4", "0800c0f4", "0800d304",
    "080170c4", "08019bac", "0801d980", "080251d4", "080284dc", "0808fecc", "080958a8",
    "0809bb34", "080a1090", "080b5ad4", "080c0184", "080c08a8", "080f377c",
  ])) {
    throw new Error("grouped DMA source allowlist self-test failed");
  }
  for (const stem of groupedDma) {
    if (!cflagsForTargetSource("gs1", `/tmp/${stem}.c`).includes("-mgrouped-dma-store")) {
      throw new Error(`grouped DMA flags self-test failed for ${stem}`);
    }
  }
  if (cflagsForTargetSource("gs1", "/tmp/080000c0.c").includes("-mgrouped-dma-store") ||
      cflagsForTargetSource("gs2", "/tmp/080958a8.c").includes("-mgrouped-dma-store")) {
    throw new Error("grouped DMA unrelated-source routing self-test failed");
  }
  if (!cflagsForTargetSource("gs1", "/tmp/080049e8.c").includes("-O1") ||
      cflagsForTargetSource("gs1", "/tmp/08004a28.c").includes("-O1") ||
      cflagsForTargetSource("gs1", "/tmp/08004a5c.c").includes("-O1") ||
      cflagsForTargetSource("gs2", "/tmp/080049e8.c").includes("-O1")) {
    throw new Error("grouped DMA O1 routing self-test failed");
  }
  const callArg0MoveFirstOverlays = [...CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES].sort();
  if (callArg0MoveFirstOverlays.length !== 28) {
    throw new Error("overlay call-argument source allowlist self-test failed");
  }
  for (const path of callArg0MoveFirstOverlays) {
    if (!cflagsForTargetSource("gs1", join(ROOT, path)).includes("-mcall-arg0-move-first") ||
        cflagsForTargetSource("gs2", join(ROOT, path)).includes("-mcall-arg0-move-first")) {
      throw new Error(`overlay call-argument flags self-test failed for ${path}`);
    }
  }
  if (cflagsForTargetSource("gs1", "/tmp/020000a0.c").includes("-mcall-arg0-move-first") ||
      cflagsForTargetSource("gs1", join(ROOT, "assets/code/resource_381/c/020000a0.c"))
        .includes("-mcall-arg0-move-first")) {
    throw new Error("overlay call-argument unrelated-source routing self-test failed");
  }
  console.log(`self-test=ok agbcc_sources=${expected.length} grouped_dma_sources=${groupedDma.length} overlay_call_arg_sources=${callArg0MoveFirstOverlays.length}`);
}

function main(): void {
  const argument = Bun.argv[2] ?? "gs1";
  if (argument === "--self-test") {
    selfTest();
    return;
  }
  if (argument === "agbcc") {
    validateAgbccBundle();
    console.log(`alchemy-gcc=agbcc ok host=${hostKey()} files=1 bytes=${statSync(AGBCC_DRIVER).size}`);
    return;
  }
  if (argument !== "gs1" && argument !== "gs2") {
    throw new Error(`unsupported compiler target: ${argument}`);
  }
  const target: CompilerTarget = argument;
  validateBundle(target);
  const bundle = bundleForTarget(target);
  const host = hostKey() as HostKey;
  const size = Object.keys(EXPECTED[host][target])
    .map((name) => statSync(join(bundle, name)).size)
    .reduce((sum, value) => sum + value, 0);
  const label = target === "gs1" ? "alchemy-gcc=ok" : "alchemy-gcc=gs2 ok";
  console.log(`${label} host=${host} files=${Object.keys(EXPECTED[host][target]).length} bytes=${size}`);
}

if (import.meta.main) main();
