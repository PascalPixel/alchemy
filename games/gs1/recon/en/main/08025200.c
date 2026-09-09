#include "types.h"

/* Item-list selection loop for a five-row page window.
 *
 * The caller supplies an owner id used to classify each entry, a u16 entry
 * table and the entry count. The routine builds a five row list window plus a
 * one line message window, keeps the page origin and the cursor row in the
 * shared menu session block at 0x03001E8C+0xA8, and spins one frame at a time
 * until the player confirms an entry or cancels. It returns the flat index of
 * the confirmed entry, or -1 on cancel.
 *
 * Uncertain: the roles of the 0xEA3 / 0xEA6 bytes in the block behind
 * 0x03001E8C (a redraw guard and a scrollbar-present mask are consistent with
 * the writes, but nothing here proves it), the exact layout of the 12-byte
 * sprite request records, and why Item_GetData is called for its side effect
 * with the result discarded in two places.
 */

#define ItemList_SelectEntry             Func_08025200

#define Resource_LoadIntoFreeSlot        Func_080040b4
#define Ui_FillVramBlockPattern          Func_08016738
#define UiWork_SetParamNibble            Func_0801e71c
#define Resource_LoadKind26EntryToBuffer Func_08021af0
#define UiWindow_SetTilemapEntry         Func_08019000
#define Item_ClassifyUseAbility          Func_08025180
#define Runtime_SetMainState19           Func_0800352c
#define Vram_CopyTile                    Func_080251d4

/* The window record produced by UiWindow_Create. Only the three tile-unit
 * geometry fields this routine reads are named. */
struct UiWindowWork {
    u8 unknown_00[0x8];
    u16 width;                  /* 0x08 */
    u8 unknown_0a[0x2];
    u16 col;                    /* 0x0c */
    u16 row;                    /* 0x0e */
};

/* One 12-byte sprite request record. The runtime accepts it through
 * Runtime_PushSlotEntry; the two words at +4 and +8 are cleared as words and
 * then patched as an eight-bit vertical position, a nine-bit horizontal
 * position and a ten-bit tile index. */
struct MenuSprite {
    u32 field_00;
    union {
        u32 word;
        struct {
            u8 y;
            u8 field_05;
            u16 x;
        } f;
    } place;
    union {
        u32 word;
        struct {
            u16 tile;
            u16 field_0a;
        } f;
    } gfx;
};

/* Shared menu session block. */
struct ItemMenuSession {
    u8 unknown_00[0x30];
    s32 cursor_pos;             /* 0x30 */
    s32 page;                   /* 0x34 */
    s32 keep_pos;               /* 0x38 */
    u8 unknown_3c[0x10];
    s32 field_4c;               /* 0x4c */
};

#define MENU_WORK     (*(u8 **)0x03001E8Cu)
#define MENU_SESSION  (*(struct ItemMenuSession **)(0x03001E8Cu + 0xA8))
#define MENU_SESSION2 (*(struct ItemMenuSession **)0x03001F34u)
#define KEY_TRIGGER   (*(u32 *)0x03001C94u)
#define KEY_REPEAT    (*(u32 *)0x03001B04u)
#define VIEW_FLAGS    (*(u32 *)0x03001E40u)

s32 Resource_LoadIntoFreeSlot(s32 size);
s32 Resource_GetBuffer(s32 handle, s32 arg1);
void Resource_ResetEntry(s32 handle);
void Runtime_PushSlotEntry(struct MenuSprite *entry, s32 slot);
struct UiWindowWork *UiWindow_Create(s32 col, s32 row, s32 width, s32 height,
                                     s32 flags);
void UiWork_Finalize(struct UiWindowWork *win, s32 mode);
void RenderOutput_RedrawSavedRect(struct UiWindowWork *win);
void Ui_FillVramBlockPattern(void);
void UiWork_SetParamNibble(s32 value);
void UiText_DrawCharacter(s32 glyph, struct UiWindowWork *win, s32 width,
                          s32 y);
s32 Resource_LoadKind26EntryToBuffer(s32 entry, s32 handle);
void UiWindow_SetTilemapEntry(struct UiWindowWork *win, s32 tile, s32 col,
                              s32 row, s32 flags);
s32 Item_ClassifyUseAbility(s32 owner, s32 entry);
void *Item_GetData(s32 entry);
void Runtime_SetMainState19(void);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
s32 FixedPoint_Ratio(s32 dividend, s32 divisor);
void Vram_CopyTile(s32 src, s32 dst);
void Func_08022768(s32 col, s32 row, s32 width, s32 height, s32 fill);
void Func_0801965c(s32 id, u8 *buf, s32 size);
void Func_08017aa4(u8 *buf, struct UiWindowWork *win, s32 arg2, s32 arg3);

s32 ItemList_SelectEntry(s32 owner, u16 *tbl, s32 cnt)
{
    struct MenuSprite icon[5];
    s32 slot[5];
    u8 msg[128];
    struct MenuSprite cursor;
    struct MenuSprite *sp;
    s32 *slotp;
    u16 *entry;
    u8 *work;
    struct UiWindowWork *win_msg;
    struct UiWindowWork *win_list;
    s32 cursor_slot;
    s32 page;
    s32 pos;
    s32 pos2;
    s32 keep_pos;
    s32 drawn_page;
    s32 drawn_pos;
    s32 icon_cnt;
    s32 result;
    s32 cls;
    s32 handle;
    s32 tile;
    s32 off;
    s32 cx;
    s32 cy;
    s32 i;
    s32 n;

    work = MENU_WORK;
    drawn_page = -1;
    drawn_pos = -1;
    cursor_slot = Resource_LoadIntoFreeSlot(0x80);
    win_msg = UiWindow_Create(0, 5, 30, 4, 42);
    icon_cnt = 0;
    page = MENU_SESSION->page;
    pos = MENU_SESSION->cursor_pos;
    keep_pos = MENU_SESSION->keep_pos;
    win_list = UiWindow_Create(15, 9, 15, 11, 6);

    sp = icon;
    for (i = 0; i <= 4; i++) {
        sp->place.word = 0x40000000;
        sp->gfx.word = 0;
        sp->place.f.x = (u16)((sp->place.f.x & ~0x1FF) |
                              (((win_list->col << 3) + 8) & 0x1FF));
        sp->place.f.y = (u8)(((((i * 2) + win_list->row)) << 3) + 4);
        sp++;
    }

    slotp = slot;
    off = 8;
    for (i = 4; i >= 0; i--) {
        handle = Resource_LoadIntoFreeSlot(0x80);
        *slotp++ = handle;
        *(u16 *)((u8 *)icon + off) =
            (u16)((*(u16 *)((u8 *)icon + off) & ~0x3FF) |
                  (Resource_GetBuffer(handle, -1) & 0x3FF));
        off += 12;
    }

    Vram_CopyTile(0xF018, 0x200);
    Vram_CopyTile(0xF018, 0x201);
    Vram_CopyTile(0xF019, 0x210);
    Vram_CopyTile(0xF019, 0x211);

    pos2 = pos * 2;

    for (;;) {
        if (page != drawn_page || pos != drawn_pos) {
            work[0xEA6] = 1;
            Func_08022768(win_list->col + 1,
                          win_list->row + (drawn_pos * 2) + 1,
                          win_list->width - 2, 1, 15);
            Ui_FillVramBlockPattern();

            if (cnt != 0) {
                if (Item_ClassifyUseAbility(owner, tbl[page + pos]) == 2) {
                    Func_0801965c(0x8EE, msg, 52);
                } else {
                    Func_0801965c((tbl[page + pos] & 0x1FF) + 0x75, msg, 52);
                }
            } else {
                Func_0801965c(0x8E5, msg, 52);
            }
            Func_08017aa4(msg, win_msg, 0, 4);
            drawn_pos = pos;

            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(win_list);
                entry = &tbl[page];
                n = 0;
                if (*entry != 0) {
                    slotp = slot;
                    off = 8;
                    for (;;) {
                        Item_GetData(*entry);
                        UiWork_SetParamNibble(15);
                        if (Item_ClassifyUseAbility(owner, *entry) != 0) {
                            UiWork_SetParamNibble(4);
                        } else if ((*entry & 0x400) != 0) {
                            UiWork_SetParamNibble(2);
                        }
                        UiText_DrawCharacter((*entry & 0x1FF) + 0x182,
                                             win_list, 16, n * 16);
                        UiWork_SetParamNibble(15);
                        handle = *slotp++;
                        tile = Resource_LoadKind26EntryToBuffer(*entry, handle);
                        *(u16 *)((u8 *)icon + off) =
                            (u16)((*(u16 *)((u8 *)icon + off) & ~0x3FF) |
                                  (tile & 0x3FF));
                        n++;
                        off += 12;
                        if (n > 4) {
                            break;
                        }
                        entry++;
                        if (*entry == 0) {
                            break;
                        }
                    }
                }
                icon_cnt = n;
                drawn_page = page;
            }

            if (cnt > 5) {
                for (i = 0; i < (n = FixedPoint_Ratio(cnt + 4, 5)); i++) {
                    tile = i + 0xF301;
                    if (i == FixedPoint_Ratio(page, 5)) {
                        tile = i + 0xF30B;
                    }
                    UiWindow_SetTilemapEntry(win_list, tile,
                                             (win_list->width - n) + i - 2,
                                             -1, 0);
                }
            }

            Func_08022768(win_list->col + 1, win_list->row + pos2 + 1,
                          win_list->width - 2, 1, 14);
            work[0xEA3] = 1;
            work[0xEA6] = 0;
        }

        if (cnt > 5) {
            for (i = 0; i < FixedPoint_Ratio(cnt + 4, 5); i++) {
                tile = i + 0xF301;
                if ((VIEW_FLAGS & 15) <= 11 &&
                    i == FixedPoint_Ratio(page, 5)) {
                    tile = i + 0xF30B;
                }
                UiWindow_SetTilemapEntry(
                    win_list, tile,
                    (win_list->width - FixedPoint_Ratio(cnt + 4, 5)) + i - 2,
                    -1, 0);
            }
            UiWindow_SetTilemapEntry(
                win_list, 0xF334,
                win_list->width - FixedPoint_Ratio(cnt + 4, 5) - 3, -1, 0);
            UiWindow_SetTilemapEntry(win_list, 0xF335, win_list->width - 2,
                                     -1, 0);
            work[0xEA3] |= (u8)(2 << ((u32)(win_list->row - 1) >> 2));
        }

        if (icon_cnt > 0) {
            sp = icon;
            i = icon_cnt;
            do {
                i--;
                Runtime_PushSlotEntry(sp, 240);
                sp++;
            } while (i != 0);
        }

        cx = (win_list->col << 3) - 2;
        cy = ((pos2 + win_list->row) << 3) + 20;
        cursor.place.word = 0x40000000;
        cursor.gfx.word = 0;
        cursor.gfx.f.tile =
            (u16)((cursor.gfx.f.tile & ~0x3FF) |
                  (Resource_GetBuffer(cursor_slot, 0x080310A4) & 0x3FF));
        cursor.place.f.x =
            (u16)((cursor.place.f.x & ~0x1FF) |
                  ((cx + ((VIEW_FLAGS & 4) >> 1) + 0xFFFC) & 0x1FF));
        cursor.place.f.y = (u8)((cy - ((VIEW_FLAGS & 4) >> 2)) + 248);

        if (cnt != 0) {
            Runtime_PushSlotEntry(&cursor, 242);
        }

        MENU_SESSION2->page = page;
        MENU_SESSION2->cursor_pos = pos;
        MENU_SESSION2->keep_pos = keep_pos;

        if ((KEY_TRIGGER & 1) != 0) {
            if (cnt == 0) {
                result = -1;
                break;
            }
            result = page + pos;
            Item_GetData(tbl[page + pos]);
            cls = 0;
            if ((tbl[page + pos] & 0x400) == 0) {
                cls = Item_ClassifyUseAbility(owner, tbl[page + pos]);
                if (cls == 0) {
                    break;
                }
            }
            Audio_PlayCue(114);
            if (cls == 2) {
                Func_0801965c(0x8EE, msg, 52);
            } else if ((tbl[page + pos] & 0x400) != 0) {
                Func_0801965c(0x8EC, msg, 52);
            } else {
                Func_0801965c(0x8EB, msg, 52);
            }
            Ui_FillVramBlockPattern();
            Func_08017aa4(msg, win_msg, 0, 4);
        } else if (MENU_SESSION2->field_4c == 0 || (KEY_TRIGGER & 2) != 0) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }

        if (cnt != 0) {
            if ((KEY_REPEAT & 0x80) != 0) {
                Audio_PlayCue(111);
                pos++;
                if (pos == 5 || (page + pos) == cnt) {
                    pos = 0;
                }
                keep_pos = pos;
                pos2 = pos * 2;
            } else if ((KEY_REPEAT & 0x40) != 0) {
                Audio_PlayCue(111);
                pos--;
                if (pos < 0) {
                    if (page == FixedPoint_Ratio(cnt - 1, 5) * 5) {
                        pos = (cnt - page) - 1;
                    } else {
                        pos = 4;
                    }
                }
                keep_pos = pos;
                pos2 = pos * 2;
            } else if ((KEY_REPEAT & 0x10) != 0) {
                Audio_PlayCue(111);
                Runtime_SetMainState19();
                if ((page + 5) >= cnt) {
                    if (page != 0) {
                        pos = keep_pos;
                        page = 0;
                        pos2 = pos * 2;
                    }
                } else {
                    page = page + 5;
                    pos = keep_pos;
                    if (page == FixedPoint_Ratio(cnt - 1, 5) * 5) {
                        pos = (cnt - page) - 1;
                        if (pos > keep_pos) {
                            pos = keep_pos;
                        }
                    }
                    pos2 = pos * 2;
                }
            } else if ((KEY_REPEAT & 0x20) != 0) {
                Audio_PlayCue(111);
                Runtime_SetMainState19();
                if (page != 0) {
                    pos = keep_pos;
                    page = page - 5;
                    pos2 = pos * 2;
                } else {
                    page = FixedPoint_Ratio(cnt - 1, 5) * 5;
                    pos = keep_pos;
                    if (page != 0) {
                        pos = (cnt - page) - 1;
                        if (pos > keep_pos) {
                            pos = keep_pos;
                        }
                    }
                    pos2 = pos * 2;
                }
            }
        }

        WaitFrames(1);
    }

    UiWork_Finalize(win_msg, 1);
    UiWork_Finalize(win_list, 1);
    WaitFrames(1);
    Resource_ResetEntry(cursor_slot);
    slotp = slot;
    for (i = 4; i >= 0; i--) {
        Resource_ResetEntry(*slotp++);
    }
    WaitFrames(1);
    return result;
}
