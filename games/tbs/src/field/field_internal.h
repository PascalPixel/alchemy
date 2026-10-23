#ifndef ALCHEMY_FIELD_INTERNAL_H
#define ALCHEMY_FIELD_INTERNAL_H

/// @file
/// What every part of the field object shares: its includes and the few
/// names more than one part uses. field.c includes it before the parts, and
/// each part includes it too, so that a part read on its own still resolves.
/// It places no bytes.

#include "unit/field/facing.h"
#include "unit/field/object_script.h"
#include "unit/field/operand.h"
#include "unit/field/map_grid_data.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "animspr.h"
#include "heap_slots.h"
#include "dma.h"
#include "ramcode.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "iowrite.h"
#include "heap.h"
#include "affine.h"
#include "task.h"
#include "object_dispatch.h"
#include "metadata_lookup.h"
#include "edition.h"
#include "objdisp.h"
#include "object.h"
#include "battle_system.h"
#include "transform.h"
#include "action_script.h"
#include "script_motion.h"
#include "script_object_entry.h"
#include "script_object_runtime.h"
#include "object_event.h"
#include "fixed_math.h"
#include "keystate.h"
#include "map_header.h"
#include "save_block.h"
#include "map_grid.h"
#include "map_grid_scroll.h"
#include "ram_buffer.h"
#include "resource_ids.h"
#include "flags.h"
#include "map_grid_copy.h"
#include "fixed_point_position.h"
#include "world_map_frame.h"
#include "battle_display.h"
#include "project.h"
#include "frame.h"
#include "palqueue.h"
#include "curve.h"
#include "battle_party.h"
#include "vram_cache.h"
#include "unit/field/update.h"
#include "unit/field/vertices.h"
#include "unit/field/curve_data.h"
#include "unit/field/griddata.h"
#include "objcmd.h"
#include "songs.h"
#include "macros.h"

/// Animation script commands. A script is pairs of bytes, a command and its
/// argument; a command below 239 shows that frame for `arg` frames at normal
/// speed (the timer counts sixteenths, less SpriteAnim.step each frame).
enum AnimationCommand {
  ANIMCMD_END = 239,      ///< blank the layer and stop its script
  ANIMCMD_SET_KIND = 240, ///< use facing table `arg`
  ANIMCMD_HOLD = 241,     ///< stay on this command and frame for good
  ANIMCMD_WAIT = 245,     ///< keep the current frame `arg` frames longer
  ANIMCMD_JUMP = 253,     ///< continue at script position `arg`
  ANIMCMD_PLAY = 254,     ///< switch to animation `arg`
  ANIMCMD_BLANK = 255     ///< show nothing for `arg`
};

/// The engine's pool sizes, and an anim's timer step at normal speed.
enum {
  RESOURCE_SLOT_COUNT = 8, ///< ObjectSystemWork.frameCaches
  OBJECT_COUNT = 64,       ///< gObjects, HEAP_SLOT_OBJECTS
  ANIM_OBJECT_COUNT = 64,  ///< sprites and sprite anims
  ANIM_LAYER_COUNT = 4,    ///< Sprite.entries
  /// Sixteenths of a frame taken off an anim's timer each frame: one frame.
  ANIM_NORMAL_STEP = 16
};

/// Layered-map viewport distances in 16.16 pixels. The followed point is below
/// the vertical centre; scroll bounds and camera following share this origin.
enum {
  FIELD_VIEW_FOCUS_X = 120 * Q16_ONE, ///< followed point from the left edge
  FIELD_VIEW_FOCUS_Y = 96 * Q16_ONE,  ///< followed point from the top edge
  FIELD_VIEW_WIDTH = 240 * Q16_ONE,   ///< the screen's width
  FIELD_VIEW_HEIGHT = 160 * Q16_ONE   ///< the screen's height
};

/// Sprite ids the field's own code creates objects of.
enum {
  FIELD_SPRITE_RIPPLE = 24,   ///< the ripple behind the leader in shallow water
  FIELD_SPRITE_FOOTPRINT = 25 ///< a footprint, on maps that show them
};

/// How a layout's metatile data is packed: the method byte
/// TileMap_DecodeStagedBuffer reads before the halfwords.
enum MetatilePacking {
  METATILE_PACKING_RAW = 0, ///< copied as it is
  /// All the high bytes, then all the low bytes; each value is XORed with
  /// the one before.
  METATILE_PACKING_SPLIT_DELTA = 1,
  METATILE_PACKING_DELTA = 2 ///< each value is XORed with the one before
};

/// A movement target coordinate that is not set.
#define TARGET_UNSET ((s32)0x80000000)

#endif
