/* The Trial Road's race progress, kept in the inventory snapshot, and the
   declarations the Trial Road shares with the Izumo Ruins and Magma Rock's
   inside: the calls their overlays make, some through their own import rows. */
#ifndef ALCHEMY_TRIAL_ROAD_H
#define ALCHEMY_TRIAL_ROAD_H

#include "types.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "field/actor.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "field/script_inline.h"

/* Trial Road's progress, kept in the inventory snapshot while the trial
   runs. */
typedef struct TRIAL_ROAD_STATE {
	Sint16 order[8]; /* the dealt stage patterns, two to a stage */
	Sint16 pattern;
	Sint16 mirrored;
	Sint16 rivalTimer; /* ticks the rival has spent at its current chest */
	Sint16 stage[2];   /* the stages the leader (0) and the rival (1) are on */
	Sint16 leaderBest; /* the furthest stage the leader has reached */
	Sint16 rivalBest;  /* the furthest stage the rival has reached */
} TRIAL_ROAD_STATE;

/* TrialRoadStateWork's place in InventorySnapshot, past what
   gmInventorySnapshotSave keeps of Felix's party: the sentinel, the four
   inventories, the shortcuts and the party order. */
#define INVENTORY_SNAPSHOT_TRIAL_ROAD 136
#define TrialRoadStateWork \
	((TRIAL_ROAD_STATE *)(InventorySnapshot + INVENTORY_SNAPSHOT_TRIAL_ROAD))

Sint32 fdMapGetTerrainHeight(Sint32 layer, Sint32 x, Sint32 z);

/* Cyclone scatters the bushes or weeds it touches
   (fdCycloneCopyTouchedMetatiles, through the overlay's own import row). */
void scTrialRoadCopyTouchedMetatiles(Sint32 x, Sint32 z, Sint32 layerIndex,
	Sint32 width, Sint32 height, Sint32 toX, Sint32 toZ);
/* A Mad Plant springs out of the cleared bushes and leaps at the leader
   (fdMadPlantLeapAtLeader, through the overlay's own import row). */
void syImportMadPlantLeapAtLeader(Sint32 sprite, Sint32 x, Sint32 z);

/* Cyclone scatters the bushes or weeds it touches
   (fdCycloneCopyTouchedMetatiles):
   script_layer.h's syFieldCycloneCopyTouchedMetatiles, through the import this
   scene calls the function by. */
static inline void syFieldCycloneCopyTouchedMetatiles(Sint32 x, Sint32 z,
	Sint32 layerIndex, Sint32 width, Sint32 height, Sint32 toX, Sint32 toZ)
{
	scTrialRoadCopyTouchedMetatiles(x, z, layerIndex, width, height, toX, toZ);
}

#endif
