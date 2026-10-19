#include "types.h"
#include "ram_buffer.h"

// A scene event as the event finder (Func_080ccd78) returns it.
struct TouchEvent {
  s32 flags;
  u8 target;
  u8 unk05;
  s16 condition;
  // A message number below 0x10000, else a function; read as a union
  // member, so a store to the field work does not let it stay in a
  // register.
  union {
    s32 result;
  } u;
};

struct TouchWork {
  u8 unk000[0x158];
  s16 exit;
  u8 unk15a[0x184 - 0x15a];
  s16 blockedSteps;
};

struct TouchEvent *Func_080ccd78(s32 kind, s32 target);
void EventRuntime_RunMessage(s32 actor, s32 message);
void Audio_PlayCue(s32 cue);
void ObjectEffect_BeginContextEffect25(void);
void ObjectEffect_BeginContextEffect26(void);
void Func_080d5fd4(void);

s32 Scene_RunRaisedEvent(s32 id) {
  u8 *descriptor = (u8 *)Func_080ccd78(6, id);
  s32 result = -1;

  if (descriptor != NULL) {
    s32 handler = *(s32 *)(descriptor + 8);

    if (handler != 0) {
      if (handler < 0x10000)
        EventRuntime_RunMessage(-1, handler);
      else
        ((void (*)(s32))handler)(id);
      result = 0;
    }
  }
  return result;
}

// Runs the touch event (kind 2) of what the leader touched, or else takes its
// exit (kind 1): plays the exit's sound (and effect) and records where it
// leads. Returns 0 when something ran, -1 when there was nothing; as ☀️'s
// Scene_RunTouchOrExit.
s32 Scene_RunTouchOrExit(s32 value) {
  struct TouchEvent *event;
  s32 handled;
  struct TouchWork *work;

  event = Func_080ccd78(2, value);
  handled = -1;
  work = Ram_HeapSlots->eventWork;
  if (event != NULL && event->u.result != 0) {
    if (event->flags & 0x200)
      work->blockedSteps = 0;
    if (event->u.result < 0x10000) {
      EventRuntime_RunMessage(handled, event->u.result);
      handled = 0;
    } else {
      ((void (*)(s32))event->u.result)(value);
      handled = 0;
    }
  } else {
    event = Func_080ccd78(1, value);
    if (event != NULL) {
      switch (event->flags & 0x70) {
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
      {
        /* FAKEMATCH(temporary): the destination loaded into its own
         * variable, so the barrier below can follow the load; as
         * `work->exit = event->u.result`, 2 of 87 instructions differ in
         * tla-en. */
        s32 destination = event->u.result;

        /* FAKEMATCH(barrier): the destination's load kept ahead of the exit
         * field's offset, as the native orders them; without the barrier, 2
         * of 87 instructions differ in tla-en. */
        __asm__ volatile("");
        work->exit = destination;
      }
      handled = 0;
    }
  }
  return handled;
}
