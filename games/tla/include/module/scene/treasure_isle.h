#ifndef ALCHEMY_MODULE_SCENE_TREASURE_ISLE_H
#define ALCHEMY_MODULE_SCENE_TREASURE_ISLE_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "module/scene/scene.h"
#include "scene_tables.h"

struct TreasureIsleCell;

extern struct ActorPlacement gTreasureIsle_Placements[];

extern struct ActorPlacement gTreasureIsle_Room2Placements[];

extern struct ActorPlacement gTreasureIsle_Room3Placements[];
extern struct ActorPlacement gTreasureIsle_Room4Placements[];
extern struct ActorPlacement gTreasureIsle_Room5Placements[];
extern u32 gData_02001128[];

// Import stubs: Event_Start and Event_End, and the pending
// actions' resolution.
void Func_02000fcc(void);

extern struct TreasureIsleCell gData_0200117c[];
extern struct TreasureIsleCell gData_020011cc[];
extern struct Trigger gTreasureIsle_Room2Triggers[];
extern struct Trigger gTreasureIsle_Room3Triggers[];
extern s32 gTreasureIsle_Room4Triggers[];
extern struct Trigger gTreasureIsle_Room5Triggers[];

void Func_020003fc(struct TreasureIsleCell *cells, s16 index);

void Func_02000ba0(void);

void Func_0200105c(void);

s32 Func_02001064(s32 a, s32 b, s32 c);

s32 Func_0200102c();

void Func_02000864(void);

void Func_02000870(void);

void Func_02000914(void);

void Func_020009a0(void);

void Func_020009d0(void);

void Func_020009fc(void);
void Func_02000a2c(void);

void Func_02000a5c(void);

void TreasureIsle_MeetWaterGuardian(void);
extern u32 gData_02001074[];
extern u32 gData_020010b0[];
extern u32 gData_020010ec[];
extern u32 gData_0200131c[];
extern u32 gData_020014e0[];

void Func_020005ac(void);

void FieldCamera_WaitForArrival(void);

#endif
