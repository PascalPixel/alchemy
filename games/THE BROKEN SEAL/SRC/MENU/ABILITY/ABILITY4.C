/* Browse the current owner's Psynergy from the status menu. L/R changes owners;
 * A advances to Item and B returns to character selection. The row coordinates
 * are signed because this view hides the four owner slots above the screen. */
#include "TYPES.H"
#include "MENU_RESULT.H"
#include "SYSTEM.H"
#include "UI.H"
#include "TBS_EDITION.H"
#include "OWNER_STATE.H"
#include "PSYNERGY_MENU.H"

struct MenuEntryIcon {
    u8 unknown_00[5];
    u8 state;
    u8 unknown_06[9];
    u8 field_0f;
};

struct PsynergyStatusMenu {
    u8 unknown_000[8];
    s32 owner;
    u8 unknown_00c[8];
    struct MenuEntryIcon *cursor;
    u8 unknown_018[4];
    s8 tab;
    u8 unknown_01d[7];
    s32 window;
    u8 unknown_028[4];
    s32 info_window;
    u8 unknown_030[0x18];
    struct MenuEntryIcon *entry_icons[32];
    u8 unknown_0c8[0x44];
    s32 icon_window;
    u8 unknown_110[0xb8];
    u16 psynergies[32];
    u16 owners[8];
    u8 count;
    u8 owner_count;
    u8 owner_id;
    u8 unknown_21b[0x21];
    s16 slot_y[4];
    u8 unknown_244[0x1c];
    s8 selected_index[8];
};

struct OwnerActionState;
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
extern u8 MsgSwitchCharacterHelp;
void Menu_BuildPatternTiles(void);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
s32 UiWindow_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
void Menu_UpdateEntryObjectTransforms(void);
void Scheduler_RemoveCallback(void (*callback)(void));
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Menu_SpawnIconEntries(struct PsynergyStatusMenu *, s32);
s32 GameFlag_TestFar(s32 flag);
void ItemMenu_PosCategory(void);
void PsynergyMenu_DrawPreparedPsynergyIcons(s32 window, s32 owner);
s32 PsynergyMenu_DrawListPage(s32 window, s32 unused, const struct MenuResult *);
s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused, struct MenuResult *);
void UiMenu_PositionCursor(s32 x, s32 y);
void Audio_PlayCue(s32 cue);
s32 Math_Mod(s32 value, s32 divisor);
void PsynergyMenu_CallIconRoutineWithValue(void *menu, s32 owner);
void ItemMenu_HideAllIcons(void);

s32 PsynergyMenu_SelectAction(void)
{
    struct PsynergyStatusMenu *menu;
    s32 result;
    s32 done;
    s32 redraw;
    s32 first;
    s32 nav;
    s32 tab;
    s32 i;
    struct MenuResult state;

    menu = ((struct PsynergyStatusMenu *)gMenuWork);
    result = 0;
    done = 0;
    Menu_BuildPatternTiles();
    RenderOutput_RedrawSavedRectFar(menu->icon_window);
    UiWindow_UpdateOrCreate(&menu->info_window, 0, 0, 30, 5, 2);
    for (i = 3; i >= 0; i--)
        menu->slot_y[i] = -16;
    {
        s32 priority = 245;
        struct MenuEntryIcon **icons = menu->entry_icons;

        for (i = 31; i >= 0; i--) {
            struct MenuEntryIcon *icon = *icons++;

            if (icon != 0)
                icon->field_0f = priority;
        }
    }
    Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
    Menu_SpawnIconEntries(menu, menu->icon_window);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp, menu->window, SWITCH_HELP_X, -24);
    UiText_DrawCharacterAtOffsetFar((s32)&MsgSwitchCharacterHelp + 2, menu->window, 0, -24);

    while (done == 0 && GameFlag_TestFar(0x150) == 0) {
        ItemMenu_PosCategory();
        RenderOutput_RedrawSavedRectFar(menu->window);
        menu->count = (s32)PsynergyMenu_CollectActions(
            (struct OwnerActionState *)Owner_GetStateFar(menu->owner_id), menu->psynergies, 0);
        WaitFrames(1);
        Menu_BuildPageResult(&state, 0);
        PsynergyMenu_DrawPreparedPsynergyIcons(menu->window, menu->owner_id);
        redraw = 1;
        first = 1;

        while (GameFlag_TestFar(0x150) == 0) {
            if (redraw != 0) {
                redraw = 0;
                if (first != 0) {
                    first = 0;
                    PsynergyMenu_DrawListPage(menu->window, 0, &state);
                }
                PsynergyMenu_DrawRangePage(menu->window, 0, &state);
                WaitFrames(1);
            }
            WaitFrames(1);
            nav = Menu_HandlePageInput(0, state.entry_count, PAGE_ROWS, &state.row, &state.page);
            menu->cursor->state = 1;
#if defined(TBS_EDITION_JA)
            UiMenu_PositionCursor(58, state.row * 16 + 52);
#else
            UiMenu_PositionCursor(55, state.row * 16 + 60);
#endif
            if (nav == 1) {
                first = 1;
                redraw = 1;
            }
            if (nav == 0)
                redraw = 1;
            if (nav == -1)
                redraw = 0;
            if (gKeyState & 1) {
                Audio_PlayCue(112);
                result = 1;
                done = 1;
                break;
            }
            if (gKeyState & 2) {
                Audio_PlayCue(113);
                result = -1;
                done = 1;
                Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
                break;
            }
            if ((gKeysRepeat & 0x100) || (gKeysRepeat & 0x200)) {
                Audio_PlayCue(111);
                tab = menu->tab;
                menu->selected_index[menu->owners[tab]] = state.selected_index;
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
    RenderOutput_ClearListFar(menu->info_window);
    RenderOutput_RedrawSavedRectFar(menu->info_window);
    ItemMenu_HideAllIcons();
    RenderOutput_ClearListFar(menu->icon_window);
    RenderOutput_RedrawSavedRectFar(menu->window);
    return result;
}

/* The routine after the Psynergy action selector: it only reports
   success. */
s32 PsynergyMenu_ActionReturnTrue(void)
{
    return 1;
}

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id)
{
    struct PsynergyMenuState *menu = gMenuWork;

    Owner_GetStateFar(owner_id);
    ItemMenu_HideAllIcons();
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_PreparedIconsNoOp(void)
{
}

/* A second empty routine; nothing in the ROM refers to either. */
void PsynergyMenu_PreparedIconsSecondNoOp(void)
{
}
