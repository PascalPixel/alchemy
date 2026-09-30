#include "TYPES.H"
#include "BATTLE_STATUS_ICON.H"

u8 *Owner_GetStateFar(s32);

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
struct StatusIconEffect *ResourceMetadata_RegisterFar(struct EffectContext *context, s32 effect_id);
void ResourceMetadata_UnregisterFar(struct EffectContext *context, struct StatusIconEffect *effect);
void Animation_SetWorkEntryFar(struct StatusIconEffect *effect, s32 entry_index);

s32 *GetBattleObjectSlot(s32);
void Object_SetMode(s32, s32);
void ObjectDispatch_ApplyValueToChildrenFar(s32, s32);

s32 BattleUnit_BuildStatusFlags(s32 id, u8 *output)
{
    u8 *state = Owner_GetStateFar(id);
    s8 mode = *(s8 *)(state + 0x131);
    u32 flags = 0;

    if (mode == 1)
        flags = 1;
    if (mode == 2)
        flags |= mode;
    if (state[0x138] != 0)
        flags |= 0x20;
    if (state[0x13B] != 0) {
        s32 kind;
        flags |= 4;
        kind = state[0x128];
        if (kind == 0x79 || kind == 0x94)
            flags &= ~4;
    }
    if (state[0x13D] != 0)
        flags |= 8;
    if (state[0x140] != 0)
        flags |= 0x40;
    if (state[0x13C] != 0)
        flags |= 0x10;
    if (state[0x141] != 0)
        flags |= 1 << (state[0x141] + 6);
    *(u16 *)(output + 0x1C) = flags;
}

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
        ResourceMetadata_UnregisterFar(context, record->icon_effect);
        record->icon_effect = 0;
    }

    if (effect_id >= 0 && changed != 0) {
        effect = ResourceMetadata_RegisterFar(context, effect_id);
        record->icon_effect = effect;
        if (effect == (struct StatusIconEffect *)-1)
            record->icon_effect = 0;
        effect = record->icon_effect;
        if (effect != 0) {
            effect->state = 3;
            Animation_SetWorkEntryFar(effect, 0);
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

s32 BattlePres_SetActorModeAndAction(s32 id)
{
    u8 *state;
    s32 value;

    state = Owner_GetStateFar(id);
    value = 1;
    if (*(s16 *)(state + 56) != 0) {
        if (state[316] != 0 || state[315] != 0 || state[325] != 0)
            value = (state[298] != 1) * 4;
    } else {
        s32 changed = state[298] ^ value;
        value = (u32)(-changed | changed) >> 31;
        value = 5 - value;
    }

    Object_SetMode(*GetBattleObjectSlot(id), value);
    ObjectDispatch_ApplyValueToChildrenFar(*GetBattleObjectSlot(id), (id & 3) + 14);
}
