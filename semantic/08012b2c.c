#include "layout_guard.h"
#include "types.h"

typedef struct ResourceMetadata_08012b2c {
    u8 padding00[4];
    u8 kind;
} ResourceMetadata_08012b2c;

typedef struct RenderEntry_08012b2c {
    u8 padding00[0x28];
    ResourceMetadata_08012b2c *metadata;
} RenderEntry_08012b2c;

typedef struct RenderState_08012b2c {
    RenderEntry_08012b2c *entries;
} RenderState_08012b2c;

typedef struct RadialPosition_08012b2c {
    s32 x;
    s32 y;
    s32 z;
    s32 padding0c;
} RadialPosition_08012b2c;

LAYOUT_OFFSET_GUARD(
    ResourceMetadata08012b2c_Kind,
    ResourceMetadata_08012b2c,
    kind,
    4);
LAYOUT_OFFSET_GUARD(
    RenderEntry08012b2c_Metadata,
    RenderEntry_08012b2c,
    metadata,
    0x28);
LAYOUT_SIZE_GUARD(
    RadialPosition08012b2c_Size,
    RadialPosition_08012b2c,
    0x10);

extern RenderState_08012b2c Data_03001e60;
extern volatile u32 Data_03001ae8;
extern volatile u32 Data_03001800;

void Func_0800447c(s32, s32, s32[3]);

static void PlaceRadialPoint_08012b2c(
    RadialPosition_08012b2c *point,
    s32 center_x,
    s32 center_z,
    s32 radius,
    u16 heading)
{
    point->x = center_x << 16;
    point->y = 0;
    point->z = center_z << 16;
    Func_0800447c(radius, heading, &point->x);
}

/*
 * Build the menu preview's radial positions around the requested center.
 * Resource kind chooses a six-, eight-, ten-, paired-, or four-point ring;
 * holding the animation key rotates the ring with the global frame counter.
 */
void Func_08012b2c(
    s32 center_x,
    s32 center_z,
    RadialPosition_08012b2c *positions)
{
    u8 kind = Data_03001e60.entries[0].metadata->kind;
    u16 heading = 0;
    u16 step;
    s32 radius;
    s32 count;
    s32 paired = 0;
    s32 index;

    if ((Data_03001ae8 & 2) != 0)
        heading = (u16)(Data_03001800 << 8);

    switch (kind) {
    case 3:
        radius = 0x380000;
        step = 0x2aaa;
        count = 6;
        break;
    case 8:
    case 0x2c:
    case 0x58:
        radius = 0x380000;
        step = 0x2000;
        count = 8;
        break;
    case 4:
    case 6:
        radius = 0x380000;
        step = 0x1999;
        count = 10;
        break;
    case 0x14:
        radius = 0x280000;
        heading += 0x4000;
        step = 0x8000;
        count = 4;
        paired = 1;
        break;
    default:
        radius = 0x380000;
        heading += 0x2000;
        step = 0x4000;
        count = 4;
        break;
    }

    for (index = 0; index < count; index++) {
        PlaceRadialPoint_08012b2c(
            &positions[index],
            center_x,
            center_z,
            radius,
            heading);
        if (!paired || (index & 1) != 0)
            heading += step;
    }
}
