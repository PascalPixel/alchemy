#include "types.h"

typedef int bool;
void Func_0800c3ec(void *arg0, unsigned int arg1)
{
  int new_var;
  if ((arg0 != ((void *) 0)) && ((0xF & (*((u8 *) (((u8 *) arg0) + 0x54)))) == 1))
  {
    new_var = 0x1E;
    *((s16 *) (((u8 *) (*((void **) (((u8 *) arg0) + 0x50)))) + new_var)) = arg1;
  }
}

