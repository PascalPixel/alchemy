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
extern u8 gMainRoutineWork[];
extern u8 Func_020029a4[];

void Func_020029c8(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 23 instructions
   * differ in tla-en. */
  s32 t2;

  Func_020045dc(0);
  Task_Add((s32)Func_020029a4, 0x480);
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 0x14) {
    t2 = GameFlag_Test(SPLIT_CONSTANT(0x900, 197));
    if (t2 == 0) {
      Func_02001cf0();
    }
  }
}
