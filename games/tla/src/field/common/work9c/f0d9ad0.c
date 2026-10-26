#include "work9c.h"
#include "edition.h"

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
// The Spanish and Italian far-call tables reach this function and the two it
// calls under the names of other entries.
#define Func_080d9ad0 Func_080d9e98
#define Func_080d9104 Func_080d897c
#define Func_080d8d68 Func_080ca6e4
#endif

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
