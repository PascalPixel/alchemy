#include "layout_guard.h"
#include "types.h"

typedef struct EffectDefinition_080a46b4 {
    u8 padding00[2];
    u8 placement;
    u8 padding03[9];
    u8 usability;
    u8 padding0d[0x1b];
    u16 action;
} EffectDefinition_080a46b4;

typedef struct ActionInfo_080a46b4 {
    u8 padding00;
    u8 flags;
    u8 padding02[6];
    u8 special_target;
} ActionInfo_080a46b4;

LAYOUT_OFFSET_GUARD(
    EffectDefinition080a46b4_Placement,
    EffectDefinition_080a46b4,
    placement,
    2);
LAYOUT_OFFSET_GUARD(
    EffectDefinition080a46b4_Usability,
    EffectDefinition_080a46b4,
    usability,
    0x0c);
LAYOUT_OFFSET_GUARD(
    EffectDefinition080a46b4_Action,
    EffectDefinition_080a46b4,
    action,
    0x28);
LAYOUT_OFFSET_GUARD(
    ActionInfo080a46b4_Flags,
    ActionInfo_080a46b4,
    flags,
    1);
LAYOUT_OFFSET_GUARD(
    ActionInfo080a46b4_SpecialTarget,
    ActionInfo_080a46b4,
    special_target,
    8);

s32 Func_08077218(s32 combatant, u16 effect_id);
EffectDefinition_080a46b4 *Func_08077018(s32 effect_id);
ActionInfo_080a46b4 *Func_08077080(s32 action);
s32 Func_0808a490(s32 effect_id);

/*
 * Classify how the battle menu should collect this effect's target. The
 * caller distinguishes immediate modes (-1/0), the normal target picker (1),
 * and the special picker (2).
 */
s32 Func_080a46b4(s32 combatant, s32 encoded_effect)
{
    u16 effect_id = encoded_effect & 0x01ff;
    EffectDefinition_080a46b4 *effect =
        Func_08077018(effect_id);
    ActionInfo_080a46b4 *action;
    s32 selection_mode = -1;

    if (Func_0808a490(effect_id) != 0)
        return 0;

    action = Func_08077080(effect->action & 0x3fff);
    if (effect->action != 0) {
        if (effect->placement == 0 ||
            (effect->usability != 3 &&
             Func_08077218(combatant, effect_id) != 0)) {
            selection_mode = 1;
        }

        if (selection_mode == 1) {
            if ((action->flags & 0x40) != 0) {
                selection_mode =
                    action->special_target == 0xff ? 2 : 1;
            } else {
                selection_mode =
                    (action->flags & 0x80) != 0 ? -1 : 0;
            }
        }
    }

    return selection_mode;
}
