#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
/* 2026-10-02 bounded list-bank structural trials; original body retained.
 * EN score 768: 28 stack-only, 4 operand, 11 reordered; no inserted or
 * deleted instructions. Native and candidate frames are 304 bytes.
 * H1, scalar u16 tile halfword at sprite +8, preserving upper six bits:
 * score 3696 (25 register, 76 stack, 14 operand, 15 reordered, 8 inserted,
 * 15 deleted), frame 296. Mask narrowed from fffffc00 to fc00. Rejected.
 * H2, named u32 bitfield view of the attribute member, original assignment:
 * score 3721 (22 register, 76 stack, 11 operand, 15 reordered, 8 inserted,
 * 16 deleted), frame 296. Full mask returned; loop address model changed.
 * H3, complete 12-byte field-only sprite view, base-plus-index tile access:
 * same score/counts/frame as H2; native indexed address became a running
 * pointer. Rejected. The word/field union relation is needed by this body;
 * narrowing only its tile access changes allocation beyond the residual.
 * All aggregate locals are used (text, cursor, handles, sprites, position);
 * no unread frame filler or missing native dead load was found. No new
 * storage, asm, fixed register, compiler option or routing was retained.
 * Three focused trials exhausted this view hypothesis; no broad search.
 */
#include "TYPES.H"
#include "IO_REG.H"
#include "MENU_LIST.H"

/*
 * Item list of the battle menu.
 *
 * One page of at most five items from `entries` in a 15x11 window, a
 * description line in the message window, a page indicator when the list is
 * longer than a page, one icon sprite per row and the blinking cursor. Returns
 * the index of the chosen item, or -1 when the player backs out. An item the
 * owner cannot use, or one that is broken (bit 10 of its entry), is drawn in
 * another colour and answers a press with a buzzer and a reason.
 *
 * Twin of BattleMenu_RunActionSelection (0802592c); the shapes that draft's
 * header lists apply here too.
 */

#define ITEM_ID_MASK 0x1ff
#define ITEM_BROKEN 0x400

extern u8 MsgItemName;
extern u8 MsgItemPlainName;

s32 Item_ClassifyUseAbility(s32 owner, s32 entry);
void *Item_Get(s32 entry);
s32 Resource_LoadKind26EntryToBuffer(s32 entry, s32 handle);

s32 ItemList_SelectEntry(s32 owner, u16 *entries, s32 count)
{
    struct UiRenderWork *render;
    s32 drawn_page;
    s32 drawn_row;
    s32 cursor_handle;
    struct UiWindow *message_window;
    s32 icon_count;
    s32 preferred_row;
    struct MenuPoint pos;
    struct MenuCell *cell;
    struct UiWindow *window;
    s32 page;
    s32 row;
    s32 entry;
    s32 tile;
    s32 edge;
    s32 reason;
    s32 result;
    s32 i;
    struct MenuSprite cursor;
    struct MenuSprite *spr;
    s16 buffer[64];
    s32 handles[PAGE_ROWS];
    struct MenuSprite sprites[PAGE_ROWS];

    render = (struct UiRenderWork *)gWindowWork[0];
    drawn_page = -1;
    drawn_row = -1;
    cursor_handle = Resource_LoadIntoFreeSlot(128);
    message_window = UiWindow_Create(0, 5, 30, 4, 42);
    icon_count = 0;
    page = ((struct MenuCell *)gWindowWork[42])->page;
    row = ((struct MenuCell *)gWindowWork[42])->row;
    preferred_row = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    window = UiWindow_Create(SHOP_LIST_X, 9, SHOP_LIST_WIDTH, PAGE_ROWS * 2 + 1, 6);

    for (i = 0; i < PAGE_ROWS; i++) {
        s32 y = i * 2;

        spr = &sprites[i];
        do {
            sprites[i].oam.word.attr01 = MENU_SPRITE_SIZE_16;
            sprites[i].oam.word.attr23 = 0;
        } while (0);
        spr->oam.f.x = window->x * 8 + 8;
        spr->oam.f.y = (y + window->y) * 8 + 4;
    }

    for (i = 0; i < PAGE_ROWS; i++) {
        handles[i] = Resource_LoadIntoFreeSlot(128);
        sprites[i].oam.f.tile = Resource_GetBuffer(handles[i], -1);
    }

    Vram_CopyTile(0xf018, 0x200);
    Vram_CopyTile(0xf018, 0x201);
    Vram_CopyTile(0xf019, 0x210);
    Vram_CopyTile(0xf019, 0x211);

    spr = &cursor;
    for (;;) {
        if (page != drawn_page || row != drawn_row) {
            render->menu_busy = 1;
            Ui_SetRectHighlight(
                window->x + 1,
                window->y + drawn_row * 2 + 1,
                window->width - 2,
                1,
                15);
            Ui_FillVramBlockPattern();
            if (count != 0) {
                if (Item_ClassifyUseAbility(owner, entries[page + row]) == 2) {
                    UiText_CopyMessageString(0x8ee, buffer, 52);
                } else {
                    UiText_CopyMessageString(
                        (entries[page + row] & ITEM_ID_MASK) + (s32)&MsgItemPlainName,
                        buffer,
                        52);
                }
            } else {
                UiText_CopyMessageString(0x8e5, buffer, 52);
            }
            UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
            drawn_row = row;

            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(window);
                for (i = 0; i < PAGE_ROWS; i++) {
                    entry = entries[page + i];
                    if (entry == 0) {
                        break;
                    }
                    Item_Get(entry);
                    UiWork_SetParamNibble(15);
                    if (Item_ClassifyUseAbility(owner, entry) != 0) {
                        UiWork_SetParamNibble(4);
                    } else if ((entry & ITEM_BROKEN) != 0) {
                        UiWork_SetParamNibble(2);
                    }
                    UiText_DrawCharacterAtOffset(
                        (entry & ITEM_ID_MASK) + (s32)&MsgItemName, window, 16, i * 16);
                    UiWork_SetParamNibble(15);
                    sprites[i].oam.f.tile =
                        Resource_LoadKind26EntryToBuffer(entry, handles[i]);
                }
                icon_count = i;
                drawn_page = page;
            }

            if (count > PAGE_ROWS) {
                for (i = 0; i < (count + 4) / PAGE_ROWS; i++) {
                    tile = i + 0xf301;
                    if (i == page / PAGE_ROWS) {
                        tile = i + 0xf30b;
                    }
                    UiWindow_SetTilemapEntry(
                        window,
                        tile,
                        window->width - (count + 4) / PAGE_ROWS + i - 2,
                        -1,
                        0);
                }
            }

            Ui_SetRectHighlight(
                window->x + 1,
                window->y + row * 2 + 1,
                window->width - 2,
                1,
                14);
            render->dirty = 1;
            render->menu_busy = 0;
        }

        if (count > PAGE_ROWS) {
            for (i = 0; i < (count + 4) / PAGE_ROWS; i++) {
                tile = i + 0xf301;
                if ((gFrameCount & 15) <= 11) {
                    if (i == page / PAGE_ROWS) {
                        tile = i + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry(
                    window,
                    tile,
                    window->width - (count + 4) / PAGE_ROWS + i - 2,
                    -1,
                    0);
            }
            edge = -1;
            UiWindow_SetTilemapEntry(
                window, 0xf334, window->width - (count + 4) / PAGE_ROWS - 3, edge, 0);
            UiWindow_SetTilemapEntry(
                window, 0xf335, window->width - 2, edge, 0);
            render->dirty |= 2 << ((u32)(window->y - 1) >> 2);
        }

        for (i = 0; i < icon_count; i++) {
            Runtime_PushSlotEntry(&sprites[i], 240);
        }

        pos.x = window->x * 8 - 2;
        pos.y = (row * 2 + window->y) * 8 + 20;
        ((u32 *)spr)[1] = MENU_SPRITE_SIZE_16;
        ((u32 *)spr)[2] = 0;
        spr->oam.f.tile =
            Resource_GetBuffer(cursor_handle, (s32)Resource_FixedBlockBTiles);
        spr->oam.f.x = pos.x + ((gFrameCount & 4) >> 1) - 4;
        spr->oam.f.y = pos.y - ((gFrameCount & 4) >> 2) - 8;
        if (count != 0) {
            Runtime_PushSlotEntry(spr, 242);
        }

        cell = gLinkCountdownWork;
        cell->page = page;
        cell->row = row;
        cell->preferred_row = preferred_row;

        if ((gKeyState & KEY_A) != 0) {
            if (count != 0) {
                result = page + row;
                Item_Get(entries[result]);
                reason = 0;
                if ((entries[result] & ITEM_BROKEN) == 0) {
                    reason = Item_ClassifyUseAbility(owner, entries[result]);
                    if (reason == 0) {
                        break;
                    }
                }
                AudioCommand_PlayFar(114);
                if (reason == 2) {
                    UiText_CopyMessageString(0x8ee, buffer, 52);
                } else if ((entries[result] & ITEM_BROKEN) != 0) {
                    UiText_CopyMessageString(0x8ec, buffer, 52);
                } else {
                    UiText_CopyMessageString(0x8eb, buffer, 52);
                }
                Ui_FillVramBlockPattern();
                UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
            } else {
                result = -1;
                break;
            }
        } else if (cell->active == 0 || (gKeyState & KEY_B) != 0) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }

        if (count != 0) {
            if ((gKeysRepeat & KEY_DOWN) != 0) {
                AudioCommand_PlayFar(111);
                row++;
                if (row == PAGE_ROWS || page + row == count) {
                    row = 0;
                }
                preferred_row = row;
            } else if ((gKeysRepeat & KEY_UP) != 0) {
                AudioCommand_PlayFar(111);
                row--;
                if (row < 0) {
                    if (page == (count - 1) / PAGE_ROWS * PAGE_ROWS) {
                        row = count - page - 1;
                    } else {
                        row = PAGE_ROWS - 1;
                    }
                }
                preferred_row = row;
            } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page + PAGE_ROWS >= count) {
                    if (page != 0) {
                        page = 0;
                        row = preferred_row;
                    }
                } else {
                    page += PAGE_ROWS;
                    row = preferred_row;
                    if (page == (count - 1) / PAGE_ROWS * PAGE_ROWS) {
                        row = count - page - 1;
                        if (row > preferred_row) {
                            row = preferred_row;
                        }
                    }
                }
            } else if ((gKeysRepeat & KEY_LEFT) != 0) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page != 0) {
                    page -= PAGE_ROWS;
                    row = preferred_row;
                } else {
                    page = (count - 1) / PAGE_ROWS * PAGE_ROWS;
                    row = preferred_row;
                    if (page != 0) {
                        row = count - page - 1;
                        if (row > preferred_row) {
                            row = preferred_row;
                        }
                    }
                }
            }
        }
        WaitFrames(1);
    }

    UiWork_Finalize(message_window, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    Resource_ResetEntry(cursor_handle);
    for (i = 0; i < PAGE_ROWS; i++) {
        Resource_ResetEntry(handles[i]);
    }
    WaitFrames(1);
    return result;
}
