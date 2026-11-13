/// @file
/// The ship at Alhafra's eastern harbour. The party boards as the mayor of
/// Madra arrives (EasternAlhafraShip_MayorArrives), and Briggs is caught after
/// the battle (EasternAlhafraShip_BriggsIsCaught). Later the baby Eoleo lifts
/// things with Psynergy (EasternAlhafraShip_EoleoLifts), Tremor shakes Large
/// Bread down from a crate (EasternAlhafraShip_ShakeDownTheBread) and Move
/// pushes a crate across the deck.

// ---- games/tla/src/scene/eastern_alhafra_ship.c ----

#include "shared/tables.h"
#include "shared/effects.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "flags.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

extern s16 gEasternAlhafraShip_ActorPlacements[];
void EasternAlhafraShip_BriggsIsCaught(void);
extern u32 gEasternAlhafraShip_FirstPirateRunsScript[];
extern s32 gEasternAlhafraShip_SecondPirateRunsScript[];

void EasternAlhafraShip_ShakeDownTheBread(void);

void Event_PickUpItem();
#include "module/scene/scene.h"
#include "module/field/event_commands.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

s32 Scene_GetPlacements(void) {
  return (s32)gEasternAlhafraShip_ActorPlacements;
}

void EasternAlhafraShip_StartMovingCrate(void) {}

#include "event_inline.h"

void EasternAlhafraShip_FinishMovingCrate(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 24 && z == 40)
    GameFlag_Set(0x900);
  if (x == 23 && z == 40)
    GameFlag_Clear(0x900);
}

void EasternAlhafraShip_ShakeDownTheBread(void) {
  s32 v5;
  struct Actor *object0;
  s32 v6;
  struct Actor *object1;

  GameFlag_Set(0x901);
  v5 = &gMainRoutineWork;
  v5 = (v5 + 0x214);
  object0 = Actor_GetById(*(s32 *)((u8 *)v5 + 0));
  v6 = 192;
  v6 = (v6 << 11);
  *(s32 *)((u8 *)object0 + 40) = v6;
  Actor_SetSpeedParameters(*(s32 *)((u8 *)v5 + 0), 0x20000, 0x10000);
  Actor_WalkTo(*(s32 *)((u8 *)v5 + 0), 136, 488);
  Event_Wait(10);
  Actor_SetDestination(15, 120, 472);
  Actor_MoveToAndWaitById(14, 136, 475);
  object1 = Actor_GetById(15);
  *(s32 *)((u8 *)object1 + 40) = v6;
  Sound_Play(0x237);
  Actor_WaitForMoveById(*(s32 *)((u8 *)v5 + 0));
  Actor_SetModeById(*(s32 *)((u8 *)v5 + 0), 1);
  Actor_ArmCallback(*(s32 *)((u8 *)v5 + 0), FACING_NORTH, 0);
  Event_Wait(20);
  Actor_WaitForMoveById(15);
  Sound_Play(159);
  return;
}

void EasternAlhafraShip_TakeTheBread(s32 arg0) {
  Event_PickUpItem(arg0, 0x1c9, 0x902);
  return;
}

void EasternAlhafraShip_EoleoLifts(s32 id) {
  struct Actor *object = Actor_GetById(12);
  u8 *flags;
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgGooGooGa);
  Event_ShowMessageAndWait(11, 0);
  Field_StartColorTransition(8, 0x1210);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode((s32)Actor_GetById(id), 2);
  Sound_Play(201);
  Event_Wait(30);
  flags = &object->motionFlags;
  *flags = 4;
  for (i = 39; i >= 0; i--) {
    object->y += 0x3333;
    Event_Wait(1);
  }
  Event_Wait(70);
  BattleEffect_SetObjectUpdateMode((s32)Actor_GetById(id), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
  *flags = 3;
  object->gravity = 0x8000;
  Event_Wait(5);
  Sound_Play(132);
  if (Event_GetFlag(0x8ae) == 0) {
    GameFlag_Set(0x8ae);
    Event_Wait(30);
    Actor_ArmCallback(10, 0x3000, 0);
    Event_Wait(30);
    Motion_SetVarCbAndRefresh(10, 2);
    Event_Wait(10);
    Event_ShowMessageThenWait(10, 0, 10);
    Actor_FaceDirection(10, 0xafff, 0);
  }
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

s32 Scene_Initialize(void) {
  struct Actor *object;

  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  if (gMainRoutineWork.entrance == 99) {
    GameFlag_Set(0x8aa);
    GameFlag_Set(FLAG_BRIGGS_CAUGHT);
    gMainRoutineWork.secondary = 3;
    gMainRoutineWork.entrance = 3;
    EasternAlhafraShip_BriggsIsCaught();
  }
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED)) {
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  } else if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    if (!GameFlag_Get(0x8bd)) {
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    }
  } else {
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  }
  if (GameFlag_Get(0x900))
    Actor_SetPosition(13, 0x1880000, 0x2880000);
  Actor_GetById(13)->priorityFlags |= 32;
  Actor_ShowItemIconById(15, 457);
  if (!GameFlag_Get(0x901)) {
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_GetById(14)->priorityFlags |= 128;
      object = Actor_GetById(15);
      object->priorityFlags |= 64;
      object->y += 0x100000;
      object->terrainHeight += 0x100000;
    }
  } else {
    Actor_SetHorizontalPositionWithTerrain(14, 0x880000, 0x1db0000);
    if (!GameFlag_Get(0x902))
      Actor_SetPosition(15, 0x780000, 0x1d80000);
    else
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

extern u32 gEasternAlhafraShip_NoteScript[];

void Actor_SelectAnimation(struct Actor020003e8 *object, s32 mode);
void Actor_StartScript(struct Actor020003e8 *object, const void *script);

void EasternAlhafraShip_SpawnNote(s32 x, s32 y, s32 z, s16 value, s32 fifth) {
  struct Actor020003e8 *object;
  struct Sprite020003e8 *sprite;

  object = ((struct Actor020003e8 *)Import_ActorCreate(0x1e8, x, y, z));
  if (object != 0) {
    sprite = object->sprite;
    Actor_SelectAnimation(object, 1);
    Actor_StartScript(object, gEasternAlhafraShip_NoteScript);
    object->motionFlags = 0;
    /* FAKEMATCH(barrier): an empty asm, so the fifth argument is read from the
     * stack before the zero for nextFrame, as the native does; as no asm, 2 of
     * 32 instructions differ in tla-en. */
    __asm__("");
    object->unk5e = fifth;
    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}

// ---- games/tla/src/scene/eastern_alhafra_ship/s0438.c ----
// Scene code of overlay 0x669, in ROM order, from s0438's offset up to the next
// object the overlay links, and the overlay's data.

/* VIEW(s0438): routine EasternAlhafraShip_SpawnNote as s32
 * EasternAlhafraShip_SpawnNote(s32 x, s32 y, s32 z, s32 rotation, s32 timer);
 * the definition has void EasternAlhafraShip_SpawnNote(s32 x, s32 y, s32 z, s16
 * value, s32 fifth); tried as the definition's void
 * EasternAlhafraShip_SpawnNote(s32 x, s32 y, s32 z, s16 value, s32 fifth),
 * which the object already declares, Actor_EmitNotes moves: 2 of 79
 * instructions in tla-ja. */
extern s32 EasternAlhafraShip_SpawnNote__s0438(
    s32 x, s32 y, s32 z, s32 rotation,
    s32 timer) __asm__("EasternAlhafraShip_SpawnNote");

DEFINE_ACTOR_EMIT_NOTES(Actor_EmitNotes, EasternAlhafraShip_SpawnNote__s0438)

extern u32 gEasternAlhafraShip_LeaveTheDeckScript[];

#include "map.h"
#include "scene_ids.h"

void EasternAlhafraShip_MayorArrives(void) {
  struct MapWork *map;
  s32 result;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgTheMayorOf);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  map = Ram_HeapSlots->mapWork;
  map->minX = 0;
  map->maxX = 0x4000000;
  map->minZ = 0;
  map->maxZ = 0x4000000;
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_WalkToAndWait(4, 456, 408);
  FieldActor_LaunchFromLeader(25, 0, -16, 0x4000);
  FieldActor_LaunchFromLeader(5, 16, 0, 0x4000);
  FieldActor_LaunchFromLeader(6, 16, -16, 0x4000);
  Actor_WaitForMoveById(6);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_ArmCallback(25, FACING_EAST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(25, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Actor_ArmCallback(4, FACING_SOUTHWEST, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(PIXELS(376), -1, PIXELS(416), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(20);
  Actor_FaceDirection(8, 0xb000, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_Jump(9, 6, 15);
  Actor_Jump(9, 6, 23);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(8, 0x100, 40);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(8, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(8, 0x102, 40);
  Event_ShowMessageThenWait(8, 0, 10);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(8, 0xb000, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_ShowEmote(9, 0x102, 40);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_ShowEmote(9, 0x100, 20);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_ShowEmote(9, 0x101, 20);
  Event_ShowMessageThenWait(9, 0, 10);
  Event_ShowMessageThenWait(8, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(8, 0x108, 30);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, 0x102);
  Event_Wait(40);
  Actor_ArmCallback(9, 0x3000, 0);
  Event_Wait(30);
  Actor_EmitNotes(9);
  Sound_Play(151);
  Actor_ShowEmote(9, 0x101, 40);
  Actor_ArmCallback(9, 0xd000, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 10);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_ShowEmote(9, 0x108, 30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(5, 0x103, 30);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_FaceDirection(25, FACING_SOUTHEAST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(25, 0x102);
  Actor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  Event_Wait(30);
  Actor_ArmCallback(8, FACING_EAST, 0);
  Actor_ArmCallback(9, FACING_EAST, 0);
  Actor_ShowEmote(8, 0x100, 0);
  Actor_ShowEmoteById(9, 0x100, 30);
  Event_Wait(20);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_SetSpeed(8, 0x18000, 0xc000);
  Actor_SetSpeed(9, 0x18000, 0xc000);
  Actor_SetMovementTypeById(9, (s32)gEasternAlhafraShip_SecondPirateRunsScript);
  Event_Wait(5);
  Actor_SetMovementTypeById(8, (s32)gEasternAlhafraShip_FirstPirateRunsScript);
  Actor_RefreshSelectorById(8);
  Actor_FaceDirection(25, FACING_SOUTH, 0);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  FieldCamera_MoveWithinMapBounds(PIXELS(448), -1, PIXELS(416), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_Jump(9, 6, 15);
  Actor_Jump(9, 6, 23);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_ShowEmote(8, 0x101, 30);
  Event_ShowMessageThenWait(0x1008, 0, 10);
  Motion_SetVarCbAndRefresh(5, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_ArmCallback(8, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Event_Wait(45);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Actor_ArmCallback(9, 0xb000, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_Wait(10);
  Actor_ArmCallback(25, FACING_EAST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(25, 0x102);
  Event_Wait(40);
  Actor_ArmCallback(5, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(5, 3);
  Event_Wait(10);
  Actor_ArmCallback(25, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(25, 0x102, 40);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Actor_FaceDirection(25, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_ArmCallback(6, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_Jump(25, 6, 15);
  Actor_Jump(25, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Actor_ShowEmoteById(8, 0x103, 50);
  Event_Wait(10);
  Actor_ShowEmote(25, 0x109, 50);
  Event_OpenMessage(0x1019, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result == 0) {
    Event_Wait(20);
    Actor_ArmCallback(5, FACING_WEST, 0);
    Event_Wait(20);
    Event_ShowMessageThenWait(5, 0, 10);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
  } else {
    Func_020032a0();
    Event_Wait(20);
    Actor_ArmCallback(5, FACING_WEST, 0);
    Event_Wait(20);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
    Event_ShowMessageThenWait(5, 0, 10);
  }
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_Jump(9, 6, 15);
  Actor_Jump(9, 6, 23);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1008, 0, 10);
  Actor_WalkBy(8, 0, -16);
  Actor_WalkByById(9, 0, -16);
  Event_Wait(10);
  Event_SetAfterBattleDestination((s32)&SceneId_Ov669, 99);

  gMainRoutineWork.battleRule24b = 2;
  Field_RequestBattleFromEncounterTable(10, 1);
  Event_End();
}

void EasternAlhafraShip_BriggsIsCaught(void) {
  struct Actor *object0;
  struct Actor *object1;
  struct Actor *object2;
  struct Actor *object3;
  struct Actor *object4;
  struct Actor *object5;
  s32 result6;
  struct Actor *result7;
  struct Actor *result8;
  struct Actor *result9;
  struct Actor *object10;
  struct Actor *object11;
  struct Actor *object12;
  struct Actor *object13;
  struct Actor *object14;
  struct Actor *object15;
  struct Actor *object16;
  struct Actor *object17;
  s32 result18;
  s32 result19;
  struct Actor *object20;
  struct Actor *object21;
  struct Actor *object22;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWellThatTakes);
  Actor_SetPosition(8, PIXELS(436), PIXELS(416));
  FieldActor_SetSpriteRotationById(8, 0x4000);
  object0 = Actor_GetById(8);
  Actor_SetSpriteFlags((s32)object0, 0);
  Actor_SetPosition(9, PIXELS(456), PIXELS(432));
  FieldActor_SetSpriteRotationById(9, 0x8000);
  object1 = Actor_GetById(9);
  Actor_SetSpriteFlags((s32)object1, 0);
  object2 = Actor_GetById(9);
  object2->facing = FACING_SOUTH;
  Actor_SetPosition(22, PIXELS(456), PIXELS(460));
  FieldActor_SetSpriteRotationById(22, 0x8000);
  object3 = Actor_GetById(22);
  Actor_SetSpriteFlags((s32)object3, 0);
  Actor_SetPosition(23, PIXELS(420), PIXELS(424));
  Actor_SetSpriteRotationById(23, 0xc000);
  object4 = Actor_GetById(23);
  Actor_SetSpriteFlags((s32)object4, 0);
  Actor_SetPosition(24, PIXELS(388), PIXELS(440));
  Actor_SetSpriteRotationById(24, 0x4000);
  object5 = Actor_GetById(24);
  Actor_SetSpriteFlags((s32)object5, 0);
  Actor_SetPosition(25, PIXELS(456), PIXELS(376));
  Actor_SetPosition(5, PIXELS(472), PIXELS(392));
  Actor_SetPosition(6, PIXELS(472), PIXELS(376));
  Actor_SetPosition(10, PIXELS(456), PIXELS(552));
  Actor_SetPosition(21, PIXELS(472), PIXELS(280));
  Actor_SetPosition(20, PIXELS(440), PIXELS(280));
  Actor_SetPosition(19, PIXELS(456), PIXELS(296));
  Actor_SetPosition(17, PIXELS(472), PIXELS(296));
  Actor_SetPosition(18, PIXELS(440), PIXELS(296));
  Actor_SetPosition(16, PIXELS(456), PIXELS(304));
  Actor_SetSpeed(21, 0x10000, 0x8000);
  Actor_SetSpeed(20, 0x10000, 0x8000);
  Actor_SetSpeed(19, 0x10000, 0x8000);
  Actor_SetSpeed(17, 0x10000, 0x8000);
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_SetSpeed(16, 0x10000, 0x8000);
  Actor_SetSpeed(4, 0x10000, 0x8000);
  Actor_SetSpeed(25, 0x10000, 0x8000);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(6, 0x10000, 0x8000);
  Actor_SetSpeed(10, 0x10000, 0x8000);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_SetSpeed(9, 0x10000, 0x8000);
  Actor_SetSpeed(22, 0x10000, 0x8000);
  Actor_SetSpeed(23, 0x10000, 0x8000);
  Actor_SetSpeed(24, 0x10000, 0x8000);
  FieldCamera_MoveWithinMapBounds(PIXELS(464), -1, PIXELS(408), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(5, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(5, 0, 10);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(6, 0, 10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1019, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 10);
  Actor_FaceDirection(4, 0xbfff, 0);
  Actor_ArmCallback(5, 0xc001, 0);
  Event_Wait(50);
  Actor_ArmCallback(4, FACING_SOUTH, 0);
  Actor_ArmCallback(5, FACING_SOUTH, 0);
  Event_Wait(40);
  Motion_SetModeAndWaitAnimation(8, 4);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1008, 0, 10);
  Motion_SetVarCbAndRefresh(5, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(5, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(10);
  Event_OpenMessage(8, 0);
  result6 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result6 != 0)
    goto L_0200900c;
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(6, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  goto L_0200903c;
L_0200900c:
  Event_Wait(40);
  Motion_SetVarCbAndRefresh(6, 2);
  Event_Wait(10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Event_ShowMessageThenWait(6, 0, 10);
L_0200903c:
  Motion_SetVarCbAndRefresh(25, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(25, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_ShowEmote(6, 0x100, 0);
  Actor_ShowEmote(4, 0x100, 30);
  FieldCamera_FollowActor(10, 1);
  FieldCamera_WaitForArrival();
  Actor_WalkToAndWait(10, 456, 504);
  Actor_WalkByAndWaitById(10, 16, 0);
  Actor_WalkToAndWaitById(10, 472, 424);
  Event_Wait(10);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(408), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_FaceDirection(25, 0x3000, 0);
  Actor_ShowEmote(25, 0x101, 30);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Actor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  Event_Wait(40);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(5, 0x100, 30);
  Event_ShowMessageThenWait(5, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 10);
  Actor_ShowEmote(10, 0x109, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_SetModeById(25, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(6, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(6, 0, 10);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageThenWait(10, 0, 15);
  Actor_FaceDirection(4, 0xbfff, 0);
  Actor_ArmCallback(5, 0xc001, 0);
  Event_Wait(50);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Event_Wait(40);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 10);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(25, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  result7 = (struct Actor *)Actor_ShowEmote(8, 0x107, 0);
  result7->update = 0;
  Actor_SetPositionAndResetMotion(result7, (result7->x + 0x160000),
                                  (result7->y + 0x80000), result7->z);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 10);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(5, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_FaceDirection(10, 0xb000, 0);
  Event_Wait(10);
  Actor_ShowEmote(6, 0x101, 30);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_ArmCallback(25, FACING_EAST, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Actor_FaceDirection(4, 0xd000, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_ShowEmote(5, 0x101, 30);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_FaceDirection(25, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(25, 0x108, 30);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Actor_ShowEmote(5, 0x100, 30);
  Event_ShowMessageThenWait(5, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 10);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirection(25, 0x3000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 4);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 20);
  Actor_WalkByById(16, 0, 8);
  Event_ShowMessageThenWait(17, 0, 10);
  result8 = (struct Actor *)Actor_ShowEmote(8, 0x100, 0);
  result8->update = 0;
  Actor_SetPositionAndResetMotion(result8, (result8->x + 0x160000),
                                  (result8->y + 0x80000), result8->z);
  Actor_ShowEmote(10, 0x100, 0);
  Actor_ShowEmote(4, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_ShowEmote(6, 0x100, 0);
  Actor_ShowEmote(25, 0x100, 50);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(25, FACING_NORTH, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(304), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  Actor_FaceDirection(16, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(16, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(16, 0, 10);
  Actor_ShowEmote(18, 0x101, 30);
  Actor_FaceDirection(18, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_ShowEmote(16, 0x102, 30);
  Event_ShowMessageThenWait(16, 0, 10);
  Actor_ShowEmote(17, 0x107, 30);
  Event_ShowMessageThenWait(17, 0, 10);
  Actor_ShowEmote(19, 0x102, 30);
  Event_ShowMessageThenWait(19, 0, 10);
  Actor_FaceDirection(16, FACING_NORTH, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(16, 0, 10);
  Actor_ShowEmote(19, 0x102, 30);
  Event_ShowMessageThenWait(19, 0, 10);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(16, 0, 10);
  Motion_SetVarCbAndRefresh(19, 2);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(16, 0, 10);
  Motion_SetModeAndWaitAnimation(19, 4);
  Motion_SetVarCbAndRefresh(18, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_FaceDirection(16, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(16, 0x102, 30);
  Event_ShowMessageThenWait(16, 0, 10);
  Actor_ShowEmote(18, 0x101, 0);
  Actor_ShowEmote(19, 0x101, 60);
  Actor_ShowEmote(17, 0x106, 30);
  Event_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirection(16, FACING_NORTHEAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(16, 0, 10);
  Motion_SetVarCbAndRefresh(18, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_FaceDirection(16, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(16, 0, 10);
  Motion_SetVarCbAndRefresh(19, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(19, 0, 10);
  Actor_FaceDirection(16, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_ShowEmote(16, 0x103, 30);
  Event_ShowMessageThenWait(16, 0, 10);
  Actor_StartRepeatedMotion(18, 3);
  Actor_SetAttachedEffect(18, 0x102);
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Actor_ShowEmote(16, 0x100, 0);
  Actor_ShowEmote(17, 0x100, 0);
  Actor_ShowEmote(18, 0x100, 0);
  Actor_ShowEmote(19, 0x100, 30);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirection(18, FACING_SOUTH, 0);
  Actor_FaceDirection(19, FACING_SOUTH, 0);
  Event_Wait(20);
  FieldCamera_MoveWithinMapBounds(PIXELS(464), -1, PIXELS(344), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  Motion_SetVarCbAndRefresh(25, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Actor_WalkByAndWaitById(17, 0, 16);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 10);
  Actor_WalkByById(16, 0, 16);
  Actor_WalkByAndWaitById(18, 0, 16);
  Actor_SetModeById(16, 1);
  Event_Wait(10);
  FieldEvent_ShowTwoMessagesAndWait(18, 13, 1, 13, 4, 16, 17, 11, 22, 6, 0);
  Actor_StartRepeatedMotion(16, 2);
  Motion_SetVarCbAndRefresh(18, 2);
  Event_Wait(20);
  Actor_FaceDirection(18, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(16, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_ShowEmote(17, 0x102, 50);
  Actor_ShowEmote(16, 0x109, 60);
  Actor_ShowEmote(18, 0x108, 50);
  Motion_SetModeAndWaitAnimation(16, 4);
  Actor_ShowEmote(16, 0x108, 50);
  Actor_ShowEmote(18, 0x107, 70);
  Actor_ShowEmote(16, 0x102, 50);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(18, 3);
  Event_Wait(20);
  Actor_FaceDirection(16, 0x3000, 0);
  Actor_FaceDirection(18, 0x3000, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(16, 0, 10);
  Motion_SetModeAndWaitAnimation(18, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Motion_SetVarCbAndRefresh(19, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(19, 0, 10);
  Actor_FaceDirection(25, 0x5000, 0);
  Actor_FaceDirection(6, 0x3fff, 0);
  Event_Wait(60);
  Actor_FaceDirection(25, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Actor_ArmCallback(25, 0x3fff, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_ArmCallback(25, FACING_NORTH, 0);
  Event_Wait(40);
  Motion_SetModeAndWaitAnimation(5, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(5, 0, 10);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_SetModeById(16, 3);
  Actor_SetModeById(17, 3);
  Actor_SetModeById(18, 3);
  Motion_SetModeAndWaitAnimation(19, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1019, 0, 10);
  Actor_StartRepeatedMotion(16, 2);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_Wait(10);
  FieldEvent_ShowTwoMessagesAndWait(17, 12, 1, 25, 4, 16, 9, 11, 14, 6, 0);
  Actor_StartRepeatedMotion(17, 3);
  Actor_SetAttachedEffect(17, 0x102);
  Actor_StartRepeatedMotion(16, 3);
  FieldEffect_SetAttached(16, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(16, FACING_NORTHEAST, 0);
  Actor_ArmCallback(17, 0x5000, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(20);
  Actor_ArmCallback(16, FACING_SOUTH, 0);
  Event_Wait(30);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 10);
  Actor_ShowEmote(17, 0x107, 30);
  Event_ShowMessageThenWait(17, 0, 10);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(432), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_ShowEmote(10, 0x102, 0);
  result9 = (struct Actor *)Actor_ShowEmote(8, 0x105, 0);
  {
    s32 x = result9->x;
    s32 y = result9->y;
    result9->update = 0;
    Actor_SetPositionAndResetMotion(result9, x + 0x160000, y + 0x80000,
                                    result9->z);
  }
  Event_Wait(50);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(344), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(16, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(16, 0, 10);
  Motion_SetModeAndWaitAnimation(16, 4);
  Event_ShowMessageThenWait(16, 0, 10);
  Actor_Jump(18, 6, 15);
  Actor_Jump(18, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(424), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 10);
  Actor_FaceDirection(25, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 10);
  object10 = Actor_GetById(8);
  Actor_SetPosition(8, ((*(s16 *)((u8 *)object10 + 10) << 16) + 0x140000),
                    ((*(s16 *)((u8 *)object10 + 18) << 16) + 0x80000));
  Actor_SetSpriteRotationById(8, 0);
  object11 = Actor_GetById(8);
  Actor_SetSpriteFlags((s32)object11, 1);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Actor_Jump(8, 6, 40);
  Motion_SetModeAndWaitAnimation(8, 4);
  Actor_SetPosition(9, PIXELS(456), PIXELS(440));
  Actor_SetSpriteRotationById(9, 0);
  object12 = Actor_GetById(9);
  Actor_SetSpriteFlags((s32)object12, 1);
  Actor_SetPosition(22, PIXELS(456), PIXELS(456));
  Actor_SetSpriteRotationById(22, 0);
  object13 = Actor_GetById(22);
  Actor_SetSpriteFlags((s32)object13, 1);
  Actor_SetPosition(23, PIXELS(424), PIXELS(424));
  Actor_SetSpriteRotationById(23, 0);
  object14 = Actor_GetById(23);
  Actor_SetSpriteFlags((s32)object14, 1);
  Actor_SetPosition(24, PIXELS(408), PIXELS(440));
  Actor_SetSpriteRotationById(24, 0);
  object15 = Actor_GetById(24);
  Actor_SetSpriteFlags((s32)object15, 1);
  Actor_FaceDirection(9, FACING_NORTH, 0);
  Actor_FaceDirection(22, FACING_NORTH, 0);
  Actor_FaceDirection(23, FACING_NORTH, 0);
  Actor_FaceDirection(24, FACING_NORTH, 0);
  Actor_Jump(22, 6, 0);
  Actor_Jump(23, 6, 0);
  Actor_Jump(24, 6, 0);
  Actor_Jump(9, 6, 40);
  Actor_SetModeById(22, 4);
  Actor_SetModeById(23, 4);
  Actor_SetModeById(24, 4);
  Motion_SetModeAndWaitAnimation(9, 4);
  Event_Wait(10);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(368), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 4);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 10);
  Motion_SetVarCbAndRefresh(18, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(400), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_ShowEmoteById(8, 0x105, 40);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(10);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ArmCallback(8, FACING_EAST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 10);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(20);
  Actor_ShowEmote(8, 0x104, 40);
  Event_ShowMessageThenWait(0x1008, 0, 10);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(30);
  Actor_ArmCallback(8, FACING_SOUTH, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(22, 3);
  Event_Wait(10);
  Actor_ArmCallback(8, FACING_SOUTHWEST, 0);
  Event_Wait(15);
  Actor_SetModeById(23, 3);
  Motion_SetModeAndWaitAnimation(24, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_WalkByAndWait(8, 16, -16);
  Actor_FaceDirection(10, 0xb000, 0);
  Actor_WalkBy(9, 0, -32);
  Actor_WalkBy(22, 0, -32);
  Actor_WalkByById(23, 16, 0);
  Actor_WalkByAndWait(24, 16, -16);
  Actor_WalkBy(5, -16, -32);
  Actor_WalkByAndWait(6, -16, -32);
  Actor_WaitForMoveById(5);
  Actor_SetModeById(5, 1);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallback(25, FACING_EAST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Actor_WalkByById(9, 16, 0);
  Actor_WalkBy(22, 0, -16);
  Actor_WalkByById(23, 16, 0);
  Actor_WalkByById(24, 16, 0);
  Actor_WalkByAndWait(8, 0, -16);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(384), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_FaceDirection(9, FACING_NORTH, 0);
  Actor_FaceDirection(23, FACING_NORTH, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_WalkBy(9, 0, -16);
  Actor_WalkByById(22, 16, 0);
  Actor_WalkBy(23, 0, -16);
  Actor_WalkByById(24, 16, 0);
  Actor_WalkByAndWait(8, 0, -16);
  Actor_WalkBy(9, 0, -16);
  Actor_WalkBy(22, 0, -16);
  Actor_WalkByById(23, 16, 0);
  Actor_WalkBy(24, 0, -16);
  Actor_WalkByAndWait(8, 0, -16);
  Actor_WalkBy(9, 0, -16);
  Actor_WalkBy(22, 0, -16);
  Actor_WalkBy(23, 0, -16);
  Actor_WalkByById(24, 16, 0);
  Actor_WalkByAndWait(8, 0, -16);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(336), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(25, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 10);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(22, 0x107, 30);
  Event_ShowMessageThenWait(22, 0, 10);
  Actor_FaceDirection(8, 0x5000, 0);
  Event_Wait(30);
  Actor_ShowEmote(8, 0x103, 30);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_ShowEmote(22, 0x102, 0);
  Actor_ShowEmote(23, 0x102, 0);
  Actor_ShowEmote(24, 0x102, 0);
  Actor_ShowEmote(9, 0x102, 50);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_FaceDirection(8, 0x5000, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(416), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  Event_Wait(50);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(336), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_ArmCallback(8, 0xb000, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(8, 0, 10);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(20);
  Actor_ArmCallback(17, FACING_EAST, 0);
  Event_Wait(10);
  object16 = Actor_GetById(17);
  object16->actionFlags &= 254;
  Actor_WalkToAndWaitById(17, 456, 312);
  Event_Wait(1);
  object17 = Actor_GetById(17);
  object17->actionFlags |= 1;
  Event_Wait(20);
  Actor_WalkBy(9, 0, -16);
  Actor_WalkBy(22, 0, -16);
  Actor_WalkBy(23, 0, -16);
  Actor_WalkBy(24, 0, -16);
  Actor_WalkByAndWait(8, 0, -16);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Event_Wait(20);
  Actor_ArmCallback(16, FACING_EAST, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(16, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1010, 0, 10);
  Motion_SetModeAndWaitAnimation(18, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_ArmCallback(16, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(30);
  Actor_ArmCallback(16, FACING_EAST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(20);
  Actor_WalkBy(9, 0, -32);
  Actor_WalkBy(22, 0, -32);
  Actor_WalkBy(23, 0, -32);
  Actor_WalkBy(24, 0, -32);
  Actor_WalkByAndWait(8, 0, -32);
  Actor_ArmCallback(19, FACING_EAST, 0);
  Actor_FaceDirection(16, FACING_NORTH, 0);
  Actor_ArmCallback(8, FACING_WEST, 0);
  Event_Wait(20);
  FieldCamera_MoveWithinMapBounds(PIXELS(464), -1, PIXELS(296), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(19, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(19, 0, 10);
  Actor_ShowEmote(8, 0x107, 30);
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_ArmCallback(18, FACING_NORTHEAST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(18, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_StartRepeatedMotion(19, 3);
  Actor_SetAttachedEffect(19, 0x102);
  Event_Wait(40);
  Actor_ArmCallback(19, 0x5000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(19, 3);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(18, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(0x1012, 0, 10);
  Actor_ShowEmote(19, 0x102, 30);
  Event_ShowMessageThenWait(19, 0, 10);
  Actor_ArmCallback(17, FACING_NORTH, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 10);
  Motion_SetModeAndWaitAnimation(19, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(19, 0, 10);
  Actor_SetModeById(17, 3);
  Motion_SetModeAndWaitAnimation(18, 3);
  Actor_SetSpeed(19, 0x1cccc, 0xe666);
  Actor_WalkByAndWait(19, 0, -32);
  Actor_WalkByAndWait(19, -16, 0);
  Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
  Event_Wait(20);
  Actor_WalkByAndWaitById(20, 16, 16);
  Actor_ArmCallback(20, FACING_EAST, 0);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(20, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(20, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(20);
  Actor_SetMovementTypeById(21, gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(8, gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(9, gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(22, gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(23, gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(24, gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(75);
  Actor_WalkByAndWaitById(20, 16, 0);
  Actor_WalkByAndWait(20, 0, -32);
  Actor_WalkByAndWait(20, -32, 0);
  Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
  Event_Wait(30);
  Actor_FaceDirection(18, 0x3000, 0);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Event_Wait(30);
  Motion_SetVarCbAndRefresh(16, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(16, 0, 10);
  Actor_FaceDirection(16, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(16, 0, 10);
  Motion_SetModeAndWaitAnimation(18, 3);
  Event_Wait(10);
  Actor_WalkByAndWaitById(16, 16, 0);
  Actor_WalkBy(16, 0, -32);
  Actor_WalkByAndWait(18, 0, -16);
  Actor_WalkByAndWaitById(18, 16, 0);
  Actor_FaceDirection(18, FACING_NORTH, 0);
  Actor_FaceDirection(17, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_ShowEmote(16, 0x100, 30);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(16, 0, 10);
  FieldCamera_MoveWithinMapBounds(PIXELS(464), -1, PIXELS(320), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(25, 2);
  Event_Wait(10);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirection(18, FACING_SOUTH, 0);
  Actor_WalkByById(4, 16, 0);
  Actor_WalkByAndWaitById(25, 16, 0);
  Actor_WalkBy(4, 0, -32);
  Actor_WalkByAndWait(25, 0, -32);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(16, 0, 10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(20);
  Actor_ShowEmote(16, 0x108, 30);
  Event_ShowMessageThenWait(16, 0, 10);
  Actor_ShowEmoteById(25, 0x101, 50);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(16, 0, 10);
  Actor_ArmCallback(16, FACING_WEST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(16, 3);
  Event_Wait(20);
  Actor_ArmCallback(18, FACING_EAST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(18, 3);
  Event_Wait(10);
  Actor_WalkBy(16, 0, -32);
  Actor_WalkByAndWait(18, 0, -32);
  Actor_WalkBy(16, -32, 0);
  Actor_WalkByAndWait(18, -16, 0);
  Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
  Actor_WaitForMoveById(16);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Event_Wait(20);
  Actor_FaceDirection(17, FACING_NORTH, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirection(17, 0x3000, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 10);
  Actor_SetModeById(25, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(30);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirection(25, 0x3000, 0);
  Actor_ArmCallback(6, 0x4001, 0);
  Event_Wait(30);
  Actor_SetModeById(25, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_FaceDirection(25, FACING_NORTH, 0);
  Actor_ArmCallback(6, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_WalkByAndWaitById(17, -32, -8);
  Event_Wait(20);
  Actor_FaceDirection(17, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_ArmCallback(17, FACING_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(17, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_WalkByAndWait(17, 24, -16);
  Event_Wait(20);
  Actor_FaceDirection(17, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_ArmCallback(17, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_ArmCallback(17, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_WalkByAndWaitById(17, 16, 16);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(17, 0, 10);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 10);
  Motion_SetVarCbAndRefresh(17, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 10);
  Actor_ArmCallback(25, FACING_SOUTH, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(25, 2);
  Event_Wait(10);
  Event_OpenMessage(25, 0);
  result18 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result18 != 0)
    goto L_0200ab7a;
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  Actor_ArmCallback(25, 0xb000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(17, 4);
  Event_ShowMessageThenWait(17, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  goto L_0200abc0;
L_0200ab7a:
  Event_Wait(40);
  Func_020032a0();
  Actor_ArmCallback(25, 0xb000, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(20);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Event_ShowMessageThenWait(17, 0, 10);
L_0200abc0:
  Actor_ShowEmote(17, 0x109, 30);
  Event_ShowMessageThenWait(17, 0, 10);
  Actor_EmitNotes(17);
  Actor_ShowEmote(17, 0x106, 40);
  Event_ShowMessageThenWait(17, 0, 10);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(17, 0, 10);
  Actor_ShowEmote(17, 0x108, 30);
  Event_ShowMessageThenWait(17, 0, 10);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_Wait(20);
  Actor_SetModeById(25, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_WalkByAndWait(17, 0, -40);
  Actor_WalkByAndWait(17, -24, 0);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(400), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  Actor_ShowEmote(10, 0x106, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_FaceDirection(25, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(10, 0x100, 50);
  Actor_StartRepeatedMotion(10, 3);
  FieldEffect_SetAttached(10, 0x102);
  Event_Wait(40);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_SetModeById(25, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_WalkToAndWait(10, 472, 376);
  FieldCamera_FollowActor(10, 1);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_WalkBy(25, -16, -32);
  Actor_WalkByAndWait(4, -16, -32);
  Actor_SetModeById(25, 1);
  Actor_ArmCallback(25, FACING_EAST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Actor_WalkToAndWait(10, 472, 320);
  Actor_WalkBy(5, 16, -32);
  Actor_WalkBy(6, 16, -32);
  Actor_FaceDirection(25, FACING_NORTH, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_WalkToAndWait(10, 472, 264);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_FaceDirection(6, 0xb000, 0);
  Actor_FaceDirection(5, 0xb000, 0);
  Actor_WalkToAndWait(10, 440, 264);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(6, 0, 10);
  Motion_SetVarCbAndRefresh(5, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(5, 0, 10);
  Motion_SetVarCbAndRefresh(6, 2);
  Event_Wait(25);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_Wait(10);
  Actor_ArmCallback(25, FACING_SOUTH, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Event_Wait(20);
  Actor_ShowEmote(25, 0x106, 30);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 10);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_ArmCallback(5, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Actor_ArmCallback(25, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(0x1019, 0, 10);
  Actor_ArmCallback(6, FACING_SOUTHWEST, 0);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(6, 2);
  Event_Wait(10);
  Event_OpenMessage(6, 0);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  result19 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result19 != 0)
    goto L_0200afbe;
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(6, 3);
  Event_Wait(20);
  Event_ShowMessageThenWait(5, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  goto L_0200afe8;
L_0200afbe:
  Event_Wait(40);
  Motion_SetModeAndWaitAnimation(5, 4);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Event_ShowMessageThenWait(5, 0, 10);
L_0200afe8:
  Actor_SetModeById(25, 3);
  Actor_SetModeById(6, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(30);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetModeById(5, 2);
  object20 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object20 == 0)
    goto L_0200b03c;
  Actor_SetDestinationById(5, *(s16 *)((u8 *)object20 + 10),
                           *(s16 *)((u8 *)object20 + 18));
L_0200b03c:
  Actor_WaitForMoveById(5);
  Actor_SetHorizontalPositionWithTerrain(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetModeById(6, 2);
  object21 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object21 == 0)
    goto L_0200b07a;
  Actor_SetDestinationById(6, *(s16 *)((u8 *)object21 + 10),
                           *(s16 *)((u8 *)object21 + 18));
L_0200b07a:
  Actor_WaitForMoveById(6);
  Actor_SetHorizontalPositionWithTerrain(6, 0, 0);
  Actor_SetSpeed(25, 0x13333, 0x9999);
  Actor_SetModeById(25, 2);
  object22 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object22 == 0)
    goto L_0200b0b8;
  Actor_SetDestinationById(25, *(s16 *)((u8 *)object22 + 10),
                           *(s16 *)((u8 *)object22 + 18));
L_0200b0b8:
  Actor_WaitForMoveById(25);
  Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Event_End();
  return;
}

// clang-format off
u32 gEasternAlhafraShip_FirstPirateRunsScript[] = {
    39, 2, 5, 0x200000, 0, 0,
    1, 5, 0, 0, 0x1c0000, 1,
    5, 0x300000, 0, 0, 1, 5,
    0xc0000, 0, 0xc0000, 1, 4, 0x1c80000,
    0, 0x1b80000, 1, 39, 1, 22,
    3, 0xc000, 0, 1, 17,
};

s32 gEasternAlhafraShip_SecondPirateRunsScript[] = {
    39, 2, 5, 0x200000, 0, 0,
    1, 5, 0xc0000, 0, -0xc0000, 1,
    5, 0x340000, 0, 0, 1, 5,
    0xc0000, 0, 0xc0000, 1, 4, 0x1d80000,
    0, 0x1b80000, 1, 39, 1, 22,
    3, 0xb000, 0, 1, 17,
};

u32 gEasternAlhafraShip_LeaveTheDeckScript[] = {
    39, 2, 4, 0x1d80000, 0, 0x1080000,
    1, 4, 0x1b80000, 0, 0x1080000, 1,
    38,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 456, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS(99, 98, 2, 99, 3, 99, 2, 5, 99, 4)

s16 gEasternAlhafraShip_ActorPlacements[] = {
    16, -1, 1, 0, 0, 360, 0, 0,
    0, 396, 0x4000, 1, 39, -1, 1, 0,
    0, 360, 0, 0, 0, 440, 0xc000, 1,
    37, -1, 1, 0, 0, 376, 0, 0,
    0, 88, 0xa000, 1, 38, -1, 1, 0,
    0, 392, 0, 0, 0, 136, 0x8000, 2,
    466, -1, 1, 0, 0, 376, 0, 0,
    0, 128, 0x4000, 1, 287, -1, 1, 0,
    0, 376, 0, 0, 0, 648, 0x4000, 2,
    288, -1, 1, 0, 0, 149, 0, 0,
    0, 475, 0x4000, 2, 489, -1, 1, 0,
    0, 149, 0, 0, 0, 472, 0x4000, 2,
    24, -1, 1, 0, 0, -56, 0, 0,
    0, -56, 0x4000, 1, 20, -1, 1, 0,
    0, -56, 0, 0, 0, -56, 0x4000, 1,
    19, -1, 1, 0, 0, -56, 0, 0,
    0, -56, 0x4000, 1, 88, -1, 1, 0,
    0, -56, 0, 0, 0, -56, 0x4000, 1,
    231, -1, 1, 0, 0, -56, 0, 0,
    0, -56, 0x4000, 1, 231, -1, 1, 0,
    0, -56, 0, 0, 0, -56, 0x4000, 1,
    39, -1, 1, 0, 0, -56, 0, 0,
    0, -56, 0x4000, 1, 39, -1, 1, 0,
    0, -56, 0, 0, 0, -56, 0x4000, 1,
    39, -1, 1, 0, 0, -56, 0, 0,
    0, -56, 0x4000, 1, 56, -1, 1, 0,
    0, -56, 0, 0, 0, -56, 0x4000, 2,
    5, -1, 1, 0, 0, -56, 0, 0,
    0, -56, 0x4000, 2, 6, -1, 1, 0,
    0, -56, 0, 0, 0, -56, 0x4000, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 20, 0x8aa, EVENT(EasternAlhafraShip_MayorArrives)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(EasternAlhafraShip_EoleoLifts)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgEoleoAndI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgIMGonna)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgIfEoleoGets)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_TakeTheBread)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     CONDITION_ALWAYS, EVENT(EasternAlhafraShip_StartMovingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_FinishMovingCrate)},
    {8, 30, CONDITION_ALWAYS, EVENT(EasternAlhafraShip_StartMovingCrate)},
    {9, 30, CONDITION_ALWAYS, EVENT(EasternAlhafraShip_FinishMovingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 14, 0x901,
     EVENT(EasternAlhafraShip_ShakeDownTheBread)},
    {TRIGGER_TABLE_END},
};

u32 gEasternAlhafraShip_NoteScript[] = {
    38,
};
