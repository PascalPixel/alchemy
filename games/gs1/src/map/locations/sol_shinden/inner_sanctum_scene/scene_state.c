/*
 * Inner sanctum scene state: clear the backdrop colour, light the statue
 * groups whose progress flags are set, and step an actor. Overlay
 * resource_37a.
 */

#include "types.h"

#define ClearSolShindenBackdrop Func_0200022c
#define SetStatueLightGroup1 Func_02000238
#define SetStatueLightGroup2 Func_020002cc
#define SetStatueLightGroup3 Func_02000360
#define SetStatueLightGroup4 Func_020003f4
#define SetSolShindenActorStep Func_020025fc
/* Declared old-style: this parameter list is not established here. */
extern void Func_02005068();

s32 Func_02002bf4(s32);
void Func_02002bec(s32, s32, s32, s32, s32, s32);
s32 Func_02002c12(s32);
void Func_02002c0a(s32, s32, s32, s32, s32, s32);
s32 Func_02002c30(s32);
void Func_02002c28(s32, s32, s32, s32, s32, s32);
s32 Func_02002c4e(s32);
void Func_02002c46(s32, s32, s32, s32, s32, s32);
s32 Func_02002c88(s32);
void Func_02002c80(s32, s32, s32, s32, s32, s32);
s32 Func_02002ca6(s32);
void Func_02002c9e(s32, s32, s32, s32, s32, s32);
s32 Func_02002cc4(s32);
void Func_02002cbc(s32, s32, s32, s32, s32, s32);
s32 Func_02002ce2(s32);
void Func_02002cda(s32, s32, s32, s32, s32, s32);
s32 Func_02002d1c(s32);
void Func_02002d14(s32, s32, s32, s32, s32, s32);
s32 Func_02002d3a(s32);
void Func_02002d32(s32, s32, s32, s32, s32, s32);
s32 Func_02002d58(s32);
void Func_02002d50(s32, s32, s32, s32, s32, s32);
s32 Func_02002d76(s32);
void Func_02002d6e(s32, s32, s32, s32, s32, s32);
s32 Func_02002db0(s32);
void Func_02002da8(s32, s32, s32, s32, s32, s32);
s32 Func_02002dce(s32);
void Func_02002dc6(s32, s32, s32, s32, s32, s32);
s32 Func_02002dec(s32);
void Func_02002de4(s32, s32, s32, s32, s32, s32);
s32 Func_02002e0a(s32);
void Func_02002e02(s32, s32, s32, s32, s32, s32);
/* Declared old-style: this parameter list is not established here. */
void Func_02004fd6();

void ClearSolShindenBackdrop(void)
{
    s32 black = 0;
    u16 *backdrop_color = (u16 *)0x5000000;
    *backdrop_color = black;
}

void SetStatueLightGroup1(void)
{
    if (Func_02002bf4(0x80b) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002bec(0x2d, 28, 0x22, 10, a, b);
    }
    if (Func_02002c12(0x80c) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c0a(0x2f, 28, 0x24, 10, a, b);
    }
    if (Func_02002c30(0x80d) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c28(0x2d, 29, 0x22, 11, a, b);
    }
    if (Func_02002c4e(0x80e) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c46(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup2(void)
{
    if (Func_02002c88(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c80(0x2d, 28, 0x22, 10, a, b);
    }
    if (Func_02002ca6(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c9e(0x2f, 28, 0x24, 10, a, b);
    }
    if (Func_02002cc4(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002cbc(0x2d, 29, 0x22, 11, a, b);
    }
    if (Func_02002ce2(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002cda(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup3(void)
{
    if (Func_02002d1c(0x80b) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d14(0x2d, 30, 0x22, 10, a, b);
    }
    if (Func_02002d3a(0x80c) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d32(0x2f, 30, 0x24, 10, a, b);
    }
    if (Func_02002d58(0x80d) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d50(0x2d, 31, 0x22, 11, a, b);
    }
    if (Func_02002d76(0x80e) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d6e(0x2f, 31, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup4(void)
{
    if (Func_02002db0(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002da8(0x2d, 30, 0x22, 10, a, b);
    }
    if (Func_02002dce(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002dc6(0x2f, 30, 0x24, 10, a, b);
    }
    if (Func_02002dec(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002de4(0x2d, 31, 0x22, 11, a, b);
    }
    if (Func_02002e0a(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002e02(0x2f, 31, 0x24, 11, a, b);
    }
}

/*
 * Passes the step to one routine with a zero and the wait to another. Both
 * arguments are read before either incoming register is overwritten, so the
 * order of these two calls is what the reference requires. The zero is
 * transcribed, not interpreted.
 */
void SetSolShindenActorStep(s32 actor_step, s32 wait_frames)
{
    Func_02005068(actor_step, 0);
    Func_02004fd6(wait_frames);
}
