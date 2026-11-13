/// @file
/// Scene code of overlay 0x69c, in ROM order.

#include "../shared/routines.h"
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

DEFINE_FUNC_020011A0(Func_02001a7c, gData_02002e34, Func_02001950)

DEFINE_FUNC_020013E4(Func_02001cc0)
#include "../shared/cell_with_bit.h"

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Func_020020e8 calls it the first time the leader steps on
/// one of the table's cells.
DEFINE_FUNC_0200146C(Func_02001d48, Func_02001cc0)

DEFINE_FUNC_02001DB8(Func_02001db8, Func_02001cc0)
