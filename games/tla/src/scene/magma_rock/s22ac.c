#include "../shared/routines.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "field/sprite.h"
#include "call.h"
#include "scene_ids.h"
#include "field/event_work.h"
#include "event_work.h"
#include "field.h"
#include "module/field/object.h"
#include "module/scene/magma_rock.h"
#include "module/scene/mars_lighthouse_star_room.h"

struct MagmaRockEventWork {
  u8 unk000[0x1ac];
  s32 field1ac;
};

s32 Math_Cosine(s32 angle);
void FieldEffect_AdvanceMotion();
void Map_CenterOnFollowedPoint(void);
void Func_02003af4(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectOptions *options);

void Func_020022ac(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct MagmaRockEffectOptions options;
  u32 i;
  s32 angle;
  s32 velocity[3];

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y += 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct MagmaRockEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.scaleX = 0xcccc;
  options.scaleY = 0xcccc;
  i = 0;
loop:
  angle = i << 12;
  velocity[0] = Math_Cosine(angle);
  velocity[1] = 0;
  velocity[2] = Math_Sin(angle);
  velocity[0] += velocity[0] >> 1;
  velocity[0] += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
  velocity[2] += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
  Func_0200015c(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                velocity[2], 0x1090001, &options);
  i++;
  if (i <= 16)
    goto loop;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  ((struct MagmaRockEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x204;
  Func_02003af4();
}

DEFINE_FUNC_0200244C(Func_0200244c)

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

void Func_020024c8(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);

  if (Event_GetFlag(0x109)) {
    s32 height;

    System_Wait(1);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    object->velocityY = 0;
    object->terrainHeight = height;
    object->y = height;
    object->targetY = PIXELS(-32768);
    EventCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
  }
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

struct LayeredSprite {
  u8 unk00[4];
  u32 attributes;
  u8 unk08[4];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk11 : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  struct AnimationSpritePart shadow;
  struct AnimationEntry *entries[4];
};

void Func_020015e4(void);
void Func_02001528(void);

static inline void AttachPart(struct Actor *part, struct Actor *anchor,
                              u8 order) {
  BYTE_AT(part, 98) = order;
  part->watchedObject = anchor;
  part->update = Func_02001528;
  Actor_SetSpriteColor(part, 3);
}

static inline void BuildParts(struct Actor *anchor) {
  struct Actor *parts[4];
  struct Actor *part;
  struct LayeredSprite *first;
  struct LayeredSprite *sprite;
  u32 i;

  parts[0] = ((struct Actor *)Import_ActorCreate(
      0x15f, anchor->x + 0x10000, anchor->y + 0x200000, anchor->z));
  i = 1;
next:
  first = parts[0]->sprite;
  part = ((struct Actor *)Import_ActorCreate(0x15f, 0, 0, 0));
  parts[i] = part;
  sprite = part->sprite;
  sprite->slot = first->slot;
  sprite->retainCacheEntry = 1;
  part->x = parts[0]->x;
  part->y = parts[0]->y;
  part->z = parts[0]->z;
  part->motionFlags = 0;
  Actor_SetSingleChildFlags(part, 0);
  Func_02003a54(parts[i], 3);
  AttachPart(parts[i], anchor, i - 1);
  if (++i != 4)
    goto next;
  part = parts[0];
  part->motionFlags = 0;
  Actor_SetSingleChildFlags(part, 0);
  Func_02003a54(parts[0], 3);
  Scene_SetActorSpritePriority(parts[0], 3);
  AttachPart(parts[0], anchor, 3);
}

void Func_02002518(u16 id) {
  if (Actor_Get(id)->update != Func_020015e4)
    return;
  BuildParts(Actor_Get(id)->watchedObject);
}

s32 Func_02000668();
void Func_02000ee4();
s32 Func_020006cc();
void Func_0200079c();

s32 Scene_Initialize(void) {
  struct MainRoutineWork *work;
  s32 scene;
  s32 *owner, *leader;
  s16 *entrance;
  struct Actor *actor;
  /* FAKEMATCH(temporary): one local for a table, a step and a size; as one
   * local each, 92 of 732 instructions differ in tla-en. */
  s32 other, rise;
  s32 offset;
  s32 value1, value2, value3, value4;
  u8 zero;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  work = &gMainRoutineWork;
  work->retreatScene = (s32)&SceneId_Ov691Room1;
  work->retreatEntrance = 1;
  scene = work->scene;
  zero = 0;
  if (scene == (s32)&SceneId_Ov691Room1) {
    value1 = 32;
    Actor_GetById(work->currentOwner)->priorityFlags |= value1;
    Actor_GetById(8)->priorityFlags |= value1;
    Actor_GetById(10)->priorityFlags |= value1;
    Func_02000668(gData_02003d9e);
    if (Event_GetFlag(0x109) == 0)
      Call3((void (*)())Func_02000ee4, 21, 13, 0);
    if (Event_GetFlag(0x8d5))
      Actor_SetPosition(8, 0x1080000, 0x1680000);
    if (Event_GetFlag(0x8d6))
      Actor_SetPosition(10, 0xf80000, 0x1980000);
    other = (s32)gData_02003d98;
    value2 = 78;
    Func_02003c0c(other);
    MapGrid_CopyCellAttributes(98, 21, 4, 3, value2, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, value2, 24);
    Func_020006cc(other);
    if (Event_GetFlag(0x109) == 0) {
      actor = Actor_GetById(20);
      actor->x += 0x10000;
      actor->motionFlags = zero;
      actor->collisionFlags = zero;
      actor->terrainHeight = actor->y += 0x180000;
      Actor_SetSingleChildFlags(actor, 0);
    }
    Call1((void (*)())Func_02002518, 20);
    switch (work->entrance) {
    case 3:
    case 4:
      offset = 0x100000;
      goto move;
    }
    goto settle;
  } else if (scene == (s32)&SceneId_Ov691Room2) {
    owner = &work->currentOwner;
    Actor_GetById(*owner)->priorityFlags |= 32;
    entrance = &work->entrance;
    if (work->entrance <= 3) {
      s32 id;

      Func_02002d08(gData_02003dba);
      Func_02000668(gData_02003db2);
      Func_02003bec();
      Call3((void (*)())Func_02003bf4, 0, 11, 12);
      if (Event_GetFlag(0x8d7))
        Actor_SetPosition(15, 0x1b80000, 0x2180000);
      Func_02003c0c(gData_02003dac);
      id = *entrance;
      if (id == 1) {
        if (Event_GetFlag(0x109) == 0) {
          actor = Actor_GetById(*owner);
          actor->z += 0x100000;
        }
        Call1((void (*)())Func_0200244c, -0x100000);
      } else if (id == 2) {
        if (Event_GetFlag(0x109) == 0) {
          actor = Actor_GetById(*owner);
          actor->z += 0x100000;
        }
        Call1((void (*)())Func_0200244c, -0x600000);
      }
    } else {
      if (*entrance <= 8) {
        s32 id;

        if (Event_GetFlag(0x109) == 0) {
          Actor_GetById(8)->motionFlags = zero;
          rise = 0x500000;
          Actor_GetById(8)->y += rise;
          Actor_GetById(9)->motionFlags = zero;
          Actor_GetById(9)->y += rise;
          Call3((void (*)())Func_02000ee4, 19, 8, 0);
          Actor_GetById(10)->motionFlags = zero;
          rise = 0xa00000;
          Actor_GetById(10)->y += rise;
          Actor_GetById(11)->motionFlags = zero;
          Actor_GetById(11)->y += rise;
          Call3((void (*)())Func_02000ee4, 20, 10, 1);
          Actor_GetById(12)->motionFlags = zero;
          rise = 0xf00000;
          Actor_GetById(12)->y += rise;
          Actor_GetById(13)->motionFlags = zero;
          Actor_GetById(13)->y += rise;
          Call3((void (*)())Func_02000ee4, 21, 12, 0);
        }
        if (Event_GetFlag(0x8d8))
          Actor_SetPosition(16, 0x3780000, 0x2380000);
        Func_02003c0c(gData_02003dd2);
        Func_02000668(gData_02003dc4);
        if (Event_GetFlag(0x109) == 0) {
          actor = Actor_GetById(15);
          other = 0x10000;
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          rise = 0x180000;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSingleChildFlags(actor, 0);
          actor = Actor_GetById(18);
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSingleChildFlags(actor, 0);
        }
        Call1((void (*)())Func_02002518, 15);
        Call1((void (*)())Func_02002518, 18);
        id = *entrance;
        if (id == 5) {
          Call1((void (*)())Func_0200244c, 0x1200000);
        } else if (id == 6) {
          if (Event_GetFlag(0x109) == 0)
            Func_020022ac();
        } else if (id == 7) {
          if (Event_GetFlag(0x109) == 0) {
            actor = Actor_GetById(*owner);
            actor->z += 0x100000;
          }
          Call1((void (*)())Func_0200244c, -0x300000);
        } else if (id == 8) {
          offset = 0x680000;
          goto move;
        }
      } else if (*entrance <= 12) {
        if (Event_GetFlag(0x8d9))
          Actor_SetPosition(8, 0x1080000, 0x2480000);
        rise = (s32)gData_02003de4;
        Func_02003c0c(rise);
        Func_020006cc(rise);
        Func_02003be4();
        Call4((void (*)())Func_02003bfc, 0, 0x250, 14, 13);
        Func_02000668(gData_02003dd6);
        if (Event_GetFlag(0x109) == 0) {
          actor = Actor_GetById(10);
          other = 0x10000;
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          rise = 0x180000;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSingleChildFlags(actor, 0);
          actor = Actor_GetById(12);
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSingleChildFlags(actor, 0);
        }
        Call1((void (*)())Func_02002518, 10);
        Call1((void (*)())Func_02002518, 12);
        if (*entrance == 9 || *entrance == 10) {
          offset = 0x300000;
          goto move;
        }
      } else {
        Func_02000668(gData_02003dee);
        if (Event_GetFlag(0x109) == 0) {
          actor = Actor_GetById(13);
          actor->x += 0x10000;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += 0x180000;
          Actor_SetSingleChildFlags(actor, 0);
        }
        Call1((void (*)())Func_02002518, 13);
        if (Event_GetFlag(0x8da))
          Actor_SetPosition(8, 0x2880000, 0x2580000);
        if (Event_GetFlag(0x8db))
          Actor_SetPosition(9, 0x2380000, 0x2680000);
        Func_0200079c(gData_02003de8);
        if (Event_GetFlag(0x250))
          MapGrid_CopyCellAttributes(121, 48, 1, 3, 123, 48);
        if (Event_GetFlag(0x251))
          MapGrid_CopyCellAttributes(121, 48, 1, 3, 125, 48);
        if (Event_GetFlag(0x252))
          MapGrid_CopyCellAttributes(108, 56, 1, 3, 109, 56);
        if (*entrance == 14) {
          Call1((void (*)())Func_0200244c, 0x300000);
        } else if (*entrance == 16 || *entrance == 17) {
          if (Event_GetFlag(0x109) == 0) {
            actor = Actor_GetById(work->currentOwner);
            actor->z += 0x100000;
          }
          offset = -0x200000;
        move:
          Func_0200244c(offset);
        }
      }
    settle:
      Func_020024c8();
    }
  } else if (scene == (s32)&SceneId_Ov691Room3) {
    if (Event_GetFlag(0x8dc)) {
      Actor_SetPosition(8, 0, 0);
      value3 = 42;
      MapGrid_CopyCellAttributes(0, 32, 5, 8, value3, 17);
      MapGrid_CopyCellAttributes(0, 96, 5, 8, value3, 81);
      value4 = 5;
      other = 8;
      MapGrid_CopyCellsTo(0, 32, 42, 17, value4, other);
      MapGrid_CopyCellsTo(0, 96, 42, 81, value4, other);
    }
    leader = &work->currentOwner;
    Actor_GetById(*leader)->priorityFlags |= 32;
    Func_02000668(gData_02003e02);
    if (work->entrance <= 2) {
      if (Event_GetFlag(0x109) == 0) {
        actor = Actor_GetById(*leader);
        actor->z += 0x100000;
      }
      Call1((void (*)())Func_0200244c, -0x200000);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// On entering, sets each listed pillar from its two flags, then lifts the
// leader onto the ground under it.
#include "map.h"

extern u8 gMapShapeGrid[];

s32 Func_02003adc(s32 layer, s32 x, s32 z);
u8 Func_02003c24(s32 layer, s32 shape, s32 height);

DEFINE_FUNC_02002D08(Func_02002d08, Func_02003c04, Func_02003adc, Func_02003b3c,
                     Func_02003c24, Func_02003a54)

void Func_020035e0(void);

void Func_02002e30(u16 *list) {
  struct Actor *leader;
  struct Actor *target;
  s32 position[3];

  if (Func_02003b8c() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003c2c(position, 0);
  if (target != NULL)
    goto test;
  return;
body:
  {
    s32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!GameFlag_Test(flag)) {
        Sound_Play(125);
        Actor_SetSpriteColor(Actor_GetById(id), 7);
        wait(2);
        Func_02003a54(target, 0);
        Actor_SetSpriteColor(Actor_GetById(id), 0);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(id), 7);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(id), 0);
        Func_020034ac(0);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        leader->motionFlags = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_020034ac(0);
        wait(8);
        Func_02003a54(target, 2);
        Func_02003784(-2, target->terrainId);
        Func_020034ac(1);
        wait(16);
        Func_02003784(-2, target->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(target, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_020035e0();
        wait(20);
        GameFlag_SetBit(flag + 1);
      }
    }
    Func_02003af4();
    return;
  }
test:
  if (list[0] != 0xffff)
    goto body;
}

void Func_02002fc4(struct Func_02002fc4_Entry *entry, s32 id) {
  struct Actor *target;
  s32 flag;
  s32 nextFlag;

  target = Actor_Get(id);
  if (entry->id == 0xffff)
    return;
body:
  if (entry->id != id) {
    entry++;
    goto test;
  }
  flag = entry->flag;
  Event_Start();
  nextFlag = flag + 1;
  if (Event_GetFlag(nextFlag) == 0) {
    Event_PlaySound(185);
    MapGrid_SetShake(0x10000, 0x40000, 0x10000);
    Func_020034ac(0);
    System_Wait(8);
    Func_02003a54(target, 2);
    Func_020036f8(-4, target->terrainId, id);
    Func_020034ac(1);
    System_Wait(16);
    MapGrid_SetShake(-1, -1, 0xe666);
    System_Wait(8);
    FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
    System_Wait(2);
    Event_PlaySound(188);
    Func_020035e0();
    System_Wait(20);
    Event_SetFlag(flag);
    Event_SetFlag(nextFlag);
  }
  Func_02003af4();
  return;
test:
  if (entry->id != 0xffff)
    goto body;
}

void Func_020030a4(s32 id) {
  Actor_Get(id)->unk5c[0] = 3;
  Func_02003b3c(id);
}

// Places the listed actors on their cells.

void Actor_MoveToObject(s32 id, s32 owner);

/// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
/// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
/// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
/// it copy actor 9's visual attributes.
DEFINE_FUNC_020030BC(Func_020030bc, Func_020030a4, Func_02003c04, Func_02003adc,
                     Func_02003b3c, Func_02003c24)

void Func_02003224(struct Actor *object, struct Actor *source) {
  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  FieldActor_SetPositionAndResetMotion(source, 0, 0, 0);
}

void Func_02003240(u16 *list) {
  struct Actor *leader;
  struct Actor *actor8;
  struct Actor *target;
  s32 position[3];

  if (Func_02003b8c() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  actor8 = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Func_02003c2c(position, 0);
  if (target == 0)
    return;
  /* FAKEMATCH(dead): a test no u16 can pass enters the loop a second way, so it
   * is no natural loop and its references are not weighted; as if (target != 0)
   * goto test; return;, 75 of 191 instructions differ in tla-en. */
  if (list[0] & 0x10000)
    goto body;
  goto test;
body:
  {
    u32 id = list[0];

    if (Actor_GetById(id) != target) {
      list += 2;
      goto test;
    }
    {
      s32 flag = list[1];

      Event_Start();
      if (!GameFlag_Test(flag)) {
        Func_02003a54(actor8, 1);
        Func_02003224(actor8, target);
        wait(1);
        Sound_Play(125);
        Actor_SetSpriteColor(Actor_GetById(8), 7);
        wait(2);
        Func_02003a54(actor8, 0);
        Actor_CopyGraphics(id, 9);
        Actor_SetSpriteColor(Actor_GetById(8), 0);
        wait(2);
        Actor_SetSpriteColor(Actor_GetById(8), 7);
        wait(4);
        Actor_SetSpriteColor(Actor_GetById(8), 0);
        Func_020034ac(0);
        Func_02003224(target, actor8);
        wait(1);
        GameFlag_SetBit(flag);
      } else if (!GameFlag_Test(flag + 1)) {
        u8 *flags;

        Func_02003a54(actor8, 0);
        Func_02003224(actor8, target);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Sound_Play(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        Func_020034ac(0);
        wait(8);
        Func_02003a54(actor8, 2);
        Func_02003784(-2, target->terrainId);
        Func_020034ac(1);
        wait(16);
        Func_02003784(-2, target->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(actor8, 0, 0, 0);
        wait(2);
        Sound_Play(188);
        Func_020035e0();
        wait(20);
        GameFlag_SetBit(flag + 1);
      }
      Func_02003af4();
      return;
    }
  }
test:
  if (list[0] != 0xffff)
    goto body;
}

#include "../../field/field_effect/advance_motion_priority.c"
