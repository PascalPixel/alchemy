/// @file
/// The scene code of overlay 0x67b that its scene tables belong to, in ROM
/// order; the rest of the overlay is in aqua_rock/.

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

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "aqua_rock/local.h"

const u8 *Func_02000344(void) {
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

struct ActorTail {
  u8 head[0x5e];
  u16 unk5e;
};

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

const u8 *Func_020004a4(void) {
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

struct Unk02003450 {
  s16 unk00;
  s16 unk02;
  s16 unk04;
  s16 unk06;
  s16 unk08;
  s16 unk0A;
  s16 unk0C;
  s16 unk0E;
  s16 unk10;
  s16 unk12;
  s16 unk14;
  s16 unk16[2];
  s16 unk1A[2];
  s16 unk1E;
};
extern struct Unk02003450 Data_02003450[];

void Func_02000580(void) {
  struct Unk02003450 *entry;
  s32 n, i, flag;
  /* FAKEMATCH(wrapper): the four copies forwarded through Call6; as direct
   * calls, 277 of 209 instructions differ in tla-en. */

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

void Func_02000b10(struct Actor *object) {
  if (gFrameCount & 2)
    Animation_ApplyChildValues(object, 7);
  else
    Animation_ApplyChildValues(object, 0);
}

void Func_02000b34(struct Actor *object) {
  if (gFrameCount & 2)
    Animation_ApplyChildValues(object, 6);
  else
    Animation_ApplyChildValues(object, 0);
}

void Func_02000b58(struct Actor *object) {
  Animation_ApplyChildValues(object, 0);
  object->update = 0;
}

void Func_02002834(s32 id);
void Func_020025a8(s32 id);
void Func_02002948(s32 layer);
void Func_02002940(s32 layer);

/// Toggles flag 0xa68 and with it which of two layer pairs is shown: four
/// rounds of a random colour value, then one pair's blend scripts and layer
/// fields set and the other's cleared, while the object blinks.
void Func_02000b68(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 i;

  Func_02002998();
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
  if (GameFlag_Test_(0xa68) == 0) {
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
  Func_020029a0();
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

void Func_02001e00(void);
s32 Func_02002970(struct Actor *object, struct Vector3 *position);
void Func_02001d30(struct Actor *object);

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

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
    result = Func_02002970(leader, &ahead);
    if ((gFrameCount & 4) == 0)
      Func_02001d30(leader);
    if (result < 0) {
      /* FAKEMATCH(wrapper): Call2 kept; as a direct call, 2 of 220 instructions
       * differ in tla-en. */
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
    result = Func_02002970(leader, &ahead);
    if (result > 0) {
      if (gMainRoutineWork.scene != (s32)&SceneId_Ov67bRoom1)
        return;
      Func_02002998();
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
      Func_020029a0();
      Import_EventSetExitRequest(6);
      return;
    }
    ahead.x = leader->x + 0x5b333;
    ahead.y = leader->y;
    ahead.z = leader->z - 0x5b333;
    result = Func_02002970(leader, &ahead);
    if (result > 0)
      return;
    ahead.x = leader->x - 0x5b333;
    ahead.y = leader->y;
    ahead.z = leader->z - 0x5b333;
    result = Func_02002970(leader, &ahead);
    if (result > 0)
      return;
    carried->z += -0x20000;
    leader->z += -0x20000;
  } else
    Func_02001e00();
}

#include "map.h"
#include "input.h"

struct AquaRockEventWork {
  u8 unk000[0x16a];
  s16 field16a;
};

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

void Func_02001f60(s32 id);

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

  Func_02002998();
  Event_ResolvePendingActions(0);
  Func_080d489c(gMainRoutineWork.currentOwner, 257);
  Func_02002910(object, 40);
  object->motionFlags = 3;
  for (i = 0; i < 180; i++) {
    wait(1);
    if (object->y <= object->terrainHeight)
      break;
  }
  state = &gMainRoutineWork;
  object->gravity = saved;
  Func_080d489c(state->currentOwner, 0);
  Actor_SetSingleChildFlags(object, 1);
  Func_02002910(object, 1);
  state->renderMode = 0;
  Func_020029a0();
}

void Func_020010f4(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  Func_02002960_(42, 32, 1, 1, 44, 32);
  GameFlag_SetBit(0x221);
}

void Func_0200112c(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  Func_02002960_(48, 32, 1, 1, 46, 32);
  GameFlag_SetBit(0x222);
}

void Func_02001164(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  Func_02002960_(44, 44, 1, 1, 46, 44);
  GameFlag_SetBit(0x223);
}

void Func_0200119c(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 column = object->x >> 20;

  if (column == 53) {
    wait(30);
    object->z += 0x10000;
    GameFlag_SetBit(0x951);
    Map_CopyCellAttributeRect(54, 58, 1, 1, column, 52);
  }
}

void Func_020011e0(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 column = object->x >> 20;

  if (column == 53) {
    wait(30);
    object->z += 0x10000;
    GameFlag_SetBit(0x952);
    Map_CopyCellAttributeRect(54, 58, 1, 1, column, 58);
  }
}

void Func_02002a10(void);
void Func_02001d98(struct Actor *object);

/// Sets flag 0xa69 after four rounds of a random colour value, runs the
/// blend scripts, then moves the camera to object 15, starts its update and
/// plays cue 623 six times.
void Func_02001224(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 i;

  Func_02002998();
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
  Actor_GetById(15)->update = (void (*)(void))Func_02001d98;
  Event_Wait(30);
  for (i = 5; i >= 0; i--) {
    Sound_Play(623);
    wait(10);
  }
  Event_Wait(30);
  Func_020029a0();
}

const u8 *Func_020013b8(void) {
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
  Func_02002998();
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
  Func_020029a0();
}

void Func_0200153c(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 saved = object->motionFlags;
  s32 i;
  s32 angle;

  Func_02002998();
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
  Func_020029a0();
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

  Func_02002998();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
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
  Func_020029a0();
  GameFlag_SetBit(0x955);
}
