#include "TYPES.H"

#define HaidiaMura_RunCameraRiseScene Func_020010d8

void Func_02007076();
s32 Func_02007082();
void Func_020070a6();
void Func_020070b0();
void Func_020070c4();
void Func_020070ee();
void Func_020070f0();
void Func_02007112();
s32 Func_02007114();
void Func_02007120();
void Func_0200712e();
void Func_02007142();
void Func_0200714e();
void Func_02007156();
void Func_02007174();
void Func_0200719a();
void Func_020071bc();
void Func_020071c6();
void Func_020071d0();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Until flag 0x808 is set, raise the camera 40 steps above an actor while
 * message 0xf4d plays, show message 0xf4f, lower it again and return the
 * camera to its target. */
void HaidiaMura_RunCameraRiseScene(void)
{
    s32 **cam;
    s32 *saved;
    s32 *rec;
    s32 n;
    s32 pos[3];

    if (Value1(Func_02007082, 0x808) == 0) {
        cam = *(s32 ***)0x03001e70;
        Func_020070b0();
        Call3(Func_020070ee, 0, 0x10000, 0x8000);
        Func_0200714e(0, 1);
        Func_020070c4(2);
        Call1(Func_0200719a, 0xf4d);
        Func_020071bc(15, 0, 2);
        Func_020071c6(16, 0, 2);
        rec = (s32 *)Value1(Func_02007114, 0);
        pos[0] = rec[2];
        pos[1] = rec[3];
        pos[2] = rec[4];
        saved = *cam;
        *cam = pos;
        n = 0;
        do {
            pos[2] += 0x20000;
            Func_02007112(1);
            n++;
            Func_02007076();
        } while (n != 40);
        Func_02007120(60);
        Call2(Func_020070f0, 0xf4f, 1);
        n = 0;
        Func_0200712e(6);
        do {
            pos[2] -= 0x20000;
            Func_02007142(1);
            n++;
            Func_020070a6();
        } while (n != 40);
        *cam = saved;
        Func_02007156(60);
        Call3(Func_020071d0, 0, 70, 0x2e5);
        Func_02007174();
    }
}
