#include "types.h"

/*
 * Resource 394 scene setup.
 *
 * The entry at 0x020003f0 has its own saved-register/stack prologue and the
 * matching outer return at 0x020007dc.  Several called service addresses fall
 * numerically inside that interval because overlay-local code and resident
 * services share the 0x02000000 address namespace.  They remain calls here:
 * treating their apparent local labels as standalone C owners would invent
 * hidden-context ABIs.
 *
 * The helpers are deliberately left with old-style declarations until their
 * shared interfaces are established from their own implementations.  Calls
 * below preserve every observed argument, including the two or three stack
 * arguments used by the six/seven-argument placement helpers.
 */
/* Resolved whole-owner call targets. */
extern void Func_02000098();
extern void Func_02000194();
extern void Func_080000c0();
extern void Func_080000d0();
extern void Func_080000d8();
extern void Func_08000130();
extern void Func_08009128();
extern void Func_080091b8();
extern void Func_08015040();
extern void Func_0808a018();
extern void Func_0808a020();
extern void Func_0808a0f0();
extern void Func_0808a100();
extern void Func_0808a208();
extern void Func_0808a210();
extern void Func_0808a218();
extern void Func_080f9010();

static s16 scene_variant(void)
{
    volatile s16 *state = *(volatile s16 **)0x020092c8;
    return state[0];
}

void Func_020003f0(void)
{
    volatile s32 *timer = (volatile s32 *)0x0200a0dc;
    volatile u16 *displayWord;
    s32 variant;

    Func_0808a018();
    Func_0808a208(0x10000, 0x4000);
    Func_0808a210(0x1080000, -1, 0x1c00000, 1);
    Func_0808a218();
    Func_08015040(0x1528, 1);
    Func_080f9010(232);

    variant = scene_variant();
    if (variant == 0) {
        Func_0808a0f0(9, 0x1000000, 0x1ce0000);
        Func_080091b8(77, 34, 1, 2, 83, 25);
        Func_080000c0(3);
        Func_080091b8(78, 34, 1, 2, 83, 25);
        Func_080000c0(3);
        Func_080091b8(79, 34, 1, 2, 83, 25);
        Func_080000c0(30);
        Func_080091b8(67, 34, 2, 5, 79, 25);
        Func_080000c0(6);
        Func_080091b8(69, 34, 2, 5, 79, 25);
        Func_0808a100(9, 1);
        Func_080f9010(240);
        Func_080000c0(6);
        Func_080091b8(71, 34, 2, 5, 79, 25);
        Func_080000c0(6);
        Func_080091b8(73, 34, 2, 5, 79, 25);
        Func_080091b8(75, 38, 2, 1, 79, 29);
        Func_080000c0(4);
        Func_080091b8(77, 38, 2, 1, 79, 29);
        Func_080000c0(6);
        Func_080091b8(79, 38, 2, 1, 79, 29);
        Func_080000c0(8);
        Func_080091b8(65, 53, 2, 1, 79, 29);
        Func_080091b8(65, 40, 2, 4, 15, 28);
    } else {
        Func_0808a0f0(9, 0x1000000, 0x1e00000);
        Func_080091b8(78, 34, 1, 2, 83, 25);
        Func_080000c0(3);
        Func_080091b8(77, 34, 1, 2, 83, 25);
        Func_080000c0(3);
        Func_080091b8(76, 34, 1, 2, 83, 25);
        Func_080000c0(30);
        Func_080091b8(65, 45, 2, 4, 15, 28);
        Func_080091b8(71, 50, 2, 5, 79, 25);
        Func_0808a100(9, 2);
        Func_080f9010(230);
        Func_080000c0(6);
        Func_080091b8(69, 50, 2, 5, 79, 25);
        Func_080000c0(6);
        Func_080091b8(67, 50, 2, 5, 79, 25);
        Func_080000c0(6);
        Func_080091b8(65, 50, 2, 5, 79, 25);
        Func_080000c0(30);
    }

    /*
     * These three calls choose the resource-specific placement family.  Their
     * numeric targets overlap this overlay but are resident service entries;
     * no local-label function is asserted here.
     */
    if (variant == 0) {
        Func_02000098(9, 19, 16, 5, 0, 9, 30);
        Func_02000098(9, 51, 16, 5, 1, 9, 30);
        Func_02000098(41, 51, 16, 5, 2, 9, 30);
    } else {
        Func_02000098(9, 19, 16, 5, 0, 9, 30);
        Func_02000098(9, 83, 16, 5, 1, 9, 30);
        Func_02000098(41, 83, 16, 5, 2, 9, 30);
    }

    *timer = 0;
    Func_080000d0((void *)0x020083c1, 2000);
    Func_080000c0(1);
    Func_08000130(1, 0, (void *)0x0200836d);
    Func_080f9010(231);
    *timer = 0;
    do {
        Func_080000c0(1);
        ++*timer;
    } while (*timer <= 100);
    Func_080f9010(0x121);

    variant = scene_variant();
    if (variant == 0) {
        Func_02000098(9, 19, 16, 5, 0, 9, 19);
        Func_02000098(9, 51, 16, 5, 1, 9, 19);
        Func_02000098(41, 51, 16, 5, 2, 9, 19);
    } else {
        Func_02000098(9, 19, 16, 5, 0, 9, 19);
        Func_02000098(9, 83, 16, 5, 1, 9, 19);
        Func_02000098(41, 83, 16, 5, 2, 9, 19);
    }

    Func_080000c0(1);
    Func_08000130(1, 0, 0);
    Func_080000c0(1);
    Func_080000d8((void *)0x020083c1);

    displayWord = *(volatile u16 **)0x020092c8;
    *displayWord ^= 1;
    Func_02000194();
    Func_08009128();
    Func_0808a020();
}
