#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"


s32 GameFlag_Test(s32 flag)
{
    s32 bit;
    u32 index;

    index = (u32)flag << 20;
    bit = flag & 7;
    flag = index >> 23;
    return (GameFlagBytes[flag] >> bit) & 1;
}

s32 GameFlag_SetBit(s32 flag)
{
    s32 bit;
    s32 mask;
    u8 *bytes;

    bit = flag & 7;
    bytes = GameFlagBytes;
    /* FAKEMATCH: Both ordinary forms load the flag base after mask setup; keep that load first. */
    __asm__("" : : "r"(bit), "r"(bytes));
    mask = 1 << bit;
    flag = ((u32)flag << 20) >> 23;
    bytes[flag] |= mask;
    return flag;
}

void GameFlag_ClearBit(s32 flag)
{
    s32 bit;
    s32 mask;
    u8 *bytes;

    bit = flag & 7;
    bytes = GameFlagBytes;
    /* FAKEMATCH: Both ordinary forms load the flag base after mask setup; keep that load first. */
    __asm__("" : : "r"(bit), "r"(bytes));
    mask = 1 << bit;
    flag = ((u32)flag << 20) >> 23;
    bytes[flag] &= ~mask;
}
