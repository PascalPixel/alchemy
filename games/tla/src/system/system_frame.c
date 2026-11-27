/* Interrupt handlers, the frame wait, key repeat and the VBlank handler. Part
   of system.c. */

/* The word a soft reset leaves in CartridgeResetMarker: a date, 4 July 1967,
   its digits packed four bits each. */
#define CARTRIDGE_RESET_MARK 0x19670704

/* DISPSTAT's low byte: its flags and interrupt enables, without the VCount line
   above them. */
#define DISPSTAT_FLAGS 0xff

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

/* Waits for the next VBlank; outside the sound test it first runs the music
   ramps and the sound driver's VBlank update, and mixes this VBlank's sound
   when the driver is free. */
void syWaitForFrameInterrupt(void)
{
	Uint32 saved;
	Uint8 *sound;

	if(SoundVBlankStateWork.soundTestActive) {
		VBlankDone &= ~1;
		for(;;)
			if(VBlankDone & 1)
				return;
	} else {
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
		sySndUpdateBGMRampsStub();
		if(SoundDriver_EnterVBlankUpdate() == 0) {
			void (*render)(Sint32) = IwramSoundRenderVBlank;

			render(8);
		}
		SoundVBlankStateWork.updating = 0;
		while(!(VBlankDone & 1))
			;
	}
}

/* Durations of the sleep: the first in frames, the others in VBlanks, since the
   sleep waits on syWaitForFrameInterrupt alone. */
enum {
	SLEEP_IDLE_FRAMES = 3 * 60 * 60, /* three minutes with no key held */
	SLEEP_WHITE_VBLANKS = 10, /* the white screen shown before the Stop */
	SLEEP_WAKE_VBLANKS = 10	  /* waited after waking, before the screen */
};

/* The lines REG_VCOUNT counts in VBlank, after the DISPLAY_HEIGHT drawn: it
   runs to DISPLAY_TOTAL_LINES - 1. */
#define VBLANK_LINES (DISPLAY_TOTAL_LINES - DISPLAY_HEIGHT)

/* Restarts the cartridge: marks CartridgeResetMarker, stops direct sound and
   jumps to the cartridge's start with interrupts off. `entry` is the caller's
   local for the start address. */
#define syReset(entry) \
	{ \
		CartridgeResetMarker = CARTRIDGE_RESET_MARK; \
		sySndSuspendDirectSndStub(); \
		(entry) = Cart_Start; \
		syIOWrite16(0, &REG_IME); \
		ASM_CART_JUMP(entry); \
	}

/* Stops the CPU and the screen until an enabled interrupt wakes them (the BIOS
   Stop call): the sleep that L, R and Select end. */
static __inline__ void Bios_Stop(void)
{
	ASM_BIOS_STOP();
}

/* Frames the debug CPU-load display holds a peak. */
enum { CPU_LOAD_PEAK_FRAMES = 30 };

/* The game's frame loop: runs `frames` frames and returns. */
void sySynch(Sint32 frames)
{
	Uint32 i;
	Uint32 line;
	Sint32 j;
	Sint16 display;
	Sint16 backdrop;
	/* The start address, set at each restart. */
	Uint32 entry;

	for(i = 0; i < (Uint32)frames; i++) {
		EventsRunning = 1;
		syExecuteEvent(EVENT_PASS_FRAME);
		EventsRunning = 0;
		if(OamCopyEnabled) {
			syRenderBuildOAMList(
				syHeapSlotAlloc(HEAP_SLOT_FRAME_SCRATCH, OAM_SIZE));
		} else {
			OamUsageWork.used = 0;
			OamUsageWork.limit = OAM_SIZE / 4;
		}
		FrameRenderPending = 1;
		if(CpuLoadDisplayEnabled) {
			/* CPU load: the line the frame's work ended on, counted from the
			   last VBlank, plus 256 for each VBlank it overran; the peak is
			   held for CPU_LOAD_PEAK_FRAMES. */
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
		/* Idle frames and the L+R+Select combination request sleep. */
		if(SleepDisabled == 0) {
			if(IdleSleepEnabled) {
				if(PerWork.data)
					IdleFrames = 0;
				else {
					IdleFrames++;
					if(IdleFrames > SLEEP_IDLE_FRAMES)
						SleepRequested = 1;
				}
			}
			if(PerWork.data == (KEYS_SHOULDERS | KEY_SELECT))
				SleepRequested = 1;
		}
		/* Debug builds: Start+Select pauses the game; while paused, A, B,
		   Select or the D-pad advances one frame and Start resumes. */
		if(DebugMode) {
			for(;;) {
				if(DebugPaused) {
					if(PerWork.repeat & (KEY_A | KEY_B | KEY_SELECT))
						break;
					if(PerWork.data & KEYS_DPAD)
						break;
					if(PerWork.repeat & KEY_START) {
						DebugPaused = 0;
						break;
					}
				} else {
					if(PerWork.data != (KEY_START | KEY_SELECT))
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
		syOAMResetFrame();
		FrameCount++;
		PlayTimeFrames++;
		syInputUpdateKeyRepeatAndDirection();
		if(SerialExchangeActive) {
			sySerialPollStat();
			if(SerialStateWork.mode)
				SerialStateWork.transferEnabled = 1;
		}
		/* Sleep: after a white screen and the keys' release, Stop the CPU until
		   L, R and Select wake it, then restore the screen once they are
		   released. A request above 1 counts down first. */
		if(SleepRequested && SleepDisabled == 0) {
			display = REG_DISPCNT;
			backdrop = PLTT_BACKDROP;
			if(SleepRequested == 1) {
				syIOWrite16(0, &REG_DISPCNT);
				syIOWrite16(RGB_WHITE, &PLTT_BACKDROP);
				for(j = 0; j < SLEEP_WHITE_VBLANKS; j++)
					syWaitForFrameInterrupt();
				while(PerWork.data)
					syWaitForFrameInterrupt();
				SleepActive = 1;
				syIOWrite16(KEYCNT_SLEEP_WAKE, &REG_KEYCNT);
				Bios_SoundBiasOff();
				Bios_Stop();
				Bios_SoundBiasOn();
				syIOWrite16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
				syIOWrite16(0, &SleepActive);
				for(j = 0; j < SLEEP_WAKE_VBLANKS; j++)
					syWaitForFrameInterrupt();
				while(PerWork.data)
					syWaitForFrameInterrupt();
				syIOWrite16((Uint16)display, &REG_DISPCNT);
				syIOWrite16((Uint16)backdrop, &PLTT_BACKDROP);
				SleepRequested = 0;
				IdleFrames = 0;
			} else
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
void syInputResetKeyRepeat(void)
{
	PerWork.repeatTmr = INPUT_KEY_REPEAT_FIRST;
}

/* Turns the keys the VBlank handler read into repeats, once a frame after
   VBlank. When the repeat timer has run out, the held keys repeat into
   PerWork.repeat. PerWork.dir is the same keys with at most one D-pad
   direction: of several held directions it keeps the one added last (prevDir
   remembers it), and with all four it keeps none. Last, the keys newly held
   since the previous frame go to pressed, and prevData takes held. */
void syInputUpdateKeyRepeatAndDirection(void)
{
	Uint32 held;
	Uint32 keys;
	Uint32 axis = 0;
	Uint32 directionCount;
	Uint32 mask;
	Sint32 delay;

	held = PerWork.data;
	delay = PerWork.repeatTmr;
	if(delay <= 0) {
		keys = held;
		PerWork.repeat = held;
		if(delay == 0)
			PerWork.repeatTmr = INPUT_KEY_REPEAT_NEXT;
		else
			PerWork.repeatTmr = INPUT_KEY_REPEAT_FIRST;
	} else {
		keys = 0;
		PerWork.repeat = 0;
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

		PerWork.dir = keys;
		switch(directionCount) {
		default:
			PerWork.prevDir = KEYS_HORIZONTAL;
			mask = KEY_WORD_BITS & ~KEYS_DPAD;
			PerWork.dir &= mask;
			break;
		case 0:
			PerWork.prevDir = KEYS_HORIZONTAL;
			break;
		case 1:
			PerWork.prevDir = keys & KEYS_DPAD;
			break;
		case 2:
			if((PerWork.prevDir & PerWork.dir) == 0)
				PerWork.prevDir = KEYS_HORIZONTAL;
			PerWork.dir &= PerWork.prevDir ^ KEY_WORD_BITS;
			break;
		case 3:
			if(PerWork.prevDir & KEYS_HORIZONTAL)
				axis = KEYS_HORIZONTAL;
			if(PerWork.prevDir & KEYS_VERTICAL)
				axis = KEYS_VERTICAL;
			mask = KEY_WORD_BITS ^ axis;
			PerWork.prevDir = keys & mask;
			PerWork.dir &= mask;
			break;
		}
	} else {
		PerWork.dir = keys;
	}

	PerWork.push = held & ~PerWork.prevData;
	syIOWrite16(held, &PerWork.prevData);
}

/* Copies 16 bytes from `source` to `destination` with one four-register load
   and store. */
static __inline__ void Copy16Bytes(
	const void *source, volatile void *destination)
{
	ASM_REGISTER(const void *src, "r0") = source;
	ASM_REGISTER(volatile void *dst, "r1") = destination;
	ASM_COPY_WORDS_4(src, dst);
}

/* The VBlank interrupt handler, installed by syInit. */
void syVBlankHandler(void)
{
	Uint32 keys;
	Uint32 vblankCount;
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
	SoundDriver_VSyncRefresh();
	if(ScreenFadeVBlanks != 0)
		syScreenFadeUpdate();
	if(FrameRenderPending != 0) {
		FrameRenderPending = 0;
		if(OamUsageWork.used != 0)
			syDMAXCopy(Ram_HeapSlots->frmScratch, OAM,
				DMA_COPY_WORDS(0) + OamUsageWork.used, REG_DMA3);
		Copy16Bytes(BgScrollWork, REG_BG0HOFS);
		syIOWriteQueueFlushPending();
	}
	/* The hook runs once: it is cleared before it is called. */
	hook = VBlankCallback;
	if(hook != 0) {
		VBlankCallback = 0;
		hook();
	}
	syRunPassBeforeFrameEvents(EVENT_PASS_VBLANK);

	/* Read the keypad (its bits are 0 while pressed). */
	keys = REG_KEYINPUT ^ KEYS_MASK;
	{
		/* volatile: the code this interrupt broke into reads the same state. */
		volatile INPUT_STATE *input = &PerWork;
		Uint32				  pressed = keys & ~input->data;

		input->push = pressed;
		input->pendingPush |= pressed;
		input->data = keys;
		if(keys == 0) {
			input->repeatTmr = INPUT_KEY_REPEAT_FIRST;
			input->repeat = keys;
		} else if(input->data & (input->prevVblankData ^ KEY_WORD_BITS)) {
			input->repeatTmr = -1;
			input->repeat = keys;
		} else if(input->repeatTmr > 0) {
			input->repeatTmr--;
		}
	}
	{
		/* volatile: the frame loop waits on the count this interrupt
		   advances. */
		volatile Uint32					 *count;
		volatile struct SoundVBlankState *sound;

		count = &VBlankCount;
		((volatile INPUT_STATE *)&PerWork)->prevVblankData = keys;
		sound = &SoundVBlankStateWork;
		vblankCount = *count;
		if(sound->lastVBlank != (Uint8)vblankCount && sound->updating == 0) {
			sound->transferCountdown =
				SOUND_INFO_BYTE(SoundInfoPointer, transferCountdown);
			sySndUpdateBGMRampsStub();
			if(SoundDriver_EnterVBlankUpdate() == 0) {
				void (*render)(Sint32) = IwramSoundRenderVBlank;

				render(8);
			}
			sound->lastVBlank = vblankCount;
		}
		*count = vblankCount + 1;
	}
	syIOWrite32(FrameVBlanks + 1, &FrameVBlanks);
	VBlankDone = 1;
}

#define SOUND_REVERB_CODE_COUNT 5 /* the codes of enum SoundReverbCode */

/* Where The Broken Seal copies the mixer code for reverb code `code` (enum
   SoundReverbCode) into IWRAM, The Lost Age keeps only the range check and
   returns 0, so its callers (the battle engine, the lobby, the sound test and
   the debug menu) change nothing. */
Sint32 sySndLoadReverbCode(Sint32 code)
{
	if((Uint32)code > SOUND_REVERB_CODE_COUNT - 1)
		code = 0;
	return 0;
}

/* Empty: nothing calls it or its neighbour. */
UNUSED void sySndReservedNoOperationA(void)
{
}

/* Empty, as sySndReservedNoOperationA. */
UNUSED void sySndReservedNoOperationB(void)
{
}

/* The keypad interrupt, from the handler table: A+B+Select+Start was pressed,
   so it sets ResetRequested for the frame loop and disarms the interrupt. */
void syInputHandleKeyInterrupt(void)
{
	if(SleepActive == 0) {
		syIOWrite16(KEYCNT_DISARMED, &REG_KEYCNT);
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
