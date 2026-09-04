#include "types.h"

extern u8 Value_00001360;


extern void Func_02005574(void);
extern s32 Func_0200554a(s32);
extern void Func_02005656(s32);
extern void Func_02004e3e(s32, s32);
extern void Func_02004e90(s32, s32, s32);
extern void Func_02004e50(s32, s32);
extern void Func_02005692(s32, s32, s32);
extern void Func_020055a8(s32);
extern void Func_02005670(s32, s32);
extern void Func_020055b6(s32);
extern void Func_02004ea8();
extern void Func_02004eca(s32, s32, s32);
extern s32 Func_020055d6(void);
extern void Func_020056c0(s32);
extern void Func_02005604(void);
extern void Func_020055dc(s32);
extern void Func_02005634(s32, s32);
extern void Func_020056e2(s32);
extern void Func_020056f2(s32, s32);
extern void Func_02005618(s32);
extern s32 Func_02005658(s32, s32);
extern void Func_02005724(s32, s32);
extern void Func_02005650(void);

void Func_0200082c(void)
{
    u8 *state;
    s32 k;

    Func_02005574();
    if (Func_0200554a(0x857) == 0) {
        k = (s32)&Value_00001360;
        Func_02005656(k);
        Func_02004e3e(16, 20);
        Func_02004e90(16, 3, 20);
        Func_02004e50(16, 30);
        Func_02005692(16, 0, 0);
        Func_020055a8(30);
        Func_02005670(16, 2);
        Func_020055b6(30);
        Func_02004ea8(0, 16, 20);
        Func_02004eca(16, 3, 20);
        state = *(u8 **)0x03001ebc;
        *(u16 *)(state + 472) += 1;
        if (Func_020055d6() == 0) {
            Func_020056c0(k + 3);
            Func_02004ea8(16, 20);
            Func_02005604();
            return;
        }
        Func_020055dc(0x857);
        Func_02005634(0xbd, 0);
    }
    Func_020056e2(0x1364);
    Func_020056f2(16, 0);
    Func_02005618(20);
    if (Func_02005658(0, 0) != 0) {
        state = *(u8 **)0x03001ebc;
        *(u16 *)(state + 472) += 1;
    }
    Func_02005724(16, 0);
    Func_02005650();
}
