#include "TYPES.H"

s32 GameFlag_Test(s32 flag);
void WorldMap_UpdateBobbingMarker(void);

/* ☀️'s: unless either story flag is set, give the actor the bobbing marker's
   update, clear its motion fields and set it to half scale. ⚓️ tests flags
   0x37 and 0x16e. */
s32 StoryActor_Initialize(u8 *actor)
{
    u8 *actor_flags;
    s32 fixed_scale;

    if (GameFlag_Test(0x37) != 0) {
        return 0;
    }
    if (GameFlag_Test(0x16E) != 0) {
        return 0;
    }
    *(s32 *)(actor + 0x6C) = (s32)WorldMap_UpdateBobbingMarker;
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
