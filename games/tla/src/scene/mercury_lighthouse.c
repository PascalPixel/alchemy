/// @file
/// The base of Mercury Lighthouse in Imil, near the end of the game: with all
/// four beacons lit, two of Imil's children hear the voices of the scattered
/// heroes, motes of light drifting round them.

#include "types.h"
#include "sprite_ids.h"
#include "system/main_routine_work.h"
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
#include "field/field_sequence_event.h"
#include "scene/scene.h"

void MercuryLighthouse_HearVoices(void);

extern s32 gMercuryLighthouse_MoteScript[];
extern u32 gMercuryLighthouse_MoteFrames;

void FieldPsynergy_StopMindReadWave(void);

#include "text/msg_ids.h"

/// A mote's script call: it flickers between colours 15 and 7 every other
/// frame.
s32 MercuryLighthouse_FlickerMote(struct Actor *object) {
  if ((gMercuryLighthouse_MoteFrames >> 1) & 1)
    FieldActor_SetSpriteColor(object, 15);
  else
    FieldActor_SetSpriteColor(object, 7);
  return 0;
}

/// An Imil villager's update while the voices talk: every 40 to 120 frames
/// it turns between two headings, as if looking around for them.
void MercuryLighthouse_LookAround(struct Actor *object) {
  u8 *timer = &object->riseCounter;
  u8 *state;

  if (*timer != 0) {
    (*timer)--;
    return;
  }
  *timer = ((Random16() * 80) >> 16) + 40;
  state = &object->blockMarked;
  switch (*state) {
  case 0:
    object->facing = FACING_WEST;
    *state = 1;
    break;
  case 1:
    object->facing = FACING_NORTH_NORTHWEST;
    *state = 0;
    break;
  case 2:
    object->facing = 0;
    *state = 3;
    break;
  case 3:
    object->facing = FACING_NORTH_NORTHEAST;
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
  ACTOR_WOMAN, ///< placed nowhere
  /// "I don't know. They've been standing here telling everyone to leave..."
  ACTOR_OLD_MAN,
  ACTOR_OLD_WOMAN, ///< "Who are those two kids talking to?"
  /// Two unseen actors the voices' Mind Read wave runs between.
  ACTOR_VOICES_SOURCE,
  ACTOR_VOICES_TARGET
};

void Actor_StartScript(struct Actor *object, const void *script);

/// The task MercuryLighthouse_HearVoices starts: every 16 frames it starts a
/// flickering mote (sprite 0x2a2) at a random spot in front of the
/// lighthouse, and it counts the frames in gMercuryLighthouse_MoteFrames.
void MercuryLighthouse_SpawnMotes(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  s32 phase;
  s32 x;
  s32 z;

  Actor_SetSpritePriority(ACTOR_BOY, 3);
  Actor_SetSpritePriority(ACTOR_GIRL, 3);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  phase = gFrameCount & 15;
  if (phase == 0) {
    object = ((struct Actor *)Import_ActorCreate(0x2a2, 0, 0, 0));
    if (object != NULL) {
      sprite = object->sprite;
      x = Random16() * 40 + 0x32c0000;
      /* FAKEMATCH(temporary): z is built before the motion flag and x are
       * stored. */
      z = 0x2b80000;
      object->motionFlags = phase;
      object->x.fixed = x;
      object->y.fixed = px(160);
      object->z.fixed = z;
      Actor_SetSpriteFlags(object, 0);
      object->priorityFlags &= ~1;
      sprite->priority = 2;
      Actor_StartScript(object, gMercuryLighthouse_MoteScript);
      Actor_SelectAnimation(object, 1);
    }
  }
  gMercuryLighthouse_MoteFrames++;
}

/// With all four beacons lit, two children of Imil at the base of Mercury
/// Lighthouse hear Mia across the world by Mind Read, and the Wise One and
/// Master Hamma join the voices.
/// "We're in Imil! We're at the base of Mercury Lighthouse!"
void MercuryLighthouse_HearVoices(void) {
  struct Actor *actor;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldActor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId), 15);
  wait(1);
  gMercuryLighthouse_MoteFrames = 0;
  Task_Add(MercuryLighthouse_SpawnMotes, TASK_PASS_VBLANK);
  Actor_SetSpritePriority(ACTOR_BOY, 2);
  Actor_SetSpritePriority(ACTOR_GIRL, 2);
  gFieldWork->transitionFrames = 40;
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
  Actor_SetAnimation(ACTOR_GIRL, 3);
  // "Of course it's us! We can hear you, Mia! You're all right! I'm so happy!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GIRL, 0);
  Actor_FaceDirectionAndWait(ACTOR_OLD_WOMAN, FACING_EAST);
  // "Who are those two kids talking to?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_OLD_WOMAN, 0);
  Actor_FaceDirectionAndWait(ACTOR_OLD_MAN, FACING_WEST);
  Actor_SetAnimation(ACTOR_OLD_MAN, 4);
  // "I don't know. They've been standing here telling everyone to leave..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_OLD_MAN, 0);
  actor = Actor_GetById(ACTOR_OLD_MAN);
  actor->riseCounter = ((Random16() * 80) >> 16) + 40;
  actor->blockMarked = 0;
  actor->update = (void (*)(void))MercuryLighthouse_LookAround;
  actor = Actor_GetById(ACTOR_OLD_WOMAN);
  {
    /* FAKEMATCH(temporary): the timer's + 40 comes before its address. */
    u8 timer = ((Random16() * 80) >> 16) + 40;

    actor->riseCounter = timer;
  }
  actor->blockMarked = 2;
  actor->update = (void (*)(void))MercuryLighthouse_LookAround;
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  // "How is it that we can hear you?"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldPsynergy_StopMindReadWave();
  Actor_SetAnimation(ACTOR_BOY, 4);
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
  Actor_SetAnimationAndWait(ACTOR_BOY, 3);
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
  Actor_SetAnimationAndWait(ACTOR_BOY, 3);
  // "I told you, I don't know, but he looked like a big rock... with a..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_BOY, 0);
  FieldPsynergy_StartMindReadWave(ACTOR_VOICES_SOURCE, ACTOR_VOICES_TARGET);
  // "The... The Wise One!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  FieldPsynergy_StopMindReadWave();
  Event_Wait(20);
  *(u16 *)BG_PLTT = 0;
  wait(1);
  gFieldWork->transitionFrames = 16;
  gFieldWork->transition = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetFlag(0xa3c);
  Import_EventSetExitRequest(3);
}

s32 Scene_Initialize(void) {
  if (gMainRoutineWork.entrance == 12) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_PlaySound(SE_ASCEND);
    PaletteFade_SetTarget(0x7fff, 0);
    PaletteFade_StartInterpolation(20);
    wait(20);
    Import_EventSetExitRequest(9);
  } else {
    MercuryLighthouse_HearVoices();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

s32 gMercuryLighthouse_MoteScript[] = {
    SC_ADD(SW_Y, px(-2)),
    SC_CALL(MercuryLighthouse_FlickerMote),
    SC_WAIT(1),
    SC_ADD(SW_Y, px(-2)),
    SC_CALL(MercuryLighthouse_FlickerMote),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_Y, px(-1)),
    SC_CALL(MercuryLighthouse_FlickerMote),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(1)),
    SC_RELEASE,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(MercuryLighthouse),
    SCENE_EXIT(2, MercuryLighthouse, 2),
    SCENE_EXIT(3, MarsLighthouseAerie, 5),
    SCENE_EXIT(9, MercuryLighthouse, 2),
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
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_TABLE_END},
};
