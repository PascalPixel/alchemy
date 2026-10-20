/// @file
/// Power-up, the cartridge's language, the build stamp and code loading.
/// Part of runtime.c, the runtime object, compiled only through it.

#include "../runtime_internal.h"

#if TBS_PATCH_2
#define SYSTEM_HALT_ON_GAMEPAK_IRQ 1
#endif

/// Powers the game up: stops any HBlank DMA left running, sets the cartridge
/// wait states, clears IWRAM, installs the interrupt handlers and the resident
/// IWRAM code, loads the window graphics and the sound driver, then starts a
/// new game. It never returns. Called once by System_Entry after it has set
/// the stacks; it clears the debug switches, enables the OAM copy and sleep,
/// and clears gCartridgeResetMarker, which a soft reset leaves set.
void System_Initialize(void) {
  {
    volatile u16 *dma0 = REG_DMA0;

    // DMA0CNT_H: drop the HBlank repeat timing, then the enable bit
    dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
    dma0[5] &= ~DMA_ENABLE;
    dma0[5];
  }
  Io_Write16(WAITCNT_GAME, &REG_WAITCNT);
  // Clear IWRAM up to the heap's end (the stacks above it are in use), then
  // install the resident runtime before any service can use its work blocks
  Dma_Fill32(0, (void *)0x03000000, 0x7800);
  Runtime_InitializeHeap();
  Runtime_InstallIwramAndIrqs();
  // UB: the count and its pad are cleared with one word store
#ifdef UBFIX
  gIoWriteQueue.count = 0;
  gIoWriteQueue.pad = 0;
#else
  *(u32 *)&gIoWriteQueue.count = 0;
#endif
  gDebugTextEnabled = 0;
  gDebugMode = 0;
  gCpuLoadDisplayEnabled = 0;
  System_CopyBuildStamp();
  Ui_LoadWindowGraphics();
  DebugText_Clear();
  REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON;
  Runtime_SetIrqHandler(IRQ_VBLANK, 1, (void (*)(void))System_VBlankHandler);
#if defined(SYSTEM_HALT_ON_GAMEPAK_IRQ)
  Runtime_SetIrqHandler(IRQ_GAMEPAK, 1, IwramHalt);
#endif
  Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
  // Start audio and frame services only after the interrupt handlers are ready.
  Audio_InitializeRuntimeDefaultsFar();
  VramCache_Reset();
  Scheduler_ResetTaskTable();
  gCartridgeResetMarker = 0;
  gOamCopyEnabled = 1;
  gSleepDisabled = 0;
  WaitFrames(10);
  Game_ResetForNewGameFar(0);
}

/// Waits out frames forever; the frame loop still runs the scheduled tasks.
/// Nothing in ☀️ calls it.
UNUSED void Runtime_IdleForever(void) {
  for (;;) {
    (void)gKeysPressed;
    WaitFrames(1);
  }
}

#if TBS_PATCH_4

enum CartridgeLanguage {
  CARTRIDGE_LANGUAGE_ENGLISH,
  CARTRIDGE_LANGUAGE_GERMAN,
  CARTRIDGE_LANGUAGE_FRENCH,
  CARTRIDGE_LANGUAGE_SPANISH,
  CARTRIDGE_LANGUAGE_ITALIAN
};

/// Returns the language of this cartridge from the header's game code letter:
/// 0 English, 1 German, 2 French, 3 Spanish, 4 Italian. Called by the battle
/// target selection and the status screen, which lay text out by language.
s32 System_GetLanguage(void) {
  s32 language;

  switch (gRom_LanguageCode) {
  case 'D':
    language = CARTRIDGE_LANGUAGE_GERMAN;
    break;
  case 'F':
    language = CARTRIDGE_LANGUAGE_FRENCH;
    break;
  case 'S':
    language = CARTRIDGE_LANGUAGE_SPANISH;
    break;
  case 'I':
    language = CARTRIDGE_LANGUAGE_ITALIAN;
    break;
  default:
    language = CARTRIDGE_LANGUAGE_ENGLISH;
    break;
  }
  return language;
}
#endif

/// Reserved empty export A in the runtime far-call table; no source caller.
UNUSED void RuntimeDispatch_ReservedNoOpA(void) {}

/// Reserved empty export B in the runtime far-call table; no source caller.
UNUSED void RuntimeDispatch_ReservedNoOpB(void) {}

/// Reserved empty export C in the runtime far-call table; no source caller.
UNUSED void RuntimeDispatch_ReservedNoOpC(void) {}

/// Reserved empty export D in the runtime far-call table; no source caller.
UNUSED void RuntimeDispatch_ReservedNoOpD(void) {}

/// Reserved empty export E in the runtime far-call table; no source caller.
UNUSED void RuntimeDispatch_ReservedNoOpE(void) {}

/// An exported stub that ignores its arguments and reports 0.
s32 RuntimeDispatch_ReturnZero(s32 unused0, const u8 *unused1) { return 0; }

/// The build stamp's twelve digit characters copied to IWRAM, and a zero word
/// after them that ends the string.
struct BuildStampCopy {
  u32 digits[3];
  u32 terminator;
};

/// Copies the build stamp resource (twelve digits) into gBuildStamp in high
/// IWRAM and writes the zero word that ends it. Called by System_Initialize.
void System_CopyBuildStamp(void) {
  const void *stamp;
  struct BuildStampCopy *copy;

  stamp = Resource_GetTableEntry((u32)&ResourceId_BuildStamp);
  copy = (struct BuildStampCopy *)gBuildStamp;
  Dma_Set(stamp, copy, DMA_COPY_WORDS(3), REG_DMA3);
  copy->terminator = 0;
}

/// An exported stub that does nothing with its argument.
void RuntimeDispatch_NoOpHook(s32 unused) {}

/// Returns the ROM address of resource `index` from the resource directory.
/// Called throughout the game before decoding a resource.
void *Resource_GetTableEntry(s32 index) {
  return Resource_DirectoryTable[index];
}

/// Resolves the Thumb long branches (BL pairs) of a block of code whose branch
/// offsets are stored from the start of the block. A BL is two halfwords: the
/// first carries the marker bits 0xf000 and the high eleven bits of the offset,
/// the second the marker 0xf800 and the low eleven bits of the halfword offset.
/// Each pair's offset is reduced by the pair's own position in the block, which
/// turns it into the pc-relative offset the CPU expects; the pairs are
/// rewritten in place. `byteLength` is the code size. Nothing calls it:
/// Resource_LoadCode uses the ARM routine that does the same.
UNUSED void Code_ResolveLongBranches(u16 *entries, s32 byteLength) {
  s32 entryCount;
  s32 entryIndex;

  entryCount = byteLength >> 1;
  entryIndex = 1;
#ifndef UBFIX
  entries++;
#endif
  if (entryIndex < entryCount) {
    u32 nextMarkerMask;
    u32 currentMarkerMask;
    u32 payloadMask = 0x7ff;
    nextMarkerMask = 0xf800;
    currentMarkerMask = 0xf000;
#ifndef UBFIX
    // UB: temporarily forms a pointer before the start of the code block.
    entries -= 2;
#endif
    // Only adjacent halfwords with both BL marker patterns are relocated.
    do {
      u16 currentEntry;
      u16 nextEntry;
      u32 nextMarkerBits;
      u32 currentMarkerBits;

#ifdef UBFIX
      nextEntry = entries[1];
#else
      nextEntry = entries[2];
#endif
      nextMarkerBits = nextEntry & nextMarkerMask;
#ifndef UBFIX
      entries++;
#endif
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
#ifdef UBFIX
      entries++;
#endif
      entryIndex++;
    } while (entryIndex < entryCount);
  }
}

/// Decodes the compressed code resource `index` to `destination` and resolves
/// its long branches there, with a RAM copy of the ARM routine that does what
/// Code_ResolveLongBranches does; the copy is allocated and freed again.
/// Called by Scene_LoadOverlay for each scene's overlay, and through the
/// far-call table.
void Resource_LoadCode(u32 index, void *destination) {
  void (*patchBranches)(void *, s32);
  s32 codeSize;

  codeSize = Resource_DecodeType01(Resource_GetTableEntry(index), destination);
  RamCode_Load(patchBranches, Resource_PatchThumbBranch,
               Resource_PatchThumbBranchCodeSize);
  patchBranches(destination, codeSize);
  MemFree(patchBranches);
}

/// The handler of every interrupt nobody has claimed.
void Runtime_IgnoreInterrupt(void) {}

/// Copies the resident code to IWRAM, points the BIOS interrupt vector
/// (gIrqVector) at its dispatcher, installs the default handler table and
/// enables the VBlank and keypad interrupts (and, outside Japan and England,
/// the cartridge-removed interrupt, which halts the game). Called by
/// System_Initialize with IME cleared; it sets IME again at the end.
void Runtime_InstallIwramAndIrqs(void) {
  u8 *iwram;

  // Keep interrupts masked while replacing both the dispatcher and its table.
  Io_Write16(0, &REG_IME);
  iwram = IwramIrqMain;
  // 0x1400 bytes of resident code and data
  Dma_Set(IwramRuntime_Rom, iwram, DMA_COPY_WORDS(0x500), REG_DMA3);
  gIrqVector = iwram;
  Dma_Set(gRuntime_IrqHandlers, gIrqHandlers, DMA_COPY_WORDS(IRQ_COUNT),
          REG_DMA3);
  Io_Write16(0, &REG_DISPSTAT);
  // Disarmed until System_Initialize arms the soft-reset keys
  Io_Write16(KEYCNT_DISARMED, &REG_KEYCNT);
#if defined(SYSTEM_HALT_ON_GAMEPAK_IRQ)
  Io_Write16(INTR_FLAG_VBLANK | INTR_FLAG_KEYPAD | INTR_FLAG_GAMEPAK, &REG_IE);
#else
  Io_Write16(INTR_FLAG_VBLANK | INTR_FLAG_KEYPAD, &REG_IE);
#endif
  Io_Write16(1, &REG_IME);
}

#undef SYSTEM_HALT_ON_GAMEPAK_IRQ
