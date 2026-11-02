// Draft, differ 170 of 410, skeleton gap 36 (tla-en, 2026-11-01)
// Control flow, calls, arguments and fields are the native's; the frame is 24
// bytes against its 20. This round took it from differ 230, gap 40:
// - The row and line loops are real, which gives the native's biv elimination:
//   row * 8 reduced, the line tail reading a second copy of it from the stack
//   (native [sp, #12]), and the n << 16 counter's constant kept in the body.
// - The table is two pointer locals to u8[8] rows. table is reloaded at the row
//   tail and lines at the line tail, the native's ldr r2 and ldr r1; a single
//   local, or gData_02002fa8[row] spelled directly, hoists the address out of
//   every loop (differ 260). The word at +4 is read as a byte, z.
// - The step, wait and cell loops stay labels, because a real loop hoists their
//   constants, 14 - i and the row address (differ 320 with all five real). So
//   the wait and cell counters k are the shifted givs a real loop would make,
//   written by hand.
// What is left, native against here:
// - 14 - i (r5) and the row address (r6) are rebuilt in each cell, where the real
//   line loop hoists both to its preheader: this is the skeleton gap's largest
//   part. Making the line loop a label too only moves 14 - i to the row loop
//   (differ 211). The native hoists less than alchemy-gcc in its loops generally;
//   why is a compiler question and out of scope here.
// - z is table + a second giv row * 8 + 4 (r11); here it is the row address + 4
//   in its own register. A z local at the row, line or cell level, or row and z
//   stepped by hand, gives the reg + reg load but costs more (differ 239 to 255).
// - One giv too many: row * 8 lives in [sp, #12], [sp, #16] and r11 where the
//   native has r7 and [sp, #12]. That extra word is the frame's 4 bytes and moves
//   i to [sp, #20].
// - cell n r7/r4, row r11/r7, line m r11/r8, map r8/r9, k r8/r10: most of the
//   differ. The native's n is caller-saved in r4, here the row pointer takes that
//   place, so row outranks n there.
// - The wait loop's k through a copy (adds r3, r5, #0), the strh of
//   gData_02003782 with r2 and r3 swapped, and one argument order in the second
//   call after it.
// Measured without gain this round: which local each site reads (a grid of 160
// forms, none under 170), a third local for the cells, a pointer p in the cell
// body (228), tile as a local or with a 0 default (251), direct calls and Call6
// in the cell loop, real wait loops through Call1 (254), level and m as separate
// variables (232), i as s32 (281), row as u32 or s16 (no change), u8 (263),
// declaration orders, a table of structs (261), a byte table with row * 8 (207),
// and a u8 (*)[8] cast at each site (differ 181, but gap 33 and 34 with the row
// section further off; it is a cast to a type the object is not, so not kept).
// Sixteen steps of rise (gData_02003788 each, gData_02003780 frames apart)
// carrying the leader when it stands on water (cell 232), then the cells for
// the new level from the gData_02002fa8 rows: level, width, height, x, then z in
// the next word. The step counter and the new level share one variable, as the
// native keeps both in one stack slot.
// Func_020019a0 is the same with other cells; these loop forms are untried there.
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

extern struct AnkohlRuinsFloatWork Data_02004030;
extern u8 gData_02002fa8[][8];

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
  u8 (*table)[8];
  u8 (*lines)[8];
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
  table = gData_02002fa8;
  for (row = 0; table[row][0] != 99; row++) {
    lines = table;
    if (i >= table[row][0]) {
      m = 0;
      if (m < table[row][2]) {
        do {
          n = 0;
          if (n < lines[row][1]) {
            s32 k = 0x10000;
          nextCell:
            AnkohlRuins_Call2dfc(21, 14 - i, 1, 1, lines[row][3] + n, lines[row][4] + m);
            AnkohlRuins_Call2dfc(25, 14 - i, 1, 1, lines[row][3] + n, lines[row][4] + m + 64);
            n = k >> 16;
            k += 0x10000;
            if (n < lines[row][1])
              goto nextCell;
          }
          m++;
          lines = gData_02002fa8;
        } while (m < lines[row][2]);
      }
    }
    table = gData_02002fa8;
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
