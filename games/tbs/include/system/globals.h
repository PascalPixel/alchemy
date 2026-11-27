#ifndef ALCHEMY_GLOBALS_H
#define ALCHEMY_GLOBALS_H

/* @file */
/* The system library's variables, as SGL's sgl_work.h holds its work areas: */
/* each group's variables in IWRAM, EWRAM and ROM. system.h includes it; a */
/* file built by another compiler can include it with system/definitions.h. */

#include "types.h"
#include "fixed_math.h"
#include "edition.h"
#include "static_assert.h"
#include "system/definitions.h"

/* The task group. Tasks, their priorities and the task runner's state. */
/* 1 while sySynch() runs the frame pass's tasks (TASK_PASS_FRAME). */
extern volatile u8 TasksRunning;
extern u8 TasksInitialized; /* 1 once the task table is set up */
/* The task table, run in priority order by each pass. */
extern TASK TasksWork[];

/* The irq group. The hardware's interrupt sources and the interrupt handler */
/* table. */
/* The handler the IWRAM dispatcher calls for each interrupt source. */
extern INTERRUPT_HANDLER IrqHandlers[IRQ_COUNT];

/* The handlers installed at power-on, copied into IrqHandlers. */
extern const u32 DefaultIrqHandlers[IRQ_COUNT];

/* The interrupt dispatcher at the start of the IWRAM-resident code, whose */
/* other functions are reached at fixed offsets from it (iwram_call.h). */
extern u8 IwramIrqMain[];
/* An instruction inside the dispatcher (`orr r3, r3, #0x92`), whose value */
/* btDmaStopDma3 runs as a function. */
extern const u32 IwramIrqModeWord;
/* Where the IWRAM-resident code lies in ROM (ld_script.ld), 5,120 bytes copied
 */
/* to IwramIrqMain at power-on. */
extern const u8 IwramResident_Rom[];
/* The BIOS's interrupt vector, pointed at IwramIrqMain at power-on. */
extern void *IrqVector;

/* The heap group. The heap's slots and HEAP_STATE. */
/* The heap's 64 words in IWRAM, seen as HEAP_STATE. */
extern void *HeapSlots[64];

/* The heap_slots group. The heap slots of HeapSlots, read through their own */
/* names. */
/* */
/* EWRAM buffers */
/* The map's cells at 0x02010000, 128 by 128 words (MAP_GRID_CELL): 64 KB,
 */
/* so while a map is loaded they run on through ActorSpriteSlots and */
/* DecodeBuffer. */
extern u32 MapGridCellBuffer[];
/* 16 KB of EWRAM at 0x0201c000. */
extern u8 DecodeBuffer[];
/* 16 KB of EWRAM at 0x02018000, where the battle's actor sprite buffers */
/* start, 16 KB a slot; the map's cells fill it while a map is loaded. */
extern u8 ActorSpriteSlots[];

/* The heap_release group. Releasing heap blocks, and the two pools' space. */
/* The EWRAM heap's space, 32 KB from 0x02030000. */
extern u8 EwramHeap[];
/* The IWRAM heap's space, from 0x03002000 up to syRamIwramHeapEnd. */
extern u8 IwramHeap[];

/* The affine group. Sprite rotation and scale, and the affine matrices handed
 */
/* out each frame. */
/* Matrices handed out this frame (syOamMatrixAllocFromTransform), up to 32; */
/* syOamResetFrame sets it back to 0 after each VBlank. */
extern u8 OamMatrixCount;
/* This frame's 32 matrices, copied into OAM with the sprites. */
extern OAM_MATRIX OamMatrices[];
/* The 256 depth slots' list heads that every drawn sprite is linked into */
/* (syOamAddSprAtDepth); syOamResetFrame empties them each frame. */
extern void *OamDepthLists[];

/* The decompress group. The decoders and canvas fades that the system library
 */
/* runs from RAM copies (system/system_decompress.c), and their sizes. */
/* */
/* Each is handwritten ARM code that the game never runs where it lies: its */
/* loader copies it to the heap or the stack first and calls the copy, so it is
 */
/* declared as an array of bytes. A symbol ending in CodeSize is the length in
 */
/* bytes of the code the loader copies: not an object, but an address the */
/* linker sets equal to the length (in the function's assembly file, or */
/* ld_script.ld where the end is the next function), so the loader reads it by
 */
/* casting it to a number. */
/* The byte-oriented LZ decoder, run by fdDecompressByteLz from a heap copy. */
extern const u8 fdDecompressByteLzArm[];
/* fdDecompressByteLzArm: how much fdDecompressByteLz allocates for it. */
extern u8 fdDecompressByteLzCodeSize[];
/* The tagged LZ decoder (a first byte of 0 for the general form, 1 for the */
/* palette form), run by fdDecompressTaggedLz from a heap copy. */
extern const u8 fdDecompressTaggedLzArm[];
/* fdDecompressTaggedLzArm, which fdDecompressTaggedLz copies to the heap. */
extern u8 fdDecompressTaggedLzCodeSize[];
/* The move-to-front LZ decoder's ARM entry: it returns 4 unless the */
/* stream's first byte is 2, the move-to-front tag. */
extern const u8 Decompress_MoveToFrontLzCommands[];
/* Decompress_MoveToFrontLzCommands through its command loop, which */
/* fdDecompressMoveToFrontLz copies to the heap. */
extern u8 Decompress_MoveToFrontLzCommandsCodeSize[];
/* The halfword LZ decoder that syDecompressHalfwords runs from its stack. */
extern const u8 Decompress_HalfwordLz[];
/* Decompress_HalfwordLz, copied onto the stack by syDecompressHalfwords. */
extern u8 syDecompressHalfwordsCodeSize[];
/* The colour index Render_DecodePicture paints every pixel of a layer drawn */
/* with SPRITE_COLOR_FILL; the sprite viewer sets it to 3. */
extern u8 DecodeFillByte;

/* The canvas fades' ARM functions and their sizes: each Canvas_CopyAnd... */
/* function in system/system_decompress.c copies its ARM code onto its own */
/* stack and runs it there. */
/* Canvas_BrightenArm, for syCanvasCopyAndBrighten. */
extern u8 syCanvasCopyAndBrightenCodeSize[];
/* Canvas_BrightenPartialArm, for */
/* syCanvasCopyAndBrightenPartial. */
extern u8 syCanvasCopyAndBrightenPartialCodeSize[];
/* Canvas_DarkenArm, for syCanvasCopyAndDarken. */
extern u8 syCanvasCopyAndDarkenCodeSize[];
/* Canvas_DarkenPartialArm, for syCanvasCopyAndDarkenPartial. */
extern u8 syCanvasCopyAndDarkenPartialCodeSize[];
/* Canvas_HalveArm, for syCanvasCopyAndHalve. */
extern u8 syCanvasCopyAndHalveCodeSize[];
/* Canvas_HalveNonzeroArm, for syCanvasCopyAndHalveNonzero. */
extern u8 syCanvasCopyAndHalveNonzeroCodeSize[];
/* Canvas_ScaleNonzeroThreeQuartersArm, for */
/* syCanvasCopyAndScaleNonzeroThreeQuarters. */
extern u8 syCanvasCopyAndScaleNonzeroThreeQuartersCodeSize[];
/* Canvas_ScaleThreeQuartersArm, for */
/* syCanvasCopyAndScaleThreeQuarters. */
extern u8 syCanvasCopyAndScaleThreeQuartersCodeSize[];
/* Raises each pixel step by an amount, capped at 63, after backing the */
/* buffer up. */
extern const u8 Canvas_BrightenArm[];
/* Brightens as Canvas_BrightenArm, writing back only groups of four */
/* pixels not all at 63 already. */
extern const u8 Canvas_BrightenPartialArm[];
/* Lowers each pixel step by an amount, stopping at 0, after backing the */
/* buffer up. */
extern const u8 Canvas_DarkenArm[];
/* Darkens as Canvas_DarkenArm, writing back only groups of four */
/* pixels not all 0 already. */
extern const u8 Canvas_DarkenPartialArm[];
/* Halves each pixel step; the battle effect canvas's transfers use it. */
extern const u8 Canvas_HalveArm[];
/* Halves each pixel step, skipping all-zero groups; Lucky Wheels' canvas uses
 */
/* it. */
extern const u8 Canvas_HalveNonzeroArm[];
/* Scales each pixel step to three quarters, skipping all-zero groups. */
extern const u8 Canvas_ScaleNonzeroThreeQuartersArm[];
/* Scales each pixel step to three quarters. */
extern const u8 Canvas_ScaleThreeQuartersArm[];

/* The stub group. The stub tables the system library's folder holds. */
/* The system library's stub table (system/stubs.s), through which the */
/* world map reaches its arctangent, sine and cosine. */
extern const SYSTEM_STUB_TABLE Stubs_System;
/* The resource module's stub, listed in the resource directory as */
/* both ResourceStubs and ResourceStubsAlternate. */
extern const u8 Stubs_Resource[];

/* The frame group. The frame loop's globals: the VBlanks a frame takes, the */
/* CPU load display and the VBlank flags. */
/* Set by sySynch() once it has built a frame's OAM list; the VBlank handler */
/* then copies the list, the BG scroll and the queued register writes, and */
/* clears it. */
extern u8 FrameRenderPending;
/* Debug: sySynch() measures the CPU load into CpuLoadPeak when set. */
extern volatile u8 CpuLoadDisplayEnabled;
/* The VBlank handler copies the OAM list only while it is set; scenes clear */
/* it to keep their own sprites on screen. */
extern u8 OamCopyEnabled;
/* VBlanks since sySynch() last started a frame: 1 when the frame took one. */
extern u16 FrameVBlanks;
/* FrameVBlanks as the last frame left it; the text typing paces itself by */
/* it. */
extern volatile u16 LastFrameVBlanks;
/* Bit 0 is set by the VBlank handler; sySynch() clears it and halts until */
/* it is set again. */
/* volatile: the VBlank handler sets it while sySynch() halts on it. */
extern volatile u16 VBlankDone;
extern u32 CpuLoadTimer; /* Frames CpuLoadPeak is still held. */
/* The highest CPU load in the last 30 frames: the scanline the frame's work */
/* ended on, plus 256 for each VBlank it overran. */
extern u32 CpuLoadPeak;
/* Frames sySynch() has finished. */
/* volatile: the VBlank pass reads it (fdWorldMapApplyAffinePage picks */
/* the world map's affine page by it) while sySynch() counts it. */
extern volatile u32 FrameCount;
extern u32 VBlankCount; /* Counted up by every VBlank. */
/* A function for the next VBlank alone: the handler clears it, then calls it.
 */
extern void (*VBlankCallback)(void);

/* Nonzero: sySynch() counts no idle time and ignores sleep requests. */
extern volatile u8 SleepDisabled;
/* Sleep after three idle minutes. */
extern volatile u8 IdleSleepEnabled;
/* 1: sleep this frame; more: count down to it. */
/* volatile: sySynch() counts it down and reads it each frame. */
extern volatile u8 SleepRequested;
/* Debug builds: Start+Select pauses the frame loop. */
/* volatile: fdActorUpdatePlayerCtrl and fdSequenceRun each read it */
/* again after an earlier test. */
extern volatile u8 DebugMode;
extern u8 DebugPaused; /* The debug pause is on. */
/* Set by the keypad interrupt; sySynch() performs the soft reset. */
extern u8 ResetRequested;
/* Frames since a key was last held. */
/* volatile: sySynch() counts it and reads it each frame. */
extern volatile u16 IdleFrames;
/* Frames L and R have been held together; three seconds request sleep. */
/* volatile: sySynch() counts it and reads it each frame. */
extern volatile u16 SleepComboFrames;
/* Written by the soft reset just before it restarts the cartridge, and */
/* cleared by syInit; nothing else reads it. */
extern u32 CartridgeResetMarker;
/* The build stamp syCopyBuildStamp copies to IWRAM at power-on: twelve */
/* digits and a zero word, at the bottom of the frame-wait stack. */
extern s32 BuildStampWork[];
extern u32 PlayTimeFrames; /* The play time clock, in frames. */
/* Bytes of the caller's stack sySynch() has copied into SavedStack, or 0. */
extern u32 SavedStackSize;
/* The caller's stack below the frame-wait stack top, while sySynch() runs. */
extern u8 SavedStack[];

/* The input group. The keypad state the VBlank interrupt keeps. */
/* */
/* The VBlank handler reads the keypad into these cells, and */
/* syInputUpdateKeyRepeatAndDirection turns them into repeats after each frame
 */
/* (system/system_frame.c). The first three are volatile because the VBlank */
/* handler sets them while the code it interrupts reads them; the next two the
 */
/* key repeat rebuilds and reads each frame. */
/* Keys down now. */
extern volatile u32 KeysHeld;
/* Keys that went down this frame. */
extern volatile u32 KeysPressed;
/* Pressed keys, repeating while held: after 19 frames, then every 6. */
extern volatile u32 KeysRepeat;
/* KeysRepeat with at most one D-pad direction. */
extern volatile u32 KeysRepeatDirected;
/* The D-pad direction the repeat last settled on: when a second direction is */
/* added, KeysRepeatDirected drops this one and keeps the new one. */
/* KEYS_HORIZONTAL, both left and right, stands for none. */
extern volatile u32 KeysDirection;

/* Kept by the frame loop and the key repeat. */
extern u32 KeysPressedLatch; /* Presses since a reader last cleared it. */
extern u32 KeysHeldLast;	 /* KeysHeld when the frame loop last read it. */
extern u32 KeysHeldAtVBlank; /* KeysHeld at the previous VBlank. */
/* Frames until the held keys repeat; -1 makes them repeat at once. */
extern s32 KeyRepeatTimer;

/* The screen_fade group. The brightness fade of the whole screen that the */
/* VBlank handler steps (system/system_screen_fade.c). */
/* Which way the screen fade goes: 0 towards black, 1 towards white. */
extern u8 ScreenFadeToWhite;
/* The running fade's length in VBlanks, 0 when no fade is running. */
/* volatile: the VBlank handler's syScrFadeUpdate clears it when the fade */
/* ends, while syScrFadeWait waits on it. */
extern volatile u8 ScreenFadeVBlanks;
/* VBlanks until the running fade reaches its target. */
/* volatile: the VBlank handler's syScrFadeUpdate counts it down. */
extern volatile u8 ScreenFadeVBlanksLeft;
/* The BLDCNT first-target layers the screen fade covers (the low six bits). */
extern u16 ScreenFadeLayers;
/* The brightness the running fade started from, 0 to 16. */
extern u8 ScreenFadeStartLevel;
/* The brightness the running fade ends at, 0 to 16. */
/* volatile: syScrFadeStart sets it for the VBlank handler's */
/* syScrFadeUpdate. */
extern volatile u8 ScreenFadeTargetLevel;

/* The io_write_queue group. The register write queue that VBlank carries out */
/* after each finished frame. */
/* The queue IO_WRITE_QUEUE_PUSH and the IoWriteQueue_ writers fill, and */
/* syIoWriteQueueFlushPending empties at VBlank. */
extern IO_WRITE_QUEUE IoWriteQueueWork;

/* The iwram_call group. The IWRAM call layer: the functions the game keeps */
/* resident in IWRAM, each at its fixed address, and one typed inline per */
/* function that every call goes through. */
/* syMulFX's function in IWRAM, which returns through ip. */
extern u8 IwramMulFXReturnIp[];
/* The IWRAM function that clears a run of words, which the OAM build calls by
 */
/* its address. */
extern u8 IwramClearWords[];

/* The math_table group. The trigonometry tables (system/math_table.c): the */
/* sine of a quarter turn and the tangents syAtan searches. */
extern const u16 Math_SinQuarter[256];
extern const u16 Math_TwoPiSteps[8];
extern const s32 Math_QuadrantSin[4];
extern const u16 Math_TanTable[258];

/* The matrix group. The 3D transform and its stack. */
/* The current 3D transform. syMultiMatrix multiplies the matrix it is */
/* given into it. */
extern MATRIX CurrentMatrix;

/* Matrices saved by syPushMatrix, 0 or 1: the save area holds one. */
extern s32 MatrixStackDepth;
/* Where syPushMatrix saves CurrentMatrix next: the save area in heap slot */
/* HEAP_SLOT_MATRIX_STACK, or just past it while a matrix is saved. */
extern u8 *MatrixStackTop;
/* syMatrixBuildLookAt's ARM dot product, seven words it copies onto its */
/* stack to run. */
extern const u32 RelocatedInnerProduct[];

/* The number_text group. The digits and place values the number formatters */
/* write with (system/number_text.c). */
extern const u8 String_HexDigits[];
extern const s32 String_PowersOfTen[9];
/* The number text buffer: syStringFormatHex writes eight digits and */
/* syStringFormatDecimal eleven characters, right-aligned, each with a */
/* terminator. */
extern u8 NumberBuffer[];

/* The projection group. The 3D projection. */
/* The current 3D projection: syPerspective and sySetScreenDist */
/* write its focal length and depth range, and the battle and its battle */
/* effects set its centre directly. */
extern PROJECTION ProjectionWork;

/* The savedata group. The save sectors' signature and header template, and the
 */
/* link test's strings (system/savedata.c). */
extern const u8 Save_Signature[];
extern const u8 Save_HeaderTemplate[8];

extern const u32 Link_TestWord;
extern const u8 Link_TestDataA[];
extern const u8 Link_TestDataB[];

/* The serial group. The link cable's runtime: serial packets, registers and */
/* transfer state. */
/* This console's block transfer state, sent to the partners each VBlank. */
extern SERIAL_TRANSFER_STATE SerialTransfer;
extern SERIAL_STATE SerialStateWork;
/* Set from sySerialInit while the consoles exchange packets every frame. */
extern u16 SerialExchangeActive;
/* What the send in progress sends from, 0 when none is running. */
extern s32 SerialSendSource;
/* The bytes the send in progress has left to send. */
/* volatile: sySerialStartSend sets it while sySerialStepBlockTransfer, which */
/* the VBlank handler runs, counts it down. */
extern volatile u16 SerialSendSize;
/* Where the receive in progress writes, 0 when none is running. */
extern s32 SerialReceiveDestination;
/* The bytes the receive in progress has taken so far. */
/* volatile: sySerialStepBlockTransfer, which the VBlank handler runs, counts it
 */
/* up while the link code outside the interrupt polls it. */
extern volatile u16 SerialReceivedSize;
extern SERIAL_TRANSFER_STATE SerialPeerPayloads[4];
/* The number of the block being sent or received, from 0 at each transfer. */
/* volatile: sySerialStartSend and sySerialStartReceive reset it while */
/* sySerialStepBlockTransfer, which the VBlank handler runs, steps it. */
extern volatile u8 SerialBlockNumber;
/* Cleared as the link starts and as the Battle Arena trades parties; nothing */
/* reads it. */
extern u8 LinkExchangeState;
/* Link status, updated by the VBlank exchange handler. */
/* volatile: the VBlank handler writes it while sySerialWaitForStatMask and the
 */
/* link screens poll it. */
extern volatile u16 LinkStatusWork;

/* The vram_cache group. The sprite VRAM block cache and its owners. */
/* The cache entry that owns each block, or VRAM_BLOCK_OWNER_FREE. */
extern u8 VramCacheBlockOwners[VRAM_BLOCK_COUNT];

/* The cache entries by number: the slot a sprite or loaded picture holds. */
extern VRAM_CACHE_ENTRY VramCacheEntries[];

#endif
