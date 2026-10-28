#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Task_Add();
extern u8 Func_020049e4[];

void Func_02004d00(void) {
  Task_Add((s32)Func_020049e4, 0x480);
}
