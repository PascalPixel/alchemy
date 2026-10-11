#include "mem.h"
#ifndef GUARD_STAGED_MOTION_H
#define GUARD_STAGED_MOTION_H

#include "types.h"
#include "callback_scheduler.h"

#define ACTOR_A 12
#define ACTOR_B 13
#define ACTOR_C 14
#define ACTOR_D 11

#include "staged_actor.h"
#include "field_event.h"
#include "field_scene.h"

enum
{
	ITEM_NUT = 181
};

enum
{
	ACTOR_DORA = 21
};

struct Resource373Actor
{
	u8	unknown_00[8];
	s32 field08;
	s32 field0c;
	s32 field10;
	u8	unknown_14[0x41];
	u8	flag55;
};

struct SourceEntity
{
	s32 f00;
	s32 f04;
	s32 f08;
	s32 f0c;
	s32 f10;
};

struct ParticleRecord
{
	u8 f00[9];
	u8 f09;
};

struct StagedParticle
{
	u8					   f00[0x48];
	s32					   f48;
	s32					   f4c;
	struct ParticleRecord *f50;
	u8					   f54;
	u8					   f55;
	u8					   f56[8];
	u16					   f5e;
};

struct Resource373Emitter
{
	u8	unknown_00[6];
	u16 field06;
	s32 x;
	s32 y;
	s32 z;
	u8	unknown_14[4];
	s32 field18;
	s32 field1c;
	u8	unknown_20[4];
	s32 field24;
	s32 field28;
	s32 field2c;
	u8	unknown_30[8];
	s32 field38;
	s32 field3c;
	s32 field40;
};

struct Resource373Particle
{
	u8	unknown_00[0x28];
	s32 lifetime;
	u8	unknown_2c[4];
	s32 field30;
	s32 field34;
	u8	unknown_38[0x10];
	s32 field48;
	u8	unknown_4c[9];
	u8	field55;
};

struct StagedVerticalEffect
{
	s32	 pad0[2];
	s32	 f8;
	s32	 fc;
	s32	 f10;
	s32	 pad14;
	s32	 f18;
	s32	 f1c;
	s32	 pad20[17];
	s16	 f64;
	s16	 pad66;
	s32 *f68;
};

typedef struct OrbitingSceneObjectSprite
{
	u8 padding_00[5];
	u8 flags_05_low : 5;
	u8 flags_05_bit_5 : 1;
	u8 flags_05_high : 2;
	u8 padding_06[3];
	u8 flags_09_low : 2;
	u8 flags_09_mode : 2;
	u8 flags_09_high : 4;
	u8 padding_0a[18];
	u8 palette;
	u8 padding_1d[10];
	u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject
{
	u8						   padding_00[8];
	s32						   x;
	s32						   y;
	u8						   padding_10[19];
	u8						   flags_23;
	u8						   padding_24[12];
	s32						   orbit_angle;
	u8						   padding_34[4];
	s32						   orbit_center_x;
	s32						   orbit_center_y;
	u8						   padding_40[16];
	OrbitingSceneObjectSprite *sprite;
	u8						   padding_54;
	u8						   mode;
	u8						   state;
	u8						   padding_57[5];
	u8						   active;
	u8						   padding_5d[4];
	u8						   visible;
	u8						   padding_62[10];
	u32						   callback;
} OrbitingSceneObject;

extern struct SceneEvent gHaidiaMuraEvents3[];
extern s32 gJasmineAction[];

extern void *gEffectWork;

#endif
