#include "types.h"

#define Party_AdvanceOwnerCountToTarget Func_080792fc

struct State_080792fc {
    u8 padding[15];
    u8 count;
};

void *Owner_GetState(s32);
s32 Func_0807905c();
void Func_08077428(s32);

void Party_AdvanceOwnerCountToTarget(s32 owner, s32 target) {
    u8 buf[16];
    s32 count = ((struct State_080792fc *)Owner_GetState(owner))->count;

    while (count < target) {
        Func_0807905c(owner, buf);
        count++;
    }
    Func_08077428(owner);
}
