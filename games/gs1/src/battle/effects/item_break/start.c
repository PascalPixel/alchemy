#include "fixed_math.h"
#include "types.h"
#include "object_efx.h"
#include "sound_ids.h"

extern void *Object_Spawn(s32, s32, s32, s32);
extern void Object_SetVelocity(struct Object_08096bec *object, s32 arg1, s32 arg2);
extern void Object_SetMode(void *, s32);
extern void Object_SetCallback(void *, void *);
extern void BattleEffect_UpdateItemBreakFragment(void *);
extern u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
extern void Audio_PlayCue(s32);

void *BattleEffect_StartItemBreak(void *source)
{
    s32 angle;
    s32 fragment_height;
    s32 fragment_scale;
    s32 fragment_count;
    u32 horizontal_velocity;
    u32 rotation_jitter;
    s32 zero;
    void *parent;
    void *child;

    angle = (*(u16 *)((s8 *)source + 6) + 0x2000) & 0xc000;
    parent = Object_Spawn(0xd7, *(s32 *)((s8 *)source + 8),
                           *(s32 *)((s8 *)source + 12) + 0x100000,
                           *(s32 *)((s8 *)source + 16));
    if (parent == 0)
        return 0;
    *(s32 *)((s8 *)parent + 0x1c) = 0x4000;
    *(s32 *)((s8 *)parent + 0x18) = 0x4000;
    *(s32 *)((s8 *)parent + 0x6c) = (s32)BattleEffect_UpdateItemBreakFragment;
    *(s32 *)((s8 *)parent + 0x30) = 0x20000;
    *(s32 *)((s8 *)parent + 0x34) = 0x20000;
    zero = 0;
    *(s8 *)((s8 *)parent + 0x55) = zero;
    Object_SetMode(parent, 3);
    Object_SetVelocity(parent, 0x100000, angle);

    fragment_count = 7;
    do {
        child = Object_Spawn(0x11d, *(s32 *)((s8 *)source + 8),
                              *(s32 *)((s8 *)source + 12) + 0x100000,
                              *(s32 *)((s8 *)source + 16));
        if (child != 0) {
            Object_SetCallback(child, &Data_0809f0d4);
            fragment_scale = Rand() + 0x10000;
            *(s32 *)((s8 *)child + 0x34) = 0x10000;
            *(s32 *)((s8 *)child + 0x30) = fragment_scale;
            *(s8 *)((s8 *)child + 0x55) = 2;
            *(s32 *)((s8 *)child + 0x48) = 0x51e;
            horizontal_velocity = Rand();
            *(s32 *)((s8 *)child + 0x28) = horizontal_velocity - Rand();
            fragment_height = Rand() * 0x18 + 0x80000;
            rotation_jitter = Rand();
            Object_SetVelocity(child, fragment_height,
                          ((rotation_jitter - Rand()) >> 3) +
                          *(u16 *)((s8 *)source + 6));
        }
        fragment_count--;
    } while (fragment_count >= 0);
    Audio_PlayCue(SOUND_ITEM_BREAK);
    return parent;
}
