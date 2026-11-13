#ifndef ALCHEMY_MODULE_SYSTEM_SYSTEM_H
#define ALCHEMY_MODULE_SYSTEM_SYSTEM_H

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
/// SEQUENCE_ values: the sequences other than the field sequence.
enum {
  // XXX: what sequence Resource_FarCall00F runs here, where The Broken
  // Seal's SEQUENCE_NONE runs nothing; SuperLuckyDice_Run
  // (field/dice_game.c) and a scene that leaves for it would
  // settle it.
  SEQUENCE_UNK_1FB = 0x1fb,
  SEQUENCE_SLOTS = 0x1fc, ///< the slots sequence (LuckyWheels_Run)
  SEQUENCE_DICE = 0x1fd,  ///< the dice sequence (LuckyDice_Run)
  SEQUENCE_BATTLE = 0x1fe ///< a battle; the entrance is the formation
};

enum { MINIGAME_PALETTE_FIRST = 14, MINIGAME_PALETTE_BYTES = 64 };

struct BgScrollRegs {
  u32 words[4];
};

struct SerialCommandWork {
  u8 unk0000[0x313c];
  u32 command;
  u16 progress;
  u16 state;
};

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

void Func_080150ac(s32 ang);
void Transform_ApplyPitch(s32 ang);
void SceneTransform_ApplyPosition(const s32 *pos);
void Projection_Set(u32 value0, u32 value1, u32 value2);
void Func_08015160(s32 x, s32 y, s32 z); // translate
void Func_0801521c(s32 scale);
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
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

s32 Battle_RunEncounterFar(s32 entrance);

s32 Resource_FarCall00E(s32 entrance);

s32 Resource_FarCall00F(s32 entrance);

void Scene_Arrive(s32 unused, s32 keep);
void Scene_LoadOverlay(s32 unused);

extern const u8 gString_HexDigits[];

extern u16 *gDebugTextCursor;

extern const u8 Resource_DecodeHalfwordLzCode[];
extern u8 Resource_DecompressHalfwordsCodeSize[];

extern u8 *gData_03007ff0;

s32 SoundDriver_EnterVBlankUpdate(void);

void IwramSoundRenderVBlank(s32) __attribute__((long_call));

void SoundDriver_VSyncRefresh(void);

void ScreenFade_Update(void);

extern u8 IwramSoundMixWorkspace[];

extern u8 Resource_ExecuteTransferCode[];
extern u8 Resource_ExecuteTransferSize[];

extern u8 gOamBucketMasks[];

extern void *gOamBuckets[];
extern u16 (*gFlash_Handler0)(u16 sector, u8 *source);

s32 Save_CompareBytes(u8 *left, u8 *right, s32 count);
s32 Save_ChecksumSector(void);
s32 Save_ReadSector(s32 index);
u32 Save_WriteSector(s32 sector);
s32 Save_InvalidateSector(s32 index);
u32 Save_SelectWriteSector(s32 partId);
u32 Save_GetLatestCounter(s32 partId);

extern const u8 Cartridge_Restart[];

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
