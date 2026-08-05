#include "types.h"

/* Complete countdown/reroll owner and its two pool words. */
extern s32 **Data_03001edc;
extern u8 *Data_03001ebc;
extern u32 Data_02000434;



extern void Func_02003f60(u8 *);
extern u32 Func_02004798(void);
void Func_02002370(void)
{
    s32 *record = *Data_03001edc;
    u8 *slot = ((u8 **)(Data_03001ebc + 0x14))[Data_02000434];

    if (record[2] != 0)
        record[2]--;
    else {
        Func_02003f60(slot);
        record[2] = (s32)((Func_02004798() * 30) >> 16) + 10;
    }
}
