#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200702a();
void Func_02007040();
void Func_0200705a();
void Func_0200706e();
void Func_020070ba();
void Func_020070c8();
void Func_02007100();
void Func_02007114();
void Func_0200712a();
void Func_0200713e();
void Func_02007176();
void Func_02007184();
void Func_02007192();
void Func_020071be();
void Func_020071fa();
void Func_02007262();
void Func_0200733a();
void Func_020073c6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}


void Func_020016c4(s32 a0)
{
    u32 i;
    s32 x;
    s32 y;
    s32 k;
    s32 six;
    s32 pos;
    s32 four;
    Func_0200733a(219);
    six = 6;
    i = 0;
    x = 41;
    y = 40;
    do {
        Call6(Func_02007100, y, 32, x, 32, 3 - (s32)i, six);
        k = 2;
        Call6(Func_02007114, 39, 51, y, 32, 1, six);
        pos = (s32)i + 106;
        four = 4;
        Func_0200712a(105, 51, pos, 32, k, four);
        if (a0 != 0) {
            Call3(Func_02007176, 0x50000, 0x50000, 0x10000);
            Call3(Func_02007184, -1, -1, 0xe666);
            Func_020071fa(a0);
        }
        i = i + 1;
        x = x + 2;
        y = y + 2;
    } while (i <= 2);
    Call1(Func_020073c6, 0x120);
    Call6(Func_02007192, 106, 33, 4, 5, 42, 33);
    Func_020071be();
}
