/// @file
/// Power-up, the cartridge's language, the build stamp and code loading.
// Part of system.c.

#include "../system_internal.h"
#include "system/ram_code.h"
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
  Irq_SetHandler(IRQ_VBLANK, 1, System_VBlankHandler);
  Irq_SetHandler(IRQ_GAMEPAK, 1, Iwram_Halt);
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
  MainRoutine_RunFar(GAME_START_TITLE);
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

/// An exported stub that ignores its arguments and reports 0.
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

/// Returns the ROM address of resource `index` from the resource directory.
/// Called throughout the game before decoding a resource.
void *Resource_GetTableEntry(s32 index) {
  return Resource_DirectoryTable[index];
}

/// Unused: resolves the Thumb long branches (BL pairs) of a block of code whose
/// branch offsets are stored from the start of the block. A BL is two
/// halfwords: the first carries the marker bits 0xf000 and the high eleven bits
/// of the offset, the second the marker 0xf800 and the low eleven bits of the
/// halfword offset. Each pair's offset is reduced by the pair's own position in
/// the block, which turns it into the pc-relative offset the CPU expects; the
/// pairs are rewritten in place. `byteLength` is the code size. Nothing calls
/// it: Overlay_Load uses the ARM routine that does the same.
UNUSED void Code_ResolveLongBranches(u16 *pair, s32 byteLength) {
  s32 entryCount = byteLength >> 1;
  s32 i = 1;

  pair++;
  if (i < entryCount) {
    u32 lowMarker = 0xf800;
    u32 offsetBits = 0x7ff;
    u32 highMarker = 0xf000;

#ifndef UBFIX
    // UB: the walk starts a halfword before the block.
    pair -= 2;
#endif
    // Each halfword from the second on is tried as the second of a pair.
    do {
      u16 first, second;
      u32 secondMarker, firstMarker;

#ifdef UBFIX
      second = pair[0];
#else
      second = pair[2];
#endif
      secondMarker = second & lowMarker;

#ifndef UBFIX
      pair++;
#endif
      if (secondMarker == lowMarker) {
#ifdef UBFIX
        first = pair[-1];
#else
        first = pair[0];
#endif
        firstMarker = first & secondMarker;

        if (firstMarker == highMarker) {
          u32 offset =
              (((first & offsetBits) << 12) | ((second & offsetBits) * 2)) -
              i * 2;
          u32 newFirst = ((offset >> 12) & offsetBits) | firstMarker;
          u32 newSecond = ((offset >> 1) & offsetBits) | secondMarker;

#ifdef UBFIX
          pair[-1] = newFirst;
          pair[0] = newSecond;
#else
          pair[0] = newFirst;
          pair[1] = newSecond;
#endif
        }
      }
#ifdef UBFIX
      pair++;
#endif
      i++;
    } while (i < entryCount);
  }
}

void Overlay_Load(u32 index, void *destination) {
  void (*routine)(void *, s32);
  s32 end;

  end = Decompress_TaggedLz((const void *)Resource_GetTableEntry(index),
                            destination);
  RamCode_Load(routine, Resource_PatchThumbBranch,
               Resource_PatchThumbBranchCodeSize);
  routine(destination, end);
  Heap_Release(routine);
}

/// The handler of every interrupt nobody has claimed.
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
