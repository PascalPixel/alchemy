#include "trial_road.h"

#include "iwram_call.h"

extern s32 Data_02007e20;
extern s32 Data_02007e24;
extern u8 Data_02007e28[];

void Func_02001f08(void);

static __inline__ void Words_Copy(void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Iwram_CopyWords(destination, source, size);
}

void Func_02001fc4(s32 mode)
{
    Data_02007e24 = 1;
    if (mode != 2)
        Words_Copy(Data_02007e28, (void *)0x050000a0, 32);
    if (mode == 1) {
        s32 zero = 0; /* FAKEMATCH: builds the zero in r2 and the counter address in r3, as overlay 699's twin does */

        Data_02007e20 = zero;
        Scheduler_AddOrUpdateCallback(Func_02001f08, 0x480);
    }
}
