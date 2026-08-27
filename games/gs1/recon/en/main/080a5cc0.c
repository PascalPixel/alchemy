#include "types.h"
#include "far_runtime.h"
#include "battle_runtime.h"
#include "battle_calc.h"

/*
 * Data_03001f2c is the polymorphic menu-runtime cell (see inventory_menu.h /
 * psynergy_menu.h). This owner reads and writes fields shared by both the
 * Inventory and Psynergy menu views (item_owner/target_owner at 0x21a/0x21b,
 * info_window at 0x2c, the selected id at 0x178, entry_count at 0x218), plus
 * two fields not yet named in either shared header (a byte "mode" at 0x268
 * that selects between three confirmation messages, and a u16 flags word at
 * 0x220). A local view is used here instead of extending the shared structs,
 * matching the project's convention for an owner-specific field range
 * (compare games/gs1/src/item/use.c's local ItemUseWork).
 */
struct MenuActionWork {
    u8 unknown_000[0x24];
    s32 field_024;             /* 0x024 */
    u8 unknown_028[4];
    s32 info_window;           /* 0x02c */
    u8 unknown_030[0x148];
    u16 selected_action;       /* 0x178 */
    u8 unknown_17a[0x9e];
    u8 entry_count;            /* 0x218 */
    u8 unknown_219;
    u8 item_owner;             /* 0x21a */
    u8 target_owner;           /* 0x21b */
    u8 unknown_21c[4];
    u16 flags_220;             /* 0x220 */
    s16 completion_flag;       /* 0x222 */
    u8 unknown_224[0x36];
    s16 message_offset;        /* 0x25a */
    u8 unknown_25c[0x0c];
    u8 mode;                   /* 0x268 */
};

extern struct MenuActionWork *Data_03001f2c;

extern char Value_00000ae2;
extern char Value_00000ae3;
extern char Value_00000ae9;
extern char Value_00000aea;
extern char Value_00000aeb;
extern char Value_00000af0;
extern char Value_00000af1;
extern char Value_00000bef;

void Func_080030f8(s32 frames);
void Func_080a3cf8(s32 unused, s32 message);
s32 Func_080a602c(s32 unused);
void Func_080a9cbc(void);
s32 Func_080a112c(s32 window, s32 owner, s32 unused0, s32 unused1);
s32 Func_080a6ccc(s32 unused);
s32 Func_080a65e4(s32 owner, s32 psynergy, s32 shortcut);
s32 Func_08015278(s32 window);
s32 Func_080a1d08(s32 message, s32 arg1, s32 arg2);
s32 Func_080a63e4(s32 unused);
s32 Func_080a5fe0(void);
s32 Func_080a9f10(s32 action, s32 owner, s32 target, s32 flags);
void Func_080aa460();
void Func_080f9010(s32 cue);

#define InventoryMenu_DrawMessage Func_080a3cf8
#define InventoryMenu_PositionCategoryItems Func_080a9cbc
#define InventoryMenu_ShowModalMessage Func_080a1d08
#define PsynergyMenu_SetShortcut Func_080a65e4
#define PsynergyMenu_ClassifySelectedPsynergy Func_080a5fe0
#define Ability_PlayUseAnimation Func_080aa460
#define WaitFrames Func_080030f8
#define Audio_PlayCue Func_080f9010

/*
 * State machine that resolves the currently selected item/Psynergy command:
 * state 0 primes the category window and waits for the underlying selection
 * loop to finish; state 1 waits for a target/shortcut pick; state 2
 * classifies the selected Psynergy (immediate vs needs-target vs
 * needs-confirm); state 3 asks for a final confirmation; state 4 actually
 * applies the action, pays its PP cost, and reports success or failure. On
 * a clean finish (state 2's default) the acting owner and the selected
 * action id are written back through the two out-parameters.
 */
s32 Func_080a5cc0(s32 *out_owner, s32 unused, s32 *out_action)
{
    struct MenuActionWork *work;
    s32 result;
    s32 state;
    s32 done;
    s32 selection;
    s32 classification;
    s32 raw;
    s32 self_flag;
    struct BattleAction *action;

    work = Data_03001f2c;
    state = 0;
    done = 0;
    result = 0;

    while (done == 0 && Func_080770c0(0x150) == 0) {
        switch (state) {
        case 0:
            work->selected_action = 0;
            InventoryMenu_DrawMessage(0, (s32)&Value_00000ae9);
            if (Func_080a602c(0) == -1) {
                done = 1;
                result = -1;
            }
            Func_08015270(work->info_window);
            state = 1;
            break;

        case 1:
            WaitFrames(1);
            Func_08077008(work->item_owner);
            state = 0;
            if (work->entry_count != 0) {
                switch (work->mode) {
                case 0:
                    InventoryMenu_DrawMessage(0, (s32)&Value_00000aea);
                    break;
                case 1:
                    InventoryMenu_DrawMessage(0, (s32)&Value_00000af1);
                    break;
                case 2:
                    InventoryMenu_DrawMessage(0, (s32)&Value_00000af0);
                    break;
                }
                InventoryMenu_PositionCategoryItems();
                Func_080a112c(work->field_024, work->item_owner, 0, 0);
                selection = Func_080a6ccc(0);
                state = 0;
                if (selection != -1) {
                    state = 2;
                    if (work->mode != 0) {
                        if (work->mode == 1) {
                            PsynergyMenu_SetShortcut(
                                work->item_owner, selection, 0);
                            Func_08015278(work->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)&Value_00000ae2, -1, -1);
                        } else {
                            PsynergyMenu_SetShortcut(
                                work->item_owner, selection, 1);
                            Func_08015278(work->info_window);
                            InventoryMenu_ShowModalMessage(
                                (s32)&Value_00000ae3, -1, -1);
                        }
                        state = 0;
                    }
                }
            }
            break;

        case 3:
            InventoryMenu_DrawMessage(0, (s32)&Value_00000aeb);
            state = 4;
            if (Func_080a63e4(0) == -1) {
                work->flags_220 |= 1;
                state = 1;
            }
            break;

        case 2:
            classification = PsynergyMenu_ClassifySelectedPsynergy();
            if (classification == 1) {
                state = 3;
                break;
            }
            if (classification == 2) {
                work->target_owner = 9;
                state = 4;
                break;
            }
            done = 1;
            result = 1;
            *out_owner = work->item_owner;
            *out_action = work->selected_action & 0x3fff;
            break;

        case 4:
            raw = work->selected_action;
            self_flag = 0;
            if (work->target_owner == 9) {
                work->target_owner = work->item_owner;
                self_flag = 9;
            }
            result = Func_080a9f10(
                raw, work->item_owner, work->target_owner, 0);
            if (result != -1) {
                action = Func_08077080(work->selected_action & 0x3fff);
                Func_08077120(work->item_owner, -action->pp_cost);
            }
            Func_08077010(work->item_owner);
            if (result != -1) {
                Func_080a112c(work->field_024, work->target_owner, 0, 0);
                Ability_PlayUseAnimation(work->selected_action & 0x3fff);
                Func_08015278(work->info_window);
                InventoryMenu_ShowModalMessage(
                    work->message_offset + (s32)&Value_00000bef, 0, -1);
            } else {
                Audio_PlayCue(114);
                Func_08015278(work->info_window);
                InventoryMenu_ShowModalMessage(
                    work->message_offset + (s32)&Value_00000bef,
                    result,
                    result);
            }
            if (result == -1) {
                work->completion_flag = 1;
                if (self_flag == 9) {
                    work->flags_220 |= 1;
                    state = 1;
                } else {
                    state = 3;
                }
            } else {
                result = 1;
                work->flags_220 |= 1;
                state = 1;
            }
            break;

        default:
            done = 1;
            break;
        }
    }

    if (Func_080770c0(0x150) != 0) {
        result = -1;
    }
    return result;
}
