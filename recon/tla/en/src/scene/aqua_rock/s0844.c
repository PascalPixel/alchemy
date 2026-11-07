#include "types.h"
#include "call.h"
#include "task.h"

struct Unk02003450 {
  s16 unk00[16];
};
extern struct Unk02003450 Data_02003450[];
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02002968(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02002958(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02000740(void);
void Func_02000580(void);

#define UNK02003450_SET(g, a, b, c, d, e, f, h, i, height, flag)               \
  (g)->unk00[0] = (a);                                                         \
  (g)->unk00[1] = (b);                                                         \
  (g)->unk00[2] = (c);                                                         \
  (g)->unk00[3] = (d);                                                         \
  (g)->unk00[4] = (e);                                                         \
  (g)->unk00[5] = (f);                                                         \
  (g)->unk00[6] = (h);                                                         \
  (g)->unk00[7] = (i);                                                         \
  (g)->unk00[8] = (height);                                                    \
  (g)->unk00[9] = (flag);                                                      \
  (g)->unk00[10] = GameFlag_Test(flag) ^ 1;                                    \
  (g)->unk00[15] = 0;                                                          \
  (g)->unk00[12] = 9999;                                                       \
  (g)->unk00[11] = 9999

void Func_02000844(void) {
  struct Unk02003450 *entry = Data_02003450;

  UNK02003450_SET(entry, 64, 74, 76, 74, 70, 10, 6, 74, 36, 0x200);
  entry++;
  UNK02003450_SET(entry, 70, 74, 82, 74, 78, 10, 14, 74, 36, 0x201);
  entry++;
  UNK02003450_SET(entry, 90, 93, 102, 93, 103, 26, 39, 90, 15, 0x202);
  entry++;
  UNK02003450_SET(entry, 96, 93, 108, 93, 107, 26, 43, 90, 15, 0x203);
  entry++;
  UNK02003450_SET(entry, 116, 95, 122, 95, 96, 32, 32, 96, 13, 0x1a6b);
  if (GameFlag_Test(0x320))
    GameFlag_SetBit(0x200);
  else
    GameFlag_ClearBit(0x200);
  if (GameFlag_Test(0x321))
    GameFlag_SetBit(0x201);
  else
    GameFlag_ClearBit(0x201);
  /* FAKEMATCH(temporary): each copy's five varying arguments held in locals
   * of their own scope; as direct calls with the arguments written in,
   * 188 of 307 instructions differ in tla-en. */
  if (GameFlag_Test(0x200)) {
    struct Unk02003450 *opened = &Data_02003450[0];
    {
      s32 srcX = opened->unk00[0] + 3, srcY = opened->unk00[1],
          height = opened->unk00[8], destX = opened->unk00[4],
          destY = opened->unk00[5];
      Func_02002968(srcX, srcY, 3, height, destX, destY);
    }
    {
      s32 srcX = opened->unk00[2] + 3, srcY = opened->unk00[3],
          height = opened->unk00[8], destX = opened->unk00[6],
          destY = opened->unk00[7];
      Func_02002958(srcX, srcY, 3, height, destX, destY);
    }
    opened->unk00[12] = 9999;
    opened->unk00[11] = 9999;
    opened->unk00[10] = 1;
  }
  if (GameFlag_Test(0x201)) {
    struct Unk02003450 *opened = &Data_02003450[1];
    {
      s32 srcX = opened->unk00[0] + 3, srcY = opened->unk00[1],
          height = opened->unk00[8], destX = opened->unk00[4],
          destY = opened->unk00[5];
      Func_02002968(srcX, srcY, 3, height, destX, destY);
    }
    {
      s32 srcX = opened->unk00[2] + 3, srcY = opened->unk00[3],
          height = opened->unk00[8], destX = opened->unk00[6],
          destY = opened->unk00[7];
      Func_02002958(srcX, srcY, 3, height, destX, destY);
    }
    opened->unk00[12] = 9999;
    opened->unk00[11] = 9999;
    opened->unk00[10] = 1;
  }
  Value2((s32 (*)())Task_Add, (s32)Func_02000740, TASK_PASS_VBLANK);
  Task_Add(Func_02000580, TASK_PASS_VBLANK);
}
