#include "types.h"
#include "scene.h"
#include "abi/object/group/store_object_and_run_step7.h"
#include "global_cells.h"

void ObjectGroup_StoreObjectAndRunStep7(void *obj)
{
  s32 out1;
  s32 out0;
  void *state;
  state = *((void **)ADDR_03001EEC);
  *((void **)(((u8 *)state) + 0x7828)) = obj;
  Obj_Do(0);
  Obj_SetRect(obj, 7, (*((s32 *)(((u8 *)(*((void **)(((u8 *)state) + 0x7828)))) + 4))) ^ 1, 0, &out0, &out1);
 do { Obj_Check(); } while (0);
}
