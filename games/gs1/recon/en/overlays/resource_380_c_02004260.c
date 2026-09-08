#include "types.h"

u8 *Func_02008b38(s32);
s32 Func_0200c95c(s32);
s32 Func_0200c954(s32, s32);
void Func_02008b50(u8 *, void *);
u8 *Func_02008b5e(s32, s32);
void Func_02008c06(s32);
void Func_02008b8c(s32, s32, u8 *);
void Func_02008b8a(s32);
void Func_02008e00(s32);
void Func_02008df0(u8 *, s32);
void Func_02008c80(s32, s32);
void Func_02008c48(s32, s32);
void Func_02008bd6(u8 *);
void Func_02008d1e(s32, s32);
extern u8 Data_0200cbe4[];

s32 FieldScene_PresentItem(s32 item)
{
    s32 zero = 0;
    u8 *buf;
    u8 *obj = Func_02008b38(22);
    s32 text = Func_0200c95c(224);
    s32 size = Func_0200c954(text, 224);
    u8 *sprite;
    u8 *p;
    s32 mask;

    if (obj != 0) {
        Func_02008b50(obj, Data_0200cbe4);
        sprite = *(u8 **)(obj + 80);
        p = sprite + 38;
        *p = zero;
        p++;
        *p = zero;
        mask = 33;
        mask = -mask;
        sprite[5] &= mask;
        sprite[9] &= 0xf;
        *(s32 *)(obj + 40) = 163840;
        *(s32 *)(obj + 72) = 16384;
        buf = Func_02008b5e(17, 1544);
        Func_02008c06(item);
        Func_02008b8c(sprite[28], 128, buf + 1024);
        Func_02008b8a(17);
        Func_02008e00(83);
        Func_02008df0(obj, 3);
        Func_02008c80(text, size);
        Func_02008c48(text, item);
        Func_02008bd6(obj);
        Func_02008d1e(0, 1);
    }
    return text;
}
