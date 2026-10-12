/* Draft, differ 8 of 25: the native finishes the palette address (adds r1, #160) after it loads the IWRAM entry, and clears Data_02007e20 with the address in r3 and the 0 in r2; this form adds first and swaps the two registers. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

#include "iwram_call.h"

extern s32 Data_02007e20;
extern s32 Data_02007e24;
extern u8 Data_02007e28[];

void Func_02001f08(void);

void Func_02001fc4(s32 mode)
{
    Data_02007e24 = 1;
    if (mode != 2)
        Iwram_CopyWords(Data_02007e28, (void *)0x050000a0, 32);
    if (mode == 1) {
        Data_02007e20 = 0;
        Scheduler_AddOrUpdateCallback(Func_02001f08, 0x480);
    }
}
