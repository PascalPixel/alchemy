#include "PROTO.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "BATTLE_TYPES.H"
#include "MENU_LIST.H"
#include "text/MSG_IDS.H"
#include "BATTLE_RUNTIME.H"


#define ACTION_ID_MASK 0x3fff


void UiWindow_DrawThreeTileColumn(
    struct UiWindow *window, s32 x, s32 y, s32 range, s32 layer);

s32 BattleMenu_RunActionSelection(s32 unit_id, u16 *actions, s32 count)
{
    struct UiRenderWork *render;
    s32 drawn_page;
    s32 page;
    s32 drawn_row;
    s32 cursor_handle;
    struct BattleUnit *unit;
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
    unit = Owner_GetStateFar(unit_id);
    message_window = UiWindow_Create(0, 5, 30, 4, 42);
    icon_count = BATTLE_LIST_ROWS;
    page = ((struct MenuCell *)gWindowWork[42])->page;
    row = ((struct MenuCell *)gWindowWork[42])->row;
    preferred_row = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    /* FAKEMATCH: finish restoring the row before setting up the list window. */
    __asm__ volatile ("");
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
                    struct BattleAction *action;

                    entry = actions[page + i];
                    if (entry == 0) {
                        break;
                    }
                    action = Ability_GetData(entry);
                    UiWindow_SetTilemapEntry(window, 0xf01f, 11, i * 2, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                    UiWindow_SetTilemapEntry(window, 0xf005, 12, i * 2, 0);
#else
                    UiWindow_SetTilemapEntry(window, 0xf01e, 12, i * 2, 0);
#endif
                    Ability_LoadGlyph(
                        entry & ACTION_ID_MASK, 0, &handles[i], &glyph, 1);
                    sprites[i].oam.f.tile = glyph;

                    if ((action->target_flags & ACTION_USABLE_IN_BATTLE) == 0) {
                        UiWork_SetParamNibble(4);
                    } else if (action->pp_cost > unit->pp) {
                        UiWork_SetParamNibble(2);
                    } else if (unit->psy_seal != 0) {
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

                    range = action->range;
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
            edge = -1;
            UiWindow_SetTilemapEntry(
                window, 0xf334, window->width - (count + 4) / BATTLE_LIST_ROWS - 3, edge, 0);
            UiWindow_SetTilemapEntry(
                window, 0xf335, window->width - 2, edge, 0);

            /* FAKEMATCH: reload render work before its dirty-field offset. */
            __asm__ volatile ("" : : "r" (render));
            render->dirty |= 2 << ((u32)(window->y - 1) >> 2);
        }

        for (i = 0; i < icon_count; i++) {
            Runtime_PushSlotEntry(&sprites[i], 240);
        }

        pos.x = window->x * 8 - 4;
        pos.y = (row * 2 + window->y) * 8 + 20;
        ((u32 *)spr)[1] = 0x40000000;
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
                if ((Ability_GetData(actions[result])->target_flags & ACTION_USABLE_IN_BATTLE) != 0) {
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
