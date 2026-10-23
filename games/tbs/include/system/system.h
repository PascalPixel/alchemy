#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

/// @file
/// The system's routines and globals: interrupts, BIOS sound bias, random
/// numbers and sleep.

#include "types.h"
#include "mem.h"
#include "edition.h"
#include "farcall.h"

void System_WaitForFrameInterrupt(void);
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
/// No code in the tree defines or reads it; Random16's seed is gRandomSeed.
extern u32 gRandomState;

/// 1 while the CPU is stopped for sleep, until L and R wake it.
extern u16 gSleepActive;

#include "types.h"
#include "edition.h"

struct FlashType;
struct FlashSetupInfo;
// The flash library's state, after pret's agb_flash: IdentifyFlash installs
// the chip's routines and type. The credits borrow four of these cells while
// no flash operation runs (graphics/credits/credits.c).
extern struct FlashSetupInfo gFlash_ChipAtmel;
extern const struct FlashSetupInfo gFlash_ChipDefault;
/// The chip's erase-all routine (FlashSetupInfo.handlers[1]).
extern u32 gFlash_Handler1;
/// The chip's erase-sector routine (handlers[2]).
extern u32 gEraseFlashSector;
#ifndef OWN_Flash_Handler4
/// The chip's wait-time table (FlashSetupInfo.maxTime).
extern u16 *gFlash_Handler4;
#endif
#ifndef OWN_gFlash
extern const struct FlashType *gFlash;
#endif
#ifndef OWN_gFlashSavedIme
/// REG_IME as StartFlashTimer found it, for StopFlashTimer to restore.
extern u16 gFlashSavedIme;
#endif
#ifndef OWN_gFlashTimeoutFlag
/// Set by FlashTimerIntr when the running operation's time is up.
extern u8 gFlashTimeoutFlag;
#endif
/// The timer, 0 to 3, the flash library times its operations with.
extern u8 gFlashTimerNum;
/// gNumberBuffer + 10, the decimal form's units digit (the form ends at
/// + 11); DebugText_PrintDecimal starts n characters before it, so it prints
/// n + 1.
extern u8 gNumberBufferDecimalEnd[];
/// The end of the hexadecimal form in gNumberBuffer.
extern u8 gNumberBufferHexEnd[];
/// The link battle timer's heading: `TIME`; `TMPO.` in Spanish, `TEMPS` in
/// French.
extern u8 gLink_TimeLabelString[];
/// Each resource's ROM address by resource id (Resource_GetTableEntry).
extern void *Resource_DirectoryTable[];
/// The size of Resource_PatchThumbBranch (decode_01.s).
extern u8 Resource_PatchThumbBranchCodeSize[];
/// The first five 256-byte colour tables of SpriteColorMode, indexed by pixel.
extern u8 gResourceSlot_ConversionTables[][256];
/// The window font's 16 colours, loaded as BG palette 15
/// (Window_LoadGraphics).
extern const u16 gWindow_Palette[];
#ifndef OWN_CountRemainingErasedFlashBytes
s32 CountRemainingErasedFlashBytes(u8 *ptr);
#endif
s32 RunFlashEraseVerifier(u8 *, u8 *);
#ifndef OWN_StartFlashTimer
void StartFlashTimer(u8 index);
#endif

struct AnimationEntry;
struct AnimationObject;
struct SelectionNode;
struct T;

/// A placeholder layout no code in the tree uses.
struct S {
  s32 f00;
  s32 f04;
  s32 f08;
  s32 f0c;
  u8 pad10[32];
  s32 f30;
  s32 f34;
  s32 f38;
  s32 f3c;
  u8 pad40[16];
  struct T *f50;
};
/// The 152 bytes of IWRAM-resident sound code (0x03000bd8, not the mixer at
/// 0x03007000) that Sound_LoadPresetParameters overwrites with a preset from
/// IwramCodeRecords.
extern u8 IwramSoundPresetCode[];
#ifndef OWN_Flash_Handler0
/// The chip's program routine (FlashSetupInfo.handlers[0]), which
/// Save_WriteSector calls.
extern u32 gFlash_Handler0;
#endif
/// HEAP_SLOT_ANIMATION_LAYERS: the animation layer records (struct
/// AnimationEntry) that Animation_AllocateLayer takes from.
extern struct AnimationEntry *gAnimationObjects[];
/// Where the debug text writes next on its 32 by 20 screen in BG0.
extern u16 *gDebugTextCursor;
/// Nonzero lets DebugText_Print write; the power-on set-up and the credits
/// clear it.
extern u8 gDebugTextEnabled;
#ifndef OWN_gFlashNumRemainingBytes
/// Bytes left of the sector being programmed.
extern u16 gFlashNumRemainingBytes;
#endif
/// The buffer a save file is built in and written from, in two halves; its
/// summary's 16-byte header lies just before it.
extern u8 gSaveBuffer[];
/// The current save file, or -1 for none (a new game).
extern s16 gSaveSlot;
/// The five sound presets' mixer code, SOUND_PRESET_SIZE bytes each.
extern u8 IwramCodeRecords[];
/// The size of Render_BuildOamListKernel (decode_byte_lz.s).
extern u8 LoadedRuntime_Size[];
/// No code in the tree defines or reads it.
extern const u16 gMath_ArcTanTable[];
/// The sprites' shadow, 16 by 8 at 256 colours, loaded into VRAM cache entry
/// 93.
extern const u8 gObject_ShadowTiles[];
/// The ARM OAM list builder, run from a heap copy by Render_BuildOamList.
extern const u8 Render_BuildOamListKernel[];
/// The ARM register-write executor, run from a stack copy by
/// IoWriteQueue_FlushPending.
extern const u8 Resource_ExecuteTransfer[];
/// The ARM routine that resolves a loaded overlay's long branches, run from a
/// heap copy by Overlay_Load.
extern const u8 Resource_PatchThumbBranch[];
/// The game code's last letter in the cartridge header: `J` in Japanese, `E`,
/// `D`, `F`, `S` or `I` (System_GetLanguage).
extern const u8 gRom_LanguageCode;
// The pixel fragments BattleAnim_BuildBlitter copies into its blitters, from
// battle_effect_composite.s: Forward reads the picture left to right and
// Reverse right to left. None of them runs where it is.
/// Max, four pixels (one canvas word), right to left; a full cell's row takes
/// it twice.
extern const u32 BattleAnim_Max4Reverse[];
/// Max, four pixels (one canvas word), left to right; a full cell's row takes
/// it twice.
extern const u32 BattleAnim_Max4Forward[];
/// Max, one pixel, right to left.
extern const u32 BattleAnim_MaxReverse[];
/// Max, one pixel, left to right.
extern const u32 BattleAnim_MaxForward[];
/// Additive, four pixels added a word at a time, right to left; a full cell's
/// row takes it twice.
extern const u32 BattleAnim_ColorReverse[];
/// Additive, four pixels added a word at a time, left to right; a full cell's
/// row takes it twice.
extern const u32 BattleAnim_ColorForward[];
/// Additive, one pixel, right to left.
extern const u32 BattleAnim_AdditiveReverse[];
/// Additive, one pixel, left to right.
extern const u32 BattleAnim_AdditiveForward[];
/// Sets 0x40 in every byte of sl, which caps an additive word's pixels at 63.
extern const u32 BattleAnim_Mask[];
/// Transparent, a full cell's eight pixels, right to left.
extern const u32 BattleAnim_Transparent8Reverse[];
/// Transparent, a full cell's eight pixels, left to right.
extern const u32 BattleAnim_Transparent8Forward[];
/// Transparent, one pixel, right to left.
extern const u32 BattleAnim_TransparentReverse[];
/// Transparent, one pixel, left to right.
extern const u32 BattleAnim_TransparentForward[];
/// Copy, one pixel, right to left.
extern const u32 BattleAnim_CopyReverse[];
/// Copy, one pixel, left to right.
extern const u32 BattleAnim_CopyForward[];
/// The basic sprite palettes 0 to 13, 224 colours
/// (Palette_LoadBasicSpriteColors).
extern const u8 gSystem_BasicColorPalette[];
/// The sprite tile uploader, run from HEAP_SLOT_FRAME_UPLOADER.
extern u8 Tile_CopyStrided[];
/// The size of Tile_CopyStrided (convert_map.s).
extern u8 Tile_CopyStridedCodeSize[];
/// The size of Resource_ExecuteTransfer (decode_01.s).
extern u8 TransferCommandExecutor_Size[];
void _call_via_r2(s32 arg0, s32 arg1, s32 target);
void Iwram_Halt(void);
void SoundDriver_EnterMixer(void);

#endif
