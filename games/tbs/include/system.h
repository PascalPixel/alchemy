#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

/* @file */
/* The system library's one header: its types, its functions and its macros. */
/* A file that uses the system library includes this and nothing else of it, */
/* as a Saturn program includes SGL's sgl.h. The name is the staff roll's: */
/* under Programming, its first heading is `System`. */
/* */
/* As SGL's sgl.h takes its types from sl_def.h, this takes the system */
/* library's types and constants from system/definitions.h and its variables */
/* from system/globals.h, and keeps the functions: the stub table's */
/* prototypes, then each group's inlines and macros in turn. */

#include "types.h"
#include "fixed_math.h"
#include "edition.h"
#include "system/asm.h"
#include "system/definitions.h"
#include "system/globals.h"

/* The heap_release group. Releasing heap blocks, and the two pools' space. */
/* Takes `size` bytes from another pool through its allocator, such as */
/* syHeapAllocEwram. */
static __inline__ void *syHeapCallAlternatePool(void *(*allocate)(s32),
												s32 size) {
	return allocate(size);
}

struct AFFINE_TRANSFORM;

/* The system library's functions in the order of its stub table */
/* (src/system/stubs.s). */
void sySynch(s32 frames);
void syInitEvent(void);
s32 sySetEvent(TASK_FUNC func, s32 priority);
s32 syCloseEvent(TASK_FUNC func);
s32 syTaskSuspend(TASK_FUNC func);
s32 syTaskResume(TASK_FUNC func);
void syExecuteEvent(s32 pass);
u32 syRandom(void);
ANGLE syAtan(FIXED x, FIXED y);
s32 syMathIntegerSqrt(s32 value);
FIXED sySquartFX(FIXED value);
/* The sine of `angle`, a whole turn being TRIG_TURN, in 16.16, read from */
/* Trig_SineTable, 256 entries for the first quarter turn. */
FIXED sySin(ANGLE angle);
/* The cosine of `angle`, a whole turn being TRIG_TURN, in 16.16; it falls */
/* through into sySin a quarter turn on. */
FIXED syCos(ANGLE angle);
void syVectorAddPolarOffset(FIXED radius, ANGLE angle, POINT position);
void syIntFunction(u32 irq, s32 vcount, INTERRUPT_HANDLER handler);
void syHeapInit(void);
void *syHeapSlotAlloc(s32 slot, s32 size);
void *syHeapSlotAllocEwram(s32 slot, s32 size);
/* Releases heap slot `slot`'s block, and with it every later block of the same
 */
/* pool, by setting the pool's next free address back to the block's start. */
void syHeapSlotRelease(s32 slot);
s32 syHeapGetRemainingIwram(void);
s32 syHeapGetRemainingEwram(void);
void *syHeapAlloc(s32 size);
void *syHeapAllocEwram(s32 size);
/* Releases `buffer`, a block taken with no heap slot, and every later block of
 */
/* its pool, as syHeapSlotRelease does. */
void syHeapRelease(void *buffer);
void syPrint(const u8 *src);
void syPrintHex(u32 value, s32 digits);
void syDebugTextPrintDecimal(s32 value, s32 digits);
void syDebugTextPrintBlanks(u32 count);
s32 fdDecompressByteLz(const void *source, void *destination);
s32 fdDecompressTaggedLz(const void *source, void *destination);
void syVramCacheReset(void);
s32 syVramCacheFreeEntry(u32 id);
s32 syVramCacheReleaseVram(u32 id);
s32 syVramCacheLoad(u32 id, u32 size, const void *src);
s32 syVramCacheFindFreeEntry(void);
void syDebugTextClear(void);
s32 syOamMatrixAllocFromTransform(AFFINE_TRANSFORM *source);
void syOamAddSprAtDepth(void *slotEntry, s32 slot);
void syWindowLoadGraphics(void);
void syPaletteLoadBasicSprCols(void);
void syInitMatrix(void);
void syMatrixResetUnit(void);
void syPushMatrix(void);
void syPopMatrix(void);
void syMatrixRotate(s32 *angles);
void syRotX(ANGLE angle);
void syRotY(ANGLE angle);
void syRotZ(ANGLE angle);
void syMatrixTranslateVector(const POINT position);
void syMatrixScaleVector(const VECTOR scale);
void syMatrixRotateTranslate(s32 *angles, POINT position);
void syMatrixRotateTranslateScale(s32 *angles, POINT position, VECTOR scale);
void syMatrixSetLookAt(POINT eye, POINT target);
s32 syConvert3Dto2D(const POINT point, s32 *screen);
void syPerspective(u32 fieldOfView, FIXED nearDepth, FIXED farDepth);
void sySetScreenDist(u32 focal, u32 nearDepth, u32 farDepth);
void syCopyBuildStamp(void);
s32 syDummyReturnZero(s32 resourceId, const u8 *table);
void *syResourceGetTableEntry(s32 index);
void syDummyNoOpHook(s32 unused);
void syOverlayLoad(u32 index, void *destination);
void syScrFadeToBlack(s32 vblanks);
void syScrFadeFromBlack(s32 vblanks);
void syScrFadeToWhite(s32 vblanks);
void syScrFadeFromWhite(s32 vblanks);
void syScrFadeStart(s8 bright, s32 layer, u32 start, s32 target, s32 vblanks);
void syScrFadeWait(void);
void syDummyReservedNoOpA(void);
void syDummyReservedNoOpB(void);
void syDummyReservedNoOpC(void);
void syDummyReservedNoOpD(void);
void syDummyReservedNoOpE(void);
void sySerialInit(void);
s32 sySerialPollAndTransfer(void *send, void *receive);
/* Queue one register write of a width (8, 16 or 32 bits): a store, or the */
/* value's bits set or cleared (system/system_io_write_queue.c). */
void syIoWriteQueueStore8(u32 address, u32 value);
void syIoWriteQueueStore16(u32 address, u32 value);
void syIoWriteQueueStore32(u32 address, u32 value);
void syIoWriteQueueSetBits8(u32 address, u32 value);
void syIoWriteQueueSetBits16(u32 address, u32 value);
void syIoWriteQueueSetBits32(u32 address, u32 value);
void syIoWriteQueueClearBits8(u32 address, u32 value);
void syIoWriteQueueClearBits16(u32 address, u32 value);
void syIoWriteQueueClearBits32(u32 address, u32 value);
void syInputResetKeyRepeat(void);
void syLookAt(POINT eye, POINT target);
u32 sySerialWaitForStatMask(s32 mask);
void sySerialRemoveIrqHandlers(void);
s32 sySndLoadReverbCode(s32 code);
s32 sySerialStartSend(void *source, s32 size);
s32 sySerialStartReceive(void *destination);
void sySerialWaitForSend(void);
void sySerialWaitForReceive(void);
void sySerialWaitForTransfers(void);
s32 sySerialGetActiveTransfers(void);
void sySerialStepBlockTransfer(void);
s32 syTaskSetFlags(TASK_FUNC func, u32 mask);

/* The system library's functions and globals that the stub table does not */
/* list. */

/* The system library's own functions and globals: interrupts, BIOS sound */
/* bias, random numbers and sleep. */
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
/* 1 while the CPU is stopped for sleep, until L and R wake it. */
extern u16 SleepActive;

/* NumberBuffer + 10, the decimal form's units digit (the form ends at */
/* + 11); syDebugTextPrintDecimal starts n characters before it, so it prints */
/* n + 1. */
extern u8 NumberBufferDecimalEnd[];
/* The end of the hexadecimal form in NumberBuffer. */
extern u8 NumberBufferHexEnd[];
/* The link battle timer's heading: `TIME`; `TMPO.` in Spanish, `TEMPS` in */
/* French. */
extern const u8 Link_TimeLabelString[];
/* Each resource's ROM address by resource id (syResourceGetTableEntry). */
extern void *syResourceDirectoryTable[];
/* The size of syResourcePatchThumbBranch (decompress_tagged_lz.s). */
extern u8 syResourcePatchThumbBranchCodeSize[];
/* The window font's 16 colours, loaded as BG palette 15 */
/* (syWindowLoadGraphics). */
extern const u16 Window_Palette[];

struct SPR_ANIMATION;
struct SPR;

/* The 152 bytes of IWRAM-resident sound code (0x03000bd8, not the mixer at */
/* 0x03007000) that sySndLoadReverbCode overwrites with a reverb code */
/* from IwramSoundReverbCodes. */
extern u8 IwramSoundReverbCode[];
/* HEAP_SLOT_SPRITE_ANIMATIONS: the sprite animations (struct */
/* SPR_ANIMATION) that fdSprAnimationAlloc takes from. */
#define SpriteAnimations                                                       \
	((struct SPR_ANIMATION **)&HeapSlots[HEAP_SLOT_SPRITE_ANIMATIONS])
/* Where the debug text writes next on its 32 by 20 screen in BG0. */
extern u16 *DebugTextCursor;
/* Nonzero lets syPrint write; the power-on set-up and the credits */
/* clear it. */
extern u8 DebugTextEnabled;
/* The buffer a save file is built in and written from, in two halves; its */
/* summary's 16-byte header lies just before it. */
extern u8 SaveBuffer[];
/* The current save file, or -1 for none (a new game). */
extern s16 SaveSlot;
/* The five reverb codes' mixer code, SOUND_REVERB_CODE_SIZE bytes each. */
extern u8 IwramSoundReverbCodes[];
/* The size of syOamBuildBufferKernel (decompress_byte_lz.s). */
extern u8 syOamBuildBufferKernelCodeSize[];
/* The ARM OAM list builder, run from a heap copy by syOamBuildBuffer. */
extern const u8 syOamBuildBufferKernel[];
/* The ARM register-write executor, run from a stack copy by */
/* syIoWriteQueueFlushPending. */
extern const u8 syResourceExecuteTransfer[];
/* The ARM function that resolves a loaded overlay's long branches, run from a
 */
/* heap copy by syOverlayLoad. */
extern const u8 syResourcePatchThumbBranch[];
/* The game code's last letter in the cartridge header: `J` in Japanese, `E`, */
/* `D`, `F`, `S` or `I` (syGetLanguage). */
extern const u8 Rom_LanguageCode;
/* The basic sprite palettes 0 to 13, 224 colours */
/* (syPaletteLoadBasicSprCols). */
extern const u8 BasicSpritePalettes[];
/* The sprite tile uploader, run from HEAP_SLOT_PICTURE_UPLOADER. */
extern u8 fdTileCopyStrided[];
/* The size of fdTileCopyStrided (map_grid_convert.s). */
extern u8 fdTileCopyStridedCodeSize[];
/* The size of syResourceExecuteTransfer (decompress_tagged_lz.s). */
extern u8 syResourceExecuteTransferCodeSize[];
/* Stops the console for good with the BIOS Stop call: in the German, French, */
/* Spanish and Italian editions, what pulling the Game Pak out does. */
void syHalt(void);
/* Runs the sound for this VBlank, the music players and then the game's own */
/* mixer in IWRAM; the last thing syVBlankHandler does. */
void Mixer_Enter(void);

void syInit(void);
void syIdleForever(void);
/* The cartridge's language, from its header (syGetLanguage). */
typedef enum CARTRIDGE_LANGUAGE {
	CARTRIDGE_LANGUAGE_ENGLISH,
	CARTRIDGE_LANGUAGE_GERMAN,
	CARTRIDGE_LANGUAGE_FRENCH,
	CARTRIDGE_LANGUAGE_SPANISH,
	CARTRIDGE_LANGUAGE_ITALIAN
} CARTRIDGE_LANGUAGE;
s32 syGetLanguage(void);
void syCodeResolveLongBranches(u16 *entries, s32 byteLength);
void syIrqIgnore(void);
void syInstallIwramAndIrqs(void);
void syInputUpdateKeyRepeatAndDirection(void);
void syVBlankHandler(void);
void sySndReservedNoOpA(void);
void sySndReservedNoOpB(void);
void syInputHandleKeyInterrupt(void);
s32 fdDecompressMoveToFrontLz(const void *source, void *destination);

void syIoWriteQueueFlushPending(void);

void syScrFadeUpdate(void);

/* Depth slots of syOamAddSprAtDepth: a higher slot is drawn in front. The */
/* windows' sprites, icons and cursors take the slots from OAM_DEPTH_WINDOW up,
 */
/* in front of the field's and the battle's, and OAM_DEPTH_FRONT is in front of
 */
/* everything. */
#define OAM_DEPTH_WINDOW 240
#define OAM_DEPTH_FRONT	 255

void syOamResetFrame(void);
void syOamBuildBuffer(void *work);

s32 syVramCacheAllocateBlocks(u32 id, u32 size);
s32 syVramCacheGetLongestFreeRun(void);
s32 syVramCacheFreeBlocks(s32 id);
s32 syVramCacheAllocEntry(s32 size);
s32 syVramCacheReload(s32 id, s32 src);

void syTaskCopyWords(u32 *destination, u32 *source, u32 byteCount);
void syTaskSort(void);
s32 syTaskFind(TASK_FUNC func);
void syTaskIdle(void);
void syTaskEmpty(void);
s32 syTaskSuspendOverlayTasks(void);
s32 syTaskResumeOverlayTasks(void);

/* A random whole number from 0 to count - 1, from syRandom. */
#define syRandom16Below(count) ((syRandom() * (count)) >> 16)

const u8 *syPaletteGetBasicSprCols(void);
void syStringFormatHex(u32 value);
void syStringFormatDecimal(s32 number);
void *memcpy(void *destination, const void *source, unsigned long size);

void syHeapReservedNoOp(void);

void syGetMatrix(MATRIX dst);
void syLoadMatrix(const MATRIX src);
void syMatrixBuildLookAt(POINT eye, POINT target, MATRIX out);
/* The far depth well past everything drawn, the largest whole 16.16 number: */
/* the battle's, Thor's and Lucky Dice's projections. */
#define PROJECTION_FAR_DEPTH_MAX toFIXED(32767)

void syDecompressHalfwords(const void *source, void *destination);
void syCanvasCopyAndBrighten(u8 *buffer, u32 amount, u8 *destination,
							 u32 bytes);
void syCanvasCopyAndDarken(u8 *buffer, u32 amount, u8 *destination, u32 bytes);
void syCanvasCopyAndScaleThreeQuarters(u8 *buffer, u8 *destination, u32 bytes);
void syCanvasCopyAndHalve(u8 *buffer, u8 *destination, u32 bytes);
void syCanvasCopyAndBrightenPartial(u8 *buffer, u32 amount, u8 *destination,
									u32 bytes);
void syCanvasCopyAndDarkenPartial(u8 *buffer, u32 amount, u8 *destination,
								  u32 bytes);
void syCanvasCopyAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *destination,
											  u32 bytes);
void syCanvasCopyAndHalveNonzero(u8 *buffer, u8 *destination, u32 bytes);
s32 sySaveInitState(void);
u32 sySaveSelectWriteSector(s32 partId);
u32 sySaveWriteSector(s32 sector);
s32 sySaveReadSector(s32 index);
u16 sySaveEraseSector(u16 sector);
s32 sySaveWritePart(s32 partId, void *source);
u32 sySaveReadPart(s32 partId, void *destination);
u32 sySaveDeletePart(s32 partId);
s32 sySaveChecksumSector(void);
u32 sySaveFindLatestSector(s32 partId);
s32 sySaveInvalidateSector(s32 index);
s32 sySaveCompareBytes(u8 *left, u8 *right, s32 count);
u32 sySaveGetLatestCounter(s32 partId);
s32 sySaveLoadFileSummaries(void);
u32 sySaveReleaseState(void);

void sySerialEnableTransferTimer(void);
void sySerialDisableTransfer(void);
s32 sySerialPollStat(void);
s32 sySerialExchangePayloads(void *send, void *receive);
void sySerialPrepareSendPacket(void *payload);
u8 sySerialCollectReceivedPayloads(void *payload);
void sySerialHandleTransferInterrupt(void);
void syLinkBattleResetTransferState(void);
void sySerialTestDelay(void);
void sySerialTestRun(void);

/* syRandom's seed; Lucky Wheels seeds it with the play time, and a link */
/* battle's parent sends it as the turn's BattleRandomSeed. */
extern u32 RandomSeed;

/* The system library's groups. */

/* The dma group. DMA channel registers, and the DMA and CPU fills. */
/* Fills `size` bytes at `destination` with the halfword `value` by DMA3, from
 */
/* a volatile copy of it on the stack. */
#define btDmaFill16(value, destination, size)                                  \
	{                                                                          \
		volatile u16 fill_ = (value);                                          \
		syDMAXCopy((const void *)&fill_, (destination),                        \
				   DMA_FILL_HALFWORDS((size) / 2), REG_DMA3);                  \
	}

/* Fills `size` bytes at `destination` with the word `value` by DMA3. */
#define btDmaFill32(value, destination, size)                                  \
	{                                                                          \
		volatile u32 fill_ = (value);                                          \
		syDMAXCopy((const void *)&fill_, (destination),                        \
				   DMA_FILL_WORDS((size) / 4), REG_DMA3);                      \
	}

/* btDmaFill32 through the caller's own volatile word `cell`, so a function */
/* that fills several blocks keeps one word for all of them and forms its */
/* address once. */
#define btDmaFill32Cell(cell, value, destination, size)                        \
	{                                                                          \
		(cell) = (value);                                                      \
		syDMAXCopy((const void *)&(cell), (destination),                       \
				   DMA_FILL_WORDS((size) / 4), REG_DMA3);                      \
	}

/* pret's CpuFill32: repeats one word through the BIOS CpuSet service. */
#define syCpuFill32(value, destination, size)                                  \
	{                                                                          \
		volatile u32 fill_ = (value);                                          \
		Bios_CpuSet((const void *)&fill_, (destination),                       \
					CPU_SET_32BIT | CPU_SET_SRC_FIXED | ((size) / 4));         \
	}

/* Stops DMA 0, which a scene may have left repeating each HBlank for a line */
/* effect: drops its start timing and repeat, then its enable, and reads the */
/* control back. */
#define syDma0Stop()                                                           \
	{                                                                          \
		volatile u16 *dma0_ = REG_DMA0;                                        \
                                                                               \
		dma0_[DMA_CONTROL_HALFWORD] &= ~DMA_START_TIMING_DRQ_REPEAT;           \
		dma0_[DMA_CONTROL_HALFWORD] &= ~DMA_ENABLE;                            \
		dma0_[DMA_CONTROL_HALFWORD];                                           \
	}

/* Starts a DMA transfer: stores the source, destination and control words */
/* into `channel`'s registers with one stmia. */
static __inline__ void syDMAXCopy(const volatile void *source,
								  volatile void *destination, u32 control,
								  volatile u32 *channel) {
	ASM_REGISTER(volatile u32 *dma, "r3") = channel;
	ASM_REGISTER(u32 src, "r0") = (u32)source;
	ASM_REGISTER(u32 dst, "r1") = (u32)destination;
	ASM_REGISTER(u32 cnt, "r2") = control;
	ASM_DMA_STORE(src, cnt, dma, dst);
}

/* Waits until `channel`'s transfer is done: SGL's syDMAWait. */
static __inline__ void syDMAWait(DMA_REGISTERS volatile *channel) {
	while((channel->ctrl & DMA_BUSY) != 0) {
	}
}

/* The io_write group. scIoWrite8, scIoWrite16 and scIoWrite32, writes to a */
/* register or memory. */
/* Writes a value to a 16-bit register or memory location. */
#define scIoWrite16(v, reg)                                                    \
	do {                                                                       \
		u32 value_ = (v);                                                      \
		*(reg) = value_;                                                       \
	} while(0)

/* Writes a value to a 32-bit register or memory location, in scIoWrite16's */
/* shape. */
#define scIoWrite32(v, reg) scIoWrite16(v, reg)

/* Writes a value to an 8-bit register or memory location, in scIoWrite16's */
/* shape. */
#define scIoWrite8(v, reg) scIoWrite16(v, reg)

/* Reads a 16-bit register or memory location into a variable. The variable */
/* is reached through its address, so a halfword one lives in the frame and */
/* is read back from there after each call. The Lost Age has the same macro. */
#define scIoRead16(reg, variable) (*(variable) = *(reg))

/* The irq_lock group. Saving, disabling and restoring the interrupt master */
/* enable. */
/* */
/* The three steps take ime, a pointer to REG_IME. Disabling writes the */
/* pointer's low halfword (0x0208, bit 0 clear) back through it. */

/* Saves the original IME value in saved. */
#define IME_SAVE(ime, saved)                                                   \
	do {                                                                       \
		(saved) = *(ime);                                                      \
	} while(0)

/* Disables interrupts. */
#define IME_DISABLE(ime)                                                       \
	do {                                                                       \
		*(ime) = (u16)(u32)(ime);                                              \
	} while(0)

/* Restores IME from saved. */
#define IME_RESTORE(ime, saved)                                                \
	do {                                                                       \
		*(ime) = (saved);                                                      \
	} while(0)

/* Disables interrupts and saves IME in saved; IRQ_UNLOCK restores it. */
#define IRQ_LOCK(saved)                                                        \
	do {                                                                       \
		volatile u16 *ime_;                                                    \
                                                                               \
		IME_SAVE((ime_ = &REG_IME), saved);                                    \
		IME_DISABLE(ime_);                                                     \
	} while(0)

/* Restores the IME value saved by IRQ_LOCK. */
#define IRQ_UNLOCK(saved)                                                      \
	do {                                                                       \
		REG_IME = (saved);                                                     \
	} while(0)

/* Disables interrupts, keeping IME in a variable of the caller's that an */
/* interrupt may read. */
static inline void syIrqLock(volatile u32 *saved) {
	u16 *ime;

	IME_SAVE((ime = (u16 *)&REG_IME), *saved);
	IME_DISABLE(ime);
}

/* Restores the IME that syIrqLock kept. */
static inline void syIrqUnlock(volatile u32 *saved) {
	u32 value = *saved;

	REG_IME = value;
}

/* The io_write_queue group. The register write queue that VBlank carries out */
/* after each finished frame. */
/* Disables interrupts to add one register write or DMA to the queue; a write */
/* to a full queue is dropped. */
/* @param control a width (IO_WRITE_BYTE to IO_WRITE_WORD), with IO_WRITE_SET */
/* or IO_WRITE_CLEAR to change bits; or a DMA control word. */
#define IO_WRITE_QUEUE_PUSH(address, value, control)                           \
	{                                                                          \
		IO_WRITE_QUEUE *q = &IoWriteQueueWork;                                 \
		u32 saved;                                                             \
		s32 count;                                                             \
                                                                               \
		IRQ_LOCK(saved);                                                       \
		count = *&q->cnt;                                                      \
		if(count <= 31) {                                                      \
			u32 *entry = q->entries[count];                                    \
                                                                               \
			*&q->cnt = count + 1;                                              \
			*entry++ = (value);                                                \
			*entry++ = (address);                                              \
			*entry = (control);                                                \
		}                                                                      \
		IRQ_UNLOCK(saved);                                                     \
	}

/* Like IO_WRITE_QUEUE_PUSH, but keeps IME's address in the caller's ime */
/* variable while disabling and restoring interrupts. */
#define IO_WRITE_QUEUE_PUSH_IME(ime, address, value, control)                  \
	{                                                                          \
		IO_WRITE_QUEUE *q;                                                     \
		u32 saved;                                                             \
		s32 count;                                                             \
                                                                               \
		q = &IoWriteQueueWork;                                                 \
		IME_SAVE(ime, saved);                                                  \
		IME_DISABLE(ime);                                                      \
		count = *&q->cnt;                                                      \
		if(count <= 31) {                                                      \
			u32 *entry = q->entries[count];                                    \
                                                                               \
			*&q->cnt = count + 1;                                              \
			*entry++ = (value);                                                \
			*entry++ = (address);                                              \
			*entry = (control);                                                \
		}                                                                      \
		IME_RESTORE(ime, saved);                                               \
	}

#include "system/iwram_call.h"
#include "game/main_routine.h"

/* The matrix group. The 3D transform and its stack. */
/* Sets a twelve-word matrix to the unit matrix: FIXED_ONE (1.0) on the */
/* rotation's diagonal, the rest 0. */
static __inline__ void syUnitMatrix(MATRIX matrix) { ASM_MATRIX_UNIT(matrix); }

/* Resets the current matrix to the unit matrix. */
static __inline__ void syMatrixSetCurrentUnit(void) {
	ASM_MATRIX_UNIT_CURRENT(CurrentMatrix);
}

#include "system/mul_fx.h"

/* The ram_code group. Copying ARM functions to RAM to run them there. */
/* */
/* Copies an ARM function to RAM for execution. bytes is its linker-defined */
/* size. */

/* Allocates a heap copy; release it with syHeapRelease when finished. */
#define syRamCodeLoad(to, code, bytes)                                         \
	do {                                                                       \
		u32 size_;                                                             \
                                                                               \
		size_ = (u32)(bytes);                                                  \
                                                                               \
		(to) = syHeapAlloc(size_);                                             \
		syDMAXCopy((const void *)(code), (to), DMA_COPY_WORDS(size_ >> 2),     \
				   REG_DMA3);                                                  \
	} while(0)

/* Allocates a heap-slot copy; release it with syHeapSlotRelease. */
#define syRamCodeLoadSlot(slot, code, bytes)                                   \
	do {                                                                       \
		u32 size_;                                                             \
                                                                               \
		size_ = (u32)(bytes);                                                  \
                                                                               \
		syDMAXCopy((const void *)(code), syHeapSlotAlloc((slot), size_),       \
				   DMA_COPY_WORDS(size_ >> 2), REG_DMA3);                      \
	} while(0)

/* The serial group. The link cable's runtime: serial packets, registers and */
/* transfer state. */
/* Adds LINK_STATUS_ERROR to a link status once a transfer has failed. */
static inline s32 sySerialAddErrorFlag(s32 status) {
	if(SerialStateWork.transferError != 0)
		status |= LINK_STATUS_ERROR;
	return status;
}

/* Abandons any send or receive and clears the block transfer's state, with */
/* interrupts held off: what a failed link battle or Battle Arena exchange */
/* does. */
static __inline__ void sySerialResetTransferState(void) {
	SERIAL_TRANSFER_STATE *state;
	u32 saved;

	state = &SerialTransfer;
	IRQ_LOCK(saved);
	{
		u32 clear = 0;

		scIoWrite8(SERIAL_STATUS_RUNNING, &state->stat);
		SerialSendSource = clear;
		SerialSendSize = clear;
		state->peerFlg = clear;
		state->flg = clear;
		SerialReceiveDestination = clear;
		SerialReceivedSize = clear;
	}
	IRQ_UNLOCK(saved);
}

#include "system/vector3.h"

#endif
