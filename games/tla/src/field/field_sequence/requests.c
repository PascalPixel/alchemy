/// @file
/// Battles and destinations an event requests, and the queued sound.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"
#include "sounds.h"
#include "flag_call.h"
#include "module/field/event_commands.h"
#include "module/field/object.h"

void Field_RequestBattleFromEncounterTable(s32 row, s32 column) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  event->encounter = Encounter_GetFormationAtSlot(row, column);
  if (event->mode == 3)
    Encounter_ResolveWorldMapZoneAndBackdrop(
        (u8 *)ObjectTable_Get(Party_GetLeaderActor()) + 8);
  BattleEffect_SelectBattleCue(row, column);
}

extern s8 gData_02001000[];

/// Queues a Djinni battle. djinn / 20 is the element and djinn % 20 the
/// Djinni, where 19 means the one recorded for the element in the save (none:
/// nothing happens). Unless flag 0x800 is set the element's encounter is
/// prepared first; the pending event is 300 + the Djinni id with that flag,
/// the enemy group is chosen, and on the world map the zone and backdrop come
/// from the leader's position.
/* VIEW(event_commands): routine Field_RequestDjinnBattle as void
 * Field_RequestDjinnBattle(u32 flags, u32 djinn); field/event.h has void
 * Field_RequestDjinnBattle(s32 flag, s32 no); tried as the definition's void
 * Field_RequestDjinnBattle(u32 flags, u32 djinn), which the object already
 * declares, it does not compile in tla-ja:
 * games/tla/src/field/field_sequence/requests.c:23: conflicting types for
 * `Field_RequestDjinnBattle'. */
void Field_RequestDjinnBattle__event_commands(u32 flags, u32 djinn) __asm__(
    "Field_RequestDjinnBattle");
#define Field_RequestDjinnBattle Field_RequestDjinnBattle__event_commands
void Field_RequestDjinnBattle(u32 flags, u32 djinn) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
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
  work->encounter = Encounter_GetDjinnFormation(djinn);
  if (work->mode == 3)
    Encounter_ResolveWorldMapZoneAndBackdrop(
        &ObjectTable_Get(Party_GetLeaderActor())->x);
  BattleEffect_SelectBattleCue(0, 0);
}

void Event_SetAfterBattleDestination(s32 first, s32 second) {
  gMainRoutineWork.pair1ee[0] = first;
  gMainRoutineWork.pair1ee[1] = second;
}

void Party_SetFields1f2And1f4(s32 first, s32 second) {
  gMainRoutineWork.pair1f2[0] = first;
  gMainRoutineWork.pair1f2[1] = second;
}

void Event_LeaveFor(s16 scene, s16 entrance) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  gMainRoutineWork.scene = scene;
  gMainRoutineWork.entrance = entrance;
  event->exit = 999;
}

void Scene_QueueMusic(s32 soundId) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 state;

  state = soundId;
  event->queuedSound = state;
  if ((s16)state == -1)
    state = 0x186;
  Sound_PlayFar(SE_298);
  Sound_PlayFar(state);
}

void Event_NoOp(void) {}

void Scene_PlayQueuedMusic(void) {
  u8 *work = Ram_HeapSlots->fieldWork;

  if (work != 0) {
    s16 cue = *(s16 *)(work + 0xcb8);

    if (cue != -1)
      Sound_PlayFar(cue);
  }
}

void Func_080d2cc4(s32 message) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_ShowFar(message, 1);
  GameFlag_Clear(0x142);
  Event_End();
  if (Party_FindOwnerWithActionFar(136) != -1)
    ((struct EventWork15a *)Ram_HeapSlots->fieldWork)->flag = 1;
}

/* VIEW(event_commands): tag struct EventWork1cc as struct
 * EventWork1cc__event_commands; module/tla.h has another definition; tried with
 * the definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/field_sequence/requests.c:119: redefinition of `struct
 * EventWork1cc'. */
#define EventWork1cc EventWork1cc__event_commands
struct EventWork1cc {
  u8 unk000[0x1cc];
  struct CueObject *focus;
};

#define ABS(x) ((x) >= 0 ? (x) : -(x))

void Event_PlayCueNearFocus(struct CueObject *object, s32 cue) {
  struct CueObject *focus = gFieldWork->camera;

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
