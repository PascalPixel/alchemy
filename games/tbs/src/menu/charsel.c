#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "layout_guard.h"
#include "types.h"
#include "tbs_edition.h"
#include "character_menu.h"
#include "mem.h"
#include "ui.h"
#include "menu_list.h"
#include "heap_state.h"
#include "window.h"
#include "callback_scheduler.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "inventory_menu.h"
#include "btlsys.h"
#include "keystate.h"
#include "djinn_menu.h"
#include "text/msg_ids.h"
#include "psynergy_menu.h"
#include "battle_unit.h"
#include "global_cells.h"
#include "btltype.h"
#include "menu_result.h"
#include "scene.h"
#include "fixed_math.h"
#include "owner_state.h"
#include "item.h"
#include "a9_motion.h"
#include "object_factory.h"
#include "battle_efx.h"
#include "btlcalc.h"
#include "game_state.h"
#include "resource.h"
#include "uiwindow.h"
#include "four_object_motion.h"
#include "field_sprite.h"
#include "animspr.h"

void FourObjectMotion_InitializeTopRow(void);
void UiWindow_ApplyRectAtObjectOrigin(void *obj, s32 x, s32 y, s32 width, s32 height, s32 palette);

void ItemMenu_Close(void);

#if EDITION_INTERNATIONAL
#define ROW_CNT 8
#else
#define ROW_CNT 4
#endif

s32 Menu_OpenCharacterSelector(void)
{
    struct CharacterMenuState *state =
        (struct CharacterMenuState *)Runtime_AllocateHeapBlock(55, 0x0a70);
    s32 result;
    s32 index;

    gMenuCtrlWork->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);

    state->party_count =
        Party_ListActiveOwnersFar(state->owner_ids);
    Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
    state->selector_window = (struct UiWindow *)UiWindow_CreateFar(13, 0, 17, 5, 2);
    for (index = 0; index < ROW_CNT; index++)
        state->owner_y[index] = 30;
    state->page = 3;

    result = CharacterSelector_Run();

    RenderOutput_ClearListFar(state->status_window);
    ItemMenu_Close();
    gMenuCtrlWork->suspended = 0;
    WaitFrames(1);
    Runtime_ReleaseHeapBlock(55);
    return result;
}

s32 CharacterSelector_Run(void)
{
    struct CharacterMenuState *work = gMenuWork;
    s32 result = 0;

    /* FAKEMATCH: the do-while keeps the cursor store in source order */
    do {
        work->selected_slots[0] = result;
    } while (0);
    if (CharacterMenu_SelectOwner(0) == -1)
        result = -1;
    else
        result = work->selected_ids[0];
    return result;
}

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

#if EDITION_INTERNATIONAL
#define SAVED_TILE_BYTES 0x2000
#else
#define SAVED_TILE_BYTES 0x800
#endif
#define FourObjectMotion_InitializeTopRow ((void (*)())FourObjectMotion_InitializeTopRow)

s32 ActionMenu_Open(void)
{
    struct CharacterMenuState *state = (struct CharacterMenuState *)Runtime_AllocateHeapBlock(55, 0x0a70);
    void *palette = Runtime_BumpAllocateAlternatePool(64);
    void *tiles = Runtime_BumpAllocateAlternatePool(SAVED_TILE_BYTES);
    s32 result;
    s32 index;

    ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    Scheduler_EnableOverlayCallbacksWithFlags();
    UiWindow_InitializeWork(0);
    state->page = 0;
    state->party_count = Party_ListActiveOwnersFar(state->owner_ids);
    Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
    Iwram_CopyWords(palette, (void *)0x05000000, 64);
#if !EDITION_INTERNATIONAL
    Iwram_CopyWords(tiles, (void *)0x06004000, SAVED_TILE_BYTES);
#endif
    Palette_LightenBankHighlight(14);
    Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, REG_DMA3);
    Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, REG_DMA3);
    Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, REG_DMA3);
    Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, REG_DMA3);
#if EDITION_INTERNATIONAL
    Iwram_CopyWords(tiles, (void *)0x06004000, SAVED_TILE_BYTES);
    Iwram_FillWords((void *)0x06004000, SAVED_TILE_BYTES, 0x33333333);
    UiWork_SetAltFlagAndClearTableFar(1);
#endif
    state->selector_window = (struct UiWindow *)UiWindow_CreateFar(13, 0, 17, 5, 2);
    for (index = 0; index < MENU_ROW_COUNT; index++)
        state->owner_y[index] = 30;
    if (Party_SumDjinnCountsFar(-1) != 0)
        FourObjectMotion_InitializeTopRow(state->selector_window, 0);
    for (index = 0; index < 4; index++) {
        state->row_x[index] = 130 + index * 32;
        state->row_y[index] = 0x80;
    }
    Link_DrawShiftedTilePairFar(0x06002500);
    Menu_CancelSoundReset();
    state->page = 0;

    result = Menu_RunActionFlow();

    Menu_EnsureCancelSound();
    RenderOutput_ClearListFar(state->status_window);
    FourObjectMotion_ClearSlotsAndSchedule();
    Scheduler_DisableOverlayCallbacksWithFlags();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
#if EDITION_INTERNATIONAL
    UiWindow_MarkVisibleTileAttributesFar();
    UiWork_SetAltFlagAndClearTableFar(0);
    WaitFrames(1);
#endif
    Iwram_CopyWords((void *)0x05000000, palette, 64);
    Iwram_CopyWords((void *)0x06004000, tiles, SAVED_TILE_BYTES);
    MemFree(tiles);
    MemFree(palette);
    ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 1;
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    Runtime_ReleaseHeapBlock(55);
    ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 0;
    WaitFrames(1);
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 0;
    return result;
}

s32 GameFlag_TestFar(s32 flag);

s32 ItemMenu_SelectItem(void);

s32 Menu_RunActionFlow(void)
{
    struct CharacterMenuState *state = gMenuWork;
    s32 step = 0;
    s32 finished = step;
    s32 result = 0;
    u32 changed;

    while (!finished && !GameFlag_TestFar(0x150)) {
        switch (step) {
        case 0:
            state->selected_slots[0] = finished;
            if (CharacterMenu_SelectOwner(0) == -1) {
                result = -1;
                finished = 1;
            }
            step = 1;
            break;
        case 1:
            state->owner_cursors[0]->active = 13;
            result = CharacterMenu_SelectCommand();
            step = result == -1 ? 0 : 2;
            break;
        case 2:
            state->owner_cursors[0]->active = 13;
            result = PsynergyMenu_SelectAction();
            step = 0;
            if (result != -1)
                step = 3;
            break;
        case 3:
            state->owner_cursors[0]->active = 13;
            result = ItemMenu_SelectItem();
            changed = ~result;
            step = (-changed | changed) >> 31;
            break;
        default:
            finished = 1;
            break;
        }
    }

    if (GameFlag_TestFar(0x150))
        result = -1;

    return result;
}

s32 CharacterMenu_SelectOwner(s32 slot)
{
    struct CharacterMenuState *menu;
    struct RenderOutput *cursor;
    s32 result;
    s32 index;

    menu = (struct CharacterMenuState *)gMenuWork;
    index = menu->owner_index[slot];
    result = 0;
    cursor = menu->owner_cursors[slot];
    cursor->active = 1;
    cursor->anim_step = 0;
    RenderOutput_RedrawSavedRectFar(menu->owner_window);
    if (GameFlag_TestFar(0x172))
        UiWindow_DrawDividerLineFar((s32)menu->owner_window, 9, 1, 9, 3);
    if (index == -1)
        menu->owner_index[slot] = 0;
    else
        UiMenu_SlideCursor(index * 24 - 10, 16);
    if (menu->page == 3)
        result = PsynergyMenu_SelectOwner();
    else
        result = CharacterSelector_RunRearrange();
    cursor = menu->owner_cursors[slot];
    UiIcon_PrepareObject(cursor);
    WaitFrames(1);
    return result;
}

#define gWindowWork (*(struct UiWork * *)&gWindowWork)
#define RenderOutput_RedrawSavedRectFar ((void (*)(s32))RenderOutput_RedrawSavedRectFar)
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)

void AudioCommand_PlayFar(s32 cue);
void UiWindow_DrawDividerLineFar(s32 window, u32 x1, u32 y1, u32 x2, u32 y2);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, u32 mode);

s32 DjinnMenu_ShowCurrentList(void)
{
    struct DjinnMenuWork *menu;
    s32 window;
    s32 row;
    s32 col;

    menu = gMenuWork;
    ItemMenu_ResetCategory();
    AudioCommand_PlayFar(112);
    RenderOutput_RedrawSavedRectFar(menu->help_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgReturnHelp, menu->help_window, 0, 16);
    menu->cursor->active = 13;
    menu->second_cursor->active = 13;
    WaitFrames(1);
    window = menu->window;
    {
        s32 i;

        for (i = 0; i < 4; i++) {
            menu->slot_x[i] = 32 + i * 56;
            menu->slot_y[i] = 70;
            menu->owner_y[i] = 30;
        }
    }
    RenderOutput_RedrawSavedRectFar(window);
    UiWindow_DrawDividerLineFar(window, 0, 11, 28, 11);
    UiText_DrawResourceFar((s32)MsgCurrentDjinn, menu->help_window, -96, 132);
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 7; col++) {
            s32 id = row * 20 + col;

            if (GameFlag_TestFar(48 + id)) {
                UiWindow_SetTilemapEntryFar(window, 0x1001 + row,
                    row * 7 + 1, col + 3, 0);
                UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnName + id,
                    window, row * 56 + 16, col * 8 + 24);
            }
        }
    }
    gWindowWork->dirty = 1;
    for (;;) {
        if (GameFlag_TestFar(0x150))
            break;
        WaitFrames(1);
        if (gKeyState & (KEY_A | KEY_B | KEY_SELECT))
            break;
    }
    RenderOutput_RedrawSavedRectFar(menu->window);
    RenderOutput_ClearListFar(menu->help_window);
    {
        s32 i;

        for (i = 0; i < 4; i++) {
            menu->slot_x[i] = 130 + i * 32;
            menu->slot_y[i] = 128;
        }
    }
    menu->cursor->active = 1;
    menu->second_cursor->active = 1;
    AudioCommand_PlayFar(113);
    /* Returns no value: the ROM has the s32 epilogue (pop {r1}; bx r1) after this void call. */
}

#define PsynergyMenu_CallIconRoutineWithValue ((void (*)(void*,s32))PsynergyMenu_CallIconRoutineWithValue)
#define DjinnMenu_ShowCurrentList ((void (*)(void))DjinnMenu_ShowCurrentList)
#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)

struct InventoryMenuState;
void PsynergyMenu_InitializeEntryObjects();
void Menu_SpawnIconEntries(struct InventoryMenuState *menu, s32 window);

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
    UnitGetFar(work->owner_ids[cursor]);
    for (i = 0; i < 4; i++) {
        work->row_x[i] = 130 + i * 32;
        work->row_y[i] = 0x80;
    }
    Palette_LightenBankHighlight(14);
    Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, REG_DMA3);
    Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, REG_DMA3);
    Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, REG_DMA3);
    Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, REG_DMA3);
    while (!GameFlag_TestFar(0x150)) {
        if (redraw) {
            redraw = 0;
            RenderOutput_RedrawSavedRectFar((s32)work->selector_window);
            UiText_DrawCharacterAtOffsetFar((s32)MsgRearrangeHelp, (s32)work->selector_window, 0, 0);
            if (GameFlag_TestFar(48))
                UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnListHelp, (s32)work->selector_window, 0, 16);
            UiText_DrawCharacterAtOffsetFar((s32)MsgRearrangeHelp - 3, (s32)work->selector_window, 0, 8);
            cursor = (cursor + count) % count;
            UnitGetFar(work->owner_ids[cursor]);
            page = (page + 3) % 3;
            Menu_CreateWindowAndEntryObjects(work->owner_ids[cursor], page);
            PsynergyMenu_CallIconRoutineWithValue(work, work->owner_ids[cursor]);
            for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
                work->owner_y[i] = 30;
            work->owner_y[cursor] = 26;
        }
        UiMenu_PositionCursor(cursor * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            AudioCommand_PlayFar(0x70);
            result = 1;
            break;
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(0x71);
            result = -1;
            break;
        }
        if (gKeysRepeat & KEY_R) {
            if (CharacterSelector_MoveEntry(cursor, 1)) {
                AudioCommand_PlayFar(0x70);
                cursor++;
                Menu_ReleaseEntryObjects();
                PsynergyMenu_InitializeEntryObjects(work->owner_window, 2, 2, 8, 0);
                for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
                    work->owner_y[i] = 30;
                work->owner_y[cursor] = 26;
            } else {
                AudioCommand_PlayFar(0x72);
            }
            WaitFrames(1);
        } else if (gKeysRepeat & KEY_L) {
            if (CharacterSelector_MoveEntry(cursor, 0)) {
                AudioCommand_PlayFar(0x70);
                cursor--;
                Menu_ReleaseEntryObjects();
                PsynergyMenu_InitializeEntryObjects(work->owner_window, 2, 2, 8, 0);
                for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
                    work->owner_y[i] = 30;
                work->owner_y[cursor] = 26;
            } else {
                AudioCommand_PlayFar(0x72);
            }
            WaitFrames(1);
        } else if ((gKeyState & KEY_SELECT) && GameFlag_TestFar(48)) {
            DjinnMenu_ShowCurrentList();
            redraw = 1;
        } else {
            if (gKeysRepeat & KEY_LEFT) {
                AudioCommand_PlayFar(0x6f);
                if (count > 1) {
                    cursor--;
                    redraw = 1;
                }
            }
            if (gKeysRepeat & KEY_RIGHT) {
                AudioCommand_PlayFar(0x6f);
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

s32 PsynergyMenu_SelectOwner(void)
{
    struct PsynergyMenuState *menu = gMenuWork;
    s32 selection = menu->tab_index[0];
    s32 count = (s8)menu->tab_counts[0];
    s32 pending = 1;
    s32 frame = menu->flags;
    s32 result;
    s32 i;

    UnitGetFar(menu->owner_table[selection]);
    RenderOutput_RedrawSavedRectFar(menu->message_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgChooseCharacter, menu->message_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgChooseCharacter + 1, menu->message_window, 0, 16);
    for (;;) {
        if (pending) {
            pending = 0;
            selection = (selection + count) % count;
            UnitGetFar(menu->owner_table[selection]);
            frame = (frame + 3) % 3;
            Menu_CreateWindowAndEntryObjects(menu->owner_table[selection], frame);
            PsynergyMenu_CallIconRoutineWithValue(menu, menu->owner_table[selection]);
            for (i = 0; i < MENU_ROW_COUNT; i++)
                menu->row_positions[i] = 30;
            menu->row_positions[selection] = 26;
            menu->psynergy_count = PsynergyMenu_CollectActions(UnitGetFar(menu->owner_table[selection]), (u16 *)menu->psynergies, 0);
            PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
            Menu_PlaceEntryObjectsInGrid(96, 96, 8);
            Menu_HideEmptyEntryIcons(menu->psynergies);
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            AudioCommand_PlayFar(112);
            result = 1;
            break;
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        {
            if (gKeysRepeat & 32) {
                AudioCommand_PlayFar(111);
                if (count > 1) {
                    selection--;
                    pending = 1;
                }
            }
            if (gKeysRepeat & 16) {
                AudioCommand_PlayFar(111);
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
    has_djinn = 0;
    if (total != 0)
        has_djinn = 1;
    UiWindow_UpdateOrCreate((s32 *)&menu->help_window, 0, 0, 30, 5, 2);
    Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
    for (i = 3; i >= 0; i--)
        menu->row_y[i] = 104;
    done = 0;
    UiMenu_SlideCursor(-10, 88);

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        UnitGetFar(menu->selected_ids[0]);
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
                        UiText_DrawCharacterAtOffsetFar((s32)MsgSwitchCharacterHelp,
                            (s32)menu->status_window, SWITCH_HELP_X, -24);
                        UiText_DrawCharacterAtOffsetFar((s32)MsgSwitchCharacterHelp + 1,
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
            if (gKeysRepeat & KEYS_DPAD)
                CharacterMenu_DrawSelectionCursor(pane, selected, entries, 1);
            if (gKeyState & KEY_A) {
                AudioCommand_PlayFar(112);
                done = 1;
                result = 1;
                break;
            }
            if (gKeyState & KEY_B) {
                AudioCommand_PlayFar(113);
                done = 1;
                result = -1;
                break;
            }
            if (gKeysRepeat & KEY_UP) {
                AudioCommand_PlayFar(111);
                redraw = 1;
                selected--;
            }
            if (gKeysRepeat & KEY_DOWN) {
                AudioCommand_PlayFar(111);
                redraw = 1;
                selected++;
            }
            if (gKeysRepeat & KEY_RIGHT) {
                AudioCommand_PlayFar(111);
                redraw = 1;
                pane++;
            }
            if (gKeysRepeat & KEY_LEFT) {
                AudioCommand_PlayFar(111);
                redraw = 1;
                pane--;
            }
            if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
                AudioCommand_PlayFar(111);
                tab = menu->owner_index[0];
                if (gKeysRepeat & KEY_R)
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
    Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
    for (i = 3; i >= 0; i--)
        menu->row_y[i] = 128;
    ItemMenu_ResetCategory();
    return result;
}

void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected,
    u8 *entries, s32 invert)
{
    struct CharacterMenuState *state = (struct CharacterMenuState *)gMenuWork;
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

    different = 1 ^ invert;
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
                s32 message = (s32)(MsgStatusAdvice + index * 2);
                UiText_DrawResourceFar(message, target, 0, -1);
                UiText_DrawResourceFar(message + 1, target, 0, 15);
            }
            count++;
        }
        index++;
    } while (index <= 4);

    if (count == 0)
        UiText_DrawResourceFar((s32)MsgStatusNormal, target, 0, 0);
}

#if EDITION_INTERNATIONAL
#define PROGRESS_TEXT_MAX 0x80
#else
#define PROGRESS_TEXT_MAX 0x40
#endif

void StatusMenu_ShowOwnerProgressMessage(
    struct UiWindow *destination,
    s32 message_variant,
    s32 preserve_variant)
{
	void UiWork_PushValueSlotFar(s32 quantity, s32 style);
    struct CharacterMenuState *menu = gMenuWork;
    void *buffer;

    if (preserve_variant == 0 && message_variant > 3) {
        message_variant++;
    }
    if (message_variant == 1) {
        struct BtlUnit *owner = UnitGetFar(menu->selected_ids[0]);

        if (owner->level == 99) {
            message_variant = 8;
        } else {
#if EDITION_INTERNATIONAL
            u64 remaining = Owner_GetLevelThresholdFar(menu->selected_ids[0], owner->level + 1) -
                owner->experience;
#else
            u32 remaining = Owner_GetLevelThresholdFar(menu->selected_ids[0], owner->level + 1) -
                owner->experience;
#endif
            UiWork_PushValueSlotFar(remaining, 5);
        }
    }

    buffer = MemAlloc(PROGRESS_TEXT_MAX * 2);
    UiText_CopyMessageStringFar(message_variant + (s32)MsgProgressHelp, buffer, PROGRESS_TEXT_MAX);
    UiText_RenderWideStringAtOffsetFar(buffer, destination, 0, -1);
    MemFree(buffer);
}

#undef UiWindow_ClearInteriorTilesFar

void UiText_DrawNumberInWindowFar(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

#undef UiWindow_ClearInteriorTilesFar
#define UiWindow_ClearInteriorTilesFar ((void (*)(void*,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)
#define UiText_DrawStringInWindowFar ((void (*)(const void*,void*,s32,s32))UiText_DrawStringInWindowFar)
#define UiText_DrawNumberInWindowFar ((void (*)(s32,s32,void*,s32,s32))UiText_DrawNumberInWindowFar)

s32 Party_SumDjinnCountsFar(s32 side);
void UiWork_SetParamNibbleFar(s32 color);
void WaitFrames(s32 frames);

void CharacterMenu_DrawStatusAilments(struct UiWindow *window, s32 unit, s32 mode)
{
	void UiText_DrawStringAtOffsetFar(const void *text, void *window, s32 x, s32 y);
    struct CharacterMenuState *state = gMenuWork;
    struct BtlUnit *status;
    s32 row;
    s32 has_djinn;
    s32 keep;
    s32 i;
    u8 ailments[8];

    if (Party_SumDjinnCountsFar(-1) != 0)
        has_djinn = 1;
    else
        has_djinn = 0;
    status = UnitGetFar(unit);
    if ((mode & 0xff) == 1)
        row = 7;
    else
        row = 10;
    state->status_cursor->active = 1;
    ItemMenu_DrawOwnerStatus((s32)window, unit, mode);
    CharacterMenu_BuildAvailability(ailments, 1, unit);
    CharacterMenu_UpdateSelectionIcons(ailments);
    keep = mode & 0x100;
    if (keep == 0)
        UiWindow_ClearInteriorTilesFar(window, 0, 40, 96, 96);
    i = 0;
    if (ailments[CHARACTER_DOWN] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgDownLabel, window, 16, 40);
        i = 1;
    }
    if (ailments[CHARACTER_POISON] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgPoisonLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (ailments[CHARACTER_VENOM] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgVenomLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (ailments[CHARACTER_CURSE] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgCurseLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (ailments[CHARACTER_HAUNT] != 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgHauntLabel, window, 16, i * 16 + 40);
        i++;
    }
    if (i == 0)
        UiText_DrawCharacterAtOffsetFar((s32)MsgNormalLabel, window, 0, 40);
    CharacterMenu_UpdateSelectionIcons(ailments);
    ItemMenu_ApplyFlags(ailments);
    if (state->page == 3)
        return;
    if (keep == 0) {
        WaitFrames(1);
        UiWindow_ClearInteriorTilesFar(window, 64, 56, 224, 96);
    }
    UiWork_SetParamNibbleFar(15);
    if (mode == 1 || has_djinn == 1) {
        UiWindow_SetTilemapEntryFar(window, 1, 15, row, 4);
        UiWindow_SetTilemapEntryFar(window, 2, 19, row, 4);
        UiWindow_SetTilemapEntryFar(window, 3, 23, row, 4);
        UiWindow_SetTilemapEntryFar(window, 4, 27, row, 4);
    }
    if (has_djinn)
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnLabel, window, 64, row * 8 + 8);
    if (mode == 1) {
        if (!has_djinn)
            row--;
#if EDITION_INTERNATIONAL
        UiText_DrawStringAtOffsetFar(Menu_LvString, window, 64, row * 8 + 16);
#else
        UiText_DrawStringInWindowFar(Menu_LvString, window, 64, row * 8 + 16);
#endif
        UiText_DrawCharacterAtOffsetFar((s32)MsgPowerLabel, window, 64, row * 8 + 24);
        UiText_DrawCharacterAtOffsetFar((s32)MsgPowerLabel + 1, window, 64, row * 8 + 32);
    }
    for (i = 0; i <= 3; i++) {
        if (has_djinn)
            UiText_DrawNumberInWindowFar(status->djinn_owned_counts[i], 1, window, i * 32 + 120, row * 8 + 8);
        if ((mode & 0xff) == 1) {
            if (has_djinn) {
                UiText_DrawNumberInWindowFar(status->djinn_active_counts[i], 1, window, i * 32 + 104, row * 8 + 8);
                UiText_DrawStringInWindowFar(Menu_SlashString, window, i * 32 + 112, row * 8 + 8);
            }
            UiText_DrawNumberInWindowFar(Owner_GetResistanceValueFar(unit, i), 2, window, i * 32 + 112, row * 8 + 16);
            UiText_DrawNumberInWindowFar(status->elm[i].pow, 3, window, i * 32 + 104, row * 8 + 24);
            UiText_DrawNumberInWindowFar(status->elm[i].res, 3, window, i * 32 + 104, row * 8 + 32);
        }
    }
}

#if EDITION_INTERNATIONAL
#include "types.h"

void Runtime_RunBusyWaitDelay(void)
{
    s32 count = 0xff;

spin:
    count--;
    if (count >= 0)
        goto spin;
}

#endif

#undef UiText_DrawNumberInWindowFar
#undef UiText_DrawStringInWindowFar
#undef UiWindow_ClearInteriorTilesFar

#undef UiWindow_ClearInteriorTilesFar
#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)
#undef UiText_DrawStringInWindowFar
#define UiText_DrawStringInWindowFar ((void (*)(const void*,s32,s32,s32))UiText_DrawStringInWindowFar)
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);

#if EDITION_INTERNATIONAL
#define STAT_DIGITS 3
#define STAT_X      200
#else
#define STAT_DIGITS 4
#define STAT_X      192
#endif

void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags)
{
	void UiText_DrawStringAtOffsetFar(const void *text, void *window, s32 x, s32 y);
    struct InventoryMenuState *menu;
    struct BtlUnit *unit;

    menu = gMenuWork;
    unit = UnitGetFar(owner);
    menu->cursor->active = 1;
    flags &= 0x100;
    if (flags == 0) {
        UiWindow_ClearInteriorTilesFar(window, 0, 0, 128, 40);
    }
    UiText_DrawStringAtOffsetFar(unit->name, window, 40, 0);
    UiText_DrawCharacterAtOffsetFar(unit->class_index + (s32)MsgClassName, window, 0, 32);
#if EDITION_INTERNATIONAL
    UiText_DrawStringAtOffsetFar(Menu_LvString, window, 104, 0);
#else
    UiText_DrawStringInWindowFar(Menu_LvString, window, 104, 0);
#endif
    UiWork_SetParamNibbleFar(15);
    UiText_DrawNumberInWindowFar(unit->level, 2, window, 128, 0);
#if EDITION_INTERNATIONAL
    UiText_DrawStringAtOffsetFar(Menu_HpString, window, 40, 16);
#else
    UiText_DrawStringInWindowFar(Menu_HpString, window, 40, 16);
#endif
    UiText_DrawNumberInWindowFar(unit->hp, 4, window, 72, 16);
    UiText_DrawNumberInWindowFar(unit->max_hp, 4, window, 112, 16);
    UiText_DrawStringInWindowFar(Menu_SlashString, window, 104, 16);
#if EDITION_INTERNATIONAL
    UiText_DrawStringAtOffsetFar(Menu_PpString, window, 40, 24);
#else
    UiText_DrawStringInWindowFar(Menu_PpString, window, 40, 24);
#endif
    UiText_DrawNumberInWindowFar(unit->ep, 4, window, 72, 24);
    UiText_DrawNumberInWindowFar(unit->max_ep, 4, window, 112, 24);
    UiText_DrawStringInWindowFar(Menu_SlashString, window, 104, 24);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel, window, 40, 8);
    UiText_DrawNumberInWindowFar(unit->experience, 7, window, 88, 8);
    if (flags == 0) {
        WaitFrames(1);
        UiWindow_ClearInteriorTilesFar(window, 144, 0, 224, 40);
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 23, window, 152, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 22, window, 152, 8);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 21, window, 152, 16);
    UiText_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 20, window, 152, 24);
    UiText_DrawNumberInWindowFar(unit->atk, STAT_DIGITS, window, STAT_X, 0);
    UiText_DrawNumberInWindowFar(unit->def, STAT_DIGITS, window, STAT_X, 8);
    UiText_DrawNumberInWindowFar(unit->agi, STAT_DIGITS, window, STAT_X, 16);
    UiText_DrawNumberInWindowFar(unit->luck, STAT_DIGITS, window, STAT_X, 24);
}

s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 id)
{
    struct BtlUnit *state;
    s32 i;
    s32 zero;
    s32 count;
    s32 mode;

    state = UnitGetFar(id);
    zero = 0;
    for (i = 4; i >= 0; i--)
        output[i] = zero;

    count = 0;
    if (state->hp == 0 && requested == 1) {
        output[CHARACTER_DOWN] = requested;
        count = 1;
    }

    mode = state->doku;
    if (mode != 0) {
        if (mode == 1)
            output[CHARACTER_POISON] = mode;
        else
            output[CHARACTER_VENOM] = 1;
        count++;
    }
    if (state->restraint != 0) {
        output[CHARACTER_CURSE] = 1;
        count++;
    }
    if (state->akuryo != 0) {
        output[CHARACTER_HAUNT] = 1;
        count++;
    }
    return count;
}

#if EDITION_INTERNATIONAL
#define GROUP_LEN 5
#else
#define GROUP_LEN 6
#endif

s32 Menu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    struct InventoryMenuState *menu = gMenuWork;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    encoded = (s32)UnitGetFar(menu->pane_owner[index]);
    limit = menu->item_count;
    value = menu->selected_index_by_owner[menu->pane_owner[index]];
    if ((value + 1) > limit) {
        value = limit - 1;
    }
    if (limit == 0) {
        value = 0;
    }
    quotient = value / GROUP_LEN;
    remainder = value % GROUP_LEN;
    groups = limit / GROUP_LEN;
    if (limit % GROUP_LEN != 0) {
        groups++;
    }
    result->owner_state = encoded;
    result->page = quotient;
    result->page_count = groups;
    result->row = remainder;
    result->entry_count = limit;
    result->selected_index = value;
    return 1;
}

static __inline__ u32 XorWord(u32 word, u32 mask)
{
    return word ^ mask;
}

extern const struct TileMask Menu_PatternRowMasks[];

void Menu_BuildPatternTiles(void)
{
#if EDITION_INTERNATIONAL
    u32 *vram = (u32 *)0x06005000;
#else
    u32 *vram = (u32 *)0x06004000;
#endif
    s32 set;
    s32 n;

    for (set = 0; set < 2; set++) {
        for (n = 0; n < 6; n++) {
            u32 *tile = vram + set * 0x60 + n * 0x10;
            s32 x;

            Iwram_FillWords(tile, 64, 0x44444444);
            for (x = 1; x <= 7; x++) {
                s32 mi = n;

                if (set == 1 && x <= 1) {
                    continue;
                }
                if (set == 0 && n > x - 2) {
                    mi = x - 2;
                    if (mi < 0) {
                        mi = 0;
                    }
                }
                tile[x] = XorWord(tile[x], Menu_PatternRowMasks[mi].word0);
                tile[x + 8] = XorWord(tile[x + 8], Menu_PatternRowMasks[mi].word1);
            }
        }
    }
}

#undef RenderOutput_ClearListFar
#undef RenderOutput_RedrawSavedRectFar
#undef UiText_DrawStringInWindowFar

void PsynergyMenu_DrawRange(
    s32 window, s32 x, s32 y, s32 range, s32 unused)
{
    s32 n;
    s32 tile;

    (void)unused;

    n = range * 2;
    tile = n + RANGE_ICON_TILE + 1;
    UiWindow_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0);
    UiWindow_SetTilemapEntryFar(window, n + RANGE_ICON_TILE, x + 1, y, 0);
    UiWindow_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

#if !EDITION_INTERNATIONAL
#endif

#define ACTION_ID_MASK 0x3fff

s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused, struct MenuResult *state)
{
    struct PsynergyMenuState *menu;
    struct BtlAct *ability;
    s32 row;
    s32 base;

    menu = (struct PsynergyMenuState *)gMenuWork;
    state->selected_index = state->page * PAGE_ROWS + state->row;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
    RenderOutput_RedrawSavedRectFar(menu->info_window);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    UiWindow_ClearInteriorTilesFar((s32)menu->info_window, 0, 0, 224, 8);
#else
    RenderOutput_ClearListFar(menu->info_window);
#endif
    WaitFrames(1);
    if (menu->psynergies[state->selected_index] != 0) {
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar(
#else
        UiText_DrawResourceFar(
#endif
            (menu->psynergies[state->selected_index] & ACTION_ID_MASK)
                + (s32)MsgAbilityDescription,
            (s32)menu->info_window, 0, 0);
        ability = BtlActGet(menu->psynergies[state->selected_index] & ACTION_ID_MASK);
#if EDITION_INTERNATIONAL
        UiWindow_ClearInteriorTilesFar(window, 0, 96, 224, 104);
        row = 0;
        if (ability->type_0c != 0 || (ability->tgt_flg & 0x40) != 0) {
            row = 2;
        }
        if ((ability->tgt_flg & 0x80) != 0) {
            row |= 1;
        }
        if (row == 3) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsableAnywhere, window, 0, 96);
        } else if (row == 2) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsableInField, window, 0, 96);
        } else if (row == 1) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsableInBattle, window, 0, 96);
        }
#else
        UiWindow_ClearInteriorTilesFar(window, 0, 72, 64, 96);
        row = 0;
        if (ability->type_0c != 0 || (ability->tgt_flg & 0x40) != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsableInField, window, 0, 72);
            row = 1;
        }
        if ((ability->tgt_flg & 0x80) != 0) {
            UiText_DrawCharacterAtOffsetFar((s32)MsgUsableInBattle, window, 0, row * 8 + 72);
            row++;
        }
        UiText_DrawCharacterAtOffsetFar((s32)MsgCanBeUsed, window, 0, row * 8 + 72);
#endif
    }

    base = state->page * PAGE_ROWS;
    for (row = 0; row <= PAGE_ROWS - 1; row++) {
        if (row == state->row) {
            ability = BtlActGet(menu->psynergies[base + row] & ACTION_ID_MASK);
            if (ability->damage_class != 4) {
                UiWindow_SetTilemapEntryFar(window, ability->damage_class + 1, 24, row * 2 + LIST_FIRST_ROW, 0);
                Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1, 14);
                Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1, 14);
            } else {
                Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1, 14);
            }
        } else {
            ability = BtlActGet(menu->psynergies[base + row] & ACTION_ID_MASK);
            if (ability->damage_class != 4) {
                UiWindow_SetTilemapEntryFar(window, ability->damage_class + 1, 24, row * 2 + LIST_FIRST_ROW, 4);
                Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1, 15);
                Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1, 15);
            } else {
                Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1, 15);
            }
        }
    }
    WaitFrames(1);
    return 1;
}
void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
struct BtlAct * BtlActGet(s32 action_id);
#undef UiText_DrawStringInWindowFar
#define UiText_DrawStringInWindowFar ((void (*)(u8*,s32,s32,s32))UiText_DrawStringInWindowFar)

#define Menu_LvString (*(u8 *)&Menu_LvString)

#define ACT_ID_MASK 0x3fff
void UiText_DrawNumberAtOffsetFar(s32, s32, s32, s32, s32);

s32 PsynergyMenu_DrawListPage(
    s32 window, s32 unused, const struct MenuResult *res)
{
	void UiText_DrawStringAtOffsetFar(const void *, void *, s32, s32);
    struct PsynergyMenuState *menu = gMenuWork;
    struct BtlUnit *owner;
    u32 first;
    u32 rows;
    u8 row;
    s32 ofs;

    (void)unused;

    owner = UnitGetFar(menu->owner_ids[0]);

    RenderOutput_RedrawSavedRectFar(window);

    first = res->page * PAGE_ROWS;
    rows = (u8)(res->entry_count - first);
    if (rows > PAGE_ROWS) {
        rows = PAGE_ROWS;
    }

    Menu_SetPageIcons(PAGE_ROWS, first, window, LIST_ICONS_X, LIST_ICONS_Y);
    Menu_DrawPageIndicator(window, res->entry_count, PAGE_ROWS, res->page, 28);

    UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, LIST_PP_X, 0);

    row = 0;
    if (rows > row) {
        ofs = first;
        do {
            struct BtlAct *act;
            s32 msg;
            s32 y;
            s32 range;
            act = BtlActGet(
                ACT_ID_MASK & menu->psynergies[ofs]);
            msg = (menu->psynergies[ofs] & ACT_ID_MASK) +
                (s32)MsgAbilityName;
            y = row * 16 + LIST_FIRST_ROW * 8;

            UiText_DrawCharacterAtOffsetFar(msg, window, LIST_NAME_X, y);
            UiText_DrawNumberAtOffsetFar(act->pp_cost, 2, window, LIST_PP_NUMBER_X, y);

            range = act->rng;
            if (range == 0xff) {
                range = 11;
            } else {
                range--;
            }
            PsynergyMenu_DrawRange(window, 25, row * 2 + LIST_FIRST_ROW, range, 0);

            row++;
            ofs++;
        } while (rows > row);
    }

    if (menu->psynergy_count == 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgNoPsynergy, window, 96, LIST_EMPTY_Y);
    }

    UiText_DrawStringAtOffsetFar(owner->name, (void *)window, 40, 0);
    UiText_DrawCharacterAtOffsetFar(
        owner->class_index + (s32)MsgClassName, window, 0, 32);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
    UiText_DrawStringAtOffsetFar(&Menu_LvString, (void *)window, 0, 48);
#else
    UiText_DrawStringInWindowFar(&Menu_LvString, window, 0, 48);
#endif
    UiText_DrawNumberInWindowFar(owner->level, 2, window, 24, 48);

    return 1;
}

#undef PsynergyMenu_CallIconRoutineWithValue

struct BtlUnit;
void ItemMenu_HideAllIcons(void);

s32 PsynergyMenu_SelectAction(void)
{

    struct PsynergyMenuState *menu;
    s32 result;
    s32 done;
    s32 redraw;
    s32 first;
    s32 nav;
    s32 tab;
    s32 i;
    struct MenuResult state;

    menu = ((struct PsynergyMenuState *)gMenuWork);
    result = 0;
    done = 0;
    Menu_BuildPatternTiles();
    RenderOutput_RedrawSavedRectFar(menu->message_window);
    UiWindow_UpdateOrCreate((s32 *)&menu->info_window, 0, 0, 30, 5, 2);
    for (i = 3; i >= 0; i--)
        menu->slot_y[i] = -16;
    {
        s32 priority = 245;
        struct RenderOutput **icons = menu->entry_icons;

        for (i = 31; i >= 0; i--) {
            struct RenderOutput *icon = *icons++;

            if (icon != 0)
                icon->sentinel = priority;
        }
    }
    Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
    Menu_SpawnIconEntries(menu, menu->message_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgSwitchCharacterHelp, menu->status_window, SWITCH_HELP_X, -24);
    UiText_DrawCharacterAtOffsetFar((s32)MsgSwitchCharacterHelp + 2, menu->status_window, 0, -24);

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        ItemMenu_PosCategory();
        RenderOutput_RedrawSavedRectFar(menu->status_window);
        menu->psynergy_count = PsynergyMenu_CollectActions(
            (struct BtlUnit *)UnitGetFar(menu->owner_ids[0]), menu->psynergies, 0);
        WaitFrames(1);
        Menu_BuildPageResult(&state, 0);
        PsynergyMenu_DrawPreparedPsynergyIcons(menu->status_window, menu->owner_ids[0]);
        redraw = 1;
        first = 1;

        while (GameFlag_TestFar(0x150) == 0) {
            if (redraw != 0) {
                redraw = 0;
                if (first != 0) {
                    first = 0;
                    PsynergyMenu_DrawListPage(menu->status_window, 0, &state);
                }
                PsynergyMenu_DrawRangePage(menu->status_window, 0, &state);
                WaitFrames(1);
            }
            WaitFrames(1);
            nav = Menu_HandlePageInput(0, state.entry_count, PAGE_ROWS, &state.row, &state.page);
            menu->pane_icon[0]->active = 1;
#if EDITION_INTERNATIONAL
            UiMenu_PositionCursor(55, state.row * 16 + 60);
#else
            UiMenu_PositionCursor(58, state.row * 16 + 52);
#endif
            if (nav == 1) {
                first = 1;
                redraw = 1;
            }
            if (nav == 0)
                redraw = 1;
            if (nav == -1)
                redraw = 0;
            if (gKeyState & KEY_A) {
                AudioCommand_PlayFar(112);
                result = 1;
                done = 1;
                break;
            }
            if (gKeyState & KEY_B) {
                AudioCommand_PlayFar(113);
                result = -1;
                done = 1;
                Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
                break;
            }
            if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
                AudioCommand_PlayFar(111);
                tab = menu->tab_index[0];
                menu->selected_index_by_owner[menu->owner_table[tab]] = state.selected_index;
                if (gKeysRepeat & KEY_R)
                    tab++;
                else
                    tab--;
                tab = (tab + menu->owner_count) % menu->owner_count;
                menu->selected_owner = menu->owner_table[tab];
                menu->owner_ids[0] = menu->owner_table[tab];
                menu->tab_index[0] = tab;
                PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->owner_table[tab]);
                break;
            }
        }
    }
    RenderOutput_ClearListFar((s32)menu->info_window);
    RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
    ItemMenu_HideAllIcons();
    RenderOutput_ClearListFar(menu->message_window);
    RenderOutput_RedrawSavedRectFar(menu->status_window);
    return result;
}

s32 PsynergyMenu_ActionReturnTrue(void)
{
    return 1;
}

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = gMenuWork;

    UnitGetFar(owner_id);
    ItemMenu_HideAllIcons();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_PreparedIconsNoOp(void)
{
}

void PsynergyMenu_PreparedIconsSecondNoOp(void)
{
}

#undef RenderOutput_ClearListFar
#undef RenderOutput_RedrawSavedRectFar

struct ItemDefinition *Item_GetDirectFar(s32 item);

#if EDITION_INTERNATIONAL
#define EQUIP_SLOT_X     1
#define EQUIP_SLOT_WIDTH 12
#else
#define EQUIP_SLOT_X     2
#define EQUIP_SLOT_WIDTH 9
#endif

#define ITEM_ID_MASK 0x1ff

s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state)
{
    struct InventoryMenuState *menu;
    struct ItemDefinition *item;
    s32 base;
    s32 row;

    menu = gMenuWork;
    base = state->page * PAGE_ROWS;
    state->selected_index = base + state->row;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
    RenderOutput_RedrawSavedRectFar(menu->info_window);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    UiWindow_ClearInteriorTilesFar((s32)menu->info_window, 0, 0, 224, 8);
#else
    RenderOutput_ClearListFar(menu->info_window);
#endif
    WaitFrames(1);
    if (menu->items[state->selected_index] != 0) {
#if EDITION_INTERNATIONAL
        UiText_DrawCharacterAtOffsetFar((menu->items[state->selected_index] & ITEM_ID_MASK)
                + (s32)MsgItemPlainName,
            (s32)menu->info_window, 0, 0);
#else
        UiText_DrawResourceFar((menu->items[state->selected_index] & ITEM_ID_MASK)
                + (s32)MsgItemPlainName,
            (s32)menu->info_window, 0, 0);
#endif
    }

    for (row = 0; row < PAGE_ROWS; row++) {
        if (row == state->row) {
            item = Item_GetDirectFar(menu->items[state->selected_index] & ITEM_ID_MASK);
            if (item->element != 4) {
                UiWindow_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1, 0);
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 14);
            } else {
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 14);
            }
        } else {
            item = Item_GetDirectFar(menu->items[base + row] & ITEM_ID_MASK);
            if (item->element != 4) {
                UiWindow_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1, 4);
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 15);
            } else {
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 15);
            }
        }
    }

    for (row = 0; row <= 3; row++) {
        Render_SetTilemapFlagRect((s32)menu->equip_window, EQUIP_SLOT_X, row * 2 + 1, EQUIP_SLOT_WIDTH, 1, 15);
    }

    if (menu->items[state->selected_index] & 0x200) {
        item = Item_GetDirectFar(menu->items[state->selected_index] & ITEM_ID_MASK);
        switch (item->type) {
        case 1:
            Render_SetTilemapFlagRect((s32)menu->equip_window, EQUIP_SLOT_X, 1, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        case 4:
            Render_SetTilemapFlagRect((s32)menu->equip_window, EQUIP_SLOT_X, 3, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        case 3:
            Render_SetTilemapFlagRect((s32)menu->equip_window, EQUIP_SLOT_X, 5, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        case 2:
            Render_SetTilemapFlagRect((s32)menu->equip_window, EQUIP_SLOT_X, 7, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        }
    }
    WaitFrames(1);
    return 1;
}

#if EDITION_INTERNATIONAL
#define SHOP_PAGE_ICONS_X 119
#define SHOP_ITEM_NAME_X  128
#define SHOP_STAT_X       80
#else
#define SHOP_PAGE_ICONS_X 123
#define SHOP_ITEM_NAME_X  136
#define SHOP_STAT_X       72
#endif

s32 Shop_DrawItemPage(s32 window, s32 unused, struct MenuResult *state)
{
	void UiText_DrawStringAtOffsetFar(const void *text, void *window, s32 x, s32 y);
    struct InventoryMenuState *menu = gMenuWork;
    struct BtlUnit *unit;
    u16 *item;
    s32 first;
    u32 count;
    u32 row;
    s32 message;

    unit = UnitGetFar(menu->pane_owner[0]);
    UiWindow_ClearInteriorTilesFar(window, 128, 8, 224, 96);
    first = state->page * PAGE_ROWS;
    count = (u8)(state->entry_count - first);
    if (count > PAGE_ROWS)
        count = PAGE_ROWS;
    Menu_SetPageIcons(PAGE_ROWS, first, window, SHOP_PAGE_ICONS_X, 52);
    Menu_DrawPageIndicator(window, state->entry_count, PAGE_ROWS, state->page, 28);
    if (menu->item_count == 0) {
#if defined(TBS_EDITION_DE)
        UiText_DrawCharacterAtOffsetFar((s32)MsgItemMenuEmpty, window, 112, 8);
#else
        UiText_DrawCharacterAtOffsetFar((s32)MsgItemMenuEmpty, window, 120, 8);
#endif
    } else {
        row = 0;
        if (count > row) {
            item = &menu->items[first];
            do {
                UiText_DrawCharacterAtOffsetFar((*item & 0x1ff) + (s32)MsgItemName,
                    window, SHOP_ITEM_NAME_X, (row << 4) + 8);
                row = (u8)(row + 1);
                item++;
            } while (count > row);
        }
    }
    UiText_DrawStringAtOffsetFar((s32)unit->name, window, 40, 0);
    message = (s32)MsgStatLabel;
    UiText_DrawCharacterAtOffsetFar(message, window, 32, 16);
    UiText_DrawCharacterAtOffsetFar(message + 1, window, 32, 24);
    UiText_DrawNumberInWindowFar(unit->atk, 3, window, SHOP_STAT_X, 16);
    UiText_DrawNumberInWindowFar(unit->def, 3, window, SHOP_STAT_X, 24);
    return 1;
}

#undef RenderOutput_ClearListFar
#define RenderOutput_ClearListFar ((void (*)(void *))RenderOutput_ClearListFar)

void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);

s32 ItemMenu_SelectItem(void)
{

    struct InventoryMenuState *menu;
    struct BtlUnit *owner;
    s32 result;
    s32 done;
    s32 redraw;
    s32 first;
    s32 nav;
    s32 tab;
    s32 i;
    u16 saved[15];
    struct MenuResult state;

    menu = gMenuWork;
    {
        s32 id = menu->pane_owner[0];

        result = 0;
        UnitGetFar(menu->owner_ids[id]);
    }
#if EDITION_INTERNATIONAL
    UiWindow_UpdateOrCreate((s32 *)&menu->equip_window, 0, 10, 15, 10, 2);
#else
    UiWindow_UpdateOrCreate((s32 *)&menu->equip_window, 0, 10, 13, 10, 2);
#endif
    ItemMenu_PosCategory();

    for (i = 0; i < 32; i++) {
        struct RenderOutput *icon = menu->entry_icons[i];

        if (icon != 0)
            icon->sentinel = 240;
    }

    Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
    UiText_DrawCharacterAtOffsetFar((s32)MsgSwitchCharacterHelp, (s32)menu->status_window, 64, -24);
    UiText_DrawCharacterAtOffsetFar((s32)MsgSwitchCharacterHelp + 3, (s32)menu->status_window, 0, -24);
    ItemMenu_PosCategory();
    WaitFrames(1);
    Menu_SpawnIconEntries(menu, (s32)menu->message_window);

    done = 0;
    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        ItemMenu_PosCategory();
        RenderOutput_RedrawSavedRectFar(menu->status_window);
        owner = UnitGetFar(menu->pane_owner[0]);
        for (i = 0; i <= 14; i++)
            saved[i] = owner->inventory[i];
        menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
        InventoryMenu_SortByListOrder(menu->items, 0);
        Menu_BuildPageResult(&state, 0);
        RenderOutput_RedrawSavedRectFar(menu->equip_window);
        ItemMenu_DrawCategory((s32)menu->equip_window, menu->pane_owner[0], 1);
        WaitFrames(1);
        ItemMenu_DrawIcons(menu->items, 0);
        redraw = 1;
        first = 1;

        while (GameFlag_TestFar(0x150) == 0) {
            if (redraw != 0) {
                RenderOutput_ClearListFar(menu->info_window);
                redraw = 0;
                if (first != 0) {
                    first = 0;
                    Shop_DrawItemPage((s32)menu->status_window, 0, &state);
                }
                ItemMenu_DrawEquipPage((s32)menu->status_window, 0, &state);
            }
            menu->pane_icons[0]->active = 1;
            UiMenu_PositionCursor(96, state.row * 16 + 52);
            WaitFrames(1);

            nav = Menu_HandlePageInput(0, state.entry_count, PAGE_ROWS, &state.row, &state.page);
            if (nav == 1) {
                first = 1;
                redraw = 1;
            }
            if (nav == 0)
                redraw = 1;
            if (nav == -1)
                redraw = 0;

            if (gKeyState & KEY_A) {
                AudioCommand_PlayFar(0x70);
                result = 1;
                done = 1;
                break;
            }
            if (gKeyState & KEY_B) {
                AudioCommand_PlayFar(0x71);
                result = -1;
                done = 1;
                break;
            }
            if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
                AudioCommand_PlayFar(0x6f);
                tab = menu->pane_index[0];
                menu->selected_index_by_owner[menu->owner_ids[tab]] = state.selected_index;
                if (gKeysRepeat & KEY_R)
                    tab++;
                else
                    tab--;
                for (i = 0; i <= 14; i++)
                    owner->inventory[i] = saved[i];
                tab = (tab + menu->party_count) % menu->party_count;
                menu->selected_owner = menu->owner_ids[tab];
                menu->pane_owner[0] = menu->owner_ids[tab];
                menu->pane_index[0] = tab;
                PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->owner_ids[tab]);
                break;
            }
        }
    }

    RenderOutput_ClearListFar(menu->info_window);
    RenderOutput_RedrawSavedRectFar(menu->info_window);
    RenderOutput_ClearListFar(menu->message_window);
    ItemMenu_HideAllIcons();
    RenderOutput_RedrawSavedRectFar(menu->status_window);
    {
        s32 delay = 200;

        delay <<= 4;
        Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, delay);
    }
    return result;
}

s32 Menu_MoveGridCursor(s32 *arg0, s32 *arg1, s32 arg2)
{
    s32 row;
    s32 col;

    col = *arg0;
    row = *arg1;
    switch (arg2) {
    case 0x40:
        row -= 1;
        if (row < 0) {
            row = 5;
        }
        if (row <= 3) {
            if (row == 3) {
                if (col <= 4) {
                    col = 0;
                } else {
                    goto set_one;
                }
            } else if (col > 1) {
set_one:
                col = 1;
            }
            if ((row == 3) && (col == 1)) {
                row = 2;
            }
        }
        break;
    case 0x80:
        row += 1;
        if (row > 5) {
            row = 0;
        }
        if ((row == 3) && (col == 1)) {
            row = 4;
        }
        if (row == 4) {
            goto set_zero;
        }
        break;
    case 0x20:
        col -= 1;
        if (row == 3) {
            col += 1;
        } else if (row > 3) {
            if (col < 0) {
                col = 7;
            }
        } else if (col < 0) {
            col = 1;
        }
        break;
    case 0x10:
        col += 1;
        if (row == 3) {
            col -= 1;
        } else if (row > 3) {
            if (col > 7) {
                goto set_zero;
            }
        } else if (col > 1) {
            goto set_zero;
        }
        break;
set_zero:
        col = 0;
        break;
    }
    *arg0 = col;
    *arg1 = row;
    return (row * 9) + col;
}

#undef ITEM_ID_MASK

s32 Menu_ReservedStatusOne(void)
{
    return 1;
}

void ItemMenu_DrawCategory(s32 window, s32 owner_id, s32 mode)
{
    struct InventoryMenuState *menu = gMenuWork;
    u16 *items;

    UnitGetFar(owner_id);
    ItemMenu_PosCategory();
    ItemMenu_HideAllIcons();
    UiText_DrawCharacterAtOffsetFar((s32)MsgEquipSlotLabels, window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgEquipSlotLabels + 1, window, 0, 32);
    UiText_DrawCharacterAtOffsetFar((s32)MsgEquipSlotLabels + 2, window, 0, 16);
    UiText_DrawCharacterAtOffsetFar((s32)MsgEquipSlotLabels + 3, window, 0, 48);
    items = menu->items;
    ItemMenu_DrawEquippedItemNames(window, items);
    if (mode == 0) {
        WaitFrames(1);
        ItemMenu_DrawIcons(items, 1);
        ItemMenu_ArrangeCategoryItemIcons(items);
    }
}

#if EDITION_INTERNATIONAL
#define NAME_X 8
#else
#define NAME_X 16
#endif

void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items)
{
    s32 i;
    s32 item;

    for (i = 0; i < 15; i++) {
        if (items[i] & INVENTORY_EQUIPPED) {
            item = items[i] & ITEM_ID_MASK;
            switch (Item_GetDirectFar(item)->type) {
            case 1:
                UiText_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, NAME_X, 8);
                break;
            case 2:
                UiText_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, NAME_X, 56);
                break;
            case 3:
                UiText_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, NAME_X, 40);
                break;
            case 4:
                UiText_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, NAME_X, 24);
                break;
            }
        }
    }
}

void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase)
{
    s32 i;
    struct RenderOutput *obj;
    struct RenderOutput **tbl;

    i = 0;
    tbl =
        ((struct InventoryMenuState *)gMenuWork)->entry_icons;
    do {
        obj = *tbl++;
        if (obj != NULL) {
            Menu_PlaceEntryObjectInGrid(obj, i, origin_x, origin_y, phase);
        }
        i += 1;
    } while (i <= 0x1F);
}

void Menu_PlaceEntryObjectInGrid(struct RenderOutput *obj, s32 index,
    s32 origin_x, s32 origin_y, s32 phase) {
    s32 no;

    no = index;
    if (no > 0x1F) {
        no = 0;
    }
    obj->y =
        (no / phase * 0x10) + origin_y;
    obj->x =
        (no % phase * 0x10) + origin_x;
    UiIcon_PrepareObject(obj);
}

void ItemMenu_ArrangeCategoryItemIcons(u16 *items)
{
    struct InventoryMenuState *state;
    struct RenderOutput *icon;
    s32 i;

    state = gMenuWork;
    ItemMenu_PosCategory();
    for (i = 0; i < 15; i++) {
        if (items[i] != 0 && (items[i] & INVENTORY_EQUIPPED) != 0) {
            icon = state->entry_icons[i];
            if (icon != 0) {
                switch (Item_GetDirectFar(items[i] & ITEM_ID_MASK)->type) {
                case 1:
                    icon->x = 216;
                    icon->y = 32;
                    break;
                case 2:
                    icon->x = 216;
                    icon->y = 80;
                    break;
                case 3:
                    icon->x = 216;
                    icon->y = 64;
                    break;
                case 4:
                    icon->x = 216;
                    icon->y = 48;
                    break;
                }
                UiIcon_PrepareObject(icon);
            }
        }
    }
}

void ItemMenu_PosCategory(void)
{
    struct InventoryMenuState *state = gMenuWork;
    s32 x = 248;
    struct RenderOutput **entry = state->entry_icons;
    s32 y = 168;
    s32 remaining = 31;

    do {
        struct RenderOutput *object = *entry++;

        if (object != 0) {
            object->x = x;
            object->y = y;
            UiIcon_PrepareObject(object);
        }
        remaining--;
    } while (remaining >= 0);
}

s32 Menu_CreateEightEntryObjects(s32 resource)
{
    struct RenderOutput **slot;
    struct RenderOutput *obj;
    s32 i;
    struct InventoryMenuState *state;
    s32 param;

    state = gMenuWork;
    i = 0;
    param = 0xA8;
    slot = state->category_icons;
    do {
        obj = RenderOutput_CreateFromResourceFar(2, i, (struct RenderInput *)resource, 0xF8, param);
        i += 1;
        *slot = obj;
        slot += 1;
    } while (i <= 7);
    return 1;
}

void ItemMenu_ApplyFlags(const u8 *flags)
{
    /* FAKEMATCH: the y-field pointer keeps the category flag index as a counter instead of a walking pointer. */
    struct InventoryMenuState *base;
    struct RenderOutput **slot;
    struct RenderOutput *entry;
    s32 index;
    s32 value;
    u16 kind;

    base = gMenuWork;
    ItemMenu_ResetCategory();
    index = 0;
    slot = base->category_icons;
    value = 88;
    do {
        entry = *slot++;
        if (entry != 0 && flags[index] != 0) {
            kind = 8;
            entry->x = kind;
            {
                s16 *y = &entry->y;

                *y = value;
            }
            entry->sentinel = 240;
            UiIcon_PrepareObject(entry);
            value += 16;
        }
        index++;
    } while (index <= 4);
}

void ItemMenu_ResetCategory(void)
{
    struct InventoryMenuState *state = gMenuWork;
    s32 index;

    for (index = 0; index < 5; index++) {
        struct RenderOutput *object = state->category_icons[index];

        if (object != 0) {
            object->x = 248;
            object->y = 168;
            object->sentinel = 240;
            UiIcon_PrepareObject(object);
        }
    }
}

s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled)
{
    struct InventoryMenuState *state = gMenuWork;
    s32 index = 0;

    do {
        if (enabled[index] != 0) {
            s32 kind;
            switch (index) {
            case CHARACTER_DOWN: kind = 16; break;
            case CHARACTER_POISON: kind = 1; break;
            case CHARACTER_VENOM: kind = 2; break;
            case CHARACTER_CURSE: kind = 15; break;
            case CHARACTER_HAUNT: kind = 7; break;
            default: kind = 0; break;
            }
            Resource_LoadByModeIntoSlotFar(8, kind, (u8)state->category_icons[index]->index, 0);
        }
        index++;
    } while (index <= 4);
    return 1;
}

void Item_PrepareUsePalette(void)
{
    Palette_CopyObjectBankToBackground14();
    Palette_LightenBankHighlight(13);
}

void Item_UseNoOpCallback(void)
{
}

#undef RenderOutput_ClearListFar

void UnitCalcFar(s32 unit_id);

#undef RenderOutput_ClearListFar
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)

#define Link_DrawShiftedTilePairFar ((void (*)(void*))Link_DrawShiftedTilePairFar)
#define FourObjectMotion_InitializeBottomRow ((void (*)(s32,s32))FourObjectMotion_InitializeBottomRow)
#define Djinn_DeactivateFar ((s32 (*)(s32,s32,s32))Djinn_DeactivateFar)
#define Trade_AddOfferFar ((s32 (*)(s32,s32,s32))Trade_AddOfferFar)

#define gMenuCtrlWork (*(struct ItemMenuDisplayState * *)&gMenuCtrlWork)

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct ItemMenuDisplayState {
    u16 reserved_00[2];
    u16 busy;
};

struct ItemMenuSavedGraphics {
    u8 reserved_000[0xa8];
    u8 tiles[0x2000];
    u16 palette[64];
    void *reserved_2128;
    s32 variant;
};

struct ItemMenuWork {
    u8 reserved_000[0x1c];
    u8 selection;
    u8 target;
    u8 reserved_01e[0xee];
    s32 window;
    u8 reserved_110[0x64];
    u16 slot;
    u16 target_slot;
    u16 item;
    u8 reserved_17a[10];
    struct ItemMenuSavedGraphics *saved;
    u8 reserved_188[0x80];
    u16 owners[8];
    u8 reserved_218;
    u8 count;
};

s32 Item_Use(s32 slot, s32 owner_id, s32 target_id)
{
    struct InventoryMenuState *work;
    s32 result;
    s32 item_id;
    struct BtlUnit *owner;
    struct ItemDefinition *item;

    owner = UnitGetFar(owner_id);
    work = gMenuWork;
    item_id = 0x1ff & owner->inventory[slot];
    item = Item_GetDirectFar(item_id);
    result = BattleEffect_ApplyToTargets(
        0x3fff & item->action_id,
        owner_id,
        target_id,
        1);
    if (result != -1) {
        item = Item_GetDirectFar(owner->inventory[slot]);
        if (item->use_type == 1) {
            Inventory_RemoveFar(owner_id, slot);
            work->item_count =
                ItemMenu_Collect(owner, work->items, 0);
        }
        if (item->use_type == 4) {
            if (item_id == 0xb8)
                item_id = 0xb9;
            owner->inventory[slot] = item_id;
        }
        result = 0;
    }
    return result;
}

s32 Item_ReturnOne(void)
{
    return 1;
}

s32 BattleEffect_ApplyToTargets(
    s32 effect_id,
    s32 source_id,
    s32 target_id,
    s32 fixed_scale)
{
	void UiWork_PushValueSlotFar(s32, s32);
    struct BtlAct *effect;
    struct InventoryMenuState *runtime;
    struct BtlUnit *target;
    struct BtlUnit *source;
    s32 later_target;
    s32 changed;
    u8 index;
    s16 scale;
    s32 random_adjust;
    s32 random_nonone;
    s32 amount;
    s32 result_code;

    effect = BtlActGet(effect_id);
    runtime = gMenuWork;
    changed = 0;
    result_code = 0;
    later_target = 0;

    if (target_id != 9)
        target = UnitGetFar(target_id);
    else
        target = UnitGetFar(0);

    /* FAKEMATCH: the dead test keeps the range load before the count guard and preserves target-index and power-load registers. */
    if (effect->rng == 0xff && later_target != 0)
        result_code = 3;

    for (index = 0; index < runtime->party_count; index++) {
        if (effect->rng == 0xff) {
            target_id = runtime->owner_ids[index];
            target = UnitGetFar(target_id);
        }

        amount = effect->pow;
        switch (effect->tgt_flg & 0xf) {
        case BATTLE_DAMAGE_HP_HEAL:
            if (fixed_scale == 0) {
                if (effect->damage_class != 4) {
                    s32 stat_offset;

                    source = UnitGetFar(source_id);
                    stat_offset = effect->damage_class;
                    scale = source->elm[stat_offset].pow;
                } else
                    scale = 100;
                amount = BtlRcvCalc(amount, scale, 0x100);
            }

            if (target->hp <= 0) {
                if (later_target == 0)
                    result_code = 2;
            } else {
                if (target->hp == target->max_hp) {
                    if (later_target == 0)
                        result_code = 4;
                } else {
                    target->hp += amount;
                    if (target->hp > target->max_hp) {
                        amount -= target->hp - target->max_hp;
                        target->hp = target->max_hp;
                        if (later_target == 0)
                            result_code = 0;
                    } else if (later_target == 0) {
                        result_code = 1;
                    }
                    UnitRatioFar(target_id);
                    changed = 1;
                    if (effect->rng == 0xff) {
                        later_target = 1;
                        result_code = 3;
                    }
                }
            }
            break;

        case 9:
            random_adjust = (Random16() * 4) >> 16;
            if (random_adjust == 0) {
                random_adjust = -1;
            } else {
                random_nonone = 1 ^ random_adjust;
                random_adjust =
                    (u32)((-random_nonone) | random_nonone) >> 31;
                random_adjust = 1 - random_adjust;
            }

            switch (effect_id & 0x3fff) {
            case 0x104:
                target->base_hp += amount + random_adjust;
                result_code = 0x10;
                changed = 1;
                break;
            case 0x105:
                target->base_ep += amount + random_adjust;
                result_code = 0x11;
                changed = 1;
                break;
            case 0x108:
                target->base_agility += amount + random_adjust;
                result_code = 0x12;
                changed = 1;
                break;
            case 0x109:
                target->base_luck += amount;
                result_code = 0x13;
                changed = 1;
                break;
            case 0x106:
                target->base_attack += amount + random_adjust;
                UiWork_PushValueSlotFar(3, 5);
                result_code = 0x14;
                changed = 1;
                break;
            case 0x107:
                target->base_defense += amount + random_adjust;
                UiWork_PushValueSlotFar(4, 5);
                result_code = 0x15;
                changed = 1;
                break;
            }
            break;

        case BATTLE_DAMAGE_PP_HEAL:
            if (target->ep == target->max_ep) {
                if (later_target == 0)
                    result_code = 7;
            } else {
                target->ep += amount;
                if (target->ep > target->max_ep) {
                    amount -= target->ep - target->max_ep;
                    target->ep = target->max_ep;
                    if (later_target == 0)
                        result_code = 5;
                } else if (later_target == 0) {
                    result_code = 6;
                }
                UnitRatioFar(target_id);
                changed = 1;
                if (effect->rng == 0xff) {
                    later_target = 1;
                    result_code = 8;
                }
            }
            break;

        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 10:
            break;
        }

        switch (effect->efx) {
        case 1:
            if (target->hp <= 0 || target->hp == target->max_hp) {
                if (later_target == 0)
                    result_code = 2;
            } else {
                target->hp += amount;
                if (target->hp > target->max_hp) {
                    target->hp = target->max_hp;
                    if (later_target == 0)
                        result_code = 0;
                } else if (later_target == 0) {
                    result_code = 1;
                }
                UnitRatioFar(target_id);
                changed = 1;
            }
            break;

        case 2:
            if (target->ep == target->max_ep) {
                if (later_target == 0)
                    result_code = 7;
            } else {
                target->ep += amount;
                if (target->ep > target->max_ep) {
                    target->ep = target->max_ep;
                    if (later_target == 0)
                        result_code = 5;
                } else if (later_target == 0) {
                    result_code = 6;
                }
                UnitRatioFar(target_id);
                changed = 1;
            }
            break;

        case EFX_REVIVE_FULL:
            if (target->hp == 0) {
                target->hp = target->max_hp;
                UnitRatioFar(target_id);
                changed = 1;
                if (later_target == 0)
                    result_code = 0xc;
            } else if (later_target == 0) {
                result_code = 0xd;
            }
            break;

        case EFX_REVIVE_HALF:
            if (target->hp == 0) {
                target->hp = target->max_hp / 2;
                UnitRatioFar(target_id);
                if (later_target == 0)
                    result_code = 0xc;
            } else if (later_target == 0) {
                result_code = 0xd;
            }
            break;

        case EFX_REVIVE_80:
            if (target->hp == 0) {
                target->hp = target->max_hp * 7 / 10;
                UnitRatioFar(target_id);
                if (later_target == 0)
                    result_code = 0xc;
            } else if (later_target == 0) {
                result_code = 0xd;
            }
            break;

        case EFX_CURE_POISON:
            if (target->doku != 0) {
                target->doku = 0;
                changed = 1;
                if (later_target == 0)
                    result_code = 0xa;
            } else if (later_target == 0) {
                result_code = 0xb;
            }
            break;
        }

        if (effect->rng != 0xff)
            break;
    }

    if (changed == 0) {
        runtime->message_offset = result_code;
        return -1;
    }

    for (index = 0; index < runtime->party_count; index++)
        UnitCalcFar(runtime->owner_ids[index]);
    runtime->message_offset = result_code;
    return 0;
}

void Item_PlayUseAnimation(u32 item)
{
    Ability_PlayUseAnimation(Item_GetDirectFar(item)->action_id & 0x3fff);
}

void Ability_PlayUseAnimation(s32 action)
{
    s32 animation_type;
    u32 target_type;
    struct BtlAct *ability;

    ability = BtlActGet(action);
    animation_type = 0xf & ability->tgt_flg;
    switch (animation_type) {
    case 1:
        Audio_PlayCueReturnOne(0x7e);

    case 11:
        Audio_PlayCueReturnOne(0x7e);
        return;
    default:
        target_type = ability->efx - 1;
        switch (target_type) {
        case 4:
            Audio_PlayCueReturnOne(0x52);
            return;
        case 2:
            Audio_PlayCueReturnOne(0x54);
            return;
        default:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
            Audio_PlayCueReturnOne(0x5b);

        case 0:
        case 1:
        case 30:
        case 31:
            return;
        }
        break;
    }
}

s32 Menu_ReservedStatusZero(void)
{
    return 0;
}

s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1)
{
    return (arg0 + arg1) % arg1;
}

s32 Menu_SetFirstObjectRowCoordinates(s32 arg0)
{
    u8 *current;
    s32 value;
    s32 count;
    current = ((u8 *)gMenuWork) + 0x134;
    arg0 += 0x3D;
    value = 0x20;
    count = 3;
    do {
        count--;
        *(s16 *)current = value;
        *(s16 *)(current + 0x10) = arg0;
        value += 0x38;
        current += 2;
    } while (count >= 0);
    return arg0;
}

s32 ItemMenu_Run(void)
{
    struct ItemMenuWork *menu;
    u32 old_mode;
    s32 state;
    struct ItemMenuSavedGraphics *saved;
    s32 zero;

    menu = Runtime_AllocateHeapBlock(55, 0xa70);
    old_mode = gGameState.message_speed;
    gGameState.message_speed = 2;
    state = gMenuCtrlWork->busy = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);
    saved = (struct ItemMenuSavedGraphics *)Runtime_BumpAllocateAlternatePool(0x2130);
    menu->saved = saved;
    saved->reserved_2128 = 0;
    saved->variant = 0;
    if (GameFlag_TestFar(0x16e)) {
        if (!GameFlag_TestFar(0x16f)) {
            if (!GameFlag_TestFar(0x171))
                saved->variant = state;
            else
                saved->variant = 14;
        } else {
            if (!GameFlag_TestFar(0x171))
                saved->variant = 27;
            else
                saved->variant = 28;
        }
    }
    Scheduler_EnableOverlayCallbacksWithFlags();
    UiWork_SetAltFlagAndClearTableFar(1);
    Link_DrawShiftedTilePairFar((void *)0x06002500);
    menu->count = Party_ListActiveOwnersFar(menu->owners);
    Resource_LoadPairedBlocksIfAvailable();
    ItemMenu_Init(0, 3, 0, 7);
    Menu_SetFirstObjectRowCoordinates(0);
    Palette_LightenBankHighlight(14);
    menu->window = UiWindow_CreateFar(13, 0, 17, 5, 2);
    menu->item = 255;
    zero = 0;
    menu->selection = zero;
    menu->target = zero;
    menu->slot = 0;
    menu->target_slot = 0;
    FourObjectMotion_InitializeBottomRow(menu->window, 0);
    DjinnMenu_RunScreen();
    FourObjectMotion_ClearSlotsAndScheduleAlt();
    Menu_ResetTwoResourceEntries();
    WaitFrames(1);
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    gMenuCtrlWork->busy = 0;
    UiWindow_MarkVisibleTileAttributesFar();
    UiWork_SetAltFlagAndClearTableFar(0);
    Iwram_CopyWords((void *)0x06004000, saved->tiles, 0x2000);
    Iwram_CopyWords((void *)0x05000080, saved->palette, 128);
    WaitFrames(1);
    Scheduler_DisableOverlayCallbacksWithFlags();
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    MemFree(menu->saved);
    Runtime_ReleaseHeapBlock(55);
    Event_ClearInvalidPackedValuesFar();
    gGameState.message_speed = old_mode;
    return 1;
}

s32 DjinnMenu_RunScreen(void)
{
    struct DjinnMenuWork *menu;
    struct RenderOutput *cursor;
    s32 result;
    s32 state;
    s32 ret;
    s32 done;
    s32 i;

    done = 0;
    result = 0;
    ret = 0;
    menu = (struct DjinnMenuWork *)gMenuWork;
    cursor = menu->cursor;
    cursor->active = 13;
    cursor->anim_step = 0;
    Menu_OpenBackdropScreen();
    WaitFrames(1);
    state = 2;
    do {
        switch (state) {
        case 0:
            if (result < 0) {
                ret = -1;
                done = 1;
            }
            state = 2;
            break;
        case 1:
            break;
        case 2:
            Menu_SetFirstObjectRowCoordinates(0);
            FourObjectMotion_SetSlotPosition(1, 0, 200, 0);
            result = DjinnMenu_SelectDjinn(0);
            state = 15;
            if (result == 10)
                break;
            state = 0;
            if (result < 0)
                break;
            menu->selection[1] = menu->selected_column[0];
            state = 10;
            if (result == 7)
                break;
            state = 3;
            break;
        case 10:
            menu->owner = menu->owners[menu->selected_column[0]];
            menu->shown_owner[0] = menu->owners[menu->selected_column[0]];
            result = DjinnMenu_ShowOwnerStatus();
            if (result == -2)
                done = 1;
            state = 2;
            break;
        case 15:
            result = DjinnMenu_ShowHelp();
            if (result == -2)
                done = 1;
            state = 2;
            break;
        case 8:
            state = 0;
            if (menu->pair_count == 0)
                break;
            result = DjinnMenu_SelectDjinn(1);
            if (result == -2)
                done = 1;
            state = 4;
            if (result < 0)
                break;
            state = 9;
            break;
        case 3:
            DjinnMenu_DrawElementList(&menu->backdrop->list);
            Menu_SetFirstObjectRowCoordinates(-8);
            menu->owner = menu->owners[menu->selected_column[0]];
            menu->shown_owner[0] = menu->owners[menu->selected_column[0]];
            FourObjectMotion_SetSlotPosition(0, menu->selected_column[0] * 56 + 48, 54, 0);
            result = DjinnMenu_SelectDjinn(1);
            for (i = 0; i < menu->owner_count; i++)
                menu->owner_y[i] += 8;
            if (result == -2)
                done = 1;
            if (result < 0) {
                state = 2;
                break;
            }
            if (result == 3 || result == 4 || result == 8 || result == 9)
                menu->shown_owner[1] = menu->owners[menu->selected_column[1]];
            if (result < 0)
                state = 2;
            else if (result == 1)
                state = 5;
            else if (result == 2)
                state = 6;
            else if (result == 3) {
                menu->pending = 2;
                state = 7;
            } else if (result == 4) {
                menu->pending = 2;
                state = 9;
            } else if (result == 5)
                state = 11;
            else if (result == 6)
                state = 12;
            else if (result == 8) {
                menu->pending = 2;
                state = 13;
            } else if (result == 9) {
                menu->pending = 2;
                state = 14;
            }
            break;
        case 7:
            result = OwnerAction_RunCompareLoop(1);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 13:
            AudioCommand_PlayFar(126);
            result = Djinn_TransferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0], menu->shown_owner[1]);
            UnitCalcFar(menu->shown_owner[0]);
            UnitCalcFar(menu->shown_owner[1]);
            menu->cursor->active = 13;
            RenderOutput_ClearListFar(menu->list_window);
            Menu_ComputeEntryValues(menu->backdrop);
            {
                s32 column;
                s32 found;
                u8 entry;

                column = (u16)((u32)menu->selection[1] % DJINN_LIST_ROWS);
                found = 0;
                entry = menu->pair_entries[0];
                for (i = 0; i < menu->backdrop->list.counts[column]; i++) {
                    if (entry == (u8)menu->backdrop->list.ids[column][i]) {
                        found = i;
                        break;
                    }
                }
                /* FAKEMATCH: needed to match: a one-pass block builds the selection address before found * 10 */
                do {
                    menu->selection[0] = column + found * DJINN_LIST_ROWS;
                } while (0);
                menu->cursor->active = 1;
            }
            state = 0;
            break;
        case 6:
            result = OwnerAction_RunCompareLoop(2);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 12:
            AudioCommand_PlayFar(175);
            Djinn_DeactivateFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            result = Trade_AddOfferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            UnitCalcFar(menu->shown_owner[0]);
            menu->cursor->active = 13;
            RenderOutput_ClearListFar(menu->list_window);
            menu->cursor->active = 1;
            state = 2;
            break;
        case 9:
            result = OwnerAction_RunCompareLoop(0);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 14:
            AudioCommand_PlayFar(126);
            Djinn_TransferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0], menu->shown_owner[1]);
            result = Djinn_TransferFar(menu->shown_owner[1], menu->pair_element[1], menu->pair_index[1], menu->shown_owner[0]);
            UnitCalcFar(menu->shown_owner[0]);
            UnitCalcFar(menu->shown_owner[1]);
            Menu_ComputeEntryValues(menu->backdrop);
            {
                s32 column;
                s32 found;
                u8 entry;

                column = (u16)((u32)menu->selection[1] % DJINN_LIST_ROWS);
                found = 0;
                entry = menu->pair_entries[0];
                for (i = 0; i < menu->backdrop->list.counts[column]; i++) {
                    if (entry == (u8)menu->backdrop->list.ids[column][i]) {
                        found = i;
                        break;
                    }
                }
                /* FAKEMATCH: needed to match: a one-pass block builds the selection address before found * 10 */
                do {
                    menu->selection[0] = column + found * DJINN_LIST_ROWS;
                } while (0);
                menu->cursor->active = 1;
            }
            state = 2;
            break;
        case 4:
            if (result == -1) {
                ret = result;
                state = 2;
            } else if (menu->pending & 1)
                state = 8;
            else if (menu->pending & 2)
                state = 7;
            break;
        case 5:
            result = OwnerAction_RunCompareLoop(3);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 11:
            AudioCommand_PlayFar(139);
            Djinn_ActivateFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            result = Trade_RemoveOfferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            UnitCalcFar(menu->shown_owner[0]);
            menu->cursor->active = 13;
            RenderOutput_ClearListFar(menu->list_window);
            menu->cursor->active = 1;
            state = 2;
            break;
        default:
            done = 1;
            break;
        }
    } while (done == 0);
    return ret;
}

#undef FourObjectMotion_InitializeTopRow

//--------------------------------------------------------------------------

// adjust_palette_bank.c

// メニュー：BG パレットの明るさを変えた写しを作る

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// BG パレット 15・5・7 の 16 色の RGB に d を足し（0〜31 に収める）、
// それぞれ 1 つ前のパレット 14・4・6 に書く。7 は d でなく -12 を足す
//--------------------------------------------------------------------------
void Graphics_AdjustPaletteBank(s32 d)
{
	s32 n;
	s32 bank;
	s32 col;
	s32 mask;
	s32 idx;

	bank = 15;
	n = 0;
	mask = 31;
	do
	{
		for (col = 0; col <= 15; col++)
		{
			u32 color;
			s32 b;
			s32 g;
			s32 r;
			s32 v;

			idx = bank * 16 + col;
			color = ((u16 *)0x05000000)[idx];
			b = (color >> 10) & mask;
			g = (color >> 5) & mask;
			r = color & mask;
			b += d;
			g += d;
			r += d;
			if (b > 31)
				b = 31;
			if (g > 31)
				g = 31;
			if (r > 31)
				r = 31;
			if (b < 0)
				b = 0;
			if (g < 0)
				g = 0;
			if (r < 0)
				r = 0;
			v = b << 10;
			v |= g << 5;
			v |= r;
			((s16 *)0x04FFFFE0)[idx] = v; // 16 色前（1 つ前のパレット）
		}
		if (n == 0)
		{
			bank = 5;
		}
		else
		{
			bank = 7;
			d = -12;
		}
		n++;
	} while (n <= 2);
}

#define UiWindow_DrawDividerLineFar ((int (*)())UiWindow_DrawDividerLineFar)
#define UiWindow_SetTilemapEntryFar ((int (*)())UiWindow_SetTilemapEntryFar)
#define UiWork_SetParamNibbleFar ((int (*)())UiWork_SetParamNibbleFar)

#define Runtime_GetLowTableAddress ((void * (*)(void))Runtime_GetLowTableAddress)
#define Djinn_ListOwnerEntries ((s16 (*)(void*,s32,s32))Djinn_ListOwnerEntries)

#define FRAME_TILE 150

#define ACTION_MASK 0x3fff
#define FLAG_FIRST 0x8000
#define FLAG_SECOND 0x4000

#undef FIELD_AT_OFFSET
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Menu_OpenBackdropScreen(void)
{
    struct DjinnMenuWork *work = gMenuWork;
    struct DjinnMenuBackdrop *backdrop = work->backdrop;

    UiWindow_UpdateOrCreate(&work->list_window, 0, 5, 30, 15, 2);
    WaitFrames(1);
    Iwram_CopyWords(backdrop->tiles, BG_CHAR_BLOCK(1), 0x2000);
    Iwram_CopyWords(backdrop->palette, (void *)&BG_PLTT_COLOR(4, 0), 128);
    Iwram_FillWords(BG_CHAR_BLOCK(1), 0x2000, 0x33333333);
    Iwram_FillWords((void *)&BG_PLTT_COLOR(4, 0), 128, 0x55555555);
    Graphics_ExpandVramTilesByColorTableFar(BG_CHAR_BLOCK(1) + 0x1000);
    Iwram_CopyWords(BG_CHAR_BLOCK(1) + FRAME_TILE * TILE_SIZE_4BPP,
              Menu_BackdropFrameTile, TILE_SIZE_4BPP);
    Dma_Set(Runtime_GetLowTableAddress(), (void *)&BG_PLTT_COLOR(5, 0), 0x80000010, REG_DMA3);
    BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
    Dma_Set((void *)&BG_PLTT_COLOR(15, 0), (void *)&BG_PLTT_COLOR(7, 0), 0x80000010, REG_DMA3);
    Graphics_AdjustPaletteBank(8);
    BG_PLTT_COLOR(7, 4) = BG_PLTT_COLOR(15, 4);
    BG_PLTT_COLOR(6, 4) = BG_PLTT_COLOR(15, 4);
    /* FAKEMATCH: the list drawing returns nothing, but called as a statement its argument is loaded before the last palette store instead of after. */
    return ((s32 (*)(struct DjinnListTable *))DjinnMenu_DrawElementList)(&backdrop->list);
}

s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b,
                          u16 *out, s32 *first_count, s32 *second_count)
{
    s32 i;
    s32 j;
    s32 total;
    s32 first;
    s32 second;

    total = 0;
    first = 0;
    second = 0;
    for (i = 0; i < 32 && b[i].encoded_action != 0; i++) {
        out[total] = b[i].encoded_action & ACTION_MASK;
        total++;
        for (j = 0; j < 32; j++) {
            if (((b[i].encoded_action ^ a[j].encoded_action) & ACTION_MASK) == 0)
                break;
        }
        if (j == 32) {
            first++;
            out[total - 1] |= FLAG_FIRST;
        }
    }
    for (i = 0; i < 32 && a[i].encoded_action != 0; i++) {
        for (j = 0; j < 32; j++) {
            if (((a[i].encoded_action ^ b[j].encoded_action) & ACTION_MASK) == 0)
                break;
        }
        if (j == 32) {
            second++;
            out[total] = (a[i].encoded_action & ACTION_MASK) | FLAG_SECOND;
            total++;
        }
    }
    *first_count = first;
    *second_count = second;
    return total;
}

s32 Menu_ComputeEntryValues(void *tbl)
{
    void *state;
    s32 i;
    void *p;
    u16 *src;
    s8 *dst;
    s16 v;
    s32 cnt;

    state = gMenuWork;
    i = 0;
    if (i < FIELD_AT_OFFSET(state, u8, 0x219)) {
        dst = (s8 *)tbl + 0xA0;
        src = (u16 *)((u8 *)state + 0x208);
        p = tbl;
        do {
            v = Djinn_ListOwnerEntries(p, *src, -1);
            cnt = FIELD_AT_OFFSET(state, u8, 0x219);
            i += 1;
            *dst = v;
            src += 1;
            dst += 1;
            p = (u8 *)p + 0x14;
        } while (i < cnt);
    }
}

#define ELEMENT_TILE 0x5001
#define ENTRY list[i]

void DjinnMenu_DrawElementList(struct DjinnListTable *tbl)
{
    struct DjinnMenuWork *menu;
    s32 i;
    s32 row;
    s32 element;
    struct UiWork *ui;
    s32 line;
    s32 usable;
    u16 *list;

    menu = gMenuWork;
    ui = *(struct UiWork **)((u8 *)&gMenuWork + (HEAP_SLOT_WINDOW - HEAP_SLOT_MENU) * sizeof(void *));
    ui->menu_busy = 1;
    i = 0;
    if (menu->owner_count != 0) {
        do {
            tbl->counts[i] = Djinn_ListOwnerEntries(tbl->ids[i], menu->owners[i], -1);
            i++;
        } while (i < menu->owner_count);
    }
    RenderOutput_RedrawSavedRectFar(menu->list_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgUnleashEffect, menu->list_window, 0, 80);
    for (row = 0; row < menu->owner_count; row++) {
        list = tbl->ids[row];
        line = 0;
        for (element = 0; element < DJINN_ELEMENTS; element++) {
            for (i = 0; i < tbl->counts[row]; i++) {
                if (element != (ENTRY & DJINN_ENTRY_ELEMENT) >> 5)
                    continue;
                if ((ENTRY & DJINN_ENTRY_IS_SET) == 0)
                    UiWork_SetParamNibbleFar(2);
                usable = 0;
                if (Trade_CanOfferDjinnFar((ENTRY & DJINN_ENTRY_OWNER) >> 8,
                        (ENTRY & DJINN_ENTRY_ELEMENT) >> 5, ENTRY & DJINN_ENTRY_INDEX)
                    || Djinn_IsActiveFar((ENTRY & DJINN_ENTRY_OWNER) >> 8,
                        (ENTRY & DJINN_ENTRY_ELEMENT) >> 5, ENTRY & DJINN_ENTRY_INDEX))
                    usable = 1;
                if (!usable)
                    UiWork_SetParamNibbleFar(4);
                UiWindow_SetTilemapEntryFar(menu->list_window,
                    ((ENTRY & DJINN_ENTRY_ELEMENT) >> 5) + ELEMENT_TILE,
                    row * 7 + 1, line + 2, 0);
                UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnName
                    + ((ENTRY & DJINN_ENTRY_ELEMENT) >> 5) * DJINN_PER_ELEMENT
                    + (ENTRY & DJINN_ENTRY_INDEX),
                    menu->list_window, row * 56 + 16, line * 8 + 16);
                line++;
                UiWork_SetParamNibbleFar(15);
            }
        }
    }
    UiWindow_DrawDividerLineFar(menu->list_window, 0, 10, 28, 10);
    (*(struct UiWork **)((u8 *)&gMenuWork + (HEAP_SLOT_WINDOW - HEAP_SLOT_MENU) * sizeof(void *)))->dirty = 1;
    ui->menu_busy = 0;
}

#undef UiWindow_DrawDividerLineFar
#undef UiWindow_SetTilemapEntryFar
#undef UiWork_SetParamNibbleFar

#undef gWindowWork

void RenderOutput_PrepareForRedrawFar(s32 window);

s32 Menu_DrawAtWindowOffset(void *win, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    UiWindow_SetPaletteBitRectFar(((struct UiWindow *)win)->x + arg1 + 1, ((struct UiWindow *)win)->y + arg2 + 1, arg3, arg4, arg5);
}

void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette)
{
    u8 *base;
    u16 *p;
    s32 col;
    u32 tile;
    u32 masked;

    base = gWindowWork[0];
    palette <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29)
        width = 30 - x;
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (width > 0 && height > 0) {
        do {
            p = &((u16 (*)[32])base)[y][x];
            for (col = width; col != 0; col--) {
                tile = *p;
                if (((tile >> 12) & 15) == 15) {
                    masked = tile & 0xffff0fff;
                    *p = masked | palette;
                }
                p++;
            }
            UiRender_MarkRow((struct UiRenderWork *)base, y);
            height--;
            y++;
        } while (height != 0);
    }
}

void UiWindow_ApplyRectAtObjectOrigin(void *obj, s32 x, s32 y, s32 width, s32 height, s32 palette)
{
    UiWindow_SetRectPalette(((struct UiWindow *)obj)->x + x + 1,
        ((struct UiWindow *)obj)->y + y + 1, width, height, palette);
}

s32 DjinnMenu_ShowHelp(void)
{
	void UiWork_FinalizeFar(s32 window, s32 mode);
    s32 win_b;
    s32 win_a;
    struct DjinnMenuWork *menu;
    void **slot_cells;
    struct UiRenderWork *work;
    s32 result;
    s32 previous;
    s32 selection;
    s32 list;
    struct UiChannelSlot *channel;
    s32 message;
    s32 cnt;
    s32 list_message;

    /* FAKEMATCH: needed to match: the menu/render heap-cell slice avoids four bytes and changed literal order from separate named cells. */
    slot_cells = (void **)&gMenuWork;
    menu = slot_cells[0];
    work = slot_cells[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU];
    result = 0;
    previous = 0;
    selection = 0;
    RenderOutput_ClearListFar(menu->list_window);
    WaitFrames(1);
    RenderOutput_PrepareForRedrawFar(menu->help_window);
    message = (s32)MsgDjinnInfoPrompt;
    UiText_DrawCharacterAtOffsetFar(message, menu->help_window, 0, 0);
    message++;
    UiText_DrawCharacterAtOffsetFar(message, menu->help_window, 0, 16);
    UiWindow_SetRectPalette(1, 1, 11, 3, 6);
    UiWindow_ApplyRectAtObjectOrigin((void *)menu->list_window, 0, 0, 28, 10, 6);
    list = UiWindow_CreateFar(0, 9, 8, 10, 6);
    win_b = UiWindow_CreateFar(8, 12, 22, 7, 2);
    win_a = UiWindow_CreateFar(8, 9, 22, 3, 2);
    UiWindow_MarkVisibleTileAttributesFar();
    cnt = 0;
    list_message = (s32)MsgDjinnInfoTopic;
    do {
        UiText_DrawCharacterAtOffsetFar(cnt + list_message, list, 0, cnt * 8);
        cnt++;
    } while (cnt <= 6);
    do {
        RenderOutput_PrepareForRedrawFar(win_a);
        UiText_DrawResourceFar(selection + (s32)MsgDjinnInfoTopic, win_a, 0, 0);
        channel = (struct UiChannelSlot *)UiText_OpenEntryMessageFar(win_b, selection + (s32)MsgDjinnInfoText);
        Menu_DrawAtWindowOffset((void *)list, 0, previous, 6, 1, 15);
        Menu_DrawAtWindowOffset((void *)list, 0, selection, 6, 1, 14);
        previous = selection;
        for (;;) {
            UiMenu_PositionCursor(-12, (((struct UiWindow *)list)->y + selection) * 8 + 8);
            WaitFrames(1);
            if (gKeysRepeat & (KEY_RIGHT | KEY_DOWN)) {
                selection++;
                selection = Menu_GetModuloOfSum(selection, 7);
                AudioCommand_PlayFar(111);
                break;
            } else if (gKeysRepeat & (KEY_LEFT | KEY_UP)) {
                selection--;
                selection = Menu_GetModuloOfSum(selection, 7);
                AudioCommand_PlayFar(111);
                break;
            } else if (gKeyState & KEY_START) {
                AudioCommand_PlayFar(113);
                result = -2;
                break;
            } else if (gKeyState & (KEY_B | KEY_SELECT)) {
                AudioCommand_PlayFar(113);
                result = -1;
                break;
            } else if (gKeyState & KEY_A) {
                if (UiWork_IsCompleteFar()) {
                    selection++;
                    selection = Menu_GetModuloOfSum(selection, 7);
                    AudioCommand_PlayFar(112);
                    break;
                } else {
                    AudioCommand_PlayFar(111);
                }
            }
        }
        if (work->glyph_resource != 99) {
            Resource_ResetEntry(work->glyph_resource);
            work->glyph_resource = 99;
        }
        ((struct UiRenderWork *)gWindowWork[0])->page_arrow_shown = 0;
        RenderOutput_PrepareForRedrawFar(win_b);
        {
            struct UiWindow *window = channel->work;

            window->frame = 0;
            window->duration = 0;
            window->state = 0;
        }
        channel->work = NULL;
    } while (result == 0);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
    RenderOutput_ClearListFar(win_a);
    RenderOutput_ClearListFar(win_b);
    WaitFrames(1);
    UiWork_FinalizeFar(win_a, 1);
    UiWork_FinalizeFar(list, 1);
    UiWork_FinalizeFar(win_b, 1);
    UiWindow_MarkVisibleTileAttributesFar();
    if (result == -2) {
        RenderOutput_PrepareForRedrawFar(menu->help_window);
        RenderOutput_PrepareForRedrawFar(menu->list_window);
        RenderOutput_PrepareForRedrawFar(*(s32 *)(menu->unknown_000 + 0x10));
        ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
    }
    Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
    return result;
}

#undef Djinn_DeactivateFar
#undef Djinn_ListOwnerEntries
#undef RenderOutput_ClearListFar
#undef RenderOutput_RedrawSavedRectFar
#undef Trade_AddOfferFar
#undef UiText_DrawNumberInWindowFar
#undef UiWindow_ClearInteriorTilesFar

#define gWindowWork (*(struct UiWork * *)&gWindowWork)

#define UiIcon_DrawVariantWithTileOffset ((void (*)(s32,s32,s32,s32))UiIcon_DrawVariantWithTileOffset)
#define UiIcon_CreateWithLoadedResource ((void (*)(s32,s32,s32,s32))UiIcon_CreateWithLoadedResource)
#define SideObject_CreateFar ((void (*)(s32,s32,s32,s32,s32,s32))SideObject_CreateFar)
#define Djinn_DeactivateFar ((void (*)(s32,s32,s32))Djinn_DeactivateFar)
#define OwnerAction_DiffSlots ((s8 (*)(void*,void*,u16*,s32*,s32*))OwnerAction_DiffSlots)

#define DJINN_ENTRY_SET (-0x8000)

void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
#define UiWindow_SetTilemapEntryFar ((void (*)(s32 window, s32 tile, s32 x, s32 y, s32 palette))UiWindow_SetTilemapEntryFar)
void UiWork_SetParamNibbleFar(s32 value);
#define UiWindow_DrawDividerLineFar ((void (*)(s32 window, s32 x0, s32 y0, s32 x1, s32 y1))UiWindow_DrawDividerLineFar)
void Djinn_ActivateFar(s32 owner, s32 element, s32 index);

#define DJINN_OWNER(v) (((v) & 0xf00) >> 8)
#define DJINN_ELEMENT(v) (((v) & 0xe0) >> 5)
#define DJINN_NUMBER(v) ((v) & 0x1f)
#if EDITION_INTERNATIONAL
#define CURSOR_WIDTH 6
#else
#define CURSOR_WIDTH 5
#endif
#undef Djinn_DeactivateFar
#define Djinn_ActivateFar ((s32 (*)(s32, s32, s32))Djinn_ActivateFar)
s32 Trade_RemoveOfferFar(s32 owner, s32 element, s32 number);
void UiWork_PushValueSlotFar(s32 value, s32 kind);
struct UiWindow *UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, u32 flags);
#define UiWindow_ApplyRectAtObjectOrigin ((void (*)(s32 window, s32 x, s32 y, s32 width, s32 height, s32 kind))UiWindow_ApplyRectAtObjectOrigin)
s32 Djinn_MarkBalancedEntries(s8 *balanced, s32 self);
void FourObjectMotion_SetSlotPhase(s32 slot, s32 phase);
s32 FourObjectMotion_ReplaceSlot(s32 slot, s32 element, s32 kind);

u32 __umodsi3(u32, u32);

s32 DjinnMenu_SelectDjinn(s32 mode)
{
	void UiWork_FinalizeFar(struct UiWindow *work, s32 mode);
    s8 buf[8];
    u8 balanced[16];
    struct DjinnMenuWork *state = gMenuWork;
    struct DjinnMenuBackdrop *lists = state->backdrop;
    struct BtlUnit *owner;
    s32 window;
    s32 redraw = 1;
    u32 refresh;
    s32 i;
    s32 j;
    s32 sel;
    s32 x = (u16)(state->selection[mode] % 10);
    s32 y = (u16)(state->selection[mode] / 10);
    s32 savedY = 0;
    u32 djinn = 0;
    s32 groupMode = 0;
    s32 setAll = 0;
    s32 result;
    s32 message;
    u32 pressed;
    s32 count;
    u32 repeat;
    struct UiWindow *work;
    s32 step;
    s8 *status;

    sel = -1;
    status = buf;
    for (i = 7; i >= 0; i--)
        status[i] = 0;
    if (mode == 0) {
        DjinnMenu_DrawElementList(&lists->list);
        for (i = 0; i < state->owner_count; i++) {
            if (lists->list.counts[i] == 0)
                status[i] = 4;
        }
        savedY = y;
        for (i = 0; i < state->owner_count; i++) {
            if (status[x] == 4) {
                x++;
                x = Menu_GetModuloOfSum(x, state->owner_count);
            }
        }
    } else {
        Djinn_MarkBalancedEntries((s8 *)balanced, state->selected_column[0]);
        for (i = 0; i < state->owner_count; i++) {
            if (i == state->selected_column[0]) {
                buf[i] = 7;
            } else if (balanced[i] != 0) {
                buf[i] = 0;
            } else {
                buf[i] = 3;
                if (lists->list.counts[i] == 0)
                    status[i] = buf[i] | 4;
            }
        }
    }
    if (mode == 1) {
        s32 fromX;
        s32 fromY;
        u16 source_cursor;
        s32 n;
        source_cursor = state->selection[0];
        fromX = (u16)__umodsi3(source_cursor, 10);
        fromY = (u16)__udivsi3(source_cursor, 10);
        Menu_DrawAtWindowOffset(state->list_window, fromX * 7 + 1, fromY + 2, CURSOR_WIDTH, 1, 14);
        UiWindow_ApplyRectAtObjectOrigin(state->list_window, fromX * 7 + 1, 2, CURSOR_WIDTH, 7, 6);
        for (n = 0; n < state->owner_count; n++) {
            if (n == state->selected_column[0]) {
                if (state->pair_entries[0] & 0x8000)
                    message = (s32)&MsgDjinnStandbyAction;
                else
                    message = (s32)&MsgDjinnSetAction;
            } else if (buf[n] & 2) {
                message = (s32)&MsgDjinnGiveAction;
            } else {
                message = (s32)&MsgDjinnTradeAction;
            }
            UiText_DrawCharacterAtOffsetFar(message, state->list_window, n * 56 + 8, 8);
        }
    }
    RenderOutput_RedrawSavedRectFar(state->help_window);
    state->cursor->active = 1;
    for (;;) {
        if (redraw != 0) {
            refresh = redraw;
            redraw = 0;
            sel = -1;
            if (!(buf[x] & 1))
                sel = y;
            window = state->status_window;
            owner = UnitGetFar(state->owners[x]);
            RenderOutput_RedrawSavedRectFar(window);
            UiText_DrawStringAtOffsetFar(owner->name, window, 0, 0);
            UiText_DrawCharacterAtOffsetFar(owner->class_index + (s32)MsgClassName, window, 0, 8);
            UiText_DrawStringAtOffsetFar(DjinnMenu_TextLevel, window, 48, 0);
            UiText_DrawNumberInWindowFar(owner->level, 2, window, 72, 0);
            if (mode == 0)
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnCharacterStatus, window, 0, 16);
            if (sel != -1)
                djinn = lists->list.ids[x][sel];
            RenderOutput_RedrawSavedRectFar(state->help_window);
            if (mode == 1) {
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                UiWindow_SetTilemapEntryFar(state->help_window, DJINN_ELEMENT(state->pair_entries[0]) + 0x5001, 0, 0, 0);
                UiWork_PushValueSlotFar(state->shown_owner[0], 1);
                UiWork_PushValueSlotFar(DJINN_ELEMENT(state->pair_entries[0]) * 20 + DJINN_NUMBER(state->pair_entries[0]) + 300, 4);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel, state->help_window, 8, 0);
#else
                UiWork_PushValueSlotFar(state->shown_owner[0], 1);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel, state->help_window, 0, 0);
                UiWork_PushValueSlotFar(DJINN_ELEMENT(state->pair_entries[0]) * 20 + DJINN_NUMBER(state->pair_entries[0]) + 300, 4);
                UiWindow_SetTilemapEntryFar(state->help_window, DJINN_ELEMENT(state->pair_entries[0]) + 0x5001, 6, 0, 0);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel + 1, state->help_window, 56, 0);
#endif
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel + 2, state->help_window, 0, 8);
            }
            if (sel == -1) {
                FourObjectMotion_SetSlotPosition(mode, 0, 200, 0);
            } else if (mode == 0) {
                if (groupMode) {
                    if (setAll == 0) {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnSetAll, state->help_window, 0, 0);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnAllStandby, state->help_window, 0, 0);
                    }
                    if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                        if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        else
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnRecovering, state->help_window, 0, 16);
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
                    }
                    if (refresh & 2)
                        FourObjectMotion_SetSlotPhase(mode, 0);
                } else {
                    UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnChoose, state->help_window, 0, 0);
                    if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                        if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                            UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnChoose + 3, state->help_window, 0, 16);
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        } else {
                            UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnChoose + 2, state->help_window, 0, 16);
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
                        }
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnChoose + 4, state->help_window, 0, 16);
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
                    }
                    if (refresh & 2)
                        FourObjectMotion_SetSlotPhase(mode, 0);
                }
            } else {
                if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                    if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                    else
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
                    FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 54, 0);
                } else {
                    FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                    FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 54, 1);
                }
                if (refresh & 2)
                    FourObjectMotion_SetSlotPhase(mode, 0);
            }
            RenderOutput_ClearListFar(state->list_window);
            if (sel != -1) {
                UiText_DrawCharacterAtOffsetFar((s32)&MsgUnleashEffect, state->list_window, 0, 80);
#if EDITION_INTERNATIONAL
                UiWindow_ClearInteriorTilesFar(state->list_window, 0, 96, 224, 104);
                UiText_DrawCharacterAtOffsetFar(DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + (s32)MsgDjinnEffect, state->list_window, 0, 96);
#else
                UiText_DrawResourceFar(DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + (s32)MsgDjinnEffect, state->list_window, 0, 96);
#endif
            }
            if (!(buf[x] & 1))
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 14);
            gWindowWork->dirty = 1;
        }
        if (buf[x] & 1)
            UiMenu_PositionCursor(x * 56 - 8, 52);
        else
            UiMenu_PositionCursor(x * 56 - 8, y * 8 + 60);
        WaitFrames(1);
        if (!(gKeysHeld & KEY_R) || (gKeysPressedLatch & 0x100)) {
            if (groupMode)
                redraw = 1;
            groupMode = 0;
            setAll = 0;
        }
        repeat = gKeysRepeat;
        pressed = gKeyState;
        step = lists->tutorial_step;
        if (step != 0) {
            pressed = 0;
            lists->tutorial_timer++;
            repeat = 0;
            switch (step) {
            case 28:
                while (!(gKeyState & KEY_A)) {
                    UiMenu_PositionCursor(150, 26);
                    WaitFrames(1);
                }
                pressed = 2;
                repeat = 2;
                break;
            case 27:
                if (lists->tutorial_timer == 60) {
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialRecovery, 9, 9, 1);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    WaitFrames(1);
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialSetAgain, 9, 9, 1);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    lists->tutorial_timer = 0;
                    BattlePlacement_UpdateTimedEntriesFar();
                    BattlePlacement_UpdateTimedEntriesFar();
                    BattlePlacement_UpdateTimedEntriesFar();
                    Djinn_DeactivateFar(0, 0, 0);
                    Trade_AddOfferFar(0, 0, 0);
                    UnitCalcFar(0);
                    repeat = 2;
                    pressed = 2;
                }
                break;
            case 1:
                if (lists->tutorial_timer == 60) {
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialGive, 9, 9, 1);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 2;
                }
                break;
            case 2:
            case 4:
            case 8:
            case 9:
            case 22:
                if (lists->tutorial_timer == 90) {
                    repeat = 1;
                    pressed = 1;
                    lists->tutorial_timer = 0;
                    lists->tutorial_step++;
                }
                break;
            case 3:
                if (lists->tutorial_timer == 90) {
                    repeat = 16;
                    pressed = 16;
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 4;
                }
                break;
            case 6:
            case 7:
                if (lists->tutorial_timer == 60) {
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialSetAfterGive, 9, 9, 1);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 8;
                }
                break;
            case 13:
                if (lists->tutorial_timer == 40) {
                    lists->tutorial_timer = 0;
                    repeat = 2;
                    pressed = 2;
                    lists->tutorial_step++;
                }
                break;
            case 14:
            case 16:
            case 17:
                if (lists->tutorial_timer == 40) {
                    lists->tutorial_timer = 0;
                    lists->tutorial_step++;
                }
                break;
            case 15:
                if (lists->tutorial_timer == 60) {
                    gGameState.message_speed = 1;
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialPowers, 9, 9, 1);
                    UiMenu_SlideCursor(2, 146);
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    while (!(gKeyState & KEY_A)) {
                        UiMenu_PositionCursor(2, 146);
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    WaitFrames(1);
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialUnleash, 9, 9, 1);
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    while (!(gKeyState & KEY_A)) {
                        UiMenu_PositionCursor(2, 146);
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    WaitFrames(1);
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialDemonstrate, 9, 9, 1);
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    while (!(gKeyState & KEY_A)) {
                        UiMenu_PositionCursor(2, 146);
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    WaitFrames(1);
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 16;
                }
                break;
            case 18:
            case 20:
                if (lists->tutorial_timer == 90) {
                    repeat = 1;
                    pressed = 1;
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 21;
                }
                break;
            case 21:
                if (lists->tutorial_timer == 90) {
                    lists->tutorial_timer = 0;
                    repeat = 32;
                    pressed = 32;
                    lists->tutorial_step = 22;
                }
                break;
            case 24:
                if (lists->tutorial_timer == 60) {
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 25;
                    repeat = 2;
                    pressed = 2;
                }
                break;
            }
        }
        if (mode == 0 && (pressed & 0x100)) {
            if (sel == -1) {
                AudioCommand_PlayFar(114);
                continue;
            }
            i = 0;
            if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                i = 1;
            groupMode = 1;
            gKeysPressedLatch = 0;
            if (i == 0) {
                AudioCommand_PlayFar(114);
                RenderOutput_ClearListFar(state->list_window);
                UiWindow_ClearInteriorTilesFar(state->list_window, 0, 80, 216, 104);
                UiText_DrawResourceFar((s32)&MsgDjinnNowRecovering, state->list_window, 0, 96);
                redraw = 1;
                continue;
            }
            if (djinn & 0x8000) {
                AudioCommand_PlayFar(175);
                Djinn_DeactivateFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
                Trade_AddOfferFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
            } else {
                AudioCommand_PlayFar(139);
                Djinn_ActivateFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
                Trade_RemoveOfferFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
            }
            UnitCalcFar(DJINN_OWNER(djinn));
            DjinnMenu_DrawElementList(&lists->list);
            redraw = 1;
        }
        if (mode == 0 && (pressed & 0x200)) {
            result = 7;
            AudioCommand_PlayFar(112);
            break;
        }
        if ((pressed & 1) || (mode == 1 && (pressed & 0x100))) {
            i = 1;
            if (!(buf[x] & 1)) {
                if (sel == -1) {
                    AudioCommand_PlayFar(114);
                    continue;
                }
                i = 0;
                if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                    i = 1;
            }
            if (i == 0) {
                AudioCommand_PlayFar(114);
                RenderOutput_ClearListFar(state->list_window);
                UiWindow_ClearInteriorTilesFar(state->list_window, 0, 80, 216, 104);
                UiText_DrawResourceFar((s32)&MsgDjinnNowRecovering, state->list_window, 0, 96);
                continue;
            }
            if (mode == 1) {
                if (buf[x] & 1) {
                    if (x == state->selected_column[0]) {
                        if (state->pair_entries[0] & 0x8000)
                            result = 2;
                        else
                            result = 1;
                    } else {
                        result = 3;
                    }
                } else {
                    result = 4;
                }
            } else {
                result = 1;
            }
            AudioCommand_PlayFar(112);
            break;
        }
        if (pressed & 8) {
            AudioCommand_PlayFar(113);
            result = -2;
            break;
        }
        if (pressed & 2) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        if (mode == 0 && (pressed & 4)) {
            if (groupMode) {
                setAll ^= 1;
                if (setAll)
                    AudioCommand_PlayFar(139);
                else
                    AudioCommand_PlayFar(175);
                for (i = 0; i < state->owner_count; i++) {
                    for (j = 0; j < lists->list.counts[i]; j++) {
                        u32 entry = lists->list.ids[i][j];
                        s32 usable;
                        usable = 0;
                        if (Trade_CanOfferDjinnFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry)) || Djinn_IsActiveFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry)))
                            usable = 1;
                        if (usable) {
                            if (setAll) {
                                if (!(entry & 0x8000)) {
                                    Djinn_ActivateFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                    Trade_RemoveOfferFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                    UnitCalcFar(DJINN_OWNER(entry));
                                }
                            } else if (entry & 0x8000) {
                                Djinn_DeactivateFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                Trade_AddOfferFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                UnitCalcFar(DJINN_OWNER(entry));
                            }
                        }
                    }
                }
                DjinnMenu_DrawElementList(&lists->list);
                redraw = 1;
            } else {
                result = 10;
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
                AudioCommand_PlayFar(112);
                break;
            }
        }
        if (repeat & 0x40) {
            AudioCommand_PlayFar(111);
            if (buf[x] & 4)
                continue;
            Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
            if (buf[x] & 4)
                continue;
            if (buf[x] & 1) {
                buf[x] &= ~1;
                y = 0;
            } else if (y == 0 && (buf[x] & 2)) {
                buf[x] |= 1;
                redraw = 2;
                continue;
            }
            y--;
            count = lists->list.counts[x];
            if (count == 0)
                count = 1;
            y = Menu_GetModuloOfSum(y, count);
            savedY = y;
            redraw = 2;
            continue;
        }
        if (repeat & 0x80) {
            AudioCommand_PlayFar(111);
            if (buf[x] & 4)
                continue;
            Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
            y++;
            count = lists->list.counts[x];
            if (count == 0)
                count = 1;
            y = Menu_GetModuloOfSum(y, count);
            if ((buf[x] & 1) && !(buf[x] & 4)) {
                buf[x] &= ~1;
                y = 0;
            } else if (y == 0 && (buf[x] & 2)) {
                buf[x] |= 1;
            }
            savedY = y;
            redraw = 2;
            continue;
        }
        if (repeat & 0x20) {
            AudioCommand_PlayFar(111);
            if (!(buf[x] & 4))
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
            x--;
            x = Menu_GetModuloOfSum(x, state->owner_count);
            if (mode == 0) {
                for (i = 0; i < state->owner_count; i++) {
                    if (buf[x] == 4) {
                        x--;
                        x = Menu_GetModuloOfSum(x, state->owner_count);
                    }
                }
            }
            y = savedY;
            count = lists->list.counts[x];
            if (count == 0)
                count = 1;
            y = Menu_GetModuloOfSum(y, count);
            redraw = 2;
            continue;
        }
        if (repeat & 0x10) {
            AudioCommand_PlayFar(111);
            if (!(buf[x] & 4))
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
            x++;
            x = Menu_GetModuloOfSum(x, state->owner_count);
            if (mode == 0) {
                for (i = 0; i < state->owner_count; i++) {
                    if (buf[x] == 4) {
                        x++;
                        x = Menu_GetModuloOfSum(x, state->owner_count);
                    }
                }
            }
            y = savedY;
            count = lists->list.counts[x];
            if (count == 0)
                count = 1;
            y = Menu_GetModuloOfSum(y, count);
            redraw = 2;
        }
    }
    state->selected_column[mode] = x;
    if (sel != -1) {
        u32 entry;
        state->pair_entries[mode] = entry = lists->list.ids[x][sel];
        state->pair_index[mode] = entry % 32;
        state->pair_element[mode] = DJINN_ELEMENT(entry);
        state->pair_owner[mode] = DJINN_OWNER(entry);
        /* FAKEMATCH: keep entry live through owner extraction and pass result through the boundary; an input alone exchanges the final index shift and table-base copy (2 instructions differ), while no boundary ties the owner mask to entry (8 differ). 2026-10-11: plain, entry dies at the owner and, so reload ties the result to entry instead of the mask (ands r2, r3 for ands r3, r2: differ 8); an input-only barrier leaves a sched2 tie (differ 2); storing the fields in other orders differs 13-1106. */
        asm("" : "+r"(result) : "r"(entry));
    }
    state->selection[mode] = x + y * 10;
    return result;
}
#undef Djinn_ActivateFar

s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element)
{
    struct BtlUnit *state = UnitGetFar(owner);
    s32 count = 0;
    s32 row;
    s32 bit;
    s32 entry;

    if (element == -1) {
        for (row = 0; row < 4; row++) {
            for (bit = 0; bit < 20; bit++) {
                if (state->djinn_active[row] & (1 << bit)) {
                    entry = (row << 5) | bit | DJINN_ENTRY_SET;
                    entry |= owner << 8;
                    out[count++] = entry;
                } else if (state->djinn_available[row] & (1 << bit)) {
                    out[count++] = (row << 5) | bit | (owner << 8);
                }
            }
        }
    } else {
        for (bit = 0; bit < 20; bit++) {
            if (state->djinn_active[element] & (1 << bit))
                out[count++] = (element << 5) | bit | DJINN_ENTRY_SET;
            else if (state->djinn_available[element] & (1 << bit))
                out[count++] = (element << 5) | bit;
        }
    }
    return count;
}

#if EDITION_INTERNATIONAL
#define ALT_PARAM 2
#else
#define ALT_PARAM 4
#endif

s32 Menu_RunPairedEntryAction(s32 mode, s32 param)
{
    s32 sp14;
    struct DjinnMenuWork *menu;

    menu = gMenuWork;
    if (mode == 0) {
        sp14 = mode;
        DjinnMenu_DrawStatPreview(menu->second_window, 0, 0, menu->pair_owner[1], 1, mode, 2, param, 1);
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pair_owner[0], mode, 1, 2, param, mode);
    } else {
        DjinnMenu_DrawStatPreview(menu->second_window, 0, 0, menu->shown_owner[1], 1, 0, ALT_PARAM, param, 1);
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->shown_owner[0], 0, 0, 1, param, 0);
    }
    return 1;
}

#if EDITION_INTERNATIONAL
#define PREVIEW_DIGITS 3
#define PREVIEW_CURRENT_X 48
#define PREVIEW_MAX_X 80
#define PREVIEW_SLASH_X 72
#define PREVIEW_LUCK_X 56
#define PREVIEW_ARROW_X icon_x
#define PREVIEW_PP_X 88
#else
#define PREVIEW_DIGITS 4
#define PREVIEW_CURRENT_X 32
#define PREVIEW_MAX_X 72
#define PREVIEW_SLASH_X 64
#define PREVIEW_LUCK_X 48
#define PREVIEW_ARROW_X (x * 8 + 64)
#define PREVIEW_PP_X 56
#endif
static __inline__ void Unit_Copy(struct BtlUnit *dst, struct BtlUnit *src)
{
    Iwram_CopyWords(dst, src, sizeof(struct BtlUnit));
}

s32 DjinnMenu_DrawStatPreview(s32 window, s32 x, s32 y, s32 owner,
    s32 give, s32 take, s32 mode, s32 page, s32 side)
{
    struct BtlUnit *state;
    struct BtlUnit *saved;
    s32 give_element;
    s32 give_index;
    s32 give_set;
    s32 take_element;
    s32 take_index;
    s32 take_set;
    s32 lost;
    s32 gained;
    u16 actions[48];
    struct DjinnMenuWork *menu;

    state = UnitGetFar(owner);
    menu = gMenuWork;
    give_element = menu->pair_element[give];
    give_index = menu->pair_index[give];
    give_set = menu->pair_entries[give] & 0x8000;
    take_element = menu->pair_element[take];
    take_index = menu->pair_index[take];
    take_set = menu->pair_entries[take] & 0x8000;
    saved = (struct BtlUnit *)MemAlloc(sizeof(struct BtlUnit));
    Unit_Copy(saved, state);

    if (page == 0) {
        if (mode == 3) {
            UiText_DrawNumberInWindowFar(state->max_hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_MAX_X, y * 8 + 56);
            UiText_DrawNumberInWindowFar(state->max_ep, PREVIEW_DIGITS, window, x * 8 + PREVIEW_MAX_X, y * 8 + 64);
            UiText_DrawNumberInWindowFar(state->hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 56);
            UiText_DrawNumberInWindowFar(state->ep, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
            UiText_DrawStringAtOffsetFar(DjinnMenu_TextSlash, window, x * 8 + PREVIEW_SLASH_X, y * 8 + 56);
            UiText_DrawStringAtOffsetFar(DjinnMenu_TextSlash, window, x * 8 + PREVIEW_SLASH_X, y * 8 + 64);
        } else {
            UiText_DrawNumberInWindowFar(state->hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 56);
            UiText_DrawNumberInWindowFar(state->ep, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
        }
        UiText_DrawNumberInWindowFar(state->atk, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 72);
        UiText_DrawNumberInWindowFar(state->def, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 80);
        UiText_DrawNumberInWindowFar(state->agi, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 88);
        UiText_DrawNumberInWindowFar(state->luck, 2, window, x * 8 + PREVIEW_LUCK_X, y * 8 + 96);
    }

    switch (mode) {
    case 0:
        Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
        Djinn_ActivateFar(owner, take_element, take_index & 31);
        break;
    case 1:
        give_index &= 31;
        Djinn_DeactivateFar(owner, give_element, give_index);
        break;
    case 2:
        if (give_set) {
            give_index &= 31;
            Djinn_DeactivateFar(owner, give_element, give_index);
        }
        Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
        if (take_set)
            Djinn_ActivateFar(owner, take_element, take_index & 31);
        break;
    case 4:
        Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
        if (take_set)
            Djinn_ActivateFar(owner, take_element, take_index & 31);
        break;
    }

    UnitCalcFar(owner);
    state = UnitGetFar(owner);

    if (page == 0) {
        s32 label;

        UiText_DrawStringAtOffsetFar(state->name, window, x * 8 + 40, y * 8);
        UiText_DrawStringAtOffsetFar(DjinnMenu_TextLevel, window, x * 8 + 40, y * 8 + 16);
        UiText_DrawNumberInWindowFar(state->level, 2, window, x * 8 + 88, y * 8 + 16);
        label = (s32)MsgPreviewStatLabel;
        UiText_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
        UiText_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
        UiText_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
        UiText_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
        UiText_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
        UiText_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
        UiText_DrawCharacterAtOffsetFar((s32)MsgClassName + saved->class_index, window, x * 8, y * 8 + 32);
    }

    {
        s32 i = 0;

    if (page == 0) {
        s32 column;
#if EDITION_INTERNATIONAL
        s32 icon_x;
#endif
        s32 px;

        if (saved->class_index != state->class_index) {
            px = x * 8;
            UiText_DrawCharacterAtOffsetFar((s32)MsgClassName + state->class_index, window, px, y * 8 + 48);
            UiWindow_SetTilemapEntryFar(window, 0xf296, x + 2, 5, 0);
        }
        column = x;
        if (saved->class_index != state->class_index)
            column += 5;
        for (i = 0; i < 4; i++) {
            UiWindow_SetTilemapEntryFar(window, 0x5001 + i, column + i * 2, y + 5, 0);
            UiWindow_SetTilemapEntryFar(window,
                state->djinn_active_counts[i] + 0xf030,
                column + i * 2 + 1, y + 5, 0);
        }
#if EDITION_INTERNATIONAL
        icon_x = x * 8 + 70;
#endif
        if (state->hp != saved->hp) {
            UiText_DrawNumberInWindowFar(state->hp, 4, window, x * 8 + 72, y * 8 + 56);
            if (state->hp > saved->hp)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 56, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 56, 1);
        }
        if (state->ep != saved->ep) {
            UiText_DrawNumberInWindowFar(state->ep, 4, window, x * 8 + 72, y * 8 + 64);
            if (state->ep > saved->ep)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 64, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 64, 1);
        }
        if (state->atk != saved->atk) {
            UiText_DrawNumberInWindowFar(state->atk, 4, window, x * 8 + 72, y * 8 + 72);
            if (state->atk > saved->atk)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 72, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 72, 1);
        }
        if (state->def != saved->def) {
            UiText_DrawNumberInWindowFar(state->def, 4, window, x * 8 + 72, y * 8 + 80);
            if (state->def > saved->def)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 80, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 80, 1);
        }
        if (state->agi != saved->agi) {
            UiText_DrawNumberInWindowFar(state->agi, 4, window, x * 8 + 72, y * 8 + 88);
            if (state->agi > saved->agi)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 88, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 88, 1);
        }
        if (state->luck != saved->luck) {
            UiText_DrawNumberInWindowFar(state->luck, 2, window, x * 8 + 88, y * 8 + 96);
            if (state->luck > saved->luck)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 96, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 96, 1);
        }
    }
    }

    if (page > 0) {
        s32 rows;
        s32 first;
        s8 count;
        s8 row;
        s8 line;

        rows = 6 - (mode != 3);
        first = rows * (page - 1);
        count = OwnerAction_DiffSlots(saved->action_slots,
            state->action_slots, actions, &gained, &lost);
        line = 0;
        for (row = 0; first < count && row < rows; row++, first++) {
            UiIcon_CreateWithLoadedResource(window, x * 8, (y + line * 2) * 8 + 4,
                actions[first] & 0x3fff);
            if (actions[first] & 0x8000)
                UiWork_SetParamNibbleFar(4);
            else if (actions[first] & 0x4000)
                UiWork_SetParamNibbleFar(2);
            else
                UiWork_SetParamNibbleFar(15);
            UiText_DrawCharacterAtOffsetFar((s32)MsgAbilityName + (actions[first] & 0x3fff), window,
                x * 8 + 16, (y + line * 2) * 8 + 8);
            UiText_DrawNumberAtOffsetFar(BtlActGet(actions[first])->pp_cost, 2, window,
                x * 8 + 88, (y + line * 2) * 8 + 8);
            line++;
        }
        UiWork_SetParamNibbleFar(15);
        UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, x * 8 + PREVIEW_PP_X, y * 8);
        if (mode != 3) {
            s32 lines;

            lines = 0;
            if (gained) {
                UiWork_SetParamNibbleFar(4);
                UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyGained, window, x * 8, y * 8 + 88);
                lines = 1;
            }
            if (lost) {
                UiWork_SetParamNibbleFar(2);
                UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyLost, window, x * 8, (y + lines) * 8 + 88);
                lines++;
            }
            if (lines == 0)
                UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyUnchanged, window, x * 8, y * 8 + 88);
            UiWork_SetParamNibbleFar(15);
            UiWindow_DrawDividerLineFar(window, 0, 11, 13, 11);
        }
        gWindowWork->dirty = 1;
    }

    if (page == 0)
        SideObject_CreateFar(owner, 0, side, window, 0, 0);
    Unit_Copy(state, saved);
    MemFree(saved);
    return 1;
}

#undef UiWindow_SetTilemapEntryFar
#undef UiWindow_DrawDividerLineFar
#undef UiWindow_ApplyRectAtObjectOrigin

#undef FourObjectMotion_InitializeBottomRow

s32 Object_ApplyProjectedPlacementFar(u32 object, u32 *request, u32 *motion, u32 limit);

void FourObjectMotion_InitializeTopRow(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 index;

    for (index = 0; index < 4; index++) {
        struct AnimationObject *object = state->objects[index];

        if (object != NULL) {
            ResourceObject_ReleaseFar(object);
            state->objects[index] = NULL;
        }
    }

    for (index = 0; index < 4; index++) {
        struct AnimationObject *object = ResourceObject_CreateFar(FourObjectMotion_ResourceIds[index]);

        if (object != NULL) {
            AnimationObjects_SelectAnimationFar(object, 2);
        }
        state->objects[index] = object;
        state->positions_x[index] = 0x10;
        state->positions_y[index] = 0x20;
    }
    Scheduler_AddOrUpdateCallback((s32)(FourObjectMotion_UpdateAllPositions), 0xc80);
}

void FourObjectMotion_ClearSlotsAndSchedule(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 index = 0;

    do {
        struct AnimationObject *object = state->objects[index];

        if (object != 0) {
            ResourceObject_ReleaseFar(object);
            state->objects[index] = 0;
        }
        index++;
    } while (index < 4);
    Scheduler_RemoveCallback((u32)FourObjectMotion_UpdateAllPositions);
}

#define FIXED_ONE 0x10000u
#define VERTICAL_ORIGIN 0x01e20000u

void FourObjectMotion_UpdateAllPositions(void)
{
    struct FourObjectMotionState *state = gMenuWork;
    s32 index;
    u32 motion[2];
    u32 request[4];

    index = 0;
    while (1) {
        u32 object = (u32)state->objects[index];

        if (object != 0) {
            u32 vertical = VERTICAL_ORIGIN -
                ((u32)(s32)state->vertical_origins[index] << 16);
            ((struct FieldSprite *)object)->priority = 0;
            motion[0] = FIXED_ONE;
            motion[1] = FIXED_ONE;
            request[0] = (u32)(s32)state->positions_x[index] << 16;
            request[1] = vertical;
            request[2] = ((u32)(s32)state->positions_y[index] << 16) + vertical;
            request[3] = 0;
            Object_ApplyProjectedPlacementFar(object, request, motion, 0x4000);
        }
        index += 1;
        if (index > 3)
            break;
    }
}

s32 Object_ApplyProjectedPlacementFar(u32, u32 *, u32 *, u32);

void FourObjectMotion_UpdateBottomRow(void)
{
    struct FourObjectMotionState *work = gMenuWork;
    s32 i;
    u32 motion[2];
    u32 request[4];

    Link_DrawShiftedTilePairFar((void *)0x06002500);
    i = 0;
    do {
        u32 obj = (u32)work->objects[i];

        if (obj != 0) {
            u32 y = (241u << 17) - ((u32)(s32)work->vertical_origins[i] << 16);
            s32 phase;
            s32 limit;
            /* Ordinary s32/s16 locals, a direct field test, an earlier limit
             * initializer and an inline sign/limit helper all differ 81 in
             * EN (119 native, 118 built), changing the outer high-register
             * allocation as well as this read. */
            /* FAKEMATCH: the row's second read lives in r2, as the native allocates it. 2026-10-11: plain locals take r3 (local-alloc's first free register, REG_ALLOC_ORDER 3,2,1,0), which costs limit its r3 preference and reshuffles the high registers (differ 81); reading py before request[3] = 0 puts it in r2 but emits the ldrsh before the zero store (differ 4 in all six editions). */
            register s32 py asm("r2");

            ((struct FieldSprite *)obj)->priority = 0;
            phase = work->phases[i];
            if (phase < 0) {
                motion[0] = -phase;
                motion[1] = -phase;
            } else {
                motion[0] = phase + (s32)(0x10000 - phase) / 3;
                motion[1] = motion[0];
                work->phases[i] = motion[0];
            }
            request[0] = (u32)(s32)work->positions_x[i] << 16;
            request[1] = y;
            request[2] = ((u32)(s32)work->positions_y[i] << 16) + y;
            request[3] = 0;
            py = work->positions_y[i];
            limit = 0x8000;
            if (py >= 0)
                limit = 0x4000;
            Object_ApplyProjectedPlacementFar(obj, request, motion, limit);
        }
        i++;
    } while (i <= 3);
}

void FourObjectMotion_InitializeBottomRow(void)
{
    struct FourObjectMotionState *state = (struct FourObjectMotionState *)gMenuWork;
    s32 index;

    for (index = 0; index < 4; index++) {
        struct AnimationObject *object = state->objects[index];

        if (object != NULL) {
            ResourceObject_ReleaseFar(object);
            state->objects[index] = NULL;
        }
    }
    for (index = 0; index < 4; index++) {
        struct AnimationObject *object = ResourceObject_CreateFar(FourObjectMotion_ResourceIds[index]);

        if (object != NULL)
            AnimationObjects_SelectAnimationFar(object, 2);
        state->objects[index] = object;
        state->phases[index] = 0x10000;
        state->positions_x[index] = 0x10;
        state->positions_y[index] = 0xc8;
    }
    Scheduler_AddOrUpdateCallback((s32)(FourObjectMotion_UpdateBottomRow), 0xc80);
}

s32 FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative)
{
    struct FourObjectMotionState *state = (struct FourObjectMotionState *)gMenuWork;

    if (state->objects[index] != NULL) {
        state->positions_x[index] = x;
        state->positions_y[index] = negative != 0 ? y | (s32)0xffff8000 : y;
        return 0x23c + index * 2;
    }
    return index;
}

void FourObjectMotion_SetSlotPhase(s32 index, s32 phase)
{
    ((struct FourObjectMotionState *)gMenuWork)->phases[index] = phase;
}

s32 FourObjectMotion_ReplaceSlot(s32 index, s32 kind, s32 value)
{
    struct FourObjectMotionState *state = (struct FourObjectMotionState *)gMenuWork;

    if (state->objects[index] != NULL) {
        ResourceObject_ReleaseFar(state->objects[index]);
        state->objects[index] = NULL;
    }
    {
        struct AnimationObject *object = ResourceObject_CreateFar(FourObjectMotion_ResourceIds[kind]);

        if (object != NULL)
            AnimationObjects_SelectAnimationFar(object, value);
        state->objects[index] = object;
    }
    return 1;
}

void FourObjectMotion_ClearSlotsAndScheduleAlt(void)
{
    struct FourObjectMotionState *state = (struct FourObjectMotionState *)gMenuWork;
    s32 index = 0;

    do {
        struct AnimationObject *object = state->objects[index];

        if (object != NULL) {
            ResourceObject_ReleaseFar(object);
            state->objects[index] = NULL;
        }
        index++;
    } while (index < 4);
    Scheduler_RemoveCallback((u32)(FourObjectMotion_UpdateBottomRow));
}

void Menu_EnableAllItemObjects(void)
{
    struct InventoryMenuState *state = gMenuWork;
    s32 index;

    for (index = 0; index < state->party_count; ++index) {
        AnimationObjects_SelectAnimationFar(state->owner_objects[index], 1);
    }
}
