#include "types.h"

/*
 * resource_39f owner at 0x02001d04, 220 bytes: code 0x02001d04-0x02001dcd, an
 * alignment halfword at 0x02001dce and the four pool words 0x00000103,
 * 0x00000307, 0x02000240 and 0x0000022b at 0x02001dd0-0x02001ddf.
 *
 * The slot-15 twin of 0x02001244: open the slot, place it at (472, 104),
 * publish an eight-argument piece through Func_02000ae8, run the presentation,
 * re-place the slot on the party's heading readings, and set the engine byte at
 * Data_02000240 + 0x22b to 3.  It differs from the slot-11 version by an extra
 * Func_0808a010(10) after the placement, a different Z bias, a different
 * notification tag, and by omitting the trailing Func_0808a0f0 call.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule,
 * and resource_39f_c_02001244.c for the Data_02000240 note.
 *
 * The epilogue is `add sp, #16 / pop {r5, r6} / pop {r0} / bx r0`, so the owner
 * is void.
 *
 * Uncertainty: r4 is written without being saved, although the prologue is only
 * `push {r5, r6, lr}`.  It carries the constant 0 into three stack argument
 * slots; nothing observable depends on the register choice.
 */

extern s16 Data_02000240[];





/* Four register arguments plus four stack words. */











extern u8 * Func_02004ac8();
extern void Func_02004ab8();
extern void Func_02002bc4();
extern void Func_02002abc();
extern void Func_02004ace();
extern void Func_02002834();
extern void Func_02004ba8();
extern void Func_02004b7a();
extern void Func_02004b00();
extern void Func_02004b70();
extern void Func_02004bba();
extern void Func_02004c30();
extern void Func_02004b1e();
extern void Func_02004b44();
extern u8 * Func_02004b4e();
extern u8 * Func_02002b32();
extern void Func_02004b42();
extern void Func_02004c3c();
void Func_02001d04(void)
{
    u8 *slot;

    slot = Func_02004ac8(15);

    /* r0 still holds the record returned above. */
    Func_02004ab8();

    Func_02002bc4(15, 0);
    Func_02002abc(15, 472, 104, 0x80000);   /* 236 << 1, 128 << 12 */
    Func_02004ace(10);

    Func_02002834(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x80000,
                  0, 0, 0, 1, 0);

    Func_02004ba8(15, 1);
    Func_02004b7a(15, 0, 0);
    Func_02004b00(30);
    Func_02004b70(15, 2);
    Func_02004bba(15, 0x103, 0);
    Func_02004c30(147);
    Func_02004b1e(60);

    Func_02004b44(15,
                  *(s16 *)(Func_02004b4e(0) + 10),
                  *(s16 *)(Func_02002b32(0) + 18),
                  0x60000);                          /* 192 << 11 */

    Func_02004b44(10);
    Func_02004b42(0x307);

    ((u8 *)Data_02000240)[0x22b] = 3;

    Func_02004c3c(53, 0);

    /* Common exit; no argument registers are set. */
    Func_02004b70();
}
