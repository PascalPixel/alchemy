/// @file
/// Battles and destinations an event requests, and the queued sound.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "djinn_ids.h"
#include "sounds.h"
#include "field/flag_call.h"
#include "field/event_commands.h"
#include "field/object.h"

/// Sets up a battle with the formation at row and column of the encounter
/// table.
void Field_RequestBattleFromEncounterTable(s32 row, s32 column) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  state->encounter = Encounter_GetFormationAtSlot(row, column);
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
    Encounter_ResolveWorldMapZoneAndBackdrop(
        &Actor_Lookup(Party_GetLeaderActor())->x.fixed);
  Encounter_SelectBattleMusic(row, column);
}

/// The low bits of Field_RequestDjinnBattle's flags: the Djinni's actor.
#define DJINN_ACTOR_MASK 0xff

/// Sets up the battle with Djinni djinn, DJINN_PER_ELEMENT to an element; an
/// element's last Djinni stands for the one of The Broken Seal's chosen for it
/// (SCENE_STATE_CHOSEN_DJINN; none: nothing happens). The low 8 bits of flags
/// are the Djinni's actor, which hops first unless DJINN_NO_HOP is set.
void Field_RequestDjinnBattle(u32 flags, u32 djinn) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  u32 skipHop = flags & DJINN_NO_HOP;
  u32 element;
  s32 index;

  flags &= DJINN_ACTOR_MASK;
  element = djinn / DJINN_PER_ELEMENT;
  index = djinn % DJINN_PER_ELEMENT;

  if (index == DJINN_PER_ELEMENT - 1) {
    index = gSceneLocalState[element + SCENE_STATE_CHOSEN_DJINN];
    if (index == 0)
      return;
    index--;
  }
  djinn = element * DJINN_PER_ELEMENT + index;
  if (skipHop == 0)
    Djinn_RunBattleStartHop(flags);
  // DJINN_NO_HOP rides along: it tells Djinn_RestoreActorAfterBattle to put
  // the Djinni back 32 pixels north of the leader, not at its placement's
  // position.
  gMainRoutineState.pendingDjinni =
      (djinn + DJINNI_BATTLE_EVENT_FIRST) | skipHop;
  state->encounter = Encounter_GetDjinnFormation(djinn);
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
    Encounter_ResolveWorldMapZoneAndBackdrop(
        &Actor_Lookup(Party_GetLeaderActor())->x.fixed);
  Encounter_SelectBattleMusic(0, 0);
}

#undef DJINN_ACTOR_MASK

/// Sets the after-battle destination (gMainRoutineState.afterBattleScene and
/// afterBattleEntrance), where the party goes once the coming battle is won.
void Event_SetAfterBattleDestination(s32 sceneId, s32 entrance) {
  gMainRoutineState.afterBattleScene = sceneId;
  gMainRoutineState.afterBattleEntrance = entrance;
}

/// Sets the defeat destination (gMainRoutineState.defeatScene and
/// defeatEntrance), where the party goes if it loses the coming battle.
void Event_SetDefeatDestination(s32 sceneId, s32 entrance) {
  gMainRoutineState.defeatScene = sceneId;
  gMainRoutineState.defeatEntrance = entrance;
}

/// Makes the event leave for entrance entrance of scene sceneId: sets
/// gMainRoutineState.next and entrance and raises the exit request
/// EXIT_KEEP_DESTINATION, so the field sequence leaves on its next pass.
void Event_LeaveFor(u16 sceneId, u16 entrance) {
  struct FieldState *state = gFieldState;
  gMainRoutineState.next = sceneId;
  gMainRoutineState.entrance = entrance;
  state->exit = EXIT_KEEP_DESTINATION;
}

/// Plays SE_STOP_EXIT_SE, then soundId (SOUND_FADE_OUT_ACTION_SE for
/// -1), and remembers soundId for Scene_PlayQueuedMusic in the field state's
/// queuedSound; -1 is remembered as none, so it plays SOUND_FADE_OUT_ACTION_SE
/// now and nothing later.
void Scene_QueueMusic(s32 soundId) {
  gFieldState->queuedSound = soundId;
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
  struct FieldState *state = Ram_HeapSlots->fieldState;

  if (state != NULL) {
    s16 cue = state->queuedSound;

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
    gFieldState->fieldMenu = 1;
}

/// Plays sound when the actor is in view: within 152 pixels across and 112 up
/// or down the screen of the actor the camera follows.
void Actor_PlaySoundInView(struct Actor *actor, s32 sound) {
  struct Actor *focus = gFieldState->camera;

  if (actor == 0 || focus == 0)
    return;
  if (ABS(focus->x.fixed - actor->x.fixed) > toFIXED(152))
    return;
  if (ABS((focus->z.fixed - focus->y.fixed) -
          (actor->z.fixed - actor->y.fixed)) > toFIXED(112))
    return;
  Sound_PlayFar(sound);
}

/// Counts one more weird answer Felix gives (gMainRoutineState.weirdAnswers).
void Event_CountAnswer(void) { gMainRoutineState.weirdAnswers++; }
