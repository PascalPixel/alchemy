/// @file
/// Sets up the sparkle: loads its picture and its four 16 by 16 sprites, and hands the
/// object to the update in s3cd8.c.
#include "types.h"
#include "actor.h"
#include "event_inline.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 offset; ///< in bytes; its tile is offset / 32
};

struct SparkleSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 blend : 1;
  u8 unk05_6 : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x1e];
};

struct SparkleWork {
  struct SparkleSprite sprites[4];
  u16 unka0;
  u16 unka2;
  s16 vramEntry;
  u16 unka6;
  struct Actor *object;
};

extern const u8 gData_020042bc[];
extern struct ResourceTableEntry gResourceTableEntries[];

s32 Resource_FindFreeEntry(void);
void Func_02004244(struct SparkleSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);
s32 Func_02004254(struct Actor *object);
void Func_02003cd8(void);

void Func_02003e54(struct Actor *object, struct SparkleWork *work) {
  struct SparkleSprite *sprite;
  s32 tile;
  s32 row;
  s32 col;

  work->vramEntry = Resource_FindFreeEntry();
  VramCache_Load(work->vramEntry, 256, gData_020042bc);
  work->unka6 = 0;
  work->unka0 = 999;
  work->unka2 = 999;
  work->object = object;
  ((u8 *)object)[0x62] = 0;
  ((u8 *)object)[0x63] = 0;
  object->update = Func_02003cd8;
  tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  object->watchedObject = (struct Actor *)work;
  for (row = 0; row <= 1; row++) {
    for (col = 1, sprite = work->sprites + row + row; col >= 0; col--) {
      Func_02004244(sprite, 16, 16, 0x80004000, tile);
      sprite->blend = 0;
      sprite->palette = 0;
      sprite->priority = Func_02004254(object);
      sprite++;
    }
  }
}
