#ifndef ALCHEMY_UNIT_FIELD_RUNPA_DOU_DOU_H
#define ALCHEMY_UNIT_FIELD_RUNPA_DOU_DOU_H

#include "types.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
void HiddenPuddle_Freeze(void);
void SouthPuddle_Freeze(void);
void Gate_DrawPropped(void);
void GatePuddle_Freeze(void);
void NorthPuddle_Freeze(void);
void Gate_Lower(void);
void LoweringSwitch_Flip(void);
void Gate_Raise(void);
void RaisingSwitch_Flip(void);
void Reunion_Begin(void);
void Reunion_Converse(void);
void WagonChoice_Run(void);
u8 Leader_AnswersYes(void);
u8 Gerald_AsksIfNotRiding(void);
u8 Gerald_AsksAboutUnfinishedBusiness(void);
u8 Party_ConfirmsStaying(void);
u8 Bunza_AsksAboutUnfinishedBusiness(void);
u8 Gerald_ChecksNothingLeftToDo(void);
u8 Mia_AsksIfRidingAfterAll(void);
u8 Bunza_CannotWait(void);
u8 Gerald_AsksAboutThingsToDo(void);
void Party_StaysBehind(void);
void Party_RidesWagon(void);
s32 Scene_Initialize(void);

#endif
