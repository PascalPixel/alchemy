/// @file
/// Scene code of overlay 0x67b, in ROM order.

// ---- games/tla/src/scene/aqua_rock.c ----

#include "../shared/actors.h"
#include "../shared/effects.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "event.h"
#include "ground_mask.h"
#include "module/scene/aqua_rock.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "task.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "../aqua_rock.h"

const u8 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67bRoom1)
    return gData_02002e30;
  if (scene == (s32)&SceneId_Ov67bRoom2)
    return gData_02002e60;
  return 0;
}

#include "actor.h"
#include "iwram_call.h"
#include "system/vector3.h"
#include "event_inline.h"

const u32 *Scene_GetExits(void) { return gSceneExits; }

s32 Func_02000384(struct Actor *object) {
  struct Vector3 spot;
  struct Vector3 unused; // Unused: gives the stack frame its 12 bytes.
  struct Vector3 *p;
  s32 x, coordinate, tries, maxDistanceSquared, distance, dx, dz;
  u32 a, b;

  tries = 0;
  maxDistanceSquared = 256;
again:
  tries++;
  if (tries <= 3) {
    p = &spot;
    p->x = object->x;
    p->y = object->y;
    p->z = object->z;
    distance = Iwram_MulQ16(Random16Far(), 0x80000) + 0x40000;
    a = Random16Far();
    b = Random16Far();
    Vector_AddPolarOffsetFar(distance, object->facing + (a >> 2) - (b >> 2), p);
    x = p->x;
    if (x < 0)
      x += 0xffff;
    dx = (x >> 16) - object->timer;
    coordinate = p->z;
    if (coordinate < 0)
      coordinate += 0xffff;
    dz = (coordinate >> 16) - (s16)object->unk66;
    if (dx * dx + dz * dz > maxDistanceSquared)
      goto again;
    goto found;
  }
  object->facing += 0x8000;
  ((struct ActorTail *)object)->unk5e = 1;
  goto end;
found:
  coordinate = p->y;
  Actor_SetMoveTarget(object, p->x, coordinate, p->z);
end:
  /* FAKEMATCH(temporary): the form kept; as return 0, 22 of 107 instructions
   * differ in tla-en. */
  x = 0;
  return x;
}

void Func_02000468(struct Actor *object) {
  struct FieldSprite *sprite;
  s32 angle;

  angle = object->facing;
  sprite = object->sprite;
  sprite->rotation = angle + 0x4000;
}

s32 Func_02000478(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  object->speedLimit = 0x10000;
  object->acceleration = 0xccc;
  object->collisionFlags = 0;
  object->update = (void (*)(void))Func_02000468;
  return 0;
}

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67bRoom1)
    return gData_02002f04;
  if (scene == (s32)&SceneId_Ov67bRoom2)
    return gData_0200312c;
  return gData_02002eec;
}

void Func_020004e4(struct Actor *object) {
  if (Map_GetTerrainHeight(0, object->x, object->z) > object->y)
    object->y += 0x20000;
  else
    object->z -= 0x20000;
  if (object->y >> 20 == 23)
    Func_02002928(object);
  if (object->z >> 20 == 48)
    Func_02002928(object);
}

void Func_0200052c(struct Actor *object) {
  if ((u32)Random16Far() < 0x1000) {
    struct Actor *child;
    s32 spread = Random16Far();
    s32 x = object->x;

    x += spread * 48;
    child = Import_ActorCreate(0x2a2, x - 0x180000, object->y, object->z);
    if (child != 0) {
      Actor_SetSingleChildFlags(child, 0);
      child->update = (void (*)(void))Func_020004e4;
      child->motionFlags = 0;
    }
  }
}

void Func_02000580(void) {
  struct Unk02003450 *entry;
  s32 n, i, flag;

  entry = Data_02003450;
  for (n = 0; n <= 4; n++, entry++) {
    flag = GameFlag_Test(entry->unk12 & 0xfff);
    if (entry->unk12 & 0x1000)
      flag ^= 1;
    if (entry->unk14 != flag) {
      entry->unk16[entry->unk1E] = 0;
      entry->unk1A[entry->unk1E] = flag;
      if (++entry->unk1E > 1)
        entry->unk1E = 0;
      entry->unk14 = flag;
    }
    for (i = 0; i < 2; i++) {
      s32 row = entry->unk16[i];
      if (row < entry->unk10 * 4) {
        s32 inverse;
        row /= 4;
        flag = entry->unk1A[i];
        inverse = flag ^ 1;
        if (entry->unk16[i] & 1) {
          MapGrid_CopyCellsLayered(flag * 3 + entry->unk00, entry->unk02 + row,
                                   3, 1, entry->unk08, entry->unk0A + row);
          MapGrid_CopyCells(flag * 3 + entry->unk04, entry->unk06 + row, 3, 1,
                            entry->unk0C, entry->unk0E + row);
        } else {
          MapGrid_CopyCellsLayered(inverse * 3 + entry->unk00,
                                   entry->unk02 + row, 3, 1, entry->unk08,
                                   entry->unk0A + row);
          MapGrid_CopyCells(inverse * 3 + entry->unk04, entry->unk06 + row, 3,
                            1, entry->unk0C, entry->unk0E + row);
        }
        entry->unk16[i]++;
      }
    }
  }
}

extern s32 gData_02003204, gData_02003208, gData_0200320c, gData_02003210;

/// Unless flag 0x104 is set, runs four 240-frame timers (gData_02003204 to
/// gData_02003210) that hold flags 0x200 to 0x203 set for their last 90
/// frames; in their first 150 the first two follow flags 0x320 and 0x321 and
/// the last two are clear.
void Func_02000740(void) {
  if (GameFlag_Test(0x104))
    return;
  if (gData_02003204 <= 149) {
    if (GameFlag_Test(0x320))
      GameFlag_SetBit(0x200);
    else
      GameFlag_ClearBit(0x200);
  } else {
    GameFlag_SetBit(0x200);
  }
  if (++gData_02003204 > 239)
    gData_02003204 = 0;
  if (gData_02003208 <= 149) {
    if (GameFlag_Test(0x321))
      GameFlag_SetBit(0x201);
    else
      GameFlag_ClearBit(0x201);
  } else {
    GameFlag_SetBit(0x201);
  }
  if (++gData_02003208 > 239)
    gData_02003208 = 0;
  if (gData_0200320c <= 149)
    GameFlag_ClearBit(0x202);
  else
    GameFlag_SetBit(0x202);
  if (++gData_0200320c > 239)
    gData_0200320c = 0;
  if (gData_02003210 <= 149)
    GameFlag_ClearBit(0x203);
  else
    GameFlag_SetBit(0x203);
  if (++gData_02003210 > 239)
    gData_02003210 = 0;
}

#include "task.h"

#define UNK02003450_SET(g, a, b, c, d, e, f, h, i, height, flag)               \
  (g)->unk00 = (a);                                                            \
  (g)->unk02 = (b);                                                            \
  (g)->unk04 = (c);                                                            \
  (g)->unk06 = (d);                                                            \
  (g)->unk08 = (e);                                                            \
  (g)->unk0A = (f);                                                            \
  (g)->unk0C = (h);                                                            \
  (g)->unk0E = (i);                                                            \
  (g)->unk10 = (height);                                                       \
  (g)->unk12 = (flag);                                                         \
  (g)->unk14 = GameFlag_Test(flag) ^ 1;                                        \
  (g)->unk1E = 0;                                                              \
  (g)->unk16[1] = 9999;                                                        \
  (g)->unk16[0] = 9999

void Func_02000844(void) {
  struct Unk02003450 *entry = Data_02003450;

  UNK02003450_SET(entry, 64, 74, 76, 74, 70, 10, 6, 74, 36, 0x200);
  entry++;
  UNK02003450_SET(entry, 70, 74, 82, 74, 78, 10, 14, 74, 36, 0x201);
  entry++;
  UNK02003450_SET(entry, 90, 93, 102, 93, 103, 26, 39, 90, 15, 0x202);
  entry++;
  UNK02003450_SET(entry, 96, 93, 108, 93, 107, 26, 43, 90, 15, 0x203);
  entry++;
  UNK02003450_SET(entry, 116, 95, 122, 95, 96, 32, 32, 96, 13, 0x1a6b);
  if (GameFlag_Test(0x320))
    GameFlag_SetBit(0x200);
  else
    GameFlag_ClearBit(0x200);
  if (GameFlag_Test(0x321))
    GameFlag_SetBit(0x201);
  else
    GameFlag_ClearBit(0x201);
  /* FAKEMATCH(temporary): each copy's five varying arguments held in locals
   * of their own scope; as direct calls with the arguments written in,
   * 188 of 307 instructions differ in tla-en. */
  if (GameFlag_Test(0x200)) {
    struct Unk02003450 *opened = &Data_02003450[0];
    {
      s32 srcX = opened->unk00 + 3, srcY = opened->unk02,
          height = opened->unk10, destX = opened->unk08, destY = opened->unk0A;
      Func_0802b450(srcX, srcY, 3, height, destX, destY);
    }
    {
      s32 srcX = opened->unk04 + 3, srcY = opened->unk06,
          height = opened->unk10, destX = opened->unk0C, destY = opened->unk0E;
      Map_CopyCellsRect(srcX, srcY, 3, height, destX, destY);
    }
    opened->unk16[1] = 9999;
    opened->unk16[0] = 9999;
    opened->unk14 = 1;
  }
  if (GameFlag_Test(0x201)) {
    struct Unk02003450 *opened = &Data_02003450[1];
    {
      s32 srcX = opened->unk00 + 3, srcY = opened->unk02,
          height = opened->unk10, destX = opened->unk08, destY = opened->unk0A;
      Func_0802b450(srcX, srcY, 3, height, destX, destY);
    }
    {
      s32 srcX = opened->unk04 + 3, srcY = opened->unk06,
          height = opened->unk10, destX = opened->unk0C, destY = opened->unk0E;
      Map_CopyCellsRect(srcX, srcY, 3, height, destX, destY);
    }
    opened->unk16[1] = 9999;
    opened->unk16[0] = 9999;
    opened->unk14 = 1;
  }
  Task_AddFrame(Func_02000740, TASK_PASS_VBLANK);
  Task_Add(Func_02000580, TASK_PASS_VBLANK);
}

DEFINE_ACTOR_FLICKER_CHILD(Func_02000b10, 7)

DEFINE_ACTOR_FLICKER_CHILD(Func_02000b34, 6)

void Func_02000b58(struct Actor *object) {
  Animation_ApplyChildValues(object, 0);
  object->update = 0;
}

void Func_02002948(s32 layer);

/// Toggles flag 0xa68 and with it which of two layer pairs is shown: four
/// rounds of a random colour value, then one pair's blend scripts and layer
/// fields set and the other's cleared, while the object blinks.
void Func_02000b68(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  object->update = (void (*)(void))Func_02000b10;
  wait(30);
  if (GameFlag_Test(0xa68) == 0)
    Func_02002834(26);
  else
    Func_02002834(0);
  for (i = 3; i >= 0; i--) {
    BattleEffect_ApplyColorToTargetBuffer(Random16Far() & 0x7fff, 0);
    BattleEffect_StartBufferInterpolation(15);
    wait(10);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    BattleEffect_StartBufferInterpolation(10);
    wait(10);
  }
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(20);
  Event_Wait(20);
  if (Event_GetFlag(0xa68) == 0) {
    GameFlag_SetBit(0xa68);
    Func_02002958_(64, 64, 1, 2, 51, 82);
    Func_02002958_(66, 64, 1, 2, 115, 18);
    Func_02002948(5);
    Func_02002948(6);
    Func_02002940(7);
    Func_02002940(8);
    object->update = (void (*)(void))Func_02000b58;
    Func_020025a8(26);
  } else {
    GameFlag_ClearBit(0xa68);
    Func_02002958_(65, 64, 1, 2, 51, 82);
    Func_02002958_(67, 64, 1, 2, 115, 18);
    Func_02002948(7);
    Func_02002948(8);
    Func_02002940(5);
    Func_02002940(6);
    object->update = (void (*)(void))Func_02000b58;
    Func_020025a8(0);
  }
  Event_Wait(30);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(30);
  Event_Wait(15);
  Event_End();
}

void GameFlag_SetByte(s32 flag, s32 value);

void Func_02000cd8(s32 unused, s32 id) {

  if (id == 24) {
    s32 column;
    s32 row;

    struct Actor *first = Actor_GetById(24);

    column = first->x >> 20;
    row = first->z >> 20;
    GameFlag_SetByte(0x310, column);
    GameFlag_SetByte(0x318, row);
    GameFlag_ClearBit_(0x320);
    GameFlag_ClearBit(0x321);
    if (column == 7 && row == 35)
      GameFlag_SetBit(0x320);
    if (column == 15 && row == 35)
      GameFlag_SetBit(0x321);
  } else if (id == 25) {
    struct Actor *object = Actor_GetById(25);

    if (object->x >> 20 == 32) {
      wait(30);
      object->z += 0x10000;
      GameFlag_SetBit(0x950);
    }
  }
}

void Func_02000d64(struct Actor *object, s32 limit) {
  s32 frames = 60;

  while (frames != 0) {
    wait(1);
    frames--;
    if (object->y <= limit)
      break;
  }
}

#include "main_routine_work.h"
#include "ram_buffer.h"
#include "frame.h"

/* VIEW(aqua_rock): routine Func_02002970 as s32 Func_02002970(struct Actor *object, struct Vector3 *position); module/scene/lemurian_ship.h has void Func_02002970(void); tried as the definition's void Func_02002970(void), which the object already declares, Func_02001e00 moves: 4 of 151 instructions in tla-ja. */
extern s32 Func_02002970__aqua_rock(struct Actor *object, struct Vector3 *position) __asm__("Func_02002970");

void Func_02000d80(void) {
  struct Actor *leader, *carried;
  struct Vector3 ahead;
  s32 owner;
  struct EventWork1cc *work;
  s32 result, i;

  if (GameFlag_Test(0xa68)) {
    work = Ram_HeapSlots->eventWork;
    owner = gMainRoutineWork.currentOwner;
    carried = work->object;
    leader = Actor_GetById(owner);
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov67bRoom1 &&
        leader->terrainHeight == 0)
      return;
    ahead.x = leader->x;
    ahead.y = leader->y;
    ahead.z = leader->z + 0x20000;
    result = Func_02002970__aqua_rock(leader, &ahead);
    if ((gFrameCount & 4) == 0)
      Func_02001d30(leader);
    if (result < 0) {
      Actor_SetAttachedEffect(owner, 258);
      Actor_SetMoveTarget(leader, leader->x, leader->y, leader->z - 0x80000);
      Func_02002910(leader, 7);
      Script_WaitForEventTimeout(leader);
      do
        wait(1);
      while (leader->y != leader->terrainHeight);
      Func_02001d30(leader);
      Func_02002910(leader, 6);
      wait(3);
      return;
    }
    ahead.x = leader->x;
    ahead.y = leader->y;
    ahead.z = leader->z - 0x80000;
    result = Func_02002970__aqua_rock(leader, &ahead);
    if (result > 0) {
      if (gMainRoutineWork.scene != (s32)&SceneId_Ov67bRoom1)
        return;
      Event_Start();
      Event_ResolvePendingActions(0);
      leader->motionFlags = 0;
      Actor_SetSingleChildFlags(leader, 0);
      gMainRoutineWork.renderMode = 2;
      Func_02001d30(leader);
      Event_Wait(5);
      for (i = 59; i >= 0; i--) {
        leader->y += 0x20000;
        leader->facing += 0x1000;
        if (Random16Far() <= 0xfff)
          Func_02001d30(leader);
        wait(1);
      }
      gMainRoutineWork.renderMode = 0;
      Event_End();
      Import_EventSetExitRequest(6);
      return;
    }
    ahead.x = leader->x + 0x5b333;
    ahead.y = leader->y;
    ahead.z = leader->z - 0x5b333;
    result = Func_02002970__aqua_rock(leader, &ahead);
    if (result > 0)
      return;
    ahead.x = leader->x - 0x5b333;
    ahead.y = leader->y;
    ahead.z = leader->z - 0x5b333;
    result = Func_02002970__aqua_rock(leader, &ahead);
    if (result > 0)
      return;
    carried->z += -0x20000;
    leader->z += -0x20000;
  } else
    Func_02001e00();
}

#include "map.h"
#include "input.h"

static inline u8 CellAttribute(struct MapCell *cells, s32 row, s32 col) {
  cells += row * 128 + col;
  return cells->attributeB;
}

void Func_02000f8c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct AquaRockEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 col;
  s32 row;

  if (gMainRoutineWork.renderMode == 2) {
    col = object->x >> 20;
    row = (object->z - object->y - 0xc0000) >> 20;
    if (CellAttribute(map->layers[1].cells, row, col) != 0)
      work->field16a = 44;
  } else {
    col = object->x >> 20;
    row = (object->z - object->y) >> 20;
    if (CellAttribute(map->layers[1].cells, row, col) != 0)
      Func_02001e00();
  }
}

void Func_02001010(void) {
  Func_02002960_(62, 41, 1, 2, 33, 41);
  Func_0802b450(125, 32, 3, 13, 96, 32);
  Func_0802b450(61, 96, 3, 13, 32, 96);
  Actor_SetPartAttribute(Actor_GetById(12), 5);
  Func_02001f60(12);
  GameFlag_SetBit(0xa6b);
}

void Func_0200106c(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 saved = object->gravity;
  s32 i;
  struct MainRoutineWork *state;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner, 257);
  Func_02002910(object, 40);
  object->motionFlags = 3;
  for (i = 0; i < 180; i++) {
    wait(1);
    if (object->y <= object->terrainHeight)
      break;
  }
  state = &gMainRoutineWork;
  object->gravity = saved;
  FieldEffect_SetAttached(state->currentOwner, 0);
  Actor_SetSingleChildFlags(object, 1);
  Func_02002910(object, 1);
  state->renderMode = 0;
  Event_End();
}

/// 3 copies: aqua_rock/aqua_rock, aqua_rock/aqua_rock, aqua_rock/aqua_rock.
#define DEFINE_FUNC_020010F4(name, n1, n2, n3, n4)                             \
  void name(s32 unused, s32 id) {                                              \
    struct Actor *object = Actor_GetById(id);                                  \
                                                                               \
    object->priorityFlags = 0;                                                 \
    object->motionFlags = 0;                                                   \
    Func_02002960_(n1, n2, 1, 1, n3, n2);                                      \
    GameFlag_SetBit(n4);                                                       \
  }

DEFINE_FUNC_020010F4(Func_020010f4, 42, 32, 44, 0x221)

DEFINE_FUNC_020010F4(Func_0200112c, 48, 32, 46, 0x222)

DEFINE_FUNC_020010F4(Func_02001164, 44, 44, 46, 0x223)

/// 2 copies: aqua_rock/aqua_rock, aqua_rock/aqua_rock.
#define DEFINE_FUNC_0200119C(name, n1, n2)                                     \
  void name(s32 unused, s32 id) {                                              \
    struct Actor *object = Actor_GetById(id);                                  \
    s32 column = object->x >> 20;                                              \
                                                                               \
    if (column == 53) {                                                        \
      wait(30);                                                                \
      object->z += 0x10000;                                                    \
      GameFlag_SetBit(n1);                                                     \
      Map_CopyCellAttributeRect(54, 58, 1, 1, column, n2);                     \
    }                                                                          \
  }

DEFINE_FUNC_0200119C(Func_0200119c, 0x951, 52)

DEFINE_FUNC_0200119C(Func_020011e0, 0x952, 58)

/// Sets flag 0xa69 after four rounds of a random colour value, runs the
/// blend scripts, then moves the camera to object 15, starts its update and
/// plays cue 623 six times.
void Func_02001224(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  object->update = (void (*)(void))Func_02000b10;
  wait(30);
  Func_02002834(8);
  Sound_Play(220);
  for (i = 3; i >= 0; i--) {
    BattleEffect_ApplyColorToTargetBuffer(Random16Far() & 0x7fff, 0);
    BattleEffect_StartBufferInterpolation(15);
    wait(10);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    BattleEffect_StartBufferInterpolation(10);
    wait(10);
  }
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(20);
  Event_Wait(20);
  GameFlag_SetBit(0xa69);
  Map_CopyCellAttributeRect(15, 48, 1, 1, 16, 48);
  Map_CopyCellAttributeRect(17, 50, 1, 1, 16, 50);
  object->update = (void (*)(void))Func_02000b58;
  Func_020025a8(8);
  Func_0802b450(64, 68, 5, 13, 14, 94);
  Map_CopyCellAttributeRect(64, 68, 5, 8, 14, 34);
  Event_Wait(30);
  Func_02002a20_(0x10000, 0);
  BattleEffect_StartBufferInterpolation(30);
  Event_Wait(15);
  Camera_SetSpeedParameters(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(PIXELS(248), -PIXELS(32), PIXELS(760), 1);
  Func_02002a10();
  Actor_SetHorizontalPositionWithTerrain(15, PIXELS(264), PIXELS(792));
  Actor_SetSingleChildFlags(Actor_GetById(15), 0);
  Actor_GetById(15)->collisionFlags = 0;
  Actor_GetById(15)->priorityFlags |= 2;
  Actor_GetById(15)->update = (void (*)(void))Actor_EmitWaterDrop;
  Event_Wait(30);
  for (i = 5; i >= 0; i--) {
    Sound_Play(623);
    wait(10);
  }
  Event_Wait(30);
  Event_End();
}

const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67bRoom1)
    return gData_02003220;
  if (scene == (s32)&SceneId_Ov67bRoom2)
    return gData_020032d4;
  return gData_02003214;
}

void Func_020013f8(void) {
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());

  if (object != 0 && (object->animationKind & 15) == 1) {
    struct FieldSprite *sprite;
    s32 priority;

    sprite = object->sprite;
    priority = gMainRoutineWork.renderMode == 2 ? 2 : 1;
    sprite->priority = priority;
    sprite->partPriority = priority;
    object->priorityFlags &= 0xfe;
  }
}

void Func_02001460(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 saved = object->motionFlags;
  s32 i;
  s32 angle;
  s32 up;
  struct MainRoutineWork *state;

  Sound_Play(612);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->y = -0x100000;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  for (i = 15; i >= 0; i--) {
    object->y += 0x10000;
    angle = object->facing;
    angle += 0x2000;
    object->facing = angle;
    wait(1);
  }
  Func_02001d30(object);
  object->motionFlags = saved;
  object->velocityY = 0x80000;
  Actor_SetSingleChildFlags(object, 1);
  up = gInput.held & 0x40;
  state = &gMainRoutineWork;
  if (up)
    Actor_SetPositionAndCommit(state->currentOwner, 264, 760);
  else
    Actor_SetPositionAndCommit(state->currentOwner, 264, 824);
  Event_End();
}

void Func_0200153c(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 saved = object->motionFlags;
  s32 i;
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->y = -0x400000;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  for (i = 23; i >= 0; i--) {
    object->y += 0x20000;
    angle = object->facing;
    angle += 0x2000;
    object->facing = angle;
    wait(1);
  }
  Func_02001d30(object);
  object->motionFlags = saved;
  object->velocityY = 0xc0000;
  Actor_SetSingleChildFlags(object, 1);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 680, 912);
  Event_End();
}

void Func_020015e8(struct Actor *object) {
  struct FieldSprite *sprite = object->sprite;
  s16 *amplitude;

  object->timer += 0x2000;
  amplitude = (s16 *)&object->unk66;
  *amplitude += 32;
  if (*amplitude > 0x200)
    *amplitude = 0x200;
  sprite->rotation = Iwram_MulQ16(Math_Sine(object->timer), *amplitude);
}

void Func_02001630(struct Actor *object) {
  struct FieldSprite *sprite = object->sprite;
  s16 *amplitude;

  object->timer += 0x2000;
  amplitude = (s16 *)&object->unk66;
  *amplitude -= 16;
  if (*amplitude < 0) {
    *amplitude = 0;
  }
  sprite->rotation = Iwram_MulQ16(Math_Sine(object->timer), *amplitude);
}

void Func_02001670(struct Actor *object) {
  struct FieldSprite *sprite = object->sprite;

  object->timer += 0x100;
  sprite->rotation = Math_Sine(object->timer) / 64 + 0x600;
}

/// Scatters objects 17 to 21 under cue 184, lines them up 32 pixels apart
/// at z 824, then settles them under cue 298 and leaves object 21 rocking;
/// sets flags 0x109 and 0x955 and clears 0x12f.
void Func_0200169c(void) {
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner, 0, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(184);
  // Two goto loops: the ROM loads the zero and the update address on each pass,
  // which loop.c hoists out of for loops (differ 18).
  i = 17;
scatter:
  {
    struct Actor *object = Actor_GetById(i);

    object->speedLimit = 0x4000;
    object->acceleration = 0x400;
    object->update = (void (*)(void))Func_020015e8;
    object->timer = Random16Far();
    object->unk66 = 0;
  }
  if (++i <= 21)
    goto scatter;
  Event_Wait(60);
  Map_CopyCellsRect(64, 69, 11, 4, 87, 53);
  Actor_ResetAndSetPosition_(17, 392, 824);
  Actor_ResetAndSetPosition_(18, 424, 824);
  Actor_ResetAndSetPosition_(19, 456, 824);
  Actor_ResetAndSetPosition_(20, 488, 824);
  Actor_ResetAndSetPosition(21, 520, 824);
  Event_Wait(240);
  Map_CopyCellsRect(64, 66, 11, 7, 87, 50);
  i = 17;
settle:
  Actor_GetById(i)->update = (void (*)(void))Func_02001630;
  if (++i <= 21)
    goto settle;
  Sound_Play(298);
  Event_Wait(60);
  {
    struct Actor *object = Actor_GetById(21);

    object->update = (void (*)(void))Func_02001670;
    object->timer = 0;
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x109);
  GameFlag_ClearBit(0x12f);
  Import_EventSetExitRequest(77);
  Event_End();
  GameFlag_SetBit(0x955);
}
#undef UNK02003450_SET

// ---- games/tla/src/scene/aqua_rock/s17f4.c ----
// Scene code of overlay 0x67b, in ROM order, from s17f4's offset up to the next object the overlay links.

#include "call.h"

void Func_02002948(s32 a);

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  struct MapBits *map = (struct MapBits *)Ram_HeapSlots->mapWork;
  s32 id, x, z;
  s16 *entrance;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  gMainRoutineWork.retreatScene = (s32)&SceneId_Ov67bRoom1;
  gMainRoutineWork.retreatEntrance = 1;
  map->mode = 1;
  Func_02002a58(170);
  if (scene == (s32)&SceneId_Ov67bRoom1) {
    Actor_SetPartAttribute(Actor_GetById(8), 4);
    Actor_SetPartAttribute(Actor_GetById(9), 4);
    Actor_SetPartAttribute(Actor_GetById(10), 4);
    Actor_SetPartAttribute(Actor_GetById(11), 4);
    if (GameFlag_Test(0xa6b))
      Actor_SetPartAttribute(Actor_GetById(12), 5);
    else
      Actor_SetPartAttribute(Actor_GetById(12), 6);
    Func_02001f60(8);
    Func_02001f60(9);
    Func_02001f60(10);
    Func_02001f60(11);
    Func_02001f60(13);
    Func_02001f60(14);
    for (id = 15; id <= 23; id++)
      Func_02001f60(id);
    for (id = 8; id <= 14; id++)
      Actor_SetSpritePriority(id, 3);
    for (id = 15; id <= 23; id++)
      Actor_SetSpritePriority(id, 1);
    Actor_GetById(15)->unk5c[0] = 2;
    Actor_SetModeById(15, 3);
    for (id = 16; id <= 23; id++) {
      Actor_SetModeById(id, 1);
      Actor_SetModeById(id, 0);
    }
    if (GameFlag_Test(0x950))
      Call3(Actor_SetHorizontalPositionWithTerrain, 25, PIXELS(520),
            PIXELS(584));
    x = GameFlag_GetByte(0x310);
    z = GameFlag_GetByte(0x318);
    if (z != 0)
      Actor_SetHorizontalPositionWithTerrain(24, (x << 20) + 0x80000,
                                             (z << 20) + 0x80000);
    Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 32;
    Task_Add(Func_020013f8, 0x480);
    Actor_SetSpritePriority(28, 1);
    Value2((s32 (*)())Actor_SetSpritePriority, 24, 1);
    Func_02002550();
    Func_0200276c();
    if (GameFlag_Test(0xa68)) {
      Func_020025a8(26);
      Func_02002834(26);
      Func_02002948(5);
      Func_02002948(6);
      Func_02002940(7);
      Func_02002940(8);
      Actor_GetById(26)->update = Func_02000b58;
    }
    Actor_SetSpritePriority(26, 1);
    if (gMainRoutineWork.entrance == 5)
      Func_0200169c();
    if (GameFlag_Test(0x955)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 17, PIXELS(392),
            PIXELS(824));
      Call3(Actor_SetHorizontalPositionWithTerrain, 18, PIXELS(424),
            PIXELS(824));
      Call3(Actor_SetHorizontalPositionWithTerrain, 19, PIXELS(456),
            PIXELS(824));
      Call3(Actor_SetHorizontalPositionWithTerrain, 20, PIXELS(488),
            PIXELS(824));
      Call3(Actor_SetHorizontalPositionWithTerrain, 21, PIXELS(520),
            PIXELS(824));
      Call6(Map_CopyCellAttributeRect, 64, 66, 9, 3, 24, 53);
      Call6(Map_CopyCellsRect, 64, 66, 11, 7, 87, 50);
      Actor_GetById(21)->update = Func_02001670;
    }
    if (GameFlag_Test(0xa6b)) {
      Call6(Map_CopyCellAttributeRect, 62, 41, 1, 2, 33, 41);
      Func_0802b450(116, 95, 3, 13, 0x60, 0x20);
      Call6(Func_0802b450, 122, 95, 3, 13, 0x20, 0x60);
      Func_02001f60(12);
    }
    Func_02000844();
    if (gMainRoutineWork.entrance == 4)
      Func_02002158();
  } else if (scene == (s32)&SceneId_Ov67bRoom2) {
    Actor_SetSingleChildFlags(Actor_GetById(14), 0);
    if (GameFlag_Test(545))
      Call6(Map_CopyCellAttributeRect, 42, 32, 1, 1, 44, 32);
    if (GameFlag_Test(546))
      Call6(Map_CopyCellAttributeRect, 48, 32, 1, 1, 46, 32);
    if (GameFlag_Test(547))
      Call6(Map_CopyCellAttributeRect, 44, 44, 1, 1, 46, 44);
    Actor_SetModeById(12, 2);
    if (GameFlag_Test(0x951)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 12, PIXELS(856),
            PIXELS(840));
      Call6(Map_CopyCellAttributeRect, 54, 58, 1, 1, 53, 52);
    }
    Actor_SetModeById(13, 2);
    if (GameFlag_Test(0x952)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 13, PIXELS(856),
            PIXELS(936));
      Call6(Map_CopyCellAttributeRect, 54, 58, 1, 1, 53, 58);
    }
    if (gMainRoutineWork.entrance == 5 || gMainRoutineWork.entrance == 7) {
      Func_02002550();
      Func_0200276c();
      if (GameFlag_Test(0xa68)) {
        Func_020025a8(14);
        Func_02002834(14);
      } else {
        Func_02002948(7);
        Func_02002948(8);
        Func_02002940(5);
        Func_02002940(6);
      }
      Actor_SetSingleChildFlags(Actor_GetById(14), 0);
    } else {
      Func_02002550();
      Func_0200276c();
      Actor_GetById(8)->update = Func_02000b58;
      if (GameFlag_Test(0xa69)) {
        Func_020025a8(8);
        Func_02002834(8);
        Actor_SetHorizontalPositionWithTerrain(15, PIXELS(264), PIXELS(792));
        Actor_SetSingleChildFlags(Actor_GetById(15), 0);
        Actor_GetById(15)->collisionFlags = 0;
        Actor_GetById(15)->priorityFlags |= 2;
        Actor_GetById(15)->update = Actor_EmitWaterDrop;
        Map_CopyCellAttributeRect(15, 48, 1, 1, 16, 48);
        Map_CopyCellAttributeRect(17, 50, 1, 1, 16, 50);
        Func_0802b450(0x40, 68, 5, 13, 14, 94);
        Map_CopyCellAttributeRect(64, 68, 5, 8, 14, 34);
      }
    }
    if (!GameFlag_Test(265)) {
      if (gMainRoutineWork.entrance == 1)
        Func_02001460();
      entrance = &gMainRoutineWork.entrance;
      if (*entrance == 6)
        Func_0200153c();
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02001d30(struct Actor *object) {
  struct Actor *child = Import_ActorCreate(
      14, object->x, object->y + 0x40000, object->z + 0x40000);
  struct FieldSprite *parentSprite = object->sprite;

  if (child != 0) {
    struct FieldSprite *sprite = child->sprite;

    child->terrainHeight = object->terrainHeight;
    Actor_StartScript(child, gData_02002c44);
    child->motionFlags = 0;
    if (sprite != 0) {
      Animation_ApplyChildArgument(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = parentSprite->priority;
    }
  }
}

// ---- games/tla/src/scene/aqua_rock/events_g.c ----
DEFINE_ACTOR_EMIT_WATER_DROP(Actor_EmitWaterDrop, gData_02002c50)

#include "scene_ids.h"

void Func_02001e00(void) {
  struct Actor *anchor;
  s32 owner;
  struct Actor *leader;
  s32 position[3];
  s32 result;

  {
    struct EventWork1cc *work = Ram_HeapSlots->eventWork;

    owner = gMainRoutineWork.currentOwner;
    anchor = work->object;
  }
  leader = Actor_GetById(owner);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67bRoom1 &&
      leader->terrainHeight == 0)
    return;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z + 0x20000;
  result = Func_02002970__aqua_rock(leader, position);
  if ((gFrameCount & 4) == 0)
    Func_02001d30(leader);
  if (result < 0) {
    Actor_SetAttachedEffect(owner, 0x102);
    Actor_SetMoveTarget(leader, leader->x, leader->y, leader->z + 0x80000);
    Func_02002910(leader, 49);
    Script_WaitForEventTimeout(leader);
    do {
      wait(1);
    } while (leader->y != leader->terrainHeight);
    Func_02001d30(leader);
    Func_02002910(leader, 49);
    wait(3);
    return;
  }
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z + 0x80000;
  result = Func_02002970__aqua_rock(leader, position);
  if (result > 0)
    return;
  position[0] = leader->x + 0x5b333;
  position[1] = leader->y;
  position[2] = leader->z + 0x5b333;
  result = Func_02002970__aqua_rock(leader, position);
  if (result > 0)
    return;
  position[0] = leader->x - 0x5b333;
  position[1] = leader->y;
  position[2] = leader->z + 0x5b333;
  result = Func_02002970__aqua_rock(leader, position);
  if (result > 0)
    return;
  anchor->z += 0x20000;
  leader->z += 0x20000;
  leader->facing = 0x4000;
}

void Func_02001f60(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->sprite, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Func_02002910(object, 0);
    Func_02002910(object, 2);
  }
}

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags,
                   s32 *params);

void Func_02001f90(s32 arg0) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 i;
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  object->motionFlags = 0;
  object->priorityFlags |= 0x20;
  Sound_Play(215);
  Func_02002910(object, 18);
  Sound_Play(612);
  for (i = 0; i < 60; i++) {
    if (i == 30)
      Event_FinishScreenTransition();
    object->y -= 0x4000;
    angle = object->facing;
    angle += 0x1000;
    object->facing = angle;
    if ((i & 7) == 0)
      Func_02001d30(Actor_GetById(15));
    wait(1);
  }
  Event_End();
  Import_EventSetExitRequest(arg0);
}

/// Every other frame, emits particle 7 (5 on every other such frame) from the
/// object, 26 pixels up, swaying with the frame count and drifting by a random
/// amount.
DEFINE_ACTOR_EMIT_MIST(Actor_EmitMist)

void Func_020020bc(s32 entrance) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 *owner = &gMainRoutineWork.currentOwner;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(228);
  leader->update = (void (*)(void))Actor_EmitMist;
  leader->motionFlags = 0;
  leader->speedLimit = 0x3333;
  Actor_SetModeById(*owner, 2);
  Actor_OffsetPositionAndResetMotion(*owner, 0, -8);
  Actor_WaitForMoveById(*owner);
  Actor_SetPartAttribute(Actor_GetById(*owner), 9);
  Actor_SetSingleChildFlags(Actor_GetById(*owner), 0);
  leader->update = 0;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(entrance);
  Event_End();
}

void Func_02002158(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 flag = GameFlag_Test(0x109);
  s32 *owner = &gMainRoutineWork.currentOwner;
  s32 offset;

  if (flag == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(-1, -1, -1, 0);
    leader->motionFlags = flag;
    Actor_SetHorizontalPositionWithTerrain(*owner, ((s16 *)&leader->x)[1] << 16,
                                           (((s16 *)&leader->z)[1] << 16) -
                                               0x100000);
    Actor_SetPartAttribute(Actor_GetById(*owner), 9);
    Actor_SetSingleChildFlags(Actor_GetById(*owner), 0);
    Event_StartScreenTransition();
    Sound_Play(228);
    leader->update = (void (*)(void))Actor_EmitMist;
    Actor_SetSpeedParameters(*owner, 0x6666, 0x3333);
    Actor_CommitPositionAndActivate(*owner, 0, 8);
    Actor_SetPartAttribute(Actor_GetById(*owner), 0);
    Actor_SetSingleChildFlags(Actor_GetById(*owner), 1);
    ((struct FieldSprite *)leader->sprite)->priority = 1;
    offset = 10;
    Actor_CommitPositionAndActivate(*owner, 0, offset);
    leader->motionFlags = 3;
    leader->update = (void (*)(void))flag;
    Event_NoOp();
    Event_WaitTransitionFrames();
    Event_End();
  }
}

#include "graphics/vram_tab.h"

void Func_02002250(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
#ifndef UBFIX
  s32 *view = &map->viewX;
#endif
  // UB: viewZ is a separate member, not a second element of viewX (as the
  // members, 69 of 169 instructions differ in tla-en).
#ifdef UBFIX
  s32 viewX = map->viewX & 0xffff0000;
  s32 viewZ = map->viewZ & 0xffff0000;
#else
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
#endif
  s32 lift = map->origin[1];
  struct AquaRockWork *work = &gData_020034f0;
  u32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  struct Actor *source;
  struct Bubble *entry;
  s32 i, x, y, z, screenX, screenY;

  if (Random16Far() < 0x1000 && (source = work->object) != 0) {
    entry = &work->bubbles[work->next];
    entry->x = source->x + Random16Far() * 3 * 16 - (24 << 16);
    entry->y = source->y;
    entry->z = source->z;
    entry->age = 0;
    if (++work->next > 63)
      work->next = 0;
  }
  entry = work->bubbles;
  for (i = 63; i >= 0; i--, entry++) {
    if (entry->x != 0) {
      // screenX holds the ground's height here, before its own value.
      screenX = Map_GetTerrainHeight(0, entry->x, entry->z);
      if (screenX > entry->y)
        entry->y += 2 << 16;
      else
        entry->z -= 2 << 16;
      if (entry->y >> 20 == 23)
        entry->x = 0;
      if (entry->z >> 20 == 48)
        entry->x = 0;
      entry->age++;
      x = entry->x - viewX;
      y = entry->y;
      z = entry->z;
      y -= lift;
      z -= viewZ;
      z -= lift;
      screenY = z - y;
      y += z;
      y >>= 16;
      screenX = x >> 16;
      x = screenX - 4;
      screenY >>= 16;
      screenY -= 4;
      z = y + 58;
      // screenX now holds its offset into the range test (on screen from
      // -11 to 323).
      screenX += 11;
      if ((u32)screenX <= 334 && screenY > -16 && screenY <= 239) {
        x &= 0x1ff;
        screenY &= 255;
        entry->oam[0] = 0;
        entry->oam[1] = screenY | (x << 16) | 0x2000;
        entry->oam[2] = (tile + (entry->age & 3) * 2) | 0x400;
        Oam_AddSpriteAtDepth(entry->oam, z);
      }
    }
  }
}

void Func_020023c0(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 lift = map->origin[1];
  struct AquaRockWork *work = &gData_020034f0;
  u32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  struct Actor *source;
  struct Bubble *bubble;
  s32 i, x, y, depth, screenX, screenY, z, row;

  if (Random16Far() < 0x1000 && (source = work->object) != 0) {
    bubble = &work->bubbles[work->next];
    bubble->x = source->x + Random16Far() * 3 * 8 - 0xc0000;
    bubble->y = source->y;
    bubble->z = source->z;
    bubble->age = 0;
    if (++work->next > 63)
      work->next = 0;
  }
  bubble = work->bubbles;
  for (i = 63; i >= 0; i--, bubble++) {
    if (bubble->x != 0) {
      if (Map_GetTerrainHeight(0, bubble->x, bubble->z) < bubble->y)
        bubble->y -= 0x20000;
      else
        bubble->z += 0x20000;
      if (bubble->y >> 20 == -4)
        bubble->x = 0;
      if (bubble->z >> 20 == 50)
        bubble->x = 0;
      bubble->age++;
      z = bubble->z;
      x = bubble->x - viewX;
      y = bubble->y - lift;
      z -= viewZ;
      z -= lift;
      row = z - y;
      screenX = x >> 16;
      y += z;
      x = screenX - 4;
      screenY = (row >> 16) - 4;
      y >>= 16;
      depth = y + 58;
      if ((u32)(screenX + 11) <= 334 && screenY > -16 && screenY <= 239) {
        x &= 0x1ff;
        screenY &= 255;
        bubble->oam[0] = 0;
        bubble->oam[1] = screenY | (x << 16) | 0x2000;
        bubble->oam[2] = (tile + (bubble->age & 3) * 2) | 0x400;
        Oam_AddSpriteAtDepth(bubble->oam, depth);
      }
    }
  }
}

void Func_02002534(void) {
  if (gData_020034f0.direction > 0)
    Func_02002250();
  else
    Func_020023c0();
}

void Func_02002550(void) {
  struct AquaRockWork *work = &gData_020034f0;
  void *buffer;

  Iwram_ClearWordsEntry(work, 0x70c);
  buffer = MemAlloc_(0x100);
  Func_020028b8(gData_02002c74, buffer);
  work->vramEntry = Resource_FindFreeEntry();
  VramCache_Load(work->vramEntry, 0x100, buffer);
  Heap_Release(buffer);
  // Use a value-returning call; a void declaration changes argument order
  // (differ 2).
  ((s32 (*)(void *, s32))Task_Add)(Func_02002534, 0x480);
}

void Func_020025a8(s32 id) {
  struct AquaRockWork *work = &gData_020034f0;

  if (id == 0) {
    work->object = 0;
  } else {
    work->object = Actor_GetById(id);
    if (id == 8)
      work->direction = -1;
    else
      work->direction = 1;
  }
}

#include "heap_release.h"
#include "dma.h"
#include "io_reg.h"
#include "resource.h"

DEFINE_GROUND_PARTICLES_DRAW(GroundParticles_Draw, gData_02002cd4)

/// Starts the sparkles: 64 sprites on the ground at the origin, their tiles
/// decoded into VRAM, and GroundParticles_Draw scheduled at priority 0x480 to move
/// them; the overlay's copy of FieldEffect_InitializeSparkles.
void Func_0200276c(void) {
  struct DustWork *work = HeapSlot_AllocEwram(116, sizeof(struct DustWork));
  struct DustParticle *p = work->particles;
  u8 *buf;
  u32 i;
  u32 size;
  volatile u32 zero;

  /* FAKEMATCH(temporary): one local is the fill word, the end word and each
   * particle's attribute cursor, so it lives on the stack and every step of
   * the cursor is stored there, as the native's; with a separate cursor, 55 of
   * 81 instructions differ in tla-en. */
  size = 0;
  zero = size;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | 517, REG_DMA3);
  buf = HeapSlot_AllocEwram(56, 0x400);
  Func_020028b0(gField_Tiles679, buf);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->vramEntry, 0x300, buf);
  HeapSlot_Release(56);
  work->field808 = size;
  i = 0;
loop:
  size = (u32)p;
  *(u32 *)size = 0;
  size += 4;
  *(u32 *)size = 0x40000400;
  size += 4;
  *(u32 *)size = 0xb400;
  p->posX = 0;
  p->posZ = 0;
  p->posY = Map_GetTerrainHeight(0, 0, 0) << 16;
  p->timer = (i & 15) + 1;
  i++;
  p++;
  if (i < 64)
    goto loop;
  Task_Add(GroundParticles_Draw, TASK_PASS_VBLANK);
}

void Func_02002834(s32 id) {
  u8 *work = Ram_HeapSlots->unk70[1];
  struct Actor *object = Actor_GetById(id);
  s32 *slot = (s32 *)(work + 0x808);

  *slot = 0;
  if (id != 0 && object != 0)
    *slot = (s32)&object->x;
}

s32 Func_02000384(struct Actor *object);

// clang-format off
u32 gData_02002a78[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002ab4[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002af0[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s32 gData_02002b2c[] = {
    46,
    (s32)Func_02000478,
    (s32)0x80010000,
    39,
    2,
    46,
    (s32)Func_02000384,
    1,
    39,
    1,
    0,
    120,
    39,
    2,
    46,
    (s32)Func_02000384,
    1,
    39,
    1,
    0,
    20,
    39,
    2,
    46,
    (s32)Func_02000384,
    1,
    39,
    1,
    0,
    180,
    13,
    0xffff,
    (s32)0xc0010000,
    17,
};

s32 gData_02002bb4[] = {
    46,
    (s32)Func_02000478,
    (s32)0x80010000,
    0,
    60,
    39,
    2,
    46,
    (s32)Func_02000384,
    1,
    39,
    1,
    0,
    120,
    39,
    2,
    46,
    (s32)Func_02000384,
    1,
    39,
    1,
    0,
    15,
    39,
    2,
    46,
    (s32)Func_02000384,
    1,
    39,
    1,
    0,
    180,
    13,
    0xffff,
    (s32)0xc0010000,
    17,
};

u32 gData_02002c44[] = {
    0, 20, 38,
};

u32 gData_02002c50[] = {
    23, 6, 0x4000, 0, 1, 13,
    30, 0, 38,
};
// clang-format on
