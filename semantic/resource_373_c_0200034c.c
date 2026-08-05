#include "types.h"

/*
 * Resource 373: find the scene object standing in the square the player is
 * about to enter.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl/fp saves and the
 * 24-byte local frame at 0x0200034c, through the single `add sp, #24`
 * epilogue at 0x02000452..0x02000462, followed by its four-word literal pool.
 * The two `return` sites inside the search jump to that epilogue with the
 * answer already in r0, so the whole owner is one region.
 *
 * The one call is placed.  It is not an interworking `call_via rN` site.
 *
 * Search shape: 58 object slots (numbered 8..65) are walked, and each slot's
 * kind identifier is matched against the six-entry list at 0x0200e1d0.  A
 * matching kind selects the same-indexed 16-byte box from the table at
 * 0x0200e1e8, and the object is returned when the probe point lies strictly
 * inside that box and is not already on the object's own row or column.
 * Even-indexed kinds gate on the Z coordinate and odd-indexed kinds on X.
 *
 * UNCERTAINTY 1: the called service address is the one encoded in the
 * overlay image (shared 0x02000000 namespace, load-time fixups).
 * UNCERTAINTY 2: the object array is reached as `scene + 0x34` while the
 * counter written back through `slotOut` starts at 8, so the two are offset
 * from each other; the reference does exactly that and the relationship is
 * reproduced rather than normalised.
 */

struct Resource373Kind {
    s16 id;
};

struct Resource373Handle {
    u8 unknown_00[0x28];
    struct Resource373Kind *kind;
};

struct Resource373Object {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    u8 unknown_0c[4];
    s32 z;                          /* 0x10 */
    u8 unknown_14[0x3c];
    struct Resource373Handle *handle;   /* 0x50 */
};

/* Objects consulted through the scene block also expose two s16 anchors. */
struct Resource373Anchor {
    u8 unknown_00[10];
    s16 anchorX;                    /* 0x0a */
    u8 unknown_0c[6];
    s16 anchorZ;                    /* 0x12 */
};

struct Resource373Box {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
};

/* Scene block; the object pointer array begins at +0x34. */
#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001ebc)

#define RESOURCE_373_DIRECTION_STEPS ((const s32 *)0x0200e190)
#define RESOURCE_373_KIND_LIST ((const s32 *)0x0200e1d0)
#define RESOURCE_373_KIND_BOXES ((const struct Resource373Box *)0x0200e1e8)

struct Resource373Object *Func_0200634e();

struct Resource373Object *Func_0200034c(s32 *directionOut, s32 *slotOut,
                                        s32 *kindIndexOut)
{
    u8 *scene = RESOURCE_373_SCENE;
    struct Resource373Object *player = Func_0200634e(0);
    struct Resource373Object **objects =
        (struct Resource373Object **)(scene + 0x34);
    s32 slot;

    *directionOut = ((const u16 *)player)[3] >> 12;   /* halfword at +6 */

    for (slot = 8; (u32)slot <= 65; slot++, objects++) {
        struct Resource373Object *object = *objects;
        struct Resource373Anchor *anchor = (struct Resource373Anchor *)object;
        s32 kind = object->handle->kind->id;
        const s32 *kinds = RESOURCE_373_KIND_LIST;
        s32 index;

        for (index = 0; (u32)index <= 5; index++) {
            const struct Resource373Box *box = &RESOURCE_373_KIND_BOXES[index];
            s32 step;
            s32 probeX;
            s32 probeZ;
            s32 boxX0;
            s32 boxZ0;
            s32 boxX1;
            s32 boxZ1;

            if (kind != *kinds++) {
                continue;
            }

            *kindIndexOut = index;

            /*
             * The packed direction word carries the X step in its high half
             * and the Z step in its low half, both already 16.16 fixed; the
             * probe point is the player's position plus that step, reduced
             * to whole squares by an arithmetic shift of 4.
             */
            step = RESOURCE_373_DIRECTION_STEPS[*directionOut];
            probeX = ((player->x >> 16) + (step >> 16)) >> 4;
            probeZ = ((player->z >> 16) + ((step << 16) >> 16)) >> 4;

            boxX0 = (anchor->anchorX + box->x0) >> 4;
            boxX1 = (anchor->anchorX + box->x1) >> 4;
            boxZ1 = (anchor->anchorZ + box->z1) >> 4;
            boxZ0 = (anchor->anchorZ + box->z0) >> 4;

            if (boxX0 > probeX || probeX >= boxX1) {
                continue;
            }
            if (boxZ0 > probeZ || probeZ >= boxZ1) {
                continue;
            }

            if ((index & 1) != 0) {
                if (boxX0 == (player->x >> 20)) {
                    continue;
                }
            } else {
                if (boxZ0 == (player->z >> 20)) {
                    continue;
                }
            }

            *slotOut = slot;
            return object;
        }
    }

    return 0;
}
