/// @file
/// Scene code of overlay 0x6a2, in ROM order.

#include "../shared/teleporter.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "event_inline.h"
#include "field.h"
#include "event_work.h"
#include "module/scene/jupiter_lighthouse_room_9.h"

/* VIEW(s1648): tag struct CellTarget as struct CellTarget__s1648; cell_target.h has another definition; tried with the definition the rest of the object has, Func_02001648 moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s1648 {
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

DEFINE_TELEPORTER_SINK(Func_02001648, CellTarget__s1648, Func_02001550)
