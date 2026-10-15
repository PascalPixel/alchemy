#include "airs_rock_inside.h"

struct FieldActor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void               Func_02005e84(s32 owner, s32 a, s32 b);
void               Func_02005efc(s32 countdown);

void Func_02003ac4(void)
{
    Func_02005e14();
    Func_02005f2c(0);
    OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 286);
    Func_02005e84(gPartyState.currentOwner, 6, 0);
    Func_02005efc(3);
    Func_02005e1c();
}
