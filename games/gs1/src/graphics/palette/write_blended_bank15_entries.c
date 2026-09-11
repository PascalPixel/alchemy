#include "types.h"
#include "scene.h"
#include "abi/graphics/palette/write_blended_bank15_entries.h"

void Palette_WriteBlendedBank15Entries(s32 arg0)
{
    *(s16 *)0x050001E8 = Sys_SetMode(arg0, 0xEEEE, 0xCCCC, 0x11110);
    *(s16 *)0x050001EA = Sys_SetMode(arg0, 0xD555, 0xBBBB, 0xEEEE);
    *(s16 *)0x050001EC = Sys_SetMode(arg0, 0xBBBB, 0xAAAA, 0xCCCC);
    *(s16 *)0x050001EE = Sys_SetMode(arg0, 0xA221, 0x9999, 0xAAAA);
    *(s16 *)0x050001F0 = Sys_SetMode(arg0, 0x10888, 0xDDDD, 0x13333);
    *(s16 *)0x050001F2 = Sys_SetMode(arg0, 0x12221, 0xEEEE, 0x15555);
    *(s16 *)0x050001F4 = Sys_SetMode(arg0, 0x13BBB, 0x10000, 0x17777);
}
