/// @file
/// Bilibin, the Bilibin Treasury and the Bilibin Barricade, three places that
/// share one overlay, each a scene of its own: SceneId_Bilibin,
/// SceneId_BilibinTreasury and SceneId_BilibinBarricade. The getters pick
/// each scene's tables by gGameState.scene, Bilibin_Initialize sets up the
/// scene being entered, and Event_PushPillar serves all three.
///
/// Bilibin (SceneId_Bilibin) is the town past the Goma Range's cave, and the
/// party finds it afraid. A man who came to the village under a curse stands at
/// its entrance turned into a tree (ACTOR_TREE_MAN); his last words were that
/// he had offended the sacred tree. Lord McCoy, the chief of Bilibin, has
/// closed the road to Kolima with a barricade, and the villagers' talk sends
/// the party to him and on to Kolima. Almost everything here turns on
/// FLAG_KOLIMA_CURSE_LIFTED, whether Kolima's curse is lifted, which the party
/// sets far from here, in Kolima Forest, when it heals Tret with the Water of
/// Hermes. Until then the tree stands at the entrance: a first look says it
/// almost looks like a man, a second (place flag FLAG_PLACE_TEMP_01 remembers
/// the first until the party leaves Bilibin) that it looks like a living
/// person, and Mind Read hears `Help me... Somebody...`. Once the curse is
/// lifted, Bilibin_RestoreVillageActors puts the man himself
/// (ACTOR_FORMER_TREE_MAN) beside the empty spot and turns the pair
/// ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST and ACTOR_OLD_WOMAN_ROAD_BLOCKED about.
/// Each of the ten villagers (ACTOR_YOUNG_MAN_WELCOME to
/// ACTOR_VILLAGER_STRANGE_CREATURES) has two lines and two thoughts in
/// gBilibinEvents, the row that needs the flag clear first, so the first row
/// that matches picks the time: before, they talk of the curse, the barricade,
/// McCoy's fear and the lighthouse of Imil; after, of the three young warriors
/// who broke the curse. A few ask the party a yes-or-no question (the
/// Event_AskAbout functions).
///
/// Bilibin's doors (touches 3 to 9) open before the party walks in
/// (Event_OpenDoorAndLeave): six lead into Bilibin's buildings
/// (SceneId_BilibinInteriors, bilibin_interiors.c) and the seventh into the
/// Bilibin Sanctum. Coming back out of the Sanctum (BILIBIN_FROM_SANCTUM) the
/// area name is not shown again. The other exits lead to the world map, to the
/// garden of McCoy's Palace and, two ways, into the Treasury: past the ivy at
/// the west edge (ACTOR_IVY, which has a Whirlwind row that runs nothing),
/// gone when the party comes back out that way
/// (BILIBIN_FROM_BILIBIN_TREASURY_WEST); and from the cell a statue
/// (ACTOR_VILLAGE_PILLAR) stands on, once Move or a push slides it aside.
/// Bilibin_UpdateVillagePillarCells keeps the cells of the statue's lane in
/// step and hops the leader clear if it lands on him; coming back out by that
/// way (BILIBIN_FROM_BILIBIN_TREASURY_EAST) the party finds the statue already
/// moved. Place flag FLAG_PLACE_TEMP_04 is set while McCoy's gate guards have
/// let the party in (FLAG_MCCOY_PALACE_ADMITTED) but it has not yet entered the
/// palace (FLAG_MCCOY_PALACE_VISITED), and the garden's east guard tests it to
/// scold the party for keeping McCoy waiting. XXX: the garden is another group
/// of scenes (9, Bilibin's is 8), and Scene_ResetFlagsOnEnter clears place
/// flags on entering a new group, so the guard seems never to see it set; a
/// playthrough would settle it.
///
/// The Bilibin Treasury (BilibinTreasury) is what its sign calls `McCoy's
/// Hidden Warehouse`. Gust, a Jupiter Djinni, waits there, placed until he has
/// joined, and joins the party when spoken to (Event_GustJoins). A second
/// statue (ACTOR_TREASURY_PILLAR) slides along one column, and
/// Bilibin_UpdateTreasuryPillarCells keeps the two cells it can stand on in
/// step. Arriving by BILIBIN_TREASURY_UPPER_FROM_LOWER or
/// BILIBIN_TREASURY_FROM_BILIBIN_NORTH the leader starts 16 pixels above the
/// floor; two exit regions (Bilibin_GetRegions) lead down its west ladder to
/// the lower room and back to Bilibin by the statue.
///
/// The Bilibin Barricade (BilibinBarricade) is the road east to Kolima where
/// McCoy's barricade stands, reached from the world map at either end. Its sign
/// reads `This area is off-limits to all those without a permit. —Lord McCoy`,
/// and a door there stays locked (`The door is tightly locked.`). At McCoy's
/// Palace the worker who built the barricade calls it shabby, a gate guard
/// thinks its hinges might break, and McCoy's captain hints that the party may
/// not need a key (mccoy_palace.c, mccoy_palace_grounds.c). A stack of crates
/// (ACTOR_RIVER_PILLAR) slides a cell at a time under Move or a push, and
/// Bilibin_UpdateRiverPillarCells rewrites the ten cells of its lane after each
/// move. By the river stand three trees, one on another's shoulders
/// (ACTOR_BOTTOM_TREE, ACTOR_MIDDLE_TREE and ACTOR_JILL_TREE at one spot, 0, 32
/// and 48 pixels up): three children of the town, the Gang of Three, turned
/// into trees while they climbed to pick persimmons. Talking to them says only
/// that they look like a person; the bottom one thinks `They're... so
/// heavy...`.
///
/// When the party passes again after Saturos is beaten at Mercury Lighthouse
/// (FLAG_SATUROS_BEATEN_AT_MERCURY), with the curse still in place, the
/// travelers from Imil have crossed the river by pushing the trees down (the
/// bottom tree thinks `They crossed the river after pushing us down.`):
/// Bilibin_RestoreRiverActors lays the middle tree on its side by the river,
/// with an invisible actor (ACTOR_FALLEN_TREE_STAND_IN) that answers for it,
/// and puts the top tree in the water. That tree is Jill: Mind Read hears her
/// cry for help, and her friends fear she will be swept downstream. Move on her
/// (Event_PullJillFromRiver) works once she is in cell column 6, by the bank:
/// the leader pulls her ashore in three tugs, she thanks the party, and
/// FLAG_BILIBIN_JILL_RESCUED keeps her on the bank. While whoever the player
/// controls is west of x 142 and under 8 pixels up, Bilibin_UpdateRiverEffect
/// leaves an effect at their feet every 30 frames.
///
/// Once the curse is lifted the trees are gone, and the Gang of Three are
/// children again on the bank (ACTOR_GANG_MEMBER_EAST, ACTOR_GANG_MEMBER_WEST
/// and ACTOR_JILL). If Jill was never pulled out she is missing, and her
/// friends mourn that they are only the Gang of Two. If she was, stepping up to
/// them (touch 13) plays Event_JillGivesHardNut: her friends recognize the
/// leader, Jill reminds him she was that tree and gives him a Hard Nut
/// (FLAG_BILIBIN_JILL_GIFT_RECEIVED), and from then on the three turn to watch
/// him.

#include "game_state.h"
#include "djinn_ids.h"
#include "flags.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/bilibin.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "bilibin.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "field_obj.h"
#include "songs.h"
#include "item_ids.h"

static void Bilibin_RestoreVillageActors(void);
static void Bilibin_UpdateVillagePillarCells(void);
static void Bilibin_RestoreRiverActors(void);
static void Bilibin_UpdateRiverPillarCells(void);
static void Bilibin_SetUpTreasury(void);
static void Bilibin_UpdateRiverEffect(void);
static void Bilibin_SpawnRiverEffect(struct FieldActor *actor);
static void Bilibin_UpdateTreasuryPillarCells(void);

/// Actors of Bilibin, numbering its placement rows from
/// ACTOR_FIRST_PLACED. The ten villagers are named by what they say before the
/// curse is lifted.
enum BilibinActor {
  /// Ivy, sprite 0xf8 (Crossbone Isle's ivy), drawn half as tall again, at the
  /// west edge where the way into the Treasury runs; its Whirlwind row runs
  /// nothing.
  /// ★ Named from what the player sees: leafy ivy against the cliff at the
  /// village's west edge, the sprite of the ivy Whirlwind blows away (its
  /// render)
  ACTOR_IVY = ACTOR_FIRST_PLACED,
  /// The man turned into a tree at the village entrance, drawn as a tree.
  ACTOR_TREE_MAN,
  /// A young man with brown hair: "Welcome to Bilibin! Please, just ignore that
  /// strange tree out front!"
  ACTOR_YOUNG_MAN_WELCOME,
  /// A woman with fair hair: "I hear a large group crossed over Goma Range,
  /// heading toward Imil."
  ACTOR_VILLAGER_GROUP_TO_IMIL,
  /// A burly grey-haired man: "Did you see the tree at the entrance?"
  /// (Event_AskAboutEntranceTree)
  ACTOR_VILLAGER_ASKS_ABOUT_TREE,
  /// A woman in a green dress: "Lord McCoy has been quite frightened since
  /// the incident."
  ACTOR_VILLAGER_MCCOY_FRIGHTENED,
  /// An old man with a cane: "Have you tried heading southeast from here?"
  /// (Event_AskAboutSoutheastRoute)
  ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST,
  /// An old woman: "The road to Kolima is blocked now, as per Lord McCoy's
  /// orders."
  ACTOR_OLD_WOMAN_ROAD_BLOCKED,
  /// A small boy: "Ever since that man turned into a tree, everyone has been
  /// too scared to go near the town's entrance." Mind Read: "Is that how all
  /// grown-ups are?"
  ACTOR_BOY_SCARED_OF_ENTRANCE,
  /// A little girl: "Is it true that a mountain exploded on the other side of
  /// Goma? The sky was a brilliant red that day."
  ACTOR_GIRL_RED_SKY,
  /// A strongman, stocky and muscular: "There's a tower at the tip of Imil
  /// that people say was built by the gods!"
  ACTOR_STRONGMAN_IMIL_TOWER,
  /// A man with a pickaxe: "Something changed in the eruption. Now, all sorts
  /// of strange creatures lurk in every forest and mountain!"
  ACTOR_VILLAGER_STRANGE_CREATURES,
  ACTOR_VILLAGE_PILLAR, ///< a statue, moved with Move or pushed
  /// The same man once the curse is lifted, a stocky, muscular man with black
  /// hair.
  ACTOR_FORMER_TREE_MAN
};

/// Actors of the Bilibin Barricade (BilibinBarricade), numbering its placement
/// rows from ACTOR_FIRST_PLACED.
enum BilibinBarricadeActor {
  /// A stack of crates, moved with Move or pushed.
  ACTOR_RIVER_PILLAR = ACTOR_FIRST_PLACED,
  /// The bottom of the three trees. Mind Read: "They're... so heavy..."
  ACTOR_BOTTOM_TREE,
  /// The tree 32 pixels up, which the travelers from Imil push down onto its
  /// side by the river.
  ACTOR_MIDDLE_TREE,
  ACTOR_JILL_TREE, ///< Jill as a tree, the top of the three
  /// Jill's two friends, once the curse is lifted, east and west of her: two
  /// boys with brown hair. With her they were the "Gang of Three".
  ACTOR_GANG_MEMBER_EAST,
  /// ★ Named from the game: "I heard we were once called the Gang of Three."
  /// (MsgBilibinIHeardWeWereOnceCalled)
  ACTOR_GANG_MEMBER_WEST,
  /// Jill herself, once the curse is lifted, drawn as the woman with long fair
  /// hair the villages use (SPRITE_WOMAN_WITH_FAIR_HAIR), though the text calls
  /// the three children.
  /// ★ Named from the game: "Jill, are these the people who saved you?"
  /// (MsgBilibinYoureGuy)
  ACTOR_JILL,
  /// The invisible actor that stands where the middle tree lies pushed down and
  /// answers for it.
  ACTOR_FALLEN_TREE_STAND_IN = ACTOR_FIRST_PLACED + 8
};

/// Actors of the Bilibin Treasury (BilibinTreasury), numbering its placement
/// rows from ACTOR_FIRST_PLACED.
enum BilibinTreasuryActor {
  /// A statue, moved with Move or pushed.
  ACTOR_TREASURY_PILLAR = ACTOR_FIRST_PLACED,
  /// Gust, a Jupiter Djinni, placed until he has joined.
  /// ★ Named from the game: "Gust" (MsgDjinnName)
  ACTOR_GUST
};

/// Counts frames between the river effects, one every 30 frames.
struct SpawnCounter {
  s16 frames;
};

struct SpawnCounter gBilibin_RiverEffectTimer;

#include "common/object_facing.h"

const struct SceneEntrance *Bilibin_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Bilibin) {
    return gBilibinEntrances;
  }
  if (scene == (s32)&SceneId_BilibinBarricade) {
    return gBilibinBarricadeEntrances;
  }
  if (scene == (s32)&SceneId_BilibinTreasury) {
    return gBilibinTreasuryEntrances;
  }
  return (const struct SceneEntrance *)gBilibinEntrancesOther;
}

/// Only the Bilibin Treasury has exit regions.
const struct SceneRegion *Bilibin_GetRegions(void) {
  if (gGameState.scene == (s32)&SceneId_BilibinTreasury) {
    return (const struct SceneRegion *)gBilibinTreasuryRegions;
  }
  return 0;
}

/// Returns the packed exit destinations shared by the three maps.
const u32 *Bilibin_GetExits(void) { return gBilibinExits; }

/// Talking to Gust in the Bilibin Treasury: Gust, a Jupiter Djinni, joins the
/// party.
static void Event_GustJoins(void) {
  Djinn_JoinParty(ACTOR_GUST, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_GUST));
}

/// Selects the actors of Bilibin, the Bilibin Barricade or the Bilibin
/// Treasury.
const struct ScenePlacement *Bilibin_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Bilibin) {
    return gBilibinPlacements;
  }
  if (scene == (s32)&SceneId_BilibinBarricade) {
    return gBilibinBarricadePlacements;
  }
  if (scene == (s32)&SceneId_BilibinTreasury) {
    return gBilibinTreasuryPlacements;
  }
  return gBilibinPlacementsOther;
}

/// Searching the well in Bilibin: the leader peers in, and its bottom cannot
/// be seen.
static void Event_InspectWell(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgBilibinBottomNotVisibleLooksVery, 1);
  Engine_EventEnd();
}

const struct SceneEvent *Bilibin_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Bilibin) {
    return gBilibinEvents;
  }
  if (scene == (s32)&SceneId_BilibinBarricade) {
    return gBilibinBarricadeEvents;
  }
  if (scene == (s32)&SceneId_BilibinTreasury) {
    return gBilibinTreasuryEvents;
  }
  return gBilibinEventsOther;
}

/// The Bilibin Barricade's sign forbids passing without a permit. Searching it
/// shows Lord McCoy's notice that no one may pass without one.
static void Event_ReadRestrictedAreaSign(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgBilibinAreaOffLimitsThoseWithout, 1);
  Engine_EventEnd();
}

/// Searching the sign in the Bilibin Treasury, which calls it McCoy's hidden
/// warehouse. `McCoy's Hidden Warehouse / Do Not Enter!`
static void Event_ReadWarehouseSign(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgBilibinMccoysHiddenWarehouseDoNot, 1);
  Engine_EventEnd();
}

/// The Gang of Three's trees at the Bilibin Barricade look like people.
/// Talking to one, standing or pushed down, or to the actor that answers for
/// the fallen one, says so.
/// `There is a tree that looks like a person.`
static void Event_InspectHumanShapedTree(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgBilibinThereTreeLooksLikePerson, 1);
  Engine_EventEnd();
}

/// Talking to the tree at Bilibin's entrance, before the curse is lifted: it
/// almost looks like a man and, looked at again, like a living person. Place
/// flag FLAG_PLACE_TEMP_01 remembers the first look until the party leaves
/// Bilibin's group of scenes.
static void Event_ExamineTreeMan(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinItsTreeButAlmostLooks);
  if (GameFlag_IsSet(FLAG_PLACE_TEMP_01) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessage(ACTOR_TREE_MAN, 0);
  GameFlag_Set(FLAG_PLACE_TEMP_01);
  Engine_EventEnd();
}

/// A burly man asks about the tree at Bilibin's entrance. He's grey-haired and
/// there before the curse is lifted, and asks whether the party saw it; on
/// yes, he says it was once a man who came under a curse.
static void Event_AskAboutEntranceTree(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinDidSeeTreeAtEntrance);
  Engine_EventAskYesNo(ACTOR_VILLAGER_ASKS_ABOUT_TREE, 0);
  Engine_EventEnd();
}

/// An old man asks whether the party has tried heading southeast. He's in
/// Bilibin before the curse is lifted, and either way he says the barricade
/// blocks the road.
static void Event_AskAboutSoutheastRoute(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinHaveTriedHeadingSoutheastFrom);
  Engine_EventAskYesNo(ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, 0);
  Engine_EventEnd();
}

/// The man who was the tree at the entrance talks about it. He's in Bilibin
/// once the curse is lifted, and asks whether the party can believe he was a
/// tree.
static void Event_AskAboutBeingATree(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinWasTurnedIntoTreeFor);
  Engine_EventAskYesNo(ACTOR_FORMER_TREE_MAN, 0);
  Engine_EventEnd();
}

/// A small boy asks whether the party broke the curse. He's in Bilibin once the
/// curse is lifted, and asks whether the party are the three young warriors who
/// broke it.
static void Event_AskAboutCurseBreakers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinCurseWasBrokenThanksEfforts);
  Engine_EventAskYesNo(ACTOR_BOY_SCARED_OF_ENTRANCE, 0);
  Engine_EventEnd();
}

/// A strongman asks whether the party has been to Imil. He's in Bilibin once
/// the curse is lifted, and asks whether the party has been there, up north,
/// and seen its lighthouse.
static void Event_AskAboutImil(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinHaveEverBeenVillageImil);
  Engine_EventAskYesNo(ACTOR_STRONGMAN_IMIL_TOWER, 0);
  Engine_EventEnd();
}

/// Walking into one of Bilibin's doors: everyone stops, the door opens and the
/// leader walks through it. The doors' touch codes, 3 to 9, are the numbers of
/// the exits they take: BILIBIN_TO_INN to BILIBIN_TO_WEAPON_AND_ARMOR_SHOP lead
/// into Bilibin's buildings, and BILIBIN_TO_SANCTUM into the Bilibin Sanctum,
/// whose door sounds and is stepped through differently.
static void Event_OpenDoorAndLeave(void) {

  u32 actorSlot;
  struct FieldActor *actor;
  s32 exitIndex;
  u8 *originBytes;
  s32 originOffset;
  s32 rowOffset;
  s32 column;
  s32 row;
  struct EventWork *work;

  work = gEventWork;
  Engine_EventBegin();
  // Freeze placed actors before the exit cells start animating.
  for (actorSlot = ACTOR_FIRST_PLACED; actorSlot < 66; actorSlot++) {
    actor = Object_GetById(actorSlot);
    if (actor != 0) {
      actor->motionFlags = 0;
    }
  }
  exitIndex = (((u16)work->touchedTrigger - 3) << 16) >> 16;
  if (exitIndex == BILIBIN_TO_SANCTUM - BILIBIN_TO_INN) {
    Event_PlayCue(SE_188);
  } else {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
  }
  // Each door has its cell and the steps that draw it opening.
  originOffset = exitIndex << 2;
  originBytes = (u8 *)gBilibin_DoorCells;
  column = *(s16 *)(originBytes + originOffset);
  rowOffset = originOffset + 2;
  row = *(s16 *)(originBytes + rowOffset);
  Engine_MapAnimateCells(gBilibin_DoorSteps[exitIndex], column, row);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (exitIndex == BILIBIN_TO_SANCTUM - BILIBIN_TO_INN) {
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
  } else {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  }
  if (exitIndex == BILIBIN_TO_ITEM_SHOP - BILIBIN_TO_INN) {
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  } else {
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  }
  Engine_EventWait(16);
  Engine_EventRequestExit(exitIndex + BILIBIN_TO_INN);
  Engine_EventEnd();
}

/// Move on Jill's tree in the river at the Bilibin Barricade, until she is
/// rescued: once Move has brought her to cell column 6, by the bank, the leader
/// pulls her ashore in three tugs, and she thanks the party for saving her from
/// being washed downstream.
static void Event_PullJillFromRiver(void) {
  struct FieldActor *leader;
  struct FieldActor *jillTree;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  jillTree = Object_GetById(ACTOR_JILL_TREE);
  if ((jillTree->x.fixed >> 20) == 6) {
    Engine_EventBegin();
    Engine_ActorSetSpritePriority(ACTOR_JILL_TREE, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
    Actor_SetSpeed(ACTOR_JILL_TREE, PIXELS(1) / 5, PIXELS(1) / 10);
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    jillTree->motionFlags = 0;
    // Each tug: the leader faces about, strains and draws her in, then steps
    // on.
    leader->scaleX = -0x10000;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_MoveToAndWait(ACTOR_JILL_TREE, 111, 196);
    leader->scaleX = 0x10000;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 185);
    Engine_EventWait(20);
    leader->scaleX = -0x10000;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_MoveToAndWait(ACTOR_JILL_TREE, 121, 190);
    leader->scaleX = 0x10000;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 141, 189);
    Engine_EventWait(20);
    leader->scaleX = -0x10000;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_MoveToAndWait(ACTOR_JILL_TREE, 132, 186);
    leader->scaleX = 0x10000;
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 166, 185);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    Engine_ActorSetSpritePriority(ACTOR_JILL_TREE, 2);
    RunSceneTransitionEffect(0, ACTOR_JILL_TREE);
    Engine_TaskWait(10);
    Engine_EventSetMessage(MsgBilibinThankSavedMeFromBeing);
    Event_ShowMessage(ACTOR_JILL_TREE, 0);
    Engine_PsynergyCancel();
    Engine_TaskWait(10);
    GameFlag_Set(FLAG_BILIBIN_JILL_RESCUED);
    Engine_EventEnd();
  }
}

/// Checks ahead before the leader takes another step. It runs on walking into
/// the cells beside the statue in the Bilibin Treasury or the crates at the
/// Bilibin Barricade, and checks the blocked direction before allowing another
/// movement step.
static void Event_CheckAhead(void) { Engine_LeaderCheckAhead(); }

/// Jill and her friends thank the leader with a Hard Nut. It happens on
/// stepping up to them on the bank at the Bilibin Barricade, once the curse is
/// lifted and if she was pulled from the river: they recognize the leader, and
/// she gives him the Hard Nut in thanks.
/// "Remember me? I was that tree!"
static void Event_JillGivesHardNut(void) {
  struct FieldActor *leader;
  struct FieldActor *eastMember;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (GameFlag_IsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0 ||
      GameFlag_IsSet(FLAG_BILIBIN_JILL_RESCUED) == 0)
    return;

  Engine_EventBegin();
  // The camera moves over to the three; the friend nearer the leader speaks
  // first.
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(263), -1, PIXELS(173), 1);
  Engine_CameraWaitForMove();
  eastMember = Actor_Get(ACTOR_GANG_MEMBER_EAST);
  if (eastMember->x.fixed > leader->x.fixed) {
    Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTH + FACING_STEP, 20);
    Actor_ShowEmote(ACTOR_GANG_MEMBER_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    20);
    Engine_EventSetMessage(MsgBilibinYoureGuy);
    Event_ShowMessageAndWait(ACTOR_GANG_MEMBER_WEST, 0, 10);
    Actor_ShowEmote(ACTOR_GANG_MEMBER_EAST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
  } else {
    Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTHEAST + FACING_STEP,
                        20);
    Actor_ShowEmote(ACTOR_GANG_MEMBER_EAST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    20);
    Engine_EventSetMessage(MsgBilibinYoureGuy);
    Event_ShowMessageAndWait(ACTOR_GANG_MEMBER_EAST, 0, 10);
    Actor_ShowEmote(ACTOR_GANG_MEMBER_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
  }
  Actor_ShowEmote(ACTOR_JILL, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_JILL, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTHEAST + FACING_STEP,
                      0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 268, 184);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_GANG_MEMBER_WEST, 2);
  Event_ShowMessageAndWait(ACTOR_GANG_MEMBER_WEST, 0, 10);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JILL, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_WEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
  Actor_SetAttachedEffect(ACTOR_JILL, 0x102);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_JILL, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTHEAST + FACING_STEP,
                      10);
  Engine_ActorRunRepeatedMotion(ACTOR_JILL, 1);
  Event_ShowMessageAndWait(ACTOR_JILL, 0, 10);
  Engine_ActorSetAnimation(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_GANG_MEMBER_WEST, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_JILL, 0);
  Actor_SetSpeed(ACTOR_JILL, PIXELS(3) / 5, PIXELS(3) / 10);
  Object_GetById(ACTOR_JILL)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JILL, 266, 172);
  Engine_EventWait(1);
  Object_GetById(ACTOR_JILL)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_JILL, 0, 10);
  // Show the named gift before adding it to the party's inventory.
  Engine_MessageShowCentered(MsgBilibinJillGaveIsaacSpecialGift, 1);
  gEventWork->message++;
  Engine_ItemShowFound(ITEM_HARD_NUT, 3);
  Engine_PartyGiveItem(ITEM_HARD_NUT, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_SetSpeed(ACTOR_JILL, PIXELS(1), PIXELS(1) / 2);
  Object_GetById(ACTOR_JILL)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JILL, 262, 156);
  Engine_EventWait(1);
  {
    struct FieldActor *jill = Actor_Get(ACTOR_JILL);
    u8 steeringFlags = jill->steeringFlags | ACTOR_FACES_MOTION;

    jill->steeringFlags = steeringFlags;
  }
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_GANG_MEMBER_EAST, 2);
  Event_ShowMessageAndWait(ACTOR_GANG_MEMBER_EAST, 0, 10);
  Engine_ActorSetAnimation(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_GANG_MEMBER_WEST, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
  // From now on the three turn to face the leader wherever he goes.
  FieldObject_SetTargetAndCallback(ACTOR_GANG_MEMBER_EAST,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gBilibin_FaceLeaderScript);
  FieldObject_SetTargetAndCallback(ACTOR_GANG_MEMBER_WEST,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gBilibin_FaceLeaderScript);
  FieldObject_SetTargetAndCallback(ACTOR_JILL,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gBilibin_FaceLeaderScript);
  GameFlag_Set(FLAG_BILIBIN_JILL_GIFT_RECEIVED);
  Engine_EventEnd();
}

/// Sets up the scene being entered, Bilibin, the Bilibin Barricade or the
/// Bilibin Treasury: its actors by story flags, its collision cells and its
/// opening transition.
s32 Bilibin_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gGameState.scene == (s32)&SceneId_Bilibin) {
    Bilibin_RestoreVillageActors();
  } else if (gGameState.scene == (s32)&SceneId_BilibinBarricade) {
    Bilibin_RestoreRiverActors();
    FieldScheduler_AddCallback(Bilibin_UpdateRiverEffect, 0xc80);
  } else if (gGameState.scene == (s32)&SceneId_BilibinTreasury) {
    Bilibin_SetUpTreasury();
  }
  return 0;
}

/// Sets up Bilibin: the tree or the man by FLAG_KOLIMA_CURSE_LIFTED, the bush
/// and the statue by the entrance used, and place flag FLAG_PLACE_TEMP_04 for
/// McCoy's garden guard.
static void Bilibin_RestoreVillageActors(void) {

  struct FieldActor *actor;
  s16 entrance;

  if (GameFlag_IsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_ActorSetPosition(ACTOR_TREE_MAN, 0, 0);
    Actor_FaceDirection(ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST,
                        FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_OLD_WOMAN_ROAD_BLOCKED,
                        FACING_SOUTH + FACING_STEP, 0);
  } else {
    actor = Actor_Get(ACTOR_TREE_MAN);
    Engine_ActorSetSpriteFlags(actor, 0);
    Actor_SetPosition(ACTOR_FORMER_TREE_MAN, 0, 0);
  }
  // Apply arrival-specific changes after restoring the population.
  actor = Actor_Get(ACTOR_IVY);
  actor->scaleY = 0x18000;
  entrance = gGameState.entrance;
  if (entrance == BILIBIN_FROM_BILIBIN_TREASURY_WEST) {
    Actor_SetPosition(ACTOR_IVY, 0, 0);
  } else if (entrance == BILIBIN_FROM_SANCTUM) {
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  }
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    entrance = gGameState.entrance;
    if (entrance == BILIBIN_FROM_BILIBIN_TREASURY_EAST) {
      Actor_SetPosition(ACTOR_VILLAGE_PILLAR, PIXELS(248), PIXELS(216));
    }
  }
  Bilibin_UpdateVillagePillarCells();
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_ADMITTED) != 0 &&
      GameFlag_IsSet(FLAG_MCCOY_PALACE_VISITED) == 0) {
    GameFlag_Set(FLAG_PLACE_TEMP_04);
  }
}

/// Updates the cells around the statue in Bilibin after it moves. It runs after
/// Move, a push, or entry: it updates the collision cells of the statue's lane,
/// opens its home cell (16, 13) once it has left it, and moves the leader clear
/// if it has landed on him.
static void Bilibin_UpdateVillagePillarCells(void) {
  s32 leaderRow;
  s32 leaderColumn;
  struct FieldActor *leader;
  struct FieldActor *pillar;
  s32 pillarX;
  s32 pillarRow;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  pillar = Object_GetById(ACTOR_VILLAGE_PILLAR);
  pillarRow = pillar->z.fixed >> 20;
  leaderColumn = (leader->x.fixed >> 20);
  leaderRow = (leader->z.fixed >> 20);
  pillarX = pillar->x.fixed;
  // Repaint the pillar's lane before clearing the cells it currently occupies.
  Map_CopyCellAttributes(15, 11, 3, 1, 15, 12);
  Map_CopyCellAttributes(15, 11, 3, 1, 15, 13);
  Map_CopyCellAttributes(15, 11, 3, 1, 15, 14);
  Map_CopyCellAttributes(1, 0, 1, 1, (pillarX >> 20), pillarRow);
  if ((pillarX >> 20) != 16 || pillarRow != 13) {
    Map_CopyCellAttributes(0, 0, 1, 1, 16, 13);
  }
  // The pillar may have moved onto the leader while this event was running.
  if (leaderColumn == 16) {
    if (leaderRow == 13) {
      Engine_EventBegin();
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      20);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
      Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
      if (pillarRow == 13) {
        Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 262, 196);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
      } else {
        Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 286, 218);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
      }
      Engine_EventEnd();
    }
  }
}

/// Sets up the Bilibin Barricade: before the curse is lifted, Jill and her
/// friends are trees here, stacked or, once Saturos is beaten at Mercury
/// Lighthouse, pushed down; afterwards they are children again on the bank.
static void Bilibin_RestoreRiverActors(void) {
  struct FieldActor *jillTree;
  s32 populationState;
  struct FieldActor *fallenTree;
  struct FieldActor *actor;
  s32 crossingState;

  fallenTree = Object_GetById(ACTOR_MIDDLE_TREE);
  jillTree = Object_GetById(ACTOR_JILL_TREE);
  actor = Actor_Get(ACTOR_RIVER_PILLAR);
  Engine_ActorSetSpriteFlags(actor, 0);
  populationState = GameFlag_IsSet(FLAG_KOLIMA_CURSE_LIFTED);
  if (populationState != 0) {
    // The curse is lifted: the trees are gone.
    Actor_SetPosition(ACTOR_BOTTOM_TREE, 0, 0);
    Actor_SetPosition(ACTOR_MIDDLE_TREE, 0, 0);
    Actor_SetPosition(ACTOR_JILL_TREE, 0, 0);
    Map_CopyCellsTo(56, 15, 40, 15, 1, 2);
    Map_CopyCellAttributes(26, 15, 1, 3, 10, 15);
    if (GameFlag_IsSet(FLAG_BILIBIN_JILL_GIFT_RECEIVED) == 0) {
      if (GameFlag_IsSet(FLAG_BILIBIN_JILL_RESCUED) == 0) {
        Engine_ActorSetPosition(ACTOR_JILL, 0, 0);
        Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_NORTH + FACING_STEP,
                            0);
        Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST,
                            FACING_NORTHWEST + FACING_STEP, 0);
      }
    } else {
      Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_NORTH + FACING_STEP,
                          0);
      Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST,
                          FACING_NORTHWEST + FACING_STEP, 0);
    }
  } else {
    Actor_SetPosition(ACTOR_GANG_MEMBER_EAST, 0, 0);
    Actor_SetPosition(ACTOR_GANG_MEMBER_WEST, 0, 0);
    Actor_SetPosition(ACTOR_JILL, 0, 0);
    actor = Actor_Get(ACTOR_BOTTOM_TREE);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor = Actor_Get(ACTOR_MIDDLE_TREE);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor = Actor_Get(ACTOR_JILL_TREE);
    Engine_ActorSetSpriteFlags(actor, 0);
    fallenTree->motionFlags = populationState;
    // Once FLAG_SATUROS_BEATEN_AT_MERCURY is set ("They crossed the river
    // after pushing us down"), the middle tree lies on its side and Jill is in
    // the water, or on the bank where she was pulled ashore.
    crossingState = GameFlag_IsSet(FLAG_SATUROS_BEATEN_AT_MERCURY);
    if (crossingState != 0) {
      Object_GetById(ACTOR_BOTTOM_TREE)->collisionFlags |= 16;
      Object_GetById(ACTOR_FALLEN_TREE_STAND_IN)->collisionFlags |= 16;
      Object_GetById(ACTOR_JILL_TREE)->collisionFlags |= 16;
      Actor_SetPosition(ACTOR_FALLEN_TREE_STAND_IN, PIXELS(142), PIXELS(156));
      actor = Actor_Get(ACTOR_FALLEN_TREE_STAND_IN);
      Engine_ActorSetSpriteFlags(actor, 0);
      Actor_SetPosition(ACTOR_MIDDLE_TREE, PIXELS(142), PIXELS(156));
      fallenTree->sprite->rotation = 0x4000;
      fallenTree->y.fixed += -0x80000;
      if (GameFlag_IsSet(FLAG_BILIBIN_JILL_RESCUED) != 0) {
        Actor_SetPosition(ACTOR_JILL_TREE, PIXELS(132), PIXELS(186));
      } else {
        Actor_SetPosition(ACTOR_JILL_TREE, PIXELS(88), PIXELS(196));
        Engine_ActorSetSpritePriority(ACTOR_JILL_TREE, 3);
        jillTree->collisionFlags |= 4;
      }
    } else {
      fallenTree->y.fixed = PIXELS(32);
      jillTree->motionFlags = crossingState;
      jillTree->y.fixed = PIXELS(48);
    }
  }
  Bilibin_UpdateRiverPillarCells();
}

/// After the crates at the Bilibin Barricade move (Move, a push, or entry):
/// restores the ten cells of their lane, then marks the cell they stand on.
static void Bilibin_UpdateRiverPillarCells(void) {
  struct FieldActor *actor;
  s32 pillarColumn;
  s32 pillarRow;
  u32 cellOffset;

  actor = Object_GetById(ACTOR_RIVER_PILLAR);
  pillarColumn = actor->x.fixed >> 20;
  pillarRow = actor->z.fixed >> 20;

  // The packed table stores ten byte-sized column/row pairs.
  for (cellOffset = 0; cellOffset < 20; cellOffset += 2) {
    s32 column = ((u8 *)gBilibin_RiverPillarCells)[cellOffset];
    s32 row = ((u8 *)gBilibin_RiverPillarCells)[cellOffset + 1];
    Map_CopyCellAttributes(1, 0, 1, 1, column, row);
  }

  Map_CopyCellAttributes(0, 0, 1, 1, pillarColumn, pillarRow);
}

/// Sets up the Bilibin Treasury: its pillar's passage cells, and the leader
/// arriving by BILIBIN_TREASURY_UPPER_FROM_LOWER or
/// BILIBIN_TREASURY_FROM_BILIBIN_NORTH starts 16 pixels above the floor.
static void Bilibin_SetUpTreasury(void) {
  struct FieldActor *leader;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Bilibin_UpdateTreasuryPillarCells();
  if (gGameState.entrance == BILIBIN_TREASURY_UPPER_FROM_LOWER ||
      gGameState.entrance == BILIBIN_TREASURY_FROM_BILIBIN_NORTH) {
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      Engine_EventBegin();
      leader->y.fixed = PIXELS(16);
      Engine_CameraMoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
      Engine_MapRedraw();
      Engine_EventEnd();
      Engine_TaskWait(1);
    }
  }
}

#include "common/object_findcell.h"

/// Pushing a statue in Bilibin or the Bilibin Treasury, or the crates at the
/// Bilibin Barricade: it slides one cell ahead with the leader behind it, if
/// the cell is free.
static void Event_PushPillar(void) {
  struct FieldActor *leader;
  struct FieldActor *pillar;
  u32 direction;
  struct FieldPosition destination;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  direction = leader->facing >> 12;
  pillar = SceneActor_FindActorAtCell(
      (leader->x.part.pixel + (gBilibin_FacingCellSteps[direction] >> 16)) >> 4,
      (leader->z.part.pixel + (s16)gBilibin_FacingCellSteps[direction]) >> 4);
  if (pillar != NULL) {
    pillar->terrainId = 2;
    destination.x =
        pillar->x.fixed + (gBilibin_FacingCellSteps[direction] & -0x10000);
    destination.y = pillar->y.fixed;
    destination.z =
        pillar->z.fixed + (gBilibin_FacingCellSteps[direction] << 16);
    if (FieldObject_CheckMovementCollision(pillar, &destination) <= 0) {
      // The leader leans on the pillar, then both move together.
      Object_SetMode(leader, 8);
      Engine_TaskWait(15);
      Engine_AudioPlayCue(SE_185);
      pillar->speed = PIXELS(1) / 5;
      pillar->acceleration = PIXELS(1) / 5;
      Engine_ObjectSetPosition(pillar, destination.x, destination.y,
                               destination.z);
      leader->speed = PIXELS(1) / 5;
      leader->acceleration = PIXELS(1) / 5;
      Engine_ObjectSetPosition(leader, destination.x, destination.y,
                               destination.z);
      FieldObject_WaitForMove(pillar);
      pillar->x.fixed = destination.x;
      pillar->z.fixed = destination.z;
      pillar->velocityX = 0;
      pillar->velocityZ = 0;
      Object_SetMode(leader, 1);
      // Each map derives its collision cells from this same moved actor.
      if (gGameState.scene == (s32)&SceneId_BilibinBarricade)
        Bilibin_UpdateRiverPillarCells();
      else if (gGameState.scene == (s32)&SceneId_Bilibin)
        Bilibin_UpdateVillagePillarCells();
      else if (gGameState.scene == (s32)&SceneId_BilibinTreasury)
        Bilibin_UpdateTreasuryPillarCells();
    }
  }
}

/// Runs every frame at the Bilibin Barricade: while the controlled actor is
/// west of x 142 and below 8 pixels high, an effect appears at it every thirty
/// frames.
static void Bilibin_UpdateRiverEffect(void) {
  struct FieldActor *actor;

  actor = Object_GetById(gGameState.selectedActor);
  if (actor->x.fixed < PIXELS(142)) {
    if (actor->y.fixed < PIXELS(8)) {
      if (gBilibin_RiverEffectTimer.frames == 0)
        Bilibin_SpawnRiverEffect(actor);
      if (++gBilibin_RiverEffectTimer.frames == 30)
        gBilibin_RiverEffectTimer.frames = 0;
    } else {
      gBilibin_RiverEffectTimer.frames = 0;
    }
  }
}

/// Spawns the semi-transparent effect, which frees itself after thirty frames.
static void Bilibin_SpawnRiverEffect(struct FieldActor *actor) {
  struct FieldActor *effect;
  struct AnimationObject *sprite;

  effect =
      Engine_ObjectCreate(24, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  if (effect == NULL)
    return;

  sprite = effect->sprite;
  Engine_ObjectSetScript(effect, gBilibin_TransientEffectScript);
  effect->motionFlags = 0;
  effect->terrainId = 1;
  effect->priorityFlags = 2;
  if (sprite == NULL)
    return;

  AnimationObjects_SelectAnimation(sprite, 2);
  sprite->flags = 0;
  sprite->image.objectMode = 1;
  sprite->image.priority = 3;
}

/// After the statue in the Bilibin Treasury moves (Move, a push, or entry):
/// updates the two cells it can stand on, rows 6 and 9 of its column.
static void Bilibin_UpdateTreasuryPillarCells(void) {
  struct FieldActor *pillar;

  pillar = Actor_Get(ACTOR_TREASURY_PILLAR);
  if (pillar == NULL)
    return;

  if (pillar->z.fixed >> 20 == 6)
    Map_CopyCellAttributes(2, 0, 1, 1, 14, 6);
  else
    Map_CopyCellAttributes(0, 0, 1, 1, 14, 6);

  if (pillar->z.fixed >> 20 == 9)
    Map_CopyCellAttributes(2, 0, 1, 1, 14, 9);
  else
    Map_CopyCellAttributes(1, 0, 1, 1, 14, 9);
}

s32 gBilibin_FaceLeaderScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 gBilibinEntrancesOther[] = {
    MAP_CELL_STEP(0, 65535, 168, 0, 168),
    MAP_CELL_STEP(16384, 0, 65535, 65535, 65535),
    MAP_CELL_STEPS_END,
};

struct SceneEntrance gBilibinEntrances[] = {
    {BILIBIN_FROM_WORLD_MAP, CONDITION_ALWAYS, 240, 0, 472, FACING_NORTH, 0, -1,
     -1, -1, -1, 0},
    {BILIBIN_FROM_MCCOY_PALACE_GROUNDS, CONDITION_ALWAYS, 224, 0, 88,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_FROM_INN, CONDITION_ALWAYS, 376, 0, 360, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {BILIBIN_FROM_HOUSE_EAST, CONDITION_ALWAYS, 200, 0, 328, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {BILIBIN_FROM_HOUSE_WEST, CONDITION_ALWAYS, 104, 0, 344, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {BILIBIN_FROM_HOUSE_NORTH, CONDITION_ALWAYS, 168, 0, 216, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {BILIBIN_FROM_ITEM_SHOP, CONDITION_ALWAYS, 72, 0, 216, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {BILIBIN_FROM_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 296, 0, 184,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_FROM_SANCTUM, CONDITION_ALWAYS, 400, 0, 168, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {BILIBIN_FROM_BILIBIN_TREASURY_WEST, CONDITION_ALWAYS, 40, 0, 152,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_FROM_BILIBIN_TREASURY_EAST, CONDITION_ALWAYS, 264, 0, 200,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBilibinBarricadeEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_BARRICADE_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 56, 0, 376,
     FACING_EAST, 0, 16, 16, 400, 400, 0},
    {BILIBIN_BARRICADE_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 376, 0, 184,
     FACING_WEST, 0, 16, 16, 400, 400, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBilibinTreasuryEntrances[] = {
    {0, CONDITION_ALWAYS, 232, 0, 144, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_TREASURY_FROM_BILIBIN_SOUTH, CONDITION_ALWAYS, 120, 0, 408,
     FACING_NORTH, 0, 0, 288, 240, 448, 0},
    {BILIBIN_TREASURY_LOWER_FROM_UPPER, CONDITION_ALWAYS, 120, 0, 384,
     FACING_SOUTH, 0, 0, 288, 240, 448, 0},
    {BILIBIN_TREASURY_UPPER_FROM_LOWER, CONDITION_ALWAYS, 72, 4, 128,
     FACING_NORTH, 0, 32, 32, 384, 304, 0},
    {BILIBIN_TREASURY_FROM_BILIBIN_NORTH, CONDITION_ALWAYS, 296, 4, 224,
     FACING_NORTH, 0, 32, 32, 384, 304, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s16 gBilibinTreasuryRegions[] = {
    66, 26, 128, 78, 38, 140, -1, 3,
    290, 26, 224, 302, 38, 236, -1, BILIBIN_TREASURY_TO_BILIBIN_NORTH,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gBilibinExits[] = {
    SCENE_EXITS(Bilibin),
    SCENE_EXIT(BILIBIN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_BILIBIN),
    SCENE_EXIT(BILIBIN_TO_MCCOY_PALACE_GROUNDS, McCoyPalaceGrounds, MCCOY_PALACE_GROUNDS_FROM_BILIBIN),
    SCENE_EXIT(BILIBIN_TO_INN, BilibinInteriors, BILIBIN_INTERIORS_INN),
    SCENE_EXIT(BILIBIN_TO_HOUSE_EAST, BilibinInteriors, BILIBIN_INTERIORS_HOUSE_EAST),
    SCENE_EXIT(BILIBIN_TO_HOUSE_WEST, BilibinInteriors, BILIBIN_INTERIORS_HOUSE_WEST),
    SCENE_EXIT(BILIBIN_TO_HOUSE_NORTH, BilibinInteriors, BILIBIN_INTERIORS_HOUSE_NORTH),
    SCENE_EXIT(BILIBIN_TO_ITEM_SHOP, BilibinInteriors, BILIBIN_INTERIORS_ITEM_SHOP),
    SCENE_EXIT(BILIBIN_TO_WEAPON_AND_ARMOR_SHOP, BilibinInteriors, BILIBIN_INTERIORS_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(BILIBIN_TO_SANCTUM, Sanctum, SANCTUM_FROM_BILIBIN),
    SCENE_EXIT(BILIBIN_TO_BILIBIN_TREASURY_WEST, BilibinTreasury, BILIBIN_TREASURY_FROM_BILIBIN_SOUTH),
    SCENE_EXIT(BILIBIN_TO_BILIBIN_TREASURY_EAST, BilibinTreasury, BILIBIN_TREASURY_FROM_BILIBIN_NORTH),
    SCENE_EXITS(BilibinTreasury),
    SCENE_EXIT(BILIBIN_TREASURY_TO_BILIBIN_SOUTH, Bilibin, BILIBIN_FROM_BILIBIN_TREASURY_WEST),
    SCENE_EXIT(BILIBIN_TREASURY_LOWER_TO_UPPER, BilibinTreasury, BILIBIN_TREASURY_UPPER_FROM_LOWER),
    SCENE_EXIT(BILIBIN_TREASURY_UPPER_TO_LOWER, BilibinTreasury, BILIBIN_TREASURY_LOWER_FROM_UPPER),
    SCENE_EXIT(BILIBIN_TREASURY_TO_BILIBIN_NORTH, Bilibin, BILIBIN_FROM_BILIBIN_TREASURY_EAST),
    SCENE_EXITS(BilibinBarricade),
    SCENE_EXIT(BILIBIN_BARRICADE_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_BILIBIN_BARRICADE_EAST),
    SCENE_EXIT(BILIBIN_BARRICADE_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_BILIBIN_BARRICADE_WEST),
    SCENE_EXITS_END,
};

struct ScenePlacement gBilibinPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gBilibinPlacements[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(40), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_TREE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(440), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(328), FACING_SOUTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(240), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(176), 0, PIXELS(328), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(128), 0, PIXELS(224), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(96), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(96), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(208), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(208), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(232), 0, PIXELS(296), FACING_SOUTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xf3, CONDITION_ALWAYS, 7,
        PIXELS(264), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(432), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBilibinBarricadePlacements[] = {
    { SPRITE_CRATES, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(300), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_TREE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_TREE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_TREE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(282), 0, PIXELS(170), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(170), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(262), 0, PIXELS(156), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x17, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBilibinTreasuryPlacements[] = {
    { 0xf3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(100), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { JUPITER_DJINNI, FLAG_DJINNI_0 + DJINNI_GUST, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gBilibinEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BILIBIN_TO_WORLD_MAP },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, BILIBIN_TO_MCCOY_PALACE_GROUNDS },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, BILIBIN_TO_BILIBIN_TREASURY_WEST },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, BILIBIN_TO_BILIBIN_TREASURY_EAST },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { EVENT_TALK, ACTOR_TREE_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExamineTreeMan) },
    { EVENT_TALK, ACTOR_FORMER_TREE_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutBeingATree) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_WELCOME, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWelcomeToBilibinPleaseJustIgnore) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_WELCOME, CONDITION_ALWAYS, MSGNO(MsgBilibinWelcomeToBilibinWellForgetThat) },
    { EVENT_TALK, ACTOR_VILLAGER_GROUP_TO_IMIL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIHearALargeGroupCrossed) },
    { EVENT_TALK, ACTOR_VILLAGER_GROUP_TO_IMIL, CONDITION_ALWAYS, MSGNO(MsgBilibinIfYoureTalkingAboutTheTravelers) },
    { EVENT_TALK, ACTOR_VILLAGER_ASKS_ABOUT_TREE, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_AskAboutEntranceTree) },
    { EVENT_TALK, ACTOR_VILLAGER_ASKS_ABOUT_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinYoureWonderingAboutTheTreeNear) },
    { EVENT_TALK, ACTOR_VILLAGER_MCCOY_FRIGHTENED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinLordMccoyHasBeenQuiteFrightened) },
    { EVENT_TALK, ACTOR_VILLAGER_MCCOY_FRIGHTENED, CONDITION_ALWAYS, MSGNO(MsgBilibinISawLordMccoyTheChief) },
    { EVENT_TALK, ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_AskAboutSoutheastRoute) },
    { EVENT_TALK, ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, CONDITION_ALWAYS, MSGNO(MsgBilibinIHearTheBarricadeGateThat) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_ROAD_BLOCKED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheRoadToKolimaIsBlocked) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_ROAD_BLOCKED, CONDITION_ALWAYS, MSGNO(MsgBilibinThoseThreeKidsVanishedDuringAll) },
    { EVENT_TALK, ACTOR_BOY_SCARED_OF_ENTRANCE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinEverSinceThatManTurnedInto) },
    { EVENT_TALK, ACTOR_BOY_SCARED_OF_ENTRANCE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutCurseBreakers) },
    { EVENT_TALK, ACTOR_GIRL_RED_SKY, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIsItTrueThatAMountain) },
    { EVENT_TALK, ACTOR_GIRL_RED_SKY, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoysMenSaidTheySaw) },
    { EVENT_TALK, ACTOR_STRONGMAN_IMIL_TOWER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheresATowerAtTheTip) },
    { EVENT_TALK, ACTOR_STRONGMAN_IMIL_TOWER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutImil) },
    { EVENT_TALK, ACTOR_VILLAGER_STRANGE_CREATURES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinSomethingChangedInTheEruptionNow) },
    { EVENT_TALK, ACTOR_VILLAGER_STRANGE_CREATURES, CONDITION_ALWAYS, MSGNO(MsgBilibinTheEruptionOnTheOtherSide) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREE_MAN, CONDITION_ALWAYS, MSGNO(MsgBilibinHelpMeSomebody) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORMER_TREE_MAN, CONDITION_ALWAYS, MSGNO(MsgBilibinINeverWantToBeTurned) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WELCOME, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYouGuysAreSoUnluckyArriving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WELCOME, CONDITION_ALWAYS, MSGNO(MsgBilibinNotManyLumberjacksAreWillingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GROUP_TO_IMIL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYouMustBeMadItsAlmost) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GROUP_TO_IMIL, CONDITION_ALWAYS, MSGNO(MsgBilibinOneOfTheTwoPeopleThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKS_ABOUT_TREE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeEndedUpLikeThisBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKS_ABOUT_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThatGuyMightTurnIntoA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MCCOY_FRIGHTENED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinEveryoneGetsWorriedWhenTownLeaders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MCCOY_FRIGHTENED, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoySeemsToBeHaving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWhereDidTheseKidsComeFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, CONDITION_ALWAYS, MSGNO(MsgBilibinApparentlyJustAboutAnyoneCouldJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ROAD_BLOCKED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinNoOneWouldDareGoAfter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ROAD_BLOCKED, CONDITION_ALWAYS, MSGNO(MsgBilibinTheGangOfThreeHaTheyre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_SCARED_OF_ENTRANCE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeBeggedForHelpAsHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_SCARED_OF_ENTRANCE, CONDITION_ALWAYS, MSGNO(MsgBilibinImSureLordMccoyWillGive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_RED_SKY, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWantToSeeThatPretty) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_RED_SKY, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyMustBeReportingYourDeeds) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_IMIL_TOWER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinOnlyGreatAdeptsCanEnterThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_IMIL_TOWER, CONDITION_ALWAYS, MSGNO(MsgBilibinIWantToShareWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_STRANGE_CREATURES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThatExplosionMustHaveBeenA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_STRANGE_CREATURES, CONDITION_ALWAYS, MSGNO(MsgBilibinAsLongAsImOkayI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VILLAGE_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_UpdateVillagePillarCells) },
    { TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, CONDITION_ALWAYS, 0 },
    { SEARCH(SEARCH_JAR), 100, 0xf50, FIND_ITEM(ITEM_HERB) },
    { SEARCH(SEARCH_JAR), 101, 0xf51, FIND_COINS(5) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectWell) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinBarricadeEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BILIBIN_BARRICADE_TO_WORLD_MAP_EAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, BILIBIN_BARRICADE_TO_WORLD_MAP_WEST },
    { EVENT_TALK, ACTOR_BOTTOM_TREE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectHumanShapedTree) },
    { EVENT_TALK, ACTOR_MIDDLE_TREE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectHumanShapedTree) },
    { EVENT_TALK, ACTOR_JILL_TREE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectHumanShapedTree) },
    { EVENT_TALK, ACTOR_FALLEN_TREE_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectHumanShapedTree) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOTTOM_TREE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgBilibinTheyCrossedTheRiverAfterPushing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOTTOM_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyreSoHeavy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_TREE_STAND_IN, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_TREE_STAND_IN, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIDDLE_TREE, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIDDLE_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL_TREE, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinAhhhhPleaseAnyonePleaseHelp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThankSavedMeFromBeing) },
    { EVENT_TALK, ACTOR_GANG_MEMBER_EAST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinIHeardWeWereOnceCalled) },
    { EVENT_TALK, ACTOR_GANG_MEMBER_EAST, CONDITION_ALWAYS, MSGNO(MsgBilibinItWasAwfulLuckBeingTurned) },
    { EVENT_TALK, ACTOR_GANG_MEMBER_WEST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinIfOnlyWeHadntTriedGetting) },
    { EVENT_TALK, ACTOR_GANG_MEMBER_WEST, CONDITION_ALWAYS, MSGNO(MsgBilibinIReallyLikeThisSpotBy) },
    { EVENT_TALK, ACTOR_JILL, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillThinkOfYouWhenever) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_EAST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinWeWorkedSoHardIfOnly) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_EAST, CONDITION_ALWAYS, MSGNO(MsgBilibinWhoseIdeaWasItToPick) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_WEST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinWhatACruelWorldWeJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_WEST, CONDITION_ALWAYS, MSGNO(MsgBilibinWellItWasntMyIdeaTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL, CONDITION_ALWAYS, MSGNO(MsgBilibinThesePersimmonsAreJustLikeMy) },
    { SEARCH(SEARCH_UNNAMED), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRestrictedAreaSign) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldDoorTightlyLocked)) },
    { EVENT_TOUCH, 13, FLAG_BILIBIN_JILL_GIFT_RECEIVED, EVENT_SCRIPT(Event_JillGivesHardNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_JILL_TREE, FLAG_BILIBIN_JILL_RESCUED, EVENT_SCRIPT(Event_PullJillFromRiver) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { TOUCH_WALKING, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIVER_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_UpdateRiverPillarCells) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinTreasuryEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BILIBIN_TREASURY_TO_BILIBIN_SOUTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, BILIBIN_TREASURY_LOWER_TO_UPPER },
    { SEARCH(SEARCH_UNNAMED), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadWarehouseSign) },
    { EVENT_TALK, ACTOR_GUST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_GustJoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TREASURY_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_UpdateTreasuryPillarCells) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { TOUCH_WALKING, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { SCENE_EVENTS_END },
};

u16 gBilibin_DoorSteps6[] = {
    MAP_CELL_STEP(37, 33, 4, 2, 5), MAP_CELL_STEP(33, 33, 4, 2, 5), MAP_CELL_STEPS_END,
};

u16 gBilibin_DoorSteps4[] = {
    MAP_CELL_STEP(35, 43, 2, 2, 5), MAP_CELL_STEP(33, 43, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gBilibin_DoorSteps5[] = {
    MAP_CELL_STEP(35, 52, 2, 2, 5), MAP_CELL_STEP(33, 52, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gBilibin_DoorSteps1[] = {
    MAP_CELL_STEP(35, 54, 2, 2, 5), MAP_CELL_STEP(33, 54, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gBilibin_DoorSteps0[] = {
    MAP_CELL_STEP(35, 48, 2, 2, 5), MAP_CELL_STEP(33, 48, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gBilibin_DoorSteps2[] = {
    MAP_CELL_STEP(35, 46, 2, 2, 5), MAP_CELL_STEP(33, 46, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gBilibin_DoorSteps3[] = {
    MAP_CELL_STEP(35, 50, 2, 2, 5), MAP_CELL_STEP(33, 50, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 *gBilibin_DoorSteps[] = {
    gBilibin_DoorSteps0,
    gBilibin_DoorSteps1,
    gBilibin_DoorSteps2,
    gBilibin_DoorSteps3,
    gBilibin_DoorSteps4,
    gBilibin_DoorSteps5,
    gBilibin_DoorSteps6,
};

u32 gBilibin_DoorCells[] = {
    0x140036, 0x12002b, 0x130025, 0xb0029, 0xb0023, 0x90031, 0x60037,
};

u32 gBilibin_RiverPillarCells[] = {
    0x12091208, 0x1308120a, 0x130a1309, 0x1409130b, 0x140b140a,
};
// clang-format on

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gBilibin_FacingCellSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

s32 gBilibin_TransientEffectScript[] = {
    SC_WAIT(30),
    SC_FREE,
};
