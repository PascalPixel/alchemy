// Draft, differ 18 of 200 (tla-en, 5 November 2026): order only.
// Queues the lit actors' sprites: for each of the gData_02002202 actors in
// gData_02002204 with a position, projects it from the map's view (as the cell
// sprite in field/map/cell_sprite.c does), skips it off screen, writes a 16 by
// 16 or 32 by 64 sprite (by gData_02002304) at its animation picture and
// priority (gData_02002306, or Func_02001b24 when that is -1) and pushes it at
// its depth. In 15 overlays of the 698 family.
// Each arm is written whole: jump2 cross-jumps the tails to the native's one
// merge, and the arms' own frame locals and two uses of depth, tile and viewY
// are what give the native's allocation (depth r11, viewY and tile on the
// stack, frame r7, the mode in ip).
// Left: the mode's read belongs after depth is computed, but it must be set
// first for its live length to fall below the first arm's frame; set first it
// also takes r3 for its address, which moves actor->x from r3 to r2. Every
// later position gives the mode r7 and the frame ip.
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
  s32 viewX, viewZ, tile, viewY;
  struct LightOam *oam;
  s32 i;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  tile = gResourceTableEntries[gData_02002200].offset >> 5;
  viewY = map->origin[1];
  oam = Data_02002244;

  for (i = 0; i < gData_02002202; i++) {
    struct FieldActor *actor = gData_02002204[i];
    if (actor != 0 && actor->x != 0) {
      s32 big, ox, y, z, oy, depth;
      u32 *word;
      big = gData_02002304;
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
        if (gData_02002306 == -1)
          oam->priority = Func_02001b24(actor);
        else
          oam->priority = gData_02002306;
        Func_020019d4(oam, depth);
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
        if (gData_02002306 == -1)
          oam->priority = Func_02001b24(actor);
        else
          oam->priority = gData_02002306;
        Func_020019d4(oam, depth);
        oam++;
      }
    }
  }
}
