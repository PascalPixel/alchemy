/// @file
/// Scene code of overlay 0x6ab, in ROM order.

#include "../shared/teleporter.h"
#include "event_work.h"
#include "field.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/scene.h"

#define CELL_TABLE gData_02004cf4
#define CELL_SPRITE_DRAW Func_02003804
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepth
#include "../../field/map/cell_sprite.c"

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "cell_target.h"

DEFINE_TELEPORTERS_FIND(Func_02003930, gData_02004cf4, Func_02003804)

#include "scene_tables.h"

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

/// Map_CopyCellsRect through this overlay's import.

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Teleporters_Step calls it the first time the leader steps on
/// one of the table's cells.
DEFINE_TELEPORTER_UNCOVER(Func_02003bfc, Leader_CenterInCell)

#include "../shared/jupiter_lighthouse_room_3.h"
#include "io_reg.h"

DEFINE_TELEPORTER_SINK_EVENT(Teleporter_SinkEvent, Leader_CenterInCell)
