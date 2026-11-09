#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
extern u8 gMainRoutineWork[];

void Func_020029a4(void) {
  s32 t0;

  t0 = ((s32)Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 532)));
  Actor_SetSpritePriority(
      16, ((u32)(FIELD_AT(FIELD_AT(t0, s32, 80), u8, 9) << 28) >> 30));
}
