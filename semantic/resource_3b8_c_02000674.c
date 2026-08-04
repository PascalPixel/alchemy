#include "types.h"

/*
 * Resource 3b8 cutscene at 0x02000674.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000674 and the matching
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return at 0x02000ad4.  Two
 * literal pools sit inside the span and are branched over: 0x020006da-0x020006e7
 * (the branch at 0x020006d8 jumps to 0x020006e8) and 0x02000ada-0x02000af7,
 * which follows the return.  Both are data.
 *
 * Resolver-confirmed main-image symbols name every external call below.
 * Old-style declarations keep the source honest where a shared engine helper
 * is reached with more than one argument shape.
 *
 * The only two absolute addresses this owner touches are the GBA display
 * control register at 0x04000000 and the IWRAM workspace pointer at
 * 0x03001ebc, the same pointer 0x020040b4 uses.
 */

void Func_080000c0();
void Func_080091e0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a360();
void Func_0808a370();
void Func_0808a580();

void Func_02000674(void)
{
    volatile u16 *displayControl = (volatile u16 *)0x04000000;
    u8 **workspace = (u8 **)0x03001ebc;
    u8 *entry;
    s32 gate;

    Func_0808a018();
    Func_0808a170(0x2280);

    entry = Func_0808a080(0);
    entry[0x54] = 0;
    entry = Func_0808a080(10);
    entry[0x54] = 0;

    Func_080000c0(1);
    *displayControl = 0x1140;
    Func_0808a180(-1, 0);
    *displayControl = 0x0140;

    entry = Func_0808a080(0);
    entry[0x54] = 1;
    entry = Func_0808a080(10);
    entry[0x54] = 1;

    Func_0808a100(0, 31);
    /* Pass the selected scene record directly to the engine handoff. */
    Func_080091e0(Func_0808a080(0), 0);

    Func_0808a0f0(1, 0x780000, 0x680000);
    Func_0808a0f0(3, 0x680000, 0x500000);
    Func_0808a0f0(2, 0x780000, 0x780000);
    Func_0808a1b8(1, 0, 0);
    Func_0808a1b8(3, 0, 0);
    Func_0808a1b8(2, 0xe000, 0);

    *(u32 *)(*workspace + 0x1c8) = 60;
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    *(u32 *)(*workspace + 0x1c8) = 24;

    Func_0808a090(3, 0x10000, 0x8000);
    Func_0808a580(3, 16, 0);
    Func_0808a1b8(3, 0x2000, 0);
    Func_0808a010(20);
    Func_0808a138(3, 2);
    Func_0808a010(30);
    Func_0808a180(3, 0);
    Func_0808a010(10);

    /* 0xfffd0000 is a negative fixed-point bias applied to two different
     * fields of two different records. */
    entry = Func_0808a080(0);
    *(u32 *)(entry + 16) += 0xfffd0000;
    entry = Func_0808a080(0);
    *(u32 *)(entry + 64) += 0xfffd0000;

    Func_0808a100(0, 32);
    Func_0808a010(40);
    Func_0808a110(0, 34);
    Func_0808a010(30);
    Func_0808a100(0, 33);
    Func_0808a010(50);
    Func_0808a138(1, 2);
    Func_0808a010(30);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a1e8(0, 0x105, 60);
    Func_0808a010(20);
    Func_0808a1e8(1, 0x102, 60);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a110(1, 4);
    Func_0808a010(20);
    Func_0808a180(1, 0);
    Func_0808a010(10);
    Func_0808a1e8(0, 0x102, 80);
    Func_0808a1e8(2, 0x106, 60);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_0808a180(2, 0);
    Func_0808a010(10);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a010(30);
    Func_0808a138(1, 2);
    Func_0808a010(45);
    Func_0808a1b8(1, 0, 0);
    Func_0808a1b8(2, 0xe000, 0);
    Func_0808a010(30);
    Func_0808a178(1, 0);

    gate = Func_0808a070(-1, 0);
    if (gate != 0) {
        Func_0808a010(10);
        Func_0808a110(0, 34);
        Func_0808a010(20);
        Func_0808a110(1, 3);
        Func_0808a010(20);
        Func_0808a180(1, 0);
        Func_0808a010(10);
        Func_0808a110(0, 33);
        Func_0808a010(30);
        Func_0808a110(1, 3);
        Func_0808a010(20);
        Func_0808a180(1, 0);
        *(u16 *)(*workspace + 0x1d8) += 1;
    } else {
        Func_0808a010(10);
        Func_0808a110(0, 33);
        *(u16 *)(*workspace + 0x1d8) += 2;
        Func_0808a010(30);
        Func_0808a110(1, 3);
        Func_0808a010(20);
        Func_0808a180(1, 0);
    }

    Func_0808a010(10);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a580(1, -16, 0);
    Func_0808a1b8(1, 0, 0);
    Func_0808a010(35);
    Func_0808a128(0, 6, 0);
    Func_0808a090(0, 0x1e666, 0xf333);
    Func_0808a580(0, -32, 0);
    /* Repeat the handoff after the camera transition. */
    Func_080091e0(Func_0808a080(0), 1);
    Func_0808a1b8(3, 0x4000, 0);
    Func_0808a1b8(2, 0xc000, 0);
    Func_0808a010(40);
    Func_0808a100(0, 3);
    Func_0808a010(30);
    Func_0808a100(2, 3);
    Func_0808a100(1, 3);
    Func_0808a110(3, 3);
    Func_0808a010(30);
    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a090(3, 0x13333, 0x9999);
    Func_0808a090(2, 0x13333, 0x9999);

    Func_0808a100(1, 2);
    entry = Func_0808a080(0);
    if (entry != 0)
        Func_0808a0b8(1, *(s16 *)(entry + 10), *(s16 *)(entry + 18));
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    Func_0808a100(3, 2);
    entry = Func_0808a080(0);
    if (entry != 0)
        Func_0808a0b8(3, *(s16 *)(entry + 10), *(s16 *)(entry + 18));
    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);

    Func_0808a100(2, 2);
    entry = Func_0808a080(0);
    if (entry != 0)
        Func_0808a0b8(2, *(s16 *)(entry + 10), *(s16 *)(entry + 18));
    Func_0808a0e8(2);
    Func_0808a0f0(2, 0, 0);

    /* Pause once more after restoring the record state. */
    Func_0808a010(10);
    Func_0808a020();
}
