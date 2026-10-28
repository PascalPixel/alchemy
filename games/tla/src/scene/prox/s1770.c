#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 GameFlag_Test(s32 flag);
void wait(s32 frames);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_SetAngleToward(s32 id, s32 other, s32 flags);
void Func_02004464(void);
void Func_0200446c(void);
void Func_020044d4(s32 id, s32 x, s32 z, s32 y);
void Func_020045ec(s32 value);

void Func_02001770(void) {
  if (GameFlag_Test(0xa3d) == 0 && GameFlag_Test(64) == 0) {
    u8 *work = Ram_HeapSlots->eventWork;

    Func_02004464();
    Func_020045ec(1);
    {
      s32 y = 0x80;
      /* FAKEMATCH: ⚓️ starts 0x4000 before the other arguments and shifts it
       * after them; plain constants build it third (differ 2). */
      asm volatile("" : "+l"(y));
      Func_020044d4(20, 0x1980000, 0x910000, y << 7);
    }
    wait(1);
    Actor_Jump(20, 4, 20);
    Actor_SetAngleToward(20, gSequenceParentWork.currentOwner, 0);
    FIELD_AT(work, u16, 362) = 200;
    Func_0200446c();
  }
}
