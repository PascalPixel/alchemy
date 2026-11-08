#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "magma_rock.h"

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_020014bc(s32 mode, s32 cue);

/// In room 1 or 2, targets actor 15 or 14, runs Func_020014bc and sets flag
/// 0x200.
void Func_020019e8(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct MainRoutineWork *party = &gMainRoutineWork;

  {
    s32 scene = party->scene;

    if (scene == (s32)&SceneId_Ov691Room1)
      work->target = Actor_GetById(15);
    else if (scene == (s32)&SceneId_Ov691Room2)
      work->target = Actor_GetById(14);
    Func_020014bc(mode, cue);
  }
  GameFlag_SetBit(0x200);
}

/// In room 1 or 2, targets actor 16 or 10, runs Func_020014bc and sets flag
/// 0x201.
void Func_02001a38(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct MainRoutineWork *party = &gMainRoutineWork;

  {
    s32 scene = party->scene;

    if (scene == (s32)&SceneId_Ov691Room1)
      work->target = Actor_GetById(16);
    else if (scene == (s32)&SceneId_Ov691Room2)
      work->target = Actor_GetById(10);
    Func_020014bc(mode, cue);
  }
  GameFlag_SetBit(0x201);
}
