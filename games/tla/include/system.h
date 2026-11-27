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
/* from system/globals.h, and keeps the functions: the io_reg, task and irq */
/* groups' first, then the stub table's prototypes, then each other */
/* group's inlines and macros in turn. */
/* */
/* The two inlines only the system library's own parts call, */
/* sySerialAddErrorFlag and syMatrixSetCurrentUnit, are in */
/* src/system/system_internal.h, not here: each inline that names a variable's
 */
/* address takes a constant-pool label where it is defined, and a unit's later
 */
/* functions number theirs after it. The game's */
/* units never defined those two, and gmMainRoutineStateInitDefaults */
/* (game/game_units.c) compiles as the ROM has it only with its labels */
/* numbered as they were. */

#include "types.h"
#include "fixed_math.h"
#include "edition.h"
#include "system/asm.h"
#include "system/definitions.h"
#include "system/globals.h"

/* The io_reg group. The hardware's I/O registers and their bits. */
/* Stores an RGB colour, built in a local of its own, as the palette functions
 */
/* several scenes copy store each colour (scene/shared/palette_cycle_fade.h and
 */
/* its kin). */
#define SET_RGB(lvalue, r, g, b)                                               \
	do {                                                                       \
		int rgb = RGB(r, g, b);                                                \
		(lvalue) = rgb;                                                        \
	} while(0)

/* The task group. Tasks, their priorities and the task table, with room for 24
 */
/* tasks. */
void fdPaletteCycleStart(void);

void syTaskCopyWords(u32 *destination, u32 *source, u32 byteCount);
void syTaskSort(void);
void syTaskIdle(void);
void syTaskEmpty(void);
s32 syTaskSuspendOverlayTasks(void);
s32 syTaskResumeOverlayTasks(void);
void mnResumeFieldTasks(void);
s32 mnSuspendFieldTasks(void);

s32 syTaskCountInPass(s32 pass);
void syTaskRunPassBeforeFrameTasks(s32 pass);

/* The irq group. The interrupt sources, their handler table and the resident */
/* IWRAM code the interrupts enter through. */
void syInstallIwramAndIrqs(void);
void syIrqIgnore(void);

struct AFFINE_TRANSFORM;
struct PROJECTED_POINT;
struct UNIT;

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
/* Returns the integer square root of `value`, rounded down. */
s32 syMathIntegerSqrt(s32 value);
FIXED sySquartFX(FIXED value);
/* The sine of `angle`, a whole turn being TRIG_TURN, in 16.16. */
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
/* Returns the EWRAM bytes left to allocate, up to the end of EWRAM. */
s32 syHeapGetRemainingEwram(void);
void *syHeapAlloc(s32 size);
void *syHeapAllocEwram(s32 size);
/* Releases `buffer`, a block taken with no heap slot, and every later block of
 */
/* its pool, as syHeapSlotRelease does. */
void syHeapRelease(void *buffer);
void syPrint(const u8 *src);
/* Prints the last `digits` hexadecimal digits of `value`. */
void syPrintHex(u32 value, s32 digits);
/* Prints the last `digits` characters of `value` in signed decimal. */
void syDebugTextPrintDecimal(s32 value, s32 digits);
/* Writes `count` blank characters at the debug text cursor and moves it on. */
void syDebugTextPrintBlanks(u32 count);
/* As syDecompressTaggedLz, with the byte LZ decoder. */
s32 syDecompressByteLz(const void *source, void *destination);
s32 syDecompressTaggedLz(const void *source, void *destination);
void syVramCacheReset(void);
s32 syVramCacheFreeEntry(u32 no);
s32 syVramCacheReleaseVram(u32 no);
s32 syVramCacheLoad(u32 no, u32 size, const void *src);
s32 syVramCacheFindFreeEntry(void);
void syDebugTextClear(void);
s32 syOamMatrixAllocFromTransform(AFFINE_TRANSFORM *source);
void syOamAddSprAtDepthClamped(void *entry, s32 slot);
void syWindowLoadGraphics(void);
void syPaletteLoadBasicSprCols(void);
/* Sets up the one-level matrix stack and makes the current matrix the unit */
/* matrix. */
void syInitMatrix(void);
void syMatrixResetUnit(void);
void syPushMatrix(void);
void syPopMatrix(void);
/* Rotates CurrentMatrix about the X, Y and Z axes by `angles`, in that order.
 */
void syMatrixRotate(s32 *angles);
void syRotX(ANGLE angle);
void syRotY(ANGLE angle);
void syRotZ(ANGLE angle);
void syMatrixTranslateVector(const POINT position);
/* Scales CurrentMatrix by `scale` (x, y, z factors in 16.16). */
void syMatrixScaleVector(const VECTOR scale);
/* Rotates CurrentMatrix by `angles` and translates it by `position`. */
void syMatrixRotateTranslate(s32 *angles, POINT position);
/* Rotates by `angles`, scales the rows by `scale`, translates by `position`. */
void syMatrixRotateTranslateScale(s32 *angles, POINT position, VECTOR scale);
void syMatrixSetLookAt(POINT eye, POINT target);
/* Projects `point` through CurrentMatrix into `screen`; returns its scale. */
s32 syConvert3Dto2D(const POINT point, struct PROJECTED_POINT *screen);
/* Sets the projection from a vertical field of view (1/65536 turns). */
void syPerspective(s32 fieldOfView, FIXED nearDepth, FIXED farDepth);
void sySetScreenDist(u32 focal, u32 nearDepth, u32 farDepth);
void syCopyBuildStamp(void);
s32 syDummyReturnZero(s32 resourceId, const u8 *table);
void *syResourceGetTableEntry(s32 resourceId);
void syResourceReservedNoOp(s32 unused);
void syOverlayLoad(u32 index, void *destination);
void syScrFadeToBlack(s32 vblanks);
void syScrFadeFromBlack(s32 vblanks);
void syScrFadeToWhite(s32 vblanks);
void syScrFadeFromWhite(s32 vblanks);
/* Starts a fade of any layers, toward white when `bright` is 1, black when 0.
 */
void syScrFadeStart(s8 bright, s32 layer, u32 start, s32 target, s32 vblanks);
void syScrFadeWait(void);
/* Unused: five empty exports the stub table reserves. */
void syDummyReservedNoOpA(void);
void syDummyReservedNoOpB(void);
void syDummyReservedNoOpC(void);
void syDummyReservedNoOpD(void);
void syDummyReservedNoOpE(void);
void sySerialInit(void);
/* Looks for the link partner and, once connected, does one exchange. */
s32 sySerialPollAndTransfer(void *send, void *receive);
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
/* Waits for the status bits in `mask`; returns this console's multiplayer id.
 */
u32 sySerialWaitForStatMask(s32 mask);
void sySerialRemoveIrqHandlers(void);
/* Checks reverb code `code`'s range and returns 0, loading nothing. */
s32 sySndLoadReverbCode(s32 code);
s32 sySerialStartSend(void *source, s32 size);
s32 sySerialStartReceive(void *destination);
void sySerialWaitForSend(void);
void sySerialWaitForReceive(void);
void sySerialWaitForTransfers(void);
s32 sySerialGetActiveTransfers(void);
void sySerialStepBlockTransfer(void);
s32 syTaskSetFlags(TASK_FUNC func, u32 mask);
struct UNIT *syUnitGet(u32 unitId);
s32 syGameFlagGet(s32 flag);
void syGameFlagClear(s32 flag);
u32 syGameFlagToggle(s32 flag);
s32 syGameFlagGetByte(s32 flag);
u8 syGameFlagIncrementByte(s32 flag);
u8 syGameFlagDecrementByte(s32 flag);
s32 syGameFlagGetNibble(u32 flag);
void syGameFlagSetNibble(s32 flag, s32 value);
s32 syTaskFind(TASK_FUNC func);
/* Counts the free blocks of the sprite VRAM cache. */
s32 syVramCacheCountFreeBlocks(void);

/* The system library's functions and globals that the stub table does not */
/* list. */

/* The stub table's names for four of its functions, which other modules' */
/* code calls (system/stubs.s). */
/* Returns a unit's record, a party member's or an enemy's. */
struct UNIT *gmUnitGetStub(u32 unitId);
/* Returns whether a game flag is set. */
s32 syGameFlagGetStub(s32 flag);
/* Sets a game flag and returns its old state, as syGameFlagSet. */
s32 syGameFlagSetStub(s32 flag);
/* Clears the flag, returning nothing where syGameFlagSetStub returns its byte.
 */
void syGameFlagClearStub(s32 flag);

/* The system library's own functions and globals, by the part each belongs to:
 */
/* the frame, random numbers, tasks, sequences and overlays, sprites, matrices,
 */
/* fades, resources, windows, the debug text and the link cable. */

/* The frame and the console: waiting, interrupts, sleep, the cartridge's */
/* language and its build stamp. */
void syWaitForFrameInterrupt(void);
void syVBlankHandler(void);
/* The Game Pak interrupt's handler, in IWRAM: stops the console for good once
 */
/* the cartridge is pulled (the BIOS's Stop, again after any wake). */
void syHalt(void);
/* The BIOS's SoundBias: Off ramps the sound output's bias down before the */
/* console sleeps, On back up once it wakes, so the speaker does not click. */
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
/* 1 while the CPU is stopped for sleep, until L and R wake it. */
extern u16 SleepActive;
/* The cartridge's first byte, as the link places it (rom_header.s). */
extern const u8 Rom_Start[];
/* The cartridge's language, from its header (syGetLanguage). */
typedef enum CARTRIDGE_LANGUAGE {
	CARTRIDGE_LANGUAGE_ENGLISH = 1,
	CARTRIDGE_LANGUAGE_GERMAN,
	CARTRIDGE_LANGUAGE_FRENCH,
	CARTRIDGE_LANGUAGE_ITALIAN,
	CARTRIDGE_LANGUAGE_SPANISH
} CARTRIDGE_LANGUAGE;
s32 syGetLanguage(void);

/* Random numbers. */
/* A random number from 0 to n - 1: n times a 16-bit random draw, over 65536. */
#define syRandomBelow(n) (syRandom() * (n) >> 16)
/* syRandom's seed; Lucky Wheels seeds it with the play time, and a link */
/* battle's parent sends it as the turn's BattleRandomSeed. */
extern u32 RandomSeed;

/* The task passes and the queued register writes they flush. */
/* A task's function as the task passes call it: returning a value, where the */
/* task itself returns none (syExecuteEvent's UB). */
typedef s32 (*TASK_INT_FUNC)(void);
struct IO_WRITE_QUEUE;
/* The function that carries out `count` queued register writes */
/* (Resource_ExecuteTransferCode, run from a copy on the stack). */
typedef void (*QUEUE_FLUSH_ROUTINE)(IO_WRITE_QUEUE *queue, u32 count);
/* The function that runs the queued register writes, which */
/* syIoWriteQueueFlushPending copies onto its stack and runs. */
extern u8 Resource_ExecuteTransferCode[];
/* The size of Resource_ExecuteTransferCode. */
extern u8 Resource_ExecuteTransferSize[];

/* The sequences the main routine runs, how the game starts, and the */
/* overlays it loads. */
/* Sequence ids above the scene table's rows, as The Broken Seal's */
/* SEQUENCE_ values: the sequences other than the field sequence, which */
/* gmMainRoutineRun runs in place of a scene. */
enum {
	/* The Super Lucky Dice sequence (fdSuperLuckyDiceRun); Contigo's dealer */
	/* leaves for it. The Broken Seal's 507, SEQUENCE_NONE, runs nothing. */
	SEQUENCE_SUPER_DICE = 507,
	SEQUENCE_SLOTS = 508, /* the slots sequence (fdLuckyWheelsRun) */
	SEQUENCE_DICE = 509,  /* the dice sequence (fdLuckyDiceRun) */
	SEQUENCE_BATTLE = 510 /* a battle; the entrance is the formation */
};
/* gmMainRoutineRun's mode: where the game starts. Only GAME_START_TITLE is */
/* passed (syInit, gmRestartQuest); the others are its debug */
/* starts. */
typedef enum GAME_START {
	GAME_START_TITLE = 0, /* the Title, with the state cleared */
	GAME_START_DEBUG_VENUS_LIGHTHOUSE = 1, /* SCENE_VENUS_LIGHTHOUSE_ENTRANCE */
	GAME_START_DEBUG_CLEAR = 2,			   /* SCENE_CLEAR, as from the title */
	GAME_START_DEBUG_MT_ALEPH = 3,		   /* SCENE_MT_ALEPH_SUMMIT */
	GAME_START_DEBUG_TITLE = 4 /* the story crawl (TITLE_STORY_CRAWL) */
} GAME_START;
s32 btRunStub(s32 entrance);
/* Plays Lucky Wheels (fdLuckyWheelsStart). */
s32 fdLuckyWheelsStartStub(s32 entrance);
s32 fdSuperLuckyDiceStartStub(s32 entrance);
void fdLuckyDiceRun(void);
void scArrive(s32 unused, s32 keep);
void scLoadOverlay(s32 unused);
/* The ARM function that resolves a loaded overlay's long branches, run from a
 */
/* heap copy by syOverlayLoad. */
extern const u8 syResourcePatchThumbBranch[];
/* The size of syResourcePatchThumbBranch (decompress_tagged_lz.s). */
extern u8 syResourcePatchThumbBranchCodeSize[];
/* The compiler library's call through r3, which the IWRAM-resident code's */
/* callers use to run code copied to RAM. */
s32 _call_via_r3(s32, u8 *, u8, s32);

/* Sprites: their depth slots, affine matrices, VRAM and colours. */
void syOamResetFrame(void);
/* Queues a sprite at a depth slot of this frame's OAM list (the OAM_DEPTH_ */
/* slots below). */
/* A unit whose ROM code is compiled against another form defines */
/* OWN_Oam_AddSpriteAtDepth and declares that form itself. */
#ifndef OWN_Oam_AddSpriteAtDepth
void syOamAddSprAtDepth(void *entry, s32 slot);
#endif
/* Builds this frame's OAM list in buffer, a scratch block of OAM_SIZE bytes, */
/* running its ARM function from a copy on the stack. */
void syRenderBuildOamList(void *buffer);
/* Which of the 256 depth slots of OamDepthLists hold sprites this frame, a */
/* bit a slot; syOamResetFrame clears it. */
extern u8 OamDepthMasks[];
/* The 256 depth slots' list heads that every drawn sprite is linked into */
/* (syOamAddSprAtDepth); syOamResetFrame empties them each frame. */
extern void *OamDepthLists[];
/* Depth slots of syOamAddSprAtDepth: a higher slot is drawn in front. The */
/* windows' sprites, icons and cursors take the slots from OAM_DEPTH_WINDOW up,
 */
/* in front of the field engine's and the battle engine's, and OAM_DEPTH_FRONT
 */
/* is in front of everything. */
#define OAM_DEPTH_WINDOW 240
#define OAM_DEPTH_FRONT	 255
/* The depth slot the field Psynergy's effect sprites take: in front of every */
/* actor, whose slots are clamped below OAM_DEPTH_WINDOW, and among the window
 */
/* sprites' slots. */
#define EFFECT_SPRITE_FRONT_DEPTH 250
struct SPR_ANIMATION;
struct SPR_ANIMATION *fdSprAnimationAlloc(s32 id);
s32 fdSprPictureCacheFindByTag(s32 key);
/* Five tables that map a picture's 256 colour indices onto a sprite's palette,
 */
/* one for each palette a field sprite can take (field/sprite_color_tables.c).
 */
extern u8 SpriteColorTables[][256];
s32 syPaletteGetBasicSprCols(void);
/* Two 16-entry colour maps for 4bpp pixels: the first draws colour 1 as */
/* colour 10, the second leaves every colour as it is. */
extern u16 Graphics_ExpandNibbleTable[];
s32 syVramCacheAllocateBlocks(u32 id, u32 size);
s32 syVramCacheFreeBlocks(s32 id);
s32 syVramCacheAllocEntry(s32 size);
s32 syVramCacheReload(s32 id, s32 src);

/* Matrices and the projection. */
void syMatrixSetRotateY(ANGLE angle);
/* The basic sprite palettes, loaded for the debug text */
/* (syPaletteGetBasicSprCols). */
extern const u8 System_BasicColorPalette[];

void syTranslate(FIXED x, FIXED y, FIXED z);
/* Sets the current matrix's translation and keeps its rotation. */
void syMatrixSetTranslation(const POINT position);
void syMatrixScaleUniform(FIXED scale);
void syScale(FIXED x, FIXED y, FIXED z);
/* The far depth well past the battlefield: the largest whole 16.16 number. */
#define BATTLE_FAR_DEPTH toFIXED(32767)
/* The dot product of (ax, ay, az) and (bx, by, bz), in 16.16 */
/* (RelocatedInnerProduct, run from a copy on the stack). */
typedef s32 (*DOT_PRODUCT_ROUTINE)(s32 ax, s32 bx, s32 ay, s32 by, s32 az,
								   s32 bz);

/* Screen fades. */
void syScrFadeUpdate(void);

/* Resources and decompression. */
/* Each resource's ROM address by resource id (syResourceGetTableEntry). */
extern void *syResourceDirectoryTable[];
void syDecompressHalfwords(const void *source, void *destination);
/* The halfword LZ decoder that syDecompressHalfwords runs from its stack. */
extern const u8 Decompress_HalfwordLz[];
/* The size of Decompress_HalfwordLz, copied onto the stack by */
/* syDecompressHalfwords. */
extern u8 syDecompressHalfwordsCodeSize[];
s32 syDecompressMoveToFrontLz(const void *source, void *destination);
s32 mnAbilityIconLoadIntoSlot(s32 resource, s32 index);

/* Windows and their text. */
/* The window font's 16 colours, loaded as BG palette 15 */
/* (syWindowLoadGraphics). */
extern const u16 Window_Palette[];
/* The party status window's layout (PARTY_STAT_WINDOW), the block of */
/* heap slot HEAP_SLOT_PARTY_STATUS. */
extern u8 PartyStatusState[];
/* The link battle timer's heading: `TIME`; `TMPO.` in Spanish, `TEMPS` in */
/* French. */
extern u8 Link_TimeLabelString[];

/* The debug text and the number strings it prints. */
/* Nonzero lets syPrint write; the power-on set-up and the credits */
/* clear it. */
extern u8 DebugTextEnabled;
/* Where the debug text writes next on its screen in BG0. */
extern u16 *DebugTextCursor;
/* The digits syStringFormatHex and syStringFormatDecimal write, right-aligned
 */
/* up to NumberTextHexEnd and NumberTextDecimalEnd. */
extern u8 NumberTextBuffer[];
/* Writes a value to NumberTextBuffer right-aligned: spaces, a sign and its */
/* decimal digits. */
void syStringFormatDecimal(s32);
void syStringFormatHex(u32);
/* The hexadecimal digits 0 to F, which syStringFormatHex writes from. */
extern const u8 String_HexDigits[];
/* The end of syStringFormatDecimal's digits: syDebugTextPrintDecimal prints */
/* the last places before it. */
extern u8 NumberTextDecimalEnd[];
/* The end of syStringFormatHex's digits: syPrintHex prints the last */
/* places before it. */
extern u8 NumberTextHexEnd[];

/* The link cable. */
/* A halfword in a record of its own, which sySerialStepBlockTransfer keeps its
 */
/* constant 1 in. */
typedef struct SERIAL_HALFWORD {
	u16 value;
} SERIAL_HALFWORD;

/* Declarations other modules' code shares. */
struct BATTLE_OBJECT;
struct ACTOR;
/* An empty function, reached only through an unnamed stub in the object */
/* module's stub table. */
void fdActorNoOp(void);
s32 fdTilesetAnimResumeAllStub(void);
s32 fdTilesetAnimSuspendAllStub(void);

/* The system library's groups. */

/* The affine group. Sprite rotation and scaling: a sprite's transform and the
 */
/* OAM matrices this frame hands out for it. */
s32 syAffineMatrixBuild(s32 angle, s32 xScale, s32 yScale);

/* The dma group. DMA and the BIOS copy service: fills, and one transfer on a */
/* channel. */
/* The BIOS CpuSet service (bios.s). */
void Bios_CpuSet(const void *source, void *destination, u32 control);

/* Stops DMA 0, which a scene may have left copying a line each HBlank: drops */
/* its start timing and repeat, then its enable, and reads the control back. */
#define DMA0_STOP()                                                            \
	{                                                                          \
		volatile u16 *dma0 = REG_DMA0;                                         \
                                                                               \
		dma0[DMA0_CONTROL] &= ~DMA_START_TIMING_DRQ_REPEAT;                    \
		dma0[DMA0_CONTROL] &= ~DMA_ENABLE;                                     \
		dma0[DMA0_CONTROL];                                                    \
	}

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

/* btDmaFill32 through `cell`, a word the caller declares, so that a function */
/* filling several blocks sets one word before each (fdActorSystemInit's */
/* two fills). */
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

/* Starts a transfer on DMA channel `channel`: its source, destination and */
/* control written in one store. */
static __inline__ void syDMAXCopy(const void *source, void *destination,
								  u32 control, volatile u32 *channel) {
	ASM_REGISTER(volatile u32 *dma, "r3") = channel;
	ASM_REGISTER(u32 src, "r0") = (u32)source;
	ASM_REGISTER(u32 dst, "r1") = (u32)destination;
	ASM_REGISTER(u32 cnt, "r2") = control;
	ASM_DMA_STORE(src, cnt, dma, dst);
}

/* The frame group. The frame loop's globals: the VBlanks a frame takes, the */
/* CPU load display and the VBlank flags, which sySynch() */
/* (system/system_frame.c) and the VBlank handler share. */
/* Stops the sound DMA and its timer. */
void sySndSuspendDirectSndStub(void);
/* The resident mixer in IWRAM, which the frame wait runs after */
/* SoundDriver_EnterVBlankUpdate returns 0. It never reads its argument; the */
/* callers pass 8. */
void IwramSoundRenderVBlank(s32) __attribute__((long_call));

/* The heap_release group. The heap's allocators and releases */
/* (system/system_heap.c). */
void syHeapFillFreeWords(u32 value);

/* A block of size bytes from the allocator given (syHeapAlloc or */
/* syHeapAllocEwram), with no heap slot. */
static __inline__ void *syHeapCallAlternatePool(void *(*allocate)(s32),
												s32 size) {
	return allocate(size);
}

/* The input group. The keypad state the VBlank interrupt keeps. */
/* */
/* The VBlank handler reads the keypad into InputWork, and */
/* syInputUpdateKeyRepeatAndDirection turns it into repeats after each frame */
/* (system/system_frame.c). held, pressed, repeat and repeatTmr are volatile
 */
/* because the VBlank handler sets them while the code it interrupts reads */
/* them; direction and prevDir the key repeat rebuilds and reads each
 */
/* frame. */
void syInputUpdateKeyRepeatAndDirection(void);
/* The keypad interrupt's handler. */
void syInputHandleKeyInterrupt(void);

/* The io_write group. The register writes and reads the whole game makes */
/* through a local. */
/* Writes a value to a 16-bit register or memory location, through a local of */
/* its own. */
#define syIoWrite16(v, reg)                                                    \
	do {                                                                       \
		u32 value_ = (v);                                                      \
		*(reg) = value_;                                                       \
	} while(0)

/* Writes a value to a 32-bit register or memory location, in syIoWrite16's */
/* shape. */
#define syIoWrite32(v, reg) syIoWrite16(v, reg)

/* Writes a value to an 8-bit register or memory location, in syIoWrite16's */
/* shape. */
#define syIoWrite8(v, reg) syIoWrite16(v, reg)

/* Reads a 16-bit register or memory location into the variable at */
/* `variable`: the battle engine's mosaic fade saves its background controls */
/* so, and Gaia Rock's slate and Anemos Inner Sanctum's symbol their blend */
/* levels. */
#define syIoRead16(reg, variable) (*(variable) = *(reg))

/* The irq_lock group. Turning interrupts off around a change the interrupt */
/* handlers also make, and back on as they were. */
/* Saves IME through `ime`, a pointer to REG_IME, into `saved`. */
#define IME_SAVE(ime, saved)                                                   \
	do {                                                                       \
		(saved) = *(ime);                                                      \
	} while(0)

/* Disables interrupts by writing the pointer's own low halfword through it: */
/* REG_IME's address ends in 0x208, whose bit 0 is clear. */
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

/* The io_write_queue group. The I/O write queue: register writes held until */
/* VBlank. */
void syIoWriteQueueFlushPending(void);

/* Queues one register write with interrupts off; a write to a full queue is */
/* dropped. `control` is an IO_WRITE_ width, with IO_WRITE_SET or */
/* IO_WRITE_CLEAR, to write `value` to `address`; or a DMA control word, to */
/* copy by DMA 3 from `value` to `address`. */
#define IO_WRITE_QUEUE_PUSH(address, value, control)                           \
	{                                                                          \
		IO_WRITE_QUEUE *q = &IoWriteQueueWork;                                 \
		u32 saved;                                                             \
		s32 count;                                                             \
                                                                               \
		IRQ_LOCK(saved);                                                       \
		count = *&q->cnt;                                                      \
		if(count < IO_WRITE_QUEUE_SIZE) {                                      \
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
		if(count < IO_WRITE_QUEUE_SIZE) {                                      \
			u32 *entry = q->entries[count];                                    \
                                                                               \
			*&q->cnt = count + 1;                                              \
			*entry++ = (value);                                                \
			*entry++ = (address);                                              \
			*entry = (control);                                                \
		}                                                                      \
		IME_RESTORE(ime, saved);                                               \
	}

/* The iwram_call group. The functions resident in IWRAM, called at their fixed
 */
/* addresses: the fixed-point multiply and divides, the square root, word fills
 */
/* and copies, the matrix multiply and the picture decoder. */
/* The 16.16 product of two 16.16 numbers (syMulFX), written as a call. */
static inline FIXED syMathMulFX(FIXED a, FIXED b) { return syMulFX(a, b); }

/* value * FIXED_ONE / divisor: the 16.16 ratio of two 16.16 numbers. */
static inline FIXED syDivFX(FIXED divisor, FIXED value) {
	return syFixedDivEntry(divisor, value);
}

/* The integer square root, rounded down, of a value from 0; 0 for a */
/* negative one. A 16.16 square comes back as 8.8. */
static inline s32 sySquart(s32 value) { return syMathSqrtEntry(value); }

/* Multiplies matrix, a 3 by 3 matrix and a translation in 16.16 fixed point, */
/* into the current transform. */
static inline void syMultiMatrix(const MATRIX matrix) {
	syMatrixMultiplyEntry(matrix);
}

/* Writes to result the 16.16 point vector moved by the current matrix: turned
 */
/* by its 3 by 3 part, then offset by its translation. */
static inline s32 syCalcPoint(const POINT vector, POINT result) {
	return syVectorTransformEntry(vector, result);
}

/* The quotient, rounded toward zero. */
static inline s32 syIntegerDivide(s32 dividend, s32 divisor) {
	return syIntegerDivideEntry(dividend, divisor);
}

/* The unsigned quotient, rounded down. */
static inline u32 syIntegerDivideUnsigned(u32 dividend, u32 divisor) {
	return syIntegerDivideUnsignedEntry(dividend, divisor);
}

/* Unpacks a PICTURE_FORMAT_COMPRESSED picture into destination, a byte above */
/* 223 leaving that many less 223 bytes there untouched, and returns NULL; one
 */
/* whose first halfword is 0 is not packed, and comes back as the data after */
/* that halfword for the caller to decode. */
static inline void *syDecompressSprPicture(const void *source,
										   void *destination) {
	return syDecompressSprPictureEntry(source, destination);
}

/* Clears size bytes, rounded down to whole words, from destination. Returns */
/* the address after the last word. */
static inline s32 syWordsClear(void *destination, s32 size) {
	return syWordsClearEntry(destination, size);
}

/* As syWordsClear, filling with value. */
static inline s32 syWordsFill(void *destination, s32 size, u32 value) {
	return syWordsFillEntry(destination, size, value);
}

/* Copies size bytes, rounded down to whole words, from source to */
/* destination. */
static inline void syWordsCopy(void *destination, const void *source,
							   s32 size) {
	syWordsCopyEntry(destination, source, size);
}

/* syWordsFill as a statement of its own, as twelve fills in the battle engine,
 */
/* its effects and the field engine's Psynergy are written: among them the */
/* battle camera's and the transition's curtain tiles, which The Broken Seal */
/* fills by DMA with btDmaFill32Cell. */
#define IWRAM_FILL_WORDS(destination, size, value)                             \
	do {                                                                       \
		syWordsFill(destination, size, value);                                 \
	} while(0)

/* syWordsCopy as a statement of its own, as IWRAM_FILL_WORDS is: the link */
/* code copies the Djinn recovery table into its send buffer so, in the lobby */
/* and in the battle, as The Broken Seal's does. */
#define IWRAM_COPY_WORDS(destination, source, size)                            \
	do {                                                                       \
		syWordsCopy(destination, source, size);                                \
	} while(0)

/* The IWRAM function behind syMulFX, called by its linked name. */
FIXED IwramMulFX(FIXED a, FIXED b) __attribute__((long_call));
/* The compiler library's signed division and remainder, which the overlays */
/* import from the IWRAM functions (system/iwram_entry.s). */
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

/* The matrix group. The current 3D matrix, its one-deep stack, and the unit */
/* matrix. */
void syMatrixRewindStack(void);
void syGetMatrix(MATRIX destination);
void syLoadMatrix(const MATRIX source);
void syMatrixBuildLookAt(POINT eye, POINT target, MATRIX out);

/* Sets a twelve-word matrix to the unit matrix: FIXED_ONE (1.0) on the */
/* rotation's diagonal, the rest 0. */
static __inline__ void syUnitMatrix(MATRIX matrix) { ASM_MATRIX_UNIT(matrix); }

#include "system/mul_fx.h"

/* The ram_code group. Copying an ARM function to RAM to run it there: the */
/* decoders, the overlay loader's branch patcher and the battle effects' canvas
 */
/* functions. */
/* Copies the function `code`, `bytes` long (its linked size), into a block */
/* taken with syHeapAlloc and points `to` at it; syHeapRelease frees it. */
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

/* As syRamCodeLoad, into heap slot `slot`'s block; syHeapSlotRelease frees it.
 */
#define syRamCodeLoadSlot(slot, code, bytes)                                   \
	do {                                                                       \
		u32 size_;                                                             \
                                                                               \
		size_ = (u32)(bytes);                                                  \
                                                                               \
		syDMAXCopy((const void *)(code), syHeapSlotAlloc((slot), size_),       \
				   DMA_COPY_WORDS(size_ >> 2), REG_DMA3);                      \
	} while(0)

/* The colour buffer functions (system/decompress_byte_lz.s), each of which */
/* copies its ARM kernel onto the stack by DMA and runs it there. */
/* Copies `size` bytes of colour levels, halving each. */
void syColBufferHalve(void *source, void *destination, s32 size);
/* Copies `size` bytes of colour levels at three quarters of each, returning */
/* what the kernel leaves in r0. */
s32 syColBufferScaleThreeQuarters(void *source, void *destination, s32 size);
/* Copies `size` bytes of colour levels, each lowered by `amount`. */
void syColBufferDarken(void *source, s32 amount, void *destination, s32 size);
/* Copies `size` bytes of colour levels, each raised by `amount`. */
void syColBufferBrighten(void *source, s32 amount, void *destination, s32 size);
/* Copies size bytes of 8-bit pixels from source to destination, then halves */
/* each colour number left in source, so a drawing fades as it is shown: Lucky
 */
/* Wheels' trails (CANVAS_FADE_HALF). */
void syColBufferHalveNonzero(u8 *source, u8 *destination, s32 size);
/* syColBufferHalveNonzero, taking each colour number to three quarters, for */
/* the trails that fade more slowly. */
void syColBufferScaleNonzeroThreeQuarters(u8 *source, u8 *destination,
										  s32 size);
/* The high word of value times magic, plus value times multiplier: a division
 */
/* by a constant (system/trig.s). */
s32 syMathUnsignedMulHighAdd(s32 value, u32 magic, s32 multiplier);

/* The save group. The save in flash: its sectors, parts and summaries, and the
 */
/* save's state. */
u32 sySaveFindLatestSector(s32 partId);
u32 sySaveReadPart(s32 partId, void *destination);
u32 sySaveDeletePart(s32 partId);

/* The save in flash. */
s32 sySaveCompareBytes(u8 *left, u8 *right, s32 count);
s32 sySaveChecksumSector(void);
s32 sySaveReadSector(s32 index);
u32 sySaveWriteSector(s32 sector);
s32 sySaveInvalidateSector(s32 index);
u32 sySaveSelectWriteSector(s32 partId);
u32 sySaveGetLatestCounter(s32 partId);
void sySaveSetProgressCallback(void (*progress)(s32 step));
s32 sySaveWritePart(s32 partId, void *source);
s32 sySaveInitState(void);
s32 sySaveLoadFileSummaries(void);
void sySaveReleaseState(void);

/* The scene group. The running scene's overlay area, a field position, and the
 */
/* area name a scene and entrance show. */
s32 fdAreaGetNameIndexStub(s32 scene, s32 entrance);

/* The serial group. The link cable's runtime: serial packets, registers and */
/* transfer state. */

/* Starts a DMA 3 transfer; control_ is the unit count with the DMA_ flags */
/* << 16. */
#define START_DMA(source_, destination_, control_)                             \
	do {                                                                       \
		DMA3->src = (u32)(source_);                                            \
		DMA3->dst = (u32)(destination_);                                       \
		DMA3->ctrl = (control_);                                               \
	} while(0)

s32 sySerialPollStat(void);
s32 sySerialExchangePayloads(void *send, void *receive);
void sySerialPrepareSendPacket(void *payload);
u8 sySerialCollectReceivedPayloads(void *payload);
void sySerialEnableTransferTimer(void);
void sySerialDisableTransfer(void);
void syLinkBattleResetTransferState(void);
void sySerialHandleTransferInterrupt(void);

#include "system/vector3.h"

#endif
