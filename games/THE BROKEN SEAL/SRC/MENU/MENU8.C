#include "CHARACTER_MENU.H"
#include "PSYNERGY_MENU.H"
#include "BATTLE_UNIT.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "TBS_EDITION.H"
#include "SYSTEM.H"
#include "GLOBAL_CELLS.H"
#include "UI.H"

struct InventoryMenuState;
extern u8 MsgRearrangeHelp[], MsgDjinnListHelp[], MsgChooseCharacter;
extern u8 MsgSwitchCharacterHelp, MsgStatusNormal[];
extern u8 MsgStatusAdvice[][2], CharacterMenu_CursorWidths[];
extern volatile u32 gKeyState, gKeysRepeat;
void Palette_LightenBankHighlight(s32 bank);
s32 GameFlag_TestFar(s32 flag);
void PsynergyMenu_CallIconRoutineWithValue(void *menu, s32 owner);
void UiMenu_PositionCursor(s32 x, s32 y);
void UiMenu_SlideCursor(s32 x, s32 y);
void Audio_PlayCue(s32 cue);
void Menu_ReleaseEntryObjects(void);
/* The portrait definition reads four words; these callers keep their extra word. */
void PsynergyMenu_InitializeEntryObjects();
void DjinnMenu_ShowCurrentList(void);
struct BattleUnit *Owner_GetStateFar(s32 owner);
void RenderOutput_RedrawSavedRectFar(s32 window);
void Menu_PlaceEntryObjectsInGrid(s32 x, s32 y, s32 columns);
s32 Party_AddActiveOwnerFar(s32 owner);
s32 Party_ListActiveOwnersFar(u16 *owners);
s32 Party_RemoveActiveOwnerFar(s32 owner);
s32 UiMenu_CreateCursor(void *menu);
struct RenderOutput *SideObject_CreateFar(s32 owner, s32 position, s32 side, s32 window, s32 x, s32 y);
s32 Menu_CreateEightEntryObjects(s32 window);
void Menu_SpawnIconEntries(struct InventoryMenuState *menu, s32 window);
s32 Party_SumDjinnCountsFar(s32 side);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Menu_UpdateEntryObjectTransforms(void);
void ItemMenu_ResetCategory(void);
void RenderOutput_ClearListFar(s32 window);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);

/* Pick a party member, rearranging the order with L and R; returns 1 when
 * one was chosen, -1 when cancelled. */
s32 CharacterSelector_RunRearrange(void)
{
    struct CharacterMenuState *work;
    s32 cursor;
    s32 count;
    s32 page;
    s32 redraw;
    s32 result;
    s32 i;

    work = (struct CharacterMenuState *)gMenuWork;
    cursor = work->owner_index[0];
    count = work->owner_count[0];
    redraw = 1;
    result = 0;
    page = work->page;
    Owner_GetStateFar(work->owner_ids[cursor]);
    for (i = 0; i < 4; i++) {
        work->row_x[i] = 130 + i * 32;
        work->row_y[i] = 0x80;
    }
    Palette_LightenBankHighlight(14);
    Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, (volatile u32 *)0x040000d4);
    while (!GameFlag_TestFar(0x150)) {
        if (redraw) {
            redraw = 0;
            RenderOutput_RedrawSavedRectFar((s32)work->selector_window);
            UiText_DrawCharacterAtOffsetFar((s32)MsgRearrangeHelp, (s32)work->selector_window, 0, 0);
            if (GameFlag_TestFar(48))
                UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnListHelp, (s32)work->selector_window, 0, 16);
            UiText_DrawCharacterAtOffsetFar((s32)MsgRearrangeHelp - 3, (s32)work->selector_window, 0, 8);
            cursor = (cursor + count) % count;
            Owner_GetStateFar(work->owner_ids[cursor]);
            page = (page + 3) % 3;
            Menu_CreateWindowAndEntryObjects(work->owner_ids[cursor], page);
            PsynergyMenu_CallIconRoutineWithValue(work, work->owner_ids[cursor]);
            for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
                work->owner_y[i] = 30;
            work->owner_y[cursor] = 26;
        }
        UiMenu_PositionCursor(cursor * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & 1) {
            Audio_PlayCue(0x70);
            result = 1;
            break;
        }
        if (gKeyState & 2) {
            Audio_PlayCue(0x71);
            result = -1;
            break;
        }
        if (gKeysRepeat & 0x100) {
            if (CharacterSelector_MoveEntry(cursor, 1)) {
                Audio_PlayCue(0x70);
                cursor++;
                Menu_ReleaseEntryObjects();
                PsynergyMenu_InitializeEntryObjects(work->owner_window, 2, 2, 8, 0);
                for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
                    work->owner_y[i] = 30;
                work->owner_y[cursor] = 26;
            } else {
                Audio_PlayCue(0x72);
            }
            WaitFrames(1);
        } else if (gKeysRepeat & 0x200) {
            if (CharacterSelector_MoveEntry(cursor, 0)) {
                Audio_PlayCue(0x70);
                cursor--;
                Menu_ReleaseEntryObjects();
                PsynergyMenu_InitializeEntryObjects(work->owner_window, 2, 2, 8, 0);
                for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
                    work->owner_y[i] = 30;
                work->owner_y[cursor] = 26;
            } else {
                Audio_PlayCue(0x72);
            }
            WaitFrames(1);
        } else if ((gKeyState & 4) && GameFlag_TestFar(48)) {
            DjinnMenu_ShowCurrentList();
            redraw = 1;
        } else {
            if (gKeysRepeat & 0x20) {
                Audio_PlayCue(0x6f);
                if (count > 1) {
                    cursor--;
                    redraw = 1;
                }
            }
            if (gKeysRepeat & 0x10) {
                Audio_PlayCue(0x6f);
                if (count > 1) {
                    redraw = 1;
                    cursor++;
                }
            }
        }
    }
    work->owner_index[0] = cursor;
    work->selected_owner = work->owner_ids[cursor];
    work->selected_ids[0] = work->owner_ids[cursor];
    return result;
}

/* Choose whose Psynergy to list: left and right step through the party,
   redrawing that member's Psynergy page; A returns 1 and B -1, and the
   chosen member is stored as the selected owner. */
s32 PsynergyMenu_SelectOwner(void)
{
    struct PsynergyMenuState *menu = gMenuWork;
    s32 selection = menu->tab_index[0];
    s32 count = (s8)menu->tab_counts[0];
    s32 pending = 1;
    s32 frame = menu->flags;
    s32 result;
    s32 i;

    Owner_GetStateFar(menu->owner_table[selection]);
    RenderOutput_RedrawSavedRectFar(menu->message_window);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgChooseCharacter, menu->message_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgChooseCharacter + 1, menu->message_window, 0, 16);
    for (;;) {
        if (pending) {
            pending = 0;
            selection = (selection + count) % count;
            Owner_GetStateFar(menu->owner_table[selection]);
            frame = (frame + 3) % 3;
            Menu_CreateWindowAndEntryObjects(menu->owner_table[selection], frame);
            PsynergyMenu_CallIconRoutineWithValue(menu, menu->owner_table[selection]);
            for (i = 0; i < MENU_ROW_COUNT; i++)
                menu->row_positions[i] = 30;
            menu->row_positions[selection] = 26;
            menu->psynergy_count = PsynergyMenu_CollectActions(Owner_GetStateFar(menu->owner_table[selection]), (u16 *)menu->psynergies, 0);
            PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
            Menu_PlaceEntryObjectsInGrid(96, 96, 8);
            Menu_HideEmptyEntryIcons(menu->psynergies);
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & 1) {
            Audio_PlayCue(112);
            result = 1;
            break;
        }
        if (gKeyState & 2) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }
        {
            volatile u32 *repeat = &gKeysRepeat;

            if (*repeat & 32) {
                Audio_PlayCue(111);
                if (count > 1) {
                    selection--;
                    pending = 1;
                }
            }
            if (*repeat & 16) {
                Audio_PlayCue(111);
                if (count > 1) {
                    selection++;
                    pending = 1;
                }
            }
        }
    }
    menu->tab_index[0] = selection;
    menu->selected_owner = menu->owner_table[selection];
    menu->owner_ids[0] = menu->owner_table[selection];
    return result;
}

s32 CharacterSelector_MoveEntry(s32 selected_index, s32 direction)
{
    struct CharacterMenuState *state = (struct CharacterMenuState *)gMenuWork;
    u32 reordered[14];
    s32 index;

    if (state->party_count <= 1) {
        return 0;
    }
    if (direction == 1) {
        if (selected_index == state->party_count - 1) {
            return 0;
        }
    } else {
        if (selected_index == 0) {
            return 0;
        }
    }

    {
        u32 zero = 0;
        u32 *clear = &reordered[13];

        do {
            *clear = zero;
            clear--;
        } while ((s32)clear >= (s32)reordered);
    }
    for (index = 0; index < state->party_count; index++) {
        reordered[index] = state->owner_ids[index];
    }

    if (direction == 1) {
        index = reordered[selected_index];
        reordered[selected_index] = reordered[selected_index + 1];
        reordered[selected_index + 1] = index;
    } else {
        index = reordered[selected_index];
        reordered[selected_index] = reordered[selected_index - 1];
        reordered[selected_index - 1] = index;
    }

    for (index = 0; index < state->party_count; index++) {
        Party_RemoveActiveOwnerFar(state->owner_ids[index]);
    }
    for (index = 0; index < state->party_count; index++) {
        Party_AddActiveOwnerFar(reordered[index]);
    }
    state->party_count = Party_ListActiveOwnersFar(state->owner_ids);
    return 1;
}

void Menu_InitSelectorCursorAndEntries(void)
{
    struct CharacterMenuState *state;

    state = (struct CharacterMenuState *)gMenuWork;
    PsynergyMenu_InitializeEntryObjects(UiMenu_CreateCursor(state), 2, 2, 8, 0);
    state->unknown_window_028 = 0;
    state->status_window = 0;
    state->help_window = 0;
    state->action_window = 0;
    state->selected_column = 0;
    state->selected_row = 0;
    state->column_count = 8;
    state->row_count = 2;
}

void Menu_CreateWindowAndEntryObjects(s32 resource, s32 frame)
{
    s32 created;
    s32 handle;
    struct RenderOutput *object;
    struct CharacterMenuState *state;

    state = (struct CharacterMenuState *)gMenuWork;
    created = 0;
    handle = (s32)state->status_window;
    if (handle == 0) {
        created = UiWindow_UpdateOrCreate((s32 *)&state->status_window, 0, 5, 0x1E, 0xF, 2);
        handle = (s32)state->status_window;
    }
    if (created != 0) {
        object = SideObject_CreateFar(resource, 0, 0, handle, 0, 0);
        state->status_cursor = object;
        if ((object->sentinel = 0xF0, state->page) == 3) {
            Menu_SpawnIconEntries((struct InventoryMenuState *)state, handle);
        }
        Menu_CreateEightEntryObjects(handle);
        CharacterMenu_DrawStatusAilments((struct UiWindow *)handle, resource, 0x100);
        return;
    }
    CharacterMenu_DrawStatusAilments((struct UiWindow *)handle, resource, 0);
}

/*
 * The character menu's command selection: shows the selected character's
 * status and available commands across two panes, switches characters on
 * request, and returns 1 when a command is confirmed or -1 when cancelled.
 */
s32 CharacterMenu_SelectCommand(void)
{
    struct CharacterMenuState *menu;
    s32 pane;
    s32 result;
    s32 selected;
    s8 count;
    s32 has_ailments;
    s32 done;
    s32 has_djinn;
    s32 redraw;
    s32 tab;
    s32 total;
    s32 i;
    u8 entries[8];

    menu = (struct CharacterMenuState *)gMenuWork;
    pane = 0;
    result = 0;
    selected = 0;
    total = Party_SumDjinnCountsFar(-1);
    /* FAKEMATCH: retain the family's neg/orr/lsr boolean conversion. */
    has_djinn = (u32)(-total | total) >> 31;
    UiWindow_UpdateOrCreate((s32 *)&menu->help_window, 0, 0, 30, 5, 2);
    Scheduler_RemoveCallback((u32)(Menu_UpdateEntryObjectTransforms));
    for (i = 3; i >= 0; i--)
        menu->row_y[i] = 104;
    done = 0;
    UiMenu_SlideCursor(-10, 88);

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        Owner_GetStateFar(menu->selected_ids[0]);
        CharacterMenu_DrawStatusAilments(menu->status_window, menu->selected_ids[0], 1);
        count = CharacterMenu_BuildAvailability(entries, 1, menu->selected_ids[0]);
        has_ailments = 0;
        if (count == 0)
            count = 1;
        else
            has_ailments = 1;
        redraw = 1;

        while (GameFlag_TestFar(0x150) == 0) {
            if (redraw != 0) {
                s32 limit;

                redraw = 0;
                limit = count;
                pane = (pane + 2) % 2;
                if (pane == 0) {
                    selected = (selected + limit) % limit;
                    RenderOutput_RedrawSavedRectFar((s32)menu->help_window);
                    if (has_ailments == 0) {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp,
                            (s32)menu->status_window, SWITCH_HELP_X, -24);
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp + 1,
                            (s32)menu->status_window, 0, -24);
                    }
                } else {
                    RenderOutput_RedrawSavedRectFar((s32)menu->help_window);
                    if (has_djinn != 0)
                        selected = (selected + 8) % 8;
                    else
                        selected = (selected + 7) % 7;
                }
                CharacterMenu_DrawSelectionCursor(pane, selected, entries, 0);
                RenderOutput_ClearListFar((s32)menu->help_window);
                WaitFrames(1);
                if (pane == 0)
                    CharacterMenu_DrawSelectionLabels((s32)menu->help_window, selected, entries);
                else
                    StatusMenu_ShowOwnerProgressMessage(menu->help_window, selected, has_djinn);
            }
            menu->owner_cursors[0]->active = 1;
            if (pane == 0)
                UiMenu_PositionCursor(-10, selected * 16 + 88);
            else if (selected <= 3)
                UiMenu_PositionCursor(24, selected * 8 + 48);
            else
                UiMenu_PositionCursor(48, selected * 8 + 80);
            WaitFrames(1);
            if (gKeysRepeat & 0xf0)
                CharacterMenu_DrawSelectionCursor(pane, selected, entries, 1);
            if (gKeyState & 1) {
                Audio_PlayCue(112);
                done = 1;
                result = 1;
                break;
            }
            if (gKeyState & 2) {
                Audio_PlayCue(113);
                done = 1;
                result = -1;
                break;
            }
            if (gKeysRepeat & 0x40) {
                Audio_PlayCue(111);
                redraw = 1;
                selected--;
            }
            if (gKeysRepeat & 0x80) {
                Audio_PlayCue(111);
                redraw = 1;
                selected++;
            }
            if (gKeysRepeat & 0x10) {
                Audio_PlayCue(111);
                redraw = 1;
                pane++;
            }
            if (gKeysRepeat & 0x20) {
                Audio_PlayCue(111);
                redraw = 1;
                pane--;
            }
            if ((gKeysRepeat & 0x100) || (gKeysRepeat & 0x200)) {
                Audio_PlayCue(111);
                tab = menu->owner_index[0];
                if (gKeysRepeat & 0x100)
                    tab++;
                else
                    tab--;
                tab = (tab + menu->party_count) % menu->party_count;
                menu->selected_owner = menu->owner_ids[tab];
                menu->selected_ids[0] = menu->owner_ids[tab];
                menu->owner_index[0] = tab;
                PsynergyMenu_CallIconRoutineWithValue(menu, menu->owner_ids[tab]);
                break;
            }
        }
    }
    RenderOutput_ClearListFar((s32)menu->help_window);
    RenderOutput_RedrawSavedRectFar((s32)menu->help_window);
    UiWindow_ClearInteriorTilesFar((s32)menu->status_window, 64, 56, 224, 96);
    Scheduler_AddOrUpdateCallback((s32)(Menu_UpdateEntryObjectTransforms), 0xc80);
    for (i = 3; i >= 0; i--)
        menu->row_y[i] = 128;
    ItemMenu_ResetCategory();
    return result;
}

void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected,
    u8 *entries, s32 invert)
{
    struct CharacterMenuState *state = *(struct CharacterMenuState *volatile *)&gMenuWork;
    u32 different;
    s32 count;
    s32 index;
    s32 x;
    s32 y;
    s32 last;
    u8 width;

    if (mode == 0) {
        y = selected * 2 + 5;
        x = 0;
        width = 5;
        count = 0;
        index = 0;
        while (index <= 4) {
            if (entries[index] != 0) {
                if (selected == count) {
                    width = CharacterMenu_CursorWidths[index];
                    break;
                }
                count++;
            }
            index++;
        }
    } else if (selected <= 3) {
        y = selected;
        x = 5;
        width = 13;
    } else {
        y = selected + 4;
        x = 8;
        width = 20;
    }

    different = 1 ^ (u32)invert;
    last = 15 - (((0u - different) | different) >> 31);
    Render_SetTilemapFlagRect(state->status_window, x, y, width, 1, last);
}

void CharacterMenu_DrawSelectionLabels(s32 target, s32 selected,
    const u8 *entries)
{
    s32 count = 0;
    s32 index = 0;

    do {
        if (entries[index] != 0) {
            if (selected == count) {
                s32 message = (s32)MsgStatusAdvice[index];
                UiText_DrawMessageAt(message, target, 0, -1);
                UiText_DrawMessageAt(message + 1, target, 0, 15);
            }
            count++;
        }
        index++;
    } while (index <= 4);

    if (count == 0)
        UiText_DrawMessageAt((s32)MsgStatusNormal, target, 0, 0);
}
