#include "types.h"

struct State_080792fc {
    u8 padding[15];
    u8 count;
};

void *Owner_GetState(s32);
s32 Func_0807905c();
void Func_08077428(s32);

void Func_080792fc(s32 owner, s32 target) {
    u8 temporary[16];
    s32 count = ((struct State_080792fc *)Owner_GetState(owner))->count;

    while (count < target) {
        Func_0807905c(owner, temporary);
        count++;
    }
    Func_08077428(owner);
}
