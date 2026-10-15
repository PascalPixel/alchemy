#ifndef ALCHEMY_TOLBI_SHIP_MAST_H
#define ALCHEMY_TOLBI_SHIP_MAST_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum
{
	ITEM_ANCHOR_CHARM = 232
};

extern s32 gTolbiShipMast_DriftY;
extern s32 gTolbiShipMast_DriftX;
extern s32 gTolbiShipMast_SwayY;
extern s32 gTolbiShipMast_SwayUnused;
extern s32 gTolbiShipMast_CameraCenter[2];
extern s32 gTolbiShipMast_CameraOrigin[2];
extern s32 gTolbiShipMast_SwayX;

enum
{
	FLAG_MAST_923 = 0x923,
	FLAG_MAST_922 = 0x922,
	FLAG_MAST_924 = 0x924
};

#endif
