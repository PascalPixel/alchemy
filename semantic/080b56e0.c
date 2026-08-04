#include "types.h"

void Func_080030f8(s32 arg0);
void Func_0800403c(void);
void Func_080040e8(void);
void Func_08004760(void);
void Func_0800479c(void);
void Func_08004858(void);
void Func_08077010(s32 arg0);
void Func_08077098(void);
void Func_080770c8(s32 arg0);
void Func_080770d0(s32 arg0);
void Func_080b5368(s32 arg0);
void Func_080b5534(void);
void Func_080b63c8(s32 arg0);
void Func_080c2a08(void);

void Func_080b56e0(void)
{
    volatile u32 *status = (volatile u32 *)0x03001ae8;
    volatile u32 *input = (volatile u32 *)0x03001b04;
    volatile u8 *mode = (volatile u8 *)0x0200046b;
    volatile u16 *command = (volatile u16 *)0x02000416;
    s32 sticky = 0;

    Func_08077098();

    for (;;) {
        s32 value = 0x101;

        Func_0800479c();
        Func_08004760();
        Func_080040e8();
        Func_08004858();
        Func_0800403c();
        Func_080770c8(0x16a);

        if ((*status & 0x80) == 0) {
            Func_080770c8(0x162);
            Func_080b63c8(0x101);
            continue;
        }

        {
            s32 previous = -1;
            s32 page = 0;

            Func_080770d0(0x16a);

            for (;;) {
                Func_080770d0(0x20);
                Func_080030f8(1);

                for (;;) {
                    if ((*input & 0x10) != 0)
                        value = (s32)((u32)value + 1);
                    if ((*input & 0x20) != 0)
                        value = (s32)((u32)value - 1);
                    if ((*input & 0x40) != 0)
                        value = (s32)((u32)value - 10);
                    if ((*input & 0x80) != 0)
                        value = (s32)((u32)value + 10);
                    if ((*input & 0x100) != 0)
                        page = (s32)((u32)page + 1);
                    if ((*input & 0x200) != 0)
                        page = (s32)((u32)page - 1);
                    if ((*input & 1) != 0)
                        break;

                    if ((*input & 8) != 0)
                        Func_080b5534();
                    if ((*input & 4) != 0)
                        Func_080c2a08();
                    if ((*input & 2) != 0 || sticky != 0) {
                        sticky = 1;
                        *mode = 5;
                    }

                    if (page != previous) {
                        Func_08077098();
                        Func_080b5368(page);
                        previous = page;
                    }

                    Func_080030f8(1);
                }

                if ((*status & 0x80) != 0)
                    Func_080770c8(0x16c);

                Func_08077010(0);
                *command = 0x1d;

                if (value == 0x1c)
                    Func_080770c8(0x16e);

                Func_080770c8(0x162);
                Func_080b63c8(value);
                Func_0800479c();
                Func_08004760();
                Func_080040e8();
                Func_08004858();
                Func_0800403c();
            }
        }
    }
}
