#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

struct SceneEventEntry {
  s32 flags;
  u8 value;
  u8 unk05;
  s16 condition;
  s32 result;
};

struct EventWorkView {
  u8 unk000[0x15a];
  s16 unk15a;
  s16 unk15c;
  s16 unk15e;
  u8 unk160[4];
  s16 unk164;
  s16 unk166;
  u8 unk168[0x1c];
  s16 blockedSteps;
  u16 pushKind;
};

void Object_JumpDownLedge(s32 owner, s32 a, s32 b);
s32 Func_080d4d08(void);
s32 Func_080d50f8(void);
s32 Func_080d53b8(void);
s32 Func_080cdc74(void);
void Func_080cdd80(void);
s32 Field_FindNearbyTalkTarget(s32 owner);
struct SceneEventEntry *Func_080ccd78(s32 kind, s32 target);
void EventRuntime_RunMessage(s32 actor, s32 message);

/// The field's push and button handler for keys, idle while
/// gPartyState.pendingThird is set. After the leader has pushed for more than
/// 12 frames: key 0x20 on a 0x80 push makes the leader jump
/// (Object_JumpDownLedge(owner, 6, 0)); key 0x80 runs
/// Func_080d4d08; key 0x40 runs Func_080d50f8 or Func_080d53b8 by the push kind
/// and clears the push state when it returns 0; key 2 with Func_080cdc74 runs
/// the nearby talk target's kind 8 event and then the kind 9 event for keys
/// (messages under 0x10000, otherwise a routine called with keys and the
/// target). Always returns 0.
s32 Func_080cd680(s32 keys) {
  struct EventWorkView *work = Ram_HeapSlots->eventWork;
  s32 owner = gPartyState.currentOwner;
  struct SceneEventEntry *event;
  s32 target;
  s32 result;

  if (gPartyState.pendingThird != 0)
    return 0;
  if ((keys & 0x20) && work->blockedSteps > 12 && (work->pushKind & 0x80)) {
    Object_JumpDownLedge(owner, 6, 0);
    work->blockedSteps = 0;
  }
  if ((keys & 0x80) && work->blockedSteps > 12 && !Func_080d4d08())
    work->blockedSteps = 0;
  if ((keys & 0x40) && work->blockedSteps > 12) {
    s32 kind = work->pushKind & 0xf0;
    s32 done = -1;

    if (kind == 0x80)
      done = Func_080d50f8();
    else if (kind == 0x40)
      done = Func_080d53b8();
    if (done == 0) {
      work->blockedSteps = 0;
      work->unk15a = 0;
      work->unk15e = 0;
      work->unk15c = 0;
      work->unk164 = 0;
      work->unk166 = 0;
    }
  }
  if ((keys & 2) && work->blockedSteps > 12 && Func_080cdc74()) {
    target = Field_FindNearbyTalkTarget(gPartyState.currentOwner);
    if (target != -1) {
      event = Func_080ccd78(8, target);
      if (event != NULL && (result = event->result) != 0) {
        if (result < 0x10000)
          EventRuntime_RunMessage(-1, result);
        else
          ((void (*)(s32, s32))result)(keys, target);
      }
      Func_080cdd80();
      event = Func_080ccd78(9, keys);
      if (event != NULL && (result = event->result) != 0) {
        if (result < 0x10000)
          EventRuntime_RunMessage(-1, result);
        else
          ((void (*)(s32, s32))result)(keys, target);
      }
      work->blockedSteps = 0;
    }
  }
  return 0;
}
