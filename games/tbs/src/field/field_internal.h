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
#include "unit/field/mapdata.h"
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
#include "global_cells.h"
#include "dma.h"
#include "ramcode.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "iowrite.h"
#include "heap_state.h"
#include "affine.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "object_dispatch.h"
#include "metadata_lookup.h"
#include "edition.h"
#include "objdisp.h"
#include "object_runtime.h"
#include "battle_system.h"
#include "transform.h"
#include "script.h"
#include "script_motion.h"
#include "script_object_entry.h"
#include "script_object_runtime.h"
#include "field_obj.h"
#include "fixed_math.h"
#include "keystate.h"
#include "field_runtime.h"
#include "game_state.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "resource_ids.h"
#include "flags.h"
#include "mapcopy.h"
#include "fixed_point_position.h"
#include "mapframe.h"
#include "battle_display.h"
#include "project.h"
#include "frame.h"
#include "palqueue.h"
#include "curve.h"
#include "battle_party.h"
#include "b5_context.h"
#include "vram_tab.h"
#include "vram_block.h"
#include "unit/field/update.h"
#include "unit/field/vertices.h"
#include "unit/field/curve_data.h"
#include "unit/field/griddata.h"
#include "objcmd.h"
#include "songs.h"
#include "macros.h"

/// Animation script commands. A script is pairs of bytes, a command and its
/// argument; a command below 239 shows that frame for `arg` frames at normal
/// speed (the timer counts sixteenths, less AnimationEntry.step each frame).
enum AnimationCommand {
  ANIMATION_COMMAND_END = 239,      ///< blank the layer and stop its script
  ANIMATION_COMMAND_SET_KIND = 240, ///< use facing table `arg`
  ANIMATION_COMMAND_HOLD = 241,     ///< stay on this command and frame for good
  ANIMATION_COMMAND_WAIT = 245, ///< keep the current frame `arg` frames longer
  ANIMATION_COMMAND_JUMP = 253, ///< continue at script position `arg`
  ANIMATION_COMMAND_PLAY = 254, ///< switch to animation `arg`
  ANIMATION_COMMAND_BLANK = 255 ///< show nothing for `arg`
};

/// The engine's pool sizes, and the layer timer's step at normal speed.
enum {
  RESOURCE_SLOT_COUNT = 8,     ///< ObjectSystemWork.slots
  FIELD_OBJECT_COUNT = 64,     ///< gObjectSlots, HEAP_SLOT_OBJECT_POOL
  ANIMATION_OBJECT_COUNT = 64, ///< sprites and layer records
  ANIMATION_LAYER_COUNT = 4,   ///< AnimationObject.entries
  /// Sixteenths of a frame taken off a layer's timer each frame: one frame.
  ANIMATION_NORMAL_STEP = 16
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

/// How a layered map's block data is packed: the method byte
/// TileMap_DecodeStagedBuffer reads before the halfwords.
enum MapBlockPacking {
  MAP_BLOCKS_RAW = 0, ///< copied as it is
  /// All the high bytes, then all the low bytes; each value is XORed with
  /// the one before.
  MAP_BLOCKS_SPLIT_DELTA = 1,
  MAP_BLOCKS_DELTA = 2 ///< each value is XORed with the one before
};

/// A movement target coordinate that is not set.
#define TARGET_UNSET ((s32)0x80000000)

#endif
