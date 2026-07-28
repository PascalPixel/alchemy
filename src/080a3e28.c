#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08015298(s32, u16, u8, s32);
void Func_080a3d24(const u16 *);

void Func_080a3e28(u16 *arg0, s32 arg1) {
    s32 remaining;
    u16 *entries;
    void **objects;
    s32 entry;

    objects = (void **)((u8 *)(*(void **)0x03001F2C) + 0x48);
    entries = arg0;
    remaining = 14;
    do {
        entry = *entries++;
        if (entry != 0) {
            if (arg1 == 0) {
                Func_08015298(2, entry, M2C_FIELD(*objects, u8 *, 0xE), 0);
            } else {
                Func_08015298(7, entry, M2C_FIELD(*objects, u8 *, 0xE), 0);
            }
        }
        objects++;
        remaining--;
    } while (remaining >= 0);
    Func_080a3d24(arg0);
}
