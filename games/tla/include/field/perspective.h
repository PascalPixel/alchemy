#ifndef ALCHEMY_PERSPECTIVE_H
#define ALCHEMY_PERSPECTIVE_H

// The world map's perspective view state.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

// The world map's perspective view, in the map's heap slot: the followed position, screen shake,
// view position, window, pitch and yaw, a 32 by 32 grid of tile ids and zoom, then the still
// image's state and the region tint.
typedef struct WORLD_MAP_VIEW
{
    Sint32 *origin;                     // the followed point, as in MAP_STATE
    Sint32  shakeX, shakeY, shakeDecay; // the screen shake, as in MAP_STATE
    Uint8  *script;                     // NULL at load
    Uint16
        // MAP_STATE's flags: REG_DISPCNT bits, BG0 to BG3 and the sprites at
        // load
                  shownLayers;
    Uint8         wideTiles; // MAP_STATE's wideTiles, 0x80 at load
    // As MAP_STATE: footprints, tileset animations and blend script.
    Uint8         animationsAndBlend[205];
    FIXED         view[2];              // the view's top left, x then y, 16.16 fixed-point pixels
    FIXED         scaleX;               // 32 in 16.16 at load
    FIXED         scaleY;               // 64 in 16.16 at load
    FIXED         limitX;               // 16352 in 16.16 at load
    FIXED         limitY;               // 16352 in 16.16 at load
    Uint8         unusedAfterLimitY[4]; // no code names it
    // Set by fdWorldMapStartStillImage while BG1 shows the drawn map; the tileset animations are
    // then neither suspended nor resumed.
    Sint8         animationActive;
    Uint8         unused[3];
    // Screen rows, 0 and 159 at load and 200 and 255 from fdWorldMapStartStillImage; each tick
    // copies them to shownTop and shownBottom and picks BG mode 2 or 0 from them.
    Uint16        windowTop;
    Uint16        windowBottom;
    Uint16        shownTop;      // windowTop as the tick update last applied it
    Uint16        shownBottom;   // windowBottom as the tick update last applied it
    Uint16        windowPending; // cleared by the tick update; not set by name
    Uint8         unusedAfterWindow[6];
    // The world map's tile offsets (ResourceIdPerspectiveDataA), each tile LZ-compressed.
    void         *tiles;
    Uint8         unusedAfterTiles[4];
    Uint16        pitch; // 1/65536 turns
    Uint16        yaw;   // 1/65536 turns
    // A row of WorldMapGraphicsResources, by the region the party is in.
    const Uint32 *resources;
    Uint8         unusedAfterResources[36];
    // the world map tile in each of 32 by 32 metatiles
    Uint16        tileIds[32 * 32];
    // The tile each quarter of each layer's block buffer shows, so an unchanged quarter is not
    // decoded again.
    Uint16        quarterTiles[8];
    // Steps left of reloading the area's graphics after the region changed, one a tick from 12
    // (fdWorldMapPreloadCurrentTileBlock).
    Sint32        reloadSteps;
    Sint32        farPlane;            // 510 pixels at load
    Sint32        distance;            // farPlane scaled by the zoom
    Sint32        prevZoom;            // the zoom a transition starts from
    FIXED         zoom;                // 16.16 fixed point, toFIXED(1) full size
    Uint16        zoomTransitionFrame; // a zoom transition's length; 0 when none
    Sint16        zoomTransitionStep;  // ticks of the transition done
    // Set when a field Psynergy cast on the World Map has used the second half of the BG tile
    // buffer; the still image then decodes it again.
    Sint8         tileBufUsed;
    // Set while BG1 shows the still image: the view then only shakes BG1.
    Sint8         stillImageShown;
    // Set while the view is held for the still image or the map screen: no tile is preloaded, and
    // without a shake the tick does nothing.
    Sint8         viewHeld;
    Sint8         tintRegion; // the encounter region the tint was chosen for
    Sint8         tint;       // brightness steps, darker when negative
    Sint8         tintTarget; // the tint the region asks for
    Sint8         fixedEye;   // set: the tick update leaves the eye where it is
} WORLD_MAP_VIEW;

STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, view) == 228);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, animationActive) == 256);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, windowTop) == 260);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, tiles) == 276);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, pitch) == 284);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, resources) == 288);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, tileIds) == 328);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, reloadSteps) == 2392);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, farPlane) == 2396);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, zoom) == 2408);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, tileBufUsed) == 2416);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, tintRegion) == 2419);
STATIC_ASSERT(offsetof(WORLD_MAP_VIEW, fixedEye) == 2422);
STATIC_ASSERT(sizeof(WORLD_MAP_VIEW) == 2424);

#endif
