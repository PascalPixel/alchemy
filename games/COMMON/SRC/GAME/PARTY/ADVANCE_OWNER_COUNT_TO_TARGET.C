#include "TYPES.H"

struct State_080792fc {
    u8 padding[15];
    u8 count;
};

void *Owner_GetState(s32);
s32 Owner_LevelUp();
void Owner_RecalculateStats(s32);

void Party_AdvanceOwnerCountToTarget(s32 owner, s32 target)
{
    u8 buf[16];
    s32 count = ((struct State_080792fc *)Owner_GetState(owner))->count;

    while (count < target) {
        Owner_LevelUp(owner, buf);
        count++;
    }
    Owner_RecalculateStats(owner);
}
