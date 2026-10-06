/* 08024934: SummonMenu_SelectSummon draft.
 * Native 946 instructions, draft 924, common 537, differ 796.
 * Remaining: native 372-byte frame versus 380, spill/register allocation,
 * sprite setup/load order, and consequent branches and literal-pool placement.
 * The scan of available and the compaction into ordered now match apart from
 * slot offsets and one reload through ip for the first *have load.
 * Diagnosed: stmt.c's expand_end_loop rolls every exit test within 30 insns
 * of a while loop's top to its bottom, so `while (*need <= *have) { n++;
 * if (n > 3) break; ... }` also moved the n > 3 exit; native keeps only the
 * comparison at the bottom, as `if (c) do { ... } while (c)` gives. Shared
 * function-wide required/supply/element/id/src pseudos crossed calls
 * elsewhere, so the scan's need could not take r0 and id lost a call-saved
 * register; the scan wants its own block locals and an indexed countdown
 * (biv elimination gives native's cmp src, available). In the draw loop,
 * loop.c reduces the tile-store giv index * 12 + 8 (lifetime 10, benefit 11,
 * 124 insns) into slot 8 with a sprites copy at 40; native recomputes it.
 * From 901: if/do-while affordability checks 894 (scan) and 887 (both);
 * block-local need/have/n in the scan 881, plus its own id 862; indexed
 * compaction loop 867, with its own id 848; indexed scan countdown from last
 * 830. Block-local check in the draw loop 839; for-with-break draw loop and y
 * beside the tile store both 830 unchanged. drawn_row = row before mask = 0
 * 825. Indexed display loop (visible[index], &sprites[index]) 796: native's
 * index counts up there, not reversed; index still takes r7 where native
 * leaves it r4 with caller-saves at 4. From 825: used = 0 before the list call
 * 830; counted columns (n * 2 + 13) 827; all-indexed sprite init 825, all
 * through sprite 870, pointer traversal 884; indexed handle and tile loop
 * 878 (native's shape there, offset r5 from 8 and stmia handles, but its
 * base pseudo takes a call-saved register and drawn_page loses r9).
 * Native's draw block uses r9 for a hoisted 0 and r8 for y; drawn_page is
 * dead there. From 796: do { } while (1) main loop 796 unchanged; scalars
 * declared in native slot order (count, window, render, drawn_row,
 * cursor_handle, description, counts, mask, preferred, cursor_position,
 * summon) 838, though every spill slot then sits exactly 8 above native's:
 * reload numbers slots by pseudo, so that order is right once the two
 * draw-loop giv slots go. In a for (;;) loop, loop.c's find_and_verify_loops
 * moves the `result = ordered[page + row]; break;` block out of the loop to
 * the barrier after the scan (b.n -1254 and back); native keeps it inline
 * with result in r6, which a goto main loop (no loop notes) gives: kept at
 * 796 (draft 924 instructions), the -1 tilemap row then takes r6 where
 * native has r5 and native stores a fresh 0 per call. The tile-store giv would stay
 * unreduced at lifetime 1 (119 vs 124, like giv 938), so native's ldrh and
 * strh addresses were probably separate givs at loop time.
 * Native slots, top down: standby 84, count 80, window 76, render 72,
 * drawn_row 68 (drawn_page r9, row r10, page r11), cursor_handle 64,
 * description 60, counts 56, mask 52, preferred 48, cursor_position 40-47,
 * summon 36, ordered 32, sprites 28, cursor 24, count - 1 at 20, row * 2 16,
 * cursor_handle << 16 at 12, &glyph 8, r4 caller-save 4. The draft's extra
 * slots are draw-loop givs (sprite offset index * 12 + 8 and a sprites copy);
 * native recomputes them from index each pass and keeps index in r4.
 *
 * One row local (y = index * 2) for the djinn columns removed the index * 2
 * giv: 914 -> 901. Declaring scalars in native slot order (count, window,
 * render, drawn_row, cursor_handle, description, counts, mask, preferred,
 * cursor_position) gave 956; count and window first alone gave 946. Counting
 * columns (col * 2 + 13) compiled the same as col += 2, with or without y.
 * From 901: y set at the top of the draw loop 1002; y set beside the sprite
 * store 901; indexed summon->djinn_required[element] columns 1116; a sprite
 * pointer for the tile store 924; the tile store after the nibble call 900
 * (against native order, not kept); a counted first pass over available with
 * count - 1 inline instead of last 913, frame 384.
 *
 * Natural trials: early cursor pointer removal 979 -> 969; indexed sprite
 * initialization and handles 977; shared index 963; pointer sprite traversal
 * with indexed handles 914. Moving scalar declarations did not change output.
 * Flat CursorSprite layout worsened the indexed trial to 998; sharing element
 * and column locals worsened it to 1063/1072. A local cursor pointer gave 916,
 * one outside the display loop 920; a short handle left 914 unchanged.
 * A one-pass attribute-store block moved the y shift into native order, 912,
 * but was removed while the wider source shape remains unsettled.
 */
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "BATTLE_SUMMON.H"
#include "MENU_LIST.H"

enum {
    SUMMON_ELEMENT_TILE = 0x5001,
    SUMMON_PAGE_TILE = 0xf301,
    SUMMON_SELECTED_PAGE_TILE = 0xf30b,
    SUMMON_LEFT_ARROW_TILE = 0xf334,
    SUMMON_RIGHT_ARROW_TILE = 0xf335
};

extern u8 MsgAbilityName;
extern u8 MsgAbilityDescription;

void UiWindow_PutGlyph(struct UiWindow *, u32, u32, u32, s32);
void Ability_LoadGlyph(s32, s32, s32 *, s32 *, s32);
s32 Trade_ListFlaggedEntriesFar(u8 *ids);

s32 SummonMenu_SelectSummon(s32 unused0, s32 unused1, const u8 *standby)
{
    struct MenuSprite cursor;
    struct MenuSprite sprites[4];
    u8 available[33];
    u8 ordered[33];
    u16 text[64];
    s32 handles[4];
    u8 visible[4];
    s32 glyph;
    struct UiRenderWork *render;
    struct UiWindow *description, *counts, *window;
    struct MenuCell *nav;
    const struct SummonDefinition *summon;
    struct MenuSprite *sprite;
    const u8 *required, *supply;
    s32 *handle;
    s32 drawn_row, drawn_page, row, page, preferred;
    s32 cursor_handle, count, last, used, mask, element;
    s32 id, tile, affordable;
    struct MenuPoint cursor_position;
    s32 pressed, repeated, result;
    s32 index;

    render = (struct UiRenderWork *)gWindowWork[0];
    drawn_row = -1;
    drawn_page = -1;
    cursor_handle = (s16)Resource_LoadIntoFreeSlot(128);
    description = UiWindow_Create(0, 4, 30, 4, 42);
    counts = UiWindow_Create(20, 8, 10, 3, 6);
    mask = 0;
    page = ((struct MenuCell *)gWindowWork[42])->page;
    row = ((struct MenuCell *)gWindowWork[42])->row;
    preferred = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    window = UiWindow_Create(13, 11, 17, 9, 6);
    for (index = 0; index < 4; index++) {
        s32 y = index * 2;

        sprite = &sprites[index];
        sprites[index].oam.word.attr01 = MENU_SPRITE_SIZE_16;
        sprites[index].oam.word.attr23 = 0;
        sprite->oam.f.x = window->x * 8 + 8;
        sprite->oam.f.y = (y + window->y) * 8 + 4;
    }
    sprite = sprites;
    for (index = 0; index < 4; index++) {
        s32 slot = Resource_LoadIntoFreeSlot(128);
        handles[index] = slot;
        sprite->oam.f.tile = Resource_GetBuffer(slot, -1);
        sprite++;
    }
    count = Trade_ListFlaggedEntriesFar(available);
    used = 0;
    last = count - 1;
    {
        s32 i;

        for (i = last; i >= 0; i--) {
            const u8 *need, *have;
            s32 n, k;

            k = available[i];
            need = SummonDefinition_Get(k)->djinn_required;
            have = standby;
            n = 0;
            if (*need <= *have) {
                do {
                    n++;
                    if (n > 3)
                        break;
                    need++;
                    have++;
                } while (*need <= *have);
            }
            if (n == 4) {
                ordered[used++] = k;
                available[i] = 32;
            }
        }
    }
    {
        s32 i, k;

        for (i = 0; i < count; i++) {
            k = available[i];
            if (k != 32)
                ordered[used++] = k;
        }
    }
    ordered[used] = 32;

loop:
    {
        if (page != drawn_page || row != drawn_row) {
            render->menu_busy = 1;
            Ui_SetRectHighlight(window->x + 1, window->y + drawn_row * 2 + 1,
                window->width - 2, 1, 15);
            Ui_FillVramBlockPattern();
            summon = SummonDefinition_Get(ordered[page + row]);
            UiText_CopyMessageString(summon->name_message_id + (s32)&MsgAbilityDescription, text, 52);
            UiText_RenderWideStringAtOffset(text, (struct UiWindow *)description, 0, 4);
            drawn_row = row;
            mask = 0;
            {
                s32 element;

                required = summon->djinn_required;
                for (element = 0; element <= 3; element++) {
                    if (*required++ != 0)
                        mask |= 1 << element;
                }
            }
            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(window);
                {
                    s32 element, col;

                    col = 1;
                    for (element = 0; element <= 3; element++) {
                        UiWindow_SetTilemapEntry(counts, element + SUMMON_ELEMENT_TILE, element * 2, 0, 0);
                        UiWindow_PutGlyph(counts, standby[element] + 48, col, 0, 0);
                        col += 2;
                    }
                }
                {
                    s32 element, col, y;

                    index = 0;
                    while (index <= 3 && (id = ordered[page + index]) != 32) {
                        summon = SummonDefinition_Get(id);
                        required = summon->djinn_required;
                        supply = standby;
                        element = 0;
                        if (*required <= *supply) {
                            do {
                                element++;
                                if (element > 3)
                                    break;
                                required++;
                                supply++;
                            } while (*required <= *supply);
                        }
                        affordable = element == 4;
                        Ability_LoadGlyph(summon->name_message_id & 0x3fff, 0, &handles[index], &glyph, 1);
                        sprites[index].oam.f.tile = glyph;
                        if (!affordable)
                            UiWork_SetParamNibble(2);
                        UiText_DrawCharacterAtOffset(SummonDefinition_Get(id)->name_message_id + (s32)&MsgAbilityName,
                            (struct RenderInput *)window, 16, index * 16);
                        required = summon->djinn_required;
                        col = 13;
                        y = index * 2;
                        for (element = 0; element <= 3; element++) {
                            if (*required != 0) {
                                UiWindow_SetTilemapEntry(window, element + SUMMON_ELEMENT_TILE, col, y, 0);
                                UiWindow_PutGlyph(window, *required + 48, col + 1, y, 0);
                                col += 2;
                            }
                            required++;
                        }
                        UiWork_SetParamNibble(15);
                        visible[index++] = 1;
                    }
                    while (index <= 3)
                        visible[index++] = 0;
                }
                drawn_page = page;
            }
            if (count > 4) {
                for (index = 0; index < (count + 3) / 4; index++) {
                    tile = index + SUMMON_PAGE_TILE;
                    if (index == page / 4)
                        tile = index + SUMMON_SELECTED_PAGE_TILE;
                    UiWindow_SetTilemapEntry(window, tile,
                        window->width - (count + 3) / 4 + index - 2, -1, 0);
                }
            }
            Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
            render->dirty = 1;
            render->menu_busy = 0;
        }
        for (index = 0; index <= 3; index++) {
            if (visible[index] != 0)
                Runtime_PushSlotEntry((s32 *)&sprites[index], 240);
        }
        cursor_position.x = window->x * 8 - 2;
        cursor_position.y = (row * 2 + window->y) * 8 + 20;
        cursor.oam.word.attr01 = 0x40000000;
        cursor.oam.word.attr23 = 0;
        cursor.oam.f.tile = Resource_GetBuffer((u16)cursor_handle, (s32)Resource_FixedBlockBTiles);
        cursor.oam.f.x = cursor_position.x + ((gFrameCount & 4) >> 1) - 4;
        cursor.oam.f.y = cursor_position.y - ((gFrameCount & 4) >> 2) + 248;
        Runtime_PushSlotEntry((s32 *)&cursor, 242);
        affordable = gFrameCount & 8;
        {
            s32 element;

            for (element = 0; element <= 3; element++) {
                tile = 15 - (affordable != 0);
                if ((mask & (1 << element)) == 0)
                    tile = 15;
                Ui_SetRectHighlight(counts->x + element * 2 + 1, counts->y + 1, 2, 1, tile);
            }
        }
        if (count > 4) {
            for (index = 0; index < (count + 3) / 4; index++) {
                tile = index + SUMMON_PAGE_TILE;
                if ((gFrameCount & 15) <= 11 && index == page / 4)
                    tile = index + SUMMON_SELECTED_PAGE_TILE;
                UiWindow_SetTilemapEntry(window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
            }
            UiWindow_SetTilemapEntry(window, SUMMON_LEFT_ARROW_TILE, window->width - (count + 3) / 4 - 3, -1, 0);
            UiWindow_SetTilemapEntry(window, SUMMON_RIGHT_ARROW_TILE, window->width - 2, -1, 0);
            render->dirty |= 2 << ((u32)(window->y - 1) >> 2);
        }
        nav = gLinkCountdownWork;
        nav->page = page;
        nav->row = row;
        nav->preferred_row = preferred;
        pressed = gKeyState;
        repeated = gKeysRepeat;
        if (nav->auto_enabled != 0) {
            repeated = 0;
            pressed = 0;
            if (nav->auto_delay == 0) {
                nav->auto_delay = 120;
                repeated = 1;
                pressed = 1;
            } else {
                nav->auto_delay--;
            }
        }
        if (pressed & 1) {
            result = ordered[page + row];
            goto done;
        }
        if (gLinkCountdownWork->active == 0 || (pressed & 2)) {
            AudioCommand_PlayFar(113);
            result = -1;
            goto done;
        }
        if (repeated & 128) {
            AudioCommand_PlayFar(111);
            row++;
            if (row == 4 || page + row == count)
                row = 0;
            preferred = row;
        } else if (repeated & 64) {
            AudioCommand_PlayFar(111);
            row--;
            if (row < 0) {
                if (page == (last / 4) * 4)
                    row = count - page - 1;
                else
                    row = 3;
            }
            preferred = row;
        } else if (repeated & 16) {
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            if (page + 4 >= count) {
                if (page != 0) {
                    row = preferred;
                    page = 0;
                }
            } else {
                page += 4;
                row = preferred;
                if (page == (last / 4) * 4) {
                    row = count - page - 1;
                    if (row > preferred)
                        row = preferred;
                }
            }
        } else if (repeated & 32) {
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            if (page != 0) {
                row = preferred;
                page -= 4;
            } else {
                page = (last / 4) * 4;
                row = preferred;
                if (page != 0) {
                    row = count - page - 1;
                    if (row > preferred)
                        row = preferred;
                }
            }
        }
        WaitFrames(1);
    }
    goto loop;
done:
    WaitFrames(1);
    handle = handles;
    index = 3;
    do {
        Resource_ResetEntry(*handle++);
    } while (--index >= 0);
    Resource_ResetEntry((u16)cursor_handle);
    UiWork_Finalize(counts, 1);
    UiWork_Finalize(description, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    return result;
}
