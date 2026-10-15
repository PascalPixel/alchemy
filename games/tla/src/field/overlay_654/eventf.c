#include "shrine_of_the_sea_god.h"
#include "scene_ids.h"

struct KabeSpot {
    s32 x;
    s32 z;
    s32 field_18;
    s32 rotation;
};

extern const struct KabeSpot Data_02003cc0[];
extern const u8 Data_02003cbc[];
extern const u8 Data_02003c10[];

void Object_SetPartAttribute(struct KabeObject *object, s32 value);
void Func_02003b00(s32 value);
void Func_02003b18(void);
void Func_02003b20(s32 a, s32 b, s32 c, s32 d);
void Func_02003b30(const void *script);
void Func_02003854(s32 actor);
void Func_0200123c(void);
void Func_020010e8(void);
void Func_02001088(void);
void Func_02001128(void);
void Func_020011a0(void);
void Func_02000f18(s32 actor);
void Func_020027f8(void);
void Func_02002830(void);
void Func_0200291c(void);
void Func_0200296c(void);
void Func_020029a4(void);
void Func_020029dc(void);
void Func_02002a2c(void);
void Func_02002b18(void);
void Func_02002b50(void);
void Func_02002c18(void);
void Func_02002c50(void);
void Func_02002c88(void);
void Func_02003094(void);
void Func_02003134(void);
void Func_0200320c(void);
void Func_02003354(void);
void Func_0200338c(void);
void Func_02003504(void);
void Func_02003598(void);
void Func_02003740(void);
void Func_0200377c(void);
void Func_0200380c(void);

s32 Func_02002ec4(void)
{
    s16 scene;

    ((struct KabeEventWork *)Ram_HeapSlots->event_work)->screen_effect = 0x204;
    scene = gPartyState.scene;
    if (scene == (s32)&SceneId_Kabe1)
        Func_02003094();
    else if (scene == (s32)&SceneId_Kabe2)
        Func_02003134();
    else if (scene == (s32)&SceneId_Kabe3)
        Func_0200320c();
    else if (scene == (s32)&SceneId_Kabe4)
        Func_02003354();
    else if (scene == (s32)&SceneId_Kabe5)
        Func_0200338c();
    else if (scene == (s32)&SceneId_Kabe6)
        Func_02003504();
    else if (scene == (s32)&SceneId_Kabe7)
        Func_02003598();
    else if (scene == (s32)&SceneId_Kabe8)
        Func_02003740();
    else if (scene == (s32)&SceneId_Kabe9)
        Func_0200377c();
    else if (scene == (s32)&SceneId_Kabe11)
        Func_0200380c();
    return 0;
}

struct KabeObject *Func_02002f88(s32 id)
{
    struct KabeObject *object = Object_GetById(id);

    object->priority_flags |= 2;
    object->y = 0xc0000;
    Func_02003aa0(id);
    return object;
}

void Func_02002fb0(s32 id)
{
    Func_02003aa0(id);
    Object_SetPartAttribute(Object_GetById(id), 15);
    ObjectMotion_SetActionVariant(id, 1);
    Object_GetById(id)->priority_flags |= 2;
}

void Func_02002fe0(void)
{
    if (GameFlag_Test(265)) {
        if (GameFlag_Test(0x208) == 0)
            Func_0200123c();
    } else {
        GameFlag_SetBit(0x208);
    }
}

void Func_0200300c(void)
{
    u32 i;

    for (i = 0; i <= 2; i++)
        Func_02002fb0(i + 9);
    Func_02002fe0();
}

void Func_02003024(void)
{
    const struct KabeSpot *spot = Data_02003cc0;
    u32 i;

    /* FAKEMATCH: loads the spot table before it clears the counter. */
    __asm__("");
    for (i = 0; i <= 15; i++) {
        s32 id = i + 8;
        struct KabeObject *object = Object_GetById(id);
        struct KabeSprite *sprite;

        Func_02003aa0(id);
        Object_SetModeById(id, 0);
        ObjectMotion_SetActionVariant(id, 3);
        object->priority_flags |= 2;
        Object_SetPartAttribute(Object_GetById(id), 12);
        sprite = object->sprite;
        object->x = spot->x;
        object->z = spot->z;
        object->field_18 = spot->field_18;
        sprite->rotation = spot->rotation;
        spot++;
    }
}

void Func_02003094(void)
{
    struct KabeMapWork *map = (struct KabeMapWork *)Ram_HeapSlots->map_work;

    Func_02003854(8);
    Func_02003854(9);
    {
        s32 priority;

        map->mode_17 = 1;
        priority = 0x480;
        Scheduler_AddOrUpdateCallback(Func_020010e8, priority);
    }
    Func_02003b18();
    Func_02003b20(0, 515, 10, 11);
    if (GameFlag_Test(0x872)) {
        Func_02003a30(12, 0, 0);
        Func_02003a30(13, 0, 0);
    }
}

void Func_020030fc(void)
{
    struct KabeObject *leader = Object_GetById(gPartyState.current_owner);
    struct KabeObject *object = Object_GetById(11);
    s32 x = object->x;
    u8 *flags = &object->collision_flags;

    if (x > leader->x)
        /* FAKEMATCH: the native ties the flag or to the constant's register. */
        *flags = *flags + 8 - (*flags & 8);
    else
        *flags &= ~8;
}

void Func_02003134(void)
{
    s32 angle;

    Func_02002f88(10)->y = -0x100000;
    ObjectMotion_SetActionVariant(10, 3);
    Func_02003b30(Data_02003cbc);
    Func_02002e70();
    Kabe_TaskAdd(Func_020011a0, 0x480);
    Func_02003aa0(11);
    Func_02003aa0(12);
    Object_GetById(12)->motion_flags = 0;
    Kabe_BlendStart(3, 15, 1, 1, 6, 15);
    if (GameFlag_Test(0x873)) {
        Func_02003a30(11, 0, 0);
    } else if (GameFlag_Test(0x872)) {
        /* FAKEMATCH: the function-scope angle temporary preserves its early constant construction; a direct argument through the unbound helper differs 2. */
        angle = 0x8000;
        Kabe_ObjPlaceTurn(11, 328 << 16, 200 << 16, angle);
        WaitFrames(1);
        ObjectMotion_EnableActionAndSetCallback(11, Data_02003c10);
        ObjectDispatch_SetSingleChildFlags(Object_GetById(11), 1);
        Object_GetById(11)->collision_flags = 1;
    }
    /* FAKEMATCH: the native builds the priority before it loads the callback. */
    KABE_TASK_ADD_LATE(Func_020030fc, 0x480);
}

void Func_0200320c(void)
{
    struct KabeObject *block;
    s32 angle;

    Func_02002f88(9);
    Func_02002f88(10);
    Func_02002f88(11);
    Func_02002f88(12);
    Func_02002f88(13);
    block = Func_02002f88(14);
    Object_SetModeById(9, 0);
    Object_SetModeById(13, 0);
    Object_SetModeById(14, 2);
    ObjectMotion_SetActionVariant(9, 1);
    ObjectMotion_SetActionVariant(10, 1);
    ObjectMotion_SetActionVariant(11, 1);
    ObjectMotion_SetActionVariant(12, 1);
    ObjectMotion_SetActionVariant(13, 1);
    ObjectMotion_SetActionVariant(14, 1);
    Kabe_TaskAdd(Func_02001088, 0x480);
    if (GameFlag_Test(0x230)) {
        Kabe_ObjSetPos(block, 408 << 16, -0xd0000, 336 << 16);
        ((struct KabeSprite *)block->sprite)->rotation = 0x1000;
        ObjectMotion_SetActionVariant(14, 3);
        block->priority_flags |= 2;
        Kabe_BlendStart(0, 50, 3, 1, 24, 70);
    }
    if (GameFlag_Test(0x84c))
        Kabe_ObjSetPos(Object_GetById(8), 200 << 16, 0, 296 << 16);
    Func_02003b30(Data_02003cbc);
    if (GameFlag_Test(0x875))
        Func_02003a30(15, 0, 0);
    else if (GameFlag_Test(0x874)) {
        /* FAKEMATCH: the function-scope angle temporary preserves its early constant construction; a direct argument through the unbound helper differs 4. */
        angle = 0x4000;
        Kabe_ObjPlaceTurn(15, 580 << 16, 344 << 16, angle);
    }
    Func_02003b00(0);
    WaitFrames(1);
}

void Func_02003354(void)
{
    struct KabeMapWork *map = (struct KabeMapWork *)Ram_HeapSlots->map_work;

    if (GameFlag_Test(0x876) == 0)
        Func_02003024();
    {
        s32 priority;

        map->mode_17 = 1;
        priority = 0x480;
        Scheduler_AddOrUpdateCallback(Func_020010e8, priority);
    }
}

void Func_0200338c(void)
{
    s32 angle;
    struct KabeObject *object = Func_02002f88(13);
    u8 *flags = &object->priority_flags;

    if ((u16)(gPartyState.entrance - 2) <= 1) {
        ObjectMotion_SetActionVariant(13, 3);
        /* FAKEMATCH: the native ties the flag or to the constant's register. */
        *flags = *flags + 2 - (*flags & 2);
    } else {
        ObjectMotion_SetActionVariant(13, 1);
        *flags &= ~2;
    }
    Func_02000f18(10);
    Func_02000f18(11);
    Func_02000f18(12);
    if (GameFlag_Test(0x200))
        Kabe_BlendStart(0, 0, 1, 1, 8, 18);
    if (GameFlag_Test(0x201))
        Kabe_BlendStart(0, 0, 1, 1, 10, 18);
    if (GameFlag_Test(0x202))
        Kabe_BlendStart(0, 0, 1, 1, 10, 17);
    Kabe_TaskAdd(Func_02001128, 0x480);
    Func_02003b30(Data_02003cbc);
    Func_02002e70();
    Kabe_TaskAdd(Func_020011a0, 0x480);
    if (GameFlag_Test(115) || GameFlag_Test(0x340)) {
        GameFlag_SetBit(0x205);
    } else if (GameFlag_Test(0x87c)) {
        struct KabeObject *block = Object_GetById(14);
        s32 moved = GameFlag_Test(265);

        if (moved == 0) {
            block->state = moved;
            Kabe_ObjPlace(14, 392 << 16, 296 << 16);
        }
        Object_LinkObjectAndSetCallback(14, gPartyState.current_owner);
    } else if (GameFlag_Test(0x877)) {
        /* FAKEMATCH: the function-scope angle temporary preserves its early constant construction; a direct argument through the unbound helper differs 4. */
        angle = 0x4000;
        Kabe_ObjPlaceTurn(14, 280 << 16, 328 << 16, angle);
    }
}

void Func_02003504(void)
{
    s32 open;

    Object_GetById(gPartyState.current_owner)->priority_flags |= 0x20;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
    open = GameFlag_Test(0x301);
    if (open) {
        Kabe_ObjPlace(8, 248 << 16, 264 << 16);
        ObjectMotion_SetActionVariant(8, 3);
        Object_GetById(8)->motion_flags = 0;
        Kabe_BlendStart(0, 0, 1, 1, 15, 16);
        Func_02003aa0(8);
    } else {
        ObjectMotion_SetActionVariant(8, 1);
        Object_GetById(8)->motion_flags = open;
    }
    Func_0200300c();
}
