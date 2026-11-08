#include "../../../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"
#include "call.h"
#include "scene_tables.h"

void Func_02000d88(void);
void Func_02004264(s32 a, s32 b, s32 c);
void Func_020042dc(void);
void Func_0200432c(s32 id, s32 x, s32 z);
void Func_020043bc(s32 id, s32 a);
void Func_020043e4(s32 a, s32 b);
void Func_0200444c(s32 a);
void Func_0200446c(s32 cue);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 a, s32 b);
void Actor_SetSpeedParameters(s32 id, s32 a, s32 b);
void Party_SetFields1eeAnd1f0(s32 a, s32 b);
extern char SceneId_Ov6abRoom1;
extern char SceneId_Ov66c;

void Func_020005b0(void) {
  Func_020042dc();
  Func_0200444c(0);
  Func_02000d88();
  Func_0200446c(0);
  FieldEvent_Wait(40);
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  Func_020043bc(gMainRoutineWork.currentOwner, 0x102);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, SPLIT_CONSTANT(0xcccc));
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 0, 8);
  Actor_Jump(gMainRoutineWork.currentOwner, 4, 10);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 0, 16);
  Actor_Jump(gMainRoutineWork.currentOwner, 6, 20);
  Func_0200446c(53);
  Actor_SetModeById(11, 2);
  Actor_SetModeById(12, 2);
  FieldEvent_Wait(24);
  Call3((void (*)())Func_0200432c, 11, PIXELS(696), PIXELS(232));
  Call3((void (*)())Func_0200432c, 12, PIXELS(664), PIXELS(232));
  Actor_SetModeById(11, 3);
  Actor_SetModeById(12, 3);
  Func_0200446c(107);
  Call3((void (*)())Func_02004264, 0x20000, 0x40000, 0x10000);
  FieldEvent_Wait(20);
  Func_0200446c(0x186);
  Call3((void (*)())Func_02004264, 0x10000, 0x20000, 0x10000);
  FieldEvent_Wait(20);
  Call3((void (*)())Func_02004264, -1, -1, SPLIT_CONSTANT(0xe666));
  FieldEvent_Wait(20);
  Func_0200446c(148);
  Actor_SetModeById(11, 4);
  Actor_SetModeById(12, 4);
  FieldEvent_Wait(60);
  gMainRoutineWork.battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov6abRoom1, 20);
  gMainRoutineWork.special = (s32)&SceneId_Ov66c;
  Func_020043e4(102, 0);
}
