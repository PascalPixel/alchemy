#ifndef ALCHEMY_MODULE_SCENE_TRIAL_ROAD_H
#define ALCHEMY_MODULE_SCENE_TRIAL_ROAD_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

struct Actor;
struct FieldSprite;
union FieldObject;

struct EventWork184 {
  u8 unk000[388];
  s16 value;
};

/// A view of the map work's byte 23.
struct MapWorkByte23 {
  u8 unk00[0x17];
  u8 bits0 : 2;
  // XXX: what bits 2 and 3 select; the map code that reads them would settle
  // it.
  u8 bits2 : 2;
  u8 bits4 : 4;
};

/// The sprite's priority bits, read as the byte that holds them.
struct EffectSpriteBits {
  u8 unk0 : 2;
  u8 priority : 2;
} __attribute__((packed));

/// A sprite's attribute 2, as the byte bits view reaches it.
struct EffectSpriteAttributes {
  u8 unk00[9];
  struct EffectSpriteBits bits;
};

struct TrialRoadReplay {
  s16 mode;
  s16 mirrored;
  s16 id;
  s16 point;
  s16 still;
  s16 resource;
};

struct TrialRoadLinkWork {
  u8 unk00[0xe4];
  u16 rival;
  u16 unke6;
  u16 linkAngle;
  s16 resource;
  s32 linkX;
  s32 linkZ;
  s16 path[0x1c20];
};

/// One queued sprite: the three words Oam_AddSpriteAtDepth adds to OAM.
struct TrialRoadSprite {
  u32 words[3];
};

struct TrialRoadLink {
  u8 unk00[0xd8];
  s16 vramEntry;
  u16 unkda;
  u8 unkdc[4];
  u16 unke0;
  u16 a;
  u16 b;
  u8 unke6[6];
  s32 height;
  s32 unkf0;
};

struct TrialRoadControl {
  s16 state;
  s16 field02;
  s16 field04;
  s16 field06;
  s16 field08;
  s16 field0a;
};

s32 TrialRoad_IsMirrored(void);

void TrialRoad_BlockCellUnder(s32 id);
void TrialRoad_UnblockCellUnder(s32 id);
void TrialRoad_AddPillarTop(s32 id);
void TrialRoad_SetPillarTopLift(s32 id, s32 value);
void TrialRoad_OpenSpoutCell(s32 id);
void TrialRoad_TrackLeaderOnPillars(void);
void TrialRoad_CheckPoundedPillarsCovered(s32 a, s32 b);
void TrialRoad_PoundPillar(s32 id, s32 a, s32 b);
void TrialRoad_StepOnSeesawPillar(s32 id, s32 other, s32 flag, s32 side);
void TrialRoad_PushLog(s32 id, s32 dx, s32 dz, s32 axis);
void TrialRoad_UncoverSpout(s32 id, s32 a, s32 frame);
void TrialRoad_CheckCrumblingStep(const u16 *table);
void TrialRoad_CrumblePillar(const void *data, s32 a);

void TrialRoad_ClearGiveUpAsked(void);

void TrialRoad_BlowBushOffLonePillar(void);

void TrialRoad_JumpAcrossGap(void);

void TrialRoad_JumpMeltedPuddle(void);

void TrialRoad_StepOnPillar18(void);

void TrialRoad_StepOnPillar19(void);

void TrialRoad_StepOnPillar20(void);

void TrialRoad_StepOnPillar21(void);

void TrialRoad_LandOnPillar(void);

void TrialRoad_CheckCrumblingPillars(void);

void TrialRoad_PushLog16West(void);

void TrialRoad_PushLog16East(void);

void TrialRoad_PushLog17West(void);

void TrialRoad_PushLog17East(void);

void TrialRoad_PushLog18North(void);

void TrialRoad_PushLog18South(void);

void TrialRoad_PushLog18SouthFromRow52(void);

void TrialRoad_StartSand(void);

void TrialRoad_EndSand(void);

void TrialRoad_RevealVine(void);

void TrialRoad_DriftWithStream(void);

void TrialRoad_LiftCaveRock(void);

void TrialRoad_CutBushes(void);

void TrialRoad_CutMadPlantBushes(void);

void TrialRoad_RevealPoolTiles(void);

void TrialRoad_HidePoolTiles(void);

void TrialRoad_ChallengeMoapa(void);

extern struct ActorPlacement gTrialRoad_PillarStagePlacements[];
extern struct ActorPlacement gTrialRoad_TorchStagePlacements[];
extern struct ActorPlacement gTrialRoad_BushPillarStagePlacements[];
extern struct ActorPlacement gTrialRoad_PoundStagePlacements[];
extern struct ActorPlacement gTrialRoad_CrumblingStagePlacements[];
extern struct ActorPlacement gTrialRoad_LogStagePlacements[];
extern struct ActorPlacement gTrialRoad_FountainStagePlacements[];
extern struct ActorPlacement gTrialRoad_SandStagePlacements[];
extern struct Trigger gTrialRoad_PillarStageTriggers[];
extern struct Trigger gTrialRoad_TorchStageTriggers[];
extern struct Trigger gTrialRoad_BushPillarStageTriggers[];
extern struct Trigger gTrialRoad_PoundStageTriggers[];
extern struct Trigger gTrialRoad_CrumblingStageTriggers[];
extern struct Trigger gTrialRoad_LogStageTriggers[];
extern struct Trigger gTrialRoad_FountainStageTriggers[];
extern struct Trigger gTrialRoad_SandStageTriggers[];

void InventorySnapshot_Restore();

void TrialRoad_Idle(void);

s32 TrialRoad_RivalIsAhead(s32 fallback);

/// Inventory_ClearSlot through its far call: empties an inventory slot and
/// returns 2.
s32 Inventory_ClearSlot(s32 owner, s32 slot);

extern s16 gTrialRoad_CrumblingPillars[];

void TrialRoad_ReachPeakFirst(void);
void TrialRoad_ReachPeakSecond(void);

void TrialRoad_LowerPillar(s32 id);

void TrialRoad_RaisePillar(s32 id);

void TrialRoad_BlockLogs(void);

void TrialRoad_StartFountains(s32 a, s32 b);

void Tightrope_Initialize(void);
void Func_02005a10(s32 a, s32 b, s32 c);

extern u32 gTrialRoad_GaspHopScript[];

extern u16 gTrialRoad_PaletteCycleColors[];

void PaletteCycle_StepAt(void);
void TrialRoad_OpenSummitGate();
void TrialRoad_PlaceMoapaAtPeak(void);
void TrialRoad_BeatMoapa(void);
void TrialRoad_LoseToMoapa(void);
void TrialRoad_ShowTraps(void);

void TrialRoad_SetUpCave(void);

void TrialRoad_SetUpPoolRoom(void);

extern u32 gTrialRoad_RivalRecordings[];
s32 TrialRoad_RivalIsWaiting(void);
void TrialRoad_PlayRivalRecording(s32 a);
void TrialRoad_DealStageOrder(u32 seed);

void Map_CopyRawCellsRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void TrialRoad_ShowProgress(s32 a, s32 b);

void TrialRoad_OpenLeaderDoor(s32 side, s32 phase);

void FieldCamera_WaitForArrival(void);

extern u32 gTrialRoad_LeftKnightEntersScript[];

extern u32 gTrialRoad_RightKnightEntersScript[];

extern u32 gTrialRoad_LeftKnightToChestScript[];
extern u32 gTrialRoad_RightKnightToChestsScript[];

extern u32 gTrialRoad_RightKnightLeavesScript[];

void Import_DecompressTaggedLz(const void *source, void *destination);
s32 Func_02005a18(struct Actor *object);
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
void Actor_DisableById(s32 id);
s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);
struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

void TrialRoad_SpawnRubble(s32 value);
void TrialRoad_SetLeaderCellShape(s32 offset, s32 layer);
void TrialRoad_SpawnDustRing(void);
void TrialRoad_SetActorCellShape(s32 a, s32 b, s32 c);

u32 Sprite_ShareVramCacheEntry(struct FieldSprite *sprite, u32 frame);

void TrialRoad_DriftRubble(union FieldObject *object);

void Actor_StartIdle(struct Actor *object);

extern const u16 gTrialRoad_ProgressPalette[];

void TrialRoad_MoveRival(void);
void TrialRoad_SetUpPillarStage(void);
void TrialRoad_SetUpTorchStage(void);
void TrialRoad_SetUpBushPillarStage(void);
void TrialRoad_SetUpPoundStage(void);
void TrialRoad_SetUpCrumblingStage(void);
void TrialRoad_SetUpLogStage(void);
void TrialRoad_SetUpFountainStage(void);

void TrialRoad_SetUpSandStage(void);

void TrialRoad_UpdateFountains(void);

#endif
