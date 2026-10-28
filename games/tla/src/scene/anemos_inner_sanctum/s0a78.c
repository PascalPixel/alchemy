#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Import_ActorCreate();
s32 Actor_SetPartAttribute();
s32 Scene_SetActorSpritePriority();
s32 Func_020032e0();
s32 Actor_SetSingleChildFlags();

void Func_02000a78(s32 a0, s32 a1, s16 a2) {
  s32 t0;

  t0 = Value4(Import_ActorCreate, 0x2a0, (a0 << 16), ((a1 + 3) << 16),
              0x980000);
  FIELD_AT((t0 + 0x55), u8, 0) = 4;
  Call2(Actor_SetPartAttribute, t0, (u16)a2);
  Call2(Scene_SetActorSpritePriority, t0, 2);
  Call2(Func_020032e0, t0, 7);
  Call2(Actor_SetSingleChildFlags, t0, 0);
}
