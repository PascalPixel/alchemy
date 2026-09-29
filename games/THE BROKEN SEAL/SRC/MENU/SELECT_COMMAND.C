/*
 * The character menu's command selection: shows the selected character's
 * status and available commands across two panes, switches characters on
 * request, and returns 1 when a command is confirmed or -1 when cancelled.
 */
#include "TYPES.H"
#include "SYSTEM.H"
#include "UI.H"

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

extern struct CharacterCommandMenu *gMenuWork;
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
extern u8 MsgSwitchCharacterHelp;

s32 Party_SumDjinnCountsFar(s32 side);
s32 UiWindow_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
s32 Scheduler_RemoveCallback(void (*callback)(void));
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Menu_UpdateEntryObjectTransforms(void);
void UiMenu_SlideCursor(s32 x, s32 y);
void UiMenu_PositionCursor(s32 x, s32 y);
void *Owner_GetStateFar(s32 owner);
void CharacterMenu_DrawStatusAilments(s32 window, s32 owner, s32 mode);
s32 CharacterMenu_BuildAvailability(u8 *entries, s32 flags, s32 owner);
s32 Math_Mod(s32 value, s32 divisor);
s32 GameFlag_TestFar(s32 flag);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void PsynergyMenu_CallIconRoutineWithValue(void *menu, s32 owner);
void ItemMenu_ResetCategory(void);
void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries, s32 invert);
void CharacterMenu_DrawSelectionLabels(s32 window, s32 selected, const u8 *entries);
void StatusMenu_ShowOwnerProgressMessage(s32 window, s32 selected, s32 has_djinn);
void Audio_PlayCue(s32 cue);

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

    menu = gMenuWork;
    pane = 0;
    result = 0;
    selected = 0;
    total = Party_SumDjinnCountsFar(-1);
    /* FAKEMATCH: retain the family's neg/orr/lsr boolean conversion. */
    has_djinn = (u32)(-total | total) >> 31;
    UiWindow_UpdateOrCreate(&menu->help_window, 0, 0, 30, 5, 2);
    Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
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
                    selected = Math_Mod(selected + limit, limit);
                    RenderOutput_RedrawSavedRectFar(menu->help_window);
                    if (has_ailments == 0) {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp,
                            menu->window, 80, -24);
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp + 1,
                            menu->window, 0, -24);
                    }
                } else {
                    RenderOutput_RedrawSavedRectFar(menu->help_window);
                    if (has_djinn != 0)
                        selected = (selected + 8) % 8;
                    else
                        selected = Math_Mod(selected + 7, 7);
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
                tab = Math_Mod(tab + menu->owner_count, menu->owner_count);
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
    Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
    for (i = 3; i >= 0; i--)
        menu->slot_y[i] = 128;
    ItemMenu_ResetCategory();
    return result;
}
