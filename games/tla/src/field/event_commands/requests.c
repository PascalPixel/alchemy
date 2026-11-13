/// @file
/// Battles and destinations an event requests, and the queued sound.
// Part of event_commands.c.

#include "../event_commands_internal.h"
#include "flag_call.h"
#include "module/field/event_commands.h"
#include "module/field/object.h"

void Field_RequestBattleFromEncounterTable(s32 row, s32 column) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  event->encounterEnemyGroup = Encounter_GetFormationAtSlot(row, column);
  if (event->controlMode == 3)
    Encounter_ResolveWorldMapZoneAndBackdrop(
        (u8 *)ObjectTable_Get(Party_GetLeaderActor()) + 8);
  BattleEffect_SelectBattleCue(row, column);
}

s32 Encounter_GetDjinnFormation(s32 djinn);

extern s8 gData_02001000[];

/// Queues a Djinni battle. djinn / 20 is the element and djinn % 20 the
/// Djinni, where 19 means the one recorded for the element in the save (none:
/// nothing happens). Unless flag 0x800 is set the element's encounter is
/// prepared first; the pending event is 300 + the Djinni id with that flag,
/// the enemy group is chosen, and on the world map the zone and backdrop come
/// from the leader's position.
void Field_RequestDjinnBattle(u32 flags, u32 djinn) {
  u8 *work = Ram_HeapSlots->eventWork;
  u32 noPrepare = flags & 0x800;
  u32 group;
  s32 index;

  flags &= 0xff;
  group = djinn / 20;
  index = djinn % 20;

  if (index == 19) {
    index = gData_02001000[group + 0x1950];
    if (index == 0)
      return;
    index--;
  }
  djinn = group * 20 + index;
  if (noPrepare == 0)
    Djinn_RunBattleStartHop(flags);
  gMainRoutineWork.pendingDjinnEvent = (djinn + 300) | noPrepare;
  *(s16 *)(work + 356) = Encounter_GetDjinnFormation(djinn);
  if (*(s8 *)(work + 394) == 3)
    Encounter_ResolveWorldMapZoneAndBackdrop(
        &ObjectTable_Get(Party_GetLeaderActor())->x);
  BattleEffect_SelectBattleCue(0, 0);
}

void Event_SetAfterBattleDestination(u16 first, u16 second) {
  gMainRoutineWork.pair1ee[0] = first;
  gMainRoutineWork.pair1ee[1] = second;
}

void Party_SetFields1f2And1f4(u16 first, u16 second) {
  gMainRoutineWork.pair1f2[0] = first;
  gMainRoutineWork.pair1f2[1] = second;
}

void Event_LeaveFor(s16 scene, s16 entrance) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  gMainRoutineWork.scene = scene;
  gMainRoutineWork.entrance = entrance;
  event->countdown = 999;
}

void Scene_QueueMusic(s32 soundId) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 state;

  state = soundId;
  event->cueState = state;
  if ((s16)state == -1)
    state = 0x186;
  Sound_PlayFar(0x12a);
  Sound_PlayFar(state);
}

void Event_NoOp(void) {}

void Scene_PlayQueuedMusic(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  if (work != 0) {
    s16 cue = *(s16 *)(work + 0xcb8);

    if (cue != -1)
      Sound_PlayFar(cue);
  }
}

s32 Party_FindOwnerWithActionFar(s32 action);

struct EventWork15a {
  u8 unk000[0x15a];
  u16 flag;
};

void Func_080d2cc4(s32 message) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_ShowFar(message, 1);
  GameFlag_ClearBit(0x142);
  Event_End();
  if (Party_FindOwnerWithActionFar(136) != -1)
    ((struct EventWork15a *)Ram_HeapSlots->eventWork)->flag = 1;
}

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct CueObject *focus;
};

#define ABS(x) ((x) >= 0 ? (x) : -(x))

void Func_080d2d08(struct CueObject *object, s32 cue) {
  struct CueObject *focus =
      ((struct EventWork1cc *)Ram_HeapSlots->eventWork)->focus;

  if (object == 0 || focus == 0)
    return;
  if (ABS(focus->x - object->x) > 0x980000)
    return;
  if (ABS((focus->top - focus->base) - (object->top - object->base)) > 0x700000)
    return;
  Sound_PlayFar(cue);
}

/// Counts up the main routine's counter274.
void Func_080d2d70(void) { gMainRoutineWork.counter274++; }
