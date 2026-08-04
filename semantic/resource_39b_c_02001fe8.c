#include "types.h"

/* Complete linked-object cleanup owner and three-word pool. */
extern s32 **Data_03001edc;
extern void Func_080770d0(s32);
extern void Func_08009098(u8 *, void *);
extern void Func_08009080(u8 *, s32);

void Func_02001fe8(void)
{
    s32 *record = *Data_03001edc;
    u8 *object;

    if (record[0] == 0)
        return;
    record[0] = 0;
    Func_080770d0(0x161);
    object = (u8 *)record[5];
    if (object != 0) {
        *(short *)(object + 0x64) = 0;
        Func_08009098(object, (void *)0x0200a7dc);
        Func_08009080(object, 7);
        record[5] = 0;
    }
}
