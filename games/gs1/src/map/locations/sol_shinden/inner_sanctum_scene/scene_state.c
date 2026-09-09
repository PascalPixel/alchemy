#include "types.h"

#define ClearSolShindenBackdrop Func_0200022c
#define SetStatueLightGroup1 Func_02000238
#define SetStatueLightGroup2 Func_020002cc
#define SetStatueLightGroup3 Func_02000360
#define SetStatueLightGroup4 Func_020003f4
#define SetSolShindenActorStep Func_020025fc
/*
 * resource_37a owner at 0x020025fc, 24 bytes: a two-argument shim --
 * it hands its FIRST argument to one main-image routine with a zero, then
 * its SECOND to another, and returns nothing.
 *
 * Complete owner: `push {r5, lr}` at 0x020025fc through the single
 * epilogue `pop {r5} / pop {r0} / bx r0` at 0x0200260c-0x02002610,
 * no literal pool. Next owner at 0x02002614. Measured to the epilogue.
 *
 * BOTH arguments are real. r1 is saved into r5 at 0x020025fe BEFORE r1
 * is overwritten with 0 for the first call, and r0 reaches the first call
 * untouched -- so the row reads both incoming registers before writing
 * either. That save is the only reason r5 is pushed at all.
 *
 * Call sites resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_37a
 * 25fc 2614`'s `+2` rule: `sites=2 veneer=2`, and two are transcribed.
 *
 * NOT PUBLISHED -- it is the one owner in this overlay's residue that is
 * bl-reached instead. Every 4-aligned word was scanned for 0x0200a5fc
 * with and without the Thumb bit and there are none, which matches sweep A
 * having found it and sweep B not.
 *
 * It is also the whole of this overlay's single sweep-D gap
 * (`CODE-SUSPECT 0x25fc-0x2613 24B after 0x25b0`), which is why sweep D was
 * run FIRST: the unkeyed sweep and the keyed one agreed on this address, and
 * agreement between instruments that work differently is worth more than
 * either alone.
 *
 * Uncertainty: neither Func_0808a180 nor Func_0808a010 is established here,
 * so both are declared old-style and the zero passed to the first is
 * transcribed rather than interpreted.
 */
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

void SetSolShindenActorStep(s32 actor_step, s32 wait_frames)
{
    Func_02005068(actor_step, 0);
    Func_02004fd6(wait_frames);
}
