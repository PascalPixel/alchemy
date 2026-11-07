#include "types.h"
#include "dma.h"

// The four 256-byte pictures of each set (type-01 compressed).
extern const u8 Data_080f0c1c[], Data_080f0c63[], Data_080f0cb8[];
extern const u8 Data_080f0d07[], Data_080f0d5e[], Data_080f0dbb[];
extern const u8 Data_080f0e00[];

void *HeapSlot_AllocEwram(s32 slot, s32 size);
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
s32 Resource_FindFreeEntry(void);
void Resource_DecodeType01(const void *source, void *destination);
s32 VramCache_Load(s32 slot, s32 size, void *tiles);
void Task_Add(void (*func)(void), s32 order);
void Func_080daab0(void);
void Func_080dacb8(void);

struct TileEffectWork {
  s32 vramEntry;
  u8 unk04[0x10e0];
};

#define TILE_EFFECT_SLOT 164
#define TILE_EFFECT_KIND_ALT 372 // the kind that takes the first picture set
#define PICTURE_SIZE 0x100

/// Starts a field effect that animates sixteen tiles of its own: takes a
/// 0x10e4-byte work block at heap offset 164 (worka0.h's WorkA4 lies there),
/// clears it, decodes four 256-byte tile pictures into a scratch buffer (one
/// set for kind 372, another for the rest; the second set repeats one
/// picture), loads them into a VRAM slot and schedules the two per-frame
/// routines Func_080daab0 and Func_080dacb8.
struct TileEffectWork *Func_080da9a8(s32 kind) {
  struct TileEffectWork *work;
  u8 *pictures;
  s32 size;

  work = HeapSlot_AllocEwram(TILE_EFFECT_SLOT, sizeof(struct TileEffectWork));
  size = 4 * PICTURE_SIZE;
  pictures = Heap_Alloc(size);
  Dma_Fill32(0, work, sizeof(struct TileEffectWork));
  work->vramEntry = Resource_FindFreeEntry();
  if (kind == TILE_EFFECT_KIND_ALT) {
    Resource_DecodeType01(Data_080f0c1c, pictures);
    Resource_DecodeType01(Data_080f0c63, pictures + PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0cb8, pictures + 2 * PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0d07, pictures + 3 * PICTURE_SIZE);
    VramCache_Load(work->vramEntry, size, pictures);
    /* FAKEMATCH(barrier): the empty asm closes the branch, so each decode
     * sets its destination before loading its source; as plain statements, 4
     * of 95 instructions differ in tla-en. */
    asm("");
  } else {
    Resource_DecodeType01(Data_080f0d5e, pictures);
    Resource_DecodeType01(Data_080f0dbb, pictures + PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0dbb, pictures + 2 * PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0e00, pictures + 3 * PICTURE_SIZE);
    VramCache_Load(work->vramEntry, size, pictures);
    /* FAKEMATCH(barrier): the empty asm closes the branch, so each decode
     * sets its destination before loading its source; as plain statements, 4
     * of 95 instructions differ in tla-en. */
    asm("");
  }
  {
    s32 priority = 0x480;

    Task_Add(Func_080daab0, priority);
  }
  Task_Add(Func_080dacb8, 0x48f);
  Heap_Release(pictures);
  return work;
}
