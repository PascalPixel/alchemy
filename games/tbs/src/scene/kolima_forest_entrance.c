/// @file
/// Kolima Forest (SceneId_KolimaForestEntrance), the first of the forest's five
/// maps, entered from the world map (KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP).
/// If the party comes here before it has been to Kolima, the curse's scene
/// plays here instead of in the village
/// (KolimaForestEntrance_WitnessForestCurse, until
/// FLAG_KOLIMA_CURSE_WITNESSED): sparkles of Tret's glamour rain down, fields
/// of the party's own Psynergy shield it, and Tret and Laurel, unseen, speak of
/// the dying forest. Five sparkles lie on the ground until Tret lifts the curse
/// (FLAG_KOLIMA_CURSE_LIFTED).
///
/// Three logs lie here, rolled by walking into them; the one pushed west to the
/// water's edge tips into the river and floats there, cells the party can
/// cross, and from the bank the leader can hop east (trigger 11,
/// KolimaForest_HopEast). KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE leads
/// on into the forest (SceneId_KolimaForestGlade),
/// KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP back to the world map.
///
/// FLAG_SCENE_TEMP_00: the log is in the river. Like every scene flag it is
/// cleared when the party next comes in, so the log is back on the bank, but a
/// battle keeps it.
///
/// Open questions:
/// - XXX: what the river log's animations 3 and 8 show as it tips into the
///   water and floats; a render of KolimaForestEntrance_PushLog would name
///   them beside the log sprites' other animations.
/// - XXX: what SE_240, as the log falls in, SE_200, the glamour's tinkle, and
///   SE_106, as a sparkle bursts on a field, sound like; a recording of the
///   curse here would name them.
/// - XXX (picture): the fields of Psynergy flickering two on even frames and
///   two on odd (KolimaForestEntrance_FlickerPsynergyFields), and the areas
///   KolimaForestEntrance_BurstOnPsynergyField tests with and without Mia.

#include "script.h"
#include "flags.h"
#include "character_ids.h"
#include "game_state.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/kolima_forest_entrance.h"
#include "unit/scene/common/kolima_forest_bridge_falling_effect.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "types.h"
#include "tbs_edition.h"
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "kolima.h"
#include "staged_actor.h"
#include "text/msg_ids.h"
#include "objcmd.h"
#include "songs.h"
#include "fixed_math.h"

/// The map's actors after Garet's, Ivan's and Mia's party slots, numbering the
/// rows of gKolimaForestEntrance_Placements from ACTOR_FIRST_PLACED.
enum {
  /// The three logs; the last is the one that can reach the river.
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  ACTOR_RIVER_LOG = ACTOR_FIRST_LOG + 2,
  /// Tret's wrathful side, unseen while the curse falls.
  /// ★ Named from the game: "Tret was once a kindly forest king."
  /// (MsgKolimaWatchOutItsHappeningAgain)
  ACTOR_TRET,
  /// Unseen while the curse falls.
  /// ★ Named from the game: "We are not the only ones who will die, Laurel."
  /// (MsgKolimaWatchOutItsHappeningAgain)
  ACTOR_LAUREL,
  /// The fields of Psynergy, one over each member; Mia's only with Mia.
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  ACTOR_LEADER_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  ACTOR_IVAN_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  ACTOR_MIA_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  ACTOR_GARET_PSYNERGY_FIELD,
  /// The sparkles of Tret's glamour on the ground, gone once the curse is
  /// lifted.
  /// ★ Named from the game: "I turned into a tree right when the sparkles
  /// fell from the sky." (MsgKolimaITurnedIntoATreeRight)
  ACTOR_FIRST_GROUND_SPARKLE,
  ACTOR_LAST_GROUND_SPARKLE = ACTOR_FIRST_GROUND_SPARKLE + 4
};

static void KolimaForestEntrance_WitnessForestCurse(void);
static void KolimaForestEntrance_ShowMessageAndWait(s32 speaker, s32 frames);
static void KolimaForestEntrance_FaceActorAndWait(s32 actorId, s32 facing,
                                                  s32 waitFrames);
static void KolimaForestEntrance_SpawnCurseSparkle(void);
static void KolimaForestEntrance_FlickerPsynergyFields(void);

extern s32 gKolimaForestEntrance_PsynergyFieldHit;
extern s32 gKolimaForestEntrance_MiaInParty;
extern s32 gKolimaForestEntrance_SparkleSoundOn;

#include "common/object_staged_actor.h"

/// The end of a field's growing: if a sparkle has burst on a field, the field
/// takes its next pose.
static s32 KolimaForestEntrance_FlashHitPsynergyFields(
    struct ScriptInterpreter *interpreter) {
  struct FieldActor *object = (struct FieldActor *)interpreter;
  s32 *pending = &gKolimaForestEntrance_PsynergyFieldHit;
  if (*pending) {
    Object_SetMode(object, 2);
    *pending = 0;
  }
  return 1;
}

/// Each frame: a ground sparkle's timer runs on at random, and the sparkle
/// shows its bright frame (7) while the timer is past 1000, its dim one (10)
/// before; at 1200 the timer starts over.
static s32 KolimaForestEntrance_TwinkleGroundSparkle(
    struct ScriptInterpreter *interpreter) {
  struct FieldActor *object = (struct FieldActor *)interpreter;
  s16 *counter = &object->timer;
  *counter = (*counter + (((u32)Engine_RandomNext() * 100) >> 16));
  if (*counter > 1000)
    ObjectGroup_SetChildValue(object, 7);
  else
    ObjectGroup_SetChildValue(object, 10);
  if (*counter > 1200) {
    u16 z = 0;
    *counter = z;
  }
  return 1;
}

/// Where the party enters, from the next map of the forest
/// (SceneId_KolimaForestGlade) and from the world map.
const struct SceneEntrance *KolimaForestEntrance_GetEntrances(void) {
  return gKolimaForestEntrance_Entrances;
}

const struct SceneRegion *KolimaForestEntrance_GetRegions(void) { return 0; }

/// The ways on into the forest (SceneId_KolimaForestGlade) and back to the
/// world map.
const u32 *KolimaForestEntrance_GetExits(void) {
  return gKolimaForestEntrance_Exits;
}

/// The party's companions, the logs, Tret, Laurel, the Psynergy fields and the
/// ground sparkles.
const struct ScenePlacement *KolimaForestEntrance_GetPlacements(void) {
  return gKolimaForestEntrance_Placements;
}

/// Kolima Forest (SceneId_KolimaForestEntrance), walking into a log: it rolls
/// ahead until something stops it. The river log, stopped at the bank (cell
/// column 20), tips over with a splash and floats in the water; its cells
/// become ground the party can cross (sets FLAG_SCENE_TEMP_00).
static void KolimaForestEntrance_PushLog(void) {
  struct StagedActorProbe res;
  Engine_EventBegin();

  if (StagedActor_FindClearPosition(&res)) {
    SceneActor_MoveAndRedraw(res);
    if (res.actorSlot == ACTOR_RIVER_LOG && (res.position.x >> 20) == 20) {
      struct FieldActor *actor;

      Engine_ActorSetAnimation(ACTOR_RIVER_LOG, 3);
      Actor_SetDestinationOffset(ACTOR_RIVER_LOG, -18, 6);
      Engine_EventWait(30);
      Engine_AudioPlayCue(SE_240);
      Engine_ActorSetAnimation(ACTOR_RIVER_LOG, 8);
      Object_GetById(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Map_CopyCellAttributes(0, 17, 2, 4, 19, 17);
      StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, 0);
      Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
      actor = Object_GetById(ACTOR_RIVER_LOG);
      Engine_ActorSetSpriteFlags(actor, 0);
    }
  }

  Engine_EventEnd();
}

#include "common/kolima_jump.h"

/// The logs, the hop east from the bank and the two ways out.
const struct SceneEvent *KolimaForestEntrance_GetEvents(void) {
  return gKolimaForestEntrance_Events;
}

/// On entry a log already in the river floats there again and the logs mark
/// the cells they cover. From the world map, before the curse has been seen,
/// the party sees it fall; once the curse is lifted the ground sparkles are
/// gone.
s32 KolimaForestEntrance_Initialize(void) {
  FieldScene_RedrawActorFootprint(ACTOR_RIVER_LOG);
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) != 0) {
    Object_GetById(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Map_CopyCellAttributes(0, 17, 2, 4, 19, 17);
    StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_RIVER_LOG), 0);
  }
  FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG);
  FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG + 1);
  if (gGameState.entrance == KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP) {
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_WITNESSED) == 0) {
      KolimaForestEntrance_WitnessForestCurse();
    }
  }
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_ActorSetPosition(ACTOR_FIRST_GROUND_SPARKLE, 0, 0);
    Engine_ActorSetPosition(ACTOR_FIRST_GROUND_SPARKLE + 1, 0, 0);
    Engine_ActorSetPosition(ACTOR_FIRST_GROUND_SPARKLE + 2, 0, 0);
    Engine_ActorSetPosition(ACTOR_FIRST_GROUND_SPARKLE + 3, 0, 0);
    Engine_ActorSetPosition(ACTOR_LAST_GROUND_SPARKLE, 0, 0);
  }
  return 0;
}

/// Kolima Forest, the first entry from the world map before the curse has been
/// seen in Kolima (FLAG_KOLIMA_CURSE_WITNESSED clear): the party sees the
/// curse fall, the same scene as Kolima_WitnessForestCurse without the trees
/// that were people. Ivan notices sparkly stuff on the ground; sparkles rain
/// from the sky, the party sinks under them, and fields of Psynergy rise around
/// each member before the sparkles reach them. They argue over whether it was
/// their Psynergy, the sparkles fall again and the Psynergy fields rise again,
/// and Tret and Laurel speak: Tret turned Kolima's people into trees for
/// cutting him, and Laurel tells them to reawaken his gentle side and look for
/// Tret deep within the forest. Lines and choices for Mia are skipped when she
/// is not in the party. Sets FLAG_KOLIMA_CURSE_WITNESSED.
static void KolimaForestEntrance_WitnessForestCurse(void) {
  struct FieldActor *record;
  s32 *party;
  const s32 *sparkleAction;
  const s32 *squashAction;
  s32 *miaInParty;
  const s32 *restoreScaleAction;
  const s32 *growFieldAction;
  const s32 *shrinkFieldAction;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Camera_MoveTo(PIXELS(246), -1, PIXELS(604), 0);
  party = &gKolimaForestEntrance_MiaInParty;
  *party = GameFlag_IsSet(FLAG_IN_PARTY(MIA));
  record = Object_GetById(ACTOR_LEADER_PSYNERGY_FIELD);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(ACTOR_IVAN_PSYNERGY_FIELD);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(ACTOR_MIA_PSYNERGY_FIELD);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(ACTOR_GARET_PSYNERGY_FIELD);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(ACTOR_FIRST_GROUND_SPARKLE);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(ACTOR_FIRST_GROUND_SPARKLE + 1);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(ACTOR_FIRST_GROUND_SPARKLE + 2);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(ACTOR_FIRST_GROUND_SPARKLE + 3);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(ACTOR_LAST_GROUND_SPARKLE);
  Engine_ActorSetSpriteFlags(record, 0);
  sparkleAction = gKolimaForestEntrance_GroundSparkleAction;
  Actor_SetActionScript(ACTOR_FIRST_GROUND_SPARKLE, sparkleAction);
  Actor_SetActionScript(ACTOR_FIRST_GROUND_SPARKLE + 1, sparkleAction);
  Actor_SetActionScript(ACTOR_FIRST_GROUND_SPARKLE + 2, sparkleAction);
  Actor_SetActionScript(ACTOR_FIRST_GROUND_SPARKLE + 3, sparkleAction);
  Actor_SetActionScript(ACTOR_LAST_GROUND_SPARKLE, sparkleAction);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(116), PIXELS(602));
  Engine_TaskWait(1);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 254, 0x251);
  Actor_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
    }
  }
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
    }
  }
  Actor_SetActionScript(GARET, gKolimaForestEntrance_GaretAction);
  Actor_SetActionScript(IVAN, gKolimaForestEntrance_IvanAction);
  if (*party != 0) {
    Actor_SetSpeed(MIA, PIXELS(3) / 5, PIXELS(3) / 10);
    {
      struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

      if (leader != 0) {
        Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
      }
    }
    Actor_SetActionScript(MIA, gKolimaForestEntrance_MiaAction);
  }
  Object_RefreshSelectorById(2);
  KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_SOUTHEAST, 40);
  KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_WEST, 20);
  KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_SOUTH, 40);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        60);
  KolimaForestEntrance_FaceActorAndWait(MIA, FACING_SOUTHEAST, 10);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventSetMessage(MsgKolimaMatterIvan);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 10);
  Engine_EventSetMessage(MsgKolimaSparklyStuffOnGround);
  KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTH, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_EAST, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        40);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTH, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        30);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTHWEST, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHEAST,
                                        30);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        20);
  KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  Event_PlayCue(MUS_017);
  Event_PlayCue(SE_206);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  gKolimaForestEntrance_SparkleSoundOn = 1;
  FieldScheduler_AddCallback(KolimaForestEntrance_SpawnCurseSparkle,
                             TASK_PRIORITY_SCENE);
  Engine_TaskWait(20);
  ColorBuffer_ApplyTarget(0x405210, 1);
  ColorBuffer_ApplyTarget(0x10000, 2);
  Engine_ColorBufferInterpolate(120);
  Engine_TaskWait(60);
  squashAction = gKolimaForestEntrance_SquashAction;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, squashAction);
  Actor_SetActionScript(GARET, squashAction);
  Actor_SetActionScript(IVAN, squashAction);
  Actor_SetActionScript(MIA, squashAction);
  Engine_EventWait(100);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 40);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Engine_EventWait(40);
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_EventWait(40);
    KolimaForestEntrance_ShowMessageAndWait(MIA, 40);
  } else {
    gEventWork->message += 1;
  }
  Engine_EventWait(20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    record = Object_GetById(MIA);
    record->velocityY = PIXELS(2);
    Engine_EventWait(10);
    Actor_SetSpeed(MIA, PIXELS(2), PIXELS(2));
    Actor_SetDestinationOffset(MIA, -2, 0);
    Actor_SetActionScript(MIA, gKolimaForestEntrance_RestoreScaleAction);
    record = Object_GetById(MIA);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(MIA, 19);
    Engine_EventWait(10);
  }
  record = Object_GetById(0);
  record->velocityY = PIXELS(2);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(2));
  restoreScaleAction = gKolimaForestEntrance_RestoreScaleAction;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, restoreScaleAction);
  record = Object_GetById(0);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Engine_EventWait(20);
  record = Object_GetById(GARET);
  record->velocityY = PIXELS(2);
  Engine_EventWait(10);
  Actor_SetSpeed(GARET, PIXELS(2), PIXELS(2));
  Actor_SetActionScript(GARET, restoreScaleAction);
  record = Object_GetById(GARET);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_ActorSetAnimation(GARET, 19);
  Engine_EventWait(40);
  record = Object_GetById(IVAN);
  record->velocityY = PIXELS(2);
  Engine_EventWait(10);
  Actor_SetActionScript(IVAN, restoreScaleAction);
  record = Object_GetById(IVAN);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_ActorSetAnimation(IVAN, 19);
  gKolimaForestEntrance_SparkleSoundOn = 0;
  Engine_EventWait(160);
  Engine_TaskRemoveCallback(KolimaForestEntrance_SpawnCurseSparkle);
  Engine_EventWait(120);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = 0x800000;
  gFallingEffectState = KOLIMA_DUST_CLOSING;
  FieldScheduler_AddCallback(Kolima_UpdateCurseDust, TASK_PRIORITY_SCENE);
  Engine_EventWait(180);
  Event_PlayCue(MUS_021);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 80);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(60);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
  gFallingEffectState = KOLIMA_DUST_CLOSING_WIDE;
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(IVAN, 3);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    KolimaForestEntrance_ShowMessageAndWait(MIA, 10);
  } else {
    gEventWork->message += 1;
  }
  gFallingEffectState = KOLIMA_DUST_DRIFTING;
  Object_GetById(0)->priorityFlags &= 254;
  Object_GetById(GARET)->priorityFlags &= 254;
  Object_GetById(IVAN)->priorityFlags &= 254;
  Object_GetById(MIA)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetSpritePriority(GARET, 3);
  Engine_ActorSetSpritePriority(IVAN, 3);
  Engine_ActorSetSpritePriority(MIA, 3);
  gKolimaForestEntrance_PsynergyFieldHit = 0;
  FieldScheduler_AddCallback(KolimaForestEntrance_FlickerPsynergyFields,
                             TASK_PRIORITY_SCENE);
  Event_PlayCue(SE_220);
  Object_GetById(ACTOR_LEADER_PSYNERGY_FIELD)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_LEADER_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_LEADER_PSYNERGY_FIELD, PIXELS(253), PIXELS(603));
  growFieldAction = gKolimaForestEntrance_PsynergyFieldGrowAction;
  Actor_SetActionScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldAction);
  Object_GetById(ACTOR_IVAN_PSYNERGY_FIELD)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_IVAN_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_IVAN_PSYNERGY_FIELD, PIXELS(233), PIXELS(629));
  Actor_SetActionScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldAction);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Object_GetById(ACTOR_MIA_PSYNERGY_FIELD)->priorityFlags &= 254;
    Engine_ActorSetSpritePriority(ACTOR_MIA_PSYNERGY_FIELD, 2);
    Actor_SetPosition(ACTOR_MIA_PSYNERGY_FIELD, PIXELS(207), PIXELS(609));
    Actor_SetActionScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldAction);
  }
  Object_GetById(ACTOR_GARET_PSYNERGY_FIELD)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_GARET_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_GARET_PSYNERGY_FIELD, PIXELS(227), PIXELS(580));
  Actor_SetActionScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldAction);
  if (gFallingEffectState != KOLIMA_DUST_CENTRED) {
    do {
      Engine_TaskWait(1);
    } while (gFallingEffectState != KOLIMA_DUST_CENTRED);
  }
  Engine_EventWait(300);
  Engine_TaskRemoveCallback(Kolima_UpdateCurseDust);
  Engine_EventWait(120);
  Event_PlayCue(MUS_017);
  ColorBuffer_ApplyTarget(0x10000, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  Engine_ActorStop(ACTOR_LEADER_PSYNERGY_FIELD);
  Engine_ActorStop(ACTOR_IVAN_PSYNERGY_FIELD);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Engine_ActorStop(ACTOR_MIA_PSYNERGY_FIELD);
  }
  Engine_ActorStop(ACTOR_GARET_PSYNERGY_FIELD);
  Engine_TaskWait(1);
  shrinkFieldAction = gKolimaForestEntrance_PsynergyFieldShrinkAction;
  Actor_SetActionScript(ACTOR_LEADER_PSYNERGY_FIELD, shrinkFieldAction);
  Actor_SetActionScript(ACTOR_IVAN_PSYNERGY_FIELD, shrinkFieldAction);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_SetActionScript(ACTOR_MIA_PSYNERGY_FIELD, shrinkFieldAction);
  }
  Actor_RunActionScriptAndWait(ACTOR_GARET_PSYNERGY_FIELD, shrinkFieldAction);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Event_OpenMessage(GARET, 0);
  Actor_SetPosition(ACTOR_TRET, PIXELS(220), PIXELS(494));
  Actor_SetPosition(ACTOR_LAUREL, PIXELS(220), PIXELS(494));
  Engine_TaskWait(1);
  if (Engine_EventChooseYesNo(11, 0) == 1) {
    gEventWork->message += 1;
  }
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKolimaKnowThoseFieldsWere);
    KolimaForestEntrance_ShowMessageAndWait(MIA, 40);
  }
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventSetMessage(MsgKolimaWasOurPsynergy);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 40);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(40);
  Engine_ActorSetSpritePriority(GARET, 2);
  Object_GetById(GARET)->priorityFlags |= 1;
  record = Object_GetById(GARET);
  Engine_ActorSetSpriteFlags(record, 1);
  Engine_ActorJump(GARET, 6, 0);
  Actor_SetDestinationOffset(GARET, -3, 0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTH, 60);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTHEAST, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(40);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTHWEST, 40);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTHEAST, 20);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTHWEST, 20);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(40);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(10);
  Engine_ActorJump(GARET, 4, 0);
  Engine_EventWait(20);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(80);
    Engine_ActorSetSpritePriority(MIA, 2);
    Object_GetById(MIA)->priorityFlags |= 1;
    record = Object_GetById(MIA);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_ActorJump(MIA, 4, 0);
    Actor_SetDestinationOffset(MIA, -2, 0);
    Engine_ActorSetAnimation(MIA, ANIM_STAND);
    KolimaForestEntrance_FaceActorAndWait(MIA, FACING_NORTHEAST, 60);
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(20);
    KolimaForestEntrance_ShowMessageAndWait(MIA, 20);
  } else {
    gEventWork->message += 1;
  }
  Engine_ActorJump(GARET, 2, 0);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTH, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_ActorSetSpritePriority(IVAN, 2);
  Object_GetById(IVAN)->priorityFlags |= 1;
  record = Object_GetById(IVAN);
  Engine_ActorSetSpriteFlags(record, 1);
  Engine_ActorJump(IVAN, 4, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Object_GetById(0)->priorityFlags |= 1;
  record = Object_GetById(0);
  Engine_ActorSetSpriteFlags(record, 1);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_EventWait(60);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        10);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTH, 10);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  Event_OpenMessage(IVAN, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimation(IVAN, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    gEventWork->message += 1;
  } else {
    Engine_ActorRunRepeatedMotion(GARET, 2);
    KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Event_ShowMessage(GARET, 0);
  }
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTH, 10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Engine_ActorRunRepeatedMotion(MIA, 2);
    KolimaForestEntrance_FaceActorAndWait(MIA, FACING_EAST, 20);
    KolimaForestEntrance_FaceActorAndWait(MIA, FACING_SOUTHEAST, 10);
    Engine_ActorSetAnimation(MIA, ANIM_SHAKE_HEAD);
    KolimaForestEntrance_ShowMessageAndWait(MIA, 10);
  } else {
    gEventWork->message += 1;
  }
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(80);
  KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTHEAST, 10);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        10);
  KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(40);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
  Event_OpenMessage(GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(40);
    gEventWork->message += 1;
  }
  Event_ShowMessage(GARET, 0);
  Event_PlayCue(MUS_021);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = 0x800000;
  gFallingEffectState = KOLIMA_DUST_CLOSING;
  FieldScheduler_AddCallback(Kolima_UpdateCurseDust, TASK_PRIORITY_SCENE);
  Engine_EventWait(80);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(60);
  KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  Engine_EventSetMessage(MsgKolimaWatchOutItsHappeningAgain);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
  KolimaForestEntrance_FaceActorAndWait(GARET, FACING_NORTH, 10);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    KolimaForestEntrance_FaceActorAndWait(MIA, FACING_NORTH, 10);
  }
  Object_GetById(0)->priorityFlags &= 254;
  Object_GetById(GARET)->priorityFlags &= 254;
  Object_GetById(IVAN)->priorityFlags &= 254;
  Object_GetById(MIA)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetSpritePriority(GARET, 3);
  Engine_ActorSetSpritePriority(IVAN, 3);
  Engine_ActorSetSpritePriority(MIA, 3);
  gFallingEffectState = KOLIMA_DUST_CLOSING_WIDE;
  Event_PlayCue(SE_220);
  Actor_SetPosition(ACTOR_LEADER_PSYNERGY_FIELD, PIXELS(253), PIXELS(603));
  growFieldAction = gKolimaForestEntrance_PsynergyFieldGrowAction;
  Actor_SetActionScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldAction);
  Actor_SetPosition(ACTOR_IVAN_PSYNERGY_FIELD, PIXELS(233), PIXELS(629));
  Actor_SetActionScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldAction);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_SetPosition(ACTOR_MIA_PSYNERGY_FIELD, PIXELS(207), PIXELS(609));
    Actor_SetActionScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldAction);
  }
  Actor_SetPosition(ACTOR_GARET_PSYNERGY_FIELD, PIXELS(227), PIXELS(580));
  Actor_SetActionScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldAction);
  Engine_EventWait(120);
  gFallingEffectState = KOLIMA_DUST_DRIFTING;
  do {
    Engine_TaskWait(1);
  } while (gFallingEffectState != KOLIMA_DUST_CENTRED);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 80);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(60);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_EventWait(40);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTH, 40);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(MIA, FACING_NORTH, 80);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(MIA, FACING_EAST, 40);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(MIA, FACING_NORTH, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimation(GARET, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimation(MIA, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(60);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(MIA, FACING_EAST, 20);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(MIA, FACING_EAST, 20);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(40);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(MIA, FACING_NORTH, 10);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(60);
  Event_ShowMessage(ACTOR_LAUREL, 0);
  Event_ShowMessage(ACTOR_TRET, 0);
  Engine_TaskRemoveCallback(Kolima_UpdateCurseDust);
  Engine_EventWait(80);
  ColorBuffer_ApplyTarget(0x10000, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(80);
  Engine_ActorStop(ACTOR_LEADER_PSYNERGY_FIELD);
  Engine_ActorStop(ACTOR_IVAN_PSYNERGY_FIELD);
  miaInParty = &gKolimaForestEntrance_MiaInParty;
  Engine_ActorStop(ACTOR_MIA_PSYNERGY_FIELD);
  Engine_ActorStop(ACTOR_GARET_PSYNERGY_FIELD);
  Engine_TaskWait(1);
  shrinkFieldAction = gKolimaForestEntrance_PsynergyFieldShrinkAction;
  Actor_SetActionScript(ACTOR_LEADER_PSYNERGY_FIELD, shrinkFieldAction);
  Actor_SetActionScript(ACTOR_IVAN_PSYNERGY_FIELD, shrinkFieldAction);
  if (*miaInParty != 0) {
    Actor_SetActionScript(ACTOR_MIA_PSYNERGY_FIELD, shrinkFieldAction);
  }
  Actor_RunActionScriptAndWait(ACTOR_GARET_PSYNERGY_FIELD, shrinkFieldAction);
  Engine_EventWait(20);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetSpritePriority(GARET, 2);
  Engine_ActorSetSpritePriority(IVAN, 2);
  Engine_ActorSetSpritePriority(MIA, 2);
  Object_GetById(0)->priorityFlags |= 1;
  Object_GetById(GARET)->priorityFlags |= 1;
  Object_GetById(IVAN)->priorityFlags |= 1;
  Object_GetById(MIA)->priorityFlags |= 1;
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTHEAST, 10);
  Event_OpenMessage(IVAN, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(10);
    Event_OpenMessage(GARET, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      KolimaForestEntrance_FaceActorAndWait(MIA, FACING_EAST, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Engine_EventWait(40);
      KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTH, 20);
      KolimaForestEntrance_ShowMessageAndWait(GARET, 10);
      KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTH, 20);
      KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTHEAST, 20);
      Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
      KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTHEAST, 20);
    } else {
      KolimaForestEntrance_FaceActorAndWait(MIA, FACING_EAST, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Engine_EventWait(40);
      KolimaForestEntrance_FaceActorAndWait(GARET, FACING_SOUTH, 20);
      Engine_EventSetMessage(MsgKolimaHesAsStumpedAsWe);
      KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
      Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
    }
    Engine_ActorSetAnimation(MIA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKolimaYoureIsaacThereIsntMuch);
    KolimaForestEntrance_ShowMessageAndWait(GARET, 10);
    Actor_FaceDirection(GARET, FACING_SOUTH, 0);
    KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                          20);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Event_OpenMessage(IVAN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
      Engine_EventWait(40);
      KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTHEAST, 10);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
      if (*miaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(MIA, FACING_EAST, 10);
        Engine_ActorStartRepeatedMotion(MIA, 3);
        KolimaForestEntrance_ShowMessageAndWait(MIA, 20);
      } else {
        gEventWork->message += 1;
      }
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Engine_EventWait(120);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 40);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(MIA, FACING_SOUTHEAST, 10);
        Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
        KolimaForestEntrance_ShowMessageAndWait(MIA, 10);
      } else {
        gEventWork->message += 1;
      }
      Engine_EventWait(60);
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTHWEST, 40);
        KolimaForestEntrance_FaceActorAndWait(IVAN, FACING_NORTHEAST, 20);
      }
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
      Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Engine_EventWait(40);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
      Engine_EventWait(20);
      Engine_ActorSetAnimation(MIA, ANIM_NOD);
      Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    } else {
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Engine_EventWait(40);
      Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
      Engine_EventSetMessage(MsgKolimaThatsReliefIsaacThoughtYoud);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
      if (*miaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(MIA, FACING_EAST, 10);
        Engine_ActorStartRepeatedMotion(MIA, 1);
        KolimaForestEntrance_ShowMessageAndWait(MIA, 20);
      } else {
        gEventWork->message += 1;
      }
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Engine_EventWait(80);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 40);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(MIA, FACING_SOUTHEAST, 20);
        Engine_ActorSetAnimation(MIA, ANIM_SHAKE_HEAD);
        KolimaForestEntrance_ShowMessageAndWait(MIA, 40);
      } else {
        gEventWork->message += 1;
      }
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      Engine_EventWait(20);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
      Engine_ActorStartRepeatedMotion(GARET, 2);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_EventWait(40);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
    }
  }
  Event_PlayCue(MUS_017);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
    }
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
    }
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Engine_ActorSetAnimation(MIA, ANIM_WALK);
    {
      struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

      if (leader != 0) {
        Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
      }
    }
    Engine_ActorWaitForMove(MIA);
    Actor_SetPosition(MIA, 0, 0);
  }
  GameFlag_Set(FLAG_KOLIMA_CURSE_WITNESSED);
  Actor_SetPosition(ACTOR_TRET, 0, 0);
  Actor_SetPosition(ACTOR_LAUREL, 0, 0);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

static void KolimaForestEntrance_ShowMessageAndWait(s32 speaker, s32 frames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(frames);
}

/// Turns an actor at once, then leaves time for the next beat.
static void KolimaForestEntrance_FaceActorAndWait(s32 actorId, s32 facing,
                                                  s32 waitFrames) {
  Engine_ActorFaceDirection(actorId, facing, 0);
  Engine_EventWait(waitFrames);
}

#include "common/kolima_grow.h"

/// Every eighth frame a sparkle of Tret's glamour (SPRITE_GOLDEN_RINGS) falls
/// toward the party, with its tinkle while the rain is at its height.
static void KolimaForestEntrance_SpawnCurseSparkle(void) {
  struct FieldActor *sparkle;
  struct AnimationObject *sprite;
  s32 framePhase;
  s32 priorityFlags;
  s32 spawnX = PIXELS(231);
  s32 spawnZ = PIXELS(460);
  s32 targetX = PIXELS(231);
  s32 targetZ = PIXELS(624);

  framePhase = gFrameCount & 7;
  if (framePhase != 0)
    return;
  if (gKolimaForestEntrance_SparkleSoundOn != 0)
    Engine_AudioPlayCue(SE_200);
  sparkle = Engine_ObjectCreate(SPRITE_GOLDEN_RINGS, spawnX, 0, spawnZ);
  if (sparkle == 0)
    return;
  sprite = sparkle->sprite;
  sprite->flags = framePhase;
  priorityFlags = 0xff & ~ACTOR_PRIORITY_AUTOMATIC;
  priorityFlags &= sparkle->priorityFlags;
  sparkle->priorityFlags = priorityFlags;
  sprite->image.priority = 1;
  sparkle->scaleX = Q16_ONE / 10;
  sparkle->speed = PIXELS(8);
  sparkle->acceleration = PIXELS(8);
  sparkle->motionFlags = framePhase;
  Object_SetMode(sparkle, 2);
  Engine_ObjectSetPosition(sparkle, targetX, 0, targetZ);
  Engine_ObjectSetScript(sparkle, gKolimaForestEntrance_SparkleScript);
}

#include "common/kolima_blink.h"

/// The end of a sparkle's fall: one that lands on a field (the Psynergy fields'
/// area, which differs with Mia) bursts with a sound, and the Psynergy fields
/// are told.
static s32 KolimaForestEntrance_BurstOnPsynergyField(
    struct ScriptInterpreter *interpreter) {
  struct FieldActor *arg0 = (struct FieldActor *)interpreter;
  s32 x;

  if (!(gKolimaForestEntrance_MiaInParty != 0
            ? (x = arg0->x.fixed,
               (x > 0xc00000 && x < 0x1120000 && arg0->z.fixed > 0x2360000 &&
                arg0->z.fixed < 0x2640000) ||
                   (x > 0xca0000 && x < 0xff0000 && arg0->z.fixed > 0x2250000 &&
                    arg0->z.fixed < 0x2780000))
            : (x = arg0->x.fixed,
               (x > 0xc00000 && x < 0xf40000 && arg0->z.fixed > 0x2250000 &&
                arg0->z.fixed <= 0x248ffff) ||
                   (x > 0xf40000 && x < 0x1120000 &&
                    arg0->z.fixed > 0x23b0000 && arg0->z.fixed <= 0x25cffff) ||
                   (x > 0xd30000 && x < 0xff0000 && arg0->z.fixed > 0x2540000 &&
                    arg0->z.fixed < 0x2780000)))) {
    return 0;
  }
  Engine_AudioPlayCue(SE_106);
  // UB: this routine returns void; call it through its declared type.
#ifdef UBFIX
  Engine_ObjectSetScript(arg0, gKolimaForestEntrance_SparkleBurstScript);
#else
  /* FAKEMATCH(wrapper): called through a cast to the unprototyped call the
   * bytes show, which loads the script before r0; as a call by its prototype,
   * 1 of 79 instructions differ in tbs-en. */
  ((s32 (*)())Engine_ObjectSetScript)(
      (s32)arg0, (s32)gKolimaForestEntrance_SparkleBurstScript);
#endif
  gKolimaForestEntrance_PsynergyFieldHit = 1;
  return 0;
}

#include "common/kolima_forest_bridge_falling_effect.h"
s32 gKolimaForestEntrance_PsynergyFieldHit __attribute__((section(".bss")));
s32 gKolimaForestEntrance_MiaInParty __attribute__((section(".bss")));
s32 gKolimaForestEntrance_SparkleSoundOn __attribute__((section(".bss")));

/// Each frame the Psynergy fields flicker: two show on even frames and two on
/// odd, the hidden ones lifted 500 pixels out of sight.
static void KolimaForestEntrance_FlickerPsynergyFields(void) {
  struct FieldActor *p;

  p = Object_GetById(ACTOR_LEADER_PSYNERGY_FIELD);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = Object_GetById(ACTOR_IVAN_PSYNERGY_FIELD);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) != 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = Object_GetById(ACTOR_MIA_PSYNERGY_FIELD);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = Object_GetById(ACTOR_GARET_PSYNERGY_FIELD);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) != 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
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
// clang-format on

/// Garet, Ivan and Mia walk to their places beside the leader.
s32 gKolimaForestEntrance_GaretAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(234), 0, PIXELS(576)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_ARG(1), SC_END,
};

s32 gKolimaForestEntrance_IvanAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(234), 0, PIXELS(623)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_EAST),
    SC_ARG(1), SC_END,
};

s32 gKolimaForestEntrance_MiaAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(211), 0, PIXELS(602)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_NORTH),
    SC_ARG(1), SC_END,
};

/// The glamour presses the party down: each squashes and wobbles until
/// released.
s32 gKolimaForestEntrance_SquashAction[] = {
    SC_ADD(SW_SCALE_X, -0x4000), SC_ADD(SW_SCALE_Y, 0x2000),  SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),  SC_ADD(SW_SCALE_Y, -0x1000), SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),  SC_ADD(SW_SCALE_Y, -2048),   SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),  SC_ADD(SW_SCALE_Y, -2048),   SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolimaForestEntrance_RestoreScaleAction[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(1),
    SC_END,
};

/// A field grows from a tenth of its size over 70 frames.
s32 gKolimaForestEntrance_PsynergyFieldGrowAction[] = {
    SC_SET(SW_SCALE_X, 0x1999),
    SC_SET(SW_SCALE_Y, 0x1999),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 1310),
    SC_ADD(SW_SCALE_Y, 1310),
    SC_WAIT(1),
    SC_LOOP(70, SC_TO(1)),
    SC_CALL(KolimaForestEntrance_FlashHitPsynergyFields),
    SC_END,
};

/// A field sinks and shrinks away over 90 frames.
s32 gKolimaForestEntrance_PsynergyFieldShrinkAction[] = {
    SC_ARG(1),
    SC_ADD(SW_Z, -0x1999),
    SC_ADD(SW_SCALE_X, -983),
    SC_ADD(SW_SCALE_Y, -983),
    SC_WAIT(1),
    SC_LOOP(90, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestEntrance_GroundSparkleAction[] = {
    SC_SET(SW_SCALE_X, 0xc000),
    SC_SET(SW_SCALE_Y, 0xc000),
    SC_SET(SW_COLLISION, 0),
    SC_SET(0x23, 0),
    SC_LABEL(1),
    SC_CALL(KolimaForestEntrance_TwinkleGroundSparkle),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gKolimaForestEntrance_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 384, 0, 312, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {KOLIMA_FOREST_ENTRANCE_FROM_KOLIMA_FOREST_GLADE, CONDITION_ALWAYS, 24, 0,
     128, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 172, 0, 602,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

/// On into the forest (SceneId_KolimaForestGlade) and out to the world map
/// (SceneId_WorldMap), with two exits no event uses back into this map.
u32 gKolimaForestEntrance_Exits[] = {
    SCENE_EXITS(KolimaForestEntrance),
    SCENE_EXIT(KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE, KolimaForestGlade,
               KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_ENTRANCE),
    SCENE_EXIT(KOLIMA_FOREST_ENTRANCE_UNUSED_EXIT_A, KolimaForestEntrance,
               KOLIMA_FOREST_ENTRANCE_UNUSED),
    SCENE_EXIT(KOLIMA_FOREST_ENTRANCE_UNUSED_EXIT_B, KolimaForestEntrance,
               KOLIMA_FOREST_ENTRANCE_UNUSED),
    SCENE_EXIT(KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_KOLIMA_FOREST_ENTRANCE),
    SCENE_EXITS_END,
};

/// Garet, Ivan and Mia for the curse; the three logs; Tret and Laurel; the
/// four Psynergy fields; and the five ground sparkles.
// clang-format off
struct ScenePlacement gKolimaForestEntrance_Placements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(232), 0, PIXELS(128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(208), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(360), 0, PIXELS(304), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_TRET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_LAUREL, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(232), 0, PIXELS(655), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(166), 0, PIXELS(576), FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(189), 0, PIXELS(532), FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(288), 0, PIXELS(570), FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(331), 0, PIXELS(659), FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gKolimaForestEntrance_Events[] = {
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestEntrance_PushLog)},
    {TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForest_HopEast)},
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP},
    {SCENE_EVENTS_END},
};

s32 gKolimaForestEntrance_SparkleScript[] = {
    SC_CALL(FieldFx_StretchUntilArrived),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A sparkle bursts on a field: it doubles in size and is gone.
s32 gKolimaForestEntrance_SparkleBurstScript[] = {
    SC_SET(SW_SCALE_X, 0x20000),
    SC_SET(SW_SCALE_Y, 0x20000),
    SC_WAIT(10),
    SC_FREE,
};

s32 gFallingEffectScript[] = {
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_Y, PIXELS(-1)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(1)),
    SC_CALL(KolimaForestEntrance_BurstOnPsynergyField),
    SC_FREE,
};
