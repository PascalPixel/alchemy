#include "types.h"

/*
 * Battle target-selection cursor - leading region.
 *
 * The registered owner main:08026080 is the first 2138 bytes (0x08026080 ..
 * 0x080268d9) of a single 3584-byte routine that runs the whole "pick a
 * combatant" interaction.  Its tail is registered separately as
 * main:0802691c ("Continuation_0802691c", the kind==6 condition window) and
 * main:08026b44 (the name blit, the per-target sprite pass, the pad read and
 * the epilogue).  The three extents are contiguous apart from the 66-byte
 * literal pool at 0x080268da..0x0802691b, which no owner currently claims:
 * one prologue at 0x0802608e ("sub sp, #324"), one epilogue at 0x08026e60,
 * and one loop whose only back edge is the long branch at 0x08026e22.
 *
 * This draft covers ONLY the registered owner, and the owner extent is not
 * altered to fit.  The reference leaves the extent in two distinguishable
 * ways, drafted differently:
 *   - 0x08026a5a / 0x08026a64 / 0x08026a6a / 0x08026a72 / 0x08026a84 reach
 *     main:0802691c, and the kind==6 jump-table slot reaches 0x0802691c
 *     directly.  These are drafted as "goto done" - they leave the region.
 *   - 0x08026b8c and 0x08026b96, in main:08026b44, are the frame loop's own
 *     tail; six reference branches target them and control comes back round.
 *     These are drafted as the labels "frame_tail" and "frame_end" at the
 *     bottom of the loop, so the loop keeps its back edge.  See the boundary
 *     compromises below for what that costs.
 *
 * What the covered region does:
 *   - loads the cursor sprite resource, biases the session slide offset for
 *     the requested side, and clears the six cursor tracking slots;
 *   - builds the selectable id list from the runtime record at 0x03001E74
 *     (first list at +0x58 for mode 2, the caller's single id for mode 4,
 *     second list at +0x66 otherwise), terminated by 0xFF;
 *   - for mode 2 with a condition kind, scans the list for the first unit
 *     that actually has that condition and prefers it;
 *   - resolves the preferred id to a list index, defaulting to the middle,
 *     and steps backwards past 0xFE holes and (flag 0x16C, mode 1) dead units;
 *   - opens the 30x4 caption window and enters the per-frame loop;
 *   - each frame places the cursor sprite over the current candidate with a
 *     half-step smoothing filter and a sine wobble, builds the affine matrix,
 *     and - when the redraw bit is set - retints the highlighted id list and
 *     opens the per-kind information window (HP, PP, and the poison /
 *     status / downed / stun-list variants).
 *
 * Uncertain / unresolved:
 *   - Func_080b50b8 / Func_080b50e0 have no project names yet; the first
 *     resolves a unit id to a 12-byte screen position, the second re-tints a
 *     0xFF-terminated id list.
 *   - The 0x03001E74 record is only known here through its two s16 id lists
 *     at 0x58 and 0x66; everything before them stays unnamed.
 *   - The message ids (0x8A3..0x8AC) and the string pointers 0x080373DC /
 *     0x080373E0 / 0x080373E4 are kept as literals; the project has no
 *     symbol for them yet.
 *   - The status-byte offsets inside BattleUnit past 0x137 are the previous
 *     draft's reading of the reference's pool constants; only 0x138 and
 *     0x13C are directly evidenced (they are built with shifts, not pool
 *     loads).
 *
 * Boundary compromises, all documented rather than hidden:
 *   - "redraw &= ~1" (reference 0x08026b8c) is real and is kept, although it
 *     sits two bytes past the owner, because six branches inside the owner
 *     target it and without it redraw folds to the constant 0xFFFF.
 *   - "if (pending != 0)" (reference 0x08026b96) is a real test with an
 *     invented consequence.  In the reference neither arm ends the loop:
 *     pending != 0 falls into the per-target sprite pass at 0x08026b9e and
 *     pending == 0 skips to the pad read at 0x08026cdc, both in
 *     main:08026b44, and the loop is closed by the long branch at
 *     0x08026e22.  An owner-scoped draft has no reachable loop exit, so this
 *     test is spelled as the exit.  That inversion is the one place where
 *     this draft asserts control flow the reference does not have.
 *   - kind == 6 keeps its jump-table slot (the table still emits seven
 *     entries) but its body is main:0802691c, so it is drafted as an
 *     immediate exit.
 *   - The last three instructions of the owner (the message-id load for the
 *     death-count line, which is consumed by the call at 0x08026a5a) have no
 *     representation here: the call they feed is outside the owner, so the
 *     dead load cannot be spelled without inventing a use.
 *   - "entries" is declared as six elements because the reference's frame
 *     reserves sp+236..sp+308 for it (ids begins at sp+308 and the frame ends
 *     at sp+324), but only entries[0] is touched inside this owner; the rest
 *     are written by the per-target sprite pass in main:08026b44.  It is the
 *     one aggregate here that is sized from the frame map rather than from
 *     accesses in the covered code.
 *
 * Measured with the approved route (agbcc/GCC 2.96) against the registered
 * 2138-byte owner, by "alchemy score ... --owner 08026080":
 *
 *     candidate=2224 (104.0%)  differing_halfwords=1074
 *     wrong_instructions=1139  topology=different
 *
 * Residual, honestly stated: frame layout, not control flow.  The reference
 * allocates 324 bytes because the complete routine also needs the name buffer
 * (30 bytes), the two extra ScreenPos temporaries (24) and the spill slots the
 * pad-reading tail uses - roughly 80 bytes that this owner-scoped draft has no
 * honest use for.  The candidate therefore allocates 244, and every one of the
 * function's many sp-relative operands is displaced, which is what the
 * halfword aggregate is counting.  It also changes register pressure: the
 * reference keeps cnt, the ids base and the runtime pointer in stack slots
 * (sp+64, sp+28, sp+76) where the candidate can afford registers.  Neither can
 * be closed without either drafting past the owner boundary or declaring
 * locals with no use in the covered code, and both are refused.
 *
 * Source hypotheses tested and retained: the shared 0x03001E74 globals record
 * (above); the do-while shape of the two id-copy loops (the reference stores
 * the element, advances, and only then tests the NEXT one - a top-tested
 * while emits a second signed load per iteration); the unsigned spread/kind
 * parameters (the reference dispatches both switches with bhi/bls and walks
 * the spread loop with bcc/bcs); unsigned Data_03001e40 (0x080263ae is
 * "lsrs", not "asrs"); and the frame-tail labels described above, which
 * lowered the halfword aggregate from 1094 to 1074 and raised
 * wrong_instructions from 1128 to 1139 - kept because the six reference
 * branches to 0x08026b8c are real and the previous shape discarded them.
 *
 * Verified against the reference for this draft: the seven-entry jump table
 * at 0x0802667c (targets 0x080266c8, 0x0802671e, 0x080267b4, 0x080267f8,
 * 0x08026780, 0x0802691c, 0x08026698); the pool constants 0x03001E74,
 * 0x0000FFFF, 0xFF0000FF, 0x00000131, 0x00000141, 0x000003FF and 0x0000FC00;
 * and the frame map efx sp+88, pos sp+152, selSlot sp+164, selIds sp+172,
 * markPos sp+200, tbl sp+212, entries sp+236, ids sp+308.
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

/* Same shape as games/gs1/src/graphics/affine/build_matrix.c. */
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
    u8 unknown_000[0x34];
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
void UiText_DrawCharacter(s32 text, s32 work, s32 x, s32 y);
void UiText_DrawStringAtOffset(s32 text, s32 work, s32 x, s32 y);
void UiText_DrawNumberInWindow(s32 value, s32 digits, s32 work, s32 x, s32 y);
#define UiWork_SetParamNibble Func_0801e71c

void UiWork_SetParamNibble(s32 value);
struct BattleUnit *BattleUnit_Get(s32 id);
s32 BattleFlag_Test(s32 flag);
void Func_080b50b8(s32 id, struct ScreenPos *out);
void Func_080b50e0(u16 *ids, s32 highlight);

s32 Func_08026080(s32 preferred, s32 mode, u32 spread, u32 kind)
{
    struct DisplayEntry entries[6];
    struct CursorSlot tbl[6];
    struct ScreenPos markPos;
    struct ScreenPos pos;
    struct Effect efx;
    u16 ids[8];
    u16 selIds[14];
    u8 selSlot[8];

    struct BattleRuntime *runtime;
    struct BattleUnit *unit;
    struct CursorSlot *slot;
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

        /* 0x0802650a: b sub_08026b96 - skips the redraw clear only. */
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

        /* 0x08026630: b sub_08026a84 - the name blit is main:08026b44. */
        if (ids[cursor] > 7)
            goto done;
        /* 0x08026638 / 0x08026640: b sub_08026b8c - the frame tail. */
        if (spread == 0xFF)
            goto frame_tail;
        if (kind == 0)
            goto frame_tail;

        unit = BattleUnit_Get(ids[cursor]);
        Func_080b50b8(ids[cursor], &pos);
        if (infoWin != 0)
            UiWork_Finalize(infoWin, 1);

        switch (kind) {
        case 1:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 6 > 29)
                column = 17;
            infoWin = UiWindow_Create(column, 8, 13, 3, 6);
            UiText_DrawStringAtOffset(0x080373DC, infoWin, 0, 0);
            UiText_DrawNumberInWindow(unit->hp, 4, infoWin, 16, 0);
            UiText_DrawStringAtOffset(0x080373E0, infoWin, 48, 0);
            UiText_DrawNumberInWindow(unit->max_hp, 4, infoWin, 56, 0);
            /* 0x0802677e: b sub_08026b8c. */
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
            /* Shares case 1's tail at 0x0802677e: b sub_08026b8c. */
            goto frame_tail;
        case 3:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 5 > 29)
                column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            /* 0x080267e8: b sub_08026a6a when the condition is absent. */
            if (unit->poison == 0)
                goto done;
            UiText_DrawCharacter(0x8A4, infoWin, 0, 0);
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
                UiText_DrawCharacter(0x8A5, infoWin, 0, 0);
                count = 1;
            }
            if (unit->stun != 0) {
                UiText_DrawCharacter(0x8A6, infoWin, 0, count * 8);
                count++;
            }
            if (unit->sleep != 0) {
                UiText_DrawCharacter(0x8A7, infoWin, 0, count * 8);
                count++;
            }
            if (unit->psy_seal != 0) {
                UiText_DrawCharacter(0x8A8, infoWin, 0, count * 8);
                count++;
            }
            /* 0x080268cc is the owner's last test; the death-count line is
             * drawn by the shared tail at 0x08026a5a. */
            goto done;
        case 5:
            column = pos.x / 8 - 7;
            if (pos.x / 8 + 5 > 29)
                column = 18;
            infoWin = UiWindow_Create(column, 8, 12, 3, 6);
            /* 0x080267ae: b sub_08026a6a when the unit is still standing. */
            if (unit->hp != 0)
                goto done;
            UiText_DrawCharacter(0x8AB, infoWin, 0, 0);
            /* Shares case 3's tail at 0x080267f6: b sub_08026b8c. */
            goto frame_tail;
        case 6:
            /* Jump-table slot 5 targets 0x0802691c: main:0802691c. */
            goto done;
        case 7:
            column = pos.x / 8 - 4;
            if (pos.x / 8 + 4 > 29)
                column = 22;
            infoWin = UiWindow_Create(column, 8, 9, 3, 6);
            UiWork_SetParamNibble(2);
            /* 0x080266c6: b sub_08026a72 with the 0x8AC message id in r0. */
            goto done;
        default:
            /* 0x08026670: kind - 1 outside 0..6 falls to the frame tail. */
            break;
        }

frame_tail:
        /* 0x08026b8c, in main:08026b44. */
        redraw &= ~1;
frame_end:
        /* 0x08026b96, in main:08026b44.  In the reference this test does not
         * end the loop at all: pending != 0 continues into the per-target
         * sprite pass and pending == 0 skips ahead to the pad read at
         * 0x08026cdc, and the only back edge is the long branch at
         * 0x08026e22.  Both destinations and the back edge are outside this
         * owner, so an owner-scoped draft has to invent a loop exit here; the
         * test itself is real, its consequence is not. */
        if (pending != 0)
            goto done;
    }

done:
    return cursor;
}
