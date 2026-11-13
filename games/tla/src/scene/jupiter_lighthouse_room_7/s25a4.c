/// @file
/// Scene code of overlay 0x6a0, in ROM order.

#include "../shared/cracked_tile.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "event_inline.h"
#include "field.h"
#include "event_work.h"
#include "../jupiter_lighthouse_room_7.h"

struct CellTargetEntry;

/* VIEW(s25a4): tag struct CellTarget as struct CellTarget__s25a4; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/jupiter_lighthouse_room_7/s25a4.c:18: dereferencing
 * pointer to incomplete type. */
struct CellTarget__s25a4 {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

#include "io_write.h"

DEFINE_CRACKED_TILE_FALL(CrackedTile_Fall, CellTarget__s25a4, Leader_CenterInCell)
