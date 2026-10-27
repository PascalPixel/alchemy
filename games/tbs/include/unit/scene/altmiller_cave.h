#ifndef ALCHEMY_UNIT_MAP_ALTMILLER_CAVE_H
#define ALCHEMY_UNIT_MAP_ALTMILLER_CAVE_H

#include "types.h"
#include "scene_tables.h"

struct Trigger;
struct ActorTemplate;
struct LightWheel;

const struct SceneEntrance *AltmillerCave_GetEntrances(void);
const struct SceneExitRegion *AltmillerCave_GetRegions(void);
const u32 *AltmillerCave_GetExits(void);
const struct ActorTemplate *AltmillerCave_GetActorTemplates(void);
const struct Trigger *AltmillerCave_GetTriggers(void);
s32 AltmillerCave_Initialize(void);
extern s32 gPushedActor_DirectionSteps[];
extern u32 gPushedActor_FootprintKinds[];
extern s32 gPushedActor_FootprintBounds[];
extern struct LightWheel *gAltmillerCave_LightWheel;
extern struct SceneEntrance gAltmillerCave_Entrances[];
extern u32 gAltmillerCave_Exits[];
extern struct ActorTemplate gAltmillerCave_ActorTemplatesOther[];
extern struct ActorTemplate gAltmillerCave_BabiActorTemplates[];
extern struct ActorTemplate gAltmillerCave_SquallActorTemplates[];
extern struct ActorTemplate gAltmillerCave_DraughtActorTemplates[];
extern u32 gAltmillerCave_PulseScales[];
extern s32 gAltmillerCave_GuardsArrivePath[];
extern s32 gAltmillerCave_SecondGuardLeavePath[];
extern s32 gAltmillerCave_FirstGuardLeavePath[];
extern s32 gAltmillerCave_BabiLeavePath[];
extern struct Trigger gAltmillerCave_TriggersOther[];
extern struct Trigger gAltmillerCave_BabiTriggers[];
extern struct Trigger gAltmillerCave_VialChestTriggers[];
extern struct Trigger gAltmillerCave_SquallTriggers[];
extern struct Trigger gAltmillerCave_CookieChestTriggers[];
extern struct Trigger gAltmillerCave_DraughtTriggers[];

#endif
