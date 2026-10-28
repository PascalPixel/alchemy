#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene.h"
#include "system.h"

#include "vram_tab.h"
#include "ram_buffer.h"

struct AnimTrack {
  u8 unk00[0x10];
  u8 *script;
  u8 index;
};

struct AnimObject {
  u8 unk00[0x18];
  u8 frame;
  u8 unk19[0xf];
  struct AnimTrack *track;
};

void Animation_WaitForTrackEnd(struct AnimObject *object) {
  u8 frame = object->frame;
  struct AnimTrack *track = object->track;
  s32 i;

  if (object != 0) {
    i = 0;
    if (track->script[track->index] != 241) {
      do {
        wait(1);
        i++;
        if (i > 89)
          break;
        if (track->script[track->index] == 241)
          break;
      } while (object->frame == frame);
    }
  }
}

void Map_RenderAnimatedTilePicture(u8 *object, u32 position) {
  u16 *destination;
  s32 count;
  u32 row;
  u8 *table;
  u32 highMask;
  u32 indexMask;
  u8 offsetMask;

  destination =
      (u16 *)(0x06010000 + gResourceTableEntries[object[0x10]].offset);
  count = (object[0x14] * object[0x15]) / 64;
  row = 0;

  if (row < (u32)count) {
    table = gMap_TileDissolveOrder;
    highMask = 0xFF00;
    indexMask = 0x3F;
    offsetMask = 0x3E;

    for (; row < (u32)count; row++, destination += 32, position++) {
      if ((u32)(position - 0x40) <= 0x3F) {
        u8 entry;
        u16 *cell;
        u32 parity;

        entry = table[(position + row * 16) & indexMask];
        cell = (u16 *)((u8 *)destination + (entry & offsetMask));
        parity = 1;
        parity &= entry;
        if (parity != 0)
          *cell = *(u8 *)cell;
        else
          *cell &= highMask;
      }
    }
  }
}

void Map_RenderAnimatedTilePicturesForObject(u8 *object) {
  u32 pos;

  pos = 0;
  do {
    Map_RenderAnimatedTilePicture(object, pos);
    Map_RenderAnimatedTilePicture(object, pos + 1);
    Map_RenderAnimatedTilePicture(object, pos + 2);
    Map_RenderAnimatedTilePicture(object, pos + 3);
    pos += 4;
    wait(1);
  } while (pos <= 0x7f);
}

void Map_RenderAllAnimatedTilePictures(u8 **table, s32 cnt) {
  u8 **top;
  u8 **p;
  s32 n;
  u32 pos;
  u32 pos1;
  u32 pos2;
  u32 pos3;

  top = table;
  pos = 0;
  do {
    if (cnt > 0) {
      pos1 = pos + 1;
      p = top;
      /* FAKEMATCH: ⚓️ reloads the table between the first two positions */
      asm volatile("" : "+l"(p));
      pos2 = pos + 2;
      pos3 = pos + 3;
      n = cnt;
      do {
        Map_RenderAnimatedTilePicture(*p, pos);
        Map_RenderAnimatedTilePicture(*p, pos1);
        Map_RenderAnimatedTilePicture(*p, pos2);
        n -= 1;
        Map_RenderAnimatedTilePicture(*p++, pos3);
      } while (n != 0);
    }
    wait(1U);
    pos += 4;
  } while (pos <= 0x7FU);
}

struct Thing {
  u8 filler0[8];
  s32 field8;
  u8 filler12[4];
  s32 field16;
};

s32 Map_GetScreenRelativePosition(struct Thing *obj, s32 *out) {
  u8 *state = (u8 *)Ram_HeapSlots->mapWork;
  s32 *org = (s32 *)(state + 228);
  s32 a;
  s32 b;
  s32 x;
  s32 y;

  a = org[0] & 0xffff0000;
  b = org[1] & 0xffff0000;
  x = obj->field8 - a;
  y = obj->field16 - b;
  if ((u32)(x + 0x001fffff) <= 0x012ffffe && y > 0 && y < 0xe00000) {
    *out++ = x >> 16;
    *out = y >> 16;
    return 0;
  }
  *out++ = 0;
  *out = 0;
  return -1;
}
