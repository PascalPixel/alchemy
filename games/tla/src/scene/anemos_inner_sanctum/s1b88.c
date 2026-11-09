#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_02003398();
s32 Func_020033a0();
extern u8 MsgSummonInheritedEarthDarkness[];

void Func_02001b88(void) {
  Func_02003398();
  Event_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(0x3c);
  Event_Wait(60);
  FieldMessage_ShowCentered((s32)MsgSummonInheritedEarthDarkness, 0, 6);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(0x3c);
  Event_Wait(60);
  GameFlag_SetBit(0xa8c);
  Func_020033a0();
}
