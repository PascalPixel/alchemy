/* 08023178: Ui_RunOwnerStatusScreen draft.
 * Native 1392 instructions, draft 1391, common 1339, differ 105.
 * Frame 352 bytes, as native. The setup loops, the cursor update, the
 * face and cursor sprites (0x3FF in r8, the zero in r7, as native),
 * the stat pushes, the djinn rows and the condition list match.
 * Remaining: the four (s8)m < 0 modifier tests (lsr #7, beq where
 * native has lsl #24, bge); the condition loads' sign extension
 * (movs/ldrsb where native has ldrb, lsl, asr); the help block's
 * saved/pointer pair (r5/r6 swapped) and code = 0 set before
 * Runtime_BumpAllocate where native sets it after; the up-key n--
 * scheduled before the 0x6F argument; one literal pool placed two
 * instructions later; and register choice in the icon loop setup.
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
 * each argument; native keeps 332 in r8 across Runtime_BumpAllocate).
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
 * code = 0 before Runtime_BumpAllocate instead of after it 331 -> 120
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
 * stores and 332 in r8 across Runtime_BumpAllocate). A 5-minute permuter
 * run (--heat, 2 jobs) reached 700 by random rewrites; its minimization
 * did not finish, so none is carried over.
 * Rejected earlier: owner search on list[n] without
 * code 868; n as the setup loop counter 1298; pixel ints in spill order
 * without the struct 1219.
 *
 * Earlier trials 2026-10-06: searched declaration order alone 1171 -> 1127. The
 * closing busy flag addressed from the battle work slot, as RESOLVE.C does,
 * reuses the native literal (gBattleWork, then +192): 1162 alone, 1118 with
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
#include "BATTLE_RUNTIME.H"
#include "BATTLE_WORK.H"
#include "MENU_LIST.H"
#include "UI.H"
#include "VRAM_BLOCK.H"
#include "MOTION_OBJECT.H"
#include "RUNTIME_MEM.H"
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
void UiWindow_DrawPartyStatusContents(s32 id);
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
    s8 *out;
    s32 code;
    s32 atkDelta;
    s32 redraw;
    struct BattleUnit *object;
    struct BattleUnit *saved;
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
                    n--;
                    AudioCommand_PlayFar(0x6F);
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
            object = Owner_GetStateFar(owner);
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
                    object->pp, 4, (s32)winMain, 56, 32);
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 32);
                UiText_DrawNumberInWindow(
                    object->max_pp, 4, (s32)winMain, 96, 32);

                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 10, (struct RenderInput *)winMain, 136, 16);
                UiText_DrawNumberInWindow(
                    object->attack, 3, (s32)winMain, 184, 16);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 9, (struct RenderInput *)winMain, 136, 24);
                UiText_DrawNumberInWindow(
                    object->defense, 3, (s32)winMain, 184, 24);
                UiText_DrawCharacterAtOffset((s32)&MsgOwnerStatusExp - 8, (struct RenderInput *)winMain, 136, 32);
                UiText_DrawNumberInWindow(
                    object->agility, 3, (s32)winMain, 184, 32);
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
                        object->elements[j].power, 3, (s32)winMain,
                        40 + j * 32, 88);
                    UiText_DrawNumberInWindow(
                        object->elements[j].resist, 3, (s32)winMain,
                        40 + j * 32, 96);
                }

                i = 0;
                if (object->hp == 0) {
                    sel[0] = 16;
                    i = 1;
                }
                {
                    s8 *o = &sel[i];

                    do {
                        if (object->restraint != 0) {
                            *o++ = 15;
                            i++;
                        }
                        if (i > 7) {
                            break;
                        }
                        if (object->death_count != 0) {
                            *o++ = 8;
                            i++;
                        }
                        if (i > 7) {
                            break;
                        }
                        if (object->poison == 1) {
                            *o++ = 1;
                            i++;
                        }
                        if (i > 7) {
                            break;
                        }
                        if (object->poison == 2) {
                            *o++ = 2;
                            i++;
                        }
                        if (i > 7) {
                            break;
                        }
                        if (object->psy_seal != 0) {
                            *o++ = 4;
                            i++;
                        }
                        if (i > 7) {
                            break;
                        }
                        if (object->stun != 0) {
                            *o++ = 3;
                            i++;
                        }
                        if (i > 7) {
                            break;
                        }
                        if (object->sleep != 0) {
                            *o++ = 5;
                            i++;
                        }
                        if (i > 7) {
                            break;
                        }
                        if (object->evil_spirit != 0) {
                            *o++ = 7;
                            i++;
                        }
                        if (i > 7) {
                            break;
                        }
                        if (object->delusion != 0) {
                            *o++ = 6;
                            i++;
                        }
                        if (i > 7) {
                            break;
                        }
                        if (object->attack_modifier_turns != 0) {
                            u8 m = object->attack_modifier;

                            if ((s8)m > 0) {
                                *o++ = 9;
                                i++;
                            }
                            if ((m >> 7) != 0) {
                                *o++ = 10;
                                i++;
                            }
                        }
                        if (i > 7) {
                            break;
                        }
                        if (object->defense_modifier_turns != 0) {
                            u8 m = object->defense_modifier;

                            if ((s8)m > 0) {
                                *o++ = 11;
                                i++;
                            }
                            if ((m >> 7) != 0) {
                                *o++ = 12;
                                i++;
                            }
                        }
                        if (i > 7) {
                            break;
                        }
                        if (object->res_modifier_turns != 0) {
                            u8 m = object->res_modifier;

                            if ((s8)m > 0) {
                                *o++ = 13;
                                i++;
                            }
                            if ((m >> 7) != 0) {
                                *o++ = 14;
                                i++;
                            }
                        }
                        if (i > 7) {
                            break;
                        }
                        {
                            u8 m = object->agility_modifier;

                            if ((s8)m > 0) {
                                *o++ = 17;
                                i++;
                            }
                            if ((m >> 7) != 0) {
                                *o = 18;
                                i++;
                            }
                        }
                    } while (0);
                }

                if (i > 0) {
                    j = i;
                    out = sel;
                    q = tbl;
                    do {
                        Resource_LoadTableEntryToBuffer(*out, *q++);
                        j--;
                        out++;
                    } while (j != 0);
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

            text = (u16 *)Runtime_BumpAllocate(0x100);
            if ((u32)cur.entry > 13) {
                iconIdx = sel[cur.entry - 14];
                if (iconIdx == 0 && object->hp == 0) {
                    iconIdx = 16;
                }

                code = 0;
                saved = (struct BattleUnit *)Runtime_BumpAllocate(sizeof(*object));
                Iwram_CopyWords(saved, object, sizeof(*object));

                atkDelta = object->attack;
                defDelta = object->defense;
                agiDelta = object->agility;
                object->attack_modifier = 0;
                object->defense_modifier = 0;
                object->agility_modifier = 0;
                BattleUnit_Recalculate(owner);
                atkDelta -= object->attack;
                defDelta -= object->defense;
                agiDelta -= object->agility;
                Iwram_CopyWords(object, saved, sizeof(*object));
                Sys_Free(saved);

                switch (iconIdx) {
                case 8:
                    code = object->death_count;
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
                    code = object->res_modifier * 20;
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
            Sys_Free(text);
            base->dirty = 1;
            redraw = 0;
        }

        q = tbl;
        p = objIcon;
        x = 112;
        for (i = 0; i <= 10; i++) {
            p->oam.word.attr01 = 0x40000400;
            p->oam.word.attr23 = 0;
            p->oam.f.tile = gVramBlockCache[*q++].offset >> 5;
            p->oam.f.x = x;
            p->oam.f.y = (u8)(winMain->y * 8 + 8);
            if (sel[i] > 0) {
                Runtime_PushSlotEntry((s32 *)p, 240);
            }
            x += 15;
            p++;
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

    q = tbl;
    for (i = 10; i >= 0; i--) {
        Resource_ResetEntry(*q++);
    }
    Resource_ResetEntry(handleFrame);
    Resource_ResetEntry(handleCursor);
    WaitFrames(1);
    UiWindow_MarkVisibleTileAttributes();
    UiWork_Finalize(winMain, 1);
    UiWork_Finalize(winDesc, 1);
    UiWindow_DrawPartyStatusContents(gBattleWork->party_status_mode);
    ((struct MenuCell **)&gBattleWork)[HEAP_SLOT_WINDOW + 42 - HEAP_SLOT_BATTLE]->busy = 0;
    WaitFrames(1);
    return 0;
}
