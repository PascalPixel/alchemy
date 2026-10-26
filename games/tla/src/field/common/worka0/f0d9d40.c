#include "worka0.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "party_state.h"
#include "edition.h"

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
// The Spanish and Italian far-call tables name the talk target search
// differently.
#define Field_FindNearbyTalkTarget Func_080cc54c
#endif

struct WobbleEvent {
  u8 unk000[362];
  u16 cue;
  u8 unk16c[24];
  s16 counter;
};

s32 GameFlag_Test(s32 flag);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Func_080d99b8(s32 first, s32 second);
void Func_080da060(s32 index, s32 first, s32 second, s32 wobble);

/// The heap slot 0xa0 work's per-frame callback (Func_080d9b08 adds it).
/// Unless flag 260 is set, once the event work's counter passes 12 it picks
/// the event work's cue: with render mode 0 the slot linking the leader's
/// nearby talk target sets 0xfc83 when Func_080d99b8 accepts its two ends, and
/// render mode 4 sets 0xfc84. Then every active slot wobbles: the amplitude
/// decays by 0.95 a frame until it drops to 0x1998, and Func_080da060 redraws
/// the slot with the sine of its angle times it.
void Func_080d9d40(void) {
  struct WobbleEvent *event = Ram_HeapSlots->eventWork;
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Slot *slot;
  s32 i = 0;

  if (!GameFlag_Test(260) && event->counter > 12) {
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: loads gPartyState's address before the render mode's offset
     * is built; without it the offset comes first (differ 2). */
    asm volatile("" : "+l"(party));
    if (party->renderMode == 0) {
      s32 target = Field_FindNearbyTalkTarget(party->currentOwner);

      if (target != -1) {
        for (i = 0; i < 8; i++) {
          if (work->slots[i].first == target || work->slots[i].second == target)
            break;
        }
        if (i < 8 && Func_080d99b8(work->slots[i].first, work->slots[i].second))
          event->cue = 0xfc83;
      }
    } else if (party->renderMode == 4) {
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
