#include "types.h"
#define NULL ((void *)0)

struct Obj {
    u8 pad00[6];
    u16 f06;
    u8 pad08[0x30];
    s32 f38;
    s32 f3c;
    s32 f40;
};

extern u8 Data_0200ae34[];
struct Obj *Func_02002d3e(s32);
void Func_02002d1c(void);
void Func_02002dae(s32, s32);
void Func_02002d7c(s32);
void Func_02002e40(s32, s32, s32);
void Func_02002d44(s32);
void Func_02002dec(s32, s32);
void Func_02002e22(s32);
void Func_02002e44(s32, s32, s32);
void Func_02002e16(s32, s32, s32);
void Func_02002e0e(s32, s32);
void Func_02002e56(s32, s32);
void Func_02002d64(s32);
void Func_02002dd2(s32);
void Func_02002ce0(s32);
void Func_02002dd0(s32, void *);
void Func_02002da4(void);

void Func_02000674(void) {
    struct Obj *p = Func_02002d3e(21);
    Func_02002d1c();
    p->f38 = 0x80000000;
    p->f3c = 0x80000000;
    p->f40 = 0x80000000;
    Func_02002dae(21, 1);
    Func_02002d7c(21);
    Func_02002e40(21, 256, 40);
    p->f06 = 0xb000;
    Func_02002d44(20);
    Func_02002dec(21, 2);
    Func_02002e22(0x1c94);
    Func_02002e44(21, 0, 40);
    Func_02002e16(21, 0, 20);
    Func_02002e0e(21, 2);
    Func_02002e56(21, 0);
    Func_02002d64(0x306);
    Func_02002dd2(21);
    Func_02002ce0(1);
    Func_02002dd0(21, Data_0200ae34);
    Func_02002da4();
}
