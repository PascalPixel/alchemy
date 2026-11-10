#include "types.h"
#include "ram_buffer.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02004594(void);
void Func_020045e4(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void FlagSet(s32 flag) { GameFlag_SetBit(flag); }

void Func_020038e8(void) {
  void *work = Ram_HeapSlots->eventWork;

  /* FAKEMATCH: ⚓️ loads the event work pointer before it builds the 0xca2
   * offset; without the barrier the offset's first half is scheduled before the
   * load */
  do {
  } while (0);
  if (FIELD_AT(work, s16, 0xca2) != 0) {
    FlagSet(0x106);
    Func_02004594();
    Func_020045e4();
    GameFlag_ClearBit(0x106);
  }
}
