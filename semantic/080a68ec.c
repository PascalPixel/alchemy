#include "layout_guard.h"
#include "types.h"

struct ActionSlot_080a68ec {
    u16 action;
    u16 unknown_02;
};

struct Combatant_080a68ec {
    u8 unknown_000[0x58];
    struct ActionSlot_080a68ec actions[32];
};

struct ActionInfo_080a68ec {
    u8 unknown_00;
    u8 flags;
    u8 unknown_02[10];
    u8 type;
};

LAYOUT_SIZE_GUARD(
    ActionSlot080a68ec_Size,
    struct ActionSlot_080a68ec,
    4);
LAYOUT_OFFSET_GUARD(
    Combatant080a68ec_Actions,
    struct Combatant_080a68ec,
    actions,
    0x58);
LAYOUT_OFFSET_GUARD(
    ActionInfo080a68ec_Flags,
    struct ActionInfo_080a68ec,
    flags,
    1);
LAYOUT_OFFSET_GUARD(
    ActionInfo080a68ec_Type,
    struct ActionInfo_080a68ec,
    type,
    0x0c);

struct ActionInfo_080a68ec *Func_08077080(s32 action);

/*
 * Build the combatant's displayable action list.
 *
 * Mode 1 keeps actions with a nonzero type.  Other modes scan the slots in
 * classification passes: pass zero admits typed or flag-0x40 actions, while
 * pass three admits the remaining actions.  Mode 2 stops before pass three.
 * The two intervening passes intentionally perform the table lookups without
 * admitting entries, matching the original traversal.
 */
u8 Func_080a68ec(
    const struct Combatant_080a68ec *combatant,
    u16 output[32],
    s32 mode)
{
    s32 count;
    s32 index;

    index = 31;
    do {
        output[index] = 0;
        index--;
    } while (index >= 0);

    count = 0;
    if (mode == 1) {
        for (index = 0; index < 32; index++) {
            u16 action = combatant->actions[index].action;

            if (action != 0 &&
                Func_08077080(action & 0x3fff)->type != 0) {
                output[count] = combatant->actions[index].action;
                count++;
            }
        }
    } else {
        s32 pass;
        s32 pass_count = mode == 2 ? 3 : 4;

        for (pass = 0; pass < pass_count; pass++) {
            for (index = 0; index < 32; index++) {
                struct ActionInfo_080a68ec *info;
                u16 action = combatant->actions[index].action;
                s32 admit;

                if (action == 0)
                    continue;

                info = Func_08077080(action & 0x3fff);
                admit = 0;
                if (pass == 0) {
                    if (info->type != 0 ||
                        (info->flags & 0x40) != 0) {
                        admit = 1;
                    }
                } else if (pass == 3) {
                    if (info->type == 0 &&
                        (info->flags & 0x40) == 0) {
                        admit = 1;
                    }
                }

                if (admit != 0) {
                    output[count] = combatant->actions[index].action;
                    count++;
                }
            }
        }
    }

    return (u8)count;
}
