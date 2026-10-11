#include "ov670.h"
#include "input.h"

void Func_02003ac8(void)
{
    struct Ov670Object *leader = Object_GetById(gPartyState.current_owner);
    s32 x = leader->x >> 20;
    s32 z = leader->z >> 20;

    if (x == 42 && z == 23 && (gInput.held & 64) == 0)
        return;
    if (x == 43 && z == 22 && (gInput.held & 32) == 0)
        return;
    Func_02003a08(680, 352);
}

void Func_02003b20(void)
{
    struct Ov670Object *leader = Object_GetById(gPartyState.current_owner);
    s32 x = leader->x >> 20;
    s32 z = leader->z >> 20;

    if (z == 38) {
        if (x == 23 && (gInput.held & 16) == 0)
            return;
        if (x == 25 && (gInput.held & 32) == 0)
            return;
    } else if (x == 24 && z == 39 && (gInput.held & 64) == 0) {
        return;
    }
    Func_02003a08(392, 608);
}

void Func_02003b88(void)
{
    struct Ov670Object *leader = Object_GetById(gPartyState.current_owner);
    s32 x = leader->x >> 20;
    s32 z = leader->z >> 20;

    if (z == 54) {
        if (x == 42 && (gInput.held & 16) == 0)
            return;
        if (x == 44 && (gInput.held & 32) == 0)
            return;
    } else if (x == 43 && z == 55 && (gInput.held & 64) == 0) {
        return;
    }
    Func_02003a08(696, 864);
}

void Func_02003bf0(void)
{
    if (GameFlag_Test(0x202))
        Func_02003898();
    Func_02003a08(376, 288);
}
