#ifndef ALCHEMY_GLOBALS_H
#define ALCHEMY_GLOBALS_H

// The system library's variables, as SGL keeps its system variables and work areas: each group's
// variables in IWRAM, EWRAM and ROM. system.h includes it; a file built by another compiler can
// include it with system/definitions.h.

#include "types.h"
#include "fixed_math.h"
#include "edition.h"
#include "static_assert.h"
#include "system/definitions.h"

// The event group. Events, their priorities and the event runner's state. 1 while sySynch() runs
// the tick pass's events (EVENT_PASS_TICK).
extern volatile Uint8          EventsRunning;
extern Uint8                   EventsInitialized; // 1 once the event table is set up
// The event table, run in priority order by each pass.
extern EVENT                   EventBuf[];

// The irq group. The hardware's interrupt sources and the interrupt handler table. The handler the
// IWRAM dispatcher calls for each interrupt source.
extern INTERRUPT_HANDLER       IrqHandlers[IRQ_COUNT];

// The handlers installed at power-on, copied into IrqHandlers.
extern const Uint32            DefaultIrqHandlers[IRQ_COUNT];

// The interrupt dispatcher at the start of the IWRAM-resident code, whose other functions are
// reached at fixed offsets from it (iwram_call.h).
extern Uint8                   syIWRAMIRQMain[];
// An instruction inside the dispatcher (`orr r3, r3, #0x92`), whose value btDMAStopDMA3 runs as a
// function.
extern const Uint32            IwramIrqModeWord;
// Where the IWRAM-resident code lies in ROM (ld_script.ld), 5,120 bytes copied to syIWRAMIRQMain at
// power-on.
extern const Uint8             IwramResidentRom[];
// The BIOS's interrupt vector, pointed at syIWRAMIRQMain at power-on.
extern void                   *IrqVector;

// The heap group. The heap's slots and HEAP_STATE. The heap's 64 words in IWRAM, seen as
// HEAP_STATE.
extern void                   *HeapSlots[64];

// The heap_slots group. The heap slots of HeapSlots, read through their own names.
//
// EWRAM buffers The map's metatiles at 0x02010000, 128 by 128 words (MAP_GRID_METATILE): 64 KB, so
// while a map is loaded they run on through ActorSpriteSlots and DecodeBuffer.
extern Uint32                  MapGridMetatileBuffer[];
// 16 KB of EWRAM at 0x0201c000.
extern Uint8                   DecodeBuffer[];
// 16 KB of EWRAM at 0x02018000, where the battle's actor sprite buffers start, 16 KB a slot; the
// map's metatiles fill it while a map is loaded.
extern Uint8                   ActorSpriteSlots[];

// The heap_release group. Releasing heap blocks, and the two pools' space. The EWRAM heap's space,
// 32 KB from 0x02030000.
extern Uint8                   EwramHeap[];
// The IWRAM heap's space, from 0x03002000 up to syRamIWRAMHeapEnd.
extern Uint8                   IwramHeap[];

// The affine group. Sprite rotation and scale, and the affine matrices handed out each tick.
// Matrices handed out this tick (syOAMMatrixAllocateFromTransform), up to 32; syOAMResetTick sets
// it back to 0 after each VBlank.
extern Uint8                   OamMatrixCount;
// This tick's 32 matrices, copied into OAM with the sprites.
extern OAM_MATRIX              OamMatrices[];
// The 256 depth slots' list heads that every drawn sprite is linked into (syOAMAddSpriteAtDepth);
// syOAMResetTick empties them each tick.
extern void                   *OamDepthLists[];

// The decompress group. The decoders and canvas fades that the system library runs from RAM copies
// (system/system_decompress.c), and their sizes.
//
// Each is handwritten ARM code that the game never runs where it lies: its loader copies it to the
// heap or the stack first and calls the copy, so it is declared as an array of bytes. A symbol
// ending in CodeSize is the length in bytes of the code the loader copies: not an object, but an
// address the linker sets equal to the length (in the function's assembly file, or ld_script.ld
// where the end is the next function), so the loader reads it by casting it to a number. The
// byte-oriented LZ decoder, run by syDecompressByteLZ from a heap copy.
extern const Uint8             syDecompressByteLZArm[];
// syDecompressByteLZArm: how much syDecompressByteLZ allocates for it.
extern Uint8                   syDecompressByteLZCodeSize[];
// The tagged LZ decoder (a first byte of 0 for the general form, 1 for the palette form), run by
// syDecompressTaggedLZ from a heap copy.
extern const Uint8             syDecompressTaggedLZArm[];
// syDecompressTaggedLZArm, which syDecompressTaggedLZ copies to the heap.
extern Uint8                   syDecompressTaggedLZCodeSize[];
// The move-to-front LZ decoder's ARM entry: it returns 4 unless the stream's first byte is 2, the
// move-to-front tag.
extern const Uint8             syDecompressMoveToFrontLZCommands[];
// syDecompressMoveToFrontLZCommands through its command loop, which syDecompressMoveToFrontLZ
// copies to the heap.
extern Uint8                   DecompressMoveToFrontLzCommandsCodeSize[];
// The halfword LZ decoder that syDecompressHalfwords runs from its stack.
extern const Uint8             syDecompressHalfwordLZ[];
// syDecompressHalfwordLZ, copied onto the stack by syDecompressHalfwords.
extern Uint8                   syDecompressHalfwordsCodeSize[];
// The colour index fdRenderDecodePicture paints every pixel of a layer drawn with
// SPRITE_COLOR_FILL; the sprite viewer sets it to 3.
extern Uint8                   DecodeFillByte;

// The canvas fades' ARM functions and their sizes: each CanvasCopyAnd... function in
// system/system_decompress.c copies its ARM code onto its own stack and runs it there.
// syCanvasBrightenArm, for syCanvasCopyAndBrighten.
extern Uint8                   syCanvasCopyAndBrightenCodeSize[];
// syCanvasBrightenPartialArm, for syCanvasCopyAndBrightenPartial.
extern Uint8                   syCanvasCopyAndBrightenPartialCodeSize[];
// syCanvasDarkenArm, for syCanvasCopyAndDarken.
extern Uint8                   syCanvasCopyAndDarkenCodeSize[];
// syCanvasDarkenPartialArm, for syCanvasCopyAndDarkenPartial.
extern Uint8                   syCanvasCopyAndDarkenPartialCodeSize[];
// syCanvasHalveArm, for syCanvasCopyAndHalve.
extern Uint8                   syCanvasCopyAndHalveCodeSize[];
// syCanvasHalveNonzeroArm, for syCanvasCopyAndHalveNonzero.
extern Uint8                   syCanvasCopyAndHalveNonzeroCodeSize[];
// syCanvasScaleNonzeroThreeQuartersArm, for syCanvasCopyAndScaleNonzeroThreeQuarters.
extern Uint8                   syCanvasCopyAndScaleNonzeroThreeQuartersCodeSize[];
// syCanvasScaleThreeQuartersArm, for syCanvasCopyAndScaleThreeQuarters.
extern Uint8                   syCanvasCopyAndScaleThreeQuartersCodeSize[];
// Raises each pixel step by an amount, capped at 63, after backing the buffer up.
extern const Uint8             syCanvasBrightenArm[];
// Brightens as syCanvasBrightenArm, writing back only groups of four pixels not all at 63 already.
extern const Uint8             syCanvasBrightenPartialArm[];
// Lowers each pixel step by an amount, stopping at 0, after backing the buffer up.
extern const Uint8             syCanvasDarkenArm[];
// Darkens as syCanvasDarkenArm, writing back only groups of four pixels not all 0 already.
extern const Uint8             syCanvasDarkenPartialArm[];
// Halves each pixel step; the battle effect canvas's transfers use it.
extern const Uint8             syCanvasHalveArm[];
// Halves each pixel step, skipping all-zero groups; Lucky Wheels' canvas uses it.
extern const Uint8             syCanvasHalveNonzeroArm[];
// Scales each pixel step to three quarters, skipping all-zero groups.
extern const Uint8             syCanvasScaleNonzeroThreeQuartersArm[];
// Scales each pixel step to three quarters.
extern const Uint8             syCanvasScaleThreeQuartersArm[];

// The stub group. The stub tables the system library's folder holds. The system library's stub
// table (system/stubs.s), through which the world map reaches its arctangent, sine and cosine.
extern const SYSTEM_STUB_TABLE StubsSystem;
// The resource module's stub, listed in the resource directory as both ResourceStubs and
// ResourceStubsAlternate.
extern const Uint8             StubsResource[];

// The tick group. The tick loop's globals: the frames a tick takes, the CPU load display and the
// VBlank flags. Set by sySynch() once it has built a tick's OAM list; the VBlank handler then
// copies the list, the BG scroll and the queued register writes, and clears it.
extern Uint8                   TickRenderPending;
// Debug: sySynch() measures the CPU load into CpuLoadPeak when set.
extern volatile Uint8          CpuLoadDisplayEnabled;
// The VBlank handler copies the OAM list only while it is set; scenes clear it to keep their own
// sprites on screen.
extern Uint8                   OamCopyEnabled;
// Frames since sySynch() last started a tick: 1 when the tick took one.
extern Uint16                  TickFrames;
// TickFrames as the last tick left it; the text typing paces itself by it.
extern volatile Uint16         LastTickFrames;
// Bit 0 is set by the VBlank handler; sySynch() clears it and halts until it is set again.
// volatile: the VBlank handler sets it while sySynch() halts on it.
extern volatile Uint16         VBlankDone;
extern Uint32                  CpuLoadTimer; // Ticks CpuLoadPeak is still held.
// The highest CPU load in the last 30 ticks: the scanline the tick's work ended on, plus 256 for
// each VBlank it overran.
extern Uint32                  CpuLoadPeak;
// Ticks sySynch() has finished. volatile: the VBlank pass reads it (fdWorldMapApplyAffinePage picks
// the world map's affine page by it) while sySynch() counts it.
extern volatile Uint32         TickCount;
extern Uint32                  FrameCount; // Counted up by every VBlank.
// A function for the next VBlank alone: the handler clears it, then calls it.
extern void                    (*VBlankCallback)(void);

// Nonzero: sySynch() counts no idle time and ignores sleep requests.
extern volatile Uint8          SleepDisabled;
// Sleep after three idle minutes.
extern volatile Uint8          IdleSleepEnabled;
// 1: sleep this tick; more: count down to it. volatile: sySynch() counts it down and reads it each
// tick.
extern volatile Uint8          SleepRequested;
// Debug builds: Start+Select pauses the tick loop. volatile: fdActorUpdatePlayerControl and
// fdModeRun each read it again after an earlier test.
extern volatile Uint8          DebugMode;
extern Uint8                   DebugPaused; // The debug pause is on.
// Set by the keypad interrupt; sySynch() performs the soft reset.
extern Uint8                   ResetRequested;
// Ticks since a key was last held. volatile: sySynch() counts it and reads it each tick.
extern volatile Uint16         IdleTicks;
// Ticks L and R have been held together; three seconds request sleep. volatile: sySynch() counts it
// and reads it each tick.
extern volatile Uint16         SleepComboTicks;
// Written by the soft reset just before it restarts the cartridge, and cleared by syInit; nothing
// else reads it.
extern Uint32                  CartridgeResetMarker;
// The build stamp syCopyBuildStamp copies to IWRAM at power-on: twelve digits and a zero word, at
// the bottom of the tick-wait stack.
extern Sint32                  BuildStampWork[];
extern Uint32                  PlayTimeTicks; // The play time clock, in ticks.
// Bytes of the caller's stack sySynch() has copied into SavedStack, or 0.
extern Uint32                  SavedStackSize;
// The caller's stack below the tick-wait stack top, while sySynch() runs.
extern Uint8                   SavedStack[];

// The input group. The keypad state the VBlank interrupt keeps.
//
// The VBlank handler reads the keypad into these metatiles, and syInputUpdateKeyRepeatAndDirection
// turns them into repeats after each tick (system/system_frame.c). The first three are volatile
// because the VBlank handler sets them while the code it interrupts reads them; the next two the
// key repeat rebuilds and reads each tick. Keys down now.
extern volatile Uint32         PerData;
// Keys that went down this tick.
extern volatile Uint32         PerPush;
// Pressed keys, repeating while held: after 19 ticks, then every 6.
extern volatile Uint32         PerRepeat;
// PerRepeat with at most one D-pad direction.
extern volatile Uint32         PerRepeatDirected;
// The D-pad direction the repeat last settled on: when a second direction is added,
// PerRepeatDirected drops this one and keeps the new one. KEYS_HORIZONTAL, both left and right,
// stands for none.
extern volatile Uint32         PerDirection;

// Kept by the tick loop and the key repeat.
extern Uint32                  PerPushLatch;    // Presses since a reader last cleared it.
extern Uint32                  PerDataLast;     // PerData when the tick loop last read it.
extern Uint32                  PerDataAtVBlank; // PerData at the previous VBlank.
// Ticks until the held keys repeat; -1 makes them repeat at once.
extern Sint32                  PerRepeatTimer;

// The screen_fade group. The brightness fade of the whole screen that the VBlank handler steps
// (system/system_screen_fade.c). Which way the screen fade goes: 0 towards black, 1 towards white.
extern Uint8                   ScreenFadeToWhite;
// The running fade's length in frames, 0 when no fade is running. volatile: the VBlank handler's
// syScreenFadeUpdate clears it when the fade ends, while syScreenFadeWait waits on it.
extern volatile Uint8          ScreenFadeFrames;
// Frames until the running fade reaches its target. volatile: the VBlank handler's
// syScreenFadeUpdate counts it down.
extern volatile Uint8          ScreenFadeFramesLeft;
// The BLDCNT first-target layers the screen fade covers (the low six bits).
extern Uint16                  ScreenFadeLayers;
// The brightness the running fade started from, 0 to 16.
extern Uint8                   ScreenFadeStartLevel;
// The brightness the running fade ends at, 0 to 16. volatile: syScreenFadeStart sets it for the
// VBlank handler's syScreenFadeUpdate.
extern volatile Uint8          ScreenFadeTargetLevel;

// The io_write_queue group. The register write queue that VBlank carries out after each finished
// tick. The queue IO_WRITE_QUEUE_PUSH and the IoWriteQueue writers fill, and
// syIOWriteQueueFlushPending empties at VBlank.
extern IO_WRITE_QUEUE          IoWriteQueueWork;

// The iwram_call group. The IWRAM call layer: the functions the game keeps resident in IWRAM, each
// at its fixed address, and one typed inline per function that every call goes through. syMulFX's
// function in IWRAM, which returns through ip.
extern Uint8                   syIWRAMMulFXReturnIp[];
// The IWRAM function that clears a run of words, which the OAM build calls by its address.
extern Uint8                   syIWRAMWordsClear[];

// The math_table group. The trigonometry tables (system/math_table.c): the sine of a quarter turn
// and the tangents syAtan searches.
extern const Uint16            MathSinQuarter[256];
extern const Uint16            MathTwoPiSteps[8];
extern const Sint32            MathQuadrantSin[4];
extern const Uint16            MathTanTable[258];

// The matrix group. The 3D transform and its stack. The current 3D transform. syMultiMatrix
// multiplies the matrix it is given into it.
extern MATRIX                  CurrentMatrix;

// Matrices saved by syPushMatrix, 0 or 1: the save area holds one.
extern Sint32                  MatrixStackDepth;
// Where syPushMatrix saves CurrentMatrix next: the save area in heap slot HEAP_SLOT_MATRIX_STACK,
// or just past it while a matrix is saved.
extern Uint8                  *MatrixStackTop;
// syMatrixBuildLookAt's ARM dot product, seven words it copies onto its stack to run.
extern const Uint32            RelocatedInnerProduct[];

// The number_text group. The digits and place values the number formatters write with
// (system/number_text.c).
extern const Uint8             StringHexDigits[];
extern const Sint32            StringPowersOfTen[9];
// The number text buffer: syStringFormatHex writes eight digits and syStringFormatDecimal eleven
// characters, right-aligned, each with a terminator.
extern Uint8                   NumberBuffer[];

// The projection group. The 3D projection. The current 3D projection: syPerspective and
// sySetScreenDist write its focal length and depth range, and the battle and its battle effects set
// its centre directly.
extern PROJECTION              ProjectionWork;

// The bupdata group. The save sectors' signature and header template, and the link test's strings
// (system/bupdata.c).
extern const Uint8             BupSignature[];
extern const Uint8             BupHeaderTemplate[8];

extern const Uint32            LinkTestWord;
extern const Uint8             LinkTestDataA[];
extern const Uint8             LinkTestDataB[];

// The serial group. The link cable's runtime: serial packets, registers and transfer state. This
// console's block transfer state, sent to the partners each VBlank.
extern SERIAL_TRANSFER_STATE   SerialTransfer;
extern SERIAL_STATE            SerialStateWork;
// Set from sySerialInit while the consoles exchange packets every tick.
extern Uint16                  SerialExchangeActive;
// What the send in progress sends from, 0 when none is running.
extern Sint32                  SerialSendSource;
// The bytes the send in progress has left to send. volatile: sySerialStartSend sets it while
// sySerialStepBlockTransfer, which the VBlank handler runs, counts it down.
extern volatile Uint16         SerialSendSize;
// Where the receive in progress writes, 0 when none is running.
extern Sint32                  SerialReceiveDestination;
// The bytes the receive in progress has taken so far. volatile: sySerialStepBlockTransfer, which
// the VBlank handler runs, counts it up while the link code outside the interrupt polls it.
extern volatile Uint16         SerialReceivedSize;
extern SERIAL_TRANSFER_STATE   SerialPeerPayloads[4];
// The number of the block being sent or received, from 0 at each transfer. volatile:
// sySerialStartSend and sySerialStartReceive reset it while sySerialStepBlockTransfer, which the
// VBlank handler runs, steps it.
extern volatile Uint8          SerialBlockNumber;
// Cleared as the link starts and as the Battle Arena trades parties; nothing reads it.
extern Uint8                   LinkExchangeState;
// Link status, updated by the VBlank exchange handler. volatile: the VBlank handler writes it while
// sySerialWaitForStatusMask and the link screens poll it.
extern volatile Uint16         LinkStatusWork;

// The vram_cache group. The sprite VRAM block cache and its owners. The cache entry that owns each
// block, or VRAM_BLOCK_OWNER_FREE.
extern Uint8                   VramCacheBlockOwners[VRAM_BLOCK_COUNT];

// The cache entries by number: the slot a sprite or loaded picture holds.
extern VRAM_CACHE_ENTRY        VramCacheEntries[];

#endif
