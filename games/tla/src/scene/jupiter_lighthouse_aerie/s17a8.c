#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"
#include "event_inline.h"

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

extern char ResourceId_StarSparkleFade;
extern char ResourceId_GoldenTrident;

void *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void *Resource_GetTableEntry(s32 id);
void Func_020029a0(void *resource, void *destination);
s32 Resource_FindFreeEntry(void);
void Resource_ResetEntry(s32 entry);
void Func_02002b30(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                   u32 attributes, s32 tile);
void Func_02002a20(struct Actor *object);
void Func_0200105c(struct Actor *object);
void Func_02001080(void);

static __inline__ void SetUp(struct EffectSprite *sprite, s32 pivotX,
                             s32 pivotY, u32 attributes, s32 tile) {
  /* FAKEMATCH(wrapper): the setup forwarded inline; as a direct call, 2 of 316
   * instructions differ in tla-en. */
  Func_02002b30(sprite, pivotX, pivotY, attributes, tile);
}

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
  s32 age;

  work->first = Actor_GetById(a);
  work->second = Actor_GetById(b);
  Func_020029a0(Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade), work);
  smallEntry = Resource_FindFreeEntry();
  smallTile = VramCache_Load(smallEntry, 256, work);
  work->smallTile = smallTile;
  spark = work->sparks;
  age = 0;
  sprite = work->sprites;
  i = 31;
  for (; i >= 0; age -= 2, i--, sprite++, spark++) {
    Func_02002b30(sprite, 4, 4, 0, smallTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    *(u16 *)&sprite->depth = 240;
    spark->age = age;
  }
  Func_020029a0(Resource_GetTableEntry((s32)&ResourceId_GoldenTrident), work);
  largeEntry = Resource_FindFreeEntry();
  {
    /* FAKEMATCH(temporary): sprite also holds the size; as a plain argument, 7
     * of 316 instructions differ in tla-en. */
    sprite = (struct EffectSprite *)512;
    largeTile = VramCache_Load(largeEntry, (s32)sprite, work);
  }
  work->largeTile = largeTile;
  large = &work->large;
  SetUp(large, 30, 7, 0x80004000, largeTile);
  *(u16 *)&large->depth = 240;
  large->oam.priority = 1;
  large->oam.palette256 = 1;
  large->oam.palette = 0;
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
  Actor_SetMoveTarget(carrier, work->start[0], work->start[1], work->start[2]);
  Sound_Play(142);
  Func_0200105c(work->first);
  Event_Wait(40);
  work->side = side;
  work->step = 0;
  work->unkb02 = 0;
  work->unkb09 = 1;
  work->unkb0a = 0;
  Task_Add(Func_02001080, 0x480);
  while (work->step != 999)
    wait(1);
  Task_RemoveFar(Func_02001080);
  Resource_ResetEntry(largeEntry);
  Resource_ResetEntry(smallEntry);
  HeapSlot_Release(220);
}
