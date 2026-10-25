// Draft, differ 341 of 350: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gData_020026c0[];
extern u8 Func_020010a8[];
void Func_02001fac();
void Func_02002044();
void GameFlag_SetBit();
void ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
void ObjectMotion_ResetAndSetPositionInMode2();
void Func_02001fec();
void Camera_MoveWithinMapBounds();
void Func_02001ffc();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Func_02002054();
void Func_02001f74();
void WaitFrames();
void FieldEvent_WaitFrames();
s32 Random16Far();
void Func_0200015c();
s32 Engine_ObjectCreate();
void Func_02001f4c();
void Party_SetFields1eeAnd1f0();
s32 Func_0200200c();
s32 Func_02001fb4();

s32 Func_02001108(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r8_1, r11_1, r9_1, r10_1, r5_1, r6_1, r7_1, r5_2, r5_3, r8_2, r5_4, r6_2,
      r8_3, r5_5, r6_3, r5_6;
  s32 v0, v1, r4, r8, r11, r9, r10, r5, r6, r7, r1, r0, r2, r3, ip, v2, v3, v4,
      v5, v6, v7, v8, lr;
  s32 stack[25];

  /* sub (s32)stack, #100 */
  Func_02001fac();
  Call1((void (*)())Func_02002044, 0);
  Call1((void (*)())GameFlag_SetBit, 0x8dd);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 8, 0x10000, 0x8000);
  v0 = Object_GetById(8);
  FIELD_AT(v0, u16, 6) = 0xc000;
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 8, 0x160, 0x108);
  Call2((void (*)())Func_02001fec, 0x10000, 0x2000);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1600000, -1, 0xc40000, 1);
  Func_02001ffc();
  Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 8);
  Call1((void (*)())Func_02002054, 78);
  Call1((void (*)())Func_02002054, 0x22c);
  FIELD_AT((s32)((u8 *)stack + 98), u16, 0) = 23755;
  /* stmia r3!, {r0, r1, r2} */
  Call6((void (*)())Func_02001f74, 49, 70, 19, 73, 6, 6);
  goto L_020091c0;
L_020091c0:
  Call1((void (*)())WaitFrames, 10);
  r4 = 160;
  r4 = r4 << 19;
  r4 = r4 + 98;
  r8_1 = 1;
  r11_1 = (s32)gData_020026c0;
  r9_1 = 0;
  r10_1 = r4;
L_020091dc:
  r4 = 31;
  r5_1 = r4;
  r5_1 = r5_1 & (FIELD_AT(r10_1, u16, 0));
  r6_1 = (u32)((*(s16 *)(r9_1 + r11_1)) << 16) >> 21;
  r7_1 = (u32)((*(s16 *)(r9_1 + r11_1)) << 16) >> 26;
  r6_1 = r6_1 & (31);
  r7_1 = r7_1 & (31);
  r1 = ((u32)((FIELD_AT(r10_1, u16, 0)) << 16) >> 21) & (31);
  r0 = ((u32)((FIELD_AT(r10_1, u16, 0)) << 16) >> 26) & (31);
  r2 = ((u32)((*(s16 *)(r9_1 + r11_1)) << 16) >> 16) & r4;
  r3 = r5_1;
  if ((u32)r5_1 >= (u32)(((u32)((*(s16 *)(r9_1 + r11_1)) << 16) >> 16) & r4))
    goto L_0200921c;
  r5_2 = r3 + 1;
  goto L_0200921c;
L_0200921c:
  r3 = (u32)(r5_2 << 16) >> 16;
  if ((u32)((u32)(r5_2 << 16) >> 16) > (u32)r2) {
    r4 = 255;
    r4 = r4 << 8;
    r4 = r4 + 255;
    r5_3 = ((r3 + r4) << 16) >> 16;
  }
  ip = r6_1;
  r3 = (u32)(r1 << 16) >> 16;
  if ((u32)((u32)(r1 << 16) >> 16) < (u32)r6_1) {
    r1 = ((r3 + 1) << 16) >> 16;
  }
  r3 = (u32)(r1 << 16) >> 16;
  if ((u32)((u32)(r1 << 16) >> 16) > (u32)ip) {
    r4 = 255;
    r4 = r4 << 8;
    r4 = r4 + 255;
    r1 = ((r3 + r4) << 16) >> 16;
  }
  ip = r7_1;
  r3 = (u32)(r0 << 16) >> 16;
  if ((u32)((u32)(r0 << 16) >> 16) < (u32)r7_1) {
    r0 = ((r3 + 1) << 16) >> 16;
  }
  r3 = (u32)(r0 << 16) >> 16;
  if ((u32)((u32)(r0 << 16) >> 16) > (u32)ip) {
    r4 = 255;
    r4 = r4 << 8;
    r4 = r4 + 255;
    r0 = ((r3 + r4) << 16) >> 16;
  }
  r8_1 = r8_1 + 1;
  r4 = 2;
  FIELD_AT(r10_1, u16, 0) = ((u32)(r5_3 << 16) >> 16) |
                            (((u32)(r0 << 16) >> 6) | ((u32)(r1 << 16) >> 11));
  r10_1 = r10_1 + r4;
  r9_1 = r9_1 + r4;
  if (r8_1 != 15)
    goto L_020091dc;
  if (lr == 0)
    goto L_020092b8;
  goto L_020091c0;
L_020092b8:
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  r4 = 0;
  r8_2 = r4;
L_020092c2:
  r3 = 63 & r8_2;
  if ((63 & r8_2) == 0) {
    Call1((void (*)())Func_02002054, 164);
  }
  r2 = r8_2;
  if ((u32)r8_2 > (u32)119)
    goto L_020092e2;
  if ((15 & r2) == 0)
    goto L_020092fe;
  goto L_02009344;
L_020092e2:
  if ((u32)r8_2 > (u32)239)
    goto L_020092f4;
  r4 = r8_2;
  if ((7 & r4) == 0)
    goto L_020092fe;
  goto L_02009344;
L_020092f4:
  if ((1 & r8_2) != 0)
    goto L_02009344;
L_020092fe:
  v2 = Random16Far();
  r5_4 = v2;
  v3 = Random16Far();
  r6_2 = 31;
  FIELD_AT((s32)((u8 *)stack + 56), s32, 4) = 7;
  r5_4 = r5_4 & r6_2;
  r5_4 = r5_4 << 16;
  r5_4 = r5_4 + 22020096;
  stack[0] = 0;
  stack[1] = 0;
  r4 = 176;
  r4 = r4 << 16;
  Func_0200015c(r5_4, 0, ((v3 & r6_2) << 16) + r4, 0, 0, 0, 0x10000,
                (s32)((u8 *)stack + 56));
  Call1((void (*)())WaitFrames, 1);
L_02009344:
  r8_2 = r8_2 + 1;
  r2 = 0x168;
  if (r8_2 != 0x168)
    goto L_020092c2;
  r8_3 = 0;
L_02009354:
  v4 = Random16Far();
  r5_5 = v4;
  v5 = Random16Far();
  FIELD_AT((s32)((u8 *)stack + 16), s32, 8) = 0x18000;
  FIELD_AT((s32)((u8 *)stack + 16), s32, 12) = 0x18000;
  FIELD_AT((s32)((u8 *)stack + 16), s32, 4) = 7;
  FIELD_AT((s32)((u8 *)stack + 16), s32, 0) = 1;
  r6_3 = 31;
  stack[0] = 0x10000;
  r5_5 = r5_5 & r6_3;
  r4 = 168;
  r4 = r4 << 17;
  stack[1] = 0;
  r5_5 = r5_5 << 16;
  r5_5 = r5_5 + r4;
  Func_0200015c(r5_5, 0, ((v5 & r6_3) << 16) + 11534336, 0, 0x10000, 0, 0xb0000,
                (s32)((u8 *)stack + 16));
  Call1((void (*)())WaitFrames, 1);
  r8_3 = r8_3 + 1;
  if (r8_3 != 16)
    goto L_02009354;
  v6 = Engine_ObjectCreate(159, 0x15c0000, 0, 0xb80000);
  r5_6 = v6;
  FIELD_AT(r5_6, s32, 108) = (s32)Func_020010a8;
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call2((void (*)())Func_02001f4c, r5_6, 1);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  r4 = 166;
  r4 = r4 << 1;
  r4 = r4 + 255;
  FIELD_AT((s32)(s32)&gPartyState + r4, u8, 0) = 2;
  Call2((void (*)())Party_SetFields1eeAnd1f0, 198, 4);
  v7 = Func_0200200c(12, 6);
  v8 = Func_02001fb4();
  /* add (s32)stack, #100 */
  return v8;
}
