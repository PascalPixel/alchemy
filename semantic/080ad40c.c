#include "types.h"

extern u8 *Data_03001f2c;

void Func_08015418(void *);
s32 Func_080022ec(s32, s32);
s32 Func_08009008(void *, s32 *, s32 *, s32);

void Func_080ad40c(void)
{
    u8 *state = Data_03001f2c;
    s32 index;
    s32 motion[2];
    s32 request[4];

    Func_08015418((void *)0x06002500);
    index = 0;
    do {
        void *object = *(void **)(state + 0x224 + index * 4);

        if (object != NULL) {
            s32 phase;
            s32 vertical;
            s32 limit;
            s16 position_y;

            vertical =
                0x01e20000 -
                ((s32)*(s16 *)(state + 0x144 + index * 2) << 16);
            ((u8 *)object)[9] &= (u8)~0xc;
            phase = *(s32 *)(state + 0x244 + index * 4);
            if (phase < 0) {
                motion[0] = -phase;
                motion[1] = -phase;
            } else {
                phase += Func_080022ec(0x10000 - phase, 3);
                motion[0] = phase;
                motion[1] = phase;
                *(s32 *)(state + 0x244 + index * 4) = motion[0];
            }
            position_y = *(s16 *)(state + 0x23c + index * 2);
            request[0] =
                (s32)*(s16 *)(state + 0x234 + index * 2) << 16;
            request[1] = vertical;
            request[2] = ((s32)position_y << 16) + vertical;
            request[3] = 0;
            limit = position_y < 0 ? 0x8000 : 0x4000;
            Func_08009008(object, request, motion, limit);
        }
        index++;
    } while (index <= 3);
}
