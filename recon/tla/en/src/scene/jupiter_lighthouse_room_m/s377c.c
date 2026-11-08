/// @file
/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
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

extern s16 gData_02005a80;
extern s16 gData_02005a82;
extern struct FieldActor *gData_02005a84[];
extern struct LightOam Data_02005ac4[];
extern s16 gData_02005b84;
extern s16 gData_02005b86;

s32 Func_0200499c(struct FieldActor *actor);
void Func_020046ac(struct LightOam *oam, s32 depth);

void Func_0200377c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02005a80].offset >> 5;
  viewY = map->origin[1];
  oam = Data_02005ac4;

  for (i = 0; i < gData_02005a82; i++) {
    struct FieldActor *actor = gData_02005a84[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      s32 frame;
      u32 *word;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      big = gData_02005b84;
      if (big == 0) {
        s32 x, sy;
        frame = (s16)actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 16;
        if ((u32)(x + 7) > 334 || oy <= -16 || oy > 239)
          continue;
        ox &= SPLIT_CONSTANT(0x1ff);
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0x40002000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_02005b86 == -1)
          oam->priority = Func_0200499c(actor);
        else
          oam->priority = gData_02005b86;
        Func_020046ac(oam, depth);
        oam++;
      } else {
        s32 x, sy;
        frame = (s16)actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 64;
        if ((u32)(x + 23) > 350 || oy <= -64 || oy > 175)
          continue;
        ox &= SPLIT_CONSTANT(0x1ff);
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0xc000a000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_02005b86 == -1)
          oam->priority = Func_0200499c(actor);
        else
          oam->priority = gData_02005b86;
        Func_020046ac(oam, depth);
        oam++;
      }
    }
  }
}
