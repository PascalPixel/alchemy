// Draft, differ 97 of 316, skeleton gap 18 removed, 17 added (tla-en, 2026-11-01)
//
// The Jupiter Lighthouse aerie's effect between objects a and b: allocates its
// work in heap slot 220, loads its two pictures (resources 0x1eb and 0x1f6)
// into VRAM, sets up 32 small sprites and one large one, picks its start and
// end points by side, points the event's object (event work word 460) at them
// (Func_02002a20, Func_02002a28), plays sound 142, runs Func_02001080 as a
// scheduled callback until its step counter reaches 999, then frees the VRAM
// entries and the work.
//
// Left: the second CSE pass carries the loop body's constants past the loop
// end, so the 32 reaches the large sprite through a call-saved register and the
// loop counter spills to the stack (frame 20, listing 12). The listing rebuilds
// 32 and 240 at the large sprite, so something ends its block at the loop;
// nothing in the listing shows what. Everything else left follows from that:
// the counter in r8, tile in r9, the 240 pool load in the loop, and the
// reload-cse adds on -13 and 32. 512 in a call-saved register copied to r1
// before VramCache_Load is also unexplained.
//
// Measured: a halfword store through a field goes through store_bit_field
// (direct_store[HImode] is 0 on Thumb) and its constant ends up as SImode movs;
// through a pointer it stays HImode and comes from the pool, which is how the
// listing loads 240 at both sites. Cast at both, CSE shares it and the gap
// rises to 45, so only the large sprite's store is cast. Bitfields at the large
// sprite (priority, palette) share 15 and -13 with the loop: the byte form with
// keepPriority keeps them apart, and gives the single load the listing has.
// Both branches of the side test store all six words: their tails cross-jump
// into the listing's shared to[1], to[2] with the constants in r0 and r1. The
// wait is a goto loop because a loop with notes hoists 999 and the step's
// address, which the listing rebuilds on each pass.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"

struct BeamSpark {
  u8 unk00[0x18];
  s32 age;
};

struct BeamWork {
  u8 tiles[0x200];
  struct EffectSprite sprites[32];
  struct BeamSpark sparks[32];
  struct EffectSprite large;
  u8 unkaa8[0x1c];
  s32 from[3];
  s32 to[3];
  s32 start[3];
  u8 unkae8[0x18];
  s16 step;
  s16 unkb02;
  u16 largeTile;
  u16 smallTile;
  u8 unkb08;
  u8 unkb09;
  u8 unkb0a;
  u8 side;
  struct Actor *first;
  struct Actor *second;
};

extern char ResourceId_Data1EB;
extern char ResourceId_Data1F6;

void *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
struct Actor *Actor_GetById(s32 id);
void *Resource_GetTableEntry(s32 id);
void Func_020029a0(void *resource, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Resource_ResetEntry(s32 entry);
void Func_02002b30(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                   u32 attributes, s32 tile);
void Func_02002a20(struct Actor *object);
void Func_02002a28(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002b68(s32 sound);
void Func_0200105c(struct Actor *object);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void Func_02001080(void);

void Func_020017a8(s32 a, s32 b, s32 side) {
  struct BeamWork *work = HeapSlot_AllocFar(220, sizeof(struct BeamWork));
  struct Actor *carrier =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 460);
  struct EffectSprite *sprite;
  struct EffectSprite *large;
  struct BeamSpark *spark;
  s32 smallEntry;
  s32 largeEntry;
  s32 smallTile;
  s32 largeTile;
  s32 i;

  work->first = Actor_GetById(a);
  work->second = Actor_GetById(b);
  Func_020029a0(Resource_GetTableEntry((s32)&ResourceId_Data1EB), work);
  smallEntry = Resource_FindFreeEntry();
  smallTile = VramCache_Load(smallEntry, 256, work);
  work->smallTile = smallTile;
  spark = work->sparks;
  sprite = work->sprites;
  for (i = 0; i < 32; i++, sprite++, spark++) {
    Func_02002b30(sprite, 4, 4, 0, smallTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    spark->age = -2 * i;
  }
  Func_020029a0(Resource_GetTableEntry((s32)&ResourceId_Data1F6), work);
  largeEntry = Resource_FindFreeEntry();
  largeTile = VramCache_Load(largeEntry, 512, work);
  work->largeTile = largeTile;
  large = &work->large;
  Func_02002b30(large, 30, 7, 0x80004000, largeTile);
  *(u16 *)&large->depth = 240;
  {
    s32 keepPriority = ~12;

    ((u8 *)&large->oam)[1] |= 32;
    ((u8 *)&large->oam)[5] = ((((u8 *)&large->oam)[5] & keepPriority) | 4) & 15;
  }
  large->scaleX = 0;
  large->scaleY = 0;
  if (side == 0) {
    work->from[0] = 152 << 16;
    work->from[1] = 0x200000;
    work->from[2] = 164 << 16;
    work->to[0] = 235 << 17;
    work->to[1] = 0x200000;
    work->to[2] = 164 << 16;
  } else {
    work->from[0] = 218 << 18;
    work->from[1] = 0x200000;
    work->from[2] = 164 << 16;
    work->to[0] = 0x2260000;
    work->to[1] = 0x200000;
    work->to[2] = 164 << 16;
  }
  work->start[0] = work->from[0];
  work->start[1] = work->from[1];
  work->start[2] = work->from[2];
  carrier->speedLimit = 0x8000;
  carrier->acceleration = 0x8000;
  Func_02002a20(carrier);
  Func_02002a28(carrier, work->start[0], work->start[1], work->start[2]);
  Func_02002b68(142);
  Func_0200105c(work->first);
  FieldEvent_Wait(40);
  work->side = side;
  work->step = 0;
  work->unkb02 = 0;
  work->unkb09 = 1;
  work->unkb0a = 0;
  Task_Add(Func_02001080, 0x480);
  if (work->step != 999) {
  again:
    wait(1);
    if (work->step != 999)
      goto again;
  }
  Task_RemoveFar(Func_02001080);
  Resource_ResetEntry(largeEntry);
  Resource_ResetEntry(smallEntry);
  HeapSlot_Release(220);
}
