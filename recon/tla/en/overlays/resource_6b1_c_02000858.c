// Matches: differ 0 (tla-en, 5 November 2026).
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

extern s16 gData_02001cdc;
extern s16 gData_02001cde;
extern struct FieldActor *gData_02001ce0[];
extern struct LightOam gData_02001d20[];
extern s16 gData_02001de0;
extern s16 gData_02001de2;
extern s16 gData_02001de4;
extern u32 gFrameCount;

s32 Func_02000d5c(struct FieldActor *actor);
void Func_02000c3c(struct LightOam *oam, s32 depth);

void Func_02000858(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, picture, tile, viewY;
  struct LightOam *oam;
  s32 i;

  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02001cdc].offset >> 5;
  viewY = map->origin[1];
  oam = gData_02001d20;
  picture = gData_02001de4 * ((gFrameCount >> 2) % 5 * 32 + 32);

  for (i = 0; i < gData_02001cde; i++) {
    struct FieldActor *actor = gData_02001ce0[i];

    if (actor != 0 && actor->x != 0) {
      s32 ox = actor->x - viewX;
      s32 y = actor->y - viewY + 0x80000;
      s32 z = actor->z - viewZ - viewY;
      s32 oy = z - y;
      s32 depth = ((y + z) >> 16) + 58;
      s32 mode = gData_02001de0;
      u32 *word;

      if (mode == 0) {
        s32 frame = (s16)actor->timer;
        s32 x = ox >> 16;
        s32 sy = oy >> 16;
        ox = x - 8;
        oy = sy - 16;
        if ((u32)(x + 7) > 334 || oy <= -16 || oy > 239)
          continue;
        ox &= 0x1ff;
        oy &= 0xff;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = oy | (ox << 16) | (frame & 0x80) << 21 | 0x40002000;
        *word = (tile + picture + (frame & 7) * 8) | 0x800;
        if (gData_02001de2 == -1)
          oam->priority = Func_02000d5c(actor);
        else
          oam->priority = gData_02001de2;
        Func_02000c3c(oam, depth);
        oam++;
      } else {
        s32 frame = (s16)actor->timer;
        s32 x = ox >> 16;
        s32 sy = oy >> 16;
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
        if (gData_02001de2 == -1)
          oam->priority = Func_02000d5c(actor);
        else
          oam->priority = gData_02001de2;
        Func_02000c3c(oam, depth);
        oam++;
      }
    }
  }
}
