#include "../../../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"
#include "scene_ids.h"

void Func_02001250(s32 show, s32 first, u32 count);

void Func_02001200(s32 show) {
  if (gMainRoutineWork.scene == (s32)&SceneId_ShrineOfTheSeaGod6)
    Func_02001250(show, 9, 3);
}

void Func_02001228(void) {
  Func_02001200(1);
  GameFlag_SetBit(0x208);
}

void Func_0200123c(void) {
  Func_02001200(0);
  GameFlag_ClearBit(0x208);
}

void Func_02001250(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct ShrineOfTheSeaGodObject *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~0x10;
    else
      actor->priorityFlags |= 0x10;
  }
}

void Func_02001290(void) {
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  GameFlag_SetBit(SPLIT_CONSTANT(SPLIT_CONSTANT(0x200, 10)));
}

void Func_020012b4(void) {
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  GameFlag_ClearBit(SPLIT_CONSTANT(SPLIT_CONSTANT(0x200, 10)));
}
