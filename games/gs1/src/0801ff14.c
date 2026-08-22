#include "types.h"

extern u8 *Data_03001f2c;
void Func_08004278(s32);
void Func_0801ff58(void);
void Func_08009038(void *);

void Func_0801ff14(void)
{
    u8 *base = Data_03001f2c;
    s32 offset = 138;
    s32 zero;
    s32 count;

    Func_08004278((s32)Func_0801ff58);
    zero = 0;
    offset *= 2;
    count = 3;
    do {
        void *entry = *(void **)(offset + (unsigned int)base);

        if (entry != 0) {
            Func_08009038(entry);
            *(s32 *)(offset + (unsigned int)base) = zero;
        }
        count--;
        offset += 4;
    } while (count >= 0);
}
