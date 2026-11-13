/// @file
/// Magma Rock's rooms set up, the leader launched and landed, and the
/// crumbling pillars and traps.

#include "../shared/map_cells.h"
#include "../shared/actors.h"
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

s32 Trig_Cos(s32 angle);
void FieldEffect_AdvanceMotion();
void Map_CenterOnFollowedPoint(void);
void Func_02003af4(void);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       struct MagmaRockEffectOptions *options);

void MagmaRock_LaunchLeader(void) {
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
  velocity[0] = Trig_Cos(angle);
  velocity[1] = 0;
  velocity[2] = Math_Sin(angle);
  velocity[0] += velocity[0] >> 1;
  velocity[0] += ((Random16() * 3) << 14 >> 16) - 0x6000;
  velocity[2] += ((Random16() * 3) << 13 >> 16) - 0x3000;
  FieldEffect_Spawn(leader->x, leader->y, leader->z, velocity[0], velocity[1],
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

DEFINE_LEADER_LIFT(Leader_Lift)

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

void MagmaRock_LandLeader(void) {
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

void MagmaRock_RaiseFlameTop(void);
void MagmaRock_RaiseFlamePart(void);

static inline void AttachPart(struct Actor *part, struct Actor *anchor,
                              u8 order) {
  BYTE_AT(part, 98) = order;
  part->watchedObject = anchor;
  part->update = MagmaRock_RaiseFlamePart;
  FieldActor_SetSpriteColor(part, 3);
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
  Actor_SetSpriteFlags(part, 0);
  Actor_SelectAnimation(parts[i], 3);
  AttachPart(parts[i], anchor, i - 1);
  if (++i != 4)
    goto next;
  part = parts[0];
  part->motionFlags = 0;
  Actor_SetSpriteFlags(part, 0);
  Actor_SelectAnimation(parts[0], 3);
  Scene_SetActorSpritePriority(parts[0], 3);
  AttachPart(parts[0], anchor, 3);
}

void MagmaRock_RebuildFlameParts(u16 id) {
  if (Actor_Get(id)->update != MagmaRock_RaiseFlameTop)
    return;
  BuildParts(Actor_Get(id)->watchedObject);
}

s32 MagmaRock_RaiseActorCells();
void MagmaRock_PlaceVolley();
s32 Blocks_MarkCells();
void MagmaRock_SettleBlocks();

s32 Scene_Initialize(void) {
  struct MainRoutineWork *work;
  s32 scene;
  s32 *owner, *leader;
  s16 *entrance;
  struct Actor *actor;
  // other serves in turn as a table, a step and a size.
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
    MagmaRock_RaiseActorCells(gMagmaRock_Room1Statues);
    if (Event_GetFlag(0x109) == 0)
      Call3((void (*)())MagmaRock_PlaceVolley, 21, 13, 0);
    if (Event_GetFlag(0x8d5))
      Actor_SetPosition(8, 0x1080000, 0x1680000);
    if (Event_GetFlag(0x8d6))
      Actor_SetPosition(10, 0xf80000, 0x1980000);
    other = (s32)gMagmaRock_Room1Pillars;
    value2 = 78;
    Actor_PlaceBlocks(other);
    MapGrid_CopyCellAttributes(98, 21, 4, 3, value2, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, value2, 24);
    Blocks_MarkCells(other);
    if (Event_GetFlag(0x109) == 0) {
      actor = Actor_GetById(20);
      actor->x += 0x10000;
      actor->motionFlags = zero;
      actor->collisionFlags = zero;
      actor->terrainHeight = actor->y += 0x180000;
      Actor_SetSpriteFlags(actor, 0);
    }
    Call1((void (*)())MagmaRock_RebuildFlameParts, 20);
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

      MagmaRock_RestoreCrumblingPillars(gMagmaRock_CrumblingPillars);
      MagmaRock_RaiseActorCells(gMagmaRock_Room2LowerStatues);
      Tightrope_Initialize();
      Call3((void (*)())Func_02003bf4, 0, 11, 12);
      if (Event_GetFlag(0x8d7))
        Actor_SetPosition(15, 0x1b80000, 0x2180000);
      Actor_PlaceBlocks(gMagmaRock_Room2LowerPillars);
      id = *entrance;
      if (id == 1) {
        if (Event_GetFlag(0x109) == 0) {
          actor = Actor_GetById(*owner);
          actor->z += 0x100000;
        }
        Call1((void (*)())Leader_Lift, -0x100000);
      } else if (id == 2) {
        if (Event_GetFlag(0x109) == 0) {
          actor = Actor_GetById(*owner);
          actor->z += 0x100000;
        }
        Call1((void (*)())Leader_Lift, -0x600000);
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
          Call3((void (*)())MagmaRock_PlaceVolley, 19, 8, 0);
          Actor_GetById(10)->motionFlags = zero;
          rise = 0xa00000;
          Actor_GetById(10)->y += rise;
          Actor_GetById(11)->motionFlags = zero;
          Actor_GetById(11)->y += rise;
          Call3((void (*)())MagmaRock_PlaceVolley, 20, 10, 1);
          Actor_GetById(12)->motionFlags = zero;
          rise = 0xf00000;
          Actor_GetById(12)->y += rise;
          Actor_GetById(13)->motionFlags = zero;
          Actor_GetById(13)->y += rise;
          Call3((void (*)())MagmaRock_PlaceVolley, 21, 12, 0);
        }
        if (Event_GetFlag(0x8d8))
          Actor_SetPosition(16, 0x3780000, 0x2380000);
        Actor_PlaceBlocks(gMagmaRock_Room2MiddlePillars);
        MagmaRock_RaiseActorCells(gMagmaRock_Room2MiddleStatues);
        if (Event_GetFlag(0x109) == 0) {
          actor = Actor_GetById(15);
          other = 0x10000;
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          rise = 0x180000;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSpriteFlags(actor, 0);
          actor = Actor_GetById(18);
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSpriteFlags(actor, 0);
        }
        Call1((void (*)())MagmaRock_RebuildFlameParts, 15);
        Call1((void (*)())MagmaRock_RebuildFlameParts, 18);
        id = *entrance;
        if (id == 5) {
          Call1((void (*)())Leader_Lift, 0x1200000);
        } else if (id == 6) {
          if (Event_GetFlag(0x109) == 0)
            MagmaRock_LaunchLeader();
        } else if (id == 7) {
          if (Event_GetFlag(0x109) == 0) {
            actor = Actor_GetById(*owner);
            actor->z += 0x100000;
          }
          Call1((void (*)())Leader_Lift, -0x300000);
        } else if (id == 8) {
          offset = 0x680000;
          goto move;
        }
      } else if (*entrance <= 12) {
        if (Event_GetFlag(0x8d9))
          Actor_SetPosition(8, 0x1080000, 0x2480000);
        rise = (s32)gMagmaRock_Room2UpperPillars;
        Actor_PlaceBlocks(rise);
        Blocks_MarkCells(rise);
        Rope_Initialize();
        Call4((void (*)())Func_02003bfc, 0, 0x250, 14, 13);
        MagmaRock_RaiseActorCells(gMagmaRock_Room2UpperStatues);
        if (Event_GetFlag(0x109) == 0) {
          actor = Actor_GetById(10);
          other = 0x10000;
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          rise = 0x180000;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSpriteFlags(actor, 0);
          actor = Actor_GetById(12);
          actor->x += other;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += rise;
          Actor_SetSpriteFlags(actor, 0);
        }
        Call1((void (*)())MagmaRock_RebuildFlameParts, 10);
        Call1((void (*)())MagmaRock_RebuildFlameParts, 12);
        if (*entrance == 9 || *entrance == 10) {
          offset = 0x300000;
          goto move;
        }
      } else {
        MagmaRock_RaiseActorCells(gMagmaRock_Room2TopStatues);
        if (Event_GetFlag(0x109) == 0) {
          actor = Actor_GetById(13);
          actor->x += 0x10000;
          actor->motionFlags = zero;
          actor->collisionFlags = zero;
          actor->terrainHeight = actor->y += 0x180000;
          Actor_SetSpriteFlags(actor, 0);
        }
        Call1((void (*)())MagmaRock_RebuildFlameParts, 13);
        if (Event_GetFlag(0x8da))
          Actor_SetPosition(8, 0x2880000, 0x2580000);
        if (Event_GetFlag(0x8db))
          Actor_SetPosition(9, 0x2380000, 0x2680000);
        MagmaRock_SettleBlocks(gMagmaRock_Room2TopPillars);
        if (Event_GetFlag(0x250))
          MapGrid_CopyCellAttributes(121, 48, 1, 3, 123, 48);
        if (Event_GetFlag(0x251))
          MapGrid_CopyCellAttributes(121, 48, 1, 3, 125, 48);
        if (Event_GetFlag(0x252))
          MapGrid_CopyCellAttributes(108, 56, 1, 3, 109, 56);
        if (*entrance == 14) {
          Call1((void (*)())Leader_Lift, 0x300000);
        } else if (*entrance == 16 || *entrance == 17) {
          if (Event_GetFlag(0x109) == 0) {
            actor = Actor_GetById(work->currentOwner);
            actor->z += 0x100000;
          }
          offset = -0x200000;
        move:
          Leader_Lift(offset);
        }
      }
    settle:
      MagmaRock_LandLeader();
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
    MagmaRock_RaiseActorCells(gMagmaRock_Room3Statues);
    if (work->entrance <= 2) {
      if (Event_GetFlag(0x109) == 0) {
        actor = Actor_GetById(*leader);
        actor->z += 0x100000;
      }
      Call1((void (*)())Leader_Lift, -0x200000);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

// On entering, sets each listed pillar from its two flags, then lifts the
// leader onto the ground under it.
#include "map.h"

extern u8 gMapShapeGrid[];

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
u8 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

DEFINE_PILLARS_RESTORE_RAISED(MagmaRock_RestoreCrumblingPillars,
                              Actor_GetCellIndex, MapGrid_GetTerrainKind,
                              Actor_DisableById, MapCollision_FindOrAddTag,
                              Actor_SelectAnimation)

void MagmaRock_SpawnDustRing(void);

void MagmaRock_StepOnCrumblingPillar(u16 *list) {
  struct Actor *leader;
  struct Actor *target;
  s32 position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Actor_FindAtCell(position, 0);
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
      if (!Event_GetFlag(flag)) {
        Event_PlaySound(125);
        FieldActor_SetSpriteColor(Actor_GetById(id), 7);
        wait(2);
        Actor_SelectAnimation(target, 0);
        FieldActor_SetSpriteColor(Actor_GetById(id), 0);
        wait(2);
        FieldActor_SetSpriteColor(Actor_GetById(id), 7);
        wait(4);
        FieldActor_SetSpriteColor(Actor_GetById(id), 0);
        MagmaRock_SpawnRubble(0);
        Event_SetFlag(flag);
      } else if (!Event_GetFlag(flag + 1)) {
        leader->motionFlags = 0;
        Event_PlaySound(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        MagmaRock_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(target, 2);
        MagmaRock_SetLeaderCellShape(-2, target->terrainId);
        MagmaRock_SpawnRubble(1);
        wait(16);
        MagmaRock_SetLeaderCellShape(-2, target->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        leader->motionFlags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(target, 0, 0, 0);
        wait(2);
        Event_PlaySound(188);
        MagmaRock_SpawnDustRing();
        wait(20);
        Event_SetFlag(flag + 1);
      }
    }
    Func_02003af4();
    return;
  }
test:
  if (list[0] != 0xffff)
    goto body;
}

void MagmaRock_CrumblePillar(struct MagmaRockCrumblingPillar *entry, s32 id) {
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
    MagmaRock_SpawnRubble(0);
    System_Wait(8);
    Actor_SelectAnimation(target, 2);
    MagmaRock_SetActorCellShape(-4, target->terrainId, id);
    MagmaRock_SpawnRubble(1);
    System_Wait(16);
    MapGrid_SetShake(-1, -1, 0xe666);
    System_Wait(8);
    FieldActor_SetPositionAndResetMotion(target, 0, 0, 0);
    System_Wait(2);
    Event_PlaySound(188);
    MagmaRock_SpawnDustRing();
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

void MagmaRock_SetPillarMode3(s32 id) {
  Actor_Get(id)->unk5c[0] = 3;
  Actor_DisableById(id);
}

// Places the listed actors on their cells.

void Actor_MoveToObject(s32 id, s32 owner);

/// Places actors 8, 9 and 10, then walks a 0xffff-ended list of (actor, flag)
/// halfword pairs: flag + 1 set parks the actor at (0, 0); otherwise its cell
/// gets attributeB 128 and its shape-grid byte is rewritten, and flag set makes
/// it copy actor 9's visual attributes.
DEFINE_PILLARS_RESTORE(MagmaRock_RestorePillars, MagmaRock_SetPillarMode3,
                       Actor_GetCellIndex, MapGrid_GetTerrainKind,
                       Actor_DisableById, MapCollision_FindOrAddTag)

void MagmaRock_TakePlaceOf(struct Actor *object, struct Actor *source) {
  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  FieldActor_SetPositionAndResetMotion(source, 0, 0, 0);
}

void MagmaRock_StepOnTrap(u16 *list) {
  struct Actor *leader;
  struct Actor *actor8;
  struct Actor *target;
  s32 position[3];

  if (Field_TryJumpForward() != 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  actor8 = Actor_GetById(8);
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  target = Actor_FindAtCell(position, 0);
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
      if (!Event_GetFlag(flag)) {
        Actor_SelectAnimation(actor8, 1);
        MagmaRock_TakePlaceOf(actor8, target);
        wait(1);
        Event_PlaySound(125);
        FieldActor_SetSpriteColor(Actor_GetById(8), 7);
        wait(2);
        Actor_SelectAnimation(actor8, 0);
        Actor_CopyGraphics(id, 9);
        FieldActor_SetSpriteColor(Actor_GetById(8), 0);
        wait(2);
        FieldActor_SetSpriteColor(Actor_GetById(8), 7);
        wait(4);
        FieldActor_SetSpriteColor(Actor_GetById(8), 0);
        MagmaRock_SpawnRubble(0);
        MagmaRock_TakePlaceOf(target, actor8);
        wait(1);
        Event_SetFlag(flag);
      } else if (!Event_GetFlag(flag + 1)) {
        u8 *flags;

        Actor_SelectAnimation(actor8, 0);
        MagmaRock_TakePlaceOf(actor8, target);
        wait(1);
        flags = &leader->motionFlags;
        *flags = 0;
        Event_PlaySound(185);
        MapGrid_SetShake(0x10000, 0x40000, 0x10000);
        MagmaRock_SpawnRubble(0);
        wait(8);
        Actor_SelectAnimation(actor8, 2);
        MagmaRock_SetLeaderCellShape(-2, target->terrainId);
        MagmaRock_SpawnRubble(1);
        wait(16);
        MagmaRock_SetLeaderCellShape(-2, target->terrainId);
        wait(4);
        MapGrid_SetShake(-1, -1, 0xe666);
        wait(8);
        *flags = 3;
        wait(5);
        Actor_SetPositionAndResetMotion(actor8, 0, 0, 0);
        wait(2);
        Event_PlaySound(188);
        MagmaRock_SpawnDustRing();
        wait(20);
        Event_SetFlag(flag + 1);
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

// ---- scene/magma_rock/s34ac.c ----

/// Magma Rock's rubble and falling rocks, and its tables.

#include "../shared/map_cells.h"
#include "../shared/effects.h"
#include "../shared/tables.h"
#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"
#include "map.h"
#include "system/ram_buffer.h"
#include "resource.h"
#include "heap_release.h"
#include "field.h"
#include "module/field/field_effect.h"
#include "module/scene/magma_rock.h"
#include "particle.h"

void Actor_StartScript(struct Actor *object, const s32 *script);
s32 Trig_Cos(s32 angle);

void MagmaRock_SpawnRubble(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)FieldActor_Create(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gMagmaRock_RubbleScript);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Math_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

void MagmaRock_SpawnDustRing(void) {
  struct Actor *leader;
  struct FieldEffect *effect;
  s32 state;
  u32 i;
  s32 angle;
  u8 zero;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader == 0)
    return;
  // Default: without it loop.c lifts the set of zero out of the loop.
  zero = 0;
  state = 0;
  for (i = 0; i <= 16; i++) {
    effect = (struct FieldEffect *)FieldActor_Create(285, leader->x, leader->y,
                                                     leader->z);
    if (effect != 0) {
      state = Sprite_ShareVramCacheEntry(effect->sprite, state);
      zero = 0;
      effect->motionFlags = zero;
      effect->collisionFlags = zero;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gMagmaRock_DustScript);
      angle = i << 12;
      effect->velocity.x = Trig_Cos(angle);
      effect->velocity.y = zero;
      effect->velocity.z = Math_Sin(angle);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += (Random16() * 0xc000 >> 16) - 0x6000;
      effect->velocity.z += (Random16() * 0x6000 >> 16) - 0x3000;
      effect->spin = (Random16() * 0x1000 >> 16) - 0x800;
      effect->scaleRateX = zero;
      effect->scaleRateY = zero;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
  }
}

extern u8 gMapShapeGrid[];

s32 MapGrid_GetTerrainKind(s32 terrainId, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
/* VIEW(s34ac): routine MapCollision_FindOrAddTag as s32
 * MapCollision_FindOrAddTag(s32 terrainId, s32 value, s32 height);
 * scene/magma_rock/s22ac has u8 MapCollision_FindOrAddTag(s32 layer, s32 shape,
 * s32 height); tried as the definition's s32 MapCollision_FindOrAddTag(s32
 * unused, s32 key, s32 value), which the object already declares,
 * MagmaRock_SetActorCellShape moves: 1 of 60 instructions in tla-ja. */
s32 MapCollision_FindOrAddTag__s34ac(
    s32 terrainId, s32 value, s32 height) __asm__("MapCollision_FindOrAddTag");

DEFINE_ACTOR_SET_CELL_SHAPE_AT(MagmaRock_SetActorCellShape,
                               MapGrid_GetTerrainKind,
                               MapCollision_FindOrAddTag__s34ac)

/* VIEW(s34ac): routine MagmaRock_StepOnCrumblingPillar as void
 * MagmaRock_StepOnCrumblingPillar(const u8 *data); scene/magma_rock/s22ac has
 * void MagmaRock_StepOnCrumblingPillar(u16 *list) {; tried as the definition's
 * void MagmaRock_StepOnCrumblingPillar(u16 *list), which the object already
 * declares, MagmaRock_SetActorCellShape (gone) moves: 60 of 60 instructions in
 * tla-ja. */
void MagmaRock_StepOnCrumblingPillar__s34ac(const u8 *data) __asm__(
    "MagmaRock_StepOnCrumblingPillar");

void MagmaRock_SetLeaderCellShape(s32 a, s32 b) {
  MagmaRock_SetActorCellShape(a, b, gMainRoutineWork.currentOwner);
}

struct ParticleSprite {
  u8 unk0[8];
  u16 attr;
  u8 unkA[30];
};

struct ParticleWork {
  u8 tiles[0x500];
  struct Particle particles[64];
  struct ParticleSprite sprites[64];
  s16 next;
  u16 tile;
};

void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

DEFINE_PARTICLES_UPDATE_FALLING(Particles_UpdateFalling,
                                EffectSprite_DrawAtWorldPosition)

void MagmaRock_DropRocks(s32 x, s32 y, s32 z, s32 count) {
  struct EntryWork *work;
  s32 i;

  work = Ram_HeapSlots->menuRuntime;
  if (count > 0) {
    i = count;
  next:
    {
      struct Entry *entry;
      s32 angle = Random16();

      entry = &work->entries[work->unk1600];
      entry->unk18 = 0;
      entry->x = x;
      entry->y = y;
      entry->z = z;
      FieldVector_AddPolarOffset(Random16() * 8 + 0x80000, angle, entry);
      entry->velocityX = 0;
      entry->velocityY = 0x50000;
      entry->velocityZ = 0;
      FieldVector_AddPolarOffset(Random16() * 2 + 0x20000, angle,
                                 &entry->velocityX);
      {
        s32 mask = 63;

        work->unk1600 = (work->unk1600 + 1) & mask;
      }
      i--;
    }
    if (i != 0)
      goto next;
  }
}

// The falling rocks' setup: loads their picture, sets up 64 small sprites,
// marks each rock idle and starts the task that spawns them.
#include "system/task.h"

extern char ResourceId_FallingRock;

struct Rock {
  u8 unk00[24];
  s32 timer;
};

struct RockSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  u16 resource;
};

void Import_DecompressTaggedLz(void *entry, void *destination);
void EffectSprite_Init(struct RockSprite *sprite, s32 width, s32 height,
                       u32 attributes, s32 tile);

void MagmaRock_StartFallingRocks(void) {
  struct RockWork *work = HeapSlot_Alloc(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Import_DecompressTaggedLz(
      Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    EffectSprite_Init(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  Task_AddFrame(Particles_UpdateFalling, 0x480);
}

void MagmaRock_EndFallingRocks(void) {
  struct MagmaRockSparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Particles_UpdateFalling);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(220);
}

#include "scene_tables.h"
#include "objcmd.h"

void MagmaRock_UnblockMovedPillar(void);
void MagmaRock_BlockMovedPillar(void);
void MagmaRock_TargetStatue(void);
void MagmaRock_BurstColumn1(void);
void MagmaRock_BurstColumn2(void);
void MagmaRock_BurstColumn3(void);
void MagmaRock_BurstColumn4(void);
void MagmaRock_BurstColumn5(void);
void MagmaRock_BlastOpenWall(void);

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

DEFINE_PUSH_LEADER_STEPS(gPushLeaderSteps)

u32 gMagmaRock_EmberScript[] = {
    39, 2, 0, 18, 3, 0,
    1, 38, 17,
};

u32 gMagmaRock_FlameEmberScript[] = {
    39, 3, 0, 23, 3, 0,
    1, 38, 17,
};

s16 gMagmaRock_Room1Pillars[] = {
    8, 10, -1,
};

s16 gMagmaRock_Room1Statues[] = {
    9, 50, 20, 12, 30, -1, -1,
};

s16 gMagmaRock_Room2LowerPillars[] = {
    8, 15, -1,
};

s16 gMagmaRock_Room2LowerStatues[] = {
    13, 30, -1, -1,
};

s16 gMagmaRock_CrumblingPillars[] = {
    9, 512, 10, 514, -1,
};

s16 gMagmaRock_Room2MiddleStatues[] = {
    14, 50, 15, 17, 51, 18, -1,
};

s16 gMagmaRock_Room2MiddlePillars[] = {
    16, -1,
};

s16 gMagmaRock_Room2UpperStatues[] = {
    9, 50, 10, 11, 51, 12, -1,
};

s16 gMagmaRock_Room2UpperPillars[] = {
    8, -1,
};

s16 gMagmaRock_Room2TopPillars[] = {
    8, 9, -1,
};

s16 gMagmaRock_Room2TopStatues[] = {
    11, 30, -1, 12, 50, 13, 15, 31,
    -1, -1,
};

s16 gMagmaRock_Room3Statues[] = {
    9, 50, -1, -1, 0,
};

DEFINE_G_DATA_02001540(gMagmaRock_RubbleScript, 33)

DEFINE_G_DATA_02001540(gMagmaRock_DustScript, 44)

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gMagmaRock_Regions[] = {
    240, 328, 352, 256, 344, 368, -1, 3,
    448, 328, 352, 464, 344, 368, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gMagmaRock_Room2Regions[] = {
    272, 56, 576, 288, 72, 592, -1, 1,
    352, 40, 560, 368, 56, 576, -1, 2,
    752, 512, 536, 768, 528, 552, -1, 5,
    784, 80, 592, 800, 96, 608, -1, 7,
    896, 512, 528, 912, 528, 544, -1, 8,
    272, 48, 576, 288, 64, 592, -1, 9,
    352, 32, 560, 368, 48, 576, -1, 10,
    784, 56, 592, 800, 72, 608, -1, 14,
    752, 0xfea8, 672, 768, 0xfeb8, 688, -1, 16,
    976, 0xfea8, 672, 992, 0xfeb8, 688, -1, 17,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gMagmaRock_Room3Regions[] = {
    240, -56, 448, 256, -40, 464, -1, 1,
    384, -56, 448, 400, -40, 464, -1, 2,
    416, -128, 448, 432, -112, 464, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(223),
    SCENE_EXIT(1, 2, 51),
    SCENE_EXIT(2, 232, 11),
    SCENE_EXIT(3, 224, 16),
    SCENE_EXIT(4, 224, 17),
    SCENE_EXITS(224),
    SCENE_EXIT(1, 224, 9),
    SCENE_EXIT(2, 224, 10),
    SCENE_EXIT(3, 224, 4),
    SCENE_EXIT(4, 224, 3),
    SCENE_EXIT(5, 225, 1),
    SCENE_EXIT(6, 225, 2),
    SCENE_EXIT(7, 224, 14),
    SCENE_EXIT(8, 225, 2),
    SCENE_EXIT(9, 224, 1),
    SCENE_EXIT(10, 224, 2),
    SCENE_EXIT(11, 224, 13),
    SCENE_EXIT(12, 224, 15),
    SCENE_EXIT(13, 224, 11),
    SCENE_EXIT(14, 224, 7),
    SCENE_EXIT(15, 224, 12),
    SCENE_EXIT(16, 223, 3),
    SCENE_EXIT(17, 223, 4),
    SCENE_EXITS(225),
    SCENE_EXIT(1, 224, 5),
    SCENE_EXIT(2, 224, 8),
    SCENE_EXIT(3, 227, 8),
    SCENE_EXIT(4, 224, 6),
    SCENE_EXITS_END,
};

struct ActorPlacement gMagmaRock_Placements[] = {
    {272, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 6, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 7, 0, 0, FACING_WEST, 2, 2},
    {404, CONDITION_ALWAYS, 1, 8, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 9, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 10, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_NORTH, 2, 2},
    {329, CONDITION_ALWAYS, 1, 12, 0, 0, FACING_NORTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_NORTH, 2, 2},
    {403, CONDITION_ALWAYS, 7, PIXELS(8), 0, PIXELS(8), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2LowerPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_NORTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_SOUTH, 2, 3},
    {377, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_EAST, 2, 2},
    {404, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 8, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2MiddlePlacements[] = {
    {377, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 12, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 13, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 13, 0, 0, FACING_NORTH, 2, 2},
    {403, CONDITION_ALWAYS, 7, PIXELS(8), 0, PIXELS(8), FACING_WEST, 2, 0},
    {403, CONDITION_ALWAYS, 7, PIXELS(8), 0, PIXELS(8), FACING_WEST, 2, 0},
    {403, CONDITION_ALWAYS, 7, PIXELS(8), 0, PIXELS(8), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2UpperPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 14, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 15, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 15, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 16, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 16, 0, 0, FACING_NORTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 17, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 18, 0, 0, FACING_SOUTH, 2, 3},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2TopPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 19, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 20, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 21, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 22, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 23, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 23, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 24, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 25, 0, 0, FACING_WEST, 2, 2},
    {299, CONDITION_ALWAYS, 7, 26, 0, 0, FACING_NORTH, 2, 2},
    {299, CONDITION_ALWAYS, 7, 27, 0, 0, FACING_NORTH, 2, 2},
    {299, CONDITION_ALWAYS, 7, 28, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room3Placements[] = {
    {384, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_WEST, 2, 2},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gMagmaRock_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xca02, 2, 0x2f8, EVENT(MagmaRock_SlideDown)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(MagmaRock_ClearSlideFlag)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 19, CONDITION_ALWAYS, 0},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x8d5, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x8d5,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     0x8d6, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x8d6,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31, 0x200,
     EVENT(MagmaRock_BurstColumn1)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 32, 0x201,
     EVENT(MagmaRock_BurstColumn2)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 33, 0x202,
     EVENT(MagmaRock_BurstColumn3)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 34, 0x203,
     EVENT(MagmaRock_BurstColumn4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 35, 0x204,
     EVENT(MagmaRock_BurstColumn5)},
    {0x80009705, 30, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2LowerTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x202, 20, CONDITION_ALWAYS, EVENT(MagmaRock_CheckCrumblingPillars)},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     0x8d7, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x8d7,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31, 0x200,
     EVENT(MagmaRock_BurstColumn1)},
    {TRIGGER_STEP, 50, 0x212, EVENT(MagmaRock_SetLedgeBehavior)},
    {TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(MagmaRock_ClearLedgeBehavior)},
    {0x80009705, 30, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2MiddleTriggers[] = {
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16,
     0x8d8, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, 0x8d8,
     EVENT(MagmaRock_BlockMovedPillar)},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {0x80009705, 51, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(MagmaRock_AmplifyBurst)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2UpperTriggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x602, 40, 0x8, EVENT(MagmaRock_PushPillarEast)},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x8d9, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x8d9,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 14, 0x250, 0},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {0x80009705, 51, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2TopTriggers[] = {
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0x8602, 40, CONDITION_ALWAYS, EVENT(MagmaRock_PushPillarWest)},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x8da, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x8da,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9,
     0x8db, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x8db,
     EVENT(MagmaRock_BlockMovedPillar)},
    {0x80009705, 30, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {0x80009705, 31, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 31,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 32, 0x201,
     EVENT(MagmaRock_BurstColumn2)},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 16, 0x250, EVENT(MagmaRock_GrowVine1)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 17, 0x251, EVENT(MagmaRock_GrowVine2)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 18, 0x252, EVENT(MagmaRock_GrowVine3)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room3Triggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_BlastOpenWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {TRIGGER_TABLE_END},
};
