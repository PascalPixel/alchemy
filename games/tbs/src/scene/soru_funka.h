#ifndef FUNKA_H
#define FUNKA_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

enum CollapseActor
{
	ACTOR_SUKURETA = ACTOR_FIRST_PLACED + 1,
	ACTOR_SATUROS,
	ACTOR_MENARDI,
	ACTOR_GARCIA = ACTOR_FIRST_PLACED + 5,
	ACTOR_ALEX
};

#include "facing_object.h"

typedef struct
{
	u8	filler0[6];
	u16 unk6;
	u8	filler8[72];
	u8 *unk50;
} Ent;

typedef struct
{
	u8 unk[9];
	u8 unk0 : 2;
	u8 mode : 2;
	u8 unk4 : 4;
} SpriteMode;

typedef struct
{
	u8	filler0[12];
	s32 unkC;
	u8	filler10[8];
	s32 unk18;
	s32 unk1C;
	u8	filler20[28];
	s32 unk3C;
} ArcEffect;

typedef struct
{
	u8	filler0[8];
	s32 unk8;
	u8	fillerC[4];
	s32 unk10;
	u8	filler14[16];
	s32 unk24;
	u8	filler28[4];
	s32 unk2C;
	u8	filler30[8];
	s32 unk38;
	u8	filler3C[4];
	s32 unk40;
} ViewCenter;

extern u8 *gParticleWork[];

extern s32 gEmberState[16];
extern s32 gEmberTimer;
extern s32 gArcEffectTimers[10];
extern s32 gEmberMask;
extern s32 gEmberLevel;
extern s32 gEmberLevelTimer;

#endif
