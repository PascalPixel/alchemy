#include "types.h"

extern void Func_02007a56(s32, s32, s32, s32, s32, s32);
extern void Func_02007a66(s32, s32, s32, s32, s32, s32);
extern void Func_02007a76(s32, s32, s32, s32, s32, s32);
extern void Func_02007cdc(s32);
extern u32 Func_02007a3e(void);
extern u32 Func_02007aac(void);
extern void Func_02002214(s32, s32, s32, s32, s32, s32, s32, s32 *);
extern void Func_0200224e(s32, s32, s32, s32, s32, s32, s32, s32 *);
extern void Func_02002288(s32, s32, s32, s32, s32, s32, s32, s32 *);
extern void Func_02007bfe(s32);
extern void Func_02007b74(s32, s32, s32, s32, s32, s32);
extern void Func_02007b8c(s32, s32, s32, s32, s32, s32);
extern void Func_02007ba2(s32, s32, s32, s32, s32, s32);

void Func_02002030(s32 mode)
{
    s32 buf[10];
    u32 i, j;

    Func_02007a56(0x70, 0x39, 0x71, 0x2a, 1, 1);
    Func_02007a66(0x75, 0x3a, 0x70, 0x2e, 1, 1);
    Func_02007a76(0x75, 0x39, 0x74, 0x2c, 1, 1);
    Func_02007cdc(0x121);
    buf[1] = 5;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 2; j++) {
        for (i = 1; i <= 7; i++) {
            if ((i & 1) != 0) {
                if (mode == 0) {
                    Func_02002214((0x319 - ((Func_02007a3e() * 5) >> 16)) << 16, 0,
                                  (((j << 2) + i) << 17) + 0x02b70000, 0,
                                  mode, 0x4000, 0x90000, buf);
                } else if (mode == 1) {
                    Func_0200224e((((j << 2) + i) << 17) + 0x03120000, 0,
                                  (((((u32 (*)(void))Func_02007a76)() * 5) >> 16) << 16) + 0x2e80000, 0x4000,
                                  0, 0, 0x90000, buf);
                } else {
                    Func_02002288(0x3380000 - (i << 17) - (j << 19), 0,
                                  (((Func_02007aac() * 5) >> 16) << 16) + 0x2c80000, 0x4000,
                                  0, 0, 0x90000, buf);
                }
                Func_02007bfe(1);
            }
        }
        if (mode == 0)
            Func_02007b74(0x70, 0x3a, 0x71, j + 43, 1, 1);
        else if (mode == 1)
            Func_02007b8c(0x70, 0x3a, j + 113, 0x2e, mode, mode);
        else
            Func_02007ba2(0x70, 0x3a, 115 - j, 0x2c, 1, 1);
    }
}
