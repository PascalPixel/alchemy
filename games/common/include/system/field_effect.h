#ifndef ALCHEMY_FIELD_EFFECT_H
#define ALCHEMY_FIELD_EFFECT_H

#include "edition.h"
#include "field_event.h"

struct FieldEffect
{
	u8					unk00[8];
	s32					x;
	s32					y;
	s32					z;
	u8					unk14[4];
	s32					scaleX;
	s32					scaleY;
	u8					unk20[3];
	u8					priorityFlags;
	u8					unk24[0x0c];
	s32					scaleRateX;
	s32					scaleRateY;
	u8					unk38[0x0c];
	s32					velocityX;
	s32					velocityY;
	s32					velocityZ;
	struct FieldSprite *sprite;
	u8					unk54;
	u8					motionFlags;
	u8					unk56[3];
	u8					collisionFlags;
	u8					unk5a[0x0a];
	u16					spin;
	u8					unk66[2];
	s32					countdown;
	void (*update)(union FieldObject *object);
};

LAYOUT_OFFSET_GUARD(FieldEffect_ScaleRateX, struct FieldEffect, scaleRateX, 0x30);
LAYOUT_OFFSET_GUARD(FieldEffect_VelocityX, struct FieldEffect, velocityX, 0x44);
LAYOUT_OFFSET_GUARD(FieldEffect_Sprite, struct FieldEffect, sprite, 0x50);
LAYOUT_OFFSET_GUARD(FieldEffect_Spin, struct FieldEffect, spin, 0x64);
LAYOUT_OFFSET_GUARD(FieldEffect_Countdown, struct FieldEffect, countdown, 0x68);
LAYOUT_OFFSET_GUARD(FieldEffect_Update, struct FieldEffect, update, 0x6c);

union FieldObject
{
	struct FieldActor  actor;
	struct FieldEffect effect;
};

#define SCRIPT_WAIT(frames) 0, (frames)
#define SCRIPT_SET_POSITION(x, y, z) 2, (x), (y), (z)
#define SCRIPT_REPEAT(count, label) 12, (count), (label)
#define SCRIPT_DELETE 27

enum EffectSpawnFlags
{
	EFFECT_SCRIPT_MASK = 0x0f,
	EFFECT_USE_PALETTE = 0x10000,
	EFFECT_USE_PRIORITY = 0x20000,
	EFFECT_SCALE_TO_TARGET = 0x40000,
	EFFECT_USE_START_SCALE = 0x80000,
	EFFECT_USE_TYPE = 0x100000,
	EFFECT_USE_SCRIPT = 0x200000,
	EFFECT_USE_ROTATION = 0x400000,
	EFFECT_USE_SPIN = 0x800000,
	EFFECT_USE_UPDATE = 0x1000000
};

struct EffectOptions
{
	s32		   priority;
	s32		   palette;
	s32		   startScaleX;
	s32		   startScaleY;
	s32		   targetScaleX;
	s32		   targetScaleY;
	s16		   type;
	const s32 *script;
	u16		   rotation;
	u16		   spin;
	void (*update)(union FieldObject *object);
};

LAYOUT_OFFSET_GUARD(EffectOptions_Type, struct EffectOptions, type, 0x18);
LAYOUT_OFFSET_GUARD(EffectOptions_Update, struct EffectOptions, update, 0x24);

#if GAME_TLA
void				Effect_SetPriority(struct FieldEffect *effect, s32 priority);
struct FieldEffect *Effect_CreateTranslucent(s32 x, s32 y, s32 z, s32 type);
struct FieldEffect *Effect_Create(s32 x, s32 y, s32 z, s32 type);
void				BattleFx_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags,
	const struct EffectOptions *options);
#endif
void Effect_SpawnResident(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ,
	u32 flags, const struct EffectOptions *options);

#endif
