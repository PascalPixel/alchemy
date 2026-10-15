#include "room.h"

void Func_02000670(void)
{
    ((struct SceneWork *)Ram_HeapSlots->event_work)->unknown_166 = 0;
    GameFlag_ClearBit(0x106);
    GameFlag_ClearBit(0x104);
    Func_02005560();
    Func_02005648(0);
    Func_020055d0(0x20000, 0x4000);
    Camera_SetBounds(0xf80000, -1, 0x2c80000, 1);
    Func_020055e0();
    WaitFrames(20);
    Func_020055d0(0x50000, 0xa000);
    Camera_SetBounds(0xf80000, -1, 0x3680000, 1);
    Func_020055e0();
    Battle_WaitMode0(120);
    Func_02005568();
}

void Func_020006f4(s32 burst)
{
    if (!Flag_IsSet(0xa46)) {
        Func_020054e0_(29, 60, 3, 4, 14, 50);
        Func_020054f0_(93, 60, 3, 4, 78, 50);
        if (burst && !Flag_IsSet(0xa43))
            Func_0200055c(14);
        Flag_Set(0xa43);
    }
}

void Func_02000754(void)
{
    Func_020055f0(2);
}

void Func_02000760(void)
{
    Func_020055f0(3);
}

void Func_0200076c(void)
{
    Func_020055f0(4);
}

void Func_02000778(void)
{
    Func_020055f0(5);
}

void Func_02000784(void)
{
    Func_020055f0(6);
}

void Func_02000790(void)
{
    Func_020055f0(7);
}

void Func_0200079c(void)
{
    Func_020055f0(8);
}

void Func_020007a8(void)
{
    Func_020055f0(9);
}

void Func_020007b4(void)
{
    Func_020055f0(10);
}

void Func_020007c0(void)
{
    Func_020055f0(11);
}

void Func_020007cc(void)
{
    Func_020055f0(12);
}

void Func_020007d8(void)
{
    Func_020055f0(13);
}

void Func_020007e4(void)
{
    Func_020055f0(14);
}

void Func_020007f0(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    if (object->x >> 20 == 13) {
        object->motion_flags = 3;
        if (object->terrain_height != Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z)) {
            s32 frames;

            WaitFrames(2);
            for (frames = 0; frames < 30 && object->y > object->terrain_height; frames++)
                WaitFrames(1);
            Func_020056c0(133);
        }
        Func_020054f0_(71, 24, 7, 1, 71, 5);
        Func_020054e8_(7, 24, 7, 1, 7, 7);
        Func_020054e8_(7, 88, 7, 1, 7, 71);
        Func_020056c0(167);
        Func_02005598(id, 0, 0);
        Flag_Set(0x358);
    }
}

void Func_020008a0(s32 unused, s32 id)
{
    if (Object_GetById(id)->x >> 20 == 7)
        Flag_Set(0xa37);
}

void Func_020008bc(void)
{
    Func_020054e0_(17, 24, 3, 1, 17, 13);
    Func_020054e8_(20, 24, 1, 1, 18, 77);
    Func_020054e0_(62, 20, 1, 2, 54, 20);
}

void Func_02000900(void)
{
    Func_020054e0_(17, 25, 3, 1, 17, 13);
    Func_020054e8_(20, 25, 1, 1, 18, 77);
    Func_020054e0_(62, 22, 1, 2, 54, 20);
}

void Func_02000944(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 frame;
    s32 delta;

    /* FAKEMATCH: taking the object's address keeps the motion flags' address inside the loop; without it GCSE hoists it out (differ 24). */
    (void)&object;
    Func_02005608();
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Object_SetModeById(gPartyState.current_owner, 28);
    Battle_WaitMode0(16);
    frame = 0;
step:
    if (frame == 5)
        Func_020056c0(204);
    object->motion_flags = 0;
    delta = -0x400;
    /* FAKEMATCH: the barrier loads the first step before the scale it is added to; plainly sched2 loads the scale first (differ 2). */
    asm volatile("" : "+l"(delta));
    object->scale_x += delta;
    object->scale_y += -0x300;
    object->y += -0x9999;
    WaitFrames(1);
    if (++frame <= 39)
        goto step;
    /* FAKEMATCH: the one-pass block keeps the store ahead of the heap slots' address; plainly sched2 builds the address first (differ 2). */
    do {
        Object_GetById(gPartyState.current_owner)->animation_kind = 0;
    } while (0);
    SCENE_WORK->unknown_1ac = 521;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020055f0(16);
}
