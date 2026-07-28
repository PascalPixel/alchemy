#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void Func_08004278(void *);
void Func_08016418(struct Work *work, s32 release);
s32 Func_08003f3c(u32 index);
void Func_08002dd8(s32);
void Func_080030f8(u32);

void Func_0802851c(void) {
    struct Work *childWork;
    s32 i;
    u16 *entry;
    void *work;

    work = *(void **)0x03001F38;
    Func_08004278((void *)0x08028195);
    childWork = M2C_FIELD(work, struct Work *, 0x78);
    if (childWork != 0) {
        Func_08016418(childWork, 2);
    }
    i = 0;
    while (i < (s32)M2C_FIELD(work, s16, 0x8E)) {
        entry = (u16 *)((u8 *)work + 0x12) + i * 10;
        Func_08003f3c(*entry);
        i += 1;
    }
    Func_08002dd8(0x3A);
    Func_080030f8(1U);
}
