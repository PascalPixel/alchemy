/// @file
/// Scene code of overlay 0x681, in ROM order, from s2cb4's offset up to the next object the overlay links.

#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/actor.h"
#include "event_inline.h"
#include "types.h"

// Starts the room where persons 8 to 16 sway: unless flag 265 is set, clears the sway
// state and places each person, then starts the sway's task.
extern u16 gSceneState[];
extern const s16 gData_0200533c[][2];

void Func_02002b04(u32 id, s8 mode);
void Func_02002b8c(void);

/// Starts the room where persons 8 to 16 sway: unless flag 265 is set, clears
/// the sway state and places each person, then starts the sway's task.
void Func_02002cb4(u8 mode) {
  u16 *state = gSceneState;
  u16 *pending = state + 1;

  if (!Event_GetFlag(265)) {
    s32 i;
    s32 id;

    *state = 0;
    *pending = 0;
    Actor_Get(FieldParty_GetLeaderActor())->priorityFlags |= 0x40;
    for (i = 0, id = 8; id <= 16; i++, id++) {
      Actor_SetPosition(id, gData_0200533c[i][0] << 19, gData_0200533c[i][1] << 19);
      Func_02002b04(id, mode);
    }
  }
  {
    s32 priority = 0x480;

    Task_AddFrame(Func_02002b8c, priority);
  }
}

// Opens the passages on each side of the hall by how many of its two pairs of flags
// (0x9bc and 0x9bd, 0x9be and 0x9bf) are set: both open the wide way, one a side way, and
// a single side opened alone also opens its connecting cells.
void Func_02002d30(void) {
  u8 a = GameFlag_Test(0x9bc);
  u8 third = GameFlag_Test(0x9be);
  s32 b = GameFlag_Test(0x9bd);
  s8 first = a;
  s8 sideThird = third;
  u8 sumLeft = b + first;
  s32 d = GameFlag_Test(0x9bf);
  u8 sumRight = sideThird + d;
  s8 left = sumLeft;
  s8 right = sumRight;

  if (left != 0) {
    if (left == 2) {
      MapGrid_CopyCellsLayered(74, 115, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(71, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(67, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(79, 110, 1, 3, 30, 85);
    } else if (first != 0) {
      MapGrid_CopyCellsLayered(66, 95, 7, 12, 24, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(81, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(75, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(65, 110, 1, 3, 30, 85);
    } else {
      MapGrid_CopyCellsLayered(69, 82, 5, 12, 26, 76);
      if (right == 0)
        return;
      if (right == 2)
        MapGrid_CopyCellsLayered(69, 110, 1, 3, 30, 85);
      else if (sideThird != 0)
        MapGrid_CopyCellsLayered(73, 110, 1, 3, 30, 85);
      else
        MapGrid_CopyCellsLayered(77, 110, 1, 3, 30, 85);
    }
  }
  if (right == 0)
    return;
  if (right == 2) {
    MapGrid_CopyCellsLayered(83, 115, 6, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(82, 124, 1, 3, 30, 85);
  } else if (sideThird != 0) {
    MapGrid_CopyCellsLayered(76, 82, 4, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(75, 91, 1, 3, 30, 85);
  } else {
    MapGrid_CopyCellsLayered(82, 82, 6, 12, 31, 76);
    if (left == 0)
      MapGrid_CopyCellsLayered(81, 91, 1, 3, 30, 85);
  }
}
