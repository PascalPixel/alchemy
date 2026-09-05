#include "types.h"
#include "battle_status_icon.h"

struct BattleStatusIconOwner {
    u8 reserved[0x50];
    void *state_pointer;
    u8 state_flags;
};

struct EffectContext {
    u8 reserved_000[32];
    u8 type;
    u8 reserved_021[4];
    u8 dirty;
};

struct StatusIconEffect {
    u8 reserved_000[6];
    u8 state;
};

struct BattleStatusIconRecord {
    struct BattleStatusIconOwner *owner;
    u8 reserved_004[4];
    u16 displayed_effect_id;
    u8 reserved_00a[18];
    u16 active_conditions;
    u8 selected_condition;
    s8 cycle_timer;
    struct StatusIconEffect *icon_effect;
    void *secondary_effect;
};

void *GetMotionRecord(struct BattleStatusIconOwner *owner, s32 entry_index);
struct StatusIconEffect *Func_08009048(struct EffectContext *context, s32 effect_id);
void Func_08009050(struct EffectContext *context, struct StatusIconEffect *effect);
void Func_08009070(struct StatusIconEffect *effect, s32 entry_index);

/*
 * The reference preserves r0 in its epilogue (pop {r1}; bx r1), matching GCC's
 * scalar-return convention. No path establishes a meaningful battle_result, and the
 * sole caller discards it; C99 6.9.1p12 only makes this fallthrough undefined
 * when the caller uses the battle_value.
 */
s32 BattleStatusIcon_Cycle(struct BattleStatusIconRecord *record)
{
    struct StatusIconEffect *old_effect;
    struct StatusIconEffect *effect;
    struct EffectContext *context;
    struct BattleStatusIconOwner *owner;
    s32 effect_id;
    s32 prev;
    s32 changed = 0;

    if (record->cycle_timer >= 0)
        record->cycle_timer--;

    old_effect = record->icon_effect;
    if (old_effect == 0) {
        if ((s16)record->active_conditions == 0)
            goto cooldown_expired;
        goto update;
    } else {
        if ((((s16)record->active_conditions >> record->selected_condition) & 1) == 0)
            goto update;
    }
cooldown_expired:
    if (record->cycle_timer != 0)
        goto done;

update:
    effect_id = -1;
    owner = record->owner;
    if ((s16)record->active_conditions != 0) {
        prev = record->selected_condition;
        for (effect_id = prev + 1;; effect_id++) {
            if (effect_id > 13)
                effect_id = 0;
            if ((((s16)record->active_conditions >> effect_id) & 1) != 0)
                break;
        }

        if (prev != effect_id || old_effect == 0) {
            record->selected_condition = effect_id;
            changed = 1;
        }
        record->cycle_timer = 80;
    } else {
        changed = 1;
    }

    context = GetMotionRecord(owner, 0);
    if (context == 0)
        goto done;

    if (effect_id >= 0) {
        if (context->type == 32)
            effect_id += 340;
        else
            effect_id += 355;
    }

    if (record->icon_effect != 0 && changed != 0) {
        Func_08009050(context, record->icon_effect);
        record->icon_effect = 0;
    }

    if (effect_id >= 0 && changed != 0) {
        effect = Func_08009048(context, effect_id);
        record->icon_effect = effect;
        if (effect == (struct StatusIconEffect *)-1)
            record->icon_effect = 0;
        effect = record->icon_effect;
        if (effect != 0) {
            effect->state = 3;
            Func_08009070(effect, 0);
        }
    }

    context->dirty = 1;
    if (effect_id >= 0)
        record->displayed_effect_id = effect_id;
    else
        record->displayed_effect_id = 0;

done:
    ;
}
