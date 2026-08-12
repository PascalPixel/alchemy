#include "types.h"

/*
 * resource_389 owner at 0x0200034c, 296 bytes: code through the value-returning
 * interworking epilogue at 0x02000452-0x02000463 and its four-word literal pool
 * at 0x02000464-0x02000473.  The next exact-C owner starts at 0x02000474.
 *
 * Given three output cells, this routine reports actor 0's direction, scans
 * actor slots 8..65, and tests six type-specific rectangles against the cell
 * one packed direction step ahead of actor 0.  On the first admissible hit it
 * returns that actor and writes rectangle index plus movement code 8; otherwise
 * it returns null.  The type, rectangle and step tables are independently
 * located at 0x02009594, 0x020095ac and 0x02009554 in this overlay.
 *
 * This control-flow family occurs at the same entry in ten overlays, but the
 * three table addresses differ in every namespace.  Each source therefore
 * carries its own resolved data addresses and is verified independently.
 *
 * Call accounting: one reachable BL, the actor-0 accessor Func_0808a080.
 * There is no call_via or m2c truncation hazard.
 */

#define RESOURCE_389_SCENE (*(u8 *volatile *)0x03001ebc)
#define RESOURCE_389_DIRECTION_STEPS ((const s32 *)0x02009554)
#define RESOURCE_389_TYPES ((const s32 *)0x02009594)

struct Resource389Kind {
    s16 id;
};

struct Resource389Handle {
    u8 unknown_00[0x28];
    struct Resource389Kind *kind;
};

struct Resource389Actor {
    u8 unknown_00[8];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
    u8 unknown_14[0x3c];
    struct Resource389Handle *handle;
};

struct Resource389Anchor {
    u8 unknown_00[10];
    s16 x;
    u8 unknown_0c[6];
    s16 z;
};

struct Resource389Rectangle {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
};

#define RESOURCE_389_RECTANGLES ((const struct Resource389Rectangle *)0x020095ac)

struct Resource389Actor *Func_0808a080();

struct Resource389Actor *Func_0200034c(s32 *directionOut, s32 *movementOut,
                                        s32 *rectangleOut)
{
    u8 *scene = RESOURCE_389_SCENE;
    struct Resource389Actor *leader = Func_0808a080(0);
    struct Resource389Actor **actors;
    s32 slot;

    *directionOut = ((const u16 *)leader)[3] >> 12;
    actors = (struct Resource389Actor **)(scene + 0x34);

    for (slot = 8; (u32)slot <= 65; slot++, actors++) {
        struct Resource389Actor *actor = *actors;
        struct Resource389Anchor *anchor = (struct Resource389Anchor *)actor;
        s16 type = actor->handle->kind->id;
        const s32 *types = RESOURCE_389_TYPES;
        const struct Resource389Rectangle *rectangle = RESOURCE_389_RECTANGLES;
        s32 index;

        for (index = 0; (u32)index <= 5; index++, rectangle++) {
            s32 step;
            s32 aheadX;
            s32 aheadZ;
            s32 minX;
            s32 minZ;
            s32 maxX;
            s32 maxZ;
            s32 leaderX;
            s32 leaderZ;

            if (type != *types++)
                continue;

            *rectangleOut = index;
            leaderX = leader->x;
            leaderZ = leader->z;
            step = RESOURCE_389_DIRECTION_STEPS[*directionOut];
            aheadX = ((leaderX >> 16) + (step >> 16)) >> 4;
            aheadZ = ((leaderZ >> 16) + ((step << 16) >> 16)) >> 4;
            minX = (anchor->x + rectangle->x0) >> 4;
            minZ = (anchor->z + rectangle->z0) >> 4;
            maxX = (anchor->x + rectangle->x1) >> 4;
            maxZ = (anchor->z + rectangle->z1) >> 4;

            if (minX > aheadX || aheadX >= maxX)
                continue;
            if (minZ > aheadZ || aheadZ >= maxZ)
                continue;

            if ((index & 1) != 0) {
                if (minX == (leaderX >> 20))
                    continue;
            } else {
                if (minZ == (leaderZ >> 20))
                    continue;
            }

            *movementOut = 8;
            return actor;
        }
    }

    return 0;
}
