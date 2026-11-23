/// @file
/// Bilibin, the Bilibin Treasury and the Bilibin Barricade, three scenes that
/// share one overlay: SceneId_Bilibin, SceneId_BilibinTreasury and
/// SceneId_BilibinBarricade. The getters pick each scene's tables by
/// gMainRoutineWork.next, and Scene_Initialize sets up the scene being
/// entered. Bilibin_PushStatueOrCrates serves all three.
///
/// Bilibin (SceneId_Bilibin) is the town past Goma Cave, whose chief, Lord
/// McCoy, has closed the road to Kolima. Almost everything here turns on
/// FLAG_KOLIMA_CURSE_LIFTED, which the party sets in Kolima Forest when it
/// heals Tret.
///
/// Until then a man turned into a tree (ACTOR_TREE_MAN) stands at the entrance.
/// A first look says it almost looks like a man, a second that it looks like a
/// living person. FLAG_BILIBIN_TREE_EXAMINED remembers the first look until the
/// party leaves Bilibin. Mind Read hears `Help me... Somebody...`.
///
/// Once the curse is lifted, Bilibin_RestoreVillageActors puts the man himself
/// (ACTOR_FORMER_TREE_MAN) beside the empty spot and turns
/// ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST and ACTOR_OLD_WOMAN_ROAD_BLOCKED about.
///
/// Each of the ten villagers (ACTOR_YOUNG_MAN_WELCOME to
/// ACTOR_VILLAGER_STRANGE_CREATURES) has two lines and two thoughts in
/// gBilibinTriggers. The row that needs the flag clear comes first, so the
/// first row that matches picks the time. A few ask a yes-or-no question (the
/// Bilibin_AskAbout functions).
///
/// Walking up to a door in Bilibin opens it before the party walks in
/// (Bilibin_OpenDoorAndLeave). Six lead into Bilibin's buildings
/// (SceneId_CottageRoomsC): the inn in the south-east (cell (23, 22)), the
/// houses east (12, 20), west (6, 21) and north (10, 13), the item shop
/// (4, 13) and the weapon and armor shop (18, 11). The seventh is the Sanctum's
/// double door at the top right (24..25, 10). Each door's cell has the number
/// of the exit it takes as its behaviour. Coming out of the Sanctum
/// (BILIBIN_FROM_SANCTUM) the area name is not shown again.
///
/// The other exits lead to the World Map, to the garden of McCoy's Palace and,
/// two ways, into the Treasury, the cave where Gust hides.
///
/// One way is the cave mouth at the west edge, behind a bush of ivy
/// (ACTOR_IVY). The walkthrough reaches it from the Sanctum over the pillar
/// tops and blows the ivy away with Whirlwind. The ivy's Whirlwind row runs no
/// event of its own, so nothing remembers that, and the ivy is back on the next
/// visit unless the party has just come out of that cave
/// (BILIBIN_FROM_BILIBIN_TREASURY_WEST).
///
/// The other way is a hole under the golden statue (ACTOR_VILLAGE_STATUE) in
/// front of the weapon and armor shop, at cell (16, 13). Pushed or moved off
/// it, the statue uncovers the hole, which drops into the Treasury's upper
/// room. Coming back out that way (BILIBIN_FROM_BILIBIN_TREASURY_EAST) the
/// party finds the statue one cell west of the hole.
///
/// FLAG_BILIBIN_KEEPING_MCCOY_WAITING is set while McCoy's gate guards have let
/// the party in (FLAG_MCCOY_PALACE_ADMITTED) but it has not yet entered the
/// palace (FLAG_MCCOY_PALACE_VISITED). It is there for the garden's east guard
/// to scold the party for keeping McCoy waiting.
///
/// The garden is another area (scenes.s gives Bilibin 8 and the garden 9), so
/// Scene_Arrive clears the flag on the way in and the guard never sees it set.
///
/// The Bilibin Treasury (SceneId_BilibinTreasury) is what its sign calls
/// `McCoy's Hidden Warehouse`. Gust, a Jupiter Djinni, is placed there until he
/// joins the party when spoken to (Bilibin_GustJoins).
///
/// In its upper room a second golden statue (ACTOR_TREASURY_STATUE) stands
/// in an alcove at the top of a narrow walkway between two pools, and Gust
/// waits on a ledge to the east. The statue slides up and down the walkway
/// under Move or a push. Where it is not, its alcove lets Isaac hop east or
/// west across the pools, and the foot of the walkway lets him hop south off it
/// (Bilibin_UpdateTreasuryStatueCells).
///
/// Arriving by BILIBIN_TREASURY_UPPER_FROM_LOWER or
/// BILIBIN_TREASURY_FROM_BILIBIN_NORTH the leader starts 16 pixels above the
/// floor. Two exit regions (Scene_GetExitRegions) lead down its west ladder to
/// the lower room and back to Bilibin by the statue.
///
/// The Bilibin Barricade (SceneId_BilibinBarricade) is the road east to Kolima
/// where McCoy's barricade stands, reached from the World Map at either end.
/// Its sign reads `This area is off-limits to all those without a permit.
/// —Lord McCoy`, and a door there stays locked.
///
/// The barricade is a wall of wooden crates. The stack of two crates at its
/// left end (ACTOR_BARRICADE_CRATES) slides out under Move or a push, which
/// opens the way through to Kolima, as the walkthrough has it
/// (Bilibin_UpdateCratesCells).
///
/// By the river stand three trees, one on another's shoulders. They are
/// ACTOR_BOTTOM_TREE, ACTOR_MIDDLE_TREE and ACTOR_JILL_TREE at one spot, 0, 32
/// and 48 pixels up. They are the Gang of Three, children of the town turned
/// into trees by the curse.
///
/// Once Saturos is beaten at Mercury Lighthouse
/// (FLAG_SATUROS_BEATEN_AT_MERCURY), while the curse holds, the travelers from
/// Imil have crossed the river by pushing the trees down.
/// Bilibin_RestoreRiverActors lays the middle tree on its side, with an
/// invisible actor (ACTOR_FALLEN_TREE_STAND_IN) that answers for it, and puts
/// Jill's tree in the water.
///
/// Standing in the shallows below the fallen tree, Isaac can use Move on her
/// (Bilibin_PullJillFromRiver). Once Move has brought her one cell east (cell
/// column 6), he pulls her ashore in three tugs, and
/// FLAG_BILIBIN_JILL_RESCUED keeps her there.
///
/// While whoever the player controls wades in the river,
/// Bilibin_UpdateWadingRipples spreads a ripple from their feet every 30
/// frames.
///
/// Once the curse is lifted the trees are gone, and the Gang of Three are
/// children again on the bank (ACTOR_GANG_MEMBER_EAST, ACTOR_GANG_MEMBER_WEST
/// and ACTOR_JILL). If Jill was never pulled out she is missing.
///
/// If she was, walking up beside the three (cells (13, 10..12) to their west
/// and (18, 10..12) to their east) plays Bilibin_JillGivesHardNut: Jill thanks
/// Isaac and gives him a Hard Nut
/// (FLAG_BILIBIN_JILL_GIFT_RECEIVED). From then on the three turn to watch him.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern s32 gBilibin_FaceLeaderScript[];
extern struct SceneEntrance gBilibinEntrancesOther[];
extern struct SceneEntrance gBilibinEntrances[];
extern struct SceneEntrance gBilibinBarricadeEntrances[];
extern struct SceneEntrance gBilibinTreasuryEntrances[];
extern s16 gBilibinTreasuryRegions[];
extern u32 gBilibinExits[];
extern struct ActorPlacement gBilibinActorPlacementsOther[];
extern struct ActorPlacement gBilibinActorPlacements[];
extern struct ActorPlacement gBilibinBarricadeActorPlacements[];
extern struct ActorPlacement gBilibinTreasuryActorPlacements[];
extern struct Trigger gBilibinTriggersOther[];
extern struct Trigger gBilibinTriggers[];
extern struct Trigger gBilibinBarricadeTriggers[];
extern struct Trigger gBilibinTreasuryTriggers[];
extern u16 *gBilibin_DoorSteps[];
/// Each door's cell, as column and row.
extern s16 gBilibin_DoorCells[][2];
/// The cells round the crates' place that push them, as column and row pairs.
extern u8 gBilibin_CratesPushCells[];
extern s32 gPushedActor_DirectionSteps[];
extern s32 gBilibin_RippleScript[];

static void Bilibin_RestoreVillageActors(void);
/// Move on the golden statue in front of Bilibin's weapon and armor shop slides
/// it a cell. Moved off cell (16, 13), it uncovers the hole into the Treasury;
/// moved back on, it covers it again.
///
/// The same work follows every push and every arrival in Bilibin. The cells of
/// the statue's dotted square, (15..17, 12..14), are made ground that pushes
/// the statue again, the statue's own cell is walled off, and the hole's cell
/// is given its exit unless the statue stands on it. If the statue lands on
/// the hole while Isaac is standing on it, he starts with a "!" and hops clear,
/// to the north or the east.
static void Bilibin_UpdateVillageStatueCells(void);
static void Bilibin_RestoreRiverActors(void);
/// Move on the stack of two crates at the left end of the Bilibin Barricade
/// drags it out of the wall of crates, opening a gap through the barricade to
/// the road to Kolima.
///
/// The same work follows every push and every arrival at the barricade: the
/// ten cells round the crate's place, listed in gBilibin_CratesPushCells, are
/// made ground that pushes the crate, and the crate's own cell is walled off.
static void Bilibin_UpdateCratesCells(void);
static void Bilibin_SetUpTreasury(void);
static void Bilibin_UpdateWadingRipples(void);
static void Bilibin_SpawnRipple(struct Actor *actor);
/// Move on the golden statue in the Treasury slides it up or down the walkway
/// between the two pools, where Gust waits on the ledge to the east.
///
/// The statue rests either in its alcove at the top, cell (14, 6), or at the
/// walkway's foot, cell (14, 9). The same work follows every push and every
/// arrival: the cell the statue stands on is walled off, the alcove when empty
/// lets Isaac hop east or west across the pools, and the foot when empty lets
/// him push the statue back up or hop south off the walkway.
static void Bilibin_UpdateTreasuryStatueCells(void);

/// Actors of Bilibin, numbering its actor placements from
/// ACTOR_FIRST_PLACED. The ten villagers are named by what they say before
/// the curse is lifted.
enum BilibinActorId {
  /// The bush of ivy over the cave mouth at the west edge, the way into the
  /// Treasury, drawn half as tall again. Whirlwind blows it away.
  ACTOR_IVY = ACTOR_FIRST_PLACED,
  /// The man turned into a tree at the village entrance, drawn as a tree.
  ACTOR_TREE_MAN,
  /// A young man with brown hair.
  ACTOR_YOUNG_MAN_WELCOME,
  /// A woman with fair hair.
  ACTOR_VILLAGER_GROUP_TO_IMIL,
  /// A burly grey-haired man (Bilibin_AskAboutEntranceTree).
  ACTOR_VILLAGER_ASKS_ABOUT_TREE,
  /// A woman in a green dress.
  ACTOR_VILLAGER_MCCOY_FRIGHTENED,
  /// An old man with a cane (Bilibin_AskAboutSoutheastRoute).
  ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST,
  /// An old woman.
  ACTOR_OLD_WOMAN_ROAD_BLOCKED,
  /// A small boy.
  ACTOR_BOY_SCARED_OF_ENTRANCE,
  /// A little girl.
  ACTOR_GIRL_RED_SKY,
  /// A strongman, stocky and muscular.
  ACTOR_STRONGMAN_IMIL_TOWER,
  /// A man with a pickaxe.
  ACTOR_VILLAGER_STRANGE_CREATURES,
  /// The golden statue in front of the weapon and armor shop, over the hole
  /// into the Treasury. It slides a cell under Move or a push.
  ACTOR_VILLAGE_STATUE,
  /// The same man once the curse is lifted, a stocky, muscular man with black
  /// hair.
  ACTOR_FORMER_TREE_MAN
};

/// Actors of the Bilibin Barricade (BilibinBarricade), numbering its
/// actor placements from ACTOR_FIRST_PLACED.
enum BilibinBarricadeActorId {
  /// The stack of two crates at the barricade's left end, which slides a cell
  /// under Move or a push.
  ACTOR_BARRICADE_CRATES = ACTOR_FIRST_PLACED,
  /// The bottom of the three trees.
  ACTOR_BOTTOM_TREE,
  /// The tree 32 pixels up, which the travelers from Imil push down onto its
  /// side by the river.
  ACTOR_MIDDLE_TREE,
  /// Jill as a tree, the top of the three.
  ACTOR_JILL_TREE,
  /// Jill's two friends, once the curse is lifted, east and west of her. They
  /// are two boys with brown hair. With her they were the "Gang of Three".
  ACTOR_GANG_MEMBER_EAST,
  ACTOR_GANG_MEMBER_WEST,
  /// Jill herself, once the curse is lifted, drawn as the woman with long fair
  /// hair the villages use (SPRITE_WOMAN_WITH_FAIR_HAIR), though the text calls
  /// the three children.
  ACTOR_JILL,
  /// Unused: a blank actor (SPRITE_BLANK_32) placed at the map's origin. No
  /// code or trigger addresses it, so it never shows or answers anything.
  ACTOR_INVISIBLE_AT_ORIGIN,
  /// The invisible actor (SPRITE_BLANK) that stands where the middle tree lies
  /// pushed down and answers for it.
  ACTOR_FALLEN_TREE_STAND_IN
};

/// Actors of the Bilibin Treasury (BilibinTreasury), numbering its
/// actor placements from ACTOR_FIRST_PLACED.
enum BilibinTreasuryActorId {
  /// The golden statue at the top of the walkway between the pools, which
  /// slides up and down it under Move or a push.
  ACTOR_TREASURY_STATUE = ACTOR_FIRST_PLACED,
  /// Gust, a Jupiter Djinni, placed until he has joined.
  ACTOR_GUST
};

/// Counts frames between the ripples, one every 30 frames.
struct BilibinRippleTimer {
  s16 frames;
};

struct BilibinRippleTimer gBilibin_RippleTimer;

#include "shared/actor_facing.c"

/// The party has looked once at the tree at Bilibin's entrance, so a second
/// look says it looks like a living person.
#define FLAG_BILIBIN_TREE_EXAMINED FLAG_AREA_TEMP_01
/// McCoy's gate guards have let the party in but it has not yet entered the
/// palace. The garden guard who reads it is in another area, so it is cleared
/// before he can.
#define FLAG_BILIBIN_KEEPING_MCCOY_WAITING FLAG_AREA_TEMP_04

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_BILIBIN) {
    return gBilibinEntrances;
  }
  if (sceneId == SCENE_BILIBIN_BARRICADE) {
    return gBilibinBarricadeEntrances;
  }
  if (sceneId == SCENE_BILIBIN_TREASURY) {
    return gBilibinTreasuryEntrances;
  }
  return gBilibinEntrancesOther;
}

/// Only the Bilibin Treasury has exit regions.
const struct SceneExitRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_BILIBIN_TREASURY) {
    return (const struct SceneExitRegion *)gBilibinTreasuryRegions;
  }
  return 0;
}

/// Returns the packed exits shared by the three scenes.
const u32 *Scene_GetExits(void) { return gBilibinExits; }

/// Talking to Gust in the Bilibin Treasury makes Gust, a Jupiter Djinni, join
/// the party.
static void Bilibin_GustJoins(void) {
  Djinn_JoinParty(ACTOR_GUST, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_GUST));
}

/// Selects the actors of Bilibin, the Bilibin Barricade or the Bilibin
/// Treasury.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_BILIBIN) {
    return gBilibinActorPlacements;
  }
  if (sceneId == SCENE_BILIBIN_BARRICADE) {
    return gBilibinBarricadeActorPlacements;
  }
  if (sceneId == SCENE_BILIBIN_TREASURY) {
    return gBilibinTreasuryActorPlacements;
  }
  return gBilibinActorPlacementsOther;
}

/// Searching the well in Bilibin, the leader peers in, and its bottom cannot be
/// seen.
static void Bilibin_InspectWell(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgBilibinBottomNotVisibleLooksVery, 1);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_BILIBIN) {
    return gBilibinTriggers;
  }
  if (sceneId == SCENE_BILIBIN_BARRICADE) {
    return gBilibinBarricadeTriggers;
  }
  if (sceneId == SCENE_BILIBIN_TREASURY) {
    return gBilibinTreasuryTriggers;
  }
  return gBilibinTriggersOther;
}

/// The Bilibin Barricade's sign forbids passing without a permit. Searching it
/// shows Lord McCoy's notice that no one may pass without one.
static void Bilibin_ReadOffLimitsSign(void) {
  Event_Start();
  FieldMessage_Show(MsgBilibinAreaOffLimitsThoseWithout, 1);
  Event_End();
}

/// Searching the sign in the Bilibin Treasury, which calls it McCoy's hidden
/// warehouse. `McCoy's Hidden Warehouse / Do Not Enter!`
static void Bilibin_ReadWarehouseSign(void) {
  Event_Start();
  FieldMessage_Show(MsgBilibinMccoysHiddenWarehouseDoNot, 1);
  Event_End();
}

/// The Gang of Three's trees at the Bilibin Barricade look like people.
static void Bilibin_InspectHumanShapedTree(void) {
  Event_Start();
  FieldMessage_Show(MsgBilibinThereTreeLooksLikePerson, 1);
  Event_End();
}

/// The tree at Bilibin's entrance almost looks like a man and, looked at again,
/// like a living person.
static void Bilibin_ExamineTreeMan(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinItsTreeButAlmostLooks);
  if (Event_GetFlag(FLAG_BILIBIN_TREE_EXAMINED) != 0) {
    gFieldWork->message++;
  }
  Event_ShowMessage(ACTOR_TREE_MAN, 0);
  GameFlag_Set(FLAG_BILIBIN_TREE_EXAMINED);
  Event_End();
}

/// A burly man asks about the tree at Bilibin's entrance.
static void Bilibin_AskAboutEntranceTree(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinDidSeeTreeAtEntrance);
  // "Did you see the tree at the entrance?"
  // Yes: "That tree was once a man... He came to our village under a..."
  // No: "I can't believe you missed the tree... You should look..."
  Event_AskYesNo(ACTOR_VILLAGER_ASKS_ABOUT_TREE, 0);
  Event_End();
}

/// An old man asks whether the party has tried heading southeast.
static void Bilibin_AskAboutSoutheastRoute(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinHaveTriedHeadingSoutheastFrom);
  // "Have you tried heading southeast from here?"
  // Yes: "That barricade was built because there's been trouble in Kolima."
  // No: "The road is blocked, so you might as well not even bother. I..."
  Event_AskYesNo(ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, 0);
  Event_End();
}

/// The man who was the tree at the entrance talks about it.
static void Bilibin_AskAboutBeingATree(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinWasTurnedIntoTreeFor);
  // "I was turned into a tree for a while... Can you believe it?"
  // Yes: "I think I now know what it means to sink your roots in one..."
  // No: "Yeah, I can't even believe it myself."
  Event_AskYesNo(ACTOR_FORMER_TREE_MAN, 0);
  Event_End();
}

/// A small boy asks whether the party broke the curse.
static void Bilibin_AskAboutCurseBreakers(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinCurseWasBrokenThanksEfforts);
  // "The curse was broken all thanks to the efforts of three..."
  // Yes: "That's amazing! You broke the so-called curse!"
  // No: "I didn't think so. You don't look very tough at all."
  Event_AskYesNo(ACTOR_BOY_SCARED_OF_ENTRANCE, 0);
  Event_End();
}

/// A strongman asks whether the party has been to Imil.
static void Bilibin_AskAboutImil(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinHaveEverBeenVillageImil);
  // "Have you ever been to the village of Imil, up north?"
  // Yes: "Did you see the great lighthouse over there? I bet it was..."
  // No: "If you ever go there, be sure to visit their famous lighthouse."
  Event_AskYesNo(ACTOR_STRONGMAN_IMIL_TOWER, 0);
  Event_End();
}

/// Walking up to a door in Bilibin opens it, and Isaac walks in: the inn
/// (cell (23, 22)), the houses east (12, 20), west (6, 21) and north (10, 13),
/// the item shop (4, 13), the weapon and armor shop (18, 11) or the Sanctum's
/// double door (24..25, 10).
///
/// Everyone in town stops where they are while the door opens. The Sanctum's
/// door opens with a thump instead of the sound of the others, and the screen
/// fades as Isaac walks through.
static void Bilibin_OpenDoorAndLeave(void) {
  u32 actorSlot;
  struct Actor *actor;
  s32 exitIndex;
  s32 column;
  s32 row;
  struct FieldWork *work;

  work = gFieldWork;
  Event_Start();
  // Freeze placed actors before the exit cells start animating.
  for (actorSlot = ACTOR_FIRST_PLACED; actorSlot < ACTOR_ID_COUNT;
       actorSlot++) {
    actor = Actor_Get(actorSlot);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  exitIndex = (s16)(work->stepTrigger - BILIBIN_TO_INN);
  if (exitIndex == BILIBIN_TO_SANCTUM - BILIBIN_TO_INN) {
    Sound_Play(SE_THUMP);
  } else {
    Sound_Play(SE_DOOR_OPEN);
  }
  // Each door has its cell and the steps that draw it opening.
  column = gBilibin_DoorCells[exitIndex][0];
  row = gBilibin_DoorCells[exitIndex][1];
  MapGrid_PlayMapEdit(gBilibin_DoorSteps[exitIndex], column, row);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (exitIndex == BILIBIN_TO_SANCTUM - BILIBIN_TO_INN) {
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
  } else {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  }
  if (exitIndex == BILIBIN_TO_ITEM_SHOP - BILIBIN_TO_INN) {
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
  } else {
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  }
  Event_Wait(16);
  Import_EventSetExitRequest(exitIndex + BILIBIN_TO_INN);
  Event_End();
}

/// Move on Jill's tree, adrift in the river at the Bilibin Barricade, lets
/// Isaac pull her ashore. Once Move has brought her one cell east, into cell
/// column 6, he hauls her out in three tugs, walks back up the bank, and her
/// thought comes through: she thanks him for saving her from being washed
/// downstream.
static void Bilibin_PullJillFromRiver(void) {
  struct Actor *leader;
  struct Actor *jillTree;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  jillTree = Actor_Get(ACTOR_JILL_TREE);
  if (FIELD_CELL(jillTree->x.fixed) == 6) {
    Event_Start();
    Actor_SetSpritePriority(ACTOR_JILL_TREE, 1);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.2);
    Actor_SetSpeed(ACTOR_JILL_TREE, 0.2);
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    jillTree->motionFlags = 0;
    // Each tug: the leader faces about, strains and draws her in, then steps
    // on.
    leader->scaleX = -FIXED_ONE;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
    Actor_MoveToAndWait(ACTOR_JILL_TREE, 111, 196);
    leader->scaleX = FIXED_ONE;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 185);
    Event_Wait(20);
    leader->scaleX = -FIXED_ONE;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
    Actor_MoveToAndWait(ACTOR_JILL_TREE, 121, 190);
    leader->scaleX = FIXED_ONE;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 141, 189);
    Event_Wait(20);
    leader->scaleX = -FIXED_ONE;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
    Actor_MoveToAndWait(ACTOR_JILL_TREE, 132, 186);
    leader->scaleX = FIXED_ONE;
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 166, 185);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    Actor_SetSpritePriority(ACTOR_JILL_TREE, 2);
    FieldPsynergy_StartMindReadWave(0, ACTOR_JILL_TREE);
    wait(10);
    Event_SetMessageId(MsgBilibinThankSavedMeFromBeing);
    // "Thank you! You saved me from being washed downstream!"
    Event_ShowMessage(ACTOR_JILL_TREE, 0);
    FieldPsynergy_StopMindReadWave();
    wait(10);
    GameFlag_Set(FLAG_BILIBIN_JILL_RESCUED);
    Event_End();
  }
}

/// Walking on into a gap from one of a few ledges, Isaac hops across it: in the
/// Treasury east or west across the pools from the statue's empty alcove, or
/// south off the foot of the walkway; at the Bilibin Barricade north from cell
/// (14, 10), or west from cell (8, 17) beside the crates.
static void Bilibin_HopAcross(void) { Field_TryJumpForward(); }

/// Jill and her friends thank the leader with a Hard Nut.
static void Bilibin_JillGivesHardNut(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0 ||
      GameFlag_Get(FLAG_BILIBIN_JILL_RESCUED) == 0)
    return;

  Event_Start();
  // The camera moves over to the three; the friend nearer the leader speaks
  // first.
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(263, CAMERA_KEEP, 173, 1);
  FieldCamera_WaitForMove();
  if (Actor_Get(ACTOR_GANG_MEMBER_EAST)->x.fixed > leader->x.fixed) {
    Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTH_SOUTHWEST, 20);
    Actor_ShowEmote(ACTOR_GANG_MEMBER_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    20);
    Event_SetMessageId(MsgBilibinYoureGuy);
    // "Hey! You're that guy!"
    Event_ShowMessageThenWait(ACTOR_GANG_MEMBER_WEST, 0, 10);
    Actor_ShowEmote(ACTOR_GANG_MEMBER_EAST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
  } else {
    Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTH_SOUTHEAST, 20);
    Actor_ShowEmote(ACTOR_GANG_MEMBER_EAST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    20);
    Event_SetMessageId(MsgBilibinYoureGuy);
    // "Hey! You're that guy!"
    Event_ShowMessageThenWait(ACTOR_GANG_MEMBER_EAST, 0, 10);
    Actor_ShowEmote(ACTOR_GANG_MEMBER_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
  }
  Actor_ShowEmote(ACTOR_JILL, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_JILL, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 268, 184);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Actor_RunRepeatedMotion(ACTOR_GANG_MEMBER_WEST, 2);
  // "Jill, are these the people who saved you?"
  Event_ShowMessageThenWait(ACTOR_GANG_MEMBER_WEST, 0, 10);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JILL, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
  Actor_SetAttachedEffect(ACTOR_JILL,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_JILL, FACING_SOUTH_SOUTHEAST, 10);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTH_SOUTHEAST, 10);
  Actor_RunRepeatedMotion(ACTOR_JILL, 1);
  // "Remember me? I was that tree!"
  Event_ShowMessageThenWait(ACTOR_JILL, 0, 10);
  Actor_SetAnimation(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GANG_MEMBER_WEST, ANIM_NOD);
  Event_Wait(20);
  // "I would have been swept down the river if it wasn't for you."
  Event_ShowMessage(ACTOR_JILL, 0);
  Actor_SetSpeed(ACTOR_JILL, 0.6);
  Actor_Get(ACTOR_JILL)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JILL, 266, 172);
  Event_Wait(1);
  Actor_Get(ACTOR_JILL)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
  // "Please, take this. It is a small token of my appreciation."
  Event_ShowMessageThenWait(ACTOR_JILL, 0, 10);
  FieldMessage_Show(MsgBilibinJillGaveIsaacSpecialGift, 1);
  gFieldWork->message++;
  Item_ShowFound(ITEM_HARD_NUT, 3);
  Party_GiveItem(ITEM_HARD_NUT, 0);
  Actor_SetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_SetSpeed(ACTOR_JILL, 1);
  Actor_Get(ACTOR_JILL)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JILL, 262, 156);
  Event_Wait(1);
  {
    struct Actor *jill = Actor_Get(ACTOR_JILL);
    u8 steeringFlags = jill->steeringFlags | ACTOR_FACES_MOTION;

    jill->steeringFlags = steeringFlags;
  }
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GANG_MEMBER_EAST, 2);
  Event_ShowMessageThenWait(ACTOR_GANG_MEMBER_EAST, 0, 10);
  Actor_SetAnimation(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GANG_MEMBER_WEST, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
  // From now on the three turn to face the leader wherever he goes.
  FieldActor_WatchAndStartScript(ACTOR_GANG_MEMBER_EAST,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                 gBilibin_FaceLeaderScript);
  FieldActor_WatchAndStartScript(ACTOR_GANG_MEMBER_WEST,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                 gBilibin_FaceLeaderScript);
  FieldActor_WatchAndStartScript(ACTOR_JILL,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                 gBilibin_FaceLeaderScript);
  GameFlag_Set(FLAG_BILIBIN_JILL_GIFT_RECEIVED);
  Event_End();
}

/// Sets up the scene being entered, Bilibin, the Bilibin Barricade or the
/// Bilibin Treasury.
s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gMainRoutineWork.next == SCENE_BILIBIN) {
    Bilibin_RestoreVillageActors();
  } else if (gMainRoutineWork.next == SCENE_BILIBIN_BARRICADE) {
    Bilibin_RestoreRiverActors();
    Task_AddFrame(Bilibin_UpdateWadingRipples, TASK_PASS_FRAME);
  } else if (gMainRoutineWork.next == SCENE_BILIBIN_TREASURY) {
    Bilibin_SetUpTreasury();
  }
  return 0;
}

/// Sets up Bilibin.
static void Bilibin_RestoreVillageActors(void) {
  s16 entrance;

  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Actor_SetPosition(ACTOR_TREE_MAN, 0, 0);
    Actor_FaceDirection(ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST,
                        FACING_SOUTH_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_OLD_WOMAN_ROAD_BLOCKED, FACING_SOUTH_SOUTHWEST,
                        0);
  } else {
    Actor_SetSpriteFlags(Actor_Get(ACTOR_TREE_MAN), 0);
    Actor_SetPosition(ACTOR_FORMER_TREE_MAN, 0, 0);
  }
  Actor_Get(ACTOR_IVY)->scaleY = 3 * FIXED_ONE / 2;
  entrance = gMainRoutineWork.entrance;
  if (entrance == BILIBIN_FROM_BILIBIN_TREASURY_WEST) {
    Actor_SetPosition(ACTOR_IVY, 0, 0);
  } else if (entrance == BILIBIN_FROM_SANCTUM) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    entrance = gMainRoutineWork.entrance;
    if (entrance == BILIBIN_FROM_BILIBIN_TREASURY_EAST) {
      Actor_SetPosition(ACTOR_VILLAGE_STATUE, 248, 216);
    }
  }
  Bilibin_UpdateVillageStatueCells();
  if (GameFlag_Get(FLAG_MCCOY_PALACE_ADMITTED) != 0 &&
      GameFlag_Get(FLAG_MCCOY_PALACE_VISITED) == 0) {
    GameFlag_Set(FLAG_BILIBIN_KEEPING_MCCOY_WAITING);
  }
}

/// The cell the village statue first stands on, over the way down to the
/// Bilibin Treasury.
enum { TREASURY_WAY_COLUMN = 16, TREASURY_WAY_ROW = 13 };

static void Bilibin_UpdateVillageStatueCells(void) {
  s32 leaderRow;
  s32 leaderColumn;
  struct Actor *leader;
  struct Actor *statue;
  FIXED statueX;
  s32 statueRow;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  statue = Actor_Get(ACTOR_VILLAGE_STATUE);
  statueRow = FIELD_CELL(statue->z.fixed);
  leaderColumn = FIELD_CELL(leader->x.fixed);
  leaderRow = FIELD_CELL(leader->z.fixed);
  statueX = statue->x.fixed;
  // Make the statue's square plain pushing ground, then wall off its cell.
  MapGrid_CopyCellAttributes(15, 11, 3, 1, 15, 12);
  MapGrid_CopyCellAttributes(15, 11, 3, 1, 15, 13);
  MapGrid_CopyCellAttributes(15, 11, 3, 1, 15, 14);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, FIELD_CELL(statueX), statueRow);
  if (FIELD_CELL(statueX) != TREASURY_WAY_COLUMN ||
      statueRow != TREASURY_WAY_ROW) {
    MapGrid_CopyCellAttributes(0, 0, 1, 1, TREASURY_WAY_COLUMN,
                               TREASURY_WAY_ROW);
  }
  // The statue may have moved onto the leader while this event was running.
  if (leaderColumn == TREASURY_WAY_COLUMN) {
    if (leaderRow == TREASURY_WAY_ROW) {
      Event_Start();
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      20);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
      Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
      if (statueRow == TREASURY_WAY_ROW) {
        Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 262, 196);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
      } else {
        Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 286, 218);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
      }
      Event_End();
    }
  }
}

/// Sets up the Bilibin Barricade.
static void Bilibin_RestoreRiverActors(void) {
  struct Actor *jillTree;
  s32 populationState;
  struct Actor *fallenTree;
  s32 crossingState;

  fallenTree = Actor_Get(ACTOR_MIDDLE_TREE);
  jillTree = Actor_Get(ACTOR_JILL_TREE);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BARRICADE_CRATES), 0);
  populationState = GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED);
  if (populationState != 0) {
    // The curse is lifted: the trees are gone.
    Actor_SetPosition(ACTOR_BOTTOM_TREE, 0, 0);
    Actor_SetPosition(ACTOR_MIDDLE_TREE, 0, 0);
    Actor_SetPosition(ACTOR_JILL_TREE, 0, 0);
    MapGrid_CopyCellsTo(56, 15, 40, 15, 1, 2);
    MapGrid_CopyCellAttributes(26, 15, 1, 3, 10, 15);
    if (GameFlag_Get(FLAG_BILIBIN_JILL_GIFT_RECEIVED) == 0) {
      if (GameFlag_Get(FLAG_BILIBIN_JILL_RESCUED) == 0) {
        Actor_SetPosition(ACTOR_JILL, 0, 0);
        Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_NORTH_NORTHEAST, 0);
        Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_NORTH_NORTHWEST, 0);
      }
    } else {
      Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_NORTH_NORTHEAST, 0);
      Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_NORTH_NORTHWEST, 0);
    }
  } else {
    Actor_SetPosition(ACTOR_GANG_MEMBER_EAST, 0, 0);
    Actor_SetPosition(ACTOR_GANG_MEMBER_WEST, 0, 0);
    Actor_SetPosition(ACTOR_JILL, 0, 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_BOTTOM_TREE), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_MIDDLE_TREE), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_JILL_TREE), 0);
    fallenTree->motionFlags = populationState;
    // Once FLAG_SATUROS_BEATEN_AT_MERCURY is set ("They crossed the river
    // after pushing us down"), the middle tree lies on its side and Jill is in
    // the water, or on the bank where she was pulled ashore.
    crossingState = GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY);
    if (crossingState != 0) {
      Actor_Get(ACTOR_BOTTOM_TREE)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
      Actor_Get(ACTOR_FALLEN_TREE_STAND_IN)->collisionFlags |=
          ACTOR_NEAR_PSYNERGY;
      Actor_Get(ACTOR_JILL_TREE)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
      Actor_SetPosition(ACTOR_FALLEN_TREE_STAND_IN, 142, 156);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_FALLEN_TREE_STAND_IN), 0);
      Actor_SetPosition(ACTOR_MIDDLE_TREE, 142, 156);
      fallenTree->sprite->rotation = DEGtoANG(90);
      fallenTree->y.fixed -= toFIXED(8);
      if (GameFlag_Get(FLAG_BILIBIN_JILL_RESCUED) != 0) {
        Actor_SetPosition(ACTOR_JILL_TREE, 132, 186);
      } else {
        Actor_SetPosition(ACTOR_JILL_TREE, 88, 196);
        Actor_SetSpritePriority(ACTOR_JILL_TREE, 3);
        jillTree->collisionFlags |= ACTOR_NEAR_TALK;
      }
    } else {
      fallenTree->y.fixed = toFIXED(32);
      jillTree->motionFlags = crossingState;
      jillTree->y.fixed = toFIXED(48);
    }
  }
  Bilibin_UpdateCratesCells();
}

/// The cells round the crate's place, as gBilibin_CratesPushCells lists them.
#define CRATES_LANE_CELLS 10

static void Bilibin_UpdateCratesCells(void) {
  struct Actor *actor;
  s32 cratesColumn;
  s32 cratesRow;
  u32 i;

  actor = Actor_Get(ACTOR_BARRICADE_CRATES);
  cratesColumn = FIELD_CELL(actor->x.fixed);
  cratesRow = FIELD_CELL(actor->z.fixed);

  for (i = 0; i < 2 * CRATES_LANE_CELLS; i += 2) {
    MapGrid_CopyCellAttributes(1, 0, 1, 1, gBilibin_CratesPushCells[i],
                               gBilibin_CratesPushCells[i + 1]);
  }

  MapGrid_CopyCellAttributes(0, 0, 1, 1, cratesColumn, cratesRow);
}

/// Sets up the Bilibin Treasury.
static void Bilibin_SetUpTreasury(void) {
  struct Actor *leader;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Bilibin_UpdateTreasuryStatueCells();
  if (gMainRoutineWork.entrance == BILIBIN_TREASURY_UPPER_FROM_LOWER ||
      gMainRoutineWork.entrance == BILIBIN_TREASURY_FROM_BILIBIN_NORTH) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      Event_Start();
      leader->y.fixed = toFIXED(16);
      FieldCamera_MoveWithinMapBounds(leader->x.fixed, leader->y.fixed,
                                      leader->z.fixed, 0);
      MapGrid_CenterOnFollowedPoint();
      Event_End();
      wait(1);
    }
  }
}

#include "shared/actor_find_at_cell.c"

/// Pushing a statue in Bilibin or the Bilibin Treasury, or the crates at the
/// Bilibin Barricade, slides it one cell ahead with the leader behind it, if
/// the cell is free.
static void Bilibin_PushStatueOrCrates(void) {
  struct Actor *leader;
  struct Actor *pushed;
  u32 direction;
  struct FieldPosition destination;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  direction = FACING_INDEX(leader->facing);
  pushed =
      Actor_FindAtCell((leader->x.part[0].integer +
                        (FIXEDtoINT(gPushedActor_DirectionSteps[direction]))) >>
                           4,
                       (leader->z.part[0].integer +
                        (s16)gPushedActor_DirectionSteps[direction]) >>
                           4);
  if (pushed != NULL) {
    pushed->layer = 2;
    Actor_StepFrom(&destination, pushed,
                   gPushedActor_DirectionSteps[direction]);
    if (FieldActor_CheckMovementCollision(pushed, &destination) <= 0) {
      // The leader leans on it, then both move together.
      Actor_SelectAnimation(leader, ANIM_PUSH);
      wait(15);
      Sound_Play(SE_PUSH);
      pushed->speedLimit = toFIXED(1) / 5;
      pushed->acceleration = toFIXED(1) / 5;
      FieldActor_SetMoveTargetFX(pushed, destination.x, destination.y,
                                 destination.z);
      leader->speedLimit = toFIXED(1) / 5;
      leader->acceleration = toFIXED(1) / 5;
      FieldActor_SetMoveTargetFX(leader, destination.x, destination.y,
                                 destination.z);
      Actor_WaitForMove(pushed);
      pushed->x.fixed = destination.x;
      pushed->z.fixed = destination.z;
      pushed->velocityX = 0;
      pushed->velocityZ = 0;
      Actor_SelectAnimation(leader, ANIM_STAND);
      // Each scene derives its cell behaviors from this same moved
      // actor.
      if (gMainRoutineWork.next == SCENE_BILIBIN_BARRICADE)
        Bilibin_UpdateCratesCells();
      else if (gMainRoutineWork.next == SCENE_BILIBIN)
        Bilibin_UpdateVillageStatueCells();
      else if (gMainRoutineWork.next == SCENE_BILIBIN_TREASURY)
        Bilibin_UpdateTreasuryStatueCells();
    }
  }
}

/// While Isaac wades in the river at the Bilibin Barricade, west of the fallen
/// tree, a ripple spreads from his feet every 30 frames. It runs every frame.
static void Bilibin_UpdateWadingRipples(void) {
  struct Actor *actor;

  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  if (actor->x.fixed < toFIXED(142)) {
    if (actor->y.fixed < toFIXED(8)) {
      if (gBilibin_RippleTimer.frames == 0)
        Bilibin_SpawnRipple(actor);
      if (++gBilibin_RippleTimer.frames == 30)
        gBilibin_RippleTimer.frames = 0;
    } else {
      gBilibin_RippleTimer.frames = 0;
    }
  }
}

/// Spawns a semi-transparent ripple (SPRITE_RIPPLE's spreading rings) at the
/// wader, which releases itself after 30 frames.
static void Bilibin_SpawnRipple(struct Actor *actor) {
  struct Actor *effect;
  struct Sprite *sprite;

  effect = FieldActor_Create(SPRITE_RIPPLE, actor->x.fixed, actor->y.fixed,
                             actor->z.fixed);
  if (effect == NULL)
    return;

  sprite = effect->sprite;
  Actor_StartScript(effect, gBilibin_RippleScript);
  effect->motionFlags = 0;
  effect->layer = 1;
  effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  if (sprite == NULL)
    return;

  Sprite_StartAnimation(sprite, 2);
  sprite->flags = 0;
  sprite->image.objectMode = 1;
  sprite->image.priority = 3;
}

static void Bilibin_UpdateTreasuryStatueCells(void) {
  struct Actor *statue;

  statue = Actor_Get(ACTOR_TREASURY_STATUE);
  if (statue == NULL)
    return;

  if (FIELD_CELL(statue->z.fixed) == 6)
    MapGrid_CopyCellAttributes(2, 0, 1, 1, 14, 6);
  else
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 14, 6);

  if (FIELD_CELL(statue->z.fixed) == 9)
    MapGrid_CopyCellAttributes(2, 0, 1, 1, 14, 9);
  else
    MapGrid_CopyCellAttributes(1, 0, 1, 1, 14, 9);
}

s32 gBilibin_FaceLeaderScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Unused: the entrances for a scene other than the three, which never loads
/// this overlay. Its one row has no SCENE_TABLE_END after it.
struct SceneEntrance gBilibinEntrancesOther[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
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
    SCENE_EXIT(BILIBIN_TO_INN, CottageRoomsC, COTTAGE_ROOMS_C_INN),
    SCENE_EXIT(BILIBIN_TO_HOUSE_EAST, CottageRoomsC, COTTAGE_ROOMS_C_HOUSE_EAST),
    SCENE_EXIT(BILIBIN_TO_HOUSE_WEST, CottageRoomsC, COTTAGE_ROOMS_C_HOUSE_WEST),
    SCENE_EXIT(BILIBIN_TO_HOUSE_NORTH, CottageRoomsC, COTTAGE_ROOMS_C_HOUSE_NORTH),
    SCENE_EXIT(BILIBIN_TO_ITEM_SHOP, CottageRoomsC, COTTAGE_ROOMS_C_ITEM_SHOP),
    SCENE_EXIT(BILIBIN_TO_WEAPON_AND_ARMOR_SHOP, CottageRoomsC, COTTAGE_ROOMS_C_WEAPON_AND_ARMOR_SHOP),
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

struct ActorPlacement gBilibinActorPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gBilibinActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        40, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        256, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 328, FACING_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 240, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        176, 0, 328, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        128, 0, 224, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        176, 0, 96, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        192, 0, 96, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        208, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        192, 0, 208, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        232, 0, 296, FACING_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 360, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GOLDEN_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        264, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 432, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gBilibinBarricadeActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        136, 0, 300, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        272, 0, 154, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        272, 0, 154, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        272, 0, 154, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        282, 0, 170, FACING_WEST_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        246, 0, 170, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        262, 0, 156, FACING_WEST_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK_32, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gBilibinTreasuryActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_GOLDEN_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 100, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_GUST), MOVEMENT_TYPE_STAND,
        328, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct Trigger gBilibinTriggersOther[] = {
    { TRIGGER_TABLE_END },
};

struct Trigger gBilibinTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_TO_WORLD_MAP },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_TO_MCCOY_PALACE_GROUNDS },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, BILIBIN_TO_BILIBIN_TREASURY_WEST },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, BILIBIN_TO_BILIBIN_TREASURY_EAST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(Bilibin_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(Bilibin_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Bilibin_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Bilibin_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Bilibin_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Bilibin_OpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Bilibin_OpenDoorAndLeave) },
    { TRIGGER_TALK, ACTOR_TREE_MAN, CONDITION_ALWAYS, EVENT(Bilibin_ExamineTreeMan) },
    { TRIGGER_TALK, ACTOR_FORMER_TREE_MAN, CONDITION_ALWAYS, EVENT(Bilibin_AskAboutBeingATree) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_WELCOME, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWelcomeToBilibinPleaseJustIgnore) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_WELCOME, CONDITION_ALWAYS, MSGNO(MsgBilibinWelcomeToBilibinWellForgetThat) },
    { TRIGGER_TALK, ACTOR_VILLAGER_GROUP_TO_IMIL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIHearALargeGroupCrossed) },
    { TRIGGER_TALK, ACTOR_VILLAGER_GROUP_TO_IMIL, CONDITION_ALWAYS, MSGNO(MsgBilibinIfYoureTalkingAboutTheTravelers) },
    { TRIGGER_TALK, ACTOR_VILLAGER_ASKS_ABOUT_TREE, FLAG_KOLIMA_CURSE_LIFTED, EVENT(Bilibin_AskAboutEntranceTree) },
    { TRIGGER_TALK, ACTOR_VILLAGER_ASKS_ABOUT_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinYoureWonderingAboutTheTreeNear) },
    { TRIGGER_TALK, ACTOR_VILLAGER_MCCOY_FRIGHTENED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinLordMccoyHasBeenQuiteFrightened) },
    { TRIGGER_TALK, ACTOR_VILLAGER_MCCOY_FRIGHTENED, CONDITION_ALWAYS, MSGNO(MsgBilibinISawLordMccoyTheChief) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, FLAG_KOLIMA_CURSE_LIFTED, EVENT(Bilibin_AskAboutSoutheastRoute) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, CONDITION_ALWAYS, MSGNO(MsgBilibinIHearTheBarricadeGateThat) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ROAD_BLOCKED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheRoadToKolimaIsBlocked) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ROAD_BLOCKED, CONDITION_ALWAYS, MSGNO(MsgBilibinThoseThreeKidsVanishedDuringAll) },
    { TRIGGER_TALK, ACTOR_BOY_SCARED_OF_ENTRANCE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinEverSinceThatManTurnedInto) },
    { TRIGGER_TALK, ACTOR_BOY_SCARED_OF_ENTRANCE, CONDITION_ALWAYS, EVENT(Bilibin_AskAboutCurseBreakers) },
    { TRIGGER_TALK, ACTOR_GIRL_RED_SKY, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIsItTrueThatAMountain) },
    { TRIGGER_TALK, ACTOR_GIRL_RED_SKY, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoysMenSaidTheySaw) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_IMIL_TOWER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheresATowerAtTheTip) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_IMIL_TOWER, CONDITION_ALWAYS, EVENT(Bilibin_AskAboutImil) },
    { TRIGGER_TALK, ACTOR_VILLAGER_STRANGE_CREATURES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinSomethingChangedInTheEruptionNow) },
    { TRIGGER_TALK, ACTOR_VILLAGER_STRANGE_CREATURES, CONDITION_ALWAYS, MSGNO(MsgBilibinTheEruptionOnTheOtherSide) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VILLAGE_STATUE, CONDITION_ALWAYS, EVENT(Bilibin_UpdateVillageStatueCells) },
    { STEP_TRIGGER_WALKING, 20, CONDITION_ALWAYS, EVENT(Bilibin_PushStatueOrCrates) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, CONDITION_ALWAYS, 0 },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, FLAG_BILIBIN_HERB, FIND_ITEM(ITEM_HERB) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_BILIBIN_COINS, FIND_COINS(5) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 90, CONDITION_ALWAYS, EVENT(Bilibin_InspectWell) },
    { TRIGGER_TABLE_END },
};

struct Trigger gBilibinBarricadeTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_BARRICADE_TO_WORLD_MAP_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_BARRICADE_TO_WORLD_MAP_WEST },
    { TRIGGER_TALK, ACTOR_BOTTOM_TREE, CONDITION_ALWAYS, EVENT(Bilibin_InspectHumanShapedTree) },
    { TRIGGER_TALK, ACTOR_MIDDLE_TREE, CONDITION_ALWAYS, EVENT(Bilibin_InspectHumanShapedTree) },
    { TRIGGER_TALK, ACTOR_JILL_TREE, CONDITION_ALWAYS, EVENT(Bilibin_InspectHumanShapedTree) },
    { TRIGGER_TALK, ACTOR_FALLEN_TREE_STAND_IN, CONDITION_ALWAYS, EVENT(Bilibin_InspectHumanShapedTree) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOTTOM_TREE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgBilibinTheyCrossedTheRiverAfterPushing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOTTOM_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyreSoHeavy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_TREE_STAND_IN, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_TREE_STAND_IN, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIDDLE_TREE, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIDDLE_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL_TREE, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinAhhhhPleaseAnyonePleaseHelp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThankSavedMeFromBeing) },
    { TRIGGER_TALK, ACTOR_GANG_MEMBER_EAST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinIHeardWeWereOnceCalled) },
    { TRIGGER_TALK, ACTOR_GANG_MEMBER_EAST, CONDITION_ALWAYS, MSGNO(MsgBilibinItWasAwfulLuckBeingTurned) },
    { TRIGGER_TALK, ACTOR_GANG_MEMBER_WEST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinIfOnlyWeHadntTriedGetting) },
    { TRIGGER_TALK, ACTOR_GANG_MEMBER_WEST, CONDITION_ALWAYS, MSGNO(MsgBilibinIReallyLikeThisSpotBy) },
    { TRIGGER_TALK, ACTOR_JILL, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillThinkOfYouWhenever) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_EAST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinWeWorkedSoHardIfOnly) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_EAST, CONDITION_ALWAYS, MSGNO(MsgBilibinWhoseIdeaWasItToPick) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_WEST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinWhatACruelWorldWeJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_WEST, CONDITION_ALWAYS, MSGNO(MsgBilibinWellItWasntMyIdeaTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL, CONDITION_ALWAYS, MSGNO(MsgBilibinThesePersimmonsAreJustLikeMy) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 11, CONDITION_ALWAYS, EVENT(Bilibin_ReadOffLimitsSign) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 30, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldDoorTightlyLocked)) },
    { TRIGGER_STEP, 13, FLAG_BILIBIN_JILL_GIFT_RECEIVED, EVENT(Bilibin_JillGivesHardNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_JILL_TREE, FLAG_BILIBIN_JILL_RESCUED, EVENT(Bilibin_PullJillFromRiver) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Bilibin_HopAcross) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT(Bilibin_HopAcross) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT(Bilibin_PushStatueOrCrates) },
    { STEP_TRIGGER_WALKING, 12, CONDITION_ALWAYS, EVENT(Bilibin_PushStatueOrCrates) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BARRICADE_CRATES, CONDITION_ALWAYS, EVENT(Bilibin_UpdateCratesCells) },
    { TRIGGER_TABLE_END },
};

struct Trigger gBilibinTreasuryTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_TREASURY_TO_BILIBIN_SOUTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_TREASURY_LOWER_TO_UPPER },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 10, CONDITION_ALWAYS, EVENT(Bilibin_ReadWarehouseSign) },
    { TRIGGER_TALK, ACTOR_GUST, CONDITION_ALWAYS, EVENT(Bilibin_GustJoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TREASURY_STATUE, CONDITION_ALWAYS, EVENT(Bilibin_UpdateTreasuryStatueCells) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT(Bilibin_PushStatueOrCrates) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT(Bilibin_PushStatueOrCrates) },
    { STEP_TRIGGER_WALKING, 13, CONDITION_ALWAYS, EVENT(Bilibin_PushStatueOrCrates) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT(Bilibin_HopAcross) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT(Bilibin_HopAcross) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT(Bilibin_HopAcross) },
    { TRIGGER_TABLE_END },
};

u16 gBilibin_SanctumDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(37, 33, 4, 2, 5), MAPGRID_ANIMATION_STEP(33, 33, 4, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 gBilibin_ItemShopDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 43, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 43, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 gBilibin_WeaponAndArmorShopDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 52, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 52, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 gBilibin_HouseEastDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 54, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 54, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 gBilibin_InnDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 48, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 48, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 gBilibin_HouseWestDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 46, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 46, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 gBilibin_HouseNorthDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 50, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 50, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};

u16 *gBilibin_DoorSteps[] = {
    gBilibin_InnDoorSteps,
    gBilibin_HouseEastDoorSteps,
    gBilibin_HouseWestDoorSteps,
    gBilibin_HouseNorthDoorSteps,
    gBilibin_ItemShopDoorSteps,
    gBilibin_WeaponAndArmorShopDoorSteps,
    gBilibin_SanctumDoorSteps,
};

/// Each door's cell, in the order of gBilibin_DoorSteps.
s16 gBilibin_DoorCells[][2] = {
    {54, 20}, {43, 18}, {37, 19}, {41, 11}, {35, 11}, {49, 9}, {55, 6},
};

/// The ten cells round the crates' place at the Bilibin Barricade that push
/// them, as column and row.
u8 gBilibin_CratesPushCells[] = {
    8,  18, 9,  18, 10, 18, 8,  19, 9,  19,
    10, 19, 11, 19, 9,  20, 10, 20, 11, 20,
};
// clang-format on

#include "shared/pushed_actor_direction_steps.c"

s32 gBilibin_RippleScript[] = {
    SC_WAIT(30),
    SC_RELEASE,
};
