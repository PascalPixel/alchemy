#include "lemurian_ship.h"

void Func_02000770(void)
{
    Func_02002dd0(Data_02003010);
}

void Func_02000780(void)
{
    Func_02002dd8();
}

void Func_02000788(void)
{
    Func_02002dd0(Data_02003010);
}

void Func_02000798(void)
{
    struct LemurianShipObject *object;
    s32 x;
    u8 *flags;
    s32 moving;

    Func_02002dd8();
    object = Object_GetById(8);
    x = object->x >> 20;
    if (x == 16) {
        Func_02002c68();
        Func_02002da8(0);
        LemurianShip_BlendStart(16, 47, 1, 1, x, 46);
        WaitFrames(1);
        flags = &object->motion_flags;
        *flags = 3;
        do {
            WaitFrames(1);
            moving = object->velocity_y;
        } while (moving != 0);
        Func_02002de0(188);
        WaitFrames(10);
        *flags = moving;
        GameFlag_SetBit(0x200);
        LemurianShip_BlendStart(14, 46, 1, 1, 16, 46);
        Func_02002c70();
    }
}

void Func_02000818(void)
{
    Func_02002dc0(Data_02003014);
}

void Func_02000828(void)
{
    Func_02002dc0(Data_0200301e);
}

void Func_02000838(void)
{
    LemurianShip_BlendStart(110, 1, 3, 1, 110, 4);
    GameFlag_SetBit(0x212);
}

void Func_02000860(void)
{
    Func_02002dc0(Data_02003034);
}
