#include "types.h"
#include "scene.h"

/*
 * Scene-overlay probe trio at 0x02000314 / 0x02000350 / 0x02000528.
 * Bodies match the previous per-function files; names are unique so the
 * three owners can share one translation unit.
 */

/* overlays/shared/calculate_fixed_point_position_distance.c */
typedef s32 (*IwramIntegerSquareRoot)(s32);

s32 CalculateFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot) 0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

/* overlays/shared/find_scene_actor_at_fixed_point_position.c */
extern u8 *gWork;

s32 *FindSceneActorAtFixedPointPosition(s32 *arg0)
{
    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if ((arg0[0] >> 20) == (p[2] >> 20)
            && (arg0[1] / 0x10000) == (p[3] / 0x10000)
            && (arg0[2] >> 20) == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/* overlays/shared/fill_scene_grid_attribute_rectangle.c */
extern u8 *gCam;
extern u8 gUnk[];

s32 FillSceneGridAttributeRectangle(u32 arg0, s32 arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5)
{
    u8 *g = gCam;
    u8 *base;
    u32 i;
    u32 j;

    if (g != 0) {
        if (arg0 <= 2) {
            u32 off = arg0 * 48 + 304;

            base = *(u8 **)(g + off);
        } else {
            base = gUnk;
        }
        base += (arg1 + (arg2 << 7)) * 4;
        for (i = 0; i < arg4; i++) {
            u8 *p = base + (i << 9);

            for (j = 0; j < arg3; j++) {
                p[2] = (u8)arg5;
                p += 4;
            }
        }
    }
    return 0;
}
