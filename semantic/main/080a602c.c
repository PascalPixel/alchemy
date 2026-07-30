#include "types.h"

extern void *Data_03001f2c;
extern u8 Data_00000000[];

void Func_080a1ac0(s32, s32);
s32 Func_08077008(s32);
u8 Func_080a68ec(void *, void *, s32);
s32 Func_080a60d4(void *, void *);
void Func_080a17c4(void *);
void Func_080030f8(s32);

s32 Func_080a602c(s32 arg0)
{
    register void *base = Data_03001f2c;
    register s32 offset = arg0 + 28;
    register s32 arg_offset = arg0 * 4;
    register s32 value;
    register s32 item_offset;
    s32 result = 0;
    u8 *byte_field;
    void *object;

    object = *(void **)(base + arg_offset + 0x14);
    *(u8 *)(object + 5) = 1;
    *(s16 *)(object + 0xC) = result;
    object = *(void **)(base + 0x21C);
    *(u8 *)(object + 5) = 13;

    value = *(s8 *)(base + offset);
    byte_field = base + 2;
    byte_field[offset] = *(u8 *)(base + 0x219);
    if (value == -1) {
        item_offset = 0;
        *(u8 *)(base + offset) = (s8)(s32)Data_00000000;
    } else {
        item_offset = value * 2;
        Func_080a1ac0(value * 24 - 10, 16);
    }

    result = Func_08077008(*(u16 *)(base + item_offset + 0x208));
    item_offset = (s32)(base + 0x1C8);
    *(u8 *)(base + 0x218) =
        Func_080a68ec((void *)result, (void *)item_offset, 2);
    result = Func_080a60d4(base + 0x208, (void *)item_offset);
    Func_080a17c4(*(void **)(base + arg_offset + 0x14));
    Func_080030f8(1);
    return result;
}
