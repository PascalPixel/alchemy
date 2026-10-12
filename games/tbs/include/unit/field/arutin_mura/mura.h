#ifndef ALCHEMY_UNIT_FIELD_ARUTIN_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_ARUTIN_MURA_MURA_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;

const struct SceneEntrance *Scene_GetEntrances(void);
s32 SceneData_ReturnZero(void);
u8 *SceneData_GetMessageTable(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
s32 Scene_Initialize(void);
extern s32 gArutinMuraPlacements2_Data[];
extern struct SceneEntrance gArutinMuraEntrancesOther[];
extern struct SceneEntrance gArutinMuraEntrances1[];
extern struct SceneEntrance gArutinMuraEntrances2[];
extern u32 Placement_Messages[];
extern struct ScenePlacement gArutinMuraPlacementsOther[];
extern struct ScenePlacement gArutinMuraPlacements1[];
extern struct ScenePlacement gArutinMuraPlacements2[];
extern struct SceneEvent gArutinMuraEventsOther[];
extern struct SceneEvent gArutinMuraEvents1[];
extern struct SceneEvent gArutinMuraEvents2[];
extern u16 ArutinMura_TriggerTable1[];
extern u16 ArutinMura_TriggerTable0[];
extern u16 ArutinMura_TriggerTable5[];
extern u16 ArutinMura_TriggerTable6[];
extern u16 ArutinMura_TriggerTable2[];
extern u16 ArutinMura_TriggerTable3[];
extern u16 ArutinMura_TriggerTable4[];
extern struct MapCellAnim ArutinMura_TriggerTable[];

#endif
