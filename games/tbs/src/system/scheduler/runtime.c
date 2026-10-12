#include "unit/system/save/savedata.h"
#include "unit/system/scheduler/mathtbl.h"
#include "unit/system/scheduler/numtext.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "ramcode.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"
#include "irq.h"
#include "types.h"
#include "scene.h"
#include "resource.h"
#include "resource_ids.h"
#include "global_cells.h"
#include "cartboot.h"
#include "ram_buffer.h"
#include "serial_runtime.h"
#include "frame.h"
#include "keystate.h"
#include "input.h"
#include "oam.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "irqlock.h"
#include "iowrite.h"
#include "lib.h"
#include "fixed_math.h"
#include "affine.h"
#include "iwram_call.h"
#include "vram_block.h"
#include "string.h"
#include "low_runtime.h"
#include "vram_tab.h"
#include "layout_guard.h"
#include "edition.h"
#include "transform.h"
#include "project.h"
#include "save_state.h"
#include "runtime_interfaces.h"
#include "flash.h"
#include "audio_engine_symbols.h"

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
#define HALT_ON_GAMEPAK_IRQ 1
#endif

void System_Initialize(void)
{
	volatile u32 zero;

	{
		volatile u16 *dma0 = REG_DMA0;

		dma0[5] &= 0xc5ff;
		dma0[5] &= 0x7fff;
		dma0[5];
	}
	Io_Write16(WAITCNT_GAME, &REG_WAITCNT);
	zero = 0;
	Dma_Set((const void *)&zero, (void *)0x03000000, 0x85001e00, REG_DMA3);
	Runtime_InitializeHeap();
	Runtime_InstallIwramAndIrqs();
	*(u32 *)&gIoWriteQueue.count = 0;
	gDebugTextEnabled = 0;
	gDebugMode = 0;
	gCpuLoadDisplayEnabled = 0;
	Resource_LoadWorkHeader();
	Ui_LoadWindowGraphics();
	Bg0_ClearTilemap();
	REG_DISPCNT = 0x140;
	Runtime_SetIrqHandler(0, 1, (void (*)(void))System_VBlankHandler);
#if defined(HALT_ON_GAMEPAK_IRQ)
	Runtime_SetIrqHandler(13, 1, IwramHalt);
#endif
	Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
	Audio_InitializeRuntimeDefaultsFar();
	Resource_InitializeTable();
	Scheduler_ResetTaskTable();
	gCartridgeResetMarker = 0;
	gOamCopyEnabled = 1;
	gSleepDisabled = 0;
	WaitFrames(10);
	Game_ResetForNewGameFar(0);
}

void Runtime_IdleForever(void)
{
	for (;;)
	{
		(void)gKeyState;
		WaitFrames(1);
	}
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)

s32 System_GetLanguage(void)
{
	s32 language;

	switch (Rom_LanguageCode)
	{
	case 'D':
		language = 1;
		break;
	case 'F':
		language = 2;
		break;
	case 'S':
		language = 3;
		break;
	case 'I':
		language = 4;
		break;
	default:
		language = 0;
		break;
	}
	return language;
}
#endif

void RuntimeDispatch_ReservedNoOpA(void)
{
}

void RuntimeDispatch_ReservedNoOpB(void)
{
}

void RuntimeDispatch_ReservedNoOpC(void)
{
}

void RuntimeDispatch_ReservedNoOpD(void)
{
}

void RuntimeDispatch_ReservedNoOpE(void)
{
}

s32 RuntimeDispatch_ReturnZero(s32 unused0, const u8 *unused1)
{
	return 0;
}

struct ResourceWork
{
	u32 header[3];
	u32 cursor;
};

void Resource_LoadWorkHeader(void)
{
	const void			*header;
	struct ResourceWork *work;

	header = Resource_GetTableEntry((u32)&ResourceId_BuildStamp);
	work = (struct ResourceWork *)&IwramHi_Data1;
	Dma_Set(header, work, 0x84000003, REG_DMA3);
	work->cursor = 0;
}

void RuntimeDispatch_NoOpHook(s32 unused)
{
}

void *Resource_GetTableEntry(s32 index)
{
	return Resource_DirectoryTable[index];
}

void PackedTable_AdjustMarkedOffsets(u16 *entries, s32 byte_length)
{
	s32 entry_count;
	s32 entry_index;

	entry_count = byte_length >> 1;
	entry_index = 1;
	entries++;
	if (entry_index < entry_count)
	{
		u32 next_marker_mask;
		u32 current_marker_mask;
		u32 payload_mask = 0x7ff;
		next_marker_mask = 0xf800;
		current_marker_mask = 0xf000;
		entries -= 2;
		do
		{
			u16 current_entry;
			u16 next_entry;
			u32 next_marker_bits;
			u32 current_marker_bits;

			next_entry = entries[2];
			next_marker_bits = next_entry & next_marker_mask;
			entries++;
			if (next_marker_bits == next_marker_mask)
			{
				current_entry = entries[0];
				current_marker_bits = current_entry & next_marker_bits;
				if (current_marker_bits == current_marker_mask)
				{
					u32 packed_offset;
					u32 adjusted_current;
					u32 adjusted_next;

					packed_offset =
						(((current_entry & payload_mask) << 12) | ((next_entry & payload_mask) * 2))
						- (entry_index * 2);
					adjusted_current = ((packed_offset >> 12) & payload_mask) | current_marker_bits;
					adjusted_next = ((packed_offset >> 1) & payload_mask) | next_marker_bits;
					entries[0] = adjusted_current;
					entries[1] = adjusted_next;
				}
			}
			entry_index++;
		} while (entry_index < entry_count);
	}
}

void Resource_LoadCode(u32 index, void *destination)
{
	void (*routine)(void *, s32);
	s32 end;

	end = Resource_DecodeType01(Resource_GetTableEntry(index), destination);
	RamCode_Load(routine, Resource_PatchThumbBranch, Resource_PatchThumbBranchCodeSize);
	routine(destination, end);
	MemFree(routine);
}

void Runtime_IgnoreInterrupt(void)
{
}

void Runtime_InstallIwramAndIrqs(void)
{
	u8 *iwram;

	Io_Write16(0, &REG_IME);
	iwram = IwramIrqMain;
	Dma_Set(IwramRuntime_Rom, iwram, 0x84000500, REG_DMA3);
	gIrqVector = iwram;
	Dma_Set(Runtime_IrqHandlers, gIrqHandlers, 0x8400000e, REG_DMA3);
	Io_Write16(0, &REG_DISPSTAT);
	Io_Write16(0xc3ff, &REG_KEYCNT);
#if defined(HALT_ON_GAMEPAK_IRQ)
	Io_Write16(0x3001, &REG_IE);
#else
	Io_Write16(0x1001, &REG_IE);
#endif
	Io_Write16(1, &REG_IME);
}

static __inline__ void System_SoftReset(void)
{
	/* FAKEMATCH: a void-returning entry pointer places the target in r1; the value-returning pointer preserves the measured r0 call. */
	s32 (*reset)(void) = (s32 (*)(void))Cart_Restart;

	gCartridgeResetMarker = 0x19670704;
	Io_Write16(0, &REG_IME);
	reset();
}

#define VBlankIntrWait()                                \
	{                                                   \
		gVBlankDone &= ~1;                              \
		do                                              \
		{                                               \
			CAMELOT_ASM("Halt, which no C compiles to") \
			__asm__ volatile("swi 2");                  \
		} while (!(gVBlankDone & 1));                   \
	}

void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler)
{
	if (irq <= 13)
	{
		u32 ime;
		u32 bit;
		u32 ie;

		IRQ_LOCK(ime);
		bit = 1 << irq;
		ie = REG_IE & ~bit;
		if (handler != 0)
			ie |= bit;
		REG_IE = ie;
		if (irq <= 2)
		{
			u32 enable = 8 << irq;
			u32 keep = ~enable;
			u32 stat;

			if (irq == 2)
			{
				enable |= vcount << 8;
				keep &= 0xff;
			}
			stat = REG_DISPSTAT & keep;
			if (handler != 0)
				stat |= enable;
			REG_DISPSTAT = stat;
		}
		if (handler != 0)
			gIrqHandlers[irq] = handler;
		else
			gIrqHandlers[irq] = Runtime_IgnoreInterrupt;
		IRQ_UNLOCK(ime);
	}
}

void WaitFrames(s32 frames)
{
	u8 *sp;
	u32 i;
	u8 *base;
	u32 line;
	s32 j;
	s16 dispcnt;
	s16 backdrop;

	CAMELOT_ASM("read the hardware stack pointer for the proven stack switch")
	__asm__ volatile("mov %0, sp" : "=r"(sp));

	base = sp;
	if (base < Ram_FrameWaitStackTop)
	{
		u8 *top = Ram_FrameWaitStackTop;

		gSavedStackSize = top - base;
		Dma_Set(base, gSavedStack, (gSavedStackSize >> 2) | DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
		/* FAKEMATCH: the tied top operand retains its DMA lifetime; an input-only operand adds a copy through ip. */
		CAMELOT_ASM("the stack switch, a write to sp no C can express")
		__asm__ volatile("mov sp, %0" : "+r"(top) : : "sp", "memory");
	}
	for (i = 0; i < frames; i++)
	{
		gSchedulerStatus = 1;
		Runtime_InvokeCallbacksByKey(0xc80);
		gSchedulerStatus = 0;
		Render_BuildOamList(Runtime_AllocateHeapBlock(52, 0x400));
		gFrameRenderPending = 1;
		if (gCpuLoadDisplayEnabled)
		{
			line = *(u16 *)&REG_VCOUNT;
			if (line >= 160)
				line -= 160;
			else
				line += 68;
			line += (gLagFrameCount - 1) << 8;
			if (gCpuLoadTimer == 0)
				gCpuLoadPeak = 0;
			else
				gCpuLoadTimer--;
			if (gCpuLoadPeak < line)
			{
				gCpuLoadPeak = line;
				gCpuLoadTimer = 30;
			}
		}
		if (gSleepDisabled == 0)
		{
			if (gOptionMirror)
			{
				if (gKeysHeld)
					gPostLoadCounter = 0;
				else
				{
					gPostLoadCounter++;
					if (gPostLoadCounter > 3 * 60 * 60)
						gSleepRequested = 1;
				}
			}
			if (gKeysHeld == KEYS_SHOULDERS)
			{
				gSleepComboFrames++;
				if (gSleepComboFrames >= 180)
				{
					gSleepComboFrames = 0;
					gSleepRequested = 1;
				}
			}
			else
			{
				gSleepComboFrames = 0;
			}
		}
		if (gDebugMode)
		{
			for (;;)
			{
				if (gDebugPaused)
				{
					if (gKeysRepeat & (KEY_A | KEY_B | KEY_SELECT))
						break;
					if (gKeysHeld & KEYS_DPAD)
						break;
					if (gKeysRepeat & KEY_START)
					{
						gDebugPaused = 0;
						break;
					}
				}
				else
				{
					if (gKeysHeld != (KEY_START | KEY_SELECT))
						break;
					gDebugPaused = 1;
				}
				CAMELOT_ASM("Halt in VBlankIntrWait")
				VBlankIntrWait();
				Input_UpdateKeyRepeatAndDirection();
				if (gResetRequested)
				{
					gResetRequested = 0;
					System_SoftReset();
				}
			}
		}
		gLagFramesShown = gLagFrameCount;
		gLagFrameCount = 0;
		CAMELOT_ASM("Halt in VBlankIntrWait")
		VBlankIntrWait();
		Runtime_ReleaseHeapBlock(52);
		Graphics_ResetFrameState();
		gFrameCount++;
		gLoadedStateWord++;
		Input_UpdateKeyRepeatAndDirection();
		if (gSerialExchangeActive)
		{
			SerialRuntime_PollStatus();
			if (gSerialRuntime.mode)
				gSerialRuntime.transfer_enabled = 1;
		}
		if (gSleepRequested && gSleepDisabled == 0)
		{
			dispcnt = REG_DISPCNT;
			backdrop = PLTT_BACKDROP;
			if (gSleepRequested == 1)
			{
				Io_Write16(0, &REG_DISPCNT);
				Io_Write16(0x7fff, &PLTT_BACKDROP);
				for (j = 0; j < 60; j++)
					CAMELOT_ASM("Halt in VBlankIntrWait")
				VBlankIntrWait();
				gSleepActive = 1;
				Io_Write16(KEYCNT_SHOULDERS_WAKE, &REG_KEYCNT);
				Bios_SoundBiasOff();
				CAMELOT_ASM("Stop, which no C compiles to")
				__asm__ volatile("swi 3");
				Bios_SoundBiasOn();
				Io_Write16(KEYCNT_SOFT_RESET, &REG_KEYCNT);
				gSleepActive = 0;
				Io_Write16((u16)dispcnt, &REG_DISPCNT);
				Io_Write16((u16)backdrop, &PLTT_BACKDROP);
				for (j = 0; j < 10; j++)
					CAMELOT_ASM("Halt in VBlankIntrWait")
				VBlankIntrWait();
				gSleepRequested = 0;
				gPostLoadCounter = 0;
			}
			else
			{
				gSleepRequested--;
			}
		}
		if (gResetRequested)
		{
			gResetRequested = 0;
			System_SoftReset();
		}
	}
	{
		u32 *size = &gSavedStackSize;

		if (*size)
		{
			/* FAKEMATCH: tying the old base to the hardware read keeps the eight-byte frame and removes an extra copy through ip. */
			CAMELOT_ASM("read the hardware stack pointer for its restore copy")
			__asm__ volatile("mov %0, sp" : "+r"(base));
			/* FAKEMATCH: needed to match: the volatile reread keeps this saved-size
             * load separate from the conditional read (7 aligned differences
             * without this qualification). */
			base -= *(volatile u32 *)size;
			sp = base;
			CAMELOT_ASM("restore the hardware stack pointer after the proven stack copy")
			__asm__ volatile("mov sp, %0" : : "r"(sp) : "sp", "memory");
			/* FAKEMATCH: clearing the old address and restoring a separate
             * pointer in complementary conditions ends the restore CSE block.
             * The conditions vanish, retaining the size-address reload in r3
             * and its load into r2. Reusing size instead of next_size changes
             * that pair to r2/r2 (4 aligned differences, same 0x434 extent);
             * deleting the empty barrier alone gives 24 differences. */
			size = NULL;
			{
				u32 *next_size = NULL;

				if (sp)
					next_size = &gSavedStackSize;
				if (!sp)
					next_size = &gSavedStackSize;
				Dma_Set(gSavedStack, base, (*next_size >> 2) | DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
			}
			while (DMA3->control & DMA_ENABLE32)
				;
			gSavedStackSize = 0;
		}
	}
}

#define KEY_REPEAT_FIRST 19
#define KEY_REPEAT_NEXT 6

void Runtime_SetMainState19(void)
{
	gKeyRepeatTimer = KEY_REPEAT_FIRST;
}

void Input_UpdateKeyRepeatAndDirection(void)
{
	s32 delay = gKeyRepeatTimer;
	u32 keys;
	u32 axis = 0;
	u32 dirs;
	u32 mask;

	if (delay <= 0)
	{
		gKeysRepeat = gKeysHeld;
		keys = gKeysRepeat;
		if (delay == 0)
			gKeyRepeatTimer = KEY_REPEAT_NEXT;
		else
			gKeyRepeatTimer = KEY_REPEAT_FIRST;
	}
	else
	{
		gKeysRepeat = 0;
		keys = gKeysRepeat;
	}

	if (keys != 0)
	{
		dirs = 0;
		if (keys & KEY_UP)
			dirs = 1;
		if (keys & KEY_DOWN)
			dirs++;
		if (keys & KEY_LEFT)
			dirs++;
		if (keys & KEY_RIGHT)
			dirs++;

		gKeysRepeatDirected = keys;
		switch (dirs)
		{
		default:
			gKeysDirection = KEYS_HORIZONTAL;
			mask = 0xffff & ~KEYS_DPAD;
			gKeysRepeatDirected &= mask;
			break;
		case 0:
			gKeysDirection = KEYS_HORIZONTAL;
			break;
		case 1:
			gKeysDirection = keys & KEYS_DPAD;
			break;
		case 2:
			if ((gKeysDirection & gKeysRepeatDirected) == 0)
				gKeysDirection = KEYS_HORIZONTAL;
			gKeysRepeatDirected &= gKeysDirection ^ 0xffff;
			break;
		case 3:
			if (gKeysDirection & KEYS_HORIZONTAL)
				axis = KEYS_HORIZONTAL;
			if (gKeysDirection & KEYS_VERTICAL)
				axis = KEYS_VERTICAL;
			mask = 0xffff ^ axis;
			gKeysDirection = keys & mask;
			gKeysRepeatDirected &= mask;
			break;
		}
	}
	else
	{
		gKeysRepeatDirected = keys;
	}

	gKeyState = (gKeysHeld ^ gKeysHeldLast) & gKeysHeld;
	gKeysHeldLast = gKeysHeld;
}

#define SOUND_PRESET_COUNT 5
#define SOUND_PRESET_SIZE 152

void System_VBlankHandler(void)
{
	u32 keys;
	void (*hook)(void);

	{
		volatile u16 *dma0 = REG_DMA0;

		dma0[5] &= ~DMA_START_HBLANK_REPEAT;
		dma0[5] &= ~DMA_ENABLE;
		dma0[5];
	}
	if (gSerialExchangeActive != 0)
	{
		u16 *status = (u16 *)&gLinkStatus;
		*status = SerialRuntime_ExchangePayloads((void *)&gSerialTransfer, gSerialPeerPayloads);
		SerialRuntime_StepBlockTransfer();
	}
	MusicPlayer_StepVolumeAndPitchTowardTargetsFar();
	BlendTransition_Update();
	if (gFrameRenderPending != 0)
	{
		if (gOamCopyEnabled != 0)
			Dma_Set(((union HeapState *)gWorkSlot)->slots[52], OAM,
				DMA_ENABLE32 | DMA_32BIT | 0x100, REG_DMA3);
		Dma_Set(gBgScroll, REG_BG0HOFS, DMA_ENABLE32 | DMA_32BIT | 4, REG_DMA3);
		IoWriteQueue_FlushPending();
		gFrameRenderPending = 0;
	}
	hook = gVBlankHook;
	if (hook != 0)
	{
		gVBlankHook = 0;
		hook();
	}
	Runtime_InvokeCallbacksByKey(0x480);
	keys = REG_KEYINPUT ^ KEYS_MASK;
	{
		u32 pressed = keys & ~gKeysHeld;

		gKeyState = pressed;
		gKeysPressedLatch |= pressed;
	}
	gKeysHeld = keys;
	if (keys == 0)
	{
		gKeyRepeatTimer = KEY_REPEAT_FIRST;
		gKeysRepeat = keys;
	}
	else if (gKeysHeld & (gKeysHeldAtVBlank ^ 0xffff))
	{
		gKeyRepeatTimer = -1;
		gKeysRepeat = keys;
	}
	else if (gKeyRepeatTimer > 0)
	{
		gKeyRepeatTimer--;
	}
	gKeysHeldAtVBlank = keys;
	gFrameTick++;
	gLagFrameCount++;
	gVBlankDone = 1;
	SoundDriver_EnterMixer();
}

s32 Sound_LoadPresetParameters(s32 preset)
{
	s32 index;

	index = preset;
	if ((u32)preset > SOUND_PRESET_COUNT - 1)
	{
		index = 0;
	}
	Dma_Set(
		IwramCodeRecords + index * SOUND_PRESET_SIZE, IwramSoundPresetCode, 0x84000026, REG_DMA3);
	return 0;
}

void Sound_ReservedNoOpA(void)
{
}

void Sound_ReservedNoOpB(void)
{
}

void Input_HandleKeyInterrupt(void)
{
	if (gSleepActive == 0)
	{
		Io_Write16(0xc3ff, &REG_KEYCNT);
		gResetRequested = 1;
	}
}

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

#define DEFINE_QUEUE_IO_WRITE(name, delay)          \
	void name(u32 address, u32 value)               \
	{                                               \
		IO_WRITE_QUEUE_PUSH(address, value, delay); \
	}

DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay1, 0x10000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay2, 0x20000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay3, 0x30000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay5, 0x50000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay6, 0x60000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay7, 0x70000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay9, 0x90000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay10, 0xa0000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay11, 0xb0000)

void IoWriteQueue_FlushPending(void)
{
	struct IoWriteQueue *queue;
	u32					 count;

	queue = &gIoWriteQueue;
	count = queue->count;
	if (count != 0)
	{
		QueueFlushRoutine routine;
		u32				  size = (u32)TransferCommandExecutor_Size;
		u32				  word_count = size >> 2;
		u32				  zero = 0;
		u32				  control = 0x84;

		control <<= 24;
		{
			u8 routine_bytes[word_count << 2];

			routine = (QueueFlushRoutine)routine_bytes;
			Dma_Set(Resource_ExecuteTransfer, (void *)routine, word_count | control, REG_DMA3);
			routine(queue, count);
			queue->count = zero;
		}
	}
}

void BlendTransition_Update(void)
{
	if (gBlendDuration != 0)
	{
		{
			volatile u16 *blend_control;
			u32			  control;

			if (gBlendBrighten != 0)
			{
				control = gBlendLayers | 0x80;
				blend_control = &REG_BLDCNT;
			}
			else
			{
				control = gBlendLayers | 0xc0;
				blend_control = &REG_BLDCNT;
			}
			*blend_control = control;
		}
		{
			u8 *remaining = &gBlendFramesLeft;
			s32 delta;
			s32 level;
			s32 step;

			(*remaining)--;
			level = gBlendTargetLevel;
			delta = gBlendStartLevel - gBlendTargetLevel;
			step = *remaining;
			level += __divsi3(delta * step, gBlendDuration);
			REG_BLDY = level;
			if (*remaining == 0)
				gBlendDuration = 0;
		}
	}
}

// フェードは VBlank ごとに BlendTransition_Update が進める。
// 明るさ（BLDY）は 0 で元のまま、16 で真っ白または真っ黒。
//   gBlendBrighten   : 0 黒へ / 1 白へ
//   gBlendLayers     : BLDCNT の対象レイヤー（下の 4 つは 0x3e で BG1〜3・OBJ・背景色）
//   gBlendFramesLeft : 残りフレーム数
//--------------------------------------------------------------------------
// BG1〜3・OBJ・背景色を、今の目標の明るさから level まで frames フレームで
// フェードする
// brighten : 0 黒へ / 1 白へ
//--------------------------------------------------------------------------
#define DEFINE_BLEND_FADE(name, brighten, level) \
	void name(s32 frames)                        \
	{                                            \
		gBlendBrighten = brighten;               \
		gBlendLayers = 0x3e;                     \
		gBlendStartLevel = gBlendTargetLevel;    \
		gBlendTargetLevel = level;               \
		gBlendDuration = frames;                 \
		gBlendFramesLeft = gBlendDuration;       \
	}

// 黒へフェードアウト
DEFINE_BLEND_FADE(Blend_SetDarkenTarget16, 0, 16)

// 黒からフェードイン
DEFINE_BLEND_FADE(Blend_SetDarkenTarget0, 0, 0)

// 白へフェードアウト
DEFINE_BLEND_FADE(Blend_SetBrightenTarget16, 1, 16)

// 白からフェードイン
DEFINE_BLEND_FADE(Blend_SetBrightenTarget0, 1, 0)

//--------------------------------------------------------------------------
// フェードを指定する
// bright : 0 黒へ / 1 白へ
// layer  : BLDCNT の対象レイヤー（下位 6 ビット）
// start  : はじめの明るさ（16 より大きければ今の目標の明るさから）
// target : 目標の明るさ
// frames : かけるフレーム数
//--------------------------------------------------------------------------
void Blend_ConfigureTransition(s8 bright, s32 layer, u32 start, s32 target, s32 frames)
{
	gBlendBrighten = bright;
	gBlendLayers = layer & 0x3f;
	if (start > 16U)
	{
		gBlendStartLevel = gBlendTargetLevel;
	}
	else
	{
		gBlendStartLevel = start;
	}
	gBlendTargetLevel = target;
	gBlendFramesLeft = (gBlendDuration = frames);
}

//--------------------------------------------------------------------------
// フェードが終わるまで待つ
//--------------------------------------------------------------------------
void Blend_WaitForTransition(void)
{
	if (gBlendDuration != 0)
	{
		do
		{
			WaitFrames(1);
		} while (gBlendDuration != 0);
	}
}

extern u8 IwramClearWords[];

s32 _call_via_r3(s32, s32, s32, s32);

void Graphics_ResetFrameState(void)
{
	(*(u8(*)[]) & gObjAffineCount)[0] = 0;
	_call_via_r3(((u32)&gRenderSlotHeads), 0x400, ((u32)&gObjAffineCount), (u32)IwramClearWords);
}

typedef void (*OamBuildRoutine)(void *work);

s32 AffineMatrix_BuildForEffect(struct AffineTransform *source)
{
	union AffineMatrix *matrix;
	s16				   *coefficient;
	s32					x_scale;
	s32					y_scale;
	s32					angle;
	u8					index;

	index = gObjAffineCount;
	x_scale = (s16)source->scale_x;
	y_scale = (s16)source->scale_y;
	angle = source->angle;
	if (index > 31)
		return 0;

	matrix = &gObjAffineMatrices[index];
	coefficient = matrix->coefficients;
	if ((x_scale == y_scale || -x_scale == y_scale) && angle == 0)
	{
		s32 reciprocal;
		s32 x_reciprocal;

		reciprocal = Iwram_SignedDivide(0x10000, y_scale);
		x_reciprocal = reciprocal;
		if (-x_scale == y_scale)
			x_reciprocal = -reciprocal;

		matrix->rows[0] = (u16)x_reciprocal;
		matrix->rows[1] = reciprocal << 16;
	}
	else
	{
		s32 sine;
		s32 cosine;

		sine = Trig_Sin(angle);
		cosine = Trig_Cos(angle);
		*coefficient = cosine / x_scale;
		coefficient++;
		*coefficient = sine / x_scale;
		coefficient++;
		*coefficient = (-sine) / y_scale;
		coefficient++;
		*coefficient = cosine / y_scale;
	}

	gObjAffineCount = index + 1;
	return index;
}

void Runtime_PushSlotEntry(void *slot_entry, s32 slot)
{
	void **head;
	void  *previous_head;

	if (slot > 0xFF)
		slot = 0xFF;
	if (slot < 0)
		slot = 0;
	head = &(*(void *(*)[]) & gRenderSlotHeads)[slot];
	previous_head = *head;
	*head = slot_entry;
	*(void **)slot_entry = previous_head;
}

void Render_BuildOamList(void *work)
{
	OamBuildRoutine routine;

	RamCode_Load(routine, Render_BuildOamListKernel, LoadedRuntime_Size);
	routine(work);
	MemFree((void *)routine);
}

//--------------------------------------------------------------------------
// rescache.c
// リソース：OBJ の VRAM をブロックに分けて貸し出すキャッシュ
//--------------------------------------------------------------------------
// OBJ のキャラクター（0x06010000 から）を 64 バイトのブロック 512 個に分け、
// ResourceBlockOwners にブロックごとの持ち主（キャッシュの番号）を置く。
// キャッシュは 96 個で、offset（バイト）が 0xffff ならあき。
// size が 16 以下のものは VRAM を持っていない

//--------------------------------------------------------------------------
// size バイト分の続いたあきブロックを前からさがし、id のものにする
// 戻り値 : VRAM の中の位置（バイト）、なければ -1
//--------------------------------------------------------------------------
s32 ResourceTable_AllocateBlocks(u32 id, u32 size)
{
	u32 blocks;
	s32 ret;
	s32 pos;
	u32 end;
	u32 i;

	blocks = size / VRAM_BLOCK_BYTES;
	if (id >= VRAM_CACHE_ENTRY_COUNT)
	{
		return -1;
	}
	pos = 0;
	for (;;)
	{
		ret = -1;
		if (pos >= VRAM_BLOCK_COUNT)
		{
			goto done;
		}
		if (ResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE)
		{
			goto occupied;
		}
		ret = pos;
		end = blocks + ret;
		while (pos < end)
		{
			if (ResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE)
			{
				goto occupied;
			}
			pos++;
		}
		for (i = 0; i < blocks; i++)
		{
			ResourceBlockOwners[ret + i] = id;
		}
		goto found;
	occupied:
		// 使っているブロックは、持ち主の大きさの分だけとばす
		pos += gVramBlockCache[ResourceBlockOwners[pos]].size / VRAM_BLOCK_BYTES;
	}
found:
	ret *= VRAM_BLOCK_BYTES;
done:
	return ret;
}

//--------------------------------------------------------------------------
// 続いたあきブロックのいちばん長い数を返す
//--------------------------------------------------------------------------
s32 ResourceTable_GetLongestFreeBlockRun(void)
{
	s32 run = 0;
	u8 *p = ResourceBlockOwners;
	s32 max = 0;
	s32 n = VRAM_BLOCK_COUNT;

	do
	{
		if (*p++ != VRAM_BLOCK_OWNER_FREE)
		{
			run = 0;
		}
		else
		{
			run++;
			if (max < run)
				max = run;
		}
		n--;
	} while (n != 0);
	return max;
}

//--------------------------------------------------------------------------
// id の持っているブロックをすべてあきにする
// 戻り値 : あけたブロックがあれば -1、なければ 0（id が範囲外なら -1）
//--------------------------------------------------------------------------
s32 Resource_ClearSlotReferences(s32 id)
{
	s32 cleared = 0;
	s32 n;
	u8 *p;
	u8	empty;

	if ((u32)id >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	p = ResourceBlockOwners;
	empty = VRAM_BLOCK_OWNER_FREE;
	n = VRAM_BLOCK_COUNT;
	do
	{
		if (*p == id)
		{
			*p = empty;
			cleared++;
		}
		n--;
		p++;
	} while (n != 0);
	if (cleared != 0)
		return -1;
	return 0;
}

//--------------------------------------------------------------------------
// キャッシュ no を空ける（ブロックを返し、あきのしるしを付ける）
//--------------------------------------------------------------------------
s32 Resource_ResetEntry(u32 no)
{
	struct VramBlockCacheEntry *entry = &gVramBlockCache[no];

	if (no >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	if (entry->offset != VRAM_CACHE_OFFSET_FREE)
	{
		Resource_ClearSlotReferences(no);
		entry->offset |= VRAM_CACHE_OFFSET_FREE;
		entry->size = 0;
	}
	return 0;
}

//--------------------------------------------------------------------------
// キャッシュ no のブロックだけを返し、size を 1 にして番号は持ったままにする
//--------------------------------------------------------------------------
s32 Resource_ActivateEntry(u32 no)
{
	struct VramBlockCacheEntry *entry = &gVramBlockCache[no];

	if (no >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	if (entry->size > 16)
	{
		s32 one;

		Resource_ClearSlotReferences(no);
		one = 1;
		entry->size = one;
	}
	return 0;
}

//--------------------------------------------------------------------------
// キャッシュ no に size バイトの VRAM を用意し、src を写す
// 同じ大きさの VRAM を持っていればそのまま使う
// src : NULL なら写さない、-1 なら 0 で埋める
// 戻り値 : OBJ のタイル番号（32 バイト単位）、用意できなければ 0
//--------------------------------------------------------------------------
s32 VramBlock_LoadCached(u32 no, u32 size, const void *src)
{
	struct VramBlockCacheEntry *entry;
	s32							ofs;
	void					   *dst;

	entry = &gVramBlockCache[no];
	if (no >= VRAM_CACHE_ENTRY_COUNT)
		return 0;
	if (size > 8192)
		return 0;
	if (entry->size > 16)
	{
		if (entry->size != size)
		{
			Resource_ResetEntry(no);
			ofs = ResourceTable_AllocateBlocks(no, size);
		}
		else
		{
			ofs = entry->offset;
		}
	}
	else
	{
		ofs = ResourceTable_AllocateBlocks(no, size);
	}

	if (ofs != -1)
	{
		dst = (void *)(0x06010000 + ofs);
		entry->size = size;
		entry->offset = ofs;
		if (src != 0)
		{
			if (src == (const void *)-1)
			{
				Iwram_ClearWords(dst, size);
			}
			else
			{
				Dma_Set(src, dst, (size >> 2) | (DMA_ENABLE32 | DMA_32BIT), REG_DMA3);
			}
		}
		return (u32)ofs >> 5;
	}
	return 0;
}

//--------------------------------------------------------------------------
// ブロックとキャッシュをすべてあきにする
//--------------------------------------------------------------------------
void Resource_InitializeTable(void)
{
	u32 limit = VRAM_BLOCK_COUNT - 1;
	u8 *p = ResourceBlockOwners;
	u32 n = 0;
	u32 empty = VRAM_BLOCK_OWNER_FREE;

	do
	{
		*p++ = empty;
		n++;
	} while (n <= limit);

	{
		struct VramBlockCacheEntry *entry = gVramBlockCache;

		n = 0;
		do
		{
			entry->offset |= VRAM_CACHE_OFFSET_FREE;
			entry->size = 0;
			entry++;
			n++;
		} while (n < VRAM_CACHE_ENTRY_COUNT);
	}
}

//--------------------------------------------------------------------------
// あいているキャッシュの番号を返す（なければ VRAM_CACHE_ENTRY_COUNT）
//--------------------------------------------------------------------------
s32 Resource_FindFreeEntry(void)
{
	s32							ret = VRAM_CACHE_ENTRY_COUNT;
	s32							no;
	struct VramBlockCacheEntry *entry = gVramBlockCache;

	for (no = 0; no < VRAM_CACHE_ENTRY_COUNT; no++, entry++)
	{
		if (entry->offset == VRAM_CACHE_OFFSET_FREE)
		{
			ret = no;
			break;
		}
	}
	return ret;
}

//--------------------------------------------------------------------------
// あいているキャッシュに size バイトの VRAM を用意して、その番号を返す
//--------------------------------------------------------------------------
s32 Resource_LoadIntoFreeSlot(s32 size)
{
	s32 no;

	no = Resource_FindFreeEntry();
	VramBlock_LoadCached(no, size, 0);
	return no;
}

//--------------------------------------------------------------------------
// キャッシュ no に、今の大きさのまま src を写しなおす
// 戻り値 : OBJ のタイル番号
//--------------------------------------------------------------------------
s32 Resource_GetBuffer(s32 no, s32 src)
{
	return VramBlock_LoadCached(no, gVramBlockCache[no].size, (const void *)src);
}

typedef s32 (*KeyCallbackFn)(void);

void Scheduler_ResetTaskTable(void)
{
	struct SchedulerTask *task = gSchedulerTaskTable;
	s32					  remaining;

	gSchedulerTaskCount = 0;
	gSchedulerStatus = 0;
	remaining = SCHEDULER_TBS_TASK_COUNT - 1;
	do
	{
		task->callback = 0;
		task->state = 0xffff;
		task->mask = 0;
		task++;
		remaining--;
	} while (remaining >= 0);
	gSchedulerTaskCount = 1;
}

void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byte_count)
{
	u32 index;
	byte_count >>= 2;
	for (index = 0; index < byte_count; index++)
		*destination++ = *source++;
}

void Scheduler_SortTasks(void)
{
	/* FAKEMATCH: the leading test and reused swap cursor retain 84 bytes; ordinary nested for loops produce 80. */
	struct SchedulerTask  saved;
	struct SchedulerTask *base = gSchedulerTaskTable;
	struct SchedulerTask *task;
	s32					  pass = SCHEDULER_TBS_TASK_COUNT - 1;
	s32					  remaining;
	goto sort_pass;
next_pass:
	base = gSchedulerTaskTable;
sort_pass:
	task = base;
	if (pass <= 0)
		goto finish_pass;
	remaining = pass;
next_task:
	if ((s16)task[1].state > (s16)task->state)
	{
		memcpy(&saved, task, sizeof(saved));
		base = task;
		task++;
		memcpy(base, task, sizeof(saved));
		memcpy(task, &saved, sizeof(saved));
	}
	else
	{
		task++;
	}
	if (--remaining != 0)
		goto next_task;
finish_pass:
	if (--pass > 1)
		goto next_pass;
}

s32 Scheduler_FindCallback(SchedulerCallback callback)
{
	s32					  result;
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (task->callback == (u32)callback)
		{
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_AddOrUpdateCallback(SchedulerCallback callback, s32 order)
{
	u32					  saved_interrupt_state;
	s32					  index;
	struct SchedulerTask *task;
	s32					  i;

	index = -1;
	task = gSchedulerTaskTable;
	(void)gSchedulerStatus;
	IRQ_LOCK(saved_interrupt_state);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (task->callback == (u32)callback)
		{
			task->state = order;
			index = i;
			break;
		}
	}
	task = gSchedulerTaskTable;
	if (index == -1)
	{
		for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
		{
			if (task->callback == 0)
			{
				task->callback = (u32)callback;
				task->state = order;
				task->mask = 0;
				index = i;
				break;
			}
		}
	}
	Scheduler_SortTasks();
	IRQ_UNLOCK(saved_interrupt_state);
	return index;
}

void Scheduler_Idle(void)
{
}

void Scheduler_EmptyCallback(void)
{
}

s32 Scheduler_RemoveCallback(SchedulerCallback callback)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (task->callback == (u32)callback)
		{
			task->callback = 0;
			task->state = 0x7fff;
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_EnableCallbacks(SchedulerCallback callback)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (callback != 0)
		{
			if (task->callback != (u32)callback)
				continue;
		}
		TASK_STATE_HIGH(task) |= 1;
		result = i;
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_EnableUnmaskedOverlayCallbacks(void)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if ((task->callback >> 24) == 2 && (task->mask & 1) == 0)
		{
			TASK_STATE_HIGH(task) |= 1;
			result = i;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_SetCallbackMask(SchedulerCallback callback, u32 mask)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (task->callback == (u32)callback)
		{
			task->mask = mask;
			result = i;
			break;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_DisableCallbacks(SchedulerCallback callback)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if (callback == 0 || task->callback == (u32)callback)
		{
			TASK_STATE_HIGH(task) &= ~1;
			result = i;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

s32 Scheduler_DisableOverlayCallbacks(void)
{
	struct SchedulerTask *task;
	u32					  saved_interrupt_master;
	s32					  result;
	s32					  i;

	result = -1;
	task = gSchedulerTaskTable;
	IRQ_LOCK(saved_interrupt_master);
	for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++)
	{
		if ((task->callback >> 24) == 2)
		{
			TASK_STATE_HIGH(task) &= ~1;
			result = i;
		}
	}
	IRQ_UNLOCK(saved_interrupt_master);
	return result;
}

void Runtime_InvokeCallbacksByKey(s32 key)
{
	struct SchedulerTask *task = gSchedulerTaskTable;
	s32					  remaining;

	key >>= 8;
	if (gSchedulerTaskCount == 1)
	{
		remaining = SCHEDULER_TBS_TASK_COUNT + 1;
		task--;
	next_task:
		remaining--;
		if (remaining != 0)
		{
			task++;
			if (TASK_STATE_HIGH(task) == key)
			{
				KeyCallbackFn callback = *(KeyCallbackFn *)&task->callback;

				callback();
			}
			goto next_task;
		}
	}
}

u32 Random16(void)
{
	u32 value = gRandomSeed * 0x41c64e6d + 0x3039;

	gRandomSeed = value;
	return (value << 8) >> 16;
}

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position)
{
	*position++ += Iwram_MulQ16(radius, Trig_Sin(angle + 0x4000));
	position++;
	*position += Iwram_MulQ16(radius, Trig_Sin(angle));
}

s32 ArcTan2(s32 x, s32 y)
{
	const u16 *table;
	s32		   value;
	s32		   ratio;
	s32		   result;

	if (x == 0)
	{
		result = 0;
	}
	else if (y == 0)
	{
		result = 0x4000;
	}
	else
	{
		ratio = y;
		if (ratio < 0)
			ratio = -ratio;
		value = x;
		if (value < 0)
			value = -value;

		ratio = (value << 8) / ratio;
		result = 0x4000;
		if (ratio <= 0xFB6A)
		{
			table = &Math_TanTable[127];
			result = 0;

			value = *table;
			table -= 0x40;
			if (ratio > value)
			{
				result = 0x2000;
				table += 0x80;
			}
			value = *table;
			table -= 0x20;
			if (ratio > value)
			{
				result |= 0x1000;
				table += 0x40;
			}
			value = *table;
			table -= 0x10;
			if (ratio > value)
			{
				result |= 0x800;
				table += 0x20;
			}
			value = *table;
			table -= 8;
			if (ratio > value)
			{
				result |= 0x400;
				table += 0x10;
			}
			value = *table;
			table -= 4;
			if (ratio > value)
			{
				result |= 0x200;
				table += 8;
			}
			value = *table;
			table -= 2;
			if (ratio > value)
			{
				result |= 0x100;
				table += 4;
			}
			value = *table;
			table--;
			if (ratio > value)
			{
				result |= 0x80;
				table += 2;
			}
			value = *table;
			if (ratio > value)
				result |= 0x40;
		}
	}

	if (y < 0)
		result = 0x8000 - result;
	if (x < 0)
		result = -result;

	return (u16)result;
}

s32 Math_IntegerSqrt(s32 value)
{
	s32 trial;
	s32 remainder;
	s32 bit;
	s32 result;

	remainder = value;
	result = 0;
	bit = 0xf;
	do
	{
		trial = (result << (bit + 1)) + (1 << (bit * 2));
		if (trial <= remainder)
		{
			result |= 1 << bit;
			remainder -= trial;
		}
		bit -= 1;
	} while (bit >= 0);
	return result;
}

s32 FixedSqrt(s32 value)
{
	return Iwram_Sqrt(value) << 8;
}

s32 Runtime_GetLowTableAddress(void)
{
	return (s32)System_BasicColorPalette;
}

void Text_FormatHexToWork(u32 value)
{
	u8		 *buffer = gNumberTextBuffer;
	const u8 *digits = Text_HexDigits;
	s32		  index = 7;

	do
	{
		buffer[index] = digits[value & 0xF];
		value >>= 4;
		index--;
	} while (index >= 0);

	{
		u8 *terminator = gNumberTextBuffer;
		terminator[8] = 0;
	}
}

void Text_FormatSignedDecimalToWork(s32 arg0)
{
	u32 *tbl;
	s8	*out;
	s32	 count;
	u32	 word;
	s32	 result;
	s32	 val;
	s8	 sign;

	result = arg0;
	val = result;
	tbl = (u32 *)Text_PowersOfTen;
	sign = 0x20;
	out = gNumberTextBuffer;
	if (val < 0)
	{
		val = -val;
		sign = 0x2D;
	}
	word = *tbl++;
	count = 9;
	if (val < word)
	{
		do
		{
			count -= 1;
			*out++ = 0x20;
			if (count == 0)
				break;
			word = *tbl++;
		} while (val < word);
	}
	*out++ = sign;
	tbl -= 1;
	if (count != 0)
	{
		do
		{
			word = *tbl++;
			result = val / word;
			*out++ = result + 0x30;
			val -= result * word;
			count -= 1;
		} while (count != 0);
	}
	out[0] = val + 0x30;
	out[1] = 0;
}

void Graphics_FillWordStreamWithF000(u32 count)
{
	u16 *dst = gDebugTextCursor;
	u32	 index;

	for (index = 0; index < count; index++)
		*dst++ = 0xf000;
	gDebugTextCursor = dst;
}

#ifdef MODERN
// Camelot's compiler copied the task inline; a modern one calls memcpy,
// and the game links no C library
void *memcpy(void *destination, const void *source, unsigned long size)
{
	u8		 *to = destination;
	const u8 *from = source;

	while (size-- != 0)
		*to++ = *from++;
	return destination;
}
#endif

void Runtime_WriteDebugTextTiles(const u8 *src)
{
	if (gDebugTextEnabled != 0)
	{
		u32	 addr = ((u32)&gDebugTextCursor);
		u32	 c = *src;
		u16 *dst = *(u16 **)addr;
		u32	 cnt = 0;
		src++;

		if (c != 0)
		{
			u32 mask = 0xf000;
			addr = (u32)(BG_SCREEN_BLOCK(4) + 0x500);
			do
			{
				*dst++ = c | mask;
				if (dst == (u16 *)addr)
					dst = (u16 *)BG_SCREEN_BLOCK(4);
				cnt++;
				if (cnt > 31)
					break;
				c = *src++;
			} while (c != 0);
			addr = ((u32)&gDebugTextCursor);
		}
		*(u16 **)addr = dst;
	}
}
//--------------------------------------------------------------------------
// debug_number.c
// デバッグ：数を右そろえでデバッグ文字にする
//--------------------------------------------------------------------------
// どちらも数を作業域に右づめで作り、その終わりから keta 文字を書く
//--------------------------------------------------------------------------
// 16 進で keta けた（1〜8、範囲外なら 8）
//--------------------------------------------------------------------------
void Text_DrawHexRightAligned(u32 value, s32 keta)
{
	s32 n;

	n = keta;
	if ((n - 1) > 7U)
	{
		n = 8;
	}
	Text_FormatHexToWork(value);
	Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextHexEnd) - n));
}

//--------------------------------------------------------------------------
// 符号つきの 10 進で keta けた（1〜10、範囲外なら 10）
//--------------------------------------------------------------------------
void Text_DrawSignedDecimalRightAligned(s32 value, s32 keta)
{
	s32 n;

	n = keta;
	if ((n - 1) > 9U)
	{
		n = 10;
	}
	Text_FormatSignedDecimalToWork(value);
	Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextDecimalEnd) - n));
}

void Bg0_ClearTilemap(void)
{
	volatile u32 fill;

	fill = 0xf000f000;
	Dma_Set((const void *)&fill, BG_SCREEN_BLOCK(4), 0x85000140, REG_DMA3);
	gDebugTextCursor = (u16 *)BG_SCREEN_BLOCK(4);
	Io_Write16(0x400, &REG_BG0CNT);
}

void Ui_LoadWindowGraphics(void)
{
	Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles), BG_CHAR_BLOCK(0), 0x84000800,
		REG_DMA3);
	Dma_Set(Ui_WindowPalette, (void *)&BG_PLTT_COLOR(15, 0), 0x80000010, REG_DMA3);
	Io_Write16(0, &PLTT_BACKDROP);
	Io_Write16(0x4180, &BG_PLTT_COLOR(15, 4));
	Io_Write16(0x3960, &BG_PLTT_COLOR(15, 5));
	Io_Write16(0x3140, &BG_PLTT_COLOR(15, 6));
	Io_Write16(0x2920, &BG_PLTT_COLOR(15, 7));
	Io_Write16(0x49a0, &BG_PLTT_COLOR(15, 8));
	Io_Write16(0x51c0, &BG_PLTT_COLOR(15, 9));
	Io_Write16(0x59e0, &BG_PLTT_COLOR(15, 10));
	Dma_Set(System_BasicColorPalette, (void *)OBJ_PLTT, 0x800000e0, REG_DMA3);
}

void PaletteDma_LoadBlock(void)
{
	Dma_Set(System_BasicColorPalette, (void *)OBJ_PLTT, 0x800000e0, REG_DMA3);
}
