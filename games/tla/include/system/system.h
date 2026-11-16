#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

/// @file
/// The system's routines and globals: interrupts, BIOS sound bias, random
/// numbers and sleep, the save slot, the debug text, and the system bank's
/// other routines, as The Broken Seal's system.h.

#include "edition.h"
#include "fixed_math.h"
#include "types.h"
#include "system/heap_release.h"

void wait(s32 frames);
void System_WaitForFrameInterrupt(void);
void Oam_ResetFrame(void);
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
u32 Random16(void);
/// A random number from 0 to n - 1: n times a 16-bit random draw, over 65536,
/// as The Broken Seal's event.h writes it.
#define Random_Below(n) (Random16() * (n) >> 16)
/// Random16's seed; Lucky Wheels seeds it with the play time, and a link
/// battle's parent sends it as the turn's gBattleRandomSeed.
extern u32 gRandomSeed;

/// 1 while the CPU is stopped for sleep, until L and R wake it.
extern u16 gSleepActive;

struct FlashType;
struct FlashSetupInfo;
extern const struct FlashSetupInfo gFlash_ChipAtmel;
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
extern u8 gFlashTimerNum;
extern u8 gNumberTextDecimalEnd[];
extern u8 gNumberTextHexEnd[];
/// The link battle timer's heading: `TIME`; `TMPO.` in Spanish, `TEMPS` in
/// French.
extern u8 gLink_TimeLabelString[];
/// Each resource's ROM address by resource id (Resource_GetTableEntry).
extern void *Resource_DirectoryTable[];
/// The size of Resource_PatchThumbBranch (decompress_tagged_lz.s).
extern u8 Resource_PatchThumbBranchCodeSize[];
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

/// A battle object's sprite as its motion code reads it: the flags at byte 9.
struct ParticleChild {
  u8 padding[9];
  u8 flags;
};
extern s32 gBattleMotion_VariantAcceleration[];
extern s32 gBattleMotion_VariantDistancePercent[];
extern s32 gBattleMotion_VariantSpeedLimit[];
extern s32 gBattleMotion_VariantVelocityY[];
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
s32 BattleEffect_FindMatchingEvent(s32, s32, void *);
void BattleMotion_ApplyVariantMotion(s32 id, s32 variant);
#ifndef OWN_BattleMotion_ApproachTarget
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travel_divisor,
                                 s32 initial_velocity_y);
#endif
#ifndef OWN_BattleMotion_InterpolatePosition
void BattleMotion_InterpolatePosition(struct BattleObject *start_slot,
                                      struct BattleObject *end_slot,
                                      s32 progress);
#endif
void LuckyDice_Run(void);
void Actor_WaitForValue16Far(struct Actor *);
/// An empty function, reached only through an unnamed veneer in the object
/// bank's far-call table.
void Actor_NoOp(void);
void Actor_SetMotionActionCallback(struct Actor *, s32);
void Overlay_Load(u32 index, void *destination);
s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index);
void SpriteAnimation_Clear(void *destination);
void String_FormatHex(u32);

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

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

/// Keyed callbacks return a value (the target address is left in r0).
typedef s32 (*TaskIntFunc)(void);

typedef s32 (*Dot3Fn)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);

/// Sequence ids above the scene table's rows, as The Broken Seal's
/// SEQUENCE_ values: the sequences other than the field sequence, which
/// MainRoutine_Run runs in place of a scene.
enum {
  /// The Super Lucky Dice sequence (SuperLuckyDice_Run); Contigo's dealer
  /// leaves for it. The Broken Seal's 0x1fb, SEQUENCE_NONE, runs nothing.
  SEQUENCE_SUPER_DICE = 0x1fb,
  SEQUENCE_SLOTS = 0x1fc, ///< the slots sequence (LuckyWheels_Run)
  SEQUENCE_DICE = 0x1fd,  ///< the dice sequence (LuckyDice_Run)
  SEQUENCE_BATTLE = 0x1fe ///< a battle; the entrance is the formation
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

/// The four backgrounds' scroll registers as words, horizontal and vertical
/// together.
struct BgScrollRegs {
  u32 words[4];
};

/// A halfword in a record of its own, which Serial_StepBlockTransfer keeps its
/// constant 1 in.
struct SerialHalfword {
  u16 v;
};

void Window_LoadGraphics(void);
void DebugText_Clear(void);

void VramCache_Reset(void);

void Matrix_RotateY(s32 ang);

s32 Trig_Atan(s32 x, s32 y);

void ScreenFade_ToBlack(s32 vblanks);
void ScreenFade_Wait(void);

void Matrix_SetRotateY(s32 angle);
void Matrix_RotateX(s32 angle);
void Matrix_TranslateVector(const FIXED *position);
void Projection_Set(u32 focal, u32 nearDepth, u32 farDepth);
void Matrix_Translate(FIXED x, FIXED y, FIXED z);
void Matrix_ScaleUniform(FIXED scale);
void Matrix_Scale(FIXED x, FIXED y, FIXED z);
void Matrix_RotateZ(s32 ang);
void Matrix_Pop(void);

void Palette_LoadBasicSpriteColors(void);

void Resource_ReservedNoOp(s32 count);

void Vector_AddPolarOffset(FIXED radius, s32 angle, s32 *position);

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

extern const u8 gString_HexDigits[];

/// Where the debug text writes next on its screen in BG0.
extern u16 *gDebugTextCursor;

extern const u8 Decompress_HalfwordLz[];
extern u8 Decompress_HalfwordsCodeSize[];

/// Where the sound driver finds its work (SOUND_INFO_PTR): a pointer to
/// gSoundInfo, whose bytes the frame wait reads.
// XXX: typed as struct SoundInfo *, transferCountdown's volatile read is not
// combined and the frame wait's registers move; whether the driver's own
// struct is right to make it volatile would settle the type.
extern u8 *gSoundInfoPointer;

s32 SoundDriver_EnterVBlankUpdate(void);

void IwramSoundRenderVBlank(s32) __attribute__((long_call));

void SoundDriver_VSyncRefresh(void);

void ScreenFade_Update(void);

extern u8 IwramSoundMixWorkspace[];

extern u8 Resource_ExecuteTransferCode[];
extern u8 Resource_ExecuteTransferSize[];

extern u8 gOamBucketMasks[];

extern void *gOamDepthLists[];
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

extern const u32 Math_InnerProductCode[];

s32 System_GetLanguage(void);

void Save_SetProgressCallback(void (*progress)(s32 step));

s32 Save_WritePart(s32 partId, void *source);
s32 Save_InitializeWork(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);

void Serial_Initialize(void);

extern u8 gDebugTextEnabled;
extern s16 gSaveSlot;
extern s16 gAnyFileCleared;
extern u8 gSaveBuffer[];

/// Depth slots of Oam_AddSpriteAtDepth: a higher slot is drawn in front. The
/// windows' sprites, icons and cursors take the slots from OAM_DEPTH_WINDOW up,
/// in front of the field's and the battle's, and OAM_DEPTH_FRONT is in front of
/// everything.
#define OAM_DEPTH_WINDOW 240
#define OAM_DEPTH_FRONT 255

#endif
