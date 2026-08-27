#include "types.h"

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

extern void *Data_03001e68;

void Func_080030f8(s32);
s32 Func_080022fc(s32, s32);
void Func_08016418(s32, s32);
s32 Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e940(s32, s32, s32, s32);
void Func_0801ea08(s32, s32, s32, s32, s32);
void Func_08019fcc(u32, s32, s32 *, s32 *, s32);
void Func_0801a404(u32, s32, s32 *, s32 *, s32);
s32 Func_08004080(void);
s32 Func_0801a2ec(s32, s32);
void Func_0801eadc(s32, s32, s32, s32, s32);

s32 Func_08029554(void)
{
    s32 redraw;
    s32 src_index;
    s32 rows;
    s32 mode;
    s32 window;
    s32 tmp;
    s32 msg;
    s32 y;
    s32 x_base;
    s32 i;
    s32 col_x;
    s32 row_off;
    s32 row_y;

    redraw = 1;
    window = 0;
    rows = 0;
    mode = 0;
    M2C_FIELD(Data_03001e68, u16 *, 4) = (u16)redraw;
    Func_080030f8(1);

loop_1:
    if (*(volatile u32 *)0x03001B04 & 0x20) {
        redraw = 1;
        rows -= 1;
    }
    if (*(volatile u32 *)0x03001B04 & 0x10) {
        redraw = 1;
        rows += 1;
    }
    if (*(volatile u32 *)0x03001B04 & 0x200) {
        redraw = 1;
        mode -= 1;
    }
    if (*(volatile u32 *)0x03001B04 & 0x100) {
        redraw = 1;
        mode += 1;
    }
    if (*(volatile u32 *)0x03001B04 & 1) {
        goto exit;
    } else if (*(volatile u32 *)0x03001B04 & 2) {
        goto exit;
    } else {
        {
            if (redraw != 0) {
                redraw = 0;
                rows = (rows + 8) % 8;
                mode = Func_080022fc(mode + 3, 3);
                Func_08016418(window, 2);
                window = Func_080162d4(0xA, 0, 0x12, 0xC, 2);
                if (mode == 0) {
                    msg = 0x08037440;
                } else if (mode == 1) {
                    msg = 0x08037448;
                } else {
                    msg = 0x08037450;
                }
                Func_0801e940(msg, window, 0, 0);
                Func_0801e940(0x08037458, window, 0, 8);
                Func_0801ea08(rows, 0, window, 0x28, 8);
                x_base = rows << 5;
                Func_0801ea08(x_base, 3, window, 0x40, 8);
                Func_0801e940(0x08037460, window, 0x58, 8);
                Func_0801ea08(x_base + 0x1F, 3, window, 0x60, 8);
                for (i = 0; i <= 0x1F; i++) {
                    col_x = i / 8;
                    row_off = (i - (col_x * 8)) * 0x10;
                    row_y = (col_x * 0x10) + 0x10;
                    if (mode == 0) {
                        Func_08019fcc(x_base + i, 1, &tmp, &src_index, mode);
                        Func_0801eadc(tmp, 0x40000000, window, row_off, row_y);
                    } else if (mode == 1) {
                        Func_0801a404(x_base + i, 1, &tmp, &src_index, 0);
                        Func_0801eadc(tmp, 0x40000000, window, row_off, row_y);
                    } else {
                        tmp = Func_08004080();
                        Func_0801a2ec(i, 0);
                        Func_0801eadc(tmp, 0x40000000, window, row_off, row_y);
                    }
                }
            }
        }
        Func_080030f8(1);
        goto loop_1;
    }

exit:
    Func_08016418(window, 2);
    M2C_FIELD(Data_03001e68, u16 *, 4) = 0;
    return 0;
}
