#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "party_state.h"
#include "object_runtime.h"
#include "callback_scheduler.h"
#include "game_flags.h"
#include "system.h"

void EventRuntime_Wait(s32 frames) {
  struct EventWork *state = Ram_HeapSlots->eventWork;
  if (state->waitMode == 0 && frames != 0)
    WaitFrames(frames);
}

void EventRuntime_PrepareCurrentObject(void) {
  struct ObjectRuntime *object =
      ObjectTable_Get(EventRuntime_GetControlledOwner());
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  /* FAKEMATCH: this boundary keeps the zero stores before party-mode address
   * construction. */
  __asm__ volatile("" : : : "memory");
  if (gPartyState.renderMode == 2)
    Object_SetMode(object, 12);
  else
    Object_SetMode(object, 1);
}

void EventRuntime_Start(void) {
  struct EventWork *state = Ram_HeapSlots->eventWork;
  UiTimedNotice_CloseIfActiveFar();
  EventRuntime_PrepareCurrentObject();
  if (state->actionCounter != 0)
    EventRuntime_ResolveAllPendingActions();
  state->unkcb0 = 0;
  state->unkcb2 = 0;
  state->unkcb4 = 0;
  state->delay = 16;
  state->waitMode = 0;
  state->messageActor = 0xffff;
  state->choice = -1;
  state->messageState = -1;
  Scheduler_AddOrUpdateCallback((s32)EventRuntime_UpdateWaitMode, 0x480);
  GameFlag_ClearBit(0x132);
  {
    struct PartyState *party = &gPartyState;
    /* FAKEMATCH: the pointer handoff keeps its load before the destination
     * offset. */
    __asm__ volatile("" : "+r"(party));
    state->currentOwner = party->currentOwner;
    state->objectStatus = 0;
  }
}

void EventRuntime_End(void) {
  Scheduler_RemoveCallback((u32)EventRuntime_UpdateWaitMode);
  {
    struct PartyState *party = &gPartyState;
    /* FAKEMATCH: the pointer handoff keeps its load before the override offset.
     */
    __asm__ volatile("" : "+r"(party));
    if (party->ownerOverride == 0)
      Camera_FollowActor(party->currentOwner, 1);
    else
      Camera_FollowActor(8, 1);
    GameFlag_RefreshLureCapFar();
  }
}

void EventRuntime_EmptyHook(void) {}
