#include "mars_lighthouse_star_room.h"
#include "call.h"

void Func_02000d88(void);
void Func_02004264(s32 a, s32 b, s32 c);
void Func_020042dc(void);
void Func_0200432c(s32 id, s32 x, s32 z);
void Func_020043bc(s32 id, s32 a);
void Func_020043e4(s32 a, s32 b);
void Func_0200444c(s32 a);
void Func_0200446c(s32 cue);
void ObjectMotion_Launch(s32 id, s32 a, s32 b);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 a, s32 b);
void ObjectMotion_SetSpeedParameters(s32 id, s32 a, s32 b);
void Party_SetFields1eeAnd1f0(s32 a, s32 b);
extern char SceneId_Ov6abRoom1;
extern char SceneId_Ov66c;

void Func_020005b0(void) {
  Func_020042dc();
  Call1((void (*)())Func_0200444c, 0);
  Func_02000d88();
  Call1((void (*)())Func_0200446c, 0);
  Call1((void (*)())FieldEvent_WaitTicks, 40);
  Object_GetById(gPartyState.currentOwner)->actionFlags &= ~1;
  Call2((void (*)())Func_020043bc, gPartyState.currentOwner, 0x102);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, gPartyState.currentOwner,
        0x19999, 0xcccc);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion,
        gPartyState.currentOwner, 0, 8);
  Call3((void (*)())ObjectMotion_Launch, gPartyState.currentOwner, 4, 10);
  Call3((void (*)())ObjectMotion_OffsetPositionAndResetMotion,
        gPartyState.currentOwner, 0, 16);
  Call3((void (*)())ObjectMotion_Launch, gPartyState.currentOwner, 6, 20);
  Call1((void (*)())Func_0200446c, 53);
  Call2((void (*)())Object_SetModeById, 11, 2);
  Call2((void (*)())Object_SetModeById, 12, 2);
  Call1((void (*)())FieldEvent_WaitTicks, 24);
  Call3((void (*)())Func_0200432c, 11, 0x2b80000, 0xe80000);
  Call3((void (*)())Func_0200432c, 12, 0x2980000, 0xe80000);
  Call2((void (*)())Object_SetModeById, 11, 3);
  Call2((void (*)())Object_SetModeById, 12, 3);
  Call1((void (*)())Func_0200446c, 107);
  Call3((void (*)())Func_02004264, 0x20000, 0x40000, 0x10000);
  Call1((void (*)())FieldEvent_WaitTicks, 20);
  Call1((void (*)())Func_0200446c, 0x186);
  Call3((void (*)())Func_02004264, 0x10000, 0x20000, 0x10000);
  Call1((void (*)())FieldEvent_WaitTicks, 20);
  Call3((void (*)())Func_02004264, -1, -1, 0xe666);
  Call1((void (*)())FieldEvent_WaitTicks, 20);
  Call1((void (*)())Func_0200446c, 148);
  Call2((void (*)())Object_SetModeById, 11, 4);
  Call2((void (*)())Object_SetModeById, 12, 4);
  Call1((void (*)())FieldEvent_WaitTicks, 60);
  gPartyState.battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov6abRoom1, 20);
  gPartyState.special = (s32)&SceneId_Ov66c;
  Call2((void (*)())Func_020043e4, 102, 0);
}
