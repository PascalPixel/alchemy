#include "types.h"

/*
 * Resource 371 owner at 0x020001c4 (140 bytes, 4 calls).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020001c4 and the interworking
 * return `pop {r5, r6} ; pop {r0} ; bx r0` at 0x0200023c.  r0 holds the
 * popped return address, so the owner returns nothing.  Bytes
 * 0x02000242-0x0200024f are alignment plus the literal pool (0x03001e40,
 * 0x15d00000, 0x03000118).
 *
 * This owner is the per-frame step callback that the byte-exact sibling
 * `assets/code/resource_371_c_02000250.c` installs into an object's +0x6c as
 * 0x020081c5 — that pool word is this address plus the overlay's 0x02008000
 * link base plus the Thumb bit.  Its single argument is therefore the object.
 *
 * `mov ip, pc ; bx r3` at 0x02000208 is an ordinary indirect call, not a
 * jump: `mov ip,pc` sets the return address to the instruction after the
 * `bx`, and r3 was loaded from the pool with 0x03000118 — an ARM-mode helper
 * relocated into IWRAM, the same family as the 0x030001d8 square root that
 * the main image reaches through `Func_080072f0`.  It is called with the
 * value returned by 0x020043a4 and the constant 0x40000, and its result is
 * biased by 0x100000 and stored as the object's +0x0c.  LAWS.md already
 * records this spelling as a codegen-only difference from a `call_via r3`
 * thunk, so r3 is the callee and never scratch.
 *
 * UNCERTAINTIES: 0x020043a4 takes the frame counter times 8 and 0x03000118
 * takes its result with 0x40000, which is consistent with an angle-to-16.16
 * curve followed by a fixed-point multiply, but neither is resolved here.
 * The original also clobbers r4 without saving it (`push {r5, r6, lr}`
 * only) — the same call-clobbered-r4 convention seen at 0x0200011c and
 * 0x02002768.
 */

typedef s32 (*RelocatedHelper_020001c4)(s32, s32);

extern u32 Data_03001e40;





extern void Func_02004434();
extern void Func_0200443e();
extern s32 Func_020043a4();
extern void Func_020043d2();
void Func_020001c4(u8 *object)
{
    u16 *frame = (u16 *)(object + 0x64);
    s32 curve;
    s32 offset;

    if ((Data_03001e40 & 2) != 0) {
        Func_02004434(object, 10);
    } else {
        Func_0200443e(object, 7);
    }

    if (*(s16 *)(object + 0x66) != 0) {
        return;
    }

    *(s32 *)(object + 0x08) = 0x15d00000;

    curve = Func_020043a4((s32)*(s16 *)frame << 3);
    offset = ((RelocatedHelper_020001c4)0x03000118)(curve, 0x40000);
    *(s32 *)(object + 0x0c) = offset + 0x100000;
    *(s32 *)(object + 0x10) = 0x5300000;

    Func_020043d2(0x100000, (s32)*(s16 *)frame, object + 0x08);

    *(u16 *)(object + 0x06) = (u16)(*frame + 0x4000);
    *frame = (u16)(*frame + 0x400);
}
