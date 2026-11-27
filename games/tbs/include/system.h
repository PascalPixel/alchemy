#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

// The system library's one header: its types, its functions and its macros. A file that uses the
// system library includes this and nothing else of it, as a Saturn program includes SGL's sgl.h.
// The name is the staff roll's: under Programming, its first heading is `System`.
//
// As SGL's sgl.h takes its types from sl_def.h, this takes the system library's types and constants
// from system/definitions.h and its variables from system/globals.h, and keeps the functions: the
// stub table's prototypes, then each group's inlines and macros in turn.

#include "types.h"
#include "fixed_math.h"
#include "edition.h"
#include "system/asm.h"
#include "system/definitions.h"
#include "system/globals.h"

// The heap_release group. Releasing heap blocks, and the two pools' space. Takes `size` bytes from
// another pool through its allocator, such as syHeapAllocateEWRAM.
static __inline__ void *syHeapCallAlternatePool(void *(*allocate)(Sint32), Sint32 size)
{
    return allocate(size);
}

struct AFFINE_TRANSFORM;

// The system library's functions in the order of its stub table (src/system/stubs.s).
void   sySynch(Sint32 ticks);
void   syInitEvent(void);
Sint32 sySetEvent(EVENT_FUNC callback, Sint32 priority);
Sint32 syCloseEvent(EVENT_FUNC callback);
Sint32 sySuspendEvent(EVENT_FUNC callback);
Sint32 syResumeEvent(EVENT_FUNC callback);
void   syExecuteEvent(Sint32 pass);
Uint32 syRandom(void);
ANGLE  syAtan(FIXED x, FIXED y);
Sint32 sySquart(Sint32 value);
FIXED  sySquartFX(FIXED value);
// The sine of `angle`, a whole turn being TRIG_TURN, in 16.16, read from TrigSineTable, 256 entries
// for the first quarter turn.
FIXED  sySin(ANGLE angle);
// The cosine of `angle`, a whole turn being TRIG_TURN, in 16.16; it falls through into sySin a
// quarter turn on.
FIXED  syCos(ANGLE angle);
void   syVectorAddPolarOffset(FIXED radius, ANGLE angle, POINT pos);
void   syIntFunction(Uint32 irq, Sint32 vcount, INTERRUPT_HANDLER handler);
void   syHeapInit(void);
void  *syHeapSlotAllocate(Sint32 slot, Sint32 size);
void  *syHeapSlotAllocateEWRAM(Sint32 slot, Sint32 size);
// Releases heap slot `slot`'s block, and with it every later block of the same pool, by setting the
// pool's next free address back to the block's start.
void   syHeapSlotRelease(Sint32 slot);
Sint32 syHeapGetRemainingIWRAM(void);
Sint32 syHeapGetRemainingEWRAM(void);
void  *syHeapAllocate(Sint32 size);
void  *syHeapAllocateEWRAM(Sint32 size);
// Releases `buf`, a block taken with no heap slot, and every later block of its pool, as
// syHeapSlotRelease does.
void   syHeapRelease(void *buf);
void   syPrint(const Uint8 *src);
void   syPrintHex(Uint32 value, Sint32 digits);
void   syPrintDecimal(Sint32 value, Sint32 digits);
void   syPrintBlanks(Uint32 count);
Sint32 syDecompressByteLZ(const void *src, void *dest);
Sint32 syDecompressTaggedLZ(const void *src, void *dest);
void   syVRAMCacheReset(void);
Sint32 syVRAMCacheFreeEntry(Uint32 id);
Sint32 syVRAMCacheReleaseVRAM(Uint32 id);
Sint32 syVRAMCacheLoad(Uint32 id, Uint32 size, const void *src);
Sint32 syVRAMCacheFindFreeEntry(void);
void   syPrintClear(void);
Sint32 syOAMMatrixAllocateFromTransform(AFFINE_TRANSFORM *src);
void   syOAMAddSpriteAtDepth(void *slotEntry, Sint32 slot);
void   syWindowLoadGraphics(void);
void   syPaletteLoadBasicSpriteColors(void);
void   syInitMatrix(void);
void   syUnitMatrixCurrent(void);
void   syPushMatrix(void);
void   syPopMatrix(void);
void   syMatrixRotate(Sint32 *angles);
void   syRotX(ANGLE angle);
void   syRotY(ANGLE angle);
void   syRotZ(ANGLE angle);
void   syMatrixTranslateVector(const POINT pos);
void   syMatrixScaleVector(const VECTOR scale);
void   syMatrixRotateTranslate(Sint32 *angles, POINT pos);
void   syMatrixRotateTranslateScale(Sint32 *angles, POINT pos, VECTOR scale);
void   syLookAtLoad(POINT eye, POINT target);
Sint32 syConvert3Dto2D(const POINT point, Sint32 *screen);
void   syPerspective(Uint32 fieldOfView, FIXED nearDepth, FIXED farDepth);
void   sySetScreenDist(Uint32 focal, Uint32 nearDepth, Uint32 farDepth);
void   syCopyBuildStamp(void);
Sint32 syDummyReturnZero(Sint32 resourceId, const Uint8 *table);
void  *syResourceGetTableEntry(Sint32 index);
void   syDummyNoOperationHook(Sint32 unused);
void   syOverlayLoad(Uint32 index, void *dest);
void   syScreenFadeToBlack(Sint32 frames);
void   syScreenFadeFromBlack(Sint32 frames);
void   syScreenFadeToWhite(Sint32 frames);
void   syScreenFadeFromWhite(Sint32 frames);
void   syScreenFadeStart(Sint8 bright, Sint32 layer, Uint32 start, Sint32 target, Sint32 frames);
void   syScreenFadeWait(void);
void   syDummyReservedNoOperationA(void);
void   syDummyReservedNoOperationB(void);
void   syDummyReservedNoOperationC(void);
void   syDummyReservedNoOperationD(void);
void   syDummyReservedNoOperationE(void);
void   sySerialInit(void);
Sint32 sySerialPollAndTransfer(void *send, void *receive);
// Queue one register write of a width (8, 16 or 32 bits): a store, or the value's bits set or
// cleared (system/system_io_write_queue.c).
void   syIOWriteQueueStore8(Uint32 addr, Uint32 value);
void   syIOWriteQueueStore16(Uint32 addr, Uint32 value);
void   syIOWriteQueueStore32(Uint32 addr, Uint32 value);
void   syIOWriteQueueSetBits8(Uint32 addr, Uint32 value);
void   syIOWriteQueueSetBits16(Uint32 addr, Uint32 value);
void   syIOWriteQueueSetBits32(Uint32 addr, Uint32 value);
void   syIOWriteQueueClearBits8(Uint32 addr, Uint32 value);
void   syIOWriteQueueClearBits16(Uint32 addr, Uint32 value);
void   syIOWriteQueueClearBits32(Uint32 addr, Uint32 value);
void   syInputResetKeyRepeat(void);
void   syLookAt(POINT eye, POINT target);
Uint32 sySerialWaitForStatusMask(Sint32 mask);
void   sySerialRemoveIRQHandlers(void);
Sint32 sySoundLoadReverbCode(Sint32 code);
Sint32 sySerialStartSend(void *src, Sint32 size);
Sint32 sySerialStartReceive(void *dest);
void   sySerialWaitForSend(void);
void   sySerialWaitForReceive(void);
void   sySerialWaitForTransfers(void);
Sint32 sySerialGetActiveTransfers(void);
void   sySerialStepBlockTransfer(void);
Sint32 sySetEventFlags(EVENT_FUNC callback, Uint32 mask);

// The system library's functions and globals that the stub table does not list.

// The system library's own functions and globals: interrupts, BIOS sound bias, random numbers and
// sleep.
void   BiosSoundBiasOff(void);
void   BiosSoundBiasOn(void);
// 1 while the CPU is stopped for sleep, until L and R wake it.
extern Uint16       SleepActive;

// NumberBuffer + 10, the decimal form's units digit (the form ends at + 11); syPrintDecimal starts
// n characters before it, so it prints n + 1.
extern Uint8        NumberBufferDecimalEnd[];
// The end of the hexadecimal form in NumberBuffer.
extern Uint8        NumberBufferHexEnd[];
// The link battle timer's heading: `TIME`; `TMPO.` in Spanish, `TEMPS` in French.
extern const Uint8  LinkTimeLabelString[];
// Each resource's ROM address by resource id (syResourceGetTableEntry).
extern void        *syResourceDirectoryTable[];
// The size of syResourcePatchThumbBranch (decompress_tagged_lz.s).
extern Uint8        syResourcePatchThumbBranchCodeSize[];
// The window font's 16 colours, loaded as BG palette 15 (syWindowLoadGraphics).
extern const Uint16 WindowPalette[];

struct SPR_ANIMATION;
struct SPR;

// The 152 bytes of IWRAM-resident sound code (0x03000bd8, not the mixer at 0x03007000) that
// sySoundLoadReverbCode overwrites with a reverb code from IwramSoundReverbCodes.
extern Uint8 IwramSoundReverbCode[];
// HEAP_SLOT_SPRITE_ANIMATIONS: the sprite animations (struct

// SPR_ANIMATION) that fdSpriteAnimationAllocate takes from.
#define SpriteAnimations ((struct SPR_ANIMATION **)&HeapSlots[HEAP_SLOT_SPRITE_ANIMATIONS])
// Where the debug text writes next on its 32 by 20 screen in BG0.
extern Uint16     *DebugTextCursor;
// Nonzero lets syPrint write; the power-on set-up and the credits clear it.
extern Uint8       DebugTextEnabled;
// The buffer a save file is built in and written from, in two halves; its summary's 16-byte header
// lies just before it.
extern Uint8       BupBuffer[];
// The current save file, or -1 for none (a new game).
extern Sint16      BupSlot;
// The five reverb codes' mixer code, SOUND_REVERB_CODE_SIZE bytes each.
extern Uint8       IwramSoundReverbCodes[];
// The size of syOAMBuildBufferKernel (decompress_byte_lz.s).
extern Uint8       syOAMBuildBufferKernelCodeSize[];
// The ARM OAM list builder, run from a heap copy by syOAMBuildBuffer.
extern const Uint8 syOAMBuildBufferKernel[];
// The ARM register-write executor, run from a stack copy by syIOWriteQueueFlushPending.
extern const Uint8 syResourceExecuteTransfer[];
// The ARM function that resolves a loaded overlay's long branches, run from a heap copy by
// syOverlayLoad.
extern const Uint8 syResourcePatchThumbBranch[];
// The game code's last letter in the cartridge header: `J` in Japanese, `E`, `D`, `F`, `S` or `I`
// (syGetLanguage).
extern const Uint8 RomLanguageCode;
// The basic sprite palettes 0 to 13, 224 colours (syPaletteLoadBasicSpriteColors).
extern const Uint8 BasicSpritePalettes[];
// The sprite tile uploader, run from HEAP_SLOT_PICTURE_UPLOADER.
extern Uint8       fdTileCopyStrided[];
// The size of fdTileCopyStrided (map_grid_convert.s).
extern Uint8       fdTileCopyStridedCodeSize[];
// The size of syResourceExecuteTransfer (decompress_tagged_lz.s).
extern Uint8       syResourceExecuteTransferCodeSize[];
// Stops the console for good with the BIOS Stop call: in the German, French, Spanish and Italian
// editions, what pulling the Game Pak out does.
void               syHalt(void);
// Runs the sound for this VBlank, the music players and then the game's own mixer in IWRAM; the
// last thing syVBlankHandler does.
void               MixerEnter(void);

void               syInit(void);
void               syIdleForever(void);

// The cartridge's language, from its header (syGetLanguage).
typedef enum CARTRIDGE_LANGUAGE
{
    CARTRIDGE_LANGUAGE_ENGLISH,
    CARTRIDGE_LANGUAGE_GERMAN,
    CARTRIDGE_LANGUAGE_FRENCH,
    CARTRIDGE_LANGUAGE_SPANISH,
    CARTRIDGE_LANGUAGE_ITALIAN
} CARTRIDGE_LANGUAGE;

Sint32 syGetLanguage(void);
void   syCodeResolveLongBranches(Uint16 *entries, Sint32 byteLength);
void   syIRQIgnore(void);
void   syInstallIWRAMAndIRQs(void);
void   syInputUpdateKeyRepeatAndDirection(void);
void   syVBlankHandler(void);
void   sySoundReservedNoOperationA(void);
void   sySoundReservedNoOperationB(void);
void   syInputHandleKeyInterrupt(void);
Sint32 syDecompressMoveToFrontLZ(const void *src, void *dest);

void   syIOWriteQueueFlushPending(void);

void   syScreenFadeUpdate(void);

// Depth slots of syOAMAddSpriteAtDepth: a higher slot is drawn in front. The windows' sprites,
// icons and cursors take the slots from OAM_DEPTH_WINDOW up, in front of the field's and the
// battle's, and OAM_DEPTH_FRONT is in front of everything.
#define OAM_DEPTH_WINDOW 240
#define OAM_DEPTH_FRONT  255

void   syOAMResetTick(void);
void   syOAMBuildBuffer(void *work);

Sint32 syVRAMCacheAllocateBlocks(Uint32 id, Uint32 size);
Sint32 syVRAMCacheGetLongestFreeRun(void);
Sint32 syVRAMCacheFreeBlocks(Sint32 id);
Sint32 syVRAMCacheAllocateEntry(Sint32 size);
Sint32 syVRAMCacheReload(Sint32 id, Sint32 src);

void   syCopyEventWords(Uint32 *dest, Uint32 *src, Uint32 byteCount);
void   sySortEvents(void);
Sint32 syFindEvent(EVENT_FUNC callback);
void   syEventNoOperationA(void);
void   syEventNoOperationB(void);
Sint32 sySuspendOverlayEvents(void);
Sint32 syResumeOverlayEvents(void);

// A random whole number from 0 to count - 1, from syRandom.
#define syRandom16Below(count) ((syRandom() * (count)) >> 16)

const Uint8 *syPaletteGetBasicSpriteColors(void);
void         syStringFormatHex(Uint32 value);
void         syStringFormatDecimal(Sint32 number);
void        *memcpy(void *dest, const void *src, unsigned long size);

void         syHeapReservedNoOperation(void);

void         syGetMatrix(MATRIX dest);
void         syLoadMatrix(const MATRIX src);
void         syMatrixBuildLookAt(POINT eye, POINT target, MATRIX out);
// The far depth well past everything drawn, the largest whole 16.16 number: the battle's, Thor's
// and Lucky Dice's projections.
#define PROJECTION_FAR_DEPTH_MAX toFIXED(32767)

void          syDecompressHalfwords(const void *src, void *dest);
void          syCanvasCopyAndBrighten(Uint8 *buf, Uint32 amount, Uint8 *dest, Uint32 bytes);
void          syCanvasCopyAndDarken(Uint8 *buf, Uint32 amount, Uint8 *dest, Uint32 bytes);
void          syCanvasCopyAndScaleThreeQuarters(Uint8 *buf, Uint8 *dest, Uint32 bytes);
void          syCanvasCopyAndHalve(Uint8 *buf, Uint8 *dest, Uint32 bytes);
void          syCanvasCopyAndBrightenPartial(Uint8 *buf, Uint32 amount, Uint8 *dest, Uint32 bytes);
void          syCanvasCopyAndDarkenPartial(Uint8 *buf, Uint32 amount, Uint8 *dest, Uint32 bytes);
void          syCanvasCopyAndScaleNonzeroThreeQuarters(Uint8 *buf, Uint8 *dest, Uint32 bytes);
void          syCanvasCopyAndHalveNonzero(Uint8 *buf, Uint8 *dest, Uint32 bytes);
Bool          syBupInitState(void);
Uint32        syBupSelectWriteSector(Sint32 partId);
Uint32        syBupWriteSector(Sint32 sector);
Sint32        syBupReadSector(Sint32 index);
Uint16        syBupEraseSector(Uint16 sector);
Bool          syBupWritePart(Sint32 partId, void *src);
Uint32        syBupReadPart(Sint32 partId, void *dest);
Uint32        syBupDeletePart(Sint32 partId);
Sint32        syBupChecksumSector(void);
Uint32        syBupFindLatestSector(Sint32 partId);
Bool          syBupInvalidateSector(Sint32 index);
Sint32        syBupCompareBytes(Uint8 *left, Uint8 *right, Sint32 count);
Uint32        syBupGetLatestCounter(Sint32 partId);
Sint32        syBupLoadFileSummaries(void);
Uint32        syBupReleaseState(void);

void          sySerialEnableTransferTimer(void);
void          sySerialDisableTransfer(void);
Sint32        sySerialPollStatus(void);
Sint32        sySerialExchangePayloads(void *send, void *receive);
void          sySerialPrepareSendPacket(void *payload);
Uint8         sySerialCollectReceivedPayloads(void *payload);
void          sySerialHandleTransferInterrupt(void);
void          syLinkBattleResetTransferState(void);
void          sySerialTestDelay(void);
void          sySerialTestRun(void);

// syRandom's seed; Lucky Wheels seeds it with the play time, and a link battle's parent sends it as
// the turn's BattleRandomSeed.
extern Uint32 RandomSeed;

// The system library's groups.

// The dma group. DMA channel registers, and the DMA and CPU fills. Fills `size` bytes at
// `destination` with the halfword `value` by DMA3, from a volatile copy of it on the stack.
#define DMA_FILL16(value, destination, size)                                                       \
    {                                                                                              \
        volatile Uint16 fill_ = (value);                                                           \
        syDMAXCopy((const void *)&fill_, (destination), DMA_FILL_HALFWORDS((size) / 2), REG_DMA3); \
    }

// Fills `size` bytes at `destination` with the word `value` by DMA3.
#define DMA_FILL32(value, destination, size)                                                       \
    {                                                                                              \
        volatile Uint32 fill_ = (value);                                                           \
        syDMAXCopy((const void *)&fill_, (destination), DMA_FILL_WORDS((size) / 4), REG_DMA3);     \
    }

// DMA_FILL32 through the caller's own volatile word `cell`, so a function that fills several blocks
// keeps one word for all of them and forms its address once.
#define DMA_FILL32_CELL(cell, value, destination, size)                                            \
    {                                                                                              \
        (cell) = (value);                                                                          \
        syDMAXCopy((const void *)&(cell), (destination), DMA_FILL_WORDS((size) / 4), REG_DMA3);    \
    }

// pret's CpuFill32: repeats one word through the BIOS CpuSet service.
#define CPU_FILL32(value, destination, size)                                                       \
    {                                                                                              \
        volatile Uint32 fill_ = (value);                                                           \
        BiosCpuSet((const void *)&fill_,                                                           \
                   (destination),                                                                  \
                   CPU_SET_32BIT | CPU_SET_SRC_FIXED | ((size) / 4));                              \
    }

// Stops DMA 0, which a scene may have left repeating each HBlank for a line effect: drops its start
// timing and repeat, then its enable, and reads the control back.
#define DMA0_STOP()                                                                                \
    {                                                                                              \
        volatile Uint16 *dma0_       = REG_DMA0;                                                   \
                                                                                                   \
        dma0_[DMA_CONTROL_HALFWORD] &= ~DMA_START_TIMING_DRQ_REPEAT;                               \
        dma0_[DMA_CONTROL_HALFWORD] &= ~DMA_ENABLE;                                                \
        dma0_[DMA_CONTROL_HALFWORD];                                                               \
    }

// Starts a DMA transfer: stores the source, destination and control words into `channel`'s
// registers with one stmia.
static __inline__ void
syDMAXCopy(const volatile void *src, volatile void *dest, Uint32 control, volatile Uint32 *channel)
{
    ASM_REGISTER(volatile Uint32 *dma, "r3") = channel;
    ASM_REGISTER(Uint32 srcReg, "r0")        = (Uint32)src;
    ASM_REGISTER(Uint32 destReg, "r1")       = (Uint32)dest;
    ASM_REGISTER(Uint32 count, "r2")         = control;
    ASM_DMA_STORE(srcReg, count, dma, destReg);
}

// Waits until `channel`'s transfer is done: SGL's slDMAWait.
static __inline__ void syDMAWait(DMA_REGISTERS volatile *channel)
{
    while ((channel->control & DMA_BUSY) != 0)
    {
    }
}

// The io_write group. IO_WRITE8, IO_WRITE16 and IO_WRITE32, writes to a register or memory. Writes
// a value to a 16-bit register or memory location.
#define IO_WRITE16(v, reg)                                                                         \
    do                                                                                             \
    {                                                                                              \
        Uint32 value_ = (v);                                                                       \
        *(reg)        = value_;                                                                    \
    } while (0)

// Writes a value to a 32-bit register or memory location, in IO_WRITE16's shape.
#define IO_WRITE32(v, reg)        IO_WRITE16(v, reg)

// Writes a value to an 8-bit register or memory location, in IO_WRITE16's shape.
#define IO_WRITE8(v, reg)         IO_WRITE16(v, reg)

// Reads a 16-bit register or memory location into a variable. The variable is reached through its
// address, so a halfword one lives in the tick and is read back from there after each call. The
// Lost Age has the same macro.
#define syIORead16(reg, variable) (*(variable) = *(reg))

// The irq_lock group. Saving, disabling and restoring the interrupt master enable.
//
// The three steps take ime, a pointer to REG_IME. Disabling writes the pointer's low halfword
// (0x0208, bit 0 clear) back through it.

// Saves the original IME value in saved.
#define IME_SAVE(ime, saved)                                                                       \
    do                                                                                             \
    {                                                                                              \
        (saved) = *(ime);                                                                          \
    } while (0)

// Disables interrupts.
#define IME_DISABLE(ime)                                                                           \
    do                                                                                             \
    {                                                                                              \
        *(ime) = (Uint16)(Uint32)(ime);                                                            \
    } while (0)

// Restores IME from saved.
#define IME_RESTORE(ime, saved)                                                                    \
    do                                                                                             \
    {                                                                                              \
        *(ime) = (saved);                                                                          \
    } while (0)

// Disables interrupts and saves IME in saved; IRQ_UNLOCK restores it.
#define IRQ_LOCK(saved)                                                                            \
    do                                                                                             \
    {                                                                                              \
        volatile Uint16 *ime_;                                                                     \
                                                                                                   \
        IME_SAVE((ime_ = &REG_IME), saved);                                                        \
        IME_DISABLE(ime_);                                                                         \
    } while (0)

// Restores the IME value saved by IRQ_LOCK.
#define IRQ_UNLOCK(saved)                                                                          \
    do                                                                                             \
    {                                                                                              \
        REG_IME = (saved);                                                                         \
    } while (0)

// Disables interrupts, keeping IME in a variable of the caller's that an interrupt may read.
static inline void syIRQLock(volatile Uint32 *saved)
{
    Uint16 *ime;

    IME_SAVE((ime = (Uint16 *)&REG_IME), *saved);
    IME_DISABLE(ime);
}

// Restores the IME that syIRQLock kept.
static inline void syIRQUnlock(volatile Uint32 *saved)
{
    Uint32 value = *saved;

    REG_IME      = value;
}

// The io_write_queue group. The register write queue that VBlank carries out after each finished
// tick. Disables interrupts to add one register write or DMA to the queue; a write to a full queue
// is dropped. control is a width (IO_WRITE_BYTE to IO_WRITE_WORD), with IO_WRITE_SET or
// IO_WRITE_CLEAR to change bits; or a DMA control word.
#define IO_WRITE_QUEUE_PUSH(address, value, control)                                               \
    {                                                                                              \
        IO_WRITE_QUEUE *q = &IoWriteQueueWork;                                                     \
        Uint32          saved;                                                                     \
        Sint32          count;                                                                     \
                                                                                                   \
        IRQ_LOCK(saved);                                                                           \
        count = *&q->count;                                                                        \
        if (count <= 31)                                                                           \
        {                                                                                          \
            Uint32 *entry = q->entries[count];                                                     \
                                                                                                   \
            *&q->count    = count + 1;                                                             \
            *entry++      = (value);                                                               \
            *entry++      = (address);                                                             \
            *entry        = (control);                                                             \
        }                                                                                          \
        IRQ_UNLOCK(saved);                                                                         \
    }

// Like IO_WRITE_QUEUE_PUSH, but keeps IME's address in the caller's ime variable while disabling
// and restoring interrupts.
#define IO_WRITE_QUEUE_PUSH_IME(ime, address, value, control)                                      \
    {                                                                                              \
        IO_WRITE_QUEUE *q;                                                                         \
        Uint32          saved;                                                                     \
        Sint32          count;                                                                     \
                                                                                                   \
        q = &IoWriteQueueWork;                                                                     \
        IME_SAVE(ime, saved);                                                                      \
        IME_DISABLE(ime);                                                                          \
        count = *&q->count;                                                                        \
        if (count <= 31)                                                                           \
        {                                                                                          \
            Uint32 *entry = q->entries[count];                                                     \
                                                                                                   \
            *&q->count    = count + 1;                                                             \
            *entry++      = (value);                                                               \
            *entry++      = (address);                                                             \
            *entry        = (control);                                                             \
        }                                                                                          \
        IME_RESTORE(ime, saved);                                                                   \
    }

#include "system/iwram_call.h"
#include "game/main_routine.h"

// The matrix group. The 3D transform and its stack. Sets a twelve-word matrix to the unit matrix:
// toFIXED(1) (1.0) on the rotation's diagonal, the rest 0.
static __inline__ void syUnitMatrix(MATRIX matrix)
{
    ASM_MATRIX_UNIT(matrix);
}

// Resets the current matrix to the unit matrix.
static __inline__ void syUnitMatrixCurrentInline(void)
{
    ASM_MATRIX_UNIT_CURRENT(CurrentMatrix);
}

#include "system/mul_fx.h"

// The ram_code group. Copying ARM functions to RAM to run them there.
//
// Copies an ARM function to RAM for execution. bytes is its linker-defined size.

// Allocates a heap copy; release it with syHeapRelease when finished.
#define RAM_CODE_LOAD(to, code, bytes)                                                             \
    do                                                                                             \
    {                                                                                              \
        Uint32 size_;                                                                              \
                                                                                                   \
        size_ = (Uint32)(bytes);                                                                   \
                                                                                                   \
        (to)  = syHeapAllocate(size_);                                                             \
        syDMAXCopy((const void *)(code), (to), DMA_COPY_WORDS(size_ >> 2), REG_DMA3);              \
    } while (0)

// Allocates a heap-slot copy; release it with syHeapSlotRelease.
#define RAM_CODE_LOAD_SLOT(slot, code, bytes)                                                      \
    do                                                                                             \
    {                                                                                              \
        Uint32 size_;                                                                              \
                                                                                                   \
        size_ = (Uint32)(bytes);                                                                   \
                                                                                                   \
        syDMAXCopy((const void *)(code),                                                           \
                   syHeapSlotAllocate((slot), size_),                                              \
                   DMA_COPY_WORDS(size_ >> 2),                                                     \
                   REG_DMA3);                                                                      \
    } while (0)

// The serial group. The link cable's runtime: serial packets, registers and transfer state. Adds
// LINK_STATUS_ERROR to a link status once a transfer has failed.
static inline Sint32 sySerialAddErrorFlag(Sint32 status)
{
    if (SerialStateWork.transferError != 0)
        status |= LINK_STATUS_ERROR;
    return status;
}

// Abandons any send or receive and clears the block transfer's state, with interrupts held off:
// what a failed link battle or Battle Arena exchange does.
static __inline__ void sySerialResetTransferState(void)
{
    SERIAL_TRANSFER_STATE *state;
    Uint32                 saved;

    state = &SerialTransfer;
    IRQ_LOCK(saved);
    {
        Uint32 clear = 0;

        IO_WRITE8(SERIAL_STATUS_RUNNING, &state->status);
        SerialSendSource         = clear;
        SerialSendSize           = clear;
        state->peerFlags         = clear;
        state->flags             = clear;
        SerialReceiveDestination = clear;
        SerialReceivedSize       = clear;
    }
    IRQ_UNLOCK(saved);
}

#include "system/vector3.h"

#endif
