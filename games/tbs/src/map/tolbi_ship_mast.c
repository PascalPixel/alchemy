/// @file
/// Tolbi-bound Ship (MapId_TolbiShipMast), the top of its mast, where the
/// ship's lookout (LOCALID_LOOKOUT) keeps watch: one of the ship's three maps
/// on its voyage from Kalay to Tolbi, which tolbi_ship_deck.c describes. The
/// party climbs up by the deck's ladder (TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK)
/// and down again by MB_TOLBI_SHIP_MAST_LADDER and the region of
/// TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK. The warp destinations from
/// TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR on are the voyage's
/// cutscenes here, each leading on into the next; during the attacks the
/// map a defeat returns to (gSaveBlock.savedMap) is set to
/// TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT, which starts the voyage
/// over. While the ship is under way (rowing, or at sea after the first
/// attack, and neither at the island nor at Tolbi) the setup sways the view.
/// In story order:
///
/// - At Kalay, while the ship waits for its replacement: the captain's anchor
///   charm, gone from his quarters, lies up here. Searching for it
///   (MB_TOLBI_SHIP_MAST_ANCHOR_CHARM) finds the Anchor Charm
///   (FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND), but only once the party has
///   confronted the crewman on deck or the warriors have gone to the captain
///   (FLAG_TOLBI_SHIP_CREWMAN_CAUGHT or FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN).
///   The lookout is the crewman who hid it: until the first attack, Mind Read
///   on him has him wonder whether the party has worked out that he was the
///   one. Once the charm is back the setup moves him to his lookout spot.
/// - Under sail: at TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR he scans
///   both sides and calls the course clear, which leads below to the
///   cast-off; the first time, the leader is moved out of view, and on later
///   departures he stands by.
/// - The attacks: TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK, "Avast!
///   Monsters sighted!", as three monsters appear where a sparkle twinkles and
///   close in (FLAG_TOLBI_SHIP_MONSTERS_SIGHTED, which plays MUS_029); Sean's
///   "Monsters!" after a bird swoops at the lookout
///   (TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK); and a monster dropping in
///   beside the lookout (TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK). Each
///   leads below or to the deck.
/// - Land: at TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO the seven birds
///   (LOCALID_FIRST_BIRD) fly away before "Land ho!". The rowing values of the
///   passengers chosen to row (gTolbiShipMast_PassengerRowValues) decide where
///   the ship comes in: those at the first and third seats less those at the
///   second and fourth. A balance of exactly 11 takes
///   TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE, off course to the island;
///   any other takes TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED and
///   Tolbi. Once the rowers have switched sides at the island the balance is
///   the opposite, so the ship comes in to Tolbi.
///
/// At sea the lookout says he is the lookout, not an oarsman; at the island he
/// warns of monsters and wonders at its treasures.
///
/// Open questions:
/// - XXX (no evidence): what the eight words after
///   gTolbiShipMast_BirdDeparture6's SC_END were; nothing reads them, since the
///   script stops at SC_END and only this overlay can name its data (O3), and
///   as sixteen halfwords they form no table any code here indexes.

#include "action_script.h"
#include "save_block.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/tolbi_ship_mast.h"
#include "unit/overworld/overworld.h"
#include "unit/field/field.h"
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
#include "heap_slots.h"
#include "item_ids.h"
#include "flags.h"
#include "songs.h"
#include "types.h"
#include "script_layer.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "map_tables.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "map_grid_scroll.h"
#include "heap.h"
#include "macros.h"

// Scene sequences and the mast's per-frame updates.
static void TolbiShipMast_SetupHiddenLeaderLookout(void);
static void TolbiShipMast_SetupVisibleLeaderLookout(void);
static void TolbiShipMast_AnnounceClearCourse(void);
static void TolbiShipMast_RunAvastScene(void);
static void TolbiShipMast_ShowMonsters(void);
static void TolbiShipMast_InitializeDriftingObjectEvent(s32 localId);
static void TolbiShipMast_RunLandingSequence(void);
static void TolbiShipMast_AnnounceLand(void);
static void TolbiShipMast_UpdateSway(void);
static s32 TolbiShipMast_GetRowingBalance(void);
static s32 TolbiShipMast_GetSeatRowValue(u32 seat);

s32 gTolbiShipMast_DriftY __attribute__((section(".bss")));
s32 gTolbiShipMast_DriftX __attribute__((section(".bss")));
s32 gTolbiShipMast_SwayY __attribute__((section(".bss")));
s32 gTolbiShipMast_SwayUnused __attribute__((section(".bss")));
s32 gTolbiShipMast_CameraCenter[2] __attribute__((section(".bss")));
s32 gTolbiShipMast_CameraOrigin[2] __attribute__((section(".bss")));
s32 gTolbiShipMast_SwayX __attribute__((section(".bss")));

/// The map's one object event template row.
enum TolbiShipMastLocalId {
  /// The ship's lookout, a man in a blue bandana
  /// (SPRITE_MAN_IN_BLUE_BANDANA). He is the crewman who hid the captain's
  /// anchor charm (LOCALID_KALAY_CREWMAN on deck, LOCALID_QUARTERS_CREWMAN
  /// below), in the
  /// same sprite: Mind Read on him at Kalay hears "I was the one who hid the
  /// anchor" (MsgTolbiShipHiddenAnchorDiscovered).
  /// ★ Named from the game: "No, no, I'm the lookout."
  /// (MsgTolbiShipLookoutCannotRow)
  LOCALID_LOOKOUT = LOCALID_FIRST_PLACED
};

/// The first attack's object events
/// (TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK), placed after the lookout from
/// gTolbiShipMast_FirstAttackObjectEventTemplates.
enum TolbiShipMastFirstAttackLocalId {
  /// A sparkle (graphics/sprite/spr_286.png, a four-pointed sparkle in eight
  /// shrinking frames), placed nowhere else. It turns, swells for 10 frames and
  /// shrinks for 24 (gTolbiShipMast_FollowCameraActions) where the monsters
  /// then appear.
  /// ★ Named from what the player sees: a white four-pointed sparkle
  /// (its render)
  LOCALID_SPARKLE = LOCALID_LOOKOUT + 1,
  /// The first of three monsters, all with the sprite of the first attack's
  /// monsters below decks (gTolbiShipInteriors_FirstAttackMonsters). They grow
  /// from half size as they close in.
  /// ★ Named from the game: "Monsters sighted!" (MsgTolbiShipAvast)
  LOCALID_FIRST_SIGHTED_MONSTER
};

/// The object events of the later cutscenes (from
/// TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK on), placed after the lookout
/// from gTolbiShipMast_BirdObjectEventTemplates and, at
/// TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK, then from
/// gTolbiShipMast_LandingObjectEventTemplates.
enum TolbiShipMastEnsembleLocalId {
  /// The first of seven birds, all of one sprite (SPRITE_BIRD, a
  /// white bird in three wing poses), that flutter and bob about the mast
  /// (TolbiShipMast_SwayObjectEvent). One swoops at the lookout at the second
  /// attack,
  /// they scatter as the third attack's monster comes, and they fly off just
  /// before "Land ho!".
  /// ★ Named from what the player sees: a white bird beating its wings (its
  /// render)
  LOCALID_FIRST_BIRD = LOCALID_LOOKOUT + 1,
  /// Sean, by the sprite he has in the hold (LOCALID_HOLD_SEAN,
  /// tolbi_ship_interiors.c)
  /// and on deck (LOCALID_SEA_SEAN, tolbi_ship_deck.c). He cries "Monsters!" at
  /// the
  /// second attack.
  /// ★ Named from the game: "That'll be me, Ouranos, and Sean on deck"
  /// (MsgTolbiShipOurReplacementNeverArrivedBut)
  LOCALID_SEAN = LOCALID_FIRST_BIRD + 7,
  /// The sprite of the third attack's monster below decks
  /// (gTolbiShipInteriors_ThirdAttackMonsters); it drops in beside the lookout.
  /// ★ Named from what the player sees: a blue winged monster (its render)
  LOCALID_LANDING_MONSTER,
  /// The landing monster's shadow: a blank sprite (SPRITE_BLANK) draws only
  /// its shadow, which grows from a tenth of its size where
  /// LOCALID_LANDING_MONSTER drops, then follows the monster away
  /// (tolbi_ship_deck.c's LOCALID_SEA_FALLING_MONSTER_SHADOW does the same).
  /// ★ Named from what the player sees: a black shadow that grows on the
  /// crow's nest where the monster lands (its render)
  LOCALID_LANDING_MONSTER_SHADOW
};

/// The mast's coord event and bg event cells.
enum TolbiShipMastMetatileBehavior {
  MB_TOLBI_SHIP_MAST_LADDER = 10, ///< the top of the ladder down to the deck
  /// Where the crewman hid the captain's anchor charm.
  /// ★ Named from the game: "I hid it on top of the mast."
  /// (MsgTolbiShipInteriorsIWaitedTooLongIllGet)
  MB_TOLBI_SHIP_MAST_ANCHOR_CHARM = 11
};

/// Keeps the effect object event relative to the saved camera origin and
/// rotates it.
static s32 TolbiShipMast_FollowCameraAndRotate(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s32 *position = ((struct MapGridScrollWork *)gMapGridWork[0])->origin;
  s32 *origin = gTolbiShipMast_CameraOrigin;
  s32 *center = gTolbiShipMast_CameraCenter;
  s32 cameraX = *position++;
  s32 cameraY = *position;

  objectEvent->x.fixed = center[0] + (cameraX - origin[0]);
  objectEvent->y.fixed = center[1] + (cameraY - origin[1]) / 2;
  objectEvent->sprite->rotation += 0x600;
  return 0;
}

/// A random sample of 6 or 9 turns the lookout to either side of north.
static s32 TolbiShipMast_RandomizeLookoutFacing(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  u32 sample = (u32)(Import_RandomNext() << 6) >> 16;

  if (sample == 6) {
    s32 value = FACING_NORTH + FACING_STEP;

    objectEvent->facing = value;
  } else if (sample == 9) {
    s32 value = FACING_NORTHWEST + FACING_STEP;

    objectEvent->facing = value;
  }

  return 1;
}

/// Grows both axes by the same amount until the object event reaches full
/// scale.
static void TolbiShipMast_GrowObjectEventScale(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  if (objectEvent->scaleX < Q16_ONE) {
    objectEvent->scaleX += 160;
    objectEvent->scaleY += 160;
  }
}

/// Which way TolbiShipMast_SwayObjectEvent moves an object event along each
/// axis, kept in its timer (the drift) and its work (the bob).
enum TolbiShipMastSway {
  SWAY_INCREASING = 0, ///< the drift speeding up, or the bob rising
  SWAY_DECREASING = 1, ///< the drift slowing, or the bob sinking
  SWAY_HELD = 9        ///< held at zero; nothing here sets it
};

/// Randomly reverses horizontal drift and vertical bobbing at their limits.
static s32 TolbiShipMast_SwayObjectEvent(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s16 *state = &objectEvent->timer;
  s32 next;

  if (*state == SWAY_HELD) {
    objectEvent->riseBaseY = 0;
  } else if (*state != SWAY_INCREASING) {
    objectEvent->riseBaseY -= (u32)(Import_RandomNext() << 11) >> 16;
    if (objectEvent->riseBaseY < -3 * PIXELS(1) / 4) {
      next = SWAY_INCREASING;
      *state = next;
    }
  } else {
    objectEvent->riseBaseY += (u32)(Import_RandomNext() << 11) >> 16;
    if (objectEvent->riseBaseY > 3 * PIXELS(1) / 4) {
      next = SWAY_DECREASING;
      *state = next;
    }
  }
  if (objectEvent->x.fixed > PIXELS(40) && objectEvent->x.fixed < PIXELS(320)) {
    objectEvent->x.fixed += objectEvent->riseBaseY;
  }
  state = (s16 *)&objectEvent->work;
  if (*state == SWAY_HELD) {
    objectEvent->y.fixed = 0;
  } else if (*state != SWAY_INCREASING) {
    objectEvent->y.fixed -= ((u32)Import_RandomNext() * 3 << 14) >> 16;
    if (objectEvent->y.fixed < 0) {
      next = SWAY_INCREASING;
      *state = next;
    }
  } else {
    objectEvent->y.fixed += ((u32)Import_RandomNext() * 3 << 14) >> 16;
    if (objectEvent->y.fixed > PIXELS(16)) {
      next = SWAY_DECREASING;
      *state = next;
    }
  }
  return 1;
}

/// Shrinks both axes; once small enough, clears the object event's position and
/// speed.
static s32 TolbiShipMast_ShrinkAndStopObjectEvent(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  struct Sprite *sprite = objectEvent->sprite;

  sprite->image.priority = 3;
  objectEvent->speed = PIXELS(2);
  objectEvent->acceleration = PIXELS(1);
  if (objectEvent->scaleX > Q16_ONE / 16) {
    objectEvent->scaleX -= Q16_ONE / 64;
    objectEvent->scaleY -= Q16_ONE / 64;
  } else {
    objectEvent->x.fixed = 0;
    objectEvent->y.fixed = 0;
    objectEvent->z.fixed = 0;
    SET_VELOCITY(objectEvent, 0, 0, 0);
  }
  return 1;
}

/// Returns the warp destinations: from the deck's ladder, and the voyage's
/// cutscenes.
const struct MapWarpDest *TolbiShipMast_GetWarpDests(void) {
  return gTolbiShipMast_WarpDests;
}

/// Returns the ladder's three-dimensional activation region.
const struct MapWarpRegion *TolbiShipMast_GetRegions(void) {
  return gTolbiShipMast_Regions;
}

/// Returns warps to the deck and the continuing ship maps.
const u32 *TolbiShipMast_GetWarps(void) { return gTolbiShipMast_Warps; }

/// Returns the lookout, the only object event placed.
const struct ObjectEventTemplate *TolbiShipMast_GetObjectEventTemplates(void) {
  return gTolbiShipMast_ObjectEventTemplates;
}

/// Returns the ladder, lookout dialogue and Anchor Charm bg events.
const struct MapEvent *TolbiShipMast_GetEvents(void) {
  return gTolbiShipMast_Events;
}

/// Walking south onto the ladder at the mast's top
/// (MB_TOLBI_SHIP_MAST_LADDER): the leader climbs down.
static void TolbiShipMast_ClimbDownLadder(void) { Field_ClimbDown(); }

/// Searching the mast's top for the captain's anchor charm
/// (MB_TOLBI_SHIP_MAST_ANCHOR_CHARM): gives the Anchor Charm once the crewman
/// has been confronted or the warriors have gone to the captain.
static void TolbiShipMast_FindAnchorCharm(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT) != 0 ||
      GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
    FieldScript_Start();
    FieldEffect_StartEffectObject22(ITEM_ANCHOR_CHARM, 3);
    PartyInventory_GiveItem(ITEM_ANCHOR_CHARM, 0);
    GameFlag_Set(FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND);
    FieldScript_End();
  }
}

/// The mast's setup: sways the view while the ship is under way, puts the
/// lookout to his spot once the charm is back, and plays what the warp
/// destination calls for: the climb up from the deck
/// (TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK) or a cutscene (the warp destinations
/// from TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR on).
s32 TolbiShipMast_Initialize(void) {
  struct ObjectEvent *leader;

  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if ((GameFlag_Get(FLAG_TOLBI_SHIP_ROWING) != 0 ||
       GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) &&
      GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
      GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) == 0) {
    gTolbiShipMast_SwayX = (u16)Import_RandomNext();
    gTolbiShipMast_SwayY = (u16)Import_RandomNext();
    FieldScheduler_AddCallback(TolbiShipMast_UpdateSway, TASK_PRIORITY_MAP);
  }
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0 &&
      GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
    ObjectEvent_SetPosition(LOCALID_LOOKOUT, PIXELS(164), PIXELS(328));
  }
  // The ship's scripted warp destinations resume their matching lookout scenes.
  switch (gSaveBlock.warpDest) {
  case TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      FieldScript_Start();
      Field_ClimbUp();
      leader->y.fixed = PIXELS(56);
      FieldCamera_MoveTo(-1, -1, -1, 0);
      WaitFrames(1);
      FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
      MapGrid_CenterOnFollowedPoint();
      WaitFrames(1);
      FieldScript_End();
    }
    break;
  case TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR:
    if (GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
      TolbiShipMast_SetupVisibleLeaderLookout();
    } else {
      TolbiShipMast_SetupHiddenLeaderLookout();
    }
    break;
  case TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK:
    gSaveBlock.savedMap = (s32)&MapId_TolbiShipInteriors;
    gSaveBlock.savedWarpDest = TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT;
    TolbiShipMast_RunAvastScene();
    break;
  case TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK:
    gSaveBlock.savedMap = (s32)&MapId_TolbiShipInteriors;
    gSaveBlock.savedWarpDest = TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT;
    TolbiShipMast_ShowMonsters();
    break;
  case TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK:
    gSaveBlock.savedMap = (s32)&MapId_TolbiShipInteriors;
    gSaveBlock.savedWarpDest = TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT;
    TolbiShipMast_RunLandingSequence();
    break;
  case TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO:
    TolbiShipMast_AnnounceLand();
    break;
  }
  return 0;
}

/// Frames the lookout with the leader moved out of view, then announces course.
static void TolbiShipMast_SetupHiddenLeaderLookout(void) {
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  FieldCamera_GetScriptObject()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(164), PIXELS(64), PIXELS(321), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  TolbiShipMast_AnnounceClearCourse();
  FieldScript_End();
}

/// Keeps the leader visible for the course announcement.
static void TolbiShipMast_SetupVisibleLeaderLookout(void) {
  struct ObjectEvent *leader;

  FieldScript_Start();
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(164), PIXELS(321));
  ObjectEvent_SetSpriteColorById(LOCALID_PARTY_LEADER, 15);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(leader, 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  TolbiShipMast_AnnounceClearCourse();
  FieldScript_End();
}

/// On the mast at TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR: the
/// lookout scans both sides, announces a clear course and leaves by
/// TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_CAST_OFF, to the cast-off below
/// decks.
static void TolbiShipMast_AnnounceClearCourse(void) {
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_LOOKOUT, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_LOOKOUT, 164, 321);
  ObjectEvent_FaceDirection(LOCALID_LOOKOUT, FACING_NORTH + FACING_STEP, 40);
  ObjectEvent_FaceDirection(LOCALID_LOOKOUT, FACING_NORTHWEST + FACING_STEP,
                            40);
  ObjectEvent_FaceDirection(LOCALID_LOOKOUT, FACING_NORTH + FACING_STEP, 40);
  ObjectEvent_FaceDirection(LOCALID_LOOKOUT, FACING_SOUTHEAST + FACING_STEP,
                            10);
  ObjectEvent_WalkToAndWait(LOCALID_LOOKOUT, 164, 334);
  ObjectMotion_Launch(LOCALID_LOOKOUT, 4, 40);
  ObjectMotion_SetVariantCallback(LOCALID_LOOKOUT, 2);
  FieldScript_SetMessageId(MsgTolbiShipShipsCourseClear);
  Script_ShowMessageAndWait(LOCALID_LOOKOUT, 0, 20);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestWarp(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_CAST_OFF);
}

/// On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK, the first
/// attack: three monsters close in and the lookout cries out. "Avast!"
static void TolbiShipMast_RunAvastScene(void) {
  s32 *placement = ((struct MapGridScrollWork *)gMapGridWork[0])->origin;
  struct ObjectEvent *objectEvent;
  struct FieldWork **work;
  s32 x;
  s32 zero = 0;
  s32 scale;
  void (*update)(union FieldObject *);

  FieldScript_Start();
  FieldScript_CallWithLastActiveObjectId(
      gTolbiShipMast_FirstAttackObjectEventTemplates);
  WaitFrames(1);
  ObjectEvent_ConfigureSpriteColor(LOCALID_PARTY_LEADER, 15);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 0);
  ObjectEvent_SetActionScript(LOCALID_LOOKOUT, gTolbiShipMast_LookoutActions);
  work = (struct FieldWork **)(gMapGridWork +
                               (HEAP_SLOT_FIELD - HEAP_SLOT_MAP_GRID));
  (*work)->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  // Save the camera origin before the sparkle begins to turn.
  FieldScript_WaitFrames(20);
  gTolbiShipMast_CameraOrigin[0] = *placement++;
  gTolbiShipMast_CameraOrigin[1] = *placement;
  ObjectEvent_SetPosition(LOCALID_SPARKLE, PIXELS(80), PIXELS(210));
  x = PIXELS(80);
  ObjectEvent_GetById(LOCALID_SPARKLE)->motionFlags = zero;
  gTolbiShipMast_CameraCenter[0] = x;
  gTolbiShipMast_CameraCenter[1] = zero;
  ObjectEvent_SetActionScript(LOCALID_SPARKLE,
                              gTolbiShipMast_FollowCameraActions);
  FieldScript_WaitFrames(20);
  Sound_PlaySong(MUS_029);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_LOOKOUT);
  WaitFrames(1);
  ObjectEvent_ShowEmote(LOCALID_LOOKOUT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_ArmCallback(LOCALID_LOOKOUT, FACING_NORTHWEST + FACING_STEP, 0);
  FieldScript_SetMessageId(MsgTolbiShipAvast);
  FieldScript_ShowMessageThenWait(LOCALID_LOOKOUT, 0, 10);
  // The three monsters appear where the sparkle turned.
  ObjectEvent_SetPosition(LOCALID_FIRST_SIGHTED_MONSTER, x, PIXELS(210));
  ObjectEvent_SetPosition(LOCALID_FIRST_SIGHTED_MONSTER + 1, x, PIXELS(210));
  Import_ObjectEventSetPosition(LOCALID_FIRST_SIGHTED_MONSTER + 2, x,
                                PIXELS(210));
  ObjectMotion_SetActionVariant(LOCALID_FIRST_SIGHTED_MONSTER, 3);
  ObjectMotion_SetActionVariant(LOCALID_FIRST_SIGHTED_MONSTER + 1, 3);
  ObjectMotion_SetActionVariant(LOCALID_FIRST_SIGHTED_MONSTER + 2, 3);
  ObjectEvent_ConfigureSpriteColor(LOCALID_FIRST_SIGHTED_MONSTER, 3);
  ObjectEvent_ConfigureSpriteColor(LOCALID_FIRST_SIGHTED_MONSTER + 1, 3);
  ObjectEvent_ConfigureSpriteColor(LOCALID_FIRST_SIGHTED_MONSTER + 2, 3);
  // Grow the three monsters from half size as they close in on the lookout.
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_SIGHTED_MONSTER);
  scale = Q16_ONE / 2;
  update = TolbiShipMast_GrowObjectEventScale;
  objectEvent->scaleY = scale;
  objectEvent->scaleX = scale;
  objectEvent->update = update;
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_SIGHTED_MONSTER + 1);
  objectEvent->scaleY = scale;
  objectEvent->scaleX = scale;
  objectEvent->update = update;
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_SIGHTED_MONSTER + 2);
  objectEvent->scaleY = scale;
  objectEvent->scaleX = scale;
  objectEvent->update = update;
  WaitFrames(1);
  ObjectEvent_SetSpeed(LOCALID_FIRST_SIGHTED_MONSTER, 13 * PIXELS(1) / 25,
                       13 * PIXELS(1) / 50);
  ObjectEvent_SetSpeed(LOCALID_FIRST_SIGHTED_MONSTER + 1, 9 * PIXELS(1) / 20,
                       9 * PIXELS(1) / 40);
  ObjectEvent_SetSpeed(LOCALID_FIRST_SIGHTED_MONSTER + 2, 3 * PIXELS(1) / 5,
                       3 * PIXELS(1) / 10);
  ObjectEvent_SetDestination(LOCALID_FIRST_SIGHTED_MONSTER, 128, 345);
  ObjectEvent_SetDestination(LOCALID_FIRST_SIGHTED_MONSTER + 1, 136, 330);
  ObjectMotion_ResetAndSetPosition(LOCALID_FIRST_SIGHTED_MONSTER + 2, 156, 340);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_LOOKOUT, 2);
  ObjectEvent_WalkToAndWait(LOCALID_LOOKOUT, 164, 344);
  ObjectMotion_Launch(LOCALID_LOOKOUT, 4, 10);
  ObjectMotion_Launch(LOCALID_LOOKOUT, 6, 40);
  ObjectMotion_SetVariantCallback(LOCALID_LOOKOUT, 3);
  FieldScript_ShowMessageThenWait(LOCALID_LOOKOUT, 0, 20);
  (*work)->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestWarp(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_MONSTERS);
  FieldScript_End();
}

/// Clears the movement left by a bird's drifting script before it swoops.
static inline void
TolbiShipMast_StopDriftingObjectEvent(struct ObjectEvent *objectEvent) {
  objectEvent->targetX = OBJ_EVENT_NO_TARGET;
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
  objectEvent->targetZ = OBJ_EVENT_NO_TARGET;
  SET_VELOCITY(objectEvent, 0, 0, 0);
  objectEvent->riseBaseY = 0;
}

/// Gives each of the seven birds its own random drift and bob.
static inline void TolbiShipMast_InitializeDriftingGroup(void) {
  TolbiShipMast_InitializeDriftingObjectEvent(LOCALID_FIRST_BIRD);
  TolbiShipMast_InitializeDriftingObjectEvent(LOCALID_FIRST_BIRD + 1);
  TolbiShipMast_InitializeDriftingObjectEvent(LOCALID_FIRST_BIRD + 2);
  TolbiShipMast_InitializeDriftingObjectEvent(LOCALID_FIRST_BIRD + 3);
  TolbiShipMast_InitializeDriftingObjectEvent(LOCALID_FIRST_BIRD + 4);
  TolbiShipMast_InitializeDriftingObjectEvent(LOCALID_FIRST_BIRD + 5);
  TolbiShipMast_InitializeDriftingObjectEvent(LOCALID_FIRST_BIRD + 6);
}

/// Stops the birds' drifting actions and gives them their speed to fly off.
static inline void TolbiShipMast_PrepareGroupDeparture(void) {
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_BIRD);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_BIRD + 1);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_BIRD + 2);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_BIRD + 3);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_BIRD + 4);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_BIRD + 5);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_BIRD + 6);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD, PIXELS(3), 3 * PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD + 1, PIXELS(3), 3 * PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD + 2, PIXELS(3), 3 * PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD + 3, PIXELS(3), 3 * PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD + 4, PIXELS(3), 3 * PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD + 5, PIXELS(3), 3 * PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD + 6, PIXELS(3), 3 * PIXELS(1) / 2);
}

/// On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK, the second
/// attack: a bird swoops at the lookout twice, and Sean cries out. "Monsters!"
static void TolbiShipMast_ShowMonsters(void) {
  struct ObjectEvent *approachingObjectEvent;
  struct ObjectEvent *leader;

  FieldScript_Start();
  ObjectEvent_SetSpriteColorById(LOCALID_PARTY_LEADER, 15);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(leader, 0);
  WaitFrames(1);
  FieldScript_CallWithLastActiveObjectId(
      gTolbiShipMast_BirdObjectEventTemplates);
  WaitFrames(1);
  TolbiShipMast_InitializeDriftingGroup();
  ObjectEvent_SetActionScript(LOCALID_LOOKOUT, gTolbiShipMast_LookoutActions);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(120);
  // The first bird swoops at the lookout twice while the rest keep drifting.
  approachingObjectEvent = ObjectEvent_GetById(LOCALID_FIRST_BIRD);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_BIRD);
  TolbiShipMast_StopDriftingObjectEvent(approachingObjectEvent);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD, PIXELS(8), PIXELS(4));
  Object_SetPosition(approachingObjectEvent, PIXELS(164), PIXELS(144),
                     PIXELS(321));
  Object_WaitForMove(approachingObjectEvent);
  Object_SetPosition(approachingObjectEvent, PIXELS(164), PIXELS(104),
                     PIXELS(321));
  Object_WaitForMove(approachingObjectEvent);
  Object_SetPosition(approachingObjectEvent, PIXELS(204), PIXELS(124),
                     PIXELS(321));
  Object_WaitForMove(approachingObjectEvent);
  Object_SetPosition(approachingObjectEvent, PIXELS(144), 0, PIXELS(169));
  ObjectMotion_EnableActionAndResetMotion(LOCALID_LOOKOUT);
  WaitFrames(1);
  ObjectEvent_FaceDirection(LOCALID_LOOKOUT, FACING_WEST, 0);
  ObjectEvent_ShowEmote(LOCALID_LOOKOUT, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD, PIXELS(2), PIXELS(1));
  TolbiShipMast_InitializeDriftingObjectEvent(LOCALID_FIRST_BIRD);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_LOOKOUT, gTolbiShipMast_LookoutActions);
  FieldScript_WaitFrames(120);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_BIRD);
  TolbiShipMast_StopDriftingObjectEvent(approachingObjectEvent);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD, PIXELS(8), PIXELS(4));
  Object_SetPosition(approachingObjectEvent, PIXELS(164), PIXELS(144),
                     PIXELS(321));
  Object_WaitForMove(approachingObjectEvent);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD, PIXELS(5), 5 * PIXELS(1) / 2);
  Object_SetPosition(approachingObjectEvent, PIXELS(164), PIXELS(104),
                     PIXELS(321));
  Object_WaitForMove(approachingObjectEvent);
  Object_SetPosition(approachingObjectEvent, PIXELS(164), PIXELS(114),
                     PIXELS(321));
  Object_WaitForMove(approachingObjectEvent);
  Object_SetPosition(approachingObjectEvent, PIXELS(164), PIXELS(104),
                     PIXELS(321));
  Object_WaitForMove(approachingObjectEvent);
  Object_SetPosition(approachingObjectEvent, PIXELS(204), PIXELS(124),
                     PIXELS(321));
  Object_WaitForMove(approachingObjectEvent);
  Object_SetPosition(approachingObjectEvent, PIXELS(144), 0, PIXELS(169));
  ObjectMotion_EnableActionAndResetMotion(LOCALID_LOOKOUT);
  WaitFrames(1);
  ObjectEvent_FaceDirection(LOCALID_LOOKOUT, FACING_WEST, 0);
  ObjectEvent_ShowEmote(LOCALID_LOOKOUT, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  ObjectEvent_SetSpeed(LOCALID_FIRST_BIRD, PIXELS(2), PIXELS(1));
  TolbiShipMast_InitializeDriftingObjectEvent(LOCALID_FIRST_BIRD);
  // The lookout jumps, the monsters' theme plays, and Sean cries out.
  ObjectMotion_Launch(LOCALID_LOOKOUT, 4, 20);
  ObjectMotion_Launch(LOCALID_LOOKOUT, 6, 40);
  Script_PlaySong(MUS_029);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  FieldScript_SetMessageId(MsgTolbiShipMonsters2);
  Script_ShowMessageAndWait(LOCALID_SEAN, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_LOOKOUT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_WalkToAndWait(LOCALID_LOOKOUT, 164, 344);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_LOOKOUT, 2);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestWarp(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_SECOND_ATTACK);
  FieldScript_End();
}

/// Initializes a drifting object event, retaining the random draws' original
/// order.
static void TolbiShipMast_InitializeDriftingObjectEvent(s32 localId) {
  struct ObjectEvent *objectEvent;
  u32 randomValue;

  objectEvent = ObjectEvent_GetById(localId);
  ObjectMotion_SetActionVariant(localId, 1);
  objectEvent->motionFlags = 0;
  objectEvent->timer = (u32)Import_RandomNext() >> 15;
  objectEvent->work = (u32)Import_RandomNext() >> 15;
  randomValue = Import_RandomNext();
  randomValue <<= 2;
  randomValue >>= 16;
  randomValue <<= 16;
  randomValue += PIXELS(6);
  objectEvent->y.fixed = randomValue;
  randomValue = Import_RandomNext();
  objectEvent->riseBaseY = ((randomValue * 3 << 13) >> 16) - 3 * PIXELS(1) / 16;
  objectEvent->scaleX = 5 * Q16_ONE / 4;
  objectEvent->scaleY = 5 * Q16_ONE / 4;
  ObjectEvent_SetActionScript(localId, gTolbiShipMast_DriftActions);
}

/// On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK, the third
/// attack: the birds scatter, then LOCALID_LANDING_MONSTER drops in beside the
/// lookout.
static void TolbiShipMast_RunLandingSequence(void) {
  u32 frame;
  struct ObjectEvent *objectEvent;

  FieldScript_Start();
  ObjectEvent_SetSpriteColorById(LOCALID_PARTY_LEADER, 15);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 0);
  WaitFrames(1);
  FieldScript_CallWithLastActiveObjectId(
      gTolbiShipMast_BirdObjectEventTemplates);
  WaitFrames(1);
  FieldScript_CallWithLastActiveObjectId(
      gTolbiShipMast_LandingObjectEventTemplates);
  WaitFrames(1);
  TolbiShipMast_InitializeDriftingGroup();
  ObjectEvent_SetActionScript(LOCALID_LOOKOUT, gTolbiShipMast_LookoutActions);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(300);
  Script_PlaySong(SE_147);
  FieldScript_WaitFrames(100);
  // The birds fly off before the monster lands.
  TolbiShipMast_PrepareGroupDeparture();
  ObjectEvent_SetDestination(LOCALID_FIRST_BIRD, 0, 100);
  ObjectEvent_SetDestination(LOCALID_FIRST_BIRD + 1, 60, 100);
  ObjectEvent_SetDestination(LOCALID_FIRST_BIRD + 2, 120, 100);
  ObjectEvent_SetDestination(LOCALID_FIRST_BIRD + 3, 180, 100);
  ObjectEvent_SetDestination(LOCALID_FIRST_BIRD + 4, 240, 100);
  ObjectEvent_SetDestination(LOCALID_FIRST_BIRD + 5, 320, 100);
  ObjectEvent_SetDestination(LOCALID_FIRST_BIRD + 6, 380, 100);
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(LOCALID_LOOKOUT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetPosition(LOCALID_FIRST_BIRD, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_BIRD + 1, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_BIRD + 2, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_BIRD + 3, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_BIRD + 4, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_BIRD + 5, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_BIRD + 6, 0, 0);
  FieldScript_WaitFrames(100);
  // LOCALID_LANDING_MONSTER_SHADOW grows where the monster will land, then
  // LOCALID_LANDING_MONSTER drops onto it.
  objectEvent = ObjectEvent_GetById(LOCALID_LANDING_MONSTER_SHADOW);
  objectEvent->scaleX = Q16_ONE / 10;
  objectEvent->scaleY = Q16_ONE / 10;
  ObjectEvent_SetPosition(LOCALID_LANDING_MONSTER_SHADOW, PIXELS(172),
                          PIXELS(340));
  ObjectMotion_EnableActionAndResetMotion(LOCALID_LOOKOUT);
  WaitFrames(1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_LOOKOUT, 1);
  ObjectEvent_FaceDirection(LOCALID_LOOKOUT, FACING_SOUTHEAST + FACING_STEP, 0);
  Script_PlaySong(MUS_029);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  // LOCALID_LANDING_MONSTER_SHADOW grows for 32 frames before
  // LOCALID_LANDING_MONSTER is placed above it.
  for (frame = 0; frame < 32; frame++) {
    objectEvent->scaleX += Q16_ONE / 20;
    objectEvent->scaleY += Q16_ONE / 20;
    WaitFrames(1);
  }
  ObjectEvent_ShowEmote(LOCALID_LOOKOUT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_LOOKOUT, 2);
  ObjectEvent_WalkToAndWait(LOCALID_LOOKOUT, 168, 340);
  ObjectEvent_WalkToAndWait(LOCALID_LOOKOUT, 200, 340);
  ObjectEvent_FaceDirection(LOCALID_LOOKOUT, FACING_WEST, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_LANDING_MONSTER);
  objectEvent->scaleX = 23 * Q16_ONE / 20;
  objectEvent->scaleY = 23 * Q16_ONE / 20;
  objectEvent->x.fixed = PIXELS(172);
  objectEvent->y.fixed = PIXELS(160);
  objectEvent->z.fixed = PIXELS(340);
  objectEvent->facing = 0;
  objectEvent->bounce = 2 * Q16_ONE / 5;
  objectEvent->gravity = PIXELS(3);
  FieldScript_WaitFrames(20);
  ObjectMotion_Launch(LOCALID_LOOKOUT, 6, 20);
  Script_PlaySong(SE_147);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_LOOKOUT, gTolbiShipMast_WaveActions);
  FieldScript_WaitFrames(80);
  // Let the monster bounce, then move it and its landing spot out of the
  // shot.
  ObjectMotion_SetActionVariant(LOCALID_LANDING_MONSTER, 1);
  ObjectEvent_SetSpeed(LOCALID_LANDING_MONSTER, PIXELS(1), PIXELS(1) / 2);
  objectEvent->bounce = Q16_ONE / 10;
  objectEvent->gravity = 7 * PIXELS(1) / 10;
  Script_PlaySong(SE_HIGH_JUMP);
  objectEvent->velocityY = PIXELS(8);
  ObjectEvent_SetDestination(LOCALID_LANDING_MONSTER, 132, 360);
  ObjectEvent_SetDestination(LOCALID_LANDING_MONSTER_SHADOW, 132, 360);
  FieldScript_WaitFrames(40);
  ObjectEvent_SetPosition(LOCALID_LANDING_MONSTER, 0, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_LOOKOUT);
  objectEvent->scaleX = Q16_ONE;
  objectEvent->scaleY = Q16_ONE;
  objectEvent->facing = FACING_SOUTH + FACING_STEP;
  FieldScript_WaitFrames(40);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestWarp(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_THIRD_ATTACK);
  FieldScript_End();
}

/// On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO: the seven birds fly
/// off, then the lookout announces land, and the rowing balance chooses the
/// warp.
static void TolbiShipMast_AnnounceLand(void) {
  FieldScript_Start();
  ObjectEvent_SetSpriteColorById(LOCALID_PARTY_LEADER, 15);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 0);
  FieldScript_CallWithLastActiveObjectId(
      gTolbiShipMast_BirdObjectEventTemplates);
  WaitFrames(1);
  TolbiShipMast_InitializeDriftingGroup();
  ObjectEvent_SetActionScript(LOCALID_LOOKOUT, gTolbiShipMast_LookoutActions);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(400);
  // Each bird takes its own way off while shrinking out of view.
  TolbiShipMast_PrepareGroupDeparture();
  ObjectEvent_SetActionScript(LOCALID_FIRST_BIRD,
                              gTolbiShipMast_BirdDeparture0);
  ObjectEvent_SetActionScript(LOCALID_FIRST_BIRD + 1,
                              gTolbiShipMast_BirdDeparture1);
  ObjectEvent_SetActionScript(LOCALID_FIRST_BIRD + 2,
                              gTolbiShipMast_BirdDeparture2);
  ObjectEvent_SetActionScript(LOCALID_FIRST_BIRD + 3,
                              gTolbiShipMast_BirdDeparture3);
  ObjectEvent_SetActionScript(LOCALID_FIRST_BIRD + 4,
                              gTolbiShipMast_BirdDeparture4);
  ObjectEvent_SetActionScript(LOCALID_FIRST_BIRD + 5,
                              gTolbiShipMast_BirdDeparture5);
  ObjectEvent_SetActionScript(LOCALID_FIRST_BIRD + 6,
                              gTolbiShipMast_BirdDeparture6);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_LOOKOUT, 3);
  ObjectEvent_SetAttachedEffect(LOCALID_LOOKOUT, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(120);
  ObjectMotion_SetVariantCallback(LOCALID_LOOKOUT, 1);
  ObjectEvent_ShowEmote(LOCALID_LOOKOUT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        60);
  ObjectEvent_SetSpeed(LOCALID_LOOKOUT, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_LOOKOUT, 164, 344);
  ObjectMotion_Launch(LOCALID_LOOKOUT, 4, 10);
  ObjectMotion_Launch(LOCALID_LOOKOUT, 6, 20);
  FieldScript_SetMessageId(MsgTolbiShipLandHo);
  Script_ShowMessageAndWait(LOCALID_LOOKOUT, 0, 20);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  // The rowing balance picks where the ship comes in: exactly 11 takes
  // TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE, off course to the island.
  gSaveBlock.savedMap = (s32)&MapId_TolbiShipInteriors;
  gSaveBlock.savedWarpDest =
      TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_ROWING_DECK;
  if (TolbiShipMast_GetRowingBalance() == 11) {
    Import_ScriptRequestWarp(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE);
  } else {
    Import_ScriptRequestWarp(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED);
  }
  FieldScript_End();
}

/// Adds horizontal and vertical sway to the camera origin each frame.
static void TolbiShipMast_UpdateSway(void) {
  s32 *position = ((struct MapGridScrollWork *)gMapGridWork[0])->origin;
  s32 swayX = Trig_Cos(gTolbiShipMast_SwayX);
  s32 swayY = Trig_Sin(gTolbiShipMast_SwayY);

  *position++ += swayX;
  swayY *= 4;
  *position += swayY;
  gTolbiShipMast_DriftX += swayX;
  gTolbiShipMast_DriftY += swayY;
  gTolbiShipMast_SwayX += (u32)(Import_RandomNext() * 3 << 7) >> 16;
  gTolbiShipMast_SwayY += (u32)(Import_RandomNext() << 9) >> 16;
  gTolbiShipMast_SwayX &= 0xffff;
  gTolbiShipMast_SwayY &= 0xffff;
}

/// The rowing balance of the chosen passengers: the values of those at the west
/// seats (the first and third filled) less those at the east seats.
static s32 TolbiShipMast_GetRowingBalance(void) {
  s32 westSeats;
  s32 eastSeats;

  westSeats = TolbiShipMast_GetSeatRowValue(0);
  westSeats += TolbiShipMast_GetSeatRowValue(2);
  eastSeats = TolbiShipMast_GetSeatRowValue(1);
  eastSeats += TolbiShipMast_GetSeatRowValue(3);
  return westSeats - eastSeats;
}

/// The rowing value of the passenger chosen for a seat, or 0 while it is empty.
static s32 TolbiShipMast_GetSeatRowValue(u32 seat) {
  s32 firstFlag = 0;
  u32 passenger;

  switch (seat) {
  case 0:
    firstFlag = FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS;
    break;
  case 1:
    firstFlag = FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS;
    break;
  case 2:
    firstFlag = FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS;
    break;
  case 3:
    firstFlag = FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS;
    break;
  }
  for (passenger = 0; passenger <= 8; passenger++) {
    if (GameFlag_Get(firstFlag + passenger) != 0)
      return gTolbiShipMast_PassengerRowValues[passenger];
  }
  return 0;
}

s32 gTolbiShipMast_FollowCameraActions[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 32),
    SC_CALL(TolbiShipMast_FollowCameraAndRotate),
    SC_WAIT(1),
    SC_LOOP(10, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_CALL(TolbiShipMast_FollowCameraAndRotate),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gTolbiShipMast_LookoutActions[] = {
    SC_CALL(TolbiShipMast_RandomizeLookoutFacing),
};

s32 gTolbiShipMast_DriftActions[] = {
    SC_CALL(TolbiShipMast_SwayObjectEvent),
};

s32 gTolbiShipMast_WaveActions[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_ADD(SW_SCALE_X, -5 * Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 16),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gTolbiShipMast_BirdDeparture0[] = {
    SC_MOVE(PIXELS(72), 0, PIXELS(216)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopObjectEvent),
    SC_END,
};

s32 gTolbiShipMast_BirdDeparture1[] = {
    SC_MOVE(PIXELS(132), 0, PIXELS(242)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopObjectEvent),
    SC_END,
};

s32 gTolbiShipMast_BirdDeparture2[] = {
    SC_MOVE(PIXELS(88), 0, PIXELS(166)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopObjectEvent),
    SC_END,
};

s32 gTolbiShipMast_BirdDeparture3[] = {
    SC_MOVE(PIXELS(220), 0, PIXELS(134)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopObjectEvent),
    SC_END,
};

s32 gTolbiShipMast_BirdDeparture4[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(228)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopObjectEvent),
    SC_END,
};

s32 gTolbiShipMast_BirdDeparture5[] = {
    SC_MOVE(PIXELS(282), 0, PIXELS(168)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopObjectEvent),
    SC_END,
};

// clang-format off
u32 gTolbiShipMast_BirdDeparture6[] = {
    SC_MOVE(PIXELS(154), 0, PIXELS(135)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopObjectEvent),
    SC_END,
    // Unidentified words following the script terminator.
    0x9090000, 0xd70090d, 0xdb315cb, 0x15f5164c,
    0x1f0e1aad, 0x1f6f227a, 0x23f223b1, 0x2bf727f4,
};

struct MapWarpDest gTolbiShipMast_WarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 165, 0, 235, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 184, 56, 352, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct MapWarpRegion gTolbiShipMast_Regions[] = {
    {176, 16, 348, 192, 32, 364, -1, 1},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gTolbiShipMast_Warps[] = {
    MAP_WARPS(TolbiShipMast),
    MAP_WARP(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST),
    MAP_WARP(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_CAST_OFF, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF),
    MAP_WARP(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_MONSTERS, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS),
    MAP_WARP(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_SECOND_ATTACK, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK),
    MAP_WARP(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_THIRD_ATTACK, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK),
    MAP_WARP(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED),
    MAP_WARP(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gTolbiShipMast_ObjectEventTemplates[] = {
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gTolbiShipMast_FirstAttackObjectEventTemplates[] = {
    { 0x11e, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gTolbiShipMast_BirdObjectEventTemplates[] = {
    { SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(144), 0, PIXELS(169), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(134), 0, PIXELS(189), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(124), 0, PIXELS(209), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(129), 0, PIXELS(174), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(139), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(149), 0, PIXELS(204), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(154), 0, PIXELS(194), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(182), 0, PIXELS(420), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gTolbiShipMast_LandingObjectEventTemplates[] = {
    { SPRITE_BLUE_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gTolbiShipMast_Events[] = {
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), MB_TOLBI_SHIP_MAST_LADDER, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipMast_ClimbDownLadder) },
    { MAP_EVENT_TALK, LOCALID_LOOKOUT, FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF, MSGNO(MsgTolbiShipWatchSeasHelpKaja) },
    { MAP_EVENT_TALK, LOCALID_LOOKOUT, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipLookoutCannotRow) },
    { MAP_EVENT_TALK, LOCALID_LOOKOUT, CONDITION_ALWAYS, MSGNO(MsgTolbiShipMonstersAnywhereAroundHere) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LOOKOUT, FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF, MSGNO(MsgTolbiShipHiddenAnchorDiscovered) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LOOKOUT, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipSeaTooVast) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LOOKOUT, CONDITION_ALWAYS, MSGNO(MsgTolbiShipIslandTreasures) },
    { BG_EVENT(BG_EVENT_SILENT), MB_TOLBI_SHIP_MAST_ANCHOR_CHARM, FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND, EVENT_SCRIPT(TolbiShipMast_FindAnchorCharm) },
    { MAP_EVENTS_END },
};

/// Each passenger's rowing value, in the order of enum TolbiShipPassenger
/// (tolbi_ship_interiors.c).
u32 gTolbiShipMast_PassengerRowValues[] = {
    7, 5, 8, 4, 9, 12, 6, 3,
    0,
};
// clang-format on
