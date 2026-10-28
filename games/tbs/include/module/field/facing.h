#ifndef ALCHEMY_MODULE_FIELD_FACING_H
#define ALCHEMY_MODULE_FIELD_FACING_H

#include "types.h"

extern const u8 gAnimationFacing_OnePicture[16];
extern const u8 gAnimationFacing_FrontBackSwapped[8];
extern const u8 gAnimationFacing_FrontBack[8];
extern const u8 gAnimationFacing_ThreePictures[16];
extern const u8 gAnimationFacing_FivePictures[16];
extern const u8 gAnimationFacing_EightPictures[8];
extern const u8 gAnimationFacing_EightPicturesMirrored[8];
extern const u8 gAnimationFacing_FourPictures[64];
extern const u8 gAnimationFacing_SixPictures[64];

extern const u8 gSpriteDissolve_PixelOrder[64];
extern const u16 gSpriteDissolve_PixelOrderTail;
extern const s32 gCamera_FixedViewMatrix[12];
extern const s32 gActorScript_CycleTable[32];
extern const s32 gActorScript_EndScript[1];
extern const s16 gActorScript_EndScriptTail[8];

extern const s32 gWorldMap_KeyYaws[16];

/// gWorldMap_KeyHeadings' entry when no direction is held, or only an
/// opposite pair: no heading, since every real one is a multiple of an eighth
/// turn.
#define WORLD_MAP_KEY_HEADING_NONE 0xffff

#endif
