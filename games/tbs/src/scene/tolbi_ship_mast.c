/// @file
/// Tolbi-bound Ship (SceneId_TolbiShipMast), the top of its mast, the Crow's
/// Nest, where the ship's lookout (ACTOR_LOOKOUT) keeps watch: one of the
/// ship's three scenes on its voyage from Kalay to Tolbi, told in
/// tolbi_ship_deck.c. The party climbs up by the deck's ladder
/// (TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK) and climbs down again by touch cell
/// 10 and the region of TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK (the deck's
/// TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST); the entrances from
/// TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR on are the voyage's
/// cutscenes here, each leading on into the next. In story order:
///
/// - At Kalay, while the ship waits for its replacement: the captain's anchor
///   charm, gone from his cabin, lies up here. Searching for it (search code
///   11) finds the Anchor Charm (FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND), but only
///   once the party has confronted the crewman on deck or the warriors have
///   gone to the captain (FLAG_TOLBI_SHIP_CREWMAN_CONFRONTED or
///   FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN). Until the first attack, Mind
///   Read on the lookout has him wonder whether the party has worked out that
///   he hid it ("I wonder if they figured out I was the one who hid the
///   anchor"), and he leaves the monsters to Kaja's crew and the party.
///   Once the charm is back, the setup moves him to his lookout spot. The
///   crewman on deck at Kalay (KALAY_CREWMAN, tolbi_ship_deck.c) and in the
///   captain's cabin (CABIN_CREWMAN, tolbi_ship_interiors.c) says he hid it
///   too. XXX: likely the lookout is that crewman, because he says the same
///   and stands on the mast only once the charm is back.
/// - Under sail: TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR has him scan
///   both sides and call the course clear, then leads below to the cast-off;
///   the first time the leader is moved out of view, on later departures he
///   stands by. The attacks: TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK,
///   "Avast! Monsters sighted!", as three monsters appear where a sparkle
///   twinkles and close in (FLAG_TOLBI_SHIP_MONSTERS_SIGHTED, which plays the
///   monsters' theme); Sean's "Monsters!" for the second
///   (TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK); a monster dropping in
///   beside the lookout for the third
///   (TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK), each leading below or to
///   the deck. During these the place a defeat returns to
///   (gGameState.savedScene) is set to
///   TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT, which starts the voyage
///   over.
/// - Land: TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO, where the seven birds
///   (ACTOR_FIRST_BIRD) flutter about and fly away before "Land ho!". The
///   rowing values of the passengers chosen to row
///   (gTolbiShipMast_PassengerRowValues) decide where the ship comes in: those
///   at the first and third seats less those at the second and fourth, and a
///   balance of exactly 11 takes TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE,
///   to the deck's TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE, off course
///   to the island; any other takes
///   TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED, to the deck's
///   TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED and Tolbi. Once the
///   rowers have switched sides at the island the balance is the opposite, so
///   the ship comes in to Tolbi.
///
/// While the ship is under way (rowing, or at sea after the first attack,
/// and neither at the island nor at Tolbi) the setup makes the view sway. At
/// sea the lookout says he is the lookout, not an oarsman; at the island he
/// warns of monsters and wonders at its treasures.

#include "script.h"
#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/tolbi_ship_mast.h"
#include "unit/object/object.h"
#include "unit/field/field.h"
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
#include "global_cells.h"
#include "item_ids.h"
#include "flags.h"
#include "songs.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "macros.h"

// Scene sequences and the mast's per-frame updates.
static void TolbiShipMast_SetupHiddenLeaderLookout(void);
static void TolbiShipMast_SetupVisibleLeaderLookout(void);
static void TolbiShipMast_AnnounceClearCourse(void);
static void TolbiShipMast_RunAvastScene(void);
static void TolbiShipMast_ShowMonsters(void);
static void TolbiShipMast_InitializeDriftingActor(s32 actorId);
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

/// The scene's one placement row, the ship's lookout. "Ship's course...
/// Clear!"
enum TolbiShipMastActor { ACTOR_LOOKOUT = ACTOR_FIRST_PLACED };

/// The first attack's actors (TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK),
/// placed after the lookout from gTolbiShipMast_StagingObjects.
enum TolbiShipMastFirstAttackActor {
  /// A sparkle (graphics/sprite/spr_286.png, a four-pointed sparkle in eight
  /// shrinking frames), placed nowhere else. It turns, swells for 10 frames and
  /// shrinks for 24 (gTolbiShipMast_FollowCameraActions) where the monsters
  /// then appear.
  ACTOR_SPARKLE = ACTOR_LOOKOUT + 1,
  /// The first of three monsters, all with the sprite of the first attack's
  /// monsters below decks (gTolbiShipInteriors_FirstAttackMonsters). They grow
  /// from half size as they close in. "Monsters sighted!"
  ACTOR_FIRST_SIGHTED_MONSTER
};

/// The actors of the later cutscenes (from
/// TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK on), placed after the lookout
/// from gTolbiShipMast_EnsembleObjects and, at
/// TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK, then from
/// gTolbiShipMast_LandingObjects.
enum TolbiShipMastEnsembleActor {
  /// The first of seven birds, all of one sprite (graphics/sprite/spr_155.png,
  /// a white bird in three wing poses), that flutter and bob about the mast
  /// (TolbiShipMast_SwayActor). One swoops at the lookout at the second attack,
  /// they scatter as the third attack's monster comes, and they fly off just
  /// before "Land ho!".
  ACTOR_FIRST_BIRD = ACTOR_LOOKOUT + 1,
  /// Sean, by the sprite he has in the hold (HOLD_SEAN, tolbi_ship_interiors.c)
  /// and on deck (SEA_SEAN, tolbi_ship_deck.c). He cries "Monsters!" at the
  /// second attack.
  ACTOR_SEAN = ACTOR_FIRST_BIRD + 7,
  /// The sprite of the third attack's monster below decks
  /// (gTolbiShipInteriors_ThirdAttackMonsters); it drops in beside the lookout.
  ACTOR_LANDING_MONSTER,
  /// The sprite kolima_forest_tree.c and vale_storm.c place as an invisible
  /// actor. It grows from a tenth of its size where ACTOR_LANDING_MONSTER
  /// drops, then follows it away. XXX: likely the monster's shadow, because it
  /// grows before the monster falls onto it.
  ACTOR_LANDING_SPOT
};

/// Keeps the effect actor relative to the saved camera origin and rotates it.
static s32
TolbiShipMast_FollowCameraAndRotate(struct ScriptInterpreter *interpreter) {
  struct FieldActor *object = (struct FieldActor *)interpreter;
  s32 *position = ((struct MapScrollWork *)gMapWork[0])->origin;
  s32 *origin = gTolbiShipMast_CameraOrigin;
  s32 *center = gTolbiShipMast_CameraCenter;
  s32 cameraX = *position++;
  s32 cameraY = *position;

  object->x.fixed = center[0] + (cameraX - origin[0]);
  object->y.fixed = center[1] + (cameraY - origin[1]) / 2;
  object->sprite->rotation += 0x600;
  return 0;
}

/// A random sample of 6 or 9 turns the lookout to either side of north.
static s32
TolbiShipMast_RandomizeLookoutFacing(struct ScriptInterpreter *interpreter) {
  struct FieldActor *object = (struct FieldActor *)interpreter;
  u32 sample = (u32)(Engine_RandomNext() << 6) >> 16;

  if (sample == 6) {
    s32 value = FACING_NORTH + FACING_STEP;

    object->facing = value;
  } else if (sample == 9) {
    s32 value = FACING_NORTHWEST + FACING_STEP;

    object->facing = value;
  }

  return 1;
}

/// Grows both axes by the same amount until the actor reaches full scale.
static void TolbiShipMast_GrowActorScale(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  if (actor->scaleX < Q16_ONE) {
    actor->scaleX += 160;
    actor->scaleY += 160;
  }
}

/// Which way TolbiShipMast_SwayActor moves an actor along each axis, kept in
/// its timer (the drift) and its work (the bob).
enum TolbiShipMastSway {
  SWAY_INCREASING = 0, ///< the drift speeding up, or the bob rising
  SWAY_DECREASING = 1, ///< the drift slowing, or the bob sinking
  SWAY_HELD = 9        ///< held at zero; nothing here sets it
};

/// Randomly reverses horizontal drift and vertical bobbing at their limits.
static s32 TolbiShipMast_SwayActor(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s16 *state = &actor->timer;
  s32 next;

  if (*state == SWAY_HELD) {
    actor->riseBaseY = 0;
  } else if (*state != SWAY_INCREASING) {
    actor->riseBaseY -= (u32)(Engine_RandomNext() << 11) >> 16;
    if (actor->riseBaseY < -3 * PIXELS(1) / 4) {
      next = SWAY_INCREASING;
      *state = next;
    }
  } else {
    actor->riseBaseY += (u32)(Engine_RandomNext() << 11) >> 16;
    if (actor->riseBaseY > 3 * PIXELS(1) / 4) {
      next = SWAY_DECREASING;
      *state = next;
    }
  }
  if (actor->x.fixed > PIXELS(40) && actor->x.fixed < PIXELS(320)) {
    actor->x.fixed += actor->riseBaseY;
  }
  state = (s16 *)&actor->work;
  if (*state == SWAY_HELD) {
    actor->y.fixed = 0;
  } else if (*state != SWAY_INCREASING) {
    actor->y.fixed -= ((u32)Engine_RandomNext() * 3 << 14) >> 16;
    if (actor->y.fixed < 0) {
      next = SWAY_INCREASING;
      *state = next;
    }
  } else {
    actor->y.fixed += ((u32)Engine_RandomNext() * 3 << 14) >> 16;
    if (actor->y.fixed > PIXELS(16)) {
      next = SWAY_DECREASING;
      *state = next;
    }
  }
  return 1;
}

/// Shrinks both axes; once small enough, clears the actor's position and speed.
static s32
TolbiShipMast_ShrinkAndStopActor(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  struct AnimationObject *sprite = actor->sprite;

  sprite->image.priority = 3;
  actor->speed = PIXELS(2);
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

/// Returns the entrances: from the deck's ladder, and the voyage's cutscenes.
const struct SceneEntrance *TolbiShipMast_GetEntrances(void) {
  return gTolbiShipMast_Entrances;
}

/// Returns the ladder's three-dimensional activation region.
const struct SceneRegion *TolbiShipMast_GetRegions(void) {
  return gTolbiShipMast_Regions;
}

/// Returns exits to the deck and the continuing ship scenes.
const u32 *TolbiShipMast_GetExits(void) { return gTolbiShipMast_Exits; }

/// Returns the lookout, the only actor placed.
const struct ScenePlacement *TolbiShipMast_GetPlacements(void) {
  return gTolbiShipMast_Placements;
}

/// Returns the ladder, lookout dialogue and Anchor Charm search events.
const struct SceneEvent *TolbiShipMast_GetEvents(void) {
  return gTolbiShipMast_Events;
}

/// Walking south onto the ladder at the mast's top (touch cell 10): the
/// leader climbs down.
static void TolbiShipMast_ClimbDownLadder(void) { Field_ClimbDown(); }

/// Searching the mast's top for the captain's anchor charm (search code 11):
/// gives the Anchor Charm once the crewman has been confronted or the warriors
/// have gone to the captain.
static void TolbiShipMast_FindAnchorCharm(void) {
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_CREWMAN_CONFRONTED) != 0 ||
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
    Engine_EventBegin();
    Engine_ItemShowFound(ITEM_ANCHOR_CHARM, 3);
    Engine_PartyGiveItem(ITEM_ANCHOR_CHARM, 0);
    Engine_GameFlagSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND);
    Engine_EventEnd();
  }
}

/// The mast's setup: sways the view while the ship is under way, puts the
/// lookout to his spot once the charm is back, and plays what the entrance
/// calls for: the climb up from the deck (TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK)
/// or a cutscene (the entrances from
/// TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR on).
s32 TolbiShipMast_Initialize(void) {
  struct FieldActor *leader;

  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if ((Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ROWING) != 0 ||
       Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) &&
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) == 0) {
    gTolbiShipMast_SwayX = (u16)Engine_RandomNext();
    gTolbiShipMast_SwayY = (u16)Engine_RandomNext();
    FieldScheduler_AddCallback(TolbiShipMast_UpdateSway, TASK_PRIORITY_SCENE);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0 &&
      Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
    Actor_SetPosition(ACTOR_LOOKOUT, PIXELS(164), PIXELS(328));
  }
  // The ship's scripted entrances resume their matching lookout scenes.
  switch (gGameState.entrance) {
  case TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      Engine_EventBegin();
      Field_ClimbUp();
      leader->y.fixed = PIXELS(56);
      Camera_MoveTo(-1, -1, -1, 0);
      Engine_TaskWait(1);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Engine_MapRedraw();
      Engine_TaskWait(1);
      Engine_EventEnd();
    }
    break;
  case TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR:
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED) != 0) {
      TolbiShipMast_SetupVisibleLeaderLookout();
    } else {
      TolbiShipMast_SetupHiddenLeaderLookout();
    }
    break;
  case TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK:
    gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
    gGameState.savedEntrance = TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT;
    TolbiShipMast_RunAvastScene();
    break;
  case TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK:
    gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
    gGameState.savedEntrance = TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT;
    TolbiShipMast_ShowMonsters();
    break;
  case TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK:
    gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
    gGameState.savedEntrance = TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_AFTER_DEFEAT;
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
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Camera_GetEventObject()->motionFlags = 0;
  Camera_MoveTo(PIXELS(164), PIXELS(64), PIXELS(321), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  TolbiShipMast_AnnounceClearCourse();
  Engine_EventEnd();
}

/// Keeps the leader visible for the course announcement.
static void TolbiShipMast_SetupVisibleLeaderLookout(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(164), PIXELS(321));
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(leader, 0);
  Engine_TaskWait(1);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  TolbiShipMast_AnnounceClearCourse();
  Engine_EventEnd();
}

/// On the mast at TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR: the
/// lookout scans both sides, announces a clear course and leaves by
/// TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_CAST_OFF, to the cast-off below
/// decks.
static void TolbiShipMast_AnnounceClearCourse(void) {
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_LOOKOUT, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 164, 321);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 164, 334);
  Engine_ActorJump(ACTOR_LOOKOUT, 4, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_LOOKOUT, 2);
  Engine_EventSetMessage(MsgTolbiShipShipsCourseClear);
  Event_ShowMessageAndWait(ACTOR_LOOKOUT, 0, 20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_CAST_OFF);
}

/// On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK, the first
/// attack: three monsters close in and the lookout cries out. "Avast!"
static void TolbiShipMast_RunAvastScene(void) {
  s32 *placement = ((struct MapScrollWork *)gMapWork[0])->origin;
  struct FieldActor *actor;
  struct EventWork **event;
  s32 x;
  s32 zero = 0;
  s32 scale;
  void (*update)(union FieldObject *);

  Engine_EventBegin();
  Event_CallWithLastActiveObjectId(gTolbiShipMast_StagingObjects);
  Engine_TaskWait(1);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Actor_SetActionScript(ACTOR_LOOKOUT, gTolbiShipMast_LookoutActions);
  event = (struct EventWork **)(gMapWork + (HEAP_SLOT_EVENT - HEAP_SLOT_MAP));
  (*event)->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  // Save the camera origin before the sparkle begins to turn.
  Engine_EventWait(20);
  gTolbiShipMast_CameraOrigin[0] = *placement++;
  gTolbiShipMast_CameraOrigin[1] = *placement;
  Actor_SetPosition(ACTOR_SPARKLE, PIXELS(80), PIXELS(210));
  x = PIXELS(80);
  Object_GetById(ACTOR_SPARKLE)->motionFlags = zero;
  gTolbiShipMast_CameraCenter[0] = x;
  gTolbiShipMast_CameraCenter[1] = zero;
  Actor_SetActionScript(ACTOR_SPARKLE, gTolbiShipMast_FollowCameraActions);
  Engine_EventWait(20);
  Engine_AudioPlayCue(MUS_029);
  Engine_GameFlagSet(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  Engine_ActorStop(ACTOR_LOOKOUT);
  Engine_TaskWait(1);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT, 0);
  Engine_ActorFaceDirection(ACTOR_LOOKOUT, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventSetMessage(MsgTolbiShipAvast);
  Engine_EventShowMessageAndWait(ACTOR_LOOKOUT, 0, 10);
  // The three monsters appear where the sparkle turned.
  Actor_SetPosition(ACTOR_FIRST_SIGHTED_MONSTER, x, PIXELS(210));
  Actor_SetPosition(ACTOR_FIRST_SIGHTED_MONSTER + 1, x, PIXELS(210));
  Engine_ActorSetPosition(ACTOR_FIRST_SIGHTED_MONSTER + 2, x, PIXELS(210));
  Engine_ActorSetSpritePriority(ACTOR_FIRST_SIGHTED_MONSTER, 3);
  Engine_ActorSetSpritePriority(ACTOR_FIRST_SIGHTED_MONSTER + 1, 3);
  Engine_ActorSetSpritePriority(ACTOR_FIRST_SIGHTED_MONSTER + 2, 3);
  Engine_ActorSetChildValue(ACTOR_FIRST_SIGHTED_MONSTER, 3);
  Engine_ActorSetChildValue(ACTOR_FIRST_SIGHTED_MONSTER + 1, 3);
  Engine_ActorSetChildValue(ACTOR_FIRST_SIGHTED_MONSTER + 2, 3);
  // Grow the three monsters from half size as they close in on the lookout.
  actor = Object_GetById(ACTOR_FIRST_SIGHTED_MONSTER);
  scale = Q16_ONE / 2;
  update = TolbiShipMast_GrowActorScale;
  actor->scaleY = scale;
  actor->scaleX = scale;
  actor->update = update;
  actor = Object_GetById(ACTOR_FIRST_SIGHTED_MONSTER + 1);
  actor->scaleY = scale;
  actor->scaleX = scale;
  actor->update = update;
  actor = Object_GetById(ACTOR_FIRST_SIGHTED_MONSTER + 2);
  actor->scaleY = scale;
  actor->scaleX = scale;
  actor->update = update;
  Engine_TaskWait(1);
  Actor_SetSpeed(ACTOR_FIRST_SIGHTED_MONSTER, 13 * PIXELS(1) / 25,
                 13 * PIXELS(1) / 50);
  Actor_SetSpeed(ACTOR_FIRST_SIGHTED_MONSTER + 1, 9 * PIXELS(1) / 20,
                 9 * PIXELS(1) / 40);
  Actor_SetSpeed(ACTOR_FIRST_SIGHTED_MONSTER + 2, 3 * PIXELS(1) / 5,
                 3 * PIXELS(1) / 10);
  Actor_SetDestination(ACTOR_FIRST_SIGHTED_MONSTER, 128, 345);
  Actor_SetDestination(ACTOR_FIRST_SIGHTED_MONSTER + 1, 136, 330);
  Engine_ActorSetDestination(ACTOR_FIRST_SIGHTED_MONSTER + 2, 156, 340);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_LOOKOUT, 2);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 164, 344);
  Engine_ActorJump(ACTOR_LOOKOUT, 4, 10);
  Engine_ActorJump(ACTOR_LOOKOUT, 6, 40);
  Engine_ActorStartRepeatedMotion(ACTOR_LOOKOUT, 3);
  Engine_EventShowMessageAndWait(ACTOR_LOOKOUT, 0, 20);
  (*event)->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_MONSTERS);
  Engine_EventEnd();
}

/// Clears the movement left by a bird's drifting script before it swoops.
static inline void TolbiShipMast_StopDriftingActor(struct FieldActor *actor) {
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

/// Stops the birds' drifting actions and gives them their speed to fly off.
static inline void TolbiShipMast_PrepareGroupDeparture(void) {
  Engine_ActorStop(ACTOR_FIRST_BIRD);
  Engine_ActorStop(ACTOR_FIRST_BIRD + 1);
  Engine_ActorStop(ACTOR_FIRST_BIRD + 2);
  Engine_ActorStop(ACTOR_FIRST_BIRD + 3);
  Engine_ActorStop(ACTOR_FIRST_BIRD + 4);
  Engine_ActorStop(ACTOR_FIRST_BIRD + 5);
  Engine_ActorStop(ACTOR_FIRST_BIRD + 6);
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
  struct FieldActor *approachingActor;
  struct FieldActor *leader;

  Engine_EventBegin();
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(leader, 0);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId(gTolbiShipMast_EnsembleObjects);
  Engine_TaskWait(1);
  TolbiShipMast_InitializeDriftingGroup();
  Actor_SetActionScript(ACTOR_LOOKOUT, gTolbiShipMast_LookoutActions);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(120);
  // The first bird swoops at the lookout twice while the rest keep drifting.
  approachingActor = Object_GetById(ACTOR_FIRST_BIRD);
  Engine_ActorStop(ACTOR_FIRST_BIRD);
  TolbiShipMast_StopDriftingActor(approachingActor);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_FIRST_BIRD, PIXELS(8), PIXELS(4));
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(144),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(104),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(204), PIXELS(124),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(144), 0, PIXELS(169));
  Engine_ActorStop(ACTOR_LOOKOUT);
  Engine_TaskWait(1);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | 3, 60);
  Actor_SetSpeed(ACTOR_FIRST_BIRD, PIXELS(2), PIXELS(1));
  TolbiShipMast_InitializeDriftingActor(ACTOR_FIRST_BIRD);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_LOOKOUT, gTolbiShipMast_LookoutActions);
  Engine_EventWait(120);
  Engine_ActorStop(ACTOR_FIRST_BIRD);
  TolbiShipMast_StopDriftingActor(approachingActor);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_FIRST_BIRD, PIXELS(8), PIXELS(4));
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(144),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  Actor_SetSpeed(ACTOR_FIRST_BIRD, PIXELS(5), 5 * PIXELS(1) / 2);
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(104),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(114),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(164), PIXELS(104),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  FieldObject_SetPosition(approachingActor, PIXELS(204), PIXELS(124),
                          PIXELS(321));
  FieldObject_WaitForMove(approachingActor);
  Engine_ObjectSetPosition(approachingActor, PIXELS(144), 0, PIXELS(169));
  Engine_ActorStop(ACTOR_LOOKOUT);
  Engine_TaskWait(1);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | 3, 60);
  Actor_SetSpeed(ACTOR_FIRST_BIRD, PIXELS(2), PIXELS(1));
  TolbiShipMast_InitializeDriftingActor(ACTOR_FIRST_BIRD);
  // The lookout jumps, the monsters' theme plays, and Sean cries out.
  Engine_ActorJump(ACTOR_LOOKOUT, 4, 20);
  Engine_ActorJump(ACTOR_LOOKOUT, 6, 40);
  Event_PlayCue(MUS_029);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  Engine_EventSetMessage(MsgTolbiShipMonsters2);
  Event_ShowMessageAndWait(ACTOR_SEAN, 0, 20);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT, 0);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 164, 344);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_LOOKOUT, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_CREW_ATTACKED);
  Engine_EventEnd();
}

/// Initializes a drifting actor, retaining the random draws' original order.
static void TolbiShipMast_InitializeDriftingActor(s32 actorId) {
  struct FieldActor *actor;
  u32 randomValue;

  actor = Object_GetById(actorId);
  Engine_ActorSetSpritePriority(actorId, 1);
  actor->motionFlags = 0;
  actor->timer = (u32)Engine_RandomNext() >> 15;
  actor->work = (u32)Engine_RandomNext() >> 15;
  randomValue = Engine_RandomNext();
  randomValue <<= 2;
  randomValue >>= 16;
  randomValue <<= 16;
  randomValue += PIXELS(6);
  actor->y.fixed = randomValue;
  randomValue = Engine_RandomNext();
  actor->riseBaseY = ((randomValue * 3 << 13) >> 16) - 3 * PIXELS(1) / 16;
  actor->scaleX = 5 * Q16_ONE / 4;
  actor->scaleY = 5 * Q16_ONE / 4;
  Actor_SetActionScript(actorId, gTolbiShipMast_DriftActions);
}

/// On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK, the third
/// attack: the birds scatter, then ACTOR_LANDING_MONSTER drops in beside the
/// lookout.
static void TolbiShipMast_RunLandingSequence(void) {
  u32 frame;
  struct FieldActor *actor;

  Engine_EventBegin();
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId(gTolbiShipMast_EnsembleObjects);
  Engine_TaskWait(1);
  Event_CallWithLastActiveObjectId(gTolbiShipMast_LandingObjects);
  Engine_TaskWait(1);
  TolbiShipMast_InitializeDriftingGroup();
  Actor_SetActionScript(ACTOR_LOOKOUT, gTolbiShipMast_LookoutActions);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(300);
  Event_PlayCue(SE_147);
  Engine_EventWait(100);
  // The birds fly off before the monster lands.
  TolbiShipMast_PrepareGroupDeparture();
  Actor_SetDestination(ACTOR_FIRST_BIRD, 0, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 1, 60, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 2, 120, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 3, 180, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 4, 240, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 5, 320, 100);
  Actor_SetDestination(ACTOR_FIRST_BIRD + 6, 380, 100);
  Engine_EventWait(40);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(20);
  Actor_SetPosition(ACTOR_FIRST_BIRD, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 1, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 2, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 3, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 4, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 5, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_BIRD + 6, 0, 0);
  Engine_EventWait(100);
  // ACTOR_LANDING_SPOT grows where the monster will land, then
  // ACTOR_LANDING_MONSTER drops onto it.
  actor = Object_GetById(ACTOR_LANDING_SPOT);
  actor->scaleX = Q16_ONE / 10;
  actor->scaleY = Q16_ONE / 10;
  Actor_SetPosition(ACTOR_LANDING_SPOT, PIXELS(172), PIXELS(340));
  Engine_ActorStop(ACTOR_LOOKOUT);
  Engine_TaskWait(1);
  Engine_ActorRunRepeatedMotion(ACTOR_LOOKOUT, 1);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_PlayCue(MUS_029);
  GameFlag_Set(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
  // ACTOR_LANDING_SPOT grows for 32 frames before ACTOR_LANDING_MONSTER is
  // placed above it.
  for (frame = 0; frame < 32; frame++) {
    actor->scaleX += Q16_ONE / 20;
    actor->scaleY += Q16_ONE / 20;
    Engine_TaskWait(1);
  }
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_LOOKOUT, 2);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 168, 340);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 200, 340);
  Actor_FaceDirection(ACTOR_LOOKOUT, FACING_WEST, 0);
  actor = Object_GetById(ACTOR_LANDING_MONSTER);
  actor->scaleX = 23 * Q16_ONE / 20;
  actor->scaleY = 23 * Q16_ONE / 20;
  actor->x.fixed = PIXELS(172);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(340);
  actor->facing = 0;
  actor->bounce = 2 * Q16_ONE / 5;
  actor->gravity = PIXELS(3);
  Engine_EventWait(20);
  Engine_ActorJump(ACTOR_LOOKOUT, 6, 20);
  Event_PlayCue(SE_147);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_LOOKOUT, gTolbiShipMast_WaveActions);
  Engine_EventWait(80);
  // Let the monster bounce, then move it and its landing spot out of the
  // shot.
  Engine_ActorSetSpritePriority(ACTOR_LANDING_MONSTER, 1);
  Actor_SetSpeed(ACTOR_LANDING_MONSTER, PIXELS(1), PIXELS(1) / 2);
  actor->bounce = Q16_ONE / 10;
  actor->gravity = 7 * PIXELS(1) / 10;
  Event_PlayCue(SE_HIGH_JUMP);
  actor->velocityY = PIXELS(8);
  Actor_SetDestination(ACTOR_LANDING_MONSTER, 132, 360);
  Actor_SetDestination(ACTOR_LANDING_SPOT, 132, 360);
  Engine_EventWait(40);
  Actor_SetPosition(ACTOR_LANDING_MONSTER, 0, 0);
  actor = Object_GetById(ACTOR_LOOKOUT);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  actor->facing = FACING_SOUTH + FACING_STEP;
  Engine_EventWait(40);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_MONSTER_LANDS);
  Engine_EventEnd();
}

/// On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO: the seven birds fly
/// off, then the lookout announces land, and the rowing balance chooses the
/// exit.
static void TolbiShipMast_AnnounceLand(void) {
  Engine_EventBegin();
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Event_CallWithLastActiveObjectId(gTolbiShipMast_EnsembleObjects);
  Engine_TaskWait(1);
  TolbiShipMast_InitializeDriftingGroup();
  Actor_SetActionScript(ACTOR_LOOKOUT, gTolbiShipMast_LookoutActions);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(400);
  // Each bird takes its own way off while shrinking out of view.
  TolbiShipMast_PrepareGroupDeparture();
  Actor_SetActionScript(ACTOR_FIRST_BIRD, gTolbiShipMast_BirdDeparture0);
  Actor_SetActionScript(ACTOR_FIRST_BIRD + 1, gTolbiShipMast_BirdDeparture1);
  Actor_SetActionScript(ACTOR_FIRST_BIRD + 2, gTolbiShipMast_BirdDeparture2);
  Actor_SetActionScript(ACTOR_FIRST_BIRD + 3, gTolbiShipMast_BirdDeparture3);
  Actor_SetActionScript(ACTOR_FIRST_BIRD + 4, gTolbiShipMast_BirdDeparture4);
  Actor_SetActionScript(ACTOR_FIRST_BIRD + 5, gTolbiShipMast_BirdDeparture5);
  Actor_SetActionScript(ACTOR_FIRST_BIRD + 6, gTolbiShipMast_BirdDeparture6);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_LOOKOUT, 3);
  Actor_SetAttachedEffect(ACTOR_LOOKOUT, 258);
  Engine_EventWait(120);
  Engine_ActorStartRepeatedMotion(ACTOR_LOOKOUT, 1);
  Actor_ShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT, 60);
  Actor_SetSpeed(ACTOR_LOOKOUT, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_LOOKOUT, 164, 344);
  Engine_ActorJump(ACTOR_LOOKOUT, 4, 10);
  Engine_ActorJump(ACTOR_LOOKOUT, 6, 20);
  Engine_EventSetMessage(MsgTolbiShipLandHo);
  Event_ShowMessageAndWait(ACTOR_LOOKOUT, 0, 20);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  // The rowing balance picks where the ship comes in: exactly 11 takes
  // TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE, off course to the island.
  gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
  gGameState.savedEntrance =
      TOLBI_SHIP_INTERIORS_PASSENGER_HOLD_FROM_ROWING_DECK;
  if (TolbiShipMast_GetRowingBalance() == 11) {
    Engine_EventRequestExit(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE);
  } else {
    Engine_EventRequestExit(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED);
  }
  Engine_EventEnd();
}

/// Adds horizontal and vertical sway to the camera origin each frame.
static void TolbiShipMast_UpdateSway(void) {
  s32 *position = ((struct MapScrollWork *)gMapWork[0])->origin;
  s32 swayX = Engine_MathCos(gTolbiShipMast_SwayX);
  s32 swayY = Engine_MathSin(gTolbiShipMast_SwayY);

  *position++ += swayX;
  swayY *= 4;
  *position += swayY;
  gTolbiShipMast_DriftX += swayX;
  gTolbiShipMast_DriftY += swayY;
  gTolbiShipMast_SwayX += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
  gTolbiShipMast_SwayY += (u32)(Engine_RandomNext() << 9) >> 16;
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
    if (Engine_GameFlagIsSet(firstFlag + passenger) != 0)
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
    SC_POS(0, 0, 0),
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
    // Unidentified words following the script terminator.
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

struct SceneRegion gTolbiShipMast_Regions[] = {
    {176, 16, 348, 192, 32, 364, -1, 1},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gTolbiShipMast_Exits[] = {
    SCENE_EXITS(TolbiShipMast),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_CAST_OFF, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CAPTAINS_CABIN_FROM_TOLBI_SHIP_MAST_CAST_OFF),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_INTERIORS_MONSTERS, TolbiShipInteriors, TOLBI_SHIP_INTERIORS_CAPTAINS_CABIN_FROM_TOLBI_SHIP_MAST_MONSTERS),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_CREW_ATTACKED, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_CREW_ATTACKED),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_MONSTER_LANDS, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_MONSTER_LANDS),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED),
    SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE),
    SCENE_EXITS_END,
};

struct ScenePlacement gTolbiShipMast_Placements[] = {
    { 0x91, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipMast_StagingObjects[] = {
    { 0x11e, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipMast_EnsembleObjects[] = {
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(144), 0, PIXELS(169), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(134), 0, PIXELS(189), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(124), 0, PIXELS(209), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(129), 0, PIXELS(174), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(139), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(149), 0, PIXELS(204), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9b, CONDITION_ALWAYS, 7,
        PIXELS(154), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(182), 0, PIXELS(420), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipMast_LandingObjects[] = {
    { 0xc5, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gTolbiShipMast_Events[] = {
    { TOUCH_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipMast_ClimbDownLadder) },
    { EVENT_TALK, ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED, MSGNO(MsgTolbiShipWatchSeasHelpKaja) },
    { EVENT_TALK, ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipLookoutCannotRow) },
    { EVENT_TALK, ACTOR_LOOKOUT, CONDITION_ALWAYS, MSGNO(MsgTolbiShipIslandMonstersUnknown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_FIRST_ATTACK_REPELLED, MSGNO(MsgTolbiShipHiddenAnchorDiscovered) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipSeaTooVast) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOOKOUT, CONDITION_ALWAYS, MSGNO(MsgTolbiShipIslandTreasures) },
    { SEARCH(SEARCH_UNNAMED), 11, FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND, EVENT_SCRIPT(TolbiShipMast_FindAnchorCharm) },
    { SCENE_EVENTS_END },
};

/// Each passenger's rowing value, in the order of enum TolbiShipPassenger
/// (tolbi_ship_interiors.c).
u32 gTolbiShipMast_PassengerRowValues[] = {
    7, 5, 8, 4, 9, 12, 6, 3,
    0,
};
// clang-format on
