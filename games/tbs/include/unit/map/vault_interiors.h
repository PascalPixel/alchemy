#ifndef ALCHEMY_UNIT_MAP_VAULT_INTERIORS_H
#define ALCHEMY_UNIT_MAP_VAULT_INTERIORS_H

#include "types.h"
#include "map_tables.h"
#include "map_header.h"
#include "static_assert.h"

struct MapEvent;
struct ObjectEventTemplate;

/// Each stop links to up to three others, with a heading for each connection.
struct VaultInteriorsStopConnection {
  u8 stopIndex; ///< the stop it leads to; 0xff for no connection
  u8 facing;    ///< the heading toward it, a facing's high byte
  u8 padding[2];
};

/// A stop on the route network, two cells square, with its links to others.
struct VaultInteriorsStop {
  u8 gridX; ///< 8-pixel cells from x 64
  u8 gridZ; ///< 8-pixel cells from z 624
  u8 padding[2];
  struct VaultInteriorsStopConnection connections[3];
};

STATIC_ASSERT(sizeof(struct VaultInteriorsStopConnection) == 4);
STATIC_ASSERT(sizeof(struct VaultInteriorsStop) == 16);

const struct MapWarpDest *Map_GetWarpDests(void);
const struct MapWarpRegion *Map_GetWarpRegions(void);
const u32 *Map_GetWarps(void);
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void);
const struct MapEvent *Map_GetEvents(void);
s32 Map_Initialize(void);
extern struct VaultInteriorsStop gVaultInteriors_Stops[];
extern s32 gVaultInteriors_ActionTable[];
extern s32 gVaultInteriors_FirstThiefEntryScript[];
extern s32 gVaultInteriors_SecondThiefEntryScript[];
extern s32 gVaultInteriors_VaultScriptA[];
extern s32 gVaultInteriors_VaultScriptB[];
extern s32 gVaultInteriors_VaultScriptC[];
extern s32 gVaultInteriors_VaultScriptD[];
extern s32 gVaultInteriors_VaultScriptE[];
extern s32 gVaultInteriors_SuspectToWestBedHead[];
extern s32 gVaultInteriors_SuspectRoundToWestBedHead[];
extern s32 gVaultInteriors_SuspectToMiddleBedHead[];
extern s32 gVaultInteriors_SuspectToEastBedHead[];
extern s32 gVaultInteriors_SuspectRoundToEastBedHead[];
extern s32 gVaultInteriors_SuspectToWestBedFoot[];
extern s32 gVaultInteriors_SuspectRoundToWestBedFoot[];
extern s32 gVaultInteriors_SuspectToMiddleBedFoot[];
extern s32 gVaultInteriors_SuspectToEastBedFoot[];
extern s32 gVaultInteriors_SuspectRoundToEastBedFoot[];
extern s32 gVaultInteriors_SuspectAcrossToWestOfBeds[];
extern s32 gVaultInteriors_SuspectToWestOfBeds[];
extern s32 gVaultInteriors_SuspectAcrossToEastOfBeds[];
extern s32 gVaultInteriors_SuspectToEastOfBeds[];
extern s32 gVaultInteriors_SuspectToSouthWestCorner[];
extern s32 gVaultInteriors_SuspectAlongToSouthWestCorner[];
extern s32 gVaultInteriors_SuspectToSouthEastCorner[];
extern s32 gVaultInteriors_SuspectAlongToSouthEastCorner[];
extern struct MapWarpDest gVaultInteriors_WarpDests[];
extern struct MapWarpRegion gVaultInteriors_Regions[];
extern u32 gVaultInteriors_Warps[];
extern struct ObjectEventTemplate gVaultInteriors_ObjectEventTemplates[];
extern struct ObjectEventTemplate gVaultInteriors_LoftObjectEventTemplates[];
extern struct MapEvent gVaultInteriors_Events[];
extern struct MapEvent gVaultInteriors_LoftEvents[];
extern struct MapEvent gVaultInteriors_EventsAfterThieves[];
extern s32 *gVaultInteriors_StepActions[];
extern s32 *gVaultInteriors_IdleActions[];
extern s32 *gVaultInteriors_ResponseScripts[];

#endif
