/* Interrupt handlers, the frame wait, key repeat and the VBlank handler. Part
   of system.c. */

/* The word a soft reset leaves in CartridgeResetMarker: a date, 4 July 1967,
   its digits packed four bits each. */
#define CARTRIDGE_RESET_MARK 0x19670704

/* DISPSTAT's low byte: its flags and interrupt enables, without the VCount line
   above them. */
#define DISPSTAT_FLAGS 0xff

/* The lines REG_VCOUNT counts in VBlank, after the DISPLAY_HEIGHT drawn: it
   runs to DISPLAY_TOTAL_LINES - 1. */
#define VBLANK_LINES (DISPLAY_TOTAL_LINES - DISPLAY_HEIGHT)

/* Every bit of a key word, as KEYINPUT's 16 bits. */
#define KEY_WORD_BITS 0xffff

/* Restarts the game from the cartridge entry point. The marker word it leaves
   in IWRAM is cleared again by syInit; nothing else reads it. */
static __inline__ void sySoftReset(void)
{
	Sint32 (*reset)(void) = (Sint32 (*)(void))Cart_Start;

	CartridgeResetMarker = CARTRIDGE_RESET_MARK;
	IO_WRITE16(0, &REG_IME);
	reset();
}

/* Halts the CPU (the BIOS Halt) until the VBlank handler has run once. */
#define SYNCH_WAIT_VBLANK() \
	{ \
		VBlankDone &= ~1; \
		do { \
			ASM_BIOS_HALT(); \
		} while(!(VBlankDone & 1)); \
	}

/* Installs `handler` for interrupt `irq` and enables that interrupt, or with a
   null handler disables it and installs the do-nothing handler. */
void syIntFunction(Uint32 irq, Sint32 vcount, INTERRUPT_HANDLER handler)
{
	if(irq <= IRQ_GAMEPAK) {
		Uint32 ime;
		Uint32 bit;
		Uint32 ie;

		IRQ_LOCK(ime);
		bit = 1 << irq;
		ie = REG_IE & ~bit;
		if(handler != 0)
			ie |= bit;
		REG_IE = ie;
		if(irq <= IRQ_VCOUNT) {
			/* The display interrupts also need their DISPSTAT enable */
			Uint32 enable = DISPSTAT_VBLANK_INTR << irq;
			Uint32 keep = ~enable;
			Uint32 stat;

			if(irq == IRQ_VCOUNT) {
				enable |= DISPSTAT_VCOUNT_LINE(vcount);
				/* The old line goes too. */
				keep &= DISPSTAT_FLAGS;
			}
			stat = REG_DISPSTAT & keep;
			if(handler != 0)
				stat |= enable;
			REG_DISPSTAT = stat;
		}
		if(handler != 0)
			IrqHandlers[irq] = handler;
		else
			IrqHandlers[irq] = syIRQIgnore;
		IRQ_UNLOCK(ime);
	}
}

/* Durations used by the sleep transitions: the first two in frames, the last
   two in VBlanks, since the sleep waits on SYNCH_WAIT_VBLANK alone. */
enum {
	SLEEP_IDLE_FRAMES = 3 * 60 * 60, /* three minutes with no key held */
	SLEEP_COMBO_FRAMES = 3 * 60,	 /* three seconds holding only L and R */
	SLEEP_WHITE_VBLANKS = 60, /* the white screen shown before the Stop */
	SLEEP_WAKE_VBLANKS = 10	  /* waited after waking, screen restored */
};

/* Frames the debug CPU-load display holds a peak. */
enum { CPU_LOAD_PEAK_FRAMES = 30 };

/* The game's frame loop: runs `frames` frames and returns. */
void sySynch(Sint32 frames)
{
	Uint8 *sp;
	Uint32 frameIndex;
	Uint8 *base;
	Uint32 line;
	Sint32 delayVBlank;
	Sint16 savedDisplayControl;
	Sint16 savedBackdropColor;

	ASM_STACK_POINTER_GET(sp);

	/* Save the caller's stack and continue at the frame-wait stack top */
	base = sp;
	if(base < Ram_FrameWaitStackTop) {
		Uint8 *top = Ram_FrameWaitStackTop;

		SavedStackSize = top - base;
		syDMAXCopy(
			base, SavedStack, DMA_COPY_WORDS(SavedStackSize >> 2), REG_DMA3);
		ASM_STACK_POINTER_SET(top);
	}
	for(frameIndex = 0; frameIndex < frames; frameIndex++) {
		/* Run the frame pass's events and build this frame's sprites */
		EventsRunning = 1;
		syExecuteEvent(EVENT_PASS_FRAME);
		EventsRunning = 0;
		syOAMBuildBuffer(syHeapSlotAlloc(HEAP_SLOT_FRAME_SCRATCH, 1024));
		FrameRenderPending = 1;
		if(CpuLoadDisplayEnabled) {
			/* CPU load: the scanline the frame's work ended on, counted from
			   the last VBlank, plus 256 for each VBlank it overran; the peak is
			   held for 30 frames The meter reads the line through a plain
			   pointer, where fdScreenTransitionUpdateScanline reads the
			   volatile register. */
			line = *(Uint16 *)&REG_VCOUNT;
			if(line >= DISPLAY_HEIGHT)
				line -= DISPLAY_HEIGHT;
			else
				line += VBLANK_LINES;
			line += (FrameVBlanks - 1) << 8;
			if(CpuLoadTimer == 0)
				CpuLoadPeak = 0;
			else
				CpuLoadTimer--;
			if(CpuLoadPeak < line) {
				CpuLoadPeak = line;
				CpuLoadTimer = CPU_LOAD_PEAK_FRAMES;
			}
		}
		/* Count idle frames and the L+R hold that request sleep */
		if(SleepDisabled == 0) {
			if(IdleSleepEnabled) {
				if(PerData)
					IdleFrames = 0;
				else {
					IdleFrames++;
					if(IdleFrames > SLEEP_IDLE_FRAMES)
						SleepRequested = 1;
				}
			}
			if(PerData == KEYS_SHOULDERS) {
				SleepComboFrames++;
				if(SleepComboFrames >= SLEEP_COMBO_FRAMES) {
					SleepComboFrames = 0;
					SleepRequested = 1;
				}
			} else {
				SleepComboFrames = 0;
			}
		}
		/* Debug builds: Start+Select pauses the game; while paused, A, B,
		   Select or the D-pad advances one frame and Start resumes */
		if(DebugMode) {
			for(;;) {
				if(DebugPaused) {
					if(PerRepeat & (KEY_A | KEY_B | KEY_SELECT))
						break;
					if(PerData & KEYS_DPAD)
						break;
					if(PerRepeat & KEY_START) {
						DebugPaused = 0;
						break;
					}
				} else {
					if(PerData != (KEY_START | KEY_SELECT))
						break;
					DebugPaused = 1;
				}
				SYNCH_WAIT_VBLANK();
				syInputUpdateKeyRepeatAndDirection();
				if(ResetRequested) {
					ResetRequested = 0;
					sySoftReset();
				}
			}
		}
		/* Wait for VBlank, then start the next frame */
		LastFrameVBlanks = FrameVBlanks;
		FrameVBlanks = 0;
		SYNCH_WAIT_VBLANK();
		syHeapSlotRelease(HEAP_SLOT_FRAME_SCRATCH);
		syOAMResetFrame();
		FrameCount++;
		PlayTimeFrames++;
		syInputUpdateKeyRepeatAndDirection();
		if(SerialExchangeActive) {
			sySerialPollStat();
			if(SerialStateWork.mode)
				SerialStateWork.transferEnabled = 1;
		}
		/* Sleep: after a second of white screen, Stop the CPU until L and R
		   wake it, then restore the screen. A request above 1 counts down
		   first. */
		if(SleepRequested && SleepDisabled == 0) {
			savedDisplayControl = REG_DISPCNT;
			savedBackdropColor = PLTT_BACKDROP;
			if(SleepRequested == 1) {
				IO_WRITE16(0, &REG_DISPCNT);
				IO_WRITE16(RGB_WHITE, &PLTT_BACKDROP);
				for(delayVBlank = 0; delayVBlank < SLEEP_WHITE_VBLANKS;
					delayVBlank++) {
					SYNCH_WAIT_VBLANK();
				}
				SleepActive = 1;
				IO_WRITE16(KEYCNT_SHOULDERS_WAKE, &REG_KEYCNT);
				Bios_SoundBiasOff();
				ASM_BIOS_STOP();
				Bios_SoundBiasOn();
				IO_WRITE16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
				SleepActive = 0;
				IO_WRITE16((Uint16)savedDisplayControl, &REG_DISPCNT);
				IO_WRITE16((Uint16)savedBackdropColor, &PLTT_BACKDROP);
				for(delayVBlank = 0; delayVBlank < SLEEP_WAKE_VBLANKS;
					delayVBlank++) {
					SYNCH_WAIT_VBLANK();
				}
				SleepRequested = 0;
				IdleFrames = 0;
			} else {
				SleepRequested--;
			}
		}
		if(ResetRequested) {
			ResetRequested = 0;
			sySoftReset();
		}
	}
	/* Put the caller's stack back */
	{
		Uint32 *size = &SavedStackSize;

		if(*size != 0) {
			/* FAKEMATCH(dead): updates of base that cancel out, under a test of
			 * base against size, keep the old base live up to the read. */
			if(base != (Uint8 *)size) {
				base++;
				base--;
			}
			ASM_STACK_POINTER_GET(base);
			/* FAKEMATCH(dead): updates of size by the new base that cancel out,
			 * before the size is read again and the copy's size by name. */
			size += (Uint32)base;
			size -= (Uint32)base;
			base -= *size;
			sp = base;
			ASM_STACK_POINTER_SET(sp);
			syDMAXCopy(SavedStack, base, DMA_COPY_WORDS(SavedStackSize >> 2),
				REG_DMA3);
			while(DMA3->ctrl & DMA_BUSY)
				;
			SavedStackSize = 0;
		}
	}
}

/* Frames a key is held before it first repeats, and between later repeats. */
#define INPUT_KEY_REPEAT_FIRST 19
#define INPUT_KEY_REPEAT_NEXT  6

/* Restarts the key repeat delay, so held keys wait the full delay again. */
void syInputResetKeyRepeat(void)
{
	PerRepeatTimer = INPUT_KEY_REPEAT_FIRST;
}

/* Works out this frame's repeating and newly pressed keys, once a frame after
   VBlank (sySynch()). Held keys repeat after INPUT_KEY_REPEAT_FIRST frames,
   then every INPUT_KEY_REPEAT_NEXT; PerRepeatDirected is that repeat with at
   most one D-pad direction, the newer of two held and none of four. */
void syInputUpdateKeyRepeatAndDirection(void)
{
	Sint32 delay = PerRepeatTimer;
	Uint32 keys;
	Uint32 axis = 0;
	Uint32 directionCount;
	Uint32 mask;

	if(delay <= 0) {
		PerRepeat = PerData;
		keys = PerRepeat;
		if(delay == 0)
			PerRepeatTimer = INPUT_KEY_REPEAT_NEXT;
		else
			PerRepeatTimer = INPUT_KEY_REPEAT_FIRST;
	} else {
		PerRepeat = 0;
		keys = PerRepeat;
	}

	if(keys != 0) {
		/* Count the D-pad directions in the repeat */
		directionCount = 0;
		if(keys & KEY_UP)
			directionCount = 1;
		if(keys & KEY_DOWN)
			directionCount++;
		if(keys & KEY_LEFT)
			directionCount++;
		if(keys & KEY_RIGHT)
			directionCount++;

		PerRepeatDirected = keys;
		switch(directionCount) {
		default:
			PerDirection = KEYS_HORIZONTAL;
			mask = KEY_WORD_BITS & ~KEYS_DPAD;
			PerRepeatDirected &= mask;
			break;
		case 0:
			PerDirection = KEYS_HORIZONTAL;
			break;
		case 1:
			PerDirection = keys & KEYS_DPAD;
			break;
		case 2:
			if((PerDirection & PerRepeatDirected) == 0)
				PerDirection = KEYS_HORIZONTAL;
			PerRepeatDirected &= PerDirection ^ KEY_WORD_BITS;
			break;
		case 3:
			if(PerDirection & KEYS_HORIZONTAL)
				axis = KEYS_HORIZONTAL;
			if(PerDirection & KEYS_VERTICAL)
				axis = KEYS_VERTICAL;
			mask = KEY_WORD_BITS ^ axis;
			PerDirection = keys & mask;
			PerRepeatDirected &= mask;
			break;
		}
	} else {
		PerRepeatDirected = keys;
	}

	PerPush = (PerData ^ PerDataLast) & PerData;
	PerDataLast = PerData;
}

#define SOUND_REVERB_CODE_COUNT 5	/* reverb codes in IwramSoundReverbCodes */
#define SOUND_REVERB_CODE_SIZE	152 /* bytes in each reverb code */

/* The VBlank interrupt handler, installed by syInit. */
void syVBlankHandler(void)
{
	Uint32 keys;
	void (*hook)(void);

	/* The display writes come first, while the blank lasts: OAM can be written
	   only in a blank, and a palette or scroll changed while lines are drawn
	   shows half changed. */
	DMA0_STOP();
	if(SerialExchangeActive != 0) {
		LinkStatusWork = sySerialExchangePayloads(
			(void *)&SerialTransfer, SerialPeerPayloads);
		sySerialStepBlockTransfer();
	}
	sySndUpdateBGMRampsStub();
	syScreenFadeUpdate();
	if(FrameRenderPending != 0) {
		if(OamCopyEnabled != 0)
			syDMAXCopy(
				((HEAP_STATE *)HeapSlots)->slots[HEAP_SLOT_FRAME_SCRATCH], OAM,
				DMA_COPY_WORDS(OAM_SIZE / 4), REG_DMA3);
		syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
		syIOWriteQueueFlushPending();
		FrameRenderPending = 0;
	}
	/* The hook runs once: it is cleared before it is called */
	hook = VBlankCallback;
	if(hook != 0) {
		VBlankCallback = 0;
		hook();
	}
	syExecuteEvent(EVENT_PASS_VBLANK);

	/* Read the keypad (its bits are 0 while pressed). */
	keys = REG_KEYINPUT ^ KEYS_MASK;
	{
		Uint32 pressed = keys & ~PerData;

		PerPush = pressed;
		PerPushLatch |= pressed;
	}
	PerData = keys;
	if(keys == 0) {
		PerRepeatTimer = INPUT_KEY_REPEAT_FIRST;
		PerRepeat = keys;
	} else if(PerData & (PerDataAtVBlank ^ KEY_WORD_BITS)) {
		PerRepeatTimer = -1;
		PerRepeat = keys;
	} else if(PerRepeatTimer > 0) {
		PerRepeatTimer--;
	}
	PerDataAtVBlank = keys;
	VBlankCount++;
	FrameVBlanks++;
	VBlankDone = 1;
	Mixer_Enter();
}

/* Copies the mixer code for reverb code `code` (enum SoundReverbCode; anything
   else loads SOUND_REVERB_MONO) from ROM into IwramSoundReverbCode. */
Sint32 sySndLoadReverbCode(Sint32 code)
{
	Sint32 index;

	index = code;
	if(code < 0 || code >= SOUND_REVERB_CODE_COUNT) {
		index = 0;
	}
	syDMAXCopy(IwramSoundReverbCodes + index * SOUND_REVERB_CODE_SIZE,
		IwramSoundReverbCode, DMA_COPY_WORDS(SOUND_REVERB_CODE_SIZE / 4),
		REG_DMA3);
	return 0;
}

/* Unused: two empty functions after the reverb loader, which nothing calls. */
UNUSED void sySndReservedNoOperationA(void)
{
}

UNUSED void sySndReservedNoOperationB(void)
{
}

/* The keypad interrupt, from the handler table: A+B+Select+Start was pressed,
   so it sets ResetRequested for the frame loop and disarms the interrupt. */
void syInputHandleKeyInterrupt(void)
{
	if(SleepActive == 0) {
		IO_WRITE16(KEYCNT_DISARMED, &REG_KEYCNT);
		ResetRequested = 1;
	}
}

#undef SYNCH_WAIT_VBLANK
#undef CARTRIDGE_RESET_MARK
#undef DISPSTAT_FLAGS
#undef VBLANK_LINES
#undef KEY_WORD_BITS
#undef INPUT_KEY_REPEAT_FIRST
#undef INPUT_KEY_REPEAT_NEXT
#undef SOUND_REVERB_CODE_COUNT
#undef SOUND_REVERB_CODE_SIZE
