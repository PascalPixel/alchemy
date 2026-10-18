// Draft of Func_080cd5bc (BattleAction_RunDescriptor here) (tla-en,
// 2026-10-18): differ 2 of 87. It runs the field action descriptor for an id:
// a kind-2 descriptor's result is a message number (shown at once) or a
// function to call; otherwise a kind-1 descriptor plays the cue its flags
// choose, starts the matching context effect and queues its result in the
// event work. Returns 0 when a descriptor ran, -1 when none did.
// Rewritten from The Broken Seal's form (the earlier draft): the short
// result here is shown through EventRuntime_RunMessage alone, kind 80 has a
// case of its own, and the flags are read signed.
//
// Left: instruction order only (the 0x158 offset's build sits one place
// after the result's load).
#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"

struct ActionDescriptor {
  s32 flags;
  u8 unk04[4];
  s32 result;
};

struct ActionDescriptor *Func_080ccd78(s32 kind, s32 id);
void EventRuntime_RunMessage(s32 actor, s32 message);
void Audio_PlayCue(s32 cue);
void ObjectEffect_BeginContextEffect25(void);
void ObjectEffect_BeginContextEffect26(void);
void Func_080d5fd4(void);

s32 BattleAction_RunDescriptor(s32 id) {
  struct ActionDescriptor *desc;
  struct EventWork *work;
  s32 ret;

  desc = Func_080ccd78(2, id);
  ret = -1;
  work = Ram_HeapSlots->eventWork;
  if (desc != 0 && desc->result != 0) {
    if (desc->flags & 0x200)
      work->unk184 = 0;
    if (desc->result < 0x10000)
      EventRuntime_RunMessage(-1, desc->result);
    else
      ((void (*)(s32))desc->result)(id);
    ret = 0;
  } else {
    desc = Func_080ccd78(1, id);
    if (desc != 0) {
      switch (desc->flags & 0x70) {
      case 0:
        Audio_PlayCue(123);
        break;
      case 32:
        Audio_PlayCue(128);
        ObjectEffect_BeginContextEffect26();
        break;
      case 48:
        Audio_PlayCue(129);
        ObjectEffect_BeginContextEffect25();
        break;
      case 80:
        Audio_PlayCue(123);
        Func_080d5fd4();
        break;
      }
      work->countdown = desc->result;
      ret = 0;
    }
  }
  return ret;
}
