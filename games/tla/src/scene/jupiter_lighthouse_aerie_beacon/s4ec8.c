/// @file
/// Scene code of overlay 0x6a5, in ROM order.

#include "../shared/teleporter.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "../shared/jupiter_lighthouse_room_3.h"
#include "io_reg.h"
#include "cell_target.h"
#include "event_work.h"
#include "field.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_aerie_beacon.h"
#include "module/scene/scene.h"

void Func_02004d9c(void);

DEFINE_TELEPORTERS_FIND(Teleporters_Find, Data_02006b74, Func_02004d9c)

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)
#include "../shared/cell_with_bit.h"

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Teleporters_Step calls it the first time the leader steps on
/// one of the table's cells.
DEFINE_TELEPORTER_UNCOVER(Teleporter_Uncover, Leader_CenterInCell)

DEFINE_TELEPORTER_SINK_EVENT(Teleporter_SinkEvent, Leader_CenterInCell)
