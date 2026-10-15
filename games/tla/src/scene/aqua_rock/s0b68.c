#include "local.h"

void Func_02002834(s32 id);
void Func_020025a8(s32 id);
void Func_02002a20(s32 a, s32 b);
void Func_02002a28(s32 frames);
void Func_02002948(s32 layer);
void Func_02002940(s32 layer);
void Func_02000b10(struct ObjectRuntime *object);
void Func_02000b58(struct ObjectRuntime *object);

// Toggles flag 0xa68 and with it which of two layer pairs is shown: four
// rounds of a random colour value, then one pair's blend scripts and layer
// fields set and the other's cleared, while the object blinks.
void Func_02000b68(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32                   i;

    Func_02002998();
    Func_02002a48(0);
    object->update = (void (*)(void))Func_02000b10;
    WaitFrames(30);
    if (GameFlag_Test_(0xa68) == 0)
        Func_02002834(26);
    else
        Func_02002834(0);
    for (i = 3; i >= 0; i--)
    {
        Func_02002a20(Random16Far() & 0x7fff, 0);
        Func_02002a28(15);
        WaitFrames(10);
        Func_02002a20(0x10000, 0);
        Func_02002a28(10);
        WaitFrames(10);
    }
    Func_02002a20(0x7fff, 0);
    Func_02002a28(20);
    Battle_WaitMode0(20);
    if (GameFlag_Test_(0xa68) == 0)
    {
        GameFlag_SetBit(0xa68);
        Func_02002958_(64, 64, 1, 2, 51, 82);
        Func_02002958_(66, 64, 1, 2, 115, 18);
        Func_02002948(5);
        Func_02002948(6);
        Func_02002940(7);
        Func_02002940(8);
        object->update = (void (*)(void))Func_02000b58;
        Func_020025a8(26);
    }
    else
    {
        GameFlag_ClearBit(0xa68);
        Func_02002958_(65, 64, 1, 2, 51, 82);
        Func_02002958_(67, 64, 1, 2, 115, 18);
        Func_02002948(7);
        Func_02002948(8);
        Func_02002940(5);
        Func_02002940(6);
        object->update = (void (*)(void))Func_02000b58;
        Func_020025a8(0);
    }
    Battle_WaitMode0(30);
    Func_02002a20(0x10000, 0);
    Func_02002a28(30);
    Battle_WaitMode0(15);
    Func_020029a0();
}
