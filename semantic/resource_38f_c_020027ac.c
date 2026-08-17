#include "types.h"

struct Sub {
    u8 pad00[9];
    u8 f09;
    u8 pad0a[28];
    u8 f26;
};

struct Obj {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[4];
    s32 f18;
    s32 f1c;
    u8 pad20[3];
    u8 f23;
    u8 pad24[0x2c];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

extern s32 Data_0200b694;
extern s32 Data_0200b690;
extern s32 Data_0200b68c;
extern u8 Data_0200b610[];

#define FRAME_COUNTER (*(volatile s32 *)0x03001e40)
#define REFERENCE_RECORD (**(s32 ***)0x03001e70)

struct Obj *Func_02005208();
void Func_0200541c();
s32 Func_02005210();
s32 Func_0200522a();
s32 Func_02005238();
void Func_02005294();
void Func_020052a4();

void Func_020027ac(void)
{
    struct Obj *p;
    struct Sub *sub;
    s32 *reference;
    s32 height;
    s32 x;
    s32 z;
    s32 v;
    s32 w;

    p = 0;

    switch ((u32)Data_0200b694) {
    case 1:
        if (Data_0200b690 <= 0x3a97) {
            Data_0200b690 = Data_0200b690 + 50;
        }
        if (Data_0200b68c > 0x003c0000) {
            Data_0200b68c = Data_0200b68c + (s32)0xffffc000;
        }
        break;
    case 2:
        if (Data_0200b690 <= 0x752f) {
            Data_0200b690 = Data_0200b690 + 50;
        }
        if (Data_0200b68c > 0x00180000) {
            Data_0200b68c = Data_0200b68c + (s32)0xffffc000;
        }
        break;
    case 3:
        height = Data_0200b68c;
        if (height < (s32)0xff800000) {
            Data_0200b694 = 0;
        } else {
            Data_0200b690 = Data_0200b690 + 50;
            Data_0200b68c = height + (s32)0xffffc000;
        }
        break;
    default:
        break;
    }

    if ((FRAME_COUNTER & 7) != 0) {
        return;
    }

    p = Func_02005208(0x11d, 0, 0, 0);
    if (p == 0) {
        return;
    }

    reference = REFERENCE_RECORD;

    if ((FRAME_COUNTER & 0x3f) == 0) {
        Func_0200541c(246);
    }

    if (Data_0200b694 != 0) {
        x = reference[0] + (s32)(((u32)(Data_0200b690 * Func_02005210()) >> 16) << 8) + Data_0200b68c;
    } else {
        x = reference[0] + (Func_0200522a() << 8) + (s32)0xff800000;
    }

    z = reference[2] + (Func_02005238() << 8) + (s32)0xff800000;

    p->f55 = 0;
    p->f0c = 0x00a00000;
    sub = p->f50;
    p->f18 = 0xe666;
    p->f1c = 0xe666;
    p->f08 = x;
    p->f10 = z;
    sub->f26 = 0;
    v = 0xfe;
    v &= p->f23;
    p->f23 = v;
    w = ~12;
    w &= sub->f09;
    w |= 4;
    sub->f09 = w;

    Func_02005294(p, 1);
    Func_020052a4(p, Data_0200b610);
}
