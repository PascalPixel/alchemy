#include "types.h"

/*
 * resource_393 owner at 0x0200034c, through its literal pool at 0x02000473.
 *
 * This is the staged-actor probe used by the adjacent footprint owner.  The
 * type and rectangle cursors are initialized once per actor, outside the
 * inner six-rectangle loop, then advanced together.  The same compiler-input
 * shape is witnessed by the resource_392 sibling; only the overlay-local
 * tables and accessor veneer differ here.
 */

struct Resource393Kind {
    s16 id;
};

struct Resource393Handle {
    u8 unknown_00[0x28];
    struct Resource393Kind *kind;
};

struct Resource393Object {
    u8 unknown_00[8];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
    u8 unknown_14[0x3c];
    struct Resource393Handle *handle;
};

struct Resource393Anchor {
    u8 unknown_00[10];
    s16 anchorX;
    u8 unknown_0c[6];
    s16 anchorZ;
};

struct Resource393Box {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
};

#define RESOURCE_393_SCENE (*(u8 *volatile *)0x03001ebc)
#define RESOURCE_393_DIRECTION_STEPS ((const s32 *)0x02008f10)
#define RESOURCE_393_KIND_LIST ((const s32 *)0x02008f50)
#define RESOURCE_393_KIND_BOXES ((const struct Resource393Box *)0x02008f68)

struct Resource393Object *Func_020011ee();

struct Resource393Object *Func_0200034c(s32 *directionOut, s32 *slotOut,
                                         s32 *kindIndexOut)
{
    u8 *scene = RESOURCE_393_SCENE;
    struct Resource393Object *player = Func_020011ee(0);
    struct Resource393Object **objects;
    s32 slot;

    *directionOut = ((const u16 *)player)[3] >> 12;
    objects = (struct Resource393Object **)(scene + 0x34);
    for (slot = 8; (u32)slot <= 65; slot++, objects++) {
        struct Resource393Object *object = *objects;
        struct Resource393Anchor *anchor = (struct Resource393Anchor *)object;
        s16 kind;
        const s32 *kinds;
        const struct Resource393Box *box;
        s32 index;

        index = 0;
        kind = object->handle->kind->id;
        box = RESOURCE_393_KIND_BOXES;
        kinds = RESOURCE_393_KIND_LIST;

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
            step = RESOURCE_393_DIRECTION_STEPS[*directionOut];
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
