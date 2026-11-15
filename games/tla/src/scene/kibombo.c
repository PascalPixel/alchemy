/// @file
/// Kibombo, the warriors' village below the Gabomba Statue, in the six
/// rooms of this overlay: the village, its shops, inn and Sanctum
/// (Kibombo_OpenWeaponShop, Kibombo_OpenInn), and the grounds before the
/// statue. On the sacred night Akafubu offers the Black Orb to the Great
/// Gabomba (Kibombo_CeremonyBegins to Kibombo_CeremonyEnds), where Piers, come
/// to take his orb back, joins the party (Kibombo_PiersThanksParty); the way to
/// the statue past the dead end is a vent Scoop uncovers
/// (Kibombo_DeadEndByTheStatue). The villagers' talk changes once Akafubu is
/// the witch doctor (Kibombo_TalkPathToPeace).

#include "system/task.h"
#include "flags.h"
/// Set while the charges stick: the music rules play silence in Kibombo and the
/// Gabomba Statue (sounds.h).
#define FLAG_GABOMBA_MUSIC_SILENCED 0x8f7
/// Set as the scene leaves the ceremony below for the statue's inside; the
/// arrows room clears it.
#define FLAG_GABOMBA_BACK_FROM_CEREMONY 0x8fc
/// Set once the party has met Piers at Kibombo (Kibombo_MeetPiers).
#define FLAG_KIBOMBO_PIERS_MET 0x8f3
#include "item_ids.h"
#include "field/sprite.h"
#include "types.h"
#include "field/field_sequence_event.h"
#include "gamepak/far_call/system.h"
#include "field/event_commands.h"
#include "scene/kibombo.h"
#include "graphics/resource.h"
#include "battle/battle.h"
#include "field/field.h"
#include "field/ground_mask.h"
#include "field/field_psynergy_work.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "system/io_reg.h"
#include "scene/scene.h"
#include "field/field_effect.h"
#include "field/field_sprite.h"
#include "system/vector3.h"
#include "system/input.h"
#include "version.h"
#include "fixed_math.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "system/frame.h"

void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_SetSpriteColor(struct Actor *object, s32 value);
void Actor_FaceActorById(s32 actor, s32 target, s32 frames);
void Actor_FaceDirectionById(s32 actor, s32 angle, s32 frames);

void MapGrid_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Event_ShowMessageThenWait(s32 actor, s32 a, s32 b);
void Event_ShowMessageAndWait(s32 speaker, s32 flags);
void Actor_FaceDirectionAndRefreshById(s32 actor, s32 angle);
void Actor_DisableById(s32 actor);
void FlagBlock_ApplyFlags(const void *script);
void Shop_Run(s32 shop, s32 keeper);
void Sanctum_Run(s32 keeper);
void Inn_CheckIn(s32 shop, s32 keeper);
void Sound_Play(s32 cue);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Actor_Release(struct Actor *object);
void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void Actor_SetAnimationById(u32 actorId, s32 animation);
void FieldPsynergy_RunHandler(s32 type);
void FieldCamera_WaitForArrival(void);
void Import_EventSetExitRequest(s32 countdown);
void FieldPsynergy_LoadResources(s32 a, s32 b);
void BattleEffect_StopMainActor(void);

void Field_TryJumpForward(void);
void FieldEffect_StartBlankOverlay(s32 a);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void FlagBlock_PlaceAll(const void *script);
void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_WaitForMove(struct Actor *object);

void FieldCamera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct Actor *FieldCamera_GetActor(void);
void Camera_SetSpeedParameters(s32 speed, s32 acceleration);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 actor, s32 x, s32 z);
void Actor_MoveToObject(s32 actor, s32 target);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Actor_SetMovementTypeAndWaitById(s32 actor, const void *callback);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void FieldEffect_SetAttached(s32 actor, s32 emote);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Actor_SetAnimationAndWait(s32 actor, s32 mode);
void Actor_WaitForMoveById(s32 actor);
void Event_AddPartyMember(s32 actor, s32 value);

void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actor, s32 x, s32 z,
                                                    s32 angle);

void Actor_WalkToById(s32 actor, s32 x, s32 z);

void PaletteFade_SetTarget(s32 a, s32 b);

/// Adds a task, discarding the slot Task_Add returns: called directly, the
/// s32 Task_Add moves two instructions of each caller.
static inline void Kibombo_TaskAdd(void (*func)(void), s32 priority) {
  Task_Add(func, priority);
}

// ---- this overlay

void Kibombo_InitializeVillage(void);
s32 Kibombo_ClampColor(s32 value);
void Kibombo_FlashSprite(struct Actor *object);
void Kibombo_DigFirstMarking(void);
void Kibombo_DigSecondMarking(void);
void Kibombo_DigThirdMarking(void);
void Kibombo_DigFourthMarking(void);
void Kibombo_PiersHasForgottenParty(void);
void Kibombo_PiersTakesPartyForEnemies(void);
void Kibombo_AkafubuOffersCrystal(void);
void Kibombo_PiersJoinsParty(void);
void Kibombo_FlickerColor(s32 index, s32 entry, s32 delta);
void Kibombo_FlickerRedderColor(s32 index, s32 entry, s32 delta);
void Kibombo_FlickerVillageFires(void);
void Kibombo_FlickerCeremonyFires(void);
void Kibombo_FlickerStatueFires(void);
void Kibombo_RaiseEmbers(void);
void Kibombo_PuffSmoke(void);
void Kibombo_LightFlameRow(void);
void Actor_Hide(s32 id);
void Scene_RunUnlessFlag(void);
void Kibombo_AkafubuChants(void);
void Kibombo_SetStatueMouth(s32 open, s32 hide);
void Kibombo_SetStatueTongue(s32 open, s32 mode);
void Kibombo_StopFireFlicker(void);
void Kibombo_DropIntoHole(s32 x, s32 z);
void Kibombo_OpenCeremonyScene(void);
void Kibombo_CrowdCheers(void);

extern s32 gKibombo_EnterDoorScript[];
extern u32 gKibombo_KradenMeetsPiersScript[];
extern u32 gKibombo_JennaMeetsPiersScript[];
extern s32 gKibombo_ShebaMeetsPiersScript[];
extern u32 gKibombo_PartyGathersScript[];
extern s32 gKibombo_JawsSettleScript[];
extern s32 gKibombo_JawsOpenScript[];
extern s16 gKibombo_FrozenPuddle[];
extern s16 gKibombo_FireColors[];
extern struct Actor *gKibombo_BlackCrystal;

/// Set once the party has found the dead end by the statue and turned back
/// (Kibombo_DeadEndByTheStatue).
#define FLAG_KIBOMBO_DEAD_END_SEEN 0x8f5
/// Set while Piers goes about Kibombo with the party: as he joins it, and as he
/// thanks it; cleared when he stays behind.
#define FLAG_KIBOMBO_PIERS_WITH_PARTY 0x92e
/// Set once Piers has refused to leave the village without the orb and stayed
/// behind (Kibombo_PiersRefusesToLeave).
#define FLAG_KIBOMBO_PIERS_STAYED_BEHIND 0x92f

/// Story flags: the vent Scoop uncovers by the statue, the block Moved for
/// Piers, the ceremony begun and Piers joined.
#define FLAG_KIBOMBO_VENT_UNCOVERED 0x86a
#define FLAG_KIBOMBO_BLOCK_MOVED_FOR_PIERS 0x9ea
#define FLAG_KIBOMBO_CEREMONY_BEGUN 0x8f1
#define FLAG_PIERS_JOINED 0x8f4
/// Set once the party has heard of the foreign ceremony.
#define FLAG_HEARD_OF_THE_FOREIGN_CEREMONY 0x9df
/// For this visit: each of the four markings dug, and the one Scoop is digging.
#define FLAG_FIRST_MARKING_DUG FLAG_SCENE_TEMP_04
#define FLAG_SECOND_MARKING_DUG FLAG_SCENE_TEMP_05
#define FLAG_THIRD_MARKING_DUG FLAG_SCENE_TEMP_06
#define FLAG_FOURTH_MARKING_DUG FLAG_SCENE_TEMP_07
#define FLAG_SCOOPING_FIRST_MARKING FLAG_SCENE_TEMP_20
#define FLAG_SCOOPING_SECOND_MARKING FLAG_SCENE_TEMP_21
#define FLAG_SCOOPING_THIRD_MARKING FLAG_SCENE_TEMP_22
#define FLAG_SCOOPING_FOURTH_MARKING FLAG_SCENE_TEMP_23

/// The grounds before the statue (SceneId_KibomboStatueGrounds and, on the
/// ceremony night, SceneId_KibomboStatueGroundsCeremony), numbering the rows of
/// their placements; the night's rows place the warriors and Akafubu's party
/// after them, and Kraden after those.
enum KibomboStatueGroundsActorId {
  ACTOR_GROUNDS_PUDDLE = 8,
  /// The stone pillar Move pushes for Piers (Kibombo_MoveBlockForPiers).
  ACTOR_GROUNDS_PILLAR = 11,
  /// The two halves of the Great Gabomba's mouth, which open to take the orb
  /// (Kibombo_SetStatueMouth), and its tongue (Kibombo_SetStatueTongue).
  ACTOR_STATUE_JAW_A = 12,
  ACTOR_STATUE_JAW_B = 13,
  ACTOR_STATUE_TONGUE = 14,
  ACTOR_GROUNDS_SHADE_A = 15, ///< black squares (spr_337)
  ACTOR_GROUNDS_SHADE_B = 16,
  /// On the ceremony night: Akafubu, "Let us now present our jewel to the
  /// Great Gabomba!", and the eight warriors in a ring before him.
  ACTOR_AKAFUBU = 17,
  ACTOR_WARRIOR_A = 18,
  ACTOR_WARRIOR_B = 19,
  ACTOR_WARRIOR_C = 20,
  ACTOR_WARRIOR_D = 21,
  ACTOR_WARRIOR_E = 22,
  ACTOR_WARRIOR_F = 23,
  ACTOR_WARRIOR_G = 24,
  ACTOR_WARRIOR_H = 25,
  ACTOR_KRADEN = 26,
  /// An unseen point the Black Crystal rides on as Akafubu raises it
  /// (Kibombo_CarryBlackCrystal); Piers's, Jenna's and Sheba's rows take
  /// their own numbers, so it follows Kraden.
  ACTOR_CRYSTAL_CARRIER = 27
};

/// The village by day and on the ceremony night: the frozen puddle, the two
/// candlesticks by the statue path, the flame, and the rope's two posts. By day
/// two warriors keep the gate and the signal fire smokes; at night a shade
/// covers the doorway the leader walks behind, and six candlesticks burn.
enum {
  ACTOR_VILLAGE_PUDDLE = 8,
  ACTOR_CANDLESTICK_A,
  ACTOR_CANDLESTICK_B,
  ACTOR_FLAME,
  ACTOR_ROPE_POST_A,
  ACTOR_ROPE_POST_B,
  ACTOR_GATE_GUARD_A = 22,
  ACTOR_GATE_GUARD_B,
  ACTOR_SIGNAL_FIRE = 25,
  ACTOR_DOORWAY_SHADE = 29,
  ACTOR_LIT_CANDLESTICK_A,
  ACTOR_LIT_CANDLESTICK_B,
  ACTOR_LIT_CANDLESTICK_C,
  ACTOR_LIT_CANDLESTICK_D,
  ACTOR_LIT_CANDLESTICK_E,
  ACTOR_LIT_CANDLESTICK_F
};

/// The houses, inn and shops (SceneId_KibomboHouses).
enum {
  /// Akafubu at home after the ceremony: "I'm going to be the one to get the
  /// Great Gabomba's magic! You'll see!"
  ACTOR_AKAFUBU_AT_HOME = 8,
  /// "My son has been so kind and compassionate since the ceremony."
  ACTOR_AKAFUBUS_FATHER = 9,
  ACTOR_KRADEN_AT_HOME = 10,
  ACTOR_MAN_WITH_ACHING_BACK = 16,
  ACTOR_WEAPON_SHOPKEEPER = 20,
  ACTOR_ARMOR_SHOPKEEPER = 21,
  ACTOR_ITEM_SHOPKEEPER = 22,
  ACTOR_INNKEEPER = 23,
  ACTOR_HEALER = 25
};

/// The village by day: "This is a signal fire. We use these to send messages
/// to neighboring..."
enum { ACTOR_SIGNAL_FIRE_KEEPER = 24 };

/// The village on the ceremony night: the healer who talks of the foreign
/// ceremony.
enum { ACTOR_HEALER_ON_THE_CEREMONY = 36 };

struct Actor;

s32 Kibombo_SpinSprite(struct Actor *object) {
  ((struct FieldSprite *)object->sprite)->rotation += 0x600;
  return 0;
}

s32 Kibombo_KeepShakeCentre(struct Actor *object) {
  object->bounce = object->x.fixed;
  object->gravity = object->y.fixed;
  return 0;
}

extern s16 gKibombo_HousesExitRegions[];

s32 Kibombo_ShakeInPlace(struct Actor *object) {
  if (gFrameCount & 1) {
    s32 dx;
    s32 dy;

    dx = -0x10000;
    dy = dx;
    dx += px(Random_Below(3));
    dy += px(Random_Below(3));

    Actor_SetPositionAndResetMotion(object, object->bounce + dx,
                                    object->gravity + dy, object->z.fixed);
  }
  return 1;
}

void Kibombo_FlashSprite(struct Actor *object) {
  if (gFrameCount & 2)
    Actor_SetSpriteColor(object, 10);
  else
    Actor_SetSpriteColor(object, 0);
}

s32 Kibombo_TurnRandomly(struct Actor *object) {
  if (object->riseCounter != 0) {
    object->riseCounter--;
  } else {
    object->riseCounter = (Random_Below(10)) + 10;
    object->facing = Random16();
  }
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

u8 *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO_HOUSES)
    return gKibombo_HousesExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gKibombo_VillageActorPlacements[];
extern struct ActorPlacement gKibombo_CeremonyActorPlacements[];
extern struct ActorPlacement gKibombo_StatueGroundsActorPlacements[];
extern struct ActorPlacement gKibombo_StatueGroundsCeremonyActorPlacements[];
extern struct ActorPlacement gKibombo_HousesActorPlacements[];
extern struct ActorPlacement gKibombo_ActorPlacements[];
extern struct Trigger gKibombo_VillageTriggers[];
extern struct Trigger gKibombo_CeremonyTriggers[];
extern struct Trigger gKibombo_StatueGroundsTriggers[];
extern struct Trigger gKibombo_StatueGroundsCeremonyTriggers[];
extern struct Trigger gKibombo_HousesTriggers[];
extern struct Trigger gKibombo_Triggers[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO)
    return gKibombo_VillageActorPlacements;
  if (scene == SCENE_KIBOMBO_CEREMONY)
    return gKibombo_CeremonyActorPlacements;
  if (scene == SCENE_KIBOMBO_STATUE_GROUNDS)
    return gKibombo_StatueGroundsActorPlacements;
  if (scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
    return gKibombo_StatueGroundsCeremonyActorPlacements;
  if (scene == SCENE_KIBOMBO_HOUSES) {
    if (GameFlag_Get(0x8ee))
      gKibombo_HousesActorPlacements[0].talkFacing = 0;
    return gKibombo_HousesActorPlacements;
  }
  return gKibombo_ActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO)
    return gKibombo_VillageTriggers;
  if (scene == SCENE_KIBOMBO_CEREMONY)
    return gKibombo_CeremonyTriggers;
  if (scene == SCENE_KIBOMBO_STATUE_GROUNDS)
    return gKibombo_StatueGroundsTriggers;
  if (scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
    return gKibombo_StatueGroundsCeremonyTriggers;
  if (scene == SCENE_KIBOMBO_HOUSES)
    return gKibombo_HousesTriggers;
  return gKibombo_Triggers;
}

void Kibombo_AskAboutFewerWarriors(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgItLooksLikeThere);
  // "It looks like there are fewer warriors in Kibombo now, huh?"
  Event_AskYesNoAndShowResponse(15, 0);
  Event_End();
}

void Kibombo_AskAboutRevenge(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYouDidnT);
  // "You didn't come to Kibombo to get revenge on us for that raid, did you?"
  Event_AskYesNoAndShowResponse(23, 0);
  Event_End();
}

void Kibombo_TalkPathToPeace(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNowThatAkafubu);
  // "Now that Akafubu is our new witch doctor, we shall embark on a path..."
  Event_ShowMessageAndWait(ACTOR_SIGNAL_FIRE_KEEPER, 0);
  Actor_FaceActorById(ACTOR_SIGNAL_FIRE_KEEPER, gMainRoutineWork.leaderActorId,
                      0);
  Event_Wait(20);
  // "This is a signal fire. We use these to send messages to neighboring..."
  Event_ShowMessageAndWait(ACTOR_SIGNAL_FIRE_KEEPER, 0);
  Actor_FaceDirectionAndWait(ACTOR_SIGNAL_FIRE_KEEPER, FACING_SOUTH_SOUTHEAST);
  Event_End();
}

void Kibombo_AskGoingWest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAreYouGoing);
  // "Are you going to head to western Gondowan?"
  Event_AskYesNoAndShowResponse(12, 0);
  Event_End();
}

#include "shared/leader_is_facing_north.h"

/// Kibombo's shops, as Shop_Run numbers them.
enum {
  SHOP_KIBOMBO_WEAPONS = 17,
  SHOP_KIBOMBO_ARMOR = 18,
  SHOP_KIBOMBO_ITEMS = 19
};

/// Kibombo's inn, as Inn_CheckIn numbers the inns.
enum { KIBOMBO_INN = 6 };

void Kibombo_OpenWeaponShop(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_KIBOMBO_WEAPONS, ACTOR_WEAPON_SHOPKEEPER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgTheWeaponsI);
    // "The weapons I sell in this shop are ones we do not use in Kibombo...."
    Event_ShowMessageAndWait(ACTOR_WEAPON_SHOPKEEPER, 0);
    Event_End();
  }
}

void Kibombo_OpenArmorShop(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_KIBOMBO_ARMOR, ACTOR_ARMOR_SHOPKEEPER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgTheOnlyDefenses);
    // "The only defenses we Kibombo need are our shields and our war paint...."
    Event_ShowMessageAndWait(ACTOR_ARMOR_SHOPKEEPER, 0);
    Event_End();
  }
}

void Kibombo_OpenItemShop(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(SHOP_KIBOMBO_ITEMS, ACTOR_ITEM_SHOPKEEPER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgIMSorryBut);
    // "I'm sorry, but the Kibombo spearhead poison is a tribal secret, used..."
    Event_ShowMessageAndWait(ACTOR_ITEM_SHOPKEEPER, 0);
    Event_End();
  }
}

void Kibombo_OpenInn(void) {
  if (Leader_IsFacingNorth()) {
    Inn_CheckIn(KIBOMBO_INN, ACTOR_INNKEEPER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDonTWorryAbout);
    // "Don't worry about the beds here... We used to have insect problems..."
    Event_ShowMessageAndWait(ACTOR_INNKEEPER, 0);
    Event_End();
  }
}

void Kibombo_OpenSanctum(void) {
  if (Leader_IsFacingNorth()) {
    Sanctum_Run(ACTOR_HEALER);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgKibomboSPeace);
    // "Kibombo's peace is restored now that the ceremony has reached its..."
    Event_ShowMessageAndWait(ACTOR_HEALER, 0);
    Event_End();
  }
}

void Kibombo_TalkForeignCeremony(void) {
  if (Event_GetFlag(0x9df)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_FaceActorById(ACTOR_HEALER_ON_THE_CEREMONY,
                        gMainRoutineWork.leaderActorId, 0);
    Event_Wait(10);
    Sanctum_Run(ACTOR_HEALER_ON_THE_CEREMONY);
    Actor_FaceDirection(ACTOR_HEALER_ON_THE_CEREMONY, FACING_NORTH_NORTHWEST,
                        0);
    Event_End();
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgOhOhMy);
    // "Oh... Oh my... I've heard of ceremonies like these... Foreign..."
    Event_ShowMessageThenWait(ACTOR_HEALER_ON_THE_CEREMONY, 0, 40);
    // "Hmm?"
    Event_ShowMessageAndWait(ACTOR_HEALER_ON_THE_CEREMONY, 0);
    Actor_FaceActorById(ACTOR_HEALER_ON_THE_CEREMONY,
                        gMainRoutineWork.leaderActorId, 0);
    Event_Wait(10);
    Sanctum_Run(ACTOR_HEALER_ON_THE_CEREMONY);
    Actor_FaceDirection(ACTOR_HEALER_ON_THE_CEREMONY, FACING_NORTH_NORTHWEST,
                        0);
    GameFlag_Set(FLAG_HEARD_OF_THE_FOREIGN_CEREMONY);
    Event_End();
  }
}

void Kibombo_TalkAchingBack(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgOhMyBack);
  // "Oh, my back... It's been aching ever since that ceremony. I guess I..."
  Event_ShowMessageAndWait(ACTOR_MAN_WITH_ACHING_BACK, 0);
  Event_End();
}

void Kibombo_ReadMindNeverAgain(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgISwearThat);
  // "I swear, that's the last time I'll ever let loose like that again!"
  Event_ShowMessageAndWait(ACTOR_MAN_WITH_ACHING_BACK, 0);
  Event_End();
}

void Kibombo_SetActorsHidden(s32 show) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO_STATUE_GROUNDS ||
      scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
    Actors_SetHidden(show, 15, 2);
  else if (scene == SCENE_KIBOMBO_HOUSES)
    Actors_SetHidden(show, 26, 2);
}

void Kibombo_HideActors(void) {
  Kibombo_SetActorsHidden(1);
  GameFlag_Set(FLAG_SCENE_TEMP_00);
}

void Kibombo_ShowActors(void) {
  Kibombo_SetActorsHidden(0);
  GameFlag_Clear(FLAG_SCENE_TEMP_00);
}

void Actors_SetHidden(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct Actor *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~0x10;
    else
      actor->priorityFlags |= ACTOR_PRIORITY_NOT_DRAWN;
  }
}

/// Frost on the puddle freezes it into a pillar.
void Kibombo_FreezePuddle(void) { FlagBlock_ApplyFlags(gKibombo_FrozenPuddle); }

void Kibombo_UncoverVent(void) {
  MapGrid_CopyCellPictureIndicesRect(34, 33, 23, 18, 1, 1);
  MapGrid_CopyCellAttributes(34, 33, 1, 1, 23, 18);
  GameFlag_Set(FLAG_KIBOMBO_VENT_UNCOVERED);
}

void Kibombo_ScoopVent(s32 mode) {
  if (mode == 1)
    Kibombo_UncoverVent();
}

/// Digs up the first of the four markings by the statue, as Scoop does.
void Kibombo_DigFirstMarking(void) {
  MapGrid_CopyCellPictureIndicesRect(36, 34, 16, 7, 1, 1);
  MapGrid_CopyCellAttributes(36, 34, 1, 1, 16, 7);
  GameFlag_Set(FLAG_FIRST_MARKING_DUG);
}

void Kibombo_ScoopFirstMarking(s32 mode) {
  if (mode == 1)
    Kibombo_DigFirstMarking();
}

void Kibombo_DigSecondMarking(void) {
  MapGrid_CopyCellPictureIndicesRect(38, 34, 16, 11, 1, 1);
  MapGrid_CopyCellAttributes(38, 34, 1, 1, 16, 11);
  GameFlag_Set(FLAG_SECOND_MARKING_DUG);
}

void Kibombo_ScoopSecondMarking(s32 mode) {
  if (mode == 1)
    Kibombo_DigSecondMarking();
}

void Kibombo_DigThirdMarking(void) {
  MapGrid_CopyCellPictureIndicesRect(36, 36, 14, 9, 1, 1);
  MapGrid_CopyCellAttributes(36, 36, 1, 1, 14, 9);
  GameFlag_Set(FLAG_THIRD_MARKING_DUG);
}

void Kibombo_ScoopThirdMarking(s32 mode) {
  if (mode == 1)
    Kibombo_DigThirdMarking();
}

void Kibombo_DigFourthMarking(void) {
  MapGrid_CopyCellPictureIndicesRect(38, 36, 18, 9, 1, 1);
  MapGrid_CopyCellAttributes(38, 36, 1, 1, 18, 9);
  GameFlag_Set(FLAG_FOURTH_MARKING_DUG);
}

void Kibombo_ScoopFourthMarking(s32 mode) {
  if (mode == 1)
    Kibombo_DigFourthMarking();
}

/// Move on the block Piers cannot reach: it drops into place with a thump, and
/// Piers joins the party.
void Kibombo_MoveBlockForPiers(void) {
  struct Actor *object = Actor_GetById(ACTOR_GROUNDS_PILLAR);

  if (FIELD_CELL(object->x.fixed) == 6 && FIELD_CELL(object->z.fixed) == 18) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_GetById(ACTOR_GROUNDS_PILLAR)->motionFlags = 3;
    object->motionFlags = 3;
    do
      wait(1);
    while (object->velocityY != 0);
    Sound_Play(SE_THUMP);
    wait(10);
    object->motionFlags = 0;
    Actor_GetById(ACTOR_GROUNDS_PILLAR)->motionFlags = 0;
    MapGrid_CopyCellAttributes(5, 18, 1, 1, 6, 18);
    GameFlag_Set(FLAG_KIBOMBO_BLOCK_MOVED_FOR_PIERS);
    if (!GameFlag_Get(FLAG_PIERS_JOINED))
      Kibombo_PiersJoinsParty();
    Event_End();
  }
}

void Kibombo_MeetPiers(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(88, CAMERA_KEEP, 392, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 72, 392);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 88, 392);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_SOUTHWEST, 10);
  Event_SetMessageId((s32)MsgWhWhoAre);
  // "Wh-Who are you?!?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetSpritePriority(ACTOR_KRADEN, 2);
  Actor_SetSpritePriority(ACTOR_JENNA, 2);
  Actor_SetSpritePriority(ACTOR_SHEBA, 2);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(5, 0.8);
  Actor_SetSpeed(6, 0.8);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gKibombo_KradenMeetsPiersScript);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_JennaMeetsPiersScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_SHEBA, gKibombo_ShebaMeetsPiersScript);
  // "What... What are your intentions?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  if (GameFlag_Get(FLAG_PIERS_INTERROGATED))
    Kibombo_PiersHasForgottenParty();
  else
    Kibombo_PiersTakesPartyForEnemies();
  Kibombo_AkafubuOffersCrystal();
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_SetSpeed(5, 1.6);
  Actor_SetSpeed(6, 1.6);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_PartyGathersScript);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gKibombo_PartyGathersScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_KRADEN, gKibombo_PartyGathersScript);
  GameFlag_Set(FLAG_KIBOMBO_PIERS_MET);
  Event_End();
}

void Kibombo_PiersHasForgottenParty(void) {
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 20);
  Event_SetMessageId((s32)MsgItLooksLikeHas);
  // "It looks like Piers has already forgotten us, huh?" Yes: "I know! We met
  // Piers when he was in JAIL!" No: "What, is he so obsessed with his black
  // orb..."
  FieldMessage_ShowAtActor(ACTOR_SHEBA, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_FaceDirection(5, FACING_SOUTH, 10);
    Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
    // "I know! We met Piers when he was in JAIL! And it's not like he had..."
    Event_ShowMessageAndWait(ACTOR_JENNA, 0);
    gFieldWork->message++;
  } else {
    Actor_FaceDirection(5, FACING_SOUTH, 10);
    Actor_SetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    gFieldWork->message++;
    // "What, is he so obsessed with his black orb that he can't even..."
    Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  }
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "Wait a second... I know you..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 40);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 20);
  // "You're the travelers who came seeking me when I was in jail."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  // "So, you do remember!"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  // "What are you doing in this place?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  // "Actually, that's what we wanted to ask you..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 40);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  // "We were worried about you, Piers, so we followed you here..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  // "And that's what brings you to this dangerous place?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  FieldActor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_Jump(ACTOR_SHEBA, 2, 10);
  Actor_Jump(ACTOR_SHEBA, 4, 10);
  // "What's that supposed to mean?"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  FieldActor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
  // "We came here to help you, Piers. Why are you being like that about it?"
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
  // "Tell me something... Exactly when did I ask you for your help?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
}

void Kibombo_PiersTakesPartyForEnemies(void) {
  Event_SetMessageId((s32)MsgHeSeemsTo);
  // "He seems to believe we are enemies..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 10);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  // "I hope he doesn't think we're with the Kibombo."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "I must admit, you don't look much like Kibombo..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 40);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  FieldEffect_SetAttached(7, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "But that begs the question, if you're not Kibombo, then who are you..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  // "They are...warriors. But how did you get in here?"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  // "Why are these warriors in a place like this?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  // "I'm not sure how to answer that..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 40);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  // "Actually, we've been looking for a boat... We kinda need one."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  // "You're looking for a boat... in the middle of the continent? That..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  FieldActor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_Jump(ACTOR_SHEBA, 2, 10);
  Actor_Jump(ACTOR_SHEBA, 4, 10);
  // "Hey, this wasn't exactly our first choice, you know?"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  FieldActor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
  // "A tidal wave destroyed all the boats in the coastal towns."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
  // "You're asking me to believe rather a lot, don't you think?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
}

void Kibombo_PiersThanksParty(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(280, CAMERA_KEEP, 88, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouVeCome);
  // "You've come to help me get my orb back? Thank you, all of you!"
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(7, FACING_WEST);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetSpeed(7, 1.6);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_PIERS, *(s16 *)((u8 *)leader + 10),
                             *(s16 *)((u8 *)leader + 18));
  Actor_WaitForMoveById(ACTOR_PIERS);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  GameFlag_Clear(FLAG_KIBOMBO_PIERS_STAYED_BEHIND);
  GameFlag_Set(FLAG_KIBOMBO_PIERS_WITH_PARTY);
  Event_AddPartyMember(7, 0);
  Event_End();
}

void Kibombo_CountAnswer(void);
void Kibombo_CarryBlackCrystal(void);
void Kibombo_CrowdBows(void);
void Kibombo_CrowdStands(void);
void Kibombo_EndOffering(void);

/// Akafubu raises the Black Crystal before the Great Gabomba while the warriors
/// chant.
void Kibombo_AkafubuOffersCrystal(void) {
  Kibombo_CrowdStands();
  Sound_Play(MUS_SILENCE);
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 10);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 388, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Sound_Play(MUS_JUPITER_LIGHTHOUSE);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CRYSTAL_CARRIER), 0);
  Actor_GetById(ACTOR_CRYSTAL_CARRIER)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_CRYSTAL_CARRIER), px(265),
                                  px(24), px(344));
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 340, 1);
  Actor_SetSpeed(ACTOR_AKAFUBU, 0.2);
  Actor_WalkToAndWait(ACTOR_AKAFUBU, 264, 374);
  Kibombo_TaskAdd(Kibombo_CarryBlackCrystal, TASK_PASS_VBLANK);
  Event_Wait(20);
  Task_Remove(Kibombo_FlickerCeremonyFires);
  Kibombo_AkafubuCastsOnCrystal();
  Task_Add(Kibombo_FlickerCeremonyFires, TASK_PASS_VBLANK);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Sound_Play(MUS_KIBOMBO);
  Kibombo_EndOffering();
  Event_Wait(160);
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 80);
  Sound_Play(SE_576);
  Kibombo_SetStatueMouth(1, 1);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 80);
  Sound_Play(SE_RATTLE);
  Kibombo_SetStatueMouth(0, 1);
  Event_Wait(20);
  FieldEffect_SetAttached(ACTOR_AKAFUBU,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 20);
  Task_Remove(Kibombo_CarryBlackCrystal);
  wait(1);
  Actor_SetPosition(ACTOR_CRYSTAL_CARRIER, 0, 0);
  Actor_FaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
  if (gKibombo_BlackCrystal != 0) {
    if (gKibombo_BlackCrystal->y.fixed > px(36)) {
      do {
        Actor_FaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
        gKibombo_BlackCrystal->y.fixed += 0xffffc000;
        wait(1);
      } while (gKibombo_BlackCrystal->y.fixed > px(36));
    }
  }
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(264, 16, 420, 1);
  Actor_SetSpeed(ACTOR_AKAFUBU, 1.6);
  Actor_WalkToAndWaitById(ACTOR_AKAFUBU, 264, 392);
  Event_Wait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
  Sound_Play(SE_BOSS_ROAR);
  Sound_Play(MUS_KIBOMBO);
  Kibombo_CrowdBows();
  Event_Wait(80);
  FieldCamera_MoveTo(88, 32, 392, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_SetAttachedEffect(7, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_SetSpritePriority(ACTOR_PIERS, 2);
  Actor_SetSpeed(7, 1.6);
  Actor_WalkToAndWait(7, 152, 386);
  Actor_FaceDirection(7, FACING_NORTHEAST, 40);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(7, FACING_NORTHEAST, 20);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 40);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_WalkToAndWait(7, 124, 356);
  Actor_FaceDirection(7, FACING_NORTHEAST, 20);
  Actor_FaceDirection(7, FACING_NORTHWEST, 20);
  Actor_FaceDirection(7, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 40);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToById(ACTOR_KRADEN, 104, 368);
  Event_Wait(20);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(111, CAMERA_KEEP, 356, 1);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 40);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Event_OpenMessage(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Event_ShowMessageAndWait(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Event_ShowMessageAndWait(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
    Kibombo_CountAnswer();
  }
}

void Kibombo_PiersRefusesToLeave(void) {
  if (!Event_GetFlag(FLAG_KIBOMBO_PIERS_WITH_PARTY)) {
    Import_EventSetExitRequest(5);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_MoveToObject(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  Actor_SetSpritePriority(
      7, ((struct FieldSprite *)Actor_GetById(gMainRoutineWork.leaderActorId)
              ->sprite)
             ->priority);
  wait(1);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Actor_SetSpeed(7, 1.6);
  Actor_WalkToAndWaitById(ACTOR_PIERS, 62, 444);
  Actor_FaceDirectionAndRefreshById(ACTOR_PIERS, FACING_SOUTHEAST);
  Event_SetMessageId((s32)MsgNowThatIKnow);
  // "Now that I know the orb is here, I won't leave this village without..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_GetById(ACTOR_PIERS)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_WalkToAndWait(7, 56, 420);
  Actor_WalkToAndWait(7, 40, 344);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 80, 510);
  Event_Wait(10);
  Import_EventSetExitRequest(5);
  GameFlag_Clear(FLAG_KIBOMBO_PIERS_WITH_PARTY);
  GameFlag_Set(FLAG_KIBOMBO_PIERS_STAYED_BEHIND);
  Party_RemoveMemberRestored(7);
  Event_End();
}

#define EVENT_WORK ((u8 *)Ram_HeapSlots->fieldWork)

void Party_GivePiersDjinn(void);
extern u32 gKibombo_LeaderStepsUpScript[];
extern u32 gKibombo_PiersStepsUpScript[];
extern u32 gKibombo_KradenJoinsPiersScript[];
extern u32 gKibombo_JennaJoinsPiersScript[];
extern u32 gKibombo_ShebaJoinsPiersScript[];
extern u32 gKibombo_PartyFollowsScript[];

/// Piers, who saw the party's Psynergy, tells them he too is an Adept, from
/// Lemuria in the middle of the Eastern Sea, and joins the party with his
/// Djinn; Felix's two answers are counted in the event work.
void Kibombo_PiersJoinsParty(void) {
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_NORTH, 0);
  Event_SetMessageId(MsgBlank7);
  // "?!?"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  ((struct Actor *)((s32)FieldCamera_GetActor()))->motionFlags = 0;
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(96, CAMERA_KEEP, 340, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  FieldEffect_SetAttached(7, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_AskYesNo(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(96, CAMERA_KEEP, 372, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.2);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetMovementTypeById(ACTOR_PIERS, gKibombo_PiersStepsUpScript);
  Actor_SetMovementTypeAndWaitById(gMainRoutineWork.leaderActorId,
                                   gKibombo_LeaderStepsUpScript);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetPosition(ACTOR_KRADEN, 56, 372);
  Actor_SetPosition(ACTOR_JENNA, 56, 372);
  Actor_SetPosition(ACTOR_SHEBA, 56, 372);
  wait(1);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gKibombo_KradenJoinsPiersScript);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_JennaJoinsPiersScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_SHEBA, gKibombo_ShebaJoinsPiersScript);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHEAST);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      1) {
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
    ((struct FieldWork *)EVENT_WORK)->message += 1;
  } else {
    ((struct FieldWork *)EVENT_WORK)->message += 2;
    Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  }
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 20);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 20);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 40);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 10);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, 7);
  Actor_WatchAndSetScript(ACTOR_JENNA, 7);
  Actor_WatchAndSetScript(ACTOR_SHEBA, 7);
  Actor_WatchAndSetScript(ACTOR_KRADEN, 7);
  Actor_SetSpeed(ACTOR_PIERS, 1.6);
  Actor_WalkToAndWaitById(ACTOR_PIERS, 72, 368);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_WalkToAndWait(ACTOR_PIERS, 40, 348);
  Actor_WalkToAndWait(ACTOR_PIERS, 40, 332);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_KRADEN);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 40);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
    Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    ((struct FieldWork *)EVENT_WORK)->message += 3;
  } else {
    ((struct FieldWork *)EVENT_WORK)->message += 1;
    Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
    Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
    Actor_FaceDirectionAndRefreshById(ACTOR_SHEBA, FACING_EAST);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
    FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Kibombo_CountAnswer();
  }
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_PIERS, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Event_ShowMessageAndWait(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetSpeed(ACTOR_PIERS, 0.4);
  Actor_WalkToAndWaitById(ACTOR_PIERS, 40, 350);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_PartyFollowsScript + 0);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gKibombo_PartyFollowsScript + 0);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gKibombo_PartyFollowsScript + 0);
  Actor_SetMovementTypeAndWaitById(ACTOR_PIERS,
                                   gKibombo_PartyFollowsScript + 0);
  GameFlag_Set(FLAG_PIERS_JOINED);
  GameFlag_Set(FLAG_KIBOMBO_PIERS_WITH_PARTY);
  Event_AddPartyMember(7, 1);
  Party_GivePiersDjinn();
}
#undef EVENT_WORK

#define EVENT_WORK ((u8 *)Ram_HeapSlots->fieldWork)

extern u32 gKibombo_JennaAtDeadEndScript[];
extern u32 gKibombo_ShebaAtDeadEndScript[];
extern u32 gKibombo_KradenAtDeadEndScript[];
extern u32 gKibombo_PiersAtDeadEndScript[];
extern u32 gKibombo_PartyLeavesDeadEndScript[];
void Kibombo_CastScoop(void);

void Kibombo_DeadEndByTheStatue(void) {
  s32 done = Event_GetFlag(FLAG_KIBOMBO_VENT_UNCOVERED);

  if (done != 0)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 344, 208);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 308, 200);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 290, 188);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_JennaAtDeadEndScript);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gKibombo_ShebaAtDeadEndScript);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gKibombo_KradenAtDeadEndScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_PIERS, gKibombo_PiersAtDeadEndScript);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_SetMessageId(MsgItSADead);
  // "It's a dead end..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
  FieldCamera_SetSpeed(2.4);
  ((struct Actor *)((s32)FieldCamera_GetActor()))->motionFlags = done;
  FieldCamera_MoveTo(200, CAMERA_KEEP, 180, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  FieldCamera_MoveTo(174, CAMERA_KEEP, 224, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 120, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  FieldCamera_MoveTo(272, CAMERA_KEEP, 180, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  // "But this was the only path..."
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  // "Isn't it a little strange that there would be a path that just goes..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, 26);
  Actor_WatchAndSetScript(ACTOR_JENNA, 26);
  Actor_WatchAndSetScript(ACTOR_SHEBA, 26);
  Actor_WatchAndSetScript(ACTOR_PIERS, 26);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 146, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_WalkToAndWait(ACTOR_KRADEN, 290, 116);
  Actor_WalkToAndWait(ACTOR_KRADEN, 268, 109);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
  // "Hmm..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 188, 1);
  Actor_WalkToAndWaitById(ACTOR_KRADEN, 218, 156);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 40);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_PIERS);
  // "I see..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 20);
  Actor_SetSpeed(ACTOR_KRADEN, 0.4);
  Actor_WalkToAndWaitById(ACTOR_KRADEN, 248, 156);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "So that's how it works..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, 26);
  Actor_WatchAndSetScript(ACTOR_JENNA, 26);
  Actor_WatchAndSetScript(ACTOR_SHEBA, 26);
  Actor_WatchAndSetScript(ACTOR_PIERS, 26);
  Event_Wait(20);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWaitById(ACTOR_KRADEN, 248, 188);
  Event_Wait(1);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_SetAnimationById(ACTOR_KRADEN, 7);
  Event_Wait(20);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_PIERS);
  // "Listen up, Felix..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.4);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 280, 188);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(10);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWaitById(ACTOR_KRADEN, 238, 188);
  Event_Wait(1);
  Actor_GetById(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
  // "Dig there!"
  FieldMessage_ShowAtActor(26, 0);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
ask:
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    ((struct FieldWork *)EVENT_WORK)->message += 1;
  } else {
    // "Don't worry. Just use Scoop... You'll see why in just a few seconds..."
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    *(s16 *)(EVENT_WORK + 0x1c4) -= 2;
    // "I see..."
    FieldMessage_ShowAtActor(26, 0);
    goto ask;
  }
  Event_SetFlag(FLAG_SCOOPING_SECOND_MARKING);
  Kibombo_CastScoop();
  GameFlag_Clear(FLAG_SCOOPING_SECOND_MARKING);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_WatchAndSetScript(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_KRADEN, gMainRoutineWork.leaderActorId);
  Actor_WatchAndSetScript(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 164, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 280, 120);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
  Event_SetFlag(FLAG_SCOOPING_FIRST_MARKING);
  Kibombo_CastScoop();
  GameFlag_Clear(FLAG_SCOOPING_FIRST_MARKING);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 232, 142);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
  Event_SetFlag(FLAG_SCOOPING_THIRD_MARKING);
  Kibombo_CastScoop();
  GameFlag_Clear(FLAG_SCOOPING_THIRD_MARKING);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 296, 142);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
  Event_SetFlag(FLAG_SCOOPING_FOURTH_MARKING);
  Kibombo_CastScoop();
  GameFlag_Clear(FLAG_SCOOPING_FOURTH_MARKING);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 188, 1);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 284, 188);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 10);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_KRADEN);
  Actor_Stop(ACTOR_PIERS);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 20);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 10);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 164, 1);
  Actor_WalkToAndWait(ACTOR_JENNA, 296, 168);
  Actor_WalkToAndWait(ACTOR_JENNA, 280, 154);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 40);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 188, 1);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 40);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 20);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 10);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN, FACING_EAST);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetSpeed(ACTOR_PIERS, 1.2);
  Actor_SetMovementTypeById(ACTOR_JENNA, gKibombo_PartyLeavesDeadEndScript);
  Actor_SetMovementTypeById(ACTOR_SHEBA, gKibombo_PartyLeavesDeadEndScript);
  Actor_SetMovementTypeById(ACTOR_KRADEN, gKibombo_PartyLeavesDeadEndScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_PIERS,
                                   gKibombo_PartyLeavesDeadEndScript);
  GameFlag_Set(FLAG_KIBOMBO_DEAD_END_SEEN);
  Event_End();
}

#undef EVENT_WORK

void Kibombo_TrackLeader(void) {
  struct FadeOverlayWork *work = Ram_HeapSlots->fadeOverlay;

  work->actor =
      (struct FadeActor *)Actor_GetById(gMainRoutineWork.leaderActorId);
}

void Kibombo_StopTrackingLeader(void) {
  struct FadeOverlayWork *work = Ram_HeapSlots->fadeOverlay;

  work->actor = 0;
}

s32 Kibombo_ClampColor(s32 value) {
  if (value > 31)
    value = 31;
  if (value < 0)
    value = 0;
  return value;
}

/// Sets a palette colour from its fire colour, brightened by delta (half for
/// green, a quarter for blue).
void Kibombo_FlickerColor(s32 index, s32 entry, s32 delta) {
  u16 color = gKibombo_FireColors[entry];
  s16 r = color & 0x1f;
  s16 g = (color >> 5) & 0x1f;
  s16 b = (color >> 10) & 0x1f;

  r += delta;
  g += delta >> 1;
  b += delta >> 2;

  r = Kibombo_ClampColor(r);
  g = Kibombo_ClampColor(g);
  b = Kibombo_ClampColor(b);
  {
    u16 *dst = &((u16 *)BG_PLTT)[index];

    r |= (b << 10) | (g << 5);
    *dst = r;
  }
}

void Kibombo_FlickerRedderColor(s32 index, s32 entry, s32 delta) {
  u16 color = gKibombo_FireColors[entry];
  s16 r = color & 0x1f;
  s16 g = (color >> 5) & 0x1f;
  s16 b = (color >> 10) & 0x1f;

  r += delta + 4;
  delta -= 4;
  g += delta >> 1;
  b += delta >> 2;

  r = Kibombo_ClampColor(r);
  g = Kibombo_ClampColor(g);
  b = Kibombo_ClampColor(b);
  {
    u16 *dst = &((u16 *)BG_PLTT)[index];

    r |= (b << 10) | (g << 5);
    *dst = r;
  }
}

#define DEFINE_KIBOMBO_WOBBLE(name, secondIndex, secondRange, secondBias,      \
                              thirdIndex, flickerFourth, fourthIndex,          \
                              fourthRange, fourthBias)                         \
  void name(void) {                                                            \
    struct FieldWork *work = Ram_HeapSlots->fieldWork;                         \
                                                                               \
    if (work->psynergy == 0 && work->fieldMenu == 0 && work->pauseMenu == 0 && \
        (gFrameCount & 7) == 0) {                                              \
      Kibombo_FlickerColor(9, 0, (Random_Below(10)) - 5);                      \
      Kibombo_FlickerColor(secondIndex, 1,                                     \
                           (Random_Below(secondRange)) - secondBias);          \
      Kibombo_FlickerColor(thirdIndex, 2, (Random_Below(4)) - 2);              \
      flickerFourth(fourthIndex, 3, (Random_Below(fourthRange)) - fourthBias); \
    }                                                                          \
  }

DEFINE_KIBOMBO_WOBBLE(Kibombo_FlickerVillageFires, 87, 8, 4, 103,
                      Kibombo_FlickerColor, 119, 4, 2)

DEFINE_KIBOMBO_WOBBLE(Kibombo_FlickerCeremonyFires, 7, 6, 3, 6,
                      Kibombo_FlickerRedderColor, 76, 8, 4)

void Kibombo_FlickerStatueFires(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work->psynergy == 0 && work->fieldMenu == 0 && work->pauseMenu == 0 &&
      (gFrameCount & 7) == 0) {
    Kibombo_FlickerColor(169, 0, (Random_Below(4)) - 2);
    Kibombo_FlickerColor(167, 1, (Random_Below(2)) - 1);
    Kibombo_FlickerColor(166, 2, Random16() >> 16);
  }
}

/// Creates the Black Crystal, drawn with the item's own icon.
void Kibombo_CreateBlackCrystal(void) {
  struct Actor **slot = &gKibombo_BlackCrystal;
  s32 zero = 0;
  struct Actor *object = Import_ActorCreate(0x1e9, px(265), px(36), px(344));

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    object->motionFlags = zero;
    object->retainSpriteCache = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Icon_DrawItem(ITEM_BLACK_CRYSTAL);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
}

/// On the roof the leader walks over the house; below he walks behind it.
void Kibombo_SetRoofByLeaderHeight(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  if (leader->y.fixed > px(16)) {
    Actor_SetPosition(ACTOR_DOORWAY_SHADE, 0, 0);
    Actor_SetSpritePriority(ACTOR_LIT_CANDLESTICK_A, 2);
  } else {
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_DOORWAY_SHADE),
                                         px(120), 0, px(168));
    Actor_SetSpritePriority(ACTOR_LIT_CANDLESTICK_A, 3);
  }
}

/// Keeps the Black Crystal above the one who holds it.
void Kibombo_CarryBlackCrystal(void) {
  if (gKibombo_BlackCrystal != 0) {
    struct Actor *object = Actor_GetById(ACTOR_CRYSTAL_CARRIER);

    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, object->x.fixed,
                                    object->y.fixed + px(12), object->z.fixed);
  }
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_KIBOMBO_WOBBLE
#undef Actor_SetSpriteFlags
#undef EVENT_WORK
#include "shared/tables.h"

void Actor_StartScript(struct Actor *object, const void *script);
extern u32 gKibombo_EmberScript[];

/// Embers rise from the statue's fire every fourth frame.
void Kibombo_RaiseEmbers(void) {
  s32 phase = gFrameCount & 3;

  if (phase == 0) {
    struct Actor *object =
        Import_ActorCreate(SPRITE_BLAZE_FLAME, px(264), px(32), px(316));
    s32 value;

    if (object != 0) {
      object->bounce = Trig_Cos(((Random16() << 13) >> 16) + 0x3000);
      object->gravity = px(1);
      object->riseBaseY = 0x40000 - (u16)Random16();
      value = (u16)Random16() + 0xffff8000;
      object->timer = value;
      object->motionFlags = 0;
      Actor_SetSpriteFlags(object, 0);
      Actor_SelectAnimation(object, 2);
      Actor_StartScript(object, gKibombo_EmberScript);
      ((struct FieldSprite *)object->sprite)->priority = 1;
      object->timer = 0;
      object->speedLimit = 0;
      object->acceleration = 0;
      object->update = (void (*)(void))FieldEffect_AdvanceMotion;
    }
  }
}

void Kibombo_RiseSmoke(struct Actor *object) {
  object->x.fixed += object->timer << 8;

  object->y.fixed += px(1) / 2;

  object->scaleX += Q16_ONE / 50;
  object->scaleY += Q16_ONE / 50;
  object->timer += 2;
  if (--*(s32 *)&object->watchedActor == 0)
    Actor_Release(object);
}

/// Smoke puffs from the fire every 64 frames.
void Kibombo_PuffSmoke(void) {
  if ((gFrameCount & 63) == 0) {
    struct Actor *object =
        Import_ActorCreate(SPRITE_DUST_CLOUD, px(88), px(16), px(314));
    u8 *part;
    s32 mask;

    if (object != 0) {
      object->retainSpriteCache = 2;
      object->motionFlags = 0;
      object->timer = 0;
      object->priorityFlags &= ~1;
      mask = 13;
      part = (u8 *)object->sprite;
      part[9] = (part[9] & -mask) | 8;
      Actor_SetSpriteFlags(object, 0);
      object->scaleX = Q16_ONE / 2;
      object->scaleY = Q16_ONE / 2;
      *(s32 *)&object->watchedActor = 60;
      object->update = (void (*)(void))Kibombo_RiseSmoke;
      Actor_SelectAnimation(object, 5);
      Actor_SetBlendMode(object, 1);
    }
  }
}

void Kibombo_FlickerFlame(struct Actor *object) {
  if (object->timer != 0) {
    object->timer--;
  } else {
    object->timer = (Random_Below(10)) + 10;
    if (object->userValue != 0) {
      object->scaleX = Q16_ONE * 3 / 2;
      object->userValue = 0;
    } else {
      object->scaleX = 0xfffe8000;
      object->userValue = 1;
    }
  }
}

/// A flame that flickers by its own timer.
void Kibombo_SpawnFlame(s32 x, s32 y, s32 z) {
  struct Actor *object = Import_ActorCreate(SPRITE_FLAME_AND_SMOKE, x, y, z);
  struct FieldSprite *sprite;

  if (object != 0) {
    object->motionFlags = 0;
    object->priorityFlags &= ~1;
    object->scaleY = Q16_ONE * 2;
    object->timer = (Random_Below(10)) + 10;
    object->userValue = 0;
    object->update = (void (*)(void))Kibombo_FlickerFlame;
    sprite = object->sprite;
    sprite->priority = 1;
    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, 5);
    Actor_SetBlendMode(object, 1);
  }
}

static inline void Kibombo_Spawn(s32 x, s32 y, s32 z) {
  Kibombo_SpawnFlame(x, y, z);
}

void Kibombo_LightFlameRow(void) {
  s32 x = px(440);

  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  Kibombo_Spawn(x, 0x280000, 0x1c20000);
  Kibombo_Spawn(x, 0x2e0000, 0x1b60000);
  Kibombo_Spawn(x, 0x340000, 0x1aa0000);
  Kibombo_Spawn(x, 0x3a0000, 0x19e0000);
  /* FAKEMATCH(dead): an empty once-only block before the last spawn. */
  do {
  } while (0);
  Kibombo_SpawnFlame(x, 0x400000, 0x1920000);
}

#include "shared/actor_hide.h"

void Scene_RunUnlessFlag(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_00) == 0)
      Kibombo_ShowActors();
  } else {
    GameFlag_Set(FLAG_SCENE_TEMP_00);
  }
}

/// Hides the grounds' two shades (black squares, spr_337) before the room
/// opens.
void Kibombo_HideGroundsPair(void) {
  u32 i;
  s32 zero;

  for (i = 0; i <= 1; i++) {
    s32 id = i + ACTOR_GROUNDS_SHADE_A;
    struct Actor *object = Actor_GetById(id);

    Actor_Hide(id);
    zero = 0;
    object->motionFlags = zero;
  }
  Actor_GetById(ACTOR_GROUNDS_SHADE_A)->y.fixed = px(32);
  Actor_GetById(ACTOR_GROUNDS_SHADE_B)->y.fixed = zero;
  Scene_RunUnlessFlag();
}

// Hides room 3's actors 26 and 27 before the room opens.
// XXX: who actors 26 and 27 are in room 3; staging the room would settle it.
void Kibombo_HideHousesPair(void) {
  u32 i;

  for (i = 0; i <= 1; i++) {
    s32 id = i + 26;
    struct Actor *object = Actor_GetById(id);

    Actor_Hide(id);
    object->motionFlags = 0;
  }
  Scene_RunUnlessFlag();
}

#define DEFINE_KIBOMBO_SET_CROWD_MODE(name, n1)                                \
  void name(void) {                                                            \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i <= 7; i++)                                                   \
      Actor_SetAnimationById(i + 18, n1);                                      \
  }

DEFINE_KIBOMBO_SET_CROWD_MODE(Kibombo_CrowdBows, 5)

DEFINE_KIBOMBO_SET_CROWD_MODE(Kibombo_CrowdStands, 1)

/// Akafubu's Psynergy on the Black Crystal.
void Kibombo_AkafubuCastsOnCrystal(void) {
  FieldPsynergy_LoadResources(ABILITY_LIFT, 1);
  FieldPsynergy_SetupActorPair(17, 27);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
}

s32 VramCache_FreeEntry(u32 slot);

union Whirl {
  struct Actor actor;
  struct {
    u8 unused[0x62];
    u8 stage;
    u8 timer;
  } whirl;
};

void Kibombo_SpinSpark(struct Actor *actor) {
  s32 wobble = 0;
  union Whirl *object;
  u8 *timer = &((union Whirl *)actor)->whirl.timer;
  s16 *action;
  u8 *stage;
  struct Vector3 centre;
  s32 radius;
  s32 spin;
  s32 turn;

  if (*timer != 0)
    wobble = Trig_Sin((actor->timer + (s16)actor->userValue) * 9 << 9) * 6;
  object = (union Whirl *)actor;
  action = &object->actor.timer;
  centre.x = object->actor.targetX;
  centre.y = object->actor.targetY;
  centre.z = object->actor.targetZ;
  radius = object->actor.riseBaseY;
  spin = (s16)object->actor.userValue;
  turn = *action;
  Vector_AddPolarOffset(px(radius) + wobble, turn * 3 * 256 + spin, &centre);
  object->actor.x.fixed = centre.x;
  object->actor.y.fixed = centre.y;
  object->actor.z.fixed = centre.z;
  stage = &object->whirl.stage;
  switch (*stage) {
  case 0:
    *action += 4;
    object->actor.riseBaseY = 1;
    ((struct FieldSprite *)object->actor.sprite)->rotation += 0x1000;
    object->actor.scaleX += 0x666;
    object->actor.scaleY += 0x666;
    if (object->actor.scaleX > Q16_ONE * 3 / 2 &&
        ((struct FieldSprite *)object->actor.sprite)->rotation == 0) {
      *stage = 1;
      *timer = 1;
    }
    break;
  case 1:
    *action += 4;
    ((struct FieldSprite *)object->actor.sprite)->rotation += 0x1000;
    if (((struct FieldSprite *)object->actor.sprite)->rotation == 0 &&
        --*timer == 0) {
      Sound_Play(SE_SHIMMER);
      *stage = 2;
    }
    break;
  case 2:
    *action += 1;
    object->actor.targetY += px(4);
    if (object->actor.targetY > px(100))
      *stage = 3;
    break;
  case 3:
    *action += 1;
    object->actor.riseBaseY += 2;
    *timer = 10;
    if (object->actor.riseBaseY > 40)
      *stage = 4;
    break;
  case 4:
    *action += 1;
    if (--*timer == 0)
      *stage = 5;
    break;
  case 5:
    *action += 1;
    object->actor.riseBaseY -= 2;
    if (object->actor.targetY < px(136))
      object->actor.targetY += px(2);
    if (object->actor.riseBaseY <= 1) {
      object->actor.riseBaseY = 0;
      *stage = 6;
    }
    break;
  case 6:
    *action += 4;
    ((struct FieldSprite *)object->actor.sprite)->rotation += 0x1000;
    object->actor.scaleX += -0x1000;
    object->actor.scaleY += -0x1000;
    if (object->actor.scaleX < 0x1200) {
      object->actor.targetX = 100;
      object->actor.targetZ = 100;
      *stage = 7;
    }
    break;
  case 7:
    VramCache_FreeEntry(
        ((struct FieldSprite *)object->actor.sprite)->vramBlock);
    Actor_Release(&object->actor);
    break;
  }
}

/// Akafubu glows and sparks circle him.
void Kibombo_AkafubuChants(void) {
  struct Actor *center = Actor_GetById(ACTOR_AKAFUBU);
  struct Actor *spark;
  s32 previous = 0;
  u32 i;
  u32 angle;
  s32 x;
  s32 y;
  s32 z;

  Sound_Play(SE_CAST);
  for (i = 0; i <= 7; i++) {
    Actor_SetSpriteColor(Actor_GetById(ACTOR_AKAFUBU), 7);
    wait(2);
    Actor_SetSpriteColor(Actor_GetById(ACTOR_AKAFUBU), 0);
    wait(2);
  }
  Sound_Play(SE_MERCURY_LIGHTHOUSE_DOOR_OPEN);
  i = 0;
  angle = FACING_EAST;
  do {
    x = center->x.fixed;
    y = center->y.fixed + px(24);
    z = center->z.fixed;
    spark = Import_ActorCreate(SPRITE_SPARKLE, x, y, z);
    if (spark != 0) {
      previous = Sprite_ShareVramCacheEntry(spark->sprite, previous);
      spark->motionFlags = 0;
      ((struct FieldSprite *)spark->sprite)->priority = 1;
      Actor_SetSpriteFlags(spark, 0);
      Actor_SelectAnimation(spark, 1);
      spark->timer = 0;
      spark->riseBaseY = 0;
      spark->userValue = angle / 360;
      spark->riseCounter = 0;
      spark->targetX = center->x.fixed;
      spark->targetY = center->y.fixed + px(24);
      spark->targetZ = center->z.fixed;
      spark->update = (void (*)(void))Kibombo_SpinSpark;
    }
    angle += 0x160000;
    i++;
  } while (i <= 15);
}

void Kibombo_EndOffering(void) {
  Task_Remove(Kibombo_FlickerCeremonyFires);
  wait(1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  if (gKibombo_BlackCrystal != 0)
    Kibombo_AkafubuChants();
}

/// Opens or closes the Great Gabomba's mouth, its jaws drawn open or hidden.
void Kibombo_SetStatueMouth(s32 open, s32 hide) {
  if (open != 0) {
    MapGrid_CopyCellsTo(49, 37, 79, 15, 3, 2);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_STATUE_JAW_A),
                                         px(253), px(32), px(289));
    Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_STATUE_JAW_B), px(275),
                                    px(32), px(289));
    if (hide != 0) {
      Actor_SetAnimationById(ACTOR_STATUE_JAW_A, 0);
      Actor_SetAnimationById(ACTOR_STATUE_JAW_B, 0);
    } else {
      Actor_SetAnimationById(ACTOR_STATUE_JAW_A, 1);
      Actor_SetAnimationById(ACTOR_STATUE_JAW_B, 1);
    }
  } else {
    MapGrid_CopyCellsTo(45, 37, 79, 15, 3, 2);
    Actor_SetPosition(ACTOR_STATUE_JAW_A, 0, 0);
    Actor_SetPosition(ACTOR_STATUE_JAW_B, 0, 0);
  }
}

/// Puts out the Great Gabomba's tongue, in the pose given, or draws it back.
void Kibombo_SetStatueTongue(s32 open, s32 mode) {
  if (open != 0) {
    MapGrid_CopyCellPictureIndicesRect(41, 38, 79, 16, 3, 3);
    MapGrid_CopyCellAttributes(12, 21, 1, 1, 16, 22);
    if (mode != 0)
      Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_STATUE_TONGUE),
                                      px(264), px(32), px(308));
    switch (mode) {
    case 0:
      Actor_SetPosition(ACTOR_STATUE_TONGUE, 0, 0);
      break;
    case 1:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 0);
      break;
    case 2:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 1);
      break;
    case 3:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 2);
      break;
    case 4:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 3);
      break;
    case 5:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 6);
      Actor_GetById(ACTOR_STATUE_TONGUE)->y.fixed -= px(34);
      break;
    case 6:
      Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 7);
      Actor_GetById(ACTOR_STATUE_TONGUE)->y.fixed -= px(34);
      break;
    }
  } else {
    MapGrid_CopyCellPictureIndicesRect(45, 38, 79, 16, 3, 3);
    MapGrid_CopyCellAttributes(17, 22, 1, 1, 16, 22);
    Actor_SetPosition(ACTOR_STATUE_TONGUE, 0, 0);
  }
}

/// While Scoop is cast, digs the marking the event is scooping.
void Kibombo_DigMarkingUnderLeader(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  if (work->stage != 0) {
    if (GameFlag_Get(FLAG_SCOOPING_FIRST_MARKING))
      Kibombo_DigFirstMarking();
    else if (GameFlag_Get(FLAG_SCOOPING_SECOND_MARKING))
      Kibombo_DigSecondMarking();
    else if (GameFlag_Get(FLAG_SCOOPING_THIRD_MARKING))
      Kibombo_DigThirdMarking();
    else if (GameFlag_Get(FLAG_SCOOPING_FOURTH_MARKING))
      Kibombo_DigFourthMarking();
  }
}

/// Felix casts Scoop on the marking he stands on.
void Kibombo_CastScoop(void) {
  FieldPsynergy_LoadResources(ABILITY_SCOOP, 1);
  Kibombo_TaskAdd(Kibombo_DigMarkingUnderLeader, TASK_PASS_VBLANK);
  FieldPsynergy_SetupActorPair(gMainRoutineWork.leaderActorId, -1);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(Kibombo_DigMarkingUnderLeader);
  FieldPsynergy_End();
}

void Kibombo_StopFireFlicker(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO_CEREMONY)
    Task_Remove(Kibombo_FlickerVillageFires);
  else if (scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
    Task_Remove(Kibombo_FlickerCeremonyFires);
  else if (scene == SCENE_KIBOMBO_STATUE_FACE)
    Task_Remove(Kibombo_FlickerStatueFires);
}

void Kibombo_LeaveRoom(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Get(FLAG_SCENE_TEMP_02))
    Kibombo_StopFireFlicker();
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

void Kibombo_LeaveRoomFading(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Kibombo_LeaveRoom();
}

/// Walking into a door: the leader centres on it, steps in and the scene leaves
/// by its exit.
void Kibombo_EnterDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader;
  s32 x;
  s32 pixelX;

  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  x = leader->x.fixed;
  pixelX = FIELD_PIXEL(x);
  Actor_SetMoveTarget(leader, px(8 - pixelX % 16) + x, leader->y.fixed,
                      leader->z.fixed);
  Actor_WaitForMove(leader);
  leader->facing = FACING_NORTH;
  if (GameFlag_Get(FLAG_SCENE_TEMP_02))
    Kibombo_StopFireFlicker();
  Actor_SetMovementTypeById(gMainRoutineWork.leaderActorId,
                            gKibombo_EnterDoorScript);
  wait(12);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

/// The leader walks to the hole at x, z, drops into it and the scene leaves by
/// its exit.
void Kibombo_DropIntoHole(s32 x, s32 z) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(party->leaderActorId);

  Event_Start();
  Event_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, x, z);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_NORTH);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 13);
  FieldActor_SetMoveTarget(leader, x, -12, z);
  Actor_WaitForMove(leader);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 10);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

/// Stepping into a hole drops the leader in only while he presses on toward it.
void Kibombo_StepIntoFirstHole(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (x == 42 && z == 23 && (gInput.held & KEY_UP) == 0)
    return;
  if (x == 43 && z == 22 && (gInput.held & KEY_LEFT) == 0)
    return;
  Kibombo_DropIntoHole(680, 352);
}

#define DEFINE_KIBOMBO_STEP_WHEN_HELD(name, n1, n2, n3, n4, n5, n6, n7)        \
  void name(void) {                                                            \
    struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);      \
    s32 x = FIELD_CELL(leader->x.fixed);                                       \
    s32 z = FIELD_CELL(leader->z.fixed);                                       \
                                                                               \
    if (z == n1) {                                                             \
      if (x == n2 && (gInput.held & KEY_RIGHT) == 0)                           \
        return;                                                                \
      if (x == n3 && (gInput.held & KEY_LEFT) == 0)                            \
        return;                                                                \
    } else if (x == n4 && z == n5 && (gInput.held & KEY_UP) == 0) {            \
      return;                                                                  \
    }                                                                          \
    Kibombo_DropIntoHole(n6, n7);                                              \
  }

DEFINE_KIBOMBO_STEP_WHEN_HELD(Kibombo_StepIntoSecondHole, 38, 23, 25, 24, 39,
                              392, 608)

DEFINE_KIBOMBO_STEP_WHEN_HELD(Kibombo_StepIntoThirdHole, 54, 42, 44, 43, 55,
                              696, 864)

void Kibombo_EnterVent(void) {
  if (GameFlag_Get(FLAG_SCENE_TEMP_02))
    Kibombo_StopFireFlicker();
  Kibombo_DropIntoHole(376, 288);
}

/// Fades the screen in and runs Kibombo_FlickerStatueFires each frame for up to
/// 160 frames, or until A, B or Start is held, then fades out.
void Kibombo_GazeAtStatue(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Task_Add(Kibombo_FlickerStatueFires, TASK_PASS_VBLANK);
  Event_Wait(20);
  i = 0;
  goto test;
next:
  i++;
test:
  if (i > 159)
    goto done;
  Event_Wait(1);
  if ((gInput.held & (KEY_A | KEY_B | KEY_START)) == 0)
    goto next;
done:
  Task_Remove(Kibombo_FlickerStatueFires);
  gFieldWork->transitionFrames = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(11);
}

/// Three flashes of light, each with the camera at another place.
void Kibombo_StatueFlashes(void) {
  Sound_Play(SE_POWER_UP);
  PaletteFade_ApplyTarget(RGB_WHITE, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  PaletteFade_ApplyTarget(0x10000, 0);
  gFieldWork->transitionFrames = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Kibombo_FlickerStatueFires, TASK_PASS_VBLANK);
  wait(60);
  Task_Remove(Kibombo_FlickerStatueFires);
  wait(1);

  Sound_Play(SE_POWER_UP);
  PaletteFade_ApplyTarget(RGB_WHITE, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  FieldCamera_MoveTo(584, CAMERA_KEEP, 248, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  Kibombo_TaskAdd(Kibombo_FlickerStatueFires, TASK_PASS_VBLANK);
  wait(60);
  Task_Remove(Kibombo_FlickerStatueFires);
  wait(1);

  Sound_Play(SE_POWER_UP);
  PaletteFade_SetTarget(0x7fff, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  FieldCamera_MoveTo(248, CAMERA_KEEP, 536, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  PaletteFade_SetTarget(0x10000, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  Task_Add(Kibombo_FlickerStatueFires, TASK_PASS_VBLANK);
  wait(180);
  Task_Remove(Kibombo_FlickerStatueFires);
  wait(1);

  gFieldWork->transitionFrames = 1;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Import_EventSetExitRequest(10);
}

void Kibombo_CeremonyBegins(void) {
  struct Actor *leader;

  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Event_SetMessageId((s32)MsgAtLastThe);
  // "At last, the sacred night has come!"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 86, 455);
  Actor_SetPosition(ACTOR_KRADEN, 86, 455);
  wait(1);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 72, 455);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(264, 16, 420, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(264, 32, 320, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(80);
  Actor_Jump(ACTOR_AKAFUBU, 4, 40);
  // "Tonight, Akafubu becomes the Kibombo's new witch doctor!"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(264, 32, 400, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST, 20);
  // "All of you, pray in the depths of your hearts for my success!"
  Event_ShowMessageAndWait(0x4000 + ACTOR_AKAFUBU, 0);
  Sound_Play(MUS_KIBOMBO);
  GameFlag_Set(FLAG_GABOMBA_CEREMONY_UNDER_WAY);
  Sound_Play(SE_BOSS_ROAR);
  Actor_GetById(ACTOR_WARRIOR_A)->velocityY = px(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_B)->velocityY = px(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_C)->velocityY = px(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_D)->velocityY = px(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_E)->velocityY = px(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_F)->velocityY = px(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_G)->velocityY = px(4);
  wait(5);
  Actor_GetById(ACTOR_WARRIOR_H)->velocityY = px(4);
  wait(20);
  Actor_FaceDirectionById(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 20);
  Kibombo_CrowdBows();
  Event_Wait(40);
  FieldCamera_SetSpeed(0.2);
  FieldCamera_MoveTo(264, 0, 400, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(200);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(111, 32, 456, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  // "It looks like the initiation ceremony is beginning."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 20);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "I'm not sure what will happen, but we'd better hurry, Felix."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].pixel,
                             leader->z.part[0].pixel);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  GameFlag_Set(FLAG_KIBOMBO_CEREMONY_BEGUN);
}

void Kibombo_CarryBlackCrystal(void);

void Kibombo_PresentOrbToGabomba(void) {
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId), 15);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Kibombo_FlickerCeremonyFires, TASK_PASS_VBLANK);
  Event_Wait(40);
  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 388, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
  Event_SetMessageId((s32)MsgLetUsNow);
  // "Let us now present our jewel to the Great Gabomba!"
  Event_ShowMessageAndWait(0x4000 + ACTOR_AKAFUBU, 0);
  Sound_Play(MUS_JUPITER_LIGHTHOUSE);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CRYSTAL_CARRIER), 0);
  Actor_GetById(ACTOR_CRYSTAL_CARRIER)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_CRYSTAL_CARRIER), px(265),
                                  px(24), px(344));
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 340, 1);
  Actor_SetSpeed(ACTOR_AKAFUBU, 0.2);
  Actor_WalkToAndWaitById(ACTOR_AKAFUBU, 264, 374);
  wait(1);
  Kibombo_TaskAdd(Kibombo_CarryBlackCrystal, TASK_PASS_VBLANK);
  Event_Wait(20);
  Task_Remove(Kibombo_FlickerCeremonyFires);
  Kibombo_AkafubuCastsOnCrystal();
  Kibombo_TaskAdd(Kibombo_FlickerCeremonyFires, TASK_PASS_VBLANK);
  // "O Great Gabomba! Please accept this, our humble gift!"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Sound_Play(MUS_KIBOMBO);
  Kibombo_EndOffering();
  Event_Wait(160);
  // "Please, Great Gabomba! Accept this jewel!"
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 80);
  Sound_Play(SE_576);
  Kibombo_SetStatueMouth(1, 1);
  Event_Wait(20);
  // "Look! The Great Gabomba has noticed the light of the jewel!"
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 40);
  Task_Remove(Kibombo_FlickerCeremonyFires);
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(10);
}

void Kibombo_GabombaNotPleased(void) {
  struct Actor *object;

  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId), 15);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CRYSTAL_CARRIER), 0);
  Actor_GetById(ACTOR_CRYSTAL_CARRIER)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_CRYSTAL_CARRIER), px(265),
                                  px(24), px(344));
  Kibombo_SetStatueMouth(1, 1);
  wait(1);
  if (gKibombo_BlackCrystal != 0) {
    gKibombo_BlackCrystal->y.fixed += px(32);
    wait(1);
    gKibombo_BlackCrystal->motionFlags = 4;
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Task_Add(Kibombo_FlickerCeremonyFires, TASK_PASS_VBLANK);
  Event_Wait(40);
  Sound_Play(SE_RATTLE);
  Kibombo_SetStatueMouth(0, 1);
  Event_Wait(20);
  FieldEffect_SetAttached(ACTOR_AKAFUBU,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouAreStill);
  // "You are still not pleased?"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  object = gKibombo_BlackCrystal;
  if (object != 0) {
    object->motionFlags = 0;
    while (object->y.fixed > px(36)) {
      Actor_FaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
      gKibombo_BlackCrystal->y.fixed -= px(1) / 4;
      wait(1);
      object = gKibombo_BlackCrystal;
    }
  }
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessageAndWait(0x4000 + ACTOR_AKAFUBU, 0);
  GameFlag_Clear(FLAG_GABOMBA_MUSIC_SILENCED);
  Sound_Play(MUS_KIBOMBO);
  Event_Wait(80);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(11);
}

void Kibombo_WitchDoctorFeelsPower(void) {
  struct FieldWork *work;
  s32 effect;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId), 15);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Kibombo_CrowdBows();
  Task_Add(Kibombo_FlickerCeremonyFires, TASK_PASS_VBLANK);
  work = Ram_HeapSlots->fieldWork;
  work->transition = effect = 513;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Event_SetMessageId((s32)MsgOhTheMagic);
  // "Oh! The magic, the power of the witch doctor fills my body!"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Task_Remove(Kibombo_FlickerCeremonyFires);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST);
  PaletteFade_SetSource(0x10000, 0);
  PaletteFade_SetTarget(0, 0);
  gFieldWork->transition = effect;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_GABOMBA_BACK_FROM_CEREMONY);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(12);
}

/// Fades in on the ceremony with the Great Gabomba's mouth shut.
void Kibombo_OpenCeremonyScene(void) {
  struct FieldWork *work;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId), 15);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Task_Add(Kibombo_FlickerCeremonyFires, TASK_PASS_VBLANK);
  Kibombo_SetStatueMouth(1, 1);
  if (gKibombo_BlackCrystal != 0) {
    gKibombo_BlackCrystal->y.fixed += px(32);
    wait(1);
    gKibombo_BlackCrystal->motionFlags = 4;
  }
  wait(1);
  work = Ram_HeapSlots->fieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
}

extern s32 gKibombo_CrowdDanceScript[];

/// The warriors jump and cheer, then dance.
void Kibombo_CrowdCheers(void) {
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(264, 0, 400, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_Jump(ACTOR_WARRIOR_A, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_B, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_C, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_D, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_E, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_F, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_G, 6, 0);
  Actor_Jump(ACTOR_WARRIOR_H, 6, 40);
  Actor_FaceDirectionAndWait(ACTOR_WARRIOR_C, FACING_SOUTH_SOUTHWEST);
  Actor_Jump(ACTOR_WARRIOR_C, 2, 0);
  Event_ShowMessageAndWait(0x4000 + ACTOR_WARRIOR_C, 0);
  Actor_FaceDirectionAndWait(ACTOR_WARRIOR_F, FACING_SOUTH_SOUTHEAST);
  Actor_Jump(ACTOR_WARRIOR_F, 2, 0);
  Event_ShowMessageAndWait(0x4000 + ACTOR_WARRIOR_F, 0);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_A, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_B, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_C, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_D, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_E, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_F, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_G, gKibombo_CrowdDanceScript);
  Actor_SetMovementTypeById(ACTOR_WARRIOR_H, gKibombo_CrowdDanceScript);
  Event_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  Actor_Jump(ACTOR_AKAFUBU, 2, 10);
  Actor_Jump(ACTOR_AKAFUBU, 4, 10);
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_Stop(ACTOR_WARRIOR_A);
  Actor_Stop(ACTOR_WARRIOR_B);
  Actor_Stop(ACTOR_WARRIOR_C);
  Actor_Stop(ACTOR_WARRIOR_D);
  Actor_Stop(ACTOR_WARRIOR_E);
  Actor_Stop(ACTOR_WARRIOR_F);
  Actor_Stop(ACTOR_WARRIOR_G);
  Actor_Stop(ACTOR_WARRIOR_H);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_WARRIOR_A, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_B, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_C, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_D, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_E, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_F, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_G, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_WARRIOR_H, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
  Actor_Jump(ACTOR_AKAFUBU, 4, 10);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_AKAFUBU, 0);
}

void Kibombo_GabombaStillNotPleased(void) {
  struct Actor **slot;
  struct Actor *object;

  Kibombo_OpenCeremonyScene();
  Sound_Play(SE_RATTLE);
  Kibombo_SetStatueMouth(0, 1);
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouAreStillNot);
  // "You are still not pleased?"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  object = gKibombo_BlackCrystal;
  if (object != 0) {
    // Lowers the object 0x4000 a frame down to y 0x240000.
    object->motionFlags = 0;
    if (object->y.fixed > 144 << 14) {
      do {
        Actor_FaceDirectionById(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
        slot = &gKibombo_BlackCrystal;
        object = *slot;
        object->y.fixed += 0xffffc000;
        wait(1);
      } while ((*slot)->y.fixed > px(36));
    }
  }
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_SHAKE_HEAD);
  // "No! This time, he seemed almost like he might..."
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHEAST);
  // "We must continue! Drums! More drums!"
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  GameFlag_Set(FLAG_GABOMBA_BACK_FROM_CEREMONY);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(12);
}

void Kibombo_GabombaSeesOrb(void) {
  Kibombo_OpenCeremonyScene();
  Sound_Play(SE_576);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_A), 7);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_B), 7);
  wait(8);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_A), 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_B), 0);
  wait(8);
  Actor_ShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_SetMessageId((s32)MsgLookLookThe);
  // "Look! Look! The Great Gabomba has seen our jewel!"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Sound_Play(SE_GABOMBA_JAWS_OPEN);
  Actor_SetMovementTypeById(ACTOR_STATUE_JAW_A, gKibombo_JawsOpenScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_STATUE_JAW_B, gKibombo_JawsOpenScript);
  Actor_SetMovementTypeById(ACTOR_STATUE_JAW_A, gKibombo_JawsSettleScript);
  Actor_SetMovementTypeById(ACTOR_STATUE_JAW_B, gKibombo_JawsSettleScript);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_A), 12);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_B), 12);
  Event_Wait(120);
  Kibombo_CrowdCheers();
  GameFlag_Set(FLAG_GABOMBA_BACK_FROM_CEREMONY);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(12);
}

void Kibombo_GabombaResponds(void) {
  Kibombo_OpenCeremonyScene();
  Event_PlaySound(0x240);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_A), 7);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_B), 7);
  wait(8);
  Actor_GetById(ACTOR_STATUE_JAW_A)->update =
      (void (*)(void))Kibombo_FlashSprite;
  Actor_GetById(ACTOR_STATUE_JAW_B)->update =
      (void (*)(void))Kibombo_FlashSprite;
  Actor_ShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_SetMessageId((s32)MsgLookTheGreat);
  // "Look! The Great Gabomba has responded to our gift!"
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 20);
  Sound_Play(SE_576);
  Kibombo_SetStatueTongue(1, 0);
  Event_Wait(20);
  Sound_Play(SE_WAVE);
  Task_Add(Kibombo_RaiseEmbers, TASK_PASS_VBLANK);
  Event_Wait(120);
  Kibombo_CrowdCheers();
  GameFlag_Set(FLAG_GABOMBA_BACK_FROM_CEREMONY);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(12);
}

void Kibombo_GabombaRespondsToGift(void) {
  struct Actor *object;
  u32 i;

  object = Actor_GetById(ACTOR_STATUE_TONGUE);
  Kibombo_OpenCeremonyScene();
  Actor_SetSpritePriority(ACTOR_AKAFUBU, 1);
  Event_PlaySound(0x240);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_A), 7);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_B), 7);
  wait(8);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_A), 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_STATUE_JAW_B), 0);
  wait(8);
  Actor_ShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_SetMessageId((s32)MsgLookTheGreatGabomba);
  // "Look! The Great Gabomba has responded to our gift!"
  Event_ShowMessageThenWait(ACTOR_AKAFUBU, 0, 20);
  Event_PlaySound(0x240);
  Kibombo_SetStatueTongue(1, 0);
  Event_Wait(20);
  Sound_Play(SE_CART_ROLL);
  object->scaleY = Q16_ONE / 10;
  Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_STATUE_TONGUE), px(264),
                                  px(24), px(308));
  wait(1);
  Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 3);
  for (i = 0; i < 64; i++) {
    object->scaleY += Q16_ONE / 64;
    object->y.fixed += px(1) / 8;
    wait(1);
  }
  Sound_Play(SE_STOP_MUSIC_PLAYER_5);
  if (gKibombo_BlackCrystal != 0) {
    struct Actor *gift = gKibombo_BlackCrystal;

    gift->motionFlags = 0;
    if (gift->y.fixed > px(36)) {
      do {
        gKibombo_BlackCrystal->y.fixed -= px(1) / 2;
        wait(1);
      } while (gKibombo_BlackCrystal->y.fixed > px(36));
    }
  }
  Sound_Play(SE_CRUNCH);
  if (gKibombo_BlackCrystal != 0) {
    gKibombo_BlackCrystal->motionFlags = 0;
    i = 0;
    do {
      gKibombo_BlackCrystal->y.fixed += px(6);
      object->scaleY -= Q16_ONE / 8;
      object->y.fixed -= px(1) / 2;
      wait(1);
      i++;
    } while (i < 4);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
    for (i = 0; i < 4; i++) {
      object->scaleY -= Q16_ONE / 8;
      object->y.fixed -= px(1) / 2;
      wait(1);
    }
  }
  Actor_SetPosition(ACTOR_STATUE_TONGUE, 0, 0);
  Kibombo_SetStatueTongue(0, 0);
  Kibombo_SetStatueMouth(1, 1);
  Event_Wait(10);
  Kibombo_SetStatueMouth(0, 1);
  Event_Wait(40);
  Event_PlaySound(0x240);
  Kibombo_SetStatueMouth(1, 1);
  Kibombo_SetStatueTongue(1, 0);
  Sound_Play(SE_WATER);
  Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_STATUE_TONGUE), px(264),
                                  px(24), px(308));
  wait(1);
  for (i = 0; i < 8; i++) {
    object->scaleY += Q16_ONE / 8;
    object->y.fixed += px(1) / 2;
    wait(1);
  }
  FieldEffect_SetAttached(ACTOR_AKAFUBU,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, 0x3000);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  FieldActor_ShowEmote(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Actor_SetSpeed(ACTOR_AKAFUBU, 0.8);
  Actor_WalkToAndWait(ACTOR_AKAFUBU, 264, 320);
  Actor_SetPosition(ACTOR_AKAFUBU, 0, 0);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_GABOMBA_RESPONDED);
  Import_EventSetExitRequest(13);
}

void Kibombo_CeremonyEnds(void) {
  struct FieldWork *work;
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_MoveTo(384, CAMERA_KEEP, 601, 0);
  wait(1);
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      gMainRoutineWork.leaderActorId, px(370), px(636), FACING_NORTH);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_SetSource(0, 0);
  PaletteFade_SetTarget(0x10002, 0);
  gFieldWork->transitionFrames = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_AKAFUBUS_FATHER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU_AT_HOME, ANIM_NOD);
  Actor_FaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHEAST, 20);
  Actor_GetById(ACTOR_KRADEN_AT_HOME)->velocityY = px(3);
  Event_Wait(40);
  Actor_SetAnimationById(ACTOR_AKAFUBUS_FATHER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU_AT_HOME, ANIM_NOD);
  PaletteFade_SetTarget(0x10000, 0);
  PaletteFade_StartInterpolation(40);
  wait(60);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
  Event_SetMessageId((s32)MsgRightWellI);
  // "Right... Well, I guess that's that!"
  Event_ShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBUS_FATHER, ANIM_NOD);
  // "You are virtuous men, coming here to deliver this message. My son..."
  Event_ShowMessageAndWait(ACTOR_AKAFUBUS_FATHER, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN_AT_HOME, FACING_EAST, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_KRADEN_AT_HOME, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 10);
  Actor_SetAnimationById(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_Jump(ACTOR_AKAFUBU_AT_HOME, 4, 10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_AKAFUBUS_FATHER, FACING_EAST);
  Actor_ShowEmoteById(ACTOR_AKAFUBU_AT_HOME, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                      20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
  Actor_ShowEmoteById(ACTOR_AKAFUBUS_FATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      20);
  Event_ShowMessageAndWait(ACTOR_AKAFUBUS_FATHER, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU_AT_HOME, FACING_WEST);
  Actor_SetAnimationById(ACTOR_AKAFUBU_AT_HOME, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU_AT_HOME, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
  Actor_SetAnimationById(ACTOR_AKAFUBU_AT_HOME, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_AKAFUBU_AT_HOME, 0);
  Actor_FaceDirection(ACTOR_AKAFUBUS_FATHER, FACING_NORTH_NORTHEAST, 0);
  Actor_SetSpeed(ACTOR_AKAFUBU_AT_HOME, 1.6);
  Actor_WalkToAndWait(ACTOR_AKAFUBU_AT_HOME, 364, 576);
  Actor_FaceDirection(ACTOR_AKAFUBU_AT_HOME, FACING_NORTH_NORTHWEST, 0);
  Actor_ShowEmoteById(ACTOR_AKAFUBUS_FATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                      80);
  Event_ShowMessageAndWait(ACTOR_AKAFUBUS_FATHER, 0);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBUS_FATHER, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessageAndWait(ACTOR_AKAFUBUS_FATHER, 0);
  Actor_SetAnimationById(ACTOR_AKAFUBUS_FATHER, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_AKAFUBUS_FATHER, 0);
  Event_ShowMessageAndWait(ACTOR_AKAFUBUS_FATHER, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_KRADEN_AT_HOME, FACING_EAST);
  Event_ShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_WEST);
  Actor_SetAnimationById(ACTOR_KRADEN_AT_HOME, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_KRADEN_AT_HOME, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetSpeed(ACTOR_KRADEN_AT_HOME, 0.8);
  Actor_SetAnimationById(ACTOR_KRADEN_AT_HOME, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_KRADEN_AT_HOME, leader->x.part[0].pixel,
                             leader->z.part[0].pixel);
  Actor_WaitForMoveById(ACTOR_KRADEN_AT_HOME);
  Actor_SetPosition(ACTOR_KRADEN_AT_HOME, 0, 0);
  work = Ram_HeapSlots->fieldWork;
  work->transitionFrames = 16;
  work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  GameFlag_Set(FLAG_GABOMBA_CEREMONY_ENDED);
  Event_End();
}

s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_KIBOMBO)
    Kibombo_InitializeVillageByDay();
  else if (scene == SCENE_KIBOMBO_CEREMONY)
    Kibombo_InitializeCeremony();
  else if (scene == SCENE_KIBOMBO_STATUE_GROUNDS)
    Kibombo_InitializeStatueGroundsByDay();
  else if (scene == SCENE_KIBOMBO_STATUE_GROUNDS_CEREMONY)
    Kibombo_InitializeStatueGroundsCeremony();
  else if (scene == SCENE_KIBOMBO_STATUE_FACE)
    Kibombo_InitializeStatueFace();
  else if (scene == SCENE_KIBOMBO_HOUSES)
    Kibombo_InitializeHouses();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The village: the frozen puddles, the rope and the roofs.
void Kibombo_InitializeVillage(void) {
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANDLESTICK_A), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANDLESTICK_B), 0);
  FieldEffect_StartBlankOverlay(0);
  FlagBlock_PlaceAll(gKibombo_FrozenPuddle);
  Actor_SetSpritePriority(ACTOR_VILLAGE_PUDDLE, 3);
  Rope_Initialize();
  Rope_Place(0, 0x9e9, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
  Actor_GetById(ACTOR_GATE_GUARD_A)->collisionFlags |= 0x80;
  Actor_GetById(ACTOR_GATE_GUARD_B)->collisionFlags |= 0x80;
}

void Kibombo_InitializeVillageByDay(void) {
  Actor_SetPosition(ACTOR_FLAME, 88, 314);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FLAME), 0);
  Actor_GetById(ACTOR_FLAME)->scaleY = Q16_ONE / 2;
  Kibombo_InitializeVillage();
  GameFlag_Clear(0x86b);
  Actor_DisableById(ACTOR_SIGNAL_FIRE);
  Actor_SetAnimationById(ACTOR_SIGNAL_FIRE, 2);
  Task_Add(Kibombo_PuffSmoke, TASK_PASS_VBLANK);
  Kibombo_LightFlameRow();
}

// Kibombo's setup.

struct Unit *Unit_Get(u32 unitId);

/// Unless flag 265 is set, places actor 11 and, arriving by entrance 19 with
/// Piers in the party, restores his HP and PP, sets his gauges, clears two of
/// his bytes, marks him as staying behind (FLAG_KIBOMBO_PIERS_STAYED_BEHIND)
/// and removes him from the party; then saves four palette colours, frees
/// actors 30 to 35, sets flags 0x202 and 0x209, places actor 29, places actors
/// 21 and 26 to 28 when flag 0x8fd is set, sets flag 0x86b and fades in with
/// Kibombo_FlickerVillageFires scheduled.
void Kibombo_InitializeCeremony(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FLAME);
  struct Unit *unit;
  s32 done;

  Event_Start();
  Event_ResolvePendingActions(0);
  done = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  if (!done) {
    actor->motionFlags = done;
    actor->y.fixed = px(42);
    actor->z.fixed += px(-42);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FLAME), 0);
    Actor_SetSpritePriority(ACTOR_FLAME, 1);
    if (gMainRoutineWork.entrance == 19 &&
        GameFlag_Get(FLAG_IN_PARTY(ACTOR_PIERS))) {
      unit = Unit_Get(ACTOR_PIERS);
      unit->hp = unit->hpMax;
      unit->pp = unit->ppMax;
      Unit_UpdateGauges(unit);
      unit->poison = 0;
      unit->haunt = 0;
      GameFlag_Set(FLAG_KIBOMBO_PIERS_STAYED_BEHIND);
      GameFlag_Clear(FLAG_KIBOMBO_PIERS_WITH_PARTY);
      Party_RemoveMemberRestored(7);
    }
  }
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  gKibombo_FireColors[0] = *(u16 *)&BG_PLTT_COLOR(0, 9);
  gKibombo_FireColors[1] = *(u16 *)&BG_PLTT_COLOR(5, 7);
  gKibombo_FireColors[2] = *(u16 *)&BG_PLTT_COLOR(6, 7);
  gKibombo_FireColors[3] = *(u16 *)&BG_PLTT_COLOR(7, 7);
  Kibombo_InitializeVillage();
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_A), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_B), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_C), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_D), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_E), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LIT_CANDLESTICK_F), 0);
  GameFlag_Set(FLAG_SCENE_TEMP_09);
  actor = Actor_GetById(ACTOR_DOORWAY_SHADE);
  actor->motionFlags = 0;
  actor->y.fixed = 0;
  Actor_Hide(ACTOR_DOORWAY_SHADE);
  Actor_SetSpritePriority(ACTOR_LIT_CANDLESTICK_A, 3);
  if (GameFlag_Get(FLAG_AKAFUBU_OFFERED_HELP)) {
    Actor_SetPosition(26, 0, 0);
    Actor_SetPosition(27, 0, 0);
    Actor_SetPosition(28, 0, 0);
    Actor_SetPosition(21, 302, 172);
  }
  GameFlag_Set(0x86b);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  {
    s32 priority = TASK_PASS_VBLANK;

    Task_Add(Kibombo_FlickerVillageFires, priority);
  }
  Event_End();
}

void Kibombo_CreateBlackCrystal(void);

/// The grounds before the statue: the Black Crystal, the markings dug, the vent
/// and the block Moved for Piers.
void Kibombo_InitializeStatueGrounds(void) {
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANDLESTICK_A), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANDLESTICK_B), 0);
  if (!GameFlag_Get(FLAG_GABOMBA_RESPONDED))
    Kibombo_CreateBlackCrystal();
  Kibombo_HideGroundsPair();
  if (!GameFlag_Get(0x869))
    GameFlag_Set(FLAG_SCENE_TEMP_01);
  FlagBlock_PlaceAll(gKibombo_FrozenPuddle);
  Actor_SetSpritePriority(ACTOR_GROUNDS_PUDDLE, 2);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_GROUNDS_PILLAR), 0);
  Actor_GetById(ACTOR_GROUNDS_PILLAR)->motionFlags = 0;
  if (GameFlag_Get(FLAG_KIBOMBO_BLOCK_MOVED_FOR_PIERS)) {
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_GROUNDS_PILLAR),
                                         px(104), 0, px(296));
    MapGrid_CopyCellAttributes(5, 18, 1, 1, 6, 18);
    wait(10);
  }
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (GameFlag_Get(FLAG_KIBOMBO_VENT_UNCOVERED))
    Kibombo_UncoverVent();
  if (GameFlag_Get(FLAG_FIRST_MARKING_DUG))
    Kibombo_DigFirstMarking();
  if (GameFlag_Get(FLAG_SECOND_MARKING_DUG))
    Kibombo_DigSecondMarking();
  if (GameFlag_Get(FLAG_THIRD_MARKING_DUG))
    Kibombo_DigThirdMarking();
  if (GameFlag_Get(FLAG_FOURTH_MARKING_DUG))
    Kibombo_DigFourthMarking();
  Actor_DisableById(ACTOR_STATUE_JAW_A);
  Actor_DisableById(ACTOR_STATUE_JAW_B);
  Actor_DisableById(ACTOR_STATUE_TONGUE);
  Actor_SetSpritePriority(ACTOR_STATUE_JAW_A, 1);
  Actor_SetSpritePriority(ACTOR_STATUE_JAW_B, 1);
  Actor_SetSpritePriority(ACTOR_STATUE_TONGUE, 1);
}

void Kibombo_InitializeStatueGroundsByDay(void) {
  s16 entrance;

  Kibombo_InitializeStatueGrounds();
  if (GameFlag_Get(FLAG_GABOMBA_RESPONDED)) {
    Kibombo_SetStatueMouth(1, 0);
    Kibombo_SetStatueTongue(1, 5);
  }
  wait(1);
  entrance = gMainRoutineWork.entrance;
  if (entrance == 99) {
    Kibombo_SetStatueMouth(0, 0);
    Kibombo_SetStatueTongue(0, 0);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (entrance == 98) {
    Kibombo_SetStatueMouth(1, 0);
    Kibombo_SetStatueTongue(1, 0);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (entrance == 97) {
    Kibombo_SetStatueMouth(1, 0);
    Kibombo_SetStatueTongue(1, 2);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (entrance == 96) {
    Kibombo_SetStatueMouth(1, 1);
    Kibombo_SetStatueTongue(1, 4);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
    Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 4);
  }
}

void Kibombo_InitializeStatueGroundsCeremony(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_Set(0x869);
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  gKibombo_FireColors[0] = BG_PLTT[9];
  gKibombo_FireColors[1] = BG_PLTT[7];
  gKibombo_FireColors[2] = BG_PLTT[6];
  gKibombo_FireColors[3] = BG_PLTT[76];
  Kibombo_InitializeStatueGrounds();
  if (GameFlag_Get(FLAG_GABOMBA_RESPONDED)) {
    Kibombo_SetStatueMouth(1, 1);
    Kibombo_SetStatueTongue(1, 4);
  }
  wait(1);
  switch (gMainRoutineWork.entrance) {
  case 10:
    Kibombo_PresentOrbToGabomba();
    return;
  case 11:
    Kibombo_GabombaNotPleased();
    return;
  case 12:
    Kibombo_WitchDoctorFeelsPower();
    return;
  case 13:
    Kibombo_GabombaStillNotPleased();
    return;
  case 14:
    Kibombo_GabombaSeesOrb();
    return;
  case 15:
    Kibombo_GabombaResponds();
    return;
  case 16:
    Kibombo_GabombaRespondsToGift();
    return;
  }
  Actor_SetSpritePriority(ACTOR_AKAFUBU, 1);
  Actor_SetSpritePriority(ACTOR_PIERS, 2);
  Actor_SetSpritePriority(ACTOR_KRADEN, 2);
  Actor_SetSpritePriority(ACTOR_JENNA, 2);
  Actor_SetSpritePriority(ACTOR_SHEBA, 2);
  if (GameFlag_Get(FLAG_KIBOMBO_PIERS_WITH_PARTY))
    Actor_SetPosition(ACTOR_PIERS, 0, 0);
  else if (GameFlag_Get(FLAG_KIBOMBO_PIERS_STAYED_BEHIND)) {
    s32 angle = FACING_SOUTHEAST;

    Actor_SetPositionAndFacing(7, px(304), px(88), angle);
  }
  if (GameFlag_Get(FLAG_AKAFUBU_OFFERED_HELP)) {
    Kibombo_SetStatueTongue(1, 6);
    Actor_SetPosition(ACTOR_AKAFUBU, 0, 0);
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Task_Add(Kibombo_FlickerCeremonyFires, TASK_PASS_VBLANK);
  for (i = 0; i <= 7; i++) {
    u8 *field = &Actor_GetById(i + 18)->retainSpriteCache;
    s32 value = 2;

    *field = value;
  }
  if (GameFlag_Get(FLAG_KIBOMBO_CEREMONY_BEGUN)) {
    if (!GameFlag_Get(FLAG_AKAFUBU_OFFERED_HELP))
      Kibombo_CrowdBows();
  } else if (gMainRoutineWork.entrance == 5) {
    Kibombo_CeremonyBegins();
  }
  Event_End();
  if (gMainRoutineWork.entrance == 99) {
    Kibombo_SetStatueMouth(0, 0);
    Kibombo_SetStatueTongue(0, 0);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 98) {
    Kibombo_SetStatueMouth(1, 1);
    Kibombo_SetStatueTongue(1, 0);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 97) {
    Kibombo_SetStatueMouth(1, 1);
    Kibombo_SetStatueTongue(1, 4);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 96) {
    Kibombo_SetStatueMouth(1, 1);
    Kibombo_SetStatueTongue(1, 4);
    Actor_SetPositionAndResetMotion(gKibombo_BlackCrystal, 0, 0, 0);
    Actor_SetAnimationById(ACTOR_STATUE_TONGUE, 5);
  }
}

#define PALETTE_WORD(index) (*(u16 *)(0x05000000 + (index) * 2))

void Kibombo_InitializeStatueFace(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  gKibombo_FireColors[0] = PALETTE_WORD(169);
  gKibombo_FireColors[1] = PALETTE_WORD(167);
  gKibombo_FireColors[2] = PALETTE_WORD(166);
  if (gMainRoutineWork.entrance == 10) {
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
    Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId), 15);
    Kibombo_StatueFlashes();
  } else if (gMainRoutineWork.entrance == 11) {
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
    Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId), 15);
    Kibombo_GazeAtStatue();
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Task_Add(Kibombo_FlickerStatueFires, TASK_PASS_VBLANK);
    Event_End();
  }
}

void Actor_FallOverById(s32 actor, s32 angle);

void Kibombo_InitializeHouses(void) {
  struct Actor *object;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Kibombo_HideHousesPair();
  if (gMainRoutineWork.entrance == 9) {
    if (GameFlag_Get(FLAG_GABOMBA_CEREMONY_ENDED)) {
      Actor_SetPosition(ACTOR_KRADEN_AT_HOME, 0, 0);
      if (GameFlag_Get(0x8ee)) {
        s32 angle = FACING_NORTH_NORTHWEST;

        Actor_SetPositionAndFacing(ACTOR_AKAFUBU_AT_HOME, px(434), px(634),
                                   angle);
      } else {
        s32 angle = FACING_NORTH_NORTHWEST;

        Actor_SetPositionAndFacing(ACTOR_AKAFUBU_AT_HOME, px(364), px(576),
                                   angle);
      }
    } else {
      Kibombo_CeremonyEnds();
    }
  }
#if TLA_PATCH_1
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    object = Actor_GetById(ACTOR_MAN_WITH_ACHING_BACK);
    object->x.fixed -= px(14);
    object->z.fixed -= px(8);
  }
#endif
  Actor_FallOverById(ACTOR_MAN_WITH_ACHING_BACK, 0);
  object = Actor_GetById(ACTOR_MAN_WITH_ACHING_BACK);
  ((struct FieldSprite *)object->sprite)->rotation = 0;
  object->scaleX = Q16_ONE;
  object->scaleY = Q16_ONE;
  object->x.fixed += px(14);
  object->z.fixed += px(8);
  Actor_SetAnimationById(ACTOR_MAN_WITH_ACHING_BACK, 6);
  if (gMainRoutineWork.entrance == 14)
    GameFlag_Set(0x8ee);
}

void Kibombo_JumpForward(void) { Field_TryJumpForward(); }

void Kibombo_AskAboutFewerWarriors(void);
void Kibombo_AskAboutRevenge(void);
void Kibombo_TalkPathToPeace(void);
void Kibombo_AskGoingWest(void);
void Kibombo_OpenWeaponShop(void);
void Kibombo_OpenArmorShop(void);
void Kibombo_OpenItemShop(void);
void Kibombo_OpenInn(void);
void Kibombo_OpenSanctum(void);
void Kibombo_TalkForeignCeremony(void);
void Kibombo_TalkAchingBack(void);
void Kibombo_ReadMindNeverAgain(void);
void Kibombo_FreezePuddle(void);
void Kibombo_MoveBlockForPiers(void);
void Kibombo_MeetPiers(void);
void Kibombo_PiersThanksParty(void);
void Kibombo_PiersRefusesToLeave(void);
void Kibombo_DeadEndByTheStatue(void);
void Kibombo_TrackLeader(void);
void Kibombo_StopTrackingLeader(void);

// clang-format off
u32 gKibombo_EmberScript[] = {
    SC_WAIT(28),
    SC_SET(SW_HOOK, 0),
    SC_WAIT(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gKibombo_EnterDoorScript[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_ADD(SW_Z, -0x8000),                                                   
      SC_ADD(SW_SCALE_X, -Q16_ONE / 64),                                               
      SC_ADD(SW_SCALE_Y, -Q16_ONE / 64),                                               
      SC_WAIT(1),                                                              
      SC_LOOP(24, SC_TOP),                                                     
      SC_END,                                                                  
  };

u32 gKibombo_KradenMeetsPiersScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(98, 0, 408),                         
      SC_SYNC,     SC_ANIMATION(1),                                                  
      SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),              
      SC_WAIT(1), SC_END,                                                     
  };

u32 gKibombo_JennaMeetsPiersScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(80, 0, 376), SC_SYNC,     SC_ANIMATION(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

s32 gKibombo_ShebaMeetsPiersScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(68, 0, 392),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
    SC_ADD(SW_SCALE_X, 0x800),
    SC_ADD(SW_SCALE_Y, 0x800),
    SC_CALL(Kibombo_SpinSprite),
    SC_WAIT(1),
    SC_LOOP(10, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_CALL(Kibombo_SpinSprite),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_RELEASE,
};

u32 gKibombo_PartyGathersScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(88, 0, 392),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gKibombo_LeaderStepsUpScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(72, 0, 296),
    SC_SYNC,
    SC_MOVE(40, 0, 340),
    SC_SYNC,
    SC_MOVE(40, 0, 356),
    SC_SYNC,
    SC_MOVE(56, 0, 372),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

u32 gKibombo_PiersStepsUpScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(88, 0, 372), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

u32 gKibombo_KradenJoinsPiersScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(48, 0, 384), SC_SYNC,     SC_ANIMATION(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gKibombo_JennaJoinsPiersScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(48, 0, 360), SC_SYNC,     SC_ANIMATION(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gKibombo_ShebaJoinsPiersScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(36, 0, 372), SC_SYNC,     SC_ANIMATION(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gKibombo_PartyFollowsScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(56, 0, 372),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

u32 gKibombo_JennaAtDeadEndScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(298, 0, 200),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(1), SC_END,                                                 
  };

u32 gKibombo_ShebaAtDeadEndScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(308, 0, 188),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(1), SC_END,                                                 
  };

u32 gKibombo_KradenAtDeadEndScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(298, 0, 176),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(1), SC_END,                                                 
  };

u32 gKibombo_PiersAtDeadEndScript[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(222, 0, 184), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

u32 gKibombo_PartyLeavesDeadEndScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(284, 0, 188),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

s32 gKibombo_JawsSettleScript[] = {
    SC_CALL((s32)Kibombo_KeepShakeCentre),
    SC_CALL((s32)Kibombo_ShakeInPlace),
};

s32 gKibombo_JawsOpenScript[] = {
    SC_SET(SW_SCALE_X, 0x18000),
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_ADD(SW_Y, px(-2)),
    SC_WAIT(20),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_Y, px(2)),
    SC_WAIT(1),
    SC_END,
};

s32 gKibombo_CrowdDanceScript[] = {
    22,
    39,
    0,
    46,
    (s32)Kibombo_TurnRandomly,
    22,
    15,
    0x6666,
    22,
    16,
    0x6666,
    (s32)0x80010000,
    39,
    2,
    4,
    0xe60000,
    0,
    0x1e60000,
    1,
    39,
    1,
    0,
    20,
    22,
    3,
    0x5000,
    0,
    40,
    39,
    2,
    4,
    0x1000000,
    0,
    0x1e60000,
    1,
    39,
    1,
    22,
    3,
    0x3000,
    0,
    20,
    22,
    3,
    0x5000,
    0,
    40,
    13,
    0xffff,
    0,
    22,
    15,
    0x6666,
    22,
    16,
    0x6666,
    (s32)0x80010000,
    39,
    2,
    4,
    0x12a0000,
    0,
    0x1e60000,
    1,
    39,
    1,
    0,
    20,
    22,
    3,
    0x3000,
    0,
    40,
    39,
    2,
    4,
    0x1100000,
    0,
    0x1e60000,
    1,
    39,
    1,
    22,
    3,
    0x5000,
    0,
    20,
    22,
    3,
    0x3000,
    0,
    40,
    13,
    0xffff,
    0,
};

s16 gKibombo_FrozenPuddle[] = {
    8, 513, -1, 0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gKibombo_HousesExitRegions[] = {
    400, 40, 368, 416, 56, 384, -1, 14,
    112, 40, 624, 128, 56, 640, -1, 15,
    384, 40, 880, 400, 56, 896, -1, 16,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(Kibombo),
    SCENE_EXIT(1, WorldMap, 27),
    SCENE_EXIT(2, KibomboStatueGrounds, 1),
    SCENE_EXIT(3, KibomboHouses, 1),
    SCENE_EXIT(4, KibomboHouses, 6),
    SCENE_EXIT(5, KibomboHouses, 5),
    SCENE_EXIT(6, KibomboHouses, 4),
    SCENE_EXIT(7, KibomboHouses, 8),
    SCENE_EXIT(8, KibomboHouses, 10),
    SCENE_EXIT(9, KibomboStatueGrounds, 2),
    SCENE_EXIT(10, KibomboStatueGrounds, 5),
    SCENE_EXITS(KibomboCeremony),
    SCENE_EXIT(1, WorldMap, 27),
    SCENE_EXIT(2, KibomboStatueGroundsCeremony, 1),
    SCENE_EXIT(3, KibomboHouses, 1),
    SCENE_EXIT(4, KibomboHouses, 6),
    SCENE_EXIT(5, KibomboHouses, 5),
    SCENE_EXIT(6, KibomboHouses, 4),
    SCENE_EXIT(7, KibomboHouses, 8),
    SCENE_EXIT(8, KibomboHouses, 10),
    SCENE_EXIT(9, KibomboStatueGroundsCeremony, 2),
    SCENE_EXIT(10, KibomboStatueGroundsCeremony, 5),
    SCENE_EXITS(KibomboStatueGrounds),
    SCENE_EXIT(1, Kibombo, 2),
    SCENE_EXIT(2, Kibombo, 9),
    SCENE_EXIT(3, GabombaStatuePinion, 1),
    SCENE_EXIT(4, GabombaStatueChamberIdol, 3),
    SCENE_EXIT(5, Kibombo, 10),
    SCENE_EXITS(KibomboStatueGroundsCeremony),
    SCENE_EXIT(1, KibomboCeremony, 2),
    SCENE_EXIT(2, KibomboCeremony, 9),
    SCENE_EXIT(3, GabombaStatuePinion, 1),
    SCENE_EXIT(4, GabombaStatueChamberIdol, 3),
    SCENE_EXIT(5, KibomboCeremony, 10),
    SCENE_EXIT(10, GabombaStatueArrows, 2),
    SCENE_EXIT(11, GabombaStatueArrows, 3),
    SCENE_EXIT(12, GabombaStatueArrows, 1),
    SCENE_EXIT(13, GabombaStatueChamberIdol, 5),
    SCENE_EXITS(KibomboHouses),
    SCENE_EXIT_UNLESS(1, Kibombo, 3, 0x86b),
    SCENE_EXIT_UNLESS(1, KibomboCeremony, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, KibomboHouses, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, KibomboHouses, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Kibombo, 6, 0x86b),
    SCENE_EXIT_UNLESS(4, KibomboCeremony, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Kibombo, 5, 0x86b),
    SCENE_EXIT_UNLESS(5, KibomboCeremony, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Kibombo, 4, 0x86b),
    SCENE_EXIT_UNLESS(6, KibomboCeremony, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, KibomboHouses, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, Kibombo, 7, 0x86b),
    SCENE_EXIT_UNLESS(8, KibomboCeremony, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, KibomboHouses, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, Kibombo, 8, 0x86b),
    SCENE_EXIT_UNLESS(10, KibomboCeremony, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, KibomboHouses, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, KibomboHouses, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, KibomboHouses, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, KibomboHouses, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, KibomboHouses, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, KibomboHouses, 13, EXIT_FLAG_NONE),
    SCENE_EXITS(KibomboStatueFace),
    SCENE_EXIT(10, KibomboStatueGroundsCeremony, 16),
    SCENE_EXIT(11, KibomboStatueGroundsCeremony, 10),
    SCENE_EXITS_END,
};

struct ActorPlacement gKibombo_ActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_VillageActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(387, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(387, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 300, 0, 228, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 248, 0, 288, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 208, 0, 176, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 288, 0, 364, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(139, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 432, 0, 274, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(140, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 58, 0, 298, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(146, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 102, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(147, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 336, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 246, 0, 488, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 282, 0, 488, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(144, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 430, 0, 442, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(368, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 460, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_CeremonyActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(144, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 104, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(141, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 114, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(139, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0, 152, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(145, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 292, 0, 128, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(145, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 230, 0, 112, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(140, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 304, 0, 220, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(146, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 278, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(147, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 158, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(145, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 132, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(139, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 326, 0, 88, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(140, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 308, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(141, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 278, 0, 146, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(143, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 138, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(144, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 196, 0, 142, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(144, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 224, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 181, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 242, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 274, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 338, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 370, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 412, 0, 60, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_StatueGroundsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(395, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(395, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(396, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_StatueGroundsCeremonyActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(395, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(395, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(396, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 392, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 218, 0, 420, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 432, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 438, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 440, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 438, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 432, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 310, 0, 422, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 406, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 130, 0, 364, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_HousesActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_AKAFUBU_WITCH_DOCTOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 366, 0, 586, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBUS_FATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 586, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 634, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 108, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(145, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 124, 0, 120, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(136, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 100, 0, 132, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 710, 0, 134, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 108, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 432, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 108, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 106, 0, 360, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 352, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(108, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 92, 0, 600, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(109, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 600, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(110, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 348, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(134, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 84, 0, 858, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(135, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 148, 0, 878, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 374, 0, 344, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 448, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gKibombo_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_VillageTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(Kibombo_EnterDoor)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Kibombo_TrackLeader)},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(Kibombo_StopTrackingLeader)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, FLAG_SCENE_TEMP_01,
     EVENT(Kibombo_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 12, 0x9e9, 0},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS,
     EVENT(Kibombo_JumpForward)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS,
     EVENT(Kibombo_JumpForward)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 11,
     CONDITION_ALWAYS, 0},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAkafubuHasFinally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgEveryoneWasTerribly)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Kibombo_AskAboutFewerWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgCoveringOurWarriors)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTheGreatGabomba)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgIWantTo)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgNowThatThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgNowThatTheWitch)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgICouldnT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgBecauseKibomboLies)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgOhISmell)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgYouDonTThink)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgGoodOlMeat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgGulpTheSmell)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWeGetTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgLatelyTheMonsters)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Kibombo_AskAboutRevenge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgNoOneInKibombo)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTheOtherWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgWithAllOur)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, EVENT(Kibombo_TalkPathToPeace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgIWonderIfI)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_CeremonyTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, FLAG_SCENE_TEMP_09,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, FLAG_SCENE_TEMP_09,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, FLAG_SCENE_TEMP_09,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, FLAG_SCENE_TEMP_09,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, FLAG_SCENE_TEMP_09,
     EVENT(Kibombo_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, FLAG_SCENE_TEMP_09,
     EVENT(Kibombo_EnterDoor)},
    {TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Kibombo_TrackLeader)},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(Kibombo_StopTrackingLeader)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, FLAG_SCENE_TEMP_01,
     EVENT(Kibombo_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 12, 0x9e9, 0},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS,
     EVENT(Kibombo_JumpForward)},
    {TRIGGER_TALK, 20, FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgAkafubuSCeremony)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTheyToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgItSStrange)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgAsLongAsThis)},
    {TRIGGER_TALK, 14, FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgDuringTheLast)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAkafubuHasBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgTheGreatGabombaIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgWhatSTaking)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgIMAfraid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS,
     MSGNO(MsgWhenHeFails)},
    {TRIGGER_TALK, 21, FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIHopeThe)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWhewThankGoodness)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgIfTheGreat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgAmIGoing)},
    {TRIGGER_TALK, 17, FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIsMakingThe)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgOnceTheCeremony)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgHeSFailed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgKibomboWasSo)},
    {TRIGGER_TALK, 22, FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgWhenTheGabomba)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgLastTimeAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgIHopeAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuSAlways)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgAkafubuWasA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuHasNot)},
    {TRIGGER_TALK, 15, FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgAkafubuLooksReally)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIThinkAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgHeTravelsTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuIsObsessed)},
    {TRIGGER_TALK, 25, FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIfTheGreatGabomba)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgIBelieveIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgIfTheGreatGabombaActually)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuAndOeia)},
    {TRIGGER_TALK, 18, FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgAkafubuHasTried)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgWhatDonT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgAsAChildAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgIMSoFar)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgHeyNoPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgThereAreSo)},
    {TRIGGER_TALK, 16, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgAkafubuIsAttempting)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTonightSeemsTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgOeiaSSudden)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgWillTheGreat)},
    {TRIGGER_TALK, 23, FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgHeWhoIs)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgOeiaSDeath)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgIfOnlyOeia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgISuspectAkafubu)},
    {TRIGGER_TALK, 19, FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgAkafubuBlamesHis)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTheySayThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgTheyAttackedSome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgTheyReSaying)},
    {TRIGGER_TALK, 24, FLAG_AKAFUBU_OFFERED_HELP, MSGNO(MsgIfTheGreatGabombaS)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAkafubuSeesThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, FLAG_AKAFUBU_OFFERED_HELP,
     MSGNO(MsgIFeelSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgIReallyWonder)},
    {TRIGGER_TALK, 36, CONDITION_ALWAYS, EVENT(Kibombo_TalkForeignCeremony)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 36, CONDITION_ALWAYS,
     MSGNO(MsgIWonderIfA)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_StatueGroundsTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 3,
     IF_FLAG_SET(FLAG_KIBOMBO_VENT_UNCOVERED), EVENT(Kibombo_EnterVent)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoomFading)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, FLAG_SCENE_TEMP_01,
     EVENT(Kibombo_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, FLAG_KIBOMBO_BLOCK_MOVED_FOR_PIERS,
     EVENT(Kibombo_MoveBlockForPiers)},
    {TRIGGER_PUSHED, 0, FLAG_KIBOMBO_BLOCK_MOVED_FOR_PIERS,
     EVENT(Kibombo_MoveBlockForPiers)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10,
     FLAG_KIBOMBO_VENT_UNCOVERED, EVENT(Kibombo_ScoopVent)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 11,
     FLAG_FIRST_MARKING_DUG, EVENT(Kibombo_ScoopFirstMarking)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 12,
     FLAG_SECOND_MARKING_DUG, EVENT(Kibombo_ScoopSecondMarking)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 13,
     FLAG_THIRD_MARKING_DUG, EVENT(Kibombo_ScoopThirdMarking)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 14,
     FLAG_FOURTH_MARKING_DUG, EVENT(Kibombo_ScoopFourthMarking)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_StatueGroundsCeremonyTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 3,
     IF_FLAG_SET(FLAG_KIBOMBO_VENT_UNCOVERED), EVENT(Kibombo_EnterVent)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoomFading)},
    {TRIGGER_STEP, 5, FLAG_KIBOMBO_PIERS_MET, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 5, FLAG_KIBOMBO_PIERS_STAYED_BEHIND,
     EVENT(Kibombo_PiersRefusesToLeave)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, FLAG_SCENE_TEMP_01,
     EVENT(Kibombo_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, FLAG_KIBOMBO_BLOCK_MOVED_FOR_PIERS,
     EVENT(Kibombo_MoveBlockForPiers)},
    {TRIGGER_PUSHED, 0, FLAG_KIBOMBO_BLOCK_MOVED_FOR_PIERS,
     EVENT(Kibombo_MoveBlockForPiers)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10,
     FLAG_KIBOMBO_VENT_UNCOVERED, EVENT(Kibombo_ScoopVent)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 11,
     FLAG_FIRST_MARKING_DUG, EVENT(Kibombo_ScoopFirstMarking)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 12,
     FLAG_SECOND_MARKING_DUG, EVENT(Kibombo_ScoopSecondMarking)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 13,
     FLAG_THIRD_MARKING_DUG, EVENT(Kibombo_ScoopThirdMarking)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 14,
     FLAG_FOURTH_MARKING_DUG, EVENT(Kibombo_ScoopFourthMarking)},
    {TRIGGER_STEP, 20, FLAG_KIBOMBO_PIERS_MET, EVENT(Kibombo_MeetPiers)},
    {TRIGGER_STEP, 22, FLAG_KIBOMBO_PIERS_WITH_PARTY,
     EVENT(Kibombo_PiersThanksParty)},
    {TRIGGER_TALK, 7, CONDITION_ALWAYS, MSGNO(MsgTheOrbIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 7, CONDITION_ALWAYS,
     MSGNO(MsgIAmCertain)},
    {TRIGGER_STEP, 21, FLAG_KIBOMBO_DEAD_END_SEEN,
     EVENT(Kibombo_DeadEndByTheStatue)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAhhSomethingIs)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgYouLookHuman)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAkafubuPossessesThe)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWeMustStand)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgMenOrGods)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgItSeemsThese)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgThisIsnT)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgItIsProper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgTheyLookLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgIfNoPerson)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgISawAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgIHopeAkafubuCompletes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgWhatIfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgLookAtThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgSureThisIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgKoffKoffAll)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_HousesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {STEP_TRIGGER_WALKING, 7, CONDITION_ALWAYS,
     EVENT(Kibombo_StepIntoFirstHole)},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {STEP_TRIGGER_WALKING, 9, CONDITION_ALWAYS,
     EVENT(Kibombo_StepIntoSecondHole)},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING, 13, CONDITION_ALWAYS,
     EVENT(Kibombo_StepIntoThirdHole)},
    {TRIGGER_TALK, 8, 0x8ee, MSGNO(MsgIMGoing)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgIJustCan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgDangWhyDid)},
    {TRIGGER_TALK, 9, 0x8ee, MSGNO(MsgIThinkThat)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMySonHasBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgAfterHeBecame)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgIHaveHeard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgIKnowI)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Kibombo_AskGoingWest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgCrossingTheRiver)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgYouCanSee)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgIfIM)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgIfYouAreLooking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgSureOurLegends)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgEachYearWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgIWonderWhatOeia)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Kibombo_TalkAchingBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     EVENT(Kibombo_ReadMindNeverAgain)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, EVENT(Kibombo_TalkAchingBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS,
     EVENT(Kibombo_ReadMindNeverAgain)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAsSoonAs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuGetsSo)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgManyInGondowan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgItWonT)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgWeThinkIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgIAmAMighty)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Kibombo_OpenWeaponShop)},
    {TRIGGER_EXAMINE, 20, CONDITION_ALWAYS, EVENT(Kibombo_OpenWeaponShop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgOurWarriorsAppear)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(Kibombo_OpenArmorShop)},
    {TRIGGER_EXAMINE, 21, CONDITION_ALWAYS, EVENT(Kibombo_OpenArmorShop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgBecauseOurPaint)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Kibombo_OpenItemShop)},
    {TRIGGER_EXAMINE, 22, CONDITION_ALWAYS, EVENT(Kibombo_OpenItemShop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgIfIHad)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, EVENT(Kibombo_OpenInn)},
    {TRIGGER_EXAMINE, 23, CONDITION_ALWAYS, EVENT(Kibombo_OpenInn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgThatBedMakes)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgICanTRemember)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgILoveKibombo)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, EVENT(Kibombo_OpenSanctum)},
    {TRIGGER_EXAMINE, 24, CONDITION_ALWAYS, EVENT(Kibombo_OpenSanctum)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgICanTBelieve)},
    {TRIGGER_TABLE_END},
};
#undef PALETTE_WORD
#undef DEFINE_KIBOMBO_SET_CROWD_MODE
#undef DEFINE_KIBOMBO_STEP_WHEN_HELD
#undef Actor_SetSpriteFlags
