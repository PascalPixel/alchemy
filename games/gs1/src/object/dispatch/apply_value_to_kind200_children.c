#include "types.h"
#include "global_cells.h"

#define ObjectDispatch_ApplyValueToKind200Children Func_08098294

void ObjectDispatch_ApplyValueToKind200Children(int arg0)
{
  s32 cnt;
  u8 *p;
  u8 kind;
  void *child;
  void *rec;
  void *obj;
  obj = *((void **) ADDR_03001E64);
  cnt = 0x3F;
  do
  {
    if (1)
    {
      if ((*((s32 *) (((u8 *) obj) + 0))) != 0)
      {
        kind = *(p = (u8 *) (((u8 *) obj) + 0x54));
        if (kind == 1)
        {
          child = *((void **) (((u8 *) obj) + 0x50));
          rec = *((void **) (((u8 *) child) + 0x28));
          if ((*((s16 *) (((u8 *) rec) + 0))) == 0xC8)
          {
            *((s8 *) (((u8 *) rec) + 5)) = arg0;
            *((u8 *) (((u8 *) child) + 0x25)) = kind;
          }
        }
      }
      cnt -= 1;
    }
    obj += 0x70;
  }
  while (cnt >= 0);
}
