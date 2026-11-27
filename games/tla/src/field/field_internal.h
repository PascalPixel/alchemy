#ifndef ALCHEMY_FIELD_INTERNAL_H
#define ALCHEMY_FIELD_INTERNAL_H

// What every part of the field object shares: its includes and the names two or more parts use.
// field.c includes it before the parts, and each part includes it too, so that a part read on its
// own still resolves. It places no bytes.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "system.h"
#include "field/sprite.h"
#include "field/perspective.h"
#include "field/world_map_affine.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "field/actor.h"
#include "field/actor_view.h"
#include "battle/battle.h"
#include "field/field.h"
#include "field/field_state.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "field/field_mode.h"
#include "sprite_ids.h"

// How far off its heading the leader's pad movement probes for a clear way: a sixteenth of a
// turn, 22.5 degrees, and three times that.
#define FIELD_PROBE_ANGLE      (TRIG_TURN / 16)
// The most a blocked actor turns toward its heading in one tick.
#define FIELD_BLOCKED_TURN_MAX (TRIG_TURN / 16)
// Two 16.16 coordinates fall in the same 16-pixel metatile, rounding to nearest.
#define SAME_METATILE(a, b)    ((((a) - (b) + toFIXED(8)) >> 20) == 0)

struct ACTOR;
struct POS;

Sint32       fdMapGridCheckCollisionAbove(ACTOR *actor, POS *pos);

// The map converter copied to RAM (fdRenderConvertMapCode), which joins a decoded map's planes into
// its metatile words.
typedef void (*CONVERT_MAP_ROUTINE)(void *destination, void *source, void *saved);

// A point the fixed camera looks from or at.
typedef struct ACTOR_CAMERA_POINT
{
    FIXED x; // 16.16 fixed-point pixels
    FIXED y; // 16.16 fixed-point pixels
    FIXED z; // 16.16 fixed-point pixels
} ACTOR_CAMERA_POINT;

// The start of the 3D camera's state (heap slot cameraState), as the fixed camera reads it: the eye
// and target it looks from and at, or the points that stand in for them.
typedef struct ACTOR_CAMERA_FIXED
{
    ACTOR_CAMERA_POINT  eye;
    ACTOR_CAMERA_POINT  target;
    ACTOR_CAMERA_POINT *followEye; // when set, used as the eye
    ACTOR_CAMERA_POINT *followPos; // when set, used as the target
} ACTOR_CAMERA_FIXED;

// The leader's sprite, or a ripple's (SPR), as the player's movement sets them, a byte at a time,
// under SPR's names.
typedef struct PLAYER_SPR
{
    Uint8  unused[5]; // no code names it
    Uint8  affine     : 2;
    Uint8  objectMode : 2;
    Uint8  mosaic     : 1;
    Uint8  fullColor  : 1;
    Uint8  shape      : 2;
    Uint8  unusedAfterShape[3];   // no code names it
    Uint8  tileHigh : 2;          // tile number bits 9..8
    Uint8  priority : 2;
    Uint8  palette  : 4;
    Uint8  unusedAfterPalette[7]; // no code names it
    Uint8  cacheFlags : 2;        // retainCacheEntry and useOwnScale
    // rows SPRITE_FLAG_SUNK sinks or SPRITE_FLAG_SHADED * shades
    Uint8  sunkRows   : 6;
    Uint16 rotation;
    Uint8  unusedAfterRotation[5]; // no code names it
    Uint8  dirty;
    Uint8  flags;
    Uint8  unusedAfterFlags[17];   // no code names it
    SPR_ANIMATION
    *secondLayer;                  // entries[1]; above priority 3 hidden
} PLAYER_SPR;

// One background layer's setup in a layout's header: its origin, its parallax and drift, and where
// it wraps.
typedef struct MAP_HEADER_LAYER
{
    Uint8 x;       // 8-pixel tiles
    Uint8 y;       // 8-pixel tiles
    Sint8 scrollX; // sixteenths of the view's motion
    Sint8 scrollY; // sixteenths of the view's motion
    Sint8 speedX;  // 1/16 pixels per tick
    Sint8 speedY;  // 1/16 pixels per tick
    Uint8 periodX; // 8-pixel tiles
    Uint8 periodY; // 8-pixel tiles
} MAP_HEADER_LAYER;

// Where a layout's tileset animation script is decoded, in the map collision buffer after the
// collision, as The Broken Seal's; its blend script follows at MAPGRID_BLEND_SCRIPT
// (field/map_grid.h).
#define MAPGRID_ANIMATION_SCRIPT (RamMapCollision + 4096)

// A layout's header: the camera's bounds, the layers' priorities, tile bases and setup, and the
// offsets, from the header's start, of its packed metatile tiles, collision, metatiles, shapes,
// tileset animation, blend script and script.
typedef struct MAP_HEADER
{
    // The camera's bounds in 8-pixel tiles: left, top, right and bottom.
    Uint8            cameraBounds[4];
    // Each layer's BG priority, 1 to 3; 0 leaves the layer off.
    Uint8            priority[3];
    Uint8            charBase[3]; // each layer's BG char block
    Uint16           reserved;    // nothing reads it
    MAP_HEADER_LAYER layers[3];
    // staged and unpacked into the metatile tiles buffer
    Sint32           metatileTiles;
    Sint32           collision; // into RamMapCollision
    Sint32           metatiles; // into RamMapMetatileBuffer
    Sint32           shapes;    // into MapShapeGrid
    Sint32           animation; // the tileset animation script, or 0 for none
    Sint32           blend;     // the blend script, or 0 for none
    // Into RamMapCollision after its MAPGRID_SHAPE_TABLE_BYTES.
    Sint32           script;
} MAP_HEADER;

// A map layer's scroll position in whole 16-pixel metatiles.
typedef struct MAP_GRID_TILE_POS
{
    Sint32 x, y;
} MAP_GRID_TILE_POS;

// One step of a map edit: the rectangle of metatiles to copy from and the wait after it.
typedef struct MAP_GRID_METATILE_ANIMATION_STEP
{
    // The source rectangle's x in metatiles, or MAPGRID_ANIMATION_STEPS_END.
    Uint16 src;
    // The source rectangle's y, width and height in metatiles, and the ticks to wait after the
    // copy.
    Sint16 y, width, height, delay;
} __attribute__((packed, aligned(2))) MAP_GRID_METATILE_ANIMATION_STEP;

// A sprite (SPR) whose image priority, its layer, is copied a byte at a time, under SPR's names: a
// wading splash's.
typedef struct SPR_LAYER_VIEW
{
    Uint8 unused[9];              // no code names it
    Uint8 tileHigh : 2;           // tile number bits 9..8
    Uint8 layer    : 2;
    Uint8 palette  : 4;
    Uint8 unusedAfterPalette[16]; // no code names it
    Uint8 flags;
} SPR_LAYER_VIEW;

// The actor as SC_SYNC clears it when it gives up waiting: its target, arrival axis and blocked
// count. Read through ACTOR, fdActorScriptWaitForArrival compiles otherwise.
typedef struct WAITING_ACTOR
{
    Uint8  unused[56]; // no code names it
    Sint32 targetX;
    Sint32 targetY;
    Sint32 targetZ;
    Uint8  unusedAfterTargetZ[18];    // no code names it
    Uint8  arrivalAxis;
    Uint8  unusedAfterArrivalAxis[9]; // no code names it
    Uint8  refused;
} WAITING_ACTOR;

// SPR_ANIMATION.kind: the facing table a sprite animation's pictures follow
// (fdSpriteComposePicture), as The Broken Seal names them; this game's five pictures cover 64
// sectors.
typedef enum ANIMATION_FACING_KIND
{
    ANIM_FACING_NONE                    = 0, // the script's own picture whatever the facing
    // 8 sectors: one picture, mirrored to the left
    ANIM_FACING_ONE_PICTURE             = 1,
    ANIM_FACING_FRONT_BACK              = 2, // 8 sectors: a front and a back picture
    ANIM_FACING_THREE_PICTURES          = 3, // 16 sectors: front, side and back
    // Unused, with no facing table: drawn as ANIM_FACING_NONE. The sprite viewer rings it ten
    // round, as The Broken Seal's does its four pictures.
    ANIM_FACING_FOUR_PICTURES           = 4,
    ANIM_FACING_FIVE_PICTURES           = 5, // 64 sectors here, 16 in The Broken Seal
    // Unused, with no facing table: drawn as ANIM_FACING_NONE. The sprite viewer still rings it ten
    // round.
    ANIM_FACING_SIX_PICTURES            = 6,
    // 8 sectors centred on the compass directions, a picture for each.
    ANIM_FACING_EIGHT_PICTURES          = 8,
    // ANIM_FACING_FRONT_BACK's table for a sprite drawn in two parts, the second the next sprite id
    // (the sprite viewer).
    ANIM_FACING_TWO_PART                = 20,
    // As ANIM_FACING_FRONT_BACK, with the front picture mirrored the other way.
    ANIM_FACING_FRONT_BACK_SWAPPED      = 22,
    // Unused, with no facing table: rung as eight facings by the sprite viewer, as The Broken
    // Seal's is.
    ANIM_FACING_EIGHT_NO_TABLE          = 44,
    // Unused, with no facing table: The Broken Seal's eight pictures with three mirrored, rung as
    // eight facings by the sprite viewer.
    ANIM_FACING_EIGHT_PICTURES_MIRRORED = 88
} ANIMATION_FACING_KIND;

// A facing table entry's offset of its picture from the base picture.
#define ANIM_FACING_PICTURE_MASK          7
// A facing table entry's bit that mirrors its picture.
#define ANIM_FACING_MIRRORED              (1 << 7)
// The sector a facing angle lies in, a turn cut into `sectors`.
#define FACING_SECTOR(direction, sectors) ((direction) / (TRIG_TURN / (sectors)) % (sectors))

// The field's pool sizes, and a sprite animation's timer step at normal speed.
enum
{
    SPRITE_PICTURE_CACHE_COUNT   = 8,  // ACTOR_SYSTEM_STATE.pictureCaches
    SPRITE_ANIMATION_COUNT       = 4,  // SPR.entries
    RUNNING_LIST_SLOTS           = 14, // the actor slots fdActorListRunning reads
    // Sixteenths of a tick taken off a sprite animation's timer each tick: one tick.
    SPRITE_ANIMATION_NORMAL_STEP = 16
};

// What stopped the leader's step this tick (fdActorUpdatePlayerControl, fdActorMoveOnWorldMap,
// fdActorUpdateHoverControl and the climbing and rope controls).
typedef enum PLAYER_STEP_BLOCK
{
    PLAYER_STEP_BLOCKED_BY_MAP   = 1, // every heading tried runs into the map
    // A solid actor ahead, or one with no room to be pushed.
    PLAYER_STEP_BLOCKED_BY_ACTOR = 2,
    PLAYER_STEP_BLOCKED          = 3, // either of the two
    // No step, and none counted as blocked: the pad is not held, or a climb meets a wall.
    PLAYER_STEP_STOPPED          = 4
} PLAYER_STEP_BLOCK;

// Layered-map viewport distances in 16.16 pixels. The followed point is below the vertical centre;
// scroll bounds and camera following share this origin.
enum
{
    FIELD_VIEW_FOCUS_X = toFIXED(120), // followed point from the left edge
    FIELD_VIEW_FOCUS_Y = toFIXED(96),  // followed point from the top edge
    FIELD_VIEW_WIDTH   = toFIXED(240), // the screen's width
    FIELD_VIEW_HEIGHT  = toFIXED(160)  // the screen's height
};

// How a layout's metatile tiles data is packed: the method byte fdTileMapDecodeStagedBuffer reads
// before the halfwords.
typedef enum METATILE_TILES_PACKING
{
    METATILE_TILES_PACKING_RAW         = 0, // copied as it is
    // All the high bytes, then all the low bytes; each value is XORed with the one before.
    METATILE_TILES_PACKING_SPLIT_DELTA = 1,
    // each value is XORed with the one before
    METATILE_TILES_PACKING_DELTA       = 2
} METATILE_TILES_PACKING;

// A row of SpritePictureCacheSources: the packed resource that holds an animation number's
// pictures, which fdSpritePictureCacheLoad decodes into a sprite picture cache; a number of 0 ends
// the table.
typedef struct SPR_PICTURE_CACHE_SOURCE
{
    Uint16 resource;
    Sint16 number;
} SPR_PICTURE_CACHE_SOURCE;

// The size of fdWorldMapUpdateAffineLinesCode, an address equal to the size (ld_script.ld).
extern Uint8                    fdWorldMapUpdateAffineLinesCodeSize[];

// The map converter's Thumb entry and ARM code (map_grid_convert.s), copied to RAM to run.
extern Uint8                    fdRenderConvertMapCode[];

// The size of fdRenderConvertMapCode (map_grid_convert.s).
extern Uint8                    fdRenderConvertMapCodeSize[];

// Each animation number's packed pictures, ending at number 0 (sprite_sources.s).
extern SPR_PICTURE_CACHE_SOURCE SpritePictureCacheSources[];

// The sprites' shadow, 16 by 8 at 256 colours, loaded into VRAM cache entry
// VRAM_CACHE_ENTRY_ACTOR_SHADOW (actor_shadow.list.tsv).
extern const Uint8              ActorShadowTiles[];

// The strided copy's Thumb entry and ARM code (copy_strided.s), copied to RAM to run.
extern Uint8                    fdRenderCopyStridedCode[];

// The size of fdRenderCopyStridedCode (copy_strided.s).
extern Uint8                    fdRenderCopyStridedCodeSize[];

// Copies fdRenderDecodePicture into the tick scratch slot, the block sized for the function and the
// colour tables it keeps after its code; the copy stops at the tables, which the decoder fills
// itself. As RAM_CODE_LOAD_SLOT, a statement.
#define btRenderLoadDecoder()                                                                      \
    do                                                                                             \
    {                                                                                              \
        syDMAXCopy(                                                                                \
            fdRenderDecodePictureCode,                                                             \
            syHeapSlotAllocate(HEAP_SLOT_TICK_SCRATCH, (Sint32)RenderDecodePictureBlockSize),      \
            DMA_COPY_WORDS((Uint32)(fdRenderDecodePictureCopyEnd - fdRenderDecodePictureCode)      \
                           >> 2),                                                                  \
            REG_DMA3);                                                                             \
    } while (0)

// field_view.c's functions, which field_tile_animation.c and field_world_map.c call before it.
void fdTileMapDrawRowsFromRam(const WORLD_MAP_AFFINE_PAIR *lines, Uint8 *pixels);
void fdWorldMapBuildScanlineTable(FIXED pitchSlope, Sint32 *pos, WORLD_MAP_SCANLINE *scanline);

// The footprints the leader leaves in snow and sand, and the leader's userValue that asks for the
// pair next. Every function that moves the leader by the pad sets it on a stop.
typedef enum FOOTPRINT_ANIMATION
{
    FOOTPRINT_ONE_FOOT  = 1, // one print, fading
    // Two prints side by side, fading: laid first after a stop.
    FOOTPRINT_BOTH_FEET = 2
} FOOTPRINT_ANIMATION;

#endif // ALCHEMY_FIELD_INTERNAL_H
