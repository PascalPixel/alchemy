/* Near miss, differ 10 of 223, from the listing: the rooms' second setup.
 * Left: (1) the native loads gPartyState's address before it builds the
 * 0x214 offset (a one-pass block fixes it but pulls the frame's sub sp
 * ahead); (2) it loads the map byte for the mode before it stores the
 * leader's priority flags (a bitfield view of the flags does not separate
 * them); (3) at both terrain calls it builds the 0 argument before it loads
 * x and z (an asm-held zero is copied from r6 instead). alchemy permute for
 * 4 minutes reached 8 only by changing a signature. */
#include "../../../../games/tla/src/scene/alhafran_cave/local.h"
#include "call.h"

void Func_020000fc(void);
void Func_02000220(void);
void Func_02001488(s32 actor, s32 x, s32 z);
void Func_02001578(const void *data);
void Func_02001598(s32 actor, s32 value);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);

void Func_020002f8(void)
{
    struct AlhafranCaveMapWork *map = (struct AlhafranCaveMapWork *)Ram_HeapSlots->mapWork;
    struct ObjectRuntime *object;

    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
    map->mode17 = 1;
    Scheduler_AddOrUpdateCallback(Func_02000220, 0x480);
    if (GameFlag_Test(0x204))
        Func_020000fc();
    object = Object_GetById(8);
    object->motionFlags = 0;
    object->y = 0xc0000;
    object = Object_GetById(9);
    object->motionFlags = 0;
    object->y = 0xc0000;
    object = Object_GetById(10);
    object->motionFlags = 0;
    object->y = 0xc0000;
    Animation_ApplyChildValues(Object_GetById(8), 6);
    Animation_ApplyChildValues(Object_GetById(9), 6);
    Animation_ApplyChildValues(Object_GetById(10), 6);
    object = Object_GetById(12);
    object->motionFlags = 0;
    object->y = 0x100000;
    object = Object_GetById(11);
    object->motionFlags = 0;
    object->y = 0x100000;
    Call2((void (*)())Func_02001598, 13, 0x1000);
    Object_SetModeById(11, 2);
    Object_SetModeById(12, 3);
    Object_SetModeById(13, 2);
    ObjectMotion_SetActionVariant(11, 2);
    ObjectMotion_SetActionVariant(12, 2);
    ObjectMotion_SetActionVariant(13, 3);
    Object_GetById(11)->priorityFlags |= 2;
    Object_GetById(12)->priorityFlags |= 2;
    Object_GetById(13)->priorityFlags |= 2;
    Animation_ApplyChildValues(Object_GetById(11), 6);
    Animation_ApplyChildValues(Object_GetById(12), 6);
    Animation_ApplyChildValues(Object_GetById(13), 6);
    if (Value1((s32 (*)())GameFlag_Test, 0x8a7)) {
        Call3((void (*)())Func_02001488, 14, 0xc80000, 0x880000);
        Call3((void (*)())Func_02001488, 15, 0x1880000, 0x1680000);
    }
    object = Object_GetById(14);
    object->motionFlags = 0;
    object->terrainHeight = 0;
    object->y = 0;
    Func_02001420_(object->x, object->z, 0, 4);
    object = Object_GetById(15);
    object->motionFlags = 0;
    object->terrainHeight = 0;
    object->y = 0;
    Func_02001420_(object->x, object->z, 0, 4);
    Func_02001578(gData_02001608);
    Object_SetActionCallback(Object_GetById(16), 9);
    if (!Value1((s32 (*)())GameFlag_Test, 0x8a7)) {
        Func_02001408_(0, 58, 12, 7, 13, 58);
        Func_02001408_(0, 58, 2, 9, 25, 66);
        Func_02001408_(0, 58, 10, 5, 15, 74);
    }
}
