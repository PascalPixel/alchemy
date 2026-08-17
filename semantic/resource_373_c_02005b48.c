#include "types.h"

/*
 * Resource 373: bring one scene entity into its idle presentation state.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl saves at
 * 0x02005b48 through the single epilogue at 0x02005c0a..0x02005c16, followed
 * by its two-word literal pool at 0x02005c18.  There is one linear path with
 * a single `if`; nothing is live past the return.
 *
 * Seven call sites over six distinct targets are placed below.  None is an
 * interworking `call_via rN` site: r3 never receives a helper address.
 *
 * TWIN.  The same 216-byte body appears in resource_38e:0974,
 * resource_392:0cb4, resource_39f:2c0c, resource_3a8:3a3c and
 * resource_3b2:2ed8.  The closest existing source was resource_392:0cb4;
 * preserving that source's pointer-and-offset shape while substituting this
 * overlay's symbols and callback produced the exact register allocation.
 *
 * UNCERTAINTY 1: the called service addresses are the ones encoded in the
 * overlay image; overlay-local code and resident services share the
 * 0x02000000 namespace and the module's `bl` displacements are fixed up on
 * load, so an encoded address is an identity, not a disassemblable location.
 * UNCERTAINTY 2: the incoming r0 is passed unchanged to the first call and is
 * named `selector` here; no later instruction reads it.
 * UNCERTAINTY 3: 0x0200dae1 is an odd (Thumb) address stored into the
 * entity's 0x6c slot, which is how this overlay publishes per-entity
 * behaviour callbacks; its signature is not established here.
 */

extern u8 *Func_0200bb38();
void Func_0200bad2();
s32 Func_0200bb38_b();
s32 Func_0200ba98();
void Func_0200bb60();
void Func_0200bac8();
void Func_0200babe();

void Func_02005b48(s32 selector)
{
    u8 *entity;
    u8 *handle;
    u8 *flagAt92;
    u8 flags;
    s32 zero;
    s32 one;
    s32 gradient;
    s32 mode;
    s32 lock;

    entity = Func_0200bb38(selector);
    handle = *(u8 **)(entity + 80);

    /* Keep both masks at signed word width: narrowing earlier makes gcc fold
     * the first mask to one 0xf3 immediate.  The reference begins the +9
     * chain first but writes +5 first. */
    mode = ~12;
    mode &= handle[9];
    mode |= 4;
    lock = handle[5];
    lock &= ~32;
    handle[5] = (u8)lock;
    mode &= 15;
    handle[9] = (u8)mode;

    zero = 0;
    handle[39] = (u8)zero;
    Func_0200bad2(entity, zero);

    flagAt92 = entity + 92;
    *flagAt92 = (u8)zero;
    entity[85] = (u8)zero;

    /* 0x109 selects a companion entity; when absent the sprite drops a row. */
    if (Func_0200bb38_b(0x109) == 0) {
        *(s32 *)(entity + 12) += 0x00200000;    /* 0x80 << 14 */
    }

    flags = (u8)(entity[35] & ~1);
    one = 1;
    entity[35] = flags;
    entity[97] = (u8)one;

    /*
     * Func_08000140 reserves 0x608 (0xc1 << 3) bytes on channel 17 and
     * returns the base of the reservation; the ramp uploaded below starts
     * 0x400 (0x80 << 3) bytes into it. Func_08015250 selects palette 0xb5
     * before the ramp transfer, and Func_08000150 releases the bank.
     */
    gradient = Func_0200ba98(17, 0x608);
    Func_0200bb60(0xb5);
    gradient += 0x400;
    Func_0200bac8(handle[28], 0x80, gradient);
    Func_0200babe(17);

    *(s32 *)(entity + 56) = *(s32 *)(entity + 8);
    *(s32 *)(entity + 48) = zero;
    *(s32 *)(entity + 60) = *(s32 *)(entity + 12);
    *flagAt92 = (u8)one;
    *(u32 *)(entity + 108) = 0x0200dae1;
    entity[86] = (u8)zero;
}
