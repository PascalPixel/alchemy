#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "event_inline.h"
s32 Func_02000fcc();
s32 Func_0200101c();
extern u8 MsgIAmTheGuardian[];
extern u8 gMainRoutineWork[];

void Func_02000a8c(void) {
  s32 t12;

  Func_02000fcc();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_StartRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(0x3c);
  FieldEvent_Wait(60);
  FieldMessage_ShowCentered((s32)MsgIAmTheGuardian, 0, 6);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(0x3c);
  FieldEvent_Wait(60);
  t12 = Actor_Get(FIELD_AT(gMainRoutineWork, s32, 532));
  if (t12 != 0) {
    Actor_SetDestination(8, FIELD_AT(t12, s16, 10),
                              FIELD_AT(t12, s16, 18));
  }
  FieldEvent_Wait(20);
  FIELD_AT(gMainRoutineWork, u16, 596) = 0x19bb;
  FIELD_AT(gMainRoutineWork, u8, 587) = 2;
  Call2(Func_0200101c, 0x67, 0);
}
