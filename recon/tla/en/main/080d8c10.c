// Draft, differ 47 of 142 (55 before, 2026-10-26): the heap slot 0x9c workspace's per-frame update.
// Unless flag 260 is set, once the event timer passes 12: on foot, the talk
// target the leader faces is looked up among the eight slots' two ids and,
// when Func_080d99b8 accepts the pair, the event prompt becomes 0xfc85; in
// render mode 3 it is 0xfc86. Then each active slot's swing is advanced: a
// damped sine (phase steps 1/16 turn, amplitude times 0.95 until it falls
// under 0.1) passed to Func_080d8fa8. No ☀️ twin found. The search is a while
// over the slots that counts before it steps, which gives the native's copy
// of slot 0's test before the loop. Left: the native then steps a separate
// pointer from the slots base, which stays in r5 for the second loop; here
// the first count folds to 1 and the base needs r4 (pushed and spilled,
// frame 4).
// The second loop matches. The event fields are struct fields (through cast
// pointers the prompt's constants come from the pool: differ 73).
#include "types.h"
#include "flag_call.h"
#include "iwram_call.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct SwingSlot {
  s16 id;
  s16 value;
  u8 unk04[8];
  s32 amplitude;
  u8 unk10[4];
  s32 phase;
  s32 active;
  u8 unk1c[4];
};

struct SwingEvent {
  u8 unk000[362];
  s16 prompt;
  u8 unk16c[24];
  s16 timer;
};

struct SwingWork {
  u8 unk000[12];
  struct SwingSlot slots[8];
};

s32 Field_FindNearbyTalkTarget(s32 owner);
s32 Func_080d99b8(s32 id, s32 value);
s32 Trig_Sin(s32 angle);
void Func_080d8fa8(s32 index, s32 id, s32 value, s32 offset);

void Func_080d8c10(void) {
  struct SwingEvent *event = Ram_HeapSlots->eventWork;
  struct SwingWork *work = Ram_HeapSlots->unk84[6];
  struct SwingSlot *slot;
  s32 i = 0;

  if (!GameFlag_Test(260) && event->timer > 12) {
    if (gMainRoutineWork.renderMode == 0) {
      s32 target = Field_FindNearbyTalkTarget(gMainRoutineWork.currentOwner);

      if (target != -1) {
        slot = work->slots;
        while (slot->id != target && slot->value != target) {
          if (++i > 7)
            break;
          slot++;
        }
        if (i < 8 && Func_080d99b8(work->slots[i].id, work->slots[i].value))
          event->prompt = 0xfc85;
      }
    } else if (gMainRoutineWork.renderMode == 3) {
      event->prompt = 0xfc86;
    }
  }
  slot = work->slots;
  for (i = 0; i < 8; i++, slot++) {
    if (slot->active != 0) {
      s32 swing = Iwram_MulQ16(Trig_Sin(slot->phase), slot->amplitude);

      slot->phase += 0x1000;
      slot->amplitude = Iwram_MulQ16(slot->amplitude, 0xf333);
      if (slot->amplitude <= 0x1998) {
        slot->amplitude = 0;
        slot->active = 0;
      }
      Func_080d8fa8(i, slot->id, slot->value, swing);
    }
  }
}
