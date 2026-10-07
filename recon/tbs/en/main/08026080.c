/* 08026080: listing 1585, draft 1585, differ 102 (common 1534); both
 * frames are 324 bytes. Every remaining difference is a spill slot offset
 * below sp+48 (see the hash analysis below); every instruction, register
 * and order matches.
 * Session S07-08026080/4 (102, nothing kept):
 * - dead register initializers reach GCSE too: each local initialized at
 *   its declaration (= 0) adds one real insn after addressof and flow
 *   deletes it; 29 of them (found ... result, i, j, n, y, total, matrix,
 *   slotId, pending, cursor, redraw, slot, unit, who, head, window,
 *   runtime, and the confirm block's id and k) give 1440 insns, 721
 *   buckets, code unchanged (102); with three more efx.angle stores, 1446
 *   and 725 buckets: differ 2, the same 2 as the efx.angle bed. entry = 0
 *   (355) and cnt = 0 (118) change the code. So the slot problem is only
 *   the count, but there are too few locals for an honest initializer
 *   habit to supply 34 to 41; not kept (a device for the lead's call).
 * - on that bed, the loop head: s32 id = ids[cursor] as the loop body's
 *   first declaration 10 (the pending store moves below the loads); n =
 *   ids[cursor] after pending = 0, 1004; pending = 0 after the call 12;
 *   cursor[ids] 24 (address rebuilt from sfp, draft 1587).
 * - RTL at the head (bed): gcse leaves (set 273 1421) (ids) and (set 275
 *   1422) (cursor * 2) before insn 919; cse2 substitutes both into 919's
 *   address and the copies die. The cursor * 2 computation (insns 5175,
 *   5940) sits after loop-back label 5214 and before label 902, which the
 *   preheader's jump 5211 enters, so the loads are a block of their own
 *   in both builds.
 * Session S07-08026080/3 (102, nothing kept), measured on the 725-bucket
 *   bed (eighteen dead efx.angle stores, differ 2, diagnosis only):
 * - the last 2 are sched2's tie: in the bed's block from label 821 the
 *   reloads 6118 (r2 <- ids) and 6121 (r3 <- cursor * 2) both have
 *   priority 121 and five forward dependents (6118: 838 845 874 885 909,
 *   r2's next set is the ldrh at 885; 6121: 838 845 848 874 909, r3's
 *   next set is 848), neither depends on the last scheduled store 6115,
 *   so rank_for_schedule falls to INSN_LUID and keeps reload's emission
 *   order. find_reloads_address_1's REG/REG case pushes op0 (ids, as
 *   base, BASE_REGS) before op1 (cursor * 2, index, LO_REGS); LO_REGS
 *   sorts first in reload_order, which is why the index takes r3 and the
 *   base r2, but emit_reload_insns emits by reload number. (plus cursor*2
 *   ids) would print [r2, r3] with r2 = cursor * 2, so the native RTL is
 *   (plus ids cursor*2) as here, and its r3 load is likely no reload of
 *   this insn but insn 836 (set (reg 275) (reg 1411)) surviving to local
 *   alloc, which gives 275 r3, so 836 loads 1411's slot first. cse2
 *   folds 836 here because 1411 is the qty's first reg; make_regs_eqv
 *   makes 275 canonical only when 275 outlives the block and 1411. Find
 *   the source shape that keeps that copy (or adds a sixth dependent to
 *   the r3 load).
 * - trials at the loop head: *(ids + cursor) 118 and cursor[ids] 118 on
 *   the draft (the address is rebuilt from sfp, draft 1587); n = cursor
 *   then ids[n] on the bed: 2 (cse1 folds n into cursor); y replaced by
 *   pos.y in the cursor block: 446 on the draft, buckets unchanged (707).
 *   Permuter 5 min, PERM_RANDOMIZE over head/pending/the call/the two
 *   word stores/tile on the bed: 2,389 candidates, no gain.
 * Session S07-08026080/2 (102), kept:
 * - case 4 tests unit->maboroshi ... unit->shiryo directly, as case 6
 *   does; the five pointer locals pd..pc are gone. Code identical (102,
 *   1411 insns after addressof): CSE keeps the field addresses in
 *   registers by itself.
 * Measured, not kept:
 * - the last 2 are not a bucket effect: with 58 dead efx.angle stores
 *   (about 765 buckets, also a native slot order) the slots match and the
 *   same 2 remain, as at 725 and 727. In greg, insn 838 (zero_extend (mem
 *   (plus ids cursor*2))) takes reloads r2 <- ids slot (6073) and r3 <-
 *   cursor*2 slot (6076); sched2 ranks the two equal (priority 121, five
 *   dependents each) and keeps reload order, so native's reload emitted
 *   the cursor*2 load first with the same registers. Thumb's
 *   BASE_REG_CLASS is BASE_REGS and INDEX_REG_CLASS LO_REGS, so
 *   reload_order follows which operand is reloaded as base; read
 *   find_reloads_address_1's REG/REG case and choose_reload_regs next.
 * - at 765: head = entries after the call 14 (pos and head addresses
 *   swap r5/r6); the id in n before pending = 0: 1002. Permuter 5 min over
 *   head/pending/the call (1826 candidates): no gain. At 725, the main
 *   loop as do { ... } while (redraw != 0): the same 2. Elsewhere (the
 *   name drawing, the confirm block) native reloads ids then cursor*2, so
 *   only the loop head emits the index first; a separate pseudo copied
 *   into r3 before insn 838 would explain it (untested).
 * - flow (life) deletes 106 insns after cse2, 67 of them present at GCSE;
 *   combine deletes 157 more. Live lengths come from life, so insns that
 *   flow deletes leave pos's priority alone, while tails that jump2
 *   merges stay through global and lengthen pos (the cross-jump lead's
 *   problem).
 * Session S07-08026080/1 (from 106), kept:
 * - head = entries as the main loop's first statement: (plus sfp -88) is
 *   now computed in the loop, PRE hoists it and numbers its reaching
 *   register in bucket order, as native: 102. The six spilled PRE
 *   pseudos now sit in native order apart from one rotation: draft efx 36,
 *   selSlot 32, tbl 28, head 24, ids 20, cursor * 2 16 against native tbl
 *   36, head 32, ids 28, cursor * 2 24, efx 20, selSlot 16 (the bucket
 *   wrap, 707 buckets: max_cuid 1414 or 1415, 1411 real insns after
 *   addressof).
 * Diagnostic, not kept: with head in the loop, eighteen extra dead
 *   efx.angle = 0 stores before the real one (two insns each at GCSE,
 *   flow deletes them) give 725 buckets and differ 2; thirty-seven give
 *   745 buckets, 102. So the table size is the whole slot problem. The
 *   last 2 at 725: the main loop's first ids[cursor] load reloads its two
 *   spilled address pseudos base first (ldr r2, [sp, #28]; ldr r3,
 *   [sp, #24]) where native loads the index first; lreg has (plus (reg
 *   ids) (reg cursor*2)), so native likely has the operands the other way
 *   round there. pending = 0 before head = entries: same 2.
 *   Twenty stores give 727 buckets and differ 2 as well (1451 insns after
 *   addressof). Target: 1445 to 1452 real insns after addressof (725 or
 *   727 buckets) against 1411 now, or 1373 to 1376 (689).
 * - the spread loop as for (i = 0; (u32)i < spread; i++): no change (102,
 *   1411 insns), kept as the plainer form.
 * Lead, not kept: jump2 cross-jumps (JUMP_CROSS_JUMP after reload), so a
 *   tail the native wrote out in every case is merged into one copy in
 *   the output, but every copy counts at GCSE. The no-condition drawing
 *   (nibble 2, 0x8A3, nibble 15) written out in cases 5, 3, 4 and 6
 *   instead of goto no_condition: 1443 insns, 723 buckets, differ 447;
 *   in cases 4 and 6 only: 1432, 717, 449. Both lengthen pos's live range
 *   so pos spills and loses fp (draft 1586: one register copy more,
 *   frame 320), the old pos/0 priority problem. Next: the tail written
 *   out in all four cases plus 2 to 9 more such insns, with pos kept in
 *   fp; check case 7's own nibble 2/15 pair and the other gotos
 *   (frame_tail, draw_name, step_back) for tails the native wrote out.
 * Bitfield stores cannot explain the extra insns: SLOW_BYTE_ACCESS is 0,
 *   so field writes take the narrowest mode, while native stores the
 *   entry's word at +4 whole.
 * Session T10-08026080/4 (from 116), kept:
 * - the clear loop as for (i = 0; i < 6; i++) tbl[i].flags = 0: loop's
 *   dbra reversal (one store to a giv of i) turns it into the native
 *   count down from tbl[5], and emits the new counter start after the
 *   hoisted 0, so movs r2, #0 now precedes movs r7, #5 as native: 106.
 *   tbl[5 - i] counting up 110 (reversed again, stepping up from tbl[0]).
 * - the pending loop's two branches compute block locals x and y from
 *   every load first, then store targetPos.x, slot->x, targetPos.y,
 *   slot->y (the else branch with slot->flags = 1 before them): the
 *   branch stores and loads now follow the native exactly. Then branch
 *   alone 114, both 108.
 * Bucket hypothesis confirmed (diagnostic, not kept): GCSE sizes its
 *   expression table from max_cuid, the real insns at GCSE entry (1412 to
 *   1415 here: 707 buckets, set table 353). Three chains of twelve dead
 *   zero stores at entry (flow deletes them, nothing else moves) give
 *   1450, 725 buckets: differ 40; four more give 727 buckets: 40. So the
 *   native source carried about 36 to 41 more real insns at GCSE (or 35
 *   to 38 fewer for 689) that later passes (combine, flow, loop) remove.
 *   At 725/727 only three slots still rotate: draft 32 (sp+212 tbl
 *   address) / 36 / 40 against native 36 / 40 / 32, likely head, which
 *   is a user variable here, not a PRE pseudo. Next: find the honest
 *   source shape with those extra insns (casts, copies or temporaries
 *   that combine folds), and head's slot.
 * The whole prologue through the first call now
 * matches: sel is the parameter itself in r10, mode in r8, and
 * gLinkCountdownWork is reached as cell 48 after BtlWk, so CSE keeps
 * the BtlWk address in r5 and adds 192 as the native does.
 * Session T10-08026080/3 (from 542), kept:
 * - the cursor's else-if branch stores tbl[0].x = pos.x without nx: nx no
 *   longer lives where the shared constant 1 does, so the 1 takes r1 with
 *   nx as native (was r4) and the whole cursor block matches apart from
 *   spill offsets: 477.
 * - each info window case divides once, anchor = pos.x / 8, and tests
 *   anchor + N: with one use the loaded pos.x dies at the division's copy,
 *   CSE keeps the copy, and the load goes straight into it (native ldr r3;
 *   cmp r3; adds r3, #7, no register copy): 440, draft 1587. anchor is an
 *   ordinary local now, so its switch-block FAKEMATCH is gone.
 * - the side block back in native order (index check, then selSlot and
 *   cnt++): with the two changes above pos keeps fp and the 0 is
 *   rematerialized as native: 135, draft 1586. The selSlot/cnt FAKEMATCH
 *   is gone; the draft has no device left except infoWin in r9.
 * - cursor = cursor + total - 1 in the step back (native adds cursor, then
 *   total, and keeps total in r1 for the modulo), and repeat = 0 / 1 set
 *   before pressed in the auto block (native r5 before r6): 116, draft
 *   1585, the instruction counts equal.
 * Trials: on 477, native order for the side block's selSlot/cnt 597
 *   (frame 320: pos loses fp again, the 0 has 24/112 against pos 205/963);
 *   on 440, the clear loop as for (i = 5; i >= 0; i--) no change; the side
 *   block through n = 6 - i 1307. On 116, the pending loop's stores in
 *   native source order (tx, sx, ty, sy; else flags first) 134 (the loads
 *   then follow the stores); through nx/ny 857; through block locals x and
 *   y 148; the else branch as slot->flags = 1; slot->x = targetPos.x =
 *   entry->x; slot->y = targetPos.y = entry->y + 8: 120 (sched2 sinks the
 *   flags store below targetPos.x and slot->x and keeps the entry->y load
 *   after them; native hoists both entry loads above every store).
 * Spill slots now: GCSE's expression table has 707 buckets (max_cuid
 *   1414 or 1415), so (plus sfp C) still hashes in C order; the native
 *   order needs a bucket multiple between 13926 - 160 and 13926 - 112:
 *   657, 689, 725, 727, 765 or 767 buckets.
 * Session T10-08026080/2 (from 624), kept:
 * - the cursor sine as Trig_Sin(...) / 32768, as the pending loop and the
 *   name drawing write it: the value stays in r0 as native, 624 (draft
 *   1595, so equal count but closer code).
 * - the pending loop indexes entry = &head[i]: CSE shares i * 2 between
 *   selIds[i] and i * 12, so the i * 2 giv (no longer single use) combines
 *   with the selIds address and is reduced to the native offset stepping
 *   by 2 at sp+12 from 2, with selIds' base in r10 and entry = head + 12
 *   set after the i < cnt guard: the whole pending loop matches native
 *   apart from slot offsets. Alone 781 (frame 328).
 * - the side marker block written as the first block: tbl[6 - i] directly
 *   (a giv stepping by -4 from &tbl[6], native r0) and selSlot[cnt] =
 *   6 - i; the mark/side FAKEMATCH is gone. With head[i]: 629 (frame 320);
 *   with spill offsets ignored the diff falls from about 420 to 225.
 *   Without head[i] 681. In native order global gives fp to the spread
 *   loop's QImode 0 (8 refs over 112 insns, priority 2142) before pos
 *   (41 refs over 963, 2128), so pos spills and the selIds giv takes fp;
 *   native keeps pos in fp and rematerializes the 0 (movs r3, #0).
 * - selSlot[cnt] and cnt++ before the side block's index check (permuter,
 *   tagged FAKEMATCH: native stores them after it): 542, frame 324, pos
 *   back in fp, but the 0 now sits in r8 and 254 in lr (native 254 in r8,
 *   ids in lr, 0 rematerialized). Find a native-order way to drop the 0's
 *   priority below pos and remove this device.
 * Trials: head replaced by entries everywhere 871 (the -88, -84, -80
 *   addresses become separate PRE pseudos); permuter 4 min on 629: 542
 *   (the reorder above; its other rewrite, a u8 tmp in case 4/6, is noise).
 * Spill slot order, measured: PRE numbers its reaching registers in hash
 *   bucket order; (plus sfp C) hashes to 13926 + C modulo the table size
 *   (max_cuid / 2 | 1, 717 here), so the draft orders efx (-236) < selSlot
 *   (-160) < tbl (-112) < ids (-16) < cursor * 2. The native order tbl <
 *   head < ids < cursor * 2 < efx < selSlot fits a table of 725 or 727
 *   (or 689) with head as a PRE pseudo of (plus sfp -88); here head is a
 *   user variable and takes an initial slot right after matrix.
 * Session of 2026-10-07 (from 1084), kept:
 * - the kind 4 and 6 searches as one || chain of byte tests each: fold's
 *   fold_truthop merges delusion/stun into the word test & 0xFF0000FF and
 *   sleep/psy_seal into a halfword test whose & 0xFFFF survives to loop,
 *   which hoists the mask into r4, caller-saved as native: 1031.
 * - the found loop's unit as its own block local st, and the side marker's
 *   slot pointer as its own block local side, so neither shares a pseudo
 *   with the long-lived unit and slot: 669. The found loop's registers now
 *   match exactly (j r5, found r7, ids offset r6, st r1, mask caller-saved
 *   at sp+8), r6 joins the reload rotation as native (spread = 1 through
 *   r6), and the pending loop spills slot at sp+4 and caller-saves the
 *   &targetPos copy in r4 at sp+8 as native.
 * - the name drawing's unit as its own local who: 624 (declared after
 *   unit or after pc, the same).
 * Next divergence: reload-time spill slots below 48 are ordered
 *   differently. Native (use counts): 12 selIds giv (4), 16 selSlot base
 *   (2), 20 (3), 24 (8), 28 ids address sp+308 (21), 32 head (7), 36 tbl
 *   address sp+212 (10), 40 and 44 (2 each); draft: 12 (7), 16 ids (22),
 *   20 tbl (12), 24 (2), 28 (3), 32 (2), 36 (2), 40 (7), 44 (2). The draft
 *   has no selIds giv: it shifts i each time.
 * Trials on the way: st alone 1186 (frame 320: one caller-save slot fewer);
 *   entry = &head[i] in the pending loop: alone 1149, with st 1046, with
 *   st and side 826 (so entry stays a stepped pointer); targetPos through
 *   a pointer tp (in the call argument, at the loop top, or before the
 *   loop): 1155 alone, 1044/1046/1036 with st and head[i], no change with
 *   side (dropped); unit without st on 669: 1041; on 624, the name
 *   character in nx instead of n 630, the pending loop counting with j
 *   1119, case 6's count as its own local no change, the cursor sine in
 *   width instead of i 624 (draft 1595). Permuter 5 min over the
 *   pending loop (st and head[i], 1046): 18 candidates, no gain.
 * The pending loop still differs: native computes entry = head + 1 after
 *   the i < cnt guard and steps a selIds byte offset (from 2) spilled at
 *   sp+12, as a giv of i would; the draft sets entry before the guard and
 *   shifts i each time.
 * Session T10-08026080/1 (from 1332), each kept:
 * - slide_offset through ((struct MenuCell **)&BtlWk)[48]: native
 *   base reuse reproduced, 1332 (gWorkSlot[9]/[57] loads gWorkSlot+0 and
 *   offsets 36/228 instead, 1338).
 * - party and enemy lists as for (i = 0; i <= 5 && units[i] != 0xFF; i++),
 *   as the selection search at the end is written: both pre-tests and loop
 *   bodies take the native shape; the s16 enemy FAKEMATCH is gone.
 * - kind cases in body order 5, 3, 4, 6 (native body addresses).
 * - the found and sel-search loops count with j (native r5) apart from
 *   the list loops' i (native r7): 1394 -> 1379.
 * - the confirm block's id and counter are block locals (native r4 and
 *   r1, no call crossed); sel kept the selected id and caller-saved it in
 *   r4: 1379 -> 1101.
 * - sel as the parameter: 1104, with sel r10 and mode r8 as native.
 * - cnt = 0 before redraw = 0xFFFF: 1094, the prologue matches.
 * - the main loop's neighbour index and name character use n, so j lives
 *   only in the mode 2 search: 1084, instruction counts now equal. Still
 *   j sits in r7 and found in r1 (caller-saved at sp+8) against native j
 *   r5, found r7 and the 0xFFFF mask caller-saved at sp+8.
 * Trials this session: gWorkSlot indices 1338; while loop for the party
 * list 1347; (u16) sleep mask, no change (the native keeps ldrh then ands
 * with 0xFFFF hoisted to r4 and caller-saved at sp+8; not reproduced);
 * on 1094: ordinary infoWin 1174, no marker temporary 1190, no coordinate
 * temporary 1104, so the three devices stay. On 1084: found declared after
 * j, no change; (s16) sleep mask, no change (fold drops every & 0xFFFF
 * form); permuter 5 min --prefix --heat, best 1099. Next divergence:
 * spread = 1 reloads through r7 against native r6 (reload rotation; r6 is
 * free there in the greg dump), then the ids address spill at 16 against
 * native 28.
 * Parallel session of 2026-10-06 (on 8b95cd7680, merged; its body 1335
 *   against this body's 116, so this body is kept): kind case 5 first and
 *   the top-only BtlWk cell base, both carried above. Its trials
 *   (base 1335): (u16) cast of the sleep word 1335; sel r10 device 1363;
 *   head declared after matrix or last 1335, after window 1401, first
 *   1409; no head (entries directly) 1438; ids/tbl as pointer locals over
 *   buffers 1432-1442; gWorkSlot[9]/[57] at the top 1341, everywhere 1352;
 *   &gWorkSlot[9] base 1335; slots + sel parameter 1342; slots + mode r8
 *   1335; slots + mode r8 + sel r10 1361; press search through j 1377,
 *   block-local id 1361, nx 1377, width 1393; permuter 5 min seed 11
 *   --heat: weighted 29540 -> 24980 but differ 1455. It noted agscc's
 *   Thumb andsi3 turns a constant 0xFFFF mask into extzv, so the native
 *   mask was not a CONST_INT at expand time (now reproduced by the ||
 *   chain above).
 * Earlier history (differ 1400 -> 1332):
 * Spill slots follow declaration order: the native keeps runtime 76,
 * window 72, cursor 68, cnt 64, redraw 56, slotId 52, pending 48, matrix 44,
 * so they are declared in that order (was 1400). Still differing: ids, tbl
 * and sp+236 address temporaries sit at 16/20/40 against native 28/36/32.
 * Trials on this order's predecessor (1400): sel as the parameter itself
 * 1401; RESOLVE.C slots with sel as parameter 1415; mode forced to r8
 * 1396; sel r10 and mode r8 1447 (with slots 1461); sel r10 alone 1451;
 * slots forced to r5 1694. On this order: mode r8 1334 (no device kept);
 * pd..pc or column/rows/count declared in the switch block, no change;
 * pd..pc right after matrix (native spills one at 40), no change.
 * Recheck after shared work areas: ordinary infoWin differs 1464; removing
 * the enemy temporary 1493/1604, marker temporary 1445/1598, coordinate
 * temporary 1410/1599; removing all three temporaries 1523/1600. Keep the
 * current draft and its measured devices; no new device or credit.
 * The exact RESOLVE.C relative heap-cell pattern differs 1417/1600; moving
 * sel initialization before the first call with it differs 1475/1596.
 * Neither reproduces the native reuse of the BtlWk cell base.
 */
#include "AFFINE.H"
#include "SELECT.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "BTLWORK.H"
#include "BTLSYS.H"
#include "MENU_LIST.H"
#include "FIXED_MATH.H"
#include "UI.H"
#include "HEAP_STATE.H"

struct CursorSlot {
    u8 x;
    u8 y;
    u8 flags;
    s8 index;
};

s32 BattleMotion_ProjectConditionalPositionFar(s32 id, s32 *out);
void BattlePres_SetActorModesFar(u16 *ids, s32 highlight);
s32 UiText_GetWideStringWidth(u16 *text);
void UiText_DrawNumberInWindow(
    s32 value, s32 digits, s32 work, s32 x, s32 y);
void Ui_ClearVramBlock(void);

s32 BattleTarget_RunSelection(s32 sel, s32 mode, u32 spread, u32 kind)
{
    u16 ids[8];
    struct SelectionSprite entries[6];
    struct CursorSlot tbl[6];
    struct EffectPosition markPos;
    u16 selIds[14];
    u8 selSlot[8];
    struct EffectPosition pos;
    u16 name[15];
    struct EffectPosition namePos;
    struct EffectPosition targetPos;
    struct AffineTransform efx;
    struct BtlUnit *unit;
    struct BtlWork *runtime;
    struct UiWindow *window;
    register struct UiWindow *infoWin asm("r9"); /* FAKEMATCH: the ordinary window lifetime differs 1464; native r9 reduces it to 1400 without changing instruction count. */
    s32 cursor;
    s32 cnt;
    s32 total;
    s32 redraw;
    s32 slotId;
    s32 pending;
    s32 matrix;
    s32 found;
    s32 anchor;
    s32 column;
    s32 rows;
    s32 count;
    s32 i;
    s32 j;
    s32 n;
    s32 nx;
    s32 ny;
    s32 pressed;
    s32 y;
    s32 width;
    s32 repeat;
    s32 result;
    struct CursorSlot *slot;
    struct SelectionSprite *entry;
    struct SelectionSprite *head;
    struct BtlUnit *who;

    runtime = BtlWk;
    cnt = 0;
    redraw = 0xFFFF;
    slotId = Resource_LoadIntoFreeSlot(256);
    infoWin = 0;
    if (spread == 0)
        spread = 1;
    if (mode == 2 || mode == 4)
        ((struct MenuCell **)&BtlWk)[48]->slide_offset = -2;
    else
        ((struct MenuCell **)&BtlWk)[48]->slide_offset = 16;
    for (i = 0; i < 6; i++)
        tbl[i].flags = 0;
    cursor = -1;
    if (mode == 2) {
        for (i = 0; i <= 5 && runtime->party_units[i] != 0xFF; i++) {
            ids[cnt] = runtime->party_units[i];
            cnt++;
        }
    } else if (mode == 4) {
        ids[0] = (u16)sel;
        cnt = 1;
    } else {
        for (i = 0; i <= 5 && runtime->enm_no[i] != 0xFF; i++) {
            ids[cnt] = runtime->enm_no[i];
            cnt++;
        }
    }
    ids[cnt] = 0xFF;
    total = cnt;
    if (mode == 2) {
        if (spread != 0xFF && kind != 0) {
            found = 0;
            for (j = 0; j < cnt; j++) {
                struct BtlUnit *st;

                if (ids[j] == 0xFE)
                    continue;
                st = UnitGetFar(ids[j]);
                switch (kind) {
                case 5:
                    if (st->hp == 0) {
                        sel = ids[j];
                        found = 1;
                    }
                    break;
                case 3:
                    if (st->doku != 0)
                        found = 1;
                    break;
                case 4:
                    if (st->maboroshi != 0 || st->shibire != 0
                        || st->nemuri != 0 || st->fuuji != 0
                        || st->shiryo != 0)
                        found = 1;
                    break;
                case 6:
                    if (st->maboroshi != 0 || st->shibire != 0
                        || st->nemuri != 0 || st->fuuji != 0
                        || st->shiryo != 0 || st->doku != 0
                        || st->akuryo != 0)
                        found = 1;
                    break;
                }
                if (found != 0) {
                    sel = ids[j];
                    break;
                }
            }
        }
        for (j = 0; j < cnt; j++) {
            if (ids[j] == sel)
                break;
        }
        if (j != cnt)
            cursor = j;
    }
    if (cursor < 0)
        cursor = (cnt - 1) / 2;
    while (1) {
        if (ids[cursor] == 0xFE)
            goto step_back;
        if (GameFlag_TestFar(0x16C) && mode == 1 && UnitGetFar(ids[cursor])->hp == 0)
            goto step_back;
        break;
    step_back:
        cursor = cursor + cnt - 1;
        cursor %= cnt;
    }
    if (mode != 2) {
        BattleMotion_ProjectConditionalPositionFar(sel, (s32 *)&markPos);
        tbl[0].flags = 8;
        tbl[0].x = (u8)markPos.x;
        tbl[0].y = 0x80;
    }
    window = UiWindow_Create(0, 12, 30, 4, 74);
    while (1) {
        head = entries;
        pending = 0;
        BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&pos);
        ((s32 *)head)[1] = 0x40002000;
        ((s32 *)head)[2] = pending;
        head->tile = Resource_GetBuffer(slotId, (((gFrameCount >> 2) & 31) << 8) + (s32)Menu_AnimatedCursorTiles);
        pos.y += Trig_Sin(gFrameCount << 12) / 32768;
        y = pos.y;
        if (tbl[0].flags & 1) {
            nx = (pos.x + tbl[0].x) / 2;
            ny = (y + tbl[0].y) / 2;
            if ((pos.x - nx < 0 ? -(pos.x - nx) : pos.x - nx) <= 7)
                pending = 1;
            pos.x = nx;
            tbl[0].x = (u8)nx;
            tbl[0].y = (u8)ny;
            pos.y = ny;
        } else if ((u8)tbl[0].flags <= 3) {
            tbl[0].x = (u8)pos.x;
            pos.y = y;
            tbl[0].y = (u8)y;
            tbl[0].flags = 1;
        } else {
            pos.x = tbl[0].x;
            pos.y = tbl[0].y;
            tbl[0].flags = (u8)(tbl[0].flags - 4);
            if (tbl[0].flags <= 3)
                tbl[0].flags = 1;
        }
        head->x = pos.x - 8;
        head->y = (u8)(pos.y - 16);
        Runtime_PushSlotEntry((s32 *)head, 240);
        if (spread == 0xFF) {
            efx.scale_x = 256;
            efx.scale_y = 256;
        } else {
            efx.scale_x = 176;
            efx.scale_y = 176;
        }
        efx.angle = 0;
        matrix = AffineMatrix_BuildForEffect(&efx);
        if ((redraw & 1) == 0)
            goto frame_end;
        cnt = 0;
        for (i = 0; i < 6; i++)
            tbl[i].flags &= (u8)~2;
        for (i = 0; (u32)i < spread; i++) {
            {
                n = cursor + i;
                if (n < total && ids[n] != 0xFE) {
                    selIds[cnt] = ids[n];
                    tbl[i].flags |= 2;
                    if (tbl[i].index != i) {
                        tbl[i].flags &= (u8)~1;
                        tbl[i].index = (s8)i;
                    }
                    selSlot[cnt] = (u8)i;
                    cnt++;
                }
                if (i != 0 && (n = cursor - i) >= 0 && ids[n] != 0xFE) {
                    selIds[cnt] = ids[n];
                    tbl[6 - i].flags |= 2;
                    if (tbl[6 - i].index != -i) {
                        tbl[6 - i].flags &= (u8)~1;
                        tbl[6 - i].index = (s8)-i;
                    }
                    selSlot[cnt] = (u8)(6 - i);
                    cnt++;
                }
            }
        }
        for (i = 0; i < 6; i++) {
            if ((tbl[i].flags & 2) == 0)
                tbl[i].index = 6;
        }
        selIds[cnt] = 0xFF;
        BattlePres_SetActorModesFar(selIds, 1);
        if (ids[cursor] > 7)
            goto draw_name;
        if (spread == 0xFF)
            goto frame_tail;
        if (kind == 0)
            goto frame_tail;
        unit = UnitGetFar(ids[cursor]);
        BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&pos);
        if (infoWin != 0)
            UiWork_Finalize(infoWin, 1);
        switch (kind) {
        case 7:
            anchor = pos.x / 8;
            column = anchor - 4;
            if (anchor + 4 > 29)
                column = 22;
            infoWin = UiWindow_Create(column, 8, 9, 3, 6);
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(0x8AC, (struct RenderInput *)infoWin, 0, 0);
            UiWork_SetParamNibble(15);
            goto frame_tail;
        case 1:
            anchor = pos.x / 8;
            column = anchor - 7;
            if (anchor + 6 > 29)
                column = 17;
            infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)infoWin, 0, 0);
            UiText_DrawNumberInWindow(unit->hp, 4, (s32)infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_hp, 4, (s32)infoWin, 56, 0);
            goto frame_tail;
        case 2:
            anchor = pos.x / 8;
            column = anchor - 7;
            if (anchor + 6 > 29)
                column = 17;
            infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_PpString, (struct RenderInput *)infoWin, 0, 0);
            UiText_DrawNumberInWindow(unit->ep, 4, (s32)infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_ep, 4, (s32)infoWin, 56, 0);
            goto frame_tail;
        case 5:
            anchor = pos.x / 8;
            column = anchor - 7;
            if (anchor + 5 > 29)
                column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            if (unit->hp != 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset(0x8AB, (struct RenderInput *)infoWin, 0, 0);
            goto frame_tail;
        case 3:
            anchor = pos.x / 8;
            column = anchor - 7;
            if (anchor + 5 > 29)
                column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            if (unit->doku == 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset(0x8A4, (struct RenderInput *)infoWin, 0, 0);
            goto frame_tail;
        case 4:
            count = 0;
            if (unit->maboroshi != 0)
                count = 1;
            if (unit->shibire != 0)
                count++;
            if (unit->nemuri != 0)
                count++;
            if (unit->fuuji != 0)
                count++;
            if (unit->shiryo != 0)
                count++;
            if (count == 0)
                count = 1;
            rows = 9 - count;
            if (rows <= 3)
                rows = 4;
            anchor = pos.x / 8;
            column = anchor - 7;
            if (anchor + 9 > 29)
                column = 14;
            infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
            count = 0;
            if (unit->maboroshi != 0) {
                UiText_DrawCharacterAtOffset(0x8A5, (struct RenderInput *)infoWin, 0, 0);
                count = 1;
            }
            if (unit->shibire != 0) {
                UiText_DrawCharacterAtOffset(0x8A6, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->nemuri != 0) {
                UiText_DrawCharacterAtOffset(0x8A7, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->fuuji != 0) {
                UiText_DrawCharacterAtOffset(0x8A8, (struct RenderInput *)infoWin, 0, count * 8);
                ++count;
            }
            if (unit->shiryo != 0) {
                UiText_DrawCharacterAtOffset(0x8A9, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (count == 0)
                goto no_condition;
            goto frame_tail;
        case 6:
            count = 0;
            if (unit->doku != 0)
                count = 1;
            if (unit->maboroshi != 0)
                count++;
            if (unit->shibire != 0)
                count++;
            if (unit->nemuri != 0)
                count++;
            if (unit->fuuji != 0)
                count++;
            if (unit->shiryo != 0)
                count++;
            if (unit->akuryo != 0)
                count++;
            if (count == 0)
                count = 1;
            rows = 9 - count;
            if (rows <= 3)
                rows = 4;
            anchor = pos.x / 8;
            column = anchor - 7;
            if (anchor + 9 > 29)
                column = 14;
            infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
            count = 0;
            if (unit->doku != 0) {
                UiText_DrawCharacterAtOffset(0x8A4, (struct RenderInput *)infoWin, 0, 0);
                count = 1;
            }
            if (unit->maboroshi != 0) {
                UiText_DrawCharacterAtOffset(0x8A5, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->shibire != 0) {
                UiText_DrawCharacterAtOffset(0x8A6, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->nemuri != 0) {
                UiText_DrawCharacterAtOffset(0x8A7, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->fuuji != 0) {
                UiText_DrawCharacterAtOffset(0x8A8, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->shiryo != 0) {
                UiText_DrawCharacterAtOffset(0x8A9, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->akuryo != 0) {
                UiText_DrawCharacterAtOffset(0x8AA, (struct RenderInput *)infoWin, 0, 8 * count);
                count++;
            }
            if (count == 0)
                goto no_condition;
            goto frame_tail;
        default:
            break;
        }
        goto frame_tail;
    no_condition:
        UiWork_SetParamNibble(2);
        UiText_DrawCharacterAtOffset(0x8A3, (struct RenderInput *)infoWin, 0, 0);
        UiWork_SetParamNibble(15);
        goto frame_tail;
    draw_name:
        if (spread == 0xFF)
            goto frame_tail;
        who = UnitGetFar(ids[cursor]);
        BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&namePos);
        namePos.y += Trig_Sin(gFrameCount << 12) / 32768;
        if (who->class_id == 125 || who->class_id == 122) {
            width = 0x80E;
            if (who->class_id == 125)
                width++;
            UiText_CopyMessageString(width, name, 14);
        } else {
            for (i = 0; i <= 13; i++) {
                name[i] = n = who->name[i];
                if (!n)
                    break;
            }
            name[i] = 0;
        }
        width = UiText_GetWideStringWidth(name);
        namePos.x -= width / 2;
        namePos.x -= 8;
        if (namePos.x + width > 224)
            namePos.x = 224 - width;
        if (namePos.x < 0)
            namePos.x = 0;
        Ui_ClearVramBlock();
        UiText_RenderWideStringAtOffset(name, window, namePos.x, 4);
    frame_tail:
        redraw &= ~1;
    frame_end:
        if (pending != 0) {
            for (i = 1; i < cnt; i++) {
                entry = &head[i];
                slot = &tbl[selSlot[i]];
                BattleMotion_ProjectConditionalPositionFar(selIds[i], (s32 *)&targetPos);
                targetPos.y = targetPos.y + Trig_Sin(gFrameCount << 12) / 32768;
                *entry = *head;
                if (slot->flags & 1) {
                    s32 x = (targetPos.x + slot->x) / 2;
                    s32 y = (targetPos.y + slot->y) / 2;

                    targetPos.x = x;
                    slot->x = x;
                    targetPos.y = y;
                    slot->y = y;
                } else {
                    s32 x = entry->x;
                    s32 y = entry->y + 8;

                    slot->flags = 1;
                    targetPos.x = x;
                    slot->x = x;
                    targetPos.y = y;
                    slot->y = y;
                }
                entry->mode = 1;
                entry->x = targetPos.x - 8;
                entry->y = targetPos.y - 12;
                if (spread == 0xFF)
                    entry->affine = 0;
                else
                    entry->affine = 1;
                entry->param = matrix;
                Runtime_PushSlotEntry((s32 *)entry, 240);
            }
        }
        pressed = gKeyState;
        repeat = gKeysRepeat;
        if (gLinkCountdownWork->auto_enabled) {
            repeat = 0;
            pressed = 0;
            if (gLinkCountdownWork->auto_delay == 0) {
                gLinkCountdownWork->auto_delay = 60;
                repeat = 1;
                pressed = 1;
            } else {
                gLinkCountdownWork->auto_delay--;
            }
        }
        if (pressed & 1) {
            s32 id;
            s32 k;

            id = ids[cursor];
            redraw = 0;
            result = -1;
            for (k = 0; k <= 5 && runtime->party_units[k] != 0xFF; k++) {
                if (runtime->party_units[k] == id) {
                    result = 0x100 | k;
                    break;
                }
            }
            if (result < 0) {
                for (k = 0; k <= 5 && runtime->enm_no[k] != 0xFF; k++) {
                    if (runtime->enm_no[k] == id) {
                        result = 0x180 | k;
                        break;
                    }
                }
            }
            cursor = result;
        } else if (spread != 0xFF) {
            if (repeat & 0x90) {
                AudioCommand_PlayFar(111);
                do {
                    cursor++;
                    cursor %= total;
                } while (ids[cursor] == 0xFE);
                redraw |= 1;
            }
            if (repeat & 0x60) {
                AudioCommand_PlayFar(111);
                do {
                    cursor = cursor + total - 1;
                    cursor %= total;
                } while (ids[cursor] == 0xFE);
                redraw |= 1;
            }
        }
        if (gLinkCountdownWork->active == 0 || (pressed & 2)) {
            AudioCommand_PlayFar(113);
            cursor = -1;
            break;
        }
        WaitFrames(1);
        if (redraw == 0)
            break;
    }
    WaitFrames(1);
    Resource_ResetEntry(slotId);
    if (infoWin != 0)
        UiWork_Finalize(infoWin, 1);
    UiWork_Finalize(window, 1);
    BattlePres_SetActorModesFar(ids, 0);
    gLinkCountdownWork->slide_offset = 0;
    WaitFrames(1);
    return cursor;
}
