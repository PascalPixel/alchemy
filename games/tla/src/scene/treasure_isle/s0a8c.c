#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02000fcc();
s32 Event_ResolvePendingActions();
s32 Actor_StartRepeatedMotion();
s32 Event_Wait();
s32 BattleEffect_ApplyColorToSourceBuffer();
s32 BattleEffect_ApplyColorToTargetBuffer();
s32 BattleEffect_StartBufferInterpolation();
s32 Func_0803ce64();
s32 Actor_GetById();
s32 Actor_ResetAndSetPosition();
s32 Func_0200101c();
extern u8 MsgIAmTheGuardian[];
extern u8 gMainRoutineWork[];

void Func_02000a8c(void) {
  s32 t12;

  Func_02000fcc();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(8, 2);
  Event_Wait(20);
  Call2(BattleEffect_ApplyColorToSourceBuffer, 0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(0x3c);
  Event_Wait(60);
  Call3(Func_0803ce64, (s32)MsgIAmTheGuardian, 0, 6);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(0x3c);
  Event_Wait(60);
  t12 = Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 532));
  if (t12 != 0) {
    Actor_ResetAndSetPosition(8, FIELD_AT(t12, s16, 10),
                              FIELD_AT(t12, s16, 18));
  }
  Event_Wait(20);
  FIELD_AT(gMainRoutineWork, u16, 596) = 0x19bb;
  FIELD_AT(gMainRoutineWork, u8, 587) = 2;
  Call2(Func_0200101c, 0x67, 0);
}
