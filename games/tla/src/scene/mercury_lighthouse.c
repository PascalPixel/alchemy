/// @file
/// The base of Mercury Lighthouse in Imil, near the end of the game: as the
/// last beacon, Mars Lighthouse's, is lit, two of Imil's children hear the
/// voices of the heroes far away, glints of light rising round them.

#include "types.h"
#include "graphics/palette_buffers.h"
#include "sprite_ids.h"
#include "system/main_routine_state.h"
#include "field/event_inline.h"
#include "field/actor.h"
#include "field/scene_tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/field_sprite.h"
#include "system/ram_buffer.h"
#include "system/task.h"
#include "system/io_reg.h"
#include "field/actor_script.h"
#include "system/frame.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "text/msg_ids.h"
#include "macros.h"

void MercuryLighthouse_HearVoices(void);

extern s32 gMercuryLighthouse_GlintScript[];
u32 gMercuryLighthouse_GlintFrames BSS_DATA; ///< frames the glint has run

void FieldPsynergy_StopMindReadWave(void);

enum {
  /// Unused: an entrance no exit leads to, which the map does not have. By it
  /// the screen fades to white to SE_ASCEND, as the Mars Lighthouse aerie does
  /// before it sends the party here, and the party comes in again by
  /// EXIT_BACK_TO_THE_VOICES.
  ENTRANCE_UNUSED_WHITE_FADE = 12,
  /// Back into this scene by the entrance where the voices begin.
  EXIT_BACK_TO_THE_VOICES = 9
};

/// A glint's script call: it flickers, hidden and white by turns every two
/// frames.
s32 MercuryLighthouse_FlickerGlint(struct Actor *actor) {
  if ((gMercuryLighthouse_GlintFrames >> 1) & 1)
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
  else
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
  return 0;
}

/// An Imil villager's update while the voices talk: every 40 to 120 frames
/// it turns between two headings, as if looking around for them.
void MercuryLighthouse_LookAround(struct Actor *actor) {
  u8 *timer = &actor->riseCounter;
  u8 *state;

  if (*timer != 0) {
    (*timer)--;
    return;
  }
  *timer = (Random_Below(80)) + 40;
  state = &actor->riseEnabled;
  switch (*state) {
  case 0:
    actor->facing = FACING_WEST;
    *state = 1;
    break;
  case 1:
    actor->facing = FACING_NORTH_NORTHWEST;
    *state = 0;
    break;
  case 2:
    actor->facing = FACING_EAST;
    *state = 3;
    break;
  case 3:
    actor->facing = FACING_NORTH_NORTHEAST;
    *state = 2;
    break;
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// The actors at the lighthouse, by the rows of gActorPlacements; Mia's,
/// Garet's and Sheba's rows go to their party slots.
enum {
  /// Two of Imil's children, who hear the party: "Hey! I know that voice!
  /// It's Mia, I just know it!" and "Of course it's us! We can hear you,
  /// Mia!"
  ACTOR_BOY = ACTOR_FIRST_PLACED,
  ACTOR_GIRL,
  ACTOR_WOMAN, ///< placed at the map's corner, out of sight; nothing uses her
  /// "I don't know. They've been standing here telling everyone to leave..."
  ACTOR_OLD_MAN,
  ACTOR_OLD_WOMAN, ///< "Who are those two kids talking to?"
  /// Two unseen actors the voices' Mind Read wave runs between.
  ACTOR_VOICES_SOURCE,
  ACTOR_VOICES_TARGET
};

void Actor_StartScript(struct Actor *actor, const void *script);

/// The task MercuryLighthouse_HearVoices starts: every 16 frames it starts a
/// flickering glint (SPRITE_GLINT) at a random spot in front of the
/// lighthouse, and it counts the frames in gMercuryLighthouse_GlintFrames.
void MercuryLighthouse_SpawnGlints(void) {
  struct Actor *actor;

  Actor_SetSpritePriority(ACTOR_BOY, 3);
  Actor_SetSpritePriority(ACTOR_GIRL, 3);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineState.leaderActorId), 0);
  if ((gFrameCount & 15) == 0) {
    actor = Import_ActorCreate(SPRITE_GLINT, 0, 0, 0);
    if (actor != NULL) {
      struct FieldSprite *sprite = actor->sprite;
      FIXED x = Random16() * 40 + toFIXED(812);
      FIXED z = toFIXED(696);

      actor->motionFlags = 0;
      actor->x.fixed = x;
      actor->y.fixed = toFIXED(160);
      actor->z.fixed = z;
      Actor_SetSpriteFlags(actor, 0);
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      sprite->priority = 2;
      Actor_StartScript(actor, gMercuryLighthouse_GlintScript);
      Actor_SelectAnimation(actor, 1);
    }
  }
  gMercuryLighthouse_GlintFrames++;
}

/// As the Mars beacon is lit, two children of Imil at the base of Mercury
/// Lighthouse hear Mia and her friends far away, each of their lines carried
/// on a Mind Read wave, until Sheba knows who sent the children there: "The...
/// The Wise One!" The party then returns to the aerie, its beacon burning.
void MercuryLighthouse_HearVoices(void) {
  struct Actor *actor;
  u8 delay;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineState.leaderActorId),
                            SPRITE_COLOR_HIDDEN);
  wait(1);
  gMercuryLighthouse_GlintFrames = 0;
  Task_Add(MercuryLighthouse_SpawnGlints, TASK_PASS_FRAME);
  Actor_SetSpritePriority(ACTOR_BOY, 2);
  Actor_SetSpritePriority(ACTOR_GIRL, 2);
  gFieldState->transitionFrames = 40;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  Event_SetMessageId(MsgImilVoicesDisbeliefReply);
  // "You... You can't be!"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_ShowEmote(ACTOR_BOY, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_BOY, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_BOY, FACING_SOUTH_SOUTHEAST, 0);
  Actor_Jump(ACTOR_BOY, 4, 10);
  // "Hey! I know that voice! It's Mia, I just know it! Mia!!!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_BOY, 0);
  Actor_ShowEmote(ACTOR_OLD_MAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_OLD_WOMAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_OLD_MAN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_OLD_WOMAN, FACING_NORTH_NORTHWEST, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  // "It's you!"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_SetAnimation(ACTOR_GIRL, ANIM_NOD);
  // "Of course it's us! We can hear you, Mia! You're all right! I'm so happy!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GIRL, 0);
  Actor_FaceDirectionAndWait(ACTOR_OLD_WOMAN, FACING_EAST);
  // "Who are those two kids talking to?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_OLD_WOMAN, 0);
  Actor_FaceDirectionAndWait(ACTOR_OLD_MAN, FACING_WEST);
  Actor_SetAnimation(ACTOR_OLD_MAN, ANIM_SHAKE_HEAD);
  // "I don't know. They've been standing here telling everyone to leave..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_OLD_MAN, 0);
  actor = Actor_GetById(ACTOR_OLD_MAN);
  delay = (Random_Below(80)) + 40;
  actor->riseCounter = delay;
  actor->riseEnabled = 0;
  actor->update = (ObjectUpdate)MercuryLighthouse_LookAround;
  actor = Actor_GetById(ACTOR_OLD_WOMAN);
  delay = (Random_Below(80)) + 40;
  actor->riseCounter = delay;
  actor->riseEnabled = 2;
  actor->update = (ObjectUpdate)MercuryLighthouse_LookAround;
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  // "How is it that we can hear you?"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_SetAnimation(ACTOR_BOY, ANIM_SHAKE_HEAD);
  // "I don't know, but he told us we'd be able to talk to you if we came..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_BOY, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  // "He? Who's "he"?"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldPsynergy_StopMindReadWave();
  // "I don't know... We were sleeping, and he came to us in a dream... He..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GIRL, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  // "The lighthouse? Why?"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldPsynergy_StopMindReadWave();
  // "He said that we have to deliver a message... He was too busy to do..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_BOY, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  // "What message?"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldPsynergy_StopMindReadWave();
  // "We have to warn everyone to stay clear of the lighthouses!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GIRL, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  // "What's going to happen?"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_SetAnimationAndWait(ACTOR_BOY, ANIM_NOD);
  // "I don't know, but he told us to warn people away from Mt. Aleph as well!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_BOY, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  // "But who is "he"!? You still haven't told us!!!"
  Event_ShowMessage(ACTOR_GARET, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_Jump(ACTOR_GIRL, 2, 0);
  FieldActor_ShowEmote(ACTOR_GIRL, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  // "You're a meanie! I don't tell meanies anything!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GIRL, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  // "Don't pay any attention to him... It's me, Mia... Can you tell me..."
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_SetAnimationAndWait(ACTOR_BOY, ANIM_NOD);
  // "I told you, I don't know, but he looked like a big rock... with a..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_BOY, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  // "The... The Wise One!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  FieldPsynergy_StopMindReadWave();
  Event_Wait(20);
  // The backdrop, black.
  BG_PLTT_COLOR(0, 0) = RGB(0, 0, 0);
  wait(1);
  gFieldState->transitionFrames = 16;
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetFlag(FLAG_MARS_BEACON_BURNING);
  FieldEvent_SetExitRequest(MERCURY_LIGHTHOUSE_TO_MARS_LIGHTHOUSE_AERIE_EAST_B);
}

s32 Scene_Initialize(void) {
  if (gMainRoutineState.entrance == ENTRANCE_UNUSED_WHITE_FADE) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_PlaySound(SE_ASCEND);
    PaletteFade_SetTarget(RGB_WHITE, 0);
    PaletteFade_StartInterpolation(20);
    wait(20);
    FieldEvent_SetExitRequest(EXIT_BACK_TO_THE_VOICES);
  } else {
    MercuryLighthouse_HearVoices();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

s32 gMercuryLighthouse_GlintScript[] = {
    SC_ADD(SW_Y, toFIXED(-2)),
    SC_CALL(MercuryLighthouse_FlickerGlint),
    SC_WAIT(1),
    SC_ADD(SW_Y, toFIXED(-2)),
    SC_CALL(MercuryLighthouse_FlickerGlint),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_Y, toFIXED(-1)),
    SC_CALL(MercuryLighthouse_FlickerGlint),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(1)),
    SC_RELEASE,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(MercuryLighthouse),
    SCENE_EXIT(2, MercuryLighthouse,
               MERCURY_LIGHTHOUSE_FROM_MARS_LIGHTHOUSE_AERIE_EAST_A),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_TO_MARS_LIGHTHOUSE_AERIE_EAST_B,
               MarsLighthouseAerie,
               MARS_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EAST_B),
    SCENE_EXIT(EXIT_BACK_TO_THE_VOICES, MercuryLighthouse,
               MERCURY_LIGHTHOUSE_FROM_MARS_LIGHTHOUSE_AERIE_EAST_A),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 696, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 696, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 906, 0, 744, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 782, 0, 762, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 946, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 946, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 780, 0, 946, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_TABLE_END},
};
