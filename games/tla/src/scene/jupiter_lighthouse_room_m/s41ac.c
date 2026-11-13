/// @file
/// Scene code of overlay 0x6a3, in ROM order.

#include "../shared/routines.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "event_inline.h"
#include "field.h"
#include "event_work.h"
#include "../shared/jupiter_lighthouse_room_m.h"

struct CellTargetEntry;

/* VIEW(s41ac): tag struct CellTarget as struct CellTarget__s41ac; cell_target.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/jupiter_lighthouse_room_m/s41ac.c:18: dereferencing pointer to incomplete type. */
struct CellTarget__s41ac {
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

DEFINE_FUNC_02001648(Func_020041ac, CellTarget__s41ac, Func_020040b4)
