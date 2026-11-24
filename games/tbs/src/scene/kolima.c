/// @file
/// Kolima's overlay, which holds three scenes. The village's houses and shops
/// are another overlay (log_rooms_c.c), and so is each scene of Kolima Forest.
///
/// - Kolima (SceneId_Kolima), the village of lumberjacks, to which Lord McCoy
///   has barricaded the road from Bilibin since the trouble began.
///
///   The party's first entry from the World Map (KOLIMA_FROM_WORLD_MAP) plays
///   Kolima_WitnessForestCurse, unless the party has already seen it in Kolima
///   Forest (FLAG_KOLIMA_CURSE_WITNESSED). Every villager has been turned into
///   a tree. Sparkles fall from the sky and press the party down until domes of
///   their own Psynergy shield them. Two unseen voices speak, Tret and Laurel,
///   the spirits of the holy trees, who send the party to look for Tret deep
///   within Kolima Forest.
///
///   Until the curse is lifted in Kolima Forest (FLAG_KOLIMA_CURSE_LIFTED),
///   Scene_GetActorPlacements gives the nine villagers (ACTOR_FIRST_VILLAGER
///   to ACTOR_LAST_VILLAGER) tree sprites that stand still. Sparkles twinkle on
///   the ground (ACTOR_FIRST_SPARKLE and the three after it, and five more that
///   Scene_Initialize places). The villagers answer only Mind Read, with the
///   thoughts of people stuck as trees.
///
///   Afterwards they are people again and talk of the curse, the holy tree and
///   McCoy's money. The healer among them (ACTOR_HEALER) offers the Sanctum's
///   services across his counter.
///
///   Granite, a Venus Djinni (ACTOR_GRANITE), waits in the village and joins
///   when spoken to.
///
///   Five doorways (KOLIMA_TO_INN to KOLIMA_TO_HOUSE_NORTH) lead into the
///   houses and shops (SceneId_LogRoomsC). KOLIMA_TO_WORLD_MAP and
///   KOLIMA_TO_WORLD_MAP_EAST lead to the World Map and KOLIMA_TO_KOLIMA_TUNNEL
///   into Kolima Tunnel.
///
/// - Kolima Tunnel (SceneId_KolimaTunnel), a passage with no one in it from one
///   of the houses (LOG_ROOMS_C_FROM_KOLIMA_TUNNEL) to the village
///   (KOLIMA_FROM_KOLIMA_TUNNEL).
///
///   It shares the village's trigger table, whose exits KOLIMA_TO_WORLD_MAP and
///   KOLIMA_TO_WORLD_MAP_EAST are, by the same numbers, its two ways out
///   (KOLIMA_TUNNEL_TO_LOG_ROOMS_C and KOLIMA_TUNNEL_TO_KOLIMA).
///
///   That table's two jars, an Apple and an Herb (examine triggers 100 and
///   101), are in the village. Only Kolima's layout (kolima's
///   map.inc) places them, at cells (23, 8) and (13, 23).
///
/// - Kolima Bridge (SceneId_KolimaBridge), "the grand bridge across the
///   river" and "the pride of Kolima" to the village's armor seller, with the
///   World Map at both ends. It is a drawbridge, standing raised between its
///   posts.
///
///   A villager stands beside it, a tree while the curse lasts
///   (ACTOR_BRIDGE_VILLAGER_AS_TREE) and a man afterwards
///   (ACTOR_BRIDGE_VILLAGER).
///
///   The first time the party then steps up to the bridge (step trigger 10),
///   Kolima_LowerBridge plays. He asks whether they are going to cross, hops
///   aside and lets the drawbridge down across the river in front of the leader
///   (FLAG_KOLIMA_BRIDGE_LOWERED; its recording,
///   places/kolima-bridge/events/Kolima_LowerBridge.gif).
///
///   From then on he says that for a while he thought no one would ever cross
///   it again. Kolima_RestoreBridgeState draws it down on every later visit,
///   and the bridge on the World Map is drawn down with the same flag.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/character_ids.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/kolima.h"
#include "scene/kolima_curse_dust.h"
#include "scene/party_nod.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap_slots.h"
#include "system/io_reg.h"
#include "system/main_routine.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern s32 gKolima_GaretWalksIn[];
extern s32 gKolima_IvanWalksIn[];
extern s32 gKolima_MiaWalksIn[];
extern s32 gKolima_PartySquashScript[];
extern s32 gKolima_PartyRestoreScaleScript[];
extern s32 gKolima_PsynergyFieldGrowActions[];
extern s32 gKolima_PsynergyFieldShrinkActions[];
extern s32 gKolima_GroundSparkleActions[];
extern s32 gKolima_DebrisScript[];
extern s32 gKolima_FaceLeaderScript[];
extern u16 gKolima_DoorCells[];
extern struct SceneEntrance gKolimaEntrances[];
extern struct SceneEntrance gKolimaBridgeEntrances[];
extern struct SceneEntrance gKolimaTunnelEntrances[];
extern struct SceneExitRegion gKolimaTunnelRegions[];
extern u32 gKolimaExits[];
extern struct ActorPlacement gKolimaTunnelActorPlacements[];
extern struct ActorPlacement gKolimaActorPlacements[];
extern struct ActorPlacement gKolima_MoreGroundSparklePlacements[];
extern struct ActorPlacement gKolimaBridgeActorPlacements[];
extern struct Trigger gKolimaTriggers[];
extern struct Trigger gKolimaBridgeTriggers[];
extern s32 gKolima_GoldenRingsScript[];
extern s32 gKolima_SparkleBurstScript[];
extern s32 gKolima_CurseDustScript[];

/// The pieces the drawbridge throws out where it lands (SPRITE_DUST_PUFF).
enum { BRIDGE_DUST_PUFFS = 10 };

static void Kolima_WitnessForestCurse(void);
static void Kolima_ShowMessageAndWait(s32 actorId, s32 waitFrames);
static void Kolima_FaceActorAndWait(s32 actorId, s32 facing, s32 waitFrames);
static void Kolima_SpawnCurseSparkle(void);
static void Kolima_RestoreBridgeState(void);

/// Actors of Kolima, numbering the rows of gKolimaActorPlacements from
/// ACTOR_FIRST_PLACED. Garet, Ivan and Mia, who stand by for the curse, take
/// their own actor slots. The nine villagers are named by what they say once
/// the curse is lifted.
enum KolimaActorId {
  /// A young man with brown hair.
  ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS = ACTOR_FIRST_PLACED,
  /// A woman with fair hair, whose mind Ivan reads in
  /// Kolima_WitnessForestCurse.
  ACTOR_VILLAGER_RELYING_ON_FORESTS,
  /// A burly grey-haired man.
  ACTOR_VILLAGER_TIME_TO_THINK,
  /// A woman in a green dress.
  ACTOR_VILLAGER_SPARKLES_FELL,
  /// An old man with a cane.
  ACTOR_OLD_MAN_MCCOYS_MONEY,
  /// An old woman.
  ACTOR_OLD_WOMAN_HOLY_TREE_PALACE,
  /// A small boy with fair hair.
  ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE,
  /// A little girl with pigtails.
  ACTOR_GIRL_SLEEPY_TREES,
  /// A man with a white beard in a tall hat and a long robe. He opens the
  /// Sanctum's services across his counter (Sanctum_Run).
  ACTOR_HEALER,
  /// Tret's wrathful side, unseen while the curse falls.
  ACTOR_TRET,
  /// Unseen while the curse falls.
  ACTOR_LAUREL,
  /// The fields of Psynergy that rise over the party while the curse falls
  /// ("You know what those fields were?"), each set where its character stands
  /// in Kolima_WitnessForestCurse. Mia's is set only with her in the party.
  ACTOR_LEADER_PSYNERGY_FIELD,
  ACTOR_IVAN_PSYNERGY_FIELD,
  ACTOR_MIA_PSYNERGY_FIELD,
  ACTOR_GARET_PSYNERGY_FIELD,
  /// The four sparkles on the ground while the curse lasts
  /// (gKolima_GroundSparkleActions).
  ACTOR_FIRST_SPARKLE,
  /// Granite, a Venus Djinni, placed until it has joined.
  ACTOR_GRANITE = ACTOR_FIRST_SPARKLE + 4,
  ACTOR_FIRST_VILLAGER = ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS,
  ACTOR_LAST_VILLAGER = ACTOR_HEALER
};

/// Kolima Bridge's villager, numbering the rows of
/// gKolimaBridgeActorPlacements from ACTOR_FIRST_PLACED: the same
/// villager as a man and as a tree, in the same place.
enum KolimaBridgeActorId {
  /// A burly grey-haired man.
  ACTOR_BRIDGE_VILLAGER = ACTOR_FIRST_PLACED,
  /// The same villager as a tree, while the curse lasts.
  ACTOR_BRIDGE_VILLAGER_AS_TREE
};

/// The end of a Psynergy field's growing: if a speck of the curse has burst on
/// the fields meanwhile, this one takes its next pose.
static s32 Kolima_FlashHitPsynergyFields(struct Actor *actor) {
  if (gKolima_PsynergyFieldHit != 0) {
    Actor_SelectAnimation(actor, 2);
    gKolima_PsynergyFieldHit = 0;
  }
  return 1;
}

/// Each frame, a sparkle on Kolima's ground: red-tinted, it flashes white for
/// the last few frames of each twinkle, which lasts about 24 frames at a
/// random pace.
static s32 Kolima_TwinkleGroundSparkle(struct Actor *actor) {
  s32 randomValue = Random_Next();
  s32 scaledRandom = randomValue * 100;
  s32 appearanceTimer = (u16)actor->timer + ((u32)scaledRandom >> 16);
  actor->timer = appearanceTimer;
  if ((s16)appearanceTimer > 1000) {
    Actor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
  }
  if ((s16)actor->timer > 1200) {
    actor->timer = 0;
  }
  return 1;
}

/// While the curse lasts, gives Kolima's nine villagers tree sprites. The
/// villagers are actor placement rows 3 to 11, ACTOR_FIRST_VILLAGER to
/// ACTOR_LAST_VILLAGER.
///
/// The sprite is SPRITE_TREE or, for rows 9 and 10
/// (ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE and ACTOR_GIRL_SLEEPY_TREES),
/// SPRITE_SMALL_TREE. They stand still and keep their facing when spoken to.
static void
Kolima_InitializeCursedVillagers(struct ActorPlacement *placements) {
  struct ActorPlacement *villager = &placements[3];
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

/// Each frame of a piece thrown out where the drawbridge lands: it flies,
/// swells and is gone when its timer runs out.
static s32 Kolima_UpdateFallingDebris(struct Actor *debris) {
  debris->x.fixed += debris->velocityX;
  debris->z.fixed += debris->velocityZ;
  debris->velocityZ -= toFIXED(1) / 25;
  debris->scaleX += 3 * FIXED_ONE / 128;
  debris->scaleY += 3 * FIXED_ONE / 128;
  {
    s32 framesLeft = (u16)debris->timer - 1;
    debris->timer = framesLeft;
    if ((u16)framesLeft == 0) {
      Actor_Release(debris);
    }
  }
  return 1;
}

#include "shared/actor_facing.c"

/// Selects the entrances of Kolima, Kolima Tunnel or Kolima Bridge.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_KOLIMA_BRIDGE) {
    return gKolimaBridgeEntrances;
  }
  if (sceneId == SCENE_KOLIMA_TUNNEL) {
    return gKolimaTunnelEntrances;
  }
  return gKolimaEntrances;
}

/// Only Kolima Tunnel has regions: its two ways out,
/// KOLIMA_TUNNEL_TO_LOG_ROOMS_C and KOLIMA_TUNNEL_TO_KOLIMA.
const struct SceneExitRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineState.next == SCENE_KOLIMA_TUNNEL) {
    return gKolimaTunnelRegions;
  }
  return 0;
}

/// The ways out of all three scenes.
const u32 *Scene_GetExits(void) { return gKolimaExits; }

/// Selects the people of Kolima or Kolima Bridge. Kolima Tunnel has none. In
/// Kolima the villagers are trees until the curse is lifted.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_KOLIMA) {
    if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Kolima_InitializeCursedVillagers(gKolimaActorPlacements);
    }
    return gKolimaActorPlacements;
  }
  if (sceneId == SCENE_KOLIMA_BRIDGE) {
    return gKolimaBridgeActorPlacements;
  }
  return gKolimaTunnelActorPlacements;
}

/// In Kolima, once the curse is lifted, talking to the healer (ACTOR_HEALER)
/// across his counter (facing north) opens the Sanctum's services.
static void Kolima_TalkToHealer(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 facing = leader->facing;
  Event_Start();
  if (FACING_IS_NORTH(facing)) {
    Sanctum_Run(ACTOR_HEALER);
  } else {
    Event_SetMessageId(MsgKolimaToldHolyTrees);
    // "I am told the holy tree's rage caused this horrible..."
    // Yes: "You may not have noticed, but something changed when those..."
    // No: "I agree. Something did change when the gems fell."
    Event_AskYesNo(ACTOR_HEALER, 0);
  }
  Event_End();
}

/// In Kolima, talking to Granite, a Venus Djinni, makes it join the party.
static void Kolima_GraniteJoins(void) {
  Djinn_JoinParty(ACTOR_GRANITE, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GRANITE));
}

/// Selects Kolima Bridge's triggers, or the table Kolima and Kolima Tunnel
/// share.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == SCENE_KOLIMA_BRIDGE) {
    return gKolimaBridgeTriggers;
  }
  return gKolimaTriggers;
}

/// Walks the leader north through a door of Kolima into the exit of the same
/// number.
static void Kolima_WalkThroughExit(s32 exitNumber) {
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  gFieldState->transitionFrames = MAP_TRANSITION_FRAMES;
  Import_EventSetExitRequest(exitNumber);
}

/// In Kolima, walking into one of the five doorways leads into the houses and
/// shops.
static void Kolima_EnterDoor(void) {
  struct FieldState *state = gFieldState;
  s32 x = 0;
  s32 y = 0;

  Event_Start();
  Sound_Play(SE_DOOR_OPEN);
  switch (state->stepTrigger) {
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
    Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 8);
    gFieldState->transitionFrames = MAP_TRANSITION_FRAMES;
    Import_EventSetExitRequest(KOLIMA_TO_HOUSE_NORTH);
    Event_End();
    return;
  }
  MapGrid_PlayMapEdit(gKolima_DoorCells, x, y);
  Kolima_WalkThroughExit(state->stepTrigger);
  Event_End();
}

/// At Kolima Bridge, the first time the party steps up to it, the villager
/// beside it (ACTOR_BRIDGE_VILLAGER) asks whether they are going to cross.
static void Kolima_LowerBridge(void) {
  u32 debrisIndex;

  Event_Start();
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(168, 0, 246, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_BRIDGE_VILLAGER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  40);
  Actor_RunRepeatedMotion(ACTOR_BRIDGE_VILLAGER, 2);
  Event_SetMessageId(MsgKolimaGoingCrossBridge);
  // "Are you going to cross the bridge?"
  Event_ShowMessage(ACTOR_BRIDGE_VILLAGER, 0);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(168, 0, 234, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 174, 278);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_BRIDGE_VILLAGER, ANIM_NOD);
  // "Oh... Right! Go right ahead!"
  Event_ShowMessage(ACTOR_BRIDGE_VILLAGER, 0);
  Actor_FaceDirection(ACTOR_BRIDGE_VILLAGER, FACING_WEST_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_BRIDGE_VILLAGER, 1);
  Event_Wait(20);
  Actor_Get(ACTOR_BRIDGE_VILLAGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_BRIDGE_VILLAGER, 2);
  Actor_Jump(ACTOR_BRIDGE_VILLAGER, 2, 0);
  Actor_MoveToAndWait(ACTOR_BRIDGE_VILLAGER, 224, 197);
  Sound_Play(SE_KOLIMA_BRIDGE_HOP);
  Event_Wait(10);
  Actor_MoveToAndWait(ACTOR_BRIDGE_VILLAGER, 234, 200);
  Event_Wait(10);
  Sound_Play(SE_KOLIMA_BRIDGE_POST);
  Event_Wait(30);
  // The drawbridge comes down in ever shorter steps.
  MapGrid_CopyCellsTo(91, 0, 72, 9, 5, 4);
  Event_Wait(12);
  MapGrid_CopyCellsTo(91, 4, 72, 9, 5, 4);
  Event_Wait(9);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  MapGrid_CopyCellsTo(91, 8, 72, 9, 5, 5);
  Event_Wait(6);
  MapGrid_CopyCellsTo(91, 13, 72, 9, 5, 6);
  Event_Wait(3);
  Sound_Play(SE_THUMP);
  // Puffs of dust where it lands, 4 pixels apart, thrown alternately left and
  // right.
  for (debrisIndex = 0; debrisIndex < BRIDGE_DUST_PUFFS; debrisIndex++) {
    struct Actor *debris = FieldActor_Create(
        SPRITE_DUST_PUFF, toFIXED(148 + debrisIndex * 4), 0, toFIXED(258));

    if (debris != NULL) {
      debris->motionFlags = 0;
      Actor_SetSpriteFlags(debris, 0);
      debris->sprite->image.priority = 0;
      debris->timer = (Random_Below(40)) + 40;
      debris->velocityX = (toFIXED(debrisIndex & 3) + toFIXED(1)) >> 1;
      debris->velocityZ = toFIXED(1);
      if (debrisIndex & 1) {
        debris->velocityX = -((toFIXED(debrisIndex & 3) + toFIXED(1)) >> 1);
      }
      Actor_SelectAnimation(debris, 1);
      Actor_StartScript(debris, gKolima_DebrisScript);
    }
  }
  MapGrid_CopyCellsTo(91, 19, 72, 9, 5, 7);
  MapGrid_CopyCellAttributes(23, 11, 5, 7, 8, 11);
  MapGrid_SetShake(0, 4, 1);
  Event_Wait(10);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Event_Wait(20);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  // From now on the villager turns to keep facing the leader.
  FieldActor_WatchAndStartScript(ACTOR_BRIDGE_VILLAGER,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                 gKolima_FaceLeaderScript);
  Event_Wait(60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  GameFlag_Set(FLAG_KOLIMA_BRIDGE_LOWERED);
  Event_End();
}

/// Sets up whichever scene is entered: Kolima Bridge as the curse and the
/// bridge leave it (Kolima_RestoreBridgeState), Kolima Tunnel with a window
/// transition, and Kolima itself.
s32 Scene_Initialize(void) {
  s16 sceneId;
  u32 actorId;
  const s32 *table;

  sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_KOLIMA_BRIDGE) {
    Kolima_RestoreBridgeState();
    return 0;
  }

  if (sceneId == SCENE_KOLIMA_TUNNEL) {
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    return 0;
  }

  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_SPARKLE), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_SPARKLE + 1), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_SPARKLE + 2), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_SPARKLE + 3), 0);

  table = gKolima_GroundSparkleActions;
  Actor_SetScript(ACTOR_FIRST_SPARKLE, table);
  Actor_SetScript(ACTOR_FIRST_SPARKLE + 1, table);
  Actor_SetScript(ACTOR_FIRST_SPARKLE + 2, table);
  Actor_SetScript(ACTOR_FIRST_SPARKLE + 3, table);

  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    for (actorId = ACTOR_FIRST_VILLAGER; actorId <= ACTOR_LAST_VILLAGER;
         actorId++) {
      Actor_SetSpriteFlags(Actor_Get(actorId), 0);
    }
    // The healer's counter west, east and south of his tree searches as a
    // wooden box, as its corners always do.
    MapGrid_CopyCellAttributes(13, 9, 1, 1, 13, 8);
    MapGrid_CopyCellAttributes(13, 9, 1, 1, 15, 8);
    MapGrid_CopyCellAttributes(13, 9, 1, 1, 14, 9);
  }

  if (GameFlag_Get(FLAG_KOLIMA_CURSE_WITNESSED) == 0) {
    if (gMainRoutineState.entrance == KOLIMA_FROM_WORLD_MAP) {
      Kolima_WitnessForestCurse();
    }
  }

  if (GameFlag_Get(FLAG_KOLIMA_CURSE_WITNESSED) != 0) {
    Actor_ReleaseById(ACTOR_GARET);
    Actor_ReleaseById(ACTOR_IVAN);
    Actor_ReleaseById(ACTOR_MIA);
    Actor_ReleaseById(ACTOR_TRET);
    Actor_ReleaseById(ACTOR_LAUREL);
    Actor_ReleaseById(ACTOR_LEADER_PSYNERGY_FIELD);
    Actor_ReleaseById(ACTOR_IVAN_PSYNERGY_FIELD);
    Actor_ReleaseById(ACTOR_MIA_PSYNERGY_FIELD);
    Actor_ReleaseById(ACTOR_GARET_PSYNERGY_FIELD);
    Event_AppendActorPlacements(gKolima_MoreGroundSparklePlacements);
  }

  return 0;
}

/// Shows one of the domes of Psynergy on its frames, even or odd, and lifts it
/// 500 pixels out of sight on the others.
static inline void Kolima_FlickerPsynergyField(s32 actorId,
                                               s32 visibleOnOddFrames) {
  struct Actor *actor = Actor_Get(actorId);
  if (actor != NULL) {
    actor->motionFlags = 0;
    if (visibleOnOddFrames ? (gFrameCount & 1) : !(gFrameCount & 1)) {
      actor->y.fixed = 0;
    } else {
      actor->y.fixed = toFIXED(500);
    }
  }
}

/// Makes the four domes flicker, two on even frames and two on odd.
static void Kolima_FlickerPsynergyFields(void) {
  Kolima_FlickerPsynergyField(ACTOR_LEADER_PSYNERGY_FIELD, 0);
  Kolima_FlickerPsynergyField(ACTOR_IVAN_PSYNERGY_FIELD, 1);
  Kolima_FlickerPsynergyField(ACTOR_MIA_PSYNERGY_FIELD, 0);
  Kolima_FlickerPsynergyField(ACTOR_GARET_PSYNERGY_FIELD, 1);
}

#include "shared/mind_read.c"

/// In Kolima, on the first entry from the World Map (KOLIMA_FROM_WORLD_MAP),
/// the party sees the curse fall. This plays until FLAG_KOLIMA_CURSE_WITNESSED.
static void Kolima_WitnessForestCurse(void) {
  struct Actor *actor;
  s32 *partyLayout;
  const s32 *squashActions;
  const s32 *restoreScaleActions;
  s32 *miaInParty;
  const s32 *growFieldActions;
  const s32 *shrinkFieldActions;

  // Enter the deserted village and let Ivan inspect one of its trees.
  Event_Start();
  partyLayout = &gKolima_MiaInParty;
  *partyLayout = GameFlag_Get(FLAG_IN_PARTY(MIA));
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LEADER_PSYNERGY_FIELD), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN_PSYNERGY_FIELD), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA_PSYNERGY_FIELD), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET_PSYNERGY_FIELD), 0);
  FieldCamera_MoveTo(104, CAMERA_KEEP, 256, 0);
  MapGrid_CenterOnFollowedPoint();
  Actor_SetPosition(ACTOR_PARTY_LEADER, 23, 247);
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 121, 238);
  Actor_SetSpeed(ACTOR_GARET, 0.6);
  Actor_SetSpeed(ACTOR_IVAN, 0.6);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetScript(ACTOR_GARET, gKolima_GaretWalksIn);
  Actor_SetScript(ACTOR_IVAN, gKolima_IvanWalksIn);
  if (*partyLayout != 0) {
    Actor_SetSpeed(ACTOR_MIA, 0.6);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetPositionFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetScript(ACTOR_MIA, gKolima_MiaWalksIn);
  }
  Actor_WaitForScriptEndById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
  partyLayout = &gKolima_MiaInParty;
  if (*partyLayout != 0) {
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 60);
  if (*partyLayout != 0) {
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  if (*partyLayout != 0) {
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
  }
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Event_SetMessageId(MsgKolimaQuiet);
  // "It's so quiet..."
  Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_IVAN, 72, 286);
  Actor_WalkToAndWait(ACTOR_IVAN, 72, 302);
  Actor_WalkToAndWait(ACTOR_IVAN, 88, 310);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  if (*partyLayout != 0) {
    Actor_FaceActor(ACTOR_MIA, ACTOR_IVAN, 0);
  }
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  if (*partyLayout != 0) {
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  }
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Event_Wait(20);
  // Mind Read reveals that the trees are the missing villagers.
  Scene_StartMindRead(ACTOR_IVAN, ACTOR_VILLAGER_RELYING_ON_FORESTS);
  Event_Wait(40);
  Scene_EndMindRead();
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_Get(ACTOR_IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_IVAN, 80, 310);
  Event_Wait(1);
  Actor_Get(ACTOR_IVAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "What is the matter, Ivan?"
  Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "This tree...was a person."
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
  if (*partyLayout != 0) {
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
  }
  Actor_FaceEachOther(ACTOR_IVAN, ACTOR_GARET, 60);
  if (*partyLayout != 0) {
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  }
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  // "Everyone in Kolima has been turned into a tree."
  Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_SetSpeed(ACTOR_IVAN, 0.5);
  Actor_Get(ACTOR_IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_IVAN, 72, 286);
  Event_Wait(1);
  Actor_Get(ACTOR_IVAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetScript(ACTOR_IVAN, gKolima_IvanWalksIn);
  if (*partyLayout != 0) {
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Event_Wait(60);
    // "It's horrible! All these trees with clothes are really..."
    Kolima_ShowMessageAndWait(ACTOR_MIA, 20);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  } else {
    gFieldState->message++;
  }
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(10);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  // "Why? Do you know why this happened?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  } else {
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    gFieldState->message++;
  }
  // Yes: "I see... It was because they hurt the holy tree?"
  // No: "I see... Who could have known?"
  Kolima_ShowMessageAndWait(ACTOR_GARET, 40);
  Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_SOUTHEAST, 40);
  Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_WEST, 20);
  Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_SOUTH, 40);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
  Kolima_FaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_SetMessageId(MsgKolimaWhatIsIt);
  // "What is it now, Ivan?"
  Kolima_ShowMessageAndWait(ACTOR_GARET, 10);
  Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  // "That sparkly stuff on the ground... What is it?"
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_EAST, 20);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 40);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 30);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 20);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 30);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 20);
  Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Sound_Play(SE_CURSE_FALL);
  PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  wait(1);
  // The falling sparkles threaten to turn the party into trees as well.
  gKolima_SparkleSoundOn = 1;
  Task_AddFrame(Kolima_SpawnCurseSparkle, TASK_PASS_FRAME);
  wait(20);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(16, 16, 20), PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(120);
  wait(60);
  squashActions = gKolima_PartySquashScript;
  Actor_SetScript(ACTOR_PARTY_LEADER, squashActions);
  Actor_SetScript(ACTOR_GARET, squashActions);
  Actor_SetScript(ACTOR_IVAN, squashActions);
  Actor_SetScript(ACTOR_MIA, squashActions);
  Event_Wait(100);
  // "Aaah! What's happening!?"
  Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
  // "My mind!!! Someone is attacking my-"
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 40);
  if (gKolima_MiaInParty != 0) {
    Event_Wait(40);
    Actor_SetAttachedEffect(ACTOR_MIA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    // "I... I can't... I can't even stand up!"
    Kolima_ShowMessageAndWait(ACTOR_MIA, 40);
  } else {
    gFieldState->message++;
  }
  Event_Wait(20);
  if (gKolima_MiaInParty != 0) {
    Actor_Get(ACTOR_MIA)->velocityY = toFIXED(2);
    Event_Wait(10);
    Actor_SetSpeedFX(ACTOR_MIA, toFIXED(2), toFIXED(2));
    Actor_SetDestinationOffset(ACTOR_MIA, -2, 0);
    Actor_SetScript(ACTOR_MIA, gKolima_PartyRestoreScaleScript);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), 0);
    Actor_SetAnimation(ACTOR_MIA, ANIM_COLLAPSE);
    Event_Wait(10);
  }
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = toFIXED(2);
  Event_Wait(10);
  Actor_SetSpeedFX(ACTOR_PARTY_LEADER, toFIXED(2), toFIXED(2));
  restoreScaleActions = gKolima_PartyRestoreScaleScript;
  Actor_SetScript(ACTOR_PARTY_LEADER, restoreScaleActions);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
  Event_Wait(20);
  Actor_Get(ACTOR_GARET)->velocityY = toFIXED(2);
  Event_Wait(10);
  Actor_SetSpeedFX(ACTOR_GARET, toFIXED(2), toFIXED(2));
  Actor_SetScript(ACTOR_GARET, restoreScaleActions);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_COLLAPSE);
  Event_Wait(40);
  Actor_Get(ACTOR_IVAN)->velocityY = toFIXED(2);
  Event_Wait(10);
  Actor_SetScript(ACTOR_IVAN, restoreScaleActions);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), 0);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_COLLAPSE);
  gKolima_SparkleSoundOn = 0;
  Event_Wait(160);
  Task_Remove(Kolima_SpawnCurseSparkle);
  Event_Wait(120);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  wait(60);
  gKolima_CurseDustWidth = 0;
  gKolima_CurseDustOffset = toFIXED(128);
  gKolima_CurseDustState = KOLIMA_DUST_CLOSING;
  Task_AddFrame(Kolima_UpdateCurseDust, TASK_PASS_FRAME);
  Event_Wait(180);
  Sound_Play(MUS_FOREST_THEME);
  // "What is that? Something's falling from the sky!"
  Kolima_ShowMessageAndWait(ACTOR_GARET, 80);
  // "It's sparkling! Could this be what turned those people into..."
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  // "We've got to get out of here!"
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
  gKolima_CurseDustState = KOLIMA_DUST_CLOSING_WIDE;
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 3);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "I can't! I can't move!!"
  Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
  if (gKolima_MiaInParty != 0) {
    Actor_SetAttachedEffect(ACTOR_MIA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    // "Oh, no... If it touches us, we'll turn into trees!"
    Kolima_ShowMessageAndWait(ACTOR_MIA, 10);
  } else {
    gFieldState->message++;
  }
  gKolima_CurseDustState = KOLIMA_DUST_DRIFTING;
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_IVAN)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Actor_SetSpritePriority(ACTOR_GARET, 3);
  Actor_SetSpritePriority(ACTOR_IVAN, 3);
  Actor_SetSpritePriority(ACTOR_MIA, 3);
  gKolima_PsynergyFieldHit = 0;
  Task_AddFrame(Kolima_FlickerPsynergyFields, TASK_PASS_FRAME);
  Sound_Play(SE_SHIMMER);
  Actor_Get(ACTOR_LEADER_PSYNERGY_FIELD)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_LEADER_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_LEADER_PSYNERGY_FIELD, 120, 248);
  growFieldActions = gKolima_PsynergyFieldGrowActions;
  Actor_SetScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldActions);
  Actor_Get(ACTOR_IVAN_PSYNERGY_FIELD)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_IVAN_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_IVAN_PSYNERGY_FIELD, 100, 274);
  Actor_SetScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldActions);
  if (gKolima_MiaInParty != 0) {
    Actor_Get(ACTOR_MIA_PSYNERGY_FIELD)->priorityFlags &=
        ~ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpritePriority(ACTOR_MIA_PSYNERGY_FIELD, 2);
    Actor_SetPosition(ACTOR_MIA_PSYNERGY_FIELD, 74, 254);
    Actor_SetScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldActions);
  }
  Actor_Get(ACTOR_GARET_PSYNERGY_FIELD)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_GARET_PSYNERGY_FIELD, 2);
  Actor_SetPosition(ACTOR_GARET_PSYNERGY_FIELD, 94, 225);
  Actor_SetScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldActions);
  while (gKolima_CurseDustState != KOLIMA_DUST_CENTRED) {
    wait(1);
  }
  Event_Wait(300);
  Task_Remove(Kolima_UpdateCurseDust);
  Event_Wait(120);
  Sound_Play(SOUND_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  wait(60);
  Actor_Stop(ACTOR_LEADER_PSYNERGY_FIELD);
  Actor_Stop(ACTOR_IVAN_PSYNERGY_FIELD);
  if (gKolima_MiaInParty != 0) {
    Actor_Stop(ACTOR_MIA_PSYNERGY_FIELD);
  }
  Actor_Stop(ACTOR_GARET_PSYNERGY_FIELD);
  wait(1);
  shrinkFieldActions = gKolima_PsynergyFieldShrinkActions;
  Actor_SetScript(ACTOR_LEADER_PSYNERGY_FIELD, shrinkFieldActions);
  Actor_SetScript(ACTOR_IVAN_PSYNERGY_FIELD, shrinkFieldActions);
  if (gKolima_MiaInParty != 0) {
    Actor_SetScript(ACTOR_MIA_PSYNERGY_FIELD, shrinkFieldActions);
  }
  Actor_RunScriptAndWait(ACTOR_GARET_PSYNERGY_FIELD, shrinkFieldActions);
  Event_Wait(80);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(40);
  // "Isaac... What just happened to us? That barrier... Did it save..."
  Event_OpenMessage(ACTOR_GARET, 0);
  Actor_SetPosition(ACTOR_TRET, 87, 139);
  Actor_SetPosition(ACTOR_LAUREL, 87, 139);
  wait(1);
  if (Event_AskYesNoWithActorAnimation(ACTOR_TRET, 0) == 1) {
    gFieldState->message++;
  }
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  // Yes: "I think so, too, Isaac. I think I know what those domes..."
  // No: "No, I think Garet is right. I think I know what those..."
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
  if (gKolima_MiaInParty != 0) {
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(10);
    Event_SetMessageId(MsgKolimaKnowThoseFieldsWere);
    // "You know what those fields were?"
    Kolima_ShowMessageAndWait(ACTOR_MIA, 40);
  }
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(80);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  // The companions discover that their Psynergy formed the barriers.
  Event_SetMessageId(MsgKolimaWasOurPsynergy);
  // "It was our Psynergy."
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 3);
  Event_Wait(40);
  Actor_SetSpritePriority(ACTOR_GARET, 2);
  Actor_Get(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GARET), SPRITE_FLAG_SHADOW);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 60);
  // "What are you, crazy?"
  Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  // "I didn't use any Psynergy!"
  Kolima_ShowMessageAndWait(ACTOR_GARET, 10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(40);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 40);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 20);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHWEST, 20);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
  Actor_Jump(ACTOR_GARET, 2, 0);
  Event_Wait(40);
  Actor_Jump(ACTOR_GARET, 2, 0);
  Event_Wait(10);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Event_Wait(20);
  // "Hey... I can stand... I can move again!"
  Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
  if (gKolima_MiaInParty != 0) {
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(80);
    Actor_SetSpritePriority(ACTOR_MIA, 2);
    Actor_Get(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpriteFlags(Actor_Get(ACTOR_MIA), SPRITE_FLAG_SHADOW);
    Actor_Jump(ACTOR_MIA, 4, 0);
    Actor_SetDestinationOffset(ACTOR_MIA, -2, 0);
    Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
    Kolima_FaceActorAndWait(ACTOR_MIA, FACING_NORTHEAST, 60);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    // "You're right. I can stand, too..."
    Kolima_ShowMessageAndWait(ACTOR_MIA, 20);
  } else {
    gFieldState->message++;
  }
  Actor_Jump(ACTOR_GARET, 2, 0);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
  // "Ivan, it's okay! Go ahead! Stand up!"
  Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_SetSpritePriority(ACTOR_IVAN, 2);
  Actor_Get(ACTOR_IVAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IVAN), SPRITE_FLAG_SHADOW);
  Actor_Jump(ACTOR_IVAN, 4, 0);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(10);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  // "Those barriers..."
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(20);
  // "We thought that Psynergy was only used when we concentrate,..."
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    gFieldState->message++;
  } else {
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
    Actor_StartRepeatedMotion(ACTOR_GARET, 2);
    // "You're such a liar, Isaac!"
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  // "When I was training, I'd focus so hard, I thought my head..."
  Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
  Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  // "Well, maybe it's just easier for you, but when I use Psynergy..."
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 10);
  if (gKolima_MiaInParty != 0) {
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Kolima_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
    Kolima_FaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
    Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
    // "But what? The only way you can use Psynergy is by concentrating!"
    Kolima_ShowMessageAndWait(ACTOR_MIA, 10);
  } else {
    gFieldState->message++;
  }
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  // "Anyway, I'm certain that was Psynergy we were projecting."
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  // "That must mean that some Psynergy acts only when it's needed."
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 10);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "You mean, like when we're in danger?"
  Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "I think it's like when we strike critical hits in battle..."
  Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  // "I can agree with what Garet is saying. It is definitely..."
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 10);
  // "Wouldn't it be something if we could use this power at any..."
  Event_OpenMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  } else {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(40);
    gFieldState->message++;
  }
  // Yes: "Yeah! Let's start trying to master this power!"
  // No: "You... You're not really into this, are you?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Sound_Play(MUS_FOREST_THEME);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  wait(60);
  gKolima_CurseDustWidth = 0;
  gKolima_CurseDustOffset = toFIXED(128);
  gKolima_CurseDustState = KOLIMA_DUST_CLOSING;
  Task_AddFrame(Kolima_UpdateCurseDust, TASK_PASS_FRAME);
  Event_Wait(80);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(60);
  Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 10);
  // A second shower raises the domes again before Tret and Laurel speak.
  Event_SetMessageId(MsgKolimaWatchOutItsHappeningAgain);
  // "Watch out! It's happening again!"
  Kolima_ShowMessageAndWait(ACTOR_IVAN, 10);
  Kolima_FaceActorAndWait(ACTOR_GARET, FACING_NORTH, 10);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
  if (gKolima_MiaInParty != 0) {
    Kolima_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
  }
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_GARET)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_IVAN)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_MIA)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Actor_SetSpritePriority(ACTOR_GARET, 3);
  Actor_SetSpritePriority(ACTOR_IVAN, 3);
  Actor_SetSpritePriority(ACTOR_MIA, 3);
  gKolima_CurseDustState = KOLIMA_DUST_CLOSING_WIDE;
  Sound_Play(SE_SHIMMER);
  Actor_SetPosition(ACTOR_LEADER_PSYNERGY_FIELD, 120, 248);
  growFieldActions = gKolima_PsynergyFieldGrowActions;
  Actor_SetScript(ACTOR_LEADER_PSYNERGY_FIELD, growFieldActions);
  Actor_SetPosition(ACTOR_IVAN_PSYNERGY_FIELD, 100, 274);
  Actor_SetScript(ACTOR_IVAN_PSYNERGY_FIELD, growFieldActions);
  if (gKolima_MiaInParty != 0) {
    Actor_SetPosition(ACTOR_MIA_PSYNERGY_FIELD, 74, 254);
    Actor_SetScript(ACTOR_MIA_PSYNERGY_FIELD, growFieldActions);
  }
  Actor_SetPosition(ACTOR_GARET_PSYNERGY_FIELD, 94, 225);
  Actor_SetScript(ACTOR_GARET_PSYNERGY_FIELD, growFieldActions);
  Event_Wait(120);
  gKolima_CurseDustState = KOLIMA_DUST_DRIFTING;
  do {
    wait(1);
  } while (gKolima_CurseDustState != KOLIMA_DUST_CENTRED);
  // Tret and Laurel, unseen, wonder at the party's immunity to the glamour.
  // "Hroom! Why? Why don't they turn into trees...?"
  Kolima_ShowMessageAndWait(ACTOR_TRET, 80);
  // "Who are they?"
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  // "What people are these, who are immune to Tret's glamour?"
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Scene_NodTogether();
  // "Have they come to save that village with their strange powers?"
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Scene_NodTogether();
  // "It will not be enough to save the people of Kolima."
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(40);
  // "They have earned their fate...Kolima took their axes to me,..."
  Kolima_ShowMessageAndWait(ACTOR_TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 0);
  Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 40);
  // "You children may not know this...but we are dying."
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 80);
  // "And Kolima Forest will wither and die when we are gone."
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 40);
  // "We are not the only ones who will die, Laurel. Once we are..."
  Kolima_ShowMessageAndWait(ACTOR_TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  // "Tret was once a kindly forest king."
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Scene_NodTogether();
  // "But he is slowly dying, and his heart has torn in two..."
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
  // "His wrathful side turned the people of Kolima into trees."
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  // "The kind Tret speaks no more."
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
  // "Hroom! The kindly heart invites destruction... I need no..."
  Kolima_ShowMessageAndWait(ACTOR_TRET, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "If you wish to save those whom Tret turned into trees, you..."
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(ACTOR_MIA, FACING_NORTH, 10);
  // "But...if you cannot stop Tret from withering, we all will..."
  Kolima_ShowMessageAndWait(ACTOR_LAUREL, 10);
  Scene_NodTogether();
  Event_Wait(60);
  // Laurel's and Tret's last words; then the party decides whether to help.
  // "Look for Tret deep within the forest."
  Event_ShowMessage(ACTOR_LAUREL, 0);
  // "It's no use, Laurel. Humans cannot be trusted! I shall die..."
  Event_ShowMessage(ACTOR_TRET, 0);
  Task_Remove(Kolima_UpdateCurseDust);
  Event_Wait(80);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  wait(80);
  Actor_Stop(ACTOR_LEADER_PSYNERGY_FIELD);
  Actor_Stop(ACTOR_IVAN_PSYNERGY_FIELD);
  miaInParty = &gKolima_MiaInParty;
  Actor_Stop(ACTOR_MIA_PSYNERGY_FIELD);
  Actor_Stop(ACTOR_GARET_PSYNERGY_FIELD);
  wait(1);
  shrinkFieldActions = gKolima_PsynergyFieldShrinkActions;
  Actor_SetScript(ACTOR_LEADER_PSYNERGY_FIELD, shrinkFieldActions);
  Actor_SetScript(ACTOR_IVAN_PSYNERGY_FIELD, shrinkFieldActions);
  if (*miaInParty != 0) {
    Actor_SetScript(ACTOR_MIA_PSYNERGY_FIELD, shrinkFieldActions);
  }
  Actor_RunScriptAndWait(ACTOR_GARET_PSYNERGY_FIELD, shrinkFieldActions);
  Event_Wait(20);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpritePriority(ACTOR_GARET, 2);
  Actor_SetSpritePriority(ACTOR_IVAN, 2);
  Actor_SetSpritePriority(ACTOR_MIA, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_IVAN)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_MIA)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
  // "Isaac... Isn't there anything we can do?"
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(10);
    // "What can we do? Do you have any idea?"
    Event_OpenMessage(ACTOR_GARET, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Kolima_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
      Event_Wait(40);
      Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
      // "It looks like Isaac has something in mind."
      Kolima_ShowMessageAndWait(ACTOR_GARET, 10);
      Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTH, 20);
      Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 20);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
      // "We'll leave it up to you, Isaac. Let's go save everyone."
      Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
      Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTHEAST, 20);
    } else {
      Kolima_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 20);
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
      Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
      Kolima_FaceActorAndWait(ACTOR_GARET, FACING_SOUTH, 20);
      Event_SetMessageId(MsgKolimaHesAsStumpedAsWe);
      // "He's as stumped as we are, but he says he'll help..."
      Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
      // "I trust Isaac. He'll come up with an idea."
      Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
    }
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  } else {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(10);
    Event_SetMessageId(MsgKolimaYoureIsaacThereIsntMuch);
    // "You're right, Isaac... There isn't much we can do now."
    Kolima_ShowMessageAndWait(ACTOR_GARET, 10);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 20);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    // "So the people of Kolima will just remain trees until they..."
    Event_OpenMessage(ACTOR_IVAN, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_Wait(20);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
      Event_Wait(40);
      Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 10);
      // "I'm disappointed in you, Isaac... How can you give up so..."
      Kolima_ShowMessageAndWait(ACTOR_IVAN, 10);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 10);
        Actor_StartRepeatedMotion(ACTOR_MIA, 3);
        // "That's right... We can't just leave them to rot!"
        Kolima_ShowMessageAndWait(ACTOR_MIA, 20);
      } else {
        gFieldState->message++;
      }
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetAttachedEffect(ACTOR_GARET,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(40);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      // "But how can we bring back the nice Tret and save the village?"
      Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
      Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Event_Wait(120);
      // "I guess I really don't know what to do."
      Kolima_ShowMessageAndWait(ACTOR_IVAN, 40);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 10);
        Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
        // "Ivan, you can't give up! Please, convince him to help!"
        Kolima_ShowMessageAndWait(ACTOR_MIA, 10);
      } else {
        gFieldState->message++;
      }
      Event_Wait(60);
      Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHWEST, 40);
        Kolima_FaceActorAndWait(ACTOR_IVAN, FACING_NORTHEAST, 20);
      }
      // "Just promise me this one thing..."
      Kolima_ShowMessageAndWait(ACTOR_IVAN, 10);
      Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      Event_Wait(40);
      // "Promise you will do everything in your power to save Kolima."
      Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
      Event_Wait(20);
      Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    } else {
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Event_Wait(40);
      Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
      Event_SetMessageId(MsgKolimaThatsReliefIsaacThoughtYoud);
      // "That's a relief, Isaac. I thought you'd given up on Kolima..."
      Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(ACTOR_MIA, FACING_EAST, 10);
        Actor_StartRepeatedMotion(ACTOR_MIA, 1);
        // "I was beginning to lose faith in you, Isaac."
        Kolima_ShowMessageAndWait(ACTOR_MIA, 20);
      } else {
        gFieldState->message++;
      }
      Actor_SetAttachedEffect(ACTOR_GARET,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(40);
      Actor_RunRepeatedMotion(ACTOR_GARET, 2);
      // "But what can we do? How can we bring back the good Tret and..."
      Kolima_ShowMessageAndWait(ACTOR_GARET, 20);
      Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
      Event_Wait(80);
      // "I really don't have any idea right now..."
      Kolima_ShowMessageAndWait(ACTOR_IVAN, 40);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(ACTOR_MIA, FACING_SOUTHEAST, 20);
        Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
        // "I'm not sure... What should we do?"
        Kolima_ShowMessageAndWait(ACTOR_MIA, 40);
      } else {
        gFieldState->message++;
      }
      Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
      Event_Wait(20);
      // "I don't know just yet... But I'm sure we'll think of something."
      Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
      Actor_StartRepeatedMotion(ACTOR_GARET, 2);
      Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Event_Wait(40);
      // "Well, let's get on with it!"
      Kolima_ShowMessageAndWait(ACTOR_IVAN, 20);
    }
  }
  // Reunite the party and remember that the first visit has played.
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_GARET, 1.2);
  Actor_SetSpeed(ACTOR_IVAN, 1.2);
  Actor_SetSpeed(ACTOR_MIA, 1.2);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_TRET, 0, 0);
  Actor_SetPosition(ACTOR_LAUREL, 0, 0);
  if (gKolima_MiaInParty != 0) {
    Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetDestination(ACTOR_MIA, actor->x.part[0].integer,
                           actor->z.part[0].integer);
    }
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
  }
  GameFlag_Set(FLAG_KOLIMA_CURSE_WITNESSED);
  Scene_PlayMusic();
  Event_End();
}

/// Shows the event's next line, spoken by actorId, then waits waitFrames.
static void Kolima_ShowMessageAndWait(s32 actorId, s32 waitFrames) {
  Event_ShowMessage(actorId, 0);
  Event_Wait(waitFrames);
}

/// Turns an actor at once to facing, then waits waitFrames.
static void Kolima_FaceActorAndWait(s32 actorId, s32 facing, s32 waitFrames) {
  Actor_FaceDirection(actorId, facing, 0);
  Event_Wait(waitFrames);
}

#include "shared/kolima_grow.c"

/// Every eighth frame, a golden ring of the curse falls from the sky over the
/// party, with its sound while gKolima_SparkleSoundOn is set.
static void Kolima_SpawnCurseSparkle(void) {
  struct Actor *sparkle;
  struct Sprite *sprite;
  s32 framePhase;

  framePhase = gFrameCount & 7;
  if (framePhase != 0) {
    return;
  }
  if (gKolima_SparkleSoundOn != 0) {
    Sound_Play(SE_SPARKLE_FALL);
  }
  sparkle =
      FieldActor_Create(SPRITE_GOLDEN_RINGS, toFIXED(98), 0, toFIXED(105));
  if (sparkle == NULL) {
    return;
  }
  sprite = sparkle->sprite;
  sprite->flags = framePhase;
  sparkle->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 1;
  sparkle->scaleX = FIXED_ONE / 10;
  sparkle->speedLimit = toFIXED(8);
  sparkle->acceleration = toFIXED(8);
  sparkle->motionFlags = framePhase;
  Actor_SelectAnimation(sparkle, 2);
  FieldActor_SetMoveTargetFX(sparkle, toFIXED(98), 0, toFIXED(269));
  Actor_StartScript(sparkle, gKolima_GoldenRingsScript);
}

#include "shared/kolima_blink.c"

/// A speck of the curse that lands inside the party's Psynergy fields (two
/// boxes with Mia in the party, three without her) bursts there with a drip,
/// and the fields take their next pose once grown
/// (Kolima_FlashHitPsynergyFields).
static s32 Kolima_BurstOnPsynergyField(struct Actor *actor) {
  if (gKolima_MiaInParty != 0) {
    if ((actor->x.fixed > toFIXED(59) && actor->x.fixed < toFIXED(141) &&
         actor->z.fixed > toFIXED(211) && actor->z.fixed < toFIXED(257)) ||
        (actor->x.fixed > toFIXED(69) && actor->x.fixed < toFIXED(122) &&
         actor->z.fixed > toFIXED(194) && actor->z.fixed < toFIXED(277))) {
      Sound_Play(SE_DRIP);
      Actor_StartScript(actor, gKolima_SparkleBurstScript);
      gKolima_PsynergyFieldHit = 1;
    }
  } else {
    if ((actor->x.fixed > toFIXED(59) && actor->x.fixed < toFIXED(111) &&
         actor->z.fixed > toFIXED(194) && actor->z.fixed < toFIXED(230)) ||
        (actor->x.fixed > toFIXED(111) && actor->x.fixed < toFIXED(141) &&
         actor->z.fixed > toFIXED(216) && actor->z.fixed < toFIXED(250)) ||
        (actor->x.fixed > toFIXED(78) && actor->x.fixed < toFIXED(122) &&
         actor->z.fixed > toFIXED(241) && actor->z.fixed < toFIXED(277))) {
      Sound_Play(SE_DRIP);
      Actor_StartScript(actor, gKolima_SparkleBurstScript);
      gKolima_PsynergyFieldHit = 1;
    }
  }
  return 0;
}

#include "shared/kolima_curse_dust.c"
s32 gKolima_PsynergyFieldHit BSS_DATA;
s32 gKolima_MiaInParty BSS_DATA;
s32 gKolima_SparkleSoundOn BSS_DATA;

/// Sets up Kolima Bridge.
static void Kolima_RestoreBridgeState(void) {
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    Actor_SetPosition(ACTOR_BRIDGE_VILLAGER, 0, 0);
    MapGrid_CopyCellAttributes(9, 17, 5, 1, 9, 18);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  } else {
    Actor_SetPosition(ACTOR_BRIDGE_VILLAGER_AS_TREE, 0, 0);
  }
  if (GameFlag_Get(FLAG_KOLIMA_BRIDGE_LOWERED) != 0) {
    MapGrid_CopyCellsTo(91, 19, 72, 9, 5, 7);
    MapGrid_CopyCellAttributes(23, 11, 5, 7, 8, 11);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
}

/// Where Garet, Ivan and Mia walk on into the silent village and the way each
/// turns (Kolima_WitnessForestCurse).
s32 gKolima_GaretWalksIn[] = {
    SC_ANIMATION(2), SC_MOVE(97, 0, 221),
    SC_SYNC,         SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_ANIMATION(1), SC_END,
};

s32 gKolima_IvanWalksIn[] = {
    SC_ANIMATION(2), SC_MOVE(101, 0, 268),
    SC_SYNC,         SC_SET(SW_FACING, FACING_EAST),
    SC_ANIMATION(1), SC_END,
};

s32 gKolima_MiaWalksIn[] = {
    SC_ANIMATION(2), SC_MOVE(78, 0, 247),
    SC_SYNC,         SC_SET(SW_FACING, FACING_NORTH),
    SC_ANIMATION(1), SC_END,
};

#include "shared/kolima_squash.c"

s32 gKolima_PsynergyFieldGrowActions[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE / 10),
    SC_SET(SW_SCALE_Y, FIXED_ONE / 10),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 50),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 50),
    SC_WAIT(1),
    SC_LOOP(70, SC_TO(1)),
    SC_CALL(Kolima_FlashHitPsynergyFields),
    SC_END,
};

s32 gKolima_PsynergyFieldShrinkActions[] = {
    SC_ANIMATION(1),
    SC_ADD(SW_Z, -toFIXED(1) / 10),
    SC_ADD(SW_SCALE_X, -3 * FIXED_ONE / 200),
    SC_ADD(SW_SCALE_Y, -3 * FIXED_ONE / 200),
    SC_WAIT(1),
    SC_LOOP(90, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// The sparkles on Kolima's ground: three quarters of their size, passed
/// through, twinkling at random (Kolima_TwinkleGroundSparkle).
s32 gKolima_GroundSparkleActions[] = {
    SC_SET(SW_SCALE_X, 3 * FIXED_ONE / 4),
    SC_SET(SW_SCALE_Y, 3 * FIXED_ONE / 4),
    SC_SET(SW_COLLISION, 0),
    SC_SET(SW_TIMER, 0),
    SC_LABEL(1),
    SC_CALL(Kolima_TwinkleGroundSparkle),
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

/// A door opening: two pictures of a cell column two cells high.
u16 gKolima_DoorCells[] = {
    MAPGRID_ANIMATION_STEP(66, 32, 1, 2, 4),
    MAPGRID_ANIMATION_STEP(67, 32, 1, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

/// Kolima's entrances. No exit leads to entrances 2, 10 and 12. Entrance 10
/// puts the party where KOLIMA_FROM_WORLD_MAP does.
struct SceneEntrance gKolimaEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 232, 0, 120, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
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
    {KOLIMA_TUNNEL_FROM_LOG_ROOMS_C, CONDITION_ALWAYS, 216, 0, 320,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KOLIMA_TUNNEL_FROM_KOLIMA, CONDITION_ALWAYS, 312, 0, 312, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneExitRegion gKolimaTunnelRegions[] = {
    {207, 31, 311, 225, 49, 329, -1, 1},
    {303, 31, 303, 321, 49, 321, -1, 2},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

/// Kolima's exits, then Kolima Bridge's and Kolima Tunnel's.
u32 gKolimaExits[] = {
    SCENE_EXITS(Kolima),
    SCENE_EXIT(KOLIMA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KOLIMA_WEST),
    SCENE_EXIT(KOLIMA_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_KOLIMA_EAST),
    SCENE_EXIT(KOLIMA_TO_INN, LogRoomsC, LOG_ROOMS_C_INN_FROM_KOLIMA),
    SCENE_EXIT(KOLIMA_TO_ITEM_SHOP, LogRoomsC, LOG_ROOMS_C_ITEM_SHOP_FROM_KOLIMA),
    SCENE_EXIT(KOLIMA_TO_HOUSE_SOUTH, LogRoomsC, LOG_ROOMS_C_HOUSE_FROM_KOLIMA_SOUTH),
    SCENE_EXIT(KOLIMA_TO_WEAPON_AND_ARMOR_SHOP, LogRoomsC, LOG_ROOMS_C_WEAPON_AND_ARMOR_SHOP_FROM_KOLIMA),
    SCENE_EXIT(KOLIMA_TO_HOUSE_NORTH, LogRoomsC, LOG_ROOMS_C_HOUSE_FROM_KOLIMA_NORTH),
    SCENE_EXIT(KOLIMA_TO_KOLIMA_TUNNEL, KolimaTunnel, KOLIMA_TUNNEL_FROM_KOLIMA),
    SCENE_EXITS(KolimaBridge),
    SCENE_EXIT(KOLIMA_BRIDGE_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_KOLIMA_BRIDGE_SOUTH),
    SCENE_EXIT(KOLIMA_BRIDGE_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_KOLIMA_BRIDGE_NORTH),
    SCENE_EXITS(KolimaTunnel),
    SCENE_EXIT(KOLIMA_TUNNEL_TO_LOG_ROOMS_C, LogRoomsC, LOG_ROOMS_C_FROM_KOLIMA_TUNNEL),
    SCENE_EXIT(KOLIMA_TUNNEL_TO_KOLIMA, Kolima, KOLIMA_FROM_KOLIMA_TUNNEL),
    SCENE_EXITS_END,
};

struct ActorPlacement gKolimaTunnelActorPlacements[] = {
    { SCENE_TABLE_END },
};

/// Kolima's people: Garet, Ivan and Mia for the curse, the nine villagers,
/// Tret's and Laurel's voices and the four domes until the curse has been seen
/// (FLAG_KOLIMA_CURSE_WITNESSED), four sparkles while it lasts, and Granite
/// until it joins (its Djinni flag, FLAG_DJINNI(DJINNI_GRANITE)).
struct ActorPlacement gKolimaActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 192, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 312, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        168, 0, 360, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        344, 0, 280, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 392, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        248, 0, 184, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 264, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 264, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_TRET, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LAUREL, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PSYNERGY_FIELD, FLAG_KOLIMA_CURSE_WITNESSED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
        24, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
        120, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
        152, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_STAND,
        40, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_GRANITE), MOVEMENT_TYPE_STAND,
        328, 0, 360, FACING_EAST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

/// Five more sparkles, placed by Scene_Initialize once the curse has been seen
/// and kept while it lasts.
struct ActorPlacement gKolima_MoreGroundSparklePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_RESET_ACTION,
        120, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_RESET_ACTION,
        296, 0, 376, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_RESET_ACTION,
        440, 0, 344, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_RESET_ACTION,
        296, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, FLAG_KOLIMA_CURSE_LIFTED, MOVEMENT_TYPE_RESET_ACTION,
        152, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

/// Kolima Bridge's villager, as a man (ACTOR_BRIDGE_VILLAGER) and as a tree
/// (ACTOR_BRIDGE_VILLAGER_AS_TREE), in the same place.
/// Kolima_RestoreBridgeState puts one of them out of sight.
struct ActorPlacement gKolimaBridgeActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        234, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        234, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

/// The triggers Kolima and Kolima Tunnel share: the exits and doorways, the
/// villagers' words once the curse is lifted and their thoughts before and
/// after, the healer, Granite and two jars.
///
/// Exits 1 and 2, which cell 99 takes too, keep their numbers, since each
/// scene's exit table gives them its own doors. Those are KOLIMA_TO_WORLD_MAP
/// and KOLIMA_TO_WORLD_MAP_EAST in Kolima, KOLIMA_TUNNEL_TO_LOG_ROOMS_C and
/// KOLIMA_TUNNEL_TO_KOLIMA in Kolima Tunnel.
struct Trigger gKolimaTriggers[] = {
    { TRIGGER_EXIT, 99, CONDITION_ALWAYS, 1 },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2 },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT(Kolima_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(Kolima_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT(Kolima_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(Kolima_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 9, CONDITION_ALWAYS, EVENT(Kolima_EnterDoor) },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, KOLIMA_TO_KOLIMA_TUNNEL },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_VILLAGE_OF_LUMBERJACKS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaThisIsKolimaVillageOfLumberjacks) },
    { TRIGGER_TALK, ACTOR_VILLAGER_RELYING_ON_FORESTS, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaWeRelyOnTheForestsSo) },
    { TRIGGER_TALK, ACTOR_VILLAGER_TIME_TO_THINK, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaYouKnowBeingATreeGives) },
    { TRIGGER_TALK, ACTOR_VILLAGER_SPARKLES_FELL, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaITurnedIntoATreeRight) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_MCCOYS_MONEY, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaDontBlameTheVillagersForBeing) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_HOLY_TREE_PALACE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaWhoWouldTryToUseThe) },
    { TRIGGER_TALK, ACTOR_BOY_DID_NOT_MIND_BEING_A_TREE, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIDidntMindBeingATree) },
    { TRIGGER_TALK, ACTOR_GIRL_SLEEPY_TREES, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaYawnTreesAreSoSleepy) },
    { TRIGGER_TALK, ACTOR_HEALER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT(Kolima_TalkToHealer) },
    { TRIGGER_TALK, ACTOR_GRANITE, CONDITION_ALWAYS, EVENT(Kolima_GraniteJoins) },
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
    { EXAMINE_TRIGGER(EXAMINE_JAR), 100, FLAG_KOLIMA_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_KOLIMA_HERB, FIND_ITEM(ITEM_HERB) },
    { TRIGGER_TABLE_END },
};

/// Kolima Bridge's triggers: its two ends, the bridge's fall and the villager.
struct Trigger gKolimaBridgeTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, KOLIMA_BRIDGE_TO_WORLD_MAP_SOUTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, KOLIMA_BRIDGE_TO_WORLD_MAP_NORTH },
    { TRIGGER_STEP, 10, FLAG_KOLIMA_BRIDGE_LOWERED, EVENT(Kolima_LowerBridge) },
    { TRIGGER_TALK, ACTOR_BRIDGE_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgKolimaForAWhileIThoughtNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIDGE_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgKolimaYouWouldntUnderstandButItsTruly) },
    { TRIGGER_TABLE_END },
};
// clang-format on

s32 gKolima_GoldenRingsScript[] = {
    SC_CALL(FieldEffect_StretchUntilArrived),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

#include "shared/kolima_sparkle_burst_script.c"

s32 gKolima_CurseDustScript[] = {
    SC_ADD(SW_Y, -toFIXED(2)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_ADD(SW_Y, -toFIXED(2)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_Y, -toFIXED(1)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(1)),
    SC_CALL(Kolima_BurstOnPsynergyField),
    SC_RELEASE,
};
