#include "types.h"

/*
 * resource_391 owner at 0x0200034c, 296 bytes through the four literal
 * words ending at 0x02000473. It probes the square ahead of actor 0 against
 * per-slot collision rectangles and returns the first admissible actor.
 *
 * The key and rectangle cursors are initialized once. The ROM advances both
 * through all 58 * 6 records rather than restarting them for each actor
 * slot. The three outputs are direction, matched rectangle index, and the
 * selected slot; only a successful probe writes the latter two.
 */

struct Resource391Kind {
    s32 id;
};

struct Resource391Handle {
    u8 unknown_00[0x28];
    struct Resource391Kind *kind;
};

struct Resource391Object {
    u8 unknown_00[8];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
    u8 unknown_14[0x3c];
    struct Resource391Handle *handle;
};

struct Resource391Anchor {
    u8 unknown_00[10];
    s32 anchorX;
    u8 unknown_0c[6];
    s32 anchorZ;
};

struct Resource391Box {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
};

#define RESOURCE_391_SCENE (*(u8 *volatile *)0x03001ebc)
#define RESOURCE_391_DIRECTION_STEPS ((const s32 *)0x0200ad68)
#define RESOURCE_391_KIND_LIST ((const s32 *)0x0200ada8)
#define RESOURCE_391_KIND_BOXES ((const struct Resource391Box *)0x0200adc0)

struct Resource391Object *Func_0808a080();

struct Resource391Object *Func_0200034c(s32 *directionOut, s32 *slotOut,
                                        s32 *kindIndexOut)
{
    const s32 *kinds = RESOURCE_391_KIND_LIST;
    const struct Resource391Box *box = RESOURCE_391_KIND_BOXES;
    struct Resource391Object **objects =
        (struct Resource391Object **)(RESOURCE_391_SCENE + 0x34);
    struct Resource391Object *player = Func_0808a080(0);
    s32 slot;

    *directionOut = ((const u16 *)player)[3] >> 12;

    for (slot = 8; slot <= 65; slot++, objects++) {
        struct Resource391Object *object = *objects;
        struct Resource391Anchor *anchor = (struct Resource391Anchor *)object;
        s32 kind = object->handle->kind->id;
        s32 index;

        for (index = 0; index <= 5; index++, box++) {
            s32 step;
            s32 playerX;
            s32 playerZ;
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
            step = RESOURCE_391_DIRECTION_STEPS[*directionOut];
            playerX = player->x;
            probeX = ((playerX >> 16) + (step >> 16)) >> 4;
            playerZ = player->z;
            probeZ = ((playerZ >> 16) + (s16)step) >> 4;
            boxX0 = (anchor->anchorX + box->x0) >> 4;
            boxZ0 = (anchor->anchorZ + box->z0) >> 4;
            boxX1 = (anchor->anchorX + box->x1) >> 4;
            boxZ1 = (anchor->anchorZ + box->z1) >> 4;

            if (boxX0 > probeX || probeX >= boxX1 ||
                boxZ0 > probeZ || probeZ >= boxZ1) {
                continue;
            }

            if ((index & 1) != 0) {
                if (boxX0 == (playerX >> 20)) {
                    continue;
                }
            } else if (boxZ0 == (playerZ >> 20)) {
                continue;
            }

            *slotOut = slot;
            return object;
        }
    }

    return 0;
}
