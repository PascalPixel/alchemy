#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "magma_rock.h"
#include "event_inline.h"

void Func_020014bc(s32 mode, s32 cue);

/// In room 1 or 2, targets actor 15 or 14, runs Func_020014bc and sets flag
/// 0x200.
void Func_020019e8(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct MainRoutineWork *party = &gMainRoutineWork;

  {
    s32 scene = party->scene;

    if (scene == (s32)&SceneId_Ov691Room1)
      work->target = Actor_Get(15);
    else if (scene == (s32)&SceneId_Ov691Room2)
      work->target = Actor_Get(14);
    Func_020014bc(mode, cue);
  }
  Event_SetFlag(0x200);
}

/// In room 1 or 2, targets actor 16 or 10, runs Func_020014bc and sets flag
/// 0x201.
void Func_02001a38(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;
  struct MainRoutineWork *party = &gMainRoutineWork;

  {
    s32 scene = party->scene;

    if (scene == (s32)&SceneId_Ov691Room1)
      work->target = Actor_Get(16);
    else if (scene == (s32)&SceneId_Ov691Room2)
      work->target = Actor_Get(10);
    Func_020014bc(mode, cue);
  }
  Event_SetFlag(0x201);
}

void Func_020014bc(s32 mode, s32 cue);
void Func_02003aec(void);
void Func_02003af4(void);

void Func_02001a8c(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  work->target = Actor_Get(17);
  Func_020014bc(mode, cue);
  Event_SetFlag(0x202);
}

void Func_02001ac0(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  work->target = Actor_Get(18);
  Func_020014bc(mode, cue);
  Event_SetFlag(0x203);
}

void Func_02001af4(s32 mode, s32 cue) {
  struct MagmaRockWork *work = Ram_HeapSlots->effectWork;

  work->target = Actor_Get(11);
  Func_020014bc(mode, cue);
  Event_SetFlag(0x204);
}

void Func_02001b24(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode != 2)
    Event_ClearFlag(0x2f8);
}

void Func_02001b44(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode != 2) {
    Func_02003aec();
    FieldEvent_ResolvePendingActions(0);
    Actor_Get(party->currentOwner)->motionFlags = 0;
    Actor_WalkTo(party->currentOwner, 264, 440);
    FieldEvent_Wait(5);
    Event_PlaySound(123);
    FieldEvent_SetExitRequest(2);
    Func_02003af4();
  }
  Event_SetFlag(0x2f8);
}

extern const u8 gData_02003dba[];
void Func_02002e30(const u8 *data);
void Func_02003b3c(s32 id);
void Func_020036f8(s32 a, s32 b, s32 owner);

void Func_02001ba4(void) { Event_ResolvePendingActions(0); }

void Func_02001bb0(void) { Func_02002e30(gData_02003dba); }

void Func_02001bc0(void) {
  Field_SetCellBehaviorAt(0, 0x1080000, 0x2280000, 30);
  Event_SetFlag(0x212);
}

void Func_02001be0(void) {
  Field_SetCellBehaviorAt(0, 0x1080000, 0x2280000, 0);
  Event_ClearFlag(0x212);
}
