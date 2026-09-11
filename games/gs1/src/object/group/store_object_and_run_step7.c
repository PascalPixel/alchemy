#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* object/group/store_object_and_run_step7.c */
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

/* battle/effects/runtime/init/fx_initialize_default_mode.c */
/* battle/effects/runtime/init/initialize_default_mode.c */
void BattleFx_InitializeDefaultMode(s32 arg0)
{
    Battle_Apply(arg0, 0);
}

/* battle/effects/runtime/init/initialize_mode_1.c */
void BattleFx_InitializeMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
