#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "party_state.h"
#include "object_runtime.h"
#include "display_transition.h"

void EventRuntime_StartScreen(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  DisplayTransition_Start(event->screenEffect, event->delay);
  event->screenStatus = 1;
}

void EventRuntime_FinishScreen(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  DisplayTransition_Finish(event->screenEffect, event->delay);
  event->screenStatus = 0;
}

void EventRuntime_WaitDelay(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  WaitFrames(event->delay);
}

void EventRuntime_SetDestination(s32 scene, s32 entrance) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct PartyState *party;

  event->countdown = 999;
  /* FAKEMATCH: materialize the party pointer before building the scene offset.
   */
  asm volatile("" : "=r"(party) : "0"(&gPartyState));
  party->scene = scene;
  party->entrance = entrance;
}

void EventRuntime_SetSavedDestination(s32 scene, s32 entrance) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct PartyState *party;

  event->countdown = 999;
  /* FAKEMATCH: materialize the party pointer before building the saved-scene
   * offset. */
  asm volatile("" : "=r"(party) : "0"(&gPartyState));
  party->savedScene = scene;
  party->savedEntrance = entrance;
}

void EventRuntime_SetCountdown(u16 frames) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  event->countdown = frames;
}

void Field_RequestBattleFromEncounterTable(s32 row, s32 column) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  event->encounterEnemyGroup = Encounter_GetEnemyGroupAtSlot(row, column);
  /* FAKEMATCH: the ARM9 scheduling would address the control mode before
   * storing the group. */
  __asm__("");
  if (event->controlMode == 3)
    Encounter_ResolveWorldMapZoneAndBackdrop(
        (u8 *)ObjectTable_Get(EventRuntime_GetControlledOwner()) + 8);
  BattleEffect_SelectBattleCue(row, column);
}
