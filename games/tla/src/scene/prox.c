/// @file
/// Prox, Felix's home in the frozen north, where Puelle's clan lives in
/// poverty in the shadow of the Mars Lighthouse. Six scenes share this
/// overlay: the south gate, the lower village, the upper village to its north,
/// the insides of the houses, the basements and cave rooms under them, and the
/// Sanctum.
///
/// The party reaches Prox once the ship's cannon has broken the ice wall in the
/// Northern Reaches, and a storm rages over the village until the lighthouse
/// is lit. In the lower village the inn is the house on the left just inside
/// the south entrance, the shop is in a cave room behind the small door in the
/// east cliff, and the Sanctum is behind the wide wooden door in the west
/// cliff. On the frozen river by the upper village's wooden bridge, the Venus
/// Djinni Mold runs off and hides in a pile of snow at the river's head until
/// Scoop digs him out. Beside the upper house two warriors are overheard
/// saying they cannot find Felix's parents, and at the top of the west path
/// Puelle, the elder and the clan argue over going to the lighthouse, then
/// meet Felix. After the ending the storm is over: the clan thanks the party in
/// the big north-west house, the children play chase outside the igloo, and the
/// families say farewell at the south gate before the credits.
#include "sprite_ids.h"
#include "graphics/palette_buffers.h"
#include "resource_ids.h"
#include "field/sprite.h"
#include "field/actor_script.h"
#include "macros.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"
#include "types.h"
#include "flags.h"
#include "djinn_ids.h"
#include "system/main_routine_state.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "field/event_inline.h"
#include "shared/tables.h"
#include "shared/event_check_spheres_overlap.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/field_sequence.h"
#include "field/field_coordinate.h"
#include "field/actor.h"
#include "version.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "field/call.h"
#include "system/input.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "window/window.h"
#include "scene/magma_rock.h"
#include "field/particle.h"
#include "graphics/resource.h"
#include "system/io_reg.h"
#include "field/field_event.h"
#include "fixed_math.h"
#include "system/iwram_call.h"
#include "system/task.h"
#include "text/msg_ids.h"
#include "scene/falling_rocks.h"
#include "scene/scene.h"
#include "shop/shop.h"

/// The people at the south gate for the farewell after the ending, numbering
/// the rows of `gProx_SouthGateFarewellPlacements`; the party's rows go to
/// their party slots.
enum ProxSouthGateActorId {
  /// Isaac's father. "Don't worry, Son. I'm sure Dora's doing fine."
  ACTOR_KYLE = ACTOR_FIRST_PLACED,
  /// A man with brown hair in a green shirt, as in Vale.
  ACTOR_JENNAS_FATHER,
  /// A woman with pink hair in a white apron. "Oh, were you crying, Jenna?"
  ACTOR_JENNAS_MOTHER,
  ACTOR_SOUTH_GATE_KRADEN
};

/// The people of the lower village, numbering the rows of
/// `gProx_TownPlacements`. Each villager is named for what they talk of while
/// the lighthouse is unlit.
enum ProxTownActorId {
  /// "Felix! Welcome back to Prox! It looks like only Mars Lighthouse still
  /// needs..."
  ACTOR_VILLAGER_WELCOMING_FELIX = ACTOR_FIRST_PLACED,
  /// "Agatio and Karst left for Mars Lighthouse a little while ago..."
  ACTOR_VILLAGER_ON_AGATIO_AND_KARST,
  /// "I'm glad you made it back safely... The ice is making it very
  /// difficult..."
  ACTOR_VILLAGER_ON_THE_ICE,
  /// "We're used to the cold up here in Prox, but lately..."
  ACTOR_VILLAGER_ON_THE_COLD,
  /// "I've been keeping a close eye on the northern skies..."
  ACTOR_VILLAGER_WATCHING_THE_SKIES,
  /// "Have you seen Saturos or Menardi?"
  ACTOR_VILLAGER_ASKING_FOR_SATUROS,
  /// "Felix, you're back? And you learned how to use Psynergy?"
  ACTOR_VILLAGER_SENSING_FELIXS_POWER,
  /// "Once the final lighthouse is lit, the winds should weaken."
  ACTOR_VILLAGER_ON_THE_WINDS,
  /// The three children of `Prox_TalkToPlayingChild`,
  /// `Prox_TalkToChildWhoMissesRunning` and `Prox_TalkToTrainingChild`, who
  /// play chase outside the igloo once the beacon is lit.
  ACTOR_PLAYING_CHILD,
  ACTOR_CHILD_WHO_MISSES_RUNNING,
  ACTOR_TRAINING_CHILD,
  /// The round boulder by the wooden bridge, which Lift raises.
  ACTOR_BOULDER,
  /// Unused: a crate drawn hidden on the roof of the north-east house; nothing
  /// but Prox_InitializeTown names it.
  ACTOR_TOWN_CRATE
};

/// The people of the upper village, numbering the rows of
/// `gProx_MoldPlacements` and `gProx_MoldPlacementsOnceLit`; Isaac's row goes
/// to his party slot.
enum ProxMoldActorId {
  /// The two warriors Puelle ordered to free Felix's parents, who stand beside
  /// the upper house. "Puelle ordered me to set your parents free."
  ACTOR_WARRIOR_SENT_FOR_PARENTS = ACTOR_FIRST_PLACED,
  ACTOR_WARRIOR_SEARCHING_FOR_PARENTS,
  ACTOR_PUELLE,
  /// "I'm speaking to you too, Puelle."
  ACTOR_ELDER,
  /// The three men and three women of the clan who would go to the lighthouse
  /// in Puelle's stead.
  /// "Puelle, I can't let you go on your own..."
  ACTOR_VILLAGER_GUARDING_THE_ROAD,
  /// "If something were to happen to you, who would lead our village?"
  ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS,
  /// "I feel so helpless... Agatio has not returned..."
  ACTOR_VILLAGER_FEELING_HELPLESS,
  /// "All we can do now is wait for Agatio and his men to return..."
  ACTOR_VILLAGER_WAITING_FOR_AGATIO,
  /// "Our Psynergy feels weak..."
  ACTOR_VILLAGER_WITH_WEAK_PSYNERGY,
  /// "Puelle promised that he would return our hostages to Vale."
  ACTOR_VILLAGER_ON_THE_HOSTAGES,
  ACTOR_MOLD_KRADEN,
  ACTOR_MOLD_CRATE,
  ACTOR_MOLD
};

/// The people inside the houses and the igloo, numbering the rows of
/// `gProx_HousesPlacements` and `gProx_HousesPlacementsOnceLit`; Isaac's and
/// Jenna's rows go to their party slots.
enum ProxHousesActorId {
  /// "The rift at Mars Lighthouse gets wider every day."
  ACTOR_VILLAGER_ON_THE_RIFT_GETTING_WIDER = ACTOR_FIRST_PLACED,
  /// "Our chieftain told me that the rift is growing larger by the day..."
  ACTOR_VILLAGER_TOLD_OF_THE_RIFT,
  /// "Not too long ago, I climbed the mountains to the north to look at
  /// the..."
  ACTOR_VILLAGER_WHO_SAW_THE_RIFT,
  /// "No one thought about our needs when they extinguished the great
  /// lighthouses..."
  ACTOR_VILLAGER_ON_THE_SEALING,
  /// "If Mars Lighthouse's fires are not rekindled, Prox will be
  /// destroyed..."
  ACTOR_VILLAGER_FEARING_FOR_PROX,
  /// "We sent emissaries to Vale to tell them that Weyard was dying."
  ACTOR_OLD_MAN_ON_THE_EMISSARIES,
  /// "Vale's elders were stubborn..."
  ACTOR_VILLAGER_ON_VALES_ELDERS,
  /// The boy inside the igloo who asks Felix whether he likes igloos.
  ACTOR_VILLAGER_ON_IGLOOS,
  /// "The wind is so cold and bitter that we decided to hide out inside..."
  ACTOR_CHILD_HIDING_FROM_THE_WIND,
  /// "My parents told me that it would do me good to get some fresh air..."
  ACTOR_CHILD_SENT_OUTSIDE,
  /// "Those poor children."
  ACTOR_VILLAGER_PITYING_THE_CHILDREN,
  /// "Puelle, our chieftain, grew worried about Agatio and Karst."
  ACTOR_VILLAGER_ON_PUELLES_WORRY,
  /// "The people Saturos rescued from the storm in Vale should be waiting..."
  ACTOR_VILLAGER_ON_THE_RESCUED,
  /// "Don't forget, Felix, your mother and father are staying in the north
  /// end..."
  ACTOR_VILLAGER_POINTING_TO_PARENTS,
  /// The innkeeper behind the inn's counter.
  ACTOR_INNKEEPER,
  /// "Your mother knows that you're strong and healthy..."
  ACTOR_VILLAGER_ON_FELIXS_MOTHER,
  /// The Mercury Djinni in the upper house, which the game's text calls
  /// Water19.
  ACTOR_MERCURY_DJINNI,
  ACTOR_HOUSES_PUELLE,
  ACTOR_HOUSES_ELDER,
  ACTOR_HOUSES_KRADEN
};

/// The people of the basements and the shop's cave room, numbering the rows
/// of `gProx_CaveHomesPlacements` and `gProx_CaveHomesPlacementsOnceLit`.
enum ProxCaveHomesActorId {
  /// "Prox originally flourished because concerned Adepts lit the fires..."
  ACTOR_VILLAGER_ON_PROXS_PURPOSE = ACTOR_FIRST_PLACED,
  /// "When you left Prox, you were so young that you could barely lift a
  /// sword..."
  ACTOR_VILLAGER_REMEMBERING_YOUNG_FELIX,
  /// The shopkeeper, the old woman behind the counter in the cave room
  /// reached by the small door in the east cliff.
  ACTOR_SHOPKEEPER,
  /// "Think of your old grandma's cooking every now and then..."
  ACTOR_OLD_WOMAN_ON_HER_COOKING
};

/// The Sanctum: one old man, the healer behind its lectern.
enum ProxCavesActorId { ACTOR_HEALER = ACTOR_FIRST_PLACED };

/// The farewells at the south gate, before the credits.
#define FLAG_SAID_FAREWELL 0x9cc
/// Mold hid in the snow pile at the head of the frozen river.
#define FLAG_MOLD_HID_IN_SNOW 0xa2d
/// The door of the upper house, which the warrior comes out of, stays open.
#define FLAG_DOOR_LEFT_OPEN FLAG_SCENE_TEMP_00
/// Set while the Mars Lighthouse is unlit: rain and storm over the village.
#define FLAG_STORM_RAGING FLAG_SCENE_TEMP_01
#define FLAG_DIG_SPOT_SCOOPED FLAG_AREA_TEMP_00

/// The cells the trigger tables answer, other than doors and exits, whose
/// cells carry the number of the exit they leave by.
enum ProxCellBehavior {
  /// Upper village: the cell of the snow pile Mold hides in, kept at cell
  /// (25, 2) and copied onto the pile at (25, 9) while Scoop is cast.
  CELL_BEHAVIOR_MOLD_SNOW_PILE = 8,
  /// Upper village: the frozen river beside Mold, where he stands at cell
  /// (25, 20). The river's upper layer keeps these cells 45 columns east, at
  /// (70, 20) to (70, 22).
  CELL_BEHAVIOR_BESIDE_MOLD = 9,
  /// Upper village: the ice of the frozen river between the cliff and the
  /// wooden bridge, cells (22, 7) to (36, 21), and its upper layer's at
  /// (67, 11) to (73, 20).
  CELL_BEHAVIOR_ICE = 10,
  /// South gate: cells (8, 9) and (9, 9), a few steps south of the way into
  /// the village.
  CELL_BEHAVIOR_FAREWELL = 10,
  /// Upper village: cells (36, 20) to (37, 22), east of the upper house's
  /// door.
  CELL_BEHAVIOR_BESIDE_UPPER_HOUSE = 11,
  /// Upper village: cells (7, 16) to (9, 16), partway up the west path.
  CELL_BEHAVIOR_WEST_PATH = 12,
  /// Upper village: cells (7, 8) to (8, 10), at the top of the west path by
  /// Puelle.
  CELL_BEHAVIOR_BEFORE_PUELLE = 13,
  /// Lower village: the hidden spot east of the igloo, cell (40, 21).
  CELL_BEHAVIOR_DARK_MATTER = 100
};

extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
extern const s32 *gFieldEffect_DurationScripts[];
void Actor_SetSpriteFlags(struct Actor *actor, u32 value);
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

/// Mold, just dug out of the snow pile at the head of the frozen river,
/// joins the party: "Felix found the Venus Djinni Mold!" It runs as soon as
/// `Prox_ScoopUpMold` has set it off, while the lighthouse is still unlit.
void Prox_MoldJoins(void) {
  Djinn_JoinParty(ACTOR_MOLD, DJINNI_MOLD / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_MOLD));
}

/// Felix talks to the Mercury Djinni in the upper house, the one Puelle's
/// warrior comes out of, and it joins the party. The walkthrough lists it as a
/// Djinni from the first game that the player never caught there.
void Prox_MercuryDjinniJoins(void) {
  Djinn_JoinParty(ACTOR_MERCURY_DJINNI, DJINNI_WATER19 / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_WATER19));
}

#include "shared/position_distance.h"

extern struct ActorPlacement gProx_SouthGateFarewellPlacements[];
extern struct ActorPlacement gProx_TownPlacements[];
extern struct ActorPlacement gProx_MoldPlacements[];
extern struct ActorPlacement gProx_MoldPlacementsOnceLit[];
extern struct ActorPlacement gProx_CaveHomesPlacements[];
extern struct ActorPlacement gProx_CaveHomesPlacementsOnceLit[];
extern struct ActorPlacement gProx_CavesPlacements[];
extern struct Trigger gProx_Triggers[];
extern struct Trigger gProx_SouthGateTriggers[];
extern struct Trigger gProx_TownTriggers[];
extern struct Trigger gProx_MoldTriggers[];
extern struct Trigger gProx_CaveHomesTriggers[];
extern struct Trigger gProx_CavesTriggers[];

extern s32 gProx_KradenStepsUpToPuelle[];

extern s32 gProx_StepBackFromPuelle[];
extern s32 gLeader_WalkThroughDoorScript[];

/// Sets up the lower village: after the ending, its three children play chase
/// outside the igloo.
void Prox_InitializeTown(void);

void Prox_InitializeMold(void);

/// Sets up the houses: on the arrival from the Mars Lighthouse after the
/// ending, the clan's thanks in the big north-west house.
void Prox_InitializeHouses(void);

/// Sets up the basements: arriving from a house, Felix comes down its stairs.
void Prox_InitializeCaveHomes(void);
extern s32 gProx_FirstChildRuns[];
extern s32 gProx_SecondChildRuns[];
extern s32 gProx_ThirdChildRuns[];
/// Felix comes down the stairs from a house into its basement.
void Prox_ComeDownStairs(s32 westOrSouth);
extern s32 gProx_KradenStepsForward[];
extern s32 gProx_JennasFatherWalksSouth[];
extern s32 gProx_JennasMotherWalksSouth[];
extern s32 gProx_LeftFileWalksSouth[];
extern s32 gProx_JennaWalksSouth[];
extern s32 gProx_LeaderWalksSouth[];
extern s32 gProx_KradenWalksSouth[];
extern s32 gProx_JennaRunsAhead[];
void Leader_StopIfPending(void);
void Particle_UpdateFalling(void);

void Prox_MercuryDjinniJoins(void);

void Prox_TalkToShopkeeper(void);

void Prox_TalkToHealer(void);
void Prox_TalkToPlayingChild(void);

void Prox_TalkToTrainingChild(void);

void Prox_SearchForParents(void);

void Prox_DecideToGoToLighthouse(void);

void Prox_MeetPuelle(void);

/// Felix walks up to Mold, who stands on the frozen river just south of the
/// wooden bridge in the upper village, and Mold bolts away north under the
/// bridge and up the river. There is a thud, the ground shakes, a cry is heard,
/// and Felix is left with "...". Mold has hidden in the pile of snow at the
/// river's head, where Scoop will find him.
void Prox_MoldHidesInSnow(void);

/// Felix casts Scoop on the pile of snow at the head of the frozen river in
/// the upper village, cell (25, 9), and digs Mold out: the Djinni hops up and
/// turns to him.
void Prox_ScoopUpMold(void);
void Prox_StartScoop(void);
void Prox_EndScoop(void);

void Prox_EnterInn(void);

void Prox_EnterSouthHouse(void);
void Prox_EnterWestHouse(void);
void Prox_EnterNorthwestHouse(void);

void Prox_EnterEastHouse(void);

void Prox_EnterSanctum(void);

void Prox_EnterUpperHouse(void);
/// Felix walks north onto the stairs in one of the three basements and climbs
/// up into the house above it: the east house, the big north-west house, or
/// the inn.
void Prox_ClimbStairs(void);
void Prox_SayFarewell(void);

/// Felix steps onto the frozen river in the upper village, between the cliff
/// and the wooden bridge, and slides on the way he was going, cell by cell,
/// until a wall, a rise in the ground, a person or the end of the ice stops
/// him.
void Leader_SlideOnIce(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

/// After the ending, with the storm over, smoke rises from the chimneys of the
/// lower village's houses and of the upper house.
void Prox_SmokeFromChimneys(void);

/// The sixteenth of a turn a heading points into, as a mask.
#define HEADING_SECTOR_MASK (TRIG_TURN - FACING_STEP)
/// The frames the leader must have pushed against a chasing child before it
/// gives way.
#define CHASE_GIVE_WAY_FRAMES 120
/// How near, in pixels, a child chases another child or the leader, and how
/// near the leader stops a child in its game (from patch 1).
#define CHASE_CHILD_RANGE 24
#define CHASE_LEADER_RANGE 32
#define CHASE_STOP_RANGE 11

/// Runs a child at its target while it is near and ahead, and says whether it
/// is chasing.
s32 Prox_ChaseTarget(struct Actor *actor, struct Actor *target, s32 range,
                     s32 force) {
  s32 result = 0;
  s32 *targetPos;
  s32 *objectPos;

  // A child being talked to holds still.
  if (actor->sceneByte != 0) {
    actor->paused = 1;
    return 1;
  }
  targetPos = &target->x.fixed;
  objectPos = &actor->x.fixed;
  if (Position_Distance(targetPos, objectPos) < range || force != 0) {
    u32 angle = (u16)Trig_Atan(target->z.fixed - actor->z.fixed,
                               *targetPos - *objectPos);
    u32 left = (angle - FACING_STEP) & HEADING_SECTOR_MASK;
    u32 right = (angle + FACING_STEP) & HEADING_SECTOR_MASK;
    u32 ahead = angle & HEADING_SECTOR_MASK;
    u32 facing = actor->facing & HEADING_SECTOR_MASK;

    if (ahead == facing || right == facing || left == facing || force != 0) {
      if (gFieldState->blockedSteps > CHASE_GIVE_WAY_FRAMES) {
        actor->paused = 0;
        Actor_SelectAnimation(actor, ANIM_WALK);
      } else {
        actor->paused = 1;
        Actor_SelectAnimation(actor, ANIM_STAND);
        result = 1;
      }
    }
  } else {
    actor->paused = 0;
    Actor_SelectAnimation(actor, ANIM_WALK);
  }
  return result;
}

/// The children's game of chase after the ending: each runs at the child it
/// chases, and from patch 1 stops when the leader comes up close.
s32 Prox_PlayChase(struct Actor *actor) {
  struct WindowState *window = (struct WindowState *)Ram_HeapSlots->windowState;
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 flag = 0;
  struct Actor *target;

#if TLA_PATCH_1
  target = Actor_Get(gMainRoutineState.leaderActorId);
  if (Position_Distance(&target->x.fixed, &actor->x.fixed) <=
      CHASE_STOP_RANGE) {
    actor->paused = flag;
    Actor_SelectAnimation(actor, ANIM_WALK);
    return 0;
  }
#endif
  if (actor->timer == 0)
    target = Actor_Get(ACTOR_TRAINING_CHILD);
  else if (actor->timer == 1)
    target = Actor_Get(ACTOR_PLAYING_CHILD);
  else
    target = Actor_Get(ACTOR_CHILD_WHO_MISSES_RUNNING);
  if (Prox_ChaseTarget(actor, target, CHASE_CHILD_RANGE, 0) == 0) {
    target = Actor_Get(gMainRoutineState.leaderActorId);
    if ((state->talk != 0 || window->mindRead != 0) && (actor->timer & 2))
      flag = 1;
    Prox_ChaseTarget(actor, target, CHASE_LEADER_RANGE, flag);
  }
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gProx_SouthGatePlacements[];
extern struct ActorPlacement gProx_HousesPlacements[];
extern struct ActorPlacement gProx_HousesPlacementsOnceLit[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_PROX_SOUTH_GATE &&
      Event_GetFlag(FLAG_THANKED_FOR_BEACON) &&
      !Event_GetFlag(FLAG_SAID_FAREWELL))
    return gProx_SouthGateFarewellPlacements;
  scene = gMainRoutineState.next;
  if (scene == SCENE_PROX)
    return gProx_TownPlacements;
  if (scene == SCENE_PROX_MOLD) {
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT))
      return gProx_MoldPlacementsOnceLit;
    return gProx_MoldPlacements;
  }
  if (scene == SCENE_PROX_HOUSES) {
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
      gProx_HousesPlacementsOnceLit[ACTOR_VILLAGER_TOLD_OF_THE_RIFT -
                                    ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY_THEN_WATCH;
      gProx_HousesPlacementsOnceLit[ACTOR_VILLAGER_FEARING_FOR_PROX -
                                    ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY_THEN_WATCH;
      return gProx_HousesPlacementsOnceLit;
    }
    return gProx_HousesPlacements;
  }
  if (scene == SCENE_PROX_CAVE_HOMES) {
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT))
      return gProx_CaveHomesPlacementsOnceLit;
    return gProx_CaveHomesPlacements;
  }
  if (scene == SCENE_PROX_CAVES)
    return gProx_CavesPlacements;
  return gProx_SouthGatePlacements;
}

/// Felix casts Scoop on the hidden spot just east of the igloo, past its
/// fence among the snowy rocks, cell (40, 21), and digs up Dark Matter:
/// "Felix got some Dark Matter."
void Prox_ScoopUpDarkMatter(s32 mode) {
  Event_DigUpTreasure(mode, FIELD_TREASURE_ACTOR, FLAG_DIG_SPOT_SCOOPED);
}

extern struct Trigger gProx_HousesTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_PROX_SOUTH_GATE)
    return gProx_SouthGateTriggers;
  if (scene == SCENE_PROX)
    return gProx_TownTriggers;
  if (scene == SCENE_PROX_MOLD)
    return gProx_MoldTriggers;
  if (scene == SCENE_PROX_HOUSES)
    return gProx_HousesTriggers;
  if (scene == SCENE_PROX_CAVE_HOMES)
    return gProx_CaveHomesTriggers;
  if (scene == SCENE_PROX_CAVES)
    return gProx_CavesTriggers;
  return gProx_Triggers;
}

/// The overlay's call to Event_End.
s32 Prox_EndEvent();

/// The boy inside the igloo asks Felix whether he likes igloos. Either way, the
/// boy says the children built it to have something to do in the cold.
void Prox_AskAboutIgloos(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgProxFelixYouLikeIgloosRight);
  Event_AskYesNo(ACTOR_VILLAGER_ON_IGLOOS, 0);
  Prox_EndEvent();
}

#include "shared/leader_is_facing_north.h"

/// Prox's shops, as Shop_Run numbers them.
enum { SHOP_PROX = 31 };

/// Prox's inn, as Inn_CheckIn numbers the inns.
enum { PROX_INN = 14 };

/// Felix talks to the innkeeper behind the inn's counter. From across the
/// counter, facing north, he is offered a room; from anywhere else he hears
/// that his parents have gone away, worried about him, or after the ending
/// how the earthquake shook the inn when the beacon was lit.
void Prox_TalkToInnkeeper(void) {

  if (Leader_IsFacingNorth()) {
    Inn_CheckIn(PROX_INN, ACTOR_INNKEEPER);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
      FieldEvent_SetMessageId((s32)MsgProxWeHadNoIdeaThere);
    } else {
      FieldEvent_SetMessageId((s32)MsgProxFelixYourParentsHaveGone);
    }
    Event_ShowMessage(ACTOR_INNKEEPER, 0);
    Prox_EndEvent();
  }
}

/// Felix talks to the shopkeeper, the old woman behind the counter in the cave
/// room behind the small door in the east cliff. From across the counter,
/// facing north, she opens the shop; from anywhere else she marvels at the
/// strong warrior he has become, or after the ending says his foster
/// grandmother always knew he would return.
void Prox_TalkToShopkeeper(void) {

  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_PROX, ACTOR_SHOPKEEPER);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
      FieldEvent_SetMessageId((s32)MsgProxYourFosterGrandmotherKnew);
    } else {
      FieldEvent_SetMessageId((s32)MsgProxYouGoAwayForLittle);
    }
    Event_ShowMessage(ACTOR_SHOPKEEPER, 0);
    Prox_EndEvent();
  }
}

/// Felix talks to the healer, the old man behind the lectern in the Sanctum
/// behind the wide wooden door in the west cliff. From in front of the
/// lectern, facing north, he offers the Sanctum's services; from anywhere else
/// he speaks of Agatio and Karst going to the Mars Lighthouse, or after the
/// ending fears for Weyard.
void Prox_TalkToHealer(void) {

  if (Leader_IsFacingNorth()) {
    Sanctum_Run(ACTOR_HEALER);
  } else {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
      FieldEvent_SetMessageId((s32)MsgProxNowThatLighthouseHasBeen);
    } else {
      FieldEvent_SetMessageId((s32)MsgProxIHearAgatioAndKarst);
    }
    Event_ShowMessage(ACTOR_HEALER, 0);
    Prox_EndEvent();
  }
}

/// After the ending, Felix stops one of the three children playing chase
/// outside the igloo, the first boy. Spoken to, he turns and cries how much fun
/// it is to play outside now it has stopped snowing; under Mind Read he is glad
/// not to have to play in the igloo any more. Then he runs on.
void Prox_TalkToPlayingChild(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_Get(ACTOR_PLAYING_CHILD);
  s16 angle = actor->facing;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  actor->sceneByte = 1;
  if (state->psynergy == 0) {
    Actor_FaceActor(ACTOR_PLAYING_CHILD, gMainRoutineState.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)MsgProxWeeeeWhatFunNow);
  } else {
    FieldEvent_SetMessageId((s32)MsgProxNowThatLighthouseIsLit);
  }
  // "Now that the lighthouse is lit, we don't have to play in that stupid..."
  Event_ShowMessage(ACTOR_PLAYING_CHILD, 0);
  if (state->psynergy == 0)
    actor->facing = angle;
  actor->sceneByte = 0;
  Prox_EndEvent();
}

/// After the ending, Felix stops the girl playing chase outside the igloo.
/// Spoken to, she turns and says how she missed running around outside; under
/// Mind Read she thinks how unreal the blizzard was. Then she runs on.
void Prox_TalkToChildWhoMissesRunning(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_Get(ACTOR_CHILD_WHO_MISSES_RUNNING);
  s16 angle = actor->facing;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  actor->sceneByte = 1;
  if (state->psynergy == 0) {
    Actor_FaceActor(ACTOR_CHILD_WHO_MISSES_RUNNING,
                    gMainRoutineState.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)MsgProxIMissRunningAroundOutside);
  } else {
    FieldEvent_SetMessageId((s32)MsgProxThatBlizzardWasUnreal);
  }
  // "That blizzard was unreal, but it's gone now, and the weather's..."
  Event_ShowMessage(ACTOR_CHILD_WHO_MISSES_RUNNING, 0);
  if (state->psynergy == 0)
    actor->facing = angle;
  actor->sceneByte = 0;
  Prox_EndEvent();
}

/// After the ending, Felix stops the second boy playing chase outside the
/// igloo. Spoken to, he turns and says he must train hard to be a great warrior
/// like Felix; under Mind Read he thinks running in the snow is better
/// exercise. Then he runs on.
void Prox_TalkToTrainingChild(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_Get(ACTOR_TRAINING_CHILD);
  s16 angle = actor->facing;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  actor->sceneByte = 1;
  if (state->psynergy == 0) {
    Actor_FaceActor(ACTOR_TRAINING_CHILD, gMainRoutineState.leaderActorId, 0);
    FieldEvent_SetMessageId((s32)MsgProxIfImGoing);
  } else {
    FieldEvent_SetMessageId((s32)MsgProxYouGetMoreExerciseRunning);
  }
  // "You get more exercise running in the snow, because it's a lot harder..."
  Event_ShowMessage(ACTOR_TRAINING_CHILD, 0);
  if (state->psynergy == 0)
    actor->facing = angle;
  actor->sceneByte = 0;
  Prox_EndEvent();
}

/// Felix comes up to the upper house in the upper village, at cells (36, 20)
/// to (37, 22) just east of its door, while the lighthouse is unlit. The door
/// opens, a warrior comes out to the one waiting outside, and Felix overhears
/// them: they cannot find his parents anywhere in the village, and Puelle will
/// be upset, as he ordered them freed when Felix returned. The door stays open.
void Prox_SearchForParents(void) {
  s32 lit = Event_GetFlag(FLAG_MARS_BEACON_LIT);

  if (lit == 0) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    Event_PlaySound(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(89, 0, 77, 16, 1, 2);
    Event_SetFlag(FLAG_DOOR_LEFT_OPEN);
    FieldEvent_Wait(10);
    Actor_SetPositionAndFacing(ACTOR_WARRIOR_SENT_FOR_PARENTS, toFIXED(520),
                               toFIXED(360), FACING_SOUTH_SOUTHEAST);
    FieldCamera_GetActor()->motionFlags = lit;
    FieldCamera_SetSpeed(1.6);
    FieldCamera_MoveTo(552, CAMERA_KEEP, 388, 1);
    FieldEvent_Wait(10);
    Actor_SetSpeed(ACTOR_WARRIOR_SENT_FOR_PARENTS, 1.2);
    Actor_WalkToAndWait(ACTOR_WARRIOR_SENT_FOR_PARENTS, 526, 384);
    Actor_FaceDirection(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, FACING_WEST, 0);
    Actor_WalkToAndWait(ACTOR_WARRIOR_SENT_FOR_PARENTS, 540, 388);
    Actor_FaceDirectionAndWait(ACTOR_WARRIOR_SENT_FOR_PARENTS, FACING_EAST);
    Actor_SetAnimation(ACTOR_WARRIOR_SENT_FOR_PARENTS, 4);
    FieldEvent_SetMessageId((s32)MsgProxTheyreNotHere);
    // "They're not here..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_SENT_FOR_PARENTS, 0);
    Actor_ShowEmote(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS,
                    EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    // "Strange... We looked all over the village, and we can't find any..."
    Event_ShowMessage(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 0);
    Actor_SetAnimation(ACTOR_WARRIOR_SENT_FOR_PARENTS, 3);
    // "Everyone says they've practically vanished without a trace... It's..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_SENT_FOR_PARENTS, 0);
    Actor_ShowEmote(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS,
                    EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    Actor_FaceDirection(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS,
                        FACING_NORTH_NORTHWEST, 0);
    // "You don't think they..."
    Event_ShowMessage(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 0);
    Actor_FaceDirection(ACTOR_WARRIOR_SENT_FOR_PARENTS, FACING_NORTH_NORTHWEST,
                        40);
    Actor_FaceDirectionAndWait(ACTOR_WARRIOR_SENT_FOR_PARENTS, FACING_EAST);
    Actor_SetAnimation(ACTOR_WARRIOR_SENT_FOR_PARENTS, 4);
    // "They wouldn't have gone to the lighthouse..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_SENT_FOR_PARENTS, 0);
    Actor_FaceDirectionAndWait(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS,
                               FACING_WEST);
    Actor_SetAnimationAndWait(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 3);
    // "Yeah, you're probably right."
    Event_ShowMessage(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 0);
    Actor_ShowEmote(ACTOR_WARRIOR_SENT_FOR_PARENTS,
                    EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "Puelle's going to be upset that we didn't follow his order to free..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_SENT_FOR_PARENTS, 0);
    Actor_SetAnimation(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 3);
    // "He was clear about it, too... "When Felix returns to Prox, set his..."
    Event_ShowMessage(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 0);
    Actor_FaceDirection(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS,
                        FACING_NORTH_NORTHWEST, 0);
    Actor_FaceDirectionAndWait(ACTOR_WARRIOR_SENT_FOR_PARENTS,
                               FACING_NORTH_NORTHWEST);
    Event_SetFlag(FLAG_HEARD_OF_PARENTS);
    Prox_EndEvent();
  }
}

void FieldCamera_WaitForArrival(void);

/// Felix walks partway up the west path in the upper village, cells (7, 16) to
/// (9, 16), and the camera moves up to its top, where Puelle tells the clan
/// they cannot wait for Agatio any longer and he will go to the Mars
/// Lighthouse himself. The villagers beg to go in his stead, the elder forbids
/// them all, and Puelle is left to wait for a miracle.
void Prox_DecideToGoToLighthouse(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(160, CAMERA_KEEP, 160, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_PUELLE, 1);
  Actor_FaceDirectionAndWait(ACTOR_PUELLE, FACING_NORTH_NORTHWEST);
  FieldEvent_SetMessageId((s32)MsgProxWeCantWaitForAgatio);
  // "We can't wait for Agatio and his men any longer..."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_GUARDING_THE_ROAD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_FEELING_HELPLESS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_WAITING_FOR_AGATIO,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_ON_THE_HOSTAGES,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST);
  // "If we wait much longer, this wind and snow will make it impossible..."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  // "I'm going to the lighthouse... Will any of you come with me?"
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_GUARDING_THE_ROAD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_FEELING_HELPLESS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_WAITING_FOR_AGATIO,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_ON_THE_HOSTAGES,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_VILLAGER_GUARDING_THE_ROAD, 4);
  // "Puelle, I can't let you go on your own..."
  Event_ShowMessage(ACTOR_VILLAGER_GUARDING_THE_ROAD, 0);
  Actor_FaceDirection(ACTOR_PUELLE, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SetAnimationAndWait(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, 4);
  // "If something were to happen to you, who would lead our village?"
  Event_ShowMessage(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, 0);
  FieldActor_Jump(ACTOR_VILLAGER_WAITING_FOR_AGATIO, 4, 10);
  // "Please, you must let us go in your stead."
  Event_ShowMessage(ACTOR_VILLAGER_WAITING_FOR_AGATIO, 0);
  Actor_FaceDirection(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "How can you expect to accomplish what Agatio and Karst could not?"
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_ShowEmote(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY,
                  EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  // "They may have been more powerful than any of us, but we can't just..."
  Event_ShowMessage(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, 0);
  FieldActor_Jump(ACTOR_VILLAGER_FEELING_HELPLESS, 2, 0);
  Actor_SetAnimation(ACTOR_VILLAGER_FEELING_HELPLESS, 3);
  // "They're not coming back, just like Saturos... You have to let us try."
  Event_ShowMessage(ACTOR_VILLAGER_FEELING_HELPLESS, 0);
  // "I can't. I can't allow it."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_FaceDirectionAndWait(ACTOR_PUELLE, FACING_WEST);
  Actor_ShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_FaceDirectionAndWait(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST);
  // "Listen to your elder... He speaks wisely."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_FaceDirectionAndWait(ACTOR_ELDER, FACING_EAST);
  // "I'm speaking to you too, Puelle."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_ShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(ACTOR_PUELLE, FACING_WEST);
  Actor_SetAnimation(ACTOR_ELDER, 4);
  // "If Agatio and Karst cannot do the task, how can you, a single man..."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_SetAttachedEffect(ACTOR_PUELLE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_VILLAGER_GUARDING_THE_ROAD, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_FEELING_HELPLESS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_WAITING_FOR_AGATIO, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_ON_THE_HOSTAGES, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_VILLAGER_GUARDING_THE_ROAD, FACING_NORTH_NORTHWEST,
                      0);
  Actor_FaceDirection(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS,
                      FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_FEELING_HELPLESS, FACING_NORTH_NORTHEAST,
                      0);
  Actor_FaceDirection(ACTOR_VILLAGER_WAITING_FOR_AGATIO, FACING_NORTH_NORTHEAST,
                      0);
  Actor_FaceDirection(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FACING_NORTH_NORTHEAST,
                      0);
  Actor_FaceDirection(ACTOR_VILLAGER_ON_THE_HOSTAGES, FACING_NORTH_NORTHEAST,
                      20);
  // "What are you saying? If we stand around debating, our town is doomed..."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "We can't risk sacrificing any more lives."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_FaceDirection(ACTOR_PUELLE, FACING_NORTH_NORTHWEST, 80);
  Actor_SetAnimationAndWait(ACTOR_PUELLE, 4);
  // "So, we must sit here and wait for a miracle to come."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Event_SetFlag(FLAG_CLAN_SPOKE_OF_LIGHTHOUSE);
  Prox_EndEvent();
}

s32 Actor_PlaceOnActor();
extern s32 gProx_IsaacStepsUpToPuelle[];

/// Felix reaches the top of the west path in the upper village, cells (7, 8)
/// to (8, 10), and walks up to Puelle and the elder before the clan. The elder
/// knows him at once; Kraden and Isaac introduce themselves, Puelle names
/// Isaac as the one Agatio said killed Saturos and Menardi, and Isaac and
/// Kraden tell how it happened. Talk turns to Agatio and Karst, who have the
/// Mars Star now, and to the lighthouse.
void Prox_MeetPuelle(void) {
  u32 i;
  s32 result;
  s32 owner;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(160, CAMERA_KEEP, 160, 1);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 134, 156);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_NORTH);
  FieldActor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(ACTOR_ELDER, FACING_SOUTH_SOUTHWEST);
  FieldEvent_SetMessageId((s32)MsgProxYouFelix);
  // "You... Felix, it's you..."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_ShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_PUELLE, FACING_WEST, 40);
  // "I'd heard that you had returned to us... You look so much stronger..."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 144, 154);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_PlaceOnActor(ACTOR_MOLD_KRADEN, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_ISAAC, gMainRoutineState.leaderActorId);
  System_Wait(1);
  Actor_FaceDirection(ACTOR_VILLAGER_FEELING_HELPLESS, FACING_NORTH_NORTHWEST,
                      0);
  Actor_SetSpeed(ACTOR_MOLD_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_ISAAC, 0.8);
  Actor_SetScript(ACTOR_ISAAC, gProx_IsaacStepsUpToPuelle);
  Actor_RunScriptAndWait(ACTOR_MOLD_KRADEN, gProx_KradenStepsUpToPuelle);
  Actor_FaceDirection(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "It seems he has brought some allies with him."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_SetAnimation(ACTOR_MOLD_KRADEN, 3);
  // "Yes... I am Kraden."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  // "My name is Isaac."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC,
                    0);
  Actor_FaceDirection(ACTOR_ELDER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PUELLE, FACING_WEST, 40);
  Actor_FaceDirectionAndWait(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST);
  // "Isaac... Isn't that..."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_FaceDirectionAndWait(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST);
  // "That is the name of the one Agatio said had killed Saturos and Menardi."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_GUARDING_THE_ROAD, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_FEELING_HELPLESS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_WAITING_FOR_AGATIO, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_ON_THE_HOSTAGES, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  Actor_SetSpeed(ACTOR_ISAAC, 2.4);
  Actor_Get(ACTOR_ISAAC)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_ISAAC, 160, 140);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_ISAAC)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetAnimationAndWait(ACTOR_ISAAC, 21);
  Actor_FaceDirection(ACTOR_VILLAGER_GUARDING_THE_ROAD, FACING_NORTH_NORTHWEST,
                      0);
  Actor_FaceDirection(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS,
                      FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_FEELING_HELPLESS, FACING_NORTH_NORTHWEST,
                      0);
  Actor_FaceDirection(ACTOR_VILLAGER_WAITING_FOR_AGATIO, FACING_NORTH_NORTHEAST,
                      0);
  Actor_FaceDirection(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FACING_NORTH_NORTHEAST,
                      0);
  Actor_FaceDirection(ACTOR_VILLAGER_ON_THE_HOSTAGES, FACING_NORTH_NORTHEAST,
                      0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOLD_KRADEN, FACING_NORTH_NORTHWEST);
  // "Isaac, stop that..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_STILL);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_STAND);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
  Actor_SetAnimation(ACTOR_MOLD_KRADEN, 3);
  // "You are correct, but Isaac had reasons for fighting them."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_SetSpeed(ACTOR_ISAAC, 0.8);
  Actor_WalkToAndWait(ACTOR_ISAAC, 160, 154);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_MOLD_KRADEN, 4);
  // "Saturos and Menardi had invaded Vale, stolen its sacred treasure..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_MOLD_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                       20);
  // "You cannot blame Isaac for pursuing them and fighting to regain what..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "We only wanted to stop them... We didn't think they'd fight us to..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC,
                    0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MOLD_KRADEN, FACING_WEST, 40);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
  // "They flung themselves into the opening of Venus Lighthouse's beacon..."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC,
                    0);
  // A pause the player can cut short by holding a key.
  for (i = 0; i < 80; i++) {
    System_Wait(1);
    if (gInput.held != 0)
      break;
  }
  Actor_RunRepeatedMotion(ACTOR_VILLAGER_GUARDING_THE_ROAD, 1);
  // "Did...Saturos and Menardi put up a good fight?"
  Event_ShowMessage(ACTOR_VILLAGER_GUARDING_THE_ROAD, 0);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  FieldActor_ShowEmote(ACTOR_VILLAGER_WAITING_FOR_AGATIO,
                       EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "I'll bet you thought you didn't stand a chance against them, huh?"
  Event_ShowMessage(ACTOR_VILLAGER_WAITING_FOR_AGATIO, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Actor_FaceDirection(ACTOR_PUELLE, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ELDER, FACING_EAST);
  Actor_SetAnimation(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, 3);
  // "You were probably really tense the whole time..."
  Event_ShowMessage(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Actor_FaceDirection(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, 3);
  // "Saturos and Menardi had a job to do... That was all."
  Event_ShowMessage(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHWEST);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Actor_SetAnimation(ACTOR_VILLAGER_FEELING_HELPLESS, 3);
  // "If I'd been in your shoes, I'd have done the same thing... I can't..."
  Event_ShowMessage(ACTOR_VILLAGER_FEELING_HELPLESS, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_VILLAGER_ON_THE_HOSTAGES, 4);
  // "And in the end, you were stronger than they were... You should be..."
  Event_ShowMessage(ACTOR_VILLAGER_ON_THE_HOSTAGES, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 20);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MOLD_KRADEN, FACING_NORTH_NORTHWEST, 20);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  // "I am sad that they lost their lives in their quest, but there was no..."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 40);
  Actor_SetAnimationAndWait(ACTOR_PUELLE, 3);
  // "What has happened cannot be undone. We should cooperate to find a..."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOLD_KRADEN, FACING_NORTH_NORTHEAST);
  Actor_SetAnimation(ACTOR_MOLD_KRADEN, 3);
  // "We understand that Agatio and Karst have made for Mars Lighthouse..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
  Actor_SetAnimation(ACTOR_ELDER, 3);
  // "Then all that remains for us is to wait until Gaia Falls carries us..."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
  // "It seems that Mars Lighthouse is as dangerous as all the rest."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC,
                    0);
  Actor_SetAnimation(ACTOR_PUELLE, 3);
  // "We have only a few soldiers remaining, and none who can make the..."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  // "Only Saturos and perhaps Agatio were strong enough to reach the top..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
  Actor_SetAnimation(ACTOR_ELDER, 4);
  // "And none of them can help us now."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_WEST);
  // "You think we can do it?"
  // Yes: "I agree, Felix..."; No: "Felix, what are you thinking?"
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC,
                    0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  result =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0);
  owner = gMainRoutineState.leaderActorId;
  if (result == 0) {
    Actor_WaitForAnimationChange(owner);
    // "I agree, Felix..."
    Event_ShowMessage(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
    gFieldState->message++;
  } else {
    FieldActor_WaitForMoveById(owner);
    gFieldState->message++;
    // "Felix, what are you thinking?"
    Event_ShowMessage(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
  }
  FieldActor_ShowEmote(ACTOR_MOLD_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOLD_KRADEN, FACING_WEST);
  // "We've climbed three lighthouses already... What's one more? Let's go..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
  Actor_FaceDirection(ACTOR_ELDER, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PUELLE, FACING_WEST);
  Actor_SetAnimation(ACTOR_ELDER, 3);
  Actor_SetAnimationAndWait(ACTOR_PUELLE, 3);
  Actor_FaceDirection(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "They've proven themselves strong enough! I say we trust in Felix to..."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_SetAnimation(ACTOR_ELDER, 3);
  // "This is an unexpected turn of events, but yes, I think you can light..."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_MOLD_KRADEN, FACING_NORTHWEST);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MOLD_KRADEN, 3);
  Actor_FaceDirection(ACTOR_VILLAGER_GUARDING_THE_ROAD, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_FEELING_HELPLESS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_WAITING_FOR_AGATIO, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_VILLAGER_ON_THE_HOSTAGES, FACING_EAST);
  Actor_SetAnimation(ACTOR_VILLAGER_GUARDING_THE_ROAD, 3);
  Actor_SetAnimation(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, 3);
  Actor_SetAnimation(ACTOR_VILLAGER_FEELING_HELPLESS, 3);
  Actor_SetAnimation(ACTOR_VILLAGER_WAITING_FOR_AGATIO, 3);
  Actor_SetAnimation(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, 3);
  Actor_SetAnimationAndWait(ACTOR_VILLAGER_ON_THE_HOSTAGES, 3);
  Actor_FaceDirectionAndWait(ACTOR_MOLD_KRADEN, FACING_WEST);
  // "Off to the Mars Lighthouse! Let's go, Felix! Come on, Isaac!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetScript(ACTOR_ISAAC, gProx_StepBackFromPuelle);
  Actor_RunScriptAndWait(ACTOR_MOLD_KRADEN, gProx_StepBackFromPuelle);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 160, 154);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_NORTH);
  Actor_RunRepeatedMotion(ACTOR_PUELLE, 1);
  // "There is one thing that I don't want you to forget."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHEAST, 20);
  Actor_SetAnimation(ACTOR_PUELLE, 4);
  // "In order to light the beacon at Mars Lighthouse, you will need to..."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_ShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(ACTOR_ELDER, FACING_EAST);
  // "Ah! And Agatio and Karst have the Mars Star now!"
  Event_ShowMessage(ACTOR_ELDER, 0);
  Actor_FaceDirectionAndWait(ACTOR_PUELLE, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_PUELLE, 3);
  Actor_FaceDirectionAndWait(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST);
  // "You must find Agatio and Karst, and you must take the Mars Star from..."
  Event_ShowMessage(ACTOR_PUELLE, 0);
  Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_FaceDirectionAndWait(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ELDER, 3);
  // "Thanks for your help. The future of Weyard rests in your hands."
  Event_ShowMessage(ACTOR_ELDER, 0);
  Event_SetFlag(FLAG_MET_PUELLE);
  Prox_EndEvent();
}

void Prox_MoldHidesInSnow(void) {
  struct Actor *actor;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 2);
  Actor_SetSpeed(ACTOR_MOLD, 2);
  actor = Actor_Get(ACTOR_MOLD);
  actor->layer = MAPGRID_LAYER_MIDDLE;
  FieldActor_SetSpritePriority(ACTOR_MOLD, 3);
  actor = Actor_Get(gMainRoutineState.leaderActorId);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 408, 326);
  FieldEvent_Wait(1);
  actor = Actor_Get(gMainRoutineState.leaderActorId);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Event_PlaySound(SE_RATTLE);
  actor = FieldCamera_GetActor();
  actor->y.fixed += toFIXED(2);
  actor = Actor_Get(ACTOR_MOLD);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_MOLD, 408, 200);
  FieldEvent_Wait(1);
  actor = Actor_Get(ACTOR_MOLD);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetPosition(ACTOR_MOLD, 0, 0);
  Event_PlaySound(SE_KOLIMA_BRIDGE_HOP);
  MapGrid_SetShake(1, 2, 1);
  FieldEvent_Wait(30);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SE_LANDING_THUD);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_ENEMY_CRY);
  FieldEvent_Wait(20);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldActor_SetSpritePriority(ACTOR_MOLD, 2);
  actor = Actor_Get(ACTOR_MOLD);
  actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  // The snow pile moves: the upper layer's cells (69, 3) and (69, 8), kept 45
  // columns east of (24, 3) and (24, 8), lose the round pile on the ledge
  // atop the cliff and gain a low mound on the frozen river at the cliff's
  // foot, the pile Scoop digs Mold out of (Prox_ScoopUpMold).
  MapGrid_CopyCellsTo(72, 3, 69, 3, 3, 3);
  MapGrid_CopyCellsTo(48, 36, 69, 8, 3, 2);
  MapGrid_CopyCellAttributes(72, 13, 1, 3, 70, 20);
  Event_SetFlag(FLAG_MOLD_HID_IN_SNOW);
  Prox_EndEvent();
}

/// The raised trigger `Prox_ScoopUpMold` leaves for `Prox_MoldJoins`.
enum ProxRaisedTrigger { RAISED_MOLD_JOINS = 200 };

void Prox_ScoopUpMold(void) {
  if (Event_GetFlag(FLAG_MARS_BEACON_LIT) == 0 &&
      Event_GetFlag(FLAG_DJINNI(DJINNI_MOLD)) == 0) {
    struct FieldState *state = Ram_HeapSlots->fieldState;

    Event_Start();
    FieldEvent_EndLastingPsynergy(1);
    Actor_SetPositionAndFacing(ACTOR_MOLD, toFIXED(408), toFIXED(145),
                               FACING_SOUTH);
    System_Wait(1);
    FieldActor_Jump(ACTOR_MOLD, 4, 20);
    Actor_FaceActor(ACTOR_MOLD, gMainRoutineState.leaderActorId, 0);
    state->raisedTrigger = RAISED_MOLD_JOINS;
    Prox_EndEvent();
  }
}

/// Nothing visible happens when Felix starts to cast Scoop facing a wall in
/// the upper village. The game quietly turns the snow pile at the head of the
/// frozen river, cell (25, 9), from a wall into Mold's hiding place for the
/// length of the cast, so that Scoop aimed at the pile can find him.
void Prox_StartScoop(void) { MapGrid_CopyCellAttributes(25, 2, 1, 1, 25, 9); }

/// Nothing visible happens when Scoop's cast ends in the upper village: the
/// snow pile at the head of the frozen river, cell (25, 9), is a wall again,
/// as Felix found it.
void Prox_EndScoop(void) { MapGrid_CopyCellAttributes(25, 3, 1, 1, 25, 9); }

#include "shared/smoke_drift.h"

#include "shared/smoke_spawn.h"

void Prox_SmokeFromChimneys(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_PROX) {
    if ((gFrameCount & 63) == 0)
      Smoke_Spawn(toFIXED(536), toFIXED(40), toFIXED(232));
    if (((gFrameCount + 60) & 63) == 0)
      Smoke_Spawn(toFIXED(168), toFIXED(40), toFIXED(296));
    if (((gFrameCount + 20) & 63) == 0) {
      Smoke_Spawn(toFIXED(280), toFIXED(42), toFIXED(184));
      Smoke_Spawn(toFIXED(152), toFIXED(48), toFIXED(424));
    }
    if (((gFrameCount + 40) & 63) == 0)
      Smoke_Spawn(toFIXED(336), toFIXED(42), toFIXED(362));
  } else if (scene == SCENE_PROX_MOLD) {
    if ((gFrameCount & 63) == 0)
      Smoke_Spawn(toFIXED(568), toFIXED(104), toFIXED(296));
  }
}

/// Felix walks into a door: it opens (the Sanctum's wide door with a thump)
/// unless it was left open, he steps to the middle of a single door, walks in,
/// growing smaller as he passes the doorway, and the screen fades.
///
/// The open door is drawn by copying its picture over the door's cells, from
/// `sourceX`, `sourceY` to `x`, `y`, `width` by `height` cells; the copies sit
/// on the map's upper layer, 45 columns east of the cells Felix walks on.
void Prox_EnterDoor(s32 sourceX, s32 sourceY, s32 x, s32 y, s32 width,
                    s32 height) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor;
  s32 cell;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (!Event_GetFlag(FLAG_DOOR_LEFT_OPEN)) {
    if (width == 2)
      Event_PlaySound(SE_THUMP);
    else
      Event_PlaySound(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(sourceX, sourceY, x, y, width, height);
    FieldEvent_Wait(20);
  }
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  if (width == 1) {
    actor = Actor_Get(gMainRoutineState.leaderActorId);
    cell = FIXEDtoINT(actor->x.fixed);
    FieldActor_SetMoveTargetFX(
        actor,
        toFIXED(MAPGRID_CELL_PIXELS / 2 - cell % MAPGRID_CELL_PIXELS) +
            actor->x.fixed,
        actor->y.fixed, actor->z.fixed);
    FieldActor_WaitForMove(actor);
    actor->facing = FACING_NORTH;
  }
  Actor_SetScript(gMainRoutineState.leaderActorId,
                  gLeader_WalkThroughDoorScript);
  System_Wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(state->stepTrigger);
}

/// Felix walks north into the door of the inn, the house on the left just
/// inside the south entrance of the lower village, cell (12, 28).
void Prox_EnterInn(void) { Prox_EnterDoor(48, 39, 57, 26, 1, 2); }

/// Felix walks north into the door of the house east of the inn, cell
/// (20, 26).
void Prox_EnterSouthHouse(void) { Prox_EnterDoor(49, 39, 65, 24, 1, 2); }

/// Felix walks north into the door of the house north of the inn, on the west
/// side of the lower village, cell (11, 21).
void Prox_EnterWestHouse(void) { Prox_EnterDoor(50, 39, 56, 19, 1, 2); }

/// Felix walks north into the door of the big house in the north-west of the
/// lower village, cell (16, 15).
void Prox_EnterNorthwestHouse(void) { Prox_EnterDoor(51, 39, 61, 13, 1, 2); }

/// Felix walks north into the door of the house in the east of the lower
/// village, north of the igloo, cell (35, 18).
void Prox_EnterEastHouse(void) { Prox_EnterDoor(51, 39, 80, 16, 1, 2); }

/// Felix walks north into the wide wooden door in the west cliff of the lower
/// village, cells (5, 13) and (6, 13), and enters the Sanctum.
void Prox_EnterSanctum(void) { Prox_EnterDoor(52, 39, 50, 9, 2, 2); }

/// Felix walks north into the small door in the east cliff of the lower
/// village, cell (35, 11), and enters the cave room where the shop is.
void Prox_EnterShopCave(void) { Prox_EnterDoor(87, 2, 80, 7, 1, 2); }

/// Felix walks north into the door of the upper house in the upper village,
/// cell (32, 22), which stands open once the warrior has come out of it.
void Prox_EnterUpperHouse(void) { Prox_EnterDoor(89, 0, 77, 16, 1, 2); }

void Prox_ClimbStairs(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 stairsExit = state->stepTrigger;
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  s32 cell;
  s32 id;

  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  cell = FIXEDtoINT(actor->x.fixed);
  FieldActor_SetMoveTargetFX(
      actor,
      toFIXED(MAPGRID_CELL_PIXELS / 2 - cell % MAPGRID_CELL_PIXELS) +
          actor->x.fixed,
      actor->y.fixed, actor->z.fixed - toFIXED(4));
  FieldActor_WaitForMove(actor);
  actor->facing = FACING_NORTH;
  Event_PlaySound(SE_STAIRS_UP);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId), 0);
  id = gMainRoutineState.leaderActorId;
  if (stairsExit == PROX_CAVE_HOMES_TO_PROX_HOUSES_NORTH)
    Actor_SetAnimation(id, ANIM_LEADER_LEAVE_ALTERNATE);
  else
    Actor_SetAnimation(id, ANIM_LEADER_LEAVE);
  System_Wait(4);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(stairsExit);
}

/// Felix comes down the stairs from a house into its basement: he appears on
/// the stairs, steps down and walks a few steps on. `westOrSouth` is set for
/// the stairs under the big north-west house and the inn.
void Prox_ComeDownStairs(s32 westOrSouth) {
  struct Actor *actor;

  actor = Actor_Get(gMainRoutineState.leaderActorId);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (westOrSouth != 0) {
    // Without his shadow on the stairs, until he is down.
    Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId), 0);
    FieldActor_SetPositionAndResetMotion(actor, actor->x.fixed - toFIXED(1),
                                         actor->y.fixed,
                                         actor->z.fixed - toFIXED(6));
    System_Wait(1);
    Event_StartScreenTransition();
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId,
                              ANIM_LEADER_ARRIVE_ALTERNATE);
  } else {
    FieldActor_SetPositionAndResetMotion(actor, actor->x.fixed + toFIXED(1),
                                         actor->y.fixed,
                                         actor->z.fixed - toFIXED(6));
    System_Wait(1);
    Event_StartScreenTransition();
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId,
                              ANIM_LEADER_ARRIVE);
  }
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId),
                       SPRITE_FLAG_SHADOW);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  Actor_SetDestinationOffset(gMainRoutineState.leaderActorId, 0, 8);
  FieldActor_WaitForMoveById(gMainRoutineState.leaderActorId);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
  Prox_EndEvent();
}
void FieldCamera_WaitForArrival(void);
s32 Prox_EndEvent();

/// After the ending, the party arrives from the Mars Lighthouse in the big
/// north-west house, where Felix, Isaac and Kraden stand before Puelle and the
/// elder. The elder thanks them for lighting the beacon; talk turns to the
/// Golden Sun forming over Mt. Aleph, and to how the beacon's Psynergy saved
/// Felix's parents. Then Jenna calls for Isaac to hurry up, Isaac asks Felix
/// whether to tell her they are leaving now, a child runs out of the house,
/// and Isaac and Kraden take their leave and rejoin Felix.
void Prox_GiveThanksForBeacon(void) {
  s32 answer;
  struct Actor *leader;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldActor_Stop(ACTOR_VILLAGER_ON_THE_RESCUED);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 184, 484);
  Actor_SetPositionAndFacing(ACTOR_ISAAC, toFIXED(168), toFIXED(484),
                             FACING_NORTH);
  Actor_SetPositionAndFacing(ACTOR_HOUSES_KRADEN, toFIXED(200), toFIXED(484),
                             FACING_NORTH_NORTHWEST);
  Actor_SetPositionAndFacing(ACTOR_CHILD_SENT_OUTSIDE, toFIXED(138),
                             toFIXED(412), FACING_SOUTH_SOUTHEAST);
  Actor_SetPositionAndFacing(ACTOR_VILLAGER_ON_THE_RESCUED, toFIXED(220),
                             toFIXED(600), FACING_NORTH_NORTHWEST);
  Actor_SetPosition(ACTOR_JENNA, 184, 684);
  System_Wait(1);
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 448, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)MsgProxThankYouFelix);
  // "Thank you, Felix. And you, too, Isaac and Kraden."
  Event_ShowMessage(ACTOR_HOUSES_ELDER, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(ACTOR_HOUSES_KRADEN, 3);
  Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_FaceDirection(ACTOR_HOUSES_ELDER, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_ShowEmote(ACTOR_HOUSES_ELDER, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "By igniting Mars Lighthouse, you stopped Gaia Falls from devouring..."
  Event_ShowMessage(ACTOR_HOUSES_ELDER, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 20);
  FieldActor_ShowEmote(gMainRoutineState.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_NORTH);
  // "We're just glad to have helped save Weyard..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  Actor_SetAttachedEffect(ACTOR_HOUSES_PUELLE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "You've been through many trials, more than any of us can ever know..."
  Event_ShowMessage(ACTOR_HOUSES_PUELLE, 0);
  Actor_SetAnimation(ACTOR_HOUSES_KRADEN, 4);
  // "We had no idea what to expect when we lit the final beacon..."
  Event_ShowMessage(ACTOR_HOUSES_KRADEN, 0);
  Actor_SetAnimation(ACTOR_VILLAGER_ON_PUELLES_WORRY, 4);
  // "When we saw the flash of the beacon, we had no idea what you..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_VILLAGER_ON_PUELLES_WORRY, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "I thought we were done for... There was no way we could escape with..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  Actor_FaceDirection(ACTOR_HOUSES_KRADEN, FACING_WEST, 20);
  FieldActor_ShowEmote(ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  // "Well, at least, not if we hadn't pulled together and overcome our..."
  Event_ShowMessage(ACTOR_HOUSES_KRADEN, 0);
  // "That light was brilliant!"
  Event_ShowMessage(ACTOR_CHILD_SENT_OUTSIDE, 0);
  Actor_FaceDirection(ACTOR_HOUSES_KRADEN, FACING_NORTH_NORTHWEST, 0);
  FieldActor_ShowEmote(ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                       20);
  // "You know, I shall always regret that I didn't get to look upon the..."
  Event_ShowMessage(ACTOR_HOUSES_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_HOUSES_ELDER, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimation(ACTOR_HOUSES_ELDER, 4);
  // "I understand your regret... As a scholar of Alchemy, you would have..."
  Event_ShowMessage(ACTOR_HOUSES_ELDER, 0);
  // "I saw the beams of blue, red, and purple streaming out toward the..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  Actor_SetAttachedEffect(ACTOR_VILLAGER_ON_PUELLES_WORRY,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "That's the direction in which Vale lies, is it not? Was the light..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_VILLAGER_ON_PUELLES_WORRY, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  // "Indeed it was. The Wise One said the beams were going to merge into..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  Actor_FaceDirection(ACTOR_HOUSES_ELDER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_SetAttachedEffect(ACTOR_HOUSES_ELDER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "The orb of golden light we saw... That was shining directly over Vale?"
  Event_ShowMessage(ACTOR_HOUSES_ELDER, 0);
  Actor_ShowEmote(ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "Yes. That was the Golden Sun forming over Mt. Aleph."
  Event_ShowMessage(ACTOR_HOUSES_KRADEN, 0);
  Actor_FaceDirection(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(ACTOR_HOUSES_PUELLE, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  // "If I remember the tales correctly... The Golden Sun is the blast of..."
  Event_ShowMessage(ACTOR_HOUSES_PUELLE, 0);
  Actor_ShowEmote(ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Such legends exist even here? I wish that I'd known that sooner!"
  Event_ShowMessage(ACTOR_HOUSES_KRADEN, 0);
  Actor_FaceDirection(ACTOR_HOUSES_ELDER, FACING_SOUTH_SOUTHEAST, 0);
  // "I fear for Vale, to think that all this energy is being released..."
  Event_ShowMessage(ACTOR_HOUSES_ELDER, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  // "We're all worried about that. We're hoping we can return to Vale as..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  Actor_FaceDirection(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(ACTOR_HOUSES_PUELLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "Oh... Is there nothing we can do to convince you to stay?"
  Event_ShowMessage(ACTOR_HOUSES_PUELLE, 0);
  Actor_SetAnimation(ACTOR_HOUSES_ELDER, 4);
  // "We had hoped you might stay in Prox a while, but that sounds unlikely..."
  FieldEvent_ShowMessageThenWait(ACTOR_HOUSES_ELDER, 0, 20);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  // "Don't worry... We plan on returning as soon as things quiet down..."
  // Yes: "Our village will not soon forget..."; No: "You have just completed a
  // long and difficult journey..."
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                             0)) {
    Actor_SetAnimationAndWait(ACTOR_HOUSES_ELDER, 3);
    // "Our village will not soon forget that you saved us from the edge of..."
    Event_ShowMessage(ACTOR_HOUSES_ELDER, 0);
    gFieldState->message++;
  } else {
    gFieldState->message++;
    Actor_SetAnimationAndWait(ACTOR_HOUSES_ELDER, 4);
    // "You have just completed a long and difficult journey. Perhaps when..."
    Event_ShowMessage(ACTOR_HOUSES_ELDER, 0);
  }
  Actor_FaceDirectionAndWait(ACTOR_HOUSES_KRADEN, FACING_WEST);
  Actor_SetAnimation(ACTOR_HOUSES_KRADEN, 4);
  // "When that time comes, I may no longer be in this world."
  Event_ShowMessage(ACTOR_HOUSES_KRADEN, 0);
  Actor_FaceDirection(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_HOUSES_PUELLE, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "Don't say such things... I'm sure a long life awaits you."
  Event_ShowMessage(ACTOR_HOUSES_PUELLE, 0);
  Actor_FaceDirection(ACTOR_HOUSES_KRADEN, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_HOUSES_KRADEN, 3);
  Actor_FaceDirectionAndWait(ACTOR_HOUSES_KRADEN, FACING_WEST);
  // "Well, I suppose we should be departing for Vale soon."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HOUSES_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
  // "You're right, Kraden... If we don't, you-know-who might start..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
  Actor_SetAttachedEffect(ACTOR_HOUSES_ELDER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "At least some of your cares have been remedied... You must be glad..."
  Event_ShowMessage(ACTOR_HOUSES_ELDER, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_HOUSES_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HOUSES_PUELLE, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_HOUSES_PUELLE, 3);
  Actor_FaceDirectionAndWait(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHWEST);
  // "I would not have thought it possible had I not seen it."
  Event_ShowMessage(ACTOR_HOUSES_PUELLE, 0);
  FieldActor_ShowEmote(ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                       20);
  // "Even I do not fully understand it..."
  Event_ShowMessage(ACTOR_HOUSES_KRADEN, 0);
  Actor_SetAnimation(ACTOR_HOUSES_KRADEN, 4);
  // "But their can be no doubt. The fire Psynergy released by the beacon..."
  Event_ShowMessage(ACTOR_HOUSES_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_ShowEmote(gMainRoutineState.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(ACTOR_HOUSES_ELDER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_HOUSES_PUELLE, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_SetAnimationAndWait(ACTOR_HOUSES_KRADEN, 3);
  // "I hadn't expected so much Psynergy to be released by the..."
  Event_ShowMessage(ACTOR_HOUSES_KRADEN, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(ACTOR_HOUSES_ELDER, 3);
  Actor_SetAnimation(ACTOR_HOUSES_PUELLE, 3);
  Actor_FaceDirection(ACTOR_HOUSES_KRADEN, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  // "I have no real explanation, other than that their life force had not..."
  Event_ShowMessage(ACTOR_HOUSES_KRADEN, 0);
  Actor_ShowEmote(ACTOR_HOUSES_ELDER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "And had your parents not been Adepts, the wave of Psynergy would..."
  FieldEvent_ShowMessageThenWait(ACTOR_HOUSES_ELDER, 0, 20);
  // "Isaac! What's taking you two so long? Don't keep me waiting!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAttachedEffect(ACTOR_HOUSES_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_HOUSES_KRADEN, FACING_SOUTH_SOUTHWEST);
  // "Uh-oh! It's Jenna... We're late, and she looks rather displeased..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HOUSES_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  // "Should I let her know we're leaving now?"
  // Yes: "I'm sorry! We're almost done..."; No: "I'm sorry, Jenna! It sounds
  // like things are going to take just a little bit longer..."
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
  answer =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId, 0);
  if (answer == 0) {
    FieldCamera_SetSpeed(2.4);
    FieldCamera_MoveTo(184, CAMERA_KEEP, 512, 1);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
    Actor_SetSpeed(ACTOR_ISAAC, 1.6);
    Actor_WalkToAndWait(ACTOR_ISAAC, 168, 520);
    // "I'm sorry! We're almost done. Can you wait for us at the village..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
    // "I'm serious! If I have to wait much longer, you're not going to be..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
    gFieldState->message += 2;
  } else {
    gFieldState->message += 2;
    FieldCamera_SetSpeed(2.4);
    FieldCamera_MoveTo(184, CAMERA_KEEP, 512, 1);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
    Actor_SetSpeed(ACTOR_ISAAC, 1.6);
    Actor_WalkToAndWait(ACTOR_ISAAC, 168, 520);
    // "I'm sorry, Jenna! It sounds like things are going to take just a..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
    // "Well, I'm not going to wait much longer! If you don't come soon..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  }
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(158, CAMERA_KEEP, 432, 1);
  FieldCamera_WaitForArrival();
  FieldActor_Jump(ACTOR_CHILD_SENT_OUTSIDE, 4, 10);
  // "I'll wait outside with them."
  Event_ShowMessage(ACTOR_CHILD_SENT_OUTSIDE, 0);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(172, CAMERA_KEEP, 520, 1);
  FieldActor_WatchAndSetScript(ACTOR_ISAAC, ACTOR_CHILD_SENT_OUTSIDE);
  FieldActor_WatchAndSetScript(gMainRoutineState.leaderActorId,
                               ACTOR_CHILD_SENT_OUTSIDE);
  FieldActor_WatchAndSetScript(ACTOR_HOUSES_KRADEN, ACTOR_CHILD_SENT_OUTSIDE);
  Actor_SetSpeed(ACTOR_CHILD_SENT_OUTSIDE, 1.6);
  Actor_WalkToAndWait(ACTOR_CHILD_SENT_OUTSIDE, 140, 478);
  Actor_WalkToAndWait(ACTOR_CHILD_SENT_OUTSIDE, 150, 540);
  Actor_WalkToAndWait(ACTOR_CHILD_SENT_OUTSIDE, 160, 560);
  Actor_WalkToAndWait(ACTOR_CHILD_SENT_OUTSIDE, 190, 640);
  FieldActor_Stop(ACTOR_ISAAC);
  FieldActor_Stop(gMainRoutineState.leaderActorId);
  FieldActor_Stop(ACTOR_HOUSES_KRADEN);
  System_Wait(1);
  Actor_SetPosition(ACTOR_CHILD_SENT_OUTSIDE, 0, 0);
  FieldEvent_Wait(10);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 448, 1);
  Actor_WalkToAndWait(ACTOR_ISAAC, 168, 484);
  Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
  // "I don't think they want to wait anymore, Felix. We're all set to..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_HOUSES_KRADEN, FACING_WEST);
  // "They have no patience whatsoever. Ah, well... I suppose it is time..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HOUSES_KRADEN, 0);
  Actor_FaceDirection(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_HOUSES_PUELLE, 3);
  // "May your journey home be safe and uneventful..."
  Event_ShowMessage(ACTOR_HOUSES_PUELLE, 0);
  Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_HOUSES_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_NORTH);
  Actor_SetAnimationAndWait(ACTOR_HOUSES_ELDER, 3);
  // "We shall never forget what you have done for Prox."
  Event_ShowMessage(ACTOR_HOUSES_ELDER, 0);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_HOUSES_KRADEN, 3);
  Actor_SetSpeed(ACTOR_ISAAC, 1.6);
  Actor_SetSpeed(ACTOR_HOUSES_KRADEN, 1.6);
  Actor_SetAnimation(ACTOR_ISAAC, ANIM_WALK);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_ISAAC, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(ACTOR_ISAAC);
  Actor_SetPosition(ACTOR_ISAAC, 0, 0);
  Actor_SetAnimation(ACTOR_HOUSES_KRADEN, 2);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_HOUSES_KRADEN, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(ACTOR_HOUSES_KRADEN);
  Actor_SetPosition(ACTOR_HOUSES_KRADEN, 0, 0);
  {
    struct FieldState *state = gFieldState;

    state->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    state->transitionFrames = 16;
  }
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Event_SetFlag(FLAG_THANKED_FOR_BEACON);
  Prox_EndEvent();
}

void TilesetAnim_Resume(s32 layer);
void TilesetAnim_Pause(u32 layer);
void FieldEffect_StartBlankOverlay(s32 actor);
void Weather_StartRain(s32 withCue);

s32 Scene_Initialize(void) {
  s32 scene;

  if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
    Event_SetFlag(FLAG_CANNOT_SAVE_HERE);
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
  }
  scene = gMainRoutineState.next;
  if (scene == SCENE_PROX_HOUSES) {
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Prox_InitializeHouses();
  } else if (scene == SCENE_PROX_CAVE_HOMES) {
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Prox_InitializeCaveHomes();
  } else if (scene == SCENE_PROX_CAVES) {
    /* FAKEMATCH(dead): the leader's 0 derived from scene; as 0. */
    s32 object = (u32)(u16)scene >> 16;

    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    FieldEffect_StartBlankOverlay(object);
  } else {
    if (!Event_GetFlag(FLAG_MARS_BEACON_LIT))
      Event_SetFlag(FLAG_STORM_RAGING);
    if (Event_GetFlag(FLAG_STORM_RAGING)) {
      Weather_StartRain(1);
    } else {
      REG_BLDALPHA = BLEND_ALPHA(8, 12);
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
      Task_Add(Prox_SmokeFromChimneys, TASK_PASS_FRAME);
      TilesetAnim_Pause(0);
      TilesetAnim_Pause(1);
      TilesetAnim_Resume(2);
      TilesetAnim_Resume(3);
      TilesetAnim_Resume(4);
      TilesetAnim_Resume(5);
      TilesetAnim_Resume(6);
    }
    scene = gMainRoutineState.next;
    if (scene == SCENE_PROX) {
      if (!Event_GetFlag(FLAG_DIG_SPOT_SCOOPED))
        Actor_SetWideSprite(FIELD_TREASURE_ACTOR, TREASURE_SPOT_WIDE);
      Prox_InitializeTown();
    } else if (scene == SCENE_PROX_MOLD) {
      Prox_InitializeMold();
    }
    if (Event_GetFlag(FLAG_STORM_RAGING)) {
      PaletteFade_SetTarget(PALETTE_TINT_MUTED, PALETTES_BG);
      PaletteFade_StartInterpolation(16);
      wait(16);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Puts a child at its place and starts its game of chase: `quarry` picks the
/// child it runs after (Prox_PlayChase).
void Prox_StartPlayingChase(s32 actorId, s32 x, s32 z, s32 quarry) {
  struct Actor *child = Actor_Get(actorId);

  child->timer = quarry;
  child->sceneByte = 0;
  child->update = (ObjectUpdate)Prox_PlayChase;
  Actor_SetPosition(actorId, x, z);
}

void Prox_InitializeTown(void) {

  if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
      Prox_StartPlayingChase(ACTOR_PLAYING_CHILD, 536, 360, 0);
      Prox_StartPlayingChase(ACTOR_CHILD_WHO_MISSES_RUNNING, 536, 424, 1);
      Prox_StartPlayingChase(ACTOR_TRAINING_CHILD, 580, 420, 2);
      System_Wait(1);
      Actor_SetScript(ACTOR_PLAYING_CHILD, gProx_FirstChildRuns);
      Actor_SetScript(ACTOR_CHILD_WHO_MISSES_RUNNING, gProx_SecondChildRuns);
      Actor_SetScript(ACTOR_TRAINING_CHILD, gProx_ThirdChildRuns);
      System_Wait(1);
    }
  }
  FieldActor_SetSpritePriority(ACTOR_TOWN_CRATE, 1);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_TOWN_CRATE), SPRITE_COLOR_HIDDEN);
}

/// Sets up the upper village. While the lighthouse is unlit, the warrior from
/// the upper house is there only once Felix has overheard the two warriors,
/// the house's door stays open, and Puelle faces away once the clan has argued.
/// Mold stays hidden in his snow pile once he has run into it.
void Prox_InitializeMold(void) {
  struct Actor *actor;

  FieldActor_SetSpritePriority(ACTOR_MOLD_CRATE, 1);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_MOLD_CRATE), SPRITE_COLOR_HIDDEN);
  if (!Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
    if (!Event_GetFlag(FLAG_HEARD_OF_PARENTS)) {
      Actor_SetPosition(ACTOR_WARRIOR_SENT_FOR_PARENTS, 0, 0);
    }
    if (Event_GetFlag(FLAG_DOOR_LEFT_OPEN)) {
      MapGrid_CopyCellsTo(89, 0, 77, 16, 1, 2);
    }
    if (Event_GetFlag(FLAG_CLAN_SPOKE_OF_LIGHTHOUSE)) {
      actor = Actor_Get(ACTOR_PUELLE);
      actor->facing = FACING_NORTH_NORTHWEST;
      System_Wait(1);
    }
  }
  if (Event_GetFlag(FLAG_MARS_BEACON_LIT)) {
    MapGrid_CopyCellsTo(72, 3, 69, 3, 3, 3);
    MapGrid_CopyCellsTo(48, 36, 69, 8, 3, 2);
    MapGrid_CopyCellAttributes(72, 13, 1, 3, 70, 20);
    Actor_SetPosition(ACTOR_MOLD, 0, 0);
  } else {
    if (Event_GetFlag(FLAG_MOLD_HID_IN_SNOW)) {
      MapGrid_CopyCellsTo(72, 3, 69, 3, 3, 3);
      MapGrid_CopyCellsTo(48, 36, 69, 8, 3, 2);
      MapGrid_CopyCellAttributes(72, 13, 1, 3, 70, 20);
      if (!Event_GetFlag(FLAG_DJINNI(DJINNI_MOLD))) {
        Actor_SetPosition(ACTOR_MOLD, 0, 0);
      }
    }
  }
}

/// Each frame, draws the child hiding from the wind in the igloo in front of or
/// behind things as Felix is.
void Prox_MatchHousesPriority(void) {
  struct Actor *actor;

  actor = Actor_Get(gMainRoutineState.leaderActorId);
  FieldActor_SetSpritePriority(ACTOR_CHILD_HIDING_FROM_THE_WIND,
                               ((struct FieldSprite *)actor->sprite)->priority);
}

void Prox_InitializeHouses(void) {

  FieldEffect_StartBlankOverlay(0);
  Task_AddFrame(Prox_MatchHousesPriority, TASK_PASS_FRAME);
  if (gMainRoutineState.entrance == PROX_HOUSES_FROM_MARS_LIGHTHOUSE_AERIE) {
    if (!Event_GetFlag(FLAG_THANKED_FOR_BEACON)) {
      Prox_GiveThanksForBeacon();
    }
  }
}

/// Each frame, draws two of the basements' people in front of or behind things
/// as Felix is.
void Prox_MatchCaveHomesPriority(void) {
  s32 variant =
      ((struct FieldSprite *)Actor_Get(gMainRoutineState.leaderActorId)->sprite)
          ->priority;

  FieldActor_SetSpritePriority(ACTOR_VILLAGER_ON_PROXS_PURPOSE, variant);
  FieldActor_SetSpritePriority(ACTOR_SHOPKEEPER, variant);
}

void Prox_InitializeCaveHomes(void) {

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
    s32 entrance = (u16)gMainRoutineState.entrance;

    if (gMainRoutineState.entrance == PROX_CAVE_HOMES_FROM_PROX_HOUSES_NORTH)
      Prox_ComeDownStairs(0);
    else if ((u16)(entrance - PROX_CAVE_HOMES_FROM_PROX_HOUSES_WEST) <=
             PROX_CAVE_HOMES_FROM_PROX_HOUSES_SOUTH -
                 PROX_CAVE_HOMES_FROM_PROX_HOUSES_WEST)
      Prox_ComeDownStairs(1);
  }
  FieldEffect_StartBlankOverlay(0);
  Task_AddFrame(Prox_MatchCaveHomesPriority, TASK_PASS_FRAME);
}

s32 Actor_PlaceOnActor();
extern s32 gProx_IsaacStepsForward[];
extern s32 gProx_RightFileWalksSouth[];
extern s32 gProx_GaretWalksSouth[];
extern s32 gProx_IsaacWalksSouth[];
extern s32 gProx_GaretRunsAhead[];

/// After the thanks in the big north-west house, Felix walks south through the
/// south gate, cells (8, 9) and (9, 9), where everyone is waiting. Jenna asks
/// what took him so long, the families and both parties say their farewells,
/// Felix, Isaac and Kraden wave goodbye, and the picture dims and fades to
/// black for the credits.
void Prox_SayFarewell(void) {
  u32 i;

  if (Event_GetFlag(FLAG_THANKED_FOR_BEACON) != 0) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    FieldActor_Jump(ACTOR_JENNA, 2, 10);
    FieldActor_Jump(ACTOR_JENNA, 4, 10);
    FieldEvent_SetMessageId((s32)MsgProxWhatTookYouSoLong);
    // "What took you so long? Mom and Dad are tired of waiting for you..."
    Event_ShowMessage(ACTOR_JENNA, 0);
    FieldCamera_SetSpeed(3.2);
    FieldCamera_MoveTo(144, CAMERA_KEEP, 228, 1);
    FieldCamera_WaitForArrival();
    FieldActor_ShowEmote(ACTOR_JENNAS_MOTHER, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    // "That's not true, Felix. You take as long as you need to say farewell..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNAS_MOTHER, 0);
    Event_PlaySound(MUS_SHAMAN_VILLAGE);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 136, 196);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
    Actor_PlaceOnActor(ACTOR_ISAAC, gMainRoutineState.leaderActorId);
    Actor_PlaceOnActor(ACTOR_SOUTH_GATE_KRADEN,
                       gMainRoutineState.leaderActorId);
    System_Wait(1);
    Actor_SetSpeed(ACTOR_ISAAC, 0.8);
    Actor_SetSpeed(ACTOR_SOUTH_GATE_KRADEN, 0.8);
    Actor_SetScript(ACTOR_ISAAC, gProx_IsaacStepsForward);
    Actor_RunScriptAndWait(ACTOR_SOUTH_GATE_KRADEN, gProx_KradenStepsForward);
    Actor_SetAnimationAndWait(ACTOR_JENNAS_FATHER, 3);
    // "Don't worry about us, Felix... Puelle and the others took good care..."
    Event_ShowMessage(ACTOR_JENNAS_FATHER, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHWEST);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
    // "Don't tell him that! We'll be stuck here forever!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
    FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    // "You look like you're feeling back to your old self, Jenna!"
    Event_ShowMessage(ACTOR_SHEBA, 0);
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
    Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
    // "You were weeping such mournful tears after the battle..."
    Event_ShowMessage(ACTOR_PIERS, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    // "What!? As if! You can't prove anything!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
    // "Go easy on her. You have to remember, she thought that both of her..."
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_ShowEmote(ACTOR_JENNAS_MOTHER, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNAS_MOTHER, FACING_NORTH_NORTHEAST);
    // "Oh, were you crying, Jenna?"
    Event_ShowMessage(ACTOR_JENNAS_MOTHER, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
    FieldActor_Jump(ACTOR_JENNA, 4, 10);
    // "No! I said... I said I wasn't!"
    Event_ShowMessage(ACTOR_JENNA, 0);
    FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_NORTHWEST);
    // "Now you've got me thinking about my family... I didn't think I'd..."
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    Actor_FaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
    // "I only hope that they're all safe back in Vale..."
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
    // "I just want to be home again... I want to see how my mother is doing..."
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0, 10);
    Actor_FaceDirection(ACTOR_KYLE, FACING_SOUTH_SOUTHEAST, 20);
    // "Don't worry, Son. I'm sure Dora's doing fine. She's a strong woman."
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_KYLE, 0, 20);
    if (Event_GetFlag(FLAG_TRANSFERRED_VISITED_SICK_DORA) != 0) {
      Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
      Actor_SetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
      // "But last time I saw her, she looked so sick, so weak... Even if..."
      Event_ShowMessage(ACTOR_ISAAC, 0);
      Actor_FaceDirectionAndWait(ACTOR_KYLE, FACING_NORTH_NORTHEAST);
      // "Don't worry, Isaac. I'm sure she's in wonderful shape. I think..."
      Event_ShowMessage(ACTOR_KYLE, 0);
      gFieldState->message++;
    } else {
      gFieldState->message += 2;
      Actor_FaceDirectionAndWait(ACTOR_KYLE, FACING_NORTH_NORTHEAST);
      // "And she wouldn't let anything happen until you came home."
      Event_ShowMessage(ACTOR_KYLE, 0);
    }
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
    Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
    // "Sorry to keep you all waiting. At last, the time has come for us..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SOUTH_GATE_KRADEN, 0);
    Actor_FaceDirection(ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
    Actor_FaceDirection(ACTOR_SOUTH_GATE_KRADEN, FACING_WEST, 20);
    Actor_FaceDirectionAndWait(ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHWEST);
    // "As soon as we leave Prox, we'll head toward Angara, and from..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SOUTH_GATE_KRADEN, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNAS_MOTHER, FACING_WEST);
    // "I can't wait to see this winged ship of yours! It sounds so incredible!"
    Event_ShowMessage(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_JENNAS_MOTHER, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNAS_FATHER, FACING_EAST);
    Actor_SetAnimation(ACTOR_JENNAS_FATHER, 3);
    // "I remember so little of our trip to Prox... This is really my..."
    Event_ShowMessage(
        SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_JENNAS_FATHER, 0);
    Actor_FaceDirectionAndWait(ACTOR_KYLE, FACING_EAST);
    // "I've heard that the wind and waves make the boat rock... I hope I..."
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_KYLE,
                      0);
    Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
    // "This ancient ship of ours actually flies above the ocean... It's..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SOUTH_GATE_KRADEN, 0);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    // "It looks like we've got an exciting last trip ahead of us, doesn't it?"
    // Yes: "Wow... Setting out like this takes me back to our own..."
    // No: "I'm still worried about my mother, but I'll try to put it..."
    Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                              0) == 0) {
      Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
      // "Wow... Setting out like this takes me back to our own quest's..."
      Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
      gFieldState->message++;
    } else {
      gFieldState->message++;
      Actor_SetAnimationAndWait(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
      // "I'm still worried about my mother, but I'll try to put it aside..."
      Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
    }
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHWEST);
    FieldActor_Jump(ACTOR_JENNA, 2, 10);
    FieldActor_Jump(ACTOR_JENNA, 4, 10);
    // "If you keep talking like this, we'll never go anywhere! Let's go..."
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_NOD);
    Actor_SetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
    Actor_SetAnimation(ACTOR_KYLE, 3);
    Actor_SetAnimation(ACTOR_JENNAS_FATHER, 3);
    Actor_SetAnimation(ACTOR_JENNAS_MOTHER, 3);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Actor_SetSpeed(ACTOR_ISAAC, 0.8);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
    Actor_SetSpeed(ACTOR_SOUTH_GATE_KRADEN, 0.8);
    Actor_SetSpeed(ACTOR_JENNA, 0.8);
    Actor_SetSpeed(ACTOR_SHEBA, 0.8);
    Actor_SetSpeed(ACTOR_PIERS, 0.8);
    Actor_SetSpeed(ACTOR_KYLE, 0.8);
    Actor_SetSpeed(ACTOR_JENNAS_FATHER, 0.8);
    Actor_SetSpeed(ACTOR_JENNAS_MOTHER, 0.8);
    Actor_SetSpeed(ACTOR_MIA, 0.8);
    Actor_SetSpeed(ACTOR_IVAN, 0.8);
    Actor_SetSpeed(ACTOR_GARET, 0.8);
    FieldActor_SetSpritePriority(ACTOR_ISAAC, 2);
    FieldActor_SetSpritePriority(gMainRoutineState.leaderActorId, 2);
    FieldActor_SetSpritePriority(ACTOR_SOUTH_GATE_KRADEN, 2);
    FieldActor_SetSpritePriority(ACTOR_JENNA, 2);
    FieldActor_SetSpritePriority(ACTOR_SHEBA, 2);
    FieldActor_SetSpritePriority(ACTOR_PIERS, 2);
    FieldActor_SetSpritePriority(ACTOR_KYLE, 2);
    FieldActor_SetSpritePriority(ACTOR_JENNAS_FATHER, 2);
    FieldActor_SetSpritePriority(ACTOR_JENNAS_MOTHER, 2);
    FieldActor_SetSpritePriority(ACTOR_MIA, 2);
    FieldActor_SetSpritePriority(ACTOR_IVAN, 2);
    FieldActor_SetSpritePriority(ACTOR_GARET, 2);
    FieldCamera_SetSpeed(0.4);
    FieldCamera_MoveTo(144, CAMERA_KEEP, 246, 1);
    Actor_SetScript(ACTOR_JENNAS_FATHER, gProx_JennasFatherWalksSouth);
    Actor_SetScript(ACTOR_JENNAS_MOTHER, gProx_JennasMotherWalksSouth);
    FieldEvent_Wait(10);
    Actor_SetScript(ACTOR_KYLE, gProx_LeftFileWalksSouth);
    Actor_SetScript(ACTOR_MIA, gProx_RightFileWalksSouth);
    FieldEvent_Wait(10);
    Actor_SetScript(ACTOR_SHEBA, gProx_LeftFileWalksSouth);
    Actor_SetScript(ACTOR_PIERS, gProx_LeftFileWalksSouth);
    FieldEvent_Wait(10);
    Actor_SetScript(ACTOR_IVAN, gProx_RightFileWalksSouth);
    Actor_SetScript(ACTOR_GARET, gProx_GaretWalksSouth);
    FieldEvent_Wait(30);
    Actor_SetScript(ACTOR_JENNA, gProx_JennaWalksSouth);
    FieldEvent_Wait(20);
    Actor_SetScript(ACTOR_ISAAC, gProx_IsaacWalksSouth);
    Actor_SetScript(gMainRoutineState.leaderActorId, gProx_LeaderWalksSouth);
    FieldEvent_Wait(10);
    Actor_RunScriptAndWait(ACTOR_SOUTH_GATE_KRADEN, gProx_KradenWalksSouth);
    Actor_ShowEmote(ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    20);
    Actor_FaceDirection(ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST, 0);
    FieldActor_WaitForScriptEndById(ACTOR_JENNA);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 40);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    Actor_ShowEmote(ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    // "What's the matter, boys? Are you reluctant to depart?"
    // Yes: "It's hard to believe that our quest is almost at its end. I..."
    // No: "After all that we've been through, you can't possibly be..."
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                              0) == 0) {
      Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
      // "It's hard to believe that our quest is almost at its end. I know..."
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                            ACTOR_SOUTH_GATE_KRADEN,
                        0);
      gFieldState->message++;
    } else {
      gFieldState->message++;
      Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 4);
      // "After all that we've been through, you can't possibly be nervous..."
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                            ACTOR_SOUTH_GATE_KRADEN,
                        0);
    }
    Actor_FaceDirection(ACTOR_ISAAC, FACING_NORTH, 20);
    // "I just hope that Vale came out of this in as good a condition as..."
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_SetAnimation(ACTOR_SOUTH_GATE_KRADEN, 4);
    // "We won't know until we get there."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    FieldActor_Jump(ACTOR_GARET, 2, 10);
    // "I can't stop thinking about how my parents are doing..."
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
    Actor_FaceDirection(ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
    // "Were the both of them in Vale?"
    Event_ShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    // "Can't you make a guess, Kraden? Will Vale still be standing when..."
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 4);
    // "Not even I can know that..."
    Event_ShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
    FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    // "I can see it in your eyes, Kraden! You think something's happened..."
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Actor_SetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
    // "It's a possibility... One that cannot be ignored..."
    Event_ShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 40);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
    // "If it's not there, then what's the point in even going back?"
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_FaceDirectionAndWait(ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
    Actor_ShowEmote(ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                    0);
    // "Is that really how you feel, Isaac? What about you, Felix?"
    // Yes: "Have you already forgotten all that we discussed? You ought..."
    // No: "You remember, don't you, Felix? Think about it for a moment..."
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                              0) == 0) {
      Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 4);
      // "Have you already forgotten all that we discussed? You ought to..."
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                            ACTOR_SOUTH_GATE_KRADEN,
                        0);
      gFieldState->message++;
    } else {
      gFieldState->message++;
      Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
      // "You remember, don't you, Felix? Think about it for a moment, Isaac."
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                            ACTOR_SOUTH_GATE_KRADEN,
                        0);
    }
    FieldActor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    // "What do you mean?"
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_FaceDirectionAndWait(ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST);
    // "Our conversation with Hamma after we ignited the beacon..."
    Event_ShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    // "Don't you remember us talking to the children from Imil at the..."
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
    // "When they were warning people away from the lighthouses?"
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_FaceDirectionAndWait(ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
    Actor_SetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
    // "Yes. The Wise One instructed everyone to seek refuge."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_NOD);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    FieldEvent_Wait(20);
    // "Yeah, so what about it?"
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_ShowEmote(ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                    0);
    Actor_FaceDirectionAndWait(ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST);
    // "You still don't see, Garet?"
    Event_ShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Actor_SetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
    // "Even if Vale were destroyed, I'd expect that the villagers have..."
    Event_ShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    // "I guess that's true... They might still be all right."
    Event_ShowMessage(ACTOR_JENNA, 0);
    FieldActor_Jump(ACTOR_GARET, 2, 10);
    // "That's true... So whatever happens, my family's alive!"
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
    // "I'd forgotten about that... Thanks, Kraden."
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    // "Garet, can you make sure everyone else knows that?"
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_FaceDirectionAndWait(ACTOR_GARET, FACING_EAST);
    Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    FieldEvent_Wait(10);
    Actor_SetSpeed(ACTOR_JENNA, 1.6);
    Actor_SetSpeed(ACTOR_GARET, 1.6);
    Actor_SetScript(ACTOR_JENNA, gProx_JennaRunsAhead);
    Actor_RunScriptAndWait(ACTOR_GARET, gProx_GaretRunsAhead);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
    Actor_SetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
    // "OK, let's go. The both of you! We have to catch up to everyone."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_WalkToAndWait(ACTOR_SOUTH_GATE_KRADEN, 156, 256);
    Actor_ShowEmote(ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
    // "Kraden..."
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
    // "Why did the Wise One change our parents into a dragon? Why did he..."
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_FaceDirectionAndWait(ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
    // "I mean, we almost killed them... He tried to make us kill our own..."
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_ShowEmote(ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                    40);
    // "Do you think that he intended for them to die from the start?"
    // Yes: "I cannot speak for him, but I think he knew that they would..."
    // No: "Ah... You don't understand why he put you through all this..."
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                              0) == 0) {
      Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
      // "I cannot speak for him, but I think he knew that they would be..."
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                            ACTOR_SOUTH_GATE_KRADEN,
                        0);
      gFieldState->message++;
    } else {
      gFieldState->message++;
      Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 4);
      // "Ah... You don't understand why he put you through all this if he..."
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                            ACTOR_SOUTH_GATE_KRADEN,
                        0);
    }
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
    Actor_SetAnimation(ACTOR_SOUTH_GATE_KRADEN, 4);
    // "We cannot hope to fathom the motives of a being as all-powerful as..."
    FieldEvent_ShowMessageThenWait(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_SOUTH_GATE_KRADEN,
        0, 20);
    // "You don't know either, Kraden?"
    Event_ShowMessage(ACTOR_ISAAC, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
    FieldEvent_Wait(20);
    // "I can only hazard a guess... The Wise One... wanted to test you."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 40);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
    // "What do you mean, test us?"
    Event_ShowMessage(ACTOR_ISAAC, 0);
    Actor_SetAnimation(ACTOR_SOUTH_GATE_KRADEN, 4);
    // "I cannot tell you more... It is up to you to find the answer."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    FieldActor_ShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(gMainRoutineState.leaderActorId,
                    EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "Will we use Alchemy to wage war, to raise armies? Or will we use..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    Actor_SetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
    // "You were willing to sacrifice everything for your quest. I'd say..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    FieldEvent_Wait(20);
    Actor_ShowEmote(ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
    Actor_FaceDirection(ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
    FieldActor_Jump(ACTOR_SOUTH_GATE_KRADEN, 4, 10);
    // "Oops! Is everyone else that far ahead of us?"
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_FaceDirectionAndWait(ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
    // "We'd better hurry and catch up to the others before they leave us..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_FaceDirectionAndWait(ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST);
    FieldActor_Jump(ACTOR_SOUTH_GATE_KRADEN, 2, 10);
    FieldActor_Jump(ACTOR_SOUTH_GATE_KRADEN, 4, 10);
    // "After all, I'm not terribly interested in trying to walk all the..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
    Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_WEST);
    Actor_FaceDirectionAndWait(ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
    // "If you feel the same, then we'd better be hurrying."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_FaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
    Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_SOUTH);
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_NOD);
    Actor_SetAnimationAndWait(gMainRoutineState.leaderActorId, ANIM_NOD);
    Actor_ShowEmote(ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirectionAndWait(ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST);
    // "Isaac! Felix! We're off!"
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
                          ACTOR_SOUTH_GATE_KRADEN,
                      0);
    Actor_SetAnimation(ACTOR_SOUTH_GATE_KRADEN, 9);
    Actor_SetAnimation(gMainRoutineState.leaderActorId, 33);
    Actor_SetAnimation(ACTOR_ISAAC, 40);
    TilesetAnim_Pause(2);
    TilesetAnim_Pause(3);
    TilesetAnim_Pause(4);
    TilesetAnim_Pause(5);
    TilesetAnim_Pause(6);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    FieldEvent_Wait(40);
    PaletteFade_ApplyTarget(PALETTE_TINT_DIM_GRAY, PALETTES_ALL);
    PaletteFade_Interpolate(120);
    System_Wait(120);
    // A pause the player can cut short by holding a key.
    for (i = 0; i < 120; i++) {
      System_Wait(1);
      if (gInput.held != 0)
        break;
    }
    PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
    PaletteFade_Interpolate(16);
    System_Wait(16);
    Event_SetFlag(FLAG_SAID_FAREWELL);
    FieldEvent_SetExitRequest(PROX_SOUTH_GATE_TO_STAFF_CREDIT);
  }
}

extern s16 gSlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"
/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  FIXED x;
  FIXED y;
  FIXED z;
};

extern s16 gSlideOnIce_AnglesByKey[];

#include "shared/leader_slide_on_ice.h"

volatile s16 gSlideOnIce_Stopped BSS_DATA;

s32 Actor_HasArrived(struct Actor *actor);

#include "shared/slide_on_ice_from_edge.c"

/// Stops the leader's slide on the ice where it is (nonzero), or lets it run.
void Leader_SetSlideStopped(s32 stopped) { gSlideOnIce_Stopped = stopped; }

#include "shared/leader_slide_on_ice_ahead.h"

void EffectSprite_DrawAtWorldPosition(struct ParticleSprite *sprite,
                                      struct Particle *particle);
void BattleEffect_StepVector3(struct Particle *particle, s32 damping,
                              s32 gravity);

#include "shared/particle_update_falling.h"

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
#include "shared/rock_spawn.h"
/// The overlay's call to EffectSprite_Init: one rock's sprite, its size,
/// pivot and attributes.
void Prox_InitRockSprite(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                         u32 attributes, s32 tile);

/// Unused: loads the falling rocks' tiles and 64 sprites and starts their
/// task, as Magma Rock does.
UNUSED void Prox_StartFallingRocks(void) {
  struct RockState *state =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockState));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct EffectSprite *sprite;

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FALLING_ROCK), state);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, sizeof(state->tiles), state);
  state->tileBase = tile;
  state->resource = entry;
  rock = state->rocks;
  for (i = 0; i < (s32)ARRAY_COUNT(state->rocks); i++) {
    sprite = &state->sprites[i];
    Prox_InitRockSprite(sprite, 8, 8, OBJ_SIZE_16X16, tile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    rock->timer = -1;
    rock++;
  }
  state->next = 0;
  FieldTask_AddFrame(Particle_UpdateFalling, TASK_PASS_FRAME);
}

#include "shared/rock_end_falling.h"

void Prox_MoldJoins(void);
void Prox_AskAboutIgloos(void);
void Prox_TalkToInnkeeper(void);
void Prox_TalkToChildWhoMissesRunning(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/leader_walk_through_door_script.c"

#include "shared/slide_on_ice_angles.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gProx_FirstChildRuns[] = {
    SC_ANIMATION(2),
    SC_SPEED(1),
    SC_ACCELERATION(0.4),
    SC_ANIMATION(2),
    SC_MOVE(560, 0, 380),
    SC_SYNC,
    SC_ANIMATION(2),
    SC_MOVE(564, 0, 424),
    SC_SYNC,
    SC_ANIMATION(2),
    SC_MOVE(536, 0, 424),
    SC_SYNC,
    SC_ANIMATION(2),
    SC_MOVE(536, 0, 380),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gProx_SecondChildRuns[] = {
    SC_ANIMATION(2),
    SC_SPEED(1),
    SC_ACCELERATION(0.4),
    SC_ANIMATION(2),
    SC_MOVE(536, 0, 380),
    SC_SYNC,
    SC_ANIMATION(2),
    SC_MOVE(560, 0, 380),
    SC_SYNC,
    SC_ANIMATION(2),
    SC_MOVE(564, 0, 424),
    SC_SYNC,
    SC_ANIMATION(2),
    SC_MOVE(536, 0, 424),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gProx_ThirdChildRuns[] = {
    SC_ANIMATION(2),
    SC_SPEED(1),
    SC_ACCELERATION(0.4),
    SC_ANIMATION(2),
    SC_MOVE(536, 0, 424),
    SC_SYNC,
    SC_ANIMATION(2),
    SC_MOVE(536, 0, 380),
    SC_SYNC,
    SC_ANIMATION(2),
    SC_MOVE(560, 0, 380),
    SC_SYNC,
    SC_ANIMATION(2),
    SC_MOVE(564, 0, 424),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gProx_KradenStepsUpToPuelle[] = {
    SC_ANIMATION(2),
    SC_MOVE(176, 0, 154),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

s32 gProx_IsaacStepsUpToPuelle[] = {
    SC_ANIMATION(2),
    SC_MOVE(160, 0, 154),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_END,
};

s32 gProx_StepBackFromPuelle[] = {
    SC_ANIMATION(2),
    SC_MOVE(144, 0, 154),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(ProxSouthGate),
    SCENE_EXIT(PROX_SOUTH_GATE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_PROX_SOUTH_GATE),
    SCENE_EXIT(PROX_SOUTH_GATE_TO_PROX, Prox, PROX_FROM_PROX_SOUTH_GATE),
    SCENE_EXIT(PROX_SOUTH_GATE_TO_STAFF_CREDIT, StaffCredit, STAFF_CREDIT_FROM_PROX_SOUTH_GATE),
    SCENE_EXITS(Prox),
    SCENE_EXIT(PROX_TO_PROX_SOUTH_GATE, ProxSouthGate, PROX_SOUTH_GATE_FROM_PROX),
    SCENE_EXIT(PROX_TO_PROX_HOUSES_SOUTHWEST, ProxHouses, PROX_HOUSES_FROM_PROX_SOUTHWEST),
    SCENE_EXIT(PROX_TO_PROX_HOUSES_SOUTH, ProxHouses, PROX_HOUSES_FROM_PROX_SOUTH),
    SCENE_EXIT(PROX_TO_PROX_HOUSES_EAST_A, ProxHouses, PROX_HOUSES_FROM_PROX_EAST_A),
    SCENE_EXIT(PROX_TO_PROX_HOUSES_WEST, ProxHouses, PROX_HOUSES_FROM_PROX_WEST),
    SCENE_EXIT(PROX_TO_PROX_HOUSES_NORTHWEST, ProxHouses, PROX_HOUSES_FROM_PROX_NORTHWEST),
    SCENE_EXIT(PROX_TO_PROX_HOUSES_EAST_B, ProxHouses, PROX_HOUSES_FROM_PROX_EAST_B),
    SCENE_EXIT(PROX_TO_PROX_CAVES, ProxCaves, PROX_CAVES_FROM_PROX),
    SCENE_EXIT(PROX_TO_PROX_MOLD_EAST, ProxMold, PROX_MOLD_FROM_PROX_EAST),
    SCENE_EXIT(PROX_TO_PROX_CAVE_HOMES, ProxCaveHomes, PROX_CAVE_HOMES_FROM_PROX),
    SCENE_EXIT(PROX_TO_PROX_MOLD_WEST, ProxMold, PROX_MOLD_FROM_PROX_WEST),
    SCENE_EXITS(ProxMold),
    SCENE_EXIT(PROX_MOLD_TO_PROX_WEST, Prox, PROX_FROM_PROX_MOLD_WEST),
    SCENE_EXIT(PROX_MOLD_TO_PROX_EAST, Prox, PROX_FROM_PROX_MOLD_EAST),
    SCENE_EXIT(PROX_MOLD_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_PROX_MOLD),
    SCENE_EXIT(PROX_MOLD_TO_PROX_HOUSES, ProxHouses, PROX_HOUSES_FROM_PROX_MOLD),
    SCENE_EXITS(ProxHouses),
    SCENE_EXIT(PROX_HOUSES_TO_PROX_SOUTH, Prox, PROX_FROM_PROX_HOUSES_SOUTH),
    SCENE_EXIT(PROX_HOUSES_TO_PROX_EAST_B, Prox, PROX_FROM_PROX_HOUSES_EAST_B),
    SCENE_EXIT(PROX_HOUSES_TO_PROX_CAVE_HOMES_NORTH, ProxCaveHomes, PROX_CAVE_HOMES_FROM_PROX_HOUSES_NORTH),
    SCENE_EXIT(PROX_HOUSES_TO_PROX_WEST, Prox, PROX_FROM_PROX_HOUSES_WEST),
    SCENE_EXIT(PROX_HOUSES_TO_PROX_NORTHWEST, Prox, PROX_FROM_PROX_HOUSES_NORTHWEST),
    SCENE_EXIT(PROX_HOUSES_TO_PROX_CAVE_HOMES_WEST, ProxCaveHomes, PROX_CAVE_HOMES_FROM_PROX_HOUSES_WEST),
    SCENE_EXIT(PROX_HOUSES_TO_PROX_MOLD, ProxMold, PROX_MOLD_FROM_PROX_HOUSES),
    SCENE_EXIT(PROX_HOUSES_TO_PROX_SOUTHWEST, Prox, PROX_FROM_PROX_HOUSES_SOUTHWEST),
    SCENE_EXIT(PROX_HOUSES_TO_PROX_CAVE_HOMES_SOUTH, ProxCaveHomes, PROX_CAVE_HOMES_FROM_PROX_HOUSES_SOUTH),
    SCENE_EXIT(PROX_HOUSES_TO_PROX_EAST_A, Prox, PROX_FROM_PROX_HOUSES_EAST_A),
    SCENE_EXITS(ProxCaveHomes),
    SCENE_EXIT(PROX_CAVE_HOMES_TO_PROX_HOUSES_NORTH, ProxHouses, PROX_HOUSES_FROM_PROX_CAVE_HOMES_NORTH),
    SCENE_EXIT(PROX_CAVE_HOMES_TO_PROX_HOUSES_WEST, ProxHouses, PROX_HOUSES_FROM_PROX_CAVE_HOMES_WEST),
    SCENE_EXIT(PROX_CAVE_HOMES_TO_PROX_HOUSES_SOUTH, ProxHouses, PROX_HOUSES_FROM_PROX_CAVE_HOMES_SOUTH),
    SCENE_EXIT(PROX_CAVE_HOMES_TO_PROX, Prox, PROX_FROM_PROX_CAVE_HOMES),
    SCENE_EXITS(ProxCaves),
    SCENE_EXIT(PROX_CAVES_TO_PROX, Prox, PROX_FROM_PROX_CAVES),
    SCENE_EXITS_END,
};

struct ActorPlacement gProx_SouthGatePlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gProx_SouthGateFarewellPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KYLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 248, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNAS_FATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 248, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNAS_MOTHER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 248, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gProx_TownPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_GREEN_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 456, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 336, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 112, 0, 248, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 452, 0, 300, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_YELLOW_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 276, 0, 148, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 580, 0, 348, FACING_WEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 240, 0, 244, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_GIRL_WITH_PINK_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 374, 0, 420, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_GIRL_WITH_PINK_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 412, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 568, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gProx_MoldPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_GREEN_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 540, 0, 388, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 388, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PUELLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 180, 0, 132, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 140, 0, 132, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 178, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 178, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 436, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_MOLD), MOVEMENT_TYPE_STAND, 404, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gProx_MoldPlacementsOnceLit[] = {
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_GREEN_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUELLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 120, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 120, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 176, 0, 162, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 216, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 146, 0, 120, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 160, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 436, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_MOLD), MOVEMENT_TYPE_STAND, 404, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gProx_HousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_GREEN_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 192, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 106, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 112, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 364, 0, 123, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 468, 0, 160, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_YELLOW_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 728, 0, 200, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 686, 0, 111, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 548, 0, 816, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_GIRL_WITH_PINK_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 466, 0, 810, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 480, 0, 800, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 530, 0, 800, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 164, 0, 524, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_YELLOW_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 222, 0, 490, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 394, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 268, 0, 794, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 816, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_WATER19), MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gProx_HousesPlacementsOnceLit[] = {
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_GREEN_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 104, 0, 196, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 106, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 112, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 364, 0, 123, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 468, 0, 160, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_YELLOW_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 728, 0, 200, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 666, 0, 134, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_GIRL_WITH_PINK_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 530, 0, 800, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 440, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_YELLOW_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 224, 0, 554, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 268, 0, 794, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 816, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_WATER19), MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUELLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 428, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 428, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gProx_CaveHomesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 628, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 808, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 370, 0, 740, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gProx_CaveHomesPlacementsOnceLit[] = {
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 666, 0, 124, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 808, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 370, 0, 740, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 456, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gProx_CavesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 777, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gProx_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_SouthGateTriggers[] = {
    {TRIGGER_EXIT, PROX_SOUTH_GATE_TO_WORLD_MAP, CONDITION_ALWAYS, PROX_SOUTH_GATE_TO_WORLD_MAP},
    {TRIGGER_EXIT, PROX_SOUTH_GATE_TO_PROX, CONDITION_ALWAYS, PROX_SOUTH_GATE_TO_PROX},
    {TRIGGER_STEP, CELL_BEHAVIOR_FAREWELL, FLAG_SAID_FAREWELL, EVENT(Prox_SayFarewell)},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_TownTriggers[] = {
    {TRIGGER_EXIT, PROX_TO_PROX_SOUTH_GATE, CONDITION_ALWAYS, PROX_TO_PROX_SOUTH_GATE},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_HOUSES_SOUTHWEST, CONDITION_ALWAYS, EVENT(Prox_EnterInn)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_HOUSES_SOUTH, CONDITION_ALWAYS, EVENT(Prox_EnterSouthHouse)},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, PROX_TO_PROX_HOUSES_EAST_A, CONDITION_ALWAYS, PROX_TO_PROX_HOUSES_EAST_A},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_HOUSES_WEST, CONDITION_ALWAYS, EVENT(Prox_EnterWestHouse)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_HOUSES_NORTHWEST, CONDITION_ALWAYS, EVENT(Prox_EnterNorthwestHouse)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_HOUSES_EAST_B, CONDITION_ALWAYS, EVENT(Prox_EnterEastHouse)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_CAVES, CONDITION_ALWAYS, EVENT(Prox_EnterSanctum)},
    {TRIGGER_EXIT, PROX_TO_PROX_MOLD_EAST, CONDITION_ALWAYS, PROX_TO_PROX_MOLD_EAST},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_CAVE_HOMES, CONDITION_ALWAYS, EVENT(Prox_EnterShopCave)},
    {TRIGGER_EXIT, PROX_TO_PROX_MOLD_WEST, CONDITION_ALWAYS, PROX_TO_PROX_MOLD_WEST},
    {TRIGGER_TALK, ACTOR_VILLAGER_WELCOMING_FELIX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxFelixWelcomeBackToProx)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WELCOMING_FELIX, CONDITION_ALWAYS, MSGNO(MsgProxThankYouForLightingFires)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WELCOMING_FELIX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxAgatioAndKarstToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WELCOMING_FELIX, CONDITION_ALWAYS, MSGNO(MsgProxFelixHasProvenHimself)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_AGATIO_AND_KARST, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxAgatioAndKarstLeft)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_AGATIO_AND_KARST, CONDITION_ALWAYS, MSGNO(MsgProxItsTooBadAboutAgatio)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_AGATIO_AND_KARST, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxAgatioAndKarstAreStrongest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_AGATIO_AND_KARST, CONDITION_ALWAYS, MSGNO(MsgProxIHeardKarstAndAgatio)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_ICE, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxImGladYouMade)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_ICE, CONDITION_ALWAYS, MSGNO(MsgProxIDontRememberLastTime)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_ICE, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxEverSinceAgatioReturned)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_ICE, CONDITION_ALWAYS, MSGNO(MsgProxBlizzardsFinallyOver)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_COLD, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxWereUsedToColdUp)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_COLD, CONDITION_ALWAYS, MSGNO(MsgProxWinterInProxCan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_COLD, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxItGetsColdAroundHere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_COLD, CONDITION_ALWAYS, MSGNO(MsgProxNowThatOurWeather)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WATCHING_THE_SKIES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIveBeenKeepingCloseEye)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WATCHING_THE_SKIES, CONDITION_ALWAYS, MSGNO(MsgProxIWasSoMovedWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WATCHING_THE_SKIES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxEverythingIsSupposedToReturn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WATCHING_THE_SKIES, CONDITION_ALWAYS, MSGNO(MsgProxIWasFirstPerson)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ASKING_FOR_SATUROS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxHaveYouSeenSaturos)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ASKING_FOR_SATUROS, CONDITION_ALWAYS, MSGNO(MsgProxLighthousesHaveAllBeenLit)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_FOR_SATUROS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxICantBelieveAnyoneCould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_FOR_SATUROS, CONDITION_ALWAYS, MSGNO(MsgProxTheyHadNoChoiceThey)},
    {TRIGGER_TALK, ACTOR_VILLAGER_SENSING_FELIXS_POWER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxFelixYoureBack)},
    {TRIGGER_TALK, ACTOR_VILLAGER_SENSING_FELIXS_POWER, CONDITION_ALWAYS, MSGNO(MsgProxYouDidAmazingJob)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SENSING_FELIXS_POWER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfFelixCouldBecomeAdept)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SENSING_FELIXS_POWER, CONDITION_ALWAYS, MSGNO(MsgProxWhenFelixLeftTownHe)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_WINDS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxOnceFinalLighthouseIsLit)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_WINDS, CONDITION_ALWAYS, MSGNO(MsgProxWillYouBeGoingBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_WINDS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfThisWeatherContinuesMuch)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_WINDS, CONDITION_ALWAYS, MSGNO(MsgProxIBetFelixAndHis)},
    {TRIGGER_TALK, ACTOR_PLAYING_CHILD, CONDITION_ALWAYS, EVENT(Prox_TalkToPlayingChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PLAYING_CHILD, CONDITION_ALWAYS, EVENT(Prox_TalkToPlayingChild)},
    {TRIGGER_TALK, ACTOR_CHILD_WHO_MISSES_RUNNING, CONDITION_ALWAYS, EVENT(Prox_TalkToChildWhoMissesRunning)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_WHO_MISSES_RUNNING, CONDITION_ALWAYS, EVENT(Prox_TalkToChildWhoMissesRunning)},
    {TRIGGER_TALK, ACTOR_TRAINING_CHILD, CONDITION_ALWAYS, EVENT(Prox_TalkToTrainingChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAINING_CHILD, CONDITION_ALWAYS, EVENT(Prox_TalkToTrainingChild)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_DARK_MATTER, FLAG_DIG_SPOT_SCOOPED, EVENT(Prox_ScoopUpDarkMatter)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_MoldTriggers[] = {
    {TRIGGER_EXIT, PROX_MOLD_TO_PROX_WEST, CONDITION_ALWAYS, PROX_MOLD_TO_PROX_WEST},
    {TRIGGER_EXIT, PROX_MOLD_TO_PROX_EAST, CONDITION_ALWAYS, PROX_MOLD_TO_PROX_EAST},
    {TRIGGER_EXIT, PROX_MOLD_TO_WORLD_MAP, CONDITION_ALWAYS, PROX_MOLD_TO_WORLD_MAP},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), PROX_MOLD_TO_PROX_HOUSES, CONDITION_ALWAYS, EVENT(Prox_EnterUpperHouse)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BESIDE_MOLD, FLAG_MOLD_HID_IN_SNOW, EVENT(Prox_MoldHidesInSnow)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ICE, CONDITION_ALWAYS, EVENT(Leader_SlideOnIce)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_BEFORE_CAST), CELL_BEHAVIOR_WALL, CONDITION_ALWAYS, EVENT(Prox_StartScoop)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_AFTER_CAST), CELL_BEHAVIOR_WALL, CONDITION_ALWAYS, EVENT(Prox_EndScoop)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_MOLD_SNOW_PILE, CONDITION_ALWAYS, EVENT(Prox_ScoopUpMold)},
    {TRIGGER_RAISED, RAISED_MOLD_JOINS, FLAG_MARS_BEACON_LIT, EVENT(Prox_MoldJoins)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BESIDE_UPPER_HOUSE, FLAG_HEARD_OF_PARENTS, EVENT(Prox_SearchForParents)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WEST_PATH, FLAG_CLAN_SPOKE_OF_LIGHTHOUSE, EVENT(Prox_DecideToGoToLighthouse)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEFORE_PUELLE, FLAG_MET_PUELLE, EVENT(Prox_MeetPuelle)},
    {TRIGGER_TALK, ACTOR_WARRIOR_SENT_FOR_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxPuelleOrderedMeToSet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_SENT_FOR_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxPuelleWantedToSetFelixs)},
    {TRIGGER_TALK, ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxFelixTookTroubleOfComing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxICantThinkOfAnywhere)},
    {TRIGGER_TALK, ACTOR_VILLAGER_GUARDING_THE_ROAD, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIsThatYouFelixWhen)},
    {TRIGGER_TALK, ACTOR_VILLAGER_GUARDING_THE_ROAD, CONDITION_ALWAYS, MSGNO(MsgProxSorryFelixButThisRoad)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GUARDING_THE_ROAD, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfLighthouseIsntLitSoon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GUARDING_THE_ROAD, CONDITION_ALWAYS, MSGNO(MsgProxRoadToLighthouse)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfWeWereBetterAdepts)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, CONDITION_ALWAYS, MSGNO(MsgProxIfYoureWorriedAboutLighthouse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNeitherOurElderNorPuelle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, CONDITION_ALWAYS, MSGNO(MsgProxEldersWereSayingThatLighthouse)},
    {TRIGGER_TALK, ACTOR_VILLAGER_FEELING_HELPLESS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIFeelSoHelplessAgatio)},
    {TRIGGER_TALK, ACTOR_VILLAGER_FEELING_HELPLESS, CONDITION_ALWAYS, MSGNO(MsgProxIJustDontThink)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEELING_HELPLESS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxPuelleIsStrongerThanAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEELING_HELPLESS, CONDITION_ALWAYS, MSGNO(MsgProxOnOtherHandNow)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WAITING_FOR_AGATIO, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxAllWeCanDoNow)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WAITING_FOR_AGATIO, CONDITION_ALWAYS, MSGNO(MsgProxFelixTaughtMeLotAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WAITING_FOR_AGATIO, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfTheyDidntLightBeacon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WAITING_FOR_AGATIO, CONDITION_ALWAYS, MSGNO(MsgProxMaybeIfITrainHard)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxOurPsynergyFeelsWeak)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, CONDITION_ALWAYS, MSGNO(MsgProxEvenJustStandingOutsideLighthouse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfOnlyMyFirePsynergy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, CONDITION_ALWAYS, MSGNO(MsgProxIfLightOfBeaconKeeps)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_HOSTAGES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxPuellePromisedThatHeWould)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_HOSTAGES, CONDITION_ALWAYS, MSGNO(MsgProxWellYouLookLikeYoure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_HOSTAGES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxPuelleNeverIntendedToTake)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_HOSTAGES, CONDITION_ALWAYS, MSGNO(MsgProxWhenFelixLeftProx)},
    {TRIGGER_TALK, ACTOR_PUELLE, CONDITION_ALWAYS, MSGNO(MsgProxYouMustFindKarstGet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUELLE, CONDITION_ALWAYS, MSGNO(MsgProxIfThisDoesntWork)},
    {TRIGGER_TALK, ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgProxFelixYouAreOnlyHope)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgProxIfTheyWereStrongEnough)},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_HousesTriggers[] = {
    {TRIGGER_EXIT, PROX_HOUSES_TO_PROX_SOUTH, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_SOUTH},
    {TRIGGER_EXIT, PROX_HOUSES_TO_PROX_EAST_B, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_EAST_B},
    {TRIGGER_EXIT, PROX_HOUSES_TO_PROX_CAVE_HOMES_NORTH, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_CAVE_HOMES_NORTH},
    {TRIGGER_EXIT, PROX_HOUSES_TO_PROX_WEST, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_WEST},
    {TRIGGER_EXIT, PROX_HOUSES_TO_PROX_NORTHWEST, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_NORTHWEST},
    {TRIGGER_EXIT, PROX_HOUSES_TO_PROX_CAVE_HOMES_WEST, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_CAVE_HOMES_WEST},
    {TRIGGER_EXIT, PROX_HOUSES_TO_PROX_MOLD, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_MOLD},
    {TRIGGER_EXIT, PROX_HOUSES_TO_PROX_SOUTHWEST, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_SOUTHWEST},
    {TRIGGER_EXIT, PROX_HOUSES_TO_PROX_CAVE_HOMES_SOUTH, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_CAVE_HOMES_SOUTH},
    {TRIGGER_EXIT, PROX_HOUSES_TO_PROX_EAST_A, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_EAST_A},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_RIFT_GETTING_WIDER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxRiftAtMarsLighthouseGets)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_RIFT_GETTING_WIDER, CONDITION_ALWAYS, MSGNO(MsgProxIThinkRiftToNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_RIFT_GETTING_WIDER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIThoughtProxWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_RIFT_GETTING_WIDER, CONDITION_ALWAYS, MSGNO(MsgProxIWasAfraidFalls)},
    {TRIGGER_TALK, ACTOR_VILLAGER_TOLD_OF_THE_RIFT, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxOurChieftainToldMe)},
    {TRIGGER_TALK, ACTOR_VILLAGER_TOLD_OF_THE_RIFT, CONDITION_ALWAYS, MSGNO(MsgProxIWonderIfThatHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TOLD_OF_THE_RIFT, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNobodyBelievedThatRiftWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TOLD_OF_THE_RIFT, CONDITION_ALWAYS, MSGNO(MsgProxWhatDoWeDo)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WHO_SAW_THE_RIFT, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNotTooLongAgo)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WHO_SAW_THE_RIFT, CONDITION_ALWAYS, MSGNO(MsgProxLighthouseWasSoBeautifulWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WHO_SAW_THE_RIFT, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNorthernRiftHasGotten)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WHO_SAW_THE_RIFT, CONDITION_ALWAYS, MSGNO(MsgProxAllLightsJoinedUpTogether)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_SEALING, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNoOneThoughtAboutOur)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_SEALING, CONDITION_ALWAYS, MSGNO(MsgProxIGuessFourLighthouses)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_SEALING, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxToThinkThatWeAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_SEALING, CONDITION_ALWAYS, MSGNO(MsgProxNowThatAlchemyIsFree)},
    {TRIGGER_TALK, ACTOR_VILLAGER_FEARING_FOR_PROX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfMarsLighthousesFires)},
    {TRIGGER_TALK, ACTOR_VILLAGER_FEARING_FOR_PROX, CONDITION_ALWAYS, MSGNO(MsgProxPeopleOfProxWontForget)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEARING_FOR_PROX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIThinkItsTooLate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEARING_FOR_PROX, CONDITION_ALWAYS, MSGNO(MsgProxFelixAndHisAlliesSaved)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_EMISSARIES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxWeSentEmissariesToVale)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_EMISSARIES, CONDITION_ALWAYS, MSGNO(MsgProxFelixTakeSomeAdvice)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_EMISSARIES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxAtLeastJourneyWasnt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_EMISSARIES, CONDITION_ALWAYS, MSGNO(MsgProxIfTheyFoundOutFelix)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_VALES_ELDERS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxValesEldersWereStubbornThey)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_VALES_ELDERS, CONDITION_ALWAYS, MSGNO(MsgProxIsaacAndHisFriendsLeft)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_VALES_ELDERS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxThreeYearsAgoWeSent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_VALES_ELDERS, CONDITION_ALWAYS, MSGNO(MsgProxMayorAndEldersInVale)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_IGLOOS, CONDITION_ALWAYS, EVENT(Prox_AskAboutIgloos)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_IGLOOS, CONDITION_ALWAYS, MSGNO(MsgProxIHateIgloos)},
    {TRIGGER_TALK, ACTOR_CHILD_HIDING_FROM_THE_WIND, CONDITION_ALWAYS, MSGNO(MsgProxWindIsSoCold)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_HIDING_FROM_THE_WIND, CONDITION_ALWAYS, MSGNO(MsgProxMamaAndPapaHaveBeen)},
    {TRIGGER_TALK, ACTOR_CHILD_SENT_OUTSIDE, CONDITION_ALWAYS, MSGNO(MsgProxMyParentsToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_SENT_OUTSIDE, CONDITION_ALWAYS, MSGNO(MsgProxIAlwaysCatchColdWhen)},
    {TRIGGER_TALK, ACTOR_VILLAGER_PITYING_THE_CHILDREN, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxThosePoorChildrenTheyDont)},
    {TRIGGER_TALK, ACTOR_VILLAGER_PITYING_THE_CHILDREN, CONDITION_ALWAYS, MSGNO(MsgProxICanHearSound)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PITYING_THE_CHILDREN, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxICantBringMyself)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PITYING_THE_CHILDREN, CONDITION_ALWAYS, MSGNO(MsgProxItsBeenSoLongSince)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_PUELLES_WORRY, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxPuelleOurChieftainGrewWorried)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_PUELLES_WORRY, CONDITION_ALWAYS, MSGNO(MsgProxTheresNoTellingWhatCould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_PUELLES_WORRY, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNowThatSaturosAndHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_PUELLES_WORRY, CONDITION_ALWAYS, MSGNO(MsgProxWeveLostSoManySoldiers)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_RESCUED, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxPeopleSaturosRescuedFromStorm)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_RESCUED, CONDITION_ALWAYS, MSGNO(MsgProxFelixIAlwaysThought)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_RESCUED, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxMarsIsLastLighthouse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_RESCUED, CONDITION_ALWAYS, MSGNO(MsgProxWhoKnewThatPoorInjured)},
    {TRIGGER_TALK, ACTOR_VILLAGER_POINTING_TO_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxDontForgetFelixYourMother)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_POINTING_TO_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxSaturosUsedFelixsParents)},
    {TRIGGER_TALK, ACTOR_HOUSES_PUELLE, CONDITION_ALWAYS, MSGNO(MsgProxIHaveMuchToDo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOUSES_PUELLE, CONDITION_ALWAYS, MSGNO(MsgProxWeMayHaveUnderestimatedPower)},
    {TRIGGER_TALK, ACTOR_HOUSES_ELDER, CONDITION_ALWAYS, MSGNO(MsgProxYourBodyHasAbsorbedGreat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOUSES_ELDER, CONDITION_ALWAYS, MSGNO(MsgProxPowerToReviveDead)},
    {TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(Prox_TalkToInnkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxFelixsParentsWereWorriedAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgProxWellNeverBeAble)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_FELIXS_MOTHER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxYourMotherKnowsThatYoure)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_FELIXS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgProxAfterThatEarthquakeIFeel)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_FELIXS_MOTHER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxFelixsParentsWereSoWorried)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_FELIXS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgProxWhenEarthquakeHitFelix)},
    {TRIGGER_TALK, ACTOR_MERCURY_DJINNI, CONDITION_ALWAYS, EVENT(Prox_MercuryDjinniJoins)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThoseBoilingVegetablesLookLike))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheresCheeseInThisStew))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThereIsSomeStrangeDish))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheyveGotSomeStrangeMeat))},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_CaveHomesTriggers[] = {
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK | FACING_NORTH, PROX_CAVE_HOMES_TO_PROX_HOUSES_NORTH, CONDITION_ALWAYS, EVENT(Prox_ClimbStairs)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK | FACING_NORTH, PROX_CAVE_HOMES_TO_PROX_HOUSES_WEST, CONDITION_ALWAYS, EVENT(Prox_ClimbStairs)},
    {STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK | FACING_NORTH, PROX_CAVE_HOMES_TO_PROX_HOUSES_SOUTH, CONDITION_ALWAYS, EVENT(Prox_ClimbStairs)},
    {TRIGGER_EXIT, PROX_CAVE_HOMES_TO_PROX, CONDITION_ALWAYS, PROX_CAVE_HOMES_TO_PROX},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_PROXS_PURPOSE, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxProxOriginallyFlourishedBecauseConcerned)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_PROXS_PURPOSE, CONDITION_ALWAYS, MSGNO(MsgProxIveBeenToldThatLighting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_PROXS_PURPOSE, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxWhenLighthousesWereAllExtinguished)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_PROXS_PURPOSE, CONDITION_ALWAYS, MSGNO(MsgProxAlchemyHasBeenFreed)},
    {TRIGGER_TALK, ACTOR_VILLAGER_REMEMBERING_YOUNG_FELIX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxWhenYouLeftProx)},
    {TRIGGER_TALK, ACTOR_VILLAGER_REMEMBERING_YOUNG_FELIX, CONDITION_ALWAYS, MSGNO(MsgProxThankYouSoMuchFelix)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_REMEMBERING_YOUNG_FELIX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxWhatHasHeGoneThrough)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_REMEMBERING_YOUNG_FELIX, CONDITION_ALWAYS, MSGNO(MsgProxINeverImaginedThatLittle)},
    {TRIGGER_TALK, ACTOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT(Prox_TalkToShopkeeper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHOPKEEPER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxFelixLooksSoConfident)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgProxItsTooBadFelixCant)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_HER_COOKING, CONDITION_ALWAYS, MSGNO(MsgProxThinkOfYourOldGrandmas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_HER_COOKING, CONDITION_ALWAYS, MSGNO(MsgProxImSoSadFelixDoesnt)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThoseBoilingVegetablesLookLike))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheresCheeseInThisStew))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThereIsSomeStrangeDish))},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheyveGotSomeStrangeMeat))},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_CavesTriggers[] = {
    {TRIGGER_EXIT, PROX_CAVES_TO_PROX, CONDITION_ALWAYS, PROX_CAVES_TO_PROX},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Prox_TalkToHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxTheseCatastrophesAreResult)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgProxLighthousesAreGoingToRestore)},
    {TRIGGER_TABLE_END},
};

s32 gProx_IsaacStepsForward[] = {
    SC_ANIMATION(2),
    SC_MOVE(120, 0, 196),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(10),
    SC_END,
};

s32 gProx_KradenStepsForward[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 196),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(10),
    SC_END,
};

s32 gProx_JennasFatherWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(136, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gProx_JennasMotherWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gProx_LeftFileWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(136, 0, 248),
    SC_SYNC,
    SC_MOVE(136, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gProx_RightFileWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 248),
    SC_SYNC,
    SC_MOVE(152, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gProx_GaretWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 248),
    SC_SYNC,
    SC_MOVE(152, 0, 272),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_END,
};

s32 gProx_JennaWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(168, 0, 272),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_END,
};

s32 gProx_IsaacWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(120, 0, 220),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(1),
    SC_END,
};

s32 gProx_LeaderWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(140, 0, 220),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(1),
    SC_END,
};

s32 gProx_KradenWalksSouth[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 240),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(10),
    SC_END,
};

s32 gProx_JennaRunsAhead[] = {
    SC_ANIMATION(2),
    SC_MOVE(168, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

// The same script as gProx_JennasMotherWalksSouth, under another name.
s32 gProx_GaretRunsAhead[] = {
    SC_ANIMATION(2),
    SC_MOVE(152, 0, 348),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};
// clang-format on
