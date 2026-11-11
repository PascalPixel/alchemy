#include "types.h"
#include "ram_buffer.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_020053e4();
s32 Func_020054e4();
s32 Func_0200560c();
void Func_02005734();
s32 Random16Far();
extern u8 Func_0200008c[];
extern u8 Func_02001eb0[];
extern u8 Func_02001ee4[];
extern u8 MsgWhoAreThey[];
extern u8 gData_02005f08[];
extern u8 gData_02005f50[];
extern u8 gData_02005f98[];
extern u8 gData_02005fdc[];
extern u8 gData_02006004[];
extern u8 gData_0200602c[];
extern u8 gData_02006070[];
extern u8 gData_02006b64[];
extern u8 gData_02006b6c[];
extern u8 gData_02006b70[];
extern u8 gData_02006c20[];
extern u8 gMainRoutineWork[];

static __inline__ void W_Func_02005714(s32 a0, s32 a1, s32 a2) {
  Func_080d47b4(a0, a1, a2);
}

void Func_02001f10(void) {
  s32 lastArg, spill;
  s32 owner, actor, owner2, t1;
  s32 t2, t16, t34;
  s32 t85, t86, t87;
  s32 t158, t162;
  s32 t166, t170;
  s32 t244, t248;

  actor = Actor_Get(17);
  t1 = Actor_Get(30);
  t2 = Actor_Get(31);
  spill = (Actor_Get(32));
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(17, 2);
  FIELD_AT(actor, u8, 85) = 0;
  FIELD_AT(actor, s32, 8) -= 0x40000;
  FIELD_AT(actor, s32, 16) += 0x100000;
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 4 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(28, 2);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(29, 3);
  /* FAKEMATCH(wrapper): Camera_MoveWithinMapBounds called through call.h's
   * Call4; as a direct call, 34 of 1038 instructions differ in tla-en. */
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(30, 7);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(31, 7);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(32, 6);
  owner = (s32)gMainRoutineWork;
  owner = (owner + 0x214);
  t16 = *(s32 *)owner;
  lastArg = 0xc000;
  Actor_SetPositionAndFacing(t16, 0x1f80000, 0x1400000, lastArg);
  System_Wait(1);
  /* FAKEMATCH(wrapper): BattleEffect_ApplyColorToSourceBuffer called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  PaletteFade_ApplySource(0, 0);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(1);
  Actor_SetSpriteColor((Actor_Get(30)), 15);
  Actor_SetSpriteColor((Actor_Get(31)), 15);
  Actor_SetSpriteColor((Actor_Get(32)), 15);
  System_Wait(1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(21, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(20, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(19, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(26, 1);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(25, 1);
  t34 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t34, s32, 428) = 0x204;
  FIELD_AT(t34, s32, 436) = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(0x186);
  /* FAKEMATCH(wrapper): BattleEffect_ApplyColorToTargetBuffer called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  PaletteFade_ApplyTarget(0x10003, 1);
  /* FAKEMATCH(wrapper): BattleEffect_ApplyColorToTargetBuffer called through call.h's Call2; as a
   * direct call, 12 of 1038 instructions differ in tla-en. */
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  FieldEvent_Wait(80);
  *(s32 *)gData_02006b64 = 0;
  *(s32 *)gData_02006b6c = 4;
  FIELD_AT((Actor_Get(17)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Actor_Get(27)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Actor_Get(28)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT((Actor_Get(29)), s32, 108) = (s32)Func_02001eb0;
  FIELD_AT(t1, s32, 108) = (s32)Func_02001ee4;
  FIELD_AT(t2, s32, 108) = (s32)Func_02001ee4;
  FIELD_AT((spill), s32, 108) = (s32)Func_02001ee4;
  /* FAKEMATCH(wrapper): Sound_Play called through call.h's Call1; as a
   * direct call, 31 of 1038 instructions differ in tla-en. */
  Event_PlaySound(0x253);
  System_Wait(10);
  /* FAKEMATCH(wrapper): Sound_Play called through call.h's Call1; as a
   * direct call, 27 of 1038 instructions differ in tla-en. */
  Event_PlaySound(0x253);
  System_Wait(10);
  /* FAKEMATCH(wrapper): Sound_Play called through call.h's Call1; as a
   * direct call, 23 of 1038 instructions differ in tla-en. */
  Event_PlaySound(0x253);
  System_Wait(10);
  /* FAKEMATCH(wrapper): Sound_Play called through call.h's Call1; as a
   * direct call, 15 of 1038 instructions differ in tla-en. */
  Event_PlaySound(0x253);
  System_Wait(10);
  Event_PlaySound(0x253);
  System_Wait(40);
  *(s32 *)gData_02006b6c = 3;
  System_Wait(30);
  *(s32 *)gData_02006b6c = 2;
  System_Wait(20);
  /* FAKEMATCH(wrapper): Actor_SetHorizontalPositionWithTerrain called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetPosition(17, 0, 0);
  /* FAKEMATCH(wrapper): Actor_SetHorizontalPositionWithTerrain called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetPosition(27, 0, 0);
  /* FAKEMATCH(wrapper): Actor_SetHorizontalPositionWithTerrain called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetPosition(28, 0, 0);
  Actor_SetPosition(29, 0, 0);
  FIELD_AT((Actor_Get(17)), s32, 108) = 0;
  FIELD_AT((Actor_Get(27)), s32, 108) = 0;
  FIELD_AT((Actor_Get(28)), s32, 108) = 0;
  FIELD_AT((Actor_Get(29)), s32, 108) = 0;
  FIELD_AT(t1, s32, 108) = 0;
  FIELD_AT(t2, s32, 108) = 0;
  FIELD_AT((spill), s32, 108) = 0;
  System_Wait(1);
  Event_PlaySound(103);
  Actor_SetSpriteColor((Actor_Get(30)), 9);
  Actor_SetSpriteColor((Actor_Get(31)), 9);
  Actor_SetSpriteColor((Actor_Get(32)), 9);
  FieldEvent_Wait(20);
  Actor_SetSpriteColor((Actor_Get(30)), 0);
  Actor_SetSpriteColor((Actor_Get(31)), 0);
  Actor_SetSpriteColor((Actor_Get(32)), 0);
  FieldEvent_Wait(20);
  t85 = Random16Far();
  FIELD_AT(t1, u8, 98) = (((u32)(((t85 << 2) + t85) << 4) >> 16) + 40);
  t86 = Random16Far();
  FIELD_AT(t2, u8, 98) = (((u32)(((t86 << 2) + t86) << 4) >> 16) + 40);
  t87 = Random16Far();
  FIELD_AT((spill), u8, 98) = (((u32)(((t87 << 2) + t87) << 4) >> 16) + 40);
  FIELD_AT(t1, s32, 108) = (s32)Func_0200008c;
  FIELD_AT(t2, s32, 108) = (s32)Func_0200008c;
  FIELD_AT((spill), s32, 108) = (s32)Func_0200008c;
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  FieldEvent_SetMessageId((s32)MsgWhoAreThey);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 24 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(26, 0x8000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(26, 0);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(19, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 25 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(22, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 26 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection((*(s32 *)owner), 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 30 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(23, 0x4000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 15 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(24, 0x4000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 23 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(21, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 24 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(20, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 10 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(26, 0x4000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(25, 0x4000, 0);
  /* FAKEMATCH(wrapper): Camera_SetSpeedParameters called through call.h's Call2; as a
   * direct call, 59 of 1038 instructions differ in tla-en. */
  FieldCamera_SetSpeed(0x19999, 0x3333);
  FIELD_AT((FieldCamera_GetActor()), u8, 85) = 0;
  FieldCamera_MoveTo(0x2000000, 0x500000, 0x1540000, 1);
  Func_02005734();
  Actor_ShowEmote(19, 0x109, 20);
  Event_ShowMessage(19, 0);
  Event_PlaySound(8);
  /* FAKEMATCH(wrapper): Func_080d489c called through call.h's Call2; as a
   * direct call, 8 of 1038 instructions differ in tla-en. */
  Actor_SetAttachedEffect(24, 0x102);
  FieldEvent_Wait(20);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(24, 0);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(25, 0);
  /* FAKEMATCH(wrapper): Motion_SetModeAndWaitAnimation called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimationAndWait(19, 3);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(19, 0);
  /* FAKEMATCH(wrapper): Func_080d489c called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAttachedEffect(26, 0x102);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x101a, 0);
  /* FAKEMATCH(wrapper): Actor_SetModeById called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimation(19, 4);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(19, 0);
  Actor_ShowEmote(23, 0x103, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 51 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(23, 0x19999, 0xcccc);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(23, 0x200, 0x148);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(23, 0x2000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x1017, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(20, 0x8000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0xa014, 0);
  Actor_ShowEmote(19, 0x100, 20);
  FieldActor_SetSpritePriority(21, 2);
  FIELD_AT((Actor_Get(20)), u8, 98) = 0;
  FIELD_AT((Actor_Get(21)), u8, 98) = 0;
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 38 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(21, 0xcccc, 0x6666);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(20, 0xcccc, 0x6666);
  Actor_SetScript(21, (s32)gData_02005f08);
  Actor_SetScript(20, (s32)gData_02005f50);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 4 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(19, 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(19, 0x21c, 0x154);
  /* FAKEMATCH(wrapper): Actor_Jump called through call.h's Call3; as a direct
   * call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_Jump(19, 4, 10);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(19, 0);
  actor = Actor_Get(21);
L_0200a47a:
  System_Wait(1);
  if (FIELD_AT(actor, u8, 98) == 0)
    goto L_0200a47a;
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(20, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(21, 0x2000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(21, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(20, 2);
  /* FAKEMATCH(wrapper): Actor_SetSpritePriority called through call.h's Call2;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_SetSpritePriority(22, 2);
  /* FAKEMATCH(wrapper): this call through W_Func_02005714; as Actor_ShowEmote(20,
   * 0x100, 0), 103 of 1038 instructions differ in tla-en (as Call3, 2). */
  W_Func_02005714(20, 0x100, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(20, 0x19999, 0xcccc);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(20, 0x1ea, 0x128);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(21, 0xe000, 20);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 139 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(20, 0xa000, 20);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(20, 0xe000, 20);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 27 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(20, 0x3333, 0x1999);
  t158 = Actor_Get(20);
  FIELD_AT(t158, u8, 90) &= 254;
  Actor_WalkToAndWait(20, 0x1e8, 0x12c);
  FieldEvent_Wait(1);
  t162 = Actor_Get(20);
  FIELD_AT(t162, u8, 90) |= 1;
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(20, 2);
  t166 = Actor_Get(20);
  FIELD_AT(t166, u8, 90) &= 254;
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 53 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(20, 0x1e6, 0x134);
  FieldEvent_Wait(1);
  t170 = Actor_Get(20);
  FIELD_AT(t170, u8, 90) |= 1;
  /* FAKEMATCH(wrapper): Camera_SetSpeedParameters called through call.h's Call2; as a
   * direct call, 57 of 1038 instructions differ in tla-en. */
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(0x2000000, 0x600000, 0x1400000, 1);
  Func_02005734();
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(21, 0, 40);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(21, 0xe000, 20);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(21, 0);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x8015, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 128 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(22, 0xc000, 0);
  owner2 = (s32)gMainRoutineWork;
  owner2 = (owner2 + 0x214);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 118 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection((*(s32 *)owner2), 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 111 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(23, 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 100 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(24, 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 107 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(26, 0xa000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 88 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(25, 0xa000, 0);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 100 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x8014, 0);
  actor = Actor_Get(32);
  FIELD_AT(actor, s32, 108) = 0;
  System_Wait(1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  /* FAKEMATCH(wrapper): Motion_SetVarCbAndRefresh called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_RunRepeatedMotion(32, 2);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(32, 0);
  Actor_ShowEmote(23, 0x100, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 55 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(23, 0xe000, 40);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(25, 0x8000, 0);
  Actor_ShowEmote(25, 0x101, 20);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(25, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(23, 0, 40);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 179 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(23, 0xc000);
  /* FAKEMATCH(wrapper): Actor_StartRepeatedMotion called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_StartRepeatedMotion(32, 3);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(32, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 162 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(24, 0xc000, 40);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(24, 0x6000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x4018, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 91 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(21, 0x2000);
  /* FAKEMATCH(wrapper): Func_080d489c called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAttachedEffect(21, 0x102);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(21, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 159 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection((*(s32 *)owner2), 0xc000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 160 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(24, 0xc000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 155 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(26, 0xc000, 0);
  Actor_FaceDirection(25, 0xc000, 0);
  actor = Actor_Get(30);
  FIELD_AT(actor, s32, 108) = 0;
  System_Wait(1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  actor = Actor_Get(31);
  FIELD_AT(actor, s32, 108) = 0;
  System_Wait(1);
  FIELD_AT(actor, s32, 24) = 0x10000;
  FIELD_AT(actor, s32, 28) = 0x10000;
  FieldEvent_Wait(10);
  /* FAKEMATCH(wrapper): Motion_SetVarCbAndRefresh called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_RunRepeatedMotion(31, 1);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x201f, 0);
  /* FAKEMATCH(wrapper): Motion_SetVarCbAndRefresh called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_RunRepeatedMotion(30, 2);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(30, 0);
  /* FAKEMATCH(wrapper): Actor_Jump called through call.h's Call3; as a direct
   * call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_Jump(20, 2, 10);
  /* FAKEMATCH(wrapper): Actor_Jump called through call.h's Call3; as a direct
   * call, 2 of 1038 instructions differ in tla-en. */
  FieldActor_Jump(20, 4, 10);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x8014, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 85 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(20, 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 85 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed((*(s32 *)owner2), 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 83 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(23, 0x26666, 0x13333);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 31 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(24, 0x26666, 0x13333);
  Actor_SetScript(20, (s32)gData_02005f98);
  Actor_SetScript((*(s32 *)owner2), (s32)gData_02005fdc);
  Actor_SetScript(23, (s32)gData_02006004);
  Actor_RunScriptAndWait(24, (s32)gData_0200602c);
  FieldEvent_Wait(20);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 18 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(22, 0x19999, 0xcccc);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(22, 0x210, 0x140);
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(22, 0x2000, 0);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 10 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(25, 0xa000);
  /* FAKEMATCH(wrapper): Event_ShowMessageAndWait called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Event_ShowMessage(0x5016, 0);
  /* FAKEMATCH(wrapper): Actor_SetSpeedParameters called through call.h's Call3;
   * as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_SetScript(25, (s32)gData_02006070);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(22, 0x1e6, 0x134);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(22, 0xa000);
  Actor_SetSpeed(21, 0x26666, 0x13333);
  t244 = Actor_Get(21);
  FIELD_AT(t244, u8, 90) &= 254;
  Actor_WalkToAndWait(21, 0x1da, 0x13c);
  FieldEvent_Wait(1);
  t248 = Actor_Get(21);
  FIELD_AT(t248, u8, 90) |= 1;
  /* FAKEMATCH(wrapper): Actor_ArmCallback called through call.h's Call3; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirection(21, 0xe000, 0);
  /* FAKEMATCH(wrapper): Actor_SetPositionAndReset called through call.h's
   * Call3; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_WalkToAndWait(22, 0x1ce, 0x118);
  /* FAKEMATCH(wrapper): Actor_ArmCallbackAndRefresh called through call.h's Call2; as a
   * direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_FaceDirectionAndWait(22, 0);
  FieldActor_SetSpritePriority(22, 1);
  Func_020053e4((Actor_Get(22)), (s32)gData_02006c20);
  Func_020053e4((Actor_Get(25)), (s32)gData_02006b70);
  FieldEvent_Wait(40);
  /* FAKEMATCH(wrapper): Motion_SetModeAndWaitAnimation called through call.h's
   * Call2; as a direct call, 2 of 1038 instructions differ in tla-en. */
  Actor_SetAnimationAndWait(19, 4);
  Event_ShowMessage(19, 0);
  Event_PlaySound(78);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(120);
  System_Wait(120);
  Func_020054e4(Actor_Get(22));
  Func_020054e4(Actor_Get(25));
  FieldEvent_Wait(80);
  FieldEvent_SetExitRequest(2);
  return;
}
