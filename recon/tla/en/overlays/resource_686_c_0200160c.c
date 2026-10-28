// Draft, differ 375 of 410: raises the water level by one step: sixteen frames
// of rise (gData_02003788 each, gData_02003780 frames apart) carrying the
// leader when it stands on water (cell 232), then the cells for the new level
// from the gData_02002fa8 rows. Written from the listing; the loops' registers
// and spills are far off (the native keeps the step count on the stack).
// Func_020019a0 is the same with other cells.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "map.h"

struct AnkohlRuinsFloatWork {
  s16 unk00;
  s16 tile;
  s32 level;
  s32 unk08;
  s32 unk0c;
};

struct AnkohlRuinsWaterRow {
  u8 level;
  u8 width;
  u8 height;
  u8 x;
  u8 z;
  u8 unk05[3];
};

extern struct AnkohlRuinsFloatWork Data_02004030;
extern struct AnkohlRuinsWaterRow gData_02002fa8[];
extern s16 gData_02003780;
extern s16 gData_02003782;
extern s32 gData_02003788;

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_WaitFrames(s32 frames);
s32 Task_Add(void (*callback)(void), s32 priority);
void Func_020015ac(void);
void Func_02002df4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002dfc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002e04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002e24(s32 a, s32 b, s32 c);
void Func_02002eec(s32 cue);

void Func_0200160c(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct AnkohlRuinsFloatWork *work = &Data_02004030;
  s32 level;
  s16 i;
  s16 j;
  s32 row;
  s16 m;
  s16 n;
  s32 top;

  Object_SetModeById(gAdventure.currentOwner, 0);
  level = work->level >> 19;
  leader->motionFlags = 0;
  Func_02002e24(0x40000, 0x40000, 0x10000);
  FieldEvent_WaitFrames(30);
  Func_02002eec(0x22c);
  Func_02002df4(22, 66, 3, 1, 7, 66);
  Func_02002df4(22, 1, 3, 5, 7, 1);
  i = 0;
step:
  {
    struct MapCell *cells;
    s32 rise;

    for (j = 0; j < gData_02003780; j++)
      FieldEvent_WaitFrames(1);
    rise = gData_02003788;
    work->level += rise;
    cells = map->layers[2].cells;
    if (cells[leader->x / 0x100000 + ((leader->z / 0x100000) << 7)]
            .collisionCode == 232) {
      leader->y += rise;
      leader->terrainHeight = leader->y;
    }
    FieldEvent_WaitFrames(1);
    if (level == 2 && i == 0) {
      Func_02002df4(11, 70, 1, 1, 11, 69);
      Func_02002e04(10, 68, 1, 1, 11, 68);
      Func_02002dfc(10, 5, 1, 1, 11, 5);
    }
    if (level == 1 && i == 15)
      Func_02002e04(18, 73, 1, 1, 11, 68);
  }
  if (++i <= 15)
    goto step;
  Func_02002e24(-1, -1, 0xe666);
  FieldEvent_WaitFrames(5);
  leader->motionFlags = 3;
  if (work->level < 0x200000) {
    gData_02003782 = 0;
    Task_Add(Func_020015ac, 0x480);
  } else {
    Func_02002e04(69, 6, 1, 2, 68, 5);
    GameFlag_SetBit(0x204);
    GameFlag_ClearBit(0x17d);
  }
  Func_02002df4(18, 66, 3, 1, 7, 66);
  Func_02002df4(18, 1, 3, 5, 7, 1);
  top = work->level >> 19;
  for (row = 0; gData_02002fa8[row].level != 99; row++) {
    if (top < gData_02002fa8[row].level)
      continue;
    for (m = 0; m < gData_02002fa8[row].height; m++) {
      for (n = 0; n < gData_02002fa8[row].width; n++) {
        Func_02002dfc(21, 14 - top, 1, 1, gData_02002fa8[row].x + n,
                      gData_02002fa8[row].z + m);
        Func_02002dfc(25, 14 - top, 1, 1, gData_02002fa8[row].x + n,
                      gData_02002fa8[row].z + m + 64);
      }
    }
  }
  Func_02002dfc(19, 14 - top, 2, 1, 2, 7);
  Func_02002dfc(22, 14 - top, 2, 1, 13, 7);
  Func_02002dfc(18, 14 - top, 1, 1, 7, 5);
  Func_02002dfc(24, 14 - top, 1, 1, 9, 5);
  Func_02002dfc(19, 14 - top, 2, 1, 2, 71);
  Func_02002dfc(22, 14 - top, 2, 1, 13, 71);
  Func_02002dfc(26, 14 - top, 1, 1, 7, 69);
  Func_02002dfc(27, 14 - top, 1, 1, 9, 69);
  if (top == 2)
    Func_02002dfc(18, 8, 1, 1, 11, 5);
}
