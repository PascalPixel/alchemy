#include "scene.h"

void Func_02001518(void)
{
    struct FieldActor *object = Object_GetById(64);
    struct FieldActor *leader;
    s32                a;
    s32                b;

    if (object != 0)
    {
        leader = Object_GetById(gPartyState.currentOwner);
        Scene_Call4090(64, 0x1200000, 0xf00000);
        /* FAKEMATCH: both coordinates are read before 0x80000 is built, as the native reads them; in one expression the constant comes between the two reads */
        a = leader->x;
        b = object->x;
        if ((a + 0x80000) >> 20 == (b + 0x80000) >> 20
            && (leader->z + 0x80000) >> 20 == (object->z + 0x80000) >> 20)
            Scene_Call4090(gPartyState.currentOwner, 0x1200000, 0x1000000);
    }
}
