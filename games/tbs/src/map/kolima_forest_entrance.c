/// @file
/// Kolima Forest (MapId_KolimaForestEntrance), the first of the forest's five
/// maps, entered from the world map (KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP).
/// If the party comes here before it has been to Kolima, the curse's cutscene
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
/// lands only inside the boxes KolimaForestEntrance_BurstOnPsynergyField
/// tests, two with Mia in the party and three without her
/// (https://alchemy.pascalpixel.com/places/kolima-forest-entrance/psynergy-fields.png:
/// two frames running without Mia and with her, each field's place labelled
/// with whose it is, in colour on the frame it shows, and the boxes outlined
/// in yellow).
///
/// Three logs lie here, rolled by walking into them; the one pushed west to the
/// water's edge tips into the river and floats there, cells the party can
/// cross, and from the bank the leader can hop east (coord event 11,
/// KolimaForest_HopEast). KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE leads
/// on into the forest (MapId_KolimaForestGlade),
/// KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP back to the world map.
///
/// FLAG_MAP_TEMP_00: the log is in the river. Like every map temp flag it is
/// cleared when the party next comes in, so the log is back on the bank, but a
/// battle keeps it.
///
/// Open questions: none.

#include "action_script.h"
#include "flags.h"
#include "character_ids.h"
#include "game_state.h"
#include "heap_slots.h"
#include "unit/import.h"
#include "unit/map/kolima_forest_entrance.h"
#include "unit/map/common/kolima_forest_bridge_falling_effect.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "script_layer.h"
#include "engine_call.h"
#include "map_tables.h"
#include "kolima.h"
#include "pushed_object.h"
#include "text/msg_ids.h"
#include "objcmd.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "fixed_math.h"

/// The map's object events after Garet's, Ivan's and Mia's party slots,
/// numbering the rows of gKolimaForestEntrance_ObjectEventTemplates from
/// LOCALID_FIRST_PLACED.
enum {
  /// The three logs; the last is the one that can reach the river.
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  LOCALID_FIRST_LOG = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  LOCALID_RIVER_LOG = LOCALID_FIRST_LOG + 2,
  /// Tret's wrathful side, unseen while the curse falls.
  /// ★ Named from the game: "Tret was once a kindly forest king."
  /// (MsgKolimaWatchOutItsHappeningAgain)
  LOCALID_TRET,
  /// Unseen while the curse falls.
  /// ★ Named from the game: "We are not the only ones who will die, Laurel."
  /// (MsgKolimaWatchOutItsHappeningAgain)
  LOCALID_LAUREL,
  /// The fields of Psynergy, one over each member; Mia's only with Mia.
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  LOCALID_LEADER_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  LOCALID_IVAN_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  LOCALID_MIA_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  LOCALID_GARET_PSYNERGY_FIELD,
  /// The sparkles of Tret's glamour on the ground, gone once the curse is
  /// lifted.
  /// ★ Named from the game: "I turned into a tree right when the sparkles
  /// fell from the sky." (MsgKolimaITurnedIntoATreeRight)
  LOCALID_FIRST_GROUND_SPARKLE,
  /// ★ Named from the game: "I turned into a tree right when the sparkles fell
  /// from the sky." (MsgKolimaITurnedIntoATreeRight)
  LOCALID_LAST_GROUND_SPARKLE = LOCALID_FIRST_GROUND_SPARKLE + 4
};

static void KolimaForestEntrance_WitnessForestCurse(void);
static void KolimaForestEntrance_ShowMessageAndWait(s32 speaker, s32 frames);
static void KolimaForestEntrance_FaceObjectEventAndWait(s32 localId, s32 facing,
                                                        s32 waitFrames);
static void KolimaForestEntrance_SpawnCurseSparkle(void);
static void KolimaForestEntrance_FlickerPsynergyFields(void);

extern s32 gKolimaForestEntrance_PsynergyFieldHit;
extern s32 gKolimaForestEntrance_MiaInParty;
extern s32 gKolimaForestEntrance_SparkleSoundOn;

#include "common/object_staged_object_event.h"

/// The end of a field's growing: if a sparkle has burst on a field, the field
/// takes its next pose.
static s32 KolimaForestEntrance_FlashHitPsynergyFields(
    struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s32 *pending = &gKolimaForestEntrance_PsynergyFieldHit;
  if (*pending) {
    ObjectDispatch_SelectAnimation(objectEvent, 2);
    *pending = 0;
  }
  return 1;
}

/// Each frame: a ground sparkle's timer runs on at random, and the sparkle
/// shows its bright frame (7) while the timer is past 1000, its dim one (10)
/// before; at 1200 the timer starts over.
static s32 KolimaForestEntrance_TwinkleGroundSparkle(
    struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s16 *counter = &objectEvent->timer;
  *counter = (*counter + (((u32)Engine_RandomNext() * 100) >> 16));
  if (*counter > 1000)
    ObjectGroup_SetChildValue(objectEvent, 7);
  else
    ObjectGroup_SetChildValue(objectEvent, 10);
  if (*counter > 1200) {
    u16 z = 0;
    *counter = z;
  }
  return 1;
}

/// Where the party enters, from the next map of the forest
/// (MapId_KolimaForestGlade) and from the world map.
const struct MapWarpDest *KolimaForestEntrance_GetWarpDests(void) {
  return gKolimaForestEntrance_WarpDests;
}

const struct MapWarpRegion *KolimaForestEntrance_GetRegions(void) { return 0; }

/// The ways on into the forest (MapId_KolimaForestGlade) and back to the
/// world map.
const u32 *KolimaForestEntrance_GetWarps(void) {
  return gKolimaForestEntrance_Warps;
}

/// The party's companions, the logs, Tret, Laurel, the Psynergy fields and the
/// ground sparkles.
const struct ObjectEventTemplate *
KolimaForestEntrance_GetObjectEventTemplates(void) {
  return gKolimaForestEntrance_ObjectEventTemplates;
}

/// Kolima Forest (MapId_KolimaForestEntrance), walking into a log: it rolls
/// ahead until something stops it. The river log, stopped at the bank (cell
/// column 20), tips over with a splash and floats in the water; its cells
/// become ground the party can cross (sets FLAG_MAP_TEMP_00).
static void KolimaForestEntrance_PushLog(void) {
  struct PushedObjectProbe res;
  FieldScript_Start();

  if (PushedObject_FindClearPosition(&res)) {
    ObjectEvent_MoveAndRedraw(res);
    if (res.objectEventSlot == LOCALID_RIVER_LOG &&
        (res.position.x >> 20) == 20) {
      struct ObjectEvent *objectEvent;

      Engine_ObjectEventSetAnimation(LOCALID_RIVER_LOG,
                                     LOG_ANIMATION_ROLL_WEST);
      ObjectEvent_SetDestinationOffset(LOCALID_RIVER_LOG, -18, 6);
      FieldScript_WaitFrames(30);
      Sound_PlaySong(SE_240);
      Engine_ObjectEventSetAnimation(LOCALID_RIVER_LOG, LOG_ANIMATION_DROP_IN);
      ObjectEvent_GetById(LOCALID_RIVER_LOG)->priorityFlags =
          OBJ_EVENT_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(0, 17, 2, 4, 19, 17);
      PushedObject_FillGridAttributeRectangle(2, 20, 17, 1, 4, 0);
      Engine_GameFlagSet(FLAG_MAP_TEMP_00);
      objectEvent = ObjectEvent_GetById(LOCALID_RIVER_LOG);
      ObjectEvent_SetSpriteFlags(objectEvent, 0);
    }
  }

  FieldScript_End();
}

#include "common/kolima_jump.h"

/// The logs, the hop east from the bank and the two ways out.
const struct MapEvent *KolimaForestEntrance_GetEvents(void) {
  return gKolimaForestEntrance_Events;
}

/// On entry a log already in the river floats there again and the logs mark
/// the cells they cover. From the world map, before the curse has been seen,
/// the party sees it fall; once the curse is lifted the ground sparkles are
/// gone.
s32 KolimaForestEntrance_Initialize(void) {
  Map_RedrawObjectEventFootprint(LOCALID_RIVER_LOG);
  if (GameFlag_Test(FLAG_MAP_TEMP_00) != 0) {
    ObjectEvent_GetById(LOCALID_RIVER_LOG)->priorityFlags =
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(0, 17, 2, 4, 19, 17);
    PushedObject_FillGridAttributeRectangle(2, 20, 17, 1, 4, 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_RIVER_LOG), 0);
  }
  Map_RedrawObjectEventFootprint(LOCALID_FIRST_LOG);
  Map_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 1);
  if (gGameState.warpDest == KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP) {
    if (GameFlag_Test(FLAG_KOLIMA_CURSE_WITNESSED) == 0) {
      KolimaForestEntrance_WitnessForestCurse();
    }
  }
  if (GameFlag_Test(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_ObjectEventSetPosition(LOCALID_FIRST_GROUND_SPARKLE, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_FIRST_GROUND_SPARKLE + 1, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_FIRST_GROUND_SPARKLE + 2, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_FIRST_GROUND_SPARKLE + 3, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_LAST_GROUND_SPARKLE, 0, 0);
  }
  return 0;
}

/// Kolima Forest, the first entry from the world map before the curse has been
/// seen in Kolima (FLAG_KOLIMA_CURSE_WITNESSED clear): the party sees the
/// curse fall, the same cutscene as Kolima_WitnessForestCurse without the trees
/// that were people. Ivan notices sparkly stuff on the ground; sparkles rain
/// from the sky, the party sinks under them, and fields of Psynergy rise around
/// each member before the sparkles reach them. They argue over whether it was
/// their Psynergy, the sparkles fall again and the Psynergy fields rise again,
/// and Tret and Laurel speak: Tret turned Kolima's people into trees for
/// cutting him, and Laurel tells them to reawaken his gentle side and look for
/// Tret deep within the forest. Lines and choices for Mia are skipped when she
/// is not in the party. Sets FLAG_KOLIMA_CURSE_WITNESSED.
static void KolimaForestEntrance_WitnessForestCurse(void) {
  struct ObjectEvent *record;
  s32 *party;
  const s32 *sparkleAction;
  const s32 *squashAction;
  s32 *miaInParty;
  const s32 *restoreScaleAction;
  const s32 *growFieldAction;
  const s32 *shrinkFieldAction;

  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  FieldCamera_MoveTo(PIXELS(246), -1, PIXELS(604), 0);
  party = &gKolimaForestEntrance_MiaInParty;
  *party = GameFlag_IsSet(FLAG_IN_PARTY(MIA));
  record = ObjectEvent_GetById(LOCALID_LEADER_PSYNERGY_FIELD);
  ObjectEvent_SetSpriteFlags(record, 0);
  record = ObjectEvent_GetById(LOCALID_IVAN_PSYNERGY_FIELD);
  ObjectEvent_SetSpriteFlags(record, 0);
  record = ObjectEvent_GetById(LOCALID_MIA_PSYNERGY_FIELD);
  ObjectEvent_SetSpriteFlags(record, 0);
  record = ObjectEvent_GetById(LOCALID_GARET_PSYNERGY_FIELD);
  ObjectEvent_SetSpriteFlags(record, 0);
  record = ObjectEvent_GetById(LOCALID_FIRST_GROUND_SPARKLE);
  ObjectEvent_SetSpriteFlags(record, 0);
  record = ObjectEvent_GetById(LOCALID_FIRST_GROUND_SPARKLE + 1);
  ObjectEvent_SetSpriteFlags(record, 0);
  record = ObjectEvent_GetById(LOCALID_FIRST_GROUND_SPARKLE + 2);
  ObjectEvent_SetSpriteFlags(record, 0);
  record = ObjectEvent_GetById(LOCALID_FIRST_GROUND_SPARKLE + 3);
  ObjectEvent_SetSpriteFlags(record, 0);
  record = ObjectEvent_GetById(LOCALID_LAST_GROUND_SPARKLE);
  ObjectEvent_SetSpriteFlags(record, 0);
  sparkleAction = gKolimaForestEntrance_GroundSparkleAction;
  ObjectEvent_SetActionScript(LOCALID_FIRST_GROUND_SPARKLE, sparkleAction);
  ObjectEvent_SetActionScript(LOCALID_FIRST_GROUND_SPARKLE + 1, sparkleAction);
  ObjectEvent_SetActionScript(LOCALID_FIRST_GROUND_SPARKLE + 2, sparkleAction);
  ObjectEvent_SetActionScript(LOCALID_FIRST_GROUND_SPARKLE + 3, sparkleAction);
  ObjectEvent_SetActionScript(LOCALID_LAST_GROUND_SPARKLE, sparkleAction);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(116), PIXELS(602));
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 254, 0x251);
  ObjectEvent_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  {
    struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

    if (leader != 0) {
      ObjectEvent_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
    }
  }
  {
    struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

    if (leader != 0) {
      ObjectEvent_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
    }
  }
  ObjectEvent_SetActionScript(GARET, gKolimaForestEntrance_GaretAction);
  ObjectEvent_SetActionScript(IVAN, gKolimaForestEntrance_IvanAction);
  if (*party != 0) {
    ObjectEvent_SetSpeed(MIA, PIXELS(3) / 5, PIXELS(3) / 10);
    {
      struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

      if (leader != 0) {
        ObjectEvent_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
      }
    }
    ObjectEvent_SetActionScript(MIA, gKolimaForestEntrance_MiaAction);
  }
  ObjectEvent_RefreshSelectorById(2);
  KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_SOUTHEAST, 40);
  KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_WEST, 20);
  KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_SOUTH, 40);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_SOUTHWEST, 60);
  KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_SOUTHEAST, 10);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_NORTHWEST, 10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_SOUTHWEST, 10);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  FieldScript_SetMessageId(MsgKolimaMatterIvan);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 10);
  FieldScript_SetMessageId(MsgKolimaSparklyStuffOnGround);
  KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTH, 20);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_EAST, 20);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_NORTHWEST, 40);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTH, 20);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_SOUTHWEST, 30);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTHWEST, 20);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_NORTHEAST, 30);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_SOUTHWEST, 20);
  KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTH, 10);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  Script_PlaySong(SE_206);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_StartInterpolation(1);
  WaitFrames(1);
  gKolimaForestEntrance_SparkleSoundOn = 1;
  FieldScheduler_AddCallback(KolimaForestEntrance_SpawnCurseSparkle,
                             TASK_PRIORITY_MAP);
  WaitFrames(20);
  PaletteFade_ApplyTarget(0x405210, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  PaletteFade_StartInterpolation(120);
  WaitFrames(60);
  squashAction = gKolimaForestEntrance_SquashAction;
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, squashAction);
  ObjectEvent_SetActionScript(GARET, squashAction);
  ObjectEvent_SetActionScript(IVAN, squashAction);
  ObjectEvent_SetActionScript(MIA, squashAction);
  FieldScript_WaitFrames(100);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 40);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    FieldScript_WaitFrames(40);
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    FieldScript_WaitFrames(40);
    KolimaForestEntrance_ShowMessageAndWait(MIA, 40);
  } else {
    gFieldWork->message += 1;
  }
  FieldScript_WaitFrames(20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    record = ObjectEvent_GetById(MIA);
    record->velocityY = PIXELS(2);
    FieldScript_WaitFrames(10);
    ObjectEvent_SetSpeed(MIA, PIXELS(2), PIXELS(2));
    ObjectEvent_SetDestinationOffset(MIA, -2, 0);
    ObjectEvent_SetActionScript(MIA, gKolimaForestEntrance_RestoreScaleAction);
    record = ObjectEvent_GetById(MIA);
    ObjectEvent_SetSpriteFlags(record, 0);
    Engine_ObjectEventSetAnimation(MIA, ANIM_COLLAPSE);
    FieldScript_WaitFrames(10);
  }
  record = ObjectEvent_GetById(0);
  record->velocityY = PIXELS(2);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(2));
  restoreScaleAction = gKolimaForestEntrance_RestoreScaleAction;
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, restoreScaleAction);
  record = ObjectEvent_GetById(0);
  ObjectEvent_SetSpriteFlags(record, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_COLLAPSE);
  FieldScript_WaitFrames(20);
  record = ObjectEvent_GetById(GARET);
  record->velocityY = PIXELS(2);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetSpeed(GARET, PIXELS(2), PIXELS(2));
  ObjectEvent_SetActionScript(GARET, restoreScaleAction);
  record = ObjectEvent_GetById(GARET);
  ObjectEvent_SetSpriteFlags(record, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIM_COLLAPSE);
  FieldScript_WaitFrames(40);
  record = ObjectEvent_GetById(IVAN);
  record->velocityY = PIXELS(2);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetActionScript(IVAN, restoreScaleAction);
  record = ObjectEvent_GetById(IVAN);
  ObjectEvent_SetSpriteFlags(record, 0);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_COLLAPSE);
  gKolimaForestEntrance_SparkleSoundOn = 0;
  FieldScript_WaitFrames(160);
  Task_Remove(KolimaForestEntrance_SpawnCurseSparkle);
  FieldScript_WaitFrames(120);
  PaletteFade_ApplyTarget(0x406218, 1);
  PaletteFade_StartInterpolation(60);
  WaitFrames(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = 0x800000;
  gFallingEffectState = KOLIMA_DUST_CLOSING;
  FieldScheduler_AddCallback(Kolima_UpdateCurseDust, TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(180);
  Script_PlaySong(MUS_021);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 80);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 40);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
  gFallingEffectState = KOLIMA_DUST_CLOSING_WIDE;
  ObjectMotion_SetVariantCallback(IVAN, 2);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(GARET, 1);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(MIA, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(IVAN, 3);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(GARET, 2);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(MIA, 2);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    KolimaForestEntrance_ShowMessageAndWait(MIA, 10);
  } else {
    gFieldWork->message += 1;
  }
  gFallingEffectState = KOLIMA_DUST_DRIFTING;
  ObjectEvent_GetById(0)->priorityFlags &= 254;
  ObjectEvent_GetById(GARET)->priorityFlags &= 254;
  ObjectEvent_GetById(IVAN)->priorityFlags &= 254;
  ObjectEvent_GetById(MIA)->priorityFlags &= 254;
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 3);
  ObjectMotion_SetActionVariant(GARET, 3);
  ObjectMotion_SetActionVariant(IVAN, 3);
  ObjectMotion_SetActionVariant(MIA, 3);
  gKolimaForestEntrance_PsynergyFieldHit = 0;
  FieldScheduler_AddCallback(KolimaForestEntrance_FlickerPsynergyFields,
                             TASK_PRIORITY_MAP);
  Script_PlaySong(SE_220);
  ObjectEvent_GetById(LOCALID_LEADER_PSYNERGY_FIELD)->priorityFlags &= 254;
  ObjectMotion_SetActionVariant(LOCALID_LEADER_PSYNERGY_FIELD, 2);
  ObjectEvent_SetPosition(LOCALID_LEADER_PSYNERGY_FIELD, PIXELS(253),
                          PIXELS(603));
  growFieldAction = gKolimaForestEntrance_PsynergyFieldGrowAction;
  ObjectEvent_SetActionScript(LOCALID_LEADER_PSYNERGY_FIELD, growFieldAction);
  ObjectEvent_GetById(LOCALID_IVAN_PSYNERGY_FIELD)->priorityFlags &= 254;
  ObjectMotion_SetActionVariant(LOCALID_IVAN_PSYNERGY_FIELD, 2);
  ObjectEvent_SetPosition(LOCALID_IVAN_PSYNERGY_FIELD, PIXELS(233),
                          PIXELS(629));
  ObjectEvent_SetActionScript(LOCALID_IVAN_PSYNERGY_FIELD, growFieldAction);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    ObjectEvent_GetById(LOCALID_MIA_PSYNERGY_FIELD)->priorityFlags &= 254;
    ObjectMotion_SetActionVariant(LOCALID_MIA_PSYNERGY_FIELD, 2);
    ObjectEvent_SetPosition(LOCALID_MIA_PSYNERGY_FIELD, PIXELS(207),
                            PIXELS(609));
    ObjectEvent_SetActionScript(LOCALID_MIA_PSYNERGY_FIELD, growFieldAction);
  }
  ObjectEvent_GetById(LOCALID_GARET_PSYNERGY_FIELD)->priorityFlags &= 254;
  ObjectMotion_SetActionVariant(LOCALID_GARET_PSYNERGY_FIELD, 2);
  ObjectEvent_SetPosition(LOCALID_GARET_PSYNERGY_FIELD, PIXELS(227),
                          PIXELS(580));
  ObjectEvent_SetActionScript(LOCALID_GARET_PSYNERGY_FIELD, growFieldAction);
  if (gFallingEffectState != KOLIMA_DUST_CENTRED) {
    do {
      WaitFrames(1);
    } while (gFallingEffectState != KOLIMA_DUST_CENTRED);
  }
  FieldScript_WaitFrames(300);
  Task_Remove(Kolima_UpdateCurseDust);
  FieldScript_WaitFrames(120);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_StartInterpolation(60);
  WaitFrames(60);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_LEADER_PSYNERGY_FIELD);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_IVAN_PSYNERGY_FIELD);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    ObjectMotion_EnableActionAndResetMotion(LOCALID_MIA_PSYNERGY_FIELD);
  }
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET_PSYNERGY_FIELD);
  WaitFrames(1);
  shrinkFieldAction = gKolimaForestEntrance_PsynergyFieldShrinkAction;
  ObjectEvent_SetActionScript(LOCALID_LEADER_PSYNERGY_FIELD, shrinkFieldAction);
  ObjectEvent_SetActionScript(LOCALID_IVAN_PSYNERGY_FIELD, shrinkFieldAction);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    ObjectEvent_SetActionScript(LOCALID_MIA_PSYNERGY_FIELD, shrinkFieldAction);
  }
  ObjectEvent_RunActionScriptAndWait(LOCALID_GARET_PSYNERGY_FIELD,
                                     shrinkFieldAction);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  FieldScript_WaitFrames(40);
  Script_OpenMessage(GARET, 0);
  ObjectEvent_SetPosition(LOCALID_TRET, PIXELS(220), PIXELS(494));
  ObjectEvent_SetPosition(LOCALID_LAUREL, PIXELS(220), PIXELS(494));
  WaitFrames(1);
  if (FieldScript_AskYesNoWithObjectEventAnimation(11, 0) == 1) {
    gFieldWork->message += 1;
  }
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    ObjectMotion_SetVarCbAndRefresh(MIA, 2);
    FieldScript_WaitFrames(10);
    FieldScript_SetMessageId(MsgKolimaKnowThoseFieldsWere);
    KolimaForestEntrance_ShowMessageAndWait(MIA, 40);
  }
  ObjectMotion_SetVariantCallback(GARET, 1);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  FieldScript_SetMessageId(MsgKolimaWasOurPsynergy);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 40);
  ObjectMotion_SetVarCbAndRefresh(GARET, 3);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetActionVariant(GARET, 2);
  ObjectEvent_GetById(GARET)->priorityFlags |= 1;
  record = ObjectEvent_GetById(GARET);
  ObjectEvent_SetSpriteFlags(record, 1);
  ObjectMotion_Launch(GARET, 6, 0);
  ObjectEvent_SetDestinationOffset(GARET, -3, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIM_STAND);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTH, 60);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  ObjectMotion_SetVariantCallback(GARET, 2);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 3);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(40);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTHWEST, 40);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 20);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTHWEST, 20);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 10);
  ObjectMotion_Launch(GARET, 2, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_Launch(GARET, 2, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(GARET, 4, 0);
  FieldScript_WaitFrames(20);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetVarCbAndRefresh(MIA, 2);
    FieldScript_WaitFrames(80);
    ObjectMotion_SetActionVariant(MIA, 2);
    ObjectEvent_GetById(MIA)->priorityFlags |= 1;
    record = ObjectEvent_GetById(MIA);
    ObjectEvent_SetSpriteFlags(record, 1);
    ObjectMotion_Launch(MIA, 4, 0);
    ObjectEvent_SetDestinationOffset(MIA, -2, 0);
    Engine_ObjectEventSetAnimation(MIA, ANIM_STAND);
    KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_NORTHEAST, 60);
    ObjectMotion_SetVarCbAndRefresh(MIA, 2);
    FieldScript_WaitFrames(20);
    KolimaForestEntrance_ShowMessageAndWait(MIA, 20);
  } else {
    gFieldWork->message += 1;
  }
  ObjectMotion_Launch(GARET, 2, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTH, 20);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 10);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetActionVariant(IVAN, 2);
  ObjectEvent_GetById(IVAN)->priorityFlags |= 1;
  record = ObjectEvent_GetById(IVAN);
  ObjectEvent_SetSpriteFlags(record, 1);
  ObjectMotion_Launch(IVAN, 4, 0);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_STAND);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_GetById(0)->priorityFlags |= 1;
  record = ObjectEvent_GetById(0);
  ObjectEvent_SetSpriteFlags(record, 1);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_SOUTHWEST, 60);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldScript_WaitFrames(60);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_NORTHWEST, 20);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_SOUTHWEST, 10);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTH, 10);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  FieldScript_WaitFrames(20);
  Script_OpenMessage(IVAN, 0);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
    gFieldWork->message += 1;
  } else {
    ObjectMotion_SetVarCbAndRefresh(GARET, 2);
    KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 10);
    ObjectMotion_SetVariantCallback(GARET, 2);
    Script_ShowMessage(GARET, 0);
  }
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTH, 10);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTH, 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    ObjectMotion_SetVarCbAndRefresh(MIA, 2);
    KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_EAST, 20);
    KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_SOUTHEAST, 10);
    Engine_ObjectEventSetAnimation(MIA, ANIM_SHAKE_HEAD);
    KolimaForestEntrance_ShowMessageAndWait(MIA, 10);
  } else {
    gFieldWork->message += 1;
  }
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_NORTHWEST, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTH, 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(80);
  KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTHEAST, 10);
  ObjectMotion_SetVariantCallback(IVAN, 2);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_NORTHWEST, 40);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_SOUTHWEST, 10);
  KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTH, 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(40);
  KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 10);
  Script_OpenMessage(GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(GARET, 2);
    FieldScript_WaitFrames(40);
    gFieldWork->message += 1;
  }
  Script_ShowMessage(GARET, 0);
  Script_PlaySong(MUS_021);
  PaletteFade_ApplyTarget(0x406218, 1);
  PaletteFade_StartInterpolation(60);
  WaitFrames(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = 0x800000;
  gFallingEffectState = KOLIMA_DUST_CLOSING;
  FieldScheduler_AddCallback(Kolima_UpdateCurseDust, TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(GARET, 2);
  ObjectMotion_SetVariantCallback(MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  FieldScript_WaitFrames(60);
  KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTH, 10);
  FieldScript_SetMessageId(MsgKolimaWatchOutItsHappeningAgain);
  KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
  KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_NORTH, 10);
  KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                              FACING_NORTH, 10);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_NORTH, 10);
  }
  ObjectEvent_GetById(0)->priorityFlags &= 254;
  ObjectEvent_GetById(GARET)->priorityFlags &= 254;
  ObjectEvent_GetById(IVAN)->priorityFlags &= 254;
  ObjectEvent_GetById(MIA)->priorityFlags &= 254;
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 3);
  ObjectMotion_SetActionVariant(GARET, 3);
  ObjectMotion_SetActionVariant(IVAN, 3);
  ObjectMotion_SetActionVariant(MIA, 3);
  gFallingEffectState = KOLIMA_DUST_CLOSING_WIDE;
  Script_PlaySong(SE_220);
  ObjectEvent_SetPosition(LOCALID_LEADER_PSYNERGY_FIELD, PIXELS(253),
                          PIXELS(603));
  growFieldAction = gKolimaForestEntrance_PsynergyFieldGrowAction;
  ObjectEvent_SetActionScript(LOCALID_LEADER_PSYNERGY_FIELD, growFieldAction);
  ObjectEvent_SetPosition(LOCALID_IVAN_PSYNERGY_FIELD, PIXELS(233),
                          PIXELS(629));
  ObjectEvent_SetActionScript(LOCALID_IVAN_PSYNERGY_FIELD, growFieldAction);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    ObjectEvent_SetPosition(LOCALID_MIA_PSYNERGY_FIELD, PIXELS(207),
                            PIXELS(609));
    ObjectEvent_SetActionScript(LOCALID_MIA_PSYNERGY_FIELD, growFieldAction);
  }
  ObjectEvent_SetPosition(LOCALID_GARET_PSYNERGY_FIELD, PIXELS(227),
                          PIXELS(580));
  ObjectEvent_SetActionScript(LOCALID_GARET_PSYNERGY_FIELD, growFieldAction);
  FieldScript_WaitFrames(120);
  gFallingEffectState = KOLIMA_DUST_DRIFTING;
  do {
    WaitFrames(1);
  } while (gFallingEffectState != KOLIMA_DUST_CENTRED);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_TRET, 80);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(40);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_TRET, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_EAST, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTH, 40);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_NORTH, 80);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_EAST, 40);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_TRET, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_NORTH, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_SHAKE_HEAD);
  Engine_ObjectEventSetAnimation(MIA, ANIM_SHAKE_HEAD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(60);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_EAST, 20);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(GARET, 2);
  ObjectMotion_SetVariantCallback(MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_EAST, 20);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_TRET, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(40);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_NORTH, 10);
  KolimaForestEntrance_ShowMessageAndWait(LOCALID_LAUREL, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  FieldScript_WaitFrames(60);
  Script_ShowMessage(LOCALID_LAUREL, 0);
  Script_ShowMessage(LOCALID_TRET, 0);
  Task_Remove(Kolima_UpdateCurseDust);
  FieldScript_WaitFrames(80);
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_StartInterpolation(60);
  WaitFrames(80);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_LEADER_PSYNERGY_FIELD);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_IVAN_PSYNERGY_FIELD);
  miaInParty = &gKolimaForestEntrance_MiaInParty;
  ObjectMotion_EnableActionAndResetMotion(LOCALID_MIA_PSYNERGY_FIELD);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET_PSYNERGY_FIELD);
  WaitFrames(1);
  shrinkFieldAction = gKolimaForestEntrance_PsynergyFieldShrinkAction;
  ObjectEvent_SetActionScript(LOCALID_LEADER_PSYNERGY_FIELD, shrinkFieldAction);
  ObjectEvent_SetActionScript(LOCALID_IVAN_PSYNERGY_FIELD, shrinkFieldAction);
  if (*miaInParty != 0) {
    ObjectEvent_SetActionScript(LOCALID_MIA_PSYNERGY_FIELD, shrinkFieldAction);
  }
  ObjectEvent_RunActionScriptAndWait(LOCALID_GARET_PSYNERGY_FIELD,
                                     shrinkFieldAction);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetActionVariant(GARET, 2);
  ObjectMotion_SetActionVariant(IVAN, 2);
  ObjectMotion_SetActionVariant(MIA, 2);
  ObjectEvent_GetById(0)->priorityFlags |= 1;
  ObjectEvent_GetById(GARET)->priorityFlags |= 1;
  ObjectEvent_GetById(IVAN)->priorityFlags |= 1;
  ObjectEvent_GetById(MIA)->priorityFlags |= 1;
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTHEAST, 10);
  Script_OpenMessage(IVAN, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_EAST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetVarCbAndRefresh(GARET, 2);
    FieldScript_WaitFrames(10);
    Script_OpenMessage(GARET, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_EAST, 20);
      ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      FieldScript_WaitFrames(40);
      KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTH, 20);
      KolimaForestEntrance_ShowMessageAndWait(GARET, 10);
      KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTH, 20);
      KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTHEAST, 20);
      ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
      KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 20);
    } else {
      KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_EAST, 20);
      ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      FieldScript_WaitFrames(40);
      KolimaForestEntrance_FaceObjectEventAndWait(GARET, FACING_SOUTH, 20);
      FieldScript_SetMessageId(MsgKolimaHesAsStumpedAsWe);
      KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
      ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
    }
    Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
    FieldScript_WaitFrames(10);
    FieldScript_SetMessageId(MsgKolimaYoureIsaacThereIsntMuch);
    KolimaForestEntrance_ShowMessageAndWait(GARET, 10);
    ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
    KolimaForestEntrance_FaceObjectEventAndWait(LOCALID_PARTY_LEADER,
                                                FACING_SOUTHWEST, 20);
    Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Script_OpenMessage(IVAN, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_WaitFrames(20);
      ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
      FieldScript_WaitFrames(40);
      KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTHEAST, 10);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
      if (*miaInParty != 0) {
        KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_EAST, 10);
        ObjectMotion_SetVariantCallback(MIA, 3);
        KolimaForestEntrance_ShowMessageAndWait(MIA, 20);
      } else {
        gFieldWork->message += 1;
      }
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                            0);
      ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      FieldScript_WaitFrames(40);
      ObjectMotion_SetVarCbAndRefresh(GARET, 2);
      KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
      ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      FieldScript_WaitFrames(120);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 40);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_SOUTHEAST, 10);
        ObjectMotion_SetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
        KolimaForestEntrance_ShowMessageAndWait(MIA, 10);
      } else {
        gFieldWork->message += 1;
      }
      FieldScript_WaitFrames(60);
      ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTHWEST, 40);
        KolimaForestEntrance_FaceObjectEventAndWait(IVAN, FACING_NORTHEAST, 20);
      }
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 10);
      ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
      ObjectMotion_SetVarCbAndRefresh(GARET, 2);
      FieldScript_WaitFrames(40);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
      ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
      ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
      FieldScript_WaitFrames(20);
      Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
      ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
    } else {
      ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      FieldScript_WaitFrames(40);
      ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
      FieldScript_SetMessageId(MsgKolimaThatsReliefIsaacThoughtYoud);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
      if (*miaInParty != 0) {
        KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_EAST, 10);
        ObjectMotion_SetVariantCallback(MIA, 1);
        KolimaForestEntrance_ShowMessageAndWait(MIA, 20);
      } else {
        gFieldWork->message += 1;
      }
      ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                            0);
      FieldScript_WaitFrames(40);
      ObjectMotion_SetVarCbAndRefresh(GARET, 2);
      KolimaForestEntrance_ShowMessageAndWait(GARET, 20);
      ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      FieldScript_WaitFrames(80);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 40);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceObjectEventAndWait(MIA, FACING_SOUTHEAST, 20);
        Engine_ObjectEventSetAnimation(MIA, ANIM_SHAKE_HEAD);
        KolimaForestEntrance_ShowMessageAndWait(MIA, 40);
      } else {
        gFieldWork->message += 1;
      }
      ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
      FieldScript_WaitFrames(20);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
      ObjectMotion_SetVariantCallback(GARET, 2);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
      FieldScript_WaitFrames(40);
      KolimaForestEntrance_ShowMessageAndWait(IVAN, 20);
    }
  }
  Script_PlaySong(SONG_FADE_OUT_BGM);
  ObjectEvent_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ObjectEventSetAnimation(GARET, ANIM_WALK);
  {
    struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

    if (leader != 0) {
      ObjectEvent_SetDestination(GARET, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
  }
  ObjectMotion_CommitCurrentPositionAndActivate(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_WALK);
  {
    struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

    if (leader != 0) {
      ObjectEvent_SetDestination(IVAN, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
  }
  ObjectMotion_CommitCurrentPositionAndActivate(IVAN);
  ObjectEvent_SetPosition(IVAN, 0, 0);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Engine_ObjectEventSetAnimation(MIA, ANIM_WALK);
    {
      struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

      if (leader != 0) {
        ObjectEvent_SetDestination(MIA, leader->x.part.pixel,
                                   leader->z.part.pixel);
      }
    }
    ObjectMotion_CommitCurrentPositionAndActivate(MIA);
    ObjectEvent_SetPosition(MIA, 0, 0);
  }
  GameFlag_Set(FLAG_KOLIMA_CURSE_WITNESSED);
  ObjectEvent_SetPosition(LOCALID_TRET, 0, 0);
  ObjectEvent_SetPosition(LOCALID_LAUREL, 0, 0);
  Map_PlayMusic();
  FieldScript_End();
}

static void KolimaForestEntrance_ShowMessageAndWait(s32 speaker, s32 frames) {
  FieldScript_ShowMessageAndWait(speaker, 0);
  FieldScript_WaitFrames(frames);
}

/// Turns an object event at once, then leaves time for the next beat.
static void KolimaForestEntrance_FaceObjectEventAndWait(s32 localId, s32 facing,
                                                        s32 waitFrames) {
  ObjectMotion_ArmCallback(localId, facing, 0);
  FieldScript_WaitFrames(waitFrames);
}

#include "common/kolima_grow.h"

/// Every eighth frame a sparkle of Tret's glamour (SPRITE_GOLDEN_RINGS) falls
/// toward the party, with its tinkle while the rain is at its height.
static void KolimaForestEntrance_SpawnCurseSparkle(void) {
  struct ObjectEvent *sparkle;
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
    Sound_PlaySong(SE_200);
  sparkle = ObjectEvent_Create(SPRITE_GOLDEN_RINGS, spawnX, 0, spawnZ);
  if (sparkle == 0)
    return;
  sprite = sparkle->sprite;
  sprite->flags = framePhase;
  priorityFlags = 0xff & ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  priorityFlags &= sparkle->priorityFlags;
  sparkle->priorityFlags = priorityFlags;
  sprite->image.priority = 1;
  sparkle->scaleX = Q16_ONE / 10;
  sparkle->speed = PIXELS(8);
  sparkle->acceleration = PIXELS(8);
  sparkle->motionFlags = framePhase;
  ObjectDispatch_SelectAnimation(sparkle, 2);
  Object_SetPosition(sparkle, targetX, 0, targetZ);
  ObjectEvent_SetScript(sparkle, gKolimaForestEntrance_SparkleScript);
}

#include "common/kolima_blink.h"

/// The end of a sparkle's fall: one that lands on a field (the Psynergy fields'
/// area, which differs with Mia) bursts with a sound, and the Psynergy fields
/// are told.
static s32 KolimaForestEntrance_BurstOnPsynergyField(
    struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *sparkle = (struct ObjectEvent *)interpreter;
  s32 x;

  if (!(gKolimaForestEntrance_MiaInParty != 0
            ? (x = sparkle->x.fixed,
               (x > 0xc00000 && x < 0x1120000 && sparkle->z.fixed > 0x2360000 &&
                sparkle->z.fixed < 0x2640000) ||
                   (x > 0xca0000 && x < 0xff0000 &&
                    sparkle->z.fixed > 0x2250000 &&
                    sparkle->z.fixed < 0x2780000))
            : (x = sparkle->x.fixed,
               (x > 0xc00000 && x < 0xf40000 && sparkle->z.fixed > 0x2250000 &&
                sparkle->z.fixed <= 0x248ffff) ||
                   (x > 0xf40000 && x < 0x1120000 &&
                    sparkle->z.fixed > 0x23b0000 &&
                    sparkle->z.fixed <= 0x25cffff) ||
                   (x > 0xd30000 && x < 0xff0000 &&
                    sparkle->z.fixed > 0x2540000 &&
                    sparkle->z.fixed < 0x2780000)))) {
    return 0;
  }
  Sound_PlaySong(SE_106);
  // UB: this routine returns void; call it through its declared type.
#ifdef UBFIX
  ObjectEvent_SetScript(sparkle, gKolimaForestEntrance_SparkleBurstScript);
#else
  /* FAKEMATCH(wrapper): called through a cast to the unprototyped call the
   * bytes show, which loads the script before r0; as a call by its prototype,
   * 1 of 79 instructions differ in tbs-en. */
  ((s32 (*)())ObjectDispatch_Initialize)(
      (s32)sparkle, (s32)gKolimaForestEntrance_SparkleBurstScript);
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
  struct ObjectEvent *p;

  p = ObjectEvent_GetById(LOCALID_LEADER_PSYNERGY_FIELD);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = ObjectEvent_GetById(LOCALID_IVAN_PSYNERGY_FIELD);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) != 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = ObjectEvent_GetById(LOCALID_MIA_PSYNERGY_FIELD);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = ObjectEvent_GetById(LOCALID_GARET_PSYNERGY_FIELD);
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

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
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
    SC_ANIMATION(1),
    SC_ADD(SW_Z, -0x1999),
    SC_ADD(SW_SCALE_X, -983),
    SC_ADD(SW_SCALE_Y, -983),
    SC_WAIT(1),
    SC_LOOP(90, SC_TOP),
    SC_POSITION(0, 0, 0),
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

struct MapWarpDest gKolimaForestEntrance_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 384, 0, 312, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {KOLIMA_FOREST_ENTRANCE_FROM_KOLIMA_FOREST_GLADE, CONDITION_ALWAYS, 24, 0,
     128, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 172, 0, 602,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

/// On into the forest (MapId_KolimaForestGlade) and out to the world map
/// (MapId_WorldMap), with two warps no map event uses back into this map.
u32 gKolimaForestEntrance_Warps[] = {
    MAP_WARPS(KolimaForestEntrance),
    MAP_WARP(KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE, KolimaForestGlade,
             KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_ENTRANCE),
    MAP_WARP(KOLIMA_FOREST_ENTRANCE_UNUSED_WARP_A, KolimaForestEntrance,
             KOLIMA_FOREST_ENTRANCE_UNUSED),
    MAP_WARP(KOLIMA_FOREST_ENTRANCE_UNUSED_WARP_B, KolimaForestEntrance,
             KOLIMA_FOREST_ENTRANCE_UNUSED),
    MAP_WARP(KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP, WorldMap,
             WORLD_MAP_FROM_KOLIMA_FOREST_ENTRANCE),
    MAP_WARPS_END,
};

/// Garet, Ivan and Mia for the curse; the three logs; Tret and Laurel; the
/// four Psynergy fields; and the five ground sparkles.
// clang-format off
struct ObjectEventTemplate gKolimaForestEntrance_ObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
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
        OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY,
        OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY,
        OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(232), 0, PIXELS(655), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(166), 0, PIXELS(576), FACING_SOUTH, TALK_FACING_PARTY,
        OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(189), 0, PIXELS(532), FACING_SOUTH, TALK_FACING_PARTY,
        OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(288), 0, PIXELS(570), FACING_SOUTH, TALK_FACING_PARTY,
        OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(331), 0, PIXELS(659), FACING_SOUTH, TALK_FACING_PARTY,
        OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};
// clang-format on

struct MapEvent gKolimaForestEntrance_Events[] = {
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestEntrance_PushLog)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForest_HopEast)},
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS, KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP},
    {MAP_EVENTS_END},
};

s32 gKolimaForestEntrance_SparkleScript[] = {
    SC_CALL(FieldEffect_StretchUntilArrived),
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
