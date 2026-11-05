#include "ankohl_ruins.h"
#include "map.h"
#include "call.h"

struct AnkohlRuinsFloatWork {
  s16 unk00;
  s16 tile;
  s32 level;
  s32 unk08;
  s32 unk0c;
};

extern struct AnkohlRuinsFloatWork Data_02004030;
struct AnkohlRuinsWaterRow {
  u8 level;
  u8 width;
  u8 height;
  u8 x;
  u8 z;
  u8 unk05[3];
};

extern struct AnkohlRuinsWaterRow gData_02002fa8[];

void Func_020015ac(void);
void Func_02002e04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200160c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct AnkohlRuinsFloatWork *work = &Data_02004030;
  s16 i;
  s16 j;
  s16 m;
  s32 row;
  s16 n;

  Actor_SetModeById(gMainRoutineWork.currentOwner, 0);
  m = work->level >> 19;
  leader->motionFlags = 0;
  AnkohlRuins_Call2e24(0x40000, 0x40000, 0x10000);
  FieldEvent_Wait(30);
  Func_02002eec(0x22c);
  AnkohlRuins_Call2df4(22, 66, 3, 1, 7, 66);
  AnkohlRuins_Call2df4(22, 1, 3, 5, 7, 1);
  for (i = 0; i <= 15; i++) {
    struct MapCell *cells;

    for (j = 0; j < gData_02003780; j++)
      FieldEvent_Wait(1);
    work->level += gData_02003788;
    cells = map->layers[2].cells;
    cells += leader->x / 0x100000 + ((leader->z / 0x100000) << 7);
    if (cells->collisionCode == 232) {
      leader->y += gData_02003788;
      leader->terrainHeight = leader->y;
    }
    FieldEvent_Wait(1);
    if (m == 2 && i == 0) {
      AnkohlRuins_Call2df4(11, 70, 1, 1, 11, 69);
      Call6((void (*)())Func_02002e04, 10, 68, 1, 1, 11, 68);
      AnkohlRuins_Call2dfc(10, 5, 1, 1, 11, 5);
    }
    if (m == 1 && i == 15)
      Call6((void (*)())Func_02002e04, 18, 73, 1, 1, 11, 68);
  }
  AnkohlRuins_Call2e24(-1, -1, 0xe666);
  FieldEvent_Wait(5);
  leader->motionFlags = 3;
  if (work->level < 0x200000) {
    gData_02003782 = 0;
    Call2((void (*)())Task_Add, (s32)Func_020015ac, 0x480);
  } else {
    Call6((void (*)())Func_02002e04, 69, 6, 1, 2, 68, 5);
    GameFlag_SetBit(0x204);
    GameFlag_ClearBit(0x17d);
  }
  AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 66);
  AnkohlRuins_Call2df4(18, 1, 3, 5, 7, 1);
  i = work->level >> 19;
  for (row = 0; gData_02002fa8[row].level != 99; row++) {
    if (i >= gData_02002fa8[row].level) {
      for (m = 0; m < gData_02002fa8[row].height; m++) {
        for (n = 0; n < gData_02002fa8[row].width; n++) {
          AnkohlRuins_Call2dfc(21, 14 - i, 1, 1, gData_02002fa8[row].x + n,
                               gData_02002fa8[row].z + m);
          AnkohlRuins_Call2dfc(25, 14 - i, 1, 1, gData_02002fa8[row].x + n,
                               gData_02002fa8[row].z + m + 64);
        }
      }
    }
  }
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 7);
  AnkohlRuins_Call2dfc(22, 14 - i, 2, 1, 13, 7);
  AnkohlRuins_Call2dfc(18, 14 - i, 1, 1, 7, 5);
  AnkohlRuins_Call2dfc(24, 14 - i, 1, 1, 9, 5);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 71);
  AnkohlRuins_Call2dfc(22, 14 - i, 2, 1, 13, 71);
  AnkohlRuins_Call2dfc(26, 14 - i, 1, 1, 7, 69);
  AnkohlRuins_Call2dfc(27, 14 - i, 1, 1, 9, 69);
  if (i == 2)
    AnkohlRuins_Call2dfc(18, 8, 1, 1, 11, 5);
}
