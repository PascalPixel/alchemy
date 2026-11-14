/// @file
/// Air's Rock's slopes, outside the rock in Osenia, in the rooms of this
/// overlay: the Wind Stones Whirlwind strengthens, the walls and ledges of the
/// climb, and the way into the rock itself (airs_rock_inside.c).

#include "types.h"
#include "actor.h"
#include "field/event_inline.h"
#include "module/scene/airs_rock.h"
#include "system/ram_buffer.h"
#include "flags.h"
#include "system/main_routine_work.h"
#include "field/field.h"
#include "event_work.h"
#include "system/frame.h"
#include "map.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "graphics/resource.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "field_event.h"
#include "system/iwram_call.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/input.h"
#include "field/event_work.h"
#include "call.h"
#include "bg_scroll.h"
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect/move.c"

// @file
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

s32 Func_0200033c(s32 a0) {
  Actor_SetSpriteFlags(a0, 0);
  return 0;
}

extern u8 gMapCellBuffer[];

s32 Func_02000348(u32 layer, s32 x, s32 y) {
  struct AirsRockMapWork *map =
      (struct AirsRockMapWork *)Ram_HeapSlots->mapWork;
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

s32 Func_02000384(u32 layer, s32 x, s32 y, s32 value) {
  struct AirsRockMapWork *map =
      (struct AirsRockMapWork *)Ram_HeapSlots->mapWork;
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

#undef EFFECT_SET_SCRIPT

// For each id of a list ended by 0xffff: clears the object's mark and, on a
// cell with attribute bit 6, blocks the cells ahead of and behind it and sets
// the mark.
#include "text/msg_ids.h"

void Actor_PlaceMovedBlock(s32 id);

void Blocks_MarkCells(u16 *ids) {
  struct Actor *object;
  u8 *blocked;
  struct MapWork *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x, z;

  while (*ids != 0xffff) {
    object = Actor_Get(*ids);
    blocked = &object->blockMarked;
    *blocked = 0;
    if (Func_02000348(0, object->x >> 20, object->z >> 20) & 0x40) {
      map = Ram_HeapSlots->mapWork;
      if (map == NULL)
        return;
      cells = &map->layers[2].cells;
      z = (object->z - object->y) >> 20;
      x = object->x >> 20;
      cell = *cells;
      cell += x + ((z + 1) << 7);
      cell->collisionCode = 255;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = 255;
      *blocked = 1;
    }
    ids++;
  }
}

void Func_02000468(struct Unk_02000468 *unk) {
  struct MapWork *map;

  if (unk->unk63 != 0) {
    map = Ram_HeapSlots->mapWork;
    if (map != NULL) {
      struct MapCell **cells = &map->layers[2].cells;
      /* FAKEMATCH(dead): cell's initial value, overwritten unread. */
      struct MapCell *cell = *cells;
      s32 x = unk->x >> 20;
      s32 row = (unk->z - unk->y) >> 20;

      cell = *cells;
      cell += x + ((row + 1) << 7);
      cell->collisionCode = 0;
      cell = *cells;
      cell += x + ((row - 1) << 7);
      cell->collisionCode = 0;
      unk->unk63 = 0;
    }
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern s16 gData_02003f44[];

s16 *Scene_GetRegions(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AirsRock1)
    return gData_02003e84;
  if (scene == (s32)&SceneId_AirsRock2)
    return gData_02003ec4;
  return gData_02003f44;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AirsRock1)
    return gAirsRock_Room1Placements;
  if (scene == (s32)&SceneId_AirsRock2)
    return gAirsRock_Room2Placements;
  return gAirsRock_Placements;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000548(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgFieldWindStoneWarning, 1);
  Event_End();
}

s32 Map_ClearLayerEntryFlag();

void Func_02000568(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_CopyCellsTo(64, 64, 83, 35, 1, 2);
  Map_ClearLayerEntryFlag(0);
  FieldEvent_Wait(45);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 312, 680);
  FieldEvent_Wait(5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(4);
  Event_End();
}

void Func_020005ec(void) {
  struct Actor *object;

  object = Actor_Get(10);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if ((object->x >> 20) == 0x1f) {
    Event_SetFlag(FLAG_AREA_TEMP_10);
  } else {
    Event_ClearFlag(FLAG_AREA_TEMP_10);
  }
  Event_End();
}

// Object 11's column sets or clears flag 0x311 and writes the low byte of the
// cell value Func_02000348 returns (its high byte 0xff when the object is
// away) into two places; with the leader 208 pixels up a block copy follows.
void Func_02000620(void) {
  struct Actor *object;
  struct Actor *leader;
  s32 value;

  object = (struct Actor *)((s32)Actor_Get(11));
  value = Func_02000348(3, 70, 67);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if ((object->x >> 20) == 32) {
    Event_SetFlag(FLAG_AREA_TEMP_11);
    value &= 0xff;
    Func_02000384(4, 70, 67, value);
    Func_02000384(2, 33, 54, value);
  } else {
    Event_ClearFlag(FLAG_AREA_TEMP_11);
    value = (value & 0xff) | 0xff00;
    Func_02000384(4, 70, 67, value);
    Func_02000384(2, 33, 54, value);
  }
  leader = (struct Actor *)((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if ((leader->y >> 19) == 26)
    MapGrid_CopyCellAttributes(70, 67, 1, 1, 33, 54);
  Event_End();
}

void Func_020006d4(void) {
  struct Actor *object;

  object = Actor_Get(13);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if ((object->x >> 19) == 0x17) {
    Event_SetFlag(0x879);
  }
  Event_End();
}

// The leader jumps up 12 pixels (cue 390, mode 6 then 1), climbs 28 pixels
// over 14 frames with cue 217 while turned to angle 0x5000, then lands 16
// pixels on and plays cue 390 again.
void Func_02000700(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 z = leader->z;
  u32 i;

  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Actor_SelectAnimation(leader, 6);
  System_Wait(10);
  Actor_SelectAnimation(leader, 1);
  Actor_SetSpriteFlags(leader, 0);
  leader->motionFlags |= 2;
  leader->velocityY = px(4);
  FieldActor_SetMoveTargetFixed(leader, leader->x, leader->y, z + 0xc0000);
  System_Wait(6);
  Event_PlaySound(SE_FLOOR_SWITCH);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH_SOUTHWEST, 0);
  leader->motionFlags = 0;
  for (i = 0; i < 14; i++) {
    leader->y += -px(2);
    leader->targetY = leader->y;
    System_Wait(1);
  }
  leader->motionFlags = 3;
  leader->velocityY = px(3);
  FieldActor_SetMoveTargetFixed(leader, leader->x, leader->y, z + 0x100000);
  FieldActor_WaitForMove(leader);
  Actor_SetSpriteFlags(leader, 1);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_NoOp();
}

const struct Trigger *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_AirsRock1)
    return gAirsRock_Room1Triggers;
  if (scene == (s32)&SceneId_AirsRock2)
    return gAirsRock_Room2Triggers;
  return gAirsRock_Triggers;
}

void Func_02000824(void) { Event_ResolvePendingActions(0); }

void Func_02000830(void) {
  FIELD_AT(((s32)Ram_HeapSlots->effectWork + 0x34), u8, 0) = 1;
}

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

// Circles the object around the centre in words 68 and 76, the radius
// growing with its height, one step of 256 per lap.
void Func_02000840(struct Actor *object) {
  s32 radius = object->y;
  s32 step = WORD_AT(object, 48);

  radius >>= 1;
  radius += 0x100000;
  object->x =
      WORD_AT(object, 68) + Iwram_MulQ16(Trig_Cos((step & 255) << 11), radius);
  object->z =
      WORD_AT(object, 76) +
      Iwram_MulQ16(Math_Sin((WORD_AT(object, 48) & 255) << 11), radius) / 3;
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
}

// The same circle on a smaller radius, twice as wide, blinking the part
// attribute every other frame.
void Func_020008ac(struct Actor *object) {
  s32 radius = object->y;
  s32 step = WORD_AT(object, 48);

  radius >>= 2;
  radius += 0xc0000;
  object->x = WORD_AT(object, 68) +
              Iwram_MulQ16(Trig_Cos((step & 255) << 11), radius) * 2;
  object->z = WORD_AT(object, 76) +
              Iwram_MulQ16(Math_Sin((WORD_AT(object, 48) & 255) << 11), radius);
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
  FieldActor_SetSpriteColor(object, ((gFrameCount & 2) >> 1) * 9);
}

void Func_0200092c(struct Actor *object) {
  u8 *bytes = (u8 *)object;
  u32 on = ((gFrameCount & 3) >> bytes[99]) & 1;

  FieldActor_SetSpriteColor(object, (u8)(on * bytes[98]));
}

// The Whirlwind gust in Air's Rock.

extern s16 gData_02004674[][4];
void Actor_StartScript(struct Actor *object, const s32 *script);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

// Whirlwind on a target in Air's Rock: a gust (object type 280) rises from the
// target and grows, shedding sparks every fourth frame while the target
// flickers; then it travels in the leader's facing, two cells a step, until
// Func_02000348 finds a cell with something on it, throwing leaves as it goes.
// With nothing there (255) it ends with cue 136; otherwise it stops one and a
// half cells short and blows the thing found along, copying its cells each
// fourth frame, and sets that thing's flag (0x850 + hit - 1) with cue 298.
// Then the gust shrinks and rises away over 8 frames.
void Func_0200095c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectWork *effect = (struct EffectWork *)Ram_HeapSlots->effectWork;
  struct Actor *gust, *spark;
  struct Vector3 velocity;
  struct EffectOptions leaves, dust;
  struct Cell delta, cell;
  s32 cellZ, cellX, newZ, newX, flag = 0, steps = 0, hit, angle, i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_RUSH);
  gust = FieldActor_Create(280, leader->x, leader->y, leader->z);
  Actor_SelectAnimation(gust, 2);
  Actor_SetSpriteFlags(gust, 0);
  gust->scaleX = 0x1b333;
  gust->scaleY = 0x3333;
  ((struct MotionView *)gust)->flags = flag;
  ((struct FieldSprite *)gust->sprite)->priority =
      ((struct FieldSprite *)effect->target->sprite)->priority;
  Actor_SetMoveTarget(gust, effect->target->x, effect->target->y,
                      effect->target->z + 0xffff0000);
  while (gust->scaleY <= 0x14ccb) {
    gust->scaleY += 0x1999;
    Event_Wait(1);
  }
  Actor_WaitForMove(gust);
  gust->z = effect->target->z + 0xffff0000;
  while (gust->scaleY <= 0x1cccb) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(0x2a0, effect->target->x, 0,
                                                 effect->target->z);
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0xcccc;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gData_02003d1c);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = Func_02000840;
    }
    if (gFrameCount & 1) {
      gust->scaleX += 655;
      gust->scaleY += 655;
      Actor_SetSpriteColor(effect->target, 15);
    } else {
      Actor_SetSpriteColor(effect->target, 4);
    }
    Event_Wait(1);
  }
  Actor_SetSpriteColor(effect->target, 0);
  Sound_Play(SE_WHOOSH);
  cell.x = effect->target->x >> 20;
  cell.z = effect->target->z >> 20;
  angle = (leader->facing + 0x2000) & 0xc000;
  delta.x = Trig_Cos(angle) >> 16;
  delta.z = Trig_Sin(angle) >> 16;
  do {
    hit = Func_02000348(2, cell.x, cell.z);
    hit >>= 8;
    cell.x += delta.x;
    cell.z += delta.z;
    steps += 2;
  } while (hit == 0);
  flag = Func_02000348(2, cell.x, cell.z);
  flag >>= 8;
  flag -= 1;
  steps -= 2;
  newX = ((s16 *)&effect->target->x)[1] + ((steps * delta.x) << 3);
  newZ = ((s16 *)&effect->target->z)[1] + ((steps * delta.z) << 3);
  ((struct FieldSprite *)gust->sprite)->priority = 1;
  cell.x = newX;
  cell.z = newZ;
  gust->speedLimit = px(4);
  gust->acceleration = px(4);
  Actor_SetMoveTarget(gust, cell.x << 16, gust->y, cell.z << 16);
  FieldCamera_SetSpeedAndAcceleration(5, 5);
  FieldCamera_MoveToFixed((s32)(newX << 16), (s32)(-1), (s32)(newZ << 16),
                          (s32)(1));
  while (Actor_HasArrived(gust) == 0) {
    if ((gFrameCount & 1) == 0) {
      leaves.priority = 1;
      leaves.type = 674;
      leaves.palette = 2;
      velocity.x = Trig_Cos((gFrameCount & 0x1ff) << 12) << 1;
      velocity.y = gust->y + ((Random16() & 31) << 16);
      velocity.z = Trig_Sin((gFrameCount & 0x1ff) << 12);
      FieldEffect_Spawn(
          gust->x, velocity.y, gust->z, velocity.x, 0, velocity.z,
          EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &leaves);
    }
    Event_Wait(1);
  }
  if (hit == 255) {
    Sound_Play(SE_WHOOSH);
  } else {
    cell.x = ((s16 *)&gust->x)[1] + delta.x * 48;
    cell.z = ((s16 *)&gust->z)[1] + delta.z * 48;
    gust->speedLimit = px(1) / 2;
    gust->acceleration = px(1) / 2;
    Actor_SetMoveTarget(gust, cell.x << 16, gust->y, cell.z << 16);
    Sound_Play(SE_235);
    hit -= 30;
    while (Actor_HasArrived(gust) == 0) {
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        dust.startScaleX = dust.startScaleY = (Random16() & 0xffff) + 0x10000;
        velocity.x = Trig_Cos((gFrameCount & 0x1ff) << 12) << 1;
        velocity.y = px(1);
        velocity.z = Trig_Sin((gFrameCount & 0x1ff) << 12);
        FieldEffect_Spawn(gust->x, gust->y, gust->z, velocity.x, velocity.y,
                          velocity.z,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY, &dust);
      }
      if ((gFrameCount & 3) == 0) {
        s32 sourceX;
        cell.x = gData_02004674[hit][2] + (gust->x >> 20);
        cell.z = gData_02004674[hit][3] + (gust->z >> 20);
        cellX = cell.x;
        cellZ = cell.z;
        if (hit & 1) {
          sourceX = (u16)gData_02004674[hit][0];
          MapGrid_CopyCellsTo((s16)sourceX, gData_02004674[hit][1], cellX,
                              cellZ - 1, 1, 3);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 2, cellX,
                                     cellZ);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 2,
                                     gust->x >> 20, gust->z >> 20);
        } else {
          sourceX = (u16)gData_02004674[hit][0];
          MapGrid_CopyCellsTo((s16)sourceX, gData_02004674[hit][1], cellX,
                              cell.z - (hit >> 1), 1, 1);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 1, cellX,
                                     cellZ);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 1,
                                     gust->x >> 20, gust->z >> 20);
        }
        Map_CenterOnFollowedPoint();
      }
      Event_Wait(1);
    }
    if (hit == 2) {
      MapGrid_CopyCellsTo(gData_02004674[2][0], gData_02004674[2][1], cell.x,
                          cell.z, 1, 1);
      Map_CenterOnFollowedPoint();
    }
    Sound_Play(SE_298);
    Event_SetFlag(flag + 0x850);
  }
  gust->acceleration = 0;
  gust->speedLimit = 0;
  gust->targetZ = 0;
  gust->targetY = 0;
  gust->targetX = 0;
  for (i = 0; i != 8; i++) {
    gust->scaleX -= 0x2666;
    gust->scaleY -= Q16_ONE / 4;
    gust->y += 0x38000;
    Event_Wait(1);
  }
  Actor_Release(gust);
  Event_Wait(30);
  FieldCamera_MoveToActorById(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Event_End();
}

// The wave scroll that overlay 696's Func_02000378 also has.
#define WAVE_SCROLL Func_02000ef4
#define WAVE_SCROLL_LINES gData_020046a0
#include "../field/field_effect/wave_scroll.c"

// Moves the object: left of x 320 it speeds up rightward and grows, past it
// it speeds up leftward and shrinks; its priority follows its direction and
// its sprite spins.
void Func_02000f9c(struct Actor *object) {
  s32 scale;

  object->y += object->gravity;
  object->x += object->bounce;
  object->z += object->unk4c;
  if ((object->x >> 19) <= 39) {
    object->bounce += Q16_ONE * 3 / 2;
    scale = object->scaleX + 0xccc;
  } else {
    object->bounce -= Q16_ONE * 3 / 2;
    scale = object->scaleX - 0x51e;
  }
  object->scaleX = scale;
  object->scaleY = scale;
  if (object->bounce > 0)
    Scene_SetActorSpritePriority(object, 1);
  else
    Scene_SetActorSpritePriority(object, 2);
  ((struct FieldSprite *)object->sprite)->rotation += 0x1800;
}

struct SpriteParts {
  u8 unk00[9];
  u8 unk09Low : 2;
  u8 part : 2;
};

void Func_02001010(void) {
  struct MainRoutineWork *work;
  u16 *buffer = Heap_Alloc(0x680);
  u16 *cell;
  struct Actor *spark;
  u32 i;
  s32 x;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  REG_DISPCNT &= ~0x100;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                      buffer);
  Dma_Set(buffer, BG_SCREEN_BLOCK(29), DMA_COPY_WORDS(208), REG_DMA3);
  Dma_Set((u8 *)buffer + 0x340, (void *)0x600ec00, DMA_COPY_WORDS(208),
          REG_DMA3);
  Heap_Release(buffer);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  FieldActor_SetSpritePriority(8, 1);
  {
    s32 control = 0x40d;

    REG_BG0CNT = control;
  }
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 2;
  Dma_Fill16(0x10, (void *)0x6002000, 0x500);
  buffer = Heap_Alloc(0x240);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap),
                      buffer);
  i = 0;
  cell = buffer;
offset:
  *cell += 0xff40;
  i++;
  cell++;
  if (i != 0x120)
    goto offset;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(0x600200f + (i << 6)), DMA_COPY_WORDS(8), REG_DMA3);
  i++;
  cell += 16;
  if (i != 18)
    goto row;
  Heap_Release(buffer);
  Event_PlaySound(SE_GLITTER);
  ((u8 *)Actor_Get(8))[98] = 9;
  ((u8 *)Actor_Get(8))[99] = 1;
  Actor_Get(8)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = FieldActor_Create(0x2a0, Actor_Get(8)->x, Actor_Get(8)->y,
                                Actor_Get(8)->z);
      {
        s32 priority = ((s8 *)Actor_Get(8)->sprite)[9] & 12;

        cell = spark->sprite;
        ((struct SpriteParts *)cell)->part = priority >> 2;
      }
      ((struct ObjBits *)spark)->flags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = px(3) / 2;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gData_02003d1c);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_Wait(1);
  }
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(330, CAMERA_KEEP, 73, 1);
  FieldCamera_WaitForArrival();
  Task_AddFrame(Func_02000ef4, 0xc80);
  Event_PlaySound(SE_163);
  REG_BLDALPHA = 0xf00;
  REG_BLDCNT = 0x3f41;
  REG_DISPCNT |= 0x100;
  MapGrid_SetShake(1, 1, 1);
  i = 0;
glow:
  REG_BLDALPHA = ((15 - (i >> 2)) << 8) | (i >> 2);
  ((u8 *)Actor_Get(8))[98] = Random16() & 7;
  i++;
  FieldEvent_Wait(1);
  if (i != 64)
    goto glow;
  FieldEvent_Wait(60);
  REG_BLDALPHA = 15;
  REG_BLDCNT = 0xc42;
  MapGrid_SetShake(3, 3, 1);
  {
    u32 j = 0;

  burst:
    j++;
    if ((j & 3) == 0) {
      struct Vector3 velocity;
      struct EffectOptions options;

      options.priority = 1;
      options.update = Func_02000f9c;
      velocity.x = 0;
      velocity.y = 0;
      velocity.z = 0xfffe0000;
      x = ((Random16() % 144) << 16) + 0x1000000;
      FieldEffect_Spawn(x, 0, (Random16() % 144) << 16, velocity.x, velocity.y,
                        velocity.z, EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY | 1,
                        &options);
    }
    REG_BLDALPHA = (((j >> 4) & 15) << 8) | (16 - ((j >> 4) & 15));
    ((u8 *)Actor_Get(8))[98] = Random16() & 7;
    FieldEvent_Wait(1);
    if (j != 255)
      goto burst;
  }
  MapGrid_CopyCellsTo(68, 0, 79, 0, 11, 10);
  MapGrid_CopyCellAttributes(38, 9, 1, 1, 16, 9);
  MapGrid_CopyCellAttributes(38, 9, 1, 1, 24, 9);
  Map_CenterOnFollowedPoint();
  FieldEvent_Wait(1);
  REG_BLDCNT = 0x3f41;
  REG_BLDALPHA = 15;
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  ((u8 *)Actor_Get(8))[98] = 6;
  i = 0;
fade:
  REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
  FieldEvent_Wait(1);
  i++;
  if (i != 65)
    goto fade;
  FieldTask_Remove(Func_02000ef4);
  {
    s32 zero = 0;

    ((u8 *)Actor_Get(8))[99] = zero;
    System_Wait(30);
    Actor_Get(8)->update = (void (*)(void))zero;
    FieldActor_SetSpriteColor(Actor_Get(8), 6);
    FieldMapGrid_WaitForShakeToSettle();
    FieldEvent_Wait(60);
    REG_BLDCNT = zero;
  }
  Window_LoadGraphics();
  DebugText_Clear();
  work = &gMainRoutineWork;
  WindowColor_Apply(work->paletteGlow[0], work->paletteGlow[1]);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  {
    s32 *ownerId = &work->currentOwner;

    Actor_Get(*ownerId)->priorityFlags &= ~32;
    Actor_Get(*ownerId)->priorityFlags |= 1;
    {
      u8 *flags = &Actor_Get(8)->priorityFlags;
      u8 v = *flags | 1;

      *flags = v;
    }
    EventCamera_FollowActor(*ownerId, 1);
  }
  FieldCamera_WaitForArrival();
  Event_SetFlag(0x858);
  Event_End();
}

void Func_02001560(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *ride = leader->watchedObject;
  struct Actor *rider, *dust;
  u8 *riding, *motion;
  s32 rise, dx, dz, moved, i, height;
  s32 angle;
  u16 facing;
  s32 still;

  Event_Start();
  Event_ResolvePendingActions(0);
  motion = &leader->motionFlags;
  *motion = 4;
  Actor_SetSpriteFlags(leader, 0);
  GameFlag_Set(FLAG_SCENE_TEMP_50);
  Sound_Play(SE_137);
  riding = (u8 *)ride + 99;
  rise = 0;
  while (*riding != 0) {
    leader->x = ride->x;
    leader->y = ride->y + rise;
    leader->z = ride->z;
    if (rise <= 0xbffff)
      rise += 0x1999;
    leader->facing = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(SE_288);
  facing = ride->facing;
  if (facing == 0xc000) {
    rider = ride->watchedObject;
    dx = 0;
    dz = 0;
    leader->priorityFlags = 4;
    leader->x = (((leader->x >> 19) & ~1) + 1) << 19;
    i = 0;
    leader->y = (((ride->y >> 19) & ~1) + 3) << 19;
    leader->z = (((leader->z >> 19) & ~1) + 1) << 19;
  waiting:
    moved = 0;
    if (gInput.held & KEY_UP) {
      dx = 0;
      dz = 0xfff00000;
      leader->facing = 0xc000;
      moved = 1;
    }
    if (gInput.held & KEY_RIGHT) {
      dx = 0x100000;
      dz = 0;
      leader->facing = dz;
      moved = 1;
    }
    if (gInput.held & KEY_LEFT) {
      dx = 0xfff00000;
      leader->facing = 0x8000;
      dz = 0;
      moved = 1;
    }
    if (moved && Func_02000348(2, (ride->x + dx) >> 20,
                               (ride->z - ride->y + dz) >> 20) >>
                         8 !=
                     255)
      goto found;
    wait(1);
    if (++i != 60)
      goto waiting;
    dx = 0;
    leader->facing = 0xc000;
    dz = 0xfff00000;
    if ((u32)(Func_02000348(2, ride->x >> 20, (ride->z - ride->y + dz) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = 0x100000;
    dz = 0;
    leader->facing = dz;
    if ((u32)(Func_02000348(2, (ride->x + dx) >> 20, (ride->z - ride->y) >> 20)
              << 8) >>
            16 !=
        255)
      goto found;
    dx = 0xfff00000;
    leader->facing = 0x8000;
  found:
    height =
        Map_GetTerrainHeight(((u8 *)rider)[98], ride->x + dx, ride->z + dz);
    leader->y = height;
    leader->terrainHeight = height;
    Actor_SelectAnimation(leader, 6);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(leader, 7);
    leader->speedLimit = px(2);
    leader->acceleration = px(1) / 2;
    leader->velocityY = px(4);
    *motion = 2;
    leader->priorityFlags = 1;
    leader->terrainId = ((u8 *)rider)[98];
    FieldActor_SetMoveTargetFixed(leader, leader->x + dx, leader->y,
                                  leader->z + dz);
    OverlayObject_WaitUntilIdle(leader);
    FieldActor_WaitForMove(leader);
    Actor_SetSpriteFlags(leader, 1);
    *motion = 3;
  } else {
    angle = ride->facing;
    leader->x = (((ride->x >> 19) & ~1) + (angle >>= 13) - 1) << 19;
    leader->z = (((ride->z >> 19) & ~1) - (angle & 2) + 1) << 19;
    still = 0;
    Actor_SetSpriteFlags(leader, 1);
    leader->velocityY = px(6);
    leader->priorityFlags = 1;
    *motion = 3;
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    dust = FieldCamera_GetActor();
    dust->speedLimit = px(8);
    dust->acceleration = px(1);
    dust->motionFlags = still;
    FieldActor_SetMoveTargetFixed(dust, leader->x,
                                  Map_GetTerrainHeight(0, leader->x, leader->z),
                                  leader->z);
    OverlayObject_WaitUntilIdle(leader);
    FieldActor_WaitForMove(dust);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Event_ClearFlag(FLAG_SCENE_TEMP_50);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  wait(10);
  Event_End();
}

#include "shared/position_flat_distance.h"

#include "shared/position_distance_above.h"

extern u32 gData_02003de0[];
extern u32 gData_02003dcc[];

void Func_02001908(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *work = Ram_HeapSlots->eventWork;
  s32 speed = 0x18000;
  s32 cue = 539;
  struct Actor *other, *launcher;
  s32 i;
  s32 *leaderPosition;
  s32 *shotPosition;
  struct Actor **objects;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0 ||
      gMainRoutineWork.renderMode == 5) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000348(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
  end:
    Event_PlayCueNearFocus(shot, cue);
    ((struct Shot *)shot)->unk63 = 0;
    shot->update = 0;
    if (shot->facing == 0xc000)
      Actor_StartScript(shot, gData_02003de0);
    else
      Actor_StartScript(shot, gData_02003dcc);
    return;
  }
  if (((struct Shot *)shot)->unk62 == 0) {
    // i first says whether the shot is beside the leader; the loop below
    // reuses it as the object index.
    i = 0;
    leaderPosition = &leader->x;
    shotPosition = &shot->x;
    if (gMainRoutineWork.renderMode == 2) {
      if (Position_DistanceAbove(leaderPosition, shotPosition) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        i = 1;
    } else {
      if (Position_FlatDistance(leaderPosition, shotPosition) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        i = 1;
    }
    if (i && !GameFlag_Get(0x104)) {
      leader->facing = shot->facing;
      leader->watchedObject = shot;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->sprite)->priority =
          ((struct FieldSprite *)shot->sprite)->priority;
      ((struct EventWork *)work)->unk16a = 200;
      gMainRoutineWork.renderMode = 0;
      ((struct Shot *)shot)->unk62 = 1;
      ((struct Shot *)shot)->unk63 = 1;
    }
  }
  objects = (struct Actor **)(work + 0x34);
  for (i = 8; i != 64; i++) {
    other = *objects++;
    if (other->script != 0) {
      launcher = shot->watchedObject;
      if (launcher->x >> 20 != other->x >> 20 ||
          launcher->y >> 20 != other->y >> 20 ||
          launcher->z >> 20 != other->z >> 20) {
        if (Position_FlatDistance(&other->x, &shot->x) <= 8 &&
            other->y >> 20 == shot->y >> 20)
          goto end;
      }
    }
  }
  if (shot->facing == 0xc000) {
    shot->x += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->y -= Iwram_MulQ16(Math_Sin(shot->facing), speed);
  } else {
    shot->x += Iwram_MulQ16(Trig_Cos(shot->facing), speed);
    shot->z += Iwram_MulQ16(Math_Sin(shot->facing), speed);
  }
}

// A launcher's update, the Air's Rock Inside launcher with a new shot each
// time: while none of the event work's four busy counters is set it counts
// frames in its action parameter, and at 240 launches a shot 8 pixels ahead
// along its angle with a dust effect below it.
void Func_02001b54(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0)
    return;
  if (object->timer == 240) {
    if (!Event_GetFlag(FLAG_SCENE_TEMP_50))
      Event_PlayCueNearFocus(object, 202);
    if (object->facing == 0xc000) {
      position.x = object->x;
      position.z = object->z;
    } else {
      /* FAKEMATCH(wrapper): forwarded. */
      position.x = object->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Trig_Cos(object->facing));
      position.z = object->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Math_Sin(object->facing));
    }
    shot = FieldActor_Create(280, position.x, object->y, position.z);
    Actor_SelectAnimation(shot, 2);
    Actor_SetSpriteFlags(shot, 0);
    ((struct FieldSprite *)shot->sprite)->priority = 1;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->facing = object->facing;
    shot->update = Func_02001908;
    shot->watchedObject = object;
    object->timer = 0;
    options.priority = 1;
    options.palette = 7;
    FieldEffect_Spawn(position.x, object->y, position.z - px(6), 0, 0, 0,
                      EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->timer++;
}

void Func_02001ca4(void) {
  struct AirsRockEventWork *work = Ram_HeapSlots->eventWork;

  Actor_LiftTargetBlock(gData_02003d94);
  Func_02000468(work->valueCbc);
}

/* VIEW(airs_rock): routine Actor_PlaceMovedBlock as void
 * Actor_PlaceMovedBlock(void); module/scene/scene.h has void
 * Actor_PlaceMovedBlock(s32 id); tried as the definition's void
 * Actor_PlaceMovedBlock(void), which the object already declares, it does not
 * compile in tla-ja (airs_rock.c): games/tla/src/scene/airs_rock.c:1056: too
 * few arguments to function `Actor_PlaceMovedBlock'. */
extern void Func_02003cc0__airs_rock(void) __asm__("Actor_PlaceMovedBlock");

void Func_02001cc8(void) {
  struct Actor *object =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  s32 x, z;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Func_02003cc0__airs_rock();
  Blocks_MarkCells(((u8 *)gData_02003d94));
  if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    OverlayObject_WaitUntilIdle(object);
    OverlayObject_WaitUntilIdle(object);
    Event_PlaySound(SE_THUMP);
    object->motionFlags = 0;
    object->terrainId = 0;
  }
  x = object->x >> 20;
  z = object->z >> 20;
  if (x == 16 && z == 43)
    Event_SetFlag(0x85f);
  else if (x == 45 && z == 45)
    Event_SetFlag(0x860);
  else if (x == 21 && z == 48)
    Event_SetFlag(0x861);
  else if (x == 39 && z == 49)
    Event_SetFlag(0x862);
  else if (x == 49 && z == 18)
    Event_SetFlag(0x887);
  Event_End();
}

s32 FlagBlock_ApplyFlags();

void Func_02001da8(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FlagBlock_ApplyFlags((s32)((u8 *)gData_02003e4c));
  Event_End();
}

extern s32 gData_02003dfc[];

void Func_02001dc4(s32 x, s32 z) {
  struct Actor *first, *rock;
  struct EffectOptions options;
  struct Vector3 scatter;
  s32 i;
  s32 radius = 0x400000;
  s32 shakeA = -1;
  s32 shakeC = 0xe666;

  first = FieldActor_Create(255, x, 0, z);
  options.priority = 1;
  options.startScaleX = 0x20000;
  options.startScaleY = 0x20000;
  for (i = 0; i != 12; i++) {
    scatter.x = x + ((Random16() & 63) << 16) - 0x200000;
    scatter.y = (Random16() & 15) << 12;
    scatter.z = z + ((Random16() & 63) << 16) - 0x300000;
    if (i & 3)
      FieldEffect_Spawn(scatter.x, 0, scatter.z, 0, scatter.y, 0,
                        EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | 1,
                        &options);
    else
      FieldEffect_Spawn(scatter.x, 0, scatter.z, 0, scatter.y, 0,
                        EFFECT_USE_PRIORITY | 1, &options);
  }
  for (i = 0; i != 6; i++) {
    rock = first;
    if (i != 0) {
      rock = FieldActor_Create(255, x, 0, z);
      Sprite_ShareVramCacheEntry(rock->sprite, first->sprite);
    }
    Actor_SetSpriteFlags(rock, 0);
    Actor_StartScript(rock, gData_02003dfc);
    Actor_SelectAnimation(rock, 2);
    rock->velocityY = ((Random16() & 3) << 16) + 0x40000;
    rock->gravity = 0x3333;
    rock->bounce = 0x6666;
    rock->scaleX = 0xcccc;
    rock->scaleY = 0xcccc;
    rock->speedLimit = 0xcccc;
    scatter.y = Random16() & 7;
    scatter.x = Iwram_MulQ16(Trig_Cos(scatter.y << 12), radius);
    scatter.z = Iwram_MulQ16(Math_Sin(scatter.y << 12), radius);
    ((struct FieldSprite *)rock->sprite)->priority = 1;
    FieldActor_SetMoveTargetFixed(rock, rock->x + scatter.x, rock->y,
                                  rock->z + scatter.z);
  }
  MapGrid_SetShake(5, 7, 1);
  MapGrid_SetShakeFixed(shakeA, shakeA, shakeC);
}

// The pillar of light in Air's Rock, overlay 664's Func_02003654 with object
// 14 glowing: the camera moves to (0x218, 0xd8), the leader turns, and the
// light flashes twice (cue 138), each time copying cells into the map
// (MapGrid_CopyCellPictureIndicesRect at (52, 12) and (52, 22)) and lighting a
// spot (Func_02001dc4), then (cue 145) three more spots light and the cells at
// (62, 2) open; sets flag 0x87b.
void Func_02001f7c(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  /* FAKEMATCH(dead): BG0CNT's value in a once-only block of its own, built
   * before its address. */
  do {
    s32 control = BGCNT_SCREEN_BASE(4);

    REG_BG0CNT = control;
  } while (0);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                      buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(4), 0x800);
  buffer = Heap_Alloc(0x80);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_LightningMap),
                      buffer);
  i = 0;
  cell = buffer;
mask:
  *cell &= 0xfcff;
  i++;
  cell++;
  if (i != 64)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(BG_SCREEN_BLOCK(4) + (i << 6)), DMA_COPY_WORDS(2),
          REG_DMA3);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  Heap_Release(buffer);
  Event_PlaySound(SE_GLITTER);
  ((u8 *)Actor_Get(14))[98] = 9;
  ((u8 *)Actor_Get(14))[99] = 1;
  Actor_Get(14)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (FieldActor_Create(0x2a0, Actor_Get(14)->x, Actor_Get(14)->y,
                                 Actor_Get(14)->z));
      {
        s32 priority = ((s8 *)Actor_Get(14)->sprite)[9] & 12;

        /* FAKEMATCH(temporary): the sprite held in the row pointer. */
        cell = spark->sprite;
        ((struct SpriteAttr2High *)cell)->priority = priority >> 2;
      }
      ((struct ObjectPriorityByte *)spark)->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = px(3) / 2;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gData_02003d1c);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_Wait(1);
  }
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(60);
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(536, CAMERA_KEEP, 216, 1);
  FieldCamera_WaitForArrival();
  Actor_Get(14)->update = 0;
  FieldActor_SetSpriteColor(Actor_Get(14), 6);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = 0xff88;
  gBgScroll[0].y = 0x10;
  System_Wait(30);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  Actor_Get(gMainRoutineWork.currentOwner)->facing = FACING_SOUTH;
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellsTo(52, 12, 29, 9, 9, 9);
  MapGrid_CopyCellsTo(52, 76, 29, 73, 9, 9);
  Func_02001dc4(0x2280000, 0xe40000);
  i = 0;
fade1:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  System_Wait(1);
  i++;
  if (i != 32)
    goto fade1;
  gBgScroll[0].x = 0xffac;
  gBgScroll[0].y = 0;
  System_Wait(1);
  Event_PlaySound(SE_ITEM_BREAK);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellsTo(52, 22, 29, 9, 9, 9);
  MapGrid_CopyCellsTo(52, 86, 29, 73, 9, 9);
  Func_02001dc4(0x2080000, 0xf20000);
  i = 0;
fade2:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  System_Wait(1);
  i++;
  if (i != 32)
    goto fade2;
  Event_PlaySound(SE_CRASH);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  FieldEvent_Wait(20);
  Func_02001dc4(0x2000000, 0xc60000);
  Func_02001dc4(0x2320000, 0xf20000);
  Func_02001dc4(0x2090000, 0xdf0000);
  MapGrid_CopyCellsTo(62, 2, 29, 9, 9, 9);
  MapGrid_CopyCellAttributes(62, 2, 9, 9, 29, 9);
  MapGrid_CopyCellsTo(62, 66, 29, 73, 9, 9);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(60);
  System_Wait(60);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(60);
  System_Wait(70);
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                         gMainRoutineWork.paletteGlow[1]);
  REG_BLDCNT = 0;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  Event_SetFlag(0x87b);
  Event_End();
}

void Func_020024b0(void) {
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  s16 *line = gData_020046a0;
  s32 i;
  volatile u16 *channel;

  REG_BLDALPHA = gData_02004694 | 0x1000;
  if ((*Ram_FrameCount & 15) == 0) {
    gData_02004694 += gData_02004695;
    if (gData_02004694 == 8 || gData_02004694 == 4)
      gData_02004695 *= -1;
  }
  for (i = 0; i != 160; i++) {
    *line =
        (scroll[0] >> 16) +
        (Math_Sin((i + (u8)*Ram_FrameCount + (scroll[1] >> 16)) << 9) >> 13);
    line++;
  }
  line = gData_020046a0;
  REG_BG1HOFS = line[0];
  channel = REG_DMA0;
  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  channel[5];
  Dma_Set(line + 1, (void *)&REG_BG1HOFS, 0xa2600001, (volatile u32 *)channel);
}

void Func_0200257c(void) {
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  s16 *line = gData_020046a0;
  s32 i, wave, spread, value;
  s16 *table;
  volatile u16 *channel;

  for (i = 0; i != 160; i++) {
    spread = (160 - i) >> 2;
    wave = Math_Sin((i + (u8)gFrameCount) << 8);
    value = (Iwram_MulQ16(wave, (144 - i) << 10) >> 11) - 4;
    line[0] = value;
    line[0] += Random16() % (spread * 2) - spread;
    if (line[0] > 56)
      line[0] -= 56;
    else if (line[0] < -64)
      line[0] += 64;
    line[1] = 0;
    value = wave + (scroll[1] >> 16);
    line[2] = (scroll[0] >> 16) + (value >> 13);
    line += 3;
  }
  REG_BLDALPHA = gData_02004696 | 0x1000;
  if ((gFrameCount & 15) == 0) {
    gData_02004696 += gData_02004697;
    if (gData_02004696 == 8 || gData_02004696 == 4)
      gData_02004697 *= -1;
  }
  table = gData_020046a0;
  *(volatile u16 *)REG_BG0HOFS = table[0];
  table += 2;
  REG_BG1HOFS = table[0];
  channel = REG_DMA0;
  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  channel[5];
  Dma_Set(table + 1, REG_BG0HOFS, 0xa2600003, (volatile u32 *)channel);
}

void Func_020026b8(void) {
  u16 *buffer = Heap_Alloc(0x680);
  s32 *beam = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  u16 *cell;
  struct Actor *spark;
  s32 i;
  s32 z;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  REG_DISPCNT &= ~0x100;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                      buffer);
  Dma_Set(buffer, BG_SCREEN_BLOCK(29), DMA_COPY_WORDS(208), REG_DMA3);
  Dma_Set((u8 *)buffer + 0x340, (void *)0x600ec00, DMA_COPY_WORDS(208),
          REG_DMA3);
  Heap_Release(buffer);
  {
    s32 control = 0x40e;

    REG_BG0CNT = control;
  }
  // A once-only loop: its loop notes order the fill's address ahead of its
  // value.
  do {
    Dma_Fill16(0x10, (void *)0x6002000, 0x500);
  } while (0);
  buffer = Heap_Alloc(0x240);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap),
                      buffer);
  cell = buffer;
  for (i = 0; i != 0x120; i++) {
    *cell += 0xff40;
    cell++;
  }
  cell = buffer;
  for (i = 0; i != 18; i++) {
    Dma_Set(cell, (void *)(0x600200f + (i << 6)), DMA_COPY_WORDS(8), REG_DMA3);
    cell += 16;
  }
  Heap_Release(buffer);
  Event_PlaySound(SE_GLITTER);
  ((u8 *)Actor_Get(12))[98] = 9;
  ((u8 *)Actor_Get(12))[99] = 1;
  Actor_Get(12)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    s32 low = gFrameCount & 3;

    if (low == 0) {
      spark = (FieldActor_Create(0x2a0, Actor_Get(12)->x, Actor_Get(12)->y,
                                 Actor_Get(12)->z));
      {
        s32 priority = ((s8 *)Actor_Get(12)->sprite)[9] & 12;

        cell = spark->sprite;
        ((struct SpriteParts *)cell)->part = priority >> 2;
      }
      ((struct ObjBits *)spark)->flags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = px(3) / 2;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16() & 0xffff;
      Actor_SelectAnimation(spark, 7);
      Actor_StartScript(spark, gData_02003d1c);
      Actor_SetSpriteFlags(spark, 0);
      FieldActor_SetSpriteColor(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_Wait(1);
  }
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(152, CAMERA_KEEP, 144, 1);
  FieldCamera_WaitForArrival();
  FieldTask_Remove(Func_020024b0);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 0;
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(8);
  Task_AddFrame(Func_0200257c, 0xc80);
  Event_PlaySound(SE_163);
  REG_DISPCNT |= 0x100;
  MapGrid_SetShake(3, 3, 1);
  z = 0xffff8000;
  for (i = 0x4000; i != 0x48000; i += 0x400, z += 0xfffffc00) {
    beam[6] = i;
    beam[7] = z;
    ((u8 *)Actor_Get(12))[98] = Random16() & 7;
    System_Wait(1);
  }
  FieldTask_Remove(Func_0200257c);
  Task_AddFrame(Func_02000ef4, 0xc80);
  REG_BLDALPHA = 8;
  REG_BLDCNT = 0x3f42;
  for (i = 0; i != 257; i++) {
    REG_BLDALPHA = (8 - (i >> 5)) | 0x1000;
    ((u8 *)Actor_Get(12))[98] = Random16() & 7;
    FieldEvent_Wait(1);
  }
  REG_DISPCNT &= ~0x200;
  System_Wait(30);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  REG_BLDALPHA = 0x10;
  REG_BLDCNT = 0x3f41;
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  ((u8 *)Actor_Get(12))[98] = 6;
  for (i = 0; i != 129; i++) {
    REG_BLDALPHA = ((i >> 3) << 8) | (0x10 - (i >> 3));
    FieldEvent_Wait(1);
  }
  FieldTask_Remove(Func_02000ef4);
  {
    s32 zero = 0;

    ((u8 *)Actor_Get(12))[99] = zero;
    System_Wait(30);
    Actor_Get(12)->update = (void (*)(void))zero;
    FieldActor_SetSpriteColor(Actor_Get(12), 6);
    FieldMapGrid_WaitForShakeToSettle();
    FieldEvent_Wait(60);
    REG_BLDCNT = zero;
  }
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                         gMainRoutineWork.paletteGlow[1]);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags &= ~32;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_Get(12)->priorityFlags |= 1;
  EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  FieldCamera_WaitForArrival();
  Event_SetFlag(0x87a);
  Event_End();
}

extern s32 gData_02004698;

void Func_02002b88(void) {
  struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
  s32 level;

  actor->priorityFlags |= 32;
  level = ((actor->y + px(32)) >> 16) / 112;
  if (gData_02004698 != level) {
    switch (level) {
    case 3:
      MapGrid_CopyCellAttributes(64, 64, 21, 3, 27, 54);
      break;
    case 2:
      MapGrid_CopyCellAttributes(64, 67, 21, 3, 27, 54);
      break;
    case 1:
      MapGrid_CopyCellAttributes(64, 70, 21, 3, 27, 54);
      break;
    case 0:
      MapGrid_CopyCellAttributes(64, 73, 21, 3, 27, 54);
      break;
    }
    gData_02004698 = level;
  }
}

#include "shared/leader_lift.h"

#define LEADER() Actor_GetById(gMainRoutineWork.currentOwner)
#define BEAM ((s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178))

// An object that sways (Func_02001b54) from the given phase.
#define SWAY(id, phase)                                                        \
  (Actor_GetById(id)->timer = (phase),                                         \
   Actor_GetById(id)->update = Func_02001b54,                                  \
   Actor_SetSpriteFlags(Actor_GetById(id), 0))

// A step that hangs at the given height and sways.
#define HANG(id, height, phase)                                                \
  (Actor_GetById(id)->motionFlags = 0, Actor_GetById(id)->y = (height),        \
   SWAY(id, phase))

s32 Scene_Initialize(void) {
  struct Actor *leader;
  s32 *beam;
  volatile struct DmaRegisters *dma;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 516;
  gMainRoutineWork.retreatScene = (s32)&SceneId_AirsRock1;
  gMainRoutineWork.retreatEntrance = 1;
  LEADER()->priorityFlags |= 32;
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRock1) {
    switch (gMainRoutineWork.entrance) {
    case 2:
    case 3:
    case 4:
      Leader_Lift(0x580000);
    }
    if (GameFlag_Get(0x850)) {
      MapGrid_CopyCellsTo(64, 0, 113, 11, 3, 3);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 113, 12);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 49, 12);
    }
    if (GameFlag_Get(0x851)) {
      MapGrid_CopyCellsTo(64, 0, 70, 39, 1, 4);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 70, 40);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 6, 40);
    }
    if (GameFlag_Get(0x852)) {
      MapGrid_CopyCellsTo(64, 0, 103, 38, 1, 4);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 103, 39);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 39, 39);
    }
    if (GameFlag_Get(0x853)) {
      MapGrid_CopyCellsTo(64, 0, 87, 43, 1, 4);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 87, 44);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 23, 44);
    }
    if (GameFlag_Get(0x854)) {
      MapGrid_CopyCellsTo(64, 0, 70, 52, 1, 4);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 70, 52);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 6, 53);
    }
    if (GameFlag_Get(0x855)) {
      MapGrid_CopyCellsTo(64, 0, 79, 49, 3, 3);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 79, 50);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 15, 50);
    }
    if (GameFlag_Get(0x856)) {
      MapGrid_CopyCellsTo(64, 0, 103, 48, 1, 4);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 103, 49);
      MapGrid_CopyCellAttributes(64, 0, 1, 3, 39, 49);
    }
    if (GameFlag_Get(0x857)) {
      MapGrid_CopyCellsTo(64, 0, 111, 52, 3, 3);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 111, 53);
      MapGrid_CopyCellAttributes(64, 0, 3, 2, 47, 53);
    }
    if (GameFlag_Get(0x858)) {
      MapGrid_CopyCellsTo(68, 0, 79, 0, 11, 10);
      MapGrid_CopyCellAttributes(38, 9, 1, 1, 16, 9);
      MapGrid_CopyCellAttributes(38, 9, 1, 1, 24, 9);
      Actor_SetSpriteColor(Actor_GetById(8), 6);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRock2) {
    switch (gMainRoutineWork.entrance) {
    case 3:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z += px(16);
      Leader_Lift(0xffb80000);
      goto swaying;
    case 1:
    case 2:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z += px(16);
      Leader_Lift(0xffd80000);
      goto swaying;
    case 5:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        LEADER()->x += px(48);
        LEADER()->y += px(16);
      }
    case 6:
      Leader_Lift(0x380000);
    case 4:
    swaying:
      SWAY(8, 60);
      SWAY(9, 180);
      SWAY(10, 0);
      SWAY(11, 60);
      if (GameFlag_Get(0x85f))
        Actor_SetPosition(12, 264, 696);
      if (GameFlag_Get(0x860))
        Actor_SetPosition(13, 728, 728);
      if (GameFlag_Get(0x861))
        Actor_SetPosition(15, 344, 776);
      if (GameFlag_Get(0x862))
        Actor_SetPosition(16, 632, 792);
      Actor_PlaceBlocks(gData_02003d94);
      Blocks_MarkCells(gData_02003d94);
      FlagBlock_PlaceAll(gData_02003e4c);
      break;
    case 11:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        LEADER()->x += px(16);
        LEADER()->y += 0xfff00000;
      }
    case 10:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z += px(16);
      Leader_Lift(0xffc80000);
    case 12:
      if (GameFlag_Get(0x887))
        Actor_SetPosition(22, 792, 296);
      Actor_PlaceBlocks(gData_02003d94);
      Blocks_MarkCells(gData_02003d94);
      HANG(17, 0x100000, 60);
      HANG(19, 0x500000, 90);
      HANG(18, 0xffd00000, 100);
      HANG(20, 0x900000, 180);
      Actor_GetById(21)->motionFlags = 0;
      Actor_GetById(21)->y = 0xff700000;
      Actor_SetSpritePriority(21, 2);
      Actor_GetById(21)->priorityFlags |= 2;
      Actor_GetById(21)->timer = 180;
      ((u8 *)Actor_GetById(21))[98] = 1;
      Actor_GetById(21)->update = Func_02001b54;
      Actor_SetSpriteFlags(Actor_GetById(21), 0);
      break;
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_AirsRock3) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
      Task_AddFrame(Func_02002b88, 0x480);
      Actor_GetById(8)->motionFlags = 0;
      Actor_GetById(8)->y = 0xfff80000;
      Actor_SetSpritePriority(8, 2);
      Actor_GetById(8)->timer = 60;
      ((u8 *)Actor_GetById(8))[98] = 0;
      Actor_GetById(8)->update = Func_02001b54;
      Actor_SetSpriteFlags(Actor_GetById(8), 0);
      Actor_GetById(9)->motionFlags = 0;
      Actor_GetById(9)->y = 0xfff80000;
      Actor_SetSpritePriority(9, 2);
      Actor_GetById(9)->timer = 180;
      ((u8 *)Actor_GetById(9))[98] = 0;
      Actor_GetById(9)->update = Func_02001b54;
      Actor_SetSpriteFlags(Actor_GetById(9), 0);
      Actor_GetById(10)->terrainId = 2;
      Actor_SetSpritePriority(10, 1);
      Actor_GetById(10)->y = px(208);
      Actor_GetById(10)->terrainHeight = Actor_GetById(10)->y;
      Actor_GetById(11)->terrainId = 2;
      Actor_SetSpritePriority(11, 1);
      Actor_GetById(11)->y = px(208);
      Actor_GetById(11)->terrainHeight = Actor_GetById(11)->y;
      if (GameFlag_Get(FLAG_AREA_TEMP_10))
        Actor_SetPosition(10, 504, 872);
      if (GameFlag_Get(FLAG_AREA_TEMP_11))
        Actor_SetPosition(11, 520, 872);
      Func_02000620();
      beam = (s32 *)Ram_HeapSlots->mapWork;
      beam = (s32 *)((u8 *)beam + 0x178);
      beam[2] = 0x5800000;
      beam[3] = 0x2400000;
      beam[4] = 0x4000;
      beam[5] = 0x4000;
      Map_CenterOnFollowedPoint();
      System_Wait(1);
      break;
    case 6:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->y += px(24);
    case 7:
      Leader_Lift(0x480000);
    case 4:
    case 5:
      Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
      break;
    case 8:
    case 9:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z += px(16);
      Leader_Lift(0xffe80000);
      goto beam;
    case 10:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->x += px(48);
    case 11:
      Leader_Lift(0x900000);
    beam:
      REG_BG1CNT &= ~3;
      REG_BG1CNT |= 1;
      REG_BG2CNT &= ~3;
      REG_BG2CNT |= 2;
      REG_BG3CNT &= ~3;
      REG_BG3CNT |= 3;
      Actor_GetById(12)->priorityFlags |= 2;
      FieldEffect_StartBlankOverlay(0);
      if (GameFlag_Get(0x879))
        Actor_SetPosition(13, 184, 280);
      if (!GameFlag_Get(0x87a)) {
        ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
        REG_BLDALPHA = 0x1008;
        REG_BLDCNT = 0x3f42;
        dma = (volatile struct DmaRegisters *)0x040000b0;
        while ((s32)dma->control < 0)
          ;
        Task_AddFrame(Func_020024b0, 0xc80);
        beam = (s32 *)Ram_HeapSlots->mapWork;
        beam = (s32 *)((u8 *)beam + 0x178);
        beam[6] = 0x4000;
        beam[7] = 0xffff8000;
        {
          s16 *scale = (s16 *)beam;
          s32 value = 3;

          scale[20] = value;
          value = 0x180;
          scale[21] = value;
        }
      } else {
        REG_BLDALPHA = 0x1000;
        REG_BLDCNT = 0x3f42;
        REG_DISPCNT &= ~0x200;
        Actor_SetSpriteColor(Actor_GetById(12), 6);
      }
      break;
    case 12:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->y += 0xffe00000;
    case 13:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        LEADER()->z += px(16);
      Leader_Lift(0xffd00000);
    case 14:
      beam = (s32 *)Ram_HeapSlots->mapWork;
      beam = (s32 *)((u8 *)beam + 0x178);
      beam[2] = 0x4800000;
      beam[3] = 0x3200000;
      beam[4] = 0xd000;
      beam[5] = 0xd000;
      if (!GameFlag_Get(0x87b)) {
        MapGrid_CopyCellsTo(29, 9, 62, 2, 9, 9);
        MapGrid_CopyCellAttributes(29, 9, 9, 9, 62, 2);
        MapGrid_CopyCellsTo(29, 73, 62, 66, 9, 9);
        MapGrid_CopyCellsTo(52, 2, 29, 9, 9, 9);
        MapGrid_CopyCellAttributes(52, 2, 9, 9, 29, 9);
        MapGrid_CopyCellsTo(52, 66, 29, 73, 9, 9);
      } else {
        Actor_SetSpriteColor(Actor_GetById(14), 6);
      }
      Actor_SetSpritePriority(14, 1);
      Map_CenterOnFollowedPoint();
      System_Wait(1);
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        leader = LEADER();
        leader->terrainId = 0;
        leader->y = leader->terrainHeight =
            Map_GetTerrainHeight(0, leader->x, leader->z - leader->y);
        EventCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
      }
      break;
    case 99:
      MapGrid_CopyCellsTo(29, 9, 62, 2, 9, 9);
      MapGrid_CopyCellAttributes(29, 9, 9, 9, 62, 2);
      MapGrid_CopyCellsTo(29, 73, 62, 66, 9, 9);
      MapGrid_CopyCellsTo(52, 2, 29, 9, 9, 9);
      MapGrid_CopyCellAttributes(52, 2, 9, 9, 29, 9);
      MapGrid_CopyCellsTo(52, 66, 29, 73, 9, 9);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_02001f7c();
      break;
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)MsgCurePoison2) {
    Func_02002b88();
  }
  return 0;
}

#undef FIELD_AT
#undef WORD_AT
#undef WAVE_SCROLL
#undef WAVE_SCROLL_LINES
#undef LEADER
#undef BEAM
#undef SWAY
#undef HANG
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY

void Func_02000620(void);

// clang-format off
u32 gData_02003ce0[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003d1c[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003d58[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gData_02003d94[] = {
    12, 13, 15, 16, 22, -1,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gData_02003ce0,
    gData_02003d1c,
    gData_02003d58,
};

s32 gData_02003dac[] = {
    22,
    10,
    0x18000,
    39,
    2,
    46,
    (s32)Func_0200033c,
    17,
};

u32 gData_02003dcc[] = {
    39, 5, 0, 12, 38,
};

u32 gData_02003de0[] = {
    0, 60, 39, 5, 0, 12,
    38,
};

s32 gData_02003dfc[] = {
    0, 1, 23, 9, -1310, 23,
    10, -1310, 13, 32, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38,
};

s16 gData_02003e4c[] = {
    14, 512, -1, 0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gData_02003e84[] = {
    256, 96, 144, 272, 112, 160, -1, 2,
    608, 96, 144, 624, 112, 160, -1, 3,
    864, 96, 144, 880, 112, 160, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gData_02003ec4[] = {
    256, -192, 816, 272, -176, 832, -1, 1,
    608, -192, 816, 624, -176, 832, -1, 2,
    864, -192, 816, 880, -176, 832, -1, 3,
    624, 176, 704, 640, 192, 720, -1, 5,
    784, 160, 688, 800, 176, 704, -1, 6,
    624, -200, 320, 640, -184, 336, -1, 7,
    784, -216, 304, 800, -200, 320, -1, 8,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gData_02003f44[] = {
    144, -96, 304, 160, -80, 320, -1, 6,
    208, -96, 304, 224, -80, 320, -1, 7,
    144, -80, 304, 160, -64, 320, -1, 8,
    208, -80, 304, 224, -64, 320, -1, 9,
    144, 160, 192, 160, 176, 208, -1, 10,
    208, 160, 192, 224, 176, 208, -1, 11,
    144, 160, 192, 160, 176, 208, -1, 10,
    208, 160, 192, 224, 176, 208, -1, 11,
    528, -96, 304, 544, -80, 320, -1, 12,
    592, -96, 304, 608, -80, 320, -1, 13,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(AirsRock1),
    SCENE_EXIT_UNLESS(1, WorldMap, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRock2, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRock2, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRock2, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRock2),
    SCENE_EXIT_UNLESS(1, AirsRock1, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRock1, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRock1, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRockInside1, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AirsRock2, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AirsRock2, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AirsRock2, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AirsRock2, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AirsRock3, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(AirsRock3),
    SCENE_EXIT_UNLESS(1, AirsRock2, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AirsRock3, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AirsRock3, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AirsRock3, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AirsRock3, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AirsRock3, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AirsRock3, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AirsRock3, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AirsRock3, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AirsRock3, 8, 0x87a),
    SCENE_EXIT_UNLESS(11, AirsRock3, 9, 0x87a),
    SCENE_EXIT_UNLESS(10, AirsRock3, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AirsRock3, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AirsRock3, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, AirsRock3, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, AirsRockInside2, 1, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gAirsRock_Room1Placements[] = {
    ACTOR_PLACEMENT(382, CONDITION_ALWAYS, 7, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(325, CONDITION_ALWAYS, (s32)gData_02003dac, 920, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(325, CONDITION_ALWAYS, (s32)gData_02003dac, 104, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(325, CONDITION_ALWAYS, (s32)gData_02003dac, 376, 0, 808, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(325, CONDITION_ALWAYS, (s32)gData_02003dac, 632, 0, 888, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(325, CONDITION_ALWAYS, (s32)gData_02003dac, 872, 0, 856, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRock_Room2Placements[] = {
    {377, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_WEST, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 2},
    {290, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0x9, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0xa, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(377, CONDITION_ALWAYS, 7, 536, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(377, CONDITION_ALWAYS, 7, 680, 0, 296, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(377, CONDITION_ALWAYS, 7, 536, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(377, CONDITION_ALWAYS, 7, 648, 0, 296, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(377, CONDITION_ALWAYS, 7, 712, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    {272, CONDITION_ALWAYS, 1, 0xb, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAirsRock_Placements[] = {
    ACTOR_PLACEMENT(377, CONDITION_ALWAYS, 7, 536, 0, 904, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(377, CONDITION_ALWAYS, 7, 712, 0, 904, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 872, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 872, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(382, CONDITION_ALWAYS, 7, 152, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(382, CONDITION_ALWAYS, 7, 536, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gAirsRock_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXAMINE, 20, CONDITION_ALWAYS, EVENT(Func_02000548)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 8, 0x858, EVENT(Func_02000824)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x858, EVENT(Func_02000830)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, 0x858, EVENT(Func_02001010)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 9, CONDITION_ALWAYS, EVENT(Func_02000824)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 9, CONDITION_ALWAYS, EVENT(Func_02000830)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 10, CONDITION_ALWAYS, EVENT(Func_02000824)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, EVENT(Func_02000830)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 11, CONDITION_ALWAYS, EVENT(Func_02000824)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 11, CONDITION_ALWAYS, EVENT(Func_02000830)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 12, CONDITION_ALWAYS, EVENT(Func_02000824)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 12, CONDITION_ALWAYS, EVENT(Func_02000830)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 13, CONDITION_ALWAYS, EVENT(Func_02000824)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 13, CONDITION_ALWAYS, EVENT(Func_02000830)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRock_Room2Triggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(Func_02000568)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 9},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Func_02001ca4)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Func_02001cc8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12, 0x85f, EVENT(Func_02001ca4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x85f, EVENT(Func_02001cc8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13, 0x860, EVENT(Func_02001ca4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x860, EVENT(Func_02001cc8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, 0x861, EVENT(Func_02001ca4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x861, EVENT(Func_02001cc8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, 0x862, EVENT(Func_02001ca4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, 0x862, EVENT(Func_02001cc8)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 14, 0x200, EVENT(Func_02001da8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 22, 0x887, EVENT(Func_02001ca4)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 22, 0x887, EVENT(Func_02001cc8)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Func_02001560)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAirsRock_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 14},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, EVENT(Func_02000700)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Func_020005ec)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT(Func_02000620)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x879, EVENT(Func_020006d4)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 12, 0x87a, EVENT(Func_02000824)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 12, 0x87a, EVENT(Func_02000830)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, 0x87a, EVENT(Func_020026b8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_FIRST), 14, 0x87b, EVENT(Func_02000824)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST), 14, 0x87b, EVENT(Func_02000830)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, 0x87b, EVENT(Func_02001f7c)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(Func_02001560)},
    {TRIGGER_TABLE_END},
};

s16 gData_02004674[][4] = {
    {64, 0, 64, 0}, {64, 0, 64, 0}, {64, 0, 64, 0}, {64, 0, 64, 0},
};
// clang-format on

s8 gData_02004694 = 6;
s8 gData_02004695 = 1;

s8 gData_02004696 = 6;
s8 gData_02004697 = 1;

s32 gData_02004698 = -1;
