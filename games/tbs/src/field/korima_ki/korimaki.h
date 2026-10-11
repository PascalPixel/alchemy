#include "resource.h"
#ifndef KORIMAKI_H
#define KORIMAKI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

#include "palette_scene.h"

struct PaletteEffectFrame
{
	s32 pad00[6];
	s32 progress;
	s32 pad1c[7];
	s32 limit;
	s32 second_limit;
	s32 third_limit;
};

struct EffectSprite
{
	u8 pad00[9];
	u8 flags;
	u8 pad0a[28];
	u8 state;
};

struct PaletteEffect
{
	u8					 pad00[0x18];
	s32					 progress;
	u8					 pad1c[7];
	u8					 flags;
	u8					 pad24[12];
	s32					 rate_x;
	s32					 rate_y;
	u8					 pad38[24];
	struct EffectSprite *sprite;
	u8					 pad54[1];
	u8					 mode;
};

struct OrbitingPaletteEffect
{
	s32 pad00[2];
	s32 x;
	s32 y;
	s32 z;
	s32 pad14;
	s32 angle_x;
	s32 angle_y;
	s32 pad20[6];
	s32 anchor_x;
	s32 anchor_y;
	s32 anchor_z;
	s32 pad44[3];
	u8 *owner;
	s32 pad54[4];
	s16 step;
	s16 heading;
};

struct PaletteSceneRecord
{
	u8	pad00[216];
	u16 values[1];
};

extern struct SceneEntrance gKorimaKiEntrances[];
extern s16 gKorimaKiRegions[];
extern u32 gKorimaKiExits[];
extern struct ScenePlacement gKorimaKiPlacements[];
extern struct SceneEvent gKorimaKiEvents[];

extern s32		 gKorimaKiSparkOrigin[3];
extern s32		 gKorimaKiSparkCount;
extern s32		 gKorimaKiSparkSound;
extern s32		 gKorimaKiTransitionStep;
extern s32 SceneAction_ActorOneEntry[];
extern s32 SceneAction_ActorTwoEntry[];
extern s32 SceneAction_ActorThreeEntry[];
extern s32 SceneAction_GroupFinish[];
extern s32 gKorimaKiEffectScript[];

#endif
