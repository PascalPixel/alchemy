/// @file
/// The scene code of overlay 0x6a9, in ROM order.

#include "shared/actors.h"
#include "types.h"
#include "scene_tables.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct CavernsMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct CavernsMapWork {
  u8 unk000[0x108];
  struct CavernsMapLayer layers[3];
};

extern char SceneId_Ov6a9Room1;
extern char SceneId_Ov6a9Room2;
extern char SceneId_Ov6a9Room3;
extern char SceneId_Ov6a9Room4;
extern u8 gMapCellBuffer[];
extern s16 gData_02001220[];
extern s16 gData_02001264[];
extern struct ActorPlacement gCaverns_Placements[];
extern struct ActorPlacement gCaverns_Room1Placements[];
extern struct ActorPlacement gCaverns_Room4Placements[];
extern struct Trigger gCaverns_Triggers[];
extern struct Trigger gCaverns_Room1Triggers[];
extern struct Trigger gCaverns_Room3Triggers[];
extern struct Trigger gCaverns_Room4Triggers[];

void Func_02000454(const s16 *data);
void Func_02000d10(void);
void Func_02000ea0(void);
void Actor_SelectAnimation(struct Actor *object, s32 value);
s32 MapGrid_CheckCollision(struct Actor *object, s16 *position);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void Func_02000ba8(void);
void Func_02000f50(void);
void Func_02000f98(void);
s32 Func_02000c38(struct Actor *object, s16 *position);
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "module/field/object.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "field/event_inline.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/ram_buffer.h"
#include "system/field_effect.h"
#include "fixed_math.h"
#include "macros.h"
#include "system/main_routine_work.h"
#include "event_work.h"
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

DEFINE_SCENE_CREATE_IMPORTED_ACTOR(Scene_CreateImportedActor, 0x22)

#include "../field/field_effect/move.c"

// @file
// Scene code of overlay 0x6a9, in ROM order.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

/// What games/tla/src/scene/ declares for the files that use it, written once.

struct Actor;

void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

extern u32 gData_0200116c[];

extern u32 gData_020011a8[];
extern u32 gData_020011e4[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const s16 *Scene_GetRegions(void) {
  const s16 *result = NULL;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6a9Room2)
    result = gData_02001264;
  return result;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6a9Room1)
    return gCaverns_Room1Placements;
  if (scene == (s32)&SceneId_Ov6a9Room4)
    return gCaverns_Room4Placements;
  return gCaverns_Placements;
}

s32 Func_020003b4(u32 layer, s32 x, s32 y) {
  struct CavernsMapWork *map = (struct CavernsMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  return (cell[2] << 8) | cell[3];
}

s32 Func_020003f0(u32 layer, s32 x, s32 y, s32 value) {
  struct CavernsMapWork *map = (struct CavernsMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}

#include "../field/object/wait_until_idle.c"

#include "text/msg_ids.h"

void Func_02000454(const s16 *data) {
  const u16 *ids = (const u16 *)data;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object;
  s32 height;
  s32 cell;

  if (*ids == 0xffff)
    goto done;
loop:
  object = Actor_GetById(*ids);
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->bounce = 0;
  Actor_SetSpriteFlags((struct Actor *)object, 0);
  height = (Map_GetTerrainHeight(0, object->x, object->z) >> 19) + 2;
  MapGrid_SetCellHeight(object->x, object->z, 0, height);
  MapGrid_SetCellHeight(object->x, object->z, 2, height);
  cell = Func_020003b4(0, object->x >> 20, object->z >> 20);
  object->unk4c = cell;
  Func_020003f0(0, object->x >> 20, object->z >> 20, cell | 0x80);
  ids++;
  if (*ids != 0xffff)
    goto loop;
done:
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    wait(1);
    leader->y = leader->terrainHeight =
        Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
    leader->velocityY = 0;
    leader->targetY = 0x80000000;
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
  }
}

static __inline__ s32 GetHeight(s32 x, s32 z) {
  return Map_GetTerrainHeight(2, x, z);
}

static __inline__ void SetHeight(s32 x, s32 z, s32 height) {
  MapGrid_SetCellHeight(x, z, 2, height);
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
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    angle = (leader->facing + 0x2000) & 0xc000;
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z;
    Vector_AddPolarOffset(0x100000, angle, position);
    if (*ids == 0xffff)
      goto search_done;
  search:
    object = Actor_GetById(*ids);
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
  MapGrid_SetCellHeight(object->x, object->z, 0, height);
  MapGrid_SetCellHeight(object->x, object->z, 2, height);
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
  ((struct FieldSprite *)object->sprite)->rotation += object->timer;
}

void Func_02000848(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 leaderZ = leader->z;
  s32 z = object->z;
  u8 *flags = &leader->priorityFlags;

  if (leaderZ > z) {
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

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void Actor_Release(struct Actor *object);

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
    kind = MapGrid_GetCellBehaviorAt(2, object->x, object->z);
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
      Actor_Release(spawned);
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
        velocity[0] = Trig_Cos(angle) * 2;
        velocity[1] = 0;
        velocity[2] = Trig_Sin(angle) * 2;
        velocity[0] += velocity[0] >> 1;
        velocity[0] += ((u32)(Random16() * 3 << 14) >> 16) - 0x6000;
        velocity[2] += ((u32)(Random16() * 3 << 13) >> 16) - 0x3000;
        /* FAKEMATCH(dead): sum, never read again, assigned the flags inside
         * the call's arguments; as 0x10b0000 passed directly, 6 of 255
         * instructions differ in tla-en. */
        FieldEffect_Spawn(object->x, object->y, object->z, velocity[0],
                          velocity[1], velocity[2], sum = 0x10b0000, &options);
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
      other = Actor_FindAtCell(velocity, 1);
      if (other != NULL) {
        object->update = (void (*)(void))Func_02000848;
        object->riseCounter = 1;
        other->riseCounter = 1;
        GameFlag_Set(0x212);
      }
    }
  }
  height = (Map_GetTerrainHeight(0, object->x, object->z) >> 19) + 2;
  MapGrid_SetCellHeight(object->x, object->z, 0, height);
  MapGrid_SetCellHeight(object->x, object->z, 2, height);
  value = Func_020003b4(0, object->x >> 20, object->z >> 20);
  object->unk4c = value;
  Func_020003f0(0, object->x >> 20, object->z >> 20, value | 128);
}

const struct Trigger *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6a9Room1)
    return gCaverns_Room1Triggers;
  if (scene == (s32)&SceneId_Ov6a9Room3)
    return gCaverns_Room3Triggers;
  if (scene == (s32)&SceneId_Ov6a9Room4)
    return gCaverns_Room4Triggers;
  return gCaverns_Triggers;
}

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6a9Room1)
    Func_02000ba8();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6a9Room4)
    Func_02000f98();
  return 0;
}

s32 Scene_PrepareMap(void) {
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
  struct Actor *object;
  s32 arg;

  Actor_GetById(4)->priorityFlags |= 0x20;
  Rope_Initialize();
  Rope_Place(0, 0x200, 8, 9);
  object = Actor_GetById(10);
  object->motionFlags = 0;

  arg = 10;
  Map_CopyCellAttributeRect(16, arg, 1, 1, object->x >> 20, arg);
}

void Func_02000bf8(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->motionFlags = 0;
  Map_CopyCellAttributeRect(13, 12, 2, 1, 13, 10);
  Map_CopyCellAttributeRect(16, 10, 1, 1, object->x >> 20, 10);
}

s32 Func_02000c38(struct Actor *object, s16 *position) {
  u8 *flags = &object->motionFlags;
  s32 saved = *flags;
  s32 blocked = MapGrid_CheckCollision(object, position);

  if (blocked == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SelectAnimation(object, 6);
    wait(6);
    Sound_Play(152);
    Actor_SelectAnimation(object, 7);
    object->speedLimit = 0x30000;
    object->acceleration = 0x20000;
    object->velocityY = 0x40000;
    *flags &= 0x7e;
    Actor_SetSpriteFlags(object, 0);
    {
      struct MainRoutineWork *party = &gMainRoutineWork;
      s32 x = position[1];
      s32 z = position[5];
      s32 owner = party->currentOwner;

      Actor_MoveToAndWaitById(owner, x, z);
    }
    Actor_SelectAnimation(object, 6);
    Actor_SetSpriteFlags(object, 1);
    *flags = saved;
    Event_End();
    return 1;
  }
  return 0;
}

void Func_02000cdc(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
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
  MapGrid_CopyCellPictureIndicesRect(35, 12, 18, 12, 3, 3);
  MapGrid_CopyCellAttributes(35, 12, 3, 3, 18, 12);
  MapGrid_CopyCellPictureIndicesRect(84, 13, 83, 13, 1, 1);
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
  BYTE_AT(Actor_GetById(4), 90) &= ~1;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffsetById(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSpriteFlags(Actor_GetById(4), 0);
  Actor_SetDestinationOffsetById(4, 0, 16);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 13);
  Sound_Play(123);
  Event_Wait(14);
  Import_EventSetExitRequest(value);
}

void Func_02000e20(void) {
  Actor_WalkByAndWait(4, 0, -14);
  Actor_WalkByAndWaitById(4, 16, 0);
  Event_Wait(10);
  Func_02000d68();
  return;
}

void Func_02000e44(void) {
  Actor_WalkByAndWait(4, 0, -14);
  Actor_WalkByAndWaitById(4, -16, 0);
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
  GameFlag_Set(0x9af);
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
  Event_Start();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(31, 43, 9, 11, 48, 47);
  MapGrid_CopyCellsTo(30, 110, 48, 110, 9, 13);
  MapGrid_CopyCellAttributes(52, 110, 1, 1, 52, 111);
  GameFlag_Set(0x917);
  Event_End();
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
  Actor_GetById(8)->priorityFlags |= 0x20;
  Actor_GetById(9)->priorityFlags |= 0x20;
  Actor_GetById(10)->collisionFlags |= 8;
  Actor_GetById(11)->collisionFlags |= 8;
}

void Func_02000fe4(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(158);
  Event_Wait(10);
  MapGrid_CopyCellsTo(7, 94, 6, 94, 1, 2);
  Event_Wait(16);
  Sound_Play(123);
  Import_EventSetExitRequest(8);
  Event_End();
}

// clang-format off
u32 gData_0200116c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020011a8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020011e4[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gData_02001220[] = {
    8, 9, -1, 0,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gData_0200116c,
    gData_020011a8,
    gData_020011e4,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 467, 0, 153, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gData_02001264[] = {
    132, 28, 692, 140, 36, 700, -1, 4,
    292, 28, 692, 300, 36, 700, -1, 5,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(276),
    SCENE_EXIT_IF(1, 2, 82, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 276, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 276, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 276, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 276, 3, CONDITION_ALWAYS),
    SCENE_EXITS(275),
    SCENE_EXIT_IF(1, 2, 83, CONDITION_ALWAYS),
    SCENE_EXITS(277),
    SCENE_EXIT_IF(5, 2, 88, CONDITION_ALWAYS),
    SCENE_EXITS(278),
    SCENE_EXIT_IF(1, 2, 89, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 278, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 278, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 278, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 278, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 278, 7, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 278, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 278, 9, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 278, 8, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

struct ActorPlacement gCaverns_Placements[] = {
    {489, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gCaverns_Room1Placements[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(168), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(168), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(168), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gCaverns_Room4Placements[] = {
    {315, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(552), FACING_NORTH, 2, 0},
    {315, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(536), FACING_NORTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(512), FACING_NORTH, 2, 0},
    {386, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(512), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gCaverns_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x4602, 2, CONDITION_ALWAYS, EVENT(Func_02000e6c)},
    {0x8602, 2, CONDITION_ALWAYS, EVENT(Func_02000e44)},
    {0x602, 2, CONDITION_ALWAYS, EVENT(Func_02000e20)},
    {0x4602, 3, CONDITION_ALWAYS, EVENT(Func_02000e6c)},
    {0x8602, 3, CONDITION_ALWAYS, EVENT(Func_02000e44)},
    {0x602, 3, CONDITION_ALWAYS, EVENT(Func_02000e20)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, 0x9af, EVENT(Func_02000e88)},
    {TRIGGER_TABLE_END},
};

struct Trigger gCaverns_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x602, 10, CONDITION_ALWAYS, EVENT(Func_02000cdc)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x200, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Func_02000bf8)},
    {TRIGGER_TABLE_END},
};

struct Trigger gCaverns_Room3Triggers[] = {
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 60, CONDITION_ALWAYS, EVENT(Func_02000ef4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gCaverns_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x21, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {0x21, 5, CONDITION_ALWAYS, 5},
    {0x31, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Func_02000fe4)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0x8, 0, 0x212, EVENT(Func_020006a8)},
    {0x9, 0, 0x212, EVENT(Func_0200087c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, 0x212, EVENT(Func_020006a8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x212, EVENT(Func_0200087c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 9, 0x212, EVENT(Func_020006a8)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x212, EVENT(Func_0200087c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, 1), 8, 0x212, EVENT(Func_020006a8)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 8, 0x212, EVENT(Func_0200087c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_CARRY, 1), 9, 0x212, EVENT(Func_020006a8)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 9, 0x212, EVENT(Func_0200087c)},
    {TRIGGER_TABLE_END},
};
