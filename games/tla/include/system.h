#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

/* The system library's one header: its types, its functions and its macros. A
   file that uses the system library includes this and nothing else of it, as a
   Saturn program includes SGL's sgl.h. The name is the staff roll's: under
   Programming, its first heading is `System`.

   As SGL's sgl.h takes its types from sl_def.h, this takes the system library's
   types and constants from system/definitions.h and its variables from
   system/globals.h, and keeps the functions: the io_reg, event and irq groups'
   first, then the stub table's prototypes, then each other group's inlines and
   macros in turn.

   The two inlines only the system library's own parts call,
   sySerialAddErrorFlag and syUnitMatrixCurrentInline, are in
   src/system/system_internal.h, not here: each inline that names a variable's
   address takes a constant-pool label where it is defined, and a unit's later
   functions number theirs after it. The game's units never defined those two,
   and gmMainRoutineStateInitDefaults (game/game_units.c) compiles as the ROM
   has it only with its labels numbered as they were. */

#include "types.h"
#include "fixed_math.h"
#include "edition.h"
#include "system/asm.h"
#include "system/definitions.h"
#include "system/globals.h"

/* The io_reg group. The hardware's I/O registers and their bits. Stores an RGB
   colour, built in a local of its own, as the palette functions several scenes
   copy store each colour (scene/shared/palette_cycle_fade.h and its kin). */
#define SET_RGB(lvalue, r, g, b) \
	do { \
		int rgb = RGB(r, g, b); \
		(lvalue) = rgb; \
	} while(0)

/* The event group. Events, their priorities and the event table, with room for
   24 events. */
void fdPaletteCycleStart(void);

void   syCopyEventWords(Uint32 *dst, Uint32 *src, Uint32 byteCnt);
void   sySortEvents(void);
void   syEventNoOperationA(void);
void   syEventNoOperationB(void);
Sint32 sySuspendOverlayEvents(void);
Sint32 syResumeOverlayEvents(void);
void   mnResumeFieldEvents(void);
Sint32 mnSuspendFieldEvents(void);

Sint32 syCountEventsInPass(Sint32 pass);
void   syRunPassBeforeTickEvents(Sint32 pass);

/* The irq group. The interrupt sources, their handler table and the resident
   IWRAM code the interrupts enter through. */
void syInstallIWRAMAndIRQs(void);
void syIRQIgnore(void);

struct AFFINE_TRANSFORM;
struct PROJECTED_POINT;
struct UNIT;

/* The system library's functions in the order of its stub table
   (src/system/stubs.s). */
void   sySynch(Sint32 ticks);
void   syInitEvent(void);
Sint32 sySetEvent(EVENT_FUNC func, Sint32 pri);
Sint32 syCloseEvent(EVENT_FUNC func);
Sint32 sySuspendEvent(EVENT_FUNC func);
Sint32 syResumeEvent(EVENT_FUNC func);
void   syExecuteEvent(Sint32 pass);
Uint32 syRandom(void);
ANGLE  syAtan(FIXED x, FIXED y);
/* Returns the integer square root of `value`, rounded down. */
Sint32 sySquart(Sint32 value);
FIXED  sySquartFX(FIXED value);
/* The sine of `ang`, a whole turn being TRIG_TURN, in 16.16. */
FIXED sySin(ANGLE ang);
/* The cosine of `ang`, a whole turn being TRIG_TURN, in 16.16; it falls
   through into sySin a quarter turn on. */
FIXED syCos(ANGLE ang);
void  syVectorAddPolarOffset(FIXED radius, ANGLE ang, POINT pos);
void  syIntFunction(Uint32 irq, Sint32 vcount, INTERRUPT_HANDLER handler);
void  syHeapInit(void);
void *syHeapSlotAlloc(Sint32 slot, Sint32 size);
void *syHeapSlotAllocEWRAM(Sint32 slot, Sint32 size);
/* Releases heap slot `slot`'s block, and with it every later block of the same
   pool, by setting the pool's next free address back to the block's start. */
void syHeapSlotRelease(Sint32 slot);
/* Returns the EWRAM bytes left to allocate, up to the end of EWRAM. */
Sint32 syHeapGetRemainingEWRAM(void);
void  *syHeapAlloc(Sint32 size);
void  *syHeapAllocEWRAM(Sint32 size);
/* Releases `buf`, a block taken with no heap slot, and every later block of
   its pool, as syHeapSlotRelease does. */
void syHeapRelease(void *buf);
void syPrint(const Uint8 *src);
/* Prints the last `digits` hexadecimal digits of `value`. */
void syPrintHex(Uint32 value, Sint32 digits);
/* Prints the last `digits` characters of `value` in signed decimal. */
void syPrintDec(Sint32 value, Sint32 digits);
/* Writes `cnt` blank characters at the debug text cursor and moves it on. */
void syPrintBlanks(Uint32 cnt);
/* As syDecompressTaggedLZ, with the byte LZ decoder. */
Sint32 syDecompressByteLZ(const void *src, void *dst);
Sint32 syDecompressTaggedLZ(const void *src, void *dst);
void   syVRAMCacheReset(void);
Sint32 syVRAMCacheFreeEntry(Uint32 no);
Sint32 syVRAMCacheReleaseVRAM(Uint32 no);
Sint32 syVRAMCacheLoad(Uint32 no, Uint32 size, const void *src);
Sint32 syVRAMCacheFindFreeEntry(void);
void   syPrintClear(void);
Sint32 syOAMMatrixAllocFromTransform(AFFINE_TRANSFORM *src);
void   syOAMAddSprAtDepthClamped(void *entry, Sint32 slot);
void   syWindowLoadGraphics(void);
void   syPaletteLoadBasicSprCols(void);
/* Sets up the one-level matrix stack and makes the current matrix the unit
   matrix. */
void syInitMatrix(void);
void syUnitMatrixCurrent(void);
void syPushMatrix(void);
void syPopMatrix(void);
/* Rotates CurrentMatrix about the X, Y and Z axes by `angles`, in that
   order. */
void syMatrixRotate(Sint32 *angles);
void syRotX(ANGLE ang);
void syRotY(ANGLE ang);
void syRotZ(ANGLE ang);
void syMatrixTranslateVector(const POINT pos);
/* Scales CurrentMatrix by `scale` (x, y, z factors in 16.16). */
void syMatrixScaleVector(const VECTOR scale);
/* Rotates CurrentMatrix by `angles` and translates it by `pos`. */
void syMatrixRotateTranslate(Sint32 *angles, POINT pos);
/* Rotates by `angles`, scales the rows by `scale`, translates by `pos`. */
void syMatrixRotateTranslateScale(Sint32 *angles, POINT pos, VECTOR scale);
void syLookAtLoad(POINT eye, POINT tgt);
/* Projects `point` through CurrentMatrix into `screen`; returns its scale. */
Sint32 syConvert3Dto2D(const POINT point, struct PROJECTED_POINT *screen);
/* Sets the projection from a vertical field of view (1/65536 turns). */
void   syPerspective(Sint32 fieldOfView, FIXED nearDepth, FIXED farDepth);
void   sySetScreenDist(Uint32 focal, Uint32 nearDepth, Uint32 farDepth);
void   syCopyBuildStamp(void);
Sint32 syDummyReturnZero(Sint32 resourceId, const Uint8 *tbl);
void  *syResourceGetTableEntry(Sint32 resourceId);
void   syResourceReservedNoOperation(Sint32 unused);
void   syOverlayLoad(Uint32 idx, void *dst);
void   syScreenFadeToBlack(Sint32 frames);
void   syScreenFadeFromBlack(Sint32 frames);
void   syScreenFadeToWhite(Sint32 frames);
void   syScreenFadeFromWhite(Sint32 frames);
/* Starts a fade of any layers, toward white when `bright` is 1, black when
   0. */
void syScreenFadeStart(
	Sint8 bright, Sint32 layer, Uint32 start, Sint32 tgt, Sint32 frames);
void syScreenFadeWait(void);
/* Unused: five empty exports the stub table reserves. */
void syDummyReservedNoOperationA(void);
void syDummyReservedNoOperationB(void);
void syDummyReservedNoOperationC(void);
void syDummyReservedNoOperationD(void);
void syDummyReservedNoOperationE(void);
void sySerialInit(void);
/* Looks for the link partner and, once connected, does one exchange. */
Sint32 sySerialPollAndTransfer(void *send, void *receive);
void   syIOWriteQueueStore8(Uint32 adr, Uint32 value);
void   syIOWriteQueueStore16(Uint32 adr, Uint32 value);
void   syIOWriteQueueStore32(Uint32 adr, Uint32 value);
void   syIOWriteQueueSetBits8(Uint32 adr, Uint32 value);
void   syIOWriteQueueSetBits16(Uint32 adr, Uint32 value);
void   syIOWriteQueueSetBits32(Uint32 adr, Uint32 value);
void   syIOWriteQueueClearBits8(Uint32 adr, Uint32 value);
void   syIOWriteQueueClearBits16(Uint32 adr, Uint32 value);
void   syIOWriteQueueClearBits32(Uint32 adr, Uint32 value);
void   syInputResetKeyRepeat(void);
void   syLookAt(POINT eye, POINT tgt);
/* Waits for the status bits in `mask`; returns this console's multiplayer
   id. */
Uint32 sySerialWaitForStatMask(Sint32 mask);
void   sySerialRemoveIRQHandlers(void);
/* Checks reverb code `code`'s range and returns 0, loading nothing. */
Sint32		 sySndLoadReverbCode(Sint32 code);
Sint32		 sySerialStartSend(void *src, Sint32 size);
Sint32		 sySerialStartReceive(void *dst);
void		 sySerialWaitForSend(void);
void		 sySerialWaitForReceive(void);
void		 sySerialWaitForTransfers(void);
Sint32		 sySerialGetActiveTransfers(void);
void		 sySerialStepBlockTransfer(void);
Sint32		 sySetEventFlags(EVENT_FUNC func, Uint32 mask);
struct UNIT *syUnitGet(Uint32 unitId);
Sint32		 syGameFlagGet(Sint32 flg);
void		 syGameFlagClear(Sint32 flg);
Uint32		 syGameFlagToggle(Sint32 flg);
Sint32		 syGameFlagGetByte(Sint32 flg);
Uint8		 syGameFlagIncrementByte(Sint32 flg);
Uint8		 syGameFlagDecrementByte(Sint32 flg);
Sint32		 syGameFlagGetNibble(Uint32 flg);
void		 syGameFlagSetNibble(Sint32 flg, Sint32 value);
Sint32		 syFindEvent(EVENT_FUNC func);
/* Counts the free blocks of the sprite VRAM cache. */
Sint32 syVRAMCacheCountFreeBlocks(void);

/* The system library's functions and globals that the stub table does not
   list. */

/* The stub table's names for four of its functions, which other modules' code
   calls (system/stubs.s). Returns a unit's record, a party member's or an
   enemy's. */
struct UNIT *syUnitGetStub(Uint32 unitId);
/* Returns whether a game flag is set. */
Sint32 syGameFlagGetStub(Sint32 flg);
/* Sets a game flag and returns its old state, as syGameFlagSet. */
Sint32 syGameFlagSetStub(Sint32 flg);
/* Clears the flag, returning nothing where syGameFlagSetStub returns its
   byte. */
void syGameFlagClearStub(Sint32 flg);

/* The system library's own functions and globals, by the part each belongs to:
   the tick, random numbers, events, sequences and overlays, sprites, matrices,
   fades, resources, windows, the debug text and the link cable. */

/* The tick and the console: waiting, interrupts, sleep, the cartridge's
   language and its build stamp. */
void syWaitForVBlankInterrupt(void);
void syVBlankHandler(void);
/* The Game Pak interrupt's handler, in IWRAM: stops the console for good once
   the cartridge is pulled (the BIOS's Stop, again after any wake). */
void syHalt(void);
/* The BIOS's SoundBias: Off ramps the sound output's bias down before the
   console sleeps, On back up once it wakes, so the speaker does not click. */
void BiosSoundBiasOff(void);
void BiosSoundBiasOn(void);
/* 1 while the CPU is stopped for sleep, until L and R wake it. */
extern Uint16 SleepActive;
/* The cartridge's first byte, as the link places it (rom_header.s). */
extern const Uint8 RomStart[];

/* The cartridge's language, from its header (syGetLanguage). */
typedef enum CARTRIDGE_LANGUAGE {
	CARTRIDGE_LANGUAGE_ENGLISH = 1,
	CARTRIDGE_LANGUAGE_GERMAN,
	CARTRIDGE_LANGUAGE_FRENCH,
	CARTRIDGE_LANGUAGE_ITALIAN,
	CARTRIDGE_LANGUAGE_SPANISH
} CARTRIDGE_LANGUAGE;

Sint32 syGetLanguage(void);

/* Random numbers. A random number from 0 to n - 1: n times a 16-bit random
   draw, over 65536. */
#define syRandomBelow(n) (syRandom() * (n) >> 16)
/* syRandom's seed; Lucky Wheels seeds it with the play time, and a link
   battle's parent sends it as the turn's BattleRandomSeed. */
extern Uint32 RandomSeed;

/* The event passes and the queued register writes they flush. An event's
   function as the event passes call it: returning a value, where the event
   itself returns none (syExecuteEvent's UB). */
typedef Sint32 (*EVENT_INT_FUNC)(void);
struct IO_WRITE_QUEUE;
/* The function that carries out `count` queued register writes
   (ResourceExecuteTransferCode, run from a copy on the stack). */
typedef void (*QUEUE_FLUSH_ROUTINE)(IO_WRITE_QUEUE *queue, Uint32 count);
/* The function that runs the queued register writes, which
   syIOWriteQueueFlushPending copies onto its stack and runs. */
extern Uint8 ResourceExecuteTransferCode[];
/* The size of ResourceExecuteTransferCode. */
extern Uint8 ResourceExecuteTransferSize[];

/* The sequences the main routine runs, how the game starts, and the overlays it
   loads. Sequence ids above the scene table's rows, as The Broken Seal's MODE_
   values: the sequences other than the field mode, which gmMainRoutineRun runs
   in place of a scene. */
enum {
	/* The Super Lucky Dice sequence (fdSuperLuckyDiceRun); Contigo's dealer
	   leaves for it. The Broken Seal's 507, MODE_NONE, runs nothing. */
	MODE_SUPER_DICE = 507,
	MODE_SLOTS = 508, /* the slots sequence (fdLuckyWheelsRun) */
	MODE_DICE = 509,  /* the dice sequence (fdLuckyDiceRun) */
	MODE_BATTLE = 510 /* a battle; the entrance is the formation */
};

/* gmMainRoutineRun's mode: where the game starts. Only GAME_START_TITLE is
   passed (syInit, gmRestartQuest); the others are its debug starts. */
typedef enum GAME_START {
	GAME_START_TITLE = 0, /* the Title, with the state cleared */
	GAME_START_DEBUG_VENUS_LIGHTHOUSE = 1, /* SCENE_VENUS_LIGHTHOUSE_ENTRANCE */
	GAME_START_DEBUG_CLEAR = 2,			   /* SCENE_CLEAR, as from the title */
	GAME_START_DEBUG_MT_ALEPH = 3,		   /* SCENE_MT_ALEPH_SUMMIT */
	GAME_START_DEBUG_TITLE = 4 /* the story crawl (TITLE_STORY_CRAWL) */
} GAME_START;

Sint32 btRunStub(Sint32 entrance);
/* Plays Lucky Wheels (fdLuckyWheelsStart). */
Sint32 fdLuckyWheelsStartStub(Sint32 entrance);
Sint32 fdSuperLuckyDiceStartStub(Sint32 entrance);
void   fdLuckyDiceRun(void);
void   gmArrive(Sint32 unused, Sint32 keep);
void   gmLoadOverlay(Sint32 unused);
/* The ARM function that resolves a loaded overlay's long branches, run from a
   heap copy by syOverlayLoad. */
extern const Uint8 syResourcePatchThumbBranch[];
/* The size of syResourcePatchThumbBranch (decompress_tagged_lz.s). */
extern Uint8 syResourcePatchThumbBranchCodeSize[];
/* The compiler library's call through r3, which the IWRAM-resident code's
   callers use to run code copied to RAM. */
Sint32 _call_via_r3(Sint32, Uint8 *, Uint8, Sint32);

/* Sprites: their depth slots, affine matrices, VRAM and colours. */
void syOAMResetTick(void);
/* Queues a sprite at a depth slot of this tick's OAM list (the OAM_DEPTH_
   slots below). A unit whose ROM code is compiled against another form defines
   OWN_syOAMAddSprAtDepth and declares that form itself. */
#ifndef OWN_syOAMAddSprAtDepth
void syOAMAddSprAtDepth(void *entry, Sint32 slot);
#endif
/* Builds this tick's OAM list in buffer, a scratch block of OAM_SIZE bytes,
   running its ARM function from a copy on the stack. */
void syRenderBuildOAMList(void *buf);
/* Which of the 256 depth slots of OamDepthLists hold sprites this tick, a bit
   a slot; syOAMResetTick clears it. */
extern Uint8 OamDepthMasks[];
/* The 256 depth slots' list heads that every drawn sprite is linked into
   (syOAMAddSprAtDepth); syOAMResetTick empties them each tick. */
extern void *OamDepthLists[];
/* Depth slots of syOAMAddSprAtDepth: a higher slot is drawn in front. The
   windows' sprites, icons and cursors take the slots from OAM_DEPTH_WINDOW up,
   in front of the field engine's and the battle engine's, and OAM_DEPTH_FRONT
   is in front of everything. */
#define OAM_DEPTH_WINDOW		  240
#define OAM_DEPTH_FRONT			  255
/* The depth slot the field Psynergy's effect sprites take: in front of every
   actor, whose slots are clamped below OAM_DEPTH_WINDOW, and among the window
   sprites' slots. */
#define EFFECT_SPRITE_FRONT_DEPTH 250
struct SPR_ANIMATION;
struct SPR_ANIMATION *fdSprAnimationAlloc(Sint32 id);
Sint32				  fdSprPictureCacheFindByTag(Sint32 key);
/* Five tables that map a picture's 256 colour indices onto a sprite's palette,
   one for each palette a field sprite can take
   (field/sprite_color_tables.c). */
extern Uint8 SpriteColorTables[][256];
Sint32		 syPaletteGetBasicSprCols(void);
/* Two 16-entry colour maps for 4bpp pixels: the first draws colour 1 as colour
   10, the second leaves every colour as it is. */
extern Uint16 GraphicsExpandNibbleTable[];
Sint32		  syVRAMCacheAllocateBlocks(Uint32 id, Uint32 size);
Sint32		  syVRAMCacheFreeBlocks(Sint32 id);
Sint32		  syVRAMCacheAllocEntry(Sint32 size);
Sint32		  syVRAMCacheReload(Sint32 id, Sint32 src);

/* Matrices and the projection. */
void syMatrixSetRotateY(ANGLE ang);
/* The basic sprite palettes, loaded for the debug text
   (syPaletteGetBasicSprCols). */
extern const Uint8 SystemBasicColorPalette[];

void syTranslate(FIXED x, FIXED y, FIXED z);
/* Sets the current matrix's translation and keeps its rotation. */
void syMatrixSetTranslation(const POINT pos);
void syMatrixScaleUniform(FIXED scale);
void syScale(FIXED x, FIXED y, FIXED z);
/* The far depth well past the battlefield: the largest whole 16.16 number. */
#define BATTLE_FAR_DEPTH toFIXED(32767)
/* The dot product of (ax, ay, az) and (bx, by, bz), in 16.16
   (RelocatedInnerProduct, run from a copy on the stack). */
typedef Sint32 (*DOT_PRODUCT_ROUTINE)(
	Sint32 ax, Sint32 bx, Sint32 ay, Sint32 by, Sint32 az, Sint32 bz);

/* Screen fades. */
void syScreenFadeUpdate(void);

/* Resources and decompression. Each resource's ROM address by resource id
   (syResourceGetTableEntry). */
extern void *syResourceDirectoryTable[];
void		 syDecompressHalfwords(const void *src, void *dst);
/* The halfword LZ decoder that syDecompressHalfwords runs from its stack. */
extern const Uint8 syDecompressHalfwordLZ[];
/* The size of syDecompressHalfwordLZ, copied onto the stack by
   syDecompressHalfwords. */
extern Uint8 syDecompressHalfwordsCodeSize[];
Sint32		 syDecompressMoveToFrontLZ(const void *src, void *dst);
Sint32		 mnAbilityIconLoadIntoSlot(Sint32 resource, Sint32 idx);

/* Windows and their text. The window font's 16 colours, loaded as BG palette 15
   (syWindowLoadGraphics). */
extern const Uint16 WindowPalette[];
/* The party status window's layout (PARTY_STAT_WINDOW), the block of heap slot
   HEAP_SLOT_PARTY_STATUS. */
extern Uint8 PartyStatusState[];
/* The link battle timer's heading: `TIME`; `TMPO.` in Spanish, `TEMPS` in
   French. */
extern Uint8 LinkTimeLabelString[];

/* The debug text and the number strings it prints. Nonzero lets syPrint write;
   the power-on set-up and the credits clear it. */
extern Uint8 DebugTextEnabled;
/* Where the debug text writes next on its screen in BG0. */
extern Uint16 *DebugTextCursor;
/* The digits syStringFormatHex and syStringFormatDecimal write, right-aligned
   up to NumberTextHexEnd and NumberTextDecimalEnd. */
extern Uint8 NumberTextBuffer[];
/* Writes a value to NumberTextBuffer right-aligned: spaces, a sign and its
   decimal digits. */
void syStringFormatDecimal(Sint32);
void syStringFormatHex(Uint32);
/* The hexadecimal digits 0 to F, which syStringFormatHex writes from. */
extern const Uint8 StringHexDigits[];
/* The end of syStringFormatDecimal's digits: syPrintDec prints the last places
   before it. */
extern Uint8 NumberTextDecimalEnd[];
/* The end of syStringFormatHex's digits: syPrintHex prints the last places
   before it. */
extern Uint8 NumberTextHexEnd[];

/* The link cable. A halfword in a record of its own, which
   sySerialStepBlockTransfer keeps its constant 1 in. */
typedef struct SERIAL_HALFWORD {
	Uint16 value;
} SERIAL_HALFWORD;

/* Declarations other modules' code shares. */
struct BATTLE_OBJECT;
struct ACTOR;
/* An empty function, reached only through an unnamed stub in the object
   module's stub table. */
void   fdActorNoOperation(void);
Sint32 fdTilesetAnimResumeAllStub(void);
Sint32 fdTilesetAnimSuspendAllStub(void);

/* The system library's groups. */

/* The affine group. Sprite rotation and scaling: a sprite's transform and the
   OAM matrices this tick hands out for it. */
Sint32 syAffineMatrixBuild(Sint32 ang, Sint32 xScale, Sint32 yScale);

/* The dma group. DMA and the BIOS copy service: fills, and one transfer on a
   channel. The BIOS CpuSet service (bios.s). */
void BiosCpuSet(const void *src, void *dst, Uint32 control);

/* Stops DMA 0, which a scene may have left copying a line each HBlank: drops
   its start timing and repeat, then its enable, and reads the control back. */
#define DMA0_STOP() \
	{ \
		volatile Uint16 *dma0 = REG_DMA0; \
\
		dma0[DMA0_CONTROL] &= ~DMA_START_TIMING_DRQ_REPEAT; \
		dma0[DMA0_CONTROL] &= ~DMA_ENABLE; \
		dma0[DMA0_CONTROL]; \
	}

/* Fills `size` bytes at `destination` with the halfword `value` by DMA3, from a
   volatile copy of it on the stack. */
#define DMA_FILL16(value, destination, size) \
	{ \
		volatile Uint16 fill_ = (value); \
		syDMAXCopy((const void *)&fill_, (destination), \
			DMA_FILL_HALFWORDS((size) / 2), REG_DMA3); \
	}

/* Fills `size` bytes at `destination` with the word `value` by DMA3. */
#define DMA_FILL32(value, destination, size) \
	{ \
		volatile Uint32 fill_ = (value); \
		syDMAXCopy((const void *)&fill_, (destination), \
			DMA_FILL_WORDS((size) / 4), REG_DMA3); \
	}

/* DMA_FILL32 through `cell`, a word the caller declares, so that a function
   filling several blocks sets one word before each (fdActorSystemInit's two
   fills). */
#define DMA_FILL32_CELL(cell, value, destination, size) \
	{ \
		(cell) = (value); \
		syDMAXCopy((const void *)&(cell), (destination), \
			DMA_FILL_WORDS((size) / 4), REG_DMA3); \
	}

/* pret's CpuFill32: repeats one word through the BIOS CpuSet service. */
#define CPU_FILL32(value, destination, size) \
	{ \
		volatile Uint32 fill_ = (value); \
		BiosCpuSet((const void *)&fill_, (destination), \
			CPU_SET_32BIT | CPU_SET_SRC_FIXED | ((size) / 4)); \
	}

/* Starts a transfer on DMA channel `channel`: its source, destination and
   control written in one store. */
static __inline__ void syDMAXCopy(
	const void *src, void *dst, Uint32 control, volatile Uint32 *channel)
{
	ASM_REGISTER(volatile Uint32 *dma, "r3") = channel;
	ASM_REGISTER(Uint32 srcReg, "r0") = (Uint32)src;
	ASM_REGISTER(Uint32 dstReg, "r1") = (Uint32)dst;
	ASM_REGISTER(Uint32 cnt, "r2") = control;
	ASM_DMA_STORE(srcReg, cnt, dma, dstReg);
}

/* The tick group. The tick loop's globals: the frames a tick takes, the CPU
   load display and the VBlank flags, which sySynch() (system/system_frame.c)
   and the VBlank handler share. Stops the sound DMA and its timer. */
void sySndSuspendDirectSndStub(void);
/* The resident mixer in IWRAM, which the tick wait runs after
   SoundDriverEnterVBlankUpdate returns 0. It never reads its argument; the
   callers pass 8. */
void IwramSoundRenderVBlank(Sint32) __attribute__((long_call));

/* The heap_release group. The heap's allocators and releases
   (system/system_heap.c). */
void syHeapFillFreeWords(Uint32 value);

/* A block of size bytes from the allocator given (syHeapAlloc or
   syHeapAllocEWRAM), with no heap slot. */
static __inline__ void *syHeapCallAlternatePool(
	void *(*allocate)(Sint32), Sint32 size)
{
	return allocate(size);
}

/* The input group. The keypad state the VBlank interrupt keeps.

   The VBlank handler reads the keypad into PerWork, and
   syInputUpdateKeyRepeatAndDirection turns it into repeats after each tick
   (system/system_frame.c). data, push, repeat and repeatTmr are volatile
   because the VBlank handler sets them while the code it interrupts reads them;
   direction and prevDir the key repeat rebuilds and reads each tick. */
void syInputUpdateKeyRepeatAndDirection(void);
/* The keypad interrupt's handler. */
void syInputHandleKeyInterrupt(void);

/* The io_write group. The register writes and reads the whole game makes
   through a local. Writes a value to a 16-bit register or memory location,
   through a local of its own. */
#define syIOWrite16(v, reg) \
	do { \
		Uint32 value_ = (v); \
		*(reg) = value_; \
	} while(0)

/* Writes a value to a 32-bit register or memory location, in syIOWrite16's
   shape. */
#define syIOWrite32(v, reg) syIOWrite16(v, reg)

/* Writes a value to an 8-bit register or memory location, in syIOWrite16's
   shape. */
#define syIOWrite8(v, reg) syIOWrite16(v, reg)

/* Reads a 16-bit register or memory location into the variable at `variable`:
   the battle engine's mosaic fade saves its background controls so, and Gaia
   Rock's slate and Anemos Inner Sanctum's symbol their blend levels. */
#define syIORead16(reg, variable) (*(variable) = *(reg))

/* The irq_lock group. Turning interrupts off around a change the interrupt
   handlers also make, and back on as they were. Saves IME through `ime`, a
   pointer to REG_IME, into `saved`. */
#define IME_SAVE(ime, saved) \
	do { \
		(saved) = *(ime); \
	} while(0)

/* Disables interrupts by writing the pointer's own low halfword through it:
   REG_IME's address ends in 0x208, whose bit 0 is clear. */
#define IME_DISABLE(ime) \
	do { \
		*(ime) = (Uint16)(Uint32)(ime); \
	} while(0)

/* Restores IME from saved. */
#define IME_RESTORE(ime, saved) \
	do { \
		*(ime) = (saved); \
	} while(0)

/* Disables interrupts and saves IME in saved; IRQ_UNLOCK restores it. */
#define IRQ_LOCK(saved) \
	do { \
		volatile Uint16 *ime_; \
\
		IME_SAVE((ime_ = &REG_IME), saved); \
		IME_DISABLE(ime_); \
	} while(0)

/* Restores the IME value saved by IRQ_LOCK. */
#define IRQ_UNLOCK(saved) \
	do { \
		REG_IME = (saved); \
	} while(0)

/* Disables interrupts, keeping IME in a variable of the caller's that an
   interrupt may read. */
static inline void syIRQLock(volatile Uint32 *saved)
{
	Uint16 *ime;

	IME_SAVE((ime = (Uint16 *)&REG_IME), *saved);
	IME_DISABLE(ime);
}

/* Restores the IME that syIRQLock kept. */
static inline void syIRQUnlock(volatile Uint32 *saved)
{
	Uint32 value = *saved;

	REG_IME = value;
}

/* The io_write_queue group. The I/O write queue: register writes held until
   VBlank. */
void syIOWriteQueueFlushPending(void);

/* Queues one register write with interrupts off; a write to a full queue is
   dropped. `control` is an IO_WRITE_ width, with IO_WRITE_SET or
   IO_WRITE_CLEAR, to write `value` to `address`; or a DMA control word, to copy
   by DMA 3 from `value` to `address`. */
#define IO_WRITE_QUEUE_PUSH(address, value, control) \
	{ \
		IO_WRITE_QUEUE *q = &IoWriteQueueWork; \
		Uint32			saved; \
		Sint32			cnt; \
\
		IRQ_LOCK(saved); \
		cnt = *&q->cnt; \
		if(cnt < IO_WRITE_QUEUE_SIZE) { \
			Uint32 *entry = q->entries[cnt]; \
\
			*&q->cnt = cnt + 1; \
			*entry++ = (value); \
			*entry++ = (address); \
			*entry = (control); \
		} \
		IRQ_UNLOCK(saved); \
	}

/* Like IO_WRITE_QUEUE_PUSH, but keeps IME's address in the caller's ime
   variable while disabling and restoring interrupts. */
#define IO_WRITE_QUEUE_PUSH_IME(ime, address, value, control) \
	{ \
		IO_WRITE_QUEUE *q; \
		Uint32			saved; \
		Sint32			cnt; \
\
		q = &IoWriteQueueWork; \
		IME_SAVE(ime, saved); \
		IME_DISABLE(ime); \
		cnt = *&q->cnt; \
		if(cnt < IO_WRITE_QUEUE_SIZE) { \
			Uint32 *entry = q->entries[cnt]; \
\
			*&q->cnt = cnt + 1; \
			*entry++ = (value); \
			*entry++ = (address); \
			*entry = (control); \
		} \
		IME_RESTORE(ime, saved); \
	}

/* The iwram_call group. The functions resident in IWRAM, called at their fixed
   addresses: the fixed-point multiply and divides, the square root, word fills
   and copies, the matrix multiply and the picture decoder. The 16.16 product of
   two 16.16 numbers (syMulFX), written as a call. */
static inline FIXED syMathMulFX(FIXED a, FIXED b)
{
	return syMulFX(a, b);
}

/* value * toFIXED(1) / divisor: the 16.16 ratio of two 16.16 numbers. */
static inline FIXED syDivFX(FIXED divisor, FIXED value)
{
	return syFixedDivEntry(divisor, value);
}

/* The integer square root, rounded down, of a value from 0; 0 for a negative
   one. A 16.16 square comes back as 8.8. */
static inline Sint32 sySquartIWRAM(Sint32 value)
{
	return sySquartEntry(value);
}

/* Multiplies matrix, a 3 by 3 matrix and a translation in 16.16 fixed point,
   into the current transform. */
static inline void syMultiMatrix(const MATRIX matrix)
{
	syMultiMatrixEntry(matrix);
}

/* Writes to result the 16.16 point vector moved by the current matrix: turned
   by its 3 by 3 part, then offset by its translation. */
static inline Sint32 syCalcPoint(const POINT vector, POINT result)
{
	return syVectorTransformEntry(vector, result);
}

/* The quotient, rounded toward zero. */
static inline Sint32 syIntegerDivide(Sint32 dividend, Sint32 divisor)
{
	return syIntegerDivideEntry(dividend, divisor);
}

/* The unsigned quotient, rounded down. */
static inline Uint32 syIntegerDivideUnsigned(Uint32 dividend, Uint32 divisor)
{
	return syIntegerDivideUnsignedEntry(dividend, divisor);
}

/* Unpacks a PICTURE_FORMAT_COMPRESSED picture into destination, a byte above
   223 leaving that many less 223 bytes there untouched, and returns NULL; one
   whose first halfword is 0 is not packed, and comes back as the data after
   that halfword for the caller to decode. */
static inline void *syDecompressSprPicture(const void *src, void *dst)
{
	return syDecompressSprPictureEntry(src, dst);
}

/* Clears size bytes, rounded down to whole words, from destination. Returns the
   address after the last word. */
static inline Sint32 syWordsClear(void *dst, Sint32 size)
{
	return syWordsClearEntry(dst, size);
}

/* As syWordsClear, filling with value. */
static inline Sint32 syWordsFill(void *dst, Sint32 size, Uint32 value)
{
	return syWordsFillEntry(dst, size, value);
}

/* Copies size bytes, rounded down to whole words, from source to
   destination. */
static inline void syWordsCopy(void *dst, const void *src, Sint32 size)
{
	syWordsCopyEntry(dst, src, size);
}

/* syWordsFill as a statement of its own, as twelve fills in the battle engine,
   its effects and the field engine's Psynergy are written: among them the
   battle camera's and the transition's curtain tiles, which The Broken Seal
   fills by DMA with DMA_FILL32_CELL. */
#define IWRAM_FILL_WORDS(destination, size, value) \
	do { \
		syWordsFill(destination, size, value); \
	} while(0)

/* syWordsCopy as a statement of its own, as IWRAM_FILL_WORDS is: the link code
   copies the Djinn recovery table into its send buffer so, in the lobby and in
   the battle, as The Broken Seal's does. */
#define IWRAM_COPY_WORDS(destination, source, size) \
	do { \
		syWordsCopy(destination, source, size); \
	} while(0)

/* The IWRAM function behind syMulFX, called by its linked name. */
FIXED syIWRAMMulFX(FIXED a, FIXED b) __attribute__((long_call));
/* The compiler library's signed division and remainder, which the overlays
   import from the IWRAM functions (system/iwram_entry.s). */
Sint32 __divsi3(Sint32 numerator, Sint32 denominator);
Sint32 __modsi3(Sint32 numerator, Sint32 denominator);

/* The matrix group. The current 3D matrix, its one-deep stack, and the unit
   matrix. */
void syMatrixRewindStack(void);
void syGetMatrix(MATRIX dst);
void syLoadMatrix(const MATRIX src);
void syMatrixBuildLookAt(POINT eye, POINT tgt, MATRIX out);

/* Sets a twelve-word matrix to the unit matrix: toFIXED(1) (1.0) on the
   rotation's diagonal, the rest 0. */
static __inline__ void syUnitMatrix(MATRIX matrix)
{
	ASM_MATRIX_UNIT(matrix);
}

#include "system/mul_fx.h"

/* The ram_code group. Copying an ARM function to RAM to run it there: the
   decoders, the overlay loader's branch patcher and the battle effects' canvas
   functions. Copies the function `code`, `bytes` long (its linked size), into a
   block taken with syHeapAlloc and points `to` at it; syHeapRelease frees
   it. */
#define RAM_CODE_LOAD(to, code, bytes) \
	do { \
		Uint32 size_; \
\
		size_ = (Uint32)(bytes); \
\
		(to) = syHeapAlloc(size_); \
		syDMAXCopy( \
			(const void *)(code), (to), DMA_COPY_WORDS(size_ >> 2), REG_DMA3); \
	} while(0)

/* As RAM_CODE_LOAD, into heap slot `slot`'s block; syHeapSlotRelease frees
   it. */
#define RAM_CODE_LOAD_SLOT(slot, code, bytes) \
	do { \
		Uint32 size_; \
\
		size_ = (Uint32)(bytes); \
\
		syDMAXCopy((const void *)(code), syHeapSlotAlloc((slot), size_), \
			DMA_COPY_WORDS(size_ >> 2), REG_DMA3); \
	} while(0)

/* The colour buffer functions (system/decompress_byte_lz.s), each of which
   copies its ARM kernel onto the stack by DMA and runs it there. Copies `size`
   bytes of colour levels, halving each. */
void syColBufferHalve(void *src, void *dst, Sint32 size);
/* Copies `size` bytes of colour levels at three quarters of each, returning
   what the kernel leaves in r0. */
Sint32 syColBufferScaleThreeQuarters(void *src, void *dst, Sint32 size);
/* Copies `size` bytes of colour levels, each lowered by `amount`. */
void syColBufferDarken(void *src, Sint32 amount, void *dst, Sint32 size);
/* Copies `size` bytes of colour levels, each raised by `amount`. */
void syColBufferBrighten(void *src, Sint32 amount, void *dst, Sint32 size);
/* Copies size bytes of 8-bit pixels from source to destination, then halves
   each colour number left in source, so a drawing fades as it is shown: Lucky
   Wheels' trails (CANVAS_FADE_HALF). */
void syColBufferHalveNonzero(Uint8 *src, Uint8 *dst, Sint32 size);
/* syColBufferHalveNonzero, taking each colour number to three quarters, for the
   trails that fade more slowly. */
void syColBufferScaleNonzeroThreeQuarters(Uint8 *src, Uint8 *dst, Sint32 size);
/* The high word of value times magic, plus value times multiplier: a division
   by a constant (system/trig.s). */
Sint32 syMathUnsignedMulHighAdd(Sint32 value, Uint32 magic, Sint32 multiplier);

/* The save group. The save in flash: its sectors, parts and summaries, and the
   save's state. */
Uint32 syBupFindLatestSector(Sint32 partId);
Uint32 syBupReadPart(Sint32 partId, void *dst);
Uint32 syBupDeletePart(Sint32 partId);

/* The save in flash. */
Sint32 syBupCompareBytes(Uint8 *left, Uint8 *right, Sint32 cnt);
Sint32 syBupChecksumSector(void);
Sint32 syBupReadSector(Sint32 idx);
Uint32 syBupWriteSector(Sint32 sector);
Bool   syBupInvalidateSector(Sint32 idx);
Uint32 syBupSelectWriteSector(Sint32 partId);
Uint32 syBupGetLatestCounter(Sint32 partId);
void   syBupSetProgressCallback(void (*progress)(Sint32 step));
Sint32 syBupWritePart(Sint32 partId, void *src);
Bool   syBupInitState(void);
Sint32 syBupLoadFileSummaries(void);
void   syBupReleaseState(void);

/* The scene group. The running scene's overlay area, a field position, and the
   area name a scene and entrance show. */
Sint32 fdAreaGetNameIndexStub(Sint32 scene, Sint32 entrance);

/* The serial group. The link cable's runtime: serial packets, registers and
   transfer state. */

/* Starts a DMA 3 transfer; control_ is the unit count with the DMA_ flags <<
   16. */
#define START_DMA(source_, destination_, control_) \
	do { \
		DMA3->src = (Uint32)(source_); \
		DMA3->dst = (Uint32)(destination_); \
		DMA3->ctrl = (control_); \
	} while(0)

Sint32 sySerialPollStat(void);
Sint32 sySerialExchangePayloads(void *send, void *receive);
void   sySerialPrepareSendPacket(void *payload);
Uint8  sySerialCollectReceivedPayloads(void *payload);
void   sySerialEnableTransferTimer(void);
void   sySerialDisableTransfer(void);
void   syLinkBattleResetTransferState(void);
void   sySerialHandleTransferInterrupt(void);

#include "system/vector3.h"

#endif
