/// @file
/// Kolima's overlay, which holds three places, each a scene of its own. The
/// village's houses and shops are another overlay (kolima_interiors.c), and so
/// is each map of Kolima Forest.
///
/// - Kolima (SceneId_Kolima), the village of lumberjacks, to which Lord
///   McCoy has barricaded the road from Bilibin since the trouble began. The
///   party's first entry from the world map (KOLIMA_FROM_WORLD_MAP) plays
///   Kolima_WitnessForestCurse, unless the party has already seen it in Kolima
///   Forest (FLAG_KOLIMA_CURSE_WITNESSED): every villager has been turned into
///   a tree, sparkles fall from the sky and press the party down until domes
///   of their own Psynergy shield them, and two unseen voices speak, Tret and
///   Laurel, the spirits of the holy trees, who send the party to look for
///   Tret deep within Kolima Forest.
///
///   Until the curse is lifted in Kolima Forest (FLAG_KOLIMA_CURSE_LIFTED),
///   Kolima_GetPlacements gives the nine villagers (ACTOR_FIRST_VILLAGER to
///   ACTOR_LAST_VILLAGER) tree sprites that stand still, sparkles twinkle on
///   the ground (ACTOR_FIRST_SPARKLE and the three after it, and five more that
///   Kolima_Initialize places), and the villagers answer only Mind Read, with
///   the thoughts of people stuck as trees. Afterwards they are people again
///   and talk of the curse, the holy tree and McCoy's money, and the healer
///   among them (ACTOR_HEALER) offers the Sanctum's services across his
///   counter. Granite, a Venus Djinni (ACTOR_GRANITE), waits in the village
///   and joins when spoken to. Five doorways (KOLIMA_TO_INN to
///   KOLIMA_TO_HOUSE_NORTH) lead into the houses and shops
///   (SceneId_KolimaInteriors), KOLIMA_TO_WORLD_MAP and
///   KOLIMA_TO_WORLD_MAP_EAST to the world map and KOLIMA_TO_KOLIMA_TUNNEL
///   into Kolima Tunnel.
/// - Kolima Tunnel (SceneId_KolimaTunnel), a passage with no one in it from
///   one of the houses (KOLIMA_INTERIORS_FROM_KOLIMA_TUNNEL) to the village
///   (KOLIMA_FROM_KOLIMA_TUNNEL). It shares the village's event table, whose
///   exits KOLIMA_TO_WORLD_MAP and KOLIMA_TO_WORLD_MAP_EAST are, by the same
///   numbers, its two ways out (KOLIMA_TUNNEL_TO_KOLIMA_INTERIORS and
///   KOLIMA_TUNNEL_TO_KOLIMA). That table's two jars, an Apple and an Herb
///   (search events 100 and 101), are in the village: only Kolima's map
///   (map_037's markers.tsv) places them, at cells (23, 8) and (13, 23).
/// - Kolima Bridge (SceneId_KolimaBridge), "the grand bridge across the
///   river" and "the pride of Kolima" to the village's armor seller, with the
///   world map at both ends. It is a drawbridge, standing raised between its
///   posts. A villager stands beside it, a tree while the curse lasts
///   (ACTOR_BRIDGE_VILLAGER_AS_TREE) and a man afterwards
///   (ACTOR_BRIDGE_VILLAGER). The first time the party then steps up to the
///   bridge (trigger 10), Kolima_LowerBridge plays: he asks whether they are
///   going to cross, hops aside and lets the drawbridge down across the river
///   in front of the leader (FLAG_KOLIMA_BRIDGE_LOWERED; its recording,
///   places/kolima-bridge/events/Kolima_LowerBridge.gif). From then on
///   he says that for a while he thought no one would ever cross it again,
///   Kolima_RestoreBridgeState draws it down on every later visit, and the
///   bridge on the world map is drawn down with the same flag.
///
/// Open questions: none.

#include "script.h"
#include "djinn_ids.h"
#include "flags.h"
#include "character_ids.h"
#include "game_state.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "unit/scene/kolima.h"
#include "unit/scene/common/kolima_forest_bridge_falling_effect.h"
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
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "kolima.h"
#include "object_runtime.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "field_runtime.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "item_ids.h"

static void Kolima_WitnessForestCurse(void);
static void Kolima_ShowMessageAndWait(s32 actorId, s32 waitFrames);
static void Kolima_FaceActorAndWait(s32 actorId, s32 facing, s32 waitFrames);
static void Kolima_SpawnCurseSparkle(void);
static void Kolima_RestoreBridgeState(void);

/// Actors of Kolima, numbering the rows of gKolimaPlacements from
/// ACTOR_FIRST_PLACED: Garet, Ivan and Mia, who stand by for the curse, take
/// their own actor slots. The nine villagers are named by what they say once
/// the curse is lifted.
enum KolimaActor {
  /// A young man with brown hair.
  /// ★ Named from the game: "This is Kolima, village of lumberjacks!"
  /// (MsgKolimaThisIsKolimaVillageOfLumberjacks)
  ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS = ACTOR_FIRST_PLACED,
  /// A woman with fair hair, whose mind Ivan reads in
  /// Kolima_WitnessForestCurse.
  /// ★ Named from the game: "We rely on the forests so much, and yet we hurt
  /// the sacred tree..." (MsgKolimaWeRelyOnTheForestsSo)
  ACTOR_VILLAGER_RELYING_ON_FORESTS,
  /// A burly grey-haired man.
  /// ★ Named from the game: "You know, being a tree gives you a lot of time to
  /// think." (MsgKolimaYouKnowBeingATreeGives)
  ACTOR_VILLAGER_TIME_TO_THINK,
  /// A woman in a green dress.
  /// ★ Named from the game: "I turned into a tree right when the sparkles fell
  /// from the sky." (MsgKolimaITurnedIntoATreeRight)
  ACTOR_VILLAGER_SPARKLES_FELL,
  /// An old man with a cane.
  /// ★ Named from the game: "Don't blame the villagers for being tempted by
  /// McCoy's money..." (MsgKolimaDontBlameTheVillagersForBeing)
  ACTOR_OLD_MAN_MCCOYS_MONEY,
  /// An old woman.
  /// ★ Named from the game: "Who would try to use the holy tree to build a
  /// palace, anyway?" (MsgKolimaWhoWouldTryToUseThe)
  ACTOR_OLD_WOMAN_HOLY_TREE_PALACE,
  /// A small boy with fair hair.
  /// ★ Named from the game: "I didn't mind being a tree... Water never tasted
  /// so good!" (MsgKolimaIDidntMindBeingATree)
  ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE,
  /// A little girl with pigtails.
  /// ★ Named from the game: "Yawn... Trees are so... sleepy..."
  /// (MsgKolimaYawnTreesAreSoSleepy)
  ACTOR_GIRL_SLEEPY_TREES,
  /// A man with a white beard in a tall hat and a long robe; opens the
  /// Sanctum's services across his counter (Engine_SanctumOpen).
  /// ★ Named from the game: "Maybe if I meditate harder, I'll turn back into a
  /// man..." (MsgKolimaMaybeIfIMeditateHarderIll)
  ACTOR_HEALER,
  /// Tret's wrathful side, unseen while the curse falls.
  /// ★ Named from the game: "Tret was once a kindly forest king."
  /// (MsgKolimaWatchOutItsHappeningAgain)
  ACTOR_TRET,
  /// Unseen while the curse falls.
  /// ★ Named from the game: "We are not the only ones who will die, Laurel."
  /// (MsgKolimaWatchOutItsHappeningAgain)
  ACTOR_LAUREL,
  /// The fields of Psynergy that rise over the party while the curse falls
  /// ("You know what those fields were?"), each set where its character
  /// stands in Kolima_WitnessForestCurse; Mia's only with her in the party.
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  ACTOR_LEADERS_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  ACTOR_IVANS_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  ACTOR_MIAS_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  ACTOR_GARETS_PSYNERGY_FIELD,
  /// The four sparkles on the ground while the curse lasts
  /// (gKolima_EntryActions).
  /// ★ Named from the game: "I turned into a tree right when the sparkles
  /// fell from the sky." (MsgKolimaITurnedIntoATreeRight)
  ACTOR_FIRST_SPARKLE,
  /// Granite, a Venus Djinni, placed until it has joined.
  /// ★ Named from the game: "Granite" (MsgDjinnName)
  ACTOR_GRANITE = ACTOR_FIRST_SPARKLE + 4,
  /// ★ Named from the game: "I need to help evacuate the other villagers."
  /// (MsgValeKyleAbleStop)
  ACTOR_FIRST_VILLAGER = ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS,
  /// ★ Named from the game: "I need to help evacuate the other villagers."
  /// (MsgValeKyleAbleStop)
  ACTOR_LAST_VILLAGER = ACTOR_HEALER
};

/// Kolima Bridge's villager, numbering the rows of gKolimaBridgePlacements from
/// ACTOR_FIRST_PLACED: the same villager as a man and as a tree, in the same
/// place.
enum KolimaBridgeActor {
  /// A burly grey-haired man.
  /// ★ Named from the game: "For a while, I thought no one would ever cross
  /// this bridge again." (MsgKolimaForAWhileIThoughtNo)
  ACTOR_BRIDGE_VILLAGER = ACTOR_FIRST_PLACED,
  /// The same villager as a tree, while the curse lasts.
  /// ★ Named from the game: "For a while, I thought no one would ever cross
  /// this bridge again." (MsgKolimaForAWhileIThoughtNo)
  ACTOR_BRIDGE_VILLAGER_AS_TREE
};

/// Applies the deferred pose change requested by the falling-effect script.
static s32 Kolima_ApplyPendingPose(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if (gKolima_PendingPose != 0) {
    Object_SetMode(actor, 2);
    gKolima_PendingPose = 0;
  }
  return 1;
}

/// Each frame of a sparkle on Kolima's ground: its timer runs on at random, and
/// it shows its bright frame (7) past 1000 and its dim one (10) before; at
/// 1200 the timer starts over.
static s32 Kolima_UpdateTimedAppearance(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s32 randomValue = Engine_RandomNext();
  s32 scaledRandom = randomValue * 100;
  s32 appearanceTimer = (u16)actor->timer + ((u32)scaledRandom >> 16);
  actor->timer = appearanceTimer;
  if ((s16)appearanceTimer > 1000) {
    ObjectGroup_SetChildValue(actor, 7);
  } else {
    ObjectGroup_SetChildValue(actor, 10);
  }
  if ((s16)actor->timer > 1200) {
    actor->timer = 0;
  }
  return 1;
}

/// While the curse lasts, gives Kolima's nine villagers (placement rows 3 to
/// 11, ACTOR_FIRST_VILLAGER to ACTOR_LAST_VILLAGER) tree sprites, SPRITE_TREE
/// or, for rows 9 and 10 (ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE and
/// ACTOR_GIRL_SLEEPY_TREES), SPRITE_SMALL_TREE; they stand still and keep their
/// facing when spoken to.
static void
Kolima_InitializeCursedVillagers(struct ScenePlacement *placements) {
  struct ScenePlacement *villager = &placements[3];
  u32 villagerIndex;
  s32 treeSprite;
  s32 alternateTreeSprite;

  villagerIndex = 0;
  treeSprite = SPRITE_TREE;
  alternateTreeSprite = SPRITE_SMALL_TREE;
  for (; villagerIndex <= 8; villagerIndex++) {
    villager->sprite = treeSprite;
    if ((villagerIndex - 6) <= 1) {
      villager->sprite = alternateTreeSprite;
    }
    villager->talkFacing = TALK_KEEP_FACING;
    villager->behavior = ACTOR_STAND;
    villager++;
  }
}

/// Each frame of a piece of Kolima Bridge as it falls: it moves, grows, and is
/// released when its timer runs out.
static s32 Kolima_UpdateFallingDebris(struct ScriptInterpreter *interpreter) {
  struct FieldActor *debris = (struct FieldActor *)interpreter;
  debris->x.fixed += debris->velocityX;
  debris->z.fixed += debris->velocityZ;
  debris->velocityZ -= PIXELS(1) / 25;
  debris->scaleX += 3 * Q16_ONE / 128;
  debris->scaleY += 3 * Q16_ONE / 128;
  {
    s32 framesLeft = (u16)debris->timer - 1;
    debris->timer = framesLeft;
    if ((u16)framesLeft == 0) {
      Engine_ObjectDispatchRelease(debris);
    }
  }
  return 1;
}

#include "common/object_facing.h"

/// Selects the entrances of Kolima, Kolima Tunnel or Kolima Bridge.
const struct SceneEntrance *Kolima_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KolimaBridge) {
    return gKolimaBridgeEntrances;
  }
  if (scene == (s32)&SceneId_KolimaTunnel) {
    return gKolimaTunnelEntrances;
  }
  return gKolimaEntrances;
}

/// Only Kolima Tunnel has regions: its two ways out,
/// KOLIMA_TUNNEL_TO_KOLIMA_INTERIORS and KOLIMA_TUNNEL_TO_KOLIMA.
const struct SceneRegion *Kolima_GetRegions(void) {
  if (gGameState.scene == (s32)&SceneId_KolimaTunnel) {
    return gKolimaTunnelRegions;
  }
  return 0;
}

/// The exits of all three scenes, each scene's after its header word (an exit
/// of kind 0): Kolima's to the world map, the houses (SceneId_KolimaInteriors)
/// and Kolima
/// Tunnel, Kolima Bridge's to the world map, and Kolima Tunnel's to a house
/// and the village.
const u32 *Kolima_GetExits(void) { return gKolimaExits; }

/// Selects the people of Kolima or Kolima Bridge; Kolima Tunnel has none. In
/// Kolima the villagers are trees until the curse is lifted.
const struct ScenePlacement *Kolima_GetPlacements(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Kolima) {
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Kolima_InitializeCursedVillagers(gKolimaPlacements);
    }
    return gKolimaPlacements;
  }
  if (scene == (s32)&SceneId_KolimaBridge) {
    return gKolimaBridgePlacements;
  }
  return gKolimaTunnelPlacements;
}

/// Kolima, once the curse is lifted: talking to the healer (ACTOR_HEALER)
/// across his counter (facing north) opens the Sanctum's services; from any
/// other side he asks whether the holy tree's rage was the only cause of the
/// curse, and either answer agrees that something changed when the gems fell.
static void Kolima_TalkToHealer(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 facing = leader->facing;
  Engine_EventStart();
  if (FACING_IS_NORTH(facing)) {
    Engine_SanctumOpen(ACTOR_HEALER);
  } else {
    Engine_EventSetMessage(MsgKolimaToldHolyTrees);
    Engine_EventAskYesNo(ACTOR_HEALER, 0);
  }
  Engine_EventEnd();
}

/// Kolima: talking to Granite, a Venus Djinni, makes it join the party.
static void Kolima_GraniteJoins(void) {
  Djinn_JoinParty(ACTOR_GRANITE, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GRANITE));
}

/// Selects Kolima Bridge's events, or the table Kolima and Kolima Tunnel share.
const struct SceneEvent *Kolima_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_KolimaBridge) {
    return gKolimaBridgeEvents;
  }
  return gKolimaEvents;
}

/// Walks the leader north through a door of Kolima into the exit of the same
/// number.
static void Kolima_WalkThroughExit(s32 exitId) {
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(exitId);
}

/// Kolima: walking into one of the five doorways into the houses and shops,
/// whose triggers are the numbers of the exits they take (KOLIMA_TO_INN to
/// KOLIMA_TO_HOUSE_NORTH). The door sound plays; the four entered walking
/// north open their door (gKolima_DoorCells) and the leader walks in, while
/// KOLIMA_TO_HOUSE_NORTH, entered walking south, has no door to draw.
static void Kolima_EnterDoor(void) {
  struct EventWork *work = gEventWork;
  s32 x = 0;
  s32 y = 0;

  Engine_EventStart();
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  switch (work->touchedTrigger) {
  case KOLIMA_TO_INN:
    x = 71;
    y = 9;
    break;
  case KOLIMA_TO_ITEM_SHOP:
    x = 73;
    y = 17;
    break;
  case KOLIMA_TO_HOUSE_SOUTH:
    x = 80;
    y = 21;
    break;
  case KOLIMA_TO_WEAPON_AND_ARMOR_SHOP:
    x = 84;
    y = 12;
    break;
  case KOLIMA_TO_HOUSE_NORTH:
    Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 8);
    gEventWork->transitionFrames = 16;
    Engine_EventRequestExit(KOLIMA_TO_HOUSE_NORTH);
    Engine_EventEnd();
    return;
  }
  Engine_MapAnimateCells(gKolima_DoorCells, x, y);
  Kolima_WalkThroughExit(work->touchedTrigger);
  Engine_EventEnd();
}

/// Kolima Bridge, the first time the party steps up to it (trigger 10, until
/// FLAG_KOLIMA_BRIDGE_LOWERED): the villager beside it
/// (ACTOR_BRIDGE_VILLAGER) asks whether they are going to cross, and at the
/// leader's nod says "Go right ahead!" and hops aside to the post. The
/// drawbridge then comes down across the river in front of the leader in four
/// ever quicker steps of map cells, ten puffs of dust fly up where it lands,
/// the screen shakes and the leader jumps back. The last cells copied also
/// open the bridge's collision. From then on the villager keeps turning to
/// face the leader. Sets FLAG_KOLIMA_BRIDGE_LOWERED.
static void Kolima_LowerBridge(void) {
  u32 debrisIndex;

  Engine_EventStart();
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(168), 0, PIXELS(246), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_BRIDGE_VILLAGER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  40);
  Engine_ActorRunRepeatedMotion(ACTOR_BRIDGE_VILLAGER, 2);
  Engine_EventSetMessage(MsgKolimaGoingCrossBridge);
  Engine_EventShowMessage(ACTOR_BRIDGE_VILLAGER, 0);
  Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(168), 0, PIXELS(234), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 174, 0x116);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_BRIDGE_VILLAGER, ANIMATION_NOD);
  Engine_EventShowMessage(ACTOR_BRIDGE_VILLAGER, 0);
  Actor_FaceDirection(ACTOR_BRIDGE_VILLAGER, FACING_WEST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_BRIDGE_VILLAGER, 1);
  Engine_EventWait(20);
  Object_GetById(ACTOR_BRIDGE_VILLAGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_BRIDGE_VILLAGER, PIXELS(2), PIXELS(1));
  Engine_ActorJump(ACTOR_BRIDGE_VILLAGER, 2, 0);
  Engine_ObjectMotionSetPositionAndCommit(ACTOR_BRIDGE_VILLAGER, 224, 197);
  Engine_AudioPlayCue(SE_176);
  Engine_EventWait(10);
  Engine_ObjectMotionSetPositionAndCommit(ACTOR_BRIDGE_VILLAGER, 234, 200);
  Engine_EventWait(10);
  Engine_AudioPlayCue(SE_198);
  Engine_EventWait(30);
  // The drawbridge comes down in ever shorter steps.
  Map_CopyCellsTo(91, 0, 72, 9, 5, 4);
  Engine_EventWait(12);
  Map_CopyCellsTo(91, 4, 72, 9, 5, 4);
  Engine_EventWait(9);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Map_CopyCellsTo(91, 8, 72, 9, 5, 5);
  Engine_EventWait(6);
  Map_CopyCellsTo(91, 13, 72, 9, 5, 6);
  Engine_EventWait(3);
  Engine_AudioPlayCue(SE_188);
  // Ten puffs of dust where it lands, thrown alternately left and right.
  for (debrisIndex = 0; debrisIndex < 10; debrisIndex++) {
    struct FieldActor *debris = Engine_ObjectCreate(
        SPRITE_DUST_PUFF, PIXELS(148 + debrisIndex * 4), 0, PIXELS(258));

    if (debris != 0) {
      debris->motionFlags = 0;
      Engine_ActorSetSpriteFlags(debris, 0);
      debris->sprite->image.priority = 0;
      {
        u16 delay = (((u32)(Engine_RandomNext() * 40)) >> 16) + 40;

        debris->timer = delay;
      }
      debris->velocityX = (s32)((PIXELS(debrisIndex & 3) + PIXELS(1))) >> 1;
      debris->velocityZ = PIXELS(1);
      if (debrisIndex & 1) {
        debris->velocityX = -((s32)(((debrisIndex & 3) << 16) + 0x10000) >> 1);
      }
      Object_SetMode(debris, 1);
      Engine_ObjectSetScript(debris, gKolima_DebrisScript);
    }
  }
  Map_CopyCellsTo(91, 19, 72, 9, 5, 7);
  Map_CopyCellAttributes(23, 11, 5, 7, 8, 11);
  Map_SetShake(0, PIXELS(4), Q16_ONE);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_EventWait(20);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  // From now on the villager turns to keep facing the leader.
  FieldObject_SetTargetAndCallback(ACTOR_BRIDGE_VILLAGER,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gKolima_FaceLeaderScript);
  Engine_EventWait(60);
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_GameFlagSet(FLAG_KOLIMA_BRIDGE_LOWERED);
  Engine_EventEnd();
}

/// Sets up whichever scene is entered: Kolima Bridge as the curse and the
/// bridge leave it (Kolima_RestoreBridgeState), Kolima Tunnel with a window
/// transition, and Kolima itself. There the four sparkles twinkle, the
/// villagers stand as trees while the curse lasts (and three cells around
/// (14, 8) take the attributes of cell (13, 9)), and the first entry from the
/// world map plays Kolima_WitnessForestCurse. Once that has been seen, here or
/// in Kolima Forest, Garet, Ivan and Mia, the two voices and the domes are
/// taken away and five more sparkles are placed.
s32 Kolima_Initialize(void) {
  s16 scene;
  u32 actor;
  const s32 *table;

  scene = gGameState.scene;

  if (scene == (s32)&SceneId_KolimaBridge) {
    Kolima_RestoreBridgeState();
    return 0;
  }

  if (scene == (s32)&SceneId_KolimaTunnel) {
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    return 0;
  }

  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_SPARKLE), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_SPARKLE + 1), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_SPARKLE + 2), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_SPARKLE + 3), 0);

  table = gKolima_EntryActions;
  Actor_SetActionScript(ACTOR_FIRST_SPARKLE, table);
  Actor_SetActionScript(ACTOR_FIRST_SPARKLE + 1, table);
  Actor_SetActionScript(ACTOR_FIRST_SPARKLE + 2, table);
  Actor_SetActionScript(ACTOR_FIRST_SPARKLE + 3, table);

  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    for (actor = ACTOR_FIRST_VILLAGER; actor <= ACTOR_LAST_VILLAGER; actor++) {
      Engine_ActorSetSpriteFlags(Object_GetById(actor), 0);
    }
    Map_CopyCellAttributes(13, 9, 1, 1, 13, 8);
    Map_CopyCellAttributes(13, 9, 1, 1, 15, 8);
    Map_CopyCellAttributes(13, 9, 1, 1, 14, 9);
  }

  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_WITNESSED) == 0) {
    if (gGameState.entrance == KOLIMA_FROM_WORLD_MAP) {
      Kolima_WitnessForestCurse();
    }
  }

  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_WITNESSED) != 0) {
    Engine_ActorDestroy(GARET);
    Engine_ActorDestroy(IVAN);
    Engine_ActorDestroy(MIA);
    Engine_ActorDestroy(ACTOR_TRET);
    Engine_ActorDestroy(ACTOR_LAUREL);
    Engine_ActorDestroy(ACTOR_LEADERS_PSYNERGY_FIELD);
    Engine_ActorDestroy(ACTOR_IVANS_PSYNERGY_FIELD);
    Engine_ActorDestroy(ACTOR_MIAS_PSYNERGY_FIELD);
    Engine_ActorDestroy(ACTOR_GARETS_PSYNERGY_FIELD);
    Event_CallWithLastActiveObjectId(gKolima_EntryScript);
  }

  return 0;
}

/// Shows one of the domes of Psynergy on its frames, even or odd, and lifts it
/// 500 pixels out of sight on the others.
static __inline__ void Kolima_SetProtectiveFieldHeight(s32 actorId,
                                                       s32 visibleOnOddFrames) {
  struct FieldActor *field = Object_GetById(actorId);
  if (field != NULL) {
    field->motionFlags = 0;
    if (visibleOnOddFrames ? (gFrameCount & 1) : !(gFrameCount & 1)) {
      field->y.fixed = 0;
    } else {
      field->y.fixed = PIXELS(500);
    }
  }
}

/// Makes the four domes flicker, two on even frames and two on odd.
static void Kolima_AlternateProtectiveFieldHeights(void) {
  Kolima_SetProtectiveFieldHeight(ACTOR_LEADERS_PSYNERGY_FIELD, 0);
  Kolima_SetProtectiveFieldHeight(ACTOR_IVANS_PSYNERGY_FIELD, 1);
  Kolima_SetProtectiveFieldHeight(ACTOR_MIAS_PSYNERGY_FIELD, 0);
  Kolima_SetProtectiveFieldHeight(ACTOR_GARETS_PSYNERGY_FIELD, 1);
}

/// Begins Mind Read on one of Kolima's villagers turned into a tree.
static void Kolima_ReadVillagerThoughts(s32 actorId, s32 targetId) {
  Engine_PsynergyStart(ABILITY_MIND_READ, 1);
  Engine_PsynergySetTarget(actorId, targetId);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_TaskWait(1);
}

/// Finishes Mind Read and releases its temporary scene objects.
static void Kolima_FinishReadingThoughts(void) {
  Engine_PsynergyPlayEffect(2);
  Engine_PsynergyLowerHands();
  FieldEffect_End();
}

/// Gives the four companions the same nod, waiting on the final actor.
static __inline__ void Kolima_NodTogether(void) {
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(MIA, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
}

/// Kolima, the first entry from the world map (KOLIMA_FROM_WORLD_MAP), until
/// FLAG_KOLIMA_CURSE_WITNESSED: the party sees the curse fall. The leader,
/// Garet and Ivan, with Mia if she has joined, walk into a silent village.
/// Ivan reads a tree's mind ("This tree...was a person"), Garet sees that
/// everyone in Kolima has been turned into a tree and asks the leader whether
/// he knows why, and Ivan notices "sparkly stuff on the ground". Sparkles rain
/// from the sky and press the party down until domes of their own Psynergy rise
/// over each of them; they argue over whether it was their Psynergy, since no
/// one was concentrating, and Ivan decides that some Psynergy acts only when it
/// is needed. A second shower raises the domes again, and Tret and Laurel,
/// unseen, wonder at people immune to Tret's glamour: Tret, dying and torn in
/// two, will not forgive Kolima's axes, and Laurel tells the party to reawaken
/// his gentle side and look for Tret deep within the forest. Whatever the
/// leader answers to Ivan and Garet, the party resolves to save Kolima. Mia's
/// lines and moves are skipped without her. Sets FLAG_KOLIMA_CURSE_WITNESSED;
/// KolimaForestEntrance_WitnessForestCurse plays the same scene in Kolima
/// Forest.
static void Kolima_WitnessForestCurse(void) {
  struct FieldActor *actor;
  s32 *partyLayout;
  s32 motionFacingFlag;
  // One staging word is reused for facing bytes, jump values and flags.
  s32 stagingValue;
  const s32 *squashActions;
  const s32 *restoreScaleActions;
  u32 *firstCurtainState;
  s32 *sceneState;
  const s32 *growFieldActions;
  const s32 *shrinkFieldActions;
  u32 *secondCurtainState;

  // Enter the deserted village and let Ivan inspect one of its trees.
  Engine_EventStart();
  partyLayout = &gKolima_MiaInParty;
  *partyLayout = GameFlag_IsSet(FLAG_IN_PARTY(MIA));
  actor = Object_GetById(ACTOR_LEADERS_PSYNERGY_FIELD);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_IVANS_PSYNERGY_FIELD);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_MIAS_PSYNERGY_FIELD);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_GARETS_PSYNERGY_FIELD);
  Engine_ActorSetSpriteFlags(actor, 0);
  Camera_MoveTo(PIXELS(104), -1, PIXELS(256), 0);
  Engine_MapRedraw();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(23), PIXELS(247));
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 121, 238);
  Actor_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetActionScript(GARET, gKolima_GaretWalksIn);
  Actor_SetActionScript(IVAN, gKolima_IvanWalksIn);
  if (*partyLayout != 0) {
    Actor_SetSpeed(MIA, PIXELS(3) / 5, PIXELS(3) / 10);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetActionScript(MIA, gKolima_MiaWalksIn);
  }
  Object_RefreshSelectorById(IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  partyLayout = &gKolima_MiaInParty;
  if (*partyLayout != 0) {
    Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 60);
  if (*partyLayout != 0) {
    Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  if (*partyLayout != 0) {
    Actor_FaceActor(MIA, ACTOR_PARTY_LEADER, 0);
  }
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_EventSetMessage(MsgKolimaQuiet);
  Kolima_ShowMessageAndWait(GARET, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(30);
  Actor_WalkToAndWait(IVAN, 72, 0x11e);
  Actor_WalkToAndWait(IVAN, 72, 0x12e);
  Actor_WalkToAndWait(IVAN, 88, 0x136);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  if (*partyLayout != 0) {
    Actor_FaceActor(MIA, IVAN, 0);
  }
  Engine_EventWait(30);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  if (*partyLayout != 0) {
    Engine_ActorSetAnimation(MIA, ANIMATION_NOD);
  }
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(30);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  Engine_EventWait(20);
  // Mind Read reveals that the trees are the missing villagers.
  Kolima_ReadVillagerThoughts(IVAN, ACTOR_VILLAGER_RELYING_ON_FORESTS);
  Engine_EventWait(40);
  Kolima_FinishReadingThoughts();
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Object_GetById(IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(IVAN, 80, 0x136);
  motionFacingFlag = ACTOR_FACES_MOTION;
  Engine_EventWait(1);
  Object_GetById(IVAN)->steeringFlags |= motionFacingFlag;
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Kolima_ShowMessageAndWait(GARET, 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Kolima_ShowMessageAndWait(IVAN, 20);
  if (*partyLayout != 0) {
    Actor_FaceActor(MIA, ACTOR_PARTY_LEADER, 0);
  }
  Engine_ActorFaceEachOther(IVAN, GARET, 60);
  if (*partyLayout != 0) {
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
  }
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Kolima_ShowMessageAndWait(GARET, 20);
  Actor_SetSpeed(IVAN, PIXELS(1) / 2, PIXELS(1) / 4);
  Object_GetById(IVAN)->steeringFlags &= 254;
  Actor_WalkToAndWait(IVAN, 72, 0x11e);
  Engine_EventWait(1);
  Object_GetById(IVAN)->steeringFlags |= motionFacingFlag;
  Actor_SetActionScript(IVAN, gKolima_IvanWalksIn);
  if (*partyLayout != 0) {
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Engine_EventWait(60);
    Kolima_ShowMessageAndWait(MIA, 20);
    Engine_ActorSetAnimation(MIA, ANIMATION_NOD);
  } else {
    gEventWork->message++;
  }
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(10);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  } else {
    Engine_ActorSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
    gEventWork->message++;
  }
  Kolima_ShowMessageAndWait(GARET, 40);
  Kolima_FaceActorAndWait(IVAN, FACING_SOUTHEAST, 40);
  Kolima_FaceActorAndWait(IVAN, FACING_WEST, 20);
  Kolima_FaceActorAndWait(IVAN, FACING_SOUTH, 40);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
  stagingValue = 160;
  Kolima_FaceActorAndWait(MIA, FACING_SOUTHEAST, 10);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, (stagingValue << 8), 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventSetMessage(MsgKolimaWhatIsIt);
  Kolima_ShowMessageAndWait(GARET, 10);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Kolima_ShowMessageAndWait(IVAN, 20);
  Kolima_FaceActorAndWait(GARET, 0, 20);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, (stagingValue << 8), 40);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 20);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 30);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHWEST, 20);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 30);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 20);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  Event_PlayCue(SE_206);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  // The falling sparkles threaten to turn the party into trees as well.
  gKolima_EffectActive = 1;
  FieldScheduler_AddCallback(Kolima_SpawnCurseSparkle, TASK_PRIORITY_SCENE);
  Engine_TaskWait(20);
  ColorBuffer_ApplyTarget(0x405210, 1);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 2);
  Engine_ColorBufferInterpolate(120);
  Engine_TaskWait(60);
  squashActions = gKolima_PartySquashActions;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, squashActions);
  Actor_SetActionScript(GARET, squashActions);
  Actor_SetActionScript(IVAN, squashActions);
  Actor_SetActionScript(MIA, squashActions);
  Engine_EventWait(100);
  Kolima_ShowMessageAndWait(GARET, 20);
  Kolima_ShowMessageAndWait(IVAN, 40);
  if (gKolima_MiaInParty != 0) {
    Engine_EventWait(40);
    Actor_SetAttachedEffect(MIA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_EventWait(40);
    Kolima_ShowMessageAndWait(MIA, 40);
  } else {
    gEventWork->message++;
  }
  Engine_EventWait(20);
  if (gKolima_MiaInParty != 0) {
    stagingValue = 128;
    actor = Object_GetById(MIA);
    actor->velocityY = (stagingValue << 10);
    Engine_EventWait(10);
    Actor_SetSpeed(MIA, (stagingValue << 10), (stagingValue << 10));
    Actor_SetDestinationOffset(MIA, -2, 0);
    Actor_SetActionScript(MIA, gKolima_PartyRestoreScaleActions);
    actor = Object_GetById(MIA);
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorSetAnimation(MIA, ANIMATION_COLLAPSE);
    Engine_EventWait(10);
  }
  stagingValue = 128;
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->velocityY = (stagingValue << 10);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, (stagingValue << 10),
                 (stagingValue << 10));
  restoreScaleActions = gKolima_PartyRestoreScaleActions;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, restoreScaleActions);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_COLLAPSE);
  Engine_EventWait(20);
  actor = Object_GetById(GARET);
  actor->velocityY = (stagingValue << 10);
  Engine_EventWait(10);
  Actor_SetSpeed(GARET, (stagingValue << 10), (stagingValue << 10));
  Actor_SetActionScript(GARET, restoreScaleActions);
  actor = Object_GetById(GARET);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(GARET, ANIMATION_COLLAPSE);
  Engine_EventWait(40);
  actor = Object_GetById(IVAN);
  actor->velocityY = (stagingValue << 10);
  Engine_EventWait(10);
  Actor_SetActionScript(IVAN, restoreScaleActions);
  actor = Object_GetById(IVAN);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(IVAN, ANIMATION_COLLAPSE);
  gKolima_EffectActive = 0;
  Engine_EventWait(160);
  Engine_TaskRemoveCallback(Kolima_SpawnCurseSparkle);
  Engine_EventWait(120);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  // Bring the falling curtain over the party and reveal their protective
  // fields.
  gFallingEffectWidth = 0;
  firstCurtainState = &gFallingEffectState;
  gFallingEffectOffset = PIXELS(128);
  *firstCurtainState = KOLIMA_DUST_CLOSING;
  FieldScheduler_AddCallback(Kolima_UpdateCurseDust, TASK_PRIORITY_SCENE);
  Engine_EventWait(180);
  Event_PlayCue(MUS_021);
  Kolima_ShowMessageAndWait(GARET, 80);
  Kolima_ShowMessageAndWait(IVAN, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Kolima_ShowMessageAndWait(IVAN, 20);
  *firstCurtainState = KOLIMA_DUST_CLOSING_WIDE;
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(IVAN, 3);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Kolima_ShowMessageAndWait(GARET, 20);
  if (gKolima_MiaInParty != 0) {
    Actor_SetAttachedEffect(MIA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Kolima_ShowMessageAndWait(MIA, 10);
  } else {
    gEventWork->message++;
  }
  sceneState = (s32 *)&gFallingEffectState;
  *sceneState = KOLIMA_DUST_DRIFTING;
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags &= 254;
  Object_GetById(GARET)->priorityFlags &= 254;
  Object_GetById(IVAN)->priorityFlags &= 254;
  Object_GetById(MIA)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetSpritePriority(GARET, 3);
  Engine_ActorSetSpritePriority(IVAN, 3);
  stagingValue = 0;
  Engine_ActorSetSpritePriority(MIA, 3);
  gKolima_PendingPose = stagingValue;
  FieldScheduler_AddCallback(Kolima_AlternateProtectiveFieldHeights,
                             TASK_PRIORITY_SCENE);
  Event_PlayCue(SE_220);
  Object_GetById(ACTOR_LEADERS_PSYNERGY_FIELD)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_LEADERS_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_LEADERS_PSYNERGY_FIELD, PIXELS(120), PIXELS(248));
  growFieldActions = gKolima_ProtectiveFieldGrowActions;
  Actor_SetActionScript(ACTOR_LEADERS_PSYNERGY_FIELD, growFieldActions);
  Object_GetById(ACTOR_IVANS_PSYNERGY_FIELD)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_IVANS_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_IVANS_PSYNERGY_FIELD, PIXELS(100), PIXELS(274));
  Actor_SetActionScript(ACTOR_IVANS_PSYNERGY_FIELD, growFieldActions);
  if (gKolima_MiaInParty != 0) {
    Object_GetById(ACTOR_MIAS_PSYNERGY_FIELD)->priorityFlags &= 254;
    Engine_ActorSetSpritePriority(ACTOR_MIAS_PSYNERGY_FIELD, 2);
    Actor_SetPosition(ACTOR_MIAS_PSYNERGY_FIELD, PIXELS(74), PIXELS(254));
    Actor_SetActionScript(ACTOR_MIAS_PSYNERGY_FIELD, growFieldActions);
  }
  Object_GetById(ACTOR_GARETS_PSYNERGY_FIELD)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_GARETS_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_GARETS_PSYNERGY_FIELD, PIXELS(94), PIXELS(225));
  Actor_SetActionScript(ACTOR_GARETS_PSYNERGY_FIELD, growFieldActions);
  if (*sceneState != KOLIMA_DUST_CENTRED) {
    do {
      Engine_TaskWait(1);
    } while (gFallingEffectState != KOLIMA_DUST_CENTRED);
  }
  Engine_EventWait(300);
  Engine_TaskRemoveCallback(Kolima_UpdateCurseDust);
  Engine_EventWait(120);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  Engine_ActorStop(ACTOR_LEADERS_PSYNERGY_FIELD);
  Engine_ActorStop(ACTOR_IVANS_PSYNERGY_FIELD);
  if (gKolima_MiaInParty != 0) {
    Engine_ActorStop(ACTOR_MIAS_PSYNERGY_FIELD);
  }
  Engine_ActorStop(ACTOR_GARETS_PSYNERGY_FIELD);
  Engine_TaskWait(1);
  shrinkFieldActions = gKolima_ProtectiveFieldShrinkActions;
  Actor_SetActionScript(ACTOR_LEADERS_PSYNERGY_FIELD, shrinkFieldActions);
  Actor_SetActionScript(ACTOR_IVANS_PSYNERGY_FIELD, shrinkFieldActions);
  if (gKolima_MiaInParty != 0) {
    Actor_SetActionScript(ACTOR_MIAS_PSYNERGY_FIELD, shrinkFieldActions);
  }
  Actor_RunActionScriptAndWait(ACTOR_GARETS_PSYNERGY_FIELD, shrinkFieldActions);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Event_OpenMessage(GARET, 0);
  Actor_SetPosition(ACTOR_TRET, PIXELS(87), PIXELS(139));
  Actor_SetPosition(ACTOR_LAUREL, PIXELS(87), PIXELS(139));
  Engine_TaskWait(1);
  if (Engine_EventChooseYesNo(ACTOR_TRET, 0) == 1) {
    gEventWork->message++;
  }
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Kolima_ShowMessageAndWait(IVAN, 20);
  if (gKolima_MiaInParty != 0) {
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKolimaKnowThoseFieldsWere);
    Kolima_ShowMessageAndWait(MIA, 40);
  }
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  // The companions discover that their Psynergy formed the barriers.
  Engine_EventSetMessage(MsgKolimaWasOurPsynergy);
  Kolima_ShowMessageAndWait(IVAN, 40);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(40);
  Engine_ActorSetSpritePriority(GARET, 2);
  Object_GetById(GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  actor = Object_GetById(GARET);
  Engine_ActorSetSpriteFlags(actor, 1);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorSetAnimation(GARET, ANIMATION_STAND);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 60);
  Kolima_ShowMessageAndWait(GARET, 20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Kolima_ShowMessageAndWait(GARET, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(40);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHWEST, 40);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 20);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHWEST, 20);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(40);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(10);
  Engine_ActorJump(GARET, 4, 0);
  Engine_EventWait(20);
  Kolima_ShowMessageAndWait(GARET, 20);
  if (gKolima_MiaInParty != 0) {
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(80);
    Engine_ActorSetSpritePriority(MIA, 2);
    Object_GetById(MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    actor = Object_GetById(MIA);
    Engine_ActorSetSpriteFlags(actor, 1);
    Engine_ActorJump(MIA, 4, 0);
    Actor_SetDestinationOffset(MIA, -2, 0);
    Engine_ActorSetAnimation(MIA, ANIMATION_STAND);
    Kolima_FaceActorAndWait(MIA, FACING_NORTHEAST, 60);
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(20);
    Kolima_ShowMessageAndWait(MIA, 20);
  } else {
    gEventWork->message++;
  }
  Engine_ActorJump(GARET, 2, 0);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
  Kolima_ShowMessageAndWait(GARET, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  stagingValue = 1;
  Engine_ActorSetSpritePriority(IVAN, 2);
  Object_GetById(IVAN)->priorityFlags |= stagingValue;
  actor = Object_GetById(IVAN);
  Engine_ActorSetSpriteFlags(actor, 1);
  Engine_ActorJump(IVAN, 4, 0);
  Engine_ActorSetAnimation(IVAN, ANIMATION_STAND);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= stagingValue;
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(actor, 1);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_EventWait(60);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 10);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Kolima_ShowMessageAndWait(IVAN, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(MIA, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  Event_OpenMessage(IVAN, 0);
  Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
    gEventWork->message++;
  } else {
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Event_ShowMessage(GARET, 0);
  }
  Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 10);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  Kolima_ShowMessageAndWait(GARET, 20);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Kolima_ShowMessageAndWait(IVAN, 10);
  if (gKolima_MiaInParty != 0) {
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Kolima_FaceActorAndWait(MIA, 0, 20);
    Kolima_FaceActorAndWait(MIA, FACING_SOUTHEAST, 10);
    Engine_ActorSetAnimation(MIA, ANIMATION_SHAKE_HEAD);
    Kolima_ShowMessageAndWait(MIA, 10);
  } else {
    gEventWork->message++;
  }
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  stagingValue = 128;
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Kolima_FaceActorAndWait(GARET, (stagingValue << 7), 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTHEAST, 10);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Kolima_ShowMessageAndWait(IVAN, 20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 40);
  Actor_FaceDirection(GARET, (stagingValue << 7), 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Kolima_ShowMessageAndWait(IVAN, 10);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Kolima_ShowMessageAndWait(GARET, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Kolima_ShowMessageAndWait(GARET, 20);
  Engine_ActorSetAnimation(IVAN, ANIMATION_NOD);
  Kolima_ShowMessageAndWait(IVAN, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
  Event_OpenMessage(GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(40);
    gEventWork->message++;
  }
  Event_ShowMessage(GARET, 0);
  Event_PlayCue(MUS_021);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = PIXELS(128);
  secondCurtainState = &gFallingEffectState;
  *secondCurtainState = KOLIMA_DUST_CLOSING;
  FieldScheduler_AddCallback(Kolima_UpdateCurseDust, TASK_PRIORITY_SCENE);
  Engine_EventWait(80);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(60);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  // A second shower raises the domes again before Tret and Laurel speak.
  Engine_EventSetMessage(MsgKolimaWatchOutItsHappeningAgain);
  Kolima_ShowMessageAndWait(IVAN, 10);
  Kolima_FaceActorAndWait(GARET, FACING_NORTH, 10);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
  if (gKolima_MiaInParty != 0) {
    Kolima_FaceActorAndWait(MIA, FACING_NORTH, 10);
  }
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags &= 254;
  Object_GetById(GARET)->priorityFlags &= 254;
  Object_GetById(IVAN)->priorityFlags &= 254;
  Object_GetById(MIA)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetSpritePriority(GARET, 3);
  Engine_ActorSetSpritePriority(IVAN, 3);
  Engine_ActorSetSpritePriority(MIA, 3);
  *secondCurtainState = KOLIMA_DUST_CLOSING_WIDE;
  Event_PlayCue(SE_220);
  Actor_SetPosition(ACTOR_LEADERS_PSYNERGY_FIELD, PIXELS(120), PIXELS(248));
  growFieldActions = gKolima_ProtectiveFieldGrowActions;
  Actor_SetActionScript(ACTOR_LEADERS_PSYNERGY_FIELD, growFieldActions);
  Actor_SetPosition(ACTOR_IVANS_PSYNERGY_FIELD, PIXELS(100), PIXELS(274));
  Actor_SetActionScript(ACTOR_IVANS_PSYNERGY_FIELD, growFieldActions);
  if (gKolima_MiaInParty != 0) {
    Actor_SetPosition(ACTOR_MIAS_PSYNERGY_FIELD, PIXELS(74), PIXELS(254));
    Actor_SetActionScript(ACTOR_MIAS_PSYNERGY_FIELD, growFieldActions);
  }
  Actor_SetPosition(ACTOR_GARETS_PSYNERGY_FIELD, PIXELS(94), PIXELS(225));
  Actor_SetActionScript(ACTOR_GARETS_PSYNERGY_FIELD, growFieldActions);
  Engine_EventWait(120);
  *secondCurtainState = KOLIMA_DUST_DRIFTING;
  do {
    Engine_TaskWait(1);
  } while (gFallingEffectState != KOLIMA_DUST_CENTRED);
  // Tret and Laurel, unseen, wonder at the party's immunity to the glamour.
  Kolima_ShowMessageAndWait(ACTOR_TRET, 80);
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(60);
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Kolima_NodTogether();
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Kolima_NodTogether();
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_EventWait(40);
  Kolima_ShowMessageAndWait(ACTOR_TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 40);
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, FACING_NORTH, 80);
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, 0, 40);
  Kolima_ShowMessageAndWait(ACTOR_TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, FACING_NORTH, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_SHAKE_HEAD);
  Engine_ActorSetAnimation(GARET, ANIMATION_SHAKE_HEAD);
  Engine_ActorSetAnimation(MIA, ANIMATION_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
  Engine_EventWait(60);
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Kolima_NodTogether();
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, 0, 20);
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, 0, 20);
  Kolima_ShowMessageAndWait(ACTOR_TRET, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, FACING_NORTH, 10);
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Kolima_NodTogether();
  Engine_EventWait(60);
  // Laurel's and Tret's last words; then the party decides whether to help.
  Event_ShowMessage(ACTOR_LAUREL, 0);
  Event_ShowMessage(ACTOR_TRET, 0);
  Engine_TaskRemoveCallback(Kolima_UpdateCurseDust);
  Engine_EventWait(80);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(80);
  Engine_ActorStop(ACTOR_LEADERS_PSYNERGY_FIELD);
  Engine_ActorStop(ACTOR_IVANS_PSYNERGY_FIELD);
  sceneState = &gKolima_MiaInParty;
  Engine_ActorStop(ACTOR_MIAS_PSYNERGY_FIELD);
  Engine_ActorStop(ACTOR_GARETS_PSYNERGY_FIELD);
  Engine_TaskWait(1);
  shrinkFieldActions = gKolima_ProtectiveFieldShrinkActions;
  Actor_SetActionScript(ACTOR_LEADERS_PSYNERGY_FIELD, shrinkFieldActions);
  Actor_SetActionScript(ACTOR_IVANS_PSYNERGY_FIELD, shrinkFieldActions);
  if (*sceneState != 0) {
    Actor_SetActionScript(ACTOR_MIAS_PSYNERGY_FIELD, shrinkFieldActions);
  }
  Actor_RunActionScriptAndWait(ACTOR_GARETS_PSYNERGY_FIELD, shrinkFieldActions);
  Engine_EventWait(20);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetSpritePriority(GARET, 2);
  Engine_ActorSetSpritePriority(IVAN, 2);
  stagingValue = 1;
  Engine_ActorSetSpritePriority(MIA, 2);
  Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |= stagingValue;
  Object_GetById(GARET)->priorityFlags |= stagingValue;
  Object_GetById(IVAN)->priorityFlags |= stagingValue;
  Object_GetById(MIA)->priorityFlags |= stagingValue;
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTHEAST, 10);
  Event_OpenMessage(IVAN, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(10);
    Event_OpenMessage(GARET, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Kolima_FaceActorAndWait(MIA, 0, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Engine_EventWait(40);
      Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 20);
      Kolima_ShowMessageAndWait(GARET, 10);
      Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 20);
      Kolima_FaceActorAndWait(IVAN, FACING_NORTHEAST, 20);
      Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
      Kolima_ShowMessageAndWait(IVAN, 20);
      Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 20);
    } else {
      Kolima_FaceActorAndWait(MIA, 0, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
      Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 20);
      Engine_EventSetMessage(MsgKolimaHesAsStumpedAsWe);
      Kolima_ShowMessageAndWait(GARET, 20);
      Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
      Kolima_ShowMessageAndWait(IVAN, 20);
    }
    Engine_ActorSetAnimation(MIA, ANIMATION_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
    Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKolimaYoureIsaacThereIsntMuch);
    Kolima_ShowMessageAndWait(GARET, 10);
    Actor_FaceDirection(GARET, FACING_SOUTH, 0);
    Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 20);
    Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
    Event_OpenMessage(IVAN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
      Engine_EventWait(40);
      Kolima_FaceActorAndWait(IVAN, FACING_NORTHEAST, 10);
      Kolima_ShowMessageAndWait(IVAN, 10);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(MIA, 0, 10);
        Engine_ActorStartRepeatedMotion(MIA, 3);
        Kolima_ShowMessageAndWait(MIA, 20);
      } else {
        gEventWork->message++;
      }
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetAttachedEffect(GARET,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Kolima_ShowMessageAndWait(GARET, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Engine_EventWait(120);
      Kolima_ShowMessageAndWait(IVAN, 40);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(MIA, FACING_SOUTHEAST, 10);
        Engine_ActorSetAnimationAndWait(MIA, ANIMATION_SHAKE_HEAD);
        Kolima_ShowMessageAndWait(MIA, 10);
      } else {
        gEventWork->message++;
      }
      Engine_EventWait(60);
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(IVAN, FACING_NORTHWEST, 40);
        Kolima_FaceActorAndWait(IVAN, FACING_NORTHEAST, 20);
      }
      Kolima_ShowMessageAndWait(IVAN, 10);
      Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Engine_EventWait(40);
      Kolima_ShowMessageAndWait(IVAN, 20);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
      Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
      Engine_EventWait(20);
      Engine_ActorSetAnimation(MIA, ANIMATION_NOD);
      Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
    } else {
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Engine_EventWait(40);
      Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
      Engine_EventSetMessage(MsgKolimaThatsReliefIsaacThoughtYoud);
      Kolima_ShowMessageAndWait(IVAN, 20);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(MIA, 0, 10);
        Engine_ActorStartRepeatedMotion(MIA, 1);
        Kolima_ShowMessageAndWait(MIA, 20);
      } else {
        gEventWork->message++;
      }
      Actor_SetAttachedEffect(GARET,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Kolima_ShowMessageAndWait(GARET, 20);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Engine_EventWait(80);
      Kolima_ShowMessageAndWait(IVAN, 40);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(MIA, FACING_SOUTHEAST, 20);
        Engine_ActorSetAnimation(MIA, ANIMATION_SHAKE_HEAD);
        Kolima_ShowMessageAndWait(MIA, 40);
      } else {
        gEventWork->message++;
      }
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      Engine_EventWait(20);
      Kolima_ShowMessageAndWait(IVAN, 20);
      Engine_ActorStartRepeatedMotion(GARET, 2);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_EventWait(40);
      Kolima_ShowMessageAndWait(IVAN, 20);
    }
  }
  // Reunite the party and remember that the first visit has played.
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorSetAnimation(GARET, ANIMATION_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIMATION_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Actor_SetPosition(ACTOR_TRET, 0, 0);
  Actor_SetPosition(ACTOR_LAUREL, 0, 0);
  if (gKolima_MiaInParty != 0) {
    Engine_ActorSetAnimation(MIA, ANIMATION_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(MIA);
    Actor_SetPosition(MIA, 0, 0);
  }
  GameFlag_Set(FLAG_KOLIMA_CURSE_WITNESSED);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

/// Shows the selected dialogue through an actor, then holds for its reaction.
static void Kolima_ShowMessageAndWait(s32 actorId, s32 waitFrames) {
  Engine_EventShowMessage(actorId, 0);
  Engine_EventWait(waitFrames);
}

/// Turns an actor immediately, then leaves time for the next scene beat.
static void Kolima_FaceActorAndWait(s32 actorId, s32 facing, s32 waitFrames) {
  Engine_ActorFaceDirection(actorId, facing, 0);
  Engine_EventWait(waitFrames);
}

#include "common/kolima_grow.h"

/// Spawns a moving curse sparkle every eighth frame.
static void Kolima_SpawnCurseSparkle(void) {
  struct FieldActor *sparkle;
  struct AnimationObject *sprite;
  s32 framePhase;
  s32 priorityFlags;
  s32 spawnX = PIXELS(98);
  s32 spawnZ = PIXELS(105);
  s32 targetX = PIXELS(98);
  s32 targetZ = PIXELS(269);

  framePhase = gFrameCount & 7;
  if (framePhase != 0) {
    return;
  }
  if (gKolima_EffectActive != 0) {
    Engine_AudioPlayCue(SE_200);
  }
  sparkle = Engine_ObjectCreate(SPRITE_GOLDEN_RINGS, spawnX, 0, spawnZ);
  if (sparkle == 0) {
    return;
  }
  sprite = sparkle->sprite;
  sprite->flags = framePhase;
  priorityFlags = 0xff & ~ACTOR_PRIORITY_AUTOMATIC;
  priorityFlags &= sparkle->priorityFlags;
  sparkle->priorityFlags = priorityFlags;
  sprite->image.priority = 1;
  sparkle->scaleX = Q16_ONE / 10;
  sparkle->speed = PIXELS(8);
  sparkle->acceleration = PIXELS(8);
  sparkle->motionFlags = framePhase;
  Object_SetMode(sparkle, 2);
  Engine_ObjectSetPosition(sparkle, targetX, 0, targetZ);
  Engine_ObjectSetScript(sparkle, gKolima_Object26Script);
}

#include "common/kolima_blink.h"

/// Triggers a temporary pose when an effect enters the party's occupied area.
static s32 Kolima_TriggerRegionScript(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if (gKolima_MiaInParty != 0) {
    if ((actor->x.fixed > PIXELS(59) && actor->x.fixed < PIXELS(141) &&
         actor->z.fixed > PIXELS(211) && actor->z.fixed < PIXELS(257)) ||
        (actor->x.fixed > PIXELS(69) && actor->x.fixed < PIXELS(122) &&
         actor->z.fixed > PIXELS(194) && actor->z.fixed < PIXELS(277))) {
      Engine_AudioPlayCue(SE_106);
      Engine_ObjectSetScript(actor, gKolima_RegionScript);
      gKolima_PendingPose = 1;
    }
  } else {
    if ((actor->x.fixed > PIXELS(59) && actor->x.fixed < PIXELS(111) &&
         actor->z.fixed > PIXELS(194) && actor->z.fixed < PIXELS(230)) ||
        (actor->x.fixed > PIXELS(111) && actor->x.fixed < PIXELS(141) &&
         actor->z.fixed > PIXELS(216) && actor->z.fixed < PIXELS(250)) ||
        (actor->x.fixed > PIXELS(78) && actor->x.fixed < PIXELS(122) &&
         actor->z.fixed > PIXELS(241) && actor->z.fixed < PIXELS(277))) {
      Engine_AudioPlayCue(SE_106);
      Engine_ObjectSetScript(actor, gKolima_RegionScript);
      gKolima_PendingPose = 1;
    }
  }
  return 0;
}

#include "common/kolima_forest_bridge_falling_effect.h"
s32 gKolima_PendingPose __attribute__((section(".bss")));
s32 gKolima_MiaInParty __attribute__((section(".bss")));
s32 gKolima_EffectActive __attribute__((section(".bss")));

/// Sets up Kolima Bridge. While the curse lasts, the villager as a man
/// (ACTOR_BRIDGE_VILLAGER) is put out of sight, leaving him a tree
/// (ACTOR_BRIDGE_VILLAGER_AS_TREE), and the row of cells at (9, 18) takes the
/// attributes of the row above it; afterwards the tree is put out of sight
/// instead. Once the bridge is down (FLAG_KOLIMA_BRIDGE_LOWERED), it is
/// drawn down and its cells changed to match. XXX: the row likely closes the
/// bridge while the curse lasts, because the bridge's one event (trigger 10)
/// needs the man who is a tree until then.
static void Kolima_RestoreBridgeState(void) {
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    Engine_ActorSetPosition(ACTOR_BRIDGE_VILLAGER, 0, 0);
    {
      Map_CopyCellAttributes(9, 17, 5, 1, 9, 18);
    }
    Engine_MapRedraw();
    Engine_TaskWait(1);
  } else {
    Engine_ActorSetPosition(ACTOR_BRIDGE_VILLAGER_AS_TREE, 0, 0);
  }
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_BRIDGE_LOWERED) != 0) {
    {
      Map_CopyCellsTo(91, 19, 72, 9, 5, 7);
    }
    {
      Map_CopyCellAttributes(23, 11, 5, 7, 8, 11);
    }
    Engine_MapRedraw();
    Engine_TaskWait(1);
  }
}

/// Where Garet, Ivan and Mia walk on into the silent village and the way each
/// turns (Kolima_WitnessForestCurse).
s32 gKolima_GaretWalksIn[] = {
    SC_ARG(2), SC_MOVE(PIXELS(97), 0, PIXELS(221)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_ARG(1), SC_END,
};

s32 gKolima_IvanWalksIn[] = {
    SC_ARG(2), SC_MOVE(PIXELS(101), 0, PIXELS(268)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_EAST),
    SC_ARG(1), SC_END,
};

s32 gKolima_MiaWalksIn[] = {
    SC_ARG(2), SC_MOVE(PIXELS(78), 0, PIXELS(247)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_NORTH),
    SC_ARG(1), SC_END,
};

s32 gKolima_PartySquashActions[] = {
    SC_ADD(SW_SCALE_X, -Q16_ONE / 4),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolima_PartyRestoreScaleActions[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(1),
    SC_END,
};

s32 gKolima_ProtectiveFieldGrowActions[] = {
    SC_SET(SW_SCALE_X, Q16_ONE / 10),
    SC_SET(SW_SCALE_Y, Q16_ONE / 10),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, Q16_ONE / 50),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 50),
    SC_WAIT(1),
    SC_LOOP(70, SC_TO(1)),
    SC_CALL(Kolima_ApplyPendingPose),
    SC_END,
};

s32 gKolima_ProtectiveFieldShrinkActions[] = {
    SC_ARG(1),
    SC_ADD(SW_Z, -PIXELS(1) / 10),
    SC_ADD(SW_SCALE_X, -3 * Q16_ONE / 200),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 200),
    SC_WAIT(1),
    SC_LOOP(90, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// The sparkles on Kolima's ground: three quarters of their size, passed
/// through, twinkling at random (Kolima_UpdateTimedAppearance).
s32 gKolima_EntryActions[] = {
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 4),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 4),
    SC_SET(SW_COLLISION, 0),
    SC_SET(0x23, 0),
    SC_LABEL(1),
    SC_CALL(Kolima_UpdateTimedAppearance),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolima_DebrisScript[] = {
    SC_CALL(Kolima_UpdateFallingDebris),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolima_FaceLeaderScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A door opening: two frames of a cell column two cells high.
u16 gKolima_DoorCells[] = {
    MAP_CELL_STEP(66, 32, 1, 2, 4),
    MAP_CELL_STEP(67, 32, 1, 2, 4),
    MAP_CELL_STEPS_END,
};

/// Kolima's entrances. No exit leads to entrances 2, 10 and 12; entrance 10
/// puts the party where KOLIMA_FROM_WORLD_MAP does.
struct SceneEntrance gKolimaEntrances[] = {
    {0, CONDITION_ALWAYS, 232, 0, 120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KOLIMA_FROM_WORLD_MAP, CONDITION_ALWAYS, 44, 0, 267, FACING_EAST, 0, -1,
     -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 418, 0, 285, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {KOLIMA_FROM_INN, CONDITION_ALWAYS, 119, 0, 182, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {KOLIMA_FROM_ITEM_SHOP, CONDITION_ALWAYS, 151, 0, 310, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {KOLIMA_FROM_HOUSE_SOUTH, CONDITION_ALWAYS, 264, 0, 373, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {KOLIMA_FROM_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 329, 0, 230,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KOLIMA_FROM_HOUSE_NORTH, CONDITION_ALWAYS, 263, 0, 326, FACING_NORTH, 0,
     -1, -1, -1, -1, 0},
    {KOLIMA_FROM_KOLIMA_TUNNEL, CONDITION_ALWAYS, 360, 0, 346, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 44, 0, 267, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 232, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKolimaBridgeEntrances[] = {
    {KOLIMA_BRIDGE_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 170, 0, 386,
     FACING_NORTH, 0, 0, 0, 288, 416, 0},
    {KOLIMA_BRIDGE_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 168, 0, 50,
     FACING_SOUTH, 0, 48, 0, 288, 416, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKolimaTunnelEntrances[] = {
    {KOLIMA_TUNNEL_FROM_KOLIMA_INTERIORS, CONDITION_ALWAYS, 216, 0, 320,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KOLIMA_TUNNEL_FROM_KOLIMA, CONDITION_ALWAYS, 312, 0, 312, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneRegion gKolimaTunnelRegions[] = {
    {207, 31, 311, 225, 49, 329, -1, 1},
    {303, 31, 303, 321, 49, 321, -1, 2},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

/// Kolima's exits, then Kolima Bridge's (from the header word for SceneId_KolimaBridge)
/// and Kolima Tunnel's (from the one for SceneId_KolimaTunnel).
u32 gKolimaExits[] = {
    SCENE_EXITS(Kolima),
    SCENE_EXIT(KOLIMA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KOLIMA_WEST),
    SCENE_EXIT(KOLIMA_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_KOLIMA_EAST),
    SCENE_EXIT(KOLIMA_TO_INN, KolimaInteriors, KOLIMA_INTERIORS_INN_FROM_KOLIMA),
    SCENE_EXIT(KOLIMA_TO_ITEM_SHOP, KolimaInteriors, KOLIMA_INTERIORS_ITEM_SHOP_FROM_KOLIMA),
    SCENE_EXIT(KOLIMA_TO_HOUSE_SOUTH, KolimaInteriors, KOLIMA_INTERIORS_HOUSE_FROM_KOLIMA_SOUTH),
    SCENE_EXIT(KOLIMA_TO_WEAPON_AND_ARMOR_SHOP, KolimaInteriors, KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_FROM_KOLIMA),
    SCENE_EXIT(KOLIMA_TO_HOUSE_NORTH, KolimaInteriors, KOLIMA_INTERIORS_HOUSE_FROM_KOLIMA_NORTH),
    SCENE_EXIT(KOLIMA_TO_KOLIMA_TUNNEL, KolimaTunnel, KOLIMA_TUNNEL_FROM_KOLIMA),
    SCENE_EXITS(KolimaBridge),
    SCENE_EXIT(KOLIMA_BRIDGE_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_KOLIMA_BRIDGE_EAST),
    SCENE_EXIT(KOLIMA_BRIDGE_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_KOLIMA_BRIDGE_WEST),
    SCENE_EXITS(KolimaTunnel),
    SCENE_EXIT(KOLIMA_TUNNEL_TO_KOLIMA_INTERIORS, KolimaInteriors, KOLIMA_INTERIORS_FROM_KOLIMA_TUNNEL),
    SCENE_EXIT(KOLIMA_TUNNEL_TO_KOLIMA, Kolima, KOLIMA_FROM_KOLIMA_TUNNEL),
    SCENE_EXITS_END,
};

struct ScenePlacement gKolimaTunnelPlacements[] = {
    { SCENE_TABLE_END },
};

/// Kolima's people: Garet, Ivan and Mia for the curse, the nine villagers,
/// Tret's and Laurel's voices and the four domes until the curse has been seen
/// (FLAG_KOLIMA_CURSE_WITNESSED), four sparkles while it lasts, and Granite until
/// it joins (its Djinni flag, 0x31).
struct ScenePlacement gKolimaPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(192), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(312), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(168), 0, PIXELS(360), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(344), 0, PIXELS(280), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(392), FACING_SOUTH, 3, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(184), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(264), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(264), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_TRET, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_LAUREL, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, ACTOR_STAND,
        PIXELS(24), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, ACTOR_STAND,
        PIXELS(40), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { VENUS_DJINNI, 0x31, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(360), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

/// Five more sparkles, placed by Kolima_Initialize once the curse has been seen
/// and kept while it lasts.
struct ScenePlacement gKolima_EntryScript[] = {
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(120), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(296), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(440), 0, PIXELS(344), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(296), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(152), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

/// Kolima Bridge's villager, as a man (ACTOR_BRIDGE_VILLAGER) and as a tree
/// (ACTOR_BRIDGE_VILLAGER_AS_TREE), in the same place;
/// Kolima_RestoreBridgeState puts one of them out of sight.
struct ScenePlacement gKolimaBridgePlacements[] = {
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(234), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, 3, 0 },
    { SPRITE_TREE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(234), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

/// The events Kolima and Kolima Tunnel share: the exits and doorways, the
/// villagers' words once the curse is lifted and their thoughts before and
/// after, the healer, Granite and two jars. Exits 1 and 2, which cell 99 takes
/// too, keep their numbers, since each scene's exit table gives them its own
/// doors: KOLIMA_TO_WORLD_MAP and KOLIMA_TO_WORLD_MAP_EAST in Kolima,
/// KOLIMA_TUNNEL_TO_KOLIMA_INTERIORS and KOLIMA_TUNNEL_TO_KOLIMA in Kolima
/// Tunnel.
struct SceneEvent gKolimaEvents[] = {
    { EVENT_EXIT, 99, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_EnterDoor) },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, KOLIMA_TO_KOLIMA_TUNNEL },
    { EVENT_TALK, ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaThisIsKolimaVillageOfLumberjacks) },
    { EVENT_TALK, ACTOR_VILLAGER_RELYING_ON_FORESTS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaWeRelyOnTheForestsSo) },
    { EVENT_TALK, ACTOR_VILLAGER_TIME_TO_THINK, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaYouKnowBeingATreeGives) },
    { EVENT_TALK, ACTOR_VILLAGER_SPARKLES_FELL, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaITurnedIntoATreeRight) },
    { EVENT_TALK, ACTOR_OLD_MAN_MCCOYS_MONEY, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaDontBlameTheVillagersForBeing) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_HOLY_TREE_PALACE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaWhoWouldTryToUseThe) },
    { EVENT_TALK, ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIDidntMindBeingATree) },
    { EVENT_TALK, ACTOR_GIRL_SLEEPY_TREES, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaYawnTreesAreSoSleepy) },
    { EVENT_TALK, ACTOR_HEALER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT_SCRIPT(Kolima_TalkToHealer) },
    { EVENT_TALK, ACTOR_GRANITE, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_GraniteJoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaForAWhileIThoughtId) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_RELYING_ON_FORESTS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaTheForestTreesAreAliveJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TIME_TO_THINK, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIWonderIfIdLiveLonger) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SPARKLES_FELL, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfThoseSparklesStartFallingAgain) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_MCCOYS_MONEY, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIveBeenPoorAllMyLife) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_HOLY_TREE_PALACE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaItsNotCalledTheHolyTree) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfWeWereAllTreesThered) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SLEEPY_TREES, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaNobodyYellsAtYouWhenYoure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfTheHolyTreeAloneCaused) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS, CONDITION_ALWAYS, MSGNO(MsgKolimaIfICouldSpeakIdTell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_RELYING_ON_FORESTS, CONDITION_ALWAYS, MSGNO(MsgKolimaHowCouldAnyoneTurnAGirl) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TIME_TO_THINK, CONDITION_ALWAYS, MSGNO(MsgKolimaThisIsADreamItHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SPARKLES_FELL, CONDITION_ALWAYS, MSGNO(MsgKolimaBeingUnableToMoveIsSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_MCCOYS_MONEY, CONDITION_ALWAYS, MSGNO(MsgKolimaIAmOldIveLivedA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_HOLY_TREE_PALACE, CONDITION_ALWAYS, MSGNO(MsgKolimaItsTheSacredTreesCurseWhy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE, CONDITION_ALWAYS, MSGNO(MsgKolimaTheWaterAtMyFeetHuh) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SLEEPY_TREES, CONDITION_ALWAYS, MSGNO(MsgKolimaIfICantMoveThenMom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgKolimaMaybeIfIMeditateHarderIll) },
    { SEARCH(SEARCH_JAR), 100, 0xf57, FIND_ITEM(ITEM_APPLE) },
    { SEARCH(SEARCH_JAR), 101, 0xf58, FIND_ITEM(ITEM_HERB) },
    { SCENE_EVENTS_END },
};

/// Kolima Bridge's events: its two ends, the bridge's fall and the villager.
struct SceneEvent gKolimaBridgeEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, KOLIMA_BRIDGE_TO_WORLD_MAP_SOUTH },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, KOLIMA_BRIDGE_TO_WORLD_MAP_NORTH },
    { EVENT_TOUCH, 10, FLAG_KOLIMA_BRIDGE_LOWERED, EVENT_SCRIPT(Kolima_LowerBridge) },
    { EVENT_TALK, ACTOR_BRIDGE_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgKolimaForAWhileIThoughtNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIDGE_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgKolimaYouWouldntUnderstandButItsTruly) },
    { SCENE_EVENTS_END },
};
// clang-format on

s32 gKolima_Object26Script[] = {
    SC_CALL(FieldEffect_StretchUntilArrived),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolima_RegionScript[] = {
    SC_SET(SW_SCALE_X, 2 * Q16_ONE),
    SC_SET(SW_SCALE_Y, 2 * Q16_ONE),
    SC_WAIT(10),
    SC_FREE,
};

s32 gFallingEffectScript[] = {
    SC_ADD(SW_Y, -PIXELS(2)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_ADD(SW_Y, -PIXELS(2)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_Y, -PIXELS(1)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(1)),
    SC_CALL(Kolima_TriggerRegionScript),
    SC_FREE,
};
