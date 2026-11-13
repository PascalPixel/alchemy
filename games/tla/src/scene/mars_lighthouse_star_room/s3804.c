#include "../shared/routines.h"
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

DEFINE_FUNC_02001250(Func_02003930, gData_02004cf4, Func_02003804)

#include "scene_tables.h"

DEFINE_FUNC_020013E4(Func_02003b74)
#include "../shared/cell_with_bit.h"

/// Map_CopyCellsRect through this overlay's import.

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Func_02003f9c calls it the first time the leader steps on
/// one of the table's cells.
DEFINE_FUNC_0200146C(Func_02003bfc, Func_02003b74)

#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_3/local.h"
#include "io_reg.h"

DEFINE_FUNC_02003BE8(Func_02003c6c, Func_02003b74)
