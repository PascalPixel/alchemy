#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "TBS_EDITION.H"
#include "SYSTEM.H"
#include "GLOBAL_CELLS.H"
#include "A8_STATE.H"
#include "UI.H"

extern u8 gMenuWork[];

/* menu/character_selector_run_rearrange.c */
struct CharacterSelectWork {
    u8 padding0[8];
    u32 owner;
    u8 padding0c[4];
    s32 object;
    u8 padding14[8];
    s8 cursor;
    u8 padding1d;
    s8 count;
    u8 padding1f[0x10c - 0x1f];
    void *window;
    u8 padding110[0x144 - 0x110];
    u16 frames[8];
    u8 padding154[0x208 - 0x154];
    u16 owners[9];
    u8 choice;
    u8 padding21b[0x220 - 0x21b];
    u16 page;
    u8 padding222[0x234 - 0x222];
    u16 slot_x[4];
    u16 slot_y[4];
};

/* The help lines: Rearrange, the Djinn list and Details. The code loads the
 * Rearrange line from the literal pool and reaches Details, three messages
 * before it, by subtracting. */
extern u8 MsgRearrangeHelp[], MsgDjinnListHelp[];
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
void Palette_LightenBankHighlight(s32 bank);
s32 GameFlag_TestFar(s32 flag);
void Menu_CreateWindowAndEntryObjects(s32 owner, s32 frame);
void PsynergyMenu_CallIconRoutineWithValue(void *work, s32 value);
void UiMenu_PositionCursor(s32 x, s32 y);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
s32 CharacterSelector_MoveEntry(s32 cursor, s32 forward);
void Menu_ReleaseEntryObjects(void);
/* Preserve this caller's extra word at the four-word portrait entry. */
void PsynergyMenu_InitializeEntryObjects();
void DjinnMenu_ShowCurrentList(void);

struct PsynergyOwnerMenu {
    u8 padding000[8];
    s32 selected_owner;
    u8 padding00c[0x10];
    s8 selection;
    u8 padding01d;
    s8 count;
    u8 padding01f[0xed];
    s32 selector_window;
    u8 padding110[0x34];
    u16 row_positions[8];
    u8 padding154[0x74];
    u8 entries[0x40];
    u16 character_ids[8];
    u8 entry_count;
    u8 padding219;
    u8 item_owner;
    u8 padding21b[5];
    u16 frame;
};

extern u8 MsgChooseCharacter;
void *Owner_GetStateFar(s32 owner);
void RenderOutput_RedrawSavedRectFar(s32 window);
struct BattleUnit;
s32 PsynergyMenu_CollectActions(struct BattleUnit *owner, u16 *entries, s32 mode);
void PsynergyMenu_DrawPsynergyIcons(void *entries);
void Menu_PlaceEntryObjectsInGrid(s32 x, s32 y, s32 columns);
void Menu_HideEmptyEntryIcons(void *entries);

struct CharacterSelectorState {
    u8 padding000[0x24];
    s32 screen_handle;
    u8 padding028[0x0e4];
    s32 selector_window;
    u8 padding110[0x34];
    u16 row_positions[8];
    u8 padding154[0x0b4];
    u16 character_ids[8];
    u8 padding218;
    u8 character_count;
    u8 padding21a[6];
    u16 flags;
};

s32 Party_AddActiveOwnerFar(s32 character_id);
s32 Party_ListActiveOwnersFar(const u16 *character_ids);
s32 Party_RemoveActiveOwnerFar(s32 character_id);
extern u8 Data_03001f2c[];

struct State080a8034 {
    u8 padding_00[0x20];
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2c;
    u8 padding_30[0xe0];
    s8 field_110;
    s8 field_111;
    s8 field_112;
    s8 field_113;
};

s32 UiMenu_CreateCursor(void *);
#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

typedef struct {
    u8 padding[15];
    u8 field_0f;
} Object0f;

void *SideObject_CreateFar(s32, s32, s32, s32, s32, s32);
void CharacterMenu_DrawStatusAilments(s32, s32, s32);
s32 Menu_CreateEightEntryObjects(s32 resource);

struct CharacterMenuCursor {
    u8 unknown_00[5];
    u8 state;
};

struct CharacterCommandMenu {
    u8 unknown_000[8];
    s32 owner;
    u8 unknown_00c[8];
    struct CharacterMenuCursor *cursor;
    u8 unknown_018[4];
    s8 tab;
    u8 unknown_01d[7];
    s32 window;
    u8 unknown_028[4];
    s32 help_window;
    u8 unknown_030[0x1d8];
    u16 owners[8];
    u8 unknown_218;
    u8 owner_count;
    u8 owner_id;
    u8 unknown_21b[0x21];
    s16 slot_y[4];
};

extern u8 MsgSwitchCharacterHelp;
s32 Party_SumDjinnCountsFar(s32 side);
s32 UiWindow_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
void Menu_UpdateEntryObjectTransforms(void);
void UiMenu_SlideCursor(s32 x, s32 y);
void CharacterMenu_DrawStatusAilments(s32 window, s32 owner, s32 mode);
s32 CharacterMenu_BuildAvailability(u8 *entries, s32 flags, s32 owner);
void RenderOutput_ClearListFar(s32 window);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void PsynergyMenu_CallIconRoutineWithValue(void *menu, s32 owner);
void ItemMenu_ResetCategory(void);
void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries, s32 invert);
void CharacterMenu_DrawSelectionLabels(s32 window, s32 selected, const u8 *entries);
void StatusMenu_ShowOwnerProgressMessage(s32 window, s32 selected, s32 has_djinn);

struct State_080a847c {
    u8 padding[36];
    u8 *object;
};

extern u8 CharacterMenu_CursorWidths[];
void Render_SetTilemapFlagRect(const u8 *, s32, s32, s32, s32, u32);
void UiText_DrawMessageAt(s32, s32, s32, s32);
extern u8 MsgStatusAdvice[][2];
extern u8 MsgStatusNormal[];

/* Pick a party member, rearranging the order with L and R; returns 1 when
 * one was chosen, -1 when cancelled. */
s32 CharacterSelector_RunRearrange(void)
{
    struct CharacterSelectWork *work;
    s32 cursor;
    s32 count;
    s32 page;
    s32 redraw;
    s32 result;
    s32 i;

    work = *(struct CharacterSelectWork **)gMenuWork;
    cursor = work->cursor;
    count = work->count;
    redraw = 1;
    result = 0;
    page = work->page;
    Owner_GetStateFar(work->owners[cursor]);
    for (i = 0; i < 4; i++) {
        work->slot_x[i] = 130 + i * 32;
        work->slot_y[i] = 0x80;
    }
    Palette_LightenBankHighlight(14);
    Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, (volatile u32 *)0x040000d4);
    while (!GameFlag_TestFar(0x150)) {
        if (redraw) {
            redraw = 0;
            RenderOutput_RedrawSavedRectFar(work->window);
            UiText_DrawCharacterAtOffsetFar((s32)MsgRearrangeHelp, work->window, 0, 0);
            if (GameFlag_TestFar(48))
                UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnListHelp, work->window, 0, 16);
            UiText_DrawCharacterAtOffsetFar((s32)MsgRearrangeHelp - 3, work->window, 0, 8);
            cursor = (cursor + count) % count;
            Owner_GetStateFar(work->owners[cursor]);
            page = (page + 3) % 3;
            Menu_CreateWindowAndEntryObjects(work->owners[cursor], page);
            PsynergyMenu_CallIconRoutineWithValue(work, work->owners[cursor]);
            for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
                work->frames[i] = 30;
            work->frames[cursor] = 26;
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
                PsynergyMenu_InitializeEntryObjects(work->object, 2, 2, 8, 0);
                for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
                    work->frames[i] = 30;
                work->frames[cursor] = 26;
            } else {
                Audio_PlayCue(0x72);
            }
            WaitFrames(1);
        } else if (gKeysRepeat & 0x200) {
            if (CharacterSelector_MoveEntry(cursor, 0)) {
                Audio_PlayCue(0x70);
                cursor--;
                Menu_ReleaseEntryObjects();
                PsynergyMenu_InitializeEntryObjects(work->object, 2, 2, 8, 0);
                for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
                    work->frames[i] = 30;
                work->frames[cursor] = 26;
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
    work->cursor = cursor;
    work->owner = work->owners[cursor];
    work->choice = work->owners[cursor];
    return result;
}

/* Choose whose Psynergy to list: left and right step through the party,
   redrawing that member's Psynergy page; A returns 1 and B -1, and the
   chosen member is stored as the selected owner. */
s32 PsynergyMenu_SelectOwner(void)
{
    struct PsynergyOwnerMenu *menu = *(struct PsynergyOwnerMenu **)gMenuWork;
    s32 selection = menu->selection;
    s32 count = menu->count;
    s32 pending = 1;
    s32 frame = menu->frame;
    s32 result;
    s32 i;

    Owner_GetStateFar(menu->character_ids[selection]);
    RenderOutput_RedrawSavedRectFar(menu->selector_window);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgChooseCharacter, menu->selector_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgChooseCharacter + 1, menu->selector_window, 0, 16);
    for (;;) {
        if (pending) {
            pending = 0;
            selection = (selection + count) % count;
            Owner_GetStateFar(menu->character_ids[selection]);
            frame = (frame + 3) % 3;
            Menu_CreateWindowAndEntryObjects(menu->character_ids[selection], frame);
            PsynergyMenu_CallIconRoutineWithValue(menu, menu->character_ids[selection]);
            for (i = 0; i < MENU_ROW_COUNT; i++)
                menu->row_positions[i] = 30;
            menu->row_positions[selection] = 26;
            menu->entry_count = PsynergyMenu_CollectActions(Owner_GetStateFar(menu->character_ids[selection]), (u16 *)menu->entries, 0);
            PsynergyMenu_DrawPsynergyIcons(menu->entries);
            Menu_PlaceEntryObjectsInGrid(96, 96, 8);
            Menu_HideEmptyEntryIcons(menu->entries);
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
    menu->selection = selection;
    menu->selected_owner = menu->character_ids[selection];
    menu->item_owner = menu->character_ids[selection];
    return result;
}

s32 CharacterSelector_MoveEntry(s32 selected_index, s32 direction)
{
    struct CharacterSelectorState *state =
        *(struct CharacterSelectorState **)gMenuWork;
    u32 reordered[14];
    s32 index;

    if (state->character_count <= 1) {
        return 0;
    }
    if (direction == 1) {
        if (selected_index == state->character_count - 1) {
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
    for (index = 0; index < state->character_count; index++) {
        reordered[index] = state->character_ids[index];
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

    for (index = 0; index < state->character_count; index++) {
        Party_RemoveActiveOwnerFar(state->character_ids[index]);
    }
    for (index = 0; index < state->character_count; index++) {
        Party_AddActiveOwnerFar(reordered[index]);
    }
    state->character_count = Party_ListActiveOwnersFar(state->character_ids);
    return 1;
}

void Menu_InitSelectorCursorAndEntries(void)
{
    struct State080a8034 *state;

    state = *(struct State080a8034 **)((u32)&Data_03001f2c);
    PsynergyMenu_InitializeEntryObjects(UiMenu_CreateCursor(state), 2, 2, 8, 0);
    state->field_28 = 0;
    state->field_24 = 0;
    state->field_2c = 0;
    state->field_20 = 0;
    state->field_110 = 0;
    state->field_111 = 0;
    state->field_112 = 8;
    state->field_113 = 2;
}

void Menu_CreateWindowAndEntryObjects(s32 resource, s32 frame)
{
    s32 created;
    s32 handle;
    void *object;
    struct State080a8088 *state;

    state = *(struct State080a8088 **)((u32)&Data_03001f2c);
    created = 0;
    handle = state->handle;
    if (handle == 0) {
        created = UiWindow_UpdateOrCreate(&state->handle, 0, 5, 0x1E, 0xF, 2);
        handle = state->handle;
    }
    if (created != 0) {
        object = SideObject_CreateFar(resource, 0, 0, handle, 0, 0);
        state->object = object;
        if ((((Object0f *)object)->field_0f = 0xF0, state->mode) == 3) {
            Menu_SpawnIconEntries(state, handle);
        }
        Menu_CreateEightEntryObjects(handle);
        CharacterMenu_DrawStatusAilments(handle, resource, 0x100);
        return;
    }
    CharacterMenu_DrawStatusAilments(handle, resource, 0);
}

/*
 * The character menu's command selection: shows the selected character's
 * status and available commands across two panes, switches characters on
 * request, and returns 1 when a command is confirmed or -1 when cancelled.
 */
s32 CharacterMenu_SelectCommand(void)
{
    struct CharacterCommandMenu *menu;
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

    menu = (*(struct CharacterCommandMenu * *)gMenuWork);
    pane = 0;
    result = 0;
    selected = 0;
    total = Party_SumDjinnCountsFar(-1);
    /* FAKEMATCH: retain the family's neg/orr/lsr boolean conversion. */
    has_djinn = (u32)(-total | total) >> 31;
    UiWindow_UpdateOrCreate(&menu->help_window, 0, 0, 30, 5, 2);
    Scheduler_RemoveCallback((u32)(Menu_UpdateEntryObjectTransforms));
    for (i = 3; i >= 0; i--)
        menu->slot_y[i] = 104;
    done = 0;
    UiMenu_SlideCursor(-10, 88);

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        Owner_GetStateFar(menu->owner_id);
        CharacterMenu_DrawStatusAilments(menu->window, menu->owner_id, 1);
        count = CharacterMenu_BuildAvailability(entries, 1, menu->owner_id);
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
                    RenderOutput_RedrawSavedRectFar(menu->help_window);
                    if (has_ailments == 0) {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp,
                            menu->window, SWITCH_HELP_X, -24);
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp + 1,
                            menu->window, 0, -24);
                    }
                } else {
                    RenderOutput_RedrawSavedRectFar(menu->help_window);
                    if (has_djinn != 0)
                        selected = (selected + 8) % 8;
                    else
                        selected = (selected + 7) % 7;
                }
                CharacterMenu_DrawSelectionCursor(pane, selected, entries, 0);
                RenderOutput_ClearListFar(menu->help_window);
                WaitFrames(1);
                if (pane == 0)
                    CharacterMenu_DrawSelectionLabels(menu->help_window, selected, entries);
                else
                    StatusMenu_ShowOwnerProgressMessage(menu->help_window, selected, has_djinn);
            }
            menu->cursor->state = 1;
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
                tab = menu->tab;
                if (gKeysRepeat & 0x100)
                    tab++;
                else
                    tab--;
                tab = (tab + menu->owner_count) % menu->owner_count;
                menu->owner = menu->owners[tab];
                menu->owner_id = menu->owners[tab];
                menu->tab = tab;
                PsynergyMenu_CallIconRoutineWithValue(menu, menu->owners[tab]);
                break;
            }
        }
    }
    RenderOutput_ClearListFar(menu->help_window);
    RenderOutput_RedrawSavedRectFar(menu->help_window);
    UiWindow_ClearInteriorTilesFar(menu->window, 64, 56, 224, 96);
    Scheduler_AddOrUpdateCallback((s32)(Menu_UpdateEntryObjectTransforms), 0xc80);
    for (i = 3; i >= 0; i--)
        menu->slot_y[i] = 128;
    ItemMenu_ResetCategory();
    return result;
}

void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected,
    u8 *entries, s32 invert)
{
    struct State_080a847c *state = (*(struct State_080a847c *volatile *)gMenuWork);
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
    Render_SetTilemapFlagRect(state->object, x, y, width, 1, last);
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
