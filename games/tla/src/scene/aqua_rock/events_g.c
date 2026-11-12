#include "local.h"

void Func_02001d98(struct Actor *object) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child =
        Import_ActorCreate_(0x10d, object->x, object->y, object->z);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Func_02002918(child, gData_02002c50);
      child->motionFlags = 0;
      child->y = -0x78000;
      child->terrainId = 1;
      if (sprite != 0) {
        Animation_ApplyChildArgument(sprite, 2);
        sprite->nextFrame = 0;
        sprite->priority = 2;
      }
    }
  }
}

#include "event_inline.h"
#include "scene_ids.h"

void Func_02001d30(struct Actor *object);

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

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
  result = Func_02002970(leader, position);
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
  result = Func_02002970(leader, position);
  if (result > 0)
    return;
  position[0] = leader->x + 0x5b333;
  position[1] = leader->y;
  position[2] = leader->z + 0x5b333;
  result = Func_02002970(leader, position);
  if (result > 0)
    return;
  position[0] = leader->x - 0x5b333;
  position[1] = leader->y;
  position[2] = leader->z + 0x5b333;
  result = Func_02002970(leader, position);
  if (result > 0)
    return;
  anchor->z += 0x20000;
  leader->z += 0x20000;
  leader->facing = 0x4000;
}

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

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

  Func_02002998();
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
  Func_020029a0();
  Import_EventSetExitRequest(arg0);
}

/// Every other frame, emits particle 7 (5 on every other such frame) from the
/// object, 26 pixels up, swaying with the frame count and drifting by a random
/// amount.
s32 Func_02002030(struct Actor *object) {
  s32 params[10];

  if (gFrameCount & 1) {
    s32 drift;
    s32 x;
    s32 y;

    params[1] = 7;
    if ((gFrameCount & 2) == 0)
      params[1] = 5;
    params[2] = 0xcccc;
    params[3] = 0xcccc;
    params[0] = 0;
    drift = ((u32)(Random16Far() << 3) >> 16) * 0x3333;
    x = object->x;
    y = object->y;
    Func_0200015c(x + ((8 - (gFrameCount & 15)) << 16), y + 0x1a0000, object->z,
                  0, -drift, 0, 0xb0000, params);
  }
  return 0;
}

void Func_020020bc(s32 entrance) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 *owner = &gMainRoutineWork.currentOwner;

  Func_02002998();
  Event_ResolvePendingActions(0);
  Sound_Play(228);
  leader->update = (void (*)(void))Func_02002030;
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
  Func_020029a0();
}

void Func_02002158(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 flag = GameFlag_Test(0x109);
  s32 *owner = &gMainRoutineWork.currentOwner;
  s32 offset;

  if (flag == 0) {
    Func_02002998();
    Event_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    leader->motionFlags = flag;
    Actor_SetHorizontalPositionWithTerrain(*owner, ((s16 *)&leader->x)[1] << 16,
                                           (((s16 *)&leader->z)[1] << 16) -
                                               0x100000);
    Actor_SetPartAttribute(Actor_GetById(*owner), 9);
    Actor_SetSingleChildFlags(Actor_GetById(*owner), 0);
    Event_StartScreenTransition();
    Sound_Play(228);
    leader->update = (void (*)(void))Func_02002030;
    Actor_SetSpeedParameters(*owner, 0x6666, 0x3333);
    Actor_CommitPositionAndActivate(*owner, 0, 8);
    Actor_SetPartAttribute(Actor_GetById(*owner), 0);
    Actor_SetSingleChildFlags(Actor_GetById(*owner), 1);
    ((struct FieldSprite *)leader->sprite)->priority = 1;
    offset = 10;
    Actor_CommitPositionAndActivate(*owner, 0, offset);
    leader->motionFlags = 3;
    leader->update = (void (*)(void))flag;
    Func_02002a60();
    Event_WaitTransitionFrames();
    Func_020029a0();
  }
}

#include "system.h"
#include "actor.h"
#include "map.h"
#include "ram_buffer.h"
#include "graphics/vram_tab.h"

void Func_020028d0(void *entry, s32 slot);

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
        Func_020028d0(entry->oam, z);
      }
    }
  }
}

#include "types.h"

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
        Func_020028d0(bubble->oam, depth);
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

  Iwram_ClearWords(work, 0x70c);
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
#include "task.h"

struct GroundParticle {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 delay;
  u16 timer;
  u8 unk1e[2];
};

struct GroundParticleWork {
  s32 entry;
  s32 tileBase;
  struct GroundParticle particles[64];
  s32 *leader;
};

extern s16 gData_02002cd4[];

void Func_020025d4(void) {
  struct MapWork *view = Ram_HeapSlots->mapWork;
  struct GroundParticleWork *work = Ram_HeapSlots->unk70[1];
  s32 *camera = &view->viewX;
  struct GroundParticle *particle = work->particles;
  s32 cameraX = camera[0];
  s32 cameraZ = camera[1];
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_Test(0x166))
      particle->timer++;
    frame = &gData_02002cd4[5 * particle->timer];
    x = (particle->posX - cameraX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - cameraZ) / 0x10000 + *frame++;
    if ((x >= -16 && x <= 239) && y >= -32 && y <= 159) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tileBase + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Func_020028d0(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *leader = work->leader;

      if (leader != NULL) {
        x = leader[0] + Random16Far() * 240 - 0x780000;
        y = leader[2] + Random16Far() * 160 - 0x500000;
        particle->posX = x;
        particle->posZ = y;
        particle->posY = Map_GetTerrainHeight(0, x, y);
        particle->timer = 16;
      } else {
        particle->posX = 0;
        particle->posZ = 0;
        particle->posY = 0;
        particle->timer = 16;
      }
    }
  }
}

struct DustParticle {
  u32 link;
  u32 attr01;
  u32 attr2;
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 pad18[4];
  u16 timer;
  u8 pad1e[2];
};

struct DustWork {
  s32 vramEntry;
  s32 tileBase;
  struct DustParticle particles[64];
  u32 field808;
  u8 pad80c[8];
};

extern const u8 gField_Tiles679[];
void Func_020028b0(const void *source, void *destination);

/// Starts the sparkles: 64 sprites on the ground at the origin, their tiles
/// decoded into VRAM, and Func_020025d4 scheduled at priority 0x480 to move
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
  Task_Add(Func_020025d4, TASK_PASS_VBLANK);
}

void Func_02002834(s32 id) {
  u8 *work = Ram_HeapSlots->unk70[1];
  struct Actor *object = Actor_GetById(id);
  s32 *slot = (s32 *)(work + 0x808);

  *slot = 0;
  if (id != 0 && object != 0)
    *slot = (s32)&object->x;
}

#include "scene_tables.h"

s32 Func_02000384(struct Actor *object);
s32 Func_02000478(struct Actor *object);

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
