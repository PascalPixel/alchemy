#include "types.h"

#define StoryActor_AdvanceTimer Func_0200011c
#define StoryActor_ConfigureSpawnedObject Func_0200013c
#define StoryActor_Initialize Func_02000250
typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} StorySpawnRecord;

extern u32 Data_03001e40;
extern u8 Data_0200811d[];
extern u8 Data_020081c5[];

void Func_02004348();
u8 *Func_02004378();
void Func_02004618();
void Func_020043dc();
void Func_0200439c();
s32 Func_020044e4(s32);
s32 Func_020044f0(s32);

void StoryActor_AdvanceTimer(u8 *actor)
{
    u16 *timer = (u16 *)(actor + 0x64);

    /* Arm order decides the branch sense: the reference falls through to the
     * increment and branches away to the call, which is `bgt`. */
    if (*(s16 *)timer <= 0) {
        *timer = (u16)(*timer + 1);
    } else {
        Func_02004348(actor);
    }
}

void StoryActor_ConfigureSpawnedObject(u8 *actor)
{
    s32 fixed_scale;
    u8 *spawned_actor;
    u8 *spawned_record;

    if ((Data_03001e40 & 4) != 0) {
        fixed_scale = 0x14ccc;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    } else {
        fixed_scale = 0x10000;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    }

    if ((Data_03001e40 & 2) == 0) {
        return;
    }

    {
        s32 x = *(s32 *)(actor + 0x08);
        s32 y = *(s32 *)(actor + 0x0c);
        s32 z = *(s32 *)(actor + 0x10);
        spawned_actor = Func_02004378(0x11d, x, y, z);
    }
    Func_02004618(0xf6);
    if (spawned_actor == 0) {
        return;
    }

    {
        u8 *spawned_flags = spawned_actor + 0x55;
        s32 zero_value = 0;

        *spawned_flags = zero_value;
        spawned_record = *(u8 **)(spawned_actor + 0x50);
        ((StorySpawnRecord *)spawned_record)->field = 1;
        Func_020043dc(spawned_actor, 0);
        Func_0200439c(spawned_actor, 1);
        *(u16 *)(spawned_actor + 0x64) = zero_value;
        *(s32 *)(spawned_actor + 0x6c) = (s32)Data_0200811d;
    }
}

s32 StoryActor_Initialize(u8 *actor)
{
    u8 *actor_flags;
    s32 fixed_scale;

    if (Func_020044e4(0x30) != 0) {
        return 0;
    }
    if (Func_020044f0(0x16E) != 0) {
        return 0;
    }
    *(s32 *)(actor + 0x6C) = (s32)Data_020081c5;
    actor_flags = actor + 0x55;
    *actor_flags = 0;
    actor_flags += 0xF;
    *(u16 *)actor_flags = 0;
    actor_flags += 2;
    *(u16 *)actor_flags = 0;
    fixed_scale = 0x8000;
    *(s32 *)(actor + 0x18) = fixed_scale;
    *(s32 *)(actor + 0x1C) = fixed_scale;
    return 0;
}
