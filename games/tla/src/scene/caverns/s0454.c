/// @file
/// Scene code of overlay 0x6a9, in ROM order, from s0454's offset up to the next object the overlay links.

#include "../../../../games/tla/src/scene/caverns/caverns.h"
#include "event_inline.h"
#include "actor.h"
#include "caverns.h"
#include "field_sprite.h"
#include "types.h"
#include "ram_buffer.h"
#include "field_effect.h"
#include "fixed_math.h"
#include "macros.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020010cc(s32 x, s32 z, s32 layer, s32 height);
s32 Func_020003b4(u32 layer, s32 x, s32 y);
s32 Func_020003f0(u32 layer, s32 x, s32 y, s32 value);

void Func_02000454(const u8 *data) {
  const u16 *ids = (const u16 *)data;
  struct Actor *leader = (struct Actor *)((struct FieldActor *)Actor_GetById(
      gMainRoutineWork.currentOwner));
  struct Actor *object;
  s32 height;
  s32 cell;

  if (*ids == 0xffff)
    goto done;
loop:
  object = (struct Actor *)((struct FieldActor *)Actor_GetById(*ids));
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->bounce = 0;
  Actor_SetSingleChildFlags((struct FieldActor *)object, 0);
  height = (Map_GetTerrainHeight(0, object->x, object->z) >> 19) + 2;
  Func_020010cc(object->x, object->z, 0, height);
  Func_020010cc(object->x, object->z, 2, height);
  cell = Func_020003b4(0, object->x >> 20, object->z >> 20);
  object->unk4c = cell;
  Func_020003f0(0, object->x >> 20, object->z >> 20, cell | 0x80);
  ids++;
  if (*ids != 0xffff)
    goto loop;
done:
  if (GameFlag_Test(0x109)) {
    wait(1);
    leader->y = leader->terrainHeight =
        Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
    leader->velocityY = 0;
    leader->targetY = 0x80000000;
    Camera_FollowActor(gMainRoutineWork.currentOwner, 0);
  }
}

static __inline__ s32 GetHeight(s32 x, s32 z) {
  return Map_GetTerrainHeight(2, x, z);
}

static __inline__ void SetHeight(s32 x, s32 z, s32 height) {
  Func_020010cc(x, z, 2, height);
}

static __inline__ s32 Half(u8 *base, s32 offset) {
  return HALF_AT(base, offset);
}

#define SET_CELL_HEIGHT(x, z)                                                  \
  height = (GetHeight(x, z) >> 19) + offset;                                   \
  SetHeight(x, z, height)

void Func_0200053c(s32 offset) {
  u8 *work = *(u8 **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  s32 height;

  offset <<= 3;
  if (HALF_AT(work, 14) > 31 || Event_GetFlag(0x213)) {
    SET_CELL_HEIGHT(142 << 18, 134 << 18);
    SET_CELL_HEIGHT(146 << 18, 138 << 18);
    if (offset == 8)
      Event_SetFlag(0x213);
    else
      Event_ClearFlag(0x213);
  } else if (Half(work, 18) == 536 || Event_GetFlag(0x213)) {
    SET_CELL_HEIGHT(134 << 18, 138 << 18);
    SET_CELL_HEIGHT(138 << 18, 138 << 18);
    SET_CELL_HEIGHT(142 << 18, 138 << 18);
    if (offset == 6)
      Event_SetFlag(0x213);
    else
      Event_ClearFlag(0x213);
  } else {
    SET_CELL_HEIGHT(134 << 18, 134 << 18);
    SET_CELL_HEIGHT(138 << 18, 134 << 18);
    SET_CELL_HEIGHT(142 << 18, 134 << 18);
  }
}

struct CurrentUnit {
  u8 unk00[0x14];
  struct Actor *object;
  u8 unk18[4];
  s16 unk1c;
};

void Func_020006a8(void) {
  s32 position[3];
  u8 *work = Ram_HeapSlots->eventWork;
  struct CurrentUnit *current = (struct CurrentUnit *)Ram_HeapSlots->effectWork;
  const u16 *ids = (const u16 *)gData_02001220;
  struct Actor *object = NULL;
  struct Actor *leader;
  s32 height;
  s32 angle;

  WORD_AT(work, 0xcbc) = 0;
  if (current != NULL && HALF_AT(work, 358) != 0) {
    object = (struct Actor *)(WORD_AT(work, 0xcbc) = (s32)current->object);
    if (current->unk1c != 147)
      Func_0200053c(1);
  } else {
    leader = (struct Actor *)((struct FieldActor *)Actor_GetById(
        gMainRoutineWork.currentOwner));
    angle = (leader->angle + 0x2000) & 0xc000;
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    Vector_AddPolarOffsetFar(0x100000, angle, position);
    if (*ids == 0xffff)
      goto search_done;
  search:
    object = (struct Actor *)((struct FieldActor *)Actor_GetById(*ids));
    if (position[0] >> 20 == object->x >> 20 &&
        position[1] / 0x10000 == object->y / 0x10000 &&
        position[2] >> 20 == object->z >> 20)
      goto search_done;
    ids++;
    object = NULL;
    if (*ids != 0xffff)
      goto search;
  search_done:
    if (object == NULL)
      return;
    WORD_AT(work, 0xcbc) = (s32)object;
    Func_0200053c(1);
  }
  height = (Map_GetTerrainHeight(0, object->x, object->z) >> 19) - 2;
  Func_020010cc(object->x, object->z, 0, height);
  Func_020010cc(object->x, object->z, 2, height);
  Func_020003f0(0, object->x >> 20, object->z >> 20, object->unk4c);
}

void Func_020007ec(struct Actor *object) {
  object->x += object->bounce;
  object->y += object->gravity;
  object->z += object->unk4c;
  object->bounce -= object->bounce / 12;
  object->unk4c -= object->unk4c / 10;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct FieldSprite *)object->animation)->rotation += object->action;
}

void Func_02000848(struct FieldActor *object) {
  struct FieldActor *leader =
      ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner));
  s32 leaderZ = leader->z;
  s32 z = object->z;
  u8 *flags = &leader->priorityFlags;

  if (leaderZ > z) {
    /* FAKEMATCH(temporary): the mask in a local set first, so that it takes
     * r3 and the flags byte r2; as *flags |= 4, 13 of 22 instructions differ
     * in tla-en. */
    s32 value = 4;

    value |= *flags;
    *flags = value;
  } else {
    *flags &= ~6;
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

struct FieldActor;

void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_0200108c(struct Actor *object);
struct Actor *Func_0200115c(s32 *position, s32 mode);
s32 Math_Cosine(s32 angle);
struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void OverlayObject_WaitUntilIdle(struct Actor *object);
u32 Random16Far(void);

void Func_0200087c(void) {
  s32 velocity[3];
  struct EffectOptions options;
  u8 *work;
  u8 *current;
  struct Actor *object;
  struct Actor *other;
  s32 kind;
  s32 height;
  s32 value;
  s32 angle;
  u32 i;
  struct Actor *spawned;
  u16 *blendControl;
  s32 sum;

  work = Ram_HeapSlots->eventWork;
  current = Ram_HeapSlots->effectWork;
  object = FIELD_AT(work, struct Actor *, 3260);
  if (current != NULL && FIELD_AT(work, s16, 358) != 0) {
    if (FIELD_AT(current, s16, 28) != 147)
      Func_0200053c(-1);
  } else {
    Func_0200053c(-1);
  }
  if (Map_GetTerrainHeight(0, object->x, object->z) != object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    object->bounce = 0;
    wait(1);
    kind = GetMapCellCollision(2, object->x, object->z);
    if (kind != 0) {
      object->priorityFlags |= 2;
      *(u16 *)0x4000052 = 0x1000;
      blendControl = (u16 *)0x4000050;
      *blendControl = 0x3f10;
      sum = kind + 2;
      spawned = OverlayObject_PrepareObject(object->x, 0,
                                            object->z + (sum << 20), 312);
      Sound_Play(204);
      OverlayObject_WaitUntilIdle(object);
      OverlayObject_WaitUntilIdle(object);
      object->x = object->y = object->z = 0;
      Func_0200108c(spawned);
      wait(1);
      *blendControl = 0;
    } else {
      OverlayObject_WaitUntilIdle(object);
      OverlayObject_WaitUntilIdle(object);
      Sound_Play(188);
      options.palette = 7;
      options.priority = 2;
      options.update = (void (*)(union FieldObject *))Func_020007ec;
      options.startScaleX = 0xcccc;
      options.startScaleY = 0xcccc;
      i = 0;
      // A goto loop carries no loop notes: as a do loop, GCSE moves the
      // array's address out of the loop.
    loop:
      {
        angle = i << 12;
        velocity[0] = Math_Cosine(angle) * 2;
        velocity[1] = 0;
        velocity[2] = Math_Sine(angle) * 2;
        velocity[0] += velocity[0] >> 1;
        velocity[0] += ((u32)(Random16Far() * 3 << 14) >> 16) - 0x6000;
        velocity[2] += ((u32)(Random16Far() * 3 << 13) >> 16) - 0x3000;
        /* FAKEMATCH(dead): sum, never read again, assigned the flags inside
         * the call's arguments; as 0x10b0000 passed directly, 6 of 255
         * instructions differ in tla-en. */
        Func_0200015c(object->x, object->y, object->z, velocity[0], velocity[1],
                      velocity[2], sum = 0x10b0000, &options);
        i++;
      }
      if (i <= 16)
        goto loop;
    }
    object->motionFlags = 0;
    object->terrainId = 0;
    // The velocity array is reused for the position under the object.
    {
      velocity[0] = object->x;
      velocity[1] = object->y - (16 << 16);
      velocity[2] = object->z;
      other = Func_0200115c(velocity, 1);
      if (other != NULL) {
        object->update = (void (*)(void))Func_02000848;
        FIELD_AT(object, u8, 98) = 1;
        FIELD_AT(other, u8, 98) = 1;
        GameFlag_SetBit(0x212);
      }
    }
  }
  height = (Map_GetTerrainHeight(0, object->x, object->z) >> 19) + 2;
  Func_020010cc(object->x, object->z, 0, height);
  Func_020010cc(object->x, object->z, 2, height);
  value = Func_020003b4(0, object->x >> 20, object->z >> 20);
  object->unk4c = value;
  Func_020003f0(0, object->x >> 20, object->z >> 20, value | 128);
}

const u8 *Func_02000ad0(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6a9Room1)
    return gData_0200149c;
  if (scene == (s32)&SceneId_Ov6a9Room3)
    return gData_020014d8;
  if (scene == (s32)&SceneId_Ov6a9Room4)
    return gData_020014fc;
  return gData_02001430;
}

s32 Func_02000b20(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6a9Room1)
    Func_02000ba8();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6a9Room4)
    Func_02000f98();
  return 0;
}

s32 Func_02000b64(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6a9Room2)
    Func_02000d10();
  else if (scene == (s32)&SceneId_Ov6a9Room3)
    Func_02000ea0();
  else if (scene == (s32)&SceneId_Ov6a9Room4)
    Func_02000f50();
  return 0;
}

void Func_02000ba8(void) {
  struct FieldActor *object;
  s32 arg;

  ((struct FieldActor *)Actor_GetById(4))->priorityFlags |= 0x20;
  Func_0200114c();
  Func_02001154(0, 0x200, 8, 9);
  object = ((struct FieldActor *)Actor_GetById(10));
  object->motionFlags = 0;

  arg = 10;
  Map_CopyCellAttributeRect(16, arg, 1, 1, object->x >> 20, arg);
}

void Func_02000bf8(s32 unused, s32 id) {
  struct FieldActor *object = ((struct FieldActor *)Actor_GetById(id));

  object->motionFlags = 0;
  Map_CopyCellAttributeRect(13, 12, 2, 1, 13, 10);
  Map_CopyCellAttributeRect(16, 10, 1, 1, object->x >> 20, 10);
}

s32 Func_02000c38(struct FieldActor *object, s16 *position) {
  u8 *flags = &object->motionFlags;
  s32 saved = *flags;
  s32 blocked = Func_020010b4(object, position);

  if (blocked == 0) {
    Func_020010dc();
    Event_ResolvePendingActions(0);
    Func_02001074(object, 6);
    wait(6);
    Sound_Play(152);
    Func_02001074(object, 7);
    object->speed = 0x30000;
    object->acceleration = 0x20000;
    object->velocityY = 0x40000;
    *flags &= 0x7e;
    Actor_SetSingleChildFlags(object, 0);
    {
      struct MainRoutineWork *party = &gMainRoutineWork;
      s32 x = position[1];
      s32 z = position[5];
      s32 owner = party->currentOwner;

      Actor_SetPositionAndCommit(owner, x, z);
    }
    Func_02001074(object, 6);
    Actor_SetSingleChildFlags(object, 1);
    *flags = saved;
    Func_020010e4();
    return 1;
  }
  return 0;
}

void Func_02000cdc(void) {
  struct FieldActor *leader =
      ((struct FieldActor *)Actor_GetById(gMainRoutineWork.currentOwner));
  s32 position[3];
  s32 x;

  x = leader->x;
  x += 0x200000;
  SET_VECTOR(position, x, leader->y, leader->z);
  Func_02000c38(leader, (s16 *)position);
}

void Func_02000d28(void);

void Func_02000d10(void) {
  s32 result0;

  result0 = Event_GetFlag(0x9af);
  if (result0 == 0)
    goto L_02008d24;
  Func_02000d28();
L_02008d24:
  return;
}

void Func_02000d28(void) {
  Map_CopyCellPictureIndicesRect(35, 12, 18, 12, 3, 3);
  MapGrid_CopyCellAttributes(35, 12, 3, 3, 18, 12);
  Map_CopyCellPictureIndicesRect(84, 13, 83, 13, 1, 1);
  return;
}

void Func_02000d68(void) {
  s32 value = HALF_AT(Ram_HeapSlots->eventWork, 340);

  if (value == 2)
    Actor_SetPosition(8, PIXELS(144), PIXELS(224));
  else
    Actor_SetPosition(8, PIXELS(224), PIXELS(224));
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  BYTE_AT(((struct FieldActor *)Actor_GetById(4)), 90) &= ~1;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(((struct FieldActor *)Actor_GetById(4)), 0);
  Actor_OffsetPositionAndResetMotion(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 13);
  Sound_Play(123);
  Event_Wait(14);
  Import_EventSetExitRequest(value);
}

void Func_02000e20(void) {
  Actor_WalkByAndWait(4, 0, -14);
  Actor_CommitPositionAndActivate(4, 16, 0);
  Event_Wait(10);
  Func_02000d68();
  return;
}

void Func_02000e44(void) {
  Actor_WalkByAndWait(4, 0, -14);
  Actor_CommitPositionAndActivate(4, -16, 0);
  Event_Wait(10);
  Func_02000d68();
  return;
}

void Func_02000e6c(void) {
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Func_02000d68();
  return;
}

void Func_02000e88(s32 arg0) {
  if (arg0 != 1)
    goto L_02008e9c;
  Func_02000d28();
  GameFlag_SetBit(0x9af);
L_02008e9c:
  return;
}

void Func_02000ea0(void) {
  s32 result0;

  result0 = Event_GetFlag(0x917);
  if (result0 == 0)
    goto L_02008eee;
  MapGrid_CopyCellAttributes(31, 43, 9, 12, 48, 47);
  MapGrid_CopyCellsTo(30, 110, 48, 110, 9, 13);
  MapGrid_CopyCellAttributes(52, 110, 1, 1, 52, 111);
L_02008eee:
  return;
}

void Func_02000ef4(void) {
  Func_020010dc();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(31, 43, 9, 11, 48, 47);
  MapGrid_CopyCellsTo(30, 110, 48, 110, 9, 13);
  MapGrid_CopyCellAttributes(52, 110, 1, 1, 52, 111);
  GameFlag_SetBit(0x917);
  Func_020010e4();
  return;
}

void Func_02000f50(void) {
  if ((u16)(gMainRoutineWork.entrance - 5) <= 1) {
    MapGrid_CopyCellAttributes(28, 0, 1, 1, 28, 5);
    Map_CopyCellAttributeRect(36, 0, 1, 1, 36, 5);
  }
}

void Func_02000f98(void) {
  Func_02000454(gData_02001220);
  ((struct FieldActor *)Actor_GetById(8))->priorityFlags |= 0x20;
  ((struct FieldActor *)Actor_GetById(9))->priorityFlags |= 0x20;
  ((struct FieldActor *)Actor_GetById(10))->collisionFlags |= 8;
  ((struct FieldActor *)Actor_GetById(11))->collisionFlags |= 8;
}

void Func_02000fe4(void) {
  Func_020010dc();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  Event_Wait(10);
  MapGrid_CopyCellsTo(7, 94, 6, 94, 1, 2);
  Event_Wait(16);
  Sound_Play(123);
  Import_EventSetExitRequest(8);
  Func_020010e4();
}
