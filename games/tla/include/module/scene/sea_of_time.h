#ifndef ALCHEMY_MODULE_SCENE_SEA_OF_TIME_H
#define ALCHEMY_MODULE_SCENE_SEA_OF_TIME_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "scene_tables.h"

// The Sea of Time's frame callback: rotates two runs of the background palette,
// pulses the red of colour 15 (or steps it with the alarm), copies the palette
// and ripples the layer's horizontal scroll line by line.
/// The event work's fields this function reads that SeaOfTimeEventWork lacks.
struct EventWork_02001650 {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0x4a];
  u16 unk1b2;
};

extern struct ActorPlacement gSeaOfTime_Room2Placements[];
extern u32 gData_020022c4[];

void Func_02000c50(void);

extern struct Trigger gSeaOfTime_Room1Triggers[];

extern struct Trigger gData_0200242c[];
extern struct Trigger gSeaOfTime_Triggers[];
extern s16 gData_0200257c[1];
void Func_0200054c(void);

void Func_02000a80(void);

extern u32 gData_0200205c[];
extern u32 gData_02002098[];
extern u32 gData_020020d4[];
extern s32 gData_02002154[];

#endif
