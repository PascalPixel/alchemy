#include "types.h"

/*
 * resource_39f owner at 0x02002c0c, 216 bytes: code 0x02002c0c-0x02002cdb and
 * the pool words 0x00000109 at 0x02002cdc and 0x0200aba5 at 0x02002ce0.
 *
 * Arms one actor for a scripted ride: it normalises two mode bytes of the
 * actor's presentation block, clears three state bytes, optionally lifts the
 * actor by two units when Func_080770c0 reports 0 for tag 265, requests a
 * 1544-byte working area, uploads 128 bytes into it, snapshots the actor's
 * position into its ride fields, and finally installs Func_02002ba4 as the
 * actor's per-frame handler.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * The handler pool word 0x0200aba5 is the third independent witness for the
 * 0x02008000 link base in this overlay: 0x0200aba5 - 0x8000 - 1 = 0x02002ba4,
 * and offset 0x2ba4 really is a `push {r5, r6, r7, lr}` prologue.  It is an
 * in-image function pointer with the Thumb bit set, not a RAM address.
 *
 * Three imports here reach the ROM dispatch stub table (asm/080000c0.s):
 *   veneer 0x02002d0c -> 0x08000140 = dispatch[16] -> Func_080048b0, the
 *     allocator, `void *Func_080048b0(s32 resource, u32 size)`;
 *   veneer 0x02002d1c -> 0x080001c8 = dispatch[33] -> Func_08003fa4;
 *   veneer 0x02002d14 -> 0x08000150 = dispatch[18] -> Func_08002dd8, the
 *     matching release.
 *
 * Uncertainty: Func_08015250's return value is discarded - r0 is reloaded from
 * the presentation block before the next call - so it is called for effect.
 *
 * The epilogue is `pop {r3, r5, r6} / ... / pop {r0} / bx r0`, so the owner is
 * void.
 */

/* Returns the actor record.  +80 holds its presentation block. */
u8 *Func_020059d4();
void Func_020059a6();
s32 Func_020059ec();
u8 *Func_0200599c();
s32 Func_02005a1c();
void Func_020059c4();
void Func_020059c2();

/* Returns 0 when the tagged condition does not hold. */
                    
/* ROM dispatch[16] -> Func_080048b0, the allocator. */

/* ROM dispatch[33] -> Func_08003fa4. */
                     
/* ROM dispatch[18] -> Func_08002dd8, the matching release. */

/* The per-frame handler installed at the end; its prologue is at that offset
 * and the pool word carries its Thumb bit. */
extern void Func_02002ba4();

void Func_02002c0c(s32 subject)
{
    u8 *actor;
    u8 *block;
    u8 *flagAt92;
    u8 flags;
    s32 zero;
    s32 one;
    s32 area;
    s32 mode;
    s32 lock;

    /* r0-r3 are untouched by the prologue, so the incoming arguments reach the
     * import unchanged. */
    actor = Func_020059d4(subject);
    block = *(u8 **)(actor + 80);

    mode = ~12;
    mode &= block[9];
    mode |= 4;
    lock = block[5];
    lock &= ~32;
    block[5] = (u8)lock;
    mode &= 15;
    block[9] = (u8)mode;

    zero = 0;
    block[39] = (u8)zero;

    Func_020059a6(actor, zero);

    flagAt92 = actor + 92;
    *flagAt92 = (u8)zero;
    actor[85] = (u8)zero;

    if (Func_020059ec(0x109) == 0) {
        *(s32 *)(actor + 12) += 0x200000;   /* 128 << 14, two units up */
    }

    flags = (u8)(actor[35] & ~1);
    one = 1;
    actor[35] = flags;
    actor[97] = (u8)one;

    area = (s32)Func_0200599c(17, 0x608);
    Func_02005a1c(0xb5);
    area += 0x400;
    Func_020059c4(block[28], 0x80, area);
    Func_020059c2(17);

    *(s32 *)(actor + 56) = *(s32 *)(actor + 8);
    *(s32 *)(actor + 48) = zero;
    *(s32 *)(actor + 60) = *(s32 *)(actor + 12);
    *flagAt92 = (u8)one;
    *(u32 *)(actor + 108) = 0x0200aba5;
    actor[86] = (u8)zero;
}
