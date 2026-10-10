/* Pin-free BattleTarget_RunSelection trial: EN/JA 1583 native instructions,
 * 1585 built, 1535 common, differ 98. The value-preserving null initializer
 * already used by ES/IT keeps the native 324-byte frame in EN too; ordinary
 * zero spills infoWin, shrinks the frame to 320 and differs 492. Here sel
 * takes r9 instead of native r10 and infoWin takes r10 instead of r9, with
 * no spill-slot changes after using head[i] directly in the frame-end loop.
 * The 2-minute seed-367 search found this ordinary source reshape in 1308
 * compiled candidates. Keeping a local entry pointer, even scoped to that
 * loop, differs 200; moving its assignment after projection differs 475.
 * Using mode, spread or kind in the same
 * null initializer still differs 200. Register qualification and an infoWin
 * input or output on the existing t barrier also leave 200. The linked
 * declaration retains its r9 binding until all six editions match. */
#include "dma.h"
#include "io_reg.h"
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
#include "callback_scheduler.h"
#include "resource.h"
#include "menu_list.h"
#include "item.h"
#include "text/msg_ids.h"
#include "btltype.h"
#include "btlsys.h"
#include "affine.h"
#include "select.h"
#include "effect_step.h"
#include "btlwork.h"
#include "fixed_math.h"
#include "ui.h"
#include "heap_state.h"
#include "edition.h"
#include "scene.h"
#include "sound_ids.h"
#include "tbs_edition.h"
#include "game_state.h"
#include "uiflow.h"


struct BtlAct * BtlActGet(s32 action_id);

#define UiWindow_DrawThreeTileColumn ((void (*)(struct UiWindow*,s32,s32,s32,s32))UiWindow_DrawThreeTileColumn)

#define ACTION_ID_MASK 0x3fff
struct CursorSlot {
    u8 x;
    u8 y;
    u8 flags;
    s8 index;
};

s32 BattleMotion_ProjectConditionalPositionFar(s32 id, s32 *out);
s32 UiText_GetWideStringWidth(u16 *text);
void UiText_DrawNumberInWindow(
    s32 value, s32 digits, s32 work, s32 x, s32 y);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
s32 System_GetLanguage(void);
#endif

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
    struct UiWindow *infoWin;
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
    /* FAKEMATCH: the known-zero initializer keeps EN's native 324-byte frame; literal zero shrinks it to 320. */
    infoWin = (struct UiWindow *)((u32)(u16)sel >> 16);
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
    while (ids[cursor] == 0xFE
           || (GameFlag_TestFar(0x16C) && mode == 1 && UnitGetFar(ids[cursor])->hp == 0)) {
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
        {
            s32 t = cursor * 2;

            /* FAKEMATCH: instruction order only: the empty asm takes cursor * 2 from its spill slot into r3 before the ids address is loaded, as the native does; through ids[cursor] both are reloads of the one load, base first (differ 2). */
            __asm__ volatile ("" : "+r" (t));
            BattleMotion_ProjectConditionalPositionFar(*(u16 *)((u8 *)ids + t), (s32 *)&pos);
        }
        ((s32 *)head)[1] = 0x40002000;
        ((s32 *)head)[2] = 0;
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
        /* FAKEMATCH: register choice (spill slots) only: seventeen dead
           stores, two insns each until flow deletes them. GCSE sizes its
           expression table from the insn count (n / 2 | 1 buckets) and
           PRE numbers its reaching registers in bucket order, which is the
           order their spill slots are assigned; with these the count is
           1451 and the table 725 buckets, so tbl, entries, ids and
           cursor * 2 take sp+36, 32, 28 and 24 before &efx (20) and
           selSlot (16) as in the native frame (1417 and 709 buckets
           without them: every remaining row is a spill slot's offset;
           ES and IT use two additional copies below to restore the slot
           order after their value-preserving null initializer). */
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
        efx.angle = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
        /* FAKEMATCH: these two additional dead stores are needed to match
         * the native spill slots; without them ES differs 136 instructions. */
        efx.angle = 0;
        efx.angle = 0;
#endif
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
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 4;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                if (System_GetLanguage() == 4) {
                    if (anchor + 6 > 29)
                        column = 20;
                    infoWin = UiWindow_Create(column, 8, 10, 3, 6);
                } else if (System_GetLanguage() == 3) {
                    if (anchor + 7 > 29)
                        column = 19;
                    infoWin = UiWindow_Create(column, 8, 11, 3, 6);
                } else {
                    if (anchor + 5 > 29)
                        column = 21;
                    infoWin = UiWindow_Create(column, 8, 9, 3, 6);
                }
#elif defined(TBS_EDITION_FR)
                if (anchor + 5 > 29)
                    column = 21;
                infoWin = UiWindow_Create(column, 8, 9, 3, 6);
#elif defined(TBS_EDITION_JA)
                if (anchor + 4 > 29)
                    column = 22;
                infoWin = UiWindow_Create(column, 8, 8, 3, 6);
#else
                if (anchor + 4 > 29)
                    column = 22;
                infoWin = UiWindow_Create(column, 8, 9, 3, 6);
#endif
            }
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset((s32)MsgTargetSetDjinni, (struct RenderInput *)infoWin, 0, 0);
            UiWork_SetParamNibble(15);
            goto frame_tail;
        case 1:
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
                if (anchor + 6 > 29)
                    column = 17;
                infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            }
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            switch (System_GetLanguage()) {
            case 1:
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_KpString, (struct RenderInput *)infoWin, 0, 0);
                break;
            case 2:
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_PvString, (struct RenderInput *)infoWin, 0, 0);
                break;
            case 3:
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_PvString, (struct RenderInput *)infoWin, 0, 0);
                break;
            case 4:
                UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)infoWin, 0, 0);
                break;
            }
#else
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_HpString, (struct RenderInput *)infoWin, 0, 0);
#endif
            UiText_DrawNumberInWindow(unit->hp, 4, (s32)infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_hp, 4, (s32)infoWin, 56, 0);
            goto frame_tail;
        case 2:
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
                if (anchor + 6 > 29)
                    column = 17;
                infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            }
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_PpString, (struct RenderInput *)infoWin, 0, 0);
            UiText_DrawNumberInWindow(unit->ep, 4, (s32)infoWin, 16, 0);
            UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_ep, 4, (s32)infoWin, 56, 0);
            goto frame_tail;
        case 5:
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
                if (anchor + 5 > 29)
                    column = 18;
                infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            }
            if (unit->hp != 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset((s32)MsgTargetDowned, (struct RenderInput *)infoWin, 0, 0);
            goto frame_tail;
        case 3:
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
                if (anchor + 5 > 29)
                    column = 18;
                infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            }
            if (unit->doku == 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset((s32)MsgTargetCurePoison, (struct RenderInput *)infoWin, 0, 0);
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
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                if (System_GetLanguage() == 3) {
                    if (anchor + 10 > 29)
                        column = 13;
                    infoWin = UiWindow_Create(column, rows, 17, count + 2, 6);
                } else {
                    if (anchor + 9 > 29)
                        column = 14;
                    infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
                }
#elif defined(TBS_EDITION_JA)
                if (anchor + 7 > 29)
                    column = 16;
                infoWin = UiWindow_Create(column, rows, 14, count + 2, 6);
#else
                if (anchor + 9 > 29)
                    column = 14;
                infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->maboroshi != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureDelusion, (struct RenderInput *)infoWin, 0, 0);
                count = 1;
            }
            if (unit->shibire != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureStun, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->nemuri != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureSleep, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->fuuji != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureSeal, (struct RenderInput *)infoWin, 0, count * 8);
                ++count;
            }
            if (unit->shiryo != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureCurse, (struct RenderInput *)infoWin, 0, count * 8);
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
            {
                s32 anchor;

                anchor = pos.x / 8;
                column = anchor - 7;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
                if (System_GetLanguage() == 3) {
                    if (anchor + 10 > 29)
                        column = 13;
                    infoWin = UiWindow_Create(column, rows, 17, count + 2, 6);
                } else {
                    if (anchor + 9 > 29)
                        column = 14;
                    infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
                }
#elif defined(TBS_EDITION_JA)
                if (anchor + 7 > 29)
                    column = 16;
                infoWin = UiWindow_Create(column, rows, 14, count + 2, 6);
#else
                if (anchor + 9 > 29)
                    column = 14;
                infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
#endif
            }
            count = 0;
            if (unit->doku != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCurePoison, (struct RenderInput *)infoWin, 0, 0);
                count = 1;
            }
            if (unit->maboroshi != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureDelusion, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->shibire != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureStun, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->nemuri != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureSleep, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->fuuji != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureSeal, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->shiryo != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureCurse, (struct RenderInput *)infoWin, 0, count * 8);
                count++;
            }
            if (unit->akuryo != 0) {
                UiText_DrawCharacterAtOffset((s32)MsgTargetCureHaunt, (struct RenderInput *)infoWin, 0, 8 * count);
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
        UiText_DrawCharacterAtOffset((s32)MsgTargetNoEffect, (struct RenderInput *)infoWin, 0, 0);
        UiWork_SetParamNibble(15);
        goto frame_tail;
    draw_name:
        if (spread == 0xFF)
            goto frame_tail;
        who = UnitGetFar(ids[cursor]);
        BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&namePos);
        namePos.y += Trig_Sin(gFrameCount << 12) / 32768;
        if (who->class_id == 125 || who->class_id == 122) {
            width = (s32)MsgMysteryPersonName;
            if (who->class_id == 125)
                width++;
#if defined(TBS_EDITION_JA)
            UiText_DecodeMessage(width, name, 10);
#else
            UiText_CopyMessageString(width, name, 14);
#endif

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
                slot = &tbl[selSlot[i]];
                BattleMotion_ProjectConditionalPositionFar(selIds[i], (s32 *)&targetPos);
                targetPos.y = targetPos.y + Trig_Sin(gFrameCount << 12) / 32768;
                head[i] = *head;
                if (slot->flags & 1) {
                    s32 x = (targetPos.x + slot->x) / 2;
                    s32 y = (targetPos.y + slot->y) / 2;

                    targetPos.x = x;
                    slot->x = x;
                    targetPos.y = y;
                    slot->y = y;
                } else {
                    s32 x = head[i].x;
                    s32 y = head[i].y + 8;

                    slot->flags = 1;
                    targetPos.x = x;
                    slot->x = x;
                    targetPos.y = y;
                    slot->y = y;
                }
                head[i].mode = 1;
                head[i].x = targetPos.x - 8;
                head[i].y = targetPos.y - 12;
                if (spread == 0xFF)
                    head[i].affine = 0;
                else
                    head[i].affine = 1;
                head[i].param = matrix;
                Runtime_PushSlotEntry((s32 *)&head[i], 240);
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
