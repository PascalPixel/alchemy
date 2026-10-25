// Draft of overlay 6af's Func_0200043c, translated from its listing: differ
// 117 of 163. Left: its shape: 160 instructions against the listing's 163.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Camera_MoveWithinMapBounds();
s32 FieldEvent_WaitFrames();
s32 Func_0200015c();
s32 Func_020003fc();
s32 Func_02000f9c();
s32 Func_02000fb4();
s32 Func_02001004();
s32 Func_02001014();
s32 Func_0200106c();
extern u8 gData_02001128[];

void Func_0200043c(s32 p0, s32 p1) {
  s32 sp_[1];
  s32 v_r11, v_r6, t1, t2, t8, t9, t11, t12, t14, t15;
  s32 t18, t19, t22, t23;

  v_r11 = (p1 << 16);
  Call2((void (*)())Func_02001004, 0x60000, 0xc000);
  v_r11 = (v_r11 >> 16);
  v_r6 = ((v_r11 << 4) + p0);
  t1 = *(s32 *)v_r6;
  t2 = FIELD_AT(v_r6, s32, 4);
  Call4((void (*)())Camera_MoveWithinMapBounds, ((t1 << 20) + 0x80000), -1,
        ((t2 << 20) + 0x80000), 1);
  Func_02001014();
  Call3((void (*)())Func_02000fb4, 0, 0x100000, 0x10000);
  Call3((void (*)())Func_02000fb4, -1, -1, 0xe666);
  Call4((void (*)())Func_0200106c, 0x237, 0, 0, 0);
  FIELD_AT(sp_, s32, 24) = 0x13333;
  FIELD_AT(sp_, s32, 28) = 0x18000;
  FIELD_AT(sp_, s32, 16) = 1;
  FIELD_AT(sp_, u16, 40) = 0x115;
  t8 = FIELD_AT(v_r6, s32, 4);
  FIELD_AT(sp_, s32, 44) = (s32)gData_02001128;
  t9 = *(s32 *)v_r6;
  Func_0200015c(((t9 << 20) + 0x80000), 0, ((t8 << 20) + 0x140000), 0, 0, 0, 0,
                ((s32)sp_ + 16));
  FIELD_AT(sp_, s32, 24) = 0x9999;
  t11 = *(s32 *)v_r6;
  t12 = FIELD_AT(v_r6, s32, 4);
  Func_0200015c(((t11 << 20) + 0xfffe0000), 0, ((t12 << 20) + 0x80000), 0, 0, 0,
                0, ((s32)sp_ + 16));
  t14 = *(s32 *)v_r6;
  t15 = FIELD_AT(v_r6, s32, 4);
  Func_0200015c(((t14 << 20) + 0x120000), 0, ((t15 << 20) + 0x80000), 0, 0, 0,
                0, ((s32)sp_ + 16));
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  t18 = *(s32 *)v_r6;
  t19 = FIELD_AT(v_r6, s32, 4);
  Call6((void (*)())Func_02000f9c, 6, 100, 3, 4, 3, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  t22 = *(s32 *)v_r6;
  t23 = FIELD_AT(v_r6, s32, 4);
  Call6((void (*)())Func_02000f9c, 3, 100, 3, 4, 3, 4);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Func_020003fc, p0, v_r11);
  return;
}
