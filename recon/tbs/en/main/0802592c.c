#include "TYPES.H"
#include "BATTLE_TYPES.H"
#include "MENU_LIST.H"

/*
 * Battle action (Psynergy) selection loop.
 *
 * Draws one page of at most five entries from `actions` into a 21x11 window,
 * with a description line in a second full-width window, a page indicator
 * when the list is longer than one page, five entry glyph sprites and a
 * blinking cursor sprite. It then blocks, one frame at a time, handling the
 * four direction keys plus confirm and cancel, and returns the selected index
 * into `actions` or -1 when the caller cancelled. Message 2279, shown for an
 * empty list, has no name in the catalogs yet.
 *
 * Remaining difference (score 691 from 13327: eleven reordered instructions
 * and one rotation of five stack slots, no other difference):
 * - sched2 ties in four places: the two stores after the first window, the
 *   move of `row` into r10, the stack argument of the first highlight, and
 *   the two page-arrow calls. Each is decided by which stores and loads the
 *   scheduler thinks may alias, so by the types the memory is reached through.
 * - the glyph load before the tile store in the entry loop: the listing shows
 *   no dependence between them, which a tile field reached through the union
 *   below cannot give (a union access aliases everything).
 * - the slots of the five addresses gcse hoists follow its hash table, whose
 *   size is the insn count; it settles when the above does.
 *
 * What the listing fixed, and the other list screens share:
 * - a declaration at the top of the entry loop body: the block note it leaves
 *   after the loop note makes the loop pass call the loop phony, so nothing in
 *   it is hoisted or strength-reduced, and `page` loses its register to the
 *   short-lived values of that loop.
 * - the cursor position is one two-word struct: set a field at a time it is
 *   live around the whole loop, so it lives on the stack.
 * - the render work flags are struct fields, not bytes of a u8 array: a char
 *   store would order itself before every later load.
 * - the cursor words are stored through u32 casts, the sprite words of the
 *   first loop through the union; the latter is what keeps window->x inside
 *   that loop. One of the two is still not the original shape.
 * - the divisions are written as divisions, so that the page count is reused
 *   after the second indicator loop and recomputed inside it.
 * - `spr` is one pointer for the row sprites and the cursor.
 * - a statement the compiler drops stands after the confirm test; without it
 *   the test is threaded to the key handling instead of inverted.
 */

#define ACTION_ID_MASK 0x3fff
#define PAGE_ROWS MENU_PAGE_ROWS

extern u8 MsgAbilityName;
extern u8 MsgAbilityDescription;

void Ability_LoadGlyph(s32 action, s32 base, s32 *source, s32 *tile, s32 reuse);
void UiWindow_DrawThreeTileColumn(
    struct UiWindow *window, s32 x, s32 y, s32 range, s32 layer);
struct BattleUnit *Owner_GetStateFar(s32 unit_id);
struct BattleAction *Ability_GetData(s32 action);

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
    s32 range;
    s32 result;
    s32 i;
    struct MenuSprite cursor;
    struct MenuSprite *spr;
    s16 buffer[64];
    s32 handles[PAGE_ROWS];
    struct MenuSprite sprites[PAGE_ROWS];
    s32 glyph;

    render = (struct UiRenderWork *)gWindowWork[0];
    drawn_row = -1;
    drawn_page = -1;
    cursor_handle = Resource_LoadIntoFreeSlot(128);
    unit = Owner_GetStateFar(unit_id);
    message_window = UiWindow_Create(0, 5, 30, 4, 42);
    icon_count = PAGE_ROWS;
    page = ((struct MenuCell *)gWindowWork[42])->page;
    row = ((struct MenuCell *)gWindowWork[42])->row;
    preferred_row = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    window = UiWindow_Create(9, 9, 21, 11, 6);

    for (i = 0; i <= 4; i++) {
        s32 y = i * 2;

        spr = &sprites[i];
        /* FAKEMATCH: the block that runs once keeps y = i * 2 first. */
        do {
            sprites[i].oam.word.attr01 = 0x40000000;
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
                UiText_CopyMessageString(
                    (actions[page + row] & ACTION_ID_MASK) +
                        (s32)&MsgAbilityDescription,
                    buffer,
                    52);
            } else {
                UiText_CopyMessageString(2279, buffer, 52);
            }
            UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
            drawn_row = row;

            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(window);
                for (i = 0; i < PAGE_ROWS; i++) {
                    struct BattleAction *action;

                    entry = actions[page + i];
                    if (entry == 0) {
                        break;
                    }
                    action = Ability_GetData(entry);
                    UiWindow_SetTilemapEntry(window, 0xf01f, 11, i * 2, 0);
                    UiWindow_SetTilemapEntry(window, 0xf01e, 12, i * 2, 0);
                    Ability_LoadGlyph(
                        entry & ACTION_ID_MASK, 0, &handles[i], &glyph, 1);
                    sprites[i].oam.f.tile = glyph;

                    if ((action->target_flags & 0x80) == 0) {
                        UiWork_SetParamNibble(4);
                    } else if (action->pp_cost > unit->pp) {
                        UiWork_SetParamNibble(2);
                    } else if (unit->psy_seal != 0) {
                        UiWork_SetParamNibble(9);
                    }

                    render->level = 5;
                    UiText_DrawCharacterAtOffset(
                        entry + (s32)&MsgAbilityName, window, 16, i * 16);
                    UiText_DrawNumberAtOffset(
                        action->pp_cost, 2, window, 104, i * 16);
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
            UiWindow_SetTilemapEntry(
                window, 0xf334, window->width - (count + 4) / PAGE_ROWS - 3, -1, 0);
            UiWindow_SetTilemapEntry(
                window, 0xf335, window->width - 2, -1, 0);
            render->dirty |=
                2 << ((u32)(window->y - 1) >> 2);
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

        if ((gKeyState & 1) != 0) {
            if (count != 0) {
                result = page + row;
                if ((Ability_GetData(actions[result])->target_flags & 0x80) != 0) {
                    break;
                }
                /* FAKEMATCH: a dead store here keeps the test above from
                   being threaded past the break. */
                result = -1;
            } else {
                result = -1;
                break;
            }
        } else if (nav->active == 0 || (gKeyState & 2) != 0) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }

        if (count != 0) {
            if ((gKeysRepeat & 128) != 0) {
                AudioCommand_PlayFar(111);
                row++;
                if (row == PAGE_ROWS || page + row == count) {
                    row = 0;
                }
                preferred_row = row;
            } else if ((gKeysRepeat & 64) != 0) {
                AudioCommand_PlayFar(111);
                row--;
                if (row < 0) {
                    if (page == (count - 1) / PAGE_ROWS * PAGE_ROWS) {
                        row = count - page - 1;
                    } else {
                        row = 4;
                    }
                }
                preferred_row = row;
            } else if ((gKeysRepeat & 16) != 0) {
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
                    if (page ==
                        (count - 1) / PAGE_ROWS * PAGE_ROWS) {
                        row = count - page - 1;
                        if (row > preferred_row) {
                            row = preferred_row;
                        }
                    }
                }
            } else if ((gKeysRepeat & 32) != 0) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                if (page != 0) {
                    row = preferred_row;
                    page -= PAGE_ROWS;
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
    for (i = 0; i < PAGE_ROWS; i++) {
        Resource_ResetEntry(handles[i]);
    }
    Resource_ResetEntry(cursor_handle);
    WaitFrames(1);
    return result;
}
