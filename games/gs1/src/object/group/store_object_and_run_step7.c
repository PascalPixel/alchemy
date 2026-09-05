#include "types.h"
#include "global_cells.h"

#define ObjectGroup_StoreObjectAndRunStep7 Func_080e6948

volatile int Func_080cd594(s32);
s32 Func_080cdbc0();
s32 Func_080de2f8(void *, s32, s32, s32, s32 *, s32 *);
void ObjectGroup_StoreObjectAndRunStep7(void *obj)
{
  s32 out1;
  s32 out0;
  void *state;
  state = *((void **) ADDR_03001EEC);
  *((void **) (((u8 *) state) + 0x7828)) = obj;
  Func_080cd594(0);
  Func_080de2f8(obj, 7, (*((s32 *) (((u8 *) (*((void **) (((u8 *) state) + 0x7828)))) + 4))) ^ 1, 0, &out0, &out1);
 do { Func_080cdbc0(); } while (0);
}
