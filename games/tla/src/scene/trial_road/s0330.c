#include "types.h"
#include "adventure.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Event_AskYesNoWithActorAnimation();
void FieldEvent_Wait();
void Func_02005868();
void Func_02005880();
void Func_02005960();
void Func_02005990();
void Func_020059e8();
void Func_02005a68();
void GameFlag_SetBit();
s32 GameFlag_Test();
void Actor_ArmCallback();
void Actor_SetPositionAndReset();
s32 Actor_GetById();
void Actor_SetModeById();
void FieldMessage_Show();
extern u8 MsgTrialRoadGiveUpPrompt[];

s32 Func_02000330(void) {
  s32 t1, t2, t9, v_r5, v_r6;

  t1 = Value1((s32 (*)())Actor_GetById, gAdventure.currentOwner);
  v_r5 = t1;
  v_r6 = FIELD_AT(0x3000000, s32, 108);
  t2 = Value1((s32 (*)())GameFlag_Test, 0x9ff);
  if (t2 == 0)
    goto L_02008400;
  Func_02005880();
  Call1((void (*)())Func_020059e8, 0);
  Call3((void (*)())Actor_SetPositionAndReset, gAdventure.currentOwner,
        (((FIELD_AT(v_r5, s32, 8) >> 20) << 4) + 8),
        (((FIELD_AT(v_r5, s32, 16) >> 20) << 4) + 8));
  Call1((void (*)())Func_02005a68, 229);
  Call3((void (*)())Actor_ArmCallback, gAdventure.currentOwner, 0x4000,
        0);
  v_r5 = (s32)&MsgTrialRoadGiveUpPrompt;
  Call2((void (*)())FieldMessage_Show, v_r5, 13);
  v_r5 = (v_r5 + 1);
  FIELD_AT((v_r6 + 0xcb0), s16, 0) = v_r5;
  FIELD_AT((v_r6 + 0xcb2), s16, 0) = 4;
  FIELD_AT((v_r6 + 0xcb4), s16, 0) = 1;
  t9 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation,
              gAdventure.currentOwner, 1);
  if (t9 != 0)
    goto L_02008400;
  Call3((void (*)())Actor_ArmCallback, gAdventure.currentOwner, 0x4000,
        0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Actor_SetModeById, gAdventure.currentOwner, 22);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02005960, gAdventure.currentOwner, 0x109, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Func_02005868();
  Call1((void (*)())Func_02005990, 9);
  return 1;
L_02008400:
  Call1((void (*)())GameFlag_SetBit, 0x281);
  return 0;
}
