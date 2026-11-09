#include "local.h"

void Map_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);

void Func_02000958(s32 value) {
  s32 width = 1;
  s32 height = 2;

  Func_02003d7c();
  Event_ResolvePendingActions(0);
  Sound_Play(188);
  Map_CopyCellPictureIndicesRect(107, 96, 19, 29, width, height);
  Event_Wait(8);
  Map_CopyCellPictureIndicesRect(108, 96, 19, 29, width, height);
  Event_Wait(8);
  Map_CopyCellPictureIndicesRect(107, 98, 19, 29, width, height);
  Event_Wait(8);
  Map_CopyCellPictureIndicesRect(108, 98, 19, 29, width, height);
  Event_Wait(15);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_GetById(gMainRoutineWork.currentOwner)->speedLimit =
      Actor_GetById(gMainRoutineWork.currentOwner)->acceleration = 0x8000;
  Sound_Play(123);
  Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, 312, 496);
  Event_Wait(5);
  Import_EventSetExitRequest(value);
}
