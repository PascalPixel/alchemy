#include "types.h"

/*
 * resource_3a6 actor-9 particle descent at 0x02000e04 (372 bytes).
 *
 * Complete saved-high-register owner through the interworking return at
 * 0x02000f6a, followed by the three directly referenced fixed-point pool words
 * through 0x02000f77. It is the actor-9 companion to the independently
 * reviewed actor-10 rain sequence at 0x02000f78, but every address, constant,
 * argument and call was read again here. All 29 BL sites are retained in
 * machine order: ten record fetches, two object creates, the 68-frame particle
 * loop, presentation/cue updates, both object destroys, and the shared close.
 */
u8 *Func_0808a080();
void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
void Func_0808a100();
void Func_0808a0c0();
void Func_0808a1b8();
void Func_080091c0();
void Func_080770c8();
void Func_080f9010();
void Func_080090d0();
s32 Func_080000f8(void);
s32 Func_02000048();
void Func_0200013c();

void Func_02000e04(void)
{
    u8 params[40];
    s32 left;
    s32 right;
    u32 index;
    s32 x;
    s32 z;

    Func_0808a018();
    if ((*(s32 *)(Func_0808a080(9) + 8) >> 20) != 23) goto close;

    Func_0808a0c0(0, 360, 664);
    Func_0808a1b8(0, 0xe000, 10);
    *(s32 *)(Func_0808a080(9) + 8) += 0x20000;

    left = Func_02000048(*(s32 *)(Func_0808a080(9) + 8) - 0x100000,
                         0,
                         *(s32 *)(Func_0808a080(9) + 16) + 0x340000,
                         241);
    right = Func_02000048(*(s32 *)(Func_0808a080(9) + 8) + 0x100000,
                          241);
                          0,
                          *(s32 *)(Func_0808a080(9) + 16) + 0x340000,
    Func_0808a080(9)[0x55] = 0;

    *(s32 *)(params + 8) = 0x9999;
    Func_080f9010(216);
    *(s32 *)(params + 12) = 0x9999;
    *(s32 *)(params + 4) = 7;

    for (index = 0; index <= 67; index++) {
        x = ((Func_080000f8() * 17) & ~0xffff) + 0x1b00000;
        z = ((Func_080000f8() * 14) & ~0xffff) + 0x2700000;
        Func_0200013c(x, 0, z, 0, 0, 0, 0x90000, params);
        *(s32 *)(Func_0808a080(9) + 12) += -0x8000;
        Func_0808a010(1);
    }

    Func_080091c0(23, 41, 1, 1, 23, 39);
    Func_0808a080(9)[0x23] |= 2;
    Func_080770c8(0x200);
    *(s32 *)(Func_0808a080(9) + 12) = -0x80000;
    Func_0808a100(9, 2);
    Func_080090d0(left);
    Func_080090d0(right);
    Func_0808a010(30);

close:
    Func_0808a020();
}
