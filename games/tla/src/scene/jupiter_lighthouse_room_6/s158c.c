#include "../shared/routines.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "event_inline.h"
#include "field.h"
#include "event_work.h"
#include "module/scene/jupiter_lighthouse_room_6.h"

struct CellTargetEntry;

/* VIEW(s158c): tag struct CellTarget as struct CellTarget__s158c; cell_target.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/jupiter_lighthouse_room_6/s158c.c:18: dereferencing pointer to incomplete type. */
struct CellTarget__s158c {
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

DEFINE_FUNC_0200156C(Func_0200158c, CellTarget__s158c, Func_02001494)
