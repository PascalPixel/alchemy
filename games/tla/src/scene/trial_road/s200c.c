#include "types.h"
#include "adventure.h"
#include "call.h"
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
s32 Object_GetById();

void Func_0200200c(void) {
  s32 t7, t8, t10, t13, t16, p3;

  if (FIELD_AT(&gAdventure, s16, 482) != 4) {
    p3 = FIELD_AT(&gAdventure, s16, 482);
    goto L_0200a02e;
  }
  Func_02005868();
  Func_020034b4();
  Call1(Func_02005990, 4);
  goto L_0200a07c;
L_0200a02e:
  if (p3 != 5)
    goto L_0200a042;
  Func_02005868();
  Func_020037c8();
  Call1(Func_02005990, 5);
  goto L_0200a07c;
L_0200a042:
  t7 = Value1(GameFlag_Test, 0x9ff);
  if (t7 == 0)
    goto L_0200a07c;
  t8 = Value1(GameFlag_Test, 0x283);
  if (t8 == 0)
    goto L_0200a064;
  Call1(Func_02001754, 0);
L_0200a064:
  t10 = Value1(GameFlag_Test, 0x385);
  if (t10 == 0)
    goto L_0200a078;
  Func_02002db0();
  goto L_0200a07c;
L_0200a078:
  Func_02002db4();
L_0200a07c:
  t13 = Value1(Object_GetById, gAdventure.currentOwner);
  FIELD_AT(t13, u8, 35) = (64 | FIELD_AT(t13, u8, 35));
  Call1(Func_02001fc4, 1);
  Call1(Func_020006b4, 10);
  t16 = Value1(GameFlag_Test, 0x9fc);
  if (t16 == 0)
    goto L_0200a0bc;
  Call3(Func_020058e0, 10, 0x880000, 0x1480000);
L_0200a0bc:
}
