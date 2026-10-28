/// @file
/// Kolima Forest (SceneId_KolimaForestEntrance), the first of the forest's five
/// scenes, entered from the World Map (KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP).
/// If the party comes here before it has been to Kolima, the curse's event
/// plays here instead of in the village
/// (KolimaForestEntrance_WitnessForestCurse, until
/// FLAG_KOLIMA_CURSE_WITNESSED): sparkles of Tret's glamour rain down, fields
/// of the party's own Psynergy shield it, and Tret and Laurel, unseen, speak of
/// the dying forest. Five sparkles lie on the ground until Tret lifts the curse
/// (FLAG_KOLIMA_CURSE_LIFTED).
///
/// The fields flicker: the leader's and Mia's show on even frames and Ivan's
/// and Garet's on odd ones, each lifted out of sight on the others
/// (KolimaForestEntrance_FlickerPsynergyFields). A sparkle bursts where it
/// lands only inside the boxes KolimaForestEntrance_BurstOnPsynergyField tests,
/// two with Mia in the party and three without her.
///
/// Three logs lie here, rolled by walking into them; the one pushed west to the
/// water's edge tips into the river and floats there, cells the party can
/// cross, and from the bank the leader can hop east (step trigger 11,
/// KolimaForest_HopEast). KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE leads
/// on into the forest (SceneId_KolimaForestGlade),
/// KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP back to the World Map.
///
/// FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER: the log is in the river. Like
/// every scene temp flag it is cleared when the party next comes in, so the log
/// is back on the bank, but a battle keeps it.

#include "script_operands.h"
#include "flags.h"
#include "character_ids.h"
#include "main_routine_work.h"
#include "heap_slots.h"
#include "module/import.h"
#include "module/scene/kolima_forest_entrance.h"
#include "module/scene/shared/kolima_forest_bridge_falling_effect.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "map_grid_copy.h"
#include "types.h"
#include "tbs_edition.h"
#include "event.h"
#include "system_call.h"
#include "scene_tables.h"
#include "kolima.h"
#include "pushed_actor.h"
#include "text/msg_ids.h"
#include "actor_script.h"
#include "sounds.h"
#include "io_reg.h"
#include "sound_driver_symbols.h"
#include "fixed_math.h"

/// The scene's actors after Garet's, Ivan's and Mia's party slots,
/// numbering the rows of gKolimaForestEntrance_ActorPlacements from
/// ACTOR_FIRST_PLACED.
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
  /// The fields of Psynergy, one over each party member; Mia's only with Mia.
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
  /// ★ Named from the game: "I turned into a tree right when the sparkles fell
  /// from the sky." (MsgKolimaITurnedIntoATreeRight)
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

#include "shared/staged_actor.h"

/// The river log has tipped into the river and floats there as ground the party
/// can cross.
/// ★ Named from what the player sees: a log lying north to south beside the
/// river (its render)
#define FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER FLAG_SCENE_TEMP_00

/// The end of a field's growing: if a sparkle has burst on a field, the field
/// takes its next pose.
static s32
KolimaForestEntrance_FlashHitPsynergyFields(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  s32 *pending = &gKolimaForestEntrance_PsynergyFieldHit;
  if (*pending) {
    Actor_SelectAnimation(actor, 2);
    *pending = 0;
  }
  return 1;
}

/// Each frame: a ground sparkle's timer runs on at random, and the sparkle
/// shows its bright frame (7) while the timer is past 1000, its dim one (10)
/// before; at 1200 the timer starts over.
static s32
KolimaForestEntrance_TwinkleGroundSparkle(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  s16 *counter = &actor->timer;
  *counter = (*counter + (((u32)Import_RandomNext() * 100) >> 16));
  if (*counter > 1000)
    Actor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
  else
    Actor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
  if (*counter > 1200) {
    u16 z = 0;
    *counter = z;
  }
  return 1;
}

/// Where the party enters, from the next scene of the forest
/// (SceneId_KolimaForestGlade) and from the World Map.
const struct SceneEntrance *KolimaForestEntrance_GetEntrances(void) {
  return gKolimaForestEntrance_Entrances;
}

const struct SceneExitRegion *KolimaForestEntrance_GetRegions(void) {
  return 0;
}

/// The ways on into the forest (SceneId_KolimaForestGlade) and back to the
/// World Map.
const u32 *KolimaForestEntrance_GetExits(void) {
  return gKolimaForestEntrance_Exits;
}

/// The party's companions, the logs, Tret, Laurel, the Psynergy fields and the
/// ground sparkles.
const struct ActorPlacement *KolimaForestEntrance_GetActorPlacements(void) {
  return gKolimaForestEntrance_ActorPlacements;
}

/// Kolima Forest (SceneId_KolimaForestEntrance), walking into a log: it rolls
/// ahead until something stops it. The river log, stopped at the bank (cell
/// column 20), tips over with a splash and floats in the water; its cells
/// become ground the party can cross (sets
/// FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER).
static void KolimaForestEntrance_PushLog(void) {
  struct PushedActorProbe res;
  Event_Start();

  if (PushedActor_FindClearPosition(&res)) {
    Actor_MoveAndRedraw(res);
    if (res.actorSlot == ACTOR_RIVER_LOG && (res.position.x >> 20) == 20) {
      struct Actor *actor;

      Import_ActorSetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_ROLL_WEST);
      Actor_SetDestinationOffset(ACTOR_RIVER_LOG, -18, 6);
      Event_Wait(30);
      Sound_Play(SE_LOG_FALL);
      Import_ActorSetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_DROP_IN);
      Actor_GetById(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(0, 17, 2, 4, 19, 17);
      PushedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4,
                                             CELL_BEHAVIOR_NORMAL);
      GameFlag_Set(FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER);
      actor = Actor_GetById(ACTOR_RIVER_LOG);
      Actor_SetSpriteFlags(actor, 0);
    }
  }

  Event_End();
}

#include "shared/kolima_jump.h"

/// The logs, the hop east from the bank and the two ways out.
const struct Trigger *KolimaForestEntrance_GetTriggers(void) {
  return gKolimaForestEntrance_Triggers;
}

/// On entry a log already in the river floats there again and the logs mark
/// the cells they cover. From the World Map, before the curse has been seen,
/// the party sees it fall; once the curse is lifted the ground sparkles are
/// gone.
s32 KolimaForestEntrance_Initialize(void) {
  Scene_RedrawActorFootprint(ACTOR_RIVER_LOG);
  if (GameFlag_Get(FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER) != 0) {
    Actor_GetById(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(0, 17, 2, 4, 19, 17);
    PushedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4,
                                           CELL_BEHAVIOR_NORMAL);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_RIVER_LOG), 0);
  }
  Scene_RedrawActorFootprint(ACTOR_FIRST_LOG);
  Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 1);
  if (gMainRoutineWork.entrance == KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP) {
    if (GameFlag_Get(FLAG_KOLIMA_CURSE_WITNESSED) == 0) {
      KolimaForestEntrance_WitnessForestCurse();
    }
  }
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Import_ActorSetPosition(ACTOR_FIRST_GROUND_SPARKLE, 0, 0);
    Import_ActorSetPosition(ACTOR_FIRST_GROUND_SPARKLE + 1, 0, 0);
    Import_ActorSetPosition(ACTOR_FIRST_GROUND_SPARKLE + 2, 0, 0);
    Import_ActorSetPosition(ACTOR_FIRST_GROUND_SPARKLE + 3, 0, 0);
    Import_ActorSetPosition(ACTOR_LAST_GROUND_SPARKLE, 0, 0);
  }
  return 0;
}

/// Kolima Forest, the first entry from the World Map before the curse has been
/// seen in Kolima (FLAG_KOLIMA_CURSE_WITNESSED clear): the party sees the
/// curse fall, the same event as Kolima_WitnessForestCurse without the trees
/// that were people. Ivan notices sparkly stuff on the ground; sparkles rain
/// from the sky, the party sinks under them, and fields of Psynergy rise around
/// each party member before the sparkles reach them. They argue over whether it
/// was their Psynergy, the sparkles fall again and the Psynergy fields rise
/// again, and Tret and Laurel speak: Tret turned Kolima's people into trees for
/// cutting him, and Laurel tells them to reawaken his gentle side and look for
/// Tret deep within the forest. Lines and choices for Mia are skipped when she
/// is not in the party. Sets FLAG_KOLIMA_CURSE_WITNESSED.
static void KolimaForestEntrance_WitnessForestCurse(void) {
  struct Actor *record;
  s32 *party;
  const s32 *sparkleAction;
  const s32 *squashAction;
  s32 *miaInParty;
  const s32 *restoreScaleAction;
  const s32 *growFieldAction;
  const s32 *shrinkFieldAction;

  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  FieldCamera_MoveTo(PIXELS(246), -1, PIXELS(604), 0);
  party = &gKolimaForestEntrance_MiaInParty;
  *party = GameFlag_Get(FLAG_IN_PARTY(MIA));
  record = Actor_GetById(ACTOR_LEADER_PSYNERGY_FIELD);
  Actor_SetSpriteFlags(record, 0);
  record = Actor_GetById(ACTOR_IVAN_PSYNERGY_FIELD);
  Actor_SetSpriteFlags(record, 0);
  record = Actor_GetById(ACTOR_MIA_PSYNERGY_FIELD);
  Actor_SetSpriteFlags(record, 0);
  record = Actor_GetById(ACTOR_GARET_PSYNERGY_FIELD);
  Actor_SetSpriteFlags(record, 0);
  record = Actor_GetById(ACTOR_FIRST_GROUND_SPARKLE);
  Actor_SetSpriteFlags(record, 0);
  record = Actor_GetById(ACTOR_FIRST_GROUND_SPARKLE + 1);
  Actor_SetSpriteFlags(record, 0);
  record = Actor_GetById(ACTOR_FIRST_GROUND_SPARKLE + 2);
  Actor_SetSpriteFlags(record, 0);
  record = Actor_GetById(ACTOR_FIRST_GROUND_SPARKLE + 3);
  Actor_SetSpriteFlags(record, 0);
  record = Actor_GetById(ACTOR_LAST_GROUND_SPARKLE);
  Actor_SetSpriteFlags(record, 0);
  sparkleAction = gKolimaForestEntrance_GroundSparkleAction;
  Actor_SetScript(ACTOR_FIRST_GROUND_SPARKLE, sparkleAction);
  Actor_SetScript(ACTOR_FIRST_GROUND_SPARKLE + 1, sparkleAction);
  Actor_SetScript(ACTOR_FIRST_GROUND_SPARKLE + 2, sparkleAction);
  Actor_SetScript(ACTOR_FIRST_GROUND_SPARKLE + 3, sparkleAction);
  Actor_SetScript(ACTOR_LAST_GROUND_SPARKLE, sparkleAction);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(116), PIXELS(602));
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 254, 593);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  {
    struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
    }
  }
  {
    struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
    }
  }
  Actor_SetScript(ACTOR_GARET, gKolimaForestEntrance_GaretAction);
  Actor_SetScript(ACTOR_IVAN, gKolimaForestEntrance_IvanAction);
  if (*party != 0) {
    Actor_SetSpeed(ACTOR_MIA, PIXELS(3) / 5, PIXELS(3) / 10);
    {
      struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

      if (leader != 0) {
        Actor_SetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
      }
    }
    Actor_SetScript(ACTOR_MIA, gKolimaForestEntrance_MiaAction);
  }
  Actor_RefreshSelectorById(2);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_SOUTHEAST, 40);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_WEST, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_SOUTH, 40);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        60);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_SetMessageId(MsgKolimaMatterIvan);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 10);
  Event_SetMessageId(MsgKolimaSparklyStuffOnGround);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_EAST, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        40);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        30);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHEAST,
                                        30);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_PlaySound(SE_CURSE_FALL);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  wait(1);
  gKolimaForestEntrance_SparkleSoundOn = 1;
  Task_AddFrame(KolimaForestEntrance_SpawnCurseSparkle, TASK_PASS_FRAME);
  wait(20);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(16, 16, 20), PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(120);
  wait(60);
  squashAction = gKolimaForestEntrance_SquashAction;
  Actor_SetScript(ACTOR_PARTY_LEADER, squashAction);
  Actor_SetScript(ACTOR_GARET, squashAction);
  Actor_SetScript(ACTOR_IVAN, squashAction);
  Actor_SetScript(ACTOR_MIA, squashAction);
  Event_Wait(100);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 40);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(40);
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 40);
  } else {
    gFieldWork->message += 1;
  }
  Event_Wait(20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    record = Actor_GetById(ACTOR_MIA);
    record->velocityY = PIXELS(2);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_MIA, PIXELS(2), PIXELS(2));
    Actor_SetDestinationOffset(ACTOR_MIA, -2, 0);
    Actor_SetScript(ACTOR_MIA, gKolimaForestEntrance_RestoreScaleAction);
    record = Actor_GetById(ACTOR_MIA);
    Actor_SetSpriteFlags(record, 0);
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_COLLAPSE);
    Event_Wait(10);
  }
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  record->velocityY = PIXELS(2);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(2));
  restoreScaleAction = gKolimaForestEntrance_RestoreScaleAction;
  Actor_SetScript(ACTOR_PARTY_LEADER, restoreScaleAction);
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(record, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
  Event_Wait(20);
  record = Actor_GetById(ACTOR_GARET);
  record->velocityY = PIXELS(2);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(2), PIXELS(2));
  Actor_SetScript(ACTOR_GARET, restoreScaleAction);
  record = Actor_GetById(ACTOR_GARET);
  Actor_SetSpriteFlags(record, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_COLLAPSE);
  Event_Wait(40);
  record = Actor_GetById(ACTOR_IVAN);
  record->velocityY = PIXELS(2);
  Event_Wait(10);
  Actor_SetScript(ACTOR_IVAN, restoreScaleAction);
  record = Actor_GetById(ACTOR_IVAN);
  Actor_SetSpriteFlags(record, 0);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_COLLAPSE);
  gKolimaForestEntrance_SparkleSoundOn = 0;
  Event_Wait(160);
  Task_Remove(KolimaForestEntrance_SpawnCurseSparkle);
  Event_Wait(120);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  wait(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = PIXELS(128);
  gFallingEffectState = KOLIMA_DUST_CLOSING;
  Task_AddFrame(Kolima_UpdateCurseDust, TASK_PASS_FRAME);
  Event_Wait(180);
  Event_PlaySound(MUS_FOREST);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 80);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
  gFallingEffectState = KOLIMA_DUST_CLOSING_WIDE;
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 3);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 10);
  } else {
    gFieldWork->message += 1;
  }
  gFallingEffectState = KOLIMA_DUST_DRIFTING;
  Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_IVAN)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Actor_SetSpritePriority(ACTOR_GARET, 3);
  Actor_SetSpritePriority(ACTOR_IVAN, 3);
  Actor_SetSpritePriority(ACTOR_MIA, 3);
  gKolimaForestEntrance_PsynergyFieldHit = 0;
  Task_AddFrame(KolimaForestEntrance_FlickerPsynergyFields, TASK_PASS_FRAME);
  Event_PlaySound(SE_SHIMMER);
  Actor_GetById(ACTOR_LEADER_PSYNERGY_FIELD)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_LEADER_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_LEADER_PSYNERGY_FIELD, PIXELS(253), PIXELS(603));
  growFieldAction = gKolimaForestEntrance_PsynergyFieldGrowAction;
  Actor_SetScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldAction);
  Actor_GetById(ACTOR_IVAN_PSYNERGY_FIELD)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_IVAN_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_IVAN_PSYNERGY_FIELD, PIXELS(233), PIXELS(629));
  Actor_SetScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldAction);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_GetById(ACTOR_MIA_PSYNERGY_FIELD)->priorityFlags &=
        ~ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpritePriority(ACTOR_MIA_PSYNERGY_FIELD, 2);
    Actor_SetPosition(ACTOR_MIA_PSYNERGY_FIELD, PIXELS(207), PIXELS(609));
    Actor_SetScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldAction);
  }
  Actor_GetById(ACTOR_GARET_PSYNERGY_FIELD)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_GARET_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_GARET_PSYNERGY_FIELD, PIXELS(227), PIXELS(580));
  Actor_SetScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldAction);
  if (gFallingEffectState != KOLIMA_DUST_CENTRED) {
    do {
      wait(1);
    } while (gFallingEffectState != KOLIMA_DUST_CENTRED);
  }
  Event_Wait(300);
  Task_Remove(Kolima_UpdateCurseDust);
  Event_Wait(120);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  wait(60);
  Actor_Stop(ACTOR_LEADER_PSYNERGY_FIELD);
  Actor_Stop(ACTOR_IVAN_PSYNERGY_FIELD);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_Stop(ACTOR_MIA_PSYNERGY_FIELD);
  }
  Actor_Stop(ACTOR_GARET_PSYNERGY_FIELD);
  wait(1);
  shrinkFieldAction = gKolimaForestEntrance_PsynergyFieldShrinkAction;
  Actor_SetScript(ACTOR_LEADER_PSYNERGY_FIELD, shrinkFieldAction);
  Actor_SetScript(ACTOR_IVAN_PSYNERGY_FIELD, shrinkFieldAction);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_SetScript(ACTOR_MIA_PSYNERGY_FIELD, shrinkFieldAction);
  }
  Actor_RunScriptAndWait(ACTOR_GARET_PSYNERGY_FIELD, shrinkFieldAction);
  Event_Wait(80);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(40);
  Event_OpenMessage(ACTOR_GARET, 0);
  Actor_SetPosition(ACTOR_TRET, PIXELS(220), PIXELS(494));
  Actor_SetPosition(ACTOR_LAUREL, PIXELS(220), PIXELS(494));
  wait(1);
  if (Event_AskYesNoWithActorAnimation(11, 0) == 1) {
    gFieldWork->message += 1;
  }
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(10);
    Event_SetMessageId(MsgKolimaKnowThoseFieldsWere);
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 40);
  }
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(80);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_SetMessageId(MsgKolimaWasOurPsynergy);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(40);
  Actor_SetSpritePriority(ACTOR_GARET, 2);
  Actor_GetById(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  record = Actor_GetById(ACTOR_GARET);
  Actor_SetSpriteFlags(record, 1);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_SetDestinationOffset(ACTOR_GARET, -3, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 60);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(40);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 40);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
  Actor_Jump(ACTOR_GARET, 2, 0);
  Event_Wait(40);
  Actor_Jump(ACTOR_GARET, 2, 0);
  Event_Wait(10);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Event_Wait(20);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(80);
    Actor_SetSpritePriority(ACTOR_MIA, 2);
    Actor_GetById(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    record = Actor_GetById(ACTOR_MIA);
    Actor_SetSpriteFlags(record, 1);
    Actor_Jump(ACTOR_MIA, 4, 0);
    Actor_SetDestinationOffset(ACTOR_MIA, -2, 0);
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
    KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_NORTHEAST, 60);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 20);
  } else {
    gFieldWork->message += 1;
  }
  Actor_Jump(ACTOR_GARET, 2, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_SetSpritePriority(ACTOR_IVAN, 2);
  Actor_GetById(ACTOR_IVAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  record = Actor_GetById(ACTOR_IVAN);
  Actor_SetSpriteFlags(record, 1);
  Actor_Jump(ACTOR_IVAN, 4, 0);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(10);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  record = Actor_GetById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(record, 1);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        10);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(20);
  Event_OpenMessage(ACTOR_IVAN, 0);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    gFieldWork->message += 1;
  } else {
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
    Actor_StartRepeatedMotion(ACTOR_GARET, 2);
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
    KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 10);
  } else {
    gFieldWork->message += 1;
  }
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(80);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        10);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(40);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
  Event_OpenMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  } else {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(40);
    gFieldWork->message += 1;
  }
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_PlaySound(MUS_FOREST);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  wait(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = PIXELS(128);
  gFallingEffectState = KOLIMA_DUST_CLOSING;
  Task_AddFrame(Kolima_UpdateCurseDust, TASK_PASS_FRAME);
  Event_Wait(80);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(60);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
  Event_SetMessageId(MsgKolimaWatchOutItsHappeningAgain);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_NORTH, 10);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
  }
  Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_IVAN)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Actor_SetSpritePriority(ACTOR_GARET, 3);
  Actor_SetSpritePriority(ACTOR_IVAN, 3);
  Actor_SetSpritePriority(ACTOR_MIA, 3);
  gFallingEffectState = KOLIMA_DUST_CLOSING_WIDE;
  Event_PlaySound(SE_SHIMMER);
  Actor_SetPosition(ACTOR_LEADER_PSYNERGY_FIELD, PIXELS(253), PIXELS(603));
  growFieldAction = gKolimaForestEntrance_PsynergyFieldGrowAction;
  Actor_SetScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldAction);
  Actor_SetPosition(ACTOR_IVAN_PSYNERGY_FIELD, PIXELS(233), PIXELS(629));
  Actor_SetScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldAction);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_SetPosition(ACTOR_MIA_PSYNERGY_FIELD, PIXELS(207), PIXELS(609));
    Actor_SetScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldAction);
  }
  Actor_SetPosition(ACTOR_GARET_PSYNERGY_FIELD, PIXELS(227), PIXELS(580));
  Actor_SetScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldAction);
  Event_Wait(120);
  gFallingEffectState = KOLIMA_DUST_DRIFTING;
  do {
    wait(1);
  } while (gFallingEffectState != KOLIMA_DUST_CENTRED);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 80);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(40);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 40);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 80);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 40);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(40);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(60);
  Event_ShowMessage(ACTOR_LAUREL, 0);
  Event_ShowMessage(ACTOR_TRET, 0);
  Task_Remove(Kolima_UpdateCurseDust);
  Event_Wait(80);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  wait(80);
  Actor_Stop(ACTOR_LEADER_PSYNERGY_FIELD);
  Actor_Stop(ACTOR_IVAN_PSYNERGY_FIELD);
  miaInParty = &gKolimaForestEntrance_MiaInParty;
  Actor_Stop(ACTOR_MIA_PSYNERGY_FIELD);
  Actor_Stop(ACTOR_GARET_PSYNERGY_FIELD);
  wait(1);
  shrinkFieldAction = gKolimaForestEntrance_PsynergyFieldShrinkAction;
  Actor_SetScript(ACTOR_LEADER_PSYNERGY_FIELD, shrinkFieldAction);
  Actor_SetScript(ACTOR_IVAN_PSYNERGY_FIELD, shrinkFieldAction);
  if (*miaInParty != 0) {
    Actor_SetScript(ACTOR_MIA_PSYNERGY_FIELD, shrinkFieldAction);
  }
  Actor_RunScriptAndWait(ACTOR_GARET_PSYNERGY_FIELD, shrinkFieldAction);
  Event_Wait(20);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpritePriority(ACTOR_GARET, 2);
  Actor_SetSpritePriority(ACTOR_IVAN, 2);
  Actor_SetSpritePriority(ACTOR_MIA, 2);
  Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_IVAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(10);
    Event_OpenMessage(ACTOR_GARET, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Event_Wait(40);
      KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 10);
      KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 20);
      KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 20);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
      KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 20);
    } else {
      KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Event_Wait(40);
      KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
      Event_SetMessageId(MsgKolimaHesAsStumpedAsWe);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
    }
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  } else {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(10);
    Event_SetMessageId(MsgKolimaYoureIsaacThereIsntMuch);
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 10);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                          20);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_OpenMessage(ACTOR_IVAN, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_Wait(20);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
      Event_Wait(40);
      KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
      if (*miaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 10);
        Actor_StartRepeatedMotion(ACTOR_MIA, 3);
        KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 20);
      } else {
        gFieldWork->message += 1;
      }
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Event_Wait(40);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Event_Wait(120);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 40);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
        Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
        KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 10);
      } else {
        gFieldWork->message += 1;
      }
      Event_Wait(60);
      Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHWEST, 40);
        KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 20);
      }
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
      Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      Event_Wait(40);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
      Event_Wait(20);
      Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    } else {
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Event_Wait(40);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
      Event_SetMessageId(MsgKolimaThatsReliefIsaacThoughtYoud);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
      if (*miaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 10);
        Actor_StartRepeatedMotion(ACTOR_MIA, 1);
        KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 20);
      } else {
        gFieldWork->message += 1;
      }
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Event_Wait(40);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Event_Wait(80);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 40);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 20);
        Import_ActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
        KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 40);
      } else {
        gFieldWork->message += 1;
      }
      Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
      Event_Wait(20);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
      Actor_StartRepeatedMotion(ACTOR_GARET, 2);
      Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Event_Wait(40);
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
    }
  }
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  {
    struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  {
    struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
    {
      struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

      if (leader != 0) {
        Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel,
                             leader->z.part.pixel);
      }
    }
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
  }
  GameFlag_Set(FLAG_KOLIMA_CURSE_WITNESSED);
  Actor_SetPosition(ACTOR_TRET, 0, 0);
  Actor_SetPosition(ACTOR_LAUREL, 0, 0);
  Scene_PlayMusic();
  Event_End();
}

static void KolimaForestEntrance_ShowMessageAndWait(s32 speaker, s32 frames) {
  Event_ShowMessageAndWait(speaker, 0);
  Event_Wait(frames);
}

/// Turns an actor at once, then leaves time for the next beat.
static void KolimaForestEntrance_FaceActorAndWait(s32 actorId, s32 facing,
                                                  s32 waitFrames) {
  Actor_ArmCallback(actorId, facing, 0);
  Event_Wait(waitFrames);
}

#include "shared/kolima_grow.h"

/// Every eighth frame a sparkle of Tret's glamour (SPRITE_GOLDEN_RINGS) falls
/// toward the party, with its tinkle while the rain is at its height.
static void KolimaForestEntrance_SpawnCurseSparkle(void) {
  struct Actor *sparkle;
  struct Sprite *sprite;
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
    Sound_Play(SE_SPARKLE_FALL);
  sparkle = FieldActor_Create(SPRITE_GOLDEN_RINGS, spawnX, 0, spawnZ);
  if (sparkle == 0)
    return;
  sprite = sparkle->sprite;
  sprite->flags = framePhase;
  priorityFlags = 0xff & ~ACTOR_PRIORITY_AUTOMATIC;
  priorityFlags &= sparkle->priorityFlags;
  sparkle->priorityFlags = priorityFlags;
  sprite->image.priority = 1;
  sparkle->scaleX = Q16_ONE / 10;
  sparkle->speedLimit = PIXELS(8);
  sparkle->acceleration = PIXELS(8);
  sparkle->motionFlags = framePhase;
  Actor_SelectAnimation(sparkle, 2);
  FieldActor_SetMoveTarget(sparkle, targetX, 0, targetZ);
  Actor_StartScript(sparkle, gKolimaForestEntrance_SparkleScript);
}

#include "shared/kolima_blink.h"

/// The end of a sparkle's fall: one that lands on a field (the Psynergy fields'
/// area, which differs with Mia) bursts with a sound, and the Psynergy fields
/// are told.
static s32
KolimaForestEntrance_BurstOnPsynergyField(struct Actor *interpreter) {
  struct Actor *sparkle = interpreter;
  s32 x;

  if (!(gKolimaForestEntrance_MiaInParty != 0
            ? (x = sparkle->x.fixed, (x > PIXELS(192) && x < PIXELS(274) &&
                                      sparkle->z.fixed > PIXELS(566) &&
                                      sparkle->z.fixed < PIXELS(612)) ||
                                         (x > PIXELS(202) && x < PIXELS(255) &&
                                          sparkle->z.fixed > PIXELS(549) &&
                                          sparkle->z.fixed < PIXELS(632)))
            : (x = sparkle->x.fixed,
               (x > PIXELS(192) && x < PIXELS(244) &&
                sparkle->z.fixed > PIXELS(549) &&
                sparkle->z.fixed <= PIXELS(585) - 1) ||
                   (x > PIXELS(244) && x < PIXELS(274) &&
                    sparkle->z.fixed > PIXELS(571) &&
                    sparkle->z.fixed <= PIXELS(605) - 1) ||
                   (x > PIXELS(211) && x < PIXELS(255) &&
                    sparkle->z.fixed > PIXELS(596) &&
                    sparkle->z.fixed < PIXELS(632))))) {
    return 0;
  }
  Sound_Play(SE_DRIP);
  // UB: this routine returns void; call it through its declared type.
#ifdef UBFIX
  Actor_StartScript(sparkle, gKolimaForestEntrance_SparkleBurstScript);
#else
  /* FAKEMATCH(wrapper): called through a cast to the unprototyped call the
   * bytes show, which loads the script before r0; as a call by its prototype,
   * 1 of 79 instructions differ in tbs-en. */
  ((s32 (*)())Actor_StartScript)((s32)sparkle,
                                 (s32)gKolimaForestEntrance_SparkleBurstScript);
#endif
  gKolimaForestEntrance_PsynergyFieldHit = 1;
  return 0;
}

#include "shared/kolima_forest_bridge_falling_effect.h"
s32 gKolimaForestEntrance_PsynergyFieldHit __attribute__((section(".bss")));
s32 gKolimaForestEntrance_MiaInParty __attribute__((section(".bss")));
s32 gKolimaForestEntrance_SparkleSoundOn __attribute__((section(".bss")));

/// Each frame the Psynergy fields flicker: two show on even frames and two on
/// odd, the hidden ones lifted 500 pixels out of sight.
static void KolimaForestEntrance_FlickerPsynergyFields(void) {
  struct Actor *p;

  p = Actor_GetById(ACTOR_LEADER_PSYNERGY_FIELD);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = Actor_GetById(ACTOR_IVAN_PSYNERGY_FIELD);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) != 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = Actor_GetById(ACTOR_MIA_PSYNERGY_FIELD);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = Actor_GetById(ACTOR_GARET_PSYNERGY_FIELD);
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

s32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] = {
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
    SC_ANIMATION(2), SC_MOVE(PIXELS(234), 0, PIXELS(576)),
    SC_SYNC,         SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_ANIMATION(1), SC_END,
};

s32 gKolimaForestEntrance_IvanAction[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(234), 0, PIXELS(623)),
    SC_SYNC,         SC_SET(SW_FACING, FACING_EAST),
    SC_ANIMATION(1), SC_END,
};

s32 gKolimaForestEntrance_MiaAction[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(211), 0, PIXELS(602)),
    SC_SYNC,         SC_SET(SW_FACING, FACING_NORTH),
    SC_ANIMATION(1), SC_END,
};

/// The glamour presses the party down: each squashes and wobbles until
/// released.
s32 gKolimaForestEntrance_SquashAction[] = {
    SC_ADD(SW_SCALE_X, -Q16_ONE / 4),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolimaForestEntrance_RestoreScaleAction[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(1),
    SC_END,
};

/// A field grows from a tenth of its size over 70 frames.
s32 gKolimaForestEntrance_PsynergyFieldGrowAction[] = {
    SC_SET(SW_SCALE_X, Q16_ONE / 10),
    SC_SET(SW_SCALE_Y, Q16_ONE / 10),
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
    SC_ANIMATION(1),
    SC_ADD(SW_Z, -PIXELS(1) / 10),
    SC_ADD(SW_SCALE_X, -983),
    SC_ADD(SW_SCALE_Y, -983),
    SC_WAIT(1),
    SC_LOOP(90, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestEntrance_GroundSparkleAction[] = {
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 4),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 4),
    SC_SET(SW_COLLISION, 0),
    SC_SET(SW_TIMER, 0),
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

/// On into the forest (SceneId_KolimaForestGlade) and out to the World Map
/// (SceneId_WorldMap), with two exits no trigger uses back into this scene.
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
struct ActorPlacement gKolimaForestEntrance_ActorPlacements[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(232), 0, PIXELS(128), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(208), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(360), 0, PIXELS(304), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TRET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_LAUREL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(232), 0, PIXELS(655), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(166), 0, PIXELS(576), FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(189), 0, PIXELS(532), FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(288), 0, PIXELS(570), FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(331), 0, PIXELS(659), FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gKolimaForestEntrance_Triggers[] = {
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS,
     EVENT(KolimaForestEntrance_PushLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS,
     EVENT(KolimaForest_HopEast)},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP},
    {TRIGGERS_END},
};

s32 gKolimaForestEntrance_SparkleScript[] = {
    SC_CALL(FieldEffect_StretchUntilArrived),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A sparkle bursts on a field: it doubles in size and is gone.
s32 gKolimaForestEntrance_SparkleBurstScript[] = {
    SC_SET(SW_SCALE_X, 2 * Q16_ONE),
    SC_SET(SW_SCALE_Y, 2 * Q16_ONE),
    SC_WAIT(10),
    SC_RELEASE,
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
    SC_RELEASE,
};
