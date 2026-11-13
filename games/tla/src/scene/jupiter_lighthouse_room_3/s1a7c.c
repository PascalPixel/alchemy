/// @file
/// Scene code of overlay 0x69c, in ROM order.

#include "../shared/teleporter.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "../shared/jupiter_lighthouse_room_3.h"
#include "io_reg.h"
#include "cell_target.h"
#include "event_work.h"
#include "field.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_room_3.h"
#include "module/scene/scene.h"

DEFINE_TELEPORTERS_FIND_LOWER(Teleporters_FindLower, gTeleporters, Teleporters_Update)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Teleporters_Step calls it the first time the leader steps on
/// one of the table's cells.
DEFINE_TELEPORTER_UNCOVER(Teleporter_Uncover, Leader_CenterInCell)

DEFINE_TELEPORTER_SINK_EVENT(Teleporter_SinkEvent, Leader_CenterInCell)
