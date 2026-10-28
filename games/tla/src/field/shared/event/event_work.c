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
#include "sequence_parent_work.h"
#include "actor.h"
#include "display_transition.h"

void Event_StartScreenTransition(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  ScreenTransition_Start(event->screenEffect, event->delay);
  event->screenStatus = 1;
}

void Event_FinishScreenTransition(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  ScreenTransition_Finish(event->screenEffect, event->delay);
  event->screenStatus = 0;
}

void Event_WaitTransitionFrames(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  wait(event->delay);
}

void Event_SetDestination(s32 scene, s32 entrance) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct SequenceParentWork *party;

  event->countdown = 999;
  /* FAKEMATCH: materialize the party pointer before building the scene offset.
   */
  asm volatile("" : "=r"(party) : "0"(&gSequenceParentWork));
  party->scene = scene;
  party->entrance = entrance;
}

void Event_SetRestartPoint(s32 scene, s32 entrance) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct SequenceParentWork *party;

  event->countdown = 999;
  /* FAKEMATCH: materialize the party pointer before building the saved-scene
   * offset. */
  asm volatile("" : "=r"(party) : "0"(&gSequenceParentWork));
  party->savedScene = scene;
  party->savedEntrance = entrance;
}

void Event_SetExitRequest(u16 frames) {
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
        (u8 *)ObjectTable_Get(Party_GetLeaderActor()) + 8);
  BattleEffect_SelectBattleCue(row, column);
}
