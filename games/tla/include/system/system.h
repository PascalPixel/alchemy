#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

#include "types.h"
#include "mem.h"
#include "edition.h"

void WaitFrames(s32 frames);
void System_WaitForFrameInterrupt(void);
void Render_ResetFrame(void);
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
u32 Random16(void);
extern u32 gRandomState;

extern u16 gSleepActive;

// SYSTEM

#include "types.h"
#include "edition.h"

struct FlashType;
struct FlashSetupInfo;
extern struct FlashSetupInfo gFlash_ChipAtmel;
extern const struct FlashSetupInfo gFlash_ChipDefault;
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
extern const u16 gWindow_Palette[];
s32 Animation_LookupValueByKey(s32 key);
#ifndef OWN_CountRemainingErasedFlashBytes
s32 CountRemainingErasedFlashBytes(u8 *ptr);
#endif
s32 Map_DisableUpdateCallbackFar(void);
s32 Map_EnableUpdateCallbackFar(void);
s32 RunFlashEraseVerifier(u8 *, u8 *);
void DebugText_Print(const u8 *src);
#ifndef OWN_StartFlashTimer
void StartFlashTimer(u8 index);
#endif
void PartyStatus_Close(void);

struct BattleObj;
struct Actor;

struct ParticleChild {
  u8 padding[9];
  u8 flags;
};
extern s32 gBattleMotion_VariantAcceleration[];
extern s32 gBattleMotion_VariantDistancePercent[];
extern s32 gBattleMotion_VariantSpeedLimit[];
extern s32 gBattleMotion_VariantVelocityY[];
extern u8 gPartyStatusWork[];
extern u16 gGraphics_ExpandNibbleTable[];
extern const u8 Resource_PatchThumbBranchCode[];
s32 _call_via_r3(s32, u8 *, u8, s32);
s32 AnimationObject_Allocate(s32 id);
s32 BattleEffect_FindMatchingEvent(s32, s32, void *);
void BattleMotion_ApplyVariantMotion(s32 id, s32 variant);
#ifndef OWN_BattleMotion_ApproachTarget
void BattleMotion_ApproachTarget(s32 actorId, s32 targetId, s32 travel_divisor,
                                 s32 initial_velocity_y);
#endif
#ifndef OWN_BattleMotion_InterpolatePosition
void BattleMotion_InterpolatePosition(struct BattleObj *start_slot,
                                      struct BattleObj *end_slot, s32 progress);
#endif
s32 LuckyDice_Run(void);
void Actor_WaitForValue16Far(struct Actor *);
void Actor_NoOp(void);
void Actor_SetMotionActionCallback(struct Actor *, s32);
void Resource_LoadCode(u32 index, void *destination);
s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index);
void ResourceMetadata_ClearRecord(void *destination);
void NumberText_FormatHex(u32);

#endif
