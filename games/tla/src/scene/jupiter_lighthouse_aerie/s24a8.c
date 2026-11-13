#include "../shared/routines.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "event_inline.h"
#include "field.h"
#include "event_work.h"

struct CellTargetEntry;

/* VIEW(s24a8): tag struct CellTarget as struct CellTarget__s24a8; cell_target.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/jupiter_lighthouse_aerie/s24a8.c:18: dereferencing pointer to incomplete type. */
struct CellTarget__s24a8 {
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

void Func_020023b0(void);
#include "io_write.h"

DEFINE_FUNC_02001648(Func_020024a8, CellTarget__s24a8, Func_020023b0)
