/* First draft, differ 4: the listing loads the owner's y and z (ldr r3, [r7, #12/#16]) before the 31 and 127 masks; this draft loads each mask first. Tried: the sum either way round, the random value as a local, % for &, * 0x10000, unsigned fields, and two minutes of alchemy permute (best differ 8). */
#include "types.h"
#include "party_state.h"

struct Actor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x50];
    s16 motion;
    s16 previous_motion;
};

extern struct Actor *Object_GetById(s32 id);
extern u32 Random16Far(void);
extern void Func_02005d5c(struct Actor *actor, s32 motion);

void Func_020004d4(struct Actor *actor)
{
    struct Actor *owner = Object_GetById(gPartyState.current_owner);

    if (Random16Far() & 0xff)
    {
        actor->x = ((Random16Far() & 0xff) << 16) + (176 << 16);
        { s32 r = Random16Far(); actor->y = owner->y + ((r & 31) << 16); }
        { s32 r = Random16Far(); actor->z = owner->z + ((r & 127) << 16); }
    }
    actor->previous_motion = actor->motion;
    if (actor->motion == 212)
        Func_02005d5c(actor, 9);
    else
        Func_02005d5c(actor, 10);
}
