/// @file
/// Kolima's overlay, which holds three maps. The
/// village's houses and shops are another overlay (kolima_interiors.c), and so
/// is each map of Kolima Forest.
///
/// - Kolima (MapId_Kolima), the village of lumberjacks, to which Lord
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
///   Kolima_GetObjectEventTemplates gives the nine villagers
///   (LOCALID_FIRST_VILLAGER to LOCALID_LAST_VILLAGER) tree sprites that stand
///   still, sparkles twinkle on the ground (LOCALID_FIRST_SPARKLE and the three
///   after it, and five more that Kolima_Initialize places), and the villagers
///   answer only Mind Read, with the thoughts of people stuck as trees.
///   Afterwards they are people again and talk of the curse, the holy tree and
///   McCoy's money, and the healer among them (LOCALID_HEALER) offers the
///   Sanctum's services across his counter. Granite, a Venus Djinni
///   (LOCALID_GRANITE), waits in the village and joins when spoken to. Five
///   doorways (KOLIMA_TO_INN to KOLIMA_TO_HOUSE_NORTH) lead into the houses and
///   shops (MapId_KolimaInteriors), KOLIMA_TO_WORLD_MAP and
///   KOLIMA_TO_WORLD_MAP_EAST to the world map and KOLIMA_TO_KOLIMA_TUNNEL
///   into Kolima Tunnel.
/// - Kolima Tunnel (MapId_KolimaTunnel), a passage with no one in it from
///   one of the houses (KOLIMA_INTERIORS_FROM_KOLIMA_TUNNEL) to the village
///   (KOLIMA_FROM_KOLIMA_TUNNEL). It shares the village's event table, whose
///   warps KOLIMA_TO_WORLD_MAP and KOLIMA_TO_WORLD_MAP_EAST are, by the same
///   numbers, its two ways out (KOLIMA_TUNNEL_TO_KOLIMA_INTERIORS and
///   KOLIMA_TUNNEL_TO_KOLIMA). That table's two jars, an Apple and an Herb
///   (bg events 100 and 101), are in the village: only Kolima's layout
///   (layout_037's treasure_spots.tsv) places them, at cells (23, 8) and (13,
///   23).
/// - Kolima Bridge (MapId_KolimaBridge), "the grand bridge across the
///   river" and "the pride of Kolima" to the village's armor seller, with the
///   world map at both ends. It is a drawbridge, standing raised between its
///   posts. A villager stands beside it, a tree while the curse lasts
///   (LOCALID_BRIDGE_VILLAGER_AS_TREE) and a man afterwards
///   (LOCALID_BRIDGE_VILLAGER). The first time the party then steps up to the
///   bridge (coord event 10), Kolima_LowerBridge plays: he asks whether they
///   are going to cross, hops aside and lets the drawbridge down across the
///   river in front of the leader (FLAG_KOLIMA_BRIDGE_LOWERED; its recording,
///   places/kolima-bridge/events/Kolima_LowerBridge.gif). From then on
///   he says that for a while he thought no one would ever cross it again,
///   Kolima_RestoreBridgeState draws it down on every later visit, and the
///   bridge on the world map is drawn down with the same flag.
///
/// Open questions: none.

#include "action_script.h"
#include "djinn_ids.h"
#include "flags.h"
#include "character_ids.h"
#include "game_state.h"
#include "heap_slots.h"
#include "map_ids.h"
#include "unit/map/kolima.h"
#include "unit/map/common/kolima_forest_bridge_falling_effect.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "script_layer.h"
#include "field_service.h"
#include "map_tables.h"
#include "kolima.h"
#include "object.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "map_header.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "item_ids.h"

static void Kolima_WitnessForestCurse(void);
static void Kolima_ShowMessageAndWait(s32 localId, s32 waitFrames);
static void Kolima_FaceObjectEventAndWait(s32 localId, s32 facing,
                                          s32 waitFrames);
static void Kolima_SpawnCurseSparkle(void);
static void Kolima_RestoreBridgeState(void);

/// Object events of Kolima, numbering the rows of gKolimaObjectEventTemplates
/// from LOCALID_FIRST_PLACED: Garet, Ivan and Mia, who stand by for the curse,
/// take their own object event slots. The nine villagers are named by what they
/// say once the curse is lifted.
enum KolimaLocalId {
  /// A young man with brown hair.
  /// ★ Named from the game: "This is Kolima, village of lumberjacks!"
  /// (MsgKolimaThisIsKolimaVillageOfLumberjacks)
  LOCALID_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS = LOCALID_FIRST_PLACED,
  /// A woman with fair hair, whose mind Ivan reads in
  /// Kolima_WitnessForestCurse.
  /// ★ Named from the game: "We rely on the forests so much, and yet we hurt
  /// the sacred tree..." (MsgKolimaWeRelyOnTheForestsSo)
  LOCALID_VILLAGER_RELYING_ON_FORESTS,
  /// A burly grey-haired man.
  /// ★ Named from the game: "You know, being a tree gives you a lot of time to
  /// think." (MsgKolimaYouKnowBeingATreeGives)
  LOCALID_VILLAGER_TIME_TO_THINK,
  /// A woman in a green dress.
  /// ★ Named from the game: "I turned into a tree right when the sparkles fell
  /// from the sky." (MsgKolimaITurnedIntoATreeRight)
  LOCALID_VILLAGER_SPARKLES_FELL,
  /// An old man with a cane.
  /// ★ Named from the game: "Don't blame the villagers for being tempted by
  /// McCoy's money..." (MsgKolimaDontBlameTheVillagersForBeing)
  LOCALID_OLD_MAN_MCCOYS_MONEY,
  /// An old woman.
  /// ★ Named from the game: "Who would try to use the holy tree to build a
  /// palace, anyway?" (MsgKolimaWhoWouldTryToUseThe)
  LOCALID_OLD_WOMAN_HOLY_TREE_PALACE,
  /// A small boy with fair hair.
  /// ★ Named from the game: "I didn't mind being a tree... Water never tasted
  /// so good!" (MsgKolimaIDidntMindBeingATree)
  LOCALID_BOY_DID_NOT_MIND_BEING_A_TREE,
  /// A little girl with pigtails.
  /// ★ Named from the game: "Yawn... Trees are so... sleepy..."
  /// (MsgKolimaYawnTreesAreSoSleepy)
  LOCALID_GIRL_SLEEPY_TREES,
  /// A man with a white beard in a tall hat and a long robe; opens the
  /// Sanctum's services across his counter (Sanctum_Run).
  /// ★ Named from the game: "Maybe if I meditate harder, I'll turn back into a
  /// man..." (MsgKolimaMaybeIfIMeditateHarderIll)
  LOCALID_HEALER,
  /// Tret's wrathful side, unseen while the curse falls.
  /// ★ Named from the game: "Tret was once a kindly forest king."
  /// (MsgKolimaWatchOutItsHappeningAgain)
  LOCALID_TRET,
  /// Unseen while the curse falls.
  /// ★ Named from the game: "We are not the only ones who will die, Laurel."
  /// (MsgKolimaWatchOutItsHappeningAgain)
  LOCALID_LAUREL,
  /// The fields of Psynergy that rise over the party while the curse falls
  /// ("You know what those fields were?"), each set where its character
  /// stands in Kolima_WitnessForestCurse; Mia's only with her in the party.
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  LOCALID_LEADER_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  LOCALID_IVAN_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  LOCALID_MIA_PSYNERGY_FIELD,
  /// ★ Named from the game: "You know what those fields were?"
  /// (MsgKolimaKnowThoseFieldsWere)
  LOCALID_GARET_PSYNERGY_FIELD,
  /// The four sparkles on the ground while the curse lasts
  /// (gKolima_EntryActions).
  /// ★ Named from the game: "I turned into a tree right when the sparkles
  /// fell from the sky." (MsgKolimaITurnedIntoATreeRight)
  LOCALID_FIRST_SPARKLE,
  /// Granite, a Venus Djinni, placed until it has joined.
  /// ★ Named from the game: "Granite" (MsgDjinnName)
  LOCALID_GRANITE = LOCALID_FIRST_SPARKLE + 4,
  /// ★ Named from the game: "I need to help evacuate the other villagers."
  /// (MsgValeKyleAbleStop)
  LOCALID_FIRST_VILLAGER = LOCALID_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS,
  /// ★ Named from the game: "I need to help evacuate the other villagers."
  /// (MsgValeKyleAbleStop)
  LOCALID_LAST_VILLAGER = LOCALID_HEALER
};

/// Kolima Bridge's villager, numbering the rows of
/// gKolimaBridgeObjectEventTemplates from LOCALID_FIRST_PLACED: the same
/// villager as a man and as a tree, in the same place.
enum KolimaBridgeLocalId {
  /// A burly grey-haired man.
  /// ★ Named from the game: "For a while, I thought no one would ever cross
  /// this bridge again." (MsgKolimaForAWhileIThoughtNo)
  LOCALID_BRIDGE_VILLAGER = LOCALID_FIRST_PLACED,
  /// The same villager as a tree, while the curse lasts.
  /// ★ Named from the game: "For a while, I thought no one would ever cross
  /// this bridge again." (MsgKolimaForAWhileIThoughtNo)
  LOCALID_BRIDGE_VILLAGER_AS_TREE
};

/// Applies the deferred pose change requested by the falling-effect script.
static s32 Kolima_ApplyPendingPose(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  if (gKolima_PendingPose != 0) {
    ObjectDispatch_SelectAnimation(objectEvent, 2);
    gKolima_PendingPose = 0;
  }
  return 1;
}

/// Each frame of a sparkle on Kolima's ground: its timer runs on at random, and
/// it shows its bright frame (7) past 1000 and its dim one (10) before; at
/// 1200 the timer starts over.
static s32
Kolima_UpdateTimedAppearance(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s32 randomValue = Engine_RandomNext();
  s32 scaledRandom = randomValue * 100;
  s32 appearanceTimer = (u16)objectEvent->timer + ((u32)scaledRandom >> 16);
  objectEvent->timer = appearanceTimer;
  if ((s16)appearanceTimer > 1000) {
    ObjectGroup_SetChildValue(objectEvent, 7);
  } else {
    ObjectGroup_SetChildValue(objectEvent, 10);
  }
  if ((s16)objectEvent->timer > 1200) {
    objectEvent->timer = 0;
  }
  return 1;
}

/// While the curse lasts, gives Kolima's nine villagers (object event template
/// rows 3 to 11, LOCALID_FIRST_VILLAGER to LOCALID_LAST_VILLAGER) tree sprites,
/// SPRITE_TREE or, for rows 9 and 10 (LOCALID_BOY_DID_NOT_MIND_BEING_A_TREE and
/// LOCALID_GIRL_SLEEPY_TREES), SPRITE_SMALL_TREE; they stand still and keep
/// their facing when spoken to.
static void
Kolima_InitializeCursedVillagers(struct ObjectEventTemplate *templates) {
  struct ObjectEventTemplate *villager = &templates[3];
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
    villager->talkFacing = TALK_FACING_KEEP;
    villager->movement = MOVEMENT_TYPE_STAND;
    villager++;
  }
}

/// Each frame of a piece of Kolima Bridge as it falls: it moves, grows, and is
/// released when its timer runs out.
static s32 Kolima_UpdateFallingDebris(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *debris = (struct ObjectEvent *)interpreter;
  debris->x.fixed += debris->velocityX;
  debris->z.fixed += debris->velocityZ;
  debris->velocityZ -= PIXELS(1) / 25;
  debris->scaleX += 3 * Q16_ONE / 128;
  debris->scaleY += 3 * Q16_ONE / 128;
  {
    s32 framesLeft = (u16)debris->timer - 1;
    debris->timer = framesLeft;
    if ((u16)framesLeft == 0) {
      ObjectEvent_Release(debris);
    }
  }
  return 1;
}

#include "common/object_facing.h"

/// Selects the warp destinations of Kolima, Kolima Tunnel or Kolima Bridge.
const struct MapWarpDest *Kolima_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_KolimaBridge) {
    return gKolimaBridgeWarpDests;
  }
  if (mapId == (s32)&MapId_KolimaTunnel) {
    return gKolimaTunnelWarpDests;
  }
  return gKolimaWarpDests;
}

/// Only Kolima Tunnel has regions: its two ways out,
/// KOLIMA_TUNNEL_TO_KOLIMA_INTERIORS and KOLIMA_TUNNEL_TO_KOLIMA.
const struct MapWarpRegion *Kolima_GetRegions(void) {
  if (gGameState.mapId == (s32)&MapId_KolimaTunnel) {
    return gKolimaTunnelRegions;
  }
  return 0;
}

/// The warps of all three maps, each map's after its header word (a warp
/// of kind 0): Kolima's to the world map, the houses (MapId_KolimaInteriors)
/// and Kolima
/// Tunnel, Kolima Bridge's to the world map, and Kolima Tunnel's to a house
/// and the village.
const u32 *Kolima_GetWarps(void) { return gKolimaWarps; }

/// Selects the people of Kolima or Kolima Bridge; Kolima Tunnel has none. In
/// Kolima the villagers are trees until the curse is lifted.
const struct ObjectEventTemplate *Kolima_GetObjectEventTemplates(void) {
  s32 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_Kolima) {
    if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Kolima_InitializeCursedVillagers(gKolimaObjectEventTemplates);
    }
    return gKolimaObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_KolimaBridge) {
    return gKolimaBridgeObjectEventTemplates;
  }
  return gKolimaTunnelObjectEventTemplates;
}

/// Kolima, once the curse is lifted: talking to the healer (LOCALID_HEALER)
/// across his counter (facing north) opens the Sanctum's services; from any
/// other side he asks whether the holy tree's rage was the only cause of the
/// curse, and either answer agrees that something changed when the gems fell.
static void Kolima_TalkToHealer(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 facing = leader->facing;
  FieldScript_Start();
  if (FACING_IS_NORTH(facing)) {
    Sanctum_Run(LOCALID_HEALER);
  } else {
    FieldScript_SetMessageId(MsgKolimaToldHolyTrees);
    FieldScript_AskYesNoAndShowResponse(LOCALID_HEALER, 0);
  }
  FieldScript_End();
}

/// Kolima: talking to Granite, a Venus Djinni, makes it join the party.
static void Kolima_GraniteJoins(void) {
  Djinn_JoinParty(LOCALID_GRANITE, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GRANITE));
}

/// Selects Kolima Bridge's map events, or the table Kolima and Kolima Tunnel
/// share.
const struct MapEvent *Kolima_GetEvents(void) {
  if (gGameState.mapId == (s32)&MapId_KolimaBridge) {
    return gKolimaBridgeEvents;
  }
  return gKolimaEvents;
}

/// Walks the leader north through a door of Kolima into the warp of the same
/// number.
static void Kolima_WalkThroughExit(s32 warpNumber) {
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, -8);
  gFieldWork->transitionFrames = 16;
  Engine_ScriptRequestWarp(warpNumber);
}

/// Kolima: walking into one of the five doorways into the houses and shops,
/// whose coord events are the numbers of the warps they take (KOLIMA_TO_INN to
/// KOLIMA_TO_HOUSE_NORTH). The door sound plays; the four entered walking
/// north open their door (gKolima_DoorCells) and the leader walks in, while
/// KOLIMA_TO_HOUSE_NORTH, entered walking south, has no door to draw.
static void Kolima_EnterDoor(void) {
  struct FieldWork *work = gFieldWork;
  s32 x = 0;
  s32 y = 0;

  FieldScript_Start();
  Sound_PlaySong(SE_DOOR_OPEN);
  switch (work->coordEvent) {
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
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    ObjectMotion_OffsetPositionAndResetMotion(LOCALID_PARTY_LEADER, 0, 8);
    gFieldWork->transitionFrames = 16;
    Engine_ScriptRequestWarp(KOLIMA_TO_HOUSE_NORTH);
    FieldScript_End();
    return;
  }
  MapGrid_PlayMetatileCopySequence(gKolima_DoorCells, x, y);
  Kolima_WalkThroughExit(work->coordEvent);
  FieldScript_End();
}

/// Kolima Bridge, the first time the party steps up to it (coord event 10,
/// until FLAG_KOLIMA_BRIDGE_LOWERED): the villager beside it
/// (LOCALID_BRIDGE_VILLAGER) asks whether they are going to cross, and at the
/// leader's nod says "Go right ahead!" and hops aside to the post. The
/// drawbridge then comes down across the river in front of the leader in four
/// ever quicker steps of map cells, ten puffs of dust fly up where it lands,
/// the screen shakes and the leader jumps back. The last cells copied also
/// give the bridge the metatile behaviors that open it. From then on the
/// villager keeps turning to face the leader. Sets FLAG_KOLIMA_BRIDGE_LOWERED.
static void Kolima_LowerBridge(void) {
  u32 debrisIndex;

  FieldScript_Start();
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(168), 0, PIXELS(246), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_BRIDGE_VILLAGER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BRIDGE_VILLAGER, 2);
  FieldScript_SetMessageId(MsgKolimaGoingCrossBridge);
  FieldScript_ShowMessageAndWait(LOCALID_BRIDGE_VILLAGER, 0);
  FieldCamera_SetSpeedParameters(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(168), 0, PIXELS(234), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 174, 0x116);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Engine_ObjectEventSetAnimation(LOCALID_BRIDGE_VILLAGER, ANIM_NOD);
  FieldScript_ShowMessageAndWait(LOCALID_BRIDGE_VILLAGER, 0);
  ObjectEvent_FaceDirection(LOCALID_BRIDGE_VILLAGER, FACING_WEST + FACING_STEP,
                            20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BRIDGE_VILLAGER, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_GetById(LOCALID_BRIDGE_VILLAGER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetSpeed(LOCALID_BRIDGE_VILLAGER, PIXELS(2), PIXELS(1));
  ObjectMotion_Launch(LOCALID_BRIDGE_VILLAGER, 2, 0);
  ObjectMotion_SetPositionAndCommit(LOCALID_BRIDGE_VILLAGER, 224, 197);
  Sound_PlaySong(SE_176);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetPositionAndCommit(LOCALID_BRIDGE_VILLAGER, 234, 200);
  FieldScript_WaitFrames(10);
  Sound_PlaySong(SE_198);
  FieldScript_WaitFrames(30);
  // The drawbridge comes down in ever shorter steps.
  MapGrid_CopyCellsTo(91, 0, 72, 9, 5, 4);
  FieldScript_WaitFrames(12);
  MapGrid_CopyCellsTo(91, 4, 72, 9, 5, 4);
  FieldScript_WaitFrames(9);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  MapGrid_CopyCellsTo(91, 8, 72, 9, 5, 5);
  FieldScript_WaitFrames(6);
  MapGrid_CopyCellsTo(91, 13, 72, 9, 5, 6);
  FieldScript_WaitFrames(3);
  Sound_PlaySong(SE_188);
  // Ten puffs of dust where it lands, thrown alternately left and right.
  for (debrisIndex = 0; debrisIndex < 10; debrisIndex++) {
    struct ObjectEvent *debris = ObjectEvent_Create(
        SPRITE_DUST_PUFF, PIXELS(148 + debrisIndex * 4), 0, PIXELS(258));

    if (debris != 0) {
      debris->motionFlags = 0;
      ObjectEvent_SetSpriteFlags(debris, 0);
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
      ObjectDispatch_SelectAnimation(debris, 1);
      ObjectEvent_SetScript(debris, gKolima_DebrisScript);
    }
  }
  MapGrid_CopyCellsTo(91, 19, 72, 9, 5, 7);
  MapGrid_CopyCellAttributes(23, 11, 5, 7, 8, 11);
  MapGrid_SetShake(0, PIXELS(4), Q16_ONE);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
  FieldScript_WaitFrames(20);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  // From now on the villager turns to keep facing the leader.
  Object_SetTargetAndCallback(LOCALID_BRIDGE_VILLAGER,
                              OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_PARTY_LEADER,
                              gKolima_FaceLeaderScript);
  FieldScript_WaitFrames(60);
  FieldEffect_SpawnLinked(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          60);
  GameFlag_Set(FLAG_KOLIMA_BRIDGE_LOWERED);
  FieldScript_End();
}

/// Sets up whichever map is entered: Kolima Bridge as the curse and the
/// bridge leave it (Kolima_RestoreBridgeState), Kolima Tunnel with a window
/// transition, and Kolima itself. There the four sparkles twinkle, the
/// villagers stand as trees while the curse lasts (and three cells around
/// (14, 8) take the metatile behavior of cell (13, 9)), and the first entry
/// from the world map plays Kolima_WitnessForestCurse. Once that has been seen,
/// here or in Kolima Forest, Garet, Ivan and Mia, the two voices and the domes
/// are taken away and five more sparkles are placed.
s32 Kolima_Initialize(void) {
  s16 mapId;
  u32 localId;
  const s32 *table;

  mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_KolimaBridge) {
    Kolima_RestoreBridgeState();
    return 0;
  }

  if (mapId == (s32)&MapId_KolimaTunnel) {
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    return 0;
  }

  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FIRST_SPARKLE), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FIRST_SPARKLE + 1), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FIRST_SPARKLE + 2), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FIRST_SPARKLE + 3), 0);

  table = gKolima_EntryActions;
  ObjectEvent_SetActionScript(LOCALID_FIRST_SPARKLE, table);
  ObjectEvent_SetActionScript(LOCALID_FIRST_SPARKLE + 1, table);
  ObjectEvent_SetActionScript(LOCALID_FIRST_SPARKLE + 2, table);
  ObjectEvent_SetActionScript(LOCALID_FIRST_SPARKLE + 3, table);

  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    for (localId = LOCALID_FIRST_VILLAGER; localId <= LOCALID_LAST_VILLAGER;
         localId++) {
      ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(localId), 0);
    }
    MapGrid_CopyCellAttributes(13, 9, 1, 1, 13, 8);
    MapGrid_CopyCellAttributes(13, 9, 1, 1, 15, 8);
    MapGrid_CopyCellAttributes(13, 9, 1, 1, 14, 9);
  }

  if (GameFlag_Get(FLAG_KOLIMA_CURSE_WITNESSED) == 0) {
    if (gGameState.warpDest == KOLIMA_FROM_WORLD_MAP) {
      Kolima_WitnessForestCurse();
    }
  }

  if (GameFlag_Get(FLAG_KOLIMA_CURSE_WITNESSED) != 0) {
    ObjectEvent_DestroyById(GARET);
    ObjectEvent_DestroyById(IVAN);
    ObjectEvent_DestroyById(MIA);
    ObjectEvent_DestroyById(LOCALID_TRET);
    ObjectEvent_DestroyById(LOCALID_LAUREL);
    ObjectEvent_DestroyById(LOCALID_LEADER_PSYNERGY_FIELD);
    ObjectEvent_DestroyById(LOCALID_IVAN_PSYNERGY_FIELD);
    ObjectEvent_DestroyById(LOCALID_MIA_PSYNERGY_FIELD);
    ObjectEvent_DestroyById(LOCALID_GARET_PSYNERGY_FIELD);
    FieldScript_CallWithLastActiveObjectId(gKolima_EntryScript);
  }

  return 0;
}

/// Shows one of the domes of Psynergy on its frames, even or odd, and lifts it
/// 500 pixels out of sight on the others.
static __inline__ void Kolima_SetProtectiveFieldHeight(s32 localId,
                                                       s32 visibleOnOddFrames) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);
  if (objectEvent != NULL) {
    objectEvent->motionFlags = 0;
    if (visibleOnOddFrames ? (gFrameCount & 1) : !(gFrameCount & 1)) {
      objectEvent->y.fixed = 0;
    } else {
      objectEvent->y.fixed = PIXELS(500);
    }
  }
}

/// Makes the four domes flicker, two on even frames and two on odd.
static void Kolima_AlternateProtectiveFieldHeights(void) {
  Kolima_SetProtectiveFieldHeight(LOCALID_LEADER_PSYNERGY_FIELD, 0);
  Kolima_SetProtectiveFieldHeight(LOCALID_IVAN_PSYNERGY_FIELD, 1);
  Kolima_SetProtectiveFieldHeight(LOCALID_MIA_PSYNERGY_FIELD, 0);
  Kolima_SetProtectiveFieldHeight(LOCALID_GARET_PSYNERGY_FIELD, 1);
}

/// Begins Mind Read on one of Kolima's villagers turned into a tree.
static void Kolima_ReadVillagerThoughts(s32 localId, s32 targetId) {
  FieldEffect_LoadActionEffectResources(ABILITY_MIND_READ, 1);
  FieldEffect_SetupObjectPair(localId, targetId);
  FieldEffect_StartCast();
  Field_DispatchTypeHandler(1);
  WaitFrames(1);
}

/// Finishes Mind Read and releases its temporary scene objects.
static void Kolima_FinishReadingThoughts(void) {
  Field_DispatchTypeHandler(2);
  FieldEffect_StopCasterHook();
  FieldEffect_End();
}

/// Gives the four companions the same nod, waiting on the final object event.
static __inline__ void Kolima_NodTogether(void) {
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
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
  struct ObjectEvent *objectEvent;
  s32 *partyLayout;
  s32 motionFacingFlag;
  // One staging word is reused for facing bytes, jump values and flags.
  s32 stagingValue;
  const s32 *squashActions;
  const s32 *restoreScaleActions;
  u32 *firstCurtainState;
  s32 *mapState;
  const s32 *growFieldActions;
  const s32 *shrinkFieldActions;
  u32 *secondCurtainState;

  // Enter the deserted village and let Ivan inspect one of its trees.
  FieldScript_Start();
  partyLayout = &gKolima_MiaInParty;
  *partyLayout = GameFlag_Get(FLAG_IN_PARTY(MIA));
  objectEvent = ObjectEvent_GetById(LOCALID_LEADER_PSYNERGY_FIELD);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_IVAN_PSYNERGY_FIELD);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_MIA_PSYNERGY_FIELD);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_GARET_PSYNERGY_FIELD);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  FieldCamera_MoveTo(PIXELS(104), -1, PIXELS(256), 0);
  MapGrid_CenterOnFollowedPoint();
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(23), PIXELS(247));
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 121, 238);
  ObjectEvent_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetPosition(GARET, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetPosition(IVAN, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  ObjectEvent_SetActionScript(GARET, gKolima_GaretWalksIn);
  ObjectEvent_SetActionScript(IVAN, gKolima_IvanWalksIn);
  if (*partyLayout != 0) {
    ObjectEvent_SetSpeed(MIA, PIXELS(3) / 5, PIXELS(3) / 10);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != NULL) {
      ObjectEvent_SetPosition(MIA, objectEvent->x.fixed, objectEvent->z.fixed);
    }
    ObjectEvent_SetActionScript(MIA, gKolima_MiaWalksIn);
  }
  ObjectEvent_RefreshSelectorById(IVAN);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 20);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  partyLayout = &gKolima_MiaInParty;
  if (*partyLayout != 0) {
    ObjectEvent_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  }
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 20);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHEAST, 20);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 60);
  if (*partyLayout != 0) {
    ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 0);
  }
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceObjectEvent(IVAN, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_PARTY_LEADER, 0);
  if (*partyLayout != 0) {
    ObjectEvent_FaceObjectEvent(MIA, LOCALID_PARTY_LEADER, 0);
  }
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(GARET, 1);
  FieldScript_SetMessageId(MsgKolimaQuiet);
  Kolima_ShowMessageAndWait(GARET, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, GARET, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(30);
  ObjectEvent_WalkToAndWait(IVAN, 72, 0x11e);
  ObjectEvent_WalkToAndWait(IVAN, 72, 0x12e);
  ObjectEvent_WalkToAndWait(IVAN, 88, 0x136);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  ObjectEvent_FaceObjectEvent(IVAN, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
  if (*partyLayout != 0) {
    ObjectEvent_FaceObjectEvent(MIA, IVAN, 0);
  }
  FieldScript_WaitFrames(30);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  if (*partyLayout != 0) {
    Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
  }
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(IVAN, FACING_EAST, 0);
  FieldScript_WaitFrames(20);
  // Mind Read reveals that the trees are the missing villagers.
  Kolima_ReadVillagerThoughts(IVAN, LOCALID_VILLAGER_RELYING_ON_FORESTS);
  FieldScript_WaitFrames(40);
  Kolima_FinishReadingThoughts();
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_GetById(IVAN)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(IVAN, 80, 0x136);
  motionFacingFlag = OBJ_EVENT_FACES_MOTION;
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(IVAN)->steeringFlags |= motionFacingFlag;
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Kolima_ShowMessageAndWait(GARET, 20);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Kolima_ShowMessageAndWait(IVAN, 20);
  if (*partyLayout != 0) {
    ObjectEvent_FaceObjectEvent(MIA, LOCALID_PARTY_LEADER, 0);
  }
  ObjectEvent_LinkPair(IVAN, GARET, 60);
  if (*partyLayout != 0) {
    ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  }
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  ObjectMotion_SetVariantCallback(GARET, 1);
  Kolima_ShowMessageAndWait(GARET, 20);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_GetById(IVAN)->steeringFlags &= 254;
  ObjectEvent_WalkToAndWait(IVAN, 72, 0x11e);
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(IVAN)->steeringFlags |= motionFacingFlag;
  ObjectEvent_SetActionScript(IVAN, gKolima_IvanWalksIn);
  if (*partyLayout != 0) {
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    FieldScript_WaitFrames(60);
    Kolima_ShowMessageAndWait(MIA, 20);
    Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
  } else {
    gFieldWork->message++;
  }
  ObjectEvent_FaceObjectEvent(IVAN, LOCALID_PARTY_LEADER, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(10);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 10);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 10);
  Script_OpenMessage(GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  } else {
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
    gFieldWork->message++;
  }
  Kolima_ShowMessageAndWait(GARET, 40);
  Kolima_FaceObjectEventAndWait(IVAN, FACING_SOUTHEAST, 40);
  Kolima_FaceObjectEventAndWait(IVAN, FACING_WEST, 20);
  Kolima_FaceObjectEventAndWait(IVAN, FACING_SOUTH, 40);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 60);
  stagingValue = 160;
  Kolima_FaceObjectEventAndWait(MIA, FACING_SOUTHEAST, 10);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, (stagingValue << 8), 10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 10);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  FieldScript_SetMessageId(MsgKolimaWhatIsIt);
  Kolima_ShowMessageAndWait(GARET, 10);
  Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTH, 20);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  Kolima_ShowMessageAndWait(IVAN, 20);
  Kolima_FaceObjectEventAndWait(GARET, 0, 20);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, (stagingValue << 8), 40);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTH, 20);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 30);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTHWEST, 20);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 30);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 20);
  Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTH, 10);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  Script_PlaySong(SE_206);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_StartInterpolation(1);
  WaitFrames(1);
  // The falling sparkles threaten to turn the party into trees as well.
  gKolima_EffectActive = 1;
  FieldScheduler_AddCallback(Kolima_SpawnCurseSparkle, TASK_PRIORITY_MAP);
  WaitFrames(20);
  PaletteFade_ApplyTarget(0x405210, 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 2);
  PaletteFade_StartInterpolation(120);
  WaitFrames(60);
  squashActions = gKolima_PartySquashActions;
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, squashActions);
  ObjectEvent_SetActionScript(GARET, squashActions);
  ObjectEvent_SetActionScript(IVAN, squashActions);
  ObjectEvent_SetActionScript(MIA, squashActions);
  FieldScript_WaitFrames(100);
  Kolima_ShowMessageAndWait(GARET, 20);
  Kolima_ShowMessageAndWait(IVAN, 40);
  if (gKolima_MiaInParty != 0) {
    FieldScript_WaitFrames(40);
    ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    Kolima_ShowMessageAndWait(MIA, 40);
  } else {
    gFieldWork->message++;
  }
  FieldScript_WaitFrames(20);
  if (gKolima_MiaInParty != 0) {
    stagingValue = 128;
    objectEvent = ObjectEvent_GetById(MIA);
    objectEvent->velocityY = (stagingValue << 10);
    FieldScript_WaitFrames(10);
    ObjectEvent_SetSpeed(MIA, (stagingValue << 10), (stagingValue << 10));
    ObjectEvent_SetDestinationOffset(MIA, -2, 0);
    ObjectEvent_SetActionScript(MIA, gKolima_PartyRestoreScaleActions);
    objectEvent = ObjectEvent_GetById(MIA);
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    Engine_ObjectEventSetAnimation(MIA, ANIM_COLLAPSE);
    FieldScript_WaitFrames(10);
  }
  stagingValue = 128;
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->velocityY = (stagingValue << 10);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, (stagingValue << 10),
                       (stagingValue << 10));
  restoreScaleActions = gKolima_PartyRestoreScaleActions;
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, restoreScaleActions);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_COLLAPSE);
  FieldScript_WaitFrames(20);
  objectEvent = ObjectEvent_GetById(GARET);
  objectEvent->velocityY = (stagingValue << 10);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetSpeed(GARET, (stagingValue << 10), (stagingValue << 10));
  ObjectEvent_SetActionScript(GARET, restoreScaleActions);
  objectEvent = ObjectEvent_GetById(GARET);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIM_COLLAPSE);
  FieldScript_WaitFrames(40);
  objectEvent = ObjectEvent_GetById(IVAN);
  objectEvent->velocityY = (stagingValue << 10);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetActionScript(IVAN, restoreScaleActions);
  objectEvent = ObjectEvent_GetById(IVAN);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_COLLAPSE);
  gKolima_EffectActive = 0;
  FieldScript_WaitFrames(160);
  Task_Remove(Kolima_SpawnCurseSparkle);
  FieldScript_WaitFrames(120);
  PaletteFade_ApplyTarget(0x406218, 1);
  PaletteFade_StartInterpolation(60);
  WaitFrames(60);
  // Bring the falling curtain over the party and reveal their protective
  // fields.
  gFallingEffectWidth = 0;
  firstCurtainState = &gFallingEffectState;
  gFallingEffectOffset = PIXELS(128);
  *firstCurtainState = KOLIMA_DUST_CLOSING;
  FieldScheduler_AddCallback(Kolima_UpdateCurseDust, TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(180);
  Script_PlaySong(MUS_021);
  Kolima_ShowMessageAndWait(GARET, 80);
  Kolima_ShowMessageAndWait(IVAN, 40);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                         ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Kolima_ShowMessageAndWait(IVAN, 20);
  *firstCurtainState = KOLIMA_DUST_CLOSING_WIDE;
  ObjectMotion_SetVariantCallback(IVAN, 2);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(GARET, 1);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(MIA, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(IVAN, 3);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(GARET, 2);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(MIA, 2);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  Kolima_ShowMessageAndWait(GARET, 20);
  if (gKolima_MiaInParty != 0) {
    ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
    Kolima_ShowMessageAndWait(MIA, 10);
  } else {
    gFieldWork->message++;
  }
  mapState = (s32 *)&gFallingEffectState;
  *mapState = KOLIMA_DUST_DRIFTING;
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags &= 254;
  ObjectEvent_GetById(GARET)->priorityFlags &= 254;
  ObjectEvent_GetById(IVAN)->priorityFlags &= 254;
  ObjectEvent_GetById(MIA)->priorityFlags &= 254;
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 3);
  ObjectMotion_SetActionVariant(GARET, 3);
  ObjectMotion_SetActionVariant(IVAN, 3);
  stagingValue = 0;
  ObjectMotion_SetActionVariant(MIA, 3);
  gKolima_PendingPose = stagingValue;
  FieldScheduler_AddCallback(Kolima_AlternateProtectiveFieldHeights,
                             TASK_PRIORITY_MAP);
  Script_PlaySong(SE_220);
  ObjectEvent_GetById(LOCALID_LEADER_PSYNERGY_FIELD)->priorityFlags &= 254;
  ObjectMotion_SetActionVariant(LOCALID_LEADER_PSYNERGY_FIELD, 2);
  ObjectEvent_SetPosition(LOCALID_LEADER_PSYNERGY_FIELD, PIXELS(120),
                          PIXELS(248));
  growFieldActions = gKolima_ProtectiveFieldGrowActions;
  ObjectEvent_SetActionScript(LOCALID_LEADER_PSYNERGY_FIELD, growFieldActions);
  ObjectEvent_GetById(LOCALID_IVAN_PSYNERGY_FIELD)->priorityFlags &= 254;
  ObjectMotion_SetActionVariant(LOCALID_IVAN_PSYNERGY_FIELD, 2);
  ObjectEvent_SetPosition(LOCALID_IVAN_PSYNERGY_FIELD, PIXELS(100),
                          PIXELS(274));
  ObjectEvent_SetActionScript(LOCALID_IVAN_PSYNERGY_FIELD, growFieldActions);
  if (gKolima_MiaInParty != 0) {
    ObjectEvent_GetById(LOCALID_MIA_PSYNERGY_FIELD)->priorityFlags &= 254;
    ObjectMotion_SetActionVariant(LOCALID_MIA_PSYNERGY_FIELD, 2);
    ObjectEvent_SetPosition(LOCALID_MIA_PSYNERGY_FIELD, PIXELS(74),
                            PIXELS(254));
    ObjectEvent_SetActionScript(LOCALID_MIA_PSYNERGY_FIELD, growFieldActions);
  }
  ObjectEvent_GetById(LOCALID_GARET_PSYNERGY_FIELD)->priorityFlags &= 254;
  ObjectMotion_SetActionVariant(LOCALID_GARET_PSYNERGY_FIELD, 2);
  ObjectEvent_SetPosition(LOCALID_GARET_PSYNERGY_FIELD, PIXELS(94),
                          PIXELS(225));
  ObjectEvent_SetActionScript(LOCALID_GARET_PSYNERGY_FIELD, growFieldActions);
  if (*mapState != KOLIMA_DUST_CENTRED) {
    do {
      WaitFrames(1);
    } while (gFallingEffectState != KOLIMA_DUST_CENTRED);
  }
  FieldScript_WaitFrames(300);
  Task_Remove(Kolima_UpdateCurseDust);
  FieldScript_WaitFrames(120);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
  PaletteFade_StartInterpolation(60);
  WaitFrames(60);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_LEADER_PSYNERGY_FIELD);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_IVAN_PSYNERGY_FIELD);
  if (gKolima_MiaInParty != 0) {
    ObjectMotion_EnableActionAndResetMotion(LOCALID_MIA_PSYNERGY_FIELD);
  }
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET_PSYNERGY_FIELD);
  WaitFrames(1);
  shrinkFieldActions = gKolima_ProtectiveFieldShrinkActions;
  ObjectEvent_SetActionScript(LOCALID_LEADER_PSYNERGY_FIELD,
                              shrinkFieldActions);
  ObjectEvent_SetActionScript(LOCALID_IVAN_PSYNERGY_FIELD, shrinkFieldActions);
  if (gKolima_MiaInParty != 0) {
    ObjectEvent_SetActionScript(LOCALID_MIA_PSYNERGY_FIELD, shrinkFieldActions);
  }
  ObjectEvent_RunActionScriptAndWait(LOCALID_GARET_PSYNERGY_FIELD,
                                     shrinkFieldActions);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  FieldScript_WaitFrames(40);
  Script_OpenMessage(GARET, 0);
  ObjectEvent_SetPosition(LOCALID_TRET, PIXELS(87), PIXELS(139));
  ObjectEvent_SetPosition(LOCALID_LAUREL, PIXELS(87), PIXELS(139));
  WaitFrames(1);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_TRET, 0) == 1) {
    gFieldWork->message++;
  }
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  Kolima_ShowMessageAndWait(IVAN, 20);
  if (gKolima_MiaInParty != 0) {
    ObjectMotion_SetVarCbAndRefresh(MIA, 2);
    FieldScript_WaitFrames(10);
    FieldScript_SetMessageId(MsgKolimaKnowThoseFieldsWere);
    Kolima_ShowMessageAndWait(MIA, 40);
  }
  ObjectMotion_SetVariantCallback(GARET, 1);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  // The companions discover that their Psynergy formed the barriers.
  FieldScript_SetMessageId(MsgKolimaWasOurPsynergy);
  Kolima_ShowMessageAndWait(IVAN, 40);
  ObjectMotion_SetVarCbAndRefresh(GARET, 3);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetActionVariant(GARET, 2);
  ObjectEvent_GetById(GARET)->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
  objectEvent = ObjectEvent_GetById(GARET);
  ObjectEvent_SetSpriteFlags(objectEvent, 1);
  ObjectMotion_Launch(GARET, 6, 0);
  Engine_ObjectEventSetAnimation(GARET, ANIM_STAND);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTH, 60);
  Kolima_ShowMessageAndWait(GARET, 20);
  ObjectMotion_SetVariantCallback(GARET, 2);
  Kolima_ShowMessageAndWait(GARET, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 3);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(40);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTHWEST, 40);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 20);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTHWEST, 20);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 10);
  ObjectMotion_Launch(GARET, 2, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_Launch(GARET, 2, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(GARET, 4, 0);
  FieldScript_WaitFrames(20);
  Kolima_ShowMessageAndWait(GARET, 20);
  if (gKolima_MiaInParty != 0) {
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetVarCbAndRefresh(MIA, 2);
    FieldScript_WaitFrames(80);
    ObjectMotion_SetActionVariant(MIA, 2);
    ObjectEvent_GetById(MIA)->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
    objectEvent = ObjectEvent_GetById(MIA);
    ObjectEvent_SetSpriteFlags(objectEvent, 1);
    ObjectMotion_Launch(MIA, 4, 0);
    ObjectEvent_SetDestinationOffset(MIA, -2, 0);
    Engine_ObjectEventSetAnimation(MIA, ANIM_STAND);
    Kolima_FaceObjectEventAndWait(MIA, FACING_NORTHEAST, 60);
    ObjectMotion_SetVarCbAndRefresh(MIA, 2);
    FieldScript_WaitFrames(20);
    Kolima_ShowMessageAndWait(MIA, 20);
  } else {
    gFieldWork->message++;
  }
  ObjectMotion_Launch(GARET, 2, 0);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTH, 20);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 10);
  Kolima_ShowMessageAndWait(GARET, 20);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  FieldScript_WaitFrames(20);
  stagingValue = 1;
  ObjectMotion_SetActionVariant(IVAN, 2);
  ObjectEvent_GetById(IVAN)->priorityFlags |= stagingValue;
  objectEvent = ObjectEvent_GetById(IVAN);
  ObjectEvent_SetSpriteFlags(objectEvent, 1);
  ObjectMotion_Launch(IVAN, 4, 0);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_STAND);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags |= stagingValue;
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(objectEvent, 1);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 60);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldScript_WaitFrames(60);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 20);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTH, 10);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  Kolima_ShowMessageAndWait(IVAN, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 1);
  FieldScript_WaitFrames(20);
  Script_OpenMessage(IVAN, 0);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
    gFieldWork->message++;
  } else {
    ObjectMotion_SetVarCbAndRefresh(GARET, 2);
    Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 10);
    ObjectMotion_SetVariantCallback(GARET, 2);
    Script_ShowMessage(GARET, 0);
  }
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTH, 10);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Kolima_ShowMessageAndWait(GARET, 20);
  Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTH, 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  Kolima_ShowMessageAndWait(IVAN, 10);
  if (gKolima_MiaInParty != 0) {
    ObjectMotion_SetVarCbAndRefresh(MIA, 2);
    Kolima_FaceObjectEventAndWait(MIA, 0, 20);
    Kolima_FaceObjectEventAndWait(MIA, FACING_SOUTHEAST, 10);
    Engine_ObjectEventSetAnimation(MIA, ANIM_SHAKE_HEAD);
    Kolima_ShowMessageAndWait(MIA, 10);
  } else {
    gFieldWork->message++;
  }
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  stagingValue = 128;
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Kolima_FaceObjectEventAndWait(GARET, (stagingValue << 7), 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTHEAST, 10);
  ObjectMotion_SetVariantCallback(IVAN, 2);
  Kolima_ShowMessageAndWait(IVAN, 20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 40);
  ObjectEvent_FaceDirection(GARET, (stagingValue << 7), 0);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTH, 10);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  Kolima_ShowMessageAndWait(IVAN, 10);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  Kolima_ShowMessageAndWait(GARET, 20);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  Kolima_ShowMessageAndWait(GARET, 20);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
  Kolima_ShowMessageAndWait(IVAN, 10);
  ObjectMotion_SetVarCbAndRefresh(GARET, 2);
  Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 10);
  Script_OpenMessage(GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(GARET, 2);
    FieldScript_WaitFrames(40);
    gFieldWork->message++;
  }
  Script_ShowMessage(GARET, 0);
  Script_PlaySong(MUS_021);
  PaletteFade_ApplyTarget(0x406218, 1);
  PaletteFade_StartInterpolation(60);
  WaitFrames(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = PIXELS(128);
  secondCurtainState = &gFallingEffectState;
  *secondCurtainState = KOLIMA_DUST_CLOSING;
  FieldScheduler_AddCallback(Kolima_UpdateCurseDust, TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(GARET, 2);
  ObjectMotion_SetVariantCallback(MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  FieldScript_WaitFrames(60);
  Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTH, 10);
  // A second shower raises the domes again before Tret and Laurel speak.
  FieldScript_SetMessageId(MsgKolimaWatchOutItsHappeningAgain);
  Kolima_ShowMessageAndWait(IVAN, 10);
  Kolima_FaceObjectEventAndWait(GARET, FACING_NORTH, 10);
  Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_NORTH, 10);
  if (gKolima_MiaInParty != 0) {
    Kolima_FaceObjectEventAndWait(MIA, FACING_NORTH, 10);
  }
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags &= 254;
  ObjectEvent_GetById(GARET)->priorityFlags &= 254;
  ObjectEvent_GetById(IVAN)->priorityFlags &= 254;
  ObjectEvent_GetById(MIA)->priorityFlags &= 254;
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 3);
  ObjectMotion_SetActionVariant(GARET, 3);
  ObjectMotion_SetActionVariant(IVAN, 3);
  ObjectMotion_SetActionVariant(MIA, 3);
  *secondCurtainState = KOLIMA_DUST_CLOSING_WIDE;
  Script_PlaySong(SE_220);
  ObjectEvent_SetPosition(LOCALID_LEADER_PSYNERGY_FIELD, PIXELS(120),
                          PIXELS(248));
  growFieldActions = gKolima_ProtectiveFieldGrowActions;
  ObjectEvent_SetActionScript(LOCALID_LEADER_PSYNERGY_FIELD, growFieldActions);
  ObjectEvent_SetPosition(LOCALID_IVAN_PSYNERGY_FIELD, PIXELS(100),
                          PIXELS(274));
  ObjectEvent_SetActionScript(LOCALID_IVAN_PSYNERGY_FIELD, growFieldActions);
  if (gKolima_MiaInParty != 0) {
    ObjectEvent_SetPosition(LOCALID_MIA_PSYNERGY_FIELD, PIXELS(74),
                            PIXELS(254));
    ObjectEvent_SetActionScript(LOCALID_MIA_PSYNERGY_FIELD, growFieldActions);
  }
  ObjectEvent_SetPosition(LOCALID_GARET_PSYNERGY_FIELD, PIXELS(94),
                          PIXELS(225));
  ObjectEvent_SetActionScript(LOCALID_GARET_PSYNERGY_FIELD, growFieldActions);
  FieldScript_WaitFrames(120);
  *secondCurtainState = KOLIMA_DUST_DRIFTING;
  do {
    WaitFrames(1);
  } while (gFallingEffectState != KOLIMA_DUST_CENTRED);
  // Tret and Laurel, unseen, wonder at the party's immunity to the glamour.
  Kolima_ShowMessageAndWait(LOCALID_TRET, 80);
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 20);
  Kolima_NodTogether();
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 10);
  Kolima_NodTogether();
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(40);
  Kolima_ShowMessageAndWait(LOCALID_TRET, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_EAST, 0);
  Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTH, 40);
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 10);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                         ATTACHED_EFFECT_ONCE);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceObjectEventAndWait(MIA, FACING_NORTH, 80);
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceObjectEventAndWait(MIA, 0, 40);
  Kolima_ShowMessageAndWait(LOCALID_TRET, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceObjectEventAndWait(MIA, FACING_NORTH, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_SHAKE_HEAD);
  Engine_ObjectEventSetAnimation(MIA, ANIM_SHAKE_HEAD);
  ObjectMotion_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(60);
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 10);
  Kolima_NodTogether();
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceObjectEventAndWait(MIA, 0, 20);
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(GARET, 2);
  ObjectMotion_SetVariantCallback(MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceObjectEventAndWait(MIA, 0, 20);
  Kolima_ShowMessageAndWait(LOCALID_TRET, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                         ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceObjectEventAndWait(MIA, FACING_NORTH, 10);
  Kolima_ShowMessageAndWait(LOCALID_LAUREL, 10);
  Kolima_NodTogether();
  FieldScript_WaitFrames(60);
  // Laurel's and Tret's last words; then the party decides whether to help.
  Script_ShowMessage(LOCALID_LAUREL, 0);
  Script_ShowMessage(LOCALID_TRET, 0);
  Task_Remove(Kolima_UpdateCurseDust);
  FieldScript_WaitFrames(80);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
  PaletteFade_StartInterpolation(60);
  WaitFrames(80);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_LEADER_PSYNERGY_FIELD);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_IVAN_PSYNERGY_FIELD);
  mapState = &gKolima_MiaInParty;
  ObjectMotion_EnableActionAndResetMotion(LOCALID_MIA_PSYNERGY_FIELD);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET_PSYNERGY_FIELD);
  WaitFrames(1);
  shrinkFieldActions = gKolima_ProtectiveFieldShrinkActions;
  ObjectEvent_SetActionScript(LOCALID_LEADER_PSYNERGY_FIELD,
                              shrinkFieldActions);
  ObjectEvent_SetActionScript(LOCALID_IVAN_PSYNERGY_FIELD, shrinkFieldActions);
  if (*mapState != 0) {
    ObjectEvent_SetActionScript(LOCALID_MIA_PSYNERGY_FIELD, shrinkFieldActions);
  }
  ObjectEvent_RunActionScriptAndWait(LOCALID_GARET_PSYNERGY_FIELD,
                                     shrinkFieldActions);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetActionVariant(GARET, 2);
  ObjectMotion_SetActionVariant(IVAN, 2);
  stagingValue = 1;
  ObjectMotion_SetActionVariant(MIA, 2);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags |= stagingValue;
  ObjectEvent_GetById(GARET)->priorityFlags |= stagingValue;
  ObjectEvent_GetById(IVAN)->priorityFlags |= stagingValue;
  ObjectEvent_GetById(MIA)->priorityFlags |= stagingValue;
  ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
  Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTHEAST, 10);
  Script_OpenMessage(IVAN, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_EAST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetVarCbAndRefresh(GARET, 2);
    FieldScript_WaitFrames(10);
    Script_OpenMessage(GARET, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      Kolima_FaceObjectEventAndWait(MIA, 0, 20);
      ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      FieldScript_WaitFrames(40);
      Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTH, 20);
      Kolima_ShowMessageAndWait(GARET, 10);
      Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTH, 20);
      Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTHEAST, 20);
      ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
      Kolima_ShowMessageAndWait(IVAN, 20);
      Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTHEAST, 20);
    } else {
      Kolima_FaceObjectEventAndWait(MIA, 0, 20);
      ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
      Kolima_FaceObjectEventAndWait(GARET, FACING_SOUTH, 20);
      FieldScript_SetMessageId(MsgKolimaHesAsStumpedAsWe);
      Kolima_ShowMessageAndWait(GARET, 20);
      ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
      Kolima_ShowMessageAndWait(IVAN, 20);
    }
    Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
    FieldScript_WaitFrames(10);
    FieldScript_SetMessageId(MsgKolimaYoureIsaacThereIsntMuch);
    Kolima_ShowMessageAndWait(GARET, 10);
    ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
    Kolima_FaceObjectEventAndWait(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 20);
    Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Script_OpenMessage(IVAN, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_WaitFrames(20);
      ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
      FieldScript_WaitFrames(40);
      Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTHEAST, 10);
      Kolima_ShowMessageAndWait(IVAN, 10);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceObjectEventAndWait(MIA, 0, 10);
        ObjectMotion_SetVariantCallback(MIA, 3);
        Kolima_ShowMessageAndWait(MIA, 20);
      } else {
        gFieldWork->message++;
      }
      ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER,
                                    ATTACHED_EFFECT_IN_FRONT |
                                        ATTACHED_EFFECT_ONCE);
      ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                               ATTACHED_EFFECT_ONCE);
      FieldScript_WaitFrames(40);
      ObjectMotion_SetVarCbAndRefresh(GARET, 2);
      Kolima_ShowMessageAndWait(GARET, 20);
      ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      FieldScript_WaitFrames(120);
      Kolima_ShowMessageAndWait(IVAN, 40);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceObjectEventAndWait(MIA, FACING_SOUTHEAST, 10);
        ObjectMotion_SetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
        Kolima_ShowMessageAndWait(MIA, 10);
      } else {
        gFieldWork->message++;
      }
      FieldScript_WaitFrames(60);
      ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTHWEST, 40);
        Kolima_FaceObjectEventAndWait(IVAN, FACING_NORTHEAST, 20);
      }
      Kolima_ShowMessageAndWait(IVAN, 10);
      ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
      ObjectMotion_SetVarCbAndRefresh(GARET, 2);
      FieldScript_WaitFrames(40);
      Kolima_ShowMessageAndWait(IVAN, 20);
      ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
      ObjectMotion_SetAnimationAndWait(GARET, ANIM_NOD);
      FieldScript_WaitFrames(20);
      Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
      ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
    } else {
      ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      FieldScript_WaitFrames(40);
      ObjectMotion_SetAnimationAndWait(IVAN, ANIM_NOD);
      FieldScript_SetMessageId(MsgKolimaThatsReliefIsaacThoughtYoud);
      Kolima_ShowMessageAndWait(IVAN, 20);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceObjectEventAndWait(MIA, 0, 10);
        ObjectMotion_SetVariantCallback(MIA, 1);
        Kolima_ShowMessageAndWait(MIA, 20);
      } else {
        gFieldWork->message++;
      }
      ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                               ATTACHED_EFFECT_ONCE);
      ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER,
                                    ATTACHED_EFFECT_IN_FRONT |
                                        ATTACHED_EFFECT_ONCE);
      FieldScript_WaitFrames(40);
      ObjectMotion_SetVarCbAndRefresh(GARET, 2);
      Kolima_ShowMessageAndWait(GARET, 20);
      ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      FieldScript_WaitFrames(80);
      Kolima_ShowMessageAndWait(IVAN, 40);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceObjectEventAndWait(MIA, FACING_SOUTHEAST, 20);
        Engine_ObjectEventSetAnimation(MIA, ANIM_SHAKE_HEAD);
        Kolima_ShowMessageAndWait(MIA, 40);
      } else {
        gFieldWork->message++;
      }
      ObjectMotion_SetVarCbAndRefresh(IVAN, 2);
      FieldScript_WaitFrames(20);
      Kolima_ShowMessageAndWait(IVAN, 20);
      ObjectMotion_SetVariantCallback(GARET, 2);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
      FieldScript_WaitFrames(40);
      Kolima_ShowMessageAndWait(IVAN, 20);
    }
  }
  // Reunite the party and remember that the first visit has played.
  Script_PlaySong(SONG_FADE_OUT_BGM);
  ObjectEvent_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ObjectEventSetAnimation(GARET, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetDestination(GARET, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetDestination(IVAN, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(IVAN);
  ObjectEvent_SetPosition(IVAN, 0, 0);
  ObjectEvent_SetPosition(LOCALID_TRET, 0, 0);
  ObjectEvent_SetPosition(LOCALID_LAUREL, 0, 0);
  if (gKolima_MiaInParty != 0) {
    Engine_ObjectEventSetAnimation(MIA, ANIM_WALK);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != NULL) {
      ObjectEvent_SetDestination(MIA, objectEvent->x.part.pixel,
                                 objectEvent->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(MIA);
    ObjectEvent_SetPosition(MIA, 0, 0);
  }
  GameFlag_Set(FLAG_KOLIMA_CURSE_WITNESSED);
  Map_PlayMusic();
  FieldScript_End();
}

/// Shows the selected dialogue through an object event, then holds for its
/// reaction.
static void Kolima_ShowMessageAndWait(s32 localId, s32 waitFrames) {
  FieldScript_ShowMessageAndWait(localId, 0);
  FieldScript_WaitFrames(waitFrames);
}

/// Turns an object event immediately, then leaves time for the next scene beat.
static void Kolima_FaceObjectEventAndWait(s32 localId, s32 facing,
                                          s32 waitFrames) {
  ObjectMotion_ArmCallback(localId, facing, 0);
  FieldScript_WaitFrames(waitFrames);
}

#include "common/kolima_grow.h"

/// Spawns a moving curse sparkle every eighth frame.
static void Kolima_SpawnCurseSparkle(void) {
  struct ObjectEvent *sparkle;
  struct Sprite *sprite;
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
    Sound_PlaySong(SE_200);
  }
  sparkle = ObjectEvent_Create(SPRITE_GOLDEN_RINGS, spawnX, 0, spawnZ);
  if (sparkle == 0) {
    return;
  }
  sprite = sparkle->sprite;
  sprite->flags = framePhase;
  priorityFlags = 0xff & ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  priorityFlags &= sparkle->priorityFlags;
  sparkle->priorityFlags = priorityFlags;
  sprite->image.priority = 1;
  sparkle->scaleX = Q16_ONE / 10;
  sparkle->speed = PIXELS(8);
  sparkle->acceleration = PIXELS(8);
  sparkle->motionFlags = framePhase;
  ObjectDispatch_SelectAnimation(sparkle, 2);
  Object_SetPosition(sparkle, targetX, 0, targetZ);
  ObjectEvent_SetScript(sparkle, gKolima_GoldenRingsScript);
}

#include "common/kolima_blink.h"

/// Triggers a temporary pose when an effect enters the party's occupied area.
static s32 Kolima_TriggerRegionScript(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  if (gKolima_MiaInParty != 0) {
    if ((objectEvent->x.fixed > PIXELS(59) &&
         objectEvent->x.fixed < PIXELS(141) &&
         objectEvent->z.fixed > PIXELS(211) &&
         objectEvent->z.fixed < PIXELS(257)) ||
        (objectEvent->x.fixed > PIXELS(69) &&
         objectEvent->x.fixed < PIXELS(122) &&
         objectEvent->z.fixed > PIXELS(194) &&
         objectEvent->z.fixed < PIXELS(277))) {
      Sound_PlaySong(SE_106);
      ObjectEvent_SetScript(objectEvent, gKolima_RegionScript);
      gKolima_PendingPose = 1;
    }
  } else {
    if ((objectEvent->x.fixed > PIXELS(59) &&
         objectEvent->x.fixed < PIXELS(111) &&
         objectEvent->z.fixed > PIXELS(194) &&
         objectEvent->z.fixed < PIXELS(230)) ||
        (objectEvent->x.fixed > PIXELS(111) &&
         objectEvent->x.fixed < PIXELS(141) &&
         objectEvent->z.fixed > PIXELS(216) &&
         objectEvent->z.fixed < PIXELS(250)) ||
        (objectEvent->x.fixed > PIXELS(78) &&
         objectEvent->x.fixed < PIXELS(122) &&
         objectEvent->z.fixed > PIXELS(241) &&
         objectEvent->z.fixed < PIXELS(277))) {
      Sound_PlaySong(SE_106);
      ObjectEvent_SetScript(objectEvent, gKolima_RegionScript);
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
/// (LOCALID_BRIDGE_VILLAGER) is put out of sight, leaving him a tree
/// (LOCALID_BRIDGE_VILLAGER_AS_TREE), and the row of cells at (9, 18) takes the
/// metatile behaviors of the row above it; afterwards the tree is put out of
/// sight instead. Once the bridge is down (FLAG_KOLIMA_BRIDGE_LOWERED), it is
/// drawn down and its cells changed to match. XXX: the row likely closes the
/// bridge while the curse lasts, because the bridge's one coord event (10)
/// needs the man who is a tree until then.
static void Kolima_RestoreBridgeState(void) {
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    Engine_ObjectEventSetPosition(LOCALID_BRIDGE_VILLAGER, 0, 0);
    {
      MapGrid_CopyCellAttributes(9, 17, 5, 1, 9, 18);
    }
    MapGrid_CenterOnFollowedPoint();
    WaitFrames(1);
  } else {
    Engine_ObjectEventSetPosition(LOCALID_BRIDGE_VILLAGER_AS_TREE, 0, 0);
  }
  if (GameFlag_Get(FLAG_KOLIMA_BRIDGE_LOWERED) != 0) {
    {
      MapGrid_CopyCellsTo(91, 19, 72, 9, 5, 7);
    }
    {
      MapGrid_CopyCellAttributes(23, 11, 5, 7, 8, 11);
    }
    MapGrid_CenterOnFollowedPoint();
    WaitFrames(1);
  }
}

/// Where Garet, Ivan and Mia walk on into the silent village and the way each
/// turns (Kolima_WitnessForestCurse).
s32 gKolima_GaretWalksIn[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(97), 0, PIXELS(221)),
    SC_SYNC,         SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_ANIMATION(1), SC_END,
};

s32 gKolima_IvanWalksIn[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(101), 0, PIXELS(268)),
    SC_SYNC,         SC_SET(SW_FACING, FACING_EAST),
    SC_ANIMATION(1), SC_END,
};

s32 gKolima_MiaWalksIn[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(78), 0, PIXELS(247)),
    SC_SYNC,         SC_SET(SW_FACING, FACING_NORTH),
    SC_ANIMATION(1), SC_END,
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
    SC_ANIMATION(1),
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
    SC_CALL(Map_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// A door opening: two frames of a cell column two cells high.
u16 gKolima_DoorCells[] = {
    MAPGRID_ANIMATION_STEP(66, 32, 1, 2, 4),
    MAPGRID_ANIMATION_STEP(67, 32, 1, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

/// Kolima's warp destinations. No warp leads to warp destinations 2, 10 and 12;
/// warp destination 10 puts the party where KOLIMA_FROM_WORLD_MAP does.
struct MapWarpDest gKolimaWarpDests[] = {
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
    {MAP_TABLE_END},
};

struct MapWarpDest gKolimaBridgeWarpDests[] = {
    {KOLIMA_BRIDGE_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 170, 0, 386,
     FACING_NORTH, 0, 0, 0, 288, 416, 0},
    {KOLIMA_BRIDGE_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 168, 0, 50,
     FACING_SOUTH, 0, 48, 0, 288, 416, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gKolimaTunnelWarpDests[] = {
    {KOLIMA_TUNNEL_FROM_KOLIMA_INTERIORS, CONDITION_ALWAYS, 216, 0, 320,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KOLIMA_TUNNEL_FROM_KOLIMA, CONDITION_ALWAYS, 312, 0, 312, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

// clang-format off
struct MapWarpRegion gKolimaTunnelRegions[] = {
    {207, 31, 311, 225, 49, 329, -1, 1},
    {303, 31, 303, 321, 49, 321, -1, 2},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

/// Kolima's warps, then Kolima Bridge's (from the header word for MapId_KolimaBridge)
/// and Kolima Tunnel's (from the one for MapId_KolimaTunnel).
u32 gKolimaWarps[] = {
    MAP_WARPS(Kolima),
    MAP_WARP(KOLIMA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KOLIMA_WEST),
    MAP_WARP(KOLIMA_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_KOLIMA_EAST),
    MAP_WARP(KOLIMA_TO_INN, KolimaInteriors, KOLIMA_INTERIORS_INN_FROM_KOLIMA),
    MAP_WARP(KOLIMA_TO_ITEM_SHOP, KolimaInteriors, KOLIMA_INTERIORS_ITEM_SHOP_FROM_KOLIMA),
    MAP_WARP(KOLIMA_TO_HOUSE_SOUTH, KolimaInteriors, KOLIMA_INTERIORS_HOUSE_FROM_KOLIMA_SOUTH),
    MAP_WARP(KOLIMA_TO_WEAPON_AND_ARMOR_SHOP, KolimaInteriors, KOLIMA_INTERIORS_WEAPON_AND_ARMOR_SHOP_FROM_KOLIMA),
    MAP_WARP(KOLIMA_TO_HOUSE_NORTH, KolimaInteriors, KOLIMA_INTERIORS_HOUSE_FROM_KOLIMA_NORTH),
    MAP_WARP(KOLIMA_TO_KOLIMA_TUNNEL, KolimaTunnel, KOLIMA_TUNNEL_FROM_KOLIMA),
    MAP_WARPS(KolimaBridge),
    MAP_WARP(KOLIMA_BRIDGE_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_KOLIMA_BRIDGE_EAST),
    MAP_WARP(KOLIMA_BRIDGE_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_KOLIMA_BRIDGE_WEST),
    MAP_WARPS(KolimaTunnel),
    MAP_WARP(KOLIMA_TUNNEL_TO_KOLIMA_INTERIORS, KolimaInteriors, KOLIMA_INTERIORS_FROM_KOLIMA_TUNNEL),
    MAP_WARP(KOLIMA_TUNNEL_TO_KOLIMA, Kolima, KOLIMA_FROM_KOLIMA_TUNNEL),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gKolimaTunnelObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

/// Kolima's people: Garet, Ivan and Mia for the curse, the nine villagers,
/// Tret's and Laurel's voices and the four domes until the curse has been seen
/// (FLAG_KOLIMA_CURSE_WITNESSED), four sparkles while it lasts, and Granite until
/// it joins (its Djinni flag, 0x31).
struct ObjectEventTemplate gKolimaObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(192), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(312), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(168), 0, PIXELS(360), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(344), 0, PIXELS(280), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(392), FACING_SOUTH, 3, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(248), 0, PIXELS(184), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(264), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(264), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_TRET, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_LAUREL, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
        PIXELS(24), 0, PIXELS(232), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
        PIXELS(40), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { VENUS_DJINNI, 0x31, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(360), FACING_EAST, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

/// Five more sparkles, placed by Kolima_Initialize once the curse has been seen
/// and kept while it lasts.
struct ObjectEventTemplate gKolima_EntryScript[] = {
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(120), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(296), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(440), 0, PIXELS(344), FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(296), 0, PIXELS(232), FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(152), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

/// Kolima Bridge's villager, as a man (LOCALID_BRIDGE_VILLAGER) and as a tree
/// (LOCALID_BRIDGE_VILLAGER_AS_TREE), in the same place;
/// Kolima_RestoreBridgeState puts one of them out of sight.
struct ObjectEventTemplate gKolimaBridgeObjectEventTemplates[] = {
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(234), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, 3, 0 },
    { SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(234), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// The map events Kolima and Kolima Tunnel share: the warps and doorways, the
/// villagers' words once the curse is lifted and their thoughts before and
/// after, the healer, Granite and two jars. Warps 1 and 2, which cell 99 takes
/// too, keep their numbers, since each map's warp table gives them its own
/// doors: KOLIMA_TO_WORLD_MAP and KOLIMA_TO_WORLD_MAP_EAST in Kolima,
/// KOLIMA_TUNNEL_TO_KOLIMA_INTERIORS and KOLIMA_TUNNEL_TO_KOLIMA in Kolima
/// Tunnel.
struct MapEvent gKolimaEvents[] = {
    { MAP_EVENT_WARP, 99, CONDITION_ALWAYS, 1 },
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, 1 },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, 2 },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_EnterDoor) },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, KOLIMA_TO_KOLIMA_TUNNEL },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaThisIsKolimaVillageOfLumberjacks) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_RELYING_ON_FORESTS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaWeRelyOnTheForestsSo) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_TIME_TO_THINK, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaYouKnowBeingATreeGives) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_SPARKLES_FELL, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaITurnedIntoATreeRight) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_MCCOYS_MONEY, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaDontBlameTheVillagersForBeing) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_HOLY_TREE_PALACE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaWhoWouldTryToUseThe) },
    { MAP_EVENT_TALK, LOCALID_BOY_DID_NOT_MIND_BEING_A_TREE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIDidntMindBeingATree) },
    { MAP_EVENT_TALK, LOCALID_GIRL_SLEEPY_TREES, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaYawnTreesAreSoSleepy) },
    { MAP_EVENT_TALK, LOCALID_HEALER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT_SCRIPT(Kolima_TalkToHealer) },
    { MAP_EVENT_TALK, LOCALID_GRANITE, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_GraniteJoins) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaForAWhileIThoughtId) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_RELYING_ON_FORESTS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaTheForestTreesAreAliveJust) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_TIME_TO_THINK, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIWonderIfIdLiveLonger) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_SPARKLES_FELL, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfThoseSparklesStartFallingAgain) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_MCCOYS_MONEY, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIveBeenPoorAllMyLife) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_HOLY_TREE_PALACE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaItsNotCalledTheHolyTree) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_DID_NOT_MIND_BEING_A_TREE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfWeWereAllTreesThered) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_SLEEPY_TREES, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaNobodyYellsAtYouWhenYoure) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfTheHolyTreeAloneCaused) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS, CONDITION_ALWAYS, MSGNO(MsgKolimaIfICouldSpeakIdTell) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_RELYING_ON_FORESTS, CONDITION_ALWAYS, MSGNO(MsgKolimaHowCouldAnyoneTurnAGirl) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_TIME_TO_THINK, CONDITION_ALWAYS, MSGNO(MsgKolimaThisIsADreamItHas) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_SPARKLES_FELL, CONDITION_ALWAYS, MSGNO(MsgKolimaBeingUnableToMoveIsSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_MCCOYS_MONEY, CONDITION_ALWAYS, MSGNO(MsgKolimaIAmOldIveLivedA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_HOLY_TREE_PALACE, CONDITION_ALWAYS, MSGNO(MsgKolimaItsTheSacredTreesCurseWhy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_DID_NOT_MIND_BEING_A_TREE, CONDITION_ALWAYS, MSGNO(MsgKolimaTheWaterAtMyFeetHuh) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_SLEEPY_TREES, CONDITION_ALWAYS, MSGNO(MsgKolimaIfICantMoveThenMom) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgKolimaMaybeIfIMeditateHarderIll) },
    { BG_EVENT(BG_EVENT_JAR), 100, 0xf57, FIND_ITEM(ITEM_APPLE) },
    { BG_EVENT(BG_EVENT_JAR), 101, 0xf58, FIND_ITEM(ITEM_HERB) },
    { MAP_EVENTS_END },
};

/// Kolima Bridge's map events: its two ends, the bridge's fall and the villager.
struct MapEvent gKolimaBridgeEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, KOLIMA_BRIDGE_TO_WORLD_MAP_SOUTH },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, KOLIMA_BRIDGE_TO_WORLD_MAP_NORTH },
    { MAP_EVENT_COORD, 10, FLAG_KOLIMA_BRIDGE_LOWERED, EVENT_SCRIPT(Kolima_LowerBridge) },
    { MAP_EVENT_TALK, LOCALID_BRIDGE_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgKolimaForAWhileIThoughtNo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BRIDGE_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgKolimaYouWouldntUnderstandButItsTruly) },
    { MAP_EVENTS_END },
};
// clang-format on

s32 gKolima_GoldenRingsScript[] = {
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
