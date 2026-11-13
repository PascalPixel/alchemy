/// @file
/// Scene code of overlay 0x69d, in ROM order.

#include "../shared/routines.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "event_inline.h"
#include "field.h"
#include "event_work.h"
#include "module/scene/jupiter_lighthouse_room_4.h"

struct CellTargetEntry;

/* VIEW(s1888): tag struct CellTarget as struct CellTarget__s1888; cell_target.h has another definition; tried with the definition the rest of the object has, Func_02001888 moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s1888 {
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

DEFINE_FUNC_0200156C(Func_02001888, CellTarget__s1888, Func_02001790)
