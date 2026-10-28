// Draft of overlay 698's Func_02000aac, in 11 overlays of the 698 family:
// differ 187 of 200. Queues the lit actors' sprites: for each of the
// gData_02002202 actors in gData_02002204 with a position, projects it from
// the map's view (as the cell sprite in field/shared/map/cell_sprite.c does),
// skips it off screen, writes a 16 by 16 or 32 by 64 sprite (by
// gData_02002304) at its animation picture and priority (gData_02002306, or
// Func_02001b24 when that is -1) and pushes it at its depth. Written in the
// cell sprite's shape (271 in the earlier form). Left: register allocation;
// the native keeps the view x and z in r10 and r8, the index in r9 and the
// depth in r11 and spills the view height and tile, where this spills the
// view x and tile. Tried: the view height read first, a goto loop.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

extern s16 gData_02002200;
extern s16 gData_02002202;
extern struct FieldActor *gData_02002204[];
extern struct LightOam Data_02002244[];
extern s16 gData_02002304;
extern s16 gData_02002306;

s32 Func_02001b24(struct FieldActor *actor);
void Func_020019d4(struct LightOam *oam, s32 depth);

void Func_02000aac(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 tile = gResourceTableEntries[gData_02002200].offset >> 5;
  struct LightOam *oam = Data_02002244;
  s32 viewY = map->origin[1];
  s32 i;

  for (i = 0; i < gData_02002202; i++) {
    struct FieldActor *actor = gData_02002204[i];
    s32 x, y, z, sy, depth, frame;
    u32 *word;

    if (actor == 0 || actor->x == 0)
      continue;
    x = actor->x - viewX;
    y = actor->y - viewY + 0x80000;
    z = actor->z - viewZ - viewY;
    sy = z - y;
    y += z;
    depth = (y >> 16) + 58;
    x >>= 16;
    sy >>= 16;
    if (gData_02002304 == 0) {
      frame = (s16)actor->timer;
      x -= 8;
      sy -= 16;
      if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
        continue;
      x &= 0x1ff;
      sy &= 0xff;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = sy | (x << 16) | 0x40002000;
    } else {
      frame = (s16)actor->timer;
      x -= 8;
      sy -= 64;
      if ((u32)(x + 31) > 350 || sy <= -64 || sy > 175)
        continue;
      x &= 0x1ff;
      sy &= 0xff;
      word = (u32 *)oam;
      *word++ = 0;
      *word++ = sy | (x << 16) | 0xc000a000;
    }
    *word = (tile + frame * 8) | 0x800;
    if (gData_02002306 == -1)
      oam->priority = Func_02001b24(actor);
    else
      oam->priority = gData_02002306;
    Func_020019d4(oam, depth);
    oam++;
  }
}
