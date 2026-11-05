// Draft, differ 18: overlay 6ad's copy of the function drafted in
// resource_698_c_02000aac.c, whose header says what it is and what still differs.
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

extern s16 gData_020074cc;
extern s16 gData_020074ce;
extern struct FieldActor *gData_020074d0[];
extern struct LightOam Data_0200f510[];
extern s16 gData_020075d0;
extern s16 gData_020075d2;

s32 Func_02005698(struct FieldActor *actor);
void Func_02005440(struct LightOam *oam, s32 depth);

void Func_02005048(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_020074cc].offset >> 5;
  viewY = map->origin[1];
  oam = Data_0200f510;

  for (i = 0; i < gData_020074ce; i++) {
    struct FieldActor *actor = gData_020074d0[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      u32 *word;
      big = gData_020075d0;
      ox = actor->x - viewX;
      y = actor->y - viewY + 0x80000;
      z = actor->z - viewZ - viewY;
      oy = z - y;
      depth = ((y + z) >> 16) + 58;
      if (big == 0) {
        s32 x, sy;
        s32 frame;
        frame = (s16)actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 16;
        if ((u32)(x + 7) > 334 || oy <= -16 || oy > 239)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0x40002000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_020075d2 == -1)
          oam->priority = Func_02005698(actor);
        else
          oam->priority = gData_020075d2;
        Func_02005440(oam, depth);
        oam++;
      } else {
        s32 x, sy;
        s32 frame;
        frame = (s16)actor->timer;
        x = ox >> 16;
        sy = oy >> 16;
        ox = x - 8;
        oy = sy - 64;
        if ((u32)(x + 23) > 350 || oy <= -64 || oy > 175)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | 0xc000a000;
        *word = (tile + frame * 8) | 0x800;
        if (gData_020075d2 == -1)
          oam->priority = Func_02005698(actor);
        else
          oam->priority = gData_020075d2;
        Func_02005440(oam, depth);
        oam++;
      }
    }
  }
}
