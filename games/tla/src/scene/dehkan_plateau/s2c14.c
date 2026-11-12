/// @file
/// Scene code of overlay 0x655, in ROM order, from s2c14's offset up to the next object the overlay links.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "event_work.h"
#include "scene_tables.h"
#include "scene_ids.h"

extern u8 gMapShapeGrid[];

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct Actor *object, struct Actor *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
void Effect_AdvanceMotionAtLeaderPriority(struct Actor *object);

/// The effect fields of an Actor, as field_effect.h names them.
struct EffectView {
  u8 unk00[0x30];
  s32 scaleRateX;
  s32 scaleRateY;
  u8 unk38[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  struct FieldSprite *sprite;
  u8 unk54[0x10];
  u16 spin;
  u8 unk66[6];
  void (*update)(struct Actor *object);
};

extern struct Actor *gData_02004420;

void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020021b4(void);
void Func_02002bcc(void);
void Func_02003bc8(void);

void Func_02002c14(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *helper = Actor_GetById(9);

  Func_02003c28();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02003bc8();
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
  Script_WaitForEventTimeout(leader);
  Event_Wait(20);
  Func_080d47b4(9, 0x103, 0);
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
  Func_02003c30();
}

void Func_02002698();
void Func_02003c28();
void Func_02003c30();
void Func_02003cf0();
void Actor_SetSingleChildFlags();

void Func_02002dd4(void) {
  struct Actor *actor;

  actor = Actor_GetById(16);
  Func_02003c28();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  Func_080d4568()->motionFlags = 0;
  Camera_MoveWithinMapBounds(PIXELS(760), PIXELS(32), PIXELS(204), 1);
  Func_02003cf0();
  Actor_SetAngleToward(16, gMainRoutineWork.currentOwner, 0);
  // Called for its value, which is unused: as Call3 the actor id is loaded
  // before the 0x100's shift.
  Actor_ShowEmote(16, 0x100, 40);
  Actor_SetSpeedParameters(16, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_SetPositionAndReset(16, 760, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Event_Wait(20);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_SetPositionAndReset(16, 792, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Func_02002698(9, 16, 0x210);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_SetPositionAndReset(16, 824, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Event_Wait(20);
  Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Event_Wait(20);
  Func_02003c30();
}

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

void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Func_02003de8(void);
void Func_020015b8(struct Actor *object);
void Func_0200172c(void);
void Func_02001804(void);

struct CellRecord {
  u16 present;
  u16 x;
  u16 y;
  u16 unk06[3];
};

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

void Func_02003cf0(void);
void Func_02003d50(s32 a, s32 b);
void Func_02003d58(s32 a, s32 b);
void Func_02003d60(void);
void Func_02003d68(void);
void Func_02003d70(void);
void Func_02003cc0(s32 id);
void Func_02003d40(struct Actor *object, s32 mode);
void Func_02003bb8(struct Actor *object);
void Func_02002b68(void);
void Func_02002f08(void);

struct EffectWork7c4 {
  u8 unk000[0x7c4];
  s16 active;
  s16 timer;
};
void Field_DispatchTypeHandler(s32 type);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_SetAngle(s32 id, s32 angle) {
  Actor_ArmCallbackAndRefresh(id, angle);
}

void Func_02002f08(void) {
  Func_02003c28();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(16, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(16, 0x100, 40);
  Actor_ArmCallbackAndRefresh(16, FACING_EAST);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(16, 3);
  Actor_SetAngle(16, FACING_WEST);
  Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
  Func_080d489c(16, 0x102);
  Event_Wait(40);
  Func_02003c30();
}

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

void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Func_02003de8(void);
void Func_020015b8(struct Actor *object);
void Func_0200172c(void);
void Func_02001804(void);

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Func_02003cf0(void);
void Func_02003d50(s32 a, s32 b);
void Func_02003d58(s32 a, s32 b);
void Func_02003d60(void);
void Func_02003d68(void);
void Func_02003d70(void);
void Func_02003cc0(s32 id);
void Func_02003d40(struct Actor *object, s32 mode);
void Func_02003bb8(struct Actor *object);
void Func_02002b68(void);
void Func_02002f08(void);

void Field_DispatchTypeHandler(s32 type);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

void Func_02002dd4(void);

void Func_02002f74(void) {
  if (GameFlag_Test(0x207) != 0)
    Func_02002f08();
  else
    Func_02002dd4();
  GameFlag_SetBit(0x209);
}

extern struct Actor *gData_02004420;

void Func_02002f9c(void) {
  Func_02003c28();
  Event_ResolvePendingActions(0);
  if (gData_02004420 != 0)
    Func_02003d40(gData_02004420, 3);
  PartyInventory_GiveItem(199, 0);
  if (gData_02004420 != 0)
    Func_02003bb8(gData_02004420);
  Map_CopyAttributes(1, 0, 1, 1, 15, 5);
  GameFlag_SetBit(0x878);
  Func_02003c30();
}

void Func_02003c28(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020030e4(void);
void Func_02003164(void);
void Func_02003284(void);
void Func_0200369c(void);
void Func_0200390c(void);
void Func_02003aa0(void);

void Func_02002ff4(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Func_02003c28();
  Event_ResolvePendingActions(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov655Room1)
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(*(s16 *)(work + 0x154));
}

s32 Func_0200304c(void) {
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

void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Func_02003de8(void);
void Func_020015b8(struct Actor *object);
void Func_0200172c(void);
void Func_02001804(void);

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

void Func_02003cf0(void);
void Func_02003d50(s32 a, s32 b);
void Func_02003d58(s32 a, s32 b);
void Func_02003d60(void);
void Func_02003d68(void);
void Func_02003d70(void);
void Func_02003cc0(s32 id);
void Func_02003d40(struct Actor *object, s32 mode);
void Func_02003bb8(struct Actor *object);
void Func_02002b68(void);
void Func_02002f08(void);

void Field_DispatchTypeHandler(s32 type);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

struct AnimationFacing {
  u8 unk00[9];
  u8 low : 2;
  u8 facing : 2;
  u8 high : 4;
};

void Func_020030c0(void) {
  Actor_SetSpritePriority(
      64, ((struct AnimationFacing *)Actor_GetById(gMainRoutineWork.currentOwner)
               ->animation)
              ->facing);
}

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

void Func_020000a4(const void *data, s32 a, s32 b);
void Func_020002f8(const void *data);
void Func_02000708(const void *data);
void Func_02001474(s32 show);
void Func_02002180(void);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Func_02003de8(void);
void Func_020015b8(struct Actor *object);
void Func_0200172c(void);
void Func_02001804(void);

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_02003cf0(void);
void Func_02003d50(s32 a, s32 b);
void Func_02003d58(s32 a, s32 b);
void Func_02003d60(void);
void Func_02003d68(void);
void Func_02003d70(void);
void Func_02003cc0(s32 id);
void Func_02003d40(struct Actor *object, s32 mode);
void Func_02003bb8(struct Actor *object);
void Func_02002b68(void);
void Func_02002f08(void);

void Field_DispatchTypeHandler(s32 type);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

void Func_020001d0(const void *data);
void Func_02000038(const void *data);
void Func_02001820(void);
void Func_02001f94(void);
void Func_020030c0(void);

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
