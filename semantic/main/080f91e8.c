typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080f9c44(void);
void Func_080fb2a4(void *, u16);
void Func_080fb2cc(void *, u32, u32);
void Func_080fb334(void *, u32, u32);

void Func_080f91e8(void)
{
    u8 countdown;
    s16 target;
    s16 current;
    s16 delta;
    u16 step;

    countdown = *(u8 *)0x02003000;
    if (countdown != 0) {
        if (countdown == 1) {
            if (*(u8 *)0x02004214 == 0) {
                *(u8 *)0x02003000 = 0;
                *(u16 *)0x02003034 = 0x0100;
            }
        } else {
            *(u8 *)0x02003000 = countdown - 1;
        }
    }

    target = *(s16 *)0x02003034;
    current = *(s16 *)0x02003008;
    if (target != current) {
        delta = target - current;
        step = *(u16 *)0x02003010;
        if (delta > 0)
            current = (s16)(u16)(current + step);
        else
            current = (s16)(u16)(current - step);
        *(s16 *)0x02003008 = current;

        if (((target - current) ^ delta) < 0) {
            current = target;
            *(s16 *)0x02003008 = current;
        }
        Func_080fb2cc((void *)0x02004290, 0xFF, (u16)current);
    }

    target = *(s16 *)0x02003030;
    current = *(s16 *)0x02003038;
    if (target != current) {
        delta = target - current;
        step = *(u16 *)0x0200300C;
        if (delta > 0)
            current = (s16)(u16)(current + step);
        else
            current = (s16)(u16)(current - step);
        *(s16 *)0x02003038 = current;

        if (((target - current) ^ delta) < 0) {
            current = target;
            *(s16 *)0x02003038 = current;
        }
        Func_080fb2a4((void *)0x02004290, (u16)current);
        Func_080fb334((void *)0x02004290, 0xFF, current * 12 - 0xC00);
    }

    Func_080f9c44();
}
