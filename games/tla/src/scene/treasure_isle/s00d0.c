#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Import_ActorCreate();
s32 Scene_SetActorSpritePriority();
s32 Actor_SetSingleChildFlags();
s32 Actor_SetPartAttribute();

s32 Func_020000d0(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 t0;

  t0 = Value4(Import_ActorCreate, a3, a0, a1, a2);
  if (t0 != 0) {
    Call2(Scene_SetActorSpritePriority, t0, 1);
    FIELD_AT((t0 + 0x55), u8, 0) = 0;
    FIELD_AT(((t0 + 0x55) + 4), u8, 0) = 8;
    Call2(Actor_SetSingleChildFlags, t0, 0);
    Call2(Actor_SetPartAttribute, t0, 0xf);
    FIELD_AT((t0 + 0x23), u8, 0) = (0x22 | FIELD_AT((t0 + 0x23), u8, 0));
    return t0;
  }
  return 0;
}
