#include "yampi_desert_cave.h"
#include "actor.h"
#include "version.h"
#include "event_inline.h"

extern const s16 gData_020019cc[];

void Func_020011e0(void);

/// The chest that follows the step count: when the tracked object stands on
/// its cell, mode 1 stops the follower and mode 2 drops the chest.
void Func_02001238(s32 mode) {
  s32 index = (((s32)GameFlag_GetByte(0x278)) / 32) * 2;
  s16 column = gData_020019cc[index];
  s32 px = column * 8;
  s16 row = gData_020019cc[index + 1];
  s32 pz = row * 8;
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *tracked =
      *(struct Actor **)(Ram_HeapSlots->battleFxWork[0] + 0x1930);
  s32 x = 0, z = 0;

  if (tracked != NULL) {
    x = tracked->x >> 20;
    z = tracked->z >> 20;
  }
  if (px / 16 != x || pz / 16 != z)
    return;
  if (mode == 1) {
    Task_RemoveFar(Func_020011e0);
    Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  }
  if (mode == 2) {
    struct Actor *chest = Actor_GetById(17);

    Actor_SetHorizontalPositionWithTerrain(17, column << 19, row << 19);
    chest->y = 0x200000;
    chest->motionFlags = 3;
    chest->gravity = 0x170a;
    wait(50);
    // Through an s32 and a pointer the 17 is built with a move; as a halfword
    // constant it comes from the pool.
    {
      s16 *counter = (s16 *)(work + 0x160);
      s32 value = 17;

      *counter = value;
    }
#if TLA_PATCH_1
    GameFlag_ClearBit(0x270);
#endif
  }
}
