#include "types.h"

extern const u8 Data_0809f1a8[];

s8 Func_0808a8d0(s32 arg0)
{
  u8 *new_var;
  new_var = (u8 *) ((arg0 * 8) + (s32)Data_0809f1a8);
  return (s8) (*((u8 *) (2 + new_var)));
}
