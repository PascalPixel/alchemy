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
void WaitFrames(s32);

struct BattleUnit *Runtime_GetObject(s32 id);
struct MotionRecord *GetMotionRecord(
    struct PresentationObject *object, s32 entry_index);

void Object_SetAction(struct PresentationObject *object, s32 action);

void Actor_ResetMotionAtAnchor(s32 id);
void Battle_Apply2(void *input, struct BattlePresentationWork *work);
void Func_080c9008(struct BattlePresentationWork *work);

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

    FunctionHead_080c10e8(0, 0);
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
        FunctionHead_080b8178(saved_input->secondary_id);
        WaitFrames(2);
        WaitFrames(4);
        WaitFrames(10);
        Actor_ResetMotionAtAnchor(saved_input->secondary_id);
    } else {
        Func_080c9008(&work);
        Battle_Run();
        Actor_ResetMotionAtAnchor(saved_input->secondary_id);
    }
    Actor_ResetMotionAtAnchor(work.primary_id);
    return 0;
}
