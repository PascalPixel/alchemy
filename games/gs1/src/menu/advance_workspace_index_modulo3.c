#include "types.h"

void Menu_AdvanceWorkspaceIndexModulo3(void *arg0)
{
  unsigned int zero;
  unsigned long cnt;
  cnt = 1 + (*((u16 *) (0x574 + ((u8 *) arg0))));
  zero = 0U;
  *((u16 *) (((u8 *) arg0) + 0x574)) = cnt;
  if (((u32) (cnt << 0x10)) >= (((unsigned long) 0x20000U) + 1))
  {
    *((u16 *) (((u8 *) arg0) + 0x574)) = zero;
  }
}
