#include "kibombo.h"

void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void  Runtime_ReleaseHeapBlock(s32 slot);
void  Func_02005a84(s32 resource);
void  VramBlock_LoadCached(s32 handle, s32 size, void *source);

void Func_02002de0(void)
{
    struct KibomboObject **slot = &gData_02007950;
    /* FAKEMATCH: the native keeps one zero in r7 for the three byte stores below; plain zeros are built in r2 where they are stored (differ 30). */
    s32                   zero = 0;
    struct KibomboObject *object = Func_02005a34(0x1e9, 0x1090000, 0x240000, 0x1580000);

    *slot = object;
    if (object != 0)
    {
        struct FieldSprite *sprite = object->sprite;
        u8                 *buffer;

        sprite->fullColor = 0;
        sprite->palette = 0;
        sprite->priority = 1;
        object->motionFlags = zero;
        object->field5c = 1;
        sprite->nextFrame = zero;
        sprite->unk1b[0] = zero;
        buffer = Runtime_AllocateHeapBlockFar(68, 0x608);
        Func_02005a84(242);
        VramBlock_LoadCached(sprite->vramBlock, 128, buffer + 0x400);
        Runtime_ReleaseHeapBlock(68);
    }
}

void Func_02002e60(void)
{
    struct KibomboObject *leader = Object_GetById(gPartyState.currentOwner);

    /* FAKEMATCH: the loop note ends the scheduling block: ⚓️ loads the leader's height before it builds the limit (2 differ without it). */
    do
    {
    } while (0);
    if (leader->y > 0x100000)
    {
        Func_02005b0c(29, 0, 0);
        ObjectMotion_SetActionVariant(30, 2);
    }
    else
    {
        Kibombo_ObjSetPos(Object_GetById(29), 0x780000, 0, 0xa80000);
        ObjectMotion_SetActionVariant(30, 3);
    }
}

void Func_02002eb0(void)
{
    if (gData_02007950 != 0)
    {
        struct KibomboObject *object = Object_GetById(27);

        Object_SetPositionAndResetMotion(gData_02007950, object->x, object->y + 0xc0000, object->z);
    }
}
