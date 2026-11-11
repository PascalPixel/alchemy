#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

struct MapBits {
  u8 unk00[23];
  u8 low : 2;
  u8 mode : 2;
};

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02000c14(void);
void Func_02000bb8(void);

void Func_02000bf0(void);

static __inline__ void Task_Add_(void (*func)(void), s32 priority) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline, which keeps each
   * call's own constants, as The Broken Seal's scene layer does; as direct
   * calls to Task_Add, 28 of 107 instructions of Func_02000dc4 differ in
   * tla-en. */
  Task_Add(func, priority);
}

void Func_02000dc4(void) {
  struct MapBits *map = (struct MapBits *)Ram_HeapSlots->mapWork;
  struct MainRoutineWork *party;

  GameFlag_SetBit(0x9b4);
  if (!GameFlag_Test(0x9ad))
    Actor_SetHorizontalPositionWithTerrain(65, 0, 0);
  map->mode = 1;
  party = &gMainRoutineWork;
  Actor_GetById(party->currentOwner)->priorityFlags |= 32;
  Task_Add_(Func_02000c14, 0x480);
  Task_Add_(Func_02000bb8, 0x480);
  Actor_SetSingleChildFlags(Actor_GetById(17), 0);
  *((u8 *)Actor_GetById(17) + 85) = 0;
  if (GameFlag_Test(0x9b2)) {
    Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(23, 0, 0);
  } else {
    if (GameFlag_Test(0x9b1)) {
      s32 speed = 0xc0;

      Actor_SetPositionAndFacing(23, PIXELS(504), PIXELS(360), speed << 6);
    }
    Actor_WatchAndSetScript(19, party->currentOwner);
    Actor_WatchAndSetScript(20, party->currentOwner);
    Actor_WatchAndSetScript(21, party->currentOwner);
    Actor_WatchAndSetScript(22, party->currentOwner);
  }
}

void Func_02000ed0(void) {
  if (gMainRoutineWork.entrance == 3)
    GameFlag_ClearBit(0x12f);
  if (GameFlag_Test(0x9b2)) {
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  } else if (GameFlag_Test(0x9b0)) {
    Actor_SetPositionAndFacing(9, 70 << 16, 308 << 16, FACING_EAST);
    Actor_SetPositionAndFacing(10, 80 << 16, 316 << 16, FACING_EAST);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 3, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 19);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 20);
    Map_CopyCellAttributeRect(1, 16, 1, 1, 4, 21);
    wait(1);
  }
  if (GameFlag_Test(0x300))
    Actor_SetPosition(14, 120 << 16, 376 << 16);
  {
    s32 priority = 0x480;

    Task_Add(Func_02000bf0, priority);
  }
}
