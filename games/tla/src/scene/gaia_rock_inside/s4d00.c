#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "task.h"
#include "event_inline.h"
extern u8 Func_020049e4[];

void Func_02004d00(void) {
  Task_AddFrame((s32)Func_020049e4, TASK_PASS_VBLANK);
}
