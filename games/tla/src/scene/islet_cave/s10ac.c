#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "scene_ids.h"
s32 Func_02001254();
s32 Func_02001324();
s32 Actor_StartRepeatedMotion();
s32 FieldEvent_Wait();
s32 Func_020012f4();
s32 Func_020012ec();
s32 Func_020012fc();
s32 Func_02001244();
s32 Actor_GetById();
s32 Actor_ResetAndSetPosition();
s32 Party_SetFields1eeAnd1f0();
s32 Func_020012d4();
s32 Func_0200125c();
extern u8 MsgSummonGuardianOfWind[];
extern u8 gAdventure[];

void Func_020010ac(void) {
  s32 t12, cue;

  Func_02001254();
  Call1(Func_02001324, 0);
  Call2(Actor_StartRepeatedMotion, 8, 2);
  Call1(FieldEvent_Wait, 0x14);
  Call2(Func_020012f4, 0x10000, 0);
  Call2(Func_020012ec, 0x10003, 1);
  Call1(Func_020012fc, 0x3c);
  Call1(FieldEvent_Wait, 0x3c);
  Call3(Func_02001244, (s32)MsgSummonGuardianOfWind, 0, 2);
  Call2(Func_020012ec, 0x10000, 0);
  Call1(Func_020012fc, 0x3c);
  Call1(FieldEvent_Wait, 0x3c);
  t12 = Value1(Actor_GetById, FIELD_AT(gAdventure, s32, 532));
  if (t12 != 0) {
    Call3(Actor_ResetAndSetPosition, 8, FIELD_AT(t12, s16, 10),
          FIELD_AT(t12, s16, 18));
  }
  Call1(FieldEvent_Wait, 0x14);
  Call2(Party_SetFields1eeAnd1f0, (s32)&SceneId_Ov675Room3, 2);
  FIELD_AT(gAdventure, u8, 587) = 2;
  cue = 0x67;
  /* FAKEMATCH: the native loads the first argument before the second; a direct
   * call and the forwarding inline load the second first (one reordered pair,
   * differ 2). */
  __asm__ volatile("" : "+r"(cue));
  Func_020012d4(cue, 2);
  Func_0200125c();
}
