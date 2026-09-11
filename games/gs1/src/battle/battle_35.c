#include "types.h"
#include "scene.h"
#include "battle_status_icon.h"
#include "motion_object.h"
#include "global_cells.h"

/* battle/placement/contains_id.c */
s32 BattlePlacement_ContainsId(s16 *entries, s32 id)
{
    s16 *entry;
    s32 target;
    u16 value;
    u32 index;

    entry = entries;
    target = id;
    if (target > 7) {
        target += 0x78;
    }
    index = 0;
loop_3:
    value = (u16)*entry;
    entry += 1;
    if (value != 0xFF) {
        if (value == target) {
            return 1;
        }
        index += 1;
        if (index > 0xDU) {
            goto block_7;
        }
        goto loop_3;
    }
block_7:
    return 0;
}

/* battle/unit/build_status_flags.c */
u8 *Runtime_GetObject(s32);

s32 BattleUnit_BuildStatusFlags(s32 id, u8 *output)
{
    u8 *state = Runtime_GetObject(id);
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

/* battle/status_icon/cycle.c */
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
struct StatusIconEffect *Battle_Run(struct EffectContext *context, s32 effect_id);

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
        Battle_Apply(context, record->icon_effect);
        record->icon_effect = 0;
    }

    if (effect_id >= 0 && changed != 0) {
        effect = Battle_Run(context, effect_id);
        record->icon_effect = effect;
        if (effect == (struct StatusIconEffect *)-1)
            record->icon_effect = 0;
        effect = record->icon_effect;
        if (effect != 0) {
            effect->state = 3;
            Battle_Apply2(effect, 0);
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

/* battle/presentation/actor/set_mode_act.c */
u8 *Runtime_GetObject(s32);
s32 *GetBattleObjectSlot(s32);
void Object_SetMode(s32, s32);
void Object_SetAction(s32, s32);

s32 BattlePres_SetActorModeAndAction(s32 id)
{
    u8 *state;
    s32 value;

    state = Runtime_GetObject(id);
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
    Object_SetAction(*GetBattleObjectSlot(id), (id & 3) + 14);
}

/* battle/motion/release_battle_object_records.c */
void ReleaseBattleObjectRecords(s32 object_id)
{
    struct BattleObjectSlot *slot;
    struct MotionObject *object;
    s32 record_index;
    void *record;

    slot = GetBattleObjectSlot(object_id);
    if (slot != NULL) {
        object = slot->object;
        if (object != NULL) {
            slot->runtime_word_20 = 0;
            slot->runtime_word_24 = 0;
            record_index = 0;
            while ((record = GetMotionRecord(object, record_index)) != NULL) {
                Battle_Do(record);
                record_index += 1;
            }
            object->record_storage_kind = (s8)record;
            object->records = record;
        }
    }
}

/* battle/motion/get_battle_object_slot.c */
struct BattleObjectSlot *GetBattleObjectSlot(s32 object_id) {
    u8 *base = *(u8 **)ADDR_03001E74;
    u8 *result_base = base + 0x74;
    s32 offset;
    if (object_id > 7) {
        object_id -= 0x78;
    }
    offset = object_id + 0x2DC;
    if (base[offset] == 0xFF) {
        return NULL;
    }
    return (struct BattleObjectSlot *)(result_base + base[offset] * 0x2C);
}

/* battle/motion/reset_motion_record_group.c */
struct Fields_080b7e04 {
    u8 filler[0x10];
    s32 field10;
};

void ResetMotionRecordGroup(void *owner)
{
    s32 remaining;
    s32 zero;
    struct Fields_080b7e04 **items;

    if (owner != NULL) {
        zero = 0;
        items = (struct Fields_080b7e04 **)((u8 *)owner + 0x28);
        for (remaining = 3; remaining >= 0; remaining--) {
            struct Fields_080b7e04 *item = *items++;
            if (item != NULL) {
                item->field10 = zero;
            }
        }
    }
}

/* battle/motion/reset_battle_object_record_groups.c */
void ResetBattleObjectRecordGroups(struct MotionObject *object)
{
    s32 storage_kind;
    s32 remaining;
    void **record_groups;
    void *record_group;

    if (object != NULL) {
        storage_kind = object->record_storage_kind & 0xF;
        switch (storage_kind) {
        case 1:
            ResetMotionRecordGroup(object->records);
            return;
        case 2:
            record_groups = object->records;
            remaining = 3;
            do {
                record_group = *record_groups++;
                if (record_group != NULL) {
                    ResetMotionRecordGroup(record_group);
                }
                remaining -= 1;
            } while (remaining >= 0);
            break;
        }
    }
}

/* battle/motion/activate_battle_object_slot.c */
s32 ActivateBattleObjectSlot(s32 object_id)
{
  struct BattleObjectSlot *slot;
  Battle_Check();
  slot = GetBattleObjectSlot(object_id);
  slot->active = 1;
  return 0;
}

/* battle/motion/destroy_all_slot_objects.c */
s32 Object_Destroy(s32);
void *GetBattleObjectSlot(s32 arg0);

void *BattleMotion_DestroyAllSlotObjects(void)
{
    s32 no;
    s32 i;
    void *slot;

    i = 0;
    do {
        no = i + 0x78;
        if (i <= 7) {
            no = i;
        }
        slot = GetBattleObjectSlot(no);
        if ((slot != NULL) && (FIELD_AT_OFFSET(slot, s16 *, 0x28) != 0)) {
            Object_Destroy(FIELD_AT_OFFSET(slot, s32 *, 0));
            FIELD_AT_OFFSET(slot, s32 *, 0) = 0;
            FIELD_AT_OFFSET(slot, s16 *, 0x28) = 0;
        }
        i += 1;
    } while (i <= 0xD);
}

/* battle/get_work_entry_pair.c */
struct Entry_080b7eb4 {
    s32 first;
    s32 second;
    u8 filler8[36];
};

struct State_080b7eb4 {
    u8 filler0[0x80];
    struct Entry_080b7eb4 entries[1];
};

struct Output_080b7eb4 {
    s32 first;
    s32 middle;
    s32 second;
};

extern struct State_080b7eb4 *volatile gBattleWork;

s32 Battle_GetWorkEntryPair(s32 no, struct Output_080b7eb4 *out)
{
    struct State_080b7eb4 *state = gBattleWork;

    out->first = state->entries[no].first;
    out->middle = 0;
    out->second = state->entries[no].second;
    return 0;
}
