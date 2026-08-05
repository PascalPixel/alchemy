#include "types.h"

/*
 * resource_392 owner at 0x0200034c, 296 bytes: code through the value-returning
 * interworking epilogue at 0x02000452-0x02000463 and its four-word literal pool
 * at 0x02000464-0x02000473.  The next exact-C owner starts at 0x02000474.
 *
 * Given three output cells, this routine reports actor 0's direction, scans
 * actor slots 8..65, and tests six type-specific rectangles against the cell
 * one packed direction step ahead of actor 0.  On the first admissible hit it
 * returns that actor and writes rectangle index plus movement code 8; otherwise
 * it returns null.  The type, rectangle and step tables are independently
 * located at 0x02008f08, 0x02008f20 and 0x02008ec8 in this overlay.
 *
 * This control-flow family occurs at the same entry in ten overlays, but the
 * three table addresses differ in every namespace.  Each source therefore
 * carries its own resolved data addresses and is verified independently.
 *
 * Call accounting: one reachable BL, the actor-0 accessor Func_0808a080.
 * There is no call_via or m2c truncation hazard.
 */

#define RESOURCE_392_SCENE (*(u8 *volatile *)0x03001ebc)

u8 *Func_0808a080();

u8 *Func_0200034c(s32 *direction_out, s32 *movement_out, s32 *rectangle_out)
{
    const s32 *types = (const s32 *)0x02008f08;
    const s32 (*rectangles)[4] = (const s32 (*)[4])0x02008f20;
    const s32 *steps = (const s32 *)0x02008ec8;
    u8 **slots = (u8 **)(RESOURCE_392_SCENE + 0x34);
    u8 *leader = Func_0808a080(0);
    s32 direction = *(u16 *)(leader + 6) >> 12;
    s32 packed_step = steps[direction];
    s32 ahead_x = ((*(s32 *)(leader + 8) >> 16) + (packed_step >> 16)) >> 4;
    s32 ahead_z = ((*(s32 *)(leader + 16) >> 16) + (s16)packed_step) >> 4;
    s32 slot;

    *direction_out = direction;

    for (slot = 8; slot <= 65; slot++) {
        u8 *actor = slots[slot];
        u8 *subrecord = *(u8 **)(actor + 0x50);
        s16 actor_type = **(s16 **)(subrecord + 0x28);
        s32 rectangle;

        for (rectangle = 0; rectangle <= 5; rectangle++) {
            s32 min_x;
            s32 min_z;
            s32 max_x;
            s32 max_z;

            if (actor_type != types[rectangle])
                continue;

            min_x = (*(s16 *)(actor + 0x0a) + rectangles[rectangle][0]) >> 4;
            min_z = (*(s16 *)(actor + 0x12) + rectangles[rectangle][1]) >> 4;
            *rectangle_out = rectangle;
            max_x = (*(s16 *)(actor + 0x0a) + rectangles[rectangle][2]) >> 4;
            max_z = (*(s16 *)(actor + 0x12) + rectangles[rectangle][3]) >> 4;

            if (min_x > ahead_x || ahead_x >= max_x ||
                min_z > ahead_z || ahead_z >= max_z)
                continue;

            if ((rectangle & 1) != 0) {
                if (min_x == (*(s32 *)(leader + 8) >> 20))
                    continue;
            } else {
                if (min_z == (*(s32 *)(leader + 16) >> 20))
                    continue;
            }

            *movement_out = 8;
            return actor;
        }
    }

    return 0;
}
