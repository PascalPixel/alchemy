/* @file */
/* The Trial Road's race progress, kept in the inventory snapshot, and the */
/* declarations the Trial Road shares with the Izumo Ruins and Magma Rock's */
/* inside: the calls their overlays make, some through their own import rows. */
#ifndef ALCHEMY_TRIAL_ROAD_H
#define ALCHEMY_TRIAL_ROAD_H

#include "types.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "field/actor.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "field/event_inline.h"

/* Trial Road's progress, kept in the inventory snapshot while the trial runs.
 */
typedef struct TRIAL_ROAD_STATE {
	s16 order[8]; /* the dealt stage patterns, two to a stage */
	s16 pattern;
	s16 mirrored;
	s16 rivalTmr;	/* frames the rival has spent at its current chest */
	s16 stage[2];	/* the stages the leader (0) and the rival (1) are on */
	s16 leaderBest; /* the furthest stage the leader has reached */
	s16 rivalBest;	/* the furthest stage the rival has reached */
} TRIAL_ROAD_STATE;

/* TrialRoadStateWork's place in InventorySnapshot, past what */
/* gmInventorySnapshotSave keeps of Felix's party: the sentinel, the four */
/* inventories, the shortcuts and the party order. */
#define INVENTORY_SNAPSHOT_TRIAL_ROAD 136
#define TrialRoadStateWork                                                     \
	((TRIAL_ROAD_STATE *)(InventorySnapshot + INVENTORY_SNAPSHOT_TRIAL_ROAD))

s32 fdMapGetTerrainHeight(s32 layer, s32 x, s32 z);

/* Cyclone scatters the bushes or weeds it touches (fdCycloneCopyTouchedCells,
 */
/* through the overlay's own import row). */
void scTrialRoadCopyTouchedCells(s32 x, s32 z, s32 layerIndex, s32 width,
								 s32 height, s32 toX, s32 toZ);
/* A Mad Plant springs out of the cleared bushes and leaps at the leader */
/* (fdMadPlantLeapAtLeader, through the overlay's own import row). */
void syImportMadPlantLeapAtLeader(s32 sprite, s32 x, s32 z);

/* Cyclone scatters the bushes or weeds it touches (fdCycloneCopyTouchedCells):
 */
/* event_layer.h's syFieldCycloneCopyTouchedCells, through the import this */
/* scene calls the function by. */
static inline void syFieldCycloneCopyTouchedCells(s32 x, s32 z, s32 layerIndex,
												  s32 width, s32 height,
												  s32 toX, s32 toZ) {
	scTrialRoadCopyTouchedCells(x, z, layerIndex, width, height, toX, toZ);
}

#endif
