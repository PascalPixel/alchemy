#include "types.h"

extern const u8 Data_08033ef8[];
extern const u8 Data_080342f8[];

s32 Func_08004080(void);
s32 Func_08003fa4(s32 value, s32 size, const void *source);

void Func_0801b248(u8 *state, s32 side)
{
    u32 offset = side * 52;
    u8 *attributes = state + offset + 40;
    u8 *slot = state + offset + 8;
    const void *source;
    u16 count;
    s16 inactive = 0;

    *(s16 *)(slot + 2) = inactive;

    if (side != 0) {
        count = *(u16 *)(state + 0x394);
        if (*(u16 *)(state + 0x39C) != 0)
            count -= *(u16 *)(state + 0x39C);
        if (count > 5) {
            *(s16 *)(slot + 2) = 1;
            count = 5;
        }

        *(s16 *)(state + 0x44) =
            *(u16 *)(state + 0x396) + (count - 1) * 16 + 17;
        source = Data_080342f8;
    } else {
        *(s16 *)(state + 0x10) = *(u16 *)(state + 0x396) - 9;
        source = Data_08033ef8;
        if (*(u16 *)(state + 0x39C) != 0)
            *(s16 *)(state + 0x0A) = 1;
    }

    if (*(s16 *)(slot + 10) == 0) {
        u8 *resource = state + offset + 12;
        u8 value;

        *(u16 *)resource = Func_08004080();
        *(u16 *)(resource + 2) =
            Func_08003fa4(*(u16 *)resource, 128, source);
        *(u16 *)(slot + 10) = *(u16 *)(state + 0x398);
        *(u16 *)slot = inactive;

        value = attributes[5];
        value &= ~12;
        value &= ~16;
        value |= 32;
        value &= ~3;

        attributes[7] = (attributes[7] & ~62) & 63;
        value &= 63;
        value |= 128;
        attributes[5] = value;
        attributes[9] &= ~12;
    }
}
