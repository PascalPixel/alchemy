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
                *(u8 *)&cursor->packed = y;
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
