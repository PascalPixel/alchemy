#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "battle_motion.h"
#include "battle_command.h"
#include "battle_runtime.h"

/* battle/run_simple_presentation.c */
struct SimplePresentationInput {
    u8 primary_id;
    u8 reserved_01;
    u8 secondary_id;
};

struct BattlePresentationWork {
    s32 flags;
    s32 secondary_is_low_id;
    s32 primary_id;
    s32 secondary_id;
    s32 initial_value;
    s32 entry_count;
    s32 battle_mode;
    s32  reserved_1c;
    u8 reserved_20[4];
    s16 members[24];
};

struct PresentationObject {
    u8 reserved_00[8];
    s32 x;
    u8 reserved_0c[4];
    s32 z;
};

struct PresentationObjectSlot {
    struct PresentationObject *object;
};

struct MotionRecordValue {
    s16 battle_value;
};

struct MotionRecord {
    u8 reserved_00[40];
    struct MotionRecordValue *child;
};

extern s32 *gIw;

struct PresentationObjectSlot *GetBattleObjectSlot(s32 id);
s32 ArcTan2(s32 first, s32 second);
void WaitFrames(s32 frames);

void Runtime_GetObject(s32 id);
struct MotionRecord *GetMotionRecord(
    struct PresentationObject *object, s32 entry_index);

void Object_SetAction(struct PresentationObject *object, s32 action);

void Actor_ResetMotionAtAnchor(s32 id);

s32 BattlePres_RunSimple(struct SimplePresentationInput *input, s32 flags)
{
    struct BattlePresentationWork work;
    struct SimplePresentationInput *saved_input;
    struct PresentationObject *object;
    struct MotionRecord *record;
    s32 *facing;
    s32 angle;
    s32 adjusted;
    s32 facing_angle;
    s32 x;
    s32 z;
    s32 divisor;
    s32 scripted;

    facing = gIw;
    saved_input = input;
    object = GetBattleObjectSlot(saved_input->primary_id)->object;
    z = object->z;
    x = object->x;
    angle = (u16)ArcTan2(x, z);
    adjusted = angle - 0x2000;
    if (saved_input->primary_id > 7)
        adjusted = angle + 0x6000;
    adjusted &= 0x7fff;
    facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

    if (*facing == facing_angle) {
        *facing = facing_angle;
        WaitFrames(5);
    } else {
        *facing = facing_angle;
        WaitFrames(20);
    }

    Battle_Apply(0, 0);
    Battle_Apply2(saved_input, &work);
    Runtime_GetObject(work.primary_id);
    Runtime_GetObject(saved_input->secondary_id);

    scripted = flags & 2;
    record = GetMotionRecord(
        GetBattleObjectSlot(saved_input->primary_id)->object, 0);
    divisor = Battle_Place(record->child->battle_value, 2, 1);
    BattleMotion_ApproachTarget(
        work.primary_id,
        saved_input->secondary_id,
        divisor,
        0);
    Object_SetAction(GetBattleObjectSlot(work.primary_id)->object, 16);
    GetBattleObjectSlot(saved_input->secondary_id);

    if (saved_input->secondary_id <= 7)
        work.secondary_is_low_id = 1;
    else
        work.secondary_is_low_id = 0;
    if (scripted != 0) {
        WaitFrames(10);
        Battle_Do(saved_input->secondary_id);
        WaitFrames(2);
        WaitFrames(4);
        WaitFrames(10);
        Actor_ResetMotionAtAnchor(saved_input->secondary_id);
    } else {
        BattleFx_DispatchByIdRange(&work);
        Battle_Run();
        Actor_ResetMotionAtAnchor(saved_input->secondary_id);
    }
    Actor_ResetMotionAtAnchor(work.primary_id);
    return 0;
}

/* battle/motion/run_value_sequence.c */
struct ObjectSlot_080babdc {
    void *object;
};

struct Runtime_080babdc {
    u8 padding_00[65];
    u8 mode;
};

extern struct Runtime_080babdc *gBattleWork;

void *Runtime_GetObject(s32 id);
struct ObjectSlot_080babdc *GetBattleObjectSlot(s32 id);
void Object_SetMode(void *object, s32 mode);

void BattleMotion_SetRecordChildValues(void *object, s32 value);
void WaitFrames(s32 frames);

void BattleMotion_RunValueSequence(s32 id)
{
    u16 selection[2];
    u16 *sel;
    s32 target;
    s32 remaining;

    Runtime_GetObject(id);
    Object_SetMode(GetBattleObjectSlot(id)->object, 5);

    sel = selection;
    remaining = 1;
    do {
        target = 0xff;
        sel[1] = target;
        sel[0] = id;
        Battle_Do(sel);
        BattleMotion_SetRecordChildValues(GetBattleObjectSlot(id)->object, 7);
        WaitFrames(2);

        sel[0] = id;
        Battle_Do(sel);
        BattleMotion_SetRecordChildValues(GetBattleObjectSlot(id)->object, Battle_Check(id));
        WaitFrames(2);
        remaining--;
    } while (remaining >= 0);

    Battle_unk2_2(gBattleWork->mode);
}

/* battle/motion/initialize_actor_records.c */
u8 *Runtime_GetObject(s32);
s32 *GetBattleObjectSlot(s32);
u8 *GetMotionRecord(s32, s32);
void Object_InitializeMode(void *, s32);
void WaitFrames(s32);

void ActivateBattleObjectSlot(s32);

void BattleMotion_InitializeActorRecords(s32 id)
{
    void *items[4];
    u8 *state;
    u8 *item;
    u8 *child;
    s32 index;

    state = Runtime_GetObject(id);
    index = 0;
    while ((item = GetMotionRecord(*GetBattleObjectSlot(id), index)) != 0) {
        if (state[0x12a] != 1)
            Object_InitializeMode(item, 4);
        else
            Object_InitializeMode(item, 5);
        index++;
    }

    if (state[0x12a] == 1) {
        index = 0;
        while ((item = GetMotionRecord(*GetBattleObjectSlot(id), index)) != 0) {
            child = *(u8 **)(item + 40);
            items[index] = item;
            child[5] = 6;
            child[22] = 0xff;
            index++;
        }
        WaitFrames(4);
        Battle_Do(id);
        Battle_Apply(items, index);
        ActivateBattleObjectSlot(id);
    }
}

/* battle/target/select_random_position.c */
struct SlotArray { s16 items[64]; };

s32 BattleTarget_SelectRandomPosition(s32 require_living_unit)
{
    u16 positions[6];
    struct SlotArray *order;
    s16 *entry;
    u16 *cursor;
    s32 value;
    s32 count;
    s32 index;
    s32 slot;
    s32 tail;
    s32 offset;

    count = 0;
    order = (struct SlotArray *)BATTLE_TURN_ORDER;

    if (require_living_unit != 0) {
        for (;;) {
            index = 0;
            slot = 44;
            if (order->items[slot] != 255) {
                entry = order->items;
                do {
                    value = entry[slot];
                    if (value != 254) {
                        if (BattleUnit_Get(value)->hp != 0) {
                            positions[count] = index | 0x100;
                            count++;
                        }
                    }
                    slot++;
                    index++;
                } while (entry[slot] != 255);
            }
            goto pick;
        }
    } else {
        index = 0;
        slot = 50;
        tail = 50;
        offset = tail * 2;
        entry = (s16 *)(order->items + 1);
        if (*(s16 *)((char *)entry + offset) != 255) {
            cursor = (u16 *)entry;
            do {
                if ((s16)cursor[slot] != 254) {
                    positions[count] = index | 0x180;
                    count++;
                }
                slot++;
                tail++;
                index++;
            } while (entry[tail] != 255);
        }
    }

pick:
    if (count == 0)
        return 0;
    return positions[(u32)(Random16() * count) >> 16];
}
