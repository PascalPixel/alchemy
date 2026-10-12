#ifndef ALCHEMY_FUNE_H
#define ALCHEMY_FUNE_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum
{
	ITEM_ANCHOR_CHARM = 232
};

extern s32 FuneHobashira_DriftY;
extern s32 FuneHobashira_DriftX;
extern s32 FuneHobashira_SwayY;
extern s32 FuneHobashira_SwayUnused;
extern s32 FuneHobashira_CameraCenter[2];
extern s32 FuneHobashira_CameraOrigin[2];
extern s32 FuneHobashira_SwayX;

enum
{
	FLAG_MAST_923 = 0x923,
	FLAG_MAST_922 = 0x922,
	FLAG_MAST_924 = 0x924
};

#endif
