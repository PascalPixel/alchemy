#ifndef ALCHEMY_FIELD_INTERNAL_H
#define ALCHEMY_FIELD_INTERNAL_H

/// @file
/// What every part of the field object shares: its includes and the few
/// names more than one part uses. field.c includes it before the parts, and
/// each part includes it too, so that a part read on its own still resolves.
/// It places no bytes.

#include "flags.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "battle/battle_camera.h"
#include "field/actor_arm_view.h"
#include "field/actor_builtin_scripts.h"
#include "field/actor_script.h"
#include "field/actor_script_operands.h"
#include "field/actor_update_arm.h"
#include "field/curve.h"
#include "field/event.h"
#include "field/facing.h"
#include "field/field.h"
#include "field/map_grid_data.h"
#include "field/map_grid_scroll.h"
#include "field/sprite_viewer_scale.h"
#include "field/terrain_tables.h"
#include "field/world_map_affine.h"
#include "field/world_map_lines.h"
#include "graphics/graphics.h"
#include "graphics/palette_snapshots.h"
#include "menu/menu.h"
#include "system/affine.h"
#include "system/fixed_point_position.h"
#include "system/io_write.h"
#include "system/iwram_call.h"
#include "system/projection.h"
#include "system/ram_buffer.h"
#include "system/ram_code.h"
#include "system/sprite_metadata.h"
#include "system/system.h"
#include "system/transform.h"
#include "system/vram_cache.h"

/// Animation script commands.
enum AnimationCommand {
  ANIMCMD_END = 239,      ///< blank the layer and stop its script
  ANIMCMD_SET_KIND = 240, ///< use facing table `arg`
  ANIMCMD_HOLD = 241,     ///< stay on this command and frame for good
  ANIMCMD_WAIT = 245,     ///< keep the current frame `arg` frames longer
  ANIMCMD_JUMP = 253,     ///< continue at script position `arg`
  ANIMCMD_PLAY = 254,     ///< switch to animation `arg`
  ANIMCMD_BLANK = 255     ///< show nothing for `arg`
};

/// The field's pool sizes, and a sprite animation's timer step at normal speed.
enum {
  RESOURCE_SLOT_COUNT = 8, ///< ActorSystemWork.pictureCaches
  ACTOR_COUNT = 64,        ///< gActors, HEAP_SLOT_ACTORS
  ANIM_OBJECT_COUNT = 64,  ///< sprites and sprite animations
  ANIM_LAYER_COUNT = 4,    ///< Sprite.entries
  /// Sixteenths of a frame taken off a sprite animation's timer each frame: one
  /// frame.
  SPRITE_ANIMATION_NORMAL_STEP = 16
};

/// Layered-map viewport distances in 16.16 pixels. The followed point is below
/// the vertical centre; scroll bounds and camera following share this origin.
enum {
  FIELD_VIEW_FOCUS_X = px(120), ///< followed point from the left edge
  FIELD_VIEW_FOCUS_Y = px(96),  ///< followed point from the top edge
  FIELD_VIEW_WIDTH = px(240),   ///< the screen's width
  FIELD_VIEW_HEIGHT = px(160)   ///< the screen's height
};

/// Sprite ids the field's own code creates actors of.
enum {
  FIELD_SPRITE_RIPPLE = 24,   ///< the ripple behind the leader in shallow water
  FIELD_SPRITE_FOOTPRINT = 25 ///< a footprint, on maps that show them
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

/// A movement target coordinate that is not set.
#define TARGET_UNSET ((s32)0x80000000)

/// A row of gSpritePictureCache_Sources: the packed resource that holds an
/// animation number's pictures, ending at number 0.
struct SpritePictureCacheSource {
  u16 resource;
  s16 number;
};

extern struct SpritePictureCacheSource gSpritePictureCache_Sources[];

#endif
