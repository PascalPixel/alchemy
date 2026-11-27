/* Power-up, the cartridge's language, the build stamp and code loading. Part of
   system.c. */

#include "sound/stubs.h"

/* Powers the game up: stops any HBlank DMA left running, sets the cartridge
   wait states, clears IWRAM, installs the interrupt handlers and the resident
   IWRAM code, loads the window graphics and the sound driver, then enters
   gmMainRoutineRun and never returns. */
void syInit(void)
{
	DMA0_STOP();
	syIOWrite16(WAITCNT_GAME, &REG_WAITCNT);
	/* Clear IWRAM up to CartridgeResetMarker; the stacks above it are in
	   use. */
	DMA_FILL32(0, (void *)IWRAM_START, 30 * 1024);
	syHeapInit();
	syInstallIWRAMAndIRQs();
#if FIX_TYPE_PUNNING
	IoWriteQueueWork.cnt = 0;
	IoWriteQueueWork.padding = 0;
#else
	*(Uint32 *)&IoWriteQueueWork.cnt = 0;
#endif
	DebugTextEnabled = 0;
	DebugMode = 0;
	CpuLoadDisplayEnabled = 0;
	syCopyBuildStamp();
	syWindowLoadGraphics();
	syPrintClear();
	REG_DISPCNT = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
	syIntFunction(IRQ_VBLANK, 1, syVBlankHandler);
	syIntFunction(IRQ_GAMEPAK, 1, syHalt);
	syIOWrite16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
	sySndInitStub();
	syVRAMCacheReset();
	syInitEvent();
	CartridgeResetMarker = 0;
	OamCopyEnabled = 1;
	OamUsageWork.limit = OAM_SIZE / 4;
	OamUsageWork.used = 0;
	SleepDisabled = 0;
	sySynch(10);
	gmMainRoutineRunStub(GAME_START_TITLE);
}

/* Unused: lets ticks pass forever, the events still running each tick. */
UNUSED void syIdleForever(void)
{
	volatile INPUT_STATE *input = &PerWork;

	for(;;) {
		input->push;
		sySynch(1);
	}
}

#if BATTLE_LABELS_BY_HEADER_LANGUAGE
/* Returns the language of this cartridge from the last letter of the game code
   in its header, as an CARTRIDGE_LANGUAGE. Only the German, Spanish, French and
   Italian editions have it. */
Sint32 syGetLanguage(void)
{
	Sint32 language;

	switch(CartLanguageCode) {
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

/* Unused: five empty exports the system library's stub table reserves; nothing
   calls them. */
UNUSED void syDummyReservedNoOperationA(void)
{
}

UNUSED void syDummyReservedNoOperationB(void)
{
}

UNUSED void syDummyReservedNoOperationC(void)
{
}

UNUSED void syDummyReservedNoOperationD(void)
{
}

UNUSED void syDummyReservedNoOperationE(void)
{
}

/* An exported stub that ignores its arguments and returns 0. */
Sint32 syDummyReturnZero(Sint32 resourceId, const Uint8 *tbl)
{
	return 0;
}

/* Copies the build stamp resource (three words) into BuildStampWork and writes
   the zero word that ends it. Called at power-up, before the window graphics
   load. */
void syCopyBuildStamp(void)
{
	syDMAXCopy(syResourceGetTableEntry(RESOURCE_BUILD_STAMP), BuildStampWork,
		DMA_COPY_WORDS(3), REG_DMA3);
	BuildStampWork[3] = 0;
}

/* An exported stub that does nothing with its argument. The title, the staff
   credits, Lucky Dice, Super Lucky Dice and Lucky Wheels call it with a stub
   call resource id. */
void syResourceReservedNoOperation(Sint32 unused)
{
}

/* Returns the ROM address of resource `idx` from the resource directory.
   Called throughout the game before decoding a resource. */
void *syResourceGetTableEntry(Sint32 idx)
{
	return syResourceDirectoryTable[idx];
}

/* A Thumb long branch (BL) is two halfwords: the first carries the marker
   THUMB_BL_HIGH and the offset's high eleven bits, from THUMB_BL_HIGH_SHIFT,
   the second the marker THUMB_BL_LOW and the low eleven bits of the halfword
   offset (THUMB_BL_OFFSET_BITS each). */
#define THUMB_BL_HIGH		 0xf000
#define THUMB_BL_LOW		 0xf800
#define THUMB_BL_OFFSET_BITS 0x7ff
#define THUMB_BL_HIGH_SHIFT	 12

/* Unused: resolves the Thumb long branches (BL pairs) of a block of code whose
   branch offsets are stored from the start of the block. Each pair's offset is
   reduced by the pair's own position in the block, which turns it into the
   pc-relative offset the CPU expects; the pairs are rewritten in place.
   `byteLength` is the code size. Nothing calls it: syOverlayLoad uses the ARM
   function that does the same. */
UNUSED void syCodeResolveLongBranches(Uint16 *pair, Sint32 byteLength)
{
	Sint32 entryCnt = byteLength >> 1;
	Sint32 i = 1;

	pair++;
	if(i < entryCnt) {
		Uint32 lowMarker = THUMB_BL_LOW;
		Uint32 ofsBits = THUMB_BL_OFFSET_BITS;
		Uint32 highMarker = THUMB_BL_HIGH;

#if !FIX_POINTER_OUTSIDE_OBJECT
		/* UB: the walk starts a halfword before the block. */
		pair -= 2;
#endif
		/* Each halfword from the second on is tried as the second of a pair. */
		for(; i < entryCnt; i++) {
			Uint16 first, second;
			Uint32 secondMarker, firstMarker;

#if FIX_POINTER_OUTSIDE_OBJECT
			second = pair[0];
#else
			second = pair[2];
#endif
			secondMarker = second & lowMarker;

#if !FIX_POINTER_OUTSIDE_OBJECT
			pair++;
#endif
			if(secondMarker == lowMarker) {
#if FIX_POINTER_OUTSIDE_OBJECT
				first = pair[-1];
#else
				first = pair[0];
#endif
				firstMarker = first & secondMarker;

				if(firstMarker == highMarker) {
					Uint32 ofs = (((first & ofsBits) << THUMB_BL_HIGH_SHIFT) |
									 ((second & ofsBits) * 2)) -
						i * 2;
					Uint32 newFirst =
						((ofs >> THUMB_BL_HIGH_SHIFT) & ofsBits) | firstMarker;
					Uint32 newSecond = ((ofs >> 1) & ofsBits) | secondMarker;

#if FIX_POINTER_OUTSIDE_OBJECT
					pair[-1] = newFirst;
					pair[0] = newSecond;
#else
					pair[0] = newFirst;
					pair[1] = newSecond;
#endif
				}
			}
#if FIX_POINTER_OUTSIDE_OBJECT
			pair++;
#endif
		}
	}
}

/* Decodes the compressed code resource `idx` to `dst` and resolves
   its long branches there, with a RAM copy of the ARM function that does what
   syCodeResolveLongBranches does; the copy is allocated and freed again. */
void syOverlayLoad(Uint32 idx, void *dst)
{
	void (*patchBranches)(void *, Sint32);
	Sint32 codeSize;

	codeSize = syDecompressTaggedLZ(syResourceGetTableEntry(idx), dst);
	RAM_CODE_LOAD(patchBranches, syResourcePatchThumbBranch,
		syResourcePatchThumbBranchCodeSize);
	patchBranches(dst, codeSize);
	syHeapRelease(patchBranches);
}

/* The handler of every interrupt nobody has claimed. */
void syIRQIgnore(void)
{
}

/* Copies the resident code to IWRAM (4 KB), points the BIOS interrupt vector
   (IrqVector) at its dispatcher, installs the default handler table and enables
   the VBlank, keypad and cartridge-removed interrupts. */
void syInstallIWRAMAndIRQs(void)
{
	/* Keep interrupts masked while replacing both the dispatcher and its
	   table. */
	syIOWrite16(0, &REG_IME);
	syDMAXCopy(
		IwramResidentRom, RamIwramIrqMain, DMA_COPY_WORDS(1024), REG_DMA3);
	*RamIrqVector = syIWRAMIRQMain;
	syDMAXCopy(
		DefaultIrqHandlers, IrqHandlers, DMA_COPY_WORDS(IRQ_COUNT), REG_DMA3);
	syIOWrite16(0, &REG_DISPSTAT);
	/* Disarmed until syInit arms the soft-reset keys */
	syIOWrite16(KEYCNT_DISARMED, &REG_KEYCNT);
	syIOWrite16(
		INTR_FLAG_VBLANK | INTR_FLAG_KEYPAD | INTR_FLAG_GAMEPAK, &REG_IE);
	syIOWrite16(1, &REG_IME);
}

#undef THUMB_BL_HIGH
#undef THUMB_BL_LOW
#undef THUMB_BL_OFFSET_BITS
#undef THUMB_BL_HIGH_SHIFT
