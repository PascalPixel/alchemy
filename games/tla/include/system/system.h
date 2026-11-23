#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

/// @file
/// The system's routines and globals: interrupts, BIOS sound bias, random
/// numbers and sleep, the save slot, the debug text, and the system bank's
/// other routines.

#include "edition.h"
#include "fixed_math.h"
#include "types.h"
#include "system/heap_release.h"

void wait(s32 frames);
void System_WaitForFrameInterrupt(void);
void Oam_ResetFrame(void);
/// The BIOS's SoundBias: Off ramps the sound output's bias down before the
/// console sleeps, On back up once it wakes, so the speaker does not click.
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
u32 Random16(void);
/// A random number from 0 to n - 1: n times a 16-bit random draw, over 65536.
#define Random_Below(n) (Random16() * (n) >> 16)
/// Random16's seed; Lucky Wheels seeds it with the play time, and a link
/// battle's parent sends it as the turn's gBattleRandomSeed.
extern u32 gRandomSeed;

/// 1 while the CPU is stopped for sleep, until L and R wake it.
extern u16 gSleepActive;

struct FlashType;
struct FlashSetupInfo;
/// The Atmel chip's setup, one of the flash library's chips (gFlash_Chips).
extern const struct FlashSetupInfo gFlash_ChipAtmel;
/// The setup IdentifyFlash gives a chip whose id it does not know, the last of
/// gFlash_Chips.
extern const struct FlashSetupInfo gFlash_ChipDefault;
#ifndef OWN_gFlash
extern const struct FlashType *gFlash;
#endif
#ifndef OWN_gFlashSavedIme
extern u16 gFlashSavedIme;
#endif
#ifndef OWN_gFlashTimeoutFlag
extern u8 gFlashTimeoutFlag;
#endif
/// The hardware timer the flash library times its waits with
/// (SAVE_FLASH_TIMER).
extern u8 gFlashTimerNum;
/// The end of String_FormatDecimal's digits: DebugText_PrintDecimal prints the
/// last places before it.
extern u8 gNumberTextDecimalEnd[];
/// The end of String_FormatHex's digits: DebugText_PrintHex prints the last
/// places before it.
extern u8 gNumberTextHexEnd[];
/// The link battle timer's heading: `TIME`; `TMPO.` in Spanish, `TEMPS` in
/// French.
extern u8 gLink_TimeLabelString[];
/// Each resource's ROM address by resource id (Resource_GetTableEntry).
extern void *Resource_DirectoryTable[];
/// The size of Resource_PatchThumbBranch (decompress_tagged_lz.s).
extern u8 Resource_PatchThumbBranchCodeSize[];
/// Five tables that map a picture's 256 colour indices onto a sprite's palette,
/// one for each palette a field sprite can take (field/sprite_color_tables.c).
extern u8 gResourceSlot_ConversionTables[][256];
/// The window font's 16 colours, loaded as BG palette 15
/// (Window_LoadGraphics).
extern const u16 gWindow_Palette[];
s32 SpritePictureCache_FindByTag(s32 key);
#ifndef OWN_CountRemainingErasedFlashBytes
s32 CountRemainingErasedFlashBytes(u8 *ptr);
#endif
s32 TilesetAnim_ResumeAllFar(void);
s32 TilesetAnim_SuspendAllFar(void);
void DebugText_Print(const u8 *src);
#ifndef OWN_StartFlashTimer
void StartFlashTimer(u8 index);
#endif
void PartyStatus_Close(void);

struct BattleObject;
struct Actor;

/// The party status window's layout (struct PartyStatusWindow), the block of
/// heap slot HEAP_SLOT_PARTY_STATUS.
extern u8 gPartyStatusWork[];
/// Two 16-entry colour maps for 4bpp pixels: the first draws colour 1 as
/// colour 10, the second leaves every colour as it is.
extern u16 gGraphics_ExpandNibbleTable[];
/// The ARM routine that resolves a loaded overlay's long branches, run from a
/// heap copy by Overlay_Load.
extern const u8 Resource_PatchThumbBranch[];
/// The compiler library's call through r3, which the IWRAM-resident code's
/// callers use to run code copied to RAM.
s32 _call_via_r3(s32, u8 *, u8, s32);
struct SpriteAnimation;
struct SpriteAnimation *SpriteAnimation_Alloc(s32 id);
#ifndef OWN_BattleMotion_ApproachTarget
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travelFrames,
                                 s32 initialVelocityY);
#endif
void LuckyDice_Run(void);
/// An empty function, reached only through an unnamed veneer in the object
/// bank's far-call table.
void Actor_NoOp(void);
void Overlay_Load(u32 index, void *destination);
s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index);
void SpriteAnimation_Clear(void *destination);
void String_FormatHex(u32);

/// The Game Pak interrupt's handler, in IWRAM: stops the console for good once
/// the cartridge is pulled (the BIOS's Stop, again after any wake).
void Iwram_Halt(void);

/// The cartridge's language, from its header (System_GetLanguage).
enum CartridgeLanguage {
  CARTRIDGE_LANGUAGE_ENGLISH = 1,
  CARTRIDGE_LANGUAGE_GERMAN,
  CARTRIDGE_LANGUAGE_FRENCH,
  CARTRIDGE_LANGUAGE_ITALIAN,
  CARTRIDGE_LANGUAGE_SPANISH
};

s32 Stub_ReturnZero(s32 resourceId, const u8 *table);

struct IoWriteQueue;

/// The routine that carries out `count` queued register writes
/// (Resource_ExecuteTransferCode, run from a copy on the stack).
typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

/// A task's routine as the task passes call it: returning a value, where the
/// task itself returns none (Task_RunPass's UB).
typedef s32 (*TaskIntFunc)(void);

/// The dot product of (ax, ay, az) and (bx, by, bz), in 16.16
/// (RelocatedInnerProduct, run from a copy on the stack).
typedef s32 (*DotProductRoutine)(s32 ax, s32 bx, s32 ay, s32 by, s32 az,
                                 s32 bz);

/// Sequence ids above the scene table's rows, as The Broken Seal's
/// SEQUENCE_ values: the sequences other than the field sequence, which
/// MainRoutine_Run runs in place of a scene.
enum {
  /// The Super Lucky Dice sequence (SuperLuckyDice_Run); Contigo's dealer
  /// leaves for it. The Broken Seal's 507, SEQUENCE_NONE, runs nothing.
  SEQUENCE_SUPER_DICE = 507,
  SEQUENCE_SLOTS = 508, ///< the slots sequence (LuckyWheels_Run)
  SEQUENCE_DICE = 509,  ///< the dice sequence (LuckyDice_Run)
  SEQUENCE_BATTLE = 510 ///< a battle; the entrance is the formation
};

/// MainRoutine_Run's mode: where the game starts. Only GAME_START_TITLE is
/// passed (System_Initialize, Game_RestartQuest); the others are its debug
/// starts.
enum GameStart {
  GAME_START_TITLE = 0,                  ///< the Title, with the work cleared
  GAME_START_DEBUG_VENUS_LIGHTHOUSE = 1, ///< SCENE_VENUS_LIGHTHOUSE_ENTRANCE
  GAME_START_DEBUG_CLEAR = 2,            ///< SCENE_CLEAR
  GAME_START_DEBUG_MT_ALEPH = 3,         ///< SCENE_MT_ALEPH_SUMMIT
  GAME_START_DEBUG_TITLE = 4             ///< SCENE_TITLE from entrance 8
};

/// A halfword in a record of its own, which Serial_StepBlockTransfer keeps its
/// constant 1 in.
struct SerialHalfword {
  u16 value;
};

void Window_LoadGraphics(void);
void DebugText_Clear(void);

void VramCache_Reset(void);

void Matrix_RotateY(s32 angle);

s32 Trig_Atan(s32 x, s32 y);

void ScreenFade_ToBlack(s32 vblanks);
void ScreenFade_Wait(void);

void Matrix_SetRotateY(s32 angle);
void Matrix_RotateX(s32 angle);
void Matrix_TranslateVector(const FIXED *position);
void Projection_Set(u32 focal, u32 nearDepth, u32 farDepth);
/// The far depth well past the battlefield: the largest whole 16.16 number.
#define BATTLE_FAR_DEPTH toFIXED(32767)
void Matrix_Translate(FIXED x, FIXED y, FIXED z);
void Matrix_ScaleUniform(FIXED scale);
void Matrix_Scale(FIXED x, FIXED y, FIXED z);
void Matrix_RotateZ(s32 angle);
void Matrix_Pop(void);

void Palette_LoadBasicSpriteColors(void);

void Resource_ReservedNoOp(s32 unused);

void Vector_AddPolarOffset(FIXED radius, s32 angle, FIXED *position);

void Oam_AddSpriteAtDepthClamped(void *entry, s32 slot);

void Decompress_Halfwords(const void *source, void *destination);

void ScreenFade_FromBlack(s32 vblanks);

s32 Decompress_MoveToFrontLz(const void *source, void *destination);

s32 Palette_GetBasicSpriteColors(void);

void Input_ResetKeyRepeat(void);

void ScreenFade_ToWhite(s32 vblanks);
void ScreenFade_FromWhite(s32 vblanks);

void Scene_PlayMusic(void);

s32 Battle_RunFar(s32 entrance);

/// Plays Lucky Wheels (LuckyWheels_Start).
s32 LuckyWheels_StartFar(s32 entrance);

s32 SuperLuckyDice_StartFar(s32 entrance);

void Scene_Arrive(s32 unused, s32 keep);
void Scene_LoadOverlay(s32 unused);

/// The hexadecimal digits 0 to F, which String_FormatHex writes from.
extern const u8 gString_HexDigits[];

/// Where the debug text writes next on its screen in BG0.
extern u16 *gDebugTextCursor;

/// The halfword LZ decoder that Decompress_Halfwords runs from its stack.
extern const u8 Decompress_HalfwordLz[];
/// The size of Decompress_HalfwordLz, copied onto the stack by
/// Decompress_Halfwords.
extern u8 Decompress_HalfwordsCodeSize[];

// XXX: typed as struct SoundInfo *, transferCountdown's volatile read is not
// combined and the frame wait's registers move; whether the driver's own
// struct is right to make it volatile would settle the type.
/// Where the sound driver finds its work (SOUND_INFO_PTR): a pointer to
/// gSoundInfo, whose bytes the frame wait reads.
extern u8 *gSoundInfoPointer;
/// The byte `member` of the sound driver's work `info` (struct SoundInfo),
/// read through gSoundInfoPointer's bytes without the struct's volatile.
#define SOUND_INFO_BYTE(info, member)                                          \
  ((info)[offsetof(struct SoundInfo, member)])

/// SoundMain as the frame wait runs it: steps the music players and the CGB
/// channels and starts the IWRAM mixer on this VBlank's PCM, returning 0; it
/// returns 1 with nothing done when the stack is too deep or the driver busy.
s32 SoundDriver_EnterVBlankUpdate(void);

/// The resident mixer in IWRAM, which the frame wait runs after
/// SoundDriver_EnterVBlankUpdate returns 0. It never reads its argument; the
/// callers pass 8.
void IwramSoundRenderVBlank(s32) __attribute__((long_call));

/// The VBlank handler's sound step, as m4aSoundVSync: counts down the PCM
/// buffer's VBlanks and, as it wraps, restarts the two sound DMAs, which then
/// read from the buffer's start again.
void SoundDriver_VSyncRefresh(void);

void ScreenFade_Update(void);

/// The sound mixer's workspace at the top of IWRAM; the IWRAM heap ends where
/// it starts (Heap_GetRemainingIwram).
extern u8 IwramSoundMixWorkspace[];

/// The routine that runs the queued register writes, which
/// IoWriteQueue_FlushPending copies onto its stack and runs.
extern u8 Resource_ExecuteTransferCode[];
/// The size of Resource_ExecuteTransferCode.
extern u8 Resource_ExecuteTransferSize[];

/// Which of the 256 depth slots of gOamDepthLists hold sprites this frame, a
/// bit a slot; Oam_ResetFrame clears it.
extern u8 gOamBucketMasks[];

/// The 256 depth slots' list heads that every drawn sprite is linked into
/// (Oam_AddSpriteAtDepth); Oam_ResetFrame empties them each frame.
extern void *gOamDepthLists[];
/// The sector writer of the chip IdentifyFlash found.
extern u16 (*gProgramFlashSector)(u16 sector, u8 *source);

s32 Save_CompareBytes(u8 *left, u8 *right, s32 count);
s32 Save_ChecksumSector(void);
s32 Save_ReadSector(s32 index);
u32 Save_WriteSector(s32 sector);
s32 Save_InvalidateSector(s32 index);
u32 Save_SelectWriteSector(s32 partId);
u32 Save_GetLatestCounter(s32 partId);

/// The cartridge's first byte, as the link places it (rom_header.s).
extern const u8 Rom_Start[];

void System_CopyBuildStamp(void);

void System_VBlankHandler(void);

s32 System_GetLanguage(void);

void Save_SetProgressCallback(void (*progress)(s32 step));

s32 Save_WritePart(s32 partId, void *source);
s32 Save_InitializeWork(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);

void Serial_Initialize(void);

/// Nonzero lets DebugText_Print write; the power-on set-up and the credits
/// clear it.
extern u8 gDebugTextEnabled;
/// The current save file, or -1 for none (a new game).
extern s16 gSaveSlot;
/// 1 when a save file on the cartridge holds a cleared game, as the save screen
/// finds on loading the summaries; a new game keeps it in
/// gMainRoutineWork.clearedFileAtStart.
extern s16 gAnyFileCleared;
/// The buffer a save file is built in and written from, in two halves; its
/// summary's 16-byte header lies just before it.
extern u8 gSaveBuffer[];

/// Depth slots of Oam_AddSpriteAtDepth: a higher slot is drawn in front. The
/// windows' sprites, icons and cursors take the slots from OAM_DEPTH_WINDOW up,
/// in front of the field's and the battle's, and OAM_DEPTH_FRONT is in front of
/// everything.
#define OAM_DEPTH_WINDOW 240
#define OAM_DEPTH_FRONT 255
/// The depth slot the field Psynergy's effect sprites take: in front of every
/// actor, whose slots are clamped below OAM_DEPTH_WINDOW, and among the window
/// sprites' slots.
#define EFFECT_SPRITE_FRONT_DEPTH 250

#endif
