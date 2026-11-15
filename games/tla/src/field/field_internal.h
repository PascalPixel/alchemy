#ifndef ALCHEMY_FIELD_INTERNAL_H
#define ALCHEMY_FIELD_INTERNAL_H

/// @file
/// What every part of the field object shares: its includes and the names two
/// or more parts use. field.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves. It places
/// no bytes.

#include "types.h"
#include "resource_ids.h"
#include "system/scene.h"
#include "field/sprite.h"
#include "field/perspective.h"
#include "field/world_map_affine.h"
#include "system/fixed_point_position.h"
#include "system/ram_buffer.h"
#include "system/io_reg.h"
#include "system/dma.h"
#include "system/io_write.h"
#include "system/heap_release.h"
#include "system/iwram_call.h"
#include "graphics/resource.h"
#include <system/affine.h>
#include "system/sprite_metadata.h"
#include "field/actor.h"
#include "field/actor_view.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/ram_code.h"
#include "field/field_sequence.h"

/// How far off its heading the leader's pad movement probes for a clear way:
/// a sixteenth of a turn, 22.5 degrees, and three times that.
#define FIELD_PROBE_ANGLE (TRIG_TURN / 16)
/// The most a blocked actor turns toward its heading in one frame.
#define FIELD_BLOCKED_TURN_MAX (TRIG_TURN / 16)

struct Actor;
struct FieldPosition;

/// The collision at an actor's probe point (tile_animation.c): 2 for a cell
/// with no tile, 1 a rise of more than 8 pixels, -1 a drop of more than 12,
/// else 0.
s32 MapGrid_CheckCollision(struct Actor *actor, struct FieldPosition *position);
s32 MapGrid_CheckCollisionAbove(struct Actor *actor,
                                struct FieldPosition *position);

typedef void (*ConvertFn)(void *destination, void *source, void *saved);

/// The words of an object's script. As in The Broken Seal's ActorScriptWords,
/// a flat word pointer changes one load's base and index order.
struct MotionScript {
  s32 word[1];
};

struct PlacedSprite {
  u8 unused[9]; ///< no code names it
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unusedB[2]; ///< no code names it
  s32 scale;     ///< 16.16 fixed point
  u8 slot;
  u8 retainCacheEntry;               ///< bit 0
  u8 unusedAfterRetainCacheEntry[7]; ///< no code names it
  u8 dirty;
  u8 unusedAfterDirty[0x0b]; ///< no code names it
  u8 low25 : 2;
  u8 shadowPriority : 2;
  u8 high25 : 4;
};

/// A point the fixed camera looks from or at.
struct ActorCameraPoint {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
};

struct MapPriorityWork {
  u8 unused[0x101]; ///< no code names it
  s8 basePriority;
};

/// The start of the 3D camera's work (heap slot cameraWork), as the fixed
/// camera reads it: the eye and target it looks from and at, or the points
/// that stand in for them.
struct ActorCameraFixed {
  struct ActorCameraPoint eye;
  struct ActorCameraPoint target;
  struct ActorCameraPoint *followEye;      ///< when set, used as the eye
  struct ActorCameraPoint *followPosition; ///< when set, used as the target
};

struct PlayerSprite {
  u8 unused[5]; ///< no code names it
  u8 low05 : 2;
  u8 objectMode : 2;
  u8 high05 : 4;
  u8 unusedB[3]; ///< no code names it
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unusedC[7]; ///< no code names it
  u8 shape11;
  u16 rotation;
  u8 unk14[5];
  u8 dirty;
  u8 flags;
  u8 unk1b[0x11];
  u8 *entry;
};

struct MapFlagsWork {
  u8 unused[0x17];  ///< no code names it
  u8 groundEffects; ///< bits: 3..2 the wading kind | 1..0 footprints
};

/// One background layer's setup in a layout's header: its origin, its
/// parallax and drift, and where it wraps.
struct MapHeaderLayer {
  u8 x;       ///< 8-pixel tiles
  u8 y;       ///< 8-pixel tiles
  s8 scrollX; ///< sixteenths of the view's motion
  s8 scrollY; ///< sixteenths of the view's motion
  s8 speedX;  ///< 1/16 pixels per frame
  s8 speedY;  ///< 1/16 pixels per frame
  u8 periodX; ///< 8-pixel tiles
  u8 periodY; ///< 8-pixel tiles
};

/// A layout's header: the camera's bounds, the layers' priorities, tile
/// bases and setup, and the offsets, from the header's start, of its packed
/// cell pictures, collision, cells, shapes, tileset animation, blend script
/// and script.
struct MapHeader {
  /// The camera's bounds in 8-pixel tiles: left, top, right and bottom.
  u8 cameraBounds[4];
  /// Each layer's BG priority, 1 to 3; 0 leaves the layer off.
  u8 priority[3];
  u8 charBase[3]; ///< each layer's BG char block
  u16 reserved;   ///< nothing reads it
  struct MapHeaderLayer layers[3];
  s32 cellPictures; ///< staged and unpacked into the cell picture buffer
  s32 collision;    ///< into Ram_MapCollision
  s32 cells;        ///< into Ram_MapCellBuffer
  s32 shapes;       ///< into gMapShapeGrid
  s32 animation;    ///< the tileset animation script, or 0 for none
  s32 blend;        ///< the blend script, or 0 for none
  s32 script;       ///< into Ram_MapCollision + 0x2000
};

struct MapLayerScroll {
  s32 x, y;
  s32 offsetX, offsetY;
  s32 scaleX, scaleY;
  s32 speedX, speedY;
  s32 phaseX, phaseY;
  u16 maskX, maskY;
  u16 cellX, cellY;
  struct MapCell *cells;
  u8 *shapes;
};

struct MapScrollWork {
  u8 unused[0x14]; ///< no code names it
  u16 flags;
  u8 unusedAfterFlags[0xe4 - 0x16]; ///< no code names it
  s32 viewX, viewY;
  s32 minX, minY, maxX, maxY;
  u8 unusedAfterUnusedAfterFlags[5]; ///< no code names it
  u8 unk101;
  u8 unusedB[2]; ///< no code names it
  u8 enabled[3];
  u8 unusedAfterEnabled; ///< no code names it
  struct MapLayerScroll layers[3];
};

struct Unk_WakeActor {
  u8 unused[0x63]; ///< no code names it
  u8 unk63;
};

struct Unk_ClimbEventWork {
  u8 unused[0xd4a]; ///< no code names it
  /* FAKEMATCH(signature): read as the low byte of a halfword. */
  u16 unkd4a : 8;
  u16 unkd4b : 8;
};

struct Unk0802a124Actor {
  u8 unused[0x62]; ///< no code names it
  u8 unk62;
  u8 unk63;
};

/// A map layer as its renderers read it: its scroll, then the window over
/// the map's cells (masks and origins that wrap the source cells).
struct MapLayerWindow {
  s32 x, y;
  s32 offsetX, offsetY;
  s32 scaleX, scaleY;
  s32 speedX, speedY;
  s32 phaseX, phaseY;
  u16 maskX, maskY;
  u16 originX, originY;
  struct MapCell *cells;
  u8 *terrainIds;
};

/// A map layer's scroll position in whole 16-pixel cells.
struct TilePos {
  s32 x, y;
};

/// One step of a map edit: the rectangle of cells to copy from and the
/// wait after it.
struct MapGridCellAnimationStep {
  u16 source; ///< the source rectangle's x in cells, or 0xffff to end
  /// The source rectangle's y, width and height in cells, and the frames to
  /// wait after the copy.
  s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));

struct Child {
  u8 unused[9]; ///< no code names it
  u8 low9 : 2;
  u8 layer : 2;
  u8 high9 : 4;
  u8 unusedB[0x10]; ///< no code names it
  u8 value1a;
};

/// The actor as SC_SYNC clears it when it gives up waiting: its target, arrival
/// axis and blocked count.
struct WaitingActor {
  u8 unused[0x38]; ///< no code names it
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  u8 unusedAfterTargetZ[0x12]; ///< no code names it
  u8 arrivalAxis;
  u8 unusedAfterArrivalAxis[9]; ///< no code names it
  u8 refused;
};

/// The field's pool sizes.
enum {
  RESOURCE_SLOT_COUNT = 8, ///< ActorSystemWork.pictureCaches
  ANIM_OBJECT_COUNT = 64,  ///< sprites and sprite animations
  ANIM_LAYER_COUNT = 4     ///< Sprite.entries
};

/// A row of gResourceSlot_NumberTable: the resource holding the pictures of
/// sprite animation number, which SpritePictureCache_Load decodes into a
/// resource slot; a number of 0 ends the table.
struct ResourceSlotNumber {
  u16 resource;
  s16 number;
};

extern u8 Transform_UpdateVerticesCodeSize[];

extern u8 Render_ConvertMapCode[];

extern u8 Render_ConvertMapCodeSize[];

extern u8 gMapSceneEditsOffset[];

extern struct ResourceSlotNumber gResourceSlot_NumberTable[];

extern const u8 gActor_ShadowTiles[];

extern u8 Render_CopyStridedCode[];

extern u8 Render_CopyStridedCodeSize[];

struct IconOwner {
  u8 unused[80]; ///< no code names it
  struct IconSprite__resource_slot *sprite;
  u8 unusedAfterSprite[8]; ///< no code names it
  u8 unk5c;
  u8 unusedB[5]; ///< no code names it
  u8 iconId;
};

/// Copies Render_DecodePicture into the frame scratch slot, the block sized for
/// the routine and the colour tables it keeps after its code; the copy stops at
/// the tables, which the decoder fills itself. As RamCode_LoadSlot, a
/// statement.
#define Render_LoadDecoder()                                                   \
  do {                                                                         \
    Dma_Set(Render_DecodePictureCode,                                          \
            HeapSlot_Alloc(HEAP_SLOT_FRAME_SCRATCH,                            \
                           (s32)Render_DecodePictureBlockSize),                \
            DMA_COPY_WORDS((u32)(Render_DecodePictureCopyEnd -                 \
                                 Render_DecodePictureCode) >>                  \
                           2),                                                 \
            REG_DMA3);                                                         \
  } while (0)

#endif // ALCHEMY_FIELD_INTERNAL_H
