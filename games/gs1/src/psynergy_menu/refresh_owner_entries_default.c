#include "types.h"

#define PsynergyMenu_RefreshOwnerEntriesDefault Func_080a1bcc

void Func_080a1bdc(s32 arg0, s32 arg1, s32 arg2);
void PsynergyMenu_RefreshOwnerEntriesDefault(void)
{
  int val;
  val = 0x28;
  Func_080a1bdc(0x6C, val, 8);
}
