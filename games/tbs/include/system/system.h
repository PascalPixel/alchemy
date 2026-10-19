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
extern u32 gRandomState;

extern u16 gSleepActive;

#include "types.h"
#include "edition.h"

struct FlashType;
struct FlashSetupInfo;
extern struct FlashSetupInfo gFlash_ChipAtmel;
extern const struct FlashSetupInfo gFlash_ChipUnknown;
extern u32 gFlash_Handler1;
extern u32 gEraseFlashSector;
#ifndef OWN_Flash_Handler4
extern u16 *gFlash_Handler4;
#endif
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
extern u8 gLink_TimeLabelString[];
extern void *Resource_DirectoryTable[];
extern u8 Resource_PatchThumbBranchCodeSize[];
extern u8 gResourceSlot_ConversionTables[][256];
extern const u16 gUi_WindowPalette[];
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
extern u8 IwramSoundPresetCode[];
#ifndef OWN_Flash_Handler0
extern u32 gFlash_Handler0;
#endif
extern struct AnimationEntry *gAnimationObjects[];
extern u16 *gDebugTextCursor;
extern u8 gDebugTextEnabled;
#ifndef OWN_gFlashNumRemainingBytes
extern u16 gFlashNumRemainingBytes;
#endif
extern u8 gSaveBuffer[];
extern s16 gSaveSlot;
extern u8 IwramCodeRecords[];
extern u8 LoadedRuntime_Size[];
extern const u16 gMath_ArcTanTable[];
extern const u8 gObject_ShadowTiles[];
extern const u8 Render_BuildOamListKernel[];
extern const u8 Resource_ExecuteTransfer[];
extern const u8 Resource_PatchThumbBranch[];
extern const u8 gRom_LanguageCode;
extern u8 gRom_UnknownBytes[];
extern const u32 BattleFx_Max4Reverse[];
extern const u32 BattleFx_Max4Forward[];
extern const u32 BattleFx_MaxReverse[];
extern const u32 BattleFx_MaxForward[];
extern const u32 BattleFx_ColorReverse[];
extern const u32 BattleFx_ColorForward[];
extern const u32 BattleFx_AdditiveReverse[];
extern const u32 BattleFx_AdditiveForward[];
extern const u32 BattleFx_Mask[];
extern const u32 BattleFx_Transparent8Reverse[];
extern const u32 BattleFx_Transparent8Forward[];
extern const u32 BattleFx_TransparentReverse[];
extern const u32 BattleFx_TransparentForward[];
extern const u32 BattleFx_CopyReverse[];
extern const u32 BattleFx_CopyForward[];
extern const u8 gSystem_BasicColorPalette[];
extern u8 Tile_CopyStrided[];
extern u8 Tile_CopyStridedCodeSize[];
extern u8 TransferCommandExecutor_Size[];
void _call_via_r2(s32 arg0, s32 arg1, s32 target);
void IwramHalt(void);
void SoundDriver_EnterMixer(void);

#endif
