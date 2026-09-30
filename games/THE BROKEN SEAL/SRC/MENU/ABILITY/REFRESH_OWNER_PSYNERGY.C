#include "FAR_RUNTIME.H"
#include "OWNER_STATE.H"
#include "PSYNERGY_MENU.H"
#include "TYPES.H"
#include "SYSTEM.H"

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

struct PsynergyOwnerIcon {
    u8 reserved_00[5];
    u8 state;
};

struct PsynergyOwnerMenu {
    u8 reserved_000[8];
    s32 selected_owner;
    u8 reserved_00c[0x10];
    s8 selection;
    u8 reserved_01d;
    s8 count;
    u8 reserved_01f;
    s32 psynergy_window;
    s32 status_window;
    s32 shortcut_window;
    s32 info_window;
    u8 reserved_030[0x114];
    u16 row_positions[4];
    u8 reserved_14c[0xcc];
    u8 psynergy_count;
    u8 reserved_219;
    u8 owner;
    u8 reserved_21b;
    struct PsynergyOwnerIcon *shortcut_icon;
    u8 reserved_220[0x48];
    u8 shortcut;
};

extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;

s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Menu_SpawnIconEntries(struct PsynergyOwnerMenu *menu, s32 window);
struct PsynergyOwnerIcon *RenderOutput_CreateFromResourceFar(s32 kind, s32 index, s32 window, s32 x, s32 y);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 mode);
s32 Func_080a6614(s32 window, s32 owner);
void PsynergyMenu_CallIconRoutineWithValue(struct PsynergyOwnerMenu *menu, s32 owner);
void UiText_DrawWorkValueWithLabel(s32 window);
void RenderOutput_ClearListFar(s32 window);
void RenderOutput_RedrawSavedRectFar(s32 window);
void GameFlag_ClearBitFar(s32 flag);
s32 GameFlag_TestFar(s32 flag);
void UiMenu_PositionCursor(s32 x, s32 y);
void *Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *buffer);
void Audio_PlayCue(s32 cue);

/* Select the owner whose Psynergy is shown, or assign an L/R shortcut. */
s32 PsynergyMenu_SetupActionIcons(u16 *owner_ids)
{
    struct PsynergyOwnerMenu *menu;
    s32 selection;
    s32 count;
    s32 pending;
    s32 result;
    struct OwnerInventoryState *owner;
    s32 window;
    s32 shown;
    s32 i;
    u16 *actions;
    s32 action_count;
    u8 found;

    menu = (struct PsynergyOwnerMenu *)gMenuWork;
    selection = menu->selection;
    count = menu->count;
    pending = 1;
    result = 0;
    shown = 0;
    menu->shortcut = result;
    owner = Owner_GetStateFar(owner_ids[selection]);
    if (UiWindow_UpdateOrCreate(&menu->psynergy_window, 13, 3, 17, 10, 2))
        Menu_SpawnIconEntries(menu, menu->psynergy_window);
    if (UiWindow_UpdateOrCreate(&menu->shortcut_window, 13, 13, 17, 4, 2)) {
        menu->shortcut_icon = RenderOutput_CreateFromResourceFar(2, 0, menu->shortcut_window, 0, result);
        menu->shortcut_icon->state = 13;
    }
    while (!GameFlag_TestFar(0x150)) {
        if (pending) {
            pending = 0;
            selection = (count + selection) % count;
            window = menu->status_window;
            owner = Owner_GetStateFar(owner_ids[selection]);
            PsynergyMenu_RefreshOwnerPsynergy(owner_ids[selection]);
            Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 0);
            Func_080a6614(menu->shortcut_window, owner_ids[selection]);
            PsynergyMenu_CallIconRoutineWithValue(menu, owner_ids[selection]);
            for (i = 3; i >= 0; i--)
                menu->row_positions[i] = 0x1e;
            menu->row_positions[selection] = 0x1a;
            if (!GameFlag_TestFar(0x151) && !shown) {
                RenderOutput_ClearListFar(menu->info_window);
                RenderOutput_RedrawSavedRectFar(menu->info_window);
                UiText_DrawWorkValueWithLabel(menu->info_window);
                shown = 1;
            } else {
                GameFlag_ClearBitFar(0x151);
            }
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & 1) {
            if (menu->psynergy_count) {
                Audio_PlayCue(112);
                result = owner_ids[selection];
                break;
            }
            Audio_PlayCue(114);
        }
        if ((gKeyState & 0x200) || (gKeyState & 0x100)) {
            result = owner_ids[selection];
            if (gKeyState & 0x200)
                menu->shortcut = 1;
            else
                menu->shortcut = 2;
            actions = Runtime_BumpAllocate(64);
            found = PsynergyMenu_CollectActions((struct OwnerActionState *)owner, actions, 1);
            Runtime_BumpFree(actions);
            action_count = (s8)found;
            if (action_count == 0) {
                menu->shortcut = action_count;
                Audio_PlayCue(114);
            } else {
                Audio_PlayCue(112);
                break;
            }
        }
        if (gKeyState & 2) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }
        if (gKeysRepeat & 32) {
            Audio_PlayCue(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & 16) {
            Audio_PlayCue(111);
            selection++;
            pending = 1;
        }
    }
    menu->selection = selection;
    menu->selected_owner = owner_ids[selection];
    menu->owner = owner_ids[selection];
    return result;
}
#endif

void PsynergyMenu_RefreshOwnerEntries(s32 x, s32 y, s32 spacing);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 *, s32 x, s32 y);

void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id)
{
    u16 *psynergies;
    struct PsynergyMenuState *menu;
    struct OwnerActionState *owner;

    menu = gMenuWork;
    owner = (struct OwnerActionState *)Owner_GetStateFar(owner_id);
    psynergies = menu->psynergies;
    menu->psynergy_count =
        PsynergyMenu_CollectActions(owner, psynergies, 2);
    RenderOutput_RedrawSavedRectFar(menu->psynergy_window);
    PsynergyMenu_RefreshOwnerEntries(0x6c, 0x20, 8);
    PsynergyMenu_DrawPsynergyIcons(psynergies);
    if (menu->psynergy_count == 0) {
        UiText_DrawCharacterAtOffsetFar(
            (s32)&MsgPsynergyMenuEmpty,
            (s32 *)menu->psynergy_window,
            0,
            0x18);
    }
}
