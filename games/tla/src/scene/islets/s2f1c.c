#include "islets.h"

void Func_02002d0c(s32 id);
void Func_02002d7c(void);
void Func_02003684(s32 actor);
void Func_02003ee4(s32 actor, s32 x, s32 z, s32 angle);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *script);
void Object_LinkObjectAndSetCallback(s32 actor, s32 other);
extern const u8 gData_02004274[];

// The room's setup: flags, the objects' places and the scene state.
void Func_02002f1c(void)
{
    struct ObjectRuntime *object;
    struct ObjectRuntime *other;
    u16 *state;
    u16 *next;
    s32 angle;
    s32 i;

    Object_GetById(gPartyState.currentOwner)->priorityFlags |= 64;
    if (gPartyState.entrance == 2)
        GameFlag_ClearBit(0x12f);
    i = 11;
    do {
        Func_02002d0c(i++);
    } while (i <= 18);
    state = gSceneState;
    next = &gSceneState[1];
    if (!GameFlag_Test(0x109)) {
        *state = 0;
        *next = 0;
    }
    Scheduler_AddOrUpdateCallback(Func_02002d7c, 0x480);
    if (GameFlag_Test(0xa2c)) {
        object = Object_GetById(10);
        Call3((void (*)())Func_02003edc, 10, 0xb00000, 0xa80000);
        Object_SetModeById(10, 4);
        object->priorityFlags |= 2;
        object->motionFlags = 0;
        object->y = -0x80000;
        object->terrainHeight = -0x80000;
        Func_02003e24(9, 1, 4, 1, 9, 10);
        Func_02003e24(9, 64, 4, 1, 9, 74);
    } else {
        Func_02003684(10);
    }
    if (!GameFlag_Test(0xaa2)) {
        Object_LinkObjectAndSetCallback(9, 19);
        Object_SetModeById(19, 0);
        Object_GetById(19)->actionFlags &= ~1;
    } else if (!GameFlag_Test(0x109)) {
        other = Object_GetById(9);
        other->angle = 0x5000;
        other->collisionFlags |= 0x80;
        /* FAKEMATCH: the angle in a temporary is built before the position (differ 4 as a constant argument). */
        angle = 0x3000;
        Func_02003ee4(19, 0xea0000, 0xcc0000, angle);
        Object_GetById(19)->collisionFlags |= 0x80;
        Call3((void (*)())ObjectMotion_SetSpeedParameters, 9, 0x20000, 0x10000);
        Call3((void (*)())ObjectMotion_SetSpeedParameters, 19, 0x19999, 0xcccc);
        ObjectMotion_EnableActionAndSetCallback(9, gData_02004274);
        ObjectMotion_EnableActionAndSetCallback(19, gData_02004274);
        GameFlag_SetBit(0x200);
    }
}

void Func_020030b4(void)
{
    struct ObjectRuntime *spawned;

    if (gPartyState.entrance == 3)
        GameFlag_ClearBit(0x12f);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    Object_SetModeById(10, 4);
    if (GameFlag_Test(0xaa3)) {
        spawned = (struct ObjectRuntime *)Value4((s32 (*)())Func_02003df4, 489, 0x680000, 0x180000, 0xa00000);
        if (spawned != 0) {
            spawned->motionFlags = 0;
            Func_02003e3c(spawned, 453);
            ObjectDispatch_SetSingleChildFlags(spawned, 0);
            spawned->unk5c[0] = 1;
        }
        Call3((void (*)())Func_02003edc, 10, 0x680000, 0xcc0000);
    }
    ObjectMotion_SetActionVariant(11, 1);
    Object_SetPartAttribute(Object_GetById(11), 15);
    Object_GetById(11)->priorityFlags |= 2;
}
