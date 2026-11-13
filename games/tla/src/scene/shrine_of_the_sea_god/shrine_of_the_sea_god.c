/// @file
/// Scene code of overlay 0x654, in ROM order.

#include "../shrine_of_the_sea_god.h"
#include "event_work.h"
#include "module/scene/scene.h"
#include "module/scene/shrine_of_the_sea_god.h"

void Func_02000280(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *state = &Actor_GetById(14)->state;
  s32 y;

  if ((u8)(*state - 4) > 1)
    return;
  y = leader->y;
  if (y > 0xc0000) {
    GameFlag_Set(0x340);
    if (*state == 4)
      GameFlag_Set(0x341);
    else if (*state == 5)
      GameFlag_Set(0x342);
    Field_RequestDjinnBattle(14, 67);
  }
}

// The scene code of overlay 0x654 that its scene tables belong to, in ROM
// order; the rest of the overlay is in shrine_of_the_sea_god/.

#include "battle.h"
/* VIEW(shrine_of_the_sea_god): routine Actor_SetPositionAndResetMotion as void
 * Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
 * shrine_of_the_sea_god.h has void Actor_SetPositionAndResetMotion(struct
 * ShrineOfTheSeaGodObject *object, s32 x, s32 y, s32 z); tried as the
 * definition's void Actor_SetPositionAndResetMotion(struct Actor *obj, s32 x,
 * s32 y, s32 z), it does not compile in tla-ja: games/tla/include/actor.h:95:
 * conflicting types for `Actor_SetPositionAndResetMotion'. */
struct Actor;
extern void Actor_SetPositionAndResetMotion__shrine_of_the_sea_god(
    struct Actor *object, s32 x, s32 y,
    s32 z) __asm__("Actor_SetPositionAndResetMotion");
#define Actor_SetPositionAndResetMotion                                        \
  Actor_SetPositionAndResetMotion__shrine_of_the_sea_god
/* VIEW(shrine_of_the_sea_god): routine Actor_SetSpritePriority as void
 * Actor_SetSpritePriority(u32, s32); shrine_of_the_sea_god.h has void
 * Actor_SetSpritePriority(s32 actor, s32 variant); tried as the definition's
 * void Actor_SetSpritePriority(u32 objectId, s32 priority), it does not compile
 * in tla-ja: games/tla/include/actor.h:106: conflicting types for
 * `Actor_SetSpritePriority'. */
extern void Actor_SetSpritePriority__shrine_of_the_sea_god(u32, s32) __asm__(
    "Actor_SetSpritePriority");
#define Actor_SetSpritePriority Actor_SetSpritePriority__shrine_of_the_sea_god
/* VIEW(shrine_of_the_sea_god): routine Actor_SetSpriteFlags as void
 * Actor_SetSpriteFlags(struct Actor *object, u32 flags);
 * shrine_of_the_sea_god.h has void Actor_SetSpriteFlags(struct
 * ShrineOfTheSeaGodObject *object, u32 value); tried as the 35 declarations'
 * void Actor_SetSpriteFlags(struct FieldEffect *effect, s32 flags), it
 * does not compile in tla-ja: games/tla/include/field/field.h:25: conflicting
 * types for `Actor_SetSpriteFlags'. */
extern void Actor_SetSingleChildFlags__shrine_of_the_sea_god(
    struct Actor *object, u32 flags) __asm__("Actor_SetSpriteFlags");
#define Actor_SetSpriteFlags Actor_SetSingleChildFlags__shrine_of_the_sea_god
#include "field.h"
#undef Actor_SetSpriteFlags
#undef Actor_SetSpritePriority
#undef Actor_SetPositionAndResetMotion
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "scene_ids.h"

extern const u8 gData_02003df4[];
extern const u8 gData_02003e14[];
extern const u8 gData_02003e44[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShrineOfTheSeaGod9)
    return (const struct SceneRegion *)gData_02003df4;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod10)
    return (const struct SceneRegion *)gData_02003e14;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod11)
    return (const struct SceneRegion *)gData_02003e44;
  return 0;
}

extern const u8 gShrineOfTheSeaGod_Placements[];
extern const u8 gShrineOfTheSeaGod_Room1Placements[];
extern const u8 gShrineOfTheSeaGod_Room2Placements[];
extern const u8 gShrineOfTheSeaGod_Room3Placements[];
extern const u8 gShrineOfTheSeaGod_Room4Placements[];
extern const u8 gShrineOfTheSeaGod_Room5Placements[];
extern const u8 gShrineOfTheSeaGod_Room6Placements[];
extern const u8 gShrineOfTheSeaGod_Room7Placements[];
extern const u8 gShrineOfTheSeaGod_Room8Placements[];
extern const u8 gShrineOfTheSeaGod_Triggers[];
extern const u8 gShrineOfTheSeaGod_Room1Triggers[];
extern const u8 gShrineOfTheSeaGod_Room2Triggers[];
extern const u8 gShrineOfTheSeaGod_Room3Triggers[];
extern const u8 gShrineOfTheSeaGod_Room4Triggers[];
extern const u8 gShrineOfTheSeaGod_Room5Triggers[];
extern const u8 gShrineOfTheSeaGod_Room6Triggers[];
extern const u8 gShrineOfTheSeaGod_Room7Triggers[];
extern const u8 gShrineOfTheSeaGod_Room8Triggers[];
extern const u8 gShrineOfTheSeaGod_Room9Triggers[];

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShrineOfTheSeaGod1)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room1Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod2)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room2Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod3)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room3Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod4)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room4Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod5)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room5Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod6)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room6Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod7)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room7Placements;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod8)
    return (const struct ActorPlacement *)gShrineOfTheSeaGod_Room8Placements;
  return (const struct ActorPlacement *)gShrineOfTheSeaGod_Placements;
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShrineOfTheSeaGod1)
    return (const struct Trigger *)gShrineOfTheSeaGod_Room1Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod2)
    return (const struct Trigger *)gShrineOfTheSeaGod_Room2Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod3)
    return (const struct Trigger *)gShrineOfTheSeaGod_Room3Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod4)
    return (const struct Trigger *)gShrineOfTheSeaGod_Room4Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod5)
    return (const struct Trigger *)gShrineOfTheSeaGod_Room5Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod6)
    return (const struct Trigger *)gShrineOfTheSeaGod_Room6Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod7)
    return (const struct Trigger *)gShrineOfTheSeaGod_Room7Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod8)
    return (const struct Trigger *)gShrineOfTheSeaGod_Room8Triggers;
  if (scene == (s32)&SceneId_ShrineOfTheSeaGod9)
    return (const struct Trigger *)gShrineOfTheSeaGod_Room9Triggers;
  return (const struct Trigger *)gShrineOfTheSeaGod_Triggers;
}

// next object the overlay links.

#include "text/msg_ids.h"
#include "../shrine_of_the_sea_god.h"
#include "io_reg.h"
#include "io_write.h"
#include "call.h"
#include "macros.h"

extern const u8 gData_02003c10[];

/// ★ Named from the game: "This statue looks a lot like the one in Daila's
/// Sanctum." (MsgFieldStatueLikeDailaShrine)
void ShrineOfTheSeaGod_ExamineStatue(void) {
  struct ShrineOfTheSeaGodEventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgFieldStatueLikeDailaShrine, 1);
  work->unk15a = 1;
  Event_End();
}

/// ★ Named from the game: "Wah! You scared me!" (MsgFieldStartledCompanion)
void ShrineOfTheSeaGod_StartleCompanion(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(11);
  ShrineOfTheSeaGod_Jump(11, 0x100, 20);
  Actor_FaceActorById(11, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgFieldStartledCompanion);
  Event_ShowMessageAndWait(11, 0);
  Event_ShowMessageAndWait(11, 0);
  Actor_SetModeById(11, 4);
  Event_ShowMessageThenWait(11, 0, 20);
  Actor_SetMovementTypeById(11, gData_02003c10);
  Event_End();
}

void Func_02000554(void) {
  struct ShrineOfTheSeaGodEventWork *work = Ram_HeapSlots->eventWork;

  work->unk15a = 1;
}

void Func_02000568(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  struct ShrineOfTheSeaGodMapLayer *layer2 = &map->layers[2];
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *camera = FieldCamera_GetActor();
  struct ShrineOfTheSeaGodObject *object8 = Actor_GetById(8);
  struct ShrineOfTheSeaGodObject *object9 = Actor_GetById(9);
  struct ShrineOfTheSeaGodObject *object10 = Actor_GetById(10);

  layer0->y += -0x8000;
  layer1->y += 0x8000;
  layer2->y += 0x8000;
  leader->z += -0x8000;
  camera->y += -0x8000;
  object8->z += -0x8000;
  object9->z += -0x8000;
  object10->z += -0x8000;
  wait(4);
}

void Func_02000618(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer = &map->layers[0];

  layer->y += -0x10000;
  wait(4);
}

/// Copies a 6 by 56 rectangle of cells from (x, y) to (18, 6), then
/// moves the first two layers' y by -0x2000 a frame for 16 frames.
void Func_0200063c(s32 x, s32 y) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  u8 i;

  ShrineOfTheSeaGod_CopyRect(x, y, 6, 56, 18, 6);
  for (i = 0; i < 16; i++) {
    layer0->y += -0x2000;
    layer1->y += -0x2000;
    wait(1);
  }
}

void BlendScript_Suspend(void);
void MapGrid_SetScreenShake(s32 a, s32 b, s32 c);
void Import_EventSetExitRequest(s32 value);

/// The Sea God's Tear (item 458) used on the statue on the island, actor 10,
/// makes the water rise.
/// ★ Named from the game: "Sea God's Tear" (item 458's name)
void ShrineOfTheSeaGod_UseSeaGodsTear(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  BlendScript_Suspend();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  FieldCamera_MoveWithinMapBounds(248 << 16, -1, 168 << 16, 1);
  FieldCamera_WaitForArrival();
  Sound_Play(244);
  Io_Write16(BLEND_ALPHA(1, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(2, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(3, 16), &REG_BLDALPHA);
  wait(10);
  Io_Write16(BLEND_ALPHA(4, 16), &REG_BLDALPHA);
  wait(10);
  Call3((void (*)())MapGrid_SetScreenShake, 0, 0x10000, 0x10000);
  ShrineOfTheSeaGod_CopyRect(59, 124, 9, 12, 12, 3);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpritePriority(8, 1);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  ShrineOfTheSeaGod_CopyRect(46, 51, 6, 56, 18, 25);
  wait(10);
  Call3((void (*)())MapGrid_SetScreenShake, 0x10000, 0x10000, 0x10000);
  Func_0200063c(65, 51);
  Func_0200063c(65, 58);
  Io_Write16(BLEND_ALPHA(5, 15), &REG_BLDALPHA);
  Func_0200063c(65, 65);
  Func_0200063c(65, 72);
  Io_Write16(BLEND_ALPHA(6, 14), &REG_BLDALPHA);
  Func_0200063c(65, 79);
  Func_0200063c(65, 86);
  Io_Write16(BLEND_ALPHA(7, 13), &REG_BLDALPHA);
  Func_0200063c(65, 93);
  Func_0200063c(65, 100);
  Io_Write16(BLEND_ALPHA(8, 12), &REG_BLDALPHA);
  Func_0200063c(84, 51);
  Func_0200063c(84, 58);
  Io_Write16(BLEND_ALPHA(9, 11), &REG_BLDALPHA);
  Func_0200063c(84, 65);
  Func_0200063c(84, 72);
  Io_Write16(BLEND_ALPHA(10, 10), &REG_BLDALPHA);
  Func_0200063c(84, 79);
  Func_0200063c(84, 86);
  Io_Write16(BLEND_ALPHA(11, 9), &REG_BLDALPHA);
  Func_0200063c(84, 93);
  Func_0200063c(84, 100);
  Io_Write16(BLEND_ALPHA(12, 8), &REG_BLDALPHA);
  for (i = 0; i < 24; i++)
    Func_02000618();
  for (i = 0; i < 24; i++)
    Func_02000568();
  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(3);
}

void Func_0200087c(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  struct ShrineOfTheSeaGodMapLayer *layer2 = &map->layers[2];
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *camera = FieldCamera_GetActor();
  struct ShrineOfTheSeaGodObject *object8 = Actor_GetById(8);
  struct ShrineOfTheSeaGodObject *object9 = Actor_GetById(9);
  struct ShrineOfTheSeaGodObject *object10 = Actor_GetById(10);

  layer0->y += 0x8000;
  layer1->y += -0x8000;
  layer2->y += -0x8000;
  leader->z += 0x8000;
  camera->y += 0x8000;
  object8->z += 0x8000;
  object9->z += 0x8000;
  object10->z += 0x8000;
  wait(4);
}

void Func_0200092c(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer = &map->layers[0];

  layer->y += 0x10000;
  wait(4);
}

/// Copies a rectangle of cells from (x, y), then raises map layers 0 and 1 by
/// 0x2000 a frame for 16 frames.
void Func_0200094c(s32 x, s32 y) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  u8 i;

  ShrineOfTheSeaGod_CopyRect(x, y, 6, 56, 18, 6);
  i = 0;
loop:
  layer0->y += 0x2000;
  layer1->y += 0x2000;
  wait(1);
  i++;
  if (i <= 15)
    goto loop;
}

void MapGrid_WaitForShakeToSettle(void);

void Func_02000994(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer0 = &map->layers[0];
  struct ShrineOfTheSeaGodMapLayer *layer1 = &map->layers[1];
  struct ShrineOfTheSeaGodMapLayer *layer2 = &map->layers[2];
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  struct ShrineOfTheSeaGodObject *ride = FieldCamera_GetActor();
  struct ShrineOfTheSeaGodObject *a = Actor_GetById(8);
  struct ShrineOfTheSeaGodObject *b = Actor_GetById(9);
  struct ShrineOfTheSeaGodObject *c = Actor_GetById(10);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpritePriority(8, 1);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  BlendScript_Suspend();
  wait(1);
  ShrineOfTheSeaGod_CopyRect(46, 51, 6, 56, 18, 25);
  ShrineOfTheSeaGod_CopyRect(84, 100, 6, 56, 18, 6);
  ShrineOfTheSeaGod_CopyRect(59, 124, 9, 12, 12, 3);
  Io_Write16(0x80c, &REG_BLDALPHA);
  wait(1);
  layer0->y += -0x440000;
  layer1->y += -0x140000;
  layer2->y += 0xc0000;
  leader->z += -0xc0000;
  ride->y += -0xc0000;
  a->z += -0xc0000;
  b->z += -0xc0000;
  c->z += -0xc0000;
  Map_CenterOnFollowedPoint();
  wait(4);
  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 0x209;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call3((void (*)())MapGrid_SetScreenShake, 0x10000, 0x10000, 0x10000);
  Sound_Play(245);
  i = 0;
  do {
    i++;
    Func_0200087c();
  } while (i <= 24);
  i = 0;
  do {
    i++;
    Func_0200092c();
  } while (i <= 24);
  Func_0200094c(84, 93);
  Func_0200094c(84, 86);
  Io_Write16(0x90b, &REG_BLDALPHA);
  Func_0200094c(84, 79);
  Func_0200094c(84, 72);
  Io_Write16(0xa0a, &REG_BLDALPHA);
  Func_0200094c(84, 65);
  Func_0200094c(84, 58);
  Io_Write16(0xb09, &REG_BLDALPHA);
  Func_0200094c(84, 51);
  Func_0200094c(65, 100);
  Io_Write16(0xc08, &REG_BLDALPHA);
  Func_0200094c(65, 93);
  Func_0200094c(65, 86);
  Io_Write16(0xd07, &REG_BLDALPHA);
  Func_0200094c(65, 79);
  Func_0200094c(65, 72);
  Io_Write16(0xe06, &REG_BLDALPHA);
  Func_0200094c(65, 65);
  Func_0200094c(65, 58);
  Io_Write16(0xf05, &REG_BLDALPHA);
  Func_0200094c(65, 51);
  Call3((void (*)())MapGrid_SetScreenShake, 0, 0x10000, 0x10000);
  Io_Write16(0x1004, &REG_BLDALPHA);
  wait(10);
  Sound_Play(390);
  Call3((void (*)())MapGrid_SetScreenShake, -1, -1, 0xe666);
  Io_Write16(0x1003, &REG_BLDALPHA);
  wait(10);
  Io_Write16(0x1002, &REG_BLDALPHA);
  wait(10);
  Io_Write16(0x1001, &REG_BLDALPHA);
  wait(10);
  Io_Write16(0x1000, &REG_BLDALPHA);
  ShrineOfTheSeaGod_CopyRect(103, 51, 6, 56, 18, 25);
  ShrineOfTheSeaGod_CopyRect(46, 108, 9, 12, 12, 3);
  wait(10);
  MapGrid_WaitForShakeToSettle();
  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 0x204;
  Func_02003854(8);
  Func_02003854(9);
  Func_02003854(10);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  leader->motionFlags = 3;
  leader->y = 0;
  leader->height = 0;
  Event_End();
}

extern const u8 gData_02003cbc[];

/// The leader is placed at y -50 and map layer 1 moved by -50; then both move
/// by 0x8000 in y a frame for 100 frames as a screen transition starts.
void Func_02000ce8(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer;
  struct ShrineOfTheSeaGodObject *leader;
  u32 i;
  s32 frames;

  layer = &map->layers[1];
  leader = Actor_GetById(gMainRoutineWork.currentOwner);

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(244);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  FieldCamera_MoveWithinMapBounds(172 << 17, -1, 248 << 16, 0);
  ShrineOfTheSeaGod_CopyRect(62, 77, 62, 18, 19, 6);
  wait(1);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  leader->motionFlags = 0;
  leader->y = -50 << 16;
  leader->height = -50 << 16;
  layer->y += -50 << 16;
  Map_CenterOnFollowedPoint();
  wait(1);
  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 515;
  Event_StartScreenTransition();
  Call3((void (*)())MapGrid_SetScreenShake, 0x10000, 0x10000, 0x10000);
  // A goto loop: as a for, while or do loop, loop.c hoists the 0x8000 the
  // bytes rebuild on every pass (differ 64).
  i = 0;
loop:
  leader->y += 0x8000;
  frames = 2;
  leader->height += 0x8000;
  /* FAKEMATCH(barrier): wait()'s argument is set before the count; as
   * wait(2), 4 of 142 instructions differ in tla-en. */
  asm volatile("" : "+l"(frames));
  i++;
  /* FAKEMATCH(barrier): the layer is read after the count; without it, 4 of
   * 142 instructions differ in tla-en. */
  asm("" : "+l"(layer) : "l"(i));
  layer->y += 0x8000;
  wait(frames);
  if (i <= 99)
    goto loop;
  Sound_Play(288);
  Sound_Play(390);
  MapGrid_SetScreenShake(0, 0, 0);
  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 521;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  ShrineOfTheSeaGod_CopyRect(62, 69, 62, 18, 19, 6);
  wait(1);
  Event_End();
}

/// The leader turns, then moves with map layer 1 by -0x8000 in y a frame for
/// 100 frames; at frame 90 the screen transition finishes.
void Func_02000e38(void) {
  struct ShrineOfTheSeaGodMapWork *map =
      (struct ShrineOfTheSeaGodMapWork *)Ram_HeapSlots->mapWork;
  struct ShrineOfTheSeaGodMapLayer *layer;
  struct ShrineOfTheSeaGodObject *object;
  u32 i;

  layer = &map->layers[1];
  object = Actor_GetById(gMainRoutineWork.currentOwner);

  Event_Start();
  Event_ResolvePendingActions(0);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  FieldCamera_MoveWithinMapBounds(344 << 16, -1, 248 << 16, 1);
  ShrineOfTheSeaGod_CopyRect(62, 77, 62, 18, 19, 6);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  object->motionFlags = 0;
  Sound_Play(245);
  Call3((void (*)())MapGrid_SetScreenShake, 0x10000, 0x10000, 0x10000);
  // A goto loop: the bytes reload the step on every pass, and a for loop
  // hoists it (differ 30).
  i = 0;
loop:
  object->y -= 0x8000;
  object->height -= 0x8000;
  layer->y -= 0x8000;
  wait(2);
  if (i == 90) {
    ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
        ->screenEffect = 0x203;
    Event_FinishScreenTransition();
  }
  i++;
  if (i <= 99)
    goto loop;
  Import_EventSetExitRequest(1);
}

void Func_02000f18(s32 actor) {
  Actor_DisableById(actor);
  Actor_GetById(actor)->priorityFlags |= 0x20;
}

/// Frost on the puddle, actor 10, makes an ice pillar (flag 0x200).
void ShrineOfTheSeaGod_FreezeFirstPuddle(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 8, 18);
  GameFlag_Set(0x200);
}

/// Frost on the puddle, actor 11, makes an ice pillar (flag 0x201).
void ShrineOfTheSeaGod_FreezeSecondPuddle(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 18);
  GameFlag_Set(0x201);
}

/// Frost on the puddle, actor 12, makes an ice pillar (flag 0x202).
void ShrineOfTheSeaGod_FreezeThirdPuddle(void) {
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 10, 17);
  GameFlag_Set(0x202);
}

void Func_02000fa8(void) { Actor_LiftTargetBlock(gData_02003cbc); }

void Func_02000fb8(void) {
  struct ShrineOfTheSeaGodObject *object;

  Actor_PlaceMovedBlock();
  object = Actor_GetById(8);
  if (object->x >> 20 == 21 && object->z >> 20 == 13)
    GameFlag_Set(0x207);
  else
    GameFlag_Clear(0x207);
}

void Func_02000fec(void) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(8);

  Actor_PlaceMovedBlock();
  if (object->x >> 20 == 12)
    GameFlag_Set(0x84c);
}

void Func_02001010(struct ShrineOfTheSeaGodObject *object) {
  object->motionFlags = 3;
  do {
    wait(1);
  } while (object->speed != 0);
  Sound_Play(188);
  wait(10);
  object->motionFlags = 0;
}

void Func_02001038(void) {
  struct ShrineOfTheSeaGodObject *object = Actor_GetById(8);
  s32 z = object->z >> 20;

  if (z == 16) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Func_02001010(object);
    Map_CopyCellAttributeRect(0, 0, 1, 1, 15, z);
    Actor_SetSpritePriority(8, 3);
    Event_End();
    GameFlag_Set(0x301);
  }
}

void Func_02001088(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  u32 id;

  for (id = 9; id <= 13; id++) {
    struct ShrineOfTheSeaGodObject *object = Actor_GetById(id);

    if (leader->y > 0x100000) {
      Actor_SetSpritePriority(id, 2);
      object->priorityFlags |= 2;
    } else {
      Actor_SetSpritePriority(id, 1);
      object->priorityFlags &= ~2;
    }
  }
}

void Func_020010e8(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);

  Actor_GetCellIndex();
  if (leader->y < 0x100000)
    gMainRoutineWork.unk2c3 = 0;
  else
    gMainRoutineWork.unk2c3 = 1;
}

void Func_02001128(void) {
  struct ShrineOfTheSeaGodObject *leader =
      Actor_GetById(gMainRoutineWork.currentOwner);
  s32 y;
  s32 z;

  y = leader->y;
  if (y < 0x100000) {
    z = leader->z;
    if (z < 0x1280000) {
      Actor_SetSpritePriority(10, 1);
      Actor_SetSpritePriority(11, 1);
      Actor_SetSpritePriority(12, 2);
    } else {
      Actor_SetSpritePriority(10, 2);
      Actor_SetSpritePriority(11, 2);
      Actor_SetSpritePriority(12, 3);
    }
  } else {
    Actor_SetSpritePriority(10, 2);
    Actor_SetSpritePriority(11, 2);
    Actor_SetSpritePriority(12, 3);
  }
}

void Func_020011a0(void) {
  struct ShrineOfTheSeaGodObject *first = Actor_GetById(8);
  struct ShrineOfTheSeaGodObject *second = Actor_GetById(9);

  SET_POSITION(second, first->x, 0x140000, first->z);
}

void Func_020011c0(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags &= ~0x20;
}

void Func_020011e0(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
}

void Func_02001250(s32 show, s32 first, u32 count);

void Func_02001200(s32 show) {
  if (gMainRoutineWork.scene == (s32)&SceneId_ShrineOfTheSeaGod6)
    Func_02001250(show, 9, 3);
}

void Func_02001228(void) {
  Func_02001200(1);
  GameFlag_Set(0x208);
}

void Func_0200123c(void) {
  Func_02001200(0);
  GameFlag_Clear(0x208);
}

void Func_02001250(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct ShrineOfTheSeaGodObject *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~0x10;
    else
      actor->priorityFlags |= 0x10;
  }
}

void Func_02001290(void) {
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  GameFlag_Set(0x20a);
}

void Func_020012b4(void) {
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  GameFlag_Clear(0x20a);
}
