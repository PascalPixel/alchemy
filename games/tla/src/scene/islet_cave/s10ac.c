#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "scene_ids.h"
#include "event_inline.h"
s32 Func_02001254();
s32 Func_020012d4();
s32 Func_0200125c();
extern u8 MsgSummonGuardianOfWind[];
extern u8 gMainRoutineWork[];

void Func_020010ac(void) {
  s32 t12, cue;

  Func_02001254();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(8, 2);
  Event_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(0x3c);
  Event_Wait(60);
  FieldMessage_ShowCentered((s32)MsgSummonGuardianOfWind, 0, 2);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(0x3c);
  Event_Wait(60);
  t12 = ((s32)Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 532)));
  if (t12 != 0) {
    Actor_ResetAndSetPosition(8, FIELD_AT(t12, s16, 10),
                              FIELD_AT(t12, s16, 18));
  }
  Event_Wait(20);
  Event_SetAfterBattleDestination((s32)&SceneId_Ov675Room3, 2);
  FIELD_AT(gMainRoutineWork, u8, 587) = 2;
  cue = 0x67;
  /* FAKEMATCH(barrier): the asm on the first argument loads it before the
   * second, as the native does; as Func_020012d4(0x67, 2), directly or through
   * a forwarding inline, one pair is reordered and 2 instructions differ. */
  __asm__ volatile("" : "+r"(cue));
  Func_020012d4(cue, 2);
  Func_0200125c();
}
