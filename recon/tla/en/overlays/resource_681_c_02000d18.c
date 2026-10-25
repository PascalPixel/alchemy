// Draft, differ 57 of 188: once any of twelve cells is filled (flag 0x9c4 still
// clear), person 15 walks off, the doorway cells open, four boulders are placed
// and the flag is set. Translated from its listing; names and structure still
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

extern u8 gData_020051d8[];
extern u8 gData_020051b8[];
extern u8 gData_02005118[];
extern u8 gData_02005168[];
void Func_02004ff4();
s32 GameFlag_Test();
s32 Func_02004dec();
s32 Object_GetById();
void Func_02004e5c();
void Func_02004fcc();
void Func_02004f2c();
void ObjectMotion_EnableActionAndSetCallback();
void FieldEvent_WaitFrames();
void Func_02004e04();
void Func_02004dfc();
s32 OverlayObject_PrepareObject();
void Func_02005014();
s32 Func_02004ebc();
void Func_02004fdc();
void GameFlag_SetBit();
s32 Func_02004f6c();

s32 Func_02000d18(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r5_1, r8_1, r7_1, r6_1, r8_2, r7_2, r6_2, r5_2, r5_3, r5_4, r6_3, r10_1,
      r8_3, r5_5;
  s32 v0, r0, r5, r8, r7, r6, r3, v1, v2, v3, r10, v4, v5;
  s32 stack[3];

  /* sub (s32)stack, #12 */
  Call7((void (*)())Func_02004ff4, 104, 20, 1, 8, 8, 117, 20);
  v0 = GameFlag_Test(0x9c4);
  r0 = v0;
  if (v0 == 0)
    goto L_02008d46;
  goto L_02008ebc;
L_02008d46:
  r5_1 = 128;
  r5_1 = r5_1 << 9;
  r8_1 = 0;
  r7_1 = 2;
  r6_1 = r5_1;
  r3 = 0;
L_02008d54:
  v1 = Func_02004dec(2, (*(s16 *)(((s32)gData_020051d8) + (r3 << 2))) << 20,
                     (*(s16 *)(((s32)gData_020051d8) + r7_1)) << 20);
  r0 = v1;
  if (v1 == 0) {
    r6_1 = r6_1 + 65536;
    r8_2 = r6_1 >> 16;
    r0 = 0x10000;
  }
  r5_1 = r5_1 + 65536;
  r7_1 = r7_1 + 4;
  r3 = r5_1 >> 16;
  if ((r5_1 >> 16) <= 11)
    goto L_02008d54;
  r3 = r8_2;
  if (r8_2 > 0)
    goto L_02008d8e;
  goto L_02008ebc;
L_02008d8e:
  v2 = Object_GetById(15);
  r7_2 = v2;
  r3 = (s32)&gPartyState;
  r6_2 = r3 + 532;
  v3 = Object_GetById(FIELD_AT(r6_2, s32, 0));
  r5_2 = v3;
  Func_02004e5c();
  Call1((void (*)())Func_02004fcc, 0);
  if (((FIELD_AT(r5_2, s32, 16)) >> 19) <= 53)
    goto L_02008dde;
  r3 = (FIELD_AT(r5_2, s32, 8)) >> 19;
  if (((FIELD_AT(r5_2, s32, 8)) >> 19) <= 83)
    goto L_02008dde;
  if (r3 > 91)
    goto L_02008dde;
  FIELD_AT(r5_2, s32, 48) = 0x20000;
  Value3((s32 (*)())Func_02004f2c, FIELD_AT(r6_2, s32, 0), 0x100, 0);
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback,
        FIELD_AT(r6_2, s32, 0), (s32)gData_020051b8);
L_02008dde:
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 15,
        (s32)gData_02005118);
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, 15,
        (s32)gData_02005168);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  stack[0] = 104;
  r5_3 = 20;
  Call6((void (*)())Func_02004e04, 117, 20, 7, 8, 104, r5_3);
  Call6((void (*)())Func_02004e04, 111, 12, 1, 6, 111, r5_3);
  Call6((void (*)())Func_02004dfc, 49, 84, 8, 8, 40, 84);
  r5_4 = 175;
  r6_3 = 143;
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  r6_3 = r6_3 << 1;
  r5_4 = r5_4 << 18;
  r10_1 = 0x1d40000;
  Call4((void (*)())OverlayObject_PrepareObject, r5_4, 0, 0x1d40000, r6_3);
  r8_3 = 0x1ea0000;
  r5_5 = 181;
  r5_5 = r5_5 << 18;
  Call4((void (*)())OverlayObject_PrepareObject, r5_5, 0, r8_3, r6_3);
  OverlayObject_PrepareObject(r5_5, 0, r10_1, r6_3);
  Call4((void (*)())OverlayObject_PrepareObject, r5_5, 0, r8_3, r6_3);
  Call1((void (*)())Func_02005014, 204);
  FIELD_AT(r7_2, u8, 85) = 3;
  FIELD_AT(FIELD_AT(r7_2, s32, 80), u8, 9) =
      (FIELD_AT(FIELD_AT(r7_2, s32, 80), u8, 9)) | 12;
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  v4 = Func_02004ebc(15, 0, 0);
  Func_02004fdc();
  Call1((void (*)())GameFlag_SetBit, 0x9c4);
  v5 = Func_02004f6c(9);
  r0 = v5;
L_02008ebc:
  /* add (s32)stack, #12 */
  return r0;
}
