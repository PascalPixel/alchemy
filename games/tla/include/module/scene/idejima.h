#ifndef ALCHEMY_MODULE_SCENE_IDEJIMA_H
#define ALCHEMY_MODULE_SCENE_IDEJIMA_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

extern struct SceneEntrance gIdejimaEntrances[];
extern struct Trigger gIdejimaEvents[];
extern struct Trigger gIdejimaEventsEntrance1[];
extern struct Trigger gIdejimaEventsWake[];
extern struct ActorPlacement gIdejimaPlacements[];
extern s32 gData_02003650[];
extern u32 gData_02003734[];
extern u32 gData_0200336c[];
extern u32 gData_020033a8[];
extern u32 gData_02003400[];
extern u32 gData_0200346c[];
extern u32 gData_020034e4[];
extern u32 gData_0200350c[];

void Func_0200161c(void);
void DriftScene_Prepare(void);

extern u32 gData_02003534[];
extern u32 gData_02003578[];
extern u32 gData_020035bc[];
extern u32 gData_02003600[];

void Func_0200153c(void);

void Func_02001558(void);
void Func_020015a0(void);

void Func_02002fd4(void);

void Party_RestoreHpPp(s32 a);

void Func_020000f0(void);

void Func_020002b4(void);

void Func_0200049c(void);

void Func_020006a8(void);

void Idejima_RunAlexDeparture(void);

void Import_ActorPlaceAtActor();
void Func_02003320();
extern s16 gData_02003c14;
extern s16 gData_02003990[2][160];

#endif
