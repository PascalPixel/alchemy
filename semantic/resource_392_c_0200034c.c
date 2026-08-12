#include "types.h"

/*
 * resource_392 owner at 0x0200034c, through its literal pool at 0x02000473.
 *
 * This is the staged-actor probe used by the adjacent movement owner.  The
 * type and box cursors are deliberately outside the slot loop: the original
 * walks both cursors continuously while it tests the 58 actor slots.  Keeping
 * that unusual traversal is important both for the observed state machine and
 * for the compiler's register lifetime.
 */

struct Resource392Kind {
    s16 id;
};

struct Resource392Handle {
    u8 unknown_00[0x28];
    struct Resource392Kind *kind;
};

struct Resource392Object {
    u8 unknown_00[8];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
    u8 unknown_14[0x3c];
    struct Resource392Handle *handle;
};

struct Resource392Anchor {
    u8 unknown_00[10];
    s16 anchorX;
    u8 unknown_0c[6];
    s16 anchorZ;
};

struct Resource392Box {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
};

#define RESOURCE_392_SCENE (*(u8 *volatile *)0x03001ebc)
#define RESOURCE_392_DIRECTION_STEPS ((const s32 *)0x02008ec8)
#define RESOURCE_392_KIND_LIST ((const s32 *)0x02008f08)
#define RESOURCE_392_KIND_BOXES ((const struct Resource392Box *)0x02008f20)

struct Resource392Object *Func_0808a080();

struct Resource392Object *Func_0200034c(s32 *directionOut, s32 *slotOut,
                                         s32 *kindIndexOut)
{
    u8 *scene = RESOURCE_392_SCENE;
    struct Resource392Object *player = Func_0808a080(0);
    struct Resource392Object **objects;
    s32 slot;

    *directionOut = ((const u16 *)player)[3] >> 12;
    objects = (struct Resource392Object **)(scene + 0x34);
    for (slot = 8; (u32)slot <= 65; slot++, objects++) {
        struct Resource392Object *object = *objects;
        struct Resource392Anchor *anchor = (struct Resource392Anchor *)object;
        s16 kind;
        const s32 *kinds;
        const struct Resource392Box *box;
        s32 index;

        index = 0;
        kind = object->handle->kind->id;
        box = RESOURCE_392_KIND_BOXES;
        kinds = RESOURCE_392_KIND_LIST;

        for (; (u32)index <= 5; index++, box++) {
            s32 step;
            s32 probeX;
            s32 probeZ;
            s32 boxX0;
            s32 boxZ0;
            s32 boxX1;
            s32 boxZ1;
            s32 playerX;
            s32 playerZ;

            if (kind != *kinds++)
                continue;

            *kindIndexOut = index;
            playerX = player->x;
            playerZ = player->z;
            step = RESOURCE_392_DIRECTION_STEPS[*directionOut];
            probeX = ((playerX >> 16) + (step >> 16)) >> 4;
            probeZ = ((playerZ >> 16) + ((step << 16) >> 16)) >> 4;

            boxX0 = (anchor->anchorX + box->x0) >> 4;
            boxZ0 = (anchor->anchorZ + box->z0) >> 4;
            boxX1 = (anchor->anchorX + box->x1) >> 4;
            boxZ1 = (anchor->anchorZ + box->z1) >> 4;

            if (boxX0 > probeX || probeX >= boxX1)
                continue;
            if (boxZ0 > probeZ || probeZ >= boxZ1)
                continue;

            if ((index & 1) != 0) {
                if (boxX0 == (playerX >> 20))
                    continue;
            } else {
                if (boxZ0 == (playerZ >> 20))
                    continue;
            }

            *slotOut = slot;
            return object;
        }
    }

    return 0;
}
