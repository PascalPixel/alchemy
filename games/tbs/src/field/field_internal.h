#ifndef ALCHEMY_FIELD_INTERNAL_H
#define ALCHEMY_FIELD_INTERNAL_H

// What every part of the field object shares: its includes and the few
// names more than one part uses. field.c includes it before the parts, and
// each part includes it too, so that a part read on its own still resolves.
// It places no bytes.

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

// Animation script commands. A script is pairs of bytes, a command and its
// argument; a command below 239 shows that frame for `arg` frames at normal
// speed (the timer counts sixteenths, less AnimationEntry.step each frame).
enum AnimationCommand {
  ANIM_CMD_END = 239,      // blank the layer and stop its script
  ANIM_CMD_SET_KIND = 240, // use facing table `arg`
  ANIM_CMD_HOLD = 241,     // stay on this command and frame for good
  ANIM_CMD_WAIT = 245,     // keep the current frame `arg` frames longer
  ANIM_CMD_JUMP = 253,     // continue at script position `arg`
  ANIM_CMD_PLAY = 254,     // switch to animation `arg`
  ANIM_CMD_BLANK = 255     // show nothing for `arg`
};

enum {
  RESOURCE_SLOT_COUNT = 8,
  ANIMATION_OBJECT_COUNT = 64,
  ANIMATION_LAYER_COUNT = 4,
  ANIMATION_NORMAL_STEP = 16
};

// A movement target coordinate that is not set.
#define TARGET_UNSET ((s32)0x80000000)

#endif
