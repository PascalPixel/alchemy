#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "fixed_math.h"

struct BurstParticleVector
{
    s32 values[3];
};

extern u8 gData_080f0ee8[];

void  Func_080ddbd8(void);
void  Audio_PlayCue(s32 no);
void  Vector_AddPolarOffset(s32 magnitude, s32 angle, struct BurstParticleVector *position);
void *Object_Spawn(s32 id, s32 x, s32 y, s32 z);
void  ObjectDispatch_InitializeFar(void *object, u32 script);
void  BattleFx_PrepareBufferInterpolation(void);

void Func_080ddb3c(void)
{
    u8                         *state = Ram_HeapSlots->effectWork;
    struct BurstParticleVector  position;
    struct BurstParticleVector *p;
    s32                         entryCount;

    Func_080ddbd8();
    Audio_PlayCue(134);
    p = &position;
    entryCount = 4;
    do
    {
        void *object;
        s32   randomValue;

        p->values[0] = *(s32 *)(state + 4);
        p->values[2] = *(s32 *)(state + 12);
        randomValue = (Random16() * 6) + 0x40000;
        Vector_AddPolarOffset(randomValue, Random16(), p);
        p->values[1] = *(s32 *)(state + 8);
        object = Object_Spawn(0x119, p->values[0], p->values[1], p->values[2]);
        if (object != 0)
        {
            ObjectDispatch_InitializeFar(object, (u32)gData_080f0ee8);
            *((u8 *)object + 0x55) = 2;
        }
        WaitFrames((((u32)Random16() * 2) >> 16) + 2);
        entryCount--;
    } while (entryCount >= 0);
    WaitFrames(30);
    BattleFx_PrepareBufferInterpolation();
}
