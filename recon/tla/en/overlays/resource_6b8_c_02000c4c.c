// Draft, differ 77 of 112, skeleton gap 29 (tla-en, 2026-11-01)
// Overlay 6b8's copy of resource_694_c_02003e54.c. Sets up an object's four
// 16 by 16 sprites in work: loads their tiles (gData_02000f44, 256 bytes) into
// a free VRAM entry, resets the work's counters (999, 999, 0), links object and
// work both ways, hooks the object's update (Func_02000ad0) and gives each
// sprite the object's priority.
// Left, all allocation: the listing's pool 0 is the HI mask of work's 16-bit
// store (reg 43, the ldr loaded into r10), and the hi register forces the early
// pool island (b over it, HI pool range). The draft gives that pseudo and
// &vramEntry to local-alloc, so r6 is taken and object falls to sl; the listing
// has both global (object r6, column r8), which needs a block split before the
// ldrsh that combine later removes. tile as a signed offset / 32 makes that
// split, but after the ldrsh, so only the zero goes global (no gain). The -33
// mask is hoisted on loop's second pass (28 real insns, threshold 14), losing
// the listing's adds copy; a 29th insn is needed. Tried: loop forms, oam
// pointer, priority and tile temporaries; none moved the skeleton.
#include "types.h"
#include "actor.h"
#include "effect_sprite.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 offset; ///< in bytes; its tile is offset / 32
};

struct SparkleWork {
  struct EffectSprite sprites[4];
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
void Func_02000ee4(struct EffectSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);
s32 Func_02000ef4(struct Actor *object);
void Func_02000ad0(void);

void Func_02000c4c(struct Actor *object, struct SparkleWork *work) {
  struct EffectSprite *sprite;
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
    for (col = 0; col < 2; col++) {
      sprite = &work->sprites[row * 2 + col];
      Func_02000ee4(sprite, 16, 16, 0x80004000, tile);
      sprite->oam.palette256 = 0;
      sprite->oam.palette = 0;
      sprite->oam.priority = Func_02000ef4(object);
    }
  }
}
