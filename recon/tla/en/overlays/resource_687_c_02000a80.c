// Draft, differ 193 of 188: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gFrameCount[];
extern u8 gData_020022c4[];
extern u8 gData_020022be[];
extern u8 gData_020022c0[];
extern u8 gData_020022bc[];
extern u8 gData_02002118[];
extern u8 gData_02002110[];
s32 Actor_GetById();
s32 Func_0200034c();
s32 Random16Far();
void Func_02002054();
void Func_0200015c();

s32 Func_02000a80(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r11, v0, v1, r6, r7, r4, r2, r1, r5, r0, r10, r3, r9, v2, r8, v3;
  s32 stack[27];

  /* sub (s32)stack, #108 */
  r11 = (s32)Ram_HeapSlots->eventWork;
  v0 = Actor_GetById(8);
  v1 = Func_0200034c(0, (FIELD_AT(v0, s32, 8)) >> 20,
                     (FIELD_AT(v0, s32, 16)) >> 20);
  stack[4] = (((v1 >> 8) - 1) << 16) >> 16;
  r6 = (s32)gFrameCount;
  r7 = (s32)gData_020022c4;
  if ((u32)((FIELD_AT(r6, s32, 0)) - (FIELD_AT(r7, s32, 0))) > (u32)50) {
    FIELD_AT((s32)gData_020022be, u16, 0) = 0;
    FIELD_AT((s32)gData_020022c0, u16, 0) = 0;
  }
  r4 = FIELD_AT(r11, s16, 340);
  r2 = FIELD_AT((s32)gData_020022bc, u16, 0);
  r1 = FIELD_AT(r11, u16, 340);
  if ((FIELD_AT((s32)gData_020022bc, u16, 0)) == r4)
    goto L_02008b5e;
  r5 = 0;
  r0 = (s32)gData_020022c0;
  if ((*(s16 *)(((s32)gData_020022c0) + r5)) != 0)
    goto L_02008b0c;
  FIELD_AT(r0, u16, 0) = r2 - r1;
  goto L_02008b0c;
L_02008b0c:
  if ((FIELD_AT(r0, s16, 0)) == 7) {
    FIELD_AT(r0, u16, 0) = -1;
  }
  r5 = 0;
  if ((*(s16 *)(r0 + r5)) == -7) {
    FIELD_AT(r0, u16, 0) = 1;
  }
  r5 = 0;
  r5 = (s32)gData_020022be;
  r1 = *(s16 *)(r0 + r5);
  if ((r2 - r4) == (*(s16 *)(r0 + r5)))
    goto L_02008b4c;
  if ((r4 - r2) != ((r1 << 3) - r1))
    goto L_02008b58;
  goto L_02008b4c;
L_02008b4c:
  FIELD_AT(r5, u16, 0) = (FIELD_AT(r5, u16, 0)) + 1;
  FIELD_AT(r7, s32, 0) = FIELD_AT(r6, s32, 0);
  goto L_02008b5e;
L_02008b58:
  FIELD_AT(r5, u16, 0) = 0;
  FIELD_AT(r0, u16, 0) = 0;
L_02008b5e:
  FIELD_AT((s32)gData_020022bc, u16, 0) = FIELD_AT(r11, u16, 340);
  if ((u32)(FIELD_AT((s32)gData_020022be, u16, 0)) <= (u32)3)
    goto L_02008c0c;
  r7 = FIELD_AT((s32)gFrameCount, s32, 0);
  r7 = r7 & 3;
  if (r7 != 0)
    goto L_02008c0c;
  r10 = (s32)((u8 *)stack + 60);
  r2 = r10;
  r3 = (s32)gData_02002118;
  goto L_02008b9c;
L_02008b9c:
  /* ldmia r3!, {r1, r4, r5} */
  /* stmia r2!, {r1, r4, r5} */
  /* ldmia r3!, {r0, r1, r4} */
  /* stmia r2!, {r0, r1, r4} */
  /* ldmia r3!, {r0, r1, r5} */
  /* stmia r2!, {r0, r1, r5} */
  /* ldmia r3!, {r0, r4, r5} */
  /* stmia r2!, {r0, r4, r5} */
  r9 = 20 + (s32)stack;
  FIELD_AT(r9, u16, 24) = 0x2a1;
  FIELD_AT(r9, s32, 0) = 2;
  v2 = Random16Far();
  r6 = v2;
  r8 = 31;
  r6 = r6 & 31;
  v3 = Random16Far();
  r4 = r8;
  r5 = v3;
  r5 = r5 & r4;
  Call1((void (*)())Func_02002054, 246);
  r6 = r6 << 16;
  stack[0] = 0x13333;
  r5 = r5 << 16;
  Func_0200015c((*(s32 *)(r10 + ((u32)(stack[4] << 16) >> 13))) + r6, 0,
                (*(s32 *)(r10 + (((u32)(stack[4] << 16) >> 13) + 4))) + r5, 0,
                0x13333, r7, 0x120001, r9);
L_02008c0c:
  r4 = stack[4];
  r0 = (s32)gData_020022be;
  r3 = *(u8 *)(((s32)gData_02002110) + ((u32)(r4 << 16) >> 16));
  if ((FIELD_AT((s32)gData_020022be, u16, 0)) !=
      (*(u8 *)(((s32)gData_02002110) + ((u32)(r4 << 16) >> 16))))
    goto L_02008c30;
  FIELD_AT(r11, u16, 362) = 200;
  FIELD_AT(r0, u16, 0) = 0;
  FIELD_AT((s32)gData_020022c0, u16, 0) = 0;
  r3 = 0;
L_02008c30:
  /* add (s32)stack, #108 */
  return r0;
}
