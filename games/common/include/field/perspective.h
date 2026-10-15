#ifndef ALCHEMY_PERSPECTIVE_H
#define ALCHEMY_PERSPECTIVE_H

#include "types.h"
#include "layout_guard.h"

struct PerspectiveWork
{
    s32  *origin;
    s32   shakeX, shakeY, shakeDecay;
    u8   *script;
    u16   fade;
    u8    fadeStep;
    u8    unk017[0xe4 - 0x17];
    s32   viewX;
    s32   viewY;
    s32   scaleX;
    s32   scaleY;
    s32   limitX;
    s32   limitY;
    s8    animationActive;
    u8    unk0fd[3];
    u16   windowTop;
    u16   windowBottom;
    u16   shownTop;
    u16   shownBottom;
    u16   windowPending;
    u8    unk10a[6];
    void *tiles;
    u8    unk114[4];
    u16   pitch;
    u16   yaw;
    u32  *resources;
    u8    unk120[0x18];
    u16   tileIds[256];
    u16   quarterTiles[8];
    s32   farPlane;
    s32   distance;
    s32   previousZoom;
    s32   zoom;
    u16   zoomTransitionFrames;
    s16   zoomTransitionStep;
};

LAYOUT_SIZE_GUARD(PerspectiveWork_Size, struct PerspectiveWork, 0x35c);
LAYOUT_OFFSET_GUARD(PerspectiveWork_View, struct PerspectiveWork, viewX, 0xe4);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Pitch, struct PerspectiveWork, pitch, 0x118);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Active, struct PerspectiveWork, animationActive, 0xfc);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Resources, struct PerspectiveWork, resources, 0x11c);
LAYOUT_OFFSET_GUARD(PerspectiveWork_TileIds, struct PerspectiveWork, tileIds, 0x138);
LAYOUT_OFFSET_GUARD(PerspectiveWork_QuarterTiles, struct PerspectiveWork, quarterTiles, 0x338);
LAYOUT_OFFSET_GUARD(PerspectiveWork_FarPlane, struct PerspectiveWork, farPlane, 0x348);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Zoom, struct PerspectiveWork, zoom, 0x354);
LAYOUT_OFFSET_GUARD(PerspectiveWork_PreviousZoom, struct PerspectiveWork, previousZoom, 0x350);
LAYOUT_OFFSET_GUARD(
    PerspectiveWork_TransitionFrames, struct PerspectiveWork, zoomTransitionFrames, 0x358);
LAYOUT_OFFSET_GUARD(
    PerspectiveWork_TransitionStep, struct PerspectiveWork, zoomTransitionStep, 0x35a);

#endif
