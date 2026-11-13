/// @file
/// Scene code of overlay 0x670, in ROM order.

// ---- games/tla/src/scene/kibombo/s2f34.c ----

#include "../shared/actors.h"
#include "../shared/tables.h"
#include "../kibombo.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/scene/kibombo.h"
#include "module/scene/scene.h"
#include "module/field/field_effect.h"

void Actor_StartScript(struct KibomboObject *object, const void *script);
extern u32 gData_02005ca4[];

void Func_02002f34(void) {
  s32 phase = gFrameCount & 3;

  if (phase == 0) {
    struct KibomboObject *object =
        Import_ActorCreate(0x16e, 0x1080000, 0x200000, 0x13c0000);
    s32 value;

    if (object != 0) {
      object->field44 = Math_Cosine(((Random16Far() << 13) >> 16) + 0x3000);
      object->field48 = 0x10000;
      object->field4c = 0x40000 - (u16)Random16Far();
      value = (u16)Random16Far() + 0xffff8000;
      object->field64 = value;
      object->motionFlags = 0;
      Actor_SetSingleChildFlags(object, 0);
      Func_02005a24(object, 2);
      Actor_StartScript(object, gData_02005ca4);
      object->sprite->priority = 1;
      object->field64 = 0;
      *(s32 *)&object->unk2c[4] = 0;
      *(s32 *)&object->unk2c[8] = 0;
      object->update = Effect_AdvanceMotion;
    }
  }
}

void Func_02002fec(struct KibomboObject *object) {
  object->x += object->field64 << 8;

  object->y += 0x8000;

  object->scaleX += 0x51e;
  object->scaleY += 0x51e;
  object->field64 += 2;
  if (--object->field68 == 0)
    Import_ActorDestroy(object);
}

void Func_02003030(void) {
  if ((gFrameCount & 63) == 0) {
    struct KibomboObject *object =
        Import_ActorCreate(0x11d, 0x580000, 0x100000, 0x13a0000);
    u8 *part;
    s32 mask;

    if (object != 0) {
      object->field5c = 2;
      object->motionFlags = 0;
      object->field64 = 0;
      object->priorityFlags &= ~1;
      mask = 13;
      part = (u8 *)object->sprite;
      part[9] = (part[9] & -mask) | 8;
      Actor_SetSingleChildFlags(object, 0);
      object->scaleX = 0x8000;
      object->scaleY = 0x8000;
      object->field68 = 60;
      object->update = Func_02002fec;
      Func_02005a24(object, 5);
      Animation_SetStateFlags(object, 1);
    }
  }
}

void Func_020030b8(struct KibomboObject *object) {
  if (object->field64 != 0) {
    object->field64--;
  } else {
    object->field64 = ((Random16Far() * 10) >> 16) + 10;
    if (object->field66 != 0) {
      object->scaleX = 0x18000;
      object->field66 = 0;
    } else {
      object->scaleX = 0xfffe8000;
      object->field66 = 1;
    }
  }
}

void Func_02003104(s32 x, s32 y, s32 z) {
  struct KibomboObject *object = Import_ActorCreate(0x106, x, y, z);
  struct FieldSprite *sprite;

  if (object != 0) {
    object->motionFlags = 0;
    object->priorityFlags &= ~1;
    object->scaleY = 0x20000;
    object->field64 = ((Random16Far() * 10) >> 16) + 10;
    object->field66 = 0;
    object->update = Func_020030b8;
    sprite = object->sprite;
    sprite->priority = 1;
    Actor_SetSingleChildFlags(object, 0);
    Func_02005a24(object, 5);
    Animation_SetStateFlags(object, 1);
  }
}

#include "io_reg.h"

struct KibomboEffectWork {
  u8 unk000[0x7c4];
  s16 field7c4;
};

static inline void Kibombo_Spawn(s32 x, s32 y, s32 z) {
  Func_02003104(x, y, z);
}

void Func_02003184(void) {
  s32 x = 0x1b80000;

  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  Kibombo_Spawn(x, 0x280000, 0x1c20000);
  Kibombo_Spawn(x, 0x2e0000, 0x1b60000);
  Kibombo_Spawn(x, 0x340000, 0x1aa0000);
  Kibombo_Spawn(x, 0x3a0000, 0x19e0000);
  /* FAKEMATCH(dead): an empty once-only block before the last spawn, which
   * shifts x's shift after the second register store as the native has it;
   * without it, 2 of 43 instructions differ in tla-en. */
  do {
  } while (0);
  Func_02003104(x, 0x400000, 0x1920000);
}

#include "scene_tables.h"

DEFINE_ACTOR_HIDE(Actor_Hide, Func_02005b8c)

DEFINE_SCENE_RUN_UNLESS_FLAG(Scene_RunUnlessFlag, 0x200, Func_0200057c)

void Func_0200324c(void) {
  u32 i;
  s32 zero;

  for (i = 0; i <= 1; i++) {
    s32 id = i + 15;
    struct KibomboObject *object = Actor_GetById(id);

    Actor_Hide(id);
    zero = 0;
    object->motionFlags = zero;
  }
  Actor_GetById(15)->y = PIXELS(32);
  Actor_GetById(16)->y = zero;
  Scene_RunUnlessFlag();
}

void Func_02003288(void) {
  u32 i;

  for (i = 0; i <= 1; i++) {
    s32 id = i + 26;
    struct KibomboObject *object = Actor_GetById(id);

    Actor_Hide(id);
    object->motionFlags = 0;
  }
  Scene_RunUnlessFlag();
}

/// 2 copies: kibombo/s2f34, kibombo/s2f34.
#define DEFINE_FUNC_020032B0(name, n1)                                         \
  void name(void) {                                                            \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i <= 7; i++)                                                   \
      Actor_SetModeById(i + 18, n1);                                           \
  }

DEFINE_FUNC_020032B0(Func_020032b0, 5)

DEFINE_FUNC_020032B0(Func_020032c8, 1)

void Func_020032e0(void) {
  BattleEffect_LoadActionEffectResources(143, 1);
  Import_BattleEffectSetupObjectPair(17, 27);
  EventObject_Initialize();
  Field_DispatchTypeHandler(1);
  BattleEffect_StopMainActor();
  BattleEffect_CleanupSceneObjects();
}

// ---- games/tla/src/scene/kibombo/s3308.c ----
#include "types.h"
/* VIEW(s3308): routine Actor_SetPositionAndResetMotion as void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z); kibombo.h has void Actor_SetPositionAndResetMotion(struct KibomboObject *object, s32 x, s32 y, s32 z); tried as the definition's void Actor_SetPositionAndResetMotion(struct Actor *obj, s32 x, s32 y, s32 z), it does not compile in tla-ja: games/tla/include/actor.h:95: conflicting types for `Actor_SetPositionAndResetMotion'. */
struct Actor;
extern void Actor_SetPositionAndResetMotion__s3308(struct Actor *object, s32 x, s32 y, s32 z) __asm__("Actor_SetPositionAndResetMotion");
#define Actor_SetPositionAndResetMotion Actor_SetPositionAndResetMotion__s3308
/* VIEW(s3308): routine Actor_SetSpritePriority as void Actor_SetSpritePriority(u32, s32); kibombo.h has void Actor_SetSpritePriority(s32 actor, s32 variant); tried as the definition's void Actor_SetSpritePriority(u32 objectId, s32 priority), it does not compile in tla-ja: games/tla/include/actor.h:106: conflicting types for `Actor_SetSpritePriority'. */
extern void Actor_SetSpritePriority__s3308(u32, s32) __asm__("Actor_SetSpritePriority");
#define Actor_SetSpritePriority Actor_SetSpritePriority__s3308
#include "actor.h"
#undef Actor_SetSpritePriority
#undef Actor_SetPositionAndResetMotion
#include "field_sprite.h"
#include "system/vector3.h"
/* VIEW(s3308): routine Task_Add as s32 Task_Add(TaskFunc func, s32 order); kibombo.h has void Task_Add(void(*func) (void), s32 priority); tried as the definition's s32 Task_Add(TaskFunc func, s32 order), which the object already declares, it does not compile in tla-ja: games/tla/include/system/task.h:55: conflicting types for `Task_Add'. */
#define Task_Add Task_Add__s3308
/* VIEW(s3308): routine Actor_GetById as struct Actor *Actor_GetById(u32 objectId); kibombo.h has struct KibomboObject *Actor_GetById(s32 id); tried as the definition's struct Actor *Actor_GetById(u32 objectId), which the object already declares, it does not compile in tla-ja: games/tla/include/field/event.h:37: conflicting types for `Actor_GetById'. */
struct Actor;
extern struct Actor *Actor_GetById__s3308(u32 objectId) __asm__("Actor_GetById");
#define Actor_GetById Actor_GetById__s3308
/* VIEW(s3308): routine Actor_SetMoveTarget as void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z); kibombo.h has void Actor_SetMoveTarget(struct KibomboObject *object, s32 x, s32 y, s32 z); tried as the definition's void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z), it does not compile in tla-ja: games/tla/include/field/event.h:59: conflicting types for `Actor_SetMoveTarget'. */
struct Actor;
extern void Actor_SetMoveTarget__s3308(struct Actor *actor, s32 x, s32 y, s32 z) __asm__("Actor_SetMoveTarget");
#define Actor_SetMoveTarget Actor_SetMoveTarget__s3308
/* VIEW(s3308): routine Import_ActorCreate as struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z); kibombo.h has struct KibomboObject *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z); tried as the rest of the object declares it, it does not compile in tla-ja: games/tla/include/field/event.h:60: conflicting types for `Import_ActorCreate'. */
struct Actor;
extern struct Actor *Import_ActorCreate__s3308(s32 type, s32 x, s32 y, s32 z) __asm__("Import_ActorCreate");
#define Import_ActorCreate Import_ActorCreate__s3308
/* VIEW(s3308): routine Actor_SetSpritePriority as void Actor_SetSpritePriority(u32 actorId, s32 priority); kibombo.h has void Actor_SetSpritePriority(s32 actor, s32 variant); tried as the definition's void Actor_SetSpritePriority(u32 objectId, s32 priority), it does not compile in tla-ja: games/tla/include/field/event.h:105: conflicting types for `Actor_SetSpritePriority'. */
extern void Actor_SetSpritePriority__s3308(u32 actorId, s32 priority) __asm__("Actor_SetSpritePriority");
#define Actor_SetSpritePriority Actor_SetSpritePriority__s3308
/* VIEW(s3308): routine Actor_SetPositionAndResetMotion as void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z); kibombo.h has void Actor_SetPositionAndResetMotion(struct KibomboObject *object, s32 x, s32 y, s32 z); tried as the definition's void Actor_SetPositionAndResetMotion(struct Actor *obj, s32 x, s32 y, s32 z), it does not compile in tla-ja: games/tla/include/field/event.h:110: conflicting types for `Actor_SetPositionAndResetMotion'. */
struct Actor;
extern void Actor_SetPositionAndResetMotion__s3308(struct Actor *object, s32 x, s32 y, s32 z) __asm__("Actor_SetPositionAndResetMotion");
#define Actor_SetPositionAndResetMotion Actor_SetPositionAndResetMotion__s3308
/* VIEW(s3308): routine Actor_SetPartAttribute as void Actor_SetPartAttribute(struct Actor *object, s32 attribute); kibombo.h has void Actor_SetPartAttribute(struct KibomboObject *object, s32 value); tried as the definition's void Actor_SetPartAttribute(struct Actor *object, s32 attribute), which the object already declares, it does not compile in tla-ja: games/tla/include/field/event.h:112: conflicting types for `Actor_SetPartAttribute'. */
struct Actor;
extern void Actor_SetPartAttribute__s3308(struct Actor *object, s32 attribute) __asm__("Actor_SetPartAttribute");
#define Actor_SetPartAttribute Actor_SetPartAttribute__s3308
/* VIEW(s3308): routine Script_WaitForEventTimeout as void Script_WaitForEventTimeout(struct Actor *actor); kibombo.h has void Script_WaitForEventTimeout(struct KibomboObject *object); tried as the definition's void Script_WaitForEventTimeout(void *fieldObject), it does not compile in tla-ja: games/tla/include/field/event.h:115: conflicting types for `Script_WaitForEventTimeout'. */
struct Actor;
extern void Script_WaitForEventTimeout__s3308(struct Actor *actor) __asm__("Script_WaitForEventTimeout");
#define Script_WaitForEventTimeout Script_WaitForEventTimeout__s3308
/* VIEW(s3308): routine Func_080d4568 as struct Actor *Func_080d4568(void); kibombo.h has struct KibomboObject *Func_080d4568(void); tried as the definition's struct Actor *Func_080d4568(void), which the object already declares, it does not compile in tla-ja: games/tla/include/field/event.h:124: conflicting types for `Func_080d4568'. */
struct Actor;
extern struct Actor *Func_080d4568__s3308(void) __asm__("Func_080d4568");
#define Func_080d4568 Func_080d4568__s3308
#include "event_inline.h"
#undef Func_080d4568
#undef Script_WaitForEventTimeout
#undef Actor_SetPartAttribute
#undef Actor_SetPositionAndResetMotion
#undef Actor_SetSpritePriority
#undef Import_ActorCreate
#undef Actor_SetMoveTarget
#undef Actor_GetById
#undef Task_Add
extern s32 Task_Add__s3308(TaskFunc func, s32 order) __asm__("Task_Add");

s32 Resource_ResetEntry(u32 slot);

union Whirl {
  struct Actor actor;
  struct {
    u8 unused[0x62];
    u8 stage;
    u8 timer;
  } whirl;
};

void Func_02003308(struct Actor *actor) {
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
    wobble = Math_Sine((actor->timer + (s16)actor->unk66) * 9 << 9) * 6;
  object = (union Whirl *)actor;
  action = &object->actor.timer;
  centre.x = object->actor.targetX;
  centre.y = object->actor.targetY;
  centre.z = object->actor.targetZ;
  radius = object->actor.unk4c;
  spin = (s16)object->actor.unk66;
  turn = *action;
  Vector_AddPolarOffsetFar((radius << 16) + wobble, turn * 3 * 256 + spin,
                           &centre);
  object->actor.x = centre.x;
  object->actor.y = centre.y;
  object->actor.z = centre.z;
  stage = &object->whirl.stage;
  switch (*stage) {
  case 0:
    *action += 4;
    object->actor.unk4c = 1;
    ((struct FieldSprite *)object->actor.sprite)->rotation += 0x1000;
    object->actor.scaleX += 0x666;
    object->actor.scaleY += 0x666;
    if (object->actor.scaleX > 0x18000 &&
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
      Sound_Play(220);
      *stage = 2;
    }
    break;
  case 2:
    *action += 1;
    object->actor.targetY += 0x40000;
    if (object->actor.targetY > 0x640000)
      *stage = 3;
    break;
  case 3:
    *action += 1;
    object->actor.unk4c += 2;
    *timer = 10;
    if (object->actor.unk4c > 40)
      *stage = 4;
    break;
  case 4:
    *action += 1;
    if (--*timer == 0)
      *stage = 5;
    break;
  case 5:
    *action += 1;
    object->actor.unk4c -= 2;
    if (object->actor.targetY < 0x880000)
      object->actor.targetY += 0x20000;
    if (object->actor.unk4c <= 1) {
      object->actor.unk4c = 0;
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
    Resource_ResetEntry(
        ((struct FieldSprite *)object->actor.sprite)->vramBlock);
    Import_ActorDestroy(&object->actor);
    break;
  }
}

// ---- games/tla/src/scene/kibombo/s3504.c ----
// Scene code of overlay 0x670, in ROM order, from s3504's offset up to the next object the overlay links.

s32 Func_02005c44(struct FieldSprite *sprite, s32 previous);

void Func_02003504(void) {
  struct KibomboObject *center = Actor_GetById(17);
  struct KibomboObject *spark;
  s32 previous = 0;
  u32 i;
  u32 angle;
  s32 x;
  s32 y;
  s32 z;

  Sound_Play(154);
  for (i = 0; i <= 7; i++) {
    Actor_SetPartAttribute(Actor_GetById(17), 7);
    wait(2);
    Actor_SetPartAttribute(Actor_GetById(17), 0);
    wait(2);
  }
  Sound_Play(209);
  i = 0;
  angle = 0;
  do {
    x = center->x;
    y = center->y + 0x180000;
    z = center->z;
    spark = Import_ActorCreate(673, x, y, z);
    if (spark != 0) {
      previous = Func_02005c44(spark->sprite, previous);
      spark->motionFlags = 0;
      spark->sprite->priority = 1;
      Actor_SetSingleChildFlags(spark, 0);
      Func_02005a24(spark, 1);
      spark->field64 = 0;
      spark->field4c = 0;
      spark->field66 = angle / 360;
      spark->timer = 0;
      spark->targetX = center->x;
      spark->targetY = center->y + 0x180000;
      spark->targetZ = center->z;
      spark->update = Func_02003308;
    }
    angle += 0x160000;
    i++;
  } while (i <= 15);
}

void Func_02003610(void) {
  Task_RemoveFar(Func_02002ccc);
  wait(1);
  Func_02005bb4();
  Event_Wait(40);
  if (gData_02007950 != 0)
    Func_02003504();
}

void Func_02003640(s32 open, s32 hide) {
  if (open != 0) {
    Kibombo_MapCopy(49, 37, 79, 15, 3, 2);
    Kibombo_ObjSetPos(Actor_GetById(12), PIXELS(253), PIXELS(32), PIXELS(289));
    Actor_SetPositionAndResetMotion(Actor_GetById(13), PIXELS(275), PIXELS(32),
                                    PIXELS(289));
    if (hide != 0) {
      Actor_SetModeById(12, 0);
      Actor_SetModeById(13, 0);
    } else {
      Actor_SetModeById(12, 1);
      Actor_SetModeById(13, 1);
    }
  } else {
    Kibombo_MapCopy(45, 37, 79, 15, 3, 2);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  }
}

void Func_020036e0(s32 open, s32 mode) {
  if (open != 0) {
    Map_CopyCellPictureIndicesRect(41, 38, 79, 16, 3, 3);
    Kibombo_BlendStart(12, 21, 1, 1, 16, 22);
    if (mode != 0)
      Actor_SetPositionAndResetMotion(Actor_GetById(14), PIXELS(264),
                                      PIXELS(32), PIXELS(308));
    switch (mode) {
    case 0:
      Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
      break;
    case 1:
      Actor_SetModeById(14, 0);
      break;
    case 2:
      Actor_SetModeById(14, 1);
      break;
    case 3:
      Actor_SetModeById(14, 2);
      break;
    case 4:
      Actor_SetModeById(14, 3);
      break;
    case 5:
      Actor_SetModeById(14, 6);
      Actor_GetById(14)->y -= PIXELS(34);
      break;
    case 6:
      Actor_SetModeById(14, 7);
      Actor_GetById(14)->y -= PIXELS(34);
      break;
    }
  } else {
    Map_CopyCellPictureIndicesRect(45, 38, 79, 16, 3, 3);
    Kibombo_BlendStart(17, 22, 1, 1, 16, 22);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  }
}

void Func_020037e4(void) {
  struct KibomboEffectWork *work = Ram_HeapSlots->effectWork;

  if (work->field7c4 != 0) {
    if (GameFlag_Test(0x220))
      Func_02000624();
    else if (GameFlag_Test(0x221))
      Func_02000668();
    else if (GameFlag_Test(0x222))
      Func_020006ac();
    else if (GameFlag_Test(0x223))
      Func_020006f0();
  }
}

void Func_0200384c(void) {
  BattleEffect_LoadActionEffectResources(136, 1);
  Task_Add(Func_020037e4, 0x480);
  Import_BattleEffectSetupObjectPair(gMainRoutineWork.currentOwner, -1);
  EventObject_Initialize();
  Field_DispatchTypeHandler(1);
  BattleEffect_StopMainActor();
  Task_RemoveFar(Func_020037e4);
  BattleEffect_CleanupSceneObjects();
}

void Func_02003898(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo4)
    Task_RemoveFar(Func_02002c34);
  else if (scene == (s32)&SceneId_Kibombo5)
    Task_RemoveFar(Func_02002ccc);
  else if (scene == (s32)&SceneId_Kibombo6)
    Task_RemoveFar(Func_02002d64);
}

void Func_020038ec(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x202))
    Func_02003898();
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void Func_02003930(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x209;
  Func_020038ec();
}

void Func_02003948(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;
  struct KibomboObject *leader;
  s32 x;
  s32 px;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  Event_Start();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  x = leader->x;
  px = x >> 16;
  Actor_SetMoveTarget(leader, ((8 - px % 16) << 16) + x, leader->y, leader->z);
  Script_WaitForEventTimeout(leader);
  leader->facing = 0xc000;
  if (GameFlag_Test(0x202))
    Func_02003898();
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_02005cd0);
  wait(12);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void Func_02003a08(s32 x, s32 z) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;
  struct KibomboObject *leader = Actor_GetById(party->currentOwner);

  Event_Start();
  Event_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, x, z);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  Actor_SetMoveTarget(leader, x << 16, PIXELS(-12), z << 16);
  Script_WaitForEventTimeout(leader);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

#include "input.h"

void Func_02003ac8(void) {
  struct KibomboObject *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 42 && z == 23 && (gInput.held & 64) == 0)
    return;
  if (x == 43 && z == 22 && (gInput.held & 32) == 0)
    return;
  Func_02003a08(680, 352);
}

/// 2 copies: kibombo/s2f34, kibombo/s2f34.
#define DEFINE_FUNC_02003B20(name, n1, n2, n3, n4, n5, n6, n7)                 \
  void name(void) {                                                            \
    struct KibomboObject *leader =                                             \
        Actor_GetById(gMainRoutineWork.currentOwner);                          \
    s32 x = leader->x >> 20;                                                   \
    s32 z = leader->z >> 20;                                                   \
                                                                               \
    if (z == n1) {                                                             \
      if (x == n2 && (gInput.held & 16) == 0)                                  \
        return;                                                                \
      if (x == n3 && (gInput.held & 32) == 0)                                  \
        return;                                                                \
    } else if (x == n4 && z == n5 && (gInput.held & 64) == 0) {                \
      return;                                                                  \
    }                                                                          \
    Func_02003a08(n6, n7);                                                     \
  }

DEFINE_FUNC_02003B20(Func_02003b20, 38, 23, 25, 24, 39, 392, 608)

DEFINE_FUNC_02003B20(Func_02003b88, 54, 42, 44, 43, 55, 696, 864)

void Func_02003bf0(void) {
  if (GameFlag_Test(0x202))
    Func_02003898();
  Func_02003a08(376, 288);
}

/// Fades the screen in and runs Func_02002d64 each frame for up to 160 frames,
/// or until A, B or Start is held, then fades out.
void Func_02003c14(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  KIBOMBO_TASK_ADD(Func_02002d64, 0x480);
  Event_Wait(20);
  // As a for loop, loop.c keeps gInput's address and the mask in registers
  // across the wait; the native builds both again each frame (differ 18).
  i = 0;
  goto test;
next:
  i++;
test:
  if (i > 159)
    goto done;
  Event_Wait(1);
  if ((gInput.held & 11) == 0)
    goto next;
done:
  Task_RemoveFar(Func_02002d64);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(11);
}

void Func_02003c84(void) {
  Sound_Play(212);
  Kibombo_FadeTo(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_FadeTo(0x10000, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(60);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  Sound_Play(212);
  Kibombo_FadeTo(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_CamMove(146 << 18, -1, PIXELS(248), 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  Kibombo_FadeTo(0x10000, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(60);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  Sound_Play(212);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Camera_MoveWithinMapBounds(PIXELS(248), -1, 134 << 18, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(1);
  wait(1);
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(180);
  Task_RemoveFar(Func_02002d64);
  wait(1);

  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 1;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Import_EventSetExitRequest(10);
}

void Func_020032b0(void);

void Func_02003dec(void) {
  struct KibomboObject *leader;

  Kibombo_CamMove(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Event_SetMessageId((s32)MsgAtLastThe);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(78);
  Kibombo_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Kibombo_ObjWalk(gMainRoutineWork.currentOwner, 86, 455);
  Actor_SetHorizontalPositionWithTerrain(26, PIXELS(86), PIXELS(455));
  wait(1);
  Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
  Kibombo_ObjWalk(26, 72, 455);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Kibombo_CamMove(PIXELS(264), PIXELS(16), PIXELS(420), 1);
  Func_02005bb4();
  Event_Wait(40);
  Camera_SetSpeedParameters(0x6666, 0xccc);
  Kibombo_CamMove(PIXELS(264), PIXELS(32), PIXELS(320), 1);
  Func_02005bb4();
  Event_Wait(80);
  Actor_Jump(17, 4, 40);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_CamSpeed(0x4cccc, 0x9999);
  Kibombo_CamMove(PIXELS(264), PIXELS(32), PIXELS(400), 1);
  Func_02005bb4();
  Event_Wait(20);
  Kibombo_ObjArm(17, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(0x4000 + 17, 0);
  Sound_Play(11);
  GameFlag_SetBit(0x8f2);
  Sound_Play(148);
  Actor_GetById(18)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(19)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(20)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(21)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(22)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(23)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(24)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(25)->velocityY = PIXELS(4);
  wait(20);
  Actor_ArmCallback(17, FACING_NORTH + FACING_STEP, 20);
  Func_020032b0();
  Event_Wait(40);
  Camera_SetSpeedParameters(0x3333, 0x666);
  Camera_MoveWithinMapBounds(PIXELS(264), 0, PIXELS(400), 1);
  Func_02005bb4();
  Event_Wait(200);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Camera_MoveWithinMapBounds(PIXELS(111), PIXELS(32), PIXELS(456), 1);
  Func_02005bb4();
  Actor_ArmCallback(26, FACING_EAST, 0);
  Event_ShowMessageAndWait(26, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 20);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(26, 4);
  Event_ShowMessageAndWait(26, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(26, 3);
  Actor_SetModeById(26, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(26, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(26);
  Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
  GameFlag_SetBit(0x8f1);
}

void Func_02002eb0(void);

void Func_020040cc(void) {
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  Event_Wait(40);
  Sound_Play(78);
  Kibombo_CamSpeed(0xcccc, 0x1999);
  Kibombo_CamMove(PIXELS(264), -1, PIXELS(388), 1);
  Func_02005bb4();
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_SetMessageId((s32)MsgLetUsNow);
  Event_ShowMessageAndWait(0x4000 + 17, 0);
  Sound_Play(26);
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(27), PIXELS(265), PIXELS(24),
                                  PIXELS(344));
  Camera_SetSpeedParameters(0x6666, 0xccc);
  Camera_MoveWithinMapBounds(PIXELS(264), -1, PIXELS(340), 1);
  Kibombo_ObjSpeed(17, 0x3333, 0x1999);
  Actor_SetPositionAndReset(17, 264, 374);
  wait(1);
  Kibombo_TaskAdd(Func_02002eb0, 0x480);
  Event_Wait(20);
  Task_RemoveFar(Func_02002ccc);
  Func_020032e0();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(11);
  Func_02003610();
  Event_Wait(160);
  Event_ShowMessageThenWait(17, 0, 80);
  Sound_Play(0x240);
  Func_02003640(1, 1);
  Event_Wait(20);
  Event_ShowMessageThenWait(17, 0, 40);
  Task_RemoveFar(Func_02002ccc);
  wait(1);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x202;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(10);
}

void Func_02004294(void) {
  struct KibomboObject **slot;
  struct KibomboObject *object;
  s32 y;

  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Actor_SetSingleChildFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(27), PIXELS(265), PIXELS(24),
                                  PIXELS(344));
  Func_02003640(1, 1);
  wait(1);
  if (gData_02007950 != 0) {
    gData_02007950->y += 0x200000;
    wait(1);
    gData_02007950->motionFlags = 4;
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Event_Wait(40);
  Sound_Play(161);
  Func_02003640(0, 1);
  Event_Wait(20);
  FieldEffect_SetAttached(17, 0x102);
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouAreStill);
  Event_ShowMessageAndWait(17, 0);
  slot = &gData_02007950;
  object = *slot;
  if (object != 0) {
    // Lowers the object 0x4000 a frame down to y 0x240000; the loop is
    // entered at its test, which reuses the pointer read for the test above.
    object->motionFlags = 0;
    goto test;
    do {
      Kibombo_ObjArm(17, FACING_NORTH + FACING_STEP, 0);
      slot = &gData_02007950;
      (*slot)->y += 0xffffc000;
      wait(1);
      object = *slot;
    test:
      y = object->y;
      /* FAKEMATCH(barrier): an empty asm on the height, so the limit's first
       * half is built before it loads; as object->y > 0x240000, 2 of 151
       * instructions differ in tla-en. */
      __asm__("" : "+l"(y));
    } while (y > 0x240000);
  }
  Motion_SetModeAndWaitAnimation(17, 4);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(0x4000 + 17, 0);
  GameFlag_ClearBit(0x8f7);
  Sound_Play(11);
  Event_Wait(80);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(11);
}

void BattleEffect_ApplyColorToSourceBuffer(s32 a, s32 b);

void Func_02004424(void) {
  struct KibomboEventWork *work;
  s32 effect;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Func_020032b0();
  KIBOMBO_TASK_ADD(Func_02002ccc, 1152);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = effect = 513;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Event_SetMessageId((s32)MsgOhTheMagic);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjTurn(17, FACING_SOUTH + FACING_STEP);
  Event_ShowMessageAndWait(0x4011, 0);
  Task_RemoveFar(Func_02002ccc);
  Kibombo_ObjTurn(17, FACING_NORTH + FACING_STEP);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(0x109);
  Import_EventSetExitRequest(12);
}

void Func_02004524(void) {
  struct KibomboEventWork *work;

  Event_Start();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Func_080d4568()->motionFlags = 0;
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Func_02003640(1, 1);
  if (gData_02007950 != 0) {
    gData_02007950->y += 0x200000;
    wait(1);
    gData_02007950->motionFlags = 4;
  }
  wait(1);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
}

extern s32 gData_020061a8[];

void Actor_Stop(s32 actor);

void Func_020045d0(void) {
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Kibombo_CamMove(132 << 17, 0, 200 << 17, 1);
  Func_02005bb4();
  Event_Wait(10);
  Actor_Jump(18, 6, 0);
  Actor_Jump(19, 6, 0);
  Actor_Jump(20, 6, 0);
  Actor_Jump(21, 6, 0);
  Actor_Jump(22, 6, 0);
  Actor_Jump(23, 6, 0);
  Actor_Jump(24, 6, 0);
  Actor_Jump(25, 6, 40);
  Kibombo_ObjTurn(20, FACING_SOUTH + FACING_STEP);
  Actor_Jump(20, 2, 0);
  Event_ShowMessageAndWait(0x4000 + 20, 0);
  Kibombo_ObjTurn(23, FACING_SOUTHEAST + FACING_STEP);
  Actor_Jump(23, 2, 0);
  Event_ShowMessageAndWait(0x4000 + 23, 0);
  Actor_SetMovementTypeById(18, gData_020061a8);
  Actor_SetMovementTypeById(19, gData_020061a8);
  Actor_SetMovementTypeById(20, gData_020061a8);
  Actor_SetMovementTypeById(21, gData_020061a8);
  Actor_SetMovementTypeById(22, gData_020061a8);
  Actor_SetMovementTypeById(23, gData_020061a8);
  Actor_SetMovementTypeById(24, gData_020061a8);
  Actor_SetMovementTypeById(25, gData_020061a8);
  Event_Wait(40);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Actor_Jump(17, 2, 10);
  Actor_Jump(17, 4, 10);
  Event_ShowMessageAndWait(17, 0);
  Actor_Stop(18);
  Actor_Stop(19);
  Actor_Stop(20);
  Actor_Stop(21);
  Actor_Stop(22);
  Actor_Stop(23);
  Actor_Stop(24);
  Actor_Stop(25);
  Event_Wait(40);
  Kibombo_ObjArm(18, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(19, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(20, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(21, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(22, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(23, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(24, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(25, FACING_NORTHWEST + FACING_STEP, 0);
  Func_080d47b4(17, 0x107, 0);
  Kibombo_MessagePut(0x8000 + 17, 0);
  Kibombo_MessagePut(0x8000 + 17, 0);
  Motion_SetModeAndWaitAnimation(17, 3);
  Kibombo_MessagePut(0x8000 + 17, 0);
  Actor_Jump(17, 4, 10);
  Event_ShowMessageAndWait(0x8000 + 17, 0);
}

void Func_020047e4(void) {
  struct KibomboObject **slot;
  struct KibomboObject *object;

  Func_02004524();
  Sound_Play(161);
  Func_02003640(0, 1);
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouAreStillNot);
  Event_ShowMessageAndWait(17, 0);
  object = gData_02007950;
  if (object != 0) {
    // Lowers the object 0x4000 a frame down to y 0x240000.
    object->motionFlags = 0;
    if (object->y > 144 << 14) {
      do {
        Actor_ArmCallback(17, FACING_NORTH + FACING_STEP, 0);
        slot = &gData_02007950;
        object = *slot;
        object->y += 0xffffc000;
        wait(1);
      } while ((*slot)->y > 0x240000);
    }
  }
  Motion_SetModeAndWaitAnimation(17, 4);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(0x4011, 0);
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Import_EventSetExitRequest(12);
}

void Func_0200489c(void) {
  Func_02004524();
  Sound_Play(0x240);
  Actor_SetPartAttribute(Actor_GetById(12), 7);
  Actor_SetPartAttribute(Actor_GetById(13), 7);
  wait(8);
  Actor_SetPartAttribute(Actor_GetById(12), 0);
  Actor_SetPartAttribute(Actor_GetById(13), 0);
  wait(8);
  Func_080d47b4(17, 0x100, 20);
  Event_SetMessageId((s32)MsgLookLookThe);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(0x241);
  Actor_SetMovementTypeById(12, gData_0200614c);
  Actor_SetMovementTypeAndWaitById(13, gData_0200614c);
  Actor_SetMovementTypeById(12, gData_0200613c);
  Actor_SetMovementTypeById(13, gData_0200613c);
  Actor_SetPartAttribute(Actor_GetById(12), 12);
  Actor_SetPartAttribute(Actor_GetById(13), 12);
  Event_Wait(120);
  Func_020045d0();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Import_EventSetExitRequest(12);
}

void Func_02004978(void) {
  Func_02004524();
  Kibombo_SePlay(0x240);
  Actor_SetPartAttribute(Actor_GetById(12), 7);
  Actor_SetPartAttribute(Actor_GetById(13), 7);
  wait(8);
  Actor_GetById(12)->update = Func_020000a0;
  Actor_GetById(13)->update = Func_020000a0;
  Func_080d47b4(17, 0x100, 20);
  Event_SetMessageId((s32)MsgLookTheGreat);
  Event_ShowMessageThenWait(17, 0, 20);
  Sound_Play(0x240);
  Func_020036e0(1, 0);
  Event_Wait(20);
  Sound_Play(162);
  Task_Add(Func_02002f34, 0x480);
  Event_Wait(120);
  Func_020045d0();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Import_EventSetExitRequest(12);
}

void Func_02004a28(void) {
  struct KibomboObject *object;
  u32 i;

  object = Actor_GetById(14);
  Func_02004524();
  Actor_SetSpritePriority(17, 1);
  Kibombo_SePlay(0x240);
  Actor_SetPartAttribute(Actor_GetById(12), 7);
  Actor_SetPartAttribute(Actor_GetById(13), 7);
  wait(8);
  Actor_SetPartAttribute(Actor_GetById(12), 0);
  Actor_SetPartAttribute(Actor_GetById(13), 0);
  wait(8);
  Func_080d47b4(17, 0x100, 20);
  Event_SetMessageId((s32)MsgLookTheGreatGabomba);
  Event_ShowMessageThenWait(17, 0, 20);
  Kibombo_SePlay(0x240);
  Func_020036e0(1, 0);
  Event_Wait(20);
  Sound_Play(233);
  object->scaleY = 0x1999;
  Actor_SetPositionAndResetMotion(Actor_GetById(14), 0x1080000, 0x180000,
                                  0x1340000);
  wait(1);
  Actor_SetModeById(14, 3);
  for (i = 0; i < 64; i++) {
    object->scaleY += 0x400;
    object->y += 0x2000;
    wait(1);
  }
  Sound_Play(0x12a);
  if (gData_02007950 != 0) {
    gData_02007950->motionFlags = 0;
    // As a while loop, which moves the global's address out of the loop, 24
    // of 288 instructions differ in tla-en.
    if (gData_02007950->y > 0x240000) {
    lower:
      gData_02007950->y -= 0x8000;
      wait(1);
      if (gData_02007950->y > 0x240000)
        goto lower;
    }
  }
  Sound_Play(232);
  if (gData_02007950 != 0) {
    gData_02007950->motionFlags = 0;
    // As a for loop, which moves the global's address out of the loop, 25 of
    // 288 instructions differ in tla-en.
    i = 0;
  rise:
    gData_02007950->y += 0x60000;
    object->scaleY -= 0x2000;
    object->y -= 0x8000;
    wait(1);
    i++;
    if (i < 4)
      goto rise;
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    for (i = 0; i < 4; i++) {
      object->scaleY -= 0x2000;
      object->y -= 0x8000;
      wait(1);
    }
  }
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Func_020036e0(0, 0);
  Func_02003640(1, 1);
  Event_Wait(10);
  Func_02003640(0, 1);
  Event_Wait(40);
  Kibombo_SePlay(0x240);
  Func_02003640(1, 1);
  Func_020036e0(1, 0);
  Sound_Play(132);
  Actor_SetPositionAndResetMotion(Actor_GetById(14), 0x1080000, 0x180000,
                                  0x1340000);
  wait(1);
  for (i = 0; i < 8; i++) {
    object->scaleY += 0x2000;
    object->y += 0x8000;
    wait(1);
  }
  FieldEffect_SetAttached(17, 0x102);
  Event_Wait(40);
  Kibombo_ObjTurn(17, 0x3000);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjEmote(17, 0x108, 20);
  Kibombo_MessagePut(0x4011, 0);
  Motion_SetModeAndWaitAnimation(17, 3);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjSpeed(17, 0xcccc, 0x6666);
  Kibombo_ObjWalk(17, 0x108, 0x140);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x203;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x8fb);
  Import_EventSetExitRequest(13);
}

void Func_02004cf4(void) {
  struct KibomboEventWork *work;
  struct KibomboObject *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Camera_MoveWithinMapBounds(PIXELS(384), -1, PIXELS(601), 0);
  wait(1);
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      gMainRoutineWork.currentOwner, PIXELS(370), PIXELS(636), FACING_NORTH);
  Map_CenterOnFollowedPoint();
  wait(1);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  BattleEffect_ApplyColorToSourceBuffer(0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10002, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Kibombo_ObjArm(10, FACING_NORTHWEST + FACING_STEP, 20);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(8, 3);
  Kibombo_ObjArm(10, FACING_NORTHWEST + FACING_STEP, 20);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 20);
  Actor_GetById(10)->velocityY = PIXELS(3);
  Event_Wait(40);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(8, 3);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(40);
  wait(60);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_SetMessageId((s32)MsgRightWellI);
  Event_ShowMessageAndWait(10, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 10);
  Actor_SetModeById(10, 3);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_Jump(8, 4, 10);
  Kibombo_MessagePut(0x2008, 0);
  Actor_ArmCallbackAndRefresh(9, FACING_EAST);
  Func_080d47b4(8, 259, 20);
  Kibombo_MessagePut(0x2008, 0);
  Func_080d47b4(9, 256, 20);
  Event_ShowMessageAndWait(9, 0);
  Kibombo_ObjTurn(8, FACING_WEST);
  Actor_SetModeById(8, 4);
  Kibombo_MessagePut(0x2008, 0);
  Kibombo_ObjTurn(8, FACING_SOUTH + FACING_STEP);
  Kibombo_MessagePut(0x2008, 0);
  Actor_SetModeById(8, 4);
  Event_ShowMessageAndWait(0x2008, 0);
  Kibombo_ObjArm(9, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjSpeed(8, 0x19999, 0xcccc);
  Kibombo_ObjWalk(8, 364, 576);
  Kibombo_ObjArm(8, FACING_NORTHWEST + FACING_STEP, 0);
  Func_080d47b4(9, 261, 80);
  Event_ShowMessageAndWait(9, 0);
  Kibombo_ObjTurn(9, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_ShowMessageAndWait(10, 0);
  Actor_ArmCallbackAndRefresh(10, FACING_EAST);
  Event_ShowMessageAndWait(10, 0);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_WEST);
  Actor_SetModeById(10, 3);
  Event_ShowMessageAndWait(10, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Kibombo_ObjSpeed(10, 0xcccc, 0x6666);
  Actor_SetModeById(10, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(10, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(10);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  work = Ram_HeapSlots->eventWork;
  work->field1b4 = 16;
  work->screenEffect = 0x209;
  GameFlag_SetBit(0x8ef);
  Event_End();
}

s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo1)
    Func_02005114();
  else if (scene == (s32)&SceneId_Kibombo4)
    Func_0200516c();
  else if (scene == (s32)&SceneId_Kibombo2)
    Func_020054e0();
  else if (scene == (s32)&SceneId_Kibombo5)
    Func_02005594();
  else if (scene == (s32)&SceneId_Kibombo6)
    Func_020057e0();
  else if (scene == (s32)&SceneId_Kibombo3)
    Func_02005898();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_020050ac(void) {
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  Actor_SetSingleChildFlags(Actor_GetById(10), 0);
  Func_02005c04(0);
  Func_02005c64(gData_02006324);
  Actor_SetSpritePriority(8, 3);
  Func_02005c54();
  Func_02005c5c(0, 0x9e9, 12, 13);
  Actor_GetById(22)->collisionFlags |= 0x80;
  Actor_GetById(23)->collisionFlags |= 0x80;
}

void Func_02005114(void) {
  Actor_SetHorizontalPositionWithTerrain(11, PIXELS(88), PIXELS(314));
  Actor_SetSingleChildFlags(Actor_GetById(11), 0);
  Actor_GetById(11)->scaleY = 0x8000;
  Func_020050ac();
  GameFlag_ClearBit(0x86b);
  Func_02005b8c(25);
  Actor_SetModeById(25, 2);
  KIBOMBO_TASK_ADD(Func_02003030, 0x480)
  Func_02003184();
}

// ---- games/tla/src/scene/kibombo/s516c.c ----
// Kibombo's setup.
#include "main_routine_work.h"
/* VIEW(s516c): routine Actor_SetSingleChildFlags as void Actor_SetSingleChildFlags(struct Actor *object, u32 flags); kibombo.h has void Actor_SetSingleChildFlags(struct KibomboObject *object, u32 value); tried as the 35 declarations' void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags), it does not compile in tla-ja: games/tla/include/field/field.h:25: conflicting types for `Actor_SetSingleChildFlags'. */
struct Actor;
extern void Actor_SetSingleChildFlags__s516c(struct Actor *object, u32 flags) __asm__("Actor_SetSingleChildFlags");
#define Actor_SetSingleChildFlags Actor_SetSingleChildFlags__s516c
/* VIEW(s516c): routine Animation_SetStateFlags as void Animation_SetStateFlags(struct Actor *object, u32 value); kibombo.h has void Animation_SetStateFlags(struct KibomboObject *object, s32 flags); tried as the definition's void Animation_SetStateFlags(struct Actor *object, u32 value), which the object already declares, it does not compile in tla-ja: games/tla/include/field/field.h:58: conflicting types for `Animation_SetStateFlags'. */
struct Actor;
extern void Animation_SetStateFlags__s516c(struct Actor *object, u32 value) __asm__("Animation_SetStateFlags");
#define Animation_SetStateFlags Animation_SetStateFlags__s516c
/* VIEW(s516c): routine Actor_SetPartAttribute as void Actor_SetPartAttribute(struct Actor *object, s32 attribute); kibombo.h has void Actor_SetPartAttribute(struct KibomboObject *object, s32 value); tried as the definition's void Actor_SetPartAttribute(struct Actor *object, s32 attribute), which the object already declares, it does not compile in tla-ja: games/tla/include/field/field.h:93: conflicting types for `Actor_SetPartAttribute'. */
extern void Actor_SetPartAttribute__s516c(struct Actor *object, s32 attribute) __asm__("Actor_SetPartAttribute");
#define Actor_SetPartAttribute Actor_SetPartAttribute__s516c
#include "field.h"
#undef Actor_SetPartAttribute
#undef Animation_SetStateFlags
#undef Actor_SetSingleChildFlags

void *Unit_Get(s32 owner);

static __inline__ s32 Gauge(s32 value, s32 max) {
  s32 gauge = (value << 14) / max;

  s32 result;

  if (gauge > 0x4000)
    result = 0x4000;
  else {
    result = 0;
    if (gauge >= 0)
      result = gauge;
  }
  return result;
}

/// Unless flag 265 is set, places actor 11 and, arriving by entrance 19 with
/// unit 7 in the party (flag 7), restores unit 7's HP and PP, sets their
/// gauges, clears two of its bytes, sets flag 0x92f, clears 0x92e and removes
/// it from the party; then saves four palette colours, frees actors 30 to 35,
/// sets flags 0x202 and 0x209, places actor 29, places actors 21 and 26 to 28
/// when flag 0x8fd is set, sets flag 0x86b and fades in with Func_02002c34
/// scheduled.
void Func_0200516c(void) {
  struct Actor *actor = Actor_GetById(11);
  u8 *unit;
  s32 done;

  Event_Start();
  Event_ResolvePendingActions(0);
  done = GameFlag_Test(265);
  if (!done) {
    actor->motionFlags = done;
    actor->y = PIXELS(42);
    actor->z += PIXELS(-42);
    Actor_SetSingleChildFlags__s516c(Actor_GetById(11), 0);
    Actor_SetSpritePriority(11, 1);
    if (gMainRoutineWork.entrance == 19 && GameFlag_Test(7)) {
      unit = Unit_Get(7);
      *(u16 *)(unit + 0x38) = *(u16 *)(unit + 0x34);
      *(u16 *)(unit + 0x3a) = *(u16 *)(unit + 0x36);
      /* FAKEMATCH(dead): both copies stored before the gauge's argument
       * is extended; without it, 2 of 218 instructions differ in tla-en. */
      do {
      } while (0);
      *(u16 *)(unit + 0x14) =
          Gauge(*(s16 *)(unit + 0x38), *(s16 *)(unit + 0x34));
      if (*(u16 *)(unit + 0x14) == 0 && *(s16 *)(unit + 0x38) != 0) {
        s32 one = 1;

        *(s16 *)(unit + 0x14) = one;
      }
      *(u16 *)(unit + 0x16) =
          Gauge(*(s16 *)(unit + 0x3a), *(s16 *)(unit + 0x36));
      if (*(u16 *)(unit + 0x16) == 0 && *(s16 *)(unit + 0x3a) != 0) {
        s32 one = 1;

        *(s16 *)(unit + 0x16) = one;
      }
      unit[0x131] = 0;
      unit[0x140] = 0;
      GameFlag_SetBit(0x92f);
      GameFlag_ClearBit(0x92e);
      Party_RemoveMemberRestored(7);
    }
  }
  GameFlag_SetBit(0x202);
  gData_02007948[0] = *(u16 *)&BG_PLTT_COLOR(0, 9);
  gData_02007948[1] = *(u16 *)&BG_PLTT_COLOR(5, 7);
  gData_02007948[2] = *(u16 *)&BG_PLTT_COLOR(6, 7);
  gData_02007948[3] = *(u16 *)&BG_PLTT_COLOR(7, 7);
  Func_020050ac();
  Actor_SetSingleChildFlags__s516c(Actor_GetById(30), 0);
  Actor_SetSingleChildFlags__s516c(Actor_GetById(31), 0);
  Actor_SetSingleChildFlags__s516c(Actor_GetById(32), 0);
  Actor_SetSingleChildFlags__s516c(Actor_GetById(33), 0);
  Actor_SetSingleChildFlags__s516c(Actor_GetById(34), 0);
  Actor_SetSingleChildFlags__s516c(Actor_GetById(35), 0);
  GameFlag_SetBit(0x209);
  actor = Actor_GetById(29);
  actor->motionFlags = 0;
  actor->y = 0;
  Actor_Hide(29);
  Actor_SetSpritePriority(30, 3);
  if (GameFlag_Test(0x8fd)) {
    Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(28, 0, 0);
    Actor_SetPosition(21, PIXELS(302), PIXELS(172));
  }
  GameFlag_SetBit(0x86b);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  {
    s32 priority = 0x480;

    Task_Add(Func_02002c34, priority);
  }
  Event_End();
}

// ---- games/tla/src/scene/kibombo/s538c.c ----
// Scene code of overlay 0x670, in ROM order, from s538c's offset up to the next object the overlay links and the overlay's data.

void Func_02002de0(void);
void Func_0200324c(void);

void Func_0200538c(void) {
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  Actor_SetSingleChildFlags(Actor_GetById(10), 0);
  if (!GameFlag_Test(0x8fb))
    Func_02002de0();
  Func_0200324c();
  if (!GameFlag_Test(0x869))
    GameFlag_SetBit(0x201);
  Func_02005c64(gData_02006324);
  Actor_SetSpritePriority(8, 2);
  Actor_SetSingleChildFlags(Actor_GetById(11), 0);
  Actor_GetById(11)->motionFlags = 0;
  if (GameFlag_Test(0x9ea)) {
    Kibombo_ObjSetPos(Actor_GetById(11), 208 << 15, 0, 148 << 17);
    Kibombo_BlendStart(5, 18, 1, 1, 6, 18);
    wait(10);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  if (GameFlag_Test(0x86a))
    Func_020005e0();
  if (GameFlag_Test(0x204))
    Func_02000624();
  if (GameFlag_Test(0x205))
    Func_02000668();
  if (GameFlag_Test(0x206))
    Func_020006ac();
  if (GameFlag_Test(0x207))
    Func_020006f0();
  Func_02005b8c(12);
  Func_02005b8c(13);
  Func_02005b8c(14);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  Actor_SetSpritePriority(14, 1);
}

void Func_020054e0(void) {
  s16 entrance;

  Func_0200538c();
  if (GameFlag_Test(0x8fb)) {
    Func_02003640(1, 0);
    Func_020036e0(1, 5);
  }
  wait(1);
  entrance = gMainRoutineWork.entrance;
  if (entrance == 99) {
    Func_02003640(0, 0);
    Func_020036e0(0, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 98) {
    Func_02003640(1, 0);
    Func_020036e0(1, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 97) {
    Func_02003640(1, 0);
    Func_020036e0(1, 2);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 96) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    Actor_SetModeById(14, 4);
  }
}

void Func_020040cc(void);
void Func_02004294(void);
void Func_02004424(void);
void Func_020047e4(void);
void Func_0200489c(void);
void Func_02004978(void);
void Func_02004a28(void);
void Func_02003dec(void);

void Func_02005594(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_SetBit(0x869);
  GameFlag_SetBit(0x202);
  gData_02007948[0] = BG_PLTT[9];
  gData_02007948[1] = BG_PLTT[7];
  gData_02007948[2] = BG_PLTT[6];
  gData_02007948[3] = BG_PLTT[76];
  Func_0200538c();
  if (GameFlag_Test(0x8fb)) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
  }
  wait(1);
  switch (gMainRoutineWork.entrance) {
  case 10:
    Func_020040cc();
    return;
  case 11:
    Func_02004294();
    return;
  case 12:
    Func_02004424();
    return;
  case 13:
    Func_020047e4();
    return;
  case 14:
    Func_0200489c();
    return;
  case 15:
    Func_02004978();
    return;
  case 16:
    Func_02004a28();
    return;
  }
  Actor_SetSpritePriority(17, 1);
  Actor_SetSpritePriority(7, 2);
  Actor_SetSpritePriority(26, 2);
  Actor_SetSpritePriority(5, 2);
  Actor_SetSpritePriority(6, 2);
  if (GameFlag_Test(0x92e))
    Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  else if (GameFlag_Test(0x92f)) {
    s32 angle = 0x2000;

    Kibombo_ObjPlaceFacing(7, 152 << 17, 176 << 15, angle);
  }
  if (GameFlag_Test(0x8fd)) {
    Func_020036e0(1, 6);
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Task_Add(Func_02002ccc, 0x480);
  for (i = 0; i <= 7; i++) {
    u8 *field = &Actor_GetById(i + 18)->field5c;
    s32 value = 2;

    *field = value;
  }
  if (GameFlag_Test(0x8f1)) {
    if (!GameFlag_Test(0x8fd))
      Func_020032b0();
  } else if (gMainRoutineWork.entrance == 5) {
    Func_02003dec();
  }
  Event_End();
  if (gMainRoutineWork.entrance == 99) {
    Func_02003640(0, 0);
    Func_020036e0(0, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 98) {
    Func_02003640(1, 1);
    Func_020036e0(1, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 97) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 96) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    Actor_SetModeById(14, 5);
  }
}

void Func_02003c14(void);
void Func_02003c84(void);

#define PALETTE_WORD(index) (*(u16 *)(0x05000000 + (index) * 2))

void Func_020057e0(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_SetBit(514);
  gData_02007948[0] = PALETTE_WORD(169);
  gData_02007948[1] = PALETTE_WORD(167);
  gData_02007948[2] = PALETTE_WORD(166);
  if (gMainRoutineWork.entrance == 10) {
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                       0);
    Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
    Func_02003c84();
  } else if (gMainRoutineWork.entrance == 11) {
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                       0);
    Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
    Func_02003c14();
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    KIBOMBO_TASK_ADD(Func_02002d64, 1152);
    Event_End();
  }
}

#include "version.h"
#include "fixed_math.h"

void Func_02003288(void);
void Func_02004cf4(void);
void Func_02005c74(s32 actor, s32 angle);

void Func_02005898(void) {
  struct KibomboObject *object;

  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Func_02003288();
  if (gMainRoutineWork.entrance == 9) {
    if (GameFlag_Test(0x8ef)) {
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
      if (GameFlag_Test(0x8ee)) {
        /* FAKEMATCH(temporary): the angle in a local of each branch, built
         * before the position; as the constant passed to
         * Kibombo_ObjPlaceFacing, 26 of 105 instructions differ in tla-en. */
        s32 angle = FACING_NORTHWEST + FACING_STEP;

        Kibombo_ObjPlaceFacing(8, PIXELS(434), PIXELS(634), angle);
      } else {
        s32 angle = FACING_NORTHWEST + FACING_STEP;

        Kibombo_ObjPlaceFacing(8, PIXELS(364), PIXELS(576), angle);
      }
    } else {
      Func_02004cf4();
    }
  }
#if TLA_PATCH_1
  if (GameFlag_Test(0x109)) {
    object = Actor_GetById(16);
    object->x -= PIXELS(14);
    object->z -= PIXELS(8);
  }
#endif
  Func_02005c74(16, 0);
  object = Actor_GetById(16);
  object->sprite->rotation = 0;
  object->scaleX = Q16_ONE;
  object->scaleY = Q16_ONE;
  object->x += PIXELS(14);
  object->z += PIXELS(8);
  Actor_SetModeById(16, 6);
  if (gMainRoutineWork.entrance == 14)
    GameFlag_SetBit(0x8ee);
}

void Func_02005994(void) { Func_02005bcc(); }

#include "text/msg_ids.h"
#include "objcmd.h"

void Func_02000038(void);
void Func_02000048(void);
void Func_02000054(void);
void Func_020000c4(void);
void Func_02000228(void);
void Func_0200024c(void);
void Func_02000270(void);
void Func_020002c4(void);
void Func_02000318(void);
void Func_02000350(void);
void Func_02000388(void);
void Func_020003c0(void);
void Func_020003f8(void);
void Func_0200042c(void);
void Func_020004dc(void);
void Func_02000500(void);
void Func_020005d0(void);
void Func_02000618(void);
void Func_0200065c(void);
void Func_020006a0(void);
void Func_020006e4(void);
void Func_02000728(void);
void Func_02000734(void);
void Func_020007c8(void);
void Func_02000c90(void);
void Func_02001218(void);
void Func_02002050(void);
void Func_02002ad8(void);
void Func_02002af8(void);
void Func_02003930(void);
void Func_02003948(void);
void Func_02003ac8(void);
void Func_02003b20(void);
void Func_02003b88(void);
void Func_02003bf0(void);

// clang-format off
u32 gData_02005ca4[] = {
    0, 28, 22, 38, 0, 0,
    1, 3, 0, 1, 38,
};

DEFINE_G_DATA_0200121C(gData_02005cd0)

DEFINE_G_DATA_02002FCC(gData_02005d18, 98, 408, 1)

DEFINE_G_DATA_02003534(gData_02005d5c, 80, 376, 1)

s32 gData_02005da0[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(68), 0, PIXELS(392)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
    SC_ADD(SW_SCALE_X, 0x800),
    SC_ADD(SW_SCALE_Y, 0x800),
    SC_CALL(Func_02000038),
    SC_WAIT(1),
    SC_LOOP(10, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -2048),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_CALL(Func_02000038),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_FREE,
};

u32 gData_02005e54[] = {
    39, 2, 4, 0x580000, 0, 0x1880000,
    1, 39, 1, 3, 0, 1,
    17,
};

u32 gData_02005e88[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(72), 0, PIXELS(296)),
    SC_SYNC,
    SC_MOVE(PIXELS(40), 0, PIXELS(340)),
    SC_SYNC,
    SC_MOVE(PIXELS(40), 0, PIXELS(356)),
    SC_SYNC,
    SC_MOVE(PIXELS(56), 0, PIXELS(372)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

DEFINE_G_DATA_02002F1C(gData_02005eec, 88, 372)

DEFINE_G_DATA_02003534(gData_02005f14, 48, 384, 1)

DEFINE_G_DATA_02003534(gData_02005f58, 48, 360, 1)

DEFINE_G_DATA_02003534(gData_02005f9c, 36, 372, 1)

u32 gData_02005fe0[] = {
    39, 2, 4, 0x380000, 0, 0x1740000,
    1, 39, 1, 3, 0, 1,
    17,
};

DEFINE_G_DATA_02002F88(gData_02006014, 298, 200, 1)

DEFINE_G_DATA_02002F88(gData_02006058, 308, 188, 1)

DEFINE_G_DATA_02002F88(gData_0200609c, 298, 176, 1)

DEFINE_G_DATA_02002F1C(gData_020060e0, 222, 184)

u32 gData_02006108[] = {
    39, 2, 4, 0x11c0000, 0, 0xbc0000,
    1, 39, 1, 3, 0, 1,
    17,
};

s32 gData_0200613c[] = {
    46,
    (s32)Func_02000048,
    46,
    (s32)Func_02000054,
};

s32 gData_0200614c[] = {
    SC_SET(SW_SCALE_X, 0x18000),
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_WAIT(20),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_Y, PIXELS(2)),
    SC_WAIT(1),
    SC_END,
};

s32 gData_020061a8[] = {
    22,
    39,
    0,
    46,
    (s32)Func_020000c4,
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

s16 gData_02006324[] = {
    8, 513, -1, 0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gData_0200635c[] = {
    400, 40, 368, 416, 56, 384, -1, 14,
    112, 40, 624, 128, 56, 640, -1, 15,
    384, 40, 880, 400, 56, 896, -1, 16,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(110),
    SCENE_EXIT(1, 2, 27),
    SCENE_EXIT(2, 111, 1),
    SCENE_EXIT(3, 112, 1),
    SCENE_EXIT(4, 112, 6),
    SCENE_EXIT(5, 112, 5),
    SCENE_EXIT(6, 112, 4),
    SCENE_EXIT(7, 112, 8),
    SCENE_EXIT(8, 112, 10),
    SCENE_EXIT(9, 111, 2),
    SCENE_EXIT(10, 111, 5),
    SCENE_EXITS(113),
    SCENE_EXIT(1, 2, 27),
    SCENE_EXIT(2, 114, 1),
    SCENE_EXIT(3, 112, 1),
    SCENE_EXIT(4, 112, 6),
    SCENE_EXIT(5, 112, 5),
    SCENE_EXIT(6, 112, 4),
    SCENE_EXIT(7, 112, 8),
    SCENE_EXIT(8, 112, 10),
    SCENE_EXIT(9, 114, 2),
    SCENE_EXIT(10, 114, 5),
    SCENE_EXITS(111),
    SCENE_EXIT(1, 110, 2),
    SCENE_EXIT(2, 110, 9),
    SCENE_EXIT(3, 123, 1),
    SCENE_EXIT(4, 120, 3),
    SCENE_EXIT(5, 110, 10),
    SCENE_EXITS(114),
    SCENE_EXIT(1, 113, 2),
    SCENE_EXIT(2, 113, 9),
    SCENE_EXIT(3, 123, 1),
    SCENE_EXIT(4, 120, 3),
    SCENE_EXIT(5, 113, 10),
    SCENE_EXIT(10, 121, 2),
    SCENE_EXIT(11, 121, 3),
    SCENE_EXIT(12, 121, 1),
    SCENE_EXIT(13, 120, 5),
    SCENE_EXITS(112),
    SCENE_EXIT_IF(1, 110, 3, 0x86b),
    SCENE_EXIT_IF(1, 113, 3, -1),
    SCENE_EXIT_IF(2, 112, 3, -1),
    SCENE_EXIT_IF(3, 112, 2, -1),
    SCENE_EXIT_IF(4, 110, 6, 0x86b),
    SCENE_EXIT_IF(4, 113, 6, -1),
    SCENE_EXIT_IF(5, 110, 5, 0x86b),
    SCENE_EXIT_IF(5, 113, 5, -1),
    SCENE_EXIT_IF(6, 110, 4, 0x86b),
    SCENE_EXIT_IF(6, 113, 4, -1),
    SCENE_EXIT_IF(7, 112, 15, -1),
    SCENE_EXIT_IF(8, 110, 7, 0x86b),
    SCENE_EXIT_IF(8, 113, 7, -1),
    SCENE_EXIT_IF(9, 112, 14, -1),
    SCENE_EXIT_IF(10, 110, 8, 0x86b),
    SCENE_EXIT_IF(10, 113, 8, -1),
    SCENE_EXIT_IF(11, 112, 12, -1),
    SCENE_EXIT_IF(12, 112, 11, -1),
    SCENE_EXIT_IF(13, 112, 16, -1),
    SCENE_EXIT_IF(14, 112, 9, -1),
    SCENE_EXIT_IF(15, 112, 7, -1),
    SCENE_EXIT_IF(16, 112, 13, -1),
    SCENE_EXITS(125),
    SCENE_EXIT(10, 114, 16),
    SCENE_EXIT(11, 114, 10),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020064f8[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02006510[] = {
    {290, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(72), FACING_SOUTH, 2, 0},
    {387, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(200), FACING_SOUTH, 2, 0},
    {387, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(200), FACING_SOUTH, 2, 1},
    {365, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(488), FACING_SOUTH, 2, 1},
    {115, CONDITION_ALWAYS, 3, PIXELS(300), 0, PIXELS(228), FACING_WEST, 0, 0},
    {116, CONDITION_ALWAYS, 3, PIXELS(248), 0, PIXELS(288), FACING_NORTH + FACING_STEP, 0, 0},
    {114, CONDITION_ALWAYS, 3, PIXELS(208), 0, PIXELS(176), FACING_EAST, 0, 0},
    {128, CONDITION_ALWAYS, 3, PIXELS(288), 0, PIXELS(364), FACING_WEST, 0, 0},
    {139, CONDITION_ALWAYS, 1, PIXELS(432), 0, PIXELS(274), FACING_NORTHWEST + FACING_STEP, 0, 0},
    {140, CONDITION_ALWAYS, 1, PIXELS(58), 0, PIXELS(298), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {146, CONDITION_ALWAYS, 1, PIXELS(102), 0, PIXELS(328), FACING_NORTHWEST + FACING_STEP, 1, 0},
    {147, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(336), FACING_NORTH + FACING_STEP, 1, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(246), 0, PIXELS(488), FACING_SOUTHEAST + FACING_STEP, 1, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(282), 0, PIXELS(488), FACING_SOUTH + FACING_STEP, 1, 0},
    {144, CONDITION_ALWAYS, 1, PIXELS(430), 0, PIXELS(442), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {368, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(460), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020066d8[] = {
    {290, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(72), FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(200), FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(200), FACING_SOUTH, 2, 1},
    {365, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(488), FACING_SOUTH, 2, 1},
    {144, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(104), FACING_NORTH + FACING_STEP, 1, 0},
    {141, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(114), FACING_NORTH + FACING_STEP, 1, 0},
    {139, CONDITION_ALWAYS, 1, PIXELS(208), 0, PIXELS(152), FACING_NORTH + FACING_STEP, 1, 0},
    {145, CONDITION_ALWAYS, 1, PIXELS(292), 0, PIXELS(128), FACING_NORTHWEST + FACING_STEP, 1, 0},
    {145, CONDITION_ALWAYS, 1, PIXELS(230), 0, PIXELS(112), FACING_NORTH + FACING_STEP, 1, 0},
    {140, CONDITION_ALWAYS, 1, PIXELS(304), 0, PIXELS(220), FACING_NORTHWEST + FACING_STEP, 1, 0},
    {146, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(278), FACING_NORTH + FACING_STEP, 1, 0},
    {147, CONDITION_ALWAYS, 1, PIXELS(320), 0, PIXELS(158), FACING_NORTHWEST + FACING_STEP, 1, 0},
    {145, CONDITION_ALWAYS, 1, PIXELS(234), 0, PIXELS(132), FACING_NORTH + FACING_STEP, 1, 0},
    {139, CONDITION_ALWAYS, 1, PIXELS(326), 0, PIXELS(88), FACING_NORTH + FACING_STEP, 1, 0},
    {140, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(308), FACING_NORTH + FACING_STEP, 1, 0},
    {141, CONDITION_ALWAYS, 1, PIXELS(278), 0, PIXELS(146), FACING_NORTHWEST + FACING_STEP, 1, 0},
    {143, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(138), FACING_NORTH + FACING_STEP, 1, 0},
    {144, CONDITION_ALWAYS, 1, PIXELS(196), 0, PIXELS(142), FACING_NORTH + FACING_STEP, 1, 0},
    {144, CONDITION_ALWAYS, 1, PIXELS(234), 0, PIXELS(224), FACING_NORTH + FACING_STEP, 1, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(181), FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(194), FACING_SOUTH, 2, 1},
    {386, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(242), FACING_SOUTH, 2, 1},
    {386, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(274), FACING_SOUTH, 2, 1},
    {386, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(338), FACING_SOUTH, 2, 1},
    {386, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(370), FACING_SOUTH, 2, 1},
    {201, CONDITION_ALWAYS, 1, PIXELS(412), 0, PIXELS(60), FACING_NORTHWEST + FACING_STEP, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020069a8[] = {
    {290, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(504), FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(392), FACING_SOUTH, 2, 1},
    {272, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {395, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {395, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {396, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(184), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02006a98[] = {
    {290, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(504), FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(392), FACING_SOUTH, 2, 1},
    {272, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {395, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {395, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {396, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(184), FACING_SOUTH, 2, 1},
    {26, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(392), FACING_NORTH + FACING_STEP, 2, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(218), 0, PIXELS(420), FACING_NORTH + FACING_STEP, 3, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(432), FACING_NORTH + FACING_STEP, 3, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(438), FACING_NORTH + FACING_STEP, 3, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(440), FACING_NORTH + FACING_STEP, 3, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(438), FACING_NORTHWEST + FACING_STEP, 3, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(432), FACING_NORTHWEST + FACING_STEP, 3, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(310), 0, PIXELS(422), FACING_NORTHWEST + FACING_STEP, 3, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(406), FACING_NORTHWEST + FACING_STEP, 3, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, PIXELS(130), 0, PIXELS(364), FACING_SOUTHEAST, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02006cd8[] = {
    {27, CONDITION_ALWAYS, 1, PIXELS(366), 0, PIXELS(586), FACING_SOUTH + FACING_STEP, 2, 0},
    {28, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(586), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(352), 0, PIXELS(634), FACING_NORTH + FACING_STEP, 2, 0},
    {115, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(108), FACING_SOUTH + FACING_STEP, 0, 0},
    {145, CONDITION_ALWAYS, 1, PIXELS(124), 0, PIXELS(120), FACING_NORTH + FACING_STEP, 0, 0},
    {136, CONDITION_ALWAYS, 3, PIXELS(100), 0, PIXELS(132), FACING_NORTH + FACING_STEP, 0, 0},
    {114, CONDITION_ALWAYS, 3, PIXELS(710), 0, PIXELS(134), FACING_EAST, 0, 0},
    {128, CONDITION_ALWAYS, 1, PIXELS(632), 0, PIXELS(108), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {112, CONDITION_ALWAYS, 1, PIXELS(432), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {113, CONDITION_ALWAYS, 1, PIXELS(448), 0, PIXELS(108), FACING_SOUTH + FACING_STEP, 1, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(106), 0, PIXELS(360), FACING_WEST, 1, 0},
    {83, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(352), FACING_EAST, 1, 0},
    {108, CONDITION_ALWAYS, 1, PIXELS(92), 0, PIXELS(600), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {109, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(600), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {110, CONDITION_ALWAYS, 1, PIXELS(648), 0, PIXELS(348), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {134, CONDITION_ALWAYS, 1, PIXELS(84), 0, PIXELS(858), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {135, CONDITION_ALWAYS, 3, PIXELS(148), 0, PIXELS(878), FACING_SOUTH + FACING_STEP, 0, 0},
    {201, CONDITION_ALWAYS, 1, PIXELS(374), 0, PIXELS(344), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(824), FACING_SOUTH, 2, 0},
    {337, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(824), FACING_SOUTH, 2, 1},
    {489, CONDITION_ALWAYS, 7, PIXELS(448), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gData_02006ee8[] = {
    {TRIGGERS_END},
};

struct Trigger gData_02006ef4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_02003948)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_02003948)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_02003948)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_02003948)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Func_02003948)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Func_02003948)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Func_02002ad8)},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(Func_02002af8)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x201, EVENT(Func_020005d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 12, 0x9e9, 0},
    {0xc602, 20, CONDITION_ALWAYS, EVENT(Func_02005994)},
    {0x8602, 21, CONDITION_ALWAYS, EVENT(Func_02005994)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 11, CONDITION_ALWAYS, 0},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAkafubuHasFinally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgEveryoneWasTerribly)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Func_02000228)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgCoveringOurWarriors)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTheGreatGabomba)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgIWantTo)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgNowThatThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgNowThatTheWitch)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgICouldnT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgBecauseKibomboLies)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgOhISmell)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgYouDonTThink)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgGoodOlMeat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgGulpTheSmell)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWeGetTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgLatelyTheMonsters)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Func_0200024c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgNoOneInKibombo)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTheOtherWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgWithAllOur)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, EVENT(Func_02000270)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgIWonderIfI)},
    {TRIGGERS_END},
};

struct Trigger gData_020070d4[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Func_020038ec)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Func_020038ec)},
    {0xc602, 3, 0x209, EVENT(Func_02003948)},
    {0xc602, 4, 0x209, EVENT(Func_02003948)},
    {0xc602, 5, 0x209, EVENT(Func_02003948)},
    {0xc602, 6, 0x209, EVENT(Func_02003948)},
    {0xc602, 7, 0x209, EVENT(Func_02003948)},
    {0xc602, 8, 0x209, EVENT(Func_02003948)},
    {TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(Func_020038ec)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Func_020038ec)},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Func_02002ad8)},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(Func_02002af8)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x201, EVENT(Func_020005d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 12, 0x9e9, 0},
    {0xc602, 20, CONDITION_ALWAYS, EVENT(Func_02005994)},
    {TRIGGER_TALK, 20, 0x8fd, MSGNO(MsgAkafubuSCeremony)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTheyToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x8fd, MSGNO(MsgItSStrange)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgAsLongAsThis)},
    {TRIGGER_TALK, 14, 0x8fd, MSGNO(MsgDuringTheLast)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAkafubuHasBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x8fd, MSGNO(MsgTheGreatGabombaIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgWhatSTaking)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgIMAfraid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgWhenHeFails)},
    {TRIGGER_TALK, 21, 0x8fd, MSGNO(MsgIHopeThe)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWhewThankGoodness)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x8fd, MSGNO(MsgIfTheGreat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgAmIGoing)},
    {TRIGGER_TALK, 17, 0x8fd, MSGNO(MsgIsMakingThe)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgOnceTheCeremony)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x8fd, MSGNO(MsgHeSFailed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKibomboWasSo)},
    {TRIGGER_TALK, 22, 0x8fd, MSGNO(MsgWhenTheGabomba)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgLastTimeAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, 0x8fd, MSGNO(MsgIHopeAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgAkafubuSAlways)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgAkafubuWasA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgAkafubuHasNot)},
    {TRIGGER_TALK, 15, 0x8fd, MSGNO(MsgAkafubuLooksReally)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIThinkAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x8fd, MSGNO(MsgHeTravelsTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgAkafubuIsObsessed)},
    {TRIGGER_TALK, 25, 0x8fd, MSGNO(MsgIfTheGreatGabomba)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgIBelieveIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, 0x8fd, MSGNO(MsgIfTheGreatGabombaActually)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgAkafubuAndOeia)},
    {TRIGGER_TALK, 18, 0x8fd, MSGNO(MsgAkafubuHasTried)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgWhatDonT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x8fd, MSGNO(MsgAsAChildAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgIMSoFar)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgHeyNoPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgThereAreSo)},
    {TRIGGER_TALK, 16, 0x8fd, MSGNO(MsgAkafubuIsAttempting)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTonightSeemsTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x8fd, MSGNO(MsgOeiaSSudden)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgWillTheGreat)},
    {TRIGGER_TALK, 23, 0x8fd, MSGNO(MsgHeWhoIs)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgOeiaSDeath)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, 0x8fd, MSGNO(MsgIfOnlyOeia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgISuspectAkafubu)},
    {TRIGGER_TALK, 19, 0x8fd, MSGNO(MsgAkafubuBlamesHis)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTheySayThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x8fd, MSGNO(MsgTheyAttackedSome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTheyReSaying)},
    {TRIGGER_TALK, 24, 0x8fd, MSGNO(MsgIfTheGreatGabombaS)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAkafubuSeesThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, 0x8fd, MSGNO(MsgIFeelSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgIReallyWonder)},
    {TRIGGER_TALK, 36, CONDITION_ALWAYS, EVENT(Func_0200042c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 36, CONDITION_ALWAYS, MSGNO(MsgIWonderIfA)},
    {TRIGGERS_END},
};

struct Trigger gData_02007434[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Func_020038ec)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Func_020038ec)},
    {0x4602, 3, 0x186a, EVENT(Func_02003bf0)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Func_02003930)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Func_020038ec)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x201, EVENT(Func_020005d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x9ea, EVENT(Func_02000734)},
    {9, 0, 0x9ea, EVENT(Func_02000734)},
    {0x50008805, 10, 0x86a, EVENT(Func_02000618)},
    {0x50008805, 11, 0x204, EVENT(Func_0200065c)},
    {0x50008805, 12, 0x205, EVENT(Func_020006a0)},
    {0x50008805, 13, 0x206, EVENT(Func_020006e4)},
    {0x50008805, 14, 0x207, EVENT(Func_02000728)},
    {TRIGGERS_END},
};

struct Trigger gData_020074dc[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Func_020038ec)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Func_020038ec)},
    {0x4602, 3, 0x186a, EVENT(Func_02003bf0)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Func_02003930)},
    {TRIGGER_STEP, 5, 0x8f3, EVENT(Func_020038ec)},
    {TRIGGER_STEP, 5, 0x92f, EVENT(Func_02001218)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Func_020038ec)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x201, EVENT(Func_020005d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x9ea, EVENT(Func_02000734)},
    {9, 0, 0x9ea, EVENT(Func_02000734)},
    {0x50008805, 10, 0x86a, EVENT(Func_02000618)},
    {0x50008805, 11, 0x204, EVENT(Func_0200065c)},
    {0x50008805, 12, 0x205, EVENT(Func_020006a0)},
    {0x50008805, 13, 0x206, EVENT(Func_020006e4)},
    {0x50008805, 14, 0x207, EVENT(Func_02000728)},
    {TRIGGER_STEP, 20, 0x8f3, EVENT(Func_020007c8)},
    {TRIGGER_STEP, 22, 0x92e, EVENT(Func_02000c90)},
    {TRIGGER_TALK, 7, CONDITION_ALWAYS, MSGNO(MsgTheOrbIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 7, CONDITION_ALWAYS, MSGNO(MsgIAmCertain)},
    {TRIGGER_STEP, 21, 0x8f5, EVENT(Func_02002050)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAhhSomethingIs)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgYouLookHuman)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAkafubuPossessesThe)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWeMustStand)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgMenOrGods)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgItSeemsThese)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgThisIsnT)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgItIsProper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTheyLookLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgIfNoPerson)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgISawAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgIHopeAkafubuCompletes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgWhatIfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgLookAtThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgSureThisIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgKoffKoffAll)},
    {TRIGGERS_END},
};

struct Trigger gData_02007698[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x202, 7, CONDITION_ALWAYS, EVENT(Func_02003ac8)},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x202, 9, CONDITION_ALWAYS, EVENT(Func_02003b20)},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x202, 13, CONDITION_ALWAYS, EVENT(Func_02003b88)},
    {TRIGGER_TALK, 8, 0x8ee, MSGNO(MsgIMGoing)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgIJustCan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgDangWhyDid)},
    {TRIGGER_TALK, 9, 0x8ee, MSGNO(MsgIThinkThat)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMySonHasBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgAfterHeBecame)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgIHaveHeard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgIKnowI)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Func_020002c4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgCrossingTheRiver)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgYouCanSee)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgIfIM)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgIfYouAreLooking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSureOurLegends)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgEachYearWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgIWonderWhatOeia)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Func_020004dc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT(Func_02000500)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, EVENT(Func_020004dc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, EVENT(Func_02000500)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAsSoonAs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgAkafubuGetsSo)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgManyInGondowan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgItWonT)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgWeThinkIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgIAmAMighty)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Func_02000318)},
    {TRIGGER_EXAMINE, 20, CONDITION_ALWAYS, EVENT(Func_02000318)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgOurWarriorsAppear)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(Func_02000350)},
    {TRIGGER_EXAMINE, 21, CONDITION_ALWAYS, EVENT(Func_02000350)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgBecauseOurPaint)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Func_02000388)},
    {TRIGGER_EXAMINE, 22, CONDITION_ALWAYS, EVENT(Func_02000388)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgIfIHad)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, EVENT(Func_020003c0)},
    {TRIGGER_EXAMINE, 23, CONDITION_ALWAYS, EVENT(Func_020003c0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgThatBedMakes)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgICanTRemember)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgILoveKibombo)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, EVENT(Func_020003f8)},
    {TRIGGER_EXAMINE, 24, CONDITION_ALWAYS, EVENT(Func_020003f8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgICanTBelieve)},
    {TRIGGERS_END},
};
#undef PALETTE_WORD
