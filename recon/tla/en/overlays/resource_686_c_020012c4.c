// Draft, differ 157 of 148: translated from its listing; names and structure
// still to do.
#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 Data_02004030[];
extern u8 gResourceTableEntries[];
s32 GameFlag_Test(s32 flag);
s32 Func_02002dac();

s32 Func_020012c4(void) {
  s32 r8, r7, r6, r10, r5, r11, v0, r9, r4, r0, r1, r3, r2;
  s32 stack[1];

  /* sub sp, #4 */
  r8 = (s32)Ram_HeapSlots->mapWork;
  r7 = FIELD_AT(r8, s32, 228);
  r6 = FIELD_AT(r8, s32, 232);
  r7 = r7 & -65536;
  r6 = r6 & -65536;
  stack[0] = FIELD_AT(FIELD_AT(r8, s32, 0), s32, 4);
  r10 = (s32)Data_02004030;
  r5 = r10;
  r11 = (u32)(FIELD_AT(((*(s16 *)(((s32)Data_02004030) + 2)) << 2) +
                           ((s32)gResourceTableEntries),
                       u16, 2)) >>
        5;
  r5 = r5 + 16;
  v0 = GameFlag_Test(0x205);
  if (v0 != 0)
    goto L_020093e8;
  if ((FIELD_AT(r10, s32, 4)) >= 0x200000)
    goto L_020093e8;
  r9 = 0;
L_02009328:
  r4 = FIELD_AT(r5, s32, 4);
  r0 = FIELD_AT(r5, s32, 8);
  r1 = FIELD_AT(r5, s32, 0);
  r3 = FIELD_AT(r5, s32, 12);
  if ((u32)(FIELD_AT(r5, s32, 12)) > (u32)4)
    goto L_02009388;
  r2 = FIELD_AT(r10, s32, 4);
L_02009358:
  r1 = r1 - r2;
  r3 = 0x100000;
L_0200935e:
  r0 = r0 + r2;
  if (r2 <= r3)
    goto L_02009388;
  goto L_02009386;
L_02009366:
  r0 = r0 + r2;
  r1 = r1 - r2;
  goto L_02009388;
L_0200936c:
  r0 = r0 + r2;
  r1 = r1 + r2;
  goto L_02009388;
L_02009372:
  r1 = r1 + r2;
  r3 = 0x100000;
  goto L_0200935e;
L_0200937a:
  if (r2 >= 0x100000)
    goto L_02009386;
  r0 = r0 + r2;
  goto L_02009388;
L_02009386:
  r1 = -0x100000;
L_02009388:
  r2 = (((r4 - r6) - r0) >> 16) - 8;
  r1 = ((r1 - r7) >> 16) - 8;
  if ((u32)(((r1 - r7) >> 16) + 7) > (u32)334)
    goto L_020093dc;
  if (r2 < -15)
    goto L_020093dc;
  if (r2 > 239)
    goto L_020093dc;
  FIELD_AT(r5, s32, 16) = 0;
  FIELD_AT(r5, s32, 20) = ((r2 & 255) | ((r1 & 511) << 16)) | 1073743872;
  FIELD_AT(r5, s32, 24) = 1024 | r11;
  Func_02002dac(r5 + 16, 255);
L_020093dc:
  r9 = r9 + 1;
  r5 = r5 + 28;
  if (r9 <= 7)
    goto L_02009328;
L_020093e8:
  FIELD_AT(r8, s32, 332) = (FIELD_AT(r10, s32, 12)) + (FIELD_AT(r10, s32, 4));
  /* add sp, #4 */
  return r10;
}
