/* Draft, not exact (2026-09-25): 848 of 864 bytes, 321 differing halfwords.
   Written from the listing: every call, message, key test and the exit
   stores are in place; OwnerAction_AddFar returns a value (r1 before r0),
   and the loop closes with a goto after the A/B/Start chain.
   Remaining: the ROM keeps the list in r7, the main window in r6, the info
   window in r9, the count in sl, the row in r8 and the redraw flag in fp
   with the cursor in a stack slot (20-byte frame); here the heading message
   base 0xb1e (Value_00000b1e, whose -2/-1 neighbours the ROM derives with
   subs from a fresh pool load) stays live in r6 and pushes the redraw flag
   to the stack (24-byte frame). */
#include "TYPES.H"
#include "RENDER_INPUT.H"

struct AbilityPair {
    u16 owner;
    u16 ability;
};

struct BattleAction {
    u8 unknown_00[9];
    u8 pp_cost;
};

extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;
extern u8 Data_02000240[];
extern u8 Value_00000b19[];
extern u8 Value_00000b1e[];
extern u8 Value_00000066[];

void *Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *block);
s32 OwnerAction_AddFar(s32 owner, s32 action);
s32 Object_CollectResources(struct AbilityPair *output);
void Func_0801c8a0(u32 *first, u32 *second, struct AbilityPair *list);
struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 slot, s32 size, const void *source);
struct RenderOutput *RenderOutput_Create(s32 no, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *win, s32 x, s32 y);
void UiText_DrawNumberAtOffset(s32, s32, struct RenderInput *, s32, s32);
u32 Math_ModU(u32, u32);
void RenderOutput_RedrawSavedRect(struct RenderInput *win);
void UiWindow_DrawDividerLine(struct RenderInput *, s32, s32, s32, s32);
struct BattleAction *BattleAction_Get(s32 ability);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void UiWork_Finalize(struct RenderInput *win, s32 release);

/* Debug screen: teaches every party member a few test abilities, then lets
   the user pick two of the party's abilities (up and down choose the row,
   left and right the ability) showing each one's PP cost, and stores the
   chosen pair as (owner << 10) | ability. */
void Debug_SelectAbilityPair(void)
{
    struct AbilityPair *list;
    struct RenderInput *win;
    struct RenderInput *title;
    struct RenderInput *info;
    struct RenderOutput *cursor;
    struct BattleAction *action;
    s32 row;
    s32 redraw;
    u32 count;
    s32 slot;
    u32 ability;
    u32 second;
    u32 first;

    cursor = NULL;
    row = 0;
    redraw = 1;
    list = Runtime_BumpAllocate(0x700);
    OwnerAction_AddFar(0, 0x8c);
    OwnerAction_AddFar(1, 0x8c);
    OwnerAction_AddFar(2, 0x8c);
    OwnerAction_AddFar(2, 0x8d);
    OwnerAction_AddFar(2, 0x4e);
    OwnerAction_AddFar(3, 0x5d);
    OwnerAction_AddFar(5, 0x8c);
    second = 0;
    first = 0;
    count = Object_CollectResources(list);
    if (count != 0) {
        Func_0801c8a0(&first, &second, list);
        win = UiWindow_Create(4, 6, 20, 7, 2);
        title = UiWindow_Create(4, 3, 20, 3, 2);
        info = UiWindow_Create(4, 14, 20, 5, 2);
        slot = Resource_FindFreeEntry();
        if (slot != 0) {
            VramBlock_LoadCached(slot, 0x80, (const void *)0x080310a4);
            cursor = RenderOutput_Create(slot, 0x40000000, win, 0, 0);
        }
        UiText_DrawCharacterAtOffset(0xb19, title, 16, 0);
    loop:
            if (redraw) {
                redraw = 0;
                first = Math_ModU(first + count, count);
                second = Math_ModU(second + count, count);
                row = (row + 2) % 2;
                slot = (row << 4) + (win->y << 3) + 28;
                cursor->y = slot;
                *(u8 *)&cursor->packed = slot;
                RenderOutput_RedrawSavedRect(win);
                UiWindow_DrawDividerLine(win, 1, 2, 17, 2);
                UiText_DrawCharacterAtOffset((s32)Value_00000b1e, win, 48, 0);
                UiText_DrawCharacterAtOffset(list[first].ability + 0x333, win, 56, 16);
                UiText_DrawCharacterAtOffset(list[second].ability + 0x333, win, 56, 32);
                UiText_DrawCharacterAtOffset((s32)(Value_00000b1e - 2), win, 16, 16);
                UiText_DrawCharacterAtOffset((s32)(Value_00000b1e - 1), win, 16, 32);
                UiText_DrawCharacterAtOffset(list[first].owner + 0x66, win, 104, 16);
                UiText_DrawCharacterAtOffset(list[second].owner + 0x66, win, 104, 32);
                RenderOutput_RedrawSavedRect(info);
                UiText_DrawCharacterAtOffset(0xaec, info, 0, 16);
                if (row != 0) {
                    action = BattleAction_Get(list[second].ability);
                    ability = list[second].ability;
                } else {
                    action = BattleAction_Get(list[first].ability);
                    ability = list[first].ability;
                }
                UiText_DrawNumberAtOffset(action->pp_cost, 2, info, 64, 16);
                UiText_DrawCharacterAtOffset(ability + 0x53a, info, 0, 0);
            }
            WaitFrames(1);
            if (Data_03001b04 & 0x20) {
                Audio_PlayCue(0x6f);
                if (row != 0)
                    second--;
                else
                    first--;
                redraw = 1;
            }
            if (Data_03001b04 & 0x10) {
                Audio_PlayCue(0x6f);
                if (row != 0)
                    second++;
                else
                    first++;
                redraw = 1;
            }
            if (Data_03001b04 & 0x40) {
                Audio_PlayCue(0x6f);
                row--;
                redraw = 1;
            }
            if (Data_03001b04 & 0x80) {
                Audio_PlayCue(0x6f);
                row++;
                redraw = 1;
            }
            if (Data_03001c94 & 1) {
                Audio_PlayCue(0x70);
            } else if (Data_03001c94 & 2) {
                Audio_PlayCue(0x71);
            } else if (Data_03001c94 & 8) {
                Audio_PlayCue(0x71);
            } else {
                goto loop;
            }
        *(u16 *)(Data_02000240 + 0x220) = (list[first].owner << 10) | list[first].ability;
        *(u16 *)(Data_02000240 + 0x222) = (list[second].owner << 10) | list[second].ability;
        UiWork_Finalize(win, 1);
        UiWork_Finalize(title, 1);
        UiWork_Finalize(info, 1);
        WaitFrames(1);
    }
    Runtime_BumpFree(list);
}
