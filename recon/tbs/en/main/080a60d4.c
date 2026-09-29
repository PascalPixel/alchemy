/* 2026-09-29 alchemy permute: score 825 to 715 on the permuter's scorer (0
   is exact); remaining 21 register-only, 4 operand, 2 reordered, 1
   inserted, 3 deleted. Kept rewrites: 5x swap commutative operands, 5x
   reorder local declarations, 4x add a same-width cast, 3x introduce a
   temporary, 2x remove a temporary, 2x split or join a compound
   assignment, 1x change loop form, 1x pointer arithmetic or indexing.
   FAKEMATCH: the permuter's temporaries, register hints and swapped
   operand orders below only steer allocation and scheduling; no programmer
   would write them, so they stay tagged until a natural spelling replaces
   them. */
/* 2026-09-29 alchemy permute: score 1251 to 825 (with the build's names) on the permuter's scorer
   (0 is exact); remaining 20 register-only, 4 operand, 4 reordered, 1
   inserted, 3 deleted. Kept rewrites: 2x reorder independent statements,
   1x introduce a temporary, 1x drop a same-width cast, 1x change loop
   form, 1x pointer arithmetic or indexing, 1x test truth or compare with
   zero. FAKEMATCH: the permuter's temporaries, register hints and swapped
   operand orders below only steer allocation and scheduling; no programmer
   would write them, so they stay tagged until a natural spelling replaces
   them. */
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
void PsynergyMenu_CallIconRoutineWithValue(struct PsynergyOwnerMenu *menu, s32 owner);
void UiText_DrawWorkValueWithLabel(s32 window);
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
    s32 window;
    s32 shown;
    s32 i;
    s8 action_count;
    u16 *actions;

    menu = gMenuWork;
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
        s32 tmp2;
        if (pending) {
            pending = 0;
            selection = Math_Mod(count + selection, count);
            window = (s32)menu->status_window;
            owner = Owner_GetStateFar(owner_ids[selection]);
            PsynergyMenu_RefreshOwnerPsynergy(owner_ids[selection]);
            Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 0);
            Func_080a6614(menu->shortcut_window, owner_ids[selection]);
            PsynergyMenu_CallIconRoutineWithValue(menu, owner_ids[selection]);
            i = 3;
            if (i >= 0) {
                while (1) {
                    menu->row_positions[i] = (s32)&Value_0000001e;
                    --i;
                    if (i < 0)
                        break;
                }
            }
            menu->row_positions[selection] = (s32)&Value_0000001a;
            if (!GameFlag_TestFar(0x151) && (u32)!shown) {
                RenderOutput_ClearListFar(menu->info_window);
                RenderOutput_RedrawSavedRectFar(menu->info_window);
                UiText_DrawWorkValueWithLabel(menu->info_window);
                shown = 1;
            } else {
                GameFlag_ClearBitFar(0x151);
            }
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        tmp2 = (s32)1;
        WaitFrames(tmp2);
        if (gKeyState & (u32)1) {
            if (menu->psynergy_count) {
                Audio_PlayCue(112);
                result = owner_ids[selection];
                break;
            }
            Audio_PlayCue(114);
        }
        if ((gKeysHeld & 0x200) || (gKeyState & 0x100)) {
            u8 tmp;
            result = owner_ids[selection];
            if ((gKeyState & 0x200) != 0)
                menu->shortcut = 1;
            else
                menu->shortcut = 2;
            actions = Runtime_BumpAllocate(64);
            tmp = PsynergyMenu_CollectActions(owner, actions, 1);
            Runtime_BumpFree(actions);
            action_count = tmp;
            if (0 == action_count) {
                menu->shortcut = action_count;
                Audio_PlayCue(114);
            } else {
                Audio_PlayCue(112);
                break;
            }
        }
        if (2 & gKeyState) {
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
            selection += 1;
        }
    }
    menu->selection = selection;
    menu->selected_owner = owner_ids[selection];
    menu[0].owner = owner_ids[selection];
    return result;
}
