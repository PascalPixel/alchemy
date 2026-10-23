/// @file
/// Lighthouse Aerie (MapId_MercuryLighthouseAerie), the top of
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
///   the party still holds the Mars Star, and the two warp over to the lift
///   and ride it down. Mia means to go with the party, since with the
///   lighthouse lit the fountain will run over with healing water again
///   (FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED, and
///   FLAG_SATUROS_BEATEN_AT_MERCURY, which Imil, Bilibin and the lighthouse
///   test).
/// - Getting about. Six platforms dip under the leader, who jumps two cells
///   from one to the next (metatile behaviors 10 and 11;
///   places/mercury-lighthouse-aerie/events/MercuryLighthouseAerie_StepOntoPlatform.gif);
///   the lift leads down to
///   Mercury Lighthouse's MapId_MercuryLighthouseExterior
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
/// flickers and sheds sparks by callback
/// (MercuryLighthouseAerie_FlickerBeacon), and the platforms start 50 pixels
/// down. Jenna, Kraden, Felix and Menardi are placed only on the first arrival:
/// every other warp destination sets FLAG_HOSTAGES_GONE before the object event
/// templates are read.
///
/// Unused: MapId_UnusedMercuryLighthouseAerieCopy, which the area
/// names call Mercury Lighthouse and the debug map list labels `MTO07NG`,
/// loads this overlay and the same map. The setup does nothing for it, and no
/// warp, table or code leads there; only the debug map selector, which
/// reaches every map, can.
///
/// Open questions:
/// - XXX (no evidence): what MercuryLighthouseAerie_UnusedAlexCastsPly was
///   for, Ply cast on the lift's statue; nothing calls it, so neither the ROM
///   nor a render shows it in a scene.

#include "game_state.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/mercury_lighthouse_aerie.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/map/common/object_nearest.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "script_layer.h"
#include "engine_call.h"
#include "map_tables.h"
#include "create_configured_overlay_object.h"
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
#include "object_event.h"
#include "map_header.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "io_reg.h"
#include "ability_ids.h"

/// The aerie's object events. The party and Jenna stand in their own slots. The
/// speakers are known from the text: Kraden ("It's all right, Jenna."), Felix
/// ("Jenna, please... Just do as she says"), Saturos (Menardi: "Oh,
/// Saturos..."), Menardi (Saturos: "Menardi... Wait!") and Alex (Mia:
/// "Alex!"). Saturos is fought as ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2.
enum MercuryLighthouseAerieLocalId {
  /// Flickers between two sizes and sheds sparks while in view
  /// (MercuryLighthouseAerie_FlickerBeacon).
  /// ★ Named from what the player sees: the swirling blue ball of light the
  /// lit beacon shows, spr_244.png (its render)
  LOCALID_BEACON = LOCALID_FIRST_PLACED,
  /// A statue, drawn as the lighthouse's statues all are, standing on
  /// the lift; it rides up and down with it.
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  LOCALID_LIFT_STATUE,
  /// Four more statues flank the beacon, two on each side of it;
  /// the two east of it, LOCALID_FIRST_BEACON_STATUE + 2 and + 3, are drawn
  /// mirrored.
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  LOCALID_FIRST_BEACON_STATUE,
  /// Six of them.
  /// ★ Named from what the player sees: round white platforms at the foot of
  /// the aerie's side ledges (its render)
  LOCALID_FIRST_PLATFORM = LOCALID_FIRST_PLACED + 6,
  /// The two platforms that stand on the lift and go up and down with it.
  /// ★ Named from what the player sees: round white platforms below the lift's
  /// statue (its render)
  LOCALID_FIRST_LIFT_PLATFORM = LOCALID_FIRST_PLATFORM + 3,
  /// ★ Named from what the player sees: round white platforms below the lift's
  /// statue (its render)
  LOCALID_SECOND_LIFT_PLATFORM,
  /// ★ Named from the game: "Kraden!" (MsgMercuryLighthouseAlreadyLit)
  LOCALID_KRADEN = LOCALID_FIRST_PLACED + 12,
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  LOCALID_FELIX,
  /// ★ Named from the game: "Oh, Saturos..." (MsgMercuryLighthouseAlreadyLit)
  LOCALID_SATUROS,
  /// ★ Named from the game: "Menardi... Wait!"
  /// (MsgMercuryLighthouseAlreadyLit)
  LOCALID_MENARDI,
  /// ★ Named from the game: "Alex!" (MsgMercuryGot)
  LOCALID_ALEX
};

/// Map flags, kept while the party stays on the aerie.
enum {
  /// The leader stands on a platform, which has dipped under the weight.
  /// ★ Named from what the player sees: the round platforms floating in the
  /// sky beside the aerie, which bob under the leader (its render)
  FLAG_ON_PLATFORM = FLAG_MAP_TEMP_50,
  /// ★ Named from what the player sees: the lift rising to the aerie with a
  /// statue on it (its render)
  FLAG_LIFT_UP = FLAG_MAP_TEMP_51,
  /// Set on every arrival but the first: Jenna, Kraden, Felix and Menardi
  /// are placed only before it.
  /// ★ Named from the game: "leave our hostages"
  /// (MsgMercuryLighthouseAlreadyLit)
  FLAG_HOSTAGES_GONE = FLAG_MAP_TEMP_53
};

enum {
  /// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
  FLAG_PSYNERGY_STONE = FLAG_MAPSEC_TEMP_50
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
#include "objcmd.h"

const struct MapWarpDest *MercuryLighthouseAerie_GetWarpDests(void) {
  return gMercuryLighthouseAerieWarpDests;
}

const struct MapWarpRegion *MercuryLighthouseAerie_GetRegions(void) {
  return 0;
}

/// Returns the one warp: the lift down to MapId_MercuryLighthouseExterior's
/// MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE, by the Fountain
/// of Healing.
const u32 *MercuryLighthouseAerie_GetWarps(void) {
  return gMercuryLighthouseAerieWarps;
}

/// Returns the object events; only the first arrival
/// (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) places Jenna, Kraden, Felix
/// and Menardi.
const struct ObjectEventTemplate *
MercuryLighthouseAerie_GetObjectEventTemplates(void) {
  if (gGameState.warpDest != MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) {
    Engine_GameFlagSet(FLAG_HOSTAGES_GONE);
  }
  return gMercuryLighthouseAerieObjectEventTemplates;
}

const struct MapEvent *MercuryLighthouseAerie_GetEvents(void) {
  return gMercuryLighthouseAerieEvents;
}

/// Sets up the Lighthouse Aerie: the beacon flickers and the six platforms
/// sit 50 pixels down; the first arrival
/// (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) plays the party's arrival, the
/// return from the battle with Saturos
/// (MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE) Alex's scene, and
/// arriving by the lift
/// (MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR) leaves the lift
/// up.
s32 MercuryLighthouseAerie_Initialize(void) {
  u32 i;
  s32 localId;
  struct ObjectEvent *objectEvent;

  Engine_GameFlagSet(FLAG_MERCURY_LIGHTHOUSE_LIT);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gGameState.mapId == (s32)&MapId_MercuryLighthouseAerie) {
    Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
    FieldScheduler_AddCallback(MercuryLighthouseAerie_FlickerBeacon,
                               TASK_PRIORITY_MAP);
    ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 1);
    ObjectMotion_SetActionVariant(GARET, 1);
    ObjectMotion_SetActionVariant(IVAN, 1);
    ObjectMotion_SetActionVariant(MIA, 1);
    ObjectMotion_SetActionVariant(JENNA, 1);
    ObjectMotion_SetActionVariant(LOCALID_KRADEN, 1);
    ObjectMotion_SetActionVariant(LOCALID_FELIX, 1);
    ObjectMotion_SetActionVariant(LOCALID_SATUROS, 1);
    ObjectMotion_SetActionVariant(LOCALID_MENARDI, 1);
    ObjectMotion_SetActionVariant(LOCALID_ALEX, 1);
    ObjectMotion_SetActionVariant(LOCALID_BEACON, 1);
    ObjectMotion_SetActionVariant(LOCALID_LIFT_STATUE, 1);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_BEACON_STATUE, 1);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_BEACON_STATUE + 1, 1);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_BEACON_STATUE + 2, 1);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_BEACON_STATUE + 3, 1);
    for (i = LOCALID_FIRST_PLATFORM; i < LOCALID_FIRST_PLATFORM + 6; i++) {
      ObjectMotion_SetActionVariant(i, 1);
      ObjectEvent_GetById(i)->motionFlags = 4;
      ObjectEvent_GetById(i)->priorityFlags |= 2;
      ObjectEvent_GetById(i)->y.fixed = -(PIXELS(50) + PIXELS(1) / 2);
    }
    if (GameFlag_Test(FLAG_KEEP_PARTY_POSITION) &&
        (localId = ObjectEvent_FindNearestFloatingPlatform()) != 0 &&
        (objectEvent = ObjectEvent_GetById(localId)) != NULL) {
      objectEvent->motionFlags = 0;
    }
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_LIFT_STATUE), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FIRST_BEACON_STATUE),
                               0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_FIRST_BEACON_STATUE + 1), 0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_FIRST_BEACON_STATUE + 2), 0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_FIRST_BEACON_STATUE + 3), 0);
    ObjectEvent_GetById(LOCALID_FIRST_BEACON_STATUE + 2)->scaleX = -Q16_ONE;
    ObjectEvent_GetById(LOCALID_FIRST_BEACON_STATUE + 3)->scaleX = -Q16_ONE;
    if (gGameState.warpDest == MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT) {
      if (!GameFlag_Test(FLAG_KEEP_PARTY_POSITION)) {
        MercuryLighthouseAerie_PartyArrives();
      }
    } else if (gGameState.warpDest ==
               MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE) {
      if (!GameFlag_Test(FLAG_LIFT_UP)) {
        {
          struct MapGridLayerScroll *lift =
              &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];

          lift->offsetY = PIXELS(1024);
        }
        MapGrid_CenterOnFollowedPoint();
        WaitFrames(1);
        MapGrid_CopyCellsTo(4, 70, 4, 74, 5, 4);
        Engine_ObjectEventSetPosition(LOCALID_LIFT_STATUE, 0, 0);
        if (!GameFlag_Test(FLAG_KEEP_PARTY_POSITION)) {
          MercuryLighthouseAerie_AlexAppears();
        }
      }
    } else if (gGameState.warpDest ==
               MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR) {
      Engine_GameFlagSet(FLAG_LIFT_UP);
    }
  }
  return 0;
}

/// Lighthouse Aerie, at MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT unless
/// returning after a battle: the Adepts of the lit element regain their PP;
/// the aerie shakes, the camera pans across and in a flash the party stands at
/// the top. Isaac shakes his head and the others gather on him.
static void MercuryLighthouseAerie_PartyArrives(void) {
  struct ObjectEvent *leader;

  FieldScript_Start();
  Party_RestorePpOfLitElements();
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(GARET), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(IVAN), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(MIA), 0);
  FieldCamera_MoveTo(PIXELS(304), -1, PIXELS(120), 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Script_PlaySong(SE_RUMBLE);
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  MapGrid_WaitForShakeToSettle();
  FieldScript_WaitFrames(30);
  FieldCamera_GetScriptObject()->motionFlags = 0;
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(512), -PIXELS(24), PIXELS(160), 1);
  FieldCamera_WaitForArrival();
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_PALE, 0);
  PaletteFade_StartInterpolation(50);
  FieldScript_WaitFrames(50);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), 0);
  PaletteFade_StartInterpolation(30);
  FieldScript_WaitFrames(30);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(504), PIXELS(168));
  ObjectEvent_SetPosition(GARET, PIXELS(528), PIXELS(144));
  ObjectEvent_SetPosition(IVAN, PIXELS(488), PIXELS(144));
  ObjectEvent_SetPosition(MIA, PIXELS(512), PIXELS(152));
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 19);
  Engine_ObjectEventSetAnimation(GARET, 19);
  Engine_ObjectEventSetAnimation(IVAN, 19);
  Engine_ObjectEventSetAnimation(MIA, 19);
  FieldScript_WaitFrames(10);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(30);
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(80);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER),
                             SPRITE_FLAG_SHADOW);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_SHAKE_HEAD);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(60);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(GARET), SPRITE_FLAG_SHADOW);
  Engine_ObjectEventSetAnimation(GARET, ANIM_STAND);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(IVAN), SPRITE_FLAG_SHADOW);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_STAND);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(MIA), SPRITE_FLAG_SHADOW);
  Engine_ObjectEventSetAnimation(MIA, ANIM_STAND);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_ObjectEventSetAnimation(GARET, ANIM_WALK);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_WALK);
  Engine_ObjectEventSetAnimation(MIA, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0)
    ObjectEvent_SetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0)
    ObjectEvent_SetDestination(IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0)
    ObjectEvent_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  ObjectMotion_CommitCurrentPositionAndActivate(MIA);
  ObjectEvent_SetPosition(MIA, 0, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(IVAN);
  ObjectEvent_SetPosition(IVAN, 0, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  FieldScript_End();
}

#include "common/object_distance.h"
#include "common/object_nearest.h"

/// Lighthouse Aerie: walking into metatile behavior 10 or 11. If nothing is in
/// the way the leader jumps two cells ahead, off any platform and onto the
/// next.
static void MercuryLighthouseAerie_JumpToPlatform(void) {
  struct ObjectEvent *objectEvent;
  struct FieldPosition position;
  s32 angle;
  u8 flags;

  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  angle = (objectEvent->facing + 0x1000) & 0xe000;
  flags = objectEvent->motionFlags;
  SET_POSITION(&position, (objectEvent->x.fixed & 0xfff00000) + PIXELS(8),
               objectEvent->y.fixed,
               (objectEvent->z.fixed & 0xfff00000) + PIXELS(8));
  Vector_AddPolarOffset(PIXELS(32), angle, &position);
  if (FieldObject_CheckMovementCollision(objectEvent, &position) == 0) {
    GameFlag_ClearBit(FLAG_ON_PLATFORM);
    MercuryLighthouseAerie_SettlePlatforms();
    ObjectDispatch_SelectAnimation(objectEvent, 6);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    ObjectDispatch_SelectAnimation(objectEvent, 7);
    objectEvent->speed = PIXELS(3);
    objectEvent->acceleration = PIXELS(2);
    objectEvent->velocityY = PIXELS(4);
    objectEvent->motionFlags &= 0x7e;
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    ObjectMotion_SetPositionAndCommit(LOCALID_PARTY_LEADER, position.x >> 16,
                                      position.z >> 16);
    ObjectDispatch_SelectAnimation(objectEvent, 6);
    ObjectEvent_SetSpriteFlags(objectEvent, SPRITE_FLAG_SHADOW);
    objectEvent->motionFlags = flags;
  }
}

/// Lighthouse Aerie: touching metatile behavior 10 when not already on a
/// platform. The platform underfoot dips with the leader and bobs back up.
static void MercuryLighthouseAerie_StepOntoPlatform(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  s32 id;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  FieldScript_Start();
  id = ObjectEvent_FindNearestFloatingPlatform();
  gMercuryLighthouseAerie_PlatformUnderfoot = id;
  if (id != 0) {
    Engine_GameFlagSet(FLAG_ON_PLATFORM);
    objectEvent =
        ObjectEvent_GetById(gMercuryLighthouseAerie_PlatformUnderfoot);
    objectEvent->motionFlags = 0;
    leader->motionFlags &= 0xfe;
    objectEvent->y.fixed -= PIXELS(3);
    leader->y.fixed -= PIXELS(3);
    leader->groundHeight -= PIXELS(3);
    WaitFrames(2);
    objectEvent->y.fixed -= PIXELS(2);
    leader->y.fixed -= PIXELS(2);
    leader->groundHeight -= PIXELS(2);
    WaitFrames(10);
    objectEvent->y.fixed += PIXELS(2);
    leader->y.fixed += PIXELS(2);
    leader->groundHeight += PIXELS(2);
    WaitFrames(4);
    objectEvent->y.fixed += PIXELS(2);
    leader->y.fixed += PIXELS(2);
    leader->groundHeight += PIXELS(2);
    WaitFrames(4);
    objectEvent->y.fixed += PIXELS(1);
    leader->y.fixed += PIXELS(1);
    leader->groundHeight += PIXELS(1);
  }
  FieldScript_End();
}

/// Lighthouse Aerie: touching metatile behavior 11, off the platforms. The
/// leader walks normally again and the platforms stop following.
static void MercuryLighthouseAerie_SettlePlatforms(void) {
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 3;
  ObjectEvent_GetById(LOCALID_FIRST_PLATFORM)->motionFlags = 4;
  ObjectEvent_GetById(LOCALID_FIRST_PLATFORM + 1)->motionFlags = 4;
  ObjectEvent_GetById(LOCALID_FIRST_PLATFORM + 2)->motionFlags = 4;
  ObjectEvent_GetById(LOCALID_FIRST_LIFT_PLATFORM)->motionFlags = 4;
  ObjectEvent_GetById(LOCALID_SECOND_LIFT_PLATFORM)->motionFlags = 4;
  ObjectEvent_GetById(LOCALID_FIRST_PLATFORM + 5)->motionFlags = 4;
}

/// Lighthouse Aerie: touching metatile behavior 5 before the confrontation.
/// Garet sees the lighthouse is lit: "It's too late! The lighthouse has already
/// been lit!" Mia learns the party is from Vale; Jenna calls out, and Felix,
/// Menardi and Saturos face the party. Menardi leaves with Jenna, Kraden and
/// Felix for the next lighthouse, and Saturos fights alone
/// (ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2).
static void MercuryLighthouseAerie_ConfrontSaturos(void) {
  struct ObjectEvent *objectEvent;

  FieldScript_Start();
  ObjectEvent_Get(LOCALID_FIRST_LIFT_PLATFORM)->motionFlags &= 0xfa;
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(GARET, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(IVAN, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(MIA, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  WaitFrames(1);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 344, 232);
  ObjectEvent_WalkTo(GARET, 328, 232);
  ObjectEvent_WalkTo(IVAN, 344, 248);
  ObjectEvent_WalkTo(MIA, 328, 248);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(GARET);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(IVAN);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(MIA);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  FieldScript_WaitFrames(50);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
  FieldCamera_MoveTo(PIXELS(328), PIXELS(40), PIXELS(176), 1);
  FieldCamera_GetScriptObject()->motionFlags = 0;
  ObjectEvent_SetSpeed(GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_WalkToAndWait(GARET, 328, 216);
  FieldCamera_WaitForArrival();
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  FieldScript_WaitFrames(60);
  FieldCamera_MoveTo(PIXELS(344), PIXELS(24), PIXELS(232), 1);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  FieldCamera_WaitForArrival();
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  FieldScript_SetMessageId(MsgMercuryLighthouseAlreadyLit);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                         ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  ObjectEvent_SetSpeed(MIA, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_WalkToAndWait(MIA, 328, 232);
  FieldScript_WaitFrames(10);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 30);
  ObjectEvent_LinkPair(IVAN, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_WalkToAndWait(IVAN, 336, 248);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectEvent_FaceDirection(MIA, FACING_EAST, 30);
  FieldMessage_ShowAtObject(MIA, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 10);
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
    Script_ShowMessageAndWait(GARET, 0, 20);
    ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(GARET, 0, 20);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    FieldScript_WaitFrames(20);
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 10);
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
    Script_ShowMessageAndWait(GARET, 0, 20);
    ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
    ObjectMotion_SetVarCbAndRefresh(GARET, 2);
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(GARET, 0, 20);
  }
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, GARET, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(MIA, 2);
  Script_ShowMessageAndWait(MIA, 0, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(MIA, FACING_EAST, 10);
  Engine_ObjectEventSetAnimation(MIA, 0x10);
  Script_ShowMessageAndWait(MIA, 0, 60);
  // Jenna calls out: "Isaac! Garet!"
  Script_PlaySong(SONG_FADE_OUT_BGM);
  ObjectEvent_SetPosition(JENNA, PIXELS(216), PIXELS(200));
  Script_ShowMessageAndWait(JENNA, 0, 20);
  ObjectEvent_SetPosition(JENNA, PIXELS(120), PIXELS(160));
  ObjectEvent_FaceDirection(JENNA, FACING_EAST, 0);
  Engine_ObjectEventSetAnimation(MIA, ANIM_STAND);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVarCbAndRefresh(GARET, 1);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 10);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 20);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 10);
  ObjectEvent_FaceDirection(MIA, FACING_WEST, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 5);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 10);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 5);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 5);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 10);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  FieldCamera_MoveTo(PIXELS(120), -PIXELS(24), PIXELS(168), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  FieldScript_WaitFrames(20);
  Script_PlaySong(MUS_061);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MENARDI, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH, 60);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_EAST, 20);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectEvent_SetPosition(GARET, PIXELS(264), PIXELS(288));
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_SetPosition(GARET, PIXELS(328), PIXELS(216));
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTHWEST, 10);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(JENNA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 30);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_EAST, 30);
  ObjectMotion_SetVarCbAndRefresh(JENNA, 2);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, 4);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, 3);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, 3);
  ObjectEvent_SetSpeed(LOCALID_FELIX, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 104, 168);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_EAST, 20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_EAST, 10);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(264), PIXELS(288));
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(296), PIXELS(120));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(JENNA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_EAST, 0);
  FieldCamera_SetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
  FieldCamera_MoveTo(PIXELS(232), PIXELS(40), PIXELS(152), 1);
  ObjectEvent_SetSpeed(LOCALID_SATUROS, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_SATUROS, 272, 128);
  ObjectEvent_WalkToAndWait(LOCALID_SATUROS, 264, 152);
  ObjectEvent_WalkToAndWait(LOCALID_SATUROS, 280, 168);
  FieldCamera_WaitForArrival();
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_MENARDI, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_SATUROS, 0);
  FieldCamera_MoveTo(PIXELS(296), PIXELS(40), PIXELS(216), 1);
  ObjectEvent_WalkToAndWait(LOCALID_SATUROS, 304, 176);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTHEAST, 0);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVariantCallback(GARET, 1);
  ObjectMotion_SetVariantCallback(IVAN, 1);
  ObjectMotion_SetVarCbAndRefresh(MIA, 1);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_SATUROS, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 70);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_SATUROS, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_SATUROS, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_MENARDI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  Script_ShowMessage(LOCALID_SATUROS, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_MENARDI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_SATUROS, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_Get(GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkTo(GARET, 328, 224);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 344, 224);
  ObjectEvent_WalkTo(IVAN, 344, 232);
  ObjectMotion_CommitCurrentPositionAndActivate(GARET);
  ObjectEvent_Get(GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(IVAN);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_SetPosition(LOCALID_MENARDI, PIXELS(168), PIXELS(200));
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectEvent_SetPosition(LOCALID_MENARDI, PIXELS(104), PIXELS(200));
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  FieldCamera_MoveTo(PIXELS(120), -PIXELS(24), PIXELS(168), 1);
  FieldCamera_WaitForArrival();
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, 3);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTH + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectEvent_FaceDirection(JENNA, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP, 70);
  ObjectEvent_LinkPair(JENNA, LOCALID_KRADEN, 0);
  FieldScript_WaitFrames(50);
  ObjectEvent_FaceDirection(JENNA, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(JENNA, 4);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        60);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectEvent_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_NORTH + FACING_STEP, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FELIX, 1);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MENARDI, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_FELIX, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 20);
  ObjectEvent_SetAttachedEffect(JENNA, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Script_ShowMessageAndWait(JENNA, 0, 40);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, 3);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(JENNA, FACING_NORTHWEST, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 30);
  ObjectEvent_SetSpeed(JENNA, PIXELS(7) / 10, PIXELS(7) / 20);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(7) / 10, PIXELS(7) / 20);
  ObjectEvent_WalkTo(JENNA, 128, 144);
  ObjectEvent_WalkToAndWait(LOCALID_KRADEN, 120, 136);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(JENNA);
  ObjectEvent_FaceDirection(JENNA, FACING_EAST, 20);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_GetById(LOCALID_FELIX)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_FELIX, 88, 152);
  ObjectEvent_Get(LOCALID_FELIX)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_NORTHWEST + FACING_STEP,
                            20);
  Engine_ObjectEventSetAnimation(LOCALID_MENARDI, 3);
  ObjectMotion_SetAnimationAndWait(LOCALID_FELIX, 3);
  FieldScript_WaitFrames(40);
  ObjectEvent_SetSpeed(LOCALID_MENARDI, PIXELS(3), PIXELS(2));
  ObjectEvent_Get(LOCALID_MENARDI)->velocityY = PIXELS(4);
  Script_PlaySong(SE_JUMP);
  ObjectEvent_Get(LOCALID_MENARDI)->motionFlags &= 0x7e;
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_MENARDI), 0);
  ObjectEvent_GetById(LOCALID_FIRST_LIFT_PLATFORM)->motionFlags = 4;
  ObjectMotion_SetPositionAndCommit(LOCALID_MENARDI, 104, 168);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_MENARDI),
                             SPRITE_FLAG_SHADOW);
  ObjectEvent_Get(LOCALID_MENARDI)->motionFlags = 3;
  ObjectEvent_FaceDirection(LOCALID_MENARDI, FACING_EAST, 30);
  ObjectEvent_FaceDirection(LOCALID_FELIX, FACING_EAST, 10);
  Script_ShowMessageAndWait(LOCALID_FELIX, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(JENNA, 2);
  ObjectMotion_SetAnimationAndWait(JENNA, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(JENNA, 0, 20);
  Script_ShowMessageAndWait(LOCALID_MENARDI, 0, 20);
  ObjectMotion_SetActionVariant(LOCALID_FIRST_LIFT_PLATFORM, 0);
  ObjectMotion_SetActionVariant(LOCALID_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_LowerLift();
  ObjectMotion_SetActionVariant(LOCALID_FIRST_LIFT_PLATFORM, 1);
  ObjectMotion_SetActionVariant(LOCALID_SECOND_LIFT_PLATFORM, 1);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTHWEST, 0);
  FieldCamera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(216), 1);
  FieldCamera_WaitForArrival();
  ObjectEvent_SetPosition(LOCALID_KRADEN, PIXELS(272), PIXELS(280));
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 20);
  ObjectEvent_SetPosition(LOCALID_KRADEN, 0, 0);
  ObjectMotion_SetVarCbAndRefresh(GARET, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_SetSpeed(GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_WalkToAndWait(GARET, 312, 216);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 10);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_WalkToAndWait(LOCALID_SATUROS, 312, 184);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTHEAST + FACING_STEP,
                            20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  ObjectEvent_SetSpeed(GARET, PIXELS(2), PIXELS(1));
  ObjectEvent_Get(GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(GARET, 328, 224);
  FieldScript_WaitFrames(1);
  ObjectEvent_Get(GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVariantCallback(GARET, 1);
  ObjectMotion_SetVariantCallback(IVAN, 1);
  ObjectMotion_SetVarCbAndRefresh(MIA, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  FieldScript_WaitFrames(10);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(10);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldMessage_ShowAtObject(LOCALID_SATUROS, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, 4);
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
    gFieldWork->message++;
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, 4);
    FieldScript_WaitFrames(20);
    gFieldWork->message++;
    Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  }
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  ObjectEvent_WalkToAndWait(LOCALID_SATUROS, 328, 200);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_NORTHWEST + FACING_STEP,
                            20);
  ObjectEvent_Get(LOCALID_SATUROS)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_SATUROS, 336, 208);
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(LOCALID_SATUROS)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_ShowEmote(LOCALID_SATUROS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(MIA, 0, 20);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  ObjectEvent_WalkToAndWait(LOCALID_SATUROS, 336, 216);
  FieldScript_SetNextDestination((s32)&MapId_MercuryLighthouseAerie, 2);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  // Saturos: ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2.
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY,
                                        2);
  FieldScript_End();
}

/// Lighthouse Aerie, at MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE, back
/// from the battle, unless returning after another: Saturos is beaten, but not
/// for long. "Isaac, we got him!" Alex appears and explains that the
/// lighthouse bound Saturos' fire Psynergy and lent Mia endless power. Saturos
/// stands again; the two warp over to the lift and ride it down, and the party
/// regroups on the leader. Sets FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED and
/// FLAG_SATUROS_BEATEN_AT_MERCURY.
static void MercuryLighthouseAerie_AlexAppears(void) {

  FieldScript_Start();
  ObjectEvent_Get(LOCALID_LIFT_STATUE)->motionFlags = 0;
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(344), PIXELS(224));
  ObjectEvent_SetPosition(GARET, PIXELS(328), PIXELS(224));
  ObjectEvent_SetPosition(IVAN, PIXELS(344), PIXELS(232));
  ObjectEvent_SetPosition(MIA, PIXELS(328), PIXELS(232));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(336), PIXELS(176));
  Engine_ObjectEventSetAnimation(LOCALID_SATUROS, 9);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_SATUROS), 0);
  FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(208), 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);

  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 20);
  FieldScript_SetMessageId(MsgMercuryGot);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 20);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 30);
  ObjectEvent_SetPosition(LOCALID_ALEX, PIXELS(312), PIXELS(112));
  Script_PlaySong(SE_STOP_SLOT3);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Script_ShowMessageAndWait(MIA, 0, 30);
  Script_PlaySong(MUS_029);
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_GetScriptObject()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(168), 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_ALEX, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_ALEX, 344, 136);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTH + FACING_STEP, 20);
  FieldCamera_WaitForArrival();
  Script_ShowMessageAndWait(MIA, 0, 30);
  FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(184), 1);
  ObjectEvent_WalkToAndWait(LOCALID_ALEX, 344, 160);
  ObjectEvent_WalkToAndWait(LOCALID_ALEX, 328, 168);
  ObjectEvent_WalkToAndWait(LOCALID_ALEX, 312, 176);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  FieldCamera_WaitForArrival();
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 20);
  ObjectMotion_SetVarCbAndRefresh(MIA, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 60);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectMotion_SetVariantCallback(LOCALID_ALEX, 2);
  ObjectEvent_ShowEmote(LOCALID_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(MIA, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(MIA, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_WalkToAndWait(LOCALID_ALEX, 312, 184);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 20);
  FieldMessage_ShowAtObject(GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetVarCbAndRefresh(GARET, 2);
    Script_ShowMessageAndWait(GARET, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    ObjectMotion_SetVarCbAndRefresh(GARET, 1);
    Script_ShowMessageAndWait(GARET, 0, 20);
  }
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_NORTHWEST + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ALEX, 2);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_ALEX, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_ALEX, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ALEX, 2);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_ALEX, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Script_ShowMessage(IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 3);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_ALEX, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ALEX, 2);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_ALEX, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_EAST + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
  FieldScript_WaitFrames(30);
  Engine_ObjectEventSetAnimation(LOCALID_SATUROS, 8);
  FieldScript_WaitFrames(45);
  Engine_ObjectEventSetAnimation(LOCALID_SATUROS, 1);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_SATUROS),
                             SPRITE_FLAG_SHADOW);
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Script_ShowMessage(IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, 4);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_SATUROS, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_ALEX, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(MIA, 2);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_ALEX, LOCALID_SATUROS, 0);
  FieldScript_WaitFrames(35);
  ObjectMotion_SetAnimationAndWait(LOCALID_SATUROS, 3);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 3);
  FieldScript_WaitFrames(20);
  ObjectEvent_WalkToAndWait(LOCALID_ALEX, 328, 176);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ObjectEventSetAnimation(LOCALID_ALEX, 5);
  Engine_ObjectEventSetAnimation(LOCALID_SATUROS, 7);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(GARET, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldMessage_ShowAtObject(LOCALID_ALEX, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVariantCallback(GARET, 2);
  ObjectMotion_SetVariantCallback(IVAN, 2);
  ObjectMotion_SetVarCbAndRefresh(MIA, 2);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 0);
  FieldScript_WaitFrames(20);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(30);
    Script_ShowMessage(LOCALID_ALEX, 0);
    gFieldWork->message++;
  } else {
    FieldScript_WaitFrames(30);

    gFieldWork->message++;
    Script_ShowMessage(LOCALID_ALEX, 0);
  }
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 4);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_ALEX, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  MercuryLighthouseAerie_SaturosAndAlexWarpOut();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(GARET, 2);
  ObjectMotion_SetVariantCallback(IVAN, 2);
  ObjectMotion_SetVarCbAndRefresh(MIA, 2);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 5);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 5);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 5);
  ObjectEvent_FaceDirection(MIA, FACING_WEST, 5);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 5);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST, 5);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 5);
  ObjectEvent_FaceDirection(MIA, FACING_EAST, 5);
  FieldCamera_GetScriptObject()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(232), 1);
  FieldCamera_WaitForArrival();
  ObjectEvent_SetChildValue(LOCALID_SATUROS, 0xF);
  ObjectEvent_SetChildValue(LOCALID_ALEX, 0xF);
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(240), PIXELS(208));
  ObjectEvent_SetPosition(LOCALID_ALEX, PIXELS(232), PIXELS(208));
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHEAST + FACING_STEP, 0);
  MercuryLighthouseAerie_SaturosAndAlexWarpIn();
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 10);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 15);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_WEST, 20);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 328, 208);
  ObjectEvent_WalkTo(IVAN, 336, 224);
  ObjectEvent_WalkToAndWait(GARET, 312, 216);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(IVAN);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 30);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(MIA, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 20);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
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
    ObjectEvent_Get(LOCALID_LIFT_STATUE)->motionFlags = 0;
    ObjectEvent_SetPosition(LOCALID_LIFT_STATUE, PIXELS(104), PIXELS(264));
    ObjectEvent_Get(LOCALID_LIFT_STATUE)->y.fixed = -PIXELS(32);
    ObjectEvent_GetById(LOCALID_LIFT_STATUE)->targetY = -PIXELS(32);
    MapGrid_CopyCellsTo(0x1D, 0x4A, 4, 0x4A, 5, 4);
    ObjectMotion_SetActionVariant(LOCALID_FIRST_LIFT_PLATFORM, 0);
    ObjectMotion_SetActionVariant(LOCALID_SECOND_LIFT_PLATFORM, 0);
    MercuryLighthouseAerie_RaiseLift();
    ObjectMotion_SetActionVariant(LOCALID_FIRST_LIFT_PLATFORM, 1);
    ObjectMotion_SetActionVariant(LOCALID_SECOND_LIFT_PLATFORM, 1);
    FieldCamera_WaitForArrival();
    FieldScript_WaitFrames(30);
    ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_WEST, 20);
    Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
    FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(216), 1);
    FieldCamera_WaitForArrival();
    ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
    Engine_ObjectEventSetAnimation(LOCALID_ALEX, 5);
    Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
    ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 20);
    Script_ShowMessageAndWait(IVAN, 0, 20);
    ObjectMotion_SetVarCbAndRefresh(GARET, 1);
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(GARET, 0, 20);
    ObjectEvent_ShowEmote(LOCALID_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    FieldMessage_ShowAtObject(LOCALID_ALEX, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_WaitFrames(20);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 2);
      FieldScript_WaitFrames(20);
      Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
      ((struct FieldWork *)cells[19])->message++;
    } else {
      ((struct FieldWork *)cells[19])->message++;
      FieldScript_WaitFrames(20);
      Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
    }
  }
  ObjectMotion_SetVarCbAndRefresh(LOCALID_ALEX, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldMessage_ShowAtObject(LOCALID_ALEX, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_ALEX, 2);
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_ALEX, 1);
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  }
  FieldScript_WaitFrames(20);
  MercuryLighthouseAerie_SaturosAndAlexWarpOut();
  FieldScript_WaitFrames(20);
  FieldCamera_MoveTo(PIXELS(128), -1, PIXELS(200), 1);
  FieldCamera_WaitForArrival();
  Engine_ObjectEventSetAnimation(LOCALID_SATUROS, 1);
  Engine_ObjectEventSetAnimation(LOCALID_ALEX, 1);
  ObjectEvent_SetChildValue(LOCALID_SATUROS, 0xF);
  ObjectEvent_SetChildValue(LOCALID_ALEX, 0xF);
  ObjectEvent_SetPosition(LOCALID_SATUROS, PIXELS(120), PIXELS(152));
  ObjectEvent_SetPosition(LOCALID_ALEX, PIXELS(112), PIXELS(160));
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHEAST + FACING_STEP, 0);
  MercuryLighthouseAerie_SaturosAndAlexWarpIn();
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_EAST, 20);
  FieldMessage_ShowAtObject(LOCALID_ALEX, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 3);
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, 4);
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  }
  Script_ShowMessageAndWait(LOCALID_ALEX, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SATUROS, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_SATUROS, FACING_EAST, 20);
  Script_ShowMessageAndWait(LOCALID_SATUROS, 0, 20);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_EAST, 20);
  ObjectMotion_SetActionVariant(LOCALID_FIRST_LIFT_PLATFORM, 0);
  ObjectMotion_SetActionVariant(LOCALID_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_LowerLift();
  ObjectMotion_SetActionVariant(LOCALID_FIRST_LIFT_PLATFORM, 1);
  ObjectMotion_SetActionVariant(LOCALID_SECOND_LIFT_PLATFORM, 1);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  FieldCamera_MoveTo(PIXELS(336), -1, PIXELS(216), 1);
  FieldCamera_WaitForArrival();
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Map_PlayMusic();
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 22);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  FieldScript_WaitFrames(10);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_FaceDirection(MIA, FACING_WEST, 20);
  Engine_ObjectEventSetAnimation(MIA, 0x10);
  ObjectEvent_Get(MIA)->scaleX = -Q16_ONE;
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldMessage_ShowAtObject(IVAN, 0);
  Engine_ObjectEventSetAnimation(MIA, ANIM_STAND);
  ObjectEvent_Get(MIA)->scaleX = Q16_ONE;
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 20);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(IVAN, 0, 20);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(IVAN, 0, 20);
  }
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 30);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  ObjectMotion_SetVarCbAndRefresh(GARET, 1);
  FieldScript_WaitFrames(10);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 20);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(MIA, 1);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(MIA, 328, 216);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_Get(GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkTo(GARET, 312, 200);
  ObjectEvent_WalkTo(MIA, 280, 216);
  ObjectMotion_CommitCurrentPositionAndActivate(GARET);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(MIA);
  ObjectMotion_SetVarCbAndRefresh(MIA, 2);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(MIA, FACING_EAST, 20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(GARET, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_NOD);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(IVAN, 2);
  ObjectMotion_SetVariantCallback(GARET, 2);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_FaceDirection(MIA, FACING_EAST, 20);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 30);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 40);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 312, 216);
  ObjectEvent_WalkTo(GARET, 312, 216);
  ObjectEvent_WalkTo(IVAN, 312, 216);
  ObjectEvent_WalkTo(MIA, 312, 216);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectMotion_CommitCurrentPositionAndActivate(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(IVAN);
  ObjectEvent_SetPosition(IVAN, 0, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(MIA);
  ObjectEvent_SetPosition(MIA, 0, 0);
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
  FieldScript_End();
}

/// Lighthouse Aerie: touching metatile behavior 6 while the lift is down. The
/// camera watches it rise with its statue.
static void MercuryLighthouseAerie_CallLift(void) {
  struct MapGridLayerScroll *lift;

  lift = &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];
  FieldScript_Start();
  lift->offsetY = PIXELS(896);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  ObjectEvent_GetById(LOCALID_LIFT_STATUE)->motionFlags = 0;
  Engine_ObjectEventSetPosition(LOCALID_LIFT_STATUE, PIXELS(104), PIXELS(264));
  ObjectEvent_GetById(LOCALID_LIFT_STATUE)->y.fixed = -PIXELS(32);
  ObjectEvent_GetById(LOCALID_LIFT_STATUE)->targetY = -PIXELS(32);
  FieldCamera_GetScriptObject()->motionFlags = 0;
  FieldCamera_SetSpeedParameters(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveWithinMapBounds(PIXELS(128), -1, PIXELS(184), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(30);
  MapGrid_CopyCellsTo(29, 74, 4, 74, 5, 4);
  ObjectMotion_SetActionVariant(LOCALID_FIRST_LIFT_PLATFORM, 0);
  ObjectMotion_SetActionVariant(LOCALID_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_RaiseLift();
  ObjectMotion_SetActionVariant(LOCALID_FIRST_LIFT_PLATFORM, 1);
  ObjectMotion_SetActionVariant(LOCALID_SECOND_LIFT_PLATFORM, 1);
  FieldScript_WaitFrames(20);
  Engine_GameFlagSet(FLAG_LIFT_UP);
  FieldScript_End();
}

/// Lighthouse Aerie: touching metatile behavior 12. The leader steps onto the
/// lift and rides it down to MapId_MercuryLighthouseExterior, arriving at
/// MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE by the Fountain of
/// Healing.
static void MercuryLighthouseAerie_RideLiftDown(void) {
  FieldScript_Start();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 104, 152);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 60);
  ObjectMotion_SetActionVariant(LOCALID_FIRST_LIFT_PLATFORM, 0);
  ObjectMotion_SetActionVariant(LOCALID_SECOND_LIFT_PLATFORM, 0);
  MercuryLighthouseAerie_LowerLift();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_ScriptRequestWarp(
      MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR);
  FieldScript_End();
}

/// Saturos and Alex warp out in a column of sparkles and vanish.
static void MercuryLighthouseAerie_SaturosAndAlexWarpOut(void) {
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;
  struct ObjectEvent *left;
  struct ObjectEvent *right;
  u32 i;
  s32 x;
  s32 r;

  left = ObjectEvent_GetById(LOCALID_SATUROS);
  right = ObjectEvent_GetById(LOCALID_ALEX);
  Sound_PlaySong(SE_190);
  ObjectEvent_SetChildValue(LOCALID_SATUROS, 0x100);
  ObjectGroup_ConfigureChildValue(LOCALID_ALEX, 0x100);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_SATUROS), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_ALEX), 0);
  options = &optionsStorage;
  options->priority = 1;
  options->palette = 5;
  options->type = SPRITE_PARTICLE;
  options->startScaleX = 2 * Q16_ONE / 5;
  options->startScaleY = 3 * Q16_ONE;
  for (i = 0; i <= 31; i++) {
    FieldScript_WaitFrames(1);
    if (i & 1) {
      r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
      x = left->x.fixed;
      x += r;
      x += -PIXELS(12);
      FieldEffect_Spawn(x,
                        left->y.fixed +
                            ((((u32)Engine_RandomNext() << 5) >> 16) << 16) +
                            -PIXELS(16),
                        left->z.fixed, 0, PIXELS(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        options);
    } else {
      r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
      x = right->x.fixed;
      x += r;
      x += -PIXELS(12);
      FieldEffect_Spawn(x,
                        right->y.fixed +
                            ((((u32)Engine_RandomNext() << 5) >> 16) << 16) +
                            -PIXELS(16),
                        right->z.fixed, 0, PIXELS(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        options);
    }
  }
  Engine_ObjectEventSetPosition(LOCALID_SATUROS, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_ALEX, 0, 0);
}

/// Saturos and Alex warp in where they have been placed.
static void MercuryLighthouseAerie_SaturosAndAlexWarpIn(void) {
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;
  struct ObjectEvent *left;
  struct ObjectEvent *right;
  u32 i;
  s32 x;
  s32 r;

  left = ObjectEvent_GetById(LOCALID_SATUROS);
  right = ObjectEvent_GetById(LOCALID_ALEX);
  Sound_PlaySong(SE_190);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_SATUROS), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_ALEX), 0);
  options = &optionsStorage;
  options->priority = 1;
  options->palette = 5;
  options->type = SPRITE_PARTICLE;
  options->startScaleX = 2 * Q16_ONE / 5;
  options->startScaleY = 3 * Q16_ONE;
  for (i = 0; i <= 31; i++) {
    FieldScript_WaitFrames(1);
    if (i & 1) {
      r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
      x = left->x.fixed;
      x += r;
      x += -PIXELS(12);
      FieldEffect_Spawn(x,
                        left->y.fixed +
                            ((((u32)Engine_RandomNext() << 5) >> 16) << 16) +
                            PIXELS(32),
                        left->z.fixed, 0, -PIXELS(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        options);
    } else {
      r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
      x = right->x.fixed;
      x += r;
      x += -PIXELS(12);
      FieldEffect_Spawn(x,
                        right->y.fixed +
                            ((((u32)Engine_RandomNext() << 5) >> 16) << 16) +
                            PIXELS(32),
                        right->z.fixed, 0, -PIXELS(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        options);
    }
    if (i == 20) {
      ObjectEvent_SetChildValue(LOCALID_SATUROS, 0x100);
      ObjectGroup_ConfigureChildValue(LOCALID_ALEX, 0x100);
    }
  }
  ObjectGroup_ConfigureChildValue(LOCALID_SATUROS, 0);
  ObjectGroup_ConfigureChildValue(LOCALID_ALEX, 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_SATUROS),
                             SPRITE_FLAG_SHADOW);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_ALEX),
                             SPRITE_FLAG_SHADOW);
}

/// MercuryLighthouseAerie_LowerLift passes its lift as a third
/// argument, which this function does not take.
static void MercuryLighthouseAerie_FindLiftRiders(s32 *out, s32 y) {
  struct FieldWork *work;
  u32 slot;

  work = gFieldWork;
  y = 64 - (y >> 20);
  for (slot = 0;
       slot < sizeof(work->objectEvents) / sizeof(work->objectEvents[0]);
       slot++) {
    struct ObjectEvent *objectEvent = work->objectEvents[slot];

    if (objectEvent != 0) {
      s32 cell = objectEvent->x.fixed >> 20;
      s32 row = objectEvent->z.fixed >> 20;

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
  struct ObjectEvent *objectEvent;

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
    ObjectEvent_GetById(ids[i])->motionFlags = 0;
    n++;
  }
  Sound_PlaySong(SE_LIFT);
  for (i = 0; i <= 227; i++) {
    lift->offsetY -= speed;
    for (j = 0; j < n; j++) {
      ObjectEvent_GetById(ids[j])->z.fixed += speed;
      objectEvent = ObjectEvent_GetById(ids[j]);
      objectEvent->targetZ = ObjectEvent_GetById(ids[j])->z.fixed;
    }
    if ((i & 3) == 3)
      speed += 0x1999;
    if (speed > 0x17fff)
      speed = 0x18000;
    WaitFrames(1);
  }
  for (i = 0; i < n; i++)
    ObjectEvent_GetById(ids[i])->motionFlags = 0;
}

/// The lift rises with everything standing on it, slowing at the top.
static void MercuryLighthouseAerie_RaiseLift(void) {
  s32 ids[5];
  struct MapGridLayerScroll *lift;
  u32 i;
  u32 j;
  u32 n;
  s32 speed;
  struct ObjectEvent *objectEvent;

  lift = &((struct MapGridScrollWork *)gMapGridWork[0])->layers[2];
  speed = 0x18000;
  n = 0;
  for (i = 0; i <= 4; i++)
    ids[i] = 66;
  MercuryLighthouseAerie_FindLiftRiders(ids, lift->offsetY);
  for (i = 0; i <= 4; i++) {
    if (ids[i] == 66)
      break;
    ObjectEvent_GetById(ids[i])->motionFlags = 0;
    n++;
  }
  Sound_PlaySong(SE_LIFT);
  for (i = 0; i <= 85; i++) {
    lift->offsetY += speed;
    for (j = 0; j < n; j++) {
      ObjectEvent_GetById(ids[j])->z.fixed -= speed;
      objectEvent = ObjectEvent_GetById(ids[j]);
      objectEvent->targetZ = ObjectEvent_GetById(ids[j])->z.fixed;
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
/// Nothing calls it. In the game, Alex and Saturos warp over to the lift and
/// ride it down without it (MercuryLighthouseAerie_AlexAppears).
UNUSED static void MercuryLighthouseAerie_UnusedAlexCastsPly(void) {
  FieldEffect_LoadActionEffectResources(ABILITY_PLY, 1);
  FieldEffect_SetupObjectPair(LOCALID_ALEX, LOCALID_LIFT_STATUE);
  FieldEffect_StartCast();
  Field_DispatchTypeHandler(1);
  FieldEffect_StopCasterHook();
  FieldEffect_End();
}

/// A beacon spark's update: it swings round the beacon it is linked to, its
/// angle (its timer, in 1/65536 turns) stepping back each frame; its speed
/// holds the swing's radius in pixels, less 28.
static void
MercuryLighthouseAerie_UpdateBeaconSpark(union FieldObject *fieldObject) {
  struct ObjectEvent *spark = &fieldObject->objectEvent;
  struct ObjectEvent *beacon;
  u16 angle;

  beacon = spark->linkedObject;
  angle = spark->timer;
  spark->x.fixed = beacon->x.fixed + Trig_Cos(angle) * (spark->speed + 28);
  spark->z.fixed = (Trig_Sin(angle) << 4) + PIXELS(144);
  spark->targetX = spark->x.fixed;
  spark->targetZ = spark->z.fixed;
  spark->timer -= 0x200;
}

/// Every frame: while the view is near the top the beacon flickers
/// between two sizes, and every 16 frames it throws off a spark that swings
/// round it.
static void MercuryLighthouseAerie_FlickerBeacon(void) {
  struct ObjectEvent *beacon;
  struct ObjectEvent *spark;
  struct Sprite *sprite;
  struct MapGridScrollWork *grid;
  s32 height;
  s32 scale;
  s32 tick;
  u32 angle;

  beacon = ObjectEvent_GetById(LOCALID_BEACON);
  grid = gMapGridWork[0];
  height = ((u32)(Engine_RandomNext() * 48) >> 16) << 16;
  if (((union FieldCoordinate *)&grid->viewY)->part.pixel <= 129) {
    if (gFrameCount & 1) {
      Engine_ObjectEventSetPosition(LOCALID_BEACON, PIXELS(304), PIXELS(144));
      scale = Q16_ONE;
      ObjectEvent_GetById(LOCALID_BEACON)->scaleX = scale;
    } else {
      Engine_ObjectEventSetPosition(LOCALID_BEACON, PIXELS(304), PIXELS(151));
      scale = 13 * Q16_ONE / 10;
      ObjectEvent_GetById(LOCALID_BEACON)->scaleX = scale;
    }
    ObjectEvent_GetById(LOCALID_BEACON)->scaleY = scale;
  } else {
    ObjectEvent_SetPosition(LOCALID_BEACON, PIXELS(8), PIXELS(8));
  }
  if (beacon == NULL) {
    return;
  }
  tick = gFrameCount & 15;
  if (tick != 0) {
    return;
  }
  spark =
      ObjectEvent_Create(SPRITE_PARTICLE, beacon->x.fixed + PIXELS(8),
                         beacon->y.fixed + height + PIXELS(8), beacon->z.fixed);
  height = height / PIXELS(6);
  height <<= 16;
  if (spark == NULL) {
    return;
  }
  sprite = spark->sprite;
  ObjectEvent_SetScript(spark, gMercuryLighthouseAerie_SparkScript);
  ObjectGroup_SetChildValue(spark, 3);
  spark->motionFlags = tick;
  angle = Engine_RandomNext() & 0x0ffff000;
  spark->timer = angle;
  spark->work = tick;
  spark->linkedObject = beacon;
  spark->update = MercuryLighthouseAerie_UpdateBeaconSpark;
  spark->speed = (Trig_Sin((height & 0xfffff) >> 4) * 24) >> 16;
  sprite->flags = 0;
  sprite->image.priority = beacon->sprite->image.priority;
}

s32 gShortEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gMediumEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gLongEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gShortEffectScript,
    gMediumEffectScript,
    gLongEffectScript,
};

struct MapWarpDest gMercuryLighthouseAerieWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT, CONDITION_ALWAYS, 8, 0, 8,
     FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE, CONDITION_ALWAYS, 8, 0, 8,
     FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR,
     CONDITION_ALWAYS, 504, 0, 152, FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {51, CONDITION_ALWAYS, 504, 0, 168, FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {MAP_TABLE_END},
};

u32 gMercuryLighthouseAerieWarps[] = {
    MAP_WARPS(MercuryLighthouseAerie),
    MAP_WARP(MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR,
             MercuryLighthouseExterior,
             MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gMercuryLighthouseAerieObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTHWEST, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTHEAST, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { JENNA, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(160), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(304), PIXELS(40), PIXELS(144), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(440), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(472), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(504), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(104), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(168), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
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
    { MAP_TABLE_END },
};
// clang-format on

struct MapEvent gMercuryLighthouseAerieEvents[] = {
    {MAP_EVENT_COORD, 5, FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED,
     EVENT_SCRIPT(MercuryLighthouseAerie_ConfrontSaturos)},
    {MAP_EVENT_COORD, 6, FLAG_LIFT_UP,
     EVENT_SCRIPT(MercuryLighthouseAerie_CallLift)},
    {COORD_EVENT_WALKING, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(MercuryLighthouseAerie_JumpToPlatform)},
    {MAP_EVENT_COORD, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(MercuryLighthouseAerie_SettlePlatforms)},
    {COORD_EVENT_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(MercuryLighthouseAerie_JumpToPlatform)},
    {MAP_EVENT_COORD, 10, FLAG_ON_PLATFORM,
     EVENT_SCRIPT(MercuryLighthouseAerie_StepOntoPlatform)},
    {BG_EVENT(BG_EVENT_SILENT), 100, FLAG_PSYNERGY_STONE, FIND_PSYNERGY_STONE},
    {MAP_EVENT_COORD, 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(MercuryLighthouseAerie_RideLiftDown)},
    {MAP_EVENTS_END},
};

s32 gMercuryLighthouseAerie_PlatformUnderfoot = 0;

s32 gMercuryLighthouseAerie_SparkScript[] = {
    SC_SET(SW_SCALE_Y, 7 * Q16_ONE / 5),
    SC_SET(SW_SCALE_X, 7 * Q16_ONE / 5),
    SC_ANIMATION(4),
    SC_WAIT(30),
    SC_FREE,
};
