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
#include "graphics/vram_tab.h"
#include <system/affine.h>
#include "system/sprite_metadata.h"
#include "field/actor.h"
#include "field/actor_view.h"
#include "battle/battle.h"
#include "field/field.h"
#include "field/field_state.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/ram_code.h"
#include "field/field_sequence.h"
#include "sprite_ids.h"

/// How far off its heading the leader's pad movement probes for a clear way:
/// a sixteenth of a turn, 22.5 degrees, and three times that.
#define FIELD_PROBE_ANGLE (TRIG_TURN / 16)
/// The most a blocked actor turns toward its heading in one frame.
#define FIELD_BLOCKED_TURN_MAX (TRIG_TURN / 16)
/// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + toFIXED(8)) >> 20) == 0)

struct Actor;
struct Point;

/// The collision at an actor's probe point (tile_animation.c): 2 for a cell
/// with no tile, 1 a rise of more than 8 pixels, -1 a drop of more than 12,
/// else 0.
s32 MapGrid_CheckCollision(struct Actor *actor, struct Point *position);
s32 MapGrid_CheckCollisionAbove(struct Actor *actor, struct Point *position);

/// The map converter copied to RAM (Render_ConvertMapCode), which joins a
/// decoded map's planes into its cell words.
typedef void (*ConvertMapRoutine)(void *destination, void *source, void *saved);

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

/// The start of the 3D camera's state (heap slot cameraState), as the fixed
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
  u8 unusedAfterShape[3]; ///< no code names it
  u8 tileHigh : 2;        ///< tile number bits 9..8
  u8 priority : 2;
  u8 palette : 4;
  u8 unusedAfterPalette[7]; ///< no code names it
  u8 cacheFlags : 2;        ///< retainCacheEntry and useOwnScale
  u8 sunkRows : 6; ///< rows SPRITE_FLAG_SUNK sinks or SPRITE_FLAG_SHADED shades
  u16 rotation;
  u8 unusedAfterRotation[5]; ///< no code names it
  u8 dirty;
  u8 flags;
  u8 unusedAfterFlags[17];             ///< no code names it
  struct SpriteAnimation *secondLayer; ///< entries[1]; above priority 3 hidden
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

/// Where a layout's tileset animation script is decoded, in the map collision
/// buffer after the collision, as The Broken Seal's; its blend script follows
/// at MAPGRID_BLEND_SCRIPT (field/map_grid.h).
#define MAPGRID_ANIMATION_SCRIPT (Ram_MapCollision + 4096)

/// A layout's header: the camera's bounds, the layers' priorities, tile
/// bases and setup, and the offsets, from the header's start, of its packed
/// cell tiles, collision, cells, shapes, tileset animation, blend script
/// and script.
struct MapHeader {
  /// The camera's bounds in 8-pixel tiles: left, top, right and bottom.
  u8 cameraBounds[4];
  /// Each layer's BG priority, 1 to 3; 0 leaves the layer off.
  u8 priority[3];
  u8 charBase[3]; ///< each layer's BG char block
  u16 reserved;   ///< nothing reads it
  struct MapHeaderLayer layers[3];
  s32 cellTiles; ///< staged and unpacked into the cell tiles buffer
  s32 collision; ///< into Ram_MapCollision
  s32 cells;     ///< into Ram_MapCellBuffer
  s32 shapes;    ///< into gMapShapeGrid
  s32 animation; ///< the tileset animation script, or 0 for none
  s32 blend;     ///< the blend script, or 0 for none
  /// Into Ram_MapCollision after its MAPGRID_SHAPE_TABLE_BYTES.
  s32 script;
};

/// A map layer's scroll position in whole 16-pixel cells.
struct MapGridTilePosition {
  s32 x, y;
};

/// One step of a map edit: the rectangle of cells to copy from and the
/// wait after it.
struct MapGridCellAnimationStep {
  /// The source rectangle's x in cells, or MAPGRID_ANIMATION_STEPS_END.
  u16 source;
  /// The source rectangle's y, width and height in cells, and the frames to
  /// wait after the copy.
  s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));

/// A sprite (struct Sprite) whose image priority, its layer, is copied a
/// byte at a time, under struct Sprite's names: a wading splash's.
struct SpriteLayerView {
  u8 unused[9];    ///< no code names it
  u8 tileHigh : 2; ///< tile number bits 9..8
  u8 layer : 2;
  u8 palette : 4;
  u8 unusedAfterPalette[16]; ///< no code names it
  u8 flags;
};

/// The actor as SC_SYNC clears it when it gives up waiting: its target, arrival
/// axis and blocked count. Read through struct Actor,
/// ActorScript_WaitForArrival compiles otherwise.
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

/// SpriteAnimation.kind: the facing table a sprite animation's pictures follow
/// (Sprite_ComposePicture), as The Broken Seal names them; this game's five
/// pictures cover 64 sectors.
enum AnimationFacingKind {
  ANIM_FACING_NONE = 0,        ///< the script's own picture whatever the facing
  ANIM_FACING_ONE_PICTURE = 1, ///< 8 sectors: one picture, mirrored to the left
  ANIM_FACING_FRONT_BACK = 2,  ///< 8 sectors: a front and a back picture
  ANIM_FACING_THREE_PICTURES = 3, ///< 16 sectors: front, side and back
  /// Unused, with no facing table: drawn as ANIM_FACING_NONE. The sprite
  /// viewer rings it ten round, as The Broken Seal's does its four pictures.
  ANIM_FACING_FOUR_PICTURES = 4,
  ANIM_FACING_FIVE_PICTURES = 5, ///< 64 sectors here, 16 in The Broken Seal
  /// Unused, with no facing table: drawn as ANIM_FACING_NONE. The sprite
  /// viewer still rings it ten round.
  ANIM_FACING_SIX_PICTURES = 6,
  /// 8 sectors centred on the compass directions, a picture for each.
  ANIM_FACING_EIGHT_PICTURES = 8,
  /// ANIM_FACING_FRONT_BACK's table for a sprite drawn in two parts, the
  /// second the next sprite id (the sprite viewer).
  ANIM_FACING_TWO_PART = 20,
  /// As ANIM_FACING_FRONT_BACK, with the front picture mirrored the other way.
  ANIM_FACING_FRONT_BACK_SWAPPED = 22,
  /// Unused, with no facing table: rung as eight facings by the sprite viewer,
  /// as The Broken Seal's is.
  ANIM_FACING_EIGHT_NO_TABLE = 44,
  /// Unused, with no facing table: The Broken Seal's eight pictures with three
  /// mirrored, rung as eight facings by the sprite viewer.
  ANIM_FACING_EIGHT_PICTURES_MIRRORED = 88
};

/// A facing table entry's offset of its picture from the base picture.
#define ANIM_FACING_PICTURE_MASK 7
/// A facing table entry's bit that mirrors its picture.
#define ANIM_FACING_MIRRORED (1 << 7)
/// The sector a facing angle lies in, a turn cut into `sectors`.
#define FACING_SECTOR(dir, sectors)                                            \
  ((dir) / (TRIG_TURN / (sectors)) % (sectors))

/// The field's pool sizes, and a sprite animation's timer step at normal speed.
enum {
  SPRITE_PICTURE_CACHE_COUNT = 8, ///< ActorSystemState.pictureCaches
  SPRITE_ANIMATION_COUNT = 4,     ///< Sprite.entries
  RUNNING_LIST_SLOTS = 14,        ///< the actor slots Actor_ListRunning reads
  /// Sixteenths of a frame taken off a sprite animation's timer each frame: one
  /// frame.
  SPRITE_ANIMATION_NORMAL_STEP = 16
};

/// What stopped the leader's step this frame (Actor_UpdatePlayerControl,
/// Actor_MoveOnWorldMap, Actor_UpdateHoverControl and the climbing and rope
/// controls).
enum PlayerStepBlock {
  PLAYER_STEP_BLOCKED_BY_MAP = 1, ///< every heading tried runs into the map
  /// A solid actor ahead, or one with no room to be pushed.
  PLAYER_STEP_BLOCKED_BY_ACTOR = 2,
  PLAYER_STEP_BLOCKED = 3, ///< either of the two
  /// No step, and none counted as blocked: the pad is not held, or a climb
  /// meets a wall.
  PLAYER_STEP_STOPPED = 4
};

/// Layered-map viewport distances in 16.16 pixels. The followed point is below
/// the vertical centre; scroll bounds and camera following share this origin.
enum {
  FIELD_VIEW_FOCUS_X = toFIXED(120), ///< followed point from the left edge
  FIELD_VIEW_FOCUS_Y = toFIXED(96),  ///< followed point from the top edge
  FIELD_VIEW_WIDTH = toFIXED(240),   ///< the screen's width
  FIELD_VIEW_HEIGHT = toFIXED(160)   ///< the screen's height
};

/// How a layout's cell tiles data is packed: the method byte
/// TileMap_DecodeStagedBuffer reads before the halfwords.
enum CellTilesPacking {
  CELL_TILES_PACKING_RAW = 0, ///< copied as it is
  /// All the high bytes, then all the low bytes; each value is XORed with
  /// the one before.
  CELL_TILES_PACKING_SPLIT_DELTA = 1,
  CELL_TILES_PACKING_DELTA = 2 ///< each value is XORed with the one before
};

/// A row of gSpritePictureCache_Sources: the packed resource that holds an
/// animation number's pictures, which SpritePictureCache_Load decodes into a
/// sprite picture cache; a number of 0 ends the table.
struct SpritePictureCacheSource {
  u16 resource;
  s16 number;
};

/// The size of WorldMap_UpdateAffineLinesCode, an address equal to the size
/// (ld_script.ld).
extern u8 WorldMap_UpdateAffineLinesCodeSize[];

/// The map converter's Thumb entry and ARM code (map_grid_convert.s), copied to
/// RAM to run.
extern u8 Render_ConvertMapCode[];

/// The size of Render_ConvertMapCode (map_grid_convert.s).
extern u8 Render_ConvertMapCodeSize[];

/// The scene data's halfword offset of its edits, from gMapScene.
extern u8 gMapSceneEditsOffset[];

/// Each animation number's packed pictures, ending at number 0 (animdata.s).
extern struct SpritePictureCacheSource gSpritePictureCache_Sources[];

/// The sprites' shadow, 16 by 8 at 256 colours, loaded into VRAM cache entry
/// VRAM_CACHE_ENTRY_ACTOR_SHADOW (animdata.s).
extern const u8 gActor_ShadowTiles[];

/// The strided copy's Thumb entry and ARM code (copy_strided.s), copied to RAM
/// to run.
extern u8 Render_CopyStridedCode[];

/// The size of Render_CopyStridedCode (copy_strided.s).
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

// field_view.c's routines, which field_tile_animation.c and field_world_map.c
// call before it.
void TileMap_DrawRowsFromRam(const struct WorldMapAffinePair *lines,
                             u8 *pixels);
void WorldMap_BuildScanlineTable(FIXED pitchSlope, s32 *position,
                                 struct WorldMapScanline *scanline);

/// The footprints the leader leaves in snow and sand, and the leader's
/// userValue that asks for the pair next. Every routine that moves the leader
/// by the pad sets it on a stop.
enum FootprintAnimation {
  FOOTPRINT_ONE_FOOT = 1, ///< one print, fading
  /// Two prints side by side, fading: laid first after a stop.
  FOOTPRINT_BOTH_FEET = 2
};

#endif // ALCHEMY_FIELD_INTERNAL_H
