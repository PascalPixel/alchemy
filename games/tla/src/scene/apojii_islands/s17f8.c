/// @file
/// Scene code of overlay 0x679, in ROM order, from s17f8's offset up to the next object the overlay links.

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_inline.h"
#include "heap_release.h"
#include "dma.h"
#include "io_reg.h"
#include "resource.h"
#include "task.h"

struct CellMarker {
  s32 x;
  s32 y;
  s32 z;
  s32 rest;
  u16 kind;
  u16 state;
  u32 oam[3];
};

struct CellMarkers {
  u16 block;
  u16 count;
  struct CellMarker markers[128];
};

extern struct CellMarkers gData_02002aa0;
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

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02001c3c(void *entry, s32 slot);

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

extern const u8 gField_Tiles679[];
void Func_02001c24(const void *source, void *destination);

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
