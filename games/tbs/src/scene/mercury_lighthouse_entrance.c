// Mercury Lighthouse's entrance: Mia's doorway scene, the Ply mechanism,
// and the rising pillars used to cross the interior pools.

#include "script.h"
#include "animspr.h"
#include "game_state.h"
#include "global_cells.h"
#include "heap_state.h"
#include "keystate.h"
#include "unit/import.h"
#include "unit/scene/mercury_lighthouse_entrance.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/mercury_lighthouse_lights.h"
#include "unit/scene/common/mercury_lighthouse_pillar.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "iowrite.h"
#include "macros.h"

#define NULL ((void *)0)
#include "field_obj.h"
#include "mem.h"
#include "types.h"
#include "mercury.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "call.h"
#include "engine_call.h"
#include "scene_ids.h"
#include "field_effect.h"
#include "io_reg.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "iwram_call.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"

// The entrance's actors. Mia stands in her own party slot; the Lizard Man is
// the monster she cannot get past (encounter table 36 fights him as "Lizard
// Man"); the Ply target is the invisible actor Ply is cast on.
enum MercuryLighthouseEntranceActor {
  ACTOR_LIZARD_MAN = ACTOR_FIRST_PLACED,
  ACTOR_PLY_TARGET = ACTOR_FIRST_PLACED + 8
};

// Entrances with a scene of their own.
enum MercuryLighthouseEntranceEntrance {
  ENTRANCE_DROP_FROM_ABOVE = 6, // the leader drops in from above
  ENTRANCE_RISE_FROM_CHANNELS = 25,
  ENTRANCE_BEFORE_LIZARD_MAN = 30,
  ENTRANCE_AFTER_LIZARD_MAN = 31 // the return from the Lizard Man battle
};

// Exits the events take: the doorway scenes at entrances 30 and 31, and on
// to Mercury Lighthouse after rising from the channels.
enum MercuryLighthouseEntranceExit {
  EXIT_TO_LIZARD_MAN = 20,
  EXIT_AFTER_RISING = 50,
  EXIT_TO_DOORWAY = 63
};

// Scene flags, kept while the party stays here.
enum {
  FLAG_MIA_GOES_IN_SEEN = FLAG_SCENE_TEMP_50,
  FLAG_DROP_FROM_ABOVE_SEEN = FLAG_SCENE_TEMP_00 + 0x51,
  // The Lizard Man has stepped into the leader's column.
  FLAG_LIZARD_MAN_STEPPED_ASIDE = FLAG_SCENE_TEMP_05
};

// The chests: an unnamed item (0x4c) and a chest whose find kind is 5.
enum { FLAG_CHEST_ITEM_4C = 0xf65, FLAG_CHEST_EF0 = 0xef0 };

// Forward declarations.
static void MercuryLighthouseEntrance_LizardManBlocksMia(void);
static void MercuryLighthouseEntrance_MiaEntersLighthouse(void);

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"
#include "common/mercury_lighthouse_sparks.h"

// The sound of the Lizard Man's sidestep, played by all three step scripts
// as he lands.
static s32
MercuryLighthouseEntrance_PlayStepSound(struct ScriptInterpreter *interpreter) {
  Engine_AudioPlayCue(SE_118);
  return 0;
}

// Mia's update while she waits by the Lizard Man: she turns to watch the
// leader while the leader is north of 184 pixels, and looks back north at
// the doorway otherwise.
// UB: the field updater invokes a void hook. Plain void removes the zero
// result and changes the native return register (three instructions in EN).
#ifdef UBFIX
static void
MercuryLighthouseEntrance_MiaWatchesLeader(union FieldObject *fieldObject) {
#else
static s32
MercuryLighthouseEntrance_MiaWatchesLeader(union FieldObject *fieldObject) {
#endif
  struct FieldActor *object = (struct FieldActor *)fieldObject;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if ((leader->z.fixed >> 19) <= 22) {
    object->facing = ArcTan2(leader->z.fixed - object->z.fixed,
                             leader->x.fixed - object->x.fixed);
  } else if (object->facing != FACING_NORTH) {
    Engine_ActorFaceDirection(MIA, FACING_NORTH, 0);
  }
#ifndef UBFIX
  return 0;
#endif
}

// Shared source is included here in ROM order.
#include "common/mercury_lighthouse_palette_cycle.h"

// Touching trigger 21 once Ply has worked: rising sparks keep appearing
// while the leader stands on the pillar.
static void MercuryLighthouseEntrance_SpawnTimedRiseEffect(void) {
  MercuryLighthouse_TickSpawnTimer();
}

// Touching trigger 31 once Ply has worked: the raised pillar is let go.
static void MercuryLighthouseEntrance_ReleaseRaisedPillar(void) {
  SceneState_ClearCurrentRecordAndReleaseTarget();
}

// Walking into trigger 11, 21 or 31 once Ply has worked: one step across
// the water pillars in the pressed direction.
static void MercuryLighthouseEntrance_TryPillarStep(void) {
  MercuryLighthouse_RunActorMove();
}

// Touching trigger 11 once Ply has worked: the pillar underfoot rises to its
// full height.
static void MercuryLighthouseEntrance_RaiseTouchedPillar(void) {
  MercuryLighthouseEntrance_RaisePillar();
}

// The entrance positions and camera bounds.
const struct SceneEntrance *MercuryLighthouseEntrance_GetEntrances(void) {
  return gMercuryLighthouseEntrance_Entrances;
}

// This overlay has no clipping regions.
const struct SceneRegion *MercuryLighthouseEntrance_GetRegions(void) {
  return NULL;
}

// The exit destinations.
const u32 *MercuryLighthouseEntrance_GetExits(void) {
  return gMercuryLighthouseEntrance_Exits;
}

// Mia, the Lizard Man, the actors at the doorway and the Ply target.
const struct ScenePlacement *MercuryLighthouseEntrance_GetPlacements(void) {
  return gMercuryLighthouseEntrance_Placements;
}

// Searching the inscription (trigger 1): "He who honors the heart of the
// goddess..."
static void MercuryLighthouseEntrance_ReadHeartInscription(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MsgMercuryWhoHonorsHeart, 1);
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/mercury_lighthouse_risedrop.h"

// Walking north into trigger 67: the leader rises to the southern landing.
static void MercuryLighthouseEntrance_RiseToSouthernLanding(void) {
  SceneActor_RunActorZeroHandledMotion(17);
}

// Walking north into trigger 68: the leader rises to the northern landing.
static void MercuryLighthouseEntrance_RiseToNorthernLanding(void) {
  SceneActor_RunActorZeroHandledMotion(18);
}

// Walking north into trigger 69: the leader rises to the middle landing.
static void MercuryLighthouseEntrance_RiseToMiddleLanding(void) {
  SceneActor_RunActorZeroHandledMotion(19);
}

// The touch, search, talk and Psynergy events.
const struct SceneEvent *MercuryLighthouseEntrance_GetEvents(void) {
  return gMercuryLighthouseEntrance_Events;
}

// Ply on the Ply target, from cell (12, 32) only: the room dims, fifteen
// sparks rise from the pool, the lights start to cycle and the pillar
// triggers come into play.
static void MercuryLighthouseEntrance_ActivateWithPly(void) {
  s32 leaderColumn;
  s32 leaderRow;
  s32 velocityZ;
  s32 sparkIndex;
  s32 sparkX;
  s32 sparkZ;
  s32 speedRoll;
  s32 velocityY;
  struct EffectOptions *effectData;
  struct EffectOptions effectOptions;

  leaderColumn = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed / 0x100000;
  leaderRow = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed / 0x100000;
  if (leaderColumn == 12 && leaderRow == 32) {
    Engine_EventBegin();
    // Fade the scene before releasing fifteen sparks above the pool.
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(120);
    ColorBuffer_ApplyTarget(0x10005, 1);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(40);
    sparkIndex = 0;
    effectData = &effectOptions;
    velocityZ = 0;
    do {
      effectData->priority = 1;
      {
        u16 effectType = 0x11e;

        effectData->type = effectType;
      }
      effectData->script = gMercuryLighthouseEntrance_SparkBurstScript;
      Engine_AudioPlayCue(SE_246);
      sparkX = 208 - ((u32)(Random_Next() << 4) >> 16);
      sparkZ = 560 - ((u32)(Random_Next() << 4) >> 16);
      speedRoll = ((u32)(Engine_RandomNext() << 2) >> 16);
      velocityY =
          Math_Divide((((speedRoll << 4) - speedRoll) << 16) + 0x3c0000, 100);
      Effect_Spawn(sparkX << 16, 0, sparkZ << 16, 0, velocityY, velocityZ,
                   EFFECT_USE_PRIORITY | EFFECT_USE_TYPE | EFFECT_USE_SCRIPT |
                       1,
                   effectData);
      Engine_EventWait(4);
      sparkIndex++;
    } while ((u32)sparkIndex <= 14);
    Engine_AudioPlayCue(SE_220);
    Engine_EventWait(60);
    // Persist the activated state and replace the pool's map cells.
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED);
    FieldScheduler_AddCallback(MercuryLighthouse_CyclePalette, 0xc80);
    Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
    Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(120);
    Engine_EventEnd();
  }
}

// Touching trigger 10 by the doorway: the scene starts again at the
// doorway, where Mia goes in if she has not yet.
static void MercuryLighthouseEntrance_ReturnToDoorway(void) {
  Engine_EventRequestExit(EXIT_TO_DOORWAY);
}

// Touching trigger 40 or 42 while the Lizard Man stands aside: he steps back
// in front of the doorway.
static void MercuryLighthouseEntrance_LizardManReturnsToCentre(void) {
  Engine_GameFlagClear(FLAG_LIZARD_MAN_STEPPED_ASIDE);
  Actor_SetActionScript(ACTOR_LIZARD_MAN,
                        gMercuryLighthouseEntrance_LizardManStepCentre);
}

// Touching trigger 41 while he stands in the doorway: the Lizard Man steps
// into the leader's way, west from column 7 and east otherwise.
static void MercuryLighthouseEntrance_LizardManBlocksLeader(void) {
  struct FieldActor *leader;
  s32 leaderX;
  s32 leaderColumn;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  leaderX = leader->x.fixed;
  leaderColumn = leaderX / 0x100000;
  Engine_GameFlagSet(FLAG_LIZARD_MAN_STEPPED_ASIDE);
  if (leaderColumn == 7) {
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepWest);
  } else {
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepEast);
  }
}

// Talking to the Lizard Man, Mind Read on him, or walking north into
// trigger 40 or 41: he stirs and the battle begins. The party returns to
// the doorway afterwards.
static void MercuryLighthouseEntrance_FightLizardMan(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_LIZARD_MAN, 2);
  Engine_EventWait(20);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  Event_SetNextDestination((s32)&SceneId_MercuryLighthouseEntrance,
                           ENTRANCE_AFTER_LIZARD_MAN);
  Battle_SetStart(3);
  // Encounter table 36, slot 1: the Lizard Man.
  Field_RequestBattleFromEncounterTable(36, 1);
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/mercury_lighthouse_arrive.h"

// On entering: the lights, the Retreat point and the pillar routes Ply has
// opened, then the scene of the entrance taken.
s32 MercuryLighthouseEntrance_Initialize(void) {
  struct EffectOptions options;
  s32 velocity[3];
  struct FieldActor *actor;
  u32 sparkIndex;
  s32 angle;
  s32 arrivalSeen;

  Io_Write16(BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND, &REG_BLDCNT);
  {
    s32 alpha = BLEND_ALPHA(16, 16);
    REG_BLDALPHA = alpha;
  }
  // Restore persistent lighting and the Retreat destination first.
  MercuryLighthouse_SpawnLightObjects(
      21, (struct MercuryLighthouseLights *)gSceneState);
  GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_LIT);
  gGameState.retreatEntrance = 11;
  gGameState.retreatScene = (s32)&SceneId_MercuryLighthouse4;
  BattleFx_StartFadeOverlay(0);
  if (GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED))
    FieldScheduler_AddCallback(MercuryLighthouse_CyclePalette, 0xc80);
  else
    MercuryLighthouse_ClearPalette();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  // Each entrance restores its route or plays its one-time arrival.
  switch (gGameState.entrance) {
  case 1:
    // Until Mia has gone in, arriving here plays the Lizard Man scene.
    if (!GameFlag_IsSet(FLAG_MIA_ENTERED_MERCURY_LIGHTHOUSE))
      Engine_EventRequestExit(EXIT_TO_LIZARD_MAN);
    // Fall through: both doorway entrances flip actors 12 to 14.
  case 2:
    Actor_Get(12)->scaleX = -0x10000;
    Actor_Get(13)->scaleX = -0x10000;
    Actor_Get(14)->scaleX = -0x10000;
    Engine_TaskWait(1);
    break;
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
    if (GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED)) {
      Map_CopyCellAttributes(84, 5, 10, 7, 20, 5);
      Map_CopyCellAttributes(101, 5, 12, 7, 37, 5);
    }
    break;
  case 3:
  case 4:
  case 5:
  case ENTRANCE_DROP_FROM_ABOVE:
    FieldScheduler_AddCallback(MercuryLighthouse_CyclePalette, 0xc80);
    if (GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED)) {
      Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
      Engine_MapRedraw();
      Engine_TaskWait(1);
      Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
    }
    if (gGameState.entrance != ENTRANCE_DROP_FROM_ABOVE)
      break;
    arrivalSeen = GameFlag_IsSet(FLAG_DROP_FROM_ABOVE_SEEN);
    if (arrivalSeen != 0)
      break;
    // The first time, the leader drops from 130 pixels up and lands in a
    // burst; until Ply has been used, the inscription's words appear.
    GameFlag_Set(FLAG_DROP_FROM_ABOVE_SEEN);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_Get(ACTOR_PARTY_LEADER)->y.fixed = PIXELS(130);
    Actor_Get(ACTOR_PARTY_LEADER)->gravity = 0x8000;
    Actor_Get(ACTOR_PARTY_LEADER)->bounce = arrivalSeen;
    Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = arrivalSeen;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(30);
    Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 3;
    Engine_AudioPlayCue(SE_204);
    Engine_EventWait(24);
    // Scatter seventeen effects around the leader before the landing.
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    options.palette = 7;
    for (sparkIndex = 0; sparkIndex <= 16; sparkIndex++) {
      angle = sparkIndex << 12;
      SET_VECTOR(velocity, Engine_MathCos(angle), 0, Engine_MathSin(angle));
      velocity[0] -= velocity[0] / 4;
      velocity[2] -= velocity[2] / 2;
      Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocity[0],
                   velocity[1], velocity[2], EFFECT_USE_PALETTE | 1, &options);
    }
    Engine_AudioPlayCue(SE_188);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 22);
    Map_SetShake(0x50000, 0x50000, 0x10000);
    Map_SetShake(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
    Actor_Get(ACTOR_PARTY_LEADER)->gravity = 0x10000;
    Actor_Get(ACTOR_PARTY_LEADER)->bounce = 0x4000;
    if (!GameFlag_IsSet(FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED)) {
      ColorBuffer_ApplySource(0x10000, 0);
      ColorBuffer_ApplyTarget(0x10003, 1);
      Engine_ColorBufferInterpolate(30);
      Engine_EventWaitForScreen();
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Engine_EventWait(30);
      UiText_ShowCenteredMessage(MsgMercuryWhoHonorsHeart, 0, 0);
      ColorBuffer_ApplyTarget(0x10000, 0);
      Engine_ColorBufferInterpolate(30);
    }
    Engine_EventEnd();
    break;
  case 18:
  case 19:
  case 20:
    MercuryLighthouse_ArriveWithSparks();
    // Fall through: all four arrivals play the same sound.
  case 17:
    BattleFx_SetQueuedSoundAndPlay(SE_170);
    break;
  case ENTRANCE_RISE_FROM_CHANNELS:
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Engine_EventBegin();
    Engine_MapRedraw();
    Engine_TaskWait(1);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(120);
    Engine_EventRequestExit(EXIT_AFTER_RISING);
    Engine_EventEnd();
    break;
  case ENTRANCE_BEFORE_LIZARD_MAN:
    if (!GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION))
      MercuryLighthouseEntrance_LizardManBlocksMia();
    else
      Map_CopyCellAttributes(0, 0, 3, 3, 7, 9);
    break;
  case ENTRANCE_AFTER_LIZARD_MAN:
    if (!GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION))
      MercuryLighthouseEntrance_MiaEntersLighthouse();
    break;
  }
  return 0;
}

// Entrance 30, unless resuming: Mia tries to get past the Lizard Man in the
// doorway, who steps across to block her each time; then she waits, watching
// the leader come up.
static void MercuryLighthouseEntrance_LizardManBlocksMia(void) {
  s32 paceCycle;
  struct FieldActor *mia;
  struct FieldActor *actor;

  Engine_EventBegin();
  actor = Object_GetById(12);
  actor->scaleX = -0x10000;
  actor = Object_GetById(13);
  actor->scaleX = -0x10000;
  actor = Object_GetById(14);
  actor->scaleX = -0x10000;
  Actor_SetPosition(MIA, PIXELS(136), PIXELS(184));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(136), PIXELS(296));
  Actor_SetPosition(ACTOR_LIZARD_MAN, PIXELS(136), PIXELS(152));
  Actor_SetSpeed(MIA, 0x18000, 0xc000);
  Actor_SetSpeed(ACTOR_LIZARD_MAN, 0x18000, 0xc000);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Engine_CameraSetSpeed(0xcccc, 0x1999);
  Engine_CameraMoveTo(PIXELS(136), -1, PIXELS(184), 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Object_GetById(ACTOR_LIZARD_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  // Twice she tries the east side and then the west.
  for (paceCycle = 0; paceCycle < 2; paceCycle++) {
    Engine_ActorWalkTo(MIA, 152, 168);
    Engine_EventWait(10);
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepEast);
    Engine_ActorWaitForMove(MIA);
    Actor_FaceDirection(MIA, FACING_NORTH, 30);
    Engine_ActorRunRepeatedMotion(MIA, 1);
    mia = Object_GetById(MIA);
    mia->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkTo(MIA, 136, 184);
    Engine_EventWait(10);
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepCentre);
    Engine_ActorWaitForMove(MIA);
    Object_GetById(MIA)->steeringFlags |= ACTOR_FACES_MOTION;
    Engine_EventWait(30);
    Engine_ActorWalkTo(MIA, 120, 168);
    Engine_EventWait(5);
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepWest);
    Engine_ActorWaitForMove(MIA);
    Engine_ActorFaceDirection(MIA, FACING_NORTH, 30);
    Engine_ActorRunRepeatedMotion(MIA, 1);
    Engine_EventWait(15);
    Object_GetById(MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Engine_ActorWalkTo(MIA, 136, 184);
    Engine_EventWait(15);
    Actor_SetActionScript(ACTOR_LIZARD_MAN,
                          gMercuryLighthouseEntrance_LizardManStepCentre);
    Engine_ActorWaitForMove(MIA);
    Engine_ActorRunRepeatedMotion(MIA, 1);
    Object_GetById(MIA)->steeringFlags |= ACTOR_FACES_MOTION;
  }
  Engine_EventWait(20);
  Engine_ActorShowEmote(MIA, EMOTE_IN_FRONT | 2, 60);
  actor = Object_GetById(MIA);
#ifdef UBFIX
  actor->update = MercuryLighthouseEntrance_MiaWatchesLeader;
#else
  actor->update =
      (void (*)(union FieldObject *))MercuryLighthouseEntrance_MiaWatchesLeader;
#endif
  // The leader walks up behind her; the cells before the doorway become
  // walkable.
  Engine_CameraMoveToActor(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 136, 264);
  Engine_CameraWaitForMove();
  Map_CopyCellAttributes(0, 0, 3, 3, 7, 9);
  Engine_EventEnd();
}

// Entrance 31, back from the Lizard Man battle and once only: Mia nods to
// the leader and goes on into the lighthouse.
static void MercuryLighthouseEntrance_MiaEntersLighthouse(void) {
  struct FieldActor *actor;

  if (GameFlag_IsSet(FLAG_MIA_GOES_IN_SEEN) == 0) {
    GameFlag_Set(FLAG_MIA_GOES_IN_SEEN);
    Engine_EventBegin();
    actor = Actor_Get(12);
    actor->scaleX = -0x10000;
    actor = Object_GetById(13);
    actor->scaleX = -0x10000;
    actor = Actor_Get(14);
    actor->scaleX = -0x10000;
    Actor_SetPosition(MIA, PIXELS(136), PIXELS(144));
    Actor_FaceDirection(MIA, FACING_SOUTH, 10);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(60);
    Actor_FaceActor(MIA, ACTOR_PARTY_LEADER, 0);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(30);
    Actor_WalkTo(MIA, 136, 72);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorWaitForMove(MIA);
    Actor_SetPosition(MIA, 0, 0);
    GameFlag_Set(FLAG_MIA_ENTERED_MERCURY_LIGHTHOUSE);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Engine_EventEnd();
  }
}

// Shared source is included here in ROM order.
#include "common/mercury_lighthouse_lights.h"

// Spawns the short-lived rising effect at an actor's position.
static void OverlayObject_SpawnKind24AtObject(struct FieldActor *source) {
  struct FieldActor *effect = Engine_ObjectCreate(
      24, source->x.fixed, source->y.fixed, source->z.fixed);

  if (effect != 0) {
    struct AnimationObject *sprite = (struct AnimationObject *)effect->sprite;

    Engine_ObjectSetScript(effect, gMercuryLighthouseEntrance_RiseScript);
    effect->motionFlags = 0;
    effect->terrainId = 1;
    effect->priorityFlags = 2;
    if (sprite != 0) {
      AnimationObjects_SelectAnimation(sprite, 2);
      sprite->flags = 0;
      sprite->image.priority = 3;
    }
  }
}

// Shared source is included here in ROM order.
#include "common/mercury_lighthouse_pillar.h"

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gMercuryLighthouseEntrance_RiseScript[] = {
    SC_WAIT(30),
    SC_FREE,
};

s32 gMercuryLighthouse_SparkScript[] = {
    SC_CALL(SceneEffect_AdvanceAnchoredRiseFrame),
    SC_FREE,
};

s32 gMercuryLighthouse_RampScript[] = {
    SC_CALL(OverlayObject_RampWords24And28Over16Frames),
    SC_END,
};

s32 gMercuryLighthouse_ScaleCounterScript[] = {
    SC_CALL(OverlayObject_AdvanceScaleCounter),
    SC_FREE,
};

s32 gMercuryLighthouse_PillarScript[] = {
    SC_CALL(MercuryLighthouse_FollowLeader),
    SC_END,
};

u16 gMercuryLighthouse_MoveAngles[] = {
    0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, 0xffff, 0xffff, FACING_NORTH,
    FACING_SOUTH, 0xffff, 0xffff, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gMercuryLighthouseEntrance_LizardManStepCentre[] = {
    SC_SET(SW_SPEED, 0x20000),
    SC_SET(SW_ACCEL, 0x20000),
    SC_SET(SW_STEERING, 0),
    SC_ARG(2),
    SC_MOVE(PIXELS(136), 0, PIXELS(152)),
    SC_CALL(MercuryLighthouseEntrance_PlayStepSound),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gMercuryLighthouseEntrance_LizardManStepWest[] = {
    SC_SET(SW_SPEED, 0x20000),
    SC_SET(SW_ACCEL, 0x20000),
    SC_SET(SW_STEERING, 0),
    SC_ARG(2),
    SC_MOVE(PIXELS(120), 0, PIXELS(152)),
    SC_CALL(MercuryLighthouseEntrance_PlayStepSound),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gMercuryLighthouseEntrance_LizardManStepEast[] = {
    SC_SET(SW_SPEED, 0x20000),
    SC_SET(SW_ACCEL, 0x20000),
    SC_SET(SW_STEERING, 0),
    SC_ARG(2),
    SC_MOVE(PIXELS(152), 0, PIXELS(152)),
    SC_CALL(MercuryLighthouseEntrance_PlayStepSound),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gPaletteCycleRed = 0;

s32 gPaletteCycleGreen = 0;

s32 gPaletteCycleBlue = 0;

struct SceneEntrance gMercuryLighthouseEntrance_Entrances[] = {
    { 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 136, 0, 280, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
    { 2, CONDITION_ALWAYS, 136, 0, 120, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
    { 3, CONDITION_ALWAYS, 200, 0, 648, FACING_NORTH, 0, 80, 432, 320, 688, 0 },
    { 4, CONDITION_ALWAYS, 120, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
    { 5, CONDITION_ALWAYS, 280, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
    { 6, CONDITION_ALWAYS, 200, 0, 552, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
    { 7, CONDITION_ALWAYS, 360, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { 8, CONDITION_ALWAYS, 456, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { 9, CONDITION_ALWAYS, 616, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { 10, CONDITION_ALWAYS, 760, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
    { 11, CONDITION_ALWAYS, 344, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
    { 12, CONDITION_ALWAYS, 536, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
    { 13, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 744, 0, 680, FACING_NORTH, 0, 544, 512, 784, 720, 0 },
    { 14, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 584, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
    { 15, FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN, 744, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
    { 13, CONDITION_ALWAYS, 584, 0, 424, FACING_NORTH, 0, 384, 256, 624, 448, 0 },
    { 14, CONDITION_ALWAYS, 424, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
    { 15, CONDITION_ALWAYS, 584, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
    { 16, CONDITION_ALWAYS, 744, 0, 424, FACING_NORTH, 0, 704, 256, 944, 448, 0 },
    { 17, CONDITION_ALWAYS, 344, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
    { 18, CONDITION_ALWAYS, 232, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
    { 19, CONDITION_ALWAYS, 568, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
    { 20, CONDITION_ALWAYS, 696, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
    { 25, CONDITION_ALWAYS, 744, 0, 600, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
    { 30, CONDITION_ALWAYS, 136, 0, 296, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
    { 31, CONDITION_ALWAYS, 136, 0, 168, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
    { 60, CONDITION_ALWAYS, 136, 0, 152, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
    { 99, CONDITION_ALWAYS, 136, 0, 232, FACING_EAST, 0, 16, 32, 256, 320, 0 },
    { SCENE_TABLE_END },
};

u32 gMercuryLighthouseEntrance_Exits[] = {
    SCENE_EXITS(53),
    SCENE_EXIT(1, 57, 12),
    SCENE_EXIT(2, 53, 3),
    SCENE_EXIT(3, 53, 2),
    SCENE_EXIT(4, 53, 9),
    SCENE_EXIT(5, 53, 8),
    SCENE_EXIT(6, 53, 17),
    SCENE_EXIT(7, 53, 4),
    SCENE_EXIT(8, 53, 5),
    SCENE_EXIT(9, 53, 19),
    SCENE_EXIT(10, 54, 1),
    SCENE_EXIT(11, 54, 2),
    SCENE_EXIT(12, 54, 7),
    SCENE_EXIT(13, 54, 4),
    SCENE_EXIT(14, 53, 18),
    SCENE_EXIT(15, 53, 20),
    SCENE_EXIT(16, 53, 7),
    SCENE_EXIT(17, 53, 13),
    SCENE_EXIT(18, 53, 10),
    SCENE_EXIT(19, 53, 16),
    SCENE_EXIT(EXIT_TO_LIZARD_MAN, 53, ENTRANCE_BEFORE_LIZARD_MAN),
    SCENE_EXIT(EXIT_AFTER_RISING, 54, 15),
    SCENE_EXIT(EXIT_TO_DOORWAY, 53, ENTRANCE_AFTER_LIZARD_MAN),
    SCENE_EXITS_END,
};

struct ScenePlacement gMercuryLighthouseEntrance_Placements[] = {
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0xc0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe0, CONDITION_ALWAYS, 7,
        PIXELS(88), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe0, CONDITION_ALWAYS, 7,
        PIXELS(88), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe0, CONDITION_ALWAYS, 7,
        PIXELS(88), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe0, CONDITION_ALWAYS, 7,
        PIXELS(184), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe0, CONDITION_ALWAYS, 7,
        PIXELS(184), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe0, CONDITION_ALWAYS, 7,
        PIXELS(184), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(200), 0, PIXELS(512), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gMercuryLighthouseEntrance_Events[] = {
    { 0x202, 31, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED), EVENT_SCRIPT(MercuryLighthouseEntrance_TryPillarStep) },
    { EVENT_TOUCH, 31, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED), EVENT_SCRIPT(MercuryLighthouseEntrance_ReleaseRaisedPillar) },
    { 0xa02, 21, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED), EVENT_SCRIPT(MercuryLighthouseEntrance_TryPillarStep) },
    { EVENT_TOUCH, 21, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED), EVENT_SCRIPT(MercuryLighthouseEntrance_SpawnTimedRiseEffect) },
    { 0x202, 11, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED), EVENT_SCRIPT(MercuryLighthouseEntrance_TryPillarStep) },
    { EVENT_TOUCH, 11, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED), EVENT_SCRIPT(MercuryLighthouseEntrance_RaiseTouchedPillar) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_ReturnToDoorway) },
    { EVENT_TOUCH, 40, IF_FLAG_SET(FLAG_LIZARD_MAN_STEPPED_ASIDE), EVENT_SCRIPT(MercuryLighthouseEntrance_LizardManReturnsToCentre) },
    { EVENT_TOUCH, 42, IF_FLAG_SET(FLAG_LIZARD_MAN_STEPPED_ASIDE), EVENT_SCRIPT(MercuryLighthouseEntrance_LizardManReturnsToCentre) },
    { EVENT_TOUCH, 41, FLAG_LIZARD_MAN_STEPPED_ASIDE, EVENT_SCRIPT(MercuryLighthouseEntrance_LizardManBlocksLeader) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 40, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 41, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 67, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_RiseToSouthernLanding) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 68, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_RiseToNorthernLanding) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 69, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_RiseToMiddleLanding) },
    { EVENT_EXIT, 51, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 52, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 53, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 54, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 55, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 56, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 57, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 58, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 59, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 60, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 61, CONDITION_ALWAYS, 11 },
    { 0x21, 62, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 63, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 64, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 65, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 66, CONDITION_ALWAYS, 16 },
    { EVENT_TALK, MIA, CONDITION_ALWAYS, MSGNO(MsgMercuryMonsterInsideTower) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), MIA, CONDITION_ALWAYS, MSGNO(MsgMercuryLetMePass) },
    { EVENT_TALK, ACTOR_LIZARD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LIZARD_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_FightLizardMan) },
    { SEARCH(SEARCH_UNNAMED), 1, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouseEntrance_ReadHeartInscription) },
    { PSYNERGY_ON_ACTOR(ABILITY_PLY), ACTOR_PLY_TARGET, FLAG_MERCURY_LIGHTHOUSE_PLY_ACTIVATED, EVENT_SCRIPT(MercuryLighthouseEntrance_ActivateWithPly) },
    { PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(OverlayObject_ReleasePublishedAttachment) },
    { SEARCH(SEARCH_CHEST), 100, FLAG_CHEST_ITEM_4C, FIND_ITEM(0x4c) },
    { SEARCH(SEARCH_CHEST), 101, FLAG_CHEST_EF0, 0x500000 },
    { SCENE_EVENTS_END },
};

// Each spark pulses in size four times, then is released.
s32 gMercuryLighthouseEntrance_SparkBurstScript[] = {
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0xa000),
    SC_SET(SW_SCALE_X, 0xa000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0x4000),
    SC_SET(SW_SCALE_X, 0x4000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0xa000),
    SC_SET(SW_SCALE_X, 0xa000),
    SC_WAIT(6),
    SC_LOOP(4, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};
// clang-format on
