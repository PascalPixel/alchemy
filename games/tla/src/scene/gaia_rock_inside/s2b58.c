#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_GetById();
s32 Actor_SetMoveTarget();

void Func_02002b58(s32 a0, s32 a1, s32 a2) {
  s32 t0;

  t0 = Actor_GetById();
  Call4(Actor_SetMoveTarget, t0,
        (((a1 << 4) + FIELD_AT((t0 + 0x64), s16, 0)) << 16),
        FIELD_AT(t0, s32, 12),
        (((a2 << 4) + FIELD_AT((t0 + 0x66), s16, 0)) << 16));
}
