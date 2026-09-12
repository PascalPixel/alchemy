#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
typedef s32 (*KeyCallbackFn)(void);
extern u8 Data_03001d34;
extern u8 Data_03001a20[];
void Runtime_InvokeCallbacksByKey(s32 arg0)
{
    s32 key = arg0;
    u8 *p = Data_03001a20;
    s32 i;
    key = key >> 8;
    if (Data_03001d34 == 1) {
        i = 0x15;
        p -= 8;
loop:
        i -= 1;
        if (i != 0) {
            p += 8;
            if (p[5] == key) (*(KeyCallbackFn *)p)();
            goto loop;
        }
    }
}
