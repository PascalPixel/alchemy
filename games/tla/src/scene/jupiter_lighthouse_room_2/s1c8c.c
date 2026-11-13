/// @file
/// Scene code of overlay 0x69b, in ROM order.

#include "../shared/teleporter.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "event_inline.h"
#include "event_work.h"
#include "field.h"
#include "module/scene/jupiter_lighthouse_room_2.h"

struct CellTargetEntry;

/* VIEW(s1c8c): tag struct CellTarget as struct CellTarget__s1c8c; cell_target.h has another definition; tried with the definition the rest of the object has, Func_02001c8c moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s1c8c {
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

DEFINE_TELEPORTER_SINK(Func_02001c8c, CellTarget__s1c8c, Func_02001b94)
