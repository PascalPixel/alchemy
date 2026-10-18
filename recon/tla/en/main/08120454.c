// BattleTargetExec, tla-en 0x08120454.
// alchemy diff: listing 3450 instructions, draft 3450, common 3426,
// differ 48 (2026-10-08; 56 before the HP drain below, 70 before the
// order devices).
// This draft is The Broken Seal's source (0x080bbb0c,
// games/tbs/src/battle/battle.c) with this game's differences written in; they
// were worked out under GAME_TLA in that file and moved here, where they stay
// until every edition of this game matches. Then one source in games/common
// carries only the real differences under GAME_TLA. Not real differences, to
// settle against ☀️'s form then: the locals block (this one orders offset,
// actionId, range, scale and pass differently, which steers registers, and
// declares apwr for the whole function), targetId u8 or s32 (no change), and
// MsgSplitFailed where ☀️ has MsgSplitFail (each game's PO name for the same
// message). Scores: the old hand draft 806; ☀️'s source compiled for this game
// with only its names resolved 3237. One power variable (apwr) for every damage
// case moved it from differ 2590 to 779, the additive case's own loop counter
// to 657, ⚓️'s unsigned guardLevel to 552, case 401 as a case of its own in the
// rate switch (rate 12 set before the test, as the listing does) to 510, the HP
// drain halving dealt in apwr (declared for the whole function, so it keeps r7
// as in the listing) to 428, the PP restore reading action->effect directly
// (the listing loads it again) and adding maxPp / 10 before reading maxu in
// case 77 to 424 (draft 3454, common 3240). BattleEvPush declared void for this
// game (its body here sets no result, and void calls load r0 before r1, as the
// listing does at 61 of 68 two-argument calls) to 330 (common 3287). Then tried
// for the HP drain, where the listing keeps heal in r5 and the amount in r6
// (ours the reverse): the amount in tmp (384), in a block-local amt (505), hp
// read after the halving (557); heal += apwr before dmg = apwr changes nothing.
// The weapon case halves defense with a signed shift sequence in the listing
// (lsrs #31, adds, asrs) where ours shifts logically: def / 2 and scale /= 2
// give the same logical shift, defense read through the function-level value
// 382. With the void BattleEvPush, plain hp reads at the top of the heal still
// lose: signed first 351, unsigned first 347, and plain maxHp reads in every
// branch too 639. The listing reads maxu before the division call and shares
// the add after it with 78 and 7%: reading maxu first instead merges both loads
// (723); direct reads with maxv as before give 435; with the void BattleEvPush,
// maxu first gives 635, and with (s16)maxu / 10 as well 635. There the listing
// copies the target into r2 at the PP restore's head, so the effect's ldrb fits
// between the pp loads and case 77 keeps r2; ours copies it into r3, which the
// effect load takes. The summon's enemy reset as a for loop over saved[] keeps
// 330 only with its (u16) read (377 without). Tried and worse: a while loop
// for the immobilize scan (832), tmp or one shared amt for the drains (708,
// 716), while loops for the affinity scans, plain hp/maxHp reads in the
// heal (557: one ldrh shared by both; 445 with the signed read first),
// the heal branches reading through stat_ptr (808), the HP drain's amount in a
// block-local amt or with the new HP held in dmg (605 both), rate set to 0 in
// the switch's default instead of before it (665). No change: targetId s32
// instead of u8, case 401 last, apwr's place among the declarations,
// BattleEvPush without a prototype, heal += dmg instead of heal += apwr (kept:
// one line shared with ☀️), plain maxHp reads and value casts in place of the
// heal and PP restore's pointer casts (kept: the same instructions).
// The enemy-slot scan after a summon now matches. Still to bring over: the
// rate switch's 409 test (the listing derives it from
// 411 in r3; ours loads it fresh in r2), the summon message tail, the heal's
// target copy (the listing copies r8 again in each branch; ours keeps one
// copy in r4); then register order. The 409 test costs two bytes (a fresh
// constant, not subs from 411), which moves every branch offset in the
// rate switch; most other differences are one load swapped with a
// neighbouring move, the load order K2 leaves open.
// Tried after the split, against 330: case 409 a case of its own with rate
// 12, before the 399 group or after 401 (348 both), 409 last among its
// labels (no change), 409 sharing 401's case (336: draft 3450, but rate 12
// is no longer set before the 401 test), default last or with 393 last,
// rate as u8 or s16 (no change), rate as the function's kind (509) or tmp
// (660); in the heal, case 76 reading maxHp directly or maxu before maxv
// (337 both), maxu before maxv in case 71 (648), 2 * maxv / 5 (no change);
// in the PP restore, old
// read before heal (332), the block's declaration order and pp read
// through s16 and u16 pointers (no change); in
// the HP drain, a block-local amt (505: heal in r5 and amt in r6 as in the
// listing, but apwr then shares r6 and the listing's copy from r7 is
// lost; heal += apwr after amt = apwr keeps 505), the HP in hp0 (740) or
// tmp (344); for the weapon case's signed
// halving, (s16)def / 2 or (s16)scale / 2 (336: the sign is extended
// first), (s16)target->def read into def (382), defense read into tmp
// (334), kind (no change) or apwr (560), value (382), power (383), guard
// (332), count (no change), scale /= 2 and def /= 2 in place (no change):
// the shift stays logical whichever variable holds the zero-extended
// load, even one set elsewhere from signed values (power, value).
// Case 77 written as 78 and the 7% are, maxv then maxu read before the
// division and heal += (s16)maxv / 10 (a short division, so the quotient
// is sign-extended as in the listing), gives 328 (draft 3452): the listing's
// shape, ldrh before the call and a branch to the shared add, leaving only
// register choices there (its target copy stays in r2 from the head; ours
// is copied again into r0). (s16)(maxv / 10) gives 341, maxv / 10 kept in
// maxv then (s16)maxv 337, and maxu read before maxv 635. With that case,
// old read before heal at the PP restore's head gives 320 (draft 3450):
// the head's copy of the target now stays in r2 and case 77 matches
// exactly, but the head shares one ldrh between old and heal and
// sign-extends it, where the listing loads ldrsh, the effect, then ldrh.
// heal or old read through an s16 or u16 pointer keeps 320; the block's
// declaration order changes nothing. A memory-clobbering asm barrier
// after old splits the load and keeps r2 (327: old's ldrh then comes

// first, before the action load, where the listing has it last); after
// heal, with heal read first, 328; an empty asm("") changes nothing.
// The rate switch's 409 test: rate 9 is two case groups, 408 in one and
// 410 in the other (here 398 390 408 382, then 392 410), 226 (draft
// 3448, common 3336). The rate switch then matches: as two labels, 408
// and 410 keep the second test of the 409 node until after combine, so
// 409 stays a register and is derived from 411 (subs r3, #2); in one
// group the second test goes early and 409 becomes a reload in r2. Every
// split of the six into two groups that parts 408 from 410 gives 226;
// 410 and 408 together in a group of their own 290, 410 alone 322, 408
// alone 341 (a lone case's rate is set before its test).
// The weapon case's halving matches as the else of the half == 2 test
// (if (half == 2) scale = 0; else scale = def / 2;), 204 (draft 3450,
// common 3348): the else starts at a label, so combine no longer knows
// def is not negative and keeps the signed shift sequence, and the
// compiler still emits the halving before the test as the listing does.
// In the same order as before, def / 2, scale /= 2 with or without
// def, the field divided directly, def as u16, or half == 1 and
// half == 2 tests all stay logical (226 or worse); def as s16 276.
// Against 204, in the heal: case 76 (and 71) reading maxHp straight into
// the product, or maxu read after the add, 239 to 241 (draft 3451).
// The HP drain matches with the new HP in dmg and the amount in kind
// (dmg = actor->hp; kind = apwr; dmg += kind; ...), 178 (common 3361):
// dmg keeps r5 as everywhere else, and the amount, a second function-level
// variable, takes r6 with its copy from apwr in r7. The same with the
// amount in tmp 393, count 406, power 439, value 2112; the new HP and
// amount swapped in their old names (heal the amount) 379.
// In the heal, case 76 reading maxHp into a local of its own (m), 174
// (common 3363): the product then loads straight into r0 as in the
// listing; its target copy and offset still take r3 and r2 where the
// listing has r2 and r3, and a later tail is 2 bytes longer. The same
// local in case 71 as well 174, in 71 alone 178; heal held in dmg in the
// heal or the PP restore no change (they share r5 already); kind for
// old or maxv, or apwr for 76's maxv, worse (184 to 249).
// The 312/316 attack sum in a variable of its own (atk, local to the
// else), with the HP drain halving dealt in size instead of apwr, 170
// (common 3365): the 312/316 block matches, atk sharing r5 with dmg and
// 312 held in r6. As kind, the sum shares the drain's pseudo, which
// conflicts with dmg, so it took r6 and 312 took r5. atk alone, or dmg,
// def or any block-level local for the sum, 375 (draft 3449): kind then
// lives only in the drain, takes apwr's register and the copy goes,
// and apwr, allocated just ahead of 312, takes r6. In the drain with atk:
// the halving in tmp 174, power 200, count 262, hp0 454, n 479; with the
// halving in size, the amount in tmp 184, count 197, power or dealt 293,
// hp0 547; halving in apwr with the amount in size, tmp, count, power,
// dealt or hp0 389 to 691; switch (actionId) for the 312/316 test or
// one if after the other no better (375, 294). The drain held the
// halving in r0 there; halving dealt in place (if (...) dealt /= 2;
// kind = dealt;) 142 (common 3379), with no other variable for it;
// the halving in pass, bonus, half, adjust, crush, hit, modifier, skip,
// affinity or offset 200, guard 244, count 262, nibble 270, hp0 454,
// value 455, n 479. dealt stays on the stack, so ours halves it
// through r3 and stores it back, where the listing loads it into r7
// once and copies it to the amount in r6.
// Tried against 142: in the drain, the amount in tmp 156, size 163,
// count 169, power 315, n 393, hp0 569; kind = dealt halved in place
// 235; the halving before dmg = actor->hp 265; kind halved and apwr the
// amount 176; a FAKEMATCH register variable in r7 for the halving 170
// (kind then joins it in r7 and the copy goes), and r6 for the amount
// as well 168. In the heal, case 76 reading maxHp through stat_ptr
// (stat_ptr[-1], or stat_ptr-- first) 153; maxu read before the signed
// read in 76 176, in 71 too 183 (one ldrh shared again); case 70's
// halving through maxv = (s16)maxu 168, (s16)target->maxHp / 2 no
// change. Where the listing reloads target from r8 for an ldrsh at 52
// or 54 (cases 76, 71, 78 and the 7%), it takes the base into the
// first register and the offset into the second (r2/r3, r4/r0, r2/r4,
// r0/r1); ours takes them the other way round (r3/r2, r0/r4, r4/r2,
// r1/r0). Read as reload's round-robin from r0 up, skipping the
// ldrsh's own output, the registers are the same in both and only the
// order differs: the listing reloads the base before the offset
// scratch, ours the scratch first. Where the base is inherited from an
// ldrh before it (the 60/30 tails, 77) both agree. An s16 local or a
// byte-offset read for case 76's maxHp changes nothing.
// The PP restore head matches exactly when it walks a pointer as the
// heal does (u16 *stat_ptr; stat_ptr = (u16 *)&target->pp;
// old = *stat_ptr--; heal = *(s16 *)(stat_ptr + 1);, tagged FAKEMATCH
// as the heal's): ldrsh, the effect, then ldrh, and the later
// (s16)old gets its lsls/asrs pair. It scores 145 (draft 3451, common
// 3378) only because our 60/30 tail is still 2 bytes longer, which now
// moves the branch and literal offsets after it; apply it with the
// tail's fix. heal read first (heal = *(s16 *)stat_ptr--; old =
// stat_ptr[1];) 150. On top of it, for the tail: maxv *= 60 or 30 then
// heal += maxv / 100 191, else if (... == ABILITY_EFFECT_HEAL_HP_30) 149. There
// sched2 puts our movs r1, #100 before each branch's last shift, so
// the two tails differ and jump2 shares only the call; the listing
// keeps it last in both and shares it. Alchemy permute over the heal
// (PERM_RANDOMIZE from stat_ptr to the clamp, 11 minutes, about 6,000
// candidates) found nothing below 142.
// The 60/30 tail matches with the product in a local p of each branch
// and an empty asm("") barrier between it and the division, tagged
// FAKEMATCH, 139 (draft 3449, common 3380): sched2 then keeps
// movs r1, #100 after the product's last shift and jump2 shares it with
// the call, as the listing does. Without the barrier, the multiplier as
// a ?: (fold distributes it), or a non-volatile asm "0" tie on p, it
// stays 142; the product divided once after the branches, in a
// block-local p, shares the tail but takes maxv into r2 and the shift
// into r3, and heal and maxu swap r5 and r6 (195); in maxv 208.
// With it, the PP restore head's pointer walk (above), 118 (draft 3450,
// common 3391): the head and the later (s16)old match.
// Case 70 reading maxHp into a u16 local m (maxu = m; heal += (s16)m
// / 2;), 114 (draft 3452, common 3394): with (s16)maxu, gcse shares
// maxu << 16 between case 70 and the clamp, so ours computes it before
// the join in r1, where the listing computes it at the join in r3; m
// keeps the two apart and the join matches, but its load becomes ldrsh
// with maxu rebuilt by lsrs (2 instructions longer). The same with m
// as u16 read through a u16 pointer 114; maxu read first and m = maxu
// 131; maxu = m after the halving 118; m as s16, read first 118 or
// after maxu 118; m as s32 137; (s16)target->maxHp / 2 read before
// maxu 125 (two ldrh, but the rest of 70 and the join match).
// An asm "+r" barrier on maxu after case 70's halving instead, tagged
// FAKEMATCH, 100 (draft 3450, common 3400): case 70 and the join match
// with the listing's single ldrh. A plain asm("") after the halving
// keeps gcse's sharing (118).
// Tried against 100: case 76 reading maxHp and maxu through stat_ptr
// (stat_ptr--; m = *(s16 *)stat_ptr; maxu = *stat_ptr;) 119; a local
// copy of target for case 76's reads no change (copy-propagated); in
// the drain, kind = dealt then kind = dealt / 2 in the if, 193.
// The two far branches back (-6086, -7822) were alchemy diff misreading
// a b.n before a relocated literal word; with diff fixed they match,
// 96 (common 3402), with no source change.
// The heal's reloads (76, 71, 78 and the 7%): the listing's RTL reads
// maxu (ldrh) before the signed read and sched2 then moves the ldrsh
// above it, so reload took the base first. maxu read first with an
// asm("" ::: "memory") barrier before the signed read, tagged FAKEMATCH,
// gives the listing's registers in all four, 72 (common 3414): 76 alone
// 90, with 71 82, with 78 76. The barrier also stops sched2, so each
// ldrh stays before its ldrsh (10 lines). Without a barrier maxu first
// shares one load (176); m read volatile 94 (ldrh plus shifts);
// asm("") 98; an asm "+r" tie on maxu after both reads 93; maxu read
// through stat_ptr, or m through it, 104; m through a byte offset 167;
// maxu = *stat_ptr-- with m through stat_ptr + 1, 86.
// In the drain against 96: apwr = dealt, then apwr = dealt / 2 in the
// if and kind = apwr, 333 (apwr and kind share r6, and cse reads apwr
// for dealt); apwr /= 2 in place 333; the halving in an if/else on apwr
// 403, or on hp0 412. apwr = dealt with an asm "+r" tie on apwr before
// the test 275, and another before kind = apwr 273 (draft 3449): apwr
// then loads before the test as in the listing, but in r6 shared with
// kind, and the halving reloads dealt from the stack (ldr r0) where the
// listing copies it from r7, since the tie ends apwr's equivalence with
// dealt's slot. apwr = dealt, the halving into apwr, kind = apwr and
// dmg += apwr (so apwr outlives the copy) 307.
// Against 72: in the drain, a block-local amt (amt = dealt; if (...)
// amt = dealt / 2, or amt = amt / 2; kind = amt;) 104: cse reads amt
// for dealt and amt takes r1, not r7; kind = dealt / 2 in an if with
// kind = dealt in its else 239. In ☀️'s heal (games/), field reads in
// place of its stat_ptr walk, old = (u16)target->hp or *(u16 *)&target->hp
// and the same for maxu, load each stat once (ldrh, then lsls/asrs for
// the signed copy), since old is used only as (s16)old, the value heal
// already holds; the walk stays. An empty asm("") after bonus = 0, tagged
// FAKEMATCH, keeps movs r3, #192 for Ram_HeapSlots below the stores of
// plan, slot and bonus, as the listing has it, 70 (common 3415). The
// same kind of barrier would place most of the remaining swaps, one
// each, but they share one pattern the compiler should explain instead:
// where ours issues the head of a constant or address chain (movs #192,
// #128, #160, #187, adds #28, lsls #2) first, the listing issues a
// neighbouring load, mostly a reload from the stack (ldr [sp, #64]),
// first and the chain just before its use. Under haifa's priorities that
// means the listing's loads rank higher than a load latency of 2 gives
// them (at the summon's 0x6b/0x64 pair, 4 or more with ties by luid),
// the open K2 load model.
// Left at 70: the drain's halving (above: the listing loads dealt into
// r7 before the test, copies it to r0 for the halving and copies r7 to
// r6 for the amount, as if the halving read a spilled dealt whose reload
// inherits r7); the four ldrh the barrier holds before their ldrsh; and
// about 20 single loads swapped with a neighbour (K2's load order).
// Load models tried as studies, each a cc1 rebuilt from agscc (the
// unchanged source rebuilds the admitted cc1 byte for byte), scored here
// and over the 713 C files tla-en links, compiled with ⚓️'s options and
// compared as assembly with the admitted cc1 (library files included, so
// an upper bound): a latency of 3 for halfword and byte loads only 188,
// 83 files change (LinkLobby_WriteSlotValue issues an lsl before its
// ldrb, which that latency forbids); a load preferred to any equal insn
// as rank_for_schedule's last tie-break after reload 76, 43 files; a load
// preferred only to an equal constant step (movs #n, or lsls/adds #n into
// the same register) 70 as the last tie-break (24 files) and 80 as the
// first after priority (50 files). Session 2's latency of 3 for every
// load gave 273. None explains these swaps without changing credited
// code.
// Why the walk and the memory barriers are needed: on Thumb, extendhisi2
// loads a signed read through an HImode ldrh, and cse.c's LOAD_EXTEND_OP
// reuse then turns it into a subreg of an earlier zero-extended
// read of the same field, so an unsigned read first always shares its
// load. Reload's round-robin registers in both games say the ROM's RTL
// had the unsigned read first, yet kept two loads. Studied with a cc1
// that has that reuse removed: plain reads (old = (u16)target->hp;
// heal = target->hp; maxu = (u16)target->maxHp; maxv =
// target->maxHp; the same for pp) and none of the two walks or four
// memory barriers score 60 (common 3420; the heal and PP restore match
// outright), against 391 with the admitted cc1. But that cc1 breaks ☀️
// English from System_Initialize on (2,406 linked sections differ), so
// the reuse itself stays; what keeps it from firing here is still open.
// In ☀️'s heal with the admitted cc1, plain reads with each signed read
// first keep both loads (heal, old, maxv, maxu: 7 instructions differ;
// maxv, heal, old, maxu: 5), but reload then gives old and maxHp's
// offset r0 and r4 where the ROM has r4 and r0, the order of an
// unsigned-first RTL.
// 2026-10-08, against 70 (trial scores below from a plain instruction
// diff, which reads one above alchemy diff here). A search that put
// one asm("") after each statement in turn, kept the best and searched
// again found three:
// after hit = 0 before the unit scan (4), after the 7% branch's maxv
// read (2), after work->unk858 = tid (2). By hand: a barrier
// between cur = target->hp and round = 1 (2), and both enemyId 369/373
// tests read into a block-local s32 e with an asm volatile "+r" tie (2
// each; with cur = 0 moved after the tie in case 5, otherwise movs r6,
// #0 lands before the ldrh), 56 (common 3422). The tie as u16 117, in
// tmp, value, size, cnt or kind 161 to 681; an asm input of
// target->enemyId instead 65. No further single asm(""), memory
// barrier, block-start barrier or "+r" tie after any assignment helps.
// The rest are not reachable by a barrier: the ROM's loads (mostly
// reloads of work from [sp, #64], also action, the ldrh/ldrsh of enemyId,
// maxHp and level, the pool load at 166 << 1 + 255) sit inside the
// statement whose constant chain ours issues first. At the summon's
// queueCount the ROM's ldr has priority 6 against the chain's 7 in
// ours (sched2 dump), so it is not a tie but a cost the admitted model
// does not give; at the 10000 clamp it is a tie (ldrsh and lsls both 3)
// the ROM breaks for the load. Tried there: a block-local copy of work
// tied with asm (157, the copy takes r3 and the next read reloads); a
// register variable in r4 for it with an asm("") barrier, 109 used for
// two reads, 65 for one (combine folds the copy back), 99 with a "+r"
// tie; a tied copy of action for ap 258. Drain: x as a register
// variable in r7 (x = dealt; x = dealt / 2 or x /= 2; kind = x) 83,
// the halving becomes a lone asrs. Studies, with a cc1 rebuilt from
// agscc in scratch and scored here only: the split constant kept as one
// insn through sched2 and printed as three (its define_split off, its
// length 4 or 6) 59 against 71 on the same scorer, but the ROM
// interleaves split pieces with other insns (movs r4, #160; ldr r0;
// lsls r4 ...), so the split does happen before sched2 and the idea is
// refuted; one or two cycles more for every sp-based load 131, 159.
// 2026-10-08, the HP drain, 48 (common 3426): ap = dealt before
// dmg = actor->hp, ap /= 2 in the 60 branch, kind = ap. The listing's
// ldr r7 is ap (r7 everywhere), loaded from dealt's slot before the
// test; cse keeps dealt, which lives longer, in the halving, and reload
// takes spilled dealt from r7 instead of its slot (adds r0, r7, #0);
// kind takes the result in r6. ap = dealt / 2 in the branch scores the
// same; ap read after dmg = actor->hp 291
// (ap and kind then share r6, and ap leaves r7 everywhere), and every
// other function-level variable for the halving 84 to 1699 (spilled or
// in r1). Left there: the ldr r7 issues before the hp copy (adds r2,
// r5, #0) where the listing issues it after, a sched2 tie (both
// priority 1) broken by insn order, which needs ap read after hp.
// Score in every edition: 48 in tla-en; 50 in ja, de, es, fr and it,
// whose 2 extra rows are the diff decoding the via-lr BL half and the
// movs after it as one strb.w, not a difference; so all six editions
// share this source as it stands. With the drain: no single asm("")
// after any statement improves on 48.
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_party.h"
#include "iwram_call.h"
#include "battle_obj.h"
#include "battle_calc.h"
#include "battle_effect.h"
#include "battle_event.h"
#include "battle_message.h"
#include "battle_system.h"
#include "battle_summon.h"
#include "battle_type.h"
#include "battle_work.h"
#include "motobj.h"
#include "mem.h"
#include "battle_display.h"
#include "party_state.h"
#include "ram_buffer.h"

#define ALLY 2 /* Ally. */

#define STEP_LIM(v)                                                            \
  {                                                                            \
    if ((v) < -4)                                                              \
      (v) = -4;                                                                \
    if ((v) > 4)                                                               \
      (v) = 4;                                                                 \
  }

#define ALLY_CHECK(id) ((id) >= 0 && (id) < 8)

#define ELEMENT_BONUS()                                                        \
  {                                                                            \
    if (element != 4)                                                          \
      bonus = power - target->element[element].resistance;                     \
  }

#define MESSAGE_PUSH(m) BattleEvPush(BATTLE_EVENT_MESSAGE, (u32)(m))

#define MESSAGE_SIDE(m, e)                                                     \
  {                                                                            \
    if (ALLY_CHECK(tid))                                                       \
      MESSAGE_PUSH(m);                                                         \
    else                                                                       \
      MESSAGE_PUSH(e);                                                         \
  }

#define GUARD_CUT()                                                            \
  {                                                                            \
    guard = target->guardLevel;                                                \
    if (guard != 0) {                                                          \
      if (guard == 1)                                                          \
        dmg /= 2;                                                              \
      else if (guard == 2)                                                     \
        dmg = dmg * 2 / 5;                                                     \
      else                                                                     \
        dmg /= 10;                                                             \
    }                                                                          \
  }

#define RES_STEP(d, v, message)                                                \
  {                                                                            \
    target->resistanceStage += (d);                                            \
    STEP_LIM(target->resistanceStage);                                         \
    BattleEvPush(BATTLE_EVENT_NUMBER, (v));                                    \
    MESSAGE_PUSH(message);                                                     \
    target->resistanceTurns = 7;                                               \
  }

#define STAT_SET(f, message)                                                   \
  {                                                                            \
    MESSAGE_PUSH(message);                                                     \
    (f) = 7;                                                                   \
  }

s32 BattleTargetExec(struct BattlePlan *plan, s32 slot) {
  struct BattleAct *action;
  s32 abilityId;
  struct BattleUnit *actor;
  s32 aid;
  s32 element;
  s32 bonus;
  struct BattleWork *work;
  s32 dist;
  s32 half;
  s32 adj;
  s32 dealt;
  s32 crush;
  s32 hit;
  s32 crit;
  s32 skip;
  s32 type;
  s32 aff;
  struct BattleUnit *bak;
  s32 power;
  s32 hp;
  s32 kind;
  s32 scale;
  s32 pass;
  s16 *cmd;
  u8 tid;
  s8 *rm;
  struct BattleUnit *target;
  u32 n;
  s32 value;
  s32 dmg;
  s32 ap;
  s8 *am;
  s32 guard;
  s16 list[8];
  s32 cnt;
  s32 tmp;
  s32 size;
  bonus = 0;
  /* FAKEMATCH: barrier; sched2 otherwise hoists movs r3, #192 for Ram_HeapSlots
   * above the stores of plan, slot and bonus, 72 without, 70 with. */
  asm("");
  work = Ram_HeapSlots->battleWork;
  half = 0;
  dealt = 0;
  crush = 0;
  skip = 0;
  aff = 0;
  size = sizeof(struct BattleUnit);
  bak = MemAlloc(size);

  if (abilityId == 312 || abilityId == 316)
    aid = plan->actorId2;
  else
    aid = plan->actorId;
  tid = plan->targetIds[slot];
  abilityId = plan->abilityId;
  element = plan->element;
  adj = plan->targetMultiplier[slot];
  crit = plan->targetCritical[slot];

  action = BattleActGet(abilityId);
  actor = UnitGet(aid);
  target = UnitGet(tid);
  Iwram_CopyWords(bak, target, size);
  switch (target->enemyId) {
  case 221:
    if (plan->command == 1 && (u32)(aid ^ tid) >> 7 != 0) {
      BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgDissipated);
      goto done;
    }
    break;
  case 101:
  case 102:
  case 103:
    if (abilityId != 573 && work->unk868 != 0 && (u32)(aid ^ tid) >> 7 != 0) {
      BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgFieldProtects);
      goto done;
    }
    break;
  }
  if (target->guardLevel == 4 && (u32)(aid ^ tid) >> 7 != 0) {
    BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, tid);
    BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
    MESSAGE_PUSH(MsgNoEffect);
    goto done;
  }

  if (action->range != 255) {
    dist = plan->targetOffset[slot];
    if (dist < 0)
      dist = -dist;
  } else {
    dist = 0;
  }

  if (element != 4) {
    s32 i;

    value = target->baseElement[element].resistance;
    for (i = 0; i < 4 && value >= target->baseElement[i].resistance; i++)
      ;
    if (i == 4)
      aff = -1;

    for (i = 0; i < 4 && value <= target->baseElement[i].resistance; i++)
      ;
    if (i == 4)
      aff = 1;
  }

  if (plan->element >= 0 && plan->element <= 3) {
    cmd = &plan->command;
    if (*cmd != 2) {
      power = actor->element[plan->element].power;
      goto power_end;
    }
  } else
    cmd = &plan->command;
  power = 100;
power_end:

  if (plan->command == 5 && (plan->element >= 0 && plan->element <= 3) &&
      aff > 0) {
    s32 chance;

    chance = power - target->element[plan->element].resistance;
    chance += 30;
    chance *= 65536 / 100;
    if (chance > (BattleRandomFar() & 0xffff))
      BattleEvPush(BATTLE_EVENT_DJINN_BONUS, 5);
  }

  if (abilityId == 686 || abilityId == 357)
    Battle_ApplyActionExtras(tid, 0, 0);
  if (abilityId == 720 || abilityId == 721)
    Battle_ApplyActionExtras(tid, 1, 0);
  if (abilityId == 728)
    Battle_ApplyActionExtras(tid, 1, 1);
  type = action->flags & 15;
  {
    s32 res;

    res = plan->targetHit[slot];
    if (res == -1)
      hit = BattleHitCheck(aid, tid, element, action->effect, gHitTable[dist]);
    else
      hit = res;
  }
  if (action->effect == 50 || action->effect == 51 || action->effect == 86 ||
      action->effect == 87) {
    s32 cls;
    s32 ent;
    s32 chg;

    cls = actor->enemyId;
    chg = -1;
    ent = EnemySlotGet();
    if (action->effect == 51) {
      cls = BattleGroupRandom(work->enemyGroup);
    } else if (action->effect == 86) {
      if (actor->enemyId == 164)
        cls = (BattleRandomFar() & 3) + 378;
      else
        cls = 81;
    } else if (action->effect == 87) {
      if (work->queueCount != 0) {
        cls = work->queuedSummons[work->queueHead];
        chg = work->queuedCharges[work->queueHead];
      } else
        hit = 0;
    }
    if (hit != 0 && EnemyCheck(cls) != 0 && ent >= 0) {
      if (chg == -1) {
        chg = EnemyCountDecrement(cls, 1);
        if (chg & 0x8000)
          EnemyCountClear(cls);
      }
      BattleUnitSetFar(ent, cls, chg & 0x7fff);
      if (action->effect == 87) {
        work->queueCount--;
        work->queueHead = (work->queueHead + 1) % 2;
      }
      if (actor->enemyId == 164)
        BattleParty_InsertUnitCentered(work->enemyIds, ent);
      else {
        s32 i;

        for (i = 0; i < 6; i++) {
          if (work->enemyIds[i] == 254) {
            work->enemyIds[i] = ent;
            break;
          }
          if (work->enemyIds[i] == 255) {
            work->enemyIds[i] = ent;
            work->enemyIds[i + 1] = 255;
            break;
          }
        }
      }
      EnemyUpdate();
      {
        struct BattleObj *obj;

        obj = BattleObjGet(ent);
        BattleObjMake(obj, ent, obj->anchorX / 0x10000, obj->anchorZ / 0x10000);
      }
      BattleObjPut();
      {
        s32 num;

        num = BattleEnemyList(list);
        for (cnt = 0; cnt < num; cnt++)
          ObjMotRst((u16)list[cnt]);
      }
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, ent);
      if (action->effect == 87)
        MESSAGE_PUSH(MsgUpToFight);
      else if (abilityId != 503)
        MESSAGE_PUSH(MsgAppears);
      else
        MESSAGE_PUSH(MsgSplitOff);
    } else if (abilityId == 503) {
      MESSAGE_PUSH(MsgSplitFailed);
    } else {
      MESSAGE_PUSH(MsgNoOneCame);
    }
  }

  if (hit != 0) {
    if (action->effect == 53 || action->effect == 83) {
      hit = 0;
      /* FAKEMATCH: barrier; sched2 otherwise issues movs r3, #187 for the
       * scan's first address before the store of hit and the reload of work,
       * where the ROM stores and reloads first, 4 instructions. */
      asm("");
      for (n = 0; n < 20; n++) {
        if (work->actions[n].unitId == tid) {
          hit = 1;
          break;
        }
      }
    } else if (action->effect == 35) {
      half = 1;
    } else if (action->effect == 34) {
      crush = 1;
    } else if (action->effect == 27) {
      skip = 1;
    } else if (action->effect == 55) {
      if (actor->hp != 0)
        BattleEvPush(BATTLE_EVENT_REMOVE_UNIT, aid);
    } else if (action->effect == 32) {
      if (target->pp != 0)
        type = 10;
      else
        hit = 0;
    } else if (action->effect == 90) {
      half = 2;
    } else if (action->effect == 91) {
      half = 2;
    }
  }

  if ((action->effect != 75 || slot == 0) && skip == 0 &&
      (target->hp != 0 || Battle_IsReviveEffectFar(action->effect) != 0)) {
    s32 pp;
    s32 cur;

    switch (type) {
    case 3:
    case 4: {
      s32 def;

      if (target->guardLevel == 4)
        break;
      hp = target->hp;
      def = target->def;
      scale = def;
      if (half != 0) {
        if (half == 2)
          scale = 0;
        else
          scale = def / 2;
      }
      pass = 1;
      do {
        ELEMENT_BONUS();
        if (pass == 0)
          bonus = 0;
        ap = action->power;
        if (type == 4)
          dmg = BattleAtkCalc(actor->atk, scale, 0, bonus) * ap / 10;
        else {
          s32 atk;

          atk = actor->atk;
          if (abilityId == 312 || abilityId == 316) {
            atk = ((struct BattleUnit *)UnitGet(plan->actorId))->atk;
            if (abilityId == 312)
              atk += ((struct BattleUnit *)UnitGet(plan->actorId2))->atk / 4;
          }
          dmg = BattleAtkCalc(atk, scale, ap, bonus);
        }
        dmg *= adj;
        if (crit != 0) {
          if (crit == 1)
            dmg = dmg * 5 / 4;
          else
            dmg = dmg * 3 / 2;
          dmg += (target->level / 5) + 6;
          if (pass == 0) {
            BattleEvPush(BATTLE_EVENT_MARK, 0);
            {
              s32 message;

              message = (s32)MsgCritical;
              if (ALLY_CHECK(tid))
                message += 1;
              BattleEvPush(BATTLE_EVENT_MESSAGE_NO_WAIT, message);
            }
          }
        }
        dmg += BattleRandomFar() & 3;
        GUARD_CUT();
        if (dmg <= 0)
          dmg = 1;
        if (crush != 0) {
          if (dmg < hp - 1) {
            dmg = hp - 1;
            if (dmg <= 0)
              dmg = 1;
          }
        }
        if (GameFlag_Test(366) != 0 && plan->command == 5 && hp <= dmg) {
          dmg = hp - 1;
        }
        pass++;
      } while (pass <= 1);
      hp -= dmg;
      BattleEvPush(BATTLE_EVENT_HIT, tid);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, dmg);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgEmphP + aff;
        else
          message = (s32)MsgDmgEmphE + aff;
        BattleEvPush(BATTLE_EVENT_MESSAGE, message);
      }

      if (hp <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
        hp = 1;
      if (hp <= 0) {
        hp = 0;
        BattleEvPush(BATTLE_EVENT_DEFEAT, tid);
        {
          s32 e;

          e = target->enemyId;
          /* FAKEMATCH: tie; sched2 otherwise issues movs #114 for 369 before
           * the ldrh of enemyId, where the ROM loads first, 2 instructions. */
          asm volatile("" : "+r"(e));
          if (e != 369 && e != 373) {
            BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
            MESSAGE_SIDE(MsgGoesDown, MsgFelled);
          }
        }
      } else
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->hp - hp;
      target->hp = hp;
      UnitRatioFar(tid);
      break;
    }

    case 10: {
      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      pp = target->pp;
      ELEMENT_BONUS();
      ap = action->power;
      dmg = BattlePowerCalc(ap, bonus, 256);
      dmg = dmg * gPpLossTable[dist] / 100;
      dmg *= adj;
      GUARD_CUT();
      if (action->effect == 32 && dmg > pp)
        dmg = pp;
      if (dmg > pp)
        dmg = pp;

      BattleEvPush(BATTLE_EVENT_HIT, tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, dmg);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgPpLossP;
        else
          message = (s32)MsgPpLossE;
        BattleEvPush(BATTLE_EVENT_MESSAGE, message);
        pp -= dmg;
      }
      if (pp <= 0)
        pp = 0;
      BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->pp - pp;
      target->pp = pp;
      UnitRatioFar(tid);
      break;
    }

    case 1: {
      if (action->power == 0)
        break;
      cur = target->hp;
      if (cur == 0)
        break;

      ap = action->power;
      dmg = BattleRecoveryCalc(ap, element == 4 ? 100 : power, 256);
      dmg = dmg * gHpRecoveryTable[dist] / 100;
      dmg *= adj;
      dmg += BattleRandomFar() & 3;
      cur += dmg;
      if (cur > target->maxHp) {
        cur = target->maxHp;
        dmg = cur - target->hp;
      }
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      if (cur == target->maxHp)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, dmg);
        MESSAGE_PUSH(MsgHpRecover);
      }
      dealt = target->hp - cur;
      target->hp = cur;
      UnitRatioFar(tid);
      break;
    }

    case -1: {
      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      pp = target->pp;
      ELEMENT_BONUS();
      ap = action->power;
      dmg = BattlePowerCalc(ap, bonus, 256);
      dmg = dmg * gPpDmgTable[dist] / 100;
      dmg *= adj;
      GUARD_CUT();
      BattleEvPush(BATTLE_EVENT_HIT, tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, dmg);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgP;
        else
          message = (s32)MsgDmgE;
        BattleEvPush(BATTLE_EVENT_MESSAGE, message);
        pp -= dmg;
      }
      if (pp <= 0)
        pp = 0;
      BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, tid);
      target->pp = pp;
      UnitRatioFar(tid);
      break;
    }

    case 5:
    case 6:
    case 8: {
      s32 round;

      if (target->guardLevel == 4)
        break;
      if (action->power == 0)
        break;
      cur = target->hp;
      /* FAKEMATCH: barrier; sched2 otherwise issues movs r1, #1 for round
       * before the ldrsh of hp, where the ROM loads first, 2 instructions. */
      asm("");
      round = 1;
      do {
        ELEMENT_BONUS();
        if (round == 0)
          bonus = 0;
        ap = action->power;
        if (abilityId == 683 || abilityId == 673 || abilityId == 724 ||
            plan->command == 6 || plan->command == 10) {
          s32 rate;

          rate = 0;
          switch (abilityId) {
          default:
          case 393:
            break;
          case 406:
          case 388:
          case 396:
          case 380:
            rate = 3;
            break;
          case 409:
          case 399:
          case 391:
          case 383:
            rate = 12;
            break;
          case 401:
            rate = 12;
            break;
          case 683:
            rate = 35;
            break;
          case 407:
          case 400:
          case 397:
          case 389:
          case 384:
          case 381:
            rate = 6;
            break;
          case 398:
          case 390:
          case 408:
          case 382:
            rate = 9;
            break;
          case 392:
          case 410:
            rate = 9;
            break;
          case 402:
            rate = 7;
            break;
          case 411:
          case 403:
          case 385:
            rate = 15;
            break;
          case 394:
            rate = 21;
            break;
          case 412:
            rate = 24;
            break;
          case 673:
          case 386:
            rate = 30;
            break;
          case 724:
          case 404:
            rate = 40;
            break;
          }
          value = target->maxHp;
          if (value > 10000)
            value = 10000;
          ap += value * rate / 100;
        }
        dmg = BattlePowerCalc(ap, bonus, 256);
        dmg *= adj;
        switch (type & 15) {
        case 5:
          dmg = gHpDmgTable5[dist] * dmg / 100;
          break;
        case 8:
          dmg = dmg * gHpDmgTable8[dist] / 100;
          break;
        case 6:
          dmg = dmg * gHpDmgTable6[dist] / 100;
          break;
        }
        dmg += BattleRandomFar() & 3;
        GUARD_CUT();
        if ((gPartyState.battleRule24b == 6 ||
             (GameFlag_Test(366) != 0 && plan->command == 6)) &&
            cur > dmg) {
          dmg = cur;
        }
        round++;
      } while (round <= 1);
      BattleEvPush(BATTLE_EVENT_HIT, tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, dmg);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      {
        s32 message;

        if (ALLY_CHECK(tid))
          message = (s32)MsgDmgEmphP + aff;
        else
          message = (s32)MsgDmgEmphE + aff;
        BattleEvPush(BATTLE_EVENT_MESSAGE, message);
        cur -= dmg;
      }

      if (cur <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
        cur = 1;
      if (gPartyState.battleRule24b == 6)
        cur = 0;
      if (cur <= 0) {
        BattleEvPush(BATTLE_EVENT_DEFEAT, tid);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
        {
          s32 message;

          cur = 0;
          if (ALLY_CHECK(tid))
            message = (s32)MsgGoesDown;
          else
            message = (s32)MsgFelled;
          BattleEvPush(BATTLE_EVENT_MESSAGE, message);
        }
      } else
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, tid);
      dealt = target->hp - cur;
      target->hp = cur;
      UnitRatioFar(tid);
      break;
    }

    case 11: {
      if (action->power == 0)
        break;
      pp = target->pp;
      ap = action->power;
      dmg = BattleRecoveryCalc(ap, element == 4 ? 100 : power, 256);
      dmg = dmg * gPpRecoveryTable[dist] / 100;
      dmg *= adj;
      pp += dmg;
      if (pp > target->maxPp) {
        pp = target->maxPp;
        dmg = pp - target->pp;
      }
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      if (pp == target->maxPp)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, dmg);
        MESSAGE_PUSH(MsgPpRecover);
      }
      target->pp = pp;
      UnitRatioFar(tid);
      break;
    }

    case 0:
    case 7:
    case 9:
      break;

    case 2:
      if (target->guardLevel == 4)
        break;
      if (hit != 0) {
        if (action->power == 0)
          break;
        cur = target->hp;
        ELEMENT_BONUS();
        ap = action->power;
        dmg = BattlePowerCalc(ap, bonus, 256);
        dmg *= adj;
        dmg = dmg * gHpDmgTable[dist] / 100;
        GUARD_CUT();
        BattleEvPush(BATTLE_EVENT_HIT, tid);
        BattleEvPush(BATTLE_EVENT_NUMBER, dmg);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
        {
          s32 message;

          if (ALLY_CHECK(tid))
            message = (s32)MsgDmgP;
          else
            message = (s32)MsgDmgE;
          BattleEvPush(BATTLE_EVENT_MESSAGE, message);
          cur -= dmg;
        }

        if (cur <= 0 && BattleUnit_KeepsOneHp(tid) != 0)
          cur = 1;
        if (cur <= 0) {
          BattleEvPush(BATTLE_EVENT_DEFEAT, tid);
          {
            s32 e;

            e = target->enemyId;
            /* FAKEMATCH: tie; as in case 3, and cur = 0 after it so movs r6, #0
             * follows the 369 as in the ROM, 2 instructions. */
            asm volatile("" : "+r"(e));
            cur = 0;
            if (e != 369 && e != 373) {
              BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
              MESSAGE_SIDE(MsgGoesDown, MsgFelled);
            }
          }
        } else
          BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, tid);
        dealt = target->hp - cur;
        target->hp = cur;
        UnitRatioFar(tid);
        break;
      }
      BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, tid);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgNoEffect);
      break;
    }
  }

  if (work->unk868 != 0 && abilityId == 573) {
    BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
    MESSAGE_PUSH(MsgFieldBroken);
    work->unk868 = 0;
  }
  BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
  if ((action->effect != 75 || slot != 0) &&
      (Battle_IsReviveEffectFar(action->effect) != 0 || target->hp != 0 ||
       BattleEffectDeadCheck(action->effect) != 0) &&
      hit != 0) {
    switch (action->effect) {

    case 64:
      if (target->delusion != 0) {
        target->delusion = 0;
        MESSAGE_PUSH(MsgCureDelusion);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureStun);
      }
      if (target->sleep != 0) {
        target->sleep = 0;
        MESSAGE_PUSH(MsgWakes);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->psySeal != 0) {
        target->psySeal = 0;
        MESSAGE_PUSH(MsgCureSeal);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->grimReaper != 0) {
        target->grimReaper = 0;
        MESSAGE_PUSH(MsgCureCurse);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->haunt != 0) {
        target->haunt = 0;
        MESSAGE_PUSH(MsgCureSpirit);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      }
      if (target->poison != 0) {
        MESSAGE_PUSH(MsgCurePoison);
        target->poison = 0;
      }
      BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      break;

    case 4:
      if (target->delusion != 0) {
        target->delusion = 0;
        BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureDelusion);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureStun);
      }
      if (target->sleep != 0) {
        target->sleep = 0;
        BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgWakes);
      }
      if (target->psySeal != 0) {
        target->psySeal = 0;
        BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgCureSeal);
      }
      if (target->grimReaper == 0)
        break;
      target->grimReaper = 0;
      BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
      MESSAGE_PUSH(MsgCureCurse);
      break;

    case 61:
    case 62:
    case 70:
    case 71:
    case 76: {
      s32 old;
      s32 maxu;
      s32 maxv;
      s32 heal;
      u16 *pt;

      /* FAKEMATCH: walking a pointer down from hp to maxHp reads each stat
       * twice, unsigned and signed, as the ROM does; plain field reads share
       * one load. */
      pt = (u16 *)&target->hp;
      old = *pt--;
      heal = *(s16 *)(pt + 1);
      if (action->effect == 76) {
        s32 m;

        maxu = (u16)target->maxHp;
        /* FAKEMATCH: barrier; with maxu read first it keeps cse from sharing
         * one load between the two reads, so reload takes the base before the
         * offset as the ROM does (96 to 90); sched2 then keeps the ldrh first,
         * where the ROM moves it after the ldrsh. */
        asm("" ::: "memory");
        m = target->maxHp;
        heal += m * 2 / 5;
      } else if (action->effect == 71) {
        maxu = (u16)target->maxHp;
        /* FAKEMATCH: barrier; the same for case 71 (90 to 82). */
        asm("" ::: "memory");
        maxv = target->maxHp;
        heal += maxv * 7 / 10;
      } else if (action->effect == 70) {
        maxu = (u16)target->maxHp;
        heal += (s16)maxu / 2;
        /* FAKEMATCH: barrier; without it gcse shares maxu << 16 between this
         * halving and the clamp, so ours shifts before the join in r1 where the
         * ROM shifts at the join in r3, 118 without, 100 with. */
        asm("" : "+r"(maxu));
      } else {
        pt--;
        maxu = *pt--;
        maxv = *(s16 *)(pt + 1);
        if (action->effect == 61) {
          s32 p = maxv * 60;
          /* FAKEMATCH: barrier; sched2 otherwise puts movs r1, #100 before the
           * product's last shift and jump2 cannot share it with the division,
           * 142 without, 139 with. */
          asm("");
          heal += p / 100;
        } else {
          s32 p = maxv * 30;
          /* FAKEMATCH: barrier; the same for the 30% branch, whose tail jump2
           * shares with the 60% one. */
          asm("");
          heal += p / 100;
        }
      }
      if (heal > (s16)maxu)
        heal = (s16)maxu;
      tmp = heal - (s16)old;
      if (tmp == 0 && type != 1)
        break;
      if (heal == (s16)maxu)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, tmp);
        MESSAGE_PUSH(MsgHpRecover);
      }
      target->hp = heal;
      UnitRatioFar(tid);
      break;
    }

    case 63:
    case 77:
    case 78: {
      s32 old;
      s32 maxv;
      s32 heal;
      s32 maxu;
      u16 *pt;

      /* FAKEMATCH: walking a pointer down from pp reads it twice, signed and
       * unsigned, as the ROM does; plain field reads share one load. */
      pt = (u16 *)&target->pp;
      old = *pt--;
      heal = *(s16 *)(pt + 1);
      if (action->effect == 77) {
        maxv = target->maxPp;
        maxu = (u16)target->maxPp;
        heal += (s16)maxv / 10;
      } else if (action->effect == 78) {
        maxu = (u16)target->maxPp;
        /* FAKEMATCH: barrier; the same for case 78 (82 to 76). */
        asm("" ::: "memory");
        maxv = target->maxPp;
        heal += maxv * 3 / 10;
      } else {
        maxu = (u16)target->maxPp;
        /* FAKEMATCH: barrier; the same for the 7% (76 to 72). */
        asm("" ::: "memory");
        maxv = target->maxPp;
        /* FAKEMATCH: barrier; sched2 otherwise issues movs r1, #100 before the
         * 7% product's shifts, where the ROM issues it last, as at 60 and 30, 2
         * instructions. */
        asm("");
        heal += maxv * 7 / 100;
      }
      if (heal > (s16)maxu)
        heal = (s16)maxu;
      tmp = heal - (s16)old;
      if (tmp == 0 && type != 11)
        break;
      if (heal == (s16)maxu)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, tmp);
        MESSAGE_PUSH(MsgPpRecover);
      }
      target->pp = heal;
      UnitRatioFar(tid);
      break;
    }

    case 59:
      target->agilityStage = 8;
      target->agilityTurns = 5;
      UnitCalcFar(tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, target->agi - bak->agi);
      MESSAGE_PUSH(MsgAgiUp);
      break;

    case 58:
      am = &target->agilityStage;
      {
        u8 v;

        v = -4;
        *am = v;
      }
      target->agilityTurns = 5;
      UnitCalcFar(tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, bak->agi - target->agi);
      MESSAGE_PUSH(MsgAgiDown);
      break;

    case 9:
      target->attackStage -= 1;
      STEP_LIM(target->attackStage);
      UnitCalcFar(tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, bak->atk - target->atk);
      MESSAGE_PUSH(MsgAtkDown);
      target->attackTurns = 7;
      break;

    case 8:
      target->attackStage -= 2;
      STEP_LIM(target->attackStage);
      UnitCalcFar(tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, bak->atk - target->atk);
      MESSAGE_PUSH(MsgAtkDown);
      target->attackTurns = 7;
      break;

    case 7:
      target->attackStage += 1;
      STEP_LIM(target->attackStage);
      UnitCalcFar(tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, target->atk - bak->atk);
      MESSAGE_PUSH(MsgAtkUp);
      target->attackTurns = 7;
      break;

    case 6:
      target->attackStage += 2;
      STEP_LIM(target->attackStage);
      UnitCalcFar(tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, target->atk - bak->atk);
      MESSAGE_PUSH(MsgAtkUp);
      target->attackTurns = 7;
      break;

    case 13:
      target->defenseStage -= 1;
      STEP_LIM(target->defenseStage);
      UnitCalcFar(tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, bak->def - target->def);
      MESSAGE_PUSH(MsgDefDown);
      target->defenseTurns = 7;
      break;

    case 12:
      target->defenseStage -= 2;
      STEP_LIM(target->defenseStage);
      UnitCalcFar(tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, bak->def - target->def);
      MESSAGE_PUSH(MsgDefDown);
      target->defenseTurns = 7;
      break;

    case 11:
      target->defenseStage += 1;
      STEP_LIM(target->defenseStage);
      UnitCalcFar(tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, target->def - bak->def);
      MESSAGE_PUSH(MsgDefUp);
      target->defenseTurns = 7;
      break;

    case 10:
      target->defenseStage += 2;
      STEP_LIM(target->defenseStage);
      UnitCalcFar(tid);
      BattleEvPush(BATTLE_EVENT_NUMBER, target->def - bak->def);
      MESSAGE_PUSH(MsgDefUp);
      target->defenseTurns = 7;
      break;

    case 5:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp;
      UnitRatioFar(tid);
      break;

    case 56:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp / 2;
      UnitRatioFar(tid);
      break;

    case 57:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = target->maxHp * 8 / 10;
      UnitRatioFar(tid);
      break;

    case 73:
      if (target->hp != 0)
        break;
      MESSAGE_PUSH(MsgRevived);
      target->hp = (s16)(target->maxHp * 6 / 10);
      UnitRatioFar(tid);
      break;

    case 3:
      if (target->poison != 0)
        MESSAGE_PUSH(MsgCurePoison);
      target->poison = 0;
      break;

    case 17:
      RES_STEP(-1, (bak->resistanceStage - target->resistanceStage) * 20,
               MsgResDown);
      break;

    case 16: {
      target->resistanceStage -= 2;
      if (-4 > target->resistanceStage)
        target->resistanceStage = -4;
      if (target->resistanceStage > 4)
        target->resistanceStage = 4;
      BattleEvPush(BATTLE_EVENT_NUMBER,
                   (bak->resistanceStage - target->resistanceStage) * 20);
      MESSAGE_PUSH(MsgResDown);
      target->resistanceTurns = 7;
    } break;

    case 15:
      RES_STEP(1, (target->resistanceStage - bak->resistanceStage) * 20,
               MsgResUp);
      break;

    case 14: {
      target->resistanceStage += 2;
      if (*(rm = &target->resistanceStage) < -4)
        target->resistanceStage = -4;
      if (target->resistanceStage > 4)
        target->resistanceStage = 4;
      BattleEvPush(BATTLE_EVENT_NUMBER,
                   (target->resistanceStage - bak->resistanceStage) * 20);
      MESSAGE_PUSH(MsgResUp);
      target->resistanceTurns = 7;
    } break;

    case 18:
      if (target->poison != 0)
        break;
      MESSAGE_PUSH(MsgPoisoned);
      target->poison = 1;
      break;

    case 19:
      if (target->poison > 1)
        break;
      MESSAGE_PUSH(MsgVenom);
      target->poison = 2;
      break;

    case 20:
    case 75:
      STAT_SET(target->delusion, MsgDelusion);
      break;

    case 21:
      STAT_SET(target->confusion, MsgConfused);
      break;

    case 22:
      STAT_SET(target->charm, MsgCharmed);
      break;

    case 23:
    case 85:
      STAT_SET(target->stun, MsgStunned);
      break;

    case 24:
      STAT_SET(target->sleep, MsgAsleep);
      break;

    case 25:
      MESSAGE_SIDE(MsgPsyBlock, MsgPsySeal);
      target->psySeal |= 7;
      break;

    case 67:
      MESSAGE_SIDE(MsgPsyBlock, MsgPsySeal);
      target->psySeal |= 16;
      break;

    case 27:
      if (BattleUnit_KeepsOneHp(tid) != 0)
        break;
      BattleEvPush(BATTLE_EVENT_DEFEAT, tid);
      if (target->side == ALLY)
        MESSAGE_PUSH(MsgKoDown);
      else if (abilityId == 219)
        MESSAGE_PUSH(MsgSuffocate);
      else
        MESSAGE_PUSH(MsgSpiritDrain);
      target->hp = 0;
      UnitRatioFar(tid);
      break;

    case 29:
      MESSAGE_PUSH(MsgHpRegen);
      target->refrain = 5;
      break;

    case 30:
      STAT_SET(target->reflect, MsgReflect);
      break;

    case 31:
    case 60:
      ap = dealt;
      dmg = actor->hp;
      if (action->effect == 60)
        ap /= 2;
      kind = ap;
      dmg += kind;
      if (dmg > actor->maxHp) {
        dmg = actor->maxHp;
        kind = dmg - actor->hp;
      }
      BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, aid);
      if (dmg == actor->maxHp)
        MESSAGE_PUSH(MsgHpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, kind);
        MESSAGE_PUSH(MsgHpRecover);
      }
      actor->hp = dmg;
      UnitRatioFar(aid);
      break;

    case 32: {
      s32 heal;
      s32 amt;

      heal = actor->pp;
      amt = dealt;
      heal += amt;
      if (heal > actor->maxPp) {
        heal = actor->maxPp;
        amt = heal - actor->pp;
      }
      BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, aid);
      if (heal == actor->maxPp)
        MESSAGE_PUSH(MsgPpFull);
      else {
        BattleEvPush(BATTLE_EVENT_NUMBER, amt);
        MESSAGE_PUSH(MsgPpRecover);
      }
      actor->pp = heal;
      UnitRatioFar(aid);
      break;
    }

    case 69:
      dmg = dealt / 10;
      if (target->pp < dmg)
        dmg = target->pp;
      if (actor->pp + dmg > actor->maxPp)
        dmg = actor->maxPp - actor->pp;
      if (dmg == 0)
        break;
      BattleEvPush(BATTLE_EVENT_NUMBER, dmg);
      MESSAGE_SIDE(MsgLeechTake, MsgLeechGain);
      Unit_AddPpFar(aid, dmg);
      break;

    case 84:
      dmg = (s16)(target->maxPp / 10);
      if (target->pp < dmg)
        dmg = target->pp;
      if (dmg == 0)
        break;
      BattleEvPush(BATTLE_EVENT_NUMBER, dmg);
      MESSAGE_SIDE(MsgMorePpLossP, MsgMorePpLossE);
      Unit_AddPpFar(tid, -dmg);
      break;

    case 33:
      if (target->attackStage > 0) {
        target->attackStage = 0;
        target->attackTurns = 0;
      }
      if (target->defenseStage > 0) {
        target->defenseStage = 0;
        target->defenseTurns = 0;
      }
      if (target->resistanceStage > 0) {
        target->resistanceStage = 0;
        target->resistanceTurns = 0;
      }
      if (target->agilityStage > 0)
        target->agilityStage = 0;
      target->elementLevels[0] = 0;
      target->elementLevels[1] = 0;
      target->elementLevels[2] = 0;
      target->elementLevels[3] = 0;
      MESSAGE_PUSH(MsgBuffsReset);
      break;

    case 26:
      MESSAGE_PUSH(MsgEvilSpirit);
      target->haunt = 1;
      break;

    case 28:
    case 80:
      if (target->grimReaper == 0) {
        MESSAGE_PUSH(MsgDeathCurse);
        target->grimReaper = 7;
        break;
      }
      if (target->grimReaper <= 1)
        break;
      target->grimReaper -= 1;
      BattleEvPush(BATTLE_EVENT_NUMBER, target->grimReaper);
      MESSAGE_PUSH(MsgDeathCount);
      break;

    case 66:
      MESSAGE_PUSH(MsgReadies);
      target->ready = 2;
      break;

    case 54:
      MESSAGE_PUSH(MsgChallenge);
      target->challenge = 1;
      if (ALLY_CHECK(tid))
        work->challengeFlag |= 2;
      break;

    case 53:
    case 83:

      MESSAGE_PUSH(MsgImmobile);
      target->immobile = 1;
      break;

    case 46:

      MESSAGE_PUSH(MsgAura);
      if (target->guardLevel > 0)
        break;
      target->guardLevel = 1;
      break;

    case 72:
      MESSAGE_PUSH(MsgAura);
      if ((u32)target->guardLevel > 1)
        break;
      target->guardLevel = 2;
      break;

    case 47:
    case 88:
      MESSAGE_PUSH(MsgAura2);
      if (target->guardLevel > 2)
        break;
      target->guardLevel = 3;
      break;

    case 79:
      MESSAGE_PUSH(MsgVanished);
      if ((u32)target->guardLevel > 3)
        break;
      target->guardLevel = 4;
      break;

    case 45:
      BattleEvPush(BATTLE_EVENT_MESSAGE, -1);
      break;

    case 81:
      MESSAGE_PUSH(MsgAllResting);
      work->unk046[1] = 1;
      break;

    case 82:
      MESSAGE_PUSH(MsgBlownAway);
      BattleEvPush(15, tid);
      break;

    case 74:
      MESSAGE_PUSH(MsgCounterReady);
      target->equipBonus[1] = 1;
      break;

    default:
      break;
    }
  }

done:
  BattleEvPush(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
  if (plan->command != 9 && (s8)target->equipBonus[1] != 0 &&
      action->targetMode == 1 && target->hp != 0) {
    work->unk858 = tid;
    /* FAKEMATCH: barrier; sched2 otherwise issues movs r1, #128 for unk85c's
     * address before the store to unk858, where the ROM stores first, 2
     * instructions. */
    asm("");
    work->unk85c = aid;
  }

  if (target->hp != 0) {
    if (target->sleep != 0)
      if (target->sleep <= 6 && dealt > 0 && (3 & BattleRandomFar()) == 0) {
        target->sleep = 0;
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, tid);
        MESSAGE_PUSH(MsgWakes);
      }
  }
  MemFree(bak);
  UnitCalcFar(tid);
  PartyStatusDrawFar(Ram_HeapSlots->battleWork->partyStatus);
  if (target->hp != 0)
    BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, tid);
  if (abilityId != 403 && actor->haunt != 0 && (BattleRandomFar() & 3) == 0 &&
      dealt > 0) {
    s32 share;

    share = dealt >> 2;
    if (share == 0)
      share = 1;
    plan->hauntDamage += share;
  }
}
