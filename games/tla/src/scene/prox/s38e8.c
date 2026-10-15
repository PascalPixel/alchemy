#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 GameFlag_SetBit(s32 flag);
s32 GameFlag_ClearBit(s32 flag);
void Func_02004594(void);
void Func_020045e4(void);

static inline void FlagSet(s32 flag) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  GameFlag_SetBit(flag);
}

static inline void FlagClr(s32 flag) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  GameFlag_ClearBit(flag);
}

void Func_020038e8(void) {
  void *work = Ram_HeapSlots->eventWork;

  /* FAKEMATCH: ⚓️ loads the event work pointer before it builds the 0xca2
   * offset; without the barrier the offset's first half is scheduled before the
   * load */
  asm volatile("");
  if (FIELD_AT(work, s16, 0xca2) != 0) {
    FlagSet(0x106);
    Func_02004594();
    Func_020045e4();
    FlagClr(0x106);
  }
}
