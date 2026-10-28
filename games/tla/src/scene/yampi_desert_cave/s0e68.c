#include "yampi_desert_cave.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);

void Func_02000e68(s32 unused, s32 id) {
  if (gAdventure.scene == (s32)&SceneId_Ov662Room2) {
    s32 row = 12;

    Func_02001848(98, 3, 3, 3, 97, row);
    Func_02001848(33, 12, 1, 1, 34, row);
  }
  /* FAKEMATCH: the native reads the scene again here; without the barrier the
   * read is moved up both paths */
  __asm__("" : : : "memory");
  if (gAdventure.scene == (s32)&SceneId_Ov662Room3) {
    if (id == 12) {
      YampiDesertCave_Call1848(102, 15, 1, 1, 103, 15);
      YampiDesertCave_Call1848(43, 14, 1, 1, 39, 16);
    }
    if (id == 13) {
      struct Actor *object = Actor_GetById(13);

      s32 column = object->x >> 20;
      s32 row = object->z >> 20;

      Func_02001848(7, 10, 1, 1, column, row);
    }
  }
}

void GameFlag_SetBit(s32 flag);

/// Rooms 2 and 3: by where object id stands, sets flags 0x303 to 0x305 and
/// makes the rooms' Func_02001848 calls.
void Func_02000f10(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (gAdventure.scene == (s32)&SceneId_Ov662Room2) {
    s32 row = object->z >> 20;

    if (row == 13) {
      object->motionFlags = 0;
      GameFlag_SetBit(0x303);
      YampiDesertCave_Call1848(32, 13, 1, 1, 34, row);
      YampiDesertCave_Call1848(95, 3, 3, 3, 97, 12);
    } else {
      row = 12;
      YampiDesertCave_Call1848(92, 3, 3, 3, 97, row);
      YampiDesertCave_Call1848(32, 13, 1, 1, 34, row);
    }
  }
  /* FAKEMATCH: the native reads the scene again here; without the barrier the
   * read is moved up both paths */
  __asm__("" : : : "memory");
  if (gAdventure.scene == (s32)&SceneId_Ov662Room3) {
    if (id == 12) {
      s32 column;

      YampiDesertCave_Call1848(103, 16, 1, 1, 103, 15);
      column = object->x >> 20;
      if (column == 38) {
        GameFlag_SetBit(0x305);
        YampiDesertCave_Call1848(38, 15, 1, 1, column, 16);
      } else {
        YampiDesertCave_Call1848(38, 15, 1, 1, 39, 16);
      }
    }
    if (id == 13) {
      if (object->x >> 20 == 10 && object->z >> 20 == 11)
        GameFlag_SetBit(0x304);
      /* FAKEMATCH: the native reads the position again for the last call;
       * without the barrier both paths keep the first reads */
      __asm__("" : : : "memory");
      YampiDesertCave_Call1848(12, 11, 1, 1, object->x >> 20, object->z >> 20);
    }
  }
}
