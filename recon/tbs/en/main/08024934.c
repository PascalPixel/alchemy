/* 08024934: SummonMenu_SelectSummon draft.
 * Native 946 instructions, draft 936, common 561, differ 760.
 * Remaining: native 372-byte frame versus 380, spill/register allocation,
 * sprite setup/load order, and consequent branches and literal-pool placement.
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
 *
 * 901 -> 887: the djinn test is `if (*required <= *supply) do { ... } while
 * (*required <= *supply);`. Native's loop head is element++ with no folded
 * first `element > 3` test; plain while, for(;;cond;incr) and ++element forms
 * all fold it (901). Indexed required[element] > supply[element] gave the
 * native loop body but an indexed pre-test (876 with the counted first pass,
 * kept out: native pre-test is through the pointers). The first passes are
 * counted (index = count - 1 .. 0, then 0 .. count - 1) and `(count - 1) / 4`
 * is written inline: GCSE then keeps count - 1 in one pseudo (native slot 20)
 * and rounds a negative one as count + 2, as native does; with last it was
 * last + 3. Native order of spilled scalars as declarations, on this base:
 * 891 (row/page/drawn_page beside drawn_row or after summon alike).
 * Native slot 16 is a GCSE/PRE pseudo row * 2, stored after every row change.
 * 887 -> 833: the two first passes count with their own i, not index. Native
 * keeps index in r4 (caller-saved, slot 4) in every other loop, but the copy
 * pass counts down in r2, so it is another variable; i only in the copy pass
 * 890, only in the first pass 850. Still differing there: native keeps id in
 * r6 and src in r5, required in r0 and element in r4; the draft has id in r4
 * (caller-saved), src r6, required r5, element r0. The same id/index swap
 * recurs in the draw loop (native index r4, id r6; draft index r7, id r4).
 * In greg the draft allocates index (higher priority) before id. From 833:
 * i also in the sprite setup loops 885, in the push loop 929, in the page
 * loops 950 (index is one variable in all of them, as native's r4 shows);
 * declaring index, i or id first, or id as u8, 833; the cleanup loop as
 * for (index = 0; index < 4; index++) Resource_ResetEntry(handles[index])
 * compiles the same as the counted pointer form and is kept; y set right
 * after Ability_LoadGlyph 833, at the top of the draw body 1014.
 * 833 -> 805: the blink value gFrameCount & 8 is its own variable (native r6,
 * the djinn highlight counter r5); in affordable it took r5, in id 869, in
 * result 805 as well. From 805: the outer element in the mask, count, draw
 * or highlight loops 816/884/868/884; a separate summon for the description
 * 887; a separate id for the first pass 876; the draw loop as a for with
 * break, or visible[index] = 1; index++, both 805 (same code).
 * 805 -> 774: the sprite push loop is indexed (visible[index], &sprites[index]),
 * so index counts up as native (still in r7, native r4); pointer walks let
 * loop.c reverse it to 3..0. From 774: the handle loop indexed
 * (sprites[index].oam.f.tile, with or without a slot local) 861, against
 * native's base + offset giv there; the sprite setup loop indexed and without
 * its y local compiles the same (774) and is kept.
 * Next: index (r7) still outranks id (r4) in greg; native's draw loop keeps
 * summon spilled (slot 36), y in r8 sharing index << 1 with index * 12, and
 * no index * 12 + 8 giv (the draft's sp 8 giv and sprites copy are the two
 * extra frame slots).
 * 774 -> 760: the page changes set page before row (page = 0; row =
 * preferred and page -= 4; row = preferred); the navigation code now
 * differs only in stack slot offsets. From 760: scalar declarations with
 * cursor_handle..element first 762.
 * The extra frame slots are loop.c's: in the draw loop it strength-reduces
 * index * 12 + 8 (giv 942, combined with the two tile-address givs) and
 * hoists a sprites copy; native recomputes both each pass, so loop.c did
 * not optimize native's draw loop at all. Written with a label and goto
 * (no loop notes), the frame is native's 372 and the tile store is native's
 * sequence, but the loop is no longer rotated (889). A loop is skipped when
 * GCSE inserts an insn between NOTE_INSN_LOOP_BEG and its label ("phony",
 * as the two page loops here are from count + 3); in this draft PRE's &glyph
 * insertion lands before the note on a split edge instead.
 * Other trials from 774: the first pass's required, supply and element as
 * block locals give native's test registers there (required r0, element r4,
 * supply r1) but 786; the draw loop's test pointers as block locals as well
 * 803; a block-local summon and required for the description 846; the
 * setup loop as a sprite pointer walk with the handle loop indexed (native's
 * address forms in both loops) 854, frame 384.
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
    s32 drawn_row, drawn_page, row, page, preferred;
    s32 cursor_handle, count, used, mask, element;
    s32 id, tile, affordable, blink;
    struct MenuPoint cursor_position;
    s32 pressed, repeated, result;
    s32 index;
    s32 i;

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
        sprites[index].oam.word.attr01 = MENU_SPRITE_SIZE_16;
        sprites[index].oam.word.attr23 = 0;
        sprites[index].oam.f.x = window->x * 8 + 8;
        sprites[index].oam.f.y = (index * 2 + window->y) * 8 + 4;
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
    for (i = count - 1; i >= 0; i--) {
        id = available[i];
        required = SummonDefinition_Get(id)->djinn_required;
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
        if (element == 4) {
            ordered[used++] = id;
            available[i] = 32;
        }
    }
    for (i = 0; i < count; i++) {
        if (available[i] != 32)
            ordered[used++] = available[i];
    }
    ordered[used] = 32;

    for (;;) {
        if (page != drawn_page || row != drawn_row) {
            render->menu_busy = 1;
            Ui_SetRectHighlight(window->x + 1, window->y + drawn_row * 2 + 1,
                window->width - 2, 1, 15);
            Ui_FillVramBlockPattern();
            summon = SummonDefinition_Get(ordered[page + row]);
            UiText_CopyMessageString(summon->name_message_id + (s32)&MsgAbilityDescription, text, 52);
            UiText_RenderWideStringAtOffset(text, (struct UiWindow *)description, 0, 4);
            mask = 0;
            drawn_row = row;
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
        blink = gFrameCount & 8;
        {
            s32 element;

            for (element = 0; element <= 3; element++) {
                tile = 15 - (blink != 0);
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
            break;
        }
        if (gLinkCountdownWork->active == 0 || (pressed & 2)) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
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
                if (page == ((count - 1) / 4) * 4)
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
                    page = 0;
                    row = preferred;
                }
            } else {
                page += 4;
                row = preferred;
                if (page == ((count - 1) / 4) * 4) {
                    row = count - page - 1;
                    if (row > preferred)
                        row = preferred;
                }
            }
        } else if (repeated & 32) {
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            if (page != 0) {
                page -= 4;
                row = preferred;
            } else {
                page = ((count - 1) / 4) * 4;
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
    WaitFrames(1);
    for (index = 0; index < 4; index++)
        Resource_ResetEntry(handles[index]);
    Resource_ResetEntry((u16)cursor_handle);
    UiWork_Finalize(counts, 1);
    UiWork_Finalize(description, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    return result;
}
