/* Stubborn (2026-10-06, sun100 T10 session 1 on 8b95cd7680): differ 1335 (listing 1585, draft 1604, common 927). Not first-try; parked. */
/* 08026080: listing 1585, draft 1604, differ 1335; both frames are 324 bytes.
 * 2026-10-06 session 1 (was 1332/1599): two structural fixes kept although
 * the count moved +3 in the register cascade. The kind switch of the
 * preferred-target scan lists case 5 first: the native lays its body out
 * right after the dispatch, before 3, 4 and 6 (GCC keeps source order).
 * The two slide_offset stores at the top go through slots = &gBattleWork
 * (RESOLVE.C's pattern) only there: the native then loads gBattleWork once
 * and reaches slot 57 as base + 192; the loop and the end keep the direct
 * gLinkCountdownWork literal, as the native does. With these, the first
 * difference is allocation: native preferred r10 and mode r8, draft r6/r10.
 * Native sel is not one pseudo for the whole function: r10 is reused in
 * the main loop and the A-press search holds the id in r4, so the press
 * branch probably reads a separate block variable, but measured worse.
 * Native ANDs the sleep/psy_seal halfword (ldrh) with 0xFFFF hoisted into
 * r4 before the scan loop; agscc's Thumb andsi3 turns a constant 0xFFFF
 * mask into extzv (no ands), so the native mask was not a CONST_INT at
 * expand time; the source form is still unknown.
 * Session 1 trials (base 1335 unless noted): (u16) cast of the sleep word
 * 1335; sel r10 device 1363; head declared after matrix or last 1335,
 * after window 1401, first 1409; no head (entries directly) 1438; ids/tbl
 * as pointer locals over buffers 1432-1442; gWorkSlot[9]/[57] at the top
 * 1341, everywhere 1352; &gWorkSlot[9] base 1335; slots + sel parameter
 * 1342; slots + mode r8 1335; slots + mode r8 + sel r10 1361; press search
 * through j 1377, block-local id 1361, nx 1377, width 1393; permuter
 * 5 min seed 11 --heat: weighted 29540 -> 24980 but differ 1455.
 * Earlier header follows.
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
 * Neither reproduced the native reuse of the gBattleWork cell base then;
 * the top-only slots form above (session 1) does.
 */
#include "AFFINE.H"
#include "SELECT.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "BATTLE_WORK.H"
#include "BATTLE_RUNTIME.H"
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

s32 BattleTarget_RunSelection(s32 preferred, s32 mode, u32 spread, u32 kind)
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
    struct BattleUnit *unit;
    struct BattleSession *runtime;
    struct UiWindow *window;
    s32 sel;
    register struct UiWindow *infoWin asm("r9"); /* FAKEMATCH: the ordinary window lifetime differs 1464; native r9 reduces it to 1400 without changing instruction count. */
    s32 cursor;
    s32 cnt;
    s32 total;
    s32 redraw;
    s32 slotId;
    s32 pending;
    s32 matrix;
    s32 found;
    s32 column;
    s32 rows;
    s32 count;
    s32 i;
    s32 j;
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
    u8 *pd;
    u8 *ps;
    u8 *pl;
    u8 *pp;
    u8 *pc;
    void **slots;

    slots = (void **)&gBattleWork;
    runtime = slots[0];
    redraw = 0xFFFF;
    cnt = 0;
    slotId = Resource_LoadIntoFreeSlot(256);
    infoWin = 0;
    if (spread == 0)
        spread = 1;
    if (mode == 2 || mode == 4)
        ((struct MenuCell *)slots[57 - HEAP_SLOT_BATTLE])->slide_offset = -2;
    else
        ((struct MenuCell *)slots[57 - HEAP_SLOT_BATTLE])->slide_offset = 16;
    i = 5;
    while (i >= 0) {
        tbl[i].flags = 0;
        i--;
    }
    sel = preferred;
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
                /* FAKEMATCH: keep the consumed enemy-id load separate;
                   folding it differs 1493 and adds five instructions, versus 1400 here. */
                s16 enemy;
                enemy = runtime->enemy_units[i];
                ids[cnt] = enemy;
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
                case 5:
                    if (unit->hp == 0) {
                        sel = ids[i];
                        found = 1;
                    }
                    break;
                case 3:
                    if (unit->poison != 0)
                        found = 1;
                    break;
                case 4:
                    if (*(u32 *)&unit->delusion & 0xFF0000FF)
                        found = 1;
                    else if ((*(u32 *)&unit->sleep & 0xFFFF) != 0)
                        found = 1;
                    else if (unit->death_count != 0)
                        found = 1;
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
    while (1) {
        if (ids[cursor] == 0xFE)
            goto step_back;
        if (GameFlag_TestFar(0x16C) && mode == 1 && Owner_GetStateFar(ids[cursor])->hp == 0)
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
    while (1) {
        pending = 0;
        BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&pos);
        ((s32 *)head)[1] = 0x40002000;
        ((s32 *)head)[2] = pending;
        head->tile = Resource_GetBuffer(slotId, (((gFrameCount >> 2) & 31) << 8) + (s32)Menu_AnimatedCursorTiles);
        if ((i = Trig_Sin(gFrameCount << 12)) < 0)
            i = i + 0x7FFF;
        pos.y += i >> 15;
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
            nx = pos.x;
            tbl[0].x = (u8)nx;
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
        i = 0;
        if ((u32)i < spread) {
            do {
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
                    /* FAKEMATCH: schedule this consumed side-marker index
                       before the slot writes; folding it differs 1445 versus 1400 here. */
                    u8 mark;
                    selIds[cnt] = ids[j];
                    mark = (u8)(6 - i);
                    slot = &tbl[6 - i];
                    selSlot[cnt] = mark;
                    slot->flags |= 2;
                    if (slot->index != -i) {
                        slot->flags &= (u8)~1;
                        slot->index = (s8)-i;
                    }
                    cnt++;
                }
                i++;
            } while ((u32)i < spread);
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
            /* FAKEMATCH: share the consumed coordinate temporary between
               these cases; ordinary direct expressions differ 1410 versus 1400 here. */
            s32 anchor;
        case 7:
            column = pos.x / 8 - 4;
            if (pos.x / 8 + 4 > 29)
                column = 22;
            infoWin = UiWindow_Create(column, 8, 9, 3, 6);
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(0x8AC, (struct RenderInput *)infoWin, 0, 0);
            UiWork_SetParamNibble(15);
            goto frame_tail;
        case 1:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 6 > 29)
                column = 17;
            infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)infoWin, 0, 0);
            UiText_DrawNumberInWindow(unit->hp, 4, (s32)infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_hp, 4, (s32)infoWin, 56, 0);
            goto frame_tail;
        case 2:
            anchor = pos.x;
            column = anchor / 8 - 7;
            if (pos.x / 8 + 6 > 29)
                column = 17;
            infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_PpString, (struct RenderInput *)infoWin, 0, 0);
            UiText_DrawNumberInWindow(unit->pp, 4, (s32)infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_pp, 4, (s32)infoWin, 56, 0);
            goto frame_tail;
        case 5:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 5 > 29)
                column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            if (unit->hp != 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset(0x8AB, (struct RenderInput *)infoWin, 0, 0);
            goto frame_tail;
        case 3:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 5 > 29)
                column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            if (unit->poison == 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset(0x8A4, (struct RenderInput *)infoWin, 0, 0);
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
            anchor = pos.x / 8;
            column = anchor - 7;
            if (pos.x / 8 + 9 > 29)
                column = 14;
            infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
            count = 0;
            if (*pd != 0) {
                UiText_DrawCharacterAtOffset(0x8A5, (struct RenderInput *)infoWin, 0, 0);
                count = 1;
            }
            if (*ps != 0) {
                UiText_DrawCharacterAtOffset(0x8A6, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (*pl != 0) {
                UiText_DrawCharacterAtOffset(0x8A7, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (*pp != 0) {
                UiText_DrawCharacterAtOffset(0x8A8, (struct RenderInput *)infoWin, 0, count * 8);
                ++count;
            }
            if (*pc != 0) {
                UiText_DrawCharacterAtOffset(0x8A9, (struct RenderInput *)infoWin, 0, count * 8);
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
                UiText_DrawCharacterAtOffset(0x8A4, (struct RenderInput *)infoWin, 0, 0);
                count = 1;
            }
            if (unit->delusion != 0) {
                UiText_DrawCharacterAtOffset(0x8A5, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->stun != 0) {
                UiText_DrawCharacterAtOffset(0x8A6, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->sleep != 0) {
                UiText_DrawCharacterAtOffset(0x8A7, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->psy_seal != 0) {
                UiText_DrawCharacterAtOffset(0x8A8, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->death_count != 0) {
                UiText_DrawCharacterAtOffset(0x8A9, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->evil_spirit != 0) {
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
                if (!j)
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
            entry = head + 1;
            for (i = 1; i < cnt; i++, entry++) {
                slot = &tbl[selSlot[i]];
                BattleMotion_ProjectConditionalPositionFar(selIds[i], (s32 *)&targetPos);
                targetPos.y = targetPos.y + Trig_Sin(gFrameCount << 12) / 32768;
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
                    cursor = total + cursor - 1;
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
