/// @file
/// Tolbi-bound Ship (SceneId_TolbiShipMast), the top of its mast, where the
/// ship's lookout (ACTOR_LOOKOUT) keeps watch: one of the ship's three scenes
/// on its voyage from Kalay to Tolbi, which tolbi_ship_deck.c describes. The
/// party climbs up by the deck's ladder (TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK)
/// and down again by CELL_BEHAVIOR_TOLBI_SHIP_MAST_LADDER and the region of
/// TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK. The entrances from
/// TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR on are the voyage's
/// events here, each leading on into the next; during the attacks the
/// restart point (gSequenceParentWork.restartScene) is set to
/// SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT, which starts the voyage
/// over. While the ship is under way (rowing, or at sea after the first
/// attack, and neither at the island nor at Tolbi) the setup sways the view.
/// In story order:
///
/// - At Kalay, while the ship waits for its replacement: the captain's anchor
///   charm, gone from his quarters, lies up here. Searching for it
///   (CELL_BEHAVIOR_TOLBI_SHIP_MAST_ANCHOR_CHARM) finds the Anchor Charm
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
///   close in (FLAG_TOLBI_SHIP_MONSTERS_SIGHTED, which plays
///   MUS_CONFRONTATION); Sean's "Monsters!" after a bird swoops at the lookout
///   (TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK); and a monster dropping in
///   beside the lookout (TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK). Each
///   leads below or to the deck.
/// - Land: at TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO the seven birds
///   (ACTOR_FIRST_BIRD) fly away before "Land ho!". The rowing values of the
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

#include "script_operands.h"
#include "sequence_parent_work.h"
#include "scene_ids.h"
#include "module/import.h"
#include "module/scene/tolbi_ship_mast.h"
#include "module/overworld/overworld.h"
#include "module/field/field.h"
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
#include "heap_slots.h"
#include "item_ids.h"
#include "flags.h"
#include "sounds.h"
#include "types.h"
#include "event.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "scene_tables.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "map_grid_scroll.h"
#include "heap.h"
#include "macros.h"

// Events and the mast's per-frame updates.
static void TolbiShipMast_SetupHiddenLeaderLookout(void);
static void TolbiShipMast_SetupVisibleLeaderLookout(void);
static void TolbiShipMast_AnnounceClearCourse(void);
static void TolbiShipMast_RunAvastEvent(void);
static void TolbiShipMast_ShowMonsters(void);
static void TolbiShipMast_InitializeDriftingActor(s32 actorId);
static void TolbiShipMast_RunMonsterLanding(void);
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

/// The scene's one actor placement row.
enum TolbiShipMastActorId {
  /// The ship's lookout, a man in a blue bandana
  /// (SPRITE_MAN_IN_BLUE_BANDANA). He is the crewman who hid the captain's
  /// anchor charm (ACTOR_KALAY_CREWMAN on deck, ACTOR_QUARTERS_CREWMAN
  /// below), in the
  /// same sprite: Mind Read on him at Kalay hears "I was the one who hid the
  /// anchor" (MsgTolbiShipHiddenAnchorDiscovered).
  /// ★ Named from the game: "No, no, I'm the lookout."
  /// (MsgTolbiShipLookoutCannotRow)
  ACTOR_LOOKOUT = ACTOR_FIRST_PLACED
};

/// The first attack's actors
/// (TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK), placed after the lookout from
/// gTolbiShipMast_FirstAttackActorPlacements.
enum TolbiShipMastFirstAttackActorId {
  /// A sparkle (graphics/sprite/spr_286.png, a four-pointed sparkle in eight
  /// shrinking frames), placed nowhere else. It turns, swells for 10 frames and
  /// shrinks for 24 (gTolbiShipMast_FollowCameraActions) where the monsters
  /// then appear.
  /// ★ Named from what the player sees: a white four-pointed sparkle
  /// (its render)
  ACTOR_SPARKLE = ACTOR_LOOKOUT + 1,
  /// The first of three monsters, all with the sprite of the first attack's
  /// monsters below decks (gShipRooms_FirstAttackMonsters). They grow
  /// from half size as they close in.
  /// ★ Named from the game: "Monsters sighted!" (MsgTolbiShipAvast)
  ACTOR_FIRST_SIGHTED_MONSTER
};

/// The actors of the later events (from
/// TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK on), placed after the lookout
/// from gTolbiShipMast_BirdActorPlacements and, at
/// TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK, then from
/// gTolbiShipMast_LandingActorPlacements.
enum TolbiShipMastEnsembleActorId {
  /// The first of seven birds, all of one sprite (SPRITE_BIRD, a
  /// white bird in three wing poses), that flutter and bob about the mast
  /// (TolbiShipMast_SwayActor). One swoops at the lookout at the second
  /// attack,
  /// they scatter as the third attack's monster comes, and they fly off just
  /// before "Land ho!".
  /// ★ Named from what the player sees: a white bird beating its wings (its
  /// render)
  ACTOR_FIRST_BIRD = ACTOR_LOOKOUT + 1,
  /// Sean, by the sprite he has in the hold (ACTOR_HOLD_SEAN,
  /// ship_rooms.c)
  /// and on deck (ACTOR_SEA_SEAN, tolbi_ship_deck.c). He cries "Monsters!" at
  /// the
  /// second attack.
  /// ★ Named from the game: "That'll be me, Ouranos, and Sean on deck"
  /// (MsgTolbiShipOurReplacementNeverArrivedBut)
  ACTOR_SEAN = ACTOR_FIRST_BIRD + 7,
  /// The sprite of the third attack's monster below decks
  /// (gShipRooms_ThirdAttackMonsters); it drops in beside the lookout.
  /// ★ Named from what the player sees: a blue winged monster (its render)
  ACTOR_LANDING_MONSTER,
  /// The landing monster's shadow: a blank sprite (SPRITE_BLANK) draws only
  /// its shadow, which grows from a tenth of its size where
  /// ACTOR_LANDING_MONSTER drops, then follows the monster away
  /// (tolbi_ship_deck.c's ACTOR_SEA_FALLING_MONSTER_SHADOW does the same).
  /// ★ Named from what the player sees: a black shadow that grows on the
  /// crow's nest where the monster lands (its render)
  ACTOR_LANDING_MONSTER_SHADOW
};

/// The mast's step trigger and examine trigger cells.
enum TolbiShipMastCellBehavior {
  CELL_BEHAVIOR_TOLBI_SHIP_MAST_LADDER =
      10, ///< the top of the ladder down to the deck
  /// Where the crewman hid the captain's anchor charm.
  /// ★ Named from the game: "I hid it on top of the mast."
  /// (MsgTolbiShipInteriorsIWaitedTooLongIllGet)
  CELL_BEHAVIOR_TOLBI_SHIP_MAST_ANCHOR_CHARM = 11
};

/// Keeps the effect actor relative to the saved camera origin and
/// rotates it.
static s32 TolbiShipMast_FollowCameraAndRotate(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  s32 *position = ((struct MapGridScrollWork *)gMapGridWork[0])->origin;
  s32 *origin = gTolbiShipMast_CameraOrigin;
  s32 *center = gTolbiShipMast_CameraCenter;
  s32 cameraX = *position++;
  s32 cameraY = *position;

  actor->x.fixed = center[0] + (cameraX - origin[0]);
  actor->y.fixed = center[1] + (cameraY - origin[1]) / 2;
  actor->sprite->rotation += 0x600;
  return 0;
}

/// A random sample of 6 or 9 turns the lookout to either side of north.
static s32 TolbiShipMast_RandomizeLookoutFacing(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  u32 sample = (u32)(Import_RandomNext() << 6) >> 16;

  if (sample == 6) {
    s32 value = FACING_NORTH + FACING_STEP;

    actor->facing = value;
  } else if (sample == 9) {
    s32 value = FACING_NORTHWEST + FACING_STEP;

    actor->facing = value;
  }

  return 1;
}

/// Grows both axes by the same amount until the actor reaches full
/// scale.
static void TolbiShipMast_GrowActorScale(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;
  if (actor->scaleX < Q16_ONE) {
    actor->scaleX += 160;
    actor->scaleY += 160;
  }
}

/// Which way TolbiShipMast_SwayActor moves an actor along each
/// axis, kept in its timer (the drift) and its work (the bob).
enum TolbiShipMastSway {
  SWAY_INCREASING = 0, ///< the drift speeding up, or the bob rising
  SWAY_DECREASING = 1, ///< the drift slowing, or the bob sinking
  SWAY_HELD = 9        ///< held at zero; nothing here sets it
};

/// Randomly reverses horizontal drift and vertical bobbing at their limits.
static s32 TolbiShipMast_SwayActor(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  s16 *state = &actor->timer;
  s32 next;

  if (*state == SWAY_HELD) {
    actor->riseBaseY = 0;
  } else if (*state != SWAY_INCREASING) {
    actor->riseBaseY -= (u32)(Import_RandomNext() << 11) >> 16;
    if (actor->riseBaseY < -3 * PIXELS(1) / 4) {
      next = SWAY_INCREASING;
      *state = next;
    }
  } else {
    actor->riseBaseY += (u32)(Import_RandomNext() << 11) >> 16;
    if (actor->riseBaseY > 3 * PIXELS(1) / 4) {
      next = SWAY_DECREASING;
      *state = next;
    }
  }
  if (actor->x.fixed > PIXELS(40) && actor->x.fixed < PIXELS(320)) {
    actor->x.fixed += actor->riseBaseY;
  }
  state = (s16 *)&actor->userValue;
  if (*state == SWAY_HELD) {
    actor->y.fixed = 0;
  } else if (*state != SWAY_INCREASING) {
    actor->y.fixed -= ((u32)Import_RandomNext() * 3 << 14) >> 16;
    if (actor->y.fixed < 0) {
      next = SWAY_INCREASING;
      *state = next;
    }
  } else {
    actor->y.fixed += ((u32)Import_RandomNext() * 3 << 14) >> 16;
    if (actor->y.fixed > PIXELS(16)) {
      next = SWAY_DECREASING;
      *state = next;
    }
  }
  return 1;
}

/// Shrinks both axes; once small enough, clears the actor's position and
/// speed.
static s32 TolbiShipMast_ShrinkAndStopActor(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  struct Sprite *sprite = actor->sprite;

  sprite->image.priority = 3;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1);
  if (actor->scaleX > Q16_ONE / 16) {
    actor->scaleX -= Q16_ONE / 64;
    actor->scaleY -= Q16_ONE / 64;
  } else {
    actor->x.fixed = 0;
    actor->y.fixed = 0;
    actor->z.fixed = 0;
    SET_VELOCITY(actor, 0, 0, 0);
  }
  return 1;
}

/// Returns the entrances: from the deck's ladder, and the voyage's
/// events.
const struct SceneEntrance *TolbiShipMast_GetEntrances(void) {
  return gTolbiShipMast_Entrances;
}

/// Returns the ladder's three-dimensional activation region.
const struct SceneExitRegion *TolbiShipMast_GetRegions(void) {
  return gTolbiShipMast_Regions;
}

/// Returns exits to the deck and the continuing ship scenes.
const u32 *TolbiShipMast_GetExits(void) { return gTolbiShipMast_Exits; }

/// Returns the lookout, the only actor placed.
const struct ActorPlacement *TolbiShipMast_GetActorPlacements(void) {
  return gTolbiShipMast_ActorPlacements;
}

/// Returns the ladder, lookout dialogue and Anchor Charm examine triggers.
const struct Trigger *TolbiShipMast_GetTriggers(void) {
  return gTolbiShipMast_Triggers;
}

/// Walking south onto the ladder at the mast's top
/// (CELL_BEHAVIOR_TOLBI_SHIP_MAST_LADDER): the leader climbs down.
static void TolbiShipMast_ClimbDownLadder(void) { Field_ClimbDown(); }

/// Searching the mast's top for the captain's anchor charm
/// (CELL_BEHAVIOR_TOLBI_SHIP_MAST_ANCHOR_CHARM): gives the Anchor Charm once
/// the crewman has been confronted or the warriors have gone to the captain.
static void TolbiShipMast_FindAnchorCharm(void) {
  if (GameFlag_Get(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT) != 0 ||
      GameFlag_Get(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
    Event_Start();
    ItemDisplay_ShowItem(ITEM_ANCHOR_CHARM, 3);
    PartyInventory_GiveItem(ITEM_ANCHOR_CHARM, 0);
    GameFlag_Set(FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND);
    Event_End();
  }
}

/// The mast's setup: sways the view while the ship is under way, puts the
/// lookout to his spot once the charm is back, and plays what the
/// entrance calls for: the climb up from the deck
/// (TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK) or an event (the entrances
/// from TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR on).
s32 TolbiShipMast_Initialize(void) {
  struct Actor *leader;

  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if ((GameFlag_Get(FLAG_TOLBI_SHIP_ROWING) != 0 ||
       GameFlag_Get(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) &&
      GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
      GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) == 0) {
    gTolbiShipMast_SwayX = (u16)Import_RandomNext();
    gTolbiShipMast_SwayY = (u16)Import_RandomNext();
    Task_AddFrame(TolbiShipMast_UpdateSway, TASK_PASS_FRAME);
  }
  if (GameFlag_Get(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0 &&
      GameFlag_Get(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
    Actor_SetPosition(ACTOR_LOOKOUT, PIXELS(164), PIXELS(328));
  }
  // The ship's scripted entrances resume their matching lookout events.
  switch (gSequenceParentWork.entrance) {
  case TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK:
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      Event_Start();
      Field_ClimbUp();
      leader->y.fixed = PIXELS(56);
      FieldCamera_MoveTo(-1, -1, -1, 0);
      wait(1);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      MapGrid_CenterOnFollowedPoint();
      wait(1);
      Event_End();
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
    gSequenceParentWork.restartScene = (s32)&SceneId_ShipRooms;
    gSequenceParentWork.restartEntrance =
        SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT;
    TolbiShipMast_RunAvastEvent();
    break;
  case TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK:
    gSequenceParentWork.restartScene = (s32)&SceneId_ShipRooms;
    gSequenceParentWork.restartEntrance =
        SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT;
    TolbiShipMast_ShowMonsters();
    break;
  case TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK:
    gSequenceParentWork.restartScene = (s32)&SceneId_ShipRooms;
    gSequenceParentWork.restartEntrance =
        SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT;
    TolbiShipMast_RunMonsterLanding();
    break;
  case TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO:
    TolbiShipMast_AnnounceLand();
    break;
  }
  return 0;
}

/// Frames the lookout with the leader moved out of view, then announces course.
static void TolbiShipMast_SetupHiddenLeaderLookout(void) {
  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  wait(1);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(164), PIXELS(64), PIXELS(321), 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  TolbiShipMast_AnnounceClearCourse();
  Event_End();
}

/// Keeps the leader visible for the course announcement.
static void TolbiShipMast_SetupVisibleLeaderLookout(void) {
  struct Actor *leader;

  Event_Start();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(164), PIXELS(321));
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(leader, 0);
  wait(1);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  TolbiShipMast_AnnounceClearCourse();
  Event_End();
}

/// On the mast at TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR: the
/// lookout scans both sides, announces a clear course and leaves by
/// TOLBI_SHIP_MAST_TO_SHIP_ROOMS_CAST_OFF, to the cast-off below
/// decks.
static void TolbiShipMast_AnnounceClearCourse(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_LOOKOUT, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 164, 321);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 164, 334);
  Actor_Jump(ACTOR_LOOKOUT, 4, 40);
  Actor_StartRepeatedMotion(ACTOR_LOOKOUT, 2);
  Event_SetMessageId(MsgTolbiShipShipsCourseClear);
  FieldEvent_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_MAST_TO_SHIP_ROOMS_CAST_OFF);
}

/// On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK, the first
/// attack: three monsters close in and the lookout cries out. "Avast!"
static void TolbiShipMast_RunAvastEvent(void) {
  s32 *placement = ((struct MapGridScrollWork *)gMapGridWork[0])->origin;
  struct Actor *actor;
  struct FieldWork **work;
  s32 x;
  s32 zero = 0;
  s32 scale;
  void (*update)(struct Actor *);

  Event_Start();
  Event_CallWithLastActiveActorId(gTolbiShipMast_FirstAttackActorPlacements);
  wait(1);
  Actor_ConfigureSpriteColor(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 0);
  Actor_SetScript(ACTOR_LOOKOUT, gTolbiShipMast_LookoutActions);
  work = (struct FieldWork **)(gMapGridWork +
                               (HEAP_SLOT_FIELD - HEAP_SLOT_MAP_GRID));
  (*work)->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  // Save the camera origin before the sparkle begins to turn.
  Event_Wait(20);
  gTolbiShipMast_CameraOrigin[0] = *placement++;
  gTolbiShipMast_CameraOrigin[1] = *placement;
  Actor_SetPosition(ACTOR_SPARKLE, PIXELS(80), PIXELS(210));
  x = PIXELS(80);
  Actor_GetById(ACTOR_SPARKLE)->motionFlags = zero;
  gTolbiShipMast_CameraCenter[0] = x;
  gTolbiShipMast_CameraCenter[1] = zero;
  Actor_SetScript(ACTOR_SPARKLE, gTolbiShipMast_FollowCameraActions);
  Event_Wait(20);
  Sound_Play(MUS_CONFRONTATION);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  Actor_Stop(ACTOR_LOOKOUT);
  wait(1);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ArmCallback(ACTOR_LOOKOUT, FACING_NORTHWEST + FACING_STEP, 0);
  Event_SetMessageId(MsgTolbiShipAvast);
  Event_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 10);
  // The three monsters appear where the sparkle turned.
  Actor_SetPosition(ACTOR_FIRST_SIGHTED_MONSTER, x, PIXELS(210));
  Actor_SetPosition(ACTOR_FIRST_SIGHTED_MONSTER + 1, x, PIXELS(210));
  Import_ActorSetPosition(ACTOR_FIRST_SIGHTED_MONSTER + 2, x, PIXELS(210));
  Actor_SetSpritePriority(ACTOR_FIRST_SIGHTED_MONSTER, 3);
  Actor_SetSpritePriority(ACTOR_FIRST_SIGHTED_MONSTER + 1, 3);
  Actor_SetSpritePriority(ACTOR_FIRST_SIGHTED_MONSTER + 2, 3);
  Actor_ConfigureSpriteColor(ACTOR_FIRST_SIGHTED_MONSTER, SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_FIRST_SIGHTED_MONSTER + 1, SPRITE_COLOR_BGR);
  Actor_ConfigureSpriteColor(ACTOR_FIRST_SIGHTED_MONSTER + 2, SPRITE_COLOR_BGR);
  // Grow the three monsters from half size as they close in on the lookout.
  actor = Actor_GetById(ACTOR_FIRST_SIGHTED_MONSTER);
  scale = Q16_ONE / 2;
  update = TolbiShipMast_GrowActorScale;
  actor->scaleY = scale;
  actor->scaleX = scale;
  actor->update = update;
  actor = Actor_GetById(ACTOR_FIRST_SIGHTED_MONSTER + 1);
  actor->scaleY = scale;
  actor->scaleX = scale;
  actor->update = update;
  actor = Actor_GetById(ACTOR_FIRST_SIGHTED_MONSTER + 2);
  actor->scaleY = scale;
  actor->scaleX = scale;
  actor->update = update;
  wait(1);
  Actor_SetSpeed(ACTOR_FIRST_SIGHTED_MONSTER, 13 * PIXELS(1) / 25,
                 13 * PIXELS(1) / 50);
  Actor_SetSpeed(ACTOR_FIRST_SIGHTED_MONSTER + 1, 9 * PIXELS(1) / 20,
                 9 * PIXELS(1) / 40);
  Actor_SetSpeed(ACTOR_FIRST_SIGHTED_MONSTER + 2, 3 * PIXELS(1) / 5,
                 3 * PIXELS(1) / 10);
  Actor_SetDestination(ACTOR_FIRST_SIGHTED_MONSTER, 128, 345);
  Actor_SetDestination(ACTOR_FIRST_SIGHTED_MONSTER + 1, 136, 330);
  Actor_ResetAndSetPosition(ACTOR_FIRST_SIGHTED_MONSTER + 2, 156, 340);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_LOOKOUT, 2);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 164, 344);
  Actor_Jump(ACTOR_LOOKOUT, 4, 10);
  Actor_Jump(ACTOR_LOOKOUT, 6, 40);
  Actor_StartRepeatedMotion(ACTOR_LOOKOUT, 3);
  Event_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 20);
  (*work)->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_MAST_TO_SHIP_ROOMS_MONSTERS);
  Event_End();
}

/// Clears the movement left by a bird's drifting script before it swoops.
static inline void TolbiShipMast_StopDriftingActor(struct Actor *actor) {
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetY = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  SET_VELOCITY(actor, 0, 0, 0);
  actor->riseBaseY = 0;
}

/// Gives each of the seven birds its own random drift and bob.
static inline void TolbiShipMast_InitializeDriftingGroup(void) {
  TolbiShipMast_InitializeDriftingActor(ACTOR_FIRST_BIRD);
  TolbiShipMast_InitializeDriftingActor(ACTOR_FIRST_BIRD + 1);
  TolbiShipMast_InitializeDriftingActor(ACTOR_FIRST_BIRD + 2);
  TolbiShipMast_InitializeDriftingActor(ACTOR_FIRST_BIRD + 3);
  TolbiShipMast_InitializeDriftingActor(ACTOR_FIRST_BIRD + 4);
  TolbiShipMast_InitializeDriftingActor(ACTOR_FIRST_BIRD + 5);
  TolbiShipMast_InitializeDriftingActor(ACTOR_FIRST_BIRD + 6);
}

/// Stops the birds' drifting actor scripts and gives them their speed to fly
/// off.
static inline void TolbiShipMast_PrepareGroupDeparture(void) {
  Actor_Stop(ACTOR_FIRST_BIRD);
  Actor_Stop(ACTOR_FIRST_BIRD + 1);
  Actor_Stop(ACTOR_FIRST_BIRD + 2);
  Actor_Stop(ACTOR_FIRST_BIRD + 3);
  Actor_Stop(ACTOR_FIRST_BIRD + 4);
  Actor_Stop(ACTOR_FIRST_BIRD + 5);
  Actor_Stop(ACTOR_FIRST_BIRD + 6);
  Actor_SetSpeed(ACTOR_FIRST_BIRD, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_FIRST_BIRD + 1, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_FIRST_BIRD + 2, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_FIRST_BIRD + 3, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_FIRST_BIRD + 4, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_FIRST_BIRD + 5, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_FIRST_BIRD + 6, PIXELS(3), 3 * PIXELS(1) / 2);
}

/// On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK, the second
/// attack: a bird swoops at the lookout twice, and Sean cries out. "Monsters!"
static void TolbiShipMast_ShowMonsters(void) {
  struct Actor *approachingActor;
  struct Actor *leader;

  Event_Start();
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(leader, 0);
  wait(1);
  Event_CallWithLastActiveActorId(gTolbiShipMast_BirdActorPlacements);
  wait(1);
  TolbiShipMast_InitializeDriftingGroup();
  Actor_SetScript(ACTOR_LOOKOUT, gTolbiShipMast_LookoutActions);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(120);
  // The first bird swoops at the lookout twice while the rest keep drifting.
  approachingActor = Actor_GetById(ACTOR_FIRST_BIRD);
  Actor_Stop(ACTOR_FIRST_BIRD);
  TolbiShipMast_StopDriftingActor(approachingActor);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_FIRST_BIRD, PIXELS(8), PIXELS(4));
  FieldActor_SetMoveTarget(approachingActor, PIXELS(164), PIXELS(144),
                           PIXELS(321));
  Actor_WaitForMove(approachingActor);
  FieldActor_SetMoveTarget(approachingActor, PIXELS(164), PIXELS(104),
                           PIXELS(321));
  Actor_WaitForMove(approachingActor);
  FieldActor_SetMoveTarget(approachingActor, PIXELS(204), PIXELS(124),
                           PIXELS(321));
  Actor_WaitForMove(approachingActor);
  FieldActor_SetMoveTarget(approachingActor, PIXELS(144), 0, PIXELS(169));
  Actor_Stop(ACTOR_LOOKOUT);
  wait(1);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_SetSpeed(ACTOR_FIRST_BIRD, PIXELS(2), PIXELS(1));
  TolbiShipMast_InitializeDriftingActor(ACTOR_FIRST_BIRD);
  Event_Wait(20);
  Actor_SetScript(ACTOR_LOOKOUT, gTolbiShipMast_LookoutActions);
  Event_Wait(120);
  Actor_Stop(ACTOR_FIRST_BIRD);
  TolbiShipMast_StopDriftingActor(approachingActor);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_FIRST_BIRD, PIXELS(8), PIXELS(4));
  FieldActor_SetMoveTarget(approachingActor, PIXELS(164), PIXELS(144),
                           PIXELS(321));
  Actor_WaitForMove(approachingActor);
  Actor_SetSpeed(ACTOR_FIRST_BIRD, PIXELS(5), 5 * PIXELS(1) / 2);
  FieldActor_SetMoveTarget(approachingActor, PIXELS(164), PIXELS(104),
                           PIXELS(321));
  Actor_WaitForMove(approachingActor);
  FieldActor_SetMoveTarget(approachingActor, PIXELS(164), PIXELS(114),
                           PIXELS(321));
  Actor_WaitForMove(approachingActor);
  FieldActor_SetMoveTarget(approachingActor, PIXELS(164), PIXELS(104),
                           PIXELS(321));
  Actor_WaitForMove(approachingActor);
  FieldActor_SetMoveTarget(approachingActor, PIXELS(204), PIXELS(124),
                           PIXELS(321));
  Actor_WaitForMove(approachingActor);
  FieldActor_SetMoveTarget(approachingActor, PIXELS(144), 0, PIXELS(169));
  Actor_Stop(ACTOR_LOOKOUT);
  wait(1);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_SetSpeed(ACTOR_FIRST_BIRD, PIXELS(2), PIXELS(1));
  TolbiShipMast_InitializeDriftingActor(ACTOR_FIRST_BIRD);
  // The lookout jumps, the monsters' theme plays, and Sean cries out.
  Actor_Jump(ACTOR_LOOKOUT, 4, 20);
  Actor_Jump(ACTOR_LOOKOUT, 6, 40);
  Event_PlaySound(MUS_CONFRONTATION);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  Event_SetMessageId(MsgTolbiShipMonsters2);
  FieldEvent_ShowMessageThenWait(ACTOR_SEAN, 0, 20);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 164, 344);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_LOOKOUT, 2);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_SECOND_ATTACK);
  Event_End();
}

/// Initializes a drifting actor, retaining the random draws' original
/// order.
static void TolbiShipMast_InitializeDriftingActor(s32 actorId) {
  struct Actor *actor;
  u32 randomValue;

  actor = Actor_GetById(actorId);
  Actor_SetSpritePriority(actorId, 1);
  actor->motionFlags = 0;
  actor->timer = (u32)Import_RandomNext() >> 15;
  actor->userValue = (u32)Import_RandomNext() >> 15;
  randomValue = Import_RandomNext();
  randomValue <<= 2;
  randomValue >>= 16;
  randomValue <<= 16;
  randomValue += PIXELS(6);
  actor->y.fixed = randomValue;
  randomValue = Import_RandomNext();
  actor->riseBaseY = ((randomValue * 3 << 13) >> 16) - 3 * PIXELS(1) / 16;
  actor->scaleX = 5 * Q16_ONE / 4;
  actor->scaleY = 5 * Q16_ONE / 4;
  Actor_SetScript(actorId, gTolbiShipMast_DriftActions);
}

/// On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK, the third
/// attack: the birds scatter, then ACTOR_LANDING_MONSTER drops in beside the
/// lookout.
static void TolbiShipMast_RunMonsterLanding(void) {
  u32 frame;
  struct Actor *actor;

  Event_Start();
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 0);
  wait(1);
  Event_CallWithLastActiveActorId(gTolbiShipMast_BirdActorPlacements);
  wait(1);
  Event_CallWithLastActiveActorId(gTolbiShipMast_LandingActorPlacements);
  wait(1);
  TolbiShipMast_InitializeDriftingGroup();
  Actor_SetScript(ACTOR_LOOKOUT, gTolbiShipMast_LookoutActions);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(300);
  Event_PlaySound(SE_BEAST_CRY);
  Event_Wait(100);
  // The birds fly off before the monster lands.
  TolbiShipMast_PrepareGroupDeparture();
  Actor_SetDestination(ACTOR_FIRST_BIRD, 0, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 1, 60, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 2, 120, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 3, 180, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 4, 240, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 5, 320, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 6, 380, 100);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(20);
  Actor_SetPosition(ACTOR_FIRST_BIRD, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 1, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 2, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 3, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 4, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 5, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 6, 0, 0);
  Event_Wait(100);
  // ACTOR_LANDING_MONSTER_SHADOW grows where the monster will land, then
  // ACTOR_LANDING_MONSTER drops onto it.
  actor = Actor_GetById(ACTOR_LANDING_MONSTER_SHADOW);
  actor->scaleX = Q16_ONE / 10;
  actor->scaleY = Q16_ONE / 10;
  Actor_SetPosition(ACTOR_LANDING_MONSTER_SHADOW, PIXELS(172), PIXELS(340));
  Actor_Stop(ACTOR_LOOKOUT);
  wait(1);
  Actor_RunRepeatedMotion(ACTOR_LOOKOUT, 1);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_PlaySound(MUS_CONFRONTATION);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  // ACTOR_LANDING_MONSTER_SHADOW grows for 32 frames before
  // ACTOR_LANDING_MONSTER is placed above it.
  for (frame = 0; frame < 32; frame++) {
    actor->scaleX += Q16_ONE / 20;
    actor->scaleY += Q16_ONE / 20;
    wait(1);
  }
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_RunRepeatedMotion(ACTOR_LOOKOUT, 2);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 168, 340);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 200, 340);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_WEST, 0);
  actor = Actor_GetById(ACTOR_LANDING_MONSTER);
  actor->scaleX = 23 * Q16_ONE / 20;
  actor->scaleY = 23 * Q16_ONE / 20;
  actor->x.fixed = PIXELS(172);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(340);
  actor->facing = 0;
  actor->bounce = 2 * Q16_ONE / 5;
  actor->gravity = PIXELS(3);
  Event_Wait(20);
  Actor_Jump(ACTOR_LOOKOUT, 6, 20);
  Event_PlaySound(SE_BEAST_CRY);
  Event_Wait(20);
  Actor_SetScript(ACTOR_LOOKOUT, gTolbiShipMast_WaveActions);
  Event_Wait(80);
  // Let the monster bounce, then move it and its landing spot out of the
  // shot.
  Actor_SetSpritePriority(ACTOR_LANDING_MONSTER, 1);
  Actor_SetSpeed(ACTOR_LANDING_MONSTER, PIXELS(1), PIXELS(1) / 2);
  actor->bounce = Q16_ONE / 10;
  actor->gravity = 7 * PIXELS(1) / 10;
  Event_PlaySound(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(8);
  Actor_SetDestination(ACTOR_LANDING_MONSTER, 132, 360);
  Actor_SetDestination(ACTOR_LANDING_MONSTER_SHADOW, 132, 360);
  Event_Wait(40);
  Actor_SetPosition(ACTOR_LANDING_MONSTER, 0, 0);
  actor = Actor_GetById(ACTOR_LOOKOUT);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  actor->facing = FACING_SOUTH + FACING_STEP;
  Event_Wait(40);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_THIRD_ATTACK);
  Event_End();
}

/// On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO: the seven birds fly
/// off, then the lookout announces land, and the rowing balance chooses the
/// exit.
static void TolbiShipMast_AnnounceLand(void) {
  Event_Start();
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 0);
  Event_CallWithLastActiveActorId(gTolbiShipMast_BirdActorPlacements);
  wait(1);
  TolbiShipMast_InitializeDriftingGroup();
  Actor_SetScript(ACTOR_LOOKOUT, gTolbiShipMast_LookoutActions);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(400);
  // Each bird takes its own way off while shrinking out of view.
  TolbiShipMast_PrepareGroupDeparture();
  Actor_SetScript(ACTOR_FIRST_BIRD, gTolbiShipMast_BirdDeparture0);
  Actor_SetScript(ACTOR_FIRST_BIRD + 1, gTolbiShipMast_BirdDeparture1);
  Actor_SetScript(ACTOR_FIRST_BIRD + 2, gTolbiShipMast_BirdDeparture2);
  Actor_SetScript(ACTOR_FIRST_BIRD + 3, gTolbiShipMast_BirdDeparture3);
  Actor_SetScript(ACTOR_FIRST_BIRD + 4, gTolbiShipMast_BirdDeparture4);
  Actor_SetScript(ACTOR_FIRST_BIRD + 5, gTolbiShipMast_BirdDeparture5);
  Actor_SetScript(ACTOR_FIRST_BIRD + 6, gTolbiShipMast_BirdDeparture6);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_LOOKOUT, 3);
  Actor_SetAttachedEffect(ACTOR_LOOKOUT,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(120);
  Actor_StartRepeatedMotion(ACTOR_LOOKOUT, 1);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_SetSpeed(ACTOR_LOOKOUT, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 164, 344);
  Actor_Jump(ACTOR_LOOKOUT, 4, 10);
  Actor_Jump(ACTOR_LOOKOUT, 6, 20);
  Event_SetMessageId(MsgTolbiShipLandHo);
  FieldEvent_ShowMessageThenWait(ACTOR_LOOKOUT, 0, 20);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  // The rowing balance picks where the ship comes in: exactly 11 takes
  // TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE, off course to the island.
  gSequenceParentWork.restartScene = (s32)&SceneId_ShipRooms;
  gSequenceParentWork.restartEntrance =
      SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK;
  if (TolbiShipMast_GetRowingBalance() == 11) {
    Import_EventSetExitRequest(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE);
  } else {
    Import_EventSetExitRequest(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED);
  }
  Event_End();
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
    SC_CALL(TolbiShipMast_SwayActor),
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
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

s32 gTolbiShipMast_BirdDeparture1[] = {
    SC_MOVE(PIXELS(132), 0, PIXELS(242)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

s32 gTolbiShipMast_BirdDeparture2[] = {
    SC_MOVE(PIXELS(88), 0, PIXELS(166)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

s32 gTolbiShipMast_BirdDeparture3[] = {
    SC_MOVE(PIXELS(220), 0, PIXELS(134)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

s32 gTolbiShipMast_BirdDeparture4[] = {
    SC_MOVE(PIXELS(264), 0, PIXELS(228)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

s32 gTolbiShipMast_BirdDeparture5[] = {
    SC_MOVE(PIXELS(282), 0, PIXELS(168)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
};

// clang-format off
u32 gTolbiShipMast_BirdDeparture6[] = {
    SC_MOVE(PIXELS(154), 0, PIXELS(135)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(196)),
    SC_CALL(TolbiShipMast_ShrinkAndStopActor),
    SC_END,
    // Unidentified words following the script terminator. XXX (no evidence):
    // what they were; nothing reads them, and as sixteen halfwords they form no
    // table any code here indexes.
    0x9090000, 0xd70090d, 0xdb315cb, 0x15f5164c,
    0x1f0e1aad, 0x1f6f227a, 0x23f223b1, 0x2bf727f4,
};

struct SceneEntrance gTolbiShipMast_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 165, 0, 235, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 184, 56, 352, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneExitRegion gTolbiShipMast_Regions[] = {
    {176, 16, 348, 192, 32, 364, -1, 1},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gTolbiShipMast_Exits[] = {
    SCENE_EXITS(TolbiShipMast),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_SHIP_ROOMS_CAST_OFF, ShipRooms, SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_SHIP_ROOMS_MONSTERS, ShipRooms, SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_SECOND_ATTACK, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_THIRD_ATTACK, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE),
    SCENE_EXITS_END,
};

struct ActorPlacement gTolbiShipMast_ActorPlacements[] = {
    { SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gTolbiShipMast_FirstAttackActorPlacements[] = {
    { 0x11e, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ActorPlacement gTolbiShipMast_BirdActorPlacements[] = {
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
    { SCENE_TABLE_END },
};

struct ActorPlacement gTolbiShipMast_LandingActorPlacements[] = {
    { SPRITE_BLUE_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gTolbiShipMast_Triggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TOLBI_SHIP_MAST_LADDER, CONDITION_ALWAYS, EVENT(TolbiShipMast_ClimbDownLadder) },
    { TRIGGER_TALK, ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF, MSGNO(MsgTolbiShipWatchSeasHelpKaja) },
    { TRIGGER_TALK, ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipLookoutCannotRow) },
    { TRIGGER_TALK, ACTOR_LOOKOUT, CONDITION_ALWAYS, MSGNO(MsgTolbiShipMonstersAnywhereAroundHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF, MSGNO(MsgTolbiShipHiddenAnchorDiscovered) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipSeaTooVast) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOOKOUT, CONDITION_ALWAYS, MSGNO(MsgTolbiShipIslandTreasures) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_SHIP_MAST_ANCHOR_CHARM, FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND, EVENT(TolbiShipMast_FindAnchorCharm) },
    { TRIGGERS_END },
};

/// Each passenger's rowing value, in the order of enum TolbiShipPassenger
/// (ship_rooms.c).
u32 gTolbiShipMast_PassengerRowValues[] = {
    7, 5, 8, 4, 9, 12, 6, 3,
    0,
};
// clang-format on
