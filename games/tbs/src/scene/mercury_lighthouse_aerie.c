/// @file
/// Lighthouse Aerie (SceneId_MercuryLighthouseAerie), the top of
/// Mercury Lighthouse, where its beacon burns. Isaac and Garet, with Ivan and
/// Mia, have climbed the lighthouse after Saturos and Menardi, who hold Jenna
/// and Kraden and carry the Elemental Stars; they come too late to stop the
/// beacon being lit.
///
/// - The first arrival (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
///   MercuryLighthouseAerie_PartyArrives): the Adepts of the lit element
///   regain their PP, the aerie shakes, and in a flash the party stands at the
///   top. Touching metatile behavior 5
///   (MercuryLighthouseAerie_ConfrontSaturos), Garet sees `It's too late! The
///   lighthouse has already been lit!` Mia learns that the party came from Vale
///   to recover the Elemental Stars, and that Vale protected Sol Sanctum as her
///   clan protects the lighthouse; Jenna calls out, and Felix, Menardi and
///   Saturos face the party. Saturos sends Menardi on to the next lighthouse
///   with Jenna, Kraden and Felix, and fights the party alone
///   (ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2), the light of Mercury
///   weakening his Psynergy.
/// - Back from the battle (MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE,
///   MercuryLighthouseAerie_AlexAppears): `Isaac, we got him!` Alex, the
///   apprentice Mia lost, appears and explains that Saturos, aligned with
///   Mars, the clan of fire, had his power bound by the lighthouse's water,
///   which lent Mia, of the Mercury Clan like Alex, endless Psynergy; he was
///   only stalling until Saturos recovered. Saturos rises, Alex asks whether
///   the party still holds the Mars Star, and the two exit over to the lift
///   and ride it down. Mia means to go with the party, since with the
///   lighthouse lit the fountain will run over with healing water again
///   (FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED, and
///   FLAG_SATUROS_BEATEN_AT_MERCURY, which Imil, Bilibin and the lighthouse
///   test).
/// - Getting about. Six platforms dip under the leader, who jumps two cells
///   from one to the next (metatile behaviors 10 and 11;
///   places/mercury-lighthouse-aerie/events/MercuryLighthouseAerie_StepOntoPlatform.gif);
///   the lift leads down to
///   Mercury Lighthouse's SceneId_MercuryLighthouseExterior
///   (mercury_lighthouse.c), arriving at
///   MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE by the Fountain
///   of Healing: metatile behavior 6 calls it up, metatile behavior 12 rides it
///   down (MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR), and
///   arriving by it (MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR)
///   leaves it up
///   (places/mercury-lighthouse-aerie/events/MercuryLighthouseAerie_RideLiftDown.gif).
///   A Psynergy Stone can be found here.
///
/// Every arrival sets FLAG_MERCURY_LIGHTHOUSE_LIT, under which Mercury Adepts
/// regain PP faster. On the aerie Retreat is blocked, the beacon
/// flickers and sheds sparks by task
/// (MercuryLighthouseAerie_FlickerBeacon), and the platforms start 50 pixels
/// down. Jenna, Kraden, Felix and Menardi are placed only on the first arrival:
/// every other entrance sets FLAG_HOSTAGES_GONE before the actor
/// templates are read.
///
/// Unused: SceneId_UnusedMercuryLighthouseAerieCopy, which the area
/// names call Mercury Lighthouse and the debug scene list labels `MTO07NG`,
/// loads this overlay and the same map. The setup does nothing for it, and no
/// exit, table or code leads there; only the debug scene selector, which
/// reaches every scene, can.

#include "save_block.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/mercury_lighthouse_aerie.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/scene/common/actor_nearest.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "event_layer.h"
#include "engine_call.h"
#include "scene_tables.h"
#include "create_configured_actor.h"
#include "heap_slots.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "iwram_call.h"
#include "map_grid_scroll.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "scene.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "io_reg.h"
#include "ability_ids.h"

/// The aerie's actors. The party and Jenna stand in their own slots. The
/// speakers are known from the text: Kraden ("It's all right, Jenna."), Felix
/// ("Jenna, please... Just do as she says"), Saturos (Menardi: "Oh,
/// Saturos..."), Menardi (Saturos: "Menardi... Wait!") and Alex (Mia:
/// "Alex!"). Saturos is fought as ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2.
enum MercuryLighthouseAerieActorId {
  /// Flickers between two sizes and sheds sparks while in view
  /// (MercuryLighthouseAerie_FlickerBeacon).
  /// ★ Named from what the player sees: the swirling blue ball of light the
  /// lit beacon shows, spr_244.png (its render)
  ACTOR_BEACON = ACTOR_FIRST_PLACED,
  /// A statue, drawn as the lighthouse's statues all are, standing on
  /// the lift; it rides up and down with it.
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  ACTOR_LIFT_STATUE,
  /// Four more statues flank the beacon, two on each side of it;
  /// the two east of it, ACTOR_FIRST_BEACON_STATUE + 2 and + 3, are drawn
  /// mirrored.
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  ACTOR_FIRST_BEACON_STATUE,
  /// Six of them.
  /// ★ Named from what the player sees: round white platforms at the foot of
  /// the aerie's side ledges (its render)
  ACTOR_FIRST_PLATFORM = ACTOR_FIRST_PLACED + 6,
  /// The two platforms that stand on the lift and go up and down with it.
  /// ★ Named from what the player sees: round white platforms below the lift's
  /// statue (its render)
  ACTOR_FIRST_LIFT_PLATFORM = ACTOR_FIRST_PLATFORM + 3,
  /// ★ Named from what the player sees: round white platforms below the lift's
  /// statue (its render)
  ACTOR_SECOND_LIFT_PLATFORM,
  /// ★ Named from the game: "Kraden!" (MsgMercuryLighthouseAlreadyLit)
  ACTOR_KRADEN = ACTOR_FIRST_PLACED + 12,
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  ACTOR_FELIX,
  /// ★ Named from the game: "Oh, Saturos..." (MsgMercuryLighthouseAlreadyLit)
  ACTOR_SATUROS,
  /// ★ Named from the game: "Menardi... Wait!"
  /// (MsgMercuryLighthouseAlreadyLit)
  ACTOR_MENARDI,
  /// ★ Named from the game: "Alex!" (MsgMercuryGot)
  ACTOR_ALEX
};

/// Scene flags, kept while the party stays on the aerie.
enum {
  /// The leader stands on a platform, which has dipped under the weight.
  /// ★ Named from what the player sees: the round platforms floating in the
  /// sky beside the aerie, which bob under the leader (its render)
  FLAG_ON_PLATFORM = FLAG_SCENE_TEMP_50,
  /// ★ Named from what the player sees: the lift rising to the aerie with a
  /// statue on it (its render)
  FLAG_LIFT_UP = FLAG_SCENE_TEMP_51,
  /// Set on every arrival but the first: Jenna, Kraden, Felix and Menardi
  /// are placed only before it.
  /// ★ Named from the game: "leave our hostages"
  /// (MsgMercuryLighthouseAlreadyLit)
  FLAG_HOSTAGES_GONE = FLAG_SCENE_TEMP_53
};

enum {
  /// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
  FLAG_PSYNERGY_STONE = FLAG_AREA_TEMP_50
};

static void MercuryLighthouseAerie_PartyArrives(void);
static void MercuryLighthouseAerie_SettlePlatforms(void);
static void MercuryLighthouseAerie_AlexAppears(void);
static void MercuryLighthouseAerie_SaturosAndAlexWarpOut(void);
static void MercuryLighthouseAerie_SaturosAndAlexWarpIn(void);
static void MercuryLighthouseAerie_LowerLift(void);
static void MercuryLighthouseAerie_RaiseLift(void);
static void MercuryLighthouseAerie_FlickerBeacon(void);

#define NULL ((void *)0)

#include "common/effect_create.h"

#include "common/effect_spawn.h"
#include "actor_script.h"

const struct SceneEntrance *MercuryLighthouseAerie_GetEntrances(void) {
  return gMercuryLighthouseAerieEntrances;
}

const struct SceneExitRegion *MercuryLighthouseAerie_GetRegions(void) {
  return 0;
}

/// Returns the one exit: the lift down to SceneId_MercuryLighthouseExterior's
/// MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE, by the Fountain
/// of Healing.
const u32 *MercuryLighthouseAerie_GetExits(void) {
  return gMercuryLighthouseAerieExits;
}

/// Returns the actors; only the first arrival
/// (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) places Jenna, Kraden, Felix
/// and Menardi.
const struct ActorTemplate *MercuryLighthouseAerie_GetActorTemplates(void) {
  if (gSaveBlock.entrance != MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) {
    GameFlag_Set(FLAG_HOSTAGES_GONE);
  }
  return gMercuryLighthouseAerieActorTemplates;
}

const struct Trigger *MercuryLighthouseAerie_GetTriggers(void) {
  return gMercuryLighthouseAerieTriggers;
}

/// Sets up the Lighthouse Aerie: the beacon flickers and the six platforms
/// sit 50 pixels down; the first arrival
/// (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) plays the party's arrival, the
/// return from the battle with Saturos
/// (MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE) Alex's event, and
/// arriving by the lift
/// (MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR) leaves the lift
/// up.
s32 MercuryLighthouseAerie_Initialize(void) {
  u32 i;
  s32 actorId;
  struct Actor *actor;

  GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_LIT);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gSaveBlock.sceneId == (s32)&SceneId_MercuryLighthouseAerie) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    Task_AddFrame(MercuryLighthouseAerie_FlickerBeacon, TASK_PASS_FRAME);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpritePriority(ACTOR_GARET, 1);
    Actor_SetSpritePriority(ACTOR_IVAN, 1);
    Actor_SetSpritePriority(ACTOR_MIA, 1);
    Actor_SetSpritePriority(ACTOR_JENNA, 1);
    Actor_SetSpritePriority(ACTOR_KRADEN, 1);
    Actor_SetSpritePriority(ACTOR_FELIX, 1);
    Actor_SetSpritePriority(ACTOR_SATUROS, 1);
    Actor_SetSpritePriority(ACTOR_MENARDI, 1);
    Actor_SetSpritePriority(ACTOR_ALEX, 1);
    Actor_SetSpritePriority(ACTOR_BEACON, 1);
    Actor_SetSpritePriority(ACTOR_LIFT_STATUE, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_BEACON_STATUE, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_BEACON_STATUE + 1, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_BEACON_STATUE + 2, 1);
    Actor_SetSpritePriority(ACTOR_FIRST_BEACON_STATUE + 3, 1);
    for (i = ACTOR_FIRST_PLATFORM; i < ACTOR_FIRST_PLATFORM + 6; i++) {
      Actor_SetSpritePriority(i, 1);
      Actor_GetById(i)->motionFlags = 4;
      Actor_GetById(i)->priorityFlags |= 2;
      Actor_GetById(i)->y.fixed = -(PIXELS(50) + PIXELS(1) / 2);
    }
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        (actorId = Actor_FindNearestFloatingPlatform()) != 0 &&
        (actor = Actor_GetById(actorId)) != NULL) {
      actor->motionFlags = 0;
    }
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIFT_STATUE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_BEACON_STATUE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_BEACON_STATUE + 1), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_BEACON_STATUE + 2), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_BEACON_STATUE + 3), 0);
    Actor_GetById(ACTOR_FIRST_BEACON_STATUE + 2)->scaleX = -Q16_ONE;
    Actor_GetById(ACTOR_FIRST_BEACON_STATUE + 3)->scaleX = -Q16_ONE;
    if (gSaveBlock.entrance == MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        MercuryLighthouseAerie_PartyArrives();
      }
    } else if (gSaveBlock.entrance ==
               MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE) {
      if (!GameFlag_Get(FLAG_LIFT_UP)) {
        {
          struct MapGridLayerScroll *lift =
              &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];

          lift->offsetY = PIXELS(1024);
        }
        MapGrid_CenterOnFollowedPoint();
        WaitFrames(1);
        MapGrid_CopyCellsTo(4, 70, 4, 74, 5, 4);
        Import_ActorSetPosition(ACTOR_LIFT_STATUE, 0, 0);
        if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
          MercuryLighthouseAerie_AlexAppears();
        }
      }
    } else if (gSaveBlock.entrance ==
               MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR) {
      GameFlag_Set(FLAG_LIFT_UP);
    }
  }
  return 0;
}

/// Lighthouse Aerie, at MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT unless
/// returning after a battle: the Adepts of the lit element regain their PP;
/// the aerie shakes, the camera pans across and in a flash the party stands at
/// the top. Isaac shakes his head and the others gather on him.
static void MercuryLighthouseAerie_PartyArrives(void) {
  struct Actor *leader;

  Event_Start();
  Party_RestorePpOfLitElements();
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_GARET), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_IVAN), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_MIA), 0);
  FieldCamera_MoveTo(PIXELS(304), -1, PIXELS(120), 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Event_PlaySong(SE_RUMBLE);
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  Event_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  MapGrid_WaitForShakeToSettle();
  Event_WaitFrames(30);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(512), -PIXELS(24), PIXELS(160), 1);
  FieldCamera_WaitForArrival();
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_PALE, PALETTES_ALL);
  PaletteFade_StartInterpolation(50);
  Event_WaitFrames(50);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(30);
  Event_WaitFrames(30);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(504), PIXELS(168));
  Actor_SetPosition(ACTOR_GARET, PIXELS(528), PIXELS(144));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(488), PIXELS(144));
  Actor_SetPosition(ACTOR_MIA, PIXELS(512), PIXELS(152));
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_COLLAPSE);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_COLLAPSE);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_COLLAPSE);
  Event_WaitFrames(10);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(30);
  Event_WaitFrames(30);
  Event_WaitFrames(80);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_WaitFrames(60);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_GARET), SPRITE_FLAG_SHADOW);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_IVAN), SPRITE_FLAG_SHADOW);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Event_WaitFrames(40);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_MIA), SPRITE_FLAG_SHADOW);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0)
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0)
    Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel,
                         leader->z.part.pixel);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0)
    Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Event_End();
}

#include "common/actor_distance.h"
#include "common/actor_nearest.h"

/// Lighthouse Aerie: walking into metatile behavior 10 or 11. If nothing is in
/// the way the leader jumps two cells ahead, off any platform and onto the
/// next.
static void MercuryLighthouseAerie_JumpToPlatform(void) {
  struct Actor *actor;
  struct FieldPosition position;
  s32 angle;
  u8 flags;

  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  angle = (actor->facing + 0x1000) & 0xe000;
  flags = actor->motionFlags;
  SET_POSITION(&position, (actor->x.fixed & 0xfff00000) + PIXELS(8),
               actor->y.fixed, (actor->z.fixed & 0xfff00000) + PIXELS(8));
  Vector_AddPolarOffset(PIXELS(32), angle, &position);
  if (FieldActor_CheckMovementCollision(actor, &position) == 0) {
    GameFlag_Clear(FLAG_ON_PLATFORM);
    MercuryLighthouseAerie_SettlePlatforms();
    Actor_SelectAnimation(actor, 6);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    Actor_SelectAnimation(actor, 7);
    actor->speedLimit = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    actor->motionFlags &= 0x7e;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER, position.x >> 16,
                               position.z >> 16);
    Actor_SelectAnimation(actor, 6);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    actor->motionFlags = flags;
  }
}

/// Lighthouse Aerie: touching metatile behavior 10 when not already on a
/// platform. The platform underfoot dips with the leader and bobs back up.
static void MercuryLighthouseAerie_StepOntoPlatform(void) {
  struct Actor *leader;
  struct Actor *actor;
  s32 id;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  Event_Start();
  id = Actor_FindNearestFloatingPlatform();
  gMercuryLighthouseAerie_PlatformUnderfoot = id;
  if (id != 0) {
    GameFlag_Set(FLAG_ON_PLATFORM);
    actor = Actor_GetById(gMercuryLighthouseAerie_PlatformUnderfoot);
    actor->motionFlags = 0;
    leader->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
    actor->y.fixed -= PIXELS(3);
    leader->y.fixed -= PIXELS(3);
    leader->groundHeight -= PIXELS(3);
    WaitFrames(2);
    actor->y.fixed -= PIXELS(2);
    leader->y.fixed -= PIXELS(2);
    leader->groundHeight -= PIXELS(2);
    WaitFrames(10);
    actor->y.fixed += PIXELS(2);
    leader->y.fixed += PIXELS(2);
    leader->groundHeight += PIXELS(2);
    WaitFrames(4);
    actor->y.fixed += PIXELS(2);
    leader->y.fixed += PIXELS(2);
    leader->groundHeight += PIXELS(2);
    WaitFrames(4);
    actor->y.fixed += PIXELS(1);
    leader->y.fixed += PIXELS(1);
    leader->groundHeight += PIXELS(1);
  }
  Event_End();
}

/// Lighthouse Aerie: touching metatile behavior 11, off the platforms. The
/// leader walks normally again and the platforms stop following.
static void MercuryLighthouseAerie_SettlePlatforms(void) {
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 3;
  Actor_GetById(ACTOR_FIRST_PLATFORM)->motionFlags = 4;
  Actor_GetById(ACTOR_FIRST_PLATFORM + 1)->motionFlags = 4;
  Actor_GetById(ACTOR_FIRST_PLATFORM + 2)->motionFlags = 4;
  Actor_GetById(ACTOR_FIRST_LIFT_PLATFORM)->motionFlags = 4;
  Actor_GetById(ACTOR_SECOND_LIFT_PLATFORM)->motionFlags = 4;
  Actor_GetById(ACTOR_FIRST_PLATFORM + 5)->motionFlags = 4;
}

/// Lighthouse Aerie: touching metatile behavior 5 before the confrontation.
/// Garet sees the lighthouse is lit: "It's too late! The lighthouse has already
/// been lit!" Mia learns the party is from Vale; Jenna calls out, and Felix,
/// Menardi and Saturos face the party. Menardi leaves with Jenna, Kraden and
/// Felix for the next lighthouse, and Saturos fights alone
/// (ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2).
static void MercuryLighthouseAerie_ConfrontSaturos(void) {
  struct Actor *actor;

  Event_Start();
  Actor_Get(ACTOR_FIRST_LIFT_PLATFORM)->motionFlags &=
      ~(ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }
  WaitFrames(1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 344, 232);
  Actor_WalkTo(ACTOR_GARET, 328, 232);
  Actor_WalkTo(ACTOR_IVAN, 344, 248);
  Actor_WalkTo(ACTOR_MIA, 328, 248);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_WaitFrames(50);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
  FieldCamera_MoveTo(PIXELS(328), PIXELS(40), PIXELS(176), 1);
  FieldCamera_GetActor()->motionFlags = 0;
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkToAndWait(ACTOR_GARET, 328, 216);
  FieldCamera_WaitForArrival();
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Event_WaitFrames(60);
  FieldCamera_MoveTo(PIXELS(344), PIXELS(24), PIXELS(232), 1);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_SetMessageId(MsgMercuryLighthouseAlreadyLit);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(60);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkToAndWait(ACTOR_MIA, 328, 232);
  Event_WaitFrames(10);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 30);
  Actor_LinkPair(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(60);
  Actor_WalkToAndWait(ACTOR_IVAN, 336, 248);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Event_WaitFrames(30);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 30);
  FieldMessage_ShowAtActor(ACTOR_MIA, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  }
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Event_WaitFrames(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 10);
  Import_ActorSetAnimation(ACTOR_MIA, 0x10);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 60);
  // Jenna calls out: "Isaac! Garet!"
  Event_PlaySong(SONG_FADE_OUT_BGM);
  Actor_SetPosition(ACTOR_JENNA, PIXELS(216), PIXELS(200));
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_SetPosition(ACTOR_JENNA, PIXELS(120), PIXELS(160));
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 5);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 5);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 5);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  FieldCamera_MoveTo(PIXELS(120), -PIXELS(24), PIXELS(168), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_WaitFrames(20);
  Event_PlaySong(MUS_SATUROS_AND_MENARDI);
  FieldEvent_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH, 60);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetPosition(ACTOR_GARET, PIXELS(264), PIXELS(288));
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetPosition(ACTOR_GARET, PIXELS(328), PIXELS(216));
  Actor_SetAnimationAndWait(ACTOR_MENARDI, 4);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 10);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Actor_SetAnimationAndWait(ACTOR_FELIX, 4);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, 3);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
  Actor_SetSpeed(ACTOR_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_FELIX, 104, 168);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(264), PIXELS(288));
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(296), PIXELS(120));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldCamera_SetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
  FieldCamera_MoveTo(PIXELS(232), PIXELS(40), PIXELS(152), 1);
  Actor_SetSpeed(ACTOR_SATUROS, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_SATUROS, 272, 128);
  Actor_WalkToAndWait(ACTOR_SATUROS, 264, 152);
  Actor_WalkToAndWait(ACTOR_SATUROS, 280, 168);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  FieldCamera_MoveTo(PIXELS(296), PIXELS(40), PIXELS(216), 1);
  Actor_WalkToAndWait(ACTOR_SATUROS, 304, 176);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST, 0);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_WaitFrames(20);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 70);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(40);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_MENARDI, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_WaitFrames(20);
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_GARET, 328, 224);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 344, 224);
  Actor_WalkTo(ACTOR_IVAN, 344, 232);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(168), PIXELS(200));
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetPosition(ACTOR_MENARDI, PIXELS(104), PIXELS(200));
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  FieldCamera_MoveTo(PIXELS(120), -PIXELS(24), PIXELS(168), 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTH + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 70);
  Actor_LinkPair(ACTOR_JENNA, ACTOR_KRADEN, 0);
  Event_WaitFrames(50);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 1);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MENARDI, 4);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(60);
  Actor_SetAnimationAndWait(ACTOR_FELIX, 4);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(60);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 40);
  Actor_SetAnimationAndWait(ACTOR_FELIX, 3);
  Event_WaitFrames(60);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 30);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_WalkTo(ACTOR_JENNA, 128, 144);
  Actor_WalkToAndWait(ACTOR_KRADEN, 120, 136);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_FELIX, 88, 152);
  Actor_Get(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_MENARDI, FACING_NORTHWEST + FACING_STEP, 20);
  Import_ActorSetAnimation(ACTOR_MENARDI, 3);
  Actor_SetAnimationAndWait(ACTOR_FELIX, 3);
  Event_WaitFrames(40);
  Actor_SetSpeed(ACTOR_MENARDI, PIXELS(3), PIXELS(2));
  Actor_Get(ACTOR_MENARDI)->velocityY = PIXELS(4);
  Event_PlaySong(SE_JUMP);
  Actor_Get(ACTOR_MENARDI)->motionFlags &= 0x7e;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_MENARDI), 0);
  Actor_GetById(ACTOR_FIRST_LIFT_PLATFORM)->motionFlags = 4;
  Actor_SetPositionAndCommit(ACTOR_MENARDI, 104, 168);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_MENARDI), SPRITE_FLAG_SHADOW);
  Actor_Get(ACTOR_MENARDI)->motionFlags = 3;
  Actor_FaceDirection(ACTOR_MENARDI, FACING_EAST, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_FELIX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_MENARDI, 0, 20);
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_LowerLift();
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
  FieldCamera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(216), 1);
  FieldCamera_WaitForArrival();
  Actor_SetPosition(ACTOR_KRADEN, PIXELS(272), PIXELS(280));
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkToAndWait(ACTOR_GARET, 312, 216);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_WalkToAndWait(ACTOR_SATUROS, 312, 184);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(2), PIXELS(1));
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_GARET, 328, 224);
  Event_WaitFrames(1);
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_WaitFrames(10);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_WaitFrames(10);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldMessage_ShowAtActor(ACTOR_SATUROS, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Event_WaitFrames(20);
    gFieldWork->message++;
    FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  }
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_WalkToAndWait(ACTOR_SATUROS, 328, 200);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_Get(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_SATUROS, 336, 208);
  Event_WaitFrames(1);
  Actor_GetById(ACTOR_SATUROS)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_ShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_WalkToAndWait(ACTOR_SATUROS, 336, 216);
  Event_SetAfterBattleDestination((s32)&SceneId_MercuryLighthouseAerie, 2);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  // Saturos: ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2.
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY,
                                        2);
  Event_End();
}

/// Lighthouse Aerie, at MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE, back
/// from the battle, unless returning after another: Saturos is beaten, but not
/// for long. "Isaac, we got him!" Alex appears and explains that the
/// lighthouse bound Saturos' fire Psynergy and lent Mia endless power. Saturos
/// stands again; the two exit over to the lift and ride it down, and the party
/// regroups on the leader. Sets FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED and
/// FLAG_SATUROS_BEATEN_AT_MERCURY.
static void MercuryLighthouseAerie_AlexAppears(void) {

  Event_Start();
  Actor_Get(ACTOR_LIFT_STATUE)->motionFlags = 0;
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(344), PIXELS(224));
  Actor_SetPosition(ACTOR_GARET, PIXELS(328), PIXELS(224));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(344), PIXELS(232));
  Actor_SetPosition(ACTOR_MIA, PIXELS(328), PIXELS(232));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(336), PIXELS(176));
  Import_ActorSetAnimation(ACTOR_SATUROS, 9);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SATUROS), 0);
  FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(208), 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
  Event_SetMessageId(MsgMercuryGot);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 30);
  Actor_SetPosition(ACTOR_ALEX, PIXELS(312), PIXELS(112));
  Event_PlaySong(SE_STOP_ACTION_SE);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 30);
  Event_PlaySong(MUS_CONFRONTATION);
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(168), 1);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_ALEX, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_ALEX, 344, 136);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH + FACING_STEP, 20);
  FieldCamera_WaitForArrival();
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 30);
  FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(184), 1);
  Actor_WalkToAndWait(ACTOR_ALEX, 344, 160);
  Actor_WalkToAndWait(ACTOR_ALEX, 328, 168);
  Actor_WalkToAndWait(ACTOR_ALEX, 312, 176);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_ALEX, 2);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_WalkToAndWait(ACTOR_ALEX, 312, 184);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
  FieldMessage_ShowAtActor(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  }
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST + FACING_STEP, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_SATUROS, 8);
  Event_WaitFrames(45);
  Import_ActorSetAnimation(ACTOR_SATUROS, 1);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SATUROS), SPRITE_FLAG_SHADOW);
  Event_WaitFrames(40);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_SATUROS, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_LinkPair(ACTOR_ALEX, ACTOR_SATUROS, 0);
  Event_WaitFrames(35);
  Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  Event_WaitFrames(20);
  Actor_WalkToAndWait(ACTOR_ALEX, 328, 176);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  Import_ActorSetAnimation(ACTOR_ALEX, 5);
  Import_ActorSetAnimation(ACTOR_SATUROS, 7);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldMessage_ShowAtActor(ACTOR_ALEX, 0);
  Event_WaitFrames(30);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_WaitFrames(20);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_WaitFrames(30);
    Event_ShowMessage(ACTOR_ALEX, 0);
    gFieldWork->message++;
  } else {
    Event_WaitFrames(30);

    gFieldWork->message++;
    Event_ShowMessage(ACTOR_ALEX, 0);
  }
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  MercuryLighthouseAerie_SaturosAndAlexWarpOut();
  Event_WaitFrames(20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 5);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 5);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 5);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 5);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 5);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 5);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 5);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 5);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(232), 1);
  FieldCamera_WaitForArrival();
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_HIDDEN);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(240), PIXELS(208));
  Actor_SetPosition(ACTOR_ALEX, PIXELS(232), PIXELS(208));
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 0);
  MercuryLighthouseAerie_SaturosAndAlexWarpIn();
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 15);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 328, 208);
  Actor_WalkTo(ACTOR_IVAN, 336, 224);
  Actor_WalkToAndWait(ACTOR_GARET, 312, 216);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  {
    struct MapGridLayerScroll *lift;
    void **cells;
    FieldCamera_MoveTo(PIXELS(152), -1, PIXELS(216), 1);
    lift = &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];
    lift->offsetY = PIXELS(896);
    cells = gMapGridWork;
    MapGrid_CenterOnFollowedPoint();
    WaitFrames(1);
    Actor_Get(ACTOR_LIFT_STATUE)->motionFlags = 0;
    Actor_SetPosition(ACTOR_LIFT_STATUE, PIXELS(104), PIXELS(264));
    Actor_Get(ACTOR_LIFT_STATUE)->y.fixed = -PIXELS(32);
    Actor_GetById(ACTOR_LIFT_STATUE)->targetY = -PIXELS(32);
    MapGrid_CopyCellsTo(0x1D, 0x4A, 4, 0x4A, 5, 4);
    Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
    Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
    MercuryLighthouseAerie_RaiseLift();
    Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
    Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
    FieldCamera_WaitForArrival();
    Event_WaitFrames(30);
    Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 20);
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(216), 1);
    FieldCamera_WaitForArrival();
    Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
    Import_ActorSetAnimation(ACTOR_ALEX, 5);
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
    FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    FieldMessage_ShowAtActor(ACTOR_ALEX, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_WaitFrames(20);
      Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
      Event_WaitFrames(20);
      FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
      ((struct FieldWork *)cells[19])->message++;
    } else {
      ((struct FieldWork *)cells[19])->message++;
      Event_WaitFrames(20);
      FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    }
  }
  Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldMessage_ShowAtActor(ACTOR_ALEX, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 1);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  }
  Event_WaitFrames(20);
  MercuryLighthouseAerie_SaturosAndAlexWarpOut();
  Event_WaitFrames(20);
  FieldCamera_MoveTo(PIXELS(128), -1, PIXELS(200), 1);
  FieldCamera_WaitForArrival();
  Import_ActorSetAnimation(ACTOR_SATUROS, 1);
  Import_ActorSetAnimation(ACTOR_ALEX, 1);
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_HIDDEN);
  Actor_SetPosition(ACTOR_SATUROS, PIXELS(120), PIXELS(152));
  Actor_SetPosition(ACTOR_ALEX, PIXELS(112), PIXELS(160));
  Actor_FaceDirection(ACTOR_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 0);
  MercuryLighthouseAerie_SaturosAndAlexWarpIn();
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 20);
  FieldMessage_ShowAtActor(ACTOR_ALEX, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  }
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_SATUROS, 1);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_SATUROS, FACING_EAST, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_SATUROS, 0, 20);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 20);
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_LowerLift();
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
  Event_PlaySong(SONG_FADE_OUT_BGM);
  FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(216), 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Scene_PlayMusic();
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 22);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(10);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Import_ActorSetAnimation(ACTOR_MIA, 0x10);
  Actor_Get(ACTOR_MIA)->scaleX = -Q16_ONE;
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldMessage_ShowAtActor(ACTOR_IVAN, 0);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_Get(ACTOR_MIA)->scaleX = Q16_ONE;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  }
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 30);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_WaitFrames(10);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_MIA, 328, 216);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_GARET, 312, 200);
  Actor_WalkTo(ACTOR_MIA, 280, 216);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(60);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 30);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 40);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 312, 216);
  Actor_WalkTo(ACTOR_GARET, 312, 216);
  Actor_WalkTo(ACTOR_IVAN, 312, 216);
  Actor_WalkTo(ACTOR_MIA, 312, 216);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  {
    struct MapGridLayerScroll *lift =
        &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];

    lift->offsetY = PIXELS(1024);
  }
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  MapGrid_CopyCellsTo(4, 0x46, 4, 0x4A, 5, 4);
  GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED);
  GameFlag_Set(FLAG_SATUROS_BEATEN_AT_MERCURY);
  Event_End();
}

/// Lighthouse Aerie: touching metatile behavior 6 while the lift is down. The
/// camera watches it rise with its statue.
static void MercuryLighthouseAerie_CallLift(void) {
  struct MapGridLayerScroll *lift;

  lift = &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];
  Event_Start();
  lift->offsetY = PIXELS(896);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Actor_GetById(ACTOR_LIFT_STATUE)->motionFlags = 0;
  Import_ActorSetPosition(ACTOR_LIFT_STATUE, PIXELS(104), PIXELS(264));
  Actor_GetById(ACTOR_LIFT_STATUE)->y.fixed = -PIXELS(32);
  Actor_GetById(ACTOR_LIFT_STATUE)->targetY = -PIXELS(32);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeedParameters(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveWithinMapBounds(PIXELS(128), -1, PIXELS(184), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(30);
  MapGrid_CopyCellsTo(29, 74, 4, 74, 5, 4);
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_RaiseLift();
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
  Event_WaitFrames(20);
  GameFlag_Set(FLAG_LIFT_UP);
  Event_End();
}

/// Lighthouse Aerie: touching metatile behavior 12. The leader steps onto the
/// lift and rides it down to SceneId_MercuryLighthouseExterior, arriving at
/// MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE by the Fountain of
/// Healing.
static void MercuryLighthouseAerie_RideLiftDown(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 152);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 60);
  Actor_SetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
  Actor_SetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_LowerLift();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Import_EventSetExitRequest(
      MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR);
  Event_End();
}

/// Saturos and Alex exit out in a column of sparkles and vanish.
static void MercuryLighthouseAerie_SaturosAndAlexWarpOut(void) {
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;
  struct Actor *left;
  struct Actor *right;
  u32 i;
  s32 x;
  s32 r;

  left = Actor_GetById(ACTOR_SATUROS);
  right = Actor_GetById(ACTOR_ALEX);
  Sound_PlaySong(SE_WARP);
  Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_BLINK);
  Actor_ConfigureSpriteColor(ACTOR_ALEX, SPRITE_COLOR_BLINK);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SATUROS), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ALEX), 0);
  options = &optionsStorage;
  options->priority = 1;
  options->palette = 5;
  options->type = SPRITE_PARTICLE;
  options->startScaleX = 2 * Q16_ONE / 5;
  options->startScaleY = 3 * Q16_ONE;
  for (i = 0; i <= 31; i++) {
    Event_WaitFrames(1);
    if (i & 1) {
      r = ((((u32)Import_RandomNext() * 24) >> 16) << 16);
      x = left->x.fixed;
      x += r;
      x += -PIXELS(12);
      FieldEffect_Spawn(x,
                        left->y.fixed +
                            ((((u32)Import_RandomNext() << 5) >> 16) << 16) +
                            -PIXELS(16),
                        left->z.fixed, 0, PIXELS(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        options);
    } else {
      r = ((((u32)Import_RandomNext() * 24) >> 16) << 16);
      x = right->x.fixed;
      x += r;
      x += -PIXELS(12);
      FieldEffect_Spawn(x,
                        right->y.fixed +
                            ((((u32)Import_RandomNext() << 5) >> 16) << 16) +
                            -PIXELS(16),
                        right->z.fixed, 0, PIXELS(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        options);
    }
  }
  Import_ActorSetPosition(ACTOR_SATUROS, 0, 0);
  Import_ActorSetPosition(ACTOR_ALEX, 0, 0);
}

/// Saturos and Alex exit in where they have been placed.
static void MercuryLighthouseAerie_SaturosAndAlexWarpIn(void) {
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;
  struct Actor *left;
  struct Actor *right;
  u32 i;
  s32 x;
  s32 r;

  left = Actor_GetById(ACTOR_SATUROS);
  right = Actor_GetById(ACTOR_ALEX);
  Sound_PlaySong(SE_WARP);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SATUROS), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ALEX), 0);
  options = &optionsStorage;
  options->priority = 1;
  options->palette = 5;
  options->type = SPRITE_PARTICLE;
  options->startScaleX = 2 * Q16_ONE / 5;
  options->startScaleY = 3 * Q16_ONE;
  for (i = 0; i <= 31; i++) {
    Event_WaitFrames(1);
    if (i & 1) {
      r = ((((u32)Import_RandomNext() * 24) >> 16) << 16);
      x = left->x.fixed;
      x += r;
      x += -PIXELS(12);
      FieldEffect_Spawn(x,
                        left->y.fixed +
                            ((((u32)Import_RandomNext() << 5) >> 16) << 16) +
                            PIXELS(32),
                        left->z.fixed, 0, -PIXELS(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        options);
    } else {
      r = ((((u32)Import_RandomNext() * 24) >> 16) << 16);
      x = right->x.fixed;
      x += r;
      x += -PIXELS(12);
      FieldEffect_Spawn(x,
                        right->y.fixed +
                            ((((u32)Import_RandomNext() << 5) >> 16) << 16) +
                            PIXELS(32),
                        right->z.fixed, 0, -PIXELS(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        options);
    }
    if (i == 20) {
      Actor_SetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_BLINK);
      Actor_ConfigureSpriteColor(ACTOR_ALEX, SPRITE_COLOR_BLINK);
    }
  }
  Actor_ConfigureSpriteColor(ACTOR_SATUROS, SPRITE_COLOR_NORMAL);
  Actor_ConfigureSpriteColor(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SATUROS), SPRITE_FLAG_SHADOW);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
}

/// MercuryLighthouseAerie_LowerLift passes its lift as a third
/// argument, which this function does not take.
static void MercuryLighthouseAerie_FindLiftRiders(s32 *out, s32 y) {
  struct FieldWork *work;
  u32 slot;

  work = gFieldWork;
  y = 64 - (y >> 20);
  for (slot = 0; slot < sizeof(work->actors) / sizeof(work->actors[0]);
       slot++) {
    struct Actor *actor = work->actors[slot];

    if (actor != 0) {
      s32 cell = actor->x.fixed >> 20;
      s32 row = actor->z.fixed >> 20;

      if (cell >= 4 && cell <= 8 && y + 8 <= row && row < y + 11)
        *out++ = slot;
    }
  }
}

/// The lift sinks with everything standing on it, speeding up.
static void MercuryLighthouseAerie_LowerLift(void) {
  s32 ids[5];
  struct MapGridLayerScroll *lift;
  u32 i;
  u32 j;
  u32 n;
  s32 speed;
  struct Actor *actor;

  lift = &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];
  speed = 0x1999;
  n = 0;
  for (i = 0; i <= 4; i++)
    ids[i] = 66;
  // the lift goes as a third argument, as the bytes show: the cast keeps it
  ((void (*)())MercuryLighthouseAerie_FindLiftRiders)(ids, lift->offsetY, lift);
  for (i = 0; i <= 4; i++) {
    if (ids[i] == 66)
      break;
    Actor_GetById(ids[i])->motionFlags = 0;
    n++;
  }
  Sound_PlaySong(SE_LIFT);
  for (i = 0; i <= 227; i++) {
    lift->offsetY -= speed;
    for (j = 0; j < n; j++) {
      Actor_GetById(ids[j])->z.fixed += speed;
      actor = Actor_GetById(ids[j]);
      actor->targetZ = Actor_GetById(ids[j])->z.fixed;
    }
    if ((i & 3) == 3)
      speed += 0x1999;
    if (speed > 0x17fff)
      speed = 0x18000;
    WaitFrames(1);
  }
  for (i = 0; i < n; i++)
    Actor_GetById(ids[i])->motionFlags = 0;
}

/// The lift rises with everything standing on it, slowing at the top.
static void MercuryLighthouseAerie_RaiseLift(void) {
  s32 ids[5];
  struct MapGridLayerScroll *lift;
  u32 i;
  u32 j;
  u32 n;
  s32 speed;
  struct Actor *actor;

  lift = &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];
  speed = 0x18000;
  n = 0;
  for (i = 0; i <= 4; i++)
    ids[i] = 66;
  MercuryLighthouseAerie_FindLiftRiders(ids, lift->offsetY);
  for (i = 0; i <= 4; i++) {
    if (ids[i] == 66)
      break;
    Actor_GetById(ids[i])->motionFlags = 0;
    n++;
  }
  Sound_PlaySong(SE_LIFT);
  for (i = 0; i <= 85; i++) {
    lift->offsetY += speed;
    for (j = 0; j < n; j++) {
      Actor_GetById(ids[j])->z.fixed -= speed;
      actor = Actor_GetById(ids[j]);
      actor->targetZ = Actor_GetById(ids[j])->z.fixed;
    }
    if ((i & 3) == 3 && i > 75)
      speed += -0x3333;
    if (speed < 0xccc)
      speed = 0xccc;
    WaitFrames(1);
  }
  lift->offsetY = PIXELS(1024);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(2);
}

/// Unused: Alex raises his hands and casts Ply on the lift's statue.
///
/// Nothing calls it. In the game, Alex and Saturos exit over to the lift and
/// ride it down without it (MercuryLighthouseAerie_AlexAppears).
UNUSED static void MercuryLighthouseAerie_UnusedAlexCastsPly(void) {
  // XXX (no evidence): what this was for; nothing calls it, so neither the
  // ROM nor a render shows it in an event.
  FieldPsynergy_LoadResources(ABILITY_PLY, 1);
  FieldPsynergy_SetupActorPair(ACTOR_ALEX, ACTOR_LIFT_STATUE);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  FieldPsynergy_StopCasterUpdate();
  FieldPsynergy_End();
}

/// A beacon spark's update: it swings round the beacon it is linked to, its
/// angle (its timer, in 1/65536 turns) stepping back each frame; its speed
/// holds the swing's radius in pixels, less 28.
static void MercuryLighthouseAerie_UpdateBeaconSpark(struct Actor *actor) {
  struct Actor *spark = actor;
  struct Actor *beacon;
  u16 angle;

  beacon = spark->linkedActor;
  angle = spark->timer;
  spark->x.fixed = beacon->x.fixed + Trig_Cos(angle) * (spark->speedLimit + 28);
  spark->z.fixed = (Trig_Sin(angle) << 4) + PIXELS(144);
  spark->targetX = spark->x.fixed;
  spark->targetZ = spark->z.fixed;
  spark->timer -= 0x200;
}

/// Every frame: while the view is near the top the beacon flickers
/// between two sizes, and every 16 frames it throws off a spark that swings
/// round it.
static void MercuryLighthouseAerie_FlickerBeacon(void) {
  struct Actor *beacon;
  struct Actor *spark;
  struct Sprite *sprite;
  struct MapGridScrollWork *grid;
  s32 height;
  s32 scale;
  s32 tick;
  u32 angle;

  beacon = Actor_GetById(ACTOR_BEACON);
  grid = gMapGridWork[0];
  height = ((u32)(Import_RandomNext() * 48) >> 16) << 16;
  if (((union FieldCoordinate *)&grid->viewY)->part.pixel <= 129) {
    if (gFrameCount & 1) {
      Import_ActorSetPosition(ACTOR_BEACON, PIXELS(304), PIXELS(144));
      scale = Q16_ONE;
      Actor_GetById(ACTOR_BEACON)->scaleX = scale;
    } else {
      Import_ActorSetPosition(ACTOR_BEACON, PIXELS(304), PIXELS(151));
      scale = 13 * Q16_ONE / 10;
      Actor_GetById(ACTOR_BEACON)->scaleX = scale;
    }
    Actor_GetById(ACTOR_BEACON)->scaleY = scale;
  } else {
    Actor_SetPosition(ACTOR_BEACON, PIXELS(8), PIXELS(8));
  }
  if (beacon == NULL) {
    return;
  }
  tick = gFrameCount & 15;
  if (tick != 0) {
    return;
  }
  spark =
      FieldActor_Create(SPRITE_PARTICLE, beacon->x.fixed + PIXELS(8),
                        beacon->y.fixed + height + PIXELS(8), beacon->z.fixed);
  height = height / PIXELS(6);
  height <<= 16;
  if (spark == NULL) {
    return;
  }
  sprite = spark->sprite;
  Actor_StartScript(spark, gMercuryLighthouseAerie_SparkScript);
  Actor_SetSpriteColor(spark, SPRITE_COLOR_BGR);
  spark->motionFlags = tick;
  angle = Import_RandomNext() & 0x0ffff000;
  spark->timer = angle;
  spark->userValue = tick;
  spark->linkedActor = beacon;
  spark->update = MercuryLighthouseAerie_UpdateBeaconSpark;
  spark->speedLimit = (Trig_Sin((height & 0xfffff) >> 4) * 24) >> 16;
  sprite->flags = 0;
  sprite->image.priority = beacon->sprite->image.priority;
}

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gMercuryLighthouseAerieEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT, CONDITION_ALWAYS, 8, 0, 8,
     FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE, CONDITION_ALWAYS, 8, 0, 8,
     FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR,
     CONDITION_ALWAYS, 504, 0, 152, FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {51, CONDITION_ALWAYS, 504, 0, 168, FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {SCENE_TABLE_END},
};

u32 gMercuryLighthouseAerieExits[] = {
    SCENE_EXITS(MercuryLighthouseAerie),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR,
               MercuryLighthouseExterior,
               MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorTemplate gMercuryLighthouseAerieActorTemplates[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTHWEST, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTHEAST, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { ACTOR_JENNA, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(160), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(304), PIXELS(40), PIXELS(144), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(440), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(472), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(504), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(104), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(168), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_KRADEN, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(152), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_FELIX, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(160), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MENARDI, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(200), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gMercuryLighthouseAerieTriggers[] = {
    {TRIGGER_STEP, 5, FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED,
     EVENT(MercuryLighthouseAerie_ConfrontSaturos)},
    {TRIGGER_STEP, 6, FLAG_LIFT_UP, EVENT(MercuryLighthouseAerie_CallLift)},
    {STEP_TRIGGER_WALKING, 11, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_JumpToPlatform)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_SettlePlatforms)},
    {STEP_TRIGGER_WALKING, 10, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_JumpToPlatform)},
    {TRIGGER_STEP, 10, FLAG_ON_PLATFORM,
     EVENT(MercuryLighthouseAerie_StepOntoPlatform)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 100, FLAG_PSYNERGY_STONE,
     FIND_PSYNERGY_STONE},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS,
     EVENT(MercuryLighthouseAerie_RideLiftDown)},
    {TRIGGERS_END},
};

s32 gMercuryLighthouseAerie_PlatformUnderfoot = 0;

s32 gMercuryLighthouseAerie_SparkScript[] = {
    SC_SET(SW_SCALE_Y, 7 * Q16_ONE / 5),
    SC_SET(SW_SCALE_X, 7 * Q16_ONE / 5),
    SC_ANIMATION(4),
    SC_WAIT(30),
    SC_RELEASE,
};
