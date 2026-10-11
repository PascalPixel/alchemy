#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

#include "types.h"
#include "mem.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

void	   WaitFrames(s32 frames);
void	   System_WaitForFrameInterrupt(void);
void	   Graphics_ResetFrameState(void);
void	   Bios_SoundBiasOff(void);
void	   Bios_SoundBiasOn(void);
u32		   Random16(void);
extern u32 gRandomState;

extern volatile u16 gSleepActive;

/* SYSTEM */

#include "types.h"
#include "edition.h"

struct FlashChipInfo;
struct FlashInfo08007220;

struct FlashChipType
{
	u32 handlers[5];
	u32 size;
	u16 sector_size;
	u16 unknown_1a;
	u8	sector_shift;
};
extern struct FlashInfo08007220	  Flash_ChipAtmel;
extern const struct FlashChipType Flash_ChipUnknown;
extern u32						  Flash_Handler1;
extern u32						  gEraseFlashSector;
#ifndef OWN_Flash_Handler4
extern u32 Flash_Handler4;
#endif
#ifndef OWN_gFlash
extern struct FlashChipInfo *gFlash;
#endif
#ifndef OWN_gFlashSavedIme
extern volatile u16 gFlashSavedIme;
#endif
#ifndef OWN_gFlashTimeoutFlag
extern u8 gFlashTimeoutFlag;
#endif
#ifndef OWN_gFlashTimerCount
extern volatile u16 gFlashTimerCount;
#endif
extern volatile u8 gFlashTimerNum;
extern u8		   gNumberTextDecimalEnd[];
extern u8		   gNumberTextHexEnd[];
extern u8		   Link_TimeLabelString[];
extern void		  *Resource_DirectoryTable[];
extern u8		   Resource_PatchThumbBranchCodeSize[];
extern u8		   ResourceSlot_ConversionTables[][256];
extern const u16   Ui_WindowPalette[];
s32				   Animation_LookupValueByKey(s32 key);
#ifndef OWN_CountRemainingErasedFlashBytes
s32 CountRemainingErasedFlashBytes(u8 *ptr);
#endif
#if GAME_TLA
s32	 Map_DisableUpdateCallbackFar(void);
s32	 Map_EnableUpdateCallbackFar(void);
#endif
s32	 RunFlashEraseVerifier(u8 *, u8 *);
void Runtime_WriteDebugTextTiles(const u8 *src);
#ifndef OWN_StartFlashTimer
void StartFlashTimer(u8 index);
#endif
void UiWork_FinalizeAndReleaseBlock16(void);

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
extern u32 Flash_Handler0;
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
extern const u16 Math_ArcTanTable[];
extern const u16 Math_TanTable[];
extern const u8	 Object_ShadowTiles[];
extern const u8	 Render_BuildOamListKernel[];
extern const u8	 Resource_ExecuteTransfer[];
extern const u8	 Resource_PatchThumbBranch[];
extern const u8	 Rom_LanguageCode;
extern const u8	 Text_HexDigits[];
extern u8		 RomBytes_080308a0[];
extern const u16 SentouKouka_BitOperands[];
extern const u32 SentouKouka_Hikaku4Gyaku[];
extern const u32 SentouKouka_Hikaku4Jun[];
extern const u32 SentouKouka_HikakuGyaku[];
extern const u32 SentouKouka_HikakuJun[];
extern const u32 SentouKouka_IroGyaku[];
extern const u32 SentouKouka_IroJun[];
extern const u32 SentouKouka_KasanGyaku[];
extern const u32 SentouKouka_KasanJun[];
extern const u32 SentouKouka_Mask[];
extern const u32 SentouKouka_Nuri8Gyaku[];
extern const u32 SentouKouka_Nuri8Jun[];
extern const u32 SentouKouka_NuriGyaku[];
extern const u32 SentouKouka_NuriJun[];
extern const u32 SentouKouka_YomiGyaku[];
extern const u32 SentouKouka_YomiJun[];
extern const u8	 System_BasicColorPalette[];
extern const s32 Text_PowersOfTen[];
extern u8		 Tile_CopyStrided[];
extern u8		 Tile_CopyStridedCodeSize[];
extern u8		 TransferCommandExecutor_Size[];
void			 _call_via_r2(s32 arg0, s32 arg1, s32 target);
s32				 Animation_InitializeObjects(struct AnimationObject *state);
void			 Animation_InitWorkFromMetadata(struct AnimationEntry *work);
#ifndef OWN_Animation_SetWorkEntry
void Animation_SetWorkEntry(struct AnimationEntry *work, s32 no);
#endif
struct AnimationEntry *AnimationObject_Allocate(s32 id);
void				   AnimationObjects_SetActiveTimerSteps(struct AnimationObject *group, s32 val);
void				   Bg0_ClearTilemap(void);
s32					   FixedSqrt(s32 value);
void				   IwramHalt(void);
s32					   Link_CreateCountdownLabelWindow(void);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse);
void ObjectSystem_Configure(s32 mode);
void PaletteDma_LoadBlock(void);
u8	*Resource_DecompressLz(const u8 *source, u8 *destination);
struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);
void				  Resource_InitializeTable(void);
s32					  Resource_LoadByMode(s32 mode, s32 value);
void				  Resource_LoadWorkHeader(void);
void				  Resource_ScheduleOwnerReset(void);
void				  Resource_ScheduleOwnerResetDelayed(void);
s32					  ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant);
#ifndef OWN_Runtime_GetLowTableAddress
s32 Runtime_GetLowTableAddress(void);
#endif
#ifndef OWN_Runtime_GetRemainingEwram
s32 Runtime_GetRemainingEwram(void);
#endif
#ifndef OWN_Runtime_GetRemainingIwram
s32 Runtime_GetRemainingIwram(void);
#endif
#ifndef OWN_RuntimeDispatch_NoOpHook
void RuntimeDispatch_NoOpHook(void);
#endif
void  SaveState_CaptureObjectTableFar(void);
void  SerialRuntime_Initialize(void);
void  SoundDriver_EnterMixer(void);
void  System_VBlankHandler(void);
void  Text_FormatHexToWork(u32 value);
void *Text_FormatPlayTime(s32 value, u8 *out);
void  Text_FormatSignedDecimalToWork(s32 arg0);
void  Ui_LoadWindowGraphics(void);
#endif

#if GAME_TLA
struct BtlObj;
struct ObjectRuntime;

struct ParticleChild
{
	u8 padding[9];
	u8 flags;
};
extern s32		BattleMotion_VariantAcceleration[];
extern s32		BattleMotion_VariantDistancePercent[];
extern s32		BattleMotion_VariantSpeedLimit[];
extern s32		BattleMotion_VariantVelocityY[];
extern u8		gPartyStatusWork[];
extern u16		Graphics_ExpandNibbleTable[];
extern const u8 Resource_PatchThumbBranchCode[];
s32				_call_via_r3(s32, u8 *, u8, s32);
s32				AnimationObject_Allocate(s32 id);
s32				BattleFx_FindMatchingEvent(s32, s32, void *);
void			BattleMotion_ApplyVariantMotion(s32 id, s32 variant);
#ifndef OWN_BattleMotion_ApproachTarget
void BattleMotion_ApproachTarget(
	s32 actor_id, s32 target_id, s32 travel_divisor, s32 initial_velocity_y);
#endif
#ifndef OWN_BattleMotion_InterpolatePosition
void BattleMotion_InterpolatePosition(
	struct BtlObj *start_slot, struct BtlObj *end_slot, s32 progress);
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
