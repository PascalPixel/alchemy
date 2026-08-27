#include "types.h"

extern void *Data_03001f2c;

void Func_08015418(void *);
s32 Func_080022ec(s32, s32);
void Func_08009008(void *object, s32 *request, s32 *motion, s32 limit);

void Func_080ad40c(void)
{
    void *state = Data_03001f2c;
    void **object_ptr;
    s32 index;
    s32 vertical_offset;
    s32 position_offset;
    s32 speed_offset;
    s32 motion[2];
    s32 request[4];

    Func_08015418((void *)0x06002500);

    index = 0;
    object_ptr = (void **)((u8 *)state + 548);
    vertical_offset = 324;
    position_offset = 564;
    speed_offset = 580;

    do {
        void *object = *object_ptr;
        object_ptr++;

        if (object != 0) {
            s16 y_val = *(s16 *)((u8 *)state + vertical_offset);
            s32 vertical = (241 << 17) - ((s32)y_val << 16);
            s32 speed;
            s32 adjusted;
            s32 limit;
            s16 pos_x;
            s16 pos_y;

            *(u8 *)((u8 *)object + 9) &= -13;

            speed = *(s32 *)((u8 *)state + speed_offset);
            if (speed < 0) {
                adjusted = -speed;
            } else {
                adjusted = speed + Func_080022ec(0x10000 - speed, 3);
            }
            motion[1] = adjusted;
            *(s32 *)((u8 *)state + speed_offset) = adjusted;

            pos_x = *(s16 *)((u8 *)state + position_offset);
            request[0] = (s32)pos_x << 16;
            request[1] = vertical;

            pos_y = *(s16 *)((u8 *)state + position_offset + 8);
            request[2] = ((s32)pos_y << 16) + vertical;
            request[3] = 0;

            limit = (pos_y < 0) ? 0x8000 : 0x4000;

            Func_08009008(object, request, motion, limit);
        }

        vertical_offset += 2;
        position_offset += 2;
        speed_offset += 4;
        index += 1;
    } while (index <= 3);
}
