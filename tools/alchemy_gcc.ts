#!/usr/bin/env bun
import { readdirSync, readFileSync, statSync } from "node:fs";
import { basename, dirname, extname, join, relative, resolve, sep } from "node:path";

export const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
// 承認済みコンパイラ束は姉妹リポジトリ PascalPixel/alchemy-gcc の dist/ に住む。
// ソースからの再構築手順は同リポジトリの build.sh、検証は下の validateBundle。
export const BUNDLE = join(ROOT, "alchemy-gcc", "dist");
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
export const GCC3_BUNDLE = join(BUNDLE, "gcc3");
export const GCC3_DRIVER = join(GCC3_BUNDLE, "cc1");
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
// Stock gcc-3.0 has no -fcall-used-r4 patch (that is gcc-2.96's Camelot ABI
// fork); it reserves the same register class the README documents as needed
// for gs2's Camelot backend mode: -ffixed-r7.
export const GCC3_CFLAGS = [
  "-O2", "-mthumb", "-mthumb-interwork", "-mcpu=arm7tdmi",
  "-fno-builtin", "-nostdinc", "-ffreestanding", "-ffixed-r7",
  `-I${join(ROOT, "include")}`,
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
// 0807a664 is a clean-room, source-routed backend fingerprint.  The valid C
// compaction source is exact only when the post-reload allocator preserves the
// reference's r4/r6 loop roles and the final table-pointer load order.
const THUMB_0807A664_SOURCES = new Set(["0807a664"]);
// This two-table signed lookup keeps its compact loop layout at -Os.  With
// jump-following CSE disabled, the resulting 124-byte owner matches exactly;
// the bounded compiler cohort found the same exact result independently with
// the already-evidenced two-insn-immediate CSE switch.
const OPTIMIZE_OS_SOURCES = new Set(["08019d2c"]);
// resource_3a7:0944 is a clean-room exact witness only at -O3.  Its two
// physical overlay veneer aliases and explicit delta/loop-tail dataflow match
// all 124 reference bytes under the routed compiler; the ordinary -O2 route
// grows the owner and fails the placement check.  Keep this source-scoped until
// an unrelated exact owner independently establishes a broader -O3 family.
const OPTIMIZE_O3_OVERLAY_SOURCES = new Set([
  "exact/resource_3a7_c_02000944.c",
]);
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
  // Transposed soft-double wrappers in resource_3a7 use the stock pop-PC
  // epilogue; preserve the same non-interworking ABI as their exact resource_3bf
  // siblings.
  "exact/resource_3a7_c_020013ac.c",
  "exact/resource_3a7_c_020013e4.c",
  // Three single-comparison predicates whose stems are already taken by
  // resource_373 and resource_3b2 rows that must keep interworking.
  "exact/resource_3a7_c_02001554.c",
  "exact/resource_3a7_c_02001740.c",
  "exact/resource_3bf_c_02005ae0.c",
  // Moved out of the stem set: resource_377 now has an interworking row at
  // 02001544, and the stem key would have stripped its interworking epilogue.
  "exact/resource_3a7_c_02001544.c",
  // These resource_3bf soft-double wrappers are stock-ABI library leaves whose
  // references return with pop {pc}; both bodies independently match once the
  // two ABI properties are selected.
  "exact/resource_3bf_c_02005a40.c",
  "exact/resource_3bf_c_02005a78.c",
  "exact/resource_3bf_c_02005c08.c",
  "exact/resource_3bf_c_02005af0.c",
  "semantic/resource_3bf_c_02005b6c.c",
  "exact/resource_3bf_c_02005b6c.c",
  "exact/resource_3a7_c_0200145c.c",
  "exact/resource_3a7_c_02001574.c",
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
  "08011590", "08029274",
  "080fb714", "080fb728", "080fb73c", "080fb750", "080fb75c",
  "080fb768", "080fb77c",
]);
// These four independently identical leaves need only the post-reload
// scheduler disabled: it keeps each increment load ahead of the corresponding
// value load. Their matching complete 54-byte owners demonstrate one shared
// compiler behaviour, so keep the evidence path-scoped rather than claiming
// the repeatedly used 02000ab0 stem globally.
const UNSCHEDULED_OVERLAY_SOURCES = new Set([
  "exact/resource_39f_c_02000ab0.c",
  "exact/resource_3b2_c_02000ab0.c",
  "exact/resource_3c4_c_02000ab0.c",
  "exact/resource_3c5_c_02000ab0.c",
  // Adopted before the second scheduler pass learned to issue a lone argument
  // immediate ahead of a ready literal-pool load.  The reference writes
  // `ldr r0, [pc]' first and `movs r1, #1' after it, which is what the
  // unscheduled order gives; the ROM blob, not the derived inventory, is the
  // witness (the asset rebuild was the thing that caught the drift).
  "exact/resource_3cb_c_02001050.c",
  "semantic/resource_3cb_c_02001050.c",
]);
// This decoder has mutually exclusive switch arms that reuse the same input
// base.  Following jumps during CSE rematerializes one arm's base in r3;
// disabling that pass preserves the reference's r6 lifetime and coalescing.
const NO_CSE_FOLLOW_SOURCES = new Set(["0800f9f4", "08019d2c"]);
// The state-mask leaf at 080108c4 rematerializes its two-instruction mask
// instead of sharing the equivalent value across the halfword load/store.
// The clean C witness is byte-exact only with this source-scoped CSE switch;
// keep it separate from the overlay path below because the overlay set is
// keyed by repository path.
const NO_CSE_TWO_INSN_IMMEDIATE_SOURCES = new Set(["080108c4"]);
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
// The second loop pass rematerializes the owner-state offset in 0807a0f4,
// breaking the reference object's reuse of the shifted 280-byte field offset.
const NO_RERUN_LOOP_OPT_SOURCES = new Set(["0807a0f4"]);
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
// 08090824 is the strict stack-zero DMA3 fill: the isolated compiler witness
// moves the saved zero before the DMA base load after grouped-DMA formation.
// Keep it on the same control-last route; the backend fingerprint is narrower
// than the generic grouped-store reorder and leaves existing owners unchanged.
// 080b010c writes its descriptor immediately after an allocator call: the
// reference copies the live source and destination into r0 and r1 before
// loading the pooled control word into r2, the same fingerprint as 08005a78.
const GROUP_CONTROL_LAST_SOURCES = new Set(["080c08a8", "08005a78", "08005c68", "080907b0", "08090824", "080b010c", "0808fe38", "080b0744"]);
// 080907b0's second descriptor has a strict value1/base scheduling fingerprint:
// the immediate, source-address add, base literal, shift, and control literal
// must be restored to the reference order after grouped-DMA formation.
const GROUP_VALUE1_BEFORE_BASE_SOURCES = new Set(["080907b0"]);
// 0801a4fc kicks its palette descriptor with the source pointer already live in
// a callee-saved register, so the copy that feeds the transfer's first stored
// word targets r0 rather than r1 and falls out of the control-last walk above.
const GROUP_POOLED_CONTROL_LAST_SOURCES = new Set(["0801a4fc"]);
// 0808b868 copies a pointer into ip between two independent low-register
// updates. The reference issues that copy before the adjacent immediate add;
// -fthumb-move-before-alu only covers low-to-low copies against register ALU
// operands, so this one needs the widened variant.
const HIGH_MOVE_BEFORE_ALU_SOURCES = new Set(["0808b868"]);
// 0801fd34 needs the other widening twice in one function: a load issued ahead
// of an immediate shift, and a low-register copy issued ahead of an immediate
// add. It also needs the pre-reload scheduler off.
const MOVE_BEFORE_IMMEDIATE_ALU_SOURCES = new Set(["0801fd34"]);
// 080b5d3c's inner loop preheader holds four insns: the object cursor and the
// counter come from the source, the totals base and the member offset are
// hoisted invariants. The reference runs the hoisted pair first; move_movables
// anchors them at the loop note, which puts them last.
const LOOP_INVARIANT_BLOCK_HEAD_SOURCES = new Set([
  "080a3354",
  "080b5d3c",
  "080a90bc",
]);

// Reconstructed 2026-08-07: Func_080a90bc needs the call-argument inversion
// widened to a register source, the pre-call constant setup sunk past the call,
// and a high-register copy issued before an adjacent negs.
const CALL_ARG0_REG_SOURCE_SOURCES = new Set(["080a90bc"]);
const SINK_CONSTANT_PAST_CALL_SOURCES = new Set(["080a90bc"]);
const MOVE_BEFORE_UNARY_ALU_SOURCES = new Set(["080a90bc"]);
// 0808fe38 allocates, zeroes a stack word through its own pointer, and kicks a
// grouped descriptor store whose saved-result and zero registers are not the
// r5/r6 pair the original repair hard-coded, so it needs the widened form.
const GROUP_ZERO_ANY_REGISTER_SOURCES = new Set(["0808fe38", "080c08a8"]);
// 0808fe38's two tail calls each take a long split immediate in r1 and a pooled
// function address in r0.  The scheduler parks the pool load inside the split;
// the reference keeps the split contiguous and loads r0 after it.
const ARG0_AFTER_SPLIT_SOURCES = new Set(["0808fe38"]);
const CALL_ARG0_POOL_LOAD_SOURCES = new Set(["0808fe38"]);
const RETURN_VALUE_BEFORE_STACK_ADJUST_SOURCES = new Set(["0808fecc"]);
const SINK_GROUP_POOL_LOADS_SOURCES = new Set(["080c08a8"]);
const SINK_STACK_ADJUST_SOURCES = new Set(["080c08a8"]);
const SINK_DEPENDENT_LOAD_SOURCES = new Set(["080c08a8"]);
const COLLAPSE_DEAD_SCRATCH_SOURCES = new Set(["0800fec8"]);
const SINK_BLOCK_CONSTANT_SOURCES = new Set(["0800430c"]);
const SINK_PAST_POOL_LOAD_SOURCES = new Set(["0800430c"]);
// 08006408 arms an interrupt-guarded request slot.  Its reference issues the
// slot store after the flag byte it shares no register with, and loads the two
// pooled addresses base-first, so the address is live before the byte store
// that uses it.  The whole sequence only lines up under one flag set, which
// this owner carries as a unit.
const SINK_STORE_PAST_STORE_SOURCES = new Set(["08006408"]);

// 080c0130 builds two DMA descriptor groups back to back.  Its reference forms
// the second group's source pointer as a copy followed by an add rather than an
// add in place, clusters the first group's two pooled constants ahead of the
// add caught between them, and otherwise materializes each group's operands in
// register order.  Post-reload scheduling has to be off for those orders to
// survive, so the owner carries the whole set as a unit.
const SINK_ADD_IMMEDIATE_SOURCES = new Set(["080c0130"]);

// 08011568 writes one DMA control halfword and then a three-word descriptor
// group through the same base pointer.  Its reference finishes adjusting the
// base before it materializes any of the group's pooled words, which needs the
// add-immediate lifted above the run of loads the scheduler puts in front of
// it, and it allocates the base out of a rotated low-register order.
const HOIST_ADD_IMMEDIATE_SOURCES = new Set(["08011568"]);

// 080cd358 calls three block movers with the same pooled addresses and the
// same 0x7800 size.  The reference rebuilds that size at every call instead
// of holding it in a register, which only this mode reproduces.
const NO_CONSTANT_REUSE_SOURCES = new Set(["080cd358"]);

// resource_3bd:0c98 writes the same three-word DMA descriptor after an object
// factory call.  Its reference copies the live source and destination into r0
// and r1 before loading the pooled control word into r2; the path-scoped mode
// preserves that order without changing unrelated overlays sharing 02000c98.
const GROUP_CONTROL_LAST_OVERLAY_SOURCES = new Set([
  "exact/resource_3bd_c_02000c98.c",
]);
// The descriptor's base pool load wins a priority-68 ready-list tie on forward
// dependent count alone; these references break it by original order instead.
// 08021d88 likewise needs original-order tie breaking for its frame adjustment
// and two split constants; its source order then reproduces the ROM exactly.
// 080903bc needs the same tie break for one callback-literal load versus the
// preceding state-byte store; its other 101 halfwords already agree.
// 08094730 has the same scheduler tell immediately before its grouped DMA
// descriptor; original-order tie breaking closes its last transposition.
const NO_SCHED_DEPEND_COUNT_SOURCES = new Set([
  // 080b0744 opens with a grouped DMA descriptor whose control-word load the
  // reference sinks; original-order tie breaking is what puts the source and
  // destination copies on the reference's side of that load.
  "080b0744",
  "08002fb0", "08003e10", "08004760", "08005340", "08005394", "080053e8", "0800d304", "08019bac", "08021d88", "080903bc", "080907b0", "08094730",
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
// later diagnostics. 0808fecc's residual is the smallest of the
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
  "080b0744",
  "0808fe38", "080c08a8",
  "08005c68", "080060e8", "08002f10", "08004838", "08004858", "080049e8", "08004a28", "08004a44",
  "08004a5c", "08004a94", "08005340", "08005394", "080053e8", "0800bc48", "0800bdd4", "0800c0f4", "0800d304", "08011590", "080170c4", "08019bac",
  "0801d014", "0801d980",
  "080251d4", "080284dc", "080907b0", "08090824", "08094730", "08095160", "08095290", "080958a8", "08097540", "0809bb34", "080c0184", "080c08a8",
  "0808fecc", "08004760", "08005a78", "080037d4", "080b5ad4", "0800300c", "080f377c",
  // 08091174 joins for the pool-load-late witness (2026-08-05): its group
  // forms under the mode and the sched tie-break closes the last 6 bytes.
  "08091174",
  "08002fb0", "08003e10",
  "080a1090",
  "080b010c",
  "0801a4fc",
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
// Overlay-safe counterpart. resource_38f:08ec has two ordinary byte-field
// updates where a constant OR input dies at the OR and the reference reuses
// that register for the result immediately stored to the field.
const ORR_DEAD_INPUT_REUSE_OVERLAY_SOURCES = new Set([
  "exact/resource_3a6_c_02001938.c",
  "exact/resource_39a_c_02001004.c",
  "exact/resource_38f_c_020008ec.c",
  "semantic/resource_38f_c_020008ec.c",
]);
// The same call sheet mostly follows the established low-destination scheduler
// fingerprint, with a repeated post-reload exception: an adjacent r1 setter
// fills the slot before a literal r0 setter. The mode recognizes the call-fed
// pair structurally and does not depend on this address.
// NOT extendable to constant pairs: several near-band owners (resource_377
// 0x020001e0, resource_399 0x020002b8, resource_3a2 0x02000ac0, resource_3ae
// 0x020002dc) each differ only by a `movs r0,#K / movs r1,#0' pair the
// reference emits in the opposite order.  A fork mode was built that ran the
// transform below without the scheduler-inversion gate and restricted to two
// literal setters (-fthumb-const-arg1-before-arg0).  It regressed every
// witness: resource_377 went 2 -> 8 halfwords because the SAME function wants
// r0 first at 0x0200019a, 0x020001a4, 0x020001de and 0x02000208 and r1 first
// only at 0x02000236.  The order therefore is not a property of the compiler's
// tie-break at all -- within one function it alternates -- so it must come from
// the original source's argument expressions, and no whole-function flag can
// model it.  The fork change was reverted and the digest restored.
// -fthumb-call-literal-arg1-first: the same transposition as
// -fthumb-call-arg1-before-arg0, but without that mode's "only undo a
// scheduler inversion" gate, and restricted to a pair of plain literals.  Some
// references write `movs r1,#b` ahead of `movs r0,#a` for a call the scheduler
// never touched, which the older mode cannot reach.
const CALL_LITERAL_ARG1_FIRST_OVERLAY_SOURCES = new Set([
  "semantic/resource_3ae_c_020002dc.c",
  "exact/resource_3ae_c_020002dc.c",
  "semantic/resource_3ae_c_0200051c.c",
  "exact/resource_3ae_c_0200051c.c",
  "semantic/resource_377_c_020001e0.c",
  "exact/resource_377_c_020001e0.c",
  "semantic/resource_3a2_c_02000ac0.c",
  "exact/resource_3a2_c_02000ac0.c",
]);

const CALL_ARG1_BEFORE_ARG0_OVERLAY_SOURCES = new Set([
  "exact/resource_38f_c_020008ec.c",
  // resource_3c3 0x02000730: same post-reload r1-before-r0 call-argument pair
  // at the 0x0200074e Func_02001078(9,0) site (movs r1,#0 before movs r0,#9).
  "exact/resource_3c3_c_02000730.c",
  // Pair-sweep exact, 2026-08-05, paired with -fsched-low-dest-first.
  "exact/resource_3c0_c_02000adc.c",
  "semantic/resource_3c0_c_02000adc.c",
]);
// Pair-sweep exact, 2026-08-05: resource_3ab:07f4 is byte-exact only under
// -O1 with lr treated as fixed. Both flags are overlay-first here; the sets
// stay path-keyed like every other overlay route so the collision lint can
// hold them to one source each.
const FIXED_LR_OVERLAY_SOURCES = new Set([
  "exact/resource_3ab_c_020007f4.c",
  "semantic/resource_3ab_c_020007f4.c",
  // Round-2 pair-sweep exact, 2026-08-05, paired with -fsched-low-dest-first.
  "exact/resource_385_c_02000a80.c",
  "semantic/resource_385_c_02000a80.c",
]);
const OPTIMIZE_O1_OVERLAY_SOURCES = new Set([
  "exact/resource_3ab_c_020007f4.c",
  "semantic/resource_3ab_c_020007f4.c",
]);
// This main-image scheduler fingerprint leaves an independent r0 call-argument
// copy immediately after a halfword store.  The reference places that copy
// before the store; the strict post-reload transform only moves a register
// copy when the next instruction is the same call, so the memory operation and
// flags remain untouched.  08077f70 is exact with this mode plus the paired
// no-alias/store-first scheduler settings (work/0807root/08077f70.c).
const CALL_ARG0_BEFORE_STORE_SOURCES = new Set(["08077f70"]);
// 08098b10's clean semantic source is exact when the post-call byte-state
// increment is retargeted to r2 by the strict post-reload compiler mode.
const POSTCALL_BYTE_INCREMENT_R2_SOURCES = new Set(["08098b10"]);
// Grouped-DMA control-word class, 2026-08-05 (fork commit cee872a).
// 080f377c: shared pool-class control word rematerialised per transfer plus
// the sched2 pool-load-late tie-break; 08091174: the tie-break alone.
const GROUP_CONTROL_REMATERIALIZE_SOURCES = new Set(["080f377c"]);
const SCHED_POOL_LOAD_LATE_SOURCES = new Set(["080f377c", "08091174"]);
// Thumb leaf link-register class, 2026-08-06.  THUMB_INITIAL_ELIMINATION_OFFSET
// asks thumb_far_jump_used_p before branch lengths exist, so any conditional
// branch reads as a far jump and the pessimistic answer is latched in
// cfun->machine->far_jump_used for the rest of the compilation.  Every Thumb
// leaf with a branch then saves lr it never needs.  -fthumb-leaf-no-lr
// suppresses that answer only where the frame is provably empty, so no
// elimination offset can move.
const THUMB_LEAF_NO_LR_SOURCES = new Set(["080f9a30", "080fa1ac", "080fa264"]);
// The reference compiler predates ifcvt.c (gcc 2.95 has no if-conversion pass
// at all), so a two-armed if/else stays two basic blocks instead of collapsing
// into a conditional move.  Measured over all 692 main-image owners the flag
// lowers residue on 47 and closes none by itself; it is a structural
// precondition, not a finisher.
const THUMB_NO_IF_CONVERT_SOURCES = new Set(["080f9a30", "080fa1ac", "080fa264"]);
// Thumb owners whose reference keeps the source order of two independent loads
// that the second scheduler swaps.  Only sched2 is disabled; the first pass
// still runs.  Witness 080f9a30.
const SCHED2_OFF_THUMB_SOURCES = new Set(["080f9a30"]);
// Eight digits for -mlow-reg-order=: the first four order r0-r3 in the entry
// block, the last four in every other block.  The port hands out {3, 2, 1, 0}
// everywhere, which shows up as a whole-function renaming of the low registers
// against an otherwise identical instruction sequence.  Sweeping all 576
// entry/default pairs is how these are found, not guessing.
const THUMB_LOW_REG_ORDER_SOURCES = new Map([
  ["080f9a30", "01231230"],
  ["080fa264", "30120123"],
]);
// Four digits for -mcallee-reg-order=: the order in which r4-r7 are handed
// out, replacing the `4, 5, 6, 7' run inside REG_ALLOC_ORDER.  The residual it
// answers is an owner that already matches the reference instruction for
// instruction with two call-saved pseudos sitting in each other's register,
// which no source form can move because the allocator's preference order picks
// it.  Witness 08092f84: r6 and r7 transposed in seven halfwords, exact under
// 0132 and no other permutation.
const THUMB_CALLEE_REG_ORDER_SOURCES = new Map([
  ["08092f84", "0132"],
  ["0801faa8", "0132"],
]);
// This no-argument initializer's reference fills the first global literal
// load's latency with the frame allocation and dependent load, then fills the
// table-index shift's latency with two stack initializers.  The compiler mode
// requires the exact hard-register, constant, stack-offset, and death-note
// fingerprint; keep it source-scoped.
const ENTRY_FRAME_CLUSTER_SOURCES = new Set(["0801c34c"]);
// 080b0744 allocates a one-word frame and takes four incoming arguments. The
// reference issues the argument copies first and only then `sub sp, #4';
// -mearly-frame-allocation only expresses the opposite direction, so this is
// its mirror. Owners are routed one at a time: the flag moves the stack
// decrement in every function that has one.
const LATE_FRAME_ALLOCATION_SOURCES = new Set(["080b0744"]);
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
const EARLY_LITERAL_POOL_OVERLAY_SOURCES = new Set(["02000e3c", "02000dfc", "02003a44"]);
// Path-scoped members: the 02000ee0 stem collides with default-flag
// adoptions in other overlays, so the resource_394 and resource_3bd members
// are routed by full path instead.
const EARLY_LITERAL_POOL_OVERLAY_PATHS = new Set([
  "exact/resource_394_c_02000ee0.c",
  "exact/resource_3bd_c_02000ee0.c",
]);
const NO_CANONICALIZE_COMPARISON_OVERLAY_SOURCES = new Set([
  "exact/resource_3a9_c_0200007c.c",
  "exact/resource_3a9_c_020000e4.c",
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
// land it one slot early, so this is the only flag that reaches it. The
// 020011bc address is shared by resource_3c6 and resource_3cb, so both owners
// are routed by full path below. Its sibling `02000178` has the same residual
// and is *not* routable: resource_3ba already owns that address.
// Keyed by stem, so every member here is also a claim that no other overlay has
// a converted row at the same offset. 02001984 was moved out to the path-keyed
// set below when resource_3b4 gained a row at that offset that the flag breaks.
const SCHED_LOW_DEST_FIRST_SOURCES = new Set([
  "08097540", "02001958", "02000260",
  // resource_3b1's flat setter-sequence unindexed rows: a shifted constant
  // argument (movs/lsls) with the callee's other args set between the two
  // halves. Verified no other overlay owns these stems (bare-address key).
  "02003dec", "02003e34", "02003d10", "020040e8",
  // resource_3a4:29dc — the r0 setter must precede the shifted x/z arguments
  // of the placement call. Stem verified unique across assets/code.
  "020029dc",
]);
const THUMB_IMMEDIATE_LATENCY_OVERLAY_SOURCES = new Set([
  // Keep the slot copy between the two halves of the 0xc000 heading setup.
  "exact/resource_3c6_c_02000078.c",
  // resource_3b7:0154 and :0178 are the same four-call sheet over two ids. The
  // third call takes -1, built as movs #1 then negs, and the reference sets the
  // second argument between those two halves; only the latency mode reproduces
  // that split. Neither tie-break direction reaches it.
  "exact/resource_3b7_c_02000154.c",
  "exact/resource_3b7_c_02000178.c",
  "exact/resource_383_c_02000428.c",
  // Paired with the callee-return-type lever: the return type fixed these
  // functions' movs/movs swaps and the latency mode their movs/lsls ones —
  // neither reaches zero alone.
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
  // resource_3b8:049c is byte-exact only with immediate-latency AND the
  // rerun-cse-after-loop rerun disabled together (notes/resource_3b8-049c.md).
  "exact/resource_3b8_c_0200049c.c",
  // resource_3b9:2668's shifted-constant calls place the independent id
  // literal (r0) between the two base movs and their shifts; only the
  // latency mode reproduces that slot.
  "exact/resource_3b9_c_02002668.c",
  // resource_3b9:2964 is the same call-sheet shape as resource_3b9:2668.
  "exact/resource_3b9_c_02002964.c",
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
  "exact/resource_3cb_c_02000128.c",
  // resource_38f:26cc reaches the reference instruction stream when the
  // original-order tie-break is paired with the one-cycle Thumb load model;
  // the pair flips its sole remaining two-insn load/use schedule.
  "exact/resource_38f_c_020026cc.c",
  // resource_398:04b4 sets r0 then r1 for a six-argument call; the dependence
  // count reverses that pair and neither tie-break direction restores it.
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
  // resource_3c4:1068 is the same heuristic on a load rather than a setter: the
  // dependence count hoists the +76 field's `ldr` above the store to +12, which
  // is the source's own order. Nothing else moves it, in either direction.
  "exact/resource_3c4_c_02001068.c",
]);
const THUMB_LOAD_LATENCY_ONE_OVERLAY_SOURCES = new Set([
  // resource_3aa:1494: with the default load model the scheduler drags the
  // workspace-slot pool load above the preceding call and permutes the pool;
  // the one-cycle model keeps it at its reference position.
  "exact/resource_3aa_c_02001494.c",
  "semantic/resource_3aa_c_02001494.c",
  "exact/resource_38f_c_020026cc.c",
  // resource_3bc:01c8's wait loop reads the frame counter through a workspace
  // pointer; the default model schedules the entry dereference away and folds
  // it into the loop (56 bytes), the one-cycle model keeps the ldr/ldr pair at
  // its reference entry position (60, byte-exact; derived-inventory mode
  // cohort, 2026-08-05).
  "exact/resource_3bc_c_020001c8.c",
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
  "exact/resource_3bd_c_02000c98.c",
  "exact/resource_3ca_c_020010d4.c",
  // Lane-D exacts, 2026-08-05: the resource_395 twins group under this mode
  // alone once the source ADDRESS is a local born before the descriptor
  // pointer (birth order gives from=r2, dma=r3, the reference economy).
  "exact/resource_395_c_02001838.c",
  "semantic/resource_395_c_02001838.c",
  "exact/resource_395_c_02001858.c",
  "semantic/resource_395_c_02001858.c",
  // resource_392:0c0c and resource_393:0ddc are exact-template twins of
  // resource_394:0fb4.  Keep the three descriptor stores on the compiler's
  // established grouped-DMA route; the per-overlay pools and veneers are the
  // only differences.
  "exact/resource_392_c_02000c0c.c",
  "exact/resource_393_c_02000ddc.c",
  "exact/resource_394_c_02000f34.c",
  "exact/resource_394_c_02000f54.c",
  "exact/resource_394_c_02000fb4.c",
  "exact/resource_393_c_02000d5c.c",
  "exact/resource_393_c_02000d7c.c",
  "exact/resource_392_c_02000b8c.c",
  "exact/resource_392_c_02000bac.c",
]);
const NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES = new Set([
  "exact/resource_383_c_0200082c.c",
  "exact/resource_3b1_c_02000b84.c",
  "exact/resource_3b1_c_02000cc8.c",
]);
// resource_372:0278 repeats the same 0x206 event id on opposite sides of a
// branch.  CSE's skip-blocks pass hoists it and rotates almost the whole leaf;
// disabling that pass restores the reference lifetimes, while the established
// low-destination scheduler tie-break fixes the remaining call-argument pair.
// Path scope matters because every decoded code overlay reuses 0x02000278.
const NO_CSE_SKIP_BLOCKS_OVERLAY_SOURCES = new Set([
  "exact/resource_372_c_02000278.c",
  // resource_398:0304 repeats the pooled 0x302 event id on both sides of the
  // gate branch; skip-blocks hoists the pool load into r6, while the
  // reference reloads it per site. Disabling the pass is byte-exact.
  "exact/resource_398_c_02000304.c",
  "semantic/resource_398_c_02000304.c",
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
  // resource_3a2:0924 writes 128<<9 and 128<<8 at two call sites each; the
  // fork caches both in callee-saved registers, which costs the owner an extra
  // `push {r6}' the reference does not have.  92 -> 18 differing halfwords,
  // 2026-08-07.
  "exact/resource_3a2_c_02000924.c",
  "semantic/resource_3a2_c_02000924.c",
  // resource_371:1888 and its two byte-identical siblings call a four-argument
  // import as (-1, -1, -1, 0); the fork CSEs the -1 into one register and
  // copies it out, where the reference materialises `movs rN, #1' into r0-r2
  // and negates each.  77 -> 31 differing halfwords, 2026-08-06.
  "exact/resource_371_c_02001888.c",
  "semantic/resource_371_c_02001888.c",
  "exact/resource_371_c_02001938.c",
  "semantic/resource_371_c_02001938.c",
  "exact/resource_371_c_020019e8.c",
  "semantic/resource_371_c_020019e8.c",
  // Reconstruction-wave (Sonnet) exacts, 2026-08-06: resource_39c:15e0 (two-
  // local stacked-argument staging law) and resource_3b1:413c (paired with
  // sched-low-dest-first, see that set).
  "exact/resource_39c_c_020015e0.c",
  "semantic/resource_39c_c_020015e0.c",
  "exact/resource_3b1_c_0200413c.c",
  "semantic/resource_3b1_c_0200413c.c",
  // resource_38f:08ec is a 732-call cutscene script. The reference rebuilds
  // recurring shifted immediates at their individual call sites rather than
  // carrying them through callee-saved registers across the whole owner.
  "exact/resource_38f_c_020008ec.c",
  "semantic/resource_38f_c_020008ec.c",
  // Pair-sweep exacts, 2026-08-05: these three owners are byte-exact only
  // under this flag together with -fsched-low-dest-first (see that set); the
  // full 2,547-pair configuration matrix found the pair and the adoption
  // dry-run confirmed each whole-overlay rebuild byte-identical.
  "exact/resource_3b9_c_02002904.c",
  "semantic/resource_3b9_c_02002904.c",
  "exact/resource_382_c_020012c0.c",
  "semantic/resource_382_c_020012c0.c",
  "exact/resource_385_c_02000d84.c",
  "semantic/resource_385_c_02000d84.c",
  // Lane-C, 2026-08-05: triple-route twins (see the pool-immediate set for
  // the r5-parking mechanism), the 3c4 placement twins, and
  // resource_3b9:25f0 whose full 120-byte pool-inclusive span is exact.
  // All pair with -fsched-low-dest-first.
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
  // The largest open overlay owner is a long event call sheet whose repeated
  // wide immediates are independently materialized throughout the reference.
  // Sharing them changes the saved-register set and every later pool boundary.
  "exact/resource_3b8_c_02002014.c",
  // resource_3bd:2c44 is another long scene call sheet. Its repeated shifted
  // immediates are rebuilt at their individual call sites in the reference.
  "exact/resource_3bd_c_02002c44.c",
  // resource_379:0074 is a 287-call cutscene sheet; mode cohort showed
  // cse-two-insn-immediate-off is the single largest gain (2133->1699 bytes),
  // matching the reference's per-site rematerialized shifted immediates.
  "exact/resource_379_c_02000074.c",
  "semantic/resource_379_c_02000074.c",
  // resource_371:1a98 rematerializes -1 three times (Func_02005eaa(-1,-1,-1,0))
  // at each argument in the reference; CSE shares it via one register and two
  // copies instead (mode_cohort: 158->88 differing bytes, 2026-08-04).
  "exact/resource_371_c_02001a98.c",
  "semantic/resource_371_c_02001a98.c",
  // resource_3aa:1494 is a 133-call cutscene sheet; the reference rebuilds
  // its repeated shifted immediates (258, 0x3000, 0x5000, -1, 0x03600000...)
  // at each call site instead of parking them in callee-saved registers.
  "exact/resource_3aa_c_02001494.c",
  "semantic/resource_3aa_c_02001494.c",
  // The two placement calls in resource_37b:1624 each rematerialize 0x800000;
  // sharing it introduces r5 and changes the whole call sequence.
  "exact/resource_37b_c_02001624.c",
  // resource_373:3380 independently materialises the repeated 0x10000/0x8000
  // scale pair.  CSE sharing changes the prologue and grows the owner by eight
  // bytes; the low-destination scheduler route below restores its call setup.
  "exact/resource_373_c_02003380.c",
  "exact/resource_373_c_020012bc.c",
  // resource_3bb:0b38 is the target-specific transpose of the exact
  // resource_3ba:0974 source; both independently materialize repeated wide
  // immediates before the low-destination call scheduler orders them.
  "exact/resource_3bb_c_02000b38.c",
  // Paired with the low-destination scheduler route below for resource_3c8's
  // three independently materialized -1 arguments.
  "exact/resource_3c8_c_020007d8.c",
  // resource_373:0cd0 has the sharing tell twice in one row: a three-argument
  // call whose first two arguments are both 0x30000, and a later one that takes
  // -1 twice. The reference builds each in place -- two `movs`/`lsls` pairs and
  // two `negs` -- where CSE builds one and copies it.
  "exact/resource_373_c_02000cd0.c",
  // resource_3b1:5c48 rematerializes -1 twice (Func_0200c0ee(-1,-1,0xe666))
  // and 0x20000 twice (Func_0200c116(0x20000,0x20000,0x10000)) independently
  // at each call site in the reference; CSE shares each pair via a register
  // copy instead, costing 4 bytes at each site.
  "exact/resource_3b1_c_02005c48.c",
  "semantic/resource_3b1_c_02005c48.c",
  // 0xC000 appears at two of this call sheet's three sites, so CSE hoists it
  // into a callee-saved register and buys a prologue the reference does not
  // have. Paired with -fsched-low-dest-first, which orders the r0 setter.
  "exact/resource_38d_c_02001984.c",
  // Same shape: 0x80000 feeds both of this call's shifted arguments.
  "exact/resource_3b4_c_020011d8.c",
  // resource_39f:2004 places the same 744/504 pair twice, once in whole units
  // and once in 16.16. CSE keeps 186 and 252 in r5/r6 across the whole body and
  // buys a `push {r5, r6, lr}` the reference does not have -- 34 groups from
  // that one decision. Paired with -fsched-low-dest-first for the r0 setter.
  "exact/resource_39f_c_02002004.c",
  // Same -1 pair shape as the entry two lines below; see the tie-break set.
  "exact/resource_3a2_c_020008a8.c",
  // Both negated arguments are -1, so CSE builds one and copies it.
  "exact/resource_3b5_c_0200028c.c",
  "exact/resource_372_c_02000f38.c",
  "exact/resource_3bf_c_02000bec.c",
  "exact/resource_3af_c_02001a98.c",
  "exact/resource_3af_c_02004218.c",
  // Parked before the mode existed, byte-exact under it with its existing draft
  // and no further source work.
  "exact/resource_3c8_c_020009c8.c",
  // Paired with -fsched-low-dest-first below: removing the constant sharing
  // exposes a scheduling transposition that the tie-break then fixes.
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
  // Four call sites share one 0xc000, built as `movs #192` + `lsls #8`. CSE
  // parks it in a callee-saved register and the tell is the prologue: the
  // reference pushes {r5, lr} and we push {r5, r6, lr}. The sibling at 0x140c
  // has four *different* constants and needs only the scheduler tie-break.
  "exact/resource_3aa_c_02001450.c",
  // resource_3b9:04c8 shares 258 (129<<1) across its two id-14 calls; the
  // reference rebuilds it at each site instead of parking it in r5.
  "exact/resource_3b9_c_020004c8.c",
  // resource_3b9:055c shares the flag id 0x3c1 across its test and set calls;
  // the reference reloads the pool word at each site instead of keeping the
  // address's dereferenced value live in r5 across the body.
  "exact/resource_3b9_c_0200055c.c",
  // resource_3b9:2904 shares the shifted displacement 428 (214<<1) across
  // three calls; the reference rebuilds it at each site instead of parking
  // it in r5 (tell: reference pushes {lr}, candidate pushes {r5, lr}).
  "exact/resource_3b9_c_02002904.c",
  // resource_3b9:2668 shares the (0x10000,0x8000) scale pair four times and
  // the 808 (202<<2) displacement three times across its long call sheet;
  // the reference rebuilds each independently instead of parking them.
  "exact/resource_3b9_c_02002668.c",
  // resource_3b9:2964 is the same call-sheet shape as resource_3b9:2668.
  "exact/resource_3b9_c_02002964.c",
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
  // resource_3a2:0924 — six of its call sites want the `movs r0,#X' setter
  // emitted before the `lsls r1,...' half of a shifted constant argument.
  // Pairs cohort put this flag at differing=13; paired with
  // cse-two-insn-immediate-off (see that set), 2026-08-07.
  "semantic/resource_3a2_c_02000924.c",
  "exact/resource_3a2_c_02000924.c",
  // resource_372:3e48 — the `movs r0,#8' setter before the `lsls r1' half at
  // 0x02003edc.  Paired with call-pool-arg1-first (see that set), 2026-08-07.
  "semantic/resource_372_c_02003e48.c",
  "exact/resource_372_c_02003e48.c",
  // Exact once the halfword store goes through a pointer local and an s32 value
  // local (tell #18/#19); the only residue was the four-argument call at 0x12,
  // whose r0 setter the reference issues between the two split constants.
  // Singles cohort exact, 2026-08-07.
  "semantic/resource_3a3_c_02000d08.c",
  "exact/resource_3a3_c_02000d08.c",
  // Singles cohort over the low-differing tail, 2026-08-07: this flag is the
  // single largest improvement for all three of these owners (20->2, 21->10 and
  // 21->15 differing bytes respectively) with no source change.
  "semantic/resource_3c8_c_02002f30.c",
  "exact/resource_3c8_c_02002f30.c",
  "semantic/resource_3c6_c_02000158.c",
  "exact/resource_3c6_c_02000158.c",
  "semantic/resource_39f_c_02000f94.c",
  "exact/resource_39f_c_02000f94.c",
  // resource_37f:17c0 (748 bytes) — found by a 139-candidate singles cohort over
  // the whole same-size band; this one flag takes it from 36 differing halfwords
  // to byte-exact, no source change.
  "semantic/resource_37f_c_020017c0.c",
  "exact/resource_37f_c_020017c0.c",
  // resource_394:07e0 needed the argument-feeding-store exclusion refinement
  // to this flag (fork commit 25b15cd) plus cse-pool-immediate-off (see that
  // set) to reach byte-exact; 2026-08-06.
  "exact/resource_394_c_020007e0.c",
  "semantic/resource_394_c_020007e0.c",
  // Reconstruction-wave (Sonnet) exacts, 2026-08-06: resource_3b1:413c
  // (paired with cse-two-insn-immediate-off) and resource_3b1:0728 (paired
  // with cse-pool-immediate-off, see that set).
  "exact/resource_3b1_c_0200413c.c",
  "semantic/resource_3b1_c_0200413c.c",
  "exact/resource_3b1_c_02000728.c",
  "semantic/resource_3b1_c_02000728.c",
  // resource_3c8:2f30 (paired with -fno-cse-shift-immediate above): the
  // reference sets r0,#0 before each shifted r1/r2 immediate build at every
  // three-argument call site, same low-destination tie-break tell as
  // resource_39e:2484 (probed exact, 2026-08-04).
  "exact/resource_3c8_c_02002f30.c",
  "semantic/resource_3c8_c_02002f30.c",
  // Tier-1 probe batch, 2026-08-06.  Same tell as the entries above -- the
  // reference issues the r0 argument setter before the r1 one at nearly every
  // multi-argument call site, the fork the other way round -- so these route
  // here, but each is STILL OPEN at 2-4 differing halfwords.  The whole
  // residual in every case is one site whose r1 setter is a literal zero,
  // which the reference issues BEFORE the r0 setter even though sibling sites
  // in the same function (including other `(10, 0)' calls) keep r0 first.
  // -fsched-call-arg1-before-arg0 does not fire on it: that mode's structural
  // recogniser wants a post-reload pair and leaves these untouched, and on
  // resource_3a2:0ac0 it costs the pool-load site as well (2 -> 4).  Do not
  // hand-permute; this is the model-divergence tier.
  // resource_377:01e0 and resource_3a2:0ac0 closed on 2026-08-07 once
  // -fthumb-call-literal-arg1-first grew its two discriminators (call uses
  // exactly r0+r1; the two literals differ), which is precisely what
  // separates the transposed site from the r0-first siblings described above.
  "semantic/resource_377_c_020001e0.c",
  "exact/resource_377_c_020001e0.c",
  "semantic/resource_3a2_c_02000ac0.c",
  "exact/resource_3a2_c_02000ac0.c",
  // resource_3ae:0144 (with -fno-cse-shift-immediate), b3 cohort 2026-08-07.
  "semantic/resource_3ae_c_02000144.c",
  "exact/resource_3ae_c_02000144.c",
  // resource_3ae:02dc closed on 2026-08-07: this mode plus
  // -fthumb-call-literal-arg1-first plus splitting the nested call
  // `Outer(Inner(0))' into two statements in reference order.
  "semantic/resource_3ae_c_020002dc.c",
  "exact/resource_3ae_c_020002dc.c",
  "semantic/resource_399_c_020002b8.c",
  "semantic/resource_39f_c_020021b0.c",
  "exact/resource_39f_c_020021b0.c",
  // Tier-2 cohort sweep, 2026-08-06: the same argument-setter ordering tell
  // reaches well past the near-exact band.  These three were at 12, 13 and 14
  // differing halfwords and go byte-exact under this mode alone.
  "exact/resource_38b_c_020009cc.c",
  "semantic/resource_38b_c_020009cc.c",
  "exact/resource_38d_c_02000568.c",
  "semantic/resource_38d_c_02000568.c",
  "exact/resource_38d_c_020005f4.c",
  "semantic/resource_38d_c_020005f4.c",
  // resource_37b:0c8c (548 bytes, was 22 halfwords) goes byte-exact here too.
  "exact/resource_37b_c_02000c8c.c",
  "semantic/resource_37b_c_02000c8c.c",
  // Still open, but strictly better routed here: resource_37f:0154 13 -> 8,
  // resource_372:150c 23 -> 18 (stacked on -fno-cse-shift-immediate).
  "exact/resource_372_c_0200150c.c",
  "semantic/resource_372_c_0200150c.c",
  "exact/resource_37f_c_02000154.c",
  "semantic/resource_37f_c_02000154.c",
  // Pair-sweep exacts, 2026-08-05, paired with -fno-cse-two-insn-immediate
  // (see that set's matching entries).
  "exact/resource_3b9_c_02002904.c",
  "semantic/resource_3b9_c_02002904.c",
  "exact/resource_382_c_020012c0.c",
  "semantic/resource_382_c_020012c0.c",
  "exact/resource_385_c_02000d84.c",
  "semantic/resource_385_c_02000d84.c",
  // Pair-sweep exact, 2026-08-05, paired with -fthumb-call-arg1-before-arg0.
  "exact/resource_3c0_c_02000adc.c",
  "semantic/resource_3c0_c_02000adc.c",
  // Lane-E exact, 2026-08-05: resource_389:09dc's remaining movs/lsls swap
  // is the low-destination scheduler tie-break (single flag suffices).
  "exact/resource_389_c_020009dc.c",
  "semantic/resource_389_c_020009dc.c",
  // Round-2 pair-sweep exacts, 2026-08-05: paired with
  // -fno-cse-shift-immediate (3b9:04c8, 3bf:206c) and -ffixed-r14
  // (385:0a80); see those sets.
  "exact/resource_3b9_c_020004c8.c",
  "semantic/resource_3b9_c_020004c8.c",
  "exact/resource_3bf_c_0200206c.c",
  "semantic/resource_3bf_c_0200206c.c",
  "exact/resource_385_c_02000a80.c",
  "semantic/resource_385_c_02000a80.c",
  // Lane-C, 2026-08-05: the triple-route twins, the 3c4 placement twins,
  // and resource_3b9:25f0 (mechanisms at their NO_CSE_* entries).
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
  // Reconstruction-wave exacts, 2026-08-05: resource_3b4:2334 (paired with
  // cse-shift-immediate-off) and resource_3b4:0ad0 (paired with
  // cse-pool-immediate-off + cse-shift-immediate-off) stop the two-insn-
  // immediate hoist and restore the movs interleave; resource_3ab:0c10/0b0c
  // sink a negs/lsls below argument staging; resource_3ab:15bc (paired with
  // cse-shift-immediate-off) supplies the interleave after the control-word
  // rematerialisation gate stops the cross-call CSE; resource_3c4:1aba
  // (paired with cfg-thread-jumps-off + cse-shift-immediate-off) is the
  // same interleave after jump-threading is disabled; resource_3b2:1214
  // swaps the two pool ldrs and hence their pool word order.
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
  // resource_3c1:0120 and :0194 (byte-identical twins) negate the shake
  // argument r2 for a three-argument call; the reference sets r0/r1 before
  // the negs, the low-destination tie-break, same tell as resource_38e:045c.

  // 020011bc is shared by resource_3c6 and resource_3cb; preserve the
  // scheduler route for both without leaking it to any future twin.
  "exact/resource_3c6_c_020011bc.c",
  // resource_3c6:0158 has two mirrored three-argument transition calls. Once
  // their per-site veneer identities are preserved, the remaining scheduler
  // tell is the same ascending r0/r1/r2 argument setup used by this cohort.
  "exact/resource_3c6_c_02000158.c",
  "exact/resource_3cb_c_020011bc.c",
  // The same resource_38f call sheet consistently orders tied r0-r2 argument
  // setters by ascending destination once its constants are rematerialized.
  "exact/resource_38f_c_020008ec.c",
  // resource_38f:0304 case-9 arm: the three-argument Func_02002dd8 call sets
  // movs r0,#0 between the r1/r2 immediate shifts; the low-destination
  // tie-break closes the last two halfwords (mode cohort exact under
  // sched-low-dest-first, 2026-08-04).
  "exact/resource_38f_c_02000304.c",
  "semantic/resource_38f_c_020008ec.c",
  // resource_379:0074, the same long call-sheet shape as resource_38f:08ec:
  // once both CSE gates rematerialize its constants, the remaining tell is the
  // movs of a lower argument register between the r1/r2 immediate shifts.
  "exact/resource_379_c_02000074.c",
  // Paired with both CSE gates for resource_3b8's long event call sheet; once
  // constants are rematerialized, this restores the reference argument order.
  "exact/resource_3b8_c_02002014.c",
  // Paired with both CSE gates for resource_3bd:2c44; its 251 call sites use
  // the same proven low-destination tie-break after rematerialization.
  "exact/resource_3bd_c_02002c44.c",
  // Near-twin of resource_394:08b0.  Its shared straight-line call sequence
  // has the same proven tied-argument ordering; paired with the pool-CSE route.
  "exact/resource_394_c_020007e0.c",
  "exact/resource_373_c_02003380.c",
  "exact/resource_373_c_020012bc.c",
  "exact/resource_3b5_c_02000644.c",
  // resource_371:281c: once the per-site local veneer symbols replace the
  // resolved far names, the only residue is movs r0,#55 landing between the
  // r1 immediate's movs/lsls halves -- the same low-destination tie-break
  // (mode cohort by hand, no inventory row for this address-only owner,
  // 2026-08-04).
  "exact/resource_371_c_0200281c.c",
  // resource_371:2858, twin of :281c's dialogue call sheet: same low-dest
  // tie-break moves each movs r0,#0 ahead of its sibling immediate synthesis
  // (mode cohort by hand, no inventory row, 2026-08-04).
  "exact/resource_371_c_02002858.c",
  "exact/resource_3bb_c_02000b38.c",
  // After the paired CSE route rematerializes resource_3c8:07d8's three -1
  // arguments, this restores its final five-instruction load/store setup.
  "exact/resource_3c8_c_020007d8.c",
  // Paired with the skip-blocks CSE route documented below; after the event-id
  // lifetime is restored this orders the final tied call setters exactly.
  "exact/resource_372_c_02000278.c",
  // Both overlays now own an exact-C function at 0x020011d8. The address-only
  // route became ambiguous as soon as resource_373 joined resource_3b4, so
  // preserve the proven scheduler choice without leaking it to future twins.
  "exact/resource_373_c_020011d8.c",
  "exact/resource_3b4_c_020011d8.c",
  // resource_3aa:140c and :1450 are the sibling pair in HANDOVER 0: four
  // three-argument setter calls, then a guarded wait. Void callees put r0 and
  // r2 in the wrong order at every site and the low-destination tie-break puts
  // them back. :1450 needs -fno-cse-two-insn-immediate as well, because its
  // four constants are the same 0xc000; :140c's four differ and it does not.
  "exact/resource_3aa_c_0200140c.c",
  "exact/resource_3aa_c_02001450.c",
  // resource_3aa:1494 is the long cutscene sheet next door: the same setters
  // want r0 set before the r1/r2 pool loads at every call site.
  "exact/resource_3aa_c_02001494.c",
  "semantic/resource_3aa_c_02001494.c",
  // resource_3aa:0184 is the same tell across sixteen call sites: the setters
  // for r0/r1/r2 tie and the reference orders them by ascending destination.
  "exact/resource_3aa_c_02000184.c",
  // Moved here from the stem-keyed set: resource_3b4 now has a row at 02001984
  // whose six-argument call wants the reference's own order, and the stem key
  // would have applied resource_38d's flag to it.
  "exact/resource_38d_c_02001984.c",
  // resource_3bf:0c78 sets r0, r1 and r2 for one call and the reference orders
  // them by ascending destination; without the tie-break r0 lands last.
  "exact/resource_3bf_c_02000c78.c",
  // resource_39f:2004's last placement sets r0, r1 and r2 and the reference
  // puts `movs r0,#22` ahead of the two finishing `lsls`. See the CSE set.
  "exact/resource_39f_c_02002004.c",
  // resource_38d:0150 has the same tie at three of its eleven sites: a shifted
  // r1 argument finishing against a plain `movs r0,#14`.
  "exact/resource_38d_c_02000150.c",
  // resource_39a:17a8 sets r0/r1/r2 at two three-argument calls and the
  // reference orders them by ascending destination at both.
  "exact/resource_39a_c_020017a8.c",
  // resource_383:19a4 sets r0/r1/r2 at two three-argument calls; ascending
  // destination order at both.
  "exact/resource_383_c_020019a4.c",
  "exact/resource_3bf_c_02000bac.c",
  // resource_3b6:0898 sets r0/r1/r2 for a three-argument call; ascending
  // destination order.
  "exact/resource_3b6_c_02000898.c",
  // resource_3b5:0568 is the same three-argument setter shape.
  "exact/resource_3b5_c_02000568.c",
  // resource_3b5:0224 sets r0/r1/r2 for a four-argument call; ascending
  // destination order.
  "exact/resource_3b5_c_02000224.c",
  "exact/resource_3b5_c_02000528.c",
  // resource_3a2:0870 and :08a8 each set r0/r1/r2 for a three-argument call and
  // the reference orders them by ascending destination. :08a8 also needs the
  // constant-sharing mode: both of its negated arguments are -1, so CSE builds
  // one and copies it where the reference negates each in place.
  "exact/resource_3a2_c_02000870.c",
  "exact/resource_3a2_c_020008a8.c",
  "exact/resource_3a2_c_02000180.c",
  "exact/resource_3c8_c_02001780.c",
  "exact/resource_3c8_c_02001150.c",
  "exact/resource_372_c_02000f38.c",
  // Needs the tie-break alone, without the paired constant-sharing mode: two
  // argument setters tie before a `bl` and the low-destination rule picks
  // `mov r0,sl` over `lsls r1,r1,#1` (notes/resource_381-0e30.md).
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
  // resource_38c:01a8 and :0430 are the same beat over actors 21 and 23: the
  // last call takes (id, 192 << 8, 10), so the `lsls` that finishes r1 ties
  // with the `movs` that sets r0, and the reference takes r0 first.
  "exact/resource_38c_c_020001a8.c",
  "exact/resource_38c_c_02000430.c",
  // Same tell twice in resource_38c:035c, whose other lever is the rerun-cse
  // one; the flags are independent and both are needed.
  "exact/resource_38c_c_0200035c.c",
  // Moved out of the stem-keyed set: 0200028c was added for resource_3b5 and
  // three more overlays have since gained a row at that offset (38d, 3bb, 399),
  // which the bare-address key was silently handing the flag to. Found by the
  // collision scan `--lint` now runs.
  "exact/resource_3b5_c_0200028c.c",
  // resource_372:198c sets r0 for a three-argument call whose r1 and r2 are both
  // finished by a `lsls`, so both shifts tie with the r0 setter.
  "exact/resource_372_c_0200198c.c",
  // resource_39f:1818 sets r0 for a three-argument call whose r1 and r2 are both
  // finished by a `lsls #17`, so both shifts tie with the r0 setter.
  "exact/resource_39f_c_02001818.c",
  // resource_39f:0d90 sets r0 for a three-argument call whose r1 is finished by
  // a `lsls`, so the shift ties with the r0 setter.
  "exact/resource_39f_c_02000d90.c",
  // resource_38e:045c has the tell at two sites: a shifted-constant pair and a
  // negated argument, both tying with the r0 setter.
  "exact/resource_38e_c_0200045c.c",
  // resource_3a3:06a4 sets r1 and negates r2 for a three-argument call and the
  // reference takes the r1 setter first.
  "exact/resource_3a3_c_020006a4.c",
  // resource_3c6:010c has the tell at two call sites; its sibling :0078 has the
  // same flag need but a pool-load residual on top and stays unconverted.
  "exact/resource_3c6_c_0200010c.c",
  // resource_39a:1ad0 sets r0 for a three-argument call whose r1 and r2 are both
  // finished by a `lsls`, so both shifts tie with the r0 setter.
  "exact/resource_39a_c_02001ad0.c",
  // resource_3c3:0288 sets r1 and negates r2 for a three-argument call and the
  // reference takes the r1 setter first.
  "exact/resource_3c3_c_02000288.c",
  // resource_386:0204 and :011c, same plain form. :011c also needed one callee
  // declared `s32` -- the reference sets r1 before r0 at that site, which only a
  // value-returning callee does; the flag alone leaves that pair swapped.
  "exact/resource_386_c_02000204.c",
  "exact/resource_386_c_0200011c.c",
  // resource_383:091c and :19e4 are the plain form of the tell: one call takes a
  // shifted constant and the `lsls` that finishes r1 ties with the `movs` that
  // sets r0. Both are transcriptions of the semantic sources, which name
  // this overlay's callees by veneer rather than raw, so every site was renamed.
  "exact/resource_383_c_0200091c.c",
  "exact/resource_383_c_020019e4.c",
  // resource_38c:0124, :01e0 and :0250 close the same overlay's family; each has
  // one `(id, 128 << k, n)` call where the finishing `lsls` ties with the `movs`
  // that sets r0. Their other half is a declaration, not a flag: the two-argument
  // callee that follows the single-argument (10) call returns a value, so its
  // setters leave r1 before r0 (HANDOVER 4, first lever).
  "exact/resource_38c_c_02000124.c",
  "exact/resource_38c_c_020001e0.c",
  "exact/resource_38c_c_02000250.c",
  "exact/resource_3b8_c_02003df8.c",
  "exact/resource_3b8_c_02003e40.c",
  // resource_373: this overlay's real fingerprint is the low-dest tie-break,
  // not the rerun-cse mode (measured neutral on ~15 functions here).
  "exact/resource_373_c_02000dc0.c",
  "exact/resource_373_c_02000e54.c",
  "exact/resource_373_c_02000e84.c",
  "exact/resource_373_c_02000f5c.c",
  "exact/resource_373_c_02000f8c.c",
  "exact/resource_373_c_02000fbc.c",
  "exact/resource_373_c_02000fec.c",
  "exact/resource_373_c_02001490.c",
  // resource_37b members: routed per function, NOT overlay-wide — this mode
  // regresses that overlay's three large word-store sheets (02001c14, 02001d14,
  // 02001e10), which are exact at default flags.
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
  // resource_39e:2484 has two three-argument calls that set movs r0,#0
  // between the r1/r2 immediate shifts, same low-destination tie-break tell.
  "exact/resource_39e_c_02002484.c",
  // resource_39e:268c/2778: same movs-r0-between-shifts low-destination
  // tell on their three-argument calls.
  "exact/resource_39e_c_0200268c.c",
  "exact/resource_39e_c_02002778.c",
  // resource_3bf's four sibling scene-transition sheets (0xce0/0xdcc/0xe80/
  // 0xf30): after -fno-cse-shift-immediate rebuilds their duplicated
  // 192<<10 arguments per site, one adds-r1-from-r6 / pool-ldr-r2
  // transposition remains at the (-1,-1,pool) call and the low-destination
  // tie-break puts it back (probed exact 2026-08-04).
  "exact/resource_3bf_c_02000ce0.c",
  "semantic/resource_3bf_c_02000ce0.c",
  "exact/resource_3bf_c_02000dcc.c",
  "semantic/resource_3bf_c_02000dcc.c",
  "exact/resource_3bf_c_02000e80.c",
  "semantic/resource_3bf_c_02000e80.c",
  "exact/resource_3bf_c_02000f30.c",
  "semantic/resource_3bf_c_02000f30.c",
  // resource_382:0ef4's first call takes (0, 0x8000, 0x4000) and the reference
  // sets movs r0,#0 between the r1/r2 immediate shifts -- the plain form of
  // this tell (derived-inventory mode cohort exact under sched-low-dest-first,
  // 2026-08-05).
  "exact/resource_382_c_02000ef4.c",
  // resource_3c9:1280's if-arm ends with (arg0, 0xcccc, 0x6666): the reference
  // sets r0 from r5 before the two pool loads that build r1/r2 (same cohort
  // probe, 2026-08-05).
  "exact/resource_3c9_c_02001280.c",
]);
// The reference objects for these owners re-materialise their shifted
// (`movs rN,#K / lsls rN,rN,#n`, constraint K) immediates at every use site
// while still sharing negated (`movs/negs`, constraint J) immediates of the
// same function in a callee-saved register -- a split
// -fno-cse-two-insn-immediate (which covers J and K together) cannot spell.
// -fno-cse-shift-immediate is the K-only fraction of that flag; see
// alchemy-gcc/gcc-2.96/gcc/config/arm/arm.h CSE_CONSTANT_CLASS.  All four
// resource_3bf siblings share one fingerprint: Func(0x30000, 0x30000,
// 0x10000) rebuilt per-argument in the reference, Func(-1, -1, pool) sharing
// r6, verified byte-exact under the pair with -fsched-low-dest-first.
const NO_CSE_SHIFT_IMMEDIATE_OVERLAY_SOURCES = new Set([
  // resource_3ae:0144, found by the b3 residual cohort on 2026-08-07: shifted
  // immediate rebuilt at each use in the reference, plus the low-dest-first
  // schedule.  Needs both modes together.
  "semantic/resource_3ae_c_02000144.c",
  "exact/resource_3ae_c_02000144.c",
  // resource_3cb:02d8 caches its 0x80<<2 (0x200) in r5 across five calls and
  // pays a `push {r5}' for it; the reference rebuilds the pair at both use
  // sites and pushes only lr.  2026-08-06 probe.
  "exact/resource_3cb_c_020002d8.c",
  "semantic/resource_3cb_c_020002d8.c",
  // resource_372:150c shows the identical tell (`movs r5,#131 / lsls r5,#1'
  // cached and reused via `adds r1, r5, #0'; the reference rebuilds the pair).
  "exact/resource_372_c_0200150c.c",
  "semantic/resource_372_c_0200150c.c",
  // resource_3bc: 15 owners re-materialise a shifted immediate at every call
  // site in the reference while the semantic C shares it in a register,
  // overflowing the registered span by 4-36 bytes before this flag; probed
  // exact-shape-restoring 2026-08-04 (overflow resolves; residual differing
  // bytes handled per-owner from here).
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
  // resource_371:17a4 rebuilds its shared 512 (128<<2) call argument at each
  // of its two call sites in the reference instead of caching it across the
  // intervening calls; -fno-cse-shift-immediate alone takes this from 44 to
  // 8 differing bytes (mode_cohort, 2026-08-04).
  "exact/resource_371_c_020017a4.c",
  "semantic/resource_371_c_020017a4.c",
  // resource_3c8:2f30 rebuilds its shared 0x80<<7 (0x4000) call argument at
  // each of its two use sites (sub_02007df6's r2, sub_02007eb8's r1)
  // instead of caching it in a register across the intervening call;
  // takes it from a 2-byte span overflow to byte-exact once paired with the
  // per-site sub_ call symbols (probed exact, 2026-08-04).
  "exact/resource_3c8_c_02002f30.c",
  "semantic/resource_3c8_c_02002f30.c",
  // Round-2 pair-sweep exacts, 2026-08-05: byte-exact under this flag with
  // -fsched-low-dest-first (see that set). Reachable locally only after the
  // darwin cc1 parity restage exposed -fno-cse-shift-immediate on this host.
  "exact/resource_3b9_c_020004c8.c",
  "semantic/resource_3b9_c_020004c8.c",
  "exact/resource_3bf_c_0200206c.c",
  "semantic/resource_3bf_c_0200206c.c",
  // Reconstruction-wave exacts, 2026-08-05, paired with sched-low-dest-first
  // (and cse-pool-immediate-off / cfg-thread-jumps-off where noted):
  // resource_3b4:2334/0ad0, resource_3ab:15bc, resource_3c4:1aba.
  "exact/resource_3b4_c_02002334.c",
  "semantic/resource_3b4_c_02002334.c",
  "exact/resource_3b4_c_02000ad0.c",
  "semantic/resource_3b4_c_02000ad0.c",
  "exact/resource_3ab_c_020015bc.c",
  "semantic/resource_3ab_c_020015bc.c",
  "exact/resource_3c4_c_02001aba.c",
  "semantic/resource_3c4_c_02001aba.c",
]);
// Lane-C exacts routed through SCHED_LOW_DEST_FIRST above plus the CSE sets:
// the 382:1238 / 385:09f8 triples and the 3c4 / 3b9 pairs are annotated at
// their NO_CSE_* entries.
// resource_372:0ec4's five module-local calls make r7 unavailable in the
// reference allocation.  Reserving it restores the exact saved-register set;
// the high-destination tie-break then orders the remaining r6 constant and
// high-register move.  Both routes are path-scoped because offsets repeat in
// every decoded code-overlay namespace.
const FIXED_R7_OVERLAY_SOURCES = new Set([
  "exact/resource_372_c_02000ec4.c",
]);
const SCHED_HIGH_DEST_FIRST_OVERLAY_SOURCES = new Set([
  "exact/resource_372_c_02000ec4.c",
]);
// The fork proves a store and a later load at two different constant offsets off
// one base independent, leaves no edge between them, and lets the load's longer
// dependence chain outrank the store; the reference keeps source order. The mode
// forces the conflict when neither MEM is RTX_UNCHANGING_P, adding the edge as
// REG_DEP_ANTI so it orders without adding cost — a true dependence lengthens
// the store's path to the block end and regresses resource_381:2e0c.
// Keyed by repository path, so a main-image `exact/` source routes here too —
// the name predates the exact/semantic tree consolidation. `08078144` stores
// a halfword and then reloads a word at a different constant offset off one
// base; the fork proves them independent and hoists the load, where the
// reference keeps source order.
// -fsched-call-dest-descending is -fsched-low-dest-first's gate with the
// comparison negated: the same argument setters in front of a call, ordered
// highest register first.  NEGATIVE RESULT, 2026-08-06: the flag exists in the
// fork (commit 1e1502b) and works, but no source routes to it and none should.
// It was built to close resource_3c8:2f30, whose residual is the pair
// `lsls r2, #18' / `lsls r0, #18' either side of a `movs r1, #0'.  Measured on
// that owner: routed low-dest-first 2 differing halfwords, descending 14,
// neither 11 -- because the reference mixes the two directions inside one
// function (ascending at its first three call sites, descending at the fourth).
// A global direction therefore cannot be the model; do not re-derive this.
const SCHED_CALL_DEST_DESCENDING_OVERLAY_SOURCES = new Set<string>([
  // The negative result above holds for resource_3c8:2f30 and for any owner
  // whose call sites disagree with each other -- but "none should" was one
  // owner's evidence generalized. resource_3bb:39fc is a genuine witness: its
  // staging emits `asrs r0,#16` before `lsls r1,#2` where the reference
  // completes the r1 shift first, and no source shape reorders it (declaration
  // order in an inner block was tried both ways and moved nothing). Routed
  // 2026-08-07 from a green verify; the mode stays default-off and per-source,
  // so 3c8:2f30 is unaffected.
  "exact/resource_3bb_c_020039fc.c",
]);
const NO_SCHED_ALIAS_OVERLAY_SOURCES = new Set([
  "exact/08078144.c",
  "exact/resource_3af_c_02002b7c.c",
  "exact/resource_3b0_c_02000030.c",
  "exact/resource_381_c_02002e0c.c",
  "exact/resource_381_c_02002e5c.c",
  // resource_38f:27ac particle-spawn store sheet: the scheduler otherwise
  // hoists the p->f50 load above the p->f0c store and reorders the
  // f08/f10/f26 stores and the f09 mask's ldrb; conservative scheduler alias
  // analysis restores the reference order (mode cohort exact, 2026-08-04).
  "exact/resource_38f_c_020027ac.c",
  // resource_391:2974 is the same particle-spawn store sheet, shared verbatim
  // with resource_38f:27ac and transposed onto this overlay's veneer table and
  // literal pool; it needs the same conservative alias analysis.
  "exact/resource_391_c_02002974.c",
]);
// gcse's partial-redundancy elimination inserts a load the reference does not
// have. The mode drops the insert and delete bits of any expression that reads
// non-RTX_UNCHANGING_P memory and would need an insertion, clearing both maps as
// a pair because PRE only deletes an occurrence that an insertion made
// available. Constant-pool loads keep their bits and are still eliminated. This
// is the narrowest of the four gates: 9 of 1,335 sources change.
// Keyed by repository path, so a main-image `exact/` source routes here too;
// the name predates the exact/semantic tree consolidation. 0807a550 is that
// member: gcse's partial-redundancy pass inserts a load the reference does
// not have, and the remaining seven halfwords vanish with the insert
// suppressed.
const NO_GCSE_INSERT_LOAD_OVERLAY_SOURCES = new Set([
  "exact/0807a550.c",
  "exact/resource_37a_c_02000d9c.c",
]);
// A store has no value for a later insn to consume, so it reaches the block end
// over a zero-cost ordering edge and takes the block's minimum priority, sinking
// behind every arithmetic insn that still has a chain. The mode saturates a
// store's effective priority so stores rank alike and above non-stores, leaving
// store-versus-store to the existing rules. Including loads in the predicate
// raises collateral from 308 to 498 sources with no further gain.
const SCHED_STORE_FIRST_OVERLAY_SOURCES = new Set([
  "exact/resource_373_c_020032b0.c",
  "exact/resource_3bd_c_02000a54.c",
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
  // resource_3ae:051c uses 0x8a5 at two call sites; without this the fork keeps
  // it in sl across the body and pays two extra saves.  2026-08-07.
  "semantic/resource_3ae_c_0200051c.c",
  "exact/resource_3ae_c_0200051c.c",
  // resource_394:07e0, paired with the sched-low-dest-first argument-
  // feeding-store exclusion (see that set); 2026-08-06.
  "exact/resource_394_c_020007e0.c",
  "semantic/resource_394_c_020007e0.c",
  // Reconstruction-wave (Sonnet) exact, 2026-08-06: resource_3b1:0728 stops
  // the repeated 0x925 flag-id constant from CSE'ing into a persistent
  // register across an intervening call (paired with sched-low-dest-first).
  "exact/resource_3b1_c_02000728.c",
  "semantic/resource_3b1_c_02000728.c",
  // Reconstruction-wave exact, 2026-08-05: resource_3b4:0ad0 rematerialises
  // its 0x9c8 pool constant (paired with cse-shift-immediate-off and
  // sched-low-dest-first at those sets).
  "exact/resource_3b4_c_02000ad0.c",
  "semantic/resource_3b4_c_02000ad0.c",
  // resource_3c8:16a4 reloads its recurring 0x201 pool word at each of its
  // two use sites instead of caching it in a callee-saved register across
  // the intervening calls; this flag takes it from a 4-byte span overflow
  // (probed exact-shape-restoring 2026-08-05).
  "exact/resource_3c8_c_020016a4.c",
  "semantic/resource_3c8_c_020016a4.c",
  // Lane-C triple-route twins, 2026-08-05: sched-low-dest-first fixes their
  // 8-byte movs placement but exposes two r5-parking regressions, each
  // suppressed by a different CSE gate (pool word 0x867 needs this flag;
  // 408=204<<1 needs -fno-cse-two-insn-immediate). First evidenced triples.
  "exact/resource_382_c_02001238.c",
  "semantic/resource_382_c_02001238.c",
  "exact/resource_385_c_020009f8.c",
  "semantic/resource_385_c_020009f8.c",
  // Its nine literal-pool islands likewise reload recurring words locally;
  // sharing them changes both the saved-register set and pool boundaries.
  "exact/resource_38f_c_020008ec.c",
  "semantic/resource_38f_c_020008ec.c",
  // resource_3aa:1494 reloads its recurring 0x2009/0x2002/0xcccc pool words
  // at each call site; sharing them adds r7/r8 to the prologue and moves the
  // pool islands.
  "exact/resource_3aa_c_02001494.c",
  "semantic/resource_3aa_c_02001494.c",
  // This event call sheet reloads recurring addresses at their call sites;
  // sharing them shifts all eight inline literal pools and grows the prologue.
  "exact/resource_3b8_c_02002014.c",
  // resource_3bd:2c44 likewise reloads recurring pool words at their call
  // sites; sharing them changes the saved-register set and pool placement.
  "exact/resource_3bd_c_02002c44.c",
  // resource_379:0074 reloads its recurring pool words (0xe666 argument and
  // script addresses) at each call site; sharing hoists one into r5.
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
  // resource_3b9:055c reloads the flag-id pool word 0x3c1 independently at its
  // test and set call sites; sharing it in r5 adds a push/pop the reference
  // does not have (tell: reference pushes {lr}, candidate pushes {r5, lr}).
  "exact/resource_3b9_c_0200055c.c",
  // resource_3b9:2668 reloads the shared pool address 0x0200adac three times
  // across the id-0/1/2 calls; the reference reloads it at each site.
  "exact/resource_3b9_c_02002668.c",
  // resource_3b9:2964 is the same call-sheet shape as resource_3b9:2668.
  "exact/resource_3b9_c_02002964.c",
]);
const NO_STRICT_ALIASING_OVERLAY_SOURCES = new Set([
  "exact/resource_380_c_02000104.c",
  "exact/resource_39c_c_02000104.c",
  "exact/resource_39e_c_02000104.c",
  // Lane-B exact, 2026-08-05: the u8** owner load may-aliases the u16
  // counter store; strict aliasing lets sched2 sink it below the sext,
  // -fno-strict-aliasing pins it at the reference position.
  "exact/resource_39b_c_02001c20.c",
  "semantic/resource_39b_c_02001c20.c",
  "exact/resource_39c_c_020003a8.c",
  "exact/resource_3c9_c_02000104.c",
  "exact/resource_3c9_c_0200215c.c",
  "exact/resource_3c9_c_020021ac.c",
  "exact/resource_3c9_c_02003600.c",
  // resource_397:02a0 writes Data_02008614 (u16*) and Data_02008616 (u16*)
  // through two independent pointers the strict-aliasing analysis treats as
  // possibly overlapping with the u32 blendPhase local, hoisting the second
  // pool-address load; the reference schedules it before the second fieldB
  // reload. Mode cohort: exact under alias-strict-off alone.
  "exact/resource_397_c_020002a0.c",
]);
// Grouped-DMA control-word class (fork commit cee872a). Path-keyed because
// this mode's stem-keyed main-image counterpart (SCHED_POOL_LOAD_LATE_SOURCES)
// would collide across overlays sharing an offset; see that set's comment.
const SCHED_POOL_LOAD_LATE_OVERLAY_SOURCES = new Set([
  // resource_3b3:2384 needs the lsls-before-ldr staging at its 0xd0 call
  // after cse-rerun-loop-off drops the cross-block pool CSE from the push
  // list (reconstruction wave, 2026-08-05).
  "exact/resource_3b3_c_02002384.c",
  "semantic/resource_3b3_c_02002384.c",
]);
// resource_3c4:1aba's loop entry is jump-threaded away from the reference's
// source-order guard; disabling thread-jumps restores it (reconstruction
// wave, 2026-08-05, paired with cse-shift-immediate-off and
// sched-low-dest-first at those sets). No prior route used this stock switch.
// Companion to the pool-load-late class: a lone `movs rN, #K' argument setter
// also issues before a ready literal-pool load.  Probe set, 2026-08-07.
const SCHED_IMMEDIATE_BEFORE_POOL_OVERLAY_SOURCES = new Set<string>([]);
// A small HImode constant reaches its register through the literal pool because
// *thumb_movhi_insn's "mn" alternative precedes its "I" one -- which is what the
// reference does for most owners (see the standing note above the pattern in
// alchemy-gcc: dropping the n regresses 25 byte-exact functions).  A few owners
// want the plain `movs' instead; -fthumb-hi-immediate is the per-source opt-in.
const THUMB_HI_IMMEDIATE_OVERLAY_SOURCES = new Set([
  "exact/resource_377_c_020003f8.c",
  "semantic/resource_377_c_020003f8.c",
]);
// A call whose r1 argument is a constant-pool load and whose r0 argument is a
// plain immediate: the reference issues the pool load first.  Distinct from
// -fthumb-call-literal-arg1-first, which transposes a pair of immediates and is
// not self-consistent within an owner; this shape is, because the sibling sites
// that keep r0 first all pass a third argument register.
const CALL_POOL_ARG1_FIRST_OVERLAY_SOURCES = new Set([
  "exact/resource_3a2_c_02000924.c",
  "semantic/resource_3a2_c_02000924.c",
  // resource_372:3e48 — exact on this flag together with sched-low-dest-first
  // (see that set).  One site wants the pool load ahead of `movs r0,#8'; the
  // sched flag alone fixes the other site and breaks this one, 2026-08-07.
  "exact/resource_372_c_02003e48.c",
  "semantic/resource_372_c_02003e48.c",
]);
// The last plain immediate call argument goes ahead of a preceding split
// constant's shift -- the mirror of -fthumb-next-arg-between-split, which
// fills the same dependency slot from the other side.
const ARG_BEFORE_FINAL_SHIFT_OVERLAY_SOURCES = new Set([
  // resource_3b1:366c -- the `movs r0,#8' before `lsls r1,r1,#17' at
  // 0x020036c0.  The general low-destination scheduler tie-break also fixes
  // this site but rotates a four-instruction argument group at 0x020036a0,
  // so the narrow peephole is the right lever here, 2026-08-07.
  "exact/resource_3b1_c_0200366c.c",
  "semantic/resource_3b1_c_0200366c.c",
]);
// The inverse of -fthumb-call-pool-arg1-first: an immediate r0 argument goes
// back ahead of a scheduled r1 pool load.  The discriminator the references
// observe is where the third argument is written -- a sheet whose r2 setter
// still follows the r0 setter is written in ascending register order, while a
// sheet whose r2 was already computed before the pool load keeps the pool load
// first.  The pass encodes exactly that, so both shapes coexist in one row.
const CALL_ARG0_BEFORE_POOL_OVERLAY_SOURCES = new Set([
  // resource_371:1888, :1938 and :19e8 -- the `movs r0,#8' before the r1 pool
  // load at 0x020018de, whose sibling call at 0x020018bc in the same row keeps
  // the pool load first because r2 there is a split constant, 2026-08-07.
  "exact/resource_371_c_02001888.c",
  "semantic/resource_371_c_02001888.c",
  "exact/resource_371_c_02001938.c",
  "semantic/resource_371_c_02001938.c",
  "exact/resource_371_c_020019e8.c",
  "semantic/resource_371_c_020019e8.c",
]);
// The register-move twin of the flag above: a two-argument sheet whose r0 is a
// plain copy of a live register, written by the references before the r1 pool
// load.  Narrower than -fthumb-call-pool-arg1-first's inverse needs to be: the
// copy has to be the insn immediately before the call.
const CALL_ARGREG_BEFORE_POOL_OVERLAY_SOURCES = new Set([
  // resource_3a7:0b8c -- `adds r0,r5,#0' before `ldr r1,[pc,#48]' at
  // 0x02000bd0, 2026-08-07.
  "exact/resource_3a7_c_02000b8c.c",
  "semantic/resource_3a7_c_02000b8c.c",
]);
// Two adjacent independent in-place constant shifts, transposed so the one
// whose input was defined earlier goes first.  The post-reload scheduler's
// tie-break lands the other way on these sheets; the age rule is what keeps
// the sibling shift pairs in the same rows untouched.
const SWAP_ADJACENT_SHIFTS_OVERLAY_SOURCES = new Set([
  // resource_3bc:4494 -- `lsls r1,r1,#2' before `asrs r0,r0,#16' at
  // 0x02004538, and resource_3a4:02cc -- `lsls r3,r3,#14' before
  // `lsls r2,r2,#11' at 0x02000316, whose sibling pair at 0x02000354 keeps the
  // scheduler's order because there r3 is the younger value, 2026-08-07.
  "exact/resource_3bc_c_02004494.c",
  "semantic/resource_3bc_c_02004494.c",
  "exact/resource_3a4_c_020002cc.c",
  "semantic/resource_3a4_c_020002cc.c",
]);
// `orrs Rd, Rm' ties its result to one of its two inputs.  When both die at the
// insn the allocator keeps the one written last; the references keep the one
// written first, so a constant that has been sitting in a register since well
// above the load becomes the accumulator.
// Both stacked call arguments materialised before either store.
const STACK_ARGS_BEFORE_STORES_OVERLAY_SOURCES = new Set([
  // resource_382:0fb4, :1010 and :113c -- `movs r2, #12' hoisted above the
  // two `str' to sp at 0x02000fd4, plus resource_3b9:1c6c, 2026-08-07.
  "exact/resource_382_c_02000fb4.c",
  "semantic/resource_382_c_02000fb4.c",
  "exact/resource_382_c_02001010.c",
  "semantic/resource_382_c_02001010.c",
  "exact/resource_382_c_0200113c.c",
  "semantic/resource_382_c_0200113c.c",
  "exact/resource_3b9_c_02001c6c.c",
  "semantic/resource_3b9_c_02001c6c.c",
  // resource_382:1090 -- `movs r2, #16' hoisted above the two `str' to sp at
  // 0x020010fa, 2026-08-07.
  "exact/resource_382_c_02001090.c",
  "semantic/resource_382_c_02001090.c",
]);
// -fthumb-call-literal-arg1-first restricted to a sheet that opens right
// after a call, which is the discriminator the references observe in
// functions that otherwise write the pair in register order.
const LITERAL_ARG1_FIRST_AFTER_CALL_OVERLAY_SOURCES = new Set([
  // resource_3b9:06bc -- `movs r1, #0' before `movs r0, #8' at 0x020006d2,
  // 2026-08-07.
  "exact/resource_3b9_c_020006bc.c",
  "semantic/resource_3b9_c_020006bc.c",
  // resource_376:0258 and resource_376:0190, same shape, 2026-08-07.
  "exact/resource_376_c_02000258.c",
  "semantic/resource_376_c_02000258.c",
  "exact/resource_376_c_02000190.c",
  "semantic/resource_376_c_02000190.c",
  // resource_382:0614 -- `movs r1, #0' before `movs r0, #9' at 0x0200062a,
  // the call sheet that follows the call at 0x02000626, 2026-08-07.
  "exact/resource_382_c_02000614.c",
  "semantic/resource_382_c_02000614.c",
]);
// -fthumb-call-literal-arg1-first-chained gates the same transposition on what
// follows the consuming call rather than on what precedes the pair: the
// references transpose a two-literal sheet only where the call it feeds is
// itself followed by another argument setter, and keep register order when the
// call is followed by a jump or by an insn that writes something else.
const SMALL_SHIFT_BEFORE_IMMEDIATES_OVERLAY_SOURCES = new Set([
  // resource_39f:21b0 -- four argument sheets where a `lsls rN, #1' split
  // constant is written ahead of the sheet's plain `movs r0/r1' immediates.
  // 2026-08-07.
  "exact/resource_39f_c_020021b0.c",
  "semantic/resource_39f_c_020021b0.c",
]);

const BLOCKMOVE_DEST_BEFORE_SOURCE_OVERLAY_SOURCES = new Set([
  // resource_39f:0f94 -- the 24-byte by-value struct argument at 0x02000fa8,
  // whose `mov r3, sp' the post-reload scheduler pushed below the matching
  // `add r2, sp, #24'. 2026-08-07.
  "exact/resource_39f_c_02000f94.c",
  "semantic/resource_39f_c_02000f94.c",
]);

const LITERAL_ARG1_FIRST_CHAINED_OVERLAY_SOURCES = new Set([
  // resource_3ad:11b8 -- `movs r1, #0' before `movs r0, #1' at 0x020011dc,
  // where the identical literal pair at 0x0200128a keeps register order
  // because its call is followed by a branch. 2026-08-07.
  "exact/resource_3ad_c_020011b8.c",
  "semantic/resource_3ad_c_020011b8.c",
]);
// -fthumb-arg-before-final-shift for a shift that is not the sheet's last
// setup insn.
// -fthumb-pool-load-before-load and -fthumb-shift-before-store-in-split for
// the two independent transpositions in resource_371:02f0: the pool word is
// read before the field load at the entry, and the split constant's shift is
// finished before the byte store at 0x02000300.
// -fthumb-high-move-before-store for a `mov rN, r8' the scheduler sank below
// the byte store that ends the previous statement.
const HIGH_MOVE_BEFORE_STORE_OVERLAY_SOURCES = new Set([
  // resource_382:1090 -- `mov r2, r8' before `strb r3, [r6, #0]' at
  // 0x02001114, 2026-08-07.
  "exact/resource_382_c_02001090.c",
  "semantic/resource_382_c_02001090.c",
]);
const POOL_LOAD_BEFORE_LOAD_OVERLAY_SOURCES = new Set([
  "exact/resource_371_c_020002f0.c",
  "semantic/resource_371_c_020002f0.c",
]);
const SHIFT_BEFORE_STORE_IN_SPLIT_OVERLAY_SOURCES = new Set([
  "exact/resource_371_c_020002f0.c",
  "semantic/resource_371_c_020002f0.c",
]);
const ARG_BEFORE_SHIFT_IN_SHEET_OVERLAY_SOURCES = new Set([
  // resource_3b1:5ca4 -- `movs r0, #8' ahead of `lsls r1, r1, #8' at
  // 0x02005ce0, with `movs r2, #80' still between the pair and the call,
  // 2026-08-07.
  "exact/resource_3b1_c_02005ca4.c",
  "semantic/resource_3b1_c_02005ca4.c",
  // resource_3a5:1874 -- `movs r0, #8' ahead of `lsls r1, r1, #8' at
  // 0x0200187c, the second of two split constants in the same sheet,
  // 2026-08-07.
  "exact/resource_3a5_c_02001874.c",
  "semantic/resource_3a5_c_02001874.c",
  // resource_382:0614 -- `movs r0, #9' ahead of `lsls r1, r1, #7' at
  // 0x02000658, with `movs r2, #0' still between the pair and the call,
  // 2026-08-07.
  "exact/resource_382_c_02000614.c",
  "semantic/resource_382_c_02000614.c",
]);
// A register load stays below the accumulate and store it was hoisted over.
const SINK_LOAD_PAST_STORE_OVERLAY_SOURCES = new Set([
  // The thirteen copies of the :0104 integrator -- `ldr r1, [r0, #80]' below
  // the store at 0x0200012a rather than above it, 2026-08-07.
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
]);
// A literal r0 argument written between the two pool loads of a sheet.
const CALL_ARG0_BETWEEN_POOL_PAIR_OVERLAY_SOURCES = new Set([
  // resource_39b:0f48 -- `movs r0, #0' between the r2 and r1 pool loads at
  // 0x02000f56, 2026-08-07.
  "exact/resource_39b_c_02000f48.c",
  "semantic/resource_39b_c_02000f48.c",
]);
// The value written by a memory-mapped store is materialised before the split
// constant that addresses it.
const STORE_VALUE_BEFORE_BASE_OVERLAY_SOURCES = new Set([
  // resource_3ca:004c -- `movs r2, #0' ahead of `movs r3, #160' at 0x02000060,
  // the 0x05000000 halfword write, 2026-08-07.
  "exact/resource_3ca_c_0200004c.c",
  "semantic/resource_3ca_c_0200004c.c",
]);
// The transposed twin of -fthumb-swap-adjacent-shifts: two in-place constant
// shifts separated by one unrelated insn, issued newest-input first.
const SWAP_SHIFTS_ACROSS_INSN_OVERLAY_SOURCES = new Set([
  // resource_3c8:2f30 -- `lsls r2' before `lsls r0' across the `movs r1, #0'
  // at 0x02002f66, 2026-08-07.
  "exact/resource_3c8_c_02002f30.c",
  "semantic/resource_3c8_c_02002f30.c",
]);
const ORR_INTO_OLDER_INPUT_OVERLAY_SOURCES = new Set([
  // resource_3b3:1fd4 -- `orrs r5, r3' at 0x02002024, where r5 holds the 1 set
  // back at 0x02002014 and dies here, 2026-08-07.
  "exact/resource_3b3_c_02001fd4.c",
  "semantic/resource_3b3_c_02001fd4.c",
]);
// The two-pool-word twin of -fthumb-call-arg0-before-pool: an immediate r0
// argument put back ahead of the pair of pool loads that set r1 and r2.
const CALL_ARG0_BEFORE_POOL_PAIR_OVERLAY_SOURCES = new Set([
  // resource_371:1a98 -- `movs r0,#8' before `ldr r1' / `ldr r2' at
  // 0x02001aee, 2026-08-07.
  "exact/resource_371_c_02001a98.c",
  "semantic/resource_371_c_02001a98.c",
]);
// A pc-relative pool load that completes a call's argument list, sunk down to
// the call itself.  The post-reload scheduler hoists it to cover its latency;
// the references fetch the word last, after the other argument setters.
const SINK_POOL_LOAD_TO_USE_OVERLAY_SOURCES = new Set([
  // resource_3c6:0158 -- both `ldr r0, .L' argument loads at 0x02000190 and
  // 0x020001ac sit after the pair of narrowing shifts, 2026-08-07.
  "exact/resource_3c6_c_02000158.c",
  "semantic/resource_3c6_c_02000158.c",
]);
// A constant materialized before a call that the references materialize after
// it.  The fork has carried this mode since before the overlay work started but
// no router or sweep list ever named it, so it was invisible: found by sweeping
// every -fthumb-* in toplev.c directly against the overlay board, 2026-08-07.
const SINK_CONSTANT_PAST_CALL_OVERLAY_SOURCES = new Set([
  // resource_39e:26d8 -- byte-exact, 140 bytes, 2026-08-07.
  "exact/resource_39e_c_020026d8.c",
  "semantic/resource_39e_c_020026d8.c",
]);
const NO_THREAD_JUMPS_OVERLAY_SOURCES = new Set([
  "exact/resource_3c4_c_02001aba.c",
  "semantic/resource_3c4_c_02001aba.c",
]);
// Measured scope of this flag, so it is not mistaken for a global build
// setting: swept across all 175 overlay owners whose first divergence is
// register-allocation:push, it left 110 unchanged, improved 37, and made 26
// WORSE, reaching byte-exact on none of them. It is a per-row lever for the
// specific tell below, not evidence about how the original was compiled.
//
// A ten-owner sample of the same class showed 1 exact, 5 better and 0 worse,
// which looked like a global signal and is not one -- the regressions only
// appear at full scale. Route it per row, on a byte-exact result.
const NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES = new Set([
  // resource_3ad:0460, found by a 60-owner singles cohort over every drafted
  // owner still within 30 differing halfwords, 2026-08-07.
  "semantic/resource_3ad_c_02000460.c",
  "exact/resource_3ad_c_02000460.c",
  // Reconstruction-wave exacts, 2026-08-05: resource_3b3:2384's cross-block
  // pool CSE and resource_3b4:1308's loop-hoisted 0x220 both drop from the
  // push list under this flag.
  "exact/resource_3b3_c_02002384.c",
  "semantic/resource_3b3_c_02002384.c",
  "exact/resource_3b4_c_02001308.c",
  "semantic/resource_3b4_c_02001308.c",
  // resource_3ad:01b0 tests flag 0x202 and then sets it, so the pooled word is
  // read twice and the rerun caches it in r5 across the guarded block where the
  // reference reloads it into r0 at each site. Took the row from 14 groups to 0.
  "exact/resource_3ad_c_020001b0.c",
  // resource_38c:035c tests one event flag and then sets the same flag id, so
  // 768 is built twice; the rerun caches it in r5 across the guarded block and
  // turns the leaf's `push {lr}` into `push {r5, lr}`. Paired with the
  // low-destination tie-break for its two `(id, 192 << 8, n)` calls.
  "exact/resource_38c_c_0200035c.c",
  // resource_3a5:0498 is the same tell with a pair of ids: it tests 0x8b2 and
  // 0x8b3, then sets 0x8b3 and 0x8b2, so both constants are built twice and
  // the rerun caches one in r5 across the guarded block. Took the row from 62
  // differing bytes to 0.
  "exact/resource_3a5_c_02000498.c",
  "exact/resource_37a_c_02000054.c",
  "exact/resource_37a_c_02000108.c",
  "exact/resource_37a_c_02000150.c",
  "exact/resource_37a_c_020001ec.c",
  "exact/resource_37a_c_02001a58.c",
  "exact/resource_373_c_02000ba8.c",
  // Third and fourth members of this overlay's rerun-cse family: the rerun caches
  // twice- and thrice-used pool constants in callee-saved registers where the
  // reference rematerializes them per use.
  "exact/resource_373_c_02005950.c",
  "exact/resource_373_c_02005a40.c",
  "exact/resource_399_c_02000abc.c",
  "exact/resource_3b8_c_0200049c.c",
  // resource_3a4 status-window family: default flags CSE-hoist a thrice-used
  // pool constant into r5; the reference keeps first-pass lifetimes. Each
  // entry byte-exact under the flag alone (notes/resource_3a4-*.md).
  "exact/resource_3a4_c_020009ec.c",
  "exact/resource_3a4_c_02000a94.c",
  "exact/resource_3a4_c_02000b3c.c",
  "exact/resource_3a4_c_02000bd8.c",
  // resource_39c:1c9c and 1d3c both load the pool constant 0x256 at two sites,
  // one of them in the entry block, so the entry-hoisted-local lever cannot
  // reach them and a &Value_ spelling is CSEd exactly like the const_int. Both
  // are byte-exact under the flag alone.
  "exact/resource_39c_c_02001c9c.c",
  "exact/resource_39c_c_02001d3c.c",
  // resource_3ba:0540 shares its 0x301 argument between the entry-block call
  // and the else-branch call once cse reruns; the reference keeps both sites
  // independent. Byte-exact (752/752) under the flag alone
  // (notes/resource_3ba-0540.md).
  "exact/resource_3ba_c_02000540.c",
  // resource_3a7:03e0 is the same twice-used pool constant tell: the guard call
  // and the body call both take 0x9a9, and the rerun parks it in r5 across the
  // two conditional branches instead of reloading it. Byte-exact under the flag
  // alone. It has to be routed by path, not by stem: resource_3a4_c_020003e0.c
  // is already exact and shares the stem.
  "exact/resource_3a7_c_020003e0.c",
  // resource_3cd:00c0 needs this alongside -fno-gcse; see NO_GCSE_OVERLAY_SOURCES.
  "exact/resource_3cd_c_020000c0.c",
  // resource_3b4:1070 loads its 0x9c4 request id at the guard call and again
  // at the commit call; the rerun keeps it in r5 across both and costs the
  // prologue a register the reference does not push.
  "exact/resource_3b4_c_02001070.c",
  // resource_37f:056c loads its 0x302 and 0x303 ids at two sites each; the
  // rerun parks both in callee-saved registers and buys two prologue pushes.
  "exact/resource_37f_c_0200056c.c",
  "exact/resource_37f_c_020005ac.c",
  "exact/resource_37f_c_020005ec.c",
  "exact/resource_37f_c_02000634.c",
  "exact/resource_37f_c_0200067c.c",
  "exact/resource_37f_c_020006c4.c",
  "exact/resource_37f_c_0200070c.c",
  "exact/resource_37f_c_02000754.c",
  // resource_38d:01b4 loads its 0x302 flag id at the test and again at the set.
  "exact/resource_38d_c_020001b4.c",
  "exact/resource_38d_c_0200028c.c",
  // resource_3a7:0368 loads its 0x9a9 request id at the guard call and again
  // at the commit call, the same shape as :03e0 in this overlay.
  "exact/resource_3a7_c_02000368.c",
  // resource_3a7:04d0 loads its 0x9aa request id at the guard and again at
  // the commit, the third row in this overlay with that shape.
  "exact/resource_3a7_c_020004d0.c",
  "exact/resource_3a7_c_0200048c.c",
]);
// -fno-gcse routed by path rather than by stem, for overlay rows whose address
// is also an offset in another overlay that is already converted.
// Path-keyed twin of NO_EXPENSIVE_SOURCES. resource_3b2:12b4 sets a flag bit
// with `ldrb / movs #2 / orrs / strb`; the reference loads into r2 and builds
// the constant in r3, and this fork allocates them the other way round. Six
// source spellings reach nothing (mask-first, a store-address local, a loaded
// local, a compound `|=`, an index local, and every pairing of those), and no
// scheduler or CSE mode moves it -- only -fno-expensive-optimizations does.
// It MUST be keyed by path: the bare stem 020012b4 is already taken by
// resource_395's converted row, and the stem set would silently recompile that
// one too (§7's overlay-blindness trap).
const NO_EXPENSIVE_OVERLAY_SOURCES = new Set([
  // The scene predicate keeps its signed table value in r3 through the XOR;
  // disabling the pass is the only exposed single mode that preserves it.
  "exact/resource_370_c_02000384.c",
  "exact/resource_3b2_c_020012b4.c",
  // resource_398:0214 has the same `ldrb / movs #2 / orrs / strb` flag-set with
  // the same r2/r3 swap; cse-expensive-off is the only single that closes it.
  "exact/resource_398_c_02000214.c",
  "semantic/resource_398_c_02000214.c",
  // resource_398:0538 repeats the same flag-set idiom with the same swap;
  // with its pooled/shifted script-call pair spelled as function-top locals
  // (the resource_3aa pattern) this flag alone is byte-exact.
  "exact/resource_398_c_02000538.c",
  "semantic/resource_398_c_02000538.c",
  // resource_3a2:0924 carries the same `ldrb / movs #1 / orrs / strb' flag-set
  // with the same r2/r3 swap; four source spellings (compound |=, split load
  // local, reversed operand order, pointer local) reach nothing and this is the
  // only single that closes it.  2026-08-07.
  "exact/resource_3a2_c_02000924.c",
  "semantic/resource_3a2_c_02000924.c",
]);
const NO_GCSE_OVERLAY_SOURCES = new Set([
  // The long resource_38f call sheet retains the original local lifetimes
  // only without global common-subexpression elimination.
  "exact/resource_38f_c_020008ec.c",
  "semantic/resource_38f_c_020008ec.c",
  // resource_3cd:00c0 fills a 16-halfword stack list through a call and then
  // walks it. Both the call argument and the walk's induction base are the
  // frame address, and the reference materialises `mov rX, sp` twice -- once
  // for the argument, once in the loop preheader. The pair of passes merges
  // them into one callee-saved copy, which costs an extra `mov` and is the
  // row's only divergence; it needs -fno-rerun-cse-after-loop alongside, as
  // either flag alone leaves the merge in place. The result is insensitive to
  // how the buffer is spelled -- plain array, struct wrapper, or byte buffer
  // cast to u16 all land.
  "exact/resource_3cd_c_020000c0.c",
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
// Overlay-safe counterpart to DEFAULT_ABI_SOURCES.  Addresses are extensively
// reused between overlays, so new evidence belongs to the full source path.
const DEFAULT_ABI_OVERLAY_SOURCES = new Set([
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
  "080069a4",
  "08006a00", "08006ba8", "08006c24", "08006c68", "08006cdc", "08006d50", "08006dec",
  "08006e24", "08006f48", "08006f84", "08007028", "08007098", "0800711c", "080071a8", "08007220",
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
  const abiBase = DEFAULT_ABI_SOURCES.has(stem) || DEFAULT_ABI_OVERLAY_SOURCES.has(sourceKey(source))
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
    ...(THUMB_0807A664_SOURCES.has(stem)
      ? ["-fno-gcse", "-fno-force-mem", "-fthumb-0807a664-exact"]
      : []),
    ...(OPTIMIZE_OS_SOURCES.has(stem) ? ["-Os"] : []),
    ...(OPTIMIZE_O3_OVERLAY_SOURCES.has(sourceKey(source)) ? ["-O3"] : []),
    ...(UNSCHEDULED_SOURCES.has(stem) ? ["-fno-schedule-insns", "-fno-schedule-insns2"] : []),
    ...(UNSCHEDULED_OVERLAY_SOURCES.has(sourceKey(source)) ? ["-fno-schedule-insns2"] : []),
    ...(NO_CSE_FOLLOW_SOURCES.has(stem) ? ["-fno-cse-follow-jumps"] : []),
    ...(NO_CSE_TWO_INSN_IMMEDIATE_SOURCES.has(stem)
      ? ["-fno-cse-two-insn-immediate"]
      : []),
    ...(NO_RERUN_CSE_AFTER_LOOP_SOURCES.has(stem) ? ["-fno-rerun-cse-after-loop"] : []),
    ...(NO_RERUN_LOOP_OPT_SOURCES.has(stem) ? ["-fno-rerun-loop-opt"] : []),
    ...(NO_GCSE_SOURCES.has(stem) ? ["-fno-gcse"] : []),
    ...(NO_EXPENSIVE_SOURCES.has(stem) ? ["-fno-expensive-optimizations"] : []),
    ...(NO_STRENGTH_REDUCE_SOURCES.has(stem) ? ["-fno-strength-reduce"] : []),
    ...(SCHED_HIGH_DEST_FIRST_SOURCES.has(stem) ? ["-fsched-high-dest-first"] : []),
    ...(SCHED_LOW_DEST_FIRST_SOURCES.has(stem) ? ["-fsched-low-dest-first"] : []),
    ...(NO_CONTIGUOUS_IMMEDIATE_SOURCES.has(stem) ? ["-fno-thumb-contiguous-immediate"] : []),
    ...(NO_SCHED_DEPEND_COUNT_SOURCES.has(stem) ? ["-fno-sched-depend-count"] : []),
    ...(LATE_FRAME_ALLOCATION_SOURCES.has(stem) ? ["-fthumb-late-frame-allocation"] : []),
    ...(SPLIT_GROUP_BASE_SOURCES.has(stem) ? ["-fthumb-split-group-base"] : []),
    ...(HOIST_PARAMETER_SAVE_SOURCES.has(stem) ? ["-fthumb-hoist-parameter-save"] : []),
    ...(MINIPOOL_TAIL_FIRST_SOURCES.has(stem) ? ["-fthumb-minipool-tail-first"] : []),
    ...(ENTRY_SAVES_DESCENDING_SOURCES.has(stem) ? ["-fthumb-entry-saves-descending"] : []),
    ...(GROUP_CONTROL_LAST_SOURCES.has(stem) ? ["-fthumb-group-control-last"] : []),
    ...(GROUP_POOLED_CONTROL_LAST_SOURCES.has(stem)
      ? ["-fthumb-group-pooled-control-last"]
      : []),
    ...(HIGH_MOVE_BEFORE_ALU_SOURCES.has(stem)
      ? ["-fthumb-high-move-before-alu"]
      : []),
    ...(MOVE_BEFORE_IMMEDIATE_ALU_SOURCES.has(stem)
      ? ["-fno-schedule-insns2", "-fthumb-move-before-immediate-alu"]
      : []),
    ...(LOOP_INVARIANT_BLOCK_HEAD_SOURCES.has(stem)
      ? ["-floop-invariant-block-head"]
      : []),
    ...(GROUP_ZERO_ANY_REGISTER_SOURCES.has(stem)
      ? ["-fthumb-group-zero-any-register"]
      : []),
    ...(ARG0_AFTER_SPLIT_SOURCES.has(stem) ? ["-fthumb-arg0-after-split"] : []),
    ...(CALL_ARG0_POOL_LOAD_SOURCES.has(stem)
      ? ["-fthumb-call-arg0-pool-load"]
      : []),
    ...(RETURN_VALUE_BEFORE_STACK_ADJUST_SOURCES.has(stem)
      ? ["-fthumb-return-value-before-stack-adjust"]
      : []),
    ...(SINK_GROUP_POOL_LOADS_SOURCES.has(stem)
      ? ["-fthumb-sink-group-pool-loads"]
      : []),
    ...(SINK_STACK_ADJUST_SOURCES.has(stem) ? ["-fthumb-sink-stack-adjust"] : []),
    ...(SINK_DEPENDENT_LOAD_SOURCES.has(stem)
      ? ["-fthumb-sink-dependent-load"]
      : []),
    ...(COLLAPSE_DEAD_SCRATCH_SOURCES.has(stem)
      ? ["-fthumb-collapse-dead-scratch"]
      : []),
    ...(SINK_BLOCK_CONSTANT_SOURCES.has(stem)
      ? ["-fthumb-sink-block-constant"]
      : []),
    ...(SINK_PAST_POOL_LOAD_SOURCES.has(stem)
      ? ["-fthumb-sink-past-pool-load"]
      : []),
    ...(GROUP_VALUE1_BEFORE_BASE_SOURCES.has(stem)
      ? ["-fthumb-group-value1-before-base"]
      : []),
    ...(GROUP_CONTROL_LAST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-group-control-last"]
      : []),
    ...(MOVE_BEFORE_ALU_SOURCES.has(stem) ? ["-fthumb-move-before-alu"] : []),
    ...(NO_REGMOVE_SOURCES.has(stem) ? ["-fno-regmove"] : []),
    ...(ENTRY_LITERAL_FIRST_SOURCES.has(stem)
      ? ["-fno-schedule-insns2", "-mthumb-entry-literal-first"] : []),
    ...(HIGH_REGISTER_MOVE_FIRST_SOURCES.has(stem) ? ["-mhigh-register-move-first"] : []),
    ...(ORR_DEAD_INPUT_REUSE_SOURCES.has(stem)
      ? ["-fthumb-orr-dead-input-reuse"]
      : []),
    ...(ORR_DEAD_INPUT_REUSE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-orr-dead-input-reuse"]
      : []),
    ...(CALL_LITERAL_ARG1_FIRST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-call-literal-arg1-first"]
      : []),
    ...(CALL_ARG1_BEFORE_ARG0_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-call-arg1-before-arg0"]
      : []),
    ...(CALL_ARG0_REG_SOURCE_SOURCES.has(stem)
      ? ["-fthumb-call-arg1-before-arg0", "-fthumb-call-arg0-reg-source"]
      : []),
    ...(SINK_CONSTANT_PAST_CALL_SOURCES.has(stem)
      ? ["-fthumb-sink-constant-past-call"]
      : []),
    ...(MOVE_BEFORE_UNARY_ALU_SOURCES.has(stem)
      ? ["-fthumb-move-before-unary-alu"]
      : []),
    // -O1 is appended after the baseline -O2 and wins as the later option,
    // matching exactly how the mode sweep scored this configuration.
    ...(OPTIMIZE_O1_OVERLAY_SOURCES.has(sourceKey(source)) ? ["-O1"] : []),
    ...(FIXED_LR_OVERLAY_SOURCES.has(sourceKey(source)) ? ["-ffixed-r14"] : []),
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
    ...(NO_CONSTANT_REUSE_SOURCES.has(stem)
      ? ["-fthumb-no-constant-reuse"]
      : []),
    ...(HOIST_ADD_IMMEDIATE_SOURCES.has(stem)
      ? [
          "-mgrouped-dma-store",
          "-fthumb-group-control-rematerialize",
          "-mlow-reg-order=2013",
          "-fthumb-sink-block-constant",
          "-fthumb-hoist-add-immediate",
        ]
      : []),
    ...(SINK_ADD_IMMEDIATE_SOURCES.has(stem)
      ? [
          "-mgrouped-dma-store",
          "-fthumb-sink-group-pool-loads",
          "-mthumb-load-latency-one",
          "-mearly-frame-allocation",
          "-fthumb-move-before-immediate-alu",
          "-fthumb-sink-block-constant",
          "-fthumb-sink-constant-past-memory",
          "-fthumb-earliest-frame-allocation",
          "-fthumb-copy-before-add-immediate",
          "-fno-schedule-insns2",
          "-fthumb-sink-add-immediate",
        ]
      : []),
    ...(SINK_STORE_PAST_STORE_SOURCES.has(stem)
      ? [
          "-mgrouped-dma-store",
          "-fthumb-sink-group-pool-loads",
          "-mthumb-load-latency-one",
          "-mearly-frame-allocation",
          "-fthumb-move-before-immediate-alu",
          "-fthumb-sink-block-constant",
          "-fthumb-sink-constant-past-memory",
          "-fthumb-sink-store-past-store",
          "-fthumb-pool-load-base-first",
        ]
      : []),
    ...(GROUP_VALUE2_IN_PLACE_SOURCES.has(stem) ? ["-fthumb-group-value2-in-place"] : []),
    ...(THUMB_IMMEDIATE_LATENCY_SOURCES.has(stem)
      ? ["-mthumb-immediate-latency"]
      : []),
    ...(CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-mcall-arg0-move-first"]
      : []),
    ...(CALL_ARG0_BEFORE_STORE_SOURCES.has(stem)
      ? ["-fno-sched-alias", "-fsched-store-first", "-fthumb-call-arg0-before-store"]
      : []),
    ...(POSTCALL_BYTE_INCREMENT_R2_SOURCES.has(stem)
      ? ["-fthumb-postcall-byte-increment-r2"]
      : []),
    ...(GROUP_CONTROL_REMATERIALIZE_SOURCES.has(stem)
      ? ["-fthumb-group-control-rematerialize"]
      : []),
    ...(SCHED_POOL_LOAD_LATE_SOURCES.has(stem)
      ? ["-fthumb-sched-pool-load-late"]
      : []),
    ...(SCHED2_OFF_THUMB_SOURCES.has(stem) ? ["-fno-schedule-insns2"] : []),
    ...(THUMB_LEAF_NO_LR_SOURCES.has(stem) ? ["-fthumb-leaf-no-lr"] : []),
    ...(THUMB_NO_IF_CONVERT_SOURCES.has(stem) ? ["-fthumb-no-if-convert"] : []),
    ...(THUMB_LOW_REG_ORDER_SOURCES.has(stem)
      ? [`-mlow-reg-order=${THUMB_LOW_REG_ORDER_SOURCES.get(stem)}`]
      : []),
    ...(THUMB_CALLEE_REG_ORDER_SOURCES.has(stem)
      ? [`-mcallee-reg-order=${THUMB_CALLEE_REG_ORDER_SOURCES.get(stem)}`]
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
    ...(THUMB_LOAD_LATENCY_ONE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-mthumb-load-latency-one"]
      : []),
    ...(sourceKey(source) === "exact/resource_379_c_02000074.c"
      ? []
      : []),
    ...(NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-rerun-cse-after-loop"]
      : []),
    ...(SCHED_POOL_LOAD_LATE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-sched-pool-load-late"]
      : []),
    ...(SCHED_IMMEDIATE_BEFORE_POOL_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-sched-immediate-before-pool"]
      : []),
    ...(THUMB_HI_IMMEDIATE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-hi-immediate"]
      : []),
    ...(CALL_POOL_ARG1_FIRST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-call-pool-arg1-first"]
      : []),
    ...(ARG_BEFORE_FINAL_SHIFT_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-arg-before-final-shift"]
      : []),
    ...(CALL_ARG0_BEFORE_POOL_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-call-arg0-before-pool"]
      : []),
    ...(CALL_ARGREG_BEFORE_POOL_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-call-argreg-before-pool"]
      : []),
    ...(SWAP_ADJACENT_SHIFTS_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-swap-adjacent-shifts"]
      : []),
    ...(STACK_ARGS_BEFORE_STORES_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-stack-args-before-stores"]
      : []),
    ...(LITERAL_ARG1_FIRST_AFTER_CALL_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-call-literal-arg1-first-after-call"]
      : []),
    ...(SMALL_SHIFT_BEFORE_IMMEDIATES_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-small-shift-before-immediates"]
      : []),
    ...(BLOCKMOVE_DEST_BEFORE_SOURCE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-blockmove-dest-before-source"]
      : []),
    ...(LITERAL_ARG1_FIRST_CHAINED_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-call-literal-arg1-first-chained"]
      : []),
    ...(HIGH_MOVE_BEFORE_STORE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-high-move-before-store"]
      : []),
    ...(POOL_LOAD_BEFORE_LOAD_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-pool-load-before-load"]
      : []),
    ...(SHIFT_BEFORE_STORE_IN_SPLIT_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-shift-before-store-in-split"]
      : []),
    ...(ARG_BEFORE_SHIFT_IN_SHEET_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-arg-before-shift-in-sheet"]
      : []),
    ...(SINK_LOAD_PAST_STORE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-sink-load-past-store"]
      : []),
    ...(CALL_ARG0_BETWEEN_POOL_PAIR_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-call-arg0-between-pool-pair"]
      : []),
    ...(STORE_VALUE_BEFORE_BASE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-store-value-before-base"]
      : []),
    ...(SWAP_SHIFTS_ACROSS_INSN_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-swap-shifts-across-insn"]
      : []),
    ...(ORR_INTO_OLDER_INPUT_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-orr-into-older-input"]
      : []),
    ...(CALL_ARG0_BEFORE_POOL_PAIR_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-call-arg0-before-pool-pair"]
      : []),
    ...(SINK_POOL_LOAD_TO_USE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-sink-pool-load-to-use"]
      : []),
    ...(SINK_CONSTANT_PAST_CALL_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fthumb-sink-constant-past-call"]
      : []),
    ...(NO_THREAD_JUMPS_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-thread-jumps"]
      : []),
    ...(NO_GCSE_OVERLAY_SOURCES.has(sourceKey(source)) ? ["-fno-gcse"] : []),
    ...(NO_EXPENSIVE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-expensive-optimizations"]
      : []),
    ...(NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-cse-follow-jumps", "-fno-cse-skip-blocks"]
      : []),
    ...(NO_CSE_SKIP_BLOCKS_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-cse-skip-blocks"]
      : []),
    ...(NO_STRICT_ALIASING_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-strict-aliasing"]
      : []),
    ...(NO_CSE_TWO_INSN_IMMEDIATE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-cse-two-insn-immediate"]
      : []),
    ...(NO_CSE_SHIFT_IMMEDIATE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-cse-shift-immediate"]
      : []),
    ...(NO_CSE_POOL_IMMEDIATE_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fno-cse-pool-immediate"]
      : []),
    ...(SCHED_LOW_DEST_FIRST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fsched-low-dest-first"]
      : []),
    ...(SCHED_HIGH_DEST_FIRST_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fsched-high-dest-first"]
      : []),
    ...(SCHED_CALL_DEST_DESCENDING_OVERLAY_SOURCES.has(sourceKey(source))
      ? ["-fsched-call-dest-descending"]
      : []),
    ...(FIXED_R7_OVERLAY_SOURCES.has(sourceKey(source)) ? ["-ffixed-r7"] : []),
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
// Passing `compiler` restricts the walk to sources that family actually
// compiles, so the flag-capability lint can probe each binary with exactly
// the flags routing can hand it.
export function evidencedRoutingFlags(compiler?: "gcc296" | "agbcc"): string[] {
  const baseline = new Set([...CFLAGS, ...GS2_CFLAGS, ...AGBCC_CFLAGS]);
  const found = new Set<string>();
  const inspect = (source: string): void => {
    if (compiler !== undefined && usesAgbccCompiler("gs1", source) !== (compiler === "agbcc")) return;
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
    ...THUMB_LOAD_LATENCY_ONE_OVERLAY_SOURCES,
    ...NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES,
    ...SCHED_POOL_LOAD_LATE_OVERLAY_SOURCES,
    ...SCHED_IMMEDIATE_BEFORE_POOL_OVERLAY_SOURCES,
    ...THUMB_HI_IMMEDIATE_OVERLAY_SOURCES,
    ...CALL_POOL_ARG1_FIRST_OVERLAY_SOURCES,
    ...ARG_BEFORE_FINAL_SHIFT_OVERLAY_SOURCES,
    ...CALL_ARG0_BEFORE_POOL_OVERLAY_SOURCES,
    ...CALL_ARGREG_BEFORE_POOL_OVERLAY_SOURCES,
    ...SWAP_ADJACENT_SHIFTS_OVERLAY_SOURCES,
    ...STACK_ARGS_BEFORE_STORES_OVERLAY_SOURCES,
    ...LITERAL_ARG1_FIRST_AFTER_CALL_OVERLAY_SOURCES,
    ...LITERAL_ARG1_FIRST_CHAINED_OVERLAY_SOURCES,
    ...SMALL_SHIFT_BEFORE_IMMEDIATES_OVERLAY_SOURCES,
    ...BLOCKMOVE_DEST_BEFORE_SOURCE_OVERLAY_SOURCES,
    ...ARG_BEFORE_SHIFT_IN_SHEET_OVERLAY_SOURCES,
    ...HIGH_MOVE_BEFORE_STORE_OVERLAY_SOURCES,
    ...POOL_LOAD_BEFORE_LOAD_OVERLAY_SOURCES,
    ...SHIFT_BEFORE_STORE_IN_SPLIT_OVERLAY_SOURCES,
    ...SINK_LOAD_PAST_STORE_OVERLAY_SOURCES,
    ...CALL_ARG0_BETWEEN_POOL_PAIR_OVERLAY_SOURCES,
    ...STORE_VALUE_BEFORE_BASE_OVERLAY_SOURCES,
    ...SWAP_SHIFTS_ACROSS_INSN_OVERLAY_SOURCES,
    ...ORR_INTO_OLDER_INPUT_OVERLAY_SOURCES,
    ...CALL_ARG0_BEFORE_POOL_PAIR_OVERLAY_SOURCES,
    ...SINK_POOL_LOAD_TO_USE_OVERLAY_SOURCES,
    ...SINK_CONSTANT_PAST_CALL_OVERLAY_SOURCES,
    ...NO_THREAD_JUMPS_OVERLAY_SOURCES,
    ...NO_GCSE_OVERLAY_SOURCES,
    ...NO_EXPENSIVE_OVERLAY_SOURCES,
    ...NO_INTERWORK_OVERLAY_SOURCES,
    ...NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES,
    ...NO_CSE_SKIP_BLOCKS_OVERLAY_SOURCES,
    ...NO_STRICT_ALIASING_OVERLAY_SOURCES,
    ...NO_CSE_TWO_INSN_IMMEDIATE_OVERLAY_SOURCES,
    ...NO_CSE_SHIFT_IMMEDIATE_OVERLAY_SOURCES,
    ...NO_CSE_POOL_IMMEDIATE_OVERLAY_SOURCES,
    ...SCHED_LOW_DEST_FIRST_OVERLAY_SOURCES,
    ...SCHED_HIGH_DEST_FIRST_OVERLAY_SOURCES,
    ...FIXED_R7_OVERLAY_SOURCES,
    ...NO_SCHED_ALIAS_OVERLAY_SOURCES,
    ...NO_GCSE_INSERT_LOAD_OVERLAY_SOURCES,
    ...SCHED_STORE_FIRST_OVERLAY_SOURCES,
    ...GROUPED_DMA_STORE_OVERLAY_SOURCES,
    ...UNSCHEDULED_OVERLAY_SOURCES,
    ...DEFAULT_ABI_OVERLAY_SOURCES,
    ...EARLY_LITERAL_POOL_OVERLAY_PATHS,
    ...CALL_ARG1_BEFORE_ARG0_OVERLAY_SOURCES,
    ...FIXED_LR_OVERLAY_SOURCES,
    ...OPTIMIZE_O1_OVERLAY_SOURCES,
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
  "exact/resource_373_c_02005610.c": 0x0200b794,
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
type HostKey = "darwin-arm64" | "darwin-x64" | "linux-x64" | "linux-arm64";

// Every darwin/linux x arm64/x86_64 combination is a first-class host. A host
// whose digest set is still empty is not "unsupported": it is admissible the
// moment someone builds the committed fork source on it, reproduces the ROM
// byte-identically under `bun run verify`, and pins the resulting digests.
function hostKey(): HostKey | null {
  if (process.platform === "darwin" && process.arch === "arm64") return "darwin-arm64";
  if (process.platform === "darwin" && process.arch === "x64") return "darwin-x64";
  if (process.platform === "linux" && process.arch === "x64") return "linux-x64";
  if (process.platform === "linux" && process.arch === "arm64") return "linux-arm64";
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
      xgcc: [
        "87e09e3f1e2fd711e952d6831c73099b14a059a6ca594b16c11b9a83394483ed",
        // Full stage.sh gcc296 restage of fork commit 6461a0c (host parity
        // with the linux -fno-cse-shift-immediate build) -- admitted from a
        // green `bun run verify` on this host, 2026-08-05.
        "df0413f0051c07c654a753764235f39891d6f08a95d603a50f3cca9c645fc4e3",
        // -fthumb-group-zero-any-register (2026-08-06): widens the existing
        // stack-zero-before-base repair so it reads the saved-result and zero
        // registers off the insns instead of requiring r5/r6. Default-off and
        // source-routed, so unrouted codegen is unchanged. Cross-host rule:
        // rebuild+pin linux from the same fork source.
        "f92badaf03dbcfc4a79f0c4da9ee5159186f48dd8328403cbf05734a1aa21f42",
      ],
      cpp: [
        "f72b13ad2368419f2cc8c24966e030a57638bfce3f97868043196dac41e13575",
        "c6e5093aa3cda856c10b8fdff5a7f645a6ca63c92d2aea46688f8da4f5357915",
        "96ef7e4d9e3932817c023712850e3a15f0eb5b33904215c63c4eda4c17b43b1a",
      ],
      tradcpp: [
        "822c5cf4b38ea231f6eeeadcdf3a457518a25202c8a0a04aadf0942154e5436b",
        "553a34add496b8a063707e32376824ba11cf0153b4b6283309c9a2518a866281",
        "ebc87e2f3bf595bd2014ee9f8a67d07a27cb83b4ba50e3b2ca62b1f91999e5d4",
      ],
      cc1: [
      "4a19cd86373f0c95f8d5e2f1d6b85867e4afcb2a166e39e1fecc9f7777533906",
        // cc1 built on darwin-arm64 from fork origin/main 52bbd1c, 2026-08-07,
        // admitted from this green verify. Compiler binaries are not
        // reproducible here, so each host build gets its own pin.
        "7e66357ce5b69114713705da032c339a6aed02a1f7f90dd8ee330ae112313bed",
        // cc1 built on darwin-arm64 from fork origin/main ed39725, 2026-08-07,
        // the unified fork line: venus's modes reconciled per function with the
        // pre-existing ones. Admitted from the green verify of the venus merge.
        // Compiler binaries are not reproducible on this host, so each host
        // build gets its own pin. Cross-host rule: rebuild+pin linux from this
        // same fork source.
        "40ede14c48e7383c7b284450cd3c1357eb2d5dc014bf4949b69a2a8bc682b241",
        // cc1 built on darwin-arm64 from fork a853888, 2026-08-07: mercury's
        // own merge of origin/main (ed39725) with their -fthumb-no-constant-reuse
        // branch (a0a47e0), so this binary carries every mode all three lines
        // discovered. Admitted from the green verify of the mercury merge, which
        // is the first build that can route -fthumb-no-constant-reuse at all.
        // Cross-host rule: rebuild+pin linux from this same fork source.
        "c69829903ed05d68d46bc134de6cc554729fd59e1abca11715828db2a9a38365",
      "45d291c1ee530c2dc6ca5928e3186e4fc55234805a8b4b79c4b7d7977f7188cb",
        // cc1 built on darwin-arm64 from reconciled fork origin/main a853888
        // (mercury's -fthumb-no-constant-reuse merged with 52bbd1c and
        // ed39725), 2026-08-07.
        "6cee4484a765a41b8731e4983e7ae0751d3a21e516d63849f37c82313045f290",
        // -fthumb-no-canonicalize-comparison (2026-08-07): suppresses the ARM
        // back end's CANONICALIZE_COMPARISON rewrite in Thumb, where its
        // const_ok_for_arm gate says nothing about what Thumb can build.
        // Default-off and source-routed, so unrouted codegen is unchanged.
        // Cross-host rule: rebuild+pin linux from the same fork source.
        "802e08a756f582f6c10d467e747143486daa46d13f53c879c9cbd572a693d897",
        // -floop-invariant-block-head (2026-08-07): anchors the insns
        // move_movables hoists at the head of the preheader block instead of
        // immediately before the loop note, so hoisted invariants lead the
        // preheader's own insns. Default-off and source-routed, so unrouted
        // codegen is unchanged. Witness 080b5d3c. Cross-host rule:
        // rebuild+pin linux from the same fork source.
        "31441a4d40d157050c7917c53a2567f1d3f9c93d6b2358d7168ba5554fe6ebaf",
        // -fthumb-move-before-immediate-alu (2026-08-07): widens
        // -fthumb-move-before-alu to ALU insns with an immediate second input
        // and to loads as well as copies. Default-off and source-routed, so
        // unrouted codegen is unchanged. Witness 0801fd34. Cross-host rule:
        // rebuild+pin linux from the same fork source.
        "41b5d62baf165b5119d3bfa569be831d0ebeb1ed1187857c51ced44394549e75",
        // -fthumb-group-pooled-control-last (2026-08-07): the pooled-control
        // twin of -fthumb-group-control-last, for grouped transfers whose
        // control word arrives as a constant-pool load rather than a movs.
        // Default-off and source-routed, so unrouted codegen is unchanged.
        // Witness 0801a4fc. Cross-host rule: rebuild+pin linux from the same
        // fork source.
        "47fdf35d6c41ed4b1879ab9ce4ce019d395350b411953bc619e316abad225955",
        "c74a9073698099d112e341db60e4e2ca85c0c189048c4fc5d1277d8d8f58923d",
        // cc1 built from fork 76a2647 on darwin-arm64, 2026-08-07. Admitted
        // from the green verify recorded with this commit.
        "610bedba4d9b133d0ff37fbd37c43e7ad1c0b066e6325a4677d9fd80d75f965e",
        // -mcallee-reg-order= (r4-r7 allocation order), built on darwin-arm64
        // 2026-08-07. Default-off: without the switch reg_alloc_order is
        // untouched. Cross-host rule: rebuild+pin linux from the same source.
        "5f95a10d93349d67bad586c52362d0f6ede3e63c02c7bee87cc07203b228613d",
      "93419a9f5dca85c386efc04dcb26368b058191b73cc3ba808ef497fd458036dc",
      "fd1bd7c1c8cdbdb224569221c12744386bb0a75c86e1b22ff1e42aa392a9de07",
      "adc81071ec570ed11eae60e138bac9db3ff11ab63bef7f1a725bd198a4c70575",
      "3c8e99bb06fe15eaae8f04c83eec291a3b362d56649d79dec57a4884b064d7cf",
      "fa87afe4493a462fc2dbb11d0967b189ed040eeeb2b860422d3da5e38ab5d69c",
      "22a827bbcd8295b84148aa2e12270ead8d92fdd6af4582e2341afa21825014b7",
      "5a62f4a9686ac3492956d1d9e2e8da3039ff1d7d74961991d96ac366d7ccfba7",
    // -fthumb-stack-args-before-stores added, 2026-08-07.
    "0b7d6f6bd1490d49017a2438eb1bdbdd6c977b179c79f5f8a41d66299ce7ba53",
    // -fthumb-call-literal-arg1-first-after-call added, 2026-08-07.
    "63bab14236d935a3e74910921f576928afe6a2a8f20cbe1c239a637d3cf4e1a6",
    // -fthumb-call-literal-arg1-first-chained added, 2026-08-07.
    "42189f1a4abeacc3eb5d07d4fbbd646052dfa38c3079a5b053ae1bc1efcc1142",
    // -fthumb-small-shift-before-immediates added, 2026-08-07.
    "63292824103cbfd4405f2d8f1a9fe7780dec4b078e792cd37a7677d4a2c13ad4",
    // Fork line merged onto fork origin/main (Main's modes plus ours), 2026-08-07.
    "2e8cc5b36323f7c1e1f3b9e8e975c30de825e7173425b156e6aba6dd0ff40130",
    // -fthumb-blockmove-dest-before-source added, 2026-08-07.
    "3f6dc8780dff73c710237741b2f3fc90e20ce000238a72e99833418ae3109ee7",
    // -fthumb-arg-before-shift-in-sheet added, 2026-08-07.
    "afe85b0001a5c6abced6f76adf9a02991ae5da0957ad4360755ca2db082f92cf",
      // -fthumb-swap-shifts-across-insn added, 2026-08-07.
    "e1bc6d7d905057d59dff47512e52503cf4215422a1e7f283754a2d446f41afa1",
    "d7f9e8909c2a1fe73b6aa9a2ef06e6689e25e63e6ba72c8fe73996746c02bd75",
    "a8e8a5d7f7684661d897e98bcccfd163ae91220c38a32d90fc104113c8951879",
    // -fthumb-orr-into-older-input added, 2026-08-07.
    "78884ab5d9682fdf2b9f0dc5b4d0060976a1a042c186fbcae848fcd2d0657dbe",
    "efb56c6c7c16ac99165388f121154f83d90ba8705a76379e686dd3fb1188d055",
    "42b0383dee924cc75538975717471faed6551d1c4908897845b0dc7d898a17b7",
    "ebcd3e838d972bbf3fdddfd06dd0ff3b7c17a34179ae131e01738dfb4dad7105",
    "ebcd3e838d972bbf3fdddfd06dd0ff3b7c17a34179ae131e01738dfb4dad7105",
    "bd5a3ad4c90eb537d970d5dae5b2df2e79255df65ee2e6e8850197c942bde35f",
    "8fb7a9c28981701f0210085cfa9d91a6c1e77768741f1dff4842b7e82ec5334c",
    "804608eb96163abf8fdd6bcf2365dfa6fbfbc160b8a60c41bd25c053a3c72139",
    "a818520e2026b0a532241ce411c3580af797bc4f6492c718463735292e72c2f9",
    "e8466545e362128ab94f384c1ab2b7eb6c7ac5475d2ff6f67fe4d044c7ed1e0c",
    "8463b1c436cd30705500715949c2734bd63165f8877245268cac4ff44c4716f4",
    "25d6216d27234fa58e19a10952422c12659133e3977c7efe5e7c896ca73d3e33",
    "529d0e9ed7793955d948ce20e73edd612f23eb90e48aaf55a4ee1d88ad6cced4",
    "70a49ce7075dc060f63e1984177620ad914613b1c08cedc9c2e9d1d98e24fcac",
    // -fthumb-sink-pool-load-to-use added, 2026-08-07.
      "c3447c0cc251c7d2c0185352d108f992d980890a02638ddbbfeba753a39a5a0b",
      "f6f8e9c1a2bd5bb0bfeb6d75a2847617e6f35bf132de9f070d731cff21d39945",
      "799c1cfb3aa700a8cc75572cb576d612d7d7ed700420ff48cf09c8ba536662e4",
      "1dcc9902c957c8504e3bbf2b43d067b8f87a7b25fe16f34ad3bfeb69357b05fa",
      "b9d32c281a4a74b092aa78568e6c6c6700a2a4aa7012670f62d1fdc8f48a1c54",
      "ea45be7c7bbcf917946bad0f8f7130b77b89e0d0828292039288c3cccdc85d24",
      "39618b85563aae1ee776522c14d6de42eb3dceadbf9c7c76cbc501b3533457a1",
        "fe41ef5881fd46a4ec84ceb4224c5ea00abb8b6cb431b726a174ac99580085c6",
        // -fthumb-call-arg0-before-pool: undo a scheduler inversion that
        // hoisted an r1 pool load above an immediate r0 argument on a call
        // that also passes r2 -- the inverse of -fthumb-call-pool-arg1-first,
        // whose gate is the two-argument sheet. Witnesses resource_371:1888,
        // :1938 and :19e8; admitted 2026-08-07. Cross-host rule: rebuild+pin
        // linux from the same commit before the next cloud session touches
        // these routes.
        "ae5cc8a44b848b4dcdab7c3a1b5aab61eefd13af1428107828fc82294423004b",
        // -fthumb-arg-before-final-shift: emit the last plain immediate call
        // argument ahead of a preceding split constant's shift, the mirror of
        // -fthumb-next-arg-between-split. Witness resource_3b1:366c; admitted
        // 2026-08-07. Cross-host rule: rebuild+pin linux from the same commit
        // before the next cloud session touches these routes.
        "cf040aad9108e4595a2e0eea69d4dfc134ee93127213819efd48eb37d2e51859",
        // -fthumb-call-literal-arg1-first, gated on the two discriminators the
        // references actually observe: the call passes exactly r0 and r1 (a
        // third argument register means the reference writes the pair in
        // register order), and the two literals differ (an equal pair is also
        // written in register order). Admitted 2026-08-07. Cross-host rule:
        // rebuild+pin linux from the same commit before the next cloud
        // session touches these routes.
        "3db8ea91c88c50a170ac918c4b3475716f70f0d554bf91a5c7e982da7e6d807a",
        // -fthumb-call-literal-arg1-first: the existing
        // -fthumb-call-arg1-before-arg0 transform without its "only undo a
        // scheduler inversion" gate, restricted to a pair of plain literals.
        // Witness resource_3ae:02dc; admitted 2026-08-07. Cross-host rule:
        // rebuild+pin linux from the same commit before the next cloud
        // session touches these routes.
        "fdf336d0b046fbae55fb670ddd0852730b65cd7ebcd173827cd0e0cd9039b768",
        "df015cd830e04f26ce2ae1d3cc83205182f98cea1e41a29d586a79fb72d193a4",
        "792d4cd9b47acafaf93f6873f58b8701918db5a39af62852e3796037473387c4",
        "cce7c26cfda8ee1844256ac9226d0420d74c476fb24823c46bcce26db89a4983",
        "e654b8f55bef2f2a06efec89f171f46a76f0a55f671eb75e8b82ddc994f85b27",
        "0767fccd6046d0b4dcaae1150a82e505a29e59ca9f4f979e2535e7970f3de449",
        "d12bf2c7b96d2b1b6cec4c09b76f986249285070b1ca09d1ba1baf31b859cc18",
        "9ebef7d0fac03bbd44ce3016b8e06534cde5ef514b29042be9dcbf9414f248ff",
        "99b10b574bebe822798dd1c24eae495f08e08ec0af052a2fc8fa545ddfe67033",
        // -fno-cse-shift-immediate host parity build (fork commit 6461a0c),
        // matching the linux-x64 pin below; admitted 2026-08-05.
        "c8ab73932d0de44ea5cf337ddcb4b50cdf2b17696bbfd925224c5026dc6a7e8d",
        // -fthumb-group-control-rematerialize + -fthumb-sched-pool-load-late
        // (fork commit cee872a): grouped-DMA control-word class, witnesses
        // 080f377c and 08091174 byte-exact, 2,083-source regression zero
        // drift unrouted -- admitted from a green `bun run verify`,
        // 2026-08-05. Cross-host rule: rebuild+pin linux from the same
        // commit before the next cloud session touches these routes.
        "52a086c84a620d4cc8d9acb7d53ecd5826892065edd7f289447ff44f77162d74",
        // Fork commit 25b15cd: sched-low-dest-first argument-feeding-store
        // exclusion (refines the existing flag) plus the unrouted
        // -fthumb-group-base-in-r3 mode. Witness resource_394:07e0
        // byte-exact; 5811-source regression zero drift unrouted, run
        // twice clean after ruling out pristine-compiler nondeterminism.
        // Admitted from a green `bun run verify`, 2026-08-06.
        "e68ef21ee84393f9ca196f05731cd1688e811dec61015e164d9b72fcdab62ca7",
        // -fthumb-group-zero-any-register (2026-08-06): the stack-zero
        // grouped-DMA repair now reads its saved-result and zero registers
        // off the insns rather than requiring r5/r6, so objects with the
        // same source shape but a different allocation reach the reference
        // order. Default-off and source-routed. Cross-host rule: rebuild+pin
        // linux from the same fork source before a cloud session uses it.
        "45d3b62f28b91b005df4063493381bf77a28e03850463a68457e5409fd486bc2",
        // 2026-08-06: adds -fthumb-call-arg0-pool-load, which lets the existing
        // arg1-before-arg0 call reordering accept a pool-loaded address as the
        // r0 argument instead of only a plain integer constant.
        "d2ac7c989cce4c289950cc76cbf269f6057f4eb42b2a999dd9c4e756f866858d",
        // 2026-08-06: adds -fthumb-arg0-after-split, the inverse of
        // -fthumb-next-arg-between-split: it pushes an r0 pool load that the
        // scheduler parked inside a long split immediate down past the shift.
        "cee7a5014ceb6ff7f702dc0b12f5378a57f92a100f6e5da772f54930604f0284",
        "0a5442b5dcc96c3acb88597bb5074cedf6af996869708a0c840feb143f9d93a8",
        "943a15a0679086634f873ef7403ca4fde941da0845bf5be3bd2d3f2744228855",
        "943a15a0679086634f873ef7403ca4fde941da0845bf5be3bd2d3f2744228855",
        "541728170855e1f3002918fde83f91824e70f9e2d19cd50e93029529dae5b547",
        "735821ddefdabb338994007671c41b5ffd3a02653411fd1613e9fc8a5e7e722b",
        "f3f9b5276f4aab31ef2d3ebb85eb5a65e3cc4050900d403ef2622ed1d60c7b2b",
        // 2026-08-06: adds -fthumb-leaf-no-lr. THUMB_INITIAL_ELIMINATION_OFFSET
        // asks whether the function contains a far jump before branch lengths
        // are known, so every conditional branch reads as far and the answer is
        // latched permanently -- a Thumb leaf with any if/else then pushes and
        // pops a link register it never needed. With an empty frame and no
        // memory arguments the answer cannot move an elimination offset, so the
        // flag defers it to the prologue. Default-off and source-routed.
        // Cross-host rule: rebuild+pin linux from the same fork source.
        "e51f4b67d08661edf2ca533df45025ddb9dd31503f761f07b17b1a9b28289ac0",
        // 2026-08-06: adds -fthumb-no-if-convert, which disables the
        // if-conversion pass. It rewrites a two-armed if/else into "set the
        // fallthrough value, then conditionally overwrite it", dropping one
        // branch and hoisting the surviving constant above the compare; the
        // reference build keeps both arms. Default-off and source-routed.
        // Cross-host rule: rebuild+pin linux from the same fork source.
        "f4509bfbe10781093b5f16e84854ef7d91729e5972f2136e078b1c77959ab1c4",
        // Adds -mlow-reg-order=NNNN: overrides the leading four entries of
        // REG_ALLOC_ORDER with an explicit permutation of r0-r3, so the
        // register-permutation near-miss class can be swept instead of guessed.
        "1dc83047ac1e444c2c399ca54c91538ab562c1ff7ced02ec946812148f8bf43c",
        // Same, with toplev.o rebuilt so the option table actually carries the
        // new -mlow-reg-order= entry (the 1dc83047 build had a stale toplev.o).
        "3bae0b4cc3685eaa14c674c4b859f67652bd15751613c48c43dd1e688ea7d584",
        // -mlow-reg-order= now also accepts eight digits: the first four order
        // the entry block, the last four every other block.
        "43ecbc402bd3e6abff7ea434414c0ef9f3001836cc689a6f1bc06bc896b44881",
        // 2026-08-07: adds -mhigh-reg-order=NNNN, the high-register twin of
        // -mlow-reg-order=. It overrides the `8, 10, 9, 11' run inside
        // REG_ALLOC_ORDER with an explicit permutation of r8-r11. Two Thumb
        // owners (080a8c2c, 08021848) spill two values to high registers and
        // differ from the reference only in which of r8/sl each pseudo lands
        // in, with an otherwise identical instruction sequence.
        "1d6043ba5cfc56bc6bdbf2396637708e45c5896e671841178e60ba12a8336e0b",
        // 2026-08-07: adds -fthumb-late-frame-allocation, the mirror of
        // -mearly-frame-allocation. Where the early mode raises a Thumb stack
        // decrement's scheduling priority, this one lowers it, so the incoming
        // argument copies issue ahead of `sub sp, #N'. Reference objects open
        // both ways and only the early direction was expressible. An -f flag
        // rather than an -m one because target_flags has no bit left.
        "610bedba4d9b133d0ff37fbd37c43e7ad1c0b066e6325a4677d9fd80d75f965e",
        // Three modes (2026-08-07), all default-off and source-routed, so
        // unrouted codegen is unchanged. Witness 080a90bc.
        // -fthumb-call-arg0-reg-source widens the existing arg1-before-arg0
        // call reordering so the r0 argument may be a plain hard-register
        // copy, not just a constant or pool load; admitted only opposite an
        // immediate r1 setter, since that is the shape the reference inverts.
        // -fthumb-sink-constant-past-call moves a callee-saved register's
        // constant or pool-load setup from before a call to just after it,
        // iterated to fixpoint so a whole pre-call run relocates.
        // -fthumb-move-before-unary-alu issues a flag-preserving high-register
        // copy ahead of an adjacent independent unary ALU insn.
        // Cross-host rule: rebuild+pin linux from the same fork source.
        "f76bdc9dccde93acc1a2f382d760ec79292591c5b39cc0221368630755068ce8",
        // Fork commit 1e1502b: -fsched-call-dest-descending, the mirror
        // direction of the existing call-argument tie-break. Witness
        // resource_3c8:2f30; admitted from a green `bun run verify`,
        // 2026-08-06. Cross-host rule: rebuild+pin linux from the same
        // commit before the next cloud session touches these routes.
        "dfe6fd74ceeae8d33695b6ac06285cedab253e06866a3dc569ba3657583ebdf5",
        // -fthumb-sched-immediate-before-pool: companion to the pool-load-late
        // class, where a lone `movs rN, #K' argument setter also issues before
        // a ready literal-pool load. Default-off and inert unless routed.
        // Witness resource_371:1a98, 2026-08-07. Cross-host rule: rebuild+pin
        // linux from the same commit before the next cloud session.
        "39dd5e4674ae60996d03a4187518eccccd40f1c7317452b3e0db22a20b71fecc",
        // -fthumb-hi-immediate: a small HImode constant reaches its register
        // with `movs' instead of the literal-pool load *thumb_movhi_insn's
        // "mn" alternative forces. Default-off and inert unless routed; the
        // standing "do not fix the mn" note in arm.md stays true globally.
        // Witness resource_377:03f8, 2026-08-07. Cross-host rule: rebuild+pin
        // linux from the same commit before the next cloud session.
        "4649d7267990a9f91503ade3b2259e15f94b702cddb0f41a6a566c14035d20d3",
      ],
    },
    gs2: {
      xgcc: ["128520f13ff01aee64a984b1279a6e3a682a3679de44c99296064f46fb1e8ec2"],
      cpp0: ["b4ac7f5ff7fd74f4eca40385832fd0360d13cb5d4f0b6c8b3ead4a67d2f3d5b0"],
      tradcpp0: ["7698319dfea3647dace68ffb5c3dbc0fd459f3a859699acb47c669d3eb8956a3"],
      cc1: ["91b2a67275a100e8b6695d85ef2d82d1fd144853cbcb361ddf1d8be31858230f"],
    },
  },
  // No approved builds exist for these two hosts yet. The empty sets are
  // deliberate: they make the admission path explicit instead of rejecting the
  // platform outright. Build with alchemy-gcc/build.sh + stage.sh on that
  // host, run the full `bun run verify`, then pin the digests here from that
  // green verify -- the same admission every listed digest already passed.
  "darwin-x64": {
    gs1: { xgcc: [], cpp: [], tradcpp: [], cc1: [] },
    gs2: { xgcc: [], cpp0: [], tradcpp0: [], cc1: [] },
  },
  "linux-arm64": {
    gs1: { xgcc: [], cpp: [], tradcpp: [], cc1: [] },
    gs2: { xgcc: [], cpp0: [], tradcpp0: [], cc1: [] },
  },
  "linux-x64": {
    gs1: {
      xgcc: [
        "845b828e15efedfeacc1956ac2694101e2b520824643d5b9f7608f9c389aee03",
        "2ed03493228a7873f020b16a63b89b3aadf4835be2d1a3a217cabca0fa244444",
        "1e7c30e3ba311981dcbf93bcc1b9e38e0581a993a22f93a7bc3193c21d254130",
      ],
      cpp: [
        "60d0b6637deb0f98cbf952a89694b02a0557fc87ca968121759be139372e90cc",
        "06096beb427848574f610626bb53408b1a76f69b178ee2d7f0a05f6c2f6d3778",
        "31bb32028d63e328877111df74a4d1390b6d5bfa8d077b739ae85bf7504d1a91",
      ],
      tradcpp: [
        "87f89bebf41cd12ac7706604dd24624061b2276f95cc1e9998c22de1accfee2a",
        "f9b951486d4e1769e06b892a59980c91a45435559505d73039130b63d1156803",
        "7a030f2838a1fa6c6e92091136dcdf8cf81d2cf49a2e11a6ee633d839c33ccac",
      ],
      cc1: [
        "c1cc6d2864567297451662d36fba7abbce7a916d138f7115832a265de6868a06",
        "640964de34d6202f6dc5943b0c22b0afd1a8f4f1307ba6d3cf30af4110f5f5e2",
        "c1c5be8f10668a7a66bfa2d6de2ca89f6ac17ffb57ffe62aedf7935da172f21a",
        // -fno-cse-shift-immediate added (K-class-only split of the
        // two-insn-immediate CSE policy; default-on, inert unless routed) --
        // admitted from a green `bun run verify` on this host.
        "13287dcb29210f197d7fe484532458197d21a7dc2bdf7ca3fca08af447631d3b",
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

const validated = new Set<CompilerTarget>();
let agbccValidated = false;
const experimentalValidated = new Set<string>();
const AGBCC_EXPECTED: Record<HostKey, readonly string[]> = {
  "darwin-arm64": [
    "4f7664872d10a737184fb2e0502c407c9d74505f0cff7313ba4e9083736c2207",
  ],
  "darwin-x64": [],
  "linux-x64": [
    "30a2a042c4be2acdd215ffc26c7d27498098ac38607ec8af43cc6598dcecdf55",
    "0c2d5ec04129f7b9d1ecf738f096167af152661bc2506f8fdb2749305fa3eb37",
    "21eca5a4e4d1138a1fdebccc03f6a140cbb74c072d3c10c299d64fa2cf13aef9",
  ],
  "linux-arm64": [],
};
const PRET_EARLY_THUMB_EXPECTED: Record<HostKey, readonly string[]> = {
  "darwin-arm64": ["8a1e0e9e18801efb595a3e0d571137db5ba8f97e413c323e99f18b0521a31636"],
  "darwin-x64": [],
  "linux-x64": ["c988f677e3ebd7252a6ad1ad2fef301f85b05be0612ee3192b37ec47d22f8082"],
  "linux-arm64": [],
};
const GCC2951_EXPECTED: Record<HostKey, readonly string[]> = {
  "darwin-arm64": ["cb41bba7e0e600721d906c46349119efb4c6fd35c711d7e0f244cb783de383a6"],
  "darwin-x64": [],
  "linux-x64": [
    "c8f80fffa2aa0aa2809d93ad86d11ea0e8ebf08e9bba6cc5b8d391aef05c3fe4",
    "edbee4fec1a1b59d0fd77273559aebbaf2c92b344bbeeb3539a10b689e71716d",
    "79859ae26c9c29d6b874fe27d4f4cdce72b80839ff05cc017906ef7e179c582a",
  ],
  "linux-arm64": [],
};
// Stock gcc-3.0 comparison probe (built.sh gcc3 / stage.sh gcc3), used only to
// test whether Golden Sun sources match unmodified GCC 3.0 codegen instead of
// the gcc-2.96/Camelot-ABI fork. Locally-built linux-x64 digest, 2026-08-04.
const GCC3_EXPECTED: Record<HostKey, readonly string[]> = {
  "darwin-arm64": [],
  "darwin-x64": [],
  "linux-x64": [
    "78840db683cc441be9741153418e0fd991ea6f628a0c4f08cb4bdc1cf5ebdb9b",
  ],
  "linux-arm64": [],
};

function outputText(value: Uint8Array): string {
  return Buffer.from(value).toString("utf8");
}

const UNSUPPORTED_HOST_MESSAGE =
  "alchemy-gcc supports darwin/linux on arm64/x86_64; this platform is none of those";

// The message a not-yet-admitted host gets. It names the admission procedure
// instead of implying the platform is out of scope: cross-host parity is a
// standing project rule, and the only thing an empty digest set means is that
// nobody has run the admission on that host yet.
function hostAdmissionMessage(host: HostKey, what: string): string {
  return [
    `alchemy-gcc has no approved ${what} digests for host ${host} yet.`,
    "Admit this host: build the committed fork source (alchemy-gcc/build.sh),",
    "stage it (alchemy-gcc/stage.sh), run the full `bun run verify`, and pin",
    "the digests from that green verify -- the same admission every listed",
    "digest already passed.",
  ].join(" ");
}

export function validateBundle(target: CompilerTarget = "gs1"): void {
  if (validated.has(target)) return;
  const host = hostKey();
  if (host === null) {
    throw new Error(UNSUPPORTED_HOST_MESSAGE);
  }
  const bundle = bundleForTarget(target);
  const entries = Object.entries(EXPECTED[host][target]);
  if (entries.every(([, expected]) => expected.length === 0)) {
    throw new Error(hostAdmissionMessage(host, target));
  }
  for (const [name, expected] of entries) {
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
    throw new Error(UNSUPPORTED_HOST_MESSAGE);
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
  const expectedAgbcc = AGBCC_EXPECTED[host];
  if (expectedAgbcc.length === 0) {
    throw new Error(hostAdmissionMessage(host, "agbcc/old_agbcc"));
  }
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
    throw new Error(UNSUPPORTED_HOST_MESSAGE);
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
  if (expected[host].length === 0) {
    throw new Error(hostAdmissionMessage(host, `experimental ${name}/cc1`));
  }
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
  | "gcc2951"
  | "gcc3";

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
    : family === "gcc3"
      ? GCC3_CFLAGS
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
        : family === "gcc3"
          ? GCC3_DRIVER
          : GCC2951_DRIVER;
    if (family === "old-agbcc") validateAgbccBundle();
    else if (family === "pret-early-thumb") {
      validateExperimentalCompiler(family, driver, PRET_EARLY_THUMB_EXPECTED);
    } else if (family === "gcc3") {
      validateExperimentalCompiler(family, driver, GCC3_EXPECTED);
    } else {
      validateExperimentalCompiler(family, driver, GCC2951_EXPECTED);
    }
    compilerInput = options.preprocessedOutput ?? inferredPreprocessedOutput(options.output);
    steps.push({
      kind: "preprocess",
      command: directPreprocessorCommand(
        options.input,
        compilerInput,
        family === "gcc2951" ? 95 : family === "gcc3" ? 0 : 9,
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

// Toolchain-free guards. Kept separate from `selfTest` so `main` -- which has
// no compiler bundle -- can run them alone via `--lint`, since the branch that
// carried the .map arity bug for four banks is exactly the one that could not
// execute the check that catches it.
export function callbackArityLint(): void {
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
  overlayStemCollisionLint();
}

// §7's routing key is the bare address, so every stem-keyed set is
// **overlay-blind**: an entry added for one overlay silently applies to every
// other overlay that later gains a row at the same offset. It does not fail as
// a byte diff — it surfaces two layers away as `palette token plan does not
// reconstruct input` from `build_assets`, on an overlay the change never
// mentions. This check generalises the rule already written above
// `NO_INTERWORK_SOURCES`, and
// like the scan above it needs no toolchain, so `main` can run it too.
export function overlayStemCollisionLint(): void {
  const root = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
  const source = readFileSync(join(root, "tools/alchemy_gcc.ts"), "utf8");
  const owners = new Map<string, string[]>();
  for (const name of readdirSync(join(root, "exact"))) {
    const stem = name.match(/_c_([0-9a-f]{8})\.c$/);
    if (stem === null) continue;
    owners.set(stem[1], [...(owners.get(stem[1]) ?? []), name]);
  }
  const collisions: string[] = [];
  const sets = source.matchAll(/const\s+([A-Z0-9_]+)\s*=\s*new Set\(\[([\s\S]*?)\]\)/g);
  for (const [, name, body] of sets) {
    for (const [, stem] of body.matchAll(/"([0-9a-f]{8})"/g)) {
      const files = owners.get(stem) ?? [];
      if (files.length > 1) collisions.push(`${name} "${stem}" -> ${files.sort().join(", ")}`);
    }
  }
  if (collisions.length > 0) {
    throw new Error(
      `these stem-keyed routing entries match more than one overlay source, so they ` +
      `apply to overlays they were never meant for -- move each to the path-keyed ` +
      `set naming the overlay it was added for: ${collisions.join("; ")}`,
    );
  }
}

// Cross-host parity gate: every flag the live routing tables can emit must be
// accepted by the staged binary that would receive it. Digest pinning alone
// cannot catch the skew this guards against -- a route added on one host
// family (with that host's rebuilt, pinned binary) leaves every other host's
// staged binary approved-but-behind, and the first symptom used to be an
// unrelated-looking mid-build failure deep inside an overlay rebuild. This
// lint turns that into an immediate, named, actionable failure.
export function flagCapabilityLint(): void {
  validateBundle("gs1");
  validateAgbccBundle();
  const gs1Flags = evidencedRoutingFlags("gcc296");
  const agbccFlags = evidencedRoutingFlags("agbcc")
    .filter((flag) => !AGBCC_CFLAGS.includes(flag as never));
  const probes: Array<{ binary: string; argv: string[] }> = [
    {
      binary: "gs1 cc1 (via xgcc)",
      argv: [driverForTarget("gs1"), `-B${bundleForTarget("gs1")}/`, ...CFLAGS, ...gs1Flags,
        "-S", "-x", "c", "-o", "/dev/null", "/dev/null"],
    },
    {
      // old_agbcc is cc1-shaped: no driver, no -x language selection.
      binary: "agbcc/old_agbcc",
      argv: [AGBCC_DRIVER, ...AGBCC_CFLAGS, ...agbccFlags, "-o", "/dev/null", "/dev/null"],
    },
  ];
  const missing: string[] = [];
  for (const probe of probes) {
    const spawned = Bun.spawnSync(probe.argv, { cwd: ROOT, stdout: "pipe", stderr: "pipe" });
    const stderr = outputText(spawned.stderr);
    for (const match of stderr.matchAll(/Unrecognized option `(-[^']+)'/g)) {
      missing.push(`${probe.binary}: ${match[1]}`);
    }
    for (const match of stderr.matchAll(/Invalid option `([^']+)'/g)) {
      missing.push(`${probe.binary}: -m${match[1]}`);
    }
  }
  if (missing.length > 0) {
    throw new Error(
      `staged compiler is behind the committed routing tables; unrecognized flags: ` +
      `${missing.join("; ")}. Rebuild and stage the committed fork source on this ` +
      `host (alchemy-gcc/build.sh && alchemy-gcc/stage.sh), run the full ` +
      "`bun run verify`, then pin the new digests from that green verify. " +
      `Cross-host parity is a standing rule: a routing or mode change lands only ` +
      `with every supported host family rebuilt, verified, and pinned.`,
    );
  }
}

function selfTest(): void {
  callbackArityLint();
  const expected = [
    "080069a4",
    "08006a00", "08006ba8", "08006c24", "08006c68", "08006cdc", "08006d50", "08006dec",
    "08006e24", "08006f48", "08006f84", "08007028", "08007098", "0800711c", "080071a8", "08007220",
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
  for (const stem of ["02005a40", "02005a78"]) {
    const source = join(ROOT, `exact/resource_3bf_c_${stem}.c`);
    const flags = cflagsForTargetSource("gs1", source);
    if (flags.includes("-fcall-used-r4") || flags.includes("-mthumb-interwork")) {
      throw new Error(`${stem} stock non-interworking ABI routing self-test failed`);
    }
    const unrelated = cflagsForTargetSource("gs1", join(ROOT, `exact/resource_3aa_c_${stem}.c`));
    if (!unrelated.includes("-fcall-used-r4") || !unrelated.includes("-mthumb-interwork")) {
      throw new Error(`${stem} overlay-path isolation self-test failed`);
    }
  }
  const groupedDma = [...GROUPED_DMA_STORE_SOURCES].sort();
  if (JSON.stringify(groupedDma) !== JSON.stringify([
    "08002f10", "08002fb0", "0800300c", "080037d4", "08003e10", "08004760",
    "08004838", "08004858", "080049e8", "08004a28", "08004a44", "08004a5c",
    "08004a94", "08005340", "08005394", "080053e8", "08005a78", "08005c68", "080060e8", "0800bc48", "0800bdd4", "0800c0f4", "0800d304",
    "08011590", "080170c4", "08019bac", "0801a4fc", "0801d014", "0801d980", "080251d4", "080284dc", "0808fe38", "0808fecc", "080907b0", "08090824", "08091174", "08094730", "08095160", "08095290", "080958a8", "08097540",
    "0809bb34", "080a1090", "080b010c", "080b0744", "080b5ad4", "080c0184", "080c08a8", "080f377c",
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
  const overlayDmaSource = join(ROOT, "exact/resource_3bd_c_02000c98.c");
  const overlayDmaFlags = cflagsForTargetSource("gs1", overlayDmaSource);
  const overlayDmaNeighbor = cflagsForTargetSource(
    "gs1", join(ROOT, "exact/resource_3bc_c_02000c98.c"),
  );
  for (const flag of ["-mgrouped-dma-store", "-fthumb-group-control-last"]) {
    if (!overlayDmaFlags.includes(flag) || overlayDmaNeighbor.includes(flag) ||
        cflagsForTargetSource("gs2", overlayDmaSource).includes(flag)) {
      throw new Error(`resource_3bd:0c98 grouped-DMA routing self-test failed for ${flag}`);
    }
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
  const value1BeforeBaseFlag = "-fthumb-group-value1-before-base";
  const value1BeforeBaseFlags = cflagsForTargetSource("gs1", "/tmp/080907b0.c");
  const value1BeforeBaseDirect = directCompilerCommand(
    "/tmp/080907b0.i", "/tmp/080907b0.s", "080907b0.c", "/tmp/080907b0.c",
  );
  if (!value1BeforeBaseFlags.includes(value1BeforeBaseFlag) ||
      !value1BeforeBaseDirect.includes(value1BeforeBaseFlag) ||
      cflagsForTargetSource("gs1", "/tmp/080907b4.c").includes(value1BeforeBaseFlag) ||
      cflagsForTargetSource("gs2", "/tmp/080907b0.c").includes(value1BeforeBaseFlag)) {
    throw new Error("080907b0 grouped-DMA value1/base routing self-test failed");
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
      cflagsForTargetSource("gs1", join(ROOT, "exact/resource_381/c/020000a0.c"))
        .includes("-mcall-arg0-move-first")) {
    throw new Error("overlay call-argument unrelated-source routing self-test failed");
  }
  const callArg0BeforeStore = "-fthumb-call-arg0-before-store";
  const callArg0BeforeStoreFlags = cflagsForTargetSource("gs1", "/tmp/08077f70.c");
  if (!callArg0BeforeStoreFlags.includes(callArg0BeforeStore) ||
      !callArg0BeforeStoreFlags.includes("-fno-sched-alias") ||
      !callArg0BeforeStoreFlags.includes("-fsched-store-first") ||
      cflagsForTargetSource("gs1", "/tmp/08077f74.c").includes(callArg0BeforeStore) ||
      cflagsForTargetSource("gs2", "/tmp/08077f70.c").includes(callArg0BeforeStore)) {
    throw new Error("08077f70 call-argument-before-store routing self-test failed");
  }
  const postcallByteIncrementR2 = "-fthumb-postcall-byte-increment-r2";
  if (!cflagsForTargetSource("gs1", "/tmp/08098b10.c").includes(postcallByteIncrementR2) ||
      cflagsForTargetSource("gs1", "/tmp/08098b11.c").includes(postcallByteIncrementR2) ||
      cflagsForTargetSource("gs2", "/tmp/08098b10.c").includes(postcallByteIncrementR2)) {
    throw new Error("08098b10 post-call byte-increment routing self-test failed");
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
  if (argument === "--lint") {
    callbackArityLint();
    flagCapabilityLint();
    console.log("lint=ok callback-arity overlay-stem-collisions flag-capability");
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
  if (argument === "gcc3") {
    validateExperimentalCompiler(argument, GCC3_DRIVER, GCC3_EXPECTED);
    console.log(`alchemy-gcc=${argument} ok host=${hostKey()} files=1 bytes=${statSync(GCC3_DRIVER).size}`);
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
