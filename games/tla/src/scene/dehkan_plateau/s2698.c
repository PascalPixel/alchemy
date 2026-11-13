/// @file
/// Scene code of overlay 0x655, in ROM order.

// ---- games/tla/src/scene/dehkan_plateau/s2698.c ----

#include "types.h"
#include "system.h"
#include "actor.h"
#include "field_sprite.h"
#include "fixed_math.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "event.h"
#include "field.h"
#include "flag_call.h"
#include "gamepak/far_call/system.h"
#include "heap_release.h"
#include "module/field/event_commands.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "scene_call.h"

void Func_02003cc0(s32 id);
extern struct Actor *gData_02004420;

void Actor_StartScript(struct Actor *object, const void *script);
/* VIEW(s2698): routine FieldEffect_AdvanceMotion as void FieldEffect_AdvanceMotion(struct
 * Actor *object); module/field/field_effect.h has void
 * FieldEffect_AdvanceMotion(union FieldObject *object); tried as the definition's
 * void FieldEffect_AdvanceMotion(union FieldObject *object), which the object
 * already declares, it does not compile in tla-ja:
 * games/tla/src/scene/dehkan_plateau/s2698.c:75: `FieldEffect_AdvanceMotion'
 * undeclared (first use in this function). */
extern void Effect_AdvanceMotion__s2698(struct Actor *object) __asm__(
    "FieldEffect_AdvanceMotion");

extern const u8 gData_02003f14[];

/// Flashes actor id, then raises eight puffs over actor center, then sets
/// flag and waits 20 frames.
void Func_02002698(s32 id, s32 center, s32 flag) {
  struct Actor *leader;
  s32 frame;
  u32 i;

  Sound_Play(125);
  Actor_SetSpriteColor(Actor_GetById(id), 7);
  wait(2);
  Actor_SetModeById(id, 0);
  Actor_SetSpriteColor(Actor_GetById(id), 7);
  wait(4);
  Actor_SetSpriteColor(Actor_GetById(id), 0);
  leader = Actor_GetById(center);
  frame = 0;
  for (i = 0; i <= 7; i++) {
    struct Actor *object;
    struct EffectView *effect;
    s32 y;

    /* FAKEMATCH(temporary): the form kept; as one call argument, 10 of 129
     * instructions differ in tla-en. */
    y = leader->y - ((((Random16Far() * 7) << 1) >> 16) << 16);
    object = ((struct Actor *)Import_ActorCreate(
        285, (i << 17) + leader->x - Q16_ONE * 8, y - Q16_ONE * 2, leader->z));
    if (object != NULL) {
      effect = (struct EffectView *)object;
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      object->motionFlags = 0;
      object->collisionFlags = 0;
      Actor_SetSingleChildFlags(object, 0);
      Func_02003ba0(object, 2);
      Actor_StartScript(object, gData_02003f14);
      object->scaleX = 0xb333;
      object->scaleY = 0xb333;
      effect->sprite->priority = 1;
      effect->velocityX = 0;
      effect->velocityZ = 0;
      effect->velocityY =
          -((((u32)Random16Far() << 14) >> 16) + Q16_ONE * 3 / 8);
      effect->spin = (((u32)Random16Far() << 9) >> 16) - 256;
      effect->scaleRateX = -(Q16_ONE / 32);
      effect->scaleRateY = -(Q16_ONE * 3 / 128);
      effect->update = Effect_AdvanceMotion__s2698;
    }
  }
  GameFlag_SetBit(flag);
  Event_Wait(20);
}

void Func_020027e0(void) {
  struct Actor *actor;

  actor = Actor_GetById(25);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 25);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(616), -1, PIXELS(280), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(25, gMainRoutineWork.currentOwner, 0);
  // A direct call: as Call3 the actor id is loaded before the 0x100's shift.
  Actor_ShowEmoteById(25, 0x100, 40);
  Actor_SetSpritePriority(25, 1);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_WalkToAndWait(25, 632, 280);
  Actor_SetSpeed(25, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 664, 280);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Func_02002698(21, 25, 0x210);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 696, 280);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Event_Wait(10);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_WalkToAndWait(25, 712, 280);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  FieldCamera_MoveWithinMapBounds(PIXELS(712), -1, PIXELS(136), 1);
  MapGrid_CopyCellPictureIndicesRect(112, 6, 110, 4, 1, 1);
  Actor_SetSpritePriority(25, 1);
  Actor_WalkToAndWait(25, 712, 200);
  Actor_WalkToAndWaitById(25, 712, 168);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_FaceDirectionById(25, FACING_EAST);
  Actor_SetSpeedParameters(25, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 744, 168);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Func_02002698(24, 25, 0x216);
  Actor_FaceDirectionById(25, FACING_NORTH);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 744, 136);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Event_Wait(10);
  Actor_WalkToAndWait(25, 744, 72);
  MapGrid_CopyCellPictureIndicesRect(66, 51, 110, 4, 1, 1);
  Sound_Play(229);
  Func_020021b4();
  Event_Wait(10);
  Actor_Jump(25, 6, 16);
  Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  Event_Wait(20);
  GameFlag_SetBit(0x870);
  Event_End();
}

void Func_02002a5c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  {
    s32 facing = FACING_NORTH;

    Actor_SetHorizontalPositionAndAngleWithTerrain(
        gMainRoutineWork.currentOwner, PIXELS(248), PIXELS(136), facing);
  }
  MapGrid_CopyCellPictureIndicesRect(65, 64, 15, 8, 1, 1);
  GameFlag_SetBit(0x245);
  Sound_Play(161);
  Func_020021b4();
  Actor_FaceActorById(9, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(9, 0x100, 20);
  Func_02002bcc();
  Event_Wait(20);
  leader->facing = FACING_SOUTH;
  leader->motionFlags = 0;
  Actor_SetSpritePriority(9, 3);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  Event_Wait(16);
  for (i = 0; i < 40; i++) {
    if (i == 5)
      Sound_Play(204);
    leader->scaleX -= 0x400;
    leader->scaleY -= 0x300;
    leader->y -= 0x9999;
    wait(1);
  }
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(23);
}

void Func_02002b68(void) {
  if (((struct EffectWork7c4 *)Ram_HeapSlots->effectWork)->active != 0) {
    Sound_Play(229);
    MapGrid_CopyCellPictureIndicesRect(64, 64, 15, 8, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(64, 66, 15, 72, 1, 2);
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  }
}

// ---- games/tla/src/scene/dehkan_plateau/events_i.c ----
extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02002180(void);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                    s32 width, s32 height);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Func_02001804(void);

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);
/* VIEW(s2698): routine ItemDisplay_ShowPicture as void
 * ItemDisplay_ShowPicture(struct Actor *object, s32 mode);
 * module/scene/airs_rock_inside.h has void
 * ItemDisplay_ShowPicture(void); tried as the definition's void
 * ItemDisplay_ShowPicture(void), which the object already declares,
 * Func_02002f9c moves: 1 of 34 instructions in tla-ja. */
extern void
Func_02003d40__s2698(struct Actor *object,
                     s32 mode) __asm__("ItemDisplay_ShowPicture");

s32 PartyInventory_GiveItem(s32 item, s32 unused);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 id, s32 x, s32 z);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_WatchAndSetScript(s32 id, s32 target);
/* VIEW(events_i): routine Task_Add as void Task_Add(void(*func) (void), s32
 * priority); task.h has s32 Task_Add(TaskFunc func, s32 order); tried as the
 * definition's s32 Task_Add(TaskFunc func, s32 order), which the object already
 * declares, Func_02002bcc moves: 1 of 24 instructions in tla-ja. */
extern void Task_Add__events_i(void (*func)(void),
                               s32 priority) __asm__("Task_Add");
void Task_RemoveFar(void (*func)(void));

void Func_02002bcc(void) {
  BattleEffect_LoadActionEffectResources(134, 1);
  Task_Add__events_i(Func_02002b68, 0x480);
  FieldPsynergy_SetupActorPair(9, gMainRoutineWork.currentOwner);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_RemoveFar(Func_02002b68);
  BattleEffect_CleanupSceneObjects();
}

// ---- games/tla/src/scene/dehkan_plateau/s2c14.c ----
// Scene code of overlay 0x655, in ROM order, from s2c14's offset up to the next
// object the overlay links.

#include "map.h"
#include "iwram_call.h"
#include "scene_ids.h"
#include "field.h"

void Func_020006ec(struct Actor *object, struct Actor *other);
void Actor_MoveToObject(s32 id, s32 target);


void Func_02002c14(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *helper = Actor_GetById(9);

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
  leader->motionFlags = 3;
  Event_Wait(24);
  Sound_Play(176);
  Func_020021b4();
  if (gData_02004420 != NULL) {
    FieldActor_SetPositionAndResetMotion(gData_02004420, 0xf80000, 0, 0x780000);
    wait(1);
    gData_02004420->speedLimit = 0x19999;
    gData_02004420->acceleration = 0x19999;
    gData_02004420->velocityY = 0x80000;
  }
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  leader->velocityY = 0x60000;
  helper->velocityY = 0x60000;
  FieldActor_SetMoveTarget(leader, 0xd80000, 0, 0x780000);
  FieldActor_SetMoveTarget(helper, 0x1180000, 0, 0x780000);
  if (gData_02004420 != NULL)
    FieldActor_SetMoveTarget(gData_02004420, 0xf80000, 0, 0x580000);
  Actor_WaitForMove(leader);
  Event_Wait(20);
  Actor_ShowEmoteById(9, 0x103, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(9, 0x8000, 60);
  Actor_WalkToAndWait(9, 296, 104);
  Actor_WalkToAndWait(9, 296, 80);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Event_Wait(20);
  if (gData_02004420 != NULL) {
    gData_02004420->motionFlags = 0;
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 15, 5);
    wait(1);
  }
  GameFlag_SetBit(0x871);
  Event_End();
}

void Func_02002dd4(void) {
  struct Actor *actor;

  actor = Actor_GetById(16);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveWithinMapBounds(PIXELS(760), PIXELS(32), PIXELS(204), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(16, gMainRoutineWork.currentOwner, 0);
  // Called for its value, which is unused: as Call3 the actor id is loaded
  // before the 0x100's shift.
  Actor_ShowEmote(16, 0x100, 40);
  Actor_SetSpeedParameters(16, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_WalkToAndWaitById(16, 760, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Event_Wait(20);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_WalkToAndWaitById(16, 792, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Func_02002698(9, 16, 0x210);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_WalkToAndWaitById(16, 824, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Event_Wait(20);
  Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Event_Wait(20);
  Event_End();
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_SetAngle(s32 id, s32 angle) {
  Actor_FaceDirectionById(id, angle);
}

void Func_02002f08(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(16, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(16, 0x100, 40);
  Actor_FaceDirectionById(16, FACING_EAST);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(16, 3);
  Actor_SetAngle(16, FACING_WEST);
  Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
  FieldEffect_SetAttached(16, 0x102);
  Event_Wait(40);
  Event_End();
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Func_02002f74(void) {
  if (GameFlag_Test(0x207) != 0)
    Func_02002f08();
  else
    Func_02002dd4();
  GameFlag_SetBit(0x209);
}

void Func_02002f9c(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (gData_02004420 != 0)
    Func_02003d40__s2698(gData_02004420, 3);
  PartyInventory_GiveItem(199, 0);
  if (gData_02004420 != 0)
    Import_ActorDestroy(gData_02004420);
  Map_CopyAttributes(1, 0, 1, 1, 15, 5);
  GameFlag_SetBit(0x878);
  Event_End();
}

void Func_02002ff4(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov655Room1)
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(*(s16 *)(work + 0x154));
}

s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    Func_020030e4();
  else if (scene == (s32)&SceneId_Ov655Room2)
    Func_02003164();
  else if (scene == (s32)&SceneId_Ov655Room3)
    Func_02003284();
  else if (scene == (s32)&SceneId_Ov655Room4)
    Func_0200369c();
  else if (scene == (s32)&SceneId_Ov655Room5)
    Func_0200390c();
  else if (scene == (s32)&SceneId_Ov655Room6)
    Func_02003aa0();
  return 0;
}

void Func_020030c0(void) {
  Actor_SetSpritePriority(64, ((struct AnimationFacing *)Actor_GetById(
                                   gMainRoutineWork.currentOwner)
                                   ->sprite)
                                  ->facing);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_02000038(const void *data);
void Func_02001820(void);

void Func_020030e4(void) {
  Func_020001d0(gData_02003f44);
  Actor_SetSpritePriority(8, 3);
  Func_02000038(gData_02003f98);
  Func_02003cc0(12);
  if (GameFlag_Test(0x301) != 0) {
    Map_CopyAttributes(54, 32, 2, 1, 56, 64);
    Actor_SetPositionAndResetMotion(Actor_GetById(12), 226 << 18, 128 << 15,
                                    130 << 18);
    wait(10);
  }
  Func_02001820();
  Func_02001f94();
  {
    s32 priority = 0x480;

    Task_Add(Func_020030c0, priority);
  }
}

// ---- games/tla/src/scene/dehkan_plateau/s3164.c ----

void Func_02000584(const void *data);
void Func_020018a8(void);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                    s32 width, s32 height);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

/* VIEW(s3164): static Actor_SetPosition as Actor_SetPosition__s3164; the
 * definition has static inline void Actor_SetPosition(s32 actorId, s32 fixed_x,
 * s32 fixed_z); tried with the definition the rest of the object has, it does
 * not compile in tla-ja: games/tla/src/scene/dehkan_plateau/s2698.c:527: too
 * many arguments to function `Actor_SetPosition'. */
static __inline__ void Actor_SetPosition__s3164(struct Actor *object, s32 x,
                                                s32 y, s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

static __inline__ void Actor_PlaceFacing(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

void Func_02003164(void) {
  if (GameFlag_Test(265) == 0) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
    if (gMainRoutineWork.entrance == 5) {
      Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
    }
  }
  Func_02000584(gData_02003f52);
  Func_020012d8();
  Func_02000038(gData_02003fa0);
  if (GameFlag_Test(0x840) != 0) {
    Func_02003cc0(23);
    Map_CopyAttributes(50, 7, 1, 1, 52, 7);
    Actor_SetPosition__s3164(Actor_GetById(23), 210 << 18, 0, 240 << 15);
  } else {
    Func_02003db8(gData_02003fc0);
  }
  Func_020018a8();
  if (GameFlag_Test(0x86c) != 0) {
    s32 angle = 0x4000;

    Actor_PlaceFacing(24, 222 << 18, 168 << 16, angle);
    MapGrid_CopyCellPictureIndicesRect(68, 33, 105, 6, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(69, 33, 105, 4, 1, 1);
    GameFlag_SetBit(583);
  }
  if (GameFlag_Test(0x86d) != 0)
    Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Func_02001fac();
}

// ---- games/tla/src/scene/dehkan_plateau/s3284.c ----
// Scene code of overlay 0x655, in ROM order, from s3284's offset up to the next
// object the overlay links.

#include "task.h"
#include "field_effect.h"

extern struct Actor *gData_02004420;

extern const u8 gData_02003fc8[];
extern s32 gData_02005678;

void Pillars_Raise(const void *data);
void Func_02001ee8(void);
/* VIEW(s3284): routine Func_02003db0 as s32 Func_02003db0(void); s2c14 has s32
 * Func_02003db0(struct Actor *object); tried as the rest of the object declares
 * it, it does not compile in tla-ja:
 * games/tla/src/scene/dehkan_plateau/s2698.c:617: too few arguments to function
 * `Func_02003db0'. */
extern s32 Func_02003db0__s3284(void) __asm__("Func_02003db0");

// The room entry written in the scene layer of Func_02003164 and Func_0200390c:
// sets actors' variants, links, cells and flags for the room.
void Func_02003284(void) {
  struct Actor *object;
  s32 zero, value;

  if (GameFlag_Test(265) == 0) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
  Actor_SetSpritePriority(8, 1);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  Actor_SetSpritePriority(11, 1);
  /* FAKEMATCH(dead): an empty once-only block ends the argument loads' tie; as
   * no block, 2 of 276 instructions differ in tla-en. */
  do {
  } while (0);
  Func_02003d98();
  Func_02003da0(0, 8, 9);
  Func_02003da0(1, 10, 11);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  Func_02003d90();
  Func_02003da8(0, 520, 12, 13);
  Func_02003db8(gData_02003fc4);
  if (Actor_GetById(19)->y > (s32)0xfff80000)
    Actor_SetSpritePriority(19, 1);
  else
    Actor_SetSpritePriority(19, 2);
  Func_02003cc0(20);
  Actor_GetById(20)->priorityFlags |= 32;
  Actor_SetSpritePriority(20, 2);
  if (GameFlag_Test(0x841) != 0)
    Func_0200172c();
  else
    MapGrid_CopyCellAttributes(38, 32, 1, 1, 38, 33);
  Actor_GetById(14)->priorityFlags &= ~1;
  Actor_GetById(15)->priorityFlags &= ~1;
  Actor_GetById(16)->priorityFlags &= ~1;
  Actor_GetById(17)->priorityFlags &= ~1;
  Pillars_Raise(gData_02003fc8);
  value = Actor_GetById(17)->x;
  zero = 0;
  if ((value >> 20) == 54) {
    struct MapCell *cell = Ram_HeapSlots->mapWork->layers[0].cells;

    cell += Func_02003db0__s3284();
    cell->attributeB &= 0x7f;
    cell->collisionCode = 11;
  }
  Actor_SetSpritePriority(14, 1);
  Actor_SetSpritePriority(15, 1);
  Func_02001930();
  if (GameFlag_Test(0x86e) != 0) {
    s32 angle = 0x4000;

    Actor_SetPositionAndFacing(25, 212 << 17, 208 << 15, angle);
  }
  Actor_SetModeById(18, 3);
  if (GameFlag_Test(0x86f) != 0) {
    object = Actor_GetById(18);
    MapGrid_CopyCellAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetModeById(18, 4);
    object->priorityFlags |= 2;
    *((u8 *)object + 89) &= ~1;
    GameFlag_SetBit(517);
    GameFlag_SetBit(577);
    MapGrid_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
    {
      s32 angle = 0x4000;

      Actor_SetPositionAndFacing(25, 154 << 18, 140 << 17, angle);
    }
  }
  if (GameFlag_Test(0x870) != 0) {
    GameFlag_SetBit(528);
    GameFlag_SetBit(534);
    Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  }
  Func_020001d0(gData_02003f80);
  Func_020013b0();
  Func_02001fc4();
  {
    struct Actor *last = Actor_GetById(27);

    *((u8 *)last + 85) = zero;
    last->y = zero;
  }
  gData_0200567c = zero;
  gData_02005678 = zero;
  Task_Add(Func_02001ee8, TASK_PASS_VBLANK);
}

extern const u16 gData_02004276[];

/// 2 copies: dehkan_plateau/s2698, dehkan_plateau/s2698.
#define DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(name, n1, n2, n3, n4, n5, n6)      \
  void name(void) {                                                            \
    /* FAKEMATCH(pin): flag in r5, which leaves r6 to the entry; as a plain    \
     * local, 61 of 79 instructions differ in tla-en. */                       \
    register s32 flag asm("r5") = 800;                                         \
    s32 offset = 0;                                                            \
                                                                               \
    while (1) {                                                                \
      if (GameFlag_Test(flag)) {                                               \
        u16 *entry = (u16 *)((u8 *)gData_02004276 + offset);                   \
        s32 y;                                                                 \
        s32 x;                                                                 \
        s32 one;                                                               \
                                                                               \
        flag -= 800;                                                           \
        x = *entry++;                                                          \
        y = *entry++;                                                          \
        one = 1;                                                               \
        MapGrid_CopyCellsTo(n1, n2, x, y, one, one);                           \
        x = entry[0];                                                          \
        y = entry[1];                                                          \
        MapGrid_CopyCellsTo(n3, n4, x, y, one, 5);                             \
        if (flag == 1)                                                         \
          MapGrid_CopyCellsTo(67, 4, 69, 3, one, one);                         \
        x -= 64;                                                               \
        y += n5;                                                               \
        MapGrid_CopyCellsTo(n1, n6, x, y, one, 4);                             \
        return;                                                                \
      }                                                                        \
      if (flag == 863)                                                         \
        return;                                                                \
      offset += 8;                                                             \
      flag++;                                                                  \
    }                                                                          \
  }

DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(Func_0200353c, 3, 8, 67, 3, 66, 69)

DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(Func_020035ec, 13, 5, 77, 0, 65, 65)

void Func_0200369c(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02001fdc();
  if (GameFlag_Test(0x86c) != 0) {
    MapGrid_CopyCellPictureIndicesRect(3, 8, 30, 33, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(67, 3, 94, 28, 1, 5);
    MapGrid_CopyCellPictureIndicesRect(3, 69, 30, 93, 1, 4);
  }
  {
    s32 entrance = *(u16 *)&gMainRoutineWork.entrance;

    if ((u16)(entrance - 11) <= 28) {
      Func_02001804();
      entrance <<= 16;
      entrance >>= 16;
      entrance += 789;
      GameFlag_SetBit(entrance);
    }
  }
  Func_0200353c();
  if (GameFlag_Test(265) == 0) {
    if (gMainRoutineWork.entrance > 10) {
      if (gMainRoutineWork.entrance <= 39)
        Func_02003764();
    }
  }
}

void Func_02003764(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;
  struct EffectOptions *opts;
  s32 spawn;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
  leader->motionFlags = 3;
  Event_Wait(24);
  opts = &options;
  opts->palette = 7;
  opts->update = Effect_AdvanceMotion__s2698;
  opts->startScaleX = 0xa3d7;
  opts->startScaleY = 0xa3d7;
  opts = &options;
  spawn = 1;
  i = 0;
loop:
  {
    s32 angle;
    s32 velocity[3];
    s32 cosine;

    angle = i << 12;
    cosine = Math_Cosine(angle);
    if (spawn) {
      velocity[0] = cosine;
      velocity[1] = 0;
      velocity[2] = Math_Sine(angle);
      velocity[0] += velocity[0] >> 1;
      velocity[0] += ((Random16Far() * 3) << 14) >> 16;
      velocity[0] += -0x6000;
      velocity[2] += (((Random16Far() * 3) << 13) >> 16) + -0x3000;
      Func_02000fc4(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                    velocity[2], 0x01090001, opts);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
  Sound_Play(188);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Event_End();
}

// ---- games/tla/src/scene/dehkan_plateau/s390c.c ----

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

/* VIEW(s390c): static Actor_SetPosition as Actor_SetPosition__s390c; the
 * definition has static inline void Actor_SetPosition(s32 actorId, s32 fixed_x,
 * s32 fixed_z); tried with the definition the rest of the object has, it does
 * not compile in tla-ja: games/tla/src/scene/dehkan_plateau/s2698.c:872: too
 * many arguments to function `Actor_SetPosition'. */
static __inline__ void Actor_SetPosition__s390c(struct Actor *object, s32 x,
                                                s32 y, s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

void Func_0200390c(void) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 entrance;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  owner->priorityFlags |= 32;
  if (GameFlag_Test(0x878) == 0) {
    Func_02003a30();
    if (GameFlag_Test(0x871) != 0) {
      Map_CopyAttributes(0, 0, 1, 1, 15, 5);
      if (gData_02004420 != NULL) {
        Actor_SetPosition__s390c(gData_02004420, 248 << 16, 0, 176 << 15);
        wait(1);
      }
    }
  }
  Func_02000038(gData_02003fb0);
  Func_020019b8();
  entrance = gMainRoutineWork.entrance;
  if (entrance > 10) {
    Func_02001804();
    GameFlag_SetBit(entrance + 789);
  }
  if (Flag_Test(0x871) != 0)
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Func_020035ec();
  if (GameFlag_Test(265) == 0) {
    if (gMainRoutineWork.entrance > 16) {
      if (gMainRoutineWork.entrance == 37 && GameFlag_Test(0x871) == 0)
        Func_02002c14();
      else
        Func_02003764();
    }
  }
  gData_0200567c = 0;
  gData_02005678 = 0;
  Task_Add(Func_02001ee8, TASK_PASS_VBLANK);
}

// ---- games/tla/src/scene/dehkan_plateau/s3a30.c ----
// Scene code of overlay 0x655, in ROM order, from s3a30's offset up to the next
// object the overlay links.
extern struct Actor *gData_02004420;

void Func_02003a30(void) {
  struct Actor **slot = &gData_02004420;
  s32 zero = 0;
  struct Actor *object = ((struct Actor *)Import_ActorCreate(489, 0, 0, 0));

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    object->motionFlags = zero;
    object->unk5c[0] = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(199);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}

void Func_02003aa0(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (GameFlag_Test(95) != 0)
    GameFlag_SetBit(521);
  Func_02000038(gData_02003fb8);
  Func_020001d0(gData_02003f92);
  if (GameFlag_Test(519) == 0) {
    Func_02002140();
    wait(10);
  }
  Func_02001ff4();
  if (GameFlag_Test(265) == 0) {
    if (gMainRoutineWork.entrance > 5) {
      if (gMainRoutineWork.entrance <= 39)
        Func_02003764();
    }
  }
}

s32 Scene_PrepareMap(void) { return 0; }
