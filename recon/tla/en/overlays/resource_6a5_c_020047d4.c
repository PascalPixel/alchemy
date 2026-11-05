// Draft, differ 18: overlay 6a5's copy of the function drafted in
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

extern s16 gData_02006a6c;
extern s16 gData_02006a6e;
extern struct FieldActor *gData_02006a70[];
extern struct LightOam Data_02006ab0[];
extern s16 gData_02006b70;
extern s16 gData_02006b72;

s32 Func_020059bc(struct FieldActor *actor);
void Func_0200571c(struct LightOam *oam, s32 depth);

void Func_020047d4(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02006a6c].offset >> 5;
  viewY = map->origin[1];
  oam = Data_02006ab0;

  for (i = 0; i < gData_02006a6e; i++) {
    struct FieldActor *actor = gData_02006a70[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      u32 *word;
      big = gData_02006b70;
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
        if (gData_02006b72 == -1)
          oam->priority = Func_020059bc(actor);
        else
          oam->priority = gData_02006b72;
        Func_0200571c(oam, depth);
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
        if (gData_02006b72 == -1)
          oam->priority = Func_020059bc(actor);
        else
          oam->priority = gData_02006b72;
        Func_0200571c(oam, depth);
        oam++;
      }
    }
  }
}
