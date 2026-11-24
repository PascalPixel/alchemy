/// @file
/// Kolima Forest (SceneId_KolimaForestEntrance), the first of the forest's five
/// scenes, entered from the World Map (KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP).
///
/// If the party comes here before it has been to Kolima, the curse's event
/// plays here instead of in the village
/// (KolimaForestEntrance_WitnessForestCurse, until
/// FLAG_KOLIMA_CURSE_WITNESSED). Sparkles of Tret's glamour rain down, fields
/// of the party's own Psynergy shield it, and Tret and Laurel, unseen, speak of
/// the dying forest. Five sparkles lie on the ground until Tret lifts the curse
/// (FLAG_KOLIMA_CURSE_LIFTED).
///
/// The fields flicker. The leader's and Mia's show on even frames and Ivan's
/// and Garet's on odd ones, each lifted out of sight on the others
/// (KolimaForestEntrance_FlickerPsynergyFields). A sparkle bursts where it
/// lands only inside the boxes KolimaForestEntrance_BurstOnPsynergyField tests,
/// two with Mia in the party and three without her.
///
/// Three logs lie here, rolled by walking into them. The one pushed west to the
/// water's edge tips into the river and floats there, cells the party can
/// cross. From the bank the leader can hop east (step trigger 11,
/// KolimaForest_HopEast).
///
/// KOLIMA_FOREST_ENTRANCE_TO_KOLIMA_FOREST_GLADE leads on into the forest
/// (SceneId_KolimaForestGlade), and KOLIMA_FOREST_ENTRANCE_TO_WORLD_MAP back to
/// the World Map.
///
/// FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER: the log is in the river. Like
/// every scene temp flag it is cleared when the party next comes in, so the log
/// is back on the bank, but a battle keeps it.

#include "flags.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/character_ids.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/import.h"
#include "scene/kolima.h"
#include "scene/kolima_curse_dust.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap_slots.h"
#include "system/io_reg.h"
#include "system/main_routine.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "system/system_call.h"
#include "text/msg_ids.h"

extern s32 gKolimaForestEntrance_GaretAction[];
extern s32 gKolimaForestEntrance_IvanAction[];
extern s32 gKolimaForestEntrance_MiaAction[];
extern s32 gKolima_PartySquashScript[];
extern s32 gKolima_PartyRestoreScaleScript[];
extern s32 gKolimaForestEntrance_PsynergyFieldGrowAction[];
extern s32 gKolimaForestEntrance_PsynergyFieldShrinkAction[];
extern s32 gKolimaForestEntrance_GroundSparkleAction[];
extern struct SceneEntrance gKolimaForestEntrance_Entrances[];
extern u32 gKolimaForestEntrance_Exits[];
extern struct ActorPlacement gKolimaForestEntrance_ActorPlacements[];
extern struct Trigger gKolimaForestEntrance_Triggers[];
extern s32 gKolimaForestEntrance_SparkleScript[];
extern s32 gKolima_SparkleBurstScript[];
extern s32 gKolima_CurseDustScript[];

/// The scene's actors after Garet's, Ivan's and Mia's party slots,
/// numbering the rows of gKolimaForestEntrance_ActorPlacements from
/// ACTOR_FIRST_PLACED.
enum {
  /// The three logs. The last is the one that can reach the river.
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_RIVER_LOG = ACTOR_FIRST_LOG + 2,
  /// Tret's wrathful side, unseen while the curse falls.
  ACTOR_TRET,
  /// Unseen while the curse falls.
  ACTOR_LAUREL,
  /// The fields of Psynergy, one over each party member. Mia's only with Mia.
  ACTOR_LEADER_PSYNERGY_FIELD,
  ACTOR_IVAN_PSYNERGY_FIELD,
  ACTOR_MIA_PSYNERGY_FIELD,
  ACTOR_GARET_PSYNERGY_FIELD,
  /// The sparkles of Tret's glamour on the ground, gone once the curse is
  /// lifted.
  ACTOR_FIRST_GROUND_SPARKLE,
  ACTOR_LAST_GROUND_SPARKLE = ACTOR_FIRST_GROUND_SPARKLE + 4
};

static void KolimaForestEntrance_WitnessForestCurse(void);
static void KolimaForestEntrance_ShowMessageAndWait(s32 speaker, s32 frames);
static void KolimaForestEntrance_FaceActorAndWait(s32 actorId, s32 facing,
                                                  s32 waitFrames);
static void KolimaForestEntrance_SpawnCurseSparkle(void);
static void KolimaForestEntrance_FlickerPsynergyFields(void);

#include "shared/pushed_actor.c"

/// The river log has tipped into the river and floats there as ground the party
/// can cross.
#define FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER FLAG_SCENE_TEMP_00

/// The column at the water's edge, where the river log pushed west tips in.
#define RIVER_EDGE_COLUMN 20

/// The end of a field's growing: if a sparkle has burst on a field, the field
/// takes its next pose.
static s32 KolimaForestEntrance_FlashHitPsynergyFields(struct Actor *actor) {
  s32 *pending = &gKolimaForestEntrance_PsynergyFieldHit;
  if (*pending) {
    Actor_SelectAnimation(actor, 2);
    *pending = 0;
  }
  return 1;
}

/// Each frame, a ground sparkle's timer runs on at random.
static s32 KolimaForestEntrance_TwinkleGroundSparkle(struct Actor *actor) {
  actor->timer = (actor->timer + (Random_Below(100)));
  if (actor->timer > 1000)
    Actor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
  else
    Actor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
  if (actor->timer > 1200) {
    actor->timer = 0;
  }
  return 1;
}

/// Where the party enters, from the next scene of the forest
/// (SceneId_KolimaForestGlade) and from the World Map.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gKolimaForestEntrance_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// The ways on into the forest (SceneId_KolimaForestGlade) and back to the
/// World Map.
const u32 *Scene_GetExits(void) { return gKolimaForestEntrance_Exits; }

/// The party's companions, the logs, Tret, Laurel, the Psynergy fields and the
/// ground sparkles.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gKolimaForestEntrance_ActorPlacements;
}

/// In Kolima Forest (SceneId_KolimaForestEntrance), walking into a log rolls it
/// ahead until something stops it.
static void KolimaForestEntrance_PushLog(void) {
  struct PushedActorProbe result;
  Event_Start();

  if (PushedActor_FindClearPosition(&result)) {
    PushedActor_Roll(result);
    if (result.actorSlot == ACTOR_RIVER_LOG &&
        (FIELD_CELL(result.position.x)) == RIVER_EDGE_COLUMN) {
      Actor_SetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_ROLL_WEST);
      Actor_SetDestinationOffset(ACTOR_RIVER_LOG, -18, 6);
      Event_Wait(30);
      Sound_Play(SE_LOG_FALL);
      Actor_SetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_DROP_IN);
      // It floats under the party's feet, and the river's cells under it
      // become ground to cross on.
      Actor_Get(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(0, 17, 2, 4, 19, 17);
      MapGrid_FillCollisionRectangle(2, 20, 17, 1, 4, CELL_BEHAVIOR_NORMAL);
      GameFlag_Set(FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_RIVER_LOG), 0);
    }
  }

  Event_End();
}

#include "shared/kolima_jump.c"

/// The logs, the hop east from the bank and the two ways out.
const struct Trigger *Scene_GetTriggers(void) {
  return gKolimaForestEntrance_Triggers;
}

/// On entry a log already in the river floats there again and the logs mark the
/// cells they cover.
s32 Scene_Initialize(void) {
  PushedActor_BlockFootprint(ACTOR_RIVER_LOG);
  if (GameFlag_Get(FLAG_KOLIMA_FOREST_ENTRANCE_LOG_IN_RIVER) != 0) {
    Actor_Get(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(0, 17, 2, 4, 19, 17);
    MapGrid_FillCollisionRectangle(2, 20, 17, 1, 4, CELL_BEHAVIOR_NORMAL);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_RIVER_LOG), 0);
  }
  PushedActor_BlockFootprint(ACTOR_FIRST_LOG);
  PushedActor_BlockFootprint(ACTOR_FIRST_LOG + 1);
  if (gMainRoutineState.entrance == KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP) {
    if (GameFlag_Get(FLAG_KOLIMA_CURSE_WITNESSED) == 0) {
      KolimaForestEntrance_WitnessForestCurse();
    }
  }
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Actor_SetPosition(ACTOR_FIRST_GROUND_SPARKLE, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_GROUND_SPARKLE + 1, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_GROUND_SPARKLE + 2, 0, 0);
    Actor_SetPosition(ACTOR_FIRST_GROUND_SPARKLE + 3, 0, 0);
    Actor_SetPosition(ACTOR_LAST_GROUND_SPARKLE, 0, 0);
  }
  return 0;
}

/// In Kolima Forest, on the first entry from the World Map before the curse has
/// been seen in Kolima (FLAG_KOLIMA_CURSE_WITNESSED clear), the party sees the
/// curse fall. It is the same event as Kolima_WitnessForestCurse without the
/// trees that were people.
static void KolimaForestEntrance_WitnessForestCurse(void) {
  s32 *party;
  const s32 *sparkleAction;
  const s32 *squashAction;
  s32 *miaInParty;
  const s32 *restoreScaleAction;
  const s32 *growFieldAction;
  const s32 *shrinkFieldAction;

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  FieldCamera_MoveTo(246, CAMERA_KEEP, 604, 0);
  party = &gKolimaForestEntrance_MiaInParty;
  *party = GameFlag_Get(FLAG_IN_PARTY(MIA));
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LEADER_PSYNERGY_FIELD), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN_PSYNERGY_FIELD), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA_PSYNERGY_FIELD), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET_PSYNERGY_FIELD), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_GROUND_SPARKLE), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_GROUND_SPARKLE + 1), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_GROUND_SPARKLE + 2), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_GROUND_SPARKLE + 3), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LAST_GROUND_SPARKLE), 0);
  sparkleAction = gKolimaForestEntrance_GroundSparkleAction;
  Actor_SetScript(ACTOR_FIRST_GROUND_SPARKLE, sparkleAction);
  Actor_SetScript(ACTOR_FIRST_GROUND_SPARKLE + 1, sparkleAction);
  Actor_SetScript(ACTOR_FIRST_GROUND_SPARKLE + 2, sparkleAction);
  Actor_SetScript(ACTOR_FIRST_GROUND_SPARKLE + 3, sparkleAction);
  Actor_SetScript(ACTOR_LAST_GROUND_SPARKLE, sparkleAction);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 116, 602);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 254, 593);
  Actor_SetSpeed(ACTOR_GARET, 0.6);
  Actor_SetSpeed(ACTOR_IVAN, 0.6);
  {
    struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

    if (leader != NULL) {
      Actor_SetPositionFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
    }
  }
  {
    struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

    if (leader != NULL) {
      Actor_SetPositionFX(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
    }
  }
  Actor_SetScript(ACTOR_GARET, gKolimaForestEntrance_GaretAction);
  Actor_SetScript(ACTOR_IVAN, gKolimaForestEntrance_IvanAction);
  if (*party != 0) {
    Actor_SetSpeed(ACTOR_MIA, 0.6);
    {
      struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

      if (leader != NULL) {
        Actor_SetPositionFX(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
      }
    }
    Actor_SetScript(ACTOR_MIA, gKolimaForestEntrance_MiaAction);
  }
  Actor_WaitForScriptEndById(ACTOR_IVAN);
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
  // "What is the matter, Ivan?"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 10);
  Event_SetMessageId(MsgKolimaSparklyStuffOnGround);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  // "That sparkly stuff on the ground... What is it?"
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
  Sound_Play(SOUND_FADE_OUT_BGM);
  Sound_Play(SE_CURSE_FALL);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  wait(1);
  gKolimaForestEntrance_SparkleSoundOn = 1;
  Task_AddFrame(KolimaForestEntrance_SpawnCurseSparkle, TASK_PASS_FRAME);
  wait(20);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(16, 16, 20), PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(120);
  wait(60);
  squashAction = gKolima_PartySquashScript;
  Actor_SetScript(ACTOR_PARTY_LEADER, squashAction);
  Actor_SetScript(ACTOR_GARET, squashAction);
  Actor_SetScript(ACTOR_IVAN, squashAction);
  Actor_SetScript(ACTOR_MIA, squashAction);
  Event_Wait(100);
  // "Aaah! What's happening!?"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  // "My mind!!! Someone is attacking my-"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 40);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(40);
    // "I... I can't... I can't even stand up!"
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 40);
  } else {
    gFieldState->message += 1;
  }
  Event_Wait(20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_Get(ACTOR_MIA)->velocityY = toFIXED(2);
    Event_Wait(10);
    Actor_SetSpeedAndAcceleration(ACTOR_MIA, 2, 2);
    Actor_SetDestinationOffset(ACTOR_MIA, -2, 0);
    Actor_SetScript(ACTOR_MIA, gKolima_PartyRestoreScaleScript);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), 0);
    Actor_SetAnimation(ACTOR_MIA, ANIM_COLLAPSE);
    Event_Wait(10);
  }
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = toFIXED(2);
  Event_Wait(10);
  Actor_SetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 2);
  restoreScaleAction = gKolima_PartyRestoreScaleScript;
  Actor_SetScript(ACTOR_PARTY_LEADER, restoreScaleAction);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
  Event_Wait(20);
  Actor_Get(ACTOR_GARET)->velocityY = toFIXED(2);
  Event_Wait(10);
  Actor_SetSpeedAndAcceleration(ACTOR_GARET, 2, 2);
  Actor_SetScript(ACTOR_GARET, restoreScaleAction);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_COLLAPSE);
  Event_Wait(40);
  Actor_Get(ACTOR_IVAN)->velocityY = toFIXED(2);
  Event_Wait(10);
  Actor_SetScript(ACTOR_IVAN, restoreScaleAction);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_COLLAPSE);
  gKolimaForestEntrance_SparkleSoundOn = 0;
  Event_Wait(160);
  Task_Remove(KolimaForestEntrance_SpawnCurseSparkle);
  Event_Wait(120);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  wait(60);
  gKolima_CurseDustWidth = 0;
  gKolima_CurseDustOffset = toFIXED(128);
  gKolima_CurseDustState = KOLIMA_DUST_CLOSING;
  Task_AddFrame(Kolima_UpdateCurseDust, TASK_PASS_FRAME);
  Event_Wait(180);
  Sound_Play(MUS_FOREST_THEME);
  // "What is that? Something's falling from the sky!"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 80);
  // "It's sparkling! Could this be what turned those people into..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  // "We've got to get out of here!"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
  gKolima_CurseDustState = KOLIMA_DUST_CLOSING_WIDE;
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
  // "I can't! I can't move!!"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    // "Oh, no... If it touches us, we'll turn into trees!"
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 10);
  } else {
    gFieldState->message += 1;
  }
  gKolima_CurseDustState = KOLIMA_DUST_DRIFTING;
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_IVAN)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Actor_SetSpritePriority(ACTOR_GARET, 3);
  Actor_SetSpritePriority(ACTOR_IVAN, 3);
  Actor_SetSpritePriority(ACTOR_MIA, 3);
  gKolimaForestEntrance_PsynergyFieldHit = 0;
  Task_AddFrame(KolimaForestEntrance_FlickerPsynergyFields, TASK_PASS_FRAME);
  Sound_Play(SE_SHIMMER);
  Actor_Get(ACTOR_LEADER_PSYNERGY_FIELD)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_LEADER_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_LEADER_PSYNERGY_FIELD, 253, 603);
  growFieldAction = gKolimaForestEntrance_PsynergyFieldGrowAction;
  Actor_SetScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldAction);
  Actor_Get(ACTOR_IVAN_PSYNERGY_FIELD)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_IVAN_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_IVAN_PSYNERGY_FIELD, 233, 629);
  Actor_SetScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldAction);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_Get(ACTOR_MIA_PSYNERGY_FIELD)->priorityFlags &=
        ~ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpritePriority(ACTOR_MIA_PSYNERGY_FIELD, 2);
    Actor_SetPosition(ACTOR_MIA_PSYNERGY_FIELD, 207, 609);
    Actor_SetScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldAction);
  }
  Actor_Get(ACTOR_GARET_PSYNERGY_FIELD)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_GARET_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_GARET_PSYNERGY_FIELD, 227, 580);
  Actor_SetScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldAction);
  while (gKolima_CurseDustState != KOLIMA_DUST_CENTRED) {
    wait(1);
  }
  Event_Wait(300);
  Task_Remove(Kolima_UpdateCurseDust);
  Event_Wait(120);
  Sound_Play(SOUND_FADE_OUT_BGM);
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
  // "Isaac... What just happened to us? That barrier... Did it save..."
  Event_OpenMessage(ACTOR_GARET, 0);
  Actor_SetPosition(ACTOR_TRET, 220, 494);
  Actor_SetPosition(ACTOR_LAUREL, 220, 494);
  wait(1);
  // Tret, unseen, takes the nod or the shake of the head.
  if (Event_AskYesNoWithActorAnimation(ACTOR_TRET, 0) == 1) {
    gFieldState->message += 1;
  }
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  // Yes: "I think so, too, Isaac. I think I know what those domes..."
  // No: "No, I think Garet is right. I think I know what those..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(10);
    Event_SetMessageId(MsgKolimaKnowThoseFieldsWere);
    // "You know what those fields were?"
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 40);
  }
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(80);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_SetMessageId(MsgKolimaWasOurPsynergy);
  // "It was our Psynergy."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(40);
  Actor_SetSpritePriority(ACTOR_GARET, 2);
  Actor_Get(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), SPRITE_FLAG_SHADOW);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_SetDestinationOffset(ACTOR_GARET, -3, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 60);
  // "What are you, crazy?"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  // "I didn't use any Psynergy!"
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
  // "Hey... I can stand... I can move again!"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(80);
    Actor_SetSpritePriority(ACTOR_MIA, 2);
    Actor_Get(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), SPRITE_FLAG_SHADOW);
    Actor_Jump(ACTOR_MIA, 4, 0);
    Actor_SetDestinationOffset(ACTOR_MIA, -2, 0);
    Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
    KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_NORTHEAST, 60);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    // "You're right. I can stand, too..."
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 20);
  } else {
    gFieldState->message += 1;
  }
  Actor_Jump(ACTOR_GARET, 2, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
  // "Ivan, it's okay! Go ahead! Stand up!"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_SetSpritePriority(ACTOR_IVAN, 2);
  Actor_Get(ACTOR_IVAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), SPRITE_FLAG_SHADOW);
  Actor_Jump(ACTOR_IVAN, 4, 0);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(10);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
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
  // "Those barriers..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(20);
  // "We thought that Psynergy was only used when we concentrate,..."
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    gFieldState->message += 1;
  } else {
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
    Actor_StartRepeatedMotion(ACTOR_GARET, 2);
    // "You're such a liar, Isaac!"
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  // "When I was training, I'd focus so hard, I thought my head..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  // "Well, maybe it's just easier for you, but when I use Psynergy..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
    KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
    Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
    // "But what? The only way you can use Psynergy is by concentrating!"
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 10);
  } else {
    gFieldState->message += 1;
  }
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
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
  // "Anyway, I'm certain that was Psynergy we were projecting."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST,
                                        40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                        10);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  // "That must mean that some Psynergy acts only when it's needed."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "You mean, like when we're in danger?"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(40);
  // "I think it's like when we strike critical hits in battle..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  // "I can agree with what Garet is saying. It is definitely..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
  // "Wouldn't it be something if we could use this power at any..."
  Event_OpenMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  } else {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(40);
    gFieldState->message += 1;
  }
  // Yes: "Yeah! Let's start trying to master this power!"
  // No: "You... You're not really into this, are you?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Sound_Play(MUS_FOREST_THEME);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  wait(60);
  gKolima_CurseDustWidth = 0;
  gKolima_CurseDustOffset = toFIXED(128);
  gKolima_CurseDustState = KOLIMA_DUST_CLOSING;
  Task_AddFrame(Kolima_UpdateCurseDust, TASK_PASS_FRAME);
  Event_Wait(80);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(60);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
  Event_SetMessageId(MsgKolimaWatchOutItsHappeningAgain);
  // "Watch out! It's happening again!"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_NORTH, 10);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
  }
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_IVAN)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Actor_SetSpritePriority(ACTOR_GARET, 3);
  Actor_SetSpritePriority(ACTOR_IVAN, 3);
  Actor_SetSpritePriority(ACTOR_MIA, 3);
  gKolima_CurseDustState = KOLIMA_DUST_CLOSING_WIDE;
  Sound_Play(SE_SHIMMER);
  Actor_SetPosition(ACTOR_LEADER_PSYNERGY_FIELD, 253, 603);
  growFieldAction = gKolimaForestEntrance_PsynergyFieldGrowAction;
  Actor_SetScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldAction);
  Actor_SetPosition(ACTOR_IVAN_PSYNERGY_FIELD, 233, 629);
  Actor_SetScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldAction);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_SetPosition(ACTOR_MIA_PSYNERGY_FIELD, 207, 609);
    Actor_SetScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldAction);
  }
  Actor_SetPosition(ACTOR_GARET_PSYNERGY_FIELD, 227, 580);
  Actor_SetScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldAction);
  Event_Wait(120);
  gKolima_CurseDustState = KOLIMA_DUST_DRIFTING;
  do {
    wait(1);
  } while (gKolima_CurseDustState != KOLIMA_DUST_CENTRED);
  // "Hroom! Why? Why don't they turn into trees...?"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 80);
  // "Who are they?"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  // "What people are these, who are immune to Tret's glamour?"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  // "Have they come to save that village with their strange powers?"
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  // "It will not be enough to save the people of Kolima."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(40);
  // "They have earned their fate...Kolima took their axes to me,..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 40);
  // "You children may not know this...but we are dying."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 80);
  // "And Kolima Forest will wither and die when we are gone."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 40);
  // "We are not the only ones who will die, Laurel. Once we are..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  // "Tret was once a kindly forest king."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  // "But he is slowly dying, and his heart has torn in two..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
  // "His wrathful side turned the people of Kolima into trees."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  // "The kind Tret speaks no more."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
  // "Hroom! The kindly heart invites destruction... I need no..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_TRET, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(40);
  // "If you wish to save those whom Tret turned into trees, you..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
  // "But...if you cannot stop Tret from withering, we all will..."
  KolimaForestEntrance_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(60);
  // "Look for Tret deep within the forest."
  Event_ShowMessage(ACTOR_LAUREL, 0);
  // "It's no use, Laurel. Humans cannot be trusted! I shall die..."
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
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_IVAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
  // "Isaac... Isn't there anything we can do?"
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(10);
    // "What can we do? Do you have any idea?"
    Event_OpenMessage(ACTOR_GARET, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Event_Wait(40);
      KolimaForestEntrance_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
      // "It looks like Isaac has something in mind."
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 10);
      KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 20);
      KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 20);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
      // "We'll leave it up to you, Isaac. Let's go save everyone."
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
      // "He's as stumped as we are, but he says he'll help..."
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
      // "I trust Isaac. He'll come up with an idea."
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
    }
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  } else {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(10);
    Event_SetMessageId(MsgKolimaYoureIsaacThereIsntMuch);
    // "You're right, Isaac... There isn't much we can do now."
    KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 10);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    KolimaForestEntrance_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST,
                                          20);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    // "So the people of Kolima will just remain trees until they..."
    Event_OpenMessage(ACTOR_IVAN, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_Wait(20);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
      Event_Wait(40);
      KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
      // "I'm disappointed in you, Isaac... How can you give up so..."
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
      if (*miaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 10);
        Actor_StartRepeatedMotion(ACTOR_MIA, 3);
        // "That's right... We can't just leave them to rot!"
        KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 20);
      } else {
        gFieldState->message += 1;
      }
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Event_Wait(40);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      // "But how can we bring back the nice Tret and save the village?"
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Event_Wait(120);
      // "I guess I really don't know what to do."
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 40);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
        Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
        // "Ivan, you can't give up! Please, convince him to help!"
        KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 10);
      } else {
        gFieldState->message += 1;
      }
      Event_Wait(60);
      Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHWEST, 40);
        KolimaForestEntrance_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 20);
      }
      // "Just promise me this one thing..."
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 10);
      Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      Event_Wait(40);
      // "Promise you will do everything in your power to save Kolima."
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
      Event_Wait(20);
      Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    } else {
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Event_Wait(40);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
      Event_SetMessageId(MsgKolimaThatsReliefIsaacThoughtYoud);
      // "That's a relief, Isaac. I thought you'd given up on Kolima..."
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
      if (*miaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 10);
        Actor_StartRepeatedMotion(ACTOR_MIA, 1);
        // "I was beginning to lose faith in you, Isaac."
        KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 20);
      } else {
        gFieldState->message += 1;
      }
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Event_Wait(40);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      // "But what can we do? How can we bring back the good Tret and..."
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_GARET, 20);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Event_Wait(80);
      // "I really don't have any idea right now..."
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 40);
      if (gKolimaForestEntrance_MiaInParty != 0) {
        KolimaForestEntrance_FaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 20);
        Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
        // "I'm not sure... What should we do?"
        KolimaForestEntrance_ShowMessageAndWait(ACTOR_MIA, 40);
      } else {
        gFieldState->message += 1;
      }
      Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
      Event_Wait(20);
      // "I don't know just yet... But I'm sure we'll think of something."
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
      Actor_StartRepeatedMotion(ACTOR_GARET, 2);
      Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Event_Wait(40);
      // "Well, let's get on with it!"
      KolimaForestEntrance_ShowMessageAndWait(ACTOR_IVAN, 20);
    }
  }
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_GARET, 1.2);
  Actor_SetSpeed(ACTOR_IVAN, 1.2);
  Actor_SetSpeed(ACTOR_MIA, 1.2);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  {
    struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

    if (leader != NULL) {
      Actor_SetDestination(ACTOR_GARET, leader->x.part[0].integer,
                           leader->z.part[0].integer);
    }
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  {
    struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

    if (leader != NULL) {
      Actor_SetDestination(ACTOR_IVAN, leader->x.part[0].integer,
                           leader->z.part[0].integer);
    }
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  if (gKolimaForestEntrance_MiaInParty != 0) {
    Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
    {
      struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

      if (leader != NULL) {
        Actor_SetDestination(ACTOR_MIA, leader->x.part[0].integer,
                             leader->z.part[0].integer);
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

/// Shows the event's next message at speaker, then waits frames.
static void KolimaForestEntrance_ShowMessageAndWait(s32 speaker, s32 frames) {
  Event_ShowMessage(speaker, 0);
  Event_Wait(frames);
}

/// Turns an actor at once, then leaves time for the next beat.
static void KolimaForestEntrance_FaceActorAndWait(s32 actorId, s32 facing,
                                                  s32 waitFrames) {
  Actor_FaceDirection(actorId, facing, 0);
  Event_Wait(waitFrames);
}

#include "shared/kolima_grow.c"

/// Every eighth frame a sparkle of Tret's glamour (SPRITE_GOLDEN_RINGS) falls
/// toward the party, with its tinkle while the rain is at its height.
static void KolimaForestEntrance_SpawnCurseSparkle(void) {
  struct Actor *sparkle;
  struct Sprite *sprite;
  s32 framePhase;

  framePhase = gFrameCount & 7;
  if (framePhase != 0)
    return;
  if (gKolimaForestEntrance_SparkleSoundOn != 0)
    Sound_Play(SE_SPARKLE_FALL);
  sparkle =
      FieldActor_Create(SPRITE_GOLDEN_RINGS, toFIXED(231), 0, toFIXED(460));
  if (sparkle == NULL)
    return;
  sprite = sparkle->sprite;
  sprite->flags = framePhase;
  sparkle->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 1;
  sparkle->scaleX = FIXED_ONE / 10;
  sparkle->speedLimit = toFIXED(8);
  sparkle->acceleration = toFIXED(8);
  sparkle->motionFlags = framePhase;
  Actor_SelectAnimation(sparkle, 2);
  FieldActor_SetMoveTargetFX(sparkle, toFIXED(231), 0, toFIXED(624));
  Actor_StartScript(sparkle, gKolimaForestEntrance_SparkleScript);
}

#include "shared/kolima_blink.c"

/// The end of a sparkle's fall: one that lands on a field (the Psynergy fields'
/// area, which differs with Mia) bursts with a sound, and the Psynergy fields
/// are told.
static s32 KolimaForestEntrance_BurstOnPsynergyField(struct Actor *sparkle) {
  FIXED x;

  if (!(gKolimaForestEntrance_MiaInParty != 0
            ? (x = sparkle->x.fixed,
               (x > toFIXED(192) && x < toFIXED(274) &&
                sparkle->z.fixed > toFIXED(566) &&
                sparkle->z.fixed < toFIXED(612)) ||
                   (x > toFIXED(202) && x < toFIXED(255) &&
                    sparkle->z.fixed > toFIXED(549) &&
                    sparkle->z.fixed < toFIXED(632)))
            : (x = sparkle->x.fixed,
               (x > toFIXED(192) && x < toFIXED(244) &&
                sparkle->z.fixed > toFIXED(549) &&
                sparkle->z.fixed < toFIXED(585)) ||
                   (x > toFIXED(244) && x < toFIXED(274) &&
                    sparkle->z.fixed > toFIXED(571) &&
                    sparkle->z.fixed < toFIXED(605)) ||
                   (x > toFIXED(211) && x < toFIXED(255) &&
                    sparkle->z.fixed > toFIXED(596) &&
                    sparkle->z.fixed < toFIXED(632))))) {
    return 0;
  }
  Sound_Play(SE_DRIP);
  // UB: this routine returns void; call it through its declared type.
#ifdef UBFIX
  Actor_StartScript(sparkle, gKolima_SparkleBurstScript);
#else
  ((s32 (*)())Actor_StartScript)((s32)sparkle, (s32)gKolima_SparkleBurstScript);
#endif
  gKolimaForestEntrance_PsynergyFieldHit = 1;
  return 0;
}

#include "shared/kolima_curse_dust.c"
s32 gKolimaForestEntrance_PsynergyFieldHit BSS_DATA;
s32 gKolimaForestEntrance_MiaInParty BSS_DATA;
s32 gKolimaForestEntrance_SparkleSoundOn BSS_DATA;

/// Each frame the Psynergy fields flicker: two show on even frames and two on
/// odd, the hidden ones lifted 500 pixels out of sight.
static void KolimaForestEntrance_FlickerPsynergyFields(void) {
  struct Actor *p;

  p = Actor_Get(ACTOR_LEADER_PSYNERGY_FIELD);
  if (p != NULL) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = toFIXED(500);
    }
  }
  p = Actor_Get(ACTOR_IVAN_PSYNERGY_FIELD);
  if (p != NULL) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) != 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = toFIXED(500);
    }
  }
  p = Actor_Get(ACTOR_MIA_PSYNERGY_FIELD);
  if (p != NULL) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = toFIXED(500);
    }
  }
  p = Actor_Get(ACTOR_GARET_PSYNERGY_FIELD);
  if (p != NULL) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) != 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = toFIXED(500);
    }
  }
}

#include "shared/pushed_actor_tables.c"

/// Garet, Ivan and Mia walk to their places beside the leader.
s32 gKolimaForestEntrance_GaretAction[] = {
    SC_ANIMATION(2), SC_MOVE(234, 0, 576),
    SC_SYNC,         SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_ANIMATION(1), SC_END,
};

s32 gKolimaForestEntrance_IvanAction[] = {
    SC_ANIMATION(2), SC_MOVE(234, 0, 623),
    SC_SYNC,         SC_SET(SW_FACING, FACING_EAST),
    SC_ANIMATION(1), SC_END,
};

s32 gKolimaForestEntrance_MiaAction[] = {
    SC_ANIMATION(2), SC_MOVE(211, 0, 602),
    SC_SYNC,         SC_SET(SW_FACING, FACING_NORTH),
    SC_ANIMATION(1), SC_END,
};

#include "shared/kolima_squash.c"

/// A field grows from a tenth of its size over 70 frames.
s32 gKolimaForestEntrance_PsynergyFieldGrowAction[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE / 10),
    SC_SET(SW_SCALE_Y, FIXED_ONE / 10),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 50),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 50),
    SC_WAIT(1),
    SC_LOOP(70, SC_TO(1)),
    SC_CALL(KolimaForestEntrance_FlashHitPsynergyFields),
    SC_END,
};

/// A field sinks and shrinks away over 90 frames.
s32 gKolimaForestEntrance_PsynergyFieldShrinkAction[] = {
    SC_ANIMATION(1),
    SC_ADD(SW_Z, -toFIXED(1) / 10),
    SC_ADD(SW_SCALE_X, -3 * FIXED_ONE / 200),
    SC_ADD(SW_SCALE_Y, -3 * FIXED_ONE / 200),
    SC_WAIT(1),
    SC_LOOP(90, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestEntrance_GroundSparkleAction[] = {
    SC_SET(SW_SCALE_X, 3 * FIXED_ONE / 4),
    SC_SET(SW_SCALE_Y, 3 * FIXED_ONE / 4),
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
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        232, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        208, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        360, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TRET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LAUREL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        232, 0, 655, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        166, 0, 576, FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        189, 0, 532, FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        288, 0, 570, FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        331, 0, 659, FACING_SOUTH, TALK_FACING_PARTY,
        ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
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
    {TRIGGER_TABLE_END},
};

s32 gKolimaForestEntrance_SparkleScript[] = {
    SC_CALL(FieldEffect_StretchUntilArrived),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

#include "shared/kolima_sparkle_burst_script.c"

s32 gKolima_CurseDustScript[] = {
    SC_ADD(SW_Y, -toFIXED(2)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_ADD(SW_Y, -toFIXED(2)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_Y, -toFIXED(1)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(1)),
    SC_CALL(KolimaForestEntrance_BurstOnPsynergyField),
    SC_RELEASE,
};
