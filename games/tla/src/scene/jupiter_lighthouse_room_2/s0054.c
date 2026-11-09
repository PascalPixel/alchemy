#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
extern u8 gMainRoutineWork[];

void Func_02000054(void) {
  s32 t0;

  t0 = ((s32)Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 532)));
  if (FIELD_AT((t0 + 0x22), u8, 0) == 1) {
    Func_080dbdd4(0x900000, 0x2400000, 2, 0xea);
    Func_080dbdd4(0x1000000, 0x2300000, 2, 0xea);
  } else {
    Func_080dbdd4(0x900000, 0x2400000, 2, 0xeb);
    Func_080dbdd4(0x1000000, 0x2300000, 2, 0xeb);
  }
}
