/* @file */
/* Interrupt handlers, the frame wait, key repeat and the VBlank handler. */
/* Part of system.c. */

/* The word a soft reset leaves in CartridgeResetMarker: a date, 4 July 1967, */
/* its digits packed four bits each. */
#define CARTRIDGE_RESET_MARK 0x19670704

/* DISPSTAT's low byte: its flags and interrupt enables, without the VCount */
/* line above them. */
#define DISPSTAT_FLAGS 0xff

/* Installs `handler` for interrupt `irq` and enables that interrupt, or with a
 */
/* null handler disables it and installs the do-nothing handler. */
void syIntFunction(u32 irq, s32 vcount, INTERRUPT_HANDLER handler) {
	if(irq <= IRQ_GAMEPAK) {
		u32 ime;
		u32 bit;
		u32 ie;

		IRQ_LOCK(ime);
		bit = 1 << irq;
		ie = REG_IE & ~bit;
		if(handler != 0)
			ie |= bit;
		REG_IE = ie;
		if(irq <= IRQ_VCOUNT) {
			/* The display interrupts also need their DISPSTAT enable */
			u32 enable = DISPSTAT_VBLANK_INTR << irq;
			u32 keep = ~enable;
			u32 stat;

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
			IrqHandlers[irq] = syIrqIgnore;
		IRQ_UNLOCK(ime);
	}
}

/* Waits for the next VBlank; outside the sound test it first runs the music */
/* ramps and the sound driver's VBlank update, and mixes this VBlank's sound */
/* when the driver is free. */
void syWaitForFrameInterrupt(void) {
	u32 saved;
	u8 *sound;

	if(SoundVBlankStateWork.soundTestActive) {
		VBlankDone &= ~1;
		for(;;)
			if(VBlankDone & 1)
				return;
	}
	else {
		IRQ_LOCK(saved);
		VBlankDone &= ~1;
		SoundVBlankStateWork.lastVBlank = VBlankCount;
		SoundVBlankStateWork.updating = 1;
		sound = SoundInfoPointer;
		if(SOUND_INFO_BYTE(sound, transferCountdown) <= 1)
			SoundVBlankStateWork.transferCountdown =
				SOUND_INFO_BYTE(sound, transferPeriod);
		else
			SoundVBlankStateWork.transferCountdown =
				SOUND_INFO_BYTE(sound, transferCountdown) - 1;
		REG_IME = saved;
		sySndUpdateBgmRampsStub();
		if(SoundDriver_EnterVBlankUpdate() == 0) {
			void (*render)(s32) = IwramSoundRenderVBlank;

			render(8);
		}
		SoundVBlankStateWork.updating = 0;
		while(!(VBlankDone & 1))
			;
	}
}

/* Durations of the sleep: the first in frames, the others in VBlanks, since */
/* the sleep waits on syWaitForFrameInterrupt alone. */
enum {
	SLEEP_IDLE_FRAMES = 3 * 60 * 60, /* three minutes with no key held */
	SLEEP_WHITE_VBLANKS = 10, /* the white screen shown before the Stop */
	SLEEP_WAKE_VBLANKS = 10	  /* waited after waking, before the screen */
};

/* The lines REG_VCOUNT counts in VBlank, after the DISPLAY_HEIGHT drawn: it */
/* runs to DISPLAY_TOTAL_LINES - 1. */
#define VBLANK_LINES (DISPLAY_TOTAL_LINES - DISPLAY_HEIGHT)

/* Restarts the cartridge: marks CartridgeResetMarker, stops direct sound and */
/* jumps to the cartridge's start with interrupts off. `entry` is the caller's
 */
/* local for the start address. */
#define syReset(entry)                                                         \
	{                                                                          \
		CartridgeResetMarker = CARTRIDGE_RESET_MARK;                           \
		sySndSuspendDirectSndStub();                                           \
		(entry) = Cart_Start;                                                  \
		syIoWrite16(0, &REG_IME);                                              \
		ASM_CART_JUMP(entry);                                                  \
	}

/* Stops the CPU and the screen until an enabled interrupt wakes them (the BIOS
 */
/* Stop call): the sleep that L, R and Select end. */
static __inline__ void Bios_Stop(void) { ASM_BIOS_STOP(); }

/* Frames the debug CPU-load display holds a peak. */
enum { CPU_LOAD_PEAK_FRAMES = 30 };

/* The game's frame loop: runs `frames` frames and returns. */
void sySynch(s32 frames) {
	u32 i;
	u32 line;
	s32 j;
	s16 display;
	s16 backdrop;
	/* The start address, set at each restart. */
	u32 entry;

	for(i = 0; i < (u32)frames; i++) {
		TasksRunning = 1;
		syExecuteEvent(TASK_PASS_FRAME);
		TasksRunning = 0;
		if(OamCopyEnabled) {
			syRenderBuildOamList(
				syHeapSlotAlloc(HEAP_SLOT_FRAME_SCRATCH, OAM_SIZE));
		}
		else {
			OamUsageWork.used = 0;
			OamUsageWork.limit = OAM_SIZE / 4;
		}
		FrameRenderPending = 1;
		if(CpuLoadDisplayEnabled) {
			/* CPU load: the line the frame's work ended on, counted from the
			 * last */
			/* VBlank, plus 256 for each VBlank it overran; the peak is held for
			 */
			/* CPU_LOAD_PEAK_FRAMES. */
			line = *(u16 *)&REG_VCOUNT;
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
		/* Idle frames and the L+R+Select combination request sleep. */
		if(SleepDisabled == 0) {
			if(IdleSleepEnabled) {
				if(InputWork.held)
					IdleFrames = 0;
				else {
					IdleFrames++;
					if(IdleFrames > SLEEP_IDLE_FRAMES)
						SleepRequested = 1;
				}
			}
			if(InputWork.held == (KEYS_SHOULDERS | KEY_SELECT))
				SleepRequested = 1;
		}
		/* Debug builds: Start+Select pauses the game; while paused, A, B,
		 * Select */
		/* or the D-pad advances one frame and Start resumes. */
		if(DebugMode) {
			for(;;) {
				if(DebugPaused) {
					if(InputWork.repeat & (KEY_A | KEY_B | KEY_SELECT))
						break;
					if(InputWork.held & KEYS_DPAD)
						break;
					if(InputWork.repeat & KEY_START) {
						DebugPaused = 0;
						break;
					}
				}
				else {
					if(InputWork.held != (KEY_START | KEY_SELECT))
						break;
					DebugPaused = 1;
				}
				syWaitForFrameInterrupt();
				syInputUpdateKeyRepeatAndDirection();
				if(ResetRequested) {
					ResetRequested = 0;
					syReset(entry);
				}
			}
		}
		LastFrameVBlanks = FrameVBlanks;
		FrameVBlanks = 0;
		syWaitForFrameInterrupt();
		syHeapSlotRelease(HEAP_SLOT_FRAME_SCRATCH);
		syOamResetFrame();
		FrameCount++;
		PlayTimeFrames++;
		syInputUpdateKeyRepeatAndDirection();
		if(SerialExchangeActive) {
			sySerialPollStat();
			if(SerialStateWork.mode)
				SerialStateWork.transferEnabled = 1;
		}
		/* Sleep: after a white screen and the keys' release, Stop the CPU until
		 * L, */
		/* R and Select wake it, then restore the screen once they are released.
		 * A */
		/* request above 1 counts down first. */
		if(SleepRequested && SleepDisabled == 0) {
			display = REG_DISPCNT;
			backdrop = PLTT_BACKDROP;
			if(SleepRequested == 1) {
				syIoWrite16(0, &REG_DISPCNT);
				syIoWrite16(RGB_WHITE, &PLTT_BACKDROP);
				for(j = 0; j < SLEEP_WHITE_VBLANKS; j++)
					syWaitForFrameInterrupt();
				while(InputWork.held)
					syWaitForFrameInterrupt();
				SleepActive = 1;
				syIoWrite16(KEYCNT_SLEEP_WAKE, &REG_KEYCNT);
				Bios_SoundBiasOff();
				Bios_Stop();
				Bios_SoundBiasOn();
				syIoWrite16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
				syIoWrite16(0, &SleepActive);
				for(j = 0; j < SLEEP_WAKE_VBLANKS; j++)
					syWaitForFrameInterrupt();
				while(InputWork.held)
					syWaitForFrameInterrupt();
				syIoWrite16((u16)display, &REG_DISPCNT);
				syIoWrite16((u16)backdrop, &PLTT_BACKDROP);
				SleepRequested = 0;
				IdleFrames = 0;
			}
			else
				SleepRequested--;
		}
		if(ResetRequested) {
			ResetRequested = 0;
			syReset(entry);
		}
	}
}

/* Frames a key is held before it first repeats, and between later repeats. */
#define INPUT_KEY_REPEAT_FIRST 19
#define INPUT_KEY_REPEAT_NEXT  6

/* Every bit of a key word, as KEYINPUT's 16 bits. */
#define KEY_WORD_BITS 0xffff

/* Restarts the key repeat delay, so held keys wait the full delay again. */
void syInputResetKeyRepeat(void) {
	InputWork.repeatTmr = INPUT_KEY_REPEAT_FIRST;
}

/* Turns the keys the VBlank handler read into repeats, once a frame after */
/* VBlank. When the repeat timer has run out, the held keys repeat into */
/* InputWork.repeat. */
/* InputWork.dir is the same keys with at most one D-pad direction: of */
/* several held directions it keeps the one added last (prevDir */
/* remembers it), and with all four it keeps none. Last, the keys newly held */
/* since the previous frame go to pressed, and prevHeld takes held. */
void syInputUpdateKeyRepeatAndDirection(void) {
	u32 held;
	u32 keys;
	u32 axis = 0;
	u32 directionCount;
	u32 mask;
	s32 delay;

	held = InputWork.held;
	delay = InputWork.repeatTmr;
	if(delay <= 0) {
		keys = held;
		InputWork.repeat = held;
		if(delay == 0)
			InputWork.repeatTmr = INPUT_KEY_REPEAT_NEXT;
		else
			InputWork.repeatTmr = INPUT_KEY_REPEAT_FIRST;
	}
	else {
		keys = 0;
		InputWork.repeat = 0;
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

		InputWork.dir = keys;
		switch(directionCount) {
		default:
			InputWork.prevDir = KEYS_HORIZONTAL;
			mask = KEY_WORD_BITS & ~KEYS_DPAD;
			InputWork.dir &= mask;
			break;
		case 0:
			InputWork.prevDir = KEYS_HORIZONTAL;
			break;
		case 1:
			InputWork.prevDir = keys & KEYS_DPAD;
			break;
		case 2:
			if((InputWork.prevDir & InputWork.dir) == 0)
				InputWork.prevDir = KEYS_HORIZONTAL;
			InputWork.dir &= InputWork.prevDir ^ KEY_WORD_BITS;
			break;
		case 3:
			if(InputWork.prevDir & KEYS_HORIZONTAL)
				axis = KEYS_HORIZONTAL;
			if(InputWork.prevDir & KEYS_VERTICAL)
				axis = KEYS_VERTICAL;
			mask = KEY_WORD_BITS ^ axis;
			InputWork.prevDir = keys & mask;
			InputWork.dir &= mask;
			break;
		}
	}
	else {
		InputWork.dir = keys;
	}

	InputWork.pressed = held & ~InputWork.prevHeld;
	syIoWrite16(held, &InputWork.prevHeld);
}

/* Copies 16 bytes from `source` to `destination` with one four-register load */
/* and store. */
static __inline__ void Copy16Bytes(const void *source,
								   volatile void *destination) {
	ASM_REGISTER(const void *src, "r0") = source;
	ASM_REGISTER(volatile void *dst, "r1") = destination;
	ASM_COPY_WORDS_4(src, dst);
}

/* The VBlank interrupt handler, installed by syInit. */
void syVBlankHandler(void) {
	u32 keys;
	u32 vblankCount;
	void (*hook)(void);

	/* The display writes come first, while the blank lasts: OAM can be written
	 */
	/* only in a blank, and a palette or scroll changed while lines are drawn */
	/* shows half changed. */
	DMA0_STOP();
	if(SerialExchangeActive != 0) {
		LinkStatusWork = sySerialExchangePayloads((void *)&SerialTransfer,
												  SerialPeerPayloads);
		sySerialStepBlockTransfer();
	}
	SoundDriver_VSyncRefresh();
	if(ScreenFadeVBlanks != 0)
		syScrFadeUpdate();
	if(FrameRenderPending != 0) {
		FrameRenderPending = 0;
		if(OamUsageWork.used != 0)
			syDMAXCopy(Ram_HeapSlots->frmScratch, OAM,
					   DMA_COPY_WORDS(0) + OamUsageWork.used, REG_DMA3);
		Copy16Bytes(BgScrollWork, REG_BG0HOFS);
		syIoWriteQueueFlushPending();
	}
	/* The hook runs once: it is cleared before it is called. */
	hook = VBlankCallback;
	if(hook != 0) {
		VBlankCallback = 0;
		hook();
	}
	syTaskRunPassBeforeFrameTasks(TASK_PASS_VBLANK);

	/* Read the keypad (its bits are 0 while pressed). */
	keys = REG_KEYINPUT ^ KEYS_MASK;
	{
		/* volatile: the code this interrupt broke into reads the same state. */
		volatile INPUT_STATE *input = &InputWork;
		u32 pressed = keys & ~input->held;

		input->pressed = pressed;
		input->pendingPressed |= pressed;
		input->held = keys;
		if(keys == 0) {
			input->repeatTmr = INPUT_KEY_REPEAT_FIRST;
			input->repeat = keys;
		}
		else if(input->held & (input->prevVblankHeld ^ KEY_WORD_BITS)) {
			input->repeatTmr = -1;
			input->repeat = keys;
		}
		else if(input->repeatTmr > 0) {
			input->repeatTmr--;
		}
	}
	{
		/* volatile: the frame loop waits on the count this interrupt advances.
		 */
		volatile u32 *count;
		volatile struct SoundVBlankState *sound;

		count = &VBlankCount;
		((volatile INPUT_STATE *)&InputWork)->prevVblankHeld = keys;
		sound = &SoundVBlankStateWork;
		vblankCount = *count;
		if(sound->lastVBlank != (u8)vblankCount && sound->updating == 0) {
			sound->transferCountdown =
				SOUND_INFO_BYTE(SoundInfoPointer, transferCountdown);
			sySndUpdateBgmRampsStub();
			if(SoundDriver_EnterVBlankUpdate() == 0) {
				void (*render)(s32) = IwramSoundRenderVBlank;

				render(8);
			}
			sound->lastVBlank = vblankCount;
		}
		*count = vblankCount + 1;
	}
	syIoWrite32(FrameVBlanks + 1, &FrameVBlanks);
	VBlankDone = 1;
}

#define SOUND_REVERB_CODE_COUNT 5 /* the codes of enum SoundReverbCode */

/* Where The Broken Seal copies the mixer code for reverb code `code` (enum */
/* SoundReverbCode) into IWRAM, The Lost Age keeps only the range check and */
/* returns 0, so its callers (the battle engine, the lobby, the sound test and
 */
/* the debug menu) change nothing. */
s32 sySndLoadReverbCode(s32 code) {
	if((u32)code > SOUND_REVERB_CODE_COUNT - 1)
		code = 0;
	return 0;
}

/* Empty: nothing calls it or its neighbour. */
UNUSED void sySndReservedNoOpA(void) {}

/* Empty, as sySndReservedNoOpA. */
UNUSED void sySndReservedNoOpB(void) {}

/* The keypad interrupt, from the handler table: A+B+Select+Start was pressed,
 */
/* so it sets ResetRequested for the frame loop and disarms the interrupt. */
void syInputHandleKeyInterrupt(void) {
	if(SleepActive == 0) {
		syIoWrite16(KEYCNT_DISARMED, &REG_KEYCNT);
		ResetRequested = 1;
	}
}

#undef CARTRIDGE_RESET_MARK
#undef VBLANK_LINES
#undef DISPSTAT_FLAGS
#undef KEY_WORD_BITS
#undef INPUT_KEY_REPEAT_FIRST
#undef INPUT_KEY_REPEAT_NEXT
#undef SOUND_REVERB_CODE_COUNT
