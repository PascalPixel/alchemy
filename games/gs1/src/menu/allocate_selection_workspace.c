#include "types.h"

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Menu_AllocateSelectionWorkspace(void)
{
  void *work;
  void *p;
  unsigned int zero;
  work = Runtime_AllocateBlock(0x13, 0x1004);
  p = work;
  zero = 0;
  if (1)
  {
  }
  *((s16 *) (((u8 *) p) + 0x46)) = zero;
  *((s16 *) (((u8 *) p) + 0x352)) = zero;
}
