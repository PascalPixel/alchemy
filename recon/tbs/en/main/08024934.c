/* Not-yet-C: complete 2124-byte summon picker, including literal pools.
 * Current EN score15925: 219 register,75 stack,50 operand,84 reordered,
 * 41 inserted,44 deleted instruction differences; no unresolved symbols.
 * Ordinary routed compilation emits2124 bytes including pools, frame376
 * versus native372. Equal extent does not mean byte identity.
 * 2026-10-02 owner recovery: MENU_LIST.H owns windows, render work, menu
 * cell, sprites and text-call prototypes. gWindowWork[0]/[42] are their
 * existing pointer cells; no invented global bundle or duplicate record.
 * MsgAbilityName/Description come from the current editable catalogs, and
 * tile-map entries are enum values rather than address-spelling symbols.
 * H0 shared owners: score17411, frame368 versus native372. Retained as
 * the source owner correction, despite the scheduling changes.
 * H1 used cursor base across rendering calls: score17294, retained.
 * H2 existing MenuPoint for both used cursor coordinates: score15925,
 * frame376. Retained as the closer ordinary source shape; it is not exact.
 * Earlier scalar baseline score16810 had eight unresolved Value_* symbols.
 * No unused storage, fixed register, inline assembly, option or output patch
 * was added. This function remains a draft and earns no edition credit.
 * Earlier near-miss measurements before this owner correction:
 * Standby counts arrive in argument three; returns a summon id or -1.
 * Candidate 2112 bytes, 655 aligned halfword edits. Per-phase counters
 * recover drawn_page in r9. Holding last=count-1 across sorting/navigation
 * recovers its spill and signed reverse walk, bringing the frame to 368
 * bytes (reference 372). Remaining: cursor-coordinate spills, first sprite
 * loop/pools, the inner affordability test and indexed icon traversal.
 * Explicit full-width OAM masks did not recover the first-loop roles.
 * A two-word cursor-position aggregate forces both coordinate spills, but
 * gives a 376-byte frame and shifts every fixed local by four bytes
 * (2124-byte candidate, 641 aligned edits); rejected as the stack model.
 * The old generated draft omitted fifth call arguments and widened byte
 * sprite writes; the typed record now follows those observed accesses. */
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
    struct MenuSprite *cursor_sprite;
    const u8 *required, *supply;
    u8 *src, *dst;
    s32 *handle;
    s32 drawn_row, drawn_page, row, page, preferred;
    s32 cursor_handle, count, last, used, mask, element;
    s32 id, tile, affordable;
    struct MenuPoint cursor_position;
    s32 pressed, repeated, result;

    render = (struct UiRenderWork *)gWindowWork[0];
    drawn_row = -1;
    drawn_page = -1;
    cursor_sprite = &cursor;
    cursor_handle = (s16)Resource_LoadIntoFreeSlot(128);
    description = UiWindow_Create(0, 4, 30, 4, 42);
    counts = UiWindow_Create(20, 8, 10, 3, 6);
    mask = 0;
    page = ((struct MenuCell *)gWindowWork[42])->page;
    row = ((struct MenuCell *)gWindowWork[42])->row;
    preferred = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    window = UiWindow_Create(13, 11, 17, 9, 6);
    {
        s32 index;

        sprite = sprites;
        index = 0;
        do {
            sprite->oam.word.attr01 = 0x40000000;
            sprite->oam.word.attr23 = 0;
            sprite->oam.f.x = window->x * 8 + 8;
            sprite->oam.f.y = (index * 2 + window->y) * 8 + 4;
            index++;
            sprite++;
        } while (index <= 3);
    }
    {
        s32 index;

        handle = handles;
        sprite = sprites;
        index = 3;
        do {
            s32 slot = Resource_LoadIntoFreeSlot(128);
            *handle++ = slot;
            sprite->oam.f.tile = Resource_GetBuffer(slot, -1);
            sprite++;
        } while (--index >= 0);

    }
    count = Trade_ListFlaggedEntriesFar(available);
    used = 0;
    last = count - 1;
    if (last >= 0) {
        src = available + last;
        do {
            id = *src;
            required = SummonDefinition_Get(id)->djinn_required;
            supply = standby;
            element = 0;
            while (*required <= *supply) {
                element++;
                if (element > 3)
                    break;
                required++;
                supply++;
            }
            if (element == 4) {
                ordered[used++] = id;
                *src = 32;
            }
            src--;
        } while ((s32)src >= (s32)available);
    }
    {
        s32 index;

        dst = ordered + used;
        src = available;
        for (index = count; index > 0; index--) {
            id = *src++;
            if (id != 32) {
                *dst++ = id;
                used++;
            }
        }
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
                    s32 index, element, col;

                    index = 0;
                    while (index <= 3 && (id = ordered[page + index]) != 32) {
                        summon = SummonDefinition_Get(id);
                        required = summon->djinn_required;
                        supply = standby;
                        element = 0;
                        while (*required <= *supply) {
                            element++;
                            if (element > 3)
                                break;
                            required++;
                            supply++;
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
                        for (element = 0; element <= 3; element++) {
                            if (*required != 0) {
                                UiWindow_SetTilemapEntry(window, element + SUMMON_ELEMENT_TILE, col, index * 2, 0);
                                UiWindow_PutGlyph(window, *required + 48, col + 1, index * 2, 0);
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
                {
                    s32 index;

                    for (index = 0; index < (count + 3) / 4; index++) {
                        tile = index + SUMMON_PAGE_TILE;
                        if (index == page / 4)
                            tile = index + SUMMON_SELECTED_PAGE_TILE;
                        UiWindow_SetTilemapEntry(window, tile,
                            window->width - (count + 3) / 4 + index - 2, -1, 0);
                    }
                }
            }
            Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
            render->dirty = 1;
            render->menu_busy = 0;
        }
        {
            s32 index;

            sprite = sprites;
            src = visible;
            for (index = 0; index <= 3; index++) {
                if (*src++ != 0)
                    Runtime_PushSlotEntry((s32 *)sprite, 240);
                sprite++;
            }
        }
        cursor_position.x = window->x * 8 - 2;
        cursor_position.y = (row * 2 + window->y) * 8 + 20;
        cursor_sprite->oam.word.attr01 = 0x40000000;
        cursor_sprite->oam.word.attr23 = 0;
        cursor_sprite->oam.f.tile = Resource_GetBuffer((u16)cursor_handle, (s32)Resource_FixedBlockBTiles);
        cursor_sprite->oam.f.x = cursor_position.x + ((gFrameCount & 4) >> 1) - 4;
        cursor_sprite->oam.f.y = cursor_position.y - ((gFrameCount & 4) >> 2) + 248;
        Runtime_PushSlotEntry((s32 *)cursor_sprite, 242);
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
            {
                s32 index;

                for (index = 0; index < (count + 3) / 4; index++) {
                    tile = index + SUMMON_PAGE_TILE;
                    if ((gFrameCount & 15) <= 11 && index == page / 4)
                        tile = index + SUMMON_SELECTED_PAGE_TILE;
                    UiWindow_SetTilemapEntry(window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
                }
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
    WaitFrames(1);
    {
        s32 index;

        handle = handles;
        index = 3;
        do {
            Resource_ResetEntry(*handle++);
        } while (--index >= 0);
    }
    Resource_ResetEntry((u16)cursor_handle);
    UiWork_Finalize(counts, 1);
    UiWork_Finalize(description, 1);
    UiWork_Finalize(window, 1);
    WaitFrames(1);
    return result;
}
