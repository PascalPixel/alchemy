#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_SetSpritePriority();

void Func_02000674(s32 a0, s32 a1) {
  Call2(Actor_SetSpritePriority, a1, 2);
}
