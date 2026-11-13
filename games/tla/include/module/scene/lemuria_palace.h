#ifndef ALCHEMY_MODULE_SCENE_LEMURIA_PALACE_H
#define ALCHEMY_MODULE_SCENE_LEMURIA_PALACE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

extern s32 gLemuriaPalace_Doors[];
extern s32 gLemuriaPalace_InnerDoor[];

void LemuriaPalace_DrawMaps(void);

void Field_StartColorTransition();

extern s16 gLemuriaPalace_DoorOpening[];

#endif
