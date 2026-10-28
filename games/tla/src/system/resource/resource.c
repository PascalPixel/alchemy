#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "dma.h"
#include "mem.h"
#include "types.h"
#include "io_reg.h"

extern s32 gIwramHi_Data1[];
extern char ResourceId_BuildStamp;

void Func_080132d0(void) {
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_BuildStamp), gIwramHi_Data1,
          0x84000003, REG_DMA3);
  gIwramHi_Data1[3] = 0;
}

void Resource_ReservedNoOp(s32 count) {}

void *Resource_GetTableEntry(s32 index) {
  return Resource_DirectoryTable[index];
}

void Code_ResolveLongBranches(u16 *entries, s32 byteLength) {
  s32 entryCount;
  s32 entryIndex;

  entryCount = byteLength >> 1;
  entryIndex = 1;
  entries++;
  if (entryIndex < entryCount) {
    u32 nextMarkerMask;
    u32 currentMarkerMask;
    u32 payloadMask;
    nextMarkerMask = 0xf800;
    payloadMask = 0x7ff;
    currentMarkerMask = 0xf000;
    entries -= 2;
    do {
      u16 currentEntry;
      u16 nextEntry;
      u32 nextMarkerBits;
      u32 currentMarkerBits;

      nextEntry = entries[2];
      nextMarkerBits = nextEntry & nextMarkerMask;
      entries++;
      if (nextMarkerBits == nextMarkerMask) {
        currentEntry = entries[0];
        currentMarkerBits = currentEntry & nextMarkerBits;
        if (currentMarkerBits == currentMarkerMask) {
          u32 packedOffset;
          u32 adjustedCurrent;
          u32 adjustedNext;

          packedOffset = (((currentEntry & payloadMask) << 12) |
                          ((nextEntry & payloadMask) * 2)) -
                         (entryIndex * 2);
          adjustedCurrent =
              ((packedOffset >> 12) & payloadMask) | currentMarkerBits;
          adjustedNext = ((packedOffset >> 1) & payloadMask) | nextMarkerBits;
          entries[0] = adjustedCurrent;
          entries[1] = adjustedNext;
        }
      }
      entryIndex++;
    } while (entryIndex < entryCount);
  }
}

void Resource_LoadCode(u32 index, void *destination) {
  void (*routine)(void *, s32);
  s32 end;
  u32 size;

  end = Resource_DecodeType01((const void *)Resource_GetTableEntry(index),
                              destination);
  /* FAKEMATCH: the wrapper keeps the size load after the decode. */
  do {
    size = (u32)Resource_PatchThumbBranchCodeSize;
  } while (0);
  routine = (void (*)(void *, s32))Heap_Alloc(size);
  Dma_Set(Resource_PatchThumbBranchCode, routine, 0x84000000 | (size >> 2),
          REG_DMA3);
  routine(destination, end);
  Heap_Release(routine);
}

void Irq_Ignore(void) {}
