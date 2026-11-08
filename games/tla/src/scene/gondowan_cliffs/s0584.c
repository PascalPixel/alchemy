#include "types.h"
#include "actor.h"
#include "call.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void Func_020011d4();
void Func_020011dc();
void Func_0200124c();
s32 Map_GetTerrainHeight(s32, s32, s32);

void Func_02000584(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 25 instructions
   * differ in tla-en. */
  struct Actor *object = Actor_GetById(12);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  Call6(Func_020011d4, 79, 28, 1, 1, 81, 28);
  GameFlag_SetBit(SPLIT_CONSTANT(0x102, 255));
}

void Func_020005bc(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 30 instructions
   * differ in tla-en. */
  struct Actor *object = Actor_GetById(13);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  object->terrainHeight = PIXELS(32);
  object->y = PIXELS(32);
  Call6(Func_020011d4, 115, 35, 1, 1, 117, 35);
  GameFlag_SetBit(SPLIT_CONSTANT(0x200, 2));
}

void Func_02000600(void) { GameFlag_SetBit(SPLIT_CONSTANT(0x800, 133)); }

void Func_02000610(s32 unused, s32 cell) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 34 instructions
   * differ in tla-en. */
  struct Actor *object;

  if (cell == 30) {
    object = Actor_GetById(17);
    Call6(Func_020011dc, 124, 1, 1, 1, 119, 43);
    Call3(Func_0200124c, 17, PIXELS(1912), PIXELS(728));
    object->terrainHeight = object->y =
        Map_GetTerrainHeight(0, object->x, object->z);
    GameFlag_SetBit(SPLIT_CONSTANT(0x800, 134));
  }
}
