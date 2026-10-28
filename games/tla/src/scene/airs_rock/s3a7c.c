#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02002b88();
extern u8 gMainRoutineWork[];
extern u8 MsgCurePoison2[];

s32 Func_02003a7c(void) {
  if (FIELD_AT(gMainRoutineWork, s16, 480) == (s32)MsgCurePoison2) {
    Func_02002b88();
  }
  return 0;
}
