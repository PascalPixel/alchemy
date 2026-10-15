#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

#include "types.h"
#include "mem.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

#if GAME_TLA
void WaitFrames(s32 frames);
#endif
void System_WaitForFrameInterrupt(void);
#if GAME_TLA
void Graphics_ResetFrameState(void);
#endif
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
#if GAME_TLA
u32 Random16(void);
#endif
extern u32 gRandomState;

extern u16 gSleepActive;

/* SYSTEM */

#include "types.h"
#include "edition.h"

struct FlashChipInfo;
struct FlashChipAtmelInfo;

struct FlashChipType
{
	u32 handlers[5];
	u32 size;
	u16 sectorSize;
	u16 unk1a;
	u8	sectorShift;
};
extern struct FlashChipAtmelInfo  gFlash_ChipAtmel;
extern const struct FlashChipType gFlash_ChipUnknown;
extern u32						  gFlash_Handler1;
extern u32						  gEraseFlashSector;
#ifndef OWN_Flash_Handler4
extern u32 gFlash_Handler4;
#endif
#ifndef OWN_gFlash
extern struct FlashChipInfo *gFlash;
#endif
#ifndef OWN_gFlashSavedIme
extern u16 gFlashSavedIme;
#endif
#ifndef OWN_gFlashTimeoutFlag
extern u8 gFlashTimeoutFlag;
#endif
extern u8 gFlashTimerNum;
extern u8		   gNumberTextDecimalEnd[];
extern u8		   gNumberTextHexEnd[];
extern u8		   gLink_TimeLabelString[];
extern void		  *Resource_DirectoryTable[];
extern u8		   Resource_PatchThumbBranchCodeSize[];
extern u8		   gResourceSlot_ConversionTables[][256];
extern const u16   gUi_WindowPalette[];
#if GAME_TLA
s32 Animation_LookupValueByKey(s32 key);
#endif
#ifndef OWN_CountRemainingErasedFlashBytes
s32 CountRemainingErasedFlashBytes(u8 *ptr);
#endif
#if GAME_TLA
s32 Map_DisableUpdateCallbackFar(void);
s32 Map_EnableUpdateCallbackFar(void);
#endif
s32 RunFlashEraseVerifier(u8 *, u8 *);
#if GAME_TLA
void Runtime_WriteDebugTextTiles(const u8 *src);
#endif
#ifndef OWN_StartFlashTimer
void StartFlashTimer(u8 index);
#endif
#if GAME_TLA
void UiWork_FinalizeAndReleaseBlock16(void);
#endif

#if !GAME_TLA
struct AnimationEntry;
struct AnimationObject;
struct SelectionNode;
struct T;

struct S
{
	s32		  f00;
	s32		  f04;
	s32		  f08;
	s32		  f0c;
	u8		  pad10[32];
	s32		  f30;
	s32		  f34;
	s32		  f38;
	s32		  f3c;
	u8		  pad40[16];
	struct T *f50;
};
extern u8 IwramSoundPresetCode[];
#ifndef OWN_Flash_Handler0
extern u32 gFlash_Handler0;
#endif
extern struct AnimationEntry *gAnimationObjects[];
extern u16					 *gDebugTextCursor;
extern u8					  gDebugTextEnabled;
#ifndef OWN_gFlashNumRemainingBytes
extern u16 gFlashNumRemainingBytes;
#endif
extern u8		 gSaveBuffer[];
extern s16		 gSaveSlot;
extern u8		 IwramCodeRecords[];
extern u8		 LoadedRuntime_Size[];
extern const u16 gMath_ArcTanTable[];
extern const u8	 gObject_ShadowTiles[];
extern const u8	 Render_BuildOamListKernel[];
extern const u8	 Resource_ExecuteTransfer[];
extern const u8	 Resource_PatchThumbBranch[];
extern const u8	 gRom_LanguageCode;
extern u8		 gRom_UnknownBytes[];
extern const u32 BattleEffect_Max4Reverse[];
extern const u32 BattleEffect_Max4Forward[];
extern const u32 BattleEffect_MaxReverse[];
extern const u32 BattleEffect_MaxForward[];
extern const u32 BattleEffect_ColorReverse[];
extern const u32 BattleEffect_ColorForward[];
extern const u32 BattleEffect_AdditiveReverse[];
extern const u32 BattleEffect_AdditiveForward[];
extern const u32 BattleEffect_Mask[];
extern const u32 BattleEffect_Transparent8Reverse[];
extern const u32 BattleEffect_Transparent8Forward[];
extern const u32 BattleEffect_TransparentReverse[];
extern const u32 BattleEffect_TransparentForward[];
extern const u32 BattleEffect_CopyReverse[];
extern const u32 BattleEffect_CopyForward[];
extern const u8	 gSystem_BasicColorPalette[];
extern u8		 Tile_CopyStrided[];
extern u8		 Tile_CopyStridedCodeSize[];
extern u8		 TransferCommandExecutor_Size[];
void			 _call_via_r2(s32 arg0, s32 arg1, s32 target);
#ifndef OWN_Animation_SetWorkEntry
#endif
void IwramHalt(void);
#ifndef OWN_Runtime_GetLowTableAddress
#endif
#ifndef OWN_Runtime_GetRemainingEwram
#endif
#ifndef OWN_Runtime_GetRemainingIwram
#endif
#ifndef OWN_RuntimeDispatch_NoOpHook
#endif
void SoundDriver_EnterMixer(void);
#endif

#if GAME_TLA
struct BattleObj;
struct ObjectRuntime;

struct ParticleChild
{
	u8 padding[9];
	u8 flags;
};
extern s32		gBattleMotion_VariantAcceleration[];
extern s32		gBattleMotion_VariantDistancePercent[];
extern s32		gBattleMotion_VariantSpeedLimit[];
extern s32		gBattleMotion_VariantVelocityY[];
extern u8		gPartyStatusWork[];
extern u16		gGraphics_ExpandNibbleTable[];
extern const u8 Resource_PatchThumbBranchCode[];
s32				_call_via_r3(s32, u8 *, u8, s32);
s32				AnimationObject_Allocate(s32 id);
#if GAME_TLA
s32 BattleFx_FindMatchingEvent(s32, s32, void *);
#endif
void			BattleMotion_ApplyVariantMotion(s32 id, s32 variant);
#ifndef OWN_BattleMotion_ApproachTarget
void BattleMotion_ApproachTarget(
	s32 actorId, s32 targetId, s32 travel_divisor, s32 initial_velocity_y);
#endif
#ifndef OWN_BattleMotion_InterpolatePosition
void BattleMotion_InterpolatePosition(
	struct BattleObj *start_slot, struct BattleObj *end_slot, s32 progress);
#endif
s32	 LuckyDice_Run(void);
void ObjectDispatch_WaitForValue16Far(struct ObjectRuntime *);
void ObjectMotion_NoOp(void);
void ObjectMotion_SetActionCallback(struct ObjectRuntime *, s32);
void Resource_LoadCode(u32 index, void *destination);
s32	 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index);
void ResourceMetadata_ClearRecord(void *destination);
void Text_FormatHexToWork(u32);
#endif

#endif
