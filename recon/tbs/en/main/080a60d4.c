/* Draft: main:080a60d4, complete 688-byte owner.
 * Candidate 676 bytes; 106 aligned halfword edits remain.
 * Recovered the missing shortcut allocation, feedback and row-position
 * branches. Remaining: early literal pools, initial scheduling and the
 * signed action-count conversion. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "OWNER_STATE.H"

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

extern struct PsynergyOwnerMenu *gMenuWork;
extern volatile u32 gKeyState;
extern volatile u32 gKeysHeld;
extern volatile u32 gKeysRepeat;
extern char Value_0000001e;
extern char Value_0000001a;

struct OwnerInventoryState *Owner_GetStateFar(s32 owner);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Menu_SpawnIconEntries(struct PsynergyOwnerMenu *menu, s32 window);
struct PsynergyOwnerIcon *RenderOutput_CreateFromResourceFar(s32 kind, s32 index, s32 window, s32 x, s32 y);
s32 Math_Mod(s32 numerator, s32 denominator);
void PsynergyMenu_RefreshOwnerPsynergy(s32 owner);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 mode);
s32 Func_080a6614(s32 window, s32 owner);
void Func_080a1804(struct PsynergyOwnerMenu *menu, s32 owner);
void Func_080a23c0(s32 window);
void RenderOutput_ClearListFar(s32 window);
void RenderOutput_RedrawSavedRectFar(s32 window);
void GameFlag_ClearBitFar(s32 flag);
s32 GameFlag_TestFar(s32 flag);
void UiMenu_PositionCursor(s32 x, s32 y);
void *Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *buffer);
u8 PsynergyMenu_CollectActions(struct OwnerInventoryState *owner, u16 *actions, s32 mode);
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
    s32 shown;
    s32 window;
    s32 i;
    u16 *actions;
    s8 action_count;

    menu = gMenuWork;
    count = menu->count;
    selection = menu->selection;
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
            selection = Math_Mod(selection + count, count);
            window = menu->status_window;
            owner = Owner_GetStateFar(owner_ids[selection]);
            PsynergyMenu_RefreshOwnerPsynergy(owner_ids[selection]);
            Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 0);
            Func_080a6614(menu->shortcut_window, owner_ids[selection]);
            Func_080a1804(menu, owner_ids[selection]);
            for (i = 3; i >= 0; i--)
                menu->row_positions[i] = (s32)&Value_0000001e;
            menu->row_positions[selection] = (s32)&Value_0000001a;
            if (!GameFlag_TestFar(0x151) && !shown) {
                RenderOutput_ClearListFar(menu->info_window);
                RenderOutput_RedrawSavedRectFar(menu->info_window);
                Func_080a23c0(menu->info_window);
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
        if ((gKeysHeld & 0x200) || (gKeyState & 0x100)) {
            result = owner_ids[selection];
            if (gKeyState & 0x200)
                menu->shortcut = 1;
            else
                menu->shortcut = 2;
            actions = Runtime_BumpAllocate(64);
            action_count = (u8)PsynergyMenu_CollectActions(owner, actions, 1);
            Runtime_BumpFree(actions);
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
            pending = 1;
            selection++;
        }
    }
    menu->selection = selection;
    menu->selected_owner = owner_ids[selection];
    menu->owner = owner_ids[selection];
    return result;
}
