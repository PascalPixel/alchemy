#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"

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

void FieldEvent_WaitFrames(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020012a0(const void *data, s32 a, s32 b);
void Func_020012a8(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_020012e0(void);
void Func_020012e8(void);
void Func_02001348(s32 id, s32 x, s32 z);
void Func_020013d0(s32 value);
void Func_020013f8(s32 value);
void Func_02001448(s32 cue);

static __inline__ void Func_020012a0_(const void *data, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020012a0(data, a, b);
}

static __inline__ void Func_020012a8_(s32 srcX, s32 srcY, s32 width, s32 height,
                                      s32 destX, s32 destY) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020012a8(srcX, srcY, width, height, destX, destY);
}

static __inline__ void Func_02001348_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001348(id, x, z);
}

static __inline__ void Offset_(s32 id, s32 dx, s32 dz) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

void Func_02000eec(void) {
  struct Adventure *party = &gAdventure;
  struct EventWork154 *work = Ram_HeapSlots->eventWork;

  /* FAKEMATCH: a one-pass block loads the adventure before the event work
   * (one reordered row). */
  do {
  } while (0);
  Actor_SetModeById(party->currentOwner, 2);
  Actor_SetSpeedParameters(gAdventure.currentOwner, 0x8000, 0x4000);
  Func_02001448(123);
  Offset_(gAdventure.currentOwner, 0, -4);
  Func_020013d0(work->value);
}
