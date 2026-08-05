#include "types.h"

/*
 * Resource 3b8 cutscene at 0x02000af8.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000af8 and the matching
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return at 0x02000fe2.  There
 * is one mid-function literal pool at 0x02000b3e-0x02000b4b, branched over by
 * the `b.n` at 0x02000b3c, and a trailing pool at 0x02000fe8-0x02000ff7.  Both
 * are data.
 *
 * Two distinct exits reach the epilogue.  The early test at 0x02000b32 jumps
 * to 0x02000fde, so it *does* run the closing import at 0x020053c8; the second
 * early exit at 0x02000b5c jumps to 0x02000fe2 and skips it.  That asymmetry is
 * reproduced below.
 *
 * The opening halfword read repeats the idiom of 0x02000108: the halfword at
 * +6 of the record returned by the first import, biased by 0x2000, masked with
 * 0xffffc000, then sign extended from bit 15 so only bits 14-15 survive.  Here
 * the value is afterwards re-narrowed to an unsigned halfword (lsls/lsrs #16)
 * before the two comparisons, so the tests are against 0 and 0x8000, not
 * against a negative number.
 *
 * Resolver-confirmed main-image symbols name every external call below.
 * Old-style declarations keep the source honest where a shared engine helper
 * is reached with more than one argument shape.
 */

s32 Func_080770c0();
void Func_080770c8();
void Func_0808a010();
s32 Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
void *Func_0808a080();
void Func_0808a090();
void Func_0808a0a0();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a210();
void Func_0808a460();
void Func_0808a570();
void Func_0808a578();
void Func_0808a580();
void Func_080b0068();

void Func_02000af8(void)
{
    u16 *context = Func_0808a080(0);
    s32 facing = (s16)(((s32)context[3] + 0x2000) & 0xc000);
    u16 quadrant;
    s16 *record;
    u8 **workspace = (u8 **)0x03001ebc;

    /* The opening result is passed directly into the scene initializer. */
    Func_0808a460(Func_0808a018(context));
    Func_0808a170(0x1ffb);
    Func_0808a178(-1, 0);

    if (Func_0808a070(0, 0) != 0) {
        goto close;
    }
    if (Func_080770c0(0x96a) != 0) {
        Func_0808a010(20);
        Func_080b0068(0);
        return;                 /* skips 0x020053c8 */
    }

    Func_0808a010(20);
    if (quadrant == 0)
        Func_0808a0d0(0, 128, 120);
    if (quadrant == 0x8000)
    quadrant = (u16)facing;
        Func_0808a0d0(0, 240, 120);

    Func_0808a0d0(0, 184, 120);
    Func_0808a1b8(0, 0, 0);
    Func_0808a010(10);
    Func_0808a570(1, 16, 0, 0x8000);
    Func_0808a0a0(1);
    Func_0808a010(10);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a580(1, 40, 0);
    Func_0808a580(1, 0, -32);
    Func_0808a1b8(1, 0x6000, 0);
    Func_0808a010(20);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a010(20);
    Func_0808a570(2, -16, 0, 0);
    Func_0808a0a0(2);
    Func_0808a010(10);
    Func_0808a180(2, 0);
    Func_0808a010(10);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a580(2, -40, 0);
    Func_0808a580(2, 0, 40);
    Func_0808a1b8(2, 0xe000, 0);
    Func_0808a010(20);
    Func_0808a1b8(0, 0, 0);
    Func_0808a010(20);
    Func_0808a570(3, 16, 0, 0x8000);
    Func_0808a0a0(3);
    Func_0808a010(10);
    Func_0808a180(3, 0);
    Func_0808a010(10);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a090(3, 0x10000, 0x8000);
    Func_0808a580(3, 40, 0);
    Func_0808a580(3, 0, 40);
    Func_0808a1b8(3, 0xa000, 0);
    Func_0808a010(20);
    Func_0808a210(-1, -1, -1, 0);
    Func_0808a580(0, -56, 0);
    Func_0808a580(0, 0, -32);
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a010(30);
    Func_0808a110(3, 3);
    Func_0808a010(10);
    Func_0808a180(3, 0);
    Func_0808a110(1, 3);
    Func_0808a010(10);

    if (Func_080770c0(0x96a) == 0)
        Func_0808a180(1, 0);
    else
        *(u16 *)(*workspace + 0x1d8) += 1;

    Func_0808a110(2, 3);
    Func_0808a010(10);

    if (Func_080770c0(0x96a) == 0)
        Func_0808a180(2, 0);
    else
        *(u16 *)(*workspace + 0x1d8) += 1;

    Func_0808a010(20);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a1b8(3, 0x8000, 0);
    Func_0808a1b8(2, 0, 0);
    Func_080b0068(0);
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a1b8(1, 0x6000, 0);
    Func_0808a1b8(3, 0xa000, 0);
    Func_0808a1b8(2, 0xe000, 0);
    Func_0808a010(20);
    Func_0808a110(1, 3);
    Func_0808a010(10);
    Func_0808a180(1, 0);
    Func_0808a110(2, 3);
    Func_0808a010(10);

    if (Func_080770c0(0x96a) == 0)
        Func_0808a180(2, 0);
    else
        *(u16 *)(*workspace + 0x1d8) += 1;

    Func_0808a110(3, 3);
    Func_0808a010(10);

    if (Func_080770c0(0x96a) == 0)
        Func_0808a180(3, 0);
    else
        *(u16 *)(*workspace + 0x1d8) += 1;

    Func_0808a580(1, 0, 32);
    Func_0808a578(1, -112, 0);
    Func_0808a580(3, 0, -40);
    Func_0808a578(3, -112, 0);
    Func_0808a010(50);
    Func_0808a580(2, 0, -24);
    Func_0808a0e8(1);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a580(1, 0, -16);
    Func_0808a0e8(3);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a090(2, 0x13333, 0x9999);
    Func_0808a090(3, 0x13333, 0x9999);

    Func_0808a100(1, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(1, record[5], record[9]);
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    Func_0808a100(3, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(3, record[5], record[9]);
    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);

    Func_0808a100(2, 2);
    record = Func_0808a080(0);
    if (record != 0)
        Func_0808a0b8(2, record[5], record[9]);
    Func_0808a0e8(2);
    Func_0808a0f0(2, 0, 0);

    /* Reached with the flag id, not as the record query above. */
    Func_080770c8(0x96a);
close:
    Func_0808a020();
}
