#ifndef ALCHEMY_FIELD_SPRITE_H
#define ALCHEMY_FIELD_SPRITE_H

// An actor's sprite as the field's scenes read it.

#include "fixed_math.h"
#include "types.h"

struct SPR;
struct SPR_ANIMATION;

// SPR (field/sprite.h) as the field's scenes read it: its image's

// OAM attributes as bit fields, its retain, own-scale and sunk-rows bits, its layer count and its
// shadow's first two attributes as bytes, and its shadow's attribute 2 as bit fields; the first
// layer is record.
typedef struct FIELD_SPR
{
    // The link to the next sprite queued at its depth (struct ANIMATION_SPR_PART's first word).
    struct SPR           *link;
    // The image's OAM attributes 0 to 2.
    Uint16                y           : 8; // screen pixels
    Uint16                affine      : 2;
    Uint16                objectMode  : 2;
    Uint16                mosaic      : 1;
    Uint16                fullColor   : 1;       // 256 colours
    Uint16                shape       : 2;
    Uint16                x           : 9;       // screen pixels
    Uint16                affineIndex : 5;
    Uint16                size        : 2;
    Uint16                tile        : 10;      // first sprite tile in VRAM
    Uint16                priority    : 2;
    Uint16                palette     : 4;
    Uint8                 unusedAfterPalette[2]; // no code names it
    FIXED                 scale;                 // 16.16 fixed point, toFIXED(1) full size
    Uint8                 slot;                  // VRAM cache entry holding its tiles
    Uint8                 retainCacheEntry : 1;
    Uint8                 useOwnScale      : 1;
    // rows SPRITE_FLAG_SUNK sinks or SPRITE_FLAG_SHADED * shades
    Uint8                 sunkRows         : 6;
    Uint16                rotation; // 1/65536 turns
    Uint8                 width;    // pixels; zero marks a free sprite
    Uint8                 height;   // pixels
    Sint8                 offsetX;  // pixels
    Sint8                 offsetY;  // pixels
    // The animation number last selected, saved with the actor's snapshot.
    Uint8                 lastNo;
    Uint8                 dirty; // nonzero redraws its pixels into VRAM when next drawn
    // The sprite flags (SPR_FLAG), saved with the actor's snapshot; zeroed for sprites shown
    // without them.
    Uint8                 flags;
    Uint8                 count; // the layer count (SPR.count), cleared with the first layer
    // The shadow's link and attributes 0 and 1; no code names them.
    Uint8                 unusedShadow[8];
    // The shadow's OAM attribute 2.
    Uint16                secondTile    : 10;
    Uint16                partPriority  : 2;
    Uint16                secondPalette : 4;
    Uint8                 unusedAfterSecondPalette[2]; // no code names it
    struct SPR_ANIMATION *record;                      // released by fdSpriteAnimationClearStub
} FIELD_SPR;

// The depth a sprite the scene draws itself takes past its screen row, just behind an actor's image
// (+ PLACEMENT_IMAGE_DEPTH, field/field.h) and shadow (two slots less) at the same spot: light
// sprites, a hole's mask, map metatile sprites, fountains and ropes.
#define SCENE_SPRITE_DEPTH 58

#endif
