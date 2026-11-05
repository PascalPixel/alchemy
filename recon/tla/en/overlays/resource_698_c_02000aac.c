// Measured under -Os, 5 November 2026: differ 128 (tla-en); notes below may predate it.
// Draft of overlay 698's Func_02000aac, in 11 overlays of the 698 family:
// differ 187 of 200. Queues the lit actors' sprites: for each of the
// gData_02002202 actors in gData_02002204 with a position, projects it from
// the map's view (as the cell sprite in field/map/cell_sprite.c does),
// skips it off screen, writes a 16 by 16 or 32 by 64 sprite (by
// gData_02002304) at its animation picture and priority (gData_02002306, or
// Func_02001b24 when that is -1) and pushes it at its depth. Written in the
// cell sprite's shape (271 in the earlier form). Left: register allocation;
// the native keeps the view x and z in r10 and r8, the index in r9 and the
// depth in r11 and spills the view height and tile, where this spills the
// view x and tile. Tried: the view height read first, a goto loop.
#include "types.h"
#include "call.h"
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

    if (actor != 0 && actor->x != 0) {
      s32 px = actor->x - viewX;
      s32 y = actor->y - viewY + 0x80000;
      s32 z = actor->z - viewZ - viewY;
      s32 py = z - y;
      s32 depth = ((y + z) >> 16) + 58;
      s32 x = px >> 16;
      s32 sy = py >> 16;
      s32 frame;
      u32 *word;

      if (gData_02002304 == 0) {
        frame = (s16)actor->timer;
        px = x - 8;
        py = sy - 16;
        if ((u32)(x + 7) > 334 || py <= -16 || py > 239)
          continue;
        px &= 0x1ff;
        py &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = py | (px << 16) | 0x40002000;
      } else {
        frame = (s16)actor->timer;
        px = x - 8;
        py = sy - 64;
        if ((u32)(x + 23) > 350 || py <= -64 || py > 175)
          continue;
        px &= 0x1ff;
        py &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = py | (px << 16) | 0xc000a000;
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
}
