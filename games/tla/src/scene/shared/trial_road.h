#ifndef ALCHEMY_TRIAL_ROAD_H
#define ALCHEMY_TRIAL_ROAD_H

#include "types.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "field/event_inline.h"

/// Trial Road's progress, kept in the inventory snapshot while the trial runs.
struct TrialRoadState {
  s16 order[8]; ///< the dealt stage patterns, two to a stage
  s16 pattern;
  s16 mirrored;
  s16 rivalTimer; ///< frames the rival has spent at its current chest
  s16 stage[2];   ///< the stages the leader (0) and the rival (1) are on
  s16 leaderBest; ///< the furthest stage the leader has reached
  s16 rivalBest;  ///< the furthest stage the rival has reached
};

extern u8 gInventorySnapshot[];

/// gTrialRoadState's place in gInventorySnapshot, past what
/// InventorySnapshot_Save keeps of Felix's party: the sentinel, the four
/// inventories, the shortcuts and the party order.
#define INVENTORY_SNAPSHOT_TRIAL_ROAD 136
#define gTrialRoadState                                                        \
  ((struct TrialRoadState *)(gInventorySnapshot +                              \
                             INVENTORY_SNAPSHOT_TRIAL_ROAD))

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

void Actor_StartScript(struct Actor *actor, const void *script);
void Actor_JumpDownLedge(s32 actorId, s32 pose, s32 param);
s32 Field_FindNearbyTalkTarget(s32 id);
s32 Import_FieldTryJumpForward(void);

void Field_RequestBattleFromEncounterTable(s32 row, s32 column);
void TrialRoad_CopyTouchedCells(s32 x, s32 z, s32 layerIndex, s32 width,
                                s32 height, s32 toX, s32 toZ);
void Import_MadPlantLeapAtLeader(s32 sprite, s32 x, s32 z);

static __inline__ void Cyclone_CopyTouchedCells_(s32 x, s32 z, s32 layerIndex,
                                                 s32 width, s32 height, s32 toX,
                                                 s32 toZ) {
  TrialRoad_CopyTouchedCells(x, z, layerIndex, width, height, toX, toZ);
}

/// The store-flag form of a logical not.
static __inline__ s32 IsZero(s32 value) { return !value; }

#endif
