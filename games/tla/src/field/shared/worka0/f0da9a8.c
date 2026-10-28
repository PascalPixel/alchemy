#include "types.h"
#include "dma.h"

// The four 256-byte frames of each set (type-01 compressed).
extern const u8 Data_080f0c1c[], Data_080f0c63[], Data_080f0cb8[];
extern const u8 Data_080f0d07[], Data_080f0d5e[], Data_080f0dbb[];
extern const u8 Data_080f0e00[];

void *Runtime_AllocateBlock(s32 slot, s32 size);
void *MemAlloc(s32 size);
void MemFree(void *block);
s32 Resource_FindFreeEntry(void);
void Resource_DecodeType01(const void *source, void *destination);
s32 VramCache_Load(s32 slot, s32 size, void *tiles);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_080daab0(void);
void Func_080dacb8(void);

struct TileEffectWork {
  s32 vramEntry;
  u8 unk04[0x10e0];
};

#define TILE_EFFECT_SLOT 164
#define TILE_EFFECT_KIND_ALT 372 // the kind that takes the first frame set
#define FRAME_SIZE 0x100

/// Starts a field effect that animates sixteen tiles of its own: takes a
/// 0x10e4-byte work block at heap offset 164 (worka0.h's WorkA4 lies there),
/// clears it, decodes four 256-byte tile frames into a scratch buffer (one set
/// for kind 372, another for the rest; the second set repeats one frame), loads
/// them into a VRAM slot and schedules the two per-frame routines Func_080daab0
/// and Func_080dacb8.
struct TileEffectWork *Func_080da9a8(s32 kind) {
  struct TileEffectWork *work;
  u8 *frames;
  s32 size;

  work = Runtime_AllocateBlock(TILE_EFFECT_SLOT, sizeof(struct TileEffectWork));
  size = 4 * FRAME_SIZE;
  frames = MemAlloc(size);
  Dma_Fill32(0, work, sizeof(struct TileEffectWork));
  work->vramEntry = Resource_FindFreeEntry();
  if (kind == TILE_EFFECT_KIND_ALT) {
    Resource_DecodeType01(Data_080f0c1c, frames);
    Resource_DecodeType01(Data_080f0c63, frames + FRAME_SIZE);
    Resource_DecodeType01(Data_080f0cb8, frames + 2 * FRAME_SIZE);
    Resource_DecodeType01(Data_080f0d07, frames + 3 * FRAME_SIZE);
    VramCache_Load(work->vramEntry, size, frames);
    /* FAKEMATCH(barrier): the empty asm closes the branch, so each decode
     * sets its destination before loading its source; as plain statements, 4
     * of 95 instructions differ in tla-en. */
    asm("");
  } else {
    Resource_DecodeType01(Data_080f0d5e, frames);
    Resource_DecodeType01(Data_080f0dbb, frames + FRAME_SIZE);
    Resource_DecodeType01(Data_080f0dbb, frames + 2 * FRAME_SIZE);
    Resource_DecodeType01(Data_080f0e00, frames + 3 * FRAME_SIZE);
    VramCache_Load(work->vramEntry, size, frames);
    /* FAKEMATCH(barrier): the empty asm closes the branch, so each decode
     * sets its destination before loading its source; as plain statements, 4
     * of 95 instructions differ in tla-en. */
    asm("");
  }
  {
    s32 priority = 0x480;

    Scheduler_AddOrUpdateCallback(Func_080daab0, priority);
  }
  Scheduler_AddOrUpdateCallback(Func_080dacb8, 0x48f);
  MemFree(frames);
  return work;
}
