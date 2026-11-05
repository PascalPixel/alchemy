// Draft of overlay 664's Func_02004cc8, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Actor_GetById();
s32 Func_02005e14();
s32 Func_02005f2c();
s32 OverlayObject_PrepareObject();
s32 Actor_SetSingleChildFlags();
s32 Import_ActorCreate();
s32 Func_02005d5c();
s32 Func_02005d8c();
s32 Func_02005f94();
s32 Event_StartScreenTransition();
s32 wait();
s32 Func_02005dec();
s32 FieldEvent_Wait();
s32 Actor_ArmCallback();
s32 Map_GetTerrainHeight();
s32 OverlayObject_WaitUntilIdle();
s32 Func_02005d94();
s32 Actor_SetModeById();
s32 Func_02005d7c();
s32 Event_WaitTransitionFrames();
s32 Func_02005e1c();
extern u8 gMainRoutineWork[];
extern u8 gFrameCount[];

void Func_02004cc8(void) {
  s32 v_r0, v_r1, v_r10, v_r11, v_r2, v_r3, v_r5, v_r6, v_r7, v_r8, v_r9;

  v_r7 = v_r11;
  v_r6 = v_r10;
  v_r5 = v_r9;
  v_r7 = v_r8;
  v_r3 = (s32)gMainRoutineWork + 0x214;
  v_r0 = FIELD_AT(v_r3, s32, 0);
  v_r0 = Actor_GetById(v_r0, 0x214);
  v_r7 = v_r0;
  Func_02005e14();
  Call1((void (*)())Func_02005f2c, 0);
  v_r0 = OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 0x11e);
  v_r2 = 85 + v_r7;
  v_r5 = 0;
  FIELD_AT(v_r2, u8, 0) = v_r5;
  v_r11 = v_r0;
  v_r10 = v_r2;
  Call3((void (*)())Actor_SetSingleChildFlags, v_r7, 0, v_r2);
  FIELD_AT(v_r7, s32, 48) = 0x18000;
  FIELD_AT(v_r7, s32, 52) = 0x18000;
  v_r2 = FIELD_AT(v_r7, s32, 12);
  v_r2 = v_r2 + -0x80000;
  v_r1 = FIELD_AT(v_r7, s32, 8);
  v_r3 = FIELD_AT(v_r7, s32, 16);
  v_r0 = Import_ActorCreate(0x118, v_r1, v_r2, v_r3);
  v_r6 = v_r0;
  v_r0 = Func_02005d5c();
  Call2((void (*)())Actor_SetSingleChildFlags, v_r6, 0);
  v_r3 = v_r6 + 85;
  FIELD_AT(v_r3, u8, 0) = v_r5;
  FIELD_AT(v_r6, s32, 48) = 0x16666;
  FIELD_AT(v_r6, s32, 52) = 0x16666;
  v_r2 = FIELD_AT(v_r7, s32, 12);
  v_r1 = FIELD_AT(v_r7, s32, 8);
  v_r2 = v_r2 + 0x500000;
  v_r3 = FIELD_AT(v_r7, s32, 16);
  v_r0 = Func_02005d8c(v_r7, v_r1, v_r2, v_r3);
  v_r2 = FIELD_AT(v_r6, s32, 12);
  v_r2 = v_r2 + 0x480000;
  v_r1 = FIELD_AT(v_r6, s32, 8);
  v_r3 = FIELD_AT(v_r6, s32, 16);
  v_r0 = Func_02005d8c(v_r6, v_r1, v_r2, v_r3);
  Call1((void (*)())Func_02005f94, 137);
  Event_StartScreenTransition();
  goto L_ja_0200cda0;
L_ja_0200cd80:
  ;
  v_r2 = FIELD_AT((s32)gFrameCount, s32, 0);
  v_r3 = FIELD_AT((s32)gFrameCount, u8, 0);
  v_r3 = v_r3 << 12;
  FIELD_AT(v_r7, u16, 6) = v_r3;
  v_r2 = v_r2 & 1;
  v_r3 = 2 << v_r2;
  v_r2 = FIELD_AT(v_r8, u8, 0);
  v_r3 = v_r3 ^ v_r2;
  FIELD_AT(v_r8, u8, 0) = v_r3;
  v_r0 = wait(1, v_r8, v_r2, v_r3);
L_ja_0200cda0:
  ;
  v_r0 = Func_02005dec(v_r7);
  v_r2 = 35 + v_r7;
  v_r8 = v_r2;
  if (v_r0 == 0)
    goto L_ja_0200cd80;
  Call1((void (*)())Func_02005f94, 0x120);
  Call1((void (*)())FieldEvent_Wait, 10);
  v_r5 = (s32)gMainRoutineWork;
  v_r5 = v_r5 + 0x214;
  v_r0 = FIELD_AT(v_r5, s32, 0);
  Call4((void (*)())Actor_ArmCallback, v_r0, 0xc000, 0, 0x214);
  v_r2 = FIELD_AT(v_r6, s32, 16);
  v_r9 = -0x100000;
  v_r2 = v_r2 + v_r9;
  v_r1 = FIELD_AT(v_r6, s32, 8);
  v_r0 = Map_GetTerrainHeight(0, v_r1, v_r2, -0x100000);
  FIELD_AT(v_r7, s32, 12) = v_r0;
  FIELD_AT(v_r7, s32, 20) = v_r0;
  v_r0 = Func_02005d5c(v_r7, 6);
  v_r0 = wait(6);
  Call1((void (*)())Func_02005f94, 152);
  v_r0 = Func_02005d5c(v_r7, 7);
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x8000;
  FIELD_AT(v_r7, s32, 40) = 0x40000;
  FIELD_AT(v_r10, u8, 0) = 2;
  FIELD_AT(v_r8, u8, 0) = 1;
  v_r3 = FIELD_AT(v_r7, s32, 16);
  v_r2 = FIELD_AT(v_r7, s32, 12);
  v_r3 = v_r3 + v_r9;
  v_r1 = FIELD_AT(v_r7, s32, 8);
  v_r0 = Func_02005d8c(v_r7, v_r1, v_r2, v_r3);
  v_r0 = OverlayObject_WaitUntilIdle(v_r7);
  Call1((void (*)())Func_02005d94, v_r7);
  v_r0 = FIELD_AT(v_r5, s32, 0);
  v_r0 = Actor_SetModeById(v_r0, 1);
  v_r0 = Func_02005d5c(v_r6, 5);
  v_r0 = wait(12);
  v_r0 = Func_02005d7c(v_r6);
  FIELD_AT(v_r10, u8, 0) = 3;
  v_r3 = FIELD_AT(v_r7, s32, 12);
  FIELD_AT(v_r7, s32, 20) = v_r3;
  Call2((void (*)())Actor_SetSingleChildFlags, v_r7, 1);
  v_r0 = Func_02005d7c(v_r11);
  v_r0 = wait(1);
  Event_WaitTransitionFrames();
  Func_02005e1c();
  v_r8 = v_r3;
  v_r9 = v_r5;
  v_r10 = v_r6;
  v_r11 = v_r7;
}
