#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "party_state.h"
#include "call.h"

extern u8 Data_02004d0c[];
extern u8 Data_02004dbc[];
extern u8 Data_02004fe4[];

s32 GameFlag_Test();
void GameFlag_SetBit();
struct ObjectRuntime *Object_GetById();
void Func_02000fa4(void);
void Func_02003f48(s32, s32, s32, s32);
void Func_02004004(void);
void Func_020040c0(void);
void Func_02004868(void *, s32, s32);
s32 Func_02004a34();
s32 Func_02004a7c();
s32 Func_02004c24();
s32 Func_02004c7c();
s32 Func_02004c8c();

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

void *Func_0200004c(void)
{
    if (GameFlag_Test(0x8ab))
        return Data_02004fe4;
    return Data_02004dbc;
}

void Func_0200006c(s32 mode, s32 id)
{
    if (id == 0)
        Func_02004004();
    if (mode == 1) {
        GameFlag_SetBit(0x8a7);
        Func_02000fa4();
    }
    if (mode == 2)
        Func_02003f48(0x1f20000, 0x100000, 0x19c0000, 2);
    if (mode == 3)
        Func_02003f48(0x1f20000, 0x100000, 0x19c0000, 30);
    if (id == 0x3e7)
        Func_020040c0();
}

void Func_020000c8(s32 mode, s32 arg)
{
    struct ObjectRuntime *a;
    struct ObjectRuntime *b;
    s32 i;

    Func_02004868(Data_02004d0c, mode, arg);
    if (mode == 1) {
        a = Object_GetById(19);
        b = Object_GetById(21);
        a->speedLimit = 0x40000;
        a->acceleration = 0x8000;
        b->acceleration = 0x8000;
        b->speedLimit = 0x40000;
        /* FAKEMATCH: ⚓️ finishes the second object's speed before it builds the first position. */
        __asm__ volatile("");
        Call4((void (*)())Object_SetPositionAndResetMotion, (s32)a, 0x1e80000, a->y, 0x1380000);
        Call4((void (*)())Func_02004a34, (s32)a, 0x1b80000, 0, 0x1380000);
        Call4((void (*)())Object_SetPositionAndResetMotion, (s32)b, 0x1e80000, b->y, 0x1380000);
        Call4((void (*)())Func_02004a34, (s32)b, 0x1b80000, 0, 0x1780000);
        Func_02004c8c(1);
        Func_02004c8c(2);
        i = 0;
        do {
            Func_02004c24(1, 18, 19);
            Func_02004c24(2, 20, 21);
            WaitFrames(1);
            Func_02004c8c(1);
            Func_02004c8c(2);
            if (Func_02004a7c(a) != 0 && Func_02004a7c(b) != 0)
                break;
            i++;
        } while (i < 60);
    }
}

void Func_02000190(s32 unused, s32 id)
{
    struct ObjectRuntime *leader;
    struct ObjectRuntime *object;
    s32 leader_x;
    s32 x;
    s32 z;

    leader = Object_GetById(gPartyState.currentOwner);
    object = Object_GetById(id);
    object->motionFlags = 0;
    if (id == 10) {
        leader_x = leader->x >> 20;
        x = object->x >> 20;
        z = object->z >> 20;
        if (x == 34 && z == 32 && leader_x == 33)
            Value4(Func_02004c7c, 0x2100000, 0x2000000, 2, 255);
        if (x == 33 && z == 32 && leader_x == 32)
            Value4(Func_02004c7c, 0x2000000, 0x2000000, 2, 255);
    }
}
