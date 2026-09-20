#include "TYPES.H"

/* Select a combatant, display its condition and animate the target markers.
 * Confirmation returns an encoded side/index; cancellation returns -1.
 */

#define BattleTarget_RunSelection Func_08026080

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

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

/* Same shape as games/THE BROKEN SEAL/SRC/GRAPHICS/AFFINE/BUILD_MATRIX.C. */
struct Effect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned angle : 16;
    unsigned unused : 16;
};

struct BattleRuntime {
    u8 unknown_00[0x58];
    s16 first_ids[6];            /* 0x58 terminated by 0xFF */
    s16 unknown_64;              /* 0x64 */
    s16 second_ids[6];           /* 0x66 terminated by 0xFF */
};

struct SessionState {
    u8 unknown_00[0x28];
    s32 slide_offset;            /* 0x28 */
    u8 unknown_2c[0x20];
    s32 timer;                   /* 0x4c */
    u8 unknown_50[0x88];
    s32 auto_enabled;            /* 0xd8 */
    s32 auto_delay;              /* 0xdc */
};

struct BattleUnit {
    u8 name[14];
    u8 unknown_00e[0x26];
    s16 max_hp;                  /* 0x034 */
    s16 max_pp;                  /* 0x036 */
    s16 hp;                      /* 0x038 */
    s16 pp;                      /* 0x03a */
    u8 unknown_03c[0xec];
    u8 class_id;                 /* 0x128 */
    u8 unknown_129[8];
    s8 poison;                   /* 0x131 */
    u8 unknown_132[6];
    u8 delusion;                 /* 0x138 */
    u8 confusion;                /* 0x139 */
    u8 charm;                    /* 0x13a */
    u8 stun;                     /* 0x13b */
    u8 sleep;                    /* 0x13c */
    u8 psy_seal;                 /* 0x13d */
    u8 unknown_13e[2];
    u8 evil_spirit;              /* 0x140 */
    u8 death_count;              /* 0x141 */
};

/*
 * The reference derives 0x03001F34 from the 0x03001E74 pool word already in a
 * register ("adds r3, r5, #0 / adds r3, #192") at both prologue sites, so the
 * two pointers are members of one record rather than independent globals.
 */
struct BattleGlobals {
    struct BattleRuntime *runtime;   /* 0x03001e74 */
    u8 unknown_04[0xbc];
    struct SessionState *session;    /* 0x03001f34 */
};

extern struct BattleGlobals Data_03001e74;
/*
 * Read twice inside one frame (0x080263aa and 0x080263cc) without CSE, so it
 * is volatile; the ">> 2" at 0x080263ae is "lsrs", so it is unsigned.
 */
extern volatile u32 Data_03001e40;

#define Resource_LoadIntoFreeSlot Func_080040b4
#define BattleUnit_Get Func_08077008
#define BattleFlag_Test Func_080770c0

s32 Resource_LoadIntoFreeSlot(s32 id);
s32 Resource_GetBuffer(s32 slot, s32 source);
void Runtime_PushSlotEntry(struct DisplayEntry *entry, s32 slot);
s32 AffineMatrix_BuildForEffect(struct Effect *source);
s32 Modulo(s32 numerator, s32 denominator);
s32 Func_08002322(s32 angle);
s32 UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWork_Finalize(s32 work, s32 release);
void UiText_DrawCharacterAtOffset(s32 text, s32 work, s32 x, s32 y);
void UiText_DrawStringAtOffset(s32 text, s32 work, s32 x, s32 y);
void UiText_DrawNumberInWindow(s32 value, s32 digits, s32 work, s32 x, s32 y);
#define UiWork_SetParamNibble Func_0801e71c

void UiWork_SetParamNibble(s32 value);
struct BattleUnit *BattleUnit_Get(s32 id);
s32 BattleFlag_Test(s32 flag);
void Func_080b50b8(s32 id, struct ScreenPos *out);
void Func_080b50e0(u16 *ids, s32 highlight);
void Func_0801965c(s32 message, u16 *text, s32 limit);
s32 UiText_GetWideStringWidth(u16 *text);
void Func_08017aa4(u16 *text, s32 work, s32 x, s32 y);
void Ui_ClearVramBlock(void);
void Resource_ResetEntry(s32 slot);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001b04;

s32 Func_08026080(s32 preferred, s32 mode, u32 spread, u32 kind)
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

    struct BattleRuntime *runtime;
    struct BattleUnit *unit;
    struct CursorSlot *slot;
    struct DisplayEntry *entry;
    s16 *src;
    s32 sel;
    s32 slotId;
    s32 window;
    s32 infoWin;
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

    runtime = Data_03001e74.runtime;
    cnt = 0;
    redraw = 0xFFFF;
    slotId = Resource_LoadIntoFreeSlot(256);
    infoWin = 0;
    sel = preferred;
    if (spread == 0)
        spread = 1;
    if (mode == 2 || mode == 4)
        Data_03001e74.session->slide_offset = -2;
    else
        Data_03001e74.session->slide_offset = 16;

    for (i = 5; i >= 0; i--)
        tbl[i].flags = 0;

    cursor = -1;
    if (mode == 2) {
        src = runtime->first_ids;
        i = 0;
        if (src[0] != 0xFF) {
            do {
                ids[cnt] = src[i];
                cnt++;
                i++;
                if (i > 5)
                    break;
            } while (src[i] != 0xFF);
        }
    } else if (mode == 4) {
        ids[0] = (u16)sel;
        cnt = 1;
    } else {
        src = runtime->second_ids;
        i = 0;
        if (src[0] != 0xFF) {
            do {
                ids[cnt] = src[i];
                cnt++;
                i++;
                if (i > 5)
                    break;
            } while (src[i] != 0xFF);
        }
    }
    ids[cnt] = 0xFF;
    total = cnt;

    if (mode == 2 && spread != 0xFF && kind != 0) {
        for (i = 0; i < cnt; i++) {
            if (ids[i] == 0xFE)
                continue;
            unit = BattleUnit_Get(ids[i]);
            found = 0;
            switch (kind) {
            case 3:
                if (unit->poison != 0)
                    found = 1;
                break;
            case 4:
                if ((*(u32 *)&unit->delusion & 0xFF0000FF) != 0)
                    found = 1;
                else if ((*(u16 *)&unit->sleep & 0xFFFF) != 0)
                    found = 1;
                else if (unit->death_count != 0)
                    found = 1;
                break;
            case 5:
                if (unit->hp == 0)
                    found = 1;
                break;
            case 6:
                if ((*(u32 *)&unit->delusion & 0xFF0000FF) != 0)
                    found = 1;
                else if ((*(u16 *)&unit->sleep & 0xFFFF) != 0)
                    found = 1;
                else if (unit->death_count != 0)
                    found = 1;
                else if (unit->poison != 0)
                    found = 1;
                else if (unit->evil_spirit != 0)
                    found = 1;
                break;
            default:
                break;
            }
            if (found != 0) {
                sel = ids[i];
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

    if (cursor < 0)
        cursor = (cnt - 1) / 2;

    for (;;) {
        if (ids[cursor] == 0xFE)
            goto step_back;
        if (BattleFlag_Test(0x16C) && mode == 1 &&
            BattleUnit_Get(ids[cursor])->hp == 0)
            goto step_back;
        break;
step_back:
        cursor = cursor + cnt - 1;
        cursor = Modulo(cursor, cnt);
    }

    if (mode != 2) {
        Func_080b50b8(sel, &markPos);
        tbl[0].flags = 8;
        tbl[0].x = (u8)markPos.x;
        tbl[0].y = 0x80;
    }

    window = UiWindow_Create(0, 12, 30, 4, 74);

    for (;;) {
        pending = 0;
        Func_080b50b8(ids[cursor], &pos);
        M2C_FIELD(&entries[0], s32 *, 4) = 0x40002000;
        M2C_FIELD(&entries[0], s32 *, 8) = pending;
        entries[0].tile = Resource_GetBuffer(
            slotId, (((Data_03001e40 >> 2) & 31) << 8) + 0x080346F8);
        i = Func_08002322(Data_03001e40 << 12);
        if (i < 0)
            i += 0x7FFF;
        pos.y += i >> 15;
        y = pos.y;
        if (tbl[0].flags & 1) {
            nx = (pos.x + tbl[0].x) / 2;
            ny = (y + tbl[0].y) / 2;
            if (pos.x - nx < 0) {
                if (nx - pos.x <= 7)
                    pending = 1;
            } else if (pos.x - nx <= 7) {
                pending = 1;
            }
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
        entries[0].x = pos.x - 8;
        entries[0].y = (u8)(pos.y - 16);
        Runtime_PushSlotEntry(&entries[0], 240);

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
        for (i = 5; i >= 0; i--)
            tbl[i].flags &= (u8)~2;

        for (i = 0; (u32)i < spread; i++) {
            if (cursor + i < total && ids[cursor + i] != 0xFE) {
                selIds[cnt] = ids[cursor + i];
                tbl[i].flags |= 2;
                if (tbl[i].index != i) {
                    tbl[i].flags &= (u8)~1;
                    tbl[i].index = (s8)i;
                }
                selSlot[cnt] = (u8)i;
                cnt++;
            }
            if (i != 0 && cursor - i >= 0 && ids[cursor - i] != 0xFE) {
                selIds[cnt] = ids[cursor - i];
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

        for (i = 5; i >= 0; i--) {
            if ((tbl[i].flags & 2) == 0)
                tbl[i].index = 6;
        }

        selIds[cnt] = 0xFF;
        Func_080b50e0(selIds, 1);

        if (ids[cursor] > 7)
            goto draw_name;
        if (spread == 0xFF)
            goto frame_tail;
        if (kind == 0)
            goto frame_tail;

        unit = BattleUnit_Get(ids[cursor]);
        Func_080b50b8(ids[cursor], &pos);
        if (infoWin != 0)
            UiWork_Finalize(infoWin, 1);

        switch (kind) {
        case 7:
            column = pos.x / 8 - 4;
            if (pos.x / 8 + 4 > 29)
                column = 22;
            infoWin = UiWindow_Create(column, 8, 9, 3, 6);
            UiWork_SetParamNibble(2);
            UiText_DrawCharacterAtOffset(0x8AC, infoWin, 0, 0);
            UiWork_SetParamNibble(15);
            goto frame_tail;
        case 1:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 6 > 29)
                column = 17;
            infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            UiText_DrawStringAtOffset(0x080373DC, infoWin, 0, 0);
            UiText_DrawNumberInWindow(unit->hp, 4, infoWin, 16, 0);
            UiText_DrawStringAtOffset(0x080373E0, infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_hp, 4, infoWin, 56, 0);
            goto frame_tail;
        case 2:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 6 > 29)
                column = 17;
            infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            UiText_DrawStringAtOffset(0x080373E4, infoWin, 0, 0);
            UiText_DrawNumberInWindow(unit->pp, 4, infoWin, 16, 0);
            UiText_DrawStringAtOffset(0x080373E0, infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_pp, 4, infoWin, 56, 0);
            goto frame_tail;
        case 5:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 5 > 29)
                column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            if (unit->hp != 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset(0x8AB, infoWin, 0, 0);
            goto frame_tail;
        case 3:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 5 > 29)
                column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            if (unit->poison == 0)
                goto no_condition;
            UiText_DrawCharacterAtOffset(0x8A4, infoWin, 0, 0);
            /* 0x080267f6: b sub_08026b8c. */
            goto frame_tail;
        case 4:
            count = 0;
            if (unit->delusion != 0)
                count = 1;
            if (unit->stun != 0)
                count++;
            if (unit->sleep != 0)
                count++;
            if (unit->psy_seal != 0)
                count++;
            if (unit->death_count != 0)
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
            if (unit->delusion != 0) {
                UiText_DrawCharacterAtOffset(0x8A5, infoWin, 0, 0);
                count = 1;
            }
            if (unit->stun != 0) {
                UiText_DrawCharacterAtOffset(0x8A6, infoWin, 0, count * 8);
                count++;
            }
            if (unit->sleep != 0) {
                UiText_DrawCharacterAtOffset(0x8A7, infoWin, 0, count * 8);
                count++;
            }
            if (unit->psy_seal != 0) {
                UiText_DrawCharacterAtOffset(0x8A8, infoWin, 0, count * 8);
                count++;
            }
            if (unit->death_count != 0) {
                UiText_DrawCharacterAtOffset(0x8A9, infoWin, 0, count * 8);
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
                UiText_DrawCharacterAtOffset(0x8A4, infoWin, 0, 0);
                count = 1;
            }
            if (unit->delusion != 0) {
                UiText_DrawCharacterAtOffset(0x8A5, infoWin, 0, count * 8);
                count++;
            }
            if (unit->stun != 0) {
                UiText_DrawCharacterAtOffset(0x8A6, infoWin, 0, count * 8);
                count++;
            }
            if (unit->sleep != 0) {
                UiText_DrawCharacterAtOffset(0x8A7, infoWin, 0, count * 8);
                count++;
            }
            if (unit->psy_seal != 0) {
                UiText_DrawCharacterAtOffset(0x8A8, infoWin, 0, count * 8);
                count++;
            }
            if (unit->death_count != 0) {
                UiText_DrawCharacterAtOffset(0x8A9, infoWin, 0, count * 8);
                count++;
            }
            if (unit->evil_spirit != 0) {
                UiText_DrawCharacterAtOffset(0x8AA, infoWin, 0, count * 8);
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
        UiText_DrawCharacterAtOffset(0x8A3, infoWin, 0, 0);
        UiWork_SetParamNibble(15);
        goto frame_tail;

draw_name:
        if (spread == 0xFF)
            goto frame_tail;
        unit = BattleUnit_Get(ids[cursor]);
        Func_080b50b8(ids[cursor], &namePos);
        namePos.y += Func_08002322(Data_03001e40 << 12) / 32768;
        if (unit->class_id == 125 || unit->class_id == 122) {
            i = 0x80E;
            if (unit->class_id == 125)
                i++;
            Func_0801965c(i, name, 14);
        } else {
            for (i = 0; i <= 13; ) {
                j = unit->name[i];
                name[i] = j;
                i++;
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
        Func_08017aa4(name, window, namePos.x, 4);

frame_tail:
        redraw &= ~1;
frame_end:
        if (pending != 0) {
            entry = entries + 1;
            for (i = 1; i < cnt; i++, entry++) {
                slot = &tbl[selSlot[i]];
                Func_080b50b8(selIds[i], &targetPos);
                targetPos.y += Func_08002322(Data_03001e40 << 12) / 32768;
                *entry = entries[0];
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
                Runtime_PushSlotEntry(entry, 240);
            }
        }

        pressed = Data_03001c94;
        repeat = Data_03001b04;
        if (Data_03001e74.session->auto_enabled != 0) {
            pressed = 0;
            repeat = 0;
            if (Data_03001e74.session->auto_delay == 0) {
                Data_03001e74.session->auto_delay = 60;
                pressed = 1;
                repeat = 1;
            } else {
                Data_03001e74.session->auto_delay--;
            }
        }
        if (pressed & 1) {
            sel = ids[cursor];
            redraw = 0;
            result = -1;
            for (i = 0; i <= 5 && runtime->first_ids[i] != 0xFF; i++) {
                if (runtime->first_ids[i] == sel) {
                    result = 0x100 | i;
                    break;
                }
            }
            if (result < 0) {
                for (i = 0; i <= 5 && runtime->second_ids[i] != 0xFF; i++) {
                    if (runtime->second_ids[i] == sel) {
                        result = 0x180 | i;
                        break;
                    }
                }
            }
            cursor = result;
        } else if (spread != 0xFF) {
            if (repeat & 0x90) {
                Audio_PlayCue(111);
                do {
                    cursor++;
                    cursor = Modulo(cursor, total);
                } while (ids[cursor] == 0xFE);
                redraw |= 1;
            }
            if (repeat & 0x60) {
                Audio_PlayCue(111);
                do {
                    cursor = cursor + total - 1;
                    cursor = Modulo(cursor, total);
                } while (ids[cursor] == 0xFE);
                redraw |= 1;
            }
        }
        if (Data_03001e74.session->timer == 0 || (pressed & 2)) {
            Audio_PlayCue(113);
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
    Func_080b50e0(ids, 0);
    Data_03001e74.session->slide_offset = 0;
    WaitFrames(1);
    return cursor;
}
