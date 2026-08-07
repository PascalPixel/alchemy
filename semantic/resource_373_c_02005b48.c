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
 * UNCERTAINTY 1: the called service addresses are the ones encoded in the
 * overlay image; overlay-local code and resident services share the
 * 0x02000000 namespace and the module's `bl` displacements are fixed up on
 * load, so an encoded address is an identity, not a disassemblable location.
 * UNCERTAINTY 2: r0 is not set before the return, so the routine is spelled
 * `void`; the incoming r0 is consumed by the first call and never reloaded.
 * UNCERTAINTY 3: 0x0200dae1 is an odd (Thumb) address stored into the
 * entity's 0x6c slot, which is how this overlay publishes per-entity
 * behaviour callbacks; its signature is not established here.
 */

struct Resource373Handle {
    u8 unknown_00[5];
    u8 flags05;
    u8 unknown_06[3];
    u8 flags09;
    u8 unknown_0a[0x12];
    u8 paletteIndex;                /* 0x1c */
    u8 unknown_1d[0xa];
    u8 field27;
};

struct Resource373Entity {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    u8 unknown_10[0x13];
    u8 flags23;
    u8 unknown_24[0xc];
    s32 field30;
    u8 unknown_34[4];
    s32 shadowX;                    /* 0x38 */
    s32 shadowY;                    /* 0x3c */
    u8 unknown_40[0x10];
    struct Resource373Handle *handle;   /* 0x50 */
    u8 unknown_54[1];
    u8 field55;
    u8 field56;
    u8 unknown_57[5];
    u8 field5c;
    u8 unknown_5d[4];
    u8 field61;
    u8 unknown_62[0xa];
    void (*behaviour)(void);        /* 0x6c */
};

struct Resource373Entity *Func_0200bb38();
void Func_0200bad2();
s32 Func_0200bb38_b();
u8 *Func_0200ba98();
void Func_0200bb60();
void Func_0200bac8();
void Func_0200babe();

                     

                                          

void Func_02005b48(s32 selector)
{
    struct Resource373Entity *entity = Func_0200bb38(selector);
    struct Resource373Handle *handle = entity->handle;
    u8 *gradient;

    handle->field27 = 0;
    handle->flags05 = (u8)(handle->flags05 & ~0x20);
    handle->flags09 = (u8)(((handle->flags09 & ~0x0c) | 0x04) & 0x0f);

    Func_0200bad2(entity, 0);

    entity->field5c = 0;
    entity->field55 = 0;

    /* 0x109 selects a companion entity; when absent the sprite drops a row. */
    if (Func_0200bb38_b(0x109) == 0) {
        entity->y += 0x00200000;    /* 0x80 << 14 */
    }

    entity->flags23 = (u8)(entity->flags23 & ~1);
    entity->field61 = 1;

    /*
     * Func_08000140 reserves 0x608 (0xc1 << 3) bytes on channel 17 and
     * returns the base of the reservation; the ramp uploaded below starts
     * 0x400 (0x80 << 3) bytes into it. Func_08015250 selects palette 0xb5
     * before the ramp transfer, and Func_08000150 releases the bank.
     */
    gradient = Func_0200ba98(17, 0x608);
    gradient += 0x400;
    Func_0200bb60(0xb5);
    Func_0200bac8(handle->paletteIndex, 0x80, gradient);
    Func_0200babe(17);

    entity->field30 = 0;
    entity->behaviour = (void (*)(void))0x0200dae1;
    entity->shadowX = entity->x;
    entity->shadowY = entity->y;
    entity->field5c = 1;
    entity->field56 = 0;
}
