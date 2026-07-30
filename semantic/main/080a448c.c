#include "layout_guard.h"
#include "types.h"

struct BattleCommandAvailability {
    s8 top_left;
    s8 top_center;
    s8 top_right;
    s8 bottom_left;
    s8 bottom_center;
    s8 bottom_right;
};

struct BattleCommandStateView {
    u8 unknown_000[0x178];
    u16 selected_effect;
    u8 unknown_17a[0x9f];
    u8 combatant_count;
    u8 current_combatant;
};

struct EffectDefinition_080a448c {
    s16 value;
    u8 placement;
    u8 flags;
};

LAYOUT_SIZE_GUARD(
    BattleCommandStateView_Size,
    struct BattleCommandStateView,
    0x21c);

extern struct BattleCommandStateView *Data_03001f2c;

struct EffectDefinition_080a448c *Func_08077018(s32 effectId);
s32 Func_08077218(s32 combatant, s32 effectId);
s32 Func_0808a490(s32 effectId);
s32 Func_080a46b4(s32 combatant, s32 encodedEffect);

/*
 * Build the enabled/disabled state for the battle command menu's 3x2 grid.
 * Entries use 1 for enabled and -1 for disabled.  Restrictions are applied in
 * presentation order, so later effect-specific rules intentionally override
 * the initial placement defaults.
 */
void Func_080a448c(struct BattleCommandAvailability *commands)
{
    struct BattleCommandStateView *battle = Data_03001f2c;
    u16 encodedEffect = battle->selected_effect;
    s32 effectId = encodedEffect & 0x1ff;
    struct EffectDefinition_080a448c *effect =
        Func_08077018(effectId);
    s32 availability;

    if (effect->placement == 0) {
        commands->top_left = 1;
        commands->top_center = -1;
    } else {
        commands->top_left = -1;
        commands->top_center = 1;
    }

    availability =
        Func_080a46b4(battle->current_combatant, encodedEffect);
    commands->top_left = availability == -1 ? -1 : 1;

    if ((encodedEffect & 0x400) != 0)
        commands->top_left = -1;

    if (Func_08077218(battle->current_combatant, effectId) == 0)
        commands->top_center = -1;

    commands->bottom_left = 1;
    commands->bottom_right = 1;
    commands->top_right = 1;

    if ((encodedEffect & 0x200) != 0) {
        commands->bottom_center = 1;
        commands->top_center = -1;
    } else {
        commands->bottom_center = -1;
    }

    if ((effect->flags & 2) != 0) {
        commands->bottom_center = -1;
        if ((encodedEffect & 0x200) != 0) {
            commands->bottom_left = -1;
            commands->bottom_right = -1;
        }
    }

    if (Func_0808a490(effectId) != 0)
        commands->top_left = 1;

    if (battle->combatant_count <= 1)
        commands->bottom_left = -1;

    if ((effect->flags & 8) != 0)
        commands->bottom_right = -1;
}
