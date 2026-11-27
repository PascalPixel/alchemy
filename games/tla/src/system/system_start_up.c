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

#include "system/dummy_reserved.c"

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

#include "system/resource_and_overlay_load.c"

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
