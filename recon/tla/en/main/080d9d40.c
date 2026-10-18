// Near miss, differ 2 of 144, instruction order only: the native loads
// gPartyState's address before building the 530 offset; this one after.
// Through a cast pointer the cue stores load 0xfc83 from the pool (differ
// 32); as a struct field they build it as the native does.
// The heap slot 0xa0 work's per-frame callback (Func_080d9b08 adds
// it). Unless flag 260 is set, once the event work's counter at 388 passes 12
// it picks the event work's cue at 362: with render mode 0 the slot linking
// the leader's nearby talk target sets 0xfc83 when Func_080d99b8 accepts its
// two ends, and render mode 4 sets 0xfc84. Then every active slot wobbles:
// the amplitude decays by 0.95 a frame until it drops to 0x1998, and
// Func_080da060 redraws the slot with the sine of its angle times it.
#include "types.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "party_state.h"
#include "ram_buffer.h"

struct WobbleEvent {
  u8 unk000[362];
  u16 cue;
  u8 unk16c[24];
  s16 counter;
};

struct WobbleSlot {
  s16 first;
  s16 second;
  u8 unk04[4];
  s32 amplitude;
  u8 unk0c[4];
  s32 angle;
  void *tail;
  void *head;
};

struct WobbleWork {
  u8 unk00[12];
  struct WobbleSlot slots[8];
};

s32 GameFlag_Test(s32 flag);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Func_080d99b8(s32 first, s32 second);
void Func_080da060(s32 index, s32 first, s32 second, s32 wobble);

void Func_080d9d40(void) {
  struct WobbleEvent *event = Ram_HeapSlots->eventWork;
  struct WobbleWork *work = Ram_HeapSlots->unk84[7];
  struct WobbleSlot *slot;
  s32 i = 0;

  if (!GameFlag_Test(260) && event->counter > 12) {
    if (gPartyState.renderMode == 0) {
      s32 target = Field_FindNearbyTalkTarget(gPartyState.currentOwner);

      if (target != -1) {
        for (i = 0; i < 8; i++) {
          if (work->slots[i].first == target ||
              work->slots[i].second == target)
            break;
        }
        if (i < 8 &&
            Func_080d99b8(work->slots[i].first, work->slots[i].second))
          event->cue = 0xfc83;
      }
    } else if (gPartyState.renderMode == 4) {
      event->cue = 0xfc84;
    }
  }

  slot = work->slots;
  for (i = 0; i < 8; i++, slot++) {
    if (slot->tail != NULL) {
      s32 wobble = Iwram_MulQ16(Trig_Sin(slot->angle), slot->amplitude);

      slot->angle += 0x1000;
      slot->amplitude = Iwram_MulQ16(slot->amplitude, 0xf333);
      if (slot->amplitude <= 0x1998) {
        slot->amplitude = 0;
        slot->tail = NULL;
      }
      Func_080da060(i, slot->first, slot->second, wobble);
    }
  }
}
