#include "FIXED_MATH.H"
#include "TYPES.H"
#include "SYSTEM.H"

#define FIELD_S32(base, offset) (*(s32 *)((u8 *)(base) + (offset)))
#define OBJECT_X(object) FIELD_S32(object, 0x08)
#define OBJECT_Y(object) FIELD_S32(object, 0x0C)
#define OBJECT_Z(object) FIELD_S32(object, 0x10)
#define OBJECT_TARGET_Y(object) FIELD_S32(object, 0x14)
#define OBJECT_VERTICAL_STEP(object) FIELD_S32(object, 0x28)
#define OBJECT_MIRRORED_Y(object) FIELD_S32(object, 0x3C)

struct Child_08092624 {
    u8 pad_00[9];
    u8 low_09 : 2;
    u8 copied_09 : 2;
    u8 high_09 : 4;
    u8 pad_0a[28];
    u8 field_26;
};

struct Object_08092624 {
    u8 pad_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[28];
    s32 field_30;
    s32 field_34;
    u8 pad_38[24];
    struct Child_08092624 *child;
    u8 pad_54;
    u8 mode_55;
    u8 pad_56[14];
    u16 field_64;
    u8 pad_66[6];
    void (*callback_6c)(void);
};

extern struct Object_08092624 *Object_CreateFar(s32, s32, s32, s32);
extern s32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void Object_SetMode(void *, s32);
extern void Object_SetCallback(struct Object_08092624 *, const void *);
extern void ObjectGroup_SetChildValue(struct Object_08092624 *);
extern s32 Math_ModU(s32, s32);
extern const u8 BattleFx_BurstParticleScriptA[];
extern const u8 BattleFx_BurstParticleScriptB[];
s32 Object_GetById(u32);
void Audio_PlayCue(s32);
void Object_SetPosition(void *, s32, s32, s32);
void ObjectMotion_ArmCallback(s32 arg0, s32 arg1, s32 arg2);
void Object_CommitPosition(void *);
void BattleFx_PlayQueuedSound(void);

void ParticleEffect_UpdateLinearMotion(void *particle);

void BattleFx_SpawnBurstParticle(struct Object_08092624 *source, s32 optional)
{
    struct Object_08092624 *object;
    struct Child_08092624 *child;
    s32 value;

    object = Object_CreateFar(222, source->x, source->y, source->z);
    if (object != 0) {
        child = object->child;
        switch (Rand() & 1) {
        case 1:
            Object_SetMode(object, 2);
            Object_SetCallback(object, BattleFx_BurstParticleScriptA);
            break;
        default:
            Object_SetMode(object, 1);
            Object_SetCallback(object, BattleFx_BurstParticleScriptB);
            break;
        }

        if (optional != 0)
            ObjectGroup_SetChildValue(object);

        object->mode_55 = 0;
        value = Math_ModU(Rand(), 10) + 5;
        object->field_34 = -0x1999 * value;
        value = Math_ModU(Rand(), 15) - 7;
        value <<= 1;
        object->field_30 = 0x1999 * value;
        object->field_64 = 0;
        object->callback_6c = (void (*)(void))ParticleEffect_UpdateLinearMotion;
        child->field_26 = 0;
        child->copied_09 = source->child->copied_09;
    }
}

void BattleFx_RunRisingObjectSequence(s32 sequence_arg, s32 mode_or_frame, s32 optional_action)
{
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
                BattleFx_SpawnBurstParticle(object, optional_action);
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
        BattleFx_PlayQueuedSound();
    }
}
