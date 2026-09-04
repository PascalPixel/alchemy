#include "fixed_math.h"
#include "types.h"
#include "object_efx.h"
#include "sound_ids.h"

struct ImpactPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct ImpactParticle;

void Audio_PlayCue(s32);
u8 *Object_Spawn(s32, s32, s32, s32);
void Object_SetCallback(void *, const void *);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void ObjectMotion_SetTargetPositionFromMagnitudeAngle(
    struct ImpactParticle *, s32, s32);

s32 SpawnHeavyImpactEffect(void *source)
{
    struct ImpactPosition position;
    void *first_object;
    void *child;
    register s32 count;
    register s32 speed;
    register struct ImpactPosition *position_pointer;

    Audio_PlayCue(SOUND_HEAVY_IMPACT);
    position.x = *(s32 *)((u8 *)source + 8);
    position.y = *(s32 *)((u8 *)source + 12);
    position.z = *(s32 *)((u8 *)source + 16);
    first_object = Object_Spawn(
        0x11B,
        position.x,
        position.y - 0x200000,
        position.z);
    if (first_object != 0) {
        u8 *field = (u8 *)first_object + 0x55;
        s32 timer;

        field[0] = 0;
        field += 9;
        timer = 20;
        *(u16 *)field = timer;
        Object_SetCallback(first_object, Data_0809f0b0);
    }

    position_pointer = &position;
    count = 11;
    speed = 0x10000;
    do {
        child = Object_Spawn(
            0x11D,
            position_pointer->x,
            position_pointer->y,
            position_pointer->z);
        if (child != 0) {
            s32 x;
            s32 y;

            Object_SetCallback(child, &Data_0809f0d4);
            x = Rand();
            x += speed;
            *(s32 *)((u8 *)child + 0x34) = speed;
            *(s32 *)((u8 *)child + 0x30) = x;
            *(u8 *)((u8 *)child + 0x55) = 0;
            y = Rand() * 24 + 0x80000;
            ObjectMotion_SetTargetPositionFromMagnitudeAngle(
                (struct ImpactParticle *)child,
                y,
                Rand());
        }
        count--;
    } while (count >= 0);
    return 0;
}
