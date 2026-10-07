#include "BTLSYS.H"
#include "ITEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "EDITION.H"

#if !GAME_TLA
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "BTLSMN.H"
#include "MENU_LIST.H"

#if EDITION_INTERNATIONAL
#define SUMMON_COST_X 13
#else
#define SUMMON_COST_X 10
#endif

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
    s32 count;
    struct UiWindow *window;
    struct UiRenderWork *render;
    s32 drawn_row, drawn_page, row, page;
    s32 cursor_handle;
    struct UiWindow *description, *counts;
    s32 mask, preferred;
    struct MenuPoint cursor_position;
    const struct SummonDefinition *summon;
    struct MenuCell *nav;
    struct MenuSprite *sprite, *cur;
    s32 used;
    s32 id, tile, affordable;
    s32 pressed, repeated, result;
    s32 index;

    /* FAKEMATCH: register choice (spill slots) only: a dead store. GCSE
       sizes its hash table from the function's length and PRE numbers its
       reaching registers in bucket order; with this store the table has
       379 buckets and &ordered and &sprites take their spill slots before
       count - 1, as in the native frame (377 buckets without it, differ
       80). */
    result = 0;
    render = (struct UiRenderWork *)gWindowWork[0];
    drawn_row = -1;
    drawn_page = -1;
    cursor_handle = (s16)Resource_LoadIntoFreeSlot(128);
    description = UiWindow_Create(0, 4, 30, 4, 42);
    counts = UiWindow_Create(20, 8, 10, 3, 6);
    mask = 0;
    /* FAKEMATCH: instruction order only: the one-pass block's loop notes
       are scheduling barriers, so counts is stored before mask and the
       window call's stack argument before its register arguments (differ
       10 without it, the table held at 379 buckets). */
    do {
        page =((struct MenuCell *)gWindowWork[42])->page;
        row = ((struct MenuCell *)gWindowWork[42])->row;
        preferred = ((struct MenuCell *)gWindowWork[42])->preferred_row;
    } while (0);
#if EDITION_INTERNATIONAL
    window = UiWindow_Create(13, 11, 17, 9, 6);
#else
    window = UiWindow_Create(16, 11, 14, 9, 6);
#endif
    for (index = 0; index < 4; index++) {
        s32 y = index * 2;

        sprite = &sprites[index];
        /* FAKEMATCH: instruction order only: the one-pass block keeps
           y = index * 2 first, as in the twins' setup loops (differ 4). */
        do {
            sprites[index].oam.word.attr01 = MENU_SPRITE_SIZE_16;
            sprites[index].oam.word.attr23 = 0;
        } while (0);
        sprite->oam.f.x = window->x * 8 + 8;
        sprite->oam.f.y = (y + window->y) * 8 + 4;
    }
    for (index = 0; index < 4; index++) {
        s32 slot = Resource_LoadIntoFreeSlot(128);
        handles[index] = slot;
        sprites[index].oam.f.tile = Resource_GetBuffer(slot, -1);
    }
    count = Trade_ListFlaggedEntriesFar(available);
    do {
        s32 i;

        /* FAKEMATCH: instruction order only: in the one-pass block count
           and used are stored before count - 1 is built (differ 4). */
        used = 0;
        for (i = count - 1; i >= 0; i--) {
            const struct SummonDefinition *s;
            s32 n, k;

            k = available[i];
            s = SummonDefinition_Get(k);
            for (n = 0; n <= 3; n++) {
                if (s->djinn_required[n] > standby[n])
                    break;
            }
            if (n == 4) {
                ordered[used++] = k;
                available[i] = 32;
            }
        }
    } while (0);
    {
        s32 i, k;

        for (i = 0; i < count; i++) {
            k = available[i];
            if (k != 32)
                ordered[used++] = k;
        }
    }
    ordered[used] = 32;

    /* FAKEMATCH: instruction order only (block placement): with the exit
       inside an enclosing loop, loop.c finds no barrier at the exit's
       depth and keeps the select block inline, as the native does; the
       block ends after the cleanup, so its loop sets the count before the
       pointer (differ 2 when it ends before the cleanup). */
    do {
    for (;;)
    {
        cur = &cursor;
        if (page != drawn_page || row != drawn_row) {
            render->menu_busy = 1;
            Ui_SetRectHighlight(window->x + 1, window->y + drawn_row * 2 + 1,
                window->width - 2, 1, 15);
            Ui_FillVramBlockPattern();
            {
                const struct SummonDefinition *s;
                s32 element;

                s = SummonDefinition_Get(ordered[page + row]);
#if EDITION_INTERNATIONAL
                UiText_CopyMessageString(s->name_message_id + (s32)&MsgAbilityDescription, text, 52);
#else
                UiText_DecodeMessage(s->name_message_id + (s32)&MsgAbilityDescription, text, 32);
#endif
                UiText_RenderWideStringAtOffset(text, (struct UiWindow *)description, 0, 4);
                drawn_row = row;
                mask = 0;
                for (element = 0; element <= 3; element++) {
                    if (s->djinn_required[element] != 0)
                        mask |= 1 << element;
                }
            }
            if (page != drawn_page) {
                RenderOutput_RedrawSavedRect(window);
                {
                    s32 element;

                    for (element = 0; element <= 3; element++) {
                        UiWindow_SetTilemapEntry(counts, element + SUMMON_ELEMENT_TILE, element * 2, 0, 0);
                        UiWindow_PutGlyph(counts, standby[element] + 48, element * 2 + 1, 0, 0);
                    }
                }
                {
                    s32 element, col, y;

                    index = 0;
                    if ((id = ordered[page + index]) != 32) {
                    next:
                        summon = SummonDefinition_Get(id);
                        col = 0;
                        for (element = 0; element <= 3; element++) {
                            if (summon->djinn_required[element] > standby[element])
                                break;
                        }
                        affordable = element == 4;
                        Ability_LoadGlyph(summon->name_message_id & 0x3fff, 0, &handles[index], &glyph, 1);
                        y = index * 2;
                        sprites[index].oam.f.tile = glyph;
                        if (!affordable)
                            UiWork_SetParamNibble(2);
                        UiText_DrawCharacterAtOffset(SummonDefinition_Get(id)->name_message_id + (s32)&MsgAbilityName,
                            (struct RenderInput *)window, 16, index * 16);
                        for (element = 0; element <= 3; element++) {
                            if (summon->djinn_required[element] != 0) {
                                UiWindow_SetTilemapEntry(window, element + SUMMON_ELEMENT_TILE, col * 2 + SUMMON_COST_X, y, 0);
                                UiWindow_PutGlyph(window, summon->djinn_required[element] + 48, col * 2 + SUMMON_COST_X + 1, y, 0);
                                col++;
                            }
                        }
                        UiWork_SetParamNibble(15);
                        visible[index++] = 1;
                        if (index <= 3 && (id = ordered[page + index]) != 32)
                            goto next;
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
        /* FAKEMATCH: instruction order only: as u32 words, not through the
           union, these stores do not alias cursor_position, so cur is
           reloaded before cursor_position.y is stored (differ 2). */
        ((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
        ((u32 *)cur)[2] = 0;
        cur->oam.f.tile = Resource_GetBuffer((u16)cursor_handle, (s32)Resource_FixedBlockBTiles);
        cur->oam.f.x = cursor_position.x + ((gFrameCount & 4) >> 1) - 4;
        cur->oam.f.y = cursor_position.y - ((gFrameCount & 4) >> 2) + 248;
        sprite = &cursor;
        Runtime_PushSlotEntry((s32 *)sprite, 242);
        {
            s32 element, tile, blink;

            blink = gFrameCount & 8;
            for (element = 0; element <= 3; element++) {
                tile = 15 - (blink != 0);
                if ((1 << element & mask) == 0)
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
            /* FAKEMATCH: instruction order only: the one-pass block's
               barrier orders both arrow calls' arguments and the dirty
               flag's loads as in the native (differ 10). */
            do {
                render->dirty |= 2 << ((u32)(window->y - 1) >> 2);
            } while (0);
        }
        nav = gLinkCountdownWork;
        /* FAKEMATCH: instruction order only: page, row and preferred are
           stored in source order before the keys are read (differ 4). */
        do {
            nav->page = page;
            nav->row = row;
            nav->preferred_row = preferred;
        } while (0);
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
    } while (0);
    return result;
}
#endif

s32 Item_ClassifyUseAbility(s32 owner, s32 item)
{
    s32 ret;
    struct ItemDefinition *definition;

    if (item == 0) {
        return 1;
    }
    definition = Item_Get(item);
    ret = 1;
    if (definition->use_type == 3) {
        return ret;
    }
    if (definition->action_id == 0) {
        return ret;
    }
    if ((definition->type != 0) && (Item_CanOwnerEquip(owner, item) == 0)) {
        return ret;
    }
    if ((0x80 & BtlActGet(definition->action_id)->tgt_flg) == 0) {
        return 2;
    }
    return 0;
}
