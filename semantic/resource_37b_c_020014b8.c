#include "types.h"

/*
 * Resource 37b sprite-group setup at 0x020014b8.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020014b8 through the interworking
 * return `pop {r5, r6} / pop {r0} / bx r0` at 0x0200155e.  The popped register
 * is r0, so it holds the return address and the owner is `void`.  The four
 * words at 0x02001564-0x02001573 are this owner's literal pool: the body ends
 * at the `bx r0` and the pool is loaded by the `ldr rN, [pc, #imm]` sites at
 * 0x020014c0/0x020014ca/0x020014e0/0x02001558.  They are data, not code.
 *
 * Link base 0x02008000 is confirmed for this overlay by the pool word
 * 0x02008eb1 at 0x02001570: that is `Func_02000eb0 + 1` (Thumb bit), a real
 * prologue in this image and already byte-exact in
 * assets/code/resource_37b_c_02000eb0.c.  It is installed here as a callback
 * through the veneer that resolves to Func_080000d0.  Consequently the even
 * pool words 0x0200aa50 and 0x0200a56c are in-image DATA at file offsets
 * 0x2a50 and 0x256c, not RAM globals; the overlay image is writable EWRAM, so
 * 0x0200aa50 is written here and is not `const`.
 *
 * Call sites resolved with `tools/overlay_call_targets.ts` (overlay `bl`
 * stores `target_offset - 2`, so the disassembler's annotations are wrong):
 *
 *   0x020014c2 -> veneer 0x020023cc -> Func_08000148   (allocate)
 *   0x020014cc -> veneer 0x020023dc -> Func_080001a0   (load/unpack)
 *   0x020014d0 -> veneer 0x020023ec -> Func_080001d0   (x3, no arguments)
 *   0x020014d8 -> veneer 0x020023e4 -> Func_080001c8   (x3)
 *   0x020014f8 -> veneer 0x020023ec -> Func_080001d0
 *   0x02001502 -> veneer 0x020023e4 -> Func_080001c8
 *   0x02001522 -> veneer 0x020023ec -> Func_080001d0
 *   0x0200152e -> veneer 0x020023e4 -> Func_080001c8
 *   0x02001550 -> veneer 0x020023d4 -> Func_08000150
 *   0x0200155a -> veneer 0x020023c4 -> Func_080000d0   (install callback)
 *
 * 10 sites, 6 distinct imports; that matches the assembly `bl` multiset for
 * this span exactly (the inventory's calls=9 is a lower bound, as documented).
 *
 * Uncertainties recorded as found:
 *  - Func_080001d0 is called with no argument register set at any of its three
 *    sites, and its result is the first argument of the following
 *    Func_080001c8 call.  It is spelled as a no-argument accessor.
 *  - r4 is used as a zero-holding scratch register although the prologue saves
 *    only r5 and r6.  Nothing observable depends on it; this is the known
 *    unsaved-r4 idiom in these overlays and is transcribed, not "fixed".
 */

/* Imports.  Old-style declarations: arity varies per site in this overlay. */
s32 Func_08000148();
void Func_080001a0();
u32 Func_080001d0();
u32 Func_080001c8();
void Func_08000150();
void Func_080000d0();

/* In-image overlay data (writable EWRAM), addressed by its pool value. */
extern u32 Data_0200aa50[];
extern u8 Data_0200a56c[];

/* Installed callback: pool word 0x02008eb1 = Func_02000eb0 + Thumb bit. */
void Func_02000eb0(void);

void Func_020014b8(void)
{
    u32 *entry;
    void *work;
    u32 attribute;
    s32 i;

    entry = Data_0200aa50;

    work = (void *)Func_08000148(14, 0x400);
    Func_080001a0(Data_0200a56c, work);

    /* Three 9-entry runs of 12-byte records, each describing one 0x80-byte
     * slice of the unpacked buffer under a different attribute flag. */
    attribute = Func_080001c8(Func_080001d0(), 0x80, work) | 0xac00;
    for (i = 0; i <= 8; i++) {
        entry[0] = 0;
        entry[1] = 0x40004000;
        entry[2] = attribute;
        entry += 3;
    }

    attribute = Func_080001c8(Func_080001d0(), 0x80, (u8 *)work + 0x80) | 0xdc00;
    for (i = 0; i <= 8; i++) {
        entry[0] = 0;
        entry[1] = 0x40004000;
        entry[2] = attribute;
        entry += 3;
    }

    attribute = Func_080001c8(Func_080001d0(), 0x80, (u8 *)work + 0x100) | 0xc00;
    for (i = 0; i <= 8; i++) {
        entry[0] = 0;
        entry[1] = 0x40004000;
        entry[2] = attribute;
        entry += 3;
    }

    Func_08000150(14);
    Func_080000d0((void *)((u32)&Func_02000eb0 | 1), 0xc80);
}
