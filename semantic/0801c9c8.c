#include "types.h"

/*
 * Remaining leaf from the former 0801c9be assembly row. The two empty
 * callbacks immediately before this function are independently adopted C.
 */
void Func_0801c9c8(u8 *state)
{
    s32 offset;
    s32 index;

    offset = 0x400;
    for (index = 0; index < 6; index++) {
        *(u16 *)(state + offset + 10) = 0;
        offset += 52;
    }
    *(u16 *)(state + offset + 10) = 0;
    offset += 60;

    *(u16 *)(state + offset) = 0;
    *(u16 *)(state + offset + 2) = 0;
    *(u16 *)(state + offset + 4) = 0;
    *(u16 *)(state + offset + 6) = 0;
    *(u16 *)(state + offset + 8) = 0;
}
