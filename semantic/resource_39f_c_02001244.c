#include "types.h"

/*
 * resource_39f owner at 0x02001244, 228 bytes: code 0x02001244-0x02001315, an
 * alignment halfword at 0x02001316 and the four pool words 0x00000103,
 * 0x00000301, 0x02000240 and 0x0000022b at 0x02001318-0x02001327.
 *
 * A full cutscene beat for slot 11: open the slot, place it at (408, 456),
 * publish an eight-argument piece through the overlay's Func_02000ae8, run the
 * presentation, then re-place the slot on the party's current heading readings
 * and set the engine byte at Data_02000240 + 0x22b to 3.
 *
 * The twin at 0x02001d04 is the same beat for slot 15.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 *
 * Data_02000240 is a genuine EWRAM global, not overlay data: this module is
 * linked at 0x02008000, so an address below that base cannot be in-image.  The
 * byte-exact sources in assets/code use the same symbol.
 *
 * The epilogue is `add sp, #16 / pop {r5} / pop {r0} / bx r0`, so the owner is
 * void.
 *
 * Uncertainty: r4 is written without being saved, although the prologue is only
 * `push {r5, lr}`.  It carries the constant 0 into three stack argument slots;
 * nothing observable depends on the register choice.
 */

extern s16 Data_02000240[];

/* Returns a slot record; +8/+12/+16 are its position and +10/+18 are the
 * signed halfwords read from slot 0. */




/* Four register arguments plus four stack words. */












extern u8 * Func_02004008();
extern void Func_02003ff6();
extern void Func_02002102();
extern void Func_02001ffc();
extern void Func_02001d72();
extern void Func_020040e6();
extern void Func_020040b8();
extern void Func_0200403e();
extern void Func_020040ae();
extern void Func_020040f8();
extern void Func_0200416e();
extern void Func_0200405c();
extern void Func_02004082();
extern u8 * Func_0200408c();
extern u8 * Func_02002070();
extern void Func_02004080();
extern void Func_020040ea();
extern void Func_02004184();
void Func_02001244(void)
{
    u8 *slot;

    slot = Func_02004008(11);

    /* r0 still holds the record returned above. */
    Func_02003ff6();

    Func_02002102(11, 0);
    Func_02001ffc(11, 408, 456, 0x60000);   /* 204 << 1, 228 << 1, 192 << 11 */

    Func_02001d72(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Func_020040e6(11, 1);
    Func_020040b8(11, 0, 0);
    Func_0200403e(30);
    Func_020040ae(11, 2);
    Func_020040f8(11, 0x103, 0);
    Func_0200416e(147);
    Func_0200405c(60);

    /* Two signed halfwords of slot 0, each read after its own fetch of the
     * record. */
    Func_02004082(11,
                  *(s16 *)(Func_0200408c(0) + 10),
                  *(s16 *)(Func_02002070(0) + 18),
                  0x40000);                          /* 128 << 11 */

    Func_02004082(10);
    Func_02004080(0x301);
    Func_020040ea(14, 0, 0);

    ((u8 *)Data_02000240)[0x22b] = 3;

    Func_02004184(53, 0);

    /* Common exit; no argument registers are set. */
    Func_020040b8();
}
