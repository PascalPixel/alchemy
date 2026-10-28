#include "work9c.h"

s32 GameFlag_Test(s32 flag);
void Func_080d8d68(s32 index, s32 a1, s32 a2);
void Func_080d9104(s32 index, s32 a1, s32 a2);

void Func_080d9ad0(s32 index, s32 flag, s32 a2, s32 a3) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot;

  /* FAKEMATCH: a one-pass block loads the work before the index is saved (one
   * reordered row). */
  do {
  } while (0);
  slot = &work->slots[index];
  slot->flag = flag;
  if (!GameFlag_Test(flag))
    Func_080d9104(index, a2, a3);
  else
    Func_080d8d68(index, a2, a3);
}
