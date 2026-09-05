#include "fixed_math.h"
#include "types.h"
#include "sound_ids.h"

struct BurstParticleVector {
    s32 values[3];
};

extern u8 *Data_03001f30;
void Func_08098698(void);
void Audio_PlayCue(s32);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void RotateVectorByMagnitude(s32, s32, struct BurstParticleVector *);
void *Object_Spawn(s32, s32, s32, s32);
void Object_SetCallback(void *, const void *);
void WaitFrames(u32);
void BattleEffect_PrepareBufferInterpolation(void);
extern const u8 Data_0809f11c[];

void RunBurstParticleEffect(void)
{
    u8 *state = Data_03001f30;
    struct BurstParticleVector position;
    struct BurstParticleVector *positionPointer;
    s32 entry_count;

    Func_08098698();
    Audio_PlayCue(SOUND_HEAVY_IMPACT);
    positionPointer = &position;
    entry_count = 4;
    do {
        void *object;
        s32 random_value;

        positionPointer->values[0] = *(s32 *)(state + 4);
        positionPointer->values[2] = *(s32 *)(state + 12);
        random_value = (Rand() * 6) + 0x40000;
        RotateVectorByMagnitude(random_value, Rand(), positionPointer);
        positionPointer->values[1] = *(s32 *)(state + 8);
        object = Object_Spawn(
            0xD9,
            positionPointer->values[0],
            positionPointer->values[1],
            positionPointer->values[2]
        );
        if (object != 0) {
            Object_SetCallback(object, Data_0809f11c);
            *((u8 *)object + 0x55) = 2;
        }
        WaitFrames((((u32)Rand() * 2) >> 16) + 2);
        entry_count--;
    } while (entry_count >= 0);
    WaitFrames(0x1E);
    BattleEffect_PrepareBufferInterpolation();
}
