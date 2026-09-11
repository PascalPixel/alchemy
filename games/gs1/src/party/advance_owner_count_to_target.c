#include "types.h"
#include "scene.h"

struct State_080792fc {
    u8 padding[15];
    u8 count;
};

void *Owner_GetState(s32);

void Party_AdvanceOwnerCountToTarget(s32 owner, s32 target)
{
    u8 buf[16];
    s32 count = ((struct State_080792fc *)Owner_GetState(owner))->count;

    while (count < target) {
        Party_Check(owner, buf);
        count++;
    }
    Party_Do(owner);
}
