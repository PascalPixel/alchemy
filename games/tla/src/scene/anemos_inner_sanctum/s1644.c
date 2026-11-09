#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
extern u8 gMainRoutineWork[];

void Func_02001644(void) {
  s32 t0;

  t0 = ((s32)Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 532)));
  FIELD_AT((t0 + 0x22), u8, 0) = 0;
}
