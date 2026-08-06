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
















       /* ROM dispatch[12] -> Func_0800231c */
       /* ROM dispatch[11] -> Func_08002322 */
/* Four register arguments plus four stack words. */







extern u8 * Func_0200508a();
extern void Func_02005078();
extern void Func_02005100();
extern void Func_0200509e();
extern u8 * Func_02005044();
extern void Func_020050ea();
extern void Func_0200515e();
extern void Func_02005178();
extern void Func_02005184();
extern void Func_020050ba();
extern void Func_0200509a();
extern void Func_0200314c();
extern void Func_02005140();
extern void Func_020050b6();
extern void Func_020050c2();
extern void Func_0200518e();
extern void Func_020050fc();
extern void Func_020050dc();
extern void Func_0200318e();
extern void Func_020050f0();
extern void Func_0200512a();
extern void Func_020031ac();
extern void Func_020051c8();
extern void Func_02003144();
extern void Func_02005156();
extern void Func_02005136();
extern s32 Func_020050de();
extern s32 Func_020050e2();
extern void Func_02002efa();
extern void Func_020051b8();
extern void Func_02005196();
extern void Func_020051a2();
extern void Func_020052e8();
extern void Func_02005248();
extern void Func_020051de();
extern void Func_02005204();
extern u8 * Func_0200520e();
extern u8 * Func_020031f4();
extern void Func_02005206();
extern void Func_02005308();
extern void Func_02005300();
extern void Func_02005234();
void Func_020022c0(void)
{
    u8 *slot;
    s32 permuted_9;
    s32 shape[3];          /* sp+16, the burst's per-step parameters */
    u32 step;

    slot = Func_0200508a(18);

    /* r0 still holds the record returned above. */
    Func_02005078();

    Func_02005100(18, 15);

    /* r0 is not reloaded between these two branches. */
    Func_0200509e(Func_02005044(18), 0);

    Func_020050ea(18, 136 << 16, 360 << 16);        /* 180 << 17 */
    Func_0200515e(0x8000, 0x1000);                  /* 128 << 8, 128 << 5 */
    Func_02005178(136 << 16, -1, 196 << 17, 1);

    /* No argument registers are set. */
    Func_02005184();

    Func_020050ba(60);

    /* First shake. */
    Func_0200509a(0x50000, 0x50000, 0x10000);       /* 160 << 11 twice, 128 << 9 */
    Func_0200314c(18);
    Func_02005140(0, 2);
    Func_020050b6(-1, -1, 0xe666);
    Func_020050c2();

    Func_0200518e(0, 0xc000, 20);                   /* 192 << 8 */
    Func_020050fc(40);

    /* Second shake. */
    Func_020050dc(0x50000, 0x50000, 0x10000);
    Func_0200318e(18);
    Func_020050f0(-1, -1, 0xe666);
    Func_020050fc();

    Func_0200512a(40);

    /* Third shake, with no camera framing of its own. */
    Func_020031ac(18);

    *(s32 *)(slot + 24) = 0x13333;
    *(s32 *)(slot + 28) = 0x13333;

    Func_020051c8(18, 5);
    Func_02003144(18, 136, 392, 0xf0000);           /* 196 << 1, 240 << 12 */
    Func_02005156(15);
    Func_02005136(0x50000, 0x50000, 0x10000);

    for (step = 0; step <= 16; step++) {
        s32 key = step << 12;
        s32 spread;

        shape[0] = Func_020050de(key);
        shape[1] = 0;
        shape[2] = Func_020050e2(key);

        /* shape[0] + shape[0] / 2: the halving is the usual signed
         * round-toward-zero sequence (`lsrs #31 / adds / asrs #1`). */
        shape[0] = spread + spread / 2;
        spread  = permuted_9;
        permuted_9 = shape[0];

        Func_02002efa(*(s32 *)(slot + 8), 0, *(s32 *)(slot + 16),
                      shape[0], shape[1], shape[2], 1, 0);
    }

    Func_020051b8(30);
    Func_02005196(-1, -1, 0xe666);
    Func_020051a2();

    Func_020052e8(148);
    Func_02005248(18, 2);
    Func_020051de(20);

    /* The party's own tile centres, each read after its own fetch of the
     * record; the Z reading is pulled back by 16 units. */
    Func_02005204(18,
                  *(s16 *)(Func_0200520e(0) + 10),
                  *(s16 *)(Func_020031f4(0) + 18) - 16,
                  0x80000);                         /* 128 << 12 */

    Func_02005206(10);

    ((u8 *)Data_02000240)[0x22b] = 3;

    Func_02005308(70, 15);
    Func_02005300(53, 1);

    /* Common exit; no argument registers are set. */
    Func_02005234();
}
