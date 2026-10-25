// Draft, differ 179 of 167: translated from its listing; names and structure
// still to do.
#include "types.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Object_GetById();
s32 GameFlag_Test();
s32 Func_02002dfc();
void GameFlag_SetBit();
void GameFlag_ClearBit();
void Scheduler_RemoveCallbackFar();
s32 Func_02002e24();
s32 Func_02002eec();

s32 Func_02001f7c(void) {
  s32 r5, r10, v0, r8, r7, v1, r6, v2, r0, r3, r1, r2, v3, v4, v5, v6, v7;
  s32 stack[2];

  r5 = 192;
  r5 = r5 << 18;
  /* sub (s32)stack, #8 */
  r10 = FIELD_AT((FIELD_AT(r5, s32, 108)), s32, 460);
  v0 = Object_GetById(9);
  r8 = (s32)&gPartyState;
  r7 = v0;
  v1 = Object_GetById(FIELD_AT(r8, s32, 532));
  r6 = v1;
  r5 = FIELD_AT(r5, s32, 108);
  v2 = GameFlag_Test(0x104);
  r0 = v2;
  if (v2 == 0)
    goto L_02009fce;
  r3 = FIELD_AT(r5, s16, 340);
  if ((FIELD_AT(r5, s16, 340)) == 40)
    goto L_02009fce;
  goto L_0200a0e0;
L_02009fce:
  FIELD_AT(r7, s32, 16) = (FIELD_AT(r7, s32, 16)) + 52428;
  r0 = (FIELD_AT(r7, s32, 16)) + 52428;
  r1 = FIELD_AT(r7, s32, 16);
  r2 = (FIELD_AT(r6, s32, 8)) >> 20;
  if ((FIELD_AT(r8, u8, 530)) == 5)
    goto L_0200a02a;
  if (r2 <= 12)
    goto L_0200a02a;
  if (r2 > 15)
    goto L_0200a02a;
  r2 = r1 + 1625292;
  r3 = FIELD_AT(r6, s32, 16);
  if ((r1 + 1625292) < (FIELD_AT(r6, s32, 16)))
    goto L_0200a010;
  if (r3 < r0)
    goto L_0200a010;
  FIELD_AT(r6, s32, 16) = r2;
  FIELD_AT(r10, s32, 16) = (FIELD_AT(r7, s32, 16)) + 1572864;
L_0200a010:
  r2 = FIELD_AT(r6, s32, 16);
  r3 = FIELD_AT(r7, s32, 16);
  if ((FIELD_AT(r7, s32, 16)) < (FIELD_AT(r6, s32, 16)))
    goto L_0200a02a;
  r1 = -0x100000;
  r3 = r3 - 1048576;
  if (r2 < (r3 - 1048576))
    goto L_0200a02a;
  FIELD_AT(r6, s32, 16) = r3;
  FIELD_AT(r10, s32, 16) = (FIELD_AT(r7, s32, 16)) + r1;
L_0200a02a:
  r5 = (FIELD_AT(r7, s32, 16)) >> 20;
  r3 = (r5 - 31) << 16;
  if ((u32)((r5 - 31) << 16) > (u32)262144)
    goto L_0200a064;
  v3 = GameFlag_Test(0x146);
  r0 = v3;
  if (v3 != 0)
    goto L_0200a064;
  Call6((void (*)())Func_02002dfc, 35, 83, 5, 4, 12, 96);
  Call1((void (*)())GameFlag_SetBit, 0x146);
  r0 = v4;
L_0200a064:
  if (r5 != 37)
    goto L_0200a090;
  v5 = GameFlag_Test(0x146);
  r0 = v5;
  if (v5 == 0)
    goto L_0200a090;
  Call6((void (*)())Func_02002dfc, 40, 83, 5, 4, 12, 96);
  Call1((void (*)())GameFlag_ClearBit, 0x146);
  r0 = v6;
L_0200a090:
  if (r5 <= 41)
    goto L_0200a0e0;
  Scheduler_RemoveCallbackFar((s32)Func_02001f7c);
  Call3((void (*)())Func_02002e24, -1, -1, 0xe666);
  Call6((void (*)())Func_02002dfc, 41, 48, 3, 2, 13, 41);
  Call1((void (*)())GameFlag_SetBit, 0x201);
  Call1((void (*)())GameFlag_ClearBit, 0x17d);
  Call1((void (*)())GameFlag_ClearBit, 0x161);
  v7 = Func_02002eec(208);
  r0 = v7;
L_0200a0e0:
  /* add (s32)stack, #8 */
  return r0;
}
