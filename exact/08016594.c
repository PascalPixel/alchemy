#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_08015ec0(u32 arg0);
s32 Func_08003f3c(u32 index);

struct Entry_08016594 {
    u8 unknown_00[4];
    u8 kind;
    s8 active;
    u8 unknown_06[8];
    u8 index;
    u8 unknown_0f[0x0a];
    u8 palette;
};

void Func_08016594(struct Entry_08016594 *entry) {
    Func_08015ec0((u32)entry);
    if (entry->kind != 0) {
        Func_08003f3c(entry->index);
        if (entry->kind == 2) {
            u8 *dst = (u8 *) (*(s32 *)0x03001E8C);
            s32 idx = ((u32)entry->palette >> 4) * 2 + 0x12D0;
            *(u16 *)(dst + idx) = 0x3E7;
        }
    }
    entry->active = 0;
}
