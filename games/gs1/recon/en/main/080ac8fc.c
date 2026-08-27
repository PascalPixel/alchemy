#include "types.h"

void *Func_08077008(s32);

s32 Func_080ac8fc(u16 *out, s32 id, s32 row_select)
{
    void *record = Func_08077008(id);
    s32 count = 0;
    s32 bit;
    s32 row;

    if (row_select == -1) {
        s32 id_shifted = id << 8;
        u32 *rows = (u32 *)((u8 *)record + 264);
        for (row = 0; row <= 3; row++) {
            u32 active = rows[row];
            u32 secondary = *(u32 *)((u8 *)record + 248 + row * 4);
            for (bit = 0; bit <= 19; bit++) {
                u32 mask = 1u << bit;
                s32 packed;
                if (active & mask) {
                    packed = (row << 5) | bit | (-0x8000) | id_shifted;
                    out[count] = (u16)packed;
                    count++;
                } else if (secondary & mask) {
                    packed = (row << 5) | bit | id_shifted;
                    out[count] = (u16)packed;
                    count++;
                }
            }
        }
    } else {
        u32 active = *(u32 *)((u8 *)record + row_select * 4 + 264);
        u32 secondary = *(u32 *)((u8 *)record + row_select * 4 + 248);
        s32 row_shifted = row_select << 5;
        for (bit = 0; bit <= 19; bit++) {
            u32 mask = 1u << bit;
            s32 packed;
            if (active & mask) {
                packed = row_shifted | bit | (-0x8000);
                out[count] = (u16)packed;
                count++;
            } else if (secondary & mask) {
                packed = row_shifted | bit;
                out[count] = (u16)packed;
                count++;
            }
        }
    }

    return count;
}
