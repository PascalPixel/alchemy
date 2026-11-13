/// @file
/// Scene code of overlay 0x6a5, in ROM order, from s4ec8's offset up to the next object the overlay links.

#include "../shared/routines.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_3/local.h"
#include "io_reg.h"
#include "cell_target.h"
#include "event_work.h"
#include "field.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/jupiter_lighthouse_aerie_beacon.h"
#include "module/scene/scene.h"

void Func_02004d9c(void);

DEFINE_FUNC_02001250(Func_02004ec8, Data_02006b74, Func_02004d9c)

DEFINE_FUNC_020013E4(Func_0200510c)
#include "../shared/cell_with_bit.h"

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Func_02005534 calls it the first time the leader steps on
/// one of the table's cells.
DEFINE_FUNC_0200146C(Func_02005194, Func_0200510c)

DEFINE_FUNC_02001DB8(Func_02005204, Func_0200510c)
