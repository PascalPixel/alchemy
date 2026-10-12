#include "room.h"

void Func_020003b4(void)
{
    s32 value;

    if (Object_GetById(15)->z >> 20 == 14) {
        value = 65;
        /* FAKEMATCH: ⚓️ builds the value before the index */
        asm volatile("" : "+l"(value));
        Func_020027ac(0, value);
        Func_020027ac(2, 19);
    } else {
        value = 17;
        /* FAKEMATCH: ⚓️ builds the value before the index */
        asm volatile("" : "+l"(value));
        Func_020027ac(0, value);
        Func_020027ac(2, 67);
    }
    if (Object_GetById(16)->x >> 20 == 6) {
        value = 65;
        /* FAKEMATCH: ⚓️ builds the value before the index */
        asm volatile("" : "+l"(value));
        Func_020027ac(1, value);
        Func_020027ac(3, 3);
    } else {
        value = 49;
        /* FAKEMATCH: ⚓️ builds the value before the index */
        asm volatile("" : "+l"(value));
        Func_020027ac(1, value);
        Func_020027ac(3, 67);
    }
}

void Func_02000418(void)
{
    struct Box box;

    if (Func_02004a8c(&box)) {
        Func_02004d10(box);
        Func_020003b4();
    }
}

void Func_02000444(s32 state)
{
    if (state == 1) {
        Func_020054e0(29, 56, 3, 4, 10, 50);
        Func_020054f0(93, 56, 3, 4, 74, 50);
        GameFlag_SetBit(0xa44);
    }
}

void Func_02000480(s32 state)
{
    if (state == 1) {
        Func_020054e0(29, 56, 3, 4, 18, 50);
        Func_020054f0(93, 56, 3, 4, 82, 50);
        GameFlag_SetBit(0xa45);
    }
}

void Func_020004bc(s32 state)
{
    if (state == 1) {
        Func_020054e0(29, 56, 3, 4, 14, 50);
        Func_020054f0(93, 56, 3, 4, 78, 50);
        GameFlag_SetBit(0xa46);
    }
}

void Func_020004f8(void)
{
    s32 value;

    Func_020056c0(0x248);
    value = 3;
    /* FAKEMATCH: ⚓️ builds the value before the index */
    asm volatile("" : "+l"(value));
    Func_020027ac(0, value);
    GameFlag_SetBit(0x220);
}

void Func_02000514(void)
{
    struct ObjectRuntime *object = Object_GetById(9);

    Func_02005598(10, 0, 0);
    Func_02005598(9, S16_AT(object, 100) << 16, S16_AT(object, 102) << 16);
    GameFlag_SetBit(0xa42);
    Func_020056c0(136);
}

void Func_02000554(void)
{
    Func_02001448();
}
