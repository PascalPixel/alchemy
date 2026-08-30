#include "types.h"

#define AudioTest_RunParameterController Func_080f92fc

extern volatile u32 Data_03001b04;
extern s32 Data_03000b5c;
extern s32 Data_03007804;
typedef struct {
    s32 a;
    s32 b;
    s32 c;
} Triple;

extern const Triple Data_080fb794;

s32 Modulo(s32, s32);
void Func_080037d4(void);
void Func_080f9080(s32);

void AudioTest_RunParameterController(void)
{
    s32 buf[3];
    s32 repeat;
    s32 row;
    s32 offset;
    s32 timer;
    s32 *ptr;

    *(Triple *)buf = Data_080fb794;

    repeat = 2;
    row = 0;
    Data_03007804 = 0;
    timer = 20;
    offset = 0;
    ptr = buf;

    for (;;) {
        if (timer != 0)
            timer -= 1;

        if (Data_03000b5c != 0) {
            Data_03000b5c = 0;
            timer = 20;
        }

        if (Data_03001b04 & 4) {
            repeat = Modulo(repeat + 1, 5);
            Func_080037d4();
        }
        if (Data_03001b04 & 0x100)
            *ptr += 10;
        if (Data_03001b04 & 0x200)
            *ptr -= 10;
        if (Data_03001b04 & 0x10)
            *ptr += 1;
        if (Data_03001b04 & 0x20)
            *ptr -= 1;
        if (Data_03001b04 & 0x40) {
            if (row > 0) {
                ptr -= 1;
                offset -= 4;
                row -= 1;
            }
        }
        if (Data_03001b04 & 0x80) {
            if (row <= 1) {
                ptr += 1;
                offset += 4;
                row += 1;
            }
        }
        if (Data_03001b04 & 1)
            Func_080f9080(*(s32 *)((u8 *)buf + offset));
        if (Data_03001b04 & 2)
            Func_080f9080(19);
        if (Data_03001b04 & 8)
            Func_080f9080(17);
        if (Data_03001b04 & 4)
            Func_080f9080(0x121);

        WaitFrames(1);
    }
}
