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
#include "adventure.h"
#include "actor.h"
#include "task.h"
#include "game_flags.h"
#include "system.h"

void Event_Wait(s32 frames) {
  struct EventWork *state = Ram_HeapSlots->eventWork;
  if (state->waitMode == 0 && frames != 0)
    wait(frames);
}

void Leader_StopMoving(void) {
  struct Actor *object =
      ObjectTable_Get(Party_GetLeaderActor());
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  if (gAdventure.renderMode == 2)
    Actor_SetMode(object, 12);
  else
    Actor_SetMode(object, 1);
}

void Event_Start(void) {
  struct EventWork *state = Ram_HeapSlots->eventWork;
  UiTimedNotice_CloseIfActiveFar();
  Leader_StopMoving();
  if (state->actionCounter != 0)
    Event_ResolveAllPendingActions();
  state->unkcb0 = 0;
  state->unkcb2 = 0;
  state->unkcb4 = 0;
  state->delay = 16;
  state->waitMode = 0;
  state->messageActor = 0xffff;
  state->choice = -1;
  state->messageState = -1;
  Task_Add((s32)Event_UpdateDebugSkip, TASK_PASS_VBLANK);
  GameFlag_ClearBit(0x132);
  {
    struct Adventure *party = &gAdventure;
    /* FAKEMATCH: the pointer handoff keeps its load before the destination
     * offset. */
    __asm__ volatile("" : "+r"(party));
    state->currentOwner = party->currentOwner;
    state->objectStatus = 0;
  }
}

void Event_End(void) {
  Task_Remove((u32)Event_UpdateDebugSkip);
  {
    struct Adventure *party = &gAdventure;
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

void Event_EmptyCallback(void) {}
