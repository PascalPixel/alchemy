#include "types.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"
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

void Func_0200200c(void) {
  s32 t7, t8, t10, t13, t16, p3;

  if (FIELD_AT(&gMainRoutineWork, s16, 482) != 4) {
    p3 = FIELD_AT(&gMainRoutineWork, s16, 482);
    goto L_0200a02e;
  }
  Func_02005868();
  Func_020034b4();
  Import_EventSetExitRequest(4);
  goto L_0200a07c;
L_0200a02e:
  if (p3 != 5)
    goto L_0200a042;
  Func_02005868();
  Func_020037c8();
  Import_EventSetExitRequest(5);
  goto L_0200a07c;
L_0200a042:
  t7 = GameFlag_Test(0x9ff);
  if (t7 == 0)
    goto L_0200a07c;
  t8 = GameFlag_Test(0x283);
  if (t8 == 0)
    goto L_0200a064;
  Func_02001754(0);
L_0200a064:
  t10 = GameFlag_Test(0x385);
  if (t10 == 0)
    goto L_0200a078;
  Func_02002db0();
  goto L_0200a07c;
L_0200a078:
  Func_02002db4();
L_0200a07c:
  t13 = ((s32)Actor_GetById(gMainRoutineWork.currentOwner));
  FIELD_AT(t13, u8, 35) = (64 | FIELD_AT(t13, u8, 35));
  Func_02001fc4(1);
  Func_020006b4(10);
  t16 = GameFlag_Test(0x9fc);
  if (t16 == 0)
    goto L_0200a0bc;
  Actor_SetPosition(10, PIXELS(136), PIXELS(328));
L_0200a0bc:
}
