#ifndef ALCHEMY_UNIT_MAP_COLOSSEUM_NAVAMPA_MATCH_H
#define ALCHEMY_UNIT_MAP_COLOSSEUM_NAVAMPA_MATCH_H

#include "types.h"
#include "map_tables.h"

const struct MapWarpDest *ColosseumNavampaMatch_GetWarpDests(void);
const struct MapWarpRegion *ColosseumNavampaMatch_GetRegions(void);
const u32 *ColosseumNavampaMatch_GetWarps(void);
const struct ObjectEventTemplate *
ColosseumNavampaMatch_GetObjectEventTemplates(void);
const struct MapEvent *ColosseumNavampaMatch_GetEvents(void);
s32 ColosseumNavampaMatch_Initialize(void);
extern const s32 gColosseumNavampaMatch_SidewalkLogOffBelt[];
extern const s8 gColosseumNavampaMatch_WallWedgePattern[][6];
extern const u16 gColosseumNavampaMatch_FacingAngles[];
extern const s32 gColosseumNavampaMatch_SidewalkLogScript[];
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColossoLetteringScriptDigit[];
extern const s16 gColossoLetteringScriptLose[];
extern char gHexDigits[20];
extern s32 gColosseumNavampaMatch_PrizeEffectScript[];
extern s32 gColosseumNavampaMatch_PrizeScript[];
extern u32 gPushedObject_DirectionSteps[];
extern struct MapWarpDest gColosseumNavampaMatchWarpDests[];
extern u32 gColosseumNavampaMatchWarps[];
extern struct ObjectEventTemplate gColosseumNavampaMatchObjectEventTemplates[];
extern s32 gColosseumNavampaMatch_WallWedgePatternIndex;
extern s32 gColosseumNavampaMatch_WallWedgeTimer;
extern struct MapEvent gColosseumNavampaMatchEvents[];
extern s32 gColosseumNavampaMatch_OpponentAfterBattle[];
extern s32 gColosseumNavampaMatch_LeaderActionA[];
extern s32 gColosseumNavampaMatch_LeaderActionB[];
extern s32 gColosseumNavampaMatch_LeaderActionC[];
extern s32 gColosseumNavampaMatch_LeaderScaleScript[];
extern s32 gColossoParticleKinds[];
extern s16 gColosso_LetteringSlot;
extern s16 gColossoLetteringScriptTitle[];
extern s16 gColossoLetteringScriptGo[];
extern s16 gColossoLetteringScriptWon[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_OpponentFinishScript[];
extern s32 gColosseumNavampaMatch_PlaceScript[];

#endif
