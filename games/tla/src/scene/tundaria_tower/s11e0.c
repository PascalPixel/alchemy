// ---- games/tla/src/scene/tundaria_tower/s11e0.c ----
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "frame.h"
#include "field_effect.h"
#include "event_inline.h"

void Func_02003048(void);
void Func_02003050(void);
void Func_02002fa8(void);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion(struct FieldEffect *effect);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

static inline void TundariaTower_CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_020011e0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  s32 angle;
  u32 i;

  Func_02003048();
  Event_ResolvePendingActions(0);
  TundariaTower_CamBounds(-1, -1, -1, 0);
  Func_02002fa8();
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
  options.palette = 7;
  options.update = (void *)Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  i = 0;
  do {
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
    velocity[2] += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
    Func_020000b8(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                  velocity[2], 0x01090001, &options);
    i++;
  } while (i <= 16);
  Sound_Play(188);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  Map_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_02003050();
}

#include "tundaria_tower.h"
#include "iwram_call.h"
#include "call.h"
#include "scene_ids.h"
#include "scene_tables.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_0200137c(void) {
  if ((Actor_GetById(gMainRoutineWork.currentOwner)->y >> 19) != 0)
    MapCell_SetAttributes(0, 14, 17, 0);
  else
    MapCell_SetAttributes(0, 14, 17, 0x3200);
}

struct TundariaTowerEventWork {
  u8 unk000[428];
  s32 screenEffect;
};

extern const u8 gData_02003340[];
extern const u8 gData_0200335a[];
union FieldObject;
extern union FieldObject *Data_02003ccc;
void Func_020010d4(void);
void Func_02000548(void);
void Func_0200058c(void);
void Func_02000404(const u8 *data);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Animation_SetStateFlags(struct Actor *object, u32 value);
void Func_02003168(const s16 *data);

s32 Func_020013b8(void) {
  struct Actor *object;
  s32 scene;
  s32 flag;

  ((struct TundariaTowerEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x204;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov68fRoom1) {
  } else if (scene == (s32)&SceneId_Ov68fRoom2) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      if (GameFlag_Test(0x942))
        Func_02000548();
      if (GameFlag_Test(0x943))
        Func_0200058c();
      break;
    case 11:
      flag = GameFlag_Test(0x945);
      if (flag == 0) {
        object = Actor_GetById(8);
        Actor_SetSingleChildFlags(Actor_GetById(8), 0);
        object->collisionFlags = 0;
        object->motionFlags = 0;
        object->terrainHeight = PIXELS(8);
        object->y = PIXELS(8);
        Actor_SetItemIcon(object, 0x1b9);
        if (GameFlag_Test(0x944)) {
          MapGrid_CopyCellsTo(31, 85, 20, 85, 3, 4);
          MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
          Actor_SetPosition(68, PIXELS(344), PIXELS(372));
          Actor_GetById(68)->update = (void (*)(void))Func_02000840;
          Actor_SetPosition(8, 0, 0);
        }
      } else if (GameFlag_Test(0x944)) {
        MapGrid_CopyCellsTo(31, 85, 20, 85, 3, 4);
        MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
        Actor_SetPosition(8, 0, 0);
      }
      break;
    }
  } else if (scene == (s32)&SceneId_Ov68fRoom4) {
    switch (gMainRoutineWork.entrance) {
    case 6:
    case 7:
      if (!GameFlag_Test(100)) {
        if (!GameFlag_Test(0x200)) {
          Actor_SetPosition(8, PIXELS(344), PIXELS(536));
          MapCell_SetAttributes(0, 21, 33, 0x3300);
          MapCell_SetAttributes(0, 22, 33, 0xff00);
        } else if (!GameFlag_Test(0x201)) {
          MapCell_SetAttributes(0, 26, 33, 0xff00);
        }
        Actor_WatchAndSetScript(8, gMainRoutineWork.currentOwner);
      }
      if (!GameFlag_Test(0x109) && gMainRoutineWork.entrance == 7)
        Func_020011e0();
      break;
    }
  } else if (scene == (s32)&SceneId_Ov68fRoom5) {
    Func_02000404(gData_02003340);
  } else if (scene == (s32)&SceneId_Ov68fRoom6) {
    switch (gMainRoutineWork.entrance) {
    case 3:
      object = FieldActor_Create(0x11e, PIXELS(472), 0, PIXELS(200));
      if (object != NULL) {
        Call2((void (*)())Scene_SetActorSpritePriority, (s32)object, 1);
        object->motionFlags = 0;
        Actor_SetSingleChildFlags(object, 0);
        Actor_SetPartAttribute(object, 14);
        Animation_SetStateFlags(object, 1);
      }
      break;
    case 1:
    case 2:
      Func_02000404(gData_0200335a);
      if (GameFlag_Test(0x947)) {
        Actor_SetPosition(13, PIXELS(216), PIXELS(136));
        if (GameFlag_Test(0x946))
          Actor_SetPosition(12, PIXELS(200), PIXELS(136));
      }
      Func_02003168(gData_02003264);
      Actor_SetSpritePriority(12, 2);
      break;
    case 4:
    case 5:
      if (GameFlag_Test(0x94b)) {
        MapGrid_CopyCellsTo(18, 97, 6, 97, 3, 4);
        MapGrid_CopyCellsTo(82, 34, 70, 34, 3, 2);
        MapGrid_CopyCellAttributes(7, 36, 1, 1, 7, 35);
      }
      {
        union FieldObject **slot = &Data_02003ccc;
        struct Actor *effect = ((struct Actor *)Import_ActorCreate(4, 0, 0, 0));

        effect->unk4c = 144 << 18;
        effect->update = Func_020010d4;
        *slot = (union FieldObject *)effect;
        effect->motionFlags = 0;
        Scene_SetActorSpritePriority(effect, 3);
      }
      break;
    case 6:
    case 7: {
      union FieldObject **slot = &Data_02003ccc;
      struct Actor *effect = ((struct Actor *)Import_ActorCreate(4, 0, 0, 0));

      effect->unk4c = 156 << 18;
      effect->update = Func_020010d4;
      *slot = (union FieldObject *)effect;
      effect->motionFlags = 0;
      Scene_SetActorSpritePriority(effect, 3);
    } break;
    case 10:
      if (!GameFlag_Test(0x109))
        Func_020011e0();
      break;
    }
  } else if (scene == (s32)&SceneId_Ov68fRoom7) {
    GameFlag_SetBit(0x144);
    if (GameFlag_Test(0x948)) {
      MapGrid_CopyCellsTo(13, 26, 13, 16, 3, 3);
      MapGrid_CopyCellAttributes(13, 26, 3, 3, 13, 16);
    } else {
      Task_Add(Func_0200137c, 0x480);
    }
    Func_02003168(gData_0200326e);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/* VIEW(s11e0): variable gInput as extern u32 gInput; input.h has extern struct InputState gInput; tried as the 3 declarations' extern u32 gInput, it does not compile in tla-ja: games/tla/include/system/input.h:37: conflicting types for `gInput'. */
extern u32 gInput__s11e0 __asm__("gInput");
extern s16 gData_02003272[];
void Func_02002f80(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

void Func_020017dc(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->timer != 0) {
    object->timer--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (GameFlag_Test(0x106))
    angle = -1;
  else
    angle = gData_02003272[(gInput__s11e0 >> 4) & 15];
  if (angle == -1) {
    Func_02002f80(object, 9);
    return;
  }
  turn = (s16)(angle - object->facing);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->facing += turn;
  Func_02002f80(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}

// ---- games/tla/src/scene/tundaria_tower/events_e.c ----
#include "input.h"

void Func_02001868(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (work->actionCounter != 0) {
    Call1((void (*)())GameFlag_SetBit, 0x106);
    Func_02003100();
    Func_02003140();
    GameFlag_ClearBit(0x106);
  }
}

extern s16 gData_020032b2[];
/* VIEW(events_e): routine Func_020017dc as void Func_020017dc(void); the definition has void Func_020017dc(struct Actor *object); tried as the definition's void Func_020017dc(struct Actor *actor), it does not compile in tla-ja: games/tla/src/scene/tundaria_tower/s11e0.c:374: `Func_020017dc__events_e' undeclared (first use in this function). */
extern void Func_020017dc__events_e(void) __asm__("Func_020017dc");
s32 GetMapCellCollision(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Actor_SetMoveTarget(struct Actor *actor, s32 x, s32 y, s32 z);
void Func_02002f80(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Script_WaitForEventTimeout(struct Actor *actor);
s32 Func_02003028(s32 *a, s32 aRadius, s32 *b, s32 bRadius);

void Func_0200189c(void) {
  struct Actor *object;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 x;
  s32 z;
  u8 *layer;
  s32 radius;
  s32 position[3];
  s32 dx, dz;
  s32 cell;
  s16 angle;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *p;
  s32 *q;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
retry:
  Func_02001868();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  p[0] = x = (object->x & 0xfff00000) + 0x80000;
  p[1] = object->y;
  p[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = GetMapCellCollision(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_020032b2[(gInput.held >> 4) & 15];
    if ((u16)angle == 0xffff)
      return;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse's hoist of the
   * position's address; as no asm, 2 of 304 instructions differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  cell = GetMapCellCollision(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->facing = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->timer = 0;
  Actor_SetMoveTarget(object, x, object->y, z);
  Func_02002f80(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  Script_WaitForEventTimeout(object);
  object->update = Func_020017dc__events_e;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    if (Value4((s32 (*)())Func_02003028, (s32)&entry->x, radius, (s32)position,
               entry->radius - 2) >= 0)
      goto blocked;
  }
  j++;
  flags += sizeof(*entry);
  entry++;
  if (j <= 63)
    goto overlap;
  targetX = q[0];
  targetZ = q[2];
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  Actor_SetMoveTarget(object, q[0], q[1], q[2]);
  Script_WaitForEventTimeout(object);
  if (cell != startCell)
    goto moved;
walk:
  /* FAKEMATCH(barrier): as the one above; as no asm, 126 of 304 instructions
   * differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
  q = position;
  cell = GetMapCellCollision(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  Script_WaitForEventTimeout(object);
  wait(2);
  goto retry;
moved:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02002f80(object, 1);
}
