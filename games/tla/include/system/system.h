#ifndef ALCHEMY_SYSTEM_H
#define ALCHEMY_SYSTEM_H

#include "types.h"
#include "heap_release.h"
#include "edition.h"

void wait(s32 frames);
void System_WaitForFrameInterrupt(void);
void Oam_ResetFrame(void);
void Bios_SoundBiasOff(void);
void Bios_SoundBiasOn(void);
u32 Random16(void);
/// Random16's seed; Lucky Wheels seeds it with the play time, and a link
/// battle's parent sends it as the turn's gBattleRandomSeed.
extern u32 gRandomSeed;

/// 1 while the CPU is stopped for sleep, until L and R wake it.
extern u16 gSleepActive;

// SYSTEM

#include "types.h"
#include "edition.h"

struct FlashType;
struct FlashSetupInfo;
extern struct FlashSetupInfo gFlash_ChipAtmel;
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
/// The size of Resource_PatchThumbBranch (patch_thumb_branch.s).
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

#endif
