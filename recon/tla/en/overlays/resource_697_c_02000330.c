// Draft of overlay 697's Func_02000330, translated from its listing: differ 9
// of 93. Left: instruction order around
//   push {r5, r6, r7, lr}
//   + ldr r3, [pc, #216]
//   - ldr r3, [pc, #220]
//   movs r2, #133
// (- the listing, + this draft). Tried: Call, Value and direct forms of each
// call, empty asms and one-pass blocks between statements.
#include "types.h"
#include "party_state.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Event_AskYesNoWithActorAnimation();
void FieldEvent_WaitFrames();
void Func_02005868();
void Func_02005880();
void Func_02005960();
void Func_02005990();
void Func_020059e8();
void Func_02005a68();
void GameFlag_SetBit();
s32 GameFlag_Test();
void ObjectMotion_ArmCallback();
void ObjectMotion_SetPositionAndReset();
s32 Object_GetById();
void Object_SetModeById();
void UiText_ShowPositionedMessageAndWait();
extern u8 MsgTrialRoadGiveUpPrompt[];

void Func_02000330(void) {
  s32 t1, t2, t9, v_r5, v_r6;

  t1 = Value1((s32 (*)())Object_GetById, gPartyState.currentOwner);
  v_r5 = t1;
  v_r6 = FIELD_AT(0x3000000, s32, 108);
  t2 = Value1((s32 (*)())GameFlag_Test, 0x9ff);
  if (t2 == 0)
    goto L_02008400;
  Call0((void (*)())Func_02005880);
  Call1((void (*)())Func_020059e8, 0);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, gPartyState.currentOwner,
        (((FIELD_AT(v_r5, s32, 8) >> 20) << 4) + 8),
        (((FIELD_AT(v_r5, s32, 16) >> 20) << 4) + 8));
  Call1((void (*)())Func_02005a68, 229);
  Call3((void (*)())ObjectMotion_ArmCallback, gPartyState.currentOwner, 0x4000,
        0);
  v_r5 = (s32)&MsgTrialRoadGiveUpPrompt;
  Call2((void (*)())UiText_ShowPositionedMessageAndWait, v_r5, 13);
  v_r5 = (v_r5 + 1);
  FIELD_AT((v_r6 + 0xcb0), s16, 0) = v_r5;
  FIELD_AT((v_r6 + 0xcb2), s16, 0) = 4;
  FIELD_AT((v_r6 + 0xcb4), s16, 0) = 1;
  t9 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation,
              gPartyState.currentOwner, 1);
  if (t9 != 0)
    goto L_02008400;
  Call3((void (*)())ObjectMotion_ArmCallback, gPartyState.currentOwner, 0x4000,
        0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Object_SetModeById, gPartyState.currentOwner, 22);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_02005960, gPartyState.currentOwner, 0x109, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Func_02005868();
  Call1((void (*)())Func_02005990, 9);
  goto L_0200840c;
L_02008400:
  Call1((void (*)())GameFlag_SetBit, 0x281);
L_0200840c:
}
