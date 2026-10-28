// Draft, differ 416 of 307: sets up Aqua Rock's five water gates (Data_02003450,
// 32 bytes each; Func_02000580 runs them): each gate's source cells, map
// place, height and flag (0x200 to 0x203 and 0x1a6b), its state the flag
// inverted and both animation rows finished (9999); flags 0x320 and 801
// set or clear the first two gates' flags, and an open gate's cells are
// copied in at once. Then it schedules Func_02000740 and Func_02000580.
// Left: a rough reading: alchemy-gcc loads the gates' halfword constants from the pool where the native builds them with movs and keeps the shared ones (70, 74, 10, 9999) in r6, r8, r10 and r11; the flag tests and stores interleave otherwise.
#include "types.h"

struct Gate {
  s16 cell[16];
};
extern struct Gate Data_02003450[];
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02002968(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02002958(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Task_Add(void (*callback)(void), s32 priority);
void Func_02000740(void);
void Func_02000580(void);

#define GATE(n) (Data_02003450[n].cell)
#define GATE_SET(n, a, b, c, d, e, f, g, h, height, flag)                     \
  (GATE(n)[0] = (a), GATE(n)[1] = (b), GATE(n)[2] = (c), GATE(n)[3] = (d),    \
   GATE(n)[4] = (e), GATE(n)[5] = (f), GATE(n)[6] = (g), GATE(n)[7] = (h),    \
   GATE(n)[8] = (height), GATE(n)[9] = (flag), GATE(n)[15] = 0,               \
   GATE(n)[10] = GameFlag_Test(flag) ^ 1, GATE(n)[12] = 9999,                 \
   GATE(n)[11] = 9999)

#define OPEN(n, flag)                                                          \
  if (GameFlag_Test(flag)) {                                                   \
    Func_02002968(GATE(n)[0] + 3, GATE(n)[1], 3, GATE(n)[8], GATE(n)[4],       \
                  GATE(n)[5]);                                                 \
    Func_02002958(GATE(n)[2] + 3, GATE(n)[3], 3, GATE(n)[8], GATE(n)[6],       \
                  GATE(n)[7]);                                                 \
    GATE(n)[12] = 9999;                                                        \
    GATE(n)[11] = 9999;                                                        \
    GATE(n)[10] = 1;                                                           \
  }

void Func_02000844(void) {
  GATE_SET(0, 0x40, 74, 76, 74, 70, 10, 6, 74, 36, 0x200);
  GATE_SET(1, 70, 74, 82, 74, 78, 10, 14, 74, 36, 513);
  GATE_SET(2, 90, 93, 102, 93, 103, 26, 39, 90, 15, 514);
  GATE_SET(3, 0x60, 93, 108, 93, 107, 26, 43, 90, 15, 515);
  GATE_SET(4, 116, 95, 122, 95, 0x60, 0x20, 0x20, 0x60, 13, 0x1a6b);
  if (GameFlag_Test(0x320))
    GameFlag_SetBit(0x200);
  else
    GameFlag_ClearBit(0x200);
  if (GameFlag_Test(801))
    GameFlag_SetBit(513);
  else
    GameFlag_ClearBit(513);
OPEN(0, 0x200);
  OPEN(1, 513);
  Task_Add(Func_02000740, 0x480);
  Task_Add(Func_02000580, 0x480);
}
