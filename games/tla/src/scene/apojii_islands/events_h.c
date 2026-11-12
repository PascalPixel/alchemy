/// @file
/// Scene code of overlay 0x679, in ROM order, from events_h's offset up to the next object the overlay links.

#include "local.h"
#include "scene_ids.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_02001140(struct Actor *object) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child =
        Import_ActorCreate_(0x10d, object->x, object->y, object->z);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Func_02001c74(child, gData_02001ef4);
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

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

void Func_020011a8(void) {
  struct EventWork1cc *work = Ram_HeapSlots->eventWork;
  s32 owner = gMainRoutineWork.currentOwner;
  struct Actor *anchor = work->object;
  struct Actor *leader = Actor_Get(owner);
  s32 position[3];
  s32 result;

  if (gMainRoutineWork.scene != (s32)&SceneId_Ov67bRoom1 ||
      leader->terrainHeight != 0) {
    position[0] = leader->x;
    position[1] = leader->y;
    position[2] = leader->z + 0x20000;
    result = Func_02001c9c(leader, position);
    if ((gFrameCount & 4) == 0)
      Func_020010d8(leader);
    if (result < 0) {
      /* FAKEMATCH(wrapper): Call2 kept; as a direct call, 2 of 151 instructions
       * differ in tla-en. */
      Actor_SetAttachedEffect(owner, 0x102);
      FieldActor_SetMoveTarget(leader, leader->x, leader->y, leader->z + 0x80000);
      Func_02001c6c(leader, 49);
      Actor_WaitForMove(leader);
      do {
        System_Wait(1);
      } while (leader->y != leader->terrainHeight);
      Func_020010d8(leader);
      Func_02001c6c(leader, 49);
      System_Wait(3);
    } else {
      position[0] = leader->x;
      position[1] = leader->y;
      position[2] = leader->z + 0x80000;
      result = Func_02001c9c(leader, position);
      if (result <= 0) {
        position[0] = leader->x + 0x5b333;
        position[1] = leader->y;
        position[2] = leader->z + 0x5b333;
        result = Func_02001c9c(leader, position);
        if (result <= 0) {
          position[0] = leader->x - 0x5b333;
          position[1] = leader->y;
          position[2] = leader->z + 0x5b333;
          result = Func_02001c9c(leader, position);
          if (result <= 0) {
            anchor->z += 0x20000;
            leader->z += 0x20000;
            leader->facing = 0x4000;
          }
        }
      }
    }
  }
}

void Func_02001308(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->sprite, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Func_02001c6c(object, 0);
    Func_02001c6c(object, 2);
  }
}

void Func_02001338(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 i;

  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  leader->motionFlags = 0;
  leader->priorityFlags |= 0x20;
  Event_PlaySound(215);
  Func_02001c6c(leader, 18);
  Event_PlaySound(612);
  for (i = 0; i < 60; i++) {
    s32 angle;

    if (i == 30)
      Event_FinishScreenTransition();
    leader->y -= 0x4000;
    angle = leader->facing;
    leader->facing = angle + 0x1000;
    if ((i & 7) == 0)
      Func_020010d8(Actor_Get(15));
    System_Wait(1);
  }
  Func_02001ccc();
  FieldEvent_SetExitRequest(entrance);
}

/// Every other frame, emits particle 7 (5 on every other such frame) from the
/// object, 26 pixels up, swaying with the frame count and drifting by a random
/// amount.
s32 Func_020013d8(struct Actor *object) {
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

void Func_02001464(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 *owner = &gMainRoutineWork.currentOwner;

  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(228);
  leader->update = (void (*)(void))Func_020013d8;
  leader->motionFlags = 0;
  leader->speedLimit = 0x3333;
  Actor_SetAnimation(*owner, 2);
  Actor_SetDestinationOffset(*owner, 0, -8);
  FieldActor_WaitForMoveById(*owner);
  Actor_SetSpriteColor(Actor_Get(*owner), 9);
  Actor_SetSingleChildFlags(Actor_Get(*owner), 0);
  leader->update = 0;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(entrance);
  Func_02001ccc();
}

void Func_02001500(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 flag = Event_GetFlag(0x109);
  s32 *owner = &gMainRoutineWork.currentOwner;
  s32 offset;

  if (flag == 0) {
    Func_02001cc4();
    FieldEvent_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    leader->motionFlags = flag;
    Actor_SetPosition(*owner, ((s16 *)&leader->x)[1] << 16,
                                           (((s16 *)&leader->z)[1] << 16) -
                                               0x100000);
    Actor_SetSpriteColor(Actor_Get(*owner), 9);
    Actor_SetSingleChildFlags(Actor_Get(*owner), 0);
    Event_StartScreenTransition();
    Event_PlaySound(228);
    leader->update = (void (*)(void))Func_020013d8;
    Actor_SetSpeed(*owner, 0x6666, 0x3333);
    Actor_WalkByAndWait(*owner, 0, 8);
    Actor_SetSpriteColor(Actor_Get(*owner), 0);
    Actor_SetSingleChildFlags(Actor_Get(*owner), 1);
    ((struct FieldSprite *)leader->sprite)->priority = 1;
    offset = 10;
    Actor_WalkByAndWait(*owner, 0, offset);
    leader->motionFlags = 3;
    leader->update = (void (*)(void))flag;
    Func_02001df4();
    Event_WaitTransitionFrames();
    Func_02001ccc();
  }
}

#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"

void Func_02001c3c(void *entry, s32 slot);

#define COPY_PICTURE(from, to)                                                 \
  Dma_Set((void *)(from), (void *)(offset + (to)), DMA_COPY_WORDS(16), REG_DMA3)

void Func_020015f8(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  // viewZ read through a pointer to viewX, its neighbour; as map->viewZ, 30
  // of 189 instructions differ in tla-en.
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  // DEAD: viewY is subtracted from both height and z and cancels in dy; the
  // source keeps its load and a store to the stack that nothing reads.
  s32 viewY = map->origin[1];
  struct CellMarkers *table = &gData_02002aa0;
  struct VramBlockCacheEntry *entry = &gResourceTableEntries[table->block];
  u32 tile = entry->offset >> 5;
  u32 offset = entry->offset;
  struct CellMarker *record;
  s32 i;

  COPY_PICTURE(0x0600f000, 0x06010000);
  COPY_PICTURE(0x0600f400, 0x06010040);
  COPY_PICTURE(0x0600f040, 0x06010080);
  COPY_PICTURE(0x0600f440, 0x060100c0);
  COPY_PICTURE(0x0600f080, 0x06010100);
  COPY_PICTURE(0x0600f480, 0x06010140);
  COPY_PICTURE(0x0600f080, 0x06010180);
  COPY_PICTURE(0x0600f480, 0x060101c0);
  record = table->markers;
  for (i = 127; i >= 0; i--, record++) {
    s32 dx, dy, height, z, palette, flip, kind;
    s32 x, y, left, top, picture;
    u32 clear;
    u32 *attr;

    if (record->state == 0)
      continue;
    dx = record->x - viewX;
    height = record->y - viewY;
    z = record->z - viewZ - viewY;
    dy = z - height;
    if (record->y == record->rest && record->state != 2)
      palette = 0xec00;
    else
      palette = 0xe800;
    y = dy >> 16;
    clear = 0;
    x = dx >> 16;
    left = x - 8;
    top = y - 8;
    if ((u32)(x + 7) > 334)
      continue;
    if ((u32)(y + 39) > 286)
      continue;
    left &= 0x1ff;
    /* FAKEMATCH(dead): a self-update of Ram_HeapSlots->mapWork after left's
     * mask; as nothing, 82 of 189 instructions differ in tla-en. */
    Ram_HeapSlots->mapWork += 0;
    top &= 255;
    attr = record->oam;
    kind = record->kind;
    picture = kind * 4;
    flip = 0;
    if (kind == 2)
      flip = 0x10000000;
    *attr++ = clear;
    *attr++ = top | (left << 16) | flip | 0x40000400;
    *attr = palette | (tile + picture);
    Func_02001c3c(record->oam, 0);
  }
}

/// Clears the effect table, takes a free VRAM entry for it with a 0x200-byte
/// block and no source, then schedules Func_020015f8.
void Func_020017bc(void) {
  Iwram_ClearWords(&gData_02002aa0, 0x1004);
  gData_02002aa0.block = Resource_FindFreeEntry();
  Vram_Load(gData_02002aa0.block, 0x200, 0);
  FieldTask_AddFrame(Func_020015f8, 0x480);
}

#include "system.h"
#include "event_inline.h"
#include "heap_release.h"
#include "io_reg.h"
#include "resource.h"
#include "task.h"

extern u8 Data_0202c001[];

void Func_020017f8(u32 base, s32 a, s32 b) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *second = &map->layers[1];
  struct MapLayer *layer = &map->layers[0];
  struct MapCell *cells = second->cells;
  s32 left = *(s32 *)&layer->unk08[0] + map->minX;
  s32 top = *(s32 *)&layer->unk08[4] + map->minZ;
  struct CellMarkers *markers = &gData_02002aa0;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 height = (map->maxZ - map->minZ) >> 20;
  struct CellMarker *entry = &markers->markers[markers->count];
  s32 *slot = (s32 *)(Data_0202c001 + 0x1df);
  s32 origin = ((map->minZ >> 20) << 7) + (map->minX >> 20);
  struct MapCell *cell;
  s16 x, z;
  u32 value;

  *slot = a << 8;
  cells += origin;
  z = 0;
  if (z < height) {
  outer:
    cell = cells + ((u16)z << 7);
    x = 0;
    if (x < width) {
    inner:
      value = cell->collisionCode;
      if (value != 0 && value >= base && value < base + 4) {
        entry->x = ((u16)x << 20) + left + 0x80000;
        entry->z = ((u16)z << 20) + top + 0x80000;
        entry->rest = b << 19;
        entry->y = a << 19;
        entry->kind = value - base;
        entry->state = 1;
        entry++;
        markers->count++;
        // Layer 0's row base in its own local (written inline, 10 of 159
        // differ).
        {
          u8 *terrain = map->layers[0].terrainIds + origin;

          terrain[(u16)x + ((u16)z << 7)] = 120;
        }
      }
      // The step spelled as a halfword sum (as x++, 40 of 159 instructions
      // differ in tla-en).
      x = ((x << 16) + 0x10000) >> 16;
      cell++;
      if ((u16)x < width)
        goto inner;
    }
    // The same halfword-sum spelling (as z++, 16 of 159 differ).
    z = ((z << 16) + 0x10000) >> 16;
    if ((u16)z < height)
      goto outer;
  }
}

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

extern s16 gData_02001f18[];

void Func_02001940(void) {
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
    frame = &gData_02001f18[5 * particle->timer];
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
      Func_02001c3c(particle, 240);
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

/// Starts the sparkles: 64 sprites on the ground at the origin, their tiles
/// decoded into VRAM, and Func_02001940 scheduled at priority 0x480 to move
/// them; the overlay's copy of FieldEffect_InitializeSparkles.
void Func_02001ad8(void) {
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
  Func_02001c24(gField_Tiles679, buf);
  work->vramEntry = Resource_FindFreeEntry();
  work->tileBase = Vram_Load(work->vramEntry, 0x300, buf);
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
  Task_AddFrame(Func_02001940, TASK_PASS_VBLANK);
}

void Func_02001ba0(s32 id) {
  u8 *work = Ram_HeapSlots->unk70[1];
  struct Actor *object = Actor_Get(id);
  s32 *slot = (s32 *)(work + 0x808);

  *slot = 0;
  if (id != 0 && object != 0)
    *slot = (s32)&object->x;
}

#include "scene_tables.h"
#include "text/msg_ids.h"
#include "version.h"

#if !TLA_PATCH_1
// The Japanese table holds 131 where later editions hold MsgFieldOff's number,
// which the Japanese text numbers alike: a changed value, from patch 1.
// XXX: is it a message at all; what reads gData_02002044 would settle it.
#define UNK_02002044_VALUE 131
#else
#define UNK_02002044_VALUE MSGNO(MsgFieldOff)
#endif

void Func_0200033c(void);
void Func_020003b0(void);
void Func_020003cc(void);
void Func_02000694(void);
void Func_020006a8(void);
void Func_020007a4(void);
void Func_02000a48(void);
void Func_02000a68(void);
void Func_02000af8(void);
void Func_02000b88(void);
void Func_02000ba8(void);
void Func_02000c38(void);
void Func_02000cb4(void);
void Func_02000d20(void);
void Func_02000d74(void);
void Func_02000de0(void);
void Func_02000e40(void);
void Func_02000e98(void);
void Func_020010d0(void);

extern u32 gData_02001e34[];
extern u32 gData_02001e70[];
extern u32 gData_02001eac[];
extern s32 gData_02002044[];

// clang-format off
u32 gData_02001e34[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001e70[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001eac[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001ee8[] = {
    0, 20, 38,
};

u32 gData_02001ef4[] = {
    23, 6, 0x4000, 0, 1, 13,
    30, 0, 38,
};

s16 gData_02001f18[] = {
    -8, -12, 20, 0, 1, -8, -12, 20,
    0, 1, -8, -12, 20, 0, 1, -8,
    -12, 16, 0, 1, -8, -12, 16, 0,
    1, -8, -12, 12, 0, 1, -8, -12,
    8, 0, 1, 0, -32, 0, 2, 2,
    8, -48, 0, 2, 2, 16, -64, 0,
    2, 2, 24, -80, 0, 2, 2, 32,
    -96, 0, 2, 2, 40, -112, 0, 2,
    2, 48, -128, 0, 2, 2, 56, -144,
    0, 2, 2, 64, -160, 0, 2, 2,
    -2, 12, -4, 12, -6, 12, -8, 8,
    -10, 8, -12, 8, -14, 8, -16, 8,
    -19, 4, -21, 4, -24, 4, -27, 4,
    -30, 4, -33, 4, -36, 4, -40, 4,
    -44, 4, -48, 0, -52, 0, -56, 0,
    -60, 0, -64, 0, -68, 0, -72, 0,
    -76, 0, -80, 0, -85, 0, -90, 0,
    -95, 0, -100, 0, -110, 0, -120, 0,
};

s32 gData_02002038[] = {
    (s32)gData_02001e34,
    (s32)gData_02001e70,
    (s32)gData_02001eac,
};

s32 gData_02002044[] = {
    22,
    30,
    UNK_02002044_VALUE,
    22,
    16,
    0x3333,
    22,
    15,
    0x9999,
    (s32)0x80010000,
    39,
    2,
    4,
    0x1280000,
    0,
    0x880000,
    1,
    4,
    0x1280000,
    0,
    0x680000,
    1,
    39,
    1,
    0,
    60,
    0,
    10,
    0,
    60,
    39,
    2,
    4,
    0x1280000,
    0,
    0x880000,
    1,
    4,
    0x1180000,
    0,
    0x880000,
    1,
    39,
    1,
    0,
    60,
    0,
    10,
    0,
    60,
    13,
    0xffff,
    (s32)0xc0010000,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(154),
    SCENE_EXIT_IF(1, 2, 36, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 155, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 155, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 156, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 156, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 156, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 156, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 156, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 58, 5, CONDITION_ALWAYS),
    SCENE_EXIT(77, 2, 78),
    SCENE_EXITS(156),
    SCENE_EXIT_IF(1, 154, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 154, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 154, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 154, 7, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 154, 8, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 135, 2, CONDITION_ALWAYS),
    SCENE_EXITS(155),
    SCENE_EXIT_IF(1, 154, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 154, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 157, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 157, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 157, 5, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

u32 gData_02002200[] = {
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002218[] = {
    0xffff0073, 1, 0x380000, 0, 0xa80000, 0x1a000,
    0xffff0074, 1, 0x1c80000, 0, 0x580000, 0x3e000,
    0xffff0072, 1, 0x2380000, 0, 0x1380000, 0x1e000,
    0xffff0080, 2, 0x1f80000, 0, 0xc80000, 0xc000,
    0xffff0070, 1, 0x2680000, 0, 0x1080000, 0x12000,
    0xffff0071, 2, 0xb80000, 0, 0x980000, 0x1e000,
    0xffff0088, 3, 0xd80000, 0, 0x1780000, 0x4000,
    0xffff008a, 3, 0x680000, 0, 0x1580000, 0x2000,
    0xffff0073, 1, 0x1580000, 0, 0x1180000, 0x1c000,
    0xffff017f, 1, 0x1600000, 0, 0xe80000, 0x24000,
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02002320[] = {
    (s32)0xffff0074,
    1,
    0x980000,
    0,
    0x980000,
    0x10000,
    (s32)0xffff0088,
    1,
    0x780000,
    0,
    0x680000,
    0x22000,
    (s32)0xffff0072,
    1,
    0x1080000,
    0,
    0x680000,
    0x12000,
    (s32)0xffff0080,
    (s32)gData_02002044,
    0x1180000,
    0,
    0x880000,
    0x10000,
    (s32)0xffff0088,
    1,
    0x1980000,
    0,
    0x680000,
    0x14000,
    (s32)0xffff008a,
    2,
    0x1580000,
    0,
    0x780000,
    0x1e000,
    (s32)0xffff0070,
    3,
    0x2500000,
    0,
    0x780000,
    0x12000,
    (s32)0xffff0071,
    1,
    0x2280000,
    0,
    0x880000,
    0x14000,
    (s32)0xffff006e,
    1,
    0x1a00000,
    0,
    0x1680000,
    0x14000,
    (s32)0xffff0086,
    1,
    0x900000,
    0,
    0x1680000,
    0x14000,
    (s32)0xffff0087,
    1,
    0xd80000,
    0,
    0x1780000,
    0x16000,
    (s32)0xffff0070,
    3,
    0x800000,
    0,
    0x26c0000,
    0x4000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

u32 gData_02002458[] = {
    0xffff0197, 1, 0x1080000, 0, 0x1180000, 0x2c000,
    0xffff0197, 1, 0xd80000, 0, 0x1180000, 0x2c000,
    0x7700f6, 1, 0x1780000, 0, 0xc80000, 0x8000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

s32 gData_020024b8[] = {
    -1,
    0,
    0,
};

s32 gData_020024c4[] = {
    -1,
};

s32 gData_020024c8[] = {
    -1,
};

s32 gData_020024cc[] = {
    -1,
};

s32 gData_020024d0[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    2,
    (s32)0xffff0004,
    (s32)Func_02000c38,
    2,
    (s32)0xffff0005,
    (s32)Func_02000c38,
    2,
    (s32)0xffff0006,
    (s32)Func_02000c38,
    2,
    (s32)0xffff0007,
    (s32)Func_02000c38,
    2,
    (s32)0xffff0008,
    (s32)Func_02000c38,
    1,
    (s32)0xffff0009,
    9,
    0xc602,
    (s32)0xffff0021,
    (s32)Func_020010d0,
    0xc602,
    (s32)0xffff0015,
    (s32)Func_020010d0,
    0,
    (s32)0xffff0008,
    MSGNO(MsgThisIsApojii),
    0,
    (s32)0xffff0009,
    MSGNO(MsgWhateverYouDo),
    0,
    (s32)0xffff000a,
    MSGNO(MsgTheHugeWaterfall),
    0,
    (s32)0xffff000b,
    MSGNO(MsgOnAquaIsland),
    0,
    (s32)0xffff000c,
    MSGNO(MsgHaveYouAny),
    0,
    (s32)0xffff000d,
    MSGNO(MsgIDonTKnowWhat),
    0,
    (s32)0xffff000e,
    MSGNO(MsgWhenTheyCome),
    0,
    (s32)0xffff000f,
    MSGNO(MsgDoYouThink),
    0,
    (s32)0xffff0010,
    MSGNO(MsgIfThereS),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgLookingOverGaia),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgTheCurrentsNear),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgAllOfThe),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgWhyWouldAnyone),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgWellAtLeast),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgApojiiHasSo),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgAquaIslandIs),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgIWonderWhere),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgAquaRockMust),
    0x2115,
    0x95f0011,
    (s32)Func_020007a4,
    0x8b85,
    -0x10000,
    (s32)Func_020003b0,
    0x40008b85,
    -0x10000,
    (s32)Func_020003cc,
    0x50009085,
    -0x10000,
    (s32)Func_02000694,
    0x40009085,
    -0x10000,
    (s32)Func_020006a8,
    0x50008905,
    (s32)0xffff0015,
    (s32)Func_02000a48,
    0x50008905,
    (s32)0xffff0016,
    (s32)Func_02000a68,
    0x50008905,
    (s32)0xffff0017,
    (s32)Func_02000af8,
    -1,
    0,
    0,
};

s32 gData_02002698[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    0,
    (s32)0xffff0008,
    MSGNO(MsgThereSThis),
    0,
    (s32)0xffff0009,
    (s32)Func_02000cb4,
    0,
    (s32)0xffff000a,
    MSGNO(MsgIAbsolutelyRefuse),
    0,
    (s32)0xffff000b,
    MSGNO(MsgTheCurrentsAround),
    0,
    (s32)0xffff000c,
    MSGNO(MsgIMNotAllowed),
    0,
    (s32)0xffff000d,
    MSGNO(MsgHeyMisterDid),
    0,
    (s32)0xffff000e,
    MSGNO(MsgThatOldAqua),
    0,
    (s32)0xffff000f,
    MSGNO(MsgHeSToo),
    0,
    (s32)0xffff0010,
    (s32)Func_02000e98,
    0,
    (s32)0xffff0011,
    (s32)Func_02000de0,
    0,
    (s32)0xffff0012,
    (s32)Func_02000e40,
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgIJustWish),
    0x8d15,
    (s32)0xffff0009,
    (s32)Func_02000d20,
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgISinkLike),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgPoorBoyHis),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgItSStupid),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgWowHeSwam),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgYouUnlockThe),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgHeAlwaysWanted),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgNoOneShips),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgIMSure),
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgThoseGuysOn),
    0,
    (s32)0xffff0013,
    MSGNO(MsgTheTemperatureOf),
    0x8d15,
    (s32)0xffff0013,
    MSGNO(MsgLongAgoPoseidon),
    -1,
    0,
    0,
};

s32 gData_0200280c[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    2,
    (s32)0xffff0003,
    (s32)Func_02001464,
    2,
    (s32)0xffff0004,
    (s32)Func_02001464,
    2,
    (s32)0xffff0005,
    (s32)Func_02001464,
    0x4602,
    (s32)0xffff0028,
    (s32)Func_02000d74,
    0,
    (s32)0xffff000a,
    (s32)Func_0200033c,
    0x8515,
    0x2000008,
    0,
    0x50008905,
    (s32)0xffff0015,
    (s32)Func_02000b88,
    0x50008905,
    (s32)0xffff0016,
    (s32)Func_02000ba8,
    -1,
    0,
    0,
};
