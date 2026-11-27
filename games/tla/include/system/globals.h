#ifndef ALCHEMY_GLOBALS_H
#define ALCHEMY_GLOBALS_H

/* The system library's variables, as SGL keeps its system variables and work
   areas: each group's variables in IWRAM, EWRAM and ROM. system.h includes it;
   a file built by another compiler can include it with system/definitions.h. */

#include "types.h"
#include "fixed_math.h"
#include "edition.h"
#include "static_assert.h"
#include "system/definitions.h"

/* The structures of other parts that the system library's RAM holds, named here
   by their tags; each part's own header completes its type. */
struct DISPLAY_SCROLL_OBJECT;
struct FlashType;
struct MusicPlayerInfo;
struct MusicPlayerTrack;
struct SoundInfo;
struct SoundVBlankState;

/* The event group. Events, their priorities and the event table, with room for
   24 events. 1 while sySynch() runs the tick pass's events
   (EVENT_PASS_TICK). */
extern volatile Uint8 EventsRunning;
extern Uint8		  EventsInitialized; /* 1 once the event table is set up */
/* The event table, run in priority order by each pass of sySynch(). */
extern EVENT EventBuf[EVENT_COUNT];

/* The irq group. The interrupt sources, their handler table and the resident
   IWRAM code the interrupts enter through. The handler the IWRAM dispatcher
   calls for each interrupt source. */
extern INTERRUPT_HANDLER IrqHandlers[IRQ_COUNT];

/* The handlers installed at power-on, copied into IrqHandlers. */
extern const INTERRUPT_HANDLER DefaultIrqHandlers[IRQ_COUNT];

/* The interrupt dispatcher at the start of the IWRAM-resident code, whose other
   functions are reached at fixed offsets from it (the iwram_call group). */
extern Uint8 syIWRAMIRQMain[];
/* Where the IWRAM-resident code lies in ROM (ld_script.ld): 4 KB of it is
   copied to syIWRAMIRQMain at power-on. */
extern const Uint8 IwramResident_Rom[];
/* The BIOS's interrupt vector, pointed at syIWRAMIRQMain at power-on. */
extern void *IrqVector;

/* The heap_slots group. Two EWRAM buffers the battle engine shares with the
   field engine and the link screens. 16 KB of EWRAM at 0x02018000, where the
   battle's actor sprite buffers start, 16 KB a slot; the map's metatiles fill
   it while a map is loaded. */
extern Uint8 ActorSpriteSlots[];

/* 24 bytes a peer: where a link battle reads its partner's signature. */
extern Uint16 LinkPeerSignatures[][12];

/* The affine group. Sprite rotation and scaling: a sprite's transform and the
   OAM matrices this tick hands out for it. Matrices handed out this tick
   (syOAMMatrixAllocFromTransform), up to 32; syOAMResetTick sets it back to 0
   after each VBlank. */
extern Uint8 OamMatrixCount;
/* This tick's 32 matrices, copied into OAM with the sprites. */
extern AFFINE_MATRIX OamMatrices[];

/* The bg_scroll group. The four backgrounds' scroll offsets, written to the
   hardware each VBlank. BG0 to BG3's scroll offsets, which the VBlank handler
   copies to REG_BG0HOFS to REG_BG3VOFS in the VBlank after sySynch() sets
   TickRenderPending. */
extern BG_SCROLL BgScrollWork[];

/* The decompress group. The decoders and other ARM code the system library
   copies to RAM to run (RAM_CODE_LOAD). Such a function is declared as an array
   of bytes, and a symbol ending in CodeSize is the length of the copy: no
   object, but an address the linker sets equal to the length, which the loader
   reads by casting it to a number. syMatrixBuildLookAt's dot product, a Thumb
   entry and its ARM code: eight words it copies onto its stack to run. */
extern const Uint32 RelocatedInnerProduct[];
/* The tagged LZ decoder in ARM, run by syDecompressTaggedLZ from a heap
   copy. */
extern const Uint8 syDecompressTaggedLZArmCode[];
/* The size of syDecompressTaggedLZArmCode, which syDecompressTaggedLZ copies to
   the heap. */
extern Uint8 syDecompressTaggedLZCodeSize[];
/* The move-to-front LZ decoder's command loop, run by syDecompressMoveToFrontLZ
   from a heap copy. */
extern const Uint8 syDecompressMoveToFrontLZCommandsCode[];
/* The size of syDecompressMoveToFrontLZCommandsCode, which
   syDecompressMoveToFrontLZ copies to the heap (ld_script.ld). */
extern Uint8 Decompress_MoveToFrontLzCommandsCodeSize[];
/* The byte LZ decoder, run by syDecompressByteLZ from a heap copy. */
extern const Uint8 Resource_DecodeByteLzCode[];
/* The size of Resource_DecodeByteLzCode, which syDecompressByteLZ copies to the
   heap. */
extern Uint8 Resource_DecodeByteLzCodeSize[];
/* The colour index fdRenderDecodePicture paints every pixel of a layer drawn
   with SPRITE_COLOR_FILL; the sprite viewer sets it to 3. */
extern Uint8 DecodeFillByte;

/* The stub group. The system module's stub table. The stub table of the system
   library's functions, which the world map's tilt reads for its arctangent. */
extern const SYSTEM_STUB_TABLE Stubs_System;

/* The flash group. The SDK's flash library keeps its state in EWRAM
   (sdk/flash.h, sdk/flash_data.h); one of its metatiles has a name here too.
   The metatiles of the SDK's flash library from WaitForFlashWriteWork on, under
   a label of their own: the credits build a backdrop's grey colours there while
   no flash operation runs (mnCreditsLoadBackground). */
extern Uint16 Credits_GreyColors[];

/* The tick group. The tick loop's globals: the frames a tick takes, the CPU
   load display and the VBlank flags, which sySynch() (system/system_frame.c)
   and the VBlank handler share. The build stamp syCopyBuildStamp copies to
   IWRAM at power-on: twelve digits and a zero word, at the bottom of the
   tick-wait stack. */
extern Sint32 BuildStampWork[];
/* The VBlank handler copies the OAM list only while it is set; scenes clear it
   to keep their own sprites on screen. */
extern Uint8 OamCopyEnabled;
/* Set by sySynch() once it has built a tick's OAM list; the VBlank handler
   then copies the list, the BG scroll and the queued register writes, and
   clears it. */
extern Uint8 TickRenderPending;
/* Debug: sySynch() measures the CPU load into CpuLoadPeak when set. */
extern Uint8 CpuLoadDisplayEnabled;
/* Frames since sySynch() last started a tick: 1 when the tick took one. */
extern Uint16 TickFrames;
/* TickFrames as the last tick left it; the text typing paces itself by
   it. */
extern Uint16 LastTickFrames;
extern Uint32 CpuLoadTimer; /* ticks CpuLoadPeak is still held */
/* The highest CPU load in the last 30 ticks: the scanline the tick's work
   ended on, plus 256 for each VBlank it overran. */
extern Uint32 CpuLoadPeak;
/* Nonzero: sySynch() counts no idle time and ignores sleep requests. */
extern volatile Uint8 SleepDisabled;
/* Sleep after three idle minutes. */
extern volatile Uint8 IdleSleepEnabled;
/* Ticks since a key was last held. */
extern volatile Uint16 IdleTicks;
/* 1: sleep this tick; more: count down to it. */
extern volatile Uint8 SleepRequested;
/* Debug builds: Start+Select pauses the tick loop. */
extern Uint8 DebugMode;
extern Uint8 DebugPaused; /* the debug pause is on */
/* Set by the keypad interrupt; sySynch() performs the soft reset. */
extern Uint8 ResetRequested;
/* Written by the soft reset just before it restarts the cartridge, and cleared
   by syInit; nothing else reads it. */
extern Uint32 CartridgeResetMarker;
/* Ticks sySynch() has finished. */
extern Uint32 TickCount;
extern Uint32 PlayTimeTicks; /* the play time clock, in ticks */

extern Uint32 FrameCount; /* counted up by every VBlank */
/* volatile: the VBlank handler sets it while syWaitForVBlankInterrupt waits on
   it, so it is no device. Bit 0 is set by the VBlank handler;
   syWaitForVBlankInterrupt clears it and halts until it is set again. */
extern volatile Uint16 VBlankDone;

/* The heap_release group. The heap's allocators and releases
   (system/system_heap.c). The EWRAM heap's space, 32 KB from 0x02030000. */
extern Uint8 EwramHeap[];
/* The IWRAM heap's space, from 0x03001300 up to Ram_IwramHeapEnd. */
extern Uint8 IwramHeap[];

/* The input group. The keypad state the VBlank interrupt keeps. */
extern INPUT_STATE PerWork;

/* The io_write_queue group. The I/O write queue: register writes held until
   VBlank. The queue IO_WRITE_QUEUE_PUSH and the IoWriteQueue_Store writers
   fill, and syIOWriteQueueFlushPending empties at VBlank. */
extern IO_WRITE_QUEUE IoWriteQueueWork;

/* The math_table group. The arctangent table. Tangents in 8.8 fixed point over
   a quarter turn in 256 steps, which syAtan searches. */
extern const Uint16 MathTanTable[258];

/* The matrix group. The current 3D matrix, its one-deep stack, and the unit
   matrix. The current 3D transform. syMultiMatrix multiplies the matrix it is
   given into it. */
extern MATRIX CurrentMatrix;

/* Matrices saved by syPushMatrix, 0 or 1: the save area holds one. */
extern Sint32 MatrixStackDepth;
/* Where syPushMatrix saves CurrentMatrix next: the save area in heap slot
   HEAP_SLOT_MATRIX_STACK, or just past it while a matrix is saved. */
extern Uint8 *MatrixStackTop;

/* The oam group. The tick's OAM list. The OAM entries the tick's list holds
   and the most it may; sySynch() empties it each tick. */
extern OAM_USAGE OamUsageWork;

/* The projection group. The 3D projection the battle engine and the field
   engine draw with (syPerspective, syConvert3Dto2D). The projection the battle
   engine and the field engine draw with (IWRAM). */
extern PROJECTION ProjectionWork;

/* The random group. The battle engine's own random numbers. gmBattleRandom's
   seed, apart from syRandom's so the battle's rolls repeat on both consoles of
   a link battle: it starts at 0 there, and each turn the parent (link side 0)
   copies RandomSeed into it and sends it to the child. */
extern Sint32 BattleRandomSeed;

/* The ram_buffer group. EWRAM's and IWRAM's buffers at fixed addresses, named
   by their address where a function reaches them so (FIXED_ADDRESS, with the
   linked symbol beside it), and the scene's own state. The map's shape grid, a
   byte a metatile, which fdMapGridLoadMap decodes from the map header's shapes
   and the field's actor metatiles read. */
extern Uint8 MapShapeGrid[];

/* The layout's metatile tiles, 32 KB of EWRAM at 0x02020000: each metatile's
   tiles 8 bytes, its top two tile entries, then its bottom two.
   fdMapGridLoadMap decodes them in. */
extern Uint32 MetatileTiles[];

/* Called once by the next VBlank, which clears it first; the world map sets it
   to show its next picture. */
extern void (*VBlankCallback)(void);

/* A scene's own state in EWRAM, which each scene lays out as it needs. */
extern Sint8 SceneLocalState[];
/* 120 more bytes of a scene's own state in EWRAM, which a scene that keeps one
   lays out under a name of its own. */
extern Uint8 SceneStateWork[];

/* The game's state (MAIN_ROUTINE_STATE). */
extern MAIN_ROUTINE_STATE MainRoutineStateWork;
/* MainRoutineStateWork from its summonMask on, under a label of its own: the
   party's DJINN_RECOVERY_TABLE, which gmDjinnGetRecoveryTable returns. Lucky
   Wheels reads its won prizes 320 bytes in (REEL_SAVE). */
extern Uint8 PartyDjinnRecovery[];
/* The game flags, 4096 bits in 512 bytes, saved with the game: flag n is bit n
   & 7 of byte GAME_FLAG_BYTE(n). */
extern Uint8 GameFlagBytes[];
/* The party's inventories, shortcuts and order as gmInventorySnapshotSave puts
   them aside for the Trial Road, with spare room after them. */
extern Uint8 InventorySnapshot[];
/* How many of each rare item the shops may still sell, 0 to 99, by counter
   number (ITEM_COUNTER_COUNT of them; ItemRareItemCounters,
   gmRareItemStockAdjust). */
extern Uint8 RareItemStock[];

/* 32 KB of EWRAM holding background tiles, such as the world map's char blocks
   2 and 3. */
extern Uint8 BgTileBuffer[];
/* The map's shape table, 16 KB of EWRAM: 4-byte entries that a map metatile's
   shape indexes. */
extern Uint8 MapGridShapes[];
/* 16 KB of EWRAM that pictures and tiles are decoded into before they are
   copied on. */
extern Uint8 DecodeBuffer[];
/* The tileset animations' tiles, 16 KB of EWRAM. */
extern Uint8 MapGridAnimTiles[];
/* The map's scene data in EWRAM, loaded from its resource: its entrance stream,
   then from the halfword offsets after it its edits and its bounds. */
extern Uint8 MapScene[];
/* The scene data's halfword offset of its edits, from MapScene. */
extern Uint8 MapSceneEditsOffset[];
/* The scene data's halfword offset of its bounds, from MapScene. */
extern Uint8 MapSceneBoundsOffset[];

/* Where each of `The Lost Age`'s pictures is unpacked before it is copied into
   sprite tiles. */
extern Uint8 TitleIntroLogoTileBuffer[];

/* The battle effects' blocks in the map metatile buffer, which they borrow
   while no map is loaded. The map metatile buffer from its second 16 KB on,
   which some effects borrow as scratch: no map is loaded during a battle. */
extern Uint8 EffectScratch[];
/* Nirvana's mesh's end, after 112 faces. */
extern Uint8 SeracMeshEnd[];
/* Inside EffectScratch: FLORA_WORK from its second tile number on, read at its
   fixed place. */
extern Uint16 FloraPetalTiles[];
/* Inside EffectScratch: FLORA_WORK.pal, the object palette as it fades. */
extern Uint16 FloraSpritePalettes[];
/* In the map metatile buffer: Haures's mound's faces, eight bytes a quad. */
extern Uint8 HauresMoundStrip[];
/* The face after Haures's mound's last, which ends the strip. */
extern Uint8 HauresMoundStripEnd[];
/* In the map metatile buffer: each of Haures's mound corners' shade. */
extern Uint8 HauresMoundShades[];
/* In the map metatile buffer: the tiles Iris draws its sprites from. */
extern Uint8 IrisTileBuffer[];

/* The world map's and the map screen's state in EWRAM. The map screen's state
   while it is open. */
extern WORLD_MAP_STATE WorldMapStateWork;
/* The destination last chosen on the world map (WORLD_MAP_CHOICE). */
extern WORLD_MAP_CHOICE MapScreenChoice;
/* The world map camera's yaw, in 1/65536 turns, the scanline parameters were
   built for; the vertex update rebuilds them when it changes. */
extern Uint32 ScanlineAngle;
/* The world map camera's pitch the scanline parameters were built for. */
extern Uint32 ScanlinePitch;
/* Never set (EWRAM, zero from boot). Set, the map screen would list only the
   places before the first end of the place table, not the second list from
   entry 128 on, and show each place's second marker (the high nibble of its
   value). XXX (no evidence): what the switch was for; no edition, debug room or
   other code writes it, so the game never shows it set. */
extern Sint16 MapScreenFirstListOnly;
/* Where the map screen's cursor glides to on its own, 16.16; 0 when it does
   not. */
extern Sint32 MapScreenGlideTargetX;
/* As MapScreenGlideTargetX, the second coordinate. */
extern Sint32 MapScreenGlideTargetY;
/* Nonzero while the map screen is zoomed in. */
extern Sint16 MapScreenZoomed;
/* Set while the place name waits to be drawn again. */
extern Sint16 MapScreenNameRedrawPending[];
/* Ticks before the place name is drawn again. */
extern Sint16 MapScreenNameRedrawDelay[];
/* The map screen's cursor marker. */
extern WORLD_MAP_MARKER MapScreenCursorMarker;
/* The world map cursor's position (x, then y, 16.16 pixels). */
extern FIXED *MapScreenCursor;
/* Where the cursor goes back to when Start is let go: the place R or L last
   stepped to, 16.16 pixels. */
extern Sint32 MapScreenCursorStartX;
extern Sint32 MapScreenCursorStartY; /* as MapScreenCursorStartX */
/* The map screen's scroll as Select toggles the zoom, 16.16 pixels. */
extern Sint32 MapScreenScrollX;
extern Sint32 MapScreenScrollY; /* as MapScreenScrollX */

/* The credits' state in EWRAM. Slides the crawl has released for the slideshow
   to show next; the slideshow waits while there are none. */
extern Sint16 CreditsPendingSlides;
/* The credits' sprite buffer, copied to OAM each tick: the border strips'
   sprites, then the text's. */
extern struct DISPLAY_SCROLL_OBJECT *DisplayScrollObjects;
/* 0 while the credits text scrolls; once a line stops it, the line's stop (1 at
   a page mark, which A or B releases), then counted up each tick after the end
   (CREDITS_TICKS_AFTER_END). */
extern Sint16 DisplayScrollBusy;
/* DisplayScrollPosition when the credits last drew a line. */
extern Sint16 DisplayScrollLastPosition;
/* How far the credits text has scrolled, in pixels: one every four frames,
   faster while B is held. */
extern Uint16 DisplayScrollPosition;
/* The scrolling image's state. mnEndingImageScroll, a VBlank event, moves the
   scroll toward its target while mnEndingImageWaitForScroll waits for it to
   arrive. */
extern Sint16 EndingImageBlock;		 /* which character block the image is in */
extern Sint16 EndingImageFadeFrames; /* frames of the fade-in, to 64 */
extern Uint32 EndingImageMapScroll;	 /* the tilemap's scroll, 16.16 pixels */
extern volatile Sint32 EndingImageScroll; /* the scroll, 16.16 pixels */
/* The row a scene asks for (mnEndingImageSetTargetRow); Row moves toward it. */
extern Sint16 EndingImageTargetRow;
extern Sint16 EndingImageRow;	/* the row the scroll is heading for */
extern Sint16 EndingImageSpeed; /* the most rows a tick moves */
extern Uint8 *EndingImageMap;	/* the image's map */
/* The 24 sprites of the image's border strips. */
extern void *EndingImageStrips;

/* The File Screen's options in EWRAM. Nonzero offers Battle on the File Screen,
   when a file has any game flag counted in its summary
   (mnSaveScanFileSummaries). */
extern Sint16 TitleBattleOptionEnabled;
/* Nonzero offers Update on the File Screen, when a file has a party whose
   summary is not marked reunited (mnSaveScanFileSummaries). */
extern Sint16 TitleUpdateOptionEnabled;

/* The save group. The save in flash: its sectors, parts and summaries, and the
   save's state. The current save file, or -1 for none (a new game). */
extern Sint16 BupSlot;
/* 1 when a save file on the cartridge holds a cleared game, as the save screen
   finds on loading the summaries; a new game keeps it in
   MainRoutineStateWork.clearedFileAtStart. */
extern Sint16 AnyFileCleared;
/* The buffer a save file is built in and written from, in two halves; its
   summary's 16-byte header lies just before it. */
extern Uint8 BupBuffer[];

/* The bupdata group. The save signature and sector header template, and the
   link test strings nothing reads (system/bupdata.c). */
extern const Uint8 BupSignature[];
extern const Uint8 BupHeaderTemplate[8];

extern const Uint32 LinkTestWord;
extern const Uint8	LinkTestDataA[];
extern const Uint8	LinkTestDataB[];

/* The scene group. The running scene's overlay area, a field position, and the
   area name a scene and entrance show. The running scene's overlay, loaded into
   this EWRAM at each scene change; its code opens with its entry table. */
extern void *OverlayArea[];

/* The serial group. The link cable's runtime: serial packets, registers and
   transfer state. This console's block transfer state, exchanged with the
   partner's each VBlank (sySerialExchangePayloads). */
extern SERIAL_TRANSFER_STATE SerialTransfer;
/* The link cable's runtime (SERIAL_RUNTIME), which sySerialInit clears. */
extern SERIAL_STATE SerialStateWork;
/* Nonzero while the VBlank handler exchanges packets with the partner each
   tick; sySerialInit sets it. */
extern Uint16 SerialExchangeActive;
/* The data a block send reads from, 0 when none is under way
   (sySerialStartSend). */
extern Sint32 SerialSendSource;
/* Bytes of the block send still to go: sySerialStartSend sets it and
   sySerialStepBlockTransfer, which the VBlank handler runs, counts it down. */
extern volatile Uint16 SerialSendSize;
/* Where a block receive writes, 0 when none is under way
   (sySerialStartReceive). */
extern Sint32 SerialReceiveDestination;
/* Bytes of the block receive taken so far: sySerialStepBlockTransfer, which the
   VBlank handler runs, counts it up while the link code polls it. */
extern volatile Uint16 SerialReceivedSize;
/* Each console's SERIAL_TRANSFER_STATE as last received, 24 bytes each. */
extern SERIAL_TRANSFER_STATE SerialPeerPayloads[4];
/* The block this console sends or expects next (SERIAL_BLOCK_NUMBER_MASK):
   sySerialStartSend and sySerialStartReceive reset it and
   sySerialStepBlockTransfer, which the VBlank handler runs, steps it. */
extern volatile Uint8 SerialBlockNumber;
/* Cleared by sySerialInit and by scLinkLobbyExchangePartyRecords; nothing reads
   it. */
extern Uint8 LinkExchangeState;
/* Link status, written by the VBlank exchange handler while
   sySerialWaitForStatMask and the link screens poll it. */
extern volatile Uint16 LinkStatusWork;

/* The screen_fade group. The screen fade's state, which syScreenFadeUpdate
   steps from the VBlank handler. Which way the screen fade goes: 0 towards
   black, 1 towards white. */
extern Uint8 ScreenFadeToWhite;
/* volatile: syScreenFadeUpdate, which the VBlank handler runs, clears it at the
   fade's end while syScreenFadeWait waits on it, so it is no device. the fade's
   length in frames, 0 when idle */
extern volatile Uint8 ScreenFadeFrames;
/* volatile: the fade starts set it for syScreenFadeUpdate, which the VBlank
   handler runs and which counts it down, so it is no device. Frames until the
   running fade reaches its target. */
extern volatile Uint8 ScreenFadeFramesLeft;
/* The BLDCNT first-target layers the screen fade covers (the low six bits). */
extern Uint16 ScreenFadeLayers;
/* The brightness the running fade started from, 0 to 16. */
extern Uint8 ScreenFadeStartLevel;
/* volatile: syScreenFadeStart sets it for syScreenFadeUpdate, which the VBlank
   handler runs, so it is no device. the brightness the fade ends at */
extern volatile Uint8 ScreenFadeTargetLevel;

/* The sound group. The game's sound state in EWRAM and IWRAM; the sound
   driver's own work is declared in sdk/sound_driver.h. The sound driver's state
   as the tick wait runs it. */
/* XXX: why the tick wait reads the sound driver's state as bytes rather than
   through struct SoundInfo, whose transferCountdown is volatile; whether the
   driver's own struct should make that member volatile would settle the type.
   Where the sound driver finds its state (SOUND_INFO_PTR): a pointer to
   SoundInfoWork, whose bytes the tick wait reads. */
extern Uint8 *SoundInfoPointer;
/* Plays music; SOUND_FADE_OUT_BGM fades it and a jingle silences it until it
   ends. */
extern struct MusicPlayerInfo MPlayInfo_BGM;
/* Plays the field's and the battle's action sounds (MUSIC_PLAYER_ACTION_SE);
   SOUND_FADE_OUT_ACTION_SE fades it. */
extern struct MusicPlayerInfo MPlayInfo_ActionSE;
/* One of the sound effects' shared players (MUSIC_PLAYER_SE5); both fades,
   SOUND_FADE_OUT_ACTION_SE and SOUND_FADE_OUT_EXIT_SE, fade it. */
extern struct MusicPlayerInfo MPlayInfo_SE5;
/* Plays jingles; the music returns once it stops. */
extern struct MusicPlayerInfo MPlayInfo_Jingle;
/* The mixer's PCM output: SoundDriver_EnterVBlankUpdate hands it to
   IwramSoundMixEntry, and sySndSetFixedSampleFreq clears it and points DMA 1 at
   its start (the right speaker) and DMA 2 at its second half (the left). */
extern Uint8 MixerPcmBuffer[];
/* The sound mixer's workspace at the top of IWRAM; the IWRAM heap ends where it
   starts (syHeapGetRemainingIWRAM). */
extern Uint8 IwramSoundMixWorkspace[];
/* Set by the resident mixer whenever it clips a sample: when a sum overflows,
   IwramSoundNormalizeChannels saturates the samples and stores the clip mask
   (0x80008000) here. Only the sound test reads it. */
extern Sint32 SoundMixerClipped;
/* volatile: the VBlank handler and the tick wait share it. Who runs the sound
   update this tick: the tick wait or the VBlank handler. */
extern volatile struct SoundVBlankState SoundVBlankStateWork;
/* The music playing, which sySndPlay sets from each music id it starts. */
extern Uint16 CurrentBgm;
/* One bit for each sound id, set once the sound has played (sySndFlagSet); the
   link lobby offers only the tunes heard. */
extern Uint8 SoundFlags[];
/* The sound effect from SOUND_SE_FIRST up each player last started, by player
   (MUSIC_PLAYER_COUNT of them); SOUND_FADE_OUT_ACTION_SE and
   SOUND_FADE_OUT_EXIT_SE clear the entries they fade. */
extern Uint16 MusicPlayerLastSound[];
/* Nonzero once SOUND_FADE_OUT_BGM has started the music's fade; further fade
   requests are ignored until the next music clears it. */
extern Uint8 BgmFadeRequested;
/* The music's volume, 8.8 fixed point (BGM_UNITY_SCALE is full), ramped toward
   BgmVolumeTarget by BgmVolumeStep each VBlank. */
extern Uint16 BgmVolume;
/* The volume BgmVolume ramps toward. */
extern Uint16 BgmVolumeTarget;
/* How far BgmVolume moves each VBlank (BGM_DEFAULT_RAMP_STEP). */
extern Uint16 BgmVolumeStep;
/* Frames after a jingle starts before the music waits for its end; held at 1
   until the jingle's player stops, then 0. */
extern Uint8 BgmRestoreDelay;
/* The tempo and pitch BgmTempoPitch ramps toward. */
extern Uint16 BgmTempoPitchTarget;
/* The music's combined tempo and pitch, 8.8 fixed point (BGM_UNITY_SCALE is the
   song's own), ramped toward BgmTempoPitchTarget by BgmTempoPitchStep each
   VBlank. */
extern Uint16 BgmTempoPitch;
/* How far BgmTempoPitch moves each VBlank. */
extern Uint16 BgmTempoPitchStep;
/* Set by sySndUpdateCommandMask; nothing reads it. */
extern Uint8 SoundCommandMask;
/* An EWRAM byte sySndInit clears and nothing reads. */
extern Uint8 Sound_UnreadByte;

/* The vram_cache group. The VRAM cache's entries and the blocks they own. The
   cache entry that owns each block, or VRAM_BLOCK_OWNER_FREE. */
extern Uint8 VramCacheBlockOwners[VRAM_BLOCK_COUNT];
/* The cache entries by number: the slot a sprite or loaded picture holds. */
extern VRAM_CACHE_ENTRY VramCacheEntries[];

#endif
