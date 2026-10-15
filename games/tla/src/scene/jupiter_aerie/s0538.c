#include "jupiter_aerie.h"

s32 Func_02000538(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 513;
    if ((u16)(gPartyState.entrance - 3) <= 1)
        Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
    Func_02000858(0);
    Func_02002b28(15, 1);
    Func_02002b28(16, 1);
    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 64;
    Object_GetById(17)->priorityFlags |= 128;
    Object_GetById(18)->priorityFlags |= 128;
    Object_GetById(19)->priorityFlags |= 128;
    Object_GetById(20)->priorityFlags |= 128;
    Object_GetById(21)->priorityFlags |= 128;
    Object_GetById(22)->priorityFlags |= 128;
    Func_02000e80();
    return 0;
}

void Scene_SetArrivalFlags(s32 flags);
void Func_02001cb4(void);
void Func_02001dbc(s32 actor, s32 mode);
void Func_02002a48(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY);
void Func_02002a50(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX, s32 destY);
void Func_02002a78(s32 x, s32 z, s32 a, s32 b);
void Func_02002ab8(s32 actor, s32 x, s32 z);

// The room's setup: actors 13 and 14 by flags 0xa0a and 0xa0b (with the map
// cells copied when set), then the terrain under each.
s32 Func_02000604(void)
{
    struct ObjectRuntime *object;

    Scene_SetArrivalFlags(0);
    Func_02001cb4();
    if (GameFlag_Test(0xa0a))
    {
        Func_02002ab8(13, 0, 0);
        Func_02002a50(66, 39, 1, 1, 70, 10);
        Func_02002a50(5, 75, 1, 1, 6, 75);
        Func_02002a48(5, 12, 1, 1, 6, 12);
        Func_02002a50(70, 39, 1, 1, 71, 10);
    }
    else
    {
        Func_02001dbc(13, 3);
        Object_GetById(13)->priorityFlags |= 32;
    }
    object = Object_GetById(13);
    object->motionFlags = 0;
    {
        s32 last = 6;

        /* FAKEMATCH: ⚓️ builds the last argument before it loads x and z; plain, it comes between the loads (differ 2 at each call). */
        asm volatile("" : "+l"(last));
        Func_02002a78(object->x, object->z, 0, last);
    }
    if (GameFlag_Test(0xa0b))
    {
        Func_02002ab8(14, 0, 0);
        Func_02002a50(68, 39, 1, 1, 121, 10);
        Func_02002a50(58, 75, 1, 1, 57, 75);
        Func_02002a48(58, 12, 1, 1, 57, 12);
        Func_02002a50(72, 39, 1, 1, 120, 10);
    }
    else
    {
        Func_02001dbc(14, 5);
        Object_GetById(14)->priorityFlags |= 32;
    }
    object = Object_GetById(14);
    object->motionFlags = 0;
    {
        s32 last = 6;

        /* FAKEMATCH: as above. */
        asm volatile("" : "+l"(last));
        Func_02002a78(object->x, object->z, 0, last);
    }
    BYTE_AT(Object_GetById(11), 0x62) = 1;
    BYTE_AT(Object_GetById(12), 0x62) = 1;
    return 0;
}
