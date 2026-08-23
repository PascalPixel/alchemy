#include "scene_effect_sequence.h"
#define NULL ((void *)0)

void *Func_02001422(s32, s32, s32, s32);
void Func_0200146c(void *, s32);
void Func_0200153c(void *, s32);

void *SceneEffect_SpawnSecondary(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Func_02001422(kind, x, y, z);

    if (effect != NULL) {
        u8 *sprite = *(u8 **)(effect + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = sprite[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        sprite[9] = mask;
        effect[0x55] = 0;
        effect[0x59] = 8;
        Func_0200146c(effect, 0);
        Func_0200153c(effect, 15);
        effect[0x23] = (effect[0x23] & 0xfe) | 2;
        return effect;
    }
    return NULL;
}
