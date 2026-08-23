#include "staged_actor_pair_scene.h"
#define NULL ((void *)0)

void *Func_0200150e(s32, s32, s32, s32);
void Func_0200155c(void *, s32);
void Func_02001574(void *, s32);
void Func_0200160c(void *, s32);

void *StagedActorPairScene_SpawnPrimaryEffect(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *effect = Func_0200150e(kind, x, y, z);

    if (effect != NULL) {
        u8 *sprite = *(u8 **)(effect + 0x50);
        s32 sprite_flags;
        s32 flag_mask = 13;

        sprite_flags = sprite[9];
        flag_mask = -flag_mask;
        flag_mask &= sprite_flags;
        sprite[9] = flag_mask;
        effect[0x55] = 0;
        effect[0x59] = 8;
        Func_0200155c(effect, 0);
        Func_0200160c(effect, 14);
        Func_02001574(effect, 1);
        return effect;
    }
    return NULL;
}
