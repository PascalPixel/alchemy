#ifndef ALCHEMY_MAP_H
#define ALCHEMY_MAP_H

#include "types.h"
#include "layout_guard.h"

struct BlendScriptState
{
    u16 *script;
    u16 *cursor;
    u16  delay;
    u16  paused;
};

struct MapAnimation
{
    const u16 *start;
    const u16 *cursor;
    u16        timer;
    u16        paused;
};

struct MapCell
{
    u16 metatileAndFlags;
    u8  collisionCode;
    u8  attributeB;
};

struct MapLayer
{
    s32             x, y;
    u8              unk08[0x28];
    struct MapCell *cells;
    u8             *terrainIds;
};

struct MapWork
{
    s32                    *origin;
    s32                     shakeX, shakeY, shakeDecay;
    u8                     *script;
    u16                     flags;
    u8                      wideTiles;
    u8                      unk017;
    struct MapAnimation     anim[16];
    struct BlendScriptState blend;
    s32                     viewX;
    s32                     viewZ;
    s32                     minX;
    s32                     minZ;
    s32                     maxX;
    s32                     maxZ;
    u8                      unk0fc[0xb];
    u8                      blendControl;
    struct MapLayer         layers[4];
    u8                      unk1e8[0x774];
    s32                     farPlane;
    s32                     zoom;
    s32                     zoomFrom;
    s32                     zoomTo;
    s16                     zoomFrames;
    s16                     zoomStep;
};

struct MapViewWork
{
    u8   unk000[0x100];
    s8   animationActive;
    u8   unk101[3];
    u16  windowTop;
    u16  windowBottom;
    u16  irqFirstLine;
    u16  irqSecondLine;
    u16  irqPhase;
    u8   unk10e[0x12];
    u32 *resources;
    u8   unk124[0x84c];
    s8   extraTiles;
    u8   frameShown;
    u8   framePending;
    s8   area;
};

struct WorldPosition
{
    s32 x;
    u8  unk04[4];
    s32 y;
};

s32 GetWorldMapCollision(struct WorldPosition *position);

LAYOUT_SIZE_GUARD(MapLayer_Size, struct MapLayer, 56);
LAYOUT_OFFSET_GUARD(MapWork_Layers, struct MapWork, layers, 0x108);

LAYOUT_SIZE_GUARD(MapAnimation_Size, struct MapAnimation, 12);
LAYOUT_OFFSET_GUARD(MapWork_Animation, struct MapWork, anim, 0x18);
LAYOUT_OFFSET_GUARD(MapWork_Blend, struct MapWork, blend, 0xd8);
LAYOUT_OFFSET_GUARD(MapWork_BlendControl, struct MapWork, blendControl, 0x107);
LAYOUT_OFFSET_GUARD(MapWork_MinX, struct MapWork, minX, 0xec);
LAYOUT_OFFSET_GUARD(MapWork_FarPlane, struct MapWork, farPlane, 0x95c);
LAYOUT_OFFSET_GUARD(MapWork_ZoomStep, struct MapWork, zoomStep, 0x96e);

#endif
