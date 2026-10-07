/* Draft: native 670 instructions, draft 670, common 670, differ 0, but
 * NOT adoptable yet: the two __asm__ __volatile__("" ::) CSE flushes
 * (step and burst address, see the fourth to seventh passes) change
 * instruction selection, not only order or registers (S2). The row
 * temporary and the CopyPalette inline remain FAKEMATCH devices.
 * Eleventh pass (2026-10-07): the step flush's cause, still not found.
 * thread_jumps runs after jump pass 1 and before CSE pass 1 (toplev.c),
 * so a repeated test is threaded away before CSE chooses its paths; in
 * the trials below a second jump to the join was always gone before CSE
 * chose them. The path has to end at a block boundary that leaves
 * no code in the final output. Of the forms tried, only a loop end does
 * that (an empty do-while, or the asm), and neither is a natural source.
 * Eleventh-pass trials, each without the step flush: the variant block
 * in its own if (frame >= k * 8) after the hit stanza, with shot assigned
 * in the first, 451 (the first test is threaded to the loop tail and CSE
 * folds the second); if (frame - k * 8 == 17) 369 (679 instructions:
 * frame - k * 8 becomes a giv, where native adds 17 to k * 8 and compares
 * it with frame); the hit test repeated around
 * BattleEventRuntime_BeginPhaseFar inside the hit block 451 (the jump dump
 * gives the join 2 uses, but CSE1's block list shows paths from the
 * k-loop top through the join to insns 929, 1005 and 1046, and the inner
 * test is gone); BattleEventRuntime_BeginPhaseFar in a second
 * if (frame == k * 8 + 17) after the hit block 451.
 * Tenth pass (2026-10-07): the flushes' natural cause, still not found.
 * The step flush needs CSE's AROUND path (cse_end_of_basic_block,
 * skip_blocks) to end after the hit block's join label: a code label
 * with uses, or a NOTE_INSN_LOOP_END, on the path between the join and
 * the variant test. A label or loop end inside the skipped hit block
 * does not help: the AROUND path jumps over it.
 * Tenth-pass trials (from 0, each without the flush it replaces): an
 * empty do { } while (0) after the hit block 0 (its LOOP_END ends the
 * path, as the asm does; still a device that changes selection);
 * do { BattleEventRuntime_BeginPhaseFar(133); } while (0) inside the
 * hit block 451; do { the hit block } while (0) 451 again (jump merges
 * the join label with the loop's exit label after the LOOP_END, so the
 * AROUND path skips it); a static __inline__ helper forwarding
 * BattleEventRuntime_BeginPhaseFar 451, one forwarding
 * ObjectGroup_UpdateMembers 451 (no label left in the hit block by CSE
 * time); the k loop as for (k = 0; k != count; k++) 456. Burst: group
 * set in the burst for's test, for (i = 0; group = k * 4, i != count;
 * i++), 24: CSE no longer folds group << 3 into k << 5 (group's set is
 * in the rotated exit test, another block), but the duplicated test puts
 * k << 2 before the count guard and PRE puts group << 3 at the loop
 * tail; native computes k << 2 after the guard and group << 3 at the top
 * of the body, so group's set sits at the body's top with a CSE block
 * boundary before its use.
 * Ninth pass (2026-10-07): the shard drawer is chosen in the call,
 * draw[burst->velocity_x > 0 ? flip ^ 1 : flip](...), with the plain
 * if (i & 1) picture += 9 first as native orders it (48 -> 8). fold()
 * distributes the index's * 4 into the conditional's arms, so the scaled
 * index t is set in both arms: a pseudo spanning blocks, which global (not
 * local-alloc, which never takes r7) gives r7 after the position pointer
 * takes r6; jump2's cross jump then merges the two identical
 * "lsls r7, r2, #2" tails, leaving native's ble over eors r2, r1. With t
 * no longer a local in r6, flip keeps r2, the constant 1 r1, the burst
 * reload r4, and the volatile position.x re-read and the odd temporary are
 * no longer needed (both removed; with the re-read kept also 0). The draw
 * loop steps burst before i (burst++, i++; 8 -> 0): reload takes r0 for
 * 28 and r1 for 1 in RTL order, then sched2 interleaves them as native.
 * Ninth-pass trials (from 48): shard_draw = &draw[flip] right after the
 * flip test 435; native order (i & 1 first, flip, shard_draw) 396 (the
 * shift still lands at the call in flip's r6); if/else flip =
 * effect->side ^ 1 / effect->side 403; if/else shard_draw = &draw[flip ^
 * 1] / &draw[flip] 99 (675 instructions: the base add stays in each arm,
 * but the position pointer takes r6); shard_draw = &draw[cond ? flip ^ 1 :
 * flip] then (*shard_draw)(...) 99. At 0: without the step flush 451,
 * without the burst flush 171, the count test through
 * SevenMode_Records[mode * 2] (no records, no row) 135.
 * Flush leads at 0: the burst flush replaced by do { group = k * 4; }
 * while (0) also scores 0 (no asm: the loop's notes or labels end CSE's
 * extended block between group's set and group * 8, so the fold into
 * k << 5 never happens). Not taken: still a device that changes
 * selection, but it points at a loop boundary in native's burst body
 * as the natural cause. The step flush replaced by do { the hit block }
 * while (0) 451 (as without the flush).
 * Eighth pass (2026-10-07): the shard draw goes through a pointer,
 * DrawRectangle *shard_draw = &draw[flip] and (*shard_draw)(...), with a
 * forced temporary odd = i & 1 whose test follows flip (FAKEMATCH, a
 * permuter hint; 64 -> 48). Everything outside the shard block now
 * matches: reload's rotation, the mode chain and both loop tails.
 * Cause, from the lreg/greg dumps: native's spill set includes r4 because
 * the burst pointer reload at the velocity_x load (insn "mov r4, r8")
 * finds r0 (picture), r1 (the constant 1), r2 (flip) and r3 (effect) busy.
 * The pointer form keeps the drawer load at the call (combine folds
 * &draw[flip] into the load; native ldr r4, [r7, r0]), but local-alloc
 * gives t = flip << 2 r6 (r7 is the hard frame pointer, never used by
 * local-alloc), global then prefers r6 for flip (set_preference through
 * the shift), the constant 1 lands in r2 and the reload takes r1: r4
 * never enters the spill set and the whole function shifts (396). With
 * odd computed first the constant 1 stays in r1 and r4 returns.
 * Remaining at 48, all in the shard block: native tests i & 1 before
 * flip (the draft's odd test follows it), keeps flip in r2 and t in r7
 * (so t was allocated by global, not local-alloc: some local of higher
 * priority holds r6 during t's life, or t spans a block boundary), and
 * the position pointer in r6 where the draft has r7.
 * Eighth-pass trials (on the 64 draft unless marked): records[mode * 2]
 * without row 68; pointer draw alone 396 (with the plain position.x
 * 418, without records 391); position pointer as register asm("r6")
 * in the burst draw block 438, with the pointer draw 262 (the burst
 * pointer then takes r7 and t ip); i & 1 test moved after flip without
 * a temporary 411; odd tested before flip 396; shard_draw = &draw[flip]
 * before the odd test 435 (440 without odd); at 48: (u32)flip 48,
 * plain position.x 257, records[mode * 2] without row 52, without
 * records 177. Permuter (5 minutes, PERM_RANDOMIZE over the shard
 * block, pointer form): odd first and its test after flip, 48. Second
 * permuter run (5 minutes, seed 7, from 48): a second forced temporary
 * tmp = burst->velocity_x before the odd test, with flip ^= 1 after it,
 * 44; not taken: one more device for four instructions, and native's
 * order (i & 1 tested before flip) is still not reproduced.
 * Seventh pass (2026-10-07): the burst count indexes records with a row
 * temporary (FAKEMATCH, 68 -> 64, a permuter hint): records[mode * 2]
 * expands to (plus (mult mode 2) records), the multiply first as RTL
 * canonical order puts it, so reload loads the row into r0 and the table
 * into r1; with a plain register index the address keeps records first
 * and native's load order (table r0, row r1). The global
 * SevenMode_Records[mode * 2] keeps native's order too, which is why only
 * the uses of the local pointer swap: with the count through the global and
 * the burst velocity test through records, the swap moves to the velocity
 * test (68). The shard draw and both loop tails remain (r4 at the call).
 * Seventh-pass trials: (mode * 2)[records] 68; group set in the burst
 * for's init and increment (for (i = 0, group = k * 4; ...; i++,
 * group = k * 4)) without the burst flush 92: the address stays unreduced
 * as native, but k << 2 lands before the count guard and group << 3 at the
 * loop tail. Without the step flush the hit block's join label (1 use) is
 * skipped AROUND by CSE's path following (cse_end_of_basic_block,
 * skip_blocks), which carries k << 3 (reg 214) and shot (reg 215) past
 * the hit block to the variant test and step: native's loss needs that
 * label to have a second use, a code label inside the hit block, or a
 * NOTE_INSN_LOOP_END between the join and the variant test; no natural
 * source for any of them found yet.
 * On the 64 draft: direct draw[flip](...) 396 and DrawRectangle *shard_draw
 * = &draw[flip] with (*shard_draw)(...) 396, both 652 instructions. The
 * shard draw itself then nearly matches (drawer loaded at the call into
 * r4 from [sp+32] as native), but flip << 2 is computed at the call into
 * flip's own register (r6) where native shifts it into r7 right after the
 * flip test, and the mode chain loses r4 (mov r0, r8 for native's
 * mov r4, r8): the load at the call changes reload's spill set for the
 * whole function, which the rest of the draft has been fitted without.
 * Permuter (6 minutes, PERM_RANDOMIZE over the burst count and the shard
 * draw, asm lines in PERM_IGNORE): only the row temporary, 64.
 * Sixth pass, later (2026-10-07): the burst count reads SevenMode_Records
 * through a local s8 *records (197 -> 80): the reload rotation through the
 * burst block and the k-loop tail now match. Whether the pointer is
 * Camelot's source or a forced temporary (S2 tag needed) is open: without
 * it the count stays at 199 in every form tried. The draw loop steps a user
 * burst pointer set in the for's init beside i (80 -> 72): native sets
 * i and the pointer before the hoisted SevenMode_Records base. The count
 * as if/else (72 -> 68): if-conversion sets count = 4 after the load, as
 * native. Remaining at 68: the burst count's two loads in swapped order
 * (native loads the table address into r0 first, then the index into r1),
 * the position pointer in r7 where native has r6 with flip << 2 in r7 and
 * draw[flip] loaded at the call (shard_draw unallocated), the draw loop
 * tail and frame-loop tail registers that follow from it.
 * Trials: if/else for the burst count (no pointer) 199; records also in
 * the burst loop's test 84; s8 *records = &SevenMode_Records[mode * 2]
 * with *records 80; burst = gBursts before the for 80; direct
 * draw[flip](...) 390; (*p)(...) with p = &draw[flip] 390. At 68:
 * *(records + mode * 2) 68; records = &SevenMode_Records[mode * 2] with
 * *records 68; shard_draw or flip declared first 68; direct draw[flip]
 * call 396 (652 instructions: r4 re-enters reload's rotation from the
 * first mode test on, as in the fourth pass).
 * Next: the two CSE flushes still block adoption (one natural cause for
 * the lost k << 3, gShots + k * 448 and group << 3 knowledge after the hit
 * block); then the shard draw's flip << 2 in r7 with draw[flip] loaded at
 * the call, which only the direct call gives, without losing r4.
 * Sixth pass (2026-10-07): native's delay is loop.c's reduced giv of k * 8,
 * not a variable. The draft writes k * 8 inline (no delay local).
 * - Reducing it needs benefit above the add cost (2 - 2 = 0 for one k << 3,
 *   "not worth while, 0 vs 167"); two separate k << 3 givs combine (2 + 2 - 2)
 *   and are reduced. So native's step, (frame - k * 8) / 3, computes k << 3
 *   afresh: CSE pass 1 did not carry the loop top's k << 3 past the hit
 *   block (frame == k * 8 + 17). With the flush there, delay and the shot
 *   pointer take native's slots (sp+12 below sp+16, giv pseudos made after
 *   shot's) and native's tail order (shot += 448, delay += 8, k++).
 * - Native's variant test and EffectPosition_ApplyBaseAndYOffset argument
 *   use the reduced shot giv (reloaded from sp+16), not the shot pointer
 *   in r7: gShots + k * 448 is also recomputed there; written as
 *   gShots[k * 16] after the flush, they match (205 -> 203).
 * - The burst address's lost group << 3 fold (below) is the same lost CSE
 *   knowledge in the same region; one natural cause probably explains all
 *   three. Not found: a split frame test 520, a for loop without the flush
 *   476, shot declared inside the variant block 201.
 * - Frame slots: camera + 12 (expression 92) and k << 2 (expression 133)
 *   share GCSE hash bucket 50, so PRE made camera + 12's reaching reg first
 *   (sp+20 above k << 2). Declaring camera after source (one pseudo later,
 *   bucket 51) gives native's order, k << 2 at sp+24 and camera->pos at
 *   sp+20 (203 -> 197).
 * Remaining at 197, besides the two flushes: reload rotation from the
 * burst count's SevenMode_Records load (native: address r0, index r1; the
 * draft's address pseudo takes r3, the index r1) through the burst block
 * and the k-loop tail; the draw loop's r8/r9 pool loads and flip << 2 in
 * r7; one branch two bytes longer (b.n +784 vs +782).
 * Sixth-pass trials: k * 8 inline without the step flush 467; with it 205;
 * ApplyBase(&gShots[k * 16]) 203; flush moved before the variant test with
 * gShots[k * 16].variant 203; camera after source 197; unused delay
 * removed 197; without the burst flush 253.
 * Fifth pass (2026-10-06): delay += 8 before k++ (232 -> 231, and 273 ->
 * 269 without the asm; native's tail updates the shot pointer, then delay,
 * then k). The burst address
 * still needs the diagnostic asm, which must not be adopted: no plain
 * source form found yet. Without the asm the draft scores 273; the CSE dump confirms insn 1076
 * (group << 3) becomes (ashift k 5): fold_rtx finds (ashift k 2) in
 * group's class through lookup_as_function, which only fails when k has
 * been set again since group's set (exp_equiv_p validates reg ticks) or
 * when the two sit in different extended blocks (a CODE_LABEL ends one).
 * Fifth-pass trials, all without the asm unless marked: a row view
 * gBurstRows[group][i] (struct EffectStep (*)[8]) 301, [k * 4][i] 301,
 * with the asm 299; for (i = 0, group = k * 4; ...) 273; i + group * 8
 * 273; k << 2 with (group << 3) + i 273; n = group * 8 then n += i 291;
 * &gBursts[group * 8] then burst += i 506 (504 with the asm); delay as a
 * s32 delay = k * 8 local in the target loop 467 with the asm (335
 * without; the frame shrinks to 76 and fp loses the sheet base); delay = 0
 * before the count test 287; shard draw as DrawRectangle *p = &draw[flip]
 * then (*p)(...) 410, draw + flip 410, direct draw[flip](...) 410 (r4
 * returns to reload's rotation and the mode chain shifts again);
 * delay declared in the frame loop block 232; a user shot pointer
 * (shot = gShots with delay = 0 after the guard, shot += 16 at the tail)
 * 344, with k++ first 350.
 * Permuter without the asm (5 minutes, PERM_GENERAL over k * 4 / k << 2
 * and the four index orders, PERM_RANDOMIZE over the burst loop): no
 * candidate beat 273.
 * Frame slot lead (delay): native's delay is not a reduced giv of k. With
 * k * 8 inline or as a s32 delay = k * 8 local, CSE leaves one k * 8 reg
 * (giv benefit 2, lifetime 35) that loop.c rejects ("not worth while,
 * 0 vs 167"), and k * 8 + 17 (DEST_REG) cannot combine into it, so k * 8
 * is recomputed each pass. In the draft, global leaves the user var delay
 * (reg 47, 17 refs over 314 insns, crosses 12 calls) in memory, so it
 * takes a slot in pseudo order (sp+32) beside frame, k, total and camera.
 * Native's delay slot comes last (sp+12), after the loop-made pseudos;
 * this fits a pseudo that global gave a hard register and reload then
 * spilled (reload numbers those slots after the ones global left), so the
 * next trial is a delay that global allocates (more references or a
 * shorter life).
 * Fourth pass (2026-10-06): the burst loop's cause is found, but only a
 * diagnostic device reproduces it; the device is NOT admissible as is
 * (S2: it changes instruction selection, not only order or registers).
 * - Native's inner loop holds group = k << 2 and group << 3 apart. GCSE's
 *   PRE (pass 07) hoists the invariant k << 2 to the loop entry (spilled
 *   to sp+24); group << 3 stays in the loop because group is set in the
 *   loop, so loop.c finds no invariant add for the address, no giv, and
 *   no strength reduction: exactly native's per-iteration address and
 *   in-loop gBursts pool load.
 * - Written plainly (group = k * 4 in the loop body), CSE pass 1 folds
 *   group << 3 into k << 5 (fold_rtx's shift association through
 *   lookup_as_function), and with group set before the loop PRE hoists
 *   group << 3 itself; either way the address becomes a giv (benefit 14,
 *   life 55) and is reduced to a pointer stepped by 28.
 * - The draft separates them with __asm__ __volatile__("" ::), an
 *   ASM_OPERANDS that flushes CSE's table (asm("") is an ASM_INPUT and
 *   flushes nothing). The burst loop then matches native instruction for
 *   instruction apart from frame slots and reload's register rotation.
 *   Next step: the source form in which CSE pass 1 cannot see
 *   group = k << 2 from group * 8 while both stay in the loop body.
 * - With the burst address right, delay = 0 inside the count test now
 *   helps (287 -> 232; 273 without the device, as before).
 * - Remaining at 232: frame slot order. Native (high to low address):
 *   camera 36, &draw 32, records index (mode * 2) 28, k << 2 24,
 *   camera->pos 20, shot pointer 16, delay 12; the draft puts delay at 32
 *   (a user pseudo, low number) and k << 2 below camera->pos. Native's
 *   delay looks like a loop-made pseudo (k * 8 reduced as a giv), but
 *   k * 8 inline with the device gives 467 (the frame loses a slot and
 *   fp no longer holds work).
 * Fourth-pass trials: group = k * 4 before the burst loop 267 (PRE hoists
 * group << 3); with asm("") 267 (no flush); group in the loop with
 * asm("") 267 (folded to k << 5); group also re-set at the loop tail 303
 * (address unreduced, k << 2 before the guard); group in the loop with the
 * flushing asm 287; then delay = 0 inside the count test 232; delay += 8
 * before k++ 296 (from 287); k * 8 inline 467; delay declared in the count
 * block 232; direct draw[flip](...) call 417 (loads at the call as native
 * does, but flip << 2 is scheduled late into flip's own register); the
 * same direct call on the 232 draft 410.
 * Third pass (2026-10-06): the mode-test reload rotation now matches
 * (r2, r3, r4, r0, r1, r2). Two shard-draw causes kept r4 out of reload's
 * spill registers for the whole function:
 * - the chosen drawer is loaded into its own local (shard_draw) before the
 *   arguments; native computes flip << 2 into r7 right after the flip test
 *   and loads the pointer at the call (442 -> 367 alone);
 * - native re-reads position.x from [r6] after __divsi3; CSE (pass 1, insn
 *   1461) carried x >> 1 across the const libcall into r4, caller-saved at
 *   sp+8. A FAKEMATCH volatile re-read restores it (367 -> 327).
 * The target loop is a do-while guarded by the count test (a permuter
 * hint, 326 -> 267; a while loop or a for with the step in the body stays
 * 326): the loop-top registers and loads now match native.
 * Remaining: (1) the burst loop. Native never strength-reduces the burst
 * address: each iteration recomputes ((k << 2) * 8 + i) * 28 + gBursts
 * (pool load in the loop), with k << 2 hoisted to the loop pre-header and
 * spilled to sp+24; the draft's inner loop reduces it to a pointer stepped
 * by 28 (loop dump: giv 1080 combined, benefit 184), whose k * 896 start
 * the outer loop then reduces to sp+12 where native keeps delay (k * 8).
 * That shifts every frame slot (records index sp+24 vs 28, draw base sp+28
 * vs 32). (2) Native keeps flip << 2 live in r7 to a load at the call
 * (shard_draw unallocated, REG_EQUIV to draw[flip]); the draft allocates
 * shard_draw to r6 and loads early. (3) literal pool offsets.
 * The four SevenMode tables still need native-pool labels before adoption.
 * Trials: direct palette calls 550; pointer/array burst forms 484/495;
 * an explicit burst-group index 486 (442 with the per-target delay);
 * burst pointer stepping 446; i + k * 32 444;
 * (k * 4) * 8 and k << 2 << 3 444; a k * 4 local 581; draw-pair shape 484;
 * draw cursor 473; group = k * 4 before the burst loop 444; k * 8 inline
 * with k * 32 + i 484; records as two-byte structs 442; group += 4 with
 * group * 8 + i 442; explicit delay with (k << 2 << 3) + i 444.
 * Third-pass trials (from 442): volatile position.x re-read alone 412;
 * shard_draw = draw[flip] alone 367, with the re-read 327; then
 * k * 32 + i (no group biv) 326, group = k * 4 before or inside the burst
 * loop 326, k * 4 * 8 + i 326; k * 8 inline 358; gMapCellBuffer as a
 * symbol for gBursts 545 (both buffers 533; the burst address is still
 * reduced); DrawRectangle *shard_draw = &draw[flip] 414; guarded do-while for the
 * target loop 267 (permuter, 5 minutes: 263 with two casts and a
 * temporary as noise); with k * 8 inline instead of delay 331.
 * With the guarded do-while: group = k * 4 inside the burst loop 267;
 * delay = 0 inside the count test 273; a function-scope burst pointer
 * shared with the draw loop 267; a separate burst counter j 267; without
 * the volatile re-read 338;
 * permuter on the burst block (5 minutes) 255 with s8 *tmp =
 * SevenMode_Records for the count test, a forced temporary not adopted
 * while the burst address still differs. Native's first literal pool sits
 * after the step > 4 draw; the draft's lands 152 bytes later.
 * Fourth pass (2026-10-06), the burst address: loop.c reduces a giv only
 * when its add value is invariant and the reg it feeds is a biv or a giv
 * (loop.c simplify_giv_expr). Native's burst loop has no givs at all: k << 2
 * sits in the preheader in a pseudo whose stack slot (sp+24) lies between
 * the records index and camera + 12, i.e. a GCSE reaching register, not a
 * user variable or loop register (slots follow regno, higher regno lower).
 * In the loop it is shifted by 3 (CSE never folded the two shifts, so they
 * were in different CSE blocks or the << 2 value was unknown there) and fed
 * into an index that is set more than once per iteration, so nothing from
 * it is a giv; the gBursts pool load stays in the loop as "not desirable".
 * A k * 4 the k loop sees must also stay unreduced (native recomputes it
 * from k at sp+44). Measured (from 267): a volatile group local 463 (burst
 * loop and pool offsets then match native, frame does not); an index n set
 * three times in the loop (n = k * 4; n *= 8; n += i) 291, no burst givs
 * but CSE folds to k << 5 and the k loop reduces k * 32 into sp+16; the
 * same with group = k * 4 at the k-loop top 291, in the guard or before
 * count 311/318, group += 4 per target 283, k * 4 * 8 into n 267; an
 * asm("" : "+r"(n)) after n = k * 4, then n = n * 8 + i, 247 (burst body
 * as native, but GCSE's k * 4 copy then lets the k loop reduce k * 4 into
 * sp+16): a measuring device, not adoptable (S2: it changes which loop
 * reduces, not only order or registers). Reused height or total as the
 * index 267/320; k * 8 inline for delay with that device 309.
 */
#include "CANVAS.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "HEAP_STATE.H"
#include "IO_REG.H"

extern s8 SevenMode_Records[];
extern u8 SevenMode_ShardWidths[];
extern u8 SevenMode_ShardHeights[];
extern u16 SevenMode_ShardOffsets[];

#define gShots ((struct EffectStep *)Ram_MapCellBuffer)
#define gBursts ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))

typedef s32 (*WordCopy)(void *, const void *, s32);

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void AudioCommand_PlayFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *target);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);

static __inline__ void CopyPalette(WordCopy copy, void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: the palette copies share the routine's address but build
       the palette address again at each call, which only an inlined constant
       argument compiles to; a direct call keeps it in a register. */
    copy(destination, source, size);
}

void BattleFx_RunSevenMode(struct BattleEffectArgument *effect, s32 mode)
{
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 k;
    s32 total;
    struct MotionObject *source;
    struct BattleCamera *camera;
    u8 *palette;
    s32 i;
    s32 height;

    heap_cache = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = heap_cache[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
    palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
    CopyPalette(Iwram_CopyWords, (void *)BG_PLTT, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    palette = Resource_GetTableEntry((s32)&ResourceId_SmokeSheet);
    CopyPalette(Iwram_CopyWords, (void *)BG_PLTT, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work->sheet + 0x3e80);
    if (mode == 0)
        palette = Resource_GetTableEntry((s32)&ResourceId_BluePalette);
    else if (mode == 1)
        palette = Resource_GetTableEntry((s32)&ResourceId_SpiderWebSheet);
    else if (mode == 2)
        palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
    else if (mode == 3)
        palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
    else if (mode == 4)
        palette = Resource_GetTableEntry((s32)&ResourceId_GoldShellSheet);
    else if (mode == 6)
        palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
        palette = Resource_GetTableEntry((s32)&ResourceId_GreenPalette);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);

    for (i = 0; i != 1024; i++)
        gShots[i].variant = -1;
    source = GetBattleObjectSlotFar(work->effect->actor)->object;
    height = 0x40000;
    for (k = 0; k != work->effect->count; k++) {
        struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[k])->object;

        for (i = 0; i != 16; i++) {
            struct EffectStep *shot = &gShots[k * 16 + i];

            shot->x = source->x;
            shot->y = height;
            shot->z = source->z;
            shot->velocity_x = (target->x - source->x) >> 4;
            shot->velocity_y = 0x40000;
            shot->velocity_z = (target->z - source->z) >> 4;
            shot->variant = 0;
        }
    }
    for (i = 0; i != 256; i++)
        gBursts[i].variant = -1;

    if (effect->side == 0) {
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
        draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
        if (SevenMode_Records[mode * 2] == 0) {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        } else {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        }
    } else {
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 2);
        draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
        if (SevenMode_Records[mode * 2] == 0) {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        } else {
            BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
            draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
        }
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (SevenMode_Records[mode * 2] == 0)
        total = work->effect->count * 8 + 72;
    else
        total = work->effect->count * 8 + 56;
    AudioCommand_PlayFar(103);

    for (frame = 0; frame != total; frame++) {
        struct EffectStep *burst;

        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
        k = 0;
        if (work->effect->count != 0) {
            do {
                if (frame >= k * 8) {
                    struct EffectStep *shot = &gShots[k * 16];

                    if (frame == k * 8 + 17) {
                        ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 16);
                        BattleEventRuntime_BeginPhaseFar(133);
                    }
                    /* Diagnostic, not admissible as is (see header): native recomputes
                       k << 3 and gShots + k * 448 after the hit block (205 -> 197 with
                       the gShots[k * 16] forms; 231 -> 205 alone). */
                    __asm__ __volatile__("" ::); /* FAKEMATCH: flushes CSE so step's k * 8 stays a separate giv */
                    if (gShots[k * 16].variant >= 0) {
                        s32 step = (frame - k * 8) / 3;

                        if (step > 9)
                            step = 9;
                        EffectPosition_ApplyBaseAndYOffset((s32 *)&gShots[k * 16], &position);
                        position.x >>= 1;
                        if (step > 4)
                            draw[0](canvas, work->sheet + step * 768, position.x - 16, position.y - 12, 32, 24);
                        else
                            draw[0](canvas, work->sheet + step * 768, position.x - 12, position.y - 16, 24, 32);
                        if (shot->variant == 0)
                            EffectStep_AdvanceWithGravity3D(shot, 63, -0x8000);
                        if (shot->y < 0) {
                            s32 count;
                            s8 *records = SevenMode_Records;
                            s32 row;

                            shot->y = 0;
                            shot->variant = 1;
                            /* FAKEMATCH: native loads the table address before the row
                               (r0, r1); records[mode * 2] puts the multiply first in the
                               address and reload loads the row first (68 -> 64). */
                            row = mode * 2;
                            if (records[row] == 0)
                                count = 4;
                            else
                                count = 16;
                            for (i = 0; i != count; i++) {
                                struct EffectStep *burst;
                                s32 group = k * 4;

                                /* Diagnostic, not admissible as is (see header): native keeps
                                   group = k << 2 and group << 3 apart and leaves the address
                                   unreduced (287 -> 232 with delay = 0 in the count test). */
                                __asm__ __volatile__("" ::); /* FAKEMATCH: flushes CSE so group << 3 is not folded into k << 5 */
                                burst = &gBursts[group * 8 + i];

                                burst->x = shot->x;
                                burst->y = shot->y;
                                burst->z = shot->z;
                                if (SevenMode_Records[mode * 2] == 0) {
                                    burst->velocity_x = ((Random16() & 63) - 32) << 11;
                                    burst->velocity_y = 0;
                                    burst->velocity_z = ((Random16() & 63) - 32) << 11;
                                } else {
                                    burst->velocity_x = ((Random16() & 63) - 32) << 13;
                                    burst->velocity_y = ((Random16() & 31) + 32) << 12;
                                    burst->velocity_z = ((Random16() & 63) - 32) << 13;
                                }
                                burst->variant = 0;
                            }
                        }
                    }
                }
                k++;
            } while (k != work->effect->count);
        }
        for (i = 0, burst = gBursts; i != 256; burst++, i++) {

            if ((u32)burst->variant <= 44 && burst->y >= 0) {
                EffectPosition_ApplyBaseAndYOffset((s32 *)burst, &position);
                position.x >>= 1;
                if (SevenMode_Records[mode * 2] == 0) {
                    draw[1](canvas, work->sheet + burst->variant / 8 * 1152 + 0x3e80,
                        position.x - 12, position.y - 24, 24, 48);
                } else {
                    s32 picture = burst->variant / 5;
                    s32 flip;

                    if (i & 1)
                        picture += 9;
                    flip = effect->side;
                    draw[burst->velocity_x > 0 ? flip ^ 1 : flip](canvas,
                        work->sheet + SevenMode_ShardOffsets[picture] + 0x1e00,
                        position.x - SevenMode_ShardWidths[picture] / 2,
                        position.y - SevenMode_ShardHeights[picture] / 2,
                        SevenMode_ShardWidths[picture], SevenMode_ShardHeights[picture]);
                }
                if (SevenMode_Records[mode * 2] == 0)
                    EffectStep_AdvanceWithGravity3D(burst, 62, 0x800);
                else
                    EffectStep_AdvanceWithGravity3D(burst, 62, -0x8000);
                burst->variant++;
            }
        }
        Camera_ApplyShake(2, 2);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
