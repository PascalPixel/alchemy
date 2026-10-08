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
#include "io_reg.h"
#include "menu_list.h"
#include "item.h"
#include "text/msg_ids.h"
struct ItemDefinition *Item_GetDirectFar(s32 item);



enum BattleListTile {
    LIST_TILE_PAGE_NUMBER = 0xf301,
    LIST_TILE_PAGE_NUMBER_LIT = 0xf30b,
    LIST_TILE_PAGES_LEFT = 0xf334,
    LIST_TILE_PAGES_RIGHT = 0xf335
};

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
    struct CursorSprite cursor;
    struct MenuSprite *spr;
    struct CursorSprite *cur;
    u16 buffer[64];
    s32 handles[BATTLE_LIST_ROWS];
    struct MenuSprite sprites[BATTLE_LIST_ROWS];

    render = (struct UiRenderWork *)gWindowWork[0];
    drawn_page = -1;
    drawn_row = -1;
    cursor_handle = Resource_LoadIntoFreeSlot(128);
    message_window = UiWindow_Create(0, 5, 30, 4, 42);
    icon_count = 0;
    /* FAKEMATCH: needed to match: a one-pass block restores the saved row before the window call */
    do {
        page = ((struct MenuCell *)gWindowWork[42])->page;
        row = ((struct MenuCell *)gWindowWork[42])->row;
        preferred_row = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    } while (0);
    window = UiWindow_Create(15, 9, 15, BATTLE_LIST_ROWS * 2 + 1, 6);

    for (i = 0; i < BATTLE_LIST_ROWS; i++) {
        s32 y = i * 2;

        spr = &sprites[i];
        /* FAKEMATCH: the one-pass block keeps both attribute words ahead of the x/y field stores. */
        do {
            sprites[i].oam.word.attr01 = MENU_SPRITE_SIZE_16;
            sprites[i].oam.word.attr23 = 0;
        } while (0);
        spr->oam.f.x = window->x * 8 + 8;
        spr->oam.f.y = (y + window->y) * 8 + 4;
    }

    for (i = 0; i < BATTLE_LIST_ROWS; i++) {
        handles[i] = Resource_LoadIntoFreeSlot(128);
        sprites[i].oam.f.tile = Resource_GetBuffer(handles[i], -1);
    }

    Vram_CopyTile(0xf018, 0x200);
    Vram_CopyTile(0xf018, 0x201);
    Vram_CopyTile(0xf019, 0x210);
    Vram_CopyTile(0xf019, 0x211);

    cur = &cursor;
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
#if EDITION_INTERNATIONAL
                    UiText_CopyMessageString((s32)MsgBattleItemUnavailable, buffer, 52);
#else
                    UiText_DecodeMessage((s32)MsgBattleItemUnavailable, buffer, 32);
#endif
                } else {
#if EDITION_INTERNATIONAL
                    UiText_CopyMessageString(
                        (entries[page + row] & ITEM_ID_MASK) + (s32)MsgItemPlainName,
                        buffer,
                        52);
#else
                    UiText_DecodeMessage(
                        (entries[page + row] & ITEM_ID_MASK) + (s32)MsgItemPlainName,
                        buffer,
                        32);
#endif
                }
            } else {
#if EDITION_INTERNATIONAL
                UiText_CopyMessageString((s32)MsgBattleNoItems, buffer, 52);
#else
                UiText_DecodeMessage((s32)MsgBattleNoItems, buffer, 32);
#endif
            }
            UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
            drawn_row = row;

            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(window);
                for (i = 0; i < BATTLE_LIST_ROWS; i++) {
                    entry = entries[page + i];
                    if (entry == 0) {
                        break;
                    }
                    Item_GetDirectFar(entry);
                    UiWork_SetParamNibble(15);
                    if (Item_ClassifyUseAbility(owner, entry) != 0) {
                        UiWork_SetParamNibble(4);
                    } else if ((entry & INVENTORY_BROKEN) != 0) {
                        UiWork_SetParamNibble(2);
                    }
                    UiText_DrawCharacterAtOffset(
                        (entry & ITEM_ID_MASK) + (s32)MsgItemName,
                        (struct RenderInput *)window, 16, i * 16);
                    UiWork_SetParamNibble(15);
                    sprites[i].oam.f.tile =
                        Resource_LoadKind26EntryToBuffer(entry, handles[i]);
                }
                icon_count = i;
                drawn_page = page;
            }

            if (count > BATTLE_LIST_ROWS) {
                for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
                    tile = i + LIST_TILE_PAGE_NUMBER;
                    if (i == page / BATTLE_LIST_ROWS) {
                        tile = i + LIST_TILE_PAGE_NUMBER_LIT;
                    }
                    UiWindow_SetTilemapEntry(
                        window,
                        tile,
                        window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2,
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

        if (count > BATTLE_LIST_ROWS) {
            for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
                tile = i + LIST_TILE_PAGE_NUMBER;
                if ((gFrameCount & 15) <= 11) {
                    if (i == page / BATTLE_LIST_ROWS) {
                        tile = i + LIST_TILE_PAGE_NUMBER_LIT;
                    }
                }
                UiWindow_SetTilemapEntry(
                    window,
                    tile,
                    window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2,
                    -1,
                    0);
            }
            edge = -1;
            UiWindow_SetTilemapEntry(
                window, LIST_TILE_PAGES_LEFT, window->width - (count + 4) / BATTLE_LIST_ROWS - 3, edge, 0);
            UiWindow_SetTilemapEntry(
                window, LIST_TILE_PAGES_RIGHT, window->width - 2, edge, 0);
            /* FAKEMATCH: reload render work before its dirty-field offset. */
            __asm__ volatile ("" : : "r" (render));
            render->dirty |= 2 << ((u32)(window->y - 1) >> 2);
        }

        for (i = 0; i < icon_count; i++) {
            spr = &sprites[i];
            Runtime_PushSlotEntry(spr, 240);
        }

        pos.x = window->x * 8 - 2;
        pos.y = (row * 2 + window->y) * 8 + 20;
        /* FAKEMATCH: re-pointing cur each frame orders the spill slots of the cursor address and row*2 as the native frame does. */
        cur = &cursor;
        ((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
        ((u32 *)cur)[2] = 0;
        cur->tile =
            Resource_GetBuffer(cursor_handle, (s32)Resource_FixedBlockBTiles);
        cur->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
        cur->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
        if (count != 0) {
            Runtime_PushSlotEntry(cur, 242);
        }

        cell = gLinkCountdownWork;
        cell->page = page;
        cell->row = row;
        cell->preferred_row = preferred_row;
        /* FAKEMATCH: finish saving the selection before reading input. */
        __asm__ volatile ("");

        if ((gKeyState & KEY_A) != 0) {
            if (count != 0) {
                result = page + row;
                Item_GetDirectFar(entries[result]);
                reason = 0;
                if ((entries[result] & INVENTORY_BROKEN) == 0) {
                    reason = Item_ClassifyUseAbility(owner, entries[result]);
                    if (reason == 0) {
                        break;
                    }
                }
                AudioCommand_PlayFar(114);
                if (reason == 2) {
#if EDITION_INTERNATIONAL
                    UiText_CopyMessageString((s32)MsgBattleItemUnavailable, buffer, 52);
#else
                    UiText_DecodeMessage((s32)MsgBattleItemUnavailable, buffer, 32);
#endif
                } else if ((entries[result] & INVENTORY_BROKEN) != 0) {
#if EDITION_INTERNATIONAL
                    UiText_CopyMessageString((s32)MsgBattleItemBroken, buffer, 52);
#else
                    UiText_DecodeMessage((s32)MsgBattleItemBroken, buffer, 32);
#endif
                } else {
#if EDITION_INTERNATIONAL
                    UiText_CopyMessageString((s32)MsgBattleItemNoEffect, buffer, 52);
#else
                    UiText_DecodeMessage((s32)MsgBattleItemNoEffect, buffer, 32);
#endif
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
                if (row == BATTLE_LIST_ROWS || page + row == count) {
                    row = 0;
                }
                preferred_row = row;
            } else if ((gKeysRepeat & KEY_UP) != 0) {
                AudioCommand_PlayFar(111);
                row--;
                if (row < 0) {
                    if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
                        row = count - page - 1;
                    } else {
                        row = BATTLE_LIST_ROWS - 1;
                    }
                }
                preferred_row = row;
            } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page + BATTLE_LIST_ROWS >= count) {
                    if (page != 0) {
                        page = 0;
                        row = preferred_row;
                    }
                } else {
                    page += BATTLE_LIST_ROWS;
                    row = preferred_row;
                    if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
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
                    page -= BATTLE_LIST_ROWS;
                    row = preferred_row;
                } else {
                    page = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
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
    for (i = 0; i < BATTLE_LIST_ROWS; i++) {
        Resource_ResetEntry(handles[i]);
    }
    WaitFrames(1);
    return result;
}
