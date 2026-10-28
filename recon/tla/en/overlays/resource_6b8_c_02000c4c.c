// Overlay 6b8's copy of resource_694_c_02003e54.c, with this overlay's symbols:
// differ 86 of 112. That draft's header follows. Draft, differ 86 of 112: sets
// up an object's four 16 by 16 sprites in work: loads their tiles
// (gData_02000f44, 256 bytes) into a free VRAM entry, resets the work's
// counters (999, 999, 0), links object and work both ways, hooks the object's
// update (Func_02000ad0) and gives each sprite the object's priority.
#include "types.h"
#include "actor.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 offset; ///< in bytes; its tile is offset / 32
};

struct SparkleSprite {
  u8 unk00[5];
  u8 attr5;
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

extern const u8 gData_02000f44[];
extern struct ResourceTableEntry gResourceTableEntries[];

s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, const void *data);
void Func_02000ee4(struct SparkleSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);
s32 Func_02000ef4(struct Actor *object);
void Func_02000ad0(void);

void Func_02000c4c(struct Actor *object, struct SparkleWork *work) {
  struct SparkleSprite *sprite;
  s32 tile;
  s32 row;
  s32 col;

  work->vramEntry = Resource_FindFreeEntry();
  VramCache_Load(work->vramEntry, 256, gData_02000f44);
  work->unka6 = 0;
  work->unka0 = 999;
  work->unka2 = 999;
  work->object = object;
  ((u8 *)object)[0x62] = 0;
  ((u8 *)object)[0x63] = 0;
  object->update = Func_02000ad0;
  tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  object->watchedObject = (struct Actor *)work;
  for (row = 0; row <= 1; row++) {
    sprite = &work->sprites[row * 2];
    for (col = 1; col >= 0; col--) {
      Func_02000ee4(sprite, 16, 16, 0x80004000, tile);
      ((u8 *)sprite)[5] &= ~32;
      ((u8 *)sprite)[9] &= 15;
      sprite->priority = Func_02000ef4(object) & 3;
      sprite++;
    }
  }
}
