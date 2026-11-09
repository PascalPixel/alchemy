#include "mars_lighthouse_star_room.h"
#include "scene_tables.h"

void Func_02000d88(void);
void Func_020042dc(void);
void Func_020043e4(s32 a, s32 b);
extern char SceneId_Ov6abRoom1;
extern char SceneId_Ov66c;

void Func_020005b0(void) {
  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Func_02000d88();
  Event_PlaySound(0);
  FieldEvent_Wait(40);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= ~1;
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, 8);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 4, 10);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, 16);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 20);
  Event_PlaySound(53);
  Actor_SetAnimation(11, 2);
  Actor_SetAnimation(12, 2);
  FieldEvent_Wait(24);
  Actor_SetPosition(11, PIXELS(696), PIXELS(232));
  Actor_SetPosition(12, PIXELS(664), PIXELS(232));
  Actor_SetAnimation(11, 3);
  Actor_SetAnimation(12, 3);
  Event_PlaySound(107);
  MapGrid_SetShake(0x20000, 0x40000, 0x10000);
  FieldEvent_Wait(20);
  Event_PlaySound(0x186);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  FieldEvent_Wait(20);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldEvent_Wait(20);
  Event_PlaySound(148);
  Actor_SetAnimation(11, 4);
  Actor_SetAnimation(12, 4);
  FieldEvent_Wait(60);
  gMainRoutineWork.battleRule24b = 2;
  Event_SetAfterBattleDestination((s32)&SceneId_Ov6abRoom1, 20);
  gMainRoutineWork.special = (s32)&SceneId_Ov66c;
  Func_020043e4(102, 0);
}
