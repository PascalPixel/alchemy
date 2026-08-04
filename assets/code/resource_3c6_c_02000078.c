#include "types.h"

s32 *Func_020015ce(s32);
s32 *Func_020015da(s32);
void Func_0200165a(s32);
void Func_02001672(s32, s32);
void Func_02001686(s32, s32, s32);
void Func_020015d4(s32);
void Func_02001614(s32, const u8 *);

extern u8 Data_02009638[];

void Func_02000078(s32 slot)
{
    /* Give the selected record matching 16.16 motion rates. */
    Func_020015ce(slot)[6] = 0x00010000;
    Func_020015da(slot)[7] = 0x00010000;

    /* Run the dialogue, facing, pause, and scripted-motion sequence. */
    Func_0200165a(0x26af);
    Func_02001672(slot, 0);
    Func_02001686(slot, 0xc000, 0);
    Func_020015d4(20);
    Func_02001614(slot, Data_02009638);
}
