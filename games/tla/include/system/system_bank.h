#ifndef ALCHEMY_SYSTEM_BANK_H
#define ALCHEMY_SYSTEM_BANK_H

/// @file
/// What the system bank's code declares for the files that use it, beside
/// system.h until the files that declare these otherwise agree.

#include "types.h"

/// The cartridge's language, from its header (System_GetLanguage).
enum CartridgeLanguage {
  CARTRIDGE_LANGUAGE_ENGLISH = 1,
  CARTRIDGE_LANGUAGE_GERMAN,
  CARTRIDGE_LANGUAGE_FRENCH,
  CARTRIDGE_LANGUAGE_ITALIAN,
  CARTRIDGE_LANGUAGE_SPANISH
};

s32 Stub_ReturnZero(s32 unused0, const u8 *unused1);

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

/// The serial transfer's command, as Serial_QueueCommand hands it to the save
/// work: the command, how far it has got and its state.
struct SerialCommandWork {
  u8 unk0000[0x313c];
  u32 command;
  u16 progress;
  u16 state;
};

/// A halfword in a record of its own, which Serial_StepBlockTransfer keeps its
/// constant 1 in.
struct SerialHalfword {
  u16 v;
};

void Window_LoadGraphics(void);
void DebugText_Clear(void);

void VramCache_Reset(void);

void Transform_ApplyYaw(s32 ang);

s32 ArcTan2(s32 x, s32 y);

void ScreenFade_ToBlack(s32 vblanks);
void ScreenFade_Wait(void);

void Transform_SetYaw(s32 ang);
void Transform_ApplyPitch(s32 ang);
void Transform_ApplyPosition(const s32 *pos);
void Projection_Set(u32 value0, u32 value1, u32 value2);
void Transform_ApplyTranslation(s32 x, s32 y, s32 z); // translate
void Transform_ApplyUniformScale(s32 scale);
void Transform_ApplyAxisScale(s32 x, s32 y, s32 z);
void Transform_ApplyRoll(s32 ang);
void Transform_Pop(void);

void Palette_LoadBasicSpriteColors(void);

void Resource_ReservedNoOp(s32 count);

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

void Oam_AddSpriteAtDepthClamped(void *entry, s32 slot);

void Decompress_Halfwords(const void *source, void *destination);

void ScreenFade_FromBlack(s32 vblanks);

s32 Resource_DecodeType2(const void *source, void *destination);

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

extern const u8 Resource_DecodeHalfwordLzCode[];
extern u8 Resource_DecompressHalfwordsCodeSize[];

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

extern const u32 Math_DotProductQ16Code[];

s32 System_GetLanguage(void);

void Serial_QueueCommand(u32 command);

s32 Save_WritePart(s32 partId, void *source);
s32 Save_InitializeWork(void);
s32 Save_LoadFileSummaries(void);
void Save_ReleaseWork(void);

void Serial_Initialize(void);

#endif
