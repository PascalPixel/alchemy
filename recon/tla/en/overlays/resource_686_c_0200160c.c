// Draft, differ 230 of 410, skeleton gap 40 (tla-en, 2026-11-01)
// Control flow, calls, arguments and fields are the native's. Three things took
// this from differ 309, gap 99; Func_020019a0 has the same shape (not yet tried):
// - z is read as table[row * 8 + 4], base plus offset, where x, width and height
//   go through table + row * 8: the native's r9 + r11 against r6 + 3.
// - The level and the line counter are one s16 variable, m (native r8). With the
//   label wait loop, this put the work pointer in r7 and the leader in r6, as
//   the native has them; the likely reason is that m now conflicts with the row
//   offset and so cannot take r7 (inferred, not traced).
// - Every loop is written with labels, as the native keeps its constants, 14 - i
//   and the row's address in each body. loop.c hoists these from a real loop (the
//   address of gData_02003780 has life 1 in 10 insns; do { } while (0) is phony to
//   loop.c and does not stop it). The counters k are the shifted givs the native
//   would have from a real loop, so a label loop is an approximation of it.
// What is left, native against here: row r7/r9, cell n r4/r7, line m r8/r10, k
// r10/r8, map r9/r8, &motionFlags r10/r9; most of the differ is these names.
// n wins r7 by priority (22 refs over 48 insns against row's 14 over 114), and the
// native's n looks like caller-save, reached once r5 to r7 are gone, which would
// mean row outranks n there; what gives row that rank is the open question. gcse
// also merges row + table across the levels into one register (p takes the
// height's value), where the native rebuilds p in the cell loop and reloads the
// table in each tail. The native also keeps a copy of row in [sp, #12] for the
// line test; a local for it is folded away, so this draft has none, and its frame
// is 16 bytes against the native's 20.
// Measured without gain: j or k shared with the cell counters (differ 309, 321),
// k, tile or p in outer scopes, n as s32, i declared last, p or the tests through
// the copy of row (no change: cse and cprop fold them), t[row + 2] and a struct
// cast for the height (no change), a rise local (236), direct calls (236),
// k read through a copy, which is nearer the native's shape (gap 38, differ 235).
// Sixteen steps of rise (gData_02003788 each, gData_02003780 frames apart)
// carrying the leader when it stands on water (cell 232), then the cells for
// the new level from the gData_02002fa8 rows. The step counter and the new
// level share one variable, as the native keeps both in one stack slot.
// Func_020019a0 is the same with other cells.
#include "../../../../games/tla/src/scene/ankohl_ruins/ankohl_ruins.h"
#include "map.h"
#include "call.h"

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
  u8 *rowTable;
  u8 *lineTable;
  u8 *cellTable;
  s16 n;

  Actor_SetModeById(gMainRoutineWork.currentOwner, 0);
  m = work->level >> 19;
  leader->motionFlags = 0;
  AnkohlRuins_Call2e24(0x40000, 0x40000, 0x10000);
  FieldEvent_Wait(30);
  Func_02002eec(0x22c);
  AnkohlRuins_Call2df4(22, 66, 3, 1, 7, 66);
  AnkohlRuins_Call2df4(22, 1, 3, 5, 7, 1);
  i = 0;
step:
  {
    struct MapCell *cells;

    if (gData_02003780 > 0) {
      s32 k = 0x10000;
    nextFrame:
      FieldEvent_Wait(1);
      j = k >> 16;
      k += 0x10000;
      if (j < gData_02003780)
        goto nextFrame;
    }
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
  if (++i <= 15)
    goto step;
  AnkohlRuins_Call2e24(-1, -1, 0xe666);
  FieldEvent_Wait(5);
  leader->motionFlags = 3;
  if (work->level < 0x200000) {
    gData_02003782 = 0;
    Task_Add(Func_020015ac, 0x480);
  } else {
    Call6((void (*)())Func_02002e04, 69, 6, 1, 2, 68, 5);
    GameFlag_SetBit(0x204);
    GameFlag_ClearBit(0x17d);
  }
  AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 66);
  AnkohlRuins_Call2df4(18, 1, 3, 5, 7, 1);
  i = work->level >> 19;
  rowTable = (u8 *)gData_02002fa8;
  if (rowTable[0] != 99) {
    s32 zOffset = 4;
    row = 0;
  nextRow:
    lineTable = rowTable;
    if (i >= rowTable[row]) {
      m = 0;
      if (m < (row + rowTable)[2]) {
      nextLine:
        {
          n = 0;
          cellTable = lineTable;
          if (n < (row + lineTable)[1]) {
            s32 k = 0x10000;
          nextCell:
            {
              u8 *p = row + cellTable;
              s32 tile = 14 - i;
              AnkohlRuins_Call2dfc(21, tile, 1, 1, p[3] + n,
                                   cellTable[zOffset] + m);
              AnkohlRuins_Call2dfc(25, tile, 1, 1, p[3] + n,
                                   cellTable[zOffset] + m + 64);
              n = k >> 16;
              k += 0x10000;
              if (n < p[1])
                goto nextCell;
            }
          }
          m++;
          lineTable = (u8 *)gData_02002fa8;
          if (m < (row + lineTable)[2])
            goto nextLine;
        }
      }
    }
    rowTable = (u8 *)gData_02002fa8;
    row += 8;
    zOffset += 8;
    if (rowTable[row] != 99)
      goto nextRow;
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
