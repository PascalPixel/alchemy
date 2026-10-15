#include "local.h"

void GameFlag_SetByte(s32 flag, s32 value);

void Func_02000cd8(s32 unused, s32 id)
{

    if (id == 24) {
        s32 column;
        s32 row;

        struct ObjectRuntime *first = Object_GetById_(24);

        column = first->x >> 20;
        row = first->z >> 20;
        GameFlag_SetByte(0x310, column);
        GameFlag_SetByte(0x318, row);
        GameFlag_ClearBit_(0x320);
        GameFlag_ClearBit_(0x321);
        if (column == 7 && row == 35)
            GameFlag_SetBit_(0x320);
        if (column == 15 && row == 35)
            GameFlag_SetBit_(0x321);
    } else if (id == 25) {
        struct ObjectRuntime *object = Object_GetById(25);

        if (object->x >> 20 == 32) {
            WaitFrames(30);
            object->z += 0x10000;
            GameFlag_SetBit_(0x950);
        }
    }
}
