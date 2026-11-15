/// @file
/// Battles and destinations an event requests, and the queued sound.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "djinn_ids.h"
#include "sounds.h"
#include "field/flag_call.h"
#include "field/event_commands.h"
#include "field/object.h"

/// Sets up a battle with the formation in slot of encounter table zone.
void Field_RequestBattleFromEncounterTable(s32 row, s32 column) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  event->encounter = Encounter_GetFormationAtSlot(row, column);
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    Encounter_ResolveWorldMapZoneAndBackdrop(
        &Actor_Lookup(Party_GetLeaderActor())->x.fixed);
  BattleEffect_SelectBattleCue(row, column);
}

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
 * games/tla/src/field/field_sequence_requests.c:25: conflicting types for
 * `Field_RequestDjinnBattle'. */
void Field_RequestDjinnBattle__event_commands(u32 flags, u32 djinn) __asm__(
    "Field_RequestDjinnBattle");
#define Field_RequestDjinnBattle Field_RequestDjinnBattle__event_commands
/// Sets up the battle with Djinni number `no`. The low 8 bits of flag are the
/// Djinni's actor, which hops first unless DJINN_NO_HOP is set.
void Field_RequestDjinnBattle(u32 flags, u32 djinn) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 noPrepare = flags & 0x800;
  u32 group;
  s32 index;

  flags &= 0xff;
  group = djinn / DJINN_PER_ELEMENT;
  index = djinn % DJINN_PER_ELEMENT;

  if (index == DJINN_PER_ELEMENT - 1) {
    index = gSceneLocalState[group + 0x1950];
    if (index == 0)
      return;
    index--;
  }
  djinn = group * DJINN_PER_ELEMENT + index;
  if (noPrepare == 0)
    Djinn_RunBattleStartHop(flags);
  gMainRoutineWork.pendingDjinni = (djinn + 300) | noPrepare;
  work->encounter = Encounter_GetDjinnFormation(djinn);
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP)
    Encounter_ResolveWorldMapZoneAndBackdrop(
        &Actor_Lookup(Party_GetLeaderActor())->x.fixed);
  BattleEffect_SelectBattleCue(0, 0);
}

/// Sets the after-battle destination (gMainRoutineWork.afterBattleScene and
/// afterBattleEntrance), where the party goes once the coming battle is won.
void Event_SetAfterBattleDestination(s32 sceneId, s32 entrance) {
  gMainRoutineWork.afterBattleScene = sceneId;
  gMainRoutineWork.afterBattleEntrance = entrance;
}

/// Sets the defeat destination (gMainRoutineWork.defeatScene and
/// defeatEntrance), where the party goes if it loses the coming battle.
void Event_SetDefeatDestination(s32 sceneId, s32 entrance) {
  gMainRoutineWork.defeatScene = sceneId;
  gMainRoutineWork.defeatEntrance = entrance;
}

/// Makes the event leave for entrance entrance of scene sceneId: sets
/// gMainRoutineWork.next and entrance and raises the exit request
/// EXIT_KEEP_DESTINATION (999), so the field sequence leaves on its next pass.
void Event_LeaveFor(u16 sceneId, u16 entrance) {
  struct FieldWork *work = gFieldWork;
  gMainRoutineWork.next = sceneId;
  gMainRoutineWork.entrance = entrance;
  work->exit = EXIT_KEEP_DESTINATION;
}

/// Plays SE_STOP_EXIT_SE, then soundId (SOUND_FADE_OUT_ACTION_SE for
/// -1), and remembers soundId for Scene_PlayQueuedMusic in the field work's
/// queuedSound; -1 is remembered as none, so it plays SOUND_FADE_OUT_ACTION_SE
/// now and nothing later.
void Scene_QueueMusic(s32 soundId) {
  gFieldWork->queuedSound = soundId;
  if ((s16)soundId == -1) {
    soundId = SOUND_FADE_OUT_ACTION_SE;
  }
  Sound_PlayFar(SE_STOP_EXIT_SE);
  Sound_PlayFar(soundId);
}

/// An event that does nothing.
void Event_NoOp(void) {}

/// Plays the sound Scene_QueueMusic remembered, if any.
void Scene_PlayQueuedMusic(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work != NULL) {
    s16 cue = work->queuedSound;

    if (cue != -1)
      Sound_PlayFar(cue);
  }
}

/// Shows message in an event and clears FLAG_NOTHING_HAPPENS; when someone in
/// the party has Scoop, the field menu then opens.
void Event_ShowMessageAndOfferScoop(s32 message) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_ShowFar(message, FIELD_MESSAGE_ZOOM);
  GameFlag_Clear(FLAG_NOTHING_HAPPENS);
  Event_End();
  if (Party_FindOwnerWithActionFar(ABILITY_SCOOP) != -1)
    gFieldWork->fieldMenu = 1;
}

#define ABS(x) ((x) >= 0 ? (x) : -(x))

/// Plays sound when the actor is in view: within 152 pixels across and 112 up
/// or down the screen of the actor the camera follows.
void Actor_PlaySoundInView(struct Actor *actor, s32 sound) {
  struct Actor *focus = gFieldWork->camera;

  if (actor == 0 || focus == 0)
    return;
  if (ABS(focus->x.fixed - actor->x.fixed) > px(152))
    return;
  if (ABS((focus->z.fixed - focus->y.fixed) -
          (actor->z.fixed - actor->y.fixed)) > px(112))
    return;
  Sound_PlayFar(sound);
}

/// Counts up the main routine's countedAnswers.
void Event_CountAnswer(void) { gMainRoutineWork.countedAnswers++; }
