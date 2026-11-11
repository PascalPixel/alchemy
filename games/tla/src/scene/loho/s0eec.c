#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 value;
};

struct LohoPicture {
  const void *data;
  u16 a;
  u16 b;
};

extern const struct LohoPicture gData_02001f14;

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020012e0(void);
void Func_020012e8(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Offset_(s32 id, s32 dx, s32 dz) {
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

void Func_02000eec(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct EventWork154 *work = Ram_HeapSlots->eventWork;

  /* FAKEMATCH(dead): an empty once-only block, so gMainRoutineWork loads
   * before the event work; as no block, one row is reordered. */
  do {
  } while (0);
  Actor_SetModeById(party->currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Sound_Play(123);
  Offset_(gMainRoutineWork.currentOwner, 0, -4);
  Import_EventSetExitRequest(work->value);
}
