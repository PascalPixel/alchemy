#include "types.h"

/*
 * resource_392 owner at 0x02000cb4, 216 bytes: bring one scene entity into
 * its idle presentation state.
 *
 * Complete owner: 'push {r5, r6, r7, lr}' plus the r8/r9/sl saves, through a
 * single epilogue, followed by its two-word literal pool.  One linear path with
 * a single 'if'; nothing is live past the return.
 *
 * TRANSPOSED from semantic/overlays/resource_373_c_02005b48.c.  The two owners
 * are the same routine shared verbatim: over all 108 halfwords they differ in
 * 15 places, 14 of which are the halves of the seven BL pairs and exactly one of
 * which is the low half of the pool word at +0xd4 holding the behaviour
 * callback pointer.
 *
 * What was changed:
 *  - the callback 0x0200dae1 becomes 0x02008c4d.  Under the proven 0x02008000
 *    link base that is file offset 0x0c4c plus the Thumb bit, and that offset
 *    really does open with 'push {r5, r6, r7, lr}' (0xb5e0) - the same opening
 *    as resource_373's 0x5ae0, which is an independent check on both the base
 *    and the pointer's meaning.
 *  - all seven calls were re-resolved with 'bun tools/overlay_call_targets.ts
 *    resource_392 0cb4': 7 sites, 7 distinct veneers, publishing exactly the same
 *    main-image imports in the same order as resource_373.  The 373 source
 *    predates the corrected 'bl' rule and named the imports by their encoded
 *    displacements; worse, it gave the first and third calls the SAME name
 *    (Func_0200bb38) when they are different imports - 0x0808a080 fetches the
 *    scene entity and 0x080770c0 is the 0x109 companion lookup.  Both are
 *    corrected here.
 *
 * UNCERTAINTY (inherited): r0 is not set before the return, so the routine is
 * spelled void; the incoming r0 is consumed by the first call and never
 * reloaded.  Func_08015250's result is discarded, so only its side effect on
 * the shared workspace matters.
 */

struct SceneHandle {
    u8 unknown_00[5];
    u8 flags05;
    u8 unknown_06[3];
    u8 flags09;
    u8 unknown_0a[0x12];
    u8 paletteIndex;                /* 0x1c */
    u8 unknown_1d[0xa];
    u8 field27;
};

struct SceneEntity {
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
    struct SceneHandle *handle;   /* 0x50 */
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

/* Old-style declarations: overlay imports vary in arity between call sites. */
struct SceneEntity *Func_0808a080();     /* scene entity by selector */
struct SceneEntity *Func_080770c0();     /* companion entity by selector, or 0 */
void Func_080091e0();               /* set presentation mode */
s32 Func_08000140();                /* reserve workspace on a channel */
void Func_08000150();               /* commit the channel reservation */
void Func_080001c8();               /* upload a palette ramp */
s32 Func_08015250();                /* shared-workspace side effect */

void Func_02000cb4(s32 selector)
{
    struct SceneEntity *entity = Func_0808a080(selector);
    struct SceneHandle *handle = entity->handle;
    s32 gradient;

    handle->field27 = 0;
    handle->flags05 = (u8)(handle->flags05 & ~0x20);
    handle->flags09 = (u8)(((handle->flags09 & ~0x0c) | 0x04) & 0x0f);

    Func_080091e0(entity, 0);

    entity->field5c = 0;
    entity->field55 = 0;

    /* 0x109 selects a companion entity; when absent the sprite drops a row. */
    if (Func_080770c0(0x109) == 0) {
        entity->y += 0x00200000;    /* 0x80 << 14 */
    }

    entity->flags23 = (u8)(entity->flags23 & ~1);
    entity->field61 = 1;

    /*
     * Func_08000140 reserves 0x608 (0xc1 << 3) bytes on channel 17 and
     * returns the base of the reservation; the ramp uploaded below starts
     * 0x400 (0x80 << 3) bytes into it.  Func_08015250's result is discarded,
     * so only its side effect on the shared workspace matters.
     */
    gradient = Func_08000140(17, 0x608);
    gradient += 0x400;
    Func_08015250(0xb5);
    Func_080001c8(handle->paletteIndex, 0x80, gradient);
    Func_08000150(17);

    entity->field30 = 0;
    entity->behaviour = (void (*)(void))0x02008c4d;
    entity->shadowX = entity->x;
    entity->shadowY = entity->y;
    entity->field5c = 1;
    entity->field56 = 0;
}
