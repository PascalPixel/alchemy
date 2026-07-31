#!/usr/bin/env bun
import { readdirSync, readFileSync, statSync } from "node:fs";
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
export const PRET_EARLY_THUMB_BUNDLE = join(BUNDLE, "pret-early-thumb");
export const PRET_EARLY_THUMB_DRIVER = join(PRET_EARLY_THUMB_BUNDLE, "cc1");
export const GCC2951_BUNDLE = join(BUNDLE, "gcc2951");
export const GCC2951_DRIVER = join(GCC2951_BUNDLE, "cc1");
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
// One overlay predicate family returns through a single `pop {pc}` where every
// other reconstructed function returns `pop {rN}` + `bx rN`. That is the
// interworking epilogue, not a scheduling or allocation difference: the whole
// body is otherwise instruction-identical, and dropping `-mthumb-interwork`
// reproduces the reference exactly. With the flag on, the two-instruction return
// overruns the 16-byte span and `overlay_adopt` rejects the placement rather
// than reporting a byte diff, which is why the shape reads as a tooling error.
//
// Scope is measured, not assumed: `pop {pc}` occurs 18 times in all unconverted
// overlay assembly against 2,195 `bx` returns, and those 18 are exactly this
// family — nine members in resource_3a7 and nine in resource_3bf, three
// `*p <= 1` / `*p == 4` / `*p == 2` triples each. So this routes a closed set,
// not an open seam; do not widen it without re-running that count.
//
// Only fifteen are listed. `overlayStem` deliberately keys routing on the bare
// address so a draft and its installed copy compile the same way — which makes
// every routing set **overlay-blind**. Three of this family's addresses are
// already occupied by unrelated, already-exact functions in other overlays
// (02001554 by resource_373, 02001740 by resource_3b2, 02005ae0 by
// resource_373), and listing them here silently recompiles those without
// interworking. That does not fail as a byte diff: it surfaces two layers away
// as `palette token plan does not reconstruct input` from build_assets on an
// overlay this change never mentions. Before adding an overlay address to any
// routing set, run `ls assets/code/*_c_<addr>.c` and confirm the only hit is the
// overlay you mean. The three excluded members stay unconverted for now; routing
// them needs an overlay-aware key, not a wider set.
const NO_INTERWORK_SOURCES = new Set([
  "0200142c", "0200143c", "0200144c",
  "02001564",
  "02001750", "02001760",
  "02005ac0", "02005ad0",
  "02005bd8", "02005be8", "02005bf8",
  "02005dd4", "02005de4", "02005df4",
]);
// Same mode, keyed by repository path instead of by stem. A `pop {pc}` epilogue
// is common in these overlays, so the offsets collide across overlays often --
// every stem above is also an implicit claim that no other overlay converts a
// row at that offset, and that claim keeps getting falsified. Prefer this set
// for anything new; the stem set stays only for its existing members.
const NO_INTERWORK_OVERLAY_SOURCES = new Set([
  // Three single-comparison predicates whose stems are already taken by
  // resource_373 and resource_3b2 rows that must keep interworking.
  "assets/code/resource_3a7_c_02001554.c",
  "assets/code/resource_3a7_c_02001740.c",
  "assets/code/resource_3bf_c_02005ae0.c",
  // Moved out of the stem set: resource_377 now has an interworking row at
  // 02001544, and the stem key would have stripped its interworking epilogue.
  "assets/code/resource_3a7_c_02001544.c",
]);
// Only the second flag does anything. The pre-reload scheduler is inert in this
// fork: 40 converted sources, including the largest, compile byte-identically
// with -fschedule-insns and with -fno-schedule-insns (measured 2026-07-26,
// work/sched_probe.ts). Every member below is carried by -fno-schedule-insns2
// alone; the first flag is kept only because removing it would rewrite the
// routed command line for sixteen already-verified regions.
const UNSCHEDULED_SOURCES = new Set([
  "08002f10", "0800307c", "08006b84", "080060e8",
  "08004198", "080042c8", "0800430c", "08004358", "0800439c", "080043e0",
  "08029274",
  "080fb714", "080fb728", "080fb73c", "080fb750", "080fb75c",
  "080fb768", "080fb77c",
]);
// This decoder has mutually exclusive switch arms that reuse the same input
// base.  Following jumps during CSE rematerializes one arm's base in r3;
// disabling that pass preserves the reference's r6 lifetime and coalescing.
const NO_CSE_FOLLOW_SOURCES = new Set(["0800f9f4"]);
// In 08006088 the post-loop CSE rerun rewrites the field9 arm's OR to read the
// return accumulator instead of the equal packed-value pseudo. That makes the
// latter die at its copy, so local allocation ties both quantities and removes
// the reference's `mov r0,r2`. Without the rerun the OR keeps packed live past
// the copy; disabling regmove as well preserves r3 for that arm's adjacent
// 0x1000 materialisation. The pair is independently exact at 96/96 bytes.
// In 080ba918 the rerun folds the terminating NULL record into a fresh zero
// return. Keeping the record pseudo lets it coalesce with the call result in r0,
// preserving the reference's defined NULL return without a redundant move.
// In 0808c30c it propagates the persistent amount through the fallback copy,
// rotating the loop allocation and changing the final in-place negate.
// A grouped descriptor transfer's third word is usually a constant-pool load into
// a pseudo, which thumb_store_multiple3 then has to copy because it hard-codes
// (reg:SI 2). The mode retargets the definition when it is a constant and nothing
// between it and the group touches r2, so the copy disappears -- the counterpart
// value0 already had. 080b5ad4 writes a three-word DMA descriptor and then passes
// the same control word as the call's third argument, which is the shape the fork
// comment calls "the last divergence in the descriptor-then-call regions".
// Off by default and routed per source like every other mode.
const GROUP_VALUE2_IN_PLACE_SOURCES = new Set(["080b5ad4"]);

const NO_RERUN_CSE_AFTER_LOOP_SOURCES = new Set([
  "08006088", "0808c30c", "080ba918", "080044d0",
]);
// 080044d0 keeps a division quotient under two names so the range comparison
// reads the copy rather than the call's return register, as the reference does.
// The rerun of CSE after loop optimisation collapses that copy pair back into
// one value and restores the return register at the comparison, which is the
// function's last differing halfword; the source-side round trip alone does not
// survive it.
// This unrolled six-item display setup shares one resource-ID base and one
// signed sentinel.  Global CSE expands them back into independent constants;
// disabling it preserves the reference's r7/r8 lifetimes.
// 080b2720 is the same tell in a copy loop: the row base is addressed once for
// the emptiness test and again for the loop preheader, and global CSE keeps the
// shared address alive across the branch instead of recomputing it inside.
// 08098c08 similarly needs its stack-vector base split from the later child
// pointer; GCSE coalesces the base into r7 and forces the loop counter into r8.
// 080981b0 has two consecutive random calls whose difference feeds a particle
// field. GCSE extends the first result through the second call and rotates the
// particle-loop allocation; disabling it retains the reference call/result
// lifetimes.
// 080b3284 keeps one message-base value across its forced, limit, and success
// branches. GCSE rematerializes the branch-specific IDs and prevents the
// reference tail merge.
// 080d40ec builds a 64-entry BGR555 palette on the stack. GCSE rotates the
// phase offsets and loop temporaries; disabling it preserves the reference's
// natural long-lived allocation and exact relocated copy call.
const NO_GCSE_SOURCES = new Set([
  "0801ed40", "080981b0", "08098c08", "080a45cc", "080b2720", "080b3284",
  "080d40ec",
]);
// This bounded-angle convergence loop only retains the reference allocation
// when GCC does not perform its expensive-expression rewrite.  The rewrite
// rotates r0-r2 and folds each signed clamp into a shorter non-reference form.
const NO_EXPENSIVE_SOURCES = new Set(["08092878"]);
// This four-step signed index loop is emitted as an ascending loop in the
// reference translation unit. Strength reduction rewrites it to a descending
// counter and changes both the allocation and loop tail.
// 080200cc was listed here speculatively while it was still unconverted assembly:
// with the flag its reconstruction is 128 bytes and 62 mismatched halfwords, and
// without it 132/132 exact. Removing the stem is what converted it. Routing keys
// per stem, so 080a9d3c (adopted, unaffected) keeps the flag.
const NO_STRENGTH_REDUCE_SOURCES = new Set(["080a9d3c"]);
// arm_reorg pulls the two halves of a split constant back together when the
// scheduler put an independent insn between them. These references want the
// insn left where it is; see alchemy-gcc 1ec1044 and work/hand/080a1090.
const NO_CONTIGUOUS_IMMEDIATE_SOURCES = new Set(["080a1090", "08005a78", "0800d304", "08019bac"]);
// The grouped transfer restores its base register, so the DMA status poll that
// follows reuses it instead of loading the pool word again the way the
// reference does. Splitting the live range is the only way to spell two
// materialisations of one constant; see alchemy-gcc ff7c566.
const SPLIT_GROUP_BASE_SOURCES = new Set(["08005a78"]);
// These references emit every parameter save before the body; ours otherwise
// leaves one after the pool load that follows it. See alchemy-gcc.
const HOIST_PARAMETER_SAVE_SOURCES = new Set(["08005340", "08005394", "080053e8", "08019bac"]);
// This effect-slot dispatcher already matches every instruction, register,
// branch, and pool position. The reference rotates the tail of its strict
// three-word Thumb minipool to the front; keep that layout fingerprint scoped
// to this source and GS1.
const MINIPOOL_TAIL_FIRST_SOURCES = new Set(["080a9aec"]);
// The reference saves the second parameter before the first; ours always follows
// parameter order. Two halfwords, and the whole difference in 08093054.
const ENTRY_SAVES_DESCENDING_SOURCES = new Set(["08093054"]);
// thumb_order_grouped_dma_store only normalises descriptor setup order when the
// three setup insns are adjacent. When the source word needs arithmetic the
// interleaved insns hide them, and the control load stays hoisted -- which also
// permutes the constant pool. See alchemy-gcc ff7c566.
// 08005c68 writes a three-word descriptor whose control word the reference
// stores last; without this the grouper emits it in source order.
const GROUP_CONTROL_LAST_SOURCES = new Set(["08005a78", "08005c68"]);
// The descriptor's base pool load wins a priority-68 ready-list tie on forward
// dependent count alone; these references break it by original order instead.
// 08021d88 likewise needs original-order tie breaking for its frame adjustment
// and two split constants; its source order then reproduces the ROM exactly.
// 080903bc needs the same tie break for one callback-literal load versus the
// preceding state-byte store; its other 101 halfwords already agree.
// 08094730 has the same scheduler tell immediately before its grouped DMA
// descriptor; original-order tie breaking closes its last transposition.
const NO_SCHED_DEPEND_COUNT_SOURCES = new Set([
  "08002fb0", "08003e10", "08005340", "08005394", "080053e8", "0800d304", "08019bac", "08021d88", "080903bc", "08094730",
  // First overlay member. §4's pool-load hoist, but the flag that reaches it is
  // this one, not -fsched-low-dest-first: the reference issues the argument
  // group's `ldr r0,[pc]` ahead of its `movs r1,#1`, and both -fsched-*-dest-first
  // leave the pair in our order. No other overlay owns 02001050.
  "02001050",
]);
// The reference issues the destination copy ahead of the following ALU work.
const MOVE_BEFORE_ALU_SOURCES = new Set([
  "08002fb0", "08003e10", "08005340", "08005394", "080053e8", "0800d304", "08019bac",
]);
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
const NO_REGMOVE_SOURCES = new Set(["08006088", "080a3d9c", "080ba918"]);
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
//   RESOLVED 2026-07-30: that compiler change was already written. The fork
// carries `flag_thumb_group_value2_in_place`, which retargets a constant value2
// definition straight into r2 so the copy disappears -- exactly the missing
// value0 counterpart this comment describes. It had never been added to any
// routing set here nor to FORK_MODES in tools/mode_sweep.ts, so no sweep could
// ever reach it. 080b5ad4 now compiles byte-exact with it (64 bytes) once its
// tail is spelled as a returned call. See GROUP_VALUE2_IN_PLACE_SOURCES below.
const GROUPED_DMA_STORE_SOURCES = new Set([
  "08005c68", "080060e8", "08002f10", "08004838", "08004858", "080049e8", "08004a28", "08004a44",
  "08004a5c", "08004a94", "08005340", "08005394", "080053e8", "0800bc48", "0800bdd4", "0800c0f4", "0800d304", "080170c4", "08019bac",
  "0801d014", "0801d980",
  "080251d4", "080284dc", "08094730", "08095160", "08095290", "080958a8", "08097540", "0809bb34", "080c0184", "080c08a8",
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
// This joined MMIO control write is exact except for one post-reload lifetime:
// the fork leaves the OR result in r2, loads the destination into r3, hoists an
// independent r6 literal, then stores. The reference keeps the result in the
// dead r3 input, reuses dead r2 for the destination, and stores before that r6
// load. The compiler mode requires this exact death-note-proven volatile-HI
// window; keep it source-scoped.
const ORR_DEAD_INPUT_REUSE_SOURCES = new Set(["08003adc"]);
// This no-argument initializer's reference fills the first global literal
// load's latency with the frame allocation and dependent load, then fills the
// table-index shift's latency with two stack initializers.  The compiler mode
// requires the exact hard-register, constant, stack-offset, and death-note
// fingerprint; keep it source-scoped.
const ENTRY_FRAME_CLUSTER_SOURCES = new Set(["0801c34c"]);
// This interrupt-mask helper already has the reference's source order once the
// second scheduler is disabled, except for one independent default-handler
// literal load.  The strict post-reload mode recognizes the exact r1/r2/r3
// constant-pool/shift/store window and uses the reference's latency slot.
const LITERAL_BEFORE_INDEX_SHIFT_SOURCES = new Set(["0800307c"]);
// These initializers materialize a low-register constant, preserve it in a
// saved high register, and independently initialize a saved-low counter.
// Their references fill the dependency slot with that counter initialization;
// 080ba918 also places two independent parameter saves in the bounded run.
// The broad immediate-latency mode perturbs unrelated call arguments, so route
// only the strict arm_reorg fingerprint.
const LOW_CONSTANT_BEFORE_HIGH_MOVE_SOURCES = new Set(["080a5614", "080ba918"]);
// This parameterized effect initializer naturally reaches the reference's
// complete allocation and instruction stream before sched2 transposes one
// saved-zero copy with the stack word it also initializes. The compiler mode
// requires the exact stack-pointer/zero/store/high-copy fingerprint and moves
// the low-register death note to the restored last use.
const HIGH_MOVE_BEFORE_STACK_STORE_SOURCES = new Set(["08095290"]);
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
  // resource_3bf:1cf0 places the arg0 register move ahead of its immediate
  // companions at `Func_0200742a(arg0, 257, 60)`; byte-exact (112/112) under
  // the mode, 2 halfwords without it. Scoped argument locals and a
  // prototype-less callee were tried and rejected, and the mode does not fix
  // its sibling 1c4c, so this stays a per-function route
  // (notes/resource_3bf-1cf0.md).
  "assets/code/resource_3bf_c_02001cf0.c",
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
// Path-scoped members: the 02000ee0 stem collides with default-flag
// adoptions in other overlays, so the resource_394 and resource_3bd members
// are routed by full path instead.
const EARLY_LITERAL_POOL_OVERLAY_PATHS = new Set([
  "assets/code/resource_394_c_02000ee0.c",
  "assets/code/resource_3bd_c_02000ee0.c",
]);
const NO_CANONICALIZE_COMPARISON_OVERLAY_SOURCES = new Set([
  "assets/code/resource_3a9_c_020000e4.c",
]);
// This main-ROM routine has one independent entry-window ordering residual:
// after `movs r3,#1`, the reference materializes the stack selection pointer
// before moving the loop count into r8. The existing latency mode moves exactly
// that pair and makes the natural typed source 144/144 bytes exact. Keep the
// broadly disruptive scheduler model change source-scoped.
const THUMB_IMMEDIATE_LATENCY_SOURCES = new Set(["080babdc"]);
// Main-image members of the ascending-destination-register tie-break. The
// high-register form covers ties with no call in them: a loop preheader's run of
// loop-setup copies (08098954 and its identical sibling 0809a294), and a
// parameter save at function entry whose position assign_parms fixes (08097540,
// which needs both directions and keeps its existing grouped-DMA route).
// Do NOT add the low-register form to 08098954/0809a294 — it regresses them.
// See docs/compiler-evidence/sched-high-dest-first.diff.
const SCHED_HIGH_DEST_FIRST_SOURCES = new Set(["08098954", "0809a294", "08097540"]);
// 02_ is the first overlay member, and it is §7's third residual rather than the
// r0-r3 ordinal tie-break: the reference defers a `negs r2,r2` behind the whole
// argument group where we keep it adjacent to the `movs r2,#16` that feeds it.
// -fsched-high-dest-first and -fno-sched-depend-count both move the pair but
// land it one slot early, so this is the only flag that reaches it. No other
// overlay owns 020011bc. Its sibling `02000178` has the same residual and is
// *not* routable: resource_3ba already owns that address, and §7's routing key
// is the bare address.
// Keyed by stem, so every member here is also a claim that no other overlay has
// a converted row at the same offset. 02001984 was moved out to the path-keyed
// set below when resource_3b4 gained a row at that offset that the flag breaks.
const SCHED_LOW_DEST_FIRST_SOURCES = new Set(["08097540", "020011bc", "02001958", "02000260", "020011d8", "0200028c"]);
const THUMB_IMMEDIATE_LATENCY_OVERLAY_SOURCES = new Set([
  // resource_3b7:0154 and :0178 are the same four-call sheet over two ids. The
  // third call takes -1, built as movs #1 then negs, and the reference sets the
  // second argument between those two halves; only the latency mode reproduces
  // that split. Neither tie-break direction reaches it.
  "assets/code/resource_3b7_c_02000154.c",
  "assets/code/resource_3b7_c_02000178.c",
  "assets/code/resource_383_c_02000428.c",
  // Paired with the callee-return-type lever: the return type fixed these
  // functions' movs/movs swaps and the latency mode their movs/lsls ones —
  // neither reaches zero alone.
  "assets/code/resource_3b1_c_02000670.c",
  "assets/code/resource_372_c_0200173c.c",
  "assets/code/resource_3bf_c_02004794.c",
  "assets/code/resource_37a_c_02001380.c",
  "assets/code/resource_37a_c_02001790.c",
  "assets/code/resource_37a_c_02002924.c",
  "assets/code/resource_372_c_020016cc.c",
  "assets/code/resource_399_c_020005dc.c",
  "assets/code/resource_399_c_02000a3c.c",
  "assets/code/resource_399_c_02000abc.c",
  "assets/code/resource_3c7_c_02000030.c",
  "assets/code/resource_3cd_c_0200004c.c",
  // resource_3b8:049c is byte-exact only with immediate-latency AND the
  // rerun-cse-after-loop rerun disabled together (notes/resource_3b8-049c.md).
  "assets/code/resource_3b8_c_0200049c.c",
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
// The resource_399 argument-setup family and resource_3ce:0244 share the
// 3cd:004c shape: one movs argument pair per call site transposed by the
// dependent-count tie-break, exact under the original-order tie-break alone
// (or, for 399:05dc and 0a3c, together with the routed immediate-latency
// mode). Each entry has its own exact-byte proof.
const NO_SCHED_DEPEND_COUNT_OVERLAY_SOURCES = new Set([
  // resource_398:04b4 sets r0 then r1 for a six-argument call; the dependence
  // count reverses that pair and neither tie-break direction restores it.
  "assets/code/resource_398_c_020004b4.c",
  "assets/code/resource_3c8_c_0200096c.c",
  "assets/code/resource_3c5_c_02000cf0.c",
  "assets/code/resource_3b2_c_02000da4.c",
  "assets/code/resource_37a_c_02001790.c",
  "assets/code/resource_399_c_0200021c.c",
  "assets/code/resource_3ca_c_020010d4.c",
  "assets/code/resource_399_c_02000254.c",
  "assets/code/resource_399_c_020005dc.c",
  "assets/code/resource_399_c_02000608.c",
  "assets/code/resource_399_c_02000668.c",
  "assets/code/resource_399_c_02000688.c",
  "assets/code/resource_399_c_02000a3c.c",
  "assets/code/resource_3cd_c_0200004c.c",
  "assets/code/resource_3ce_c_02000244.c",
]);
// In resource_37a:0054 the cse rerun after the copy loop folds the shared
// window base back into per-site constants; in resource_399:0abc it rewrites
// the loop-carried accumulator's equivalence the same way. Both references
// keep the first-pass lifetimes, and both sources are byte-exact with the
// rerun disabled (0abc together with the routed immediate-latency mode).
// resource_383:082c keeps three branch-local resource IDs alive through a
// four-way dispatch. Path-following cse (and its skip-blocks variant) folds
// them into the shared head, rotating the callee-saved allocation; with both
// followed-path cse passes off the source is byte-exact.
// resource_3ca:10d4 is the canonical single-descriptor DMA zero-fill: the
// grouped-store mode forms its stmia and the original-order tie-break places
// the descriptor inputs; both modes carry existing evidence and the pairing
// is byte-exact for this source alone.
const GROUPED_DMA_STORE_OVERLAY_SOURCES = new Set([
  "assets/code/resource_3ca_c_020010d4.c",
  "assets/code/resource_394_c_02000f34.c",
  "assets/code/resource_394_c_02000f54.c",
  "assets/code/resource_394_c_02000fb4.c",
]);
const NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES = new Set([
  "assets/code/resource_383_c_0200082c.c",
  "assets/code/resource_3b1_c_02000b84.c",
  "assets/code/resource_3b1_c_02000cc8.c",
]);
// resource_3c9 field/`(u16*)` mixed-access family: with strict aliasing, our
// scheduler treats the u16-view store as independent of the struct-field
// re-read and sinks the load below the store pair; the reference keeps the
// original order. Four functions in the overlay are byte-exact with alias
// analysis conservative (-fno-strict-aliasing) and mismatch without it
// (notes/resource_3c9-{0104,215c,3600}.md); each entry carries its own
// exact-byte proof.
// The reference objects rematerialize a two-instruction Thumb immediate
// (`movs rN,#K` then `lsls rN,rN,#n`, or `movs`/`negs` for a negatable value)
// independently at every call site. `cse_insn` records the register just loaded
// with such a constant in the constant's own equivalence class, so `insert_regs`
// merges the quantities of every register holding it, `canon_reg` rewrites the
// later loads, and the value ends up shared in a callee-saved register that also
// changes the prologue. `arm_rtx_costs` prices the constant at
// COSTS_N_INSNS(2), which is why one-instruction immediates never share.
// -fno-cse-two-insn-immediate suppresses only that sharing (mechanism, RTL
// evidence, prototype, and the 109-source collateral measurement are in
// docs/compiler-evidence/). It must stay per-source: enabled globally it changes
// 109 of the 1,335 gcc296-routed sources. Literal-pool constants are
// deliberately excluded, since a pool load is one instruction and sharing it is
// not a size change.
const NO_CSE_TWO_INSN_IMMEDIATE_OVERLAY_SOURCES = new Set([
  // 0xC000 appears at two of this call sheet's three sites, so CSE hoists it
  // into a callee-saved register and buys a prologue the reference does not
  // have. Paired with -fsched-low-dest-first, which orders the r0 setter.
  "assets/code/resource_38d_c_02001984.c",
  // Same shape: 0x80000 feeds both of this call's shifted arguments.
  "assets/code/resource_3b4_c_020011d8.c",
  // Same -1 pair shape as the entry two lines below; see the tie-break set.
  "assets/code/resource_3a2_c_020008a8.c",
  // Both negated arguments are -1, so CSE builds one and copies it.
  "assets/code/resource_3b5_c_0200028c.c",
  "assets/code/resource_372_c_02000f38.c",
  "assets/code/resource_3bf_c_02000bec.c",
  "assets/code/resource_3af_c_02001a98.c",
  "assets/code/resource_3af_c_02004218.c",
  // Parked before the mode existed, byte-exact under it with its existing draft
  // and no further source work.
  "assets/code/resource_3c8_c_020009c8.c",
  // Paired with -fsched-low-dest-first below: removing the constant sharing
  // exposes a scheduling transposition that the tie-break then fixes.
  "assets/code/resource_373_c_020031b4.c",
  "assets/code/resource_3c5_c_02001158.c",
  "assets/code/resource_3c5_c_02000eac.c",
  "assets/code/resource_3a8_c_0200158c.c",
  "assets/code/resource_3a8_c_020015b4.c",
  "assets/code/resource_374_c_02000780.c",
  "assets/code/resource_39c_c_020010c0.c",
  "assets/code/resource_39e_c_0200071c.c",
  "assets/code/resource_3af_c_02001f90.c",
  "assets/code/resource_380_c_02000390.c",
  "assets/code/resource_373_c_020032b0.c",
  "assets/code/resource_3bf_c_02004704.c",
  "assets/code/resource_3bf_c_020049a0.c",
  "assets/code/resource_3bf_c_0200169c.c",
  "assets/code/resource_3bf_c_0200269c.c",
  "assets/code/resource_3bf_c_02002718.c",
  "assets/code/resource_3bf_c_020025f8.c",
  "assets/code/resource_3bf_c_020021c4.c",
  "assets/code/resource_3b0_c_020004b0.c",
  "assets/code/resource_39e_c_02000414.c",
  "assets/code/resource_372_c_02003c48.c",
  "assets/code/resource_3b8_c_02003e40.c",
  "assets/code/resource_3a4_c_02000c9c.c",
  "assets/code/resource_3af_c_02001b58.c",
  "assets/code/resource_3af_c_020019c0.c",
  "assets/code/resource_3af_c_020012f0.c",
  "assets/code/resource_3af_c_02002b7c.c",
  "assets/code/resource_3ba_c_02000974.c",
  // Four call sites share one 0xc000, built as `movs #192` + `lsls #8`. CSE
  // parks it in a callee-saved register and the tell is the prologue: the
  // reference pushes {r5, lr} and we push {r5, r6, lr}. The sibling at 0x140c
  // has four *different* constants and needs only the scheduler tie-break.
  "assets/code/resource_3aa_c_02001450.c",
]);
// Every edge into a CALL_INSN costs 1, so a call's argument setters tie in
// `rank_for_schedule` on priority, insn class and forward-dependent count alike,
// and the fork falls through to the `INSN_LUID` tie-break. It then emits a
// two-instruction immediate's `lsls` where the reference emits an independent
// `movs` for a lower argument register. The mode adds a final tie-break
// preferring the lower-numbered destination register, restricted to insns that
// have a CALL_INSN among their forward dependents. Do NOT combine with
// -mthumb-immediate-latency, which subsumes and then breaks these
// (docs/compiler-evidence/sched-and-pre-modes.diff).
const SCHED_LOW_DEST_FIRST_OVERLAY_SOURCES = new Set([
  // resource_3aa:140c and :1450 are the sibling pair in HANDOVER 0: four
  // three-argument setter calls, then a guarded wait. Void callees put r0 and
  // r2 in the wrong order at every site and the low-destination tie-break puts
  // them back. :1450 needs -fno-cse-two-insn-immediate as well, because its
  // four constants are the same 0xc000; :140c's four differ and it does not.
  "assets/code/resource_3aa_c_0200140c.c",
  "assets/code/resource_3aa_c_02001450.c",
  // resource_3aa:0184 is the same tell across sixteen call sites: the setters
  // for r0/r1/r2 tie and the reference orders them by ascending destination.
  "assets/code/resource_3aa_c_02000184.c",
  // Moved here from the stem-keyed set: resource_3b4 now has a row at 02001984
  // whose six-argument call wants the reference's own order, and the stem key
  // would have applied resource_38d's flag to it.
  "assets/code/resource_38d_c_02001984.c",
  // resource_3bf:0c78 sets r0, r1 and r2 for one call and the reference orders
  // them by ascending destination; without the tie-break r0 lands last.
  "assets/code/resource_3bf_c_02000c78.c",
  // resource_39a:17a8 sets r0/r1/r2 at two three-argument calls and the
  // reference orders them by ascending destination at both.
  "assets/code/resource_39a_c_020017a8.c",
  // resource_383:19a4 sets r0/r1/r2 at two three-argument calls; ascending
  // destination order at both.
  "assets/code/resource_383_c_020019a4.c",
  "assets/code/resource_3bf_c_02000bac.c",
  // resource_3b6:0898 sets r0/r1/r2 for a three-argument call; ascending
  // destination order.
  "assets/code/resource_3b6_c_02000898.c",
  // resource_3b5:0568 is the same three-argument setter shape.
  "assets/code/resource_3b5_c_02000568.c",
  // resource_3b5:0224 sets r0/r1/r2 for a four-argument call; ascending
  // destination order.
  "assets/code/resource_3b5_c_02000224.c",
  "assets/code/resource_3b5_c_02000528.c",
  // resource_3a2:0870 and :08a8 each set r0/r1/r2 for a three-argument call and
  // the reference orders them by ascending destination. :08a8 also needs the
  // constant-sharing mode: both of its negated arguments are -1, so CSE builds
  // one and copies it where the reference negates each in place.
  "assets/code/resource_3a2_c_02000870.c",
  "assets/code/resource_3a2_c_020008a8.c",
  "assets/code/resource_3c8_c_02001780.c",
  "assets/code/resource_3c8_c_02001150.c",
  "assets/code/resource_372_c_02000f38.c",
  // Needs the tie-break alone, without the paired constant-sharing mode: two
  // argument setters tie before a `bl` and the low-destination rule picks
  // `mov r0,sl` over `lsls r1,r1,#1` (notes/resource_381-0e30.md).
  "assets/code/resource_381_c_02000e30.c",
  "assets/code/resource_3c5_c_02001030.c",
  "assets/code/resource_38f_c_020002b4.c",
  "assets/code/resource_3a8_c_02001ed8.c",
  "assets/code/resource_3a8_c_02000504.c",
  "assets/code/resource_3a8_c_02000464.c",
  "assets/code/resource_374_c_02000750.c",
  "assets/code/resource_374_c_020007ec.c",
  "assets/code/resource_374_c_0200081c.c",
  "assets/code/resource_374_c_0200084c.c",
  "assets/code/resource_374_c_0200087c.c",
  "assets/code/resource_374_c_020008ac.c",
  "assets/code/resource_374_c_02000780.c",
  "assets/code/resource_3b2_c_02001494.c",
  "assets/code/resource_39e_c_0200071c.c",
  "assets/code/resource_3af_c_02001f90.c",
  "assets/code/resource_380_c_02000390.c",
  "assets/code/resource_373_c_020032b0.c",
  "assets/code/resource_3bf_c_02004704.c",
  "assets/code/resource_3bf_c_020049a0.c",
  "assets/code/resource_3bf_c_0200269c.c",
  "assets/code/resource_3bf_c_02002718.c",
  "assets/code/resource_3bf_c_020025f8.c",
  "assets/code/resource_3bf_c_020021c4.c",
  "assets/code/resource_3b0_c_02000468.c",
  "assets/code/resource_399_c_020019bc.c",
  "assets/code/resource_39c_c_02000ffc.c",
  "assets/code/resource_39c_c_02002e6c.c",
  "assets/code/resource_39e_c_02000658.c",
  "assets/code/resource_3af_c_02000c28.c",
  "assets/code/resource_372_c_02001600.c",
  "assets/code/resource_3b0_c_020004b0.c",
  "assets/code/resource_372_c_02000398.c",
  "assets/code/resource_372_c_020003cc.c",
  "assets/code/resource_372_c_02000400.c",
  "assets/code/resource_3b8_c_020003b0.c",
  "assets/code/resource_3b8_c_02003d40.c",
  // resource_38c:01a8 and :0430 are the same beat over actors 21 and 23: the
  // last call takes (id, 192 << 8, 10), so the `lsls` that finishes r1 ties
  // with the `movs` that sets r0, and the reference takes r0 first.
  "assets/code/resource_38c_c_020001a8.c",
  "assets/code/resource_38c_c_02000430.c",
  // Same tell twice in resource_38c:035c, whose other lever is the rerun-cse
  // one; the flags are independent and both are needed.
  "assets/code/resource_38c_c_0200035c.c",
  // resource_38c:0124, :01e0 and :0250 close the same overlay's family; each has
  // one `(id, 128 << k, n)` call where the finishing `lsls` ties with the `movs`
  // that sets r0. Their other half is a declaration, not a flag: the two-argument
  // callee that follows the single-argument (10) call returns a value, so its
  // setters leave r1 before r0 (HANDOVER 4, first lever).
  "assets/code/resource_38c_c_02000124.c",
  "assets/code/resource_38c_c_020001e0.c",
  "assets/code/resource_38c_c_02000250.c",
  "assets/code/resource_3b8_c_02003df8.c",
  "assets/code/resource_3b8_c_02003e40.c",
  // resource_373: this overlay's real fingerprint is the low-dest tie-break,
  // not the rerun-cse mode (measured neutral on ~15 functions here).
  "assets/code/resource_373_c_02000dc0.c",
  "assets/code/resource_373_c_02000e54.c",
  "assets/code/resource_373_c_02000e84.c",
  "assets/code/resource_373_c_02000f5c.c",
  "assets/code/resource_373_c_02000f8c.c",
  "assets/code/resource_373_c_02000fbc.c",
  "assets/code/resource_373_c_02000fec.c",
  "assets/code/resource_373_c_02001490.c",
  // resource_37b members: routed per function, NOT overlay-wide — this mode
  // regresses that overlay's three large word-store sheets (02001c14, 02001d14,
  // 02001e10), which are exact at default flags.
  "assets/code/resource_37b_c_020015d4.c",
  "assets/code/resource_37b_c_020015fc.c",
  "assets/code/resource_37b_c_0200101c.c",
  "assets/code/resource_37b_c_0200166c.c",
  "assets/code/resource_37b_c_020016a4.c",
  "assets/code/resource_37b_c_0200195c.c",
  "assets/code/resource_3a4_c_02000c9c.c",
  "assets/code/resource_394_c_020008b0.c",
  "assets/code/resource_3b8_c_02000264.c",
  "assets/code/resource_373_c_020031b4.c",
  "assets/code/resource_3af_c_02001b58.c",
  "assets/code/resource_3af_c_020019c0.c",
  "assets/code/resource_3af_c_020012f0.c",
  "assets/code/resource_3af_c_02002b7c.c",
  "assets/code/resource_3ba_c_02000974.c",
]);
// The fork proves a store and a later load at two different constant offsets off
// one base independent, leaves no edge between them, and lets the load's longer
// dependence chain outrank the store; the reference keeps source order. The mode
// forces the conflict when neither MEM is RTX_UNCHANGING_P, adding the edge as
// REG_DEP_ANTI so it orders without adding cost — a true dependence lengthens
// the store's path to the block end and regresses resource_381:2e0c.
// Keyed by repository path, so a main-image `src/` source routes here too — the
// name predates the first main-image member. `08078144` stores a halfword and
// then reloads a word at a different constant offset off one base; the fork
// proves them independent and hoists the load, where the reference keeps source
// order.
const NO_SCHED_ALIAS_OVERLAY_SOURCES = new Set([
  "src/08078144.c",
  "assets/code/resource_3af_c_02002b7c.c",
  "assets/code/resource_3b0_c_02000030.c",
  "assets/code/resource_381_c_02002e0c.c",
  "assets/code/resource_381_c_02002e5c.c",
]);
// gcse's partial-redundancy elimination inserts a load the reference does not
// have. The mode drops the insert and delete bits of any expression that reads
// non-RTX_UNCHANGING_P memory and would need an insertion, clearing both maps as
// a pair because PRE only deletes an occurrence that an insertion made
// available. Constant-pool loads keep their bits and are still eliminated. This
// is the narrowest of the four gates: 9 of 1,335 sources change.
// Keyed by repository path, so a main-image `src/` source routes here too; the
// name predates the first main-image member. 0807a550 is that member: gcse's
// partial-redundancy pass inserts a load the reference does not have, and the
// remaining seven halfwords vanish with the insert suppressed.
const NO_GCSE_INSERT_LOAD_OVERLAY_SOURCES = new Set([
  "src/0807a550.c",
  "assets/code/resource_37a_c_02000d9c.c",
]);
// A store has no value for a later insn to consume, so it reaches the block end
// over a zero-cost ordering edge and takes the block's minimum priority, sinking
// behind every arithmetic insn that still has a chain. The mode saturates a
// store's effective priority so stores rank alike and above non-stores, leaving
// store-versus-store to the existing rules. Including loads in the predicate
// raises collateral from 308 to 498 sources with no further gain.
const SCHED_STORE_FIRST_OVERLAY_SOURCES = new Set([
  "assets/code/resource_373_c_020032b0.c",
  "assets/code/resource_3bd_c_02000a54.c",
]);
// The pool-word sibling of the mode above, and a different kind of defect: for a
// two-instruction constant the cost model is right and only the reference's
// preference differs, but `arm_rtx_costs` prices a literal-pool constant at
// COSTS_N_INSNS(3) when `*thumb_movsi_insn` emits a single `ldr rN,[pc,#K]` —
// wrong by 3x — so every repeated pool word is shared in a callee-saved register
// and the prologue changes with it. The reference reloads the pool word per site.
// A function whose reference BOTH reloads some pool words and keeps another in a
// register to derive a related value by an add cannot be matched by this gate,
// because related-value reuse needs exactly the recording the flag suppresses
// (measured on resource_373:2cb0 — do not re-attack it with a whole-function
// flag). docs/compiler-evidence/cse-pool-immediate.diff.
const NO_CSE_POOL_IMMEDIATE_OVERLAY_SOURCES = new Set([
  "assets/code/resource_372_c_02000f38.c",
  "assets/code/resource_37b_c_02001b44.c",
  "assets/code/resource_3c5_c_020024d0.c",
  "assets/code/resource_3c5_c_02002548.c",
  "assets/code/resource_3a8_c_02001ed8.c",
  "assets/code/resource_374_c_02000634.c",
  "assets/code/resource_3af_c_02001f90.c",
  "assets/code/resource_380_c_02000390.c",
  "assets/code/resource_3bf_c_020049a0.c",
  "assets/code/resource_3bf_c_02002308.c",
  "assets/code/resource_3bf_c_0200238c.c",
  "assets/code/resource_372_c_02001600.c",
  "assets/code/resource_3b8_c_02003f84.c",
  "assets/code/resource_37b_c_0200195c.c",
  "assets/code/resource_37b_c_02002244.c",
  "assets/code/resource_37b_c_020022f4.c",
  "assets/code/resource_3a4_c_02000c9c.c",
  "assets/code/resource_394_c_020008b0.c",
  "assets/code/resource_3b8_c_02000264.c",
  "assets/code/resource_3bf_c_02004bfc.c",
  "assets/code/resource_39c_c_020014cc.c",
  "assets/code/resource_3bf_c_0200175c.c",
]);
const NO_STRICT_ALIASING_OVERLAY_SOURCES = new Set([
  "assets/code/resource_380_c_02000104.c",
  "assets/code/resource_39c_c_02000104.c",
  "assets/code/resource_39e_c_02000104.c",
  "assets/code/resource_39c_c_020003a8.c",
  "assets/code/resource_3c9_c_02000104.c",
  "assets/code/resource_3c9_c_0200215c.c",
  "assets/code/resource_3c9_c_020021ac.c",
  "assets/code/resource_3c9_c_02003600.c",
]);
const NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES = new Set([
  // resource_38c:035c tests one event flag and then sets the same flag id, so
  // 768 is built twice; the rerun caches it in r5 across the guarded block and
  // turns the leaf's `push {lr}` into `push {r5, lr}`. Paired with the
  // low-destination tie-break for its two `(id, 192 << 8, n)` calls.
  "assets/code/resource_38c_c_0200035c.c",
  "assets/code/resource_37a_c_02000054.c",
  "assets/code/resource_37a_c_02000108.c",
  "assets/code/resource_37a_c_02000150.c",
  "assets/code/resource_37a_c_020001ec.c",
  "assets/code/resource_37a_c_02001a58.c",
  "assets/code/resource_373_c_02000ba8.c",
  // Third and fourth members of this overlay's rerun-cse family: the rerun caches
  // twice- and thrice-used pool constants in callee-saved registers where the
  // reference rematerializes them per use.
  "assets/code/resource_373_c_02005950.c",
  "assets/code/resource_373_c_02005a40.c",
  "assets/code/resource_399_c_02000abc.c",
  "assets/code/resource_3b8_c_0200049c.c",
  // resource_3a4 status-window family: default flags CSE-hoist a thrice-used
  // pool constant into r5; the reference keeps first-pass lifetimes. Each
  // entry byte-exact under the flag alone (notes/resource_3a4-*.md).
  "assets/code/resource_3a4_c_020009ec.c",
  "assets/code/resource_3a4_c_02000a94.c",
  "assets/code/resource_3a4_c_02000b3c.c",
  "assets/code/resource_3a4_c_02000bd8.c",
  // resource_39c:1c9c and 1d3c both load the pool constant 0x256 at two sites,
  // one of them in the entry block, so the entry-hoisted-local lever cannot
  // reach them and a &Value_ spelling is CSEd exactly like the const_int. Both
  // are byte-exact under the flag alone.
  "assets/code/resource_39c_c_02001c9c.c",
  "assets/code/resource_39c_c_02001d3c.c",
  // resource_3ba:0540 shares its 0x301 argument between the entry-block call
  // and the else-branch call once cse reruns; the reference keeps both sites
  // independent. Byte-exact (752/752) under the flag alone
  // (notes/resource_3ba-0540.md).
  "assets/code/resource_3ba_c_02000540.c",
  // resource_3a7:03e0 is the same twice-used pool constant tell: the guard call
  // and the body call both take 0x9a9, and the rerun parks it in r5 across the
  // two conditional branches instead of reloading it. Byte-exact under the flag
  // alone. It has to be routed by path, not by stem: resource_3a4_c_020003e0.c
  // is already exact and shares the stem.
  "assets/code/resource_3a7_c_020003e0.c",
  // resource_3cd:00c0 needs this alongside -fno-gcse; see NO_GCSE_OVERLAY_SOURCES.
  "assets/code/resource_3cd_c_020000c0.c",
  // resource_3b4:1070 loads its 0x9c4 request id at the guard call and again
  // at the commit call; the rerun keeps it in r5 across both and costs the
  // prologue a register the reference does not push.
  "assets/code/resource_3b4_c_02001070.c",
  // resource_37f:056c loads its 0x302 and 0x303 ids at two sites each; the
  // rerun parks both in callee-saved registers and buys two prologue pushes.
  "assets/code/resource_37f_c_0200056c.c",
  "assets/code/resource_37f_c_020005ac.c",
  "assets/code/resource_37f_c_020005ec.c",
  "assets/code/resource_37f_c_02000634.c",
  "assets/code/resource_37f_c_0200067c.c",
  "assets/code/resource_37f_c_020006c4.c",
  "assets/code/resource_37f_c_0200070c.c",
  // resource_38d:01b4 loads its 0x302 flag id at the test and again at the set.
  "assets/code/resource_38d_c_020001b4.c",
  "assets/code/resource_38d_c_0200028c.c",
  // resource_3a7:0368 loads its 0x9a9 request id at the guard call and again
  // at the commit call, the same shape as :03e0 in this overlay.
  "assets/code/resource_3a7_c_02000368.c",
  // resource_3a7:04d0 loads its 0x9aa request id at the guard and again at
  // the commit, the third row in this overlay with that shape.
  "assets/code/resource_3a7_c_020004d0.c",
  "assets/code/resource_3a7_c_0200048c.c",
]);
// -fno-gcse routed by path rather than by stem, for overlay rows whose address
// is also an offset in another overlay that is already converted.
const NO_GCSE_OVERLAY_SOURCES = new Set([
  // resource_3cd:00c0 fills a 16-halfword stack list through a call and then
  // walks it. Both the call argument and the walk's induction base are the
  // frame address, and the reference materialises `mov rX, sp` twice -- once
  // for the argument, once in the loop preheader. The pair of passes merges
  // them into one callee-saved copy, which costs an extra `mov` and is the
  // row's only divergence; it needs -fno-rerun-cse-after-loop alongside, as
  // either flag alone leaves the merge in place. The result is insensitive to
  // how the buffer is spelled -- plain array, struct wrapper, or byte buffer
  // cast to u16 all land.
  "assets/code/resource_3cd_c_020000c0.c",
]);
// 既定ABI(標準のr4被呼出保存)で構築された収蔵ライブラリ翻訳単位。
// 証拠: r4を保存する序文は -fcall-used-r4 の下では出ない
// (割込保護記録08006a00、バイト複写08006b84、比較08006c24、
// フラッシュ書込列08006dec、LAWS.md「第四層」)。
// 同一cc1・既定フラグ。
const DEFAULT_ABI_SOURCES = new Set([
  "08006a00", "08006b84", "08006ba8", "08006c24", "08006dec", "08007098",
  "080fada0", "080fadf0",
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
// under old_agbcc. 08006c68, 08006f84, 0800711c, and 08007220 now independently
// extend that proof through the same flash cohort (116/116, 164/164, 140/140,
// and 196/196 bytes). The 280-byte CGB initializer at 080fa55c also has an
// independent exact stock-compiler proof; the rest stay on the fork until
// exact-byte proof.
const AGBCC_SOURCES = new Set([
  "08006a00", "08006ba8", "08006c24", "08006c68", "08006cdc", "08006d50", "08006dec",
  "08006e24", "08006f84", "08007028", "08007098", "0800711c", "080071a8", "08007220",
  "080f9a50",
  "080fada0", "080fadf0",
  "080fa1fc", "080fa2a0", "080fa324", "080fa350", "080fa39c", "080fa3f0",
  "080fa424", "080fa458", "080fa490", "080fa514", "080fa55c", "080fa6a0", "080fa83c", "080fa8d4", "080fa928", "080fa9a4",
  "080fa9e0", "080fab3c", "080fab7c", "080fac44", "080facf8", "080fb2cc", "080fb334", "080fb3a8", "080fb430", "080fb4a4",
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
// semantics require. 08006c68 repeats the copied-body loop fingerprint above;
// 08006f84's flash-byte loop, 0800711c's retry CFG, and 08007220's nested
// program-sector retry loop likewise reproduce their reference layouts only
// at -O1.
const AGBCC_OPTIMIZE_O1_SOURCES = new Set([
  "08006a00", "08006ba8", "08006c68", "08006cdc", "08006d50", "08006e24", "08006f84",
  "08007028", "08007098", "0800711c", "080071a8", "08007220", "080fa514",
]);
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
  const abiBase = DEFAULT_ABI_SOURCES.has(stem)
    ? CFLAGS.filter((flag) => flag !== "-fcall-used-r4")
    : [...CFLAGS];
  const base = NO_INTERWORK_SOURCES.has(stem) || NO_INTERWORK_OVERLAY_SOURCES.has(sourceKey(source))
    ? abiBase.filter((flag) => flag !== "-mthumb-interwork")
    : abiBase;
  return [
    ...base,
    ...(FIXED_R3_SOURCES.has(stem) ? ["-ffixed-r3"] : []),
    ...(FIXED_LR_SOURCES.has(stem) ? ["-ffixed-r14"] : []),
    ...(OPTIMIZE_O1_SOURCES.has(stem) ? ["-O1"] : []),
    ...(UNSCHEDULED_SOURCES.has(stem) ? ["-fno-schedule-insns", "-fno-schedule-insns2"] : []),
    ...(NO_CSE_FOLLOW_SOURCES.has(stem) ? ["-fno-cse-follow-jumps"] : []),
    ...(NO_RERUN_CSE_AFTER_LOOP_SOURCES.has(stem) ? ["-fno-rerun-cse-after-loop"] : []),
    ...(NO_GCSE_SOURCES.has(stem) ? ["-fno-gcse"] : []),
    ...(NO_EXPENSIVE_SOURCES.has(stem) ? ["-fno-expensive-optimizations"] : []),
    ...(NO_STRENGTH_REDUCE_SOURCES.has(stem) ? ["-fno-strength-reduce"] : []),
    ...(SCHED_HIGH_DEST_FIRST_SOURCES.has(stem) ? ["-fsched-high-dest-first"] : []),
    ...(SCHED_LOW_DEST_FIRST_SOURCES.has(stem) ? ["-fsched-low-dest-first"] : []),
    ...(NO_CONTIGUOUS_IMMEDIATE_SOURCES.has(stem) ? ["-fno-thumb-contiguous-immediate"] : []),
    ...(NO_SCHED_DEPEND_COUNT_SOURCES.has(stem) ? ["-fno-sched-depend-count"] : []),
    ...(SPLIT_GROUP_BASE_SOURCES.has(stem) ? ["-fthumb-split-group-base"] : []),
    ...(HOIST_PARAMETER_SAVE_SOURCES.has(stem) ? ["-fthumb-hoist-parameter-save"] : []),
    ...(MINIPOOL_TAIL_FIRST_SOURCES.has(stem) ? ["-fthumb-minipool-tail-first"] : []),
    ...(ENTRY_SAVES_DESCENDING_SOURCES.has(stem) ? ["-fthumb-entry-saves-descending"] : []),
    ...(GROUP_CONTROL_LAST_SOURCES.has(stem) ? ["-fthumb-group-control-last"] : []),
    ...(MOVE_BEFORE_ALU_SOURCES.has(stem) ? ["-fthumb-move-before-alu"] : []),
    ...(NO_REGMOVE_SOURCES.has(stem) ? ["-fno-regmove"] : []),
    ...(ENTRY_LITERAL_FIRST_SOURCES.has(stem)
      ? ["-fno-schedule-insns2", "-mthumb-entry-literal-first"] : []),
    ...(HIGH_REGISTER_MOVE_FIRST_SOURCES.has(stem) ? ["-mhigh-register-move-first"] : []),
    ...(ORR_DEAD_INPUT_REUSE_SOURCES.has(stem)
      ? ["-fthumb-orr-dead-input-reuse"]
      : []),
    ...(ENTRY_FRAME_CLUSTER_SOURCES.has(stem)
      ? ["-fthumb-entry-frame-cluster"]
      : []),
    ...(LITERAL_BEFORE_INDEX_SHIFT_SOURCES.has(stem)
      ? ["-fthumb-literal-before-index-shift"]
      : []),
    ...(LOW_CONSTANT_BEFORE_HIGH_MOVE_SOURCES.has(stem)
      ? ["-fthumb-low-constant-before-high-move"]
      : []),
    ...(HIGH_MOVE_BEFORE_STACK_STORE_SOURCES.has(stem)
      ? ["-fthumb-high-move-before-stack-store"]
      : []),
    ...(EARLY_FRAME_ALLOCATION_SOURCES.has(stem) ? ["-mearly-frame-allocation"] : []),
    ...(NO_OPTIMIZE_SIBLING_CALLS_SOURCES.has(stem) ? ["-fno-optimize-sibling-calls"] : []),
    ...(GROUPED_DMA_STORE_SOURCES.has(stem) ? ["-mgrouped-dma-store"] : []),
    ...(GROUP_VALUE2_IN_PLACE_SOURCES.has(stem) ? ["-fthumb-group-value2-in-place"] : []),
    ...(THUMB_IMMEDIATE_LATENCY_SOURCES.has(stem)
      ? ["-mthumb-immediate-latency"]
      : []),
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
    ...(NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-rerun-cse-after-loop"]
      : []),
    ...(NO_GCSE_OVERLAY_SOURCES.has(sourceKey(source)) ? ["-fno-gcse"] : []),
    ...(NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-cse-follow-jumps", "-fno-cse-skip-blocks"]
      : []),
    ...(NO_STRICT_ALIASING_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-strict-aliasing"]
      : []),
    ...(NO_CSE_TWO_INSN_IMMEDIATE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-cse-two-insn-immediate"]
      : []),
    ...(NO_CSE_POOL_IMMEDIATE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-cse-pool-immediate"]
      : []),
    ...(SCHED_LOW_DEST_FIRST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fsched-low-dest-first"]
      : []),
    ...(NO_SCHED_ALIAS_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-sched-alias"]
      : []),
    ...(NO_GCSE_INSERT_LOAD_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-gcse-insert-load"]
      : []),
    ...(SCHED_STORE_FIRST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fsched-store-first"]
      : []),
    ...(GROUPED_DMA_STORE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-mgrouped-dma-store"]
      : []),
    ...(EARLY_LITERAL_POOL_OVERLAY_SOURCES.has(overlayStem(source)) ||
      EARLY_LITERAL_POOL_OVERLAY_PATHS.has(sourceKey(source))
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

// Return every non-baseline flag that live GS1 routing can select. Compiler
// exploration uses this as an executable coverage contract: adding a routed
// mode without adding a corresponding explorer mode must fail its self-test.
export function evidencedRoutingFlags(): string[] {
  const baseline = new Set([...CFLAGS, ...GS2_CFLAGS, ...AGBCC_CFLAGS]);
  const found = new Set<string>();
  const inspect = (source: string): void => {
    for (const flag of cflagsForTargetSource("gs1", source)) {
      if (!baseline.has(flag)) found.add(flag);
    }
  };
  for (let address = 0x08000000; address < 0x08100000; address += 4) {
    inspect(`/tmp/${address.toString(16).padStart(8, "0")}.c`);
  }
  for (const source of [
    ...CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES,
    ...THUMB_IMMEDIATE_LATENCY_OVERLAY_SOURCES,
    ...NO_CANONICALIZE_COMPARISON_OVERLAY_SOURCES,
    ...NO_SCHED_DEPEND_COUNT_OVERLAY_SOURCES,
    ...NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES,
    ...NO_GCSE_OVERLAY_SOURCES,
    ...NO_INTERWORK_OVERLAY_SOURCES,
    ...NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES,
    ...NO_STRICT_ALIASING_OVERLAY_SOURCES,
    ...NO_CSE_TWO_INSN_IMMEDIATE_OVERLAY_SOURCES,
    ...NO_CSE_POOL_IMMEDIATE_OVERLAY_SOURCES,
    ...SCHED_LOW_DEST_FIRST_OVERLAY_SOURCES,
    ...NO_SCHED_ALIAS_OVERLAY_SOURCES,
    ...NO_GCSE_INSERT_LOAD_OVERLAY_SOURCES,
    ...SCHED_STORE_FIRST_OVERLAY_SOURCES,
    ...GROUPED_DMA_STORE_OVERLAY_SOURCES,
    ...EARLY_LITERAL_POOL_OVERLAY_PATHS,
  ]) inspect(join(ROOT, source));
  for (const stem of EARLY_LITERAL_POOL_OVERLAY_SOURCES) inspect(`/tmp/${stem}.c`);
  return [...found].sort();
}

// The optional trailing letter names a *second* external symbol whose value is
// the same address. The stock objects sometimes referenced one address through
// several symbols, and arm.c's minipool keeps one entry per distinct SYMBOL_REF,
// so a reconstruction that spells them all the same way collapses the duplicate
// pool words (and, with them, the blocks that load them).
const ADDRESS_SYMBOL = /^(Func|Data|Value)_([0-9a-f]{8})(?:_[a-z])?$/;
const CALL_VIA_SYMBOL = /^_call_via_r(1[0-3]|[0-9])$/;
const CALL_VIA_ALIAS = /^_call_via_(sl|fp|ip|sp)$/;
export const CALL_VIA_BASE = 0x080072e4;
const CALL_VIA_REGISTERS: Record<string, number> = { sl: 10, fp: 11, ip: 12, sp: 13 };

export interface ExternalSymbol {
  address: number;
  thumb: boolean;
}

// `callViaBase` names the `bx rN` bank the `_call_via_rN` stubs branch through.
// Every overlay carries its own bank, so a caller that knows which image it is
// linking passes that bank's address; the default is the main image's, which is
// what a `src/` translation unit links against.
// Address of each overlay's own `_call_via_rN` bank, in the space the
// reconstruction links against.
//
// An indirect call compiles to `bl _call_via_rN`, a stub that is nothing but
// `bx rN`. Every overlay carries its own bank, so resolving the stub to the
// main image's puts the branch a few bytes wrong: the row compares clean and
// then fails adoption, because the comparator never links. Roughly half the
// overlay rows that still have a semantic reference make an indirect call.
//
// Derive a new entry with `callViaBankBase` from `overlay_disasm.ts`, run
// against the overlay's assembly *before* any row that calls the bank has been
// adopted. It cannot be derived on demand: the address a `bl` encodes for a
// slot and the slot's own address in the image differ by a per-overlay constant
// (resource_373 +0x60, resource_3bc +0x19e), so the derivation has to read a
// real call into the bank -- and once that row is C, the assembly no longer has
// one. Hence a table, recorded once, rather than a scan on every build.
const OVERLAY_CALL_VIA_BASE: Record<string, number> = {
  resource_373: 0x020061b4,
  resource_382: 0x02003138,
  resource_385: 0x020014fc,
  resource_389: 0x02001578,
  resource_391: 0x02002d8c,
  resource_392: 0x02000eec,
  resource_393: 0x02000f34,
  resource_39b: 0x02002960,
  resource_39c: 0x02005fb0,
  resource_39d: 0x02004108,
  resource_39f: 0x02002f1c,
  resource_3a0: 0x020018bc,
  resource_3a5: 0x02002c6e,
  resource_3a6: 0x020020a8,
  resource_3b2: 0x02003180,
  resource_3b3: 0x02002f00,
  resource_3b4: 0x02002668,
  resource_3b5: 0x02000edc,
  resource_3b7: 0x020028e0,
  resource_3ba: 0x02003f4e,
  resource_3bc: 0x02004d4e,
  resource_3bd: 0x02003ed8,
  resource_3be: 0x02001920,
  resource_3bf: 0x02005810,
  resource_3c4: 0x02003214,
  resource_3c5: 0x02002ff8,
  resource_3c8: 0x02005324,
  resource_3c9: 0x02006384,
  resource_3cb: 0x02001a96,
};

// Per-source overrides, which win over the overlay's entry.
//
// The stored displacement is the same at every site that reaches a given slot,
// so the address a `bl` to it resolves to depends on where the call is. One
// address per overlay serves every site in a *narrow* address range and not a
// wide one, so a row far from the rest gets its own.
const SOURCE_CALL_VIA_BASE: Record<string, number> = {
  // Same routine as resource_373_c_02000030.c and the same slot, called from
  // 0x563e instead of 0x5e.
  "assets/code/resource_373_c_02005610.c": 0x0200b794,
};

export function overlayCallViaBase(overlay: string, source?: string): number {
  const override = source === undefined ? undefined : SOURCE_CALL_VIA_BASE[sourceKey(source)];
  return override ?? OVERLAY_CALL_VIA_BASE[overlay] ?? CALL_VIA_BASE;
}

export function externalSymbol(name: string, callViaBase = CALL_VIA_BASE): ExternalSymbol | null {
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
      address: callViaBase + Number.parseInt(callVia[1], 10) * 4,
      thumb: true,
    };
  }
  const callViaAlias = name.match(CALL_VIA_ALIAS);
  if (callViaAlias !== null) {
    return {
      address: callViaBase + CALL_VIA_REGISTERS[callViaAlias[1]] * 4,
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
export function externalSymbolAssembly(name: string, callViaBase = CALL_VIA_BASE): string {
  const symbol = externalSymbol(name, callViaBase);
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

// 各ファイルは承認済みダイジェストの集合を持つ。一つの host+target につき
// 複数の束が並ぶのは、同一の alchemy-gcc ソースを別のホスト環境で再構築すると
// 実行体のバイト列は変わるがコード生成は変わらないため。gcc は接頭辞パスなどを
// 実行体に埋め込むので、これは想定内の差分である。
//
// A listed digest is not a guess: every entry here has independently passed the
// admission test PROVENANCE.md requires of any toolchain substitution — the
// full source-only build owns all 8,388,608 bytes with zero ROM fallback, and
// `bun run build:full` reproduces gs1-en.gba (SHA-1
// 5c4695205413df7db52b9a184815a07783999971) byte-identically under it. Adding a
// digest without running `bun run verify` on that bundle defeats the check;
// re-pin only from a green verify.
const EXPECTED: Record<HostKey, Record<CompilerTarget, Record<string, readonly string[]>>> = {
  "darwin-arm64": {
    gs1: {
      xgcc: ["87e09e3f1e2fd711e952d6831c73099b14a059a6ca594b16c11b9a83394483ed"],
      cpp: ["f72b13ad2368419f2cc8c24966e030a57638bfce3f97868043196dac41e13575"],
      tradcpp: ["822c5cf4b38ea231f6eeeadcdf3a457518a25202c8a0a04aadf0942154e5436b"],
      cc1: ["df015cd830e04f26ce2ae1d3cc83205182f98cea1e41a29d586a79fb72d193a4"],
    },
    gs2: {
      xgcc: ["128520f13ff01aee64a984b1279a6e3a682a3679de44c99296064f46fb1e8ec2"],
      cpp0: ["b4ac7f5ff7fd74f4eca40385832fd0360d13cb5d4f0b6c8b3ead4a67d2f3d5b0"],
      tradcpp0: ["7698319dfea3647dace68ffb5c3dbc0fd459f3a859699acb47c669d3eb8956a3"],
      cc1: ["91b2a67275a100e8b6695d85ef2d82d1fd144853cbcb361ddf1d8be31858230f"],
    },
  },
  "linux-x64": {
    gs1: {
      xgcc: [
        "845b828e15efedfeacc1956ac2694101e2b520824643d5b9f7608f9c389aee03",
        "2ed03493228a7873f020b16a63b89b3aadf4835be2d1a3a217cabca0fa244444",
      ],
      cpp: [
        "60d0b6637deb0f98cbf952a89694b02a0557fc87ca968121759be139372e90cc",
        "06096beb427848574f610626bb53408b1a76f69b178ee2d7f0a05f6c2f6d3778",
      ],
      tradcpp: [
        "87f89bebf41cd12ac7706604dd24624061b2276f95cc1e9998c22de1accfee2a",
        "f9b951486d4e1769e06b892a59980c91a45435559505d73039130b63d1156803",
      ],
      cc1: [
        "c1cc6d2864567297451662d36fba7abbce7a916d138f7115832a265de6868a06",
        "640964de34d6202f6dc5943b0c22b0afd1a8f4f1307ba6d3cf30af4110f5f5e2",
      ],
    },
    gs2: {
      xgcc: [
        "7b1a6a96fc4bd5e9de4d83fb2a4ba2ca2a82397cdcd102c4a4d76ef91dc17f58",
        "d0b10d67bc7f9965d586eba766b77e6ca54cc791b5eb297b55a6b9b6d6d0ef3d",
      ],
      cpp0: [
        "89791031fa8d4dd686355efb0efdb7c019a4080b770f187b15671dc3c1e71ecc",
        "9d93c7762f60d13474764d2ca9e721b235ed4935ba7b69012aba054cace60d0d",
      ],
      tradcpp0: [
        "a1013c94647eefbe0caad3c2e244b66c1acf2961197bdc31012e4259616c3198",
        "010da8763b9ebf39cb52aab0412ca350e038ccf4c3aa5647440c2abc91dcad6c",
      ],
      cc1: [
        "3e5f1cbcae107b0f6c038a8b91880e377a3612a965014165002b4c530feab56b",
        "1b1c039eda51c0c2ee67d076f33e3284dd369789378cdf34671b66ba76cd6c75",
      ],
    },
  },
};

const LINUX_AGBCC_EXPECTED: readonly string[] = [
  "30a2a042c4be2acdd215ffc26c7d27498098ac38607ec8af43cc6598dcecdf55",
  "0c2d5ec04129f7b9d1ecf738f096167af152661bc2506f8fdb2749305fa3eb37",
];

const validated = new Set<CompilerTarget>();
let agbccValidated = false;
const experimentalValidated = new Set<string>();
const AGBCC_EXPECTED: readonly string[] = [
  "4f7664872d10a737184fb2e0502c407c9d74505f0cff7313ba4e9083736c2207",
];
const PRET_EARLY_THUMB_EXPECTED: Record<HostKey, readonly string[]> = {
  "darwin-arm64": ["8a1e0e9e18801efb595a3e0d571137db5ba8f97e413c323e99f18b0521a31636"],
  "linux-x64": ["c988f677e3ebd7252a6ad1ad2fef301f85b05be0612ee3192b37ec47d22f8082"],
};
const GCC2951_EXPECTED: Record<HostKey, readonly string[]> = {
  "darwin-arm64": ["cb41bba7e0e600721d906c46349119efb4c6fd35c711d7e0f244cb783de383a6"],
  "linux-x64": [
    "c8f80fffa2aa0aa2809d93ad86d11ea0e8ebf08e9bba6cc5b8d391aef05c3fe4",
    "edbee4fec1a1b59d0fd77273559aebbaf2c92b344bbeeb3539a10b689e71716d",
  ],
};

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
    if (!expected.includes(actual)) {
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
  if (!expectedAgbcc.includes(actual)) {
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

function validateExperimentalCompiler(
  name: string,
  driver: string,
  expected: Record<HostKey, readonly string[]>,
): void {
  if (experimentalValidated.has(name)) return;
  const host = hostKey();
  if (host === null) {
    throw new Error(`alchemy-gcc experimental ${name} requires native arm64 macOS or x64 Linux`);
  }
  let mode = 0;
  try {
    mode = statSync(driver).mode;
  } catch {
    throw new Error(`alchemy-gcc experimental ${name} is missing executable cc1`);
  }
  if ((mode & 0o111) === 0) {
    throw new Error(`alchemy-gcc experimental ${name} is missing executable cc1`);
  }
  const actual = new Bun.CryptoHasher("sha256").update(readFileSync(driver)).digest("hex");
  if (!expected[host].includes(actual)) {
    throw new Error(`alchemy-gcc experimental ${name}/cc1 has an unapproved digest`);
  }
  const smoke = Bun.spawnSync(
    [driver, "/dev/null", "-quiet", "-O2", "-o", "/dev/null"],
    { cwd: ROOT, stdout: "pipe", stderr: "pipe" },
  );
  if (smoke.exitCode !== 0) {
    const detail = (outputText(smoke.stderr) || outputText(smoke.stdout)).trim();
    throw new Error(`alchemy-gcc experimental ${name} smoke compile failed: ${detail}`);
  }
  experimentalValidated.add(name);
}

export function compilerBundleSignature(): string {
  const paths = [
    join(BUNDLE, "xgcc"), join(BUNDLE, "cpp"), join(BUNDLE, "tradcpp"), join(BUNDLE, "cc1"),
    join(GS2_BUNDLE, "xgcc"), join(GS2_BUNDLE, "cpp0"), join(GS2_BUNDLE, "tradcpp0"),
    join(GS2_BUNDLE, "cc1"), AGBCC_DRIVER,
    PRET_EARLY_THUMB_DRIVER, GCC2951_DRIVER,
  ];
  const digest = new Bun.CryptoHasher("sha256");
  for (const path of paths) {
    digest.update(path);
    digest.update("\0");
    try {
      digest.update(readFileSync(path));
    } catch {
      digest.update("missing");
    }
    digest.update("\0");
  }
  return digest.digest("hex");
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

// A compilation plan is the canonical boundary between compiler policy and
// process execution. `routingSource` identifies the translation unit whose
// evidenced compiler family/flags must be used; `input` is the file that is
// actually compiled. Keeping those names separate is essential for candidate
// and corpus work, where a temporary source must compile exactly as the
// eventual installed source would.
export type CompilerFamily =
  | "routed"
  | "gcc296"
  | "old-agbcc"
  | "pret-early-thumb"
  | "gcc2951";

export interface CompilerFlagMutations {
  addFlags?: readonly string[];
  removeFlags?: readonly string[];
}

export interface SourceToAssemblyPlanOptions {
  target: CompilerTarget;
  routingSource: string;
  input: string;
  output: string;
  family?: CompilerFamily;
  flags?: CompilerFlagMutations;
  // old_agbcc consumes preprocessed input. Supplying this makes intermediate
  // ownership explicit; otherwise it is placed beside the assembly output.
  preprocessedOutput?: string;
  dumpbase?: string;
}

export interface CompilerCommandStep {
  kind: "preprocess" | "compile";
  command: readonly string[];
}

export interface SourceToAssemblyPlan {
  target: CompilerTarget;
  requestedFamily: CompilerFamily;
  family: Exclude<CompilerFamily, "routed">;
  routingSource: string;
  input: string;
  output: string;
  compilerInput: string;
  flags: readonly string[];
  steps: readonly CompilerCommandStep[];
}

function mutatedCompilerFlags(
  canonical: readonly string[],
  mutations: CompilerFlagMutations = {},
): string[] {
  const added = [...(mutations.addFlags ?? [])];
  const removed = new Set(mutations.removeFlags ?? []);
  for (const flag of added) {
    if (removed.has(flag)) {
      throw new Error(`compiler flag cannot be both added and removed: ${flag}`);
    }
  }
  return [...canonical.filter((flag) => !removed.has(flag)), ...added];
}

function inferredPreprocessedOutput(output: string): string {
  const extension = extname(output);
  return extension === "" ? `${output}.i` : `${output.slice(0, -extension.length)}.i`;
}

export function sourceToAssemblyPlan(
  options: SourceToAssemblyPlanOptions,
): SourceToAssemblyPlan {
  const requestedFamily = options.family ?? "routed";
  const family = requestedFamily === "routed"
    ? (usesAgbccCompiler(options.target, options.routingSource) ? "old-agbcc" : "gcc296")
    : requestedFamily;
  if (family !== "gcc296" && options.target !== "gs1") {
    throw new Error(`${family} is only approved for gs1`);
  }

  const canonical = requestedFamily === "routed"
    ? cflagsForTargetSource(options.target, options.routingSource)
    : family !== "gcc296"
      ? AGBCC_CFLAGS
      : cflagsForTarget(options.target);
  const flags = mutatedCompilerFlags(canonical, options.flags);
  const dumpbase = options.dumpbase ?? basename(options.routingSource);
  let compilerInput = options.input;
  const steps: CompilerCommandStep[] = [];

  if (family !== "gcc296") {
    const driver = family === "old-agbcc"
      ? AGBCC_DRIVER
      : family === "pret-early-thumb"
        ? PRET_EARLY_THUMB_DRIVER
        : GCC2951_DRIVER;
    if (family === "old-agbcc") validateAgbccBundle();
    else if (family === "pret-early-thumb") {
      validateExperimentalCompiler(family, driver, PRET_EARLY_THUMB_EXPECTED);
    } else {
      validateExperimentalCompiler(family, driver, GCC2951_EXPECTED);
    }
    compilerInput = options.preprocessedOutput ?? inferredPreprocessedOutput(options.output);
    steps.push({
      kind: "preprocess",
      command: directPreprocessorCommand(
        options.input,
        compilerInput,
        family === "gcc2951" ? 95 : 9,
      ),
    });
    steps.push({
      kind: "compile",
      command: [
        driver, compilerInput, "-dumpbase", dumpbase,
        ...flags, "-o", options.output,
      ],
    });
  } else {
    steps.push({
      kind: "compile",
      command: compilerCommandForTarget(
        options.target,
        ...flags, "-S", "-o", options.output, options.input,
      ),
    });
  }

  return {
    target: options.target,
    requestedFamily,
    family,
    routingSource: options.routingSource,
    input: options.input,
    output: options.output,
    compilerInput,
    flags,
    steps,
  };
}

// Hot-search pipeline: invoke the approved preprocessor and cc1 directly, saving
// one driver process per candidate. These arguments are the exact subprocesses
// emitted by xgcc for CFLAGS; ordinary builds use the source-aware command API.
export function directPreprocessorCommand(
  input: string,
  output: string,
  gccMinor = 96,
): string[] {
  validateBundle();
  return [
    join(BUNDLE, "cpp"), "-lang-c", "-nostdinc",
    "-D__GNUC__=2", `-D__GNUC_MINOR__=${gccMinor}`, "-D__GNUC_PATCHLEVEL__=0",
    "-Acpu(arm)", "-Amachine(arm)", "-D__CHAR_UNSIGNED__", "-D__OPTIMIZE__",
    "-D__ARM_ARCH_4T__", "-D__APCS_32__", "-D__ARMEL__", "-D__THUMBEL__",
    "-Darm_elf", "-D__ELF__", "-Dthumb", "-D__thumb__",
    `-I${join(ROOT, "include")}`, input, output,
  ];
}

export function directCompilerCommand(
  input: string,
  output: string,
  dumpbase: string,
  source = dumpbase,
): string[] {
  validateBundle();
  const flags = cflagsForSource(source).filter((flag) =>
    flag !== "-nostdinc" && !flag.startsWith("-I"));
  return [
    join(BUNDLE, "cc1"), input, "-quiet", "-dumpbase", dumpbase,
    ...flags,
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
  // `externalSymbolAssembly` grew a second parameter when overlays stopped
  // sharing one `call_via` bank, and every `names.map(externalSymbolAssembly)`
  // in the tree silently started feeding it the array index: `_call_via_r3`
  // resolved to 0x0000000c instead of 0x080072f0, the assembler could not reach
  // it with a Thumb `bl`, and each affected main-image object grew a long-branch
  // veneer that ran past its claimed span. `build:claimed` failed to link with
  // fifteen section overlaps and nothing in the C had changed. Passing the
  // callback by reference is the trap; this pins both arities so it cannot
  // return silently.
  const direct = externalSymbolAssembly("_call_via_r3");
  if (!direct.includes("0x080072f0")) {
    throw new Error(`call_via default base self-test failed: ${direct.trim()}`);
  }
  if (!externalSymbolAssembly("_call_via_r3", 0x020061b4).includes("0x020061c0")) {
    throw new Error("call_via explicit base self-test failed");
  }
  // The arity check above only pins this file. Seven tools wrote the emitter as
  // a bare callback and all seven were wrong, so scan for the shape itself.
  const toolsDirectory = dirname(Bun.fileURLToPath(import.meta.url));
  const leaking = readdirSync(toolsDirectory)
    .filter((name) => name.endsWith(".ts"))
    .filter((name) => readFileSync(join(toolsDirectory, name), "utf8")
      .split("\n")
      .filter((line) => !line.trimStart().startsWith("//"))
      .some((line) => /\.map\(\s*externalSymbol(Assembly)?\s*\)/.test(line)))
    .sort();
  if (leaking.length > 0) {
    throw new Error(
      `these pass externalSymbolAssembly to .map() by reference, which feeds the ` +
      `array index in as callViaBase: ${leaking.join(", ")}`,
    );
  }
  const expected = [
    "08006a00", "08006ba8", "08006c24", "08006c68", "08006cdc", "08006d50", "08006dec",
    "08006e24", "08006f84", "08007028", "08007098", "0800711c", "080071a8", "08007220",
    "080f9a50",
    "080fa1fc", "080fa2a0", "080fa324", "080fa350", "080fa39c", "080fa3f0",
    "080fa424", "080fa458", "080fa490", "080fa514", "080fa55c", "080fa6a0", "080fa83c", "080fa8d4", "080fa928", "080fa9a4",
    "080fa9e0", "080fab3c", "080fab7c", "080fac44", "080facf8", "080fada0", "080fadf0", "080fb2cc", "080fb334", "080fb3a8", "080fb430", "080fb4a4",
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
      ...(["08006a00", "08006ba8", "08006c68", "08006cdc", "08006d50", "08006e24",
           "08006f84", "08007028", "08007098", "0800711c", "080071a8",
           "08007220"].includes(stem) ? ["-O1"] : []),
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
    "08004a94", "08005340", "08005394", "080053e8", "08005a78", "08005c68", "080060e8", "0800bc48", "0800bdd4", "0800c0f4", "0800d304",
    "080170c4", "08019bac", "0801d014", "0801d980", "080251d4", "080284dc", "0808fecc", "08094730", "08095160", "08095290", "080958a8", "08097540",
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
  for (const stem of ["08095160", "08095290"]) {
    const flags = cflagsForTargetSource("gs1", `/tmp/${stem}.c`);
    const direct = directCompilerCommand(
      `/tmp/${stem}.i`, `/tmp/${stem}.s`, `${stem}.c`, `/tmp/${stem}.c`,
    );
    const neighbor = `${(Number.parseInt(stem, 16) + 4).toString(16).padStart(8, "0")}`;
    const neighborDirect = directCompilerCommand(
      `/tmp/${neighbor}.i`, `/tmp/${neighbor}.s`, `${neighbor}.c`, `/tmp/${neighbor}.c`,
    );
    const gs2 = cflagsForTargetSource("gs2", `/tmp/${stem}.c`);
    if (!flags.includes("-mgrouped-dma-store") ||
        !direct.includes("-mgrouped-dma-store") ||
        neighborDirect.includes("-mgrouped-dma-store") ||
        gs2.includes("-mgrouped-dma-store")) {
      throw new Error(`${stem} effect-initializer DMA routing self-test failed`);
    }
  }
  const polledDmaFlags = cflagsForTargetSource("gs1", "/tmp/08094730.c");
  const polledDmaDirect = directCompilerCommand(
    "/tmp/08094730.i", "/tmp/08094730.s", "08094730.c", "/tmp/08094730.c",
  );
  const polledDmaNeighborDirect = directCompilerCommand(
    "/tmp/08094734.i", "/tmp/08094734.s", "08094734.c", "/tmp/08094734.c",
  );
  for (const flag of ["-mgrouped-dma-store", "-fno-sched-depend-count"]) {
    if (!polledDmaFlags.includes(flag) ||
        !polledDmaDirect.includes(flag) ||
        cflagsForTargetSource("gs1", "/tmp/08094734.c").includes(flag) ||
        polledDmaNeighborDirect.includes(flag) ||
        cflagsForTargetSource("gs2", "/tmp/08094730.c").includes(flag)) {
      throw new Error(`08094730 polled-DMA routing self-test failed for ${flag}`);
    }
  }
  const stackZeroFlag = "-fthumb-high-move-before-stack-store";
  const stackZeroFlags = cflagsForTargetSource("gs1", "/tmp/08095290.c");
  const stackZeroDirect = directCompilerCommand(
    "/tmp/08095290.i", "/tmp/08095290.s", "08095290.c", "/tmp/08095290.c",
  );
  const stackZeroSiblingDirect = directCompilerCommand(
    "/tmp/08095160.i", "/tmp/08095160.s", "08095160.c", "/tmp/08095160.c",
  );
  const stackZeroNeighborDirect = directCompilerCommand(
    "/tmp/08095294.i", "/tmp/08095294.s", "08095294.c", "/tmp/08095294.c",
  );
  if (JSON.stringify([...HIGH_MOVE_BEFORE_STACK_STORE_SOURCES].sort()) !==
        JSON.stringify(["08095290"]) ||
      !stackZeroFlags.includes(stackZeroFlag) ||
      !stackZeroDirect.includes(stackZeroFlag) ||
      cflagsForTargetSource("gs1", "/tmp/08095160.c").includes(stackZeroFlag) ||
      stackZeroSiblingDirect.includes(stackZeroFlag) ||
      cflagsForTargetSource("gs1", "/tmp/08095294.c").includes(stackZeroFlag) ||
      stackZeroNeighborDirect.includes(stackZeroFlag) ||
      cflagsForTargetSource("gs2", "/tmp/08095290.c").includes(stackZeroFlag)) {
    throw new Error("08095290 stack-zero ordering routing self-test failed");
  }
  const copiedDecompressorFlags = [
    "-mgrouped-dma-store",
    "-fthumb-move-before-alu",
    "-fno-sched-depend-count",
    "-fthumb-hoist-parameter-save",
  ];
  for (const stem of ["08005340", "08005394", "080053e8"]) {
    const copiedDecompressor = cflagsForTargetSource("gs1", `/tmp/${stem}.c`);
    const copiedDecompressorDirect = directCompilerCommand(
      `/tmp/${stem}.i`, `/tmp/${stem}.s`, `${stem}.c`, `/tmp/${stem}.c`,
    );
    const copiedDecompressorGs2 = cflagsForTargetSource("gs2", `/tmp/${stem}.c`);
    for (const flag of copiedDecompressorFlags) {
      if (!copiedDecompressor.includes(flag) ||
          !copiedDecompressorDirect.includes(flag) ||
          copiedDecompressorGs2.includes(flag)) {
        throw new Error(`${stem} copied-decompressor routing self-test failed for ${flag}`);
      }
    }
  }
  for (const source of ["/tmp/08005344.c", "/tmp/08005398.c", "/tmp/080053ec.c"]) {
    for (const flag of copiedDecompressorFlags) {
      if (cflagsForTargetSource("gs1", source).includes(flag)) {
        throw new Error(`${source} copied-decompressor neighbor routing self-test failed for ${flag}`);
      }
    }
  }
  const minipoolTailFirst = "-fthumb-minipool-tail-first";
  const minipoolSource = "/tmp/080a9aec.c";
  const minipoolDirect = directCompilerCommand(
    "/tmp/080a9aec.i", "/tmp/080a9aec.s", "080a9aec.c", minipoolSource,
  );
  if (!cflagsForTargetSource("gs1", minipoolSource).includes(minipoolTailFirst) ||
      !minipoolDirect.includes(minipoolTailFirst) ||
      cflagsForTargetSource("gs1", "/tmp/080a9af0.c").includes(minipoolTailFirst) ||
      directCompilerCommand(
        "/tmp/080a9af0.i", "/tmp/080a9af0.s", "080a9af0.c", "/tmp/080a9af0.c",
      ).includes(minipoolTailFirst) ||
      cflagsForTargetSource("gs2", minipoolSource).includes(minipoolTailFirst)) {
    throw new Error("080a9aec minipool routing self-test failed");
  }
  const thumbImmediateLatency = "-mthumb-immediate-latency";
  const thumbImmediateSource = "/tmp/080babdc.c";
  const thumbImmediateDirect = directCompilerCommand(
    "/tmp/080babdc.i", "/tmp/080babdc.s", "080babdc.c", thumbImmediateSource,
  );
  if (JSON.stringify([...THUMB_IMMEDIATE_LATENCY_SOURCES].sort()) !==
        JSON.stringify(["080babdc"]) ||
      !cflagsForTargetSource("gs1", thumbImmediateSource).includes(thumbImmediateLatency) ||
      !thumbImmediateDirect.includes(thumbImmediateLatency) ||
      cflagsForTargetSource("gs1", "/tmp/080babe0.c").includes(thumbImmediateLatency) ||
      directCompilerCommand(
        "/tmp/080babe0.i", "/tmp/080babe0.s", "080babe0.c", "/tmp/080babe0.c",
      ).includes(thumbImmediateLatency) ||
      cflagsForTargetSource("gs2", thumbImmediateSource).includes(thumbImmediateLatency)) {
    throw new Error("080babdc immediate-latency routing self-test failed");
  }
  const copyLifetimeFlags = cflagsForTargetSource("gs1", "/tmp/08006088.c");
  const unrelatedFlags = cflagsForTargetSource("gs1", "/tmp/0800608c.c");
  if (!copyLifetimeFlags.includes("-fno-rerun-cse-after-loop") ||
      !copyLifetimeFlags.includes("-fno-regmove") ||
      unrelatedFlags.includes("-fno-rerun-cse-after-loop") ||
      unrelatedFlags.includes("-fno-regmove") ||
      cflagsForTargetSource("gs2", "/tmp/08006088.c").includes("-fno-rerun-cse-after-loop")) {
    throw new Error("08006088 copy-lifetime routing self-test failed");
  }
  if (!cflagsForTargetSource("gs1", "/tmp/0808c30c.c").includes("-fno-rerun-cse-after-loop") ||
      cflagsForTargetSource("gs1", "/tmp/0808c310.c").includes("-fno-rerun-cse-after-loop") ||
      cflagsForTargetSource("gs2", "/tmp/0808c30c.c").includes("-fno-rerun-cse-after-loop")) {
    throw new Error("0808c30c CSE-rerun routing self-test failed");
  }
  if (!cflagsForTargetSource("gs1", "/tmp/0801ed40.c").includes("-fno-gcse") ||
      !cflagsForTargetSource("gs1", "/tmp/080981b0.c").includes("-fno-gcse") ||
      !cflagsForTargetSource("gs1", "/tmp/08098c08.c").includes("-fno-gcse") ||
      !cflagsForTargetSource("gs1", "/tmp/080b3284.c").includes("-fno-gcse") ||
      !cflagsForTargetSource("gs1", "/tmp/080d40ec.c").includes("-fno-gcse") ||
      !directCompilerCommand(
        "/tmp/080d40ec.i", "/tmp/080d40ec.s", "080d40ec.c", "/tmp/080d40ec.c",
      ).includes("-fno-gcse") ||
      directCompilerCommand(
        "/tmp/080d40f0.i", "/tmp/080d40f0.s", "080d40f0.c", "/tmp/080d40f0.c",
      ).includes("-fno-gcse") ||
      cflagsForTargetSource("gs1", "/tmp/0801ed44.c").includes("-fno-gcse") ||
      cflagsForTargetSource("gs1", "/tmp/080981b4.c").includes("-fno-gcse") ||
      cflagsForTargetSource("gs1", "/tmp/08098c0c.c").includes("-fno-gcse") ||
      cflagsForTargetSource("gs1", "/tmp/080b3288.c").includes("-fno-gcse") ||
      cflagsForTargetSource("gs1", "/tmp/080d40f0.c").includes("-fno-gcse") ||
      cflagsForTargetSource("gs2", "/tmp/0801ed40.c").includes("-fno-gcse") ||
      cflagsForTargetSource("gs2", "/tmp/080981b0.c").includes("-fno-gcse") ||
      cflagsForTargetSource("gs2", "/tmp/08098c08.c").includes("-fno-gcse") ||
      cflagsForTargetSource("gs2", "/tmp/080b3284.c").includes("-fno-gcse") ||
      cflagsForTargetSource("gs2", "/tmp/080d40ec.c").includes("-fno-gcse")) {
    throw new Error("no-GCSE routing self-test failed");
  }
  if (!cflagsForTargetSource("gs1", "/tmp/08021d88.c").includes("-fno-sched-depend-count") ||
      cflagsForTargetSource("gs1", "/tmp/08021d8c.c").includes("-fno-sched-depend-count") ||
      cflagsForTargetSource("gs2", "/tmp/08021d88.c").includes("-fno-sched-depend-count")) {
    throw new Error("08021d88 scheduler routing self-test failed");
  }
  const lowConstantBeforeHighMove = "-fthumb-low-constant-before-high-move";
  if (!cflagsForTargetSource("gs1", "/tmp/080a5614.c").includes(lowConstantBeforeHighMove) ||
      !cflagsForTargetSource("gs1", "/tmp/080ba918.c").includes(lowConstantBeforeHighMove) ||
      !cflagsForTargetSource("gs1", "/tmp/080ba918.c").includes("-fno-rerun-cse-after-loop") ||
      cflagsForTargetSource("gs1", "/tmp/080ba918.c").includes("-fno-expensive-optimizations") ||
      !cflagsForTargetSource("gs1", "/tmp/080ba918.c").includes("-fno-regmove") ||
      cflagsForTargetSource("gs1", "/tmp/080a6a98.c").includes(lowConstantBeforeHighMove) ||
      cflagsForTargetSource("gs2", "/tmp/080a5614.c").includes(lowConstantBeforeHighMove) ||
      cflagsForTargetSource("gs2", "/tmp/080ba918.c").includes(lowConstantBeforeHighMove)) {
    throw new Error("low-constant-before-high-move routing self-test failed");
  }
  const orrDeadInputReuse = "-fthumb-orr-dead-input-reuse";
  if (!cflagsForTargetSource("gs1", "/tmp/08003adc.c").includes(orrDeadInputReuse) ||
      cflagsForTargetSource("gs1", "/tmp/08003ae0.c").includes(orrDeadInputReuse) ||
      cflagsForTargetSource("gs2", "/tmp/08003adc.c").includes(orrDeadInputReuse)) {
    throw new Error("ORR dead-input reuse routing self-test failed");
  }
  const entryFrameCluster = "-fthumb-entry-frame-cluster";
  if (!cflagsForTargetSource("gs1", "/tmp/0801c34c.c").includes(entryFrameCluster) ||
      cflagsForTargetSource("gs1", "/tmp/0801c350.c").includes(entryFrameCluster) ||
      cflagsForTargetSource("gs2", "/tmp/0801c34c.c").includes(entryFrameCluster)) {
    throw new Error("entry-frame-cluster routing self-test failed");
  }
  const literalBeforeIndexShift = "-fthumb-literal-before-index-shift";
  const interruptMaskFlags = cflagsForTargetSource("gs1", "/tmp/0800307c.c");
  if (!interruptMaskFlags.includes("-fno-schedule-insns2") ||
      !interruptMaskFlags.includes(literalBeforeIndexShift) ||
      cflagsForTargetSource("gs1", "/tmp/08003080.c").includes(literalBeforeIndexShift) ||
      cflagsForTargetSource("gs2", "/tmp/0800307c.c").includes(literalBeforeIndexShift)) {
    throw new Error("literal-before-index-shift routing self-test failed");
  }
  if (!cflagsForTargetSource("gs1", "/tmp/080049e8.c").includes("-O1") ||
      cflagsForTargetSource("gs1", "/tmp/08004a28.c").includes("-O1") ||
      cflagsForTargetSource("gs1", "/tmp/08004a5c.c").includes("-O1") ||
      cflagsForTargetSource("gs2", "/tmp/080049e8.c").includes("-O1")) {
    throw new Error("grouped DMA O1 routing self-test failed");
  }
  const callArg0MoveFirstOverlays = [...CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES].sort();
  if (callArg0MoveFirstOverlays.length !== 29) {
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

  const plannedGcc = sourceToAssemblyPlan({
    target: "gs1",
    routingSource: "/installed/08006088.c",
    input: "/work/candidate.c",
    output: "/work/candidate.s",
    flags: {
      removeFlags: ["-fcall-used-r4"],
      addFlags: ["-ffixed-r5"],
    },
  });
  if (plannedGcc.requestedFamily !== "routed" ||
      plannedGcc.family !== "gcc296" ||
      plannedGcc.steps.length !== 1 ||
      plannedGcc.steps[0].kind !== "compile" ||
      plannedGcc.compilerInput !== "/work/candidate.c" ||
      plannedGcc.steps[0].command.at(-1) !== "/work/candidate.c" ||
      !plannedGcc.flags.includes("-fno-rerun-cse-after-loop") ||
      !plannedGcc.flags.includes("-fno-regmove") ||
      !plannedGcc.flags.includes("-ffixed-r5") ||
      plannedGcc.flags.includes("-fcall-used-r4")) {
    throw new Error("source-to-assembly GCC routing/mutation self-test failed");
  }

  const plannedAgbcc = sourceToAssemblyPlan({
    target: "gs1",
    routingSource: "/installed/080fa514.c",
    input: "/work/candidate.c",
    output: "/work/candidate.s",
    preprocessedOutput: "/work/candidate.preprocessed.i",
  });
  if (plannedAgbcc.requestedFamily !== "routed" ||
      plannedAgbcc.family !== "old-agbcc" ||
      plannedAgbcc.compilerInput !== "/work/candidate.preprocessed.i" ||
      plannedAgbcc.steps.length !== 2 ||
      plannedAgbcc.steps[0].kind !== "preprocess" ||
      plannedAgbcc.steps[1].kind !== "compile" ||
      plannedAgbcc.steps[0].command.at(-2) !== "/work/candidate.c" ||
      plannedAgbcc.steps[0].command.at(-1) !== "/work/candidate.preprocessed.i" ||
      plannedAgbcc.steps[1].command[1] !== "/work/candidate.preprocessed.i" ||
      plannedAgbcc.steps[1].command.at(-1) !== "/work/candidate.s" ||
      !plannedAgbcc.flags.includes("-O1") ||
      !plannedAgbcc.flags.includes("-mcommutative-copy-constant")) {
    throw new Error("source-to-assembly old_agbcc routing/preprocessing self-test failed");
  }

  const forcedAgbcc = sourceToAssemblyPlan({
    target: "gs1",
    routingSource: "/installed/not-routed.c",
    input: "/work/forced.c",
    output: "/work/forced.s",
    family: "old-agbcc",
  });
  if (forcedAgbcc.family !== "old-agbcc" ||
      forcedAgbcc.flags.includes("-mcommutative-copy-constant") ||
      forcedAgbcc.compilerInput !== "/work/forced.i") {
    throw new Error("source-to-assembly forced old_agbcc self-test failed");
  }

  for (const [family, driver, minor] of [
    ["pret-early-thumb", PRET_EARLY_THUMB_DRIVER, "9"],
    ["gcc2951", GCC2951_DRIVER, "95"],
  ] as const) {
    const planned = sourceToAssemblyPlan({
      target: "gs1",
      routingSource: "/installed/not-routed.c",
      input: `/work/${family}.c`,
      output: `/work/${family}.s`,
      family,
    });
    const preprocess = planned.steps[0]?.command ?? [];
    const compile = planned.steps[1]?.command ?? [];
    if (planned.requestedFamily !== family ||
        planned.family !== family ||
        planned.steps.length !== 2 ||
        planned.steps[0].kind !== "preprocess" ||
        planned.steps[1].kind !== "compile" ||
        planned.compilerInput !== `/work/${family}.i` ||
        !preprocess.includes(`-D__GNUC_MINOR__=${minor}`) ||
        compile[0] !== driver ||
        compile[1] !== `/work/${family}.i` ||
        compile.at(-1) !== `/work/${family}.s`) {
      throw new Error(`source-to-assembly ${family} family self-test failed`);
    }
  }

  let rejectedConflict = false;
  try {
    sourceToAssemblyPlan({
      target: "gs1",
      routingSource: "/installed/080000c0.c",
      input: "/work/candidate.c",
      output: "/work/candidate.s",
      flags: { addFlags: ["-O1"], removeFlags: ["-O1"] },
    });
  } catch (error) {
    rejectedConflict = String(error).includes("both added and removed");
  }
  if (!rejectedConflict) {
    throw new Error("source-to-assembly conflicting mutation self-test failed");
  }

  let rejectedGs2Agbcc = false;
  try {
    sourceToAssemblyPlan({
      target: "gs2",
      routingSource: "/installed/080000c0.c",
      input: "/work/candidate.c",
      output: "/work/candidate.s",
      family: "old-agbcc",
    });
  } catch (error) {
    rejectedGs2Agbcc = String(error).includes("only approved for gs1");
  }
  if (!rejectedGs2Agbcc) {
    throw new Error("source-to-assembly GS2 old_agbcc rejection self-test failed");
  }
  for (const family of ["pret-early-thumb", "gcc2951"] as const) {
    let rejected = false;
    try {
      sourceToAssemblyPlan({
        target: "gs2",
        routingSource: "/installed/080000c0.c",
        input: "/work/candidate.c",
        output: "/work/candidate.s",
        family,
      });
    } catch (error) {
      rejected = String(error).includes("only approved for gs1");
    }
    if (!rejected) {
      throw new Error(`source-to-assembly GS2 ${family} rejection self-test failed`);
    }
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
  if (argument === "pret-early-thumb") {
    validateExperimentalCompiler(argument, PRET_EARLY_THUMB_DRIVER, PRET_EARLY_THUMB_EXPECTED);
    console.log(`alchemy-gcc=${argument} ok host=${hostKey()} files=1 bytes=${statSync(PRET_EARLY_THUMB_DRIVER).size}`);
    return;
  }
  if (argument === "gcc2951") {
    validateExperimentalCompiler(argument, GCC2951_DRIVER, GCC2951_EXPECTED);
    console.log(`alchemy-gcc=${argument} ok host=${hostKey()} files=1 bytes=${statSync(GCC2951_DRIVER).size}`);
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
