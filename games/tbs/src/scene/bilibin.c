/// @file
/// Bilibin, the Bilibin Treasury and the Bilibin Barricade, three places that
/// share one overlay, each a scene of its own: Bilibin (scene 30),
/// BilibinTreasury (scene 32) and BilibinBarricade (scene 35). The getters pick
/// each scene's tables by gGameState.scene, Bilibin_Initialize sets up the
/// scene being entered, and Event_PushPillar serves all three.
///
/// Bilibin (Bilibin) is the town past the Goma Range's cave, and the party
/// finds it afraid. A man who came to the village under a curse stands at its
/// entrance turned into a tree (TREE_MAN); his last words were that he had
/// offended the sacred tree. Lord McCoy, the chief of Bilibin, has closed the
/// road to Kolima with a barricade, and the villagers' talk sends the party to
/// him and on to Kolima. Almost everything here turns on
/// FLAG_KOLIMA_CURSE_LIFTED, whether Kolima's curse is lifted, which the party
/// sets far from here, in Kolima Forest, when it heals Tret with the Water of
/// Hermes. Until then the tree stands at the entrance: a first look says it
/// almost looks like a man, a second (place flag FLAG_PLACE_TEMP_01 remembers
/// the first until the party leaves Bilibin) that it looks like a living
/// person, and Mind Read hears `Help me... Somebody...`. Once the curse is
/// lifted, Bilibin_RestoreVillageActors puts the man himself (FORMER_TREE_MAN)
/// beside the empty spot and turns villagers 14 and 15 about. Each of the ten
/// villagers (actors 10 to 19) has two lines and two thoughts in
/// gBilibinEvents1, the row that needs the flag clear first, so the first row
/// that matches picks the time: before, they talk of the curse, the
/// barricade, McCoy's fear and the lighthouse of Imil; after, of the three
/// young warriors who broke the curse. A few ask the party a yes-or-no
/// question (the Event_AskAbout functions).
///
/// Bilibin's doors (touches 3 to 9) open before the party walks in
/// (Event_OpenDoorAndLeave): six lead into Bilibin's buildings (scene 31,
/// bilibin_interiors.c) and the seventh into the Bilibin Sanctum. Coming back
/// out of the Sanctum (entrance 9) the area name is not shown again. The other
/// exits lead to the world map, to the garden of McCoy's Palace and, two ways,
/// into the Treasury: past a bush at the west edge (actor 8, which has a
/// Whirlwind row that runs nothing), gone when the party comes back out that
/// way (entrance 10); and from the cell a statue (VILLAGE_PILLAR) stands on,
/// once Move or a push slides it aside. Bilibin_UpdateVillagePillarCells keeps
/// the cells of the statue's lane in step and hops the leader clear if it
/// lands on him; coming back out by that way (entrance 11) the party finds the
/// statue already moved. Place flag FLAG_PLACE_TEMP_04 is set while McCoy's
/// gate guards have let the party in (FLAG_MCCOY_PALACE_ADMITTED) but it has
/// not yet entered the palace (FLAG_MCCOY_PALACE_VISITED), and the garden's
/// east guard tests it to scold the party for keeping McCoy waiting. XXX: the
/// garden is another group of scenes (9, Bilibin's is 8), and
/// Scene_ResetFlagsOnEnter clears place flags on entering a new group, so the
/// guard seems never to see it set; a playthrough would settle it.
///
/// The Bilibin Treasury (BilibinTreasury) is what its sign calls `McCoy's
/// Hidden Warehouse`. Gust, a Jupiter Djinni, waits there, placed until he has
/// joined, and joins the party when spoken to (Event_GustJoins). A second
/// statue (TREASURY_PILLAR) slides along one column, and
/// Bilibin_UpdateTreasuryPillarCells keeps the two cells it can stand on in
/// step. Arriving by entrance 3 or 4 the leader starts 4 pixels above the
/// floor; two exit regions (Bilibin_GetRegions) lead to its other entrance and
/// back to Bilibin by the statue.
///
/// The Bilibin Barricade (BilibinBarricade) is the road east to Kolima where
/// McCoy's barricade stands, reached from the world map at either end. Its sign
/// reads `This area is off-limits to all those without a permit. —Lord McCoy`,
/// and a door there stays locked (`The door is tightly locked.`). At McCoy's
/// Palace the worker who built the barricade calls it shabby, a gate guard
/// thinks its hinges might break, and McCoy's captain hints that the party may
/// not need a key (mccoy_palace.c, mccoy_palace_grounds.c). A stack of crates
/// (RIVER_PILLAR) slides a cell at a time under Move or a push, and
/// Bilibin_UpdateRiverPillarCells rewrites the ten cells of its lane after
/// each move. By the river stand three trees, one on another's shoulders
/// (actors 9, 10 and 11 at one spot, 0, 32 and 48 pixels up): three children of
/// the town, the Gang of Three, turned into trees while they climbed to pick
/// persimmons. Talking to them says only that they look like a person; the
/// bottom one thinks `They're... so heavy...`.
///
/// When the party passes again after Saturos is beaten at Mercury Lighthouse
/// (FLAG_SATUROS_BEATEN_AT_MERCURY), with the curse still in place, the
/// travelers from Imil have crossed the river by pushing the trees down (the
/// bottom tree thinks `They crossed the river after pushing us down.`):
/// Bilibin_RestoreRiverActors lays tree 10 on its side by the river, with an
/// invisible actor (16) that answers for it, and puts the top tree in the
/// water. That tree is Jill: Mind Read hears her cry for help, and her friends
/// fear she will be swept downstream. Move on her (Event_PullJillFromRiver)
/// works once she is in cell column 6, by the bank: the leader pulls her
/// ashore in three tugs, she thanks the party, and FLAG_BILIBIN_JILL_RESCUED
/// keeps her on the bank. While whoever the player controls is west of x 142
/// and under 8 pixels up, Bilibin_UpdateRiverEffect leaves an effect at their
/// feet every 30 frames.
///
/// Once the curse is lifted the trees are gone, and the Gang of Three are
/// children again on the bank (GANG_MEMBER_WEST, GANG_MEMBER_EAST and JILL).
/// If Jill was never pulled out she is missing, and her friends mourn that
/// they are only the Gang of Two. If she was, stepping up to them (touch 13)
/// plays Event_JillGivesHardNut: her friends recognize the leader, Jill
/// reminds him she was that tree and gives him a Hard Nut
/// (FLAG_BILIBIN_JILL_GIFT_RECEIVED), and from then on the three turn to
/// watch him.

#include "game_state.h"
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

/// Actors of Bilibin (Bilibin), numbered from its placement rows.
enum {
  TREE_MAN = 9,        ///< the man turned into a tree at the village entrance
  VILLAGE_PILLAR = 20, ///< a statue, moved with Move or pushed
  FORMER_TREE_MAN = 21 ///< the same man once the curse is lifted
};

/// Actors of the Bilibin Barricade (BilibinBarricade).
enum {
  RIVER_PILLAR = 8, ///< a stack of crates, moved with Move or pushed
  JILL_TREE = 11,   ///< Jill as a tree, the top of the three
  /// Jill's two friends, once the curse is lifted. With her they were the
  /// "Gang of Three".
  GANG_MEMBER_WEST = 12,
  GANG_MEMBER_EAST = 13,
  JILL = 14 ///< Jill herself, once the curse is lifted
};

/// Actors of the Bilibin Treasury (BilibinTreasury).
enum {
  TREASURY_PILLAR = 8, ///< a statue, moved with Move or pushed
  GUST = 9
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
    return gBilibinEntrances1;
  }
  if (scene == (s32)&SceneId_BilibinBarricade) {
    return gBilibinEntrances3;
  }
  if (scene == (s32)&SceneId_BilibinTreasury) {
    return gBilibinEntrances2;
  }
  return (const struct SceneEntrance *)gBilibinEntrancesOther;
}

/// Only the Bilibin Treasury has exit regions.
const struct SceneRegion *Bilibin_GetRegions(void) {
  if (gGameState.scene == (s32)&SceneId_BilibinTreasury) {
    return (const struct SceneRegion *)gBilibinRegions2;
  }
  return 0;
}

/// Returns the packed exit destinations shared by the three maps.
const u32 *Bilibin_GetExits(void) { return gBilibinExits; }

/// Talking to Gust in the Bilibin Treasury: Gust, a Jupiter Djinni, joins the
/// party.
static void Event_GustJoins(void) { Djinn_JoinParty(GUST, ELEMENT_JUPITER, 0); }

/// Selects the actors of Bilibin, the Bilibin Barricade or the Bilibin
/// Treasury.
const struct ScenePlacement *Bilibin_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Bilibin) {
    return gBilibinPlacements1;
  }
  if (scene == (s32)&SceneId_BilibinBarricade) {
    return gBilibinPlacements3;
  }
  if (scene == (s32)&SceneId_BilibinTreasury) {
    return gBilibinPlacements2;
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
    return gBilibinEvents1;
  }
  if (scene == (s32)&SceneId_BilibinBarricade) {
    return gBilibinEvents3;
  }
  if (scene == (s32)&SceneId_BilibinTreasury) {
    return gBilibinEvents2;
  }
  return gBilibinEventsOther;
}

/// Searching the sign at the Bilibin Barricade shows Lord McCoy's notice that
/// no one may pass without a permit.
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

/// Talking to one of the Gang of Three's trees at the Bilibin Barricade,
/// standing or pushed down, or to the actor that answers for the fallen one.
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
  Event_ShowMessage(TREE_MAN, 0);
  GameFlag_Set(FLAG_PLACE_TEMP_01);
  Engine_EventEnd();
}

/// A villager in Bilibin, before the curse is lifted, asks whether the party
/// saw the tree at the entrance; on yes, that it was once a man who came under
/// a curse.
static void Event_AskAboutEntranceTree(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinDidSeeTreeAtEntrance);
  Engine_EventAskYesNo(12, 0);
  Engine_EventEnd();
}

/// A villager in Bilibin, before the curse is lifted, asks whether the party
/// has tried heading southeast; either way, the barricade blocks the road.
static void Event_AskAboutSoutheastRoute(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinHaveTriedHeadingSoutheastFrom);
  Engine_EventAskYesNo(14, 0);
  Engine_EventEnd();
}

/// In Bilibin once the curse is lifted, the man who was the tree at the
/// entrance asks whether the party can believe he was one.
static void Event_AskAboutBeingATree(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinWasTurnedIntoTreeFor);
  Engine_EventAskYesNo(FORMER_TREE_MAN, 0);
  Engine_EventEnd();
}

/// A villager in Bilibin, once the curse is lifted, asks whether the party are
/// the three young warriors who broke it.
static void Event_AskAboutCurseBreakers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinCurseWasBrokenThanksEfforts);
  Engine_EventAskYesNo(16, 0);
  Engine_EventEnd();
}

/// A villager in Bilibin, once the curse is lifted, asks whether the party has
/// been to Imil, up north, and seen its lighthouse.
static void Event_AskAboutImil(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinHaveEverBeenVillageImil);
  Engine_EventAskYesNo(18, 0);
  Engine_EventEnd();
}

/// Walking into one of Bilibin's doors: everyone stops, the door opens and the
/// leader walks through it. Doors 3 to 8 lead into Bilibin's buildings, door 9
/// into the Bilibin Sanctum, whose door sounds and is stepped through
/// differently.
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
  if (exitIndex == 6) {
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
  Object_GetById(0)->motionFlags = 0;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (exitIndex == 6) {
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
  } else {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  }
  if (exitIndex == 4) {
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  } else {
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  }
  Engine_EventWait(16);
  Engine_EventRequestExit(exitIndex + 3);
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
  jillTree = Object_GetById(JILL_TREE);
  if ((jillTree->x.fixed >> 20) == 6) {
    Engine_EventBegin();
    Engine_ActorSetSpritePriority(JILL_TREE, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
    Actor_SetSpeed(JILL_TREE, PIXELS(1) / 5, PIXELS(1) / 10);
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    jillTree->motionFlags = 0;
    // Each tug: the leader faces about, strains and draws her in, then steps
    // on.
    leader->scaleX = -0x10000;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_MoveToAndWait(JILL_TREE, 111, 196);
    leader->scaleX = 0x10000;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 185);
    Engine_EventWait(20);
    leader->scaleX = -0x10000;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_MoveToAndWait(JILL_TREE, 121, 190);
    leader->scaleX = 0x10000;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 141, 189);
    Engine_EventWait(20);
    leader->scaleX = -0x10000;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_MoveToAndWait(JILL_TREE, 132, 186);
    leader->scaleX = 0x10000;
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 166, 185);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    Engine_ActorSetSpritePriority(JILL_TREE, 2);
    RunSceneTransitionEffect(0, JILL_TREE);
    Engine_TaskWait(10);
    Engine_EventSetMessage(MsgBilibinThankSavedMeFromBeing);
    Event_ShowMessage(JILL_TREE, 0);
    Engine_PsynergyCancel();
    Engine_TaskWait(10);
    GameFlag_Set(FLAG_BILIBIN_JILL_RESCUED);
    Engine_EventEnd();
  }
}

/// Walking into the cells beside the statue in the Bilibin Treasury or the
/// crates at the Bilibin Barricade: checks the blocked direction before
/// allowing another movement step.
static void Event_CheckAhead(void) { Engine_LeaderCheckAhead(); }

/// Stepping up to Jill and her friends on the bank at the Bilibin Barricade,
/// once the curse is lifted and if she was pulled from the river: they
/// recognize the leader, and she gives him a Hard Nut in thanks.
/// "Remember me? I was that tree!"
static void Event_JillGivesHardNut(void) {
  struct FieldActor *leader;
  struct FieldActor *westMember;

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
  westMember = Actor_Get(GANG_MEMBER_WEST);
  if (westMember->x.fixed > leader->x.fixed) {
    Actor_FaceDirection(GANG_MEMBER_EAST, FACING_SOUTH + FACING_STEP, 20);
    Actor_ShowEmote(GANG_MEMBER_EAST, EMOTE_IN_FRONT, 20);
    Engine_EventSetMessage(MsgBilibinYoureGuy);
    Event_ShowMessageAndWait(GANG_MEMBER_EAST, 0, 10);
    Actor_ShowEmote(GANG_MEMBER_WEST, EMOTE_IN_FRONT, 0);
  } else {
    Actor_FaceDirection(GANG_MEMBER_WEST, FACING_SOUTHEAST + FACING_STEP, 20);
    Actor_ShowEmote(GANG_MEMBER_WEST, EMOTE_IN_FRONT, 20);
    Engine_EventSetMessage(MsgBilibinYoureGuy);
    Event_ShowMessageAndWait(GANG_MEMBER_WEST, 0, 10);
    Actor_ShowEmote(GANG_MEMBER_EAST, EMOTE_IN_FRONT, 0);
  }
  Actor_ShowEmote(JILL, EMOTE_IN_FRONT, 0);
  Actor_FaceDirection(JILL, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(GANG_MEMBER_WEST, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GANG_MEMBER_EAST, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 268, 184);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(GANG_MEMBER_EAST, 2);
  Event_ShowMessageAndWait(GANG_MEMBER_EAST, 0, 10);
  Actor_FaceDirection(GANG_MEMBER_EAST, FACING_EAST, 0);
  Actor_FaceDirection(JILL, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(GANG_MEMBER_WEST, FACING_WEST, 20);
  Engine_ActorSetAnimationAndWait(GANG_MEMBER_WEST, ANIM_NOD);
  Actor_SetAttachedEffect(JILL, 0x102);
  Engine_EventWait(40);
  Actor_FaceDirection(JILL, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(GANG_MEMBER_WEST, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GANG_MEMBER_EAST, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(JILL, 1);
  Event_ShowMessageAndWait(JILL, 0, 10);
  Engine_ActorSetAnimation(GANG_MEMBER_WEST, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GANG_MEMBER_EAST, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(JILL, 0);
  Actor_SetSpeed(JILL, PIXELS(3) / 5, PIXELS(3) / 10);
  Object_GetById(JILL)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(JILL, 266, 172);
  Engine_EventWait(1);
  Object_GetById(JILL)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(JILL, ANIM_NOD);
  Event_ShowMessageAndWait(JILL, 0, 10);
  // Show the named gift before adding it to the party's inventory.
  Engine_MessageShowCentered(MsgBilibinJillGaveIsaacSpecialGift, 1);
  gEventWork->message++;
  Engine_ItemShowFound(ITEM_HARD_NUT, 3);
  Engine_PartyGiveItem(ITEM_HARD_NUT, 0);
  Engine_ActorSetAnimationAndWait(JILL, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_SetSpeed(JILL, PIXELS(1), PIXELS(1) / 2);
  Object_GetById(JILL)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(JILL, 262, 156);
  Engine_EventWait(1);
  {
    struct FieldActor *jill = Actor_Get(JILL);
    u8 steeringFlags = jill->steeringFlags | ACTOR_FACES_MOTION;

    jill->steeringFlags = steeringFlags;
  }
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GANG_MEMBER_WEST, 2);
  Event_ShowMessageAndWait(GANG_MEMBER_WEST, 0, 10);
  Engine_ActorSetAnimation(GANG_MEMBER_WEST, ANIM_NOD);
  Engine_ActorSetAnimation(GANG_MEMBER_EAST, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JILL, ANIM_NOD);
  // From now on the three turn to face the leader wherever he goes.
  FieldObject_SetTargetAndCallback(GANG_MEMBER_WEST,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gBilibin_FaceLeaderScript);
  FieldObject_SetTargetAndCallback(GANG_MEMBER_EAST,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gBilibin_FaceLeaderScript);
  FieldObject_SetTargetAndCallback(JILL,
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
    Engine_ActorSetPosition(TREE_MAN, 0, 0);
    Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(15, FACING_SOUTH + FACING_STEP, 0);
  } else {
    actor = Actor_Get(TREE_MAN);
    Engine_ActorSetSpriteFlags(actor, 0);
    Actor_SetPosition(FORMER_TREE_MAN, 0, 0);
  }
  // Apply arrival-specific changes after restoring the population.
  actor = Actor_Get(8);
  actor->scaleY = 0x18000;
  entrance = gGameState.entrance;
  if (entrance == 10) {
    Actor_SetPosition(8, 0, 0);
  } else if (entrance == 9) {
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  }
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    entrance = gGameState.entrance;
    if (entrance == 11) {
      Actor_SetPosition(VILLAGE_PILLAR, PIXELS(248), PIXELS(216));
    }
  }
  Bilibin_UpdateVillagePillarCells();
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_ADMITTED) != 0 &&
      GameFlag_IsSet(FLAG_MCCOY_PALACE_VISITED) == 0) {
    GameFlag_Set(FLAG_PLACE_TEMP_04);
  }
}

/// After the statue in Bilibin moves (Move, a push, or entry): updates the
/// collision cells of its lane, opens its home cell (16, 13) once it has left
/// it, and moves the leader clear if it has landed on him.
static void Bilibin_UpdateVillagePillarCells(void) {
  s32 leaderRow;
  s32 leaderColumn;
  struct FieldActor *leader;
  struct FieldActor *pillar;
  s32 pillarX;
  s32 pillarRow;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  pillar = Object_GetById(VILLAGE_PILLAR);
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
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 20);
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

  fallenTree = Object_GetById(10);
  jillTree = Object_GetById(JILL_TREE);
  actor = Actor_Get(RIVER_PILLAR);
  Engine_ActorSetSpriteFlags(actor, 0);
  populationState = GameFlag_IsSet(FLAG_KOLIMA_CURSE_LIFTED);
  if (populationState != 0) {
    // The curse is lifted: the trees are gone.
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(JILL_TREE, 0, 0);
    Map_CopyCellsTo(56, 15, 40, 15, 1, 2);
    Map_CopyCellAttributes(26, 15, 1, 3, 10, 15);
    if (GameFlag_IsSet(FLAG_BILIBIN_JILL_GIFT_RECEIVED) == 0) {
      if (GameFlag_IsSet(FLAG_BILIBIN_JILL_RESCUED) == 0) {
        Engine_ActorSetPosition(JILL, 0, 0);
        Actor_FaceDirection(GANG_MEMBER_WEST, FACING_NORTH + FACING_STEP, 0);
        Actor_FaceDirection(GANG_MEMBER_EAST, FACING_NORTHWEST + FACING_STEP,
                            0);
      }
    } else {
      Actor_FaceDirection(GANG_MEMBER_WEST, FACING_NORTH + FACING_STEP, 0);
      Actor_FaceDirection(GANG_MEMBER_EAST, FACING_NORTHWEST + FACING_STEP, 0);
    }
  } else {
    Actor_SetPosition(GANG_MEMBER_WEST, 0, 0);
    Actor_SetPosition(GANG_MEMBER_EAST, 0, 0);
    Actor_SetPosition(JILL, 0, 0);
    actor = Actor_Get(9);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor = Actor_Get(10);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor = Actor_Get(JILL_TREE);
    Engine_ActorSetSpriteFlags(actor, 0);
    fallenTree->motionFlags = populationState;
    // Once FLAG_SATUROS_BEATEN_AT_MERCURY is set ("They crossed the river
    // after pushing us down"), tree 10 lies on its side and Jill is in the
    // water, or on the bank where she was pulled ashore.
    crossingState = GameFlag_IsSet(FLAG_SATUROS_BEATEN_AT_MERCURY);
    if (crossingState != 0) {
      Object_GetById(9)->collisionFlags |= 16;
      Object_GetById(16)->collisionFlags |= 16;
      Object_GetById(JILL_TREE)->collisionFlags |= 16;
      Actor_SetPosition(16, PIXELS(142), PIXELS(156));
      actor = Actor_Get(16);
      Engine_ActorSetSpriteFlags(actor, 0);
      Actor_SetPosition(10, PIXELS(142), PIXELS(156));
      fallenTree->sprite->rotation = 0x4000;
      fallenTree->y.fixed += -0x80000;
      if (GameFlag_IsSet(FLAG_BILIBIN_JILL_RESCUED) != 0) {
        Actor_SetPosition(JILL_TREE, PIXELS(132), PIXELS(186));
      } else {
        Actor_SetPosition(JILL_TREE, PIXELS(88), PIXELS(196));
        Engine_ActorSetSpritePriority(JILL_TREE, 3);
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

  actor = Object_GetById(RIVER_PILLAR);
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
/// arriving by entrance 3 or 4 starts 16 pixels above the floor.
static void Bilibin_SetUpTreasury(void) {
  struct FieldActor *leader;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Bilibin_UpdateTreasuryPillarCells();
  if (gGameState.entrance == 3 || gGameState.entrance == 4) {
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

  pillar = Actor_Get(TREASURY_PILLAR);
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

struct SceneEntrance gBilibinEntrances1[] = {
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

struct SceneEntrance gBilibinEntrances3[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_BARRICADE_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 56, 0, 376,
     FACING_EAST, 0, 16, 16, 400, 400, 0},
    {BILIBIN_BARRICADE_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 376, 0, 184,
     FACING_WEST, 0, 16, 16, 400, 400, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBilibinEntrances2[] = {
    {0, CONDITION_ALWAYS, 232, 0, 144, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_TREASURY_FROM_BILIBIN_SOUTH, CONDITION_ALWAYS, 120, 0, 408,
     FACING_NORTH, 0, 0, 288, 240, 448, 0},
    {2, CONDITION_ALWAYS, 120, 0, 384, FACING_SOUTH, 0, 0, 288, 240, 448, 0},
    {3, CONDITION_ALWAYS, 72, 4, 128, FACING_NORTH, 0, 32, 32, 384, 304, 0},
    {BILIBIN_TREASURY_FROM_BILIBIN_NORTH, CONDITION_ALWAYS, 296, 4, 224,
     FACING_NORTH, 0, 32, 32, 384, 304, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s16 gBilibinRegions2[] = {
    66, 26, 128, 78, 38, 140, -1, 3,
    290, 26, 224, 302, 38, 236, -1, 4,
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
    SCENE_EXIT(2, BilibinTreasury, 3),
    SCENE_EXIT(3, BilibinTreasury, 2),
    SCENE_EXIT(BILIBIN_TREASURY_TO_BILIBIN_NORTH, Bilibin, BILIBIN_FROM_BILIBIN_TREASURY_EAST),
    SCENE_EXITS(BilibinBarricade),
    SCENE_EXIT(BILIBIN_BARRICADE_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_BILIBIN_BARRICADE_EAST),
    SCENE_EXIT(BILIBIN_BARRICADE_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_BILIBIN_BARRICADE_WEST),
    SCENE_EXITS_END,
};

struct ScenePlacement gBilibinPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gBilibinPlacements1[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(40), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x69, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(440), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(328), FACING_SOUTHWEST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(240), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(176), 0, PIXELS(328), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(128), 0, PIXELS(224), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(96), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(96), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(208), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(208), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(232), 0, PIXELS(296), FACING_SOUTHWEST, TALK_FACE_PARTY, 0 },
    { 0x87, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xf3, CONDITION_ALWAYS, 7,
        PIXELS(264), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(432), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBilibinPlacements3[] = {
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(300), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x69, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x69, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x69, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(282), 0, PIXELS(170), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(170), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(262), 0, PIXELS(156), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x17, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBilibinPlacements2[] = {
    { 0xf3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(100), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x5d, FLAG_DJINNI_0 + ELEMENT_JUPITER * 20, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gBilibinEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndLeave) },
    { EVENT_TALK, TREE_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ExamineTreeMan) },
    { EVENT_TALK, FORMER_TREE_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutBeingATree) },
    { EVENT_TALK, 10, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWelcomeToBilibinPleaseJustIgnore) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBilibinWelcomeToBilibinWellForgetThat) },
    { EVENT_TALK, 11, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIHearALargeGroupCrossed) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBilibinIfYoureTalkingAboutTheTravelers) },
    { EVENT_TALK, 12, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_AskAboutEntranceTree) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBilibinYoureWonderingAboutTheTreeNear) },
    { EVENT_TALK, 13, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinLordMccoyHasBeenQuiteFrightened) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBilibinISawLordMccoyTheChief) },
    { EVENT_TALK, 14, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_AskAboutSoutheastRoute) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBilibinIHearTheBarricadeGateThat) },
    { EVENT_TALK, 15, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheRoadToKolimaIsBlocked) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgBilibinThoseThreeKidsVanishedDuringAll) },
    { EVENT_TALK, 16, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinEverSinceThatManTurnedInto) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutCurseBreakers) },
    { EVENT_TALK, 17, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIsItTrueThatAMountain) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoysMenSaidTheySaw) },
    { EVENT_TALK, 18, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheresATowerAtTheTip) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutImil) },
    { EVENT_TALK, 19, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinSomethingChangedInTheEruptionNow) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgBilibinTheEruptionOnTheOtherSide) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TREE_MAN, CONDITION_ALWAYS, MSGNO(MsgBilibinHelpMeSomebody) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), FORMER_TREE_MAN, CONDITION_ALWAYS, MSGNO(MsgBilibinINeverWantToBeTurned) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYouGuysAreSoUnluckyArriving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBilibinNotManyLumberjacksAreWillingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYouMustBeMadItsAlmost) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBilibinOneOfTheTwoPeopleThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeEndedUpLikeThisBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBilibinThatGuyMightTurnIntoA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinEveryoneGetsWorriedWhenTownLeaders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoySeemsToBeHaving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWhereDidTheseKidsComeFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgBilibinApparentlyJustAboutAnyoneCouldJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinNoOneWouldDareGoAfter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgBilibinTheGangOfThreeHaTheyre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeBeggedForHelpAsHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgBilibinImSureLordMccoyWillGive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWantToSeeThatPretty) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyMustBeReportingYourDeeds) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinOnlyGreatAdeptsCanEnterThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgBilibinIWantToShareWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThatExplosionMustHaveBeenA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgBilibinAsLongAsImOkayI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), VILLAGE_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_UpdateVillagePillarCells) },
    { TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, 0 },
    { SEARCH(SEARCH_JAR), 100, 0xf50, FIND_ITEM(ITEM_HERB) },
    { SEARCH(SEARCH_JAR), 101, 0xf51, FIND_COINS(5) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectWell) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectHumanShapedTree) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectHumanShapedTree) },
    { EVENT_TALK, JILL_TREE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectHumanShapedTree) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectHumanShapedTree) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgBilibinTheyCrossedTheRiverAfterPushing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyreSoHeavy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), JILL_TREE, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinAhhhhPleaseAnyonePleaseHelp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), JILL_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThankSavedMeFromBeing) },
    { EVENT_TALK, GANG_MEMBER_WEST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinIHeardWeWereOnceCalled) },
    { EVENT_TALK, GANG_MEMBER_WEST, CONDITION_ALWAYS, MSGNO(MsgBilibinItWasAwfulLuckBeingTurned) },
    { EVENT_TALK, GANG_MEMBER_EAST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinIfOnlyWeHadntTriedGetting) },
    { EVENT_TALK, GANG_MEMBER_EAST, CONDITION_ALWAYS, MSGNO(MsgBilibinIReallyLikeThisSpotBy) },
    { EVENT_TALK, JILL, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillThinkOfYouWhenever) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GANG_MEMBER_WEST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinWeWorkedSoHardIfOnly) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GANG_MEMBER_WEST, CONDITION_ALWAYS, MSGNO(MsgBilibinWhoseIdeaWasItToPick) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GANG_MEMBER_EAST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinWhatACruelWorldWeJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GANG_MEMBER_EAST, CONDITION_ALWAYS, MSGNO(MsgBilibinWellItWasntMyIdeaTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), JILL, CONDITION_ALWAYS, MSGNO(MsgBilibinThesePersimmonsAreJustLikeMy) },
    { SEARCH(SEARCH_UNNAMED), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRestrictedAreaSign) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldDoorTightlyLocked)) },
    { EVENT_TOUCH, 13, FLAG_BILIBIN_JILL_GIFT_RECEIVED, EVENT_SCRIPT(Event_JillGivesHardNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), JILL_TREE, FLAG_BILIBIN_JILL_RESCUED, EVENT_SCRIPT(Event_PullJillFromRiver) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { TOUCH_WALKING, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), RIVER_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_UpdateRiverPillarCells) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { SEARCH(SEARCH_UNNAMED), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadWarehouseSign) },
    { EVENT_TALK, GUST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_GustJoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), TREASURY_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_UpdateTreasuryPillarCells) },
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
