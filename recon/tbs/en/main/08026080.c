/* Draft, not exact: retained EN score16188 (413 register-only,58 stack-only,
 * 97 operand,74 reordered,45 inserted,29 deleted); frame320 vs native324.
 * Every symbol resolves. Complete native extent3584 includes all pools.
 * Ordinary routed compilation emits3612 bytes including pools.
 * Retained devices bind only the live window and selected combatant.
 * No frame storage, masks, compiler options or output bytes were added;
 * no adoption or edition credit is claimed.
 * 2026-10-02 continuation:
 * H4: unsigned eight-bit cursor flags: identical score21123/counts/frame320;
 * the field type alone changes no generated operation. Reverted.
 * H5: both six-record passes walk a pointer while their count descends:
 * score21171 (461 register,46 stack,88 operand,79 reordered,70 inserted,
 * 50 deleted), frame320. Worse; indexed passes restored.
 * H6: sleep/psy_seal as the low half of the four-byte condition group:
 * identical21123/counts/frame320. Retained; the mask excludes refrain and
 * reflect rather than redundantly masking an already unsigned halfword.
 * H7: sole live infoWin binding to native r9: score18857 (429 register,
 * 47 stack,108 operand,83 reordered,56 inserted,36 deleted), frame320.
 * Retained; fourteen inserted and fourteen deleted differences disappear.
 * H8: setup-mode copy bound to r8: identical18857/counts/frame320;
 * the compiler propagates the argument instead. Copy and binding removed.
 * H9: existing selected-combatant local bound to native setup r10:
 * score16188 (413 register,58 stack,97 operand,74 reordered,45 inserted,
 * 29 deleted), frame320. Retained with the live window r9 binding.
 * H10: separate setup and confirmation selections into their real scopes:
 * first compile exposed the confirmation use; with its ordinary local,
 * score17290 (412 register,55 stack,107 operand,82 reordered,44 inserted,
 * 34 deleted), frame320. Worse; single consumed selection restored.
 * Remaining: compiler spills/register order, native eligibility AND masks,
 * table/spread allocation, branches and complete literal-pool placement.
 */
/* Previous bounded trials closed with H0 real-owner body and no devices.
 * H1: five pointers scoped inside case 4; identical score/counts/frame to H0.
 * H2: actual sleep/psy_seal pair as unsigned16 bitfield; identical to H0,
 * still no native mask/spill; removed the exploratory view.
 * H3: case-owned used pointer registers r7/r8/r10/r6, after scope failed:
 * score 21188 (442 register, 48 stack, 85 operand, 80 reordered,
 * 70 inserted, 51 deleted), frame still 320. Worse; all bindings removed.
 * No unused storage, dead instructions, compiler options or output edits.
 * Native complete English listing is 3584 bytes, including all pools.
 * The current candidate is not exact; no adoption or six-edition credit.
 */
/* H0 result: score 21123 (450 register, 48 stack, 85 operand,
 * 80 reordered, 70 inserted, 50 deleted), frame 320; all symbols resolve.
 * The real-owner/interface repair is retained regardless of matching score.
 * EN status literals now use the existing physical OwnerStatus string names.
 */
/* Current uncorrected EN baseline: score 21249 (446 register, 49 stack,
 * 89 operand, 78 reordered, 73 inserted, 49 deleted); frame 320 vs native
 * 324. Modulo was unresolved. H0 removes fabricated global/record owners,
 * imports shared declarations, uses real signed remainder and explicit
 * window/text/sprite boundary casts. No storage or steering device added.
 */
/* 2026-10-02 source ownership and frame audit, before new scoring:
 * BATTLE_WORK.H owns the BattleSession pointer and its party_units/enemy_units
 * at +58/+66. The former bundled BattleGlobals conflates two named pointer
 * cells: the target-selection menu view belongs behind gLinkCountdownWork,
 * with slide offset +28, countdown +4c and auto input +d8/+dc.
 * BATTLE_UNIT.H owns the combatant/status fields; TEXT_RENDER_RUNTIME.H owns
 * byte-string render input. Shared declaration repairs are root-owned.
 * Native frame 324: efx 8; four projected positions 48; name 30 plus alignment
 * 2; selection slots 8 and ids 28; cursor records 24; sprites 72; base ids 16;
 * remaining 88 bytes are compiler spills. Each aggregate has a real use.
 * Case 4 keeps five used status pointers across UiWindow_Create: delusion r7,
 * stun r8, sleep r10, psy_seal at spill+40, death_count r6. The old candidate
 * header reports recomputation and one extra spill; remeasure after the
 * compiler cache is ready. Try actual used lifetimes, never frame filler.
 * Modulo has no current definition; native calls the signed remainder helper
 * __modsi3 at all three sites, which should be written as C remainder.
 */
#include "TYPES.H"
#include "BATTLE_WORK.H"
#include "BATTLE_RUNTIME.H"
#include "MENU_LIST.H"
#include "FIXED_MATH.H"
#include "UI.H"

/* Select a combatant, display its condition and animate the target markers.
 * Confirmation returns an encoded side/index; cancellation returns -1.
 *
 * Draft, not exact: `alchemy drafts` scores it 784 instructions off of
 * 1,585 (the field macro with a type argument kept it from scoring before).
 * Every call site is in the ROM's order. Proven from the ROM:
 * the id copies read runtime->party_units[i] directly (movs r3, #88; ldrsh);
 * the status search and the sel lookup both sit inside if (mode == 2), with
 * case 5 setting sel itself; the two per-frame tbl loops ascend (GCC counts
 * r7 down while the pointer walks up); the frame is 88 bytes of spills
 * below efx (sp+88), targetPos 96, namePos 108, name 120, pos 152,
 * selSlot 164, selIds 172, markPos 200, tbl 212, entries 236, ids 308.
 * Remaining: the candidate needs one more spill slot (every local sits 4
 * bytes higher); infoWin lives in r9 in the ROM; case 4 keeps the five
 * status-byte addresses in r7, r8, sl, [sp+40] and r6 across
 * UiWindow_Create where the candidate recomputes them; the 0xFFFF mask of
 * the sleep halfword is hoisted into r4 (ldrh; ands r3, r4); ids[cursor+i]
 * in the spread loop is indexed, not strength-reduced.
 */


/* Object list entry chained by Runtime_PushSlotEntry; ordinary GBA OAM. */
struct DisplayEntry {
    struct DisplayEntry *next;   /* 0x00 written by Runtime_PushSlotEntry */
    u8 y;                        /* 0x04 */
    u8 object_mode : 2;          /* 0x05 */
    u8 gfx_mode : 2;
    u8 mosaic : 1;
    u8 color_mode : 1;
    u8 shape : 2;
    u16 x : 9;                   /* 0x06 */
    u16 matrix : 5;
    u16 size : 2;
    u16 tile : 10;               /* 0x08 */
    u16 priority : 2;
    u16 palette : 4;
    u16 pad_0a;                  /* 0x0a */
};

/* One tracked cursor position; index 0 is the cursor, 1..5 the side marks. */
struct CursorSlot {
    u8 x;                        /* 0x00 */
    u8 y;                        /* 0x01 */
    u8 flags;                    /* 0x02 bit0 = tracking, bit1 = in use */
    s8 index;                    /* 0x03 signed step this slot follows */
};

struct ScreenPos {
    s32 x;
    s32 y;
    s32 z;
};

/* Same shape as games/THE BROKEN SEAL/SRC/GRAPHICS/DISPLAY/BUILD_MATRIX.C. */
struct Effect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned angle : 16;
    unsigned unused : 16;
};

/* Views whose callee declarations have no shared owner yet. */
s32 AffineMatrix_BuildForEffect(struct Effect *source);
s32 BattleMotion_ProjectConditionalPositionFar(s32 id, s32 *out);
void BattlePres_SetActorModesFar(u16 *ids, s32 highlight);
s32 UiText_GetWideStringWidth(u16 *text);
void UiText_DrawNumberInWindow(
    s32 value, s32 digits, s32 work, s32 x, s32 y);
void Ui_ClearVramBlock(void);


s32 BattleTarget_RunSelection(s32 preferred, s32 mode, u32 spread, u32 kind)
{
    u16 ids[8];
    struct DisplayEntry entries[6];
    struct CursorSlot tbl[6];
    struct ScreenPos markPos;
    u16 selIds[14];
    u8 selSlot[8];
    struct ScreenPos pos;
    u16 name[15];
    struct ScreenPos namePos;
    struct ScreenPos targetPos;
    struct Effect efx;

    struct BattleSession *runtime;
    struct BattleUnit *unit;
    struct CursorSlot *slot;
    struct DisplayEntry *entry;
    struct DisplayEntry *head;
    /* FAKEMATCH: Live sel in r10; scope trials failed, score18857 ->16188. */
    register s32 sel asm("r10");
    s32 slotId;
    struct UiWindow *window;
    /* FAKEMATCH: Live infoWin in r9; walks failed, score21123 ->18857. */
    register struct UiWindow *infoWin asm("r9");
    s32 matrix;
    s32 cursor;
    s32 cnt;
    s32 total;
    s32 redraw;
    s32 pending;
    s32 found;
    s32 column;
    s32 rows;
    s32 count;
    s32 i;
    s32 j;
    s32 nx;
    s32 ny;
    s32 y;
    s32 width;
    s32 pressed;
    s32 repeat;
    s32 result;
    u8 *pd;
    u8 *ps;
    u8 *pl;
    u8 *pp;
    u8 *pc;

    runtime = gBattleWork;
    cnt = 0;
    redraw = 0xFFFF;
    slotId = Resource_LoadIntoFreeSlot(256);
    infoWin = 0;
    sel = preferred;
    if (spread == 0)
        spread = 1;
    if (mode == 2 || mode == 4)
        gLinkCountdownWork->slide_offset = -2;
    else
        gLinkCountdownWork->slide_offset = 16;

    for (i = 5; i >= 0; i--)
        tbl[i].flags = 0;

    cursor = -1;
    if (mode == 2) {
        i = 0;
        if (runtime->party_units[0] != 0xFF) {
            do {
                ids[cnt] = runtime->party_units[i];
                cnt++;
                i++;
                if (i > 5)
                    break;
            } while (runtime->party_units[i] != 0xFF);
        }
    } else if (mode == 4) {
        ids[0] = (u16)sel;
        cnt = 1;
    } else {
        i = 0;
        if (runtime->enemy_units[0] != 0xFF) {
            do {
                ids[cnt] = runtime->enemy_units[i];
                cnt++;
                i++;
                if (i > 5)
                    break;
            } while (runtime->enemy_units[i] != 0xFF);
        }
    }
    ids[cnt] = 0xFF;
    total = cnt;

    if (mode == 2) {
        if (spread != 0xFF && kind != 0) {
            found = 0;
            for (i = 0; i < cnt; i++) {
                if (ids[i] == 0xFE)
                    continue;
                unit = Owner_GetStateFar(ids[i]);
                switch (kind) {
                case 3:
                    if (unit->poison != 0)
                        found = 1;
                    break;
                case 4:
                    if ((*(u32 *)&unit->delusion & 0xFF0000FF) != 0)
                        found = 1;
                    else if ((*(u32 *)&unit->sleep & 0xFFFF) != 0)
                        found = 1;
                    else if (unit->death_count != 0)
                        found = 1;
                    break;
                case 5:
                    if (unit->hp == 0) {
                        sel = ids[i];
                        found = 1;
                    }
                    break;
                case 6:
                    if ((*(u32 *)&unit->delusion & 0xFF0000FF) != 0)
                        found = 1;
                    else if ((*(u32 *)&unit->sleep & 0xFFFF) != 0)
                        found = 1;
                    else if (unit->death_count != 0)
                        found = 1;
                    else if (unit->poison != 0)
                        found = 1;
                    else if (unit->evil_spirit != 0)
                        found = 1;
                    break;
                }
                if (found != 0) {
                    sel = ids[i];
                    break;
                }
            }
        }
        for (i = 0; i < cnt; i++) {
            if (ids[i] == sel)
                break;
        }
        if (i != cnt)
            cursor = i;
    }

    if (cursor < 0)
        cursor = (cnt - 1) / 2;

    for (;;) {
        if (ids[cursor] == 0xFE)
            goto step_back;
        if (GameFlag_TestFar(0x16C) && mode == 1 &&
            Owner_GetStateFar(ids[cursor])->hp == 0)
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
    head = entries;

    for (;;) {
        pending = 0;
        BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&pos);
        ((s32 *)head)[1] = 0x40002000;
        ((s32 *)head)[2] = pending;
        head->tile = Resource_GetBuffer(
            slotId, (((gFrameCount >> 2) & 31) << 8) + (s32)Menu_AnimatedCursorTiles);
        i = Trig_Sin(gFrameCount << 12);
        if (i < 0)
            i += 0x7FFF;
        pos.y += i >> 15;
        y = pos.y;
        if (tbl[0].flags & 1) {
            nx = (pos.x + tbl[0].x) / 2;
            ny = (y + tbl[0].y) / 2;
            if (((pos.x - nx) < 0 ? -(pos.x - nx) : (pos.x - nx)) <= 7)
                pending = 1;
            pos.x = nx;
            pos.y = ny;
            tbl[0].x = (u8)nx;
            tbl[0].y = (u8)ny;
        } else if ((u8)tbl[0].flags <= 3) {
            nx = pos.x;
            pos.y = y;
            tbl[0].x = (u8)nx;
            tbl[0].y = (u8)y;
            tbl[0].flags = 1;
        } else {
            pos.x = tbl[0].x;
            pos.y = tbl[0].y;
            tbl[0].flags = (u8)(tbl[0].flags - 4);
            if ((u8)tbl[0].flags <= 3)
                tbl[0].flags = 1;
        }
        head->x = pos.x - 8;
        head->y = (u8)(pos.y - 16);
        Runtime_PushSlotEntry((s32 *)head, 240);

        if (spread == 0xFF) {
            efx.x = 256;
            efx.y = 256;
        } else {
            efx.x = 176;
            efx.y = 176;
        }
        efx.angle = 0;
        matrix = AffineMatrix_BuildForEffect(&efx);

        if ((redraw & 1) == 0)
            goto frame_end;

        cnt = 0;
        for (i = 0; i < 6; i++)
            tbl[i].flags &= (u8)~2;

        for (i = 0; (u32)i < spread; i++) {
            j = cursor + i;
            if (j < total && ids[j] != 0xFE) {
                selIds[cnt] = ids[j];
                tbl[i].flags |= 2;
                if (tbl[i].index != i) {
                    tbl[i].flags &= (u8)~1;
                    tbl[i].index = (s8)i;
                }
                selSlot[cnt] = (u8)i;
                cnt++;
            }
            if (i != 0 && (j = cursor - i) >= 0 && ids[j] != 0xFE) {
                selIds[cnt] = ids[j];
                slot = &tbl[6 - i];
                slot->flags |= 2;
                if (slot->index != -i) {
                    slot->flags &= (u8)~1;
                    slot->index = (s8)-i;
                }
                selSlot[cnt] = (u8)(6 - i);
                cnt++;
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

        unit = Owner_GetStateFar(ids[cursor]);
        BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&pos);
        if (infoWin != 0)
            UiWork_Finalize(infoWin, 1);

        switch (kind) {
        case 7:
            column = pos.x / 8 - 4;
            if (pos.x / 8 + 4 > 29)
                column = 22;
            infoWin = UiWindow_Create(column, 8, 9, 3, 6);
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(0x8AC, (struct TextRenderWork *)infoWin, 0, 0);
            UiWork_SetParamNibble(15);
            goto frame_tail;
        case 1:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 6 > 29)
                column = 17;
            infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct TextRenderWork *)infoWin, 0, 0);
            UiText_DrawNumberInWindow(unit->hp, 4, (s32)infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct TextRenderWork *)infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_hp, 4, (s32)infoWin, 56, 0);
            goto frame_tail;
        case 2:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 6 > 29)
                column = 17;
            infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_PpString, (struct TextRenderWork *)infoWin, 0, 0);
            UiText_DrawNumberInWindow(unit->pp, 4, (s32)infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct TextRenderWork *)infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_pp, 4, (s32)infoWin, 56, 0);
            goto frame_tail;
        case 5:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 5 > 29)
                column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            if (unit->hp != 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset(0x8AB, (struct TextRenderWork *)infoWin, 0, 0);
            goto frame_tail;
        case 3:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 5 > 29)
                column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            if (unit->poison == 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset(0x8A4, (struct TextRenderWork *)infoWin, 0, 0);
            /* 0x080267f6: b sub_08026b8c. */
            goto frame_tail;
        case 4:
            count = 0;
            if (*(pd = &unit->delusion) != 0)
                count = 1;
            if (*(ps = &unit->stun) != 0)
                count++;
            if (*(pl = &unit->sleep) != 0)
                count++;
            if (*(pp = &unit->psy_seal) != 0)
                count++;
            if (*(pc = &unit->death_count) != 0)
                count++;
            if (count == 0)
                count = 1;
            rows = 9 - count;
            if (rows <= 3)
                rows = 4;
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 9 > 29)
                column = 14;
            infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
            count = 0;
            if (*pd != 0) {
                UiText_DrawCharacterAtOffset(0x8A5, (struct TextRenderWork *)infoWin, 0, 0);
                count = 1;
            }
            if (*ps != 0) {
                UiText_DrawCharacterAtOffset(0x8A6, (struct TextRenderWork *)infoWin, 0, count * 8);
                count++;
            }
            if (*pl != 0) {
                UiText_DrawCharacterAtOffset(0x8A7, (struct TextRenderWork *)infoWin, 0, count * 8);
                count++;
            }
            if (*pp != 0) {
                UiText_DrawCharacterAtOffset(0x8A8, (struct TextRenderWork *)infoWin, 0, count * 8);
                count++;
            }
            if (*pc != 0) {
                UiText_DrawCharacterAtOffset(0x8A9, (struct TextRenderWork *)infoWin, 0, count * 8);
                count++;
            }
            if (count == 0)
                goto no_condition;
            goto frame_tail;
        case 6:
            count = 0;
            if (unit->poison != 0)
                count = 1;
            if (unit->delusion != 0)
                count++;
            if (unit->stun != 0)
                count++;
            if (unit->sleep != 0)
                count++;
            if (unit->psy_seal != 0)
                count++;
            if (unit->death_count != 0)
                count++;
            if (unit->evil_spirit != 0)
                count++;
            if (count == 0)
                count = 1;
            rows = 9 - count;
            if (rows <= 3)
                rows = 4;
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 9 > 29)
                column = 14;
            infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
            count = 0;
            if (unit->poison != 0) {
                UiText_DrawCharacterAtOffset(0x8A4, (struct TextRenderWork *)infoWin, 0, 0);
                count = 1;
            }
            if (unit->delusion != 0) {
                UiText_DrawCharacterAtOffset(0x8A5, (struct TextRenderWork *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->stun != 0) {
                UiText_DrawCharacterAtOffset(0x8A6, (struct TextRenderWork *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->sleep != 0) {
                UiText_DrawCharacterAtOffset(0x8A7, (struct TextRenderWork *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->psy_seal != 0) {
                UiText_DrawCharacterAtOffset(0x8A8, (struct TextRenderWork *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->death_count != 0) {
                UiText_DrawCharacterAtOffset(0x8A9, (struct TextRenderWork *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->evil_spirit != 0) {
                UiText_DrawCharacterAtOffset(0x8AA, (struct TextRenderWork *)infoWin, 0, count * 8);
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
        UiText_DrawCharacterAtOffset(0x8A3, (struct TextRenderWork *)infoWin, 0, 0);
        UiWork_SetParamNibble(15);
        goto frame_tail;

draw_name:
        if (spread == 0xFF)
            goto frame_tail;
        unit = Owner_GetStateFar(ids[cursor]);
        BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&namePos);
        namePos.y += Trig_Sin(gFrameCount << 12) / 32768;
        if (unit->class_id == 125 || unit->class_id == 122) {
            width = 0x80E;
            if (unit->class_id == 125)
                width++;
            UiText_CopyMessageString(width, name, 14);
        } else {
            for (i = 0; i <= 13; i++) {
                name[i] = j = unit->name[i];
                if (j == 0)
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
        UiText_RenderWideStringAtOffset(name, (struct TextWindow *)window, namePos.x, 4);

frame_tail:
        redraw &= ~1;
frame_end:
        if (pending != 0) {
            entry = head + 1;
            for (i = 1; i < cnt; i++, entry++) {
                slot = &tbl[selSlot[i]];
                BattleMotion_ProjectConditionalPositionFar(selIds[i], (s32 *)&targetPos);
                targetPos.y += Trig_Sin(gFrameCount << 12) / 32768;
                *entry = *head;
                if (slot->flags & 1) {
                    targetPos.x = (targetPos.x + slot->x) / 2;
                    targetPos.y = (targetPos.y + slot->y) / 2;
                    slot->x = targetPos.x;
                    slot->y = targetPos.y;
                } else {
                    targetPos.x = entry->x;
                    targetPos.y = entry->y + 8;
                    slot->flags = 1;
                    slot->x = targetPos.x;
                    slot->y = targetPos.y;
                }
                entry->gfx_mode = 1;
                entry->x = targetPos.x - 8;
                entry->y = targetPos.y - 12;
                if (spread == 0xFF)
                    entry->object_mode = 0;
                else
                    entry->object_mode = 1;
                entry->matrix = matrix;
                Runtime_PushSlotEntry((s32 *)entry, 240);
            }
        }

        pressed = gKeyState;
        repeat = gKeysRepeat;
        if (gLinkCountdownWork->auto_enabled != 0) {
            pressed = 0;
            repeat = 0;
            if (gLinkCountdownWork->auto_delay == 0) {
                gLinkCountdownWork->auto_delay = 60;
                pressed = 1;
                repeat = 1;
            } else {
                gLinkCountdownWork->auto_delay--;
            }
        }
        if (pressed & 1) {
            sel = ids[cursor];
            redraw = 0;
            result = -1;
            for (i = 0; i <= 5 && runtime->party_units[i] != 0xFF; i++) {
                if (runtime->party_units[i] == sel) {
                    result = 0x100 | i;
                    break;
                }
            }
            if (result < 0) {
                for (i = 0; i <= 5 && runtime->enemy_units[i] != 0xFF; i++) {
                    if (runtime->enemy_units[i] == sel) {
                        result = 0x180 | i;
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
