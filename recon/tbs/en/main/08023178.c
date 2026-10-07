/* 08023178: Ui_RunOwnerStatusScreen draft.
 * Native 1392 instructions, draft 1392, common 1380, differ 24.
 * Frame 352 bytes, as native. The setup loops, the cursor update, the
 * face and cursor sprites (0x3FF in r8, the zero in r7, as native),
 * the stat pushes, the djinn rows, the condition list with its four
 * modifier tests and the condition loads match.
 * Remaining: the help block's saved/pointer pair (r5/r6 swapped) and
 * code = 0 set before MemAlloc where native sets it after.
 *
 * Session 9 2026-10-07: the modifiers read the field in each test, as
 * object->atk_lv > 0 then object->atk_lv < 0, without m: 40 -> 24, all
 * four native (ldrb r1 kept for the second test, the sign-extending
 * read with dest == base for the first, lsl #24 and bge). GCSE's PRE
 * still reloads the field at the end of the first then-block (any store
 * kills a load there), but CSE2 then finds that reload equal to the
 * first test's QI register and deletes it: the then-block's store is
 * sel[i] (alias set of the s8 array, 45), which strict aliasing keeps
 * apart from the BtlUnit field (65). Session 6's same trial (73) stored
 * through an s8 pointer (alias set 0), so the reload stayed.
 *
 * Session 8 2026-10-07: (s8)m < 0 for the second tests on 40: 182 (one
 * ldrsb for both tests). The CSE1 dump's block list shows every path
 * from the list's labels (the do-while(0) start and each skipped
 * then-block) reaching each second test with the first then-block
 * AROUND (the 8th entry from the list's start), so PATHLENGTH does not
 * end it. Even without CSE1 the merge would follow: gcse.c's
 * want_to_gcse_p takes any expression but a register or constant, and
 * the first test's (ashift m 24) dominates the second's with m unchanged,
 * so PRE deletes the second. Native's first test therefore shifted no
 * m at GCSE time: its sign extension came from a second QI read of the
 * field that CSE's LOAD_EXTEND_OP lookup did not fold into m's subreg,
 * and combine then put that read (sign_extend mem, at the ashiftrt)
 * after m's ldrb. Diagnostics: each first test in do { } while (0) 182
 * (jump1 drops the empty loop's notes, so no NOTE_INSN_LOOP_END ends
 * the path); attack only as s32 t = turns, if (t != 0 && (s8)(m = field)
 * > 0) then if (t != 0 && (s8)m < 0), 91 (jump1 threads the t == 0 jump
 * past the second then-block and deletes the second t test, so the
 * label is used once again before CSE1); s32 t = field, u8 m = field,
 * t > 0 then (s8)m < 0 on 40, 643. Not found: a source whose second read
 * of the field follows m's read without the fold.
 *
 * Session 7 2026-10-07: the condition loads as a plain
 * for (j = 0; j < i; j++) f(sel[j], tbl[j]) 59 -> 40: loop.c reduces
 * both addresses and reverses j itself (native's i > 0 guard, count-down
 * r4 and ldmia), and the byte load, lsl and asr stay three insns into r0
 * as native, with the long branch now native's. Modifier diagnosis: an
 * s8 local is promoted to SImode zero-extended (ARM PROMOTE_MODE), so
 * m's ldrb r1 is native's; native's first test (ldrb r1, then the
 * sign-extending ldrb/lsl/asr with dest == base) is combine's split of
 * the three-insn PARALLEL (m kept, sign_extend (mem) for the test),
 * which the draft already reproduces. Native's second test recomputes
 * (ashift m 24), so CSE did not carry the first test's ashift across the
 * then-block: here cse_end_of_basic_block skips the then-block (AROUND,
 * label with one use, no label inside), marks the next block QImode and
 * so processes the second test only on the path that knows the ashift.
 * s8 m with m > 0, m < 0 182 (one ldrsb for both). The condition list
 * as sel[i++] = code, without the write pointer and its FAKEMATCH, 40
 * (no change: loop.c reduces sel + i itself, i first as native); s8 m
 * on that form 182; s32 t and u8 m read in that order with t > 0 and
 * (s8)m < 0 643. Not found yet: why native's first test is not skipped
 * AROUND (it would need a second use of the label after the then-block,
 * a label inside it, or nine branches already on the path). Code = 0
 * after MemAlloc on 40: 285. Editions (similar --editions,
 * native against English): de identical (08021e3c), fr 1 (0802211c), ja
 * 12 (080231dc), es and it 189 (080221b4, 08023120); diff --edition
 * cannot bound these scaffold copies yet ("bounded Thumb function only").
 *
 * Session 6 2026-10-07: the second modifier test is native's (lsls r3,
 * r1, #24; cmp; bge) once the first test's sign-extended read of the
 * field comes before m's read: s32 t = field; u8 m = field; t > 0, then
 * (s8)m < 0, attack only, 59 -> 126. Cause: CSE's LOAD_EXTEND_OP lookup
 * (cse.c, "See if a MEM has already been loaded with a widening
 * operation") turns any QI read of the field after m's zero-extending
 * read into (subreg:QI m), fold_rtx's paradoxical SUBREG case then makes
 * the first test's shift (ashift m 24), the second test's, and CSE or
 * GCSE joins them. Read first, the field keeps its own QI register and
 * the two shifts differ. The cost: the shared address (object + 309)
 * then lives 5 luids (read, two shifts, m's read) and loop.c hoists it
 * out of the do-while(0) (loop.08: 191 real insns; life 5 x savings 2
 * moved, life 2 x 2 not desirable), so the address sits in r0 before the
 * loop and the read is mov/ldrsb with dest != base. Native reads m
 * (ldrb r1) and then the field with dest == base, so its address dies
 * at the second read: native read the field for the first test after m
 * without that fold; no source form found yet. Rejected: m read first,
 * then t, 92 (folded); s8 *mp with t = *mp, m = *mp 126 (as t, m); no m,
 * the field read in both tests 73 (the second test's lsl #24 is native's,
 * but gcse.c counts any store a kill of every load, so PRE reloads the
 * field after the then-block's store: native's second test reads no
 * memory), the same with o = sel + i 75; s32 m = field & 0xFF with the
 * field read in the first test and (s8)m < 0, 184 (both reads stay in
 * place and the second test is native's, but combine puts the
 * sign-extending read, m's intermediate, before m's ldrb: mov/ldrsb with
 * dest != base, and o moves from r2 to r1). Condition loads: the byte read
 * into s32 c first 59, *q++ read into h first 59, *out++ in the call 59
 * (no change: combine still joins the QI read and both shifts into one
 * sign_extend; native's three instructions into r0 were never joined).
 *
 * Session 5 2026-10-07:the up-key n-- after the 0x6F call 87 -> 85
 * (sched2 moves it across the call to native's slot, after the 0x6F
 * argument). The face pool: the face x literal 172 is a movhi pool
 * load (pool_range 64, the pool's head); arm_reorg counts 58 bytes from
 * it to the gFrameCount load (ldrh 4, bl 4, the rest 2), under 64 - 4,
 * so gFrameCount joins the pool and the barrier follows it. Native puts
 * gFrameCount in the next pool and the barrier after the cursor tile
 * strh: it counts at least 2 bytes more there, or loads 172 through
 * zero_extendhisi2 (pool_range 60); no source form found for either.
 * An empty asm after the cursor's attr23 store (counted 4 bytes) 85 ->
 * 63, FAKEMATCH-tagged; after the face push it reorders the cursor's
 * attr23 store, 67. The condition loads with q = tbl before out = sel
 * 63 -> 61 (native loads tbl into r7 first). Their sign extension:
 * native's ldrb r0, [r5]; lsl; asr has base r5 and dest r0, which no
 * extendqisi2 memory alternative prints (that form needs dest == base),
 * so native loads the byte into a register and extends it apart:
 * combine did not join them there. u8 *out with (s8)*out 61, and the
 * byte shifted << 24 >> 24 by hand 61 (no change: combine joins them
 * again). The icon loop through a block-local cache pointer, set in the
 * for header after i, then x = 112, 61 -> 59: native's order (i = 0,
 * the cache base into r8, then 112); vc set before the for with i = 0
 * in its header 61. code = 0 after MemAlloc, retried on 61:
 * 296 (global allocation cascades from the frame setup on); the agility
 * delta read first 711. Modifier order diagnosed from native: ldrb r1
 * comes before the sign-extending ldrb r3, [r3] (dest == base), so in
 * native's RTL m's zero-extending load precedes a second, sign-extending
 * load that CSE left alone; reg alloc cannot reverse the two (the second
 * overwrites the address). Trial on 59: m read as *(u8 *)&field with the
 * field itself in the > 0 test, 92 (one ldrsb serves both again).
 *
 * Modifier tests 2026-10-07 (session 2): native reads each modifier
 * as u8 m (ldrb r1), tests (s8)m > 0 through a second sign-extending
 * load (combine keeps m's load), and recomputes (s8)m < 0 as m << 24
 * in the next block. CSE1's skip-blocks path reuses the first sign
 * extension for the second test here (.03.cse), so the < 0 test shares
 * one ldrsb. u8 m, (s8)m > 0 and m & 0x80: 389 -> 377 (the > 0 test is
 * native's; the & 0x80 test is an ands with 0x80 where native has lsl
 * #24). Rejected: the field read directly for the > 0 test 389 (CSE
 * equates the load with m's low byte); s8 m with m & 0x80 392; u8 m
 * with m >= 0x80 389; the condition chain with goto instead of
 * do-while(0) 541 (still merged); the cursor tile & 0x3FF 377 (no change).
 * Also rejected: (s32)(m << 24) < 0 for the second test 382; a block
 * size variable for the three 332-byte calls 377 (CSE folds it back into
 * each argument; native keeps 332 in r8 across MemAlloc).
 *
 * Session 3 2026-10-07: the second modifier test as (m >> 7) != 0
 * 377 -> 331 (lsr #7, beq where native has lsl #24, bge, but no
 * shared ands constant, so the list's branch offsets are native's).
 * CSE1 deletes any second test that shares the first's ashift: in
 * thumb the sign extension expands to ashift 24 + ashiftrt 24 and the
 * first test's QI load becomes (subreg:QI m) by LOAD_EXTEND_OP, so
 * (s8)m < 0 389, (s32)(m << 24) < 0 382, (s16)(m << 8) < 0 382,
 * (s8)m >> 7 382, (s8)m <= -1 413, (s8)m & 0x80 392 all merge or
 * worsen; the field read directly for the first test, or into a
 * separate s8 local before or after m, changes nothing (CSE still
 * folds the load to m's subreg). The skip-blocks path needs the label
 * after the first then-block to have one use; native must differ
 * there or in the path length (PATHLENGTH 10), not yet found.
 * code = 0 before MemAlloc instead of after it 331 -> 120
 * (the 332 pseudo, REG_EQUIV and local to its block, now keeps r8
 * across the three calls, code goes to sp+4 and reloads into r4 for
 * the modifier stores, and the freed registers fix the face/cursor
 * zero, the stat pushes' 24 in r8 and redraw in r10, as native).
 * redraw = 1 before idx = -1 120 -> 110 (native builds 1 in r3 and
 * negates it for idx); the loop-end check as gLinkCountdownWork->active
 * 110 -> 105 (native's literal; the spill slots no longer shift).
 * Rejected: code = 0 after the first copy 331, at the top of the help
 * block 708, after the modifier stores 785; the agility delta read
 * first 682; n-- before keys = 0, or n -= 1, 120 (no change); the
 * condition write pointer as sel + i or i + sel 120 (no change).
 * redraw = 0 before base->dirty = 1 105 -> 95; the icon loop's
 * p = objIcon before q = tbl 95 -> 93; the closing reset loop through
 * a block-local pointer (native r5, q's r7 before) 93 -> 89.
 * Rejected after 105: code = 0
 * after MemAlloc 318 (native's order, but the 332 pseudo
 * loses r8 again); j-- before the call in the condition loads 107;
 * n-- then the call then keys = 0 107; the agility modifier cleared
 * first 107; *out++ in the call 105, saved without the cast or the
 * copies sized by the struct 105 (no change); x = 112 first 105.
 * After 89: the write pointer as o = sel; o += i, or &i[sel], 89 (no
 * change: the add keeps sel first where native has i first). Session 4:
 * the pointer as (s8 *)(i + (s32)sel), FAKEMATCH-tagged, 89 -> 87 (the
 * add takes i first, as native). Modifier trials on 87: the field read
 * directly in both tests, no m, 212 (GCSE's PRE reloads it after the
 * store through o in the first then-block: o may alias the field); a
 * named label in the first then-block (diagnostic only: it ends CSE1's
 * path) with (s8)m < 0, 120, and s8 m assigned in the first test,
 * (m = field) > 0 then m < 0, 120, or u8 m with (s8)(m = field) > 0
 * then (s8)m < 0, 120: each shares one ldrsb (mov #0, ldrsb r1) for
 * both tests, cmp r1 where native has lsl #24, so GCSE also joins the
 * two sign extensions once CSE1 does not. Native keeps them apart.
 * Icon loop trials on 87: for (i = 0, x = 112; ...) 87 (no change);
 * x written as 112 + i * 15 89 (112 then follows i = 0, as native, but
 * the reduced x is incremented after p); with p = &objIcon[i] as well 89
 * (the loop end is native's, adds r6, r4, r5, but q's reload moves
 * before p's); tbl[i] for *q++ 344; p++ before x += 15 89; p++ in the
 * for header with x as 112 + i * 15, 87 (no change: the loop end and the
 * p, q reloads are native's, but the reduced x's 112 still comes before
 * the gVramBlockCache copy into r8 where native has it after). The
 * help block's r5/r6 swap is global allocation of saved and the
 * &object->agi address pseudo (r9 + 64): native gives the address r5.
 * saved declared after agiDelta, or block-local to the help block, 87
 * (no change).
 *
 * Register diagnosis 2026-10-07 (session 2, lreg/greg dumps): with
 * -fcall-used-r4 the call-saved low registers are r5, r6, r7. In the
 * face/cursor block (bb 66) the 0xFC00, 0xFE00 and 0x3FF constants are
 * block-local; regmove ties the 0x3FF to the cursor's and (insn 1568,
 * operand 2), and it is already in r7 before global allocation (it is
 * absent from greg's "regs to allocate" list) with 0xFC00 in r5 and
 * 0xFE00 in r6. Global then gives the zero (469: 10 refs over 228
 * insns, crosses 11 calls) r10, as r5-r7 are taken, r8 is a hard
 * conflict and r9 is object; redraw (54: 14 refs over 982) falls to r11.
 * Native has 0x3FF in r8 and the zero in r7, which frees r10 for redraw:
 * one cause for both. The hard r8 conflict of the zero and of 0x3FF has
 * no r8 insn inside bb 66 (not yet explained). Trials: the zero as a register
 * variable in r7 scores 294 but miscompiles (r7 is the eliminated frame
 * pointer, so global still gives it to 0x3FF and the cursor's attr23
 * receives 0x3FF): rejected; 0x3FF as a register variable in r8 with an
 * explicit mask at the cursor 549. A 5-minute permuter run (2 jobs, 963
 * candidates) with PERM_RANDOMIZE over the cursor pixels, face and
 * cursor found nothing below 377.
 *
 * Diagnosis 2026-10-07: most of the earlier cascade was variables shared
 * across calls where the native's value lives only between two calls:
 * every block-local below moved a value from a call-saved register to the
 * call-clobbered one native uses, and freed a call-saved register for the
 * long-lived values.
 *
 * Trials 2026-10-07: objName tile stored without "& 0x3FF" (the 10-bit
 * field masks in HImode, so 0x3FF and 8 lead the first pool and the pool
 * falls before the switch with a branch over it, as native) 750 -> 732;
 * the cursor tile likewise, 732 (no change, kept for symmetry); the face
 * record fetched before the attribute stores (native calls
 * GetBattleObjectSlotFar first) through p 732; the owner search value
 * as a block-local (native r3) 732 -> 675; the face record through a
 * block-local pointer (native r2) 675 -> 629; n = cur.pos[cur.mode] after
 * cur.mode = 2 (native indexes by the mode register) 629 -> 599; the stat
 * cursor index as a block-local (native r2) 599 -> 570; the status
 * message as a ?: argument (native loads it straight into r0) 570 -> 565;
 * the djinn tile code as a block-local 565 -> 536, written as 1, cleared
 * when extended, then + 8 (native's branch; 8/9 if/else becomes
 * 9 - (extended != 0) in jump) 536 -> 524; the condition list's write
 * pointer as a block-local (native r2) 524 -> 482; i for the condition
 * count instead of n (native r6) 482 -> 408. Rejected: code =
 * (extended == 0); code += 8 1055 (hoisted as neg/orr/lsr, 1 - x); j for
 * the condition help value 528, n 500, i 655. Each modifier read into
 * an s8 block-local m for its two sign tests 408 -> 389 (u8 m with s8
 * casts the same); not native: native loads the field twice, ldrb into
 * r1 for the < 0 test and ldrb, lsl, asr into the address register for
 * the > 0 test (the dest == base form of the sign-extending load), where
 * m is one ldrsb with a zeroed index here, 9 instructions short.
 * The loop-end check loads gLinkCountdownWork in native (its literal),
 * not gWindowWork + 168: written as gLinkCountdownWork->active 389 -> 444,
 * as the array-address spill slots then shift by 8 (sp+8/12 -> 16/20);
 * kept as gWindowWork[42] until the spill order is understood.
 *
 * Merged from a parallel session 2 (2026-10-06) whose body scored 769
 * (this body 89, kept): face sprite tile read through a pointer fetched
 * before the attribute stores 1087 -> 1085; the name sprite through a
 * pointer declared before icons (native keeps &objName at sp+28) 873 ->
 * 845; the cursor movement index as its own variable k, not the shared
 * count n (native r5, &cur in r6) 845 -> 769. Rejected there: the list
 * search reading list[n] directly 780; a separate djinn-loop index j 1148,
 * frame 356; the pixel zero stores after the first window call or inside
 * its arguments 769 (the store register still r3 where native reuses the
 * argument's r1); the modifiers read as (s8)(u8) 769; pixel offsets
 * assigned only when the entry changes (wrong semantics, diagnostic only)
 * 1028, frame 348. Permuter, 6 minutes on 769: best 750 through
 * temporaries only (noise, not carried).
 *
 * Diagnosis 2026-10-06: the native frame is 96 bytes of spill
 * slots under exactly 256 bytes of locals (cur 96, objName 128, objIcon 140,
 * sel 272, tbl 284, objCursor 328, objFace 340: declaration order, frame
 * growing down). Spill slots follow pseudo (declaration) order downward:
 * list, listCount, owner, base, winMain, winDesc, idx, handleCursor, the
 * cursor pixel pair, handleFrame, cnt, extended, keys, text, iconIdx, then
 * the array addresses. The cursor pixel offsets' zero stores survive flow
 * only if their sets are partial: one 8-byte {x, y} struct held in a DImode
 * pseudo, spilled to sp+56/sp+60. That gave the 4 missing frame bytes.
 *
 * Trials 2026-10-06: pixel pair as struct StatusPixel 1087 ->
 * 1007; declarations in native spill order 859; searched single moves 857;
 * owner search indexed by i 817; tbl/sel/objIcon used directly instead of
 * handles/codes/icons aliases 814; j for the redraw block's loops (djinn
 * rows, condition loads and fill: native r4 there, r6 for the others) 750.
 * Rejected: j also for the stat cursor 782; j for the djinn loop only 1154,
 * for the condition loops only 1152; search value in j 762, x 1072, n 750
 * (no change); djinn tile code as if/else either way, ?:, 8 + !extended,
 * (extended == 0) + 8 or a 0/1 flag plus 8: 815 to 1134, each folded to
 * 9 - (extended != 0) or hoisted. code = 0 before the allocation 760,
 * after the first copy 750 (native keeps that 0 in r4 for the modifier
 * stores and 332 in r8 across MemAlloc). A 5-minute permuter
 * run (--heat, 2 jobs) reached 700 by random rewrites; its minimization
 * did not finish, so none is carried over.
 *
 * Trials of a parallel session 2 (2026-10-06, its own line from 1087, best 769): face sprite tile read through a pointer
 * fetched before the attribute stores (native order): 1087 -> 1085.
 * Pixel offsets as a struct {x, y} with a separate icon-loop x: 1007 and
 * frame 352. Native pseudo order for the declarations: 873. The name sprite
 * through a pointer declared before icons (native keeps &objName at sp+28
 * above icons/codes/handles): 845. The cursor movement index as its own
 * variable k, not the shared count n (native keeps it in r5, &cur in r6):
 * 769. The list search reading list[n] directly instead of code: 780 (native
 * keeps the value in r3, the draft in r7). A separate djinn-loop index j
 * (native's djinn loop index sits in r4, caller-saved at sp+4, while i is r6
 * elsewhere): 1148, frame 356. The pixel zero stores after the first
 * window call or inside its arguments: 769, the store register still r3
 * where native reuses the argument's r1. Permuter, 6 minutes on the 769
 * draft: best 750 through temporaries only (noise, not carried).
 * Open: native loads each (s8) modifier twice with ldrb and sign-extends the
 * first with lsl/asr; the draft uses ldrsb and reloads after the store
 * (about 12 rows at each of the four modifier tests); reading them as
 * (s8)(u8) leaves 769. Pixel offsets assigned only when the
 * entry changes (wrong semantics, diagnostic only): 1028, frame still 348.
 * Rejected earlier: owner search on list[n] without
 * code 868; n as the setup loop counter 1298; pixel ints in spill order
 * without the struct 1219.
 *
 * Earlier trials 2026-10-06: searched declaration order alone 1171 -> 1127. The
 * closing busy flag addressed from the battle work slot, as RESOLVE.C does,
 * reuses the native literal (BtlWk, then +192): 1162 alone, 1118 with
 * the searched order. Cursor tables indexed by cur.pos[cur.mode] (native
 * loads by the mode register): 1113. Equipment and condition cursors read
 * straight from their tables (native loads entry by [table, index]): 1087.
 * A separate icon-loop x: 1354 (1330 after search). A separate index for
 * the condition code: 1179. Declarations in native spill-slot order: 1250.
 *
 * Earlier compiler ablation: three old register bindings gave differ 1280.
 * Removing only the redraw binding gave 1331; removing both copier bindings
 * as well gave 1196 and reduced the frame difference from 8 to 4 bytes.
 * Ordinary direct MenuSprite accesses give 1184. Keeping the redraw binding
 * with direct copies gave 1197; direct handle/code arrays gave 1267.
 * Indexed cursor initialization gives 1178, indexed handle initialization
 * 1177, together 1171. Unsigned redraw and block-local saved-unit variables
 * leave 1184 unchanged. The indexed loops preserve the native store bounds.
 * No register bindings or other steering devices remain in this draft.
 */
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "BTLSYS.H"
#include "BTLWORK.H"
#include "MENU_LIST.H"
#include "UI.H"
#include "VRAM_BLOCK.H"
#include "MOTOBJ.H"
#include "MEM.H"
#include "IWRAM_CALL.H"
#include "HEAP_STATE.H"

struct StatusPixel {
    s32 x;
    s32 y;
};

struct StatusCursor {
    s8 pos[5];
    s32 entry;
    s32 lastEntry;
    s32 mode;
    s32 limit;
    s32 col;
    s32 row;
};

extern u8 MsgClassName;
extern char MsgOwnerStatusExp;
extern char MsgOwnerStatusNormal;
extern char MsgOwnerStatusDowned;
extern char MsgOwnerStatusNextLevel;
extern char MsgOwnerStatusEntryHelp;
extern char MsgOwnerStatusConditionHelp;

void UiWindow_MapTextCanvasTiles(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWork_PushValueSlot(s32 value, s32 slot);
void UiWindow_MarkVisibleTileAttributes(void);
void UiWindow_DrawDividerLine(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawNumberInWindow(s32 value, s32 digits, s32 window, s32 x, s32 y);
void PtyStDraw(s32 id);
void Resource_LoadTableEntryToBuffer(s32 icon, s32 handle);
s32 Ui_LoadEntryForKind(s32 owner, s32 handle);
s32 Owner_GetResistanceValueFar(s32 owner, s32 index);
s32 Owner_GetLevelThresholdFar(s32 owner, s32 level);
s32 Party_SumDjinnCountsFar(s32 request);

s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner)
{
    struct MenuSprite objFace;
    struct MenuSprite objCursor;
    s32 tbl[11];
    s8 sel[11];
    struct MenuSprite objIcon[11];
    struct MenuSprite objName;
    struct StatusCursor cur;
    struct UiRenderWork *base;
    struct UiWindow *winMain;
    struct UiWindow *winDesc;
    s32 idx;
    s32 handleCursor;
    struct StatusPixel pix;
    s32 handleFrame;
    s32 cnt;
    s32 extended;
    s32 keys;
    u16 *text;
    s32 iconIdx;
    s32 *q;
    s32 defDelta;
    s32 i;
    s32 j;
    s32 code;
    s32 atkDelta;
    s32 redraw;
    struct BtlUnit *object;
    struct BtlUnit *saved;
    const struct OwnerStatusStatCursor *stat;
    struct MenuSprite *p;
    s32 agiDelta;
    s32 n;
    struct MenuCell *screen;
    s32 x;

    base = (struct UiRenderWork *)gWindowWork[0];
    redraw = 1;
    idx = -1;
    handleFrame = Resource_LoadIntoFreeSlot(0x200);
    cnt = 0;
    extended = Party_SumDjinnCountsFar(-1);

    screen = ((struct MenuCell *)gWindowWork[42]);
    screen->busy = 1;
    if (screen->work != NULL) {
        UiWork_Finalize(screen->work, 1);
        screen->work = NULL;
    }
    AudioCommand_PlayFar(0x70);

    for (i = 4; i >= 0; i--) {
        cur.pos[i] = 0;
    }
    cur.entry = 0;
    cur.lastEntry = 0;
    cur.mode = 0;

    handleCursor = Resource_LoadIntoFreeSlot(0x80);
    for (i = 0; i <= 10; i++) {
        tbl[i] = Resource_LoadIntoFreeSlot(0x80);
        sel[i] = -1;
    }

    if (listCount != 0) {
        for (i = 0; i <= 5; i++) {
            s32 kind = list[i];

            if (kind == 0xFF) {
                break;
            }
            if (kind != 0xFE) {
                if (kind == owner) {
                    idx = i;
                    break;
                }
            }
        }
    }

    pix.x = 0;
    pix.y = 0;
    winMain = UiWindow_Create(0, 0, 30, 20, 6);
    winDesc = UiWindow_Create(0, 14, 30, 6, 10);
    UiWindow_MarkVisibleTileAttributes();

    for (;;) {
        keys = gKeysRepeat;

        objName.oam.word.attr01 = 0x80000400;
        objName.oam.word.attr23 = 0;
        objName.oam.f.tile = Ui_LoadEntryForKind(owner, handleFrame);
        objName.oam.f.x = 8;
        objName.oam.f.y = 24;
        objName.oam.f.palette = 14;
        Runtime_PushSlotEntry((s32 *)&objName, 240);

        switch (cur.entry) {
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        default:
            cur.mode = 0;
            if (extended != 0) {
                cur.limit = 9;
            } else {
                cur.limit = 7;
            }
            break;
        case 10:
        case 11:
        case 12:
        case 13:
            cur.mode = 1;
            cur.limit = 4;
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            cur.mode = 2;
            cur.limit = cnt;
            break;
        }

        Ui_SetRectHighlight(
            winMain->x, winMain->y, winMain->width, winMain->height, 15);

        if (cnt != 0) {
            if ((u32)cur.mode <= 1) {
                n = cur.pos[cur.mode];
                if ((keys & 0x80) != 0) {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    n++;
                    if (n >= cur.limit) {
                        n = 0;
                        if (cur.mode == 1) {
                            cur.mode = 2;
                            n = cur.pos[cur.mode];
                        }
                    }
                } else if ((keys & 0x40) != 0) {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    n--;
                    if (n < 0) {
                        n = cur.limit - 1;
                        if (cur.mode == 1) {
                            cur.mode = 2;
                            n = cur.pos[cur.mode];
                        }
                    }
                } else if ((keys & 0x31) != 0) {
                    keys = 0;
                    AudioCommand_PlayFar(0x6F);
                    cur.mode ^= 2;
                    if ((gKeysRepeat & KEY_A) != 0) {
                        n = cur.pos[cur.mode];
                    } else if ((gKeysRepeat & KEY_LEFT) != 0) {
                        n = cnt - 1;
                    } else {
                        n = 0;
                    }
                }
                cur.pos[cur.mode] = (s8)n;
            } else if (cur.mode == 2) {
                n = cur.pos[cur.mode];
                if (n >= cur.limit) {
                    n = cur.limit - 1;
                }
                if (n < 0) {
                    cur.mode = 0;
                    n = cur.pos[0];
                } else if ((keys & 0x10) != 0) {
                    keys = 0;
                    n++;
                    if (n >= cur.limit) {
                        cur.mode = 0;
                        n = cur.pos[0];
                    }
                    AudioCommand_PlayFar(0x6F);
                } else if ((keys & 0x20) != 0) {
                    keys = 0;
                    n--;
                    if (n < 0) {
                        cur.mode = 0;
                        n = cur.pos[0];
                    }
                    AudioCommand_PlayFar(0x6F);
                } else if ((keys & 0xC1) != 0) {
                    keys = 0;
                    cur.mode = 0;
                    n = cur.pos[0];
                    AudioCommand_PlayFar(0x6F);
                }
                cur.pos[cur.mode] = (s8)n;
            }
        }

        if (cur.mode == 0) {
            s32 k = cur.pos[cur.mode];

            if (extended == 0) {
                k += 9;
            }
            stat = &OwnerStatus_StatCursors[k];
            cur.entry = stat->entry;
            cur.col = stat->column;
            cur.row = stat->row;
            Ui_SetRectHighlight(
                winMain->x + stat->highlight_column + 1, winMain->y + stat->highlight_row + 1, stat->highlight_width, 1,
                14);
        } else if (cur.mode == 1) {
            cur.entry = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].entry;
            cur.col = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].column;
            cur.row = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].row;
        } else if (cur.mode == 2) {
            cur.entry = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].entry;
            cur.col = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].column;
            cur.row = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].row;
        }

        if (cur.lastEntry != cur.entry) {
            cur.lastEntry = cur.entry;
            redraw = 2;
        }
        pix.x = cur.col * 8;
        pix.y = cur.row * 8;

        {
            struct MenuSprite *face = GetBattleObjectSlotFar(owner)->object->records;

            objFace.oam.word.attr01 = 0xC0002400;
            objFace.oam.word.attr23 = 0;
            objFace.oam.f.tile = face->oam.f.tile;
            objFace.oam.f.x = 0xAC;
            objFace.oam.f.y = 56;
            Runtime_PushSlotEntry((s32 *)&objFace, 240);
        }

        objCursor.oam.word.attr01 = 0x40000400;
        objCursor.oam.word.attr23 = 0;
        /* FAKEMATCH: arm_reorg counts this empty asm as 4 bytes, so the gFrameCount literal falls past the face x literal's 64-byte ldrh range and opens the next pool, as native; the pool and its branch move two instructions earlier, 85 -> 63 */
        asm("");
        objCursor.oam.f.tile = Resource_GetBuffer(handleCursor, (s32)Resource_FixedBlockBTiles);
        objCursor.oam.f.x = (pix.x + winMain->x * 8
                          - ((gFrameCount & 4) >> 2))
            + 16;
        objCursor.oam.f.y = (u8)((pix.y + winMain->y * 8
                               - ((gFrameCount & 4) >> 2))
            + 16);
        objCursor.oam.f.affine_index = 8;
        Runtime_PushSlotEntry((s32 *)&objCursor, 241);

        if (redraw != 0) {
            object = UnitGetFar(owner);
            UiWindow_MarkVisibleTileAttributes();
            Ui_FillVramBlockPattern();

            if ((redraw & 1) != 0) {
                RenderOutput_RedrawSavedRect(winMain);
                UiWindow_MapTextCanvasTiles(winDesc->x, winDesc->y, winDesc->width,
                    winDesc->height, 0);
                UiWindow_DrawDividerLine(winMain, 0, 14, 29, 14);

                UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_LevelString, (struct RenderInput *)winMain, 56, 0);
                UiText_DrawNumberInWindow(object->level, 2, (s32)winMain, 72, 0);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp, (struct RenderInput *)winMain, 0, 8);
                UiText_DrawNumberInWindow(
                    object->experience, 8, (s32)winMain, 40, 8);

                UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)winMain, 40, 24);
                UiText_DrawNumberInWindow(
                    object->hp, 4, (s32)winMain, 56, 24);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 24);
                UiText_DrawNumberInWindow(
                    object->max_hp, 4, (s32)winMain, 96, 24);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_PpString, (struct RenderInput *)winMain, 40, 32);
                UiText_DrawNumberInWindow(
                    object->ep, 4, (s32)winMain, 56, 32);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 32);
                UiText_DrawNumberInWindow(
                    object->max_ep, 4, (s32)winMain, 96, 32);

                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 10, (struct RenderInput *)winMain, 136, 16);
                UiText_DrawNumberInWindow(
                    object->atk, 3, (s32)winMain, 184, 16);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 9, (struct RenderInput *)winMain, 136, 24);
                UiText_DrawNumberInWindow(
                    object->def, 3, (s32)winMain, 184, 24);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 8, (struct RenderInput *)winMain, 136, 32);
                UiText_DrawNumberInWindow(
                    object->agi, 3, (s32)winMain, 184, 32);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 7, (struct RenderInput *)winMain, 136, 40);
                UiText_DrawNumberInWindow(object->luck, 3, (s32)winMain, 184, 40);

                UiText_DrawCharacterAtOffset(
                    (s32)&MsgClassName + object->class_index, (struct RenderInput *)winMain, 0, 48);
                if (extended != 0) {
                    UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 1, (struct RenderInput *)winMain, 0, 72);
                }
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 5, (struct RenderInput *)winMain, 0, 80);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 4, (struct RenderInput *)winMain, 0, 88);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 3, (struct RenderInput *)winMain, 0, 96);

                for (j = 0; j <= 3; j++) {
                    s32 pal;

                    pal = 1;
                    if (extended != 0) {
                        pal = 0;
                    }
                    UiWindow_SetTilemapEntry(
                        winMain, 0x5001 + j, 7 + j * 4, pal + 8, 0);
                    if (extended != 0) {
                        UiText_DrawNumberInWindow(object->djinn_active_counts[j], 1,
                            (s32)winMain, 40 + j * 32, 72);
                        UiText_DrawStringAtOffset(
                            (u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 48 + j * 32, 72);
                        UiText_DrawNumberInWindow(object->djinn_owned_counts[j], 1,
                            (s32)winMain, 48 + j * 32 + 8, 72);
                    }
                    UiText_DrawNumberInWindow(Owner_GetResistanceValueFar(owner, j), 2,
                        (s32)winMain, 48 + j * 32, 80);
                    UiText_DrawNumberInWindow(
                        object->elm[j].pow, 3, (s32)winMain,
                        40 + j * 32, 88);
                    UiText_DrawNumberInWindow(
                        object->elm[j].res, 3, (s32)winMain,
                        40 + j * 32, 96);
                }

                i = 0;
                if (object->hp == 0) {
                    sel[0] = 16;
                    i = 1;
                }
                do {
                    if (object->restraint != 0) {
                        sel[i++] = 15;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->shiryo != 0) {
                        sel[i++] = 8;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->doku == 1) {
                        sel[i++] = 1;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->doku == 2) {
                        sel[i++] = 2;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->fuuji != 0) {
                        sel[i++] = 4;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->shibire != 0) {
                        sel[i++] = 3;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->nemuri != 0) {
                        sel[i++] = 5;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->akuryo != 0) {
                        sel[i++] = 7;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->maboroshi != 0) {
                        sel[i++] = 6;
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->atk_cnt != 0) {
                        if (object->atk_lv > 0) {
                            sel[i++] = 9;
                        }
                        if (object->atk_lv < 0) {
                            sel[i++] = 10;
                        }
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->def_cnt != 0) {
                        if (object->def_lv > 0) {
                            sel[i++] = 11;
                        }
                        if (object->def_lv < 0) {
                            sel[i++] = 12;
                        }
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->res_cnt != 0) {
                        if (object->res_lv > 0) {
                            sel[i++] = 13;
                        }
                        if (object->res_lv < 0) {
                            sel[i++] = 14;
                        }
                    }
                    if (i > 7) {
                        break;
                    }
                    if (object->agi_lv > 0) {
                        sel[i++] = 17;
                    }
                    if (object->agi_lv < 0) {
                        sel[i++] = 18;
                    }
                } while (0);

                for (j = 0; j < i; j++) {
                    Resource_LoadTableEntryToBuffer(sel[j], tbl[j]);
                }
                if (i == 0) {
                    sel[0] = 0;
                    i = 1;
                }
                if (i <= 10) {
                    for (j = i; j < 11; j++) {
                        sel[j] = -1;
                    }
                }
                cnt = i;
                if (sel[0] == 0) {
                    UiText_DrawCharacterAtOffset(
                        object->hp != 0 ? (s32)&MsgOwnerStatusNormal : (s32)&MsgOwnerStatusDowned,
                        (struct RenderInput *)winMain, 112, 0);
                }
            }

            text = (u16 *)MemAlloc(0x100);
            if ((u32)cur.entry > 13) {
                iconIdx = sel[cur.entry - 14];
                if (iconIdx == 0 && object->hp == 0) {
                    iconIdx = 16;
                }

                code = 0;
                saved = (struct BtlUnit *)MemAlloc(sizeof(*object));
                Iwram_CopyWords(saved, object, sizeof(*object));

                atkDelta = object->atk;
                defDelta = object->def;
                agiDelta = object->agi;
                object->atk_lv = 0;
                object->def_lv = 0;
                object->agi_lv = 0;
                BattleUnit_Recalculate(owner);
                atkDelta -= object->atk;
                defDelta -= object->def;
                agiDelta -= object->agi;
                Iwram_CopyWords(object, saved, sizeof(*object));
                MemFree(saved);

                switch (iconIdx) {
                case 8:
                    code = object->shiryo;
                    break;
                case 9:
                    code = atkDelta;
                    break;
                case 10:
                    code = -atkDelta;
                    break;
                case 11:
                    code = defDelta;
                    break;
                case 12:
                    code = -defDelta;
                    break;
                case 17:
                    code = agiDelta;
                    break;
                case 18:
                    code = -agiDelta;
                    break;
                case 13:
                case 14:
                    code = object->res_lv * 20;
                    break;
                default:
                    break;
                }
                UiWork_PushValueSlot(code, 5);
                UiText_CopyMessageString(iconIdx + (s32)&MsgOwnerStatusConditionHelp, text, 0x80);
            } else if (cur.entry == 2 && object->level <= 98) {
                UiWork_PushValueSlot(
                    Owner_GetLevelThresholdFar(owner, object->level + 1)
                        - object->experience,
                    5);
                UiText_CopyMessageString((s32)&MsgOwnerStatusNextLevel, text, 0x80);
            } else {
                UiText_CopyMessageString(cur.entry + (s32)&MsgOwnerStatusEntryHelp, text, 0x80);
            }

            UiText_RenderWideStringAtOffset(text, (struct UiWindow *)winDesc, 0, 4);
            MemFree(text);
            redraw = 0;
            base->dirty = 1;
        }

        p = objIcon;
        q = tbl;
        {
            struct VramBlockCacheEntry *vc;

            for (i = 0, vc = gVramBlockCache, x = 112; i <= 10; i++) {
                p->oam.word.attr01 = 0x40000400;
                p->oam.word.attr23 = 0;
                p->oam.f.tile = vc[*q++].offset >> 5;
                p->oam.f.x = x;
                p->oam.f.y = (u8)(winMain->y * 8 + 8);
                if (sel[i] > 0) {
                    Runtime_PushSlotEntry((s32 *)p, 240);
                }
                x += 15;
                p++;
            }
        }

        if (gLinkCountdownWork->active == 0) {
            break;
        }
        if ((gKeyState & KEY_B) != 0) {
            break;
        }

        if (listCount != 0) {
            if ((keys & 0x100) != 0) {
                idx++;
                if (idx >= listCount) {
                    idx = 0;
                }
                owner = list[idx];
                redraw = 1;
                AudioCommand_PlayFar(0x6F);
            } else if ((keys & 0x200) != 0) {
                idx--;
                if (idx < 0) {
                    idx = listCount - 1;
                }
                owner = list[idx];
                redraw = 1;
                AudioCommand_PlayFar(0x6F);
            }
        }
        WaitFrames(1);
    }

    {
        s32 *h = tbl;

        for (i = 10; i >= 0; i--) {
            Resource_ResetEntry(*h++);
        }
    }
    Resource_ResetEntry(handleFrame);
    Resource_ResetEntry(handleCursor);
    WaitFrames(1);
    UiWindow_MarkVisibleTileAttributes();
    UiWork_Finalize(winMain, 1);
    UiWork_Finalize(winDesc, 1);
    PtyStDraw(BtlWk->pty_st);
    ((struct MenuCell **)&BtlWk)[HEAP_SLOT_WINDOW + 42 - HEAP_SLOT_BATTLE]->busy = 0;
    WaitFrames(1);
    return 0;
}
