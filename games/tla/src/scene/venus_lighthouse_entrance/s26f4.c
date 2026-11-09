#include "types.h"
#include "ram_buffer.h"
#include "event_inline.h"

struct SparkState {
  u8 unk00[24];
  s32 frame;
};

struct SparkSprite {
  u8 unk00[4];
  u16 attr0 : 13;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkWork {
  u8 unk0000[0x800];
  struct SparkState states[96];
  struct SparkSprite sprites[96];
  u16 unk2180;
  u16 tileBase;
  u16 resource;
};

extern char ResourceId_DustPuffA;

void *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
void Func_02002824(void *source, void *destination);
s32 Resource_FindFreeEntry(void);
void Func_02002a0c(struct SparkSprite *sprite, s32 width, s32 height, s32 attr,
                   s32 tile);
void Func_0200258c(void);

void Func_020026f4(void) {
  struct SparkWork *work = HeapSlot_AllocFar(220, 0x2188);
  s32 resource;
  s32 tileBase;
  s32 i; struct SparkState *state; struct SparkSprite *sprite;

  Func_02002824(Resource_GetTableEntry((s32)&ResourceId_DustPuffA), work);
  resource = Resource_FindFreeEntry();
  tileBase = Vram_Load(resource, 0x800, work);
  work->tileBase = tileBase;
  work->resource = resource;
  state = work->states;
  sprite = work->sprites;
  for (i = 95; i >= 0; i--) {
    Func_02002a0c(sprite, 8, 8, 0x40000000, tileBase);
    sprite->fullColor = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    state->frame = -1;
    state++;
    sprite++;
  }
  { s32 zero = 0; work->unk2180 = zero; }
  Task_AddFrame(Func_0200258c, (s32)(0x480));
}
