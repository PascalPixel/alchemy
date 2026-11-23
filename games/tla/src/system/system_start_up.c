/// @file
/// Power-up, the cartridge's language, the build stamp and code loading.
// Part of system.c.

#include "system_internal.h"
#include "system/ram_code.h"
#include "gamepak/far_call/field_sequence.h"
#include "gamepak/far_call/sound.h"
#include "resource_ids.h"

/// Powers the game up: stops any HBlank DMA left running, sets the cartridge
/// wait states, clears IWRAM, installs the interrupt handlers and the resident
/// IWRAM code, loads the window graphics and the sound driver, then enters
/// MainRoutine_Run and never returns.
void System_Initialize(void) {
  DMA0_STOP();
  Io_Write16(WAITCNT_GAME, &REG_WAITCNT);
  // Clear IWRAM up to gCartridgeResetMarker; the stacks above it are in use.
  Dma_Fill32(0, (void *)IWRAM_START, 30 * 1024);
  Heap_Initialize();
  System_InstallIwramAndIrqs();
#ifdef UBFIX
  gIoWriteQueue.count = 0;
  gIoWriteQueue.padding = 0;
#else
  *(u32 *)&gIoWriteQueue.count = 0;
#endif
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
  // Start audio and frame services only after the interrupt handlers are ready.
  Sound_InitializeFar();
  VramCache_Reset();
  Task_ResetAll();
  gCartridgeResetMarker = 0;
  gOamCopyEnabled = 1;
  gOamUsage.limit = OAM_SIZE / 4;
  gOamUsage.used = 0;
  gSleepDisabled = 0;
  wait(10);
  MainRoutine_RunFar(GAME_START_TITLE);
}

/// Unused: waits out frames forever; the frame loop still runs the
/// tasks.
UNUSED void System_IdleForever(void) {
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

/// Unused: reserved empty export A in the System far-call table; no source
/// caller.
UNUSED void Stub_ReservedNoOpA(void) {}

/// Unused: reserved empty export B in the System far-call table; no source
/// caller.
UNUSED void Stub_ReservedNoOpB(void) {}

/// Unused: reserved empty export C in the System far-call table; no source
/// caller.
UNUSED void Stub_ReservedNoOpC(void) {}

/// Unused: reserved empty export D in the System far-call table; no source
/// caller.
UNUSED void Stub_ReservedNoOpD(void) {}

/// Unused: reserved empty export E in the System far-call table; no source
/// caller.
UNUSED void Stub_ReservedNoOpE(void) {}

/// An exported stub that ignores its arguments and reports 0.
s32 Stub_ReturnZero(s32 resourceId, const u8 *table) { return 0; }

/// Copies the build stamp resource (three words) into gBuildStamp and
/// writes the zero word that ends it. Called at power-up, before the window
/// graphics load.
void System_CopyBuildStamp(void) {
  Dma_Set(Resource_GetTableEntry(RESOURCE_BUILD_STAMP), gBuildStamp,
          DMA_COPY_WORDS(3), REG_DMA3);
  gBuildStamp[3] = 0;
}

/// An exported stub that does nothing with its argument. The title, the staff
/// credits, Lucky Dice, Super Lucky Dice and Lucky Wheels call it with a
/// far-call resource id.
void Resource_ReservedNoOp(s32 unused) {}

/// Returns the ROM address of resource `index` from the resource directory.
/// Called throughout the game before decoding a resource.
void *Resource_GetTableEntry(s32 index) {
  return Resource_DirectoryTable[index];
}

/// A Thumb long branch (BL) is two halfwords: the first carries the marker
/// THUMB_BL_HIGH and the offset's high eleven bits, from THUMB_BL_HIGH_SHIFT,
/// the second the marker THUMB_BL_LOW and the low eleven bits of the halfword
/// offset (THUMB_BL_OFFSET_BITS each).
#define THUMB_BL_HIGH 0xf000
#define THUMB_BL_LOW 0xf800
#define THUMB_BL_OFFSET_BITS 0x7ff
#define THUMB_BL_HIGH_SHIFT 12

/// Unused: resolves the Thumb long branches (BL pairs) of a block of code whose
/// branch offsets are stored from the start of the block. Each pair's offset is
/// reduced by the pair's own position in the block, which turns it into the
/// pc-relative offset the CPU expects; the pairs are rewritten in place.
/// `byteLength` is the code size. Nothing calls it: Overlay_Load uses the ARM
/// routine that does the same.
UNUSED void Code_ResolveLongBranches(u16 *pair, s32 byteLength) {
  s32 entryCount = byteLength >> 1;
  s32 i = 1;

  pair++;
  if (i < entryCount) {
    u32 lowMarker = THUMB_BL_LOW;
    u32 offsetBits = THUMB_BL_OFFSET_BITS;
    u32 highMarker = THUMB_BL_HIGH;

#ifndef UBFIX
    // UB: the walk starts a halfword before the block.
    pair -= 2;
#endif
    // Each halfword from the second on is tried as the second of a pair.
    for (; i < entryCount; i++) {
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
          u32 offset = (((first & offsetBits) << THUMB_BL_HIGH_SHIFT) |
                        ((second & offsetBits) * 2)) -
                       i * 2;
          u32 newFirst =
              ((offset >> THUMB_BL_HIGH_SHIFT) & offsetBits) | firstMarker;
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
    }
  }
}

/// Decodes the compressed code resource `index` to `destination` and resolves
/// its long branches there, with a RAM copy of the ARM routine that does what
/// Code_ResolveLongBranches does; the copy is allocated and freed again.
void Overlay_Load(u32 index, void *destination) {
  void (*patchBranches)(void *, s32);
  s32 codeSize;

  codeSize = Decompress_TaggedLz(Resource_GetTableEntry(index), destination);
  RamCode_Load(patchBranches, Resource_PatchThumbBranch,
               Resource_PatchThumbBranchCodeSize);
  patchBranches(destination, codeSize);
  Heap_Release(patchBranches);
}

/// The handler of every interrupt nobody has claimed.
void Irq_Ignore(void) {}

/// Copies the resident code to IWRAM, points the BIOS interrupt vector
/// (gIrqVector) at its dispatcher, installs the default handler table and
/// enables the VBlank, keypad and cartridge-removed interrupts.
void System_InstallIwramAndIrqs(void) {
  // Keep interrupts masked while replacing both the dispatcher and its table.
  Io_Write16(0, &REG_IME);
  Dma_Set(IwramResident_Rom, Ram_IwramIrqMain, DMA_COPY_WORDS(1024), REG_DMA3);
  *Ram_IrqVector = IwramIrqMain;
  Dma_Set(gDefaultIrqHandlers, gIrqHandlers, DMA_COPY_WORDS(IRQ_COUNT),
          REG_DMA3);
  Io_Write16(0, &REG_DISPSTAT);
  // Disarmed until System_Initialize arms the soft-reset keys
  Io_Write16(KEYCNT_DISARMED, &REG_KEYCNT);
  Io_Write16(INTR_FLAG_VBLANK | INTR_FLAG_KEYPAD | INTR_FLAG_GAMEPAK, &REG_IE);
  Io_Write16(1, &REG_IME);
}

#undef THUMB_BL_HIGH
#undef THUMB_BL_LOW
#undef THUMB_BL_OFFSET_BITS
#undef THUMB_BL_HIGH_SHIFT
