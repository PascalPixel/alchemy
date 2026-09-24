/* 2026-09-24: 16 differing halfwords (from 20) after a do-while wrap and
   statement-swap sweep; the do-while wraps are search artefacts. */
#include "TYPES.H"

extern u8 Data_080af2a6[];
extern u8 Data_080af2d0[];
extern u8 Data_080af2bc[];
extern u8 Data_080af2b1[];

void Func_080a1f74(s32 arg0, s8 *dst)
{
    s8 *src;
    s32 i;
    s8 c;

    src = (s8 *)Data_080af2a6;
    switch (arg0) {
    case 0:
        src = (s8 *)Data_080af2d0;
        break;
    case 1:
        src = (s8 *)Data_080af2bc;
        break;
    case 2:
        src = (s8 *)Data_080af2b1;
        break;
    }

    *dst = c;
    c = *src;
    i = 0;
    if (c != -1) {
        do {
            i++;
            if (i > 31)
                break;
            c = *src;
            src++;
            dst++;
            do { *dst = c; } while (0);
        } while (c != -1);
    }
}
