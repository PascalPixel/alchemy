/*
 * Draft of Func_02000c38 (overlay 6A9, 02008c38): differ 20, one swap: ⚓️ keeps
 * the position in r6 and the motion flags' address in r7.
 */
#include "../../../../games/tla/src/field/overlay_6a9/ov6a9.h"

s32 Func_02000c38(struct FieldActor *object, s16 *position);

struct FieldActor *Func_020000d0(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

    if (object != NULL) {
        Object_SetSpritePriority(object, 1);
        object->motion_flags = 0;
        object->collision_flags = 8;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Object_SetPartAttribute(object, 15);
        object->priority_flags |= 0x22;
        return object;
    }
    return NULL;
}

const u8 *Func_02000344(void)
{
    const u8 *result = NULL;

    if (gPartyState.scene == (s32)&SceneId_Ov6a9Room2)
        result = Data_02001264;
    return result;
}

const u8 *Func_02000374(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov6a9Room1)
        return Data_02001358;
    if (scene == (s32)&SceneId_Ov6a9Room4)
        return Data_020013b8;
    return Data_02001328;
}

s32 Func_020003b4(u32 layer, s32 x, s32 y)
{
    struct Ov6a9MapWork *map = (struct Ov6a9MapWork *)Ram_HeapSlots->map_work;
    u8 *cell;

    if (map == NULL)
        return 0;
    if (layer <= 2)
        cell = map->layers[layer].cells;
    else
        cell = gMapCellBuffer;
    cell += (x + (y << 7)) << 2;
    return (cell[2] << 8) | cell[3];
}

s32 Func_020003f0(u32 layer, s32 x, s32 y, s32 value)
{
    struct Ov6a9MapWork *map = (struct Ov6a9MapWork *)Ram_HeapSlots->map_work;
    u8 *cell;
    s32 old;

    if (map == NULL)
        return 0;
    if (layer <= 2)
        cell = map->layers[layer].cells;
    else
        cell = gMapCellBuffer;
    cell += (x + (y << 7)) << 2;
    old = (cell[2] << 8) | cell[3];
    cell[2] = value >> 8;
    cell[3] = value;
    return old;
}

void Func_020007ec(struct FieldActor *object)
{
    s32 dx = WORD_AT(object, 68);
    s32 dy = WORD_AT(object, 72);
    s32 dz = WORD_AT(object, 76);

    object->x += dx;
    object->y += dy;
    object->z += dz;
    WORD_AT(object, 68) = dx - dx / 12;
    WORD_AT(object, 76) = dz - dz / 10;
    WORD_AT(object, 24) += WORD_AT(object, 48);
    WORD_AT(object, 28) += WORD_AT(object, 52);
    object->sprite->rotation += object->timer;
}

void Func_02000848(struct FieldActor *object)
{
    struct FieldActor *leader = Object_GetById(gPartyState.current_owner);

    u8 *flags = &leader->priority_flags;

    if (leader->z > object->z)
        *flags |= 4;
    else
        *flags &= ~6;
}

const u8 *Func_02000ad0(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov6a9Room1)
        return Data_0200149c;
    if (scene == (s32)&SceneId_Ov6a9Room3)
        return Data_020014d8;
    if (scene == (s32)&SceneId_Ov6a9Room4)
        return Data_020014fc;
    return Data_02001430;
}

s32 Func_02000b20(void)
{
    s16 *scene;

    WORD_AT(Ram_HeapSlots->event_work, 428) = 516;
    scene = (s16 *)((u8 *)&gPartyState + 480);
    if (*scene == (s32)&SceneId_Ov6a9Room1)
        Func_02000ba8();
    if (*scene == (s32)&SceneId_Ov6a9Room4)
        Func_02000f98();
    return 0;
}

s32 Func_02000b64(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov6a9Room2)
        Func_02000d10();
    else if (scene == (s32)&SceneId_Ov6a9Room3)
        Func_02000ea0();
    else if (scene == (s32)&SceneId_Ov6a9Room4)
        Func_02000f50();
    return 0;
}

void Func_02000ba8(void)
{
    struct FieldActor *object;
    s32 x;

    Object_GetById(4)->priority_flags |= 0x20;
    Func_0200114c();
    Func_02001154(0, 512, 8, 9);
    object = Object_GetById(10);
    object->motion_flags = 0;
    x = object->x >> 20;
    Call6((void (*)())Func_020010ac, 16, 10, 1, 1, x, 10);
}

void Func_02000bf8(s32 unused, s32 id)
{
    struct FieldActor *object = Object_GetById(id);

    object->motion_flags = 0;
    Call6((void (*)())Func_020010ac, 13, 12, 2, 1, 13, 10);
    Call6((void (*)())Func_020010ac, 16, 10, 1, 1, object->x >> 20, 10);
}

s32 Func_02000c38(struct FieldActor *object, s16 *position)
{
    u8 *flags = &object->motion_flags;
    u8 saved = *flags;

    if (Func_020010b4(object) == 0) {
        Func_020010dc();
        Func_0200113c(0);
        Func_02001074(object, 6);
        WaitFrames(6);
        Func_02001164(152);
        Func_02001074(object, 7);
        WORD_AT(object, 48) = 0x30000;
        WORD_AT(object, 52) = 0x20000;
        object->velocity_y = 0x40000;
        *flags &= 0x7e;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Call3((void (*)())ObjectMotion_SetPositionAndCommit, gPartyState.current_owner, position[1], position[5]);
        Func_02001074(object, 6);
        ObjectDispatch_SetSingleChildFlags(object, 1);
        *flags = saved;
        Func_020010e4();
        return 1;
    }
    return 0;
}
