#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_020045dc();
s32 Task_Add();
s32 GameFlag_Test();
s32 Func_02001cf0();
extern u8 gAdventure[];
extern u8 Func_020029a4[];

void Func_020029c8(void) {
  s32 t2;

  Call1(Func_020045dc, 0);
  Call2(Task_Add, (s32)Func_020029a4, 0x480);
  if (FIELD_AT(gAdventure, s16, 482) == 0x14) {
    t2 = Value1(GameFlag_Test, 0x9c5);
    if (t2 == 0) {
      Func_02001cf0();
    }
  }
}
