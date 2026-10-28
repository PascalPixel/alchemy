#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const u8 gData_02001ac8[];

s32 Func_02000bb4(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov66bRoom1)
    FIELD_AT(Ram_HeapSlots->mapWork, const u8 *, 252) = gData_02001ac8;
  return 0;
}
