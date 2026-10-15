#include "gabomba_statue_chamber.h"

extern struct GabombaStatueChamberObject *Data_02002058;
void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void VramBlock_LoadCached(s32 entry, s32 size, void *source);
void Func_02001d90(s32 item);
void Func_02000ef0();

void Func_02000f00(void)
{
    struct GabombaStatueChamberObject *leader = Object_GetById(8);
    s32 x = leader->x;
    s32 z = leader->z;
    /* FAKEMATCH: one zero for the three byte stores, set before the null test, takes r7 as the native does (as constants, differ 20). */
    s32 zero = 0;
    struct GabombaStatueChamberObject *object;

    if ((Data_02002058 = object = Func_02001d28(489, x, 0x120000, z)) != NULL) {
        struct FieldSprite *sprite = object->sprite;
        u8 *block;

        sprite->fullColor = 0;
        sprite->palette = 0;
        sprite->priority = 2;
        object->motionFlags = zero;
        object->field5c = 1;
        object->update = Func_02000ef0;
        sprite->nextFrame = zero;
        sprite->unk1b[0] = zero;
        block = Runtime_AllocateHeapBlockFar(68, 0x608);
        Func_02001d90(230);
        VramBlock_LoadCached(sprite->vramBlock, 128, block + 0x400);
        Runtime_ReleaseHeapBlock(68);
    }
}

void Func_02000f88(void)
{
    struct GabombaStatueChamberObject *leader = Object_GetById(8);
    s32 x = leader->x;
    s32 z = leader->z;
    /* FAKEMATCH: one zero for the three byte stores, set before the null test, takes r7 as the native does (as constants, differ 20). */
    s32 zero = 0;
    struct GabombaStatueChamberObject *object;

    if ((Data_02002058 = object = Func_02001d28(489, x, 0x120000, z)) != NULL) {
        struct FieldSprite *sprite = object->sprite;
        u8 *block;

        sprite->fullColor = 0;
        sprite->palette = 0;
        sprite->priority = 2;
        object->motionFlags = zero;
        object->field5c = 1;
        object->update = Func_02000ef0;
        sprite->nextFrame = zero;
        sprite->unk1b[0] = zero;
        block = Runtime_AllocateHeapBlockFar(68, 0x608);
        Func_02001d90(242);
        VramBlock_LoadCached(sprite->vramBlock, 128, block + 0x400);
        Runtime_ReleaseHeapBlock(68);
    }
}
