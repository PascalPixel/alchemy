#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_GetById();
s32 Actor_SetSpritePriority();
extern u8 gSequenceParentWork[];

void Func_020029a4(void) {
  s32 t0;

  t0 = Value1(Actor_GetById, FIELD_AT(gSequenceParentWork, s32, 532));
  Call2(Actor_SetSpritePriority, 0x10,
        ((u32)(FIELD_AT(FIELD_AT(t0, s32, 80), u8, 9) << 28) >> 30));
}
