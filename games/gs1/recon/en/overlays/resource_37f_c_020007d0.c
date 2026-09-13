#include "types.h"

extern s16 Data_02000240_t[][1];
extern s32 Data_0200a65c_t[][1];
u8 *Func_020023ac(s32);
u8 *Func_02000fa6(s32, s32);
s32 Func_020023aa(u8 *, s32 *);
void Func_02002386(u8 *, s32);
void Func_02002376(s32);
void Func_02002524(s32);
void Func_020023b4(u8 *, s32, s32, s32);
void Func_020023c6(u8 *, s32, s32, s32);
void Func_020023d4(u8 *);
void Func_020023d2(u8 *, s32);
void Func_02000e20(void);
void Func_02000e64(void);
void Func_02000eaa(void);
void Func_02000ef6(void);
void Func_02000f42(void);
void Func_02000f8e(void);
void Func_02000fda(void);
void Func_02001026(void);

void Func_020007d0(void)
{
    s32 pos[3];
    u8 *actor = Func_020023ac(0);
    u32 dir = *(u16 *)(actor + 6) >> 12;
    s32 offset = Data_0200a65c_t[dir][0];
    u8 *obj = Func_02000fa6((*(s16 *)(actor + 10) + (offset >> 16)) >> 4,
                          (*(s16 *)(actor + 18) + (s16)offset) >> 4);
    s32 state;
    s32 zero;
    s32 step;

    if (obj == 0) return;
    zero = 0;
    obj[34] = 2;
    step = Data_0200a65c_t[dir][0];
    pos[0] = *(s32 *)(obj + 8) + (step & 0xffff0000);
    pos[1] = *(s32 *)(obj + 12);
    pos[2] = *(s32 *)(obj + 16) + (step << 16);
    if (Func_020023aa(obj, pos) > 0) return;
    Func_02002386(actor, 8);
    Func_02002376(15);
    Func_02002524(185);
    *(s32 *)(obj + 48) = 0x3333;
    *(s32 *)(obj + 52) = 0x3333;
    Func_020023b4(obj, pos[0], pos[1], pos[2]);
    *(s32 *)(actor + 48) = 0x3333;
    *(s32 *)(actor + 52) = 0x3333;
    Func_020023c6(actor, pos[0], pos[1], pos[2]);
    Func_020023d4(obj);
    *(s32 *)(obj + 8) = pos[0];
    *(s32 *)(obj + 16) = pos[2];
    *(s32 *)(obj + 36) = zero;
    *(s32 *)(obj + 44) = zero;
    Func_020023d2(actor, 1);
    state = Data_02000240_t[225][0];
    switch (state) {
    case 11: case 12: case 13:
        Func_02000e20();
        Func_02000e64();
        break;
    case 14: case 15: case 16:
        Func_02000eaa();
        Func_02000ef6();
        Func_02000f42();
        Func_02000f8e();
        Func_02000fda();
        Func_02001026();
    }
}
