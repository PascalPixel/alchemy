#ifndef ALCHEMY_PROJECT_H
#define ALCHEMY_PROJECT_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* The projection Render_ProjectPoint applies after the scene transform. */
struct Projection {
    s32 focal;
    s32 near;
    s32 far;
    s32 center_x;
    s32 center_y;
};

LAYOUT_SIZE_GUARD(Projection_Size, struct Projection, 0x14);
LAYOUT_OFFSET_GUARD(Projection_CenterX, struct Projection, center_x, 0x0c);
LAYOUT_OFFSET_GUARD(Projection_CenterY, struct Projection, center_y, 0x10);

extern struct Projection gProjection;

#endif
