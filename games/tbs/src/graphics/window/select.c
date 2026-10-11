#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "io_reg.h"
#include "render_input.h"
#include "btltype.h"
#include "game_state.h"
#include "uiflow.h"
#include "text/msg_ids.h"
#include "owner_state.h"
#include "window.h"
#include "global_cells.h"
#include "btlpty.h"
#include "b5_context.h"

struct BtlAct * BtlActGet(s32 action_id);

#define Resource_FixedBlockBTiles ((const u8 *)&Resource_FixedBlockBTiles)
#define UiWindow_Create ((struct RenderInput * (*)(s32, s32, s32, s32, s32))UiWindow_Create)
#define UiText_DrawCharacterAtOffset ((void (*)(s32, struct RenderInput *, s32, s32))UiText_DrawCharacterAtOffset)
#define RenderOutput_RedrawSavedRect ((void (*)(struct RenderInput *))RenderOutput_RedrawSavedRect)

#define Menu_FindShortcutEntries ((void (*)(u32*,u32*,const struct ShortcutListEntry*))Menu_FindShortcutEntries)

#define Object_CollectResources ((s32 (*)(struct ShortcutListEntry*))Object_CollectResources)

struct ShortcutListEntry {
    u16 owner;
    u16 ability;
};

void UiText_DrawNumberAtOffset(s32 value, s32 digits, struct RenderInput *window, s32 x, s32 y);
void UiWindow_DrawDividerLine(struct RenderInput *window, s32 x, s32 y, s32 width, s32 style);
void WaitFrames(s32 frames);
void AudioCommand_PlayFar(s32 cue);

void Debug_SelectAbilityPair(void)
{
    struct ShortcutListEntry *list;
    struct RenderInput *win;
    struct RenderInput *title;
    struct RenderInput *info;
    struct RenderOutput *cursor;
    u32 first;
    u32 second;
    u32 count;
    s32 done = 0;
    s32 row = 0;
    s32 redraw;
    s32 slot;
    s32 y;
    s32 cost;
    s32 ability;

    cursor = NULL;
    redraw = 1;
    list = MemAlloc(0x700);
    OwnerAction_AddFar(0, 140);
    OwnerAction_AddFar(1, 140);
    OwnerAction_AddFar(2, 140);
    OwnerAction_AddFar(2, 141);
    OwnerAction_AddFar(2, 78);
    OwnerAction_AddFar(3, 93);
    OwnerAction_AddFar(5, 140);
    second = 0;
    first = 0;
    count = Object_CollectResources(list);
    if (count != 0) {
        Menu_FindShortcutEntries(&first, &second, list);
        win = UiWindow_Create(4, 6, 20, 7, 2);
        title = UiWindow_Create(4, 3, 20, 3, 2);
        info = UiWindow_Create(4, 14, 20, 5, 2);
        slot = Resource_FindFreeEntry();
        if (slot != 0) {
            VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
            cursor = RenderOutput_Create(slot, 0x40000000, win, 0, 0);
        }
        /* FAKEMATCH: needed to match: the title message reuses the slot variable */
        slot = (s32)MsgPanelHpMaxLabel;
        UiText_DrawCharacterAtOffset(slot, title, 16, 0);
        for (;;) {
            if (redraw != 0) {
                redraw = 0;
                first = (first + count) % count;
                second = (second + count) % count;
                row = (row + 2) % 2;
                y = (row << 4) + (win->y << 3) + 28;
                cursor->y = y;
                cursor->oam.y = y;
                RenderOutput_RedrawSavedRect(win);
                UiWindow_DrawDividerLine(win, 1, 2, 17, 2);
                UiText_DrawCharacterAtOffset((s32)MsgPanelStatusLabel, win, 48, 0);
                UiText_DrawCharacterAtOffset(list[first].ability + (s32)MsgAbilityName, win, 56, 16);
                UiText_DrawCharacterAtOffset(list[second].ability + (s32)MsgAbilityName, win, 56, 32);
                UiText_DrawCharacterAtOffset((s32)MsgPanelStatusLabel - 2, win, 16, 16);
                UiText_DrawCharacterAtOffset((s32)MsgPanelStatusLabel - 1, win, 16, 32);
                UiText_DrawCharacterAtOffset(list[first].owner + (s32)MsgCharacterName, win, 104, 16);
                UiText_DrawCharacterAtOffset(list[second].owner + (s32)MsgCharacterName, win, 104, 32);
                RenderOutput_RedrawSavedRect(info);
                UiText_DrawCharacterAtOffset((s32)MsgPsynergyNeedMorePp, info, 0, 16);
                if (row != 0) {
                    cost = BtlActGet(list[second].ability)->pp_cost;
                    ability = list[second].ability;
                } else {
                    cost = BtlActGet(list[first].ability)->pp_cost;
                    ability = list[first].ability;
                }
                UiText_DrawNumberAtOffset(cost, 2, info, 64, 16);
                UiText_DrawCharacterAtOffset(ability + (s32)MsgAbilityDescription, info, 0, 0);
            }
            WaitFrames(1);
            if (gKeysRepeat & KEY_LEFT) {
                AudioCommand_PlayFar(111);
                if (row != 0)
                    second--;
                else
                    first--;
                redraw = 1;
            }
            if (gKeysRepeat & KEY_RIGHT) {
                AudioCommand_PlayFar(111);
                if (row != 0)
                    second++;
                else
                    first++;
                redraw = 1;
            }
            if (gKeysRepeat & KEY_UP) {
                AudioCommand_PlayFar(111);
                row--;
                redraw = 1;
            }
            if (gKeysRepeat & KEY_DOWN) {
                AudioCommand_PlayFar(111);
                row++;
                redraw = 1;
            }
            if (gKeyState & KEY_A)
                AudioCommand_PlayFar(112);
            else if (gKeyState & KEY_B)
                AudioCommand_PlayFar(113);
            else if (gKeyState & KEY_START)
                AudioCommand_PlayFar(113);
            else
                continue;
            /* FAKEMATCH: the unread flag forces fresh zeros for both entries and the cursor argument instead of reusing the row register. */
            done = 1;
            break;
        }
        gGameState.first_shortcut = (list[first].owner << 10) | list[first].ability;
        gGameState.second_shortcut = (list[second].owner << 10) | list[second].ability;
        UiWork_Finalize(win, 1);
        UiWork_Finalize(title, 1);
        UiWork_Finalize(info, 1);
        WaitFrames(1);
    }
    MemFree(list);
}

#undef Menu_FindShortcutEntries
#undef Object_CollectResources

struct BtlUnit *UnitGetFar(s32 object_id);

#define BtlActGet ((void * (*)(s32))BtlActGet)

#define ACTION_ID_MASK 0x3FFF

struct AbilityListEntry {
    u16 owner;
    u16 ability;
};

s32 Object_CollectResources(struct AbilityListEntry *output)
{
    u16 object_ids[14];
    s32 output_count = 0;
    s32 object_count = Party_ListActiveOwnersFar(object_ids);
    s32 i;

    for (i = 0; i < object_count; i++) {
        struct BtlUnit *object;
        u32 id;
        s32 index;
        u32 resource_id;

        id = object_ids[i];
        object = UnitGetFar(id);
        for (index = 0; index < (s32)(sizeof(object->action_slots) / sizeof(object->action_slots[0])); index++) {
            resource_id = object->action_slots[index].encoded_action & ACTION_ID_MASK;
            if (resource_id == 0)
                break;
            BtlActGet(resource_id);
            output[output_count].owner = id;
            output[output_count].ability = resource_id;
            output_count++;
        }
    }

    return output_count;
}

void Menu_FindShortcutEntries(u32 *first_index, u32 *second_index,
                              const struct AbilityListEntry *entries)
{
    s32 i;

    *first_index = 0;
    *second_index = 0;

    for (i = 0; i <= 447; i++) {
        if (entries[i].ability == (gGameState.first_shortcut & 0x3ff) &&
            entries[i].owner == (gGameState.first_shortcut >> 10)) {
            *first_index = i;
            break;
        }
    }

    for (i = 0; i <= 447; i++) {
        if (entries[i].ability == (gGameState.second_shortcut & 0x3ff) &&
            entries[i].owner == (gGameState.second_shortcut >> 10)) {
            *second_index = i;
            break;
        }
    }
}

void Menu_RunSelectionHook(void)
{
    Debug_SelectAbilityPair();
}

struct SelectionWorkspace {
    u8 unknown_000[0x46];
    u16 first_active;
    u16 first_resource;
    u8 unknown_04a[0x352 - 0x4a];
    u16 second_active;
    u16 second_resource;
    u8 unknown_356[0xff4 - 0x356];
    struct UiWindow *window;
    u8 unknown_ff8[12];
};

void Menu_AllocateSelectionWorkspace(void)
{
    struct SelectionWorkspace *work;
    u32 zero;

    work = Runtime_AllocateBlock(0x13, 0x1004);
    zero = 0;
    work->first_active = zero;
    work->second_active = zero;
}

void Ui_FinalizeWorkAndReleaseHeap13(void)
{
    struct SelectionWorkspace *state;
    u16 *p;

    state = *(struct SelectionWorkspace **)gSelectionWorkspace;
    UiWork_Finalize(state->window, 0);
    while (UiWork_IsIdle(state->window) == 0) {
        WaitFrames(1);
    }
    if (state->first_active != 0) {
        Resource_ResetEntry(state->first_resource);
    }
    p = &state->second_active;
    if (*p != 0) {
        p = (u16 *)((u8 *)p + 2);
        Resource_ResetEntry(*p);
    }
    Runtime_ReleaseHeapBlock(0x13);
}

void GraphicsPalette_ReservedNoOpC9BC(void)
{
}

void GraphicsPalette_ReservedNoOpC9C0(void)
{
}

void GraphicsPalette_ReservedNoOpC9C4(void)
{
}
