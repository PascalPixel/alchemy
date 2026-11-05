// Draft, differ 32 of 67 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Measured under -Os, 5 November 2026: differ 33 (tla-en); notes below may predate it.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"

struct MagmaRockEventWork {
  u8 unk000[0x15a];
  s16 value15a;
  u8 unk15c[2];
  s16 value15e;
  u8 unk160[6];
  s16 value166;
  s16 value168;
  u8 unk16a[0x48];
  u16 value1b2;
};

void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02001738(void);

void Func_02000ff4(struct Actor *object) {
  struct MagmaRockEventWork *work = Ram_HeapSlots->eventWork;
  s32 mask;
  /* FAKEMATCH(pin): timer in r8, as the native keeps it; as a plain local,
   * 56 of 67 instructions differ in tla-en. */
  register u16 timer asm("r8") = object->action;

  if (object->action > 528) {
    mask = 2;
    /* FAKEMATCH(barrier): an empty asm holds the order around it; as no asm,
     * 33 of 67 instructions differ in tla-en. */
    asm("");
    goto blink;
  }
  if (object->action > 400) {
    mask = 16;
  blink:
    if (timer & mask)
      Actor_SetPartAttribute(object, 10);
    else
      Actor_SetPartAttribute(object, 0);
  } else {
    object->update = Func_02001738;
    object->action = 1;
  }
  if (work->value166 == 0 && work->value15a == 0 && work->value15e == 0 &&
      work->value168 == 0 && work->value1b2 != 0)
    object->action--;
}
