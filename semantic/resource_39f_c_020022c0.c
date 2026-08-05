#include "types.h"

/*
 * resource_39f owner at 0x020022c0, 492 bytes: code 0x020022c0-0x02002497, an
 * alignment halfword at 0x0200249a and the five pool words 0x0000e666,
 * 0x00013333, 0x02000240, 0x0000022b and 0x00000046 at
 * 0x02002498-0x020024ab.
 *
 * The overlay's climax beat for slot 18.  It stages the camera, plays three
 * timed shakes against Func_02000e18, then emits a seventeen-step burst of
 * effects through Func_02000ae8 before restoring the camera and handing back to
 * the party.
 *
 * All 42 assembly `bl` sites are placed, which matches the inventory's call
 * count for the row exactly.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * Func_08000120 and Func_08000118 are ROM dispatch stubs 12 and 11 from
 * asm/080000c0.s, forwarding to Func_0800231c and Func_08002322 - the same pair
 * the burst loop in 0x02000e18 uses.
 *
 * Data_02000240 is an EWRAM global, below this module's 0x02008000 link base
 * and therefore not in-image; the byte-exact sources in assets/code use the
 * same symbol.
 *
 * The epilogue is `add sp, #28 / pop {r3, r5} / ... / pop {r0} / bx r0`, so the
 * owner is void.
 */

extern s16 Data_02000240[];

u8 *Func_0808a080();
void Func_0808a018();
void Func_0808a158();
void Func_080091e0();
void Func_0808a0f0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a010();
void Func_080091f0();
void Func_02000e18();
void Func_0808a130();
void Func_080091f8();
void Func_0808a1b8();
void Func_02000d90();
s32 Func_08000120();       /* ROM dispatch[12] -> Func_0800231c */
s32 Func_08000118();       /* ROM dispatch[11] -> Func_08002322 */
/* Four register arguments plus four stack words. */
void Func_02000ae8();
void Func_080f9010();
void Func_0808a138();
void Func_0808a260();
void Func_0808a250();
void Func_0808a020();

void Func_020022c0(void)
{
    u8 *slot;
    s32 shape[3];          /* sp+16, the burst's per-step parameters */
    u32 step;

    slot = Func_0808a080(18);

    /* r0 still holds the record returned above. */
    Func_0808a018();

    Func_0808a158(18, 15);

    /* r0 is not reloaded between these two branches. */
    Func_080091e0(Func_0808a080(18), 0);

    Func_0808a0f0(18, 136 << 16, 360 << 16);        /* 180 << 17 */
    Func_0808a208(0x8000, 0x1000);                  /* 128 << 8, 128 << 5 */
    Func_0808a210(136 << 16, -1, 196 << 17, 1);

    /* No argument registers are set. */
    Func_0808a218();

    Func_0808a010(60);

    /* First shake. */
    Func_080091f0(0x50000, 0x50000, 0x10000);       /* 160 << 11 twice, 128 << 9 */
    Func_02000e18(18);
    Func_0808a130(0, 2);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();

    Func_0808a1b8(0, 0xc000, 20);                   /* 192 << 8 */
    Func_0808a010(40);

    /* Second shake. */
    Func_080091f0(0x50000, 0x50000, 0x10000);
    Func_02000e18(18);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();

    Func_0808a010(40);

    /* Third shake, with no camera framing of its own. */
    Func_02000e18(18);

    *(s32 *)(slot + 24) = 0x13333;
    *(s32 *)(slot + 28) = 0x13333;

    Func_0808a158(18, 5);
    Func_02000d90(18, 136, 392, 0xf0000);           /* 196 << 1, 240 << 12 */
    Func_0808a010(15);
    Func_080091f0(0x50000, 0x50000, 0x10000);

    for (step = 0; step <= 16; step++) {
        s32 key = step << 12;
        s32 spread;

        shape[0] = Func_08000120(key);
        shape[1] = 0;
        shape[2] = Func_08000118(key);

        /* shape[0] + shape[0] / 2: the halving is the usual signed
         * round-toward-zero sequence (`lsrs #31 / adds / asrs #1`). */
        spread = shape[0];
        shape[0] = spread + spread / 2;

        Func_02000ae8(*(s32 *)(slot + 8), 0, *(s32 *)(slot + 16),
                      shape[0], shape[1], shape[2], 1, 0);
    }

    Func_0808a010(30);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();

    Func_080f9010(148);
    Func_0808a138(18, 2);
    Func_0808a010(20);

    /* The party's own tile centres, each read after its own fetch of the
     * record; the Z reading is pulled back by 16 units. */
    Func_02000d90(18,
                  *(s16 *)(Func_0808a080(0) + 10),
                  *(s16 *)(Func_0808a080(0) + 18) - 16,
                  0x80000);                         /* 128 << 12 */

    Func_0808a010(10);

    ((u8 *)Data_02000240)[0x22b] = 3;

    Func_0808a260(70, 15);
    Func_0808a250(53, 1);

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
