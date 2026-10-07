#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_STATUS_ICON.H"
#include "BATTLE_RUNTIME.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"
#include "BATTLE_PRESENTATION.H"
#include "FIELD_EVENT.H"

#define ObjectDispatch_ApplyValueToChildrenFar ((void (*)(struct MotionObject*,s32))ObjectDispatch_ApplyValueToChildrenFar)

void BattleActor_UpdateSpritePriorities(void)
{
    u16 ids[14];
    s32 priority[2];
    s32 i;
    s32 j;
    s32 count;
    struct MotionObject *object;
    struct AnimationObject *record;
    struct AnimationObject **list;

    BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
    /* FAKEMATCH: the scan is rotated by hand with a goto, which keeps the loop pass off it; as a for loop it becomes a pointer walk. */
    i = 0;
    if (ids[i] != BATTLE_UNIT_LIST_END) {
again:
        {
            struct BtlObj *slot = BtlObjGet(ids[i]);

            if (slot != 0) {
                object = slot->object;

                BattleStatusIcon_Cycle(slot);
                if (slot->effect_entry != 0) {
                    record = GetMotionRecord(object, 0);

                    if (record != 0) {
                        struct SpriteEntry *effect;
                        s32 state = 0;

                        if (object->y != 0)
                            state = 9;
                        effect = slot->effect_entry;
                        if (effect->priority != state) {
                            effect->priority = state;
                            record->dirty = 1;
                        }
                    }
                }
            }
        }
        i++;
        if (i <= 13 && ids[i] != BATTLE_UNIT_LIST_END)
            goto again;
    }
    if ((s16)gCameraWork->yaw >= 0) {
        priority[0] = 1;
        priority[1] = 2;
    } else {
        priority[0] = 2;
        priority[1] = 1;
    }
    {
        count = BattleParty_ListActorIds(BATTLE_SIDE_PARTY, ids);
        for (i = 0; i < count; i++) {
            struct BtlObj *slot = BtlObjGet(ids[i]);

            if (slot != 0) {
                object = slot->object;

                switch (object->record_storage_kind & 15) {
                case 1:
                {
                    record = object->records;
                    record->part[0].priority = priority[0];
                    break;
                }
                case 2:
                {
                    list = (struct AnimationObject **)object->records;
                    for (j = 0; j < 4; j++) {
                        record = *list++;
                        if (record != 0)
                            record->part[0].priority = priority[0];
                    }
                    break;
                }
                }
            }
        }
    }
    {
        count = BattleParty_ListActorIds(BATTLE_SIDE_ENEMIES, ids);
        for (i = 0; i < count; i++) {
            struct BtlObj *slot = BtlObjGet(ids[i]);

            if (slot != 0) {
                object = slot->object;

                switch (object->record_storage_kind & 15) {
                case 1:
                {
                    record = object->records;
                    record->part[0].priority = priority[1];
                    break;
                }
                case 2:
                {
                    list = (struct AnimationObject **)object->records;
                    for (j = 0; j < 4; j++) {
                        record = *list++;
                        if (record != 0)
                            record->part[0].priority = priority[1];
                    }
                    break;
                }
                }
            }
        }
    }
}



s32 BattleUnit_BuildStatusFlags(s32 id, struct BtlObj *output)
{
    struct BtlUnit *state = UnitGetFar(id);
    s8 mode = state->doku;
    u32 flags = 0;

    if (mode == 1)
        flags = 1;
    if (mode == 2)
        flags |= mode;
    if (state->maboroshi != 0)
        flags |= 0x20;
    if (state->shibire != 0) {
        s32 kind;
        flags |= 4;
        kind = state->class_id;
        if (kind == 0x79 || kind == 0x94)
            flags &= ~4;
    }
    if (state->fuuji != 0)
        flags |= 8;
    if (state->akuryo != 0)
        flags |= 0x40;
    if (state->nemuri != 0)
        flags |= 0x10;
    if (state->shiryo != 0)
        flags |= 1 << (state->shiryo + 6);
    output->active_conditions = flags;
}

s32 BattleStatusIcon_Cycle(struct BtlObj *record)
{
    struct AnimationEntry *old_effect;
    struct AnimationEntry *effect;
    struct AnimationObject *context;
    struct MotionObject *owner;
    s32 effect_id;
    s32 entry_handle;
    s32 prev;
    s32 changed = 0;

    if (record->cycle_timer >= 0)
        record->cycle_timer--;

    old_effect = (struct AnimationEntry *)record->animation_entry;
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
        ResourceMetadata_UnregisterFar(context, (s32)record->animation_entry);
        record->animation_entry = 0;
    }

    if (effect_id >= 0 && changed != 0) {
        /* FAKEMATCH: storing the handle before testing failure avoids an extra unconditional branch. */
        entry_handle = ResourceMetadata_RegisterFar(context, effect_id);
        record->animation_entry = (struct SpriteEntry *)entry_handle;
        if (entry_handle == -1)
            record->animation_entry = 0;
        effect = (struct AnimationEntry *)record->animation_entry;
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
    struct BtlUnit *state;
    s32 value;

    state = UnitGetFar(id);
    value = 1;
    if (state->hp != 0) {
        if (state->nemuri != 0 || state->shibire != 0 || state->miugoki != 0)
            value = (state->side != BATTLE_UNIT_ENEMY) * 4;
    } else {
        s32 changed = state->side ^ value;
        value = (u32)(-changed | changed) >> 31;
        value = 5 - value;
    }

    Object_SetMode(BtlObjGet(id)->object, value);
    ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(id)->object, (id & 3) + 14);
}
