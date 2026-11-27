/* @file */
/* Power-up, the cartridge's language, the build stamp and code loading. */
/* Part of system.c. */

#include "sound/stubs.h"

/* Powers the game up: stops any HBlank DMA left running, sets the cartridge */
/* wait states, clears IWRAM, installs the interrupt handlers and the resident
 */
/* IWRAM code, loads the window graphics and the sound driver, then enters */
/* gmMainRoutineRun and never returns. */
void syInit(void) {
	DMA0_STOP();
	syIoWrite16(WAITCNT_GAME, &REG_WAITCNT);
	/* Clear IWRAM up to CartridgeResetMarker; the stacks above it are in use.
	 */
	btDmaFill32(0, (void *)IWRAM_START, 30 * 1024);
	syHeapInit();
	syInstallIwramAndIrqs();
#ifdef UBFIX
	IoWriteQueueWork.cnt = 0;
	IoWriteQueueWork.padding = 0;
#else
	*(u32 *)&IoWriteQueueWork.cnt = 0;
#endif
	DebugTextEnabled = 0;
	DebugMode = 0;
	CpuLoadDisplayEnabled = 0;
	syCopyBuildStamp();
	syWindowLoadGraphics();
	syDebugTextClear();
	REG_DISPCNT = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
	syIntFunction(IRQ_VBLANK, 1, syVBlankHandler);
	syIntFunction(IRQ_GAMEPAK, 1, syHalt);
	syIoWrite16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
	sySndInitStub();
	syVramCacheReset();
	syInitEvent();
	CartridgeResetMarker = 0;
	OamCopyEnabled = 1;
	OamUsageWork.limit = OAM_SIZE / 4;
	OamUsageWork.used = 0;
	SleepDisabled = 0;
	sySynch(10);
	gmMainRoutineRunStub(GAME_START_TITLE);
}

/* Unused: lets frames pass forever, the tasks still running each frame. */
UNUSED void syIdleForever(void) {
	volatile INPUT_STATE *input = &InputWork;

	for(;;) {
		input->pressed;
		sySynch(1);
	}
}

#if (LANGUAGE != LANGUAGE_JAPANESE) && (LANGUAGE != LANGUAGE_ENGLISH)
/* Returns the language of this cartridge from the last letter of the game */
/* code in its header, as an CARTRIDGE_LANGUAGE. Only the German, */
/* Spanish, French and Italian editions have it. */
s32 syGetLanguage(void) {
	s32 language;

	switch(Cart_LanguageCode) {
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

/* Unused: five empty exports the system library's stub table reserves; */
/* nothing calls them. */
UNUSED void syDummyReservedNoOpA(void) {}
UNUSED void syDummyReservedNoOpB(void) {}
UNUSED void syDummyReservedNoOpC(void) {}
UNUSED void syDummyReservedNoOpD(void) {}
UNUSED void syDummyReservedNoOpE(void) {}

/* An exported stub that ignores its arguments and returns 0. */
s32 syDummyReturnZero(s32 resourceId, const u8 *table) { return 0; }

/* Copies the build stamp resource (three words) into BuildStampWork and */
/* writes the zero word that ends it. Called at power-up, before the window */
/* graphics load. */
void syCopyBuildStamp(void) {
	syDMAXCopy(syResourceGetTableEntry(RESOURCE_BUILD_STAMP), BuildStampWork,
			   DMA_COPY_WORDS(3), REG_DMA3);
	BuildStampWork[3] = 0;
}

/* An exported stub that does nothing with its argument. The title, the staff */
/* credits, Lucky Dice, Super Lucky Dice and Lucky Wheels call it with a */
/* stub call resource id. */
void syResourceReservedNoOp(s32 unused) {}

/* Returns the ROM address of resource `index` from the resource directory. */
/* Called throughout the game before decoding a resource. */
void *syResourceGetTableEntry(s32 index) {
	return syResourceDirectoryTable[index];
}

/* A Thumb long branch (BL) is two halfwords: the first carries the marker */
/* THUMB_BL_HIGH and the offset's high eleven bits, from THUMB_BL_HIGH_SHIFT, */
/* the second the marker THUMB_BL_LOW and the low eleven bits of the halfword */
/* offset (THUMB_BL_OFFSET_BITS each). */
#define THUMB_BL_HIGH		 0xf000
#define THUMB_BL_LOW		 0xf800
#define THUMB_BL_OFFSET_BITS 0x7ff
#define THUMB_BL_HIGH_SHIFT	 12

/* Unused: resolves the Thumb long branches (BL pairs) of a block of code whose
 */
/* branch offsets are stored from the start of the block. Each pair's offset is
 */
/* reduced by the pair's own position in the block, which turns it into the */
/* pc-relative offset the CPU expects; the pairs are rewritten in place. */
/* `byteLength` is the code size. Nothing calls it: syOverlayLoad uses the ARM
 */
/* function that does the same. */
UNUSED void syCodeResolveLongBranches(u16 *pair, s32 byteLength) {
	s32 entryCount = byteLength >> 1;
	s32 i = 1;

	pair++;
	if(i < entryCount) {
		u32 lowMarker = THUMB_BL_LOW;
		u32 offsetBits = THUMB_BL_OFFSET_BITS;
		u32 highMarker = THUMB_BL_HIGH;

#ifndef UBFIX
		/* UB: the walk starts a halfword before the block. */
		pair -= 2;
#endif
		/* Each halfword from the second on is tried as the second of a pair. */
		for(; i < entryCount; i++) {
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
			if(secondMarker == lowMarker) {
#ifdef UBFIX
				first = pair[-1];
#else
				first = pair[0];
#endif
				firstMarker = first & secondMarker;

				if(firstMarker == highMarker) {
					u32 offset =
						(((first & offsetBits) << THUMB_BL_HIGH_SHIFT) |
						 ((second & offsetBits) * 2)) -
						i * 2;
					u32 newFirst =
						((offset >> THUMB_BL_HIGH_SHIFT) & offsetBits) |
						firstMarker;
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

/* Decodes the compressed code resource `index` to `destination` and resolves */
/* its long branches there, with a RAM copy of the ARM function that does what
 */
/* syCodeResolveLongBranches does; the copy is allocated and freed again. */
void syOverlayLoad(u32 index, void *destination) {
	void (*patchBranches)(void *, s32);
	s32 codeSize;

	codeSize =
		syDecompressTaggedLz(syResourceGetTableEntry(index), destination);
	syRamCodeLoad(patchBranches, syResourcePatchThumbBranch,
				  syResourcePatchThumbBranchCodeSize);
	patchBranches(destination, codeSize);
	syHeapRelease(patchBranches);
}

/* The handler of every interrupt nobody has claimed. */
void syIrqIgnore(void) {}

/* Copies the resident code to IWRAM (4 KB), points the BIOS interrupt vector */
/* (IrqVector) at its dispatcher, installs the default handler table and */
/* enables the VBlank, keypad and cartridge-removed interrupts. */
void syInstallIwramAndIrqs(void) {
	/* Keep interrupts masked while replacing both the dispatcher and its table.
	 */
	syIoWrite16(0, &REG_IME);
	syDMAXCopy(IwramResident_Rom, Ram_IwramIrqMain, DMA_COPY_WORDS(1024),
			   REG_DMA3);
	*Ram_IrqVector = IwramIrqMain;
	syDMAXCopy(DefaultIrqHandlers, IrqHandlers, DMA_COPY_WORDS(IRQ_COUNT),
			   REG_DMA3);
	syIoWrite16(0, &REG_DISPSTAT);
	/* Disarmed until syInit arms the soft-reset keys */
	syIoWrite16(KEYCNT_DISARMED, &REG_KEYCNT);
	syIoWrite16(INTR_FLAG_VBLANK | INTR_FLAG_KEYPAD | INTR_FLAG_GAMEPAK,
				&REG_IE);
	syIoWrite16(1, &REG_IME);
}

#undef THUMB_BL_HIGH
#undef THUMB_BL_LOW
#undef THUMB_BL_OFFSET_BITS
#undef THUMB_BL_HIGH_SHIFT
