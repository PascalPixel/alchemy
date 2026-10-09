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

void Vram_CopyTile(u32 source_tile, u32 destination_tile)
{
    Dma_Set((u8 *)0x06000000 + (source_tile & 0x3ff) * 32,
            (u8 *)0x06000000 + (destination_tile & 0x3ff) * 32,
            0x84000008, REG_DMA3);
}

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
            /* FAKEMATCH: removing this remaining barrier gives EN differ 54. One extra dead ((u32 *)cur)[2] = 0 after the cursor clear restores every spill slot: reference/built 770, common 764, differ 12, ten displayed rows of page-call arguments and dirty-pointer load order remain. */
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
        /* FAKEMATCH: a one-pass block finishes these stores before input is read. The duplicate final store is removed by flow but preserves the native spill-slot order; with one store, 42 instructions differ only in their stack offsets. */
        cell->preferred_row = preferred_row;
        cell->preferred_row = preferred_row;
        do { } while (0);

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

struct BtlAct * BtlActGet(s32 action_id);

#define UiWindow_DrawThreeTileColumn ((void (*)(struct UiWindow*,s32,s32,s32,s32))UiWindow_DrawThreeTileColumn)

#define ACTION_ID_MASK 0x3fff

s32 BattleMenu_RunActionSelection(s32 unit_id, u16 *actions, s32 count)
{
    struct UiRenderWork *render;
    s32 drawn_page;
    s32 page;
    s32 drawn_row;
    s32 cursor_handle;
    struct BtlUnit *unit;
    struct UiWindow *message_window;
    s32 icon_count;
    s32 preferred_row;
    struct MenuPoint pos;
    struct MenuCell *nav;
    struct UiWindow *window;
    s32 row;
    s32 entry;
    s32 tile;
    s32 edge;
    s32 range;
    s32 result;
    s32 i;
    struct MenuSprite cursor;
    struct MenuSprite *spr;
    u16 buffer[64];
    s32 handles[BATTLE_LIST_ROWS];
    struct MenuSprite sprites[BATTLE_LIST_ROWS];
    s32 glyph;

    render = (struct UiRenderWork *)gWindowWork[0];
    drawn_row = -1;
    drawn_page = -1;
    cursor_handle = Resource_LoadIntoFreeSlot(128);
    unit = UnitGetFar(unit_id);
    message_window = UiWindow_Create(0, 5, 30, 4, 42);
    icon_count = BATTLE_LIST_ROWS;
    /* FAKEMATCH: needed to match: a one-pass block restores the saved row before the list window */
    do {
        page = ((struct MenuCell *)gWindowWork[42])->page;
        row = ((struct MenuCell *)gWindowWork[42])->row;
        preferred_row = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    } while (0);
    window = UiWindow_Create(9, 9, 21, BATTLE_LIST_ROWS * 2 + 1, 6);

    for (i = 0; i < BATTLE_LIST_ROWS; i++) {
        s32 y = i * 2;

        spr = &sprites[i];
        /* FAKEMATCH: the block that runs once keeps y = i * 2 first. */
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
#if EDITION_INTERNATIONAL
                UiText_CopyMessageString(
                    (actions[page + row] & ACTION_ID_MASK) +
                        (s32)MsgAbilityDescription,
                    buffer,
                    52);
#else
                UiText_DecodeMessage(
                    (actions[page + row] & ACTION_ID_MASK) +
                        (s32)MsgAbilityDescription,
                    buffer,
                    32);
#endif
            } else {
#if EDITION_INTERNATIONAL
                UiText_CopyMessageString(MSGNO(MsgBattleNoPsynergy), buffer, 52);
#else
                UiText_DecodeMessage(MSGNO(MsgBattleNoPsynergy), buffer, 32);
#endif
            }
            UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
            drawn_row = row;

            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(window);
                for (i = 0; i < BATTLE_LIST_ROWS; i++) {
                    struct BtlAct *action;

                    entry = actions[page + i];
                    if (entry == 0) {
                        break;
                    }
                    action = BtlActGet(entry);
                    UiWindow_SetTilemapEntry(window, 0xf01f, 11, i * 2, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                    UiWindow_SetTilemapEntry(window, 0xf005, 12, i * 2, 0);
#else
                    UiWindow_SetTilemapEntry(window, 0xf01e, 12, i * 2, 0);
#endif
                    Ability_LoadGlyph(
                        entry & ACTION_ID_MASK, 0, &handles[i], &glyph, 1);
                    sprites[i].oam.f.tile = glyph;

                    if ((action->tgt_flg & ACTION_USABLE_IN_BATTLE) == 0) {
                        UiWork_SetParamNibble(4);
                    } else if (action->pp_cost > unit->ep) {
                        UiWork_SetParamNibble(2);
                    } else if (unit->fuuji != 0) {
                        UiWork_SetParamNibble(9);
                    }

                    render->level = 5;
                    UiText_DrawCharacterAtOffset(
                        entry + (s32)MsgAbilityName,
                        (struct RenderInput *)window, 16, i * 16);
                    UiText_DrawNumberAtOffset(
                        action->pp_cost, 2, (struct RenderInput *)window,
                        104, i * 16);
                    UiWork_SetParamNibble(15);
                    render->level = 15;

                    if (action->damage_class != 4) {
                        UiWindow_SetTilemapEntry(
                            window, action->damage_class + 0x5001, 15, i * 2, 0);
                    }

                    range = action->rng;
                    if (range == 255) {
                        range = 11;
                    } else {
                        range--;
                    }
                    UiWindow_DrawThreeTileColumn(window, 16, i * 2, range, 0);
                }
                icon_count = i;
                drawn_page = page;
            }

            if (count > BATTLE_LIST_ROWS) {
                for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++) {
                    tile = i + 0xf301;
                    if (i == page / BATTLE_LIST_ROWS) {
                        tile = i + 0xf30b;
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
                tile = i + 0xf301;
                if ((gFrameCount & 15) <= 11) {
                    if (i == page / BATTLE_LIST_ROWS) {
                        tile = i + 0xf30b;
                    }
                }
                UiWindow_SetTilemapEntry(
                    window,
                    tile,
                    window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2,
                    -1,
                    0);
            }
            /* FAKEMATCH: this one-pass block is needed to match the page-call
             * arguments and render reload order (ten instructions). */
            do {
                edge = -1;
                UiWindow_SetTilemapEntry(
                    window, 0xf334, window->width - (count + 4) / BATTLE_LIST_ROWS - 3, edge, 0);
                UiWindow_SetTilemapEntry(
                    window, 0xf335, window->width - 2, edge, 0);
            } while (0);
            render->dirty |= 2 << ((u32)(window->y - 1) >> 2);
        }

        for (i = 0; i < icon_count; i++) {
            Runtime_PushSlotEntry(&sprites[i], 240);
        }

        pos.x = window->x * 8 - 4;
        pos.y = (row * 2 + window->y) * 8 + 20;
        ((u32 *)spr)[1] = 0x40000000;
        ((u32 *)spr)[2] = 0;
        /* FAKEMATCH: this dead duplicate is needed to match the native spill
         * slots; removing it changes 42 aligned instructions. */
        ((u32 *)spr)[2] = 0;
        spr->oam.f.tile =
            Resource_GetBuffer(cursor_handle, (s32)Resource_FixedBlockBTiles);
        spr->oam.f.x = pos.x + ((gFrameCount & 4) >> 1) - 4;
        spr->oam.f.y = pos.y - ((gFrameCount & 4) >> 2) - 8;
        if (count != 0) {
            Runtime_PushSlotEntry(spr, 242);
        }

        nav = gLinkCountdownWork;
        nav->page = page;
        nav->row = row;
        nav->preferred_row = preferred_row;

        if ((gKeyState & KEY_A) != 0) {
            if (count != 0) {
                result = page + row;
                if ((BtlActGet(actions[result])->tgt_flg & ACTION_USABLE_IN_BATTLE) != 0) {
                    break;
                }
                /* FAKEMATCH: a dead store here keeps the test above from being threaded past the break. */
                result = -1;
            } else {
                result = -1;
                break;
            }
        } else if (nav->active == 0 || (gKeyState & KEY_B) != 0) {
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
                    if (page ==
                        (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS) {
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
                    row = preferred_row;
                    page -= BATTLE_LIST_ROWS;
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
    for (i = 0; i < BATTLE_LIST_ROWS; i++) {
        Resource_ResetEntry(handles[i]);
    }
    Resource_ResetEntry(cursor_handle);
    WaitFrames(1);
    return result;
}

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
    /* FAKEMATCH: removing both edition pin declarations and the t barrier together gives EN 1585/1585, common 1343, differ 484: infoWin spills and the frame shrinks 324 -> 320. ES 1684/1684, common 1569, differ 230: infoWin/sel swap r10/r9 and spill slots permute. Both extents stay unchanged. */
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    struct UiWindow *infoWin;
#else
    /* FAKEMATCH: register choice only: as an ordinary local the info window is spilled and the frame shrinks to 320 bytes (differ 492); the native keeps it in r9. */
    register struct UiWindow *infoWin asm("r9");
#endif
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
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    /* FAKEMATCH: this value-preserving null initializer is needed to match
     * infoWin in r10; a direct zero swaps high registers and spill slots. */
    infoWin = (struct UiWindow *)((u32)(u16)sel >> 16);
#else
    infoWin = 0;
#endif
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
                entry = &head[i];
                slot = &tbl[selSlot[i]];
                BattleMotion_ProjectConditionalPositionFar(selIds[i], (s32 *)&targetPos);
                targetPos.y = targetPos.y + Trig_Sin(gFrameCount << 12) / 32768;
                *entry = *head;
                if (slot->flags & 1) {
                    s32 x = (targetPos.x + slot->x) / 2;
                    s32 y = (targetPos.y + slot->y) / 2;

                    targetPos.x = x;
                    slot->x = x;
                    targetPos.y = y;
                    slot->y = y;
                } else {
                    s32 x = entry->x;
                    s32 y = entry->y + 8;

                    slot->flags = 1;
                    targetPos.x = x;
                    slot->x = x;
                    targetPos.y = y;
                    slot->y = y;
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

#define gLinkCountdownWork (*(struct LinkCountdownState * *)&gLinkCountdownWork)
#define Ui_FillVramBlockPattern ((unsigned char (*)(void))Ui_FillVramBlockPattern)
#define UiWindow_Create ((s32 (*)(s32, s32, s32, s32, s32))UiWindow_Create)
#define UiText_CopyMessageString ((s32 (*)(s32, s16 *, s32))UiText_CopyMessageString)
#define UiText_DecodeMessage ((void (*)(s32, s16 *, s32))UiText_DecodeMessage)

#define UiText_RenderWideStringAtOffset ((void (*)(s16*,s32,s32,s32))UiText_RenderWideStringAtOffset)

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

struct CountdownDisplayEntry {
    u8 data[12];
};

struct LinkSignature {
    u8 padding_000[8];
    u16 e;
    u16 d;
    u16 v;
    u16 s;
    u8 padding_010[8];
};

struct LinkCountdownState {
    struct CountdownDisplayEntry entries[3];
    u8 entryActive[3];
    u8 padding_027;
    s32 targetOffset;
    s32 currentOffset;
    u8 padding_030[20];
    s32 displayHandle;
    s32 secondaryHandle;
    s32 timer;
    s32 enabled;
};

extern struct LinkSignature gLinkPeerSignatures[];
void UiText_DrawNumberInWindow(s32 value, s32 width, s32 handle, s32 arg3, s32 arg4);
void AudioCommand_PlayFar(s32 soundId);

void UpdateLinkSessionCountdown(void)
{
    struct LinkCountdownState *state = gLinkCountdownWork;
    struct BtlWork *runtime;
    struct CountdownDisplayEntry *entry;
    u8 *active;
    struct LinkSignature *signature;
    s32 target;
    s32 current;
    s32 difference;
    s32 step;
    s32 newOffset;
    s32 timer;
    s32 nextTimer;
    s32 seconds;
    s32 i;

    if (state != 0) {
        target = state->targetOffset;
        current = state->currentOffset;
        if (target != current) {
            difference = target - current;
            step = difference / 3;
            if (step == 0) {
                step--;
                if (difference >= 0)
                    step = 1;
            }
            newOffset = current + step;
            state->currentOffset = newOffset;
            Camera_ConfigureSceneFar(newOffset);
        }

        entry = state->entries;
        active = state->entryActive;
        i = 2;
        do {
            if (*active++ != 0)
                Runtime_PushSlotEntry(entry, 240);
            i--;
            entry++;
        } while (i >= 0);
        Link_DrawShiftedTilePair(0x06006680);

        if (state->enabled == 0)
            goto done;

        runtime = BtlWk;
        if (runtime->link_paused == 0)
            goto load_timer;
        state->timer = 0;
        goto done;

signature_tail:
        if (signature->v != 'V')
            goto signature_done;
        if (signature->s != 'S')
            goto signature_done;
        state->timer = COUNTDOWN_START_FRAMES;
        timer = state->timer;
        goto signature_done;

load_timer:
        timer = state->timer;
        if (timer >= 0)
            goto timer_ready;
        signature = &gLinkPeerSignatures[runtime->link_side ^ 1];
        if (signature->e != 'E')
            goto signature_done;
        if (signature->d == 'D')
            goto signature_tail;

signature_done:
        if (timer < 0)
            goto done;

timer_ready:
        if (state->displayHandle == 0 && state->secondaryHandle == 0) {
            state->displayHandle = Link_CreateCountdownLabelWindow();
            timer = state->timer;
        }

        if (timer > 0) {
            nextTimer = timer - 1;
            state->timer = nextTimer;
            timer = nextTimer;
        }
        if (timer < 0)
            goto done;

        seconds = (timer + FRAMES_PER_SECOND - 1) / FRAMES_PER_SECOND;
        if (seconds != 0 && seconds *FRAMES_PER_SECOND == timer)
            AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);

        if (state->displayHandle != 0)
            UiText_DrawNumberInWindow(seconds, 2, state->displayHandle, 16, 8);
    }

done:
    return;
}

#if EDITION_INTERNATIONAL
#define TEXT_COUNT 52
#else
#define TEXT_COUNT 32
#endif

#if EDITION_INTERNATIONAL


#else


#endif

s32 UiText_ShowLocalizedMessageAndWait(void)
{
    s16 buffer[64];
    s32 work;
    s32 result;
    struct BtlWork *battle;
    s32 mode;

    battle = BtlWk;
    mode = gGameState.battle_start;
    if (gGameState.battle_start == 2 || mode <= 2 || (result = 1, mode > 4)) {
        result = 0;
    }
    if (result == 0) {
        if ((s8)battle->chouhatsu_flg != 0) {
            result = 1;
        }
        if (result != 0) {
            goto active;
        }
    } else {
active:
        work = UiWindow_Create(0, 7, 30, 4, 42);
        Ui_FillVramBlockPattern();
#if EDITION_INTERNATIONAL
        UiText_CopyMessageString((s32)MsgNoTimeToRun, buffer, TEXT_COUNT);
#else
        UiText_DecodeMessage((s32)MsgNoTimeToRun, buffer, TEXT_COUNT);
#endif
        UiText_RenderWideStringAtOffset(buffer, work, 0, 4);
        do {
            WaitFrames(1);
        } while ((*(volatile u32 *)&gKeyState & (KEY_A | KEY_B)) == 0 &&
                 gLinkCountdownWork->timer != 0);
        UiWork_Finalize(work, 1);
    }
    return result;
}
