/// @file
/// Altin, the mining village flooded by the water the hydros statues in the
/// mine spew. Two scenes share this overlay:
///
/// - the village itself, where the water drops in three stages as the party
///   drains the mine, a water beast spits water at the houses until the
///   party drives it off, and the shopkeepers wait for their shops to dry;
/// - the houses and the inn, where families sheltering from the flood go
///   home as the water recedes.
///
/// A third, empty set of tables serves any other scene number.

#include "game_state.h"
#include "version.h"
#include "scene_ids.h"
#include "unit/scene/altin.h"
#include "unit/object/object.h"
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
#include "types.h"
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "altin.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "songs.h"
#include "objcmd.h"

#if !TBS_PATCH_3
#define ALTIN_COOK_LAST_POT_X 384
#else
#define ALTIN_COOK_LAST_POT_X 380
#endif

#define SCENE_ALTIN_VILLAGE ((s32)&SceneId_Altin1)
#define SCENE_ALTIN_HOUSES ((s32)&SceneId_Altin2)

/// The village's actors.
enum AltinVillageActor {
  ACTOR_GREETING_VILLAGER = 8,
  ACTOR_CURIOUS_VILLAGER = 9,
  ACTOR_THIRSTY_VILLAGER = 14,
  /// Villagers who come back as the water recedes.
  ACTOR_RETURNING_VILLAGER_A = 15,
  ACTOR_RETURNING_VILLAGER_B = 16,
  ACTOR_RETURNING_VILLAGER_C = 17,
  ACTOR_WATER_BEAST = 18,
  ACTOR_MOVABLE_BLOCK = 19,
  /// The armor and item sellers, waiting outside while their shops are
  /// under water.
  ACTOR_FLOODED_ARMOR_SELLER = 20,
  ACTOR_FLOODED_ITEM_SELLER = 21,
  /// Three ground sprites drawn under the party.
  ACTOR_FIRST_GROUND_SPRITE = 22,
  ACTOR_SECOND_GROUND_SPRITE = 23,
  ACTOR_THIRD_GROUND_SPRITE = 24
};

/// The houses' actors.
enum AltinHouseActor {
  /// The family sheltering in the inn's Room One until the first drop.
  ACTOR_ROOM_ONE_GUEST = 8,
  ACTOR_ROOM_ONE_SECOND_GUEST = 9,
  /// The family in Room Two, until the second drop.
  ACTOR_ROOM_TWO_GUEST = 10,
  ACTOR_ROOM_TWO_SECOND_GUEST = 11,
  ACTOR_KISSING_VILLAGER = 14,
  ACTOR_WEAPON_SELLER = 15,
  ACTOR_ITEM_SELLER = 16,
  ACTOR_ARMOR_SELLER = 17,
  ACTOR_INNKEEPER = 18,
  ACTOR_COOK = 20,
  ACTOR_MINE_WORKER = 21
};

/// The village's water level, set on entry from the mine's drain flags.
enum {
  FLAG_WATER_DROPPED_ONCE = FLAG_SCENE_TEMP_40,
  FLAG_WATER_DROPPED_TWICE = FLAG_SCENE_TEMP_41,
  FLAG_WATER_DRAINED = FLAG_SCENE_TEMP_42
};

enum {
  SHOP_ALTIN_WEAPONS = 19,
  SHOP_ALTIN_ARMOR = 20,
  SHOP_ALTIN_ITEMS = 21,
  INN_ALTIN = 6
};

enum {
  ENTRANCE_FROM_MINE = 3,
  /// The door triggers are numbered from 2 in the village's event table.
  FIRST_DOOR_TRIGGER = 2,
  DOOR_MINE = 1,
  DOOR_WITH_WINDOW = 3,
  DOOR_STRAIGHT_IN = 6
};

enum { SPRITE_SPRAY = 222, ANIM_BEAST_SPIT = 3 };

#define WATER_BEAST_HOP_PERIOD 60
#define WATER_SPRAY_LIFETIME 20
#define WATER_SPRAY_START_SCALE 0x4ccc

// UB: the native facing test overflows a signed left shift for many headings.
#ifdef UBFIX
#define ALTIN_FACES_COUNTER(facing) FACING_IS_NORTH(facing)
#else
#define ALTIN_FACES_COUNTER(facing)                                            \
  ((u32)(((facing) + 0x5fff) << 16) <= 0x3ffe0000)
#endif

// UB: the random wobble can be negative. Both factors fit in signed32 bits
// for the actor's signed16-bit timer and wobble values.
#ifdef UBFIX
#define WATER_SPRAY_LATERAL_STEP(timer, wobble)                                \
  ((timer) * 0x1000 + (wobble) * 0x8000)
#else
#define WATER_SPRAY_LATERAL_STEP(timer, wobble)                                \
  (((timer) << 12) + ((wobble) << 15))
#endif

enum { TRIGGER_NEAR_BEAST = 20 };

/// The highest actor slot.
enum { ACTOR_LAST_PLACED = 65 };

static void Village_ApplyWaterLevel(void);
static void Houses_PlaceSheltering(void);
static void Beast_SpawnSpray(void);

/// Returns the entrance table of the scene being entered.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == SCENE_ALTIN_VILLAGE) {
    return gAltinVillage_Entrances;
  }
  if (scene == SCENE_ALTIN_HOUSES) {
    return gAltinHouses_Entrances;
  }
  return gAltinOther_Entrances;
}

/// Neither scene has regions.
const struct SceneRegion *Scene_GetRegions(void) { return 0; }

/// Both scenes share one exit table.
const u32 *Scene_GetExits(void) { return gAltin_Exits; }

/// Returns the actor placements of the scene being entered. Some villagers
/// only turn back after talking once the flood is over or their family has
/// gone home.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s32 scene = gGameState.scene;
  if (scene == SCENE_ALTIN_VILLAGE) {
    if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED)) {
      gAltinVillage_Placements[5].talkFacing = TALK_FACE_PARTY;
      gAltinVillage_Placements[6].talkFacing = TALK_FACE_PARTY;
    }
    return gAltinVillage_Placements;
  }
  if (scene == SCENE_ALTIN_HOUSES) {
    if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN))
      gAltinHouses_Placements[1].talkFacing = TALK_FACE_PARTY_AND_BACK;
    if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) ||
        Engine_GameFlagIsSet(FLAG_ALTIN_MINE_THIRD_DRAIN))
      gAltinHouses_Placements[3].talkFacing = TALK_FACE_PARTY_AND_BACK;
    FieldScene_PrepareActors(gAltinHouses_Placements);
    return gAltinHouses_Placements;
  }
  return gAltinOther_Placements;
}

/// Returns the event table of the scene being entered.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == SCENE_ALTIN_VILLAGE) {
    return gAltinVillage_Events;
  }
  if (scene == SCENE_ALTIN_HOUSES) {
    return gAltinHouses_Events;
  }
  return gAltinOther_Events;
}

/// A villager asks whether this is the party's first visit; either way, the
/// flood has changed Altin.
static void Village_AskAboutFirstVisit(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinYourFirstTimeVisitAltin);

  Engine_EventOpenMessage(ACTOR_GREETING_VILLAGER, 0);

  if (Engine_EventChooseYesNo(0, 0) == 1) {
    Engine_EventShowMessage(ACTOR_GREETING_VILLAGER, 0);
  } else {
    Event_SkipMessages(1);
    Engine_EventAskYesNo(ACTOR_GREETING_VILLAGER, 0);
  }

  Engine_EventEnd();
}

/// A villager asks whether the slain hydros creatures gushed water.
static void Village_AskAboutHydros(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinDidSeeWaterGushingOut);
  Engine_EventAskYesNo(ACTOR_CURIOUS_VILLAGER, 0);
  Engine_EventEnd();
}

/// While the beasts live, this villager complains that their water cannot be
/// drunk, and the first time the camera pans to the water beast spitting at
/// the houses. Once the statues are gone, he asks for help rebuilding.
static void Village_TalkToThirstyVillager(void) {

  struct FieldActor *beast;
  struct FieldActor *leader;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_EventSetMessage(MsgAltinOnesWhoDefeatedWaterBeasts);
    Event_AskYesNo(ACTOR_THIRSTY_VILLAGER, 0);
  } else {
    Engine_ActorSetAnimation(ACTOR_THIRSTY_VILLAGER, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(MsgAltinWeCantDrinkWaterMonsters);
    Event_ShowMessageAndWait(ACTOR_THIRSTY_VILLAGER, 0, 10);
    if (GameFlag_IsSet(FLAG_ALTIN_WATER_BEAST_LEFT) == 0) {
      beast = Object_GetById(ACTOR_WATER_BEAST);

      Camera_GetEventObject()->motionFlags = 0;
      Camera_SetSpeed(0x10000, 0x2000);
      Camera_MoveTo(beast->x.fixed, beast->y.fixed, beast->z.fixed, 1);
      // 0x4000 is south as a facing, but this call takes an actor number.
      Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
      Actor_FaceDirection(ACTOR_THIRSTY_VILLAGER, FACING_SOUTH - FACING_STEP,
                          0);
      Engine_CameraWaitForMove();
      Engine_EventWait(120);
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      Camera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
      Engine_CameraWaitForMove();
    }
    Engine_ActorSetAnimationAndWait(ACTOR_THIRSTY_VILLAGER, ANIM_SHAKE_HEAD);
  }
  Engine_EventEnd();
}

/// A villager asks whether the party really found ruins deep in the mine.
static void Village_AskAboutRuins(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinTrueFoundAncientRuinsIn);
  Engine_EventAskYesNo(ACTOR_RETURNING_VILLAGER_C, 0);
  Engine_EventEnd();
}

/// A guest asks whether the party is who the girl from Xian was looking for.
static void Houses_AskAboutXianGirl(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinGirlFromXianWasAsking);
  Engine_EventAskYesNo(ACTOR_ROOM_ONE_SECOND_GUEST, 0);
  Engine_EventEnd();
}

/// A guest asks whether the party seeks the way west, and points to the
/// bottom of the mine.
static void Houses_AskAboutWayWest(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinTryingFindYourWayWest);
  Engine_EventAskYesNo(ACTOR_ROOM_TWO_GUEST, 0);
  Engine_EventEnd();
}

/// A grateful villager thanks the party with a kiss.
static void Houses_AskAboutMonsters(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinDefeatedThoseMonstersDidnt);
  Engine_EventAskYesNo(ACTOR_KISSING_VILLAGER, 0);
  Engine_EventEnd();
}

/// A miner asks whether the remaining beasts are deep below.
static void Houses_AskAboutMineBeasts(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinThereFewBeastsInMine);
  Engine_EventAskYesNo(ACTOR_MINE_WORKER, 0);
  Engine_EventEnd();
}

/// Returns whether the party leader faces north, toward a shop counter: its
/// facing lies strictly between northwest and northeast, measured in 16 bits.
static s32 Leader_FacesCounter(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (ALTIN_FACES_COUNTER(leader->facing)) {
    return 1;
  }
  return 0;
}

/// The weapon seller: until the water drains he cannot sell; afterwards he
/// opens his shop when spoken to across the counter.
static void Houses_TalkToWeaponSeller(void) {

  if (Engine_GameFlagIsSet(FLAG_WATER_DRAINED) == 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgAltinDoWantWeapons);

    Engine_EventAskYesNo(ACTOR_WEAPON_SELLER, 0);
    Engine_EventEnd();
    return;
  }

  if (Leader_FacesCounter() != 0) {
    Engine_ShopOpen(SHOP_ALTIN_WEAPONS, ACTOR_WEAPON_SELLER);
    return;
  }

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinThankGoodnessWaterHasReceded);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_EventSetMessage(MsgAltinYoullHaveFindPassageIn);
  }
  Engine_EventShowMessage(ACTOR_WEAPON_SELLER, 0);
  Engine_EventEnd();
}

/// The armor seller, in either scene: outside, while his shop is under water,
/// he laments; once it has dried after the second drop, he sells across his
/// counter in the houses.
static void Altin_TalkToArmorSeller(void) {

  if (Engine_GameFlagIsSet(FLAG_WATER_DROPPED_TWICE) == 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgAltinMyStoreSubmergedWantSell);
    Engine_EventShowMessage(ACTOR_FLOODED_ARMOR_SELLER, 0);
    Engine_EventEnd();
    return;
  }

  if (Leader_FacesCounter() != 0) {
    Engine_ShopOpen(SHOP_ALTIN_ARMOR, ACTOR_ARMOR_SELLER);
    return;
  }

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinItsGreatCanSellArmor);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_EventSetMessage(MsgAltinHowAboutArentImpressedBy);
  }
  Engine_EventShowMessage(ACTOR_ARMOR_SELLER, 0);
  Engine_EventEnd();
}

/// The item seller, the same way: she reopens after the first drop.
static void Altin_TalkToItemSeller(void) {
  if (Engine_GameFlagIsSet(FLAG_WATER_DROPPED_ONCE) == 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgAltinWillDoIfMyMerchandise);
    Engine_EventShowMessage(ACTOR_FLOODED_ITEM_SELLER, 0);
    Engine_EventEnd();
    return;
  }

  if (Leader_FacesCounter() != 0) {
    Engine_ShopOpen(SHOP_ALTIN_ITEMS, ACTOR_ITEM_SELLER);
    return;
  }

  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltinNoneMyGoodsWereDamaged);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_EventSetMessage(MsgAltinGirlFromXianBoughtLot);
  }
  Engine_EventShowMessage(ACTOR_ITEM_SELLER, 0);
  Engine_EventEnd();
}

/// The innkeeper opens the inn across the counter, and otherwise talks about
/// the flood or, once it is over, the temple west of the mine.
static void Houses_TalkToInnkeeper(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (ALTIN_FACES_COUNTER(leader->facing)) {
    Engine_InnOpen(INN_ALTIN, ACTOR_INNKEEPER);
    return;
  }

  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0) {
    Engine_EventSetMessage(MsgAltinThereSmallTempleWestAltin);
    Engine_EventShowMessage(ACTOR_INNKEEPER, 0);
  } else {
    Engine_EventSetMessage(MsgAltinWeGotLittleDampBut);
    Engine_EventAskYesNo(ACTOR_INNKEEPER, 0);
  }

  Engine_EventEnd();
}

/// Walking north into one of the village's doors: everyone stops, the door
/// opens and the party leader walks in. The mine's wide door is redrawn in
/// two halves by hand; the others play their opening pictures.
static void Village_EnterDoor(void) {
  u32 actorId;
  struct EventWork *event;
  struct FieldActor *actor;
  s32 doorIndex;
  u8 *doorBytes;
  s32 doorByteOffset;
  s32 coordinatesByteOffset;
  s32 column;
  s32 row;
  s32 rightColumn;

  event = gEventWork;
  Engine_EventBegin();
  for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_PLACED; actorId++) {
    actor = Object_GetById(actorId);
    if (actor != 0) {
      actor->motionFlags = 0;
    }
  }

  // The bytes read the entry through byte offsets and sign-extend the door
  // number by shifts; indexing the structs loads the fields differently.
  doorBytes = (u8 *)gAltinVillage_Doors;
  doorIndex =
      ((*(u16 *)&event->touchedTrigger - FIRST_DOOR_TRIGGER) << 16) >> 16;
  doorByteOffset = doorIndex << 3;
  coordinatesByteOffset = doorByteOffset + 4;
  column = *(s16 *)(doorBytes + coordinatesByteOffset);
  row = *(s16 *)(doorBytes + coordinatesByteOffset + 2);
  if (doorIndex == DOOR_MINE) {
    // The mine's door slides open with SE_188, as Lama Temple's does.
    Event_PlayCue(SE_188);
    Map_CopyCellsTo(42, 33, column, row, 2, 2);
    rightColumn = column + 2;
    Map_CopyCellsTo(42, 35, rightColumn, row, 2, 2);
    Engine_EventWait(4);
    Map_CopyCellsTo(40, 33, column, row, 2, 2);
    Map_CopyCellsTo(40, 35, rightColumn, row, 2, 2);
    Engine_EventWait(4);
  } else {
    Event_PlayCue(SE_DOOR_OPEN);
    if (doorIndex == DOOR_WITH_WINDOW) {
      Map_CopyCellsTo(33, 42, 8, 17, 1, 2);
    }
    Map_AnimateCells(*(s32 *)(doorBytes + doorByteOffset), column, row);
  }

  // Walk the leader through.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (doorIndex == DOOR_STRAIGHT_IN) {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, 0);
  } else {
    if (doorIndex != DOOR_MINE) {
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -4);
    } else {
      Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
      Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
    }
  }
  Engine_EventWait(10);
  Engine_EventRequestExit(event->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// Walking north into a doorway without a door: the leader walks out.
static void Village_WalkThroughDoorway(void) {
  struct EventWork *work;

  work = gEventWork;
  Engine_EventBegin();
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Event_PlayCue(SE_MAP_EXIT);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Engine_EventRequestExit(work->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// Update callback of the hopping water beast: once a second it leaps up,
/// falls back with a splash that shakes the screen briefly, and stands while
/// on the ground.
static void Beast_Hop(union FieldObject *fieldObject) {
  struct WaterActor *beast = (struct WaterActor *)fieldObject;
  if (beast->shakeFrames != 0) {
    if (--beast->shakeFrames == 1)
      Map_SetShake(-1, -1, 0xe666);
  }
  if (beast->velocityY == 0) {
    Object_SetMode(beast, 1);
    beast->y += -0x18000;
    if (beast->y < beast->ground) {
      if (beast->counter != 0) {
        Event_PlayCue(SE_229);
        beast->counter = 0;
        beast->shakeFrames = 4;
        Map_SetShake(0, 0x10000, 0x10000);
      }
      beast->y = beast->ground;
    }
    beast->onGround = 1;
  } else {
    beast->onGround = 0;
  }
  if (beast->timer == 0) {
    Event_PlayCue(SE_JUMP);
    beast->counter = 1;
    Object_SetMode(beast, 2);
    beast->velocityY = 0x30000;
  }
  if (++beast->timer == WATER_BEAST_HOP_PERIOD)
    beast->timer = 0;
}

/// The water beast hops away across the flood and dives in with a splash.
static void Beast_HopAway(void) {
  struct WaterActor *beast;

  beast = (struct WaterActor *)Object_GetById(ACTOR_WATER_BEAST);
  beast->timer = 0;
  beast->shakeFrames = 0;
  ((struct FieldActor *)beast)->gravity = 0x6666;
  beast->update = Beast_Hop;
  Actor_SetSpeed(ACTOR_WATER_BEAST, 0x13333, 0x9999);
  Actor_MoveToAndWait(ACTOR_WATER_BEAST, 28, 460);
  Actor_MoveToAndWait(ACTOR_WATER_BEAST, 24, 448);
  Event_PlayCue(SE_229);
  Engine_ActorDestroy(ACTOR_WATER_BEAST);
  Map_SetShake(0, 0x10000, 0x10000);
  Engine_EventWait(4);
  Map_SetShake(-1, -1, 0xe666);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_WATER_BEAST, ANIM_STAND);
}

/// After Move: remembers whether the block now stands in map column 22, the
/// spot it is pushed to.
static void Village_CheckBlockMoved(void) {
  struct FieldActor *block = Object_GetById(ACTOR_MOVABLE_BLOCK);

  if ((block->x.fixed >> 20) == 22) {
    Engine_GameFlagSet(FLAG_ALTIN_BLOCK_MOVED);
  } else {
    Engine_GameFlagClear(FLAG_ALTIN_BLOCK_MOVED);
  }
}

/// Works out the village's water level from the mine's drain flags, then sets
/// up whichever scene is being entered. Returns 0.
s32 Scene_Initialize(void) {
  s16 scene;

  if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN) != 0) {
    Engine_GameFlagSet(FLAG_WATER_DROPPED_ONCE);
  }

  if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0 ||
      Engine_GameFlagIsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    Engine_GameFlagSet(FLAG_WATER_DROPPED_TWICE);
  }

  if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0 &&
      Engine_GameFlagIsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    Engine_GameFlagSet(FLAG_WATER_DRAINED);
  }

  scene = gGameState.scene;
  if (scene == SCENE_ALTIN_VILLAGE) {
    Village_ApplyWaterLevel();
  } else if (scene == SCENE_ALTIN_HOUSES) {
    Houses_PlaceSheltering();
  }

  return 0;
}

/// Sets up the village: the map half showing the current water level (the
/// cells kept in four 32 by 32 blocks), the villagers and sellers present at
/// that level, the water beast with its spray while it remains, and the
/// pushed block. Then redraws the map around the leader.
static void Village_ApplyWaterLevel(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  // Select the water-level map and remove villagers whose areas are flooded.
  if (GameFlag_IsSet(FLAG_WATER_DRAINED) != 0) {
    Map_CopyCellsTo(64, 32, 0, 32, 32, 32);
    Map_CopyCellAttributes(64, 32, 32, 32, 0, 0);
    Engine_ActorDestroy(ACTOR_FLOODED_ARMOR_SELLER);
    Engine_ActorDestroy(ACTOR_FLOODED_ITEM_SELLER);
  } else if (GameFlag_IsSet(FLAG_WATER_DROPPED_TWICE) != 0) {
    Map_CopyCellsTo(64, 0, 0, 32, 32, 32);
    Map_CopyCellAttributes(64, 0, 32, 32, 0, 0);
    Engine_ActorDestroy(ACTOR_RETURNING_VILLAGER_C);
    Engine_ActorDestroy(ACTOR_FLOODED_ARMOR_SELLER);
    Engine_ActorDestroy(ACTOR_FLOODED_ITEM_SELLER);
  } else if (GameFlag_IsSet(FLAG_WATER_DROPPED_ONCE) != 0) {
    Map_CopyCellsTo(0, 64, 0, 32, 32, 32);
    Map_CopyCellAttributes(0, 64, 32, 32, 0, 0);
    Engine_ActorDestroy(ACTOR_RETURNING_VILLAGER_B);
    Engine_ActorDestroy(ACTOR_RETURNING_VILLAGER_C);
    Engine_ActorDestroy(ACTOR_FLOODED_ITEM_SELLER);
  } else {
    Map_CopyCellAttributes(0, 32, 32, 32, 0, 0);
    Engine_ActorDestroy(ACTOR_RETURNING_VILLAGER_A);
    Engine_ActorDestroy(ACTOR_RETURNING_VILLAGER_B);
    Engine_ActorDestroy(ACTOR_RETURNING_VILLAGER_C);
  }
  // Restore the remaining beast spitting water, with its sound (SE_170) and
  // its spray.
  if (GameFlag_IsSet(FLAG_ALTIN_WATER_BEAST_LEFT) != 0) {
    Engine_ActorDestroy(ACTOR_WATER_BEAST);
  } else {
    BattleFx_SetQueuedSoundAndPlay(SE_170);
    Actor_SetChildValue(ACTOR_WATER_BEAST, 2);
    Engine_ActorSetAnimation(ACTOR_WATER_BEAST, ANIM_BEAST_SPIT);
    FieldScheduler_AddCallback(Beast_SpawnSpray, TASK_PRIORITY_SCENE);
  }
  if (gGameState.entrance == ENTRANCE_FROM_MINE) {
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  }
  Map_CopyCellAttributes(0, 33, 4, 3, 20, 41);
  if (GameFlag_IsSet(FLAG_ALTIN_BLOCK_MOVED) != 0) {
    Actor_SetPosition(ACTOR_MOVABLE_BLOCK, PIXELS(360), PIXELS(168));
  }
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_MOVABLE_BLOCK), 0);
  // Keep the three ground sprites below the party before redrawing.
  Actor_SetChildValue(ACTOR_FIRST_GROUND_SPRITE, 15);
  Engine_ActorSetChildValue(ACTOR_SECOND_GROUND_SPRITE, 15);
  Actor_SetChildValue(ACTOR_THIRD_GROUND_SPRITE, 15);
  Object_GetById(ACTOR_FIRST_GROUND_SPRITE)->collisionFlags |= 8;
  Object_GetById(ACTOR_SECOND_GROUND_SPRITE)->collisionFlags |= 8;
  Object_GetById(ACTOR_THIRD_GROUND_SPRITE)->collisionFlags |= 8;
  Object_GetById(ACTOR_FIRST_GROUND_SPRITE)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Object_GetById(ACTOR_SECOND_GROUND_SPRITE)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Object_GetById(ACTOR_THIRD_GROUND_SPRITE)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Engine_ActorSetSpritePriority(ACTOR_FIRST_GROUND_SPRITE, 1);
  Engine_ActorSetSpritePriority(ACTOR_SECOND_GROUND_SPRITE, 1);
  Engine_ActorSetSpritePriority(ACTOR_THIRD_GROUND_SPRITE, 1);
  // Redraw at the leader after the map and object state are ready.
  Engine_TaskWait(1);
  Engine_EventBegin();
  Camera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
  Engine_MapRedraw();
  Engine_EventEnd();
  Engine_TaskWait(1);
}

/// Sets up the houses: the families still sheltering in the inn's rooms, the
/// weapon seller waiting there until the water drains, and the shopkeepers
/// kept from wandering.
static void Houses_PlaceSheltering(void) {
  struct FieldActor *actor;

  if (GameFlag_IsSet(FLAG_WATER_DROPPED_ONCE) == 0) {
    Actor_SetPosition(ACTOR_ROOM_ONE_GUEST, PIXELS(808), PIXELS(727));
    Object_GetById(ACTOR_ROOM_ONE_GUEST)->facing = FACING_SOUTH - FACING_STEP;
    Actor_SetPosition(ACTOR_ROOM_ONE_SECOND_GUEST, PIXELS(794), PIXELS(825));
  }
  if (GameFlag_IsSet(FLAG_WATER_DROPPED_TWICE) == 0) {
    Actor_SetPosition(ACTOR_ROOM_TWO_GUEST, PIXELS(560), PIXELS(710));
    Object_GetById(ACTOR_ROOM_TWO_GUEST)->facing = FACING_EAST + FACING_STEP;
    Actor_SetPosition(ACTOR_ROOM_TWO_SECOND_GUEST, PIXELS(576), PIXELS(710));
  }
  if (GameFlag_IsSet(FLAG_WATER_DRAINED) == 0) {
    Actor_SetPosition(ACTOR_WEAPON_SELLER, PIXELS(295), PIXELS(744));
    Object_GetById(ACTOR_WEAPON_SELLER)->facing =
        FACING_NORTHWEST + FACING_STEP;
  } else {
    actor = Object_GetById(ACTOR_WEAPON_SELLER);
    actor->collisionFlags |= 4;
  }
  actor = Object_GetById(ACTOR_ARMOR_SELLER);
  if (actor != 0) {
    actor->collisionFlags |= 4;
  }
  actor = Object_GetById(ACTOR_ITEM_SELLER);
  if (actor != 0) {
    actor->collisionFlags |= 4;
  }
}

/// Gives a new spray particle its look: standing still, drawn on sprite
/// priority 1 and at 0.3 scale.
static void Spray_Initialize(struct SprayActor *spray) {
  spray->motionFlags = 0;
  spray->sprite->image.priority = 1;
  ObjectGroup_SetChildValue(spray, 3);
  Engine_ActorSetSpriteFlags((struct FieldActor *)spray, 0);
  spray->scaleX = WATER_SPRAY_START_SCALE;
  spray->scaleY = WATER_SPRAY_START_SCALE;
}

/// Update callback of a spray particle: it drifts sideways with a random
/// wobble, rising and stretching for a few frames and then falling and
/// growing, sometimes changing frame, until its life runs out.
static void Spray_Drift(union FieldObject *fieldObject) {
  struct WaterActor *spray = (struct WaterActor *)fieldObject;
  spray->x += WATER_SPRAY_LATERAL_STEP(
      spray->timer, (s16)((s32)(((u32)Engine_RandomNext() * 2) >> 16) - 1));
  if (spray->timer <= 3) {
    spray->z += -(((u32)Engine_RandomNext() * 0x8000) >> 16) - 0x10000;
    spray->scaleX += 0x2666;
    spray->scaleY += -0xa3d;
  } else {
    spray->z += 0x20000;
    spray->scaleX += 0x7ae;
    spray->scaleY += 0x7ae;
  }
  if (((u32)Engine_RandomNext() * spray->timer) >> 16 == 0)
    ObjectGroup_SetChildValue(spray, 7);
  if (spray->timer != 0)
    spray->timer--;
  else
    spray->timer = (((u32)Engine_RandomNext() * 5) >> 16) * 2 + 2;
  if (--spray->counter == 0) {
    spray->update = 0;
    Engine_ObjectDispatchRelease(spray);
  }
}

/// Scheduler task while the water beast spits: every fourth frame a spray
/// particle starts from its spout, living 20 frames.
static void Beast_SpawnSpray(void) {
  struct WaterActor *spray;
  if ((gFrameCount & 3) == 0) {
    spray = ((struct WaterActor *)Object_Create(SPRITE_SPRAY, PIXELS(64), 0,
                                                PIXELS(400)));
    if (spray != 0) {
      spray->timer = WATER_SPRAY_LIFETIME;
      spray->shakeFrames = 0;
      spray->counter = WATER_SPRAY_LIFETIME;
      Spray_Initialize(spray);
      spray->update = Spray_Drift;
      Object_SetMode(spray, 1);
    }
  }
}

/// Stepping near the water beast: the camera pans to it, it stops spitting,
/// turns to the party, cries out and hops away into the flood.
static void Village_ScareOffBeast(void) {
  Engine_EventBegin();
  Camera_SetSpeed(0x6666, 0xccc);
  Camera_MoveTo(PIXELS(63), -1, PIXELS(450), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_WATER_BEAST, ANIM_STAND);
  // The spitting stops: its sound and its spray end.
  BattleFx_SetQueuedSoundAndPlay(-1);
  Engine_TaskRemoveCallback(Beast_SpawnSpray);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WATER_BEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_NORTH + FACING_STEP, 40);
  Event_PlayCue(SE_147);
  Engine_ActorRunRepeatedMotion(ACTOR_WATER_BEAST, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_WATER_BEAST, FACING_NORTHWEST + FACING_STEP, 40);
  Beast_HopAway();
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimationAndWait(ACTOR_THIRSTY_VILLAGER, ANIM_SHAKE_HEAD);
  GameFlag_Set(FLAG_ALTIN_WATER_BEAST_LEFT);
  Engine_EventEnd();
}

/// The cook from Xian paces between his pots, nodding over each.
s32 gAltinHouses_CookScript[] = {
    SC_ADD(SW_COLLISION, 128),
    SC_SET(SW_SPEED, 0x26666),
    SC_SET(SW_ACCEL, 0x13333),
    SC_LABEL(1),
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(342), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ARG(ANIM_NOD),
    SC_WAIT(30),
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(356), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ARG(ANIM_NOD),
    SC_WAIT(50),
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(342), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ARG(ANIM_NOD),
    SC_WAIT(20),
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(ALTIN_COOK_LAST_POT_X), 0, PIXELS(746)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ARG(ANIM_NOD),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_EAST + FACING_STEP),
    SC_WAIT(90),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gAltinOther_Entrances[] = {
    {0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinVillage_Entrances[] = {
    {0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 488, 0, 184, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 168, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 336, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 424, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 136, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 280, 0, 408, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 456, 0, 224, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 184, 0, 408, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 168, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 24, 0, 456, FACING_SOUTHEAST, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 264, 0, 224, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 328, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 343, 0, 177, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinHouses_Entrances[] = {
    {0, CONDITION_ALWAYS, 200, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 159, 0, 258, FACING_NORTH, 0, 25, 16, 255, 300, 0},
    {2, CONDITION_ALWAYS, 336, 0, 213, FACING_NORTH, 0, 268, 40, 500, 250, 0},
    {3, CONDITION_ALWAYS, 688, 0, 204, FACING_NORTH, 0, 542, 40, 768, 250, 0},
    {4, CONDITION_ALWAYS, 240, 0, 500, FACING_NORTH, 0, 110, 336, 350, 540, 0},
    {5, CONDITION_ALWAYS, 496, 0, 504, FACING_NORTH, 0, 360, 336, 600, 540, 0},
    {6, CONDITION_ALWAYS, 752, 0, 516, FACING_NORTH, 0, 635, 280, 845, 556, 0},
    {7, CONDITION_ALWAYS, 80, 0, 864, FACING_NORTH, 0, 16, 638, 432, 912, 0},
    {8, CONDITION_ALWAYS, 68, 0, 728, FACING_SOUTH, 0, 16, 638, 432, 912, 0},
    {9, CONDITION_ALWAYS, 690, 0, 732, FACING_SOUTH, 0, 480, 626, 896, 882, 0},
    {SCENE_TABLE_END},
};

u32 gAltin_Exits[] = {
    SCENE_EXITS(75),       SCENE_EXIT(1, 2, 16),  SCENE_EXIT(2, 76, 7),
    SCENE_EXIT(3, 9, 29),  SCENE_EXIT(4, 76, 1),  SCENE_EXIT(5, 76, 2),
    SCENE_EXIT(6, 76, 3),  SCENE_EXIT(7, 76, 5),  SCENE_EXIT(8, 76, 6),
    SCENE_EXIT(9, 76, 4),  SCENE_EXIT(10, 77, 1), SCENE_EXIT(11, 80, 1),
    SCENE_EXIT(12, 84, 4), SCENE_EXIT(13, 83, 3), SCENE_EXIT(0, 76, 0),
    SCENE_EXIT(1, 75, 4),  SCENE_EXIT(2, 75, 5),  SCENE_EXIT(3, 75, 6),
    SCENE_EXIT(4, 75, 9),  SCENE_EXIT(5, 75, 7),  SCENE_EXIT(6, 75, 8),
    SCENE_EXIT(7, 75, 2),  SCENE_EXIT(8, 76, 9),  SCENE_EXIT(9, 76, 8),
    SCENE_EXITS_END,
};

struct ScenePlacement gAltinOther_Placements[] = {
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinVillage_Placements[] = {
    {0x72, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(440), 0, PIXELS(200),
     FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0},
    {0x74, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(408), 0, PIXELS(216),
     FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0},
    {0x6a, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(310), 0, PIXELS(217),
     FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0},
    {0x65, CONDITION_ALWAYS, ACTOR_WANDER, PIXELS(246), 0, PIXELS(186),
     FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0},
    {0x67, CONDITION_ALWAYS, 3, PIXELS(58), 0, PIXELS(206),
     FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0},
    {0x73, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(104), 0, PIXELS(286),
     FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0},
    {0x6f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(41), 0, PIXELS(441),
     FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0},
    {0x6c, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(176), 0, PIXELS(296),
     FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0},
    {0x72, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(136), 0, PIXELS(461),
     FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0},
    {0x67, CONDITION_ALWAYS, ACTOR_WANDER, PIXELS(350), 0, PIXELS(286),
     FACING_NORTH, TALK_FACE_PARTY, 0},
    {0xbe, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(60), 0, PIXELS(474),
     FACING_SOUTHEAST, TALK_FACE_PARTY, 0},
    {0xcb, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(344), 0, PIXELS(168),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x7d, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(88), 0, PIXELS(397),
     FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0},
    {0x76, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(422), 0, PIXELS(208),
     FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0},
    {0xdf, CONDITION_ALWAYS, 7, PIXELS(452), 0, PIXELS(216), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0xdf, CONDITION_ALWAYS, 7, PIXELS(332), 0, PIXELS(360), FACING_SOUTH,
     TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {0xdf, CONDITION_ALWAYS, 7, PIXELS(332), 0, PIXELS(396), FACING_SOUTH,
     TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gAltinHouses_Placements[] = {
    {0x6c, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(104), 0, PIXELS(120), FACING_EAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {0x6a, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(105), 0, PIXELS(233), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {0x67, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(456), 0, PIXELS(182), FACING_WEST + FACING_STEP, 3,
        0},
    {0x6f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(376), 0, PIXELS(120),
        FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
    {0x6c, 0x0, ACTOR_STAND, PIXELS(690), 0, PIXELS(128), FACING_SOUTHWEST + FACING_STEP,
        TALK_FACE_PARTY_AND_BACK, 0},
    {0x6a, 0x0, ACTOR_STAND, PIXELS(669), 0, PIXELS(128), FACING_EAST + FACING_STEP,
        TALK_FACE_PARTY_AND_BACK, 0},
    {0x66, 0x0, ACTOR_STAND, PIXELS(604), 0, PIXELS(145), FACING_SOUTHEAST + FACING_STEP, 3, 0},
    {0x7c, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(263), 0, PIXELS(436), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {0x76, 0x0, ACTOR_STAND, PIXELS(520), 0, PIXELS(454), FACING_SOUTH + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {0x7d, 0x0, ACTOR_STAND, PIXELS(743), 0, PIXELS(454), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {0x77, 0x0, ACTOR_STAND, PIXELS(103), 0, PIXELS(744), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {0x80, 0x0, 3, PIXELS(228), 0, PIXELS(756), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
    {0x7b, 0x0, (s32)gAltinHouses_CookScript, PIXELS(350), 0, PIXELS(746),
        FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0},
    {0x6c, 0x0, ACTOR_STAND, PIXELS(646), 0, PIXELS(734), FACING_SOUTHEAST + FACING_STEP,
        TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

struct SceneEvent gAltinOther_Events[] = {
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinVillage_Events[] = {
    {TOUCH_FACING(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Village_WalkThroughDoorway)},
    {TOUCH_FACING(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Village_WalkThroughDoorway)},
    {TOUCH_FACING(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Village_WalkThroughDoorway)},
    {EVENT_EXIT, 13, CONDITION_ALWAYS, 13},
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS,
        EVENT_SCRIPT(Village_EnterDoor)},
    {EVENT_TALK, ACTOR_GREETING_VILLAGER, FLAG_HYDROS_STATUES_DEFEATED, EVENT_SCRIPT(Village_AskAboutFirstVisit)},
    {EVENT_TALK, ACTOR_GREETING_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgAltinThankYouSoMuchForDefeating)},
    {EVENT_TALK, ACTOR_CURIOUS_VILLAGER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinHowCanThoseCreaturesSpitUp)},
    {EVENT_TALK, ACTOR_CURIOUS_VILLAGER, CONDITION_ALWAYS, EVENT_SCRIPT(Village_AskAboutHydros)},
    {EVENT_TALK, 10, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinHowHorribleMyHouseIsTotally)},
    {EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAltinOnceTheMineOpensAgainWell)},
    {EVENT_TALK, 11, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinEvenTheMineIsSubmergedNo)},
    {EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgAltinTheyAlreadyStartedMiningTheUpper)},
    {EVENT_TALK, 12, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIfWeCouldJustDestroyThe)},
    {EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAltinTheWaterDrainedOutOfThe)},
    {EVENT_TALK, 13, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAltinAlwaysUsedToRunShort)},
    {EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAltinThankGoodnessTheVillageIsFree)},
    {EVENT_TALK, ACTOR_THIRSTY_VILLAGER, CONDITION_ALWAYS, EVENT_SCRIPT(Village_TalkToThirstyVillager)},
    {EVENT_TALK, ACTOR_RETURNING_VILLAGER_A, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWatersAroundHereHaveSubsided)},
    {EVENT_TALK, ACTOR_RETURNING_VILLAGER_A, CONDITION_ALWAYS, MSGNO(MsgAltinTheVillageWentRightBackTo)},
    {EVENT_TALK, ACTOR_RETURNING_VILLAGER_B, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWaterBeastsLookedExactlyLike)},
    {EVENT_TALK, ACTOR_RETURNING_VILLAGER_B, CONDITION_ALWAYS, MSGNO(MsgAltinSoThoseHydrosCreaturesReallyWere)},
    {EVENT_TALK, ACTOR_RETURNING_VILLAGER_C, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheWatersHaveFlushedOutTons)},
    {EVENT_TALK, ACTOR_RETURNING_VILLAGER_C, CONDITION_ALWAYS, EVENT_SCRIPT(Village_AskAboutRuins)},
    {EVENT_TALK, ACTOR_FLOODED_ARMOR_SELLER, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToArmorSeller)},
    {EVENT_TALK, ACTOR_FLOODED_ITEM_SELLER, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToItemSeller)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_VILLAGER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinYouCameToAltinAtA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_VILLAGER, CONDITION_ALWAYS,
        MSGNO(MsgAltinWeCanGoBackToWork)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CURIOUS_VILLAGER, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinItsAlmostLikeTheyreTurningThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CURIOUS_VILLAGER, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWasAfraidMoreWaterWould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinImGladISavedMyVery)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
        MSGNO(MsgAltinIHopeAltinMineWillThrive)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinEvenTheToughestOfMinersCant)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheWaterSeepedIntoTheMine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinIfTheWaterBeastsContinueTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
        MSGNO(MsgAltinItsSoHumidInsideThatCave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinWaterIsNiceButNotWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
        MSGNO(MsgAltinThatStrangeChamberInTheMines)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRSTY_VILLAGER, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinUghTheMonstersSpewedOutAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRSTY_VILLAGER, CONDITION_ALWAYS,
        MSGNO(MsgAltinYouFoughtPrettyWellDespiteAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RETURNING_VILLAGER_A, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAsTheMonstersGoAwayThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RETURNING_VILLAGER_A, CONDITION_ALWAYS,
        MSGNO(MsgAltinThatMineIsFullOfHoles)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RETURNING_VILLAGER_B, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinComeToThinkOfItIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RETURNING_VILLAGER_B, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheOldGuardianStatuesAlwaysCreeped)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RETURNING_VILLAGER_C, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinItllBeBetterThanDiggingUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RETURNING_VILLAGER_C, CONDITION_ALWAYS,
        MSGNO(MsgAltinIUsedToFindAllSorts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FLOODED_ARMOR_SELLER, CONDITION_ALWAYS,
        MSGNO(MsgAltinAllMyArmorInventoryIsRusting)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FLOODED_ITEM_SELLER, CONDITION_ALWAYS,
        MSGNO(MsgAltinIfMyGoodsAreWetIll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MOVABLE_BLOCK, CONDITION_ALWAYS, EVENT_SCRIPT(Village_CheckBlockMoved)},
    {EVENT_TOUCH, TRIGGER_NEAR_BEAST, FLAG_ALTIN_WATER_BEAST_LEFT, EVENT_SCRIPT(Village_ScareOffBeast)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinHouses_Events[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
    {EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
    {EVENT_EXIT, 8, CONDITION_ALWAYS, 8},
    {EVENT_EXIT, 9, CONDITION_ALWAYS, 9},
    {EVENT_TALK, ACTOR_ROOM_ONE_GUEST, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinNoMatterWhatHappensIdNever)},
    {EVENT_TALK, ACTOR_ROOM_ONE_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOhNoTheWholeHouseIs)},
    {EVENT_TALK, ACTOR_ROOM_ONE_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinIHearThatSilkRoadIs)},
    {EVENT_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinOfCourseOurHouseWillBe)},
    {EVENT_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIfTheMonsterThatDidThis)},
    {EVENT_TALK, ACTOR_ROOM_ONE_SECOND_GUEST, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_AskAboutXianGirl)},
    {EVENT_TALK, ACTOR_ROOM_TWO_GUEST, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinNotManyPeopleComeUpSilk)},
    {EVENT_TALK, ACTOR_ROOM_TWO_GUEST, FLAG_HYDROS_STATUES_DEFEATED, EVENT_SCRIPT(Houses_AskAboutWayWest)},
    {EVENT_TALK, ACTOR_ROOM_TWO_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinAncientRuinsAndTreasureDefinitelyGo)},
    {EVENT_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinItsBeenALongTimeSince)},
    {EVENT_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinDontBeTooHarshOnThose)},
    {EVENT_TALK, ACTOR_ROOM_TWO_SECOND_GUEST, CONDITION_ALWAYS, MSGNO(MsgAltinIfOnlyTheyHadLeftA)},
    {EVENT_TALK, 12, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIDidntWantToLeaveMy)},
    {EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAltinReallyAncientRuinsAtTheBottom)},
    {EVENT_TALK, 13, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIHeardSomeoneKilledTheWater)},
    {EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAltinThereProbablyIsntMuchTreasureIn)},
    {EVENT_TALK, ACTOR_KISSING_VILLAGER, FLAG_HYDROS_STATUES_DEFEATED, EVENT_SCRIPT(Houses_AskAboutMonsters)},
    {EVENT_TALK, ACTOR_KISSING_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgAltinSomethingsWrongWithMomAndPop)},
    {EVENT_TALK, ACTOR_WEAPON_SELLER, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_TalkToWeaponSeller)},
    {EVENT_TALK, ACTOR_ARMOR_SELLER, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToArmorSeller)},
    {EVENT_TALK, ACTOR_ITEM_SELLER, CONDITION_ALWAYS, EVENT_SCRIPT(Altin_TalkToItemSeller)},
    {EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_TalkToInnkeeper)},
    {EVENT_TALK, 19, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIdGiveAnythingForSomeoneTo)},
    {EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAltinThereIsAnEndlessDesertWest)},
    {EVENT_TALK, ACTOR_COOK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinICameFromXianICook)},
    {EVENT_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgAltinWestOfTheMountainsIsVery)},
    {EVENT_TALK, ACTOR_MINE_WORKER, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinNoneOfUsCanGoHome)},
    {EVENT_TALK, ACTOR_MINE_WORKER, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinTheGuestsInRoomOneMoved)},
    {EVENT_TALK, ACTOR_MINE_WORKER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheGuestsInRoomTwoAlso)},
    {EVENT_TALK, ACTOR_MINE_WORKER, CONDITION_ALWAYS, EVENT_SCRIPT(Houses_AskAboutMineBeasts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, FLAG_WATER_DROPPED_ONCE,
        MSGNO(MsgAltinLeavingAltinForTolbiWellThats)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinItsGoingToTakeALot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheMerchantsMustBeHavingA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinNowIFinallyHaveAnExcuse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOneBlowsNotEnoughJabJab)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_ONE_SECOND_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWonderWhatBusinessThatGirl)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, FLAG_WATER_DROPPED_TWICE, MSGNO(MsgAltinOfCourseYouCantGoWest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinIThoughtTheVillageWasDone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheySayYouCanStillFind)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_WATER_DROPPED_TWICE,
        MSGNO(MsgAltinIHateSittingAroundLikeThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_SECOND_GUEST, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinImJustGladToGetMy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOM_TWO_SECOND_GUEST, CONDITION_ALWAYS,
        MSGNO(MsgAltinHowCanITrulyBeHappy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinTheBottomOfTheMineThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
        MSGNO(MsgAltinCripesAncientRuinsDownInThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinMyHusbandsSenseForFindingTreasure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
        MSGNO(MsgAltinIHadNoIdeaThereWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KISSING_VILLAGER, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinMomAndPopKnowSomethingAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KISSING_VILLAGER, CONDITION_ALWAYS,
        MSGNO(MsgAltinIWonderIfMomAndPop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_SELLER, FLAG_WATER_DRAINED, MSGNO(MsgAltinCripesIsThisHowMyWeapon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_SELLER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinOnlyAMerchantKnowsTheJoy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_SELLER, CONDITION_ALWAYS,
        MSGNO(MsgAltinIveHeardOfAPassageIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_SELLER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinWeDriedOffAllThatArmor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_SELLER, CONDITION_ALWAYS,
        MSGNO(MsgAltinWellIfYouAreImpressedThen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SELLER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinAFewThingsGotDampBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_SELLER, CONDITION_ALWAYS,
        MSGNO(MsgAltinTheGirlFromXianBoughtAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_HYDROS_STATUES_DEFEATED,
        MSGNO(MsgAltinWellComeThroughThisAllRight)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
        MSGNO(MsgAltinIThinkThatTempleWasCalled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIKnowTheyDontLookSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
        MSGNO(MsgAltinYouWontBeAbleToGo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinIBecameACookAfterI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS,
        MSGNO(MsgAltinNoOneCanGoWestOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINE_WORKER, FLAG_WATER_DROPPED_ONCE, MSGNO(MsgAltinMostOfTheVillageIsStill)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINE_WORKER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgAltinItLooksLikeTheWaterHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINE_WORKER, CONDITION_ALWAYS,
        MSGNO(MsgAltinItWillTakeSomeTimeBefore)},
    {SEARCH(SEARCH_BARREL), 100, FLAG_ALTIN_NUT, FIND_ITEM(ITEM_NUT)},
    {SEARCH(SEARCH_BARREL), 101, FLAG_ALTIN_COINS, FIND_COINS(9)},
    {SEARCH(SEARCH_BED), 102, FLAG_ALTIN_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB)},
    {SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinThePotsAndPansAreRusted))},
    {SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingRoastBeefEven))},
    {SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinHeIsMakingShrimpInA))},
    {SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS,
        FIND_MESSAGE(MSGNO(MsgAltinThereIsABookTitledAltin))},
    {SCENE_EVENTS_END},
};
// clang-format on

/// The opening pictures of the village's doors, two cells square. The mine's
/// door (the second) is wider and redrawn by hand instead.
u16 gAltinVillage_MineDoorCells[] = {
    MAP_CELL_STEP(42, 33, 2, 2, 0),
    MAP_CELL_STEP(42, 35, 2, 2, 5),
    MAP_CELL_STEP(40, 33, 2, 2, 0),
    MAP_CELL_STEP(40, 35, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_DoorCellsA[] = {
    MAP_CELL_STEP(35, 48, 2, 2, 5),
    MAP_CELL_STEP(33, 48, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_DoorCellsF[] = {
    MAP_CELL_STEP(35, 35, 2, 2, 5),
    MAP_CELL_STEP(33, 35, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_DoorCellsG[] = {
    MAP_CELL_STEP(35, 46, 2, 2, 5),
    MAP_CELL_STEP(33, 46, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_DoorCellsC[] = {
    MAP_CELL_STEP(35, 50, 2, 2, 5),
    MAP_CELL_STEP(33, 50, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_DoorCellsD[] = {
    MAP_CELL_STEP(35, 33, 2, 2, 5),
    MAP_CELL_STEP(33, 33, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gAltinVillage_DoorCellsE[] = {
    MAP_CELL_STEP(35, 52, 2, 2, 5),
    MAP_CELL_STEP(33, 52, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

/// Each door trigger's opening picture and cell, from trigger 2.
struct DoorAnim gAltinVillage_Doors[] = {
    {gAltinVillage_DoorCellsA, 41, 5},  {gAltinVillage_MineDoorCells, 51, 6},
    {gAltinVillage_DoorCellsC, 57, 17}, {gAltinVillage_DoorCellsD, 39, 17},
    {gAltinVillage_DoorCellsE, 48, 25}, {gAltinVillage_DoorCellsF, 59, 9},
    {gAltinVillage_DoorCellsG, 42, 23}, {gAltinVillage_DoorCellsD, 41, 27},
};
