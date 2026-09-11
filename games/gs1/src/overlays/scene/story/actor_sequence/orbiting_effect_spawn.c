#include "types.h"

#define EffectDescriptorTable Data_02009be0
#define GetPartyEffect Func_02001b5e
#define SpawnEffect Func_02001afc
#define SetEffectVariant Func_02001b06
#define SetEffectDescriptor Func_02001b20
#define SetEffectMode Func_02001c82
#define ScaleEffectDeltaFromAccumulated Func_02001b94
#define ScaleEffectDeltaFromOrigin Func_02001bac
#define ScaleEffectVerticalDelta Func_02001bba
#define SetEffectCallbackMode Func_02001c20
#define SetEffectCallbackArgument Func_02001c30
#define SpawnConfiguredEffect Func_0200013c

#include "configured_effect_spawn.h"

s32 Func_02003122(s32);
s32 Func_02003138(s32);
s32 Func_02003152(s32);
s32 Func_02003150(void);
s32 Func_02003156(void);

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

s32 Func_020017d8(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Func_02003122(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Func_02003138(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Func_02003152(p[12] + 0x8000) / 8;
    a = Func_02003150();
    b = Func_02003156();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}
