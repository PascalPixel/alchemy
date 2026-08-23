#include "types.h"

#define FIELD_S32(base, offset) (*(s32 *)((u8 *)(base) + (offset)))
#define OBJECT_X(object) FIELD_S32(object, 0x08)
#define OBJECT_Y(object) FIELD_S32(object, 0x0C)
#define OBJECT_Z(object) FIELD_S32(object, 0x10)
#define OBJECT_TARGET_Y(object) FIELD_S32(object, 0x14)
#define OBJECT_VERTICAL_STEP(object) FIELD_S32(object, 0x28)
#define OBJECT_MIRRORED_Y(object) FIELD_S32(object, 0x3C)

s32 Object_GetById(u32);
void Audio_PlayCue(s32);
void Object_SetMode(void *, s32);
void WaitFrames(u32);
void Object_SetPosition(void *, s32, s32, s32);
void ObjectMotion_ArmCallback(s32 arg0, s32 arg1, s32 arg2);
void BattleEffect_SpawnBurstParticle(void *, s32);
void Object_CommitPosition(void *);
void Func_0809202c(void);

void BattleEffect_RunRisingObjectSequence(s32 sequence_arg, s32 mode_or_frame, s32 optional_action) {
    s32 next_y;
    s32 base_z;
    void *object;
    u8 *object_flags;

    object = (void *)Object_GetById(sequence_arg);
    base_z = OBJECT_Z(object);
    if (object != 0) {
        Audio_PlayCue(0x121);
        Object_SetMode(object, mode_or_frame);
        WaitFrames(10);
        object_flags = (u8 *)object + 0x55;
        Object_SetMode(object, 1);
        {
            u8 flags = 2;
            flags |= *object_flags;
            *object_flags = flags;
        }
        OBJECT_VERTICAL_STEP(object) = 0x40000;
        Object_SetPosition(object, OBJECT_X(object), OBJECT_Y(object),
            base_z + 0xC0000);
        WaitFrames(6);
        Audio_PlayCue(0xD9);
        mode_or_frame = 0;
        ObjectMotion_ArmCallback(sequence_arg, 0x5000, 0);
        *object_flags = 0;
        do {
            next_y = OBJECT_Y(object) + 0xFFFE0000;
            OBJECT_Y(object) = next_y;
            OBJECT_MIRRORED_Y(object) = next_y;
            WaitFrames(1);
            if ((optional_action != -1) && (mode_or_frame & 1)) {
                BattleEffect_SpawnBurstParticle(object, optional_action);
            }
            mode_or_frame++;
        } while ((u32)mode_or_frame <= 0xD);
        *object_flags = 3;
        OBJECT_VERTICAL_STEP(object) = 0x30000;
        Object_SetPosition(object, OBJECT_X(object), OBJECT_Y(object),
            base_z + 0x100000);
        Object_CommitPosition(object);
        mode_or_frame = 0;
        if (OBJECT_Y(object) > OBJECT_TARGET_Y(object)) {
wait_for_target_y:
            WaitFrames(1);
            mode_or_frame++;
            if ((u32)mode_or_frame <= 0xB3) {
                if (OBJECT_Y(object) > OBJECT_TARGET_Y(object)) {
                    goto wait_for_target_y;
                }
            }
        }
        WaitFrames(2);
        Func_0809202c();
    }
}
