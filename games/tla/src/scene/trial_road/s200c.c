#include "types.h"
#include "main_routine_work.h"
#include "call.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020006b4();
void Func_02001754();
void Func_02001fc4();
void Func_02002db0();
void Func_02002db4();
void Func_020034b4();
void Func_020037c8();
void Func_02005868();
void Func_020058e0();
void Func_02005990();
s32 GameFlag_Test();
s32 Actor_GetById();

void Func_0200200c(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 16 of 72 instructions
   * differ in tla-en. */
  s32 t7, t8, t10, t13, t16, p3;

  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 4) {
    p3 = FIELD_AT(&gMainRoutineWork, s16, 482);
    goto L_0200a02e;
  }
  Func_02005868();
  Func_020034b4();
  Func_02005990(4);
  goto L_0200a07c;
L_0200a02e:
  if (p3 != 5)
    goto L_0200a042;
  Func_02005868();
  Func_020037c8();
  Func_02005990(5);
  goto L_0200a07c;
L_0200a042:
  t7 = GameFlag_Test(SPLIT_CONSTANT(0x900, 255));
  if (t7 == 0)
    goto L_0200a07c;
  t8 = GameFlag_Test(SPLIT_CONSTANT(0x184, 255));
  if (t8 == 0)
    goto L_0200a064;
  Func_02001754(0);
L_0200a064:
  t10 = GameFlag_Test(SPLIT_CONSTANT(0x300, 133));
  if (t10 == 0)
    goto L_0200a078;
  Func_02002db0();
  goto L_0200a07c;
L_0200a078:
  Func_02002db4();
L_0200a07c:
  t13 = Actor_GetById(gMainRoutineWork.currentOwner);
  FIELD_AT(t13, u8, 35) = (64 | FIELD_AT(t13, u8, 35));
  Func_02001fc4(1);
  Func_020006b4(10);
  t16 = GameFlag_Test(SPLIT_CONSTANT(0x900, 252));
  if (t16 == 0)
    goto L_0200a0bc;
  Call3(Func_020058e0, 10, PIXELS(136), PIXELS(328));
L_0200a0bc:
}
