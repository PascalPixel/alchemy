#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RENDER_INPUT.H"
#include "BATTLE_TYPES.H"
#include "GAME_STATE.H"

/* One entry of a collected ability list, in the parts of a shortcut. */
struct ShortcutListEntry {
    u16 owner;
    u16 ability;
};

extern volatile u32 gKeysRepeat;
extern volatile u32 gKeyState;
extern const u8 Resource_FixedBlockBTiles[];
extern u8 MsgPanelHpMaxLabel[], MsgPanelStatusLabel[], MsgPsynergyNeedMorePp[];
extern u8 MsgAbilityName[], MsgCharacterName[], MsgAbilityDescription[];

s32 OwnerAction_AddFar(s32 owner, s32 action);
s32 Object_CollectResources(struct ShortcutListEntry *output);
void Menu_FindShortcutEntries(u32 *first_index, u32 *second_index,
                              const struct ShortcutListEntry *entries);
struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *window,
                                  s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 digits,
                               struct RenderInput *window, s32 x, s32 y);
void RenderOutput_RedrawSavedRect(struct RenderInput *window);
void UiWindow_DrawDividerLine(struct RenderInput *window, s32 x, s32 y,
                              s32 width, s32 style);
struct BattleAction *Ability_GetData(s32 ability);
void WaitFrames(s32 frames);
void AudioCommand_PlayFar(s32 cue);
void UiWork_Finalize(struct RenderInput *window, s32 release);

/* A debug screen that picks the two Psynergy shortcuts. It first hands a few
   abilities to the party, then lists every ability anyone knows: left and
   right step the entry on the cursor's row, up and down change the row, and
   A, B or Start store both entries as the shortcuts and leave.

   The labels are borrowed from the status panel: "HP/Max" is the title,
   "Status" heads the list, and the two rows are "Attack" and "Defense", the
   two messages before "Status". The line under the list is "You need more
   PP." followed by the cost of the entry on the cursor's row. */
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
    list = Runtime_BumpAllocate(0x700);
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
        {
            /* The title's message is held in r5 and copied to the argument.
               Written plainly, with the three labels in the loop as this
               message plus 5, 3 and 4, the compiler does exactly that by
               itself and the whole function matches: that is how the
               original addressed them. The id gate refuses a message id with
               an offset in a literal pool, so the labels are addressed from
               "Status" instead, whose id the pool holds whole. */
            /* FAKEMATCH: pin the title's message to r5, where the plain form puts it. */
            register s32 message asm("r5") = (s32)MsgPanelHpMaxLabel;

            UiText_DrawCharacterAtOffset(message, title, 16, 0);
        }
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
                    cost = Ability_GetData(list[second].ability)->pp_cost;
                    ability = list[second].ability;
                } else {
                    cost = Ability_GetData(list[first].ability)->pp_cost;
                    ability = list[first].ability;
                }
                UiText_DrawNumberAtOffset(cost, 2, info, 64, 16);
                UiText_DrawCharacterAtOffset(ability + (s32)MsgAbilityDescription, info, 0, 0);
            }
            WaitFrames(1);
            if (gKeysRepeat & 0x20) {
                AudioCommand_PlayFar(111);
                if (row != 0)
                    second--;
                else
                    first--;
                redraw = 1;
            }
            if (gKeysRepeat & 0x10) {
                AudioCommand_PlayFar(111);
                if (row != 0)
                    second++;
                else
                    first++;
                redraw = 1;
            }
            if (gKeysRepeat & 0x40) {
                AudioCommand_PlayFar(111);
                row--;
                redraw = 1;
            }
            if (gKeysRepeat & 0x80) {
                AudioCommand_PlayFar(111);
                row++;
                redraw = 1;
            }
            if (gKeyState & 1)
                AudioCommand_PlayFar(112);
            else if (gKeyState & 2)
                AudioCommand_PlayFar(113);
            else if (gKeyState & 8)
                AudioCommand_PlayFar(113);
            else
                continue;
            /* FAKEMATCH: the flag is written and never read. With it the
               zeros stored to the two entries and to the cursor's last
               argument are loaded fresh; without it they are copied from the
               row's register. A variable cleared at the top whose later
               writes are all dropped is the only thing found that does that,
               so the original very likely had such a flag, but its name and
               where it was set are a guess. */
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
    Runtime_BumpFree(list);
}
