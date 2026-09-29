#include "TYPES.H"

/*
 * Battle ability picker (owner main:08023e70, 2756 bytes).
 *
 * Builds a flat table of the acting unit's selectable abilities out of two
 * 4 x 20 bit grids in the unit record, then runs a modal five-per-page list
 * until the player confirms an entry or cancels.  The return value is the
 * packed table code of the confirmed entry, or -1 on cancel.
 *
 * Packed table code, as used by every draw and commit site below:
 *   bits  0.. 7  row within the grid column (0..19)
 *   bits  8..11  grid column (0..3)
 *   bit  16      entry came from the second grid (field_0f8), so it carries
 *                a placement record and a detail sub-window
 *   bits 17..21  placement timer, when positive
 *   bit  31      table terminator (0x80000000)
 *
 * The single `bl` back into the middle of the owner at 0x0802489c is the
 * out-of-range back edge of the outer `for (;;)`, not a call.
 *
 * Uncertain and left neutral: the 12-byte cursor record pushed through
 * Runtime_PushSlotEntry is written both as words and as narrow sub-fields,
 * so its sub-word members are spelled through offsets rather than guessed
 * bitfields; the +0xFFFA and +248 cursor biases are transcribed as the
 * reference materializes them.
 *
 * Residual, measured rather than guessed: every reference block, branch,
 * loop, call and store is present and the extent matches to four bytes, but
 * the allocator picks different callee-saved registers (the result lives in
 * r5 here and r6 there, the icon and page loop counters in r4 here and r7
 * there), so two fewer pseudos are spilled and every stack slot above the
 * outgoing-argument word is offset by eight.  Because the allocator gave the
 * two "close the detail window" blocks and the two 0x666 message blocks the
 * same registers here, the late cross-jump pass tail-merged them; the
 * reference keeps both copies, which is why three `bl` sites are absent from
 * the candidate even though nothing is missing from the source.
 */

/* Only the m2c spellings this draft actually uses. */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

#define RUNTIME (*(struct RuntimeCells *)0x03001e8c)
#define MENU_STATE (RUNTIME.menu)
#define CURSOR_OWNER (*(struct CursorOwner **)0x03001e90)
#define FRAME_COUNTER (*(u32 *)0x03001e40)
#define BATTLE_OPTIONS (*(u32 *)0x03001ae8)
#define INPUT_TRIGGER (*(s32 *)0x03001c94)
#define INPUT_REPEAT (*(s32 *)0x03001b04)

/* Flag bytes the render context exposes to the frame builder. */
#define CTX_LIST_DIRTY 0xEA3
#define CTX_TEXT_BUSY 0xEA6

struct MenuRuntime;

struct UiWindow {
    s32 field_00;
    s32 field_04;
    u16 field_08;
    u16 field_0a;
    u16 field_0c;
    u16 field_0e;
};

struct CursorOwner {
    struct UiWindow *field_00;
    u8 unknown_04[8];
    u16 field_0c;
};

/* Written as words and then patched as narrow fields; see the header note. */
struct RenderSlot {
    s32 field_00;
    s32 field_04;
    s32 field_08;
};

struct RuntimeCells {
    u8 *ctx;
    u8 unknown_004[0xa4];
    struct MenuRuntime *menu;
};

struct MenuRuntime {
    u8 unknown_000[0x30];
    s32 field_30;
    s32 field_34;
    s32 field_38;
    u8 unknown_03c[0x10];
    s32 field_4c;
    u8 unknown_050[0x88];
    s32 field_d8;
    s32 field_dc;
    u8 unknown_0e0[4];
    s32 field_e4[8];
};

/* Same shape the placement updater already uses. */
struct PlacementEntry {
    u8 x;
    u8 y;
    u8 id;
    s8 timer;
};

struct PlacementList {
    struct PlacementEntry entries[64];
    s32 count;
};

struct PlacementTable {
    u8 padding[8];
    struct PlacementList list;
};

struct BattleUnitGrid {
    u8 unknown_000[0xf8];
    u32 field_0f8[4];
    u32 field_108[4];
};

extern u8 Data_080310a4[];
extern u8 Data_08037308[];
extern u8 Data_080373e7[];
extern u8 Data_080373eb[];

s32 Math_Div(s32 numerator, s32 denominator);
void Sys_Free(void *buf);
void WaitFrames(s32 frames);
void Runtime_SetMainState19(void);
void Runtime_PushSlotEntry(void *slot, s32 kind);
void Resource_ResetEntry(s32 resource);
s32 Resource_LoadIntoFreeSlot(s32 kind);
s32 Resource_GetBuffer(s32 resource, u8 *data);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 w, s32 h, s32 pal);
void UiWork_Finalize(struct UiWindow *win, s32 mode);
void RenderOutput_RedrawSavedRect(struct UiWindow *win);
void RenderOutput_ClearList(struct UiWindow *win);
void Ui_FillVramBlockPattern(void);
void UiText_RenderWideStringAtOffset(s16 *buf, struct UiWindow *win, s32 x, s32 y);
void UiWindow_SetTilemapEntry(struct UiWindow *win, s32 tile, s32 x, s32 y, s32 flags);
s32 UiText_CopyMessageString(s32 message, s16 *buf, s32 count);
void UiWork_ClearValueNameTables(void);
void UiWork_PushValueSlot(s32 value, s32 width);
void Func_0801e318(void);
void UiWork_SetAltFlagAndClearTable(s32 flag);
void UiWork_SetParamNibble(s32 nibble);
void UiText_DrawCharacterAtOffset(s32 message, struct UiWindow *win, s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 digits, struct UiWindow *win, s32 x, s32 y);
void Ui_SetRectHighlight(s32 x, s32 y, s32 w, s32 h, s32 pal);
struct UiWindow *DjinnMenu_ShowChangePreview(struct UiWindow *win, s32 owner, u32 code, s32 sel,
                               s32 *count);
struct PlacementTable *Func_08077000(s32 side);
struct BattleUnitGrid *Owner_GetStateFar(s32 owner);
void Func_080f9010(s32 cue);

/* The two IWRAM helpers below are reached through the compiler's call-via
 * veneer bank, so they are modelled as what they are: typed indirect calls
 * to fixed relocated routines. */
typedef void (*VramFillProc)(u32 dst, s32 count, u32 value);
typedef void (*VramCopyProc)(u32 dst, u8 *src, s32 count);

#define VRAM_FILL ((VramFillProc)0x03000168)
#define VRAM_COPY ((VramCopyProc)0x03001388)

s32 Battle_SelectAbility(s32 owner)
{
    u8 *ctx;
    struct MenuRuntime *menu;
    struct BattleUnitGrid *unit;
    struct PlacementList *list;
    struct UiWindow *msg_win;
    struct UiWindow *list_win;
    struct UiWindow *detail_win;
    struct UiWindow *icon_win;
    struct CursorOwner *cursor;
    u32 *tbl;
    u32 *pos;
    u32 cur;
    u32 code;
    u32 blink;
    s32 res;
    s32 cnt;
    s32 page;
    s32 row;
    s32 saved_row;
    s32 prev_page;
    s32 prev_row;
    s32 sub_sel;
    s32 sub_cnt;
    s32 redraw;
    s32 is_used;
    s32 mask;
    s32 ret;
    s32 pages;
    s32 tile;
    s32 pal;
    s32 curs_x;
    s32 curs_y;
    s32 trig;
    s32 rep;
    s32 opts;
    s32 x;
    s32 y;
    s32 i;
    s32 j;
    s16 buf[64];
    struct RenderSlot slot;

    ctx = RUNTIME.ctx;
    detail_win = 0;
    prev_row = -1;
    res = Resource_LoadIntoFreeSlot(0x80);
    tbl = (u32 *)Runtime_BumpAllocateAlternatePool(0xa8 * 2);
    cnt = 0;
    prev_page = -1;
    mask = 0;
    sub_sel = 0;
    sub_cnt = 0;
    redraw = 0;
    is_used = 0;
    msg_win = UiWindow_Create(0, 4, 30, 4, 42);
    UiWork_SetAltFlagAndClearTable(1);
    list_win = UiWindow_Create(21, 9, 9, 11, 6);
    page = MENU_STATE->field_34;
    row = MENU_STATE->field_30;
    saved_row = MENU_STATE->field_38;
    unit = Owner_GetStateFar(owner);

    /* Flatten the two ability grids into one page-ordered table. */
    for (x = 0; x <= 3; x++) {
        pos = &tbl[cnt];
        for (y = 0; y <= 19; y++) {
            if (unit->field_108[x] & (u32)(1 << y)) {
                *pos = (u32)((x << 8) | y);
                pos++;
                cnt++;
            } else if (unit->field_0f8[x] & (u32)(1 << y)) {
                list = &Func_08077000((u32)owner > 7 ? 1 : 0)->list;
                for (i = 0; i < list->count; i++) {
                    if (list->entries[i].id == owner && list->entries[i].x == x &&
                        list->entries[i].y == y) {
                        break;
                    }
                }
                code = (u32)((x << 8) | y | 0x10000);
                *pos = code;
                if (list->entries[i].timer > 0) {
                    code |= (u32)(list->entries[i].timer << 17);
                    *pos = code;
                }
                cnt++;
                pos++;
            }
        }
    }
    tbl[cnt] = 0x80000000;
    ctx[CTX_LIST_DIRTY] = 1;

    for (;;) {
        if (page != prev_page || row != prev_row || redraw != 0) {
            cur = tbl[page + row];
            ctx[CTX_TEXT_BUSY] = 1;
            mask = 0;
            Ui_SetRectHighlight(list_win->field_0c + 1, list_win->field_0e + prev_row * 2 + 1,
                          list_win->field_08 - 2, 1, 15);
            if (sub_sel != 0) {
                UiWork_Finalize(msg_win, 1);
                msg_win = UiWindow_Create(0, 4, 30, 4, 42);
                Ui_FillVramBlockPattern();
            }
            redraw = 0;
            if (cnt != 0) {
                UiWork_ClearValueNameTables();
                is_used = 0;
                for (i = 0; i <= 7; i++) {
                    if ((u32)MENU_STATE->field_e4[i] == cur) {
                        is_used = 1;
                        break;
                    }
                }
                if (is_used != 0) {
                    UiText_CopyMessageString(0x8ef, buf, 52);
                    if (detail_win != 0) {
                        UiWork_Finalize(detail_win, 1);
                        detail_win = 0;
                        sub_sel = 0;
                    }
                } else if (cur & 0x10000) {
                    if (cur & 0x3e0000) {
                        UiWork_PushValueSlot((s32)((cur & 0x3e0000) >> 17), 5);
                        UiText_CopyMessageString((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + 0x666,
                                      buf, 52);
                        if (detail_win != 0) {
                            UiWork_Finalize(detail_win, 1);
                            detail_win = 0;
                            sub_sel = 0;
                        }
                    } else {
                        detail_win = DjinnMenu_ShowChangePreview(detail_win, owner, cur, sub_sel, &sub_cnt);
                        UiText_CopyMessageString(0x899, buf, 52);
                        mask = 1 << ((cur & 0xf00) >> 8);
                    }
                } else {
                    detail_win = DjinnMenu_ShowChangePreview(detail_win, owner, cur, sub_sel, &sub_cnt);
                    UiText_CopyMessageString((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + 0x666,
                                  buf, 52);
                }
            } else {
                UiText_CopyMessageString(0x8ed, buf, 52);
            }
            ctx[CTX_TEXT_BUSY] = 0;
            if (sub_sel == 0) {
                ctx[CTX_TEXT_BUSY] = 1;
                UiWork_Finalize(msg_win, 1);
                msg_win = UiWindow_Create(0, 4, 30, 4, 42);
                ctx[CTX_TEXT_BUSY] = (u8)sub_sel;
            }
            UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
            prev_row = row;
            if (page != prev_page) {
                RenderOutput_RedrawSavedRect(list_win);
                pos = &tbl[page];
                code = *pos;
                j = 0;
                while (code != 0x80000000) {
                    UiWindow_SetTilemapEntry(list_win, (s32)((code & 0xf00) >> 8) + 0x5001, 0, j * 2, 0);
                    if (code & 0x3e0000) {
                        UiWork_SetParamNibble(4);
                    } else if (code & 0x10000) {
                        UiWork_SetParamNibble(2);
                    }
                    UiText_DrawCharacterAtOffset((s32)(((code & 0xf00) >> 8) * 20 + (code & 0xff)) + 0x45f,
                                  list_win, 8, j * 16);
                    if (code & 0x3e0000) {
                        UiText_DrawNumberAtOffset((s32)((code & 0x3e0000) >> 17), 1, list_win, 48, j * 16);
                    }
                    UiWork_SetParamNibble(15);
                    j++;
                    if (j > 4) {
                        break;
                    }
                    pos++;
                    code = *pos;
                }
                prev_page = page;
            }
            if (cnt > 5) {
                for (j = 0; j < (pages = Math_Div(cnt + 4, 5)); j++) {
                    tile = j + 0xf301;
                    if (j == Math_Div(page, 5)) {
                        tile = j + 0xf30b;
                    }
                    UiWindow_SetTilemapEntry(list_win, tile, list_win->field_08 - pages + j - 2, -1, 0);
                }
            }
            Ui_SetRectHighlight(list_win->field_0c + 1, list_win->field_0e + row * 2 + 1,
                          list_win->field_08 - 2, 1, 14);
            ctx[CTX_LIST_DIRTY] = 1;
            ctx[CTX_TEXT_BUSY] = 0;
        }

        if (cnt > 5) {
            for (j = 0; j < (pages = Math_Div(cnt + 4, 5)); j++) {
                tile = j + 0xf301;
                if ((BATTLE_OPTIONS & 0x100) || (FRAME_COUNTER & 15) <= 11) {
                    if (j == Math_Div(page, 5)) {
                        tile = j + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry(list_win, tile,
                              list_win->field_08 - Math_Div(cnt + 4, 5) + j - 2, -1, 0);
            }
            if ((BATTLE_OPTIONS & 0x100) == 0) {
                UiWindow_SetTilemapEntry(list_win, 0xf334, list_win->field_08 - pages - 3, -1, 0);
                UiWindow_SetTilemapEntry(list_win, 0xf335, list_win->field_08 - 2, -1, 0);
            } else {
                UiWindow_SetTilemapEntry(list_win, 0xf011, list_win->field_08 - pages - 3, -1, 0);
                UiWindow_SetTilemapEntry(list_win, 0xf012, list_win->field_08 - 2, -1, 0);
            }
            ctx[CTX_LIST_DIRTY] |= (u8)(2 << ((u32)(list_win->field_0e - 1) >> 2));
        }

        trig = INPUT_TRIGGER;
        rep = INPUT_REPEAT;
        opts = (s32)BATTLE_OPTIONS;
        menu = MENU_STATE;
        if (menu->field_d8 != 0) {
            trig = 0;
            rep = 0;
            opts = 0;
            if (menu->field_dc == 0) {
                menu->field_dc = 60;
                rep = 1;
                trig = 1;
            } else {
                menu->field_dc = menu->field_dc - 1;
            }
        }
        menu = MENU_STATE;
        if (menu->field_4c == 0 || (trig & 2)) {
            Func_080f9010(0x71);
            ret = -1;
            break;
        }
        if (trig & 1) {
            if (cnt == 0) {
                ret = -1;
                break;
            }
            cur = tbl[page + row];
            if ((cur & 0x3e0000) == 0) {
                if (is_used == 0) {
                    menu->field_34 = page;
                    menu->field_30 = row;
                    menu->field_38 = saved_row;
                    ret = (s32)cur;
                    break;
                }
            } else if (is_used == 0) {
                Ui_FillVramBlockPattern();
                UiWork_ClearValueNameTables();
                UiWork_PushValueSlot((s32)((cur & 0x3e0000) >> 17), 5);
                UiText_CopyMessageString(0x898, buf, 52);
                UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
            }
            Func_080f9010(0x72);
        }

        if (cnt != 0) {
            if (rep & 0x80) {
                Func_080f9010(0x6f);
                row++;
                if (row == 5 || page + row == cnt) {
                    row = 0;
                }
                saved_row = row;
            } else if (rep & 0x40) {
                Func_080f9010(0x6f);
                row--;
                if (row < 0) {
                    if (page == Math_Div(cnt - 1, 5) * 5) {
                        row = cnt - page - 1;
                    } else {
                        row = 4;
                    }
                }
                saved_row = row;
            } else if (opts & 0x100) {
                if (detail_win != 0) {
                    for (i = 0; i < sub_cnt; i++) {
                        tile = i + 0xf301;
                        if ((FRAME_COUNTER & 15) <= 11 && i == sub_sel - 1) {
                            tile = sub_sel + 0xf30b;
                        }
                        UiWindow_SetTilemapEntry(detail_win, tile,
                                      detail_win->field_08 - sub_cnt + i - 2, -1, 0);
                    }
                    UiWindow_SetTilemapEntry(detail_win, 0xf334, detail_win->field_08 - sub_cnt - 3, -1,
                                  0);
                    UiWindow_SetTilemapEntry(detail_win, 0xf335, detail_win->field_08 - 2, -1, 0);
                    ctx[CTX_LIST_DIRTY] |= (u8)(2 << (detail_win->field_0e >> 2));
                }
                if (sub_sel == 0 && sub_cnt != 0) {
                    if (detail_win != 0) {
                        RenderOutput_ClearList(detail_win);
                    }
                    sub_sel = 1;
                    redraw = 1;
                } else {
                    if (sub_sel > sub_cnt) {
                        sub_sel = sub_cnt;
                    }
                    if (sub_sel != 0) {
                        if (rep & 16) {
                            Func_080f9010(0x6f);
                            sub_sel++;
                            if (sub_sel > sub_cnt) {
                                sub_sel = 1;
                            }
                            redraw = 1;
                        } else if (rep & 32) {
                            Func_080f9010(0x6f);
                            sub_sel--;
                            if (sub_sel <= 0) {
                                sub_sel = sub_cnt;
                            }
                            redraw = 1;
                        }
                    }
                }
            } else if (sub_sel != 0) {
                if (detail_win != 0) {
                    RenderOutput_ClearList(detail_win);
                }
                sub_sel = 0;
                redraw = 1;
            } else if (rep & 16) {
                Func_080f9010(0x6f);
                Runtime_SetMainState19();
                if (page + 5 >= cnt) {
                    if (page != 0) {
                        row = saved_row;
                        page = 0;
                    }
                } else {
                    page = page + 5;
                    row = saved_row;
                    if (page == Math_Div(cnt - 1, 5) * 5) {
                        row = cnt - page - 1;
                        if (row > saved_row) {
                            row = saved_row;
                        }
                    }
                }
            } else if (rep & 32) {
                Func_080f9010(0x6f);
                Runtime_SetMainState19();
                if (page != 0) {
                    page -= 5;
                    row = saved_row;
                } else {
                    page = Math_Div(cnt - 1, 5) * 5;
                    row = saved_row;
                    if (page != 0) {
                        row = cnt - page - 1;
                        if (row > saved_row) {
                            row = saved_row;
                        }
                    }
                }
            }
        }

        curs_x = list_win->field_0c * 8 - 2;
        curs_y = (row * 2 + list_win->field_0e) * 8 + 20;
        slot.field_04 = 0x40000000;
        slot.field_08 = 0;
        M2C_FIELD(&slot, u16 *, 8) =
            (u16)((M2C_FIELD(&slot, u16 *, 8) & 0xfffffc00) |
                  (Resource_GetBuffer(res, Data_080310a4) & 0x3ff));
        M2C_FIELD(&slot, u16 *, 6) =
            (u16)((M2C_FIELD(&slot, u16 *, 6) & 0xfffffe00) |
                  ((curs_x + (s32)((FRAME_COUNTER & 4) >> 1) + 0xfffa) & 0x1ff));
        M2C_FIELD(&slot, u8 *, 4) = (u8)(curs_y - (s32)((FRAME_COUNTER & 4) >> 2) + 248);
        if (cnt != 0) {
            Runtime_PushSlotEntry(&slot, 242);
        }
        cursor = CURSOR_OWNER;
        blink = FRAME_COUNTER & 4;
        icon_win = cursor->field_00;
        if (cursor->field_0c & 2) {
            for (i = 0; i <= 3; i++) {
                pal = 15 - (blink != 0);
                if (((1 << i) & mask) == 0) {
                    pal = 15;
                }
                Ui_SetRectHighlight(icon_win->field_0c + Data_080373e7[i] + 1,
                              icon_win->field_0e + Data_080373eb[i] + 1, 2, 2, pal);
            }
        }
        if (FRAME_COUNTER & 4) {
            VRAM_COPY(0x06006500, Data_08037308, 32);
            VRAM_COPY(0x06006520, Data_08037308, 32);
        } else {
            VRAM_FILL(0x06006500, 32, 0x44444444);
            VRAM_COPY(0x06006520, Data_08037308, 32);
        }
        WaitFrames(1);
    }

    cursor = CURSOR_OWNER;
    if (cursor->field_0c & 2) {
        icon_win = cursor->field_00;
        Ui_SetRectHighlight(icon_win->field_0c + 1, icon_win->field_0e + 1, 4, 4, 15);
    }
    Resource_ResetEntry(res);
    UiWork_Finalize(msg_win, 1);
    UiWork_Finalize(detail_win, 1);
    UiWork_Finalize(list_win, 1);
    Func_0801e318();
    UiWork_SetAltFlagAndClearTable(0);
    Sys_Free(tbl);
    WaitFrames(1);
    return ret;
}
