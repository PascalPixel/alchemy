// Draft, differ 503 of 418: lays out the maze path once (flag 0x3f0, not after
// flag 0x109): seeded by the given value, a random walk of fourteen steps from
// the middle of a 28 by 28 grid never touches its own path, retrying a step up
// to sixteen times and the whole walk after that; then for each step it records
// which step lies beyond each of its four sides, and marks the ends. Translated
// from its listing; names and structure still to do.
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

extern u8 gRandomState[];
extern u8 gData_020023c4[];
extern u8 IwramClearWords[];
s32 GameFlag_Test();
s32 Runtime_BumpAllocateAlternatePool();
s32 Random16Far();
void MemFree();
void GameFlag_SetBit();

s32 Func_020044f8(s32 a0, s32 a1) {
  s32 r6, r5, r11, v0, r0, v1, v2, r4, r10, r8, r7, r1, r9, v3, r3, r2, ip, v4,
      lr;
  s32 stack[2];

  r6 = (s32)gRandomState;
  r5 = a0;
  /* sub (s32)stack, #8 */
  stack[1] = FIELD_AT(r6, s32, 0);
  r11 = ((s32)gData_020023c4 + 0x288) + 4;
  v0 = GameFlag_Test(0x109);
  r0 = v0;
  if (v0 == 0)
    goto L_0200c524;
  goto L_0200c846;
L_0200c524:
  v1 = GameFlag_Test(0x3f0);
  r0 = v1;
  if (v1 == 0)
    goto L_0200c532;
  goto L_0200c846;
L_0200c532:
  FIELD_AT(r6, s32, 0) = r5;
L_0200c534:
  v2 = Runtime_BumpAllocateAlternatePool(0xc40);
  r5 = v2;
  Call2((void (*)())Iwram_ClearWords, v2, 0xc40);
  r4 = r5;
  r10 = 14;
  r8 = 14;
  r6 = 0;
  r4 = r4 + 112;
  r7 = 28;
  r1 = r5;
  r0 = r5;
L_0200c55a:
  r7 = r7 - 1;
  FIELD_AT(r0, u8, 0) = 99;
  r6 = r6 + 4;
  FIELD_AT(r4, u8, 0) = 99;
  FIELD_AT(r1, u8, 0) = 99;
  r4 = r4 + 112;
  *(u8 *)(r5 + (3136 + r6)) = 99;
  r1 = r1 + 4;
  r0 = r0 + 112;
  if (r7 >= 0)
    goto L_0200c55a;
  r7 = 1;
L_0200c57a:
  r9 = 0;
L_0200c57e:
  r9 = r9 + 1;
  if (r9 > 16)
    goto L_0200c534;
  stack[0] = r8;
  v3 = Random16Far();
  r6 = r10;
  r1 = stack[0];
  r0 = (u32)(v3 << 2) >> 16;
  if (((u32)(v3 << 2) >> 16) == 0) {
    if (r7 == 1)
      goto L_0200c57e;
    if (r7 == 13)
      goto L_0200c57e;
  }
  r3 = r8 << 3;
  if (r0 == 1)
    goto L_0200c5f8;
  if (r0 > 1)
    goto L_0200c5b6;
  if (r0 == 0)
    goto L_0200c5c0;
  goto L_0200c6a4;
L_0200c5b6:
  if (r0 == 2)
    goto L_0200c62e;
  if (r0 == 3)
    goto L_0200c668;
  goto L_0200c6a4;
L_0200c5c0:
  r2 = (((r3 - r8) << 2) + r10) << 2;
  if ((*(s8 *)(r5 + (((((r3 - r8) << 2) + r10) << 2) + 112))) != 0)
    goto L_0200c57e;
  if ((*(s8 *)(r5 + (r2 + 224))) != 0)
    goto L_0200c57e;
  if ((*(s8 *)(r5 + (r2 + 108))) != 0)
    goto L_0200c57e;
  if ((*(s8 *)(r5 + (r2 + 116))) != 0)
    goto L_0200c57e;
  r1 = r8 + 1;
  goto L_0200c6a4;
L_0200c5f8:
  r2 = (((r3 - r8) << 2) + r10) << 2;
  if ((*(s8 *)(r5 + (((((r3 - r8) << 2) + r10) << 2) - 4))) != 0)
    goto L_0200c57e;
  if ((*(s8 *)(r5 + (r2 - 8))) != 0)
    goto L_0200c57e;
  if ((*(s8 *)(r5 + (r2 - 116))) != 0)
    goto L_0200c57e;
  if ((*(s8 *)(r5 + (r2 + 108))) != 0)
    goto L_0200c57e;
  r6 = r10;
  r6 = r6 - 1;
  goto L_0200c6a4;
L_0200c62e:
  r2 = (((r3 - r8) << 2) + r10) << 2;
  if ((*(s8 *)(r5 + (((((r3 - r8) << 2) + r10) << 2) + 4))) != 0)
    goto L_0200c57e;
  if ((*(s8 *)(r5 + (r2 + 8))) != 0)
    goto L_0200c57e;
  if ((*(s8 *)(r5 + (r2 - 108))) == 0)
    goto L_0200c656;
  goto L_0200c57e;
L_0200c656:
  if ((*(s8 *)(r5 + (r2 + 116))) == 0)
    goto L_0200c662;
  goto L_0200c57e;
L_0200c662:
  r6 = r10;
  r6 = r6 + 1;
  goto L_0200c6a4;
L_0200c668:
  r1 = r8;
  r2 = (((r3 - r8) << 2) + r10) << 2;
  if ((*(s8 *)(r5 + (((((r3 - r8) << 2) + r10) << 2) - 112))) == 0)
    goto L_0200c67e;
  goto L_0200c57e;
L_0200c67e:
  if ((*(s8 *)(r5 + (r2 - 224))) == 0)
    goto L_0200c68a;
  goto L_0200c57e;
L_0200c68a:
  if ((*(s8 *)(r5 + (r2 - 116))) == 0)
    goto L_0200c696;
  goto L_0200c57e;
L_0200c696:
  if ((*(s8 *)(r5 + (r2 - 108))) == 0)
    goto L_0200c6a2;
  goto L_0200c57e;
L_0200c6a2:
  r1 = r1 - 1;
L_0200c6a4:
  FIELD_AT((((((r8 << 3) - r8) << 2) + r10) << 2) + r5, u8, 0) = r7;
  r7 = r7 + 1;
  FIELD_AT((((((r8 << 3) - r8) << 2) + r10) << 2) + r5, u8, 1) = r6;
  FIELD_AT((((((r8 << 3) - r8) << 2) + r10) << 2) + r5, u8, 2) = r1;
  r10 = r6;
  r8 = r1;
  if (r7 > 14)
    goto L_0200c6c4;
  goto L_0200c57a;
L_0200c6c4:
  r10 = 14;
  r8 = 14;
  r7 = 1;
L_0200c6cc:
  r4 = r7 + 1;
  ip = (((((r8 << 3) - r8) << 2) + r10) << 2) + r5;
  r2 = r7;
  r1 = *(s8 *)(r5 + ((((((r8 << 3) - r8) << 2) + r10) << 2) + 112));
  if ((*(s8 *)(r5 + ((((((r8 << 3) - r8) << 2) + r10) << 2) + 112))) != r4)
    goto L_0200c6f0;
  r2 = r4;
  goto L_0200c70c;
L_0200c6f0:
  r0 = ip;
  if (r1 != (r7 - 1))
    goto L_0200c6fc;
  r2 = r1;
  goto L_0200c70c;
L_0200c6fc:
  r0 = r0 - 112;
  r1 = FIELD_AT(r0 - 112, s8, 0);
  if ((FIELD_AT(r0 - 112, s8, 0)) != (r2 - 1))
    goto L_0200c70c;
  r2 = r1;
  goto L_0200c6fc;
L_0200c70c:
  if (r2 == 0) {
    r2 = 1;
  }
  r9 = r7 << 3;
  FIELD_AT((r7 << 3) + r11, u8, 2) = r2;
  r2 = r7;
  r1 = *(s8 *)(r5 + (((((lr - r8) << 2) + r10) << 2) - 112));
  r0 = (((lr - r8) << 2) + r10) << 2;
  if ((*(s8 *)(r5 + (((((lr - r8) << 2) + r10) << 2) - 112))) != r4)
    goto L_0200c736;
  r2 = r4;
  goto L_0200c752;
L_0200c736:
  r0 = r0 + r5;
  if (r1 != (r7 - 1))
    goto L_0200c742;
  r2 = r1;
  goto L_0200c752;
L_0200c742:
  r0 = r0 + 112;
  r1 = FIELD_AT(r0, s8, 112);
  if ((FIELD_AT(r0, s8, 112)) != (r2 - 1))
    goto L_0200c752;
  r2 = r1;
  goto L_0200c742;
L_0200c752:
  if (r2 == 0) {
    r2 = 1;
  }
  FIELD_AT(r9 + r11, u8, 5) = r2;
  r2 = r7;
  r1 = *(s8 *)(r5 + (((((lr - r8) << 2) + r10) << 2) - 4));
  if ((*(s8 *)(r5 + (((((lr - r8) << 2) + r10) << 2) - 4))) != r4)
    goto L_0200c778;
  r2 = r4;
  goto L_0200c79e;
L_0200c778:
  r6 = r10;
  if (r1 != (r7 - 1))
    goto L_0200c784;
  r2 = r1;
  goto L_0200c79e;
L_0200c784:
  r6 = r6 + 1;
  r1 = *(s8 *)(r5 + ((((lr - r8) << 2) + r6) << 2));
  if ((*(s8 *)(r5 + ((((lr - r8) << 2) + r6) << 2))) != (r2 - 1))
    goto L_0200c79e;
  r2 = r1;
  goto L_0200c784;
L_0200c79e:
  if (r2 == 0) {
    r2 = 1;
  }
  FIELD_AT(r9 + r11, u8, 3) = r2;
  r2 = r7;
  r0 = r8 << 3;
  r1 = *(s8 *)(r5 + ((((((r8 << 3) - r8) << 2) + r10) << 2) + 4));
  if ((*(s8 *)(r5 + ((((((r8 << 3) - r8) << 2) + r10) << 2) + 4))) != r4)
    goto L_0200c7c4;
  r2 = r4;
  goto L_0200c7f4;
L_0200c7c4:
  r6 = r10;
  if (r1 != (r7 - 1))
    goto L_0200c7dc;
  r2 = r1;
  goto L_0200c7f4;
L_0200c7dc:
  r6 = r6 - 1;
  r1 = *(s8 *)(r5 + ((((r0 - r8) << 2) + r6) << 2));
  if ((*(s8 *)(r5 + ((((r0 - r8) << 2) + r6) << 2))) != (r2 - 1))
    goto L_0200c7f4;
  r2 = r1;
  goto L_0200c7dc;
L_0200c7f4:
  if (r2 == 0) {
    r2 = 1;
  }
  FIELD_AT(r9 + r11, u8, 4) = r2;
  FIELD_AT(r9 + r11, u8, 0) = r7;
  r7 = r4;
  r10 = ((FIELD_AT(ip, u8, 1)) << 24) >> 24;
  r8 = ((FIELD_AT(ip, u8, 2)) << 24) >> 24;
  if (r7 > 14)
    goto L_0200c81e;
  goto L_0200c6cc;
L_0200c81e:
  FIELD_AT(r11, u8, 117) = 251;
  FIELD_AT(r11, u8, 10) = 250;
  FIELD_AT((s32)gData_020023c4 + 0x288, s32, 0) = 1;
  MemFree(r5);
  Call1((void (*)())GameFlag_SetBit, 0x3f0);
  FIELD_AT((s32)gRandomState, s32, 0) = stack[1];
  r0 = v4;
  r3 = (s32)gRandomState;
L_0200c846:
  /* add (s32)stack, #8 */
  return r0;
}
