#ifndef ALCHEMY_FACING_H
#define ALCHEMY_FACING_H

/* The constant tables of facing.c: which picture of an animation an actor shows
   for each way it faces, and beside them the sprite dissolve's pixel order, the
   fixed camera's matrix, a floating actor's bob, the actor scripts' end script
   and the World Map camera's lean for the direction pad. */

#include "fixed_math.h"
#include "types.h"

extern const Uint8 AnimationFacing_OnePicture[16];
extern const Uint8 AnimationFacing_FrontBackSwapped[8];
extern const Uint8 AnimationFacing_FrontBack[8];
extern const Uint8 AnimationFacing_ThreePictures[16];
extern const Uint8 AnimationFacing_FivePictures[16];
extern const Uint8 AnimationFacing_EightPictures[8];
extern const Uint8 AnimationFacing_EightPicturesMirrored[8];
extern const Uint8 AnimationFacing_FourPictures[64];
extern const Uint8 AnimationFacing_SixPictures[64];

extern const Uint8	SpriteDissolve_PixelOrder[64];
extern const Uint16 SpriteDissolve_PixelOrderTail;
extern const FIXED	Camera_FixedViewMatrix[12];
extern const Sint32 ActorScript_CycleTable[32];
extern const Sint32 ActorScript_EndScript[1];
extern const Sint16 ActorScript_EndScriptTail[8];

extern const Sint32 WorldMap_KeyYaws[16];

/* A facing table entry: the picture to draw, counted from the sprite
   animation's base picture (bits 0-2), mirrored when bit 7 is set. */
#define FACING_MIRRORED_BIT 0x80
#define MIRRORED(picture)	(FACING_MIRRORED_BIT | (picture))

/* A scene's key-heading table entry when no direction is held, or only an
   opposite pair, where the others are facings. */
#define KEY_HEADING_NONE (-1)

/* WorldMap_KeyHeadings' entry when no direction is held, or only an opposite
   pair: no heading, since every real one is a multiple of an eighth turn. */
#define WORLD_MAP_KEY_HEADING_NONE 65535

#endif
