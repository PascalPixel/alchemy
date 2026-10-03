#include "TYPES.H"
#include "BATTLE_STATUS_ICON.H"
#include "BATTLE_RUNTIME.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"

struct SpriteEntry *ResourceMetadata_RegisterFar(struct AnimationObject *context, s32 effect_id);
void ResourceMetadata_UnregisterFar(struct AnimationObject *context, struct SpriteEntry *effect);
void Animation_SetWorkEntryFar(struct SpriteEntry *effect, s32 entry_index);

void Object_SetMode(s32, s32);
void ObjectDispatch_ApplyValueToChildrenFar(s32, s32);

s32 BattleUnit_BuildStatusFlags(s32 id, struct BattleObjectSlot *output)
{
    struct BattleUnit *state = Owner_GetStateFar(id);
    s8 mode = state->poison;
    u32 flags = 0;

    if (mode == 1)
        flags = 1;
    if (mode == 2)
        flags |= mode;
    if (state->delusion != 0)
        flags |= 0x20;
    if (state->stun != 0) {
        s32 kind;
        flags |= 4;
        kind = state->class_id;
        if (kind == 0x79 || kind == 0x94)
            flags &= ~4;
    }
    if (state->psy_seal != 0)
        flags |= 8;
    if (state->evil_spirit != 0)
        flags |= 0x40;
    if (state->sleep != 0)
        flags |= 0x10;
    if (state->death_count != 0)
        flags |= 1 << (state->death_count + 6);
    output->active_conditions = flags;
}

/*
 * The reference preserves r0 in its epilogue (pop {r1}; bx r1), matching GCC's
 * scalar-return convention. No path establishes a meaningful result, and the
 * sole caller discards it; C99 6.9.1p12 only makes this fallthrough undefined
 * when the caller uses the value.
 */
s32 BattleStatusIcon_Cycle(struct BattleObjectSlot *record)
{
    struct SpriteEntry *old_effect;
    struct SpriteEntry *effect;
    struct AnimationObject *context;
    struct MotionObject *owner;
    s32 effect_id;
    s32 prev;
    s32 changed = 0;

    if (record->cycle_timer >= 0)
        record->cycle_timer--;

    old_effect = record->animation_entry;
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
    owner = record->object;
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
        if (context->width == 32)
            effect_id += 340;
        else
            effect_id += 355;
    }

    if (record->animation_entry != 0 && changed != 0) {
        ResourceMetadata_UnregisterFar(context, record->animation_entry);
        record->animation_entry = 0;
    }

    if (effect_id >= 0 && changed != 0) {
        effect = ResourceMetadata_RegisterFar(context, effect_id);
        record->animation_entry = effect;
        if (effect == (struct SpriteEntry *)-1)
            record->animation_entry = 0;
        effect = record->animation_entry;
        if (effect != 0) {
            effect->priority = 3;
            Animation_SetWorkEntryFar(effect, 0);
        }
    }

    context->dirty = 1;
    if (effect_id >= 0)
        record->animation = effect_id;
    else
        record->animation = 0;

done:
;
}

s32 BattlePres_SetActorModeAndAction(s32 id)
{
    struct BattleUnit *state;
    s32 value;

    state = Owner_GetStateFar(id);
    value = 1;
    if (state->hp != 0) {
        if (state->sleep != 0 || state->stun != 0 || state->cannot_move != 0)
            value = (state->status_12a != 1) * 4;
    } else {
        s32 changed = state->status_12a ^ value;
        value = (u32)(-changed | changed) >> 31;
        value = 5 - value;
    }

    Object_SetMode((s32)GetBattleObjectSlot(id)->object, value);
    ObjectDispatch_ApplyValueToChildrenFar((s32)GetBattleObjectSlot(id)->object, (id & 3) + 14);
}
