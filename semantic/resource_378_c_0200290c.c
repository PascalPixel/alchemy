#include "types.h"

extern void Func_02003098(s32 presentation, s32 x, s32 y, s32 z);
extern void Func_0808a010();
extern u8 *Func_0808a080();
extern void Func_0808a090();
extern void Func_0808a098();
extern void Func_0808a0a0();
extern void Func_0808a0c8();
extern void Func_0808a0d0();
extern void Func_0808a0e8();
extern void Func_0808a100();
extern void Func_0808a110();
extern void Func_0808a128();
extern void Func_0808a130();
extern void Func_0808a138();
extern void Func_0808a148();
extern void Func_0808a150();
extern void Func_0808a1b8();
extern void Func_0808a1e8();

/*
 * One uninterrupted two-lead ensemble choreography.  Actors 0 and 1 trade
 * focus through actors 8-12, perform two mirrored turning patterns, then
 * converge on paired scripts at 0x0200b740/0x0200b81c before the closing
 * formation.  Every wait and every alternative pose is authored as a
 * distinct machine call site; none of the repeated beats are synthetic loops.
 *
 * Complete owner: 0x0200290c through the sole unwind return at 0x02003094,
 * alignment and its three-word pool through 0x02003097 (1,932 bytes).
 */
void Func_0200290c(void)
{
    u8 *actor;

    Func_0808a090(0, 0x18000, 0xc000);
    Func_0808a090(1, 0x18000, 0xc000);
    Func_0808a138(12, 2);
    Func_0808a010(10);
    Func_0808a110(12, 3);
    Func_0808a010(10);
    Func_0808a110(0, 3);
    Func_0808a010(15);
    Func_0808a148(0, 1, 0);
    Func_0808a130(0, 1);
    actor = Func_0808a080(0);
    actor[90] &= (u8)~1;
    Func_0808a0c8(0, 184, 168);
    actor = Func_0808a080(1);
    actor[90] &= (u8)~1;
    Func_0808a0d0(1, 200, 168);
    Func_0808a010(1);
    actor = Func_0808a080(1);
    actor[90] |= 1;
    Func_0808a0e8(0);
    Func_0808a100(0, 1);
    actor = Func_0808a080(0);
    actor[90] |= 1;
    actor = Func_0808a080(1);
    actor[90] |= 1;

    Func_0808a128(1, 2, 0);
    Func_0808a010(15);
    Func_0808a148(1, 8, 0);
    Func_0808a010(5);
    Func_0808a128(1, 2, 0);
    Func_0808a010(25);
    Func_0808a138(1, 2);
    Func_0808a148(1, 12, 0);
    Func_0808a010(5);
    Func_0808a110(1, 3);
    Func_0808a010(10);
    Func_0808a110(0, 3);
    Func_0808a010(5);
    Func_0808a110(0, 3);
    Func_0808a010(10);
    Func_0808a148(1, 0, 0);
    Func_0808a010(10);
    Func_0808a110(1, 3);
    Func_0808a010(15);

    Func_0808a100(11, 3);
    Func_0808a100(12, 3);
    Func_0808a100(8, 3);
    Func_0808a100(9, 3);
    Func_0808a110(10, 3);
    Func_0808a010(20);
    Func_0808a148(0, 12, 0);
    Func_0808a148(1, 12, 0);
    Func_0808a010(10);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a148(0, 11, 0);
    Func_0808a148(1, 11, 0);
    Func_0808a010(10);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(20);

    Func_0808a1b8(0, 0, 0);
    Func_0808a010(15);
    Func_0808a138(0, 2);
    Func_0808a010(10);
    Func_02003098(222, 0x00b80000, 0x001b0000, 0x00a80000);
    Func_0808a148(1, 0, 0);
    Func_0808a010(10);
    Func_0808a138(1, 1);
    Func_0808a010(10);
    Func_0808a128(1, 4, 0);

    /* First mirrored five-turn pattern. */
    Func_0808a1b8(1, 0xd000, 0);
    Func_0808a010(15);
    Func_0808a1b8(1, 0xb000, 0);
    Func_0808a010(10);
    Func_0808a1b8(1, 0xd000, 0);
    Func_0808a010(10);
    Func_0808a1b8(1, 0xb000, 0);
    Func_0808a010(10);
    Func_0808a1b8(1, 0xd000, 0);
    Func_0808a010(30);
    Func_0808a128(1, 4, 0);

    /* Second mirrored five-turn pattern. */
    Func_0808a1b8(1, 0x3000, 0);
    Func_0808a010(15);
    Func_0808a1b8(1, 0x5000, 0);
    Func_0808a010(10);
    Func_0808a1b8(1, 0x3000, 0);
    Func_0808a010(10);
    Func_0808a1b8(1, 0x5000, 0);
    Func_0808a010(10);
    Func_0808a1b8(1, 0x3000, 0);
    Func_0808a010(30);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a010(10);
    Func_0808a1e8(0, 0x102, 0);
    Func_0808a1e8(1, 0x102, 0);
    Func_0808a010(60);
    Func_0808a148(1, 12, 0);
    Func_0808a010(10);
    Func_0808a110(1, 3);
    Func_0808a010(10);

    /* Give actors 8 and 12 the shared per-frame callback for this beat. */
    actor = Func_0808a080(8);
    *(u16 *)(actor + 100) = 1;
    actor = Func_0808a080(8);
    *(u16 *)(actor + 100) = 1;
    *(void **)(actor + 108) = (void *)0x02008031;
    actor = Func_0808a080(12);
    actor = Func_0808a080(12);
    *(void **)(actor + 108) = (void *)0x02008031;

    Func_0808a0d0(1, 196, 180);
    Func_0808a0d0(1, 184, 184);
    Func_0808a0d0(1, 180, 180);
    Func_0808a0d0(1, 168, 168);
    Func_0808a0d0(1, 180, 156);
    Func_0808a0c8(1, 200, 104);
    Func_0808a0d0(0, 192, 168);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a0e8(1);
    Func_0808a010(30);
    Func_0808a138(1, 1);
    Func_0808a010(10);
    Func_0808a1b8(1, 0x5000, 0);
    Func_0808a010(15);

    actor = Func_0808a080(12);
    *(void **)(actor + 108) = 0;
    actor = Func_0808a080(8);
    *(void **)(actor + 108) = 0;
    Func_0808a130(8, 2);
    Func_0808a1e8(8, 0x100, 0);
    Func_0808a010(60);
    Func_0808a100(8, 0);
    Func_0808a1e8(0, 0x102, 0);
    Func_0808a010(60);
    Func_0808a150(0, 11, 0);
    Func_0808a010(10);
    Func_0808a110(0, 3);
    Func_0808a010(10);
    Func_0808a110(11, 3);
    Func_0808a010(10);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a010(10);
    Func_0808a138(0, 2);
    Func_0808a010(10);
    Func_0808a128(0, 2, 0);
    Func_0808a010(20);
    Func_0808a128(0, 2, 0);
    Func_0808a010(20);
    Func_0808a010(15);
    Func_0808a150(0, 12, 0);
    Func_0808a010(10);
    Func_0808a110(0, 3);
    Func_0808a010(10);
    Func_0808a110(12, 3);
    Func_0808a010(60);

    Func_0808a0d0(1, 208, 168);
    Func_0808a148(0, 11, 0);
    Func_0808a148(1, 12, 0);
    Func_0808a010(10);
    Func_0808a100(0, 4);
    Func_0808a110(1, 4);
    Func_0808a010(10);
    Func_0808a130(0, 1);
    Func_0808a138(1, 1);
    Func_0808a010(10);
    Func_0808a1b8(1, 0, 0);
    Func_0808a010(10);
    Func_0808a110(1, 3);
    Func_0808a010(10);
    Func_0808a148(1, 12, 0);
    Func_0808a010(10);
    Func_0808a110(12, 3);
    Func_0808a010(10);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(1, 0, 0);
    Func_0808a010(10);
    Func_0808a130(0, 2);
    Func_0808a138(1, 2);
    Func_0808a010(10);

    Func_0808a098(0, (const void *)0x0200b740);
    Func_0808a098(1, (const void *)0x0200b81c);
    Func_0808a0a0(0);
    Func_0808a0a0(1);
    Func_0808a090(0, 0x18000, 0xc000);
    Func_0808a090(1, 0x18000, 0xc000);
    Func_0808a128(0, 6, 0);
    Func_0808a128(1, 6, 0);

    /* Three one-frame focus exchanges. */
    Func_0808a148(0, 9, 0);
    Func_0808a148(1, 8, 0);
    Func_0808a010(1);
    Func_0808a148(0, 12, 0);
    Func_0808a148(1, 11, 0);
    Func_0808a010(1);
    Func_0808a148(0, 8, 0);
    Func_0808a148(1, 9, 0);
    Func_0808a010(1);

    Func_0808a0c8(0, 192, 168);
    Func_0808a0d0(1, 208, 168);
    Func_0808a0e8(0);
    Func_0808a010(10);
    Func_0808a1b8(0, 0x3000, 0);
    Func_0808a1b8(1, 0xd000, 0);
    Func_0808a010(10);
    Func_0808a1b8(0, 0x5000, 0);
    Func_0808a1b8(1, 0xb000, 0);
    Func_0808a010(10);
    Func_0808a1b8(0, 0x3000, 0);
    Func_0808a1b8(1, 0xd000, 0);
    Func_0808a010(10);
    Func_0808a1b8(0, 0x5000, 0);
    Func_0808a1b8(1, 0xb000, 0);
    Func_0808a010(20);

    Func_0808a148(0, 11, 0);
    Func_0808a148(1, 12, 0);
    Func_0808a010(20);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(10);
    Func_0808a100(11, 3);
    Func_0808a110(12, 3);
    Func_0808a010(30);
    Func_0808a150(0, 1, 0);
    Func_0808a010(20);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(10);
    Func_0808a1b8(1, 0, 0);
    Func_0808a010(10);
    Func_0808a100(0, 2);
    Func_0808a100(1, 2);
    Func_0808a010(60);
}
