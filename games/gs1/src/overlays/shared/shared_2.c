#include "types.h"
#include "scene.h"

/* overlays/shared/calculate_position_distance.c */
typedef s32 (*IwramSqrt02000314)(s32);

s32 Sys_Run(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return ((IwramSqrt02000314) 0x030001D8)(dxsq + dysq + dzsq);
}

/* overlays/shared/find_scene_actor_at_fixed_point_position.c */
extern u8 *gWork;

s32 *Sys_Run(s32 *arg0)
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
