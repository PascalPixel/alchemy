#include "types.h"

u8 *Func_08077008(s32);
void Func_080c1ebc(s32);
extern u8 Value_000000fe;

void Func_080bac6c(s32 id)
{
    s32 base;
    s32 offset;
    s32 current;
    s32 index;
    s32 second;
    s32 empty;
    u16 replacement;
    u8 *runtime;

    base = *(s32 *)0x03001e74;
    runtime = Func_08077008(id);
    replacement = (u16)(u32)&Value_000000fe;
    runtime[0x12a] = 0;

    offset = 0x58;
    goto first;
advance_first:
    offset += 2;
first:
    current = *(s16 *)((u8 *)offset + base);
    if (current != id)
        goto miss_first;
    *(u16 *)((u8 *)offset + base) = replacement;
    goto found;
miss_first:
    if (current != 0xff)
        goto advance_first;

    index = 0;
    second = base + 2;
second_loop:
    offset = (index << 1) + 0x64;
    current = *(s16 *)(second + offset);
    if (current == id) {
        *(u16 *)(second + offset) = 0xfe;
        goto found;
    }
    index++;
    if (current != 0xff)
        goto second_loop;
    return;

found:
    Func_080c1ebc(id);
    index = 0;
    offset = 0xbb;
    empty = 0xff;
    offset <<= 2;
    do {
        if (*(s16 *)((u8 *)offset + base) == id)
            *(u16 *)((u8 *)offset + base) = empty;
        index++;
        offset += 0x10;
    } while ((u32)index <= 0x13);
}
