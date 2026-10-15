#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Object_GetById();
extern u8 gPartyState[];

void Func_02001f68(void) {
  s32 t0;

  t0 = Value1(Object_GetById, FIELD_AT(gPartyState, s32, 532));
  FIELD_AT((t0 + 0x22), u8, 0) = 2;
}
