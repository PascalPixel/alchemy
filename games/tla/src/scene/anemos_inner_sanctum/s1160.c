#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern char SceneId_Ov6B0Room2;
extern char SceneId_Ov6B0Room3;
extern char SceneId_Ov6B0Room4;

void Func_02003380(s32 x, s32 z, s32 a, s32 b);
void Func_02003398(void);
void Func_020033a0(void);
void Func_020034a0(void);

static __inline__ void MoveObject(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as a direct call, 2
   * of 243 instructions differ in tla-en. */
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void SetCell(s32 a, s32 x, s32 z, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as direct calls, 36
   * of 243 instructions differ in tla-en. */
  SetMapCellCollision(a, x, z, b);
}

/// An event handler from the sanctum's event table (data.c), reading the
/// leader's place in 8-pixel cells: sets the cells of rooms 3 and 4, and sets
/// flags 0xa80 to 0xa86 where the leader stands on given cells, running the
/// event at x 67, z 45 of room 3 and moving object 13 to object 12 at x 91,
/// z 59 until flag 0xa84 is set.
void Func_02001160(void) {
  struct Actor *leader =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  s32 x;
  s32 z;

  Func_02003398();
  Event_ResolvePendingActions(0);
  Func_020034a0();
  x = leader->x >> 19;
  z = leader->z >> 19;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room3) {
    SetCell(2, 0x480000, 0x1b80000, 232);
    if (x != 8)
      SetCell(2, 0x480000, 0x1a80000, 232);
  }
  /* FAKEMATCH(barrier): an empty asm with a memory clobber, so GCSE does not
   * keep gMainRoutineWork's address from the first test; without it, 11 of 243
   * instructions differ in tla-en. */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4) {
    SetCell(2, 0x1180000, 0x1580000, 0);
    SetCell(2, 0x1180000, 0x1780000, 232);
    SetCell(2, 0x1280000, 0x1680000, 0);
    SetCell(2, 0x2080000, 0x2780000, 232);
  }
  /* FAKEMATCH(barrier): as above, so the scene is read again for each test
   * below rather than loaded early by GCSE; without it, 46 of 243 instructions
   * differ in tla-en. */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room2) {
    if (x == 37 && z == 31) {
      GameFlag_SetBit(0xa80);
      Actor_SetSpritePriority(8, 2);
    }
    if (x == 55 && z == 95)
      GameFlag_SetBit(0xa81);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room3) {
    if (x == 7 && z == 53)
      GameFlag_SetBit(0xa82);
    if (x == 67 && z == 45) {
      MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
      Actor_SetPosition(14, 0, 0);
      Event_PlaySound(0x248);
      FieldEvent_Wait(30);
      Event_PlaySound(188);
      MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
      FieldEvent_Wait(5);
      MapGrid_CopyCellsTo(51, 20, 31, 20, 1, 2);
      FieldEvent_Wait(5);
      Func_02003380(0x1f00000, 0x1500000, 0, 0);
      GameFlag_SetBit(0xa83);
    }
    if (x == 93 && z == 57)
      GameFlag_SetBit(0xa84);
    if (x == 91 && z == 59) {
      if (!Event_GetFlag(0xa84)) {
        s32 fromX = (Actor_Get(12)->x >> 16) << 16;

        Actor_SetPosition(
            13, fromX, (Actor_Get(12)->z >> 16) << 16);
        MoveObject(12, PIXELS(728), PIXELS(472));
      }
      GameFlag_SetBit(0xa85);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4 && x == 33 && z == 45)
    GameFlag_SetBit(0xa86);
  Func_020033a0();
}
