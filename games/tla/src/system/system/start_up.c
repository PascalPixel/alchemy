/// @file
/// Power-up, the cartridge's language, the build stamp and code loading.
// Part of system.c.

#include "../system_internal.h"
#include "ram_code.h"
#include "gamepak/far_call/field_sequence.h"
#include "gamepak/far_call/sound.h"
#include "resource_ids.h"

void System_Initialize(void) {
  {
    volatile u16 *dma0 = REG_DMA0;

    dma0[5] &= 0xc5ff;
    dma0[5] &= 0x7fff;
    dma0[5];
  }
  Io_Write16(WAITCNT_GAME, &REG_WAITCNT);
  Dma_Fill32(0, (void *)IWRAM_START, 0x7800);
  Heap_Initialize();
  System_InstallIwramAndIrqs();
  *(u32 *)&gIoWriteQueue.count = 0;
  gDebugTextEnabled = 0;
  gDebugMode = 0;
  gCpuLoadDisplayEnabled = 0;
  System_CopyBuildStamp();
  Window_LoadGraphics();
  DebugText_Clear();
  REG_DISPCNT = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
  Irq_SetHandler(0, 1, System_VBlankHandler);
  Irq_SetHandler(13, 1, Iwram_Halt);
  Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
  Sound_InitializeFar();
  VramCache_Reset();
  Task_ResetAll();
  gCartridgeResetMarker = 0;
  gRenderOamEnabled = 1;
  gOamUsage.limit = 0x100;
  gOamUsage.used = 0;
  gSleepDisabled = 0;
  wait(10);
  MainRoutine_RunFar(0);
}

void System_IdleForever(void) {
  volatile struct InputState *input = &gInput;

  for (;;) {
    input->pressed;
    wait(1);
  }
}

#if (LANGUAGE != LANGUAGE_JAPANESE) && (LANGUAGE != LANGUAGE_ENGLISH)
/// Returns the language of this cartridge from the last letter of the game
/// code in its header, as an enum CartridgeLanguage. Only the German,
/// Spanish, French and Italian editions have it.
s32 System_GetLanguage(void) {
  s32 language;

  switch (Cart_LanguageCode) {
  case 'D':
    language = CARTRIDGE_LANGUAGE_GERMAN;
    break;
  case 'F':
    language = CARTRIDGE_LANGUAGE_FRENCH;
    break;
  case 'I':
    language = CARTRIDGE_LANGUAGE_ITALIAN;
    break;
  case 'S':
    language = CARTRIDGE_LANGUAGE_SPANISH;
    break;
  default:
    language = CARTRIDGE_LANGUAGE_ENGLISH;
    break;
  }
  return language;
}
#endif

void RuntimeDispatch_ReservedNoOpA(void) {}

void RuntimeDispatch_ReservedNoOpB(void) {}

void RuntimeDispatch_ReservedNoOpC(void) {}

void RuntimeDispatch_ReservedNoOpD(void) {}

void RuntimeDispatch_ReservedNoOpE(void) {}

s32 Stub_ReturnZero(s32 unused0, const u8 *unused1) { return 0; }

/// Copies the build stamp resource (three words) into gBuildStamp and
/// writes the zero word that ends it. Called at power-up, before the window
/// graphics load.
void System_CopyBuildStamp(void) {
  Dma_Set(Resource_GetTableEntry((s32)&ResourceId_BuildStamp), gBuildStamp,
          DMA_COPY_WORDS(3), REG_DMA3);
  gBuildStamp[3] = 0;
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

void Overlay_Load(u32 index, void *destination) {
  void (*routine)(void *, s32);
  s32 end;

  end = Decompress_TaggedLz((const void *)Resource_GetTableEntry(index),
                            destination);
  RamCode_Load(routine, Resource_PatchThumbBranchCode,
               Resource_PatchThumbBranchCodeSize);
  routine(destination, end);
  Heap_Release(routine);
}

void Irq_Ignore(void) {}

void System_InstallIwramAndIrqs(void) {
  Io_Write16(0, &REG_IME);
  Dma_Set(IwramResident_Rom, Ram_IwramIrqMain, DMA_COPY_WORDS(1024), REG_DMA3);
  *Ram_IrqVector = IwramIrqMain;
  Dma_Set(gDefaultIrqHandlers, gIrqHandlers, DMA_COPY_WORDS(IRQ_COUNT),
          REG_DMA3);
  Io_Write16(0, &REG_DISPSTAT);
  Io_Write16(0xc3ff, &REG_KEYCNT);
  Io_Write16(INTR_FLAG_VBLANK | INTR_FLAG_KEYPAD | INTR_FLAG_GAMEPAK, &REG_IE);
  Io_Write16(1, &REG_IME);
}
