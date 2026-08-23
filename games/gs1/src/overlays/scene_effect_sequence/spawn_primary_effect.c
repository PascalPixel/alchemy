#include "scene_effect_sequence.h"
#define NULL ((void *)0)

void *Func_020013ca(s32, s32, s32, s32);
void Func_02001410(void *, s32);
void Func_02001428(void *, s32);
void Func_020014e0(void *, s32);

void *SceneEffect_SpawnPrimary(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Func_020013ca(kind, x, y, z);

    if (effect != NULL) {
        u8 *sprite = *(u8 **)(effect + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = sprite[9];
        mask = -mask;
        mask &= flags;
        sprite[9] = mask;
        effect[0x55] = 0;
        effect[0x59] = 8;
        Func_02001410(effect, 0);
        Func_020014e0(effect, 14);
        Func_02001428(effect, 1);
        return effect;
    }
    return NULL;
}
