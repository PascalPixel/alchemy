#include "types.h"
#include "fixed_math.h"

#define BattleEffect_RunPhasedRadialParticleSequence Func_08095dd0

extern s32 Data_02000240[];

struct PhasedRadialSequenceObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[68];
    u16 timer;
    u8 unknown_66[6];
    void *callback;
};

void BattleEffect_RunPhasedRadialParticleSequence(s32 arg)
{
    s8 *object_pointer;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } position;
    s32 value;
    s32 remaining;
    struct PhasedRadialSequenceObject *source_object;
    void *effect_slots;
    struct PhasedRadialSequenceObject *target_object;
    void *slot;
    s8 *slot_state;

    source_object = Object_GetById(arg);
    target_object = Object_GetById(Data_02000240[125]);
    if (source_object == NULL)
        return;

    Func_080958a8();
    effect_slots = *(void **)0x03001F30;
    Func_080b0048(0x201090);
    WaitFrames(30);
    ObjectMotion_ArmCallback(arg, 0x4000, 0);
    WaitFrames(20);
    do {
        Audio_PlayCue(173);
        Func_080925cc(arg, 1);
    } while (0);
    do {
        Audio_PlayCue(174);
        Func_080925cc(arg, 1);
    } while (0);
    Audio_PlayCue(175);
    Func_080925cc(arg, 1);
    WaitFrames(20);
    Audio_PlayCue(140);
    source_object->callback = (void *)0x08095bad;
    source_object->timer = 0;
    WaitFrames(80);
    source_object->callback = (void *)0x08095bd9;
    Object_SetMode(source_object, 3);

    position.x = source_object->x;
    position.y = source_object->y;
    position.z = source_object->z;
    NormalizeVector(&position);

    slot = (u8 *)effect_slots + 88;
    remaining = 23;
    do {
        Func_0809ba90(slot, 284, position.x, position.z);
        EffectSlot_SetCallback(slot, (void *)0x08095c09);
        EffectSlot_SetObjectMode(slot, 7);
        do {
            object_pointer = slot;
            value = *(s32 *)object_pointer;
            Func_08009248(value, 10);
        } while (0);
        value = Func_080022f4(Random16(), 3) + 0x10000;
        *(s32 *)((u8 *)slot + 44) = value;
        *(s32 *)((u8 *)slot + 40) = value;
        remaining--;
        WaitFrames(1);
        slot = (u8 *)slot + 72;
    } while (remaining >= 0);

    WaitFrames(60);
    ObjectMotion_ArmCallback(Data_02000240[125], 0x4000, 0);
    WaitFrames(20);
    Object_SetMode(Object_GetById(Data_02000240[125]), 28);
    {
        s32 next_state;

        WaitFrames(20);
        next_state = 2;
        slot_state = effect_slots;
        slot_state += 152;
        remaining = 23;
        do {
            if (slot_state[5] != 0)
                slot_state[0] = next_state;
            remaining--;
            slot_state += 72;
        } while (remaining >= 0);
    }

    WaitFrames(60);
    target_object->callback = (void *)0x08095b8d;
    WaitFrames(100);

    {
        s32 next_state;

        slot_state = effect_slots;
        next_state = 5;
        slot_state += 152;
        remaining = 23;
        do {
            if (slot_state[5] != 0)
                slot_state[0] = next_state;
            remaining--;
            slot_state += 72;
        } while (remaining >= 0);
    }

    WaitFrames(10);
    slot = NULL;
    target_object->callback = slot;
    target_object->scale_x = 0x10000;
    target_object->scale_y = 0x10000;
    WaitFrames(30);
    Func_080b0050();
    BattleEffect_ClearActiveSlotsAndScheduleUpdates();
}
