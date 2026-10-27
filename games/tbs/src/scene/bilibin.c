/// @file
/// Bilibin, the Bilibin Treasury and the Bilibin Barricade, three scenes that
/// share one overlay: SceneId_Bilibin, SceneId_BilibinTreasury and
/// SceneId_BilibinBarricade. The getters pick each scene's tables by
/// gPersistentState.sceneId, Bilibin_Initialize sets up the scene being
/// entered, and Bilibin_PushStatueOrCrates serves all three.
///
/// Bilibin (SceneId_Bilibin) is the town past Goma Cave, whose chief, Lord
/// McCoy, has closed the road to Kolima. Almost everything here turns on
/// FLAG_KOLIMA_CURSE_LIFTED, which the party sets in Kolima Forest when it
/// heals Tret. Until then a man turned into a tree (ACTOR_TREE_MAN) stands at
/// the entrance: a first look says it almost looks like a man, a second that it
/// looks like a living person (FLAG_BILIBIN_TREE_EXAMINED remembers the first
/// until the party leaves Bilibin), and Mind Read hears `Help me...
/// Somebody...`. Once the curse is lifted, Bilibin_RestoreVillageActors
/// puts the man himself (ACTOR_FORMER_TREE_MAN) beside the empty spot and
/// turns ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST and
/// ACTOR_OLD_WOMAN_ROAD_BLOCKED about. Each of the ten villagers
/// (ACTOR_YOUNG_MAN_WELCOME to ACTOR_VILLAGER_STRANGE_CREATURES) has two
/// lines and two thoughts in gBilibinTriggers, the row that needs the flag
/// clear first, so the first row that matches picks the time; a few ask a
/// yes-or-no question (the Bilibin_AskAbout functions).
///
/// Bilibin's doors (step triggers 3 to 9, the numbers of the exits they take)
/// open before the party walks in (Bilibin_OpenDoorAndLeave): six lead into
/// Bilibin's buildings (SceneId_CottageRoomsC) and the seventh into the
/// Bilibin Sanctum, out of which (BILIBIN_FROM_SANCTUM) the area name is not
/// shown again. The other exits lead to the World Map, to the garden of McCoy's
/// Palace and, two ways, into the Treasury: past the ivy at the west edge
/// (ACTOR_IVY), gone when the party comes back out that way
/// (BILIBIN_FROM_BILIBIN_TREASURY_WEST); its Whirlwind row runs no event, so
/// Field_UsePsynergy only aims Whirlwind at it and the ivy stays. And
/// from the cell a statue (ACTOR_VILLAGE_STATUE) stands on, once Move or a
/// push slides it aside. Bilibin_UpdateVillageStatueCells keeps the cells of
/// the statue's lane in step and hops the leader clear if it lands on him;
/// coming back out that way (BILIBIN_FROM_BILIBIN_TREASURY_EAST) the party
/// finds the statue moved.
///
/// FLAG_BILIBIN_KEEPING_MCCOY_WAITING is set while McCoy's gate guards have let
/// the party in (FLAG_MCCOY_PALACE_ADMITTED) but it has not yet entered the
/// palace (FLAG_MCCOY_PALACE_VISITED), for the garden's east guard to scold the
/// party for keeping McCoy waiting. The garden is another area
/// (scenes.s gives Bilibin 8 and the garden 9), so
/// Scene_Arrive clears the flag on the way in and the guard never
/// sees it set.
///
/// The Bilibin Treasury (SceneId_BilibinTreasury) is what its sign calls
/// `McCoy's Hidden Warehouse`. Gust, a Jupiter Djinni, is placed there until he
/// joins the party when spoken to (Bilibin_GustJoins). A second statue
/// (ACTOR_TREASURY_STATUE) slides along one column, and
/// Bilibin_UpdateTreasuryStatueCells keeps the two cells it can stand on in
/// step. Arriving by BILIBIN_TREASURY_UPPER_FROM_LOWER or
/// BILIBIN_TREASURY_FROM_BILIBIN_NORTH the leader starts 16 pixels above the
/// floor; two exit regions (Bilibin_GetRegions) lead down its west ladder to
/// the lower room and back to Bilibin by the statue.
///
/// The Bilibin Barricade (SceneId_BilibinBarricade) is the road east to Kolima
/// where McCoy's barricade stands, reached from the World Map at either end.
/// Its sign reads `This area is off-limits to all those without a permit. —Lord
/// McCoy`, and a door there stays locked. A stack of crates
/// (ACTOR_BARRICADE_CRATES) slides a cell at a time under Move or a push, and
/// Bilibin_UpdateCratesCells rewrites the ten cells of its lane after each move
/// (its recording,
/// places/bilibin-barricade/events/Bilibin_UpdateCratesCells.gif). By the river
/// stand three trees, one on another's shoulders (ACTOR_BOTTOM_TREE,
/// ACTOR_MIDDLE_TREE and ACTOR_JILL_TREE at one spot, 0, 32 and 48 pixels
/// up): the Gang of Three, children of the town turned into trees by the curse.
///
/// Once Saturos is beaten at Mercury Lighthouse
/// (FLAG_SATUROS_BEATEN_AT_MERCURY), while the curse holds, the travelers from
/// Imil have crossed the river by pushing the trees down:
/// Bilibin_RestoreRiverActors lays the middle tree on its side, with an
/// invisible actor (ACTOR_FALLEN_TREE_STAND_IN) that answers for it,
/// and puts Jill's tree in the water. Move on her
/// (Bilibin_PullJillFromRiver) works once she is in cell column 6, by the
/// bank: the leader pulls her ashore in three tugs, and
/// FLAG_BILIBIN_JILL_RESCUED keeps her there. While whoever the player controls
/// wades in the river, Bilibin_UpdateWadingRipples spreads a ripple from their
/// feet every 30 frames.
///
/// Once the curse is lifted the trees are gone, and the Gang of Three are
/// children again on the bank (ACTOR_GANG_MEMBER_EAST,
/// ACTOR_GANG_MEMBER_WEST and ACTOR_JILL). If Jill was never pulled out she
/// is missing. If she was, stepping up to them (step trigger 13) plays
/// Bilibin_JillGivesHardNut, which gives the leader a Hard Nut
/// (FLAG_BILIBIN_JILL_GIFT_RECEIVED); from then on the three turn to watch him.

#include "persistent_state.h"
#include "djinn_ids.h"
#include "flags.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/bilibin.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
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
#include "event_layer.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "actor.h"
#include "songs.h"
#include "item_ids.h"
#include "fixed_math.h"

static void Bilibin_RestoreVillageActors(void);
static void Bilibin_UpdateVillageStatueCells(void);
static void Bilibin_RestoreRiverActors(void);
static void Bilibin_UpdateCratesCells(void);
static void Bilibin_SetUpTreasury(void);
static void Bilibin_UpdateWadingRipples(void);
static void Bilibin_SpawnRipple(struct Actor *actor);
static void Bilibin_UpdateTreasuryStatueCells(void);

/// Actors of Bilibin, numbering its actor templates from
/// ACTOR_FIRST_PLACED. The ten villagers are named by what they say before
/// the curse is lifted.
enum BilibinActorId {
  /// Ivy drawn half as tall again at the west edge, where the way into the
  /// Treasury runs; its Whirlwind row runs nothing.
  /// ★ Named from what the player sees: leafy ivy against the cliff at the
  /// village's west edge, the sprite of the ivy Whirlwind blows away (its
  /// render)
  ACTOR_IVY = ACTOR_FIRST_PLACED,
  /// The man turned into a tree at the village entrance, drawn as a tree.
  /// ★ Named from the game: "It's a tree, but it almost looks like... a man?"
  /// (MsgBilibinItsTreeButAlmostLooks)
  ACTOR_TREE_MAN,
  /// A young man with brown hair.
  /// ★ Named from the game: "Welcome to Bilibin! Please, just ignore that
  /// strange tree out front!" (MsgBilibinWelcomeToBilibinPleaseJustIgnore)
  ACTOR_YOUNG_MAN_WELCOME,
  /// A woman with fair hair.
  /// ★ Named from the game: "I hear a large group crossed over Goma Range,
  /// heading toward Imil." (MsgBilibinIHearALargeGroupCrossed)
  ACTOR_VILLAGER_GROUP_TO_IMIL,
  /// A burly grey-haired man (Bilibin_AskAboutEntranceTree).
  /// ★ Named from the game: "Did you see the tree at the entrance?"
  /// (MsgBilibinDidSeeTreeAtEntrance)
  ACTOR_VILLAGER_ASKS_ABOUT_TREE,
  /// A woman in a green dress.
  /// ★ Named from the game: "Lord McCoy has been quite frightened"
  /// (MsgBilibinLordMccoyHasBeenQuiteFrightened)
  ACTOR_VILLAGER_MCCOY_FRIGHTENED,
  /// An old man with a cane (Bilibin_AskAboutSoutheastRoute).
  /// ★ Named from the game: "Have you tried heading southeast from here?"
  /// (MsgBilibinHaveTriedHeadingSoutheastFrom)
  ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST,
  /// An old woman.
  /// ★ Named from the game: "The road to Kolima is blocked now"
  /// (MsgBilibinTheRoadToKolimaIsBlocked)
  ACTOR_OLD_WOMAN_ROAD_BLOCKED,
  /// A small boy.
  /// ★ Named from the game: "Ever since that man turned into a tree"
  /// (MsgBilibinEverSinceThatManTurnedInto)
  ACTOR_BOY_SCARED_OF_ENTRANCE,
  /// A little girl.
  /// ★ Named from the game: "Is it true that a mountain exploded"
  /// (MsgBilibinIsItTrueThatAMountain)
  ACTOR_GIRL_RED_SKY,
  /// A strongman, stocky and muscular.
  /// ★ Named from the game: "There's a tower at the tip of Imil"
  /// (MsgBilibinTheresATowerAtTheTip)
  ACTOR_STRONGMAN_IMIL_TOWER,
  /// A man with a pickaxe.
  /// ★ Named from the game: "all sorts of strange creatures lurk in every
  /// forest and mountain!" (MsgBilibinSomethingChangedInTheEruptionNow)
  ACTOR_VILLAGER_STRANGE_CREATURES,
  /// A statue that slides a cell under Move or a push.
  /// ★ Named from what the player sees: a golden statue of a figure in a long
  /// robe, SPRITE_GOLDEN_STATUE (its render)
  ACTOR_VILLAGE_STATUE,
  /// The same man once the curse is lifted, a stocky, muscular man with black
  /// hair.
  /// ★ Named from the game: "I was turned into a tree for a while..."
  /// (MsgBilibinWasTurnedIntoTreeFor)
  ACTOR_FORMER_TREE_MAN
};

/// Actors of the Bilibin Barricade (BilibinBarricade), numbering its
/// actor templates from ACTOR_FIRST_PLACED.
enum BilibinBarricadeActorId {
  /// A stack of crates that slides a cell under Move or a push.
  /// ★ Named from what the player sees: a stack of two wooden crates,
  /// SPRITE_CRATES (its render)
  ACTOR_BARRICADE_CRATES = ACTOR_FIRST_PLACED,
  /// The bottom of the three trees.
  /// ★ Named from the game: "They're... so heavy..."
  /// (MsgBilibinTheyreSoHeavy)
  ACTOR_BOTTOM_TREE,
  /// The tree 32 pixels up, which the travelers from Imil push down onto its
  /// side by the river.
  /// ★ Named from the game: "There is a tree that looks like a person."
  /// (MsgBilibinThereTreeLooksLikePerson)
  ACTOR_MIDDLE_TREE,
  /// Jill as a tree, the top of the three.
  /// ★ Named from the game: "AHHHH! Please! Anyone! Please help!"
  /// (MsgBilibinAhhhhPleaseAnyonePleaseHelp)
  ACTOR_JILL_TREE,
  /// Jill's two friends, once the curse is lifted, east and west of her: two
  /// boys with brown hair. With her they were the "Gang of Three".
  /// ★ Named from the game: "It was awful luck, being turned into"
  /// (MsgBilibinItWasAwfulLuckBeingTurned)
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
  /// Unused: a blank actor (SPRITE_BLANK_32) placed at the map's origin;
  /// no
  /// code or trigger addresses it, so it never shows or answers anything.
  ACTOR_INVISIBLE_AT_ORIGIN,
  /// The invisible actor (SPRITE_BLANK) that stands where the middle
  /// tree lies
  /// pushed down and answers for it.
  /// ★ Named from the game: "Oh, it looks like she is okay."
  /// (MsgBilibinOhItLooksLikeSheIs)
  ACTOR_FALLEN_TREE_STAND_IN
};

/// Actors of the Bilibin Treasury (BilibinTreasury), numbering its
/// actor templates from ACTOR_FIRST_PLACED.
enum BilibinTreasuryActorId {
  /// A statue that slides along one column under Move or a push.
  /// ★ Named from what the player sees: a golden statue of a figure in a long
  /// robe, SPRITE_GOLDEN_STATUE (its render)
  ACTOR_TREASURY_STATUE = ACTOR_FIRST_PLACED,
  /// Gust, a Jupiter Djinni, placed until he has joined.
  /// ★ Named from the game: "Gust" (MsgDjinnName)
  ACTOR_GUST
};

/// Counts frames between the ripples, one every 30 frames.
struct BilibinRippleTimer {
  s16 frames;
};

struct BilibinRippleTimer gBilibin_RippleTimer;

#include "common/actor_facing.h"

/// The party has looked once at the tree at Bilibin's entrance, so a second
/// look says it looks like a living person.
#define FLAG_BILIBIN_TREE_EXAMINED FLAG_AREA_TEMP_01
/// McCoy's gate guards have let the party in but it has not yet entered the
/// palace; the garden guard who reads it is in another area, so it is
/// cleared before he can.
/// ★ Named from the game: "The master can be quite cranky. Hurry along, now!"
/// (MsgBilibinMasterQuiteCranky)
#define FLAG_BILIBIN_KEEPING_MCCOY_WAITING FLAG_AREA_TEMP_04

const struct SceneEntrance *Bilibin_GetEntrances(void) {
  s16 sceneId = gPersistentState.sceneId;

  if (sceneId == (s32)&SceneId_Bilibin) {
    return gBilibinEntrances;
  }
  if (sceneId == (s32)&SceneId_BilibinBarricade) {
    return gBilibinBarricadeEntrances;
  }
  if (sceneId == (s32)&SceneId_BilibinTreasury) {
    return gBilibinTreasuryEntrances;
  }
  return (const struct SceneEntrance *)gBilibinEntrancesOther;
}

/// Only the Bilibin Treasury has exit regions.
const struct SceneExitRegion *Bilibin_GetRegions(void) {
  if (gPersistentState.sceneId == (s32)&SceneId_BilibinTreasury) {
    return (const struct SceneExitRegion *)gBilibinTreasuryRegions;
  }
  return 0;
}

/// Returns the packed exits shared by the three scenes.
const u32 *Bilibin_GetExits(void) { return gBilibinExits; }

/// Talking to Gust in the Bilibin Treasury: Gust, a Jupiter Djinni, joins the
/// party.
static void Bilibin_GustJoins(void) {
  Djinn_JoinParty(ACTOR_GUST, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_GUST));
}

/// Selects the actors of Bilibin, the Bilibin Barricade or the Bilibin
/// Treasury.
const struct ActorTemplate *Bilibin_GetActorTemplates(void) {
  s16 sceneId = gPersistentState.sceneId;

  if (sceneId == (s32)&SceneId_Bilibin) {
    return gBilibinActorTemplates;
  }
  if (sceneId == (s32)&SceneId_BilibinBarricade) {
    return gBilibinBarricadeActorTemplates;
  }
  if (sceneId == (s32)&SceneId_BilibinTreasury) {
    return gBilibinTreasuryActorTemplates;
  }
  return gBilibinActorTemplatesOther;
}

/// Searching the well in Bilibin: the leader peers in, and its bottom cannot
/// be seen.
static void Bilibin_InspectWell(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgBilibinBottomNotVisibleLooksVery, 1);
  Event_End();
}

const struct Trigger *Bilibin_GetTriggers(void) {
  s16 sceneId = gPersistentState.sceneId;

  if (sceneId == (s32)&SceneId_Bilibin) {
    return gBilibinTriggers;
  }
  if (sceneId == (s32)&SceneId_BilibinBarricade) {
    return gBilibinBarricadeTriggers;
  }
  if (sceneId == (s32)&SceneId_BilibinTreasury) {
    return gBilibinTreasuryTriggers;
  }
  return gBilibinTriggersOther;
}

/// The Bilibin Barricade's sign forbids passing without a permit. Searching it
/// shows Lord McCoy's notice that no one may pass without one.
static void Bilibin_ReadRestrictedAreaSign(void) {
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
/// Talking to one, standing or pushed down, or to the actor that answers
/// for the fallen one, says so. `There is a tree that looks like a person.`
static void Bilibin_InspectHumanShapedTree(void) {
  Event_Start();
  FieldMessage_Show(MsgBilibinThereTreeLooksLikePerson, 1);
  Event_End();
}

/// Talking to the tree at Bilibin's entrance, before the curse is lifted: it
/// almost looks like a man and, looked at again, like a living person.
/// Map-section temp flag FLAG_BILIBIN_TREE_EXAMINED remembers the first look
/// until the party leaves Bilibin's area.
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

/// A burly man asks about the tree at Bilibin's entrance. He's grey-haired and
/// there before the curse is lifted, and asks whether the party saw it; on
/// yes, he says it was once a man who came under a curse.
static void Bilibin_AskAboutEntranceTree(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinDidSeeTreeAtEntrance);
  Event_AskYesNoAndShowResponse(ACTOR_VILLAGER_ASKS_ABOUT_TREE, 0);
  Event_End();
}

/// An old man asks whether the party has tried heading southeast. He's in
/// Bilibin before the curse is lifted, and either way he says the barricade
/// blocks the road.
static void Bilibin_AskAboutSoutheastRoute(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinHaveTriedHeadingSoutheastFrom);
  Event_AskYesNoAndShowResponse(ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, 0);
  Event_End();
}

/// The man who was the tree at the entrance talks about it. He's in Bilibin
/// once the curse is lifted, and asks whether the party can believe he was a
/// tree.
static void Bilibin_AskAboutBeingATree(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinWasTurnedIntoTreeFor);
  Event_AskYesNoAndShowResponse(ACTOR_FORMER_TREE_MAN, 0);
  Event_End();
}

/// A small boy asks whether the party broke the curse. He's in Bilibin once the
/// curse is lifted, and asks whether the party are the three young warriors who
/// broke it.
static void Bilibin_AskAboutCurseBreakers(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinCurseWasBrokenThanksEfforts);
  Event_AskYesNoAndShowResponse(ACTOR_BOY_SCARED_OF_ENTRANCE, 0);
  Event_End();
}

/// A strongman asks whether the party has been to Imil. He's in Bilibin once
/// the curse is lifted, and asks whether the party has been there, up north,
/// and seen its lighthouse.
static void Bilibin_AskAboutImil(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinHaveEverBeenVillageImil);
  Event_AskYesNoAndShowResponse(ACTOR_STRONGMAN_IMIL_TOWER, 0);
  Event_End();
}

/// Walking into one of Bilibin's doors: everyone stops, the door opens and the
/// leader walks through it. The doors' step triggers, 3 to 9, are the numbers
/// of the exits they take: BILIBIN_TO_INN to BILIBIN_TO_WEAPON_AND_ARMOR_SHOP
/// lead into Bilibin's buildings, and BILIBIN_TO_SANCTUM into the Bilibin
/// Sanctum, whose door sounds and is stepped through differently.
static void Bilibin_OpenDoorAndLeave(void) {

  u32 actorSlot;
  struct Actor *actor;
  s32 exitIndex;
  u8 *originBytes;
  s32 originOffset;
  s32 rowOffset;
  s32 column;
  s32 row;
  struct FieldWork *work;

  work = gFieldWork;
  Event_Start();
  // Freeze placed actors before the exit cells start animating.
  for (actorSlot = ACTOR_FIRST_PLACED; actorSlot < ACTOR_ID_COUNT;
       actorSlot++) {
    actor = Actor_GetById(actorSlot);
    if (actor != 0) {
      actor->motionFlags = 0;
    }
  }
  exitIndex = (((u16)work->stepTrigger - BILIBIN_TO_INN) << 16) >> 16;
  if (exitIndex == BILIBIN_TO_SANCTUM - BILIBIN_TO_INN) {
    Event_PlaySong(SE_THUMP);
  } else {
    Sound_PlaySong(SE_DOOR_OPEN);
  }
  // Each door has its cell and the steps that draw it opening.
  originOffset = exitIndex << 2;
  originBytes = (u8 *)gBilibin_DoorCells;
  column = *(s16 *)(originBytes + originOffset);
  rowOffset = originOffset + 2;
  row = *(s16 *)(originBytes + rowOffset);
  MapGrid_PlayMetatileCopySequence(gBilibin_DoorSteps[exitIndex], column, row);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (exitIndex == BILIBIN_TO_SANCTUM - BILIBIN_TO_INN) {
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
  } else {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  }
  if (exitIndex == BILIBIN_TO_ITEM_SHOP - BILIBIN_TO_INN) {
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
  } else {
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  }
  Event_WaitTicks(16);
  Import_EventSetExitRequest(exitIndex + BILIBIN_TO_INN);
  Event_End();
}

/// Move on Jill's tree in the river at the Bilibin Barricade, until she is
/// rescued: once Move has brought her to cell column 6, by the bank, the leader
/// pulls her ashore in three tugs, and she thanks the party for saving her from
/// being washed downstream.
static void Bilibin_PullJillFromRiver(void) {
  struct Actor *leader;
  struct Actor *jillTree;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  jillTree = Actor_GetById(ACTOR_JILL_TREE);
  if ((jillTree->x.fixed >> 20) == 6) {
    Event_Start();
    Actor_SetSpritePriority(ACTOR_JILL_TREE, 1);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_WaitTicks(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
    Actor_SetSpeed(ACTOR_JILL_TREE, PIXELS(1) / 5, PIXELS(1) / 10);
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    jillTree->motionFlags = 0;
    // Each tug: the leader faces about, strains and draws her in, then steps
    // on.
    leader->scaleX = -Q16_ONE;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
    Actor_MoveToAndWait(ACTOR_JILL_TREE, 111, 196);
    leader->scaleX = Q16_ONE;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 185);
    Event_WaitTicks(20);
    leader->scaleX = -Q16_ONE;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
    Actor_MoveToAndWait(ACTOR_JILL_TREE, 121, 190);
    leader->scaleX = Q16_ONE;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 141, 189);
    Event_WaitTicks(20);
    leader->scaleX = -Q16_ONE;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
    Actor_MoveToAndWait(ACTOR_JILL_TREE, 132, 186);
    leader->scaleX = Q16_ONE;
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 166, 185);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    Actor_SetSpritePriority(ACTOR_JILL_TREE, 2);
    FieldPsynergy_StartMindReadWave(0, ACTOR_JILL_TREE);
    WaitTicks(10);
    Event_SetMessageId(MsgBilibinThankSavedMeFromBeing);
    Event_ShowMessage(ACTOR_JILL_TREE, 0);
    FieldPsynergy_FinishWaveAndReleaseHeapBlock();
    WaitTicks(10);
    GameFlag_Set(FLAG_BILIBIN_JILL_RESCUED);
    Event_End();
  }
}

/// Walking into the statue in the Bilibin Treasury or the crates at the
/// Bilibin Barricade: the leader jumps forward onto them where he can
/// (Field_TryJumpForward).
static void Bilibin_JumpOntoStatueOrCrates(void) { Field_TryJumpForward(); }

/// Jill and her friends thank the leader with a Hard Nut. It happens on
/// stepping up to them on the bank at the Bilibin Barricade, once the curse is
/// lifted and if she was pulled from the river: they recognize the leader, and
/// she gives him the Hard Nut in thanks.
/// "Remember me? I was that tree!"
static void Bilibin_JillGivesHardNut(void) {
  struct Actor *leader;
  struct Actor *eastMember;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0 ||
      GameFlag_Get(FLAG_BILIBIN_JILL_RESCUED) == 0)
    return;

  Event_Start();
  // The camera moves over to the three; the friend nearer the leader speaks
  // first.
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(263), -1, PIXELS(173), 1);
  FieldCamera_WaitForArrival();
  eastMember = Actor_Get(ACTOR_GANG_MEMBER_EAST);
  if (eastMember->x.fixed > leader->x.fixed) {
    Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTH + FACING_STEP, 20);
    Actor_ShowEmote(ACTOR_GANG_MEMBER_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    20);
    Event_SetMessageId(MsgBilibinYoureGuy);
    FieldEvent_ShowMessageThenWait(ACTOR_GANG_MEMBER_WEST, 0, 10);
    Actor_ShowEmote(ACTOR_GANG_MEMBER_EAST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
  } else {
    Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTHEAST + FACING_STEP,
                        20);
    Actor_ShowEmote(ACTOR_GANG_MEMBER_EAST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    20);
    Event_SetMessageId(MsgBilibinYoureGuy);
    FieldEvent_ShowMessageThenWait(ACTOR_GANG_MEMBER_EAST, 0, 10);
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
  Actor_RunRepeatedMotion(ACTOR_GANG_MEMBER_WEST, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_GANG_MEMBER_WEST, 0, 10);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JILL, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
  Actor_SetAttachedEffect(ACTOR_JILL,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitTicks(40);
  Actor_FaceDirection(ACTOR_JILL, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTHEAST + FACING_STEP,
                      10);
  Actor_RunRepeatedMotion(ACTOR_JILL, 1);
  FieldEvent_ShowMessageThenWait(ACTOR_JILL, 0, 10);
  Import_ActorSetAnimation(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GANG_MEMBER_WEST, ANIM_NOD);
  Event_WaitTicks(20);
  Event_ShowMessage(ACTOR_JILL, 0);
  Actor_SetSpeed(ACTOR_JILL, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_GetById(ACTOR_JILL)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JILL, 266, 172);
  Event_WaitTicks(1);
  Actor_GetById(ACTOR_JILL)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_WaitTicks(10);
  Actor_SetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_JILL, 0, 10);
  // Show the named gift before adding it to the party's inventory.
  FieldMessage_Show(MsgBilibinJillGaveIsaacSpecialGift, 1);
  gFieldWork->message++;
  ItemDisplay_ShowItem(ITEM_HARD_NUT, 3);
  PartyInventory_GiveItem(ITEM_HARD_NUT, 0);
  Actor_SetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_SetSpeed(ACTOR_JILL, PIXELS(1), PIXELS(1) / 2);
  Actor_GetById(ACTOR_JILL)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_JILL, 262, 156);
  Event_WaitTicks(1);
  {
    struct Actor *jill = Actor_Get(ACTOR_JILL);
    u8 steeringFlags = jill->steeringFlags | ACTOR_FACES_MOTION;

    jill->steeringFlags = steeringFlags;
  }
  Event_WaitTicks(20);
  Actor_RunRepeatedMotion(ACTOR_GANG_MEMBER_EAST, 2);
  FieldEvent_ShowMessageThenWait(ACTOR_GANG_MEMBER_EAST, 0, 10);
  Import_ActorSetAnimation(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GANG_MEMBER_WEST, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
  // From now on the three turn to face the leader wherever he goes.
  FieldActor_SetTargetAndCallback(ACTOR_GANG_MEMBER_EAST,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                  gBilibin_FaceLeaderScript);
  FieldActor_SetTargetAndCallback(ACTOR_GANG_MEMBER_WEST,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                  gBilibin_FaceLeaderScript);
  FieldActor_SetTargetAndCallback(ACTOR_JILL,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                  gBilibin_FaceLeaderScript);
  GameFlag_Set(FLAG_BILIBIN_JILL_GIFT_RECEIVED);
  Event_End();
}

/// Sets up the scene being entered, Bilibin, the Bilibin Barricade or the
/// Bilibin Treasury: its actors by story flags, its metatile behaviors
/// and its opening transition.
s32 Bilibin_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gPersistentState.sceneId == (s32)&SceneId_Bilibin) {
    Bilibin_RestoreVillageActors();
  } else if (gPersistentState.sceneId == (s32)&SceneId_BilibinBarricade) {
    Bilibin_RestoreRiverActors();
    Task_AddFrame(Bilibin_UpdateWadingRipples, TASK_PASS_FRAME);
  } else if (gPersistentState.sceneId == (s32)&SceneId_BilibinTreasury) {
    Bilibin_SetUpTreasury();
  }
  return 0;
}

/// Sets up Bilibin: the tree or the man by FLAG_KOLIMA_CURSE_LIFTED, the ivy
/// and the statue by the entrance used, and area temp flag
/// FLAG_BILIBIN_KEEPING_MCCOY_WAITING for McCoy's garden guard.
static void Bilibin_RestoreVillageActors(void) {

  struct Actor *actor;
  s16 entrance;

  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Import_ActorSetPosition(ACTOR_TREE_MAN, 0, 0);
    Actor_FaceDirection(ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST,
                        FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_OLD_WOMAN_ROAD_BLOCKED,
                        FACING_SOUTH + FACING_STEP, 0);
  } else {
    actor = Actor_Get(ACTOR_TREE_MAN);
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetPosition(ACTOR_FORMER_TREE_MAN, 0, 0);
  }
  // Apply arrival-specific changes after restoring the population.
  actor = Actor_Get(ACTOR_IVY);
  actor->scaleY = 3 * Q16_ONE / 2;
  entrance = gPersistentState.entrance;
  if (entrance == BILIBIN_FROM_BILIBIN_TREASURY_WEST) {
    Actor_SetPosition(ACTOR_IVY, 0, 0);
  } else if (entrance == BILIBIN_FROM_SANCTUM) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    entrance = gPersistentState.entrance;
    if (entrance == BILIBIN_FROM_BILIBIN_TREASURY_EAST) {
      Actor_SetPosition(ACTOR_VILLAGE_STATUE, PIXELS(248), PIXELS(216));
    }
  }
  Bilibin_UpdateVillageStatueCells();
  if (GameFlag_Get(FLAG_MCCOY_PALACE_ADMITTED) != 0 &&
      GameFlag_Get(FLAG_MCCOY_PALACE_VISITED) == 0) {
    GameFlag_Set(FLAG_BILIBIN_KEEPING_MCCOY_WAITING);
  }
}

/// Updates the cells around the statue in Bilibin after it moves. It runs after
/// Move, a push, or entry: it updates the metatile behaviors of the statue's
/// lane, opens its home cell (16, 13) once it has left it, and moves the leader
/// clear if it has landed on him.
static void Bilibin_UpdateVillageStatueCells(void) {
  s32 leaderRow;
  s32 leaderColumn;
  struct Actor *leader;
  struct Actor *statue;
  s32 statueX;
  s32 statueRow;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  statue = Actor_GetById(ACTOR_VILLAGE_STATUE);
  statueRow = statue->z.fixed >> 20;
  leaderColumn = (leader->x.fixed >> 20);
  leaderRow = (leader->z.fixed >> 20);
  statueX = statue->x.fixed;
  // Repaint the statue's lane before clearing the cells it currently occupies.
  MapGrid_CopyCellAttributes(15, 11, 3, 1, 15, 12);
  MapGrid_CopyCellAttributes(15, 11, 3, 1, 15, 13);
  MapGrid_CopyCellAttributes(15, 11, 3, 1, 15, 14);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, (statueX >> 20), statueRow);
  if ((statueX >> 20) != 16 || statueRow != 13) {
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 16, 13);
  }
  // The statue may have moved onto the leader while this event was running.
  if (leaderColumn == 16) {
    if (leaderRow == 13) {
      Event_Start();
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      20);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
      Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
      if (statueRow == 13) {
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

/// Sets up the Bilibin Barricade: before the curse is lifted, Jill and her
/// friends are trees here, stacked or, once Saturos is beaten at Mercury
/// Lighthouse, pushed down; afterwards they are children again on the bank.
static void Bilibin_RestoreRiverActors(void) {
  struct Actor *jillTree;
  s32 populationState;
  struct Actor *fallenTree;
  struct Actor *actor;
  s32 crossingState;

  fallenTree = Actor_GetById(ACTOR_MIDDLE_TREE);
  jillTree = Actor_GetById(ACTOR_JILL_TREE);
  actor = Actor_Get(ACTOR_BARRICADE_CRATES);
  Actor_SetSpriteFlags(actor, 0);
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
        Import_ActorSetPosition(ACTOR_JILL, 0, 0);
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
    Actor_SetSpriteFlags(actor, 0);
    actor = Actor_Get(ACTOR_MIDDLE_TREE);
    Actor_SetSpriteFlags(actor, 0);
    actor = Actor_Get(ACTOR_JILL_TREE);
    Actor_SetSpriteFlags(actor, 0);
    fallenTree->motionFlags = populationState;
    // Once FLAG_SATUROS_BEATEN_AT_MERCURY is set ("They crossed the river
    // after pushing us down"), the middle tree lies on its side and Jill is in
    // the water, or on the bank where she was pulled ashore.
    crossingState = GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY);
    if (crossingState != 0) {
      Actor_GetById(ACTOR_BOTTOM_TREE)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
      Actor_GetById(ACTOR_FALLEN_TREE_STAND_IN)->collisionFlags |=
          ACTOR_NEAR_PSYNERGY;
      Actor_GetById(ACTOR_JILL_TREE)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
      Actor_SetPosition(ACTOR_FALLEN_TREE_STAND_IN, PIXELS(142), PIXELS(156));
      actor = Actor_Get(ACTOR_FALLEN_TREE_STAND_IN);
      Actor_SetSpriteFlags(actor, 0);
      Actor_SetPosition(ACTOR_MIDDLE_TREE, PIXELS(142), PIXELS(156));
      fallenTree->sprite->rotation = 0x4000;
      fallenTree->y.fixed += -PIXELS(8);
      if (GameFlag_Get(FLAG_BILIBIN_JILL_RESCUED) != 0) {
        Actor_SetPosition(ACTOR_JILL_TREE, PIXELS(132), PIXELS(186));
      } else {
        Actor_SetPosition(ACTOR_JILL_TREE, PIXELS(88), PIXELS(196));
        Actor_SetSpritePriority(ACTOR_JILL_TREE, 3);
        jillTree->collisionFlags |= ACTOR_NEAR_TALK;
      }
    } else {
      fallenTree->y.fixed = PIXELS(32);
      jillTree->motionFlags = crossingState;
      jillTree->y.fixed = PIXELS(48);
    }
  }
  Bilibin_UpdateCratesCells();
}

/// After the crates at the Bilibin Barricade move (Move, a push, or entry):
/// restores the ten cells of their lane, then marks the cell they stand on.
static void Bilibin_UpdateCratesCells(void) {
  struct Actor *actor;
  s32 cratesColumn;
  s32 cratesRow;
  u32 cellOffset;

  actor = Actor_GetById(ACTOR_BARRICADE_CRATES);
  cratesColumn = actor->x.fixed >> 20;
  cratesRow = actor->z.fixed >> 20;

  // The packed table stores ten byte-sized column/row pairs.
  for (cellOffset = 0; cellOffset < 20; cellOffset += 2) {
    s32 column = ((u8 *)gBilibin_CratesLaneCells)[cellOffset];
    s32 row = ((u8 *)gBilibin_CratesLaneCells)[cellOffset + 1];
    MapGrid_CopyCellAttributes(1, 0, 1, 1, column, row);
  }

  MapGrid_CopyCellAttributes(0, 0, 1, 1, cratesColumn, cratesRow);
}

/// Sets up the Bilibin Treasury: its statue's cells, and the leader
/// arriving by BILIBIN_TREASURY_UPPER_FROM_LOWER or
/// BILIBIN_TREASURY_FROM_BILIBIN_NORTH starts 16 pixels above the floor.
static void Bilibin_SetUpTreasury(void) {
  struct Actor *leader;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Bilibin_UpdateTreasuryStatueCells();
  if (gPersistentState.entrance == BILIBIN_TREASURY_UPPER_FROM_LOWER ||
      gPersistentState.entrance == BILIBIN_TREASURY_FROM_BILIBIN_NORTH) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      Event_Start();
      leader->y.fixed = PIXELS(16);
      FieldCamera_MoveWithinMapBounds(leader->x.fixed, leader->y.fixed,
                                      leader->z.fixed, 0);
      MapGrid_CenterOnFollowedPoint();
      Event_End();
      WaitTicks(1);
    }
  }
}

#include "common/actor_findcell.h"

/// Pushing a statue in Bilibin or the Bilibin Treasury, or the crates at the
/// Bilibin Barricade: it slides one cell ahead with the leader behind it, if
/// the cell is free.
static void Bilibin_PushStatueOrCrates(void) {
  struct Actor *leader;
  struct Actor *pushed;
  u32 direction;
  struct FieldPosition destination;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  direction = leader->facing >> 12;
  pushed = Actor_FindAtCell(
      (leader->x.part.pixel + (gBilibin_FacingCellSteps[direction] >> 16)) >> 4,
      (leader->z.part.pixel + (s16)gBilibin_FacingCellSteps[direction]) >> 4);
  if (pushed != NULL) {
    pushed->layer = 2;
    destination.x =
        pushed->x.fixed + (gBilibin_FacingCellSteps[direction] & -0x10000);
    destination.y = pushed->y.fixed;
    destination.z =
        pushed->z.fixed + (gBilibin_FacingCellSteps[direction] << 16);
    if (FieldActor_CheckMovementCollision(pushed, &destination) <= 0) {
      // The leader leans on it, then both move together.
      Actor_SelectAnimation(leader, ANIM_PUSH);
      WaitTicks(15);
      Sound_PlaySong(SE_PUSH);
      pushed->speedLimit = PIXELS(1) / 5;
      pushed->acceleration = PIXELS(1) / 5;
      FieldActor_SetMoveTarget(pushed, destination.x, destination.y,
                               destination.z);
      leader->speedLimit = PIXELS(1) / 5;
      leader->acceleration = PIXELS(1) / 5;
      FieldActor_SetMoveTarget(leader, destination.x, destination.y,
                               destination.z);
      Actor_WaitForMove(pushed);
      pushed->x.fixed = destination.x;
      pushed->z.fixed = destination.z;
      pushed->velocityX = 0;
      pushed->velocityZ = 0;
      Actor_SelectAnimation(leader, ANIM_STAND);
      // Each scene derives its metatile behaviors from this same moved
      // actor.
      if (gPersistentState.sceneId == (s32)&SceneId_BilibinBarricade)
        Bilibin_UpdateCratesCells();
      else if (gPersistentState.sceneId == (s32)&SceneId_Bilibin)
        Bilibin_UpdateVillageStatueCells();
      else if (gPersistentState.sceneId == (s32)&SceneId_BilibinTreasury)
        Bilibin_UpdateTreasuryStatueCells();
    }
  }
}

/// Runs every frame at the Bilibin Barricade: while whoever the player
/// controls wades in the river, west of x 142 and under 8 pixels up, a ripple
/// spreads from their feet every 30 frames.
static void Bilibin_UpdateWadingRipples(void) {
  struct Actor *actor;

  actor = Actor_GetById(gPersistentState.leaderActorId);
  if (actor->x.fixed < PIXELS(142)) {
    if (actor->y.fixed < PIXELS(8)) {
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

  Sprite_StartAnim(sprite, 2);
  sprite->flags = 0;
  sprite->image.objectMode = 1;
  sprite->image.priority = 3;
}

/// After the statue in the Bilibin Treasury moves (Move, a push, or entry):
/// updates the two cells it can stand on, rows 6 and 9 of its column.
static void Bilibin_UpdateTreasuryStatueCells(void) {
  struct Actor *statue;

  statue = Actor_Get(ACTOR_TREASURY_STATUE);
  if (statue == NULL)
    return;

  if (statue->z.fixed >> 20 == 6)
    MapGrid_CopyCellAttributes(2, 0, 1, 1, 14, 6);
  else
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 14, 6);

  if (statue->z.fixed >> 20 == 9)
    MapGrid_CopyCellAttributes(2, 0, 1, 1, 14, 9);
  else
    MapGrid_CopyCellAttributes(1, 0, 1, 1, 14, 9);
}

s32 gBilibin_FaceLeaderScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Unused: the entrances for a scene other than the three, which never
/// loads this overlay. Its words read as a MAPGRID_ANIMATION_STEP list, not as
/// entrances.
u16 gBilibinEntrancesOther[] = {
    MAPGRID_ANIMATION_STEP(0, 65535, 168, 0, 168),
    MAPGRID_ANIMATION_STEP(16384, 0, 65535, 65535, 65535),
    MAPGRID_ANIMATION_STEPS_END,
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

struct ActorTemplate gBilibinActorTemplatesOther[] = {
    { SCENE_TABLE_END },
};

struct ActorTemplate gBilibinActorTemplates[] = {
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(40), 0, PIXELS(140), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(256), 0, PIXELS(440), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(328), FACING_SOUTHWEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(240), FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(176), 0, PIXELS(328), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(128), 0, PIXELS(224), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(176), 0, PIXELS(96), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(96), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(208), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(208), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(232), 0, PIXELS(296), FACING_SOUTHWEST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_GOLDEN_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(264), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(432), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gBilibinBarricadeActorTemplates[] = {
    { SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(300), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(282), 0, PIXELS(170), FACING_WEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(246), 0, PIXELS(170), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(262), 0, PIXELS(156), FACING_WEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK_32, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gBilibinTreasuryActorTemplates[] = {
    { SPRITE_GOLDEN_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(100), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { JUPITER_DJINNI, FLAG_DJINNI(DJINNI_GUST), MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gBilibinTriggersOther[] = {
    { TRIGGERS_END },
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
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, 0xf50, FIND_ITEM(ITEM_HERB) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, 0xf51, FIND_COINS(5) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 90, CONDITION_ALWAYS, EVENT(Bilibin_InspectWell) },
    { TRIGGERS_END },
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
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 11, CONDITION_ALWAYS, EVENT(Bilibin_ReadRestrictedAreaSign) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 30, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldDoorTightlyLocked)) },
    { TRIGGER_STEP, 13, FLAG_BILIBIN_JILL_GIFT_RECEIVED, EVENT(Bilibin_JillGivesHardNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_JILL_TREE, FLAG_BILIBIN_JILL_RESCUED, EVENT(Bilibin_PullJillFromRiver) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(Bilibin_JumpOntoStatueOrCrates) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT(Bilibin_JumpOntoStatueOrCrates) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT(Bilibin_PushStatueOrCrates) },
    { STEP_TRIGGER_WALKING, 12, CONDITION_ALWAYS, EVENT(Bilibin_PushStatueOrCrates) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BARRICADE_CRATES, CONDITION_ALWAYS, EVENT(Bilibin_UpdateCratesCells) },
    { TRIGGERS_END },
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
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT(Bilibin_JumpOntoStatueOrCrates) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT(Bilibin_JumpOntoStatueOrCrates) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT(Bilibin_JumpOntoStatueOrCrates) },
    { TRIGGERS_END },
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

/// A door's cell: its column in the low halfword, its row in the high one.
#define DOOR_CELL(column, row) (((row) << 16) | (column))

/// Each door's cell, in the order of gBilibin_DoorSteps.
u32 gBilibin_DoorCells[] = {
    DOOR_CELL(54, 20), DOOR_CELL(43, 18), DOOR_CELL(37, 19), DOOR_CELL(41, 11),
    DOOR_CELL(35, 11), DOOR_CELL(49, 9),  DOOR_CELL(55, 6),
};

/// Two cells as the bytes column, row, column, row.
#define CELL_PAIR(firstColumn, firstRow, secondColumn, secondRow) \
  (((u32)(secondRow) << 24) | ((secondColumn) << 16) | ((firstRow) << 8) | \
   (firstColumn))

/// The ten cells of the crates' lane at the Bilibin Barricade.
u32 gBilibin_CratesLaneCells[] = {
    CELL_PAIR(8, 18, 9, 18),  CELL_PAIR(10, 18, 8, 19),
    CELL_PAIR(9, 19, 10, 19), CELL_PAIR(11, 19, 9, 20),
    CELL_PAIR(10, 20, 11, 20),
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

s32 gBilibin_RippleScript[] = {
    SC_WAIT(30),
    SC_RELEASE,
};
