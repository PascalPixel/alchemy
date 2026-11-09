#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern char SceneId_Ov6A6Room1;

void Func_020000ac(void);

s32 Func_020011ec(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6A6Room1 && GameFlag_Test(0xa5f))
    Func_020000ac();
  return 0;
}
