// Draft of overlay 6b3's Func_02000318, translated from its listing: differ
// 125 of 426. Left: its shape: 421 instructions against the listing's 426.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 FieldEvent_WaitFrames();
s32 Func_02000098();
s32 Func_02000244();
s32 Func_020007c4();
s32 Func_020007cc();
s32 Func_02000824();
s32 Func_02000844();
s32 Func_0200084c();
s32 Func_02000854();
s32 Func_0200085c();
s32 Func_02000864();
s32 Func_0200086c();
s32 GameFlag_ClearBit();
s32 Actor_GetById();
s32 WindowSystem_InitField();
s32 WaitFrames();
extern u8 MsgFinallyWeVe[];
extern u8 gBgScroll[];
extern u8 gData_02000a28[];
extern u8 gInput[];
extern u8 gMapCellBuffer[];
extern u8 gAdventure[];
extern u8 gRenderOamEnabled[];

s32 Func_02000318(void) {
  s32 v_r6, v_r7, v_r5, t0, t1, t2, t5, t90, t103, t105;

  t0 = *(s32 *)((s32)gAdventure + 0x214);
  v_r6 = ((s32)gAdventure + 0x22c);
  t1 = *(u8 *)v_r6;
  t2 = Value1((s32 (*)())Actor_GetById, t0);
  *(u8 *)(t2 + 85) = 0;
  *(u8 *)v_r6 = 1;
  Func_02000844();
  WindowSystem_InitField();
  t5 = FIELD_AT(0x3000000, s32, 60);
  FIELD_AT(t5, u8, 6) = 1;
  *(u8 *)(t5 + 0x138c) = 1;
  *(s32 *)(s32)gData_02000a28 = (s32)MsgFinallyWeVe;
  Func_02000098();
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Func_02000244, 5, 0, 40);
  Call3((void (*)())Func_02000244, 1, 1, 40);
  Call3((void (*)())Func_02000244, 6, 0, 40);
  Call3((void (*)())Func_02000244, 2, 1, 40);
  Call3((void (*)())Func_02000244, 3, 0, 30);
  Call3((void (*)())Func_02000244, 7, 1, 30);
  Call3((void (*)())Func_02000244, 56, 0, 40);
  Call3((void (*)())Func_02000244, 60, 1, 30);
  Call3((void (*)())Func_02000244, 61, 1, 40);
  Call3((void (*)())Func_02000244, 0, 0, 40);
  Call3((void (*)())Func_02000244, 62, 1, 30);
  Call3((void (*)())Func_02000244, 0, 0, 30);
  Call3((void (*)())Func_02000244, 0, 0, 30);
  Call3((void (*)())Func_02000244, 5, 1, 40);
  Call3((void (*)())Func_02000244, 4, 0, 30);
  Call3((void (*)())Func_02000244, 1, 1, 30);
  Call1((void (*)())Func_0200086c, 67);
  FIELD_AT((s32)gBgScroll, u16, 14) = 16;
  FIELD_AT((s32)gBgScroll, u16, 10) = 16;
  Call3((void (*)())Func_0200084c, 121, (s32)gMapCellBuffer, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 100);
  Call3((void (*)())Func_02000244, 60, 2, 30);
  Call3((void (*)())Func_02000244, 61, 2, 40);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 5, 2, 40);
  Call1((void (*)())Func_02000854, 120);
  Call1((void (*)())WaitFrames, 1);
  Func_0200085c();
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Func_0200084c, 122, (s32)gMapCellBuffer, 120);
  Call1((void (*)())FieldEvent_WaitFrames, 120);
  Call3((void (*)())Func_02000244, 5, 2, 30);
  Call3((void (*)())Func_02000244, 7, 2, 30);
  Call3((void (*)())Func_02000244, 56, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 40);
  Call3((void (*)())Func_02000244, 4, 2, 30);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 6, 2, 30);
  Call3((void (*)())Func_02000244, 5, 2, 30);
  Call3((void (*)())Func_02000244, 3, 2, 40);
  Call3((void (*)())Func_02000244, 1, 2, 30);
  Call1((void (*)())Func_0200086c, 78);
  Call3((void (*)())Func_02000244, 0x8040, 2, 30);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 0x8041, 2, 30);
  Call3((void (*)())Func_02000244, 0, 2, 30);
  Call3((void (*)())Func_02000244, 0x8045, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 30);
  Call3((void (*)())Func_0200084c, 123, (s32)gMapCellBuffer, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call1((void (*)())Func_0200086c, 73);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Func_02000244, 64, 2, 30);
  Call3((void (*)())Func_02000244, 65, 2, 30);
  Call3((void (*)())Func_02000244, 67, 2, 30);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 70, 2, 30);
  Call3((void (*)())Func_02000244, 68, 2, 30);
  Call3((void (*)())Func_02000244, 56, 2, 30);
  Call3((void (*)())Func_02000244, 1, 2, 40);
  Call3((void (*)())Func_02000244, 56, 2, 40);
  Call1((void (*)())Func_02000854, 200);
  Call1((void (*)())WaitFrames, 1);
  Func_0200085c();
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call3((void (*)())Func_02000244, 62, 2, 20);
  Call3((void (*)())Func_02000244, 0, 2, 40);
  Call3((void (*)())Func_02000244, 66, 2, 20);
  Call3((void (*)())Func_02000244, 62, 2, 30);
  Call3((void (*)())Func_02000244, 66, 2, 40);
  Call3((void (*)())Func_0200084c, 0, (s32)gMapCellBuffer, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  FIELD_AT((s32)gBgScroll, u16, 14) = 0;
  FIELD_AT((s32)gBgScroll, u16, 10) = 0;
  Call1((void (*)())Func_02000864, 0);
  Call3((void (*)())Func_0200084c, 125, (s32)gMapCellBuffer, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  v_r7 = t1;
  goto L_02008692;
L_0200868c:
  Call1((void (*)())WaitFrames, 1);
L_02008692:
  t90 = FIELD_AT((s32)gInput, s32, 4);
  if (t90 == 0)
    goto L_0200868c;
  Call3((void (*)())Func_0200084c, 126, (s32)gMapCellBuffer, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 120);
  Call1((void (*)())Func_02000854, 120);
  Call1((void (*)())WaitFrames, 1);
  Func_0200085c();
  Call1((void (*)())FieldEvent_WaitFrames, 180);
  Call3((void (*)())Func_0200084c, 127, (s32)gMapCellBuffer, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 0x12c);
  Call3((void (*)())Func_0200084c, 128, (s32)gMapCellBuffer, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 180);
  Call1((void (*)())Func_0200086c, 78);
  *(u8 *)((s32)gAdventure + 0x22c) = v_r7;
  Call1((void (*)())FieldEvent_WaitFrames, 0x12c);
  t103 = FIELD_AT((s32)gInput, s32, 4);
  v_r5 = 0;
  if (t103 != v_r5)
    goto L_0200871e;
L_02008704:
  Call1((void (*)())WaitFrames, 1);
  v_r5 = (v_r5 + 1);
  if (v_r5 > 0x2a2f)
    goto L_0200871e;
  t105 = FIELD_AT((s32)gInput, s32, 4);
  if (t105 == 0)
    goto L_02008704;
L_0200871e:
  Call3((void (*)())Func_0200084c, 0, (s32)gMapCellBuffer, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  *(u16 *)0x4000050 = 0;
  *(u16 *)0x4000000 = 0x1040;
  Func_020007cc();
  Func_020007c4();
  *(u8 *)(s32)gRenderOamEnabled = 1;
  Call1((void (*)())GameFlag_ClearBit, 0x17c);
  Call2((void (*)())Func_02000824, 1, 2);
  return 0;
}
