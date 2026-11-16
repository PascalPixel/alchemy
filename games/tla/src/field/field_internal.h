#ifndef ALCHEMY_FIELD_INTERNAL_H
#define ALCHEMY_FIELD_INTERNAL_H

/// @file
/// What every part of the field object shares: its includes and the names two
/// or more parts use. field.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves. It places
/// no bytes.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "system/scene.h"
#include "system/heap_slots.h"
#include "field/sprite.h"
#include "field/perspective.h"
#include "field/world_map_affine.h"
#include "system/point.h"
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

/// A point the fixed camera looks from or at.
struct ActorCameraPoint {
  FIXED x; ///< 16.16 fixed-point pixels
  FIXED y; ///< 16.16 fixed-point pixels
  FIXED z; ///< 16.16 fixed-point pixels
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

/// The leader's sprite, or a ripple's (struct Sprite), as the player's
/// movement sets them, a byte at a time, under struct Sprite's names.
struct PlayerSprite {
  u8 unused[5]; ///< no code names it
  u8 affine : 2;
  u8 objectMode : 2;
  u8 mosaic : 1;
  u8 fullColor : 1;
  u8 shape : 2;
  u8 unusedB[3];   ///< no code names it
  u8 tileHigh : 2; ///< tile number bits 9..8
  u8 priority : 2;
  u8 palette : 4;
  u8 unusedC[7];     ///< no code names it
  u8 cacheFlags : 2; ///< retainCacheEntry and useOwnScale
  u8 sunkRows : 6; ///< rows the picture is sunk (flags bit 3) or shaded (bit 2)
  u16 rotation;
  u8 unusedD[5]; ///< no code names it
  u8 dirty;
  u8 flags;
  u8 unusedE[17];                      ///< no code names it
  struct SpriteAnimation *secondLayer; ///< entries[1]; above priority 3 hidden
};

struct MapFlagsWork {
  u8 unused[23];    ///< no code names it
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

/// Where a layout's tileset animation and blend scripts are decoded, in the
/// map collision buffer after the collision, as The Broken Seal's.
#define MAPGRID_ANIMATION_SCRIPT (Ram_MapCollision + 0x1000)
#define MAPGRID_BLEND_SCRIPT (Ram_MapCollision + 0x1e00)

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

/// FieldWork as the sand trail reads its priority
/// (FieldWork.sandTrailPriority).
struct SandTrailWork {
  u8 unused[3402]; ///< no code names it
  u16 trailPriority
      : 8;            ///< read as the low byte of FieldWork.sandTrailPriority
  u16 unusedHigh : 8; ///< no code names it
};

/// A map layer's scroll position in whole 16-pixel cells.
struct MapGridTilePosition {
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

/// A sprite (struct Sprite) whose image priority, its layer, is copied a
/// byte at a time, under struct Sprite's names.
struct Child {
  u8 unused[9];    ///< no code names it
  u8 tileHigh : 2; ///< tile number bits 9..8
  u8 layer : 2;
  u8 palette : 4;
  u8 unusedB[16]; ///< no code names it
  u8 flags;
};

/// The actor as SC_SYNC clears it when it gives up waiting: its target, arrival
/// axis and blocked count.
struct WaitingActor {
  u8 unused[56]; ///< no code names it
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  u8 unusedAfterTargetZ[18]; ///< no code names it
  u8 arrivalAxis;
  u8 unusedAfterArrivalAxis[9]; ///< no code names it
  u8 refused;
};

/// Animation script commands.
/// SpriteAnimation.kind: the facing table a sprite animation's pictures follow
/// (Sprite_ComposePicture), as The Broken Seal names them; this game's six
/// pictures are kind 5.
enum AnimationFacingKind {
  ANIM_FACING_NONE = 0,        ///< the script's own picture whatever the facing
  ANIM_FACING_ONE_PICTURE = 1, ///< 8 sectors: one picture, mirrored to the left
  ANIM_FACING_FRONT_BACK = 2,  ///< 8 sectors: a front and a back picture
  ANIM_FACING_THREE_PICTURES = 3, ///< 16 sectors: front, side and back
  ANIM_FACING_SIX_PICTURES = 5,   ///< 64 sectors
  /// 8 sectors centred on the compass directions, a picture for each.
  ANIM_FACING_EIGHT_PICTURES = 8,
  /// ANIM_FACING_FRONT_BACK's table for a sprite drawn in two parts, the
  /// second the next sprite id (the sprite viewer).
  ANIM_FACING_TWO_PART = 20,
  /// As ANIM_FACING_FRONT_BACK, with the front picture mirrored the other way.
  ANIM_FACING_FRONT_BACK_SWAPPED = 22
};

enum AnimationCommand {
  ANIMCMD_END = 239,      ///< blank the layer and stop its script
  ANIMCMD_SET_KIND = 240, ///< use facing table `arg`
  ANIMCMD_HOLD = 241,     ///< stay on this command and frame for good
  ANIMCMD_OFFSET_Y = 242, ///< set the sprite's offsetY to `arg`
  ANIMCMD_OFFSET_X = 243, ///< set the sprite's offsetX to `arg`
  ANIMCMD_WAIT = 245,     ///< keep the current frame `arg` frames longer
  ANIMCMD_SOUND = 252,    ///< play sound `arg`
  ANIMCMD_JUMP = 253,     ///< continue at script position `arg`
  ANIMCMD_PLAY = 254,     ///< switch to animation `arg`
  ANIMCMD_BLANK = 255     ///< show nothing for `arg`
};

/// The field's pool sizes, and a sprite animation's timer step at normal speed.
enum {
  SPRITE_PICTURE_CACHE_COUNT = 8, ///< ActorSystemWork.pictureCaches
  ACTOR_COUNT = 64,               ///< Ram_HeapSlots->actors
  ANIM_OBJECT_COUNT = 64,         ///< sprites and sprite animations
  ANIM_LAYER_COUNT = 4,           ///< Sprite.entries
  /// Sixteenths of a frame taken off a sprite animation's timer each frame: one
  /// frame.
  SPRITE_ANIMATION_NORMAL_STEP = 16
};

/// Layered-map viewport distances in 16.16 pixels. The followed point is below
/// the vertical centre; scroll bounds and camera following share this origin.
enum {
  FIELD_VIEW_FOCUS_X = toFIXED(120), ///< followed point from the left edge
  FIELD_VIEW_FOCUS_Y = toFIXED(96),  ///< followed point from the top edge
  FIELD_VIEW_WIDTH = toFIXED(240),   ///< the screen's width
  FIELD_VIEW_HEIGHT = toFIXED(160)   ///< the screen's height
};

/// Sprite ids the field's own code creates actors of.
enum {
  FIELD_SPRITE_RIPPLE = 24, ///< the ripple behind the leader in shallow water
  FIELD_SPRITE_FOOTPRINT = 25, ///< a footprint, on maps that show them
  /// The ripple behind the leader or the ship in the world map's shallow
  /// water, rings that widen and fade (spr_269.png).
  WORLD_MAP_SPRITE_RIPPLE = 269,
  /// The puffs the ship leaves behind it as it sails or flies low, on
  /// gActor_WakeScript (spr_356.png).
  WORLD_MAP_SPRITE_WAKE = 356
};

/// How a layout's cell picture data is packed: the method byte
/// TileMap_DecodeStagedBuffer reads before the halfwords.
enum CellPicturePacking {
  CELL_PICTURE_PACKING_RAW = 0, ///< copied as it is
  /// All the high bytes, then all the low bytes; each value is XORed with
  /// the one before.
  CELL_PICTURE_PACKING_SPLIT_DELTA = 1,
  CELL_PICTURE_PACKING_DELTA = 2 ///< each value is XORed with the one before
};

/// A row of gSpritePictureCache_Sources: the packed resource that holds an
/// animation number's pictures, which SpritePictureCache_Load decodes into a
/// sprite picture cache; a number of 0 ends the table.
struct SpritePictureCacheSource {
  u16 resource;
  s16 number;
};

extern u8 Matrix_UpdateVerticesCodeSize[];

extern u8 Render_ConvertMapCode[];

extern u8 Render_ConvertMapCodeSize[];

extern u8 gMapSceneEditsOffset[];

extern struct SpritePictureCacheSource gSpritePictureCache_Sources[];

extern const u8 gActor_ShadowTiles[];

extern u8 Render_CopyStridedCode[];

extern u8 Render_CopyStridedCodeSize[];

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
