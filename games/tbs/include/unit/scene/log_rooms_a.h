#ifndef ALCHEMY_UNIT_MAP_LOG_ROOMS_A_H
#define ALCHEMY_UNIT_MAP_LOG_ROOMS_A_H

#include "types.h"
#include "map_tables.h"
#include "scene_header.h"
#include "static_assert.h"

struct MapEvent;
struct ActorTemplate;

/// Each stop links to up to three others, with a heading for each connection.
struct LogRoomsAStopConnection {
  u8 stopIndex; ///< the stop it leads to; 0xff for no connection
  u8 facing;    ///< the heading toward it, a facing's high byte
  u8 padding[2];
};

/// A stop on the route network, two cells square, with its links to others.
struct LogRoomsAStop {
  u8 gridX; ///< 8-pixel cells from x 64
  u8 gridZ; ///< 8-pixel cells from z 624
  u8 padding[2];
  struct LogRoomsAStopConnection connections[3];
};

STATIC_ASSERT(sizeof(struct LogRoomsAStopConnection) == 4);
STATIC_ASSERT(sizeof(struct LogRoomsAStop) == 16);

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorTemplate *Scene_GetActorTemplates(void);
const struct MapEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern struct LogRoomsAStop gLogRoomsA_Stops[];
extern s32 gLogRoomsA_ActionTable[];
extern s32 gLogRoomsA_FirstThiefEntryScript[];
extern s32 gLogRoomsA_SecondThiefEntryScript[];
extern s32 gLogRoomsA_VaultScriptA[];
extern s32 gLogRoomsA_VaultScriptB[];
extern s32 gLogRoomsA_VaultScriptC[];
extern s32 gLogRoomsA_VaultScriptD[];
extern s32 gLogRoomsA_VaultScriptE[];
extern s32 gLogRoomsA_SuspectToWestBedHead[];
extern s32 gLogRoomsA_SuspectRoundToWestBedHead[];
extern s32 gLogRoomsA_SuspectToMiddleBedHead[];
extern s32 gLogRoomsA_SuspectToEastBedHead[];
extern s32 gLogRoomsA_SuspectRoundToEastBedHead[];
extern s32 gLogRoomsA_SuspectToWestBedFoot[];
extern s32 gLogRoomsA_SuspectRoundToWestBedFoot[];
extern s32 gLogRoomsA_SuspectToMiddleBedFoot[];
extern s32 gLogRoomsA_SuspectToEastBedFoot[];
extern s32 gLogRoomsA_SuspectRoundToEastBedFoot[];
extern s32 gLogRoomsA_SuspectAcrossToWestOfBeds[];
extern s32 gLogRoomsA_SuspectToWestOfBeds[];
extern s32 gLogRoomsA_SuspectAcrossToEastOfBeds[];
extern s32 gLogRoomsA_SuspectToEastOfBeds[];
extern s32 gLogRoomsA_SuspectToSouthWestCorner[];
extern s32 gLogRoomsA_SuspectAlongToSouthWestCorner[];
extern s32 gLogRoomsA_SuspectToSouthEastCorner[];
extern s32 gLogRoomsA_SuspectAlongToSouthEastCorner[];
extern struct SceneEntrance gLogRoomsA_Entrances[];
extern struct SceneExitRegion gLogRoomsA_Regions[];
extern u32 gLogRoomsA_Exits[];
extern struct ActorTemplate gLogRoomsA_ActorTemplates[];
extern struct ActorTemplate gLogRoomsA_LoftActorTemplates[];
extern struct MapEvent gLogRoomsA_Events[];
extern struct MapEvent gLogRoomsA_LoftEvents[];
extern struct MapEvent gLogRoomsA_EventsAfterThieves[];
extern s32 *gLogRoomsA_StepActions[];
extern s32 *gLogRoomsA_IdleActions[];
extern s32 *gLogRoomsA_ResponseScripts[];

#endif
