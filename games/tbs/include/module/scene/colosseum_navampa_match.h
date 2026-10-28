#ifndef ALCHEMY_MODULE_MAP_COLOSSEUM_NAVAMPA_MATCH_H
#define ALCHEMY_MODULE_MAP_COLOSSEUM_NAVAMPA_MATCH_H

#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *ColosseumNavampaMatch_GetEntrances(void);
const struct SceneExitRegion *ColosseumNavampaMatch_GetRegions(void);
const u32 *ColosseumNavampaMatch_GetExits(void);
const struct ActorPlacement *ColosseumNavampaMatch_GetActorPlacements(void);
const struct Trigger *ColosseumNavampaMatch_GetTriggers(void);
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
extern u32 gPushedActor_DirectionSteps[];
extern struct SceneEntrance gColosseumNavampaMatchEntrances[];
extern u32 gColosseumNavampaMatchExits[];
extern struct ActorPlacement gColosseumNavampaMatchActorPlacements[];
extern s32 gColosseumNavampaMatch_WallWedgePatternIndex;
extern s32 gColosseumNavampaMatch_WallWedgeTimer;
extern struct Trigger gColosseumNavampaMatchTriggers[];
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
