/* Power-up, the cartridge's language, the build stamp and code loading. Part of
   system.c. */

/* The German, French, Spanish and Italian editions also take the
   cartridge-removed interrupt, whose handler is syHalt. */
#if HALT_ON_CARTRIDGE_REMOVED
#define SYSTEM_HALT_ON_GAMEPAK_IRQ 1
#endif

/* Powers the game up: stops any HBlank DMA left running, sets the cartridge
   wait states, clears IWRAM, installs the interrupt handlers and the resident
   IWRAM code, loads the window graphics and the sound driver, then enters
   gmMainRoutineRun and never returns. */
void syInit(void)
{
	DMA0_STOP();
	IO_WRITE16(WAITCNT_GAME, &REG_WAITCNT);
	/* Clear IWRAM up to the heap's end (the stacks above it are in use), then
	   install the resident IWRAM code before any service can use its state
	   blocks */
	DMA_FILL32(0, (void *)IWRAM_START, 30 * 1024);
	syHeapInit();
	syInstallIWRAMAndIRQs();
	/* UB: count and its padding are cleared as one word. */
#if FIX_TYPE_PUNNING
	IoWriteQueueWork.cnt = 0;
	IoWriteQueueWork.pad = 0;
#else
	*(Uint32 *)&IoWriteQueueWork.cnt = 0;
#endif
	DebugTextEnabled = 0;
	DebugMode = 0;
	CpuLoadDisplayEnabled = 0;
	syCopyBuildStamp();
	syWindowLoadGraphics();
	syPrintClear();
	REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON;
	syIntFunction(IRQ_VBLANK, 1, (void (*)(void))syVBlankHandler);
#if defined(SYSTEM_HALT_ON_GAMEPAK_IRQ)
	syIntFunction(IRQ_GAMEPAK, 1, syHalt);
#endif
	IO_WRITE16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
	/* Start audio and tick services only after the interrupt handlers are
	   ready. */
	sySndInitStub();
	syVRAMCacheReset();
	syInitEvent();
	CartridgeResetMarker = 0;
	OamCopyEnabled = 1;
	SleepDisabled = 0;
	sySynch(10);
	gmMainRoutineRunStub(GAME_START_TITLE);
}

/* Unused: waits out ticks forever; the tick loop still runs the events. */
UNUSED void syIdleForever(void)
{
	for(;;) {
		(void)PerPush;
		sySynch(1);
	}
}

#if BATTLE_LABELS_BY_HEADER_LANGUAGE

/* Returns the language of this cartridge from the header's game code letter, an
   CARTRIDGE_LANGUAGE. Only the Spanish and Italian editions have it. */
Sint32 syGetLanguage(void)
{
	Sint32 language;

	switch(RomLanguageCode) {
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

#include "system/dummy_reserved.c"

/* The build stamp's twelve digit characters copied to IWRAM, and a zero word
   after them that ends the string. */
typedef struct BUILD_STAMP_COPY {
	Uint32 digits[3];
	Uint32 terminator;
} BUILD_STAMP_COPY;

/* Copies the build stamp resource (twelve digits) into BuildStampWork in high
   IWRAM and writes the zero word that ends it. */
void syCopyBuildStamp(void)
{
	const void		 *stamp;
	BUILD_STAMP_COPY *copy;

	stamp = syResourceGetTableEntry((Uint32)RESOURCE_BUILD_STAMP);
	copy = (BUILD_STAMP_COPY *)BuildStampWork;
	syDMAXCopy(stamp, copy, DMA_COPY_WORDS(3), REG_DMA3);
	copy->terminator = 0;
}

/* An exported stub that does nothing with its argument. The Title, Lucky Dice
   and Lucky Wheels (fdLuckyWheelsRun) call it with a stub call resource id. */
void syDummyNoOperationHook(Sint32 unused)
{
}

#include "system/resource_and_overlay_load.c"

/* Copies the resident code to IWRAM, points the BIOS interrupt vector
   (IrqVector) at its dispatcher, installs the default handler table and enables
   the VBlank and keypad interrupts (and, in the German, French, Spanish and
   Italian editions, the cartridge-removed interrupt, which halts the game). */
void syInstallIWRAMAndIRQs(void)
{
	Uint8 *iwram;

	/* Keep interrupts masked while replacing both the dispatcher and its
	   table. */
	IO_WRITE16(0, &REG_IME);
	iwram = syIWRAMIRQMain;
	/* 5,120 bytes of resident code and data. */
	syDMAXCopy(IwramResidentRom, iwram, DMA_COPY_WORDS(5120 / 4), REG_DMA3);
	IrqVector = iwram;
	syDMAXCopy(
		DefaultIrqHandlers, IrqHandlers, DMA_COPY_WORDS(IRQ_COUNT), REG_DMA3);
	IO_WRITE16(0, &REG_DISPSTAT);
	/* Disarmed until syInit arms the soft-reset keys */
	IO_WRITE16(KEYCNT_DISARMED, &REG_KEYCNT);
#if defined(SYSTEM_HALT_ON_GAMEPAK_IRQ)
	IO_WRITE16(
		INTR_FLAG_VBLANK | INTR_FLAG_KEYPAD | INTR_FLAG_GAMEPAK, &REG_IE);
#else
	IO_WRITE16(INTR_FLAG_VBLANK | INTR_FLAG_KEYPAD, &REG_IE);
#endif
	IO_WRITE16(1, &REG_IME);
}

#undef SYSTEM_HALT_ON_GAMEPAK_IRQ
#undef THUMB_BL_HIGH
#undef THUMB_BL_LOW
#undef THUMB_BL_OFFSET_BITS
#undef THUMB_BL_HIGH_SHIFT
