#include "types.h"
#include "adventure.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_0200012c();
void Func_02000168();
void Func_02004c80();
s32 GameFlag_Test();
s32 Map_GetTerrainHeight();
void Object_SetSpritePriority();

void Func_020001f8(s32 a0) {
  s32 t1, t2, t3, t4, v10, v11, v5, v6, v7, v8, v9;

  v5 = a0;
  v9 = (FIELD_AT(v5, s32, 8) >> 20);
  v10 = (FIELD_AT(v5, s32, 16) >> 20);
  t1 = Value3(Func_0200012c, 0, v9, v10);
  v8 = t1;
  t2 = Value3(Func_0200012c, 2, v9, v10);
  v6 = t2;
  v11 = (34 + v5);
  t3 = Value3(Map_GetTerrainHeight, FIELD_AT((34 + v5), u8, 0),
              FIELD_AT(v5, s32, 8), FIELD_AT(v5, s32, 16));
  v7 = (t3 >> 19);
  t4 = Value1(GameFlag_Test, FIELD_AT(v5, s16, 100));
  if (t4 == 0)
    goto L_02008268;
  v6 = (v6 & -0xff01);
  v8 = (v8 & -129);
  if ((FIELD_AT(v5, s32, 20) >> 19) == v7)
    goto L_02008282;
  v7 = (v7 + -4);
  goto L_02008282;
L_02008268:
  v6 = (v6 & 255);
  v6 = (v6 | 0xff00);
  v8 = (v8 | 128);
  v7 = (v7 + 4);
  Call2(Object_SetSpritePriority, v5, 2);
L_02008282:
  Call4(Func_02000168, 0, v9, v10, v8);
  Call4(Func_02000168, 2, v9, v10, v6);
  Call4(Func_02004c80, (v9 << 20), (v10 << 20), FIELD_AT(v11, u8, 0), v7);
}
