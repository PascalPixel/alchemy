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

void Func_080f9010(s32);
u8 *Func_08096c80(s32, s32, s32, s32);
void Func_08009098(void *, const void *);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void Func_08096bec(struct ImpactParticle *, s32, s32);

#define SpawnHeavyImpactEffect Func_08098c08
s32 Func_08098c08(void *source)
{
    struct ImpactPosition position;
    void *first_object;
    void *child;
    register s32 count;
    register s32 speed;
    register struct ImpactPosition *position_pointer;

    Func_080f9010(SOUND_HEAVY_IMPACT);
    position.x = *(s32 *)((u8 *)source + 8);
    position.y = *(s32 *)((u8 *)source + 12);
    position.z = *(s32 *)((u8 *)source + 16);
    first_object = Func_08096c80(
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
        Func_08009098(first_object, Data_0809f0b0);
    }

    position_pointer = &position;
    count = 11;
    speed = 0x10000;
    do {
        child = Func_08096c80(
            0x11D,
            position_pointer->x,
            position_pointer->y,
            position_pointer->z);
        if (child != 0) {
            s32 x;
            s32 y;

            Func_08009098(child, &Data_0809f0d4);
            x = Rand();
            x += speed;
            *(s32 *)((u8 *)child + 0x34) = speed;
            *(s32 *)((u8 *)child + 0x30) = x;
            *(u8 *)((u8 *)child + 0x55) = 0;
            y = Rand() * 24 + 0x80000;
            Func_08096bec(
                (struct ImpactParticle *)child,
                y,
                Rand());
        }
        count--;
    } while (count >= 0);
    return 0;
}
