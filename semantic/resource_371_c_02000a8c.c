#include "types.h"

/*
 * Resource 371 owner at 0x02000a8c (412 bytes).
 *
 * Complete owner: `push {lr}` at 0x02000a8c, the interworking return
 * `pop {r0}; bx r0` at 0x02000c16-0x02000c19, two alignment bytes, and the
 * three-word literal pool at 0x02000c1c-0x02000c27.  The preceding scene
 * dispatcher owns its own pool through 0x02000a8b; the next owner starts at
 * 0x02000c28.
 *
 * All 48 `bl` sites resolve through veneers to the 17 imports declared
 * below.  The three guarded actor lookups all copy the signed halfwords at
 * +0x0a and +0x12, the same actor-coordinate layout used by the following
 * owner at 0x02000c28.  Old-style declarations preserve the overlay's
 * call-site evidence without inventing unavailable interface prototypes.
 */

void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0e0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a1b8();
void Func_0808a248();
void Func_0808a360();
void Func_0808a368();
void Func_0808a370();
void Func_0808a3d8();

void Func_02000a8c(void)
{
    u8 *actor;

    Func_0808a018();
    Func_0808a360();
    Func_0808a370();
    Func_0808a3d8();
    Func_080770c8(0x94f);

    Func_0808a0f0(11, 0x16e00000, 0x049c0000);
    Func_0808a0e0(11, 24, 8);
    Func_0808a0e8(11);
    Func_0808a010(60);

    Func_0808a0f0(12, 0x16e00000, 0x049c0000);
    Func_0808a0e0(12, 12, 24);
    Func_0808a010(30);

    Func_0808a1b8(11, 0x5000, 0);
    Func_0808a1b8(12, 0xd000, 0);
    Func_0808a010(60);
    Func_0808a100(11, 3);
    Func_0808a100(12, 3);
    Func_0808a010(120);

    Func_0808a0f0(8, 0x16f80000, 0x04b80000);
    Func_0808a010(60);
    Func_0808a100(12, 2);

    actor = Func_0808a080(8);
    if (actor != 0) {
        Func_0808a0b8(12, *(s16 *)(actor + 0x0a), *(s16 *)(actor + 0x12));
    }
    Func_0808a0e8(12);
    Func_0808a0f0(12, 0, 0);
    Func_0808a010(60);
    Func_0808a100(11, 2);

    actor = Func_0808a080(8);
    if (actor != 0) {
        Func_0808a0b8(11, *(s16 *)(actor + 0x0a), *(s16 *)(actor + 0x12));
    }
    Func_0808a0e8(11);
    Func_0808a0f0(11, 0, 0);
    Func_0808a010(60);
    Func_0808a100(0, 2);

    actor = Func_0808a080(8);
    if (actor != 0) {
        Func_0808a0b8(0, *(s16 *)(actor + 0x0a), *(s16 *)(actor + 0x12));
    }
    Func_0808a0e8(0);
    Func_0808a0f0(0, 0, 0);
    Func_0808a010(60);

    Func_0808a090(8, 0x8000, 0x4000);
    Func_0808a0e0(8, 56, 8);
    Func_0808a0e8(8);
    Func_0808a0e0(8, 40, 40);
    Func_0808a0e8(8);
    Func_0808a0e0(8, 8, 88);
    Func_0808a0e8(8);
    Func_0808a368();
    Func_0808a248(108);
    Func_0808a020();
}
