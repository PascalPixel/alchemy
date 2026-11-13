/// @file
/// The scene code of overlay 0x678, in ROM order.

#include "shared/routines.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "frame.h"

extern u32 gSceneExits[];

extern char SceneId_Ov678Room1;
extern char SceneId_Ov678Room2;
extern char SceneId_Ov678Room3;
extern char SceneId_Ov678Room4;
extern char SceneId_Ov678Room5;
extern char SceneId_Ov678Room6;
extern char SceneId_Ov678Room7;
extern char SceneId_Ov678Room8;
extern char SceneId_Ov678Room9;
extern char SceneId_Ov678Room10;
extern char SceneId_Ov678Room11;

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02002764(s32 id, s32 a);
void Func_02002778(s32 a, s32 b);
void Event_End(void);
void Actor_JumpDownLedge(s32 id, s32 a, s32 b);
void Func_02002928(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Actor_ReturnHome(s32 a, s32 b, s32 c);
void Func_02002990(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_020029a8(s32 a, s32 b, s32 c);
void Actor_StartScript(struct Actor *object, const void *data);
void Animation_SetStateFlags(struct Actor *object, u32 value);

extern s32 gData_02003458;
extern s32 gData_0200345c;
extern s32 gData_02003460;
extern const u8 gData_020029c8[];
void Func_02002948(void);
void Func_02002084(s32 a);
void Func_020029a0(s32 message);
void Func_020027f8(struct Actor *object, s32 a);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 a);
s32 Func_02002730(void);
void Func_0200271c(s32 a, s32 b);
s32 Func_0200273c(s32 a);
void Func_02002978(s32 a);

struct SpriteWork {
  u8 unk00[17];
  u8 layer : 2;
  u8 level : 6;
  u8 unk12[7];
  u8 dirty;
  u8 mode;
  u8 unk1b[17];
  u8 *child;
};

struct SceneState {
  s16 mode;
  s16 timer;
  s32 x;
  s32 z;
};

extern struct SceneState gSceneState;

typedef volatile u16 vu16;

struct IoReg16 {
  u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)
struct VentEntry {
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

struct VentWork {
  s32 counter;
  s32 (*hook)(void);
  struct VentEntry entries[16];
  u16 ids[4];
};

extern u8 gInventorySnapshot[];
#define VENT_WORK ((struct VentWork *)(gInventorySnapshot + 0x188))
#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

s32 Math_Cosine(s32 angle);
void Func_02002400(struct Actor *object);
extern const u8 gData_02002af4[];
extern s32 gData_020035f0;
extern u8 gMapShapeGrid[];
void Map_CenterOnFollowedPoint(void);
void DisplayBlend_EnableRunScript(void);
void Func_02001eb8(void);
void Func_02001ff0(s32 x, s32 z);
void Func_0200237c(void);
void Func_02000920(s32 a, s32 id);
extern const u8 gData_02002a60[];
extern const u8 gData_02002a6c[];
void Func_02000810(s32 id);
void Func_020008d8(s32 id);
extern const u8 gData_02002a48[];
extern const u8 gData_02002a54[];
extern u8 MsgIThoughtI;
void Func_02000c24(void);
void Func_02000ca0(void);
void Func_02000cec(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 GameFlag_GetByte_(s32 flag) {
  return GameFlag_GetByte(flag);
}

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx,
                                                           s32 dz) {
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Func_02002840_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static __inline__ void Func_020028d0_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void Func_020028e0_(s32 id, s32 a, s32 b) {
  Actor_JumpDownLedge(id, a, b);
}

static __inline__ void Func_02002910_(s32 id, s32 a) {
  FieldEffect_SetAttached(id, a);
}

static __inline__ void Func_02002958_(s32 a, s32 b, s32 c) {
  Actor_ReturnHome(a, b, c);
}

static __inline__ void Func_02002990_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  Func_02002990(a, b, c, d, e, f, g);
}

static __inline__ struct Actor *Func_02002808_(s32 type, s32 x, s32 y, s32 z) {
  return Import_ActorCreate(type, x, y, z);
}

static __inline__ void Func_02002838_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellsRect(a, b, c, d, e, f);
}

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                 s32 acceleration) {
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Func_02002848_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Func_0802b450(a, b, c, d, e, f);
}

static __inline__ void Func_02002818_(struct Actor *object, s32 x, s32 y,
                                      s32 z) {
  Actor_SetMoveTarget(object, x, y, z);
}

static __inline__ void Func_020029a8_(s32 a, s32 b, s32 c) {
  Func_020029a8(a, b, c);
}
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "main_routine_work.h"
#include "system/vector3.h"
#include "iwram_call.h"
#include "version.h"
#include "event_work.h"
#include "ram_buffer.h"
#include "call.h"
#include "scene_tables.h"
#include "field.h"
#include "graphics.h"
#include "module/scene/airs_rock_inside.h"
#include "module/scene/scene.h"
#include "module/scene/taopo_swamp.h"
#include "module/tla.h"

void Func_02000038(void) { Field_RequestDjinnBattle(8, 11); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02002c54[];
extern const u8 gData_02002c6c[];
extern const u8 gData_02002d2c[];
extern const u8 gData_02002d5c[];
extern const u8 gData_02002dbc[];
extern const u8 gData_02002e04[];
extern const u8 gData_02002e64[];
extern const u8 gData_02002f9c[];
extern const u8 gData_02002fe4[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov678Room2)
    return (const struct ActorPlacement *)gData_02002c6c;
  if (scene == (s32)&SceneId_Ov678Room4)
    return (const struct ActorPlacement *)gData_02002d2c;
  if (scene == (s32)&SceneId_Ov678Room5)
    return (const struct ActorPlacement *)gData_02002d5c;
  if (scene == (s32)&SceneId_Ov678Room7)
    return (const struct ActorPlacement *)gData_02002dbc;
  if (scene == (s32)&SceneId_Ov678Room8)
    return (const struct ActorPlacement *)gData_02002e04;
  if (scene == (s32)&SceneId_Ov678Room9)
    return (const struct ActorPlacement *)gData_02002f9c;
  if (scene == (s32)&SceneId_Ov678Room10)
    return (const struct ActorPlacement *)gData_02002e64;
  if (scene == (s32)&SceneId_Ov678Room11)
    return (const struct ActorPlacement *)gData_02002fe4;
  return (const struct ActorPlacement *)gData_02002c54;
}

void Func_02000104(s32 a, s32 b) { GameFlag_SetBit(b + 0x8d8); }

void Func_02000118(void) {
  Func_02002840_(50, 16, 1, 2, 47, 16);
  Event_SetFlag(0x220);
}

extern u8 gData_02002ae8[];
s32 Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags);
void Animation_ApplyChildValuesToRecord(struct ActorSpriteView *record,
                                        u32 value);
void Animation_ApplyChildValue(struct ChildGroup *group, s32 value);

// When actor id stands at x 12, inside an event: the actor drops in place, then
// for 25 frames throws splashes, then two sparks, and sets flag 0x8e5.
void Func_0200013c(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  struct Actor *splash, *spark;
  struct FieldSprite *sprite, *splashSprite, *sparkSprite;
  s32 i, x, z, k, zero;

  if (actor->x >> 20 != 12)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  actor->motionFlags = 3;
  System_Wait(5);
  actor->motionFlags = 0;
  actor->velocityY = 0;
  actor->y = 0x40000;
  sprite = actor->sprite;
  i = 0;
  do {
    ((struct Charge *)sprite)->level = i;
    sprite->nextFrame |= 8;
    sprite->nextFrame &= ~1;
    k = i;
    sprite->unk19 = 1;
    k &= 3;
    if (k == 0) {
      x = actor->x + Random16Far() * 2 - Random16Far() * 2;
      z = actor->z + Random16Far() * 2 - Random16Far() * 2;
      splash = (FieldActor_Create(14, x, actor->y, z));
      if (splash != 0) {
        splashSprite = splash->sprite;
        splash->terrainHeight = actor->terrainHeight;
        Actor_StartScript(splash, gData_02002ae8);
        splash->motionFlags = 0;
        if (splashSprite != 0) {
          ((s32 (*)())Animation_ApplyChildArgument)(splashSprite, 1);
          splashSprite->nextFrame = k;
          splashSprite->priority = sprite->priority;
          Animation_ApplyChildValuesToRecord(splashSprite, 13);
          Animation_ApplyChildValue(splashSprite, 8);
        }
      }
    }
    System_Wait(3);
    i++;
  } while (i <= 24);

  actor->animationKind = 0;
  actor->y = 0;
  for (i = 1; i >= 0; i--) {
    spark = (FieldActor_Create(269, actor->x, actor->y, actor->z));
    if (spark != 0) {
      sparkSprite = spark->sprite;
      Actor_StartScript(spark, gData_02002ae8);
      {
        u8 *flags = &spark->motionFlags;

        zero = 0;
        *flags = zero;
      }
      spark->terrainId = 1;
      if (sparkSprite != 0) {
        Animation_ApplyChildArgument(sparkSprite, 2);
        sparkSprite->nextFrame = zero;
        sparkSprite->priority = 2;
      }
    }
    System_Wait(15);
  }
  FieldEvent_Wait(30);
  actor->animationKind = 1;
  Actor_SetPosition(id, 0, 0);
  Event_End();
  Event_SetFlag(0x8e5);
}

void Func_020002f4(s32 a) { Func_02002958_(a, 65, 0x301); }

void Func_02000304(s32 a) { Func_02002958_(a, 64, 0x300); }

void Func_02000314(void) { Func_02002990_(76, 70, 14, 8, 14, 37, 0); }

void Func_02000334(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Func_02002990_(76, 70, 14, 8, 14, 37, 0);
  if (Event_GetFlag(0xf99) == 0) {
    Func_020028d0_(64, PIXELS(264), PIXELS(664));
    if (leader->x >> 20 == 16 && leader->z >> 20 == 41) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -16);
      leader->velocityY = 0x60000;
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

struct MainRoutineWork254 {
  u8 unk000[0x254];
  u16 value;
};

void Func_020003c4(void) {
  Func_02002990_(76, 70, 14, 8, 14, 37, 0);
  if (!Event_GetFlag(0x9b6)) {
    Func_020029a8_(249, 344, 648);
    ((struct MainRoutineWork254 *)&gMainRoutineWork)->value = 0x19b6;
    gMainRoutineWork.battleRule24b = 2;
    Field_RequestBattleFromEncounterTable(106, 1);
  }
}

void Func_02000428(s32 a) {
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Func_020028e0_(gMainRoutineWork.currentOwner, 6, 0x1000);
  Event_PlaySound(310);
  Event_SetFlag(0x122);
  FieldEvent_SetExitRequest(a);
}

void Func_02000464(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 9) {
    if (object->x >> 20 == 17) {
      object->z += 0x10000;
      Func_02002764(2, 0x80000);
      Event_SetFlag(0xa6d);
    }
  } else if (id == 10) {
    if (object->x >> 20 == 29) {
      object->z += 0x10000;
      Func_02002764(8, 0x80000);
      Event_SetFlag(0xa6e);
    }
  }
}

extern u8 gData_020029b8[];
extern u8 gData_020029c0[];

void Func_020004c8(s32 hole) {
  struct Actor *leader;
  struct Vector3 position;
  s32 found, result, slip;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room5) {
    found = Func_02002708(gData_020029b8[hole - 10]);
    if (hole == 12 && GameFlag_Test(0xa6e))
      return;
    if (hole == 18 && GameFlag_Test(0xa6d))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room7) {
    u8 *table = gData_020029c0 + 1;
    s32 at = hole - 10;
    found = Func_02002708(table[at]);
    if (hole == 10 && GameFlag_Test(545))
      return;
    if (hole == 12 && GameFlag_Test(545))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room8) {
    found = Func_02002708(hole - 10);
    if (hole == 10 && GameFlag_Test(545))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room9)
    found = Func_02002708(hole - 10);
  if (found == 0)
    return;
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  goto test;
sink:
  if (result < 0) {
    leader->facing = 0x4000;
    Func_02000428(hole);
    return;
  }
  leader->z += 0x20000;
  slip = (leader->x & 0xfffff) + 0xfff80000;
  {
    s32 limit = 0x10001;

    if (slip >= limit)
      slip = 0x10000;
    limit = -0x10001;
    if (slip <= limit)
      slip = -0x10000;
  }
  leader->x -= slip;
  leader->facing += 0x1000;
  System_Wait(1);
test:
  position.x = leader->x;
  position.y = leader->y;
  position.z = leader->z + 0x80000;
  result = Func_02002850(leader, &position);
  if (result <= 0)
    goto sink;
}

void Func_0200065c(void) {
  struct Actor *object = Actor_Get(8);
  s32 speed;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 8, 0);
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner, 256, 30);
  FieldCamera_MoveTo(PIXELS(408), -1, PIXELS(312), 1);
  Func_02002928();
  Func_02002910_(8, 257);
  speed = 0x8000;
  i = 2;
  do {
    object->velocityY = 0x20000;
    Event_PlaySound(152);
    object->speedLimit = speed;
    object->acceleration = speed / 2;
    Actor_WalkByAndWait(8, 0, 8);
    speed += 0x4000;
  } while (--i >= 0);
  Event_PlaySound(310);
  object->velocityY = 0x40000;
  Actor_WalkByAndWait(8, 0, 8);
  FieldEvent_Wait(60);
  Event_SetFlag(0x310);
  Event_End();
}

DEFINE_FUNC_0200015C(Func_02000710)

void Func_02000744(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 9 && object->x >> 20 == 16) {
    Func_02000710(9);
    MapGrid_CopyCellAttributes(15, 26, 1, 1, 16, 26);
    Event_SetFlag(0xa6f);
  }
}

void Func_02000784(s32 a, s32 id) {
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_SetAnimation(id, 1);
  Event_SetFlag(0x220);
  Event_SetFlag(0x222);
}

DEFINE_FUNC_020007BC(Func_020007bc, 28, 8, 26)

void Func_02000810(s32 id) {
  struct Actor *leader;
  struct Actor *object;
  s32 zero = 0;

  Actor_SetAnimation(id, 8);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = (void (*)(void))zero;
  Func_02002778(0, 0);
  Event_ClearFlag(0x220);
  Event_ClearFlag(0x221);
  Event_ClearFlag(0x222);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object = Actor_Get(id);
  if (object->x >> 20 == leader->x >> 20 &&
      object->z >> 20 == leader->z >> 20) {
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
    Actor_WalkByAndWait(gMainRoutineWork.currentOwner, 0, 16);
    FieldEvent_Wait(2);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
  }
  Func_02002840_(22, 8, 1, 1, 26, 8);
}

void Func_020008d8(s32 id) {
  Actor_SetAnimation(id, 7);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = 0;
  Func_02002778(0, 0);
  Event_ClearFlag(0x220);
  Event_ClearFlag(0x222);
}

void Func_02000920(s32 a, s32 id) {
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_SetAnimation(id, 1);
  if (a == 5 || FieldGameFlag_GetByte(0x230) > 3) {
    Event_SetFlag(0x220);
    Event_SetFlag(0x222);
  }
}

DEFINE_FUNC_020007BC(Func_0200096c, 23, 27, 25)

void Func_020009c0(s32 id) {
  struct Actor *leader;
  struct Actor *object;

  Actor_SetAnimation(id, 8);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = 0;
  Func_02002778(0, 0);
  Event_ClearFlag(0x220);
  Event_ClearFlag(0x221);
  Event_ClearFlag(0x222);
  GameFlag_SetByte(0x230, 0);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object = Actor_Get(id);
  if (object->x >> 20 == leader->x >> 20 &&
      object->z >> 20 == leader->z >> 20) {
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
    Actor_WalkByAndWait(gMainRoutineWork.currentOwner, 0, 16);
    FieldEvent_Wait(2);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
  }
  Func_02002840_(1, 1, 1, 1, 25, 27);
}

void Func_02000a94(s32 id) {
  s32 zero = 0;

  Actor_SetAnimation(id, 7);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = (void (*)(void))zero;
  Func_02002778(0, 0);
  Event_ClearFlag(0x220);
  GameFlag_SetByte(0x230, 0);
}

void Func_02000adc(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(0x331);
  Event_ClearFlag(0x332);
}

void Func_02000afc(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 7) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Event_SetFlag(0x332);
    Event_SetFlag(0x333);
    MapGrid_CopyCellAttributes(7, 17, 1, 1, 7, 15);
  }
}

void Func_02000b58(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(0x334);
  Event_ClearFlag(0x335);
  Func_02002840_(27, 14, 1, 1, 27, 13);
}

void Func_02000b90(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->z >> 20 == 16) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Event_SetFlag(0x335);
    Event_SetFlag(0x336);
    Actor_SetSpeed(id, 0x8000, 0x4000);
    Actor_SetDestinationOffset(id, -32, 0);
    FieldActor_WaitForMoveById(id);
    Actor_SetAnimation(id, 3);
    MapGrid_CopyCellAttributes(25, 18, 1, 1, 25, 16);
  }
}

void Func_02000c14(struct Actor *object) {
  *(s16 *)((u8 *)object->sprite + 18) -= 0x800;
}

void Func_02000c24(void) {
  s32 a;
  s32 x;
  s32 z;
  struct Actor *object;

  gData_02003458--;
  a = Random16Far() * 32;
  x = Random16Far() * 16 + a + 0x12c0000;
  z = Random16Far() * 16 + a + 0x1040000;
  object = FieldActor_Create(0x11d, x, 0, z);
  Actor_StartScript(object, gData_020029c8);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  if (gData_02003458 == 0)
    FieldTask_Remove(Func_02000c24);
}

void Func_02000ca0(void) {
  s32 timer = --gData_0200345c;
  s32 step = timer / 8;

  Func_02002838_(47 - (timer & 1) * 2, 64 - step, 2, step, 29, 15 - step);
  if (gData_0200345c == 0)
    FieldTask_Remove(Func_02000ca0);
}

void Func_02000cec(void) {
  s32 n = --gData_02003460;

  Func_02002838_(50, (n & 1) * 5 + 59, 13, 5, 18, 15);
  if (gData_02003460 == 0)
    FieldTask_Remove(Func_02000cec);
}

extern u8 gData_02002a04[];

void Func_02000d2c(void) {
  struct Actor *rock = Actor_Get(19);
  struct Actor *clod;
  s32 i, row, cosine;
  s32 (*mul)(s32, s32);
  u32 angle, speed, scale;
  s32 turn = -256;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  for (i = 9; i >= 0; i--) {
    System_Wait(1);
    ((struct FieldSprite *)rock->sprite)->rotation += turn;
    rock->x -= Math_Cosine(((struct FieldSprite *)rock->sprite)->rotation) / 2;
    rock->targetX = 0x80000000;
  }
  rock->update = Func_02000c14;
  Actor_SetSpeed(19, 0x20000, 0x10000);
  Actor_SetDestination(19, 492, 246);
  rock->gravity = 0xcccc;
  rock->motionFlags = 3;
  rock->terrainId = 0;
  FieldActor_WaitForMoveById(19);
  rock->velocityY = 0;
  rock->y = rock->terrainHeight;
  Actor_SetPosition(19, 0x1e00000, 0xf80000);
  ((struct FieldSprite *)rock->sprite)->rotation = 0;
  Event_PlaySound(188);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  Event_PlaySound(141);
  MapGrid_SetShake(-1, -1, 0xe666);
  Actor_SetPosition(10, 0x1e00000, 0xf80000);
  Actor_SetDestination(10, 440, 232);
  Actor_Get(10)->velocityY = 0xa0000;
  rock->update = 0;
  i = 15;
burst:
  clod = (FieldActor_Create(255, rock->x, rock->y + 0x100000, rock->z));
  if (clod == 0)
    goto next;
  angle = Random16Far();
  speed = Random16Far();
  scale = (Random16Far() >> 2) + 0x1000;
  Actor_StartScript(clod, gData_02002a04);
  Actor_SetSingleChildFlags(clod, 0);
  clod->velocityY = (angle << 2) + 0x20000;
  cosine = Math_Cosine(angle);
  mul = Iwram_MulQ16;
  speed <<= 3;
  clod->velocityZ = mul(speed, cosine);
  clod->velocityX = mul(speed, Math_Sin(angle));
  clod->acceleration = 0;
  clod->gravity = 0x547a;
  clod->scaleX = scale;
  clod->scaleY = scale;
  clod->bounce = 0x8000;
next:
  i--;
  if (i >= 0)
    goto burst;
  MapGrid_WaitForShakeToSettle();
  Task_AddFrame(Func_02000ca0, 0x480);
  System_Wait(40);
  FieldCamera_SetSpeed(0xe666, 0x1ccc);
  FieldCamera_MoveTo(0x1500000, -1, 0x1180000, 1);
  i = 11;
  row = 114;
drain:
  MapGrid_CopyCells(50, row, 13, 5, 18, 15);
  System_Wait(8);
  row -= 5;
  i--;
  if (i > 0)
    goto drain;
  Task_AddFrame(Func_02000c24, 0x480);
  System_Wait(60);
  Task_AddFrame(Func_02000cec, 0x480);
  System_Wait(24);
  FieldEvent_Wait(120);
  Event_End();
  Event_SetFlag(0x330);
}

void Func_02000f9c(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(0x338);
  Event_ClearFlag(0x339);
}

void Func_02000fbc(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->z >> 20 == 21) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Event_SetFlag(0x339);
    Event_SetFlag(0x33a);
    MapGrid_CopyCellAttributes(11, 19, 1, 1, 11, 21);
  }
}

void Func_02001018(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(0x33b);
  Event_ClearFlag(0x33c);
}

void Func_02001038(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 31) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Event_SetFlag(0x33c);
    Event_SetFlag(0x33d);
    Actor_SetSpeed(id, 0x8000, 0x4000);
    Actor_SetDestinationOffset(id, 0, -32);
    FieldActor_WaitForMoveById(id);
    Actor_SetAnimation(id, 3);
    MapGrid_CopyCellAttributes(30, 13, 1, 1, 31, 11);
  }
}

#if TLA_PATCH_1 /* Absent from the Japanese edition. */
void Func_020010bc(void) { Func_02002948(); }
#endif

void Func_020010c4(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(0x340);
  Event_ClearFlag(0x341);
  Func_02002840_(7, 39, 1, 1, 7, 38);
  Func_02002084(1);
}

void Func_02001100(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(0x342);
  Event_ClearFlag(0x343);
  Func_02002084(1);
}

void Func_02001128(s32 a) { Func_02002958_(a, 64, 0x348); }

void Func_02001138(void) { Func_020029a0((s32)&MsgIThoughtI); }

extern const u8 gData_0200302c[];
extern const u8 gData_02003038[];
extern const u8 gData_02003068[];
extern const u8 gData_02003128[];
extern const u8 gData_02003194[];
extern const u8 gData_020031e8[];
extern const u8 gData_02003314[];
extern const u8 gData_02003338[];
extern const u8 gData_020033e0[];
extern const u8 gData_02003464[];
extern const u8 gData_020034e8[];
extern const u8 gData_02003560[];

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov678Room1)
    return (const struct Trigger *)gData_02003038;
  if (scene == (s32)&SceneId_Ov678Room2)
    return (const struct Trigger *)gData_02003068;
  if (scene == (s32)&SceneId_Ov678Room3)
    return (const struct Trigger *)gData_02003128;
  if (scene == (s32)&SceneId_Ov678Room4)
    return (const struct Trigger *)gData_02003194;
  if (scene == (s32)&SceneId_Ov678Room5)
    return (const struct Trigger *)gData_020031e8;
  if (scene == (s32)&SceneId_Ov678Room6)
    return (const struct Trigger *)gData_02003314;
  if (scene == (s32)&SceneId_Ov678Room7)
    return (const struct Trigger *)gData_02003338;
  if (scene == (s32)&SceneId_Ov678Room8)
    return (const struct Trigger *)gData_020033e0;
  if (scene == (s32)&SceneId_Ov678Room9)
    return (const struct Trigger *)gData_020034e8;
  if (scene == (s32)&SceneId_Ov678Room10)
    return (const struct Trigger *)gData_02003464;
  if (scene == (s32)&SceneId_Ov678Room11)
    return (const struct Trigger *)gData_02003560;
  return (const struct Trigger *)gData_0200302c;
}

s32 Func_02001228(struct Actor *object) {
  s32 y = object->y;
  struct Actor *spark;
  s32 scale;

  if (y + object->velocityY < object->terrainHeight) {
    spark = Func_02002808_(0x10d, object->x, y, object->z);
    scale = 0x8ccc;
    if (spark) {
      spark->scaleX = scale;
      spark->scaleY = scale;
      Actor_StartScript(spark, gData_02002a48);
      Actor_SetSingleChildFlags(spark, 0);
      Func_020027f8(spark, 2);
    }
    Event_PlaySound(106);
    object->y = object->terrainHeight;
    object->velocityY = 0;
    return 0;
  }
  return 1;
}

void Func_02001288(void) {
  if ((gFrameCount & 7) == 0 && (u32)Random16Far() < 0x4000) {
    s32 x = (Random16Far() << 5) + 0x2080000;
    s32 z = 0xb00000 - (Random16Far() << 3);
    s32 y = 0xa00000;
    struct Actor *object = FieldActor_Create(0x12d, x, y, z);
    s32 scale = 0x547a;

    if (object != NULL) {
      object->scaleX = scale;
      object->scaleY = scale;
      object->gravity = 0x7333;
      Actor_StartScript(object, gData_02002a54);
      Actor_SetSingleChildFlags(object, 0);
      Actor_SetSpritePriorityDirect(object, 3);
    }
  }
}

// The vent setter called through a function type returning s32; each caller's
// tag says why.
#define Vent_SetActive(index, active)                                          \
  ((s32 (*)(s32, s32))Func_0200271c)(index, active)

void Func_02001304(void) {
  s32 timer = Func_02002730();

  /* FAKEMATCH(signature): Func_0200271c called through Vent_SetActive as
   * returning s32, so the arguments are set up as for a call whose value is
   * used (flag before index); as a call to the void setter local.h declares
   * for the modules that match with it, 32 of 117 instructions differ in
   * tla-en. */
  switch (timer) {
  case 0:
    Vent_SetActive(0, 1);
    Vent_SetActive(1, 1);
    Vent_SetActive(4, 1);
    Vent_SetActive(5, 1);
    Func_02002978(170);
    break;
  case 180:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Vent_SetActive(4, 0);
    Vent_SetActive(5, 0);
    Func_02002978(-1);
    break;
  case 240:
    Vent_SetActive(2, 1);
    Vent_SetActive(3, 1);
    Vent_SetActive(6, 1);
    Vent_SetActive(7, 1);
    Vent_SetActive(8, 1);
    Vent_SetActive(9, 1);
    Func_02002978(170);
    break;
  case 420:
    Vent_SetActive(2, 0);
    Vent_SetActive(3, 0);
    Vent_SetActive(6, 0);
    Vent_SetActive(7, 0);
    Vent_SetActive(8, 0);
    Vent_SetActive(9, 0);
    Func_02002978(-1);
    break;
  }
  if ((u32)(timer - 251) <= 178 && Event_GetFlag(0xa6e) && !Event_GetFlag(59) &&
      !Event_GetFlag(0x310))
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 100;
  Func_0200273c(480);
}

DEFINE_FUNC_02001424(Func_02001424, Func_02000810, 8, Func_020008d8)

DEFINE_FUNC_02001424(Func_020014a0, Func_020009c0, 10, Func_02000a94)

void Func_0200151c(void) {
  /* FAKEMATCH(signature): Func_0200271c called as returning s32, as in
   * Func_02001304; as a call to the void setter, 4 of 28 instructions differ in
   * tla-en. */
  switch (Func_02002730()) {
  case 0:
    Vent_SetActive(0, 1);
    Vent_SetActive(1, 1);
    Func_02002978(170);
    break;
  case 180:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Func_02002978(-1);
    break;
  }
  Func_0200273c(240);
}

s32 Func_02001564(struct Actor *object) {
  s32 flag;
  s32 count;
  struct Actor *spark;
  s32 scale;

  if (object->y + object->velocityY < object->terrainHeight) {
    flag = Event_GetFlag(0x221);
    if (flag) {
      Event_PlaySound(106);
      return 0;
    }
    spark = Func_02002808_(0x10d, object->x, object->y, object->z);
    scale = 0x8ccc;
    if (spark) {
      spark->scaleX = scale;
      spark->scaleY = scale;
      Actor_StartScript(spark, gData_02002a48);
      Actor_SetSingleChildFlags(spark, 0);
      Func_020027f8(spark, 2);
    }
    Event_PlaySound(106);
    object->y = object->terrainHeight;
    object->velocityY = 0;
    if (Event_GetFlag(0x104) == 0) {
      count = GameFlag_GetByte_(0x230);
      if (count <= 3) {
        count++;
        GameFlag_SetByte(0x230, count);
      }
      if (count != 0)
        Func_02000920(0, 10);
      object = Actor_Get(10);
      object->scaleX = count << 14;
      object->scaleY = count << 14;
    }
    return 0;
  }
  return 1;
}

void Func_02001618(void) {
  if ((gFrameCount & 63) == 0) {
    s32 x = 0x1980000;
    s32 z = 0x1b80000;
    s32 y = 0xa00000;
    struct Actor *object = FieldActor_Create(0x12d, x, y, z);
    s32 scale = 0x547a;

    if (object != NULL) {
      object->scaleX = scale;
      object->scaleY = scale;
      object->gravity = 0x7333;
      Actor_StartScript(object, gData_02002a60);
      Actor_SetSingleChildFlags(object, 0);
    }
  }
}

s32 Func_0200166c(struct Actor *object) {
  if (object->y + object->velocityY < -0x300000)
    return 0;
  return 1;
}

DEFINE_FUNC_02001684(Func_02001684, 0x1800000, 0x1900000)

DEFINE_FUNC_02001684(Func_020016fc, 0x1a00000, 0x1600000)

extern const u8 gData_02002a78[], gData_02002ab6[], gData_02002abc[],
    gData_02002aca[], gData_02002ad8[];
void Func_02002120(void);
void Actor_SetWideSprite(s32 id, s32 wide);
void Func_02002614(u16 *points, u16 *values, s32 arg);

#if !TLA_PATCH_1
s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object, *dummy, *piece;
  s32 id, entrance;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 513;
  if (scene == (s32)&SceneId_Ov678Room2) {
    gMainRoutineWork.retreatScene = (s32)&SceneId_Ov678Room1;
    gMainRoutineWork.retreatEntrance = 1;
    Func_02002120();
    for (id = 8; id <= 12; id++) {
      if (GameFlag_Test(id + 0x8d8))
        Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
      else
        Actor_GetById(id)->scaleY = 0x14000;
    }
    Actor_SetSpritePriority(13, 3);
    Actor_GetById(13)->collisionFlags = 0;
    if (GameFlag_Test(0x220))
      Call6(Map_CopyCellAttributeRect, 50, 16, 1, 2, 47, 16);
    dummy = Actor_GetById(14);
    if (dummy != 0) {
      dummy->scaleX = 0xc000;
      dummy->scaleY = 0xc000;
      dummy->motionFlags = 0;
    }
    Task_Add(Func_02001288, 0x480);
    ((s8 *)map)[23] = (((s8 *)map)[23] & ~12) | 8;
    gDecodeFillByte = 93;
  } else if (scene == (s32)&SceneId_Ov678Room3) {
    if (!GameFlag_Test(0x300))
      Actor_SetWideSprite(0x40, 1);
    if (!GameFlag_Test(769))
      Actor_SetWideSprite(65, 1);
  } else if (scene == (s32)&SceneId_Ov678Room4) {
    Func_02002960(0);
    if (!GameFlag_Test(0x310))
      Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  } else if (scene == (s32)&SceneId_Ov678Room5) {
    Func_02002960(0);
    if (GameFlag_Test(0x310))
      Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    if (GameFlag_Test(0xa6d))
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(280),
            PIXELS(232));
    if (GameFlag_Test(0xa6e))
      Call3(Actor_SetHorizontalPositionWithTerrain, 10, PIXELS(472),
            PIXELS(248));
    Func_02002614(gData_02002a78, gData_02002ab6, Func_02001304);
  } else if (scene == (s32)&SceneId_Ov678Room7) {
    Func_02002960(0);
    Task_Add(Func_02001684, 0x480);
    if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5)
      Func_02002614(gData_02002abc, 0, Func_02001424);
    piece = Actor_GetById(8);
    piece->motionFlags = 0;
    Actor_SetSingleChildFlags(piece, 0);
    if (GameFlag_Test(545)) {
      Call6(Map_CopyCellAttributeRect, 28, 8, 1, 1, 26, 8);
      Func_020027f8(piece, 0);
    } else {
      Call6(Map_CopyCellAttributeRect, 22, 8, 1, 1, 26, 8);
      Actor_SetSpritePriority(8, 3);
      piece->priorityFlags |= 2;
      if (!GameFlag_Test(0x220))
        Func_020027f8(piece, 7);
    }
    piece = Actor_GetById(9);
    piece->motionFlags = 0;
    if (GameFlag_Test(0xa6f)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(264),
            PIXELS(424));
      piece->terrainHeight = 0;
      piece->y = 0;
      Call6(Map_CopyCellAttributeRect, 15, 26, 1, 1, 16, 26);
    }
  } else if (scene == (s32)&SceneId_Ov678Room8) {
    if (gMainRoutineWork.entrance != 2 && gMainRoutineWork.entrance != 4 &&
        gMainRoutineWork.entrance != 9) {
      Func_02002980();
      Call4(Func_02002988, 0, 776, 8, 9);
    } else {
      Value3((s32 (*)())Func_02002614, (s32)gData_02002aca, 0,
             (s32)Func_020014a0);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Call2(Task_Add, Func_020016fc, 0x480);
      Call2(Task_Add, Func_02001618, 0x480);
      object = Actor_GetById(10);
      object->motionFlags = 0;
      Actor_SetSingleChildFlags(object, 0);
      if (GameFlag_Test(545)) {
        Func_020027f8(object, 0);
      } else {
        Actor_SetSpritePriority(10, 3);
        object->priorityFlags |= 2;
        if (!GameFlag_Test(0x220))
          Func_020027f8(object, 7);
      }
      Call6(Map_CopyCellsRect, 50, 36, 13, 13, 60, 5);
    }
  } else if (scene == (s32)&SceneId_Ov678Room9) {
    Func_02002614(gData_02002ad8, 0, Func_0200151c);
    if (!GameFlag_Test(824)) {
      GameFlag_SetBit(825);
    } else if (GameFlag_Test(826)) {
      Actor_SetModeById(8, 3);
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, PIXELS(184),
            PIXELS(344));
      Actor_GetById(8)->collisionFlags = 0;
      Actor_GetById(8)->priorityFlags |= 2;
      Actor_SetSpritePriority(8, 3);
      Call6(Map_CopyCellAttributeRect, 11, 19, 1, 1, 11, 21);
    }
    if (!GameFlag_Test(827)) {
      GameFlag_SetBit(828);
    } else if (GameFlag_Test(829)) {
      Actor_SetModeById(9, 3);
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(504),
            PIXELS(184));
      Actor_GetById(9)->collisionFlags = 0;
      Actor_GetById(9)->priorityFlags |= 2;
      Call6(Map_CopyCellAttributeRect, 30, 13, 1, 1, 31, 11);
    }
  } else if (scene == (s32)&SceneId_Ov678Room10) {
    piece = Actor_GetById(19);
    if (GameFlag_Test(0x330)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 19, PIXELS(480),
            PIXELS(248));
      if (!GameFlag_Test(265))
        Call3(Actor_SetHorizontalPositionWithTerrain, 10, PIXELS(440),
              PIXELS(232));
      Call6(Map_CopyCellsRect, 47, 50, 2, 14, 29, 1);
      Call6(Map_CopyCellsRect, 50, 59, 13, 5, 18, 15);
    } else {
      piece->motionFlags = 0;
      piece->y = 0x200000;
    }
    if (!GameFlag_Test(817)) {
      GameFlag_SetBit(818);
    } else if (GameFlag_Test(819)) {
      Actor_SetModeById(9, 3);
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(120),
            PIXELS(248));
      Actor_GetById(9)->collisionFlags = 0;
      Actor_GetById(9)->priorityFlags |= 2;
      Call6(Map_CopyCellAttributeRect, 7, 17, 1, 1, 7, 15);
    } else {
      Actor_SetModeById(9, 0);
    }
    if (!GameFlag_Test(820)) {
      GameFlag_SetBit(821);
    } else {
      Call6(Map_CopyCellAttributeRect, 27, 14, 1, 1, 27, 13);
      if (GameFlag_Test(822)) {
        Actor_SetModeById(10, 3);
        Call3(Actor_SetHorizontalPositionWithTerrain, 10, PIXELS(408),
              PIXELS(264));
        Actor_GetById(10)->collisionFlags = 0;
        Actor_GetById(10)->priorityFlags |= 2;
        Call6(Map_CopyCellAttributeRect, 25, 18, 1, 1, 25, 16);
      } else {
        Actor_SetModeById(10, 0);
      }
    }
  } else if (scene == (s32)&SceneId_Ov678Room11) {
    if (!GameFlag_Test(0x340)) {
      GameFlag_SetBit(833);
    } else {
      Actor_SetModeById(8, 0);
      Call6(Map_CopyCellAttributeRect, 7, 39, 1, 1, 7, 38);
    }
    if (!GameFlag_Test(834))
      GameFlag_SetBit(835);
    else
      Actor_SetModeById(9, 0);
    if (!GameFlag_Test(840))
      Actor_SetWideSprite(0x40, 0);
    entrance = gMainRoutineWork.entrance;
    if ((u32)(entrance - 2) <= 1) {
      if (GameFlag_Test(0x340))
        Func_02002084(0);
      else
        Func_02002114(0);
    }
    if (entrance == 6) {
      if (GameFlag_Test(834))
        Func_02002084(0);
      else
        Func_02002114(0);
    }
    if ((u32)(entrance - 4) <= 1)
      DisplayBlend_EnableRunScript();
  }
  return 0;
}
#else
s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *effect = (s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac);
  struct Actor *object, *dummy, *piece;
  s32 id, entrance;

  *effect = 0;
  if (scene == (s32)&SceneId_Ov678Room2) {
    gMainRoutineWork.retreatScene = (s32)&SceneId_Ov678Room1;
    gMainRoutineWork.retreatEntrance = 1;
    Func_02002120();
    for (id = 8; id <= 12; id++) {
      if (GameFlag_Test(id + 0x8d8))
        Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
      else
        Actor_GetById(id)->scaleY = 0x14000;
    }
    Actor_SetSpritePriority(13, 3);
    Actor_GetById(13)->collisionFlags = 0;
    if (GameFlag_Test(0x220))
      Call6(Map_CopyCellAttributeRect, 50, 16, 1, 2, 47, 16);
    dummy = Actor_GetById(14);
    if (dummy != 0) {
      dummy->scaleX = 0xc000;
      dummy->scaleY = 0xc000;
      dummy->motionFlags = 0;
    }
    Task_Add(Func_02001288, 0x480);
    ((s8 *)map)[23] = (((s8 *)map)[23] & ~12) | 8;
    gDecodeFillByte = 93;
  } else if (scene == (s32)&SceneId_Ov678Room3) {
    *effect = 516;
    if (!GameFlag_Test(0x300))
      Actor_SetWideSprite(0x40, 1);
    if (!GameFlag_Test(769))
      Actor_SetWideSprite(65, 1);
  } else if (scene == (s32)&SceneId_Ov678Room4) {
    *effect = 516;
    Func_02002960(0);
    if (!GameFlag_Test(0x310))
      Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  } else if (scene == (s32)&SceneId_Ov678Room5) {
    *effect = 516;
    Func_02002960(0);
    if (GameFlag_Test(0x310))
      Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    if (GameFlag_Test(0xa6d))
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(280),
            PIXELS(232));
    if (GameFlag_Test(0xa6e))
      Call3(Actor_SetHorizontalPositionWithTerrain, 10, PIXELS(472),
            PIXELS(248));
    Func_02002614(gData_02002a78, gData_02002ab6, Func_02001304);
  } else if (scene == (s32)&SceneId_Ov678Room6) {
    *effect = 516;
  } else if (scene == (s32)&SceneId_Ov678Room7) {
    *effect = 516;
    Func_02002960(0);
    Task_Add(Func_02001684, 0x480);
    if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5)
      Func_02002614(gData_02002abc, 0, Func_02001424);
    piece = Actor_GetById(8);
    piece->motionFlags = 0;
    Actor_SetSingleChildFlags(piece, 0);
    if (GameFlag_Test(545)) {
      Call6(Map_CopyCellAttributeRect, 28, 8, 1, 1, 26, 8);
      Func_020027f8(piece, 0);
    } else {
      Call6(Map_CopyCellAttributeRect, 22, 8, 1, 1, 26, 8);
      Actor_SetSpritePriority(8, 3);
      piece->priorityFlags |= 2;
      if (!GameFlag_Test(0x220))
        Func_020027f8(piece, 7);
    }
    piece = Actor_GetById(9);
    piece->motionFlags = 0;
    if (GameFlag_Test(0xa6f)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(264),
            PIXELS(424));
      piece->terrainHeight = 0;
      piece->y = 0;
      Call6(Map_CopyCellAttributeRect, 15, 26, 1, 1, 16, 26);
    }
  } else if (scene == (s32)&SceneId_Ov678Room8) {
    *effect = 516;
    if (gMainRoutineWork.entrance != 2 && gMainRoutineWork.entrance != 4 &&
        gMainRoutineWork.entrance != 9) {
      Func_02002980();
      Call4(Func_02002988, 0, 776, 8, 9);
    } else {
      Value3((s32 (*)())Func_02002614, (s32)gData_02002aca, 0,
             (s32)Func_020014a0);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Call2(Task_Add, Func_020016fc, 0x480);
      Call2(Task_Add, Func_02001618, 0x480);
      object = Actor_GetById(10);
      object->motionFlags = 0;
      Actor_SetSingleChildFlags(object, 0);
      if (GameFlag_Test(545)) {
        Func_020027f8(object, 0);
      } else {
        Actor_SetSpritePriority(10, 3);
        object->priorityFlags |= 2;
        if (!GameFlag_Test(0x220))
          Func_020027f8(object, 7);
      }
      Call6(Map_CopyCellsRect, 50, 36, 13, 13, 60, 5);
    }
  } else if (scene == (s32)&SceneId_Ov678Room9) {
    *effect = 516;
    Func_02002614(gData_02002ad8, 0, Func_0200151c);
    if (!GameFlag_Test(824)) {
      GameFlag_SetBit(825);
    } else if (GameFlag_Test(826)) {
      Actor_SetModeById(8, 3);
      Actor_SetHorizontalPositionWithTerrain(8, PIXELS(184), PIXELS(344));
      Actor_GetById(8)->collisionFlags = 0;
      Actor_GetById(8)->priorityFlags |= 2;
      Actor_SetSpritePriority(8, 3);
      Call6(Map_CopyCellAttributeRect, 11, 19, 1, 1, 11, 21);
    }
    if (!GameFlag_Test(827)) {
      GameFlag_SetBit(828);
    } else if (GameFlag_Test(829)) {
      Actor_SetModeById(9, 3);
      Actor_SetHorizontalPositionWithTerrain(9, PIXELS(504), PIXELS(184));
      Actor_GetById(9)->collisionFlags = 0;
      Actor_GetById(9)->priorityFlags |= 2;
      Call6(Map_CopyCellAttributeRect, 30, 13, 1, 1, 31, 11);
    }
  } else if (scene == (s32)&SceneId_Ov678Room10) {
    *effect = 516;
    piece = Actor_GetById(19);
    if (GameFlag_Test(0x330)) {
      Actor_SetHorizontalPositionWithTerrain(19, PIXELS(480), PIXELS(248));
      if (!GameFlag_Test(265))
        Call3(Actor_SetHorizontalPositionWithTerrain, 10, PIXELS(440),
              PIXELS(232));
      Call6(Map_CopyCellsRect, 47, 50, 2, 14, 29, 1);
      Call6(Map_CopyCellsRect, 50, 59, 13, 5, 18, 15);
    } else {
      piece->motionFlags = 0;
      piece->y = 0x200000;
    }
    if (!GameFlag_Test(817)) {
      GameFlag_SetBit(818);
    } else if (GameFlag_Test(819)) {
      Actor_SetModeById(9, 3);
      Actor_SetHorizontalPositionWithTerrain(9, PIXELS(120), PIXELS(248));
      Actor_GetById(9)->collisionFlags = 0;
      Actor_GetById(9)->priorityFlags |= 2;
      Call6(Map_CopyCellAttributeRect, 7, 17, 1, 1, 7, 15);
    } else {
      Actor_SetModeById(9, 0);
    }
    if (!GameFlag_Test(820)) {
      GameFlag_SetBit(821);
    } else {
      Call6(Map_CopyCellAttributeRect, 27, 14, 1, 1, 27, 13);
      if (GameFlag_Test(822)) {
        Actor_SetModeById(10, 3);
        Actor_SetHorizontalPositionWithTerrain(10, PIXELS(408), PIXELS(264));
        Actor_GetById(10)->collisionFlags = 0;
        Actor_GetById(10)->priorityFlags |= 2;
        Call6(Map_CopyCellAttributeRect, 25, 18, 1, 1, 25, 16);
      } else {
        Actor_SetModeById(10, 0);
      }
    }
  } else if (scene == (s32)&SceneId_Ov678Room11) {
    *effect = 516;
    if (!GameFlag_Test(0x340)) {
      GameFlag_SetBit(833);
    } else {
      Actor_SetModeById(8, 0);
      Call6(Map_CopyCellAttributeRect, 7, 39, 1, 1, 7, 38);
    }
    if (!GameFlag_Test(834))
      GameFlag_SetBit(835);
    else
      Actor_SetModeById(9, 0);
    if (!GameFlag_Test(840))
      Actor_SetWideSprite(0x40, 0);
    entrance = gMainRoutineWork.entrance;
    if ((u32)(entrance - 2) <= 1) {
      if (GameFlag_Test(0x340))
        Func_02002084(0);
      else
        Func_02002114(0);
    }
    if (entrance == 6) {
      if (GameFlag_Test(834))
        Func_02002084(0);
      else
        Func_02002114(0);
    }
    if ((u32)(entrance - 4) <= 1)
      DisplayBlend_EnableRunScript();
  }
  return 0;
}
#endif

s32 Scene_PrepareMap(void) { return 0; }

void Func_02001eb8(void) {
  s32 frame = -1;

  if (Event_GetFlag(0x348)) {
    frame = 62;
  } else {
    switch (gData_020035f0) {
    case 0:
      frame = 59;
      break;
    case 10:
    case 50:
      frame = 56;
      break;
    case 20:
    case 40:
      frame = 53;
      break;
    case 30:
      frame = 50;
      break;
    }
    if (++gData_020035f0 > 59)
      gData_020035f0 = 0;
  }
  if (frame != -1)
    Func_02002848_(45, frame, 3, 3, 76, 61);
}

extern u8 gMapCellBuffer[];

void Func_02001ff0(s32 x, s32 z) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct MapFocus *focus;
  s32 index, fx = x << 19;

  focus = (struct MapFocus *)(map + 0x178);
  focus->x = fx;
  focus->z = z << 19;
  focus->unk28 = 127;
  focus->unk2a = 0x3f80;
  focus->row = z << 7;
  focus->cellX = x;
  focus->unk18[0] = 0;
  focus->unk18[1] = 0;
  focus->unk18[2] = 0;
  focus->unk18[3] = 0;
  index = (z / 2 << 7) + x / 2;
  focus->scaleZ = focus->scaleX = 0x10000;
  focus->cell = gMapCellBuffer + index * 4;
  focus->shape = gMapShapeGrid + index;
  focus->scrolledX = Iwram_MulQ16(*(s32 *)(map + 0xe4), 0x10000) + focus->x;
  focus->scrolledZ =
      Iwram_MulQ16(*(s32 *)(map + 0xe8), focus->scaleZ) + focus->z;
  Map_CenterOnFollowedPoint();
  System_Wait(1);
}

static __inline__ void Blend_SetAlpha(s32 value) {
  REG_BLDALPHA->value = value;
}

void Func_02002084(s32 fade) {
  DisplayBlend_EnableRunScript();
  if (fade) {
    Blend_SetAlpha(0xe06);
    /* FAKEMATCH(dead): the empty once-only block finishes the blend value
     * before the shared register address; without it, 2 of 60 instructions
     * differ in tla-en. */
    do {
    } while (0);
    System_Wait(10);
    Blend_SetAlpha(0xd04);
    System_Wait(10);
    Blend_SetAlpha(0xd02);
    System_Wait(10);
    Blend_SetAlpha(0xc01);
    System_Wait(10);
    Blend_SetAlpha(0xc00);
    System_Wait(10);
  }
  Func_02001ff0(100, 50);
  Blend_SetAlpha(0xc07);
  if (gMainRoutineWork.entrance == 6)
    FieldTask_AddFrame(Func_02001eb8, 0x480);
}

void Func_02002114(void) { Func_02001ff0(100, 100); }

void Func_02002120(void) {
  struct SceneState *state = &gSceneState;

  if (Event_GetFlag(0x109) == 0)
    Iwram_ClearWordsEntry(state, 12);
}

// volatile: the VBlank handler sets the held keys, as gKeysHeld in The Broken
// Seal.
/* VIEW(taopo_swamp): variable gInput as volatile u32 gInput; input.h has struct
 * InputState gInput; tried as the 3 declarations' extern u32 gInput,
 * Func_02002144 moves: 5 of 159 instructions in tla-ja. */
extern volatile u32 gInput__taopo_swamp __asm__("gInput");

void Func_02002144(void) {
  struct SceneState *state = &gSceneState;
  struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
  struct SpriteWork *sprite = actor->sprite;
  struct Actor *spark;
  struct FieldSprite *sparkSprite;
  s32 charge, step, i;

  if (sprite->child != 0)
    sprite->child[6] = 1;
  sprite->mode |= 8;
  sprite->mode &= 0xfe;
  /* FAKEMATCH(dead): the once-only block orders this statement; as a plain
   * statement, 4 of 159 instructions differ in tla-en. */
  do {
    sprite->dirty = 1;
  } while (0);
  if (gInput__taopo_swamp & 0xf0) {
    step = 0x1400;
    if (gInput__taopo_swamp & 2)
      step = step * 3 >> 1;
    state->timer += step / 96 + 1;
  }
  sprite->level = state->timer / 256;
  state->mode = 1;
  if (state->timer < 0x1400)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  actor->animationKind = 0;
  for (i = 1; i >= 0; i--) {
    spark = (FieldActor_Create(269, actor->x, actor->y, actor->z));
    if (spark != 0) {
      sparkSprite = spark->sprite;
      Actor_StartScript(spark, gData_02002ae8);
      spark->motionFlags = 0;
      spark->terrainId = 1;
      if (sparkSprite != 0) {
        Animation_ApplyChildArgument(sparkSprite, 2);
        sparkSprite->nextFrame = 0;
        sparkSprite->priority = 2;
      }
    }
    System_Wait(15);
  }
  FieldEvent_Wait(30);
  actor->animationKind = 1;
  sprite->level = 0;
  sprite->mode = 0;
  Event_End();
  actor->x = state->x;
  actor->z = state->z;
  state->mode = 0;
  state->timer = 0;
}

void Func_020022ac(void) {
  struct SceneState *state = &gSceneState;
  struct Actor *object;
  struct SpriteWork *sprite;

  if (state->mode != 2) {
    object = Actor_Get(FieldParty_GetLeaderActor());
    sprite = object->sprite;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Func_02002818_(object, (object->x & 0xfff00000) + 0x80000, object->y,
                   (object->z & 0xfff00000) + 0x80000);
    Actor_WaitForMove(object);
    while (sprite->level != 0) {
      sprite->level--;
      sprite->dirty = 1;
      System_Wait(1);
    }
    Event_End();
    sprite->mode |= 8;
    sprite->mode &= 0xfe;
    state->mode = 2;
    state->timer = 0;
  }
}

void Func_02002334(void) {
  struct SceneState *state = &gSceneState;
  struct SpriteWork *sprite;

  Func_0200237c();
  if (state->mode != 0) {
    sprite = Actor_Get(FieldParty_GetLeaderActor())->sprite;
    if (sprite->child)
      sprite->child[6] = 9;
    sprite->mode = (sprite->mode & 0xf7) | 1;
    sprite->level = 0;
    sprite->dirty = 1;
    state->mode = 0;
    state->timer = 0;
  }
}

void Func_0200237c(void) {
  struct SceneState *state = &gSceneState;
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());

  state->x = (object->x & 0xfff00000) + 0x80000;
  state->z = (object->z & 0xfff00000) + 0x80000;
}

#define VENT_TOUCHING Func_020023a8
#define VENT_BOUNCE Func_02002400
#include "../field/object/vent_bounce.c"

DEFINE_FUNC_020024F8(Func_020024f8, Func_020027f8, gData_02002af4)

void Func_02002554(s32 *position, s32 base) {
  struct Actor *object =
      Func_02002808_(0x11d, position[0], position[1], position[2] - 0x20000);
  s32 angle;

  if (object != 0) {
    object->bounce =
        Math_Cosine(((u32)(Random16Far() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = (u16)(Random16Far() << 1) + base;
    /* FAKEMATCH(temporary): an s32 local keeps the zero extension and the
     * pool word 0xffff8000; as a direct store, which narrows the add to 16
     * bits (movs, lsls for 0x8000, no extension), 10 of 50 instructions differ
     * in tla-en. */
    angle = (u16)Random16Far() - 0x8000;
    object->timer = angle;
    Func_020024f8(object, 3);
    object->update = (void (*)(void))Func_02002400;
    Animation_SetStateFlags(object, 1);
  }
}

DEFINE_FUNC_020025D4(Func_020025d4, Func_02002554)

// The registers by io_reg.h's names from here on: local.h names the blend
// register its own way for Func_02002084 above.
#undef REG_BLDALPHA
#include "system/io_reg.h"

void Func_02002614(u16 *points, u16 *values, s32 arg) {
  struct SwingWork *work = (struct SwingWork *)(gInventorySnapshot + 0x188);
  struct SwingPoint *point = work->points;

  if (!Event_GetFlag(0x109)) {
    s32 x;
    s32 i;

    Iwram_ClearWordsEntry(work, 0x150);
    x = *points++;
    i = 0;
    while (x > 0) {
      s32 z = points[0];
      s32 w = points[1];

      point->unk00 = 0;
      point->x = x << 16;
      point->z = z << 16;
      point->y = Map_GetTerrainHeight(0, x << 16, (z << 16) + 0x100000);
      point->w = w << 16;
      point->index = i++;
      points += 2;
      point++;
      if (i > 15)
        break;
      x = *points++;
    }
    if (values != 0) {
      x = *values++;
      i = 0;
      if (x > 0) {
      loop2:
        work->values[i++] = x;
        if (i > 3)
          goto done2;
        x = *values++;
        if (x > 0)
          goto loop2;
      }
    }
  done2:;
  }
  work->arg = arg;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = 0xc08;
      REG_BLDALPHA = alpha;
    }
    {
      s32 cnt = 0x3f10;
      REG_BLDCNT = cnt;
    }
  }
  Task_AddFrame(Func_020025d4, 0x480);
}

/// The accessors step the work pointer by whole entries and then take entry 0:
/// the bytes add the base to index * 20 before the field offset.
#define VENT_AT(work, index)                                                   \
  ((struct VentWork *)((u8 *)(work) + (index) * sizeof(struct VentEntry)))

s32 Func_02002708(s32 index) {
  struct VentWork *work;

  work = VENT_WORK;
  return VENT_AT(work, index)->entries[0].active;
}

void Func_0200271c(s32 index, s32 active) {
  struct VentWork *work;

  work = VENT_WORK;
  VENT_AT(work, index)->entries[0].active = active;
}

s32 Func_02002730(void) { return VENT_WORK->counter; }

DEFINE_FUNC_0200273C(Func_0200273c)

/// The accessors step the work pointer by whole entries and then take entry 0:
/// the bytes add the base to index * 20 before the field offset.

void Func_02002764(s32 index, s32 value) {
  struct VentWork *work;

  work = VENT_WORK;
  VENT_AT(work, index)->entries[0].value = value;
}

DEFINE_FUNC_02002778(Func_02002778)
